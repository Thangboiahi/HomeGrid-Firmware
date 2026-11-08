# FW-02D — Mock Actuator and Application Orchestration Report

**Milestone:** FW-02 — Smart Switch Firmware Foundation  
**Work package:** FW-02D — Mock Actuator and Host Tests  
**Authority:** `HG-HQ-ED-FW02-001` / `HG-HQ-DEC-20261108-FW02-001`  
**Predecessor:** FW-02C verification gate passed based on user confirmation of successful GitHub Actions.  
**Architecture reference:** `docs/FW-02B-firmware-architecture.md`  
**Status:** Host CI verification passed; Firmware Department acceptance recorded  
**Branch:** `fw-02d-mock-actuator-application`

## 1. Implementation inventory

| Path | Purpose |
|---|---|
| `components/homegrid_mock/CMakeLists.txt` | ESP-IDF component registration for the host/mock adapter |
| `components/homegrid_mock/include/homegrid/mock_actuator.h` | Mock request record and controlled-result API |
| `components/homegrid_mock/mock_actuator.c` | Deterministic mock adapter; no hardware access |
| `components/homegrid_application/CMakeLists.txt` | ESP-IDF component registration for orchestration |
| `components/homegrid_application/include/homegrid/application.h` | Application state, result and orchestration API |
| `components/homegrid_application/application.c` | Synchronous command-to-domain-to-mock orchestration |
| `tests/host/test_fw02d_application.c` | Eight native C tests for orchestration and mock behavior |
| `tests/host/README.md` | Reproduction commands for both C test suites |
| `.github/workflows/fw02-host-tests.yml` | CI compiles/runs FW-02C and FW-02D suites, then Python scaffold tests |

## 2. Implemented behavior

- The mock starts with an UNAVAILABLE result. A test must explicitly select ACCEPTED, REJECTED, UNAVAILABLE or INTERNAL_ERROR to change that behavior.
- Each valid request records the requested logical value and increments a request counter. The selected result is deterministic and remains in effect until changed.
- The mock accepts only logical OFF/ON values and does not contain GPIO, relay, fan, timing, voltage, board or peripheral controls.
- The application initializes the existing safe domain state and mock adapter.
- The application calls the domain policy gate first. UNRESOLVED or DENY results do not call the mock and do not mutate desired state.
- Only an explicit ALLOW reaches the mock adapter.
- Mock results are recorded through the existing domain transition API. They update software-side applied/availability/fault fields only.
- Observed state remains UNKNOWN and output permission remains DENIED for all outcomes.
- The orchestration is synchronous. No worker task, queue, callback, transport, persistence or shared global state was added.

## 3. Test coverage added

Eight host-native C tests cover:
1. Safe application startup and unavailable-by-default mock behavior.
2. Unresolved policy blocks the mock.
3. Denied policy blocks the mock.
4. Explicitly allowed command is recorded by the mock without physical-state claims.
5. Mock rejection is reflected in software state.
6. Mock unavailability is reflected in software state.
7. Invalid command does not call the mock.
8. Repeated requests are counted and the last request is recorded deterministically.

The CI workflow compiles with C11 and `-Wall -Wextra -Werror -pedantic`, runs both C test executables, and runs the existing Python scaffold tests.

## 4. Safety and scope audit

- No physical GPIO implementation or actuator driver exists in this package.
- No relay, fan, mains, external circuit or hardware test is performed or authorized.
- No radio, network, MQTT, provisioning, OTA, credential, persistent-storage or shared-contract code is added.
- No output-permission grant path is introduced.
- No local/remote command priority or restart-state policy is assumed.
- The mock is the only actuator adapter in this work package.

## 5. Verification status

- [x] New files and component boundaries committed to the FW-02D branch.
- [x] Host tests and workflow commands added.
- [x] Source-level review confirms no GPIO/peripheral/network/storage implementation in the new mock/application sources.
- [x] GitHub Actions run 12 for commit `0baa013227729e2739ce798891163fbea957325d` completed successfully.
- [x] Job `host-tests` completed successfully; FW-02C C tests, FW-02D application/mock C tests, and six Python scaffold tests all passed.
- [x] Firmware Department verification gate accepted; proceed to FW-02E.

No local compiler run or branch CI pass is claimed in this report. The FW-02D gate remains open until GitHub Actions completes successfully.

---
**Prepared by:** HomeGrid Firmware Department  
**Document control:** FW-02D implementation report; host/mock-only work package.
