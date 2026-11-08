# FW-02 host-native tests

The C host harness compiles the same pure-C domain, mock adapter, diagnostics and application sources used by the ESP-IDF components. It does not require an ESP32 board or ESP-IDF runtime.

## FW-02C domain tests

From the repository root, using a C11 compiler:

```sh
cc -std=c11 -Wall -Wextra -Werror -pedantic \
  -I components/homegrid_domain/include \
  components/homegrid_domain/switch_state.c \
  tests/host/test_switch_state.c \
  -o test_switch_state
./test_switch_state
```

## FW-02D application and mock tests

```sh
cc -std=c11 -Wall -Wextra -Werror -pedantic \
  -I components/homegrid_domain/include \
  -I components/homegrid_mock/include \
  -I components/homegrid_application/include \
  -I components/homegrid_diagnostics/include \
  components/homegrid_domain/switch_state.c \
  components/homegrid_mock/mock_actuator.c \
  components/homegrid_diagnostics/diagnostics.c \
  components/homegrid_application/application.c \
  tests/host/test_fw02d_application.c \
  -o test_fw02d_application
./test_fw02d_application
```

## FW-02E diagnostics tests

```sh
cc -std=c11 -Wall -Wextra -Werror -pedantic \
  -I components/homegrid_domain/include \
  -I components/homegrid_diagnostics/include \
  components/homegrid_diagnostics/diagnostics.c \
  tests/host/test_fw02e_diagnostics.c \
  -o test_fw02e_diagnostics
./test_fw02e_diagnostics
```

On Windows, run the equivalent commands in a shell with a supported C compiler available. Compiler installation or system PATH changes are not part of this work package.

FW-02D tests cover safe initialization, unresolved/denied policy blocking, deterministic mock outcomes, request recording, repeated requests and permanent output-denied/observed-unknown restrictions.

FW-02E tests cover bounded event retention, chronological reads, event counters, sticky last-fault reporting, invalid-input rejection and ring overwrite accounting.

These tests are separate from the existing Python FW-01 scaffold tests. The GitHub Actions workflow runs all three C suites and the Python scaffold suite.


## FW-04D mock IO and feedback tests

The FW-04D host-only mock covers logical switch/relay/fan request recording and feedback validity/freshness. It has no GPIO or ESP-IDF dependency.

```sh
cc -std=c11 -Wall -Wextra -Werror -pedantic \
  -I components/homegrid_mock/include \
  components/homegrid_mock/mock_io.c \
  tests/host/test_fw04d_mock_io.c \
  -o test_fw04d_mock_io
./test_fw04d_mock_io
```

The tests verify safe unavailable/unconfigured defaults, invalid configuration rejection, deterministic result injection, request recording without physical-state claims, and UNKNOWN observation for missing, stale, invalid or contradictory feedback.
