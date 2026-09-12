#!/usr/bin/env python3
r"""ledger95.py -- the master reconstruction ledger (Phase A of the roadmap).

One per-module map joining ORIGIN-triage with AUTHORED-LANGUAGE, reported over the correct
(app/OS-specific) denominator -- the live dashboard the rest of the roadmap is measured against.
Per module: total functions/bytes, CRT/compiler-identified (factored out), and the app/OS-specific
split by authored form (C-simple / C-logic / asm). Writes a JSON and prints the rollup.

    wt --target win95 ledger95 [MODULE ...]      # default: the modules that define Win95
"""
import os
import sys
import json
import collections

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import triage

# the 32-bit PE modules that define Windows 95 (the readable-C target surface)
KEY = ["kernel32.dll", "user32.dll", "gdi32.dll", "shell32.dll", "advapi32.dll", "ole32.dll",
       "oleaut32.dll", "comctl32.dll", "comdlg32.dll", "winmm.dll", "mpr.dll", "version.dll",
       "lz32.dll", "crtdll.dll", "rpcrt4.dll", "shlwapi.dll", "wsock32.dll", "mapi32.dll",
       "imm32.dll", "rasapi32.dll", "msvfw32.dll", "wininet.dll"]


def _pct(a, b):
    return 100.0 * a / b if b else 0.0


def run(modules):
    rows = []
    tot = collections.Counter()
    for m in modules:
        try:
            t = triage.triage_module(m)
        except Exception as e:
            print("  SKIP %-14s (%s)" % (m, str(e)[:50])); continue
        code_b = t["crt_bytes"] + t["app_bytes"]
        by = collections.Counter(rt for _r, rt, _s in t["app"])
        by_b = collections.Counter()
        for _r, rt, sz in t["app"]:
            by_b[rt] += sz
        rec = {"module": m, "n": t["n"], "code_b": code_b,
               "crt_n": len(t["crt"]), "crt_b": t["crt_bytes"],
               "app_n": len(t["app"]), "app_b": t["app_bytes"],
               "c_simple_b": by_b.get("c", 0), "c_logic_b": by_b.get("human", 0),
               "asm_b": by_b.get("asm", 0),
               "n_c": by.get("c", 0), "n_logic": by.get("human", 0), "n_asm": by.get("asm", 0)}
        rows.append(rec)
        for k in ("code_b", "crt_b", "app_b", "c_simple_b", "c_logic_b", "asm_b"):
            tot[k] += rec[k]
        print("  %-14s code=%-7d CRT/comp=%-5d app=%-7d  app-lang: C-simple %d%% / C-logic %d%% / asm %d%%" % (
            m, code_b, rec["crt_b"], rec["app_b"],
            _pct(rec["c_simple_b"], rec["app_b"]), _pct(rec["c_logic_b"], rec["app_b"]),
            _pct(rec["asm_b"], rec["app_b"])), flush=True)
    out = os.path.join(os.getcwd(), "reports", "master_ledger.json")
    try:
        os.makedirs(os.path.dirname(out), exist_ok=True)
        json.dump({"modules": rows, "totals": dict(tot)}, open(out, "w"), indent=1)
    except Exception:
        out = None
    cb = tot["code_b"] or 1
    app = tot["app_b"] or 1
    print("=" * 72)
    print("MASTER LEDGER -- %d modules, %d code bytes" % (len(rows), tot["code_b"]))
    print("  CRT / compiler (identified, NOT targets): %d B (%.1f%%)" % (tot["crt_b"], _pct(tot["crt_b"], cb)))
    print("  app/OS-specific (reconstruction target)  : %d B (%.1f%%)" % (tot["app_b"], _pct(tot["app_b"], cb)))
    print("  --- app/OS-specific by authored form ---")
    print("    C-simple (emit_c)      : %d B (%.1f%% of app)" % (tot["c_simple_b"], _pct(tot["c_simple_b"], app)))
    print("    C-logic  (behavioral)  : %d B (%.1f%% of app)  <- the bulk of the real work" % (tot["c_logic_b"], _pct(tot["c_logic_b"], app)))
    print("    asm (recover as asm)   : %d B (%.1f%% of app)" % (tot["asm_b"], _pct(tot["asm_b"], app)))
    if out:
        print("  wrote", out)
    print("=" * 72)


from codebreaker.cli.common import opt as _opt


def main():
    argv = [a for a in sys.argv[1:] if not a.startswith("-")]
    if sys.argv[1:] and sys.argv[1] in ("-h", "--help"):
        print(__doc__); return
    from codebreaker.core import config
    config.apply_config()
    print("building the master reconstruction ledger...", flush=True)
    run(argv or KEY)


if __name__ == "__main__":
    main()
