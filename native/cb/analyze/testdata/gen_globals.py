"""gen_globals.py <outdir> <module>... -- run from the repo root, with the Python package and cbanalyze.dll built (the oracle for
globals_test.cpp's golden test; goes with the Python).

For each module writes <outdir>/<name>.globals.json: what cbanalyze.dll answered to globals.variables and globals.tables
(the live engine native/analyze/globals.cc), and the report the live Python made of them (counts and text).
"""
import json
import os
import sys

sys.path.insert(0, ".")
from codebreaker.analyze import globals as global_variables, image, strings  # noqa: E402
from codebreaker.base import corpus, settings  # noqa: E402

out = sys.argv[1]
os.makedirs(out, exist_ok=True)
for m in sys.argv[2:]:
    name, _, member = m.partition("#")
    path = corpus.resolve(name)
    kind = image.kind_of(path)
    work = settings.state("analyze", "work", os.path.basename(path).lower() + ("-" + member if member else ""))
    os.makedirs(work, exist_ok=True)
    img = image._load_pe(path) if kind == "pe" else image._load_ne(path, work) if kind == "ne" else image._load_le(path, member, work)
    img.discover()
    strings.table(img)
    h = img._handle()
    h.names(img)
    variables = h.ask("globals.variables")
    tables = h.ask("globals.tables")
    report = global_variables.analyse(img)
    with open(os.path.join(out, os.path.basename(path).lower() + ".globals.json"), "w") as f:
        json.dump({"variables": variables, "tables": tables, "counts": report["counts"], "text": report["text"]}, f,
                  separators=(",", ":"))
    print(name, len(variables), len(tables))
