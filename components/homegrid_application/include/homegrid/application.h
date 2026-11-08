/*
 * FW-02D synchronous application orchestration API with FW-02E diagnostics.
 *
 * Commands require explicit policy. The actuator dependency remains mock-only.
 */
#ifndef HOMEGRID_APPLICATION_H
#define HOMEGRID_APPLICATION_H

#include "homegrid/diagnostics.h"
#include "homegrid/mock_actuator.h"
#include "homegrid/switch_state.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    hg_switch_status_t domain_status;
    uint8_t actuator_called;
    hg_switch_actuator_result_t actuator_result;
} hg_application_result_t;

typedef struct {
    hg_switch_state_t switch_state;
    hg_mock_actuator_t mock_actuator;
    hg_diagnostics_t diagnostics;
} hg_application_t;

typedef enum {
    HG_APPLICATION_STATUS_OK = 0,
    HG_APPLICATION_STATUS_INVALID_ARGUMENT,
    HG_APPLICATION_STATUS_INITIALIZATION_ERROR,
    HG_APPLICATION_STATUS_COMMAND_REJECTED,
    HG_APPLICATION_STATUS_STATE_ERROR,
    HG_APPLICATION_STATUS_MOCK_ERROR
} hg_application_status_t;

hg_application_status_t hg_application_init(hg_application_t *application);

/* Test seam: configures only the mock result; it cannot enable output. */
hg_application_status_t hg_application_set_mock_result(
    hg_application_t *application,
    hg_switch_actuator_result_t result);

/*
 * Processes one command synchronously. Policy rejection never reaches the
 * mock. Outcomes are recorded in bounded in-memory diagnostics.
 */
hg_application_status_t hg_application_process_command(
    hg_application_t *application,
    const hg_switch_command_t *command,
    hg_switch_policy_result_t policy,
    hg_application_result_t *result);

#ifdef __cplusplus
}
#endif

#endif /* HOMEGRID_APPLICATION_H */
