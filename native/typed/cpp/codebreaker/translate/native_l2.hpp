// codebreaker/translate/native_l2.hpp -- typed C++ port of codebreaker.translate.native_l2.
//
// native_l2.py -- the L2 core and the C back end in C++ (native/l2: cbl2.dll), through ctypes.
//
// A thin shim, kept only until each caller's own turn to move (docs/NATIVE.md step 5): the callers of
// codebreaker/translate/backends/c.py keep calling it, and the work is done by the library -- the same
// text, the same facts, the same exceptions (Decline, KeyError, ValueError where the Python raised
// them).
//
// Typing notes (how the Python's values are typed here):
//   * There is no DLL and no ctypes here: the library's sources (native/l2/src: api.cc and what it
//     calls) are linked into the program, and `call_raw` is l2_call itself. The Python's text crossing
//     stays: the arguments are written as the library's tagged JSON, the answer is read back from it,
//     so the library sees what the DLL saw. `enabled()` is true and `why_not()` is nullopt; `dll_path()`
//     says where the DLL would have been (the `l2_bin` setting).
//   * The Python's caches that exist only to make the crossing cheaper (_BIG and its $keep / $big numbers,
//     _HELD and $put / $doc, _TEXTS and known_text, _DESCRIBED and _TAKES) are not kept: nothing crosses a
//     process boundary, and every answer is the same without them. known_text() is therefore a no-op, and
//     `plain=` of call() (a faster way to write a document as JSON) has no effect on the text and is gone.
//     Each of signature / module_names32 / type_names / parameter_names asks the library for its own
//     entry (the Python asked `describe` once for the four; the answers and the exceptions are the same).
//   * `Wire` is native/l2's own value (native/l2/src/value.hpp: None, bool, int, float, str, list, tuple,
//     set, dict of any key, shared containers, Python's equality and order): what the L2 documents and the
//     C back end's facts and plans are made of -- the tuples, sets and dicts of other keys that the JSON
//     of an ir::Dict (ir::Val) has no word for. It is the type of `facts` (a dict the back end reads and
//     the call changes in place, as the Python's) and of the plans (segment_plan, caller_frame_plan ...)
//     whose records mix str, int, tuple and dict keys per entry; everything with a fixed shape is typed.
//     Convert with to_wire() / from_wire() (a tuple or a set becomes a list in a Val).
//   * An L2 document is an ir::Dict (the JSON dict of l2::to_json). A Type document (type_text's) is an ir::Val.
//   * An exception of the library is raised as the Python raised it: Decline (backends::c::Decline, or
//     backends::cpp17::Decline for "Cpp17Decline"), KeyError, ValueError, TypeError, IndexError,
//     AttributeError; any other becomes NativeError("<type>: <message>").
//   * `far` (a set of names, written as the sorted list) is a py::set<py::str>; the Python also lets it be
//     any other value, which nothing does.
//   * Python ints are int64_t.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>
#include <vector>

#include "codebreaker/translate/ir.hpp"
#include "py.hpp"
#include "../../../../l2/src/value.hpp"

namespace codebreaker::translate::native_l2 {

namespace ir = codebreaker::translate::ir;
using ir::Dict;
using ir::Val;
using Wire = ::py::V;

// An error inside the library that the Python has no exception for.
struct NativeError : py::RuntimeError {
    NativeError() : py::RuntimeError() {}
    explicit NativeError(const std::string& m) : py::RuntimeError(m) {}
    const char* type() const override { return "NativeError"; }
};

// Where cbl2.dll is: the `l2_bin` setting (default native/l2/build/bin).
py::str dll_path();
// The native writer loads in this process (always: it is linked in).
bool enabled();
// Why it does not (never: nullopt).
std::optional<py::str> why_not();
// The decompiler's note an L2 document was read from: a crossing cache here, nothing to keep.
void known_text(const Dict& doc, const py::str& text);

// ---------------------------------------------------------------------------------------------- values
// A JSON document as the library's value (TypeError for a Type, which is no JSON), and back (a tuple or
// a set is a list).
Wire to_wire(const Val& v);
Val from_wire(const Wire& v);

// One entry point with its arguments already as the tagged JSON array -> (status, the answer's text).
std::tuple<int64_t, py::str> call_raw(const py::str& fn, const py::str& text);
// An entry point of the library with `args` -> its result. `facts`: a dict the call changes in place
// (backends/c.function's): the keys it wrote are put back, also when it raises.
Wire call(const py::str& fn, const std::vector<Wire>& args, const std::optional<Wire>& facts = std::nullopt);

// ---------------------------------------------------------------------------------------------- backends/c.py
// (the entries c.py hands on: its `function`, `module`, ... are these)

// the fields of module_names32's answer
struct ModuleNames {
    py::set<py::str> names;
    py::set<py::str> calls;
    py::set<py::str> called_through;
    py::list<std::tuple<py::str, int64_t>> arity;     // (name, how many arguments it is called with)
};

// (doc, moved=(), far=None, facts=None) -> C text of one function. `facts` is changed in place.
py::str function(const Dict& doc, const py::list<py::str>& moved = py::list<py::str>(),
                 const std::optional<py::set<py::str>>& far = std::nullopt, const std::optional<Wire>& facts = std::nullopt);
py::str function(const Wire& doc, const py::list<py::str>& moved = py::list<py::str>(),
                 const std::optional<py::set<py::str>>& far = std::nullopt, const std::optional<Wire>& facts = std::nullopt);
// (doc, moved=MOVED) -> (C text, {name: assembly}) of a Module
std::tuple<py::str, py::dict<py::str, py::str>> module(const Dict& doc, const std::optional<py::list<py::str>>& moved = std::nullopt);
std::tuple<py::str, py::dict<py::str, py::str>> module(const Wire& doc, const std::optional<py::list<py::str>>& moved = std::nullopt);
// (node) -> a MachineFragment as the C of its module holds it
py::str fragment(const Dict& node);
py::str fragment(const Wire& node);
// (t, name='', far=False) -> a declaration, or the type alone
py::str type_text(const Val& t, const py::str& name = py::str(""), bool far = false);
// (e) -> C text of an expression
py::str expr(const Dict& e);
py::str expr(const Wire& e);
// (e, inner=False) -> a branch's condition, bracketed as PrintC does
py::str condition(const Dict& e, bool inner = false);
py::str condition(const Wire& e, bool inner = false);
// (doc, facts) -> the function's flags in the module
py::set<py::str> flags(const Dict& doc, const Wire& facts);
py::set<py::str> flags(const Wire& doc, const Wire& facts);
// (doc) -> `int2 __cdecl16far sub_1_0000(void)`
py::str signature(const Dict& doc);
py::str signature(const Wire& doc);
// (signature) -> arguments a 32-bit definition takes, None for __cdecl
std::optional<int64_t> takes32(const py::str& signature);
// (doc) -> {'names', 'calls', 'called_through', 'arity'} of a 32-bit function
ModuleNames module_names32(const Dict& doc);
ModuleNames module_names32(const Wire& doc);
// (doc) -> the decompiler's own type names a function's tree uses
py::set<py::str> type_names(const Dict& doc);
py::set<py::str> type_names(const Wire& doc);
// (doc) -> its parameters' names as its heading writes them
py::list<py::str> parameter_names(const Dict& doc);
py::list<py::str> parameter_names(const Wire& doc);
// (doc, param) -> [(base type, stars, offset)] through `param`
py::list<std::tuple<py::str, py::str, int64_t>> struct_accesses(const Dict& doc, const py::str& param);
py::list<std::tuple<py::str, py::str, int64_t>> struct_accesses(const Wire& doc, const py::str& param);
// (headings, order) -> {name: a declaration with its parameters}; `headings` is {name: the facts["heading"] of
// the function's writing}
py::dict<py::str, py::str> prototypes(const Wire& headings, const py::list<py::str>& order);
// (functions, near, where, sites, code, data_segment, other_data, first_selector) -> {function: its plan}.
// `code` is read only as the set of its keys: a dict of segments' bytes, or a set.
Wire segment_plan(const Wire& functions, const Wire& near, const Wire& where, const Wire& sites, const Wire& code,
                  const Wire& data_segment, const Wire& other_data, const Wire& first_selector);
// (reports, frames) -> the helpers that work in their callers' frames
Wire caller_frame_plan(const Wire& reports, const Wire& frames);
// (fields) -> a selector's structure as a declaration's body, nullopt on overlap. `fields` is {offset: type}.
std::optional<py::list<py::str>> seg_layout(const py::dict<int64_t, py::str>& fields);
// (text, open_paren) -> ([argument text], index after `)`)
std::tuple<py::list<py::str>, int64_t> arguments_at(const py::str& text, int64_t open_paren);
// (ty) -> the words a value of C type `ty` takes on the stack, or None
std::optional<int64_t> words(const py::str& ty);
// (text) -> (heading, [(name, type)]) of an older-style definition, or None
std::optional<std::tuple<py::str, py::list<std::tuple<py::str, py::str>>>> parameters(const py::str& text);
// (arg, types) -> the words an argument takes on the stack, or None
std::optional<int64_t> argument_words(const py::str& arg, const Wire& types);

// ---------------------------------------------------------------------------------------------- the L2 core
// json.dumps(l2.to_json(l2.from_json(doc))), from the native core.
py::str roundtrip_text(const Dict& doc);
py::str roundtrip_text(const Wire& doc);
// (valid, why) of `doc` against l2.schema.json, as l2.validate decides.
std::tuple<bool, py::str> validate(const Dict& doc);

}  // namespace codebreaker::translate::native_l2
