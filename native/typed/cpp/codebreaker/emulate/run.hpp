// codebreaker/emulate/run.hpp -- SHIM over cb/emulate/run.hpp (docs/CPP_DESIGN.md 6.3): the old record, as the
// old callers fill and read it (py:: types, public members), with the three things it computes forwarded to the
// new code. Deleted with its last py:: consumer (the emulators and lanes that still build a Run).
//
// Typing notes (where the Python mixes kinds in one slot):
//   Register   a result register holds an int, a float (PowerPC f1) or None (a driver's missing EAX).
//   Detail     a Call's detail is None, a text, a (device, service) pair, or the 16-bit tracer's {"api": ..., "n": ...}
//              note.
//   raw        the emulator's own record: an opaque handle, never looked into here.
#pragma once

#include <memory>
#include <optional>
#include <tuple>
#include <variant>

#include "cb/compat/py_bridge.hpp"
#include "cb/emulate/run.hpp"

namespace codebreaker::emulate::run {

using Register = std::variant<py::none_t, int64_t, double>;
using Word = std::variant<py::str, int64_t>;
using Detail = std::variant<py::none_t, py::str, std::tuple<std::optional<py::str>, Word>, py::dict<py::str, Word>>;

// Write = namedtuple("Write", "address size value old harness")
struct Write {
    int64_t address = 0;
    int64_t size = 0;
    int64_t value = 0;
    std::optional<int64_t> old;
    bool harness = false;
};

// Call = namedtuple("Call", "name args detail")
struct Call {
    std::optional<py::str> name;
    std::optional<py::list<int64_t>> args;
    Detail detail;
};

// IO = namedtuple("IO", "step kind port value size")
struct IO {
    std::optional<int64_t> step;
    py::str kind;
    int64_t port = 0;
    int64_t value = 0;
    std::optional<int64_t> size;
};

using Preserved = py::dict<py::str, std::tuple<std::optional<int64_t>, std::optional<int64_t>>>;

inline py::str ended(const std::optional<py::str>& e) {
    return cb::compat::to_py(cb::emulate::ended(e ? std::optional<std::string>(e->utf8()) : std::nullopt));
}

class Run {
  public:
    std::optional<py::str> exit;
    py::dict<py::str, Register> results;
    py::list<Write> writes;
    bool capped = false;
    py::list<Call> calls;
    py::list<IO> io;
    std::optional<int64_t> stack;
    std::optional<Preserved> preserved;
    py::list<int64_t> path;
    std::optional<int64_t> steps;
    bool truncated = false;
    py::str endian;
    std::shared_ptr<void> raw;   // the emulator's own record (opaque)

    Run(const std::optional<py::str>& exit_, const std::optional<py::dict<py::str, Register>>& results_ = std::nullopt,
        const py::list<Write>& writes_ = py::list<Write>(), bool capped_ = false,
        const py::list<Call>& calls_ = py::list<Call>(), const py::list<IO>& io_ = py::list<IO>(),
        std::optional<int64_t> stack_ = std::nullopt, const std::optional<Preserved>& preserved_ = std::nullopt,
        const py::list<int64_t>& path_ = py::list<int64_t>(), std::optional<int64_t> steps_ = std::nullopt,
        bool truncated_ = false, const py::str& endian_ = py::str("little"), std::shared_ptr<void> raw_ = nullptr)
        : exit(exit_),
          results(results_ ? results_->copy() : py::dict<py::str, Register>()),
          writes(writes_),
          capped(capped_),
          calls(calls_),
          io(io_),
          stack(stack_),
          preserved(preserved_),
          path(path_),
          steps(steps_),
          truncated(truncated_),
          endian(endian_),
          raw(std::move(raw_)) {}

    py::str ended() const { return run::ended(exit); }

    // Whether the registers a function must hand back came back as they went in (those of `names`, else every one
    // recorded); False when they were not read.
    bool kept(const std::optional<py::list<py::str>>& names = std::nullopt) const {
        cb::emulate::Run r;
        if (preserved) {
            cb::emulate::Preserved p;
            for (const auto& kv : preserved->items()) {
                const auto& [entry, end] = std::get<1>(kv);
                auto u = [](const std::optional<int64_t>& v) { return v ? std::optional<uint64_t>(static_cast<uint64_t>(*v)) : std::nullopt; };
                p[std::get<0>(kv).utf8()] = cb::emulate::SavedRegister{u(entry), u(end)};
            }
            r.preserved = std::move(p);
        }
        if (!names) return r.kept();
        std::vector<std::string> wanted;
        for (const py::str& n : *names) wanted.push_back(n.utf8());
        return r.kept(wanted);
    }

    // Python's __repr__
    py::str repr() const {
        cb::emulate::Run r;
        if (exit) r.exit = exit->utf8();
        r.steps = steps ? std::optional<uint64_t>(static_cast<uint64_t>(*steps)) : std::nullopt;
        r.calls.resize(static_cast<size_t>(py::len(calls)));
        r.writes.resize(static_cast<size_t>(py::len(writes)));
        r.truncated = truncated;
        return cb::compat::to_py(r.summary());
    }
};

}  // namespace codebreaker::emulate::run
