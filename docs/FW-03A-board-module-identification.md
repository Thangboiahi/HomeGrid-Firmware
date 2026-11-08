# FW-03A — Exact Board and Module Identification Report

**Milestone:** FW-03 — Hardware Abstraction and Board Integration Planning  
**Work package:** FW-03A — Exact Board and Module Identification  
**Authority:** `HG-HQ-ED-FW03-001` / `HG-HQ-DEC-20261108-FW03-001`  
**Issued:** 2026-11-08  
**Status:** Physical photo evidence reviewed; visible board/module markings recorded; carrier manufacturer/PCB revision and electrical documentation remain unverified  
**Repository baseline:** `main` after FW-02 closure

## 1. Purpose and scope

Establish the hardware facts that firmware may safely rely on before proposing board-specific interfaces. This report separates directly evidenced facts, department-level family identification, and unresolved details.

FW-03A is an identification and documentation task only. No GPIO is configured, no peripheral is activated, no firmware is flashed, and no relay, fan, mains or load is connected or operated.

## 2. Authority and evidence reviewed

| Evidence | Source | Finding | Confidence / limitation |
|---|---|---|---|
| FW-01F final acceptance | `docs/FW-01-final-acceptance-report.md` | Accepted FW-01 physical verification record | Primary project evidence |
| User-supplied physical photographs (five views, 2026-11-08) | Front, back and close-up board photographs | Back silkscreen reads `ESP32 DEVKIT V1`; module shield reads `ESP-WROOM-32`; visible FCC ID `2AC7Z-ESP32WROOM32`; micro-USB connector and EN/BOOT buttons visible | Physical markings are evidence; they do not establish carrier manufacturer, PCB revision, module suffix, schematic or electrical ratings |
| FW-01E hardware report | `docs/FW-01E-hardware-verification-report.md` | Physical chip and flash identification, serial interface evidence | Underlying test evidence; not a board schematic |
| FW-01 serial capture | `docs/FW-01E-serial.log` and timestamp record | ESP-IDF v6.1 boot/runtime; chip revision and capability reporting | Does not identify PCB or module suffix |
| FW-01 build/configuration record | `docs/FW-01D-build-report.md` and project configuration | Original ESP32 target; 2 MB image configuration | Build configuration is not a physical flash BOM |

## 3. Consolidated identity register

| Attribute | Current recorded value | Evidence status | FW-03A disposition |
|---|---|---|---|
| Development board | ESP32 DEVKIT V1 (as printed on PCB back) | User-supplied photograph; visible silkscreen | Board family/printed designation identified; clone manufacturer and exact PCB revision unknown |
| Carrier manufacturer | Unknown / not marked in supplied views | No manufacturer logo or authoritative vendor record visible | Remains unverified |
| Carrier model | ESP32 DEVKIT V1 designation (printed) | Visible back silkscreen `ESP32 DEVKIT V1` | Designation recorded; exact vendor-specific model not established |
| Carrier PCB revision | Unknown | No revision identifier visible in supplied photographs | Remains unverified |
| SoC | ESP32-D0WD-V3 | Positively identified by esptool | Verified |
| Silicon revision | 3.0 | esptool and boot evidence | Verified |
| Module marking | ESP-WROOM-32 | Visible shield marking; FCC ID `2AC7Z-ESP32WROOM32` also visible | Marking recorded; exact module suffix/order code and BOM revision not shown |
| Module suffix/order code | Not visible / not established | Shield reads `ESP-WROOM-32` without a legible suffix | Do not infer flash/PSRAM variant from this marking alone |
| Module hardware revision | Unknown | No exact module marking/document | Pending |
| ESP-IDF target | `esp32` | FW-01D build and FW-01E runtime accepted | Verified; preserve |
| Framework baseline | ESP-IDF v6.1 | FW-01 build record | Verified; preserve |
| Physical flash | 4 MB detected | esptool flash identification and bootloader report | Verified for tested device |
| Application image flash view | 2 MB configured | Image header reports 2048 kB | Verified configuration; do not change |
| PSRAM | Unknown | No exact module/board evidence or supported runtime result | Must remain unknown |
| USB-UART bridge | CH9102 | Windows PnP: VID `1A86`, PID `55D4`, COM8 at FW-01E capture | Verified at time of test; COM port may change |
| USB connector | Micro-USB | Clearly visible in supplied front photographs | Connector type visually identified; power-path/rating remains unverified |
| Wi-Fi/BLE capability | ESP32 family-level capability | Not activated or tested in FW-01 | Capability reference only; not an FW-03 implementation permission |
| Board power input/rating | Unknown | No schematic or authoritative carrier documentation | Hardware Department to verify |
| Available peripherals/pin routing | Unknown | No carrier schematic or verified pinout | Do not allocate pins yet |

## 4. Confirmed firmware baseline

The following are safe to carry forward as the tested firmware baseline or physically observed markings (the source of each fact is identified):

- SoC: ESP32-D0WD-V3, silicon revision 3.0.
- ESP-IDF target: `esp32`.
- Framework: ESP-IDF v6.1.
- Physical flash detected: 4 MB.
- Existing image header/configuration: 2 MB, 40 MHz, DIO.
- USB serial bridge observed during FW-01E: CH9102, VID:PID `1A86:55D4`.
- Carrier PCB is printed `ESP32 DEVKIT V1` in the supplied back-side photograph; manufacturer and PCB revision are not visible.
- Module shield is printed `ESP-WROOM-32`; visible FCC ID is `2AC7Z-ESP32WROOM32`. No suffix is established.
- Micro-USB connector, EN and BOOT buttons are visually present.
- Existing FW-02 output-permission domain remains deny-only. FW-03 does not authorize changing it.

The 4 MB physical flash observation does not authorize changing the 2 MB image configuration. No PSRAM presence or capacity is assumed.

## 5. Hardware Department verification request

Hardware Department is requested to inspect the actual bare development board and return evidence for the following:

1. Confirm whether `ESP32 DEVKIT V1` is the exact board designation and identify its manufacturer/vendor, if known.
2. Confirm whether any additional module suffix/order code is present; the supplied photographs show `ESP-WROOM-32` and FCC ID `2AC7Z-ESP32WROOM32` only.
3. Provide exact carrier-board PCB revision or explicitly record that no revision marking is present. The supplied photographs already provide front, back and module close-up views.
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

**Physical identification now established from photographs:** the carrier PCB is marked `ESP32 DEVKIT V1`; the module shield is marked `ESP-WROOM-32` with visible FCC ID `2AC7Z-ESP32WROOM32`; the connector is Micro-USB. These are visual observations, not proof of manufacturer or electrical specification.

**Still pending for full hardware qualification:** carrier manufacturer and PCB revision (or formal confirmation that none is marked), exact module variant/order code if available, PSRAM evidence, board power-input/power-path characteristics, and authoritative schematic/pinout. Hardware Department must verify these or formally record them as unavailable. FW-03B can proceed with abstract interfaces; board-specific GPIO allocation and electrical assumptions remain gated.

No unsupported identity, memory, power, GPIO or peripheral claims are promoted to verified status. FW-03B may begin only on the basis of the verified facts and may document abstract interface requirements without relying on unresolved board-specific details. FW-03C pin allocation remains gated by the Hardware evidence above.

---
**Prepared by:** HomeGrid Firmware Department  
**Review requested from:** HomeGrid Hardware Department  
**Coordination:** HomeGrid Software Department  
