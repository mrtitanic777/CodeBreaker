// cb/formats/xbox/effect.hpp -- an Xbox 360 Direct3D effect (an XNA Effect's content, or the SDK's fxc /Tfx_2_0 output),
// as data.
//
//     effect::Effect fx = effect::read(data);   .parameters, .techniques, .objects {object id: bytes}
//     fx.parameter("World")                     a Parameter: name, semantic, type, class, rows, columns, elements,
//                                               members, value (numbers, or the id of an object), a sampler's states,
//                                               annotations
//     pass.states                               the State of a pass: name ("AlphaBlendEnable", "VertexShader", ...),
//                                               index, the parameter holding its value
//
// XNA writes an effect as 0xBCF00BCF, the offset of the Direct3D effect in it, and the effect -- the D3DX9 effect layout
// (fx_2_0: 0xFEFF0901, then the offset of the parameter and technique lists in the data that follows), which Wine's
// d3dx9 effect.c reads too. The Xbox 360's XNA content pipeline writes it big-endian; the SDK's fxc writes it
// little-endian. Its shaders are Xbox shader containers (formats/xbox/shader), and the numbering of its states is the
// Xbox's own (state_name below: the SDK's fxc compiled one state at a time and read back -- the PC's table numbers them
// differently).
//
// A value is what the effect holds for a parameter: its numbers (floats for a float parameter, signed integers for the
// rest), or the id of an object (a string, a texture, a shader: its bytes are in Effect::objects), or nothing. An array
// parameter holds its elements as `members`, each with its own value; a sampler holds its `states`.
//
// A damaged effect is a FormatError (TruncatedError for a read past the end); nothing is read from beyond the bytes given.
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/formats/xbox/d3dx.hpp"

namespace cb::formats::xbox::effect {

inline constexpr std::uint32_t kXnaMagic = 0xBCF00BCF;
inline constexpr std::uint32_t kD3dxMagic = 0xFEFF0901;

// The id an effect gives one of its objects.
struct ObjectId {
    std::uint32_t id = 0;
    friend bool operator==(ObjectId, ObjectId) = default;
};

// A parameter's own value: nothing, floats, integers, or an object.
using Value = std::variant<std::monostate, std::vector<std::int32_t>, std::vector<float>, ObjectId>;

struct State;

struct Parameter {
    std::string name, semantic;
    d3dx::ParamType type = d3dx::ParamType::Void;
    d3dx::ParamClass cls = d3dx::ParamClass::Scalar;
    std::uint32_t rows = 0, columns = 0, elements = 0;
    std::vector<Parameter> members;       // a struct's members, or an array's elements
    Value value;
    std::vector<State> states;            // a sampler's
    std::uint32_t flags = 0;
    std::vector<Parameter> annotations;
    std::optional<std::string> reference;   // the name of a parameter holding the object (a texture a sampler names)
    std::optional<Bytes> expression;        // a preshader: kept, not evaluated

    Parameter();
    Parameter(const Parameter&);
    Parameter(Parameter&&) noexcept;
    Parameter& operator=(const Parameter&);
    Parameter& operator=(Parameter&&) noexcept;
    ~Parameter();

    // Bytes of value data (an object or sampler has none).
    std::size_t size() const;
    // The numbers of a numeric parameter as floats / ints; empty for any other value.
    std::span<const float> floats() const;
    std::span<const std::int32_t> ints() const;
    // The object this parameter is (a string, a texture, a shader), if it is one.
    std::optional<ObjectId> object() const;
};

// The Xbox 360's name for an effect state number: "ZEnable", "VertexShader" ... ; "state_<n>" for one it does not know.
std::string state_name(std::uint32_t operation);

struct State {
    std::uint32_t operation = 0;
    std::uint32_t index = 0;
    Parameter parameter;        // what the state is set to
    std::string name;           // state_name(operation)

    // The value as an .fx file writes it: an enumerated state by its name, a flag as true/false, else the number; nullopt
    // for an object (a shader, a texture) or anything that is not one number.
    std::optional<std::string> text() const;
};

struct Pass {
    std::string name;
    std::vector<Parameter> annotations;
    std::vector<State> states;

    const State* state(std::string_view name) const;
};

struct Technique {
    std::string name;
    std::vector<Parameter> annotations;
    std::vector<Pass> passes;
};

struct Effect {
    std::vector<Parameter> parameters;
    std::vector<Technique> techniques;
    std::unordered_map<std::uint32_t, Bytes> objects;   // object id -> bytes (strings, shaders); looked up by id only
    bool big_endian = true;

    const Parameter* parameter(std::string_view name) const;
    // The string an object holds (up to its NUL); "" when there is no such object.
    std::string string(ObjectId id) const;
    const Bytes* object(ObjectId id) const;
};

// An effect's bytes (XNA's, with or without its header, or fxc's) -> Effect.
Effect read(ByteView data);

}  // namespace cb::formats::xbox::effect
