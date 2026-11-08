// codebreaker/translate/backends/cpp17.cpp -- typed C++ port of codebreaker.translate.backends.cpp17 (see the header).
#include "codebreaker/translate/backends/cpp17.hpp"

#include "codebreaker/translate/desugar.hpp"
#include "codebreaker/translate/l2.hpp"
#include "codebreaker/translate/native_l2.hpp"

namespace codebreaker::translate::backends::cpp17 {

namespace l2 = codebreaker::translate::l2;
namespace native_l2 = codebreaker::translate::native_l2;
using native_l2::Wire;

namespace {

py::str K(const char* s) { return py::str(s); }

// <repo>, found from this source file's place in the tree (<repo>/native/typed/cpp/codebreaker/translate/backends/cpp17.cpp)
std::string repo_root_() {
    std::string p = __FILE__;
    for (int up = 0; up < 7; ++up) {
        size_t cut = p.find_last_of("/\\");
        if (cut == std::string::npos) break;
        p = p.substr(0, cut);
    }
    return p;
}
py::str under_repo_(const char* rel) { return py::bytes(repo_root_() + "/" + rel).decode("utf-8"); }

py::str S(const std::string& utf8) { return py::bytes(utf8).decode("utf-8"); }

}  // namespace

// os.path.join(dirname(dirname(abspath(__file__))), 'runtime', 'cpp17') -- codebreaker/translate/runtime/cpp17
const py::str RUNTIME = under_repo_("codebreaker/translate/runtime/cpp17");
const py::list<py::str> FLAGS = {K("-std=c++17"), K("-fwrapv"), K("-w"), K("-O1")};
// codebreaker/java/api_maps
const py::str API_MAPS = under_repo_("codebreaker/java/api_maps");

py::str header_path(const py::str& module) { return module.replace(K("."), K("/")) + K(".hpp"); }

py::str source_path(const py::str& module) { return module.replace(K("."), K("/")) + K(".cpp"); }

std::tuple<py::str, py::str, py::dict<py::str, std::tuple<int64_t, int64_t>>> write_unit(const ir::NodeP& unit,
                                                                                          const std::optional<py::str>&) {
    ir::Dict doc = l2::to_json(unit);
    if (doc.get(K("lang"), ir::Val()).is_none()) doc.set(K("lang"), ir::Val("python"));    // (a unit the Python front end made before it said so)
    Wire r = native_l2::call(K("cpp17_unit"), {native_l2::to_wire(ir::Val(doc)), Wire(API_MAPS.utf8())});
    py::dict<py::str, std::tuple<int64_t, int64_t>> spans;
    for (const auto& kv : r[2].items()) {
        const Wire& v = kv.second;
        spans.set(S(kv.first.str()), std::make_tuple(static_cast<int64_t>(v[0].as_int()), static_cast<int64_t>(v[1].as_int())));
    }
    return {S(r[0].str()), S(r[1].str()), spans};
}

py::str program(const py::list<ir::NodeP>& units, const std::optional<py::str>& main) {
    desugar::run(units, py::list<py::str>{K("foreach"), K("switchexpr")});
    std::vector<Wire> docs;
    for (const ir::NodeP& u : units) docs.push_back(native_l2::to_wire(ir::Val(l2::to_json(u))));
    Wire r = native_l2::call(K("cpp17_program"), {Wire::list(docs), main ? Wire(main->utf8()) : Wire()});
    return S(r.str());
}

std::shared_ptr<cpp03::Bodies17> Bodies(const py::ref<cpp03::Context>& ctx, const py::list<ir::NodeP>& units) {
    return std::make_shared<cpp03::Bodies17>(ctx, units);
}

}  // namespace codebreaker::translate::backends::cpp17
