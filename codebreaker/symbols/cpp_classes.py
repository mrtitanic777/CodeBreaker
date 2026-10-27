"""cpp_classes.py -- the C++ classes of a PowerPC (Xbox 360) program, from Microsoft's run-time type
information: each class's bases, its virtual function tables, and the class that introduces each
virtual function.

    python -m codebreaker classes <program.xex or image> [-o classes.json]
    from codebreaker.symbols import cpp_classes
    c = cpp_classes.read(image_path)
    c.classes            {name: {"bases": [...], "vtables": [{"address", "offset", "for", "slots"}], ...}}
    c.functions          {address: (class, "vf3" / "vf3_off8")}: virtual functions, by the class that introduces them
    c.found              [(class, vtable, slot, address)]: functions no table of starts knew, which the layout requires

A program built with RTTI (dynamic_cast, typeid) keeps, for every polymorphic class:

    TypeDescriptor             { type_info's vftable, 0, ".?AVName@@" }
    CompleteObjectLocator      { 0, offset, constructor-displacement offset, TypeDescriptor*, ClassHierarchyDescriptor* }
    ClassHierarchyDescriptor   { 0, attributes, number of bases, BaseClassDescriptor** } (the class first)
    BaseClassDescriptor        { TypeDescriptor*, contained bases, mdisp, pdisp, vdisp, attributes }

and every virtual function table is preceded by a pointer to its locator. Nothing is guessed: a
table's slot is taken while it points at a known function start, or where the C++ layout requires
it -- a derived class's table for a base sub-object holds every slot of that base's table -- and
those slots are reported as found functions. A function is named by the one class that introduces
it (every other class whose tables hold it derives from that one); a function several unrelated
classes share (a compiler-merged body) keeps no class name.
"""
import json
import os
import sys
from collections import defaultdict

from codebreaker.symbols import demangle


class Classes(object):
    pass


def read(path):
    from codebreaker.formats import powerpc_image, xex
    img = powerpc_image.read(xex.basefile(path) if xex.is_xex(path) else path)
    return from_image(img)


def from_image(img):
    secs = {s[0].strip("\0"): (s[1], s[1] + s[2]) for s in img.sections}
    rdata = secs.get(".rdata", (0, 0))
    text = secs.get(".text", img.code)
    data_ranges = [r for n, r in secs.items() if n in (".data", ".rdata")]
    starts = set(img.functions) | set(img.extents)

    def u32(a):
        w = img.word(a)
        return 0 if w is None else w

    def s32(a):
        v = u32(a)
        return v - (1 << 32) if v & 0x80000000 else v

    def inside(a, r):
        return r[0] <= a < r[1]

    # 1. type descriptors: the name 8 bytes in, after type_info's vftable and a zero word; the
    #    vftable every real descriptor shares is type_info's
    tds = {}
    for lo, hi in data_ranges:
        block = img.bytes_at(lo, hi - lo)
        p = 0
        while True:
            i = block.find(b".?A", p)
            if i < 0:
                break
            end = block.find(b"\0", i)
            if end < 0:
                break
            p = end
            td = lo + i - 8
            if td % 4 or u32(td + 4) != 0:
                continue
            tds[td] = {"mangled": block[i:end].decode("ascii", "replace"), "vftable": u32(td)}
    if not tds:
        c = Classes()
        c.classes, c.functions, c.found = {}, {}, []
        return c
    counts = defaultdict(int)
    for t in tds.values():
        counts[t["vftable"]] += 1
    type_info = max(counts, key=counts.get)
    tds = {a: dict(t, name=demangle.type_name(t["mangled"]) or t["mangled"]) for a, t in tds.items()
           if t["vftable"] == type_info}

    # 2. hierarchy descriptors and object locators (.rdata)
    def hierarchy(a):
        if not inside(a, rdata) or a % 4 or u32(a) != 0:
            return None
        attrs, n, array = u32(a + 4), u32(a + 8), u32(a + 12)
        if not 1 <= n <= 256 or not inside(array, rdata) or attrs & ~3:
            return None
        bases = []
        for k in range(n):
            bcd = u32(array + 4 * k)
            if not inside(bcd, rdata) or u32(bcd) not in tds:
                return None
            bases.append({"name": tds[u32(bcd)]["name"], "contained": u32(bcd + 4), "mdisp": s32(bcd + 8),
                          "pdisp": s32(bcd + 12), "vdisp": s32(bcd + 16), "attributes": u32(bcd + 20)})
        return {"attributes": attrs, "bases": bases}
    locators = {}
    for a in range(rdata[0], rdata[1] - 20, 4):
        if u32(a) != 0 or u32(a + 12) not in tds:
            continue
        h = hierarchy(u32(a + 16))
        if h is None or h["bases"][0]["name"] != tds[u32(a + 12)]["name"]:
            continue
        locators[a] = {"offset": u32(a + 4), "td": u32(a + 12), "chd": u32(a + 16), "hierarchy": h}

    # 3. tables: a word pointing at a locator, then function pointers
    pointers = {a for a in range(rdata[0], rdata[1] - 4, 4) if u32(a) in locators}
    classes = {}
    for loc in locators.values():
        name = tds[loc["td"]]["name"]
        classes.setdefault(name, {"name": name, "mangled": tds[loc["td"]]["mangled"],
                                  "bases": loc["hierarchy"]["bases"][1:], "vtables": []})

    def scan(q, slots):
        while q < rdata[1] and q not in pointers and u32(q) in starts:
            slots.append(u32(q))
            q += 4
        return q
    for a in sorted(pointers):
        loc = locators[u32(a)]
        c = classes[tds[loc["td"]]["name"]]
        slots = []
        scan(a + 4, slots)
        owner = None
        if loc["offset"]:
            owner = next((b["name"] for b in c["bases"] if b["pdisp"] == -1 and b["mdisp"] == loc["offset"]), None)
        c["vtables"].append({"address": a + 4, "offset": loc["offset"], "for": owner, "slots": slots})
    for c in classes.values():
        c["vtables"].sort(key=lambda v: (v["offset"], v["address"]))

    # 4. the layout rule: a derived class's table for a base sub-object is at least that base's table
    def nonvirtual(cls):
        return {0} | {b["mdisp"] for b in cls["bases"] if b["pdisp"] == -1}

    def required(c, v):
        need = 1                                    # a class has a table only for a virtual function
        for b in c["bases"]:
            base = classes.get(b["name"])
            if b["pdisp"] != -1 or base is None:
                continue
            for bv in base["vtables"]:
                if bv["offset"] in nonvirtual(base) and b["mdisp"] + bv["offset"] == v["offset"]:
                    need = max(need, len(bv["slots"]))
        return need
    found, changed = [], True
    while changed:
        changed = False
        for c in classes.values():
            for v in c["vtables"]:
                need = required(c, v)
                q = v["address"] + 4 * len(v["slots"])
                while len(v["slots"]) < need:
                    t = u32(q)
                    if q in pointers or not inside(t, text):
                        break                       # the layout and the image disagree: stop, guess nothing
                    if t not in starts:
                        found.append((c["name"], v["address"], len(v["slots"]), t))
                        starts.add(t)
                    v["slots"].append(t)
                    q += 4
                    changed = True
                scan(q, v["slots"])

    # 5. names: the class that introduces each virtual function
    occurrences = defaultdict(list)
    for c in classes.values():
        for v in c["vtables"]:
            for s, f in enumerate(v["slots"]):
                occurrences[f].append((c["name"], v["offset"], s))
    bases_of = {n: {b["name"] for b in c["bases"]} | {n} for n, c in classes.items()}
    functions = {}
    for f, occ in occurrences.items():
        roots = {cn for cn, _o, _s in occ if all(cn in bases_of[d] for d, _o2, _s2 in occ)}
        root_occ = {(o[1], o[2]) for o in occ if o[0] in roots}
        if len(roots) == 1 and len(root_occ) == 1:
            off, s = next(iter(root_occ))
            functions[f] = (next(iter(roots)), "vf%d" % s if off == 0 else "vf%d_off%X" % (s, off))
    c = Classes()
    c.classes, c.functions, c.found = classes, functions, found
    return c


def main(argv):
    if not argv or argv[0].startswith("-"):
        print(__doc__.split("\n\n")[0] + "\n\n" + __doc__.split("\n\n")[1])
        return 2
    c = read(argv[0])
    print("%s: %d classes, %d virtual function tables, %d virtual functions named by class, %d functions found by layout" % (
        os.path.basename(argv[0]), len(c.classes), sum(len(x["vtables"]) for x in c.classes.values()),
        len(c.functions), len(c.found)))
    deepest = sorted(c.classes.values(), key=lambda x: -len(x["bases"]))[:8]
    for x in deepest:
        print("  %-40s bases: %s" % (x["name"][:40], ", ".join(b["name"] for b in x["bases"])[:100]))
    if "-o" in argv:
        out = argv[argv.index("-o") + 1]
        with open(out, "w", encoding="utf-8") as f:
            json.dump({"classes": sorted(c.classes.values(), key=lambda x: x["name"]),
                       "functions": {"%08X" % a: list(v) for a, v in sorted(c.functions.items())},
                       "found": [list(x) for x in c.found]}, f, indent=1)
        print("  -> %s" % out)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
