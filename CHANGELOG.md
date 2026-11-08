# Change log

## FW-01F final acceptance and FW-02 proposal — 2026-11-08
- Audited FW-01A–FW-01E reports and evidence, reconciled the board records to measured ESP32-D0WD-V3 v3.0 / 4 MB flash / CH9102 COM8 (VID_1A86:PID_55D4), and retained the 2 MB image-header warning and unknown board/module markings.
- Formally closed FW-01 following Firmware Department acceptance of FW-01E. Created `docs/FW-01-final-acceptance-report.md` with evidence index and non-blocking hardware follow-ups.
- Prepared `docs/FW-02-work-package.md` for Department review only; no FW-02 implementation began and no relay/mains work is authorized.

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
- Stopped before tool installation completed because EIM unexpectedly selected `C:\Espressif\dist` for downloads despite the requested user-level base path; recorded partial state and exact paths in the installation report.
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
