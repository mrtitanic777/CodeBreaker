"""Writes arith.golden.txt, logical.golden.txt, memory.golden.txt, system.golden.txt and vector.golden.txt: what
codebreaker/recompile/builders/<module>.py emits for every mnemonic over a spread of operands, as the reference the C++
(cb/recompile/builders/<module>.cpp) is checked against. Run from the checkout root:
    python native/cb/recompile/testdata/gen_builders_golden.py
Each case is "## <mnemonic> <operands> [@0xWORD]" then the statements, one per line (a case with no statements is a
mnemonic that emits nothing). The operand tokens are the recompiler's: r3 f4 v5 cr6 cr1eq 0x10 -0x70(r1).
A (mnemonic, operands) is kept only when the Python gives statements in which no operand was written as "None" (a token
the helper could not read: the old builders printed that, the C++ refuses with a ParseError instead)."""
import itertools
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(HERE, "..", "..", "..", ".."))
from codebreaker.recompile import powerpc
from codebreaker.recompile.builders import arith, floating, logical, memory, system, vector

HELPERS = (powerpc._reg_or_zero, powerpc._vreg, powerpc._freg, powerpc._imm, powerpc._cr, powerpc._mem, powerpc._rv,
           powerpc._base_expr, powerpc._addr)

REG_PATTERNS = [(3, 4, 5, 6, 7), (0, 0, 0, 0, 0), (31, 0, 3, 3, 1), (13, 12, 11, 10, 9)]
VREG_PATTERNS = [(3, 4, 5, 6), (0, 0, 0, 0), (127, 1, 2, 3)]
MEMS = ["-0x70(r1)", "16(r0)", "0(r31)", "0x7FFC(r3)", "-32768(r4)"]
CR_BITS = [("cr0lt", "cr1gt", "cr7so"), ("cr3eq", "cr3eq", "cr6un"), ("cr2gt", "cr5lt", "cr4eq")]
IMM_POOLS = {
    1: [[0, 1, 5, 16, 31, 32, 63, 0x7FFF, 0x8000, 0xFFFF, -1, -0x8000]],
    2: [[0, 5, 16, 31, 63, 0xFFFF], [0, 5, 31, 63]],
    3: [[0, 5, 31], [0, 5, 31], [0, 26, 31]],
}
VECTOR_IMMS = {1: [[0, 1, 2, 3, 5, 7, 15, 31]], 2: [[0, 1, 3, 15], [0, 1, 2, 3]], 3: [[0, 1, 2, 3, 4, 5, 6], [0, 1, 2, 3], [0, 1, 2, 3]]}

# candidate operand signatures, shortest first: R general, F float, V vector, C condition field, B condition bit,
# I immediate, M memory operand
SIGNATURES = ["", "R", "F", "V", "RR", "RI", "VI", "IR", "IF", "CC", "RM", "FM", "VV", "RRR", "RRI", "CRR", "CRI", "VVV", "VVI", "BBB", "FRR",
              "VRR", "RRII", "RRRI", "VVVV", "VVVI", "VVII", "RRIII", "RRRII", "VVIII"]


def operand_lists(sig, imm_pools, regs_limit=None):
    """Every operand list of signature `sig`: register patterns x immediate values."""
    n_imm = sig.count("I")
    pools = imm_pools.get(n_imm, [[0, 1]] * n_imm) if n_imm else []
    patterns = REG_PATTERNS if regs_limit is None else REG_PATTERNS[:regs_limit]
    out = []
    for pattern in patterns:
        for vpattern in VREG_PATTERNS[: (1 if n_imm >= 2 else 3)]:
            # the register patterns only differ where there is more than one kind of register or a repeated one
            for mem in MEMS if "M" in sig else [None]:
                for bits in CR_BITS if "B" in sig else [None]:
                    for imms in itertools.product(*pools):
                        ops, ri, vi, ii, bi = [], 0, 0, 0, 0
                        for ch in sig:
                            if ch == "R":
                                ops.append("r%d" % pattern[ri]); ri += 1
                            elif ch == "F":
                                ops.append("f%d" % pattern[ri]); ri += 1
                            elif ch == "V":
                                ops.append("v%d" % vpattern[vi]); vi += 1
                            elif ch == "C":
                                ops.append("cr%d" % (pattern[ri] % 8)); ri += 1
                            elif ch == "B":
                                ops.append(bits[bi]); bi += 1
                            elif ch == "I":
                                ops.append(str(imms[ii])); ii += 1
                            elif ch == "M":
                                ops.append(mem)
                        if ops not in out:
                            out.append(ops)
    return out


def run(module, mnemonic, ops, word=0):
    try:
        lines = module.emit(mnemonic, ops, *HELPERS, 0x82000000, word)
    except Exception:
        return None
    if lines is None:
        return None
    if any("None" in line for line in lines):
        return None
    return lines


def collect(module, mnemonics, imm_pools=IMM_POOLS, extra=()):
    cases = []
    for mn in list(mnemonics) + list(extra):
        taken = None
        for sig in SIGNATURES:
            if module is vector and "R" in sig:
                continue   # (a vector builder never reads a general register; the Python ignored such a token in a case it did not branch on)
            found = []
            for ops in operand_lists(sig, imm_pools, regs_limit=None if sig.count("I") < 2 else 2):
                lines = run(module, mn, ops)
                if lines is not None:
                    found.append((ops, lines))
            if found:
                taken = found
                break
        if taken is None:
            raise SystemExit("no signature worked for " + mn)
        cases.extend((mn, ops, None, lines) for ops, lines in taken)
    return cases


def write(name, cases):
    out = []
    for mn, ops, word, lines in cases:
        out.append("## " + " ".join([mn] + ops) + ("" if word is None else " @0x%08X" % word))
        out.extend(lines)
    with open(os.path.join(HERE, name + ".golden.txt"), "w", newline="\n") as f:
        f.write("\n".join(out) + "\n")
    print(name, len(cases), "cases", len(out), "lines")


def trap_cases():
    cases = []
    tos = [0, 0x04, 0x0C, 0x14, 0x1F]
    for mn in system.MNEMONICS:
        if mn not in system._TWI and mn not in system._TDI and mn not in system._TW and mn not in system._TD:
            continue
        for to in tos:
            for ra in (0, 3):
                for third in (5, 0x8000) if (mn in system._TWI or mn in system._TDI) else (4,):
                    if mn in system._TWI:
                        word = (3 << 26) | (to << 21) | (ra << 16) | third
                    elif mn in system._TDI:
                        word = (2 << 26) | (to << 21) | (ra << 16) | third
                    else:
                        word = (31 << 26) | (to << 21) | (ra << 16) | (third << 11)
                    lines = system.emit(mn, [], *HELPERS, 0x82000000, word)
                    cases.append((mn, [], word, lines))
    return cases


def explicit(module, mnemonic, operand_sets):
    out = []
    for ops in operand_sets:
        lines = run(module, mnemonic, ops)
        assert lines is not None, (mnemonic, ops)
        out.append((mnemonic, ops, None, lines))
    return out


def real_cases():
    """The instructions of ppc_slice.listing.txt (16 KB of Minecraft Xbox 360 Edition code, real Capstone and VMX128 operand
    text), each unique (mnemonic, operands) run through the builders in the recompiler's order: the first that answers."""
    seen, cases = set(), []
    with open(os.path.join(HERE, "ppc_slice.listing.txt")) as f:
        for line in f:
            if line.startswith("#") or not line.strip():
                continue
            head, _, rest = line.rstrip().partition("|")
            fields = head.split()
            word = int(fields[1], 16)
            mn = fields[3]
            ops = [o.strip() for o in rest.split(",")] if rest.strip() else []
            if mn.startswith(".") or (mn, tuple(ops)) in seen:
                continue
            seen.add((mn, tuple(ops)))
            for module in (floating, arith, logical, system, memory, vector):
                try:
                    lines = module.emit(mn, ops, *HELPERS, 0x82000000, word)
                except Exception:
                    lines = None
                    break
                if lines is not None:
                    if not any("None" in l for l in lines):
                        cases.append((mn, ops, word if mn in TRAPS else None, lines))
                    break
    return cases


TRAPS = set(system._TWI) | set(system._TDI) | set(system._TW) | set(system._TD)


def main():
    write("arith", collect(arith, arith.MNEMONICS, extra=["add.", "subfc.", "cmpw."]))
    write("logical", collect(logical, logical.MNEMONICS))
    write("memory", collect(memory, memory.MNEMONICS))
    system_cases = collect(system, [m for m in system.MNEMONICS if m not in system._TWI and m not in system._TDI and m not in system._TW and m not in system._TD],
                           extra=["mr.", "clrldi."])
    # plain condition fields in a bit operation, and the field forms
    system_cases += explicit(system, "crand", [["cr0", "cr1", "cr2"], ["cr7", "cr3", "cr5"]])
    write("system", system_cases + trap_cases())
    vcases = collect(vector, [m for m in vector.MNEMONICS if m != "vpkd3d128"], imm_pools=VECTOR_IMMS, extra=[m + "." for m in vector.MNEMONICS if m.startswith("vcmp")])
    # vpkd3d128 vD, vB, type, mask, shift: every format (and one that is none), and the shifts and masks that matter
    vcases += explicit(vector, "vpkd3d128", [["v3", "v4", str(t), str(m), str(sh)] for t in (0, 1, 2, 3, 4, 5, 6, 9) for m in (0, 1, 2, 3) for sh in (0, 1, 2, 3)])
    # the VMX128 misidentification (a third operand of 0x60), and every unpack format of vupkd3d128
    for mn in ("vupkhsb128", "vupklsb128"):
        vcases += explicit(vector, mn, [["v3", "v4", "0x60"], ["v3", "v4", "5"]])
    vcases += explicit(vector, "vupkd3d128", [["v3", "v4", str(i)] for i in (0x08, 0x10, 0x14, 0x18, 0x1C)])
    write("vector", vcases)
    write("real", real_cases())


main()
