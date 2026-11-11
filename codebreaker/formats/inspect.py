r"""inspect.py -- everything an inspector shows about an executable, as one plain dict.

    python -m codebreaker info <file-or-module>                  the summary, headers and sections
    python -m codebreaker info <file-or-module> --section NAME   one part (see SECTIONS; `all` for every part)
    python -m codebreaker info <file-or-module> --json           the whole dict as JSON
    python -m codebreaker info <file> --section strings [--min N]
    python -m codebreaker info <file> --section hex [--offset N] [--length N]

    from codebreaker.formats import inspect
    info = inspect.describe(path)        # the same keys whatever the file is
    inspect.strings(path)                # [{offset, address, kind, category, text}]
    inspect.hexdump(path, 0x400, 256)    # the classic offset / hex / text listing
    inspect.resource_body(path, item)    # a resource shown: an image, text, or a hex listing

One description for every kind of file the corpus holds -- 32-bit Windows (PE), 16-bit Windows
(NE), virtual device drivers (LE/LX, and the W3 bundle of them), plain DOS programs (MZ):

    format, kind, name, path, size, ok, notes     what it is; `notes` lists parts that were damaged
    summary        the handful of facts worth a first look, the same keys for every format
    dos            the DOS header every one of these starts with
    headers        the format's own header fields
    sections       sections / segments / objects: name, address, size, file_offset, file_size, flags, code
    exports        {module, count, symbols: [{name, ordinal, hint, address, forward}]}
    imports        [{module, count, delay, symbols: [{name, ordinal, hint}]}]
    resources      {count, types: [{type, count, items: [{name, language, offset, size, type_id, preview}]}]}
    directories, relocations, tls, debug, certificates, clr, rich, load_config     (PE)
    driver, members                                                             (drivers)

The file is read by the format readers (pe.py, ne.py, le.py, detect.py); nothing is parsed a
second time here. A part that cannot be read is left empty and named in `notes`; describing a
file never raises.
"""
import json
import os
import re
import struct
import time

from codebreaker.base import corpus
from codebreaker.formats import detect, le, native, ne, pe, resources

SECTIONS = ("summary", "dos", "headers", "directories", "sections", "exports", "imports", "resources",
            "relocations", "tls", "debug", "certificates", "clr", "rich", "driver", "members",
            "strings", "hex")
DIRECTORIES = ("Export", "Import", "Resource", "Exception", "Security", "BaseReloc", "Debug",
               "Architecture", "GlobalPtr", "TLS", "LoadConfig", "BoundImport", "IAT", "DelayImport",
               "CLR", "Reserved")
RESOURCE_TYPES = {1: "CURSOR", 2: "BITMAP", 3: "ICON", 4: "MENU", 5: "DIALOG", 6: "STRING", 7: "FONTDIR",
                  8: "FONT", 9: "ACCELERATOR", 10: "RCDATA", 11: "MESSAGETABLE", 12: "GROUP_CURSOR",
                  14: "GROUP_ICON", 16: "VERSION", 17: "DLGINCLUDE", 19: "PLUGPLAY", 20: "VXD",
                  21: "ANICURSOR", 22: "ANIICON", 23: "HTML", 24: "MANIFEST"}
_TYPE_IDS = {v: k for k, v in RESOURCE_TYPES.items()}
_IMAGES, _TEXTS = (1, 2, 3), (6, 10, 16, 23, 24)
_FILE_FLAGS = ((0x0001, "NO_RELOCS"), (0x0002, "EXECUTABLE"), (0x0020, "LARGE_ADDRESS"), (0x0100, "32BIT"),
               (0x0200, "NO_DEBUG"), (0x1000, "SYSTEM"), (0x2000, "DLL"))
_SECTION_FLAGS = ((0x20, "CODE"), (0x40, "INITDATA"), (0x80, "UNINITDATA"), (0x02000000, "DISCARDABLE"),
                  (0x04000000, "NOTCACHED"), (0x08000000, "NOTPAGED"), (0x10000000, "SHARED"),
                  (0x20000000, "EXECUTE"), (0x40000000, "READ"), (0x80000000, "WRITE"))
_NE_FLAGS = ((0x0001, "SINGLE_DATA"), (0x0002, "MULTIPLE_DATA"), (0x0004, "GLOBAL_INIT"),
             (0x0008, "PROTECTED_ONLY"), (0x0800, "SELF_LOADING"), (0x2000, "LINK_ERRORS"), (0x8000, "LIBRARY"))
_SEGMENT_FLAGS = ((0x0001, "DATA"), (0x0010, "MOVABLE"), (0x0020, "SHAREABLE"), (0x0040, "PRELOAD"),
                  (0x0080, "READ_ONLY"), (0x0100, "RELOCINFO"), (0x1000, "DISCARDABLE"))
_OBJECT_FLAGS = ((0x0001, "READ"), (0x0002, "WRITE"), (0x0004, "EXECUTE"), (0x0008, "RESOURCE"),
                 (0x0010, "DISCARDABLE"), (0x0020, "SHARED"), (0x0040, "PRELOAD"), (0x2000, "32BIT"))
_NE_SYSTEMS = {0: "unknown", 1: "OS/2", 2: "Windows", 3: "DOS 4", 4: "Windows 386"}
_DEBUG_TYPES = {0: "Unknown", 1: "COFF", 2: "CodeView", 3: "FPO", 4: "Misc", 5: "Exception", 6: "Fixup",
                9: "Borland", 12: "VC feature", 16: "Repro"}


def _flags(value, table):
    return [name for bit, name in table if value & bit]


def _when(stamp):
    """A link timestamp as text, or '' when it is not a date (0, or a hash as newer linkers write)."""
    try:
        return time.strftime("%Y-%m-%d %H:%M:%S", time.gmtime(stamp)) if 0 < stamp < 0x7FFFFFFF else ""
    except (OverflowError, OSError, ValueError):
        return ""


def _preview(type_id):
    return "image" if type_id in _IMAGES else "text" if type_id in _TEXTS else "hex"


class _Parts(object):
    """Fills in a description one part at a time; a part that fails is noted, not raised."""

    def __init__(self, info):
        self.info = info

    def __call__(self, key, make, default=None):
        try:
            self.info[key] = make()
        except Exception as e:                                # a damaged table must not hide the rest
            self.info[key] = default
            self.info["notes"].append("%s: could not be read (%s: %s)" % (key, type(e).__name__, str(e)[:80]))
        return self.info[key]


# ---- 32-bit Windows --------------------------------------------------------------------------
def _pe(path, info, part):
    f = pe.open_pe(path)
    opt = part("headers", lambda: {
        "file": {"machine": f.machine_name(), "machine_id": f.machine, "sections": f.n_sections,
                 "timestamp": f.timestamp, "linked": _when(f.timestamp),
                 "optional_header_size": f.opt_size, "characteristics": _flags(f.characteristics, _FILE_FLAGS)},
        "optional": f.optional_header()}, {"file": {}, "optional": {}})["optional"]
    part("directories", lambda: [{"name": DIRECTORIES[i] if i < 16 else str(i), "rva": va, "size": size}
                                 for i, (va, size) in enumerate(f.datadirs[:16])], [])
    part("sections", lambda: [{"name": s["name"], "address": f.image_base + s["vaddr"], "size": s["vsize"],
                               "file_offset": s["rawptr"], "file_size": s["rawsize"],
                               "flags": _flags(s["chars"], _SECTION_FLAGS),
                               "code": bool(s["chars"] & 0x20000020)} for s in f.sections], [])

    def exports():
        t = f.export_table()
        for s in t["symbols"]:
            s["address"] = f.image_base + s.pop("rva")
        return {"module": t["module"], "count": len(t["symbols"]), "ordinal_base": t["ordinal_base"],
                "symbols": t["symbols"]}

    def imports():
        return [{"module": m["module"], "count": len(m["symbols"]), "delay": delay, "symbols": m["symbols"]}
                for delay in (False, True) for m in f.import_table(delay)]

    def resources():
        types = []
        for t in f.resource_tree():
            tid = t["id"] if t["id"] is not None else -1
            for it in t["items"]:
                it.update(type_id=tid, preview=_preview(tid), wide=True)
            types.append({"type": RESOURCE_TYPES.get(t["type"], str(t["type"])) if t["id"] is not None else t["type"],
                          "id": t["id"], "count": len(t["items"]), "items": t["items"]})
        return {"count": sum(t["count"] for t in types), "types": types}

    def relocations():
        blocks = f.reloc_blocks()
        return {"blocks": len(blocks), "entries": sum(n for _p, n in blocks), "addresses": len(f.base_relocs())}

    ex = part("exports", exports, {"module": "", "count": 0, "symbols": []})
    im = part("imports", imports, [])
    rs = part("resources", resources, {"count": 0, "types": []})
    part("relocations", relocations, {"blocks": 0, "entries": 0, "addresses": 0})
    part("tls", f.tls)
    part("debug", lambda: [dict(e, type=_DEBUG_TYPES.get(e["type"], str(e["type"])), linked=_when(e["timestamp"]))
                           for e in f.debug_entries()], [])
    part("certificates", f.certificates, [])
    part("clr", f.clr)
    part("rich", f.rich)
    part("load_config", lambda: f.directory(10)[1] > 0, False)
    part("symbols_file", f.debug_codeview)
    info["summary"] = {
        "format": "PE32+" if f.is_plus else "PE32", "type": "DLL" if f.is_dll() else "EXE",
        "machine": f.machine_name(), "module": ex["module"], "description": "",
        "subsystem": opt.get("subsystem_name", ""), "linker": opt.get("linker_version", ""),
        "linked": _when(f.timestamp), "image_base": f.image_base,
        "entry": f.image_base + f.entry if f.entry else 0, "code_bytes": f.code_bytes(),
        "sections": len(f.sections), "exports": ex["count"], "import_modules": len(im),
        "imports": sum(m["count"] for m in im), "resources": rs["count"]}


# ---- 16-bit Windows --------------------------------------------------------------------------
def _ne(path, info, part):
    f = ne.NEFile(path)
    h = f.h
    part("headers", lambda: {
        "linker_version": "%d.%d" % (h["linker_ver"], h["linker_rev"]), "flags": _flags(h["flags"], _NE_FLAGS),
        "flags_value": h["flags"], "automatic_data_segment": h["autodata_seg"], "heap": h["heap"],
        "stack": h["stack"], "entry_segment": h["csip"] >> 16, "entry_offset": h["csip"] & 0xFFFF,
        "stack_segment": h["sssp"] >> 16, "stack_pointer": h["sssp"] & 0xFFFF, "segments": h["n_segs"],
        "module_references": h["n_modrefs"], "movable_entries": h["n_movable"],
        "alignment_shift": h["align_shift"], "target_system": _NE_SYSTEMS.get(h["exetype"], str(h["exetype"])),
        "expected_windows": ("%d.%d" % (h["expect_winver"] >> 8, h["expect_winver"] & 0xFF))
        if h["expect_winver"] else "", "checksum": h["crc"]}, {})
    segs = part("sections", lambda: [{
        "name": "seg %d" % s["idx"], "address": "%d:0000" % s["idx"], "size": s["minalloc"],
        "file_offset": s["file_off"], "file_size": s["length"], "flags": _flags(s["flags"], _SEGMENT_FLAGS),
        "code": s["kind"] == "CODE"} for s in f.segments()], [])

    def names():
        res, non = f.resident_names(), f.nonresident_names()
        return (res[0][0] if res else ""), (non[0][0] if non else ""), {o: n for n, o in res[1:] + non[1:]}

    module, description, by_ordinal = part("_names", names, ("", "", {}))
    del info["_names"]

    def exports():
        symbols, seen = [], set()
        for e in f.entries():
            if e["kind"] == "UNUSED":
                continue
            seen.add(e["ordinal"])
            symbols.append({"name": by_ordinal.get(e["ordinal"], ""), "ordinal": e["ordinal"], "hint": -1,
                            "address": "%d:%04x" % (e["seg"], e["offset"]), "forward": "",
                            "movable": e["kind"] == "MOVEABLE", "exported": bool(e["flags"] & 1)})
        for o, n in sorted(by_ordinal.items()):                 # named, but with no entry of its own
            if o not in seen:
                symbols.append({"name": n, "ordinal": o, "hint": -1, "address": "", "forward": "",
                                "movable": False, "exported": False})
        return {"module": module, "count": len(symbols), "symbols": symbols}

    def imports():
        mods = f.module_refs()
        used = [(set(), set()) for _ in mods]
        for s in f.segments():
            for r in f.relocations(s):
                if r["kind"] in ("IMPORTORD", "IMPORTNAME") and 1 <= r["module"] <= len(mods):
                    if r["kind"] == "IMPORTORD":
                        used[r["module"] - 1][0].add(r["ordinal"])
                    else:
                        used[r["module"] - 1][1].add(f.imported_name(r["name_off"]))
        out = []
        for m, (ords, by_name) in zip(mods, used):
            symbols = [{"name": "", "ordinal": o, "hint": -1} for o in sorted(ords)] + \
                      [{"name": n, "ordinal": -1, "hint": -1} for n in sorted(by_name)]
            out.append({"module": m, "count": len(symbols), "delay": False, "symbols": symbols})
        return out

    def resources():
        types = {}
        for r in f.resources():
            tname = r["type"] or "?"
            tid = _TYPE_IDS.get(tname, int(tname[1:]) if re.fullmatch(r"#\d+", tname) else -1)
            types.setdefault(tname, []).append({
                "name": r["id"], "language": 0, "offset": r["off"], "size": r["length"],
                "type_id": tid, "preview": _preview(tid), "wide": False})
        out = [{"type": t, "id": items[0]["type_id"] if items[0]["type_id"] >= 0 else None,
                "count": len(items), "items": items} for t, items in types.items()]
        return {"count": sum(t["count"] for t in out), "types": out}

    ex = part("exports", exports, {"module": module, "count": 0, "symbols": []})
    im = part("imports", imports, [])
    rs = part("resources", resources, {"count": 0, "types": []})
    part("relocations", lambda: {"records": sum(len(f.relocations(s)) for s in f.segments())}, {"records": 0})
    info["summary"] = {
        "format": "NE", "type": "DLL" if h["flags"] & 0x8000 else "EXE", "machine": "i286/i386 (16-bit)",
        "module": module, "description": description,
        "subsystem": _NE_SYSTEMS.get(h["exetype"], str(h["exetype"])),
        "linker": "%d.%d" % (h["linker_ver"], h["linker_rev"]), "linked": "", "image_base": 0,
        "entry": "%d:%04x" % (h["csip"] >> 16, h["csip"] & 0xFFFF),
        "code_bytes": sum(s["file_size"] for s in segs if s["code"]), "sections": len(segs),
        "exports": ex["count"], "import_modules": len(im), "imports": sum(m["count"] for m in im),
        "resources": rs["count"]}


# ---- drivers ---------------------------------------------------------------------------------
def _driver(f, name, info, part):
    """One LE/LX image: a driver file, or one member of a W3 bundle."""
    head = part("headers", lambda: dict(
        f.header(), cpu_name={1: "80286", 2: "80386", 3: "80486", 4: "Pentium"}.get(f.cpu, str(f.cpu)),
        os_name={1: "OS/2", 2: "Windows", 3: "DOS 4", 4: "Windows 386"}.get(f.ostype, str(f.ostype))), {})
    objs = part("sections", lambda: [{
        "name": "object %d" % o["idx"], "address": o["base"], "size": o["vsize"],
        "file_offset": f.datapages + (o["pmidx"] - 1) * f.pagesize, "file_size": o["pcnt"] * f.pagesize,
        "flags": _flags(o["flags"], _OBJECT_FLAGS), "code": o["exec"]} for o in f.objects], [])

    def imports():
        mods = f.import_modules()
        used = [set() for _ in mods]
        try:
            for fx in f.fixups().values():
                if fx and fx["kind"] == "import" and 1 <= fx["module"] <= len(mods):
                    used[fx["module"] - 1].add(fx["ordinal"])
        except Exception as e:
            info["notes"].append("imports: the fixup records could not all be read (%s)" % type(e).__name__)
        return [{"module": m, "count": len(u), "delay": False,
                 "symbols": [{"name": "", "ordinal": o, "hint": -1} for o in sorted(u)]}
                for m, u in zip(mods, used)]

    im = part("imports", imports, [])
    ddb = part("driver", f.ddb)
    module = part("_name", f.resident_name, "")
    description = part("_text", f.description, "")
    del info["_name"], info["_text"]
    info["exports"] = {"module": module, "count": 0, "symbols": []}
    info["resources"] = {"count": 0, "types": []}
    info["summary"] = {
        "format": f.sig.decode("latin1"), "type": "driver", "machine": head.get("cpu_name", ""),
        "module": (ddb or {}).get("name") or module or name, "description": description,
        "subsystem": head.get("os_name", ""), "linker": "", "linked": "", "image_base": 0,
        "entry": "%d:%08x" % (f.eip_obj, f.eip) if f.eip_obj else 0,
        "code_bytes": sum(o["size"] for o in objs if o["code"]), "sections": len(objs), "exports": 0,
        "import_modules": len(im), "imports": sum(m["count"] for m in im), "resources": 0}


def _le(path, info, part):
    with open(path, "rb") as fh:
        _driver(le.LEFile.from_buffer(fh.read()), info["name"], info, part)


def _bundle(data, info, part):
    """A W3 file (vmm32.vxd): several drivers packed one after another."""
    w3 = le.W3File(data, struct.unpack_from("<I", data, 0x3C)[0])
    members = []
    for e in w3.entries:
        sub = {"name": e["name"], "offset": e["off"], "size": e["size"], "notes": []}
        if data[e["off"]:e["off"] + 2] in (b"LE", b"LX"):
            try:
                _driver(le.LEFile(data, e["off"]), e["name"], sub, _Parts(sub))
            except Exception as err:
                sub["notes"].append("%s: %s" % (type(err).__name__, str(err)[:80]))
        members.append(sub)
    info["members"] = members
    info["headers"] = {"version": w3.version, "members": w3.count}
    info["sections"] = [{"name": m["name"], "address": 0, "size": m["size"], "file_offset": m["offset"],
                         "file_size": m["size"], "flags": [], "code": True} for m in members]
    info["exports"] = {"module": "", "count": 0, "symbols": []}
    info["imports"], info["resources"] = [], {"count": 0, "types": []}
    done = [m["summary"] for m in members if "summary" in m]
    info["summary"] = {
        "format": "W3", "type": "driver bundle", "machine": "80386", "module": info["name"],
        "description": "%d drivers in one file" % len(members), "subsystem": "Windows 386", "linker": "",
        "linked": "", "image_base": 0, "entry": 0, "code_bytes": sum(s["code_bytes"] for s in done),
        "sections": len(members), "exports": 0, "import_modules": 0,
        "imports": sum(s["imports"] for s in done), "resources": 0}


# ---- DOS -------------------------------------------------------------------------------------
def _dos(data, info, part):
    h = info["dos"] or {}
    start, size = h.get("header_size", 0), h.get("load_size", 0)
    info["headers"] = dict(h)
    info["sections"] = [{"name": "program", "address": "0000:0000", "size": size + h.get("min_alloc", 0) * 16,
                         "file_offset": start, "file_size": size, "flags": [], "code": True}]
    extra = len(data) - start - size
    if extra > 0:
        info["sections"].append({"name": "after the program", "address": "", "size": extra,
                                 "file_offset": start + size, "file_size": extra, "flags": [], "code": False})
    info["relocations"] = {"records": h.get("relocations", 0),
                           "entries": ["%04x:%04x" % e for e in info.pop("_relocs", [])[:512]]}
    info["exports"] = {"module": "", "count": 0, "symbols": []}
    info["imports"], info["resources"] = [], {"count": 0, "types": []}
    info["summary"] = {
        "format": "MZ", "type": "DOS program", "machine": "8086 (16-bit)", "module": info["name"],
        "description": "", "subsystem": "DOS", "linker": "", "linked": "", "image_base": 0,
        "entry": "%04x:%04x" % (h.get("cs", 0), h.get("ip", 0)), "code_bytes": size, "sections": 1,
        "exports": 0, "import_modules": 0, "imports": 0, "resources": 0}


def describe(path):
    """Everything known about the file at `path` (see the module docstring for the keys)."""
    info = {"path": path, "name": os.path.basename(path).lower(), "size": 0, "format": None, "kind": None,
            "ok": False, "notes": [], "summary": {}, "dos": None, "headers": {}, "sections": [],
            "exports": {"module": "", "count": 0, "symbols": []}, "imports": [],
            "resources": {"count": 0, "types": []}}
    fmt = detect.format_of(path) if os.path.isfile(path) else None
    if fmt is None:                                          # not an executable: what it is, from its header
        return _other(path, info)
    try:
        with open(path, "rb") as fh:
            data = fh.read()
    except OSError as e:
        info["notes"].append("cannot be read: %s" % e)
        return info
    info["size"] = len(data)
    info["format"], info["kind"] = fmt, detect.lane(path)
    part = _Parts(info)
    try:
        h = detect.dos_header(data) or {}
        info["_relocs"] = h.pop("relocation_entries", [])
        info["dos"] = h
        if fmt == "PE":
            _pe(path, info, part)
        elif fmt == "NE":
            _ne(path, info, part)
        elif fmt in ("LE", "LX"):
            _le(path, info, part)
        elif fmt == "W3":
            _bundle(data, info, part)
        else:
            _dos(data, info, part)
        info["ok"] = True
    except Exception as e:                                    # the header itself is damaged
        info["notes"].append("%s: %s" % (type(e).__name__, str(e)[:120]))
    info.pop("_relocs", None)
    return info


def _other(path, info):
    """A file that is not an MZ executable: what detect.identify says it is, with what the format's
    reader adds (formats/xbox for the Xbox containers and assets, media for disc and disk images)."""
    try:
        info["size"] = os.path.getsize(path)
    except OSError as e:
        info["notes"].append("cannot be read: %s" % e)
        return info
    found = detect.identify(path)
    if not found:
        info["notes"].append("not a format CodeBreaker recognises")
        return info
    m = found[0]
    info["format"], info["kind"] = m["format"], m["family"]
    info["summary"] = {"format": m["format"], "type": m["detail"], "family": m["family"],
                       "read with": ("codebreaker " + m["command"]) if m["command"] else "(nothing yet)"}
    if m["command"] == "media" or m["family"] in ("xbox", "xbox360"):
        try:
            from codebreaker.formats import xbox
            info["summary"].update(xbox.summary(path, m))
        except Exception as e:                               # a damaged container: the format line stands
            info["notes"].append("%s: %s" % (type(e).__name__, str(e)[:120]))
    info["ok"] = True
    return info


def folder_map(folder, log=None):
    """Every file under `folder`, identified and tallied by format (the map of a directory).
    -> {"folder", "files", "bytes", "formats": [{format, family, command, files, bytes, example}],
        "unrecognised": [{extension, files, bytes, example}]}"""
    by, other, files, total = {}, {}, 0, 0
    skip = {".git", "__pycache__", "node_modules", ".gradle", ".vs"}
    for dp, dn, fn in os.walk(folder):
        dn[:] = sorted(d for d in dn if d not in skip)
        for n in sorted(fn):
            p = os.path.join(dp, n)
            try:
                size = os.path.getsize(p)
            except OSError:
                continue
            files += 1
            total += size
            if not size:
                continue
            fmt = detect.format_of(p)
            if fmt:
                m = {"format": {"PE": "PE executable", "NE": "16-bit Windows (NE)", "LE": "virtual device driver (LE)",
                                "LX": "OS/2 / VxD (LX)", "MZ": "DOS program (MZ)"}.get(fmt, fmt),
                     "family": "dos" if fmt == "MZ" else "windows", "command": "info"}
                if fmt in ("PE", "W3", "W4"):               # the CPU decides the platform; a bundle of drivers
                    m = (detect.identify(p) or [m])[0]
            else:
                found = detect.identify(p)
                m = found[0] if found else None
            if m is None:
                m = _text_kind(p, n)
            if m is None:
                ext = os.path.splitext(n)[1].lower() or "(none)"
                r = other.setdefault(ext, {"extension": ext, "files": 0, "bytes": 0, "example": p})
            else:
                key = (m["format"], m.get("detail", "") if m["format"] == "PE executable" else "")
                r = by.setdefault(key, {"format": m["format"] + (" (%s)" % key[1] if key[1] else ""),
                                        "family": m["family"], "command": m["command"],
                                        "files": 0, "bytes": 0, "example": p})
            r["files"] += 1
            r["bytes"] += size
        if log and files and files % 2000 == 0:
            log("  %d files..." % files)
    order = lambda rows: sorted(rows, key=lambda r: (-r["bytes"], -r["files"]))
    return {"folder": folder, "files": files, "bytes": total, "formats": order(by.values()),
            "unrecognised": order(other.values())}


_SOURCE = {".c": "C", ".h": "C/C++ header", ".cc": "C++", ".cpp": "C++", ".cxx": "C++", ".hpp": "C++ header",
           ".hh": "C++ header", ".cs": "C#", ".java": "Java", ".py": "Python", ".asm": "assembly", ".s": "assembly",
           ".inc": "assembly include", ".mac": "assembly macros", ".js": "JavaScript", ".ts": "TypeScript",
           ".rs": "Rust", ".go": "Go", ".lua": "Lua", ".hlsl": "HLSL shader", ".fx": "effect shader",
           ".ps1": "PowerShell", ".bat": "batch", ".cmd": "batch", ".sh": "shell", ".pl": "Perl",
           ".md": "Markdown", ".txt": "text", ".json": "JSON", ".xml": "XML", ".toml": "TOML", ".yml": "YAML",
           ".yaml": "YAML", ".ini": "INI", ".cfg": "config", ".csv": "CSV", ".html": "HTML", ".css": "CSS",
           ".def": "module definition", ".mak": "makefile", ".cmake": "CMake", ".gradle": "Gradle",
           ".csproj": "C# project", ".vcxproj": "C++ project", ".sln": "Visual Studio solution",
           ".obj": "Wavefront OBJ model", ".mtl": "Wavefront material", ".resx": ".NET resources"}


def _text_kind(path, name):
    """A text file, named by what its extension says it holds; None when it is not text."""
    ext = os.path.splitext(name)[1].lower()
    if name in ("Makefile", "CMakeLists.txt", "LICENSE", "NOTICE", "README", "AUTHORS"):
        what = {"Makefile": "makefile", "CMakeLists.txt": "CMake"}.get(name, "text")
    elif ext in _SOURCE:
        what = _SOURCE[ext]
    else:
        return None
    try:
        with open(path, "rb") as f:
            if b"\0" in f.read(4096):
                return None
    except OSError:
        return None
    return {"format": "source / text: " + what, "family": "source", "command": ""}


def _size(n):
    for unit in ("B", "KB", "MB", "GB", "TB"):
        if n < 1024 or unit == "TB":
            return ("%d %s" % (n, unit)) if unit == "B" else ("%.1f %s" % (n, unit))
        n /= 1024.0


def markdown(info, xex_header=None):
    """A one-page markdown report of a description (describe()), or of a folder map (folder_map())."""
    if "formats" in info:                                    # a folder
        md = ["# %s" % info["folder"], "",
              "%d files, %s. Recognised:" % (info["files"], _size(info["bytes"])), "",
              "| Format | Platform | Files | Size | Read with |", "|---|---|---:|---:|---|"]
        for r in info["formats"]:
            md.append("| %s | %s | %d | %s | %s |" % (r["format"], r["family"], r["files"], _size(r["bytes"]),
                                                    ("`codebreaker %s`" % r["command"]) if r["command"] else "-"))
        if info["unrecognised"]:
            md += ["", "Not recognised, by extension:", "", "| Extension | Files | Size | Example |", "|---|---:|---:|---|"]
            for r in info["unrecognised"][:40]:
                md.append("| `%s` | %d | %s | `%s` |" % (r["extension"], r["files"], _size(r["bytes"]),
                                                      os.path.relpath(r["example"], info["folder"])))
        return "\n".join(md) + "\n"
    md = ["# %s" % info["name"], "", "%s, %s." % (info["format"] or "not recognised", _size(info["size"])), ""]
    if xex_header:
        md += ["## Xbox 360 executable", "", "| Field | Value |", "|---|---|"]
        md += ["| %s | %s |" % (k, v) for k, v in xex_header.items()] + [""]
    if info.get("summary"):
        md += ["## Summary", "", "| Field | Value |", "|---|---|"]
        md += ["| %s | %s |" % (k, _cell(k, v)) for k, v in info["summary"].items()] + [""]
    if info.get("sections"):
        md += ["## Sections", "", "| Name | Address | Size | Flags |", "|---|---|---:|---|"]
        for s in info["sections"]:
            md.append("| %s | %s | %s | %s |" % (s.get("name", ""), _cell("address", s.get("address")),
                                                _cell("size", s.get("size")), _cell("flags", s.get("flags", ""))))
        md.append("")
    imps = info.get("imports") or []
    if imps:
        md += ["## Imports", "", "| Module | Functions |", "|---|---:|"]
        md += ["| %s | %d |" % (i.get("module", ""), i.get("count", 0)) for i in imps] + [""]
    exp = info.get("exports") or {}
    if exp.get("count"):
        md += ["- **Exports:** %d" % exp["count"], ""]
    for n in info.get("notes", []):
        md.append("> %s" % n)
    return "\n".join(md) + "\n"


def xex_facts(path):
    """What a .xex's own header says (formats/xex.py), for `info`; {} when it is not one."""
    from codebreaker.formats import xex
    if not xex.is_xex(path):
        return {}
    h = xex.header(path)
    out = {"image name": h.name, "load address": "0x%08X" % (h.base or 0), "entry point": "0x%08X" % (h.entry or 0),
           "import libraries": ", ".join(h.imports)}
    if h.libraries:
        out["built with"] = ", ".join("%s %s" % lv for lv in h.libraries)
    return out


# ---- strings, bytes, resource bodies ---------------------------------------------------------
def _locator(path):
    """file offset -> where that byte is loaded: an address in a 32-bit image, 'segment:offset' in
    a 16-bit one, None elsewhere."""
    fmt = detect.format_of(path)
    try:
        if fmt == "PE":
            f = pe.open_pe(path)
            spans = [(s["rawptr"], s["rawsize"], f.image_base + s["vaddr"]) for s in f.sections]
            return lambda off: next((va + off - at for at, n, va in spans if at <= off < at + n), None)
        if fmt == "NE":
            spans = [(s["file_off"], s["length"], s["idx"]) for s in ne.NEFile(path).segments() if s["file_off"]]
            return lambda off: next(("%d:%04x" % (i, off - at) for at, n, i in spans if at <= off < at + n), None)
    except Exception:
        pass
    return lambda off: None


def strings(path, min_len=4, wide=True, limit=None):
    """[{offset, address, kind, category, text}] -- every run of printable characters in the file,
    8-bit ('ascii') and 16-bit ('utf16'), in file order."""
    with open(path, "rb") as fh:
        data = fh.read()
    where = _locator(path)
    return [{"offset": off, "address": where(off), "kind": kind, "category": category, "text": text}
            for off, kind, category, text in native.call("inspect.strings", data, min_len, wide, limit)]


def _hex_lines(data, base=0):
    return native.call("inspect.hex_lines", data, base)


def hexdump(path, offset=0, length=256):
    """`length` bytes of the file from `offset`, as offset / hex / text lines."""
    with open(path, "rb") as fh:
        fh.seek(max(0, offset))
        return "\n".join(_hex_lines(fh.read(max(0, length)), max(0, offset)))


def _version_text(data, wide):
    fixed, table, _translations = resources.version(data, wide)
    out = []
    if fixed:
        a, b, c, d = fixed[2:6]
        out += ["FileVersion      %d.%d.%d.%d" % (a >> 16, a & 0xFFFF, b >> 16, b & 0xFFFF),
                "ProductVersion   %d.%d.%d.%d" % (c >> 16, c & 0xFFFF, d >> 16, d & 0xFFFF), ""]
    for _language, entries in sorted(table.items()):
        out += ["%-17s%s" % (key, text.strip()) for key, text in entries if text.strip()]
    return "\n".join(out) or "(no version fields could be read)"


def _string_table(data, wide):
    """A string resource holds sixteen strings, each led by its length."""
    out = ["[%2d] %s" % (i, text.replace("\r", " ").replace("\n", " "))
           for i, text in enumerate(resources.strings(data, wide)) if text]
    return "\n".join(out) or "(no strings in this block)"


def _as_icon(image, cursor):
    """An icon or cursor resource is one image of an icon file; wrap it as a file of one."""
    hot = (0, 0)
    if cursor:
        hot, image = struct.unpack_from("<HH", image, 0), image[4:]
    if image[:4] == b"\x89PNG":
        w = h = 0
        bits = 32
    else:
        w, h = struct.unpack_from("<II", image, 4)
        bits = struct.unpack_from("<H", image, 14)[0]
        h //= 2                                              # picture and mask are stacked
    a, b = hot if cursor else (1, bits)
    return struct.pack("<HHH", 0, 2 if cursor else 1, 1) + struct.pack(
        "<BBBBHHII", w & 0xFF, h & 0xFF, 0, 0, a, b, len(image), 22) + image


def resource_body(path, offset, size, type_id=-1, wide=True, limit=1 << 20):
    """A resource's content made showable: {'kind': 'image', 'content_type', 'bytes'} for bitmaps,
    icons and cursors; {'kind': 'text', 'text'} for version information, string tables, manifests
    and readable raw data; {'kind': 'hex', 'text'} for everything else. `offset` and `size` are
    an item's from describe()['resources']; `wide` is False for a 16-bit module."""
    try:
        with open(path, "rb") as fh:
            fh.seek(max(0, offset))
            data = fh.read(max(0, min(size, limit)))
    except OSError as e:
        return {"kind": "hex", "text": "cannot be read: %s" % e}
    if not data:
        return {"kind": "hex", "text": "(empty)"}
    try:
        if type_id == 2 and len(data) < 12:
            raise ValueError("too short to be a picture")
        if type_id == 2:
            return {"kind": "image", "content_type": "image/bmp", "bytes": resources.bitmap_file(data)}
        if type_id in (1, 3):
            return {"kind": "image", "content_type": "image/x-icon", "bytes": _as_icon(data, type_id == 1)}
        if type_id == 16:
            return {"kind": "text", "text": _version_text(data, wide)}
        if type_id == 6:
            return {"kind": "text", "text": _string_table(data, wide)}
        if type_id in (23, 24):
            return {"kind": "text", "text": data.decode("utf-8", "replace")}
        if type_id == 10 and sum(1 for b in data[:256] if 9 <= b <= 13 or 32 <= b < 127) > 0.85 * len(data[:256]):
            return {"kind": "text", "text": data.decode("latin1")}
    except Exception as e:
        return {"kind": "hex", "text": "could not be shown as its type (%s)\n\n%s" % (
            type(e).__name__, "\n".join(_hex_lines(data[:4096])))}
    more = ["... %d more bytes" % (len(data) - 4096)] if len(data) > 4096 else []
    return {"kind": "hex", "text": "\n".join(_hex_lines(data[:4096]) + more)}


# ---- as text ---------------------------------------------------------------------------------
_HEX_KEYS = re.compile(r"address|offset|image_base|code_base|^entry$|entry_point|rva|_va$|slot|checksum|^key$|"
                       r"flags_value|module_flags|magic|machine_id|device_id|ddk_version")


def _cell(key, value):
    if isinstance(value, bool):
        return "yes" if value else "no"
    if isinstance(value, int):
        return ("0x%x" % value) if _HEX_KEYS.search(key) and value >= 0 else str(value)
    if isinstance(value, (list, tuple)):
        return " ".join(_cell(key, v) for v in value)
    return "" if value is None else str(value).replace("\r", "\\r").replace("\n", "\\n")


def render(value, key="", indent=0):
    """Any part of a description as readable lines: a dict as `name  value` lines, a list of
    records as a table."""
    pad = "  " * indent
    if isinstance(value, dict):
        lines = []
        width = max([len(k) for k in value] + [1])
        for k, v in value.items():
            if isinstance(v, dict) or (isinstance(v, list) and v and isinstance(v[0], dict)):
                lines += ["%s%s:" % (pad, k)] + render(v, k, indent + 1)
            else:
                lines.append("%s%-*s  %s" % (pad, width, k, _cell(k, v)))
        return lines
    if isinstance(value, list) and value and isinstance(value[0], dict):
        def holds_records(k):                                # any row with records under k makes the column nested
            return any(isinstance(r, dict) and (isinstance(r.get(k), dict) or (
                isinstance(r.get(k), list) and r[k] and isinstance(r[k][0], dict))) for r in value)
        plain = [k for k in value[0] if not holds_records(k)]
        nested = [k for k in value[0] if k not in plain]
        rows = [[_cell(k, r.get(k)) for k in plain] for r in value]
        widths = [min(48, max([len(k)] + [len(r[i]) for r in rows])) for i, k in enumerate(plain)]
        lines = [pad + "  ".join("%-*s" % (w, k) for w, k in zip(widths, plain)).rstrip()]
        for r, row in zip(value, rows):
            lines.append(pad + "  ".join("%-*s" % (w, c) for w, c in zip(widths, row)).rstrip())
            for k in nested:
                lines += render(r[k], k, indent + 2)
        return lines
    if isinstance(value, list):
        return [pad + _cell(key, v) for v in value] or [pad + "(none)"]
    return [pad + _cell(key, value)]


def main(argv):
    """python -m codebreaker info <file-or-module> [--json] [--section NAME] [--offset N] [--length N] [--min N]
    python -m codebreaker info <folder> [--json]                  every file in it, identified and tallied
    python -m codebreaker info <file-or-folder> --markdown [--out FILE.md]   a one-page report
    python -m codebreaker info <packed.exe> --unpack <out.exe>

    Show what a file is and holds: executables in full (NAME is one of: summary dos headers
    directories sections exports imports resources relocations tls debug certificates clr rich
    driver members strings hex, or `all`), and any other file by its format -- Xbox containers
    and assets, disc and disk images, objects, bytecode. A folder is mapped: every file in it
    identified, tallied by format. --unpack writes a packed DOS program (EXEPACK, PKLITE, ...) back
    out unpacked, as a plain MZ file that runs the same way."""
    def option(name, default=None):
        return argv[argv.index(name) + 1] if name in argv and argv.index(name) + 1 < len(argv) else default

    taken = {option(n) for n in ("--section", "--offset", "--length", "--min", "--unpack", "--out")}
    names = [a for a in argv if not a.startswith("--") and a not in taken]
    if not names:
        print(main.__doc__)
        return 2
    as_md, out_md = "--markdown" in argv, option("--out")
    try:                                                     # names and strings may hold any byte
        import sys
        sys.stdout.reconfigure(errors="replace")
    except (AttributeError, ValueError):
        pass

    def emit(text):
        if out_md:
            with open(out_md, "w", encoding="utf-8", newline="\n") as f:
                f.write(text)
            print("wrote %s" % out_md)
        else:
            print(text)

    if os.path.isdir(names[0]):                              # a folder: its map
        m = folder_map(names[0], log=lambda s: print(s, flush=True))
        if "--json" in argv:
            emit(json.dumps(m, indent=1))
        elif as_md:
            emit(markdown(m))
        else:
            lines = ["%s: %d files, %s" % (m["folder"], m["files"], _size(m["bytes"]))]
            for r in m["formats"]:
                lines.append("  %-46s %-9s %7d files %10s   %s" % (r["format"][:46], r["family"], r["files"], _size(r["bytes"]),
                                                               ("codebreaker " + r["command"]) if r["command"] else ""))
            if m["unrecognised"]:
                lines.append("  not recognised: " + ", ".join("%s %d" % (r["extension"], r["files"]) for r in m["unrecognised"][:25]))
            emit("\n".join(lines))
        return 0
    xex_head = xex_facts(names[0]) if os.path.isfile(names[0]) else {}
    path = corpus.retail(names[0])
    if not path:
        print("no file or corpus module %r" % names[0])
        return 2
    if option("--unpack"):
        from codebreaker.formats import dos
        p = dos.read(path)
        if not p.unpacked:
            print("%s is not packed%s" % (p.name, "" if not p.packer else " readably (%s)" % "; ".join(p.notes)))
            return 1
        with open(option("--unpack"), "wb") as f:
            f.write(dos.unpacked_file(p))
        print("%s (%s, unpacked %s) -> %s" % (p.name, p.packer, p.unpacked, option("--unpack")))
        return 0
    section, as_json = option("--section"), "--json" in argv
    if section == "hex":
        text = hexdump(path, int(option("--offset", "0"), 0), int(option("--length", "256"), 0))
        print(json.dumps({"hex": text}) if as_json else text)
        return 0
    if section == "strings":
        rows = strings(path, int(option("--min", "4"), 0))
        if as_json:
            print(json.dumps(rows))
        else:
            for r in rows:
                print("%08x  %-10s %-5s %-8s %s" % (r["offset"], _cell("address", r["address"]), r["kind"],
                                                   r["category"], _cell("", r["text"])[:120]))
        return 0
    info = describe(path)
    if xex_head:
        info["xex"] = xex_head
    if as_md:
        emit(markdown(info, xex_head))
        return 0 if info["ok"] else 1
    if section and section != "all" and section not in info:
        print("%s has no part %r (it has: %s)" % (info["name"], section, " ".join(
            k for k in SECTIONS if info.get(k) or k in ("strings", "hex"))))
        return 2
    if as_json:
        print(json.dumps(info if section in (None, "all") else info[section], indent=1))
    elif section and section != "all":
        print("\n".join(render(info[section], section)))
    else:
        print("%s  (%s, %d bytes)" % (info["path"], info["format"] or "not an executable", info["size"]))
        short = ("xex", "summary", "headers", "sections") if section is None else \
            [k for k in info if isinstance(info[k], (dict, list)) and info[k] and k != "notes"]
        for k in short:
            if info.get(k):
                print("\n%s:" % k)
                print("\n".join(render(info[k], k, 1)))
        if section is None and info["ok"] and "exports" in info["summary"]:
            s = info["summary"]
            print("\n%d exports, %d imports from %d modules, %d resources  (--section exports | imports | "
                  "resources | all)" % (s["exports"], s["imports"], s["import_modules"], s["resources"]))
        for n in info["notes"]:
            print("note: " + n)
    return 0 if info["ok"] else 1
