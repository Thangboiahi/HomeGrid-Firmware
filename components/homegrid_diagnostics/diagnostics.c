#include "homegrid/diagnostics.h"

static int hg_diagnostics_event_is_valid(hg_diagnostics_event_type_t type)
{
    switch (type) {
    case HG_DIAG_EVENT_STARTED:
    case HG_DIAG_EVENT_COMMAND_REJECTED:
    case HG_DIAG_EVENT_STATE_ERROR:
    case HG_DIAG_EVENT_MOCK_ACCEPTED:
    case HG_DIAG_EVENT_MOCK_REJECTED:
    case HG_DIAG_EVENT_MOCK_UNAVAILABLE:
    case HG_DIAG_EVENT_MOCK_INTERNAL_ERROR:
        return 1;
    default:
        return 0;
    }
}

static int hg_diagnostics_fault_is_valid(hg_switch_fault_t fault)
{
    switch (fault) {
    case HG_SWITCH_FAULT_NONE:
    case HG_SWITCH_FAULT_MOCK_REJECTED:
    case HG_SWITCH_FAULT_ACTUATOR_UNAVAILABLE:
    case HG_SWITCH_FAULT_INTERNAL:
        return 1;
    default:
        return 0;
    }
}

static void hg_increment_saturated(uint32_t *value)
{
    if (*value < UINT32_MAX) {
        (*value)++;
    }
}

hg_diagnostics_status_t hg_diagnostics_init(hg_diagnostics_t *diagnostics)
{
    uint8_t index;

    if (diagnostics == 0) {
        return HG_DIAGNOSTICS_STATUS_INVALID_ARGUMENT;
    }

    for (index = 0U; index < HG_DIAGNOSTICS_EVENT_CAPACITY; index++) {
        diagnostics->events[index].sequence = 0U;
        diagnostics->events[index].type = HG_DIAG_EVENT_STARTED;
        diagnostics->events[index].correlation_id = 0U;
        diagnostics->events[index].state_revision = 0U;
        diagnostics->events[index].fault = HG_SWITCH_FAULT_NONE;
    }

    diagnostics->snapshot.total_events = 0U;
    diagnostics->snapshot.dropped_events = 0U;
    diagnostics->snapshot.command_rejections = 0U;
    diagnostics->snapshot.state_errors = 0U;
    diagnostics->snapshot.mock_acceptances = 0U;
    diagnostics->snapshot.mock_rejections = 0U;
    diagnostics->snapshot.mock_unavailable = 0U;
    diagnostics->snapshot.mock_internal_errors = 0U;
    diagnostics->snapshot.last_fault = HG_SWITCH_FAULT_NONE;
    diagnostics->snapshot.retained_events = 0U;
    diagnostics->next_index = 0U;

    return HG_DIAGNOSTICS_STATUS_OK;
}

hg_diagnostics_status_t hg_diagnostics_record(
    hg_diagnostics_t *diagnostics,
    hg_diagnostics_event_type_t type,
    uint32_t correlation_id,
    uint32_t state_revision,
    hg_switch_fault_t fault)
{
    hg_diagnostics_event_t *event;

    if (diagnostics == 0) {
        return HG_DIAGNOSTICS_STATUS_INVALID_ARGUMENT;
    }
    if (!hg_diagnostics_event_is_valid(type) ||
        !hg_diagnostics_fault_is_valid(fault)) {
        return HG_DIAGNOSTICS_STATUS_INVALID_EVENT;
    }

    event = &diagnostics->events[diagnostics->next_index];
    if (diagnostics->snapshot.retained_events == HG_DIAGNOSTICS_EVENT_CAPACITY) {
        hg_increment_saturated(&diagnostics->snapshot.dropped_events);
    } else {
        diagnostics->snapshot.retained_events++;
    }

    hg_increment_saturated(&diagnostics->snapshot.total_events);
    event->sequence = diagnostics->snapshot.total_events;
    event->type = type;
    event->correlation_id = correlation_id;
    event->state_revision = state_revision;
    event->fault = fault;

    switch (type) {
    case HG_DIAG_EVENT_COMMAND_REJECTED:
        hg_increment_saturated(&diagnostics->snapshot.command_rejections);
        break;
    case HG_DIAG_EVENT_STATE_ERROR:
        hg_increment_saturated(&diagnostics->snapshot.state_errors);
        break;
    case HG_DIAG_EVENT_MOCK_ACCEPTED:
        hg_increment_saturated(&diagnostics->snapshot.mock_acceptances);
        break;
    case HG_DIAG_EVENT_MOCK_REJECTED:
        hg_increment_saturated(&diagnostics->snapshot.mock_rejections);
        break;
    case HG_DIAG_EVENT_MOCK_UNAVAILABLE:
        hg_increment_saturated(&diagnostics->snapshot.mock_unavailable);
        break;
    case HG_DIAG_EVENT_MOCK_INTERNAL_ERROR:
        hg_increment_saturated(&diagnostics->snapshot.mock_internal_errors);
        break;
    case HG_DIAG_EVENT_STARTED:
        break;
    default:
        return HG_DIAGNOSTICS_STATUS_INVALID_EVENT;
    }

    if (fault != HG_SWITCH_FAULT_NONE) {
        diagnostics->snapshot.last_fault = fault;
    }
    diagnostics->next_index =
        (uint8_t)((diagnostics->next_index + 1U) % HG_DIAGNOSTICS_EVENT_CAPACITY);

    return HG_DIAGNOSTICS_STATUS_OK;
}

hg_diagnostics_status_t hg_diagnostics_get_snapshot(
    const hg_diagnostics_t *diagnostics,
    hg_diagnostics_snapshot_t *snapshot)
{
    if (diagnostics == 0 || snapshot == 0) {
        return HG_DIAGNOSTICS_STATUS_INVALID_ARGUMENT;
    }
    *snapshot = diagnostics->snapshot;
    return HG_DIAGNOSTICS_STATUS_OK;
}

hg_diagnostics_status_t hg_diagnostics_get_event(
    const hg_diagnostics_t *diagnostics,
    uint8_t offset,
    hg_diagnostics_event_t *event)
{
    uint8_t oldest_index;

    if (diagnostics == 0 || event == 0) {
        return HG_DIAGNOSTICS_STATUS_INVALID_ARGUMENT;
    }
    if (offset >= diagnostics->snapshot.retained_events) {
        return HG_DIAGNOSTICS_STATUS_INVALID_ARGUMENT;
    }

    oldest_index = (uint8_t)(
        (diagnostics->next_index + HG_DIAGNOSTICS_EVENT_CAPACITY -
         diagnostics->snapshot.retained_events) % HG_DIAGNOSTICS_EVENT_CAPACITY);
    *event = diagnostics->events[
        (uint8_t)((oldest_index + offset) % HG_DIAGNOSTICS_EVENT_CAPACITY)];
    return HG_DIAGNOSTICS_STATUS_OK;
}
