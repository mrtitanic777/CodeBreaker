// Xenos microcode -> HLSL. Spec: analysis/xenos/03a (control flow), 03b (fetch), 03c (ALU), 04 (vertex fetch),
// 05a (texture fetch constant). The structure (per-invocation predication, pc-dispatch loop for shaders with jumps)
// follows Xenia's translators (BSD-3) as described in 03a §7; the code is our own.

#include "gpu/shader_translator.h"

#include "gpu/ucode_fields.h"
#include "log.h"

#include <windows.h>
#include <d3dcompiler.h>
#include <dxcapi.h>
#include <wrl/client.h>

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <format>
#include <optional>
#include <fstream>
#include <iterator>
#include <mutex>
#include <set>

using Microsoft::WRL::ComPtr;

namespace gpu
{
    namespace
    {
        using namespace ucode;

        const char kComponents[] = "xyzw";
        constexpr uint32_t kFetchSetTextureLod = 24; // GPUTEXTUREFETCHOP (03b §3.1)
        constexpr uint32_t kFetchGetTextureGradients = 18;

        // CB_PS_PROBE=<hash>:<slot>:<register> (hex hash, decimal slot and register): the pixel shader with that hash
        // outputs the register's value after that slot as colour 0 (xyz, alpha 1), for finding where a value goes wrong.
        // CB_PS_PROBE=ALL[:<n>]: every pixel shader (longer than n dwords) outputs a flat colour, the low three bytes of
        // its hash as R, G, B, to see which shader draws what.
        struct Probe
        {
            uint64_t hash = 0;
            uint32_t slot = 0, reg = 0;
            bool all = false;
            uint32_t minDwords = 0; // ALL:<n>: only shaders longer than n dwords
        };
        const Probe g_probe = [] {
            Probe p;
            if (const char* v = std::getenv("CB_PS_PROBE"); v && *v)
            {
                unsigned long long hash = 0;
                unsigned slot = 0, reg = 0;
                if (std::string(v).rfind("ALL", 0) == 0)
                {
                    p.all = true;
                    std::sscanf(v, "ALL:%u", &p.minDwords);
                }
                else if (std::sscanf(v, "%llx:%u:%u", &hash, &slot, &reg) == 3)
                    p = { hash, slot, reg };
            }
            return p;
        }();

        // Exact power of two 2^e as a float literal (e in [-126, 127]).
        std::string PowerOfTwo(int32_t e)
        {
            return std::format("asfloat(0x{:08X}u)", uint32_t(e + 127) << 23);
        }

        class Translator
        {
        public:
            Translator(const Shader& shader, const TranslationOptions& options)
                : shader_(shader), options_(options), vertex_(shader.stage == Stage::kVertex)
            {
            }

            TranslatedShader Run();

        private:
            // The line that replaces colour 0 for CB_PS_PROBE (empty when this shader is not probed).
            std::string ProbeColor() const
            {
                if (g_probe.all && shader_.words.size() > g_probe.minDwords)
                    return std::format("    s.color[0] = float4({}, {}, {}, 1.0);\n", (shader_.hash & 0xFF) / 255.0,
                                       ((shader_.hash >> 8) & 0xFF) / 255.0, ((shader_.hash >> 16) & 0xFF) / 255.0);
                return g_probe.hash == shader_.hash ? "    s.color[0] = float4(s.probe.xyz, 1.0);\n" : "";
            }

            const Shader& shader_;
            const TranslationOptions& options_;
            const bool vertex_;
            TranslatedShader result_;
            std::string body_;
            int indent_ = 1;
            uint32_t currentSlot_ = 0;
            std::set<uint32_t> labels_;
            uint32_t cfCount_ = 0;

            template<class... Args>
            void Line(std::format_string<Args...> fmt, Args&&... args)
            {
                body_.append(size_t(indent_) * 4, ' ');
                body_ += std::format(fmt, std::forward<Args>(args)...);
                body_ += '\n';
            }

            [[noreturn]] void Unsupported(const std::string& what) const
            {
                Fatal("GPU: {} shader {:016X} slot {}: {} is not implemented by the HLSL translator",
                      StageName(shader_.stage), shader_.hash, currentSlot_, what);
            }

            std::string ConstantWindow() const
            {
                return vertex_ || options_.computeHarness ? "xe_sys_const_window.x, xe_sys_const_window.y"
                                                          : "xe_sys_const_window.z, xe_sys_const_window.w";
            }

            static std::string TempRef(uint32_t reg, bool relative)
            {
                return relative ? std::format("r[({}u + uint(s.aL)) & 63u]", reg) : std::format("r[{}]", reg);
            }

            std::string ConstRef(uint32_t index, bool relative, bool toA0) const
            {
                const std::string i = relative ? std::format("{} + {}", index, toA0 ? "s.a0" : "s.aL")
                                               : std::format("{}", index);
                return std::format("xe_const({}, {})", i, ConstantWindow());
            }

            // Register/constant of ALU operand i (1-3), unswizzled and unmodified.
            std::string AluRegister(const Alu& alu, int i) const
            {
                const uint32_t reg = alu.reg(i);
                if (alu.isTemp(i))
                    return TempRef(reg & 0x3F, (reg & 0x40) != 0);
                return ConstRef(reg, alu.constantRelative(i), alu.relativeToA0());
            }

            bool AluAbs(const Alu& alu, int i) const
            {
                return alu.isTemp(i) ? (alu.reg(i) & 0x80) != 0 : alu.absConstants();
            }

            // Sign-bit modifiers (hlsl_library.cpp xe_abs/xe_neg) of a float4 (vector) or float value.
            static std::string Modify(std::string value, bool vector, bool abs, bool negate)
            {
                const char* suffix = vector ? "4" : "";
                if (abs)
                    value = std::format("xe_abs{}({})", suffix, value);
                return negate ? std::format("xe_neg{}({})", suffix, value) : value;
            }

            // Flush, then abs, then negate (03c §1, §2.1). Operands are flushed at their source instead of at every
            // read: temporary registers only ever hold flushed values (ALU and fetch results,
            // interpolators and harness inputs are flushed when written) and the renderer flushes the float constants
            // when it uploads them (FlushDenormals).
            static std::string Flushed(const Alu&, int, const std::string& value, bool) { return value; }

            std::string AluVectorOperand(const Alu& alu, int i) const
            {
                const uint32_t swizzle = alu.swizzle(i);
                std::string swz = ".";
                for (uint32_t c = 0; c < 4; c++)
                    swz += kComponents[((swizzle >> (2 * c)) + c) & 3];
                return Modify(Flushed(alu, i, AluRegister(alu, i) + swz, true), true, AluAbs(alu, i), alu.negate(i));
            }

            std::string AluScalarOperand(const Alu& alu, uint32_t component) const
            {
                return Modify(Flushed(alu, 3, std::format("{}.{}", AluRegister(alu, 3), kComponents[component]), false),
                              false, AluAbs(alu, 3), alu.negate(3));
            }

            void EmitAlu(const Alu& alu);
            void EmitVertexFetch(uint32_t w0, uint32_t w1, uint32_t w2);
            void EmitTextureFetch(uint32_t opcode, uint32_t w0, uint32_t w1, uint32_t w2);
            void EmitFetchDestination(uint32_t w0, uint32_t w1, const std::string& value);
            void EmitExecBody(uint64_t cf);
            std::string ExecCondition(uint64_t cf) const;
            std::string Jump(uint32_t target) const;
            void EmitLinear();
            void EmitDispatch();
            uint32_t TextureBindingIndex(uint32_t tf, uint32_t dimension);
            uint32_t SamplerBindingIndex(const SamplerBinding& binding);
            std::string Declarations() const;
            std::string EntryPoint() const;
        };

        // ---- ALU ----------------------------------------------------------------------------------------------------

        // Comparison helper (hlsl_library.cpp) of setp_*_push: eq, ne, gt, ge against 0. All comparisons go through
        // integer-key helpers so host compilers cannot turn compare-selects into NaN-dropping min/max.
        const char* SetpCompare(uint32_t op)
        {
            switch (op)
            {
            case 20: return "xe_eq";
            case 21: return "xe_ne";
            case 22: return "xe_gt";
            default: return "xe_ge";
            }
        }

        // Operations whose result is, bit for bit, one of their (flushed) operands, 0, 1 or another normal constant:
        // max / min / selects / compares / floor / trunc / kills. Their results need no flush before a register
        // write; saturate keeps that (it returns the value, 0 or 1).
        bool VectorResultFlushed(uint32_t op)
        {
            return (op >= 2 && op <= 7) || op == 9 || op == 10 || (op >= 12 && op <= 14) || op == 19 ||
                   (op >= 24 && op <= 27) || op == 29;
        }
        bool ScalarResultFlushed(uint32_t op)
        {
            return (op >= 5 && op <= 10) || op == 12 || op == 13 || op == 23 || op == 24 || (op >= 27 && op <= 31) ||
                   (op >= 33 && op <= 39);
        }

        void Translator::EmitAlu(const Alu& alu)
        {
            const uint32_t vop = alu.vectorOp(), sop = alu.scalarOp();
            if (vop >= 30 || sop == 41 || sop > 50)
                Unsupported(std::format("ALU opcode {}/{}", vop, sop));
            if (alu.predicated())
                Line("if (s.p0 == {})", alu.predicateCondition() ? "true" : "false");
            Line("{{");
            indent_++;

            // Read every operand before anything is written (03c §1).
            for (uint32_t i = 1; i <= VectorOperandCount(vop); i++)
                Line("float4 v{} = {};", i, AluVectorOperand(alu, int(i)));
            const uint32_t swizzle = alu.swizzle(3);
            const uint32_t a = ((swizzle >> 6) + 3) & 3, b = swizzle & 3;
            switch (ScalarOperandKind(sop))
            {
            case ScalarOperands::kNone:
                break;
            case ScalarOperands::kOne:
                Line("float sa = {};", AluScalarOperand(alu, a));
                break;
            case ScalarOperands::kTwo:
                Line("float sa = {};", AluScalarOperand(alu, a));
                Line("float sb = {};", AluScalarOperand(alu, b));
                break;
            case ScalarOperands::kConstantTemp:
            {
                // c[src3_reg] (relative as operand 3) and a temp built from other fields; abs_constants and the
                // src3 negate apply to both (03c §2.4).
                const std::string c = ConstRef(alu.reg(3), alu.constantRelative(3), alu.relativeToA0());
                Line("float sa = {};", Modify(std::format("{}.{}", c, kComponents[a]), false, alu.absConstants(), alu.negate(3)));
                Line("float sb = {};", Modify(std::format("{}.{}", TempRef(alu.scalarConstantTempReg(), false),
                                                          kComponents[b]), false, alu.absConstants(), alu.negate(3)));
                break;
            }
            }

            // Vector operation (03c §5).
            switch (vop)
            {
            case 0: Line("precise float4 vr = v1 + v2;"); break;
            case 1: Line("float4 vr = xe_mul4(v1, v2);"); break;
            case 2: Line("float4 vr = xe_max4(v1, v2);"); break;
            case 3: Line("float4 vr = xe_min4(v1, v2);"); break;
            case 4: Line("float4 vr = xe_b2f4(xe_eq4(v1, v2));"); break;
            case 5: Line("float4 vr = xe_b2f4(xe_gt4(v1, v2));"); break;
            case 6: Line("float4 vr = xe_b2f4(xe_ge4(v1, v2));"); break;
            case 7: Line("float4 vr = xe_b2f4(xe_ne4(v1, v2));"); break;
            case 8: Line("float4 vr = xe_frc4(v1);"); break;
            case 9: Line("float4 vr = trunc(v1);"); break;
            case 10: Line("float4 vr = floor(v1);"); break;
            case 11: Line("float4 vr = xe_mad4(v1, v2, v3);"); break;
            case 12: Line("float4 vr = xe_select4(xe_eq4(v1, 0.0), v2, v3);"); break;
            case 13: Line("float4 vr = xe_select4(xe_ge4(v1, 0.0), v2, v3);"); break;
            case 14: Line("float4 vr = xe_select4(xe_gt4(v1, 0.0), v2, v3);"); break;
            case 15: Line("float4 vr = xe_dp4(v1, v2).xxxx;"); break;
            case 16: Line("float4 vr = xe_dp3(v1, v2).xxxx;"); break;
            case 17: Line("float4 vr = xe_dp2add(v1, v2, v3).xxxx;"); break;
            case 18: Line("float4 vr = xe_cube(v1);"); break;
            case 19: Line("float4 vr = xe_max4_reduce(v1).xxxx;"); break;
            case 20: case 21: case 22: case 23:
            {
                const char* cmp = SetpCompare(vop);
                Line("s.p0 = xe_is_zero(v1.w) && {}(v2.w, 0.0);", cmp);
                Line("precise float vx = v1.x + 1.0;");
                Line("float4 vr = xe_select(xe_is_zero(v1.x) && {}(v2.x, 0.0), 0.0, vx).xxxx;", cmp);
                break;
            }
            case 24: case 25: case 26: case 27:
            {
                static const char* const kOps[] = { "xe_eq4", "xe_gt4", "xe_ge4", "xe_ne4" };
                Line("bool vhit = any({}(v1, v2));", kOps[vop - 24]);
                Line("if (vhit) s.killed = true;");
                Line("float4 vr = (vhit ? 1.0 : 0.0).xxxx;");
                break;
            }
            case 28: Line("float4 vr = float4(1.0, xe_mul(v1.y, v2.y), v1.z, v2.w);"); break;
            case 29:
                Line("s.a0 = xe_clamp_a0(xe_round_half_up(v1.w));");
                Line("float4 vr = xe_max4(v1, v2);");
                break;
            }

            // Scalar operation (03c §6); the result also becomes ps.
            switch (sop)
            {
            case 0: Line("precise float sr = sa + sb;"); break;
            case 1: Line("precise float sr = sa + s.ps;"); break;
            case 2: Line("float sr = xe_mul(sa, sb);"); break;
            case 3: Line("float sr = xe_mul(sa, s.ps);"); break;
            case 4: Line("float sr = xe_muls_prev2(sa, sb, s.ps);"); break;
            case 5: Line("float sr = xe_max(sa, sb);"); break;
            case 6: Line("float sr = xe_min(sa, sb);"); break;
            case 7: Line("float sr = xe_is_zero(sa) ? 1.0 : 0.0;"); break;
            case 8: Line("float sr = xe_gt(sa, 0.0) ? 1.0 : 0.0;"); break;
            case 9: Line("float sr = xe_ge(sa, 0.0) ? 1.0 : 0.0;"); break;
            case 10: Line("float sr = xe_ne(sa, 0.0) ? 1.0 : 0.0;"); break;
            case 11: Line("float sr = xe_frc(sa);"); break;
            case 12: Line("float sr = trunc(sa);"); break;
            case 13: Line("float sr = floor(sa);"); break;
            case 14: Line("float sr = xe_exp(sa);"); break;
            case 15: Line("float sr = xe_logc(sa);"); break;
            case 16: Line("float sr = xe_log(sa);"); break;
            case 17: Line("float sr = xe_rcpc(sa);"); break;
            case 18: Line("float sr = xe_rcpf(sa);"); break;
            case 19: Line("float sr = xe_rcp(sa);"); break;
            case 20: Line("float sr = xe_rsqc(sa);"); break;
            case 21: Line("float sr = xe_rsqf(sa);"); break;
            case 22: Line("float sr = xe_rsq(sa);"); break;
            case 23:
                Line("s.a0 = xe_clamp_a0(xe_round_half_up(sa));");
                Line("float sr = xe_max(sa, sb);");
                break;
            case 24:
                Line("s.a0 = xe_clamp_a0(floor(sa));");
                Line("float sr = xe_max(sa, sb);");
                break;
            case 25: Line("precise float sr = sa - sb;"); break;
            case 26: Line("precise float sr = sa - s.ps;"); break;
            case 27: case 28: case 29: case 30:
            {
                static const char* const kOps[] = { "xe_eq", "xe_ne", "xe_gt", "xe_ge" };
                Line("s.p0 = {}(sa, 0.0);", kOps[sop - 27]);
                Line("float sr = s.p0 ? 0.0 : 1.0;");
                break;
            }
            case 31:
                Line("s.p0 = xe_is_one(sa);");
                Line("float sr = xe_select(s.p0, 0.0, xe_select(xe_is_zero(sa), 1.0, sa));");
                break;
            case 32:
                Line("precise float st = sa - 1.0;");
                Line("s.p0 = xe_le(st, 0.0);");
                Line("float sr = xe_select(s.p0, 0.0, st);");
                break;
            case 33:
                Line("s.p0 = false;");
                Line("float sr = XE_FLT_MAX;");
                break;
            case 34:
                Line("s.p0 = xe_is_zero(sa);");
                Line("float sr = sa;");
                break;
            case 35: case 36: case 37: case 38: case 39:
            {
                static const char* const kOps[] = { "xe_eq(sa, 0.0)", "xe_gt(sa, 0.0)", "xe_ge(sa, 0.0)", "xe_ne(sa, 0.0)",
                                                    "xe_is_one(sa)" };
                Line("bool shit = {};", kOps[sop - 35]);
                Line("if (shit) s.killed = true;");
                Line("float sr = shit ? 1.0 : 0.0;");
                break;
            }
            case 40: Line("float sr = xe_sqrt(sa);"); break;
            case 42: case 43: Line("float sr = xe_mul(sa, sb);"); break;
            case 44: case 45: Line("precise float sr = sa + sb;"); break;
            case 46: case 47: Line("precise float sr = sa - sb;"); break;
            case 48: Line("float sr = sin(sa);"); break;
            case 49: Line("float sr = cos(sa);"); break;
            case 50: Line("float sr = s.ps;"); break;
            }
            Line("s.ps = sr;");
            if (alu.vectorSaturate())
                Line("vr = saturate(vr);");
            Line("float srw = {};", alu.scalarSaturate() ? "saturate(sr)" : "sr");

            // Writes: vector first, then scalar (Xenia's order when both hit one register).
            const uint32_t vmask = alu.vectorMask(), smask = alu.scalarMask();
            if (alu.exports())
            {
                std::string target;
                const uint32_t index = alu.vectorDest();
                if (vertex_ && index < 16)
                    target = std::format("s.o[{}]", index);
                else if (vertex_ && index == 62)
                    target = "s.pos";
                else if (vertex_ && index == 63)
                    target = "s.pts";
                else if (!vertex_ && index < 4)
                    target = std::format("s.color[{}]", index);
                else if (!vertex_ && index == 61)
                    target = "s.depth";
                else
                    Unsupported(std::format("export to register {} (memory export or undefined)", index));
                // 03c §4: both masks -> 1, vector only -> vector, scalar only -> scalar, neither -> 0 if
                // scalar_dest_rel, else unwritten.
                for (uint32_t c = 0; c < 4; c++)
                {
                    const bool v = (vmask >> c) & 1, sc = (smask >> c) & 1;
                    if (v && sc)
                        Line("{}.{} = 1.0;", target, kComponents[c]);
                    else if (v)
                        Line("{}.{} = vr.{};", target, kComponents[c], kComponents[c]);
                    else if (sc)
                        Line("{}.{} = srw;", target, kComponents[c]);
                    else if (alu.scalarDestRelative())
                        Line("{}.{} = 0.0;", target, kComponents[c]);
                }
            }
            else
            {
                if (vmask)
                {
                    Line("float4 vrf = {};", VectorResultFlushed(vop) ? "vr" : "xe_flush4(vr)");
                    const std::string dest = TempRef(alu.vectorDest(), alu.vectorDestRelative());
                    for (uint32_t c = 0; c < 4; c++)
                        if ((vmask >> c) & 1)
                            Line("{}.{} = vrf.{};", dest, kComponents[c], kComponents[c]);
                }
                if (smask)
                {
                    Line("float srf = {};", ScalarResultFlushed(sop) ? "srw" : "xe_flush(srw)");
                    const std::string dest = TempRef(alu.scalarDest(), alu.scalarDestRelative());
                    for (uint32_t c = 0; c < 4; c++)
                        if ((smask >> c) & 1)
                            Line("{}.{} = srf;", dest, kComponents[c]);
                }
            }
            indent_--;
            Line("}}");
        }

        // ---- Fetches ------------------------------------------------------------------------------------------------

        // dst_swiz: 0-3 = fetched component, 4 = 0, 5 = 1, 7 = keep (03b §1.1).
        void Translator::EmitFetchDestination(uint32_t w0, uint32_t w1, const std::string& value)
        {
            const std::string dest = TempRef(Bits(w0, 12, 6), Bits(w0, 18, 1));
            const uint32_t swizzle = Bits(w1, 0, 12);
            for (uint32_t c = 0; c < 4; c++)
            {
                const uint32_t select = (swizzle >> (3 * c)) & 7;
                if (select < 4)
                    Line("{}.{} = xe_flush({}.{});", dest, kComponents[c], value, kComponents[select]);
                else if (select == 4 || select == 5)
                    Line("{}.{} = {};", dest, kComponents[c], select == 4 ? "0.0" : "1.0");
                else if (select == 6)
                    Unsupported("fetch destination swizzle 6");
            }
        }

        void Translator::EmitVertexFetch(uint32_t w0, uint32_t w1, uint32_t w2)
        {
            const bool mini = Bits(w1, 30, 1);
            if (Bits(w1, 31, 1))
                Line("if (s.p0 == {})", Bits(w2, 31, 1) ? "true" : "false");
            Line("{{");
            indent_++;
            if (!mini)
            {
                const uint32_t hw = Bits(w0, 20, 5) * 3 + Bits(w0, 25, 2);
                Line("float vi = {}.{};", TempRef(Bits(w0, 5, 6), Bits(w0, 11, 1)), kComponents[Bits(w0, 30, 2)]);
                if (Bits(w1, 15, 1))
                    Line("int vindex = int(xe_round_half_up(vi));");
                else
                    Line("int vindex = int(floor(vi));");
                Line("uint vd0 = xe_fetch_dword({}u), vd1 = xe_fetch_dword({}u);", hw * 2, hw * 2 + 1);
                Line("s.vfBase = int(vd0 >> 2) + {} * vindex;", Bits(w2, 0, 8));
                Line("s.vfEndian = vd1 & 3u;");
            }
            const int32_t offset = SignExtend(Bits(w2, 8, 23), 23);
            const uint32_t format = Bits(w1, 16, 6);
            const bool isSigned = Bits(w1, 12, 1), isInteger = Bits(w1, 13, 1), noZero = Bits(w1, 14, 1);
            uint32_t words = 0;
            std::vector<std::string> components;
            auto fixed = [&](uint32_t word, uint32_t off, uint32_t bits) {
                components.push_back(std::format("xe_unpack_fixed(vw{}, {}u, {}u, {}, {}, {})", word, off, bits,
                                                 isSigned ? "true" : "false", isInteger ? "true" : "false",
                                                 noZero ? "true" : "false"));
            };
            switch (format)
            {
            case 6: words = 1; fixed(0, 0, 8); fixed(0, 8, 8); fixed(0, 16, 8); fixed(0, 24, 8); break;
            case 7: words = 1; fixed(0, 0, 10); fixed(0, 10, 10); fixed(0, 20, 10); fixed(0, 30, 2); break;
            case 16: words = 1; fixed(0, 0, 11); fixed(0, 11, 11); fixed(0, 22, 10); break;
            case 17: words = 1; fixed(0, 0, 10); fixed(0, 10, 11); fixed(0, 21, 11); break;
            case 25: words = 1; fixed(0, 0, 16); fixed(0, 16, 16); break;
            case 26: words = 2; fixed(0, 0, 16); fixed(0, 16, 16); fixed(1, 0, 16); fixed(1, 16, 16); break;
            case 31: words = 1; components = { "xe_half(vw0 & 0xFFFFu)", "xe_half(vw0 >> 16)" }; break;
            case 32:
                words = 2;
                components = { "xe_half(vw0 & 0xFFFFu)", "xe_half(vw0 >> 16)", "xe_half(vw1 & 0xFFFFu)", "xe_half(vw1 >> 16)" };
                break;
            case 33: case 34: case 35:
                words = format == 33 ? 1 : format == 34 ? 2 : 4;
                for (uint32_t i = 0; i < words; i++)
                    fixed(i, 0, 32);
                break;
            case 36: case 37: case 38: case 57:
                words = format == 36 ? 1 : format == 37 ? 2 : format == 38 ? 4 : 3;
                for (uint32_t i = 0; i < words; i++)
                    components.push_back(std::format("asfloat(vw{})", i));
                break;
            default:
                Unsupported(std::format("vertex format {}", format));
            }
            for (uint32_t i = 0; i < words; i++)
                Line("uint vw{} = xe_swap(xe_memory.Load(uint(s.vfBase + {} + {}) * 4u), s.vfEndian);", i, offset, i);
            // Components the format lacks read as 0 (Xenia; hardware unverified, 03b §2.2).
            while (components.size() < 4)
                components.push_back("0.0");
            Line("float4 vf = float4({}, {}, {}, {});", components[0], components[1], components[2], components[3]);
            if (const int32_t exp = SignExtend(Bits(w1, 24, 6), 6))
                Line("vf *= {};", PowerOfTwo(exp));
            EmitFetchDestination(w0, w1, "vf");
            indent_--;
            Line("}}");
        }

        uint32_t Translator::TextureBindingIndex(uint32_t tf, uint32_t dimension)
        {
            for (uint32_t i = 0; i < result_.textures.size(); i++)
                if (result_.textures[i].fetchConstant == tf && result_.textures[i].dimension == dimension)
                    return i;
            result_.textures.push_back({ tf, dimension });
            return uint32_t(result_.textures.size() - 1);
        }

        uint32_t Translator::SamplerBindingIndex(const SamplerBinding& binding)
        {
            for (uint32_t i = 0; i < result_.samplers.size(); i++)
                if (result_.samplers[i] == binding)
                    return i;
            result_.samplers.push_back(binding);
            return uint32_t(result_.samplers.size() - 1);
        }

        void Translator::EmitTextureFetch(uint32_t opcode, uint32_t w0, uint32_t w1, uint32_t w2)
        {
            const uint32_t dimension = Bits(w2, 14, 2);
            if (opcode == kFetchSetTextureLod)
            {
                // setTexLOD: the register LOD = src.x (first selected component), used by later fetches with
                // UseRegisterLOD (03b §3.1, §3.4). Hardware keeps one value per 2x2 quad, taken from its first live
                // pixel (03b §3.7); kept per pixel here as Xenia does, which differs only if a quad's pixels set
                // different values. Initially 0 (undefined on hardware).
                if (Bits(w1, 31, 1))
                    Line("if (s.p0 == {})", Bits(w2, 31, 1) ? "true" : "false");
                Line("s.regLod = {}.{};", TempRef(Bits(w0, 5, 6), Bits(w0, 11, 1)), kComponents[Bits(w0, 26, 6) & 3]);
                return;
            }
            if (opcode == kFetchGetTextureGradients)
            {
                // getGradients: the screen-space derivatives of the coordinate's first two components, (d/dx, d/dy)
                // as (x.x, x.y, y.x, y.y) (Xenia's layout); a vertex shader has none (0). Coarse and fine are one on
                // the hardware.
                const std::string src = TempRef(Bits(w0, 5, 6), Bits(w0, 11, 1));
                const uint32_t swz = Bits(w0, 26, 6);
                if (Bits(w1, 31, 1))
                    Line("if (s.p0 == {})", Bits(w2, 31, 1) ? "true" : "false");
                Line("{{");
                indent_++;
                Line("float2 tg = float2({}.{}, {}.{});", src, kComponents[swz & 3], src, kComponents[(swz >> 2) & 3]);
                if (vertex_)
                    Line("float4 tf = float4(0.0, 0.0, 0.0, 0.0);");
                else
                    Line("float4 tf = float4(ddx_coarse(tg), ddy_coarse(tg));");
                EmitFetchDestination(w0, w1, "tf");
                indent_--;
                Line("}}");
                return;
            }
            if (opcode != 1)
                Unsupported(std::format("texture instruction {}", opcode));
            if (dimension == 2)
                Unsupported("tfetch3D");
            if (Bits(w2, 0, 1))
                Unsupported("register gradients (setGradientH/V)");
            if (options_.computeHarness)
                Unsupported("texture fetch in the compute harness");
            const uint32_t tf = Bits(w0, 20, 5);
            const uint32_t texture = TextureBindingIndex(tf, dimension);
            const uint32_t sampler = SamplerBindingIndex({ tf, Bits(w1, 12, 2), Bits(w1, 14, 2), Bits(w1, 16, 2), Bits(w1, 18, 3) });

            if (Bits(w1, 31, 1))
                Line("if (s.p0 == {})", Bits(w2, 31, 1) ? "true" : "false");
            Line("{{");
            indent_++;
            const std::string src = TempRef(Bits(w0, 5, 6), Bits(w0, 11, 1));
            const uint32_t swz = Bits(w0, 26, 6);
            const uint32_t base = tf * 6;
            Line("uint td0 = xe_fetch_dword({}u), td2 = xe_fetch_dword({}u), td3 = xe_fetch_dword({}u), "
                 "td4 = xe_fetch_dword({}u);", base, base + 2, base + 3, base + 4);
            if (dimension == 0)
            {
                Line("float tu = {}.{};", src, kComponents[swz & 3]);
                Line("float tsize = float((td2 & 0xFFFFFFu) + 1u);");
            }
            else if (dimension == 3)
            {
                // Cube: X and Y are the coordinates on the face in [1, 2) (the cube instruction's SC / TC over the
                // major axis, plus 1.5), Z the face (0 +X, 1 -X, 2 +Y, 3 -Y, 4 +Z, 5 -Z). Back to the direction the
                // cube instruction took them from, for a host cube texture (its faces in the same order).
                Line("float tcs = {}.{} * 2.0 - 3.0, tct = {}.{} * 2.0 - 3.0;", src, kComponents[swz & 3], src,
                     kComponents[(swz >> 2) & 3]);
                Line("uint tface = min(uint(max({}.{}, 0.0) + 0.5), 5u);", src, kComponents[(swz >> 4) & 3]);
                Line("float3 tu = tface == 0u ? float3(1.0, -tct, -tcs) : tface == 1u ? float3(-1.0, -tct, tcs) :");
                Line("    tface == 2u ? float3(tcs, 1.0, tct) : tface == 3u ? float3(tcs, -1.0, -tct) :");
                Line("    tface == 4u ? float3(tcs, -tct, 1.0) : float3(-tcs, -tct, -1.0);");
            }
            else
            {
                Line("float2 tu = float2({}.{}, {}.{});", src, kComponents[swz & 3], src, kComponents[(swz >> 2) & 3]);
                Line("float2 tsize = float2(float((td2 & 0x1FFFu) + 1u), float(((td2 >> 13) & 0x1FFFu) + 1u));");
            }
            // Unnormalized coordinates are in texels; honoured only with non-wrapping addressing (03b §3.3), which the
            // sampler setup must check.
            if (Bits(w0, 25, 1) && dimension != 3)
                Line("tu /= tsize;");
            // Offsets are in half texels, applied at the base level as Xenia does (03b §3.5).
            const int32_t ox = SignExtend(Bits(w2, 16, 5), 5), oy = SignExtend(Bits(w2, 21, 5), 5);
            if ((ox || oy) && dimension != 3)
            {
                if (dimension == 0)
                    Line("tu += {} / tsize;", float(ox) * 0.5f);
                else
                    Line("tu += float2({}, {}) / tsize;", float(ox) * 0.5f, float(oy) * 0.5f);
            }
            // LOD bias: instruction (1/16 units) + fetch constant (dword 4 bits 12-21, 1/32 units) (03b §3.4).
            Line("float tbias = {} + float(int(td4 << 10) >> 22) / 32.0;", float(SignExtend(Bits(w2, 2, 7), 7)) / 16.0f);
            // UseRegisterLOD adds the setTexLOD value (LOD is additive, 03b §3.4).
            if (Bits(w1, 29, 1))
                Line("tbias += s.regLod;");
            const std::string object = std::format("xe_texture_{}", texture);
            const std::string samplerName = std::format("xe_sampler_{}", sampler);
            if (!vertex_ && Bits(w1, 28, 1))
                Line("float4 tv = {}.SampleBias({}, tu, tbias);", object, samplerName);
            else
                // No implicit derivatives (vertex shader) or computed LOD disabled: LOD = bias only.
                Line("float4 tv = {}.SampleLevel({}, tu, tbias);", object, samplerName);
            // Bias signs, the fetch-constant swizzle and ExpAdjust (xe_tex_finish), then the instruction's destination
            // swizzle. The other sign/number conversions happen before filtering (texture.h).
            Line("float4 tf = xe_tex_finish(tv, td0, td3);");
            EmitFetchDestination(w0, w1, "tf");
            indent_--;
            Line("}}");
        }

        // ---- Control flow -------------------------------------------------------------------------------------------

        void Translator::EmitExecBody(uint64_t cf)
        {
            const uint32_t address = Bits(cf, 0, 12), count = Bits(cf, 12, 3), sequence = Bits(cf, 16, 12);
            const auto& words = shader_.words;
            for (uint32_t i = 0; i < count; i++)
            {
                currentSlot_ = address + i;
                const uint32_t w0 = words[currentSlot_ * 3], w1 = words[currentSlot_ * 3 + 1], w2 = words[currentSlot_ * 3 + 2];
                Line("// slot {}", currentSlot_);
                if ((sequence >> (2 * i)) & 1)
                {
                    const uint32_t opcode = Bits(w0, 0, 5);
                    if (opcode == 0)
                        EmitVertexFetch(w0, w1, w2);
                    else
                        EmitTextureFetch(opcode, w0, w1, w2);
                }
                else
                {
                    EmitAlu(Alu{ w0, w1, w2 });
                }
                if (!vertex_ && g_probe.hash == shader_.hash && g_probe.slot == currentSlot_)
                    Line("s.probe = s.r[{}];", g_probe.reg);
            }
        }

        std::string Translator::ExecCondition(uint64_t cf) const
        {
            const uint32_t op = Bits(cf, 44, 4);
            const bool condition = Bits(cf, 42, 1);
            switch (op)
            {
            case kCfCondExec: case kCfCondExecEnd: case kCfCondExecPredClean: case kCfCondExecPredCleanEnd:
                return std::format("xe_bool({}u) == {}", Bits(cf, 34, 8), condition ? "true" : "false");
            case kCfCondExecPred: case kCfCondExecPredEnd:
                // Per invocation (Xenia); hardware evaluates CF predication over 64 invocations (03a §5).
                return std::format("s.p0 == {}", condition ? "true" : "false");
            default:
                return "true";
            }
        }

        void Translator::EmitLinear()
        {
            Line("bool xe_done = false;");
            for (uint32_t index = 0; index < cfCount_; index++)
            {
                const uint64_t cf = ControlFlow(shader_.words, index);
                const uint32_t op = Bits(cf, 44, 4);
                if (!IsExec(op))
                    continue; // nop, alloc, vfetche: no effect on registers
                Line("// cf {}", index);
                Line("if (!xe_done && ({}))", ExecCondition(cf));
                Line("{{");
                indent_++;
                EmitExecBody(cf);
                if (IsExecEnd(op))
                    Line("xe_done = true;");
                indent_--;
                Line("}}");
            }
        }

        std::string Translator::Jump(uint32_t target) const
        {
            return std::format("xe_pc = {}u; xe_jumped = true;", target < cfCount_ ? target : 0xFFFFu);
        }

        // pc-dispatch loop for shaders with jumps, calls or loops (03a §4, §7): one block per label, in ascending
        // order, each statement guarded by "no jump taken in this iteration". A block that ends normally sets pc to
        // the next label, which runs in the same iteration; a taken jump runs its target in the next iteration.
        // No switch and no `continue`: FXC fails on a `break` out of an if inside a case ("internal error: l-value
        // expected"), forbids `continue` in a switch, and with `continue` in an if-chain it dropped the register writes
        // of a jump target's block (found by the CB_SHADER_TEST control-flow programs).
        void Translator::EmitDispatch()
        {
            Line("uint xe_pc = 0u;");
            Line("[loop] while (xe_pc != 0xFFFFu)");
            Line("{{");
            indent_++;
            Line("bool xe_jumped = false;");
            std::vector<uint32_t> starts(labels_.begin(), labels_.end());
            for (size_t l = 0; l < starts.size(); l++)
            {
                const uint32_t first = starts[l];
                if (first >= cfCount_)
                    break;
                const uint32_t next = l + 1 < starts.size() && starts[l + 1] < cfCount_ ? starts[l + 1] : cfCount_;
                Line("if (xe_pc == {}u && !xe_jumped)", first);
                Line("{{");
                indent_++;
                for (uint32_t index = first; index < next; index++)
                {
                    const uint64_t cf = ControlFlow(shader_.words, index);
                    const uint32_t op = Bits(cf, 44, 4);
                    const bool condition = Bits(cf, 42, 1);
                    const uint32_t target = Bits(cf, 0, 13);
                    if (op == kCfNop || op == kCfAlloc || op == kCfMarkVsFetchDone)
                        continue;
                    Line("// cf {}", index);
                    Line("if (!xe_jumped)");
                    Line("{{");
                    indent_++;
                    auto decision = [&]() -> std::string {
                        if (Bits(cf, 13, 1))
                            return "true";
                        if (Bits(cf, 14, 1))
                            return std::format("s.p0 == {}", condition ? "true" : "false");
                        return std::format("xe_bool({}u) == {}", Bits(cf, 34, 8), condition ? "true" : "false");
                    };
                    const char* pop = "if (s.loopDepth > 0) { s.loopDepth--; s.aL = xe_aL_stack[s.loopDepth]; "
                                      "s.iter = xe_iter_stack[s.loopDepth]; }";
                    if (IsExec(op))
                    {
                        Line("if ({})", ExecCondition(cf));
                        Line("{{");
                        indent_++;
                        EmitExecBody(cf);
                        if (IsExecEnd(op))
                            Line("xe_pc = 0xFFFFu; xe_jumped = true;");
                        indent_--;
                        Line("}}");
                    }
                    else switch (op)
                    {
                    case kCfCondJmp:
                        Line("if ({}) {{ {} }}", decision(), Jump(target));
                        break;
                    case kCfCondCall:
                        Line("if ({})", decision());
                        Line("{{");
                        Line("    if (s.callDepth < 4) {{ xe_call_stack[s.callDepth] = {}u; s.callDepth++; }}",
                             index + 1 < cfCount_ ? index + 1 : 0xFFFFu);
                        Line("    {}", Jump(target));
                        Line("}}");
                        break;
                    case kCfReturn:
                        // ret with an empty stack continues with the next CF (03a §4).
                        Line("if (s.callDepth > 0) {{ s.callDepth--; xe_pc = xe_call_stack[s.callDepth]; xe_jumped = true; }}");
                        break;
                    case kCfLoopStart:
                        Line("uint lc = xe_loop_constant({}u);", Bits(cf, 16, 5));
                        Line("if (s.loopDepth < 4) {{ xe_aL_stack[s.loopDepth] = s.aL; xe_iter_stack[s.loopDepth] = s.iter; s.loopDepth++; }}");
                        Line("s.iter = 0u;");
                        if (!Bits(cf, 13, 1))
                            Line("s.aL = int((lc >> 8) & 0xFFu);");
                        Line("if ((lc & 0xFFu) == 0u) {{ {} {} }}", pop, Jump(target));
                        break;
                    case kCfLoopEnd:
                        Line("uint lc = xe_loop_constant({}u);", Bits(cf, 16, 5));
                        Line("s.iter++;");
                        Line("if (s.iter < (lc & 0xFFu){})", Bits(cf, 21, 1)
                             ? std::format(" && !(s.p0 == {})", condition ? "true" : "false") : std::string());
                        Line("{{ s.aL += int(lc << 8) >> 24; {} }}", Jump(target));
                        Line("else {{ {} }}", pop);
                        break;
                    }
                    indent_--;
                    Line("}}");
                }
                Line("if (!xe_jumped) xe_pc = {}u;", next < cfCount_ ? next : 0xFFFFu);
                indent_--;
                Line("}}");
            }
            // Every block fell through (or pc named no block): the program is done.
            Line("if (!xe_jumped) xe_pc = 0xFFFFu;");
            indent_--;
            Line("}}");
        }

        // ---- Shader assembly ----------------------------------------------------------------------------------------

        std::string Translator::Declarations() const
        {
            std::string text;
            for (uint32_t i = 0; i < result_.textures.size(); i++)
                text += std::format("{} xe_texture_{} : register(t{}, space1);\n",
                                    result_.textures[i].dimension == 0 ? "Texture1D<float4>" :
                                    result_.textures[i].dimension == 3 ? "TextureCube<float4>" : "Texture2D<float4>", i, i);
            for (uint32_t i = 0; i < result_.samplers.size(); i++)
                text += std::format("SamplerState xe_sampler_{} : register(s{});\n", i, i);
            text += R"(
float4 xe_b2f4(bool4 b) { return b ? float4(1.0, 1.0, 1.0, 1.0) : float4(0.0, 0.0, 0.0, 0.0); }

struct XeState
{
    float4 r[64];
    float ps;
    bool p0;
    int a0;
    int aL;
    bool killed;
    int vfBase;
    uint vfEndian;
    float regLod;
    uint iter;
    int loopDepth;
    int aLStack[4];
    uint iterStack[4];
    int callDepth;
    uint callStack[4];
    float4 o[16];
    float4 pos;
    float4 pts;
    float4 color[4];
    float4 depth;
    float4 probe; // CB_PS_PROBE
};

// Registers start at a zero read from the system constants: with literal zeros FXC folds arithmetic on unwritten
// registers non-IEEE (it rewrote x - (-0) as x, wrong for x = -0).
XeState xe_initial_state()
{
    XeState s = (XeState)0;
    float4 zero = asfloat(xe_sys_misc.wwww);
    [unroll] for (uint i = 0; i < 64u; i++)
        s.r[i] = zero;
    s.ps = zero.x;
    s.regLod = zero.x;
    s.probe = zero;
    return s;
}
)";
            return text;
        }

        std::string Translator::EntryPoint() const
        {
            std::string text;
            const uint32_t registers = std::min<uint32_t>(options_.registerCount, 64);
            if (options_.computeHarness)
            {
                text += std::format(R"(
RWByteAddressBuffer xe_test_io : register(u0);

[numthreads(1, 1, 1)]
void main(uint3 id : SV_DispatchThreadID)
{{
    XeState s = xe_initial_state();
    uint base = id.x * {}u;
    [unroll] for (uint i = 0; i < {}u; i++)
        s.r[i] = xe_flush4(asfloat(xe_test_io.Load4(base + i * 16u)));
    uint4 state = xe_test_io.Load4(base + {}u);
    s.ps = asfloat(state.x);
    s.p0 = state.y != 0u;
    s.a0 = int(state.z);
    xe_run(s);
    [unroll] for (uint j = 0; j < {}u; j++)
        xe_test_io.Store4(base + j * 16u, asuint(s.r[j]));
    xe_test_io.Store4(base + {}u, uint4(asuint(s.ps), s.p0 ? 1u : 0u, uint(s.a0), s.killed ? 1u : 0u));
    xe_test_io.Store4(base + {}u, asuint({}));
}}
)", kHarnessCaseBytes, kHarnessRegisters, kHarnessRegisters * 16, kHarnessRegisters, kHarnessRegisters * 16,
                                    kHarnessRegisters * 16 + 16, vertex_ ? "s.o[0]" : "s.color[0]");
                return text;
            }

            if (vertex_)
            {
                text += "\nstruct XeVertexOutput\n{\n    float4 position : SV_Position;\n";
                for (uint32_t i = 0; i < 16; i++)
                    text += std::format("    float4 o{} : TEXCOORD{};\n", i, i);
                text += "    float4 clip0 : SV_ClipDistance0;\n    float2 clip1 : SV_ClipDistance1;\n};\n";
                if (options_.domain == 1)
                    text += R"(
// A tessellated quad: the tessellator's point (u, v) and the patch's four corners (their draw positions, as the
// control-point shader passed them), in the registers the guest's tessellated vertex shader reads.
struct XeControlPoint
{
    uint position : XEPOSITION;
};
struct XePatchConstants
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

[domain("quad")]
XeVertexOutput main(XePatchConstants xe_patch, float2 xe_uv : SV_DomainLocation,
                    const OutputPatch<XeControlPoint, 4> xe_corners)
{
    XeState s = xe_initial_state();
    s.r[0].xy = xe_uv;
    s.r[0].z = xe_vertex_index(xe_corners[0].position);
    s.r[1].x = xe_vertex_index(xe_corners[1].position);
    s.r[1].y = xe_vertex_index(xe_corners[2].position);
    s.r[1].z = xe_vertex_index(xe_corners[3].position);
    xe_run(s);)";
                else
                    text += R"(
XeVertexOutput main(uint xe_vertex_id : SV_VertexID)
{
    XeState s = xe_initial_state();
    s.r[0].x = xe_vertex_index(xe_vertex_id);
    xe_run(s);)";
                text += R"(
    XeVertexOutput output;
    // Undo the PA_CL_VTE_CNTL formats so the position is ordinary clip space (01a §5).
    precise float4 p = s.pos;
    if (!(xe_sys_flags.x & XE_SYS_VTX_W0_FMT))
        p.w = rcp(p.w);
    if (xe_sys_flags.x & XE_SYS_VTX_XY_FMT)
        p.xy *= p.w;
    if (xe_sys_flags.x & XE_SYS_VTX_Z_FMT)
        p.z *= p.w;
    // User clip planes in guest clip space.
    float distances[6];
    [unroll] for (uint i = 0; i < 6u; i++)
        distances[i] = (xe_sys_misc.x >> i) & 1u ? dot(p, xe_sys_clip_planes[i]) : 0.0;
    output.clip0 = float4(distances[0], distances[1], distances[2], distances[3]);
    output.clip1 = float2(distances[4], distances[5]);
    // Guest viewport, window offset and pixel-center convention mapped onto the host viewport (draw code).
    precise float3 host = p.xyz * xe_sys_ndc_scale.xyz + xe_sys_ndc_offset.xyz * p.w;
    output.position = float4(host, p.w);
)";
                for (uint32_t i = 0; i < 16; i++)
                    text += std::format("    output.o{} = s.o[{}];\n", i, i);
                text += "    return output;\n}\n";
                return text;
            }

            // Pixel shader: interpolator i -> r[i] for i < min(16, GPR count); others start at 0 (Xenia).
            const uint32_t inputs = std::min<uint32_t>(registers, 16);
            const bool om = options_.useOutputMerger;
            text += "\nstruct XePixelInput\n{\n    float4 position : SV_Position;\n";
            if (options_.pointSprite)
                text += "    float4 pointCoord : TEXCOORD16;\n";
            for (uint32_t i = 0; i < inputs; i++)
            {
                std::string modifier;
                if ((options_.flatInterpolators >> i) & 1)
                    modifier = "nointerpolation ";
                else if ((options_.centroidInterpolators >> i) & 1)
                    modifier = "centroid ";
                text += std::format("    {}float4 t{} : TEXCOORD{};\n", modifier, i, i);
            }
            text += "    bool frontFace : SV_IsFrontFace;\n";
            if (om && options_.outputMerger.msaa != 0)
                text += "    uint coverage : SV_Coverage;\n";
            // MSAA: shaded per sample (each invocation owns the one sample it covers). At pixel frequency this GPU
            // (RTX 3060, 2026-10-19) sometimes ran two fully covered invocations for one pixel on a quad's shared
            // edge, so a sample was blended twice: dark specks along the dashboard background's diagonal (fed back
            // frame to frame into drifting trails) and dashes on the boot ribbons.
            if (om && options_.outputMerger.msaa != 0)
                text += "    uint sampleIndex : SV_SampleIndex;\n";
            text += "};\n";
            if (om)
            {
                text += OutputMergerHlsl(options_.outputMerger, OutputMergerTarget::kPixelShader);
                text += "\nvoid main(XePixelInput input)\n{\n    XeState s = xe_initial_state();\n";
            }
            else
            {
                if (options_.alphaTest & 8)
                    text += AlphaTestHlsl(options_.alphaTest);
                text += "\nstruct XePixelOutput\n{\n";
                for (uint32_t i = 0; i < 4; i++)
                    text += std::format("    float4 color{} : SV_Target{};\n", i, i);
                if (shader_.analysis.writesDepth)
                    text += "    float depth : SV_Depth;\n";
                text += "};\n\nXePixelOutput main(XePixelInput input)\n{\n    XeState s = xe_initial_state();\n";
            }
            for (uint32_t i = 0; i < inputs; i++)
                text += std::format("    s.r[{}] = xe_flush4(input.t{});\n", i, i);
            if (options_.paramGen && options_.paramGenRegister < registers)
                text += std::format(R"(    {{
        // PsParamGen (01a §3): |XY| = pixel position (D3D9 integer centers), X sign = back face, Y sign = point,
        // Z sign = line, W = 0; for points ZW = the sprite coordinates (0..1 left to right, top to bottom; Xenia).
        float2 position = floor(input.position.xy);
        uint x = asuint(position.x), y = asuint(position.y), z = 0u;
        if ((xe_sys_misc.z & 1u) && !input.frontFace)
            x |= 0x80000000u;
        if (xe_sys_misc.z & 2u)
            y |= 0x80000000u;
        if (xe_sys_misc.z & 4u)
            z |= 0x80000000u;
        s.r[{}] = float4(asfloat(x), asfloat(y), asfloat(z), 0.0);
{}    }}
)", options_.paramGenRegister, options_.pointSprite ?
                    std::format("        s.r[{}].zw = saturate(input.pointCoord.xy);\n", options_.paramGenRegister) : "");
            if (om)
            {
                // Depth derivatives before any divergent control flow (per-sample depth, polygon offset).
                text += "    float2 xe_dz = float2(ddx(input.position.z), ddy(input.position.z));\n";
                text += "    xe_run(s);\n    if (s.killed)\n        return;\n";
                text += ProbeColor();
                text += "    float4 xe_colors[4] = { s.color[0], s.color[1], s.color[2], s.color[3] };\n";
                if (options_.outputMerger.msaa != 0)
                    text += "    uint xe_cov = input.coverage & (1u << input.sampleIndex);\n";
                text += std::format("    xe_output_merger(input.position, xe_dz, xe_colors, s.depth.x, input.frontFace, {});\n}}\n",
                                    options_.outputMerger.msaa != 0 ? "xe_cov" : "1u");
                return text;
            }
            text += "    xe_run(s);\n    if (s.killed)\n        discard;\n";
            text += ProbeColor();
            if (options_.alphaTest & 8)
                text += "    if (!xe_alpha_pass(s.color[0].w))\n        discard;\n";
            text += "    XePixelOutput output;\n";
            for (uint32_t i = 0; i < 4; i++)
                text += std::format("    output.color{} = s.color[{}];\n", i, i);
            if (shader_.analysis.writesDepth)
                text += "    output.depth = s.depth.x;\n";
            text += "    return output;\n}\n";
            return text;
        }

        TranslatedShader Translator::Run()
        {
            const auto& words = shader_.words;
            cfCount_ = shader_.analysis.cfCount;
            bool dispatch = false;
            labels_.insert(0);
            for (uint32_t index = 0; index < cfCount_; index++)
            {
                const uint64_t cf = ControlFlow(words, index);
                const uint32_t op = Bits(cf, 44, 4);
                if (op == kCfLoopStart || op == kCfLoopEnd || op == kCfCondJmp || op == kCfCondCall)
                {
                    labels_.insert(Bits(cf, 0, 13));
                    dispatch = true;
                }
                if (op == kCfCondCall)
                    labels_.insert(index + 1); // return address
                if (op == kCfReturn)
                    dispatch = true;
            }

            if (dispatch)
                EmitDispatch();
            else
                EmitLinear();

            result_.hlsl = std::format("// {} shader {:016X}\n", StageName(shader_.stage), shader_.hash);
            result_.hlsl += kXenosHlslLibrary;
            result_.hlsl += Declarations();
            // The body works on local copies of the arrays: FXC cannot keep a [loop] when arrays inside an inout struct
            // are written with dynamic indices (error X3531), local arrays are fine.
            result_.hlsl += "\nvoid xe_run(inout XeState s)\n{\n"
                            "    float4 r[64] = s.r;\n"
                            "    int xe_aL_stack[4] = s.aLStack;\n"
                            "    uint xe_iter_stack[4] = s.iterStack;\n"
                            "    uint xe_call_stack[4] = s.callStack;\n" +
                            body_ +
                            "    s.r = r;\n"
                            "    s.aLStack = xe_aL_stack;\n"
                            "    s.iterStack = xe_iter_stack;\n"
                            "    s.callStack = xe_call_stack;\n"
                            "}\n";
            result_.hlsl += EntryPoint();
            return std::move(result_);
        }
    }

    TranslatedShader TranslateShader(const Shader& shader, const TranslationOptions& options)
    {
        Translator translator(shader, options);
        return translator.Run();
    }

    namespace
    {
        // Compiled bytecode is kept on disk (shader_cache/ in the working directory, i.e. beside TU46.xex) so a shader
        // is compiled once, not at its first use in every run: FXC takes up to seconds for the larger translated
        // shaders, and the command processor waits for it (a hitch when a world loads or a new effect appears).
        // Each file holds the exact compile input (target, flags, source), so a hit is the same compile.
        // CB_SHADER_CACHE=0 turns it off.
        std::mutex g_cacheMutex;

        const std::filesystem::path& CacheDirectory()
        {
            static const std::filesystem::path directory = [] {
                const char* value = std::getenv("CB_SHADER_CACHE");
                if (value && std::string(value) == "0")
                    return std::filesystem::path();
                return std::filesystem::path("shader_cache");
            }();
            return directory;
        }

        uint64_t Fnv1a(const std::string& text)
        {
            uint64_t hash = 0xCBF29CE484222325ull;
            for (unsigned char c : text)
                hash = (hash ^ c) * 0x100000001B3ull;
            return hash;
        }

        bool ReadCached(const std::filesystem::path& file, const std::string& key, std::vector<uint8_t>& bytecode)
        {
            std::ifstream in(file, std::ios::binary);
            if (!in)
                return false;
            const std::string data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
            if (data.size() < key.size() + 8 || std::memcmp(data.data(), "MCSC", 4) != 0)
                return false;
            uint32_t keySize = 0;
            std::memcpy(&keySize, data.data() + 4, 4);
            if (keySize != key.size() || data.compare(8, key.size(), key) != 0)
                return false;
            bytecode.assign(data.begin() + 8 + std::ptrdiff_t(key.size()), data.end());
            return !bytecode.empty();
        }

        void WriteCached(const std::filesystem::path& file, const std::string& key, const std::vector<uint8_t>& bytecode)
        {
            std::error_code error;
            std::filesystem::create_directories(file.parent_path(), error);
            const std::filesystem::path temporary = file.string() + ".tmp";
            {
                std::ofstream out(temporary, std::ios::binary);
                const uint32_t keySize = uint32_t(key.size());
                out.write("MCSC", 4);
                out.write(reinterpret_cast<const char*>(&keySize), 4);
                out.write(key.data(), std::streamsize(key.size()));
                out.write(reinterpret_cast<const char*>(bytecode.data()), std::streamsize(bytecode.size()));
                if (!out)
                    return;
            }
            std::filesystem::rename(temporary, file, error);
        }
    }

    namespace
    {
        // DXC (Shader Model 6): dxcompiler.dll + dxil.dll beside the exe (build.bat copies the Windows SDK's; they are
        // redistributable), loaded at first use. Faster compiles than FXC for the large translated shaders and DXIL
        // the drivers optimise better. CB_DXC=0, a missing DLL or a shader DXC rejects: FXC as before (per shader).
        struct Dxc
        {
            ComPtr<IDxcUtils> utils;
            ComPtr<IDxcCompiler3> compiler;
        };

        const Dxc* LoadDxc()
        {
            static const std::optional<Dxc> dxc = []() -> std::optional<Dxc> {
                if (const char* v = std::getenv("CB_DXC"); v && std::string(v) == "0")
                    return std::nullopt;
                HMODULE module = LoadLibraryW(L"dxcompiler.dll");
                if (!module)
                {
                    Log("GPU: dxcompiler.dll not found: shaders compile with FXC (Shader Model 5.1)");
                    return std::nullopt;
                }
                auto create = reinterpret_cast<DxcCreateInstanceProc>(GetProcAddress(module, "DxcCreateInstance"));
                Dxc d;
                if (!create || FAILED(create(CLSID_DxcUtils, IID_PPV_ARGS(&d.utils))) ||
                    FAILED(create(CLSID_DxcCompiler, IID_PPV_ARGS(&d.compiler))))
                {
                    Log("GPU: dxcompiler.dll does not start: shaders compile with FXC");
                    return std::nullopt;
                }
                Log("GPU: shaders compile with DXC (Shader Model 6.0)");
                return d;
            }();
            return dxc ? &*dxc : nullptr;
        }

        // vs_5_1 -> vs_6_0 ...
        std::wstring Sm6Target(const char* target)
        {
            std::wstring t(target, target + std::strlen(target));
            if (t.size() >= 4)
                t = t.substr(0, 3) + L"6_0";
            return t;
        }

        bool CompileDxc(const Dxc& dxc, const std::string& source, const char* target, std::vector<uint8_t>& bytecode,
                        std::string& errors)
        {
            DxcBuffer buffer{ source.data(), source.size(), DXC_CP_UTF8 };
            const std::wstring t = Sm6Target(target);
            // -Gis: IEEE strictness (as FXC's D3DCOMPILE_IEEE_STRICTNESS); -O3; -HV 2018: the HLSL FXC accepts (the
            // translator's per-component `cond ? a : b` on vectors; HLSL 2021 wants select()).
            LPCWSTR args[] = { L"-E", L"main", L"-T", t.c_str(), L"-O3", L"-Gis", L"-HV", L"2018" };
            ComPtr<IDxcResult> result;
            if (FAILED(dxc.compiler->Compile(&buffer, args, UINT32(std::size(args)), nullptr, IID_PPV_ARGS(&result))))
                return false;
            HRESULT status = E_FAIL;
            result->GetStatus(&status);
            ComPtr<IDxcBlobUtf8> messages;
            if (SUCCEEDED(result->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&messages), nullptr)) && messages)
                errors.assign(messages->GetStringPointer(), messages->GetStringLength());
            if (FAILED(status))
                return false;
            ComPtr<IDxcBlob> object;
            if (FAILED(result->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&object), nullptr)) || !object)
                return false;
            bytecode.assign(static_cast<const uint8_t*>(object->GetBufferPointer()),
                            static_cast<const uint8_t*>(object->GetBufferPointer()) + object->GetBufferSize());
            return !bytecode.empty();
        }
    }

    bool CompileHlsl(const std::string& source, const char* target, const char* name, std::vector<uint8_t>& bytecode,
                     std::string& errors)
    {
        constexpr UINT kFlags = D3DCOMPILE_IEEE_STRICTNESS | D3DCOMPILE_OPTIMIZATION_LEVEL3;
        const Dxc* dxc = LoadDxc();
        // The cache key names the compiler: DXIL and DXBC entries never mix.
        const std::string key = std::format("{}|{:08X}|{}|", target, kFlags, dxc ? "dxc6" : "fxc") + source;
        std::filesystem::path file;
        if (!CacheDirectory().empty())
        {
            file = CacheDirectory() / std::format("{:016X}.bin", Fnv1a(key));
            std::lock_guard lock(g_cacheMutex);
            if (ReadCached(file, key, bytecode))
            {
                errors.clear();
                return true;
            }
        }
        const auto started = std::chrono::steady_clock::now();
        if (dxc)
        {
            std::string dxcErrors;
            if (CompileDxc(*dxc, source, target, bytecode, dxcErrors))
            {
                errors.clear();
                Log("GPU: compiled {} ({}, DXC) in {:.0f} ms", name, target,
                    std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - started).count());
                if (!file.empty())
                {
                    std::lock_guard lock(g_cacheMutex);
                    WriteCached(file, key, bytecode);
                }
                return true;
            }
            Log("GPU: DXC rejected {} ({}); FXC instead: {}", name, target, dxcErrors.substr(0, 300));
        }
        ComPtr<ID3DBlob> code, messages;
        const HRESULT hr = D3DCompile(source.data(), source.size(), name, nullptr, nullptr, "main", target, kFlags, 0,
                                      &code, &messages);
        errors = messages ? std::string(static_cast<const char*>(messages->GetBufferPointer()), messages->GetBufferSize())
                          : std::string();
        if (FAILED(hr))
            return false;
        bytecode.assign(static_cast<const uint8_t*>(code->GetBufferPointer()),
                        static_cast<const uint8_t*>(code->GetBufferPointer()) + code->GetBufferSize());
        Log("GPU: compiled {} ({}) in {:.0f} ms", name, target,
            std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - started).count());
        if (!file.empty())
        {
            std::lock_guard lock(g_cacheMutex);
            WriteCached(file, key, bytecode);
        }
        return true;
    }
}
