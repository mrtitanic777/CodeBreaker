"""Writes floating.golden.txt: what codebreaker/recompile/builders/floating.py emits for every mnemonic, as the
reference the C++ (cb/recompile/builders/floating.cpp) is checked against. Run from the checkout root:
    python native/cb/recompile/testdata/gen_floating_golden.py
Operands: f3 f4 f5 f6 for the floating-point registers, cr6 for the condition field."""
import os
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "..", ".."))
from codebreaker.recompile.builders import floating


def num(tok):
    return int(tok[2:]) if tok.startswith("cr") else int(tok[1:])


out = []
for mn in floating.MNEMONICS:
    ops = ["cr6", "f4", "f5"] if mn in ("fcmpu", "fcmpo") else ["f3", "f4", "f5", "f6"]
    lines = floating.emit(mn, ops, None, None, num, None, num, None, None, None, None, 0x82000000, 4)
    out.append("## " + mn + " " + " ".join(ops))
    out.extend(lines)
assert floating.emit("add", ["r3", "r4", "r5"], None, None, num, None, num, None, None, None, None, 0, 4) is None
out.append("## helpers")
out.extend(floating.RUNTIME_HELPERS.splitlines())
path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "floating.golden.txt")
with open(path, "w", newline="\n") as f:
    f.write("\n".join(out) + "\n")
print(len(floating.MNEMONICS), "mnemonics ->", path)
