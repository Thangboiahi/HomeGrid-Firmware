#include "homegrid/switch_state.h"

static int hg_state_is_valid(const hg_switch_state_t *state)
{
    if (state == 0) {
        return 0;
    }

    if (state->desired < HG_SWITCH_VALUE_UNKNOWN ||
        state->desired > HG_SWITCH_VALUE_ON ||
        state->applied < HG_SWITCH_APPLIED_UNKNOWN ||
        state->applied > HG_SWITCH_APPLIED_REJECTED ||
        state->observed != HG_SWITCH_VALUE_UNKNOWN ||
        state->availability < HG_SWITCH_AVAILABILITY_STARTING ||
        state->availability > HG_SWITCH_AVAILABILITY_UNAVAILABLE ||
        state->fault < HG_SWITCH_FAULT_NONE ||
        state->fault > HG_SWITCH_FAULT_INTERNAL ||
        state->output_permission != HG_SWITCH_OUTPUT_DENIED) {
        return 0;
    }

    return 1;
}

hg_switch_status_t hg_switch_state_init(hg_switch_state_t *state)
{
    if (state == 0) {
        return HG_SWITCH_STATUS_INVALID_ARGUMENT;
    }

    state->desired = HG_SWITCH_VALUE_UNKNOWN;
    state->applied = HG_SWITCH_APPLIED_UNKNOWN;
    state->observed = HG_SWITCH_VALUE_UNKNOWN;
    state->availability = HG_SWITCH_AVAILABILITY_STARTING;
    state->fault = HG_SWITCH_FAULT_NONE;
    state->output_permission = HG_SWITCH_OUTPUT_DENIED;
    state->revision = 0U;

    return HG_SWITCH_STATUS_OK;
}

hg_switch_status_t hg_switch_apply_command(
    hg_switch_state_t *state,
    const hg_switch_command_t *command,
    hg_switch_policy_result_t policy_result)
{
    hg_switch_value_t requested;

    if (state == 0 || command == 0) {
        return HG_SWITCH_STATUS_INVALID_ARGUMENT;
    }

    if (!hg_state_is_valid(state)) {
        return HG_SWITCH_STATUS_INVALID_STATE;
    }

    if (command->source < HG_SWITCH_SOURCE_LOCAL ||
        command->source > HG_SWITCH_SOURCE_INTERNAL) {
        return HG_SWITCH_STATUS_INVALID_ARGUMENT;
    }

    switch (command->kind) {
    case HG_SWITCH_COMMAND_REQUEST_OFF:
        requested = HG_SWITCH_VALUE_OFF;
        break;
    case HG_SWITCH_COMMAND_REQUEST_ON:
        requested = HG_SWITCH_VALUE_ON;
        break;
    default:
        return HG_SWITCH_STATUS_UNSUPPORTED_COMMAND;
    }

    switch (policy_result) {
    case HG_SWITCH_POLICY_UNRESOLVED:
        return HG_SWITCH_STATUS_POLICY_UNRESOLVED;
    case HG_SWITCH_POLICY_DENY:
        return HG_SWITCH_STATUS_POLICY_DENIED;
    case HG_SWITCH_POLICY_ALLOW:
        break;
    default:
        return HG_SWITCH_STATUS_INVALID_ARGUMENT;
    }

    state->desired = requested;
    state->revision++;
    return HG_SWITCH_STATUS_OK;
}

hg_switch_status_t hg_switch_record_mock_result(
    hg_switch_state_t *state,
    hg_switch_actuator_result_t result)
{
    if (state == 0) {
        return HG_SWITCH_STATUS_INVALID_ARGUMENT;
    }

    if (!hg_state_is_valid(state)) {
        return HG_SWITCH_STATUS_INVALID_STATE;
    }

    switch (result) {
    case HG_SWITCH_ACTUATOR_ACCEPTED_BY_MOCK:
        state->applied = HG_SWITCH_APPLIED_ACCEPTED;
        break;
    case HG_SWITCH_ACTUATOR_REJECTED_BY_MOCK:
        state->applied = HG_SWITCH_APPLIED_REJECTED;
        state->availability = HG_SWITCH_AVAILABILITY_DEGRADED;
        state->fault = HG_SWITCH_FAULT_MOCK_REJECTED;
        break;
    case HG_SWITCH_ACTUATOR_UNAVAILABLE:
        state->applied = HG_SWITCH_APPLIED_NOT_APPLIED;
        state->availability = HG_SWITCH_AVAILABILITY_UNAVAILABLE;
        state->fault = HG_SWITCH_FAULT_ACTUATOR_UNAVAILABLE;
        break;
    case HG_SWITCH_ACTUATOR_INTERNAL_ERROR:
        state->applied = HG_SWITCH_APPLIED_NOT_APPLIED;
        state->availability = HG_SWITCH_AVAILABILITY_DEGRADED;
        state->fault = HG_SWITCH_FAULT_INTERNAL;
        break;
    default:
        return HG_SWITCH_STATUS_INVALID_ARGUMENT;
    }

    /* FW-02 has no physical feedback and can never authorize output. */
    state->observed = HG_SWITCH_VALUE_UNKNOWN;
    state->output_permission = HG_SWITCH_OUTPUT_DENIED;
    state->revision++;

    return HG_SWITCH_STATUS_OK;
}
