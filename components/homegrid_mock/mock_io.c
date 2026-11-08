#include "homegrid/mock_io.h"

static int result_valid(hg_mock_io_result_t result)
{
    return result >= HG_MOCK_IO_RESULT_ACCEPTED &&
           result <= HG_MOCK_IO_RESULT_INTERNAL_ERROR;
}

hg_mock_io_status_t hg_mock_io_init(hg_mock_io_t *mock)
{
    if (mock == 0) return HG_MOCK_IO_INVALID_ARGUMENT;
    mock->request_count = 0U;
    mock->last_channel_id = 0U;
    mock->last_kind = HG_MOCK_IO_SWITCH;
    mock->last_value = HG_MOCK_VALUE_UNKNOWN;
    mock->next_result = HG_MOCK_IO_RESULT_UNAVAILABLE;
    mock->configured = 0U;
    return HG_MOCK_IO_OK;
}

hg_mock_io_status_t hg_mock_io_configure(hg_mock_io_t *mock, uint8_t valid)
{
    if (mock == 0) return HG_MOCK_IO_INVALID_ARGUMENT;
    if (valid > 1U) return HG_MOCK_IO_INVALID_CONFIGURATION;
    mock->configured = valid;
    if (valid == 0U) mock->next_result = HG_MOCK_IO_RESULT_INVALID_CONFIGURATION;
    return HG_MOCK_IO_OK;
}

hg_mock_io_status_t hg_mock_io_set_next_result(hg_mock_io_t *mock,
                                                hg_mock_io_result_t result)
{
    if (mock == 0) return HG_MOCK_IO_INVALID_ARGUMENT;
    if (!result_valid(result)) return HG_MOCK_IO_INVALID_CONFIGURATION;
    mock->next_result = result;
    return HG_MOCK_IO_OK;
}

hg_mock_io_status_t hg_mock_io_submit(hg_mock_io_t *mock,
                                      hg_mock_io_kind_t kind,
                                      uint32_t channel_id,
                                      hg_mock_value_t requested,
                                      hg_mock_io_result_t *result)
{
    if (mock == 0 || result == 0) return HG_MOCK_IO_INVALID_ARGUMENT;
    if (kind < HG_MOCK_IO_SWITCH || kind > HG_MOCK_IO_FAN ||
        (requested != HG_MOCK_VALUE_OFF && requested != HG_MOCK_VALUE_ON)) {
        return HG_MOCK_IO_INVALID_CONFIGURATION;
    }
    if (mock->configured == 0U) {
        *result = HG_MOCK_IO_RESULT_INVALID_CONFIGURATION;
        return HG_MOCK_IO_OK;
    }
    mock->last_kind = kind;
    mock->last_channel_id = channel_id;
    mock->last_value = requested;
    if (mock->request_count < UINT32_MAX) mock->request_count++;
    *result = mock->next_result;
    return HG_MOCK_IO_OK;
}

hg_mock_io_status_t hg_mock_feedback_set(hg_mock_feedback_t *feedback,
                                         hg_mock_feedback_status_t status,
                                         hg_mock_value_t value,
                                         uint32_t age_ms)
{
    if (feedback == 0) return HG_MOCK_IO_INVALID_ARGUMENT;
    if (status < HG_MOCK_FEEDBACK_VALID ||
        status > HG_MOCK_FEEDBACK_UNAVAILABLE ||
        value < HG_MOCK_VALUE_UNKNOWN || value > HG_MOCK_VALUE_ON) {
        return HG_MOCK_IO_INVALID_CONFIGURATION;
    }
    feedback->status = status;
    feedback->value = value;
    feedback->age_ms = age_ms;
    return HG_MOCK_IO_OK;
}

hg_mock_value_t hg_mock_feedback_observed(const hg_mock_feedback_t *feedback,
                                          uint32_t max_age_ms)
{
    if (feedback == 0 || feedback->status != HG_MOCK_FEEDBACK_VALID ||
        feedback->value == HG_MOCK_VALUE_UNKNOWN ||
        feedback->age_ms > max_age_ms) {
        return HG_MOCK_VALUE_UNKNOWN;
    }
    return feedback->value;
}
