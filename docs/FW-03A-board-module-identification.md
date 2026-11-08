# FW-03A — Exact Board and Module Identification Report

**Milestone:** FW-03 — Hardware Abstraction and Board Integration Planning  
**Work package:** FW-03A — Exact Board and Module Identification  
**Authority:** `HG-HQ-ED-FW03-001` / `HG-HQ-DEC-20261108-FW03-001`  
**Issued:** 2026-11-08  
**Status:** Firmware evidence audit complete; exact carrier-board and module identity awaiting Hardware Department verification  
**Repository baseline:** `main` after FW-02 closure

## 1. Purpose and scope

Establish the hardware facts that firmware may safely rely on before proposing board-specific interfaces. This report separates directly evidenced facts, department-level family identification, and unresolved details.

FW-03A is an identification and documentation task only. No GPIO is configured, no peripheral is activated, no firmware is flashed, and no relay, fan, mains or load is connected or operated.

## 2. Authority and evidence reviewed

| Evidence | Source | Finding | Confidence / limitation |
|---|---|---|---|
| FW-01F final acceptance | `docs/FW-01-final-acceptance-report.md` | Accepted FW-01 physical verification record | Primary project evidence |
| Board identification record | `docs/board-identification.md` | Generic ESP32 development board; ESP32-D0WD-V3 rev 3.0; ESP-WROOM-32 family-level identification | Exact carrier and module markings absent |
| FW-01E hardware report | `docs/FW-01E-hardware-verification-report.md` | Physical chip and flash identification, serial interface evidence | Underlying test evidence; not a board schematic |
| FW-01 serial capture | `docs/FW-01E-serial.log` and timestamp record | ESP-IDF v6.1 boot/runtime; chip revision and capability reporting | Does not identify PCB or module suffix |
| FW-01 build/configuration record | `docs/FW-01D-build-report.md` and project configuration | Original ESP32 target; 2 MB image configuration | Build configuration is not a physical flash BOM |

## 3. Consolidated identity register

| Attribute | Current recorded value | Evidence status | FW-03A disposition |
|---|---|---|---|
| Development board | Generic ESP32 development board | Family-level description only; no maker/model | Exact carrier identity pending |
| Carrier manufacturer | Unknown | No legible marking or vendor document recorded | Hardware Department to inspect |
| Carrier model | Unknown | No product identifier recorded | Hardware Department to inspect |
| Carrier PCB revision | Unknown | No silkscreen/photo evidence recorded | Hardware Department to inspect |
| SoC | ESP32-D0WD-V3 | Positively identified by esptool | Verified |
| Silicon revision | 3.0 | esptool and boot evidence | Verified |
| Module family | ESP-WROOM-32 | Department-identified family | Family-level only |
| Module suffix/order code | Unknown | Shield marking not read | Hardware Department to inspect |
| Module hardware revision | Unknown | No exact module marking/document | Pending |
| ESP-IDF target | `esp32` | FW-01D build and FW-01E runtime accepted | Verified; preserve |
| Framework baseline | ESP-IDF v6.1 | FW-01 build record | Verified; preserve |
| Physical flash | 4 MB detected | esptool flash identification and bootloader report | Verified for tested device |
| Application image flash view | 2 MB configured | Image header reports 2048 kB | Verified configuration; do not change |
| PSRAM | Unknown | No exact module/board evidence or supported runtime result | Must remain unknown |
| USB-UART bridge | CH9102 | Windows PnP: VID `1A86`, PID `55D4`, COM8 at FW-01E capture | Verified at time of test; COM port may change |
| USB connector | Micro-USB recorded in prior worksheet | No clear photo/board documentation cited | Treat as provisional pending Hardware inspection |
| Wi-Fi/BLE capability | ESP32 family-level capability | Not activated or tested in FW-01 | Capability reference only; not an FW-03 implementation permission |
| Board power input/rating | Unknown | No schematic or authoritative carrier documentation | Hardware Department to verify |
| Available peripherals/pin routing | Unknown | No carrier schematic or verified pinout | Do not allocate pins yet |

## 4. Confirmed firmware baseline

The following are safe to carry forward as the tested firmware baseline:

- SoC: ESP32-D0WD-V3, silicon revision 3.0.
- ESP-IDF target: `esp32`.
- Framework: ESP-IDF v6.1.
- Physical flash detected: 4 MB.
- Existing image header/configuration: 2 MB, 40 MHz, DIO.
- USB serial bridge observed during FW-01E: CH9102, VID:PID `1A86:55D4`.
- Existing FW-02 output-permission domain remains deny-only. FW-03 does not authorize changing it.

The 4 MB physical flash observation does not authorize changing the 2 MB image configuration. No PSRAM presence or capacity is assumed.

## 5. Hardware Department verification request

Hardware Department is requested to inspect the actual bare development board and return evidence for the following:

1. Clear front and back photographs of the board, with external loads disconnected.
2. Close-up photograph of the module shield marking, including the complete suffix/order code.
3. Legible carrier-board manufacturer, model and PCB revision markings.
4. Exact board/module datasheet, schematic and pinout, if identifiable from the markings.
5. USB-UART bridge package marking or authoritative carrier documentation confirming CH9102.
6. Board power-input connector, supported input voltage and documented power path.
7. Exact module flash/PSRAM specifications from the matching manufacturer documentation; reconcile against the 4 MB detected flash evidence.
8. Board-routed peripherals and any pins reserved by onboard circuitry.
9. Any errata or revision-specific restrictions relevant to GPIO, boot strapping or peripheral routing.

Do not infer identity from appearance alone. If the board is an unbranded generic carrier and no authoritative documentation exists, record it as such and identify only the parts supported by physical markings and direct evidence.

## 6. Evidence handling and safety

- Do not photograph, publish or commit unique MAC addresses, eFuse data, credentials, tokens or other unnecessary device-unique secrets.
- No flash erase, firmware flashing, system configuration changes or tool installation is required for this work package.
- Do not connect relays, fans, mains or unknown external circuits during identification.
- Do not test GPIO by driving pins or connecting external loads.
- Any later non-destructive identification command must be separately reviewed if it requires new software, driver, system or hardware changes.

## 7. Open items and ownership

| ID | Open item | Owner | Required evidence | Blocks |
|---|---|---|---|---|
| FW03A-OPEN-01 | Carrier manufacturer/model/revision | Hardware | Board markings, photos, official schematic/datasheet | Board-specific pin/peripheral mapping |
| FW03A-OPEN-02 | Exact ESP-WROOM-32 suffix and module revision | Hardware | Shield marking and matching Espressif document | Module capability and memory confirmation |
| FW03A-OPEN-03 | PSRAM presence/capacity | Hardware + Firmware | Exact BOM/module document and later approved runtime evidence if needed | Any PSRAM-dependent design |
| FW03A-OPEN-04 | Board USB connector and power input characteristics | Hardware | Physical inspection and carrier documentation | Power-interface design |
| FW03A-OPEN-05 | Onboard peripheral and pin routing | Hardware | Exact schematic/pinout | FW-03C GPIO proposal |

## 8. FW-03A exit assessment

**Completed by Firmware:** prior evidence was reviewed and consolidated; verified facts and unknowns are explicitly separated; the Hardware Department inspection request and evidence checklist are documented.

**Pending before FW-03A can be marked fully complete:** Hardware Department must verify the exact carrier-board identity, module marking/suffix and relevant electrical/documentation facts, or formally report that the board is generic/unidentifiable and provide the strongest available physical evidence.

No unsupported identity, memory, power, GPIO or peripheral claims are promoted to verified status. FW-03B may begin only on the basis of the verified facts and may document abstract interface requirements without relying on unresolved board-specific details. FW-03C pin allocation remains gated by the Hardware evidence above.

---
**Prepared by:** HomeGrid Firmware Department  
**Review requested from:** HomeGrid Hardware Department  
**Coordination:** HomeGrid Software Department  
