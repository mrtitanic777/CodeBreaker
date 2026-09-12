"""Minimal interpreted XEX runner — load a whole XEX image into guest memory
and execute it from the entry point on our PowerPC interpreter, HLE-stubbing and
logging every kernel/XAM import it calls.

This is the dynamic complement to the static toolchain: where ppctrace runs one
extracted function, this loads the *program* — the flat PE image at its virtual
base — resolves the import thunks from the XEX import table, and single-steps
from the entry, intercepting each call into an import so it doesn't wander into
placeholder thunk bytes. Every import hit is logged with its library, ordinal
and resolved name, giving a first-boot import trace with no console.

It is deliberately honest about its limits: a real title's entry does elaborate
CRT/TLS/security setup and will soon reach a floating-point, VMX or privileged
instruction the integer interpreter doesn't model — at which point the run stops
and reports exactly where and which imports it reached. That first-boot import
trace is the point, not full emulation.

Interpreter semantics (decode + execute) are reused wholesale from ppctrace, so
this module only adds image loading, import resolution, and the import-aware run
loop. Verified self-containedly: assemble a tiny program that calls a stubbed
import and returns, run it, and assert the import was logged and the HLE return
value propagated.
"""

import importlib
import struct
import sys

DESCRIPTION = "run a XEX from its entry on the PPC interpreter, HLE-stub + log kernel/XAM imports"

WORD = 4
SENTINEL = 0xBFFFFFFC          # LR the entry "returns" to -> clean program exit
STACK_TOP = 0x70000000         # initial r1


def _imp(name):
    try:
        return importlib.import_module("." + name, __package__)
    except Exception:
        return importlib.import_module(name)


def import_map(x):
    """{guest_addr: (library, ordinal, name)} for every *thunk* import (a called
    function), resolving ordinals to names where we know them."""
    ordinals = None
    try:
        ordinals = _imp("ordinals")
    except Exception:
        pass
    out = {}
    try:
        imps = x.imports()
    except Exception:
        return out
    for lib, entries in imps.items():
        for addr, ordinal, kind in entries:
            if kind != "thunk":
                continue
            name = None
            if ordinals is not None:
                # the ordinals table keys on the bare module ("xboxkrnl"/"xam"),
                # while the import table gives "xboxkrnl.exe"/"xam.xex".
                mod = lib.rsplit(".", 1)[0] if "." in lib else lib
                try:
                    r = ordinals.lookup(mod, ordinal)
                    if r:
                        name = r[0]
                except Exception:
                    pass
            out[addr & 0xFFFFFFFF] = (lib, ordinal, name or ("ord_%d" % ordinal))
    return out


def _run_core(tracer, cpu, intercepts, sentinel, max_steps, stub_return, log,
              stub_returns=None):
    """Drive ppctrace's decode/exec with an import-intercept in front. Returns
    the stop reason string. `log` receives (pc, lib, ordinal, name) per import."""
    ppctrace = _imp("ppctrace")
    disasm_ppc = _imp("disasm_ppc")
    MASK32 = ppctrace.MASK32
    reason = "max_steps"
    while cpu.steps < max_steps:
        pc = cpu.pc & MASK32
        if pc == sentinel:
            reason = "returned"
            break
        hit = intercepts.get(pc)
        if hit is not None:
            log.append((pc, hit[0], hit[1], hit[2]))
            rv = stub_return
            if stub_returns and hit[2] in stub_returns:
                rv = stub_returns[hit[2]]
            cpu.gpr[3] = rv & MASK32          # HLE return value in r3
            cpu.pc = cpu.lr & MASK32          # simulate blr out of the import
            cpu.steps += 1
            continue
        ins = tracer._decode(pc)
        if ins is None:
            # capstone can't decode it — if it's a VMX/VMX128 vector op (fixed
            # width, never branches, doesn't touch the integer/CF state we track)
            # skip past it so execution keeps flowing; only truly halt on data.
            off = pc - tracer.base
            raw = tracer.code[off:off + WORD] if 0 <= off <= len(tracer.code) - WORD else b""
            if len(raw) == WORD and disasm_ppc.is_vmx128(raw):
                cpu.pc = (pc + WORD) & MASK32
                cpu.steps += 1
                continue
            reason = "no-code@0x%08X" % pc
            break
        old = pc
        try:
            stop = tracer._exec(cpu, ins)
        except Exception as e:                # noqa: BLE001
            reason = "fault:%s@0x%08X" % (type(e).__name__, pc)
            break
        cpu.steps += 1
        if stop == "return":
            if (cpu.pc & MASK32) in (sentinel, 0):
                reason = "returned"
                break
            continue
        if (cpu.pc & MASK32) == old:
            cpu.pc = (old + 4) & MASK32
    return reason


def run(xex_path, start=None, max_steps=20000, stub_return=0, r3=0, r4=0):
    """Load a XEX and run it from `start` (default: entry point). Returns a dict
    with the stop reason, step count, imports hit, and final registers."""
    ppctrace = _imp("ppctrace")
    xex = _imp("xex")
    x = xex.open_xex(xex_path)
    img = x.basefile()
    base = x.image_base or 0
    entry = start if start is not None else (x.entry_point or base)

    mem = ppctrace.Mem()
    mem.write(base, img)
    intercepts = import_map(x)
    tracer = ppctrace.Tracer(base, img)
    cpu = ppctrace.Cpu(mem)
    cpu.pc = entry & 0xFFFFFFFF
    cpu.lr = SENTINEL
    cpu.gpr[1] = STACK_TOP
    cpu.gpr[3] = r3
    cpu.gpr[4] = r4

    log = []
    reason = _run_core(tracer, cpu, intercepts, SENTINEL, max_steps, stub_return, log)
    # de-dup imports while keeping first-seen order + counts
    seen, order = {}, []
    for _pc, lib, ordv, name in log:
        k = (lib, ordv, name)
        if k not in seen:
            seen[k] = 0
            order.append(k)
        seen[k] += 1
    return {
        "entry": entry, "image_base": base, "reason": reason,
        "steps": cpu.steps, "import_thunks": len(intercepts),
        "imports_hit": [{"library": l, "ordinal": o, "name": n, "count": seen[(l, o, n)]}
                        for (l, o, n) in order],
        "r3": cpu.gpr[3], "lr": cpu.lr, "pc": cpu.pc,
    }


# ── self-verification ──────────────────────────────────────────────────────
def _selfcheck():
    ppctrace = _imp("ppctrace")
    ppcasm = _imp("ppcasm")
    base = 0x82000000
    thunk = 0x82000100
    # save incoming LR (=SENTINEL), call the import, use its result, restore LR,
    # return — a well-formed non-leaf, exactly the shape real prologues take.
    src = ("mflr r0\n"                  # r0 = incoming LR (SENTINEL)
           "li r4, 7\n"
           "bl 0x82000100\n"            # import -> stub sets r3 = 42
           "add r3, r3, r4\n"           # r3 = 42 + 7
           "mtlr r0\n"                  # restore LR = SENTINEL
           "blr\n")                     # pc = SENTINEL -> clean exit
    code = ppcasm.assemble(src, base)
    mem = ppctrace.Mem()
    mem.write(base, code)
    tracer = ppctrace.Tracer(base, code)
    cpu = ppctrace.Cpu(mem)
    cpu.pc = base
    cpu.lr = SENTINEL
    cpu.gpr[1] = STACK_TOP
    intercepts = {thunk: ("xboxkrnl.exe", 5, "TestImport")}
    log = []
    reason = _run_core(tracer, cpu, intercepts, SENTINEL, 100, 0, log,
                       stub_returns={"TestImport": 42})
    if reason != "returned":
        return False, "did not return cleanly: %s" % reason
    if not log or log[0][3] != "TestImport":
        return False, "import not intercepted/logged: %r" % log
    if cpu.gpr[3] != 42 + 7:                 # 42 from the stub + 7 added after
        return False, "HLE return / post-call math wrong: r3=%d" % cpu.gpr[3]
    return True, "loaded+ran a program: import stubbed(42)+logged, r3=42+7, clean return"


def main():
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage:\n"
              "  xexrun <file.xex> [--start 0xADDR] [--steps N] [--stub-ret N]\n"
              "  xexrun --selftest")
        return
    if a[0] == "--selftest":
        ok, msg = _selfcheck()
        print(("PASS: " if ok else "FAIL: ") + msg)
        sys.exit(0 if ok else 1)

    def opt(flag, default):
        return a[a.index(flag) + 1] if flag in a else default

    path = a[0]
    start = int(opt("--start", "0"), 0) or None
    steps = int(opt("--steps", "20000"))
    stub = int(opt("--stub-ret", "0"), 0)
    res = run(path, start=start, max_steps=steps, stub_return=stub)
    print("entry     0x%08X   image_base 0x%08X" % (res["entry"], res["image_base"]))
    print("stop      %s after %d steps" % (res["reason"], res["steps"]))
    print("import thunks in table: %d" % res["import_thunks"])
    print("imports hit (%d distinct):" % len(res["imports_hit"]))
    for imp in res["imports_hit"]:
        print("  %-14s ord %-4d x%-3d %s" %
              (imp["library"], imp["ordinal"], imp["count"], imp["name"]))
    print("final r3=0x%08X pc=0x%08X" % (res["r3"], res["pc"]))


if __name__ == "__main__":
    main()
