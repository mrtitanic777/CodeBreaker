// cb/port/pre_ansi_types.cpp -- see pre_ansi_types.hpp.

#include "cb/port/pre_ansi_types.hpp"

#include <algorithm>

#include <nlohmann/json.hpp>

#include "cb/core/json.hpp"
#include "cb/core/text.hpp"

namespace cb::port::pre_ansi {

namespace {

std::vector<std::string> strings_of(const Json& doc, const char* key) {
    std::vector<std::string> out;
    if (const auto it = doc.find(key); it != doc.end() && it->is_array())
        for (const Json& x : *it)
            if (x.is_string()) out.push_back(x.get<std::string>());
    return out;
}

std::optional<unsigned> unsigned_of(const Json& doc, const char* key) {
    if (const auto it = doc.find(key); it != doc.end() && it->is_number_integer() && it->get<std::int64_t>() >= 0)
        return static_cast<unsigned>(it->get<std::int64_t>());
    return std::nullopt;
}

}  // namespace

Profile Profile::load(const std::string& path) {
    const Json doc = json::load(path);
    Profile p;
    if (const auto it = doc.find("name"); it != doc.end() && it->is_string()) p.name = it->get<std::string>();
    p.libc = strings_of(doc, "libc");
    p.runtime_defines = strings_of(doc, "runtime_defines");
    p.runtime_headers = strings_of(doc, "runtime_headers");
    p.runtime_internals = strings_of(doc, "runtime_internals");
    if (const auto it = doc.find("platform"); it != doc.end() && it->is_object()) {
        if (const auto h = it->find("header"); h != it->end() && h->is_string()) p.platform_header = h->get<std::string>();
        p.platform_calls = strings_of(*it, "calls");
        if (const auto r = it->find("renames"); r != it->end() && r->is_object())
            for (auto kv = r->begin(); kv != r->end(); ++kv)
                if (kv.value().is_string()) p.platform_renames[kv.key()] = kv.value().get<std::string>();
    }
    if (const auto it = doc.find("widths"); it != doc.end() && it->is_object()) p.widths_pointer = unsigned_of(*it, "pointer");
    p.int_storage = unsigned_of(doc, "int_storage");
    if (const auto it = doc.find("printf_hex_upper"); it != doc.end() && it->is_boolean()) p.printf_hex_upper = it->get<bool>();
    p.struct_align = unsigned_of(doc, "struct_align");
    if (const auto it = doc.find("keywords_removed"); it != doc.end() && it->is_array()) p.keywords_removed = strings_of(doc, "keywords_removed");
    if (const auto it = doc.find("arg_order"); it != doc.end() && it->is_string()) p.arg_order = it->get<std::string>();
    return p;
}

std::string SplitDecl::operator()(const RegexMatch& m) const {
    return (*this)(m.group(1), m.group(2), m.group(3), m.group(4));
}

std::string SplitDecl::operator()(std::string_view lead_text, std::string_view base, std::string_view before, std::string_view after) const {
    // the names of the list that stay: the ones before the retyped name and the ones after it
    std::string joined = std::string(before) + std::string(after);
    std::replace(joined.begin(), joined.end(), ',', ' ');
    std::vector<std::string> rest;
    for (std::string_view w : text::split_ws(joined)) rest.emplace_back(w);
    const std::string lead(lead_text);
    std::string out = rest.empty() ? lead : lead + std::string(base) + " " + text::join(rest, ", ") + "; ";
    std::string storage;   // its storage class: so (extern int a, v, b;  -> extern for the new declaration too)
    if (!rest.empty()) {
        for (std::string_view w : text::split_ws(lead)) {
            if (w == "extern" || w == "static") storage += (storage.empty() ? "" : " ") + std::string(w);
        }
    }
    return out + (storage.empty() ? "" : storage + " ") + type + (type.ends_with('*') ? "" : " ") + name + ";";
}

DeclRule::DeclRule(std::string pattern, std::string replacement)
    : regex_(std::make_shared<const Regex>(pattern)), replacement_(std::move(replacement)) {}

DeclRule::DeclRule(std::string pattern, SplitDecl split)
    : regex_(std::make_shared<const Regex>(pattern)), split_(std::move(split)) {}

std::string DeclRule::apply(std::string_view text) const {
    if (split_) {
        const SplitDecl& s = *split_;
        return regex_->replace_all(text, [&s](const RegexMatch& m) { return s(m); });
    }
    return regex_->replace_all(text, replacement_);
}

}  // namespace cb::port::pre_ansi
