// codebreaker/decompile/xenos_hlsl.hpp -- COMPAT SHIM (docs/CPP_DESIGN.md 6.3). The module now lives in
// native/cb/decompile/xenos_hlsl.{hpp,cpp}; this header keeps the old py:: surface for what still calls it:
// decompile/xbox_effect (translate, Function, HELPERS, _num, Unsupported). Conversion and forwarding only; deleted with its
// last py:: consumer (the new Shader type is what formats/xbox/shader will hold once it is refactored).
#pragma once

#include <variant>

#include "cb/compat/py_bridge.hpp"
#include "cb/decompile/xenos_hlsl.hpp"
#include "codebreaker/formats/xbox/shader.hpp"
#include "py.hpp"

namespace codebreaker::decompile::xenos_hlsl {

namespace nx = ::cb::decompile::xenos_hlsl;
namespace shader = codebreaker::formats::xbox::shader;

// raised for what the translator does not do; the message names the instruction
struct Unsupported : py::Exception {
    explicit Unsupported(const py::str& m) : py::Exception(m.utf8()) {}
    const char* type() const override { return "Unsupported"; }
};

// The translated shader: .text, .samplers, the input and output fields with their semantics, whether the cube helpers are needed.
struct Function {
    py::str text;
    py::list<py::str> samplers;
    py::dict<py::str, py::str> inputs;
    py::dict<py::str, py::str> outputs;
    bool needs_helpers = false;
};

inline const py::str HELPERS = py::str(std::string(nx::kHelpers));

// A float as HLSL writes it, shortest round-trip form.
inline py::str _num(double f) { return ::cb::compat::to_py(nx::number_text(f)); }

namespace detail {

inline nx::Shader to_new(const shader::Shader& s) {
    nx::Shader n;
    n.stage = s.stage == py::str("vertex") ? nx::Stage::Vertex : nx::Stage::Pixel;
    for (const shader::Constant& c : s.constants) {
        nx::Constant k;
        k.name = c.name.utf8();
        const std::string set = c.set.utf8();
        k.set = set == "bool" ? nx::RegisterSet::Bool : set == "int4" ? nx::RegisterSet::Int4 : set == "float4" ? nx::RegisterSet::Float4 : nx::RegisterSet::Sampler;
        k.first_register = static_cast<std::uint32_t>(c.register_);
        k.count = static_cast<std::uint32_t>(c.count);
        const std::string cls = c.cls.utf8();
        k.cls = cls == "scalar" ? nx::ConstantClass::Scalar : cls == "vector" ? nx::ConstantClass::Vector
              : cls == "matrix_rows" ? nx::ConstantClass::MatrixRows : cls == "matrix_columns" ? nx::ConstantClass::MatrixColumns : nx::ConstantClass::Other;
        k.is_bool = c.type == py::str("bool");
        k.rows = static_cast<std::uint32_t>(c.rows);
        k.columns = static_cast<std::uint32_t>(c.columns);
        k.elements = static_cast<std::uint32_t>(c.elements);
        if (c.default_) {
            for (double d : *c.default_) k.defaults.push_back(static_cast<float>(d));
        }
        n.constants.push_back(std::move(k));
    }
    if (const auto* vi = std::get_if<py::list<shader::VertexInput>>(&s.inputs)) {
        for (const auto& e : *vi) n.vertex_inputs.push_back({static_cast<std::uint32_t>(std::get<0>(e)), std::get<1>(e).utf8(), static_cast<std::uint32_t>(std::get<2>(e))});
    } else if (const auto* pi = std::get_if<py::list<shader::Interpolator>>(&s.inputs)) {
        for (const auto& e : *pi) {
            n.pixel_inputs.push_back({static_cast<std::uint32_t>(std::get<0>(e)), std::get<1>(e).utf8(), static_cast<std::uint32_t>(std::get<2>(e)), static_cast<std::uint32_t>(std::get<3>(e))});
        }
    }
    for (const auto& e : s.outputs) {
        n.outputs.push_back({static_cast<std::uint32_t>(std::get<0>(e)), std::get<1>(e).utf8(), static_cast<std::uint32_t>(std::get<2>(e)), static_cast<std::uint32_t>(std::get<3>(e))});
    }
    for (const auto& kv : s.literals.items()) {
        const auto& v = std::get<1>(kv);
        n.literals[static_cast<std::uint32_t>(std::get<0>(kv))] = {static_cast<float>(std::get<0>(v)), static_cast<float>(std::get<1>(v)),
                                                                   static_cast<float>(std::get<2>(v)), static_cast<float>(std::get<3>(v))};
    }
    for (const auto& kv : s.loop_literals.items()) n.loop_literals[static_cast<std::uint32_t>(std::get<0>(kv))] = static_cast<std::uint32_t>(std::get<1>(kv));
    for (int64_t w : s.words) n.words.push_back(static_cast<std::uint32_t>(w));
    return n;
}

}  // namespace detail

// formats/xbox/shader.Shader -> Function (raises Unsupported, naming the instruction, for what is not done).
inline Function translate(const shader::Shader& s, const py::str& name) {
    try {
        const nx::Function f = nx::translate(detail::to_new(s), name.utf8());
        Function out;
        out.text = ::cb::compat::to_py(f.text);
        for (const std::string& n : f.samplers) out.samplers.append(::cb::compat::to_py(n));
        for (const auto& kv : f.inputs) out.inputs.set(::cb::compat::to_py(kv.first), ::cb::compat::to_py(kv.second));
        for (const auto& kv : f.outputs) out.outputs.set(::cb::compat::to_py(kv.first), ::cb::compat::to_py(kv.second));
        out.needs_helpers = f.needs_helpers;
        return out;
    } catch (const nx::Unsupported& e) {
        throw Unsupported(py::percent("%s shader %s, slot %d: %s", ::cb::compat::to_py(nx::stage_name(e.stage())), ::cb::compat::to_py(e.shader()),
                                      static_cast<int64_t>(e.slot()), ::cb::compat::to_py(e.what_failed())));
    } catch (const ::cb::Error& e) {
        ::cb::compat::rethrow_as_py(e);
    }
}

}  // namespace codebreaker::decompile::xenos_hlsl
