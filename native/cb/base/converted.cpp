#include "cb/base/converted.hpp"

#include <algorithm>
#include <chrono>
#include <ctime>
#include <set>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/json_struct.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/detect.hpp"
#include "cb/formats/xex.hpp"

namespace cb::base::converted {
namespace stdfs = std::filesystem;

namespace {

// file names that say nothing about the program (a console's default.xex, a setup.exe): the project is named after what
// the binary calls itself, or its folder, instead
bool is_generic(std::string_view stem) {
    static const std::set<std::string, std::less<>> kGeneric = {"default", "main", "setup", "install", "game", "program", "app", "boot"};
    return kGeneric.contains(text::lower(stem));
}

// A path as text with the separators Windows tools write.
std::string native_text(const stdfs::path& p) {
    std::string s = fs::to_text(p);
    std::replace(s.begin(), s.end(), '/', '\\');
    return s;
}

// The file name without its extension as os.path.splitext cuts it: leading dots belong to the name (`...x` has none).
std::string stem_of(const stdfs::path& p) {
    const std::string name = fs::to_text(p.filename());
    std::size_t lead = 0;
    while (lead < name.size() && name[lead] == '.') ++lead;
    const std::size_t dot = name.rfind('.');
    return dot == std::string::npos || dot < lead ? name : name.substr(0, dot);
}

std::size_t code_points(std::string_view s) {
    std::size_t n = 0;
    for (const char c : s) n += (static_cast<unsigned char>(c) & 0xC0) != 0x80;
    return n;
}

// The name an XEX gives itself (without its extension), if the file is one and says it.
std::optional<std::string> xex_own_name(const stdfs::path& binary) {
    if (!formats::xex::probe(binary)) return std::nullopt;
    try {
        const formats::xex::Header header = formats::xex::read_header(binary);
        if (!header.name) return std::nullopt;
        return stem_of(fs::from_text(*header.name));
    } catch (const FormatError&) {   // an XEX2 header that does not parse names nothing
        return std::nullopt;
    } catch (const IoError&) {
        return std::nullopt;
    }
}

// ---- project.json -------------------------------------------------------------------------------------------------

Extra take_extra(const Json& object, const std::set<std::string, std::less<>>& known_and_used) {
    Extra extra;
    for (const auto& [key, value] : object.items()) {
        if (!known_and_used.contains(key)) extra.emplace_back(key, value.dump());
    }
    return extra;
}

Step step_of(const Json& j) {
    Step s;
    std::set<std::string, std::less<>> used;
    if (const auto it = j.find("command"); it != j.end() && it->is_string()) {
        s.command = it->get<std::string>();
        used.insert("command");
    }
    if (const auto it = j.find("when"); it != j.end() && it->is_string()) {
        s.when = it->get<std::string>();
        used.insert("when");
    }
    if (const auto it = j.find("notes"); it != j.end() && it->is_string()) {
        s.notes = it->get<std::string>();
        used.insert("notes");
    }
    if (const auto it = j.find("outputs"); it != j.end() && it->is_array() &&
                                           std::all_of(it->begin(), it->end(), [](const Json& o) { return o.is_string(); })) {
        for (const Json& o : *it) s.outputs.push_back(o.get<std::string>());
        used.insert("outputs");
    }
    s.extra = take_extra(j, used);
    return s;
}

Manifest manifest_of(const Json& j) {
    Manifest m;
    std::set<std::string, std::less<>> used;
    const auto text_of = [&](const char* key, std::optional<std::string>& into) {
        if (const auto it = j.find(key); it != j.end() && it->is_string()) {
            into = it->get<std::string>();
            used.insert(key);
        }
    };
    text_of("name", m.name);
    text_of("file", m.file);
    text_of("source", m.source);
    text_of("sha1", m.sha1);
    text_of("kind", m.kind);
    if (const auto it = j.find("size"); it != j.end() && it->is_number_unsigned()) {
        m.size = it->get<std::uint64_t>();
        used.insert("size");
    }
    if (const auto it = j.find("steps"); it != j.end() && it->is_array()) {
        for (const Json& s : *it) {
            if (s.is_object()) m.steps.push_back(step_of(s));   // DIVERGES: a step that is no object is dropped (the Python raised)
        }
        used.insert("steps");
    }
    m.extra = take_extra(j, used);
    return m;
}

Json json_of(const Step& s) {
    Json j = Json::object();
    j["command"] = s.command;
    j["when"] = s.when;
    j["outputs"] = s.outputs;
    if (!s.notes.empty()) j["notes"] = s.notes;
    for (const auto& [key, value] : s.extra) j[key] = Json::parse(value);
    return j;
}

Json json_of(const Manifest& m) {
    Json j = Json::object();
    if (m.name) j["name"] = *m.name;
    if (m.file) j["file"] = *m.file;
    if (m.source) j["source"] = *m.source;
    if (m.sha1) j["sha1"] = *m.sha1;
    if (m.size) j["size"] = *m.size;
    if (m.kind) j["kind"] = *m.kind;
    j["steps"] = Json::array();
    for (const Step& s : m.steps) j["steps"].push_back(json_of(s));
    for (const auto& [key, value] : m.extra) j[key] = Json::parse(value);
    return j;
}

// The manifest in the text of a project.json; empty when the text is not a JSON object.
Manifest manifest_in(std::string_view text) {
    Json j;
    if (!json::try_parse(text, j)) return {};
    return j.is_object() ? manifest_of(j) : Manifest{};
}

// The text of a project.json; "" when it is missing or cannot be read (a manifest that cannot be read is no manifest).
std::string manifest_text(const stdfs::path& file) {
    try {
        return fs::read_text(file);
    } catch (const IoError&) {
        return {};
    }
}

// project.json of a folder; empty when it is missing, unreadable or not an object.
Manifest read_manifest(const stdfs::path& file) { return manifest_in(manifest_text(file)); }

std::string readme_of(const Manifest& m, const std::string& folder_name) {
    const std::string name = m.name.value_or(folder_name);
    std::vector<std::string> lines = {
        name,
        std::string(code_points(name), '='),
        "",
        "What CodeBreaker made of " + m.file.value_or("a binary") + ".",
        "",
        "  Input:   " + m.source.value_or(""),
        "  Kind:    " + (m.kind && !m.kind->empty() ? *m.kind : std::string("unknown")),
        "  Size:    " + (m.size ? std::to_string(*m.size) : std::string("?")) + " bytes",
        "  SHA-1:   " + m.sha1.value_or(""),
        "",
        "Folders:",
        "  original/  the binary as it came in",
        "  src/       source CodeBreaker produced",
        "  assets/    resources and data",
        "  build/     what was built from src/",
        "  reports/   analysis, listings, logs",
        ""};
    for (const Step& s : m.steps) {
        std::string when = s.when;
        std::replace(when.begin(), when.end(), 'T', ' ');
        lines.push_back(s.command + "  (" + when + ")");
        for (const std::string& o : s.outputs) lines.push_back("    " + o);
        for (const std::string_view n : text::lines(s.notes)) lines.push_back("    " + std::string(n));
        lines.push_back("");
    }
    lines.push_back("The binary and everything made from it are the original's owner's: this folder is never committed.");
    std::string out;
    for (const std::string& l : lines) out += l + "\r\n";
    return out;
}

}  // namespace

std::string clean_name(std::string_view name) {
    std::string out;
    bool in_run = false;
    for (const char c : name) {
        const bool ok = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '.' || c == '_' || c == '-';
        if (ok) {
            out += c;
            in_run = false;
        } else if (!in_run) {   // a run of other bytes (a multi-byte letter is several) is one '_'
            out += '_';
            in_run = true;
        }
    }
    const std::size_t first = out.find_first_not_of("._");
    if (first == std::string::npos) return "binary";
    const std::size_t last = out.find_last_not_of("._");
    return out.substr(first, last - first + 1);
}

std::string default_name(const stdfs::path& binary) {
    const std::string stem = stem_of(binary);
    if (!is_generic(stem)) return clean_name(stem);
    if (const auto own = xex_own_name(binary); own && !own->empty() && !is_generic(*own)) return clean_name(*own);
    const std::string parent = fs::to_text(stdfs::absolute(binary).parent_path().filename());
    return clean_name(parent.empty() ? stem : parent);
}

std::string kind_of(const stdfs::path& binary) {
    const auto found = formats::detect::identify(binary);
    return found.empty() ? std::string() : found.front().format;
}

std::string sha1_of(const stdfs::path& file) {
    crypto::Hasher hasher(crypto::Hasher::Algorithm::Sha1);
    if (fs::file_size(file) != 0) {
        const fs::MappedFile mapped(file);
        constexpr std::size_t kBlock = std::size_t{1} << 20;
        const ByteView all = mapped.view();
        for (std::size_t at = 0; at < all.size(); at += kBlock) hasher.update(all.subspan(at, std::min(kBlock, all.size() - at)));
    }
    return crypto::to_hex(hasher.finish());
}

std::string local_time_now() {
    const std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm parts{};
    localtime_s(&parts, &now);
    char buffer[32];
    std::strftime(buffer, sizeof buffer, "%Y-%m-%dT%H:%M:%S", &parts);
    return buffer;
}

// ---- Project ------------------------------------------------------------------------------------------------------

Project::Project(stdfs::path folder, Clock clock) : root_(std::move(folder)), clock_(std::move(clock)) {
    for (const std::string_view sub : kFolders) fs::make_dirs(root_ / sub);
}

std::string Project::name() const { return fs::to_text(root_.filename()); }

Manifest Project::manifest() const { return read_manifest(manifest_path()); }

std::optional<stdfs::path> Project::original_file() const {
    const Manifest m = manifest();
    if (!m.file || m.file->empty()) return std::nullopt;
    return original() / fs::from_text(*m.file);
}

void Project::record(std::string_view command, const std::vector<stdfs::path>& outputs, std::string_view notes) const {
    Manifest m = manifest();
    Step step;
    step.command = std::string(command);
    step.when = clock_();
    const stdfs::path here = stdfs::absolute(root_).lexically_normal();
    for (const stdfs::path& out : outputs) {
        const stdfs::path abs = stdfs::absolute(out).lexically_normal();
        // inside the project folder (by whole path components): relative to it; anywhere else: as given
        const stdfs::path rel = abs.lexically_relative(here);
        const bool inside = !rel.empty() && *rel.begin() != ".." && !rel.is_absolute();
        step.outputs.push_back(native_text(inside ? rel : abs));
    }
    step.notes = std::string(notes);
    // one entry per command: running it again replaces what it said before
    std::erase_if(m.steps, [&](const Step& s) { return s.command == step.command; });
    m.steps.push_back(std::move(step));
    write(m);
}

void Project::write(const Manifest& m) const {
    // indent 1 and ASCII escapes: the text the Python's json.dump(d, indent=1) wrote
    const std::string text = json_of(m).dump(1, ' ', true);
    // project.json is the record of what was done: replaced whole or not at all. README.txt is made from it, and again at the next
    // write.
    fs::write_file_atomic(manifest_path(), as_bytes(text));
    fs::write_text(root_ / "README.txt", readme_of(m, name()));
}

// ---- Store --------------------------------------------------------------------------------------------------------

stdfs::path Store::root() const {
    fs::make_dirs(root_);
    return root_;
}

Project Store::project(const stdfs::path& binary, const std::optional<std::string>& name_given,
                       const std::optional<stdfs::path>& base_folder) const {
    // DIVERGES: an empty name is no name (the Python took it as one for finding a project, so nothing was found)
    const std::optional<std::string> name = name_given && !name_given->empty() ? name_given : std::nullopt;
    const stdfs::path path = stdfs::absolute(binary);
    const std::string digest = sha1_of(path);
    const stdfs::path base = base_folder ? *base_folder : root();
    fs::make_dirs(base);

    std::vector<std::string> entries;
    for (const auto& entry : stdfs::directory_iterator(base)) entries.push_back(fs::to_text(entry.path().filename()));
    std::sort(entries.begin(), entries.end());
    const std::string wanted_entry = name ? clean_name(*name) : std::string();
    for (const std::string& entry : entries) {
        const stdfs::path manifest = base / fs::from_text(entry) / "project.json";
        std::error_code ec;
        if (!stdfs::is_regular_file(manifest, ec)) continue;
        if (name && entry != wanted_entry) continue;
        // (most of the folders are other binaries': the digest is not in their manifest, which then need not be parsed)
        const std::string text = manifest_text(manifest);
        if (text.find(digest) == std::string::npos) continue;
        if (manifest_in(text).sha1 == digest) return Project(base / fs::from_text(entry), clock_);
    }

    const std::string wanted = name ? clean_name(*name) : default_name(path);
    stdfs::path folder = base / fs::from_text(wanted);
    for (int n = 2; stdfs::exists(folder); ++n) folder = base / fs::from_text(wanted + "_" + std::to_string(n));

    Project p(folder, clock_);
    stdfs::copy_file(path, p.original() / path.filename(), stdfs::copy_options::overwrite_existing);
    Manifest m;
    m.name = p.name();
    m.file = fs::to_text(path.filename());
    m.source = native_text(path);
    m.sha1 = digest;
    m.size = fs::file_size(path);
    m.kind = kind_of(path);
    p.write(m);
    return p;
}

}  // namespace cb::base::converted
