# FW-02E — Diagnostics and Fault Handling Report

**Milestone:** FW-02 — Smart Switch Firmware Foundation  
**Work package:** FW-02E — Diagnostics and Fault Handling  
**Authority:** `HG-HQ-ED-FW02-001` / `HG-HQ-DEC-20261108-FW02-001`  
**Predecessor:** FW-02D host CI passed; Firmware Department acceptance recorded.  
**Architecture reference:** `docs/FW-02B-firmware-architecture.md`  
**Status:** Implementation committed; FW-02E branch CI verification pending  
**Branch:** `fw-02e-diagnostics-fault-handling`

## 1. Implementation inventory

| Path | Purpose |
|---|---|
| `components/homegrid_diagnostics/CMakeLists.txt` | ESP-IDF component registration |
| `components/homegrid_diagnostics/include/homegrid/diagnostics.h` | Bounded diagnostics API and event/snapshot types |
| `components/homegrid_diagnostics/diagnostics.c` | Fixed-capacity event ring, counters, validation and snapshot access |
| `components/homegrid_application/include/homegrid/application.h` | Application diagnostics ownership |
| `components/homegrid_application/application.c` | Records startup, policy rejection, state errors and mock outcomes |
| `components/homegrid_application/CMakeLists.txt` | Diagnostics component dependency |
| `tests/host/test_fw02e_diagnostics.c` | Host tests for diagnostics behavior |
| `.github/workflows/fw02-host-tests.yml` | Adds FW-02E C host suite to CI |
| `tests/host/README.md` | Reproducible test commands |
| `CHANGELOG.md` | FW-02E change record |

## 2. Diagnostics behavior

- Diagnostics retain the newest eight events in a fixed-size in-memory ring.
- Events are readable oldest-first and include sequence, event type, command correlation ID, state revision and fault classification.
- Counters track total events, overwritten events, policy rejections, state errors and each mock outcome.
- Counters saturate at `UINT32_MAX`; event storage remains bounded.
- The summary exposes the last non-NONE fault as a sticky diagnostic indicator; this package adds no automatic fault-clear policy.
- Invalid event types, fault values, pointers and out-of-range event reads are rejected.
- Application instrumentation records startup, unresolved/denied policy results, state errors and mock accepted/rejected/unavailable/internal-error outcomes.

## 3. Safety and scope

- No timestamp is invented: the diagnostics component has no clock dependency.
- No logging backend, persistent storage, telemetry, transport or credential handling is introduced.
- No GPIO, relay, fan, peripheral, mains or physical actuator code is introduced.
- No RTOS worker, queue or asynchronous task is introduced.
- Output permission remains permanently DENIED; observed state remains UNKNOWN.
- Local/remote authority, restart behavior and fault-clear policy remain unresolved and are not inferred.

## 4. Host verification plan

The CI workflow compiles with C11, `-Wall -Wextra -Werror -pedantic` and runs:
1. FW-02C domain tests.
2. FW-02D application/mock tests, now linked with diagnostics.
3. FW-02E diagnostics tests.
4. Existing Python FW-01 scaffold tests.

FW-02E is not accepted until the branch CI run and job logs are reviewed. No ESP-IDF build or physical hardware test is implied.

## 5. Exit checklist

- [x] Fixed-capacity diagnostics event ring implemented.
- [x] Fault and outcome counters implemented with saturation.
- [x] Application startup, policy rejection and mock outcomes instrumented.
- [x] Host test suite added and CI workflow extended.
- [x] Safety exclusions and unresolved policies preserved.
- [ ] FW-02E GitHub Actions run passes.
- [ ] CI job logs reviewed.
- [ ] Firmware Department acceptance recorded.

---
**Prepared by:** HomeGrid Firmware Department  
**Document control:** FW-02E host/mock-only diagnostics implementation; verification pending.
