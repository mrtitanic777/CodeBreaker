"""Live console interaction — send commands, receive responses."""

import struct
import socket
import time
from typing import Optional

# Command types (must match xHTTP v9 main.cpp)
CMD_READ_MEM    = 0x10
CMD_WRITE_MEM   = 0x11
CMD_CALL_ORD    = 0x12
CMD_EXEC_PROBE  = 0x13
CMD_PING        = 0x14
CMD_DUMP_RANGE  = 0x15
CMD_PEEK_DWORD  = 0x16
CMD_HV_READ     = 0x17
CMD_TRIGGER_AUTH = 0x18

# Response types
RSP_DATA  = 0x20
RSP_ACK   = 0x21
RSP_ERROR = 0x22


class ConsoleLink:
    """Direct command interface to a running xHTTP v8 probe on the console."""

    def __init__(self, console_ip: str, port: int = 27020):
        self.console_ip = console_ip
        self.port = port
        self.sock: Optional[socket.socket] = None
        self.timeout = 3.0

    def connect(self) -> bool:
        """Create UDP socket for sending commands."""
        try:
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            self.sock.settimeout(self.timeout)
            return True
        except Exception as e:
            print(f"Failed to create socket: {e}")
            return False

    def disconnect(self):
        if self.sock:
            self.sock.close()
            self.sock = None

    def _send_recv(self, data: bytes, expect_size: int = 2048) -> Optional[bytes]:
        """Send a packet and wait for response."""
        if not self.sock:
            return None
        try:
            self.sock.sendto(data, (self.console_ip, self.port))
            resp, addr = self.sock.recvfrom(expect_size)
            return resp
        except socket.timeout:
            return None
        except Exception as e:
            print(f"Send/recv error: {e}")
            return None

    def ping(self) -> bool:
        """Ping the console probe. Returns True if it responds."""
        resp = self._send_recv(bytes([CMD_PING]))
        if resp and len(resp) >= 2 and resp[0] == RSP_ACK:
            return True
        return False

    def read_memory(self, address: int, size: int) -> Optional[bytes]:
        """Read memory from the console. Returns bytes or None on error."""
        if size > 1024:
            # Chain multiple reads
            result = b""
            while size > 0:
                chunk_size = min(size, 1024)
                chunk = self._read_chunk(address, chunk_size)
                if chunk is None:
                    return None
                result += chunk
                address += chunk_size
                size -= chunk_size
            return result
        return self._read_chunk(address, size)

    def _read_chunk(self, address: int, size: int) -> Optional[bytes]:
        """Read a single chunk (max 1024 bytes)."""
        pkt = bytes([CMD_READ_MEM]) + struct.pack(">II", address, size)
        resp = self._send_recv(pkt)
        if resp and len(resp) >= 5 and resp[0] == RSP_DATA:
            return resp[5:]  # Skip RSP_DATA + 4 addr bytes
        return None

    def write_memory(self, address: int, data: bytes) -> bool:
        """Write data to console memory. Returns True on success."""
        pkt = bytes([CMD_WRITE_MEM]) + struct.pack(">I", address) + data
        resp = self._send_recv(pkt)
        return resp is not None and len(resp) >= 2 and resp[0] == RSP_ACK

    def peek_dword(self, address: int) -> Optional[int]:
        """Read a single DWORD (4 bytes, big-endian) from console."""
        pkt = bytes([CMD_PEEK_DWORD]) + struct.pack(">I", address)
        resp = self._send_recv(pkt)
        if resp and len(resp) >= 9 and resp[0] == RSP_DATA:
            return struct.unpack(">I", resp[5:9])[0]
        return None

    def resolve_ordinal(self, module: int, ordinal: int) -> Optional[dict]:
        """Resolve a kernel/XAM ordinal on the live console.
        
        module: 0 = xboxkrnl.exe, 1 = xam.xex
        Returns dict with 'address' and 'first_instruction' or None.
        """
        pkt = bytes([CMD_CALL_ORD, module]) + struct.pack(">H", ordinal)
        resp = self._send_recv(pkt)
        if resp and len(resp) >= 12 and resp[0] == RSP_DATA:
            addr = struct.unpack(">I", resp[4:8])[0]
            instr = struct.unpack(">I", resp[8:12])[0]
            return {"address": addr, "first_instruction": instr}
        return None

    def trigger_auth(self) -> Optional[int]:
        """Send CMD_TRIGGER_AUTH to make xHTTP call XamUserLogon.
        Returns the HRESULT from the call, or None on failure."""
        pkt = bytes([CMD_TRIGGER_AUTH])
        resp = self._send_recv(pkt)
        if resp and len(resp) >= 6 and resp[0] == RSP_DATA:
            result = struct.unpack(">I", resp[2:6])[0]
            return result
        return None

    def dump_range(self, address: int, size: int, callback=None) -> bytes:
        """Dump a large memory range in 1024-byte chunks with progress."""
        result = bytearray()
        offset = 0
        while offset < size:
            chunk_size = min(1024, size - offset)
            chunk = self._read_chunk(address + offset, chunk_size)
            if chunk is None:
                # Fill with 0xFF for unreadable regions
                result.extend(b"\xFF" * chunk_size)
            else:
                result.extend(chunk)
            offset += chunk_size
            if callback:
                callback(offset, size)
        return bytes(result)