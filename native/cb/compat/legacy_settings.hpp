// cb/compat/legacy_settings.hpp -- what the shims of cb/base need from the old layer's settings (docs/CPP_DESIGN.md 6.3).
//
// The new modules take what they need as plain values (a state folder, a worker policy, the products of the corpus);
// nothing in them reads settings. The old layer reads them from settings.json through py::settings, which answers a
// setting of any scalar kind. These functions turn that into the new modules' inputs. COMPAT(py): deleted with the
// last shim, when the application's configuration (cb::Config) hands the same values down.
#pragma once

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <optional>
#include <sstream>
#include <string>
#include <variant>

#include "cb/base/corpus.hpp"
#include "cb/base/parallel.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"

// py_settings.hpp / py_os.hpp are the old layer's own (included through py.hpp's directory)
#include "py_os.hpp"
#include "py_settings.hpp"

namespace cb::compat::legacy {

// A path in the old layer's text (native separators), from a new path.
inline py::str native(const std::filesystem::path& p) {
    std::string s = fs::to_text(p);
    for (char& c : s)
        if (c == '/') c = '\\';
    return to_py(s);
}

inline std::filesystem::path path_of(const py::str& s) { return fs::from_text(s.utf8()); }

// settings "state"
inline std::filesystem::path state_dir() { return path_of(py::settings::get_str(py::str("state"))); }

// A setting as the Python's int(): a number, or text that is one, else 0.
inline std::int64_t setting_int(const char* key) {
    const py::settings::Value v = py::settings::get(py::str(key));
    if (const auto* i = std::get_if<std::int64_t>(&v)) return *i;
    if (const auto* d = std::get_if<double>(&v)) return static_cast<std::int64_t>(*d);
    if (const auto* b = std::get_if<bool>(&v)) return *b ? 1 : 0;
    if (const auto* s = std::get_if<py::str>(&v)) return text::parse_int<std::int64_t>(text::trim(s->utf8())).value_or(0);
    return 0;
}

// A setting as a truth value the way "low_load" is read: empty, 0, false, no, off and none are false.
inline bool setting_truthy(const char* key) {
    const py::settings::Value v = py::settings::get(py::str(key));
    if (const auto* i = std::get_if<std::int64_t>(&v)) return *i != 0;
    if (const auto* d = std::get_if<double>(&v)) return *d != 0;
    if (const auto* b = std::get_if<bool>(&v)) return *b;
    if (const auto* s = std::get_if<py::str>(&v)) {
        const std::string t = text::lower(text::trim(s->utf8()));
        return !(t.empty() || t == "0" || t == "false" || t == "no" || t == "off" || t == "none");
    }
    return false;
}

// settings "workers" and "low_load" (CB_JOBS and CB_LOW_LOAD are read by cb::parallel::workers itself)
inline cb::parallel::WorkerPolicy worker_policy() {
    cb::parallel::WorkerPolicy p;
    p.workers = static_cast<int>(std::clamp<std::int64_t>(setting_int("workers"), 0, 4096));
    p.low_load = setting_truthy("low_load");
    return p;
}

// settings "workers", "low_load", "min_free_gb" and the slot folder <state>/slots
inline cb::base::budget::Limits budget_limits() {
    cb::base::budget::Limits l;
    l.workers = worker_policy();
    l.min_free_gb = 6.0;
    const py::settings::Value v = py::settings::get(py::str("min_free_gb"));
    if (const auto* i = std::get_if<std::int64_t>(&v)) l.min_free_gb = static_cast<double>(*i);
    if (const auto* d = std::get_if<double>(&v)) l.min_free_gb = *d;
    if (const auto* b = std::get_if<bool>(&v)) l.min_free_gb = *b ? 1.0 : 0.0;
    if (const auto* s = std::get_if<py::str>(&v)) {
        const std::string t(text::trim(s->utf8()));
        char* end = nullptr;
        const double parsed = std::strtod(t.c_str(), &end);
        if (!t.empty() && end == t.c_str() + t.size()) l.min_free_gb = parsed;
    }
    l.slot_dir = state_dir() / "slots";
    return l;
}

// settings.json's "corpora": {name: folder as written in settings.json} (read once, as the old corpus port did)
inline const std::map<std::string, std::string>& listed_corpora() {
    static std::once_flag once;
    static std::map<std::string, std::string> out;
    std::call_once(once, [] {
        std::ifstream f(fs::from_text(py::settings::checkout_root().utf8()) / "settings.json", std::ios::binary);
        if (!f) return;   // a missing settings.json: no corpora
        std::stringstream ss;
        ss << f.rdbuf();
        const std::string s = ss.str();
        std::size_t i = 0;
        py::settings::json_ws_(s, i);
        if (i >= s.size() || s[i] != '{') throw py::ValueError("settings.json: not a JSON object");
        ++i;
        for (;;) {
            py::settings::json_ws_(s, i);
            if (i < s.size() && s[i] == '}') break;
            if (i >= s.size() || s[i] != '"') throw py::ValueError("settings.json: a key expected");
            const std::string key = py::settings::json_str_(s, i);
            py::settings::json_ws_(s, i);
            if (i >= s.size() || s[i] != ':') throw py::ValueError("settings.json: ':' expected");
            ++i;
            py::settings::json_ws_(s, i);
            if (key == "corpora" && i < s.size() && s[i] == '{') {
                ++i;
                for (;;) {
                    py::settings::json_ws_(s, i);
                    if (i < s.size() && s[i] == '}') {
                        ++i;
                        break;
                    }
                    if (i >= s.size() || s[i] != '"') throw py::ValueError("settings.json: a key expected");
                    const std::string name = py::settings::json_str_(s, i);
                    py::settings::json_ws_(s, i);
                    if (i >= s.size() || s[i] != ':') throw py::ValueError("settings.json: ':' expected");
                    ++i;
                    py::settings::json_ws_(s, i);
                    if (i < s.size() && s[i] == '"') out[name] = py::settings::json_str_(s, i);
                    else py::settings::json_skip_(s, i);   // (a folder that is not a str: nothing to look in)
                    py::settings::json_ws_(s, i);
                    if (i < s.size() && s[i] == ',') ++i;
                }
            } else if (i < s.size() && s[i] == '"') {
                py::settings::json_str_(s, i);
            } else {
                py::settings::json_skip_(s, i);
            }
            py::settings::json_ws_(s, i);
            if (i < s.size() && s[i] == ',') ++i;
        }
    });
    return out;
}

// The products of the corpus as settings.json names them: "target", "corpus", "corpora" (relative folders are made
// absolute against the checkout), "state", "reference".
inline cb::base::Products corpus_products() {
    cb::base::Products p;
    p.checkout_root = path_of(py::settings::checkout_root());
    for (const auto& [name, folder] : listed_corpora()) {
        std::filesystem::path f = fs::from_text(folder);
        p.listed[name] = f.is_absolute() ? f : (p.checkout_root / f).lexically_normal();
    }
    p.target = py::settings::get_str(py::str("target")).utf8();
    p.target_folder = path_of(py::settings::get_str(py::str("corpus")));
    p.state = state_dir();
    const std::string reference = py::settings::get_str(py::str("reference")).utf8();
    if (!reference.empty()) p.reference = fs::from_text(reference);
    return p;
}

}  // namespace cb::compat::legacy
