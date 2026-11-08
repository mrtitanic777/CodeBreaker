// codebreaker/formats/xbox/shader.hpp -- typed C++ from codebreaker.formats.xbox.shader.
//
// shader.py -- an Xbox 360 compiled shader (what the SDK's fxc writes for vs_3_0 / ps_3_0, and what an effect holds).
//
//     shader::Shader s = shader::read(blob);
//     s.stage                 "vertex" or "pixel"
//     s.constants             [Constant]: name, set ("float4", "bool", "int4", "sampler"), register_, count,
//                             cls, type, rows, columns, elements, default_ (floats)
//     s.literals              {register: (x, y, z, w)}: the compiler's own constants (`def c255, ...`)
//     s.loop_literals         {loop constant: count | start << 8 | step << 16} (`defi i31, ...`)
//     s.inputs                vertex: [(slot of the vertex fetch, usage, usage index)];
//                             pixel: [(register, usage, usage index, mask)] -- the interpolators
//     s.outputs               vertex: [(export register, usage, usage index, mask)]
//     s.words                 the microcode, as dwords (decompile/xenos_hlsl.py reads it)
//
// The container is big-endian: 0x102A1100 (pixel) or 0x102A1101 (vertex), the offset and size of the
// physical part (the literal constants, then the microcode), the offset of its constant table -- the
// Direct3D 9 CTAB, as on the PC -- the offset of the literal records, and the offset of the program
// information, which begins with the literals' and the microcode's sizes. A vertex shader's information
// lists, after five words, the inputs (a vertex fetch's slot in bits 0-11, the usage in 12-15, its index
// in 16-19), the outputs (index 0-3, usage 4-7, export register 8-11, mask 12-15) and the instructions
// writing each output; a pixel shader's, after its interpolator control and output mask, its inputs in the
// outputs' form. The SDK's fxc listing (/Fc /Xudsrd) names the same tables, which is what they were read
// against.
//
// C++ spelling: `register` and `default` are C++ keywords, so the Python attributes of those names are
// `register_` and `default_`.
#pragma once

#include <optional>
#include <tuple>
#include <variant>

#include "py.hpp"

namespace codebreaker::formats::xbox::shader {

inline constexpr int64_t PIXEL = 0x102A1100;
inline constexpr int64_t VERTEX = 0x102A1101;
extern const py::dict<int64_t, py::str> USAGES;
extern const py::dict<int64_t, py::str> REGISTER_SETS;
extern const py::dict<int64_t, py::str> CLASSES;
extern const py::dict<int64_t, py::str> TYPES;

// One constant of the shader's table (Python builds it from keyword arguments).
struct Constant {
    py::str name;
    py::str set;
    int64_t register_ = 0;
    int64_t count = 0;
    py::str cls;
    py::str type;
    int64_t rows = 0;
    int64_t columns = 0;
    int64_t elements = 0;
    int64_t members = 0;
    std::optional<py::list<double>> default_;   // floats, or None

    py::str __repr__() const;
};

// (slot of the vertex fetch, usage, usage index) -- a vertex shader's input
using VertexInput = std::tuple<int64_t, py::str, int64_t>;
// (register, usage, usage index, mask) -- a pixel shader's input, and the output of either stage
using Interpolator = std::tuple<int64_t, py::str, int64_t, int64_t>;

class Shader {
  public:
    py::str stage = py::str("");
    py::list<Constant> constants;
    // The Python list holds 3-tuples for a vertex shader and 4-tuples for a pixel shader (std::variant says so).
    std::variant<py::list<VertexInput>, py::list<Interpolator>> inputs{py::list<VertexInput>{}};
    py::list<Interpolator> outputs;
    py::list<int64_t> output_writers;
    py::dict<int64_t, std::tuple<double, double, double, double>> literals;
    py::dict<int64_t, int64_t> loop_literals;   // loop constant -> its value, for the ones the compiler made
    py::list<int64_t> words;
    py::str target = py::str("");
    // set by read() for a pixel shader only (the attributes do not exist on a vertex shader)
    std::optional<int64_t> interpolator_control;
    std::optional<int64_t> output_mask;

    // The constant whose registers include `register_` -> (Constant, offset into it), or (None, 0).
    std::tuple<std::optional<Constant>, int64_t> constant_at(const py::str& register_set, int64_t register_) const;
};

int64_t _u32(const py::bytes& b, int64_t at);
py::str _cstring(const py::bytes& b, int64_t at);

// The Direct3D constant table at `base` (big-endian here) -> ([Constant], target).
std::tuple<py::list<Constant>, py::str> _ctab(const py::bytes& b, int64_t base);

std::tuple<py::str, int64_t> _semantic(int64_t entry);

// An Xbox shader container's bytes -> Shader.
Shader read(const py::bytes& blob);

}  // namespace codebreaker::formats::xbox::shader
