#!/usr/bin/env python3
r"""link95.py -- RUNG 2b: LINK modules across boundaries (whole-OS scale).

Takes rung-1/2a compiling C and makes modules LINK together -- across all three Win95
executable formats (PE 32-bit, NE 16-bit, LE/VxD 32-bit flat):
  1. NAMESPACE every module's `sub_<off>` symbols with the module prefix (KERNEL32_sub_...)
     so they're globally unique.
  2. EXPORT ALIAS: for each export (name/ordinal -> offset) emit a `__attribute__((alias(...)))`
     symbol MOD_<Name> / MOD_ord_<n> on the def at that offset. Uncarved exports get a marked
     `--stub` placeholder.
  3. IMPORT REWRITE: each import call site -> `((u32)&<targetdll>_<Name>)`, resolved through the
     importing module's imports. PE: the IAT slot in `*(u32*)(0x<slot>)`. NE: the far-call target
     the byte-exact disasm comment carries (`-> USER.POSTAPPMESSAGE`), recovered by il.py. LE/VxD:
     the DDB service table (le.ddb_services()).
Then compile each module and partial-link (`ld -r`, via @response-file) them, and report how many
cross-module symbols resolve. Run from the win95 data dir (so target.resolve finds the binaries).

    python link95.py kernel32.dll user32.dll gdi32.dll   # a few modules
    python link95.py --all --stub                        # the whole OS (PE + NE + LE)
"""
import glob
from src.core import cores
import os
import re
import struct
import subprocess
import sys
import tempfile

HERE = os.path.dirname(os.path.abspath(__file__))                # engine/recompile95/tools
INC = os.path.join(os.path.dirname(HERE), "include")             # engine/recompile95/include
_ENGINE = os.path.dirname(os.path.dirname(HERE))                 # the shared core: engine/
sys.path.insert(0, HERE)
sys.path.insert(0, os.path.join(_ENGINE, "targets", "win95"))   # win95 tools (target, pe, ne, le)
sys.path.insert(0, _ENGINE)                                      # shared engine
import rc          # noqa: E402
target = cores.get('target')
from src.formats import pe
from src.formats import ne as ne_mod
le_mod = cores.get('le')

GEN = os.path.join(os.environ.get("WT_DATA_ROOT") or os.path.join(   # win95 corpus (wt sets
    os.path.dirname(os.path.dirname(_ENGINE)), "win95"),             # WT_DATA_ROOT to it)
    "src", "generated")


def prefix(mod):
    return re.sub(r'\W', '_', mod.rsplit('.', 1)[0]).upper()   # kernel32.dll -> KERNEL32


def cid(s):
    """A valid C identifier fragment: export/import names carry @ (stdcall _Foo@4), ?, .,
    etc. that are illegal in C symbols -> map to _."""
    return re.sub(r'\W', '_', s)


def cfile(mod):
    fs = glob.glob(os.path.join(GEN, "**", mod + ".c"), recursive=True)
    return fs[0] if fs else None


def sym_maps(mod):
    """(exports: [(name_or_None, ordinal, off)], imports: {slot_va:(dll,name)}) with export
    RVAs converted to section-relative offsets (matching the C's sub_<off> names)."""
    P = pe.PEFile(target.resolve(mod))
    code = [s for s in P.sections if s['chars'] & 0x20000000]

    def rva2off(r):
        for s in code:
            if s['vaddr'] <= r < s['vaddr'] + s['vsize']:
                return r - s['vaddr']
        return None
    exps = []
    for ordi, name, rva in P.exports():
        off = rva2off(rva)
        if off is not None:
            exps.append((name, ordi, off))
    # code sections as (seg_index_1based, vaddr, vsize) + a seg->vaddr map, so a call target
    # can be resolved across sections (the lift labels inter-section calls in wrapped,
    # caller-section-relative coordinates -> huge 0xffff.... values).
    segs = [(i + 1, s['vaddr'], s['vsize']) for i, s in enumerate(code)]
    segvaddr = {i + 1: s['vaddr'] for i, s in enumerate(code)}
    return exps, P.imports(), segs, segvaddr


def is_ne(path):
    try:
        d = open(path, "rb").read()
        return d[:2] == b"MZ" and d[struct.unpack_from("<I", d, 0x3c)[0]:][:2] == b"NE"
    except Exception:
        return False


def sym_maps_ne(mod):
    """NE (16-bit) analog of sym_maps: exports via the entry table (ordinal->seg:offset) +
    resident/nonresident name tables (name->ordinal). Returns offset-only exports (the C's
    sub_<off> names are offset-keyed). Imports + inter-segment far-call resolution are DEFERRED
    -- NE far-call targets live in the per-segment relocation table keyed by call-site offset,
    which the readable C doesn't expose per-call; those stay undefined (like external imports).
    So NE modules link INTERNALLY (near intra-segment calls) and export their symbols."""
    N = ne_mod.NEFile(target.resolve(mod))
    ents = {e['ordinal']: e.get('offset') for e in N.entries() if 'seg' in e}
    exps = []
    for name, ordi in N.resident_names()[1:] + N.nonresident_names():
        off = ents.get(ordi)
        if off is not None:
            exps.append((name, ordi, off))
    # the module's DECLARED name (USER/GDI/KERNEL) -- NE imports reference modules by this name,
    # NOT the filename (krnl386.exe's module_name is KERNEL). Use it as the symbol prefix so a
    # cross-NE import `KERNEL_ALLOCCSTODSALIAS` resolves to krnl386's export alias.
    mname = re.sub(r'\W', '_', N.module_name()).upper()
    return exps, {}, [], {}, mname   # no import rewrite (il.py already named far calls)


def is_le(path):
    """True for an LE/LX image (VxDs .vxd/.386, .pdr miniports, some LE apps)."""
    try:
        d = open(path, "rb").read()
        return d[:2] == b"MZ" and d[struct.unpack_from("<I", d, 0x3c)[0]:][:2] in (b"LE", b"LX")
    except Exception:
        return False


def is_w3(path):
    """True for a W3 container (vmm32.vxd) -- an MZ loader wrapping bundled LE module(s)."""
    try:
        d = open(path, "rb").read()
        return d[:2] == b"MZ" and d[struct.unpack_from("<I", d, 0x3c)[0]:][:2] == b"W3"
    except Exception:
        return False


def sym_maps_w3(mod):
    """W3 container (vmm32.vxd): link its bundled VMM LE. Objects/prefix come from the embedded
    LE (w3.embedded_les -> LEFile.from_buffer). EXPORTS are VMM's services -- whose table VMM
    hides in the LE preamble with a flat pointer, so ddb_services can't see it; le.vmm_services()
    decodes it to [(index, obj, offset)], aliased VMM_ord_<index> onto the def at each handler."""
    from src.formats import w3
    les = w3.embedded_les(mod)
    if not les:
        return [], {}, [], {}, prefix(mod)
    _name, _off, L = les[0]
    objs = L.objects()
    segs = [(i + 1, o['base'], o['vsize']) for i, o in enumerate(objs)]
    segvaddr = {i + 1: o['base'] for i, o in enumerate(objs)}
    mname = re.sub(r'\W', '_', L.module_name()).upper() or 'VMM'
    exps = [(None, index, off) for index, _ob, off in L.vmm_services()]
    return exps, {}, segs, segvaddr, mname


def sym_maps_le(mod):
    """LE (VxD) analog: link INTERNALLY. objects() give the object bases (the LE analogue of NE
    segments / PE sections) for inter-object far-call resolution; module_name is the symbol
    prefix. EXPORTS: a VxD exports SERVICES via its DDB service table (not the LE entry table).
    le.ddb_services() resolves that table -> [(index, obj, offset)]; each entry is a service
    entry point the carve now splits into its own function (le_seg adds the DDB offsets as
    starts), so we alias it to the def at that offset. Most devices have no friendly service
    names (not in the base DDK tables), so these are ORDINAL aliases <MODULE>_ord_<index>; a
    named-service map can refine them later. Offsets are object-relative, matching the sub_<off>
    def names; the flat offset->def map (transform) is unambiguous here (services don't collide
    across objects)."""
    L = le_mod.LEFile(target.resolve(mod))
    objs = L.objects()
    segs = [(i + 1, o['base'], o['vsize']) for i, o in enumerate(objs)]
    segvaddr = {i + 1: o['base'] for i, o in enumerate(objs)}
    mname = re.sub(r'\W', '_', L.module_name()).upper() or prefix(mod)
    exps = []
    try:                                              # DDB service table -> export aliases
        for index, _obj, off in L.ddb_services():
            exps.append((None, index, off))           # (name, ordinal, offset); name deferred
    except Exception:
        pass
    return exps, {}, segs, segvaddr, mname


def resolve_calls(src, pfx, segs, segvaddr, defmap):
    """Resolve every <PFX>_sub_<val> reference to its true (section, offset). Within a
    function at seg S, target RVA = segvaddr[S] + val (mod 2^32); find the code section that
    contains it and rewrite to the REAL def symbol at that section-offset (via defmap, which
    carries the exact name incl. its zero-padding). Intra-section calls map to themselves;
    inter-section (wrapped) calls recover their real target. No def there -> left as-is."""
    parts = re.split(r'(/\* \[AUTO\][^\n]*\bseg(\d+):0x[0-9a-f]+[^\n]*?\*/)', src)
    out = [parts[0]]
    i = 1
    while i + 2 <= len(parts) - 1:
        comment, seg, body = parts[i], parts[i + 1], parts[i + 2]
        vbase = segvaddr.get(int(seg))
        if vbase is not None:
            def rs(m, vbase=vbase):
                rva = (vbase + int(m.group(1), 16)) & 0xffffffff
                for si, va, vs in segs:
                    if va <= rva < va + vs:
                        return defmap.get(rva - va, m.group(0))
                return m.group(0)
            body = re.sub(r'%s_sub_([0-9a-f]+)\b' % pfx, rs, body)
        out += [comment, body]
        i += 3
    return "".join(out)


def transform(mod, dll_of_prefix, stub=False):
    """Namespace + export-alias + import-rewrite the module's C. dll_of_prefix maps an
    imported DLL name (upper, no ext) -> its prefix, for cross-module import symbols."""
    try:                                              # rung 2c: real arities (ret N) -> drop casts;
        import sigrecover                              # rung-3 SDK fill: ret N > SDK arity (hypothesis)
        _ar, _rep = sigrecover.arities_plus(mod)
        if _rep.get("disagree") or _rep.get("filled"):
            print("  [sigrecover] SDK cross-check: %d agree, %d disagree, %d cdecl-export fills"
                  % (_rep["agree"], _rep["disagree"], _rep["filled"]))
    except Exception:
        _ar = None
    src = rc.adapt(open(cfile(mod), encoding="utf-8", errors="replace").read(), link=True, arities=_ar)
    # dispatch on the real binary format: PE gets full sym_maps (sections/imports) + file prefix;
    # NE gets the entry-table export map + its DECLARED module_name as prefix (so cross-NE imports,
    # which the lift named MODULE_EXPORT via il._call_target, resolve to that module's exports).
    binpath = target.resolve(mod)
    if is_pe(binpath):
        exps, imps, segs, segvaddr = sym_maps(mod)
        pfx = prefix(mod)
    elif is_w3(binpath):
        exps, imps, segs, segvaddr, pfx = sym_maps_w3(mod)
    elif is_le(binpath):
        exps, imps, segs, segvaddr, pfx = sym_maps_le(mod)
    else:
        exps, imps, segs, segvaddr, pfx = sym_maps_ne(mod)
    # 1. namespace all sub_ symbols
    src = re.sub(r'\bsub_', pfx + '_sub_', src)
    # 1b. resolve inter-section call targets (wrapped 0xffff.... -> real def offset). Build
    #     the def map (offset -> exact def symbol) once and reuse it for the export aliases.
    defmap = {int(o, 16): "%s_sub_%s" % (pfx, o)
              for o in re.findall(r'\nu32 %s_sub_([0-9a-f]+)\(' % pfx, src)}
    src = resolve_calls(src, pfx, segs, segvaddr, defmap)
    # 3. import rewrite: *(u32*)(0x<slot>) -> ((u32)&TARGET) ; collect externs
    externs = set()
    for slot, (dll, name) in imps.items():
        dp = re.sub(r'\W', '_', dll.rsplit('.', 1)[0]).upper()
        tgt = "%s_ord_%s" % (dp, name[1:]) if name.startswith('#') else "%s_%s" % (dp, cid(name))
        needle = "*(u32*)(0x%x)" % slot
        if needle in src:
            src = src.replace(needle, "((u32)&%s)" % tgt)
            externs.add(tgt)
    ext_decl = "".join("extern u32 %s();\n" % e for e in sorted(externs))
    # 2. export aliases (name + ordinal) -> the def at that offset. Match the ACTUAL def
    #    symbols (zero-padded %05x) and only alias exports whose target is really defined
    #    (some exports point at merged/forwarded offsets with no standalone def).
    defmap = {int(o, 16): "%s_sub_%s" % (pfx, o)
              for o in re.findall(r'\nu32 %s_sub_([0-9a-f]+)\(' % pfx, src)}
    seen = set()
    alias = []
    for name, ordi, off in exps:
        syms = [("%s_%s" % (pfx, cid(name))) if name else None, "%s_ord_%d" % (pfx, ordi)]
        syms = [s for s in syms if s]
        if off in defmap:
            target_sym = defmap[off]
            for asym in syms:
                if asym in seen or asym == target_sym:
                    continue
                seen.add(asym)
                alias.append('u32 %s() __attribute__((alias("%s")));' % (asym, target_sym))
        elif stub:
            # export points at an offset with NO carved def -- an INT 3F MOVEABLE thunk or a
            # region the carve treated as data. Define the export symbol as a marked placeholder
            # so cross-module importers resolve; the real fix is carving these thunks/regions.
            for asym in syms:
                if asym in seen:
                    continue
                seen.add(asym)
                alias.append('u32 %s(){return 0;} /*export: uncarved thunk*/' % asym)
    # forward-declare EVERY referenced sub_ (K&R extern). Defined-but-later symbols need the
    # forward decl for the cast at an earlier call site; genuinely-undefined ones become a
    # clean link-time U symbol instead of a compile-time undeclared identifier.
    refs = set(re.findall(r'\b%s_sub_[0-9a-f]+\b' % pfx, src))
    missing = "".join("extern u32 %s();\n" % s for s in sorted(refs))
    # stub mode: define a marked placeholder for every intra-module sub_ still undefined --
    # calls INTO the middle of a function (secondary entry points the carve didn't split out).
    # These are <PFX>_sub_ (this module's own prefix) not resolved by any def -> safe to stub;
    # cross-module names (<OTHERDLL>_...) are NOT touched (they link against real exports).
    stubs = ""
    if stub:
        undef = sorted(refs - set(defmap.values()))
        stubs = "".join("u32 %s(){return 0;} /*2ndary-entry: split enclosing fn*/\n" % s
                        for s in undef)
    # place externs + aliases right after the prelude include (line 1)
    nl = src.index('\n') + 1
    return src[:nl] + ext_decl + missing + stubs + "\n".join(alias) + "\n" + src[nl:]


def is_pe(path):
    """True only for a real 32-bit PE image. Win95 ships MANY .dll/.exe as 16-bit NE (and
    some LE); those need a separate 16-bit-segmented linker, not this PE one."""
    try:
        d = open(path, "rb").read()
        if d[:2] != b"MZ" or len(d) < 0x40:
            return False
        e = struct.unpack_from("<I", d, 0x3c)[0]
        return d[e:e + 4] == b"PE\0\0"
    except Exception:
        return False


def collect_pe():
    """Every real PE OR NE module (by signature) whose binary target.resolve finds -- the whole
    linkable OS surface, both formats. (LE/VxD are excluded; they're a further axis.)"""
    out = []
    for f in glob.glob(os.path.join(GEN, "**", "*.c"), recursive=True):
        b = os.path.basename(f)[:-2]
        # no extension filter: the 16-bit core ships as extensionless generated C (user.c ->
        # user.exe, gdi.c, krnl386.c). Include any module whose binary is a real PE or NE.
        try:
            p = target.resolve(b)
            if p and (is_pe(p) or is_ne(p) or is_le(p) or is_w3(p)):
                out.append(b)
        except Exception:
            pass
    return sorted(set(out))


def build_one(m, tmp, stub=False):
    try:
        c = os.path.join(tmp, prefix(m) + ".c")
        open(c, "w", encoding="utf-8").write(transform(m, {}, stub))
    except Exception:
        return (m, None, 0, 0)
    obj = c[:-2] + ".o"
    r = subprocess.run(["gcc", "-std=gnu89", "-c", "-w", "-I", INC, "-o", obj, c],
                       capture_output=True, text=True)
    if r.returncode != 0 or not os.path.exists(obj):
        return (m, None, 0, 0)
    n = subprocess.run(["nm", obj], capture_output=True, text=True).stdout
    return (m, obj, n.count(" T ") + n.count(" t "), n.count(" U "))


def main():
    from concurrent.futures import ThreadPoolExecutor
    # target.resolve() finds binaries under files/ relative to cwd -> anchor at the win95
    # data root (GEN = <win95>/src/generated) so this works from anywhere.
    os.chdir(os.path.dirname(os.path.dirname(GEN)))
    argv = sys.argv[1:]
    stub = "--stub" in argv
    if "--all" in argv:
        mods = collect_pe()
    else:
        mods = [a for a in argv if not a.startswith("-")] or \
               ["kernel32.dll", "user32.dll", "gdi32.dll"]
    jobs = max(2, (os.cpu_count() or 4) - 2)
    import atexit
    import shutil
    tmp = tempfile.mkdtemp()
    atexit.register(shutil.rmtree, tmp, ignore_errors=True)   # don't leak objs under %TEMP%
    print("RUNG-2b link: %d modules (-j%d)" % (len(mods), jobs), flush=True)
    objs, built, failed = [], 0, 0
    with ThreadPoolExecutor(max_workers=jobs) as ex:
        for m, obj, t, u in ex.map(lambda x: build_one(x, tmp, stub), mods):
            if obj:
                objs.append(obj); built += 1
            else:
                failed += 1
            if (built + failed) % 100 == 0:
                print("  ...%d/%d built=%d" % (built + failed, len(mods), built), flush=True)
    print("  compiled to object: %d/%d  (%d failed to build)" % (built, len(mods), failed))
    if len(objs) < 2:
        print("need >=2 objects to link"); return
    combined = os.path.join(tmp, "combined.o")
    # --allow-multiple-definition: a few degenerate modules emit an internal duplicate symbol
    # (offset-collision funcs at seg1:0x0); don't let one bad module kill the whole link.
    # Pass objects via a @response-file -- 880 paths blow past the Windows cmdline length limit.
    rsp = os.path.join(tmp, "objs.rsp")
    open(rsp, "w").write("\n".join('"%s"' % o for o in objs))
    lr = subprocess.run(["ld", "-r", "--allow-multiple-definition", "-o", combined, "@" + rsp],
                        capture_output=True, text=True)
    if not os.path.exists(combined):
        print("ld -r failed:", lr.stderr[:300]); return
    n = subprocess.run(["nm", combined], capture_output=True, text=True).stdout
    undef = re.findall(r' U (\S+)', n)
    defined = len(re.findall(r' [Tt] ', n))
    our = [u for u in undef if re.match(r'[A-Z0-9_]+_(sub_|ord_|[A-Za-z])', u)]
    submid = [u for u in our if "_sub_" in u]
    named = [u for u in our if "_sub_" not in u]
    print("=" * 56)
    print("COMBINED LINK (ld -r, %d objects)" % len(objs))
    print("  defined symbols:       %d" % defined)
    print("  still-undefined:       %d" % len(undef))
    print("  our-module unresolved: %d  (named cross-module=%d, mid-fn _sub_=%d)"
          % (len(our), len(named), len(submid)))
    print("  external (advapi32/ntdll/… outside set): %d" % (len(undef) - len(our)))
    print("  => cross-module import resolution among the set is the (undef - external - mid-fn)")


if __name__ == "__main__":
    main()
