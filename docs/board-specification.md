# HomeGrid Board Specification — FW-01C Preparation

**Profile:** Generic ESP32 development board using ESP-WROOM-32  
**Status:** FW-01E verified diagnostic build/run; exact carrier-board details pending  
**Verified target:** `esp32`

## Accepted hardware profile

| Property | Accepted / current value | Qualification |
|---|---|---|
| SoC | ESP32-D0WD-V3, revision 3.0 | esptool chip identification and boot log on COM8 |
| Module | ESP-WROOM-32 family | Department-identified; exact suffix and revision still unread |
| Carrier | Generic ESP32 development board | Maker, model, PCB revision, and schematic unknown |
| USB bridge / connector | CH9102, VID_1A86:PID_55D4 / Micro-USB (department-reported) | Enumerated by Windows as CH9102 on COM8; this supersedes the earlier CP2102 expectation |
| Radio family capability | 2.4 GHz Wi-Fi; Bluetooth/BLE | Capability only; FW-01 does not initialize radios |
| SPI flash | 4 MB detected | Read-only esptool flash identification; image header is configured for 2 MB and boot log warns it will use that configured size |
| PSRAM | Unknown | Do not assume presence or capacity |
| Board power limits / protection | Unknown | Obtain exact board documentation before connecting anything beyond documented USB input |
| GPIO assignments | None | This profile assigns no input, output, relay, or switch pins |

## GPIO use and boot behavior

The Original ESP32 has a multiplexed GPIO matrix. Board header labels and chip GPIO numbers are not interchangeable without checking the exact carrier-board schematic and traces. Espressif's [ESP-IDF v6.1 GPIO reference](https://docs.espressif.com/projects/esp-idf/en/v6.1/esp32/api-reference/peripherals/gpio.html) and [ESP32 datasheet](https://documentation.espressif.com/esp32_datasheet_en.html) identify the following constraints:

| GPIOs | Constraint | FW-01C handling |
|---|---|---|
| 0, 2, 5, 12, 15 | Boot-strapping pins sampled during reset/boot. GPIO0 affects download/boot mode; GPIO12 (MTDI) affects the VDD_SDIO voltage strap; loading these can prevent normal boot or alter boot configuration. | Excluded from the preliminary candidate pool. Do not attach external loads before board-specific analysis. |
| 6–11 | SPI0/1 connections used for module flash access. | Reserved; do not use for application signals. |
| 16–17 | Espressif's ESP32 GPIO guide notes these are usually connected to in-module SPI flash/PSRAM. | Avoid unless exact module documentation and physical routing prove otherwise; excluded from candidates. |
| 1, 3 | UART0 TX/RX, normally used by the USB-UART bridge for programming and boot logs. | Avoid for switch signals unless a later reviewed design accepts loss/interference with logging/programming. |
| 12–15 | JTAG functions are available; GPIO12 and GPIO15 are also strapping pins. | Avoid in the preliminary candidate pool; preserve JTAG/boot options pending board review. |
| 34–39 | Input-only; no output driver and no internal pull-up/pull-down. | May be considered only for a later low-voltage input after external bias and board routing are checked. Never output candidates. |
| 25–27 | General-purpose capable, but also ADC2 pins; ADC2 analog use conflicts with Wi-Fi operation. | Candidate only for digital use if the later design needs no conflicting ADC2 behavior. |
| 32–33 | General-purpose capable and RTC-capable; can have alternate low-frequency crystal roles. | Candidate only after confirming the carrier board does not use them for another function. |

All ESP32 IO is a 3.3 V logic domain and must not be treated as 5 V tolerant. This is a chip-level constraint, not a statement about the carrier board's protection or power circuitry. Do not connect a relay, mains circuit, wall switch, fan, or unknown external circuit as part of FW-01.

## Preliminary candidate GPIOs for future low-voltage switch-control work

The following are a **candidate pool only**, not an assignment: **GPIO18, GPIO19, GPIO21, GPIO22, GPIO23, GPIO25, GPIO26, GPIO27, GPIO32, GPIO33**. These avoid the usual ESP32 strapping pins, flash pins, UART0 pins, and input-only group. Each still requires a board schematic/pad continuity review and checks for onboard LEDs, peripherals, and other carrier-board loading. GPIO25–27 are ADC2-capable; GPIO32–33 may have alternate crystal use.

GPIO34–39 may be evaluated separately if the future design needs an input only; they need suitable external bias because they lack internal pulls. Their input-only property excludes them from output or relay-driver candidates.

No candidate is declared electrically safe for an external circuit. Software pin names, board silkscreen labels, relay pin, polarity, active level, boot default, and driver circuit remain unassigned. Future switch/relay design needs a separate electrical review; mains safety is outside this specification.

## Flash and module-revision verification plan

FW-01E detected 4 MB physical SPI flash and the diagnostic image ran successfully. The image header is configured for 2 MB, so the current firmware view is intentionally smaller than the detected device. The exact `ESP-WROOM-32` module suffix and carrier-board revision are still needed to reconcile the fitted flash against the official module/carrier documentation. Verify in a later hardware session:

1. Read and photograph the full shield marking, including any `D`, `U`, `E`, `UE`, or other suffix and lot/date text needed to select the correct datasheet. Avoid publishing unique IDs.
2. Record the exact Espressif module datasheet title/revision and its specified flash configuration for that precise order code.
3. Verify the physical carrier-board revision and any flash-size options in its official schematic/BOM.
4. With the verified USB-UART port and installed ESP-IDF tools, query chip/flash identification using a non-erasing identification command. Capture the reported flash size and SoC revision, while redacting MAC/unique identifiers. Do not run erase or write operations for identification.
5. Reconcile module datasheet, carrier-board BOM, and detected flash capacity. Record any discrepancy; do not silently choose one source.
6. Verify PSRAM separately from exact module/carrier documentation and a supported non-destructive runtime check. No PSRAM is currently claimed.

## Board configuration header

`main/board_config.h` is a reference profile only. It states accepted identity/capability metadata and explicit unknown/unassigned sentinels. It is not included in the application and defines no GPIO mapping or relay behavior. Keep firmware logic independent of this board profile until a later approved integration task.

## References

- [ESP-IDF v6.1 GPIO & RTC GPIO — ESP32](https://docs.espressif.com/projects/esp-idf/en/v6.1/esp32/api-reference/peripherals/gpio.html)
- [ESP32 Series Datasheet](https://documentation.espressif.com/esp32_datasheet_en.html)
- [ESP-WROOM-32 Datasheet](https://documentation.espressif.com/esp32-wroom-32_datasheet_en.html)
- [CP2102 data sheet / Silicon Labs product information](https://www.silabs.com/interface/usb-bridges/classic/device.cp2102)
