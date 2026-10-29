"""runtime_library.py -- which of a module's functions came with the compiler, and which compiler.

    python -m codebreaker identify comctl32.dll
    python -m codebreaker identify --learn "Visual C++ 2.0 C runtime=C:\\msvc20\\lib\\libc.lib" ...

    from codebreaker.symbols import runtime_library
    runtime_library.identify(path)      {address: (name, library)}
    runtime_library.compiler(path)      {"linker": "2.50", "compiler": "Microsoft Visual C++ 2.x", "evidence": [...]}

Nobody wrote memcpy, the 64-bit multiply helper, the stack probe or the start-up code for the
module they turn up in: they are linked in from the compiler's libraries. Such a function is
IDENTIFIED -- named, with the library it is from -- not reconstructed as if the module's author
had written it.

How: the libraries themselves are the evidence. Each function in a period library (the C
runtime, MFC's static library) is reduced to a signature -- its length, the places the linker
fills in (calls to other functions, addresses of data: listed by the library's own relocation
records, so nothing is guessed), and a digest of every other byte. A function in a module is
that library function when all of those bytes are the same. The signatures are in
runtime_signatures.json beside this file; `identify --learn` makes them from library files
(32-bit archives of objects, 16-bit libraries of object modules: read by symbols/libraries).

Two more things are known without signatures: a module that IS a runtime library (CRTDLL,
MSVCRT20, an MFC DLL) -- its exports are that library's functions; and exports of such a
library that a module merely imports are already named by the import.

Addresses are as the decompiler's descriptions use them: the address for a 32-bit module,
describe.address(segment, offset) for a 16-bit one.
"""
import hashlib
import json
import os
import re
import struct

from codebreaker.base import corpus
from codebreaker.compare import mask as masks
from codebreaker.decompile import describe
from codebreaker.formats.ne import NEFile
from codebreaker.formats.pe import open_pe
from codebreaker.symbols import libraries, ordinals

_FILE = os.path.join(os.path.dirname(os.path.abspath(__file__)), "runtime_signatures.json")
_LONGEST = 1024                 # bytes of a function that are compared
_FEWEST = 10                    # bytes the linker does not touch that a signature must have
_LEAD = 6                       # signatures are looked up by this many leading bytes (fewer when they are masked)
_index = None

# modules that are a runtime library themselves: stem -> what their exports are
ITSELF = {"CRTDLL": "C runtime (this module is the runtime library)",
          "MSVCRT": "C runtime (this module is the runtime library)",
          "MSVCRT10": "C runtime (this module is the runtime library)",
          "MSVCRT20": "C runtime (this module is the runtime library)",
          "MSVCRT40": "C runtime (this module is the runtime library)",
          "MSVCIRT": "C++ stream runtime (this module is the runtime library)",
          "MFC30": "MFC 3.0 (this module is the class library)", "MFCO30": "MFC 3.0 OLE (this module is the class library)",
          "MFCD30": "MFC 3.0 database (this module is the class library)",
          "MFCN30": "MFC 3.0 sockets (this module is the class library)",
          "MFC40": "MFC 4.0 (this module is the class library)", "MFCO40": "MFC 4.0 OLE (this module is the class library)",
          "MFC250": "MFC 2.5 (this module is the class library)"}


# ---- signatures ----------------------------------------------------------------------------------
def signature(code, mask):
    """(size, mask text, digest) of a function's bytes with what its byte mask leaves out left out
    (compare/mask.py), or None when too little is left to tell it from anything else."""
    size = min(len(code), _LONGEST)
    masked = [o for o in range(min(size, len(mask))) if mask[o]]
    if size - len(masked) < _FEWEST:
        return None
    body = masks.blank(code[:size], masked)
    if len(set(body)) < 4:                                   # padding, a table of zeros
        return None
    runs, i = [], 0
    while i < len(masked):
        j = i
        while j + 1 < len(masked) and masked[j + 1] == masked[j] + 1:
            j += 1
        runs.append("%x.%x" % (masked[i], j - i + 1))
        i = j + 1
    return size, ",".join(runs), hashlib.sha1(body).hexdigest()[:16]


def _offsets(mask_text):
    out = []
    for run in mask_text.split(",") if mask_text else ():
        start, n = run.split(".")
        out += range(int(start, 16), int(start, 16) + int(n, 16))
    return out


def _signatures():
    """{"32"|"16": {leading bytes: [(size, masked offsets, digest, name, library)]}}"""
    global _index
    if _index is None:
        from codebreaker import knowledge
        _index = {}
        for bits in ("32", "16"):                            # (runtime_signatures.json, as the knowledge base has it)
            by_lead = {}
            for lead, size, mask, digest, name, library in knowledge.signatures(bits):
                by_lead.setdefault(bytes.fromhex(lead), []).append((size, _offsets(mask), digest, name, library))
            _index[bits] = by_lead
    return _index


def match(bits, code):
    """(name, library) of the library function `code` starts with, or None."""
    table = _signatures()[str(bits)]
    for n in range(_LEAD, 0, -1):
        for size, masked, digest, name, library in table.get(bytes(code[:n]), ()):
            if size <= len(code):
                if hashlib.sha1(masks.blank(code[:size], masked)).hexdigest()[:16] == digest:
                    return name, library
    return None


# ---- identifying -----------------------------------------------------------------------------------
def _c_name(symbol):
    """A linker symbol as the C name: `_memcpy` -> memcpy, `__allmul` -> _allmul, `_f@8` -> f."""
    if symbol.startswith("?"):
        return symbol
    name = symbol[1:] if symbol.startswith("_") else symbol
    return name.split("@")[0] if "@" in name and name.rsplit("@", 1)[1].isdigit() else name


THUNK_COMPILER = "Microsoft thunk compiler: a 32-to-16-bit flat thunk"


def _flat_thunk_stubs(module_path, pe, sections):
    """The functions of a flat-thunk module (build/flat_thunk.classify) that the thunk compiler
    generated: each reaches the 16-bit side through a pointer kept in the code section itself
    (QT_Thunk's, filled in by ThunkConnect32 when the DLL is loaded), which no compiled C does.
    They are the thunk compiler's output, rebuilt from the .thk script (`flat-thunk`), not C --
    and cannot run here: mnmcpi32.dll's 297 such functions all called into nothing."""
    from codebreaker.build import flat_thunk
    from codebreaker.formats import pe_functions
    import capstone
    try:
        if flat_thunk.classify(module_path).get("subtype") != "flat-thunk":
            return []
    except Exception:                                       # (not readable as one: not one)
        return []
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    in_code = lambda a: any(b <= a < b + len(raw) for b, raw, _n in sections)
    def reaches(va, hops):
        """Whether the code at `va`, followed through `hops` more jumps, calls through such a pointer."""
        for base, raw, _name in sections:
            if base <= va < base + len(raw):
                for ins in md.disasm(raw[va - base:va - base + 512], va):
                    if ins.mnemonic == "call" and ins.op_str.startswith("dword ptr [0x"):
                        if in_code(int(ins.op_str[len("dword ptr ["):-1], 16)):
                            return True
                    if ins.mnemonic == "jmp" and ins.op_str.startswith("0x"):
                        # (a short stub sets up and jumps to the common thunk code: `mov cl, 3 / jmp`)
                        return hops > 0 and reaches(int(ins.op_str, 16), hops - 1)
                    if ins.mnemonic in ("ret", "retn", "int3"):
                        return False
        return False
    return [va for va in sorted(pe_functions.carve(module_path).funcs) if reaches(va, 1)]


def identify(module_path):
    """{address: (name, library)} for the functions of a module that belong to a runtime library."""
    lane = corpus.lane(module_path)
    stem = os.path.splitext(os.path.basename(module_path))[0].upper()
    out = {}
    if lane == "pe":
        from codebreaker.formats import pe_functions
        pe = open_pe(module_path)
        if pe.machine != 0x014C:
            return out
        sections = pe.code_sections()
        for va in sorted(pe_functions.carve(module_path).funcs):
            for base, raw, _name in sections:
                if base <= va < base + len(raw):
                    hit = match(32, raw[va - base:va - base + _LONGEST])
                    if hit:
                        out[va] = hit
                    break
        for va in _flat_thunk_stubs(module_path, pe, sections):
            out.setdefault(va, ("thunk stub", THUNK_COMPILER))
        if stem in ITSELF:
            unnamed = ordinals.table(stem)
            for o, nm, rva in pe.exports():
                nm = nm or unnamed.get(o)
                if nm and any(b <= pe.image_base + rva < b + len(raw) for b, raw, _n in sections):
                    out.setdefault(pe.image_base + rva, (nm, ITSELF[stem]))
    elif lane == "ne":
        nef = NEFile(module_path)
        segs = {s["idx"]: s for s in nef.segments()}
        for (seg, off) in sorted(describe.functions(nef)):
            s = segs[seg]
            code = nef.data[s["file_off"] + off:s["file_off"] + min(s["length"], off + _LONGEST)]
            hit = match(16, code)
            if hit:
                out[describe.address(seg, off)] = hit
        stem = nef.module_name().upper()
        if stem in ITSELF:
            names = ordinals.table(stem)
            for e in nef.entries():
                if e.get("kind") in ("FIXED", "MOVEABLE") and segs.get(e["seg"], {}).get("kind") == "CODE" \
                        and names.get(e["ordinal"]):
                    out.setdefault(describe.address(e["seg"], e["offset"]), (names[e["ordinal"]], ITSELF[stem]))
    return out


# ---- which compiler ----------------------------------------------------------------------------------
_LINKERS = {(2, 50): "Microsoft Visual C++ 2.x", (2, 55): "Microsoft Visual C++ 2.x", (2, 60): "Microsoft Visual C++ 2.x",
            (3, 0): "Microsoft Visual C++ 4.0", (3, 10): "Microsoft Visual C++ 4.0/4.1", (4, 20): "Microsoft Visual C++ 4.2",
            (5, 0): "Microsoft Visual C++ 5.0", (5, 10): "Microsoft Visual C++ 5.0", (5, 12): "Microsoft Visual C++ 5.0",
            (6, 0): "Microsoft Visual C++ 6.0", (2, 25): "Borland C++ (TLINK32)"}
# 16-bit: the segmented linker's version says which generation of Microsoft's tools linked the
# module (the compiler or assembler itself is not recorded in the file)
_LINKERS16 = {(5, 10): "Microsoft tools of the C 6.0 generation", (5, 13): "Microsoft tools of the C 6.0 generation",
              (5, 15): "Microsoft tools of the C 6.0 generation", (5, 30): "Microsoft tools of the C/C++ 7.0 generation",
              (5, 50): "Microsoft tools of the Visual C++ 1.0 generation (16-bit)",
              (5, 60): "Microsoft tools of the Visual C++ 1.5 generation (16-bit)"}
_MARKS = ((b"Borland C++ - Copyright", "Borland C++"), (b"Borland Intl", "Borland C++"),
          (b"WATCOM C/C++", "Watcom C/C++"), (b"Microsoft Visual C++ Runtime Library", "Microsoft Visual C++"),
          (b"MS Run-Time Library", "Microsoft C"), (b"Microsoft (R) C ", "Microsoft C"),
          (b"__CxxFrameHandler", "Microsoft Visual C++ (C++ exception handling)"))
_RUNTIME_DLLS = {"CRTDLL": "the system C runtime (CRTDLL)", "MSVCRT20": "Visual C++ 2.x's runtime DLL",
                 "MSVCRT40": "Visual C++ 4.x's runtime DLL", "MSVCRT": "Visual C++ 4.2 or later's runtime DLL",
                 "MSVCRT10": "Visual C++ 1.0 (32-bit)'s runtime DLL", "MFC30": "MFC 3.0 (Visual C++ 2.x)",
                 "MFC40": "MFC 4.0 (Visual C++ 4.x)", "MFC250": "MFC 2.5 (Visual C++ 1.5)",
                 "VBRUN300": "Visual Basic 3.0", "MSVBVM50": "Visual Basic 5.0"}


def _rich(data, end):
    """[(product id, build, count)] from the block Visual C++ 6 and later leave before the PE
    header, or []."""
    at = data.rfind(b"Rich", 0x40, end)
    if at < 0:
        return []
    key = struct.unpack_from("<I", data, at + 4)[0]
    out, p = [], at - 8
    while p >= 0x40:
        a, b = (x ^ key for x in struct.unpack_from("<II", data, p))
        if a == 0x536E6144:                                  # "DanS": where the block starts
            break
        out.append((a >> 16, a & 0xFFFF, b))
        p -= 8
    return out[::-1]


def compiler(module_path, identified=None):
    """What built a module, from what the file itself says: the linker's version in the header,
    marks the runtime library leaves in the image, the runtime DLLs it imports, and which
    library its identified functions came from."""
    lane = corpus.lane(module_path)
    with open(module_path, "rb") as f:
        data = f.read()
    evidence, guess, linker, uses = [], "", "", []
    if lane == "pe":
        pe = open_pe(module_path)
        major, minor = (int(x) for x in pe.build_meta()["linker_version"].split("."))
        linker = "%d.%02d" % (major, minor)
        guess = _LINKERS.get((major, minor), "")
        evidence.append("linker version %s in the header" % linker)
        rich = _rich(data, pe.e_lfanew)
        if rich:
            evidence.append("tool block before the header: " + ", ".join("%d.%d x%d" % r for r in rich[:6]))
        uses = sorted({os.path.splitext(d)[0].upper() for d, _n in pe.imports().values()})
    elif lane == "ne":
        nef = NEFile(module_path)
        linker = "%d.%02d" % (nef.h["linker_ver"], nef.h["linker_rev"])
        guess = _LINKERS16.get((nef.h["linker_ver"], nef.h["linker_rev"]), "")
        evidence.append("linker version %s in the header" % linker)
        uses = [m.upper() for m in nef.module_refs()]
    for mark, who in _MARKS:
        if mark in data:
            evidence.append("the text %r" % mark.decode("latin1"))
            if not guess or who.startswith(("Borland", "Watcom")):
                guess = who
    for stem in uses:
        if stem in _RUNTIME_DLLS:
            evidence.append("imports %s: %s" % (stem, _RUNTIME_DLLS[stem]))
    libs = {}
    for _name, library in (identify(module_path) if identified is None else identified).values():
        libs[library] = libs.get(library, 0) + 1
    for library, n in sorted(libs.items(), key=lambda kv: -kv[1])[:3]:
        evidence.append("%d functions from %s" % (n, library))
    if not guess and libs:
        guess = max(libs, key=libs.get).split(":")[0]            # a library is described as "<compiler>: <library>"
    return {"linker": linker, "compiler": guess or "not established", "evidence": evidence}


# ---- making runtime_signatures.json ------------------------------------------------------------
_FRAME = re.compile(
    r"(push|pop) (e?bp|e?si|e?di|e?bx|ds|es)$|mov (e?bp, e?sp|e?sp, e?bp|ds, ax|ax, ds|es, ax)$|(inc|dec) bp$|nop|leave|"
    r"ret|retf|mov (e?ax|e?dx), (0x)?[0-9a-f]+$|xor (e?ax, e?ax|e?dx, e?dx)$|(sub|add) (e?sp|bp), |lea sp, |int3")


def substance(bits, code, size):
    """How many instructions of a function do something of its own: not the frame every function
    sets up and takes down, not `return 0`. An empty function is the same bytes in every program
    and every library; only one with substance can be told to be a particular library function."""
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32 if bits == 32 else capstone.CS_MODE_16)
    return sum(1 for _a, _s, mnemonic, operands in md.disasm_lite(bytes(code[:size]), 0)
               if not _FRAME.match((mnemonic + " " + operands).strip()))


def learn(sources, log=print):
    """Rewrite runtime_signatures.json from [(what the library is, file)], earlier ones first:
    when two libraries hold the same function, it is reported as the earlier one's.

    Kept out: a function with too little substance to be told from anybody's (fewer than 4
    instructions of its own; 8 for a class library, whose thousands of small methods look like
    everyone's small methods), and any signature two differently named functions share once the
    linker's bytes are left out -- which of them a match would be is not known."""
    labels, found, names = [], {}, {}
    for tag, path in sources:
        try:
            bits, functions = libraries.functions(path)
        except OSError as e:
            log("%s: %s" % (path, e))
            continue
        label = "%s (%s)" % (tag, os.path.basename(path).upper())
        need = 8 if "MFC" in tag else 4
        added = 0
        if bits not in (16, 32):
            log("%s: not x86 code" % path)
            continue
        for symbol, code, mask, _at in functions:
            sig = signature(code, mask)
            lead = 0
            while lead < _LEAD and lead < len(code) and not mask[lead]:
                lead += 1
            if not sig or not lead:
                continue
            key = (bits, sig)
            names.setdefault(key, set()).add(_c_name(symbol))
            if key in found or substance(bits, code, sig[0]) < need:
                continue
            if label not in labels:
                labels.append(label)
            found[key] = [bytes(code[:lead]).hex(), sig[0], sig[1], sig[2], _c_name(symbol), labels.index(label)]
            added += 1
        log("%-60s %5d functions, %5d new signatures" % (label, len(functions), added))
    unsure = [k for k in found if k[1][1] and len(names[k]) > 1]
    for k in unsure:
        del found[k]
    log("%d signatures left out: shared by differently named functions" % len(unsure))
    entries = {bits: [e for (b, _s), e in found.items() if str(b) == bits] for bits in ("32", "16")}
    with open(_FILE, "w", encoding="utf-8", newline="\n") as f:
        f.write('{"libraries": %s,\n' % json.dumps(labels))
        f.write(",\n".join('"%s": [\n%s\n]' % (bits, ",\n".join(json.dumps(e, separators=(",", ":")) for e in entries[bits]))
                           for bits in ("32", "16")) + "\n}\n")
    global _index
    from codebreaker.knowledge import sources
    sources.changed(_FILE)                                   # the knowledge base loads it again
    _index = None
    return {bits: len(v) for bits, v in entries.items()}


def main(argv):
    """python -m codebreaker identify <module>                  its runtime-library functions and its compiler
    python -m codebreaker identify --learn "WHAT=FILE" ...   make the signatures from library files"""
    if not argv:
        print(main.__doc__)
        return 2
    if argv[0] == "--learn":
        print("%(32)d 32-bit and %(16)d 16-bit signatures" % learn([a.split("=", 1) for a in argv[1:] if "=" in a]))
        return 0
    path = corpus.retail(argv[0])
    if not path:
        raise SystemExit("no module %r in the corpus" % argv[0])
    found = identify(path)
    lane = corpus.lane(path)
    for address in sorted(found):
        name, library = found[address]
        where = "%d:%04x" % describe.where(address) if lane == "ne" else "%08x" % address
        print("%s  %-40s %s" % (where, name[:40], library))
    total = len(describe.functions(NEFile(path))) if lane == "ne" else len(describe.function_starts(path)) if lane == "pe" else 0
    info = compiler(path, found)
    print("%s: %d of %d functions are a runtime library's" % (os.path.basename(path).lower(), len(found), total))
    print("built with: %s (linker %s)" % (info["compiler"], info["linker"] or "?"))
    for line in info["evidence"]:
        print("    " + line)
    return 0
