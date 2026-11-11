import sys
sys.path.insert(0, r"C:\Users\mrtit\Documents\GitHub\CodeBreaker\.claude\worktrees\agent-ae870db3b374ffca8")
import importlib
b = importlib.import_module("codebreaker.source.vxd.boot")
doc = b.__doc__
assert ')BOOTDOC"' not in doc
open(r"C:\Users\mrtit\Documents\GitHub\CodeBreaker\.claude\worktrees\agent-ae870db3b374ffca8\_stage\boot_doc.inc", "w", encoding="utf-8", newline="\n").write('R"BOOTDOC(' + doc + ')BOOTDOC"')
print(len(doc), doc.count("\\"))
