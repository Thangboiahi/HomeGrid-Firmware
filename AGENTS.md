# HomeGrid Firmware — Codex working rules

## Role and project boundary
You are implementing the HomeGrid ESP32 firmware repository. Work only within this repository and the approved HomeGrid architecture. The first milestone is FW-01: establish a reproducible ESP-IDF environment, identify the actual board, and build/flash a diagnostic application.

## Non-negotiable rules
1. Production firmware is C using ESP-IDF and FreeRTOS. Python is for host-side scripts/tests only. Do not introduce MicroPython.
2. Do not assume the ESP32 variant, module, dev-board model/revision, flash/PSRAM size, GPIO mapping, USB interface, or relay polarity. Obtain physical markings, USB enumeration and/or official board documentation first.
3. Do not run `idf.py set-target`, build for a guessed target, or flash a board until the exact SoC/target and board have been established and recorded in `docs/board-identification.md`.
4. FW-01 is a bare-board diagnostic only. Do not connect to or operate AC mains, a wall switch, fan, relay module, or unknown external circuit. Do not write relay/switch control code in this milestone.
5. Do not initialise Wi-Fi, BLE, MQTT, provisioning, OTA, GPIO outputs or peripherals in FW-01. Only report SoC capability flags; do not turn radios on.
6. Never print, commit or request production credentials, private keys, Wi-Fi passwords, device secrets, tokens, or unique identifiers unnecessarily.
7. Do not change HomeGrid shared communication contracts, topic names, payload schemas, identity rules, security policy or OTA contracts. Raise a proposal for HQ review instead.
8. Do not install software, change system-wide settings, modify PATH, install USB drivers, erase flash or run privileged/destructive commands without the user's explicit approval. Explain the command and its effect before requesting approval.
9. Use the pinned ESP-IDF v6.1 baseline for FW-01. Do not silently upgrade, downgrade or mix toolchains. If v6.1 is unavailable for the identified SoC, stop and report the incompatibility for an engineering decision.
10. Keep changes small, reviewable and documented. Update `CHANGELOG.md` and `MILESTONE_REPORT_FW-01.md` with real results only. Never claim a build, flash or hardware test passed without captured command output/evidence.

## Working sequence
1. Inspect repository contents and run `python -m unittest discover -s tests -v` (or the platform equivalent) without changing the hardware target.
2. Audit the host OS, architecture, shell, free disk space, Python, Git, CMake, Ninja, EIM/ESP-IDF availability and installed compiler tools. Report findings before installation.
3. Ask the user to provide front/back/module photos and make/model markings; capture USB VID:PID and serial-port evidence with the bare board connected. Do not infer identity from appearance alone if markings are unclear.
4. Record exact chip, module and board details in `docs/board-identification.md`, with source links/evidence and unresolved unknowns.
5. Only after board identity is confirmed, agree on the correct ESP-IDF target and perform the v6.1 environment setup/build steps with user approval.
6. Flash only the bare development board over its documented USB/power input, after the user approves. Capture the full build log, boot log and at least two minutes of heartbeat output.
7. Review the diff, tests and evidence. Leave FW-01 open if any acceptance criterion is unverified.

## Deliverables
- Source code and build configuration that compile with the pinned ESP-IDF release.
- Reproduction commands and tool versions.
- Test output and actual build/flash/serial evidence, clearly separating host-only checks from hardware verification.
- Updated change log and milestone report.
- A concise handover summary for the HomeGrid Firmware Department Lead. No HQ contract changes without approval.
