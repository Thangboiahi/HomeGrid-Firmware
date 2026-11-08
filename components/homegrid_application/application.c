#include "homegrid/application.h"

static hg_diagnostics_event_type_t hg_event_for_mock_result(
    hg_switch_actuator_result_t result)
{
    switch (result) {
    case HG_SWITCH_ACTUATOR_ACCEPTED_BY_MOCK:
        return HG_DIAG_EVENT_MOCK_ACCEPTED;
    case HG_SWITCH_ACTUATOR_REJECTED_BY_MOCK:
        return HG_DIAG_EVENT_MOCK_REJECTED;
    case HG_SWITCH_ACTUATOR_UNAVAILABLE:
        return HG_DIAG_EVENT_MOCK_UNAVAILABLE;
    case HG_SWITCH_ACTUATOR_INTERNAL_ERROR:
        return HG_DIAG_EVENT_MOCK_INTERNAL_ERROR;
    default:
        return HG_DIAG_EVENT_STATE_ERROR;
    }
}

static void hg_record_diagnostic(
    hg_application_t *application,
    hg_diagnostics_event_type_t type,
    uint32_t correlation_id)
{
    (void)hg_diagnostics_record(
        &application->diagnostics,
        type,
        correlation_id,
        application->switch_state.revision,
        application->switch_state.fault);
}

hg_application_status_t hg_application_init(hg_application_t *application)
{
    if (application == 0) {
        return HG_APPLICATION_STATUS_INVALID_ARGUMENT;
    }

    if (hg_switch_state_init(&application->switch_state) !=
        HG_SWITCH_STATUS_OK) {
        return HG_APPLICATION_STATUS_INITIALIZATION_ERROR;
    }
    if (hg_mock_actuator_init(&application->mock_actuator) !=
        HG_SWITCH_STATUS_OK) {
        return HG_APPLICATION_STATUS_INITIALIZATION_ERROR;
    }
    if (hg_diagnostics_init(&application->diagnostics) !=
        HG_DIAGNOSTICS_STATUS_OK) {
        return HG_APPLICATION_STATUS_INITIALIZATION_ERROR;
    }

    hg_record_diagnostic(application, HG_DIAG_EVENT_STARTED, 0U);
    return HG_APPLICATION_STATUS_OK;
}

hg_application_status_t hg_application_set_mock_result(
    hg_application_t *application,
    hg_switch_actuator_result_t result)
{
    if (application == 0) {
        return HG_APPLICATION_STATUS_INVALID_ARGUMENT;
    }
    if (hg_mock_actuator_set_result(&application->mock_actuator, result) !=
        HG_SWITCH_STATUS_OK) {
        return HG_APPLICATION_STATUS_INVALID_ARGUMENT;
    }
    return HG_APPLICATION_STATUS_OK;
}

hg_application_status_t hg_application_process_command(
    hg_application_t *application,
    const hg_switch_command_t *command,
    hg_switch_policy_result_t policy,
    hg_application_result_t *result)
{
    hg_switch_status_t domain_status;
    hg_switch_status_t mock_status;

    if (application == 0 || command == 0 || result == 0) {
        return HG_APPLICATION_STATUS_INVALID_ARGUMENT;
    }

    result->domain_status = HG_SWITCH_STATUS_INVALID_ARGUMENT;
    result->actuator_called = 0U;
    result->actuator_result = HG_SWITCH_ACTUATOR_UNAVAILABLE;

    domain_status = hg_switch_apply_command(
        &application->switch_state, command, policy);
    result->domain_status = domain_status;

    if (domain_status == HG_SWITCH_STATUS_POLICY_DENIED ||
        domain_status == HG_SWITCH_STATUS_POLICY_UNRESOLVED) {
        hg_record_diagnostic(
            application, HG_DIAG_EVENT_COMMAND_REJECTED,
            command->correlation_id);
        return HG_APPLICATION_STATUS_COMMAND_REJECTED;
    }
    if (domain_status != HG_SWITCH_STATUS_OK) {
        hg_record_diagnostic(
            application, HG_DIAG_EVENT_STATE_ERROR,
            command->correlation_id);
        return HG_APPLICATION_STATUS_STATE_ERROR;
    }

    mock_status = hg_mock_actuator_submit(
        &application->mock_actuator,
        application->switch_state.desired,
        &result->actuator_result);
    if (mock_status != HG_SWITCH_STATUS_OK) {
        hg_record_diagnostic(
            application, HG_DIAG_EVENT_STATE_ERROR,
            command->correlation_id);
        return HG_APPLICATION_STATUS_MOCK_ERROR;
    }

    result->actuator_called = 1U;
    if (hg_switch_record_mock_result(
            &application->switch_state, result->actuator_result) !=
        HG_SWITCH_STATUS_OK) {
        hg_record_diagnostic(
            application, HG_DIAG_EVENT_STATE_ERROR,
            command->correlation_id);
        return HG_APPLICATION_STATUS_STATE_ERROR;
    }

    hg_record_diagnostic(
        application,
        hg_event_for_mock_result(result->actuator_result),
        command->correlation_id);

    return HG_APPLICATION_STATUS_OK;
}
