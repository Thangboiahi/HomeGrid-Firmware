# FW-01E Serial Runtime Analysis

**Date:** 2026-11-08 (Asia/Calcutta)  
**Result:** Diagnostic boot and ten-minute runtime evidence captured on physical ESP32 COM8.

## Device and capture

- PnP device: `USB-Enhanced-SERIAL CH9102 (COM8)`, manufacturer `wch.cn`, hardware ID prefix `USB\VID_1A86&PID_55D4`.
- The same session identified `ESP32-D0WD-V3`, revision 3.0; flash identification detected 4 MB. The serial device is distinguished from Intel AMT SOL COM3 and Dell LTE modem/NMEA COM4–COM6.
- Capture: COM8 at 115200 baud; initiated before the user pressed the physical EN/RESET button. ESP-IDF application-start marker began the 600-second runtime timer.
- Raw bytes: [`FW-01E-serial.log`](FW-01E-serial.log), 9,208 bytes. Read timestamps and byte offsets: [`FW-01E-serial-timestamps.csv`](FW-01E-serial-timestamps.csv), 18,685 bytes. Timestamp records span 623.422 seconds, including capture setup and reset activity; timed application window was 600 seconds.

## Results from raw log

- One ROM boot sequence appears, with `rst:0x1 (POWERON_RESET)` and ESP-IDF v6.1 second-stage bootloader, followed by the application loaded from `0x10000`.
- The diagnostic app reports ESP-IDF `v6.1`, chip revision value `300` (v3.0), two CPU cores, feature flags `0x00000032`, Wi-Fi/BLE capability, and FreeRTOS `main_task` startup followed by `app_main()`.
- Initial free heap: 303,328 bytes. Every captured post-boot heartbeat reports free and minimum free heap as 303,328 bytes.
- There are 60 post-boot heartbeat lines from uptime 299 ms through 590,309 ms. Intervals are 10,000–10,010 ms, consistent with the 10-second FreeRTOS delay. No second boot banner or reset was observed in the 600-second timed runtime.
- No Guru Meditation, watchdog, brownout, abort, assertion, or panic text was found.
- The raw file begins with three heartbeat lines buffered from the pre-reset running instance, before the captured ROM reset banner. They are excluded from post-boot counts. The physical reset and subsequent boot sequence are present in the same raw capture.

## Flash configuration observation

The boot log states that detected flash size (4096 kB) exceeds the image header setting (2048 kB), and that ESP-IDF will use the image-header size. The configured 2 MB layout still fits the physical 4 MB device; the NVS, PHY, and 1 MB factory-app partitions booted successfully. This informational warning must remain recorded because the remaining device capacity is outside the active 2 MB image view.

The application was already built and flashed before this verification. No rebuild, reflash, erase, or firmware modification was performed during serial capture and analysis.

## Earlier capture attempts

The first capture stopped after roughly 70 seconds when the long-running shell session was interrupted. The next helper revisions also captured the boot, but failed to start their duration timer because the startup marker could arrive fragmented or within a large read. Those partial raw logs and timestamp sidecars were preserved as `FW-01E-serial-attempt{1,2,3}-partial.log` and corresponding timestamp CSVs. The final helper checks the marker in the combined read before retaining the rolling suffix; this final attempt completed the full timed window.
