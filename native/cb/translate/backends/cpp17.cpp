// cb/translate/backends/cpp17.cpp -- the C++17 back end's client: the tree as its L2 document in, the C++ text out (see cpp17.hpp).

#include "cb/translate/backends/cpp17.hpp"

#include <array>
#include <nlohmann/json.hpp>

#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/text.hpp"
#include "cb/java/paths.hpp"
#include "cb/translate/desugar.hpp"
#include "cb/translate/l2.hpp"

namespace cb::translate::backends::cpp17 {

std::filesystem::path runtime_dir() {
    // the Java lane's folder is codebreaker/java; the runtimes are under codebreaker/translate/runtime
    return java::JavaData::locate().root().parent_path() / "translate" / "runtime" / "cpp17";
}

std::filesystem::path api_maps_dir() { return java::JavaData::locate().path({"api_maps"}); }

std::string header_path(std::string_view module) { return text::replace_all(module, ".", "/") + ".hpp"; }
std::string source_path(std::string_view module) { return text::replace_all(module, ".", "/") + ".cpp"; }

UnitText write_unit(const native_l2::Library& lib, const ir::Node& unit) {
    Json doc = l2::to_json(unit);
    if (!doc.contains("lang") || doc["lang"].is_null()) doc["lang"] = "python";   // a unit the Python front end made before it said so
    return lib.cpp17_unit(doc, fs::to_text(api_maps_dir()));
}

std::string program(const native_l2::Library& lib, Arena& arena, std::span<ir::Node* const> units, const std::optional<std::string>& main) {
    static constexpr std::array<desugar::Form, 2> kept = {desugar::Form::ForEach, desugar::Form::SwitchExpr};
    desugar::run(arena, units, kept);
    std::vector<Json> docs;
    docs.reserve(units.size());
    for (const ir::Node* u : units) docs.push_back(l2::to_json(*u));
    return lib.cpp17_program(docs, main);
}

}  // namespace cb::translate::backends::cpp17
