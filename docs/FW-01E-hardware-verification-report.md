# FW-01E — Physical Hardware Verification Report

**Date:** 2026-11-08 (Asia/Calcutta)  
**Diagnostic firmware:** HomeGrid Diagnostic Firmware v0.1  
**ESP-IDF:** v6.1.0, target `esp32`  
**Result:** **Physical flash and ten-minute diagnostic runtime verified.**

## Phase 1 — Device discovery

Windows CIM discovery was unavailable, so discovery used `pnputil /enum-devices /connected` and `HKLM:\HARDWARE\DEVICEMAP\SERIALCOMM`.

| Item | Evidence |
|---|---|
| Serial device | `USB-Enhanced-SERIAL CH9102 (COM8)`, manufacturer `wch.cn` |
| USB hardware ID | `USB\VID_1A86&PID_55D4` (VID `1A86`, PID `55D4`; unique suffix omitted) |
| Serial mapping | `\Device\Serial2 : COM8` |
| Confirmed SoC | `ESP32-D0WD-V3`, revision v3.0, confirmed by esptool on COM8 |
| Features | Wi-Fi, Bluetooth, dual core + LP core, 240 MHz; 40 MHz crystal |
| Physical flash | 4 MB detected by esptool `flash-id` |

COM3 is Intel(R) AMT SOL. COM4–COM6 belong to the Dell DW5821e LTE modem interfaces. They were not treated as the development board. COM8's bridge is CH9102, differing from the prior CP2102 family description; the ESP32 chip query positively established that COM8 reaches the development board. Exact carrier maker/model and module shield suffix remain unrecorded.

## Phase 2 — Backup and compatibility

Before programming, esptool read and verified these regions from the confirmed 4 MB flash. All backup files match the required sizes:

| Saved region | Flash range | Size | SHA-256 |
|---|---:|---:|---|
| [`bootloader-region.bin`](../build/fw01e-backup/bootloader-region.bin) | `0x1000`–`0x7FFF` | 28,672 (`0x7000`) bytes | `A7EE858DF9A67EE8309A47D8A2AFE5AAA632C7E742881645350ADEB813FEEA37` |
| [`partition-table-region.bin`](../build/fw01e-backup/partition-table-region.bin) | `0x8000`–`0x8FFF` | 4,096 (`0x1000`) bytes | `E9EF7F3348C1503A55106EFC99F03356D4A96B6766B6A60401872990594304ED` |
| [`app-region.bin`](../build/fw01e-backup/app-region.bin) | `0x10000`–`0x10FFFF` | 1,048,576 (`0x100000`) bytes | `6B19A70671B67D1144B8D5065EFD5584FB25F4B9557DC529CF58C3109EB6D566` |

ESP-IDF's decoded partition table contains NVS at `0x9000` (24 KiB), PHY init at `0xF000` (4 KiB), and a factory application at `0x10000` (1 MiB). The firmware image setting is 2 MB, smaller than detected physical 4 MB; the full configured partition layout fits within that 2 MB view. This compatibility was corroborated by a successful boot. The boot log reports that 4 MB was detected but ESP-IDF uses the 2 MB size in the image header. The unused upper flash is not exposed by the current image configuration.

The existing FW-01D binaries were used without rebuild:

| Artifact | Size | Flash offset | SHA-256 |
|---|---:|---:|---|
| [`bootloader.bin`](../build/bootloader/bootloader.bin) | 26,176 bytes | `0x1000` | `63BC36077F3FDD491E084684C660C655608515460DF585FAC7C485F0058B4D75` |
| [`partition-table.bin`](../build/partition_table/partition-table.bin) | 3,072 bytes | `0x8000` | `7F00B6C042A89B15B0CAC534F82ED988CAF29278FF5700B0C511EB1B5BB7C820` |
| [`homegrid_fw01.bin`](../build/homegrid_fw01.bin) | 143,056 bytes | `0x10000` | `03AEC99F9F17BCDC5EFA5B1ED42F55D1D4F4C948CEB98C9B40C34627D51FABAC` |

## Phase 3 — Controlled flash and runtime

The operation used esptool `write-flash` with only the three listed address ranges. Each region was erased only as required for those writes and read-back hash verification passed. No `erase-flash` or full-chip erase command was issued. Complete esptool output is preserved in [`FW-01E-flash.log`](FW-01E-flash.log).

The physical EN/RESET button was pressed after the final serial capture was armed. At 115200 baud, the raw serial capture observed the ESP32 ROM boot, ESP-IDF v6.1 bootloader, partition loading, `main_task` startup, `app_main()` entry, and the HomeGrid diagnostic startup message. The application logged ESP-IDF `v6.1`, chip revision 300/v3.0, two cores, chip feature flags, Wi-Fi/BLE capabilities, and initial heap of 303,328 bytes.

The final capture recorded the 600-second app-start runtime window. It contains 60 post-boot heartbeats from 299 ms to 590,309 ms uptime, spaced 10,000–10,010 ms apart. Free and minimum free heap remained 303,328 bytes. No further boot sequence, unexpected reset, watchdog, exception, panic, abort, or assertion appeared during the timed interval.

Evidence files:

- Raw boot/runtime bytes: [`FW-01E-serial.log`](FW-01E-serial.log)
- UTC/monotonic timestamps and byte offsets: [`FW-01E-serial-timestamps.csv`](FW-01E-serial-timestamps.csv)
- Separate reviewed analysis: [`FW-01E-analysis.md`](FW-01E-analysis.md)
- Capture helper used: [`FW-01E-capture.py`](FW-01E-capture.py)
- Earlier partial attempts are retained with `attempt1`, `attempt2`, and `attempt3` suffixes and are not used for the ten-minute acceptance result.

## Exceptions and scope

- The USB bridge found is CH9102 rather than the previously expected CP2102. Chip identification on COM8 resolved board-port association; exact board model and module suffix are still pending physical markings.
- The image header's 2 MB flash-size field differs from detected 4 MB physical capacity. The 2 MB partition layout fits and the application booted, but the boot warning is retained and the upper 2 MB remains outside configured addressability.
- No relay, fan-control, mains circuit, GPIO output, Wi-Fi connection, or BLE operation was used. No source, architecture, or HomeGrid contract was modified.

FW-01E physical verification evidence is ready for Firmware Department review. FW-01 overall remains open for review and outstanding exact carrier/module markings; this report does not authorize future flash operations.
