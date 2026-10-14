"""names.py -- the best name for every function and import of a module, and where it came from.

    python -m codebreaker names comctl32.dll               every function and import, named
    python -m codebreaker names comctl32.dll --set 7fe0183a StrChrHelper
    python -m codebreaker names --shared                   how many bodies the indexed modules share
    python -m codebreaker names --ordinals FILE...         remake the table of names for numbered exports

    from codebreaker.symbols import names
    names.of_module(path)        {"functions": [(address, key, name, source)], "imports": [(module, ordinal, name, source)]}
    names.bodies(path)           {address: digest of the body}
    names.index(path)            record the module's bodies, and the names it gives them, in the store
    names.set_name(path, address, "Name")     name a body: it holds in every module with that body

A function's name is taken from the first of these that has one:

    export          the module's own export table
    debug symbols   CodeView records in the file, or the module's .SYM file (Microsoft's symbol
                    files for the system modules, when the corpus's reference folder holds them)
                    -- used only where the file's exports say the symbols are for this build
    ordinal table   the name its maker's definition file gives an export the module leaves unnamed
    named body      a name given to this body by hand
    runtime library the library function its bytes are (symbols/runtime_library.py)
    same body       the name another module gives the same body
    (none)          sub_<address>: a placeholder, not a name

The same function is linked into many modules -- the C runtime, helper libraries, code shared
between the 16-bit and 32-bit halves of a component. Its body is recognised by a digest of its
bytes with everything the linker fills in left out (addresses inside the module, calls and
jumps out of the function), so the digest is the same wherever the code sits. The function
store keeps one row per body per module; a name given to a body is kept once and read back for
every module that has it.
"""
import hashlib
import os
import re
import struct

from codebreaker.base import corpus
from codebreaker.decompile import describe
from codebreaker.formats.ne import NEFile
from codebreaker.formats.pe import open_pe
from codebreaker.store import functions as store
from codebreaker.symbols import demangle, ordinals, runtime_library

_SMALLEST = 24                  # bytes; a body shorter than this is everybody's
_ENDS = ("ret", "retf", "retn", "jmp", "ljmp", "iret", "iretd")


def key(module_path, address):
    """The function store's text for an address (see verify/win32.py and win16.py)."""
    if corpus.lane(module_path) == "ne":
        from codebreaker.verify import win16
        return win16.key(*describe.where(address))
    from codebreaker.verify import win32
    return win32.LANES["pe"].key(address)


# ---- bodies ------------------------------------------------------------------------------------------
def _digest(code, masked, last):
    """Digest of a function's bytes up to its last return or jump (what follows is padding),
    with the offsets in `masked` left out. None for a body with nothing of its own in it."""
    body = bytearray(code[:last])
    for o in masked:
        if o < last:
            body[o] = 0
    if last < _SMALLEST:
        return None
    return hashlib.sha1(b"%d:" % last + bytes(body)).hexdigest()[:16]


def _bodies32(path):
    from codebreaker.formats import pe_functions
    carve = pe_functions.carve(path)
    pe = carve.pe
    lo = pe.image_base
    hi = lo + max((s["vaddr"] + max(s["vsize"], s["rawsize"]) for s in pe.sections), default=0)
    out = {}
    for start, f in carve.funcs.items():
        base, raw = carve._sec_of(start)
        if raw is None:
            continue
        end, masked, last, va, own = f["end"], set(), 0, start, 0
        while va < end and va in carve.insn:
            mnemonic, operands, size, ins, _target = carve.insn[va]
            at = va - start
            for off, n, value in ((ins.disp_offset, ins.disp_size, ins.disp & 0xFFFFFFFF),
                                  (ins.imm_offset, ins.imm_size, None)):
                if not off or n != 4:
                    continue
                if value is None:
                    value = struct.unpack_from("<I", raw, va - base + off)[0]
                    if mnemonic == "call" or mnemonic[0] == "j":             # relative: where it goes
                        value = (va + size + value) & 0xFFFFFFFF
                        if start <= value < end:
                            continue                                         # inside the function: the same anywhere
                        masked.update(range(at + off, at + off + 4))
                        continue
                if lo <= value < hi:                                         # an address in the module
                    masked.update(range(at + off, at + off + 4))
            own += not runtime_library._FRAME.match((mnemonic + " " + operands).strip())
            va += size
            if mnemonic in _ENDS:
                last = va - start
        digest = _digest(raw[start - base:end - base], masked, last) if own >= 6 else None
        if digest:
            out[start] = digest
    return out


def _bodies16(path):
    import capstone
    nef = NEFile(path)
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
    segs = {s["idx"]: s for s in nef.segments()}
    starts = {}
    for (seg, off) in describe.functions(nef):
        starts.setdefault(seg, []).append(off)
    out = {}
    for seg, offs in starts.items():
        s = segs[seg]
        code = nef.data[s["file_off"]:s["file_off"] + s["length"]]
        patched = set()                                          # every place the loader writes
        for r in nef.relocations(s):
            width = {3: 4, 2: 2, 5: 2, 0: 1}.get(r["src"], 2)
            off, hops = r["offset"], 0
            while off != 0xFFFF and off + 1 < len(code) and hops < 0x10000:
                patched.update(range(off, off + width))
                if r["additive"]:
                    break
                off, hops = struct.unpack_from("<H", code, off)[0], hops + 1
        offs.sort()
        for i, start in enumerate(offs):
            end = offs[i + 1] if i + 1 < len(offs) else len(code)
            masked, last, own = {o - start for o in patched if start <= o < end}, 0, 0
            for at, size, mnemonic, operands in md.disasm_lite(bytes(code[start:end]), start):
                if (mnemonic == "call" or mnemonic[0] == "j") and size == 3 and re.fullmatch(r"0x[0-9a-f]+", operands):
                    if not start <= int(operands, 16) < end:                 # a near call or jump out of the function
                        masked.update(range(at - start + 1, at - start + 3))
                own += not runtime_library._FRAME.match((mnemonic + " " + operands).strip())
                if mnemonic in _ENDS:
                    last = at + size - start
            digest = _digest(code[start:end], masked, last) if own >= 6 else None
            if digest:
                out[describe.address(seg, start)] = digest
    return out


def bodies(module_path):
    """{address: digest} for the functions of a module that have a body worth telling apart."""
    lane = corpus.lane(module_path)
    if lane == "pe" and open_pe(module_path).machine == 0x014C:
        return _bodies32(module_path)
    return _bodies16(module_path) if lane == "ne" else {}


# ---- debug symbols -------------------------------------------------------------------------------------
def codeview(data):
    """{(segment, offset): name} from CodeView symbols appended to an executable (the NB09 form:
    public symbols and procedures), or {}."""
    base = data.find(b"NB09")
    out = {}
    while base >= 0 and not out:
        try:
            p = base + struct.unpack_from("<I", data, base + 4)[0]
            head, entry, count = struct.unpack_from("<HHI", data, p)
            for i in range(count if 12 <= entry <= 32 else 0):
                kind, _module, lfo, size = struct.unpack_from("<HHIi", data, p + head + i * entry)
                if kind not in (0x129, 0x12A, 0x134):
                    continue
                s = base + lfo + (0 if kind == 0x134 else 16)
                end = min(base + lfo + size, len(data))
                while s + 4 <= end:
                    n, rec = struct.unpack_from("<HH", data, s)
                    if n < 2:
                        break
                    at = s + 4 if rec == 0x0203 else s + 28 if rec in (0x0204, 0x0205) else None
                    if at:
                        off, seg = struct.unpack_from("<IH", data, at)
                        q = at + (8 if rec == 0x0203 else 9)
                        name = data[q + 1:q + 1 + data[q]].decode("latin1", "replace")
                        if name and seg:
                            out.setdefault((seg, off), name)
                    s += 2 + n
        except (struct.error, IndexError):
            out = {}
        base = data.find(b"NB09", base + 4)
    return out


def read_sym(path):
    """[(segment number, {offset: name})] from a .SYM file (the symbol file MAPSYM makes from a
    linker map: per segment, each public name and its offset)."""
    with open(path, "rb") as f:
        d = f.read()
    out, seg, count = [], struct.unpack_from("<H", d, 12)[0] * 16, struct.unpack_from("<H", d, 10)[0]
    for _ in range(count):
        if not seg or seg + 21 > len(d):
            break
        nxt, nsym, table, number = struct.unpack_from("<HHHH", d, seg)
        wide = d[seg + 14] & 1
        symbols = {}
        for i in range(nsym):
            at = seg + struct.unpack_from("<H", d, seg + table + 2 * i)[0]
            if at + 5 > len(d):
                break
            value = struct.unpack_from("<I" if wide else "<H", d, at)[0]
            n = d[at + (4 if wide else 2)]
            symbols[value] = d[at + (5 if wide else 3):at + (5 if wide else 3) + n].decode("latin1", "replace")
        out.append((number, symbols))
        if nxt * 16 <= seg:
            break
        seg = nxt * 16
    return out


def sym_objects(path):
    """{segment number: name} from a .SYM file. The linker names a segment after the object
    file its code came from (_MSGBOX, _WINUTIL, ...), so where a module was linked one segment
    per object these are the names of its maker's source files. {} when it cannot be read."""
    try:
        with open(path, "rb") as f:
            d = f.read()
        out, seg, count = {}, struct.unpack_from("<H", d, 12)[0] * 16, struct.unpack_from("<H", d, 10)[0]
        for _ in range(count):
            if not seg or seg + 21 > len(d):
                break
            nxt, _nsym, _table, number = struct.unpack_from("<HHHH", d, seg)
            name = d[seg + 21:seg + 21 + d[seg + 20]].decode("latin1", "replace")
            if name:
                out[number] = name
            if nxt * 16 <= seg:
                break
            seg = nxt * 16
        return out
    except (OSError, struct.error, IndexError):
        return {}


_PLAIN_SEGMENTS = {"_TEXT", "TEXT", "DGROUP", "BSS", "_BSS", "RDATA", "_DATA", "DATA", "CONST", "IGROUP", "LGROUP",
                   "_LGROUP", "_IGROUP", "EDATA", "IDATA", "RELOC", "RSRC", "CODE", "_CODE", "AUTO", "STACK", "HEAP",
                   "_INIT", "INIT"}


def source_files(module_path):
    """{address: name of the object file the function was linked from}, for the functions the
    module's .SYM file places in a segment named after an object -- and only where that file is
    shown to be for this build (see debug_names). {} when there is no such file."""
    return _debug(module_path)[1]


_sym_files = None


def _sym_file(stem):
    """The .SYM files for a module in the reference folder (settings "reference", else the
    folder of that name beside the corpus), or []."""
    global _sym_files
    if _sym_files is None:
        _sym_files = {}
        ref = corpus.reference()
        for dp, dirs, fns in os.walk(ref):
            dirs[:] = [d for d in dirs if d.upper() not in ("ALPHA", "MIPS", "PPC")]
            for fn in fns:
                if fn.lower().endswith(".sym"):
                    _sym_files.setdefault(fn[:-4].upper(), []).append(os.path.join(dp, fn))
    return _sym_files.get(stem, [])


def _plain(name):
    return demangle.plain(name)[0].upper()


def debug_names(module_path):
    """{address: (name, where it is from)} from debug symbols that are shown to be for this very
    build: a symbol file is from some build of the module, and offsets move between builds, so
    a segment's names are used only where the file's own exports sit at the offsets the symbols
    give them (at least two, and two in three of those the symbol file names)."""
    return _debug(module_path)[0]


_debugged = {}


def _debug(module_path):
    key_ = os.path.abspath(module_path)
    if key_ not in _debugged:
        files = {}
        _debugged[key_] = (_debug_names(module_path, files), files)
    return _debugged[key_]


def _debug_names(module_path, files):
    lane = corpus.lane(module_path)
    out = {}
    with open(module_path, "rb") as f:
        data = f.read()
    if lane == "ne":
        nef = NEFile(module_path)
        entries = {e["ordinal"]: (e["seg"], e["offset"]) for e in nef.entries() if e.get("kind") in ("FIXED", "MOVEABLE")}
        exports = {nm.upper(): entries[o] for nm, o in nef.resident_names()[1:] + nef.nonresident_names()[1:]
                   if o in entries}
        stem = nef.module_name().upper()

        def place(seg, off):
            return describe.address(seg, off)
        sections = None
    elif lane == "pe":
        pe = open_pe(module_path)
        exports = {_plain(nm): rva for _o, nm, rva in pe.exports() if nm}
        stem = os.path.splitext(os.path.basename(module_path))[0].upper()
        sections = [s["vaddr"] for s in pe.sections]

        def place(seg, off):
            return pe.image_base + sections[seg - 1] + off if 0 < seg <= len(sections) else None
    else:
        return out
    tables = [("CodeView symbols in the file", _by_segment(codeview(data)), {})]
    tables += [(os.path.basename(p), _read_sym(p), sym_objects(p)) for p in _sym_file(stem)]
    for where, segments, objects in tables:
        for number, symbols in segments:
            agree = named = 0
            for off, name in symbols.items():
                at = exports.get(_plain(name))
                if at is not None:
                    named += 1
                    if lane == "ne":
                        agree += at == (number, off & 0xFFFF)
                    else:
                        agree += place(number, off) == pe.image_base + at
            if agree >= 2 and agree * 3 >= named * 2:
                for off, name in symbols.items():
                    address = place(number, off & 0xFFFF if lane == "ne" else off)
                    if address is not None:
                        out.setdefault(address, (demangle.plain(name)[0], where))
                        if objects.get(number, "").upper() not in _PLAIN_SEGMENTS and objects.get(number):
                            files.setdefault(address, objects[number].lstrip("_").lower())
        if out:
            break
    return out


def _by_segment(symbols):
    by = {}
    for (seg, off), name in symbols.items():
        by.setdefault(seg, {})[off] = name
    return sorted(by.items())


def _read_sym(path):
    try:
        return read_sym(path)
    except (OSError, struct.error, IndexError):
        return []


# ---- the names of a module -------------------------------------------------------------------------------
def _starts(module_path):
    lane = corpus.lane(module_path)
    if lane == "ne":
        return sorted(describe.address(seg, off) for seg, off in describe.functions(NEFile(module_path)))
    return list(describe.function_starts(module_path)) if lane == "pe" else []


def own_names(module_path):
    """{address: (name, source)} from what the module and its symbol files say themselves:
    exports, debug symbols, the ordinal table."""
    lane = corpus.lane(module_path)
    out = {}
    if lane == "ne":
        nef = NEFile(module_path)
        entries = {e["ordinal"]: describe.address(e["seg"], e["offset"]) for e in nef.entries()
                   if e.get("kind") in ("FIXED", "MOVEABLE")}
        for nm, o in nef.nonresident_names()[1:] + nef.resident_names()[1:]:
            if o in entries:
                out.setdefault(entries[o], (nm, "export"))
        stem, numbered = nef.module_name().upper(), entries
    elif lane == "pe":
        pe = open_pe(module_path)
        numbered = {}
        for o, nm, rva in pe.exports():
            numbered[o] = pe.image_base + rva
            if nm:
                out.setdefault(pe.image_base + rva, (nm, "export"))
        stem = os.path.splitext(os.path.basename(module_path))[0].upper()
    else:
        return out
    for address, found in debug_names(module_path).items():
        out.setdefault(address, (found[0], "debug symbols (%s)" % found[1]))
    for o, address in numbered.items():
        if address not in out and ordinals.linker_name(stem, o):
            out[address] = (ordinals.linker_name(stem, o), "ordinal table (%s)" % ordinals.source(stem, o))
    return out


def index(module_path, fs=None, own=None):
    """Record a module's bodies in the store, each with the name the module itself gives it.
    -> number of bodies"""
    fs = fs or store.open()
    sha = corpus.sha(module_path) or ""
    own = own_names(module_path) if own is None else own
    fs.add_module(sha, os.path.basename(module_path), module_path, corpus.lane(module_path) or "")
    rows = {key(module_path, a): (d, own.get(a, (None, None))[0], own.get(a, (None, None))[1])
            for a, d in bodies(module_path).items()}
    fs.set_bodies(sha, rows)
    return len(rows)


def set_name(module_path, address, name, fs=None):
    """Name the body of the function at `address`; every module holding that body then has the
    name. -> the body's digest, or None when the function has no body worth naming."""
    fs = fs or store.open()
    sha = corpus.sha(module_path) or ""
    if not fs.bodies(sha):
        index(module_path, fs)
    row = fs.bodies(sha).get(key(module_path, address))
    if row:
        fs.name_body(row[0], name)
    return row[0] if row else None


def of_module(module_path, fs=None):
    """Every function and import of a module with its best name and where the name is from.
    -> {"functions": [(address, store key, name, source)], "imports": [(module, ordinal or None, name, source)]}"""
    fs = fs or store.open()
    lane = corpus.lane(module_path)
    sha = corpus.sha(module_path) or ""
    own = own_names(module_path)
    if not fs.bodies(sha):
        index(module_path, fs, own)
    library = runtime_library.identify(module_path)
    from_bodies = fs.names_from_bodies(sha)
    functions = []
    for address in _starts(module_path):
        k = key(module_path, address)
        shared = from_bodies.get(k)
        if address in own and own[address][1] == "export":
            name, source = own[address]
        elif shared and not shared[1].startswith("the same body"):
            name, source = shared[0], "named body (%s)" % shared[1]
        elif address in own:
            name, source = own[address]
        elif address in library:
            name, source = library[address][0], "runtime library: " + library[address][1]
        elif shared:
            name, source = shared
        else:
            name, source = "sub_" + k.replace(":", "_"), "(none)"
        functions.append((address, k, name, source))
    imports = []
    if lane == "ne":
        nef = NEFile(module_path)
        refs = nef.module_refs()
        seen = set()
        for seg in nef.segments():
            for r in nef.relocations(seg):
                if r["kind"] == "IMPORTNAME" and 0 < r["module"] <= len(refs):
                    item = (refs[r["module"] - 1].upper(), None, nef.imported_name(r["name_off"]), "imported by name")
                    if item not in seen:
                        seen.add(item)
                        imports.append(item)
        for (module, o), name in sorted(ordinals.imports16(module_path).items()):
            imports.append((module, o, name or "%s.%d" % (module, o),
                            "%s's exports (%s)" % (module, ordinals.source(module, o)) if name else "(none)"))
    elif lane == "pe":
        for dll, name in sorted(set(open_pe(module_path).imports().values())):
            module = os.path.splitext(dll)[0].upper()
            if name.startswith("#") and name[1:].isdigit():
                real = ordinals.name(module, int(name[1:]))
                imports.append((module, int(name[1:]), real or "%s.%s" % (module, name[1:]),
                                "%s's exports (%s)" % (module, ordinals.source(module, int(name[1:]))) if real else "(none)"))
            else:
                imports.append((module, None, name, "imported by name"))
    return {"functions": functions, "imports": imports}


def main(argv):
    """python -m codebreaker names <module> [--set ADDRESS NAME] | --shared | --ordinals FILE..."""
    if not argv:
        print(__doc__)
        return 2
    if argv[0] == "--ordinals":
        ordinals.collect(argv[1:])
        return 0
    fs = store.open()
    if argv[0] == "--shared":
        print("%d bodies recorded, %d distinct; %d bodies are in two or more modules (%d functions are such copies)"
              % fs.shared_bodies())
        return 0
    path = corpus.retail(argv[0])
    if not path:
        raise SystemExit("no module %r in the corpus" % argv[0])
    lane = corpus.lane(path)
    if "--set" in argv:
        text, name = argv[argv.index("--set") + 1:][:2]           # the address as the listing prints it
        address = describe.address(int(text.split(":")[0]), int(text.split(":")[1], 16)) if ":" in text else int(text, 16)
        digest = set_name(path, address, name, fs)
        print("body %s is now %s" % (digest, name) if digest else "that function has no body worth naming")
        return 0 if digest else 1
    found = of_module(path, fs)
    count = {}
    for address, k, name, source in found["functions"]:
        shown = demangle.readable(name) if name.startswith("?") else name
        print("%-10s %-48s %s" % (k, shown[:48], source))
        count[source.split(" (")[0].split(":")[0]] = count.get(source.split(" (")[0].split(":")[0], 0) + 1
    icount = {}
    for module, o, name, source in found["imports"]:
        shown = demangle.readable(name) if name.startswith("?") else name
        print("import %-12s %-6s %-40s %s" % (module, "" if o is None else o, shown[:40], source))
        what = "by name" if o is None else "by number, named" if source != "(none)" else "by number, no name"
        icount[what] = icount.get(what, 0) + 1
    print("%s: %d functions -- %s" % (os.path.basename(path).lower(), len(found["functions"]),
                                     ", ".join("%s %d" % kv for kv in sorted(count.items(), key=lambda kv: -kv[1]))))
    print("%d imports -- %s" % (len(found["imports"]), ", ".join("%s %d" % kv for kv in sorted(icount.items()))))
    info = runtime_library.compiler(path)
    print("built with: %s (linker %s)" % (info["compiler"], info["linker"] or "?") if lane in ("pe", "ne") else "")
    return 0
