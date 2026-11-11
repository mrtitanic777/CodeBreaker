// cb/decompile/xenos_hlsl.hpp -- an Xbox 360 shader's microcode as HLSL (Shader Model 3), the language it was written in.
//
//     Function f = xenos_hlsl::translate(shader, "PS_Main");   f.text is the HLSL: the input and output structs and the
//                                                              function; f.samplers the sampler names it reads
//
// What comes out reads as the compiled program did: one statement per operation, on registers r0..rN, with the
// constant table's names for the constants (`World._m00_m10_m20_m30` for a column of a matrix, `Bones[a0 / 3]` for an
// array read through the address register), the compiler's literal constants written in place, and the inputs and
// outputs by their semantics. It compiles for the PC as vs_3_0 or ps_3_0 (MonoGame's mgfxc).
//
// The operations are the microcode's (decompile/xenos decodes it; native/x360's GPU translator, from which the operation
// semantics here are taken, runs it on the console's terms): a vector and a scalar operation per ALU instruction,
// reading every operand before writing; the predicate p0; kills; the address register a0; vertex fetches by the input
// table's semantic (the vertex declaration on the PC converts formats, as the console's runtime patched the fetch for
// it); texture fetches by the sampler bound to the fetch constant. Control flow: conditional execs (on a bool constant
// or the predicate), loops (a `for` over the loop constant's count, with the loop counter aL) and forward jumps (a
// `skip_to` index: the blocks a taken jump passes over do not run; out of a loop, a break); calls and backward jumps
// stop with Unsupported, naming the instruction.
//
// The shader is given as `Shader`, the part of an Xbox shader container (formats/xbox/shader) the translation reads.
#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "cb/core/error.hpp"
#include "cb/core/ordered_map.hpp"

namespace cb::decompile::xenos_hlsl {

enum class Stage : std::uint8_t { Vertex, Pixel };
std::string_view stage_name(Stage stage);   // "vertex" or "pixel"

// The register file a constant lives in.
enum class RegisterSet : std::uint8_t { Bool, Int4, Float4, Sampler };

enum class ConstantClass : std::uint8_t { Scalar, Vector, MatrixRows, MatrixColumns, Other };

// One constant of the shader's table (the Direct3D constant table the compiler left in the container).
struct Constant {
    std::string name;
    RegisterSet set = RegisterSet::Float4;
    std::uint32_t first_register = 0;
    std::uint32_t count = 0;               // registers it covers
    ConstantClass cls = ConstantClass::Scalar;
    bool is_bool = false;                  // its type is `bool`
    std::uint32_t rows = 0, columns = 0, elements = 0;
    std::vector<float> defaults;           // the default value's floats; empty when the table gives none
};

// A vertex shader's input: the slot of a vertex fetch, and the semantic it takes.
struct VertexInput {
    std::uint32_t slot = 0;
    std::string usage;                     // "POSITION", "TEXCOORD", "BLENDWEIGHT" ...
    std::uint32_t index = 0;
};

// A pixel shader's input (an interpolator register) and a vertex shader's output (an export register).
struct Interpolator {
    std::uint32_t reg = 0;
    std::string usage;
    std::uint32_t index = 0;
    std::uint32_t mask = 0;                // the components written, bit 0 = x
};

struct Shader {
    Stage stage = Stage::Pixel;
    std::vector<Constant> constants;
    std::vector<VertexInput> vertex_inputs;       // a vertex shader's
    std::vector<Interpolator> pixel_inputs;       // a pixel shader's
    std::vector<Interpolator> outputs;            // a vertex shader's exports
    // The compiler's own constants (`def c255, ...`) by register; its loop constants (`defi i31, ...`) by index as
    // count | start << 8 | step << 16. Looked up, never listed.
    std::unordered_map<std::uint32_t, std::array<float, 4>> literals;
    std::unordered_map<std::uint32_t, std::uint32_t> loop_literals;
    std::vector<std::uint32_t> words;             // the microcode

    // The constant of `set` whose registers include `reg`, and the offset of `reg` into it; null when none does.
    struct Found {
        const Constant* constant = nullptr;
        std::uint32_t offset = 0;
    };
    Found constant_at(RegisterSet set, std::int64_t reg) const;
};

// The translated shader.
struct Function {
    std::string text;                                  // the input and output structs and the function
    std::vector<std::string> samplers;                 // the sampler names it reads, in order of first use
    OrderedMap<std::string, std::string> inputs;       // field name -> semantic, in order of first use
    OrderedMap<std::string, std::string> outputs;
    bool needs_helpers = false;                        // it calls the cube helpers (kHelpers)
};

// What the translator does not do (a call, a backward jump, a fetch it has no HLSL for), naming the instruction: the
// shader, the microcode slot, what it is.
class Unsupported : public UnsupportedError {
public:
    Unsupported(Stage stage, std::string_view shader_name, std::uint32_t slot, std::string_view what)
        : UnsupportedError("xenos_hlsl", slot, std::string(stage_name(stage)) + " shader " + std::string(shader_name) + ": " + std::string(what)),
          stage_(stage), shader_(shader_name), slot_(slot), what_(what) {}
    Stage stage() const { return stage_; }
    const std::string& shader() const { return shader_; }
    std::uint32_t slot() const { return slot_; }
    const std::string& what_failed() const { return what_; }
    CB_ERROR_BOILERPLATE(Unsupported)

private:
    Stage stage_;
    std::string shader_;
    std::uint32_t slot_;
    std::string what_;
};

// Translate a shader. Unsupported for what is not done; FormatError for microcode that addresses itself past its end.
Function translate(const Shader& shader, std::string_view name);

// A float as HLSL writes it, the shortest digits that read back as the same float32 (`0.5`, `3.0`, `1e-05`).
std::string number_text(double f);

// HLSL for the Xbox 360 GPU's cube instruction and the face-to-direction conversion: Function::needs_helpers says a
// translated shader calls them; the effect writer puts this text before the shaders.
extern const std::string_view kHelpers;

}  // namespace cb::decompile::xenos_hlsl
