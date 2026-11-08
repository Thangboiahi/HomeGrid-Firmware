# FW-02 host-native tests

The C test harness compiles the same pure-C domain source used by the ESP-IDF component. It does not require an ESP32 board or ESP-IDF runtime.

From the repository root, using a C11 compiler:

```sh
cc -std=c11 -Wall -Wextra -Werror -pedantic \
  -I components/homegrid_domain/include \
  components/homegrid_domain/switch_state.c \
  tests/host/test_switch_state.c \
  -o test_switch_state
./test_switch_state
```

On Windows, run the equivalent command in a shell with a supported C compiler available. Compiler installation or system PATH changes are not part of this work package.

The tests exercise startup invariants, explicit command policy handling, no-mutation behavior for unresolved/denied policy, desired-state updates, mock outcomes, fault/availability changes and the permanent output-denied/observed-unknown restrictions.

These tests are separate from the existing Python FW-01 scaffold tests. No test execution is claimed until actual compiler output is captured.
