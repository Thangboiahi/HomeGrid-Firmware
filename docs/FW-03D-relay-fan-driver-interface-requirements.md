# FW-03D — Relay and Fan-Driver Interface Requirements

**Milestone:** FW-03 — Hardware Abstraction and Board Integration Planning  
**Work package:** FW-03D — Relay and Fan-Driver Interface Requirements  
**Authority:** `HG-HQ-ED-FW03-001` / `HG-HQ-DEC-20261108-FW03-001`  
**Status:** Logical interface and evidence requirements drafted; Hardware Department electrical review pending  
**Predecessors:** FW-03A photo-based identity; FW-03B interface requirements; FW-03C preliminary GPIO proposal

## 1. Purpose and restrictions

Define the information and interface requirements that Hardware must provide before a future relay or fan driver can be designed and Firmware can implement a physical adapter.

This is not a circuit design, pin assignment, driver selection or implementation authorization.

**Prohibited throughout FW-03:** GPIO configuration/output, relay energization, fan operation, mains connection, electrical probing of energized circuits, or modification of the FW-02 output-permission boundary. No relay polarity, active level, coil voltage, driver topology, isolation rating, fan type, PWM frequency or feedback level is assumed.

## 2. Present evidence and explicit unknowns

Available evidence establishes only the photographed ESP32 DEVKIT V1 silkscreen and ESP-WROOM-32 module marking, plus the previously verified ESP32-D0WD-V3 rev 3.0, 4 MB flash and CH9102 USB-UART baseline.

No relay module, fan-driver schematic, part number, PCB, datasheet, isolation barrier, connector pinout, load rating or feedback circuit has been verified in the firmware evidence available for this work package.

The following therefore remain **UNKNOWN / NOT APPROVED**:
- Relay module/driver manufacturer, model, topology and channel count.
- Input logic voltage/current, active polarity, thresholds and default state.
- Coil voltage/current, suppression network and contact ratings.
- Whether input-to-load galvanic isolation exists and its certified rating.
- Creepage, clearance, dielectric withstand and enclosure requirements.
- Fan motor type (AC induction, BLDC, DC brushed or other), rated voltage/current and starting/stall current.
- Fan speed-control method, if any (relay switching, phase control, PWM, 0–10 V, proprietary interface, etc.).
- Feedback signal type, voltage/current, frequency, isolation and failure behavior.
- Shared supply/ground arrangements and transient/noise characteristics.

No statement in this document should be read as evidence that a relay/fan driver is safe or compatible.

## 3. Required logical firmware boundary

The future application-facing abstraction should remain independent of electrical details. The intended conceptual boundary is:

`Application use case → logical actuator port → Hardware-owned driver adapter → approved driver circuit → load`

The domain/application layer may request a logical operation on an identified channel. It must not know:
- GPIO number or header position;
- active-high/active-low polarity;
- transistor/MOSFET/optocoupler/relay topology;
- coil or motor voltage/current;
- PWM carrier frequency, duty-cycle limits or phase timing;
- electrical feedback voltage thresholds.

A driver adapter must return a typed software result such as accepted, rejected, unavailable, timed out or internal error. A successful software request is not proof that a relay contact moved, a fan started or the load reached the requested state.

The physical output permission remains denied in the current FW-02 implementation. No API or configuration introduced by FW-03D may grant it.

## 4. Relay driver requirements for Hardware

Hardware Department must provide an evidence-backed specification for each relay channel.

### 4.1 Identification and topology
- Manufacturer, exact part number, revision and authoritative datasheet.
- Complete schematic and BOM, including input stage, driver transistor/MOSFET, optocoupler (if present), relay coil and suppression components.
- Channel count and connector/pinout with unambiguous signal names.
- Whether input and load sides share a ground or supply.
- Any indicator LED or onboard circuitry that loads the control input.

### 4.2 Logic interface
- Input voltage operating range and absolute maximum.
- Guaranteed logic-low and logic-high thresholds.
- Input current at each logic state and any required source/sink capability.
- Active polarity and truth table, including disconnected, high-impedance and unpowered controller conditions.
- Power-up, reset, brownout and cable-disconnect behavior.
- Required external pull-up/pull-down and whether it is present on the driver.
- Maximum switching rate, minimum pulse width and any latching behavior.

These are requirements to be measured/documented by Hardware, not values Firmware may infer.

### 4.3 Load-side and protection
- Relay coil voltage/current and tolerance.
- Contact ratings by load type, voltage, current, inrush and switching category.
- DC/AC ratings separately; resistive ratings must not be treated as motor ratings.
- Contact life and expected switching duty.
- Coil flyback/surge suppression and contact arc/surge suppression design.
- Thermal limits, PCB copper, terminal ratings and enclosure requirements.
- Fusing/overcurrent protection and fault containment.
- Certified isolation details, creepage/clearance and dielectric test evidence where mains separation is claimed.

A relay contact rating alone does not establish suitability for a particular motor or installation.

## 5. Fan-driver requirements for Hardware

Hardware must first identify the fan/motor class and intended control method. No fan control architecture is selected in FW-03D.

Required information:
- Fan/motor manufacturer, model, type and authoritative datasheet.
- Nominal/minimum/maximum supply voltage.
- Running, startup/inrush and stall current.
- Whether speed control is supported and the manufacturer-approved method.
- Driver topology, semiconductor ratings, thermal design and protection.
- Control input electrical levels, polarity, isolation and safe default.
- Minimum/maximum command range, deadband, startup boost or minimum stable speed, where applicable.
- Behavior on controller reset, loss of control signal, supply brownout and driver fault.
- Required suppression/filtering and EMC considerations.
- Feedback availability and electrical details (tachometer, fault output, current sense or other).
- Safe response to blocked rotor, overtemperature, overcurrent and disconnected fan.

If fan speed control is not supported by the selected motor/driver, Firmware must not emulate it by inventing PWM or phase-control behavior.

## 6. Feedback and diagnostics interface requirements

Command and feedback are separate paths.

For each feedback signal Hardware must specify:
- Physical quantity actually measured (contact position, auxiliary contact, tachometer pulse, current, temperature, fault flag, etc.).
- Source component and exact circuit location.
- Signal type and electrical range at the ESP32-side interface.
- Isolation and protection boundary.
- Active polarity, thresholds, pulse width/frequency range and sampling constraints.
- External biasing, filtering, debounce and edge conditioning.
- Validity detection, stale-data threshold and open/short/stuck-signal behavior.
- Startup and disconnected-state behavior.
- Diagnostic coverage and known undetectable failures.

Firmware must report `UNKNOWN` where valid physical feedback is absent. It must not infer physical state from command acceptance, relay coil command, PWM request or elapsed time alone.

## 7. Failure behavior and safe defaults

Hardware Department must document the physical state of the driver/load for:
- ESP32 unpowered;
- ESP32 reset/bootloader/firmware startup;
- GPIO high impedance;
- controller brownout;
- driver logic supply absent while load supply remains;
- load supply absent while logic supply remains;
- control wire open/short;
- driver thermal/overcurrent fault;
- relay contact welded or failed to close;
- fan disconnected or stalled;
- feedback wire open/short or feedback sensor failure.

The required safe state must be established through system hazard analysis and Hardware design, not by a software default alone. A software-only deny flag cannot guarantee an electrical output state during reset, brownout or hardware failure.

Any safety-critical de-energization, interlock or watchdog behavior must be enforced by appropriate hardware and independently reviewed. This document does not prescribe a particular circuit.

## 8. Hardware evidence package / review checklist

Hardware Department is requested to provide, for each distinct driver:
1. Photograph of both sides and legible part markings.
2. Manufacturer/model/revision and authoritative datasheet.
3. Schematic and BOM, including driver and protection components.
4. Connector pinout and logic-side electrical specification.
5. Load-side ratings and applicable load category.
6. Isolation architecture and evidence; clearly distinguish optocoupler presence from certified safety isolation.
7. Power budget and thermal limits.
8. Reset/unpowered/disconnected behavior truth table.
9. Feedback circuit schematic and signal specification.
10. Failure-mode analysis and safe-state recommendation.
11. Required enclosure, fuse, creepage/clearance and installation constraints.
12. Hardware sign-off identifying any conditions that prohibit firmware control.

If a detail cannot be verified, mark it unknown and identify the evidence required. Do not substitute generic module descriptions or marketplace claims for the exact installed hardware documentation.

## 9. Firmware adapter contract requirements (future, not implemented)

A future driver adapter design shall:
- expose logical channel identifiers, not raw GPIO numbers;
- validate channel capability and command range;
- return bounded, typed outcomes;
- have explicit initialization and unavailable/fault behavior;
- avoid blocking indefinitely;
- report request outcome separately from physical feedback;
- never grant output permission;
- default to unavailable/denied when hardware profile or configuration is invalid;
- preserve safe behavior on restart and configuration mismatch;
- emit bounded diagnostics without credentials or unnecessary identifiers.

Exact APIs, timing bounds and error codes are deferred until Hardware evidence and HQ-approved shared contracts exist.

## 10. Cross-department dependencies and risk register

| ID | Dependency / risk | Owner | Blocks |
|---|---|---|---|
| FW03D-DEP-01 | Exact relay driver identity, schematic and datasheet | Hardware | Relay adapter design |
| FW03D-DEP-02 | Input logic voltage/current, thresholds, polarity and unpowered state | Hardware | Electrical interface |
| FW03D-DEP-03 | Coil/contact ratings, load category and protection | Hardware | Relay/load suitability |
| FW03D-DEP-04 | Isolation, creepage/clearance and certification evidence | Hardware / HQ safety review | Any mains-related product design |
| FW03D-DEP-05 | Fan/motor class, ratings and approved control method | Hardware | Fan control architecture |
| FW03D-DEP-06 | Driver reset/brownout/disconnect truth table | Hardware | Safe-state design |
| FW03D-DEP-07 | Feedback circuit and diagnostic coverage | Hardware / Firmware | Physical state reporting |
| FW03D-DEP-08 | Board schematic and final GPIO/net mapping | Hardware | Pin-level adapter design |
| FW03D-DEP-09 | Approved command/authority/configuration contracts | HQ / Software | Application-to-driver integration |
| FW03D-DEP-10 | Separate implementation authorization and safety review | HQ | Any physical GPIO/actuator implementation |

## 11. FW-03D exit assessment

**Firmware planning completed:** logical actuator boundary and typed-result expectations documented; relay and fan electrical evidence requirements separated; feedback and failure-mode requirements recorded; all unknown electrical values explicitly left unknown; hardware evidence checklist and cross-department dependencies prepared.

**Pending:** Hardware Department must identify and document the actual relay/fan driver hardware (if selected), supply schematics/datasheets/electrical ratings and review the failure behavior. No driver hardware details were present in the evidence reviewed for this package.

FW-03D is a requirements document, not an approved circuit or pin assignment. FW-03E may proceed with generic feedback, fault-detection and safe-state requirements without selecting physical signals or implementing outputs.

---
**Prepared by:** HomeGrid Firmware Department  
**Review requested from:** HomeGrid Hardware Department  
**Safety/implementation authority:** HomeGrid HQ
