"""Runs the Python windows16 on real 16-bit modules and writes what the C++ test compares: native/cb/emulate/testdata/windows16.golden.json.

Run from the main checkout (it has the native DLLs):
    python -B native/cb/emulate/testdata/make_windows16_golden.py <worktree>
"""
import json
import sys

sys.path.insert(0, '.')
from codebreaker.emulate import windows16 as w
from codebreaker.emulate import emulator

wt = sys.argv[1]
LZ = wt + '/native/cb/formats/testdata/lzexpand.dll'
VER = wt + '/native/cb/emulate/testdata/ver.dll'
COMM = wt + '/native/cb/emulate/testdata/comm.drv'

M64 = (1 << 64) - 1


def fold(values):
    """FNV-1a over the numbers (each as 8 bytes, little endian): the checksum the C++ test recomputes."""
    h = 0xCBF29CE484222325
    for v in values:
        for k in range(8):
            h ^= (v >> (8 * k)) & 0xFF
            h = (h * 0x100000001B3) & M64
    return h


def summary(rec):
    out = {
        "exit": rec.get("exit"), "ret": rec.get("ret"), "ret32": rec.get("ret32"),
        "insn_count": rec.get("insn_count"), "truncated": rec.get("truncated"),
        "sp_delta": rec.get("sp_delta"), "n_imports": rec.get("n_imports"),
        "entry_regs": rec.get("entry_regs"), "exit_regs": rec.get("exit_regs"),
        "path_len": len(rec.get("path") or []), "path_fold": fold(rec.get("path") or []),
        "path_head": (rec.get("path") or [])[:12],
        "writes_len": len(rec.get("mem_writes") or []),
        "writes_fold": fold([v for x in (rec.get("mem_writes") or []) for v in (x["addr"], x["size"], x["value"])]),
        "writes_head": [[x["addr"], x["size"], x["value"]] for x in (rec.get("mem_writes") or [])][:6],
        "reads_len": len(rec.get("mem_reads") or []),
        "reads_fold": fold([x["addr"] for x in (rec.get("mem_reads") or [])]),
        "imports": [[h.get("api"), h.get("n"), h.get("args_stack"), h.get("ret_ip"), h.get("ret_cs"),
                     h.get("arg_bytes"), bool(h.get("unmodelled")), h.get("kind"), h.get("slot"),
                     sorted((k, v.hex()) for k, v in (h.get("pointed") or {}).items())]
                    for h in (rec.get("imports_hit") or [])],
        "calls_out": [[c["name"], c["args"], c["sp"], c["n"], c["ret_ip"]] for c in (rec.get("calls_out") or [])],
        "unsure": rec.get("unsure"), "interrupt": rec.get("interrupt"),
        "fault": rec.get("fault"), "heap_sels": sorted(rec.get("heap_sels", {}).items()),
        "lazy_maps": len(rec.get("lazy_maps") or []), "memops": rec.get("memops"),
        "prelude_ran": rec.get("prelude_ran"),
    }
    if "layout" in rec:
        out["segment_bases"] = [rec["layout"]["seg_bases"][k] for k in sorted(rec["layout"]["seg_bases"])]
        out["data_base"] = rec["layout"]["ds_base"]
    if rec.get("branches") is not None:
        out["branches"] = [[b[0], b[1], list(b[2]) if b[2] else None] for b in rec["branches"]]
    if rec.get("io") is not None:
        out["io"] = [list(e) for e in rec["io"]]
    return out


cases = []


def case(name, module, **kw):
    path = {"lz": LZ, "ver": VER, "comm": COMM}[module]
    conv = {k: v for k, v in kw.items()}
    try:
        rec = w.probe(path, **conv)
    except Exception as e:
        cases.append({"name": name, "module": module, "error": type(e).__name__})
        return
    cases.append({"name": name, "module": module, "result": summary(rec)})


# every export of lzexpand and ver with zeros, and with a few words
for module, count in (("lz", 24), ("ver", 14)):
    for ordinal in range(1, count + 1):
        case("%s zero ordinal %d" % (module, ordinal), module, ordinal=ordinal, args=[0, 0, 0, 0])
        case("%s words ordinal %d" % (module, ordinal), module, ordinal=ordinal, args=[1, 2, 3, 4, 5, 6])
case("lz bad ordinal", "lz", ordinal=999, args=[])
case("lz near", "lz", seg=2, offset=610, near=True, args=[1, 2])
case("lz callee", "lz", ordinal=3, args=[0, 0, 0, 0], callees={(2, 610): ("LZStart", 4, False), (4, 348): ("Other", 8, True)})
case("lz unknown callee size", "lz", ordinal=3, args=[0, 0, 0, 0], callees={(2, 610): ("LZStart", -1, True)})
case("lz cdecl", "lz", ordinal=7, args=[1, 2, 3], cdecl=True)
case("lz stub_ret", "lz", ordinal=3, args=[0, 0, 0, 0], stub_ret=0x12345678)
case("lz data", "lz", ordinal=3, args=[0, 0, 0, 0], data_image=bytes(range(64)), data_end=(64, 0x300))
case("lz mem_init", "lz", ordinal=3, args=[0, 0, 0, 0], mem_init=[(("D", 8), b"hello"), (0x120000, b"zz"), (("D", 0xFFFE), b"toolong")])
case("lz bp", "lz", ordinal=3, args=[0, 0, 0, 0], bp=0x1234)
case("lz cap", "lz", ordinal=3, args=[0, 0, 0, 0], insn_cap=40)
case("lz branches", "lz", ordinal=3, args=[0, 0, 0, 0], branches=(0, 0x800))
case("lz branches 2", "lz", ordinal=7, args=[1, 2, 3, 4], branches=(0, 0x400))
case("lz prelude", "lz", ordinal=3, args=[0, 0, 0, 0], prelude=[{"ordinal": 7, "args": [1]}, {"seg": 2, "offset": 0, "args": [0, 0, 0, 0]}])
for ordinal in range(1, 40):
    case("comm ports ordinal %d" % ordinal, "comm", ordinal=ordinal, args=[0, 1, 2, 3], ports=True)


def descriptor(base, limit, access):
    d = bytearray(8)
    d[0], d[1] = limit & 0xFF, (limit >> 8) & 0xFF
    d[2], d[3], d[4], d[7] = base & 0xFF, (base >> 8) & 0xFF, (base >> 16) & 0xFF, (base >> 24) & 0xFF
    d[5] = access
    return bytes(d)


# A recorded call replayed: its stack, registers and selectors (an LDT of them) ...
regs = {"AX": 0x1111, "BX": 0x2222, "CX": 0x3333, "DX": 0x4444, "SI": 0x5555, "DI": 0x6666, "BP": 0x7777}
ctx1 = {"stack": bytes(range(256)) * 256, "sp": 0xFFD0, "ss": 0x4F, "ds": 0x57, "es": 0x5F, "regs": regs,
        "ldt": {0x57: (b"\x11" * 100, False), 0x5F: (b"\x22" * 100, True)}, "data_sel": 0x57, "key": "ctx1"}
case("lz context", "lz", ordinal=3, context=ctx1)
case("lz context near", "lz", seg=2, offset=610, near=True, context=ctx1)
# ... and with the whole of memory at the call
mem = bytearray(((i * 7) & 0xFF) for i in range(0x6000))
for i, (base, limit, access) in enumerate([(0, 0, 0), (0x100, 0xFFF, 0x92), (0x1000, 0xFFF, 0x9A), (0x2000, 0xFFF, 0x92)]):
    mem[0x4000 + 8 * i:0x4000 + 8 * i + 8] = descriptor(base, limit, access)
mem[0x1020:0x1023] = bytes([0xCA, 0x04, 0x00])
ctx2 = {"stack": bytes(0x10000), "sp": 0xFFE0, "ss": (1 << 3) | 7, "ds": (3 << 3) | 7, "es": (1 << 3) | 7, "regs": regs,
        "ldt": {}, "data_sel": (3 << 3) | 7, "snapshot": bytes(mem), "ldt_at": (0x4000, 8 * 8 - 1), "own": {}, "key": "ctx2"}
case("lz context snapshot", "lz", ordinal=3, context=ctx2)
case("lz context snapshot callees", "lz", ordinal=2, args=[], context=ctx2, callees={(2, 0): ("A", 4, True)})

# the data segment as loaded
relocated = {}
for module, path in (("lz", LZ), ("ver", VER), ("comm", COMM)):
    data = w.relocated_data(path)
    relocated[module] = {"len": len(data), "fold": fold(list(data))}

with open(wt + '/native/cb/emulate/testdata/windows16.golden.json', 'w') as f:
    f.write('{"relocated": ' + json.dumps(relocated, sort_keys=True) + ',\n"cases": [\n'
            + ",\n".join(json.dumps(c, sort_keys=True) for c in cases) + "\n]}\n")
print(len(cases), "cases;", sum(1 for c in cases if "error" in c), "errors")
