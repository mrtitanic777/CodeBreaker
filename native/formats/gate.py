#!/usr/bin/env python3
"""gate.py -- formats.dll against the Python readers: every file through both, every fact compared.

    python native/formats/gate.py [--readers pe,ne,...] [--corpus NAME_OR_DIR ...] [--limit N] [--show N]

Each file of each corpus (default: every corpus settings names) goes through a reader's Python
(codebreaker.formats.<reader>) and its C++ (formats.dll, through codebreaker.formats.native),
and every fact the reader gives is compared: every attribute the object carries and every
method's answer -- or, where the reader raises, the exception's name. Values are compared with
their types (a tuple is not a list, True is not 1) and dicts in their order. A file "accepted"
is one the reader opens without raising. Prints, per reader: files, accepted, identical, and
the seconds both sides took over the whole corpus (file reads included, each side reading).
"""
import argparse
import os
import struct
import sys
import time

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
sys.path.insert(0, ROOT)

from codebreaker.base import settings  # noqa: E402
from codebreaker.formats import native  # noqa: E402


def ename(e):
    return "struct.error" if isinstance(e, struct.error) else type(e).__name__


def canon(x):
    """x with its types spelled out, so == compares what Python's == lets slip (1 == True, (1,) != [1] ...)."""
    if x is None or isinstance(x, (bool, int, float, str, bytes)):
        return (type(x).__name__, x)
    if isinstance(x, bytearray):
        return ("bytearray", bytes(x))
    if isinstance(x, (list, tuple)):
        return (type(x).__name__, tuple(canon(v) for v in x))
    if isinstance(x, dict):
        return ("dict", tuple((canon(k), canon(v)) for k, v in x.items()))
    if isinstance(x, (set, frozenset)):
        return (type(x).__name__, tuple(sorted((canon(v) for v in x), key=repr)))
    if hasattr(x, "__dict__"):
        return ("object", type(x).__name__.replace("Native", ""), canon({k: v for k, v in vars(x).items() if not k.startswith("_")}))
    return ("other", repr(x))


SPENT = [0.0]               # seconds inside the readers (the comparison's own work not counted)


def timed(fn, *args):
    t0 = time.perf_counter()
    try:
        return fn(*args)
    finally:
        SPENT[0] += time.perf_counter() - t0


def ask(fn):
    try:
        return canon(timed(fn))
    except Exception as e:                      # noqa: BLE001 -- an exception is a fact
        return ("raise", ename(e))


# ---- the facts of each reader -----------------------------------------------------------------
def pe_facts(open_pe, path, _data):
    try:
        f = timed(open_pe, path)
    except Exception as e:                      # noqa: BLE001
        return False, [("open", ("raise", ename(e)))]
    out = [("attrs", canon({k: v for k, v in vars(f).items() if not k.startswith("_")}))]
    for m in ("machine_name", "is_dll", "code_bytes", "module_name", "export_addrs", "exports", "imports",
              "code_sections", "base_relocs", "build_meta", "debug_codeview", "optional_header", "export_table",
              "resource_tree", "reloc_blocks", "tls", "debug_entries", "certificates", "clr", "rich"):
        out.append((m, ask(getattr(f, m))))
    out.append(("import_table", ask(f.import_table)))
    out.append(("import_table(delay)", ask(lambda: f.import_table(delay=True))))
    rvas = {0, 1, f.entry, 0xFFFFFFFF, -1}
    for s in f.sections:
        rvas.update((s["vaddr"], s["vaddr"] + s["vsize"], s["vaddr"] + s["rawsize"] - 1))
    for va, _sz in f.datadirs[:16]:
        rvas.add(va)
    for r in sorted(rvas):
        out.append(("_rva_to_off(%d)" % r, ask(lambda: f._rva_to_off(r))))
        out.append(("file_offset(%d)" % r, ask(lambda: f.file_offset(r))))
    n = len(f.data)
    for off in (None, -1, 0, 0x3C, f.e_lfanew, n - 9, n - 1, n, n + 5):
        for fmt in "BHIQ":
            out.append(("_u(%s,%s)" % (fmt, off), ask(lambda: f._u(fmt, off))))
        out.append(("_text(%s)" % off, ask(lambda: f._text(off))))
        out.append(("_text(%s,4)" % off, ask(lambda: f._text(off, 4))))
    for i in list(range(17)) + [-1]:
        out.append(("directory(%d)" % i, ask(lambda: f.directory(i))))
    return True, out


def pe_all(path, data):
    from codebreaker.formats import pe
    ok, out = pe_facts(pe.open_pe, path, data)
    out.append(("is_dotnet", ask(lambda: pe.is_dotnet(path))))
    return ok, out


def ne_module_facts(f, prefix=""):
    from codebreaker.formats import ne
    out = [(prefix + "attrs", canon({k: v for k, v in vars(f).items() if not k.startswith("_")}))]
    for m in ("resident_names", "nonresident_names", "module_name", "module_refs", "entries", "export_ordinals",
              "resources", "segments"):
        out.append((prefix + m, ask(getattr(f, m))))
    try:
        segs = f.segments()
    except Exception:                           # noqa: BLE001
        segs = []
    for s in segs:
        out.append((prefix + "relocations(%d)" % s["idx"], ask(lambda: f.relocations(s))))
    out.append((prefix + "relocations(nodict)", ask(lambda: f.relocations({"flags": 0x100}))))
    h = f.h
    out.append((prefix + "imported_names", ask(lambda: [f.imported_name(f.u16(f.ne_off + h["mod_tab"] + 2 * i))
                                                        for i in range(h["n_modrefs"])])))
    for off in (-1, 0, 1, 5000, 10 ** 6):
        out.append((prefix + "imported_name(%d)" % off, ask(lambda: f.imported_name(off))))
    n = len(f.data)
    for off in (0, 0x3C, f.ne_off, n - 3, n - 2, n - 1, n, -2):
        out.append((prefix + "u16(%d)" % off, ask(lambda: f.u16(off))))
        out.append((prefix + "u32(%d)" % off, ask(lambda: f.u32(off))))
    out.append((prefix + "_read_name_table(ne_off)", ask(lambda: f._read_name_table(f.ne_off))))
    out.append((prefix + "_rsrc_string", ask(lambda: f._rsrc_string(f.ne_off + h["rsrc_tab"], 6))))
    out.append((prefix + "_tables_end", ask(lambda: ne._tables_end(f))))
    out.append((prefix + "standalone", ask(lambda: ne.standalone(f))))
    return out


def ne_all(path, data):
    from codebreaker.formats import ne
    out = []
    with open(path, "rb") as fh:
        whole = fh.read()
    out.append(("embedded", ask(lambda: ne.embedded(whole))))
    out.append(("embedded(0)", ask(lambda: ne.embedded(whole, 0))))
    try:
        f = timed(ne.NEFile, path)
        ok = True
        out += ne_module_facts(f)
    except Exception as e:                      # noqa: BLE001
        ok = False
        out.append(("open", ("raise", ename(e))))
    try:
        mods = timed(ne.embedded, whole)
    except Exception:                           # noqa: BLE001
        mods = []
    if mods:
        for off, _name in mods[:8]:             # each module read from its own header (no MZ stub)
            try:
                m = timed(ne.NEFile.from_buffer, whole, off, path)
                out += ne_module_facts(m, "buf%x." % off)
            except Exception as e:              # noqa: BLE001
                out.append(("buf%x" % off, ("raise", ename(e))))
        if path.lower().endswith(".bin"):       # a Windows 1.x boot image: modules over resident + overlay
            try:
                for name, m in timed(ne.open_boot_image, path).items():
                    out += ne_module_facts(m, "boot.%s." % name)
            except Exception as e:              # noqa: BLE001
                out.append(("boot", ("raise", ename(e))))
    return ok, out


def le_image_facts(lf, prefix):
    from codebreaker.formats import le
    out = [(prefix + "attrs", canon({k: v for k, v in vars(lf).items() if not k.startswith("_")}))]
    for m in ("fixups", "import_modules", "resident_name", "header", "description", "ddb"):
        out.append((prefix + m, ask(getattr(lf, m))))
    for pg in list(range(0, min(lf.mpages, 64) + 2)) + [lf.mpages, lf.mpages + 1, 10 ** 6]:
        out.append((prefix + "page_to_obj(%d)" % pg, ask(lambda: lf.page_to_obj(pg))))
    for f in ("header_fields", "is_member", "page_map", "names", "entries", "_entry_table_size", "exports",
              "fixup_records", "references", "pointers", "descriptor", "service_table", "entry_points",
              "service_calls"):
        out.append((prefix + f, ask(lambda: getattr(le, f)(lf))))
    for o in lf.objects[:16]:
        out.append((prefix + "object_bytes(%d)" % o["idx"], ask(lambda: lf.object_bytes(o))))
        out.append((prefix + "object_data(%d)" % o["idx"], ask(lambda: le.object_data(lf, o))))
        for off in (0, 1, o["vsize"] - 1, o["vsize"], 0x1234, -1):
            out.append((prefix + "file_offset(%d,%d)" % (o["idx"], off), ask(lambda: le.file_offset(lf, o["idx"], off))))
    out.append((prefix + "file_offset(0,0)", ask(lambda: le.file_offset(lf, 0, 0))))
    try:
        d = le.descriptor(lf)
    except Exception:                           # noqa: BLE001
        d = None
    if d:
        out.append((prefix + "service_table(d)", ask(lambda: le.service_table(lf, d))))
    try:
        recs = le.fixup_records(lf)
    except Exception:                           # noqa: BLE001
        recs = []
    out.append((prefix + "encode_fixup", ask(lambda: [le.encode_fixup(r) for _pg, r in recs[:400]])))
    out.append((prefix + "_names", ask(lambda: le._names(lf.d, lf.resnames))))
    out.append((prefix + "_plausible", ask(lambda: [le._plausible(lf.d, at) for at in (0, 4, lf.lfa, -1)])))
    out.append((prefix + "read_image", ask(lambda: le.read_image(lf))))
    out.append((prefix + "write_image", ask(lambda: le.write_image(le.read_image(lf)))))
    return out


def le_all(path, data):
    from codebreaker.formats import le
    with open(path, "rb") as fh:
        whole = fh.read()
    out = [("container", ask(lambda: le.container(whole)))]
    try:
        c = le.container(whole)
    except Exception:                           # noqa: BLE001
        c = None
    if c:
        out.append(("expand", ask(lambda: le.expand(whole))))
        if c["kind"] == "W4":                   # a page or two through the compressor and back
            plain = timed(le.expand, whole)
            at = c["at"]
            pages = [plain[at + 0x10 + k * 0x2000:at + 0x10 + (k + 1) * 0x2000].ljust(0x2000, b"\0") for k in range(2)]
            out.append(("compress_page", ask(lambda: [le.compress_page(p) for p in pages])))
            out.append(("expand_page", ask(lambda: [le.expand_page(le.compress_page(p)) for p in pages])))
        else:
            out.append(("pack_directory", ask(lambda: le.pack_directory(c))))
            w3 = timed(le.W3File, whole, c["at"])
            out.append(("w3.attrs", canon({k: v for k, v in vars(w3).items() if not k.startswith("_")})))
            out.append(("w3.modules", ask(lambda: [n for n, _lf in w3.modules()])))
    out.append(("open_vxds", ask(lambda: [n for n, _lf in le.open_vxds(path)])))
    out.append(("from_buffer", ask(lambda: le.LEFile.from_buffer(whole).lfa)))
    try:
        mods = timed(le.members, path)
        ok = bool(mods)
    except Exception as e:                      # noqa: BLE001
        out.append(("members", ("raise", ename(e))))
        return False, out
    out.append(("members", canon([n for n, _lf in mods])))
    for name, lf in mods[:40]:
        out += le_image_facts(lf, "%s." % name)
    return ok, out


def dos_all(path, data):
    from codebreaker.formats import dos
    with open(path, "rb") as fh:
        whole = fh.read()
    name = os.path.basename(path)
    out = [("kind", ask(lambda: dos.kind(path)))]
    for f, call in (("_kind", lambda: dos._kind(whole, name)), ("_mz", lambda: dos._mz(whole)),
                    ("_mz(stub)", lambda: dos._mz(whole, True)), ("chain", lambda: dos.chain(whole)),
                    ("driver_and_vxd", lambda: dos.driver_and_vxd(whole)),
                    ("dos_and_windows", lambda: dos.dos_and_windows(whole)), ("packer", lambda: dos.packer(path)),
                    ("devices", lambda: dos.devices(whole[:0x10000])),
                    ("devices(2)", lambda: dos.devices(whole[:0x10000], 2, (0x70,), 8))):
        out.append((f, ask(call)))
    h = dos._mz(whole)
    if h:
        out.append(("_packer", ask(lambda: dos._packer(whole, h))))
        out.append(("_kernel_body", ask(lambda: dos._kernel_body(whole, h))))
        if dos._packer(whole, h) == "EXEPACK":
            module = whole[h["header"]:h["size"]]
            out.append(("unpack_exepack", ask(lambda: dos.unpack_exepack(module, h["cs"], h["ip"]))))
        if dos._packer(whole, h) == "PKLITE":
            out.append(("pklite_head", ask(lambda: dos.pklite_head(path, 0x100))))
    if dos.driver_and_vxd(whole):
        out.append(("driver_part", ask(lambda: dos.driver_part(path))))
    if not dos._kind(whole, name):
        return False, out
    for movers in (True, False):
        try:
            p = timed(dos.read, path, movers)
        except Exception as e:                  # noqa: BLE001
            out.append(("read(%s)" % movers, ("raise", ename(e))))
            continue
        out.append(("read(%s)" % movers, canon(p)))
        if p.entries:
            out.append(("_data_segment(%s)" % movers, ask(lambda: dos._data_segment(p, p.entries[0][0]))))
        out.append(("unpacked_file(%s)" % movers, ask(lambda: dos.unpacked_file(p))))
    return True, out


def omf_module_facts(module, prefix):
    from codebreaker.formats import omf
    out = []
    for f in ("records", "names_of", "publics", "externs", "_code", "code_segment", "function_bytes"):
        out.append((prefix + f, ask(lambda: getattr(omf, f)(module))))
    out.append((prefix + "walk", ask(lambda: list(omf.walk(module)))))
    out.append((prefix + "walk(5)", ask(lambda: list(omf.walk(module, 5)))))
    try:
        o = timed(omf.read, module)
    except Exception as e:                      # noqa: BLE001
        out.append((prefix + "read", ("raise", ename(e))))
        return out
    out.append((prefix + "read", canon(o)))
    out.append((prefix + "code_segments", ask(lambda: omf.code_segments(o))))
    for s in [s for s, _c, _l, _a in o.segs][:12] + ["_TEXT", "NOSUCH"]:
        out.append((prefix + "holes(%s)" % s, ask(lambda: sorted(omf.holes(o, s)))))
        out.append((prefix + "functions(%s)" % s, ask(lambda: omf.functions(o, s))))
    out.append((prefix + "functions()", ask(lambda: omf.functions(o))))
    if o.ext:
        out.append((prefix + "rename_externs", ask(lambda: omf.rename_externs(module, {o.ext[0]: "RENAMED"}))))
    pubs = {n: off & 0xFFFF for n, _s, off in o.pubs[:40]}
    out.append((prefix + "raw_object", ask(lambda: omf.raw_object("T", module[:3000], pubs, module[:50], pubs))))
    out.append((prefix + "bss_object", ask(lambda: omf.bss_object({n: a + 0x100 for n, a in pubs.items()}, 0x100,
                                                                   0x100 + 0x10000 - 1, para=True))))
    out.append((prefix + "record", ask(lambda: omf.record(0x88, module[:300]))))
    out.append((prefix + "name", ask(lambda: [omf.name("ABC"), omf.name(b"xy")])))
    return out


def omf_all(path, data):
    from codebreaker.formats import omf
    with open(path, "rb") as fh:
        whole = fh.read()
    if not whole or whole[0] not in (0x80, 0x82, 0xF0) or os.path.splitext(path)[1].lower() not in (".obj", ".lib", ".o"):
        return False, [("skipped", ("str", "not an object or library"))]
    out = []
    try:
        mods = timed(omf.library, whole)
    except Exception as e:                      # noqa: BLE001
        return False, [("library", ("raise", ename(e)))]
    out.append(("library", canon([(n, len(b)) for n, b in mods])))
    for name, module in mods[:60]:
        out += omf_module_facts(module, name + ".")
    return True, out


def xex_all(path, data):
    from codebreaker.formats import xex
    with open(path, "rb") as fh:
        whole = fh.read()
    out = [("is_xex", ask(lambda: xex.is_xex(path))), ("ids", ask(lambda: xex.ids(whole))),
           ("optional_headers", ask(lambda: list(xex.optional_headers(whole))))]
    if not whole.startswith(b"XEX2"):
        return False, out
    out.append(("header", ask(lambda: xex.header(path))))
    return True, out


READERS = {"pe": pe_all, "ne": ne_all, "le": le_all, "dos": dos_all, "omf": omf_all,   # reader -> its facts(path, data)
           "xex": xex_all}


# ---- the corpus -------------------------------------------------------------------------------
def corpus_files(roots, limit=None):
    out = []
    for root in roots:
        for dp, dirs, files in os.walk(root):
            dirs.sort()
            for fn in sorted(files):
                out.append(os.path.join(dp, fn))
                if limit and len(out) >= limit:
                    return out
    return out


def roots_of(names):
    known = settings.corpora()
    kits = settings._load().get("kits") or {}       # the period toolkits: their objects and libraries
    for k, v in kits.items():
        known.setdefault("kit:" + k, v if os.path.isabs(v) else os.path.normpath(os.path.join(settings.ROOT, v)))
    if not names:
        return list(dict.fromkeys(v for k, v in known.items() if not k.startswith("kit:")))
    if names == ["kits"]:
        return [v for k, v in known.items() if k.startswith("kit:") and os.path.isdir(v)]
    return [known.get(n, n) for n in names]


CALLS = [0]                 # questions the C++ was asked (the Python side must ask none)
_real_call = native.call


FALLBACKS = {}             # the C++'s Fallback reasons (question: reason -> count): none once closed


def _counted_call(*a, **k):
    CALLS[0] += 1
    try:
        return _real_call(*a, **k)
    except native.Fallback as e:
        key = "%s: %s" % (a[0], e)
        FALLBACKS[key] = FALLBACKS.get(key, 0) + 1
        raise


native.call = _counted_call


def side(reader, on, facts, path):
    """facts(path) with the reader's switch set: on = the C++, off = the Python."""
    key = "CB_NATIVE_FORMATS_" + reader.upper()
    old = os.environ.get(key)
    os.environ[key] = "1" if on else "0"
    before = CALLS[0]
    try:
        return facts(path, None)
    finally:
        if not on and CALLS[0] != before:
            raise AssertionError("the Python side of %s asked the C++ (%s)" % (reader, path))
        if on:
            SIDE_CALLS[0] += CALLS[0] - before
        if old is None:
            del os.environ[key]
        else:
            os.environ[key] = old


SIDE_CALLS = [0]


def gate(reader, files, show=5):
    facts = READERS[reader]
    SIDE_CALLS[0] = 0
    accepted = identical = 0
    t_py = t_cpp = 0.0
    shown = 0
    for path in files:
        t0 = SPENT[0]
        ok_py, f_py = side(reader, False, facts, path)
        t1 = SPENT[0]
        ok_cpp, f_cpp = side(reader, True, facts, path)
        t2 = SPENT[0]
        t_py += t1 - t0
        t_cpp += t2 - t1
        accepted += ok_py
        if f_py == f_cpp:
            identical += 1
            continue
        if shown < show:
            shown += 1
            a, b = dict(f_py), dict(f_cpp)
            diff = [k for k in list(a) + [k for k in b if k not in a] if a.get(k) != b.get(k)]
            print("  DIFF %s: %s" % (path, ", ".join(diff[:6])))
            for k in diff[:2]:
                print("     py : %.300r" % (a.get(k),))
                print("     c++: %.300r" % (b.get(k),))
    return {"files": len(files), "accepted": accepted, "identical": identical, "py": t_py, "cpp": t_cpp,
            "asked": SIDE_CALLS[0]}


def main(argv=None):
    ap = argparse.ArgumentParser()
    ap.add_argument("--readers", default="")
    ap.add_argument("--corpus", nargs="*", default=[])
    ap.add_argument("--limit", type=int, default=None)
    ap.add_argument("--show", type=int, default=5)
    ap.add_argument("--ext", default="")
    args = ap.parse_args(argv)
    if native.why_not():
        print("formats.dll not usable:", native.why_not())
        return 2
    readers = [r for r in args.readers.split(",") if r] or list(READERS)
    roots = roots_of(args.corpus)
    files = corpus_files(roots, None if args.ext else args.limit)
    if args.ext:                                    # only files of these extensions (a library of games: its .xex)
        files = [f for f in files if os.path.splitext(f)[1].lower() in args.ext.split(",")][:args.limit]
    print("corpus: %d files from %s" % (len(files), ", ".join(roots)))
    print("| reader | files | accepted | identical | Python s | C++ s | questions asked of the C++ |")
    print("|---|---|---|---|---|---|---|")
    bad = 0
    for r in readers:
        res = gate(r, files, args.show)
        bad += res["files"] - res["identical"] + (0 if res["asked"] else 1)
        print("| %s | %d | %d | %d | %.2f | %.2f | %d |" % (r, res["files"], res["accepted"], res["identical"],
                                                            res["py"], res["cpp"], res["asked"]))
        sys.stdout.flush()
    for k, n in sorted(FALLBACKS.items()):
        print("  fallback x%d  %s" % (n, k))
    bad += sum(FALLBACKS.values())
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
