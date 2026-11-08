# HomeGrid Development Board Identification

**Status:** ESP32 device positively identified and FW-01E hardware run accepted; exact carrier-board and module markings remain pending.  
**Updated:** 2026-11-08

## Department acceptance

The HomeGrid Firmware Department accepted the following development-hardware description and target in its FW-01B authorization:

| Field | Department-accepted value | Local evidence status |
|---|---|---|
| Board | Generic ESP32 development board | Exact maker, model, and PCB revision not supplied |
| SoC | ESP32-D0WD-V3, revision 3.0 | Confirmed on COM8 with Espressif esptool `chip-id`; raw serial boot agrees with revision v3.0 |
| Module family | ESP-WROOM-32 | Department-identified; exact shield suffix/revision marking not supplied |
| ESP-IDF target | `esp32` | Department-approved; selected for the FW-01D build and successfully built with ESP-IDF v6.1 |
| USB-UART | CH9102 | Windows PnP: `USB-Enhanced-SERIAL CH9102 (COM8)`, manufacturer `wch.cn`, hardware ID prefix `USB\\VID_1A86&PID_55D4`; this supersedes the earlier CP2102 expectation |
| Board USB connector | Micro-USB | Board marking/photo and input-power details still need inspection |
| Radio capabilities | 2.4 GHz Wi-Fi and BLE | Consistent with ESP32/WROOM family; not exercised or locally verified |
| Flash capacity | 4 MB detected | Espressif esptool `flash-id`; bootloader independently reports 4096 kB |
| PSRAM | Unknown | Must be checked; no presence/capacity assumed |

Department acceptance supplied the family-level identity. FW-01E then positively associated COM8 with the SoC using chip identification and captured its flash capacity and revision. No photographs or legible shield/PCB markings were provided, so the carrier maker/model/PCB revision and exact module order-code suffix remain unknown. The USB bridge observed in hardware differs from the earlier CP2102 description; actual evidence is CH9102.

## Evidence and references

- Department source: HomeGrid Firmware Department authorization in the FW-01C preparation request, dated 2026-11-08.
- SoC/GPIO reference: [Espressif ESP32 datasheet](https://documentation.espressif.com/esp32_datasheet_en.html).
- Module reference: [Espressif ESP-WROOM-32 datasheet](https://documentation.espressif.com/esp32-wroom-32_datasheet_en.html).
- Pin usage reference for the pinned framework: [ESP-IDF v6.1 ESP32 GPIO guide](https://docs.espressif.com/projects/esp-idf/en/v6.1/esp32/api-reference/peripherals/gpio.html).
- Remaining local evidence: front/back board photographs, readable module shield marking/suffix, PCB silkscreen/revision, and manufacturer page/schematic for the exact carrier board. USB enumeration and SoC/flash identification are recorded in the FW-01E report.

## Outstanding identity checks

Before hardware verification is considered complete:

1. Photograph both sides of the bare development board and a clear close-up of the module shield marking.
2. Record the carrier-board maker, exact model, PCB revision, and any board-specific schematic or product documentation. If no vendor is identifiable, retain “generic ESP32 development board.”
3. With the board connected only by its documented USB input, capture USB VID:PID and the COM-port description. Verify the bridge model from the package marking or authoritative board documentation; do not treat an unrelated COM port as the board.
4. Read the exact module suffix and look up its matching Espressif datasheet revision. Do not infer the flash capacity from `ESP-WROOM-32` family name alone.
5. Once the serial device is confirmed, use non-destructive identification to record SoC model/revision and detected flash capacity. Do not record the MAC address, eFuse secrets, or other unique identifiers in this report.
6. Verify PSRAM presence and capacity from exact board/module documentation and a later supported runtime check; unknown remains unknown until evidence exists.

## Target gate

The Department approved target `esp32` for the Original ESP32. FW-01D selected that target and built successfully with pinned ESP-IDF v6.1; FW-01E booted the resulting diagnostic firmware on the identified ESP32-D0WD-V3 revision 3.0. The FW-01C preparation itself did not select a target or flash hardware.

## Change history

- 2026-11-08: Replaced the unconfirmed worksheet state with the department-accepted ESP32 / ESP-WROOM-32 / CP2102 identity. Clearly marked local evidence gaps and recorded approved target `esp32` without running target-selection commands.
- 2026-11-08 (FW-01E): Recorded esptool-confirmed ESP32-D0WD-V3 revision 3.0, 4 MB physical flash, and observed CH9102 COM8 / VID_1A86:PID_55D4. Corrected the earlier CP2102 expectation and retained the exact carrier PCB revision, module suffix, and PSRAM as unknown pending markings/documentation.
- Initial FW-01 worksheet: no identity had been assumed.
