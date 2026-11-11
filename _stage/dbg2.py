import sys, os
sys.path.insert(0, r'C:\Users\mrtit\Documents\GitHub\CodeBreaker\state\work\typed-port\_ne_common')
import common as C
sys.path.insert(0, r'C:\Users\mrtit\Documents\GitHub\CodeBreaker\state\work\typed-port\_vxd_common')
from codebreaker.formats import le
from codebreaker.source.vxd import disassembly as dis
rows = []
for p in le.corpus_files():
    for i, (n, v) in enumerate(le.members(p)):
        rows.append((len(v.d), p, i, n, v))
rows.sort(key=lambda r: r[0])
print(len(rows))
for sz, p, i, n, v in rows[:25]:
    D = dis.VxDDisasm(v).run()
    print(sz, os.path.basename(p), i, n, "ddb" if D.ddb else "NO-DDB", v.pagesize, len(v.objects))
nod = 0
for sz, p, i, n, v in rows:
    if sz < 120000:
        D = dis.VxDDisasm(v).run()
        if not D.ddb:
            nod += 1
            print("noddb", sz, os.path.basename(p), i, n)
            if nod > 6:
                break
