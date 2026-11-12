"""Regenerates testdata/fixes/*.golden.cpp from testdata/fixes/*.in.cpp with the Python fix passes
(codebreaker.java.projects.minecraft.{applet_ping, sound_fix, render_fix}).

The goldens are REVIEWED after generation, not trusted: the only intended difference from what the Python writes is the
generator tag in the inserted comments ("[generator: sound_fix.py]" -> "[generator: sound_fix]"), which this script
applies. Run from the repository root:  python native/cb/java/projects/minecraft/testdata/gen_fixes_golden.py
"""
import os
import sys

sys.path.insert(0, os.getcwd())
from codebreaker.java.projects.minecraft import applet_ping, render_fix, sound_fix  # noqa: E402

HERE = os.path.dirname(os.path.abspath(__file__))
FIX = os.path.join(HERE, "fixes")


def tag(text):
    for name in ("applet_ping", "sound_fix", "render_fix"):
        text = text.replace("[generator: %s.py]" % name, "[generator: %s]" % name)
        text = text.replace("[%s.py]" % name, "[%s]" % name)
    return text


def read(name):
    with open(os.path.join(FIX, name), "r", encoding="utf-8", newline="") as f:
        return f.read()


def write(name, text):
    with open(os.path.join(FIX, name), "w", encoding="utf-8", newline="") as f:
        f.write(text)


for case in sorted(f for f in os.listdir(FIX) if f.endswith(".in.cpp")):
    base = case[: -len(".in.cpp")]
    text = read(case)
    if base.startswith("applet"):
        out, _n = applet_ping.process(text)
    elif base.startswith("sound"):
        out, _s, _m = sound_fix.process(text)
    else:
        out, _u, _h = render_fix.process(text)
    write(base + ".golden.cpp", tag(out))
    print(base, "->", len(out))
