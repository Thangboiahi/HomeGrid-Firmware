# HomeGrid Firmware — FW-01 diagnostic baseline

**Version:** 0.1.1 (packaging and Codex handoff correction)  
**Status:** In progress; actual host, board identity, ESP-IDF build and physical flash are not yet verified.

This repository contains a board-neutral ESP-IDF C application. It reports ESP-IDF/SoC/runtime metadata and emits a 10-second FreeRTOS heartbeat. It does not configure GPIO, operate relays, initialise Wi-Fi/BLE/MQTT, or interact with mains-connected equipment.

## 1. Development rules

- Production ESP32 firmware: C + ESP-IDF + FreeRTOS.
- Python: host-side tooling and automated tests only.
- Framework baseline: ESP-IDF v6.1, pinned for FW-01.
- Do not select the ESP-IDF target until the actual SoC/module is identified and documented.
- Do not connect mains, switches, fans, relays or unknown circuits during FW-01.

Read `AGENTS.md` before asking Codex to work in this repository. Start with `CODEX_FIRST_TASK.md`.

## 2. Codex Desktop workflow

1. Extract this repository to a stable local folder, for example `HomeGrid/Firmware/FW-01`.
2. Open the ChatGPT desktop app and select **Codex**. Sign in with the ChatGPT account that has Go access.
3. Open the extracted repository folder as the local project/workspace and grant access only to that folder.
4. Start a new Codex task. Ask it to read `AGENTS.md` and execute the read-only audit in `CODEX_FIRST_TASK.md`.
5. Review proposed commands. Do not approve software installation, driver installation, system-wide changes or flashing until the purpose and effects are clear.

Codex usage is subject to plan limits. Local Codex work uses the selected machine's development tools and permissions; Codex Cloud is not required for this workflow.

## 3. ESP-IDF installation

First complete the read-only host audit and identify the operating system. Then use Espressif's official installation instructions for that OS:

- Windows: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/windows-setup.html
- macOS: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/macos-setup.html
- Linux: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/linux-setup.html
- ESP-IDF release: https://github.com/espressif/esp-idf/releases/tag/v6.1

Install the exact v6.1 release and its matching tools using Espressif's supported installer process. Do not combine tools from different ESP-IDF installations in one shell. Record `idf.py --version` and tool paths in `docs/host-audit.md`.

## 4. Board identification gate

Complete `docs/board-identification.md` using physical markings, host USB enumeration and official manufacturer documentation. Do not guess the chip from the board's appearance. Confirm exact SoC, module, board model/revision, flash/PSRAM and USB interface.

Only after identity is confirmed may the verified ESP-IDF target be selected:

```bash
idf.py --version
idf.py set-target <verified-idf-target>
idf.py build
```

The placeholder must be replaced with the target supported by the exact chip. Do not run this command with a guessed value.

## 5. Flash and monitor gate

Only after the correct target builds, and only with the bare development board connected through its documented USB/power input, identify the correct serial port and obtain user approval before flashing:

```bash
idf.py -p <verified-serial-port> flash monitor
```

Record the entire build output, flash output and boot log. Confirm that the 10-second heartbeat continues for at least two minutes without reset or crash.

## 6. Host scaffold tests

Run from the repository root:

```bash
# Windows
py -m unittest discover -s tests -v

# macOS/Linux
python3 -m unittest discover -s tests -v
```

These are static host-side guardrail tests only. They are not an ESP-IDF compile and do not prove hardware operation.

## 7. FW-01 exit criteria

1. Reproducible host setup recorded; pinned `idf.py --version` verified.
2. Exact board, SoC, module, USB interface and relevant memory/radio capabilities documented with evidence.
3. Project builds for the verified target without errors.
4. Firmware flashes to the bare board and expected boot output is captured.
5. Two minutes of 10-second heartbeat operation observed without reset/crash.
6. Source, host tests, build/flash logs, board evidence, change log and milestone report archived.

## 8. Safety and boundaries

- No AC mains or relay/fan switching in FW-01.
- No GPIO pinout or active-high/active-low assumptions.
- No custom PCB work.
- No production credentials or secrets in logs/source control.
- No HomeGrid shared-contract changes without HQ review and approval.
