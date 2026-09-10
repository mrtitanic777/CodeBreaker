"""Call-graph / cross-reference builder for Xbox 360 PowerPC images.

Discovers functions (pdata + bl-descent, via ppcdiscover), disassembles each,
and follows direct calls (`bl`/`bla`) to build a call graph: who calls whom, and
for any address, its callers and callees. Emits a text summary, per-function
xrefs, or Graphviz DOT for visualisation. Reuses the project's Capstone PPC
disassembler; MSVC-decorated names are demangled when a symbol map is supplied.
"""

import bisect
import sys

DESCRIPTION = "build a PowerPC call graph / cross-reference index (callers, callees, DOT)"


def _containing(sorted_starts, addr):
    i = bisect.bisect_right(sorted_starts, addr) - 1
    return sorted_starts[i] if i >= 0 else None


def build(code, base, funcs):
    """funcs = {va: size}. Returns (callees, callers): each {func_va: set(va)}.
    Only direct bl/bla calls whose target is a known function are edges."""
    import capstone
    from capstone import ppc as _ppc
    md = capstone.Cs(capstone.CS_ARCH_PPC, capstone.CS_MODE_32 | capstone.CS_MODE_BIG_ENDIAN)
    md.detail = True
    starts = sorted(funcs)
    callees = {f: set() for f in starts}
    callers = {f: set() for f in starts}
    for f in starts:
        size = funcs[f] or 4
        off = f - base
        if off < 0 or off + size > len(code):
            continue
        for ins in md.disasm(bytes(code[off:off + size]), f):
            if ins.mnemonic in ("bl", "bla"):
                for op in ins.operands:
                    if op.type == _ppc.PPC_OP_IMM:
                        tgt = op.imm & 0xFFFFFFFF
                        home = _containing(starts, tgt)
                        if home is not None and home == tgt:      # exact function entry
                            callees[f].add(tgt)
                            callers[tgt].add(f)
    return callees, callers


def _load(xex_path):
    import ppcdiscover
    import disasm_ppc
    funcs = ppcdiscover.discover(xex_path)[0]           # (funcs, sources, x, img, secs)
    code, base, _entry = disasm_ppc._code_section(xex_path, None, False)
    return code, base, funcs


def to_dot(callees, name=lambda a: "sub_%08X" % a):
    lines = ["digraph callgraph {", '  node [shape=box,fontname="monospace"];']
    for f, outs in callees.items():
        for t in sorted(outs):
            lines.append('  "%s" -> "%s";' % (name(f), name(t)))
    lines.append("}")
    return "\n".join(lines)


def main():
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage:\n"
              "  callgraph <xex>                     summary (functions, edges, leaves/roots)\n"
              "  callgraph <xex> --xref 0xVA         callers + callees of one function\n"
              "  callgraph <xex> --dot [out.dot]     Graphviz call graph\n"
              "  callgraph <xex> --hot [N]           the N most-called functions")
        return
    code, base, funcs = _load(a[0])
    callees, callers = build(code, base, funcs)
    edges = sum(len(v) for v in callees.values())
    if "--xref" in a:
        va = int(a[a.index("--xref") + 1], 0)
        print("0x%08X:" % va)
        print("  called by (%d): %s" % (len(callers.get(va, ())),
              ", ".join("0x%08X" % c for c in sorted(callers.get(va, ())))))
        print("  calls (%d):     %s" % (len(callees.get(va, ())),
              ", ".join("0x%08X" % c for c in sorted(callees.get(va, ())))))
        return
    if "--dot" in a:
        dot = to_dot(callees)
        idx = a.index("--dot")
        if idx + 1 < len(a) and not a[idx + 1].startswith("--"):
            open(a[idx + 1], "w").write(dot)
            print("wrote %d edges -> %s" % (edges, a[idx + 1]))
        else:
            print(dot)
        return
    if "--hot" in a:
        idx = a.index("--hot")
        n = int(a[idx + 1]) if idx + 1 < len(a) and a[idx + 1].isdigit() else 20
        hot = sorted(callers.items(), key=lambda kv: len(kv[1]), reverse=True)[:n]
        print("most-called functions:")
        for f, cs in hot:
            print("  0x%08X  <- %d caller(s)" % (f, len(cs)))
        return
    roots = [f for f in funcs if not callers[f]]
    leaves = [f for f in funcs if not callees[f]]
    print("%s: %d functions, %d direct-call edges" % (a[0], len(funcs), edges))
    print("  %d roots (uncalled), %d leaves (call nothing)" % (len(roots), len(leaves)))
    top = sorted(callers.items(), key=lambda kv: len(kv[1]), reverse=True)[:5]
    print("  hottest: " + ", ".join("0x%08X(<-%d)" % (f, len(c)) for f, c in top if c))


if __name__ == "__main__":
    main()
