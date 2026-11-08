# FW-02A — Requirements and Interface Traceability

**Milestone:** FW-02 — Smart Switch Firmware Foundation  
**Work package:** FW-02A — Requirements and Interface Traceability  
**Authority:** HomeGrid HQ Executive Directive `HG-HQ-ED-FW02-001`  
**Decision:** `HG-HQ-DEC-20261108-FW02-001`  
**Directive date:** 2026-11-08  
**Status:** Department baseline prepared; pending verification and acceptance  
**Repository baseline:** `main` at the FW-01F-closed revision

## 1. Purpose and authority

This document converts the HQ-approved FW-02 scope into traceable firmware requirements and records which interfaces are approved, provisional, or blocked pending an authoritative owner decision. The HQ directive supersedes the earlier draft-only status in `docs/FW-02-work-package.md`.

FW-02 is authorized for implementation only within the explicit boundaries below. This package does not authorize physical actuation, radio/network integration, provisioning, OTA, production credentials, or changes to shared HomeGrid contracts.

## 2. Evidence and authority register

| Reference | Authority / evidence | Use in FW-02A | Status |
|---|---|---|---|
| `HG-HQ-ED-FW02-001` | HomeGrid HQ executive directive, 2026-11-08 | Authorizes FW-02A through FW-02F with restrictions | Approved |
| `HG-HQ-DEC-20261108-FW02-001` | HQ decision reference | Decision authority for FW-02 | Approved |
| `docs/FW-02-work-package.md` | Firmware Department proposed scope | Scope detail, exclusions and deliverables | Superseded as to approval status; scope constrained by HQ directive |
| `docs/FW-01-final-acceptance-report.md` | FW-01F acceptance record | Predecessor baseline and hardware follow-ups | Accepted predecessor |
| `AGENTS.md` | Repository engineering and safety instructions | Development constraints | Binding repository instruction |
| `DCX_Master_Context_Pack_v1.1.json` | Unrelated/non-authoritative context pack | Not a HomeGrid smart-switch protocol authority | Not applicable |
| `ENTERPRISE_SECURITY_AND_IP_STANDARD_v1.0.md` | Generic security/IP standard surfaced in project context | Applicability not confirmed by HQ | Pending HQ confirmation |

No external or generic document is promoted to a HomeGrid contract by inference. Where a required contract is absent, the interface remains abstract and implementation is blocked at the integration boundary.

## 3. Requirement traceability

Status meanings:
- **Approved** — explicit in the HQ directive or binding repository baseline.
- **Constrained** — permitted only within the stated restriction.
- **Pending** — requires an authoritative owner decision or evidence.
- **Deferred** — intentionally outside FW-02 implementation.

| ID | Requirement | Source / authority | Owner | Firmware allocation | Verification | Status |
|---|---|---|---|---|---|---|
| FW02-REQ-001 | FW-02 work shall follow the six-package sequence FW-02A through FW-02F. | HQ directive | Firmware Lead | Milestone control | Milestone records | Approved |
| FW02-REQ-002 | All FW-02 actuator behavior shall be host-test/mock-only. | HQ directive | Firmware | Domain and mock actuator | Host tests; backend-selection guard | Approved |
| FW02-REQ-003 | FW-02 shall not configure or drive physical GPIO outputs. | HQ directive | Firmware / Hardware | Platform boundary | Source/config audit | Approved |
| FW02-REQ-004 | No relay/fan actuation, mains-connected testing, or load testing is permitted. | HQ directive | HQ / Firmware | Excluded | Scope and test audit | Approved |
| FW02-REQ-005 | No Wi-Fi, BLE, MQTT, provisioning, or OTA implementation is permitted in FW-02. | HQ directive | HQ / Software / Firmware | Abstract ports only where useful | Dependency and source audit | Approved |
| FW02-REQ-006 | Shared HomeGrid contracts shall not be modified without HQ approval. | HQ directive; AGENTS.md | HQ | Contract adapters remain abstract | Diff and interface review | Approved |
| FW02-REQ-007 | Local-versus-remote command priority shall not be assumed. | HQ directive | HQ / Product / Software | Policy decision interface only | Requirements review | Pending |
| FW02-REQ-008 | Restart and power-loss state restoration policy shall not be assumed. | HQ directive | HQ / Product / Hardware / Firmware | Startup policy boundary | Requirements review | Pending |
| FW02-REQ-009 | Device identity format and lifecycle shall come from an authoritative HQ contract. | HQ dependency register | HQ / Architecture | Identity provider interface only | Interface contract review | Pending |
| FW02-REQ-010 | Provisioning and credential lifecycle shall come from HQ/security authority. | HQ dependency register | HQ / Security | No implementation; no secrets | Security review | Pending |
| FW02-REQ-011 | Command protocol, topic names, payloads, acknowledgement and ordering semantics shall be owned by Software/HQ. | HQ dependency register | Software / HQ | Abstract command port only | Contract review | Pending |
| FW02-REQ-012 | Diagnostic/telemetry export semantics shall be owned by Software/HQ. | HQ dependency register | Software / HQ | Local diagnostics only | Contract review | Pending |
| FW02-REQ-013 | Device trust, authentication, authorization and key handling shall follow approved security architecture. | HQ dependency register | HQ / Security | No security integration in FW-02 | Security review | Pending |
| FW02-REQ-014 | OTA authenticity, rollback and recovery requirements shall be approved by HQ before implementation. | HQ dependency register | HQ | Deferred | Separate approval gate | Deferred |
| FW02-REQ-015 | Configuration persistence, reset and factory-default semantics shall be approved before persistent behavior is implemented. | HQ dependency register | HQ / Firmware | Non-secret in-memory defaults only unless otherwise approved | Host tests and decision record | Pending |
| FW02-REQ-016 | Production firmware shall remain C using ESP-IDF and FreeRTOS; Python is limited to host tooling/tests. | AGENTS.md | Firmware | Application and platform layers | Build/source audit | Approved |
| FW02-REQ-017 | ESP-IDF v6.1 remains the pinned framework baseline; no silent toolchain change. | AGENTS.md / FW-01 baseline | Firmware | Build configuration | Build metadata review | Approved |
| FW02-REQ-018 | Preserve the verified physical flash record of 4 MB and existing 2 MB image configuration; do not enlarge or reinterpret it in FW-02. | FW-01F evidence / HQ baseline | Firmware | Build configuration | sdkconfig/partition audit | Constrained |
| FW02-REQ-019 | FW-02 shall not require PSRAM or assume its presence. | FW-01F unresolved hardware evidence | Firmware / Hardware | Memory design | Configuration/source audit | Approved |
| FW02-REQ-020 | Board/module identity details not verified in FW-01F shall remain unknown; no inferred GPIO, polarity or electrical assumptions. | FW-01F / AGENTS.md | Hardware / Firmware | Board abstraction only | Board-assumption audit | Approved |
| FW02-REQ-021 | Logical state shall distinguish desired, applied, observed, availability, fault and output-permission dimensions. | HQ-approved FW-02 scope and safety principles | Firmware | Switch domain model | State transition tests | Constrained |
| FW02-REQ-022 | Startup shall represent physical state as unknown and inhibit output; a requested state shall not be treated as proof of physical state. | HQ-approved FW-02 scope and safety principles | Firmware | State model / startup | Host tests | Approved |
| FW02-REQ-023 | Actuator interface shall use a mock backend only; mock requests and controlled outcomes shall be observable in tests. | HQ directive | Firmware | Mock actuator port | Host tests and build guard | Approved |
| FW02-REQ-024 | Diagnostics shall remain local/internal in FW-02 and shall not transmit externally. | HQ directive and scope restriction | Firmware | Diagnostics component | Source audit and tests | Approved |

## 4. Interface ownership and contract status

| Interface / concern | Intended owner | FW-02 treatment | Required input before integration |
|---|---|---|---|
| Device identity | HQ / Architecture | Abstract identity provider; no invented identifier format | Approved identity schema, lifecycle and uniqueness rules |
| Provisioning and credentials | HQ / Security | Excluded; no credential storage or provisioning code | Approved provisioning flow, trust model and secret-handling rules |
| Command transport and payload | Software Department / HQ | Abstract command input boundary only | Versioned command contract, validation, ordering, acknowledgement and error semantics |
| Local/remote authority | HQ / Product Architecture | No priority policy hard-coded | Explicit arbitration and conflict-resolution decision |
| State persistence and restart | HQ / Product / Hardware / Firmware | Startup unknown; no assumed restoration behavior | Power-loss/restart and desired-state retention decision |
| Actuator electrical interface | Hardware Department | Mock only; no GPIO backend or pin mapping | Exact board/module, driver schematic, isolation/protection, polarity, feedback and safety review |
| Diagnostics export | Software Department / HQ | Local diagnostics only | Versioned event schema, privacy/security constraints and transport ownership |
| Device security | HQ / Security | No radio/security integration | Approved device trust, authentication, authorization and key lifecycle architecture |
| OTA | HQ | Deferred | Signed image, rollback, recovery and release authorization policy |
| Persistent configuration | HQ / Firmware | In-memory non-secret defaults only | Schema/versioning, migration, reset and factory-default semantics |

## 5. Explicit non-assumptions

The following must not be inferred or hard-coded:
- Whether local or remote commands have priority, or how simultaneous/conflicting commands are arbitrated.
- Whether the device restores desired state, last applied state, or a safe default after reboot or power loss.
- Any device identifier format, provisioning method, credential lifecycle, command topic, payload schema, telemetry schema, or OTA protocol.
- Any relay GPIO number, output polarity, boot level, driver circuit, feedback pin, fan-control method, isolation rating, or load characteristics.
- Exact carrier-board PCB revision, module suffix, and PSRAM presence.
- Applicability of generic security/IP standards until HQ confirms them as HomeGrid policy.

The physical device state is unknown unless a separately approved and implemented feedback mechanism proves otherwise. A software request or mock result is not physical feedback.

## 6. Safety and implementation constraints for downstream packages

1. Pure domain logic should be host-buildable and independent of ESP-IDF, FreeRTOS, GPIO, radio/network and storage details wherever practical.
2. Application orchestration, switch domain, configuration, identity abstraction, diagnostics, communications abstraction, actuator abstraction and ESP-IDF platform services must have explicit boundaries.
3. The actuator abstraction must fail closed to the mock implementation. There must be no selectable real GPIO backend in FW-02.
4. Startup and fault states must inhibit output and retain unknown physical observation.
5. Keep task count minimal. Document task ownership, priority, stack, blocking/I/O, watchdog interaction and failure behavior before adding tasks.
6. Record heap metrics and task stack high-water marks where supported; compare with the FW-01 diagnostic baseline without treating the FW-01 heap reading as a universal acceptance threshold.
7. Do not disable, bypass or arbitrarily feed the watchdog.
8. Do not log credentials, tokens, keys or other secrets.
9. Preserve the existing 4 MB physical-flash / 2 MB image configuration and no-PSRAM dependency.
10. No system-wide installation, environment mutation, flash erase, privileged/destructive action or physical flashing without the required separate authorization.

## 7. Open dependency register

| Dependency ID | Decision / evidence required | Owner | Blocks |
|---|---|---|---|
| FW02-DEP-01 | Device identity format and lifecycle | HQ / Architecture | Identity integration |
| FW02-DEP-02 | Provisioning and credential lifecycle | HQ / Security | Provisioning/security integration |
| FW02-DEP-03 | Command protocol and transport contract | Software / HQ | Communications integration |
| FW02-DEP-04 | Diagnostic/telemetry export contract | Software / HQ | External diagnostics |
| FW02-DEP-05 | Device trust and security architecture | HQ / Security | Radio/network and credential integration |
| FW02-DEP-06 | OTA authenticity, rollback and recovery policy | HQ | OTA |
| FW02-DEP-07 | Local-versus-remote command authority | HQ / Product | Arbitration policy |
| FW02-DEP-08 | Power-loss/restart state restoration policy | HQ / Hardware / Firmware | Persistence and startup policy |
| FW02-DEP-09 | Configuration persistence/reset semantics | HQ / Firmware | Persistent configuration |
| FW02-DEP-10 | Exact carrier-board/module identity and electrical design | Hardware | Board-specific integration and any future actuation review |
| FW02-DEP-11 | Applicability of generic security/IP standard | HQ | Compliance interpretation |

These dependencies do not prevent FW-02A traceability or a board-independent, mock-only FW-02B architecture. They do prevent implementation of the corresponding integrations and policies.

## 8. FW-02A exit checklist

- [x] HQ authorization and scope restrictions recorded.
- [x] Existing repository and FW-01F baseline identified.
- [x] Requirements assigned stable IDs, owners, components and verification methods.
- [x] Approved, constrained, pending and deferred items distinguished.
- [x] Shared interface ownership and missing contracts documented.
- [x] Non-assumptions and safety constraints made explicit.
- [x] Open dependencies assigned to authoritative owners.
- [ ] Repository change committed and reread from GitHub.
- [ ] Firmware Department acceptance recorded after repository verification.

## 9. FW-02B entry recommendation

Proceed with a board-independent architecture and explicit state model using pure C domain logic and mock-only ports. Keep unresolved interfaces as narrow abstractions and document their contract gaps. Do not implement real communications, persistent identity/credentials, GPIO, physical actuation, or restart restoration policy. The architecture package should include component responsibilities, dependency direction, state ownership, error propagation, task model, build/test boundaries and a clear mock-only backend enforcement mechanism.

---
**Prepared by:** HomeGrid Firmware Department  
**Document control:** FW-02A working baseline; repository commit and acceptance status must be recorded separately.
