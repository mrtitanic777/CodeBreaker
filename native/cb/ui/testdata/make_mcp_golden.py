"""Talks to the Python MCP server over stdio and records its answers (the golden the C++ server is compared with).

Run it with the main checkout as the third argument (its Python is served):  python -B native/cb/ui/testdata/make_mcp_golden.py <worktree> <out.json> [<main checkout>]
"""
import json
import os
import subprocess
import sys

wt, out = sys.argv[1], sys.argv[2]
main = sys.argv[3] if len(sys.argv) > 3 else "."   # the checkout whose Python is served (default: the current folder)
env = dict(os.environ, CB_SERVICE="0")
p = subprocess.Popen([sys.executable, "-B", "-m", "codebreaker", "mcp"], stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                     stderr=subprocess.DEVNULL, env=env, cwd=main)


def rpc(msg):
    p.stdin.write((json.dumps(msg) + "\n").encode())
    p.stdin.flush()
    if "id" not in msg:
        return None
    return json.loads(p.stdout.readline())


answers = {}
answers["initialize"] = rpc({"jsonrpc": "2.0", "id": 1, "method": "initialize", "params": {
    "protocolVersion": "2025-06-18", "capabilities": {}, "clientInfo": {"name": "t", "version": "1"}}})
rpc({"jsonrpc": "2.0", "method": "notifications/initialized"})
answers["tools/list"] = rpc({"jsonrpc": "2.0", "id": 2, "method": "tools/list"})
answers["prompts/list"] = rpc({"jsonrpc": "2.0", "id": 3, "method": "prompts/list"})
answers["prompts/get"] = rpc({"jsonrpc": "2.0", "id": 4, "method": "prompts/get", "params": {"name": "triage", "arguments": {"path": "lz32.dll"}}})
answers["resources/list"] = rpc({"jsonrpc": "2.0", "id": 5, "method": "resources/list"})
answers["resources/templates/list"] = rpc({"jsonrpc": "2.0", "id": 6, "method": "resources/templates/list"})
answers["ping"] = rpc({"jsonrpc": "2.0", "id": 7, "method": "ping"})
answers["nosuch"] = rpc({"jsonrpc": "2.0", "id": 8, "method": "nosuch/method"})
calls = [
    ("demangle", {"decorated": ["?Foo@@YAXH@Z", "_Bar@8", "plain"]}),
    ("hex_dump", {"module": wt + "/native/cb/formats/testdata/lzexpand.dll", "offset": 16, "length": 40}),
    ("strings", {"module": wt + "/native/cb/formats/testdata/lzexpand.dll", "find": "LZ", "limit": 5}),
    ("command_help", {"command": "nosuchcommand"}),
    ("hex_dump", {"module": "no-such-module.dll"}),
    ("describe", {}),
    ("strings", {"module": wt + "/native/cb/formats/testdata/lzexpand.dll", "min_len": "x", "limit": 2.5}),
    ("hex_dump", {"module": wt + "/native/cb/formats/testdata/lzexpand.dll", "offset": True, "length": "20"}),
    ("jobs", {"extra": 1}),
    ("job_file", {"job_id": "nothing"}),
    ("stop_job", {"job_id": "nothing"}),
    ("read_file", {"path": "C:/Windows/notepad.exe"}),
    ("demangle", {"decorated": 5}),
    ("name_function", {"module": "x", "addr": "1", "name": "bad name"}),
    ("job", {"job_id": "nothing"}),
    ("jobs", {}),
    ("nosuchtool", {}),
    # the code, the names and the runtime libraries (calls 17 and on): a 32-bit module (lz32.dll), a 16-bit one (lzexpand.dll) and a DOS program
    ("disasm", {"module": wt + "/native/cb/formats/testdata/lz32.dll", "addr": "bfea1000"}),
    ("disasm", {"module": wt + "/native/cb/formats/testdata/lz32.dll", "addr": "bfea1000", "count": 12}),
    ("disasm", {"module": wt + "/native/cb/formats/testdata/lzexpand.dll", "addr": "2:0000"}),
    ("disasm", {"module": wt + "/native/cb/formats/testdata/lzexpand.dll", "addr": "LZINIT", "count": 8}),
    ("disasm", {"module": wt + "/native/cb/formats/testdata/find.exe", "addr": "100"}),
    ("xrefs", {"module": wt + "/native/cb/formats/testdata/lz32.dll", "addr": "bfea1000"}),
    ("xrefs", {"module": wt + "/native/cb/formats/testdata/lz32.dll", "addr": "KERNEL32!OpenFile"}),
    ("xrefs", {"module": wt + "/native/cb/formats/testdata/lzexpand.dll", "addr": "LZINIT"}),
    ("xrefs", {"module": wt + "/native/cb/formats/testdata/lzexpand.dll", "addr": "4:0000"}),
    ("names", {"module": wt + "/native/cb/formats/testdata/lz32.dll", "limit": 10}),
    ("names", {"module": wt + "/native/cb/formats/testdata/lzexpand.dll", "limit": 12}),
    ("identify", {"module": wt + "/native/cb/formats/testdata/lz32.dll"}),
    ("identify", {"module": wt + "/native/cb/formats/testdata/lzexpand.dll"}),
    ("hex_dump", {"module": wt + "/native/cb/formats/testdata/lz32.dll", "addr": "bfea1000", "length": 32}),
    ("hex_dump", {"module": wt + "/native/cb/formats/testdata/lzexpand.dll", "addr": "2:0000"}),
    ("diff_function", {"module_a": wt + "/native/cb/formats/testdata/lz32.dll", "module_b": wt + "/native/cb/formats/testdata/lz32.dll", "addr": "bfea1000"}),
    ("diff_function", {"module_a": wt + "/native/cb/formats/testdata/lz32.dll", "module_b": wt + "/native/cb/formats/testdata/lz32.dll", "addr": "nosuchfunction"}),
    # one function decompiled now (calls 34 and on): a 32-bit function by itself, a 16-bit one cut out of its module's C, a function that is not there
    ("decompile_function", {"module": wt + "/native/cb/formats/testdata/lz32.dll", "addr": "bfea1000"}),
    ("decompile_function", {"module": wt + "/native/cb/formats/testdata/lzexpand.dll", "addr": "1:0000"}),
    ("decompile_function", {"module": wt + "/native/cb/formats/testdata/lzexpand.dll", "addr": "9:9999"}),
]
for i, (name, args) in enumerate(calls):
    answers["call %d %s" % (i, name)] = rpc({"jsonrpc": "2.0", "id": 100 + i, "method": "tools/call", "params": {"name": name, "arguments": args}})
p.stdin.close()
p.wait(timeout=20)
json.dump(answers, open(out, "w"), indent=1, sort_keys=True)
print({k: ("error" in v and "ERR" or "ok") if v else None for k, v in answers.items()})
