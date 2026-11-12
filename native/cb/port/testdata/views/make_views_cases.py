"""make_views_cases.py -- the golden data of native/cb/port/views_test.cpp, old_c_text_test.cpp and program_prep_test.cpp.

Runs the ORIGINAL Python (codebreaker.port.pre_ansi.Port) over small hand-written programs (one for each pass of
views.hpp and the constructor's own mending) and over the real trees of testdata/old_c, and writes for each case
  testdata/views/<case>/originals/<f>   the sources as given
  testdata/views/<case>/ctor/<f>        (synthetic cases) the files the Port holds after __init__
  testdata/views/<case>/expected.json   what the Port made of them: the constructor's results, each text pass on each
                                        file, and what offset_views / pass_through_params / bss_ends did inside modernize()
  testdata/views/<case>/state.json      the Port's functions and parameter types at the first offset_views call
Usage: python make_views_cases.py
The expected values were read through by hand for the synthetic cases (see the comments of CASES): the Python is the
oracle, and where a pass is a plain bug of the original (a stale offset) the case says so.
It re-runs itself with PYTHONHASHSEED=0 (the port's own passes iterate sets of names).
"""
import collections
import json
import os
import re
import sys
import zlib

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", "..", "..", ".."))
sys.path.insert(0, ROOT)
from codebreaker.base.ctext import blank                                  # noqa: E402
from codebreaker.port import pre_ansi                                     # noqa: E402

RUNTIMES = os.path.join(ROOT, "codebreaker", "port", "runtimes")
OLD_C = os.path.join(HERE, "..", "old_c")

# each case: files; every case runs the dos_msc3 runtime
CASES = {
    # an anonymous structure a header's member is, given by value (a parameter), and one given by a pointer: a tag each
    "anon": {
        "members": {"rec.n": "char *"},
        "files": {
            "rec.h": "struct rec {\n\tstruct { char kind, flags; } hd;\n\tint n;\n};\n",
            "a.c": '#include "rec.h"\nfa(h)\nstruct { char kind, flags; } h;\n{\n\treturn h.kind;\n}\n'
                   "fb(p)\nregister struct { int a; int b; } *p;\n{\n\treturn p->a;\n}\n",
        }},
    # struct pnode { *kid[2]; int pa; }: the array one longer, the word gone (kid[i] is indexed past 1, pa never named)
    "tails": {
        "files": {
            "node.h": "struct pnode {\n\tstruct pnode *kid[2];\n\tint pa;\n};\n",
            "a.c": '#include "node.h"\nwalk(p, i)\nstruct pnode *p;\nint i;\n{\n\treturn p->kid[i] != 0;\n}\n',
        }},
    # NEXT(p) = q: the first word of an isym its link
    "link": {
        "files": {
            "isym.h": "struct isym {\n\tint link;\n\tchar kind;\n};\n",
            "a.c": '#include "isym.h"\n#define NEXT(s) (*(struct isym **)(s))\nchain(p, q)\nstruct isym *p, *q;\n{\n\tNEXT(p) = q;\n\treturn 0;\n}\n',
        }},
    # int (*fn)() given mk, a function of char *: char *(*fn)()  (the file starts with a comment: its first line is blank)
    "fnptr": {
        "files": {
            "a.c": "/* a */\nchar *mk(n)\nint n;\n{\n\treturn 0;\n}\nrun(fn, k)\nint (*fn)();\nint k;\n{\n\treturn (*fn)(k) != 0;\n}\n"
                   "main()\n{\n\trun(mk, 3);\n\treturn 0;\n}\n",
        }},
    # struct opt (b.c) is struct flagdef (a.h): one cast to the other: the words that are pointers there
    "cast": {
        "files": {
            "a.h": "struct flagdef {\n\tchar *pat;\n\tint *var;\n};\n",
            "a.c": '#include "a.h"\nuse(p)\nstruct flagdef *p;\n{\n\treturn *p->var;\n}\n',
            "b.c": "struct opt {\n\tchar *pat;\n\tint o2;\n};\nstruct opt o;\nmain()\n{\n\tuse(&o);\n\treturn 0;\n}\n",
        }},
    # struct macro (b.c) the bytes of struct mac (mac.h), ints where mac has pointers: a function it declares gives a mac
    "twin": {
        "files": {
            "mac.h": "struct mac {\n\tstruct mac *next;\n\tchar *name;\n\tint hash;\n};\n",
            "b.c": '#include "mac.h"\nstruct macro {\n\tint f0;\n\tint f2;\n\tint f4;\n};\nstruct macro *getmac();\n'
                   "use()\n{\n\tstruct macro *m;\n\tm = getmac();\n\treturn m->f0;\n}\n",
            "c.c": '#include "mac.h"\nstruct mac *\ngetmac()\n{\n\treturn 0;\n}\n',
        }},
    # top and v one after another are struct stk's members: one variable
    "views": {
        "files": {
            "a.c": "struct stk {\n\tint top;\n\tchar *v[20];\n};\nint top = 20;\nchar *v[20];\npush(p)\nchar *p;\n{\n\tchar **q;\n"
                   "\tq = &top;\n\tv[0] = p;\n\treturn 0;\n}\n",
        }},
    # a table's (int *) column given pointers' places (buf) and numbers' (cnt)
    "columns": {
        "files": {
            "a.c": 'struct ent {\n\tchar *name;\n\tint *val;\n};\nchar *buf;\nint cnt;\nstruct ent tab[] = {\n\t{ "a", (int *)&buf },\n'
                   '\t{ "b", &cnt },\n};\n',
        }},
    # the constructor's own mending: a structure over a union's view, over another's first bytes, a far pointer, the 8086's words
    "misc": {
        "files": {
            "s.h": "struct sym {\n\tchar *name;\n\tint kind;\n\tchar *link;\n};\nstruct usym {\n\tint kind;\n\tunion {\n"
                   "\t\tstruct { int a_old; int b_old; } rfl;\n\t\tchar c;\n\t} u;\n};\n#define NEAR near pascal\n",
            "a.c": '#include "s.h"\nstruct comsym {\n\tchar head[4];\n\tint count;\n};\nstruct rfld {\n\tint x;\n\tint y;\n};\n'
                   "int NEAR bump(sym)\nstruct sym *sym;\n{\n\treturn ((struct comsym *)sym)->count;\n}\n"
                   "g(us)\nstruct usym *us;\n{\n\tstruct rfld *r;\n\tchar far *fp;\n\tr = (struct rfld *)&us->u.rfl;\n"
                   "\tFP_SEG(fp) = 0x40;\n\tFP_OFF(fp) = 0x10;\n\treturn r->x;\n}\n"
                   "/* an empty stand-in */\n",
            "stdio.h": "int x;\n",
            "same.c": "/* SAMEFILE */\nint lost(a, b)\n{\n}\nint kept;\n",
        }},
    # (char *)&g_100a: the place after the char array g_1000, the array's end
    "bss": {
        "files": {
            "a.c": "char g_1000[10];\nint g_100a;\nf()\n{\n\tchar *p;\n\tp = (char *)&g_100a;\n\treturn 0;\n}\n",
        }},
    # relay(p) only gives p on: every caller gives a struct b, so p is one (pass_through_params)
    "passthru": {
        "files": {
            "t.h": "struct a {\n\tint x;\n};\nstruct b {\n\tint y;\n};\n",
            "a.c": '#include "t.h"\nrelay(p)\nstruct a *p;\n{\n\treturn use(p);\n}\n'
                   "top()\n{\n\tstruct b *q;\n\treturn relay(q);\n}\n",
        }},
    # a file's own view of another structure by its old places (offset_views): lab over targ
    "offsets": {
        "files": {
            "t.h": "struct targ {\n\tint kind;\n\tchar *name;\n\tint flags;\n};\nstruct sym {\n\tstruct targ *type;\n\tint cls;\n};\n",
            "x.c": '#include "t.h"\nstruct lab {\n\tint a;\n\tchar *b;\n\tint c;\n};\nf(s)\nstruct sym *s;\n{\n\tstruct lab *p;\n'
                   "\tp = (struct lab *)s->type;\n\treturn p->a + p->c;\n}\n",
            "y.c": '#include "t.h"\ng(s)\nstruct sym *s;\n{\n\treturn s->cls;\n}\n',
        }},
}

REAL = {"msc-3.00-p3": "dos_msc3", "msc-3.00-p0": "dos_msc3"}


def profile(name):
    return json.load(open(os.path.join(RUNTIMES, name + ".json")))


def crc(t):
    return [len(t), zlib.crc32(t.encode("utf-8")) & 0xFFFFFFFF]


def write(path, text):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    open(path, "w", encoding="utf-8", newline="").write(text)


def notes_of(d):
    return [[k, list(v)] for k, v in d.items()]


def mapping_json(m):
    out = []
    for k, v in m.items():
        out.append([k, ["pun", v[1], v[2], v[3]] if isinstance(v, tuple) else ["names", list(v)]])
    return out


def run(name, files, prof_name, members, real):
    prof = profile(prof_name)
    files = dict(sorted(files.items()))          # (the order the tests read a folder in)
    base = os.path.join(HERE, name)
    if not real:                                 # (the real trees' sources are old_c/<name>/originals)
        for n, t in files.items():
            write(os.path.join(base, "originals", n), t)
    p = pre_ansi.Port(dict(files), prof, dict(members or {}))
    names = sorted({"g_" + h for t in files.values() for h in re.findall(r"\bg_([0-9a-f]{4})\b", t)})
    p.ds = {n: int(n[2:], 16) for n in names}            # (as old_c/make_cases.py: the variables named by their place)
    p.bss = {n: 10 for n in names}
    exp = {"profile": prof_name, "members": members or {}}
    exp["ctor"] = {
        "kept": list(p.files), "left_out": [[k, v] for k, v in p.left_out.items()],
        "notes": notes_of(p.notes), "union_notes": notes_of(p.union_notes), "member_fixes": list(p.member_fixes),
        "far_near": [[k, v] for k, v in getattr(p, "notes_init", {}).items()],
        "aliases": [[k, v[0], v[1]] for k, v in p.aliases.items()], "view_vars": sorted(getattr(p, "view_vars", ())),
        "view_headers": sorted(p.view_headers), "renames": sorted(p.renames.items()), "int16": p.int16,
        "originals": {n: crc(t) for n, t in p.originals.items()},
        "files": {n: crc(t) for n, t in p.files.items()}}
    if not real:
        for n, t in p.files.items():
            write(os.path.join(base, "ctor", n), t)
    # the text passes of old_c_text.hpp on each file the port keeps
    lay_cache = {}
    texts = {}
    for n in sorted(p.files):
        t = p.files[n]
        rec = {}
        nt = collections.defaultdict(list)
        r = pre_ansi.widths(t, blank(t), nt, n, p.int16)[0]
        rec["widths"] = [crc(r), notes_of(nt)]
        nt = collections.defaultdict(list)
        r = pre_ansi.formats(t, blank(t), nt, n, p.int16, prof.get("printf_hex_upper", False))
        rec["formats"] = [crc(r), notes_of(nt)]
        if n.endswith(".h"):
            nt = collections.defaultdict(list)
            r, defs = pre_ansi.header_data(t, blank(t), n, nt)
            rec["header_data"] = [crc(r), defs, notes_of(nt)]
        known = set(prof["libc"]) | {f.name for f, s, j, k in pre_ansi.definitions(t.split("\n"), blank(t).split("\n"))}
        for other in p.files.values():
            known |= {f.name for f, s, j, k in pre_ansi.definitions(other.split("\n"), blank(other).split("\n"))}
        nt = collections.defaultdict(list)
        r = pre_ansi.old_declarations(t, blank(t), known, nt, n, list(prof["platform"]["calls"]))
        rec["old_declarations"] = [crc(r), notes_of(nt)]
        nt = collections.defaultdict(list)
        r = pre_ansi.mem_sizes(t, p.old_var_sizes(), p.old_elem_sizes(), nt, n)
        rec["mem_sizes"] = [crc(r), notes_of(nt)]
        nt = collections.defaultdict(list)
        r = pre_ansi.header_offsets(t, p.old_layouts_for(n), nt, n)
        rec["header_offsets"] = [crc(r), notes_of(nt)]
        nt = collections.defaultdict(list)
        pre_ansi.POOLS.clear()
        r = pre_ansi.struct_sizes(t, p.old_layouts_for(n), nt, n, p.allocators())
        rec["struct_sizes"] = [crc(r), notes_of(nt), sorted([k, sorted(v)] for k, v in pre_ansi.POOLS.items())]
        texts[n] = rec
    exp["text_passes"] = texts
    pre_ansi.POOLS.clear()
    # modernize, with pass_through_params, offset_views and bss_ends watched
    calls = []
    st = {}
    orig_ov, orig_bss, orig_pt = p.offset_views, p.bss_ends, p.pass_through_params

    def spy_pt():
        if "state" not in st:
            st["state"] = snapshot(p)
            st["before"] = (dict(p.param_types), {k: list(v) for k, v in p.mended.items()})
        orig_pt()

    def spy_ov(t, fname):
        if "state2" not in st:                   # (the functions as the loop typed them by now)
            st["state2"] = snapshot(p)
        r = orig_ov(t, fname)
        calls.append(["offset_views", fname, crc(t), crc(r)])
        if not real:
            write(os.path.join(base, "in", "offset_views", fname), t)
            write(os.path.join(base, "out", "offset_views", fname), r)
        return r

    def spy_bss(t, fname):
        r = orig_bss(t, fname)
        calls.append(["bss_ends", fname, crc(t), crc(r)])
        if not real:
            write(os.path.join(base, "in", "bss_ends", fname), t)
            write(os.path.join(base, "out", "bss_ends", fname), r)
        return r
    p.pass_through_params, p.offset_views, p.bss_ends = spy_pt, spy_ov, spy_bss
    p.modernize()
    before_types, before_mended = st["before"]
    exp["modernize"] = {
        "calls": calls,
        "views": [[x, v[0], v[2], mapping_json(v[1])] for x, v in getattr(p, "_views", {}).items()],
        "views_weak": sorted(getattr(p, "_views_weak", ())), "views_new": sorted(getattr(p, "_views_new", ())),
        "need_headers": [[k, sorted(v)] for k, v in sorted(p.need_headers.items())],
        "view_wide": list(getattr(p, "notes_view_wide", [])), "puns_refused": list(getattr(p, "view_puns_refused", [])),
        "param_types_new": [[k[0], k[1], v] for k, v in p.param_types.items() if before_types.get(k) != v],
        "mended_new": [[k, v[len(before_mended.get(k, [])):]] for k, v in p.mended.items()
                       if v[len(before_mended.get(k, [])):]],
        "copied": sorted(p.copied_members()), "ptr2": sorted(p.ptr2_names())}
    post = {}
    for n in sorted(p.files):                    # (each file, as the port has it, through the passes in the state they left)
        post[n] = [crc(orig_ov(p.files[n], n)), crc(orig_bss(p.files[n], n))]
    exp["modernize"]["post"] = post
    write(os.path.join(base, "state.json"), json.dumps({"pass_through": st.get("state", {}), "offset_views": st.get("state2", {})}, indent=1))
    write(os.path.join(base, "expected.json"), json.dumps(exp, indent=1))
    print(name, "views:", list(getattr(p, "_views", {})), "calls:", len(calls), "pass_through:",
          len(exp["modernize"]["param_types_new"]))


def snapshot(p):
    base = {n: t for n, t in p.originals.items()}
    changed = {n: t for n, t in p.files.items() if t != base.get(n)}
    fns = []
    for f in p.fns.values():
        fns.append({"name": f.name, "ret": f.ret, "params": [list(x) for x in f.params], "static": f.static,
                    "line": f.line, "void": f.void, "loose": f.loose, "bare": f.bare,
                    "varargs": getattr(f, "varargs", False), "va_slots": getattr(f, "va_slots", False),
                    "ret_set": getattr(f, "ret_set", False), "int_ret": getattr(f, "int_ret", False),
                    "keep_int": getattr(f, "keep_int", False), "implicit": sorted(getattr(f, "implicit", ())),
                    "file": getattr(f, "file", None), "body": getattr(f, "body", "")})
    return {"files_changed": changed, "kept": list(p.files), "fns": fns, "view_vars": sorted(getattr(p, "view_vars", ())),
            "view_headers": sorted(p.view_headers),
            "param_types": [[k[0], k[1], v] for k, v in p.param_types.items()],
            "bss": [[k, v] for k, v in p.bss.items()], "aliases": [[k, v[0], v[1]] for k, v in p.aliases.items()]}


def main():
    if os.environ.get("PYTHONHASHSEED") != "0":
        import subprocess
        sys.exit(subprocess.call([sys.executable] + sys.argv, env=dict(os.environ, PYTHONHASHSEED="0")))
    for name, spec in CASES.items():
        run("syn-" + name, spec["files"], "dos_msc3", spec.get("members"), False)
    for name, prof in REAL.items():
        d = os.path.join(OLD_C, name, "originals")
        files = {n: open(os.path.join(d, n), encoding="utf-8", newline="").read() for n in sorted(os.listdir(d))}
        run(name, files, prof, None, True)


if __name__ == "__main__":
    main()
