# FW-01 Host and Repository Audit

**Audit date/time:** 2026-11-08 10:18 Asia/Calcutta (UTC+05:30)  
**Scope:** Read-only host and repository audit. No tools were installed, no system settings were changed, and no ESP32 target/build/flash operation was performed.

## Host inventory

| Item | Finding |
|---|---|
| OS | Microsoft Windows NT 10.0.26300.9818 (Windows 11 family); .NET OS version reported `Microsoft Windows NT 10.0.26300.0` |
| Architecture | x64 OS and x64 PowerShell process |
| Shell | PowerShell Core 7.6.9 (`pwsh.exe`) |
| Workspace volume | M: NTFS not established; PowerShell drive reported 12,185,051,136 bytes used and 247,190,708,224 bytes free (about 230.1 GiB free) |
| Python | Python 3.14.3 at `C:\Users\dell\AppData\Local\Python\bin\python.exe`; WindowsApps `py.exe` alias was present but could not start. Direct Python executable worked. |
| Git | Git for Windows 2.56.0.windows.4, available through the Codex runtime path. `git status --short` returned “fatal: not a git repository”; this workspace has no discoverable `.git` repository metadata. |
| CMake / Ninja | Not found on PATH |
| Espressif Installation Manager (EIM) | `eim` not found on PATH; common `C:\Espressif` install directory absent |
| ESP-IDF / `idf.py` | Not found on PATH; `IDF`/`ESP` environment variables absent; common Espressif install directories checked were absent |
| ESP32 compiler tools | `xtensa-esp32-elf-gcc`, `xtensa-esp32s2-elf-gcc`, `xtensa-esp32s3-elf-gcc`, and `riscv32-esp-elf-gcc` not found on PATH |
| Disk-space caveat | `Get-CimInstance Win32_LogicalDisk` was denied by the host environment. Free space above is from `Get-PSDrive M` and not a filesystem/CIM report. |

The initial `py --version` and `py -0p` checks failed because the WindowsApps launcher could not start in this environment. The test run used the direct Python path listed above.

## USB and serial enumeration

Read-only `pnputil.exe /enum-devices /connected /class Ports` reported:

- COM3: Intel(R) Active Management Technology - SOL (PCI-connected serial-over-LAN interface).
- COM4, COM5, COM6: Dell DW5821e-eSIM Snapdragon X20 LTE modem interfaces. The modem USB device reports VID:PID `413C:81E0`.

The connected USB enumeration showed the Dell LTE modem, integrated webcam, Intel Bluetooth device, and USB root hubs. No USB serial adapter or device identifiable as an Espressif/ESP32 board appeared. Therefore, **there is no visible evidence that the HomeGrid development board is connected**. The listed COM ports must not be treated as the board port. `Get-PnpDevice` and CIM serial queries were denied with “Access to a CIM resource was not available”; `pnputil` enumeration did work. No serial numbers are recorded here.

## Repository structure

Files observed with `rg --files`:

```text
AGENTS.md
CHANGELOG.md
CODEX_FIRST_TASK.md
CMakeLists.txt
README.md
MILESTONE_REPORT_FW-01.md
docs/board-identification.md
main/CMakeLists.txt
main/app_main.c
tests/test_fw01_scaffold.py
```

`CMakeLists.txt` and `main/CMakeLists.txt` are present. Test bytecode cache files were also present under `tests/__pycache__` before this audit. No existing build directory was listed.

## Host tests

Command run from the repository root:

```powershell
& 'C:\Users\dell\AppData\Local\Python\bin\python.exe' -m unittest discover -s tests -v
```

Result: **6 tests passed, 0 failed** (process exit code 0).

```text
test_codex_guardrails_and_board_worksheet_exist ... ok
test_component_registers_c_source ... ok
test_heartbeat_uses_freertos_delay ... ok
test_no_hardware_or_network_initialization_in_fw01 ... ok
test_root_cmake_uses_esp_idf_project ... ok
test_source_reports_chip_and_runtime_metadata ... ok
Ran 6 tests in 0.004s
OK
```

These are host scaffold checks only. They do not compile firmware or verify hardware.

## ESP-IDF v6.1 status and recommended next setup

ESP-IDF v6.1, EIM, `idf.py`, CMake/Ninja, and the ESP32 toolchains are **not detected as installed/available** in this shell. Espressif documents EIM as the recommended Windows installation route for ESP-IDF v6.0 and newer and recommends v6.1 installation with `eim install -i v6.1`.

Official references:

- [ESP-IDF v6.1 Windows setup](https://docs.espressif.com/projects/esp-idf/en/v6.1/esp32/get-started/windows-setup.html)
- [ESP-IDF v6.1 release installation instructions](https://github.com/espressif/esp-idf/releases/tag/v6.1)

Proposed commands, **not run**:

```powershell
# Install Espressif's EIM command-line installer through Microsoft's WinGet.
winget install Espressif.EIM-CLI

# Install ESP-IDF v6.1 and its matching tools with EIM.
eim install -i v6.1

# In the EIM-created ESP-IDF v6.1 environment, capture versions and paths.
idf.py --version
where.exe idf.py cmake ninja git python
```

The `winget` command downloads and installs EIM. The `eim` command downloads ESP-IDF v6.1 and the matching build tools/toolchains. Installation location and any prerequisite prompts should be reviewed in EIM. No driver install is recommended at this audit stage. After setup, confirm the installed version is exactly v6.1 and capture the activated environment paths. The board must first be physically identified and recorded before a target is selected or any build/flash commands are considered.

Proceeding with these installation commands requires the user's explicit approval because they install software, download toolchains, and modify the machine's user-level development environment. This audit itself did not request or use elevated privileges.

## Audit command limitations

The following read-only checks were attempted but blocked by the environment's access policy:

- `Get-CimInstance Win32_OperatingSystem`, `Win32_ComputerSystem`, `Win32_LogicalDisk`, and `Win32_SerialPort`: CIM access unavailable.
- `Get-PnpDevice -PresentOnly`: CIM access unavailable. `pnputil.exe` provided the USB/serial enumeration above.
- WindowsApps `py.exe` alias: could not start; direct Python 3.14.3 was used instead.
- `git status --short`: no `.git` repository metadata was present.

No hardware identity can be established from this host audit. The required front/back/module markings and USB evidence for the bare board remain outstanding.
