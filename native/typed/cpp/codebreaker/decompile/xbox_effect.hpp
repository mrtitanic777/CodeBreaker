// codebreaker/decompile/xbox_effect.hpp -- typed C++ port of codebreaker.decompile.xbox_effect.
//
// xbox_effect.py -- an Xbox 360 effect (an XNA Effect's content) as an .fx file: its HLSL source.
//
//     py::str text = xbox_effect::to_fx(data, "Bloom");   the effect as .fx text (raises xenos_hlsl::Unsupported,
//                                                         naming the shader and instruction, for what is not
//                                                         translated yet)
//
// The parameters are declared as the effect declares them (type, array size, semantic, default value), the
// samplers with their states and textures, each shader as HLSL (decompile/xenos_hlsl.py: one function per shader,
// named for the technique and pass that use it first), and the techniques with their passes' render states. It
// compiles for the PC as fx with vs_3_0 / ps_3_0 shaders: MonoGame's mgfxc builds it (xna/port.py).
//
// A pass that picks its shader from an array by an expression -- XNA's stock effects (BasicEffect and the rest, which
// games copy) write `VertexShader = (VSArray[VSIndices[ShaderIndex]])` -- becomes one technique per value of the
// selecting parameter: the technique itself for 0, <technique>__<n> for n; the PC runtime makes the one the game's
// ShaderIndex names current (XnaPort.Effects.SetShaderIndex), as MonoGame's own stock effects do.
//
// Notes on the port:
//  * The effect (formats/xbox/effect) and shader (formats/xbox/shader) readers and the HLSL translator
//    (decompile/xenos_hlsl) are the typed ports of those modules.
//  * A parameter's `value` is the effect port's std::variant (Value): the Python tests it with isinstance /
//    truthiness / == and indexes it; those are the helpers in the .cpp. A value of a kind the effect reader never
//    gives a numeric parameter (the Python would raise a TypeError or IndexError there too, on most of them) is a
//    py::TypeError.
#pragma once

#include <optional>
#include <tuple>

#include "codebreaker/decompile/xenos_hlsl.hpp"
#include "codebreaker/formats/xbox/effect.hpp"
#include "codebreaker/formats/xbox/shader.hpp"
#include "py.hpp"

namespace codebreaker::decompile::xbox_effect {

namespace fxformat = codebreaker::formats::xbox::effect;

// the render states MonoGame's effect compiler takes; the rest are written as comments
extern const py::set<py::str> PASS_STATES;
extern const py::set<py::str> SAMPLER_STATES;

// "float" / "float4" / "float4x4" / "int" ... for a numeric parameter, None for anything else.
std::optional<py::str> _type_name(const fxformat::Parameter& p);

// Values of a numeric parameter as an HLSL initializer.
py::str _literal(const fxformat::Parameter& p, const fxformat::Value& values);

// " <string name = "..."; float4 other = float4(...);>" for the annotations that can be written, else "".
py::str _string_annotations(const fxformat::Effect& fx, const fxformat::Parameter& p);

// The texture parameter a sampler's Texture state names.
std::optional<py::str> _texture_of(const fxformat::Effect& fx, const fxformat::State& state);

// A shader state choosing from a shader array by an expression: -> ([object id per selector value], selector
// name), or None. The expression's preshader names the array (first), the selecting int and the int array
// indexing the shader array, if there is one; that is the stock effects' form, and the only one taken.
std::optional<std::tuple<py::list<int64_t>, py::str>> _array_selection(const fxformat::Effect& fx,
                                                                         const fxformat::State& state);

// The effect as .fx text.
py::str to_fx(const py::bytes& data, const py::str& name = py::str("Effect"));

}  // namespace codebreaker::decompile::xbox_effect
