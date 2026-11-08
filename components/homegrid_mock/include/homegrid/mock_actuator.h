/*
 * FW-02D deterministic host/mock actuator.
 *
 * This adapter records software requests only. It has no hardware access.
 */
#ifndef HOMEGRID_MOCK_ACTUATOR_H
#define HOMEGRID_MOCK_ACTUATOR_H

#include <stdint.h>

#include "homegrid/switch_state.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t request_count;
    hg_switch_value_t last_requested_value;
    hg_switch_actuator_result_t next_result;
} hg_mock_actuator_t;

hg_switch_status_t hg_mock_actuator_init(hg_mock_actuator_t *mock);

/* Selects the deterministic result returned by the next and later requests. */
hg_switch_status_t hg_mock_actuator_set_result(
    hg_mock_actuator_t *mock,
    hg_switch_actuator_result_t result);

/* Records the request and returns the configured mock result. */
hg_switch_status_t hg_mock_actuator_submit(
    hg_mock_actuator_t *mock,
    hg_switch_value_t requested_value,
    hg_switch_actuator_result_t *result);

#ifdef __cplusplus
}
#endif

#endif /* HOMEGRID_MOCK_ACTUATOR_H */
