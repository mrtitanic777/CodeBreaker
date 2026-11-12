// codebreaker/translate/native_l2.hpp -- COMPAT SHIM over cb/translate/native_l2.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/translate/native_l2.{hpp,cpp}: a typed client of the L2 core library (native/l2). This header keeps
// the old API for the ports that still call it with the library's own values (`Wire`, native/l2/src/value.hpp) and the old
// ir::Dict documents: each entry point packs its arguments as the library's tagged JSON, asks the new client, and unpacks the
// answer into the old types (py::set, py::list, tuples), turning the library's errors back into the old exceptions (Decline,
// KeyError ...). The library's sources are linked into the program that includes this (l2_call / l2_free).
//
// The old header's notes on the library's values and tagged form hold as they were: tuples, sets and dicts of other keys cross
// as {"$t"}, {"$s"}, {"$d"}; `facts` is the Wire dict the call changes in place.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>
#include <vector>

#include "cb/compat/old_val_json.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/translate/native_l2.hpp"
#include "codebreaker/translate/ir.hpp"
#include "py.hpp"
#include "../../../../l2/src/json.hpp"
#include "../../../../l2/src/value.hpp"

extern "C" {
int l2_call(const char* fn, const char* in, size_t inlen, char** out, size_t* outlen);
void l2_free(char* p);
}

#include "codebreaker/translate/l2.hpp"

namespace codebreaker::translate::native_l2 {



namespace ir = codebreaker::translate::ir;
using ir::Dict;
using ir::Val;
using Wire = ::py::V;

// A node the C back end does not write (backends/c.hpp's Decline is this), and the C++17 back end's (cpp17.hpp's).
struct Decline : py::Exception {
    Decline() : py::Exception() {}
    explicit Decline(const std::string& m) : py::Exception(m) {}
    explicit Decline(const py::str& m) : py::Exception(m.utf8()) {}
    const char* type() const override { return "Decline"; }
};
struct Cpp17Decline : py::Exception {
    Cpp17Decline() : py::Exception() {}
    explicit Cpp17Decline(const std::string& m) : py::Exception(m) {}
    explicit Cpp17Decline(const py::str& m) : py::Exception(m.utf8()) {}
    const char* type() const override { return "Decline"; }
};

// An error inside the library that the Python has no exception for.
struct NativeError : py::RuntimeError {
    NativeError() : py::RuntimeError() {}
    explicit NativeError(const std::string& m) : py::RuntimeError(m) {}
    const char* type() const override { return "NativeError"; }
};

namespace detail_shim {
inline cb::translate::native_l2::Library& library() { return codebreaker::translate::l2::shim_detail::library(); }
// the library's tagged JSON as its Wire value and back (the one crossing: text)
inline cb::Json wire_to_json(const Wire& w) { return cb::Json::parse(::py::json_tagged(w)); }
inline Wire json_to_wire(const cb::Json& j) { return ::py::json_parse(j.dump(-1, ' ', false), true); }
[[noreturn]] inline void throw_old(const std::string& type, const std::string& message) {
    if (type == "Decline") throw Decline(message);
    if (type == "Cpp17Decline") throw Cpp17Decline(message);
    if (type == "KeyError") throw py::KeyError(message);
    if (type == "ValueError") throw py::ValueError(message);
    if (type == "TypeError") throw py::TypeError(message);
    if (type == "IndexError") throw py::IndexError(message);
    if (type == "AttributeError") throw py::AttributeError(message);
    throw NativeError(type + ": " + message);
}
}  // namespace detail_shim
using namespace detail_shim;

// Where cbl2.dll is: the `l2_bin` setting (default native/l2/build/bin).
inline py::str dll_path();
// The native writer loads in this process (always: it is linked in).
inline bool enabled();
// Why it does not (never: nullopt).
inline std::optional<py::str> why_not();
// The decompiler's note an L2 document was read from: a crossing cache here, nothing to keep.
inline void known_text(const Dict& doc, const py::str& text);

// ---------------------------------------------------------------------------------------------- values
// A JSON document as the library's value (TypeError for a Type, which is no JSON), and back (a tuple or
// a set is a list).
inline Wire to_wire(const Val& v);
inline Val from_wire(const Wire& v);

// One entry point with its arguments already as the tagged JSON array -> (status, the answer's text).
inline std::tuple<int64_t, py::str> call_raw(const py::str& fn, const py::str& text);
// An entry point of the library with `args` -> its result. `facts`: a dict the call changes in place
// (backends/c.function's): the keys it wrote are put back, also when it raises.
inline Wire call(const py::str& fn, const std::vector<Wire>& args, const std::optional<Wire>& facts = std::nullopt);

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
inline py::str function(const Dict& doc, const py::list<py::str>& moved = py::list<py::str>(),
                 const std::optional<py::set<py::str>>& far = std::nullopt, const std::optional<Wire>& facts = std::nullopt);
inline py::str function(const Wire& doc, const py::list<py::str>& moved = py::list<py::str>(),
                 const std::optional<py::set<py::str>>& far = std::nullopt, const std::optional<Wire>& facts = std::nullopt);
// (doc, moved=MOVED) -> (C text, {name: assembly}) of a Module
inline std::tuple<py::str, py::dict<py::str, py::str>> module(const Dict& doc, const std::optional<py::list<py::str>>& moved = std::nullopt);
inline std::tuple<py::str, py::dict<py::str, py::str>> module(const Wire& doc, const std::optional<py::list<py::str>>& moved = std::nullopt);
// (node) -> a MachineFragment as the C of its module holds it
inline py::str fragment(const Dict& node);
inline py::str fragment(const Wire& node);
// (t, name='', far=False) -> a declaration, or the type alone
inline py::str type_text(const Val& t, const py::str& name = py::str(""), bool far = false);
// (e) -> C text of an expression
inline py::str expr(const Dict& e);
inline py::str expr(const Wire& e);
// (e, inner=False) -> a branch's condition, bracketed as PrintC does
inline py::str condition(const Dict& e, bool inner = false);
inline py::str condition(const Wire& e, bool inner = false);
// (doc, facts) -> the function's flags in the module
inline py::set<py::str> flags(const Dict& doc, const Wire& facts);
inline py::set<py::str> flags(const Wire& doc, const Wire& facts);
// (doc) -> `int2 __cdecl16far sub_1_0000(void)`
inline py::str signature(const Dict& doc);
inline py::str signature(const Wire& doc);
// (signature) -> arguments a 32-bit definition takes, None for __cdecl
inline std::optional<int64_t> takes32(const py::str& signature);
// (doc) -> {'names', 'calls', 'called_through', 'arity'} of a 32-bit function
inline ModuleNames module_names32(const Dict& doc);
inline ModuleNames module_names32(const Wire& doc);
// (doc) -> the decompiler's own type names a function's tree uses
inline py::set<py::str> type_names(const Dict& doc);
inline py::set<py::str> type_names(const Wire& doc);
// (doc) -> its parameters' names as its heading writes them
inline py::list<py::str> parameter_names(const Dict& doc);
inline py::list<py::str> parameter_names(const Wire& doc);
// (doc, param) -> [(base type, stars, offset)] through `param`
inline py::list<std::tuple<py::str, py::str, int64_t>> struct_accesses(const Dict& doc, const py::str& param);
inline py::list<std::tuple<py::str, py::str, int64_t>> struct_accesses(const Wire& doc, const py::str& param);
// (headings, order) -> {name: a declaration with its parameters}; `headings` is {name: the facts["heading"] of
// the function's writing}
inline py::dict<py::str, py::str> prototypes(const Wire& headings, const py::list<py::str>& order);
// (functions, near, where, sites, code, data_segment, other_data, first_selector) -> {function: its plan}.
// `code` is read only as the set of its keys: a dict of segments' bytes, or a set.
inline Wire segment_plan(const Wire& functions, const Wire& near, const Wire& where, const Wire& sites, const Wire& code,
                  const Wire& data_segment, const Wire& other_data, const Wire& first_selector);
// (reports, frames) -> the helpers that work in their callers' frames
inline Wire caller_frame_plan(const Wire& reports, const Wire& frames);
// (fields) -> a selector's structure as a declaration's body, nullopt on overlap. `fields` is {offset: type}.
inline std::optional<py::list<py::str>> seg_layout(const py::dict<int64_t, py::str>& fields);
// (text, open_paren) -> ([argument text], index after `)`)
inline std::tuple<py::list<py::str>, int64_t> arguments_at(const py::str& text, int64_t open_paren);
// (ty) -> the words a value of C type `ty` takes on the stack, or None
inline std::optional<int64_t> words(const py::str& ty);
// (text) -> (heading, [(name, type)]) of an older-style definition, or None
inline std::optional<std::tuple<py::str, py::list<std::tuple<py::str, py::str>>>> parameters(const py::str& text);
// (arg, types) -> the words an argument takes on the stack, or None
inline std::optional<int64_t> argument_words(const py::str& arg, const Wire& types);

// ---------------------------------------------------------------------------------------------- the L2 core
// json.dumps(l2.to_json(l2.from_json(doc))), from the native core.
inline py::str roundtrip_text(const Dict& doc);
inline py::str roundtrip_text(const Wire& doc);
// (valid, why) of `doc` against l2.schema.json, as l2.validate decides.
inline std::tuple<bool, py::str> validate(const Dict& doc);




namespace detail_shim {

inline py::str S(const std::string& utf8) { return py::bytes(utf8).decode("utf-8"); }
inline py::str K(const char* s) { return py::str(s); }
inline Wire W(const py::str& s) { return Wire(s.utf8()); }
inline Wire wire_str_list(const py::list<py::str>& xs) {
    std::vector<Wire> items;
    for (const py::str& s : xs) items.push_back(W(s));
    return Wire::list(items);
}
// the items of a tuple, list or set answer
inline const std::vector<Wire>& items_of(const Wire& v) { return v.seq(); }

inline py::set<py::str> str_set(const Wire& v) {
    py::set<py::str> out;
    for (const Wire& x : items_of(v)) out.add(S(x.str()));
    return out;
}
inline py::list<py::str> str_list(const Wire& v) {
    py::list<py::str> out;
    for (const Wire& x : items_of(v)) out.append(S(x.str()));
    return out;
}

// dict.update: the keys of `from` put into `facts`
inline void update(const Wire& facts, const Wire& from) {
    if (!from.is_dict()) return;
    for (const auto& kv : from.items()) facts.set(kv.first, kv.second);
}

// c_text's passes done on L2, every one there is (backends/c.py's MOVED): what `module` is asked to do by default
inline const py::list<py::str>& default_moved() {
    static const py::list<py::str> all = [] {
        py::list<py::str> out;
        for (const char* n :
             {"far", "near_calls", "far_from_stack", "frame_through_ss", "pieces", "segment_registers", "null_is_zero",
              "global_pointer_steps", "one_name_for_globals", "far_result", "model_word", "joined_arguments", "far_arguments",
              "void_result", "byte_result", "export", "loadds", "selector_structs", "struct_gaps", "own_segment_calls",
              "open_arguments", "interrupts", "far_addresses", "far_casts", "entry_sp", "stack_arguments", "far_locals",
              "far_concat_casts", "buffer_fields", "far_reads", "near_to_far", "stack_segment_stores", "callee_pops",
              "caller_frame_slots", "selector_imports", "segment_values", "code_segment_reads", "names_used", "heading_facts",
              "int_result", "register_arguments", "code_pointers", "compare_as_numbers", "keep_frame", "globals_through",
              "global_widths", "same_cast", "fit_calls", "struct_fields", "wide_address", "homed_arguments", "uint8_result"})
            out.append(py::str(n));
        return out;
    }();
    return all;
}

}  // namespace detail_shim
using namespace detail_shim;

inline py::str dll_path() {
    py::str dir = py::settings::get_opt_str(K("l2_bin")).value_or(py::str(""));
    if (py::len(dir) == 0) dir = py::os::path::join(py::settings::checkout_root(), K("native"), K("l2"), K("build"), K("bin"));
    return py::os::path::join(dir, K("cbl2.dll"));
}

inline bool enabled() { return true; }
inline std::optional<py::str> why_not() { return std::nullopt; }
inline void known_text(const Dict&, const py::str&) {}

// ---------------------------------------------------------------------------------------------- values
inline Wire to_wire(const Val& v) { return json_to_wire(cb::compat::to_json_value(v)); }
inline Val from_wire(const Wire& v) { return cb::compat::from_json_value(wire_to_json(v)); }

inline std::tuple<int64_t, py::str> call_raw(const py::str& fn, const py::str& text) {
    const auto [status, answer] = library().call_raw(fn.utf8(), text.utf8());
    return {status, S(answer)};
}

inline Wire call(const py::str& fn, const std::vector<Wire>& args, const std::optional<Wire>& facts) {
    cb::Json arguments = cb::Json::array();
    for (const Wire& a : args) arguments.push_back(wire_to_json(a));
    cb::Json written = cb::Json::object();   // the facts the library wrote, kept also when it fails
    try {
        return json_to_wire(library().call(fn.utf8(), arguments, &written));
    } catch (const cb::translate::native_l2::LibraryError& e) {
        if (facts && facts->is_dict()) {
            for (const auto& [k, v] : written.items()) facts->set(Wire(k), json_to_wire(v));
        }
        std::string type = e.type();
        throw_old(type, e.what());
    }
}

// ---------------------------------------------------------------------------------------------- backends/c.py
py::str function(const Dict& doc, const py::list<py::str>& moved, const std::optional<py::set<py::str>>& far,
                 const std::optional<Wire>& facts) {
    return function(to_wire(Val(doc)), moved, far, facts);
}

py::str function(const Wire& doc, const py::list<py::str>& moved, const std::optional<py::set<py::str>>& far,
                 const std::optional<Wire>& facts) {
    Wire far_w;                                            // None
    if (far) {
        std::vector<std::string> names;
        for (const py::str& s : *far) names.push_back(s.utf8());
        std::sort(names.begin(), names.end());               // (sorted(far): code points = UTF-8 byte order)
        std::vector<Wire> items;
        for (const std::string& s : names) items.push_back(Wire(s));
        far_w = Wire::list(items);
    }
    Wire r = call(K("function"), {doc, wire_str_list(moved), far_w, facts ? *facts : Wire()}, facts);
    if (facts) update(*facts, r[1]);
    return S(r[0].str());
}

inline std::tuple<py::str, py::dict<py::str, py::str>> module(const Dict& doc, const std::optional<py::list<py::str>>& moved) {
    Wire r = call(K("module"), {to_wire(Val(doc)), wire_str_list(moved ? *moved : default_moved())});
    py::dict<py::str, py::str> asm_;
    for (const auto& kv : r[1].items()) asm_.set(S(kv.first.str()), S(kv.second.str()));
    return {S(r[0].str()), asm_};
}

// (the same for a document held as the library's value: an int past int64, which a Val cannot hold, stays whole)
inline std::tuple<py::str, py::dict<py::str, py::str>> module(const Wire& doc, const std::optional<py::list<py::str>>& moved) {
    Wire r = call(K("module"), {doc, wire_str_list(moved ? *moved : default_moved())});
    py::dict<py::str, py::str> asm_;
    for (const auto& kv : r[1].items()) asm_.set(S(kv.first.str()), S(kv.second.str()));
    return {S(r[0].str()), asm_};
}

inline py::str fragment(const Dict& node) { return S(call(K("fragment"), {to_wire(Val(node))}).str()); }
inline py::str fragment(const Wire& node) { return S(call(K("fragment"), {node}).str()); }

inline py::str type_text(const Val& t, const py::str& name, bool far) {
    return S(call(K("type_text"), {to_wire(t), W(name), Wire(far)}).str());
}

inline py::str expr(const Dict& e) { return S(call(K("expr"), {to_wire(Val(e))}).str()); }
inline py::str expr(const Wire& e) { return S(call(K("expr"), {e}).str()); }

inline py::str condition(const Dict& e, bool inner) { return S(call(K("condition"), {to_wire(Val(e)), Wire(inner)}).str()); }
inline py::str condition(const Wire& e, bool inner) { return S(call(K("condition"), {e, Wire(inner)}).str()); }

inline py::set<py::str> flags(const Dict& doc, const Wire& facts) {
    return str_set(call(K("flags"), {to_wire(Val(doc)), facts}));
}

// (the same for a document held as the library's value: an int past int64, which a Val cannot hold, stays whole)
inline py::set<py::str> flags(const Wire& doc, const Wire& facts) {
    return str_set(call(K("flags"), {doc, facts}));
}

inline py::str signature(const Dict& doc) { return S(call(K("signature"), {to_wire(Val(doc))}).str()); }
inline py::str signature(const Wire& doc) { return S(call(K("signature"), {doc}).str()); }

inline std::optional<int64_t> takes32(const py::str& sig) {
    Wire r = call(K("takes32"), {W(sig)});
    if (r.is_none()) return std::nullopt;
    return r.as_int();
}

inline ModuleNames module_names32(const Dict& doc) {
    Wire r = call(K("module_names32"), {to_wire(Val(doc))});
    ModuleNames out;
    out.names = str_set(r.at("names"));
    out.calls = str_set(r.at("calls"));
    out.called_through = str_set(r.at("called_through"));
    for (const Wire& t : items_of(r.at("arity"))) out.arity.append(std::make_tuple(S(t[0].str()), static_cast<int64_t>(t[1].as_int())));
    return out;
}

// (the same for a document held as the library's value: an int past int64, which a Val cannot hold, stays whole)
inline ModuleNames module_names32(const Wire& doc) {
    Wire r = call(K("module_names32"), {doc});
    ModuleNames out;
    out.names = str_set(r.at("names"));
    out.calls = str_set(r.at("calls"));
    out.called_through = str_set(r.at("called_through"));
    for (const Wire& t : items_of(r.at("arity"))) out.arity.append(std::make_tuple(S(t[0].str()), static_cast<int64_t>(t[1].as_int())));
    return out;
}

inline py::set<py::str> type_names(const Dict& doc) { return str_set(call(K("type_names"), {to_wire(Val(doc))})); }
inline py::set<py::str> type_names(const Wire& doc) { return str_set(call(K("type_names"), {doc})); }

inline py::list<py::str> parameter_names(const Dict& doc) { return str_list(call(K("parameter_names"), {to_wire(Val(doc))})); }
inline py::list<py::str> parameter_names(const Wire& doc) { return str_list(call(K("parameter_names"), {doc})); }

inline py::list<std::tuple<py::str, py::str, int64_t>> struct_accesses(const Dict& doc, const py::str& param) {
    Wire r = call(K("struct_accesses"), {to_wire(Val(doc)), W(param)});
    py::list<std::tuple<py::str, py::str, int64_t>> out;
    for (const Wire& t : items_of(r)) out.append(std::make_tuple(S(t[0].str()), S(t[1].str()), static_cast<int64_t>(t[2].as_int())));
    return out;
}

// (the same for a document held as the library's value: an int past int64, which a Val cannot hold, stays whole)
inline py::list<std::tuple<py::str, py::str, int64_t>> struct_accesses(const Wire& doc, const py::str& param) {
    Wire r = call(K("struct_accesses"), {doc, W(param)});
    py::list<std::tuple<py::str, py::str, int64_t>> out;
    for (const Wire& t : items_of(r)) out.append(std::make_tuple(S(t[0].str()), S(t[1].str()), static_cast<int64_t>(t[2].as_int())));
    return out;
}

inline py::dict<py::str, py::str> prototypes(const Wire& headings, const py::list<py::str>& order) {
    Wire r = call(K("prototypes"), {headings, wire_str_list(order)});
    py::dict<py::str, py::str> out;
    for (const auto& kv : r.items()) out.set(S(kv.first.str()), S(kv.second.str()));
    return out;
}

Wire segment_plan(const Wire& functions, const Wire& near, const Wire& where, const Wire& sites, const Wire& code,
                  const Wire& data_segment, const Wire& other_data, const Wire& first_selector) {
    // (`code` is read only as the set of its keys; sorted)
    std::vector<Wire> keys;
    if (code.is_dict()) {
        for (const auto& kv : code.items()) keys.push_back(kv.first);
    } else {
        keys = code.seq();
    }
    ::py::sort_values(keys);
    return call(K("segment_plan"), {functions, near, where, sites, Wire::list(keys), data_segment, other_data, first_selector});
}

inline Wire caller_frame_plan(const Wire& reports, const Wire& frames) { return call(K("caller_frame_plan"), {reports, frames}); }

inline std::optional<py::list<py::str>> seg_layout(const py::dict<int64_t, py::str>& fields) {
    Wire f = Wire::dict();
    for (const auto& kv : fields.items()) f.set(Wire((long long)std::get<0>(kv)), W(std::get<1>(kv)));
    Wire r = call(K("seg_layout"), {f});
    if (r.is_none()) return std::nullopt;
    return str_list(r);
}

inline std::tuple<py::list<py::str>, int64_t> arguments_at(const py::str& text, int64_t open_paren) {
    Wire r = call(K("arguments_at"), {W(text), Wire((long long)open_paren)});
    return {str_list(r[0]), static_cast<int64_t>(r[1].as_int())};
}

inline std::optional<int64_t> words(const py::str& ty) {
    Wire r = call(K("words"), {W(ty)});
    if (r.is_none()) return std::nullopt;
    return r.as_int();
}

inline std::optional<std::tuple<py::str, py::list<std::tuple<py::str, py::str>>>> parameters(const py::str& text) {
    Wire r = call(K("parameters"), {W(text)});
    if (r.is_none()) return std::nullopt;
    py::list<std::tuple<py::str, py::str>> ps;
    for (const Wire& t : items_of(r[1])) ps.append(std::make_tuple(S(t[0].str()), S(t[1].str())));
    return std::make_tuple(S(r[0].str()), ps);
}

inline std::optional<int64_t> argument_words(const py::str& arg, const Wire& types) {
    Wire r = call(K("argument_words"), {W(arg), types});
    if (r.is_none()) return std::nullopt;
    return r.as_int();
}

// ---------------------------------------------------------------------------------------------- the L2 core
inline py::str roundtrip_text(const Dict& doc) { return S(call(K("roundtrip_text"), {to_wire(Val(doc))}).str()); }
inline py::str roundtrip_text(const Wire& doc) { return S(call(K("roundtrip_text"), {doc}).str()); }

inline std::tuple<bool, py::str> validate(const Dict& doc) {
    const std::string schema = cb::fs::read_text(cb::fs::from_text(l2::SCHEMA.utf8()));
    const auto verdict = library().validate(cb::compat::to_json_value(doc), schema);
    return {verdict.valid, S(verdict.why)};
}


}  // namespace codebreaker::translate::native_l2
