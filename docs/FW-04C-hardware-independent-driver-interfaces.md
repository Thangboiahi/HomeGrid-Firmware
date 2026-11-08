# FW-04C — Hardware-Independent Driver Interface Specification

**Authority:** `HG-HQ-ED-FW04-001` / `HG-HQ-DEC-20261108-FW04-001`  
**Status:** Interface specification only; no real driver implementation

## 1. Purpose

Specify logical, hardware-independent interfaces for future switch, relay, fan and feedback adapters. These interfaces separate application intent from physical electrical implementation and must be implementable by deterministic host mocks.

No real GPIO, PWM, LEDC, MCPWM, RMT, I2C, SPI or other ESP-IDF peripheral driver is authorized in FW-04C.

## 2. State and safety invariants

The following remain separate and must never be collapsed:
- **Desired:** what application policy requests.
- **Applied:** what an adapter reports it successfully accepted/applied at its interface boundary.
- **Observed:** what valid independent feedback reports about the physical system.
- **Availability:** whether a capability can currently be used.
- **Fault:** current/latched fault information.
- **Output permission:** authorization state; remains permanently DENIED in the current implementation.

An adapter returning ACCEPTED is not evidence that a relay contact changed, a fan rotated or a load received power. Without valid independent feedback, observed state is UNKNOWN.

## 3. Common logical result model

Proposed result categories:
- `ACCEPTED`: request accepted by the adapter boundary only.
- `REJECTED`: request invalid or not supported.
- `UNAVAILABLE`: capability not initialized or not present.
- `BUSY`: adapter cannot process now.
- `TIMEOUT`: operation did not complete within its contract.
- `FAULT`: adapter detected an error.
- `INVALID_CONFIGURATION`: required profile/configuration is invalid.
- `OUTPUT_DENIED`: application safety policy forbids the request.
- `INTERNAL_ERROR`: unexpected adapter failure.

Results include a stable logical capability ID and optional diagnostic code/correlation ID. No electrical values or raw GPIO identifiers are exposed to the application.

## 4. Logical interfaces

### 4.1 Switch request interface

Input: logical channel ID and requested logical state (ON/OFF).  
Output: typed request result.  
Rules:
- Must pass through application policy and output-permission checks.
- Does not expose GPIO or polarity.
- Does not update observed state.
- In the current system, output permission is DENIED and therefore real output requests remain blocked.

### 4.2 Relay adapter interface

Logical operations: initialize capability metadata, submit a logical channel request, report adapter availability and typed status.

Hardware-specific relay polarity, coil voltage/current, driver transistor/MOSFET, optocoupler, flyback protection, contact rating, isolation and default electrical state are outside this interface and must be established by Hardware. The interface must not encode normally-open/normally-closed assumptions.

### 4.3 Fan adapter interface

Logical operations: initialize fan capability metadata, submit an abstract fan command only after a future approved control model exists, report availability and typed status.

No PWM frequency, duty-cycle range, tachometer pulses/revolution, motor type, phase control, triac, voltage or current assumptions are defined here. Those are Hardware-owned inputs and HQ-approved product behavior. No fan command schema is finalized by this package.

### 4.4 Feedback interface

Input record should carry:
- logical feedback channel ID;
- value representation (known/unknown plus typed logical value);
- validity status;
- freshness/age metadata supplied by a future time source;
- source/diagnostic code;
- correlation reference where available.

Feedback validity states: VALID, MISSING, STALE, INVALID, CONTRADICTORY, UNAVAILABLE. Only VALID and fresh feedback may update observed state. All other states leave observed UNKNOWN or mark it invalid/unavailable according to approved application policy. A missing feedback signal must never be interpreted as OFF.

## 5. API ownership and layering

Application/domain owns policy and desired-state transitions. Logical ports define stable firmware-internal contracts. Board-support owns capability/profile validation. Concrete hardware adapters own electrical and peripheral details. Mock adapters implement the same ports for host tests.

No layer above a concrete adapter may import ESP-IDF GPIO/peripheral headers or use raw pin numbers. No shared backend API/schema is changed in FW-04C.

## 6. Error and fault propagation

- Adapter errors are typed and propagated without conversion to success.
- Rejected/unavailable/timed-out operations must not be recorded as physical OFF/ON.
- Fault records should distinguish configuration, availability, communication-with-adapter, timeout and feedback faults.
- Correlation IDs may link request, adapter result and feedback, but must not contain secrets or personal data.
- Fault-clear/retry/reconciliation authority remains an HQ decision.

## 7. Mock compatibility

A mock implementation must support deterministic injection of:
- accepted, rejected, unavailable, busy, timeout, fault and invalid-configuration results;
- valid, missing, stale, invalid and contradictory feedback;
- unsupported logical channel IDs;
- repeated requests and correlation matching.

Mock acceptance remains a software test result only. It must never claim physical actuation.

## 8. Non-goals and restrictions

No real GPIO or peripheral driver. No pin assignment. No relay/fan/pump operation. No electrical polarity, driver topology, isolation or ratings assumptions. No shared HomeGrid contract changes. No modification to deny-only output permission. Preserve ESP-IDF v6.1, target `esp32` and 2 MB application image configuration.

## 9. Review dependencies

Hardware must review feasibility and supply actual interface/electrical facts. Software must review mapping requirements for device identity, command, acknowledgement, desired/reported state and diagnostics. HQ must approve any shared contract, command policy or output authorization.

**FW-04C conclusion:** Logical switch/relay/fan/feedback boundaries and typed outcomes are specified without binding them to unverified hardware. Ready for FW-04D mock adapter and host-test work.

---
**Prepared by:** HomeGrid Firmware Department
