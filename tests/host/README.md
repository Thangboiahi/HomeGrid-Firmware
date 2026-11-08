# FW-02 host-native tests

The C host harness compiles the same pure-C domain, mock adapter and application sources used by the ESP-IDF components. It does not require an ESP32 board or ESP-IDF runtime.

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
  components/homegrid_domain/switch_state.c \
  components/homegrid_mock/mock_actuator.c \
  components/homegrid_application/application.c \
  tests/host/test_fw02d_application.c \
  -o test_fw02d_application
./test_fw02d_application
```

On Windows, run the equivalent commands in a shell with a supported C compiler available. Compiler installation or system PATH changes are not part of this work package.

FW-02D tests cover safe initialization, unresolved/denied policy blocking, deterministic mock outcomes, request recording, repeated requests, error handling and the permanent output-denied/observed-unknown restrictions.

These tests are separate from the existing Python FW-01 scaffold tests. The GitHub Actions workflow runs both C suites and the Python scaffold suite.
