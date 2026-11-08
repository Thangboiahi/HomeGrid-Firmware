# FW-02C — State Model and Command Processing Implementation

**Milestone:** FW-02 — Smart Switch Firmware Foundation  
**Work package:** FW-02C — State Model and Command Processing  
**Authority:** `HG-HQ-ED-FW02-001` / `HG-HQ-DEC-20261108-FW02-001`  
**Architecture reference:** `docs/FW-02B-firmware-architecture.md`  
**Status:** Implementation committed; local mirrored test passed; repository CI verification pending  
**Branch:** `fw-02c-state-command-domain`

## 1. Implementation inventory

| Path | Purpose |
|---|---|
| `components/homegrid_domain/CMakeLists.txt` | ESP-IDF component registration for the pure-C domain source |
| `components/homegrid_domain/include/homegrid/switch_state.h` | Platform-independent state, command, policy and result types/API |
| `components/homegrid_domain/switch_state.c` | Safe initialization, enum/state validation, command policy gate and mock-result state transitions |
| `tests/host/test_switch_state.c` | Native C assertions for startup, policy, state and mock-result behavior |
| `tests/host/README.md` | Reproducible host compiler invocation and test scope |
| `.github/workflows/fw02-host-tests.yml` | CI job compiling/running the C domain tests and existing Python scaffold suite on PR/push |

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

A GitHub Actions workflow was added to compile with C11, `-Wall -Wextra -Werror -pedantic`, execute the C test binary, and run the existing Python scaffold suite.

**Local check:** A temporary local mirror of the domain implementation and eight test cases was compiled with GCC 14.2.0 using C11, `-Wall -Wextra -Werror -pedantic`; all eight assertions passed. This was not a checkout/build of the GitHub branch and is not treated as repository CI evidence.

**CI status:** The GitHub Actions workflow file is committed, but no workflow run is currently visible through the available repository run lookup. Do not claim a CI pass, ESP-IDF build or physical test. The exact branch CI result still needs to be retrieved from GitHub.

## 5. Remaining verification gates

- [ ] GitHub Actions C host test passes and its logs are reviewed.
- [ ] Existing Python scaffold tests pass in CI and their logs are reviewed.
- [ ] Review actual compiler/test logs and correct any failures.
- [ ] Read back all implementation/test/workflow files from GitHub.
- [ ] Audit diff for forbidden hardware/network/storage APIs and secrets.
- [ ] Firmware Department acceptance after evidence review.

## 6. FW-02D entry recommendation

After the CI run passes and the implementation is reviewed, proceed to FW-02D to add the mock actuator adapter with deterministic request recording and controlled outcomes, then expand host tests around application orchestration. Keep the mock as the only actuator backend and retain the permanent output-denied invariant.

---
**Prepared by:** HomeGrid Firmware Department  
**Document control:** FW-02C implementation report; acceptance depends on captured automated test evidence.
