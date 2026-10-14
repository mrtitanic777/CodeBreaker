#!/usr/bin/env python3
"""port.py -- carry modules from the old CodeBreaker checkout into this layout.

    python migrate/port.py [--from ../CodeBreaker] [--dry] [--force]

Each old module has one new home (MAP below, plus the package rules in RULES). Starting from
ROOTS it follows every import, copies what it reaches to its new home and rewrites the imports
to the new names. A module that has no home yet stops the run with its name, so nothing arrives
half-connected. A file already present here is left alone unless --force: once a module has been
ported, this checkout is where it is edited.

migrate/ported.json records what came from where.
"""
import ast
import json
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
V2 = os.path.dirname(HERE)
PKG = "codebreaker"

# what the first slice must be able to do: decompile, build, run, judge, record, write a module
ROOTS = [
    "src.targets.win95.module_source", "src.targets.win95.bank", "src.targets.win95.verification.pipeline",
    "src.targets.win95.target", "src.disasm.trace", "src.formats.lefile", "src.formats.cab",
    "src.targets.win95.pe32util", "src.targets.win95.vmmsvc",
]

# old module -> new module (without the package prefix)
MAP = {
    # base: where things are, how work is spread and reported
    "src.core.config": "base.config", "src.core.paths": "base.paths", "src.core.corpus": "base.corpus",
    "src.core.layout": "base.layout", "src.core.parallel": "base.parallel", "src.core.progress": "base.progress",
    "src.core.toolchain": "base.period_tools", "src.cli.common": "base.args",
    "src.core.progressui": "ui.progress_window", "src.ir.explore_model": "ui.explore_model",
    # running code
    "src.engines.behavior.tracer": "emulate.trace32", "src.engines.behavior.tracer16": "emulate.trace16",
    "src.engines.behavior.env": "emulate.env", "src.engines.behavior.envprov": "emulate.provision",
    "src.engines.behavior.perturb": "emulate.perturb", "src.engines.behavior.apiarity": "emulate.api_stack",
    "src.targets.win95.ne_dynoracle": "emulate.ne_probe",
    # getting source out of a binary
    "src.targets.win95.verification.ghidra_core": "decompile.ghidra_core",
    "src.toolchain.ghidra.ghidra_batch": "decompile.ghidra_headless_batch",
    "src.toolchain.ghidra.ghidra_decomp": "decompile.ghidra_headless",
    "src.targets.win95.raise95": "decompile.raise32", "src.targets.win95.neraise95": "decompile.raise16",
    "src.targets.win95.beautify95": "decompile.beautify",
    # judging a reconstruction against the original
    "src.targets.win95.verify95": "verify.oracle", "src.targets.win95.bodyverify": "verify.body32",
    "src.targets.win95.bodyverify16": "verify.body16",
    "src.targets.win95.verification.pipeline": "verify.pipeline",
    "src.targets.win95.verification.store": "verify.store",
    "src.targets.win95.verification.ghidra_engine": "verify.whole_module",
    "src.targets.win95.bank": "verify.bank",
    # writing the result
    "src.targets.win95.module_source": "source.module_writer",
    # names, prototypes, origin
    "src.engines.symbol.demangle": "symbols.demangle", "src.engines.symbol.symdb": "symbols.symdb",
    "src.engines.symbol.win16proto": "symbols.win16_prototypes", "src.toolchain.sdk.apidb": "symbols.api_db",
    "src.targets.win95.sdkprotos": "symbols.sdk_prototypes",
    "src.targets.win95.vmmsvc": "symbols.vmm_services", "src.targets.win95.pe32util": "formats.pe_util",
    # Windows 95 specifics
    "src.targets.win95.target": "targets.win95.profile", "src.targets.win95.origintriage": "targets.win95.origin",
    "src.targets.win95.sharedruntime": "targets.win95.shared_runtime",
    "src.targets.win95.autobuild95": "targets.win95.flat_thunk_build",
    "src.toolchain.sdk.w95paths": "targets.win95.paths",
    "src.engines.behavior.struct": "recover.structs",
}
# whole packages that keep their module names
RULES = [("src.engines.behavior.", "recover."), ("src.engines.recon.", "recover."), ("src.formats.", "formats."),
         ("src.disasm.", "disasm."), ("src.engines.symbol.", "symbols."), ("src.engines.build.", "build."),
         ("src.engines.coverage.", "coverage.")]
# replaced by something written for this layout rather than carried over
# siblings the old tool directories imported by bare name
BARE = {"cab": "src.formats.cab", "le": "src.formats.lefile", "pe32util": "src.targets.win95.pe32util",
        "vmmsvc": "src.targets.win95.vmmsvc"}
REPLACED = {"src.core.cores": "base.registry"}
# old package names that appear in comments and docstrings -> where that code lives now
PKG_HOME = {"src.core": "base", "src.formats": "formats", "src.toolchain.sdk": "symbols",
            "src.targets.win95.verification": "verify", "src.targets.win95": "targets.win95",
            "src.engines.behavior": "recover", "src.disasm": "disasm"}
PACKAGES = {"src", "src.core", "src.cli", "src.ir", "src.formats", "src.disasm", "src.engines", "src.targets",
            "src.toolchain", "src.engines.behavior", "src.engines.recon", "src.engines.symbol",
            "src.engines.build", "src.engines.coverage", "src.targets.win95", "src.targets.win95.verification",
            "src.toolchain.sdk", "src.toolchain.ghidra"}


def new_name(old):
    if old in REPLACED:
        return PKG + "." + REPLACED[old]
    if old in MAP:
        return PKG + "." + MAP[old]
    for a, b in RULES:
        if old.startswith(a) and "." not in old[len(a):]:
            return PKG + "." + b + old[len(a):]
    return None


def old_path(root, mod):
    p = os.path.join(root, *mod.split("."))
    if os.path.isfile(p + ".py"):
        return p + ".py"
    return None


def is_pkg(root, mod):
    return os.path.isdir(os.path.join(root, *mod.split(".")))


def rewrite(root, mod, text, need, problems):
    """Rewrite one module's imports; add the old modules it uses to `need`."""
    tree = ast.parse(text)
    lines = text.split("\n")
    edits = []                                               # (first line, last line, replacement)

    def use(old):
        if old in REPLACED:
            return new_name(old)
        if not old_path(root, old):
            return None
        n = new_name(old)
        if n is None:
            problems.add("%s imports %s, which has no home yet" % (mod, old))
            return None
        need.add(old)
        return n

    for node in ast.walk(tree):
        if isinstance(node, ast.ImportFrom) and node.module and not node.level and \
                (node.module == "src" or node.module.startswith("src.")):
            indent = re.match(r"\s*", lines[node.lineno - 1]).group(0)
            out = []
            as_module = use(node.module)                     # `from <module> import name`
            if as_module and not is_pkg(root, node.module):
                names = ", ".join(a.name + (" as " + a.asname if a.asname else "") for a in node.names)
                out.append("%sfrom %s import %s" % (indent, as_module, names))
            else:                                            # `from <package> import module`
                for a in node.names:
                    n = use(node.module + "." + a.name)
                    local = a.asname or a.name
                    if n is None:
                        if not is_pkg(root, node.module + "." + a.name):
                            problems.add("%s: cannot place `from %s import %s`" % (mod, node.module, a.name))
                        continue
                    pkg, leaf = n.rsplit(".", 1)
                    out.append("%sfrom %s import %s%s" % (indent, pkg, leaf, "" if leaf == local else " as " + local))
            tail = lines[node.end_lineno - 1][node.end_col_offset:]
            if out:
                out[-1] += tail
            edits.append((node.lineno, node.end_lineno, out))
        elif isinstance(node, ast.ImportFrom) and not node.level and node.module in BARE:
            indent = re.match(r"\s*", lines[node.lineno - 1]).group(0)
            names = ", ".join(a.name + (" as " + a.asname if a.asname else "") for a in node.names)
            edits.append((node.lineno, node.end_lineno, ["%sfrom %s import %s%s" % (
                indent, use(BARE[node.module]), names, lines[node.end_lineno - 1][node.end_col_offset:])]))
        elif isinstance(node, ast.Import):
            out, indent = [], re.match(r"\s*", lines[node.lineno - 1]).group(0)
            for a in node.names:
                if a.name == "src" or a.name.startswith("src."):
                    problems.add("%s: `import %s` must be written as a from-import first" % (mod, a.name))
                elif a.name in BARE:                         # a tool-directory sibling, by bare name
                    n = use(BARE[a.name])
                    pkg, leaf = n.rsplit(".", 1)
                    local = a.asname or a.name
                    out.append("%sfrom %s import %s%s" % (indent, pkg, leaf, "" if leaf == local else " as " + local))
                else:
                    out.append("%simport %s%s" % (indent, a.name, " as " + a.asname if a.asname else ""))
            if any(a.name in BARE for a in node.names):
                out[-1] += lines[node.end_lineno - 1][node.end_col_offset:]
                edits.append((node.lineno, node.end_lineno, out))
    for first, last, out in sorted(edits, reverse=True):
        lines[first - 1:last] = out
    text = "\n".join(lines)

    # dotted names inside strings (`python -m src.x.y`, importlib.import_module("src.x.y"))
    def dotted(m):
        old, rest = m.group(0), ""
        while old.count("."):                                # `src.x.y.func` -> the module, then .func
            if old_path(root, old) or old in REPLACED:
                n = use(old)
                return (n + rest) if n else m.group(0)
            if old in PKG_HOME:                              # a package named in prose
                return PKG + "." + PKG_HOME[old] + rest
            old, tail = old.rsplit(".", 1)
            rest = "." + tail + rest
        return m.group(0)
    text = re.sub(r"\bsrc(?:\.[A-Za-z_][A-Za-z0-9_]*)+", dotted, text)

    def prose(m):                                            # `from src.pkg import module` in a docstring
        full = m.group(1) + "." + m.group(2)
        n = use(full) if (old_path(root, full) or full in REPLACED) else None
        if not n:
            return m.group(0)
        pkg, leaf = n.rsplit(".", 1)
        return "from %s import %s%s" % (pkg, leaf, "" if leaf == m.group(2) else " as " + m.group(2))
    return re.sub(r"from (src(?:\.\w+)*) import (\w+)", prose, text)


def main(argv):
    root = os.path.abspath(argv[argv.index("--from") + 1] if "--from" in argv else os.path.join(V2, "..", "CodeBreaker"))
    dry, force = "--dry" in argv, "--force" in argv
    need, done, problems, log = set(ROOTS), {}, set(), {}
    while need - set(done):
        mod = sorted(need - set(done))[0]
        src = old_path(root, mod)
        if mod in REPLACED or not src:
            done[mod] = None
            continue
        n = new_name(mod)
        if n is None:
            problems.add("%s has no home yet" % mod)
            done[mod] = None
            continue
        text = open(src, encoding="utf-8", errors="replace").read().replace("\r\n", "\n")
        done[mod] = (n, rewrite(root, mod, text, need, problems))
    if problems:
        print("\n".join(sorted(problems)))
        print("%d problems -- nothing written" % len(problems))
        return 1
    wrote = kept = 0
    for mod, item in sorted(done.items()):
        if not item:
            continue
        n, text = item
        dst = os.path.join(V2, *n.split(".")) + ".py"
        log[n] = mod
        if os.path.exists(dst) and not force:
            kept += 1
            continue
        wrote += 1
        if dry:
            continue
        os.makedirs(os.path.dirname(dst), exist_ok=True)
        d = os.path.dirname(dst)
        while os.path.normcase(d) != os.path.normcase(V2):   # every level is a package
            init = os.path.join(d, "__init__.py")
            if not os.path.exists(init):
                open(init, "w").close()
            d = os.path.dirname(d)
        with open(dst, "w", encoding="utf-8", newline="\n") as f:
            f.write(text)
    if not dry:
        with open(os.path.join(HERE, "ported.json"), "w", encoding="utf-8") as f:
            json.dump(log, f, indent=1, sort_keys=True)
    print("%d modules: %d written, %d already here" % (wrote + kept, wrote, kept))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
