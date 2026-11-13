"""Drives the Python MCP server (`python -m codebreaker mcp`) and the native one (cb_mcp_stdio) over stdio with the same requests and compares the
answers tool by tool.

    python -B native/cb/ui/testdata/compare_mcp.py --main <main checkout> --cpp <cb_mcp_stdio.exe> --out <folder> [--only tool,tool] [--calls calls.json]

Both servers run against the main checkout's state and corpus (CB_CHECKOUT_ROOT), with CB_SERVICE=0 (each answers in its own process). Prints one line per
call -- identical, or differs and where -- and writes both answers to <out>/python.json and <out>/native.json.

The default calls read lz32.dll, lzexpand.dll and find.exe by corpus name. The files beside this one are other call lists (`--calls`):
    compare_mcp_versions.json   compare_versions and diff_function over the Win95 and Win98 builds
    compare_mcp_jobs.json       run_command and the jobs it starts (give the native server CB_MCP_LAUNCHER=<codebreaker.exe>)
    compare_mcp_naming.json     name_function and the names it gives (give each server a state folder of its own: CB_STATE through
                                --py-env and --cpp-env -- the naming writes the knowledge base)
Differences that are expected: the verifier settings the two make (give the native server --cpp-env CB_MCP_STORED_PROFILES=1 to read the results as
last filed), and the words of `commands` / `command_help` (the native table is cb/app's catalog, the Python's the modules' docstrings).
"""
import argparse
import json
import os
import re
import subprocess
import sys
import time

ap = argparse.ArgumentParser()
ap.add_argument("--main", required=True)
ap.add_argument("--cpp", required=True)
ap.add_argument("--out", required=True)
ap.add_argument("--only", default="")
ap.add_argument("--calls", default="")
ap.add_argument("--timeout", type=float, default=600)
ap.add_argument("--wt", default="")
ap.add_argument("--py-env", action="append", default=[], help="NAME=VALUE for the Python server only")
ap.add_argument("--cpp-env", action="append", default=[], help="NAME=VALUE for the native server only")
args = ap.parse_args()
args.main, args.cpp, args.out = os.path.normpath(args.main), os.path.abspath(args.cpp), os.path.abspath(args.out)   # (backslashes: the paths the servers write are then the same)
os.makedirs(args.out, exist_ok=True)
WT = args.wt or os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))))
DATA = WT.replace("\\", "/") + "/native/cb/formats/testdata"


class Server(object):
    def __init__(self, argv, env, cwd):
        self.p = subprocess.Popen(argv, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.DEVNULL, env=env, cwd=cwd)
        self.n = 0
        self.rpc("initialize", {"protocolVersion": "2025-06-18", "capabilities": {}, "clientInfo": {"name": "t", "version": "1"}})
        self.p.stdin.write((json.dumps({"jsonrpc": "2.0", "method": "notifications/initialized"}) + "\n").encode())
        self.p.stdin.flush()

    def rpc(self, method, params=None):
        self.n += 1
        self.p.stdin.write((json.dumps({"jsonrpc": "2.0", "id": self.n, "method": method, "params": params or {}}) + "\n").encode())
        self.p.stdin.flush()
        line = self.p.stdout.readline()
        return json.loads(line) if line else {"error": "the server ended"}

    def call(self, name, arguments):
        return self.rpc("tools/call", {"name": name, "arguments": arguments})

    def close(self):
        try:
            self.p.stdin.close()
            self.p.wait(timeout=30)
        except Exception:
            self.p.kill()


env = dict(os.environ, CB_SERVICE="0", CB_CHECKOUT_ROOT=args.main)
py = Server([sys.executable, "-B", "-m", "codebreaker", "mcp"], dict(env, **dict(x.split("=", 1) for x in args.py_env)), args.main)
cpp = Server([args.cpp], dict(env, **dict(x.split("=", 1) for x in args.cpp_env)), args.main)

CALLS = [
    ("describe", {"path": "lz32.dll"}), ("describe", {"path": "lzexpand.dll"}), ("describe", {"path": "find.exe"}),
    ("inspect", {"module": "lz32.dll"}), ("inspect", {"module": "lzexpand.dll", "section": "summary"}), ("inspect", {"module": "find.exe", "section": "summary"}),
    ("strings", {"module": "lz32.dll", "limit": 20}), ("strings", {"module": "find.exe", "find": "file", "limit": 10}),
    ("hex_dump", {"module": "lz32.dll", "offset": 64, "length": 64}), ("hex_dump", {"module": "lz32.dll", "addr": "bfea1000", "length": 48}),
    ("hex_dump", {"module": "lzexpand.dll", "addr": "1:0000"}), ("hex_dump", {"module": "find.exe", "addr": "100"}),
    ("modules", {"kind": "pe", "find": "lz32", "limit": 5}), ("coverage", {}), ("progress", {}),
    ("functions", {"module": "lz32.dll", "limit": 8}), ("functions", {"module": "lzexpand.dll", "limit": 8}), ("functions", {"module": "find.exe", "limit": 8}),
    ("function", {"module": "lz32.dll", "addr": "bfea1000"}),
    ("names", {"module": "lz32.dll", "limit": 40}), ("names", {"module": "lzexpand.dll", "limit": 40}), ("names", {"module": "find.exe", "limit": 10}),
    ("identify", {"module": "lz32.dll"}), ("identify", {"module": "lzexpand.dll"}), ("identify", {"module": "find.exe"}),
    ("demangle", {"decorated": ["?Foo@@YAXH@Z", "_Bar@8", "plain"]}),
    ("commands", {}), ("command_help", {"command": "decompile"}), ("command_help", {"command": "names"}),
    ("reach_results", {}), ("projects", {}),
    ("source_tree", {"module": "lz32.dll"}), ("source_tree", {"module": "find.exe"}),
    ("read_file", {"path": WT + "/native/cb/ui/testdata/make_mcp_golden.py", "length": 200}),
    ("disasm", {"module": "lz32.dll", "addr": "bfea1000"}), ("disasm", {"module": "lz32.dll", "addr": "bfea1000", "count": 12}),
    ("disasm", {"module": "lzexpand.dll", "addr": "1:0000", "count": 10}), ("disasm", {"module": "find.exe", "addr": "100"}),
    ("xrefs", {"module": "lz32.dll", "addr": "bfea1000"}), ("xrefs", {"module": "lz32.dll", "addr": "KERNEL32!CloseHandle"}),
    ("xrefs", {"module": "lzexpand.dll", "addr": "1:0000"}), ("xrefs", {"module": "find.exe", "addr": "100"}),
    ("decompile_function", {"module": "lz32.dll", "addr": "bfea1000"}), ("decompile_function", {"module": "lzexpand.dll", "addr": "1:0000"}),
    ("decompile_function", {"module": "find.exe", "addr": "100"}),
    ("triage", {"module": "lz32.dll"}), ("triage", {"module": "lzexpand.dll"}), ("triage", {"module": "find.exe"}),
    ("jobs", {}),
]
if args.calls:
    CALLS = [tuple(c) for c in json.load(open(args.calls))]
only = set(x for x in args.only.split(",") if x)
if only:
    CALLS = [c for c in CALLS if c[0] in only]

VOLATILE = re.compile(r"^(time|seconds|seconds_so_far|started|when|age|id|job_id|pid|owner|idle)$")
TIMING = re.compile(r"\(\d+\.\ds\)")   # the "(0.3s)" a comparison report ends with


def body(answer):
    """What a tool call came to: the JSON of its text (or the text), the error flag."""
    r = answer.get("result")
    if r is None:
        return {"protocol_error": answer.get("error")}
    texts = [c.get("text", "") for c in r.get("content", []) if c.get("type") == "text"]
    text = "\n".join(texts)
    try:
        value = json.loads(text)
    except ValueError:
        value = text
    return {"isError": bool(r.get("isError")), "value": value}


def diff(a, b, path="$"):
    out = []
    if type(a) != type(b) and not (isinstance(a, (int, float)) and isinstance(b, (int, float))):
        return ["%s: %s vs %s" % (path, short(a), short(b))]
    if isinstance(a, dict):
        for k in a:
            if k not in b:
                out.append("%s.%s: only in python" % (path, k))
            elif not VOLATILE.match(k):
                out += diff(a[k], b[k], path + "." + k)
        out += ["%s.%s: only in native" % (path, k) for k in b if k not in a]
    elif isinstance(a, list):
        if len(a) != len(b):
            out.append("%s: %d items vs %d" % (path, len(a), len(b)))
        for i, (x, y) in enumerate(zip(a, b)):
            out += diff(x, y, "%s[%d]" % (path, i))
    elif isinstance(a, str) and TIMING.sub("(Ns)", a) == TIMING.sub("(Ns)", b):
        pass
    elif a != b:
        out.append("%s: %s vs %s" % (path, short(a), short(b)))
    return out


def short(v):
    s = json.dumps(v) if not isinstance(v, str) else v
    return (s[:90] + "...") if len(s) > 90 else s


results = {"python": [], "native": []}
rows = []
last = {"python": None, "native": None}   # the id a job was started under, for the calls that say "$LAST_ID"


def bound(arguments, who):
    return {k: (last[who] if v == "$LAST_ID" else v) for k, v in arguments.items()}


for name, arguments in CALLS:
    if name == "@sleep":
        time.sleep(arguments.get("seconds", 1))
        continue
    t = time.time()
    a = body(py.call(name, bound(arguments, "python")))
    ta = time.time() - t
    t = time.time()
    b = body(cpp.call(name, bound(arguments, "native")))
    tb = time.time() - t
    for who, got in (("python", a), ("native", b)):
        if isinstance(got.get("value"), dict) and isinstance(got["value"].get("id"), str):
            last[who] = got["value"]["id"]
    results["python"].append([name, arguments, a])
    results["native"].append([name, arguments, b])
    d = diff(a, b)
    label = "%s %s" % (name, json.dumps(arguments)[:70])
    rows.append((label, "identical" if not d else "differs", d, ta, tb))
    print("%-10s %6.2fs %6.2fs  %s%s" % ("identical" if not d else "DIFFERS", ta, tb, label, ("\n      " + "\n      ".join(d[:6])) if d else ""), flush=True)
json.dump(results["python"], open(os.path.join(args.out, "python.json"), "w"), indent=1)
json.dump(results["native"], open(os.path.join(args.out, "native.json"), "w"), indent=1)
py.close()
cpp.close()
print("%d identical, %d differ of %d" % (sum(1 for r in rows if r[1] == "identical"), sum(1 for r in rows if r[1] != "identical"), len(rows)))
