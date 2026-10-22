"""The 6502 SLEIGH specification checked against CodeBreaker's 6502 interpreter, instruction by
instruction -- tools/sleigh_verify.py's check, for the specification the NES lane decompiles with
(languages/6502.sla, 6502:LE:16:default).

    python tools/sleigh_verify_6502.py [--states 24] [--report FILE]

No emulator of a real 6502 is at hand (Unicorn has none), so the reference is the interpreter
(emulate/m6502.py), written from the chip's documented behaviour; a disagreement says one of
the two is wrong, and the report says where. Every documented opcode runs from --states random
machine states, through the interpreter and through the specification's p-code
(native/decompiler/build/bin/pcodecheck, mode 6502), and what each left is compared: A X Y, the
stack pointer's low byte, the flags N V D I Z C (B is no register: it exists only in what PHP
and BRK push), the bytes of memory written, and where execution goes next. The operands point
into a fixed pattern of memory, given to both sides alike.
"""
import collections
import os
import random
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)

from codebreaker.decompile import m6502 as isa      # noqa: E402
from codebreaker.emulate import m6502 as cpu6502    # noqa: E402

TOOL = os.path.join(ROOT, "native", "decompiler", "build", "bin", "pcodecheck" + (".exe" if os.name == "nt" else ""))
SLA = os.path.join(ROOT, "native", "decompiler", "languages", "6502.sla")
PC = 0x0400
FLAGS = "NVDIZC"


def _states(op, n, rnd):
    for k in range(n):
        st = {"A": rnd.randrange(256), "X": rnd.randrange(256), "Y": rnd.randrange(256),
              "S": rnd.choice([0xFD, 0xF0, 0x80, rnd.randrange(4, 252)]),
              "P": rnd.randrange(256) & ~0x08 | 0x20}       # (decimal off: the 2A03's case)
        if k % 6 == 0:
            st["A"], st["X"] = 0x7F, 0xFF                    # overflow and wrap edges
        b1, b2 = rnd.randrange(256), rnd.randrange(256)
        if k % 5 == 1:
            b1 = 0xFF                                        # zero-page and page-end wraps
        if isa.OPCODES[op][1] == "rel" and b1 == 0xFF:
            b1 = 0xFE                                        # (not a branch into its own operand)
        st["code"] = bytes([op, b1, b2])
        yield st


def _memory(st):
    mem = bytearray((i * 37 + 11) & 0xFF for i in range(0x10000))
    mem[0x1FE], mem[0x1FF] = 0x34, 0x12
    size = isa.SIZE[isa.OPCODES[st["code"][0]][1]]
    mem[PC:PC + 3] = st["code"][:size] + b"\xea" * (3 - size)     # (what follows decodes as NOP)
    return mem


def interpret(st):
    mem = _memory(st)
    before = bytes(mem)
    c = cpu6502.CPU(mem)
    c.a, c.x, c.y, c.s, c.p, c.pc = st["A"], st["X"], st["Y"], st["S"], st["P"], PC
    c.step()
    writes = {a: mem[a] for a in range(0x10000) if mem[a] != before[a]}
    return {"PC": c.pc, "A": c.a, "X": c.x, "Y": c.y, "S": c.s,
            **{f: c.flags[f] for f in FLAGS}, "W": writes}


def pcode(cases):
    lines = []
    for i, st in enumerate(cases):
        mem = _memory(st)
        p = st["P"]
        regs = "A=%x X=%x Y=%x SP=%x N=%d V=%d B=%d D=%d I=%d Z=%d C=%d IP=%x" % (
            st["A"], st["X"], st["Y"], 0x100 | st["S"], p >> 7 & 1, p >> 6 & 1, p >> 4 & 1, p >> 3 & 1,
            p >> 2 & 1, p >> 1 & 1, p & 1, PC)
        # all of memory but the stack page's pattern is reachable by some operand; give it in
        # one block (the p-code side reads zero where nothing is given)
        lines.append("c%d %s M0=%s" % (i, regs, mem.hex()))
    p = subprocess.run([TOOL, SLA, "6502"], input="\n".join(lines) + "\n", capture_output=True, text=True)
    out = {}
    for line in p.stdout.splitlines():
        parts = line.split()
        if len(parts) < 2:
            continue
        i = int(parts[0][1:])
        if parts[1] != "ok":
            out[i] = {"error": " ".join(parts[1:])}
            continue
        r = {"W": {}}
        for tok in parts[2:]:
            k, _, v = tok.partition("=")
            if k.startswith("W"):
                r["W"][int(k[1:], 16)] = int(v, 16)
            elif k == "LIN":
                r["PC"] = int(v, 16) & 0xFFFF
            elif k == "SP":
                r["S"] = int(v, 16) & 0xFF
            else:
                r[k] = int(v, 16)
        out[i] = r
    return out


def main(argv):
    n = int(argv[argv.index("--states") + 1]) if "--states" in argv else 24
    report = argv[argv.index("--report") + 1] if "--report" in argv else None
    if not os.path.isfile(TOOL):
        print("pcodecheck is not built (cmake --build native/decompiler/build --config Release --target pcodecheck)")
        return 2
    rnd = random.Random(6502)
    cases = [st for op in sorted(isa.OPCODES) for st in _states(op, n, rnd)]
    got = pcode(cases)
    per_form = collections.OrderedDict()
    for i, st in enumerate(cases):
        m, mode = isa.OPCODES[st["code"][0]]
        form = "%s %s" % (m, mode)
        mine, theirs = interpret(st), got.get(i, {"error": "no answer"})
        if "error" in theirs:
            diff = [theirs["error"]]
        else:
            diff = [k for k in ("PC", "A", "X", "Y", "S") + tuple(FLAGS) if mine[k] != theirs.get(k)]
            if mine["W"] != theirs["W"]:
                diff.append("memory")
        rec = per_form.setdefault(form, {"cases": 0, "agree": 0, "differ": collections.Counter(), "example": None})
        rec["cases"] += 1
        if diff:
            rec["differ"].update(diff)
            if rec["example"] is None:
                rec["example"] = (st["code"].hex(), {k: mine.get(k) for k in diff if k in mine},
                                  {k: theirs.get(k) for k in diff if k in theirs})
        else:
            rec["agree"] += 1
    lines, bad = [], 0
    for form, rec in per_form.items():
        if rec["agree"] != rec["cases"]:
            bad += 1
            lines.append("%-10s %d/%d agree; differ in %s; e.g. %s interpreter %s, p-code %s" % (
                form, rec["agree"], rec["cases"], dict(rec["differ"]), *rec["example"]))
    lines.append("%d of %d forms agree in every case" % (len(per_form) - bad, len(per_form)))
    text = "\n".join(lines)
    print(text)
    if report:
        with open(report, "w", encoding="utf-8") as f:
            f.write(text + "\n")
    return 0 if not bad else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
