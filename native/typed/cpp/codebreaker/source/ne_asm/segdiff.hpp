// codebreaker/source/ne_asm/segdiff.hpp -- SHIM over cb/source/ne_asm/segdiff.hpp (docs/CPP_DESIGN.md 6.3): the whole-segment byte-match
// harness (is this reconstructed C byte-exact?). The old types in (sets of offsets, py::bytes), the new functions called; the compiler is
// the old build/msvc152 (Microsoft C 8 under DOSBox-X), the module lookup the old corpus shim. Deleted with its last py:: consumer.
//
//     segdiff --selftest                                 prove the mechanism
//     segdiff recon.c --module KRNL386 --seg 1 --off 0x1234 --func _Foo       the verdict against the real bytes
//
// Differences from the old port that the shim cannot hide: `_selftest` and `main` write to CodeBreaker's stdout/stderr (cb::out), not to
// the streams they are handed; the streams parameters are kept so callers compile.
#pragma once

#include <cstdint>
#include <filesystem>
#include <iosfwd>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/source/ne_asm/segdiff.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/msvc152.hpp"

namespace codebreaker::source::ne_asm::segdiff {

namespace cbs = cb::source::ne_asm::segdiff;

namespace shim_ {

// Microsoft C 8 as the old build/msvc152 runs it: the object file bytes. Its work folder is removed after, as the Python did.
inline cb::Bytes compile(std::string_view c_source, std::string_view flags) {
    const auto r = build::msvc152::compile_c(cb::compat::to_py(c_source), cb::compat::to_py(flags));
    struct Cleanup {
        const build::msvc152::Compiled& r;
        ~Cleanup() {
            std::error_code ec;
            if (r.work) std::filesystem::remove_all(cb::fs::from_text(cb::compat::from_py(*r.work)), ec);
        }
    } cleanup{r};
    if (!r.obj) throw cb::ToolError("msvc152", 1, r.log ? cb::compat::from_py(*r.log) : std::string(), "the compiler produced no object");
    return cb::compat::from_py(*r.obj);
}

}  // namespace shim_

// NE relocation source-type -> width in bytes of the relocated field
inline const py::dict<int64_t, int64_t> NE_SRC_WIDTH = {{0, 1}, {2, 2}, {3, 4}, {5, 2}, {11, 6}};

// What the old target registry answered: where a module of the corpus is.
struct Corpus {
    static py::str resolve(const py::str& module) {
        if (const auto found = base::corpus::retail(module)) return *found;
        return module;
    }
};
inline Corpus _target() { return Corpus{}; }

// {function: (its bytes, the offsets its fix-ups cover)}; RuntimeError (a tool error) when the compiler made no object
inline py::dict<py::str, std::tuple<py::bytes, py::set<int64_t>>> compile_group(const py::str& csrc, const py::str& flags = py::str("/AM /Ox /c")) {
    return cb::compat::guard([&] {
        py::dict<py::str, std::tuple<py::bytes, py::set<int64_t>>> out;
        const auto group = cbs::compile_group(shim_::compile, cb::compat::from_py(csrc), cb::compat::from_py(flags));
        for (const auto& [name, f] : group) {
            py::set<int64_t> mask;
            for (const std::uint64_t m : f.fixup_mask) mask.add(static_cast<int64_t>(m));
            out.set(cb::compat::to_py(name), std::make_tuple(cb::compat::to_py(cb::ByteView(f.code)), mask));
        }
        return out;
    });
}

inline std::tuple<py::bytes, py::set<int64_t>> shipped_ne_segment(const py::str& module_path, int64_t seg_index) {
    return cb::compat::guard([&] {
        const auto module = cb::formats::ne::Module::open_file(cb::fs::from_text(cb::compat::from_py(module_path)));
        const cbs::Shipped s = cbs::shipped_segment(module, static_cast<std::size_t>(seg_index));
        py::set<int64_t> mask;
        for (const std::uint64_t m : s.mask) mask.add(static_cast<int64_t>(m));
        return std::make_tuple(cb::compat::to_py(cb::ByteView(s.code)), mask);
    });
}

struct DiffResult {
    bool exact = false;
    bool length_match = false;
    int64_t our_len = 0, shipped_len = 0;
    int64_t n_diff = 0;
    std::optional<int64_t> first_diff;
    int64_t masked = 0;
};

namespace shim_ {
inline cbs::Mask mask_of(const py::set<int64_t>& s) {
    cbs::Mask m;
    for (const int64_t v : s) m.insert(static_cast<std::uint64_t>(v));
    return m;
}
inline cbs::Verdict verdict_of(const DiffResult& r) {
    cbs::Verdict v;
    v.exact = r.exact;
    v.length_match = r.length_match;
    v.our_len = static_cast<std::size_t>(r.our_len);
    v.shipped_len = static_cast<std::size_t>(r.shipped_len);
    v.n_diff = static_cast<std::size_t>(r.n_diff);
    if (r.first_diff) v.first_diff = static_cast<std::size_t>(*r.first_diff);
    v.masked = static_cast<std::size_t>(r.masked);
    return v;
}
}  // namespace shim_

// Compare two byte strings ignoring masked positions on either side (sets of offsets).
inline DiffResult diff(const py::bytes& our, const py::set<int64_t>& omask, const py::bytes& shipped, const py::set<int64_t>& smask) {
    const cb::Bytes a = cb::compat::from_py(our), b = cb::compat::from_py(shipped);
    const cbs::Verdict v = cbs::diff(a, shim_::mask_of(omask), b, shim_::mask_of(smask));
    DiffResult r;
    r.exact = v.exact;
    r.length_match = v.length_match;
    r.our_len = static_cast<int64_t>(v.our_len);
    r.shipped_len = static_cast<int64_t>(v.shipped_len);
    r.n_diff = static_cast<int64_t>(v.n_diff);
    if (v.first_diff) r.first_diff = static_cast<int64_t>(*v.first_diff);
    r.masked = static_cast<int64_t>(v.masked);
    return r;
}

// the verdict as text; `our` and `shipped` (given) show the bytes around the first difference
inline py::str render(const DiffResult& res, const std::optional<py::bytes>& our = std::nullopt, const std::optional<py::bytes>& shipped = std::nullopt,
                      const py::set<int64_t>& omask = py::set<int64_t>(), const py::set<int64_t>& smask = py::set<int64_t>()) {
    const cb::Bytes a = our ? cb::compat::from_py(*our) : cb::Bytes(), b = shipped ? cb::compat::from_py(*shipped) : cb::Bytes();
    return cb::compat::to_py(cbs::render(shim_::verdict_of(res), our ? std::optional<cb::ByteView>(a) : std::nullopt,
                                         shipped ? std::optional<cb::ByteView>(b) : std::nullopt, shim_::mask_of(omask), shim_::mask_of(smask)));
}

inline const py::str GROUP = py::str(std::string(cbs::kSelftestGroup));

inline bool _selftest(std::ostream&) { return cbs::selftest(shim_::compile); }

inline int64_t main(const py::list<py::str>& argv, std::ostream&, std::ostream&) {
    return cb::compat::guard([&]() -> int64_t {
        std::vector<std::string> args;
        for (const py::str& a : argv) args.push_back(cb::compat::from_py(a));
        return cbs::main({::codebreaker::base::corpus::detail::instance(), shim_::compile}, args);
    });
}

}  // namespace codebreaker::source::ne_asm::segdiff
