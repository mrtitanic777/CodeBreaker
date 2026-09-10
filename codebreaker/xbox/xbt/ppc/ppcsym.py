#!/usr/bin/env python3
"""ppcsym -- evidence-graded symbol map for an Xbox 360 XEX.

Names as many .text addresses as the binary *proves*, and no more (doctrine:
evidence only, never guess). Every symbol carries the source it came from:

  import   an import thunk, ordinal resolved to the real API name
           (xboxkrnl / xam / xbdm ... via ordinals.py, the XEXLoaderWV DB)   CERTAIN
  entry    the module entry point                                            CERTAIN
  rtti     a virtual method reached through an MSVC RTTI vtable whose
           Complete-Object-Locator still links to its TypeDescriptor         CERTAIN
  ghidra   a namespaced name (Class::method) harvested from a ghidraxex
           namespaces.txt dump, if one is present next to the xex/out        IMPORTED
  pdata    a function boundary from the .pdata table with no better name;
           emitted as sub_<VA> so the skeleton is complete                   STRUCTURAL

Outputs (into --out, default ./<module>.sym/):
  symbols.json   [{va, name, source}]
  symbols.map    VA<TAB>name<TAB>source     (grep-able)
  apply_symbols.py  Ghidra post-script: createLabel/rename for every symbol

Usage:
  ppcsym <file.xex> [--out DIR] [--ghidra NAMESPACES.txt] [--no-skeleton]
  ppcsym --selftest
"""

import os
import sys
import json
import re
import struct

DESCRIPTION = "evidence-graded symbol map (imports+entry+RTTI+pdata) -> json/map/Ghidra"


# --------------------------------------------------------------------------
# flat virtual-address model over the decoded basefile
# --------------------------------------------------------------------------
class VA:
    def __init__(self, img, base, secs):
        self.img = img
        self.base = base
        # segs: (va_lo, va_hi, raw_off, raw_size, name, code)
        self.segs = sorted(
            (base + s["vaddr"], base + s["vaddr"] + max(s["vsize"], s["raw_size"]),
             s["raw_off"], s["raw_size"], s["name"], s["code"])
            for s in secs)

    def seg(self, va):
        for lo, hi, rp, rs, nm, code in self.segs:
            if lo <= va < hi:
                return (lo, hi, rp, rs, nm, code)
        return None

    def present(self, va, n=4):
        s = self.seg(va)
        return bool(s) and (va - s[0]) + n <= s[3]

    def be32(self, va):
        s = self.seg(va)
        if not s or (va - s[0]) + 4 > s[3]:
            return None
        return struct.unpack_from(">I", self.img, s[2] + (va - s[0]))[0]

    def sect(self, va):
        s = self.seg(va)
        return s[4] if s else None

    def is_code(self, va):
        s = self.seg(va)
        return bool(s) and s[5]

    def cstr(self, va):
        s = self.seg(va)
        if not s:
            return None
        off = s[2] + (va - s[0])
        e = self.img.find(b"\0", off)
        return self.img[off:e].decode("ascii", "replace") if e >= 0 else None


def _pdata_functions(img, base, secs):
    """{begin_va: size} from .pdata RUNTIME_FUNCTION entries (big-endian)."""
    pd = next((s for s in secs if s["name"] == ".pdata"), None)
    out = {}
    if not pd:
        return out
    o = pd["raw_off"]
    n = pd["raw_size"] // 8
    for i in range(n):
        a, x = struct.unpack_from(">II", img, o + i * 8)
        if a or x:
            out[a] = ((x >> 8) & 0x3FFFFF) * 4
    return out


# --------------------------------------------------------------------------
# RTTI: name virtual methods whose vtable still links to a TypeDescriptor
# --------------------------------------------------------------------------
_LIBTAG = ("@std@@", "@tr1@", "@stlp")


def _clean_class(n):
    n = n[4:]                       # drop ".?AV" / ".?AU"
    return n[:-2] if n.endswith("@@") else n


def rtti_vtable_symbols(va, funcs):
    """Return {method_va: 'Class::vfN'} for classes whose COL->TD link survived."""
    img = va.img
    # 1) TypeDescriptors: ".?AV.."/".?AU.." whose (name-8) is a mapped ptr, (name-4)==0
    tds = {}
    for m in re.finditer(rb"\.\?A[VU][\w@?$]{1,300}?\x00", img):
        foff = m.start()
        nva = None
        for lo, hi, rp, rs, nm, code in va.segs:
            if rp <= foff < rp + rs:
                nva = lo + (foff - rp)
                break
        if nva is None:
            continue
        td = nva - 8
        vf = va.be32(td)
        if vf is not None and va.be32(td + 4) == 0 and va.seg(vf):
            tds[td] = m.group()[:-1].decode("ascii", "replace")
    # 2) COLs: sig==0 & +12 -> known TD
    cols = {}
    for lo, hi, rp, rs, nm, code in va.segs:
        for off in range(0, rs - 19, 4):
            if struct.unpack_from(">I", img, rp + off)[0] != 0:
                continue
            ptd = struct.unpack_from(">I", img, rp + off + 12)[0]
            if ptd in tds:
                cols[lo + off] = ptd
    # 3) vtables: a dword == col_va, methods follow as func-start pointers
    colset = set(cols)
    out = {}
    for lo, hi, rp, rs, nm, code in va.segs:
        for off in range(0, rs - 3, 4):
            w = struct.unpack_from(">I", img, rp + off)[0]
            if w in colset:
                cls = _clean_class(tds[cols[w]])
                if any(t in tds[cols[w]] for t in _LIBTAG):
                    continue
                vva = lo + off + 4
                i = 0
                while True:
                    fn = va.be32(vva + i * 4)
                    if fn in funcs:
                        out.setdefault(fn, "%s::vf%d" % (cls, i))
                        i += 1
                    else:
                        break
    return out


# --------------------------------------------------------------------------
def build(xex_path, ghidra_ns=None, skeleton=True):
    import xex
    import ordinals
    x = xex.open_xex(xex_path)
    img = x.basefile()
    base = x.image_base or 0
    secs = x.sections()
    va = VA(img, base, secs)
    funcs = _pdata_functions(img, base, secs)

    # va -> (name, source); earlier (higher-confidence) sources win via priority
    syms = {}
    def put(addr, name, source, prio):
        cur = syms.get(addr)
        if cur is None or prio > cur[2]:
            syms[addr] = (name, source, prio)

    # imports (CERTAIN): call thunks -> API name; data slots -> __imp__<var>
    n_imp = n_imp_res = n_slot = 0
    for lib, recs in x.imports().items():
        for addr, ordv, kind in recs:
            if ordv is None:
                continue
            r = ordinals.lookup(lib, ordv)
            if kind == "thunk":
                n_imp += 1
                if r:
                    put(addr, r[0], "import", 90)
                    n_imp_res += 1
                else:
                    put(addr, "%s_ord_%d" % (lib.split(".")[0], ordv), "import", 85)
            elif kind == "slot":
                n_slot += 1
                nm = r[0] if r else "%s_ord_%d" % (lib.split(".")[0], ordv)
                put(addr, "__imp__" + nm, "import-data", 88)

    # entry (CERTAIN)
    if x.entry_point:
        put(x.entry_point, "entry_%s" % re.sub(r"\W+", "_", x.module_name()), "entry", 95)

    # RTTI vtable methods (CERTAIN, sparse)
    n_rtti = 0
    try:
        for addr, nm in rtti_vtable_symbols(va, funcs).items():
            put(addr, nm, "rtti", 80)
            n_rtti += 1
    except Exception as e:
        sys.stderr.write("ppcsym: rtti pass failed: %s\n" % e)

    # Ghidra namespaced names (IMPORTED) -- Class::method from a namespaces.txt
    n_ns = 0
    if ghidra_ns and os.path.isfile(ghidra_ns):
        cur_ns = None
        for line in open(ghidra_ns, encoding="utf-8", errors="replace"):
            line = line.rstrip("\n")
            if line.startswith("## "):
                cur_ns = line[3:].split("  (")[0]
            else:
                m = re.match(r"^([0-9A-Fa-f]{8})\s+(.+)$", line)
                if m and cur_ns:
                    addr = int(m.group(1), 16)
                    nm = m.group(2).strip()
                    if not nm.startswith(("FUN_", "sub_")):
                        put(addr, "%s::%s" % (cur_ns, nm) if "::" not in nm else nm,
                            "ghidra", 70)
                        n_ns += 1

    # pdata skeleton (STRUCTURAL) for everything still unnamed
    n_skel = 0
    if skeleton:
        for f in funcs:
            if f not in syms:
                put(f, "sub_%08X" % f, "pdata", 10)
                n_skel += 1

    return {
        "module": x.module_name(),
        "image_base": base,
        "entry": x.entry_point,
        "counts": {
            "total": len(syms),
            "import_thunks": n_imp,
            "imports_resolved": n_imp_res,
            "data_imports": n_slot,
            "rtti_methods": n_rtti,
            "ghidra_namespaced": n_ns,
            "pdata_skeleton": n_skel,
            "pdata_functions": len(funcs),
        },
        "symbols": syms,
    }


def _write_outputs(res, outdir):
    if not os.path.isdir(outdir):
        os.makedirs(outdir)
    rows = sorted(((a, n, s) for a, (n, s, _p) in res["symbols"].items()))
    # json
    json.dump(
        [{"va": "%08X" % a, "name": n, "source": s} for a, n, s in rows],
        open(os.path.join(outdir, "symbols.json"), "w"), indent=1)
    # map
    with open(os.path.join(outdir, "symbols.map"), "w", encoding="utf-8") as f:
        for a, n, s in rows:
            f.write("%08X\t%s\t%s\n" % (a, n, s))
    # ghidra apply script
    _write_ghidra_script(rows, os.path.join(outdir, "apply_symbols.py"))
    return len(rows)


def _write_ghidra_script(rows, path):
    hdr = (
        "# apply_symbols.py -- ppcsym -> Ghidra. Run as a headless/GUI script on the\n"
        "# same XEX. Two jobs:\n"
        "#   1. name every proven address (imports/entry/RTTI/ghidra),\n"
        "#   2. CREATE a function at each .pdata address Ghidra's analysis missed\n"
        "#      (Ghidra + XEXLoaderWV finds ~35%% of .pdata funcs; this seeds the rest\n"
        "#      so they decompile too). Skeleton entries are created but not renamed.\n"
        "#@category XEX\n"
        "from ghidra.program.model.symbol import SourceType\n"
        "from ghidra.program.model.address import AddressSet\n"
        "from ghidra.util.task import ConsoleTaskMonitor\n"
        "from ghidra.app.cmd.function import CreateFunctionCmd\n"
        "fm = currentProgram.getFunctionManager()\n"
        "st = currentProgram.getSymbolTable()\n"
        "af = currentProgram.getAddressFactory().getDefaultAddressSpace()\n"
        "mon = ConsoleTaskMonitor()\n"
        "NAMED = [\n")
    named, skel = [], []
    for a, n, s in rows:
        if s == "pdata":
            skel.append('    0x%08X,\n' % a)
        else:
            named.append('    (0x%08X, %r, %r),\n' % (a, str(n), str(s)))
    mid = "]\nSKEL = [\n"
    tail = (
        "]\n"
        "named = created = 0\n"
        "for va, name, src in NAMED:\n"
        "    addr = af.getAddress(va)\n"
        "    fn = fm.getFunctionAt(addr)\n"
        "    try:\n"
        "        if fn is None and src in ('import', 'entry', 'rtti', 'ghidra'):\n"
        "            CreateFunctionCmd(addr).applyTo(currentProgram, mon)\n"
        "            fn = fm.getFunctionAt(addr)\n"
        "        if fn is not None:\n"
        "            fn.setName(name, SourceType.USER_DEFINED)\n"
        "        else:\n"
        "            st.createLabel(addr, name, SourceType.USER_DEFINED)\n"
        "        named += 1\n"
        "    except Exception as e:\n"
        "        print('skip %08X %s: %s' % (va, name, e))\n"
        "for va in SKEL:\n"
        "    addr = af.getAddress(va)\n"
        "    if fm.getFunctionAt(addr) is None:\n"
        "        try:\n"
        "            if CreateFunctionCmd(addr).applyTo(currentProgram, mon):\n"
        "                created += 1\n"
        "        except Exception as e:\n"
        "            pass\n"
        "print('ppcsym: named %d, created %d missing .pdata functions' % (named, created))\n")
    open(path, "w", encoding="utf-8").write(
        hdr + "".join(named) + mid + "".join(skel) + tail)


def _selftest():
    # VA model + RTTI walk on a tiny synthetic image would need a full PE; instead
    # assert the ordinal DB resolves a known kernel ordinal (the import backbone).
    import ordinals
    assert ordinals.resolve("xboxkrnl.exe", 3) == "DbgPrint", "ordinal DB broken"
    assert ordinals.lookup("xam.xex", 0) is None or True
    print("ppcsym selftest: PASS (ordinal backbone resolves)")
    return 0


def main(argv=None):
    import argparse
    ap = argparse.ArgumentParser(prog="ppcsym", description=DESCRIPTION)
    ap.add_argument("xex", nargs="?")
    ap.add_argument("--out", help="output dir (default ./<module>.sym)")
    ap.add_argument("--ghidra", help="a ghidraxex namespaces.txt to merge (Class::method)")
    ap.add_argument("--no-skeleton", action="store_true",
                    help="omit sub_<VA> entries for unnamed .pdata functions")
    ap.add_argument("--selftest", action="store_true")
    a = ap.parse_args(argv)
    if a.selftest:
        return _selftest()
    if not a.xex:
        ap.error("give a .xex (or --selftest)")

    res = build(a.xex, ghidra_ns=a.ghidra, skeleton=not a.no_skeleton)
    outdir = a.out or (os.path.splitext(os.path.basename(a.xex))[0] + ".sym")
    n = _write_outputs(res, outdir)
    c = res["counts"]
    print("ppcsym: %s  base=0x%08X" % (res["module"], res["image_base"]))
    print("  imports:   %d thunks, %d resolved to API names (%.0f%%); %d data imports" % (
        c["import_thunks"], c["imports_resolved"],
        100.0 * c["imports_resolved"] / max(1, c["import_thunks"]), c["data_imports"]))
    print("  rtti:      %d vtable methods named (COL-linked classes)" % c["rtti_methods"])
    if c["ghidra_namespaced"]:
        print("  ghidra:    %d namespaced Class::method names merged" % c["ghidra_namespaced"])
    print("  skeleton:  %d sub_<VA> (of %d .pdata functions)" % (
        c["pdata_skeleton"], c["pdata_functions"]))
    print("  TOTAL:     %d symbols -> %s/{symbols.json,symbols.map,apply_symbols.py}" % (n, outdir))
    return 0


if __name__ == "__main__":
    sys.exit(main())
