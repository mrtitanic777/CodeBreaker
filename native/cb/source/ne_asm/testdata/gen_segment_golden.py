"""Goldens for native/cb/source/ne_asm/segment_test.cpp, made by the ORIGINAL codebreaker.source.ne_asm.segment (Python).

Needs the Python package (codebreaker) and a built formats.dll (CB_FORMATS_BIN names its folder), and objdump on the PATH:
    python native/cb/source/ne_asm/testdata/gen_segment_golden.py
Writes native/cb/source/ne_asm/testdata/: ORDINALS.md, scenario_a.rows, scenario_b.rows, scenario_c.rows, segment.golden.txt, real_lzexpand.golden.txt
"""
import contextlib
import io
import os
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "..", "..", ".."))
sys.path.insert(0, ROOT)
os.chdir(ROOT)
from codebreaker.source.ne_asm import fixups as F  # noqa: E402
from codebreaker.source.ne_asm import segment as S  # noqa: E402

OUT = os.path.join(ROOT, "native", "cb", "source", "ne_asm", "testdata")
os.makedirs(OUT, exist_ok=True)
LZ = os.path.join(ROOT, "native", "cb", "formats", "testdata", "lzexpand.dll")

ORDINALS = """# Ordinals of the Windows 3.1 system modules (a few of them, for the tests)

| Ordinal | Name | Notes |
|---|---|---|
| KERNEL.15 | `GlobalAlloc` | |
| KERNEL.16 | `GlobalReAlloc` | |
| KERNEL.17 | `GlobalFree` | |
| KERNEL.18 | `GlobalLock` | |
| KERNEL.19 | `GlobalUnlock` | |
| KERNEL.81 | `_lclose` | |
| KERNEL.82 | `_lread` | |
| KERNEL.83 | `_lcreat` | |
| KERNEL.84 | `_llseek` | |
| KERNEL.85 | `_lopen` | |
| KERNEL.86 | `_lwrite` | |
| LZEXPAND.15 | LZCopy | |
| USER.107 | DefWindowProc | |
| not a row | at all | |
"""

# a window procedure written as a compare chain, the dialog form and a handler that falls through
SCENARIO_A = """0\tpush   bp
1\tmov    bp,sp
3\tpush   si
4\tmov    ax,WORD PTR [bp+0xc]
7\tcmp    ax,0x1
a\tje     0x40
c\tcmp    ax,0x2
f\tjne    0x20
11\tjmp    0x50
14\tcmp    ax,0x10
17\tjne    0x20
19\tnop
1a\tnop
20\tcmp    WORD PTR [bp+0xc],0x110
25\tje     0x60
27\txor    ax,ax
29\tjmp    0x80
40\tmov    ax,0x1
43\tpush   ax
44\tcall   0x0:0x0
49\tpop    ax
4a\tjmp    0x80
50\tcall   0x90
53\tcall   0xc0
56\tret
60\tmov    ah,0x3d
62\tint    0x21
64\tmov    ah,0x99
66\tjmp    0x80
80\tpop    si
81\tpop    bp
82\tretf   0xa
90\tpush   bp
91\tmov    bp,sp
93\tmov    ax,WORD PTR [bp+0x6]
96\tmov    bx,WORD PTR [bp+0xa]
99\tpop    bp
9a\tret    0x4
a0\tenter  0x2,0x0
a4\tmov    bx,WORD PTR [bp+0x8]
a8\tcall   0x0:0x0
ad\tretf
c0\tmov    bx,0x1234
c3\tpush   bx
c4\tjmp    0x90
c7\tnop
c8\tint3
"""

# a window procedure written as a jump table, in segment 2 (the table's words are the module's own bytes at 0x20)
SCENARIO_B = """0	push   bp
1	mov    bp,sp
3	push   si
4	mov    ax,WORD PTR [bp+0xc]
7	sub    ax,0x1
a	cmp    ax,0x2
d	ja     0x40
f	add    ax,ax
11	mov    bx,ax
13	jmp    WORD PTR cs:[bx+0x20]
18	call   0x0:0x0
1d	ret
40	call   0x80
43	ret
46	call   0x0:0x0
4b	ret
80	mov    ax,0x1
83	retf
"""

# segment 1 of a module whose first two relocation records are rewritten to point inside the module:
# record 0 -> segment 2 offset 0x120 (a far reference), record 1 -> segment 1 offset 0x90 (an internal one)
SCENARIO_C = """0	push   bp
1	mov    bp,sp
20	call   0x0:0x0
52	call   0x0:0x0
70	jmp    0x0:0x0
8a	call   0x0:0x0
90	push   bp
91	mov    bp,sp
93	pop    bp
94	ret
a0	jmp    0x0:0x0
"""


def patched_module():
    data = bytearray(open(LZ, "rb").read())
    ne0 = F.open_ne(LZ)
    seg1 = ne0.segments()[0]
    first = seg1["file_off"] + seg1["length"] + 2
    for k, (segment, entry) in enumerate(((2, 0x0120), (1, 0x0090))):
        rec = first + 8 * k
        data[rec + 1] = 0                       # an internal reference to a fixed segment
        data[rec + 4], data[rec + 5] = segment, 0
        data[rec + 6], data[rec + 7] = entry & 0xFF, entry >> 8
    import tempfile
    with tempfile.TemporaryDirectory() as folder:
        path = os.path.join(folder, "lz_patched.dll")
        open(path, "wb").write(bytes(data))
        return F.open_ne(path)


def rows_of(text):
    return [(int(line.split("\t")[0], 16), line.split("\t")[1]) for line in text.strip("\n").split("\n")]


def write(name, text):
    with open(os.path.join(OUT, name), "w", encoding="utf-8", newline="\n") as f:
        f.write(text)


def captured(fn, *a):
    buf = io.StringIO()
    with contextlib.redirect_stdout(buf):
        fn(*a)
    return buf.getvalue()


write("ORDINALS.md", ORDINALS)
write("scenario_a.rows", SCENARIO_A)
write("scenario_b.rows", SCENARIO_B)
write("scenario_c.rows", SCENARIO_C)
ords = S.load_ordinals(os.path.join(OUT, "ORDINALS.md"))
ne = F.open_ne(LZ)

golden = []


def section(title, text):
    golden.append("@@@ " + title)
    golden.append(text.rstrip("\n"))


section("ordinals", "\n".join("%s=%s" % kv for kv in sorted(ords.items())))
for name, text, seg_index, module in (("a", SCENARIO_A, 1, ne), ("b", SCENARIO_B, 2, ne), ("c", SCENARIO_C, 1, patched_module())):
    rows = rows_of(text)
    S.disasm_whole = lambda code, base=0, _r=rows: list(_r)
    sg = S.Seg(module, seg_index, ords)
    section("%s starts" % name, " ".join("%x" % s for s in sorted(sg.starts)))
    section("%s funcs" % name, "\n".join("%x-%x %s" % (f["start"], f["end"], f["name"]) for _, f in sorted(sg.funcs.items())))
    section("%s exports" % name, "\n".join("%x %s" % kv for kv in sorted(sg.export_names.items())))
    for s in sorted(sg.funcs):
        section("%s function %x" % (name, s), captured(S.print_function, sg, s, True))
        disp = sg.dispatch(s)
        hs = sorted(h for _, h in disp)
        lines = ["dispatch %s" % disp, "args %s" % sg.args_used(s), "thunk %s" % sg.is_thunk(s), "calls %s" % sg.calls_in(s)]
        for val, h in sorted(disp):
            lines.append("handler %x: %s" % (h, sg.handler_calls(s, h, hs)))
        section("%s facts %x" % (name, s), "\n".join(lines))
    section("%s call graph 0" % name, " ".join("%x" % x for x in S.trace_callgraph(sg, 0)))
    section("%s cskel" % name, captured(S.cmd_cskel, module, ords, seg_index, 0))
    section("%s trace" % name, captured(S.cmd_trace, module, ords, seg_index, 0))
    section("%s wndprocs" % name, captured(S.cmd_wndprocs, module, ords, seg_index))
    section("%s resolve" % name, "\n".join("%x %s -> %s" % (o, a, sg.resolve_call(o, a)) for o, a in rows if a.startswith(("call", "jmp"))))
    section("%s annotate" % name, "\n".join(S.annotate_row(sg, o, a) for o, a in rows))
    section("%s word_at" % name, " ".join(str(sg.word_at(o)) for o in (0, 0x20, 0x1c6, 0x1c7, 0x2000)))

write("segment.golden.txt", "\n".join(golden) + "\n")

# the real module, with the real objdump (when the run that made this had one): the Python's own output
import importlib  # noqa: E402,F401
importlib.reload(S)
ords = S.load_ordinals(os.path.join(OUT, "ORDINALS.md"))
real = []
real.append("@@@ map\n" + captured(S.cmd_map, ne, ords).rstrip("\n"))
real.append("@@@ list 6\n" + captured(S.cmd_list, ne, ords, 6).rstrip("\n"))
real.append("@@@ list 3\n" + captured(S.cmd_list, ne, ords, 3).rstrip("\n"))
real.append("@@@ fn 6 a4\n" + captured(S.cmd_fn, ne, ords, 6, 0xA4).rstrip("\n"))
real.append("@@@ trace 3 0\n" + captured(S.cmd_trace, ne, ords, 3, 0).rstrip("\n"))
real.append("@@@ wndprocs\n" + captured(S.cmd_wndprocs, ne, ords, None).rstrip("\n"))
write("real_lzexpand.golden.txt", "\n".join(real) + "\n")
print("written to", OUT)
