// cb/translate/native_l2.cpp -- the typed client of the L2 core library (see native_l2.hpp).
#include "cb/translate/native_l2.hpp"

#include <algorithm>
#include <mutex>
#include <nlohmann/json.hpp>

#include "cb/core/dynlib.hpp"
#include "cb/core/json.hpp"

namespace cb::translate::native_l2 {
namespace {

constexpr int kLibraryVersion = 1;   // l2_version() of the library this speaks to

[[noreturn]] void bad_answer(std::string_view fn, const std::string& what) {
    throw FormatError("native_l2", 0, std::string(fn) + ": " + what);
}

// An answer that is a list, a tuple ({"$t": [...]}) or a set ({"$s": [...]}): its items.
const Json& items(const Json& v) {
    if (v.is_array()) return v;
    if (v.is_object() && v.size() == 1) {
        for (const char* tag : {"$t", "$s"}) {
            const auto it = v.find(tag);
            if (it != v.end() && it->is_array()) return *it;
        }
    }
    throw FormatError("native_l2", 0, "an answer is not a list, tuple or set");
}

std::string text_of(const Json& v) {
    if (!v.is_string()) throw FormatError("native_l2", 0, "an answer is not text");
    return v.get<std::string>();
}

std::int64_t int_of(const Json& v) {
    if (!v.is_number_integer()) throw FormatError("native_l2", 0, "an answer is not an integer");
    return v.get<std::int64_t>();
}

std::set<std::string> string_set(const Json& v) {
    std::set<std::string> out;
    for (const Json& x : items(v)) out.insert(text_of(x));
    return out;
}

std::vector<std::string> string_list(const Json& v) {
    std::vector<std::string> out;
    for (const Json& x : items(v)) out.push_back(text_of(x));
    return out;
}

Json strings_json(std::span<const std::string> v) {
    Json out = Json::array();
    for (const std::string& s : v) out.push_back(s);
    return out;
}

// The text the library reads: ASCII only (every non-ASCII character escaped), as its own writer's.
std::string wire(const Json& arguments) { return arguments.dump(-1, ' ', true); }

}  // namespace

struct Library::Impl {
    CallFn call = nullptr;
    FreeFn release = nullptr;
    std::shared_ptr<DynLib> dll;   // keeps the DLL loaded as long as any handle is
    std::mutex lock;
    std::string schema;            // the schema the library was last told
};

Library::Library(CallFn call, FreeFn release) : impl_(std::make_shared<Impl>()) {
    impl_->call = call;
    impl_->release = release;
}

Library Library::load(const std::filesystem::path& dll) {
    auto lib = std::make_shared<DynLib>(DynLib::load(dll));
    const auto version = lib->function<int (*)()>("l2_version");
    if (version() != kLibraryVersion) throw UnsupportedError("native_l2", 0, dll.string() + " is another version: rebuild native/l2");
    Library out(lib->function<CallFn>("l2_call"), lib->function<FreeFn>("l2_free"));
    out.impl_->dll = std::move(lib);
    return out;
}

std::filesystem::path Library::default_dll(const std::filesystem::path& checkout, const std::filesystem::path& folder) {
    if (!folder.empty()) return folder / "cbl2.dll";
    return checkout / "native" / "l2" / "build" / "bin" / "cbl2.dll";
}

std::pair<int, std::string> Library::call_raw(std::string_view name, std::string_view arguments_json) const {
    const std::string fn(name);
    std::lock_guard hold(impl_->lock);
    char* out = nullptr;
    std::size_t length = 0;
    const int status = impl_->call(fn.c_str(), arguments_json.data(), arguments_json.size(), &out, &length);
    std::string answer = out != nullptr ? std::string(out, length) : std::string();
    if (out != nullptr) impl_->release(out);
    return {status, std::move(answer)};
}

Json Library::call(std::string_view name, const Json& arguments, Json* facts) const {
    const auto [status, text] = call_raw(name, wire(arguments));
    Json answer;
    if (!json::try_parse(text, answer)) bad_answer(name, "the library's answer is not JSON");
    if (status == 0) return answer;
    if (facts != nullptr && answer.is_object() && answer.contains("facts") && answer["facts"].is_object()) {
        if (!facts->is_object()) *facts = Json::object();
        for (const auto& [k, v] : answer["facts"].items()) (*facts)[k] = v;
    }
    const std::string type = answer.is_object() && answer.contains("type") && answer["type"].is_string() ? answer["type"].get<std::string>() : "None";
    const std::string message = answer.is_object() && answer.contains("message") && answer["message"].is_string() ? answer["message"].get<std::string>() : "";
    if (type == "Decline") throw Declined(message);
    if (type == "Cpp17Decline") throw Cpp17Declined(message);
    throw LibraryError(type, message);
}

// ---- the C back end -------------------------------------------------------------------------------------------------

std::string Library::function(const Json& doc, std::span<const std::string> moved, const std::optional<std::set<std::string>>& far,
                              Json* facts) const {
    Json far_json(nullptr);
    if (far) {
        far_json = Json::array();
        for (const std::string& name : *far) far_json.push_back(name);   // (a set: already in order)
    }
    const Json r = call("function", Json::array({doc, strings_json(moved), far_json, facts != nullptr ? *facts : Json(nullptr)}), facts);
    if (facts != nullptr && items(r).size() > 1 && items(r)[1].is_object()) {
        if (!facts->is_object()) *facts = Json::object();
        for (const auto& [k, v] : items(r)[1].items()) (*facts)[k] = v;
    }
    return text_of(items(r).at(0));
}

ModuleText Library::module(const Json& doc, std::span<const std::string> moved) const {
    const Json r = call("module", Json::array({doc, strings_json(moved)}));
    ModuleText out;
    out.text = text_of(items(r).at(0));
    for (const auto& [name, assembly] : items(r).at(1).items()) out.assembly.insert_or_assign(name, text_of(assembly));
    return out;
}

std::string Library::fragment(const Json& node) const { return text_of(call("fragment", Json::array({node}))); }

std::string Library::type_text(const Json& type, std::string_view name, bool far) const {
    return text_of(call("type_text", Json::array({type, std::string(name), far})));
}

std::string Library::expr(const Json& expression) const { return text_of(call("expr", Json::array({expression}))); }

std::string Library::condition(const Json& expression, bool inner) const {
    return text_of(call("condition", Json::array({expression, inner})));
}

std::set<std::string> Library::flags(const Json& doc, const Json& facts) const {
    return string_set(call("flags", Json::array({doc, facts})));
}

std::string Library::signature(const Json& doc) const { return text_of(call("signature", Json::array({doc}))); }

std::optional<std::int64_t> Library::takes32(std::string_view heading) const {
    const Json r = call("takes32", Json::array({std::string(heading)}));
    if (r.is_null()) return std::nullopt;
    return int_of(r);
}

ModuleNames Library::module_names32(const Json& doc) const {
    const Json r = call("module_names32", Json::array({doc}));
    ModuleNames out;
    out.names = string_set(r.at("names"));
    out.calls = string_set(r.at("calls"));
    out.called_through = string_set(r.at("called_through"));
    for (const Json& pair : items(r.at("arity"))) out.arity.emplace_back(text_of(items(pair).at(0)), int_of(items(pair).at(1)));
    return out;
}

std::set<std::string> Library::type_names(const Json& doc) const { return string_set(call("type_names", Json::array({doc}))); }

std::vector<std::string> Library::parameter_names(const Json& doc) const {
    return string_list(call("parameter_names", Json::array({doc})));
}

std::vector<StructAccess> Library::struct_accesses(const Json& doc, std::string_view param) const {
    const Json r = call("struct_accesses", Json::array({doc, std::string(param)}));
    std::vector<StructAccess> out;
    for (const Json& t : items(r)) out.push_back({text_of(items(t).at(0)), text_of(items(t).at(1)), int_of(items(t).at(2))});
    return out;
}

OrderedMap<std::string, std::string> Library::prototypes(const Json& headings, std::span<const std::string> order) const {
    const Json r = call("prototypes", Json::array({headings, strings_json(order)}));
    OrderedMap<std::string, std::string> out;
    for (const auto& [name, declaration] : r.items()) out.insert_or_assign(name, text_of(declaration));
    return out;
}

Json Library::segment_plan(const Json& functions, const Json& near, const Json& where, const Json& sites,
                           std::span<const std::int64_t> code_segments, const Json& data_segment, const Json& other_data,
                           const Json& first_selector) const {
    Json code = Json::array();
    for (std::int64_t n : code_segments) code.push_back(n);
    return call("segment_plan", Json::array({functions, near, where, sites, code, data_segment, other_data, first_selector}));
}

Json Library::caller_frame_plan(const Json& reports, const Json& frames) const {
    return call("caller_frame_plan", Json::array({reports, frames}));
}

std::optional<std::vector<std::string>> Library::seg_layout(const OrderedMap<std::int64_t, std::string>& fields) const {
    Json pairs = Json::array();
    for (const auto& [offset, type] : fields) pairs.push_back(Json::array({offset, type}));
    const Json r = call("seg_layout", Json::array({Json{{"$d", std::move(pairs)}}}));
    if (r.is_null()) return std::nullopt;
    return string_list(r);
}

ArgumentList Library::arguments_at(std::string_view text, std::int64_t open_paren) const {
    const Json r = call("arguments_at", Json::array({std::string(text), open_paren}));
    return {string_list(items(r).at(0)), int_of(items(r).at(1))};
}

std::optional<std::int64_t> Library::words(std::string_view type) const {
    const Json r = call("words", Json::array({std::string(type)}));
    if (r.is_null()) return std::nullopt;
    return int_of(r);
}

std::optional<Definition> Library::parameters(std::string_view text) const {
    const Json r = call("parameters", Json::array({std::string(text)}));
    if (r.is_null()) return std::nullopt;
    Definition out;
    out.heading = text_of(items(r).at(0));
    for (const Json& p : items(items(r).at(1))) out.parameters.emplace_back(text_of(items(p).at(0)), text_of(items(p).at(1)));
    return out;
}

std::optional<std::int64_t> Library::argument_words(std::string_view argument, const Json& types) const {
    const Json r = call("argument_words", Json::array({std::string(argument), types}));
    if (r.is_null()) return std::nullopt;
    return int_of(r);
}

// ---- the L2 core ----------------------------------------------------------------------------------------------------

std::string Library::roundtrip_text(const Json& doc) const { return text_of(call("roundtrip_text", Json::array({doc}))); }

Validation Library::validate(const Json& doc, std::string_view schema_text) const {
    bool told = false;
    {
        std::lock_guard hold(impl_->lock);
        told = impl_->schema == schema_text;
    }
    if (!told) {
        const auto [status, answer] = call_raw("set_schema", "[" + std::string(schema_text) + "]");
        if (status != 0) bad_answer("set_schema", answer);
        std::lock_guard hold(impl_->lock);
        impl_->schema = std::string(schema_text);
    }
    const Json r = call("validate", Json::array({doc}));
    const Json& pair = items(r);
    return {pair.at(0).is_boolean() ? pair.at(0).get<bool>() : !pair.at(0).is_null(), text_of(pair.at(1))};
}

}  // namespace cb::translate::native_l2
