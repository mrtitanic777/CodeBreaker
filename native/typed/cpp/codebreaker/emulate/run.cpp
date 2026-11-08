// codebreaker/emulate/run.cpp -- C++ port of codebreaker.emulate.run (see run.hpp).

#include "codebreaker/emulate/run.hpp"

namespace codebreaker::emulate::run {

py::str ended(const std::optional<py::str>& e) {
    if (!e.has_value()) {
        return py::str("none");
    }
    const py::str& s = *e;
    if (s == py::str("ret")) {
        return py::str("ret");
    }
    if (s.startswith(py::str("exception"))) {
        return py::str("fault");
    }
    return py::split(s, py::str(":"))[0];
}

Run::Run(const std::optional<py::str>& exit_, const std::optional<py::dict<py::str, Register>>& results_,
         const py::list<Write>& writes_, bool capped_, const py::list<Call>& calls_, const py::list<IO>& io_,
         std::optional<int64_t> stack_, const std::optional<Preserved>& preserved_, const py::list<int64_t>& path_,
         std::optional<int64_t> steps_, bool truncated_, const py::str& endian_, std::shared_ptr<void> raw_)
    : exit(exit_),
      results(results_ ? results_->copy() : py::dict<py::str, Register>()),   // dict(results or {})
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

py::str Run::ended() const {
    return ::codebreaker::emulate::run::ended(exit);
}

bool Run::kept(const std::optional<py::list<py::str>>& names) const {
    if (!preserved.has_value()) {
        return false;
    }
    using Pair = std::tuple<std::optional<int64_t>, std::optional<int64_t>>;
    if (!names.has_value()) {
        for (const auto& kv : preserved->items()) {
            const Pair& p = std::get<1>(kv);
            if (!(std::get<0>(p) == std::get<1>(p))) return false;
        }
        return true;
    }
    for (const py::str& n : *names) {
        Pair p = preserved->get(n, Pair(std::nullopt, std::nullopt));
        if (!(std::get<0>(p) == std::get<1>(p))) return false;
    }
    return true;
}

py::str Run::repr() const {
    return py::percent(py::str("<Run %s steps=%s calls=%d writes=%d%s>"), exit, steps, py::len(calls), py::len(writes),
                       truncated ? py::str(" truncated") : py::str(""));
}

}  // namespace codebreaker::emulate::run
