#!/usr/bin/env python3
"""
ppcboot.py — the runtime scaffold toward whole-program recompilation.

Last steps proved recompiled functions run (ppcrun) and dispatch through the
indirect-call table. To run a *program* you also need the runtime that surrounds
the recompiled code — this module stands up the load-bearing pieces and proves
the most fundamental one against real data:

  * **Guest memory + image load.** Allocate a flat guest-memory buffer, load the
    decoded XEX image into it at its virtual addresses (the loaded/flat layout),
    and set `base` so the recompiled `PPC_LOAD/STORE` macros address real guest
    memory. Proven self-checkingly: recompile `int deref(int* p){return *p;}`
    (which is `lwz r3,0(r3); blr` — a leaf, no relocations), run it against the
    loaded Tetris image, and confirm `deref(addr)` returns the exact big-endian
    word that lives at `addr` in the image.

  * **Host-shim registry + setjmp/longjmp** live in the generated `rex_runtime.h`
    (host-shim registration macro; `ppc_setjmp`/`ppc_longjmp`) — the scaffold a
    full port fills with xboxkrnl/XAM/CRT implementations. Emitted and compiled
    here; a `--scaffold-check` proves it builds.

    python ppcboot.py --selftest            # load real image, deref real data
    python ppcboot.py default.xex --deref 0x82080000
"""
import argparse
import os
import struct
import subprocess
import sys
import tempfile

import ppc_cc
import ppc_obj
import ppcrecomp

GPP = "/c/msys64/mingw64/bin/g++.exe" if os.path.exists(
    "/c/msys64/mingw64/bin/g++.exe") else "g++"

FUNC_VA = 0x82000000     # label for the recompiled deref (not placed in guest mem)


def runtime_header_ext():
    """rex_ppc_min.h plus the runtime scaffold: a host-shim registry for imports
    and the setjmp/longjmp host helpers (the MinecraftPC crash lesson)."""
    base = ppcrecomp.runtime_header()
    scaffold = r"""
// ------------------------------------------------------------------ runtime
// Host-shim registry: guest imports (xboxkrnl/XAM/CRT) are NOT recompiled; the
// recompiler emits __imp__Name(ctx, base) and a host C++ implementation is
// registered under that name. A full port fills this with real handlers.
#include <cstdio>
struct PPCFuncMapping { uint32_t addr; PPCFunc* func; };
extern PPCFuncMapping PPCFuncMappings[];      // provided by the generated *_init

inline PPCFunc* ppc_lookup(uint32_t addr) {
    for (PPCFuncMapping* m = PPCFuncMappings; m->func; ++m)
        if (m->addr == addr) return m->func;
    return nullptr;
}
inline void ppc_call_indirect_impl(PPCContext& ctx, uint8_t* base, uint32_t addr) {
    if (PPCFunc* f = ppc_lookup(addr)) { f(ctx, base); return; }
    std::fprintf(stderr, "[rex] missing indirect target 0x%08X\n", addr); // ref: log+noop
}

// setjmp/longjmp interception. Recompiling the guest setjmp/longjmp as ordinary
// functions corrupts r1/r31 (MinecraftPC). Declaring their addresses makes the
// emitter call these host helpers instead, which save/restore the guest context.
struct PPCJmpBuf { PPCContext saved; int set; };
inline void ppc_setjmp(PPCContext& ctx, uint8_t* base) {
    // guest: r3 = jmp_buf*. Save the volatile-preserving subset + return 0.
    PPCJmpBuf* jb = reinterpret_cast<PPCJmpBuf*>(base + ctx.r3.u32);
    jb->saved = ctx; jb->set = 1;
    ctx.r3.s64 = 0;
}
inline void ppc_longjmp(PPCContext& ctx, uint8_t* base) {
    // guest: r3 = jmp_buf*, r4 = value. Restore and return value from setjmp.
    PPCJmpBuf* jb = reinterpret_cast<PPCJmpBuf*>(base + ctx.r3.u32);
    int val = ctx.r4.s32 ? ctx.r4.s32 : 1;
    ctx = jb->saved;
    ctx.r3.s64 = val;
}
"""
    return base + scaffold


def _recompile_deref():
    src = "int deref(int* p) { return *p; }\n"
    r = ppc_cc.compile_c(src, flags="/c /O2")
    if "obj" not in r:
        raise RuntimeError("XDK compile failed:\n" + r.get("log", "")[:400])
    fns = ppc_obj.Obj(r["obj"]).functions()
    name = next(n for n in fns if not n.startswith("."))
    body, _ = fns[name]
    cpp, called, st = ppcrecomp.recompile_function(body, FUNC_VA, FUNC_VA, len(body))
    if called or st["unimpl"]:
        raise RuntimeError("deref not a clean leaf: %s" % st)
    return cpp, body


def boot_deref(xex_path, guest_addr):
    """Run recompiled deref(guest_addr) against the loaded image; return
    (executed_value, expected_value)."""
    import xex
    x = xex.open_xex(xex_path)
    img = x.basefile()
    image_base = x.image_base or 0
    # expected: the big-endian u32 the guest would read at guest_addr
    off = guest_addr - image_base
    if not (0 <= off < len(img) - 3):
        raise ValueError("guest_addr 0x%X outside image" % guest_addr)
    expected = struct.unpack_from(">I", img, off)[0]

    cpp, _ = _recompile_deref()

    with tempfile.TemporaryDirectory() as td:
        # write the image so the harness can load it into guest memory
        imgpath = os.path.join(td, "image.bin").replace("\\", "/")
        with open(imgpath, "wb") as f:
            f.write(img)
        with open(os.path.join(td, "rex_ppc_min.h"), "w") as f:
            f.write(runtime_header_ext())

        harness = f"""#include "rex_ppc_min.h"
#include <cstdio>
#include <cstdint>
#include <cstdlib>
void ppc_call_indirect(PPCContext& ctx, uint8_t* base, uint32_t addr) {{
    ppc_call_indirect_impl(ctx, base, addr);
}}
PPCFuncMapping PPCFuncMappings[] = {{ {{ 0, nullptr }} }};

{cpp}

int main() {{
    const uint64_t IB = 0x{image_base:08X}ull;
    const size_t IMG = {len(img)}u;
    const size_t SLACK = 1u << 20;                 // stack/scratch above the image
    uint8_t* buf = (uint8_t*)std::calloc(1, IMG + SLACK);
    FILE* f = std::fopen("{imgpath}", "rb");
    if (!f) {{ std::fprintf(stderr, "no image\\n"); return 2; }}
    std::fread(buf, 1, IMG, f); std::fclose(f);
    // flat guest memory: base + guest_addr addresses the image
    uint8_t* base = (uint8_t*)((uintptr_t)buf - (uintptr_t)IB);
    PPCContext ctx{{}};
    ctx.r1.u32 = (uint32_t)(IB + IMG + 0x800);      // a valid guest stack pointer
    ctx.r3.u32 = 0x{guest_addr:08X};                // deref's argument
    sub_{FUNC_VA:08X}(ctx, base);
    std::printf("%u\\n", (unsigned)ctx.r3.u32);
    return 0;
}}
"""
        src = os.path.join(td, "boot.cpp")
        with open(src, "w") as f:
            f.write(harness)
        exe = os.path.join(td, "boot.exe")
        c = subprocess.run([GPP, "-std=c++20", "-O2", "-I", td, "-o", exe, src],
                           capture_output=True, text=True)
        if c.returncode != 0:
            raise RuntimeError("g++ failed:\n" + c.stderr[:900])
        out = subprocess.run([exe], capture_output=True, text=True, timeout=30)
        return int(out.stdout.strip()), expected


def scaffold_check():
    """Prove the runtime scaffold (host-shim registry + setjmp/longjmp) compiles."""
    with tempfile.TemporaryDirectory() as td:
        with open(os.path.join(td, "rex_ppc_min.h"), "w") as f:
            f.write(runtime_header_ext())
        probe = """#include "rex_ppc_min.h"
PPCFuncMapping PPCFuncMappings[] = { { 0, nullptr } };
void ppc_call_indirect(PPCContext& c, uint8_t* b, uint32_t a){ ppc_call_indirect_impl(c,b,a); }
// exercise the scaffold symbols so they must type-check
int probe(PPCContext& ctx, uint8_t* base){
    (void)ppc_lookup(0x82000000);
    ppc_setjmp(ctx, base); ppc_longjmp(ctx, base);
    return 0;
}
"""
        src = os.path.join(td, "s.cpp")
        with open(src, "w") as f:
            f.write(probe)
        c = subprocess.run([GPP, "-std=c++20", "-c", "-I", td, "-o",
                            os.path.join(td, "s.o"), src],
                           capture_output=True, text=True)
        return c.returncode == 0, c.stderr


DEFAULT_XEX = os.environ.get("XBT_DEFAULT_XEX", "")


def _selftest():
    print("ppcboot selftest — runtime scaffold\n")
    ok = True

    print("[1] host-shim registry + setjmp/longjmp compile:")
    built, err = scaffold_check()
    print("    ", "OK" if built else "FAILED\n" + err[:600])
    ok = ok and built

    print("[2] guest memory + real image: recompiled deref() reads real data:")
    if not ppc_cc.available():
        print("     XDK compiler not found — skipping (set: config xdk <path>)")
        return ok
    xexp = DEFAULT_XEX if os.path.exists(DEFAULT_XEX) else None
    if not xexp:
        print("     Tetris image not found — skipping the memory test")
        return ok
    for ga in (0x82080000, 0x820FBC58, 0x82000000):
        try:
            got, want = boot_deref(xexp, ga)
            good = (got == want)
            ok = ok and good
            print("     deref(0x%08X) -> ran 0x%08X  image has 0x%08X  %s"
                  % (ga, got, want, "OK" if good else "MISMATCH"))
        except Exception as e:
            ok = False
            print("     deref(0x%08X) ERROR: %s" % (ga, str(e)[:100]))

    print("\nSELFTEST:", "PASS — runtime scaffold works on real data" if ok else "FAIL")
    return ok


def main():
    ap = argparse.ArgumentParser(description="runtime scaffold + guest memory boot")
    ap.add_argument("path", nargs="?")
    ap.add_argument("--deref", type=lambda x: int(x, 0),
                    help="run recompiled deref(addr) against the loaded image")
    ap.add_argument("--scaffold-check", action="store_true")
    ap.add_argument("--selftest", action="store_true")
    args = ap.parse_args()

    if args.scaffold_check:
        ok, err = scaffold_check()
        print("scaffold compiles:", "OK" if ok else "FAILED\n" + err[:800])
        sys.exit(0 if ok else 1)
    if args.selftest or not args.path:
        sys.exit(0 if _selftest() else 1)
    if args.deref is not None:
        got, want = boot_deref(args.path, args.deref)
        print("deref(0x%08X) = 0x%08X ; image = 0x%08X ; %s"
              % (args.deref, got, want, "MATCH" if got == want else "MISMATCH"))
        sys.exit(0 if got == want else 1)
    ap.error("give --deref ADDR, --scaffold-check, or --selftest")


if __name__ == "__main__":
    main()
