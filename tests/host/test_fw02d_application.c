#include <assert.h>
#include <stdio.h>

#include "homegrid/application.h"

static hg_switch_command_t make_on_command(void)
{
    hg_switch_command_t command = {
        HG_SWITCH_COMMAND_REQUEST_ON, HG_SWITCH_SOURCE_LOCAL, 100U
    };
    return command;
}

static void assert_safe(const hg_application_t *app)
{
    assert(app->switch_state.observed == HG_SWITCH_VALUE_UNKNOWN);
    assert(app->switch_state.output_permission == HG_SWITCH_OUTPUT_DENIED);
}

static void test_init_is_safe_and_mock_defaults_to_unavailable(void)
{
    hg_application_t app;
    assert(hg_application_init(&app) == HG_APPLICATION_STATUS_OK);
    assert(app.switch_state.desired == HG_SWITCH_VALUE_UNKNOWN);
    assert(app.switch_state.applied == HG_SWITCH_APPLIED_UNKNOWN);
    assert(app.mock_actuator.request_count == 0U);
    assert(app.mock_actuator.next_result == HG_SWITCH_ACTUATOR_UNAVAILABLE);
    assert_safe(&app);
}

static void test_unresolved_policy_never_calls_mock(void)
{
    hg_application_t app;
    hg_application_result_t result;
    hg_switch_command_t command = make_on_command();

    assert(hg_application_init(&app) == HG_APPLICATION_STATUS_OK);
    assert(hg_application_process_command(
        &app, &command, HG_SWITCH_POLICY_UNRESOLVED, &result) ==
        HG_APPLICATION_STATUS_COMMAND_REJECTED);
    assert(result.domain_status == HG_SWITCH_STATUS_POLICY_UNRESOLVED);
    assert(result.actuator_called == 0U);
    assert(app.mock_actuator.request_count == 0U);
    assert(app.switch_state.desired == HG_SWITCH_VALUE_UNKNOWN);
    assert_safe(&app);
}

static void test_denied_policy_never_calls_mock(void)
{
    hg_application_t app;
    hg_application_result_t result;
    hg_switch_command_t command = make_on_command();

    assert(hg_application_init(&app) == HG_APPLICATION_STATUS_OK);
    assert(hg_application_process_command(
        &app, &command, HG_SWITCH_POLICY_DENY, &result) ==
        HG_APPLICATION_STATUS_COMMAND_REJECTED);
    assert(result.domain_status == HG_SWITCH_STATUS_POLICY_DENIED);
    assert(result.actuator_called == 0U);
    assert(app.mock_actuator.request_count == 0U);
    assert_safe(&app);
}

static void test_allowed_command_is_recorded_by_mock_only(void)
{
    hg_application_t app;
    hg_application_result_t result;
    hg_switch_command_t command = make_on_command();

    assert(hg_application_init(&app) == HG_APPLICATION_STATUS_OK);
    assert(hg_application_set_mock_result(
        &app, HG_SWITCH_ACTUATOR_ACCEPTED_BY_MOCK) ==
        HG_APPLICATION_STATUS_OK);
    assert(hg_application_process_command(
        &app, &command, HG_SWITCH_POLICY_ALLOW, &result) ==
        HG_APPLICATION_STATUS_OK);

    assert(result.domain_status == HG_SWITCH_STATUS_OK);
    assert(result.actuator_called == 1U);
    assert(result.actuator_result == HG_SWITCH_ACTUATOR_ACCEPTED_BY_MOCK);
    assert(app.mock_actuator.request_count == 1U);
    assert(app.mock_actuator.last_requested_value == HG_SWITCH_VALUE_ON);
    assert(app.switch_state.desired == HG_SWITCH_VALUE_ON);
    assert(app.switch_state.applied == HG_SWITCH_APPLIED_ACCEPTED);
    assert(app.switch_state.observed == HG_SWITCH_VALUE_UNKNOWN);
    assert_safe(&app);
}

static void test_mock_rejection_is_reported_without_physical_claim(void)
{
    hg_application_t app;
    hg_application_result_t result;
    hg_switch_command_t command = make_on_command();

    assert(hg_application_init(&app) == HG_APPLICATION_STATUS_OK);
    assert(hg_application_set_mock_result(
        &app, HG_SWITCH_ACTUATOR_REJECTED_BY_MOCK) ==
        HG_APPLICATION_STATUS_OK);
    assert(hg_application_process_command(
        &app, &command, HG_SWITCH_POLICY_ALLOW, &result) ==
        HG_APPLICATION_STATUS_OK);

    assert(result.actuator_result == HG_SWITCH_ACTUATOR_REJECTED_BY_MOCK);
    assert(app.switch_state.applied == HG_SWITCH_APPLIED_REJECTED);
    assert(app.switch_state.fault == HG_SWITCH_FAULT_MOCK_REJECTED);
    assert(app.switch_state.observed == HG_SWITCH_VALUE_UNKNOWN);
    assert_safe(&app);
}

static void test_mock_unavailable_is_recorded(void)
{
    hg_application_t app;
    hg_application_result_t result;
    hg_switch_command_t command = make_on_command();

    assert(hg_application_init(&app) == HG_APPLICATION_STATUS_OK);
    assert(hg_application_process_command(
        &app, &command, HG_SWITCH_POLICY_ALLOW, &result) ==
        HG_APPLICATION_STATUS_OK);

    assert(result.actuator_result == HG_SWITCH_ACTUATOR_UNAVAILABLE);
    assert(app.switch_state.availability == HG_SWITCH_AVAILABILITY_UNAVAILABLE);
    assert(app.switch_state.fault == HG_SWITCH_FAULT_ACTUATOR_UNAVAILABLE);
    assert_safe(&app);
}

static void test_invalid_command_does_not_call_mock(void)
{
    hg_application_t app;
    hg_application_result_t result;
    hg_switch_command_t command = {
        (hg_switch_command_kind_t)99, HG_SWITCH_SOURCE_LOCAL, 101U
    };

    assert(hg_application_init(&app) == HG_APPLICATION_STATUS_OK);
    assert(hg_application_process_command(
        &app, &command, HG_SWITCH_POLICY_ALLOW, &result) ==
        HG_APPLICATION_STATUS_STATE_ERROR);
    assert(result.actuator_called == 0U);
    assert(app.mock_actuator.request_count == 0U);
    assert_safe(&app);
}

static void test_multiple_requests_are_counted_deterministically(void)
{
    hg_application_t app;
    hg_application_result_t result;
    hg_switch_command_t command = make_on_command();

    assert(hg_application_init(&app) == HG_APPLICATION_STATUS_OK);
    assert(hg_application_set_mock_result(
        &app, HG_SWITCH_ACTUATOR_ACCEPTED_BY_MOCK) ==
        HG_APPLICATION_STATUS_OK);
    assert(hg_application_process_command(
        &app, &command, HG_SWITCH_POLICY_ALLOW, &result) ==
        HG_APPLICATION_STATUS_OK);

    command.kind = HG_SWITCH_COMMAND_REQUEST_OFF;
    assert(hg_application_process_command(
        &app, &command, HG_SWITCH_POLICY_ALLOW, &result) ==
        HG_APPLICATION_STATUS_OK);
    assert(app.mock_actuator.request_count == 2U);
    assert(app.mock_actuator.last_requested_value == HG_SWITCH_VALUE_OFF);
    assert(app.switch_state.desired == HG_SWITCH_VALUE_OFF);
    assert_safe(&app);
}

int main(void)
{
    test_init_is_safe_and_mock_defaults_to_unavailable();
    test_unresolved_policy_never_calls_mock();
    test_denied_policy_never_calls_mock();
    test_allowed_command_is_recorded_by_mock_only();
    test_mock_rejection_is_reported_without_physical_claim();
    test_mock_unavailable_is_recorded();
    test_invalid_command_does_not_call_mock();
    test_multiple_requests_are_counted_deterministically();
    puts("FW-02D application and mock-actuator host tests passed.");
    return 0;
}
