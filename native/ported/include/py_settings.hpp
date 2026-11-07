// py_settings.hpp -- codebreaker.base.settings.get for the typed C++: a setting that is a str.
//
// Typed code asks settings::get_str("formats_bin") where the Python asks settings.get("formats_bin") and
// the setting is a str (a path, a name: pyengine types the call so only for those). Two ways to answer:
//
//   in codebreaker.exe   the hook the program sets (its settings module's own get, looked up when asked),
//                        so typed and Python code read one setting, a test's patch of settings.get included
//   standalone           (a judge's driver) settings.py's get itself: CB_<KEY>, else settings.json's value
//                        over the defaults, a path key's relative value joined to the checkout
//
// The checkout is CB_CHECKOUT_ROOT (what the driver is given); a setting that is not a str is Python's
// TypeError, as os.path.isabs raises for a path key (and a str-typed caller could not take it).
#pragma once

#include <cctype>
#include <fstream>
#include <functional>
#include <map>
#include <optional>
#include <sstream>
#include <string>

#include "py.hpp"

namespace py {
namespace settings {

inline std::function<str(const str&)>& hook() {
    static std::function<str(const str&)> h;
    return h;
}

// the defaults of settings.py (its DEFAULTS), and the keys that are paths (its _PATHS)
inline const std::map<std::string, std::string>& defaults() {
    static const std::map<std::string, std::string> d = {
        {"corpus", "..\\Windows95\\design\\corpus\\files"}, {"openwatcom", ""},
        {"decompiler_bin", "native\\decompiler\\build\\bin"}, {"xex_bin", "native\\xex\\build\\bin"},
        {"tracer_bin", "native\\tracer\\build\\bin"}, {"formats_bin", "native\\formats\\build\\bin"},
        {"engine_bin", "native\\engine\\build\\bin"}, {"dosexec_bin", "native\\dosexec\\build\\bin"},
        {"symbols_bin", "native\\symbols\\build\\bin"}, {"invert_bin", "native\\invert\\build\\bin"},
        {"analyze_bin", "native\\analyze\\build\\bin"}, {"ppc_binutils", "toolchains\\binutils-ppc"},
        {"converted", "Converted"}, {"nasm", "native\\nasm\\build\\bin\\nasm.exe"},
        {"retromcp", "native\\retromcp\\build\\libs\\retromcp.jar"}, {"state", "state"}, {"target", "win95"},
        {"threads", ""}, {"struct_recovery", "all"}};
    return d;
}
inline bool is_path_key(const std::string& k) {
    static const char* const P[] = {"corpus", "openwatcom", "build_cache", "decompiler_bin", "xex_bin", "tracer_bin", "formats_bin",
                                    "engine_bin", "dosexec_bin", "symbols_bin", "invert_bin", "analyze_bin", "state", "powerpc_tools",
                                    "powerpc_lib", "period_compiler", "dosbox_x", "msc6", "xenix_c", "masm", "nasm", "msvc152", "java",
                                    "retromcp", "xbox360_sdk", "host_cxx", "msbuild", "java_overlays", "java_libs", "visual_studio",
                                    "ppc_binutils", "converted", "xbox360_keys", "xbox360_crypto_bin", "ilspycmd", "xna_overlays",
                                    "win95_installed_hd"};
    for (auto* p : P)
        if (k == p) return true;
    return false;
}

// settings.json's top-level entries: a str value as its text, any other value as nothing (std::nullopt)
struct JsonTop {
    std::map<std::string, std::optional<std::string>> values;
};
inline void json_ws_(const std::string& s, size_t& i) {
    while (i < s.size() && std::isspace(static_cast<unsigned char>(s[i]))) i++;
}
inline std::string json_str_(const std::string& s, size_t& i) {   // at the opening quote
    std::string out;
    i++;
    while (i < s.size() && s[i] != '"') {
        char c = s[i++];
        if (c != '\\') {
            out += c;
            continue;
        }
        char e = s[i++];
        switch (e) {
            case 'n': out += '\n'; break;
            case 't': out += '\t'; break;
            case 'r': out += '\r'; break;
            case 'b': out += '\b'; break;
            case 'f': out += '\f'; break;
            case 'u': {
                unsigned v = std::stoul(s.substr(i, 4), nullptr, 16);
                i += 4;
                if (v >= 0xD800 && v < 0xDC00 && s.compare(i, 2, "\\u") == 0) {
                    unsigned lo = std::stoul(s.substr(i + 2, 4), nullptr, 16);
                    i += 6;
                    v = 0x10000 + ((v - 0xD800) << 10) + (lo - 0xDC00);
                }
                out += str(std::u32string(1, static_cast<char32_t>(v))).utf8();
                break;
            }
            default: out += e;
        }
    }
    i++;
    return out;
}
inline void json_skip_(const std::string& s, size_t& i) {   // any value
    json_ws_(s, i);
    if (i >= s.size()) return;
    if (s[i] == '"') {
        json_str_(s, i);
        return;
    }
    if (s[i] == '{' || s[i] == '[') {
        int depth = 0;
        do {
            if (s[i] == '"') {
                json_str_(s, i);
                continue;
            }
            if (s[i] == '{' || s[i] == '[') depth++;
            if (s[i] == '}' || s[i] == ']') depth--;
            i++;
        } while (i < s.size() && depth > 0);
        return;
    }
    while (i < s.size() && s[i] != ',' && s[i] != '}' && s[i] != ']') i++;
}
inline JsonTop json_top_(const std::string& s) {
    JsonTop t;
    size_t i = 0;
    json_ws_(s, i);
    if (i >= s.size() || s[i] != '{') throw ValueError("settings.json: not a JSON object");
    i++;
    for (;;) {
        json_ws_(s, i);
        if (i < s.size() && s[i] == '}') break;
        if (i >= s.size() || s[i] != '"') throw ValueError("settings.json: a key expected");
        std::string k = json_str_(s, i);
        json_ws_(s, i);
        if (i >= s.size() || s[i] != ':') throw ValueError("settings.json: ':' expected");
        i++;
        json_ws_(s, i);
        if (i < s.size() && s[i] == '"') t.values[k] = json_str_(s, i);
        else {
            json_skip_(s, i);
            t.values[k] = std::nullopt;
        }
        json_ws_(s, i);
        if (i < s.size() && s[i] == ',') i++;
    }
    return t;
}

inline str checkout_root() {
    auto r = os::environ_::get(str("CB_CHECKOUT_ROOT"));
    if (!r) throw RuntimeError("settings: CB_CHECKOUT_ROOT names no checkout (the typed code runs outside codebreaker.exe)");
    return *r;
}
// settings.json, read once (settings.py's _load)
inline const JsonTop& loaded_() {
    static const JsonTop t = [] {
        std::ifstream f(std::filesystem::u8path(os::path::join(checkout_root(), str("settings.json")).utf8()), std::ios::binary);
        if (!f) return JsonTop{};
        std::stringstream ss;
        ss << f.rdbuf();
        return json_top_(ss.str());
    }();
    return t;
}

inline str get_str(const str& key) {
    if (hook()) return hook()(key);
    std::string k = key.utf8();
    std::string upper = k;
    for (auto& c : upper) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    auto env = os::environ_::get(str("CB_" + upper));
    std::optional<str> value;
    if (env && !env->empty()) value = *env;
    else {
        const auto& j = loaded_().values;
        auto it = j.find(k);
        if (it != j.end()) {
            if (!it->second) throw TypeError("settings.get(" + repr(key).utf8() + "): the setting is not a str");
            value = str(*it->second);
        } else {
            auto d = defaults().find(k);
            value = str(d != defaults().end() ? d->second : std::string());
        }
    }
    if (!value->empty() && is_path_key(k) && !os::path::isabs(*value)) value = os::path::normpath(os::path::join(checkout_root(), *value));
    return *value;
}

}  // namespace settings
}  // namespace py
