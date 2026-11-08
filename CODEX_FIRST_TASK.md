# Codex task — FW-01 local environment audit (first session)

You are working in the HomeGrid Firmware repository. Read `AGENTS.md` and `README.md` first and obey all safety and scope restrictions.

## Task
Perform a **read-only host and repository audit**. Do not install or update anything yet.

1. Identify the actual host OS/version, architecture, active shell and available disk space.
2. Check whether Python, Git, CMake, Ninja, Espressif Installation Manager (EIM), ESP-IDF, `idf.py`, and ESP32 compiler toolchains are installed. Record exact versions and paths where available.
3. Enumerate currently connected USB/serial devices using read-only commands. Do not guess which device is the ESP32. State whether a board is connected and what evidence is visible.
4. Inspect the repository structure and verify that the ESP-IDF CMake files are present.
5. Run the host scaffold tests:
   - Windows: `py -m unittest discover -s tests -v` (or `python -m unittest ...` if `py` is unavailable).
   - macOS/Linux: `python3 -m unittest discover -s tests -v`.
6. Do not run `idf.py set-target`, `idf.py build`, `idf.py flash`, `esptool`, erase commands, GPIO tests, or any mains/relay tests during this first audit.
7. Do not change files except to add a clearly justified audit report at `docs/host-audit.md`; include the date/time and exact commands/results. Do not fabricate unavailable results.

## Return to the user
Provide:
- OS and hardware/software inventory;
- host test results;
- whether ESP-IDF v6.1 is installed;
- USB/serial findings and any ambiguity;
- the official next installation steps for this OS, with the exact commands you propose to run;
- what permissions/approvals are needed before installation.

Stop after the audit. Wait for the user's approval before installing tools or changing the machine.
