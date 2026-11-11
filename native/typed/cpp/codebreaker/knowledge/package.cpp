// codebreaker/knowledge/package.cpp -- typed C++ port of codebreaker/knowledge/__init__.py (see package.hpp).
#include "codebreaker/knowledge/package.hpp"

#include "codebreaker/knowledge/reads.hpp"
#include "codebreaker/knowledge/store.hpp"
#include "codebreaker/symbols/api_arguments.hpp"
#include "codebreaker/symbols/ordinals.hpp"
#include "codebreaker/symbols/runtime_library.hpp"
#include "py_os.hpp"

namespace codebreaker::knowledge {

using py::str;

const py::dict<str, str>& PLATFORMS() {
    static const py::dict<str, str> p = [] {
        py::dict<str, str> d;
        d.set(str("win16"), str("16-bit Windows"));
        d.set(str("win32"), str("32-bit Windows"));
        d.set(str("windows"), str("16- and 32-bit Windows"));
        d.set(str("vxd"), str("ring-0 drivers"));
        d.set(str("dos"), str("DOS"));
        d.set(str("xbox360"), str("Xbox 360"));
        return d;
    }();
    return p;
}

// _ALIASES: ne, pe, le, mz, powerpc, "16", "32" (and the ints 16, 32)
str platform(const str& name) {
    static const py::dict<str, str> aliases = [] {
        py::dict<str, str> d;
        d.set(str("ne"), str("win16"));
        d.set(str("pe"), str("win32"));
        d.set(str("le"), str("vxd"));
        d.set(str("mz"), str("dos"));
        d.set(str("powerpc"), str("xbox360"));
        d.set(str("16"), str("win16"));
        d.set(str("32"), str("win32"));
        return d;
    }();
    auto it = aliases.get(name);
    return it ? *it : name;
}

str platform(int64_t bits) {
    if (bits == 16) return str("win16");
    if (bits == 32) return str("win32");
    return py::to_str(bits);
}

std::shared_ptr<KnowledgeBase> open(const std::optional<str>& path) { return store::open(path); }

// ---- anything ----
std::optional<Value> known(const str& platform_, const str& kind, const str& key) {
    reads::record(platform(platform_), kind, key);
    return store::open()->get(platform(platform_), kind, key);
}

Value known(const str& platform_, const str& kind, const str& key, const Value& default_) {
    auto got = known(platform_, kind, key);
    return got ? *got : default_;
}

void learn(const str& platform_, const str& kind, const str& key, const Value& value, const str& source) {
    store::open()->put(platform(platform_), kind, key, value, source, store::LEARNED);
}

py::dict<str, Value> _table(const str& platform_, const str& kind) {
    str p = platform(platform_);
    if (sources::AUTHORED().contains(std::make_tuple(p, kind))) {
        py::dict<str, Value> out;
        for (const auto& kv : sources::authored(p, kind).items()) out.set(std::get<0>(kv), Value(std::get<1>(kv)));
        return out;
    }
    return store::open()->table(p, kind);
}

py::dict<str, Value> table(const str& platform_, const str& kind) {
    reads::record_kind(platform(platform_), kind);
    return _table(platform_, kind);
}

// ---- prototypes ----
str _bare(const str& name) {
    auto m = py::re::fullmatch(str("\\w+[.!](\\w+)"), name);
    return m ? m->group(1) : name;
}

std::optional<symbols::prototypes::Prototype> prototype(const str& platform_, const str& name) {
    return symbols::prototypes::of(_bare(name), platform(platform_) == str("win16") ? 16 : 32);
}

std::optional<str> declaration(const str& platform_, const str& name) {
    return symbols::prototypes::declaration(_bare(name), platform(platform_) == str("win16") ? 16 : 32);
}

str spelling(const str& name) { return symbols::prototypes::spelling(name); }

std::optional<int64_t> argument_bytes(const str& platform_, const str& api, const std::optional<str>& module) {
    if (platform(platform_) == str("win16")) return symbols::api_arguments::of(api);
    if (!module) throw py::TypeError("argument_bytes: a win32 lookup needs the DLL that exports the function");
    return symbols::api_arguments::stack_bytes(*module, api);
}

std::optional<int64_t> declared_argument_bytes(const str& name) { return symbols::api_arguments::header_bytes(name); }

std::optional<str> import_name(const str& module, int64_t ordinal) { return symbols::ordinals::identifier(module, ordinal); }

// ---- names of bodies ----
std::optional<engine::native_engine::Cells> name(const str& body_id, const std::shared_ptr<KnowledgeBase>& kb) {
    return (kb ? kb : store::open())->name(body_id);
}

void name_body(const str& body_id, const str& name_, const str& source, const std::shared_ptr<KnowledgeBase>& kb) {
    (kb ? kb : store::open())->name_body(body_id, name_, source);
}

// ---- signatures, structures, behaviour ----
py::list<Value> signatures(const str& platform_) {
    py::list<Value> out;
    for (const auto& kv : table(platform_, str("signature")).items()) out.append(std::get<1>(kv));
    return out;
}

std::optional<std::tuple<str, str>> signature_matches(const str& platform_, const py::bytes& code) {
    return symbols::runtime_library::match(platform(platform_) == str("win16") ? 16 : 32, code);
}

std::optional<symbols::sdk_structs::Struct> struct_(const str& platform_, const str& name_) {
    if (platform(platform_) == str("win32")) return symbols::sdk_structs::layout(name_);
    return std::nullopt;
}

std::optional<py::dict<str, sources::Json>> api_model(const str& platform_, const str& api) {
    reads::record(platform(platform_), str("api_model"), api);
    py::dict<str, Value> rows = _table(platform_, str("api_model"));
    auto it = rows.get(api);
    if (!it) return std::nullopt;
    const sources::Json* model = std::get_if<sources::Json>(&*it);
    // `{k: v for k, v in model.items() if not k.startswith("_")} if model else None`: an empty or absent model is None
    if (!model || model->kind != sources::Json::Dict || model->dict.size() == 0) return std::nullopt;
    py::dict<str, sources::Json> out;
    for (const auto& kv : model->dict.items())
        if (!std::get<0>(kv).startswith(str("_"))) out.set(std::get<0>(kv), std::get<1>(kv));
    return out;
}

}  // namespace codebreaker::knowledge
