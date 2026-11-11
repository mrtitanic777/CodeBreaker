// codebreaker/symbols/libraries/package.hpp -- compat shim for cb/symbols/libraries/package.hpp (docs/CPP_DESIGN.md 6.3).
// The old records in, the new reader called, the old records out; header-only. Deleted with its last old-layer consumer
// (verify/gate16/runtime16).
//
//     auto [kind, found] = libraries::functions(path);   every function of a library or object file:
//                                                        kind 16, 32 (x86) or "powerpc"; found [Function]
//     libraries::fits(function, code, code_mask)         whether `code` starts with the library function
//     Index index(words);                                a program's code, to search for functions in
//     index.find(words, ignore)                          every place a function of words is
//
//   * Function is the namedtuple "name code mask at". Its mask is a byte mask (x86 and 16-bit OMF: one flag per byte) or,
//     for PowerPC, the bits left out of each word (a list of ints): a std::variant.
//   * functions() answers (kind, [Function]); kind is 16 or 32 (int) or "powerpc" (str): std::variant<int64_t, str>.
//   * Index holds ints (the words of PowerPC code, the only units the callers use); `units` is public as it was.
//   * `fits`'s code_mask is the optional byte mask of `code`; an empty one is as none.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>
#include <vector>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/symbols/libraries/package.hpp"
#include "py.hpp"

namespace codebreaker::symbols::libraries {

using Mask = std::variant<py::bytes, py::list<int64_t>>;
using Kind = std::variant<int64_t, py::str>;

// One function of a library: its symbol, its bytes, its mask (a byte mask for x86; for PowerPC the bits left out of each
// word) and its offset in its section.
struct Function {
    py::str name;
    py::bytes code;
    Mask mask;
    int64_t at = 0;

    // (a namedtuple compares field by field)
    friend bool operator==(const Function& a, const Function& b) {
        return a.name == b.name && a.code == b.code && a.mask == b.mask && a.at == b.at;
    }
};

// (kind, [Function]) for every function in a library or object file: kind 16 for an OMF file, else the machine of its
// COFF objects -- 32 (x86) or "powerpc".
inline std::tuple<Kind, py::list<Function>> functions(const py::str& path) {
    return cb::compat::guard([&] {
        const cb::symbols::libraries::Library lib = cb::symbols::libraries::read_file(cb::fs::from_text(cb::compat::from_py(path)));
        py::list<Function> out;
        for (const auto& f : lib.functions) {
            Function o;
            o.name = cb::compat::to_py(f.name);
            o.code = cb::compat::to_py(cb::ByteView(f.code));
            if (lib.machine == cb::symbols::libraries::Machine::PowerPc) o.mask = cb::compat::to_py(f.ignore);
            else o.mask = cb::compat::to_py(cb::ByteView(f.mask));
            o.at = f.at;
            out.append(o);
        }
        Kind kind = int64_t(16);
        if (lib.machine == cb::symbols::libraries::Machine::X86_32) kind = int64_t(32);
        else if (lib.machine == cb::symbols::libraries::Machine::PowerPc) kind = py::str("powerpc");
        return std::tuple<Kind, py::list<Function>>{kind, out};
    });
}

// Whether `code` (with `code_mask`, the places it has that a loader filled in) starts with the library function: as long
// at least, and the same at every byte neither leaves out.
inline bool fits(const Function& function, const py::bytes& code, const std::optional<py::bytes>& code_mask = std::nullopt) {
    cb::symbols::libraries::Function f;
    f.code = cb::compat::from_py(function.code);
    if (const py::bytes* m = std::get_if<py::bytes>(&function.mask)) f.mask = cb::compat::from_py(*m);
    const cb::Bytes theirs = code_mask ? cb::compat::from_py(*code_mask) : cb::Bytes();
    return cb::symbols::libraries::fits(f, cb::compat::from_py(code), theirs);
}

// A program's code as a run of units (the words of PowerPC code), indexed by each pair of neighbours.
class Index {
public:
    explicit Index(const py::list<int64_t>& units_) : units(units_), state_(std::make_shared<State>()) {
        for (const int64_t w : units_.vec()) state_->words.push_back(static_cast<std::uint32_t>(w));
        state_->index = std::make_unique<cb::symbols::libraries::Index>(state_->words);
    }

    py::list<int64_t> units;

    // Every place, in order, where the units `sig` are, outside the bits `ignore` leaves out of each; [] when no two
    // neighbours of it are whole (nothing to look it up by).
    py::list<int64_t> find(const py::list<int64_t>& sig, const py::list<int64_t>& ignore) const {
        std::vector<std::uint32_t> s, i;
        for (const int64_t w : sig.vec()) s.push_back(static_cast<std::uint32_t>(w));
        for (const int64_t w : ignore.vec()) i.push_back(static_cast<std::uint32_t>(w));
        return cb::compat::to_py(cb::compat::guard([&] { return state_->index->find(s, i); }));
    }

private:
    struct State {
        std::vector<std::uint32_t> words;
        std::unique_ptr<cb::symbols::libraries::Index> index;
    };
    std::shared_ptr<State> state_;
};

}  // namespace codebreaker::symbols::libraries
