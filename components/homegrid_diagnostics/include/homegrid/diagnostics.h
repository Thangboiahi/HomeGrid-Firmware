/*
 * FW-02E bounded in-memory diagnostics API.
 *
 * No clock, logging backend, storage, transport, hardware or RTOS dependency.
 */
#ifndef HOMEGRID_DIAGNOSTICS_H
#define HOMEGRID_DIAGNOSTICS_H

#include <stdint.h>

#include "homegrid/switch_state.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HG_DIAGNOSTICS_EVENT_CAPACITY 8U

typedef enum {
    HG_DIAG_EVENT_STARTED = 0,
    HG_DIAG_EVENT_COMMAND_REJECTED,
    HG_DIAG_EVENT_STATE_ERROR,
    HG_DIAG_EVENT_MOCK_ACCEPTED,
    HG_DIAG_EVENT_MOCK_REJECTED,
    HG_DIAG_EVENT_MOCK_UNAVAILABLE,
    HG_DIAG_EVENT_MOCK_INTERNAL_ERROR
} hg_diagnostics_event_type_t;

typedef struct {
    uint32_t sequence;
    hg_diagnostics_event_type_t type;
    uint32_t correlation_id;
    uint32_t state_revision;
    hg_switch_fault_t fault;
} hg_diagnostics_event_t;

typedef struct {
    uint32_t total_events;
    uint32_t dropped_events;
    uint32_t command_rejections;
    uint32_t state_errors;
    uint32_t mock_acceptances;
    uint32_t mock_rejections;
    uint32_t mock_unavailable;
    uint32_t mock_internal_errors;
    hg_switch_fault_t last_fault;
    uint8_t retained_events;
} hg_diagnostics_snapshot_t;

typedef struct {
    hg_diagnostics_event_t events[HG_DIAGNOSTICS_EVENT_CAPACITY];
    hg_diagnostics_snapshot_t snapshot;
    uint8_t next_index;
} hg_diagnostics_t;

typedef enum {
    HG_DIAGNOSTICS_STATUS_OK = 0,
    HG_DIAGNOSTICS_STATUS_INVALID_ARGUMENT,
    HG_DIAGNOSTICS_STATUS_INVALID_EVENT
} hg_diagnostics_status_t;

hg_diagnostics_status_t hg_diagnostics_init(hg_diagnostics_t *diagnostics);

hg_diagnostics_status_t hg_diagnostics_record(
    hg_diagnostics_t *diagnostics,
    hg_diagnostics_event_type_t type,
    uint32_t correlation_id,
    uint32_t state_revision,
    hg_switch_fault_t fault);

/* Copies a stable summary; it does not expose mutable internal storage. */
hg_diagnostics_status_t hg_diagnostics_get_snapshot(
    const hg_diagnostics_t *diagnostics,
    hg_diagnostics_snapshot_t *snapshot);

/*
 * Reads retained events oldest-first. offset zero is the oldest retained
 * event. Returns INVALID_ARGUMENT when offset is outside retained history.
 */
hg_diagnostics_status_t hg_diagnostics_get_event(
    const hg_diagnostics_t *diagnostics,
    uint8_t offset,
    hg_diagnostics_event_t *event);

#ifdef __cplusplus
}
#endif

#endif /* HOMEGRID_DIAGNOSTICS_H */
