"""gen_goldens.py <outdir> <module>... -- run from the repo root, with the Python package and cbanalyze.dll built (the oracle for golden_test.cpp; goes with the Python).

For each module writes <outdir>/<name>.image (the blob the DLL is opened with), <name>.names (the names after
discover) and <name>.answers.json (every answer the DLL gave, by question, in the order they were asked).
"""
import json
import os
import sys

sys.path.insert(0, ".")
from codebreaker.analyze import call_graph, image, native, resources, strings  # noqa: E402

out = sys.argv[1]
os.makedirs(out, exist_ok=True)


def dash(v):
    return "-" if v is None else str(v)


def canonical_op(o):
    if o[0] == "r":
        return "r:%s:%d" % (o[1], o[2])
    if o[0] == "i":
        return "i:%d:%d" % (o[1], o[2])
    return "m:%s|%s|%d|%d|%s|%d" % (dash(o[1]), dash(o[2]), o[3], o[4], dash(o[5]), o[6])


def canonical(row):
    """One decoded function as text the C++ test builds the same way from its own Function."""
    start, limit, end, insns, tables = row
    lines = ["F %d %d %d" % (start, limit, end)]
    for addr, size, mn, text, ops, rd, wr, flow, target in insns:
        lines.append("%d %d %s|%s|%s|%s|%s|%d|%s" % (addr, size, mn, text, ";".join(canonical_op(o) for o in ops),
                                                    ",".join(sorted(rd)), ",".join(sorted(wr)), flow, dash(target)))
    for t in tables:
        lines.append("T %d %d %d %s|%s|%d|%s|%s" % (t["site"], t["table"], t["low"], t["selector"], dash(t["default"]),
                                                   1 if t["bounded"] else 0, ",".join(str(x) for x in t["targets"]),
                                                   "-" if t["bytes"] is None else ",".join(str(x) for x in t["bytes"])))
    return "\n".join(lines) + "\n"


def digest(row):
    import zlib
    text = canonical(row)
    return [row[0], row[1], row[2], len(row[3]), len(row[4]), zlib.crc32(text.encode("utf-8"))]
answers = []
real_ask = native.Handle.ask


def ask(self, question, arg=0):
    got = real_ask(self, question, arg)
    answers.append([question, arg, got])
    return got


native.Handle.ask = ask
for m in sys.argv[2:]:
    del answers[:]
    name, _, member = m.partition("#")
    from codebreaker.base import corpus, settings  # noqa: E402
    path = corpus.resolve(name)
    kind = image.kind_of(path)
    work = settings.state("analyze", "work", os.path.basename(path).lower() + ("-" + member if member else ""))
    os.makedirs(work, exist_ok=True)
    img = image._load_pe(path) if kind == "pe" else image._load_ne(path, work) if kind == "ne" else image._load_le(path, member, work)
    blob = native._blob(img)
    img.discover()
    names = native._names(img)
    g = call_graph.build(img)
    strings.table(img)
    strings.uses(img)
    strings.stack_strings(img)
    strings.formats(img)
    resources.uses(img)
    stem = os.path.basename(path).lower()
    with open(os.path.join(out, stem + ".image"), "wb") as f:
        f.write(blob)
    with open(os.path.join(out, stem + ".names"), "wb") as f:
        f.write(names)
    # an answer asked twice is the same answer: keep the first of each question
    seen, kept = set(), []
    for q, a, got in answers:
        if (q, a) in seen:
            continue
        seen.add((q, a))
        kept.append([q, a, got])
    for entry in kept:
        if entry[0] == "image.discover":     # every decoded instruction would be megabytes: keep a digest of each function
            entry[2]["decoded"] = [digest(row) for row in entry[2]["decoded"]]
    with open(os.path.join(out, stem + ".answers.json"), "w") as f:
        json.dump({"kind": img.kind, "bits": img.bits, "answers": kept}, f, separators=(",", ":"))
    d = {q: got for q, a, got in kept}
    n = {"fn": len(d["image.discover"]["functions"]), "tables": sum(r[4] for r in d["image.discover"]["decoded"]),
         "edges": len(d["call_graph.build"]["calls"]), "refs": len(d["call_graph.build"]["refs"]),
         "str": len(d["strings.table"]), "uses": len(d["strings.uses"]), "stack": len(d["strings.stack"]),
         "fmt": len(d["strings.formats"]["rows"]), "own": len(d["strings.formats"]["own"]),
         "loads": len(d["resources.uses"]["loads"]), "ctl": len(d["resources.uses"]["controls"]),
         "cyc": len(d["call_graph.build"]["cycles"]), "disc": len(d["image.discover"]["discovered"])}
    t = [a for q, _x, a in kept if q == "strings.table"]
    print(stem, img.kind, img.bits, "fn", len(img.functions), "blob", len(blob), n)
