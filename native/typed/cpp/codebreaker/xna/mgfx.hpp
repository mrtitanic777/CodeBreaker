// codebreaker/xna/mgfx.hpp -- C++ from codebreaker.xna.mgfx.
//
// mgfx.py -- MonoGame's compiled effect (.mgfxo, version 10: MonoGame 3.8.1), read far enough to add parameters.
//
//     mgfx::add_parameters(data, parameters)   the effect with every parameter in `parameters` (name,
//                                              semantic, type, cls, rows, columns, elements, members, value --
//                                              formats/xnb.py writes them) it lacks appended, with its default
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

#include "py.hpp"

namespace codebreaker::xna::mgfx {

extern const py::bytes SIGNATURE;
extern const py::dict<py::str, int64_t> CLASSES;
extern const py::dict<py::str, int64_t> TYPES;

// One parameter, as the python takes it: a dict with the keys below (a _P in the module, which makes its
// attributes of the dict and its members _Ps), or an object with those attributes. A key that is None in the
// python (semantic, rows, columns, elements, value) is nullopt; "members" None is the empty list.
// `value` holds numbers (ints are held as the floats the module makes of them).
struct Parameter {
    py::str name;
    std::optional<py::str> semantic;
    py::str type;
    py::str cls;
    std::optional<int64_t> rows;
    std::optional<int64_t> columns;
    std::optional<int64_t> elements;
    std::optional<py::list<double>> value;
    py::list<Parameter> members;
};

// _Reader: a cursor in the bytes. `at` may be moved by the caller, as the python's r.at is (and may be
// negative: the python then counts from the end of the data, as the access below does).
class Reader {
  public:
    py::bytes d;
    int64_t at;
    Reader(const py::bytes& data, int64_t at_) : d(data), at(at_) {}

    // take("i"), take("h"), take("B"): a little-endian signed int, short, unsigned byte
    int64_t take_i();
    int64_t take_h();
    int64_t take_B();
    // take("?")
    bool take_flag();
    // A length-prefixed (7-bit varint) UTF-8 string.
    py::str string();
    // A count of annotations: MonoGame's compiler writes none, so any is a ValueError.
    void annotations();
};

// A str as MGFX writes it: its UTF-8 length as a 7-bit varint, then the bytes.
py::bytes _string(const py::str& s);

// Steps over a list of parameters (a count, then each one), their elements and members; the names of the top
// level ones go in `names` when given.
void _skip_parameters(Reader& r, py::set<py::str>* names = nullptr);

// One parameter in MGFX's form (its value: the default, as floats -- MojoShader keeps every number a float).
py::bytes _parameter(const Parameter& p, bool element = false);

// `data` (an .mgfxo) with each of `parameters` it lacks (by name) appended to its parameter list.
py::bytes add_parameters(const py::bytes& data, const py::list<Parameter>& parameters);

}  // namespace codebreaker::xna::mgfx
