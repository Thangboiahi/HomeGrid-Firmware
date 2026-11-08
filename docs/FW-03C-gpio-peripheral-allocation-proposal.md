# FW-03C — GPIO and Peripheral Allocation Proposal

**Milestone:** FW-03 — Hardware Abstraction and Board Integration Planning  
**Work package:** FW-03C — GPIO and Peripheral Allocation Proposal  
**Authority:** `HG-HQ-ED-FW03-001` / `HG-HQ-DEC-20261108-FW03-001`  
**Status:** Preliminary SoC-level suitability proposal; carrier schematic and Hardware Department review pending  
**Baseline:** ESP32-D0WD-V3 silicon revision 3.0; ESP-WROOM-32 shield marking; PCB silkscreen `ESP32 DEVKIT V1`; ESP-IDF v6.1; existing 2 MB image configuration

## 1. Purpose and safety boundary

This document proposes a cautious GPIO/peripheral suitability register for the photographed ESP32 DEVKIT V1-style board. It is a planning artifact only.

**No GPIO configuration, pin muxing, output writes, peripheral initialization, electrical probing, actuator connection, relay switching, fan operation or mains-connected test is authorized or performed.** The FW-02 output-permission enum remains deny-only. This document does not authorize a future physical driver.

The photographs show header labels, not copper routing, board schematic, component values or electrical ratings. Therefore the table is a preliminary ESP32-class signal suitability map, not a final carrier-board pin allocation.

## 2. Evidence basis and confidence

### 2.1 Directly observed in user-supplied photographs

- PCB back silkscreen: `ESP32 DEVKIT V1`.
- Module shield: `ESP-WROOM-32`; visible FCC ID `2AC7Z-ESP32WROOM32`.
- Header labels visible include `D0`, `D2`, `D4`, `D5`, `D12`–`D19`, `D21`–`D23`, `D25`–`D27`, `D32`–`D35`, `VP`, `VN`, `TX`, `RX`, `3V3`, `VIN`, `GND`, `EN`, `VP`, `VN`.
- Micro-USB connector, EN and BOOT buttons are visible.

### 2.2 Previously verified by FW-01

- ESP32-D0WD-V3 silicon revision 3.0.
- 4 MB physical flash detected; existing application image configuration is 2 MB and must remain unchanged.
- CH9102 USB-UART bridge observed as VID:PID `1A86:55D4`.
- ESP-IDF v6.1 and `esp32` target.

### 2.3 Evidence not available

- Authoritative schematic, board vendor, PCB revision and exact board BOM.
- Confirmation that every header silk label is routed directly to the identically numbered SoC GPIO.
- Exact module suffix/order code and PSRAM status.
- Onboard LED, USB-UART, regulator, auto-reset and boot-button circuit nets.
- Board power input limits, regulator thermal/current capability and 3.3 V rail budget.
- External circuitry already attached to any header or net.

**Rule:** A visible label is evidence of the printed designation only. Before any implementation, Hardware must verify the exact schematic/net mapping and inspect onboard loads.

## 3. ESP32-class restrictions relevant to this proposal

The following are SoC-family constraints from Espressif's ESP32 documentation. They are not a substitute for the carrier-board schematic.

1. GPIO6–GPIO11 are normally used by the module's SPI flash interface. Treat them as reserved and unavailable.
2. GPIO34–GPIO39 are input-only. They do not provide output drivers or internal pull-up/pull-down resistors. The photographed board exposes labels for GPIO34, GPIO35, VP/GPIO36 and VN/GPIO39; GPIO37/38 are not normally exposed on this board style.
3. GPIO0, GPIO2, GPIO5, GPIO12/MTDI and GPIO15/MTDO are strapping pins. External circuitry or inappropriate reset-time levels can change boot configuration. Do not select them for safety-relevant outputs in this proposal.
4. GPIO1/TX0 and GPIO3/RX0 are associated with UART0 and commonly used for boot logs/programming through the onboard USB-UART bridge. Reserve them unless Hardware verifies a safe alternative programming/logging path.
5. GPIO16 and GPIO17 are available on the classic ESP32-WROOM-32 SoC family in general, but board routing and any module variant must still be checked.
6. ADC2 resources have Wi-Fi-related limitations on the classic ESP32. Do not assign an ADC2 input for a future requirement without checking the ESP-IDF version-specific peripheral restrictions and expected Wi-Fi coexistence.
7. GPIO pins are not 5 V tolerant. Any future external interface must be electrically compatible with ESP32 IO limits and be reviewed by Hardware.

Primary references:
- Espressif ESP32 Series Datasheet, pin restrictions and boot strapping: https://documentation.espressif.com/esp32_datasheet_en.html
- Espressif ESP32-DevKitC board documentation (reference only; the photographed board is not proven to be an Espressif-manufactured DevKitC): https://documentation.espressif.com/projects/arduino-esp32/en/latest/boards/ESP32-DevKitC-1.html

## 4. Preliminary GPIO suitability register

Legend:
- **Reserved** — do not allocate.
- **Avoid** — do not select for new HomeGrid outputs without an explicit, documented exception and Hardware approval.
- **Conditional candidate** — potentially suitable at SoC level; not approved or assigned to a HomeGrid function.
- **Input-only** — can be considered only for a compatible input/feedback function after electrical verification.
- **Board verification required** — onboard circuit/routing could alter suitability.

| GPIO / board label | SoC-level characteristics / known concern | Preliminary disposition | Required verification |
|---|---|---|---|
| GPIO0 / D0 | Boot-mode strapping; BOOT button commonly pulls it low | Avoid; not a default actuator candidate | Confirm BOOT circuit and reset-time levels |
| GPIO1 / TX0 | UART0 TX; commonly boot/programming log output | Reserve for console/programming unless formally redesigned | Verify CH9102 routing and programming/debug strategy |
| GPIO2 / D2 | Boot strapping; boot-mode interaction | Avoid for outputs | Confirm board reset circuitry and external loading |
| GPIO3 / RX0 | UART0 RX; commonly connected to USB-UART | Reserve for console/programming unless formally redesigned | Verify CH9102 routing and programming/debug strategy |
| GPIO4 / D4 | General-purpose capable; ADC2/touch multiplexing | Conditional candidate | Verify onboard circuitry, reset state, IO voltage and selected peripheral |
| GPIO5 / D5 | Strapping pin; also peripheral signal function | Avoid for outputs by default | Verify boot strap network and attached circuits |
| GPIO6–GPIO11 | Module SPI flash signals | Reserved; do not use | No allocation permitted |
| GPIO12 / D12 / MTDI | Strapping pin; affects VDD_SDIO voltage selection | Avoid; especially unsuitable for casual output assignment | Verify module flash supply/strap behavior and board network |
| GPIO13 / D13 | General-purpose capable; JTAG-related alternate function | Conditional candidate | Verify routing, debug needs and attached circuitry |
| GPIO14 / D14 | General-purpose capable; JTAG-related alternate function | Conditional candidate | Verify routing, debug needs and attached circuitry |
| GPIO15 / D15 / MTDO | Strapping pin; boot log/SDIO timing function | Avoid for outputs by default | Verify strap network and boot behavior |
| GPIO16 / D16 (if exposed) | General-purpose capable on classic ESP32-WROOM-32 | Conditional candidate; board exposure not confirmed from supplied views | Confirm exact header pad/routing and module variant |
| GPIO17 / D17 (if exposed) | General-purpose capable on classic ESP32-WROOM-32 | Conditional candidate; board exposure not confirmed from supplied views | Confirm exact header pad/routing and module variant |
| GPIO18 / D18 | General-purpose capable; common SPI clock alternate | Conditional candidate | Check SPI/peripheral sharing and onboard routing |
| GPIO19 / D19 | General-purpose capable; common SPI MISO alternate | Conditional candidate | Check SPI/peripheral sharing and onboard routing |
| GPIO21 / D21 | General-purpose capable; commonly used for I2C SDA by convention | Conditional candidate | Confirm no onboard use; I2C pull-ups and bus voltage require Hardware design |
| GPIO22 / D22 | General-purpose capable; commonly used for I2C SCL by convention | Conditional candidate | Confirm no onboard use; I2C pull-ups and bus voltage require Hardware design |
| GPIO23 / D23 | General-purpose capable; common SPI MOSI alternate | Conditional candidate | Check SPI/peripheral sharing and onboard routing |
| GPIO25 / D25 | General-purpose capable; ADC2/DAC alternate functions | Conditional candidate | Check ADC2/Wi-Fi coexistence if analog input is proposed |
| GPIO26 / D26 | General-purpose capable; ADC2/DAC alternate functions | Conditional candidate | Check ADC2/Wi-Fi coexistence if analog input is proposed |
| GPIO27 / D27 | General-purpose capable; ADC2/touch alternate functions | Conditional candidate | Check ADC2/Wi-Fi coexistence if analog input is proposed |
| GPIO32 / D32 | General-purpose capable; ADC1/touch/RTC alternate functions | Conditional candidate | Confirm routing and intended analog/digital requirements |
| GPIO33 / D33 | General-purpose capable; ADC1/touch/RTC alternate functions | Conditional candidate | Confirm routing and intended analog/digital requirements |
| GPIO34 / D34 | Input-only; no internal pull-up/down | Input/feedback candidate only | Verify external biasing, signal conditioning and protection |
| GPIO35 / D35 | Input-only; no internal pull-up/down | Input/feedback candidate only | Verify external biasing, signal conditioning and protection |
| GPIO36 / VP | Input-only; ADC1; no internal pull-up/down | Input/feedback candidate only | Verify external biasing, input range and protection |
| GPIO39 / VN | Input-only; ADC1; no internal pull-up/down | Input/feedback candidate only | Verify external biasing, input range and protection |

No GPIO is assigned to a relay, fan, switch, sensor or HomeGrid channel in this document.

## 5. Peripheral planning register

| Peripheral / function | Candidate signal set (SoC-level only) | Constraints and dependencies | FW-03 disposition |
|---|---|---|---|
| UART0 console/programming | GPIO1 TX0, GPIO3 RX0 | Existing CH9102 bridge likely uses this path; verify board routing | Reserve |
| SPI general-purpose bus | Common VSPI signals GPIO18 SCLK, GPIO19 MISO, GPIO23 MOSI, optional GPIO5 CS | GPIO5 is a strapping pin; select a verified non-strapping CS only after design review | No allocation |
| I2C | GPIO21 SDA and GPIO22 SCL are common conventions, not fixed requirements | Requires verified pull-ups, voltage domain, bus capacitance and peripheral inventory | No allocation |
| ADC1 input | GPIO32, GPIO33, GPIO34, GPIO35, GPIO36, GPIO39 (subject to package/board exposure) | Input range, attenuation, source impedance, filtering and protection require design review | Input candidates only |
| ADC2 input | GPIO4, GPIO25, GPIO26, GPIO27 and other ADC2-capable pins | Classic ESP32 ADC2/Wi-Fi resource contention must be considered | Avoid as a default design choice |
| PWM / LEDC | GPIO matrix can route PWM to eligible output-capable GPIOs | Pin suitability, reset behavior, driver circuit and output-safe behavior must be reviewed; no PWM parameters selected | No allocation |
| RMT / pulse interface | Eligible GPIO matrix pins | Timing and electrical interface depend on actual transceiver/driver hardware | No allocation |
| Touch / capacitive | Touch-capable GPIOs including GPIO4, 12–15, 27, 32–33 | Noise, calibration and electrical layout; not assumed suitable for mains switch sensing | No allocation |
| TWAI/CAN | GPIO matrix routing to a suitable transceiver interface | External transceiver, isolation, termination and protection required | Not in current scope |
| Fan speed feedback | A verified input path; potentially GPIO34/35/36/39 for digital/analog observation | Signal conditioning, isolation, pulse levels, frequency and fail-detection must be specified by Hardware | No allocation |

Peripheral signal names are mux alternatives, not simultaneous independent resources. Resource conflicts must be checked against the complete selected peripheral configuration.

## 6. Conflict and risk register

| ID | Conflict / risk | Severity | Mitigation / exit evidence | Owner |
|---|---|---|---|---|
| FW03C-R01 | GPIO6–11 reserved for module flash | Critical | Hard-reserve; reject any allocation | Firmware |
| FW03C-R02 | GPIO0/2/5/12/15 strapping and boot dependencies | High | Exclude from default output candidates; verify reset network | Hardware + Firmware |
| FW03C-R03 | GPIO1/3 UART0 and USB-UART programming/debug dependency | High | Reserve until console strategy and routing are verified | Hardware + Firmware |
| FW03C-R04 | GPIO34–39 input-only; no internal pulls | High | Restrict to input-only use; Hardware supplies external bias/protection | Hardware |
| FW03C-R05 | Board clone/vendor/revision and schematic unknown | Critical | Obtain schematic or perform controlled net verification by Hardware; no final pin map before evidence | Hardware |
| FW03C-R06 | Onboard LED, regulator, auto-reset, USB-UART or other circuit may load candidate pins | High | Hardware BOM/schematic inspection and net tracing | Hardware |
| FW03C-R07 | 3.3 V IO limits and external 5 V/inductive/noisy signals | Critical | Hardware-approved level shifting, clamping, isolation and protection | Hardware |
| FW03C-R08 | ADC2/Wi-Fi contention | Medium | Prefer ADC1 for future analog sensing where appropriate; validate selected driver/API | Firmware |
| FW03C-R09 | Peripheral mux collisions (SPI/I2C/UART/PWM/RMT) | Medium | Maintain one integrated pin/peripheral allocation matrix after requirements are frozen | Firmware |
| FW03C-R10 | Power budget and regulator capability unknown | Critical | Obtain regulator part number/datasheet, input range, thermal/current limits and rail budget | Hardware |
| FW03C-R11 | Header silk labels may not match assumptions about a reference DevKit design | High | Verify exact board nets; don't infer from generic pinout images | Hardware |
| FW03C-R12 | Boot/reset transient could energize a future driver | Critical | Future hardware must provide independent hardware-safe default; separate safety review and implementation authorization required | Hardware + HQ |

## 7. Preliminary candidate shortlist — not an assignment

For planning only, the less constrained output-capable GPIOs worth examining first after Hardware verification are:

`GPIO13, GPIO14, GPIO16, GPIO17, GPIO18, GPIO19, GPIO21, GPIO22, GPIO23, GPIO25, GPIO26, GPIO27, GPIO32, GPIO33`.

This list is **not a declaration that all these pins are free or safe on this particular carrier board**. GPIO16/17 exposure is not confirmed from the supplied photos. GPIO25/26/27 have ADC2 alternate functions; the eventual use must consider Wi-Fi coexistence. All candidates remain subject to board routing, onboard circuit, reset-level, driver electrical and product-level requirements review.

For input-only feedback planning, GPIO34, GPIO35, GPIO36/VP and GPIO39/VN are possible SoC-level candidates, but they require external biasing/conditioning because internal pulls are absent. Their use is not approved.

## 8. Required Hardware Department review

Hardware Department must return an evidence-backed disposition for:

1. Exact board manufacturer/vendor and PCB revision, or formal statement that markings are absent.
2. Schematic and BOM, or an explicit statement that authoritative files are unavailable.
3. Net mapping from every proposed header pad to the ESP32 module GPIO.
4. CH9102 UART0 and auto-reset/BOOT circuit connections.
5. Onboard LED and any other components attached to candidate GPIOs.
6. Regulator part number, USB/VIN input path, permitted supply range and 3.3 V rail current/thermal budget.
7. Confirmation of ESP-WROOM-32 variant and any module/flash/PSRAM implications.
8. External header voltage assumptions and protection requirements.
9. Hardware-safe default requirements for any future relay/fan driver.
10. Identification of GPIOs that must be reserved for future sensors, buses or expansion.

If no schematic exists, Hardware should record the limits of the available evidence and propose a separately authorized, non-energized inspection method. Firmware will not infer or probe pins itself under this work package.

## 9. FW-03C exit assessment

**Firmware work completed:** SoC-level restrictions have been catalogued; photographed header labels have been recorded; candidate, avoid, reserved and input-only classes are distinguished; peripheral mux conflicts and hardware dependencies are listed; a preliminary shortlist is provided without assigning any product function.

**Pending for final board-specific allocation:** Hardware schematic/net verification, onboard circuit review, power/regulator evidence and Hardware Department acceptance. Therefore FW-03C is a **preliminary proposal**, not a final approved GPIO map.

FW-03D may proceed at the requirements/documentation level using logical actuator interfaces only. It must not select physical output pins or assume relay/fan driver polarity, voltage, isolation, timing or feedback circuitry.

## 10. Reference documents

- Espressif, ESP32 Series Datasheet: https://documentation.espressif.com/esp32_datasheet_en.html
- Espressif, ESP32-DevKitC-1 documentation (reference design only, not proof of this board's manufacturer): https://documentation.espressif.com/projects/arduino-esp32/en/latest/boards/ESP32-DevKitC-1.html

---
**Prepared by:** HomeGrid Firmware Department  
**Review requested from:** HomeGrid Hardware Department  
**Authority for final pin allocation and implementation:** HomeGrid HQ
