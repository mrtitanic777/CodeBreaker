"""The SLEIGH specification checked against a real processor's emulator, instruction by instruction
(docs/CORE_ROADMAP.md, Track 1 item A).

    python tools/sleigh_verify.py [--per-form 3] [--states 8] [--report FILE] [MODULE ...]

Takes the instructions the 16-bit modules of the golden corpus actually use (every distinct
form -- mnemonic and operand kinds -- up to --per-form encodings each, from their code segments),
runs each from --states random machine states in Unicorn (x86 real mode) and through the
specification's p-code (native/decompiler/build/bin/pcodecheck), and compares what each left:
the registers, the flags (those Intel leaves undefined for that instruction, as Capstone reports
them, are not compared), the bytes of memory written, and where execution goes next.

Memory reads as a fixed pattern on the Unicorn side; the bytes Unicorn read are handed to the
p-code side, which reads zero anywhere else -- so a p-code address computed wrongly shows as a
difference too. A case is 'untested' when the p-code uses an operation the emulator has no meaning
for (in, out, int ...) or Unicorn cannot run it (a privileged instruction in real mode).

The report: per form, agree / disagree / untested, and for each disagreement what differed."""
import argparse
import collections
import os
import random
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)

REGS32 = ["EAX", "ECX", "EDX", "EBX", "ESP", "EBP", "ESI", "EDI"]
SEGS = ["ES", "CS", "SS", "DS", "FS", "GS"]
FLAGS = {"CF": 0, "PF": 2, "AF": 4, "ZF": 6, "SF": 7, "TF": 8, "IF": 9, "DF": 10, "OF": 11}
CS, IP = 0x1000, 0x0100
MEMSIZE = 0x110000


def pattern_byte(a):
    return ((a * 2654435761) >> 13) & 0xFF


def forms(modules, per_form, seed):
    """{form: [encoding bytes]} from the code segments of the given NE modules."""
    import capstone
    from codebreaker.formats.ne import NEFile
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
    md.detail = True
    found = collections.OrderedDict()
    for path in modules:
        nef = NEFile(path)
        for s in nef.segments():
            if not s["file_off"] or s.get("kind") not in ("CODE", "code") and s["flags"] & 1:
                continue
            raw = nef.data[s["file_off"]:s["file_off"] + s["length"]]
            for ins in md.disasm(bytes(raw), 0):
                kinds = tuple(op.type for op in ins.operands)
                sizes = tuple(op.size for op in ins.operands)
                prefix = tuple(p for p in ins.prefix if p)
                key = (ins.mnemonic, kinds, sizes, prefix)
                lst = found.setdefault(key, [])
                b = bytes(ins.bytes)
                if b not in lst and len(lst) < per_form * 4:
                    lst.append(b)
    rng = random.Random(seed)
    return {k: rng.sample(v, min(per_form, len(v))) for k, v in found.items()}


def state(rng, encoding, mnemonic):
    regs = {r: rng.getrandbits(32) for r in REGS32}
    for s in SEGS:
        regs[s] = rng.choice([0x2000, 0x3000, 0x4000, 0x5000, 0x6000, 0x7000])
    regs["CS"] = CS
    regs["SS"] = 0x8000
    regs["ESP"] = (regs["ESP"] & 0xFFFF0000) | rng.choice([0x1000, 0x2000, 0x7FF0])
    if mnemonic.startswith("rep") or encoding[:1] in (b"\xf2", b"\xf3"):
        regs["ECX"] = (regs["ECX"] & 0xFFFF0000) | 1          # (one pass: both sides finish the instruction)
    for f in FLAGS:
        regs[f] = rng.getrandbits(1)
    regs["TF"] = 0
    return regs


def unicorn_run(encoding, regs):
    """-> (outcome, final registers, {address: byte written}, {address: byte read}, next linear)"""
    import unicorn as U
    from unicorn import x86_const as X
    uc = U.Uc(U.UC_ARCH_X86, U.UC_MODE_16)
    uc.mem_map(0, MEMSIZE)
    uc.mem_write(0, PATTERN)
    lin = CS * 16 + IP
    uc.mem_write(lin, encoding)
    for r in REGS32 + SEGS:
        uc.reg_write(getattr(X, "UC_X86_REG_" + r), regs[r])
    uc.reg_write(X.UC_X86_REG_EIP, IP)
    fl = 0x2
    for f, bit in FLAGS.items():
        fl |= regs[f] << bit
    uc.reg_write(X.UC_X86_REG_EFLAGS, fl)
    reads, writes = {}, {}

    def on_mem(uc_, access, addr, size, value, _ud):
        if access == U.UC_MEM_READ:
            for k in range(size):
                if addr + k not in writes:
                    reads.setdefault(addr + k, PATTERN[addr + k] if addr + k < MEMSIZE else 0)
        else:
            for k in range(size):
                writes[addr + k] = (value >> (8 * k)) & 0xFF
    interrupted = []
    uc.hook_add(U.UC_HOOK_MEM_READ | U.UC_HOOK_MEM_WRITE, on_mem)
    uc.hook_add(U.UC_HOOK_INTR, lambda uc_, n, ud: (interrupted.append(n), uc_.emu_stop()))
    try:
        uc.emu_start(lin, 0xFFFFFFFF, count=1)
    except U.UcError as e:
        return "untested:unicorn %s" % e, None, None, reads, None
    if interrupted:
        return "untested:interrupt", None, None, reads, None
    out = {r: uc.reg_read(getattr(X, "UC_X86_REG_" + r)) for r in REGS32 + SEGS}
    fl = uc.reg_read(X.UC_X86_REG_EFLAGS)
    for f, bit in FLAGS.items():
        out[f] = (fl >> bit) & 1
    nxt = out["CS"] * 16 + (uc.reg_read(X.UC_X86_REG_EIP) & 0xFFFF)
    for r in REGS32:                      # (16-bit code: the registers are compared whole)
        out[r] &= 0xFFFFFFFF
    return "ok", out, writes, reads, nxt


SHIFTS = ("shl", "sal", "shr", "sar", "rol", "ror", "rcl", "rcr")


def undefined_flags(encoding, regs):
    """The flags Intel leaves undefined after this instruction from this state: what Capstone
    reports, and what it leaves out -- OF after a shift or rotate by anything but 1, SF ZF AF PF
    after a multiply (Intel SDM: "undefined")."""
    import capstone
    from capstone import x86_const as X
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
    md.detail = True
    ins = next(md.disasm(encoding, IP), None)
    if ins is None:
        return set()
    names = {"CF": X.X86_EFLAGS_UNDEFINED_CF, "PF": X.X86_EFLAGS_UNDEFINED_PF, "AF": X.X86_EFLAGS_UNDEFINED_AF,
             "ZF": X.X86_EFLAGS_UNDEFINED_ZF, "SF": X.X86_EFLAGS_UNDEFINED_SF, "OF": X.X86_EFLAGS_UNDEFINED_OF}
    out = {f for f, bit in names.items() if ins.eflags & bit}
    if ins.mnemonic in SHIFTS:
        if len(ins.operands) < 2:
            count = 1
        elif ins.operands[1].type == X.X86_OP_IMM:
            count = ins.operands[1].imm
        else:
            count = regs["ECX"] & 0xFF
        if count & 0x1F != 1:
            out.add("OF")
    if ins.mnemonic in ("imul", "mul"):
        out |= {"SF", "ZF", "AF", "PF"}
    return out


def main(argv):
    global PATTERN
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("modules", nargs="*")
    ap.add_argument("--per-form", type=int, default=3)
    ap.add_argument("--states", type=int, default=8)
    ap.add_argument("--seed", type=int, default=1)
    ap.add_argument("--report", default=os.path.join(ROOT, "state", "sleigh_verify.txt"))
    a = ap.parse_args(argv)
    mini = os.path.join(ROOT, "state", "mini", "original")
    modules = a.modules or [os.path.join(mini, n) for n in ("USER.EXE", "GDI.EXE", "KRNL386.EXE", "VGA.DRV",
                                                            "KEYBOARD.DRV", "COMM.DRV", "LZEXPAND.DLL")]
    PATTERN = bytes(pattern_byte(i) for i in range(MEMSIZE))
    tool = os.path.join(ROOT, "native", "decompiler", "build", "bin", "pcodecheck.exe")
    sla = os.path.join(ROOT, "native", "decompiler", "languages", "x86.sla")
    fs = forms([m for m in modules if os.path.isfile(m)], a.per_form, a.seed)
    rng = random.Random(a.seed)
    cases = []                                   # (form, encoding, state, unicorn result)
    for form, encs in fs.items():
        for enc in encs:
            for _ in range(a.states):
                st = state(rng, enc, form[0])
                cases.append((form, enc, st, unicorn_run(enc, st)))
    lines = []
    for k, (form, enc, st, u) in enumerate(cases):
        mem = dict(u[3])
        for addr in (u[2] or {}):                 # (what is written starts as what was there)
            if addr < MEMSIZE:
                mem.setdefault(addr, PATTERN[addr])
        for i, b in enumerate(enc):
            mem[CS * 16 + IP + i] = b
        toks = ["%s=%x" % (r, st[r]) for r in REGS32 + SEGS + list(FLAGS)] + ["IP=%x" % IP]
        toks += ["M%x=%02x" % (addr, b) for addr, b in sorted(mem.items())]
        lines.append("c%d %s" % (k, " ".join(toks)))
    p = subprocess.run([tool, sla, "x86-16-real"], input="\n".join(lines) + "\n", capture_output=True, text=True)
    got = {}
    for ln in p.stdout.splitlines():
        parts = ln.split()
        if len(parts) >= 2:
            got[parts[0]] = parts[1:]
    by_form = collections.OrderedDict()
    for k, (form, enc, st, u) in enumerate(cases):
        rec = by_form.setdefault(form, {"agree": 0, "disagree": 0, "untested": 0, "why": collections.Counter(), "example": None})
        g = got.get("c%d" % k)
        if form[0] == "hlt":                       # (the specification spins on it; the processor waits)
            rec["untested"] += 1
            rec["why"]["hlt waits"] += 1
            continue
        if u[0] != "ok":
            rec["untested"] += 1
            rec["why"][u[0]] += 1
            continue
        if not g or g[0] != "ok":
            rec["untested"] += 1
            rec["why"]["pcode " + (" ".join(g) if g else "no answer")[:60]] += 1
            continue
        pc = {}
        pw = {}
        for t in g[1:]:
            key, _eq, val = t.partition("=")
            if key.startswith("W"):
                pw[int(key[1:], 16)] = int(val, 16)
            else:
                pc[key] = int(val, 16)
        diffs = []
        undef = undefined_flags(enc, st)
        for r in REGS32 + SEGS:
            if pc.get(r, 0) & 0xFFFFFFFF != u[1][r]:
                diffs.append("%s %x/%x" % (r, pc.get(r, 0), u[1][r]))
        for f in FLAGS:
            if f not in undef and pc.get(f, 0) != u[1][f]:
                if f == "AF":                     # (not modelled at all: counted once, apart)
                    rec["af"] = rec.get("af", 0) + 1
                    continue
                diffs.append("%s %d/%d" % (f, pc.get(f, 0), u[1][f]))
        uw = u[2]
        for addr in sorted(set(uw) | set(pw)):
            if uw.get(addr, PATTERN[addr] if addr < MEMSIZE else 0) != pw.get(addr, PATTERN[addr] if addr < MEMSIZE else 0):
                diffs.append("mem[%x] %x/%x" % (addr, pw.get(addr, -1), uw.get(addr, -1)))
                break
        # where execution goes: the offset within the code segment (the specification's indirect
        # near transfers -- ret, jmp/call through a register or memory -- go to the bare offset,
        # its direct ones to CS:offset; both are the same instruction to run next)
        p_ip = (pc.get("LIN", 0) - pc.get("CS", CS) * 16) & 0xFFFF if pc.get("LIN", 0) >= pc.get("CS", CS) * 16 \
            else pc.get("LIN", 0) & 0xFFFF
        u_ip = (u[4] - u[1]["CS"] * 16) & 0xFFFF
        if p_ip != u_ip:
            diffs.append("next %x/%x" % (p_ip, u_ip))
        if diffs:
            rec["disagree"] += 1
            for d in diffs:
                rec["why"][d.split()[0]] += 1
            rec["example"] = rec["example"] or "%s: %s" % (enc.hex(), ", ".join(diffs[:4]))
        else:
            rec["agree"] += 1
    total = collections.Counter()
    out = []
    for form, rec in sorted(by_form.items(), key=lambda kv: (-kv[1]["disagree"], kv[0][0])):
        status = "DISAGREE" if rec["disagree"] else "agree" if rec["agree"] else "untested"
        total[status] += 1
        name = "%s %s" % (form[0], "/".join("%d:%d" % (t, s) for t, s in zip(form[1], form[2])))
        out.append("%-9s %-40s agree %3d  disagree %3d  untested %3d  %s%s" % (
            status, name[:40], rec["agree"], rec["disagree"], rec["untested"],
            ", ".join("%s x%d" % kv for kv in rec["why"].most_common(4)),
            ("   e.g. " + rec["example"]) if rec["example"] else ""))
    af = sum(1 for rec in by_form.values() if rec.get("af"))
    summary = ("%d instruction forms from %d modules, %d cases: %d agree, %d disagree, %d untested "
               "(AF, the auxiliary carry, is not modelled by the specification: wrong in %d forms, not counted above)") % (
        len(by_form), len(modules), len(cases), total["agree"], total["DISAGREE"], total["untested"], af)
    with open(a.report, "w", encoding="utf-8") as f:
        f.write(summary + "\n\n" + "\n".join(out) + "\n")
    print(summary)
    print("report:", a.report)
    return 0


PATTERN = b""

if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
