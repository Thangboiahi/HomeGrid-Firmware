# FW-02B — Firmware Architecture and Interface Design

**Milestone:** FW-02 — Smart Switch Firmware Foundation  
**Work package:** FW-02B — Firmware Architecture  
**Authority:** `HG-HQ-ED-FW02-001` / `HG-HQ-DEC-20261108-FW02-001`  
**Predecessor:** FW-02A accepted by Firmware Department on branch `fw-02a-requirements-traceability`; PR #1 remains an integration-review dependency.  
**Status:** Architecture baseline — prepared for department verification  
**Target:** Original ESP32 target already verified for the existing FW-01 build; ESP-IDF v6.1; C; FreeRTOS.  
**Safety class for this package:** Host/mock-only design. No hardware output path is designed or enabled.

## 1. Architecture goals

1. Preserve the verified FW-01 application/build baseline while creating a maintainable foundation for a future smart-switch application.
2. Keep switch-domain behavior deterministic, synchronous and host-buildable.
3. Separate logical intent, software application outcome, physical observation, device availability, faults and output permission.
4. Make all physical actuator behavior impossible to select in FW-02; the only actuator implementation is a host-test mock.
5. Keep unresolved HomeGrid identity, command, authority, persistence, security and telemetry contracts outside the domain model.
6. Keep board-specific electrical details outside application and domain code.
7. Add no unnecessary FreeRTOS tasks, network stacks, storage backends or hardware drivers.

## 2. Architectural decisions

| Decision | Baseline | Rationale / boundary |
|---|---|---|
| FW02B-ADR-001 | Layered, ports-and-adapters architecture | Domain logic must not depend on ESP-IDF, FreeRTOS or board-specific code. |
| FW02B-ADR-002 | Pure C domain library | Same domain source can be compiled by ESP-IDF and host-native tests. |
| FW02B-ADR-003 | Synchronous state transition API | Deterministic behavior and testability; avoid concurrency until a real requirement exists. |
| FW02B-ADR-004 | Application orchestrator owns use-case sequencing | Keeps transport, state, actuator and diagnostics from calling each other arbitrarily. |
| FW02B-ADR-005 | Mock actuator is the sole FW-02 actuator adapter | No physical GPIO adapter, driver, pin map or backend selection is included. |
| FW02B-ADR-006 | Unknown is a first-class observation | Startup and lack of feedback must never be represented as OFF. |
| FW02B-ADR-007 | Output permission is permanently denied in FW-02 | Domain requests and mock responses cannot authorize physical output. |
| FW02B-ADR-008 | No persistence in FW-02 | Restart/power-loss and configuration persistence semantics are unresolved. |
| FW02B-ADR-009 | No communications implementation in FW-02 | Command transport/schema and local/remote authority are external dependencies. |
| FW02B-ADR-010 | No new FreeRTOS worker task by default | Existing `app_main` task is sufficient for the bounded foundation; future tasking requires measured need and review. |

These are firmware-internal architecture decisions. They do not modify or establish shared HomeGrid contracts.

## 3. Logical component view

```text
                 ESP-IDF platform entry (app_main)
                              |
                              v
                   Application orchestrator
                    /        |          \
                   v         v           v
             Command port  Domain API  Diagnostics port
                  |           |             |
           (no transport)     v        Local diagnostics
                         Switch domain
                              |
                              v
                        Actuator port
                              |
                              v
                        Mock adapter
                       (FW-02 only)

        Configuration / Identity abstractions
           (non-secret, no persistence, no provisioning)
```

### 3.1 Component responsibilities

| Component | Owns | Must not own / depend on |
|---|---|---|
| Platform entry | ESP-IDF startup, platform lifecycle, future resource sampling | Switch policy, GPIO actuation, network initialization |
| Application orchestrator | Use-case sequencing, dependency construction, result routing | Protocol policy, hardware pin mapping, persistent state |
| Switch domain | State types, invariants, transition rules, command validation independent of transport | ESP-IDF, FreeRTOS, GPIO, storage, network |
| Command boundary | Transport-neutral command envelope and source metadata | MQTT topics, JSON schema, radio stack, authority policy assumptions |
| Actuator port | Narrow request/result contract | GPIO numbers, electrical polarity, ESP-IDF driver calls |
| Mock actuator adapter | Record mock requests and return test-controlled outcomes | Any physical I/O or automatic success claims about physical state |
| Configuration boundary | Validated non-secret runtime configuration view | Secrets, NVS persistence, provisioning, assumed reset semantics |
| Identity boundary | Opaque identity value/provider contract | Invented identity format, MAC-derived identity policy, credential lifecycle |
| Diagnostics | Structured local events and counters | External telemetry, credentials, transport dependencies |
| Host test harness | Instantiate domain and mock adapters, assert deterministic outcomes | Hardware or ESP-IDF runtime assumptions |

## 4. Proposed source layout

This is the intended FW-02 component organization. It is a design target, not a claim that these files already exist.

```text
main/
  CMakeLists.txt
  app_main.c                    # ESP-IDF composition root / lifecycle
  platform/
    platform_metrics.c          # ESP-IDF heap/task metrics adapter (FW-02E)
  application/
    homegrid_app.c               # use-case orchestration (FW-02C onward)
    homegrid_app.h
  domain/
    switch/
      switch_state.h             # transport/platform-independent types
      switch_state.c             # invariants and pure transitions
      switch_command.h           # transport-neutral command envelope
      switch_command.c           # validation / policy seam
  ports/
    actuator_port.h              # actuator request/result interface
    command_input_port.h         # abstract command input boundary
    identity_port.h              # opaque identity provider contract
    config_port.h                # validated non-secret config view
    diagnostics_port.h           # local diagnostic event interface
  adapters/
    mock/
      mock_actuator.c             # only actuator adapter in FW-02
      mock_actuator.h
    local/
      local_diagnostics.c         # local-only diagnostic sink
components/
  homegrid_domain/                # host-buildable pure C domain library
tests/
  host/
    test_switch_state.c
    test_switch_command.c
    test_mock_actuator.c
```

The final directory/CMake decomposition may be adjusted during implementation if the repository's ESP-IDF component structure warrants it. Such adjustment must preserve dependency direction and safety invariants. No generated file list here should be interpreted as already implemented.

## 5. Dependency direction and allowed imports

Allowed dependency direction:

- `domain/switch` → C standard library only.
- `application` → domain types and abstract ports.
- `adapters` → abstract ports and their own implementation dependencies.
- `platform` → ESP-IDF/FreeRTOS and application composition root.
- `tests/host` → domain sources, abstract ports and mock adapters.

Prohibited dependencies:

- Domain code must not include `esp_*`, `freertos/*`, `driver/gpio.h`, network headers, NVS headers or board configuration headers.
- Domain code must not log directly; it returns typed outcomes for the application/diagnostics boundary.
- Mock actuator must not include GPIO or peripheral driver headers.
- Application/domain code must not read or write persistent storage in FW-02.
- No component may import Wi-Fi, BLE, MQTT, provisioning or OTA APIs in FW-02.
- No physical actuator adapter may be added to the component graph.

Host build rule: the same domain C sources used by firmware should be compiled into native tests. Do not maintain a separate Python reimplementation of domain behavior.

## 6. State ownership and data model

The domain owns the logical state. The application orchestrator is the only writer through domain transition functions. Adapters return outcomes; they do not mutate domain state directly.

The model must keep these dimensions distinct:

| Dimension | Suggested representation | Meaning |
|---|---|---|
| Desired state | `UNKNOWN / OFF / ON` | Latest accepted logical target, if any |
| Applied state | `UNKNOWN / NOT_APPLIED / ACCEPTED / REJECTED` | Software-side actuator request outcome only; never proof of physical output |
| Observed state | `UNKNOWN / OFF / ON` | Physical observation; remains UNKNOWN without an approved feedback source |
| Availability | `STARTING / AVAILABLE / DEGRADED / UNAVAILABLE` | Logical service availability, not physical load state |
| Fault | `NONE / ACTIVE` plus internal fault code | Whether the domain/application has an active fault |
| Output permission | `DENIED` in all FW-02 builds | Explicit gate; no transition in FW-02 can grant it |

### 6.1 Required invariants

1. Initial desired state is UNKNOWN; startup policy does not invent an OFF or ON target.
2. Initial applied state is UNKNOWN.
3. Initial observed state is UNKNOWN.
4. Initial availability is STARTING or UNAVAILABLE until the application completes its mock-only initialization path.
5. Output permission is DENIED and cannot be changed by a command, configuration value, mock result or runtime state transition in FW-02.
6. A successful mock request may update applied state to ACCEPTED, but observed state remains UNKNOWN.
7. A failed mock request must not change observed state and must produce a typed failure/fault result.
8. A command rejected by validation or unresolved authority policy must not mutate desired state.
9. An unavailable or degraded service is not equivalent to OFF.
10. Fault clear/recovery semantics must not be invented; fault transitions are explicit and tested when defined.

### 6.2 State transition ownership

Only pure domain transition functions may update domain state. The mock adapter returns a result value. The orchestrator passes that result into a domain transition function. This keeps state mutation deterministic and makes tests independent of execution order.

No asynchronous callbacks, shared mutable globals, locks or queues are required for the initial synchronous model.

## 7. Command processing boundary

A transport-neutral command envelope may carry:
- command kind (request ON / request OFF / query or internal operation, as later defined);
- source classification (LOCAL / REMOTE / INTERNAL) without priority implication;
- an opaque correlation token if supplied by an upstream adapter, subject to future privacy/security rules;
- a validated timestamp/sequence only if an approved contract later requires it.

The envelope must not embed MQTT topic names, JSON field names, authentication claims or invented device identifiers.

Processing stages:
1. Parse/decode in a future transport adapter (not part of FW-02).
2. Validate structural command fields at the boundary.
3. Ask an explicit policy seam whether the source is authorized and whether arbitration is resolved.
4. If authority/arbitration is unresolved, return `POLICY_UNRESOLVED` and leave state unchanged.
5. Apply the pure domain transition only for a valid, policy-approved command.
6. If the use case requests an actuator operation, route only to the mock port in FW-02.
7. Record local diagnostic outcome; do not transmit telemetry.

Local-vs-remote priority is not defined here. FW-02 must not implement “local always wins”, “remote always wins”, last-write-wins, timestamp ordering or any other arbitration policy without HQ approval.

## 8. Ports and adapter contracts

The interfaces below describe semantics, not final ABI signatures. FW-02C/D may settle exact C types and naming while preserving the rules.

### 8.1 Actuator port

Input: a logical requested state and an internal request correlation value.  
Output: typed result such as accepted-by-mock, rejected, unavailable or internal-error.

Rules:
- The port contains no GPIO pin, polarity, timing, voltage, relay or fan fields.
- The mock records every request and allows tests to configure deterministic outcomes.
- A mock acceptance is not a physical actuation claim.
- No real adapter or runtime backend selector is permitted.
- If the mock is not constructed or returns an error, the application fails closed and output permission remains DENIED.

### 8.2 Command input port

Provides a validated transport-neutral command envelope to the application. FW-02 does not implement a concrete network or local hardware transport. The port must not smuggle in assumptions about ordering, delivery guarantees, authentication or authority.

### 8.3 Identity port

Returns an opaque identity result only when an approved provider exists. In FW-02, identity may be unavailable/unknown. Do not synthesize identity from MAC address, chip ID, serial number or random value.

### 8.4 Configuration port

Provides validated non-secret runtime defaults only. No NVS, filesystem, cloud provisioning, migration, factory reset or credential storage is implemented. Configuration cannot grant output permission or change safety restrictions.

### 8.5 Diagnostics port

Accepts typed local events/counters. No external transmission, sensitive identifiers, credentials or secrets. Diagnostics failures must not crash the domain or silently change switch state.

## 9. Error model and fault propagation

Use typed result/error codes across boundaries rather than parsing log text. Minimum categories to support during implementation:

- invalid argument / malformed command;
- unsupported command;
- policy unresolved / unauthorized;
- actuator unavailable;
- mock-reported rejection;
- internal invariant violation;
- diagnostics sink failure;
- configuration unavailable/invalid;
- identity unavailable.

Error handling rules:
- Domain functions return errors and leave state unchanged on rejected input.
- Adapter failures are translated by the orchestrator into domain fault transitions only where the transition is explicitly defined.
- Diagnostics sink failure is recorded as a secondary error; it must not overwrite the primary use-case result.
- No secrets or raw credential-bearing payloads may appear in errors or logs.
- Unknown error categories fail closed; they never grant output permission.

## 10. Configuration and identity lifecycle

FW-02 uses immutable-at-runtime, non-secret defaults supplied at composition time. No persistent storage is introduced.

- No device identity format or lifecycle is established.
- No network credentials, tokens, keys, certificates or provisioning material are represented.
- Missing identity is a normal explicit state, not a reason to invent a fallback identity.
- Configuration validation rejects unknown/unsupported fields rather than interpreting them permissively.
- Any future persistence or reset behavior requires the HQ decision listed in FW-02A dependency register.

## 11. Task, concurrency and watchdog model

Initial architecture uses the existing ESP-IDF `app_main` task as the composition root. Domain transitions and mock calls are synchronous and bounded.

No additional worker task, queue, timer callback, mutex or event group is justified for FW-02B. If later measurements or an approved transport require asynchronous work, document:
- single owner of each mutable state object;
- task name, priority, stack size and stack high-water mark;
- blocking calls and maximum expected duration;
- queue depth, overflow behavior and backpressure;
- watchdog expectations and recovery behavior;
- shutdown/restart semantics.

Do not disable, bypass or arbitrarily feed the task/watchdog mechanisms. No long blocking operation should be added to `app_main` without a reviewed resource/liveness rationale.

## 12. Resource and diagnostics architecture

Platform metrics adapter (planned for FW-02E) may sample:
- current free heap;
- minimum free heap;
- largest free block where supported;
- capability-specific heap values where supported by the target/API;
- allocation failures where observable;
- task stack high-water marks for tasks the application owns.

Metrics are diagnostic evidence, not control inputs for output permission. Use ESP-IDF APIs only in the platform adapter. Do not put ESP-IDF heap calls into the pure domain library.

The FW-01 reported heap baseline is a comparison point, not a universal pass/fail threshold. FW-02 must record the build configuration and measurement conditions before making comparisons.

## 13. Build and test architecture

### Firmware build
- ESP-IDF v6.1 remains pinned.
- Existing verified `esp32` target and 2 MB image configuration remain unchanged.
- No PSRAM dependency.
- Production firmware sources remain C.
- Do not change target, partitions, flash size or board configuration as part of FW-02B.

### Host build
- Compile the same pure C domain source files natively.
- Use deterministic test doubles for ports.
- Keep tests independent of ESP-IDF installation and physical hardware.
- Test invalid input, every defined transition, no-mutation-on-error, output-permission denial, startup unknown state, mock accepted/rejected/unavailable results and unresolved command policy.

### Safety/static checks
- Search FW-02 sources for GPIO initialization/output APIs, relay/fan driver code, Wi-Fi/BLE/MQTT/provisioning/OTA APIs, NVS/persistent writes and credential-like test data.
- Ensure no real actuator backend selector exists.
- Verify domain include graph has no ESP-IDF/FreeRTOS dependencies.
- Verify FW-01 source/build configuration is preserved unless a narrowly justified and reviewed integration change is required.

No test or build is claimed by this architecture document. FW-02C/D/E/F will add implementation and captured verification evidence.

## 14. Security and privacy boundaries

- Treat all external commands as untrusted until an approved security and authorization architecture exists.
- No authentication, authorization, key management or cryptographic policy is invented here.
- No credentials or unique device identifiers in logs, tests, examples or commits.
- Correlation tokens must be opaque and must not contain secrets or personal data.
- Diagnostics are local-only in FW-02 and should minimize identity-bearing data.
- Generic security/IP standard applicability remains pending HQ confirmation.

## 15. Future integration gates

| Gate | Required owner input | What remains blocked |
|---|---|---|
| Identity | HQ-approved identity schema and lifecycle | Real identity provider |
| Security/provisioning | HQ/security trust model, provisioning and credential lifecycle | Authentication, credentials, radio provisioning |
| Command interface | Software/HQ versioned command contract | Concrete transport adapter and protocol parsing |
| Arbitration | HQ/product local-vs-remote authority decision | Source-priority policy |
| Persistence/restart | HQ-approved desired-state and reset behavior | Persistent config/state restoration |
| Hardware | Exact board/module, approved schematic, driver and safety analysis | GPIO/actuator implementation and physical tests |
| Telemetry | Software/HQ diagnostic export contract | External telemetry |
| OTA | HQ release authenticity, rollback and recovery policy | OTA implementation |

## 16. FW-02B acceptance checklist

- [x] Architecture layers and responsibilities defined.
- [x] Dependency direction and forbidden imports documented.
- [x] Intended source layout documented without claiming files exist.
- [x] State ownership, dimensions and safety invariants defined.
- [x] Command-processing boundary documented without inventing arbitration.
- [x] Actuator, command, identity, configuration and diagnostics ports specified.
- [x] Error propagation, task/concurrency and resource model documented.
- [x] Host/firmware build and verification boundaries documented.
- [x] Future integration gates mapped to authoritative owners.
- [ ] Document committed and reread from GitHub.
- [ ] Firmware Department acceptance recorded after repository verification.

## 17. FW-02C entry criteria and recommendation

Proceed to FW-02C with pure C domain types and deterministic transition/command-validation functions only. Do not implement a physical adapter, persistent storage, transport, authentication or unresolved source-priority policy. Preserve the permanent FW-02 output-permission denial. Add host tests alongside the domain code and verify that the same sources remain free of ESP-IDF dependencies.

---
**Prepared by:** HomeGrid Firmware Department  
**Document control:** FW-02B architecture baseline; implementation and verification are subsequent work packages.
