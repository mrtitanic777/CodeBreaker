// stubs.cpp -- TEST ONLY.  verify::reconstruct::identity, answered from knowledge.txt (what the original's
// reconstruct.identity said for each path the run asked about: expected.py recorded it).  The real verify/reconstruct.cpp
// (branch worktree-agent-a5ea8e80b92df7c70) pulls in the decompiler, the builders and every lane.
#include <map>
#include <string>
#include <vector>

#include "codebreaker/verify/reconstruct.hpp"

namespace testkit {
std::map<std::string, std::vector<std::string>>& identities() {
    static std::map<std::string, std::vector<std::string>> m;
    return m;
}
}  // namespace testkit

namespace codebreaker::verify::reconstruct {

std::tuple<py::str, py::str> identity(const py::str& module_path) {
    auto it = testkit::identities().find(module_path.utf8());
    if (it == testkit::identities().end()) throw py::RuntimeError("stub: no recorded identity for " + module_path.utf8());
    const auto& v = it->second;
    if (v[0] == "OK") return {py::str(v[1]), py::str(v[2])};
    if (v[1] == "ValueError") throw py::ValueError(v[2]);
    if (v[1] == "OSError" || v[1] == "FileNotFoundError") throw py::OSError(v[2]);
    if (v[1] == "TypeError") throw py::TypeError(v[2]);
    if (v[1] == "KeyError") throw py::KeyError(v[2]);
    throw py::RuntimeError(v[2]);
}

}  // namespace codebreaker::verify::reconstruct
