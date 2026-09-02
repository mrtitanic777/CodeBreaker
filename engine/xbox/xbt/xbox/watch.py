"""Live memory watch — real-time polling of console memory."""

import time
import struct
from typing import Optional


def watch_memory(probe, address: int, size: int = 4, interval_ms: int = 500,
                 duration: int = 0, callback=None) -> list:
    """Poll a memory address at regular intervals and track changes.
    
    Args:
        probe: ConsoleLink instance
        address: Memory address to watch
        size: Bytes to read (4 = DWORD, 1-64)
        interval_ms: Polling interval in milliseconds
        duration: Max seconds to run (0 = until interrupted)
        callback: Function called with (timestamp, data) on each read
    
    Returns: List of (timestamp, data) tuples
    """
    history = []
    last_data = None
    start = time.time()
    count = 0
    changes = 0

    try:
        while True:
            elapsed = time.time() - start
            if duration > 0 and elapsed >= duration:
                break

            data = probe.read_memory(address, size)
            if data is None:
                if callback:
                    callback(elapsed, None, count, changes, False)
                time.sleep(interval_ms / 1000.0)
                count += 1
                continue

            changed = (data != last_data)
            if changed and last_data is not None:
                changes += 1

            history.append((elapsed, data))
            last_data = data
            count += 1

            if callback:
                callback(elapsed, data, count, changes, changed)

            time.sleep(interval_ms / 1000.0)

    except KeyboardInterrupt:
        pass

    return history


def format_watch_value(data: bytes, address: int) -> str:
    """Format watched data as hex + integer interpretations."""
    hex_str = " ".join(f"{b:02X}" for b in data)
    
    parts = [hex_str]
    if len(data) == 4:
        val_be = struct.unpack(">I", data)[0]
        val_le = struct.unpack("<I", data)[0]
        parts.append(f"BE={val_be} (0x{val_be:08X})")
    elif len(data) == 2:
        val_be = struct.unpack(">H", data)[0]
        parts.append(f"BE={val_be} (0x{val_be:04X})")
    elif len(data) == 1:
        parts.append(f"={data[0]} (0x{data[0]:02X})")

    return "  ".join(parts)
