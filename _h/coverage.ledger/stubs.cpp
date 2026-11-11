// stubs.cpp -- TEST ONLY.  verify::reconstruct's lane registry, as much of it as the ledger calls (digests).
// The real verify/reconstruct.cpp (branch worktree-agent-a5ea8e80b92df7c70) pulls in the decompiler, the builders and every
// lane; its two answers the ledger needs -- the verifier profile per kind -- are registered here by test.cpp from
// digests.txt, which expected.py wrote from the original's reconstruct.digests().
#include <memory>
#include <vector>

#include "codebreaker/verify/reconstruct.hpp"

namespace codebreaker::verify::reconstruct {

namespace {
std::vector<std::pair<py::str, std::shared_ptr<Kind>>>& reg() {
    static std::vector<std::pair<py::str, std::shared_ptr<Kind>>> r;
    return r;
}
}  // namespace

void register_kind(const py::str& name, const std::shared_ptr<Kind>& kind) { reg().emplace_back(name, kind); }

py::dict<py::str, std::optional<py::str>> digests() {
    py::dict<py::str, std::optional<py::str>> out;
    for (const auto& e : reg()) {
        try {
            out.set(e.first, std::optional<py::str>(std::get<0>(e.second->profile(py::str("")))));
        } catch (const std::exception&) {
            out.set(e.first, std::optional<py::str>());
        }
    }
    return out;
}

}  // namespace codebreaker::verify::reconstruct
