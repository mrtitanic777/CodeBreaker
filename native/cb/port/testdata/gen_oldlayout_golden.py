"""Writes testdata/oldlayout/expected/: what the Python's port/oldlayout.py made of prog.c and prog.h (clang from Visual
Studio, take()'s first parameter a pointer), read through by hand; the C++ pass (cb/port/oldlayout) is held to it.

    python native/cb/port/testdata/gen_oldlayout_golden.py
"""
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__)).replace("\\", "/")
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", "..", ".."))
sys.path.insert(0, ROOT)
from codebreaker.port import oldlayout  # noqa: E402

CLANG = "C:/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/Llvm/x64/bin/clang.exe"
folder = HERE + "/oldlayout"
os.makedirs(folder + "/expected", exist_ok=True)
oldlayout.PTR_PARAMS.clear()
oldlayout.PTR_PARAMS["take"] = [True, False]
oldlayout.PTR_PARAMS["later"] = [True]
out, olds, found, refused = oldlayout.run(CLANG, folder, ["prog.c"])
for name in ("prog.c", "prog.h"):
    text = out.get(name) or open(folder + "/" + name, encoding="latin1", newline="").read()
    text = oldlayout.declare(text, olds, found)
    if name.endswith(".c"):
        text = oldlayout.null_reads(text)
    with open(folder + "/expected/" + name, "w", encoding="latin1", newline="") as f:
        f.write(text)
with open(folder + "/expected/prog.found", "w", newline="") as f:
    f.write("olds " + " | ".join(sorted(olds)) + "\n")
    for (rec, member), t in sorted(found.items()):
        f.write("found %s | %s | %s\n" % (rec, member, t))
    for name, at, op in refused:
        f.write("refused %s %d %s\n" % (name, at, op))
