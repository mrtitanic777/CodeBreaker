#include "formats/xbox/d3dx.hpp"

#include <array>

namespace cb::formats::xbox::d3dx {

namespace {

constexpr std::array<std::string_view, 7> kClassNames = {"scalar", "vector", "matrix_rows", "matrix_columns", "object", "struct", "unknown"};
constexpr std::array<std::string_view, 18> kTypeNames = {
    "void", "bool", "int", "float", "string", "texture", "texture1D", "texture2D", "texture3D", "textureCUBE",
    "sampler", "sampler1D", "sampler2D", "sampler3D", "samplerCUBE", "pixelshader", "vertexshader", "unknown"};

}  // namespace

ParamClass class_of(std::uint32_t number) { return number < 6 ? static_cast<ParamClass>(number) : ParamClass::Unknown; }

ParamType type_of(std::uint32_t number) { return number < 17 ? static_cast<ParamType>(number) : ParamType::Unknown; }

std::string_view class_name(ParamClass cls) { return kClassNames[static_cast<std::size_t>(cls)]; }

std::string_view type_name(ParamType type) { return kTypeNames[static_cast<std::size_t>(type)]; }

bool is_object_type(ParamType t) {
    switch (t) {
        case ParamType::String: case ParamType::Texture: case ParamType::Texture1D: case ParamType::Texture2D:
        case ParamType::Texture3D: case ParamType::TextureCube: case ParamType::PixelShader: case ParamType::VertexShader:
            return true;
        default:
            return false;
    }
}

bool is_sampler_type(ParamType t) {
    switch (t) {
        case ParamType::Sampler: case ParamType::Sampler1D: case ParamType::Sampler2D: case ParamType::Sampler3D:
        case ParamType::SamplerCube:
            return true;
        default:
            return false;
    }
}

bool is_texture_type(ParamType t) {
    switch (t) {
        case ParamType::Texture: case ParamType::Texture1D: case ParamType::Texture2D: case ParamType::Texture3D:
        case ParamType::TextureCube:
            return true;
        default:
            return false;
    }
}

bool is_numeric_class(ParamClass cls) {
    return cls == ParamClass::Scalar || cls == ParamClass::Vector || cls == ParamClass::MatrixRows ||
           cls == ParamClass::MatrixColumns;
}

}  // namespace cb::formats::xbox::d3dx
