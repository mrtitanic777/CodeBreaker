"""JRPC2 (JSON RPC v2) client over XBDM.

Ported from XeCLI's Xbox360.Remote.Jrpc2Client.
Requires JRPC2.xex loaded on the console alongside xbdm.xex.

JRPC2 uses the XBDM "consolefeatures" command to dispatch typed
RPC calls to functions on the console, read hardware info, control
LEDs, show notifications, resolve ordinals, and more.
"""

from typing import Optional
from xbdm import XbdmClient, XbdmResponse, XBDM_SINGLE


# ── Data types for RPC ──────────────────────────────────

class RpcDataType:
    VOID       = 0
    INT        = 1
    STRING     = 2
    FLOAT      = 3
    BYTE       = 4
    INT_ARRAY  = 5
    FLOAT_ARRAY = 6
    BYTE_ARRAY = 7
    UINT64     = 8
    UINT64_ARRAY = 9


class SensorType:
    CPU         = 0
    GPU         = 1
    EDRAM       = 2
    MOTHERBOARD = 3


SENSOR_NAMES = {
    SensorType.CPU: "CPU",
    SensorType.GPU: "GPU",
    SensorType.EDRAM: "EDRAM",
    SensorType.MOTHERBOARD: "Motherboard",
}


# ── Helpers ─────────────────────────────────────────────

def _str_to_hex(s: str) -> str:
    """Convert ASCII string to hex representation."""
    return s.encode("ascii").hex().upper()


def _extract_hex(text: str, length: int) -> str:
    """Extract up to `length` hex characters from text."""
    result = []
    for ch in text:
        if ch in "0123456789abcdefABCDEF":
            result.append(ch)
            if len(result) == length:
                break
    return "".join(result)


# ── JRPC2 Client ───────────────────────────────────────

class Jrpc2Client:
    """JRPC2 protocol client — wraps an XbdmClient.

    Usage:
        xbdm = XbdmClient("10.0.0.225")
        xbdm.connect()
        jrpc2 = Jrpc2Client(xbdm)
        cpukey = jrpc2.get_cpukey()
        temps = jrpc2.get_temperatures()
    """

    def __init__(self, xbdm: XbdmClient):
        self.xbdm = xbdm

    def _cmd(self, command: str) -> XbdmResponse:
        """Send a consolefeatures command and return the response."""
        return self.xbdm.command(command)

    def _cmd_ok(self, command: str) -> str:
        """Send command, raise on non-200, return message."""
        resp = self._cmd(command)
        if resp.code != XBDM_SINGLE:
            raise IOError(f"JRPC2 error ({resp.code}): {resp.message}")
        return resp.message

    # ── Built-in queries (type 9-17) ────────────────────

    def get_cpukey(self) -> str:
        """Get the console's CPU key (32 hex chars)."""
        msg = self._cmd_ok('consolefeatures ver=2 type=10 params="A\\0\\A\\0\\"')
        key = _extract_hex(msg, 32)
        if len(key) != 32:
            raise IOError(f"Invalid CPU key response: {msg}")
        return key.upper()

    def get_dashboard_version(self) -> int:
        """Get the dashboard/kernel version number."""
        msg = self._cmd_ok('consolefeatures ver=2 type=13 params="A\\0\\A\\4\\"')
        try:
            return int(msg.strip())
        except ValueError:
            raise IOError(f"Invalid dashboard version: {msg}")

    def get_temperature(self, sensor: int) -> int:
        """Read a temperature sensor. Returns degrees Celsius."""
        msg = self._cmd_ok(
            f'consolefeatures ver=2 type=15 params="A\\0\\A\\1\\{RpcDataType.INT}\\{sensor}\\"')
        try:
            return int(msg.strip(), 16)
        except ValueError:
            raise IOError(f"Invalid temperature response: {msg}")

    def get_temperatures(self) -> dict:
        """Read all four temperature sensors."""
        temps = {}
        for sensor_id, name in SENSOR_NAMES.items():
            try:
                temps[name] = self.get_temperature(sensor_id)
            except IOError:
                temps[name] = None
        return temps

    def get_title_id(self) -> int:
        """Get the currently running title ID."""
        msg = self._cmd_ok('consolefeatures ver=2 type=16 params="A\\0\\A\\0\\"')
        try:
            return int(msg.strip(), 16)
        except ValueError:
            raise IOError(f"Invalid title ID: {msg}")

    def get_motherboard_type(self) -> str:
        """Get the motherboard revision string."""
        return self._cmd_ok('consolefeatures ver=2 type=17 params="A\\0\\A\\0\\"').strip()

    # ── LEDs ────────────────────────────────────────────

    def set_leds(self, top_left: int, top_right: int, bottom_left: int, bottom_right: int):
        """Set the ring-of-light LED quadrants.

        Values: 0=off, 1=green, 2=red, 3=orange
        """
        self._cmd_ok(
            f'consolefeatures ver=2 type=14 params="A\\0\\A\\4\\'
            f'{RpcDataType.INT}\\{top_left}\\'
            f'{RpcDataType.INT}\\{top_right}\\'
            f'{RpcDataType.INT}\\{bottom_left}\\'
            f'{RpcDataType.INT}\\{bottom_right}\\"')

    # ── Notifications ───────────────────────────────────

    def show_notification(self, message: str, logo: int = 0):
        """Show an on-screen notification on the console.

        Logo values: 0=none, various XNotify icon indices.
        """
        hex_msg = _str_to_hex(message)
        self.xbdm.command(
            f'consolefeatures ver=2 type=12 params="A\\0\\A\\2\\'
            f'2/{len(message)}\\{hex_msg}\\'
            f'{RpcDataType.INT}\\{logo}\\"')

    # ── Function resolution ─────────────────────────────

    def resolve_function(self, module: str, ordinal: int) -> Optional[int]:
        """Resolve a function address by module name and ordinal.

        Returns the address or None if not found.
        """
        hex_mod = _str_to_hex(module)
        resp = self._cmd(
            f'consolefeatures ver=2 type=9 params="A\\0\\A\\2\\'
            f'{RpcDataType.STRING}/{len(module)}\\{hex_mod}\\'
            f'{RpcDataType.INT}\\{ordinal}\\"')
        if resp.code != XBDM_SINGLE:
            return None
        parts = resp.message.strip().split(" ", 1)
        if len(parts) < 2:
            return None
        try:
            return int(parts[1].strip(), 16)
        except ValueError:
            return None

    # ── Shutdown ────────────────────────────────────────

    def shutdown(self):
        """Power off the console via JRPC2."""
        self._cmd_ok('consolefeatures ver=2 type=11 params="A\\0\\A\\0\\"')

    # ── Generic RPC call ────────────────────────────────

    def call(self, address: int = 0, module: str = None, ordinal: int = None,
             return_type: int = RpcDataType.VOID, system_thread: bool = False,
             args: list = None) -> str:
        """Call an arbitrary function on the console via JRPC2.

        Args can be tuples of (type, value):
            ("int", 42)
            ("uint", 0xDEADBEEF)
            ("float", 3.14)
            ("string", "hello")
            ("bytes", b"\\x00\\x01")
            ("bool", True)
            ("uint64", 0x123456789ABCDEF0)

        Returns the response string (may need parsing depending on return_type).
        """
        args = args or []
        argc = 0
        params_str = ""

        for arg in args:
            atype, aval = arg
            if atype in ("int", "uint"):
                params_str += f"{RpcDataType.INT}\\{int(aval)}\\"
                argc += 1
            elif atype == "bool":
                params_str += f"{RpcDataType.INT}\\{1 if aval else 0}\\"
                argc += 1
            elif atype == "float":
                params_str += f"{RpcDataType.FLOAT}\\{float(aval)}\\"
                argc += 1
            elif atype == "byte":
                params_str += f"{RpcDataType.BYTE}\\{int(aval)}\\"
                argc += 1
            elif atype == "string":
                s = str(aval)
                params_str += f"{RpcDataType.BYTE_ARRAY}/{len(s)}\\{_str_to_hex(s)}\\"
                argc += 1
            elif atype == "bytes":
                b = bytes(aval)
                params_str += f"{RpcDataType.BYTE_ARRAY}/{len(b)}\\{b.hex().upper()}\\"
                argc += 1
            elif atype == "uint64":
                params_str += f"{RpcDataType.UINT64}\\{int(aval)}\\"
                argc += 1

        cmd = f"consolefeatures ver=2 type={return_type}"
        if system_thread:
            cmd += " system"
        if module and ordinal is not None:
            cmd += f' module="{module}" ord={ordinal}'
        cmd += f' as=0 params="A\\{address:X}\\A\\{argc}\\{params_str}"'

        msg = self._cmd_ok(cmd)

        # Handle buf_addr loop (async buffer fetch)
        for _ in range(10):
            if "buf_addr=" not in msg:
                break
            idx = msg.find("buf_addr=")
            addr_str = msg[idx + 9:].strip()
            try:
                buf_addr = int(addr_str, 16)
            except ValueError:
                break
            msg = self._cmd_ok(f"consolefeatures ver=2 buf_addr=0x{buf_addr:X}")

        # Extract the return value (after first space)
        parts = msg.strip().split(" ", 1)
        if len(parts) > 1:
            return parts[1].strip()
        return msg.strip()

    def call_void(self, module: str, ordinal: int, args: list = None,
                  system_thread: bool = False):
        """Call a void function by module + ordinal."""
        self.call(module=module, ordinal=ordinal, return_type=RpcDataType.VOID,
                  system_thread=system_thread, args=args)

    def call_int(self, module: str, ordinal: int, args: list = None,
                 system_thread: bool = False) -> int:
        """Call a function returning int by module + ordinal."""
        result = self.call(module=module, ordinal=ordinal, return_type=RpcDataType.INT,
                           system_thread=system_thread, args=args)
        return int(result, 16) if result else 0

    def call_address(self, address: int, return_type: int = RpcDataType.VOID,
                     args: list = None, system_thread: bool = False) -> str:
        """Call a function at a raw address."""
        return self.call(address=address, return_type=return_type,
                         system_thread=system_thread, args=args)


DESCRIPTION = "call JRPC2 on a dev console (CPU key, temperatures)"


# ---- real-time editing (peek / poke / freeze) — Xbox 360 memory is big-endian
import struct as _struct

_TYPES = {"u8": ">B", "u16": ">H", "u32": ">I", "u64": ">Q",
          "s8": ">b", "s16": ">h", "s32": ">i", "float": ">f", "double": ">d"}


def pack_typed(kind: str, value) -> bytes:
    """Pack a value to big-endian bytes for poking. 'bytes' expects a hex str."""
    if kind == "bytes":
        return bytes.fromhex(value.replace(" ", "")) if isinstance(value, str) else bytes(value)
    fmt = _TYPES[kind]
    v = float(value) if kind in ("float", "double") else int(str(value), 0)
    return _struct.pack(fmt, v)


def unpack_typed(kind: str, data: bytes):
    """Interpret big-endian bytes as a typed value for peeking."""
    if kind == "bytes":
        return data.hex()
    return _struct.unpack(_TYPES[kind], data[:_struct.calcsize(_TYPES[kind])])[0]


def peek(xbdm, addr: int, kind: str = "u32"):
    size = len(bytes(1)) if kind == "bytes" else _struct.calcsize(_TYPES[kind])
    return unpack_typed(kind, xbdm.memory_read(addr, max(size, 4)))


def poke(xbdm, addr: int, kind: str, value):
    xbdm.memory_write(addr, pack_typed(kind, value))


def main():
    import sys
    import time
    a = sys.argv[1:]
    usage = ("usage:\n"
             "  jrpc2 <ip> cpukey|temps\n"
             "  jrpc2 <ip> peek   <addr> [u8|u16|u32|u64|float|bytes[:N]]\n"
             "  jrpc2 <ip> poke   <addr> <value> [type]\n"
             "  jrpc2 <ip> freeze <addr> <value> [type] [--hz N]   (Ctrl-C to stop)\n"
             "  jrpc2 <ip> call   <module> <ordinal> [int-args...]  (returns int)")
    if not a or a[0] in ("-h", "--help"):
        print(usage); return
    ip, op = a[0], (a[1] if len(a) > 1 else "cpukey")
    x = XbdmClient(ip)
    x.connect()
    j = Jrpc2Client(x)
    try:
        if op == "cpukey":
            print(j.get_cpukey())
        elif op in ("temps", "temperatures"):
            for k, v in j.get_temperatures().items():
                print("  %-12s %s" % (k, v))
        elif op == "peek":
            addr = int(a[2], 0)
            kind = a[3] if len(a) > 3 else "u32"
            if kind.startswith("bytes"):
                n = int(kind.split(":")[1]) if ":" in kind else 16
                print("0x%08X: %s" % (addr, x.memory_read(addr, n).hex()))
            else:
                print("0x%08X = %s (%s)" % (addr, peek(x, addr, kind), kind))
        elif op == "poke":
            addr, val = int(a[2], 0), a[3]
            kind = a[4] if len(a) > 4 else "u32"
            poke(x, addr, kind, val)
            print("poked 0x%08X <- %s (%s)" % (addr, val, kind))
        elif op == "freeze":
            addr, val = int(a[2], 0), a[3]
            kind = a[4] if len(a) > 4 and not a[4].startswith("--") else "u32"
            hz = 10
            if "--hz" in a:
                hz = int(a[a.index("--hz") + 1])
            data = pack_typed(kind, val)
            print("freezing 0x%08X = %s (%s) @ %d Hz — Ctrl-C to stop" % (addr, val, kind, hz))
            try:
                while True:
                    x.memory_write(addr, data)
                    time.sleep(1.0 / hz)
            except KeyboardInterrupt:
                print("\nstopped.")
        elif op == "call":
            module, ordinal = a[2], int(a[3], 0)
            args = [int(v, 0) for v in a[4:]]
            print("call %s:%d(%s) -> %s" % (module, ordinal, args,
                                            j.call_int(module, ordinal, args)))
        else:
            print(usage); sys.exit(2)
    finally:
        x.disconnect()


if __name__ == "__main__":
    main()
