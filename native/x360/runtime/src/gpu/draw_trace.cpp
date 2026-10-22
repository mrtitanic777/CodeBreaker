// Per-draw GPU state trace (see draw_trace.h). Register layouts: analysis/xenos/01a, 01b, 04, 05a, 07.

#include "gpu/draw_trace.h"

#include "gpu/gpu.h"
#include "gpu/shader_cache.h"
#include "gpu/texture.h"
#include "log.h"
#include "memory.h"

#include <bit>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <format>
#include <fstream>
#include <set>
#include <string>

namespace gpu
{
    namespace
    {
        struct Range
        {
            uint64_t first = 0, last = 0;   // inclusive; first = 0: tracing off
        };

        const Range g_range = [] {
            Range range;
            const char* value = std::getenv("CB_GPU_TRACE");
            if (!value || !value[0])
                return range;
            unsigned long long first = 0, last = 0;
            const int fields = std::sscanf(value, "%llu-%llu", &first, &last);
            if (fields >= 1 && first > 0)
            {
                range.first = first;
                range.last = fields == 2 && last >= first ? last : first;
            }
            return range;
        }();

        std::ofstream g_out;
        uint64_t g_frame = 1;           // frame being built (1-based)
        bool g_headerWritten = false;
        uint32_t g_drawsInFrame = 0;
        uint32_t g_surveyDraws = 0;
        std::set<uint64_t> g_shadersInFrame;

        std::ofstream& Out()
        {
            if (!g_out.is_open())
            {
                g_out.open("gpu_trace.log");
                Log("GPU: tracing frames {}-{} to gpu_trace.log", g_range.first, g_range.last);
            }
            if (!g_headerWritten)
            {
                g_headerWritten = true;
                g_out << std::format("=== frame {}\n", g_frame);
            }
            return g_out;
        }

        uint32_t Reg(uint32_t index) { return ReadRegister(index); }
        float RegFloat(uint32_t index) { return std::bit_cast<float>(ReadRegister(index)); }

        int32_t SignExtend(uint32_t value, uint32_t bits)
        {
            const uint32_t shift = 32 - bits;
            return int32_t(value << shift) >> shift;
        }

        const char* PrimitiveName(uint32_t type)
        {
            switch (type)
            {
            case 0: return "none";
            case 1: return "point_list";
            case 2: return "line_list";
            case 3: return "line_strip";
            case 4: return "triangle_list";
            case 5: return "triangle_fan";
            case 6: return "triangle_strip";
            case 7: return "triangle_with_w_flags";
            case 8: return "rectangle_list";
            case 12: return "line_loop";
            case 13: return "quad_list";
            case 14: return "quad_strip";
            case 15: return "polygon";
            case 16: return "2d_copy_rect_list_v0";
            case 17: return "2d_copy_rect_list_v1";
            case 18: return "2d_copy_rect_list_v2";
            case 19: return "2d_copy_rect_list_v3";
            case 20: return "2d_fill_rect_list";
            case 21: return "2d_line_strip";
            case 22: return "2d_triangle_strip";
            default: return "undefined";
            }
        }

        const char* const kEndian[] = { "none", "8in16", "8in32", "16in32" };
        const char* const kEndian128[] = { "none", "8in16", "8in32", "16in32", "8in64", "8in128", "#6", "#7" };


        const char* ColorFormatName(uint32_t format)
        {
            switch (format)
            {
            case 0: return "8_8_8_8";
            case 1: return "8_8_8_8_GAMMA";
            case 2: return "2_10_10_10";
            case 3: return "2_10_10_10_FLOAT";
            case 4: return "16_16";
            case 5: return "16_16_16_16";
            case 6: return "16_16_FLOAT";
            case 7: return "16_16_16_16_FLOAT";
            case 10: return "2_10_10_10_AS_10_10_10_10";
            case 12: return "2_10_10_10_FLOAT_AS_16_16_16_16";
            case 14: return "32_FLOAT";
            case 15: return "32_32_FLOAT";
            default: return "undefined";
            }
        }

        const char* EdramModeName(uint32_t mode)
        {
            switch (mode)
            {
            case 0: return "nop";
            case 4: return "color_depth";
            case 5: return "depth_only";
            case 6: return "copy";
            default: return "undefined";
            }
        }

        std::string Swizzle(uint32_t swizzle) // 4 x 3 bits (texture fetch constant)
        {
            std::string text;
            for (uint32_t i = 0; i < 4; i++)
                text += "XYZW01??"[(swizzle >> (3 * i)) & 7];
            return text;
        }

        // Texture fetch constant `n` (6 dwords at 0x4800 + 6n), 05a §2.
        std::string DescribeTexture(uint32_t n)
        {
            uint32_t d[6];
            for (uint32_t i = 0; i < 6; i++)
                d[i] = Reg(0x4800 + n * 6 + i);
            const uint32_t type = d[0] & 3, format = d[1] & 0x3F, dimension = (d[5] >> 9) & 3;
            std::string text = std::format("tf{}: ", n);
            if (type != 2)
                return text + std::format("type {} (not a texture) {:08X} {:08X} {:08X} {:08X} {:08X} {:08X}",
                                          type, d[0], d[1], d[2], d[3], d[4], d[5]);
            static const char* const kDimensions[] = { "1D", "2D", "3D", "Cube" };
            text += std::format("{} {}", TextureFormatName(format), kDimensions[dimension]);
            switch (dimension)
            {
            case 0: text += std::format(" {}", (d[2] & 0xFFFFFF) + 1); break;
            case 1:
                text += std::format(" {}x{}", (d[2] & 0x1FFF) + 1, ((d[2] >> 13) & 0x1FFF) + 1);
                if ((d[1] >> 10) & 1)
                    text += std::format(" stacked {}", (d[2] >> 26) + 1);
                break;
            case 2:
                text += std::format(" {}x{}x{}", (d[2] & 0x7FF) + 1, ((d[2] >> 11) & 0x7FF) + 1, (d[2] >> 22) + 1);
                break;
            case 3: text += std::format(" {}x{}", (d[2] & 0x1FFF) + 1, ((d[2] >> 13) & 0x1FFF) + 1); break;
            }
            text += std::format(" {} pitch={} endian={} base={:08X} mip={:08X}{}", (d[0] >> 31) ? "tiled" : "linear",
                                ((d[0] >> 22) & 0x1FF) * 32, kEndian[(d[1] >> 6) & 3], d[1] & 0xFFFFF000,
                                d[5] & 0xFFFFF000, ((d[5] >> 11) & 1) ? " packed" : "");
            text += std::format(" levels={}-{}", (d[4] >> 2) & 0xF, (d[4] >> 6) & 0xF);
            text += std::format(" signs={}{}{}{}", (d[0] >> 2) & 3, (d[0] >> 4) & 3, (d[0] >> 6) & 3, (d[0] >> 8) & 3);
            text += std::format(" {} swizzle={}", (d[3] & 1) ? "integer" : "fraction", Swizzle((d[3] >> 1) & 0xFFF));
            if (const int32_t exp = SignExtend((d[3] >> 13) & 0x3F, 6))
                text += std::format(" exp_adjust={}", exp);
            text += std::format(" filter(mag {} min {} mip {} aniso {})", (d[3] >> 19) & 3, (d[3] >> 21) & 3,
                                (d[3] >> 23) & 3, (d[3] >> 25) & 7);
            text += std::format(" clamp({} {} {})", (d[0] >> 10) & 7, (d[0] >> 13) & 7, (d[0] >> 16) & 7);
            if (const int32_t bias = SignExtend((d[4] >> 12) & 0x3FF, 10))
                text += std::format(" lod_bias={}/32", bias);
            text += std::format(" border={}{}", d[5] & 3, ((d[5] >> 2) & 1) ? " force_w" : "");
            if ((d[1] >> 11) & 1)
                text += " clamp_policy_ogl";
            if (d[3] >> 31)
                text += " border_texels";
            return text;
        }

        // Vertex fetch constant `n` (hardware index 0-95; 2 dwords at 0x4800 + 2n), 04 §2.
        std::string DescribeVertexBuffer(uint32_t n)
        {
            const uint32_t d0 = Reg(0x4800 + n * 2), d1 = Reg(0x4800 + n * 2 + 1);
            if ((d0 & 3) != 3)
                return std::format("vf{}: type {} (not a vertex buffer) {:08X} {:08X}", n, d0 & 3, d0, d1);
            return std::format("vf{}: base={:08X} size={} dwords endian={}{}", n, d0 & 0xFFFFFFFC, (d1 >> 2) & 0xFFFFFF,
                               kEndian[d1 & 3], (d1 >> 26) ? std::format(" d1[31:26]={:X}", d1 >> 26) : "");
        }

        std::string DescribeShader(const char* label, const Shader* shader)
        {
            if (!shader)
                return std::format("{}=none", label);
            const auto& a = shader->analysis;
            std::string text = std::format("{}={:016X} ({} dw", label, shader->hash, shader->words.size());
            if (a.textureFetchConstants.size())
            {
                text += " tf";
                for (uint32_t tf : a.textureFetchConstants)
                    text += std::format(" {}", tf);
            }
            if (a.vertexFetchConstants.size())
            {
                text += " vf";
                for (uint32_t vf : a.vertexFetchConstants)
                    text += std::format(" {}", vf);
            }
            if (a.kills) text += " kill";
            if (a.writesDepth) text += " oDepth";
            if (a.memoryExport) text += " memexport";
            if (a.usesLoops) text += " loops";
            if (a.usesCalls) text += " calls";
            if (a.usesPredication) text += " pred";
            if (shader->stage == ucode::Stage::kVertex)
                text += std::format(" o={:X}{}", a.interpolatorsWritten, a.writesPointSize ? " oPts" : "");
            else
                text += std::format(" oC={:X}", a.colorsWritten);
            return text + ")";
        }

        float ReadPhysicalFloat(uint32_t physical, uint32_t endian)
        {
            uint32_t raw;
            std::memcpy(&raw, guest::g_memory.Translate(guest::kPhysicalWindow64k + (physical & 0x1FFFFFFF)), 4);
            switch (endian)
            {
            case 1: raw = ((raw & 0x00FF00FF) << 8) | ((raw >> 8) & 0x00FF00FF); break;
            case 2: raw = __builtin_bswap32(raw); break;
            case 3: raw = (raw << 16) | (raw >> 16); break;
            }
            return std::bit_cast<float>(raw);
        }
    }

    bool TraceActive(uint64_t presentedFrames)
    {
        const uint64_t frame = presentedFrames + 1;
        return g_range.first && frame >= g_range.first && frame <= g_range.last;
    }

    void TraceDraw(const DrawPacket& draw, const Shader* vs, const Shader* ps)
    {
        auto& out = Out();
        const uint32_t initiator = draw.initiator;
        const uint32_t prim = initiator & 0x3F, source = (initiator >> 6) & 3;
        static const char* const kSources[] = { "dma", "immediate", "auto", "#3" };
        std::string line = std::format("draw {} {} {} {} count={}", g_drawsInFrame++,
                                       draw.opcode == 0x22 ? "DRAW_INDX" : "DRAW_INDX_2", PrimitiveName(prim),
                                       kSources[source], initiator >> 16);
        if (source == 0)
            line += std::format(" idx{} endian={} base={:08X} words={}", ((initiator >> 11) & 1) ? 32 : 16,
                                kEndian[draw.dmaSize >> 30], draw.dmaBase, draw.dmaSize & 0xFFFFFF);
        if ((initiator >> 8) & 3)
            line += std::format(" major_mode={}", (initiator >> 8) & 3);
        if ((initiator >> 12) & 1)
            line += " not_eop";
        if (draw.vizToken & 0x100)
            line += std::format(" conditional_on_query={}", draw.vizToken & 0x3F);
        else if (draw.vizToken)
            line += std::format(" viz_token={:X}", draw.vizToken);
        if (draw.surveyOnly)
        {
            line += " SURVEY (not drawn)";
            g_surveyDraws++;
        }
        out << line << "\n";

        const uint32_t modeControl = Reg(0x2208), mode = modeControl & 7;
        const uint32_t surface = Reg(0x2000);
        out << std::format("  mode={} surface pitch={} msaa={}x hiz_pitch={}", EdramModeName(mode), surface & 0x3FFF,
                           1u << ((surface >> 16) & 3), surface >> 18);
        const uint32_t colorMask = Reg(0x2104);
        static const uint32_t kColorInfo[4] = { 0x2001, 0x2003, 0x2004, 0x2005 };
        for (uint32_t rt = 0; rt < 4; rt++)
        {
            const uint32_t mask = (colorMask >> (4 * rt)) & 0xF;
            if (!mask && !(mode == 6 && (Reg(0x2318) & 7) == rt))
                continue;
            const uint32_t info = Reg(kColorInfo[rt]);
            out << std::format(" | rt{} base={} {} mask={:X}", rt, info & 0xFFF, ColorFormatName((info >> 16) & 0xF), mask);
            if (const int32_t bias = SignExtend((info >> 20) & 0x3F, 6))
                out << std::format(" exp_bias={}", bias);
            if (mode != 6)
                out << std::format(" blend={:08X}", Reg(rt == 0 ? 0x2201 : 0x2208 + rt));
        }
        const uint32_t depthControl = Reg(0x2200), depthInfo = Reg(0x2002);
        if ((depthControl & 3) || (mode == 6 && (Reg(0x2318) & 7) == 4))
            out << std::format(" | depth base={} {} control={:08X}", depthInfo & 0xFFF,
                               ((depthInfo >> 16) & 1) ? "D24FS8" : "D24S8", depthControl);
        out << "\n";

        if (mode == 6)
        {
            // Resolve: the RECTLIST's three float2 vertices come from vertex fetch constant 0 (07 §4).
            const uint32_t d0 = Reg(0x4800), d1 = Reg(0x4801);
            out << std::format("  copy control={:08X} dest base={:08X} pitch={:08X} info={:08X} slice={:08X}"
                               " clear color={:08X}/{:08X} depth={:08X} func={:08X} ref={:08X} mask={:08X}\n",
                               Reg(0x2318), Reg(0x2319), Reg(0x231A), Reg(0x231B), Reg(0x2323), Reg(0x231E),
                               Reg(0x231F), Reg(0x231D), Reg(0x2320), Reg(0x2321), Reg(0x2322));
            out << "  copy rect vf0:";
            if ((d0 & 3) == 3)
                for (uint32_t i = 0; i < 6; i++)
                    out << std::format(" {}", ReadPhysicalFloat((d0 & 0xFFFFFFFC) + i * 4, d1 & 3));
            else
                out << std::format(" not a vertex buffer ({:08X} {:08X})", d0, d1);
            out << std::format(" window_offset={:08X} scissor={:08X}-{:08X}\n", Reg(0x2080), Reg(0x2081), Reg(0x2082));
            return;
        }

        out << std::format("  {} {} sq_program_cntl={:08X} context_misc={:08X} interp_cntl={:08X}\n",
                           DescribeShader("vs", vs), DescribeShader("ps", ps), Reg(0x2180), Reg(0x2181), Reg(0x2182));
        // Constant windows (01a §3) and the first pixel-shader float constants.
        const uint32_t vsConst = Reg(0x2307), psConst = Reg(0x2308);
        out << std::format("  vs_const base={} count={} ps_const base={} count={} ps c0-c3:", vsConst & 0x1FF,
                           ((vsConst >> 12) & 0x1FF) + 1, psConst & 0x1FF, ((psConst >> 12) & 0x1FF) + 1);
        for (uint32_t c = 0; c < 4; c++)
            out << std::format(" ({} {} {} {})", RegFloat(0x4000 + ((psConst & 0x1FF) + c) * 4),
                               RegFloat(0x4001 + ((psConst & 0x1FF) + c) * 4), RegFloat(0x4002 + ((psConst & 0x1FF) + c) * 4),
                               RegFloat(0x4003 + ((psConst & 0x1FF) + c) * 4));
        out << "\n";
        out << std::format("  colorcontrol={:08X} alpha_ref={} su_sc_mode={:08X} clip_cntl={:08X} vte={:08X}"
                           " stencilref={:08X} vtx_cntl={:08X} indx_offset={} reset_indx={:X} vtx_indx={:X}-{:X}\n",
                           Reg(0x2202), RegFloat(0x210E), Reg(0x2205), Reg(0x2204), Reg(0x2206), Reg(0x210D),
                           Reg(0x2302), Reg(0x2102), Reg(0x2103), Reg(0x2101), Reg(0x2100));
        out << std::format("  viewport scale=({} {} {}) offset=({} {} {}) window_offset={:08X} scissor={:08X}-{:08X}"
                           " screen_scissor={:08X}-{:08X}\n",
                           RegFloat(0x210F), RegFloat(0x2111), RegFloat(0x2113), RegFloat(0x2110), RegFloat(0x2112),
                           RegFloat(0x2114), Reg(0x2080), Reg(0x2081), Reg(0x2082), Reg(0x200E), Reg(0x200F));
        for (const Shader* shader : { vs, ps })
        {
            if (!shader)
                continue;
            g_shadersInFrame.insert(shader->hash);
            for (uint32_t vf : shader->analysis.vertexFetchConstants)
                out << "  " << DescribeVertexBuffer(vf) << "\n";
            for (uint32_t tf : shader->analysis.textureFetchConstants)
                out << "  " << DescribeTexture(tf) << "\n";
        }
    }

    void TraceNote(const char* text)
    {
        Out() << text << "\n";
    }

    void TraceSwap(uint64_t presentedFrames)
    {
        if (TraceActive(presentedFrames - 1))
        {
            Out() << std::format("=== end of frame {}: {} draws ({} survey-only), {} distinct shaders this frame,"
                                 " {} shaders seen in total\n\n",
                                 presentedFrames, g_drawsInFrame, g_surveyDraws, g_shadersInFrame.size(), ShaderCount());
            g_out.flush();
            if (presentedFrames == g_range.last)
                Log("GPU: trace of frames {}-{} written to gpu_trace.log", g_range.first, g_range.last);
        }
        g_drawsInFrame = 0;
        g_surveyDraws = 0;
        g_shadersInFrame.clear();
        g_frame = presentedFrames + 1;
        g_headerWritten = false;
    }
}
