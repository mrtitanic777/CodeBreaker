// Xenos shader microcode disassembler. Field layouts: analysis/xenos/03a (control flow), 03b (fetch), 03c (ALU),
// cross-checked against the XDK's d3d9gpu.h and Xenia's ucode.h (BSD-3); the code is our own. The text is our own
// XNA-like syntax, meant for reading and diffing, not for reassembly.

#include "gpu/ucode.h"
#include "gpu/ucode_fields.h"

#include <algorithm>
#include <format>
#include <set>

namespace gpu::ucode
{
    namespace
    {
        const char kComponents[] = "xyzw";

        // ---- Shared helpers ---------------------------------------------------------------------------------

        struct Context
        {
            Stage stage;
            Analysis& analysis;
            std::set<uint32_t> textureConstants;
            std::set<uint32_t> vertexConstants;
            std::string text;

            void Error(std::string message)
            {
                text += "    ; ERROR: " + message + "\n";
                analysis.errors.push_back(std::move(message));
            }
        };

        // Export register name for `index` (03c §4; Xenia ucode.h ExportRegister), empty if undefined.
        std::string ExportName(Stage stage, uint32_t index, Analysis& analysis)
        {
            if (index == 32)
            {
                analysis.memoryExport = true;
                return "eA";
            }
            if (index >= 33 && index <= 37)
            {
                analysis.memoryExport = true;
                return std::format("eM{}", index - 33);
            }
            if (stage == Stage::kVertex)
            {
                if (index < 16)
                {
                    analysis.interpolatorsWritten |= 1u << index;
                    return std::format("o{}", index);
                }
                if (index == 62)
                {
                    analysis.writesPosition = true;
                    return "oPos";
                }
                if (index == 63)
                {
                    analysis.writesPointSize = true;
                    return "oPts";
                }
                return {};
            }
            if (index < 4)
            {
                analysis.colorsWritten |= 1u << index;
                return std::format("oC{}", index);
            }
            if (index == 61)
            {
                analysis.writesDepth = true;
                return "oDepth";
            }
            return {};
        }

        std::string Predicate(bool predicated, bool condition)
        {
            if (!predicated)
                return {};
            return condition ? "(p0) " : "(!p0) ";
        }

        // ---- Fetch instructions (03b) -----------------------------------------------------------------------

        std::string FetchDestination(uint32_t reg, bool relative, uint32_t swizzle, Context& c)
        {
            std::string text = relative ? std::format("r[{}+aL]", reg) : std::format("r{}", reg);
            text += '.';
            for (uint32_t i = 0; i < 4; i++)
            {
                const uint32_t select = (swizzle >> (3 * i)) & 7;
                if (select < 4)
                    text += kComponents[select];
                else if (select == 4)
                    text += '0';
                else if (select == 5)
                    text += '1';
                else if (select == 7)
                    text += '_';
                else
                {
                    text += '?';
                    c.Error(std::format("fetch destination swizzle {} (6 is undefined)", select));
                }
            }
            return text;
        }

        std::string FetchSource(uint32_t reg, bool relative, uint32_t swizzle, uint32_t components)
        {
            std::string text = relative ? std::format("r[{}+aL]", reg) : std::format("r{}", reg);
            text += '.';
            for (uint32_t i = 0; i < components; i++)
                text += kComponents[(swizzle >> (2 * i)) & 3];
            return text;
        }

        const char* VertexFormatName(uint32_t format)
        {
            switch (format)
            {
            case 6: return "8_8_8_8";
            case 7: return "2_10_10_10";
            case 16: return "10_11_11";
            case 17: return "11_11_10";
            case 25: return "16_16";
            case 26: return "16_16_16_16";
            case 31: return "16_16_FLOAT";
            case 32: return "16_16_16_16_FLOAT";
            case 33: return "32";
            case 34: return "32_32";
            case 35: return "32_32_32_32";
            case 36: return "32_FLOAT";
            case 37: return "32_32_FLOAT";
            case 38: return "32_32_32_32_FLOAT";
            case 57: return "32_32_32_FLOAT";
            default: return nullptr;
            }
        }

        void DisassembleVertexFetch(uint32_t w0, uint32_t w1, uint32_t w2, Context& c)
        {
            const bool mini = Bits(w1, 30, 1);
            const uint32_t constIndex = Bits(w0, 20, 5), constSelect = Bits(w0, 25, 2);
            const uint32_t format = Bits(w1, 16, 6);
            std::string line = "    " + Predicate(Bits(w1, 31, 1), Bits(w2, 31, 1));
            line += mini ? "vfetch_mini " : "vfetch_full ";
            line += FetchDestination(Bits(w0, 12, 6), Bits(w0, 18, 1), Bits(w1, 0, 12), c);
            if (!mini)
            {
                const uint32_t hw = constIndex * 3 + constSelect;
                line += ", " + FetchSource(Bits(w0, 5, 6), Bits(w0, 11, 1), Bits(w0, 30, 2), 1);
                line += std::format(", vf{}", hw);
                if (constSelect == 3)
                    c.Error(std::format("vertex fetch constant select 3 (group {})", constIndex));
                else
                    c.vertexConstants.insert(hw);
                line += std::format(" stride={}", Bits(w2, 0, 8));
                if (Bits(w1, 15, 1))
                    line += " round_index";
                line += std::format(" prefetch={}", Bits(w0, 27, 3) + 1);
            }
            const char* name = VertexFormatName(format);
            line += name ? std::format(" format={}", name) : std::format(" format=#{}", format);
            line += std::format(" offset={}", SignExtend(Bits(w2, 8, 23), 23));
            line += Bits(w1, 12, 1) ? " signed" : " unsigned";
            line += Bits(w1, 13, 1) ? " integer" : " fraction";
            if (Bits(w1, 14, 1))
                line += " signed_rf_no_zero";
            if (const int32_t exp = SignExtend(Bits(w1, 24, 6), 6))
                line += std::format(" exp_adjust={}", exp);
            c.text += line + "\n";
            if (!name)
                c.Error(std::format("vertex format {} is not a vertex format", format));
            if (!mini && Bits(w0, 19, 1) == 0)
                c.Error("vertex fetch with must_be_one = 0");
        }

        void DisassembleTextureFetch(uint32_t opcode, uint32_t w0, uint32_t w1, uint32_t w2, Context& c)
        {
            static const char* const kDimensions[] = { "1D", "2D", "3D", "Cube" };
            const uint32_t dimension = Bits(w2, 14, 2);
            const uint32_t coords[] = { 1, 2, 3, 3 };
            std::string name;
            uint32_t sourceComponents = coords[dimension];
            bool hasDestination = true;
            switch (opcode)
            {
            case 1: name = std::string("tfetch") + kDimensions[dimension]; break;
            case 16: name = std::string("getBCF") + kDimensions[dimension]; break;
            case 17: name = std::string("getCompTexLOD") + kDimensions[dimension]; break;
            case 18: name = "getGradients"; sourceComponents = 2; break;
            case 19: name = std::string("getWeights") + kDimensions[dimension]; break;
            case 24: name = "setTexLOD"; sourceComponents = 1; hasDestination = false; break;
            case 25: name = "setGradientH"; sourceComponents = 3; hasDestination = false; break;
            case 26: name = "setGradientV"; sourceComponents = 3; hasDestination = false; break;
            default:
                c.text += std::format("    fetch_op#{} {:08X} {:08X} {:08X}\n", opcode, w0, w1, w2);
                c.Error(std::format("texture fetch opcode {} is undefined", opcode));
                return;
            }
            std::string line = "    " + Predicate(Bits(w1, 31, 1), Bits(w2, 31, 1)) + name + " ";
            if (hasDestination)
                line += FetchDestination(Bits(w0, 12, 6), Bits(w0, 18, 1), Bits(w1, 0, 12), c) + ", ";
            line += FetchSource(Bits(w0, 5, 6), Bits(w0, 11, 1), Bits(w0, 26, 6), sourceComponents);
            const uint32_t tf = Bits(w0, 20, 5);
            if (opcode != 24 && opcode != 25 && opcode != 26)
            {
                line += std::format(", tf{}", tf);
                c.textureConstants.insert(tf);
            }

            // "" = keep (use the fetch constant's value), nullptr = undefined [d3d9gpu.h GPUMINMAGFILTER etc.].
            static const char* const kMinMag[] = { "point", "linear", nullptr, "" };
            static const char* const kMip[] = { "point", "linear", "basemap", "" };
            static const char* const kAniso[] = { "disabled", "1:1", "2:1", "4:1", "8:1", "16:1", nullptr, "" };
            std::string undefined;
            auto filter = [&](const char* label, const char* const* table, uint32_t value) {
                if (!table[value])
                {
                    line += std::format(" {}=#{}", label, value);
                    undefined = std::format("texture fetch {} filter {} is undefined", label, value);
                }
                else if (table[value][0])
                    line += std::format(" {}={}", label, table[value]);
            };
            filter("mag", kMinMag, Bits(w1, 12, 2));
            filter("min", kMinMag, Bits(w1, 14, 2));
            filter("mip", kMip, Bits(w1, 16, 2));
            filter("aniso", kAniso, Bits(w1, 18, 3));
            if (Bits(w1, 21, 3) != 7)
                line += std::format(" arbitrary_filter={}", Bits(w1, 21, 3));
            filter("vol_mag", kMinMag, Bits(w1, 24, 2));
            filter("vol_min", kMinMag, Bits(w1, 26, 2));
            if (!Bits(w1, 28, 1))
                line += " no_computed_lod";
            if (Bits(w1, 29, 1))
                line += " register_lod";
            if (Bits(w1, 30, 1))
                line += " w1.30";
            if (Bits(w2, 0, 1))
                line += " register_gradients";
            if (Bits(w2, 1, 1))
                line += " sample_location=1";
            if (const int32_t bias = SignExtend(Bits(w2, 2, 7), 7))
                line += std::format(" lod_bias={}/16", bias);
            if (Bits(w0, 25, 1))
                line += " unnormalized";
            if (Bits(w0, 19, 1))
                line += " fetch_valid_only";
            const int32_t ox = SignExtend(Bits(w2, 16, 5), 5), oy = SignExtend(Bits(w2, 21, 5), 5);
            const int32_t oz = SignExtend(Bits(w2, 26, 5), 5);
            if (ox || oy || oz)
                line += std::format(" offset=({},{},{})/2", ox, oy, oz);
            c.text += line + "\n";
            if (!undefined.empty())
                c.Error(undefined);
        }

        // ---- ALU instructions (03c) -------------------------------------------------------------------------

        const char* const kVectorOps[32] = {
            "add", "mul", "max", "min", "seq", "sgt", "sge", "sne", "frc", "trunc", "floor", "mad", "cndeq",
            "cndge", "cndgt", "dp4", "dp3", "dp2add", "cube", "max4", "setp_eq_push", "setp_ne_push",
            "setp_gt_push", "setp_ge_push", "kill_eq", "kill_gt", "kill_ge", "kill_ne", "dst", "maxa", nullptr, nullptr,
        };

        const char* const kScalarOps[64] = {
            "adds", "adds_prev", "muls", "muls_prev", "muls_prev2", "maxs", "mins", "seqs", "sgts", "sges", "snes",
            "frcs", "truncs", "floors", "exp", "logc", "log", "rcpc", "rcpf", "rcp", "rsqc", "rsqf", "rsq", "maxas",
            "maxasf", "subs", "subs_prev", "setp_eq", "setp_ne", "setp_gt", "setp_ge", "setp_inv", "setp_pop",
            "setp_clr", "setp_rstr", "kills_eq", "kills_gt", "kills_ge", "kills_ne", "kills_one", "sqrt", nullptr,
            "mulsc", "mulsc", "addsc", "addsc", "subsc", "subsc", "sin", "cos", "retain_prev",
        };

        // Register or constant name of ALU operand `i` (1-3), without swizzle or modifiers.
        std::string OperandRegister(const Alu& alu, int i)
        {
            const uint32_t reg = alu.reg(i);
            if (alu.isTemp(i))
                return (reg & 0x40) ? std::format("r[{}+aL]", reg & 0x3F) : std::format("r{}", reg & 0x3F);
            if (alu.constantRelative(i))
                return std::format("c[{}+{}]", reg, alu.relativeToA0() ? "a0" : "aL");
            return std::format("c{}", reg);
        }

        bool OperandAbs(const Alu& alu, int i)
        {
            return alu.isTemp(i) ? (alu.reg(i) & 0x80) != 0 : alu.absConstants();
        }

        std::string Modified(std::string operand, bool abs, bool negate)
        {
            if (abs)
                operand = "|" + operand + "|";
            return negate ? "-" + operand : operand;
        }

        std::string VectorOperand(const Alu& alu, int i)
        {
            std::string name = OperandRegister(alu, i);
            const uint32_t swizzle = alu.swizzle(i);
            if (swizzle != 0)
            {
                name += '.';
                for (uint32_t c = 0; c < 4; c++)
                    name += kComponents[((swizzle >> (2 * c)) + c) & 3];
            }
            // Modifiers wrap the swizzled register: abs first, then negate (03c §2.1).
            return Modified(name, OperandAbs(alu, i), alu.negate(i));
        }

        std::string WriteMask(uint32_t mask)
        {
            std::string text = ".";
            for (uint32_t c = 0; c < 4; c++)
                text += (mask >> c) & 1 ? kComponents[c] : '_';
            return text;
        }

        void DisassembleAlu(const Alu& alu, Context& c)
        {
            const std::string predicate = Predicate(alu.predicated(), alu.predicateCondition());
            if (alu.predicated())
                c.analysis.usesPredication = true;

            // Destinations (03c §4 for exports).
            std::string vectorDest, scalarDest;
            if (alu.exports())
            {
                std::string name = ExportName(c.stage, alu.vectorDest(), c.analysis);
                if (name.empty())
                {
                    name = std::format("export#{}", alu.vectorDest());
                    c.Error(std::format("{} shader export register {} is undefined", StageName(c.stage), alu.vectorDest()));
                }
                if (alu.vectorDestRelative())
                    c.Error("relative vector destination while exporting");
                std::string vectorMask = ".", scalarMask = ".";
                for (uint32_t i = 0; i < 4; i++)
                {
                    const bool v = (alu.vectorMask() >> i) & 1, s = (alu.scalarMask() >> i) & 1;
                    vectorMask += v && s ? '1' : v ? kComponents[i] : (!s && alu.scalarDestRelative()) ? '0' : '_';
                    scalarMask += s && !v ? kComponents[i] : '_';
                }
                vectorDest = name + vectorMask;
                scalarDest = name + scalarMask;
            }
            else
            {
                vectorDest = (alu.vectorDestRelative() ? std::format("r[{}+aL]", alu.vectorDest())
                                                       : std::format("r{}", alu.vectorDest())) + WriteMask(alu.vectorMask());
                scalarDest = (alu.scalarDestRelative() ? std::format("r[{}+aL]", alu.scalarDest())
                                                       : std::format("r{}", alu.scalarDest())) + WriteMask(alu.scalarMask());
            }

            // Vector half.
            const uint32_t vop = alu.vectorOp();
            std::string line = "    " + predicate;
            if (kVectorOps[vop])
            {
                line += std::format("{}{} {}", kVectorOps[vop], alu.vectorSaturate() ? "_sat" : "", vectorDest);
                for (uint32_t i = 1; i <= VectorOperandCount(vop); i++)
                    line += ", " + VectorOperand(alu, int(i));
                if (vop >= 24 && vop <= 27)
                    c.analysis.kills = true;
            }
            else
            {
                line += std::format("vector_op#{}", vop);
                c.Error(std::format("vector opcode {} is undefined", vop));
            }

            // Scalar half: operands come from src3 (03c §2.2, §2.4).
            const uint32_t sop = alu.scalarOp();
            line += "\n    " + std::string(predicate.size(), ' ') + "+ ";
            if (sop < 64 && kScalarOps[sop])
            {
                line += std::format("{}{} {}", kScalarOps[sop], alu.scalarSaturate() ? "_sat" : "", scalarDest);
                const uint32_t swizzle = alu.swizzle(3);
                const uint32_t a = ((swizzle >> 6) + 3) & 3, b = swizzle & 3;
                switch (ScalarOperandKind(sop))
                {
                case ScalarOperands::kNone:
                    break;
                case ScalarOperands::kOne:
                    line += ", " + Modified(OperandRegister(alu, 3) + "." + kComponents[a], OperandAbs(alu, 3), alu.negate(3));
                    break;
                case ScalarOperands::kTwo:
                {
                    const std::string reg = OperandRegister(alu, 3);
                    line += ", " + Modified(reg + "." + kComponents[a], OperandAbs(alu, 3), alu.negate(3));
                    line += ", " + Modified(reg + "." + kComponents[b], OperandAbs(alu, 3), alu.negate(3));
                    break;
                }
                case ScalarOperands::kConstantTemp:
                {
                    // Constant c[src3_reg] (relative as operand 3) and a temp assembled from other fields; negate and
                    // abs_constants apply to both.
                    const uint32_t constant = alu.reg(3);
                    const std::string cname = alu.constantRelative(3)
                        ? std::format("c[{}+{}]", constant, alu.relativeToA0() ? "a0" : "aL")
                        : std::format("c{}", constant);
                    const uint32_t temp = alu.scalarConstantTempReg();
                    line += ", " + Modified(cname + "." + kComponents[a], alu.absConstants(), alu.negate(3));
                    line += ", " + Modified(std::format("r{}.{}", temp, kComponents[b]), alu.absConstants(), alu.negate(3));
                    break;
                }
                }
                if (sop >= 35 && sop <= 39)
                    c.analysis.kills = true;
            }
            else
            {
                line += std::format("scalar_op#{}", sop);
                c.Error(std::format("scalar opcode {} is undefined", sop));
            }
            c.text += line + "\n";
        }
    }

    const char* StageName(Stage stage)
    {
        return stage == Stage::kVertex ? "vertex" : "pixel";
    }

    std::string Disassemble(Stage stage, const std::vector<uint32_t>& words, Analysis& analysis)
    {
        analysis = {};
        Context c{ stage, analysis };
        const uint32_t slots = uint32_t(words.size() / 3);
        c.text = std::format("; {} shader, {} dwords\n", StageName(stage), words.size());
        if (words.size() % 3)
            c.Error(std::format("size {} dwords is not a whole number of slots", words.size()));

        // The CF block ends at the lowest slot an exec refers to (03a §1, as Xenia/freedreno). Empty execs are
        // ignored: their address is meaningless.
        uint32_t boundSlots = slots;
        std::set<uint32_t> labels;
        for (uint32_t index = 0; index < boundSlots * 2; index++)
        {
            const uint64_t cf = ControlFlow(words, index);
            const uint32_t op = Bits(cf, 44, 4);
            if (IsExec(op) && Bits(cf, 12, 3) != 0)
                boundSlots = std::min(boundSlots, Bits(cf, 0, 12));
            if (op == kCfLoopStart || op == kCfLoopEnd || op == kCfCondCall || op == kCfCondJmp)
                labels.insert(Bits(cf, 0, 13));
        }
        analysis.cfCount = boundSlots * 2;

        for (uint32_t index = 0; index < analysis.cfCount; index++)
        {
            if (labels.count(index))
                c.text += std::format("label_{}:\n", index);
            const uint64_t cf = ControlFlow(words, index);
            const uint32_t op = Bits(cf, 44, 4);
            std::string line = std::format("  {:3}  ", index);
            const uint32_t boolIndex = Bits(cf, 34, 8);
            const bool condition = Bits(cf, 42, 1);
            switch (op)
            {
            case kCfNop: line += "cnop"; break;
            case kCfMarkVsFetchDone: line += "vfetche"; break;
            case kCfExec: case kCfExecEnd: line += op == kCfExec ? "exec" : "exece"; break;
            case kCfCondExec: case kCfCondExecEnd: case kCfCondExecPredClean: case kCfCondExecPredCleanEnd:
                line += std::format("{} {}b{}", IsExecEnd(op) ? "cexece" : "cexec", condition ? "" : "!", boolIndex);
                if (op >= kCfCondExecPredClean)
                    line += " (predicate_clean)";
                break;
            case kCfCondExecPred: case kCfCondExecPredEnd:
                line += std::format("{} {}", condition ? "(p0)" : "(!p0)", op == kCfCondExecPred ? "exec" : "exece");
                analysis.usesPredication = true;
                break;
            case kCfLoopStart:
                line += std::format("loop i{}, skip to {}{}", Bits(cf, 16, 5), Bits(cf, 0, 13), Bits(cf, 13, 1) ? " repeat" : "");
                analysis.usesLoops = true;
                break;
            case kCfLoopEnd:
                line += std::format("endloop i{}, body {}", Bits(cf, 16, 5), Bits(cf, 0, 13));
                if (Bits(cf, 21, 1))
                {
                    line += std::format(" break if p0 == {}", int(condition));
                    analysis.usesPredication = true;
                }
                analysis.usesLoops = true;
                break;
            case kCfCondCall: case kCfCondJmp:
            {
                line += op == kCfCondCall ? "call " : "jmp ";
                if (Bits(cf, 13, 1))
                    line += "always";
                else if (Bits(cf, 14, 1))
                {
                    line += condition ? "if p0" : "if !p0";
                    analysis.usesPredication = true;
                }
                else
                    line += std::format("if {}b{}", condition ? "" : "!", boolIndex);
                line += std::format(", {}", Bits(cf, 0, 13));
                if (op == kCfCondCall)
                    analysis.usesCalls = true;
                break;
            }
            case kCfReturn: line += "ret"; analysis.usesCalls = true; break;
            case kCfAlloc:
            {
                static const char* const kTypes[] = { "none", "position", "interpolators", "export" };
                line += std::format("alloc {} size={}{}", kTypes[Bits(cf, 41, 2)], Bits(cf, 0, 3),
                                    Bits(cf, 40, 1) ? " no_serialize" : "");
                break;
            }
            }
            c.text += line + "\n";
            if (!IsExec(op))
                continue;

            const uint32_t address = Bits(cf, 0, 12), count = Bits(cf, 12, 3), sequence = Bits(cf, 16, 12);
            if (count > 6)
                c.Error(std::format("exec at CF {} runs {} instructions (6 is the maximum)", index, count));
            for (uint32_t i = 0; i < count; i++)
            {
                const uint32_t slot = address + i;
                if (slot >= slots)
                {
                    c.Error(std::format("exec at CF {} refers to slot {} beyond the shader", index, slot));
                    break;
                }
                const uint32_t w0 = words[slot * 3], w1 = words[slot * 3 + 1], w2 = words[slot * 3 + 2];
                const bool fetch = (sequence >> (2 * i)) & 1;
                c.text += std::format("       slot {}{}:\n", slot, (sequence >> (2 * i + 1)) & 1 ? " serialize" : "");
                if (fetch)
                {
                    const uint32_t opcode = Bits(w0, 0, 5);
                    if (opcode == 0)
                        DisassembleVertexFetch(w0, w1, w2, c);
                    else
                        DisassembleTextureFetch(opcode, w0, w1, w2, c);
                }
                else
                {
                    DisassembleAlu(Alu{ w0, w1, w2 }, c);
                }
            }
        }

        analysis.textureFetchConstants.assign(c.textureConstants.begin(), c.textureConstants.end());
        analysis.vertexFetchConstants.assign(c.vertexConstants.begin(), c.vertexConstants.end());
        return std::move(c.text);
    }
}
