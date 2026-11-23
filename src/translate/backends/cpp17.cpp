// translate/backends/cpp17.cpp -- the C++17 back end's client: the tree as its L2 document in, the C++ text out (see cpp17.hpp).

#include "translate/backends/cpp17.hpp"

#include <array>
#include <nlohmann/json.hpp>

#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/text.hpp"
#include "java/paths.hpp"
#include "translate/desugar.hpp"
#include "translate/l2.hpp"

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
    std::string text = lib.cpp17_program(docs, main);
    if (text.find("Character::getType(") == std::string::npos) return text;
    // Java's Unicode categories are a large table: only a program that asks for them carries it
    const std::string table = fs::read_text(runtime_dir().parent_path() / "java_char_types.inc");
    const std::string include = "#include \"jrt.hpp\"\n";
    const std::size_t at = text.find(include);
    CB_ASSERT(at != std::string::npos, "the program includes its runtime");
    text.insert(at + include.size(),
                "static const uint16_t jrt_char_types[] = {\n" + table + "};\n"
                "inline int32_t jrt::Character::getType(char16_t c) {\n"
                "\tsize_t low = 0, high = sizeof(jrt_char_types) / sizeof(jrt_char_types[0]) / 2;\n"
                "\twhile (high - low > 1) {\n"
                "\t\tconst size_t middle = low + (high - low) / 2;\n"
                "\t\tif (jrt_char_types[middle * 2] <= c) low = middle; else high = middle;\n"
                "\t}\n"
                "\treturn jrt_char_types[low * 2 + 1];\n"
                "}\n");
    return text;
}

}  // namespace cb::translate::backends::cpp17
