// codebreaker/decompile/xenos_hlsl.hpp -- typed C++ port of codebreaker.decompile.xenos_hlsl:
// an Xbox 360 shader's microcode as HLSL (Shader Model 3), the language it was written in.
//
// The shader is a formats::xbox::shader::Shader (its typed port); translate() turns it into a Function.
// C++ spelling: the Python class `_Translator` is `Translator_` (a leading underscore and a capital is reserved
// in C++), and Python's `Function` attribute `needs_helpers`, set after the fact, is a plain member.
//
// Where the Python mixes types: a lane token is (base, component) with the component a str, None, or -- for
// base "#" -- a number; `Lane` holds all three (comp is None / a str; value is the number of a "#" lane).
// The Python reads a shader's `inputs` as 3-tuples (vertex) or 4-tuples (pixel); the C++ Shader holds a variant,
// and a stage whose inputs have the other arity fails with ValueError as the tuple unpacking does.
#pragma once

#include <array>
#include <optional>
#include <tuple>
#include <utility>
#include <vector>

#include "codebreaker/formats/xbox/shader.hpp"
#include "py.hpp"

namespace codebreaker::decompile::xenos_hlsl {

namespace shader = codebreaker::formats::xbox::shader;

extern const py::str DOC;   // the module docstring
extern const py::str CH;

// raised for what the translator does not do; the message names the instruction
struct Unsupported : py::Exception {
    explicit Unsupported(const py::str& m) : py::Exception(m.utf8()) {}
    const char* type() const override { return "Unsupported"; }
};

// The translated shader: .text (the HLSL: input and output structs and the function), .samplers (the sampler
// names it reads), the input and output fields with their semantics, and whether the cube helpers are needed.
struct Function {
    py::str text;
    py::list<py::str> samplers;
    py::dict<py::str, py::str> inputs;
    py::dict<py::str, py::str> outputs;
    bool needs_helpers = false;
};

int64_t _bits(int64_t v, int64_t low, int64_t n);
int64_t _sext(int64_t v, int64_t n);

// A float as HLSL writes it, shortest round-trip form.
py::str _num(double f);
// a + b, with `a + -x` written `a - x`.
py::str _plus(const py::str& a, const py::str& b);
py::str _minus(const py::str& a, const py::str& b);
// `r1.xxxx` -> `r1.x`, `(0.5).xxxx` -> `0.5`: one value repeated; else None.
std::optional<py::str> _uniform(const py::str& v);

int64_t control_flow(const py::list<int64_t>& words, int64_t index);

extern const py::set<int64_t> EXEC_OPS;

// One lane token: (base, component) -- a component of a vector ('x'), of a matrix ('_m03'), or None for a
// whole scalar -- or ("#", number).
struct Lane {
    py::str base;
    std::optional<py::str> comp;
    double value = 0.0;   // the number of a "#" lane
};
inline Lane number_lane(double v) { return Lane{py::str("#"), std::nullopt, v}; }

// Lane tokens -> one HLSL expression.
py::str _lanes_text(const py::list<Lane>& lanes, std::optional<int64_t> n = std::nullopt);
py::str _lane_text(const Lane& t);

extern const py::set<int64_t> SCALAR_TWO;
extern const py::set<int64_t> VECTOR_SIDE_EFFECTS;
extern const py::str HELPERS;

using Word3 = std::array<int64_t, 3>;

class Translator_ {
  public:
    Translator_(const shader::Shader& shader, const py::str& name);

    const shader::Shader& s;
    py::str name;
    bool vertex;
    py::list<int64_t> words;
    py::list<py::str> lines;
    int64_t indent = 1;
    py::list<py::str> samplers;
    py::set<int64_t> used_regs;
    bool uses_ps = false, uses_a0 = false, uses_p0 = false;
    py::dict<py::str, py::str> inputs, outputs;   // field name -> semantic
    int64_t slot = 0;

    // set by run() / __init_body
    bool uses_ps_anywhere = false;
    bool jumps = false;
    py::dict<int64_t, int64_t> loop_end;
    py::list<int64_t> program;
    bool uses_aL = false, uses_lod = false;
    bool s_declared_ = false, t_declared_ = false;
    bool cube_needed = false, cube_fetch_needed = false;

    void line(const py::str& text);
    [[noreturn]] void fail(const py::str& what);

    // ---- operands
    py::str temp(int64_t reg, bool relative = false);
    // The four lanes of float constant c[index] (+ a0 when relative).
    py::list<Lane> constant_lanes(int64_t index, bool relative, bool to_a0);
    // Operand i (1-3) of an ALU instruction: its four lanes after the swizzle, with abs / negate.
    std::tuple<py::list<Lane>, bool, bool> alu_lanes(const Word3& w, int64_t i);
    static bool constant_relative(const Word3& w, int64_t i);
    static py::str modify(const py::str& text, bool absolute, bool negate);
    // Operand i's lanes `lanes_wanted` (a list of lane indices) as one expression.
    py::str vector_operand(const Word3& w, int64_t i, const py::list<int64_t>& lanes_wanted);
    // A scalar operand: lane `lane` (absolute, not through the swizzle) of operand 3's register.
    py::str scalar_operand(const Word3& w, int64_t lane);
    py::list<Lane> _raw_lanes(const Word3& w, int64_t i);

    // ---- ALU
    py::str export_target(int64_t index);
    void emit_alu(const py::list<int64_t>& w);
    static py::str widen(const py::str& value, int64_t n);
    static bool reads_register(const py::str& text, int64_t reg);
    // The vector operation's lanes `lanes` into `dest` (a register or an export).
    void emit_vector(const Word3& w, int64_t vop, const py::list<int64_t>& lanes, const std::optional<py::str>& dest,
                     bool saturate);
    // The scalar op's result as an expression, and statements its side effects need -> (text, [lines]).
    std::tuple<py::str, py::list<py::str>> scalar_expression(const Word3& w, int64_t sop);

    // ---- fetches
    // The fetch's destination swizzle: 0-3 a fetched lane, 4 zero, 5 one, 7 left as it is.
    void fetch_destination(int64_t w0, int64_t w1, const py::list<Lane>& value_lanes);
    void emit_fetch(const py::list<int64_t>& w);
    void emit_vertex_fetch(const Word3& w);
    shader::Constant sampler_for(int64_t tf);
    void emit_texture_fetch(const Word3& w);
    static py::str pad(const py::str& coord, const py::str& kind);

    // ---- control flow
    Function run();
    void emit_range(int64_t lo, int64_t hi, const std::vector<std::pair<int64_t, int64_t>>& loops);
    void emit_loop(int64_t start, int64_t end, const std::vector<std::pair<int64_t, int64_t>>& loops);
    // Loop constant k -> (count, aL's first value, its step) as expressions.
    std::tuple<py::str, py::str, py::str> loop_constant(int64_t k);
    py::str bool_name(int64_t b);
    void init_body_();   // Python's __init_body
    void emit_exec(int64_t cf);

    // ---- the function
    Function assemble();
};

// formats/xbox/shader.Shader -> Function (raises Unsupported, naming the instruction, for what is not done).
Function translate(const shader::Shader& shader, const py::str& name);

}  // namespace codebreaker::decompile::xenos_hlsl
