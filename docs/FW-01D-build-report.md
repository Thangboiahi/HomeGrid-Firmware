# FW-01D — First ESP-IDF Build Report

**Date:** 2026-11-08 (Asia/Calcutta)  
**Project:** HomeGrid Diagnostic Firmware v0.1  
**Result:** **Build passed** for ESP-IDF target `esp32`.  
**Hardware action:** No board was flashed or connected; this is host compilation evidence only.

## Environment and commands

The build ran in a fresh EIM-activated PowerShell environment from the project root. `IDF_PATH` was `C:/Users/dell/.espressif/v6.1/esp-idf` and the selected EIM entry was `fff9895`.

| Tool | Version |
|---|---|
| ESP-IDF | `v6.1` (`idf.py --version`) |
| ESP-IDF Git revision | Tag `v6.1`, commit `fff9895c82d744c7237be8847347bdd1b07c6643` |
| Xtensa GCC | `15.2.0` (`esp-15.2.0_20251204`) |
| CMake | `4.0.3` |
| Ninja | `1.12.1` |
| Python | `3.14.3`, EIM venv `C:\Users\dell\.espressif\tools\python\fff9895\venv` |

Commands executed:

```powershell
idf.py set-target esp32
idf.py build
```

The target-selection command reported `Set Target to: esp32, new sdkconfig will be created.` The generated [`sdkconfig`](../sdkconfig) contains `CONFIG_IDF_TARGET="esp32"`. No prior `build` directory or `sdkconfig` existed before target selection. For a complete native build transcript, generated Ninja outputs were cleaned while preserving source and configuration, then `idf.py build` was repeated. The clean rebuild also exited 0. Full captured output, including tool versions, warnings, compile steps, link, size check, and exit status, is in [`FW-01D-build.log`](FW-01D-build.log).

## Result and generated artifacts

The application source `main/app_main.c` compiled at step `[972/1035]`. ESP-IDF linked `homegrid_fw01.elf`, generated the ESP32 image, completed the partition-size check, and reported `Project build complete`. Final build exit code: `0`. The application image is `0x22ed0` (143,056) bytes; the configured smallest app partition is `0x100000` bytes, leaving 86% free.

| Artifact | Size | SHA-256 |
|---|---:|---|
| `build/bootloader/bootloader.bin` | 26,176 bytes | `63BC36077F3FDD491E084684C660C655608515460DF585FAC7C485F0058B4D75` |
| `build/partition_table/partition-table.bin` | 3,072 bytes | `7F00B6C042A89B15B0CAC534F82ED988CAF29278FF5700B0C511EB1B5BB7C820` |
| `build/homegrid_fw01.bin` | 143,056 bytes | `03AEC99F9F17BCDC5EFA5B1ED42F55D1D4F4C948CEB98C9B40C34627D51FABAC` |
| `build/homegrid_fw01.elf` | 2,837,260 bytes | `4A259A2E5D932843BDB1353B57EC161DE03742D226D720E843D50A8439A78D45` |

The build directory is [`../build`](../build). These generated images are build products only; none was written to hardware.

Espressif `esptool v5.6.0 image-info` successfully parsed `build/homegrid_fw01.bin` as an ESP32 image: 6 segments, image size 143,056 bytes, valid image checksum and validation hash. The image metadata carries the current build configuration's 2 MB flash-size field, 40 MHz, DIO mode. This is a compile-time image setting only; it does **not** establish the physical module's flash capacity, which remains unverified.

## Warnings and notices

No project C compilation error occurred, and the build completed despite the following reported notices:

- EIM's PowerShell activation profile printed its Unicode-console advisory and an `idf.py` Python-venv path warning. The venv interpreter resolved to the installed EIM Python 3.14.3 location; slash normalization differed in the comparison. No persistent Windows language, security, or PATH setting was changed.
- ESP-IDF Kconfig emitted 22 `NOTE` entries across configuration passes. These include existing v6.1 defaults of numeric `0` for several Boolean BT/FATFS options (treated as `n`) and deprecated hexadecimal literal formatting notices. Full messages are preserved in the build log.
- Initial project configuration emitted CMake component-validation warnings about `wpa_supplicant` using private `esp_wifi` roaming-app include directories. These are in ESP-IDF component definitions and did not stop configuration/build.
- A non-fatal Git version lookup notice appeared because this workspace did not expose Git metadata to the CMake project-version probe. The build proceeded with application version `1`.

The application itself required no source changes. It remains a C/FreeRTOS diagnostic and does not configure GPIO, initialize Wi-Fi/BLE, operate a relay, or alter HomeGrid communication contracts.

## Host tests

After compilation, ran:

```powershell
python -m unittest discover -s tests -v
```

Result: **6 passed, 0 failed**. Tests are host-side scaffold checks, not hardware tests.

## Review boundary

FW-01D compile acceptance is presented for Firmware Department review. No `idf.py flash`, erase, serial monitor, or hardware connection command was run. Physical board identity details, flash/PSRAM confirmation, boot log, and the two-minute heartbeat remain outstanding. FW-01 remains open until the Department reviews this evidence and later hardware criteria are completed.
