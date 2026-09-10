"""Xenia Canary `.patch.toml` game-patch loader / applier.

Parses the community `xenia-canary/game-patches` TOML format and applies the
patches either statically (baked into a XEX via the xexinject VA-write + resign
path) or live (poked into a running title over JRPC2). Format reimplemented
clean-room from the Xenia patch spec (op types + big-endian value encoding).

A `.patch.toml` file:

    title_name = "..."
    title_id = "584108A8"
    hash = ["...", "..."]          # per-build image hashes Xenia matches on
    media_id = ["..."]

    [[patch]]
    name = "Infinite Health"
    author = "..."
    desc = "..."
    is_enabled = false             # opt-in; only true patches apply by default

      [[patch.be32]]               # be8/be16/be32/be64/array/f32/f64/string/u16string
      address = 0x82012340
      value = 0x7FFFFFFF

All numeric values are written **big-endian** (Xbox 360). Addresses are virtual.

Commands: info / verify / apply (static → XEX) / live (→ JRPC2 console).
"""

import struct
import sys

DESCRIPTION = "apply Xenia `.patch.toml` game patches to a XEX (static) or live console (JRPC2)"

_OPS = ("be8", "be16", "be32", "be64", "array", "f32", "f64", "string", "u16string")


# ── minimal TOML value parsing (matches Xenia's MiniToml) ──────────────
def _strip_comment(s):
    out, q = [], None
    for c in s:
        if q:
            out.append(c)
            if c == q:
                q = None
        elif c in "\"'":
            q = c
            out.append(c)
        elif c == "#":
            break
        else:
            out.append(c)
    return "".join(out).strip()


def _parse_long(v):
    v = _strip_comment(v).strip().strip('"').strip("'").strip()
    neg = v.startswith("-")
    if neg:
        v = v[1:]
    base = 16 if v.lower().startswith("0x") else 10
    n = int(v, base) if v else 0
    return -n if neg else n


def _parse_double(v):
    return float(_strip_comment(v).strip().strip('"').strip("'"))


def _parse_string(v):
    v = _strip_comment(v).strip()
    if len(v) >= 2 and v[0] in "\"'" and v[-1] == v[0]:
        v = v[1:-1]
    return (v.replace("\\n", "\n").replace("\\t", "\t")
             .replace('\\"', '"').replace("\\\\", "\\"))


def _parse_bool(v):
    return _strip_comment(v).strip().lower() == "true"


def _parse_string_list(v):
    # v may be a single line "[ "a", "b" ]" or the whole multi-line blob.
    out, cur, inq = [], [], False
    for c in v:
        if inq:
            if c == '"':
                out.append("".join(cur))
                cur, inq = [], False
            else:
                cur.append(c)
        elif c == '"':
            inq = True
    return out


# ── value encoder ──────────────────────────────────────────────────────
def encode_value(op, raw):
    """Encode a patch value as the big-endian byte sequence written to memory."""
    if op == "be8":
        return bytes([_parse_long(raw) & 0xFF])
    if op == "be16":
        return struct.pack(">H", _parse_long(raw) & 0xFFFF)
    if op == "be32":
        return struct.pack(">I", _parse_long(raw) & 0xFFFFFFFF)
    if op == "be64":
        return struct.pack(">Q", _parse_long(raw) & 0xFFFFFFFFFFFFFFFF)
    if op == "f32":
        return struct.pack(">f", _parse_double(raw))
    if op == "f64":
        return struct.pack(">d", _parse_double(raw))
    if op == "array":
        return bytes.fromhex(_parse_string(raw).replace(" ", "").replace(",", ""))
    if op == "string":
        return _parse_string(raw).encode("utf-8")
    if op == "u16string":
        return _parse_string(raw).encode("utf-16-be")
    raise ValueError("unknown patch op type: %s" % op)


# ── model ──────────────────────────────────────────────────────────────
class Operation:
    def __init__(self, op, address, raw):
        self.op = op
        self.address = address & 0xFFFFFFFF
        self.raw = raw
        self.blob = encode_value(op, raw)


class Patch:
    def __init__(self):
        self.name = self.author = self.desc = ""
        self.is_enabled = False
        self.ops = []


class PatchFile:
    def __init__(self):
        self.title_name = ""
        self.title_id = ""
        self.hashes = []
        self.media_ids = []
        self.patches = []

    def enabled(self):
        return [p for p in self.patches if p.is_enabled]


def parse(text):
    f = PatchFile()
    cur_patch = None
    cur_op = None
    lines = text.splitlines()
    i = 0
    while i < len(lines):
        line = _strip_comment(lines[i])
        i += 1
        if not line:
            continue
        if line.startswith("[["):
            path = line[2:line.index("]]")].strip() if "]]" in line else line[2:].strip()
            if path.lower() == "patch":
                cur_patch = Patch()
                f.patches.append(cur_patch)
                cur_op = None
            elif path.lower().startswith("patch."):
                sub = path[len("patch."):].strip().lower()
                cur_op = {"type": sub if sub in _OPS else "unknown", "address": 0, "value": "0"}
                if cur_patch is not None:
                    cur_patch.ops.append(cur_op)
            continue
        if line.startswith("["):
            cur_patch = None
            cur_op = None
            continue
        if "=" not in line:
            continue
        key, val = line.split("=", 1)
        key, val = key.strip().lower(), val.strip()
        # multi-line array continuation (hash/media_id split across lines)
        if val.startswith("[") and "]" not in val:
            while i < len(lines) and "]" not in val:
                val += " " + _strip_comment(lines[i])
                i += 1
        if cur_op is not None:
            if key == "address":
                cur_op["address"] = _parse_long(val) & 0xFFFFFFFF
            elif key == "value":
                cur_op["value"] = val
        elif cur_patch is not None:
            if key == "name":
                cur_patch.name = _parse_string(val)
            elif key == "author":
                cur_patch.author = _parse_string(val)
            elif key == "desc":
                cur_patch.desc = _parse_string(val)
            elif key == "is_enabled":
                cur_patch.is_enabled = _parse_bool(val)
        else:
            if key == "title_name":
                f.title_name = _parse_string(val)
            elif key == "title_id":
                f.title_id = _parse_string(val).upper()
            elif key == "hash":
                f.hashes += [h.upper() for h in _parse_string_list(val)]
            elif key == "media_id":
                f.media_ids += [m.upper() for m in _parse_string_list(val)]
    # materialize ops
    for p in f.patches:
        real = []
        for o in p.ops:
            if o["type"] in _OPS:
                try:
                    real.append(Operation(o["type"], o["address"], o["value"]))
                except (ValueError, struct.error):
                    pass
        p.ops = real
    return f


def load(path):
    with open(path, "r", encoding="utf-8-sig") as fh:
        return parse(fh.read())


# ── apply: static (bake into a XEX) ────────────────────────────────────
def apply_static(pf, xex_path, out_path, only=None, force=False, encrypt=False):
    """Write every enabled patch's ops into the XEX image at their VAs and
    re-emit a resigned XEX. `only` selects a single patch by name."""
    import xex
    import xexinject
    x = xex.open_xex(xex_path)
    image = bytearray(x.basefile())
    patches = [p for p in pf.patches
               if (p.name == only) or (only is None and p.is_enabled) or force]
    writes = 0
    for p in patches:
        for o in p.ops:
            _, off = xexinject._va_off(x, o.address)
            if 0 <= off <= len(image) - len(o.blob):
                image[off:off + len(o.blob)] = o.blob
                writes += 1
    xexinject.reemit(x, bytes(image), out_path, encrypt=encrypt)
    return writes, len(patches)


# ── apply: live (poke a running title over JRPC2) ──────────────────────
def apply_live(pf, ip, only=None, force=False):
    import xbdm
    import jrpc2
    cli = jrpc2.Jrpc2Client(xbdm.XbdmClient(ip))
    patches = [p for p in pf.patches
               if (p.name == only) or (only is None and p.is_enabled) or force]
    writes = 0
    for p in patches:
        for o in p.ops:
            cli.set_memory(o.address, o.blob)
            writes += 1
    return writes, len(patches)


# ── CLI ────────────────────────────────────────────────────────────────
def _print_info(pf):
    print("title   %s  [%s]" % (pf.title_name or "?", pf.title_id or "????????"))
    if pf.media_ids:
        print("media   %s" % ", ".join(pf.media_ids))
    print("patches %d (%d enabled)" % (len(pf.patches), len(pf.enabled())))
    for p in pf.patches:
        print("  %s %-40s %d write(s)%s" % (
            "[x]" if p.is_enabled else "[ ]", p.name, len(p.ops),
            ("  by %s" % p.author) if p.author else ""))


def main():
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage:\n"
              "  xeniapatch info   <file.patch.toml>\n"
              "  xeniapatch verify <file.patch.toml> <title.xex>\n"
              "  xeniapatch apply  <file.patch.toml> <title.xex> <out.xex> [--name NAME | --all]\n"
              "  xeniapatch live   <file.patch.toml> <console-ip>          [--name NAME | --all]")
        return
    op = a[0]
    if op == "info":
        _print_info(load(a[1])); return
    if op == "verify":
        import xex
        pf = load(a[1]); x = xex.open_xex(a[2])
        tid = (x.execution_info or {}).get("title_id", "").upper() if hasattr(x, "execution_info") else ""
        mid = (x.execution_info or {}).get("media_id", "").upper() if hasattr(x, "execution_info") else ""
        print("patch title_id %s  media_ids %s" % (pf.title_id, pf.media_ids))
        print("xex   title_id %s  media_id %s" % (tid, mid))
        tid_ok = (not pf.title_id) or (tid and tid.endswith(pf.title_id[-8:]))
        mid_ok = (not pf.media_ids) or (mid and any(mid.endswith(m[-8:]) for m in pf.media_ids))
        print("match: title_id=%s media_id=%s" % ("OK" if tid_ok else "NO", "OK" if mid_ok else "NO"))
        return
    only = None
    force = "--all" in a
    if "--name" in a:
        only = a[a.index("--name") + 1]
    if op == "apply":
        pf = load(a[1])
        w, n = apply_static(pf, a[2], a[3], only=only, force=force)
        print("applied %d write(s) from %d patch(es) -> %s" % (w, n, a[3])); return
    if op == "live":
        pf = load(a[1])
        w, n = apply_live(pf, a[2], only=only, force=force)
        print("poked %d write(s) from %d patch(es) -> %s" % (w, n, a[2])); return
    print("unknown op: %s" % op)


if __name__ == "__main__":
    main()
