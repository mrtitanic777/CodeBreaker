#!/usr/bin/env python3
r"""server.py -- the git-style visual interface for CodeBreaker Diff (`wt diff ui`).

The corpus IS a version-control history of Windows itself: each release is a
commit, each module a directory, each function a file, and a cross-version match
is the same file's line of descent. This serves that view -- a browsable commit
rail (win101 -> win310 -> win95_gold), a function browser per release, a
function's lineage across releases, and a git-style side-by-side diff of two
functions' normalized instruction streams.

Stdlib only (http.server), matching ledger/dash.py. Self-contained page, no CDN.

    wt diff ui [--port 8790] [--db PATH] [--no-open]
"""
import json
import os
import sys
import webbrowser
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from urllib.parse import urlparse, parse_qs

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import corpus
import matcher
import extractor
try:
    import difflib
except ImportError:                     # pragma: no cover
    difflib = None

DB = corpus.DEFAULT_DB


def _conn():
    return corpus.open_db(DB)


# --------------------------------------------------------------------------- API
def api_overview():
    c = _conn()
    out = []
    for vid in ("win101", "win310", "win95_gold"):
        v = corpus.VERSIONS.get(vid, {})
        row = c.execute(
            "SELECT COUNT(*) n, SUM(CASE WHEN symbol_name NOT LIKE 'sub_%' THEN 1 ELSE 0 END)"
            " named, COUNT(DISTINCT module_name) mods FROM functions WHERE version_id=?",
            (vid,)).fetchone()
        out.append({"id": vid, "arch": v.get("architecture"), "date": v.get("release_date"),
                    "functions": row["n"] or 0, "named": row["named"] or 0,
                    "modules": row["mods"] or 0})
    return {"versions": out}


def api_modules(q):
    c = _conn()
    rows = c.execute(
        "SELECT module_name, group_name, COUNT(*) n,"
        " SUM(CASE WHEN symbol_name NOT LIKE 'sub_%' THEN 1 ELSE 0 END) named"
        " FROM functions WHERE version_id=? GROUP BY module_name, group_name"
        " ORDER BY module_name", (q["version"][0],)).fetchall()
    return {"modules": [dict(r) for r in rows]}


def api_functions(q):
    c = _conn()
    ver = q["version"][0]
    mod = q.get("module", [None])[0]
    term = (q.get("q", [""])[0] or "").strip().lower()
    limit = int(q.get("limit", ["400"])[0])
    sql = ("SELECT function_id, symbol_name, segment_offset, module_name, insn_count,"
           " cfg_node_count, cyclomatic_complexity, call_count FROM functions"
           " WHERE version_id=?")
    args = [ver]
    if mod:
        sql += " AND module_name=?"; args.append(mod)
    if term:
        sql += " AND (LOWER(symbol_name) LIKE ? OR LOWER(segment_offset) LIKE ?)"
        args += ["%" + term + "%", "%" + term + "%"]
    sql += " ORDER BY (symbol_name LIKE 'sub_%'), symbol_name LIMIT ?"
    args.append(limit)
    return {"functions": [dict(r) for r in c.execute(sql, args).fetchall()]}


def api_function(q):
    c = _conn()
    r = c.execute("SELECT * FROM functions WHERE function_id=?", (q["fid"][0],)).fetchone()
    if not r:
        return {"error": "not found"}
    return {"function": dict(r), "lineage": matcher.lineage(c, q["fid"][0])}


def _resolve_src(r):
    """Absolute path to a function's .asm, resilient to the corpus being reorganised
    after ingest (the stored src_path can go stale when a group dir is renamed).
    Falls back to locating <module>/<basename> under the version's asm root."""
    import glob
    p = os.path.join(corpus.REPO, r["src_path"])
    if os.path.exists(p):
        return p
    base = os.path.basename(r["src_path"])
    root = os.path.join(corpus.REPO, corpus.VERSIONS.get(r["version_id"], {})
                        .get("asm_root", ""))
    hits = glob.glob(os.path.join(root, "**", r["module_name"], base), recursive=True)
    return hits[0] if hits else p


def _stream(fid, c):
    r = c.execute("SELECT * FROM functions WHERE function_id=?", (fid,)).fetchone()
    if not r:
        return None, []
    try:
        view = extractor.instruction_view(_resolve_src(r))
    except Exception:
        view = []
    return dict(r), view


def api_diff(q):
    c = _conn()
    ameta, aview = _stream(q["a"][0], c)
    bmeta, bview = _stream(q["b"][0], c)
    if ameta is None or bmeta is None:
        return {"error": "not found"}
    an = [v[1] for v in aview]
    bn = [v[1] for v in bview]
    rows = []
    if difflib:
        sm = difflib.SequenceMatcher(a=an, b=bn, autojunk=False)
        for op, i1, i2, j1, j2 in sm.get_opcodes():
            if op == "equal":
                for k in range(i2 - i1):
                    rows.append({"op": "same", "a": aview[i1 + k][0], "b": bview[j1 + k][0]})
            elif op == "replace":
                for k in range(max(i2 - i1, j2 - j1)):
                    rows.append({"op": "chg",
                                 "a": aview[i1 + k][0] if i1 + k < i2 else "",
                                 "b": bview[j1 + k][0] if j1 + k < j2 else ""})
            elif op == "delete":
                for k in range(i1, i2):
                    rows.append({"op": "del", "a": aview[k][0], "b": ""})
            elif op == "insert":
                for k in range(j1, j2):
                    rows.append({"op": "ins", "a": "", "b": bview[k][0]})
        ratio = round(sm.ratio(), 3)
    else:
        ratio = None
    def meta(m):
        return {"name": m["symbol_name"], "version": m["version_id"], "module": m["module_name"],
                "off": m["segment_offset"], "insn": m["insn_count"], "nodes": m["cfg_node_count"],
                "cyclo": m["cyclomatic_complexity"], "calls": m["call_count"], "bits": m["bits"]}
    return {"a": meta(ameta), "b": meta(bmeta), "ratio": ratio, "rows": rows}


ROUTES = {"/api/overview": lambda q: api_overview(), "/api/modules": api_modules,
          "/api/functions": api_functions, "/api/function": api_function,
          "/api/diff": api_diff}


class Handler(BaseHTTPRequestHandler):
    def log_message(self, *a):
        pass

    def _send(self, body, ctype="application/json"):
        b = body.encode("utf-8") if isinstance(body, str) else body
        self.send_response(200)
        self.send_header("Content-Type", ctype)
        self.send_header("Content-Length", str(len(b)))
        self.end_headers()
        self.wfile.write(b)

    def do_GET(self):
        u = urlparse(self.path)
        if u.path in ("/", "/index.html"):
            return self._send(PAGE, "text/html; charset=utf-8")
        fn = ROUTES.get(u.path)
        if fn:
            try:
                return self._send(json.dumps(fn(parse_qs(u.query))))
            except Exception as e:
                return self._send(json.dumps({"error": str(e)}))
        self.send_response(404); self.end_headers()


def serve(port=8790, open_browser=True):
    srv = ThreadingHTTPServer(("127.0.0.1", port), Handler)
    url = "http://localhost:%d" % port
    print("CodeBreaker Diff UI  ->  %s   (Ctrl-C to stop)" % url)
    if open_browser:
        try:
            webbrowser.open(url)
        except Exception:
            pass
    try:
        srv.serve_forever()
    except KeyboardInterrupt:
        print("\nstopped.")


PAGE = r"""<!doctype html><html><head><meta charset="utf-8">
<title>CodeBreaker Diff — Windows lineage</title>
<style>
:root{--bg:#0d1117;--panel:#161b22;--panel2:#0d1117;--line:#30363d;--fg:#c9d1d9;
--dim:#8b949e;--acc:#58a6ff;--green:#3fb950;--red:#f85149;--yellow:#d29922;--purple:#bc8cff}
*{box-sizing:border-box}
body{margin:0;font:13px/1.5 "Segoe UI",system-ui,sans-serif;background:var(--bg);color:var(--fg)}
code,.mono{font-family:"Cascadia Code",Consolas,"SF Mono",monospace}
header{padding:10px 16px;border-bottom:1px solid var(--line);display:flex;align-items:center;gap:12px}
header h1{font-size:15px;margin:0;font-weight:600}
header .sub{color:var(--dim);font-size:12px}
.app{display:grid;grid-template-columns:250px 320px 1fr;height:calc(100vh - 46px)}
.pane{overflow:auto;border-right:1px solid var(--line)}
.pane h2{font-size:11px;text-transform:uppercase;letter-spacing:.06em;color:var(--dim);
margin:0;padding:10px 14px;border-bottom:1px solid var(--line);position:sticky;top:0;background:var(--bg)}
/* commit rail */
.commit{padding:12px 14px;border-bottom:1px solid var(--line);cursor:pointer;position:relative}
.commit:hover{background:var(--panel)}
.commit.sel{background:var(--panel);border-left:3px solid var(--acc)}
.commit .rail{position:absolute;left:20px;top:0;bottom:0;width:2px;background:var(--line)}
.commit .dot{position:absolute;left:15px;top:16px;width:12px;height:12px;border-radius:50%;
background:var(--acc);border:2px solid var(--bg);z-index:1}
.commit .body{margin-left:26px}
.commit .id{font-weight:600;color:var(--acc)}
.commit .meta{color:var(--dim);font-size:11px}
.commit .bar{height:5px;border-radius:3px;background:var(--line);margin-top:6px;overflow:hidden}
.commit .bar span{display:block;height:100%;background:var(--green)}
/* lists */
.row{padding:6px 14px;border-bottom:1px solid #1c2128;cursor:pointer;display:flex;
justify-content:space-between;gap:8px;align-items:center}
.row:hover{background:var(--panel)}
.row.sel{background:#1f2937}
.row .nm{overflow:hidden;text-overflow:ellipsis;white-space:nowrap}
.row .nm.named{color:var(--purple)}
.row .rt{color:var(--dim);font-size:11px;white-space:nowrap}
.pill{font-size:10px;padding:1px 6px;border-radius:10px;border:1px solid var(--line);color:var(--dim)}
.search{width:100%;padding:7px 12px;background:var(--panel2);border:none;border-bottom:1px solid var(--line);
color:var(--fg);outline:none}
/* detail */
.detail{padding:16px}
.detail .fname{font-size:16px;font-weight:600}
.detail .fname.named{color:var(--purple)}
.stats{display:flex;gap:14px;flex-wrap:wrap;margin:12px 0}
.stat{background:var(--panel);border:1px solid var(--line);border-radius:6px;padding:8px 12px;min-width:70px}
.stat b{display:block;font-size:18px}.stat span{color:var(--dim);font-size:11px}
.lineage{margin-top:10px}
.lin{display:flex;align-items:center;gap:10px;padding:8px 10px;border:1px solid var(--line);
border-radius:6px;margin-bottom:8px;cursor:pointer;background:var(--panel)}
.lin:hover{border-color:var(--acc)}
.lin .v{font-weight:600;color:var(--acc);min-width:88px}
.score{font-weight:700;padding:2px 8px;border-radius:10px;font-size:12px}
.mt{font-size:10px;padding:1px 6px;border-radius:4px;border:1px solid var(--line)}
.mt.EXACT{color:var(--green);border-color:var(--green)}
.mt.CFG_OPCODE{color:var(--yellow);border-color:var(--yellow)}
.mt.CALL_GRAPH{color:var(--acc)}.mt.WEAK{color:var(--dim)}
/* diff */
.diff{margin-top:16px;border:1px solid var(--line);border-radius:8px;overflow:hidden}
.diff .dh{display:flex;background:var(--panel);border-bottom:1px solid var(--line);font-size:12px}
.diff .dh div{flex:1;padding:8px 12px}.diff .dh .r{border-left:1px solid var(--line)}
.dtable{width:100%;border-collapse:collapse;font-size:12px}
.dtable td{padding:1px 10px;vertical-align:top;white-space:pre;width:50%;border-left:1px solid #1c2128}
.dtable tr.same td{color:var(--fg)}
.dtable tr.del td:first-child{background:rgba(248,81,73,.14)}
.dtable tr.ins td:last-child{background:rgba(63,185,80,.14)}
.dtable tr.chg td:first-child{background:rgba(248,81,73,.10)}
.dtable tr.chg td:last-child{background:rgba(63,185,80,.10)}
.empty{color:var(--dim);padding:30px;text-align:center}
.hint{color:var(--dim);font-size:12px;margin-top:6px}
.badgebar{display:flex;gap:8px;margin-left:auto;font-size:11px;color:var(--dim)}
</style></head><body>
<header><h1>CodeBreaker Diff</h1><span class="sub" id="sub">a version-control history of Windows 1.0 → 95, by function fingerprint</span>
<div class="badgebar" id="hdrstats"></div></header>
<div class="app">
  <div class="pane" id="rail"><h2>Releases · commit graph</h2><div id="commits"></div></div>
  <div class="pane" id="fns"><h2 id="fnhdr">Functions</h2>
     <input class="search" id="search" placeholder="filter by name or seg:offset…">
     <div id="fnlist"><div class="empty">pick a release</div></div></div>
  <div class="pane" id="detail" style="border-right:none"><h2>Function · lineage · diff</h2>
     <div id="detailbody"><div class="empty">pick a function to see its cross-version lineage</div></div></div>
</div>
<script>
const $=s=>document.querySelector(s), api=(p,q)=>fetch(p+(q?'?'+new URLSearchParams(q):'')).then(r=>r.json());
let state={version:null,module:null,fid:null};
const scoreColor=s=>s>=0.85?'#3fb950':s>=0.7?'#d29922':s>=0.5?'#8b949e':'#484f58';
const named=n=>n&&!n.startsWith('sub_');

async function boot(){
  const o=await api('/api/overview');
  const prev={}; let html=''; const max=Math.max(...o.versions.map(v=>v.functions),1);
  o.versions.forEach((v,i)=>{
    const pct=v.functions?Math.round(100*v.named/v.functions):0;
    html+=`<div class="commit" data-v="${v.id}">${i<o.versions.length?'<div class=rail></div>':''}
      <div class="dot"></div><div class="body">
      <div class="id">${v.id}</div>
      <div class="meta">${v.arch} · ${v.date||''}</div>
      <div class="meta">${v.functions.toLocaleString()} functions · ${v.modules} modules</div>
      <div class="meta">${v.named.toLocaleString()} named (${pct}%)</div>
      <div class="bar"><span style="width:${Math.round(100*v.functions/max)}%"></span></div>
      </div></div>`;
  });
  $('#commits').innerHTML=html;
  const tot=o.versions.reduce((a,v)=>a+v.functions,0), nm=o.versions.reduce((a,v)=>a+v.named,0);
  $('#hdrstats').innerHTML=`<span>${tot.toLocaleString()} functions</span><span>${nm.toLocaleString()} named</span><span>${o.versions.length} releases</span>`;
  document.querySelectorAll('.commit').forEach(el=>el.onclick=()=>selectVersion(el.dataset.v,el));
}
async function selectVersion(v,el){
  state.version=v;state.module=null;
  document.querySelectorAll('.commit').forEach(e=>e.classList.remove('sel'));el.classList.add('sel');
  $('#fnhdr').textContent='Functions · '+v;
  loadFns();
}
async function loadFns(){
  const q={version:state.version}; const t=$('#search').value.trim(); if(t)q.q=t;
  const d=await api('/api/functions',q);
  if(!d.functions.length){$('#fnlist').innerHTML='<div class=empty>no functions (ingest this release)</div>';return;}
  $('#fnlist').innerHTML=d.functions.map(f=>`<div class="row" data-f="${f.function_id}">
     <span class="nm ${named(f.symbol_name)?'named':''}">${f.symbol_name}</span>
     <span class="rt">${f.segment_offset} · ${f.insn_count}i · cc${f.cyclomatic_complexity}</span></div>`).join('');
  document.querySelectorAll('#fnlist .row').forEach(el=>el.onclick=()=>selectFn(el.dataset.f,el));
}
let searchTimer; $('#search').oninput=()=>{clearTimeout(searchTimer);searchTimer=setTimeout(loadFns,180)};
async function selectFn(fid,el){
  state.fid=fid;
  document.querySelectorAll('#fnlist .row').forEach(e=>e.classList.remove('sel'));if(el)el.classList.add('sel');
  const d=await api('/api/function',{fid});
  const f=d.function;
  let h=`<div class="fname ${named(f.symbol_name)?'named':''}">${f.symbol_name}</div>
    <div class="hint">${f.version_id} · ${f.module_name} · ${f.segment_offset} · ${f.bits}-bit</div>
    <div class="stats">
      <div class="stat"><b>${f.insn_count}</b><span>instructions</span></div>
      <div class="stat"><b>${f.cfg_node_count}</b><span>blocks</span></div>
      <div class="stat"><b>${f.cyclomatic_complexity}</b><span>cyclomatic</span></div>
      <div class="stat"><b>${f.call_count}</b><span>calls</span></div>
    </div>
    <div class="lineage"><h2 style="border:none;padding:0;margin:8px 0">Lineage across releases</h2>`;
  const lin=d.lineage, keys=Object.keys(lin);
  if(!keys.length) h+=`<div class="hint">no cross-version match ≥ 0.30 — this function appears unique to ${f.version_id} (rewritten or new).</div>`;
  keys.forEach(v=>{const m=lin[v];
    h+=`<div class="lin" data-a="${fid}" data-b="${m.function_id}">
       <span class="v">${v}</span>
       <span class="score" style="background:${scoreColor(m.score)}22;color:${scoreColor(m.score)}">${m.score.toFixed(2)}</span>
       <span class="mt ${m.match_type}">${m.match_type}</span>
       <span class="nm ${named(m.symbol_name)?'named':''}">${m.symbol_name}</span>
       <span class="rt" style="margin-left:auto;color:var(--dim)">${m.module_name} ${m.segment_offset}</span></div>`;
  });
  h+=`</div><div id="diffbox"></div>`;
  $('#detailbody').innerHTML=h;
  document.querySelectorAll('.lin').forEach(el=>el.onclick=()=>showDiff(el.dataset.a,el.dataset.b));
}
async function showDiff(a,b){
  $('#diffbox').innerHTML='<div class="hint" style="padding:14px">computing diff…</div>';
  const d=await api('/api/diff',{a,b});
  if(d.error){$('#diffbox').innerHTML='<div class=empty>'+d.error+'</div>';return;}
  const rows=d.rows.map(r=>`<tr class="${r.op}"><td>${esc(r.a)}</td><td>${esc(r.b)}</td></tr>`).join('');
  $('#diffbox').innerHTML=`<div class="diff">
    <div class="dh"><div>${d.a.version} · ${d.a.name} <span class=pill>${d.a.insn}i cc${d.a.cyclo}</span></div>
      <div class="r">${d.b.version} · ${d.b.name} <span class=pill>${d.b.insn}i cc${d.b.cyclo}</span></div></div>
    <div style="max-height:46vh;overflow:auto"><table class="dtable mono"><tbody>${rows}</tbody></table></div>
    </div><div class="hint">normalized-instruction diff · sequence similarity ${d.ratio!=null?d.ratio:'—'} · red = only in left, green = only in right</div>`;
}
const esc=s=>(s||'').replace(/[&<>]/g,c=>({'&':'&amp;','<':'&lt;','>':'&gt;'}[c]));
boot();
</script></body></html>"""
