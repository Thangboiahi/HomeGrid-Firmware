#ifndef HOMEGRID_MOCK_IO_H
#define HOMEGRID_MOCK_IO_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    HG_MOCK_IO_OK = 0,
    HG_MOCK_IO_INVALID_ARGUMENT,
    HG_MOCK_IO_INVALID_CONFIGURATION
} hg_mock_io_status_t;

typedef enum {
    HG_MOCK_IO_SWITCH = 0,
    HG_MOCK_IO_RELAY,
    HG_MOCK_IO_FAN
} hg_mock_io_kind_t;

typedef enum {
    HG_MOCK_IO_RESULT_ACCEPTED = 0,
    HG_MOCK_IO_RESULT_REJECTED,
    HG_MOCK_IO_RESULT_UNAVAILABLE,
    HG_MOCK_IO_RESULT_BUSY,
    HG_MOCK_IO_RESULT_TIMEOUT,
    HG_MOCK_IO_RESULT_FAULT,
    HG_MOCK_IO_RESULT_INVALID_CONFIGURATION,
    HG_MOCK_IO_RESULT_OUTPUT_DENIED,
    HG_MOCK_IO_RESULT_INTERNAL_ERROR
} hg_mock_io_result_t;

typedef enum {
    HG_MOCK_FEEDBACK_VALID = 0,
    HG_MOCK_FEEDBACK_MISSING,
    HG_MOCK_FEEDBACK_STALE,
    HG_MOCK_FEEDBACK_INVALID,
    HG_MOCK_FEEDBACK_CONTRADICTORY,
    HG_MOCK_FEEDBACK_UNAVAILABLE
} hg_mock_feedback_status_t;

typedef enum {
    HG_MOCK_VALUE_UNKNOWN = 0,
    HG_MOCK_VALUE_OFF,
    HG_MOCK_VALUE_ON
} hg_mock_value_t;

typedef struct {
    uint32_t request_count;
    uint32_t last_channel_id;
    hg_mock_io_kind_t last_kind;
    hg_mock_value_t last_value;
    hg_mock_io_result_t next_result;
    uint8_t configured;
} hg_mock_io_t;

typedef struct {
    hg_mock_feedback_status_t status;
    hg_mock_value_t value;
    uint32_t age_ms;
} hg_mock_feedback_t;

hg_mock_io_status_t hg_mock_io_init(hg_mock_io_t *mock);
hg_mock_io_status_t hg_mock_io_configure(hg_mock_io_t *mock, uint8_t valid);
hg_mock_io_status_t hg_mock_io_set_next_result(hg_mock_io_t *mock,
                                                hg_mock_io_result_t result);
hg_mock_io_status_t hg_mock_io_submit(hg_mock_io_t *mock,
                                      hg_mock_io_kind_t kind,
                                      uint32_t channel_id,
                                      hg_mock_value_t requested,
                                      hg_mock_io_result_t *result);
hg_mock_io_status_t hg_mock_feedback_set(hg_mock_feedback_t *feedback,
                                         hg_mock_feedback_status_t status,
                                         hg_mock_value_t value,
                                         uint32_t age_ms);
hg_mock_value_t hg_mock_feedback_observed(const hg_mock_feedback_t *feedback,
                                          uint32_t max_age_ms);

#ifdef __cplusplus
}
#endif
#endif
