// cb-unity: alone (helpers named for what they do here)
#include "knowledge/describe.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <map>

#include "core/error.hpp"
#include "core/fs.hpp"

namespace cb::knowledge {

namespace stdfs = std::filesystem;

bool is_platform_folder(std::string_view name) { return name != "models" && name != "kept"; }

bool is_entry_kind(std::string_view kind) {
    static constexpr std::array<std::string_view, 9> kinds = {"api",      "service", "register", "region", "export",
                                                              "constant", "type",    "helper",   "signature"};
    return std::find(kinds.begin(), kinds.end(), kind) != kinds.end();
}

std::string Issue::text() const { return std::format("{}: {}: {}", file, where.empty() ? "file" : where, what); }

std::optional<std::uint64_t> hex_value(std::string_view text) {
    if (text.starts_with("0x") || text.starts_with("0X")) text.remove_prefix(2);
    if (text.empty() || text.size() > 16) return std::nullopt;
    std::uint64_t value = 0;
    for (const char c : text) {
        unsigned digit;
        if (c >= '0' && c <= '9') digit = static_cast<unsigned>(c - '0');
        else if (c >= 'a' && c <= 'f') digit = static_cast<unsigned>(c - 'a' + 10);
        else if (c >= 'A' && c <= 'F') digit = static_cast<unsigned>(c - 'A' + 10);
        else return std::nullopt;
        value = (value << 4) | digit;
    }
    return value;
}

namespace {

const std::string* text_of(const Value& map, std::string_view key) {
    const Value* v = map.get(key);
    return v != nullptr ? v->text() : nullptr;
}

// {text: text} with every value text.
void check_text_map(const Value& entry, std::string_view key, std::string_view file, const std::string& where,
                    std::vector<Issue>& issues, bool hex_values) {
    const Value* v = entry.get(key);
    if (v == nullptr) return;
    const Value::Map* map = v->map();
    if (map == nullptr) {
        issues.push_back({std::string(file), where + "." + std::string(key), "an object was expected"});
        return;
    }
    for (const auto& [name, item] : *map) {
        const std::string* t = item.text();
        if (t == nullptr || (hex_values && !hex_value(*t)))
            issues.push_back({std::string(file), where + "." + std::string(key) + "." + name,
                              hex_values ? "a hex number as text was expected" : "text was expected"});
    }
}

void check_entry(const Value& entry, std::string_view file, std::size_t index, std::map<std::string, std::size_t>& ids,
                 std::vector<Issue>& issues) {
    const std::string where = std::format("entries[{}]", index);
    if (entry.map() == nullptr) {
        issues.push_back({std::string(file), where, "an object was expected"});
        return;
    }
    const std::string* id = text_of(entry, "id");
    if (id == nullptr || id->empty()) {
        issues.push_back({std::string(file), where, "no id (text)"});
    } else if (const auto [it, fresh] = ids.emplace(*id, index); !fresh) {
        issues.push_back({std::string(file), where, std::format("the id \"{}\" repeats entries[{}]", *id, it->second)});
    }
    const std::string* kind = text_of(entry, "kind");
    if (kind == nullptr || !is_entry_kind(*kind)) {
        issues.push_back({std::string(file), where, std::format("the kind \"{}\" is not one of the schema's", kind ? *kind : "")});
        return;
    }
    check_text_map(entry, "select", file, where, issues, true);
    check_text_map(entry, "names", file, where, issues, false);
    for (const char* key : {"doc", "since", "until", "origin", "label", "name", "vector"}) {
        const Value* v = entry.get(key);
        if (v != nullptr && v->text() == nullptr) issues.push_back({std::string(file), where + "." + key, "text was expected"});
    }
    if (const Value* model = entry.get("model"); model != nullptr && model->map() == nullptr)
        issues.push_back({std::string(file), where + ".model", "an object was expected"});
    if (*kind == "constant") {
        const std::string* value = text_of(entry, "value");
        if (value == nullptr || !hex_value(*value)) issues.push_back({std::string(file), where + ".value", "a hex number as text was expected"});
        if (text_of(entry, "set") == nullptr) issues.push_back({std::string(file), where + ".set", "a constant belongs to a set (text)"});
    }
    if (*kind == "service") {
        // a service is named by what it is called with (`select`, with a `vector` for an interrupt) or by its id alone, and has a name or a label
        if (text_of(entry, "label") == nullptr && text_of(entry, "name") == nullptr)
            issues.push_back({std::string(file), where, "a service has a name or a label"});
    }
    if (*kind == "region") {
        for (const char* key : {"address", "size"}) {
            const std::string* number = text_of(entry, key);
            if (number == nullptr || !hex_value(*number)) issues.push_back({std::string(file), where + "." + key, "a hex number as text was expected"});
        }
    }
    if (*kind == "register") {
        const std::string* address = text_of(entry, "address");
        if (address == nullptr || !hex_value(*address)) issues.push_back({std::string(file), where + ".address", "a hex number as text was expected"});
    }
}

}  // namespace

std::vector<Issue> check_description(const Value& document, std::string_view file, std::string_view platform, std::string_view domain) {
    std::vector<Issue> issues;
    if (document.map() == nullptr) {
        issues.push_back({std::string(file), "", "an object was expected at the top"});
        return issues;
    }
    const Value* version = document.get("cbk");
    if (version == nullptr || version->integer() != kDescriptionVersion)
        issues.push_back({std::string(file), "cbk", std::format("the schema version must be {}", kDescriptionVersion)});
    const std::string* p = text_of(document, "platform");
    if (p == nullptr || *p != platform) issues.push_back({std::string(file), "platform", std::format("must be \"{}\", the file's folder", platform)});
    const std::string* d = text_of(document, "domain");
    if (d == nullptr || *d != domain) issues.push_back({std::string(file), "domain", std::format("must be \"{}\", the file's name", domain)});
    const std::string* about = text_of(document, "about");
    if (about == nullptr || about->empty()) issues.push_back({std::string(file), "about", "one line of text is required"});
    const Value* source = document.get("source");
    if (source == nullptr || source->map() == nullptr || text_of(*source, "what") == nullptr)
        issues.push_back({std::string(file), "source", "an object with \"what\" is required"});
    const Value* entries = document.get("entries");
    const Value::List* list = entries != nullptr ? entries->list() : nullptr;
    if (list == nullptr) {
        issues.push_back({std::string(file), "entries", "a list was expected"});
        return issues;
    }
    std::map<std::string, std::size_t> ids;
    for (std::size_t i = 0; i < list->size(); ++i) check_entry((*list)[i], file, i, ids, issues);
    return issues;
}

Description parse_description(std::string_view text, std::string_view file, std::string_view platform, std::string_view domain) {
    const Value document = Value::from_json_text(text, file);
    const std::vector<Issue> issues = check_description(document, file, platform, domain);
    if (!issues.empty()) throw FormatError(file, 0, issues.front().text());
    Description out;
    out.platform = *text_of(document, "platform");
    out.domain = *text_of(document, "domain");
    out.about = *text_of(document, "about");
    out.source = *document.get("source");
    const Value::List& list = *document.get("entries")->list();
    out.entries.reserve(list.size());
    for (const Value& entry : list) out.entries.insert_or_assign(*text_of(entry, "id"), entry);
    return out;
}

std::vector<stdfs::path> description_files(const stdfs::path& root) {
    std::vector<stdfs::path> out;
    std::error_code ec;
    for (stdfs::directory_iterator folders(root, ec), end; !ec && folders != end; folders.increment(ec)) {
        std::error_code is_dir_ec;
        if (!folders->is_directory(is_dir_ec)) continue;
        if (!is_platform_folder(fs::to_text(folders->path().filename()))) continue;
        std::error_code inner_ec;
        for (stdfs::directory_iterator files(folders->path(), inner_ec), done; !inner_ec && files != done; files.increment(inner_ec)) {
            std::error_code file_ec;
            if (files->is_regular_file(file_ec) && files->path().extension() == ".json") out.push_back(files->path());
        }
    }
    std::sort(out.begin(), out.end());
    return out;
}

ValidationReport validate_descriptions(const stdfs::path& root) {
    ValidationReport report;
    for (const stdfs::path& path : description_files(root)) {
        ++report.files;
        const std::string platform = fs::to_text(path.parent_path().filename());
        const std::string domain = fs::to_text(path.stem());
        const std::string file = platform + "/" + fs::to_text(path.filename());
        try {
            const Value document = Value::from_json_text(fs::read_text(path), file);
            for (Issue& issue : check_description(document, file, platform, domain)) report.issues.push_back(std::move(issue));
            if (const Value* entries = document.get("entries"); entries != nullptr && entries->list() != nullptr)
                report.entries += entries->list()->size();
        } catch (const Error& e) {
            report.issues.push_back({file, "", e.what()});
        }
    }
    return report;
}

}  // namespace cb::knowledge
