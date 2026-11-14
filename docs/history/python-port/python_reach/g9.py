"""g9.py: holders of each codebreaker module. usage: g9.py [holders|why MOD]"""
import sys, os
sys.argv=[sys.argv[0]]+sys.argv[1:]
exec(open("docs/history/python-port/python_reach/g7.py").read().split('if __name__')[0])
g,weak,mods=graph(set())
for m in g: g[m]|=weak[m]
rev={}
for m,o in g.items():
    for x in o: rev.setdefault(x,set()).add(m)
def holders(m):
    return sorted(h for h in rev.get(m,()) if h!=m)
if __name__=="__main__":
    if len(sys.argv)>2 and sys.argv[1]=="why":
        for h in holders(sys.argv[2]): print(h)
    else:
        keep={"codebreaker","codebreaker.ui","codebreaker.ui.client","codebreaker.ui.terminal"}
        for m in sorted(mods):
            if not m.startswith("codebreaker") or m in keep: continue
            hs=holders(m)
            t=sorted({h.split(".")[1] if h.startswith("tests.") else "" for h in hs if h.startswith("tests.")})
            tl=sorted({h for h in hs if h.startswith("tools.")})
            cb=[h for h in hs if h.startswith("codebreaker")]
            lines=sum(1 for _ in open(files[m],encoding="utf-8",errors="replace"))
            print(m,lines,"tests=",t,"tools=",tl,"cb=",len(cb))
