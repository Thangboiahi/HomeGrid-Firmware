# FW-02 — Smart Switch Firmware Foundation

**Status:** APPROVED FOR IMPLEMENTATION by HomeGrid HQ Executive Directive `HG-HQ-ED-FW02-001` (2026-11-08).  
**Decision reference:** `HG-HQ-DEC-20261108-FW02-001`  
**Predecessor:** FW-01 formally closed by accepted FW-01F report.  
**Purpose:** Establish a safe, testable firmware foundation without enabling physical load actuation.

## Authorized work-package sequence

1. **FW-02A — Requirements and interface traceability:** record approved requirements, authorities, owners, verification methods, interface gaps and dependencies.
2. **FW-02B — Firmware architecture:** define component boundaries, dependency direction, state ownership, task model and build/test boundaries.
3. **FW-02C — State model and command processing:** implement deterministic domain logic and command validation/arbitration seams without inventing unresolved policy.
4. **FW-02D — Mock actuator and host tests:** implement mock-only actuator behavior and comprehensive host-native tests.
5. **FW-02E — Diagnostics and fault handling:** implement local diagnostics, fault propagation and resource instrumentation appropriate to the approved scope.
6. **FW-02F — Verification and final handover:** run permitted host/build verification, audit exclusions, document evidence and report unresolved dependencies to HQ.

## Authorized scope

- Requirements and interface traceability.
- Board-independent C firmware architecture using the repository's ESP-IDF v6.1 / FreeRTOS baseline.
- Explicit switch state model with distinct desired, applied, observed, availability, fault and output-permission dimensions.
- Abstract configuration and identity interfaces; non-secret defaults only where approved.
- Local diagnostics and fault handling.
- Mock actuator only and host-native verification.
- Documentation of future integration requirements and approval gates.

## Mandatory restrictions

- Host-test and mocked-operation only.
- **No physical GPIO implementation or output enablement.**
- **No relay/fan actuation, mains-connected testing or load testing.**
- No Wi-Fi, BLE, MQTT, provisioning or OTA implementation.
- No modifications to shared HomeGrid contracts without separate HQ approval.
- Local-versus-remote command priority and restart/power-loss state restoration remain undecided; do not invent or hard-code policy.
- No production credentials, secrets or real keys in source, logs, tests or examples.
- Do not assume unresolved carrier-board/module details, GPIO assignments, electrical polarity, driver topology or PSRAM.
- Preserve the verified 4 MB physical flash record and existing 2 MB image configuration; no PSRAM dependency.
- No system-wide installations, environment mutation, flash erase, privileged/destructive actions or physical flashing under this directive.

## Required architecture and safety properties

- Keep pure C domain logic independent of ESP-IDF, FreeRTOS, GPIO, network and storage details wherever practical.
- Separate application orchestration, switch domain, configuration, identity abstraction, diagnostics, communications abstraction, actuator abstraction and platform services.
- Startup and fault states must inhibit output and represent physical observation as unknown.
- A requested or mock-applied state is not proof of physical state.
- Actuator backend selection must be fail-closed to the mock; no real GPIO backend may be selectable in FW-02.
- Keep task count minimal and document ownership, priority, stack, blocking/I/O, watchdog interaction and failure behavior before adding tasks.
- Do not disable, bypass or arbitrarily feed the watchdog.
- Record relevant heap metrics and task stack high-water marks where supported, comparing against FW-01 baseline without treating one reading as a universal threshold.

## Deliverables and acceptance evidence

- FW-02A requirements/interface traceability document.
- FW-02B architecture and interface design.
- FW-02C deterministic state and command-processing implementation with tests.
- FW-02D mock actuator and host test suite.
- FW-02E diagnostics/fault-handling implementation and tests.
- FW-02F verification report, source-control references, test/build evidence, exclusion audit, unresolved dependency register and HQ handover.

## External dependencies and decision owners

Device identity, provisioning/security, command transport/schema, telemetry export, OTA policy, local/remote authority, restart restoration, persistent configuration/reset semantics, and exact hardware/electrical details remain with their authoritative HQ, Software, Security, Product and Hardware owners. FW-02 may define narrow abstractions but must not invent these contracts or implement blocked integrations.

See `docs/FW-02A-requirements-traceability.md` for requirement IDs, interface ownership, open dependencies and FW-02A acceptance record.

## Change control

Any scope expansion, shared contract modification, physical GPIO/actuator implementation, radio/network integration, provisioning, OTA, credentials, mains/load testing, or destructive/production action requires separate explicit authorization. Pause affected work and notify HQ if a material blocker or conflict arises.
