#!/usr/bin/env python3
r"""recover_cond.py -- fix the biggest lift-defect class: `if (1 /* cond not recovered */)`.

il.py loses which x86 condition a branch tests (~123k sites). But the byte-exact ASM still has the
real `jz`/`jc`/`jle`/... and its TARGET offset -- and the lifted C's `if(1 ...) goto L_<target>`
carries that same target. So we recover the condition by matching target offsets and mapping the
x86 jcc to the register-machine flag expression (ZF/CF/SF/OF/PF). Verified by the differential
oracle. This raises functional faithfulness without touching il.py.

    python recover_cond.py <module>            # report resolvable count
    recover_src(module, src) -> patched src    # used by emit paths
"""
import os, re, sys, glob
import hybrid_emit as H

# x86 jcc -> register-machine flag expression (globals CF/ZF/SF/OF/PF the lift maintains)
JCC = {
    "je": "ZF", "jz": "ZF", "jne": "!ZF", "jnz": "!ZF",
    "jb": "CF", "jc": "CF", "jnae": "CF", "jae": "!CF", "jnc": "!CF", "jnb": "!CF",
    "jbe": "(CF||ZF)", "jna": "(CF||ZF)", "ja": "(!CF&&!ZF)", "jnbe": "(!CF&&!ZF)",
    "js": "SF", "jns": "!SF", "jo": "OF", "jno": "!OF", "jp": "PF", "jpe": "PF", "jnp": "!PF", "jpo": "!PF",
    "jl": "(SF!=OF)", "jnge": "(SF!=OF)", "jge": "(SF==OF)", "jnl": "(SF==OF)",
    "jle": "(ZF||(SF!=OF))", "jng": "(ZF||(SF!=OF))", "jg": "(!ZF&&(SF==OF))", "jnle": "(!ZF&&(SF==OF))",
}
_J = re.compile(r'^\s*(j[a-z]+)\s+(?:short\s+)?L?_?([0-9a-f]+)h?\b', re.I)


def asm_cond_map(module):
    """{target_offset: flag_expr} from the module's byte-exact ASM conditional jumps."""
    ent = H._index().get(module.strip().lower())
    if not ent:
        return {}
    mdir, prefix = ent
    out = {}
    for af in glob.glob(os.path.join(mdir, prefix + "*_*.asm")):
        for line in open(af, encoding="utf-8", errors="replace"):
            m = _J.match(line)
            if m:
                mn = m.group(1).lower()
                if mn in JCC:
                    out.setdefault(int(m.group(2), 16), mn)
    return out


def recover_src(module, src):
    """Patch `if (1 /* cond not recovered */) goto L_<t>` -> `if (<flag expr>) goto L_<t>` using ASM."""
    cmap = asm_cond_map(module)
    n = [0]

    def repl(m):
        tgt = int(m.group(1), 16)
        mn = cmap.get(tgt)
        if mn:
            n[0] += 1
            return "if (%s) goto L_%s" % (JCC[mn], m.group(1))
        return m.group(0)
    src = re.sub(r'if \(1 /\* cond not recovered \*/\) goto L_([0-9a-f]+)', repl, src)
    return src, n[0]


if __name__ == "__main__":
    module = sys.argv[1]
    c = glob.glob(os.path.join(os.environ.get("WT_DATA_ROOT", "."), "src", "generated", "**", module + ".c"), recursive=True)
    src = open(c[0], encoding="utf-8", errors="replace").read()
    total = len(re.findall(r'if \(1 /\* cond not recovered \*/\) goto L_[0-9a-f]+', src))
    ext = len(re.findall(r'if \(1 /\* cond not recovered \*/\) goto \*', src))
    _p, n = recover_src(module, src)
    print("%s: %d label-goto not-recovered conditions, %d RECOVERED from ASM (%d%%); %d external-goto (harder)"
          % (module, total, n, 100 * n // max(1, total), ext))
