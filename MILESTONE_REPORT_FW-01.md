# HomeGrid Firmware Milestone Report — FW-01

**Report date:** 2026-11-08  
**Package version:** 0.1.1  
**Milestone:** FW-01 — Development environment and hardware identification  
**Status:** **COMPLETE — formally closed by Firmware Department FW-01F decision (2026-11-08).** The environment, ESP32 target build, controlled flash, and physical ten-minute diagnostic runtime passed review. Exact carrier PCB revision/module suffix and PSRAM status remain documented as non-blocking hardware-identification follow-ups.

## Completed in repository preparation

- Selected ESP-IDF v6.1 as the pinned initial framework baseline.
- Established C + ESP-IDF + FreeRTOS for firmware; Python reserved for host-side testing/utilities.
- Added an ESP-IDF root/component CMake structure without hard-coding the chip target.
- Added a board-neutral diagnostic application reporting SoC features/runtime heap and a 10-second heartbeat.
- Added `AGENTS.md`, a Codex first-task audit brief, board-identification worksheet, host tests and corrected build documentation.
- Completed a read-only host audit and recorded its findings in `docs/host-audit.md`; the six host scaffold tests passed.
- Attempted the authorized FW-01A installation. EIM/ESP-IDF v6.1 installation was blocked by network gateway timeouts; see `docs/FW-01A-installation-report.md`. No software was installed.
- Recorded the Department-accepted generic ESP32 development board, Original ESP32, ESP-WROOM-32, CP2102, Micro-USB and approved ESP-IDF target `esp32` in `docs/board-identification.md`. The exact carrier model/revision and local USB/photo evidence remain outstanding.
- Added `docs/board-specification.md` and a reference-only `main/board_config.h`. GPIO caveats and future candidate pins are documented; no pin is assigned and the header is not used by application code.
- FW-01A recovery installed and verified ESP-IDF v6.1.0, its Xtensa toolchain, CMake, Ninja, and Python dependencies under the authorized per-user `.espressif` root; see `docs/FW-01A-installation-report.md`.
- FW-01D selected the approved `esp32` target and completed the first actual `idf.py build` using ESP-IDF v6.1. The application compiled and linked without source changes; full build evidence, tool versions, warnings/notices, and binary hashes are in `docs/FW-01D-build-report.md` and `docs/FW-01D-build.log`.
- The six host-side tests passed again after the target build. These remain host-only checks.
- FW-01E positively associated COM8 / CH9102 (VID_1A86:PID_55D4) with ESP32-D0WD-V3 revision 3.0 using esptool, detected 4 MB flash, verified pre-flash backups, and wrote only the bootloader, partition table, and application ranges. Read-back hashes passed; full-chip erase was not used.
- Captured the physical boot and full 600-second diagnostic runtime. The app reported ESP-IDF v6.1, FreeRTOS startup, stable heap diagnostics, and 60 regular ten-second heartbeats with no unexpected reset/crash found. See `docs/FW-01E-hardware-verification-report.md`, `docs/FW-01E-analysis.md`, and raw serial evidence.

## Host-side checks

The six static tests passed during the FW-01A host audit, after FW-01C preparation, and again after the FW-01D build. Results validate repository scaffold/guardrails only; they do not prove physical hardware operation.

## Not verified

- Exact carrier vendor/model/revision and readable module suffix from physical markings.
- PSRAM capacity/configuration remains unknown.
- Exact module/board documentation to reconcile module suffix and fitted flash option.

## Risks and controls

- **Wrong target:** Department accepted `esp32`; esptool identified ESP32-D0WD-V3 revision 3.0 and the FW-01D image booted on it. Exact carrier-board/module markings remain pending.
- **Flash-size metadata:** physical capacity is 4 MB while the current image header is configured for 2 MB; the 2 MB partition layout fits and booted, but firmware cannot address the upper 2 MB under this configuration.
- **Wrong GPIO/electrical assumptions:** no GPIO or external module control is present.
- **Mains safety:** no mains or relay testing is allowed in FW-01.
- **Toolchain drift:** use only the pinned v6.1 release and matching tools.
- **Contract drift:** no shared HomeGrid communication contract is changed.

## FW-01F closure decision

The Firmware Department accepted FW-01E and authorized final closure. Acceptance is supported by the completed ESP-IDF v6.1 build, verified backups, region-limited controlled flash, esptool read-back verification, and the 600-second serial runtime evidence. The 4 MB physical flash versus 2 MB image-header setting is explicitly documented; the 2 MB partition layout fits and booted. No full-chip erase occurred. The exact carrier PCB revision, module shield suffix, and PSRAM status remain unknown and are recorded as non-blocking follow-ups rather than inferred values.

**Next:** Present the FW-01F acceptance report and proposed FW-02 work package for review before implementation. GPIO candidates remain provisional. Relay actuation, mains-voltage work, and HomeGrid contract changes are outside this closure and require separate authorization/review.

## HomeGrid HQ impact

No shared interfaces or communication contracts changed. This report is a firmware milestone status record, not a request to approve a contract change.
