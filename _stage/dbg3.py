import sys, os
sys.path.insert(0, r'C:\Users\mrtit\Documents\GitHub\CodeBreaker\state\work\typed-port\_ne_common')
import vxdcommon as X
from codebreaker.base import settings
from codebreaker.formats import le
from codebreaker.source.vxd import disassembly as dis, masm as vxd_masm, tree as T
WASM = os.path.join(os.path.dirname(settings.get("openwatcom") or ""), "bld", "wasm", "nt386", "wasm.exe")
p = r"C:\Users\mrtit\Documents\GitHub\Windows95\design\corpus\files\WINDOWS\SYSTEM\splitter.vxd"
v = le.members(p)[0][1]
D = dis.VxDDisasm(v).run()
D.v = v
oi = 1
o = v.objects[0]
cls, seg = T.seg_class(o["flags"])
head = ["; x.asm -- splitter.vxd object 1 segment %s" % seg, ".386p", "%s segment dword public use32 '%s'" % (seg, cls), ""]
tail = ["%s ends" % seg, "        end", ""]
db = D.ddb
extra = {db["rva"]: "VXD_DDB"} if db and db["obj"] == oi else {}
probe = T._wasm_probe(WASM)
body, linemap = vxd_masm.object_body(D, oi, force_raw=set(), extra_labels=extra, base_line=len(head), crange=(0, len(D.code[oi])))
open("dbg3_a.asm", "w", newline="").write("\r\n".join(head + body + tail))
rc, err, rx = probe("dbg3_a.asm")
print("iter1 rc", rc, err[:300].replace("\n", " | "))
lines = {int(m.group(1)) for m in rx.finditer(err)}
new = {linemap[L] for L in lines if L in linemap}
print("new", len(new), "lines", sorted(lines)[:5])
alloff = {a for (oo, a) in D.insns if oo == oi}
body_raw, _ = vxd_masm.object_body(D, oi, force_raw=(new | alloff), extra_labels=extra, base_line=len(head), crange=(0, len(D.code[oi])))
open("dbg3_b.asm", "w", newline="").write("\r\n".join(head + body_raw + tail))
rc2, err2, _ = probe("dbg3_b.asm")
print("raw rc", rc2, err2[:300].replace("\n", " | "), len(body_raw), len(body))
