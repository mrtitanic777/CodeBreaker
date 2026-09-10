#!/usr/bin/env python3
r"""trace.py -- `wt behavior trace`: record a function's behavior into the evidence corpus.

    wt behavior trace <module> [func] [options]

    <module>         a win95 binary the active target can resolve, e.g. gdi32.dll
    [func]           function selector: a VA or section offset as hex (0x401250) or decimal,
                     or omitted to trace the module's first recovered function.
    --args a,b,c     explicit stdcall dword args (hex or decimal); default: zero-filled.
    --arity N        number of zero dwords to push when --args is absent (default: recovered).
    --list           list the module's recovered functions and exit (no run).
    --insns N        instruction cap for the run (default 20000).

One run -> one TraceRecord stored in <data>/behavior/. Reuses difftest's proven entry state;
lazily maps memory so pointer-touching functions are traceable, not just pure leaves.
"""
import sys

from codebreaker.engines.behavior import tracer, corpus


from codebreaker.cli.common import opt as _opt


def main():
    argv = sys.argv[1:]
    if not argv or argv[0] in ("-h", "--help"):
        print(__doc__); return
    module = argv[0]
    flags = {"--args", "--arity", "--insns", "--stub-cleanup"}
    # positional func = first bare token after the module that isn't a flag or a flag's value
    func = None
    i = 1
    while i < len(argv):
        a = argv[i]
        if a in flags:
            i += 2; continue
        if a.startswith("-"):
            i += 1; continue
        func = a; break
    if "--list" in argv:
        print("%s -- recovered functions (off / VA / size):" % module)
        for off, va, size in tracer.list_functions(module):
            print("  0x%-6x  VA 0x%-8x  %5d bytes" % (off, va, size))
        return

    args = None
    if "--args" in argv:
        raw = _opt(argv, "--args", "")
        args = [int(x, 0) for x in raw.split(",") if x.strip() != ""]
    arity = _opt(argv, "--arity")
    arity = int(arity) if arity is not None else None
    insns = _opt(argv, "--insns")
    insns = int(insns) if insns is not None else tracer._MAX_INSNS

    stub = "--stub-externals" in argv
    cleanup = int(_opt(argv, "--stub-cleanup", "0"), 0)
    sig = None
    if stub:
        from codebreaker.engines.behavior import env
        sig = env.resolver()
    rec = tracer.trace_function(module, func=func, args=args, arity=arity, insn_cap=insns,
                                stub_externals=stub, stub_cleanup=cleanup, sig_resolver=sig)
    fp = corpus.record(rec)

    print("traced %s  func VA 0x%x  (off 0x%x, %s)" %
          (module, rec["func_va"], rec["func_off"], rec["section"]))
    print("  input args : %s" % ", ".join("0x%x" % a for a in rec["input"]["args"]) or "(none)")
    print("  exit       : %s" % rec["exit"])
    print("  ret (eax)  : %s" % ("0x%x" % rec["ret"] if rec["ret"] is not None else "(none)"))
    print("  instructions: %d%s" % (rec["insn_count"], "  [TRUNCATED]" if rec["truncated"] else ""))
    print("  mem reads  : %d   mem writes: %d   lazy maps: %d   calls: %d" %
          (len(rec["mem_reads"]), len(rec["mem_writes"]), len(rec["lazy_maps"]), len(rec["calls"])))
    if rec.get("api_deps"):
        print("  API deps   : %s" % ", ".join(rec["api_deps"][:12]))
    print("  stored     : %s" % fp)


if __name__ == "__main__":
    main()
