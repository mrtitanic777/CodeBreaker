// stubs.cpp -- TEST ONLY.  What versions calls from ports it does not link, answered from knowledge.txt (expected.py recorded
// what the original's functions said for each path they were asked about):
//   verify::reconstruct::identity       (the real verify/reconstruct.cpp pulls in the decompiler, the builders and every lane)
//   decompile::describe::functions(nef) (decompile/describe.cpp pulls in the emulator and z3 through analyze.image)
#include <map>
#include <string>
#include <vector>

#include "codebreaker/decompile/describe.hpp"
#include "codebreaker/verify/reconstruct.hpp"

namespace testkit {
std::map<std::string, std::vector<std::string>>& identities() {
    static std::map<std::string, std::vector<std::string>> m;
    return m;
}
// path -> [(segment, offset, name)] in the order the original's dict gave them
std::map<std::string, std::vector<std::tuple<int64_t, int64_t, std::string>>>& starts() {
    static std::map<std::string, std::vector<std::tuple<int64_t, int64_t, std::string>>> m;
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

namespace codebreaker::decompile::describe {

py::dict<SegOff, py::str> functions(const formats::ne::NEFile& nef) {
    auto it = testkit::starts().find(nef.path.utf8());
    if (it == testkit::starts().end()) throw py::RuntimeError("stub: describe.functions was not asked about " + nef.path.utf8());
    py::dict<SegOff, py::str> out;
    for (const auto& e : it->second) out.set(SegOff(std::get<0>(e), std::get<1>(e)), py::str(std::get<2>(e)));
    return out;
}

}  // namespace codebreaker::decompile::describe
