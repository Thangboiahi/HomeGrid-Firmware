# Change log

## FW-02F verification and final handover — 2026-11-08
- Added automated source-level safety regression tests for FW-02 domain, mock, application and diagnostics components.
- Added the FW-02 final handover report with host CI evidence, exclusion audit, integration status and HQ-owned dependency register.
- Recorded that FW-02 remains host/mock verified only; no ESP-IDF build, physical actuation or production readiness is claimed.

## FW-02E diagnostics and fault handling — 2026-11-08
- Added a bounded, fixed-capacity in-memory diagnostics event ring with chronological reads, saturating counters, overwrite accounting and sticky last-fault summary.
- Instrumented application startup, policy rejection, state errors and mock actuator outcomes with correlation IDs and state revisions.
- Added host tests for event validation, counters, chronological retention, ring overwrite and fault reporting; extended CI to run the new suite.
- Diagnostics have no clock, logger, storage, transport, GPIO, network or RTOS dependency; output permission remains permanently denied and observed state remains unknown.

## FW-02D mock actuator and application orchestration — 2026-11-08
- Added a deterministic host-only mock actuator that records requested values/counts and returns controlled outcomes; its safe default is unavailable.
- Added synchronous application orchestration that applies the explicit command-policy gate before calling the mock and records typed outcomes in the domain state.
- Added eight host tests for policy blocking, mock request recording, accepted/rejected/unavailable outcomes, invalid commands and repeated requests.
- Extended GitHub Actions to compile and run both C suites and the existing Python scaffold tests on FW-02 branches and pull requests.
- No GPIO, physical actuator, relay/fan, network, persistence, provisioning, OTA, credentials or shared-contract implementation was added.

## FW-02C state model and command processing — 2026-11-08
- Added a platform-independent pure-C switch-domain API and implementation with distinct desired/applied/observed/availability/fault/output-permission dimensions.
- Implemented safe UNKNOWN startup, permanent output DENIED, explicit command policy gating (unresolved/denied commands do not mutate state), typed mock-result state recording and enum/state validation.
- Added eight native C test cases, host build instructions and a GitHub Actions workflow to compile/run the C suite and existing Python scaffold tests.
- Added `docs/FW-02C-implementation-report.md`. GitHub Actions pass was confirmed by the user; detailed run logs were not independently retrieved in this session. No physical GPIO, actuator, network, persistence, provisioning, OTA or shared-contract code was added.

## FW-02B firmware architecture — 2026-11-08
- Added `docs/FW-02B-firmware-architecture.md` defining the layered ports-and-adapters architecture, component responsibilities, dependency direction, source layout, state ownership/invariants, command boundary, mock-only actuator contract, identity/configuration/diagnostics ports, error model, task/resource model, test strategy and future integration gates.
- Kept the package architecture-only: no firmware source, GPIO, actuator, network, provisioning, OTA, persistence or shared-contract implementation was added.
- FW-02B architecture document was reread from GitHub and accepted by the Firmware Department. PR integration review remains pending; no source implementation was introduced.

## FW-02A requirements traceability and HQ authorization — 2026-11-08
- Recorded HQ authorization `HG-HQ-ED-FW02-001` / `HG-HQ-DEC-20261108-FW02-001` and converted the former FW-02 proposal into the approved six-package execution plan with 24 stable requirement IDs, authority/evidence register, interface ownership, non-assumptions, safety constraints, open dependencies and FW-02B entry recommendation.
- Updated `docs/FW-02-work-package.md` to reflect the HQ decision and explicit exclusions. No GPIO, actuator, radio/network, provisioning, OTA, credential or shared-contract implementation was introduced.
- Changes are on branch `fw-02a-requirements-traceability`; FW-02A requirements and related documentation were reread from GitHub and accepted by the Firmware Department; PR #1 remains open for integration review/merge.

## FW-01F final acceptance and FW-02 proposal — 2026-11-08
- Audited FW-01A–FW-01E reports and evidence, reconciled the board records to measured ESP32-D0WD-V3 v3.0 / 4 MB flash / CH9102 COM8 (VID_1A86:PID_55D4), and retained the 2 MB image-header warning and unknown board/module markings.
- Formally closed FW-01 following Department acceptance of FW-01E. Created `docs/FW-01-final-acceptance-report.md` with evidence index and non-blocking hardware follow-ups.
- Prepared `docs/FW-02-work-package.md` for HQ decision only; no FW-02 implementation began and no relay/mains work is authorized.

## FW-01E physical verification — 2026-11-08
- Identified the development board on COM8 as ESP32-D0WD-V3 revision 3.0 behind a CH9102 bridge (VID_1A86:PID_55D4); confirmed 4 MB physical flash and separated COM3 and COM4–COM6 as Intel AMT/Dell LTE devices.
- Verified exact-size backups of the bootloader, partition-table, and 1 MiB application regions, then recorded their SHA-256 hashes before controlled writes.
- Flashed only bootloader `0x1000`, partition table `0x8000`, and application `0x10000`; esptool read-back verification passed for all three. No full-chip erase was used.
- Captured raw serial boot plus a 600-second runtime: ESP-IDF v6.1 startup, FreeRTOS app task, heap diagnostics, and 60 post-boot heartbeats at 10.000–10.010 second intervals; no unexpected reset or crash observed.
- Added FW-01E hardware report, separate serial analysis, raw serial/timestamp evidence, and retained partial failed-capture attempts. Recorded the 4 MB detected vs 2 MB image-header warning and unresolved carrier/module markings.

## FW-01D first ESP-IDF build — 2026-11-08
- Verified the EIM-activated ESP-IDF v6.1 environment and selected the Department-approved `esp32` target with `idf.py set-target esp32`.
- Built HomeGrid Diagnostic Firmware v0.1 successfully with `idf.py build`; full captured output and compiler/tool versions are in `docs/FW-01D-build.log` and `docs/FW-01D-build-report.md`.
- Recorded generated bootloader, partition-table, ELF, and application binaries with sizes and SHA-256 hashes; no firmware source changes were needed.
- Reran all six host-side tests successfully. No physical board was flashed or operated; FW-01 remains open pending Department review and later hardware evidence.
- Documented ESP-IDF Kconfig/CMake notices and EIM environment Unicode/Python-path warnings without changing Windows settings.

## FW-01A installation retry — 2026-11-08
- Installed the checksum-verified official EIM CLI v0.20.1 in the current user's LocalAppData folder.
- EIM checked out ESP-IDF v6.1 from Espressif tag `v6.1` at commit `fff9895c82d744c7237be8847347bdd1b07c6643` and completed submodule retrieval.
- Stopped before tool installation completed because EIM unexpectedly selected `C:\\Espressif\\dist` for downloads despite the requested user-level base path; recorded partial state and exact paths in the installation report.
- CMake, Ninja, compiler and environment verification remain pending. No target-selection, build, or flash command was run.

## FW-01C preparation — 2026-11-08
- Recorded the Department-accepted Original ESP32 / ESP-WROOM-32 / CP2102 hardware family and approved `esp32` target, while clearly marking local evidence and exact board revision gaps.
- Added a board specification with ESP32 GPIO/boot-strapping restrictions, preliminary future-use GPIO candidates, and flash/module-revision verification steps.
- Added a reference-only board configuration header with unknown-capacity sentinels and no GPIO mapping; the diagnostic application remains board-logic independent.
- Kept firmware installation, target selection, build, flash, and external-circuit work out of scope.

## 0.1.1 — 2026-11-08
- Corrected the FW-01 handoff package so every documented project path exists.
- Added root and component ESP-IDF CMake files.
- Added Codex `AGENTS.md` guardrails and a read-only first-task brief.
- Added the board-identification worksheet and host scaffold tests.
- Clarified that host tests are not ESP-IDF build or physical hardware evidence.
- Retained the board-neutral C diagnostic application and ESP-IDF v6.1 baseline.

## 0.1.0 — 2026-11-08
- Initial board-neutral C diagnostic application scaffold.
- Initial FW-01 milestone report and development notes.
