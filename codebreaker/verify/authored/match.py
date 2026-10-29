"""match.py -- does a period compiler turn the reconstruction back into the original's bytes?

    python -m codebreaker match <module> [--flags "/O2 /Gz"] [--try] [--json FILE]

This is NOT the measure of a reconstruction: that is whether it behaves as the original
(`measure`). It is a second, optional signal. Where a function's reconstructed C, compiled with
a compiler of the original's time, comes out as the very bytes the original has, the C is as
close to what was written as a binary can show; where it does not, nothing follows -- the same
source gives different bytes under another compiler version or another switch.

Each reconstructed function is compiled on its own with a compiler of the original's time, its
bytes are read from the object file with the places the linker fills in masked out, and compared
with the original function's bytes, masked at the same places (compare/mask.py):

    identical     every byte outside the masked places is the same
    same size     as long as the original, some bytes differ (registers, instruction choice)
    different     another length

`--try` compiles under several sets of switches and reports, per function, the best; which set
matches most functions says how the module was built.

What differs from one processor to the next is in ARCH, one entry each:

    "x86"       a 32-bit Windows module: the compiler `period_compiler` in settings.json names (a
                32-bit Microsoft C compiler that runs natively: the folder holding CL.EXE)
    "x86-16"    a 16-bit module: Microsoft C 6.0a or 8.00c (build/msc6.py); match16.py drives it
    "powerpc"   an Xbox 360 image (or .xex): the compiler `powerpc_tools` names

-- how its code is compared, the switches tried, the padding a compiler leaves after a function,
what the compiler's object says the linker fills in, and the original's functions with the places
masked that depend on where things are: relocated words, and the distance of a call or jump.

On PowerPC what may differ is masked at the level of bits, not bytes. Every instruction is one
word, and the object says, as a relocation, which instructions hold something the linker fills
in -- the 24-bit distance of a `bl`, the 14 bits of a conditional branch to another function, the
16-bit halves of an address built by `lis`/`addi` -- so only that field of the word is ignored and
its opcode and registers are still compared. The image has no such records; the object's mask is
laid over the original's function at the same places (where the two agree that far, they agree on
which words are relocated), and the original's own calls out of the function are masked as well.
"""
import json
import os
import pickle
import re
import struct
import subprocess
from concurrent.futures import ThreadPoolExecutor

from codebreaker.compare import mask as masks
from codebreaker.symbols.libraries import coff
from codebreaker import platforms

FLAG_SETS = ("/O2 /Gz", "/Oxs /Gz", "/O1 /Gz", "/Ox /Gz", "/O2 /Gz /Oy-", "/Od /Gz", "/O2 /Gd")
# Microsoft C 8.00c (Visual C++ 1.52c) built Setup's SUWIN, without /Gw, with switches that differ
# from one source file to the next: its segment 1 at /Od, its segment 3 at /G2 /Ox.
MSC8_FLAG_SETS = ("/AM /Gs /Od", "/AM /G2s /Ox", "/AM /G2s /Oxs", "/AM /G2s /Os", "/AM /Gs /Ox", "/AM /Gs /Oxs",
                  "/AM /Gsw /Od", "/AM /G2sw /Ox", "/AM /G3s /Ox", "/AM /G3s /Oxs")   # (/G3: push dword ptr)   # (/Oxs: SUWIN's sub_2_5CFC, a 2-byte cleanup as pop bx)
POWERPC_FLAG_SETS = ("/O2", "/Ox", "/O1", "/Os", "/O2 /Oy-", "/Od")
POWERPC_REV = 3                         # how a PowerPC object is read (_powerpc_objects): part of the cache key
_POWERPC_FLAGS = ["/nologo", "/c", "/TC", "/GS-", "/Zl", "/Gy", "/W0", "/FIm.h"]

# IMAGE_REL_PPC_* (winnt.h) -> which bits of the 4-byte word the linker fills in
_LI, _BD, _HALF, _WORD = 0x03FFFFFC, 0x0000FFFC, 0x0000FFFF, 0xFFFFFFFF
_FIELD = {0x03: _LI, 0x06: _LI,                            # ADDR24, REL24 (b/bl)
          0x05: _BD, 0x07: _BD,                            # ADDR14, REL14 (bc)
          0x04: _HALF, 0x08: _HALF, 0x09: _HALF, 0x0F: _HALF, 0x10: _HALF, 0x11: _HALF,
          0x13: _HALF, 0x14: _HALF,                        # ADDR16, TOCREL, SECREL16, REFHI/LO ...
          0x02: _WORD, 0x0B: _WORD}                        # ADDR32, SECREL
_NE_SIZE = {0: 1, 2: 2, 3: 4, 5: 2, 11: 6, 13: 4}           # bytes a 16-bit relocation of each source type covers


def compiler():
    """CL.EXE of the period compiler, or '' when settings.json names none."""
    from codebreaker.base import settings
    d = settings.get("period_compiler")
    exe = os.path.join(d or "", "CL.EXE")
    return exe if d and os.path.isfile(exe) else ""


# ---- what a compiler made: the functions of its object ---------------------------------------------
def _code_symbols(obj, also=lambda s: True):
    """{section: [(offset, name)]} of the symbols an object's code sections say are functions, in order."""
    funcs = {}
    for s in obj.symbols:
        if s and s["sec"] > 0 and obj.sections[s["sec"] - 1]["chars"] & 0x20 and s["type"] == 0x20 and also(s):
            funcs.setdefault(s["sec"], []).append((s["value"], s["name"]))
    for lst in funcs.values():
        lst.sort()
    return funcs


def object_functions(obj):
    """{symbol: (bytes, mask)} for the functions of an x86 COFF object file; mask has 1 for every
    byte the linker fills in (an address, the distance of a call)."""
    o = coff.Coff(obj)
    out = {}
    for sect, lst in _code_symbols(o).items():
        sec = o.sections[sect - 1]
        relocated = bytearray(sec["size"])
        for at, _sym, _type in o.relocations(sec):
            relocated[at:at + 4] = b"\1\1\1\1"
        for k, (value, name) in enumerate(lst):
            end = lst[k + 1][0] if k + 1 < len(lst) else sec["size"]
            out[name] = (obj[sec["ptr"] + value:sec["ptr"] + end], bytes(relocated[value:end]))
    return out


def _powerpc_objects(obj):
    """{symbol: (bytes, [ignored bits per word])} for the functions of a PowerPC COFF object (a
    function's symbol says so; labels inside one -- $M..., class 6 -- do not)."""
    machine = struct.unpack_from("<H", obj, 0)[0]
    if machine != coff.POWERPC:
        raise ValueError("not a PowerPC object (machine %#x)" % machine)
    o = coff.Coff(obj)
    out = {}
    for sect, lst in _code_symbols(o, lambda s: s["cls"] != 6).items():
        sec = o.sections[sect - 1]
        ignore = [0] * ((sec["size"] + 3) // 4)
        for at, _sym, kind in o.relocations(sec):
            if at // 4 < len(ignore):
                ignore[at // 4] |= _FIELD.get(kind, 0)
        for k, (value, name) in enumerate(lst):
            end = lst[k + 1][0] if k + 1 < len(lst) else sec["size"]
            out[name] = (obj[sec["ptr"] + value:sec["ptr"] + end], ignore[value // 4:(end + 3) // 4])
    return out


def _trim(code, mask):
    """Without the padding a compiler puts after a function (int 3, nop, zero bytes)."""
    n = len(code)
    while n and not mask[n - 1] and code[n - 1] in (0xCC, 0x90, 0x00) and n > 1 and code[n - 2] in (0xC3, 0xCC, 0x90, 0x00):
        n -= 1
    return code[:n], mask[:n]


def _trim_words(code, mask):
    """Without the zero words a compiler pads a function with."""
    n = len(code) // 4
    while n > 1 and code[4 * n - 4:4 * n] == b"\0\0\0\0":
        n -= 1
    return code[:4 * n], mask[:n]


# ---- the original's functions ------------------------------------------------------------------------
def original_functions(path, addresses):
    """{address: (bytes, mask)} for functions of a 32-bit module; the mask marks relocated words
    and the distances of direct calls and jumps, which depend on where things are."""
    from codebreaker.formats import pe_functions
    from codebreaker.formats.pe import open_pe
    pe = open_pe(path)
    spans = pe_functions.function_spans(path)
    relocated = set()
    va, size = pe.directory(5)
    off = pe.file_offset(va) if va else None
    end = (off or 0) + size
    while off is not None and off + 8 <= min(end, len(pe.data)):
        page, block = struct.unpack_from("<II", pe.data, off)
        if block < 8:
            break
        for k in range((block - 8) // 2):
            e = struct.unpack_from("<H", pe.data, off + 8 + 2 * k)[0]
            if e >> 12 == 3:
                relocated.add(pe.image_base + page + (e & 0xFFF))
        off += block
    md = platforms.decoder(32)
    out = {}
    for start in addresses:
        if start not in spans:
            continue
        stop = spans[start][0]
        o = pe.file_offset(start - pe.image_base)
        if o is None:
            continue
        code = pe.data[o:o + (stop - start)]
        mask = bytearray(len(code))
        for a in range(start, stop):
            if a in relocated:
                mask[a - start:a - start + 4] = b"\1\1\1\1"
        for addr, n, mnemonic, _ops in md.disasm_lite(code, start):
            first = code[addr - start]
            if first in (0xE8, 0xE9) and n == 5:
                mask[addr - start + 1:addr - start + 5] = b"\1\1\1\1"
        out[start] = _trim(code, bytes(mask[:len(code)]))
    return out


def _originals_ne(module_path, where, spans):
    """{address: (bytes, mask)} for a 16-bit module's functions: every word a relocation record of its
    segment names, and the distance of every near call and jump to another function. `where`
    {address: (segment, offset)}, `spans` {address: ((start, end), instructions)} as
    verify/lanes/win16.py makes them."""
    from codebreaker.formats.ne import NEFile
    nef = NEFile(module_path)
    segs = {s["idx"]: s for s in nef.segments()}
    by_seg = {}
    for idx, s in segs.items():
        code = nef.data[s["file_off"]:s["file_off"] + s["length"]]
        relocated = bytearray(len(code))
        for r in nef.relocations(s):
            n = _NE_SIZE.get(r["src"], 2)
            at, seen = r["offset"], 0
            while 0 <= at < len(code) and seen < 0x4000:     # the places are chained through the code
                nxt = struct.unpack_from("<H", code, at)[0] if at + 2 <= len(code) else 0xFFFF
                relocated[at:at + n] = b"\1" * min(n, len(code) - at)
                if r["additive"] or nxt == 0xFFFF:
                    break
                at, seen = nxt, seen + 1
        by_seg[idx] = (bytes(code), relocated)
    md = platforms.decoder(16)
    out = {}
    for va, (seg, _off) in where.items():
        if va not in spans or seg not in by_seg:
            continue
        (start, end), _n = spans[va]
        code, relocated = by_seg[seg]
        piece, m = code[start:end], bytearray(relocated[start:end])
        for addr, n, _mn, _ops in md.disasm_lite(piece, start):
            first = piece[addr - start]
            if first in (0xE8, 0xE9) and n == 3:             # a near call or jump: its distance
                target = (addr + 3 + struct.unpack_from("<h", piece, addr - start + 1)[0]) & 0xFFFF
                if not start <= target < end:
                    m[addr - start + 1:addr - start + 3] = b"\1\1"
        out[va] = _trim(piece, bytes(m))
    return out


def _originals_powerpc(img, addresses):
    """{address: (bytes, [ignored bits per word])} for functions of a PowerPC image: each runs to
    the length its .pdata record gives, else to the next function, and ends before the first zero
    word (the padding between functions -- no instruction is zero; a record's length can take in
    padding and a small function that has no record of its own); a `b`/`bl` that leaves the
    function has its distance masked (where the callee sits is the linker's choice)."""
    starts = sorted(set(img.functions) | set(img.imports) | set(img.inline))
    nxt = {a: b for a, b in zip(starts, starts[1:] + [img.code[1]])}
    out = {}
    for va in addresses:
        size = img.extents.get(va, (0,))[0] or (nxt.get(va, va) - va)
        code = img.bytes_at(va, size)
        for i in range(0, len(code) - 3, 4):
            if code[i:i + 4] == b"\0\0\0\0":
                code = code[:i]
                break
        if len(code) < 4:
            continue
        ignored = []
        for i in range(len(code) // 4):
            w = struct.unpack_from(">I", code, 4 * i)[0]
            ignore = 0
            if w >> 26 == 18:                                       # b / bl
                d = w & 0x03FFFFFC
                d -= 0x04000000 if d & 0x02000000 else 0
                if not (w & 2) and not (va <= va + 4 * i + d < va + len(code)):
                    ignore = _LI
            ignored.append(ignore)
        out[va] = _trim_words(code, ignored)
    return out


# ---- the processors -----------------------------------------------------------------------------------
class Arch(object):
    """One processor's part of `match`.

    unit, byteorder     how its code is compared: a byte at a time (a mask flags each byte left
                        out), or as words of `unit` bytes (a mask names the bits of each word left out)
    flag_sets           the compiler switches `--try` goes through
    trim(code, mask)    a function without the padding its compiler puts after it
    object_functions    (object file bytes) -> {symbol: (bytes, mask)}; None where the compiler's
                        output is read a module at a time (build/msc6.py)
    originals           -> {address: (bytes, mask)}, the original's functions (its arguments are
                        the processor's: a path and addresses; a path, segments and spans; an image)"""

    def __init__(self, unit, byteorder, flag_sets, trim, object_functions, originals):
        self.unit, self.byteorder, self.flag_sets = unit, byteorder, flag_sets
        self.trim, self.object_functions, self.originals = trim, object_functions, originals


ARCH = {"x86": Arch(1, "little", FLAG_SETS, _trim, object_functions, original_functions),
        "x86-16": Arch(1, "little", MSC8_FLAG_SETS, _trim, None, _originals_ne),
        "powerpc": Arch(4, "big", POWERPC_FLAG_SETS, _trim_words, _powerpc_objects, _originals_powerpc)}


# ---- comparing ------------------------------------------------------------------------------------------
def compare(built, original):
    """-> ("identical" | "same size" | "different", share of bytes equal outside masked places)"""
    (a, ma), (b, mb) = _trim(*built), original
    if len(a) != len(b):
        import difflib
        return "different", difflib.SequenceMatcher(None, a, b, autojunk=False).ratio()
    free = sum(1 for i in range(len(a)) if not ma[i] and not mb[i])
    same = free - len(masks.differing(a, b, ma, mb))
    return ("identical" if same == free else "same size"), (same / free if free else 1.0)


def distance(built, original, arch="x86"):
    """How far a compiled function is from the original, outside the places the linker fills in.
    -> (verdict, number of units -- bytes, or a PowerPC's words -- that differ, total compared,
    first differing unit or -1). When the lengths differ the whole tail counts as differing, so a
    longer or shorter function is always farther than one of the right length."""
    a = ARCH[arch]
    (x, mx), (y, my) = a.trim(*built), original
    return masks.distance(x, y, mx, my, a.unit, a.byteorder)


def relocation_targets(path):
    """The addresses the module's own relocations point at: every place in it that a word of
    code or data names by address."""
    from codebreaker.formats.pe import open_pe
    pe = open_pe(path)
    out = set()
    va, size = pe.directory(5)
    off = pe.file_offset(va) if va else None
    end = (off or 0) + size
    while off is not None and off + 8 <= min(end, len(pe.data)):
        page, block = struct.unpack_from("<II", pe.data, off)
        if block < 8:
            break
        for k in range((block - 8) // 2):
            e = struct.unpack_from("<H", pe.data, off + 8 + 2 * k)[0]
            if e >> 12 == 3:
                o = pe.file_offset(page + (e & 0xFFF))
                if o is not None and o + 4 <= len(pe.data):
                    out.add(struct.unpack_from("<I", pe.data, o)[0])
        off += block
    return out


_BOUND = re.compile(r"(?m)^#define (\w+) \(\*\(([^()]*?) ?\*\)0x([0-9a-fA-F]+)\)[ \t]*$")
_CONSTANT = re.compile(r"(-\s*)?\b0x([0-9a-fA-F]{5,8})\b")
_RESULT_OF_NOTHING = re.compile(r"(?m)^[ \t]*return in_EAX;[ \t]*\n")


def period_source(header, texts, imports, targets):
    """(header, {name: text}) as they are given to a period compiler.

    For judging behaviour the reconstruction names the original's storage by its address and
    calls imports as plain functions: that is what lets it run beside the original. The original
    was not written that way, and a compiler shows the difference in every instruction that
    touches a global or calls Windows. Here the same source is given the declarations its author
    had, which change no meaning:

      * an imported function is `__declspec(dllimport)` -- called through its slot;
      * a global is a variable, not a number cast to a pointer, and an address taken as a number
        (`0xbff10070`, where the module's relocations show that number IS an address) is the
        address of a variable -- so the compiler leaves it to the linker, as the original's did;
      * a function whose every `return` hands back "whatever was in EAX" returns nothing (when
        no caller uses a result).
    """
    lines, bound = [], {}
    for m in _BOUND.finditer(header):
        if int(m.group(3), 16) in targets:
            bound[m.group(1)] = m.group(2)
    names = {}

    def at(address):
        names[address] = "codebreaker_at_%08x" % address
        return names[address]

    def constant(m, text):
        value = int(m.group(2), 16)
        if m.group(1):
            value = (-value) & 0xFFFFFFFF
        if value not in targets:
            return m.group(0)
        before = text[:m.start()].rstrip()[-1:]
        joined = m.group(1) and (before.isalnum() or before in "_)]")
        return ("+ " if joined else "") + "(int4)&" + at(value)
    out, nothing = {}, set()
    bodies = "\n".join(t.partition("{")[2] for t in texts.values())

    def used(name):
        """Is the function's result used anywhere (a call that is not a statement of its own)?"""
        for m in re.finditer(r"\b%s\s*\(" % re.escape(name), bodies):
            before = bodies[:m.start()].rstrip()
            if before[-1:] not in ";{})" and not before.endswith("else"):
                return True
        return False
    for name, text in texts.items():
        text = _CONSTANT.sub(lambda m, t=text: constant(m, t), text)
        returns = re.findall(r"\breturn\b[^;]*;", text)
        if returns and all(r.replace(" ", "") == "returnin_EAX;" for r in returns) and not used(name):
            head, brace, body = text.partition("{")
            body = _RESULT_OF_NOTHING.sub("", re.sub(r"(?m)^[ \t]*\w+ in_EAX;[ \t]*\n", "", body))
            if "in_EAX" not in body:                     # (given a value somewhere: it is a result)
                text = re.sub(r"^\s*\w+\b", "void", head, count=1) + brace + body
                nothing.add(name)
        out[name] = text
    for line in header.split("\n"):
        m = _BOUND.match(line)
        if m and m.group(1) in bound:
            line = "extern %s %s;" % (bound[m.group(1)], m.group(1))
        else:
            f = re.match(r"^(\w+)((?: __\w+)?) (\w+)\((.*)\);[ \t]*$", line)
            if f and f.group(3) in imports and f.group(4) == "":
                line = "__declspec(dllimport) " + line
            elif f and f.group(3) in nothing:
                line = "void%s %s(%s);" % (f.group(2), f.group(3), f.group(4))
        lines.append(line)
    lines += ["extern char %s;" % n for _a, n in sorted(names.items())]
    return "\n".join(lines), out


_DECLARES = re.compile(r"^(?:__declspec\(dllimport\) )?(?:extern )?[\w ]*?[ *](\w+)(?:\(.*\)|\[\d*\])?;[ \t]*$|^#define (\w+) \(\*")


def needed_header(header, text):
    """The header with only the declarations `text` uses (types and macros are all kept).

    What this compiler makes of a function depends on what was declared before it -- a variable
    the function never mentions, declared a thousand lines earlier, can turn `cmp eax, ecx` into
    `cmp ecx, eax`. The author's file had its own declarations, not the whole module's; a function
    given just what it uses is one more context to try, and often the nearer one."""
    words = set(re.findall(r"[A-Za-z_]\w*", text))
    keep, inside = [], False
    for line in header.split("\n"):
        if inside or line.startswith("typedef struct"):          # a structure, whole or not at all
            if line.startswith("typedef struct"):
                inside = True
                wanted = line.split()[2] in words
            if wanted:
                keep.append(line)
            inside = not line.startswith("}")
            continue
        m = _DECLARES.match(line)
        if m and (m.group(1) or m.group(2)) not in words and not line.startswith("typedef"):
            continue
        keep.append(line)
    return "\n".join(keep)


# ---- compiling ---------------------------------------------------------------------------------------
def _cached(key, build):
    """What the compiler made of a text, kept (base/cache.py): a search tries the same text many times."""
    from codebreaker.base import cache
    had = cache.get("period", key)
    if had is not None:
        return pickle.loads(had)
    built = build()
    cache.put("period", key, pickle.dumps(built))
    return built


def compile_function(cl, env, header, source, name, work, flags):
    """Compile one function's C with the period compiler. -> (bytes, mask) of the function, or
    None when it does not compile. `header` is the shared types/declarations."""
    from codebreaker.base import cache
    return _cached(cache.key(cache.tool(cl), flags, header, source, name),
                   lambda: _compile_function(cl, env, header, source, name, work, flags))


def _compile_function(cl, env, header, source, name, work, flags):
    os.makedirs(work, exist_ok=True)
    with open(os.path.join(work, "m.h"), "w", newline="\n", encoding="utf-8") as f:
        f.write(header.replace("unsigned long long", "unsigned __int64").replace("long long", "__int64"))
    stem = "one"
    for ext in (".obj", ".c"):
        try:
            os.remove(os.path.join(work, stem + ext))
        except OSError:
            pass
    with open(os.path.join(work, stem + ".c"), "w", newline="\n", encoding="utf-8") as f:
        f.write('#include "m.h"\n\n' + source + "\n")
    r = subprocess.run([cl, "/nologo", "/c", "/W0"] + flags.split() + [stem + ".c"], cwd=work, env=env,
                       capture_output=True, text=True, errors="replace", timeout=300)
    if r.returncode or not os.path.isfile(os.path.join(work, stem + ".obj")):
        return None
    funcs = object_functions(open(os.path.join(work, stem + ".obj"), "rb").read())
    return next((v for k, v in funcs.items() if k.lstrip("_@").split("@")[0] == name), None) or \
        (next(iter(funcs.values())) if len(funcs) == 1 else None)


def compiler_env(cl):
    return dict(os.environ, PATH=os.path.dirname(cl) + os.pathsep + os.environ.get("PATH", ""),
                INCLUDE=os.path.join(os.path.dirname(os.path.dirname(cl)), "INCLUDE"))


def run(module_path, flags, work, jobs=4):
    """Compile every reconstructed function of a module with the period compiler and compare.
    -> {function name: (verdict, share, compiled size, original size)}; a function the compiler
    rejects has verdict "does not compile"."""
    from codebreaker.verify.lanes import win32
    cl = compiler()
    if not cl:
        raise SystemExit("no period compiler (settings.json: period_compiler -- the folder holding CL.EXE)")
    m = win32.prepare(module_path, os.path.join(work, "source"))
    out_dir = os.path.join(work, re.sub(r"\W+", "_", flags).strip("_") or "default")
    os.makedirs(out_dir, exist_ok=True)
    with open(os.path.join(out_dir, "m.h"), "w", newline="\n", encoding="utf-8") as f:
        # (the 32-bit C 8.00 of 1993 -- C13232 -- has no 64-bit integer: a function using one does not match anyway)
        wide = "double" if os.path.isfile(os.path.join(os.path.dirname(cl), "C13232.EXE")) else "__int64"
        f.write(m.header.replace("unsigned long long", "unsigned " + wide).replace("long long", wide)
                .replace("unsigned double", "double"))
    env = dict(os.environ, PATH=os.path.dirname(cl) + os.pathsep + os.environ.get("PATH", ""),
               INCLUDE=os.path.join(os.path.dirname(os.path.dirname(cl)), "INCLUDE"))
    names = list(m.order)

    def one(i):
        stem = "f%04d" % i
        with open(os.path.join(out_dir, stem + ".c"), "w", newline="\n", encoding="utf-8") as f:
            f.write('#include "m.h"\n\n' + m.texts[names[i]] + "\n")
        r = subprocess.run([cl, "/nologo", "/c", "/W0"] + flags.split() + [stem + ".c"], cwd=out_dir, env=env,
                           capture_output=True, text=True, errors="replace", timeout=300)
        if r.returncode:
            return None
        with open(os.path.join(out_dir, stem + ".obj"), "rb") as f:
            return object_functions(f.read())
    with ThreadPoolExecutor(max_workers=jobs) as ex:
        objects = list(ex.map(one, range(len(names))))
    address = {name: va for va, name in m.by_va.items()}
    originals = original_functions(module_path, sorted(address.values()))
    results = {}
    for name, funcs in zip(names, objects):
        orig = originals.get(address[name])
        if funcs is None or orig is None:
            results[name] = ("does not compile" if funcs is None else "no original extent", 0.0, 0, len(orig[0]) if orig else 0)
            continue
        mine = next((v for k, v in funcs.items() if k.lstrip("_@").split("@")[0] == name), None) or \
            (next(iter(funcs.values())) if len(funcs) == 1 else None)
        if mine is None:
            results[name] = ("does not compile", 0.0, 0, len(orig[0]))
            continue
        verdict, share = compare(mine, orig)
        results[name] = (verdict, share, len(_trim(*mine)[0]), len(orig[0]))
    return results


def compile_powerpc(cl, header, text, name, work, flags):
    """Compile one function on its own with the Xbox 360 compiler. -> (bytes, mask), or None when it
    does not compile."""
    from codebreaker.base import cache
    return _cached(cache.key(cache.tool(cl), "powerpc", POWERPC_REV, flags, header, text, name),
                   lambda: _compile_powerpc(cl, header, text, name, work, flags))


def _compile_powerpc(cl, header, text, name, work, flags):
    os.makedirs(work, exist_ok=True)
    with open(os.path.join(work, "m.h"), "w", newline="\n", encoding="utf-8") as f:
        f.write(header)
    for ext in (".obj", ".c"):
        try:
            os.remove(os.path.join(work, "one" + ext))
        except OSError:
            pass
    with open(os.path.join(work, "one.c"), "w", newline="\n", encoding="utf-8") as f:
        f.write(text + "\n")
    r = subprocess.run([cl] + _POWERPC_FLAGS + flags.split() + ["/Foone.obj", "one.c"], cwd=work,
                       capture_output=True, text=True, errors="replace", timeout=600)
    if r.returncode or not os.path.isfile(os.path.join(work, "one.obj")):
        return None
    funcs = _powerpc_objects(open(os.path.join(work, "one.obj"), "rb").read())
    return funcs.get(name) or (next(iter(funcs.values())) if len(funcs) == 1 else None)


def run_powerpc(module_path, flags, work, jobs=4):
    """Compile every reconstructed function of a PowerPC image and compare.
    -> {name: (verdict, words differing, words, original words)}"""
    from codebreaker.build import functions
    from codebreaker.verify.lanes import powerpc
    tools = functions.powerpc_tools()
    if not tools:
        raise SystemExit("no PowerPC compiler (settings.json: powerpc_tools, powerpc_lib)")
    cl = tools[0]
    m = powerpc.prepare(module_path, os.path.join(work, "source"))
    address = {name: va for va, name in m.by_va.items()}
    originals = _originals_powerpc(m.image, sorted(address.values()))
    names = list(m.order)

    def one(i):
        return compile_powerpc(cl, m.header, m.texts[names[i]], names[i], os.path.join(work, "w%d" % (i % jobs)), flags)
    by_slot = {}
    for i in range(len(names)):
        by_slot.setdefault(i % jobs, []).append(i)
    built = {}

    def slot(ids):                                           # one working folder per worker
        for i in ids:
            built[names[i]] = one(i)
    with ThreadPoolExecutor(max_workers=jobs) as ex:
        list(ex.map(slot, by_slot.values()))
    results = {}
    for name in names:
        orig = originals.get(address.get(name))
        if built[name] is None or orig is None:
            results[name] = ("does not compile" if built[name] is None else "no original extent", 0, 0,
                             len(orig[0]) // 4 if orig else 0)
            continue
        verdict, diff, words, _first = distance(built[name], orig, "powerpc")
        results[name] = (verdict, diff, words, len(orig[0]) // 4)
    return results


_ORDER = ("identical", "same size", "different", "no original extent", "does not compile")


def _main_powerpc(argv):
    from codebreaker.base import settings
    from codebreaker.formats import xex
    args = [a for a in argv if not a.startswith("--")]
    flags = argv[argv.index("--flags") + 1] if "--flags" in argv else POWERPC_FLAG_SETS[0]
    out_json = argv[argv.index("--json") + 1] if "--json" in argv else None
    args = [a for a in args if a not in (flags, out_json)]
    path = xex.basefile(args[0]) if args[0].lower().endswith(".xex") else args[0]
    work = settings.state("match", os.path.basename(path).lower())
    sets = POWERPC_FLAG_SETS if "--try" in argv else (flags,)
    best = {}
    for fl in sets:
        r = run_powerpc(path, fl, work)
        if len(sets) > 1:
            print("  %-12s %d identical" % (fl, sum(1 for v in r.values() if v[0] == "identical")), flush=True)
        for name, v in r.items():
            rank = (_ORDER.index(v[0]), v[1])
            if name not in best or rank < best[name][0]:
                best[name] = (rank, v, fl)
    tally = {}
    for _rank, v, _fl in best.values():
        tally[v[0]] = tally.get(v[0], 0) + 1
    print("%s: %d functions" % (os.path.basename(args[0]), len(best)))
    for verdict in _ORDER:
        if tally.get(verdict):
            print("  %-20s %d" % (verdict, tally[verdict]))
    if out_json:
        with open(out_json, "w", encoding="utf-8") as f:
            json.dump({n: {"verdict": v[0], "words differing": v[1], "words": v[2], "original words": v[3],
                           "flags": fl} for n, (_r, v, fl) in sorted(best.items())}, f, indent=1)
    return 0


def main(argv):
    """python -m codebreaker match <module> [--flags "/O2 /Gz"] [--try] [--json FILE]"""
    from codebreaker.base import corpus, settings
    from codebreaker.formats.pe import open_pe
    args = [a for a in argv if not a.startswith("--")]
    flags = argv[argv.index("--flags") + 1] if "--flags" in argv else FLAG_SETS[0]
    args = [a for a in args if a != flags and not a.endswith(".json")]
    if not args:
        print(__doc__)
        return 2
    path = args[0] if os.path.isfile(args[0]) else corpus.retail(args[0])
    if args[0].lower().endswith(".xex") or (path and corpus.lane(path) == "pe" and
                                            open_pe(path).machine == 0x01F2):   # PowerPC: the Xbox 360 compiler
        return _main_powerpc(argv)
    if path and corpus.lane(path) == "ne":                   # a 16-bit module: Microsoft C 6
        from codebreaker.verify.authored import match16
        return match16.main(argv)
    if not path or corpus.lane(path) != "pe":
        raise SystemExit("%s: a 32-bit or 16-bit Windows module is needed" % args[0])
    work = settings.state("match", os.path.basename(path).lower())
    sets = FLAG_SETS if "--try" in argv else (flags,)
    best, per_set = {}, {}
    for fl in sets:
        r = run(path, fl, work)
        per_set[fl] = sum(1 for v in r.values() if v[0] == "identical")
        for name, v in r.items():
            rank = ("identical", "same size", "different", "no original extent", "does not compile").index(v[0])
            if name not in best or (rank, -v[1]) < best[name][0]:
                best[name] = ((rank, -v[1]), v, fl)
        if len(sets) > 1:
            print("  %-16s %d identical" % (fl, per_set[fl]), flush=True)
    counts = {}
    for _rank, v, _fl in best.values():
        counts[v[0]] = counts.get(v[0], 0) + 1
    print("%s: %d functions compiled with the period compiler: %s" % (
        os.path.basename(path), len(best), ", ".join("%s %d" % (k, counts[k]) for k in (
            "identical", "same size", "different", "does not compile", "no original extent") if counts.get(k))))
    close = sorted(((v[1], n, v) for n, (_r, v, _f) in best.items() if v[0] != "identical"), reverse=True)[:8]
    for share, n, v in close:
        print("  %-28s %-9s %3d%% of bytes equal  (%d bytes, original %d)" % (n, v[0], 100 * share, v[2], v[3]))
    if "--json" in argv:
        with open(argv[argv.index("--json") + 1], "w", encoding="utf-8") as f:
            json.dump({n: {"verdict": v[0], "share": v[1], "size": v[2], "original": v[3], "flags": fl}
                       for n, (_r, v, fl) in best.items()}, f, indent=1, sort_keys=True)
    return 0
