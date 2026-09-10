#!/usr/bin/env python3
r"""package_ff.py -- FULL-FIDELITY drop-in: clean editable C where the lift is faithful, the
byte-exact ORIGINAL bytes where it isn't. Makes ANY module swap-safe (not just the all-clean ones).

Key trick: DllMain reconstitutes the module's ORIGINAL image at its ORIGINAL ImageBase, executable,
and sets _imgbase = ImageBase. Then:
  * clean C functions run normally, reading data via (_imgbase + rva) == the original absolute VA;
  * DEFECTIVE functions (empty / (*(fnptr)0) / not-recovered / external / un-lifted callee) are
    emitted as a naked stub that JUMPS to the original bytes at (_imgbase + off) -- so they execute
    Microsoft's exact code, which is self-consistent because _imgbase == the original ImageBase.
The DLL itself is linked at a DIFFERENT ImageBase so Win95 loads the C code elsewhere, leaving the
original ImageBase free for the reconstituted image.

    python package_ff.py <module.dll> --out DIR
"""
import os, re, sys, glob, struct
_HERE = os.path.abspath(__file__)
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(_HERE)))))
sys.path.insert(0, os.path.dirname(_HERE))
from codebreaker.format_loaders.pe import open_pe
import certify_fn, rc, recover_flags, hybrid_emit as H
from codebreaker.ir_engine import il                          # deregisterized (clean) emission
from codebreaker.foundation import layout
from codebreaker.foundation import paths
W = os.environ.get("WT_DATA_ROOT", paths.binaries("win95"))
CLEAN = "--clean" in sys.argv       # BOOT-CEILING: emit DEREGISTERIZED good fns (registers->locals)
def _argval(flag):
    return sys.argv[sys.argv.index(flag) + 1] if flag in sys.argv and sys.argv.index(flag) + 1 < len(sys.argv) else None
VERIFIED_PATH = _argval("--verified")   # oracle-gate: ONLY deregisterize offsets in this manifest
try:
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    _MD = Cs(CS_ARCH_X86, CS_MODE_32)
except Exception:
    _MD = None


def _retN(img, rva, base, depth=0):
    """Recover an export's stdcall cleanup bytes = the operand of its terminating `ret N`
    (following a single tail `jmp` into the image). None if far/indirect/undeterminable."""
    if _MD is None or depth > 2 or not (0 <= rva < len(img)):
        return None
    for ins in _MD.disasm(bytes(img[rva:rva + 512]), base + rva):
        m = ins.mnemonic
        if m in ("ret", "retn"):
            o = ins.op_str.strip()
            return int(o, 16) if o else 0
        if m in ("retf", "retfq", "iret", "iretd"):
            return None
        if m == "jmp":
            o = ins.op_str.strip()
            if o.startswith("0x"):
                t = int(o, 16) - base
                if 0 <= t < len(img) and t != rva:
                    return _retN(img, t, base, depth + 1)
            return None                      # indirect/import tail jmp -> unknown
    return None


MSVC = "--msvc" in sys.argv          # emit MSVC (__declspec(naked)+Intel __asm) instead of GCC AT&T
REDIRECT = "--redirect" in sys.argv  # patch reconstituted image so byte-exact callers reach clean C


def _redir_publish(body):
    """REDIRECT: before each `return` in a clean fn, publish its MODIFIED GP register-locals to the
    global register-file (*g_edx=edx, ...), so the redirect thunk can propagate NON-eax outputs (a
    secondary edx return, esi/edi side effects) to the byte-exact caller. eax is excluded -- the C
    return already carries it. Only registers actually ASSIGNED in the body are published (they are
    declared, so referencing them compiles)."""
    _GP = ("ecx", "edx", "ebx", "esi", "edi", "ebp")
    asg = [r for r in _GP if re.search(r"\b" + r + r"\b\s*=(?!=)", body)]
    if not asg:
        return body
    pub = "".join("*g_%s=%s;" % (r, r) for r in asg)
    return re.sub(r"\breturn\b", lambda m: pub + " return", body)

# SDK API signatures (argc/varargs) -> authoritative stdcall cleanup for named exports.
try:
    import json as _json
    SDK_API = _json.load(open(os.path.join(os.environ.get("WT_DATA_ROOT", "."), "reports", "sdk_api.json")))
except Exception:
    SDK_API = {}


_CLEAN_REGP = {}          # off -> ['ecx'] for thiscall exports (set in build_ff --clean); read by the thunks


def _knr_defs(src):
    """Convert the clean lift's prototype-style function DEFINITIONS to K&R (old-style) so neither gcc
    nor MSVC arity-checks the calls. The lift's arity is heuristic -- a caller may push MORE or FEWER
    stack slots than the callee reads (inherent to recovering C from machine code) -- and a K&R
    definition creates NO prototype, so mismatched calls compile (exactly the permissive semantics the
    register-machine corpus already relied on). Naked/__declspec wrappers start with a keyword, not a
    return type, so they're never matched.
        int f(u32 ecx, u32 arg4){        ->   int f(ecx, arg4)
                                              u32 ecx; u32 arg4;
                                              {
        void f(void){                    ->   void f() {
    """
    hdr = re.compile(
        r'^(?P<ret>(?:unsigned\s+)?(?:u32|int|void|short|long)\s*\**|struct\s+\w+\s*\**)[ \t]+'
        r'(?P<name>sub_[0-9a-f]+|[A-Za-z_]\w*)[ \t]*\((?P<params>[^;{}()]*)\)[ \t]*\n\{', re.M)

    def repl(m):
        ret, name, params = m.group('ret').rstrip(), m.group('name'), m.group('params').strip()
        if params == "" or params == "void":
            return "%s %s() {" % (ret, name)                 # 0 params -> K&R empty (no arity check)
        names, decls = [], []
        for p in params.split(","):
            pm = re.match(r'(.+?)([A-Za-z_]\w*)\s*$', p.strip())
            if not pm:
                return m.group(0)                            # unparseable param -> leave prototype
            names.append(pm.group(2)); decls.append("%s %s;" % (pm.group(1).strip(), pm.group(2)))
        return "%s %s(%s)\n%s\n{" % (ret, name, ", ".join(names), " ".join(decls))
    return hdr.sub(repl, src)


def _clean_fixups(src, msvc):
    """Emission fixups for the clean lift -- bugs that were never exercised while --clean silently
    produced byte-exact output (the cwd bug). (1) NAKED byte-exact wrappers are declared `(void)` but
    the boundary sync calls them WITH the original stack args -> drop `void` so there's no arity check
    (the wrapper ignores C args anyway). (2) `&<const>` is a `lea` of a folded constant address -- not
    an lvalue; it's just the constant value. (3) gcc computed goto needs a `void*` target (the MSVC
    path already rewrote `goto *` to inline asm, so skip there)."""
    src = re.sub(r'\bsub_([0-9a-f]+)\s*\(\s*void\s*\)', r'sub_\1()', src)         # naked-wrapper arity
    src = re.sub(r'([=,(]\s*)&\s*(0x[0-9a-fA-F]+|\d+)\b', r'\1\2', src)           # &<const> -> <const>
    # partial-word WRITE to a non-register variable: `((V>>S)&M) = X;` and `(V&M) = X;` aren't lvalues
    # (the lane macros only alias the register file). Rewrite as a mask-merge into V.
    src = re.sub(r'\(\((\w+)\s*>>\s*(0x[0-9a-fA-F]+|\d+)\)\s*&\s*(0x[0-9a-fA-F]+|\d+)\)\s*=(?!=)\s*([^;]+);',
                 r'\1 = (\1 & ~((\3) << (\2))) | (((u32)(\4) & (\3)) << (\2));', src)
    src = re.sub(r'\((\w+)\s*&\s*(0x[0-9a-fA-F]+|\d+)\)\s*=(?!=)\s*([^;]+);',
                 r'\1 = (\1 & ~(\2)) | ((u32)(\3) & (\2));', src)
    # struct-typed pointer params in ARITHMETIC/BITWISE -> cast to u32. Fixes BOTH compilation (C
    # rejects `ptr & int`, `u32 - ptr`) AND semantics (`ptr + n` SCALES by sizeof(struct), but the
    # lift means a BYTE offset). No-op for already-u32 operands. `argN->field` derefs left alone.
    _P = r'(arg\d+|loc_[0-9a-f]+)'
    # LEFT operand `argN <op> ..`: argN is unambiguously a value here (a `*name` declarator puts the *
    # BEFORE the name and ends in `;`), so ALL binary ops are safe. Exclude `argN->` / `argN(`.
    src = re.sub(r'\b' + _P + r'\b(?!\s*(?:->|\())(\s*)([-+&|^*/%]|>>|<<)', r'(u32)\1\2\3', src)
    # RIGHT operand `<binop> argN`: require a left operand ([\w)]) so the op is BINARY, and EXCLUDE
    # `*`/`&` (pointer-declarator / deref / address-of ambiguity -- those are handled as LEFT above).
    src = re.sub(r'([\w)])(\s*)([-+|^/%]|>>|<<)(\s*)\b' + _P + r'\b(?!\s*->)', r'\1\2\3\4(u32)\5', src)
    if not msvc:
        src = re.sub(r'goto\s*\*\s*(\([^;]+)\)\s*;', r'goto *(void*)\1);', src)   # gcc computed goto
    return src


def _leading_reg_params(cb, off):
    """The lifted function's leading REGISTER params, in signature order (il emits recovered register
    inputs as the leading params, named after the register). ['ecx','edx',...] or []."""
    m = re.search(r"\bsub_%x\s*\(" % off, cb)
    if not m:
        return []
    i, depth, seg = m.end(), 1, []
    while i < len(cb) and depth:
        ch = cb[i]
        if ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
            if depth == 0:
                break
        seg.append(ch); i += 1
    regs = []
    for p in "".join(seg).split(","):
        nm = p.replace("*", " ").split()[-1] if p.strip() else ""
        if nm in ("eax", "ecx", "edx", "ebx", "esi", "edi", "ebp"):
            regs.append(nm)
        else:
            break
    return regs


def _entry_regs(cb):
    """Registers a lifted clean body READS before writing = genuine register inputs (edx/esi/edi/ebx/
    ecx; never eax=return). On a frozenset (all-local) body this is precise: a register whose FIRST
    textual occurrence is a read (not an `r =` assignment) is a real entry dependence. Isolates the
    function body (drops leading struct typedefs + the signature) and the local declaration statement
    (`u32 a,b,c;`) so neither is misread as a use. {} => none; {'ecx'} => thiscall/fastcall this."""
    m = re.search(r"\bsub_[0-9a-f]+\s*\([^)]*\)\s*\{", cb)
    body = cb[m.end():] if m else cb
    body = re.sub(r'\b(?:u32|int|short|long|unsigned)\s+[A-Za-z_][\w, ]*;', ' ', body)  # drop decls
    regs = set()
    for r_ in ("edx", "esi", "edi", "ebx", "ecx"):
        mm = re.search(r"\b%s\b\s*(=)?" % r_, body)
        if mm and not (mm.group(1) == "=" and body[mm.end():mm.end() + 1] != "="):
            regs.add(r_)                          # first occurrence is a READ (not `r =`, not `r ==`)
    return regs


def _thread(body, coff, regs, is_own):
    """Thread a callee's recovered register args into every call-site of sub_<coff>: `sub_X(a)` ->
    `sub_X(ecx, a)` -- the caller's own register locals coincide by name, so it passes exactly the
    value it holds at the call. Skips the callee's OWN definition (first match when is_own)."""
    if not regs:
        return body
    csv = ", ".join(regs)
    pat = re.compile(r"\bsub_%x\s*\((\s*\))?" % coff)
    cnt = [0]

    def repl(m):
        cnt[0] += 1
        if is_own and cnt[0] == 1:
            return m.group(0)
        if m.group(1) is not None:
            return "sub_%x(%s)" % (coff, csv)
        return "sub_%x(%s, " % (coff, csv)
    return pat.sub(repl, body)


def _stdcall_thunk(wrap, target, N, has_this=False):
    """Naked stdcall wrapper: copy N bytes of stack args, call the cdecl C function `target`, then
    `ret N` (real stdcall cleanup). Win95 calls exports stdcall; the lifted C is cdecl. For a THISCALL
    export (has_this) the deregisterized target takes this=ecx as its LEADING param, so the thunk also
    captures the incoming ecx and pushes it last (the leftmost cdecl arg)."""
    if N < 0 or (N == 0 and not has_this):
        return ""
    NW = max(0, N // 4)
    retN = N if N > 0 else 0
    if MSVC:
        s = ('void %s(void);\n__declspec(naked) void %s(void){ __asm {\n'
             '  push ebp\n  mov ebp, esp\n' % (wrap, wrap))
        if has_this:
            s += '  mov eax, ecx\n'                       # save incoming this (the loop clobbers ecx)
        if NW:
            s += '  mov ecx, %d\n  L1: push dword ptr [ebp+ecx*4+4]\n  loop L1\n' % NW
        if has_this:
            s += '  push eax\n'                           # this = leading cdecl arg (pushed last)
        s += '  call %s\n  mov esp, ebp\n  pop ebp\n  ret %d\n  } }\n' % (target, retN)
        return s
    s = ('__attribute__((naked, used)) void %s(void){ __asm__ volatile(\n'
         '  "push %%ebp\\n\\t"\n  "mov %%esp,%%ebp\\n\\t"\n' % wrap)
    if has_this:
        s += '  "mov %ecx,%eax\\n\\t"\n'          # single % -- plain concat (not a %-format string)
    if NW:
        s += ('  "mov $%d,%%ecx\\n\\t"\n  "1: pushl 4(%%ebp,%%ecx,4)\\n\\t"\n  "loop 1b\\n\\t"\n' % NW)
    if has_this:
        s += '  "push %eax\\n\\t"\n'              # single % -- plain concat (not a %-format string)
    s += ('  "call _%s\\n\\t"\n  "mov %%ebp,%%esp\\n\\t"\n  "pop %%ebp\\n\\t"\n  "ret $%d\\n"\n  ); }\n'
          % (target, retN))
    return s


def build_ff(module, outdir):
    os.makedirs(outdir, exist_ok=True)
    pe = open_pe(os.path.join(W, "disc", "staged", "bin", module))
    base = pe.image_base
    tv = [s for s in pe.sections if s["chars"] & 0x20000000][0]["vaddr"]
    # base-relocation directory (data dir[5]) -> applied to the reconstituted image in DllMain so that
    # absolute pointers baked into .data (vtables, fn-pointer tables) point at the ACTUAL load base, not
    # the preferred base. Without this, a module whose preferred base can't be VirtualAlloc'd (e.g. the
    # shared arena >=0x80000000: COMCTL32 @ 0xbfed0000) reconstitutes at a fallback base and every
    # data-pointer deref lands in wrong memory -> crash. (win95's own loader relocates the pristine DLL.)
    _pehdr = struct.unpack_from("<I", pe.data, 0x3c)[0]
    _reloc_rva, _reloc_sz = struct.unpack_from("<II", pe.data, _pehdr + 24 + 96 + 5 * 8)
    funcs, defects, calls = certify_fn._analyze(module)
    # a function is "bad" if it carries a defect OR is only referenced but never defined
    referenced = set()
    for cs in calls.values():
        referenced |= cs
    bad = {o for o in funcs if defects.get(o)} | {o for o in referenced if o not in funcs}
    # every EXPORT must have a body: fold export targets that aren't defined funcs into `bad`
    # (they become jmp-to-original-image stubs -> run Microsoft's exact code, faithfully).
    _ex = pe.exports()
    for _t in (_ex.items() if isinstance(_ex, dict) else _ex):
        if isinstance(_t, (list, tuple)) and len(_t) == 3:
            _off = _t[2] - tv
            if 0 <= _off < 0x100000 and _off not in funcs:   # named AND ordinal-only (NONAME) exports
                bad.add(_off)

    c = glob.glob(os.path.join(W, "src", "generated", "**", module + ".c"), recursive=True)
    src = open(c[0], encoding="utf-8", errors="replace").read()
    src, _n = recover_flags.recover_flags_src(module, src)
    # relocatable data refs -> (_imgbase + rva) (which == absolute VA since _imgbase==ImageBase)
    simg = 0
    for s in pe.sections:
        simg = max(simg, s["vaddr"] + max(s["vsize"], s["rawsize"]))
    simg = (simg + 0xFFF) & ~0xFFF
    lo, hi = base, base + 0x10000000

    def repl(m):
        va = int(m.group(0), 16)
        return "(_imgbase + 0x%x)" % (va - base) if lo <= va < hi else m.group(0)
    src = rc.adapt(src, link=True)
    # rc.adapt prepends a `static u32 __cf0;` decl, but win95c.h now declares `extern u32 __cf0;`
    # (unconditional, for the clean-path adc/sbb CF snapshot) and win95rt.c defines it -> static-after-
    # non-static collision that breaks the byte-exact drop-in. Drop the redundant static decl.
    src = re.sub(r'(?m)^\s*static\s+u32\s+__cf0\s*;\s*$', '', src)
    src = re.sub(r'0x[0-9a-fA-F]{6,8}', repl, src)
    # DROP the definitions of bad functions from the C (we replace them with jump-stubs) and
    # gather the set of sub_<off> that must become original-byte stubs.
    badsyms = set("sub_%x" % o for o in bad)
    # remove `... sub_X(...) { ... }` definitions for bad syms  (rc.adapt named defs sub_<off>)
    def strip_bad(text):
        out = []
        for blk in re.split(r'(?=\n(?:u32|int|void|short|long)\s+sub_[0-9a-f]+\s*\()', text):
            m = re.match(r'\n(?:u32|int|void|short|long)\s+(sub_[0-9a-f]+)\s*\(', blk)
            if m and m.group(1) in badsyms:
                continue
            out.append(blk)
        return "".join(out)
    src = strip_bad(src)
    # capture each bad sym's declared signature (rc.adapt's extern), then drop the extern
    badsig = {}
    for m in re.finditer(r'\n\s*extern\s+(\w+)\s+(sub_[0-9a-f]+)\s*\(([^)]*)\)\s*;', src):
        if m.group(2) in badsyms:
            # naked byte-exact wrappers ignore their C params entirely (they read the stack via inline
            # asm), so give them NO prototype `()` -- else a caller/boundary-sync passing a different
            # arg count than the recovered signature is arity-checked ((void)/9-u32 -> "too many args").
            badsig[m.group(2)] = (m.group(1), "")
    src = re.sub(r'\n\s*extern\s+\w+\s+(sub_[0-9a-f]+)\s*\([^;]*\);',
                 lambda m: "" if m.group(1) in badsyms else m.group(0), src)

    sym = module.replace(".", "_")
    if CLEAN:                            # BOOT-CEILING: deregisterize the GOOD fns -- registers become
        # function-LOCALS (kills the register-global threading that caused the swap wall), and thiscall
        # this=ecx is recovered as a leading param + threaded across call-sites. Absolute data addrs
        # (0xBF..) stay valid: DllMain reconstitutes the image at its ORIGINAL base. Functions with
        # non-standard register inputs we can't cleanly recover stay BYTE-EXACT (folded into `bad`).
        _verified = None                                   # oracle-gate: only these offsets may go clean
        if VERIFIED_PATH and os.path.isfile(VERIFIED_PATH):
            _verified = set()
            for _ln in open(VERIFIED_PATH, encoding="utf-8", errors="replace"):
                _pp = _ln.strip().split("\t")
                # accept both manifest shapes: 2-field `MOD<tab>off` (memdifftest, clean_graph)
                # and 3-field `MOD<tab>seg<tab>off` (difftest). The offset is always the last
                # field and is section-relative in every writer (same convention as sec1_*.asm).
                if len(_pp) in (2, 3) and _pp[0].upper() == module.upper():
                    try:
                        _verified.add(int(_pp[-1], 16))
                    except Exception:
                        pass
        _exp_offs = set()                                  # exports: Win95 calls them stdcall(+ecx this)
        _ex0 = pe.exports()
        for _t0 in (_ex0.items() if isinstance(_ex0, dict) else _ex0):
            if isinstance(_t0, (list, tuple)) and len(_t0) == 3:
                _exp_offs.add(_t0[2] - tv)
        _raw, _regp, _unsafe = {}, {}, set()
        for _o in sorted(set(funcs) - bad):
            _ap = os.path.join(layout.asm_module_dir(module), "sec1_%05x.asm" % _o)
            if not os.path.isfile(_ap):
                _unsafe.add(_o); continue
            # SOUND register-input recovery: reg_inputs=None makes il use its DATAFLOW liveness, which
            # catches every register read-before-write (all branch/loop paths), so we never MISS a real
            # input (the textual-first heuristic did -> deregisterized wrong -> crash). Over-approx
            # (callee-save) is harmless for internals: the extra reg param is threaded and only saved/
            # restored. il emits recovered inputs as LEADING params (register-named).
            try:
                _cb = il.lift_asm_function(_ap, "sub_%x" % _o, clean=True, reg_inputs=None)
            except Exception:
                _unsafe.add(_o); continue
            # GLOBAL SYMBOLIZATION: rewrite module-range absolute globals (*(u32*)(0x7fe23b28)) to the
            # relocatable (_imgbase + rva) form -- _imgbase is the real load base, so the ref resolves
            # to the drop-in's OWN (byte-exact-preserved) global instead of a hardcoded constant that
            # points at wrong memory once the DLL is relocated (the SHELL32 desktop-garble class).
            _cb = il.reloc_imgbase(_cb, base)
            _rp = _leading_reg_params(_cb, _o)
            # Any absolute deref STILL present is out-of-module (truly non-relocatable) -> byte-exact.
            if re.search(r"\*\(u\d+\*\)\(0x[0-9a-f]{4,}\)", _cb):
                _unsafe.add(_o); continue
            # ORACLE GATE (strict): a fn ships clean ONLY if the differential oracle proved its exact
            # deregisterized emission bit-identical to Microsoft's bytes (in _verified). EVERYTHING else
            # -- including "zero-register" fns (whose liveness could be a false negative) -- stays
            # byte-exact. Correct-by-construction: no unproven deregisterization ever ships.
            if _verified is not None and _o not in _verified:
                _unsafe.add(_o); continue
            # EXPORTS can only receive this=ecx from the stdcall thunk; an export with esi/edi/edx as a
            # leading param would shift its stack args (the thunk can't feed those) -> keep byte-exact.
            if _o in _exp_offs and (set(_rp) - {"ecx"}):
                _unsafe.add(_o); continue
            _regp[_o] = _rp
            _raw[_o] = _cb
        for _o in list(_raw):            # thread each callee's recovered register args into call-sites
            _b = _raw[_o]
            for _c in _raw:
                if _regp.get(_c):
                    _b = _thread(_b, _c, _regp[_c], _o == _c)
            _raw[_o] = _b
        bad = bad | _unsafe                                # non-recoverable good fns -> byte-exact wrappers
        # BOUNDARY SYNC (replaces poison propagation -> preserves yield, no cascade). A clean fn keeps
        # its CPU state in LOCALS; a byte-exact fn's wrapper loads the GLOBAL register file into the real
        # regs, runs Microsoft's bytes, saves them back. So at a clean->byte-exact call, publish the
        # caller's locals to the globals just before, and read the callee's register effects back just
        # after. il emits every call as its own statement, so wrap each byte-exact call statement.
        _SO = "*g_eax=eax,*g_ebx=ebx,*g_ecx=ecx,*g_edx=edx,*g_esi=esi,*g_edi=edi"
        _SI = "eax=*g_eax,ebx=*g_ebx,ecx=*g_ecx,edx=*g_edx,esi=*g_esi,edi=*g_edi"
        _callpat = re.compile(r"\bsub_([0-9a-f]+)\s*\(")

        def _bridge(_body):
            _out = []
            for _ln in _body.split("\n"):
                _m = _callpat.search(_ln)
                if _m and int(_m.group(1), 16) in bad and _ln.rstrip().endswith(";"):
                    _ind = _ln[:len(_ln) - len(_ln.lstrip())]
                    _out.append("%s{ %s; %s %s; }" % (_ind, _SO, _ln.strip(), _SI))
                else:
                    _out.append(_ln)
            return "\n".join(_out)
        for _o in list(_raw):
            _raw[_o] = _bridge(_raw[_o])
            if REDIRECT:                          # publish non-eax register outputs so the redirect
                _raw[_o] = _redir_publish(_raw[_o])   # thunk can propagate them to byte-exact callers
        src_clean = "\n".join(_raw[_o] for _o in sorted(_raw))
        _CLEAN_REGP.clear()                                # exports still clean keep their thiscall thunks
        _CLEAN_REGP.update({o: r for o, r in _regp.items() if o in _raw})
    # embed the original image + imports
    img = bytearray(simg)
    for s in pe.sections:
        raw = pe.data[s["rawptr"]:s["rawptr"] + s["rawsize"]]
        img[s["vaddr"]:s["vaddr"] + len(raw)] = raw
    with open(os.path.join(outdir, sym + "_pkg.h"), "w") as f:
        f.write("static const unsigned char IMGDATA[]={%s};\n" % ",".join(str(b) for b in img))
        _entry = getattr(pe, "entry", 0) or 0     # AddressOfEntryPoint (RVA) -> module's own DllMain
        f.write("#define IMGSZ %du\n#define IMGBASE 0x%08xu\n#define ENTRY_RVA 0x%xu\n" % (simg, base, _entry))
        f.write("#define RELOC_RVA 0x%xu\n#define RELOC_SZ %du\n" % (_reloc_rva, _reloc_sz))
        imports = pe.imports()
        # include ORDINAL imports too (name "#N" -> ord=N); Win95 DLLs import kernel32 etc.
        # heavily by ordinal, and leaving those IAT slots unresolved crashes on the first call.
        f.write("typedef struct{unsigned slotrva;const char*dll,*name;unsigned ord;}imp_t;\nstatic const imp_t IMPS[]={\n")
        for slot_va, (dll, name) in sorted(imports.items()):
            if name.startswith("#"):
                f.write('  {0x%xu,"%s",0,%du},\n' % (slot_va - base, dll, int(name[1:])))
            else:
                f.write('  {0x%xu,"%s","%s",0u},\n' % (slot_va - base, dll, name))
        f.write("};\n#define NIMP (sizeof(IMPS)/sizeof(IMPS[0]))\n")
    # the module TU: prelude + _imgbase + clean C + jump-stubs for bad fns
    with open(os.path.join(outdir, sym + "_reloc.c"), "w") as f:
        f.write('#include "win95c.h"\nunsigned _imgbase;\n')
        # ASM-origin functions: emit their byte-exact instructions as editable naked-ASM source
        # (inter-fn calls symbolic so they link; data refs stay absolute -> resolve against the
        # reconstituted image at ImageBase). This is the genuine C+ASM mix: C where it was C, ASM
        # where Microsoft wrote ASM.
        # DEFECTIVE functions run Microsoft's byte-exact original code at its reconstituted
        # address (_imgbase + rva, rva = text_vaddr + off). The original is STDCALL (ret N);
        # the surrounding lifted C is cdecl (caller cleans). Calling the original directly would
        # double-clean the args -> stack smash the moment the code is actually executed. So emit a
        # CDECL-PRESENTING wrapper: call the original, then undo its `ret N` cleanup, and return
        # cdecl (ret 0) so the C caller's own cleanup is correct. Stack stays balanced regardless.
        nasm = 0
        for o in sorted(bad):
            ret, params = badsig.get("sub_%x" % o, ("u32", ""))
            rva = (tv + o) & 0xffffffff                    # runtime addr = _imgbase + rva; mask so a
            # forwarder/low-address export (o wrapped negative, e.g. 0xfffff033) recovers its intended
            # low rva (0x33) instead of overflowing 32 bits -> `addl $0x100000033` (assembler error).
            N = _retN(img, rva, base)
            if MSVC:
                # MSVC (__declspec(naked)+Intel __asm). First cut: arg-copy cdecl wrapper (no
                # register threading yet -- clash-free: the register-globals are NAMED eax.. which
                # collide with asm register keywords, so we don't reference them here). Copy N arg
                # bytes, call the original (stdcall ret N cleans the copy), return cdecl.
                if N is not None and 0 <= N <= 0x400 and N % 4 == 0 and N > 0:
                    # register-sync wrapper: copy N args, load globals->real regs, call the
                    # original (stdcall ret N cleans the copies), save real regs->globals, ret cdecl.
                    f.write('%s sub_%x();\n'
                            '__declspec(naked) %s sub_%x(){ __asm {\n'
                            '  push ebp\n  mov ebp, esp\n  sub esp, 8\n'        # [ebp-4]=orig [ebp-8]=result
                            '  push ebx\n  push esi\n  push edi\n'             # PRESERVE caller callee-saved
                            '  mov eax, _imgbase\n  add eax, 0x%x\n  mov [ebp-4], eax\n'
                            '  mov ecx, %d\n  L1: push dword ptr [ebp+ecx*4+4]\n  loop L1\n'
                            '  mov eax, g_ebx\n  mov ebx, [eax]\n'              # globals -> real (eax last)
                            '  mov eax, g_ecx\n  mov ecx, [eax]\n'
                            '  mov eax, g_edx\n  mov edx, [eax]\n'
                            '  mov eax, g_esi\n  mov esi, [eax]\n'
                            '  mov eax, g_edi\n  mov edi, [eax]\n'
                            '  mov eax, g_eax\n  mov eax, [eax]\n'
                            '  call dword ptr [ebp-4]\n'
                            '  mov [ebp-8], eax\n'                             # real -> globals (stash eax)
                            '  mov eax, g_ebx\n  mov [eax], ebx\n'
                            '  mov eax, g_ecx\n  mov [eax], ecx\n'
                            '  mov eax, g_edx\n  mov [eax], edx\n'
                            '  mov eax, g_esi\n  mov [eax], esi\n'
                            '  mov eax, g_edi\n  mov [eax], edi\n'
                            '  mov ebx, [ebp-8]\n  mov eax, g_eax\n  mov [eax], ebx\n'
                            '  pop edi\n  pop esi\n  pop ebx\n'                # RESTORE caller callee-saved
                            '  mov eax, [ebp-8]\n'                             # return value
                            '  mov esp, ebp\n  pop ebp\n  ret\n} }\n'
                            % (ret, o, ret, o, rva, N // 4))
                else:                                       # no clean N -> tail-jmp to original
                    f.write('%s sub_%x();\n'
                            '__declspec(naked) %s sub_%x(){ __asm {\n'
                            '  mov eax, _imgbase\n  add eax, 0x%x\n  jmp eax\n} }\n'
                            % (ret, o, ret, o, rva))
                nasm += 1
                continue
            if N is not None and 0 <= N <= 0x400 and N % 4 == 0:
                # The lift THREADS CPU state through global register variables (eax..edi); the
                # byte-exact original uses the REAL registers. So around the call we must sync:
                # load globals->real regs (correct INPUT state) before, store real->globals after
                # (propagate the original's effects). Also: copy the N args, let the original's
                # `ret N` clean the COPY, and return cdecl so the caller cleans its own args.
                # Original address is saved in the ebp frame at [ebp-4] (regs are all reloaded).
                loop = ('  "mov $%d,%%ecx\\n\\t"\n  "1: pushl 4(%%ebp,%%ecx,4)\\n\\t"\n  "loop 1b\\n\\t"\n'
                        % (N // 4)) if N > 0 else ""
                f.write('__attribute__((naked,used)) %s sub_%x(%s){ __asm__ volatile(\n'
                        '  "push %%ebp\\n\\t"\n'
                        '  "mov %%esp,%%ebp\\n\\t"\n'
                        # PRESERVE the caller\'s callee-saved regs. The global-sync below overwrites the
                        # real ebx/esi/edi with the register-file globals -- fine for a clean caller (it
                        # reads state from globals), but an EXTERNAL stdcall caller (SHELL32, win95)
                        # expects ebx/esi/edi/ebp preserved across the call. Without this, its ebx became
                        # the stale global -> `add ebx,eax` -> deref garbage -> crash (COMCTL32 #356).
                        '  "push %%ebx\\n\\t"\n'           # [ebp-4]  caller ebx
                        '  "push %%esi\\n\\t"\n'           # [ebp-8]  caller esi
                        '  "push %%edi\\n\\t"\n'           # [ebp-12] caller edi
                        '  "movl __imgbase,%%eax\\n\\t"\n'
                        '  "addl $0x%x,%%eax\\n\\t"\n'
                        '  "push %%eax\\n\\t"\n'           # [ebp-16] = original address
                        '%s'                               # copy N args (above the saved address)
                        '  "movl _eax,%%eax\\n\\t"\n'      # load threaded input state: globals -> real
                        '  "movl _ebx,%%ebx\\n\\t"\n'
                        '  "movl _ecx,%%ecx\\n\\t"\n'
                        '  "movl _edx,%%edx\\n\\t"\n'
                        '  "movl _esi,%%esi\\n\\t"\n'
                        '  "movl _edi,%%edi\\n\\t"\n'
                        '  "call *-16(%%ebp)\\n\\t"\n'     # run the original with correct input regs
                        '  "movl %%eax,_eax\\n\\t"\n'      # store output state: real -> globals
                        '  "movl %%ebx,_ebx\\n\\t"\n'
                        '  "movl %%ecx,_ecx\\n\\t"\n'
                        '  "movl %%edx,_edx\\n\\t"\n'
                        '  "movl %%esi,_esi\\n\\t"\n'
                        '  "movl %%edi,_edi\\n\\t"\n'
                        '  "mov -4(%%ebp),%%ebx\\n\\t"\n'  # restore caller callee-saved (eax=return kept)
                        '  "mov -8(%%ebp),%%esi\\n\\t"\n'
                        '  "mov -12(%%ebp),%%edi\\n\\t"\n'
                        '  "mov %%ebp,%%esp\\n\\t"\n'
                        '  "pop %%ebp\\n\\t"\n'
                        '  "ret\\n"\n'                     # cdecl (result already in real eax)
                        '  ); }\n' % (ret, o, params, rva, loop))
                nasm += 1
            else:                                          # unknown cleanup -> jmp to original (rare)
                f.write('__attribute__((naked,used)) %s sub_%x(%s){ __asm__ volatile('
                        '"movl __imgbase,%%eax\\n\\taddl $0x%x,%%eax\\n\\tjmp *%%eax"); }\n' % (ret, o, params, rva))
        f.write("\n")
        if CLEAN:                        # emit the DEREGISTERIZED good fns in place of the register-machine src
            src = src_clean
        if MSVC:
            # PURE FORWARDERS: a function whose ENTIRE body is one `goto *(*(u32*)(_imgbase+off))`
            # is an import thunk -- the original `jmp [IAT slot]`. Emitting it as a framed C
            # function makes MSVC lay down push ebp / locals; the `jmp` then leaves that frame
            # standing, so the stdcall import's `ret N` returns into the saved registers ->
            # crash (the LINKINFO Explorer "internal error"). Emit them FRAMELESS
            # (__declspec(naked)): esp/ebp are untouched, the import returns straight to OUR
            # caller and cleans its N arg bytes -- byte-for-byte the original thunk's behaviour.
            # `[\s)]*;` after off guarantees a FIXED slot (no `+ idx*4`), so real switch jump
            # tables (which also have index math and more than one statement) are never matched.
            # Must run BEFORE the generic goto* rewrite below (else it wraps them in a frame).
            src = re.sub(
                r'(?P<sig>(?:u32|int|void|short|long)\s+sub_[0-9a-f]+\s*\([^)]*\))\s*\{\s*'
                r'goto\s*\*[^;\n]*_imgbase\s*\+\s*(?P<off>0x[0-9a-fA-F]+)[\s)]*;[^\n]*\n\s*\}',
                lambda m: '__declspec(naked) %s{ __asm {\n'
                          '  mov eax, _imgbase\n  add eax, %s\n  jmp dword ptr [eax]\n} }'
                          % (m.group('sig'), m.group('off')),
                src)
            # esp-guard fnptr calls: the lift's call-site arg counts don't always match the real
            # API's, and MSVC's push model leaves esp drifted (an under-count drifts UP and clobbers
            # the frame). Save esp before each single-line fnptr-call statement and restore after ->
            # always balanced, args unchanged. (mingw's accumulate model self-corrects instead.)
            src = re.sub(r'^([ \t]*)(\S[^\n]*\(\*\(fnptr\)[^\n]*;)[ \t]*$',
                         r'\1{ unsigned __sp; __asm{mov __sp,esp} \2 __asm{mov esp,__sp} }',
                         src, flags=re.M)
            # MSVC has no computed goto: `goto *(EXPR);` -> inline-asm indirect jump (same runtime
            # behavior -- jump to the computed address). eax here is the REAL register (no clash).
            src = re.sub(r'goto\s*\*\s*([^;]+);',
                         lambda m: "{ u32 _gt=(u32)(%s); __asm{mov eax,_gt} __asm{jmp eax} }" % m.group(1).strip(),
                         src)
        if CLEAN:                            # K&R the clean fn definitions so the lift's arity-flexible
            src = _knr_defs(src)             # calls compile (no prototype -> no arity check; gcc + MSVC)
            src = _clean_fixups(src, MSVC)   # + naked-wrapper arity, &<const>, gcc computed-goto cast
        f.write(src)
        if REDIRECT and CLEAN and _raw:
            # REDIRECTION THUNKS: a clean INTERNAL fn is dead unless something reaches it -- byte-exact
            # callers run reconstituted ORIGINAL bytes, whose calls stay in the reconstituted image. So
            # for each clean fn we generate a marshaling thunk and (in DllMain) patch its entry in the
            # reconstituted image to `jmp thunk`. The thunk presents the byte-exact caller's REAL register
            # inputs as the clean C's leading params, calls the clean C, then does the original `ret N`
            # cleanup -- the register save/restore boundary that makes sparse gating deregisterize LIVE code.
            f.write("\n/* --- redirection thunks (byte-exact caller -> clean C) --- */\n")
            f.write("unsigned redir_hits; static unsigned thk_ret;\n")
            _rtbl = []
            for _o in sorted(_raw):
                _regs = _regp.get(_o, [])                 # leading register params, in signature order
                _N = _retN(img, tv + _o, base)            # original callee stack cleanup (bytes)
                if _N is None:
                    continue                              # unknown cleanup -> leave unpatched (byte-exact)
                _nm = "sub_%x" % _o
                _GP = ("eax", "ecx", "edx", "ebx", "esi", "edi", "ebp")
                # MULTI-REGISTER OUTPUTS: publish the caller's real regs to the global register-file on
                # entry (so registers the fn never touches pass through unchanged), and restore from it
                # on exit. The clean fn publishes its MODIFIED register-locals to that file before each
                # return (see _redir_publish), so non-eax outputs (edx secondary return, esi/edi effects)
                # propagate back to the byte-exact caller -- not just eax.
                _pub = ['  "movl %%%s,_%s\\n\\t"' % (r, r) for r in _GP]
                # restore EXCLUDES eax: the C return value is already in the real eax and is the
                # authoritative eax output (a fn returning a constant never assigns the eax local, so
                # its global is stale -- restoring it would clobber the return).
                _rst = ['  "movl _%s,%%%s\\n\\t"' % (r, r) for r in _GP if r != "eax"]
                _ls = ['__attribute__((naked,used)) void %s_redir(void){ __asm__ volatile(' % _nm,
                       '  "incl _redir_hits\\n\\t"'] + _pub + [   # handoff counter + publish input regs
                       '  "popl _thk_ret\\n\\t"']                 # save return addr (clean leaves -> single slot OK)
                for _r in reversed(_regs):                 # push reg args reverse so param0 ends on top
                    _ls.append('  "pushl %%%s\\n\\t"' % _r)
                _ls += ['  "call _%s\\n\\t"' % _nm,
                        '  "addl $%d,%%esp\\n\\t"' % (len(_regs) * 4 + _N)] + _rst + [  # drop args + restore output regs
                        '  "pushl _thk_ret\\n\\t"',
                        '  "ret\\n"',
                        '  ); }']
                f.write("\n".join(_ls) + "\n")
                _rtbl.append((tv + _o, _nm))
            f.write("typedef void(*_thkfn)(void);\ntypedef struct{unsigned rva;_thkfn fn;}redir_t;\n")
            f.write("redir_t REDIR[]={%s};\n" % ",".join("{0x%xu,(_thkfn)%s_redir}" % (rva, nm) for rva, nm in _rtbl))
            f.write("unsigned NREDIR=%d;\n" % len(_rtbl))
            f.write("unsigned get_redir_hits(void){return redir_hits;}\nunsigned get_imgbase(void){return _imgbase;}\n")
    # DllMain: reconstitute at ORIGINAL ImageBase (executable), populate IAT, set _imgbase
    with open(os.path.join(outdir, sym + "_glue.c"), "w") as f:
        # WIN32_LEAN_AND_MEAN skips objbase.h/OLE (MSVC 2.0's objbase.h uses the obsolete __export).
        f.write('#define WIN32_LEAN_AND_MEAN\n#include <windows.h>\n#include "%s_pkg.h"\nextern unsigned _imgbase;\n' % sym)
        if REDIRECT:
            f.write("typedef void(*_thkfn)(void);\ntypedef struct{unsigned rva;_thkfn fn;}redir_t;\n"
                    "extern redir_t REDIR[];extern unsigned NREDIR;\n")
        # patch each clean fn's entry in the reconstituted image to `jmp thunk` (E9 rel32) BEFORE the
        # original entry runs, so byte-exact callers detour into the clean C via the marshaling thunk.
        _rpatch = ("    { unsigned _k; for(_k=0;_k<NREDIR;_k++){ unsigned char*_q=b+REDIR[_k].rva;\n"
                   "      unsigned _t=(unsigned)(size_t)REDIR[_k].fn;\n"
                   "      _q[0]=0xE9; *(unsigned*)(_q+1)=_t-((unsigned)(size_t)_q+5); } }\n") if REDIRECT else ""
        # base-relocate the reconstituted image if it did NOT land on its preferred base (delta!=0):
        # add delta to every HIGHLOW (type 3) fixup so absolute pointers in code+data resolve correctly.
        _rreloc = ("    { unsigned _d=(unsigned)(size_t)b-IMGBASE;\n"
                   "      if(_d && RELOC_SZ){ unsigned char*_rp=b+RELOC_RVA,*_re=_rp+RELOC_SZ; unsigned _pg,_bl,_n,_j;\n"
                   "        while(_rp+8<=_re){ _pg=*(unsigned*)_rp; _bl=*(unsigned*)(_rp+4); if(_bl<8) break;\n"
                   "          { unsigned short*_e=(unsigned short*)(_rp+8); _n=(_bl-8)/2;\n"
                   "            for(_j=0;_j<_n;_j++) if((_e[_j]>>12)==3) *(unsigned*)(b+_pg+(_e[_j]&0xFFF))+=_d; }\n"
                   "          _rp+=_bl; } } }\n")
        f.write("BOOL WINAPI DllMain(HINSTANCE h,DWORD r,LPVOID x){ unsigned i;\n"
                "  if(r==DLL_PROCESS_ATTACH){\n"
                "    unsigned char*b=(unsigned char*)VirtualAlloc((void*)IMGBASE,IMGSZ,MEM_RESERVE|MEM_COMMIT,PAGE_EXECUTE_READWRITE);\n"
                "    if(!b) b=(unsigned char*)VirtualAlloc(NULL,IMGSZ,MEM_RESERVE|MEM_COMMIT,PAGE_EXECUTE_READWRITE);\n"
                "    memcpy(b,IMGDATA,IMGSZ); _imgbase=(unsigned)(size_t)b;\n"
                + _rreloc + _rpatch +
                "    for(i=0;i<NIMP;i++){\n"
                "      /* GetModuleHandle first: a pure lookup, NOT a load. Calling LoadLibrary here\n"
                "         (inside DllMain PROCESS_ATTACH) re-enters KERNEL32's loader mid-load and\n"
                "         corrupts its module database -> the loader-walk then faults. */\n"
                "      HMODULE m=GetModuleHandleA(IMPS[i].dll); if(!m) m=LoadLibraryA(IMPS[i].dll);\n"
                "      { FARPROC p=m?GetProcAddress(m, IMPS[i].name?IMPS[i].name:(LPCSTR)(size_t)IMPS[i].ord):0;\n"
                "        if(p)*(void**)(b+IMPS[i].slotrva)=(void*)p; }}\n"
                "    if(ENTRY_RVA){ BOOL(WINAPI*om)(HINSTANCE,DWORD,LPVOID)=(BOOL(WINAPI*)(HINSTANCE,DWORD,LPVOID))(b+ENTRY_RVA);\n"
                "      om((HINSTANCE)b, DLL_PROCESS_ATTACH, 0); }\n"
                "  } return TRUE; }\n")
    # export .def -- CLEAN-C exports are compiled cdecl (ret 0) but Win95 calls exports
    # stdcall (ret N). Wrap each clean-C export whose ORIGINAL ends in `ret N` (N>0) in a
    # naked stdcall thunk so the arg bytes are cleaned -> no stack imbalance when called.
    ex = pe.exports()
    items = ex.items() if isinstance(ex, dict) else ex
    lines = ["LIBRARY %s" % module.split(".")[0], "EXPORTS"]
    cleanset = set(funcs) - set(bad)      # functions emitted as editable C (cdecl, ret 0)
    thunks, wrapped, nwrap, n = [], {}, 0, 0
    for t in items:
        if not (isinstance(t, (list, tuple)) and len(t) == 3):
            continue
        ordn, name, rva = t
        off = rva - tv
        if not (0 <= off < 0x100000):      # keep ORDINAL-ONLY (NONAME) exports too, not just named
            continue
        target = "sub_%x" % off
        # Win95 calls every export STDCALL. The inner sub_<off> is cdecl (clean-C, or the
        # cdecl-presenting wrapper for a defective fn). Wrap EVERY export whose original ends
        # in `ret N` (N>0) in a stdcall thunk so the arg bytes are cleaned exactly once.
        if off in wrapped:                           # another export name aliases this fn
            target = wrapped[off]
        else:
            sig = SDK_API.get(name) if name else None
            if sig and sig.get("varargs"):
                N = 0                                # cdecl varargs export (wsprintf..) -> no stdcall cleanup
            else:
                N = _retN(img, rva, base)            # binary ret N = ground truth
                if (not N) and sig:                  # binary unknown -> documented arg count (authoritative)
                    N = sig["argc"] * 4
            this = _CLEAN_REGP.get(off) == ["ecx"]        # thiscall export: deregisterized this=ecx param
            Nok = N if (N and 0 < N <= 0x400 and N % 4 == 0) else 0
            if Nok or this:
                wrap = "expw_%x" % off
                thunks.append(_stdcall_thunk(wrap, target, Nok, has_this=this))
                wrapped[off] = wrap; target = wrap; nwrap += 1
        if name:
            lines.append("  %s=%s @%d" % (name, target, ordn))
        else:                              # ordinal-only export (e.g. shell32's undocumented ordinals)
            lines.append("  %s @%d NONAME" % (target, ordn))
        n += 1
    if thunks:
        with open(os.path.join(outdir, sym + "_reloc.c"), "a") as f:
            f.write("\n/* stdcall export thunks (clean N arg bytes; Win95 calls exports stdcall) */\n")
            f.write("".join(thunks))
    if REDIRECT:                                   # verification hooks: handoff counter + reconstituted base
        lines.append("  get_redir_hits")
        lines.append("  get_imgbase")
    open(os.path.join(outdir, sym + ".def"), "w").write("\n".join(lines) + "\n")
    return sym, len(bad), len(funcs), n, base


if __name__ == "__main__":
    mod = sys.argv[1]
    outdir = sys.argv[sys.argv.index("--out") + 1] if "--out" in sys.argv else "."
    sym, nbad, nfn, nexp, base = build_ff(mod, outdir)
    print("full-fidelity %s: %d/%d fns fall back to original bytes; %d exports; ImageBase 0x%08x"
          % (mod, nbad, nfn, nexp, base))
    print("  link with a DIFFERENT --image-base so Win95 loads the C code off the original base.")
