# FW-03F — Integration Planning and Verification Handover

**Milestone:** FW-03 — Hardware Abstraction and Board Integration Planning  
**Work package:** FW-03F — Integration Planning and Verification Handover  
**Authority:** `HG-HQ-ED-FW03-001` / `HG-HQ-DEC-20261108-FW03-001`  
**Issued:** 8 November 2026  
**Status:** Firmware planning milestone package consolidated; Hardware electrical evidence and cross-department acceptance remain open  
**Safety classification:** Planning/documentation only; no physical actuation authorization

## 1. Executive summary

Firmware has completed the FW-03 planning deliverables across FW-03A–FW-03F. The user-supplied photographs establish the visible board designation `ESP32 DEVKIT V1`, module shield marking `ESP-WROOM-32`, visible FCC ID `2AC7Z-ESP32WROOM32`, Micro-USB connector and EN/BOOT buttons. Previously verified FW-01 evidence establishes ESP32-D0WD-V3 silicon revision 3.0, 4 MB detected physical flash and CH9102 USB-UART bridge VID:PID `1A86:55D4`.

The photographs do **not** establish the carrier manufacturer/revision, exact module suffix, PSRAM, schematic, exact net routing, power/regulator characteristics or electrical safety. Therefore this report does not claim complete electrical hardware qualification or a final board-specific GPIO map.

FW-03B–FW-03E establish firmware-side interface, GPIO suitability, relay/fan driver, feedback, fault and safe-state requirements. These are planning specifications and proposals, not approved shared contracts or physical implementation.

**FW-03 is complete as a Firmware Department planning/documentation milestone, with explicit Hardware and HQ dependencies carried forward.** It is not hardware validation, production readiness, physical actuator readiness or safety approval.

## 2. Authority and scope

- HQ directive: `HG-HQ-ED-FW03-001`
- HQ decision: `HG-HQ-DEC-20261108-FW03-001`
- Predecessor: FW-02 formally accepted and closed.
- Supporting department: Hardware.
- Coordination department: Software.

Mandatory restrictions preserved:
1. No physical GPIO output implementation.
2. No relay switching or fan operation.
3. No mains-connected testing.
4. No modification to the FW-02 deny-only output-permission boundary.
5. No assumptions about relay polarity, driver circuitry or isolation.
6. No shared HomeGrid contract changes.
7. No system-wide development tool or PATH modification.
8. Preserve ESP-IDF v6.1, ESP32 target and 2 MB application image configuration.
9. Hardware claims remain evidence-qualified.
10. Any physical actuation requires separate HQ implementation authorization and Hardware safety review.

## 3. Consolidated hardware identity and capability report

### 3.1 Verified/observed identity

| Item | Finding | Evidence class |
|---|---|---|
| Board designation | `ESP32 DEVKIT V1` printed on PCB back | Direct user photograph |
| Module marking | `ESP-WROOM-32` on shield | Direct user photograph |
| Visible FCC ID | `2AC7Z-ESP32WROOM32` | Direct user photograph |
| Connector/buttons | Micro-USB, EN and BOOT visible | Direct user photograph |
| SoC | ESP32-D0WD-V3 | FW-01 device evidence |
| Silicon revision | 3.0 | FW-01 device evidence |
| Physical flash | 4 MB detected | FW-01 device evidence |
| Application image configuration | 2 MB | Existing firmware configuration; preserve |
| USB-UART | CH9102, VID:PID `1A86:55D4` | FW-01 host/device evidence |
| Toolchain baseline | ESP-IDF v6.1, target `esp32` | FW-01 verified baseline |

### 3.2 Unverified capability and board details

| Item | Status | Evidence needed / owner |
|---|---|---|
| Carrier manufacturer/vendor | Unknown | Hardware identification or vendor record |
| Exact carrier PCB revision | Unknown; no revision visible in supplied views | PCB marking or Hardware inspection |
| Exact ESP-WROOM-32 suffix/order code | Not established | Legible full module code/vendor documentation |
| PSRAM | Unknown | Exact module BOM/datasheet or verified runtime evidence |
| Schematic/BOM | Not supplied/verified | Hardware Department |
| Header-to-SoC net mapping | Labels visible; routing not independently verified | Schematic/net verification by Hardware |
| Onboard LED and auxiliary circuit loads | Unknown | Schematic/BOM and inspection |
| USB/VIN input limits and regulator | Unknown | Regulator marking/datasheet and board documentation |
| 3.3 V rail current/thermal budget | Unknown | Hardware power analysis |
| External IO tolerance/protection | Not established | Schematic and electrical review |

The board is identified by its visible designation, but the exact carrier vendor/revision and electrical capability are not fully qualified. No PSRAM capability is assumed. The 4 MB flash observation does not change the existing 2 MB application image configuration.

## 4. Deliverable inventory

| Package | Deliverable | Result | Open qualification |
|---|---|---|---|
| FW-03A | `docs/FW-03A-board-module-identification.md` | Photo evidence and verified baseline reconciled | Hardware electrical identity/capability evidence |
| FW-03B | `docs/FW-03B-hardware-software-interface-requirements.md` | Firmware/hardware/software boundaries and dependencies specified | Software contract review; Software repo was empty at inspection |
| FW-03C | `docs/FW-03C-gpio-peripheral-allocation-proposal.md` | Preliminary SoC suitability, conflicts and conditional candidates documented | Not a final pin map; Hardware schematic/net review |
| FW-03D | `docs/FW-03D-relay-fan-driver-interface-requirements.md` | Relay/fan electrical and logical evidence requirements documented | Actual driver/motor hardware not identified |
| FW-03E | `docs/FW-03E-feedback-fault-safe-state-requirements.md` | Feedback, fault taxonomy, state distinction and safe-state requirements documented | Actual feedback circuit and HQ hazard/safe-state decisions |
| FW-03F | This report | Consolidated handover and next-stage recommendation | HQ review/acceptance |

## 5. Firmware-to-hardware interface specification summary

Future hardware-specific implementations must sit behind a board-support/driver adapter. Application/domain code should request logical channel operations and receive typed results; it must not contain GPIO numbers, electrical polarity, coil voltage, PWM frequency or driver topology.

Key requirements:
- Separate desired, applied and observed state.
- Keep availability, fault and output permission independent.
- A driver request result is not proof of physical operation.
- Feedback is a separate input path with validity and freshness semantics.
- Invalid board profile, driver configuration, authorization or policy fails closed.
- Hardware must define electrical safe defaults for reset, brownout, unpowered MCU and disconnected control.
- No shared Software protocol/schema was invented; identity, command, configuration, state and diagnostics contracts require Software/HQ review.

## 6. GPIO and peripheral allocation proposal summary

The preliminary ESP32-class register classifies:
- GPIO6–GPIO11 as reserved for module flash.
- GPIO0/2/5/12/15 as strapping pins to avoid for new outputs by default.
- GPIO1/3 as UART0/USB-UART console/programming pins to reserve pending verification.
- GPIO34–GPIO39 as input-only; no internal pull-up/down.
- Other pins as conditional candidates only, pending exact carrier routing, onboard loads, reset behavior, power and product requirements.

No GPIO is assigned to any HomeGrid relay, fan, switch or sensor function. The candidate shortlist is not a final or approved pin map.

## 7. Relay/fan driver requirements summary

No actual relay/fan driver hardware, schematic, datasheet, polarity, input threshold, coil/contact rating, isolation rating, fan type, control method or feedback circuit was verified.

Hardware must provide exact part identification, schematics/BOM, input logic specification, load ratings, protection, isolation evidence, power/thermal budget, reset/unpowered behavior, feedback details and failure-mode analysis.

Firmware must not infer safety from a relay contact rating, optocoupler presence, module marketing description or software command result. Any mains separation/isolation claim requires appropriate authoritative evidence and independent safety review.

## 8. Feedback, fault detection and safe-state requirements summary

- No feedback means UNKNOWN, not OFF.
- Driver request acceptance does not establish contact closure or fan rotation.
- Stale, missing, implausible or contradictory feedback invalidates physical observation.
- Reboot must not automatically restore a previous ON command.
- Fault-clear authority, latching, retry and reconciliation policies require HQ/Software approval.
- Hardware must independently enforce required safe electrical defaults and protection.
- The FW-02 output-permission enum remains deny-only; no grant path was added.

## 9. Consolidated dependency and risk register

| ID | Dependency / risk | Owner | Impact if unresolved |
|---|---|---|---|
| FW03-HW-01 | Exact carrier vendor/revision, schematic and BOM | Hardware | Cannot finalize board profile or pin map |
| FW03-HW-02 | Module variant, flash/PSRAM capability | Hardware | Cannot finalize memory/peripheral capability profile |
| FW03-HW-03 | Header-to-GPIO mapping and onboard circuit loading | Hardware | Cannot approve pin allocation |
| FW03-HW-04 | Regulator, USB/VIN limits and 3.3 V rail budget | Hardware | Cannot approve external circuit/power integration |
| FW03-HW-05 | Actual relay driver identity, logic and load ratings | Hardware | Cannot design relay adapter |
| FW03-HW-06 | Fan/motor class and supported control method | Hardware | Cannot design fan control adapter |
| FW03-HW-07 | Isolation/protection and load installation safety | Hardware / HQ | Blocks any mains-related product implementation |
| FW03-HW-08 | Feedback circuits and diagnostic coverage | Hardware | Cannot claim physical observation |
| FW03-HQ-01 | System hazard analysis and safe-state decision | HQ / Hardware | Blocks physical implementation authorization |
| FW03-HQ-02 | Local/remote arbitration and command authority | HQ / Software | Blocks command execution policy |
| FW03-SW-01 | Versioned identity/registration contract | Software / HQ | Blocks device identity integration |
| FW03-SW-02 | Command, acknowledgement, idempotency and expiry contract | Software / HQ | Blocks command transport adapter |
| FW03-SW-03 | Desired/reported state and reconciliation contract | Software / HQ | Blocks state synchronization |
| FW03-SW-04 | Configuration schema/version/reset policy | Software / HQ | Blocks configuration integration |
| FW03-SW-05 | Diagnostics schema/privacy/retention | Software / HQ | Blocks diagnostics export |
| FW03-HQ-03 | Separate physical implementation authorization | HQ | Blocks any physical GPIO/actuator code |
| FW03-HQ-04 | Hardware safety review and approved test plan | Hardware / HQ | Blocks energized or mains-connected testing |

## 10. Future integration and verification plan

The recommended sequence is gated, not an automatic permission to energize hardware.

### Gate 0 — Hardware evidence completion
- Hardware supplies exact board/module identification, schematic/BOM, net mapping and power/regulator evidence.
- Hardware identifies the actual relay/fan driver and feedback components, if selected.
- Firmware updates the board capability profile and pin proposal only from verified evidence.
- Hardware signs off on electrical assumptions.

### Gate 1 — Cross-department contract review
- Software supplies authoritative identity, command, state, configuration and diagnostics contracts.
- HQ decides arbitration, authorization, restart and fault-clear policy.
- All shared contract changes go through HQ approval.

### Gate 2 — Design review
- Firmware prepares a board-support/driver adapter design against verified hardware.
- Hardware reviews reset-safe circuitry, isolation, protection, power and load suitability.
- HQ reviews hazard analysis and grants or rejects a separate implementation authorization.
- No physical actuation until this gate is explicitly passed.

### Gate 3 — Non-actuating implementation and tests
If separately authorized, begin with host tests and a mock adapter. Any platform code must preserve deny-by-default behavior and must not activate a real output path without a separately approved scope.

### Gate 4 — Controlled hardware verification
Only after written implementation authorization and Hardware safety approval:
- validate power/reset behavior with loads disconnected;
- verify logic-level interfaces using approved safe test fixtures;
- verify feedback conditioning independently;
- conduct any low-voltage load tests only under an approved procedure;
- mains-connected tests require a separate explicit approval and qualified safety arrangements.

### Gate 5 — Integration acceptance
- Reconcile firmware state with valid physical feedback.
- Verify faults, stale data, resets, brownouts and loss of communication.
- Review logs and evidence.
- Submit an integration acceptance report to HQ.

## 11. Recommendation for the next implementation milestone

**Recommended next milestone: FW-04 — Board Support and Driver Adapter Design (non-actuating, conditional).**

Proposed FW-04 should initially be a design and host-test milestone only, and should not begin physical GPIO output implementation until:
1. Hardware supplies and verifies the board schematic/net mapping and electrical capability report.
2. Actual relay/fan driver and feedback hardware are identified and documented.
3. Software/HQ provide the approved shared contracts and policy decisions required for integration.
4. HQ issues a separate implementation authorization defining exactly which non-actuating or physical behaviors may be implemented.
5. Hardware completes the relevant safety review.

The first FW-04 work package should reconcile the verified board profile and produce a reviewed board-support interface design. Physical driver implementation should be a separately gated package, not assumed by the FW-04 milestone title.

## 12. Final completion statement

The Firmware Department has completed the six FW-03 planning/documentation work packages within the authorized scope. The deliverables establish a defensible firmware-side abstraction and integration plan and record all known unknowns.

The following remain explicitly **not completed/approved**:
- Full electrical qualification of the development board.
- Final carrier-specific GPIO map.
- Actual relay/fan driver selection and electrical validation.
- Physical feedback implementation.
- Hazard analysis and product safe-state approval.
- Software contract integration.
- ESP-IDF hardware-driver build/integration.
- Physical actuator operation or mains testing.
- Production readiness or hardware safety certification.

**FW-03 completion is not permission for physical actuation.** FW-02 deny-only output permission remains unchanged.

---
**Prepared by:** HomeGrid Firmware Department  
**Submitted to:** HomeGrid HQ  
**Supporting review:** Hardware Department  
**Coordination review:** Software Department
