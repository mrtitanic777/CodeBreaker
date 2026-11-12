// cb/formats/xbox/d3dx.hpp -- the Direct3D 9 effect framework's names for what a parameter or constant is: its class
// (D3DXPARAMETER_CLASS) and its type (D3DXPARAMETER_TYPE). Shared by the shader container's constant table and the effect
// file's parameter list, which both write them as the D3DX numbers.
#pragma once

#include <cstdint>
#include <optional>
#include <string_view>

namespace cb::formats::xbox::d3dx {

enum class ParamClass : std::uint8_t { Scalar, Vector, MatrixRows, MatrixColumns, Object, Struct, Unknown };

enum class ParamType : std::uint8_t {
    Void, Bool, Int, Float, String, Texture, Texture1D, Texture2D, Texture3D, TextureCube,
    Sampler, Sampler1D, Sampler2D, Sampler3D, SamplerCube, PixelShader, VertexShader,
    Unknown   // a number the effect framework does not define
};

// The class a D3DX number names; Unknown for one past the table.
ParamClass class_of(std::uint32_t number);
// The type a D3DX number names; Unknown for one past the table.
ParamType type_of(std::uint32_t number);

// "scalar", "vector", "matrix_rows", ... ; "unknown".
std::string_view class_name(ParamClass cls);
// How an .fx file spells the type: "float", "texture2D", "samplerCUBE", "vertexshader" ...; "unknown".
std::string_view type_name(ParamType type);

// string, the textures and the shaders: a parameter that is one object of the effect.
bool is_object_type(ParamType type);
// sampler and its dimensioned forms.
bool is_sampler_type(ParamType type);
// texture and its dimensioned forms.
bool is_texture_type(ParamType type);
// bool, int and float: what a numeric parameter or constant is made of.
bool is_numeric_class(ParamClass cls);

}  // namespace cb::formats::xbox::d3dx
