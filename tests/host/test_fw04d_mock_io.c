#include <assert.h>
#include <stdio.h>
#include "homegrid/mock_io.h"

static void test_safe_default_and_invalid_configuration(void)
{
    hg_mock_io_t mock;
    hg_mock_io_result_t result = HG_MOCK_IO_RESULT_ACCEPTED;
    assert(hg_mock_io_init(&mock) == HG_MOCK_IO_OK);
    assert(mock.configured == 0U);
    assert(mock.next_result == HG_MOCK_IO_RESULT_UNAVAILABLE);
    assert(hg_mock_io_submit(&mock, HG_MOCK_IO_RELAY, 1U,
        HG_MOCK_VALUE_ON, &result) == HG_MOCK_IO_OK);
    assert(result == HG_MOCK_IO_RESULT_INVALID_CONFIGURATION);
    assert(mock.request_count == 0U);
    assert(hg_mock_io_configure(&mock, 2U) == HG_MOCK_IO_INVALID_CONFIGURATION);
}

static void test_deterministic_mock_request_is_not_physical_observation(void)
{
    hg_mock_io_t mock;
    hg_mock_io_result_t result;
    assert(hg_mock_io_init(&mock) == HG_MOCK_IO_OK);
    assert(hg_mock_io_configure(&mock, 1U) == HG_MOCK_IO_OK);
    assert(hg_mock_io_set_next_result(&mock, HG_MOCK_IO_RESULT_ACCEPTED) == HG_MOCK_IO_OK);
    assert(hg_mock_io_submit(&mock, HG_MOCK_IO_FAN, 7U,
        HG_MOCK_VALUE_ON, &result) == HG_MOCK_IO_OK);
    assert(result == HG_MOCK_IO_RESULT_ACCEPTED);
    assert(mock.request_count == 1U);
    assert(mock.last_kind == HG_MOCK_IO_FAN);
    assert(mock.last_channel_id == 7U);
    assert(mock.last_value == HG_MOCK_VALUE_ON);
}

static void test_result_error_injection(void)
{
    hg_mock_io_t mock;
    hg_mock_io_result_t result;
    assert(hg_mock_io_init(&mock) == HG_MOCK_IO_OK);
    assert(hg_mock_io_configure(&mock, 1U) == HG_MOCK_IO_OK);
    assert(hg_mock_io_set_next_result(&mock, HG_MOCK_IO_RESULT_TIMEOUT) == HG_MOCK_IO_OK);
    assert(hg_mock_io_submit(&mock, HG_MOCK_IO_SWITCH, 2U,
        HG_MOCK_VALUE_OFF, &result) == HG_MOCK_IO_OK);
    assert(result == HG_MOCK_IO_RESULT_TIMEOUT);
    assert(hg_mock_io_set_next_result(&mock, (hg_mock_io_result_t)99) ==
        HG_MOCK_IO_INVALID_CONFIGURATION);
}

static void test_only_valid_fresh_feedback_becomes_observed(void)
{
    hg_mock_feedback_t feedback;
    assert(hg_mock_feedback_set(&feedback, HG_MOCK_FEEDBACK_VALID,
        HG_MOCK_VALUE_ON, 20U) == HG_MOCK_IO_OK);
    assert(hg_mock_feedback_observed(&feedback, 100U) == HG_MOCK_VALUE_ON);
    assert(hg_mock_feedback_observed(&feedback, 10U) == HG_MOCK_VALUE_UNKNOWN);
    assert(hg_mock_feedback_set(&feedback, HG_MOCK_FEEDBACK_MISSING,
        HG_MOCK_VALUE_OFF, 0U) == HG_MOCK_IO_OK);
    assert(hg_mock_feedback_observed(&feedback, 100U) == HG_MOCK_VALUE_UNKNOWN);
    assert(hg_mock_feedback_set(&feedback, HG_MOCK_FEEDBACK_CONTRADICTORY,
        HG_MOCK_VALUE_ON, 0U) == HG_MOCK_IO_OK);
    assert(hg_mock_feedback_observed(&feedback, 100U) == HG_MOCK_VALUE_UNKNOWN);
    assert(hg_mock_feedback_set(&feedback, HG_MOCK_FEEDBACK_INVALID,
        HG_MOCK_VALUE_OFF, 0U) == HG_MOCK_IO_OK);
    assert(hg_mock_feedback_observed(&feedback, 100U) == HG_MOCK_VALUE_UNKNOWN);
}

int main(void)
{
    test_safe_default_and_invalid_configuration();
    test_deterministic_mock_request_is_not_physical_observation();
    test_result_error_injection();
    test_only_valid_fresh_feedback_becomes_observed();
    puts("FW-04D mock IO tests passed");
    return 0;
}
