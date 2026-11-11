// codebreaker/symbols/xdk_libraries.cpp -- typed C++ port of codebreaker.symbols.xdk_libraries (see xdk_libraries.hpp).

#include "codebreaker/symbols/xdk_libraries.hpp"

#include <algorithm>
#include <mutex>

#include "codebreaker/compare/mask.hpp"
#include "codebreaker/engine/facts.hpp"
#include "codebreaker/formats/powerpc_image.hpp"
#include "codebreaker/formats/xex.hpp"
#include "codebreaker/knowledge/reads.hpp"
#include "codebreaker/knowledge/store.hpp"
#include "codebreaker/symbols/libraries/package.hpp"
#include "py_json.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::symbols::xdk_libraries {

namespace {
using engine::facts::Json;
using engine::facts::Value;

int64_t floordiv(int64_t a, int64_t b) {
    int64_t q = a / b;
    if ((a % b != 0) && ((a < 0) != (b < 0))) --q;
    return q;
}
}  // namespace

const py::str DEFAULT_LIBS("C:\\Program Files (x86)\\Microsoft Xbox 360 SDK\\lib\\xbox");

py::str lib_folder() {
    py::str s = py::settings::get_str(py::str("xdk_libs"));
    return s.empty() ? DEFAULT_LIBS : s;
}

std::tuple<py::bytes, py::list<int64_t>> _trim(const py::bytes& code, const py::list<int64_t>& mask) {
    int64_t n = code.size() / 4;
    while (n && code.slice(4 * n - 4, 4 * n) == py::bytes(std::string(4, '\0'))) n -= 1;
    py::list<int64_t> m;
    for (int64_t i = 0; i < n && i < mask.size(); ++i) m.append(mask[i]);
    return {code.slice(0, 4 * n), m};
}

bool _falls_through(int64_t w) {
    int64_t op = w >> 26;
    if (w == 0 || op == 18) return false;                                           // padding; b / bl / ba
    if (op == 19 && (((w >> 1) & 0x3FF) == 16 || ((w >> 1) & 0x3FF) == 528) && ((w >> 21) & 0x14) == 0x14) {
        return false;                                                               // unconditional blr / bctr
    }
    if (op == 3 || (op == 31 && ((w >> 1) & 0x3FF) == 4)) return false;             // twi / tw
    return true;
}

// ---- the signatures ----
py::list<Signature> _read_signatures(const py::str& folder) {
    // dict (sig, ignore) -> entry, in the order first seen
    std::map<std::pair<std::vector<int64_t>, std::vector<int64_t>>, size_t> at;
    py::list<Signature> sigs;
    py::list<py::str> files = py::os::listdir(folder);
    for (const py::str& fname : py::sorted(files)) {
        if (!fname.lower().endswith(py::str(".lib"))) continue;
        auto [kind, found] = libraries::functions(py::os::path::join(folder, fname));
        const py::str* machine = std::get_if<py::str>(&kind);
        if (!machine || *machine != py::str("powerpc")) continue;
        for (const libraries::Function& f : found) {
            auto [code, ignore] = _trim(f.code, std::get<py::list<int64_t>>(f.mask));
            if (ignore.size() < 4) continue;
            std::vector<int64_t> words, ign;
            const std::string& raw = code.raw();
            for (int64_t i = 0; i < ignore.size(); ++i) {
                if ((i + 1) * 4 > static_cast<int64_t>(raw.size())) throw py::StructError("unpack requires a buffer of more bytes");
                uint32_t w = (static_cast<uint32_t>(static_cast<unsigned char>(raw[4 * i])) << 24) |
                             (static_cast<uint32_t>(static_cast<unsigned char>(raw[4 * i + 1])) << 16) |
                             (static_cast<uint32_t>(static_cast<unsigned char>(raw[4 * i + 2])) << 8) |
                             static_cast<uint32_t>(static_cast<unsigned char>(raw[4 * i + 3]));
                words.push_back(w);
                ign.push_back(ignore[i]);
            }
            auto key = std::make_pair(words, ign);
            auto it = at.find(key);
            if (it == at.end()) {
                Signature s;
                s.words = words;
                s.ignore = ign;
                at[key] = static_cast<size_t>(sigs.size());
                sigs.append(s);
                it = at.find(key);
            }
            Signature& entry = sigs.vec()[it->second];
            entry.names.add(f.name);
            entry.libs.add(fname);
            entry.secondary = entry.secondary || f.at != 0;
        }
    }
    return sigs;
}

namespace {
Json json_ints(const std::vector<int64_t>& xs) {
    Json j;
    j.kind = Json::List;
    for (int64_t x : xs) {
        Json i;
        i.kind = Json::Int;
        i.i = x;
        j.list.push_back(i);
    }
    return j;
}
Json json_strs(const py::set<py::str>& xs) {
    Json j;
    j.kind = Json::List;
    py::list<py::str> sorted_xs;
    for (const py::str& x : xs) sorted_xs.append(x);
    for (const py::str& x : py::sorted(sorted_xs)) {
        Json s;
        s.kind = Json::Str;
        s.s = x;
        j.list.push_back(s);
    }
    return j;
}
}  // namespace

py::list<Signature> signatures(const py::str& folder) {
    py::list<py::str> libs;
    for (const py::str& f : py::os::listdir(folder)) {
        if (f.lower().endswith(py::str(".lib"))) libs.append(f);
    }
    libs = py::sorted(libs);
    py::str repr_text = py::str("[");
    bool first = true;
    for (const py::str& f : libs) {
        py::str p = py::os::path::join(folder, f);
        if (!first) repr_text += py::str(", ");
        first = false;
        repr_text += py::percent(py::str("(%r, %d, %d)"), f, py::os::path::getsize(p), static_cast<int64_t>(py::os::path::getmtime(p)));
    }
    repr_text += py::str("]");
    py::str stamp = py::str("cpp-") + py::percent(py::str("%d-%s"), REV, py::hashlib::sha1(py::bytes(repr_text.utf8())).hexdigest().slice(0, 16));
    knowledge::reads::record(py::str("xbox360"), py::str("library_signatures"), stamp);     // (knowledge/reads)

    auto kb = knowledge::store::open();
    auto table = kb->ensure_values(
        py::str("xbox360"), py::str("library_signatures"), stamp,
        [&]() {
            py::dict<py::str, Value> out;
            Json all;
            all.kind = Json::List;
            for (const Signature& s : _read_signatures(folder)) {
                Json entry;
                entry.kind = Json::List;
                entry.list.push_back(json_ints(s.words));
                entry.list.push_back(json_ints(s.ignore));
                entry.list.push_back(json_strs(s.names));
                entry.list.push_back(json_strs(s.libs));
                Json sec;
                sec.kind = Json::Bool;
                sec.b = s.secondary;
                entry.list.push_back(sec);
                all.list.push_back(entry);
            }
            out.set(stamp, Value(all));
            return out;
        },
        py::str("the XDK's libraries in ") + folder, knowledge::store::DERIVED);
    py::list<Signature> sigs;
    const Json& all = std::get<Json>(table.at(stamp));
    for (const Json& e : all.list) {
        Signature s;
        for (const Json& w : e.list.at(0).list) s.words.push_back(w.i);
        for (const Json& w : e.list.at(1).list) s.ignore.push_back(w.i);
        for (const Json& n : e.list.at(2).list) s.names.add(n.s);
        for (const Json& n : e.list.at(3).list) s.libs.add(n.s);
        s.secondary = e.list.at(4).b;
        sigs.append(s);
    }
    return sigs;
}

py::dict<int64_t, Found> match(const py::str& image_path, const std::optional<py::str>& folder) {
    py::str path = formats::xex::is_xex(image_path) ? formats::xex::basefile(image_path) : image_path;
    py::ref<formats::powerpc_image::Image> img = formats::powerpc_image::read(path);
    std::optional<std::tuple<py::str, int64_t, int64_t, int64_t>> text;
    for (const auto& s : img->sections) {
        if (std::get<0>(s).strip(py::str(std::u32string(1, U'\0'))) == ".text") {
            text = s;
            break;
        }
    }
    if (!text) throw py::ValueError(image_path.utf8() + ": no .text section");
    int64_t t0 = std::get<1>(*text), size = std::get<2>(*text);
    py::bytes tbytes = img->bytes_at(t0, size);
    int64_t nwords = tbytes.size() / 4;
    std::vector<int64_t> words;
    for (int64_t i = 0; i < nwords; ++i) {
        const std::string& raw = tbytes.raw();
        uint32_t w = (static_cast<uint32_t>(static_cast<unsigned char>(raw[4 * i])) << 24) |
                     (static_cast<uint32_t>(static_cast<unsigned char>(raw[4 * i + 1])) << 16) |
                     (static_cast<uint32_t>(static_cast<unsigned char>(raw[4 * i + 2])) << 8) |
                     static_cast<uint32_t>(static_cast<unsigned char>(raw[4 * i + 3]));
        words.push_back(w);
    }
    libraries::Index index{py::list<int64_t>(words)};
    py::set<int64_t> starts;
    for (int64_t a : img->extents.keys()) starts.add(floordiv(a - t0, 4));
    std::map<int64_t, Found> found;
    std::map<int64_t, std::pair<py::set<py::str>, py::set<py::str>>> sets;
    py::list<Signature> sigs = signatures(folder ? *folder : lib_folder());
    for (const Signature& s : sigs) {
        int64_t whole = 0;
        for (int64_t m : s.ignore) whole += m ? 0 : 1;
        if (whole < 3) continue;                                    // too generic to place
        std::vector<int64_t> hits;
        for (int64_t h : index.find(py::list<int64_t>(s.words), py::list<int64_t>(s.ignore))) {            // (not the tail of another function)
            if (s.secondary || starts.contains(h) || h == 0 || !_falls_through(words[static_cast<size_t>(h - 1)])) hits.push_back(h);
        }
        if (hits.size() == 1) {
            int64_t a = t0 + hits[0] * 4;
            Found& f = found[a];
            auto& st = sets[a];
            for (const py::str& n : s.names) st.first.add(n);
            for (const py::str& n : s.libs) st.second.add(n);
            f.size = std::max<int64_t>(f.size, static_cast<int64_t>(s.words.size()) * 4);
        }
    }
    py::dict<int64_t, Found> out;
    for (auto& kv : found) {
        auto& st = sets[kv.first];
        py::list<py::str> names, libs;
        for (const py::str& n : st.first) names.append(n);
        for (const py::str& n : st.second) libs.append(n);
        kv.second.names = py::sorted(names);
        kv.second.libs = py::sorted(libs);
        out.set(kv.first, kv.second);
    }
    return out;
}

const py::str USAGE(
    "xdk_libraries.py -- naming the XDK library functions an Xbox 360 program was statically linked with.\n\n"
    "    python -m codebreaker xdk-match <program.xex or image> [<XDK lib folder>] [-o names.json]\n"
    "    from codebreaker.symbols import xdk_libraries\n"
    "    found = xdk_libraries.match(image_path, lib_folder)     # {address: {\"names\", \"libs\", \"size\"}}");

namespace {
// json.dump({"%08X" % a: v}, f, indent=1)
void dump_indent1(std::string& o, const py::dict<int64_t, Found>& found) {
    if (found.empty()) {
        o += "{}";
        return;
    }
    o += "{";
    bool first = true;
    auto strs = [&](const py::list<py::str>& xs) {
        if (xs.empty()) {
            o += "[]";
            return;
        }
        o += "[";
        bool f2 = true;
        for (const py::str& x : xs) {
            o += f2 ? "\n   " : ",\n   ";
            f2 = false;
            py::json::put(o, x);
        }
        o += "\n  ]";
    };
    for (const auto& kv : found.items()) {
        o += first ? "\n " : ",\n ";
        first = false;
        char key[32];
        std::snprintf(key, sizeof key, "\"%08llX\"", static_cast<unsigned long long>(std::get<0>(kv)));
        o += key;
        o += ": {\n  \"names\": ";
        strs(std::get<1>(kv).names);
        o += ",\n  \"libs\": ";
        strs(std::get<1>(kv).libs);
        o += ",\n  \"size\": " + std::to_string(std::get<1>(kv).size) + "\n }";
    }
    o += "\n}";
}
}  // namespace

int64_t main(const py::list<py::str>& argv) {
    if (argv.empty() || argv[0].startswith(py::str("-"))) {
        py::list<py::str> paras = py::split(USAGE, py::str("\n\n"));
        py::print(paras[0] + py::str("\n\n") + paras[1]);
        return 2;
    }
    std::optional<py::str> out;
    int64_t at = -1;
    for (int64_t i = 0; i < argv.size(); ++i) {
        if (argv[i] == "-o") {
            at = i;
            break;
        }
    }
    if (at >= 0) {
        if (at + 1 >= argv.size()) throw py::IndexError("list index out of range");
        out = argv[at + 1];
    }
    py::list<py::str> rest;
    for (int64_t i = 0; i < argv.size(); ++i) {
        if (argv[i] != "-o" && (i == 0 || argv[i - 1] != "-o")) rest.append(argv[i]);
    }
    py::str folder = rest.size() > 1 ? rest[1] : lib_folder();
    if (!py::os::path::isdir(folder)) {
        py::print(py::str("no XDK library folder: ") + folder + py::str(" (setting xdk_libs)"));
        return 1;
    }
    py::dict<int64_t, Found> found = match(rest[0], folder);
    int64_t text_size = 0;
    std::map<py::str, int64_t> per_lib;
    for (const auto& kv : found.items()) {
        text_size += std::get<1>(kv).size;
        per_lib[std::get<1>(kv).libs[0]] += 1;
    }
    // format(text_size, ",")
    std::string digits = std::to_string(text_size), grouped;
    for (size_t i = 0; i < digits.size(); ++i) {
        if (i && (digits.size() - i) % 3 == 0) grouped += ',';
        grouped += digits[i];
    }
    py::print(py::percent(py::str("%s: %d library functions named (%s bytes)"), py::os::path::basename(rest[0]),
                          found.size(), py::str(grouped)));
    std::vector<std::pair<py::str, int64_t>> order;       // sorted(per_lib.items(), key=-n): dict order = first seen
    {
        py::list<py::str> seen;
        for (const auto& kv : found.items()) {
            const py::str& lib0 = std::get<1>(kv).libs[0];
            bool have = false;
            for (const py::str& s : seen) have = have || s == lib0;
            if (!have) seen.append(lib0);
        }
        for (const py::str& l : seen) order.emplace_back(l, per_lib[l]);
    }
    std::stable_sort(order.begin(), order.end(), [](const auto& a, const auto& b) { return -a.second < -b.second; });
    for (size_t i = 0; i < order.size() && i < 15; ++i) {
        py::print(py::percent(py::str("  %6d  %s"), order[i].second, order[i].first));
    }
    if (out) {
        std::string text;
        dump_indent1(text, found);
        auto f = py::open(*out, py::str("w"), py::str("utf-8"));
        f.write(py::str(text));
        f.close();
        py::print(py::str("  -> ") + *out);
    }
    return 0;
}

}  // namespace codebreaker::symbols::xdk_libraries
