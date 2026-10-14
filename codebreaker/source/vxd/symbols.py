r"""symbols.py -- recover VxD function/handler names from the Win95 DDK.

Three DDK sources name things the raw disassembly can only call sub_XXXX:

  * control_messages()  -- the system control messages (SYS_CRITICAL_INIT=0, DEVICE_INIT=1, ...,
    from VMM.INC), so a control-dispatch handler can be named after the message it services.
  * sym_names(name)     -- the DDK debug .SYM files (DEBUG/<DEV>.SYM, MAPSYM output) carry the REAL
    internal function names (VKD_Filter_Keyboard_Input, VKD_Put_Byte, ...) for 12 core VxDs. Record
    format: <u16 offset><u8 namelen><name>. NOTE these are the DEBUG build's offsets; the retail
    oracle differs, so a caller applies a name only where the offset lands on a real function start.

The service names live in engine.inverse_engine.vxd_services (the exported interface); this module
adds the control handlers and the internal functions.
"""
import os
import re
import glob
import struct
import functools
from codebreaker.base import corpus as paths, settings

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
_INC = [paths.reference("Microsoft Windows 95 DDK", "INC32"),   # the moved reference SDK/DDK tree
        os.path.dirname(settings.ROOT) + "/docs/reference/win95ddk/INC32"]
_DEBUG = [paths.reference("Microsoft Windows 95 DDK", "DEBUG"),
          os.path.dirname(settings.ROOT) + "/docs/reference/win95ddk/DEBUG"]


def _find(cands, leaf=""):
    for c in cands:
        p = os.path.join(ROOT, c, leaf) if leaf else os.path.join(ROOT, c)
        if os.path.exists(p):
            return p
    return None


@functools.lru_cache(maxsize=1)
def control_messages():
    """{message_number: MESSAGE_NAME} for the system control messages, from VMM.INC."""
    vmm = _find(_INC, "VMM.INC")
    if not vmm:
        return {}
    txt = open(vmm, encoding="latin1").read()
    out, started = {}, False
    for line in txt.splitlines():
        m = re.match(r"\s*([A-Za-z_][A-Za-z0-9_]*)\s+EQU\s+([0-9A-Fa-f]+)H\s*$", line)
        if not m:
            continue
        name, val = m.group(1), int(m.group(2), 16)
        if name.upper() == "SYS_CRITICAL_INIT":
            started = True
        if not started or val >= 0x40:
            continue
        u = name.upper()
        if u.endswith(("_BIT", "_FLAG", "_ID")) or u.startswith("DIOC"):
            continue
        out.setdefault(val, name)
        if u == "SYSTEM_EXIT2":                            # tail of the message block
            break
    return out


def sym_names(device):
    """{offset: name} from DEBUG/<device>.SYM, or {} if there is no symbol file for it."""
    dbg = _find(_DEBUG)
    if not dbg:
        return {}
    hits = glob.glob(os.path.join(dbg, device.upper() + ".SYM")) or \
        glob.glob(os.path.join(dbg, device.lower() + ".sym"))
    if not hits:
        return {}
    d = open(hits[0], "rb").read()
    out = {}
    p = 0
    while p < len(d) - 3:
        off = struct.unpack_from("<H", d, p)[0]
        ln = d[p + 2]
        nm = d[p + 3:p + 3 + ln]
        if 3 <= ln <= 40 and all(c == 0x5F or 48 <= c <= 57 or 65 <= c <= 90 or 97 <= c <= 122 for c in nm):
            out.setdefault(off, nm.decode("latin1"))
            p += 3 + ln
        else:
            p += 1
    # drop the segment/group markers (offsets 0xff00/0x1a00-style with class names)
    return {o: n for o, n in out.items() if not n.startswith("_") or n.count("_") > 1}


if __name__ == "__main__":
    cm = control_messages()
    print("control messages:", len(cm))
    for k in sorted(cm)[:6]:
        print("  %2d %s" % (k, cm[k]))
    for dev in ("VKD", "VPICD", "VMM"):
        print("%s.SYM: %d symbols" % (dev, len(sym_names(dev))))
