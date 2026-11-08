#include <assert.h>
#include <stdio.h>

#include "homegrid/diagnostics.h"

static void test_initialization_and_snapshot(void)
{
    hg_diagnostics_t diagnostics;
    hg_diagnostics_snapshot_t snapshot;

    assert(hg_diagnostics_init(&diagnostics) == HG_DIAGNOSTICS_STATUS_OK);
    assert(hg_diagnostics_get_snapshot(&diagnostics, &snapshot) ==
           HG_DIAGNOSTICS_STATUS_OK);
    assert(snapshot.total_events == 0U);
    assert(snapshot.retained_events == 0U);
    assert(snapshot.dropped_events == 0U);
    assert(snapshot.last_fault == HG_SWITCH_FAULT_NONE);
}

static void test_records_counters_and_fault(void)
{
    hg_diagnostics_t diagnostics;
    hg_diagnostics_snapshot_t snapshot;
    hg_diagnostics_event_t event;

    assert(hg_diagnostics_init(&diagnostics) == HG_DIAGNOSTICS_STATUS_OK);
    assert(hg_diagnostics_record(
        &diagnostics, HG_DIAG_EVENT_COMMAND_REJECTED, 71U, 3U,
        HG_SWITCH_FAULT_NONE) == HG_DIAGNOSTICS_STATUS_OK);
    assert(hg_diagnostics_record(
        &diagnostics, HG_DIAG_EVENT_MOCK_UNAVAILABLE, 72U, 5U,
        HG_SWITCH_FAULT_ACTUATOR_UNAVAILABLE) == HG_DIAGNOSTICS_STATUS_OK);
    assert(hg_diagnostics_get_snapshot(&diagnostics, &snapshot) ==
           HG_DIAGNOSTICS_STATUS_OK);
    assert(snapshot.total_events == 2U);
    assert(snapshot.command_rejections == 1U);
    assert(snapshot.mock_unavailable == 1U);
    assert(snapshot.last_fault == HG_SWITCH_FAULT_ACTUATOR_UNAVAILABLE);
    assert(hg_diagnostics_get_event(&diagnostics, 0U, &event) ==
           HG_DIAGNOSTICS_STATUS_OK);
    assert(event.sequence == 1U);
    assert(event.correlation_id == 71U);
    assert(hg_diagnostics_get_event(&diagnostics, 1U, &event) ==
           HG_DIAGNOSTICS_STATUS_OK);
    assert(event.type == HG_DIAG_EVENT_MOCK_UNAVAILABLE);
    assert(event.state_revision == 5U);
}

static void test_ring_retains_newest_events_in_order(void)
{
    hg_diagnostics_t diagnostics;
    hg_diagnostics_snapshot_t snapshot;
    hg_diagnostics_event_t event;
    uint32_t index;

    assert(hg_diagnostics_init(&diagnostics) == HG_DIAGNOSTICS_STATUS_OK);
    for (index = 0U; index < 10U; index++) {
        assert(hg_diagnostics_record(
            &diagnostics, HG_DIAG_EVENT_STARTED, index, index,
            HG_SWITCH_FAULT_NONE) == HG_DIAGNOSTICS_STATUS_OK);
    }
    assert(hg_diagnostics_get_snapshot(&diagnostics, &snapshot) ==
           HG_DIAGNOSTICS_STATUS_OK);
    assert(snapshot.total_events == 10U);
    assert(snapshot.retained_events == HG_DIAGNOSTICS_EVENT_CAPACITY);
    assert(snapshot.dropped_events == 2U);
    assert(hg_diagnostics_get_event(&diagnostics, 0U, &event) ==
           HG_DIAGNOSTICS_STATUS_OK);
    assert(event.sequence == 3U);
    assert(event.correlation_id == 2U);
    assert(hg_diagnostics_get_event(
        &diagnostics, HG_DIAGNOSTICS_EVENT_CAPACITY - 1U, &event) ==
        HG_DIAGNOSTICS_STATUS_OK);
    assert(event.sequence == 10U);
    assert(event.correlation_id == 9U);
    assert(hg_diagnostics_get_event(
        &diagnostics, HG_DIAGNOSTICS_EVENT_CAPACITY, &event) ==
        HG_DIAGNOSTICS_STATUS_INVALID_ARGUMENT);
}

static void test_invalid_arguments_and_events_are_rejected(void)
{
    hg_diagnostics_t diagnostics;
    hg_diagnostics_event_t event;

    assert(hg_diagnostics_init(0) == HG_DIAGNOSTICS_STATUS_INVALID_ARGUMENT);
    assert(hg_diagnostics_record(
        0, HG_DIAG_EVENT_STARTED, 0U, 0U, HG_SWITCH_FAULT_NONE) ==
        HG_DIAGNOSTICS_STATUS_INVALID_ARGUMENT);
    assert(hg_diagnostics_init(&diagnostics) == HG_DIAGNOSTICS_STATUS_OK);
    assert(hg_diagnostics_record(
        &diagnostics, (hg_diagnostics_event_type_t)99, 0U, 0U,
        HG_SWITCH_FAULT_NONE) == HG_DIAGNOSTICS_STATUS_INVALID_EVENT);
    assert(hg_diagnostics_record(
        &diagnostics, HG_DIAG_EVENT_STARTED, 0U, 0U,
        (hg_switch_fault_t)99) == HG_DIAGNOSTICS_STATUS_INVALID_EVENT);
    assert(hg_diagnostics_get_event(&diagnostics, 0U, &event) ==
           HG_DIAGNOSTICS_STATUS_INVALID_ARGUMENT);
}

int main(void)
{
    test_initialization_and_snapshot();
    test_records_counters_and_fault();
    test_ring_retains_newest_events_in_order();
    test_invalid_arguments_and_events_are_rejected();
    puts("FW-02E diagnostics host tests passed.");
    return 0;
}
