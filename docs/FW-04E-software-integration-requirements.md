# FW-04E — Software Integration Requirements

**Authority:** `HG-HQ-ED-FW04-001` / `HG-HQ-DEC-20261108-FW04-001`  
**Coordination:** HomeGrid Software Department  
**Status:** Firmware-facing requirements and proposed mappings only; no shared contract implementation

## 1. Coordination evidence and limitation

HQ directed Software to coordinate with Firmware on device identity, command/acknowledgement, desired/reported state, configuration, diagnostics and telemetry. The accessible `Thangboiahi/HomeGrid-Software` repository returned GitHub's `Git Repository is empty` response when its main tree was inspected. No Software-owned schema, API specification, protocol definition or review comments were therefore available to reconcile.

This package records Firmware requirements and proposals only. It does not claim Software Department approval or establish authoritative HomeGrid shared contracts.

## 2. Device identity requirements

Firmware needs a stable logical device identity independent of mutable network addresses and human-readable labels. Proposed information categories for joint review:
- immutable device identifier assigned through an HQ-approved registration process;
- hardware/board profile identifier and profile revision;
- firmware name, semantic version and build identifier;
- hardware capability summary with evidence/qualification state;
- registration/identity validity state.

Do not use MAC address, serial number, board silkscreen or user-provided label alone as a globally authoritative identity without HQ policy. No identifier format, provisioning flow or registration endpoint is defined here.

## 3. Command and acknowledgement requirements

Firmware-facing command envelope requirements for Software/HQ review:
- unique command identifier for deduplication/correlation;
- target device and logical capability/channel;
- logical operation and validated arguments;
- issuer/authority context;
- issue time and expiry semantics;
- idempotency/replay protection metadata;
- schema/protocol version;
- acknowledgement status and diagnostic reason.

Firmware must distinguish received, syntactically valid, policy-authorized, accepted by application, submitted to adapter, adapter result and independently observed outcome. These are separate stages; an acknowledgement must not falsely imply physical actuation.

Local/remote arbitration, command authority, authorization, expiry policy and conflict resolution are HQ-retained decisions. No command format or topic/path is finalized.

## 4. Desired/reported state requirements

Proposed conceptual separation:
- desired state: current application intent;
- adapter-applied state: request outcome at the adapter boundary;
- reported/observed state: independent valid physical feedback;
- availability: capability readiness;
- fault: current diagnostic/fault status;
- output permission: independent safety authorization.

Transport acknowledgements, cached desired state and adapter acceptance must not be represented as physical reported state. Missing, stale, invalid or contradictory feedback must not be normalized to OFF. State timestamps and freshness thresholds require an approved time model and Software/HQ contract.

## 5. Configuration requirements

Firmware requires versioned, validated configuration with:
- explicit schema version;
- device/profile compatibility;
- typed values and bounded ranges;
- atomic apply/rollback semantics;
- provenance and update status;
- validation diagnostics;
- safe behavior for missing, malformed, stale or unsupported configuration.

Configuration must not permit remote arbitrary GPIO selection, driver polarity selection or bypass of firmware safety gates. Hardware resource mapping remains firmware/Hardware-controlled and evidence-qualified. Reset/persistence behavior and remote configuration authority require HQ decisions.

## 6. Diagnostics and telemetry requirements

Proposed diagnostic event fields for joint review:
- event type and severity/category;
- device identity reference;
- firmware/build/profile revision;
- sequence/correlation identifier;
- monotonic sequence or approved timestamp;
- logical capability/channel;
- typed result/fault code;
- relevant state revision and feedback validity;
- bounded, privacy-safe context.

Telemetry should distinguish software request, adapter outcome and independently observed feedback. It must avoid secrets, credentials, personal data and unnecessary raw hardware detail. Retention, sampling, delivery, privacy and export policy require Software/HQ approval.

The existing FW-02 diagnostics component is bounded in-memory and has no transport, clock, persistence or telemetry export. This package does not add those capabilities.

## 7. Firmware/Software ownership proposal

| Concern | Firmware responsibility | Software responsibility | HQ approval |
|---|---|---|---|
| Device identity | expose verified firmware/profile facts | registration and service identity contract | identity authority/assignment |
| Commands | validate envelope and execute approved policy | delivery and command service contract | authority/arbitration/expiry |
| Acknowledgement | report precise processing stage | external acknowledgement semantics | authoritative state meaning |
| Desired/reported state | maintain independent local state dimensions | represent/synchronize service state | reconciliation policy |
| Configuration | validate supported local config | distribute/store approved config | schema and update authority |
| Diagnostics | emit bounded typed events | ingestion, storage and visualization | privacy/retention policy |

This table is a proposed ownership model for review, not a signed interface agreement.

## 8. Required Software/HQ review questions

1. What is the authoritative device ID and registration lifecycle?
2. What command schema/version, expiry, idempotency and acknowledgement stages are required?
3. How are desired, adapter-applied and physically reported states represented distinctly?
4. What is the source of time and how are stale commands/feedback identified?
5. What configuration is remotely mutable, and what is immutable Hardware/firmware profile data?
6. What diagnostics fields, severity taxonomy, privacy rules and retention are approved?
7. What is the command authority and local/remote arbitration policy?
8. What happens to desired state, queued commands and reported state after reboot/power restoration?

## 9. Non-goals and restrictions

No API endpoint, MQTT topic, JSON schema, protobuf definition, shared header, generated client, backend code or production communication integration is created. No shared contract is changed. No Wi-Fi, BLE, MQTT, HTTP, storage, provisioning or OTA implementation is added. No physical GPIO/actuator behavior is authorized.

## 10. FW-04E exit assessment

| Criterion | Status |
|---|---|
| Firmware-facing identity requirements documented | PASS |
| Command/acknowledgement requirements documented | PASS |
| Desired/reported state requirements documented | PASS |
| Configuration requirements documented | PASS |
| Diagnostics/telemetry requirements documented | PASS |
| Existing Software contracts inspected | BLOCKED — Software repository empty |
| Software review received | PENDING |
| HQ shared-contract approval | PENDING |

**FW-04E conclusion:** Firmware-side integration requirements and open questions are documented. Software/HQ review remains an explicit dependency. No authoritative shared contract has been invented or changed.

---
**Prepared by:** HomeGrid Firmware Department  
**Coordination owner:** HomeGrid Software Department
