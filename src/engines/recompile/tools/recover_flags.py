#!/usr/bin/env python3
r"""recover_flags.py -- recover the CONDITION *and* its data for `if (1 /* cond not recovered */)`.

recover_cond.py maps the jcc to a flag expr, but the lift drops the FLAG side-effects of the
setting instruction, so the flag is never computed. This instead reads the ASM's flag-setting
instruction that precedes each conditional jump and re-derives the branch as a DIRECT comparison
on the register-machine globals -- no flags needed:
    cmp [X],0 ; jne   ->  if (*(u32*)(X) != 0)
    test eax,eax ; je ->  if (eax == 0)
    cmp al,5 ; jb     ->  if ((u32)al < (u32)5)
Covers the dominant cmp/test setters (the vast majority of branches). Verified by the oracle.

    recover_flags_src(module, src) -> (patched_src, n)
"""
import os, re, glob
import hybrid_emit as H

# how a jcc turns a cmp result (A - B) into a C comparison
CMP = {
    "je": "{A} == {B}", "jz": "{A} == {B}", "jne": "{A} != {B}", "jnz": "{A} != {B}",
    "jb": "(u32)({A}) < (u32)({B})", "jc": "(u32)({A}) < (u32)({B})", "jnae": "(u32)({A}) < (u32)({B})",
    "jae": "(u32)({A}) >= (u32)({B})", "jnb": "(u32)({A}) >= (u32)({B})", "jnc": "(u32)({A}) >= (u32)({B})",
    "jbe": "(u32)({A}) <= (u32)({B})", "jna": "(u32)({A}) <= (u32)({B})",
    "ja": "(u32)({A}) > (u32)({B})", "jnbe": "(u32)({A}) > (u32)({B})",
    "jl": "(i32)({A}) < (i32)({B})", "jnge": "(i32)({A}) < (i32)({B})",
    "jge": "(i32)({A}) >= (i32)({B})", "jnl": "(i32)({A}) >= (i32)({B})",
    "jle": "(i32)({A}) <= (i32)({B})", "jng": "(i32)({A}) <= (i32)({B})",
    "jg": "(i32)({A}) > (i32)({B})", "jnle": "(i32)({A}) > (i32)({B})",
}
# test A,B sets flags on R=(A&B); being a LOGICAL op it clears OF and CF. So OF=CF=0:
#   signed jcc reduce to comparing R to 0 (SF==OF becomes SF==0); unsigned jcc reduce via CF=0
#   (jb never, jae always -- degenerate, omitted; ja/jbe collapse to !=0 / ==0). This covers the
#   ubiquitous `test reg,reg ; jge/jg/jl/jle/ja/jbe` idiom the base je/jne/js/jns set missed.
TST = {"je": "(({A}) & ({B})) == 0", "jz": "(({A}) & ({B})) == 0",
       "jne": "(({A}) & ({B})) != 0", "jnz": "(({A}) & ({B})) != 0",
       "js": "(i32)(({A}) & ({B})) < 0", "jns": "(i32)(({A}) & ({B})) >= 0",
       "jg": "(i32)(({A}) & ({B})) > 0", "jnle": "(i32)(({A}) & ({B})) > 0",
       "jge": "(i32)(({A}) & ({B})) >= 0", "jnl": "(i32)(({A}) & ({B})) >= 0",
       "jl": "(i32)(({A}) & ({B})) < 0", "jnge": "(i32)(({A}) & ({B})) < 0",
       "jle": "(i32)(({A}) & ({B})) <= 0", "jng": "(i32)(({A}) & ({B})) <= 0",
       "ja": "(({A}) & ({B})) != 0", "jnbe": "(({A}) & ({B})) != 0",
       "jbe": "(({A}) & ({B})) == 0", "jna": "(({A}) & ({B})) == 0"}
_ARITH = {"add", "sub", "and", "or", "xor", "shl", "shr", "sar", "sal", "inc", "dec", "neg", "adc", "sbb"}
# arithmetic sets flags on the RESULT (dest); je/jne test zero, js/jns test sign of the result
_RESULT = {"je": "({D}) == 0", "jz": "({D}) == 0", "jne": "({D}) != 0", "jnz": "({D}) != 0",
           "js": "(i32)({D}) < 0", "jns": "(i32)({D}) >= 0"}
_SZ = {"byte": 8, "word": 16, "dword": 32, "qword": 64}
_REG = re.compile(r'^(e?[a-d]x|[a-d][lh]|e?[sd]i|e?bp|e?sp)$')


def _operand_c(op):
    """An x86 operand -> a C expression on the register-machine globals (absolute mem kept as-is;
    symbolize relocates later)."""
    op = op.strip()
    m = re.match(r'(byte|word|dword|qword)\s+ptr\s+\[(.+)\]$', op)
    if m:
        sz = _SZ[m.group(1)]
        addr = m.group(2).strip()
        addr = re.sub(r'\b([0-9a-fA-F]+)h\b', lambda x: "0x" + x.group(1), addr)   # 7e47h -> 0x7e47
        return "*(u%d*)(%s)" % (sz, addr)
    if re.match(r'^\[.+\]$', op):
        addr = re.sub(r'\b([0-9a-fA-F]+)h\b', lambda x: "0x" + x.group(1), op[1:-1])
        return "*(u32*)(%s)" % addr
    if _REG.match(op):
        return op
    if re.match(r'^[0-9a-fA-F]+h$', op):
        return "0x" + op[:-1]
    if re.match(r'^-?\d+$', op):
        return op
    if re.match(r'^0x[0-9a-fA-F]+$', op):
        return op
    return None                                                  # unhandled operand -> skip


_PARENT = {**{r: r for r in ("eax", "ebx", "ecx", "edx", "esi", "edi", "ebp", "esp")},
           "ax": "eax", "al": "eax", "ah": "eax", "bx": "ebx", "bl": "ebx", "bh": "ebx",
           "cx": "ecx", "cl": "ecx", "ch": "ecx", "dx": "edx", "dl": "edx", "dh": "edx",
           "si": "esi", "di": "edi", "bp": "ebp", "sp": "esp"}
# instructions whose EFFECT we can't easily bound -> stop the backward scan
_BARRIER = {"call", "ret", "retf", "int", "iret", "leave", "push", "pop", "pushad", "popad",
            "mul", "imul", "div", "idiv", "movs", "movsb", "movsd", "stos", "lods", "scas",
            "rep", "repe", "repne", "cdq", "cwde", "in", "out"}


def _regs(operand):
    return set(_PARENT.get(r, r) for r in re.findall(r'\b(e?[a-d]x|[a-d][lh]|e?[sd]i|e?bp|e?sp)\b', operand))


def _writes(mnem, ops):
    """32-bit-parent registers an instruction writes (its destination), or None if a barrier."""
    if mnem in _BARRIER:
        return None
    dest = ops.split(",")[0].strip() if ops else ""
    if re.match(r'^(byte|word|dword|qword)\s+ptr|\[', dest):     # memory dest: no reg written
        return set()
    return _regs(dest)


_LINE = re.compile(r';\s*([0-9a-f]+)\s+(.*)$')                    # trailing "; <off> <disasm/bytes>"


def _instrs(path):
    """[(off, mnemonic, operand_str)] for one function's asm (normal + db-with-comment lines)."""
    out = []
    for line in open(path, encoding="utf-8", errors="replace"):
        s = line.rstrip("\n")
        st = s.strip()
        if not st or st.startswith(("title", "page", ".386", "_TEXT", "assume", "end", "public", "extrn")) or re.match(r'^L_[0-9a-f]+:', st):
            continue
        if st.startswith("db"):                                  # db ... ; <off> <disasm>
            m = _LINE.search(s)
            if not m:
                continue
            toks = m.group(2).split(None, 1)                     # "<off> mnem ops" already split by _LINE
            dis = m.group(2)
        else:
            m = _LINE.search(s)
            dis = st.split(";")[0].strip()
            if not m:
                dis = st
        # offset + text
        mo = re.search(r';\s*([0-9a-f]+)\s+', s)
        off = int(mo.group(1), 16) if mo else None
        # the mnemonic/operands: prefer the real instruction text (db lines carry it in the comment)
        text = dis
        if st.startswith("db"):
            cm = re.search(r';\s*[0-9a-f]+\s+(.*)$', s)
            text = cm.group(1).strip() if cm else ""
        parts = text.split(None, 1)
        if not parts:
            continue
        mnem = parts[0].lower()
        ops = parts[1] if len(parts) > 1 else ""
        if off is not None:
            out.append((off, mnem, ops))
    return out


def _cond_map(module):
    """{jump_target_off: c_condition} from ASM cmp/test immediately before each conditional jump."""
    ent = H._index().get(module.strip().lower())
    if not ent:
        return {}
    mdir, prefix = ent
    out = {}
    for af in glob.glob(os.path.join(mdir, prefix + "*_*.asm")):
        ins = _instrs(af)
        for i, (off, mnem, ops) in enumerate(ins):
            if not re.match(r'^j[a-z]+$', mnem) or mnem == "jmp":
                continue
            mt = re.match(r'(?:short\s+)?L?_?([0-9a-f]+)h?', ops.strip())
            if not mt:
                continue
            tgt = int(mt.group(1), 16)
            # scan back to the flag-setter; safe as long as nothing between CLOBBERS its operands
            clob = set()                                            # 32-bit-parent regs written since
            for j in range(i - 1, -1, -1):
                pm, po = ins[j][1], ins[j][2]
                if pm in ("cmp", "test"):
                    a, _, b = po.partition(",")
                    if _regs(a) & clob or _regs(b) & clob:
                        break                                       # an operand was overwritten -> unsafe
                    A, B = _operand_c(a), _operand_c(b)
                    tbl = CMP if pm == "cmp" else TST
                    if A and B and mnem in tbl:
                        out[tgt] = tbl[mnem].format(A=A, B=B)
                    break
                if pm in _ARITH and mnem in _RESULT:                # arithmetic sets flags on its RESULT
                    if _regs(po.split(",")[0]) & clob:
                        break
                    dest = _operand_c(po.split(",")[0])
                    if dest:
                        out[tgt] = _RESULT[mnem].format(D=dest)
                    break
                w = _writes(pm, po)
                if w is None or pm.startswith("j"):                 # barrier / branch -> can't reason further
                    break
                clob |= w
    return out


def recover_flags_src(module, src):
    cmap = _cond_map(module)
    n = [0]

    def repl(m):
        c = cmap.get(int(m.group(1), 16))
        if c:
            n[0] += 1
            return "if (%s) goto L_%s" % (c, m.group(1))
        return m.group(0)
    src = re.sub(r'if \(1 /\* cond not recovered \*/\) goto L_([0-9a-f]+)', repl, src)
    return src, n[0]


if __name__ == "__main__":
    import sys
    module = sys.argv[1]
    c = glob.glob(os.path.join(os.environ.get("WT_DATA_ROOT", "."), "src", "generated", "**", module + ".c"), recursive=True)
    src = open(c[0], encoding="utf-8", errors="replace").read()
    total = len(re.findall(r'if \(1 /\* cond not recovered \*/\) goto L_[0-9a-f]+', src))
    _p, n = recover_flags_src(module, src)
    print("%s: %d label-goto not-recovered; %d recovered as DIRECT comparisons from ASM cmp/test (%d%%)"
          % (module, total, n, 100 * n // max(1, total)))
