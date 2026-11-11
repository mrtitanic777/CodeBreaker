import sys, os
sys.path.insert(0, r'C:\Users\mrtit\Documents\GitHub\CodeBreaker\state\work\typed-port\_ne_common')
import common as C
from codebreaker.formats import ne
from codebreaker.source.ne_asm import idiom as I, segment as trace
ORD = trace.load_ordinals(r"C:\Users\mrtit\Documents\GitHub\CodeBreaker\state\work\typed-port\source.ne_asm.idiom\ORDINALS.md")
s = trace.Seg(ne.NEFile(C.MINI + "NOMOUSE.DRV"), 1, ORD)
text, _ = I._emit(s, 0, len(s.code))
print(text[:500])
r = I.assemble_nasm(text)
print("obj", None if r["obj"] is None else len(r["obj"]))
print("LOG", r["log"][:900])
print("LST", r["lst"][:200])
print(I._nasm_bin(), os.path.exists(I._nasm_bin()))
