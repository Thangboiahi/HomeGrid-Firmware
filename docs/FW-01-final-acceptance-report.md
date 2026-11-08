# HomeGrid FW-01F — Final Acceptance and Closure

**Decision date:** 2026-11-08  
**Department decision:** FW-01E accepted; **FW-01 is formally closed**.  
**Basis:** Audited FW-01A–FW-01E reports, build artifacts, backup hashes, flash log, and physical serial evidence.

## Accepted results

- **Environment:** EIM-managed ESP-IDF v6.1.0, Git tag `v6.1`, commit `fff9895c82d744c7237be8847347bdd1b07c6643`; Xtensa GCC 15.2.0, CMake 4.0.3, Ninja 1.12.1, Python 3.14.3. Installation and verification evidence: [FW-01A installation report](FW-01A-installation-report.md).
- **Target build:** `idf.py set-target esp32` and `idf.py build` succeeded. ESP-IDF generated the application, bootloader, and partition table. Build report and complete captured output: [FW-01D build report](FW-01D-build-report.md), [FW-01D build log](FW-01D-build.log).
- **Physical chip:** esptool positively identified ESP32-D0WD-V3 revision 3.0 on COM8. Physical flash identification reported 4 MB.
- **USB bridge:** Windows enumerated `USB-Enhanced-SERIAL CH9102 (COM8)`, VID `1A86`, PID `55D4`. This replaces the preliminary CP2102 expectation. Intel AMT SOL COM3 and Dell LTE COM4–COM6 were excluded.
- **Image configuration:** ESP-IDF image header is configured for 2 MB, 40 MHz, DIO. The partition table is NVS at `0x9000`/24 KiB, PHY init at `0xF000`/4 KiB, and factory app at `0x10000`/1 MiB. This layout fits within 2 MB and therefore within the detected 4 MB flash.
- **Boot warning:** the boot log reports 4096 kB detected versus 2048 kB in the image header and says ESP-IDF uses the image-header size. This is recorded as a configuration limitation; the upper 2 MB is outside the current image view. Successful boot/runtime does not resolve exact module/carrier flash BOM details.
- **Backups:** pre-flash ranges matched exact requested lengths and SHA-256 checksums; values are in the [FW-01E hardware report](FW-01E-hardware-verification-report.md). Backup files are in `../build/fw01e-backup/`.
- **Controlled writes:** esptool wrote and verified only bootloader at `0x1000`, partition table at `0x8000`, and application at `0x10000`. Each read-back hash verified. No full-chip erase was issued. See [flash log](FW-01E-flash.log).
- **Physical runtime:** after EN/RESET, the serial log confirms ROM/ESP-IDF v6.1 boot, application startup, FreeRTOS main task, chip revision, capability flags, and heap diagnostics. The timed capture ran 600 seconds and recorded 60 post-boot heartbeats from 299 ms through 590,309 ms uptime at 10,000–10,010 ms intervals. Free and minimum free heap stayed at 303,328 bytes. No unexpected reset, boot loop, watchdog, exception, crash, panic, abort, assertion, or brownout was observed.
- **Host tests:** all six repository host-side tests passed during FW-01A/FW-01C/FW-01D verification. They remain host checks and are not a substitute for the physical evidence above.
- **Scope controls:** no relay/fan module, mains circuit, GPIO output, Wi-Fi connection, BLE operation, production contract, or firmware architecture change was introduced.

## Evidence index

- [FW-01A installation report](FW-01A-installation-report.md)
- [FW-01D build report](FW-01D-build-report.md)
- [FW-01D full build log](FW-01D-build.log)
- [FW-01E hardware verification report](FW-01E-hardware-verification-report.md)
- [FW-01E reviewed analysis](FW-01E-analysis.md)
- [FW-01E raw serial log](FW-01E-serial.log)
- [FW-01E serial timestamps](FW-01E-serial-timestamps.csv)
- [FW-01E esptool flash log](FW-01E-flash.log)
- [Board identification record](board-identification.md)
- [Board specification](board-specification.md)
- [Milestone report](../MILESTONE_REPORT_FW-01.md) and [change log](../CHANGELOG.md)

All named evidence artifacts were checked to exist during this final audit. The three earlier partial capture logs are retained separately and are explicitly excluded from the accepted ten-minute run.

## Outstanding, non-blocking hardware details

1. Exact carrier-board manufacturer/model and PCB revision; no legible board photos or local markings were supplied.
2. Exact ESP-WROOM-32 shield suffix/order code and module revision; department-provided family name is recorded but the marking was not read locally.
3. PSRAM presence/capacity is unknown.
4. The installed USB bridge is CH9102, not the previously stated CP2102; update downstream inventory from the observed VID/PID.
5. The configured image exposes a 2 MB flash view on a detected 4 MB device. Any future change to flash-size/partition configuration requires a separately reviewed work item.

These details do not invalidate the accepted diagnostic build, bounded writes, or observed ten-minute run. They remain required inputs before board-specific GPIO assignment or electrical integration.

## Closure and next-step boundary

**FW-01 acceptance: COMPLETE / CLOSED.** The milestone establishes the pinned build environment, accepted ESP32 target, reproducible diagnostic build, controlled board write, and physical runtime evidence. The repository milestone status is updated accordingly.

The proposed [FW-02 Smart Switch Firmware Foundation work package](FW-02-work-package.md) is a review document only. **No FW-02 implementation is authorized by this closure report.** FW-02 scope must be reviewed and approved before code changes. Relay actuation and mains-voltage testing are explicitly not authorized.
