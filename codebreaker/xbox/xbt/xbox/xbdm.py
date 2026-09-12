"""XBDM (Xbox Debug Monitor) TCP client.

Ported from XeCLI's Xbox360.Remote.XbdmClient.
Connects to port 730 on RGH/devkit consoles running xbdm.xex.

Protocol:
  - Text-based command/response over TCP
  - Responses start with 3-digit status code
  - 200 = single response, 201 = connected, 202 = multi-line, 203 = binary, 204 = ready for binary
  - Multi-line responses terminated by a lone "."
"""

import socket
import struct
import time
from typing import Optional
from datetime import datetime, timezone


# ── Response types ──────────────────────────────────────

XBDM_SINGLE    = 200
XBDM_CONNECTED = 201
XBDM_MULTI     = 202
XBDM_BINARY    = 203
XBDM_READY_BIN = 204

XBDM_PORT = 730


_DATA_BREAK_ACCESS = ("read", "write", "readwrite", "execute")


def cmd_data_break(address: int, access: str = "write", size: int = 4, clear: bool = False):
    """XBDM hardware data-breakpoint command string. `access` selects the access
    type that trips it: read / write / readwrite / execute."""
    access = access.lower()
    if access not in _DATA_BREAK_ACCESS:
        raise ValueError("access must be one of %s" % ", ".join(_DATA_BREAK_ACCESS))
    s = "break %s=0x%08X size=%d" % (access, address & 0xFFFFFFFF, size)
    return s + " clear" if clear else s


def cmd_code_break(address: int, clear: bool = False):
    """XBDM execution-breakpoint command string."""
    s = "break addr=0x%08X" % (address & 0xFFFFFFFF)
    return s + " clear" if clear else s


class XbdmResponse:
    """Parsed XBDM response."""
    __slots__ = ('code', 'message', 'raw')

    def __init__(self, code: int, message: str, raw: str):
        self.code = code
        self.message = message
        self.raw = raw

    @property
    def ok(self) -> bool:
        return self.code == XBDM_SINGLE

    @property
    def is_multi(self) -> bool:
        return self.code == XBDM_MULTI

    @property
    def is_binary(self) -> bool:
        return self.code == XBDM_BINARY

    def __repr__(self):
        return f"XbdmResponse({self.code}, {self.message!r})"


# ── Param parsing helpers ───────────────────────────────

def _parse_param_str(line: str, key: str) -> Optional[str]:
    """Extract key="value" or key=value from an XBDM response line."""
    needle = f"{key}="
    idx = line.find(needle)
    if idx == -1:
        return None
    start = idx + len(needle)
    if start < len(line) and line[start] == '"':
        end = line.find('"', start + 1)
        if end == -1:
            return line[start + 1:]
        return line[start + 1:end]
    end = line.find(' ', start)
    if end == -1:
        return line[start:]
    return line[start:end]


def _parse_param_u32(line: str, key: str) -> Optional[int]:
    """Extract a uint32 param (hex with 0x prefix or decimal)."""
    val = _parse_param_str(line, key)
    if val is None:
        return None
    val = val.strip()
    try:
        if val.startswith("0x") or val.startswith("0X"):
            return int(val, 16)
        return int(val)
    except ValueError:
        return None


def _parse_param_u64(line: str, key: str) -> Optional[int]:
    """Extract a uint64 param."""
    val = _parse_param_str(line, key)
    if val is None:
        return None
    val = val.strip()
    try:
        if val.startswith("0x") or val.startswith("0X"):
            return int(val, 16)
        return int(val)
    except ValueError:
        return None


def _has_flag(line: str, flag: str) -> bool:
    """Check if a bare flag word exists in the line."""
    parts = line.split()
    return flag in parts


# ── Data models ─────────────────────────────────────────

class ConsoleInfo:
    """Console identification data."""
    def __init__(self):
        self.console_id: str = ""
        self.debug_name: str = ""
        self.exec_state: str = ""
        self.title_ip: Optional[str] = None
        self.process_id: Optional[int] = None


class ModuleInfo:
    """Loaded module (XEX) info."""
    def __init__(self):
        self.name: str = ""
        self.base_address: int = 0
        self.size: int = 0
        self.original_size: int = 0
        self.timestamp: Optional[datetime] = None
        self.entry_point: Optional[int] = None
        self.sections: list = []


class SectionInfo:
    """Module section info."""
    def __init__(self):
        self.name: Optional[str] = None
        self.base_address: int = 0
        self.size: int = 0
        self.index: int = 0
        self.flags: int = 0


class DriveEntry:
    """Console drive info."""
    def __init__(self):
        self.name: str = ""
        self.total_bytes: int = 0
        self.free_bytes: int = 0


class FileEntry:
    """File/directory entry from XBDM dirlist."""
    def __init__(self):
        self.name: str = ""
        self.size: int = 0
        self.is_directory: bool = False
        self.created: Optional[datetime] = None
        self.modified: Optional[datetime] = None


class ThreadInfo:
    """Thread entry from XBDM threads."""
    def __init__(self):
        self.thread_id: int = 0
        self.suspend_count: int = 0
        self.priority: int = 0
        self.tls_base: int = 0


class UserInfo:
    """Signed-in user from XBDM userlist."""
    def __init__(self):
        self.gamertag: Optional[str] = None
        self.xuid: Optional[int] = None
        self.sign_in_state: Optional[int] = None


class MemoryRegion:
    """Memory region from XBDM walkmem."""
    def __init__(self):
        self.base_address: int = 0
        self.size: int = 0
        self.protect: int = 0


# ── XBDM Client ────────────────────────────────────────

class XbdmClient:
    """TCP client for Xbox Debug Monitor (XBDM) protocol.

    Requires xbdm.xex running on the console (standard on devkits,
    available for RGH via dashlaunch plugin loading).

    Usage:
        client = XbdmClient("10.0.0.225")
        client.connect()
        info = client.get_console_info()
        modules = client.get_modules()
        client.disconnect()
    """

    def __init__(self, host: str, port: int = XBDM_PORT, timeout: float = 5.0):
        self.host = host
        self.port = port
        self.timeout = timeout
        self._sock: Optional[socket.socket] = None
        self._buf = b""

    # ── Connection ──────────────────────────────────────

    def connect(self) -> XbdmResponse:
        """Connect to XBDM and read the handshake."""
        self._sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self._sock.settimeout(self.timeout)
        self._sock.connect((self.host, self.port))
        self._buf = b""
        resp = self._read_response()
        if resp.code != XBDM_CONNECTED and resp.code != 201:
            raise ConnectionError(f"Unexpected XBDM handshake: {resp.raw}")
        return resp

    def disconnect(self):
        """Send bye and close."""
        if self._sock:
            try:
                self._send_line("bye")
            except Exception:
                pass
            try:
                self._sock.close()
            except Exception:
                pass
            self._sock = None
            self._buf = b""

    @property
    def connected(self) -> bool:
        return self._sock is not None

    # ── Low-level I/O ───────────────────────────────────

    def _send_line(self, line: str):
        """Send a command line terminated with \\r\\n."""
        self._sock.sendall((line + "\r\n").encode("ascii"))

    def _recv_bytes(self, count: int) -> bytes:
        """Read exactly count bytes."""
        while len(self._buf) < count:
            chunk = self._sock.recv(8192)
            if not chunk:
                raise ConnectionError("XBDM connection closed")
            self._buf += chunk
        result = self._buf[:count]
        self._buf = self._buf[count:]
        return result

    def _read_line(self) -> str:
        """Read a single \\r\\n terminated line."""
        while True:
            idx = self._buf.find(b"\n")
            if idx >= 0:
                raw = self._buf[:idx]
                self._buf = self._buf[idx + 1:]
                line = raw.decode("ascii", errors="replace")
                if line.endswith("\r"):
                    line = line[:-1]
                return line
            chunk = self._sock.recv(8192)
            if not chunk:
                raise ConnectionError("XBDM connection closed")
            self._buf += chunk

    def _read_response(self) -> XbdmResponse:
        """Read and parse a single XBDM response line."""
        line = self._read_line()
        if len(line) < 3:
            raise ConnectionError(f"Invalid XBDM response: {line!r}")
        try:
            code = int(line[:3])
        except ValueError:
            raise ConnectionError(f"Invalid XBDM response code: {line!r}")
        message = line[4:] if len(line) > 4 else ""
        return XbdmResponse(code, message, line)

    def _read_multi_lines(self) -> list:
        """Read multi-line response terminated by a lone '.'."""
        lines = []
        while True:
            line = self._read_line()
            if line == ".":
                break
            lines.append(line)
        return lines

    # ── Command interface ───────────────────────────────

    def command(self, cmd: str) -> XbdmResponse:
        """Send a command and return the response."""
        self._send_line(cmd)
        return self._read_response()

    def command_lines(self, cmd: str) -> list:
        """Send a command expecting a multi-line response. Returns the lines."""
        self._send_line(cmd)
        resp = self._read_response()
        if resp.code == XBDM_MULTI:
            return self._read_multi_lines()
        if resp.ok:
            return [resp.message] if resp.message else []
        raise IOError(f"XBDM error ({resp.code}): {resp.message}")

    def command_binary(self, cmd: str, size: int) -> bytes:
        """Send a command expecting a binary response of known size."""
        self._send_line(cmd)
        resp = self._read_response()
        if resp.code == XBDM_BINARY or resp.code == XBDM_READY_BIN:
            return self._recv_bytes(size)
        raise IOError(f"Expected binary response, got: {resp.raw}")

    def command_ok(self, cmd: str) -> XbdmResponse:
        """Send a command expecting 200 OK."""
        resp = self.command(cmd)
        if resp.code != XBDM_SINGLE:
            raise IOError(f"XBDM error ({resp.code}): {resp.message}")
        return resp

    # ── Console info ────────────────────────────────────

    def get_console_info(self) -> ConsoleInfo:
        """Query console identification."""
        info = ConsoleInfo()

        resp = self.command("getconsoleid")
        cid = _parse_param_str(resp.message, "consoleid")
        info.console_id = cid if cid else resp.message

        resp = self.command("dbgname")
        info.debug_name = resp.message

        resp = self.command("getexecstate")
        info.exec_state = resp.message

        resp = self.command("altaddr")
        addr = _parse_param_u32(resp.message, "addr")
        if addr is not None:
            ip_bytes = struct.pack(">I", addr)
            info.title_ip = ".".join(str(b) for b in ip_bytes)

        resp = self.command("getpid")
        pid = _parse_param_u32(resp.message, "pid")
        if pid is not None:
            info.process_id = pid

        return info

    def get_dm_version(self) -> str:
        """Get XBDM version string."""
        return self.command("dmversion").message

    # ── Modules ─────────────────────────────────────────

    def get_modules(self, include_sections: bool = False) -> list:
        """List loaded modules (XEXs)."""
        lines = self.command_lines("modules")
        modules = []
        for line in lines:
            name = _parse_param_str(line, "name")
            base = _parse_param_u32(line, "base")
            size = _parse_param_u32(line, "size")
            if name is None or base is None or size is None:
                continue

            mod = ModuleInfo()
            mod.name = name
            mod.base_address = base
            mod.size = size
            osize = _parse_param_u32(line, "osize")
            mod.original_size = osize if osize else 0
            ts = _parse_param_u32(line, "timestamp")
            if ts:
                mod.timestamp = datetime.fromtimestamp(ts, tz=timezone.utc)

            if include_sections:
                try:
                    sec_lines = self.command_lines(f'modsections name="{name}"')
                    for sl in sec_lines:
                        sec = SectionInfo()
                        sec.name = _parse_param_str(sl, "name")
                        sec.base_address = _parse_param_u32(sl, "base") or 0
                        sec.size = _parse_param_u32(sl, "size") or 0
                        sec.index = _parse_param_u32(sl, "index") or 0
                        sec.flags = _parse_param_u32(sl, "flags") or 0
                        mod.sections.append(sec)
                except IOError:
                    pass

            modules.append(mod)
        return modules

    # ── File system ─────────────────────────────────────

    def get_drives(self, include_size: bool = False) -> list:
        """List console drives."""
        lines = self.command_lines("drivelist")
        drives = []
        for line in lines:
            name = _parse_param_str(line, "drivename")
            if not name:
                continue
            d = DriveEntry()
            d.name = name + ":"
            if include_size:
                try:
                    size_lines = self.command_lines(f'drivefreespace name="{d.name}\\"')
                    if size_lines:
                        sl = size_lines[0]
                        tlo = _parse_param_u32(sl, "totalbyteslo") or 0
                        thi = _parse_param_u32(sl, "totalbyteshi") or 0
                        flo = _parse_param_u32(sl, "totalfreebyteslo") or 0
                        fhi = _parse_param_u32(sl, "totalfreebyteshi") or 0
                        d.total_bytes = (thi << 32) | tlo
                        d.free_bytes = (fhi << 32) | flo
                except IOError:
                    pass
            drives.append(d)
        return drives

    def get_directory(self, path: str) -> list:
        """List files/directories at a path."""
        if not path.endswith("\\"):
            path += "\\"
        lines = self.command_lines(f'dirlist name="{path}"')
        entries = []
        for line in lines:
            name = _parse_param_str(line, "name")
            if not name:
                continue
            e = FileEntry()
            e.name = name
            slo = _parse_param_u32(line, "sizelo") or 0
            shi = _parse_param_u32(line, "sizehi") or 0
            e.size = (shi << 32) | slo
            e.is_directory = _has_flag(line, "directory")

            clo = _parse_param_u32(line, "createlo") or 0
            chi = _parse_param_u32(line, "createhi") or 0
            if clo or chi:
                ft = (chi << 32) | clo
                # Windows FILETIME: 100ns intervals since 1601-01-01
                if ft > 116444736000000000:
                    e.created = datetime.fromtimestamp(
                        (ft - 116444736000000000) / 10000000, tz=timezone.utc)

            mlo = _parse_param_u32(line, "changelo") or 0
            mhi = _parse_param_u32(line, "changehi") or 0
            if mlo or mhi:
                ft = (mhi << 32) | mlo
                if ft > 116444736000000000:
                    e.modified = datetime.fromtimestamp(
                        (ft - 116444736000000000) / 10000000, tz=timezone.utc)

            entries.append(e)
        return entries

    # ── Memory ──────────────────────────────────────────

    def memory_read(self, address: int, size: int) -> bytes:
        """Read memory from the console.

        Tries getmem (hex-encoded response) first, falls back to
        getmemex (binary response) if the console returns 203.
        """
        result = bytearray()
        remaining = size
        addr = address
        while remaining > 0:
            chunk = min(remaining, 1024)
            self._send_line(f"getmem addr=0x{addr:08X} length={chunk}")
            resp = self._read_response()

            if resp.code == XBDM_SINGLE:
                # Hex-encoded text response
                hex_data = resp.message.strip()
                if not hex_data:
                    break
                # Filter to only hex chars (skip "memory data follows" etc.)
                clean = "".join(c for c in hex_data if c in "0123456789abcdefABCDEF")
                if clean:
                    result.extend(bytes.fromhex(clean))
                else:
                    break

            elif resp.code == XBDM_BINARY or "memory" in resp.message.lower():
                # Binary response — read raw bytes
                # Some XBDMs send the hex on the next line(s) as multi-line
                # Others send raw binary after the 203 header
                try:
                    # Try reading as multi-line hex first
                    lines = self._read_multi_lines()
                    hex_str = "".join(lines).strip()
                    clean = "".join(c for c in hex_str if c in "0123456789abcdefABCDEF")
                    if clean:
                        result.extend(bytes.fromhex(clean))
                    else:
                        # Raw binary
                        raw = self._recv_bytes(chunk)
                        result.extend(raw)
                except Exception:
                    # Last resort: try raw binary read
                    try:
                        raw = self._recv_bytes(chunk)
                        result.extend(raw)
                    except Exception:
                        break

            elif resp.code == XBDM_MULTI:
                # Multi-line hex response
                lines = self._read_multi_lines()
                hex_str = "".join(lines).strip()
                clean = "".join(c for c in hex_str if c in "0123456789abcdefABCDEF")
                if clean:
                    result.extend(bytes.fromhex(clean))
                else:
                    break

            else:
                raise IOError(f"Memory read failed at 0x{addr:08X}: {resp.message}")

            addr += chunk
            remaining -= chunk
        return bytes(result)

    def memory_write(self, address: int, data: bytes):
        """Write data to console memory."""
        hex_str = data.hex().upper()
        self.command_ok(f"setmem addr=0x{address:08X} data={hex_str}")

    def memory_find(self, address: int, size: int, pattern: bytes) -> Optional[int]:
        """Search memory for a byte pattern. Returns address or None."""
        # Read the region and search locally — XBDM doesn't have native search
        try:
            mem = self.memory_read(address, size)
            idx = mem.find(pattern)
            if idx >= 0:
                return address + idx
        except IOError:
            pass
        return None

    def get_memory_regions(self) -> list:
        """Walk memory regions via walkmem."""
        lines = self.command_lines("walkmem")
        regions = []
        for line in lines:
            r = MemoryRegion()
            r.base_address = _parse_param_u32(line, "base") or 0
            r.size = _parse_param_u32(line, "size") or 0
            r.protect = _parse_param_u32(line, "protect") or 0
            regions.append(r)
        return regions

    # ── Threads ─────────────────────────────────────────

    def get_threads(self) -> list:
        """List running threads."""
        lines = self.command_lines("threads")
        threads = []
        for line in lines:
            t = ThreadInfo()
            tid = _parse_param_u32(line, "thread")
            if tid is None:
                # Some formats just list the thread ID directly
                try:
                    t.thread_id = int(line.strip(), 16) if line.strip().startswith("0x") else int(line.strip())
                except ValueError:
                    continue
            else:
                t.thread_id = tid
            t.suspend_count = _parse_param_u32(line, "suspend") or 0
            t.priority = _parse_param_u32(line, "priority") or 0
            t.tls_base = _parse_param_u32(line, "tlsbase") or 0
            threads.append(t)
        return threads

    def suspend_thread(self, thread_id: int):
        self.command_ok(f"suspend thread=0x{thread_id:08X}")

    def resume_thread(self, thread_id: int):
        self.command_ok(f"resume thread=0x{thread_id:08X}")

    def get_thread_context(self, thread_id: int) -> dict:
        """Get thread register context."""
        lines = self.command_lines(f"getcontext thread=0x{thread_id:08X} control")
        ctx = {}
        for line in lines:
            parts = line.split("=", 1)
            if len(parts) == 2:
                key = parts[0].strip()
                try:
                    ctx[key] = int(parts[1].strip(), 16)
                except ValueError:
                    ctx[key] = parts[1].strip()
        return ctx

    # ── Users ───────────────────────────────────────────

    def get_users(self) -> list:
        """List signed-in users."""
        try:
            lines = self.command_lines("userlist")
        except IOError:
            return []
        users = []
        for line in lines:
            u = UserInfo()
            u.gamertag = (_parse_param_str(line, "name")
                          or _parse_param_str(line, "gamertag")
                          or _parse_param_str(line, "gtag"))
            xuid = _parse_param_u64(line, "xuid")
            u.xuid = xuid
            u.sign_in_state = _parse_param_u32(line, "signinstate")
            users.append(u)
        return users

    # ── Debug control ───────────────────────────────────

    def debug_stop(self):
        """Break all threads."""
        self.command_ok("stop")

    def debug_go(self):
        """Resume all threads."""
        self.command_ok("go")

    def set_breakpoint(self, address: int):
        """Set an execution breakpoint."""
        self.command_ok(cmd_code_break(address))

    def clear_breakpoint(self, address: int):
        """Remove an execution breakpoint."""
        self.command_ok(cmd_code_break(address, clear=True))

    def set_data_breakpoint(self, address: int, size: int = 4, access: str = "write"):
        """Set a hardware data-access breakpoint. `access` is one of
        read/write/readwrite/execute — the debugger halts (and reports the PC of
        the instruction) when the address range is accessed that way."""
        self.command_ok(cmd_data_break(address, access, size))

    def clear_data_breakpoint(self, address: int, size: int = 4, access: str = "write"):
        self.command_ok(cmd_data_break(address, access, size, clear=True))

    def clear_all_breakpoints(self):
        self.command_ok("break clearall")

    def break_now(self):
        """Force an immediate debug break."""
        self.command_ok("break now")

    def debugger_connect(self, name: str = "XboxTranspiler", override: bool = True):
        cmd = "debugger connect"
        if override:
            cmd += " override"
        cmd += f' name="{name}"'
        return self.command(cmd)

    def is_debugger(self) -> bool:
        """True if a debugger is attached (XBDM replies 410)."""
        return self.command("isdebugger").code == 410

    def watch(self, address: int, size: int = 4, access: str = "write", timeout: float = 30.0):
        """"What accesses this address": attach the debugger, arm a data
        breakpoint, and return the first break notification (the accessing PC /
        thread). Returns the raw notification text, or None on timeout."""
        self.debugger_connect()
        self.set_data_breakpoint(address, size, access)
        try:
            resp = self.command("break now") and None  # arm/notify channel
        except Exception:
            resp = None
        # the console pushes an async "break" notification on access; read it.
        try:
            return self._read_notification(timeout)
        except Exception:
            return None

    def _read_notification(self, timeout: float):
        import socket as _s
        if not self._sock:
            return None
        self._sock.settimeout(timeout)
        try:
            data = self._sock.recv(4096)
        except _s.timeout:
            return None
        return data.decode("latin-1", "replace").strip()

    # ── System control ──────────────────────────────────

    def reboot(self, cold: bool = False):
        """Reboot the console."""
        cmd = "magicboot" if not cold else "magicboot cold"
        try:
            self.command(cmd)
        except Exception:
            pass  # Connection drops on reboot

    def shutdown(self):
        """Power off the console."""
        try:
            self.command("shutdown")
        except Exception:
            pass

    def launch(self, path: str):
        """Launch a XEX by path."""
        self.command_ok(f'magicboot title="{path}"')

    def get_running_xex(self) -> Optional[str]:
        """Get the path of the currently running XEX."""
        lines = self.command_lines("xbeinfo running")
        for line in lines:
            name = _parse_param_str(line, "name")
            if name:
                return name
        return None

    # ── Screenshot ──────────────────────────────────────

    def screenshot(self) -> bytes:
        """Capture a screenshot. Returns raw pixel data.

        The XBDM screenshot command returns a header followed by pixel data.
        Format is typically 1280x720 32bpp BGRA.
        """
        self._send_line("screenshot")
        resp = self._read_response()
        if resp.code != XBDM_BINARY:
            raise IOError(f"Screenshot failed: {resp.raw}")

        # Parse pitch, width, height, format from response message
        pitch = _parse_param_u32(resp.message, "pitch") or 5120
        height = _parse_param_u32(resp.message, "height") or 720
        width = _parse_param_u32(resp.message, "width") or 1280
        fmt = _parse_param_u32(resp.message, "format") or 0

        size = pitch * height
        data = self._recv_bytes(size)
        return data

    # ── File transfer ───────────────────────────────────

    def get_file(self, remote_path: str) -> bytes:
        """Download a file from the console."""
        self._send_line(f'getfile name="{remote_path}"')
        resp = self._read_response()
        if resp.code != XBDM_BINARY:
            raise IOError(f"getfile failed: {resp.raw}")
        size = _parse_param_u32(resp.message, "size")
        if size is None:
            raise IOError(f"No size in getfile response: {resp.message}")
        return self._recv_bytes(size)

    def send_file(self, remote_path: str, data: bytes):
        """Upload a file to the console."""
        resp = self.command(f'sendfile name="{remote_path}" length={len(data)}')
        if resp.code != XBDM_READY_BIN:
            raise IOError(f"sendfile rejected: {resp.raw}")
        self._sock.sendall(data)
        confirm = self._read_response()
        if confirm.code != XBDM_SINGLE:
            raise IOError(f"sendfile failed: {confirm.raw}")

    def mkdir(self, path: str):
        """Create a directory on the console."""
        self.command_ok(f'mkdir name="{path}"')

    def delete(self, path: str, is_dir: bool = False):
        """Delete a file or directory."""
        cmd = f'delete name="{path}"'
        if is_dir:
            cmd += " dir"
        self.command_ok(cmd)

    def rename(self, old_path: str, new_path: str):
        """Rename/move a file."""
        self.command_ok(f'rename name="{old_path}" newname="{new_path}"')

    # ── Notify / XEX info ──────────────────────────────

    def notify(self, message: str):
        """Send a debug notification string."""
        self.command(f'debugstr string="{message}"')

    def get_xex_info(self, module_name: str, field: int) -> Optional[str]:
        """Query a XEX optional header field for a loaded module."""
        try:
            lines = self.command_lines(f'xexfield module="{module_name}" field=0x{field:X}')
            return lines[-1] if lines else None
        except IOError:
            return None

    # ── Raw command ─────────────────────────────────────

    def raw(self, cmd: str) -> tuple:
        """Send a raw XBDM command. Returns (response, lines_or_none)."""
        self._send_line(cmd)
        resp = self._read_response()
        if resp.code == XBDM_MULTI:
            lines = self._read_multi_lines()
            return (resp, lines)
        return (resp, None)


DESCRIPTION = "talk to a dev console over XBDM (info / modules / drives / raw cmd)"


def main():
    import sys
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage: xbdm <ip> [info|modules|drives|raw <cmd>|\n"
              "                  break <addr>|watch <addr> [size] [read|write|readwrite|execute]]")
        return
    ip, op = a[0], (a[1] if len(a) > 1 else "info")
    c = XbdmClient(ip)
    c.connect()
    try:
        if op == "info":
            print(c.get_console_info())
        elif op == "modules":
            for m in c.get_modules():
                print("  %s" % (m,))
        elif op == "drives":
            for d in c.get_drives(True):
                print("  %s" % (d,))
        elif op == "raw" and len(a) > 2:
            print(c.command(a[2]).raw)
        elif op == "break" and len(a) > 2:
            addr = int(a[2], 0)
            c.set_breakpoint(addr)
            print("execution breakpoint set @0x%08X" % addr)
        elif op == "watch" and len(a) > 2:
            addr = int(a[2], 0)
            access = next((x for x in a[3:] if x in _DATA_BREAK_ACCESS), "write")
            size = next((int(x, 0) for x in a[3:] if x not in _DATA_BREAK_ACCESS), 4)
            print("watching 0x%08X size=%d for %s access..." % (addr, size, access))
            note = c.watch(addr, size, access)
            print(note if note else "(no access observed before timeout)")
        else:
            print("usage: xbdm <ip> [info|modules|drives|raw <cmd>|break <addr>|watch <addr> [size] [access]]")
            sys.exit(2)
    finally:
        c.disconnect()


if __name__ == "__main__":
    main()
