#!/usr/bin/env python3
r"""clean_graph.py -- WHOLE-CALL-GRAPH clean promotion. Extends clean_promote past leaves: to
promote a NON-leaf function F, lift F AND its entire intra-module callee closure to clean C (each
named sub_<off> so the calls link), compile them together, then oracle-verify F end-to-end -- run
the ORIGINAL F in unicorn (the whole .text is mapped, so its calls execute the real callees) and
compare to the candidate clean F (which calls the clean callees). A match proves the ENTIRE closure's
deregisterization is behaviour-identical to Microsoft's code -- a strictly stronger check than per-leaf
(it catches any cross-call register threading / side-effect the per-fn detector could miss).

v1 scope: closures that are all-clean (certify_fn: no notrec/goto*/external/unlifted), all
ZERO-register-input (so cdecl stack-arg calls link with no register marshaling), and whose top F
runs cleanly in the emulator (pure-compute; pointer-deref args are a later, memdifftest-style step).

    (run from win95 data root)  python clean_graph.py MODULE.DLL [--limit N]
"""
import os, sys, re, glob, ctypes, random, subprocess, tempfile
HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(HERE)))))
from codebreaker.foundation import cores
cores.set_target(paths.target_dir("win95"))
from codebreaker.format_loaders.pe import open_pe
from codebreaker.format_loaders.pe_seg import PeSeg
from codebreaker.format_loaders.pe_trace import PeCarve
from codebreaker.foundation import layout
from codebreaker.ir_engine import il
from codebreaker.foundation import paths
import reg_inputs as R
import difftest as D
import certify_fn
INC = os.path.join(os.path.dirname(HERE), "include")


def _closure(off, funcs, calls):
    seen, stack = set(), [off]
    while stack:
        o = stack.pop()
        if o in seen or o not in funcs:
            return None if o not in funcs else seen   # unlifted callee -> not promotable
        seen.add(o)
        stack.extend(calls.get(o, ()))
    return seen


def _compile_multi(bodies):
    src = '#include "win95c.h"\n' + "\n\n".join(bodies)
    d = tempfile.mkdtemp()
    open(os.path.join(d, "g.c"), "w").write(src)
    so = os.path.join(d, "g.dll")
    r = subprocess.run(["gcc", "-std=gnu89", "-w", "-shared", "-o", so, os.path.join(d, "g.c"),
                        os.path.join(INC, "win95rt.c"), "-I", INC], capture_output=True, text=True)
    return so if r.returncode == 0 else None


def verify_graph(module, off, sg, va0, code, an):
    funcs, defects, calls = an
    if off not in funcs or defects.get(off):
        return None
    clo = _closure(off, funcs, calls)
    if not clo:                                        # size 1 = leaf, >1 = whole graph -- one path
        return None
    if any(defects.get(o) for o in clo):
        return None                                    # closure must be entirely clean
    sym = module.replace(".", "_")
    def _fn(o):
        return "f_%s_s1_%05x" % (sym, o)               # TU-consistent names so promoted closures link
    bodies = {}
    for o in sorted(clo):
        if o not in sg.funcs:
            return None
        fo = sg.funcs[o]
        if fo["end"] - o > 600:
            return None
        inp, ran = R.detect(va0, code, va0 + o, fo["end"])
        if inp:
            return None                                # zero-reg-input closure only (v1)
        ap = os.path.join(layout.asm_module_dir(module), "sec1_%05x.asm" % o)
        if not os.path.isfile(ap):
            return None
        try:
            cb = il.lift_asm_function(ap, _fn(o), clean=True, reg_inputs=frozenset())
        except Exception:
            return None
        body = cb.split("{", 1)[-1]
        if ("not recovered" in body or "goto *" in body or "__lifted" in body
                or re.search(r"\*\(u\d+\*\)\(0x[0-9a-f]{4,}\)", body)):   # non-relocatable abs global
            return None
        cb = re.sub(r"\bsub_([0-9a-f]+)\b",              # rename callee call-sites to TU names
                    lambda mm: "f_%s_s1_%05x" % (sym, int(mm.group(1), 16)), cb)
        bodies[o] = cb
    so = _compile_multi(list(bodies.values()))
    if not so:
        return None
    # oracle-verify the TOP function F end-to-end
    fend = sg.funcs[off]["end"]
    try:
        lib = ctypes.CDLL(so); cf = getattr(lib, _fn(off))
        cf.restype = ctypes.c_uint32
    except Exception:
        return None
    if re.match(r"\s*void\b", bodies[off]):
        return None
    npar = D._nparams(bodies[off])
    cf.argtypes = [ctypes.c_uint32] * npar
    slots = D._arg_slots(code, va0, off, fend)
    stack_slots = slots[:npar] if len(slots) >= npar else [k + 1 for k in range(npar)]
    tested = 0
    for t in range(25):
        rng = random.Random(off * 1009 + t)
        depth = max([4] + stack_slots)
        stack = [rng.getrandbits(32) for _ in range(depth)]
        regvals = {r: rng.getrandbits(32) for r in R.GP}
        o = R._emul(va0, code, va0 + off, fend, stack, regvals)
        if o is None:
            return None
        try:
            c = cf(*[stack[s - 1] for s in stack_slots]) & 0xffffffff
        except Exception:
            return None
        tested += 1
        if o != c:
            return None                                # oracle rejects the closure
    return (bodies, len(clo)) if tested else None


GP = set(R.GP)


def _leading_regs(body, off):
    """The function's leading REGISTER-input params, in signature order (il emits genuine register
    inputs -- thiscall ecx / fastcall ecx,edx -- as the LEADING params, named after the register).
    Returns e.g. ['ecx'] for `int sub_X(u32 ecx, u32 arg3, ...)`; [] for a pure stdcall fn."""
    m = re.search(r"\bsub_%x\s*\(" % off, body)
    if not m:
        return []
    i, depth, seg = m.end(), 1, []
    while i < len(body) and depth:
        ch = body[i]
        if ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
            if depth == 0:
                break
        seg.append(ch); i += 1
    params = "".join(seg).strip()
    if not params:
        return []
    regs = []
    for p in params.split(","):
        nm = p.replace("*", " ").split()[-1] if p.strip() else ""
        if nm in GP:
            regs.append(nm)          # leading register params only -- stop at the first stack arg
        else:
            break
    return regs


def _thread(body, coff, regs, is_own):
    """Thread a callee's register args into every call-site of sub_<coff> in `body`:
    `sub_X(a,b)` -> `sub_X(ecx, a,b)`. The reg names coincide with the CALLER's own register locals,
    so this passes exactly the value the caller holds at the call -- the cross-call register threading
    the per-fn lift can't see. Skips the callee's OWN definition line (first match when is_own)."""
    if not regs:
        return body
    csv = ", ".join(regs)
    pat = re.compile(r"\bsub_%x\s*\((\s*\))?" % coff)
    cnt = [0]

    def repl(m):
        cnt[0] += 1
        if is_own and cnt[0] == 1:
            return m.group(0)                      # leave the definition untouched
        if m.group(1) is not None:                 # empty-arg call: sub_X()
            return "sub_%x(%s)" % (coff, csv)
        return "sub_%x(%s, " % (coff, csv)         # call had stack args
    return pat.sub(repl, body)


def verify_graph_reg(module, off, sg, va0, code, an):
    """Like verify_graph, but HANDLES register-input functions (the boot-ceiling case): each fn's
    reg inputs become leading params, threaded across call-sites, and the top is oracle-verified with
    its registers SET (emul regvals == candidate leading args). Proves the register-param recovery +
    cross-call threading is bit-identical to Microsoft's code. Returns (bodies, closure_size)."""
    funcs, defects, calls = an
    if off not in funcs or defects.get(off):
        return None
    clo = _closure(off, funcs, calls)
    if not clo or any(defects.get(o) for o in clo):
        return None
    sym = module.replace(".", "_")
    raw, regp = {}, {}
    for o in sorted(clo):
        if o not in sg.funcs:
            return None
        fo = sg.funcs[o]
        if fo["end"] - o > 600:
            return None
        ap = os.path.join(layout.asm_module_dir(module), "sec1_%05x.asm" % o)
        if not os.path.isfile(ap):
            return None
        try:
            # SOUNDNESS: lift with reg_inputs=None (il DATAFLOW liveness) -- the EXACT lift package_ff
            # ships (it also passes reg_inputs=None). Verifying an R.detect-threaded emission instead
            # would prove a DIFFERENT function bit-identical than the one that runs. _leading_regs below
            # reads back whatever il actually recovered, so threading + the oracle test match the ship.
            cb = il.lift_asm_function(ap, "sub_%x" % o, clean=True, reg_inputs=None)
        except Exception:
            return None
        b = cb.split("{", 1)[-1]
        if ("not recovered" in b or "goto *" in b or "__lifted" in b
                or re.search(r"\*\(u\d+\*\)\(0x[0-9a-f]{4,}\)", b)):   # non-relocatable abs global
            return None
        raw[o] = cb
        regp[o] = _leading_regs(cb, o)
    # thread every call-site with its callee's recovered register args, then TU-consistent rename
    bodies = {}
    for o in sorted(clo):
        b = raw[o]
        for c in sorted(clo):
            b = _thread(b, c, regp[c], is_own=(o == c))
        b = re.sub(r"\bsub_([0-9a-f]+)\b",
                   lambda mm: "f_%s_s1_%05x" % (sym, int(mm.group(1), 16)), b)
        bodies[o] = b
    so = _compile_multi(list(bodies.values()))
    if not so:
        return None
    fend = sg.funcs[off]["end"]
    _fn = lambda o: "f_%s_s1_%05x" % (sym, o)
    if re.match(r"\s*void\b", bodies[off]):
        return None
    try:
        lib = ctypes.CDLL(so); cf = getattr(lib, _fn(off))
        cf.restype = ctypes.c_uint32
    except Exception:
        return None
    regs_top = regp[off]
    nreg = len(regs_top)
    total = D._nparams(bodies[off])
    nstack = max(0, total - nreg)
    cf.argtypes = [ctypes.c_uint32] * total
    slots = D._arg_slots(code, va0, off, fend)
    stack_slots = slots[:nstack] if len(slots) >= nstack else [k + 1 for k in range(nstack)]
    tested = 0
    for t in range(25):
        rng = random.Random(off * 1009 + t)
        depth = max([4] + stack_slots)
        stack = [rng.getrandbits(32) for _ in range(depth)]
        regvals = {r: rng.getrandbits(32) for r in R.GP}
        o = R._emul(va0, code, va0 + off, fend, stack, regvals)
        if o is None:
            return None
        try:
            args = [regvals[r] for r in regs_top] + [stack[s - 1] for s in stack_slots]
            c = cf(*args) & 0xffffffff
        except Exception:
            return None
        tested += 1
        if o != c:
            return None
    return (bodies, len(clo)) if tested else None


def main():
    module = [a for a in sys.argv[1:] if not a.startswith("-")][0]
    limit = int(sys.argv[sys.argv.index("--limit") + 1]) if "--limit" in sys.argv else None
    reg = "--reg" in sys.argv                         # reg-aware (thiscall/fastcall) whole-graph
    manifest = sys.argv[sys.argv.index("--manifest") + 1] if "--manifest" in sys.argv else None
    mf = open(manifest, "a", encoding="utf-8") if manifest else None   # oracle-gate: MOD<tab>off
    vg = verify_graph_reg if reg else verify_graph
    target = cores.get("target"); pe = open_pe(target.resolve(module))
    va0, code, secname = pe.code_sections()[0]
    sg = PeSeg(module, section=secname, carve=PeCarve(target.resolve(module)))
    an = certify_fn._analyze(module)
    if not an:
        sys.exit("no analysis for " + module)
    offs = sorted(sg.funcs.items())
    if limit:
        offs = offs[:limit]
    proven = 0; fns_promoted = set(); sizes = []; nreg_top = 0
    for off, f in offs:
        r = vg(module, off, sg, va0, code, an)
        if r:
            bodies, n = r
            proven += 1; fns_promoted |= set(bodies); sizes.append(n)
            if mf:                                     # the WHOLE closure was proven bit-identical
                for _co in set(bodies):                # together -> every member is safe to ship clean
                    mf.write("%s\t%x\n" % (module, _co))
                mf.flush()
            rp = reg and _leading_regs(bodies[off], off)   # il's ACTUAL recovered inputs (what shipped)
            tag = ("  regs=%s" % rp) if rp else ""
            if rp:
                nreg_top += 1
            print("  PROVEN non-leaf sub_%x  (closure=%d fns)%s" % (off, n, tag))
    print("\n%s: whole-call-graph promoted TOP fns=%d  total distinct fns in verified closures=%d%s"
          % (module, proven, len(fns_promoted), ("  (%d with register inputs)" % nreg_top) if reg else ""))
    if sizes:
        print("  closure sizes: min=%d max=%d avg=%.1f" % (min(sizes), max(sizes), sum(sizes) / len(sizes)))


if __name__ == "__main__":
    main()
