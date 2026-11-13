#include "cb/exact/seed.hpp"

#include <algorithm>
#include <filesystem>
#include <format>
#include <set>
#include <unordered_map>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/exact/names.hpp"

namespace cb::exact::seed {

namespace {

bool sd_word(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }

bool sd_hex_digits(std::string_view s, std::size_t lo, std::size_t hi) {
    if (s.size() < lo || s.size() > hi) return false;
    return std::all_of(s.begin(), s.end(), [](char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'); });
}

// fXXXXX: an address-name of code.
bool sd_code_name(std::string_view n) { return n.size() == 6 && n[0] == 'f' && sd_hex_digits(n.substr(1), 5, 5); }
// g_XXXX: an address-name of data (one to four digits).
bool sd_data_name(std::string_view n) { return n.starts_with("g_") && sd_hex_digits(n.substr(2), 1, 4); }

// The head line of a function: [static] [type] [*]name(parameters) with nothing after the closing parenthesis; group 2 is the name.
const Regex& sd_function_head() {
    static const Regex re(R"(^((?:static\s+)?(?:(?:unsigned|struct\s+\w+|union\s+\w+|[A-Za-z_]\w*)\s+\**\s*)?\**\s*([A-Za-z_]\w*)\s*\([^;{]*\)\s*)$)");
    return re;
}

// Does `name(` occur in `text` (the name whole, blanks allowed before the parenthesis)?
bool sd_calls(std::string_view text, std::string_view name) {
    for (std::size_t at = text.find(name); at != std::string_view::npos; at = text.find(name, at + 1)) {
        if (at > 0 && sd_word(text[at - 1])) continue;
        std::size_t k = at + name.size();
        while (k < text.size() && text::is_space(text[k])) ++k;
        if (k < text.size() && text[k] == '(') return true;
    }
    return false;
}

std::int64_t sd_count(std::string_view s, char c) { return static_cast<std::int64_t>(std::count(s.begin(), s.end(), c)); }

bool sd_starts_with_any(std::string_view s, std::initializer_list<std::string_view> prefixes) {
    return std::any_of(prefixes.begin(), prefixes.end(), [&](std::string_view p) { return s.starts_with(p); });
}

// A source file's text with the line ends a reader of text sees: \r\n and a lone \r are \n.
std::string sd_read_source(const std::filesystem::path& path) {
    const std::string raw = fs::read_text(path);
    std::string text;
    text.reserve(raw.size());
    for (std::size_t i = 0; i < raw.size(); ++i) {
        if (raw[i] == '\r') {
            if (i + 1 < raw.size() && raw[i + 1] == '\n') ++i;
            text.push_back('\n');
        } else {
            text.push_back(raw[i]);
        }
    }
    return text;
}

std::string sd_join_lines(const std::vector<std::string_view>& pending, const std::vector<std::string_view>& lines, std::size_t from, std::size_t to) {
    std::string out;
    for (std::string_view l : pending) {
        out.append(l);
        out.push_back('\n');
    }
    for (std::size_t i = from; i < to; ++i) {
        out.append(lines[i]);
        out.push_back('\n');
    }
    return out;
}

bool sd_is_keyword(std::string_view n) { return n == "if" || n == "while" || n == "for" || n == "switch" || n == "return" || n == "sizeof"; }

std::string sd_without_underscore(std::string_view n) { return std::string(n.starts_with('_') ? n.substr(1) : n); }

}  // namespace

Split split(std::string_view text) {
    const std::vector<std::string_view> lines = text::split(text, '\n');
    Split out;
    std::size_t k = 0;
    std::vector<std::string_view> pending;   // (a remark just before a function: its own)
    const std::size_t n = lines.size();
    while (k < n) {
        const std::string_view l = lines[k];
        std::optional<RegexMatch> head;
        if (!sd_starts_with_any(l, {" ", "\t", "#", "}"})) head = sd_function_head().match(l);
        if (head && !sd_is_keyword(head->group(2))) {
            const std::string name(head->group(2));
            std::size_t j = k;
            while (j < n && !lines[j].starts_with('{')) {   // (K&R: the parameters' declarations)
                if (j > k && text::rtrim(lines[j]).ends_with(';') && sd_starts_with_any(lines[j], {"extern", "static"})) break;
                ++j;
            }
            if (j < n && lines[j].starts_with('{')) {
                std::size_t e = j;
                std::int64_t depth = 0;
                while (e < n) {
                    depth += sd_count(lines[e], '{') - sd_count(lines[e], '}');
                    if ((depth <= 0 && e > j) || (depth <= 0 && text::trim(lines[e]) == "}")) break;
                    ++e;
                }
                out.funcs.insert_or_assign(name, sd_join_lines(pending, lines, k, std::min(e + 1, n)));
                const std::string before = text::replace_all(l.substr(0, l.find(name)), "static", "");
                const std::string type(text::trim(before));
                if (!type.empty() && type != "int") out.types.insert_or_assign(name, type);
                pending.clear();
                k = e + 1;
                continue;
            }
        }
        if (l.starts_with("/*") && !text::rtrim(l).ends_with(';')) {
            pending.assign(1, l);
            while (lines[k].find("*/") == std::string_view::npos && k + 1 < n) {
                ++k;
                pending.push_back(lines[k]);
            }
            ++k;
            continue;
        }
        if (!text::trim(l).empty()) {
            for (std::string_view p : pending) out.decls.emplace_back(p);
            out.decls.emplace_back(l);
        }
        pending.clear();
        ++k;
    }
    return out;
}

std::map<std::int64_t, std::string> seeds(project::Project& project, project::Project& other) {
    const names::Carried carried = names::carry(project, other);
    const names::Known theirs = names::known(other);
    struct SourceFile {
        std::filesystem::path path;
        Split split;
    };
    std::vector<SourceFile> files;
    for (const std::shared_ptr<project::Project>& part : other.sets()) {
        for (const project::Source& s : part->sources()) {
            if (text::iends_with(fs::to_text(s.path), ".c")) files.push_back({s.path, seed::split(sd_read_source(s.path))});
        }
    }
    std::unordered_map<std::string, std::size_t> where;   // function name -> the first file that has it (lookup only)
    for (std::size_t i = 0; i < files.size(); ++i)
        for (const auto& [name, text] : files[i].split.funcs) where.try_emplace(name, i);

    // OTHER's address-names -- for wherever OTHER's names were taken from -- as this program's places.
    std::unordered_map<std::string, std::string> rename;
    OrderedMap<std::int64_t, std::int64_t> by_other;      // the pairs the other way (a place met twice keeps the later one)
    for (const auto& [a, b] : carried.pairs) by_other.insert_or_assign(b, a);
    for (const auto& [b, a] : by_other) {
        const std::string* have = theirs.code.find(b);
        const std::string n = have ? sd_without_underscore(*have) : std::string();
        if (sd_code_name(n)) rename[n] = std::format("f{:05x}", a);
    }
    for (const auto& [a, b] : carried.data_pairs) {
        const std::string* have = theirs.data.find(b);
        const std::string n = have ? sd_without_underscore(*have) : std::string();
        if (sd_data_name(n)) rename[n] = std::format("g_{:04x}", a);
    }
    static const Regex address_names(R"(\b(f[0-9a-f]{5}|g_[0-9a-f]{1,4})\b)");

    std::vector<std::pair<std::int64_t, std::int64_t>> sorted(carried.pairs.begin(), carried.pairs.end());
    std::sort(sorted.begin(), sorted.end());
    std::map<std::int64_t, std::string> out;
    for (const auto& [a, b] : sorted) {
        const std::string fb = std::format("f{:05x}", b);
        const std::string* have = theirs.code.find(b);
        const std::string n = sd_without_underscore(have ? std::string_view(*have) : std::string_view(fb));
        auto at = where.find(n);
        if (at == where.end()) at = where.find(fb);
        if (at == where.end()) continue;
        const SourceFile& file = files[at->second];
        const std::string* fn = file.split.funcs.find(n);
        if (fn == nullptr) fn = file.split.funcs.find(fb);
        if (fn == nullptr) continue;   // (the file has neither: no function to seed)
        std::string text = "/* seeded from " + fs::to_text(other.exe().filename()) + " " + fs::to_text(file.path.lexically_relative(other.dir())) + " */\n";
        std::vector<std::string> head = file.split.decls;   // (the declarations, then the externs of the functions it calls, a line each)
        for (const auto& [f, type] : file.split.types)
            if (f != n && sd_calls(*fn, f)) head.push_back("extern " + type + " " + f + "();");
        text += text::join(head, "\n") + "\n\n" + *fn;
        out[a] = address_names.replace_all(text, [&](const RegexMatch& m) {
            const auto r = rename.find(std::string(m.group(1)));
            return r == rename.end() ? std::string(m.group(1)) : r->second;
        });
    }
    return out;
}

int run(project::Project& project, const std::vector<std::string>& args) {
    const auto from = std::find(args.begin(), args.end(), "--from");
    if (from == args.end() || from + 1 == args.end()) {
        out("seed: --from OTHER (a project of another version of the program, with sources)\n");
        return 1;
    }
    const std::filesystem::path given = fs::from_text(*(from + 1));
    const std::shared_ptr<project::Project> other = project::Project::open(std::filesystem::absolute(given), project.services_ptr());
    const std::map<std::int64_t, std::string> all = seeds(project, *other);
    std::set<std::int64_t> have;
    for (const project::Source& s : project.sources()) have.insert(s.at);
    std::vector<std::pair<std::int64_t, const std::string*>> fresh;
    for (const auto& [a, text] : all)
        if (!have.contains(a)) fresh.emplace_back(a, &text);
    out("{} functions seeded from {} ({} the project has a file for already)\n", all.size(), fs::to_text(other->exe().filename()), all.size() - fresh.size());
    if (std::find(args.begin(), args.end(), "--write") != args.end()) {
        fs::make_dirs(project.src());
        for (const std::shared_ptr<project::Project>& part : other->sets()) {   // (the headers the sources include, when this has none)
            std::error_code ec;
            for (const auto& entry : std::filesystem::directory_iterator(part->src(), ec)) {
                const std::string name = fs::to_text(entry.path().filename());
                const std::string low = text::lower(name);
                const bool header = low.ends_with(".h") || low.ends_with(".inc") || low.ends_with(".lit");
                if (header && entry.is_regular_file() && !std::filesystem::exists(project.path(name))) fs::write_file(project.path(name), fs::read_file(entry.path()));
            }
        }
        for (const auto& [a, text] : fresh) fs::write_text(project.path(std::format("f{:05x}.c", a)), *text);
        out("written: {} files in {}\n", fresh.size(), fs::to_text(project.src()));
    }
    return 0;
}

}  // namespace cb::exact::seed
