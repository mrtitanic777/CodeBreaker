// codebreaker/translate/native_l2.cpp -- typed C++ port of codebreaker.translate.native_l2 (see the header).
#include "codebreaker/translate/native_l2.hpp"

#include <algorithm>
#include <fstream>
#include <mutex>
#include <sstream>

#include "codebreaker/translate/backends/c.hpp"
#include "codebreaker/translate/backends/cpp17.hpp"
#include "codebreaker/translate/l2.hpp"
#include "../../../../l2/src/json.hpp"

// the library's entry (native/l2/src/api.cc, compiled into the program with CBL2_EXPORTS): the DLL's l2_call
extern "C" {
int l2_call(const char* fn, const char* in, size_t inlen, char** out, size_t* outlen);
void l2_free(char* p);
}

namespace codebreaker::translate::native_l2 {

namespace {

py::str S(const std::string& utf8) { return py::bytes(utf8).decode("utf-8"); }
py::str K(const char* s) { return py::str(s); }
Wire W(const py::str& s) { return Wire(s.utf8()); }
Wire wire_str_list(const py::list<py::str>& xs) {
    std::vector<Wire> items;
    for (const py::str& s : xs) items.push_back(W(s));
    return Wire::list(items);
}
// the items of a tuple, list or set answer
const std::vector<Wire>& items_of(const Wire& v) { return v.seq(); }

py::set<py::str> str_set(const Wire& v) {
    py::set<py::str> out;
    for (const Wire& x : items_of(v)) out.add(S(x.str()));
    return out;
}
py::list<py::str> str_list(const Wire& v) {
    py::list<py::str> out;
    for (const Wire& x : items_of(v)) out.append(S(x.str()));
    return out;
}

// dict.update: the keys of `from` put into `facts`
void update(const Wire& facts, const Wire& from) {
    if (!from.is_dict()) return;
    for (const auto& kv : from.items()) facts.set(kv.first, kv.second);
}

// backends/c.py's _raise
[[noreturn]] void raise_(const Wire& err) {
    std::string t = err.get("type").is_str() ? err.get("type").str() : std::string("None");
    std::string msg = err.has("message") ? err.get("message").str() : std::string();
    if (t == "Decline") throw backends::c::Decline(S(msg));
    if (t == "Cpp17Decline") throw backends::cpp17::Decline(S(msg));
    if (t == "KeyError") throw py::KeyError(msg);
    if (t == "ValueError") throw py::ValueError(msg);
    if (t == "TypeError") throw py::TypeError(msg);
    if (t == "IndexError") throw py::IndexError(msg);
    if (t == "AttributeError") throw py::AttributeError(msg);
    throw NativeError(t + ": " + msg);
}

}  // namespace

py::str dll_path() {
    py::str dir = py::settings::get_opt_str(K("l2_bin")).value_or(py::str(""));
    if (py::len(dir) == 0) dir = py::os::path::join(py::settings::checkout_root(), K("native"), K("l2"), K("build"), K("bin"));
    return py::os::path::join(dir, K("cbl2.dll"));
}

bool enabled() { return true; }
std::optional<py::str> why_not() { return std::nullopt; }
void known_text(const Dict&, const py::str&) {}

// ---------------------------------------------------------------------------------------------- values
Wire to_wire(const Val& v) {
    return std::visit(
        [](const auto& x) -> Wire {
            using T = std::decay_t<decltype(x)>;
            if constexpr (std::is_same_v<T, std::monostate>) return Wire();
            else if constexpr (std::is_same_v<T, bool>) return Wire(x);
            else if constexpr (std::is_same_v<T, int64_t>) return Wire((long long)x);
            else if constexpr (std::is_same_v<T, double>) return Wire(x);
            else if constexpr (std::is_same_v<T, py::str>) return Wire(x.utf8());
            else if constexpr (std::is_same_v<T, ir::TypeP>) throw py::TypeError("no L2 shim form for 'Type'");
            else if constexpr (std::is_same_v<T, Val::List>) {
                std::vector<Wire> items;
                for (const Val& e : x) items.push_back(to_wire(e));
                return Wire::list(items);
            } else {
                Wire out = Wire::dict();
                for (const auto& kv : x.items()) out.set(Wire(std::get<0>(kv).utf8()), to_wire(std::get<1>(kv)));
                return out;
            }
        },
        v.v);
}

Val from_wire(const Wire& v) {
    switch (v.k) {
        case ::py::K::None: return Val();
        case ::py::K::Bool: return Val(v.b);
        case ::py::K::Int:
            if (v.u64) throw py::OverflowError("int too large for int64");   // (2**63 .. 2**64-1: a Val is int64)
            return Val(static_cast<int64_t>(v.i));
        case ::py::K::Float: return Val(v.f);
        case ::py::K::Str: return Val(S(v.str()));
        case ::py::K::List:
        case ::py::K::Tuple:
        case ::py::K::Set: {
            Val::List out;
            for (const Wire& x : v.seq()) out.append(from_wire(x));
            return Val(out);
        }
        case ::py::K::Dict: {
            Dict out;
            for (const auto& kv : v.items()) {
                if (!kv.first.is_str()) throw py::TypeError("a dict with keys other than str is no JSON document");
                out.set(S(kv.first.str()), from_wire(kv.second));
            }
            return Val(out);
        }
    }
    return Val();
}

std::tuple<int64_t, py::str> call_raw(const py::str& fn, const py::str& text) {
    std::string name = fn.utf8(), data = text.utf8();
    char* out = nullptr;
    size_t n = 0;
    int status = l2_call(name.c_str(), data.data(), data.size(), &out, &n);
    std::string got(out, n);
    l2_free(out);
    return {status, S(got)};
}

Wire call(const py::str& fn, const std::vector<Wire>& args, const std::optional<Wire>& facts) {
    std::string text = "[";
    for (size_t k = 0; k < args.size(); k++) {
        if (k) text += ",";
        text += ::py::json_tagged(args[k]);
    }
    text += "]";
    auto [status, got] = call_raw(fn, S(text));
    Wire res = ::py::json_parse(got.utf8(), true);
    if (status) {
        if (facts && res.is_dict() && ::py::truthy(res.get("facts"))) update(*facts, res.get("facts"));
        raise_(res);
    }
    return res;
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

std::tuple<py::str, py::dict<py::str, py::str>> module(const Dict& doc, const std::optional<py::list<py::str>>& moved) {
    Wire r = call(K("module"), {to_wire(Val(doc)), wire_str_list(moved ? *moved : backends::c::MOVED)});
    py::dict<py::str, py::str> asm_;
    for (const auto& kv : r[1].items()) asm_.set(S(kv.first.str()), S(kv.second.str()));
    return {S(r[0].str()), asm_};
}

// (the same for a document held as the library's value: an int past int64, which a Val cannot hold, stays whole)
std::tuple<py::str, py::dict<py::str, py::str>> module(const Wire& doc, const std::optional<py::list<py::str>>& moved) {
    Wire r = call(K("module"), {doc, wire_str_list(moved ? *moved : backends::c::MOVED)});
    py::dict<py::str, py::str> asm_;
    for (const auto& kv : r[1].items()) asm_.set(S(kv.first.str()), S(kv.second.str()));
    return {S(r[0].str()), asm_};
}

py::str fragment(const Dict& node) { return S(call(K("fragment"), {to_wire(Val(node))}).str()); }
py::str fragment(const Wire& node) { return S(call(K("fragment"), {node}).str()); }

py::str type_text(const Val& t, const py::str& name, bool far) {
    return S(call(K("type_text"), {to_wire(t), W(name), Wire(far)}).str());
}

py::str expr(const Dict& e) { return S(call(K("expr"), {to_wire(Val(e))}).str()); }
py::str expr(const Wire& e) { return S(call(K("expr"), {e}).str()); }

py::str condition(const Dict& e, bool inner) { return S(call(K("condition"), {to_wire(Val(e)), Wire(inner)}).str()); }
py::str condition(const Wire& e, bool inner) { return S(call(K("condition"), {e, Wire(inner)}).str()); }

py::set<py::str> flags(const Dict& doc, const Wire& facts) {
    return str_set(call(K("flags"), {to_wire(Val(doc)), facts}));
}

// (the same for a document held as the library's value: an int past int64, which a Val cannot hold, stays whole)
py::set<py::str> flags(const Wire& doc, const Wire& facts) {
    return str_set(call(K("flags"), {doc, facts}));
}

py::str signature(const Dict& doc) { return S(call(K("signature"), {to_wire(Val(doc))}).str()); }
py::str signature(const Wire& doc) { return S(call(K("signature"), {doc}).str()); }

std::optional<int64_t> takes32(const py::str& sig) {
    Wire r = call(K("takes32"), {W(sig)});
    if (r.is_none()) return std::nullopt;
    return r.as_int();
}

ModuleNames module_names32(const Dict& doc) {
    Wire r = call(K("module_names32"), {to_wire(Val(doc))});
    ModuleNames out;
    out.names = str_set(r.at("names"));
    out.calls = str_set(r.at("calls"));
    out.called_through = str_set(r.at("called_through"));
    for (const Wire& t : items_of(r.at("arity"))) out.arity.append(std::make_tuple(S(t[0].str()), static_cast<int64_t>(t[1].as_int())));
    return out;
}

// (the same for a document held as the library's value: an int past int64, which a Val cannot hold, stays whole)
ModuleNames module_names32(const Wire& doc) {
    Wire r = call(K("module_names32"), {doc});
    ModuleNames out;
    out.names = str_set(r.at("names"));
    out.calls = str_set(r.at("calls"));
    out.called_through = str_set(r.at("called_through"));
    for (const Wire& t : items_of(r.at("arity"))) out.arity.append(std::make_tuple(S(t[0].str()), static_cast<int64_t>(t[1].as_int())));
    return out;
}

py::set<py::str> type_names(const Dict& doc) { return str_set(call(K("type_names"), {to_wire(Val(doc))})); }
py::set<py::str> type_names(const Wire& doc) { return str_set(call(K("type_names"), {doc})); }

py::list<py::str> parameter_names(const Dict& doc) { return str_list(call(K("parameter_names"), {to_wire(Val(doc))})); }
py::list<py::str> parameter_names(const Wire& doc) { return str_list(call(K("parameter_names"), {doc})); }

py::list<std::tuple<py::str, py::str, int64_t>> struct_accesses(const Dict& doc, const py::str& param) {
    Wire r = call(K("struct_accesses"), {to_wire(Val(doc)), W(param)});
    py::list<std::tuple<py::str, py::str, int64_t>> out;
    for (const Wire& t : items_of(r)) out.append(std::make_tuple(S(t[0].str()), S(t[1].str()), static_cast<int64_t>(t[2].as_int())));
    return out;
}

// (the same for a document held as the library's value: an int past int64, which a Val cannot hold, stays whole)
py::list<std::tuple<py::str, py::str, int64_t>> struct_accesses(const Wire& doc, const py::str& param) {
    Wire r = call(K("struct_accesses"), {doc, W(param)});
    py::list<std::tuple<py::str, py::str, int64_t>> out;
    for (const Wire& t : items_of(r)) out.append(std::make_tuple(S(t[0].str()), S(t[1].str()), static_cast<int64_t>(t[2].as_int())));
    return out;
}

py::dict<py::str, py::str> prototypes(const Wire& headings, const py::list<py::str>& order) {
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

Wire caller_frame_plan(const Wire& reports, const Wire& frames) { return call(K("caller_frame_plan"), {reports, frames}); }

std::optional<py::list<py::str>> seg_layout(const py::dict<int64_t, py::str>& fields) {
    Wire f = Wire::dict();
    for (const auto& kv : fields.items()) f.set(Wire((long long)std::get<0>(kv)), W(std::get<1>(kv)));
    Wire r = call(K("seg_layout"), {f});
    if (r.is_none()) return std::nullopt;
    return str_list(r);
}

std::tuple<py::list<py::str>, int64_t> arguments_at(const py::str& text, int64_t open_paren) {
    Wire r = call(K("arguments_at"), {W(text), Wire((long long)open_paren)});
    return {str_list(r[0]), static_cast<int64_t>(r[1].as_int())};
}

std::optional<int64_t> words(const py::str& ty) {
    Wire r = call(K("words"), {W(ty)});
    if (r.is_none()) return std::nullopt;
    return r.as_int();
}

std::optional<std::tuple<py::str, py::list<std::tuple<py::str, py::str>>>> parameters(const py::str& text) {
    Wire r = call(K("parameters"), {W(text)});
    if (r.is_none()) return std::nullopt;
    py::list<std::tuple<py::str, py::str>> ps;
    for (const Wire& t : items_of(r[1])) ps.append(std::make_tuple(S(t[0].str()), S(t[1].str())));
    return std::make_tuple(S(r[0].str()), ps);
}

std::optional<int64_t> argument_words(const py::str& arg, const Wire& types) {
    Wire r = call(K("argument_words"), {W(arg), types});
    if (r.is_none()) return std::nullopt;
    return r.as_int();
}

// ---------------------------------------------------------------------------------------------- the L2 core
namespace {
std::once_flag schema_once;
void load_schema() {
    std::call_once(schema_once, [] {
        std::ifstream f(l2::SCHEMA.utf8(), std::ios::binary);
        if (!f) throw py::Exception("cannot open " + l2::SCHEMA.utf8());   // (FileNotFoundError)
        std::stringstream ss;
        ss << f.rdbuf();
        call_raw(K("set_schema"), S("[" + ss.str() + "]"));
    });
}
}  // namespace

py::str roundtrip_text(const Dict& doc) { return S(call(K("roundtrip_text"), {to_wire(Val(doc))}).str()); }
py::str roundtrip_text(const Wire& doc) { return S(call(K("roundtrip_text"), {doc}).str()); }

std::tuple<bool, py::str> validate(const Dict& doc) {
    load_schema();
    Wire r = call(K("validate"), {to_wire(Val(doc))});
    return {::py::truthy(r[0]), S(r[1].str())};
}

}  // namespace codebreaker::translate::native_l2
