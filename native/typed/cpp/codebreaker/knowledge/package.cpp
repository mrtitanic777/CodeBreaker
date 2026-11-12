// codebreaker/knowledge/package.cpp -- typed C++ port of codebreaker/knowledge/__init__.py (see package.hpp).
#include "codebreaker/knowledge/package.hpp"

#include "cb/compat/py_bridge.hpp"
#include "cb/knowledge/package.hpp"

#include "codebreaker/knowledge/reads.hpp"
#include "codebreaker/knowledge/store.hpp"
#include "codebreaker/symbols/api_arguments.hpp"
#include "codebreaker/symbols/ordinals.hpp"
#include "codebreaker/symbols/runtime_library.hpp"
#include "py_os.hpp"

namespace codebreaker::knowledge {

using py::str;

// The platform vocabulary is cb/knowledge/package.hpp's; these forward to it (the rest of this file is the legacy base
// over engine/native_engine's KnowledgeBase, which the new cb::knowledge::Store replaces when native_engine is
// refactored: COMPAT(py): remove then).
const py::dict<str, str>& PLATFORMS() {
    static const py::dict<str, str> p = [] {
        py::dict<str, str> d;
        for (const cb::knowledge::PlatformInfo& info : cb::knowledge::known_platforms())
            d.set(cb::compat::to_py(info.name), cb::compat::to_py(info.title));
        return d;
    }();
    return p;
}

str platform(const str& name) { return cb::compat::to_py(cb::knowledge::platform(cb::compat::from_py(name))); }

str platform(int64_t bits) { return cb::compat::to_py(cb::knowledge::platform(static_cast<int>(bits))); }

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
str _bare(const str& name) { return cb::compat::to_py(cb::knowledge::bare_name(cb::compat::from_py(name))); }

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
