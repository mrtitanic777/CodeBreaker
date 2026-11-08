// codebreaker/emulate/api_models.cpp -- see api_models.hpp.
#include "codebreaker/emulate/api_models.hpp"

#include <map>

namespace codebreaker::emulate::api_models {

namespace {
// the rows of a platform's api_model kind, read from its authored file once per process
const knowledge::sources::Rows& rows(const py::str& platform) {
    static std::map<std::string, knowledge::sources::Rows> memo;
    auto it = memo.find(platform.utf8());
    if (it != memo.end()) return it->second;
    py::str file = py::os::path::join(knowledge::sources::_MODELS(), platform + py::str(".json"));
    knowledge::sources::Rows r;
    try {
        r = knowledge::sources::_part(py::str("functions"))(file);
    } catch (const py::OSError&) {   // (sources.authored: a file that is not there has no rows)
    } catch (const py::ValueError&) {
    }
    return memo.emplace(platform.utf8(), r).first->second;
}
}  // namespace

std::optional<py::dict<py::str, Json>> api_model(const py::str& platform, const py::str& api) {
    auto row = rows(platform).get(api);
    if (!row) return std::nullopt;
    if (row->kind != Json::Dict || row->dict.empty()) return std::nullopt;   // (`if model`: an empty row is none)
    py::dict<py::str, Json> out;
    for (const auto& kv : row->dict.items()) {
        if (!std::get<0>(kv).startswith(py::str("_"))) out.set(std::get<0>(kv), std::get<1>(kv));
    }
    return out;
}

}  // namespace codebreaker::emulate::api_models
