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
    // Java's Unicode categories are a large table: only a program that asks for them carries it, in a source of its own
    const std::string table = fs::read_text(runtime_dir().parent_path() / "java_char_types.inc");
    return text + "// ==== jrt_char_types.cpp ====\n"
                  "#include \"jrt.hpp\"\n\n"
                  "static const uint16_t jrt_char_types[] = {\n" + table + "};\n"
                  "int32_t jrt::Character::getType(char16_t c) {\n"
                  "\tsize_t low = 0, high = sizeof(jrt_char_types) / sizeof(jrt_char_types[0]) / 2;\n"
                  "\twhile (high - low > 1) {\n"
                  "\t\tconst size_t middle = low + (high - low) / 2;\n"
                  "\t\tif (jrt_char_types[middle * 2] <= c) low = middle; else high = middle;\n"
                  "\t}\n"
                  "\treturn jrt_char_types[low * 2 + 1];\n"
                  "}\n";
}

std::vector<File> split_files(std::string_view text) {
    static constexpr std::string_view mark = "// ==== ";
    std::vector<File> files;
    std::size_t at = 0;
    while (at < text.size()) {
        CB_ASSERT(text.substr(at, mark.size()) == mark, "a program's text starts each file with its marker line");
        const std::size_t eol = text.find('\n', at);
        const std::string_view line = text.substr(at + mark.size(), eol - at - mark.size());
        File f;
        f.path = std::string(line.substr(0, line.size() - 5));   // (" ====")
        std::size_t next = eol + 1;
        while (next < text.size() && text.substr(next, mark.size()) != mark) {
            const std::size_t end = text.find('\n', next);
            next = end == std::string_view::npos ? text.size() : end + 1;
        }
        f.text = std::string(text.substr(eol + 1, next - eol - 1));
        files.push_back(std::move(f));
        at = next;
    }
    return files;
}

std::vector<File> program_files(const native_l2::Library& lib, Arena& arena, std::span<ir::Node* const> units, const std::optional<std::string>& main) {
    return split_files(program(lib, arena, units, main));
}

std::vector<std::filesystem::path> write_files(const std::vector<File>& files, const std::filesystem::path& folder) {
    std::vector<std::filesystem::path> sources;
    for (const File& f : files) {
        const std::filesystem::path path = folder / fs::from_text(f.path);
        fs::make_dirs(path.parent_path());
        fs::write_text(path, f.text);
        if (path.extension() == ".cpp") sources.push_back(path);
    }
    return sources;
}

}  // namespace cb::translate::backends::cpp17
