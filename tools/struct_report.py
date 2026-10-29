"""Structure recovery (docs/CORE_ROADMAP.md, Track 1 item C), measured on the golden corpus.

    python tools/struct_report.py [NAME ...]

Runs the decompiler with CB_STRUCTS=1 on each golden module's frozen description (tools/golden.py)
and compares with the frozen output:

- structures recovered, their fields, the functions given one;
- accesses `*(type *)(p + offset)` left in the C, before and after;
- functions that fail, before and after;
- agreement with the SDK: an exported function whose header declares a parameter as a pointer to
  a structure the SDK defines (symbols/prototypes.py, symbols/sdk_structs.py) -- each field
  recovered for that parameter is checked against the SDK's layout: a field of the SDK structure
  starts at that offset and has that size.

The report is written to state/struct_report/struct_report.txt; each module's output to
state/struct_report/modules/.
"""
import json
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)

from codebreaker.decompile import decompiler                 # noqa: E402
from codebreaker.symbols import prototypes, sdk_structs      # noqa: E402

GOLDEN = os.path.join(ROOT, "state", "golden")
HOME = os.path.join(ROOT, "state", "struct_report")
OUT = os.path.join(HOME, "modules")
DEREF = re.compile(r"\*\([\w ]+\*+\)\(\(?\w+\)? ?\w* \+ (?:0x)?[0-9a-f]+\)")
NOTE = re.compile(r"^/\*@S (\S+) (\d+) ((?:[0-9a-f]+:\d+:\S+ ?)*)\*/$", re.M)


def blocks(text):
    """{va: (name, body)} of a decompiler output"""
    out = {}
    for b in text.split("/*@@ ")[1:]:
        head, _, body = b.partition("*/\n")
        name, va = head.split()[:2]
        out[int(va, 16)] = (name, body)
    return out


def run(name, desc):
    os.makedirs(OUT, exist_ok=True)
    path = os.path.join(OUT, name + ".txt")
    env = dict(os.environ, CB_LANGUAGES=decompiler._languages(), CB_STRUCTS="1",
               CB_THREADS=os.environ.get("CB_THREADS", "4"), CB_MEMORY_MB=os.environ.get("CB_MEMORY_MB", "4000"))
    with open(path, "w", encoding="utf-8", errors="replace") as f:
        subprocess.run([decompiler._exe(), desc], stdout=f, stderr=subprocess.DEVNULL, env=env)
    return open(path, encoding="utf-8", errors="replace").read()


def sdk_check(fname, struct_name, fields, bits):
    """-> (fields agreeing, fields disagreeing, SDK structure name) or None when the header says
    nothing about this parameter"""
    m = re.match(r"s_.+_a(\d+)$", struct_name)
    if not m or bits != 32:
        return None
    proto = prototypes.of(fname, bits=bits)
    if not proto:
        return None
    k = int(m.group(1)) - 1
    params = proto.get("parameters") or []
    if k >= len(params):
        return None
    target = sdk_structs.pointed_to(params[k][0])
    lay = sdk_structs.layout(target) if target else None
    if lay is None:
        return None
    agree = sum(1 for off, size in fields if lay.at(off, size) or _element(lay, off, size))
    return agree, len(fields) - agree, target


def _element(lay, off, size):
    """Is there an array field with an element of `size` bytes starting at `off`?"""
    for foff, n, _name, ctype in lay.fields:
        m = re.search(r"\[(\d+)\]$", ctype)
        if m and foff <= off < foff + n:
            elem = n // int(m.group(1))
            return elem == size and (off - foff) % elem == 0
    return False


def main(names):
    names = names or sorted(os.listdir(GOLDEN))
    lines, total = [], {"structs": 0, "fields": 0, "funcs": 0, "before": 0, "after": 0,
                        "fail0": 0, "fail1": 0, "agree": 0, "disagree": 0, "params": 0, "whole": 0}
    sdk_rows = []
    for name in names:
        gpath = os.path.join(GOLDEN, name, "golden.json")
        if not os.path.exists(gpath):
            continue
        member = next(iter(json.load(open(gpath, encoding="utf-8")).values()))   # (one per module)
        frozen = member["functions"]
        desc_path = member["description"]
        desc = open(desc_path, encoding="utf-8", errors="replace").read()
        bits = 32 if re.search(r"^arch\tx86:LE:32", desc, re.M) else 16
        text = run(name, desc_path)
        now = blocks(text)
        st = {"structs": 0, "fields": 0, "funcs": 0, "before": 0, "after": 0, "fail0": 0, "fail1": 0}
        for va, (fname, body) in now.items():
            notes = NOTE.findall(body)
            if notes:
                st["funcs"] += 1
            for sname, _size, flds in notes:
                fields = [(int(o, 16), int(n)) for o, n, _t in (x.split(":", 2) for x in flds.split())]
                st["structs"] += 1
                st["fields"] += len(fields)
                r = sdk_check(fname, sname, fields, bits)
                if r:
                    total["params"] += 1
                    total["agree"] += r[0]
                    total["disagree"] += r[1]
                    total["whole"] += r[1] == 0
                    sdk_rows.append("  %-9s %-28s %-14s %d of %d fields at the SDK's offsets" %
                                    (name, fname, r[2], r[0], r[0] + r[1]))
            st["after"] += len(DEREF.findall(body))
            st["fail1"] += "decompile failed" in body
        for k, body in frozen.items():
            st["before"] += len(DEREF.findall(body))
            st["fail0"] += "decompile failed" in body
        for k in st:
            total[k] += st[k]
        lines.append("%-9s structures %4d (fields %5d) in %4d functions; `*(T *)(p + n)` %5d -> %5d; failures %d -> %d"
                     % (name, st["structs"], st["fields"], st["funcs"], st["before"], st["after"], st["fail0"], st["fail1"]))
        print(lines[-1], flush=True)
    lines.append("%-9s structures %4d (fields %5d) in %4d functions; `*(T *)(p + n)` %5d -> %5d; failures %d -> %d"
                 % ("TOTAL", total["structs"], total["fields"], total["funcs"], total["before"], total["after"],
                    total["fail0"], total["fail1"]))
    lines.append("")
    lines.append("SDK agreement (32-bit exports whose header declares the parameter a pointer to an SDK structure): "
                 "%d parameters, %d wholly agreeing; fields %d agree, %d do not" %
                 (total["params"], total["whole"], total["agree"], total["disagree"]))
    lines += sdk_rows
    report = "\n".join(lines) + "\n"
    os.makedirs(HOME, exist_ok=True)
    with open(os.path.join(HOME, "struct_report.txt"), "w", encoding="utf-8") as f:
        f.write(report)
    print(report)


if __name__ == "__main__":
    main(sys.argv[1:])
