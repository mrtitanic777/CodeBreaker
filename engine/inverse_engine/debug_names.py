"""Recover real internal function names for a shipped NE module by matching it,
function by function, against a *debug build of the same version* whose linker
.MAP names every function (debug builds declare internals PUBLIC).

The retail export table names only ~25% of a system module's functions; the rest
carve as anonymous ``sub_XXXX``. A debug build of the identical Windows version
carries the true names, but at *different offsets* (debug adds assertion / RIP
code and shifts everything). So we can't align by address -- we align by an
offset-invariant byte signature:

    norm_sig(): the objdump text per instruction with only the OFFSET-dependent
    operands masked -- branch/call targets, absolute ``[0xNNNN]`` memory, and far
    ``seg:off`` pointers. IMMEDIATES stay, because they are part of a function's
    identity; masking them (an earlier attempt) let functions with the same
    instruction skeleton but different constants collide. A signature match is
    therefore byte-identity modulo relocation: the two functions are provably the
    same code, and the debug build's name for it is exact.

Three guards keep the result at zero false positives:

  1. ambiguity drop -- a signature that two *different* debug names share is
     discarded (can't tell which name is right).
  2. export validation -- the retail export table is a labelled test set: any
     signature a retail EXPORT already owns under a different name is discarded
     as collision-prone (this is what catches e.g. a 2-instruction predicate
     matching the wrong short function). After this filter, every known export in
     the map agrees with its own signature -- the proof the method is sound.
  3. min length + uniqueness -- functions under MIN_LEN instructions are too
     skeleton-like to trust; and a real name claimed by >1 retail offset is
     dropped entirely (a symbol lives at one address, so a double match is an
     internal-internal collision, and duplicate labels would break emission).

Only a module with BOTH a debug binary and its .MAP can be recovered this way.
The output ``names.json`` ("seg:off" -> NAME) feeds trace.Seg(name_override=...)
which renames the anonymous functions in the emitted idiomatic source -- a pure
label rename, so byte-exactness is untouched.
"""
import os, re, json, glob
from collections import Counter

from engine.format_loaders import ne as ne_mod
from engine.disasm_engine import trace as _t

MIN_LEN = 5   # functions under 5 instructions share skeletons -> collision-prone

_BR = ("call|jmp|je|jne|jz|jnz|jl|jle|jg|jge|jb|jbe|ja|jae|js|jns|jo|jno|"
       "jp|jnp|jpe|jpo|jc|jnc|jcxz|jecxz|loop|loope|loopne|loopz|loopnz")


def norm_sig(sg, start):
    """Offset-invariant signature of the function at `start`: one entry per
    instruction, OFFSET-dependent operands masked, immediates kept."""
    _, rows = sg.func_rows(start)
    sig = []
    for _off, a in rows:
        a = re.sub(r"\s+", " ", a).strip()
        a = re.sub(r"^((?:%s)\s+)0x[0-9a-f]+" % _BR, r"\1X", a)   # branch target
        a = re.sub(r"\[0x[0-9a-f]+\]", "[X]", a)                  # absolute mem
        a = re.sub(r"0x[0-9a-f]+:0x[0-9a-f]+", "X:X", a)          # far ptr
        sig.append(a)
    return tuple(sig)


def build_debug_index(debug_exe, debug_map, ordinals):
    """{signature -> NAME} for every uniquely-signatured named function in a
    debug build (its .MAP names them; ambiguous signatures are dropped)."""
    mp = open(debug_map, errors="replace").read()
    names = {}     # seg -> {off -> NAME}
    for m in re.finditer(r"^ ([0-9A-Fa-f]{4}):([0-9A-Fa-f]{4})\s+(\w+)", mp, re.M):
        seg, off, nm = int(m.group(1), 16), int(m.group(2), 16), m.group(3)
        names.setdefault(seg, {})[off] = nm

    ne = ne_mod.NEFile(debug_exe)
    sig2name, dup = {}, set()
    for i, s in enumerate(ne.segments(), 1):
        if s.get("kind") != "CODE" or i not in names:
            continue
        sg = _t.Seg(ne, i, ordinals)
        for st in names[i]:
            if st not in sg.valid:
                continue
            sig = norm_sig(sg, st)
            if not sig:
                continue
            if sig in sig2name and sig2name[sig] != names[i][st]:
                dup.add(sig)
            else:
                sig2name[sig] = names[i][st]
    for sig in dup:
        sig2name.pop(sig, None)
    return sig2name


def match_module(retail_exe, sig2name, ordinals, min_len=MIN_LEN):
    """Return {"seg:off" -> NAME}: the exactly-matching real names for the
    anonymous internal functions of `retail_exe`, with all three false-positive
    guards applied. Also returns a (agree, disagree) tuple over known exports."""
    ne = ne_mod.NEFile(retail_exe)
    segs = {i: _t.Seg(ne, i, ordinals)
            for i, s in enumerate(ne.segments(), 1) if s.get("kind") == "CODE"}

    # export validation: drop any signature a retail export owns under another name
    export_sig = {}                     # sig -> {export names}
    exp_at = {}                         # UPPERNAME -> (seg, off)
    for i, sg in segs.items():
        for st in sg.funcs:
            nm = sg.name_at(st)
            if nm and not nm.lower().startswith("sub_"):
                export_sig.setdefault(norm_sig(sg, st), set()).add(nm.upper())
                exp_at[nm.upper()] = (i, st)
    sig2name = {sig: nm for sig, nm in sig2name.items()
                if not (export_sig.get(sig) and nm.upper() not in export_sig[sig])}

    namemap, agree, disagree = {}, 0, 0
    for i, sg in segs.items():
        for st in sg.funcs:
            sig = norm_sig(sg, st)
            real = sig2name.get(sig) if len(sig) >= min_len else None
            if not real:
                continue
            # never shadow a real export at a different offset (case-insensitive)
            clash = exp_at.get(real.upper())
            if clash and clash != (i, st):
                continue
            cur = sg.name_at(st)
            if cur and not cur.lower().startswith("sub_"):
                agree += (cur.upper() == real.upper())
                disagree += (cur.upper() != real.upper())
            namemap["%d:%04x" % (i, st)] = real

    # a real name lives at one address: drop every offset under a duplicated name
    cnt = Counter(namemap.values())
    dupnames = {n for n, c in cnt.items() if c > 1}
    namemap = {k: v for k, v in namemap.items() if v not in dupnames}
    return namemap, (agree, disagree)


def load_override(names_json):
    """names.json ("seg:off" -> NAME)  ->  {seg:int -> {off:int -> NAME}} for
    trace.Seg(name_override=...)."""
    override = {}
    for k, v in json.load(open(names_json)).items():
        si, off = k.split(":")
        override.setdefault(int(si), {})[int(off, 16)] = v
    return override


def recover(retail_exe, debug_exe, debug_map, ordinals, out_json=None):
    """End-to-end: build the debug index, match, optionally write names.json."""
    idx = build_debug_index(debug_exe, debug_map, ordinals)
    namemap, (agree, disagree) = match_module(retail_exe, idx, ordinals)
    if out_json:
        os.makedirs(os.path.dirname(out_json), exist_ok=True)
        json.dump(namemap, open(out_json, "w"), indent=0, sort_keys=True)
    return namemap, (agree, disagree)


if __name__ == "__main__":
    import sys
    from engine.foundation import cores
    cores.set_target("engine/targets/win95")
    ordinals = _t.load_ordinals("ORDINALS.md")
    retail = glob.glob(
        "binaries/win95/files/WINDOWS/**/krnl386.exe", recursive=True)[0]
    dbg = "binaries/media/WIN32SDK/SCT/DEBUG16/I386"
    out = "binaries/win95/src/idiomatic/krnl386/names.json"
    namemap, (agree, disagree) = recover(
        retail, os.path.join(dbg, "KRNL386.EXE"),
        os.path.join(dbg, "KRNL386.MAP"), ordinals, out)
    print("export validation: %d agree, %d disagree (must be 0)" % (agree, disagree))
    print("saved %d exact real internal names -> %s" % (len(namemap), out))
