#include <assert.h>
#include <stdio.h>

#include "homegrid/switch_state.h"

static void assert_safe_invariants(const hg_switch_state_t *state)
{
    assert(state->observed == HG_SWITCH_VALUE_UNKNOWN);
    assert(state->output_permission == HG_SWITCH_OUTPUT_DENIED);
}

static void test_startup_is_unknown_and_denied(void)
{
    hg_switch_state_t state;
    assert(hg_switch_state_init(&state) == HG_SWITCH_STATUS_OK);
    assert(state.desired == HG_SWITCH_VALUE_UNKNOWN);
    assert(state.applied == HG_SWITCH_APPLIED_UNKNOWN);
    assert(state.observed == HG_SWITCH_VALUE_UNKNOWN);
    assert(state.availability == HG_SWITCH_AVAILABILITY_STARTING);
    assert(state.fault == HG_SWITCH_FAULT_NONE);
    assert(state.revision == 0U);
    assert_safe_invariants(&state);
}

static void test_unresolved_policy_does_not_mutate_state(void)
{
    hg_switch_state_t state;
    hg_switch_command_t command = {
        HG_SWITCH_COMMAND_REQUEST_ON, HG_SWITCH_SOURCE_REMOTE, 42U
    };
    assert(hg_switch_state_init(&state) == HG_SWITCH_STATUS_OK);
    assert(hg_switch_apply_command(
        &state, &command, HG_SWITCH_POLICY_UNRESOLVED) ==
        HG_SWITCH_STATUS_POLICY_UNRESOLVED);
    assert(state.desired == HG_SWITCH_VALUE_UNKNOWN);
    assert(state.revision == 0U);
    assert_safe_invariants(&state);
}

static void test_denied_policy_does_not_mutate_state(void)
{
    hg_switch_state_t state;
    hg_switch_command_t command = {
        HG_SWITCH_COMMAND_REQUEST_OFF, HG_SWITCH_SOURCE_LOCAL, 43U
    };
    assert(hg_switch_state_init(&state) == HG_SWITCH_STATUS_OK);
    assert(hg_switch_apply_command(
        &state, &command, HG_SWITCH_POLICY_DENY) ==
        HG_SWITCH_STATUS_POLICY_DENIED);
    assert(state.desired == HG_SWITCH_VALUE_UNKNOWN);
    assert(state.revision == 0U);
    assert_safe_invariants(&state);
}

static void test_allowed_command_changes_only_desired_state(void)
{
    hg_switch_state_t state;
    hg_switch_command_t command = {
        HG_SWITCH_COMMAND_REQUEST_ON, HG_SWITCH_SOURCE_INTERNAL, 44U
    };
    assert(hg_switch_state_init(&state) == HG_SWITCH_STATUS_OK);
    assert(hg_switch_apply_command(
        &state, &command, HG_SWITCH_POLICY_ALLOW) == HG_SWITCH_STATUS_OK);
    assert(state.desired == HG_SWITCH_VALUE_ON);
    assert(state.applied == HG_SWITCH_APPLIED_UNKNOWN);
    assert(state.observed == HG_SWITCH_VALUE_UNKNOWN);
    assert(state.revision == 1U);
    assert_safe_invariants(&state);
}

static void test_mock_acceptance_is_not_physical_observation(void)
{
    hg_switch_state_t state;
    assert(hg_switch_state_init(&state) == HG_SWITCH_STATUS_OK);
    assert(hg_switch_record_mock_result(
        &state, HG_SWITCH_ACTUATOR_ACCEPTED_BY_MOCK) == HG_SWITCH_STATUS_OK);
    assert(state.applied == HG_SWITCH_APPLIED_ACCEPTED);
    assert(state.observed == HG_SWITCH_VALUE_UNKNOWN);
    assert(state.output_permission == HG_SWITCH_OUTPUT_DENIED);
    assert_safe_invariants(&state);
}

static void test_mock_rejection_sets_degraded_fault(void)
{
    hg_switch_state_t state;
    assert(hg_switch_state_init(&state) == HG_SWITCH_STATUS_OK);
    assert(hg_switch_record_mock_result(
        &state, HG_SWITCH_ACTUATOR_REJECTED_BY_MOCK) == HG_SWITCH_STATUS_OK);
    assert(state.applied == HG_SWITCH_APPLIED_REJECTED);
    assert(state.availability == HG_SWITCH_AVAILABILITY_DEGRADED);
    assert(state.fault == HG_SWITCH_FAULT_MOCK_REJECTED);
    assert_safe_invariants(&state);
}

static void test_mock_unavailable_sets_unavailable_fault(void)
{
    hg_switch_state_t state;
    assert(hg_switch_state_init(&state) == HG_SWITCH_STATUS_OK);
    assert(hg_switch_record_mock_result(
        &state, HG_SWITCH_ACTUATOR_UNAVAILABLE) == HG_SWITCH_STATUS_OK);
    assert(state.applied == HG_SWITCH_APPLIED_NOT_APPLIED);
    assert(state.availability == HG_SWITCH_AVAILABILITY_UNAVAILABLE);
    assert(state.fault == HG_SWITCH_FAULT_ACTUATOR_UNAVAILABLE);
    assert_safe_invariants(&state);
}

static void test_invalid_command_is_rejected_without_mutation(void)
{
    hg_switch_state_t state;
    hg_switch_command_t command = {
        (hg_switch_command_kind_t)99, HG_SWITCH_SOURCE_LOCAL, 45U
    };
    assert(hg_switch_state_init(&state) == HG_SWITCH_STATUS_OK);
    assert(hg_switch_apply_command(
        &state, &command, HG_SWITCH_POLICY_ALLOW) ==
        HG_SWITCH_STATUS_UNSUPPORTED_COMMAND);
    assert(state.desired == HG_SWITCH_VALUE_UNKNOWN);
    assert(state.revision == 0U);
    assert_safe_invariants(&state);
}

int main(void)
{
    test_startup_is_unknown_and_denied();
    test_unresolved_policy_does_not_mutate_state();
    test_denied_policy_does_not_mutate_state();
    test_allowed_command_changes_only_desired_state();
    test_mock_acceptance_is_not_physical_observation();
    test_mock_rejection_sets_degraded_fault();
    test_mock_unavailable_sets_unavailable_fault();
    test_invalid_command_is_rejected_without_mutation();
    puts("FW-02C switch-domain host tests passed.");
    return 0;
}
