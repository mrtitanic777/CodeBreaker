#!/usr/bin/env python3
"""
ppcdiscover.py — function discovery for Xbox 360 images (the recompiler's Analyze
phase, in our own code).

The MinecraftPC/PixarUP debugging showed that function *discovery*, not
translation, is where a static recompiler lives or dies: a missed entry silently
no-ops a vtable thunk and crashes the game. This module finds function
boundaries the way the reference (rexglue-sdk phase_register/phase_discover) does,
strongest source first:

  1. **.pdata (exception/unwind table) — authoritative.** MSVC emits an
     IMAGE_CE_RUNTIME_FUNCTION per function: an 8-byte record {BeginAddress u32,
     packed u32}, big-endian in the image. The packed dword is
     PrologLength:8 | FunctionLength:22 | 32bit:1 | ExceptionFlag:1, so function
     size = FunctionLength * 4. This alone recovers the large majority of
     functions, each with an exact extent.
  2. **Entry point** (from the XEX header).
  3. **Iterative bl-target descent.** Disassemble each known function; every
     direct `bl` target is itself a function. Repeat to fixpoint.

Not yet done (the reference also does these; see ROADMAP): vtable/function-table
pointer scanning for indirectly-referenced methods, and switch-table case-body
sealing. Those are the tail the config files patch by hand.

    python ppcdiscover.py default.xex                 # summary + sources
    python ppcdiscover.py default.xex --list | head
"""
import argparse
import struct
import sys

import disasm_ppc

WORD = 4


def _u32_be(b, o):
    return struct.unpack_from(">I", b, o)[0]


def from_pdata(img, sections, image_base):
    """{func_va: size} from the .pdata RUNTIME_FUNCTION table (exact extents)."""
    out = {}
    pd = next((s for s in sections if s["name"].lower().startswith(".pdata")), None)
    if pd is None:
        return out
    # basefile is the loaded image -> section content at its VirtualAddress
    data = img[pd["vaddr"]: pd["vaddr"] + pd["raw_size"]]
    for o in range(0, len(data) - 7, 8):
        begin = _u32_be(data, o)
        packed = _u32_be(data, o + 4)
        if begin == 0 and packed == 0:
            continue
        func_len = (packed >> 8) & 0x3FFFFF        # 22-bit FunctionLength
        size = func_len * WORD
        if begin >= image_base and size:
            out[begin] = size
    return out


_PROLOGUE = {"mflr", "stwu", "stw", "std", "mr", "li", "lis", "or", "addi",
             "stmw", "b", "clrlwi", "rlwinm", "cmpwi", "cmplwi", "lwz", "add"}


def from_pointers(img, sections, image_base, code_ranges, known_extents):
    """Function starts referenced as POINTERS in the image (vtables / function
    tables) that control-flow discovery misses — the class that silently no-ops a
    vtable thunk and crashes the game (MinecraftPC/PixarUP). Scan non-code data
    for 4-aligned big-endian words that land on a 4-aligned code address whose
    first instruction is a plausible function prologue AND is not interior to an
    already-known function (those are mid-function refs, not new entries).

    Returns a set of candidate function addresses."""
    def in_code(a):
        return any(lo <= a < hi for lo, hi in code_ranges)

    # sort extents once; interior() is then O(log n) via bisect (was O(n) per
    # candidate — the whole-image scan made that the dominant cost).
    import bisect
    ext = sorted(known_extents)
    starts = [s for s, _ in ext]

    def interior(a):
        i = bisect.bisect_right(starts, a) - 1     # rightmost start <= a
        if i < 0:
            return False
        s, sz = ext[i]
        return s < a < s + sz                       # a == start is not interior

    # index code section bodies for the prologue check (flat/virtual layout)
    code_bodies = [(image_base + s["vaddr"], img[s["vaddr"]: s["vaddr"] + s["raw_size"]])
                   for s in sections if s["code"]]

    def prologue_ok(a):
        for sv, body in code_bodies:
            if sv <= a < sv + len(body):
                off = a - sv
                ins = next(iter(disasm_ppc.listing(body, sv, start=off, count=1)), None)
                return ins is not None and ins[2] in _PROLOGUE
        return False

    out = set()
    for s in sections:
        if s["code"]:
            continue                       # scan data/rodata, not code
        data = img[s["vaddr"]: s["vaddr"] + s["raw_size"]]
        for o in range(0, len(data) - 3, 4):
            a = struct.unpack_from(">I", data, o)[0]
            if a & 3 or not in_code(a) or interior(a):
                continue
            if prologue_ok(a):
                out.add(a)
    return out


def discover(xex_path, pointer_scan=True, extra=None):
    """Return (funcs, sources) where funcs = {va: size} and sources counts how
    many came from each origin.

    `extra` is an optional iterable of guest addresses to seed as function
    entries — used to feed back indirect-call targets observed at runtime that
    static discovery (pdata + pointer scan + bl-descent) didn't reach. They are
    seeded BEFORE bl-descent so their callees are discovered transitively."""
    import xex
    x = xex.open_xex(xex_path)
    img = x.basefile()
    base = x.image_base or 0
    secs = xex._pe_sections(img)

    try:
        import progress as _pg
        _pg.caption("Discovering functions")
    except Exception:
        _pg = None

    def _say(msg):
        if _pg:
            _pg.status(msg)

    funcs = {}
    sources = {"pdata": 0, "entry": 0, "bl-descent": 0, "pointers": 0, "seed": 0}

    # 0) runtime-observed indirect-call targets fed back in (seed before descent)
    for a in (extra or ()):
        if a not in funcs:
            funcs[a] = 0
            sources["seed"] += 1

    # 1) .pdata — authoritative starts + sizes
    _say("Reading .pdata function table...")
    pd = from_pdata(img, secs, base)
    funcs.update(pd)
    sources["pdata"] = len(pd)

    # 2) entry point
    if x.entry_point and x.entry_point not in funcs:
        funcs[x.entry_point] = 0
        sources["entry"] = 1

    # map code sections for disassembly (flat/virtual layout)
    code_secs = [(base + s["vaddr"], img[s["vaddr"]: s["vaddr"] + s["raw_size"]])
                 for s in secs if s["code"]]

    # 2b) vtable/function-table pointer scan — recovers indirectly-referenced
    #     methods that control flow never reaches (the crash-causing tail).
    if pointer_scan:
        _say("Scanning vtable/function-table pointers...")
        code_ranges = [(sv, sv + len(body)) for sv, body in code_secs]
        known_extents = [(a, s) for a, s in pd.items()]     # pdata start+size
        for a in from_pointers(img, secs, base, code_ranges, known_extents):
            if a not in funcs:
                funcs[a] = 0
                sources["pointers"] += 1

    def code_at(va):
        for sv, body in code_secs:
            if sv <= va < sv + len(body):
                return sv, body
        return None, None

    # 3) iterative bl-target descent to fixpoint. Scan raw opcode bits (a `bl`
    #    is I-form opcode 18 with LK=1) instead of a full Capstone decode per
    #    instruction — pure integer work, ~10x faster on a whole image.
    _say("Following the call graph (bl targets)...")
    # XL-form (opcode 19) extended opcodes that end a run: bclr(16)/bcctr(528)/rfid(18)
    _XL_ENDERS = frozenset((16, 528, 18))
    worklist = list(funcs)
    seen = set(funcs)
    while worklist:
        va = worklist.pop()
        sv, body = code_at(va)
        if body is None:
            continue
        size = funcs.get(va) or 0
        end = va + size if size else min(sv + len(body), va + 0x4000)
        off = va - sv
        limit = end - sv
        while off + WORD <= limit:
            w = int.from_bytes(body[off:off + WORD], "big")
            op = w >> 26
            if op == 18:                            # b / bl (I-form)
                aa, lk = (w >> 1) & 1, w & 1
                li = w & 0x03FFFFFC
                if li & 0x02000000:
                    li -= 0x04000000                # sign-extend 26-bit
                if aa == 0:
                    t = (sv + off) + li
                    if lk:                          # bl -> a call: target is a func
                        if t not in seen and code_at(t)[1] is not None:
                            seen.add(t)
                            funcs.setdefault(t, 0)
                            worklist.append(t)
                            sources["bl-descent"] += 1
                    elif not size:                  # unconditional b ends a no-pdata run
                        break
            elif not size and op == 19 and ((w >> 1) & 0x3FF) in _XL_ENDERS:
                break                               # blr/bctr/rfid ends a no-pdata run
            off += WORD
    return funcs, sources, x, img, secs


def main():
    ap = argparse.ArgumentParser(description="Xbox 360 function discovery")
    ap.add_argument("path")
    ap.add_argument("--list", action="store_true", help="print every function va,size")
    args = ap.parse_args()

    funcs, sources, x, img, secs = discover(args.path)
    if args.list:
        for va in sorted(funcs):
            print("0x%08X %d" % (va, funcs[va]))
        return
    sized = sum(1 for v in funcs.values() if v)
    print("%s" % args.path)
    print("  entry 0x%08X, image base 0x%08X" % (x.entry_point or 0, x.image_base or 0))
    print("  functions discovered: %d" % len(funcs))
    print("    from .pdata (exact extent): %d" % sources["pdata"])
    print("    from entry point          : %d" % sources["entry"])
    print("    from vtable/pointer scan   : %d" % sources["pointers"])
    print("    from bl-target descent     : %d" % sources["bl-descent"])
    print("  with known size: %d ; size-unknown (need sweep): %d"
          % (sized, len(funcs) - sized))


if __name__ == "__main__":
    main()
