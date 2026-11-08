#include "homegrid/mock_actuator.h"

static int hg_mock_result_is_valid(hg_switch_actuator_result_t result)
{
    switch (result) {
    case HG_SWITCH_ACTUATOR_ACCEPTED_BY_MOCK:
    case HG_SWITCH_ACTUATOR_REJECTED_BY_MOCK:
    case HG_SWITCH_ACTUATOR_UNAVAILABLE:
    case HG_SWITCH_ACTUATOR_INTERNAL_ERROR:
        return 1;
    default:
        return 0;
    }
}

hg_switch_status_t hg_mock_actuator_init(hg_mock_actuator_t *mock)
{
    if (mock == 0) {
        return HG_SWITCH_STATUS_INVALID_ARGUMENT;
    }

    mock->request_count = 0U;
    mock->last_requested_value = HG_SWITCH_VALUE_UNKNOWN;
    /* Safe default: no request is treated as accepted unless configured. */
    mock->next_result = HG_SWITCH_ACTUATOR_UNAVAILABLE;
    return HG_SWITCH_STATUS_OK;
}

hg_switch_status_t hg_mock_actuator_set_result(
    hg_mock_actuator_t *mock,
    hg_switch_actuator_result_t result)
{
    if (mock == 0) {
        return HG_SWITCH_STATUS_INVALID_ARGUMENT;
    }
    if (!hg_mock_result_is_valid(result)) {
        return HG_SWITCH_STATUS_INVALID_ARGUMENT;
    }

    mock->next_result = result;
    return HG_SWITCH_STATUS_OK;
}

hg_switch_status_t hg_mock_actuator_submit(
    hg_mock_actuator_t *mock,
    hg_switch_value_t requested_value,
    hg_switch_actuator_result_t *result)
{
    if (mock == 0 || result == 0) {
        return HG_SWITCH_STATUS_INVALID_ARGUMENT;
    }
    if (requested_value != HG_SWITCH_VALUE_OFF &&
        requested_value != HG_SWITCH_VALUE_ON) {
        return HG_SWITCH_STATUS_INVALID_ARGUMENT;
    }

    mock->last_requested_value = requested_value;
    if (mock->request_count < UINT32_MAX) {
        mock->request_count++;
    }
    *result = mock->next_result;
    return HG_SWITCH_STATUS_OK;
}
