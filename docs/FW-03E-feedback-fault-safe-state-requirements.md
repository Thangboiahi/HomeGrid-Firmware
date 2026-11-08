# FW-03E — Feedback, Fault Detection and Safe-State Requirements

**Milestone:** FW-03 — Hardware Abstraction and Board Integration Planning  
**Work package:** FW-03E — Feedback, Fault Detection and Safe-State Requirements  
**Authority:** `HG-HQ-ED-FW03-001` / `HG-HQ-DEC-20261108-FW03-001`  
**Status:** Generic requirements and failure model drafted; Hardware evidence and HQ safety review pending  
**Baseline:** FW-02 deny-only output permission; ESP-IDF v6.1; ESP32 target; existing 2 MB image configuration

## 1. Purpose and strict boundary

Define the system-level semantics for physical feedback, fault detection and safe-state handling before any physical actuator implementation.

This document does not choose sensors, pins, voltage levels, thresholds, debounce intervals, watchdog timeouts, relay polarity or fan control behavior. It does not authorize GPIO, relay/fan operation, mains connection or physical testing.

The FW-02 output-permission boundary remains deny-only. Firmware must not convert a fault-clear condition into output permission. Any future permission mechanism requires a separate HQ decision and safety review.

## 2. State distinctions that must remain separate

The implementation and future interfaces must preserve these independent facts:

- **Desired state:** logical target requested by an authorized command.
- **Applied state:** state the driver interface reports it has accepted/applied.
- **Observed state:** physical state supported by a valid feedback source.
- **Availability:** whether the relevant subsystem is starting, available, degraded or unavailable.
- **Fault:** diagnostic condition and its severity/lifecycle.
- **Output permission:** independent safety authorization; remains denied in the current firmware.

No one field may be inferred from another. In particular:
- Command accepted ≠ driver applied.
- Driver request accepted ≠ relay contact changed.
- Relay coil energized ≠ contact state verified.
- Fan command issued ≠ fan rotating.
- No feedback ≠ OFF.
- Communications lost ≠ physical load OFF.
- Fault cleared ≠ permission granted.

If physical feedback is absent, invalid, stale or contradictory, observed state must remain UNKNOWN or transition to an explicitly defined unknown/degraded representation. It must not silently retain a misleading confirmed state indefinitely.

## 3. Feedback source requirements

Each proposed feedback source must have a Hardware-owned interface definition.

| Required field | Description |
|---|---|
| Feedback identifier | Stable logical name, not GPIO number |
| Physical quantity | What is actually sensed (auxiliary contact, tachometer, current, voltage, temperature, driver fault, etc.) |
| Source component | Exact sensor/contact/driver component and datasheet |
| Electrical interface | Output type, voltage/current range, polarity, thresholds, isolation and protection |
| Signal conditioning | Divider, comparator, optocoupler, filtering, biasing, debounce or edge conditioning |
| Validity | How open circuit, short circuit, out-of-range and sensor failure are detected |
| Freshness | Maximum permitted age, based on a documented system requirement |
| Sampling | Rate, pulse capture, filtering and timing requirements |
| Startup behavior | Initial unknown state and settling behavior |
| Failure coverage | Faults detected and faults that cannot be detected |
| Correlation | Whether observation can be linked to a logical channel/request |
| Evidence | Schematic, BOM, datasheet, analysis and Hardware sign-off |

No electrical range or timing threshold is supplied by Firmware in FW-03E.

## 4. Feedback semantics by possible measurement

These are possible measurement categories, not selected hardware.

### 4.1 Relay / switched-load feedback
- An auxiliary contact can indicate a mechanically linked contact state only to the extent supported by the relay design and datasheet.
- Coil-drive feedback indicates the coil command/energization path, not necessarily the load contact state.
- Load voltage/current sensing measures an electrical condition and does not automatically prove the intended load is functioning.
- Welded contacts, failed-open contacts and wiring faults require explicit diagnostic coverage analysis.

### 4.2 Fan feedback
- Tachometer feedback may indicate rotation pulses but requires defined pulse-per-revolution, voltage, conditioning and timeout semantics.
- Current sensing can indicate electrical consumption but cannot alone prove correct airflow or safe operation.
- Temperature feedback measures only the location and quantity specified by its sensor design.
- A driver fault pin reports only the faults covered by that driver's implementation.

A fan with no supported feedback must report observation as unavailable/unknown; software must not fabricate rotation or health.

## 5. Fault taxonomy

The following taxonomy is proposed for review. It is semantic guidance, not a shared Software contract or finalized numeric enum.

| Fault category | Example condition | Required reporting |
|---|---|---|
| COMMAND_INVALID | Invalid operation, channel or range | Reject without driver request |
| POLICY_UNRESOLVED | Authority/arbitration cannot be established | Fail closed; no driver request |
| DRIVER_UNAVAILABLE | Driver not initialized or not present | Mark unavailable; observed state unknown |
| DRIVER_TIMEOUT | Driver request exceeds approved time bound | Report timeout; do not assume outcome |
| DRIVER_REJECTED | Driver reports request rejected | Preserve requested vs applied distinction |
| FEEDBACK_MISSING | Required feedback not present | Observed state unknown; raise diagnostic |
| FEEDBACK_STALE | Last valid feedback exceeds approved freshness bound | Invalidate observation; raise diagnostic |
| FEEDBACK_IMPLAUSIBLE | Signal outside defined electrical/physical range | Mark invalid; raise diagnostic |
| FEEDBACK_CONTRADICTION | Feedback conflicts with expected transition or independent evidence | Mark degraded/fault; require defined reconciliation |
| ACTUATOR_MISMATCH | Requested/applied and observed states disagree beyond approved interval | Report mismatch; no automatic repeated actuation |
| OVERCURRENT | Hardware current monitor indicates threshold exceeded | Hardware protection acts independently; firmware records fault |
| OVERTEMPERATURE | Driver/motor thermal protection or sensor threshold | Preserve fault and safe-state report |
| SUPPLY_FAULT | Logic/load supply outside verified range | Mark subsystem unavailable/fault |
| WATCHDOG_OR_RESET | Unexpected restart or watchdog event | Start in unknown state; no automatic restore of outputs |
| INTERNAL_ERROR | Unexpected firmware failure | Fail closed; retain diagnostic where possible |

Fault severity, latching, clearing authority and numeric codes require HQ/Software review. No automatic fault-clear or retry policy is introduced.

## 6. Safe-state requirements

### 6.1 Firmware behavior
- Startup state remains conservative: desired/applied/observed unknown; availability starting; output permission denied.
- A reboot must not restore a previous ON command automatically.
- A missing or invalid board profile, driver configuration, identity, policy or command contract must fail closed.
- Driver unavailable, timeout or feedback invalid must never be translated into confirmed OFF.
- No unbounded retry loop or repeated switching in response to uncertain feedback.
- A fault must not be cleared solely because a later command was accepted.
- Diagnostics must distinguish the initiating fault, subsequent state and any recovery observation.
- Communication failure must not cause software to claim a physical state.
- The current firmware has no physical output permission grant path; FW-03E must not add one.

### 6.2 Hardware behavior
Hardware must independently establish a safe electrical default for reset, bootloader, high-impedance GPIO, unpowered MCU, brownout, disconnected control cable and driver power sequencing.

Software cannot guarantee safe electrical behavior while the MCU is reset, unpowered, crashed or disconnected. Any safety-critical de-energization, interlock, overcurrent/thermal protection and watchdog action must be designed and verified in hardware where required by hazard analysis.

### 6.3 System hazard review
Before a future implementation, HQ and Hardware must review:
- hazards from unintended energization;
- hazards from failure to de-energize;
- hazards from false-positive/false-negative feedback;
- hazards from welded relay contacts or semiconductor short circuit;
- hazards from fan stall, blocked rotor or thermal failure;
- hazards from reset/brownout and power sequencing;
- hazards from communication loss and stale commands;
- hazards from maintenance, wiring error and installation misuse.

This document is not a safety certification, hazard analysis approval or permission to connect mains.

## 7. Transition and mismatch handling requirements

A future state machine must define explicit transitions for:
1. Startup → unknown/unavailable.
2. Hardware/driver self-check → available or faulted.
3. Authorized logical request → request pending.
4. Driver result → applied/accepted or rejected/unavailable/timeout.
5. Valid feedback → observed state update.
6. Feedback deadline exceeded → stale/unknown.
7. Observed state inconsistent with expected state → mismatch fault/degraded state.
8. Recovery observation → recovery candidate, not automatic fault clear.
9. Explicit authorized fault-clear action, if approved in future → clear according to a reviewed policy only.

Exact timing, retries, state transition authority and recovery rules remain unresolved until hardware characteristics and system hazard analysis are available.

## 8. Diagnostics and event requirements

A future diagnostic event should carry, subject to approved shared contract:
- event sequence and event type;
- logical device/channel identifier;
- fault category and severity;
- source component;
- command/correlation identifier when applicable;
- desired/applied/observed state snapshot;
- state revision;
- validity/freshness indicator;
- timestamp and time-validity source;
- recovery/clear status and authority;
- privacy classification.

Existing FW-02 diagnostics are local, bounded and in-memory. FW-03E does not add storage, telemetry, network transport or a new shared schema. Sensitive identifiers and credentials must not appear in diagnostic payloads.

## 9. Verification requirements for a future implementation

Before physical implementation authorization, the verification plan must include (as applicable and only after separate approval):
- domain tests for all valid/invalid state transitions;
- tests that unresolved policy never reaches a driver;
- startup/reset state tests and no automatic output restoration;
- mock tests for driver rejection, timeout, unavailable and internal error;
- feedback-valid, missing, stale, out-of-range and contradictory input tests;
- fault latching/clearing authority tests;
- state mismatch and recovery tests;
- power sequencing/reset behavior review;
- hardware-in-the-loop tests under approved safe conditions;
- independent hardware safety review before any energized or mains-connected testing.

FW-03E itself performs no physical or hardware-in-the-loop tests.

## 10. Dependency and risk register

| ID | Dependency / risk | Owner | Blocks |
|---|---|---|---|
| FW03E-DEP-01 | Selected relay/fan driver and authoritative datasheets | Hardware | Feedback design |
| FW03E-DEP-02 | Defined physical quantity and feedback source for each channel | Hardware / Product / HQ | Observed-state semantics |
| FW03E-DEP-03 | Signal conditioning, isolation, thresholds and protection | Hardware | Feedback adapter |
| FW03E-DEP-04 | Freshness/sampling/timing requirements | Hardware / Firmware | Stale detection |
| FW03E-DEP-05 | Hazard analysis and system safe-state decision | HQ / Hardware | Any physical implementation |
| FW03E-DEP-06 | Reset/brownout/unpowered electrical behavior | Hardware | Safe-state validation |
| FW03E-DEP-07 | Fault severity/latching/clear authority | HQ / Software / Firmware | Fault lifecycle |
| FW03E-DEP-08 | Versioned diagnostics contract and privacy rules | HQ / Software | Diagnostics export |
| FW03E-DEP-09 | Separate physical implementation authorization | HQ | Any GPIO/actuator code |
| FW03E-DEP-10 | Hardware safety review and approved test procedure | Hardware / HQ | Energized testing |

## 11. FW-03E exit assessment

**Firmware planning completed:** independent state dimensions and feedback semantics documented; fault taxonomy proposed; startup, reset, stale/invalid feedback, mismatch and recovery requirements recorded; hardware versus firmware safe-state responsibilities separated; verification requirements and dependencies listed.

**Pending:** Hardware must identify actual feedback circuitry and its diagnostic coverage. HQ must approve system hazard analysis, safe-state decisions and any future fault-clear authority. Software must review shared diagnostics semantics when an authoritative contract is available.

FW-03E is a requirements baseline only. It does not change FW-02 runtime behavior, output permission or any shared contract.

---
**Prepared by:** HomeGrid Firmware Department  
**Review requested from:** HomeGrid Hardware Department, Software Department and HomeGrid HQ
