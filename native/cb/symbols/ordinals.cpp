// cb/symbols/ordinals.cpp -- the name of a function imported by number (see ordinals.hpp).
#include "cb/symbols/ordinals.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <format>

#include <nlohmann/json.hpp>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/json.hpp"
#include "cb/core/text.hpp"
#include "cb/knowledge/reads.hpp"
#include "cb/symbols/demangle.hpp"
#include "cb/symbols/libraries/coff.hpp"

namespace cb::symbols::ordinals {
namespace {

using std::string;
using std::string_view;

constexpr std::array<string_view, 5> kModuleExtensions = {".dll", ".exe", ".drv", ".cpl", ".ocx"};

const std::map<std::uint32_t, string>& no_names() {
    static const std::map<std::uint32_t, string> none;
    return none;
}

}  // namespace

std::string module_stem(std::string_view module) {
    const size_t slash = module.find_last_of("/\\");
    const string base = text::upper(slash == string_view::npos ? module : module.substr(slash + 1));
    for (const string_view ext : kModuleExtensions) {
        if (base.ends_with(text::upper(ext))) return base.substr(0, base.size() - ext.size());
    }
    return base;
}

std::optional<Documented> documented(std::string_view module, std::uint32_t ordinal) {
    // Geoff Chappell's list of the ordinal-only exports of KERNEL32 4.0 (the names per the Windows Me symbol file)
    static const std::map<std::uint32_t, string_view> kernel32 = {{17, "CommonUnimpStub"},
                                                                  {20, "DosFileHandleToWin32Handle"},
                                                                  {21, "Win32HandleToDosFileHandle"},
                                                                  {22, "DisposeLZ32Handle"},
                                                                  {41, "ThunkInitLSF"},
                                                                  {89, "FT_PrologPrime"},
                                                                  {90, "QT_ThunkPrime"}};
    if (module_stem(module) != "KERNEL32") return std::nullopt;
    const auto it = kernel32.find(ordinal);
    if (it == kernel32.end()) return std::nullopt;
    return Documented{string(it->second), "Geoff Chappell, Ordinal-Only Exports In KERNEL32 4.0 (names per the Windows Me symbol file)"};
}

// ---- ordinal_names.json ----------------------------------------------------------------------------------------

ShippedTables parse_shipped(std::string_view json_text) {
    const Json doc = json::parse(json_text);
    constexpr string_view kSource = "ordinal_names.json";
    if (!doc.is_object()) throw ParseError(kSource, 0, 0, "not an object of modules");
    ShippedTables out;
    for (const auto& [module, entry] : doc.items()) {
        if (!entry.is_object()) throw ParseError(kSource, 0, 0, module + ": not an object");
        ShippedModule m;
        if (const auto it = entry.find("from"); it != entry.end()) {
            if (!it->is_string()) throw ParseError(kSource, 0, 0, module + ": \"from\" is not text");
            m.from = it->get<string>();
        }
        if (const auto it = entry.find("checked"); it != entry.end()) {
            if (!it->is_array() || it->size() != 2 || !(*it)[0].is_number_unsigned() || !(*it)[1].is_number_unsigned()) {
                throw ParseError(kSource, 0, 0, module + ": \"checked\" is not [yes, no]");
            }
            m.checked_yes = (*it)[0].get<std::uint32_t>();
            m.checked_no = (*it)[1].get<std::uint32_t>();
        }
        if (const auto it = entry.find("names"); it != entry.end()) {
            if (!it->is_object()) throw ParseError(kSource, 0, 0, module + ": \"names\" is not an object");
            for (const auto& [number, name] : it->items()) {
                const auto n = text::parse_int<std::uint32_t>(number);
                if (!n || !name.is_string()) throw ParseError(kSource, 0, 0, module + ": bad name entry \"" + number + "\"");
                m.names[*n] = name.get<string>();
            }
        }
        out[module] = std::move(m);
    }
    return out;
}

std::string format_shipped(const ShippedTables& tables) {
    Json doc = Json::object();
    for (const auto& [module, m] : tables) {
        std::map<string, string> names;   // text keys sorted as text
        for (const auto& [number, name] : m.names) names[std::to_string(number)] = name;
        Json jnames = Json::object();
        for (const auto& [number, name] : names) jnames[number] = name;
        doc[module] = Json{{"checked", Json::array({m.checked_yes, m.checked_no})}, {"from", m.from}, {"names", jnames}};
    }
    return json::dump(doc, json::Style::Pretty) + "\n";
}

// ---- the table of one module ------------------------------------------------------------------------------------

Ordinals::Ordinals(const ModuleExports& exports, ShippedTables shipped, Spelling spelling)
    : exports_(exports), shipped_(std::move(shipped)), spelling_(std::move(spelling)) {}

std::shared_ptr<const ModuleExports::Table> Ordinals::from_corpus(const std::string& stem) const {
    {
        const std::lock_guard lock(mutex_);
        if (const auto it = corpus_.find(stem); it != corpus_.end()) return it->second;
    }
    auto read = std::make_shared<const ModuleExports::Table>(exports_.table(stem).value_or(ModuleExports::Table{}));
    const std::lock_guard lock(mutex_);
    return corpus_.try_emplace(stem, std::move(read)).first->second;
}

Ordinals::FromFile Ordinals::from_file(const std::string& stem) const {
    static const string nothing;
    knowledge::record("windows", "ordinal_names", stem);
    const auto it = shipped_.find(stem);
    if (it == shipped_.end()) return {&no_names(), &nothing};
    return {&it->second.names, &it->second.from};
}

std::optional<std::string> Ordinals::linker_name(std::string_view module, std::uint32_t ordinal) const {
    const string stem = module_stem(module);
    const auto table = from_corpus(stem);
    if (const auto it = table->names.find(ordinal); it != table->names.end() && !it->second.empty()) return it->second;
    const FromFile file = from_file(stem);
    if (const auto it = file.names->find(ordinal); it != file.names->end() && !it->second.empty()) return it->second;
    return std::nullopt;
}

std::optional<std::string> Ordinals::name(std::string_view module, std::uint32_t ordinal) const {
    std::optional<string> raw = linker_name(module, ordinal);
    if (raw && !raw->starts_with('?') && spelling_) return spelling_(*raw);
    return raw;
}

std::string Ordinals::source(std::string_view module, std::uint32_t ordinal) const {
    const string stem = module_stem(module);
    const auto table = from_corpus(stem);
    if (table->names.contains(ordinal)) return table->file;
    const FromFile file = from_file(stem);
    return file.names->contains(ordinal) ? *file.from : string();
}

std::map<std::uint32_t, std::string> Ordinals::table(std::string_view module) const {
    const string stem = module_stem(module);
    const FromFile file = from_file(stem);
    std::map<std::uint32_t, string> merged = *file.names;
    for (const auto& [number, name] : from_corpus(stem)->names) merged[number] = name;   // the corpus's names win
    if (spelling_) {
        for (auto& [number, name] : merged) {
            if (!name.starts_with('?')) name = spelling_(name);
        }
    }
    return merged;
}

std::optional<std::string> Ordinals::identifier(std::string_view module, std::uint32_t ordinal) const {
    std::optional<string> nm = name(module, ordinal);
    if (nm && nm->find('@') != string::npos && !nm->starts_with('?')) nm = demangle::plain(*nm).name;   // `_Name@12`, `Name@12`
    if (nm && text::is_identifier(*nm)) return nm;
    return std::nullopt;
}

std::vector<NamedImport> Ordinals::names_of(std::span<const ImportRef> imports) const {
    std::vector<NamedImport> out;
    std::set<ImportRef> seen;
    for (const ImportRef& i : imports) {
        if (!seen.insert(i).second) continue;
        out.push_back({i, name(i.module, i.ordinal)});
    }
    return out;
}

// ---- definition files -----------------------------------------------------------------------------------------------

std::map<std::string, std::map<std::uint32_t, std::string>> read_definition_file(std::string_view file_text,
                                                                                   std::string_view module_from_file) {
    string module = text::upper(module_from_file);
    std::map<std::uint32_t, string> names;
    const auto is_word = [](char c) { return std::isalnum(static_cast<unsigned char>(c)) || c == '_'; };
    string normalised = text::replace_all(file_text, "\r\n", "\n");
    std::replace(normalised.begin(), normalised.end(), '\r', '\n');
    for (string_view line : text::lines(normalised)) {
        line = line.substr(0, line.find(';'));   // a comment runs to the end of the line
        // LIBRARY name
        {
            const string_view l = text::ltrim(line);
            if (text::istarts_with(l, "LIBRARY") && l.size() > 7 && text::is_space(l[7])) {
                string_view rest = text::ltrim(l.substr(7));
                size_t n = 0;
                while (n < rest.size() && is_word(rest[n])) ++n;
                if (n > 0) module = text::upper(rest.substr(0, n));
            }
        }
        // name[=alias] @ ordinal
        const string_view l = text::ltrim(line);
        size_t w = 0;
        while (w < l.size() && !text::is_space(l[w])) ++w;
        if (w == 0) continue;
        const string_view word = l.substr(0, w);
        const size_t eq = word.find('=', 1);   // `name=alias`: the name is up to the first '=' that something follows
        const string_view name = (eq != string_view::npos && eq + 1 < word.size()) ? word.substr(0, eq) : word;
        size_t p = w;
        const size_t gap = p;
        while (p < l.size() && text::is_space(l[p])) ++p;
        if (p == gap || p >= l.size() || l[p] != '@') continue;
        ++p;
        while (p < l.size() && text::is_space(l[p])) ++p;
        const size_t digits = p;
        while (p < l.size() && l[p] >= '0' && l[p] <= '9') ++p;
        if (p == digits) continue;
        const auto ordinal = text::parse_int<std::uint32_t>(l.substr(digits, p - digits));
        if (ordinal) names.try_emplace(*ordinal, string(name));   // the first line for a number wins
    }
    std::map<string, std::map<std::uint32_t, string>> out;
    if (!names.empty()) out[module] = std::move(names);
    return out;
}

// ---- import libraries -----------------------------------------------------------------------------------------------

std::map<std::string, std::map<std::uint32_t, std::string>> read_import_library(ByteView data) {
    std::map<string, std::map<std::uint32_t, string>> out;
    constexpr string_view kArchive = "!<arch>\n";
    if (data.size() >= 8 && as_text(data.subspan(0, 8)) == kArchive) {
        for (const libraries::coff::Member& member : libraries::coff::members(data)) {
            if (const auto hit = libraries::coff::import_by_ordinal(member.body)) {
                out[module_stem(hit->dll)].try_emplace(hit->ordinal, hit->symbol);
            }
        }
        return out;
    }
    // the 16-bit form: comment records of class A0, kind 1 (an import definition) between the library's own records
    size_t at = 0;
    size_t page = 1;
    while (at + 3 <= data.size()) {
        const std::uint8_t kind = data[at];
        const size_t size = data[at + 1] | (data[at + 2] << 8);
        const size_t body_at = at + 3;
        const ByteView body = body_at >= data.size() ? ByteView() : data.subspan(body_at, std::min(size > 0 ? size - 1 : size, data.size() - body_at));
        at += 3 + size;
        if (kind == 0xF0) {   // the library header gives the page size
            page = size + 3;
        } else if (kind == 0x8A || kind == 0x8B) {   // the end of a member: the next starts on a page
            at += (page - at % page) % page;
        } else if (kind == 0xF1) {
            break;
        } else if (kind == 0x88 && body.size() > 4 && body[1] == 0xA0 && body[2] == 1) {
            const bool by_number = body[3] != 0;
            const size_t n = body[4];
            const size_t q = 5 + n;
            if (q >= body.size()) continue;
            const size_t m = body[q];
            const string internal = text::from_latin1(body.subspan(5, std::min(n, body.size() - 5)));
            const string module = text::upper(text::from_latin1(body.subspan(q + 1, std::min(m, body.size() - (q + 1)))));
            if (by_number && q + 3 + m <= body.size()) {
                const std::uint32_t ordinal = body[q + 1 + m] | (body[q + 2 + m] << 8);
                out[module].try_emplace(ordinal, internal);
            }
        }
    }
    return out;
}

// ---- collect ----------------------------------------------------------------------------------------------------------

std::vector<std::filesystem::path> library_files(std::span<const std::filesystem::path> paths) {
    std::vector<std::filesystem::path> files;
    const std::function<void(const std::filesystem::path&)> walk = [&](const std::filesystem::path& dir) {
        std::vector<std::filesystem::path> here, below;
        std::error_code ec;
        for (const auto& entry : std::filesystem::directory_iterator(dir, ec)) {
            if (entry.is_directory(ec)) {
                const string name = text::upper(fs::to_text(entry.path().filename()));
                if (name != "ALPHA" && name != "MIPS" && name != "PPC") below.push_back(entry.path());
            } else {
                here.push_back(entry.path());
            }
        }
        const auto by_name = [](const auto& a, const auto& b) { return fs::to_text(a.filename()) < fs::to_text(b.filename()); };
        std::sort(here.begin(), here.end(), by_name);
        std::sort(below.begin(), below.end(), by_name);
        files.insert(files.end(), here.begin(), here.end());
        for (const auto& d : below) walk(d);
    };
    for (const auto& p : paths) {
        std::error_code ec;
        if (std::filesystem::is_directory(p, ec)) walk(p);
        else files.push_back(p);
    }
    return files;
}

ShippedTables collect(std::span<const std::filesystem::path> paths, const ModuleExports& exports, const ExportCheck& check,
                      const std::function<void(std::string_view)>& log) {
    const std::set<string> wanted = check.imported_by_number();   // modules something in the corpus imports by number
    ShippedTables out;
    for (const std::filesystem::path& path : library_files(paths)) {
        const string ext = text::lower(fs::to_text(path.extension()));
        std::map<string, std::map<std::uint32_t, string>> found;
        bool archive = false;
        try {   // one file of a batch: a file that cannot be read gives nothing
            if (ext == ".def") {
                const Bytes raw = fs::read_file(path);
                found = read_definition_file(text::from_cp1252(raw), fs::to_text(path.stem()));
            } else if (ext == ".lib") {
                const Bytes raw = fs::read_file(path);
                found = read_import_library(raw);
                archive = raw.size() >= 8 && as_text(ByteView(raw).subspan(0, 8)) == "!<arch>\n";
            }
        } catch (const Error&) {
            found.clear();
        }
        const string file_name = fs::to_text(path.filename());
        for (const auto& [module, names] : found) {
            if (out.contains(module)) continue;   // earlier files win
            const auto own = exports.table(module).value_or(ModuleExports::Table{});
            const auto numbers = check.numbers(module);
            std::map<std::uint32_t, string> keep;
            for (const auto& [ordinal, name] : names) {
                const bool present_known = numbers.has_value();
                const bool present = present_known && numbers->contains(ordinal);
                if (own.names.contains(ordinal) || (present_known && !present) || (!present_known && !wanted.contains(module))) continue;
                keep[ordinal] = name;
            }
            if (keep.empty()) continue;
            std::map<std::uint32_t, std::optional<bool>> checks;
            for (const auto& [ordinal, name] : keep) {
                const auto expect = demangle::stack_bytes(name);
                checks[ordinal] = expect ? check.arguments_agree(module, ordinal, *expect) : std::nullopt;
            }
            if (archive) {   // a 32-bit library holds the linker's symbols
                for (auto& [ordinal, name] : keep) name = demangle::plain(name).name;
            }
            std::uint32_t yes = 0;
            std::uint32_t no = 0;
            for (const auto& [ordinal, verdict] : checks) {
                if (verdict && *verdict) ++yes;
                if (verdict && !*verdict) ++no;
            }
            if (static_cast<std::uint64_t>(no) * 20 > static_cast<std::uint64_t>(yes) + no) {
                if (log) log(std::format("{:<10} NOT USED: {} names {} exports rightly and {} wrongly", module, file_name, yes, no));
                continue;
            }
            ShippedModule m;
            m.from = file_name;
            m.checked_yes = yes;
            m.checked_no = no;
            for (const auto& [ordinal, name] : keep) {
                const auto verdict = checks.at(ordinal);
                if (!(verdict && !*verdict)) m.names[ordinal] = name;   // a name the code disagrees with is dropped
            }
            out[module] = std::move(m);
        }
    }
    if (log) {
        for (const auto& [module, m] : out) {
            log(std::format("{:<10} {:>5} names from {:<14} ({} checked against the code, {} dropped)", module, m.names.size(), m.from,
                            m.checked_yes, m.checked_no));
        }
    }
    return out;
}

}  // namespace cb::symbols::ordinals
