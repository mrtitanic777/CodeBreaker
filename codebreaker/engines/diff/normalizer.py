#!/usr/bin/env python3
r"""normalizer.py -- collapse architecture-specific asm into a canonical stream.

The point of CodeBreaker Diff is to match the SAME function across the 16-bit
real-mode / 16-bit protected-mode / 32-bit flat eras. The bytes differ (operand
size, segment overrides, register allocation, absolute offsets); the *shape* of
the computation does not. This module maps each reconstructed-asm instruction to
an invariant token so two functions that do the same thing hash the same.

Design choices that make the token cross-era stable:
  * Registers -> ROLE, not name (eax/ax/al -> R_ACC), so a 16->32 widening or a
    different register allocation does not change the stream.
  * Mnemonics -> class for the interchangeable ones (test/cmp -> CANON_CMP; the
    whole jcc family -> CANON_JCC; mov/lea/movzx/movsx -> CANON_MOV) -- these are
    the substitutions a recompile legitimately makes between releases.
  * Every immediate -> IMM and every memory reference -> [MEM], because absolute
    addresses and displacements are exactly what relocation/rebasing changes.

We deliberately normalize the DISASSEMBLY text (what il.parse_asm yields), not raw
opcode bytes: raw bytes are NOT invariant across operand-size (a 16- vs 32-bit
`mov` is different machine code for the same operation), and text-level roles are.
"""
import hashlib
import re

EMPTY_SHA256 = hashlib.sha256(b"").hexdigest()


class Normalizer:
    # raw register (any lane) -> canonical role
    REG_MAP = {
        "al": "R_ACC", "ah": "R_ACC", "ax": "R_ACC", "eax": "R_ACC",
        "cl": "R_CTR", "ch": "R_CTR", "cx": "R_CTR", "ecx": "R_CTR",
        "dl": "R_GEN", "dh": "R_GEN", "dx": "R_GEN", "edx": "R_GEN",
        "bl": "R_GEN", "bh": "R_GEN", "bx": "R_GEN", "ebx": "R_GEN",
        "si": "R_IDX", "esi": "R_IDX", "di": "R_IDX", "edi": "R_IDX",
        "bp": "R_FRAME", "ebp": "R_FRAME", "sp": "R_STACK", "esp": "R_STACK",
        # segment registers collapse to one role -- a 16-bit far reference and a
        # 32-bit flat one should not diverge on the segment operand alone.
        "cs": "R_SEG", "ds": "R_SEG", "es": "R_SEG", "ss": "R_SEG",
        "fs": "R_SEG", "gs": "R_SEG",
    }

    # mnemonic -> class, for the operations a cross-release recompile treats as
    # interchangeable. Everything not listed keeps its own (lower-cased) mnemonic.
    MNEMONIC_CLASS = {
        "test": "CANON_CMP", "cmp": "CANON_CMP",
        "mov": "CANON_MOV", "lea": "CANON_MOV", "movsx": "CANON_MOV",
        "movzx": "CANON_MOV", "movsxd": "CANON_MOV",
        "jz": "CANON_JCC", "je": "CANON_JCC", "jnz": "CANON_JCC", "jne": "CANON_JCC",
        "jg": "CANON_JCC", "jge": "CANON_JCC", "jl": "CANON_JCC", "jle": "CANON_JCC",
        "ja": "CANON_JCC", "jae": "CANON_JCC", "jb": "CANON_JCC", "jbe": "CANON_JCC",
        "jo": "CANON_JCC", "jno": "CANON_JCC", "js": "CANON_JCC", "jns": "CANON_JCC",
        "jc": "CANON_JCC", "jnc": "CANON_JCC", "jp": "CANON_JCC", "jnp": "CANON_JCC",
        "jpe": "CANON_JCC", "jpo": "CANON_JCC", "jcxz": "CANON_JCC", "jecxz": "CANON_JCC",
    }

    # instruction prefixes to peel before classifying (so `es mov ..`, `lock add ..`,
    # `rep movsb` classify on the real mnemonic).
    _PREFIX = re.compile(
        r"^\s*((es|cs|ss|ds|fs|gs|lock|rep|repe|repz|repne|repnz|addr32|data32|"
        r"addr16|data16|short|near|far|dword|word|byte|ptr)\s+)+", re.I)

    _IMM = re.compile(r"^(0x[0-9a-f]+|[0-9a-f]+h|\d+|-\d+)$", re.I)

    @classmethod
    def _norm_operand(cls, op):
        op = op.strip()
        low = op.lower()
        if low in cls.REG_MAP:
            return cls.REG_MAP[low]
        if "[" in op or ":" in op:          # memory / far pointer reference
            return "[MEM]"
        if cls._IMM.match(low):
            return "IMM"
        # a bare label / symbol / ordinal (call and jump targets, externs)
        return "REF"

    @classmethod
    def normalize_instruction(cls, text):
        """One reconstructed-asm instruction (mnemonic + operands, no comment) ->
        canonical token string, e.g. `CANON_MOV R_ACC,[MEM]`."""
        text = cls._PREFIX.sub("", text.strip())
        if not text:
            return ""
        parts = text.split(None, 1)
        mnem = parts[0].lower()
        mnem = cls.MNEMONIC_CLASS.get(mnem, mnem)
        if len(parts) == 1:
            return mnem
        ops = [cls._norm_operand(o) for o in cls._split_operands(parts[1])]
        return "%s %s" % (mnem, ",".join(ops))

    @staticmethod
    def _split_operands(rest):
        """Split an operand list on top-level commas (commas inside [] are part of
        a scaled-index memory operand, e.g. `[eax+ecx*2]` has none, but `[bx+si]`
        style and MASM `ptr` forms can, so respect brackets)."""
        out, depth, cur = [], 0, ""
        for ch in rest:
            if ch == "[":
                depth += 1
            elif ch == "]":
                depth = max(0, depth - 1)
            if ch == "," and depth == 0:
                out.append(cur); cur = ""
            else:
                cur += ch
        if cur.strip():
            out.append(cur)
        return out

    @classmethod
    def hash_opcode_stream(cls, normalized_instructions):
        stream = ";".join(i for i in normalized_instructions if i)
        return hashlib.sha256(stream.encode("utf-8")).hexdigest()
