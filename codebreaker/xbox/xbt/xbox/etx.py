"""ETX parser — decode XAM's undocumented telemetry/event ring buffer."""

import struct
from typing import Optional


ETX_MAGIC = b"ETX\x00"
ETX_BASE = 0x3A000000


def parse_etx_header(data: bytes) -> dict:
    """Parse the ETX header structure."""
    if len(data) < 0x30 or data[0:4] != ETX_MAGIC:
        return {"error": "Not an ETX buffer", "magic": data[0:4].hex().upper()}
    
    header = {
        "magic": "ETX",
        "flags": struct.unpack('>I', data[4:8])[0],
        "section_count": struct.unpack('>I', data[8:12])[0],
        "reserved": struct.unpack('>I', data[12:16])[0],
        "subsection_count": struct.unpack('>I', data[16:20])[0],
        "reserved2": struct.unpack('>I', data[20:24])[0],
        "section1_ptr": struct.unpack('>I', data[24:28])[0],
        "section2_ptr": struct.unpack('>I', data[28:32])[0],
        "section1_size": struct.unpack('>I', data[32:36])[0],
        "section2_size": struct.unpack('>I', data[36:40])[0],
    }
    return header


def parse_etx_events(data: bytes, base: int = ETX_BASE) -> list:
    """Extract event records from ETX buffer.
    
    Events appear to be variable-length records with:
    - Event type/ID
    - Timestamp or sequence number
    - String fields (error names, tokens)
    - Binary data
    """
    events = []
    
    # Scan for repeated event patterns
    # The pattern "LogonFail_Other" appears every ~0x9F bytes
    # Each event record starts with a structure header
    
    marker = b"LogonFail_Other"
    offset = 0
    
    while True:
        idx = data.find(marker, offset)
        if idx == -1:
            break
        
        # Back up to find the start of this event record
        # Look for the event header (likely starts ~0x1C bytes before the string)
        event_start = max(0, idx - 0x20)
        
        # Extract fields around the marker
        event = {
            "offset": event_start,
            "address": base + event_start,
            "logon_status": "LogonFail_Other",
        }
        
        # Look for the machine token (QAUPYWCTR4852 pattern)
        token_search = data[idx:idx+0x80]
        for token_offset in range(len(token_search) - 14):
            candidate = token_search[token_offset:token_offset+14]
            if all(0x30 <= b <= 0x5A for b in candidate):
                event["machine_token"] = candidate.decode('ascii')
                break
        
        # Look for "PROD" environment marker
        prod_idx = data.find(b"PROD", idx, idx + 0x100)
        if prod_idx >= 0:
            event["environment"] = "PROD"
        
        # Look for "event" type string
        evt_idx = data.find(b"event", idx, idx + 0x100)
        if evt_idx >= 0:
            evt_end = evt_idx + 5
            while evt_end < len(data) and 0x30 <= data[evt_end] <= 0x39:
                evt_end += 1
            event["event_type"] = data[evt_idx:evt_end].decode('ascii')
        
        # Extract any 4-byte values that look like timestamps
        if event_start + 0x10 <= len(data):
            ts_candidate = struct.unpack('>I', data[event_start+0xC:event_start+0x10])[0]
            if 0x40000000 <= ts_candidate <= 0x70000000:
                event["possible_timestamp"] = ts_candidate
        
        events.append(event)
        offset = idx + len(marker)
    
    return events


def extract_etx_strings(data: bytes, base: int = ETX_BASE) -> list:
    """Extract all readable strings from ETX buffer."""
    strings = []
    current = []
    start = 0
    
    for i, b in enumerate(data):
        if 0x20 <= b <= 0x7E:
            if not current:
                start = i
            current.append(chr(b))
        else:
            if len(current) >= 4:
                strings.append((base + start, ''.join(current)))
            current = []
    
    if len(current) >= 4:
        strings.append((base + start, ''.join(current)))
    
    return strings


def analyze_etx_ring_buffer(events: list) -> dict:
    """Analyze the ETX ring buffer to determine event frequency and patterns."""
    stats = {
        "total_events": len(events),
        "unique_tokens": len(set(e.get("machine_token", "") for e in events if "machine_token" in e)),
        "environments": list(set(e.get("environment", "?") for e in events)),
        "event_types": list(set(e.get("event_type", "?") for e in events)),
    }
    
    if len(events) >= 2:
        # Calculate spacing between events
        offsets = [e["offset"] for e in events]
        spacings = [offsets[i+1] - offsets[i] for i in range(len(offsets)-1)]
        stats["avg_event_spacing"] = sum(spacings) // len(spacings)
        stats["min_spacing"] = min(spacings)
        stats["max_spacing"] = max(spacings)
    
    return stats


DESCRIPTION = "decode an Xbox 360 ETX trace ring buffer (header, events, strings)"


def main():
    import sys
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage: etx <etx-dump>"); return
    data = open(a[0], "rb").read()
    for k, v in parse_etx_header(data).items():
        print("  %-18s %s" % (k, v))
    ev = parse_etx_events(data)
    print("  events: %d" % len(ev))
    for k, v in analyze_etx_ring_buffer(ev).items():
        print("  %-18s %s" % (k, v))


if __name__ == "__main__":
    main()
