/*
 * HomeGrid FW-02C pure switch-domain API.
 *
 * Platform-independent: do not include ESP-IDF, FreeRTOS, GPIO, network,
 * storage or board headers here.
 */
#ifndef HOMEGRID_SWITCH_STATE_H
#define HOMEGRID_SWITCH_STATE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    HG_SWITCH_VALUE_UNKNOWN = 0,
    HG_SWITCH_VALUE_OFF,
    HG_SWITCH_VALUE_ON
} hg_switch_value_t;

typedef enum {
    HG_SWITCH_APPLIED_UNKNOWN = 0,
    HG_SWITCH_APPLIED_NOT_APPLIED,
    HG_SWITCH_APPLIED_ACCEPTED,
    HG_SWITCH_APPLIED_REJECTED
} hg_switch_applied_t;

typedef enum {
    HG_SWITCH_AVAILABILITY_STARTING = 0,
    HG_SWITCH_AVAILABILITY_AVAILABLE,
    HG_SWITCH_AVAILABILITY_DEGRADED,
    HG_SWITCH_AVAILABILITY_UNAVAILABLE
} hg_switch_availability_t;

typedef enum {
    HG_SWITCH_FAULT_NONE = 0,
    HG_SWITCH_FAULT_MOCK_REJECTED,
    HG_SWITCH_FAULT_ACTUATOR_UNAVAILABLE,
    HG_SWITCH_FAULT_INTERNAL
} hg_switch_fault_t;

/*
 * FW-02 intentionally defines no GRANTED value. Output permission cannot
 * be enabled by a command, configuration value or mock result.
 */
typedef enum {
    HG_SWITCH_OUTPUT_DENIED = 0
} hg_switch_output_permission_t;

typedef enum {
    HG_SWITCH_COMMAND_REQUEST_OFF = 0,
    HG_SWITCH_COMMAND_REQUEST_ON
} hg_switch_command_kind_t;

typedef enum {
    HG_SWITCH_SOURCE_LOCAL = 0,
    HG_SWITCH_SOURCE_REMOTE,
    HG_SWITCH_SOURCE_INTERNAL
} hg_switch_command_source_t;

typedef enum {
    HG_SWITCH_POLICY_UNRESOLVED = 0,
    HG_SWITCH_POLICY_ALLOW,
    HG_SWITCH_POLICY_DENY
} hg_switch_policy_result_t;

typedef enum {
    HG_SWITCH_ACTUATOR_ACCEPTED_BY_MOCK = 0,
    HG_SWITCH_ACTUATOR_REJECTED_BY_MOCK,
    HG_SWITCH_ACTUATOR_UNAVAILABLE,
    HG_SWITCH_ACTUATOR_INTERNAL_ERROR
} hg_switch_actuator_result_t;

typedef enum {
    HG_SWITCH_STATUS_OK = 0,
    HG_SWITCH_STATUS_INVALID_ARGUMENT,
    HG_SWITCH_STATUS_INVALID_STATE,
    HG_SWITCH_STATUS_UNSUPPORTED_COMMAND,
    HG_SWITCH_STATUS_POLICY_UNRESOLVED,
    HG_SWITCH_STATUS_POLICY_DENIED
} hg_switch_status_t;

typedef struct {
    hg_switch_value_t desired;
    hg_switch_applied_t applied;
    hg_switch_value_t observed;
    hg_switch_availability_t availability;
    hg_switch_fault_t fault;
    hg_switch_output_permission_t output_permission;
    uint32_t revision;
} hg_switch_state_t;

typedef struct {
    hg_switch_command_kind_t kind;
    hg_switch_command_source_t source;
    uint32_t correlation_id;
} hg_switch_command_t;

/* Establishes the safe FW-02 startup state. */
hg_switch_status_t hg_switch_state_init(hg_switch_state_t *state);

/*
 * Applies a command only when the caller supplies an explicit ALLOW policy
 * result. FW-02 has no policy adapter; unresolved authority must remain
 * unresolved and must not mutate state.
 */
hg_switch_status_t hg_switch_apply_command(
    hg_switch_state_t *state,
    const hg_switch_command_t *command,
    hg_switch_policy_result_t policy_result);

/*
 * Records a mock actuator outcome. This updates software-side applied/fault
 * information only. Observed state remains UNKNOWN and output stays DENIED.
 */
hg_switch_status_t hg_switch_record_mock_result(
    hg_switch_state_t *state,
    hg_switch_actuator_result_t result);

#ifdef __cplusplus
}
#endif

#endif /* HOMEGRID_SWITCH_STATE_H */
