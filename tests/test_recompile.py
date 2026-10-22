#!/usr/bin/env python3
"""The recompiler (codebreaker/recompile) end to end: recompiled PowerPC runs and gives the
answers the C it was compiled from gives.

    python tests/test_recompile.py

Each case is C compiled with the Xbox 360 compiler (settings.json: powerpc_tools), its PowerPC
recompiled to C++ (recompile/powerpc.py), built with g++ against the kernel core's runtime headers,
run, and compared with the same computation done here. Also: two functions dispatched through
the indirect-call table, a function reading big-endian guest memory, VMX128 words decoding first,
and (when the game is on this machine) a whole image recompiled with nothing left untranslated
but padding. SKIPPED without the compiler or g++."""
import os
import random
import struct
import subprocess
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)

from codebreaker.recompile import disassemble, powerpc    # noqa: E402

VA = 0x82000000
GAME = os.path.join(ROOT, "Projects", "TetrisPC", "work", "default_unc.xex")
RESULTS = []


class Skip(Exception):
    pass


def check(fn):
    try:
        fn()
        RESULTS.append((fn.__name__, "ok", ""))
    except Skip as s:
        RESULTS.append((fn.__name__, "SKIP", str(s)))
    except Exception as e:
        RESULTS.append((fn.__name__, "FAIL", "%s: %s" % (type(e).__name__, e)))


def _tools():
    from codebreaker.build import functions
    tools = functions.powerpc_tools()
    if not tools:
        raise Skip("no PowerPC compiler set up")
    if not powerpc.compiler():
        raise Skip("no g++")
    return tools[0]


def leaf(csrc, name, va):
    """Compile one C function with the Xbox 360 compiler and recompile it at `va`."""
    from codebreaker.verify import match_powerpc
    built = match_powerpc._compile(_tools(), "", csrc, name, tempfile.mkdtemp(prefix="cb_rc_"), "/O2")
    if built is None:
        raise RuntimeError("the Xbox 360 compiler rejected %s" % name)
    body = built[0]
    cpp, called, st = powerpc.recompile_function(body, va, va, len(body))
    if st["unimpl"]:
        raise RuntimeError("%s: %d instructions not translated" % (name, st["unimpl"]))
    return cpp, called


def run(harness):
    """Build and run a harness against the runtime a generated project uses (the rexglue kernel
    core's headers: its PPCContext, FPSCR, SIMD layer and traps are all inline). -> its output lines"""
    with tempfile.TemporaryDirectory() as td:
        with open(os.path.join(td, "rex_ppc_min.h"), "w") as f:
            f.write(powerpc.runtime_header())
        src, exe = os.path.join(td, "t.cpp"), os.path.join(td, "t.exe")
        with open(src, "w") as f:
            f.write(harness)
        inc = powerpc.kernel_includes()
        if not inc:
            raise Skip("the rexglue kernel core is not in the tree (native/rexglue/kernel)")
        cmd = [powerpc.compiler(), "-std=c++23", "-O1", "-w", "-DFMT_HEADER_ONLY", "-I", td]
        for d in inc:
            cmd += ["-I", d]
        c = subprocess.run(cmd + ["-o", exe, src], capture_output=True, text=True)
        if c.returncode:
            raise RuntimeError("g++: " + c.stderr[:600])
        return subprocess.run([exe], capture_output=True, text=True, timeout=60).stdout.split()


def _i32(x):
    x &= 0xFFFFFFFF
    return x - (1 << 32) if x & 0x80000000 else x


INT_CASES = [("a + b", 7, 5), ("a * 3 + b", 7, 5), ("a * b", 6, 7), ("(a << 4) - b", 10, 3),
             ("(a & b) | 7", 0x33, 0x14), ("a - b * 2", 100, 9), ("a * a + b * b", 8, 6),
             ("a / b + a % b", -37, 5), ("(unsigned)a >> (b & 31)", -1, 7), ("a > b ? a - b : b - a", 3, 11)]


def test_integer_leaves():
    for expr, a, b in INT_CASES:
        cpp, _called = leaf("int f(int a, int b) { return %s; }\n" % expr, "f", VA)
        out = run('#include "rex_ppc_min.h"\n#include <cstdio>\nvoid ppc_call_indirect(PPCContext&, uint8_t*, uint32_t) {}\n'
                  + cpp + "\nint main() { static uint8_t mem[1 << 20]; PPCContext ctx{}; ctx.r1.u32 = 0x8000; "
                  "ctx.r3.s64 = %d; ctx.r4.s64 = %d; sub_%08X(ctx, mem); std::printf(\"%%d\\n\", ctx.r3.s32); }\n"
                  % (a, b, VA))
        want = _i32(_python(expr, a, b))
        assert int(out[0]) == want, (expr, a, b, out, want)


def _python(expr, a, b):
    """The C expression's value, with C's 32-bit wrap, truncating division and unsigned shift."""
    if expr == "a / b + a % b":
        q = int(a / b)
        return q + (a - b * q)
    if expr == "(unsigned)a >> (b & 31)":
        return (a & 0xFFFFFFFF) >> (b & 31)
    if expr == "a > b ? a - b : b - a":
        return a - b if a > b else b - a
    return eval(expr, {"a": a, "b": b})


def test_floating_point():
    cpp, _c = leaf("float f(float a, float b) { return a * b - a * a; }\n", "f", VA)
    out = run('#include "rex_ppc_min.h"\n#include <cstdio>\nvoid ppc_call_indirect(PPCContext&, uint8_t*, uint32_t) {}\n'
              + cpp + "\nint main() { static uint8_t mem[1 << 20]; PPCContext ctx{}; ctx.r1.u32 = 0x8000; "
              "ctx.f1.f64 = 2.5; ctx.f2.f64 = 4.0; sub_%08X(ctx, mem); std::printf(\"%%.4f\\n\", ctx.f1.f64); }\n" % VA)
    assert abs(float(out[0]) - 3.75) < 1e-6, out   # (no constants: the harness maps no data)


def test_loop_over_memory():
    """A loop summing big-endian words of guest memory: the loads byte-swap as the console's do."""
    cpp, _c = leaf("int f(const int *p, int n) { int s = 0; while (n-- > 0) s += *p++; return s; }\n", "f", VA)
    words = [3, -7, 1000, 123456, 5]
    init = "".join("mem[0x1000 + %d] = %d; " % (4 * i + k, b) for i, w in enumerate(words)
                   for k, b in enumerate(struct.pack(">i", w)))
    out = run('#include "rex_ppc_min.h"\n#include <cstdio>\nvoid ppc_call_indirect(PPCContext&, uint8_t*, uint32_t) {}\n'
              + cpp + "\nint main() { static uint8_t mem[1 << 20]; %s PPCContext ctx{}; ctx.r1.u32 = 0x8000; "
              "ctx.r3.u32 = 0x1000; ctx.r4.s64 = %d; sub_%08X(ctx, mem); std::printf(\"%%d\\n\", ctx.r3.s32); }\n"
              % (init, len(words), VA))
    assert int(out[0]) == sum(words), out


def test_indirect_call_table():
    a, b = 0x82001000, 0x82002000
    dbl, _c = leaf("int dbl(int x) { return x * 2; }\n", "dbl", a)
    inc, _c = leaf("int inc(int x) { return x + 10; }\n", "inc", b)
    out = run('#include "rex_ppc_min.h"\n#include <cstdio>\n' + dbl + inc + """
struct Mapping { uint32_t addr; PPCFunc* func; };
static Mapping table[] = {{0x%08X, sub_%08X}, {0x%08X, sub_%08X}, {0, nullptr}};
void ppc_call_indirect(PPCContext& ctx, uint8_t* base, uint32_t addr) {
    for (auto* m = table; m->func; ++m) if (m->addr == addr) { m->func(ctx, base); return; }
}
int main() { static uint8_t mem[1 << 20]; PPCContext ctx{}; ctx.r1.u32 = 0x8000; ctx.r3.s64 = 5;
    ctx.ctr.u32 = 0x%08X; ppc_call_indirect(ctx, mem, ctx.ctr.u32);
    ctx.ctr.u32 = 0x%08X; ppc_call_indirect(ctx, mem, ctx.ctr.u32);
    std::printf("%%d\\n", ctx.r3.s32); }
""" % (a, a, b, b, a, b))
    assert int(out[0]) == 20, out


def test_vmx128_first():
    """A VMX128 word is read as VMX128, not as the later instruction a stock decoder makes of it."""
    word = 0x102320C3                                   # lvx128 v1,r3,r4
    got = next(iter(disassemble.listing(struct.pack(">I", word), VA, count=1)))
    assert got[2:] == ("lvx128", "v1,r3,r4"), got
    stmts = powerpc.emit_insn(VA, "lvx128", "v1,r3,r4", {}, VA, VA + 4, word)
    assert stmts and not any("UNIMPLEMENTED" in s for s in stmts), stmts


def test_whole_image():
    if not os.path.isfile(GAME):
        raise Skip("no game image here")
    body, _mapping, agg = powerpc.recompile_image(GAME, jobs=4)
    left = set(__import__("re").findall(r'PPC_UNIMPLEMENTED\("([^"]*)"', body))
    assert all(x.startswith(".long 0x00000000") for x in left), sorted(left)[:5]   # (only padding)
    assert agg["funcs"] >= agg["discovered"] > 9000      # (plus the tail-jump entries)
    head, _, rest = body.partition("\n\n")
    decls, _, funcs = rest.partition("\n\nPPC_FUNC_IMPL")
    funcs = ["PPC_FUNC_IMPL" + f for f in ("PPC_FUNC_IMPL" + funcs)[len("PPC_FUNC_IMPL"):].split("\nPPC_FUNC_IMPL")]
    sample = random.Random(7).sample(funcs, 150) + [f for f in funcs if "simde_mm" in f][:50]
    ok, why = powerpc.check_compiles(head + "\n\n" + decls + "\n\n" + "\n".join(dict.fromkeys(sample)))
    if ok is None:
        raise Skip(why)
    assert ok, why[:400]


def main():
    for fn in (test_integer_leaves, test_floating_point, test_loop_over_memory, test_indirect_call_table,
               test_vmx128_first, test_whole_image):
        check(fn)
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    fails = sum(1 for r in RESULTS if r[1] == "FAIL")
    print("%d passed, %d skipped, %d failed" % (
        sum(1 for r in RESULTS if r[1] == "ok"), sum(1 for r in RESULTS if r[1] == "SKIP"), fails))
    return 1 if fails else 0


def test_discover_built_addresses():
    """Functions reached only by an address the code builds (lis + addi), and leaf functions
    starting right after another one ends, are found."""
    if not os.path.isfile(GAME):
        raise Skip("no game image here")
    from codebreaker.recompile import discover
    funcs, sources, _img = discover.discover(GAME)
    assert sources["constants"] > 0, sources
    # the CRT's FlsAlloc stand-in, two callbacks it registers, and a device setter after a blr
    for a in (0x82164700, 0x82131380, 0x82107278, 0x82178348):
        assert a in funcs, hex(a)


if __name__ == "__main__":
    sys.exit(main())
