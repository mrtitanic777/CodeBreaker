// necommon.hpp -- shared by the test.cpp of the ne_asm harnesses: Seg cache, text fields, JSON helpers (on top of ../_vxd_common/common.hpp).
#pragma once
#include <map>
#include <memory>
#include <sstream>

#include "../_vxd_common/common.hpp"
#include "codebreaker/formats/ne.hpp"
#include "codebreaker/source/ne_asm/segment.hpp"

namespace nh {
using namespace vh;

// a std::terminate says what was thrown (a case that dies must not lose the lines before it silently)
inline const bool g_term = [] {
    std::set_terminate([] {
        try {
            throw;
        } catch (const std::exception& e) {
            fprintf(stderr, "terminate: %s\n", e.what());
        } catch (...) {
            fputs("terminate: unknown exception\n", stderr);
        }
        std::abort();
    });
    return true;
}();
namespace ne = codebreaker::formats::ne;
namespace seg = codebreaker::source::ne_asm::segment;

inline const py::dict<py::str, py::str>& ordinals() {
    static const py::dict<py::str, py::str> o = seg::load_ordinals(py::str("ORDINALS.md"));
    return o;
}

struct Box {
    std::unique_ptr<ne::NEFile> ne;
    std::unique_ptr<seg::Seg> sg;
};

// the Seg of segment `si` of the module at `path` (built once)
inline seg::Seg& seg_of(const string& path, int64_t si) {
    static std::map<string, std::unique_ptr<Box>> cache;
    const string key = path + "|" + std::to_string(si);
    auto it = cache.find(key);
    if (it == cache.end()) {
        auto b = std::make_unique<Box>();
        b->ne = std::make_unique<ne::NEFile>(py::str(path));
        b->sg = std::make_unique<seg::Seg>(*b->ne, si, ordinals());
        it = cache.emplace(key, std::move(b)).first;
    }
    return *it->second->sg;
}

inline string unhex(const string& h) {
    if (h == "~") return "";
    return from_hex(h).raw();
}

inline string agg(const vector<string>& xs) {
    string all;
    for (size_t i = 0; i < xs.size(); ++i) all += (i ? "\n" : "") + xs[i];
    return jarr({jint(static_cast<int64_t>(xs.size())), jstr(sha1(all))});
}
}  // namespace nh
