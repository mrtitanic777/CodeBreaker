"""describe.py -- take an executable apart and describe it to the decompiler.

    describe32(module_path, addresses, workdir)   a PE module (x86 or PowerPC)
    describe16(module_path, workdir)              a 16-bit Windows module (NE)
    describe_le(image, name, workdir)             a virtual device driver (LE), one member at a time
    describe_dos(module_path, workdir)            a DOS program (MZ)

The native decompiler knows processors, not file formats. A description says which processor,
where the file's bytes sit in memory, and the facts read from the binary rather than guessed:
every function's start and name, how it returns (far or near, how many bytes of arguments it
removes), and every import with the same for it.

A PE module is nearly its own description: sections at their addresses, an import table.
A 16-bit module is a set of segments the loader places and patches, so describing one means
doing what the loader does: each segment gets a selector (segment 1 -> 0x1000, then every 8)
and so an address (selector * 0x10000), every relocation is applied, and a reference to an
imported function becomes a far pointer to an address standing for that function.
"""
import os
import re
import struct


# language id by PE machine type
_ARCH = {0x014C: "x86:LE:32:default:windows", 0x01F2: "PowerPC:BE:64:A2ALT-32addr:homed"}


def _ident(name):
    return re.sub(r"\W", "_", name)


def function_names(module_path, funcs):
    """{va: name} -- an export's own name, else sub_<rva>. A name also used by an import of this
    module stays with the import (the call has to bind to it)."""
    from codebreaker.formats.pe import open_pe
    from codebreaker.build import watcom as B
    pe = open_pe(module_path)
    imported = set(B.import_map(module_path))
    by_rva = {}
    for nm, rva in sorted(pe.export_addrs().items()):
        if re.fullmatch(r"[A-Za-z_]\w*", nm) and nm not in imported:
            by_rva.setdefault(rva, nm)
    return {va: by_rva.get(va - pe.image_base, "sub_%06X" % (va - pe.image_base)) for va in funcs}


def decorated_names(module_path):
    """{address: linker name} for the functions a module exports under a name that is not a C
    name -- a C++ function's decorated name. In the C text such a function is sub_<address>;
    this is the symbol it must be linked as, and symbols/demangle.py makes it a declaration.
    Addresses are as in the description (16-bit: address(segment, offset))."""
    from codebreaker.base import corpus
    plain = re.compile(r"[A-Za-z_]\w*\Z")
    out = {}
    if corpus.lane(module_path) == "ne":
        from codebreaker.formats.ne import NEFile
        nef = NEFile(module_path)
        entries = {e["ordinal"]: e for e in nef.entries() if e.get("kind") in ("FIXED", "MOVEABLE")}
        for nm, ordv in nef.resident_names()[1:] + nef.nonresident_names()[1:]:
            if ordv in entries and not plain.match(nm):
                out.setdefault(address(entries[ordv]["seg"], entries[ordv]["offset"]), nm)
    else:
        from codebreaker.formats.pe import open_pe
        pe = open_pe(module_path)
        for nm, rva in sorted(pe.export_addrs().items()):
            if not plain.match(nm):
                out.setdefault(pe.image_base + rva, nm)
    return out


def import_names16(nef):
    """{symbol: name} for a 16-bit module's imports by number: the symbol the C text and the
    link use (`USER_91`) and the function's real name (`GetDlgItem`), where the exporting
    module or its definition file gives one that is a C name."""
    from codebreaker.symbols import ordinals
    refs = nef.module_refs()
    out = {}
    for s in nef.segments():
        for r in nef.relocations(s):
            if r["kind"] == "IMPORTORD" and 0 < r["module"] <= len(refs):
                symbol = re.sub(r"\W", "_", "%s_%d" % (refs[r["module"] - 1], r["ordinal"]))
                if symbol not in out:
                    out[symbol] = ordinals.identifier(refs[r["module"] - 1], r["ordinal"])
    return {symbol: name for symbol, name in out.items() if name}


def function_starts(module_path):
    """Addresses of a PE module's functions. PowerPC images carry a table of them (.pdata: one
    record per function, its start and length); for x86 they are found by following the code."""
    from codebreaker.formats import powerpc_image
    from codebreaker.formats.pe import open_pe
    if open_pe(module_path).machine == powerpc_image.MACHINE:
        return powerpc_image.read(module_path).functions
    from codebreaker.verify import win32 as pipeline
    return pipeline.LANES["pe"].functions(module_path)


def describe32(module_path, funcs, outdir):
    """Write the description of a PE module for the native program: its processor, where its
    bytes sit in memory, and the facts about `funcs` (addresses) and its imports.
    -> (path of the description, {va: name})"""
    from codebreaker.symbols import api_arguments as api_stack
    from codebreaker.symbols import ordinals
    from codebreaker.build import watcom as B
    from codebreaker.formats.pe import open_pe
    pe = open_pe(module_path)
    arch = _ARCH.get(pe.machine)
    if not arch:
        raise ValueError("%s: no language for machine type %#x" % (os.path.basename(module_path), pe.machine))
    x86 = arch.startswith("x86:")
    names = function_names(module_path, funcs) if x86 else {va: "sub_%08X" % va for va in funcs}
    lines = ["arch\t%s" % arch]
    if not x86:
        # PowerPC: the image says where its bytes sit, which stubs are imports, and which
        # register save/restore runs are part of their callers (formats/powerpc_image.py)
        from codebreaker.formats import powerpc_image
        img = powerpc_image.read(module_path)
        for va, off, size in img.spans:
            lines.append("bytes\t%x\t%s\t%x\t%x" % (va, os.path.abspath(module_path), off, size))
        for va, name in sorted(img.imports.items()):
            lines.append("X\t%x\t%s\t-1" % (va, name))
        for va, name in sorted(img.inline.items()):
            lines.append("F\t%x\t%s\t-1\tinline" % (va, name))
    else:
        for sec in pe.sections:
            size = min(sec["rawsize"], sec["vsize"] or sec["rawsize"])
            if size and sec["rawptr"]:
                lines.append("bytes\t%x\t%s\t%x\t%x" % (pe.image_base + sec["vaddr"],
                                                        os.path.abspath(module_path), sec["rawptr"], size))
    mod = api_stack._Mod(module_path) if x86 else None
    for va in funcs:
        pop = None
        if x86:
            try:
                pop = api_stack._scan(mod, va - pe.image_base, 4)
            except Exception:
                pop = None
        lines.append("F\t%x\t%s\t%d" % (va, names[va], -1 if pop is None else pop))
    # An import by number is called by its real name when the module that exports it (or that
    # module's definition file) gives it one that is a C name nothing else here already has;
    # how many bytes of arguments it takes is read from the export with that number.
    taken = dict.fromkeys(set(B.import_map(module_path)) | set(names.values())) if x86 else {}
    for slot, (dll, name) in sorted((pe.imports() if x86 else {}).items()):
        stem = os.path.splitext(dll)[0].upper()
        if not name or name.startswith("#"):
            number = int(name[1:]) if name and name[1:].isdigit() else 0
            real = ordinals.identifier(stem, number)
            if real and taken.setdefault(real, (stem, number)) == (stem, number):
                pop = api_stack.header_bytes(real)
                pop = api_stack.stack_bytes(stem, name) if pop is None else pop
                lines.append("I\t%x\t%s\t%d" % (slot, real, -1 if pop is None else pop))
            else:
                lines.append("I\t%x\t%s\t-1" % (slot, _ident("%s_ord_%s" % (stem, (name or "#0")[1:]))))
            continue
        pop = api_stack.stack_bytes(stem, name) if x86 else None
        lines.append("I\t%x\t%s\t%d" % (slot, _ident(name), -1 if pop is None else pop))
    os.makedirs(outdir, exist_ok=True)
    path = os.path.join(outdir, "description.txt")
    with open(path, "w", newline="\n", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")
    return path, names


ARCH = "x86:LE:16:Protected Mode:default"
FIRST_SELECTOR = 0x1000
EXTERN_SELECTOR = 0xE000           # imported functions stand at EXTERN_SELECTOR:16*n


def selector(seg):
    return FIRST_SELECTOR + 8 * (seg - 1)


def address(seg, off):
    return (selector(seg) << 16) + off


def where(va):
    """(segment, offset) of an address inside the module's segments."""
    return ((va >> 16) - FIRST_SELECTOR) // 8 + 1, va & 0xFFFF


def inside(d, va):
    """Is `va` in one of the described module's code segments?"""
    seg, off = where(va)
    return ((va >> 16) - FIRST_SELECTOR) % 8 == 0 and off < d.code.get(seg, 0)


class Description(object):
    def __init__(self, path, names, where, imports):
        self.path, self.names, self.where, self.imports = path, names, where, imports


def _near_call_targets(code):
    """Offsets inside one code segment that two or more `call rel16` sites agree on. A stray
    0xE8 in data does not coincide with another, so an agreed target is a real function start."""
    sites = {}
    n = len(code)
    for i in range(n - 2):
        if code[i] == 0xE8:
            tgt = (i + 3 + struct.unpack_from("<h", code, i + 1)[0]) & 0xFFFF
            if tgt < n:
                sites.setdefault(tgt, set()).add(i)
    return {t for t, s in sites.items() if len(s) >= 2}


def functions(nef):
    """{(segment, offset): "far" | "near"} -- every function start that can be established."""
    segs = {s["idx"]: s for s in nef.segments()}
    out = {}
    for e in nef.entries():
        if e.get("kind") in ("FIXED", "MOVEABLE") and segs.get(e.get("seg"), {}).get("kind") == "CODE":
            out[(e["seg"], e["offset"])] = "far"
    for s in nef.segments():
        if s["kind"] != "CODE":
            continue
        for r in nef.relocations(s):
            if r.get("kind") == "INTERNAL" and r.get("seg") not in (None, 0xFF) \
                    and segs.get(r["seg"], {}).get("kind") == "CODE" and r.get("src") == 3:
                out.setdefault((r["seg"], r.get("entry", 0)), "far")
        code = nef.data[s["file_off"]:s["file_off"] + s["length"]]
        for off in _near_call_targets(code):
            out.setdefault((s["idx"], off), "near")
    return out


def returns(code, off, limit=600):
    """How the function at `off` returns: ("far" | "near" | None, bytes of arguments removed).

    Read from the function's own first return instruction: `retf` is a far function, `ret` a
    near one, and the operand is what it removes. That is the function's own statement of how
    it must be called; where it was found (entry table, a relocation, a near call) only says
    how SOMEONE calls it. None when no return is met within `limit` instructions."""
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
    n = 0
    for ins in md.disasm(bytes(code[off:off + 4 * limit]), off):
        n += 1
        if ins.mnemonic in ("ret", "retf", "lret"):
            pop = int(ins.op_str, 0) if ins.op_str else 0
            return ("near" if ins.mnemonic == "ret" else "far"), pop
        if n >= limit:
            break
    return None, -1


def model_line(d, va, name, default="near"):
    """The description line for a function at `va`."""
    seg, off = where(va)
    kind, pop = returns(d.segment_bytes.get(seg, b""), off)
    # A function that removes its own arguments says so in its return (`retf 6`); one that ends
    # in a plain return leaves them to the caller.
    conv = "__stdcall16" if pop > 0 else "__cdecl16" if pop == 0 else "__stdcall16"
    return "F\t%x\t%s\t%d\t%s%s" % (va, name, pop, conv, kind or default)


def describe16(module_path, outdir):
    """Write the description (and the relocated image it points at). -> Description"""
    from codebreaker.formats.ne import NEFile
    nef = NEFile(module_path)
    segs = nef.segments()
    entries = {e["ordinal"]: e for e in nef.entries() if e.get("kind") in ("FIXED", "MOVEABLE")}
    refs = nef.module_refs()
    images = {}
    for s in segs:
        data = bytearray(nef.data[s["file_off"]:s["file_off"] + s["length"]]) if s["file_off"] else bytearray()
        size = max(len(data), min(s["minalloc"], 0x10000))
        images[s["idx"]] = data + bytearray(size - len(data))
    imports, order = {}, []                                  # name -> stand-in address

    def extern(name):
        if name not in imports:
            imports[name] = (EXTERN_SELECTOR << 16) + 16 * len(order)
            order.append(name)
        return imports[name]

    for s in segs:
        img = images[s["idx"]]
        for r in nef.relocations(s):
            kind = r["kind"]
            if kind == "INTERNAL":
                if r["seg"] == 0xFF:                         # by entry-table ordinal
                    e = entries.get(r["entry"])
                    if not e:
                        continue
                    tseg, toff = e["seg"], e["offset"]
                else:
                    tseg, toff = r["seg"], r["entry"]
                tsel = selector(tseg)
            elif kind in ("IMPORTORD", "IMPORTNAME"):
                mod = refs[r["module"] - 1] if 0 < r["module"] <= len(refs) else "?"
                nm = ("%s_%d" % (mod, r["ordinal"])) if kind == "IMPORTORD" else nef.imported_name(r["name_off"])
                a = extern(re.sub(r"\W", "_", nm))
                tsel, toff = a >> 16, a & 0xFFFF
            else:
                continue                                     # floating-point emulator fixups
            # Where the same target is needed in several places the file chains them: each place
            # holds the offset of the next, ending at 0xFFFF. An "additive" record is one place.
            off, hops = r["offset"], 0
            while off != 0xFFFF and off + 1 < len(img) and hops < 0x10000:
                nxt = struct.unpack_from("<H", img, off)[0]
                src = r["src"]
                add = nxt if r["additive"] else 0
                if src == 3 and off + 3 < len(img):          # far pointer: offset, selector
                    struct.pack_into("<HH", img, off, (toff + add) & 0xFFFF, tsel)
                elif src == 2:                               # selector only
                    struct.pack_into("<H", img, off, tsel)
                elif src == 5:                               # offset only
                    struct.pack_into("<H", img, off, (toff + add) & 0xFFFF)
                elif src == 0:                               # low byte
                    img[off] = (toff + add) & 0xFF
                if r["additive"]:
                    break
                off, hops = nxt, hops + 1

    os.makedirs(outdir, exist_ok=True)
    blob = os.path.abspath(os.path.join(outdir, "describe.bin"))
    lines, pos = ["arch\t%s" % ARCH], 0
    with open(blob, "wb") as f:
        for s in segs:
            img = images[s["idx"]]
            if not img:
                continue
            f.write(img)
            lines.append("bytes\t%x\t%s\t%x\t%x" % (address(s["idx"], 0), blob, pos, len(img)))
            pos += len(img)
    data_seg = nef.h.get("autodata_seg")
    if data_seg:
        lines.append("reg\tDS\t%x" % selector(data_seg))
    exported = {}
    for nm, ordv in nef.resident_names()[1:] + nef.nonresident_names()[1:]:
        e = entries.get(ordv)
        if e and re.fullmatch(r"[A-Za-z_]\w*", nm):
            exported.setdefault((e["seg"], e["offset"]), nm)
    names, where, funcs = {}, {}, []
    for (seg, off), dist in sorted(functions(nef).items()):
        va = address(seg, off)
        nm = exported.get((seg, off)) or "sub_%d_%04X" % (seg, off)
        if nm in imports:
            nm = "sub_%d_%04X" % (seg, off)
        names[va], where[va] = nm, (seg, off)
        funcs.append((va, nm, dist))
    d = Description("", names, where, order)
    d.import_names = import_names16(nef)                                       # symbol -> the import's real name
    d.code = {s["idx"]: s["length"] for s in segs if s["kind"] == "CODE"}     # segment -> bytes of code
    d.images = {k: bytes(v) for k, v in images.items()}                       # segment -> its bytes, relocated
    d.data_segment = data_seg or 0
    d.segment_bytes = {s["idx"]: bytes(nef.data[s["file_off"]:s["file_off"] + s["length"]])
                       for s in segs if s["kind"] == "CODE" and s["file_off"]}
    for va, nm, dist in funcs:
        lines.append(model_line(d, va, nm, default=dist))
    from codebreaker.symbols import api_arguments as api16
    for nm in order:                 # Windows API: far, and the callee removes its arguments --
        size = api16.of(nm)          # how many bytes is read from the module that exports it
        lines.append("X\t%x\t%s\t%d\t__stdcall16far" % (imports[nm], nm, -1 if size is None else size))
    path = os.path.join(outdir, "description.txt")
    with open(path, "w", newline="\n", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")
    d.path = path
    return d


# Virtual device drivers (LE) ---------------------------------------------------------------------
LE_ARCH = "x86:LE:32:default:windows"
LE_BASE, LE_STRIDE = 0x01000000, 0x01000000      # object n sits at LE_BASE + (n-1) * LE_STRIDE
LE_SLOTS = 0x0F000000                            # one pointer per service called, read as that service


def le_address(obj, off):
    return LE_BASE + (obj - 1) * LE_STRIDE + off


def describe_le(lf, name, outdir):
    """Describe one LE image (`lf`, from formats.le) to the decompiler. -> Description

    A driver's objects are relocatable, so each is given an address of its own and the
    fix-ups the loader would apply are applied: a 32-bit reference to another place in the
    image gets that place's address. Ring-0 code calls the system and other drivers with
    `int 20h` followed by a service number -- six bytes the loader rewrites into a call; they
    are rewritten here into a call through a pointer that stands for that service, so the
    decompiler sees a named call instead of an interrupt and four bytes of nonsense."""
    import struct
    images = {}
    for o in lf.objects:
        b = bytearray(lf.object_bytes(o))
        images[o["idx"]] = b + bytearray(max(0, o["vsize"] - len(b)))
    execs = {o["idx"] for o in lf.objects if o["exec"]}
    starts = set()
    try:
        fixups = lf.fixups()
    except Exception:
        fixups = {}
    for (obj, off), fx in fixups.items():
        img = images.get(obj)
        if img is None or fx is None or off < 0 or off + 4 > len(img):
            continue
        if fx["kind"] != "internal" or fx.get("target") is None or fx["obj"] not in images:
            continue
        target = le_address(fx["obj"], fx["target"])
        if fx.get("srctype") == 7:                           # 32-bit offset
            struct.pack_into("<I", img, off, target & 0xFFFFFFFF)
        elif fx.get("srctype") == 8:                         # 32-bit self-relative (a call or jump)
            struct.pack_into("<I", img, off, (target - (le_address(obj, off) + 4)) & 0xFFFFFFFF)
            if fx["obj"] in execs:
                starts.add(target)
    # service calls: CD 20 <service:16> <device:16>
    services, order = {}, []
    for obj in sorted(execs):
        img = images[obj]
        i = img.find(b"\xCD\x20")
        while i != -1 and i + 6 <= len(img):
            svc, dev = struct.unpack_from("<HH", img, i + 2)
            if 0 < dev < 0x600 and (svc & 0x7FFF) < 0x400:
                key = (dev, svc & 0x7FFF)
                if key not in services:
                    services[key] = LE_SLOTS + 4 * len(order)
                    order.append(key)
                img[i:i + 2] = b"\xFF\x25" if svc & 0x8000 else b"\xFF\x15"   # jmp / call [slot]
                struct.pack_into("<I", img, i + 2, services[key])
                i = img.find(b"\xCD\x20", i + 6)
            else:
                i = img.find(b"\xCD\x20", i + 2)
    # where functions start: the control procedure, self-relative targets, agreed near calls
    try:
        ddb = lf.ddb()
    except Exception:
        ddb = None
    if ddb:
        img = images.get(ddb["obj"])
        at = ddb["rva"] + 4 + 20
        if img is not None and at + 4 <= len(img):
            cp = struct.unpack_from("<I", img, at)[0]
            if any(le_address(o, 0) <= cp < le_address(o, len(images[o])) for o in execs):
                starts.add(cp)
    for obj in sorted(execs):
        img, sites = images[obj], {}
        for i in range(len(img) - 4):
            if img[i] == 0xE8:
                t = (i + 5 + struct.unpack_from("<i", img, i + 1)[0])
                if 0 <= t < len(img):
                    sites.setdefault(t, set()).add(i)
        starts |= {le_address(obj, t) for t, ss in sites.items() if len(ss) >= 2}
    os.makedirs(outdir, exist_ok=True)
    blob = os.path.abspath(os.path.join(outdir, "image_le.bin"))
    lines, pos = ["arch\t%s" % LE_ARCH], 0
    with open(blob, "wb") as f:
        for obj in sorted(images):
            if images[obj]:
                f.write(images[obj])
                lines.append("bytes\t%x\t%s\t%x\t%x" % (le_address(obj, 0), blob, pos, len(images[obj])))
                pos += len(images[obj])
    names = {va: "sub_%d_%05X" % ((va - LE_BASE) // LE_STRIDE + 1, (va - LE_BASE) % LE_STRIDE) for va in sorted(starts)}
    for va in sorted(names):
        lines.append("F\t%x\t%s\t-1" % (va, names[va]))
    for dev, svc in order:
        lines.append("I\t%x\t%s\t-1" % (services[(dev, svc)],
                                        ("VMM_%04X" % svc) if dev == 1 else "VxD_%04X_%04X" % (dev, svc)))
    d = Description(os.path.join(outdir, "description.txt"), names, {}, [])
    d.spans = {obj: (le_address(obj, 0), len(images[obj])) for obj in execs}
    d.device = (ddb or {}).get("name") or name
    with open(d.path, "w", newline="\n", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")
    return d


# DOS programs (MZ) -------------------------------------------------------------------------------
DOS_ARCH = "x86:LE:16:Real Mode:default"
DOS_LOAD = 0x1000                                # the segment the program is loaded at


def describe_dos(module_path, outdir):
    """Describe a DOS program to the decompiler. -> Description

    The load module is placed at segment 0x1000 (address = segment * 16 + offset, the real-mode
    rule) and the header's relocations are applied: each names a word that holds a segment and
    needs the load segment added. The one function the file itself names is its entry point;
    the rest are found from the calls the decompiler shows."""
    import struct
    data = open(module_path, "rb").read()
    if data[:2] not in (b"MZ", b"ZM"):
        raise ValueError("%s: not a DOS program" % os.path.basename(module_path))
    (last, pages, nreloc, hdr_paras, _mn, _mx, _ss, _sp, _ck, ip, cs, reloc_off) = struct.unpack_from("<12H", data, 2)
    size = pages * 512 - ((512 - last) if last else 0)
    img = bytearray(data[hdr_paras * 16:size])
    for i in range(nreloc):
        off, seg = struct.unpack_from("<HH", data, reloc_off + 4 * i)
        at = seg * 16 + off
        if at + 2 <= len(img):
            struct.pack_into("<H", img, at, (struct.unpack_from("<H", img, at)[0] + DOS_LOAD) & 0xFFFF)
    os.makedirs(outdir, exist_ok=True)
    blob = os.path.abspath(os.path.join(outdir, "image_dos.bin"))
    with open(blob, "wb") as f:
        f.write(img)
    base = DOS_LOAD * 16
    entry = ((DOS_LOAD + cs) & 0xFFFF) * 16 + ip
    lines = ["arch\t%s" % DOS_ARCH, "bytes\t%x\t%s\t0\t%x" % (base, blob, len(img)),
             "reg\tDS\t%x" % DOS_LOAD, "F\t%x\tentry\t-1\t__cdecl16far" % entry]
    d = Description(os.path.join(outdir, "description.txt"), {entry: "entry"}, {}, [])
    d.spans = {1: (base, len(img))}
    d.image = bytes(img)
    d.base = base
    with open(d.path, "w", newline="\n", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")
    return d
