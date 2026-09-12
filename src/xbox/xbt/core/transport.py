"""Bidirectional UDP transport to Xbox 360 console probe."""

import socket
import struct
import time
import threading
from typing import Optional, Callable

# Protocol constants (must match xProbe.h)
XPROBE_VERSION = 1
XPROBE_MAX_PAYLOAD = 1024

# Packet types — Console to PC
PKT_HELLO       = 1
PKT_LOG         = 2
PKT_DATA        = 3
PKT_BEGIN_GROUP = 4
PKT_END_GROUP   = 5
PKT_GOODBYE     = 6

# Packet types — PC to Console (new for interactive mode)
CMD_READ_MEM    = 0x10
CMD_WRITE_MEM   = 0x11
CMD_CALL_ORD    = 0x12
CMD_EXEC_PROBE  = 0x13
CMD_PING        = 0x14
CMD_TRIGGER_AUTH = 0x18

class ConsoleTransport:
    """Manages UDP communication with the xHTTP probe on the console."""

    def __init__(self, console_ip: str, console_port: int = 27020, listen_port: int = 27020):
        self.console_ip = console_ip
        self.console_port = console_port
        self.listen_port = listen_port
        self.sock: Optional[socket.socket] = None
        self.connected = False
        self.sequence = 0
        self._listener_thread: Optional[threading.Thread] = None
        self._running = False
        self._callbacks: dict[int, Callable] = {}
        self._recv_buffer: list = []

    def connect(self) -> bool:
        """Bind the UDP socket and start listening."""
        try:
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            self.sock.bind(("0.0.0.0", self.listen_port))
            self.sock.settimeout(1.0)
            self.connected = True

            # Start listener thread
            self._running = True
            self._listener_thread = threading.Thread(target=self._listen_loop, daemon=True)
            self._listener_thread.start()
            return True
        except Exception as e:
            print(f"Transport connect failed: {e}")
            return False

    def disconnect(self):
        self._running = False
        self.connected = False
        if self.sock:
            self.sock.close()
            self.sock = None

    def send_command(self, cmd_type: int, payload: bytes = b"") -> None:
        """Send a command packet to the console."""
        if not self.connected or not self.sock:
            return
        header = struct.pack("<BBH I", cmd_type, XPROBE_VERSION, self.sequence, int(time.time()) & 0xFFFFFFFF)
        self.sequence = (self.sequence + 1) & 0xFFFF
        self.sock.sendto(header + payload, (self.console_ip, self.console_port))

    def read_memory(self, address: int, size: int) -> Optional[bytes]:
        """Request a memory read from the console."""
        payload = struct.pack(">II", address, size)
        self.send_command(CMD_READ_MEM, payload)
        # Wait for response (simplified — real implementation would use event)
        return self._wait_for_response(timeout=5.0)

    def write_memory(self, address: int, data: bytes) -> bool:
        """Write data to console memory."""
        payload = struct.pack(">I", address) + data
        self.send_command(CMD_WRITE_MEM, payload)
        return True

    def on_packet(self, pkt_type: int, callback: Callable):
        """Register a callback for a specific packet type."""
        self._callbacks[pkt_type] = callback

    def _listen_loop(self):
        """Background thread: receive and dispatch packets."""
        while self._running:
            try:
                data, addr = self.sock.recvfrom(2048)
                if len(data) < 8:
                    continue

                pkt_type = data[0]
                # Store in buffer
                self._recv_buffer.append((pkt_type, data))

                # Dispatch callback if registered
                if pkt_type in self._callbacks:
                    self._callbacks[pkt_type](data, addr)

            except socket.timeout:
                continue
            except OSError:
                break

    def _wait_for_response(self, timeout: float = 5.0) -> Optional[bytes]:
        """Block until a DATA packet arrives or timeout."""
        start = time.time()
        while time.time() - start < timeout:
            for i, (pkt_type, data) in enumerate(self._recv_buffer):
                if pkt_type == PKT_DATA:
                    self._recv_buffer.pop(i)
                    return data[8:]  # Skip header
            time.sleep(0.01)
        return None

    def drain_buffer(self) -> list:
        """Return and clear all buffered packets."""
        buf = list(self._recv_buffer)
        self._recv_buffer.clear()
        return buf

    def parse_data_packet(self, data: bytes) -> tuple:
        """Parse a DATA packet into (key, value)."""
        if len(data) < 8:
            return ("", "")
        # Skip header (8 bytes), then key (32 bytes), then value (256 bytes)
        key = data[8:40].split(b'\x00')[0].decode('ascii', errors='replace')
        value = data[40:296].split(b'\x00')[0].decode('ascii', errors='replace')
        return (key, value)

    def parse_log_packet(self, data: bytes) -> tuple:
        """Parse a LOG packet into (level, message)."""
        if len(data) < 10:
            return (0, "")
        level = data[8]
        message = data[9:265].split(b'\x00')[0].decode('ascii', errors='replace')
        return (level, message)
