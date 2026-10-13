"""image16.py -- describe a 16-bit Windows module (NE) to the decompiler.

A 16-bit module is not one block of memory. It is a set of segments, each up to 64 KB, that the
loader places wherever it likes and names by selector; code refers to another segment or to an
imported function through places in the file the loader patches. So before the decompiler can
read it, this does what the loader does:

  * gives each segment a selector (segment 1 -> 0x1000, then every 8) and so an address
    (selector * 0x10000 -- the 16-bit protected-mode rule the decompiler applies);
  * patches every relocation: a reference to another segment gets that segment's selector, a
    reference to an entry point gets its selector and offset, and a reference to an imported
    function gets a far pointer to an address standing for that function;
  * says where the functions are (the entry table, relocation targets, and near-call targets
    two call sites agree on), which are far and which near, and what the data segment register
    holds.

    from codebreaker.decompile import image16
    d = image16.describe(module_path, outdir)
    d.path           the description file for decompiler.run()
    d.names          {address: name}
    d.where          {address: (segment, offset)}
"""
import os
import re
import struct

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


def describe(module_path, outdir):
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
    blob = os.path.abspath(os.path.join(outdir, "image16.bin"))
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
    d.code = {s["idx"]: s["length"] for s in segs if s["kind"] == "CODE"}     # segment -> bytes of code
    d.images = {k: bytes(v) for k, v in images.items()}                       # segment -> its bytes, relocated
    d.data_segment = data_seg or 0
    d.segment_bytes = {s["idx"]: bytes(nef.data[s["file_off"]:s["file_off"] + s["length"]])
                       for s in segs if s["kind"] == "CODE" and s["file_off"]}
    for va, nm, dist in funcs:
        lines.append(model_line(d, va, nm, default=dist))
    for nm in order:                                         # Windows API: far, callee removes arguments
        lines.append("X\t%x\t%s\t-1\t__stdcall16far" % (imports[nm], nm))
    path = os.path.join(outdir, "description.txt")
    with open(path, "w", newline="\n", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")
    d.path = path
    return d
