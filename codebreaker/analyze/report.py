"""report.py -- run the analyses on a module and write what they found.

    python -m codebreaker analyze <module> [--what a,b,c] [--out FILE] [--run N|all] [--facts]
                                           [--starts FILE] [--json] [--no-store] [--summary]

    <module>     any module the corpus names, or a path: a 32-bit module, a 16-bit module, or a
                 driver ("vmm32.vxd#vmm" names one driver inside a bundle)
    --what       which analyses, of: call_graph, conventions, globals, strings, structures,
                 classes, dispatch, resources, observe      (default: all but observe)
    --run N      also run N functions in the emulator (observe); "all" for every function.
                 32-bit modules only; a few hundred functions take minutes.
    --out FILE   where the report goes (default: state/analyze/<module>.txt)
    --facts      list every fact in the report, not only the counts and highlights
    --starts FILE  write the function starts found here that the decompiler's description lacks,
                 as lines of that description (`F <address> <name> <bytes removed> [model]`)
    --json       print the counts and timings as one JSON object
    --summary    print one line per analysis instead of the report's path
    --no-store   do not write the facts to the function store

The facts go to the function store's `fact` table (analyze/facts.py), replacing what the same
analysis stored for the module before.

    from codebreaker.analyze import report
    r = report.run("comctl32.dll")          # {"counts": {analysis: {...}}, "seconds": {...}, ...}
"""
import json
import os
import sys
import time

from codebreaker.analyze import (call_graph, classes, conventions, dispatch, facts, globals as global_variables,
                                 image, observe, resources, strings, structures)
from codebreaker.base import corpus, settings
from codebreaker.store import functions

ANALYSES = (("call_graph", call_graph), ("conventions", conventions), ("globals", global_variables),
            ("strings", strings), ("structures", structures), ("classes", classes), ("dispatch", dispatch),
            ("resources", resources), ("observe", observe))
DEFAULT = tuple(name for name, _m in ANALYSES if name != "observe")


def run(module, what=None, store=True, observed=None):
    """Analyse one module. -> {"module", "kind", "functions", "counts", "seconds", "results"}
    `observed`: how many functions to run in the emulator when "observe" is asked for (None: all)."""
    what = tuple(what or DEFAULT)
    unknown = [w for w in what if w not in dict(ANALYSES)]
    if unknown:
        raise ValueError("no analysis called %s (there are: %s)" % (", ".join(unknown), ", ".join(n for n, _m in ANALYSES)))
    t0 = time.time()
    img = image.load(module)
    out = {"module": img.name, "path": img.path, "kind": img.kind, "bits": img.bits, "functions": len(img.functions),
           "counts": {}, "seconds": {"load": round(time.time() - t0, 2)}, "results": {}, "image": img}
    for name, mod in ANALYSES:
        if name not in what:
            continue
        t = time.time()
        try:
            res = mod.analyse(img, limit=observed) if name == "observe" else mod.analyse(img)
        except ImportError:
            raise
        except Exception as e:                      # one analysis failing on a module is a finding, not the end
            res = {"counts": {"failed": "%s: %s" % (type(e).__name__, e)}, "facts": [], "text": []}
        out["seconds"][name] = round(time.time() - t, 2)
        out["counts"][name] = res["counts"]
        out["results"][name] = res
    out["seconds"]["total"] = round(time.time() - t0, 2)
    if store:
        sha = corpus.sha(img.path)
        if img.name != os.path.basename(img.path).lower():           # one driver of a bundle
            sha = "%s#%s" % (sha, img.name.split("#", 1)[1])
        fs = functions.open()
        fs.add_module(sha, img.name, img.path, img.kind)
        fs.close()
        table = facts.open()
        for name in out["results"]:
            table.put(sha, name, [(facts.key(img, f.get("addr")), f["kind"],
                                   {k: v for k, v in f.items() if k not in ("addr", "kind")})
                                  for f in out["results"][name]["facts"]])
        table.close()
        out["sha"] = sha
    return out


def starts(img):
    """The function starts found beyond the decompiler's description, as description lines
    (see decompile/describe.py): address, name, bytes of arguments the function removes (-1 when
    its return was not reached) and, for 16-bit code, how it is called."""
    facts_c = conventions.functions(img)
    lines = []
    for va in sorted(img.discovered):
        fact = facts_c[va]
        pop = fact.get("removes")
        line = "F\t%x\t%s\t%d" % (va, img.names[va], -1 if pop is None else pop)
        if img.kind == "ne":
            line += "\t__%s16%s" % ("cdecl" if pop == 0 else "stdcall", "far" if (fact.get("convention") or "").endswith("far") else "near")
        lines.append(line)
    return lines


def _feedback(result):
    """What the analyses found that a description for the decompiler could carry."""
    img, c = result["image"], result["counts"].get("conventions") or {}
    rows = [("function starts the description does not have", len(img.discovered))]
    if "caller_removes_arguments" in c:
        rows += [("caller-cleans functions whose argument count is now known",
                  c.get("argument_count_from_callers", 0) + c.get("argument_count_from_frame", 0)),
                 ("functions taking arguments in registers", c.get("register_arguments", 0)),
                 ("functions returning nothing", c.get("result_none", 0)),
                 ("functions with a variable argument list", c.get("variadic", 0))]
    return ["what the decompiler's description could be told:"] + ["  %-58s %d" % r for r in rows] + [""]


def _value(v, img):
    """A fact's value as report text: addresses in hex, long lists cut."""
    if isinstance(v, bool) or v is None:
        return str(v)
    if isinstance(v, int):
        return "%#x" % v if v > 0xFFFF else str(v)
    if isinstance(v, dict):
        items = list(v.items())
        return "{" + ", ".join("%s: %s" % (k, _value(x, img)) for k, x in items[:40]) + (", ..." if len(items) > 40 else "") + "}"
    if isinstance(v, (list, tuple, set, frozenset)):
        items = sorted(v, key=repr) if isinstance(v, (set, frozenset)) else list(v)
        return "[" + ", ".join(_value(x, img) for x in items[:40]) + (", ..." if len(items) > 40 else "") + "]"
    return str(v)


def text(result, all_facts=False):
    """The report as text."""
    img = result["image"]
    lines = ["%s  (%s, %d-bit)  %s" % (result["module"], {"pe": "32-bit Windows module", "ne": "16-bit Windows module",
                                                         "le": "virtual device driver"}[result["kind"]],
                                      result["bits"], result["path"]),
             "%d functions, %d of them found from calls and held addresses beyond the decompiler's description"
             % (result["functions"], len(img.discovered)),
             "analysed in %.1f s (loading and decoding %.1f s)" % (result["seconds"]["total"], result["seconds"]["load"]), ""]
    lines += _feedback(result)
    for name, _m in ANALYSES:
        res = result["results"].get(name)
        if res is None:
            continue
        lines.append("== %s  (%.1f s)" % (name.replace("_", " "), result["seconds"][name]))
        counts = res["counts"]
        width = max(len(k) for k in counts) if counts else 0
        lines += ["  %-*s  %s" % (width, k.replace("_", " "), v) for k, v in counts.items()]
        if res.get("text"):
            lines.append("")
            lines += ["  " + t for t in res["text"]]
        if all_facts:
            lines.append("")
            for f in res["facts"]:
                where = img.label(f["addr"]) if f.get("addr") is not None else "(module)"
                rest = ", ".join("%s=%s" % (k, _value(v, img)) for k, v in f.items() if k not in ("addr", "kind", "name"))
                lines.append("  %-18s %-22s %s" % (f["kind"], where, rest))
        lines.append("")
    return "\n".join(lines)


def main(argv):
    """python -m codebreaker analyze <module> [--what a,b,c] [--out FILE] [--run N|all] [--facts]
    [--starts FILE] [--json] [--no-store] [--summary]"""
    args = list(argv)
    opts = {"what": None, "out": None, "run": None, "starts": None}
    flags = set()
    rest = []
    while args:
        a = args.pop(0)
        if a in ("--what", "--out", "--run", "--starts"):
            if not args:
                print("analyze: %s needs a value" % a)
                return 2
            opts[a[2:]] = args.pop(0)
        elif a in ("--facts", "--json", "--no-store", "--summary"):
            flags.add(a)
        elif a in ("-h", "--help"):
            print(__doc__)
            return 0
        else:
            rest.append(a)
    if len(rest) != 1:
        print(__doc__)
        return 2
    what = [w.strip() for w in opts["what"].split(",") if w.strip()] if opts["what"] else list(DEFAULT)
    limit = None
    if opts["run"] is not None:
        if "observe" not in what:
            what.append("observe")
        limit = None if opts["run"] == "all" else int(opts["run"])
    elif "observe" in what:
        limit = 100
    try:
        result = run(rest[0], what, store="--no-store" not in flags, observed=limit)
    except ValueError as e:
        print("analyze: %s" % e)
        return 1
    out = opts["out"] or settings.state("analyze", result["module"].replace("#", "-") + ".txt")
    os.makedirs(os.path.dirname(os.path.abspath(out)), exist_ok=True)
    with open(out, "w", encoding="utf-8", newline="\n") as f:
        f.write(text(result, all_facts="--facts" in flags))
    if opts["starts"]:
        with open(opts["starts"], "w", encoding="utf-8", newline="\n") as f:
            f.write("\n".join(starts(result["image"])) + "\n")
    if "--json" in flags:
        json.dump({k: result[k] for k in ("module", "kind", "functions", "counts", "seconds")}, sys.stdout, indent=1)
        print()
    elif "--summary" in flags:
        for name, counts in result["counts"].items():
            print("%-12s %5.1fs  %s" % (name, result["seconds"][name], ", ".join("%s %s" % (k.replace("_", " "), v)
                                                                               for k, v in list(counts.items())[:7])))
    else:
        print("%s: %d functions, %s; report in %s" % (
            result["module"], result["functions"],
            ", ".join("%s %.1fs" % (n, result["seconds"][n]) for n in result["counts"]), out))
    return 0
