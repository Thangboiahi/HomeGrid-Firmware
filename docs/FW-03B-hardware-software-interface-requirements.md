# FW-03B — Hardware/Software Interface Requirements

**Milestone:** FW-03 — Hardware Abstraction and Board Integration Planning  
**Work package:** FW-03B — Hardware/Software Interface Requirements  
**Authority:** `HG-HQ-ED-FW03-001` / `HG-HQ-DEC-20261108-FW03-001`  
**Status:** Firmware-side interface requirements drafted; Software Department contract review pending  
**Predecessor:** FW-03A photo evidence recorded on `fw-03a-board-module-identification`  
**Baseline:** ESP-IDF v6.1, ESP32 target, existing 2 MB image configuration; FW-02 host/mock architecture

## 1. Purpose and boundary

Define the requirements and ownership boundaries between firmware, the physical board/driver electronics, and future HomeGrid software services. This is an interface specification, not a shared HomeGrid contract and not an implementation authorization.

No GPIO is configured or driven. No physical actuator adapter, communications stack, persistent storage, provisioning, OTA, relay/fan operation or mains testing is introduced. The FW-02 output permission remains deny-only.

## 2. Evidence and coordination status

### 2.1 Hardware evidence available

User-supplied photographs show:
- PCB back silkscreen: `ESP32 DEVKIT V1`.
- Module shield marking: `ESP-WROOM-32`.
- Visible module FCC ID: `2AC7Z-ESP32WROOM32`.
- Micro-USB connector and EN/BOOT buttons.
- Header labels include power/ground and ESP32 GPIO labels.

These markings do not establish carrier manufacturer, PCB revision, module suffix/order code, PSRAM, input voltage/current rating, regulator capability, pin routing or electrical safety. The earlier FW-01 evidence separately verifies ESP32-D0WD-V3 silicon revision 3.0, 4 MB detected flash and CH9102 USB-UART VID:PID `1A86:55D4`. Preserve the existing 2 MB image configuration.

### 2.2 Software Department repository review

The accessible `Thangboiahi/HomeGrid-Software` repository currently reports as empty (GitHub contents API returned HTTP 404: repository is empty). Consequently, no current Software Department source, schema, API specification or versioned contract could be reviewed. This is an integration dependency, not permission to invent a replacement contract.

Software Department is requested to provide or confirm:
1. Current device identity and registration contract, including identifier lifecycle and uniqueness rules.
2. Command envelope/schema, command identifiers, source metadata, expiry/sequence rules, acknowledgement and error semantics.
3. Desired/reported state model and reconciliation ownership.
4. Configuration schema, versioning, defaults, update and reset semantics.
5. Diagnostics/event schema, severity taxonomy, correlation and privacy requirements.
6. Connectivity/offline lifecycle and backend retry/replay expectations, when authorized for a later milestone.
7. Compatibility/version negotiation and backward-compatibility policy.
8. Security/authentication/provisioning ownership and credential lifecycle, through HQ-approved security authority.

No contract modification is proposed by this document.

## 3. Interface principles

1. Hardware-specific details terminate at a future board-support/driver adapter; they must not leak into the pure switch domain.
2. Application use cases consume typed logical requests and typed results, not raw GPIO numbers or electrical levels.
3. Software-facing messages terminate at a future transport/protocol adapter; domain/application code must not depend on MQTT, JSON, topics, HTTP or a specific backend.
4. Identity is opaque to switch logic. Firmware must not invent a MAC-derived or hard-coded identity policy.
5. Desired, applied, observed, availability and fault remain distinct state dimensions.
6. A command acceptance or driver request result is not proof of physical state.
7. Physical observation requires a separately specified feedback source with defined electrical semantics and diagnostic validity.
8. Unknown, invalid, stale or unauthenticated input must not create an actuation permission.
9. Missing authority, schema or arbitration rules remain unresolved and fail closed.
10. All future cross-department contracts require HQ approval before shared-contract changes or implementation.

## 4. Logical interface inventory

| Interface | Provider → consumer | Minimum semantic content | Current FW-02/FW-03 status | Owner |
|---|---|---|---|---|
| Board identity/capability | Board-support adapter → application diagnostics | Board/module identity reference, verified capability flags, firmware/board compatibility | Physical markings partly identified; no machine-readable board profile | Hardware + Firmware |
| Device identity | Approved identity provider → application/transport | Opaque stable device identifier and lifecycle status | Abstract only; no format chosen | HQ / Software |
| Runtime configuration | Approved config provider → application | Validated versioned non-secret settings, source/version and validity | Abstract only; no persistence or provisioning | HQ / Software / Firmware |
| Command input | Transport adapter → application | Contract-versioned command, opaque command/correlation ID, source, requested operation, validity/expiry metadata as approved | Existing FW-02 command type is internal and not a shared protocol | Software / HQ |
| Authority decision | Policy/authorization service → application | Explicit allow/deny/unresolved result and reason; no implicit source priority | FW-02 policy seam only; unresolved policy fails closed | HQ / Software |
| Logical state report | Domain/application → transport adapter | Desired/applied/observed/availability/fault dimensions, revision and freshness | Internal state exists; no shared payload/schema | Firmware / Software |
| Actuator request | Application → board-support driver port | Logical operation, bounded request identity and typed result | Mock only; no physical implementation | Firmware / Hardware |
| Physical feedback | Hardware feedback adapter → application/domain | Signal identity, normalized observation, validity/freshness, typed sensor fault | Not designed; no feedback pins/sensors established | Hardware / Firmware |
| Diagnostics | Diagnostics provider → application/transport adapter | Event code, severity, sequence, correlation, state revision, timestamp source and privacy class | FW-02 local bounded diagnostics only; export contract absent | Firmware / Software |
| Lifecycle/health | Platform adapter → application/diagnostics | Boot/reset cause, uptime/time validity, resource health and connectivity state where authorized | Existing FW-01 metrics baseline; expanded semantics pending | Firmware / Software |
| Firmware compatibility | Firmware image ↔ backend/device registry | Firmware version, hardware compatibility identifier, contract versions/capabilities | Metadata requirements only | Firmware / Software / HQ |

## 5. Firmware-to-hardware boundary requirements

The eventual board-support package shall expose semantic operations and capability declarations, not electrical implementation details.

### 5.1 Board profile

A board profile must be sourced from Hardware-verified evidence and should identify:
- board designation and exact PCB revision, or explicitly unknown;
- module marking and exact module variant, or explicitly unknown;
- SoC and silicon revision;
- physical flash and supported image/partition arrangement;
- PSRAM only when verified;
- onboard peripherals and pin reservations;
- supported supply/input limits from authoritative documentation;
- profile revision and evidence source.

The current `ESP32 DEVKIT V1` marking is not sufficient to create a final board profile or pin map.

### 5.2 Actuator abstraction

A future actuator port may represent logical channels and typed results. It must not expose GPIO numbers, active-high/active-low assumptions, relay coil voltage, pulse timing, PWM frequency or fan electrical characteristics to the domain layer.

A physical adapter is outside FW-03 planning authorization. Its future implementation requires:
- separate HQ implementation authorization;
- Hardware-approved schematic and driver requirements;
- reviewed boot/reset electrical behavior and fail-safe state;
- isolation/protection and load-category evidence;
- independent code review and test plan;
- separate hardware safety review before any energized testing.

### 5.3 Feedback abstraction

Feedback must be separate from command and actuator-request results. A future feedback interface must define:
- what physical condition is actually measured;
- signal conditioning and isolation owned by Hardware;
- normalized states and invalid/unknown conditions;
- freshness, debounce/filtering and sampling semantics;
- open/short/stuck-signal and sensor-failure detection;
- correlation to the logical channel/request where technically possible.

No feedback capability is assumed from the current board photographs.

## 6. Software-facing boundary requirements

### 6.1 Identity and registration
- Firmware consumes an opaque identifier supplied by an HQ-approved identity provider.
- Identity must not be derived from a guessed module serial, MAC address or PCB marking.
- Device replacement, reset, re-provisioning and ownership transfer semantics require an authoritative lifecycle contract.
- Unique identifiers and credentials must not be emitted in ordinary diagnostics.

### 6.2 Commands and arbitration
- Firmware accepts only a validated, versioned command object from a future adapter.
- Duplicate delivery, ordering, expiry, replay protection, idempotency and acknowledgement behavior must be defined by Software/HQ.
- LOCAL/REMOTE/INTERNAL source labels do not establish priority.
- Local-versus-remote arbitration remains unresolved until HQ decides.
- An unresolved authorization or arbitration result must not mutate desired state or reach an actuator port.
- No remote command transport is implemented in FW-03.

### 6.3 State and reconciliation
- Firmware must distinguish accepted logical intent from driver request outcome and physical feedback.
- A state report must not label mock acceptance or command acceptance as physical ON/OFF.
- Desired-state persistence and restart restoration remain unapproved.
- Backend reconciliation behavior after offline periods, reboot or state divergence requires Software/HQ definition.

### 6.4 Configuration
- Configuration is typed, versioned and validated before application.
- Hardware calibration/electrical constants must not be remotely overridden unless explicitly designed and approved.
- Secrets are not ordinary configuration fields and require a separate security/credential architecture.
- Persistence, migration, reset and factory-default behavior remain unapproved.

### 6.5 Diagnostics
- Existing FW-02 diagnostics are local, bounded and in-memory.
- Future exported events require an approved schema, severity taxonomy, timestamp source, correlation policy, retention/privacy classification and rate limits.
- Diagnostics must not expose credentials, keys, raw unique identifiers or unnecessary personal data.
- Loss of diagnostics transport must not affect output-permission safety.

## 7. Error and availability semantics

The following semantic distinctions are required, but exact shared enum names/codes remain subject to Software/HQ contract review:

- `INVALID_INPUT`: malformed, unsupported or out-of-range input.
- `UNAUTHENTICATED_OR_UNAUTHORIZED`: security/policy decision from an approved authority.
- `POLICY_UNRESOLVED`: authority or arbitration cannot be determined.
- `UNAVAILABLE`: required provider/adapter cannot currently serve the request.
- `TIMEOUT`: operation did not complete within an approved bound.
- `HARDWARE_FAULT`: driver or feedback path reports a hardware-level fault.
- `OBSERVATION_UNKNOWN`: no valid physical observation is available.
- `STALE_DATA`: received state/configuration/feedback is older than its approved validity window.
- `VERSION_UNSUPPORTED`: contract or configuration version is not supported.
- `INTERNAL_ERROR`: unexpected software failure.

Do not collapse these into a generic OFF state. A transport or backend outage must not be interpreted as a physical load state.

## 8. Timing, concurrency and resource requirements

- Keep the FW-02 synchronous application model unless an evidenced interface requirement justifies asynchronous execution.
- Any future worker/task must have documented owner, priority, stack, blocking behavior, queue bounds, watchdog interaction and shutdown/restart behavior.
- Driver and feedback operations must have finite, reviewed time bounds; exact values depend on the selected hardware and protocol.
- No timing, debounce, PWM or retry constants are invented in FW-03B.
- Preserve ESP-IDF v6.1, target `esp32`, 2 MB image configuration and no-PSRAM dependency.
- Do not install tools or alter system-wide environment settings.

## 9. Contract and dependency register

| ID | Required decision/evidence | Owner | Blocks |
|---|---|---|---|
| FW03B-DEP-01 | Versioned device identity and registration lifecycle | HQ / Software | Identity integration |
| FW03B-DEP-02 | Command schema, idempotency, ordering, expiry and acknowledgement | Software / HQ | Command adapter |
| FW03B-DEP-03 | Local/remote arbitration and authorization semantics | HQ / Software | Command execution policy |
| FW03B-DEP-04 | Desired/reported state and reconciliation contract | Software / HQ | State synchronization |
| FW03B-DEP-05 | Versioned configuration, migration and reset policy | Software / HQ / Firmware | Configuration integration |
| FW03B-DEP-06 | Diagnostics event schema and privacy policy | Software / HQ | Telemetry export |
| FW03B-DEP-07 | Security, provisioning and credential lifecycle | HQ / Security / Software | Authenticated communications |
| FW03B-DEP-08 | Offline/reconnect, retry and replay semantics | Software / HQ | Reliable backend integration |
| FW03B-DEP-09 | Hardware-verified board profile and electrical interface | Hardware / Firmware | Board-specific driver design |
| FW03B-DEP-10 | Feedback sensor/circuit and normalized semantics | Hardware / Firmware | Physical observation |
| FW03B-DEP-11 | Hardware/firmware compatibility and release metadata | Hardware / Firmware / Software | Fleet compatibility |

## 10. FW-03B exit assessment

**Completed by Firmware:** internal FW-02 APIs and architecture were reviewed; hardware-facing and software-facing boundaries are specified; required semantic fields, ownership, error distinctions, safety rules and dependencies are documented; no shared contract was changed.

**Pending:** Software Department review against its authoritative contracts. The accessible Software repository was empty at review time, so this report cannot claim compatibility with an existing backend schema. Hardware Department review remains required for board-profile, driver and feedback feasibility.

FW-03B can be considered a Firmware-side requirements draft, not a cross-department-approved interface contract. FW-03C must continue to treat all board-specific pin and electrical details as unknown until Hardware evidence is supplied. No physical actuator implementation is authorized by this document.

---
**Prepared by:** HomeGrid Firmware Department  
**Review requested from:** HomeGrid Hardware Department and Software Department  
**Authority for any shared contract change:** HomeGrid HQ
