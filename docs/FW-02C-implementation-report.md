# FW-02C — State Model and Command Processing Implementation

**Milestone:** FW-02 — Smart Switch Firmware Foundation  
**Work package:** FW-02C — State Model and Command Processing  
**Authority:** `HG-HQ-ED-FW02-001` / `HG-HQ-DEC-20261108-FW02-001`  
**Architecture reference:** `docs/FW-02B-firmware-architecture.md`  
**Status:** Department verification gate passed based on user confirmation of successful GitHub Actions; detailed run logs were not independently retrieved in this session  
**Branch:** `fw-02c-state-command-domain`

## 1. Implementation inventory

| Path | Purpose |
|---|---|
| `components/homegrid_domain/CMakeLists.txt` | ESP-IDF component registration for the pure-C domain source |
| `components/homegrid_domain/include/homegrid/switch_state.h` | Platform-independent state, command, policy and result types/API |
| `components/homegrid_domain/switch_state.c` | Safe initialization, enum/state validation, command policy gate and mock-result state transitions |
| `tests/host/test_switch_state.c` | Native C assertions for startup, policy, state and mock-result behavior |
| `tests/host/README.md` | Reproducible host compiler invocation and test scope |
| `.github/workflows/fw02-host-tests.yml` | CI job compiling/running the C domain tests and existing Python scaffold tests on PR/push |

## 2. Implemented behavior

- Startup initializes desired, applied and observed states to UNKNOWN.
- Availability starts at STARTING; fault starts at NONE.
- Output permission has only one representable FW-02 value: DENIED. There is no grant enumerator or setter.
- Command processing accepts only known command/source/policy enum values.
- UNRESOLVED or DENY policy returns a typed status without changing state.
- An explicit ALLOW changes desired state only; it does not claim actuator application or physical observation.
- Mock acceptance updates software-side applied state to ACCEPTED while observed remains UNKNOWN.
- Mock rejection and mock unavailability record distinct applied/availability/fault outcomes.
- Invalid state values and unsupported commands are rejected.
- State transitions are synchronous and use no ESP-IDF, FreeRTOS, GPIO, network or storage APIs.

## 3. Safety boundary audit

The component exposes no physical actuator driver. It contains no GPIO pin, polarity, relay, fan, voltage, timing or board-specific fields. The mock result API cannot update observed state to ON/OFF or grant output permission.

No command priority is implemented. The caller must supply an explicit policy result; the safe default is UNRESOLVED. No persistent state, device identity, credential, provisioning, radio/network, telemetry-export or OTA behavior is implemented.

## 4. Test coverage added

The C host test source covers:
1. Safe startup unknown/denied state.
2. Unresolved policy no-mutation behavior.
3. Denied policy no-mutation behavior.
4. Allowed command changes desired state only.
5. Mock acceptance is not physical observation.
6. Mock rejection creates a degraded fault state.
7. Mock unavailability creates an unavailable fault state.
8. Unsupported command rejection without mutation.

A GitHub Actions workflow compiles with C11, `-Wall -Wextra -Werror -pedantic`, executes the C test binary, and runs the existing Python scaffold suite.

**Local check:** A temporary local mirror of the domain implementation and eight test cases was compiled with GCC 14.2.0 using C11, `-Wall -Wextra -Werror -pedantic`; all eight assertions passed. This was not a checkout/build of the GitHub branch.

**CI status:** The user confirmed that the GitHub Actions checks passed. The detailed run logs were not independently retrieved in this session; no ESP-IDF build or physical hardware test is implied.

## 5. Verification and acceptance

- [x] User confirmed GitHub Actions passed.
- [x] Existing Python scaffold suite is included in the same workflow; detailed job logs were not independently retrieved.
- [x] Implementation and test sources were reread from GitHub.
- [x] Safety boundaries and permanent output-denied invariant reviewed.
- [x] Firmware Department verification gate accepted for progression to FW-02D.

## 6. FW-02D entry recommendation

FW-02C verification gate is passed. Proceed to FW-02D to add the mock actuator adapter with deterministic request recording and controlled outcomes, then expand host tests around application orchestration. Keep the mock as the only actuator backend and retain the permanent output-denied invariant.

---
**Prepared by:** HomeGrid Firmware Department  
**Document control:** FW-02C verification gate passed on user confirmation; no hardware or physical actuation approval is implied.
