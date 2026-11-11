import sys, os, hashlib
sys.path.insert(0, r'C:\Users\mrtit\Documents\GitHub\CodeBreaker\state\work\typed-port\_ne_common')
import common as C
from codebreaker.formats import ne
from codebreaker.source.ne_asm import idiom as I, segment as trace
ORD = trace.load_ordinals(r"C:\Users\mrtit\Documents\GitHub\CodeBreaker\state\work\typed-port\source.ne_asm.idiom\ORDINALS.md")
for path, si, hi in ((C.MINI + "NOMOUSE.DRV", 1, 45), (C.MINI + "KEYBOARD.DRV", 7, 55), (C.MINI + "SOUND.DRV", 1, 600)):
    s = trace.Seg(ne.NEFile(path), si, ORD)
    text, _ = I._emit(s, 0, min(hi, len(s.code)))
    objs = [I.assemble_nasm(text)["obj"] for _ in range(3)]
    print(os.path.basename(path), [hashlib.sha1(o).hexdigest()[:8] for o in objs], len(objs[0]))
    a, b = objs[0], objs[1]
    if a != b:
        d = [i for i in range(len(a)) if a[i] != b[i]]
        print("  differ at", d[:20], a[d[0] - 8:d[0] + 8], b[d[0] - 8:d[0] + 8])
