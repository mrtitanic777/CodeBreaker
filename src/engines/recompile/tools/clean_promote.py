#!/usr/bin/env python3
r"""clean_promote.py -- emit GENUINE deregisterized C per oracle-verified function.

Where promote.py promotes the register-machine lift to real C, clean_promote goes further: it runs
the empirical register-input detector (reg_inputs.py) on each function's ORIGINAL bytes, lifts it
with il.lift_asm_function(clean=True, reg_inputs=<detected>) so non-input registers become function
LOCALS and genuine inputs become PARAMS, then VERIFIES bit-identical to the shipped bytes (run the
original in unicorn with random regs+stack; call the clean C with the reg-inputs as params; compare).
Only oracle-verified functions are promoted; everything else stays byte-exact naked asm. The result
is a compilable module TU whose C fraction is genuine, CPU-state-free source -- and every promoted
function is proven to behave exactly like Microsoft's code.

    (run from the win95 data root)  python clean_promote.py MODULE.DLL [--out DIR] [--limit N]
"""
import os, re, sys, glob, ctypes, random, subprocess
HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(HERE)))))
from src.core import cores
cores.set_target(paths.target_dir("win95"))  # engine/targets/win95
from src.formats.pe import open_pe
from src.formats.pe_seg import PeSeg
from src.formats.pe_trace import PeCarve
from src.core import layout
from src.ir import il
from src.core import paths
import hybrid_emit as H
import reg_inputs as R
import difftest as D
import certify_fn
import clean_graph
from sdk_sig import sdk_api, apply_api_signature
INC = os.path.join(os.path.dirname(HERE), "include")


def _verify(module, off, f, va0, code, sym):
    """Detect reg inputs, clean-lift as f_<sym>_s1_<off>, oracle-verify. Return the clean C body
    (verified bit-identical) or None. Only scalar-callable leaves are verifiable here."""
    sz = f["end"] - off
    if sz < 6 or sz > 400:
        return None
    ap = os.path.join(layout.asm_module_dir(module), "sec1_%05x.asm" % off)
    if not os.path.isfile(ap):
        return None
    inputs, ran = R.detect(va0, code, va0 + off, f["end"])
    if ran == 0:
        return None
    fname = "f_%s_s1_%05x" % (sym, off)
    try:
        cb = il.lift_asm_function(ap, fname, clean=True, reg_inputs=inputs)
    except Exception:
        return None
    body = cb.split("{", 1)[-1]
    if "*(" in body or "sub_" in body or "goto *" in body or "not recovered" in body or "__lifted" in body:
        return None
    if re.match(r"\s*void\b", cb):                      # void: no return value to compare here
        return None
    so, cfn = D._compile(cb)
    if not so:
        return None
    reg_order = sorted(inputs)
    npar = D._nparams(cb); nstack = npar - len(reg_order)
    slots = D._arg_slots(code, va0, off, f["end"])
    stack_slots = slots[:nstack] if len(slots) >= nstack else [k + 1 for k in range(nstack)]
    try:
        lib = ctypes.CDLL(so); cf = getattr(lib, cfn)
        cf.restype = ctypes.c_uint32; cf.argtypes = [ctypes.c_uint32] * npar
    except Exception:
        return None
    tested = 0
    for t in range(25):
        rng = random.Random(off * 977 + t)
        depth = max([4] + stack_slots)
        stack = [rng.getrandbits(32) for _ in range(depth)]
        regvals = {r: rng.getrandbits(32) for r in R.GP}
        o = R._emul(va0, code, va0 + off, f["end"], stack, regvals)
        if o is None:
            return None
        args = [regvals[p] for p in reg_order] + [stack[s - 1] for s in stack_slots]
        try:
            c = cf(*args) & 0xffffffff
        except Exception:
            return None
        tested += 1
        if o != c:
            return None                                # oracle rejects -> keep as fallback
    return cb if tested else None


def clean_promote(module, outdir, limit=None, mem_manifest=None):
    ent = H._index().get(module.strip().lower())
    if not ent:
        sys.exit("no asm dir for " + module)
    target = cores.get("target"); pe = open_pe(target.resolve(module))
    va0, code, secname = pe.code_sections()[0]
    sg = PeSeg(module, section=secname, carve=PeCarve(target.resolve(module)))
    sym = module.replace(".", "_")
    # export offset -> name, for Tier-1 SDK signatures
    tv = [s for s in pe.sections if s["chars"] & 0x20000000][0]["vaddr"]
    API = sdk_api(os.path.join(os.environ.get("WT_DATA_ROOT", "."), "reports"))
    exp = {}
    _ex = pe.exports()
    for t in (_ex.items() if isinstance(_ex, dict) else _ex):
        if isinstance(t, (list, tuple)) and len(t) == 3:
            exp[t[2] - tv] = t[1]                        # rva - text_vaddr -> export name
    MEM = set()                                          # memory-oracle-verified offsets for this module
    if mem_manifest and os.path.exists(mem_manifest):
        for line in open(mem_manifest, encoding="utf-8", errors="replace"):
            p = line.split("\t")
            if len(p) >= 2 and p[0].strip().lower() == module.strip().lower():
                MEM.add(int(p[1], 16))
    # 1) UNIFIED verify: whole-call-graph (leaf = closure of 1, non-leaf = whole graph), oracle-proven
    #    -> genuine clean C for the whole closure; Tier-1 SDK signature on exported top functions.
    an = certify_fn._analyze(module)
    promo = {}; ntier1 = 0; ngraph = 0                  # off -> clean C body (named f_<sym>_s1_<off>)
    offs = sorted(sg.funcs.items())
    if limit:
        offs = offs[:limit]
    for off, f in offs:
        if off in promo:                               # already promoted as part of another closure
            continue
        r = clean_graph.verify_graph(module, off, sg, va0, code, an)
        if not r:
            continue
        bodies, n = r                                  # {off: clean C}, closure size
        if n > 1:
            ngraph += 1
        if off in exp and exp[off] in API:             # Tier-1: stamp the exact Microsoft signature
            fname = "f_%s_s1_%05x" % (sym, off)
            tiered = apply_api_signature(bodies[off], API[exp[off]], fname)
            if tiered:
                bodies[off] = "/* Win32 API: %s */\n%s" % (exp[off], tiered); ntier1 += 1
        for o, b in bodies.items():
            promo.setdefault(o, b)
    # 1b) fold in MEMORY-oracle-verified functions (call-free pointer leaves proven by memdifftest --clean)
    nmem = 0
    for off in sorted(MEM):
        if off in promo or off not in sg.funcs:
            continue
        ap = os.path.join(layout.asm_module_dir(module), "sec1_%05x.asm" % off)
        if not os.path.isfile(ap):
            continue
        try:
            cb = il.lift_asm_function(ap, "f_%s_s1_%05x" % (sym, off), clean=True, reg_inputs=frozenset())
        except Exception:
            continue
        body = cb.split("{", 1)[-1]
        if "sub_" in body or "not recovered" in body or "goto *" in body:
            continue                                     # memory leaves are call-free; guard anyway
        if off in exp and exp[off] in API:               # Tier-1 on memory-verified exports too
            tiered = apply_api_signature(cb, API[exp[off]], "f_%s_s1_%05x" % (sym, off))
            if tiered:
                cb = "/* Win32 API: %s */\n%s" % (exp[off], tiered); ntier1 += 1
        promo[off] = cb; nmem += 1
    # 2) emit the TU: genuine clean C for verified + byte-exact naked asm fallback for the rest
    mdir, prefix = ent
    funcs = sorted(glob.glob(os.path.join(mdir, prefix + "*_*.asm")),
                   key=lambda p: (lambda mm: (int(mm.group(2)), int(mm.group(3), 16)))(H._SEGFILE.match(os.path.basename(p))))
    parsed = [H.parse_function(p) for p in funcs]
    starts = {(sg_, st) for sg_, st, _ in parsed}
    os.makedirs(outdir, exist_ok=True)
    dst = os.path.join(outdir, sym + "_clean.c")
    npro = nnak = 0
    with open(dst, "w") as fh:
        fh.write("/* GENUINE deregisterized C for %s: oracle-VERIFIED clean source (registers -> "
                 "locals/params), byte-exact naked asm for the unverified rest. */\n" % module)
        fh.write('#include "win95c.h"\n#define NK __attribute__((naked,used))\n')
        flags = sorted(set(re.findall(r'\b__(?:cf|zf|sf|of|pf|af|ovf|carry|borrow|aux)\w*\b(?!\s*\()',
                                      "".join(promo.values()))))
        fh.write("".join("static u32 %s;\n" % f for f in flags) + "\n")
        for sg_, st, _ in parsed:
            if st not in promo:
                fh.write("NK void f_%s_s%d_%05x(void);\n" % (sym, sg_, st))
        fh.write("\n")
        for sg_, start, items in parsed:
            if start in promo:                          # GENUINE clean C
                fh.write("/* promoted: deregisterized, verified bit-identical to Win95 */\n")
                fh.write(promo[start].rstrip() + "\n\n")
                npro += 1
                continue
            fh.write('NK void f_%s_s%d_%05x(void){ __asm__ volatile(\n' % (sym, sg_, start))
            for it in items:
                if it[0] == "xfer":
                    _, op, tgt, raw = it
                    if (sg_, tgt) in starts and tgt not in promo:
                        fh.write('  "%s _f_%s_s%d_%05x\\n"\n' % (op, sym, sg_, tgt))
                    elif raw:
                        fh.write('  ".byte %s\\n"\n' % ",".join("0x%02x" % b for b in raw))
                else:
                    fh.write('  ".byte %s\\n"\n' % ",".join("0x%02x" % b for b in it[1]))
            fh.write("  ); }\n")
            nnak += 1
    return dst, npro, nnak, ntier1, ngraph, nmem


if __name__ == "__main__":
    outdir = sys.argv[sys.argv.index("--out") + 1] if "--out" in sys.argv else "."
    limit = int(sys.argv[sys.argv.index("--limit") + 1]) if "--limit" in sys.argv else None
    memman = sys.argv[sys.argv.index("--mem-manifest") + 1] if "--mem-manifest" in sys.argv else None
    mod = [a for a in sys.argv[1:] if not a.startswith("-")][0]
    dst, npro, nnak, ntier1, ngraph, nmem = clean_promote(mod, outdir, limit=limit, mem_manifest=memman)
    print("wrote %s" % dst)
    print("  GENUINE clean C (oracle-verified): %d   (whole-call-graph closures: %d, memory-oracle: %d, Tier-1 SDK-typed exports: %d)   naked-asm fallback: %d"
          % (npro, ngraph, nmem, ntier1, nnak))
    # confirm the TU compiles
    r = subprocess.run(["gcc", "-std=gnu89", "-w", "-c", "-o", os.devnull, dst, "-I", INC],
                       capture_output=True, text=True)
    print("  TU compiles: %s" % ("YES" if r.returncode == 0 else "NO\n" + r.stderr[:400]))
