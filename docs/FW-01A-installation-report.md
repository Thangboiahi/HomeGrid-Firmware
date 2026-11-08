# FW-01A Installation and Verification Report

**Updated:** 2026-11-08 (Asia/Calcutta, UTC+05:30)  
**Status:** **Installed and verified with notes.** ESP-IDF v6.1, its ESP32 toolchain, and dependencies are installed in the authorized per-user root.  
**Scope:** No target-selection command, firmware build, or flash was run. No USB serial driver or Windows settings/PATH were changed.

## Requested environment

Pinned baseline: ESP-IDF v6.1 on Windows 11 x64, installed using Espressif Installation Manager (EIM).

Official references consulted:

- [Espressif ESP-IDF v6.1 Windows installation guide](https://docs.espressif.com/projects/esp-idf/en/v6.1/esp32/get-started/windows-setup.html)
- [Espressif ESP-IDF v6.1 release](https://github.com/espressif/esp-idf/releases/tag/v6.1)
- [Espressif EIM CLI release v0.20.1](https://github.com/espressif/idf-im-ui/releases/tag/v0.20.1)

Espressif documents EIM for Windows and the command `eim install -i v6.1` to pin the framework version. EIM v0.20.1 was installed from its checksum-verified official release asset during the retry described below.

## Initial installation attempts (before retry)

1. `winget install Espressif.EIM-CLI` was attempted. WinGet selected its Microsoft Store source and prompted for Microsoft Store terms and disclosure of the machine's two-letter region. That prompt was not accepted; the command ended with `0x8a150042 : Error reading input in prompt`.
2. `winget install --source winget Espressif.EIM-CLI` was attempted to avoid the Store source. WinGet resolved `Espressif.EIM-CLI` v0.20.1 and reported a `Git.Git` package dependency. It began downloading Git for Windows v2.56.0.4, then failed with `0x801901f8 : Gateway timeout (504)`. The EIM package was not installed.
3. The official EIM v0.20.1 portable CLI asset was requested from Espressif's GitHub release URL for temporary storage, with a planned SHA-256 comparison against the WinGet manifest value `ed5130ed1d88f4f567c08847f76090b27f528cf3ec580cfcaf5888a63c074d2b`. `Invoke-WebRequest` failed with `504 Gateway Time-out` before the file was downloaded; therefore, the checksum was not verified and the executable was not run.

## Authorized retry — 2026-11-08

### Network checks

Read-only HTTPS HEAD checks were run against official Espressif/GitHub endpoints. Each returned HTTP 200:

- Espressif ESP-IDF v6.1 Windows setup guide.
- Espressif EIM CLI v0.20.1 release asset (23,138,104 bytes).
- Espressif ESP-IDF v6.1 release archive endpoint.

During EIM's mirror checks, it logged GitHub `504`, timeouts for `git.espressif.com.cn` and `dl.espressif.cn`, `403` for the Aliyun PyPI mirror, and a timeout for the USTC PyPI mirror. EIM selected `https://dl.espressif.com/github_assets` for tool downloads and `https://pypi.org/simple` for Python packages.

### Actions and partial results

1. Downloaded the official [EIM CLI v0.20.1 release](https://github.com/espressif/idf-im-ui/releases/tag/v0.20.1) to the temporary folder and verified SHA-256 `ed5130ed1d88f4f567c08847f76090b27f528cf3ec580cfcaf5888a63c074d2b` against the WinGet manifest value.
2. Installed the portable EIM CLI for the current user at `C:\Users\dell\AppData\Local\Programs\Espressif\EIM\eim.exe`. `eim --version` returned `eim 0.20.1`.
3. Started the exact-version installation with:

   ```powershell
   & 'C:\Users\dell\AppData\Local\Programs\Espressif\EIM\eim.exe' --do-not-track true install --path 'C:\Users\dell\esp\FW-01A' --idf-versions v6.1 --non-interactive true
   ```

   EIM found its prerequisites satisfied and used the pre-existing Python 3.14.3 and Codex-provided Git 2.56.0.windows.4. It cloned Espressif's `https://github.com/espressif/esp-idf.git` at tag `v6.1`, commit `fff9895c82d744c7237be8847347bdd1b07c6643`, and completed submodule update.
4. EIM then reported: `Downloading tools to: "C:\\Espressif\\dist"` and began downloading `xtensa-esp-elf`. This was unexpected because the command specified a user-level base directory. The process was interrupted at that point. Read-only inspection found `C:\Espressif\dist\xtensa-esp-elf-15.2.0_20251204-x86_64-w64-mingw32.zip` (63,573,663 bytes). EIM is no longer running. No file was deleted.

The original v6.1 source checkout existed at `C:\Users\dell\esp\FW-01A\v6.1\esp-idf`. The 63.6 MB archive in `C:\Espressif\dist` was inspected read-only: its actual size (63,573,663 bytes) did not match the ESP-IDF v6.1 manifest size (408,348,988 bytes); its SHA-256 also did not match manifest value `efa1e337b5f64239674bf755b767969cbba7f4e66fe7c6ab8d19b650f56012a9`, and archive listing reported truncation. It was not deleted, moved, or used.

## Department-authorized recovery — 2026-11-08

### EIM configuration and safe reuse

Inspected EIM v0.20.1 CLI help and its official v0.20.1 configuration, installation, and command documentation. The supported TOML fields used for this recovery include `path`, `idf_path`, `esp_idf_json_path`, `tool_download_folder_name`, `tool_install_folder_name`, `activation_script_path_override`, `python_env_folder_name`, `target`, `idf_versions`, `tools_json_file`, and download/source mirrors. EIM's reuse behavior requires `path` to point directly at the existing valid ESP-IDF Git checkout. The recovery configuration is `C:\Users\dell\.espressif\eim-fw01a-recovery.toml`; it pins `idf_versions = ["v6.1"]`, `target = ["esp32"]`, and directs IDF, metadata, downloads, installed tools, activation support, and Python environment under `C:\Users\dell\.espressif`.

An initial recovery attempt using the checkout's parent path caused EIM to begin operating on the destination checkout, which then lacked Git metadata and files. The original checkout remained intact. It was restored to the user-level destination with a copy-only Robocopy operation (no mirror/delete options). Before resuming installation, the restored destination was verified to have commit `fff9895c82d744c7237be8847347bdd1b07c6643`, exact tag `v6.1`, a clean worktree, and all 24 submodules. No source changes were made.

### Installation result

EIM v0.20.1 was invoked with the recovery TOML and installed ESP-IDF v6.1 and the original ESP32/Xtensa tools. EIM logged successful Python environment setup, tool installation, component synchronization, and `Successfully installed IDF`. During registry sync it logged one transient `ConnectionAbortedError(10053)`; it then reported registry synchronization and required component downloads successful. The tool set includes Xtensa GCC 15.2.0, CMake 4.0.3, Ninja 1.12.1, esp-clang 21.1.3_20260408, Xtensa GDB 17.1_20260402, OpenOCD ESP32 v0.12.0-esp32-20260703, and ESP32 ULP ELF tools. No RISC-V target toolchain was selected.

EIM metadata shows two entries associated with the same v6.1 destination: a stale `v6.1` entry marked `in_progress` from the earlier failed attempt and a `fff9895` entry marked `finished` from the successful run. Both resolve to the same ESP-IDF path. Neither entry was removed because removing the stale record might remove the shared installation. The verified environment was started by EIM's `run` command using the finished `fff9895` entry.

### Verification results

The first direct `export.ps1` attempt was not a valid EIM activation path: the stock script expected a legacy Python environment at `C:\Users\dell\.espressif\python_env\idf6.1_py3.14_env`, while EIM created its environment at `C:\Users\dell\.espressif\tools\python\fff9895\venv`. Verification was therefore run in a fresh EIM-activated PowerShell process. That shell emitted a Python-path comparison warning (the same environment was displayed with different slash normalization) and a Windows Unicode-console advisory. The following checks were then repeated directly against installed binaries where appropriate, including an unrestricted read-only process for the compiler and Git checkout. The compiler and checkout checks passed.

## Verification results

| Check | Result |
|---|---|
| Fresh EIM PowerShell environment | Started using EIM `run` for finished entry `fff9895`; `IDF_PATH=C:/Users/dell/.espressif/v6.1/esp-idf`, `IDF_TOOLS_PATH=C:/Users/dell/.espressif/tools`, Python env `C:/Users/dell/.espressif/tools/python/fff9895/venv`. |
| `idf.py --version` | **ESP-IDF v6.1.0**. EIM emitted a Python-path normalization warning despite invoking the venv interpreter; version command succeeded. |
| ESP-IDF Git revision | **Commit `fff9895c82d744c7237be8847347bdd1b07c6643`, exact tag `v6.1`**, clean worktree. |
| CMake | **4.0.3**, available in fresh EIM environment. |
| Ninja | **1.12.1**, available in fresh EIM environment. |
| Python | **3.14.3** in EIM venv at `C:\Users\dell\.espressif\tools\python\fff9895\venv`; `python -m pip check` reported `No broken requirements found.` |
| Xtensa compiler | **xtensa-esp-elf-gcc 15.2.0**; direct compiler invocation passed. Initial sandboxed invocation failed with Windows `Illegal System DLL Relocation`; retry from an unrestricted read-only process succeeded. |
| Host tests | **6 passed, 0 failed**; full captured output below. |

### Host scaffold tests

Command run from the repository root:

```powershell
& 'C:\Users\dell\AppData\Local\Python\bin\python.exe' -m unittest discover -s tests -v
```

Result: **6 passed, 0 failed** (exit code 0), rerun after installation. These are host-only scaffold checks and do not verify hardware operation.

```text
test_codex_guardrails_and_board_worksheet_exist ... ok
test_component_registers_c_source ... ok
test_heartbeat_uses_freertos_delay ... ok
test_no_hardware_or_network_initialization_in_fw01 ... ok
test_root_cmake_uses_esp_idf_project ... ok
test_source_reports_chip_and_runtime_metadata ... ok
Ran 6 tests in 0.005s
OK
```

## Final paths and versions

- EIM 0.20.1: `C:\Users\dell\AppData\Local\Programs\Espressif\EIM\eim.exe` (official release asset; SHA-256 verified as `ed5130ed1d88f4f567c08847f76090b27f528cf3ec580cfcaf5888a63c074d2b`).
- EIM configuration: `C:\Users\dell\.espressif\eim-fw01a-recovery.toml`.
- ESP-IDF: `C:\Users\dell\.espressif\v6.1\esp-idf` — v6.1 / commit `fff9895c82d744c7237be8847347bdd1b07c6643`.
- Tool downloads: `C:\Users\dell\.espressif\dist`.
- Installed tools: `C:\Users\dell\.espressif\tools`.
- Python virtual environment: `C:\Users\dell\.espressif\tools\python\fff9895\venv` — Python 3.14.3.
- CMake 4.0.3, Ninja 1.12.1, Xtensa GCC 15.2.0.
- Original incomplete archive remains untouched at `C:\Espressif\dist\xtensa-esp-elf-15.2.0_20251204-x86_64-w64-mingw32.zip`; it failed integrity/compatibility checks and was not reused.

Installation and required host verification are complete. No target selection, firmware build, or flash was performed. No firmware source files were changed.
