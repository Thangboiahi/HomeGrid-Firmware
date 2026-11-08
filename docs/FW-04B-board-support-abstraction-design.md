# FW-04B — Board Support Abstraction Design

**Authority:** `HG-HQ-ED-FW04-001` / `HG-HQ-DEC-20261108-FW04-001`  
**Status:** Design specification; no hardware implementation  
**Predecessor:** FW-04A board capability profile

## 1. Purpose

Define a board-support boundary that allows the HomeGrid application/domain to use logical device capabilities without knowing carrier-specific GPIO numbers, electrical polarity, peripheral routing or component identities.

The abstraction must work with mock adapters and incomplete board evidence. It must fail closed when the board profile is absent, invalid, unsupported or inconsistent.

## 2. Design principles

1. Application/domain layers consume logical capabilities, never raw GPIO identifiers.
2. Board-specific configuration belongs exclusively to the board-support implementation/profile.
3. A board profile is evidence-backed, versioned and validated before use.
4. Missing or unknown capabilities remain unavailable; they are not guessed.
5. Initialization reports typed outcomes and does not imply an output has been enabled.
6. Capability discovery is descriptive, not an authorization grant.
7. Output permission remains permanently DENIED under the current FW-02 domain.
8. Hardware adapters are not part of this design package and are not authorized by it.

## 3. Proposed logical ownership and dependency direction

`Application / Domain`
- Calls logical capability interfaces.
- Owns desired state, command policy and application-level orchestration.
- Does not include ESP-IDF GPIO headers or board-specific pin tables.

`Board Support API`
- Exposes board identity, profile version, supported logical capabilities and initialization result.
- Validates a selected board profile and maps logical resources to adapter-owned configuration.
- Does not decide command authority or grant output permission.

`Board Profile`
- Contains board identifier, profile revision, evidence references, capability declarations and configuration validity.
- Physical pin/net mapping is represented only after Hardware verifies it.
- Unknown fields are explicit unknown/unavailable values, not default GPIO numbers.

`Hardware Adapter` (future; not implemented here)
- Owns ESP-IDF and board-specific peripheral details.
- Implements logical switch/relay/fan/feedback ports only after separate authorization and hardware review.
- Returns typed operation outcomes; does not claim physical observation without independent feedback.

`Mock Adapter`
- Implements the same logical API for deterministic host tests.
- Must not include GPIO, ESP-IDF, network, storage or real actuator dependencies.

Dependency rule: Application/Domain -> logical ports -> adapter implementation. Board profile and platform details must not leak upward into domain/application logic.

## 4. Proposed board profile data model

| Field | Meaning | Validation |
|---|---|---|
| profile_schema_version | Format version | Required; supported version only |
| board_id | Stable logical board identifier | Required; non-empty and known |
| board_label | Human-readable label | Informational; never used as electrical proof |
| carrier_revision | Exact carrier revision | Explicit unknown until Hardware verifies |
| module_order_code | Exact module variant | Explicit unknown until verified |
| soc_target | ESP-IDF target | Must match build target |
| silicon_identity | SoC/revision evidence | Evidence-linked; no inference from label |
| flash_bytes | Verified physical flash capacity | Must not silently alter image configuration |
| psram_bytes | Verified PSRAM capacity | Unknown is distinct from zero/unavailable |
| image_flash_config | Current firmware flash view | Preserve existing 2 MB configuration |
| capabilities | Logical capability declarations | Each capability has evidence and availability |
| resource_map | Hardware-verified mapping references | Not populated with speculative pins |
| evidence_refs | Source document/report identifiers | Required for hardware claims |
| validation_state | VALID / INCOMPLETE / INVALID | Incomplete/invalid profiles fail closed |
| profile_revision | Monotonic profile revision | Changes require review |

This is a design model, not an approved shared HomeGrid contract or serialized protocol schema.

## 5. Initialization lifecycle and result model

Proposed lifecycle:
1. Load the selected immutable board profile.
2. Validate schema and profile revision.
3. Compare declared SoC target with the compiled target.
4. Validate required identity/capability evidence.
5. Validate all resources required by the requested logical capability.
6. Report initialized capability set and typed status.
7. Leave any unsupported or unverified capability unavailable.

Proposed result classes:
- `HG_BOARD_INIT_OK`: profile validated; only means board-support metadata initialized.
- `HG_BOARD_INIT_INCOMPLETE`: required evidence/capability is unknown.
- `HG_BOARD_INIT_INVALID_PROFILE`: malformed, unsupported or inconsistent profile.
- `HG_BOARD_INIT_TARGET_MISMATCH`: compiled target and profile disagree.
- `HG_BOARD_INIT_RESOURCE_UNAVAILABLE`: requested logical capability is not supported.
- `HG_BOARD_INIT_INTERNAL_ERROR`: internal adapter/validation failure.

A successful board-profile initialization must not alter output permission, desired state, applied state or observed state.

## 6. Capability reporting

Each logical capability should report:
- capability identifier;
- supported / unsupported / unknown status;
- evidence reference and profile revision;
- adapter availability;
- initialization status.

Capability reporting must not expose unverified electrical ratings as facts. It must not indicate that a relay, fan, switch or sensor is physically connected unless Hardware evidence confirms it.

## 7. Failure behavior

- No profile: return incomplete/invalid; expose no board-specific resource.
- Unknown pin mapping: resource unavailable.
- Unsupported target: fail initialization.
- Conflicting profile evidence: mark invalid and require Hardware reconciliation.
- Adapter initialization failure: typed failure; no retry loop or output activation is implied.
- Runtime adapter error: report typed error and fault through existing application/domain boundaries.
- Any failure: output permission remains DENIED.

## 8. Configuration ownership

Firmware owns profile schema validation and mapping of logical resources to adapter interfaces. Hardware owns evidence for physical board identity, nets, electrical ratings, reset behavior and protection. Software/HQ own approval of any shared identity/configuration protocol. The board profile in this package is an internal firmware design artifact only.

No secrets, network credentials or remotely mutable GPIO maps are permitted. Remote configuration must never be allowed to create arbitrary GPIO assignments.

## 9. Testability requirements

Host tests for a future implementation must cover:
- valid known profile;
- missing profile;
- unsupported schema version;
- target mismatch;
- unknown carrier/module identity;
- unavailable capability;
- incomplete evidence;
- duplicate/conflicting resource mapping;
- initialization error propagation;
- successful metadata initialization leaves output permission DENIED;
- profile/capability reporting does not change desired/applied/observed state.

## 10. Explicit non-goals and safety

This design does not:
- implement ESP-IDF GPIO/peripheral code;
- assign or configure physical pins;
- create a real board profile with guessed net mapping;
- enable any output;
- operate relay/fan/pump/appliance hardware;
- change FW-02 safety logic;
- modify shared HomeGrid contracts;
- alter ESP-IDF v6.1, target `esp32`, partition table or 2 MB application image configuration.

## 11. Review dependencies

Hardware review required for the profile fields, evidence model and ownership of physical resource mapping. Software review required for future device identity/configuration integration. HQ approval is required before any shared contract or policy changes.

**FW-04B conclusion:** Board-support ownership, API boundaries, profile model, initialization semantics and failure behavior are specified. No hardware-specific implementation is included. This design is suitable for FW-04C logical driver interface design.

---
**Prepared by:** HomeGrid Firmware Department
