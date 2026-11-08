# FW-04A — Board Capability Profile and Evidence Reconciliation

**Authority:** `HG-HQ-ED-FW04-001` / `HG-HQ-DEC-20261108-FW04-001`  
**Milestone:** FW-04 — Board Support and Driver Adapter Design  
**Work package:** FW-04A — Board Capability Profile and Evidence Reconciliation  
**Status:** Evidence reconciliation completed; Hardware evidence gaps remain open  
**Classification:** Documentation/design only; no physical actuation

## 1. Purpose and decision

This package establishes a structured, evidence-qualified capability profile for the development board using:
- accepted FW-01 physical-device and build evidence;
- the board photographs supplied by the project owner;
- the FW-03A–FW-03F planning record; and
- the FW-04 HQ authorization.

No new Hardware Department report, schematic, BOM, board-revision record or electrical measurement was available in the accessible HomeGrid repositories during this package. The Hardware Department has a formal directive to supply those items; their absence is recorded as an open dependency, not filled with assumptions.

**FW-04A outcome:** the currently supportable board profile is reconciled and the remaining identity/capability gaps are registered. This is not full electrical qualification and does not authorize GPIO or actuator implementation.

## 2. Evidence classification

- **Verified:** directly established by accepted FW-01 device/build/runtime evidence.
- **Photo-observed:** visible in the owner-supplied photographs, but not independently authenticated against a manufacturer record.
- **Inferred / family-level:** consistent with the visible family marking or general chip family, but not a board-specific verified fact.
- **Unknown / pending:** evidence is absent or insufficient.
- **Configuration constraint:** an existing firmware/build setting that must be preserved under the HQ directive.

## 3. Structured board capability profile

| Profile field | Current value | Evidence class | Confidence / qualification |
|---|---|---|---|
| Project board label | ESP32 development board | Photo-observed | Generic description only |
| PCB silkscreen | `ESP32 DEVKIT V1` | Photo-observed | Visible on board back; does not uniquely identify manufacturer/revision |
| Carrier manufacturer | Unknown | Unknown | No verified manufacturer mark or vendor record |
| Carrier model/revision | Generic DevKit V1 marking; exact revision unknown | Photo-observed / Unknown | No revision identifier established |
| Module shield marking | `ESP-WROOM-32` | Photo-observed | Family marking; exact suffix/order code not legible/established |
| Visible FCC ID | `2AC7Z-ESP32WROOM32` | Photo-observed | Visible on shield; exact module variant still unverified |
| SoC | ESP32-D0WD-V3 | Verified | FW-01 esptool identification |
| Silicon revision | 3.0 | Verified | FW-01 esptool identification |
| CPU architecture/target | ESP32 / `esp32` | Verified | FW-01 ESP-IDF target and device evidence |
| Physical flash detected | 4 MB | Verified | FW-01 esptool flash identification |
| Firmware image flash configuration | 2 MB | Configuration constraint | Image header uses 2 MB; preserve |
| Partition layout | NVS 24 KiB at 0x9000; PHY init 4 KiB at 0xF000; factory app 1 MiB at 0x10000 | Verified | FW-01F accepted record |
| PSRAM | Unknown | Unknown | No reliable module BOM or accepted runtime evidence establishes presence/capacity |
| USB connector | Micro-USB | Photo-observed | Connector shape visible |
| USB-UART bridge | CH9102 | Verified | Windows enumeration in FW-01 |
| USB-UART VID:PID | `1A86:55D4` | Verified | FW-01 Windows device evidence |
| Host serial port at FW-01 test | COM8 | Verified (historical) | Port assignment may change between sessions |
| EN and BOOT buttons | Present | Photo-observed | Labels visible |
| Header labels | GPIO/power labels visible on two sides | Photo-observed | Labels do not prove exact net routing or electrical characteristics |
| Regulator identity | Unknown | Unknown | Part marking/BOM not verified |
| USB/VIN allowable input | Unknown | Unknown | No schematic/datasheet evidence |
| 3.3 V rail current/thermal budget | Unknown | Unknown | No power analysis |
| GPIO voltage tolerance/protection | Unknown | Unknown | No carrier schematic/electrical qualification |
| Onboard LED/auxiliary loads | Unknown | Unknown | No schematic/BOM or verified tracing |
| Reset/brownout/unpowered output electrical state | Unknown | Unknown | Requires Hardware evidence and safety review |
| ESP-IDF version | v6.1.0 | Verified | FW-01 accepted environment |
| Build target | `esp32` | Verified | FW-01 target build |
| Application image size configuration | 2 MB | Configuration constraint | HQ requires preservation |
| PSRAM dependency in firmware | None permitted/assumed | Configuration constraint | FW-02/FW-03 architecture has no PSRAM dependency |

## 4. Reconciled baseline and constraints

1. Continue using ESP-IDF v6.1.0 and target `esp32`.
2. Preserve the existing 2 MB application image configuration. FW-01 detected 4 MB physical flash, but the image header exposed a 2 MB view. Do not enlarge the flash view or alter partitioning under FW-04A.
3. Do not assume PSRAM exists.
4. Do not derive carrier manufacturer/revision from the generic `ESP32 DEVKIT V1` silkscreen alone.
5. Do not treat visible header labels as verified net mapping.
6. Do not infer regulator, USB/VIN, 3.3 V current, IO tolerance, protection or isolation characteristics from appearance.
7. Do not assign any GPIO to a HomeGrid load in FW-04A.
8. Keep the FW-02 output permission deny-only. This package makes no changes to firmware source or runtime behavior.

## 5. Hardware evidence request register

| ID | Required evidence | Why required | Owner | Status |
|---|---|---|---|---|
| HW-EV-01 | Carrier manufacturer, exact model and PCB revision | Board-specific capability identity | Hardware | Open |
| HW-EV-02 | Schematic, BOM and board layout | Verify net routing and onboard circuitry | Hardware | Open |
| HW-EV-03 | Exact module order code/suffix and datasheet | Confirm module variant and memory options | Hardware | Open |
| HW-EV-04 | PSRAM presence/capacity evidence | Establish actual memory capability | Hardware | Open |
| HW-EV-05 | Regulator part number/datasheet and power path | Establish supply limits | Hardware | Open |
| HW-EV-06 | USB/VIN ratings and 3.3 V rail budget | Prevent unsupported power assumptions | Hardware | Open |
| HW-EV-07 | Header-to-SoC GPIO net map | Permit board-specific pin review | Hardware | Open |
| HW-EV-08 | Onboard LED, USB bridge, boot circuitry and other pin loads | Identify conflicts and reset effects | Hardware | Open |
| HW-EV-09 | Reset, brownout and unpowered pin behavior | Establish safe electrical defaults | Hardware | Open |
| HW-EV-10 | GPIO protection/voltage limits and relevant layout evidence | Establish IO interface constraints | Hardware | Open |

## 6. Dependency and risk updates

| Risk | Description | Mitigation / exit condition |
|---|---|---|
| R-04A-01 | Generic board label may correspond to multiple carrier designs/revisions | Obtain PCB revision, manufacturer record and schematic |
| R-04A-02 | Module family marking does not establish exact memory variant | Obtain complete module code and manufacturer datasheet/BOM |
| R-04A-03 | 4 MB detected flash could be misinterpreted as permission to change firmware image layout | Preserve 2 MB image configuration until separately reviewed |
| R-04A-04 | Header silkscreen could be mistaken for verified electrical mapping | Require Hardware schematic/net continuity evidence |
| R-04A-05 | Unknown regulator and IO protection could cause unsafe external interfacing | No external electrical design approval until Hardware evidence review |
| R-04A-06 | No Hardware evidence package was available in accessible repository | Keep board-specific capabilities marked unknown; request formal Hardware handover |

## 7. Repository and scope impact

This is a documentation-only package. No firmware source, CMake, Kconfig, partition table, build configuration, shared HomeGrid contract, GPIO mapping, hardware driver or output-permission logic is changed.

No ESP-IDF build, flash, GPIO test, relay/fan operation or physical electrical test was performed for FW-04A.

## 8. FW-04A exit assessment

| Exit criterion | Result |
|---|---|
| Existing accepted FW-01 evidence reconciled | PASS |
| Owner-supplied board photographs incorporated with proper evidence labels | PASS |
| Confirmed and unverified characteristics separated | PASS |
| Structured board capability profile established | PASS |
| Board identity/capability gap register established | PASS |
| Missing schematic/pin/electrical data invented | PASS — none invented |
| Hardware Department evidence received | PENDING |
| Final board-specific electrical qualification | NOT CLAIMED |

**Package conclusion:** FW-04A is internally complete as an evidence-reconciliation and gap-register package. Hardware-owned evidence remains open and must be consumed before a board-specific profile can be marked electrically qualified. FW-04B may proceed with an abstraction design that does not depend on unresolved board-specific facts.

---
**Prepared by:** HomeGrid Firmware Department  
**Supporting owner:** Hardware Department  
**Authority:** `HG-HQ-ED-FW04-001`
