"""Capture raw COM8 serial bytes; start the 10-minute window at app startup."""

import time
from datetime import datetime, timezone
from pathlib import Path

import serial


PORT = "COM8"
BAUD = 115200
DURATION_SECONDS = 600
START_MARKER = b"HomeGrid FW-01 diagnostic firmware starting"
LOG_PATH = Path(__file__).with_name("FW-01E-serial.log")
TIME_PATH = Path(__file__).with_name("FW-01E-serial-timestamps.csv")


def main() -> None:
    ser = serial.Serial(port=None, baudrate=BAUD, timeout=1)
    ser.port = PORT
    ser.dtr = False
    ser.rts = False
    ser.open()
    print(f"CAPTURE_ARMED port={PORT} baud={BAUD}; press the board EN/RESET button now", flush=True)

    started = None
    marker_window = b""
    offset = 0
    last_progress = time.monotonic()
    with LOG_PATH.open("wb") as raw, TIME_PATH.open("w", encoding="utf-8", newline="") as stamps:
        stamps.write("utc,monotonic_seconds,offset,length\n")
        while True:
            chunk = ser.read(ser.in_waiting or 1)
            if chunk:
                now = time.monotonic()
                utc = datetime.now(timezone.utc).isoformat()
                raw.write(chunk)
                raw.flush()
                stamps.write(f"{utc},{now:.3f},{offset},{len(chunk)}\n")
                stamps.flush()
                offset += len(chunk)
                if started is None:
                    scan = marker_window + chunk
                    if START_MARKER in scan:
                        started = now
                        print("APP_START_SEEN; beginning 600-second runtime window", flush=True)
                    marker_window = scan[-len(START_MARKER) :]
                if started is not None and now - started >= DURATION_SECONDS:
                    break
            now = time.monotonic()
            if started is None and now - last_progress >= 10:
                print("WAITING_FOR_APP_START", flush=True)
                last_progress = now
            elif started is not None and now - last_progress >= 10:
                print(f"RUNTIME_SECONDS={int(now-started)} BYTES={offset}", flush=True)
                last_progress = now
    ser.close()
    if started is None:
        print("CAPTURE_ENDED_WITHOUT_APP_START", flush=True)
        raise SystemExit(2)
    print(f"CAPTURE_COMPLETE runtime_seconds={int(time.monotonic()-started)} bytes={offset}", flush=True)


if __name__ == "__main__":
    main()
