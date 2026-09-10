#!/usr/bin/env python3
"""
ppcrun.py — end-to-end proof that recompiled PowerPC actually RUNS correctly.

Closes the whole loop and self-checks it, with no emulator oracle needed:

    C source --(XDK cl.exe, ppc_cc)--> real Xbox 360 PowerPC bytes
             --(our disasm_ppc + ppcrecomp)--> C++
             --(g++ link + execute)--> result
             == the C function computed natively?

If the executed recompiled function returns the same value as the original C
computed on the host, the pipeline (XDK compile -> our disassembly -> our
recompilation -> host compile -> run) is proven correct on real instructions.

    python ppcrun.py --selftest
    python ppcrun.py --expr "a*3 + b" --a 7 --b 5
"""
import argparse
import os
import subprocess
import sys
import tempfile

import ppc_cc
import ppc_obj
import ppcrecomp

GPP = "/c/msys64/mingw64/bin/g++.exe" if os.path.exists(
    "/c/msys64/mingw64/bin/g++.exe") else "g++"

# a leaf function: two int args (r3,r4) -> int (r3). /O2 keeps it register-only.
_TEMPLATE = "int f(int a, int b) {{ return {expr}; }}\n"

FUNC_VA = 0x82000000


def run_expr(expr, a, b):
    """Compile `int f(int a,int b){return <expr>;}` with the XDK, recompile it,
    run it with a,b, and return the executed result (or raise)."""
    csrc = _TEMPLATE.format(expr=expr)
    r = ppc_cc.compile_c(csrc, flags="/c /O2")
    if "obj" not in r:
        raise RuntimeError("XDK compile failed:\n" + r.get("log", "")[:400])
    fns = ppc_obj.Obj(r["obj"]).functions()
    if "f" not in fns:
        raise RuntimeError("function f not in object; have: %s" % list(fns))
    body, _mask = fns["f"]
    cpp, called, stats = ppcrecomp.recompile_function(body, FUNC_VA, FUNC_VA, len(body))
    if called:
        raise RuntimeError("not a leaf (calls %s) — pick a simpler expr"
                           % [hex(c) for c in called])
    if stats["unimpl"]:
        raise RuntimeError("expr uses instructions not yet translated "
                           "(%d unimplemented) — see disasm" % stats["unimpl"])

    harness = f"""#include "rex_ppc_min.h"
#include <cstdio>
#include <cstdint>
void ppc_call_indirect(PPCContext&, uint8_t*, uint32_t) {{}}

{cpp}

int main() {{
    static uint8_t mem[1 << 20];
    uint8_t* base = mem;
    PPCContext ctx{{}};
    ctx.r3.s64 = {a};
    ctx.r4.s64 = {b};
    ctx.r1.u32 = 0x8000;          // a stack pointer inside mem (for any prologue)
    sub_{FUNC_VA:08X}(ctx, base);
    std::printf("%lld\\n", (long long)ctx.r3.s32);
    return 0;
}}
"""
    with tempfile.TemporaryDirectory() as td:
        with open(os.path.join(td, "rex_ppc_min.h"), "w") as f:
            f.write(ppcrecomp.runtime_header())
        src = os.path.join(td, "run.cpp")
        with open(src, "w") as f:
            f.write(harness)
        exe = os.path.join(td, "run.exe")
        c = subprocess.run([GPP, "-std=c++20", "-O2", "-I", td, "-o", exe, src],
                           capture_output=True, text=True)
        if c.returncode != 0:
            raise RuntimeError("g++ failed:\n" + c.stderr[:800])
        out = subprocess.run([exe], capture_output=True, text=True, timeout=30)
        return int(out.stdout.strip())


# to-int32 helper so Python matches C's 32-bit wraparound semantics
def _i32(x):
    x &= 0xFFFFFFFF
    return x - (1 << 32) if x & 0x80000000 else x


CASES = [
    ("a + b", 7, 5),
    ("a * 3 + b", 7, 5),
    ("a * b", 6, 7),
    ("(a << 4) - b", 10, 3),
    ("(a & b) | 7", 0x33, 0x14),
    ("a - b * 2", 100, 9),
    ("a * a + b * b", 8, 6),
]


def _native(expr, a, b):
    a = _i32(a); b = _i32(b)
    return _i32(eval(expr, {"a": a, "b": b}))    # trusted: our own CASES only


def _recompile_leaf(csrc, va):
    """Compile a one-function C leaf with the XDK and recompile it at guest
    address `va`. Returns the C++ text (defining sub_<va>)."""
    r = ppc_cc.compile_c(csrc, flags="/c /O2")
    if "obj" not in r:
        raise RuntimeError("XDK compile failed:\n" + r.get("log", "")[:400])
    fns = ppc_obj.Obj(r["obj"]).functions()
    name = next(n for n in fns if not n.startswith("."))
    body, _ = fns[name]
    cpp, called, st = ppcrecomp.recompile_function(body, va, va, len(body))
    if called or st["unimpl"]:
        raise RuntimeError("leaf %s not clean (calls=%s unimpl=%d)"
                           % (name, called, st["unimpl"]))
    return cpp


def run_call_graph():
    """Prove the RUNTIME + indirect-call table: two functions recompiled at
    distinct guest addresses, linked together, dispatched INDIRECTLY through the
    address-keyed function table (the bctrl / vtable mechanism). Returns
    (executed, expected)."""
    A, B = 0x82001000, 0x82002000
    dbl = _recompile_leaf("int dbl(int x){ return x*2; }\n", A)
    inc = _recompile_leaf("int inc(int x){ return x+10; }\n", B)

    x0 = 5
    expected = _i32(x0 * 2 + 10)      # inc(dbl(5)) via two indirect calls

    harness = f"""#include "rex_ppc_min.h"
#include <cstdio>
#include <cstdint>

{dbl}
{inc}

// --- the runtime indirect-call table (RexGlue PPCFuncMappings analogue) ---
struct PPCFuncMapping {{ uint32_t addr; PPCFunc* func; }};
static PPCFuncMapping g_table[] = {{
    {{ 0x{A:08X}, sub_{A:08X} }},
    {{ 0x{B:08X}, sub_{B:08X} }},
    {{ 0, nullptr }},
}};
void ppc_call_indirect(PPCContext& ctx, uint8_t* base, uint32_t addr) {{
    for (auto* m = g_table; m->func; ++m)
        if (m->addr == addr) {{ m->func(ctx, base); return; }}
    std::fprintf(stderr, "missing indirect target 0x%08X\\n", addr);  // ref logs+noops
}}

int main() {{
    static uint8_t mem[1 << 20];
    uint8_t* base = mem;
    PPCContext ctx{{}};
    ctx.r1.u32 = 0x8000;
    ctx.r3.s64 = {x0};
    // indirect dispatch: ctr = &dbl; bctrl  -> then ctr = &inc; bctrl
    ctx.ctr.u32 = 0x{A:08X};
    ppc_call_indirect(ctx, base, ctx.ctr.u32);   // r3 = dbl(5) = 10
    ctx.ctr.u32 = 0x{B:08X};
    ppc_call_indirect(ctx, base, ctx.ctr.u32);   // r3 = inc(10) = 20
    std::printf("%lld\\n", (long long)ctx.r3.s32);
    return 0;
}}
"""
    with tempfile.TemporaryDirectory() as td:
        with open(os.path.join(td, "rex_ppc_min.h"), "w") as f:
            f.write(ppcrecomp.runtime_header())
        src = os.path.join(td, "cg.cpp")
        with open(src, "w") as f:
            f.write(harness)
        exe = os.path.join(td, "cg.exe")
        c = subprocess.run([GPP, "-std=c++20", "-O2", "-I", td, "-o", exe, src],
                           capture_output=True, text=True)
        if c.returncode != 0:
            raise RuntimeError("g++ failed:\n" + c.stderr[:800])
        out = subprocess.run([exe], capture_output=True, text=True, timeout=30)
        return int(out.stdout.strip()), expected


def _selftest():
    print("ppcrun selftest — recompiled PowerPC executes and matches native\n")
    if not ppc_cc.available():
        print("XDK compiler not found — set: XboxTranspiler config xdk <path>")
        return False
    ok = True
    for expr, a, b in CASES:
        try:
            got = run_expr(expr, a, b)
            want = _native(expr, a, b)
            good = (got == want)
            ok = ok and good
            print("  f(a,b)=%-14s a=%-4d b=%-4d -> ran %-8d want %-8d  %s"
                  % (expr, a, b, got, want, "OK" if good else "MISMATCH"))
        except Exception as e:
            ok = False
            print("  f(a,b)=%-14s ERROR: %s" % (expr, str(e)[:80]))

    # runtime + indirect-call table: two functions linked, dispatched via the table
    try:
        got, want = run_call_graph()
        good = (got == want)
        ok = ok and good
        print("\n  indirect-call table: inc(dbl(5)) via 2 table dispatches -> "
              "ran %d want %d  %s" % (got, want, "OK" if good else "MISMATCH"))
    except Exception as e:
        ok = False
        print("\n  indirect-call table: ERROR: %s" % str(e)[:120])

    print("\nSELFTEST:", "PASS — recompiled code runs correctly" if ok else "FAIL")
    return ok


def main():
    ap = argparse.ArgumentParser(description="run recompiled PowerPC end-to-end")
    ap.add_argument("--expr", help="a C expression in a,b (e.g. 'a*3+b')")
    ap.add_argument("--a", type=int, default=7)
    ap.add_argument("--b", type=int, default=5)
    ap.add_argument("--selftest", action="store_true")
    args = ap.parse_args()
    if args.selftest or not args.expr:
        sys.exit(0 if _selftest() else 1)
    got = run_expr(args.expr, args.a, args.b)
    want = _native(args.expr, args.a, args.b)
    print("ran = %d ; native = %d ; %s" % (got, want,
          "MATCH" if got == want else "MISMATCH"))
    sys.exit(0 if got == want else 1)


if __name__ == "__main__":
    main()
