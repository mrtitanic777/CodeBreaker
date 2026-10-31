"""gate_teeth16.py -- is the 16-bit gate (verify/gate16/isolated16.py) a ruler worth trusting?

    python tools/gate_teeth16.py self   MODULE                 the original against itself
    python tools/gate_teeth16.py teeth  MODULE BUILD [N]       planted bugs in a build's C

self:  every function of the original run against the original itself, through the same
       checker. Whatever it calls different is the checker's own noise: there must be none.
teeth: N functions the build has as C and the gate passes; in a copy of the build, one small
       bug planted in one of them -- a conditional jump turned around, an immediate off by
       one, a memory operand's displacement moved by two -- and the gate run on that function
       alone. A bug it calls "same" is one it cannot see. Every mutant is checked to have
       changed an instruction the function actually runs (else it proves nothing).
MODULE is a file under state/mini/original; BUILD a folder under state/mini/src."""
import os
import random
import re
import shutil
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)
import capstone  # noqa: E402  (before the workers: the disassembler's DLL)

from codebreaker.formats.ne import NEFile  # noqa: E402
from codebreaker.verify.teeth import plant  # noqa: E402  (the harness every lane's self-test runs on)
from codebreaker.verify import verdicts as V  # noqa: E402
from codebreaker.verify.gate16 import isolated16  # noqa: E402

ORIG = os.path.join(ROOT, "state", "mini", "original")
SRC = os.path.join(ROOT, "state", "mini", "src")
_JCC = {0x74: 0x75, 0x75: 0x74, 0x7C: 0x7D, 0x7D: 0x7C, 0x7E: 0x7F, 0x7F: 0x7E,
        0x72: 0x73, 0x73: 0x72, 0x76: 0x77, 0x77: 0x76}


def self_test(module):
    original = os.path.join(ORIG, module)
    tables = isolated16._tables(original)
    folder = tempfile.mkdtemp(prefix="selfgate")
    with open(os.path.join(folder, "m.map"), "w") as f:      # (the original's places, as a map)
        for name, (seg, off) in tables["place"].items():
            f.write("%04x:%04x      _%s\n" % (seg, off, name))
    shutil.copyfile(original, os.path.join(folder, module.lower()))
    names = sorted(n for n in tables["place"] if n in tables["funcs"])
    differ = [(n, w) for n, v, w in isolated16.check(original, folder, names, tables=tables) if v == V.DIVERGENT]
    shutil.rmtree(folder, ignore_errors=True)
    print("%s against itself: %d functions, %d called different" % (module, len(names), len(differ)))
    for n, w in differ[:20]:
        print("   %s  %s" % (n, w[:160]))
    return differ


def _spans(build):
    """{name: (seg, start, end)} of the build's functions, from its map."""
    places = isolated16._built_places(build)
    by_seg = {}
    for n, (s, o) in places.items():
        by_seg.setdefault(s, []).append((o, n))
    out = {}
    for s, lst in by_seg.items():
        lst.sort()
        for k, (o, n) in enumerate(lst):
            out[n] = (s, o, lst[k + 1][0] if k + 1 < len(lst) else o + 0x200)
    return out


def _mutants(code, start, end, rnd):
    """(offset in the segment, new bytes, what) for one planted bug inside start..end."""
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
    md.detail = True                                         # (where the displacement is)
    ins = list(md.disasm(bytes(code[start:end]), start))
    out = []
    for i in ins:
        b = bytearray(i.bytes)
        if b[0] in _JCC and i.size == 2:
            out.append((i.address, bytes([_JCC[b[0]]]) + bytes(b[1:]), "jump turned: %s %s" % (i.mnemonic, i.op_str)))
        elif i.mnemonic in ("cmp", "add", "sub", "mov", "and", "or") and re.search(r", (0x[0-9a-f]+|\d+)$", i.op_str) \
                and i.size >= 3 and "ptr" not in i.op_str.split(",")[1]:
            k = len(b) - 1                                   # (the immediate's low byte: the last but one when wide)
            if i.size >= 4 and b[0] in (0x81, 0xB8, 0xB9, 0xBA, 0xBB, 0xBE, 0xBF, 0x3D, 0x05, 0x2D, 0x25, 0x0D):
                k = len(b) - 2
            b[k] = (b[k] + 1) & 0xFF
            out.append((i.address, bytes(b), "immediate +1: %s %s" % (i.mnemonic, i.op_str)))
        elif re.search(r"\[(bp|bx|si|di)[^\]]*[+-] (0x[0-9a-f]+|\d+)\]", i.op_str) and i.size >= 3:
            d = i.disp_offset
            if d and i.disp_size == 1:
                b[d] = (b[d] + 2) & 0xFF
                out.append((i.address, bytes(b), "displacement +2: %s %s" % (i.mnemonic, i.op_str)))
    rnd.shuffle(out)
    return out


def _executed(original, build, image, tables, names):
    """{name: {offsets in its segment of the instructions the gate's inputs run}} in the build."""
    from codebreaker.emulate import windows16 as emu
    from codebreaker.verify.lanes import win16
    funcs = {n: tuple(v) for n, v in tables["funcs"].items()}
    place = {n: tuple(v) for n, v in tables["place"].items()}
    ours = isolated16._built_places(build)
    callees = {ours[n]: (n, pop, far) for n, (pop, far) in funcs.items() if n in ours}
    data = emu.relocated_data(original)
    nef = NEFile(original)
    auto = nef.h.get("autodata_seg")
    seg = next((s for s in nef.segments() if s["idx"] == auto), None)
    data_end = (seg["length"], max(seg["length"], seg.get("minalloc") or 0)) if seg else None
    out = {}
    for name in names:
        pop, far = funcs[name]
        oseg, ooff = place[name]
        later = [o for (sg, o) in place.values() if sg == oseg and o > ooff]
        aimed = win16.directed(original, oseg, (ooff, min(later) if later else 0x10000), most=12)
        hit = set()
        for v in win16.vectors() + aimed:
            try:
                r = emu.probe(image, seg=ours[name][0], offset=ours[name][1], args=v["args"], near=not far,
                              callees=callees, insn_cap=20000, data_image=data, data_end=data_end)
            except Exception:
                continue
            base = r.get("code_base") or 0
            hit |= {a - base for a in r.get("path") or () if 0 <= a - base < 0x10000}
        out[name] = hit
    return out


def teeth(module, build, n=40, seed=1):
    original = os.path.join(ORIG, module)
    build = os.path.join(SRC, build)
    image = next(os.path.join(build, f) for f in os.listdir(build) if f.lower() == module.lower())
    tables = isolated16._tables(original)
    spans = _spans(build)
    names = [x for x in sorted(spans) if x in tables["funcs"] and x in tables["place"]]
    # (the ones the build has as C and the gate passes as they are)
    mapf = open(os.path.join(build, "m.map"), errors="replace").read()
    c_names = set()
    module_of = None
    for line in mapf.splitlines():
        mm = re.match(r"Module: (\S+)\(", line)
        if mm:
            module_of = mm.group(1)
            continue
        mm = isolated16._MAP.match(line.strip())
        if mm and module_of and not module_of.startswith("asm/"):
            c_names.add(mm.group(3))
    pool = [x for x in names if x in c_names]
    rnd = random.Random(seed)
    rnd.shuffle(pool)
    passing = [x for x, v, _w in isolated16.check(original, build, pool[:n * 2], tables=tables) if v == V.EQUIVALENT][:n]
    nef = NEFile(image)
    segs = {s["idx"]: s for s in nef.segments()}
    raw = open(image, "rb").read()
    work = tempfile.mkdtemp(prefix="teeth")
    ran = _executed(original, build, image, tables, passing)

    def broken():                                            # (a round each: one function, one bug)
        for name in passing:
            s, lo, hi = spans[name]
            seg = segs[s]
            code = raw[seg["file_off"]:seg["file_off"] + seg["length"]]
            # (only where the gate's own inputs go: a bug in code no input reaches proves nothing)
            live = [x for x in _mutants(code, lo, min(hi, len(code)), rnd) if x[0] in ran.get(name, ())]
            for at, new, what in live[:1]:
                m = bytearray(raw)
                m[seg["file_off"] + at:seg["file_off"] + at + len(new)] = new
                mut = os.path.join(work, os.path.basename(image))
                open(mut, "wb").write(bytes(m))
                yield [(name, what, mut)]

    def judge(batch):
        (name, _what, mut), = batch
        got = list(isolated16.check(original, build, [name], image=mut, tables=tables, workers=1))
        return {name: got[0][1] if got else V.INCONCLUSIVE}

    planted = [x for batch in plant(broken(), judge, {V.DIVERGENT: "caught", V.EQUIVALENT: "missed"}.get) for x in batch]
    caught = [(name, what) for name, what, _m, _v, got in planted if got == "caught"]
    missed = [(name, what) for name, what, _m, _v, got in planted if got == "missed"]
    unrun = [(name, what) for name, what, _m, _v, got in planted if got is None]
    shutil.rmtree(work, ignore_errors=True)
    total = len(caught) + len(missed)
    print("%s: %d planted bugs, caught %d, missed %d (%.0f%%), not run %d" % (
        module, total, len(caught), len(missed), 100.0 * len(caught) / max(total, 1), len(unrun)))
    for name, what in missed[:25]:
        print("   missed  %s  %s" % (name, what))
    return caught, missed


if __name__ == "__main__":
    if sys.argv[1] == "self":
        self_test(sys.argv[2])
    else:
        teeth(sys.argv[2], sys.argv[3], int(sys.argv[4]) if len(sys.argv) > 4 else 40)
