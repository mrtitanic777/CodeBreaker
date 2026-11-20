// formats/xbox/shader.hpp -- an Xbox 360 compiled shader: what the SDK's fxc writes for vs_3_0 / ps_3_0, and what an
// effect holds.
//
//     shader::Shader s = shader::read(container);   throws FormatError for what is not a shader container
//     s.stage            Vertex or Pixel
//     s.constants        the constant table: name, register set, first register, count, class, type, rows, columns,
//                        elements, default value (floats)
//     s.literals         {register: x y z w}: the compiler's own constants (`def c255, ...`)
//     s.loop_literals    {loop constant: count | start << 8 | step << 16} (`defi i31, ...`)
//     s.vertex_inputs    a vertex shader's inputs: the slot of the vertex fetch, the usage, its index
//     s.pixel_inputs     a pixel shader's inputs (the interpolators): register, usage, index, mask
//     s.outputs          a vertex shader's outputs: export register, usage, index, mask
//     s.words            the microcode, as dwords (decompile/xenos_hlsl reads it)
//
// The container is big-endian: 0x102A1100 (pixel) or 0x102A1101 (vertex), the offset and size of the physical part (the
// literal constants, then the microcode), the offset of its constant table -- the Direct3D 9 CTAB, as on the PC -- the
// offset of the literal records, and the offset of the program information, which begins with the literals' and the
// microcode's sizes. A vertex shader's information lists, after five words, the inputs (a vertex fetch's slot in bits
// 0-11, the usage in 12-15, its index in 16-19), the outputs (index 0-3, usage 4-7, export register 8-11, mask 12-15) and
// the instructions writing each output; a pixel shader's, after its interpolator control and output mask, its inputs in
// the outputs' form. The SDK's fxc listing (/Fc /Xudsrd) names the same tables, which is what they were read against.
//
// A container that is cut short or whose tables point outside it is a FormatError (TruncatedError for a read past the
// end); nothing is read from beyond the bytes given.
#pragma once

#include <array>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "core/bytes.hpp"
#include "formats/xbox/d3dx.hpp"

namespace cb::formats::xbox::shader {

inline constexpr std::uint32_t kPixelMagic = 0x102A1100;
inline constexpr std::uint32_t kVertexMagic = 0x102A1101;

enum class Stage : std::uint8_t { Vertex, Pixel };

enum class RegisterSet : std::uint8_t { Bool, Int4, Float4, Sampler, Other };

// One constant of the table.
struct Constant {
    std::string name;
    RegisterSet set = RegisterSet::Float4;
    std::uint16_t first_register = 0;
    std::uint16_t count = 0;                                   // registers it covers
    d3dx::ParamClass cls = d3dx::ParamClass::Scalar;
    d3dx::ParamType type = d3dx::ParamType::Float;
    std::uint16_t rows = 0, columns = 0, elements = 0, members = 0;
    std::vector<float> defaults;                               // 4 per register; empty when the table gives none
};

struct VertexInput {
    std::uint32_t slot = 0;        // the vertex fetch's
    std::string usage;             // "POSITION", "TEXCOORD" ... ("USAGE9" for a number past the table)
    std::uint32_t index = 0;
};

// A pixel shader's input (an interpolator) or a vertex shader's output (an export).
struct Interpolator {
    std::uint32_t reg = 0;
    std::string usage;
    std::uint32_t index = 0;
    std::uint32_t mask = 0;        // the components written, bit 0 = x
};

struct Shader {
    Stage stage = Stage::Pixel;
    std::string target;                                        // "ps_3_0"
    std::vector<Constant> constants;
    std::vector<VertexInput> vertex_inputs;                    // a vertex shader's
    std::vector<Interpolator> pixel_inputs;                    // a pixel shader's
    std::vector<Interpolator> outputs;                         // a vertex shader's
    std::vector<std::uint32_t> output_writers;                 // a vertex shader's: the instruction writing each output
    std::uint32_t interpolator_control = 0, output_mask = 0;   // a pixel shader's
    // Looked up by register, never listed in an output, so the sorted map is only for stable tests.
    std::map<std::uint32_t, std::array<float, 4>> literals;
    std::map<std::uint32_t, std::uint32_t> loop_literals;
    std::vector<std::uint32_t> words;

    // The constant of `set` whose registers include `reg`, and the offset of `reg` into it; null when none does.
    struct Found {
        const Constant* constant = nullptr;
        std::uint32_t offset = 0;
    };
    Found constant_at(RegisterSet set, std::uint32_t reg) const;
};

// An Xbox shader container's bytes -> Shader.
Shader read(ByteView container);

// True when the bytes begin with a shader container's magic (what effect objects are tried with).
bool is_container(ByteView data);

// "bool", "int4", "float4", "sampler" -- the register set's spelling in the SDK's listings ("other" for a set it does not name).
std::string_view register_set_name(RegisterSet set);

}  // namespace cb::formats::xbox::shader
