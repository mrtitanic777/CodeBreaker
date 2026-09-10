#!/usr/bin/env python3
"""sysfiles -- Xbox 360 system-file toolkit (dashboard / NAND dumps).

Folds the re_tools corpus utilities into XboxTranspiler as native code, driven by
this project's own XEX reader (xex.py) instead of an external xextool. It maps a
folder of system files, decodes the non-XEX ones, graphs the module dependencies,
and flags homebrew provenance.

Commands:
    sysfiles triage  <dir>              inventory + classify every file
    sysfiles blob    <file>             decode a non-XEX file (patch / font / blob)
    sysfiles fonts   <dir> [outdir]     extract embedded sfnt fonts from .xtt
    sysfiles xref    <dir>              cross-module import dependency graph
    sysfiles genuine <dir>             separate homebrew from genuine Microsoft files
"""
DESCRIPTION = "Xbox 360 system files: triage, blob/patch/font decode, import xref, provenance"

import hashlib
import math
import os
import re
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import xex  # noqa: E402

# sfnt (TrueType/OpenType) container signatures
_SFNT = {b"\x00\x01\x00\x00": "TrueType (sfnt 1.0)", b"OTTO": "OpenType (CFF)",
         b"true": "TrueType (Apple)", b"ttcf": "TrueType Collection"}
_BLOB = {b"CRLP": "Certificate Revocation List package", b"DAEP": "DAE / dash blob"}
# strong homebrew provenance markers (chosen to avoid MS false positives like
# the legit function name "GetDashLaunchURI")
_HOMEBREW = re.compile(rb"DashLaunch V|launch\.ini|dlaunch:|DL3\.0|\\DL3|"
                       rb"xblaexitdash|could not patch dash|x360sdk\\plugins")


# --------------------------------------------------------------------- helpers
def fmt_size(n):
    if n < 1024:
        return "%dB" % n
    if n < 1024 * 1024:
        return "%.1fKB" % (n / 1024)
    return "%.1fMB" % (n / 1024 / 1024)


def entropy(data):
    if not data:
        return 0.0
    hist = [0] * 256
    for b in data:
        hist[b] += 1
    n = len(data)
    return -sum((c / n) * math.log2(c / n) for c in hist if c)


def sha1_file(path):
    h = hashlib.sha1()
    with open(path, "rb") as f:
        for chunk in iter(lambda: f.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def classify(path):
    """(kind, magic4). kind: xex, xex-patch, xtt-font, xtt-patch, crl, dae,
    patch-container, security-blob, data, unknown."""
    try:
        with open(path, "rb") as f:
            magic = f.read(8)
    except OSError:
        return "unknown", b""
    m4 = magic[:4]
    ext = os.path.splitext(path)[1].lower()
    if m4 == b"XEX2":
        flags = struct.unpack_from(">I", magic, 4)[0] if len(magic) >= 8 else 0
        return ("xex-patch" if (flags & 0x70) else "xex"), m4
    if m4 == b"xttf":
        return ("xtt-patch" if ext.startswith(".xttp") else "xtt-font"), m4
    if m4 in _BLOB:
        return ({b"CRLP": "crl", b"DAEP": "dae"}[m4]), m4
    if ext.startswith(".xexp"):
        return "patch-container", m4
    if ext == ".bin":
        return "security-blob", m4
    if ext == ".dat":
        return "data", m4
    return "unknown", m4


def _hexdump(data, length=64, base=0):
    out = []
    for i in range(0, min(length, len(data)), 16):
        chunk = data[i:i + 16]
        hexs = " ".join("%02x" % b for b in chunk)
        txt = "".join(chr(b) if 32 <= b < 127 else "." for b in chunk)
        out.append("  %08x  %-47s  %s" % (base + i, hexs, txt))
    return out


def _find_sfnt(data):
    for sig, desc in _SFNT.items():
        idx = data.find(sig, 4)          # skip the xttf magic region
        if idx != -1:
            return idx, desc
    return None, None


def _iter_files(folder):
    for name in sorted(os.listdir(folder)):
        p = os.path.join(folder, name)
        if os.path.isfile(p):
            yield p


# ------------------------------------------------------------------- commands
def cmd_triage(folder):
    files = list(_iter_files(folder))
    print("corpus: %s  (%d files)\n" % (folder, len(files)))
    print("  %-26s %-13s %-9s %-10s %s" % ("file", "kind", "size", "title", "version / note"))
    print("  " + "-" * 78)
    kinds = {}
    for p in files:
        kind, _ = classify(p)
        kinds[kind] = kinds.get(kind, 0) + 1
        note = title = ""
        if kind in ("xex", "xex-patch"):
            try:
                s = xex.open_xex(p).summary()
                title = s.get("title_id", "")
                if "delta" in s:
                    note = "%s -> %s" % (s["delta"]["source_version"], s["delta"]["target_version"])
                else:
                    note = "%s %s" % (s.get("version", ""), s.get("module_type", ""))
            except Exception as e:
                note = "parse error: %s" % e
        print("  %-26s %-13s %-9s %-10s %s"
              % (os.path.basename(p)[:26], kind, fmt_size(os.path.getsize(p)), title, note.strip()))
    print("\nkinds: " + ", ".join("%s=%d" % (k, v) for k, v in sorted(kinds.items())))


def cmd_blob(path):
    kind, m4 = classify(path)
    data = open(path, "rb").read()
    print("%s  (%s, %s)\n" % (os.path.basename(path), kind, fmt_size(len(data))))
    if kind in ("xex-patch", "patch-container") or m4 == b"XEX2":
        if m4 != b"XEX2":
            print("not a XEX2 container (magic %r) -- wrapped/encrypted update" % m4)
            print("header entropy %.2f bits/byte" % entropy(data[:4096]))
            print("\n".join(_hexdump(data, 48)))
            return
        x = xex.open_xex(path)
        d = x.delta_descriptor
        print("module type : %s  [%s]" % (x.module_type, ", ".join(x.module_flag_names())))
        if not d:
            print("no delta-patch descriptor (not a delta patch)")
            return
        print("source ver  : %s" % d["source_version"])
        print("target ver  : %s" % d["target_version"])
        print("base SHA-1  : %s" % d["source_digest"])
        print("image key   : %s" % d["image_key"])
        print("tgt hdr size: 0x%X" % d["target_header_size"])
        print("delta hdrs  : src 0x%X (0x%X) -> tgt 0x%X"
              % (d["delta_headers"]["src_off"], d["delta_headers"]["src_size"], d["delta_headers"]["tgt_off"]))
        print("delta image : src 0x%X (0x%X) -> tgt 0x%X"
              % (d["delta_image"]["src_off"], d["delta_image"]["src_size"], d["delta_image"]["tgt_off"]))
    elif kind in ("xtt-font", "xtt-patch"):
        idx, desc = _find_sfnt(data)
        hdr = " ".join("%08X" % struct.unpack_from(">I", data, i)[0] for i in range(0, min(32, len(data)), 4))
        print("XTT font container, header dwords: %s" % hdr)
        if idx is not None:
            print("embedded %s at 0x%X (%s to end)" % (desc, idx, fmt_size(len(data) - idx)))
        else:
            print("no embedded sfnt signature (proprietary glyph layout)")
    else:
        e = entropy(data)
        print("entropy %.2f bits/byte  %s" % (e, "(encrypted/compressed)" if e > 7.5
                                              else "(sparse/structured)" if e < 4.0 else ""))
        print("\n".join(_hexdump(data, 64)))


def cmd_fonts(folder, outdir=None):
    outdir = outdir or os.path.join(folder, "_fonts")
    n = 0
    for p in _iter_files(folder):
        kind, _ = classify(p)
        if kind not in ("xtt-font", "xtt-patch"):
            continue
        data = open(p, "rb").read()
        idx, desc = _find_sfnt(data)
        if idx is None:
            print("  %-24s no embedded sfnt" % os.path.basename(p))
            continue
        os.makedirs(outdir, exist_ok=True)
        dest = os.path.join(outdir, os.path.basename(p) + ".ttf")
        with open(dest, "wb") as f:
            f.write(data[idx:])
        n += 1
        print("  %-24s %s -> %s" % (os.path.basename(p), desc, dest))
    print("extracted %d font(s)%s" % (n, " -> " + outdir if n else ""))


def cmd_xref(folder):
    imports_of = {}
    for p in _iter_files(folder):
        kind, _ = classify(p)
        if kind not in ("xex", "xex-patch"):
            continue
        try:
            libs = sorted(xex.open_xex(p).imports().keys())
        except Exception:
            continue
        imports_of[os.path.basename(p)] = libs
    imported_by = {}
    for mod, libs in imports_of.items():
        for lib in libs:
            imported_by.setdefault(lib, set()).add(mod)
    print("cross-module imports (%d modules)\n" % len(imports_of))
    print("  %-22s imported by" % "module/lib")
    print("  " + "-" * 60)
    for lib in sorted(imported_by):
        users = sorted(imported_by[lib])
        print("  %-22s %s (%d)" % (lib, ", ".join(users), len(users)))


def _basefile_bytes(path, kind):
    if kind in ("xex", "xex-patch") or path.lower().endswith(".xex"):
        try:
            return xex.open_xex(path).basefile()
        except Exception:
            pass
    return open(path, "rb").read()


def cmd_genuine(folder):
    genuine, foreign = [], []
    for p in _iter_files(folder):
        kind, _ = classify(p)
        try:
            data = _basefile_bytes(p, kind)
        except Exception:
            data = b""
        markers = sorted({m.group(0).decode("latin-1") for m in _HOMEBREW.finditer(data)})
        (foreign if markers else genuine).append((os.path.basename(p), markers))
    print("provenance scan: %d genuine, %d foreign\n" % (len(genuine), len(foreign)))
    if foreign:
        print("FOREIGN (homebrew markers):")
        for name, m in foreign:
            print("  %-24s <- %s" % (name, ", ".join(m)))
        print()
    print("GENUINE Microsoft files (%d):" % len(genuine))
    for name, _ in genuine:
        print("  %s" % name)


# ------------------------------------------------------------------------ CLI
def main():
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print(__doc__.strip())
        return
    op, rest = a[0], a[1:]
    if op == "triage" and rest:
        cmd_triage(rest[0])
    elif op == "blob" and rest:
        cmd_blob(rest[0])
    elif op == "fonts" and rest:
        cmd_fonts(rest[0], rest[1] if len(rest) > 1 else None)
    elif op == "xref" and rest:
        cmd_xref(rest[0])
    elif op == "genuine" and rest:
        cmd_genuine(rest[0])
    else:
        print("usage: sysfiles {triage|blob|fonts|xref|genuine} <path>"); sys.exit(2)


if __name__ == "__main__":
    main()
