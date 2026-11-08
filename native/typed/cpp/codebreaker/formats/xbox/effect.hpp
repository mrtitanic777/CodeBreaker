// codebreaker/formats/xbox/effect.hpp -- typed C++ from codebreaker.formats.xbox.effect.
//
// effect.py -- an Xbox 360 Direct3D effect (an XNA Effect's content, or the SDK's fxc /Tfx_2_0 output), as data.
//
//     effect::Effect fx = effect::read(data);   Effect: .parameters, .techniques, .shaders {object id: container bytes}
//     fx.parameter("World")                     a Parameter: name, semantic, type, cls, rows, columns, elements,
//                                               members, value (numbers, a string, a texture's object id, or a
//                                               sampler's states), annotations
//     pass_.states                              [State]: name ("AlphaBlendEnable", "VertexShader", ...), index, value
//
// XNA writes an effect as 0xBCF00BCF, the offset of the Direct3D effect in it, and the effect -- the
// D3DX9 effect layout (fx_2_0: 0xFEFF0901, then the offset of the parameter and technique lists in the
// data that follows), which Wine's d3dx9 effect.c reads too. The Xbox 360's XNA content pipeline writes
// it big-endian; the SDK's fxc writes it little-endian. Its shaders are Xbox shader containers
// (formats/xbox/shader.py), and the numbering of its states is the Xbox's own (STATES below: the SDK's
// fxc compiled one state at a time and read back -- the PC's table numbers them differently).
//
// C++ spelling notes:
//  * a Parameter's `value` is Python's "None, a list of numbers, an object id, a sampler's list of States, or
//    (for an array) a list of the elements' values": that mix is the std::variant in Value.
//  * a Parameter's `.reference` / `.expression` exist only after read() set them: std::optional.
//  * State.parameter is the same Parameter object the effect keeps: a shared_ptr.
//  * Effect.objects keeps a shader container under its object id; an id that is not an int (a malformed
//    effect whose resource names a state without an object value) is not representable: read() then throws
//    py::TypeError where Python would have stored the blob under a None key.
#pragma once

#include <memory>
#include <optional>
#include <tuple>
#include <variant>

#include "py.hpp"

namespace codebreaker::formats::xbox::effect {

inline constexpr int64_t XNA_MAGIC = 0xBCF00BCF;
inline constexpr int64_t D3DX_MAGIC = 0xFEFF0901;

// D3DXPARAMETER_TYPE and D3DXPARAMETER_CLASS
extern const py::list<py::str> TYPES;
extern const py::list<py::str> CLASSES;
extern const py::set<py::str> OBJECT_TYPES;
extern const py::set<py::str> SAMPLER_TYPES;

// The Xbox 360's effect state numbering: operation -> the state's name in an .fx file. Found by compiling an
// effect setting each state with the SDK's fxc (53-55 and 65-87 are the Xbox's own states, none of which an
// effect for the PC has).
extern const py::dict<int64_t, py::str> STATES;

// The Xbox 360's values of the enumerated states (the SDK's d3d9types.h), by their .fx names.
extern const py::dict<py::str, py::dict<int64_t, py::str>> VALUES;

struct State;
struct Value;
struct Parameter;

// A Parameter's value: None, a list of ints, a list of floats, an object id (an int), a sampler's list of
// States, or an array's list of its elements' values.
using ValueData = std::variant<std::monostate, py::list<int64_t>, py::list<double>, int64_t, py::list<State>,
                               py::list<Value>>;
struct Value {
    ValueData data;
    Value() = default;
    Value(ValueData d) : data(std::move(d)) {}
};

// The Python repr() of a value (what a State's repr shows).
py::str repr_value(const Value& v);

struct Parameter {
    py::str name = py::str("");
    py::str semantic = py::str("");
    py::str type = py::str("");
    py::str cls = py::str("");
    int64_t rows = 0;
    int64_t columns = 0;
    int64_t elements = 0;
    py::list<Parameter> members;           // a struct's members, or an array's elements
    Value value;
    int64_t flags = 0;
    py::list<Parameter> annotations;
    std::optional<py::str> reference;      // set by read(): the name of a parameter holding the object
    std::optional<py::bytes> expression;   // set by read(): a preshader, kept, not evaluated

    // Bytes of value data (an object or sampler has none here).
    int64_t size() const;
    py::str __repr__() const;
};

struct State {
    int64_t operation = 0;
    int64_t index = 0;
    std::shared_ptr<Parameter> parameter;
    py::str name = py::str("");

    State() = default;
    State(int64_t operation, int64_t index, std::shared_ptr<Parameter> parameter);

    Value value() const;
    // The value as an .fx file writes it: an enumerated state by its name, a flag as true/false, else the
    // number (None for an object: a shader, a texture).
    std::optional<py::str> text() const;
    py::str __repr__() const;
};

struct Pass {
    py::str name = py::str("");
    py::list<Parameter> annotations;
    py::list<State> states;

    Pass() = default;
    Pass(py::str name, py::list<Parameter> annotations, py::list<State> states);
    std::optional<State> state(const py::str& name) const;
};

struct Technique {
    py::str name = py::str("");
    py::list<Parameter> annotations;
    py::list<Pass> passes;

    Technique() = default;
    Technique(py::str name, py::list<Parameter> annotations, py::list<Pass> passes);
};

struct Effect {
    py::list<Parameter> parameters;
    py::list<Technique> techniques;
    py::dict<int64_t, py::bytes> objects;   // object id -> bytes (strings, shaders)
    bool big_endian = true;

    std::optional<Parameter> parameter(const py::str& name) const;
    py::str string(int64_t object_id) const;
};

// An effect's bytes (XNA's, with or without its header, or fxc's) -> Effect.
Effect read(const py::bytes& data);

// -- the readers read() is made of (the Python's private _names)

struct Reader {
    py::bytes data;
    int64_t base = 0;
    bool big = true;

    Reader(py::bytes data, int64_t base, bool big);
    int64_t u32(int64_t at) const;
    // A length-prefixed string at `at` (relative to the data base).
    py::str name(int64_t at) const;
};

// A type definition at `at`; -> (Parameter, offset after it).
std::tuple<Parameter, int64_t> _typedef(const Reader& r, int64_t at, const Parameter* parent = nullptr);
Parameter _copy(const Parameter& p);
// Fill p's value from the value data at `at` (numbers) and the stream at ptr (objects, samplers);
// -> (ptr after, bytes of value data used).
std::tuple<int64_t, int64_t> _value(const Reader& r, Parameter& p, int64_t at, int64_t ptr, bool numbers);
std::tuple<State, int64_t> _state(const Reader& r, int64_t ptr);
std::tuple<py::list<Parameter>, int64_t> _annotations(const Reader& r, int64_t ptr, int64_t count);

}  // namespace codebreaker::formats::xbox::effect
