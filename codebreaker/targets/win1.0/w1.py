#!/usr/bin/env python3
"""w1 -- one front door for the whole Windows 1.01 reconstruction toolkit.

Instead of remembering which of ~80 scripts to run, everything goes through here:

    python w1.py <command> [args...]   run a tool with its own CLI, unchanged
    python w1.py list [group]          list every command, grouped, with a blurb
    python w1.py help <command>        show a tool's full docstring
    python w1.py which <command>       show the file a command maps to
    python w1.py find <text>           search commands+blurbs for a keyword

A command name is just the tool's filename without ".py", so `w1 lift CALC`
runs `lift.py CALC` and `w1 graft ...` runs `graft.py ...`. This is a dispatcher,
not a rewrite: the underlying tools -- their flags, behavior, and output -- are
untouched. Every .py in the project is reachable; the curated GROUPS below only
decide ordering/headings in `list`, nothing more.
"""
import os
import sys
import ast
import runpy
import difflib

ROOT = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, ROOT)  # so dispatched tools can import their siblings

# Curated headings for the main reconstruction flow, in roughly the order you'd
# run them. Anything not listed here still works and appears under "More tools"
# in `list`, auto-described from its module docstring -- so coverage is total.
GROUPS = [
    ("Disassemble & decode", [
        "disasm", "difdec", "opc8086", "cfg", "ne", "rsrc", "sniff",
    ]),
    ("Decompile / lift to C", [
        "lift", "decomp", "wndproc", "masmlift", "ansi2knr",
    ]),
    ("Build & link (period toolchain)", [
        "build16", "link16", "graft", "segsplit", "mkdef", "mkrc",
        "mkstubs", "build", "splitbuild", "linkall", "linkdll",
    ]),
    ("Analyze & validate", [
        "validate", "verify", "segprofile", "api_matrix", "correlate",
        "scoreboard", "symscore", "evid", "asmratio", "identify",
        "sdkcheck", "argcheck", "sweep",
    ]),
    ("Names, symbols & maps", [
        "names", "applysym", "symfile", "idord", "declglobals", "globals",
        "libnames", "undecl", "msgfwd", "cmdmap", "appmap", "appledger",
        "ledger", "dash",
    ]),
    ("Resources & media", [
        "rsrc", "fon", "extract_iso", "fat12", "split_bundle",
    ]),
]


def all_tools():
    """Map command name -> file path for every dispatchable .py in ROOT."""
    out = {}
    me = os.path.basename(__file__)
    for f in sorted(os.listdir(ROOT)):
        if f.endswith(".py") and f != me:
            out[f[:-3]] = os.path.join(ROOT, f)
    return out


def blurb(path):
    """First non-empty line of a module's docstring, read WITHOUT importing it
    (parsing beats importing -- no side effects, no dependency on the tool
    actually being importable)."""
    try:
        doc = ast.get_docstring(ast.parse(open(
            path, encoding="utf-8", errors="replace").read()))
        if doc:
            for line in doc.splitlines():
                if line.strip():
                    return line.strip()
    except Exception:
        pass
    return ""


def cmd_list(group_filter=None):
    tools = all_tools()
    shown = set()
    for title, names in GROUPS:
        if group_filter and group_filter.lower() not in title.lower():
            continue
        rows = [(n, tools[n]) for n in names if n in tools]
        if not rows:
            continue
        print("\n%s" % title)
        print("-" * len(title))
        for n, p in rows:
            print("  %-15s %s" % (n, blurb(p)))
            shown.add(n)
    if not group_filter:
        rest = [n for n in sorted(tools) if n not in shown]
        if rest:
            print("\nMore tools")
            print("----------")
            for n in rest:
                print("  %-15s %s" % (n, blurb(tools[n])))
    print("\n%d commands. Run:  python w1.py <command> [args]   "
          "(python w1.py help <command> for details)" % len(tools))


def cmd_help(name):
    tools = all_tools()
    if name not in tools:
        _unknown(name, tools)
        return
    src = open(tools[name], encoding="utf-8", errors="replace").read()
    doc = ast.get_docstring(ast.parse(src))
    print("%s  ->  %s\n" % (name, tools[name]))
    print(doc.strip() if doc else "(no docstring)")
    print("\nRun `python w1.py %s --help` for command-line options "
          "(if the tool provides them)." % name)


def cmd_which(name):
    tools = all_tools()
    if name in tools:
        print(tools[name])
    else:
        _unknown(name, tools)


def cmd_find(text):
    tools = all_tools()
    t = text.lower()
    hits = [(n, blurb(p)) for n, p in tools.items()
            if t in n.lower() or t in blurb(p).lower()]
    if not hits:
        print("no command matches %r" % text)
        return
    for n, b in sorted(hits):
        print("  %-15s %s" % (n, b))


def _unknown(name, tools):
    sys.stderr.write("w1: unknown command '%s'\n" % name)
    near = difflib.get_close_matches(name, list(tools), n=3)
    if near:
        sys.stderr.write("did you mean:  %s\n" % ", ".join(near))
    sys.stderr.write("try:  python w1.py list\n")
    sys.exit(2)


def dispatch(name, argv):
    """Run tool `name` exactly as if invoked as `python name.py argv...`."""
    tools = all_tools()
    if name not in tools:
        _unknown(name, tools)
    sys.argv = [tools[name]] + argv
    runpy.run_path(tools[name], run_name="__main__")


def main():
    args = sys.argv[1:]
    if not args or args[0] in ("-h", "--help"):
        print(__doc__.strip())
        print()
        cmd_list()
        return
    cmd, rest = args[0], args[1:]
    if cmd == "list":
        cmd_list(rest[0] if rest else None)
    elif cmd == "help":
        if not rest:
            print(__doc__.strip())
        else:
            cmd_help(rest[0])
    elif cmd == "which":
        cmd_which(rest[0]) if rest else sys.exit("usage: w1 which <command>")
    elif cmd == "find":
        cmd_find(rest[0]) if rest else sys.exit("usage: w1 find <text>")
    else:
        dispatch(cmd, rest)


if __name__ == "__main__":
    main()
