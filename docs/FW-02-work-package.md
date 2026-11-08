# Proposed FW-02 — Smart Switch Firmware Foundation

**Status:** Draft for Firmware Department review; not approved for implementation.  
**Predecessor:** FW-01 closed by accepted FW-01F report.  
**Purpose:** Define a safe, testable firmware foundation for a HomeGrid smart-switch device without enabling physical load actuation in this work package.

## Proposed scope for approval

1. **Requirements and interface traceability:** collect the currently approved HomeGrid device identity, provisioning, messaging, security, telemetry, and OTA contracts from their authoritative owners. Map requirements to firmware components and host tests. Do not invent or alter shared contract details.
2. **Firmware structure:** propose C/ESP-IDF component boundaries for configuration, device state, diagnostics, communications adapters, and hardware abstraction. Keep application behavior independent of board-specific GPIO numbers and driver polarity.
3. **State model:** specify an explicit, testable switch intent/state model and safe startup/fault states. Model behavior in host tests or mocks only; do not drive any GPIO or relay.
4. **Configuration and identity handling:** document non-secret defaults and interfaces for approved device identity and provisioning. No production credentials or real secrets in source, logs, tests, or example configuration.
5. **Observability and fault handling:** define structured diagnostic events, reset/fault reporting, task ownership, watchdog policy, and heap/stack instrumentation appropriate to ESP-IDF/FreeRTOS.
6. **Verification strategy:** define unit/host tests, build checks on pinned ESP-IDF v6.1, and later hardware test gates. Keep hardware output drivers disabled until a separate safety/design review is approved.

## Proposed deliverables

- Approved FW-02 requirements and architecture note with traceability to existing contracts.
- Component/interface skeleton in C only, once scope is approved.
- Host-side tests for state transitions, defaults, and error handling.
- A board-independent GPIO/actuator interface with no permanent pin assignments.
- Build and verification report; no physical actuation in the foundation milestone.

## Explicit exclusions and gates

- No relay/fan control, GPIO output enablement, mains connection, or load test.
- No permanent relay GPIO selection, polarity, or electrical assumptions.
- No changes to HomeGrid topic names, payloads, identity rules, security policy, or OTA contract. Contract changes require separate HQ approval.
- No Wi-Fi/BLE/MQTT/provisioning/OTA implementation until requirements, security review, and test scope are explicitly approved.
- No production credentials or secrets in the repository.
- The exact carrier PCB revision, module suffix, and PSRAM status from FW-01F remain hardware follow-ups. Any board-specific integration must first resolve the applicable details and receive a separate authorization.

## Review questions for Firmware Department

1. Approve or revise the proposed component boundaries and state model before implementation.
2. Identify the authoritative owners/documents for existing communication, identity, security, provisioning, and OTA contracts.
3. Decide whether FW-02 should remain simulation/host-test only or include a reviewed communications-only hardware milestone.
4. Set separate approval gates for radio/network integration, GPIO driver design, and any future relay/mains work.

**No implementation has started.** This document is a proposed scope package for review only.
