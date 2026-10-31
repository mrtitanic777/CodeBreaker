// facts.cpp -- the gate's C++ side for the ported NE reader (codebreaker/formats/ne.py).
//
//   ne_facts <file listing paths>     one JSON line per path: what the reader says about the file
//
// gate.py runs the Python reader over the same files and compares every field. A method the
// front end refused (left in Python) is not in the port; the facts list which were computed
// ("_have"), so the comparison covers exactly the translated part.
#include <chrono>
#include <cstdio>
#include <fstream>
#include <string>
#include <type_traits>

#include "codebreaker/formats/ne.hpp"
#include "py_json.hpp"

using namespace codebreaker::formats::ne;
using py::json::attempt;
using py::json::put;

// which methods the port has (a refused one is not declared)
#define HAS(m)                                                                                            \
    template <class T, class = void> struct has_##m : std::false_type {};                                 \
    template <class T> struct has_##m<T, std::void_t<decltype(std::declval<T&>().m())>> : std::true_type {};
HAS(resident_names)
HAS(nonresident_names)
HAS(module_name)
HAS(module_refs)
HAS(entries)
HAS(export_ordinals)
HAS(segments)
HAS(resources)
template <class T, class S, class = void> struct has_relocations : std::false_type {};
template <class T, class S>
struct has_relocations<T, S, std::void_t<decltype(std::declval<T&>().relocations(std::declval<S>()))>> : std::true_type {};
template <class T, class = void> struct has_imported_name : std::false_type {};
template <class T> struct has_imported_name<T, std::void_t<decltype(std::declval<T&>().imported_name(0))>> : std::true_type {};

// (a template, so that what the port does not have is never compiled: `if constexpr` on N)
template <class N> static std::string facts_of(const std::string& path) {
    std::string o = "{\"path\": ";
    put(o, py::str(path));
    std::string have;
    py::ref<N> f;
    try {
        f = std::make_shared<N>(py::str(path));
    } catch (const py::Exception& e) {
        return o + ", \"error\": \"" + e.type() + "\"}";
    }
    auto fact = [&](const char* name, auto&& fn) {
        have += std::string(have.empty() ? "" : ", ") + "\"" + name + "\"";
        o += std::string(", \"") + name + "\": " + attempt(fn);
    };
    fact("ne_off", [&](std::string& v) { put(v, f->ne_off); });
    fact("h", [&](std::string& v) { put(v, f->h); });
#define FACT(m) \
    if constexpr (has_##m<N>::value) fact(#m, [&](std::string& v) { put(v, f->m()); });
    FACT(resident_names)
    FACT(nonresident_names)
    FACT(module_name)
    FACT(module_refs)
    FACT(entries)
    FACT(export_ordinals)
    FACT(resources)
    if constexpr (has_segments<N>::value) {
        fact("segments", [&](std::string& v) { put(v, f->segments()); });
        using Seg = std::decay_t<decltype(f->segments()[0])>;
        if constexpr (has_relocations<N, Seg>::value) {
            fact("relocations", [&](std::string& v) {
                v += '[';
                bool first = true;
                for (const auto& s : f->segments()) {
                    v += first ? "" : ", ";
                    v += attempt([&](std::string& one) { put(one, f->relocations(s)); });
                    first = false;
                }
                v += ']';
            });
        }
    }
    if constexpr (has_imported_name<N>::value) {
        fact("imported_names", [&](std::string& v) {  // the module reference table's names, by offset
            v += '[';
            for (int64_t i = 0; i < f->h.n_modrefs; ++i) {
                v += i ? ", " : "";
                put(v, f->imported_name(f->u16(f->ne_off + f->h.mod_tab + i * 2)));
            }
            v += ']';
        });
    }
    fact("embedded", [&](std::string& v) { put(v, embedded(f->data, 0x40)); });
    return o + ", \"_have\": [" + have + "]}";
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::fprintf(stderr, "usage: ne_facts <file listing paths, one per line>\n");
        return 2;
    }
    std::ifstream list(argv[1]);
    std::string path, out;
    auto t0 = std::chrono::steady_clock::now();
    while (std::getline(list, path)) {
        if (!path.empty()) out += facts_of<NEFile>(path) + "\n";
    }
    double secs = std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count();
    std::fwrite(out.data(), 1, out.size(), stdout);
    std::fprintf(stderr, "seconds %.3f\n", secs);
    return 0;
}
