// cb/decompile/xbox_effect.hpp -- an Xbox 360 effect (an XNA Effect's content) as an .fx file: its HLSL source.
//
//     std::string text = xbox_effect::to_fx(data, "Bloom");   the effect as .fx text
//
// The parameters are declared as the effect declares them (type, array size, semantic, default value), the samplers with
// their states and textures, each shader as HLSL (decompile/xenos_hlsl: one function per shader, named for the technique
// and pass that use it first), and the techniques with their passes' render states. It compiles for the PC as fx with
// vs_3_0 / ps_3_0 shaders: MonoGame's mgfxc builds it (xna/port).
//
// A pass that picks its shader from an array by an expression -- XNA's stock effects (BasicEffect and the rest, which
// games copy) write `VertexShader = (VSArray[VSIndices[ShaderIndex]])` -- becomes one technique per value of the selecting
// parameter: the technique itself for 0, <technique>__<n> for n; the PC runtime makes the one the game's ShaderIndex names
// current (XnaPort.Effects.SetShaderIndex), as MonoGame's own stock effects do.
#pragma once

#include <string>
#include <string_view>

#include "cb/core/bytes.hpp"

namespace cb::decompile::xbox_effect {

// The effect in `data` (formats/xbox/effect) as .fx text. FormatError for a damaged effect or shader; xenos_hlsl::Unsupported,
// naming the shader and instruction, for what the translator does not do yet.
std::string to_fx(ByteView data, std::string_view name = "Effect");

}  // namespace cb::decompile::xbox_effect
