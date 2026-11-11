// cb/xna/mgfx.hpp -- MonoGame's compiled effect (.mgfxo, version 10: MonoGame 3.8.1), read far enough to add
// parameters to it.
//
// The effect compiler keeps only the parameters a shader reads. An XNA game looks its effect's parameters up by name
// and sets them whether a shader reads them or not (a stock effect's ShaderIndex, a value only one technique uses), so
// the port gives a compiled effect back every parameter the original declared; one no shader reads holds what the
// game sets and does nothing. They go at the end of the list, since constant buffers and samplers name parameters by
// their index.
//
// The layout is MonoGame's Effect.ReadEffect and Shader's constructor: "MGFX", version, profile, key; the constant
// buffers; the shaders; the parameters (class, type, name, semantic, annotations, rows, columns, elements, members,
// then the values of a numeric one); the techniques; "MGFX" again.
#pragma once

#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::xna::mgfx {

// MGFX's parameter classes. `MatrixRows` and `MatrixColumns` are one class on the wire; they differ in how the
// default's numbers are laid out (the Xbox's effect keeps a column at a time, MonoGame a row).
enum class ParamClass : std::uint8_t { Scalar, Vector, MatrixRows, MatrixColumns, Object, Struct };

// A parameter's type. The wire has the first ten; the last three exist in the description the game's effect was
// recovered into: a sampler is written as the texture it reads, the shader and string kinds are never added.
enum class ParamType : std::uint8_t {
    Void,
    Bool,
    Int,
    Float,
    String,
    Texture,
    Texture1D,
    Texture2D,
    Texture3D,
    TextureCube,
    Sampler,
    VertexShader,
    PixelShader,
};

// One parameter as the recovered effect (a .fx.json written by formats/xnb) declares it.
struct Parameter {
    std::string name;
    std::optional<std::string> semantic;
    ParamType type = ParamType::Void;
    ParamClass cls = ParamClass::Object;
    std::uint32_t rows = 0;           // 0..255 on the wire; a vector is written as one row of `columns`
    std::uint32_t columns = 0;        // 0..255
    std::uint32_t elements = 0;       // non-zero: an array, whose elements are `members`
    std::vector<double> value;        // the default, row-major; shorter than rows*columns is padded with zeros
    std::vector<Parameter> members;   // an array's elements, or a struct's fields
};

// The class / type a description spells as text ("vector", "matrix_columns", "float", "texture2D", "sampler2D" ...).
// A class not named is an object, a type not named is void.
ParamClass parse_class(std::string_view text);
ParamType parse_type(std::string_view text);

// The parameters of a .fx.json: an array of objects with the keys name, semantic, type, cls, rows, columns,
// elements, value, members (a key left out, or null, is empty). Throws cb::ParseError / cb::FormatError.
std::vector<Parameter> parse_parameters(std::string_view json_text);

// The names of an effect's own parameters, in order. Throws FormatError (TruncatedError when the data ends inside
// the structures it declares; UnsupportedError for annotations, which MonoGame's compiler never writes).
std::vector<std::string> parameter_names(ByteView effect);

// `effect` with each of `parameters` it lacks (by name) appended to its parameter list, each with its default. A
// parameter without a name, a sampler, a shader and a string are not added; an effect that already has every
// parameter comes back unchanged (so applying it twice changes nothing). Throws FormatError when `effect` is not an
// MGFX effect or ends early, UsageError when a parameter's rows or columns do not fit a byte.
Bytes add_parameters(ByteView effect, std::span<const Parameter> parameters);

}  // namespace cb::xna::mgfx
