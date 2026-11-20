#pragma once

// Bit-field accessors for Xenos microcode words (analysis/xenos/03a-03c), shared by the disassembler and the HLSL
// translator.

#include <cstdint>
#include <vector>

namespace gpu::ucode
{
    constexpr uint32_t Bits(uint64_t value, uint32_t low, uint32_t count)
    {
        return uint32_t((value >> low) & ((1ull << count) - 1));
    }

    inline int32_t SignExtend(uint32_t value, uint32_t bits)
    {
        const uint32_t shift = 32 - bits;
        return int32_t(value << shift) >> shift;
    }

    // ---- Control flow (03a §2-3) ----------------------------------------------------------------------------

    enum CfOp : uint32_t
    {
        kCfNop = 0, kCfExec, kCfExecEnd, kCfCondExec, kCfCondExecEnd, kCfCondExecPred, kCfCondExecPredEnd,
        kCfLoopStart, kCfLoopEnd, kCfCondCall, kCfReturn, kCfCondJmp, kCfAlloc, kCfCondExecPredClean,
        kCfCondExecPredCleanEnd, kCfMarkVsFetchDone,
    };

    inline bool IsExec(uint32_t op)
    {
        return (op >= kCfExec && op <= kCfCondExecPredEnd) || op == kCfCondExecPredClean ||
               op == kCfCondExecPredCleanEnd;
    }

    inline bool IsExecEnd(uint32_t op)
    {
        return op == kCfExecEnd || op == kCfCondExecEnd || op == kCfCondExecPredEnd || op == kCfCondExecPredCleanEnd;
    }

    // CF instruction `index` as a 48-bit value (two per 3-dword slot).
    inline uint64_t ControlFlow(const std::vector<uint32_t>& words, uint32_t index)
    {
        const uint32_t pair = index / 2;
        const uint64_t d0 = words[pair * 3], d1 = words[pair * 3 + 1], d2 = words[pair * 3 + 2];
        if ((index & 1) == 0)
            return d0 | ((d1 & 0xFFFF) << 32);
        return (d1 >> 16) | ((d2 & 0xFFFF) << 16) | ((d2 >> 16) << 32);
    }

    // ---- ALU instruction (03c §2) -----------------------------------------------------------------------------

    struct Alu
    {
        uint32_t w0, w1, w2;
        uint32_t vectorDest() const { return Bits(w0, 0, 6); }
        bool vectorDestRelative() const { return Bits(w0, 6, 1); }
        bool absConstants() const { return Bits(w0, 7, 1); }
        uint32_t scalarDest() const { return Bits(w0, 8, 6); }
        bool scalarDestRelative() const { return Bits(w0, 14, 1); }
        bool exports() const { return Bits(w0, 15, 1); }
        uint32_t vectorMask() const { return Bits(w0, 16, 4); }
        uint32_t scalarMask() const { return Bits(w0, 20, 4); }
        bool vectorSaturate() const { return Bits(w0, 24, 1); }
        bool scalarSaturate() const { return Bits(w0, 25, 1); }
        uint32_t scalarOp() const { return Bits(w0, 26, 6); }
        uint32_t swizzle(int i) const { return Bits(w1, 16 - 8 * (i - 1), 8); }   // src1 at 16, src2 8, src3 0
        bool negate(int i) const { return Bits(w1, 26 - (i - 1), 1); }            // src1 26, src2 25, src3 24
        bool predicateCondition() const { return Bits(w1, 27, 1); }
        bool predicated() const { return Bits(w1, 28, 1); }
        bool relativeToA0() const { return Bits(w1, 29, 1); }
        bool const1Relative() const { return Bits(w1, 30, 1); }
        bool const0Relative() const { return Bits(w1, 31, 1); }
        uint32_t reg(int i) const { return Bits(w2, 16 - 8 * (i - 1), 8); }       // src1 16, src2 8, src3 0
        uint32_t vectorOp() const { return Bits(w2, 24, 5); }
        bool isTemp(int i) const { return Bits(w2, 31 - (i - 1), 1); }            // src1 31, src2 30, src3 29

        // 03c §2.3 (Xenia's rule): the first constant operand uses const_0_rel_abs, later ones const_1_rel_abs,
        // judged by the operand-select bits regardless of whether the vector op reads them.
        bool constantRelative(int i) const
        {
            switch (i)
            {
            case 1: return const0Relative();
            case 2: return isTemp(1) ? const0Relative() : const1Relative();
            default: return (isTemp(1) && isTemp(2)) ? const0Relative() : const1Relative();
            }
        }

        // mulsc/addsc/subsc (scalar opcodes 42-47): the temp operand's register (03c §2.4).
        uint32_t scalarConstantTempReg() const
        {
            return (scalarOp() & 1) | (uint32_t(isTemp(3)) << 1) | (swizzle(3) & 0x3C);
        }
    };

    inline uint32_t VectorOperandCount(uint32_t op)
    {
        switch (op)
        {
        case 8: case 9: case 10: case 19: return 1;          // frc, trunc, floor, max4
        case 11: case 12: case 13: case 14: case 17: return 3; // mad, cnd*, dp2add
        default: return 2;
        }
    }

    enum class ScalarOperands { kNone, kOne, kTwo, kConstantTemp };

    inline ScalarOperands ScalarOperandKind(uint32_t op)
    {
        switch (op)
        {
        case 33: case 50: return ScalarOperands::kNone;                                      // setp_clr, retain_prev
        case 0: case 2: case 4: case 5: case 6: case 23: case 24: case 25: return ScalarOperands::kTwo;
        case 42: case 43: case 44: case 45: case 46: case 47: return ScalarOperands::kConstantTemp;
        default: return ScalarOperands::kOne;
        }
    }
}
