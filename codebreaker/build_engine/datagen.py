#!/usr/bin/env python3
r"""
datagen.py -- emit a module's initialized data as reassembleable `db` source and
PROVE it round-trips: assemble the emitted source with period MASM and byte-compare
to the shipped data. This is the mechanical completion of the from-source build --
data is content, not structure, so "reconstruction" here is faithful transcription
to `db`, exactly like the code db-fallback. The point is that the emitted source
rebuilds the bytes WITHOUT the original binary, which `verify()` demonstrates.

    python datagen.py MODULE          # emit + assemble-verify each data section
    python datagen.py MODULE --emit DIR   # also write the .asm data source
"""
import os
from codebreaker.foundation import cores
import sys

ROOT = os.path.dirname(os.path.abspath(__file__))
sys.path.append(ROOT)
def _emit_db(data, per=16):
    """Data bytes -> MASM `db` lines (byte-exact, reassembleable)."""
    out = []
    for i in range(0, len(data), per):
        row = data[i:i + per]
        out.append("        db " + ",".join("0%02xh" % b for b in row))
    return "\n".join(out)


def data_sections(module):
    """[(name, bytes)] for the initialized-data sections of `module` (PE)."""
    target = cores.get('target')
    from codebreaker.format_loaders import pe
    path = target.resolve(module)
    if target.format_of(path) != "PE":
        return []
    pf = pe.open_pe(path)
    raw = open(path, "rb").read()
    out = []
    for s in pf.sections:
        nm = s["name"].lower().lstrip(".")
        if (s["chars"] & 0x20) or (s["chars"] & 0x20000000):
            continue
        if nm.startswith(("rsrc", "reloc", "idata", "edata")):
            continue
        if s["rawsize"] == 0:                        # .bss: no file bytes
            continue
        out.append((s["name"], raw[s["rawptr"]:s["rawptr"] + s["rawsize"]]))
    return out


def verify(module, emitdir=None):
    """Emit `db` source for each data section, assemble it with MASM, and compare
    the assembled bytes to the shipped data. Returns {section: (match, nbytes)}."""
    masmlift = cores.get('masmlift')
    asmrecon32 = cores.get('asmrecon32')
    from codebreaker.format_loaders import objextract
    secs = data_sections(module)
    if not secs:
        return {}
    items, meta = [], []
    for i, (name, data) in enumerate(secs):
        src = ("        .386\n_DATA segment dword public use32 'DATA'\n"
               + _emit_db(data) + "\n_DATA ends\n        end\n")
        items.append(("D%d" % i, src))
        meta.append((name, data))
        if emitdir:
            os.makedirs(emitdir, exist_ok=True)
            with open(os.path.join(emitdir, "%s.asm" % name.lstrip(".")), "w") as f:
                f.write(src)
    res = masmlift.assemble_many(items, masmroot=asmrecon32.MASM32ROOT).get("results", {})
    out = {}
    for i, (name, data) in enumerate(meta):
        obj = res.get("D%d" % i, {}).get("obj")
        if not obj:
            out[name] = (False, len(data))
            continue
        o = objextract.Obj(obj)
        asm = bytes(o.seg_bytes.get("_DATA", b""))
        out[name] = (asm[:len(data)] == data, len(data))
    return out


def main():
    if len(sys.argv) < 2:
        sys.exit("usage: datagen.py MODULE [--emit DIR]")
    os.chdir(os.path.dirname(ROOT) if os.path.basename(ROOT) == "tools" else ".")
    from codebreaker.foundation import config
    config.apply_config()
    emitdir = sys.argv[sys.argv.index("--emit") + 1] if "--emit" in sys.argv else None
    r = verify(sys.argv[1], emitdir)
    if not r:
        print("%s: no initialized data sections" % sys.argv[1]); return
    tot = ok = 0
    for name, (match, nb) in r.items():
        tot += nb
        if match:
            ok += nb
        print("  %-10s %8d bytes  %s" % (name, nb, "ASSEMBLES BYTE-EXACT" if match else "DIFF"))
    print("  data reassembled from db source: %d / %d bytes" % (ok, tot))


if __name__ == "__main__":
    main()
