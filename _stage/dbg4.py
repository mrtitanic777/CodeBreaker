import sys, os
sys.path.insert(0, r'C:\Users\mrtit\Documents\GitHub\CodeBreaker\state\work\typed-port\_ne_common')
import vxdcommon as X
from codebreaker.formats import le
from codebreaker.source.vxd import disassembly as dis, masm as vxd_masm, tree as T
p = r"C:\Users\mrtit\Documents\GitHub\Windows95\design\corpus\files\WINDOWS\SYSTEM\splitter.vxd"
v = le.members(p)[0][1]
D = dis.VxDDisasm(v).run()
for o in v.objects:
    oi = o["idx"]
    offs = sorted(a for (oo, a) in D.insns if oo == oi)
    print(oi, len(offs), vxd_masm.split_ranges(D, oi, 40))
import shutil
shutil.rmtree(r"C:\Users\mrtit\Documents\GitHub\CodeBreaker\.claude\worktrees\agent-ae870db3b374ffca8\_stage\t_tree", ignore_errors=True)
print(T.emit_tree(v, "TSPLITTER", r"C:\Users\mrtit\Documents\GitHub\CodeBreaker\.claude\worktrees\agent-ae870db3b374ffca8\_stage\t_tree", 40))
