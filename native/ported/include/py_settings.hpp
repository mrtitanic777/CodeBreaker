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
// get / get_int / get_float / get_bool / get_opt_* give a setting of any scalar kind (settings.json holds
// workers and min_free_gb as numbers). The root is CB_CHECKOUT_ROOT (what the driver is given); a setting that is not a str is Python's
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

// A setting as settings.get hands it back: a str (a path, a name), or what settings.json holds as a number,
// true / false or null. (std::variant: the one place a setting's type is Python's to decide; a list or an
// object in settings.json is not representable and is a TypeError.)
using Value = std::variant<str, int64_t, double, bool, std::monostate>;

// what the hook throws for a setting that is not a str: a TypeError (what get_str's callers meet) that
// carries the value, "<kind>:<str(value)>" with kind i, f, b or n (int, float, bool, None)
struct NotStr : TypeError {
    std::string tagged;
    NotStr(const std::string& m, const std::string& t) : TypeError(m), tagged(t) {}
};

// settings.json's top-level entries; nullopt for a value that is a list or an object
struct JsonTop {
    std::map<std::string, std::optional<Value>> values;
    std::map<std::string, std::string> others;   // the key of a list or an object -> "list" / "dict", "" in front when empty
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
// a JSON number, true, false or null as Python's json.load reads it (nullopt: a list or an object)
inline std::optional<Value> scalar_(const std::string& raw) {
    if (raw == "true") return Value(true);
    if (raw == "false") return Value(false);
    if (raw == "null") return Value(std::monostate{});
    if (raw == "NaN") return Value(std::nan(""));
    if (raw == "Infinity") return Value(HUGE_VAL);
    if (raw == "-Infinity") return Value(-HUGE_VAL);
    if (raw.empty() || !(raw[0] == '-' || (raw[0] >= '0' && raw[0] <= '9'))) return std::nullopt;
    if (raw.find_first_of(".eE") != std::string::npos) return Value(float_from_str(str(raw)));
    return Value(int_parse(str(raw)));
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
        if (i < s.size() && s[i] == '"') {
            t.values[k] = Value(str(json_str_(s, i)));
        } else {
            size_t start = i;
            json_skip_(s, i);
            std::string raw = s.substr(start, i - start);
            while (!raw.empty() && std::isspace(static_cast<unsigned char>(raw.back()))) raw.pop_back();
            std::optional<Value> v = scalar_(raw);
            t.values[k] = v;
            if (!v) {
                std::string squeezed;
                for (char c : raw)
                    if (!std::isspace(static_cast<unsigned char>(c))) squeezed += c;
                t.others[k] = std::string(squeezed == "[]" || squeezed == "{}" ? "empty-" : "") + (raw[0] == '[' ? "list" : "dict");
            } else {
                t.others.erase(k);
            }
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
// settings.json, read once (settings.py's _load); reload() forgets it
inline std::optional<JsonTop>& loaded_cache_() {
    static std::optional<JsonTop> t;
    return t;
}
inline void reload() { loaded_cache_().reset(); }
inline const JsonTop& loaded_() {
    auto& c = loaded_cache_();
    if (!c) {
        std::ifstream f(std::filesystem::u8path(os::path::join(checkout_root(), str("settings.json")).utf8()), std::ios::binary);
        if (!f) {
            c = JsonTop{};
        } else {
            std::stringstream ss;
            ss << f.rdbuf();
            c = json_top_(ss.str());
        }
    }
    return *c;
}

inline Value decode_tagged_(const std::string& t) {
    std::string text = t.size() > 2 ? t.substr(2) : std::string();
    switch (t.empty() ? 'n' : t[0]) {
        case 'i': return Value(int_parse(str(text)));
        case 'f': return Value(float_from_str(str(text)));
        case 'b': return Value(text == "True");
        default: return Value(std::monostate{});
    }
}
inline const char* type_name_(const Value& v) {
    switch (v.index()) {
        case 0: return "str";
        case 1: return "int";
        case 2: return "float";
        case 3: return "bool";
        default: return "NoneType";
    }
}
inline bool truthy_(const Value& v) {
    switch (v.index()) {
        case 0: return !std::get<str>(v).empty();
        case 1: return std::get<int64_t>(v) != 0;
        case 2: return std::get<double>(v) != 0.0;
        case 3: return std::get<bool>(v);
        default: return false;
    }
}

// settings.get(key): CB_<KEY> if set and not empty, else settings.json's value over the defaults ("" when
// there is none); a path key's relative text is joined to the root (a path key holding a non-empty
// non-str is the TypeError os.path.isabs raises)
inline Value get(const str& key) {
    if (hook()) {
        try {
            return Value(hook()(key));
        } catch (const NotStr& e) {
            return decode_tagged_(e.tagged);
        }
    }
    std::string k = key.utf8();
    std::string upper = k;
    for (auto& c : upper) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    auto env = os::environ_::get(str("CB_" + upper));
    Value value{str("")};
    if (env && !env->empty()) {
        value = *env;
    } else {
        const auto& j = loaded_().values;
        auto it = j.find(k);
        if (it != j.end()) {
            if (!it->second) {
                const std::string& what = loaded_().others.at(k);
                if (what.rfind("empty-", 0) != 0 && is_path_key(k))
                    throw TypeError("expected str, bytes or os.PathLike object, not " + what);
                throw TypeError("settings.get(" + repr(key).utf8() + "): the setting is a list or an object");
            }
            value = *it->second;
        } else {
            auto d = defaults().find(k);
            value = str(d != defaults().end() ? d->second : std::string());
        }
    }
    if (truthy_(value) && is_path_key(k)) {
        if (value.index() != 0) throw TypeError(std::string("expected str, bytes or os.PathLike object, not ") + type_name_(value));
        const str v = std::get<str>(value);
        if (!os::path::isabs(v)) value = os::path::normpath(os::path::join(checkout_root(), v));
    }
    return value;
}

inline str get_str(const str& key) {
    Value v = get(key);
    if (v.index() != 0) throw TypeError("settings.get(" + repr(key).utf8() + "): the setting is not a str");
    return std::get<str>(v);
}
// the setting as an int (a bool is not one here), a float (an int widens), a bool; TypeError for any other kind
[[noreturn]] inline void not_a_(const str& key, const char* what) {
    throw TypeError("settings.get(" + repr(key).utf8() + "): the setting is not " + what);
}
inline int64_t get_int(const str& key) {
    Value v = get(key);
    if (v.index() != 1) not_a_(key, "an int");
    return std::get<int64_t>(v);
}
inline double get_float(const str& key) {
    Value v = get(key);
    if (v.index() == 2) return std::get<double>(v);
    if (v.index() == 1) return static_cast<double>(std::get<int64_t>(v));
    not_a_(key, "a float");
}
inline bool get_bool(const str& key) {
    Value v = get(key);
    if (v.index() != 3) not_a_(key, "a bool");
    return std::get<bool>(v);
}
// ... or None (null in settings.json) as nothing
inline std::optional<int64_t> get_opt_int(const str& key) {
    Value v = get(key);
    if (v.index() == 4) return std::nullopt;
    if (v.index() != 1) not_a_(key, "an int or None");
    return std::get<int64_t>(v);
}
inline std::optional<double> get_opt_float(const str& key) {
    Value v = get(key);
    if (v.index() == 4) return std::nullopt;
    if (v.index() == 2) return std::get<double>(v);
    if (v.index() == 1) return static_cast<double>(std::get<int64_t>(v));
    not_a_(key, "a float or None");
}
inline std::optional<bool> get_opt_bool(const str& key) {
    Value v = get(key);
    if (v.index() == 4) return std::nullopt;
    if (v.index() != 3) not_a_(key, "a bool or None");
    return std::get<bool>(v);
}
inline std::optional<str> get_opt_str(const str& key) {
    Value v = get(key);
    if (v.index() == 4) return std::nullopt;
    if (v.index() != 0) not_a_(key, "a str or None");
    return std::get<str>(v);
}

}  // namespace settings
}  // namespace py
