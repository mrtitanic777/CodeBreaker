#include "cb/exact/modules.hpp"

#include <algorithm>
#include <format>
#include <string_view>
#include <tuple>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/exact/omf_object.hpp"
#include "cb/formats/omf.hpp"

namespace cb::exact::modules {

namespace stdfs = std::filesystem;
namespace omf = formats::omf;

namespace {

bool is_word(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }
bool is_hex_digit(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }
bool is_lower_hex(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'); }

// A project file as bytes-as-text (no decoding: each byte is one character).
std::string read_source(const stdfs::path& p) {
    const Bytes b = fs::read_file(p);
    return std::string(as_text(b));
}

bool file_exists(const stdfs::path& p) {
    std::error_code ec;
    return stdfs::exists(p, ec);
}

// The text of a line of a project list with its comment and the space around it taken off.
std::string_view without_comment(std::string_view line) { return text::trim(line.substr(0, line.find('#'))); }

// The lines of a project list file.
std::vector<std::string> lines_of(const stdfs::path& p) {
    const std::string content = fs::read_text(p);
    std::vector<std::string> out;
    for (const auto l : text::lines(content)) out.emplace_back(l);
    return out;
}

// Four or five hex digits and nothing else: an address of the 8086's megabyte.
std::optional<std::uint32_t> address_text(std::string_view s) {
    if ((s.size() != 4 && s.size() != 5) || !std::all_of(s.begin(), s.end(), is_hex_digit)) return std::nullopt;
    return text::parse_int<std::uint32_t>(s, 16);
}

// The sorted rows the code-segment parts lines are read as: (address, name, third field).
struct ListedRow {
    std::uint32_t address = 0;
    std::string name, third;
    auto operator<=>(const ListedRow&) const = default;
};
std::vector<ListedRow> listed_rows(const Project& project) {
    std::vector<ListedRow> out;
    const stdfs::path p = project.path("MODULES");
    if (!file_exists(p)) return out;
    for (const std::string& l : lines_of(p)) {
        const auto f = text::split_ws(without_comment(l));
        if (f.size() < 2) continue;
        if (const auto a = address_text(f[1])) out.push_back({*a, std::string(f[0]), f.size() > 2 ? std::string(f[2]) : std::string()});
    }
    std::sort(out.begin(), out.end());
    return out;
}

// ---- the compiler's switches ----------------------------------------------------------------------------------------

// A "segment_switch" template: "{name}" is the segment's name, "{{" and "}}" the braces themselves.
std::string segment_switch(std::string_view pattern, std::string_view name) {
    std::string out;
    for (std::size_t i = 0; i < pattern.size(); ++i) {
        if (pattern.substr(i).starts_with("{{")) { out += '{'; ++i; }
        else if (pattern.substr(i).starts_with("}}")) { out += '}'; ++i; }
        else if (pattern.substr(i).starts_with("{name}")) { out += name; i += 5; }
        else if (pattern[i] == '{' || pattern[i] == '}') throw UsageError("a segment_switch of a toolchain profile has a field other than {name}");
        else out += pattern[i];
    }
    return out;
}

bool calls(std::string_view body, std::string_view name, std::string_view after) {
    const std::string needle = std::string(name) + std::string(after);
    for (std::size_t at = body.find(needle); at != std::string_view::npos; at = body.find(needle, at + 1))
        if (at == 0 || !is_word(body[at - 1])) return true;
    return false;
}

// "f%05x.c" (the function of that address)
std::optional<std::uint32_t> function_address(std::string_view filename) {
    if (filename.size() != 8 || filename[0] != 'f' || !filename.ends_with(".c")) return std::nullopt;
    const auto digits = filename.substr(1, 5);
    if (!std::all_of(digits.begin(), digits.end(), is_lower_hex)) return std::nullopt;
    return text::parse_int<std::uint32_t>(digits, 16);
}

}  // namespace

// ---- src/MODULES and src/CRTLIB ------------------------------------------------------------------------------------

std::optional<std::uint32_t> lib_start(const Project& project) {
    const stdfs::path p = project.path("CRTLIB");
    if (!file_exists(p)) return std::nullopt;
    std::vector<std::uint32_t> at;
    for (const std::string& l : lines_of(p)) {
        if (l.starts_with("#")) continue;
        // "code ADDR" and "code=ADDR": five lowercase hex digits, the word `code` standing alone
        for (std::size_t i = l.find("code"); i != std::string::npos; i = l.find("code", i + 1)) {
            if (i > 0 && is_word(l[i - 1])) continue;
            if (i + 4 + 1 + 5 > l.size() || (l[i + 4] != '=' && l[i + 4] != ' ')) continue;
            const std::string_view digits = std::string_view(l).substr(i + 5, 5);
            if (!std::all_of(digits.begin(), digits.end(), is_lower_hex)) continue;
            if (i + 10 < l.size() && is_word(l[i + 10])) continue;
            at.push_back(*text::parse_int<std::uint32_t>(digits, 16));
        }
        // "NAME [!NAME] # ADDR ..." : a note that begins with its place
        const std::string_view line = l;
        std::size_t i = 0;
        while (i < line.size() && !text::is_space(line[i])) ++i;
        if (i == 0) continue;
        const auto note_at = [&](std::size_t from) -> std::optional<std::uint32_t> {
            std::size_t k = from;
            if (k >= line.size() || !text::is_space(line[k])) return std::nullopt;
            while (k < line.size() && text::is_space(line[k])) ++k;
            if (k >= line.size() || line[k] != '#') return std::nullopt;
            ++k;
            while (k < line.size() && text::is_space(line[k])) ++k;
            if (k + 5 > line.size() || !std::all_of(line.begin() + static_cast<std::ptrdiff_t>(k), line.begin() + static_cast<std::ptrdiff_t>(k + 5), is_lower_hex))
                return std::nullopt;
            if (k + 5 < line.size() && is_word(line[k + 5])) return std::nullopt;
            return text::parse_int<std::uint32_t>(line.substr(k, 5), 16);
        };
        std::size_t k = i;   // (the optional "!NAME" first)
        std::size_t j = k;
        while (j < line.size() && text::is_space(line[j])) ++j;
        std::optional<std::uint32_t> found;
        if (j > k && j < line.size() && line[j] == '!') {
            std::size_t e = j;
            while (e < line.size() && !text::is_space(line[e])) ++e;
            if (e > j + 1) found = note_at(e);
        }
        if (!found) found = note_at(i);
        if (found) at.push_back(*found);
    }
    if (at.empty()) return std::nullopt;
    return *std::min_element(at.begin(), at.end());
}

std::vector<Row> rows(const Project& project) {
    const stdfs::path p = project.path("MODULES");
    if (!file_exists(p)) return {};
    std::vector<Row> listed;
    std::size_t line_number = 0;
    for (const std::string& l : lines_of(p)) {
        ++line_number;
        const std::string_view line = without_comment(l);
        if (line.empty()) continue;
        // name, code start, and the rest (data start and switches)
        std::size_t i = 0;
        const auto token = [&]() {
            while (i < line.size() && text::is_space(line[i])) ++i;
            const std::size_t start = i;
            while (i < line.size() && !text::is_space(line[i])) ++i;
            return line.substr(start, i - start);
        };
        const std::string_view name = token(), code = token();
        if (name.empty() || code.empty()) throw ParseError(fs::to_text(p), static_cast<std::uint32_t>(line_number), 0, "a module line needs a name and a code start");
        Row row;
        row.name = std::string(name);
        if (code != "-") {
            row.code_start = text::parse_int<std::uint32_t>(code, 16);
            if (!row.code_start) throw ParseError(fs::to_text(p), static_cast<std::uint32_t>(line_number), 0, "the code start is not a hex address");
        }
        const std::string_view rest = text::trim(line.substr(i));
        // a data start: four or five hex digits standing alone as the first word of the rest
        std::size_t word_end = 0;
        while (word_end < rest.size() && is_word(rest[word_end])) ++word_end;
        if (const auto data = address_text(rest.substr(0, word_end)); data && std::all_of(rest.begin(), rest.begin() + static_cast<std::ptrdiff_t>(word_end), is_hex_digit)) {
            row.data_start = data;
            const std::string_view sw = text::trim(rest.substr(word_end));
            if (!sw.empty()) row.switches = std::string(sw);
        } else if (!rest.empty()) {
            row.switches = std::string(rest);
        }
        listed.push_back(std::move(row));
    }
    std::vector<std::uint32_t> code;
    for (const Row& r : listed)
        if (r.code_start) code.push_back(*r.code_start);
    if (const auto lib = lib_start(project)) code.push_back(*lib);   // (the library's part ends the module before it: the program's own modules may go on after it)
    std::sort(code.begin(), code.end());
    code.erase(std::unique(code.begin(), code.end()), code.end());
    std::vector<Row> out;
    for (Row& r : listed) {
        if (r.switches && r.switches->starts_with("@")) continue;   // (a part of the file named, in a code segment of its own: compiled with it - see parts)
        if (r.code_start) {
            const auto later = std::upper_bound(code.begin(), code.end(), *r.code_start);
            if (later != code.end()) r.code_end = *later;
            else if (project.code_end() != 0 && project.code_end() > *r.code_start) r.code_end = project.code_end();
            else r.code_end = project.own_end();
        }
        out.push_back(std::move(r));
    }
    return out;
}

std::vector<Part> parts(const Project& project, std::string_view module) {
    const std::vector<ListedRow> listed = listed_rows(project);
    std::vector<Part> out;
    for (std::size_t k = 0; k < listed.size(); ++k) {
        const ListedRow& r = listed[k];
        if (r.name == module && r.third.starts_with("@"))
            out.push_back({r.address, k + 1 < listed.size() ? listed[k + 1].address : project.own_end(), r.third.substr(1)});
    }
    return out;
}

std::vector<std::uint32_t> starts(const Project& project) {
    std::vector<std::uint32_t> out;
    for (const ListedRow& r : listed_rows(project)) out.push_back(r.address);
    return out;   // (already ascending)
}

std::vector<stdfs::path> files_of(const stdfs::path& src, std::uint32_t start, std::uint32_t end) {
    struct Found {
        std::uint32_t address;
        std::size_t kind;   // d (what the file defines before the function), f (the function), e (after it)
        stdfs::path path;
    };
    std::vector<Found> found;
    std::error_code ec;
    for (const auto& entry : stdfs::directory_iterator(src, ec)) {
        const std::string name = entry.path().filename().string();
        if (name.size() != 8 || !name.ends_with(".c") || (name[0] != 'd' && name[0] != 'f' && name[0] != 'e')) continue;
        const std::string_view digits = std::string_view(name).substr(1, 5);
        if (!std::all_of(digits.begin(), digits.end(), is_lower_hex)) continue;
        const std::uint32_t a = *text::parse_int<std::uint32_t>(digits, 16);
        if (start <= a && a < end) found.push_back({a, std::string_view("dfe").find(name[0]), entry.path()});
    }
    std::sort(found.begin(), found.end(), [](const Found& x, const Found& y) {
        return std::tie(x.address, x.kind, x.path) < std::tie(y.address, y.kind, y.path);
    });
    std::vector<stdfs::path> out;
    for (Found& f : found) out.push_back(std::move(f.path));
    return out;
}

// ---- putting a module together -------------------------------------------------------------------------------------

FunctionTypes FunctionTypes::read(const stdfs::path& src) {
    // "<type>\nf12345(" at the start of a line: the type a function returns, on the line before its name
    static const Regex declaration(R"(^((?:unsigned \w+|unsigned|struct \w+|union \w+|\w+)(?: ?\*+)?)\n(f[0-9a-f]{5})\()", RegexFlags::Multiline);
    FunctionTypes out;
    std::vector<stdfs::path> files;
    std::error_code ec;
    for (const auto& entry : stdfs::directory_iterator(src, ec)) {
        const std::string name = entry.path().filename().string();
        if (name.size() > 2 && name[0] == 'f' && name.ends_with(".c")) files.push_back(entry.path());
    }
    std::sort(files.begin(), files.end());
    for (const stdfs::path& p : files) {
        const std::string t = read_source(p);
        declaration.for_each_match(t, [&](const RegexMatch& m) {
            const std::string ty(text::trim(m.group(1)));
            if (ty == "int" || ty == "static" || ty.starts_with("static") || ty.starts_with("extern")) return true;
            out.types_.insert_or_assign(std::string(m.group(2)), ty);
            return true;
        });
    }
    return out;
}

namespace {

// An #include's file as compared: the line without its comment, the spaces and the quotes or brackets, in lower case.
std::string include_key(std::string_view line) {
    const std::string_view head = line.substr(0, line.find("/*"));
    std::string key;
    for (const char c : head.substr(std::min<std::size_t>(8, head.size())))
        if (!text::is_space(c) && c != '"' && c != '<' && c != '>') key += c;
    return text::lower(key);
}

// "/* TAG: switches */\n" at the start of a text: the switches, and where the comment ends.
std::optional<std::pair<std::string_view, std::size_t>> header_switches(std::string_view t, std::string_view tag) {
    const std::string prefix = "/* " + std::string(tag) + ": ";
    if (!t.starts_with(prefix)) return std::nullopt;
    const std::size_t star = t.find('*', prefix.size());   // (the switches hold no '*')
    if (star == std::string_view::npos || star == 0 || t[star - 1] != ' ' || star - 1 < prefix.size() || !t.substr(star).starts_with("*/\n"))
        return std::nullopt;
    return std::make_pair(t.substr(prefix.size(), star - 1 - prefix.size()), star + 3);
}

}  // namespace

Combined combine(const std::vector<stdfs::path>& paths, std::string_view default_switches, std::optional<std::string_view> header_tag,
                 const std::map<stdfs::path, std::string>& before, const FunctionTypes& types) {
    // what a pass asked of a function: "/* TAG: 1, 2 */"; the module compiles as it is, no pass's askings answered
    static const Regex asking(R"(/\* [A-Z]+: [0-9, ]+ \*/\n)");
    // the empty stand-ins of the file's other functions ("/* SAMEFILE */"): declared instead - they are here now
    static const Regex stand_in_declaration(R"(/\* SAMEFILE \*/\n([^\n{;]*\([^)]*\))\s*\{\s*\})");
    static const Regex stand_in_text(R"(/\* SAMEFILE \*/\n[^\n{;]*\([^)]*\)\s*\{\s*\}\n\n?)");
    static const Regex parameters(R"(\(.*\))");
    static const Regex trailing_comment(R"(\s*/\*.*\*/\s*$)");

    std::vector<std::string> includes, externs, bodies;
    std::vector<std::string> include_keys;
    std::set<std::string> seen, flags;
    for (const stdfs::path& p : paths) {
        std::string t = text::replace_all(read_source(p), "\r\n", "\n");
        std::string_view body = t;
        const auto header = header_tag ? header_switches(t, *header_tag) : std::nullopt;
        if (p.filename().string().starts_with("f")) flags.insert(header ? std::string(header->first) : std::string(default_switches));
        if (header) t = t.substr(header->second);
        t = asking.replace_all(t, "");
        stand_in_declaration.for_each_match(t, [&](const RegexMatch& m) {
            const std::string key = "extern " + std::string(text::trim(parameters.replace_all(std::string(m.group(1)), "()"))) + ";";
            if (seen.insert(key).second) externs.push_back(key);
            return true;
        });
        t = stand_in_text.replace_all(t, "");
        body = t;
        const std::vector<std::string_view> lines = text::split(body, '\n');
        std::size_t k = 0;
        std::vector<std::string_view> pre;
        while (k < lines.size()) {
            const std::string_view l = lines[k];
            if (l.starts_with("/*")) {
                for (;; ++k) {
                    pre.push_back(lines[k]);
                    if (lines[k].find("*/") != std::string_view::npos) break;
                    if (k + 1 >= lines.size()) throw ParseError(fs::to_text(p), static_cast<std::uint32_t>(k + 1), 0, "a comment at the top of the file is never closed");
                }
            } else if (l.starts_with("#include")) {   // (once each file, however the line is written)
                const std::string key = include_key(l);
                if (std::find(include_keys.begin(), include_keys.end(), key) == include_keys.end()) {
                    include_keys.push_back(key);
                    includes.emplace_back(l);
                }
            } else if (l.starts_with("extern ") && l.find('{') == std::string_view::npos &&
                       text::trim(trailing_comment.replace_first(std::string(l), "")).ends_with(";")) {
                const std::string key(text::trim(trailing_comment.replace_first(std::string(l), "")));
                if (seen.insert(key).second) externs.emplace_back(l);
            } else if (!text::trim(l).empty()) {
                break;
            }
            ++k;
        }
        std::string joined;
        if (const auto it = before.find(p); it != before.end()) joined = it->second;
        std::vector<std::string_view> rest = pre;
        rest.insert(rest.end(), lines.begin() + static_cast<std::ptrdiff_t>(std::min(k, lines.size())), lines.end());
        joined += text::join(rest, "\n");
        bodies.emplace_back(text::trim(joined, "\n"));
    }
    const std::string have = text::join(externs, "\n");
    const std::string body = text::join(bodies, "\n");
    std::vector<std::string> forward;
    for (const auto& [fn, ty] : types.all()) {
        if (!calls(body, fn, "(") || calls(have + body, fn, "()")) continue;
        const std::size_t stars = ty.size() - ty.find_last_not_of('*') - 1;
        forward.push_back(std::format("extern {} {}{}();", text::trim(std::string_view(ty).substr(0, ty.find_last_not_of('*') + 1)), std::string(stars, '*'), fn));
    }
    std::vector<std::string> head = includes;
    head.insert(head.end(), externs.begin(), externs.end());
    head.insert(head.end(), forward.begin(), forward.end());
    return {text::join(head, "\n") + "\n\n" + text::join(bodies, "\n\n") + "\n", std::move(flags)};
}

std::optional<std::string> first_segment(const Project& project, std::optional<std::uint32_t> start) {
    const auto name = project.config_text("first_segment");
    if (!name || name->empty() || !start) return std::nullopt;
    std::optional<std::uint32_t> lowest_later;
    for (const std::uint32_t s : project.code_segments())
        if (s > 0 && (!lowest_later || s < *lowest_later)) lowest_later = s;
    if (!lowest_later || *start < *lowest_later) return name;
    return std::nullopt;
}

std::optional<Source> source(const Project& project, const Row& row, const FunctionTypes& types) {
    if (text::iends_with(row.name, ".obj")) return std::nullopt;
    const stdfs::path whole = project.src() / row.name;
    if (file_exists(whole)) {   // (a file as it is: of data only, or an assembler's)
        std::string t = read_source(whole);
        const auto seg = first_segment(project, row.code_start);
        if (seg && !text::iends_with(row.name, ".c")) t = text::replace_all(t, "_TEXT", *seg);   // (the first code segment's name, in the assembly too)
        return Source{std::move(t), {}, {}};
    }
    std::vector<stdfs::path> paths;
    if (row.code_start) paths = files_of(project.src(), *row.code_start, row.code_end.value_or(0));
    const stdfs::path data = project.src() / (stdfs::path(row.name).stem().string() + "-data.c");
    if (file_exists(data)) paths.push_back(data);

    static const Regex definition(R"(^([A-Za-z][^;(\n]*?\b(\w+))\([^)\n]*\)\s*$)", RegexFlags::Multiline);
    std::map<stdfs::path, std::string> before;
    const std::vector<Part> other_segments = parts(project, row.name);
    for (const Part& part : other_segments) {   // (the functions of another code segment, after a pragma)
        const std::vector<stdfs::path> in_part = files_of(project.src(), part.start, part.end);
        std::vector<std::string> fns;
        for (const stdfs::path& q : in_part) {
            definition.for_each_match(read_source(q), [&](const RegexMatch& m) {
                const std::string fn(m.group(2));
                if (std::find(fns.begin(), fns.end(), fn) == fns.end()) fns.push_back(fn);
                return true;
            });
        }
        if (!in_part.empty()) before[in_part.front()] = std::format("#pragma alloc_text({}, {})\n\n", part.segment, text::join(fns, ", "));
        paths.insert(paths.end(), in_part.begin(), in_part.end());
    }
    const auto tc = project.toolchain();
    const std::string default_switches = project.switches().value_or(tc->profile("switches").value_or(""));
    const std::optional<std::string> file_switches = tc->profile("file_switches");
    Combined combined = combine(paths, default_switches, file_switches ? std::optional<std::string_view>(*file_switches) : std::nullopt, before, types);
    std::string fl = row.switches ? *row.switches : (combined.switches.empty() ? default_switches : *combined.switches.begin());
    if (const auto seg = first_segment(project, row.code_start)) {
        if (const auto sw = tc->profile("segment_switch"); sw && !sw->empty()) fl = (fl.empty() ? "" : fl + " ") + segment_switch(*sw, *seg);   // (a module of the first code segment: its name given)
    }
    std::string text_out = std::move(combined.text);
    if (file_switches && !file_switches->empty() && !fl.empty()) text_out = "/* " + *file_switches + ": " + fl + " */\n" + text_out;
    std::vector<std::uint32_t> addrs;
    for (const stdfs::path& p : paths) {
        const auto a = function_address(p.filename().string());
        if (!a) continue;
        const bool in_part = std::any_of(other_segments.begin(), other_segments.end(), [&](const Part& q) { return q.start <= *a && *a < q.end; });
        if (!in_part) addrs.push_back(*a);   // (the functions of the main code segment: another one's are not made stand-ins)
    }
    return Source{std::move(text_out), std::move(addrs), std::move(combined.switches)};
}

SetOf set_of(const Project& project, std::string_view module) {
    for (const auto& part : project.also_sets()) {
        for (const char* key : {"unplaced", "sources"}) {
            const auto pattern = part->config_text(key);
            if (pattern && !pattern->empty() && Regex(*pattern, RegexFlags::IgnoreCase).full_match(module)) return {part, part->toolchain()};
        }
    }
    return {nullptr, project.toolchain()};
}

Compiled compile_module(const Project& project, const Row& row, const FunctionTypes& types, const Toolchain* toolchain) {
    const auto s = source(project, row, types);
    if (!s) return {};
    const Project* set = &project;
    std::shared_ptr<const Project> own;
    std::shared_ptr<const Toolchain> chosen;
    const Toolchain* tc = toolchain;
    if (file_exists(project.src() / row.name)) {   // (a whole file: by the toolchain of the set it is of)
        SetOf found = set_of(project, row.name);
        if (found.own) {
            own = found.own;
            set = own.get();
            chosen = found.toolchain;
            tc = chosen.get();
        } else if (!tc) {
            chosen = found.toolchain;
            tc = chosen.get();
        }
    }
    if (!tc) {
        chosen = project.toolchain();
        tc = chosen.get();
    }
    const CompileOptions options{set->headers(), set->include(), set->switches()};
    CompileResult r = tc->compile(as_bytes(s->text), options);
    return {std::move(r.object), std::move(r.console), s->addresses};
}

// ---- checking ------------------------------------------------------------------------------------------------------

CheckResult check(const Project& project, const Row& row, const FunctionTypes& types, const std::optional<Bytes>& object,
                  const x86::Decoder16& decoder) {
    CheckResult out;
    if (text::iends_with(row.name, ".obj")) {
        out.kit_object = true;
        return out;
    }
    Bytes raw;
    if (object) {
        raw = *object;
    } else {
        Compiled c = compile_module(project, row, types);
        if (!c.object) {
            out.error = c.console;
            return out;
        }
        raw = std::move(*c.object);
    }
    const omf::Object parsed = omf::read(raw);
    const ObjectFile o = object_of(parsed, raw);
    const ByteView img = project.image();
    SymbolTable syms = project.symbols();
    SymbolTable learnt;
    const compare::Program program = project.program();
    const std::vector<std::string> code = omf::code_segments(parsed);
    const std::string text_segment = code.empty() ? "_TEXT" : code.front();
    for (const omf::FunctionSpan& fn : omf::functions(parsed, text_segment)) {
        FunctionResult r;
        r.name = fn.name;
        r.length = static_cast<std::uint32_t>(fn.length);
        std::optional<std::uint32_t> a;
        const std::string_view n = fn.name;
        const std::string_view bare = n.starts_with("_") ? n.substr(1) : n;
        if (bare.size() == 6 && bare[0] == 'f' && std::all_of(bare.begin() + 1, bare.end(), is_lower_hex)) {
            a = text::parse_int<std::uint32_t>(bare.substr(1), 16);
        } else if (const SymPlace* place = syms.find(fn.name); place && place->kind == SymKind::Code) {
            a = static_cast<std::uint32_t>(place->address);
        }
        if (!a) {
            r.verdict = Verdict::NoPlace;
            out.functions.push_back(std::move(r));
            continue;
        }
        r.address = a;
        const compare::Subject subject{o, fn.name, static_cast<std::uint32_t>(fn.offset), static_cast<std::uint32_t>(fn.length), *a, text_segment};
        const compare::Report report = compare::function(subject, img, program, syms, learnt, nullptr, decoder);
        r.verdict = report.identical() ? Verdict::Same : Verdict::Differs;
        r.differing_bytes = report.bad.size();
        out.functions.push_back(std::move(r));
    }
    // the initialized data, with the data segment: word for word where the linker fills a word
    ByteView d;
    if (row.data_start) {
        if (const Bytes* data = o.seg.find("_DATA")) {
            const std::uint32_t* filled = o.filled.find("_DATA");
            d = ByteView(*data).first(std::min<std::size_t>(filled ? *filled : 0, data->size()));
        }
    }
    out.data_length = static_cast<std::uint32_t>(d.size());
    const std::uint32_t dstart = row.data_start.value_or(0);
    // the program's data segment (a slice that starts where the data segment does, up to the image's end)
    ByteView ds;
    if (program.dgroup >= 0 && static_cast<std::uint64_t>(program.dgroup) < img.size()) ds = img.subspan(static_cast<std::size_t>(program.dgroup));
    std::map<std::uint32_t, const Fixup*> holes;   // lookup by offset only
    for (const Fixup& f : o.fix)
        if (f.segment == "_DATA" && f.kind == FixKind::Off) holes[f.offset] = &f;   // (a later fix-up of the same word wins)
    const auto word = [](ByteView b, std::size_t at) { return static_cast<std::uint16_t>(b[at] | (b[at + 1] << 8)); };
    std::size_t k = 0;
    while (k < d.size()) {
        if (const auto hole = holes.find(static_cast<std::uint32_t>(k)); hole != holes.end()) {
            const Fixup& f = *hole->second;
            if (k + 2 > d.size()) {   // (a fix-up the object's data cannot hold: the object is damaged)
                out.data_differences.push_back(std::format("{:04x}: a fix-up word past the object's data", dstart + k));
                break;
            }
            if (dstart + k + 2 > ds.size()) {
                out.data_differences.push_back(std::format("{:04x}: past the program's data", dstart + k));
                k += 2;
                continue;
            }
            const std::uint16_t theirs = word(ds, dstart + k);
            const std::uint16_t mine = static_cast<std::uint16_t>(word(d, k) + f.displacement);
            if (f.target.kind != TargetKind::External && f.target.name == "_DATA" && theirs != static_cast<std::uint16_t>(dstart + mine))
                out.data_differences.push_back(std::format("{:04x}: -> {:04x}, the program's -> {:04x}", dstart + k, static_cast<std::uint16_t>(dstart + mine), theirs));
            k += 2;
            continue;
        }
        if (dstart + k >= ds.size())
            out.data_differences.push_back(std::format("{:04x}: {:02x}, past the program's data", dstart + k, d[k]));
        else if (ds[dstart + k] != d[k])
            out.data_differences.push_back(std::format("{:04x}: {:02x}, the program's {:02x}", dstart + k, d[k], ds[dstart + k]));
        ++k;
    }
    out.object = std::move(raw);
    return out;
}

int run(const Project& project, const std::vector<std::string>& args, const x86::Decoder16& decoder) {
    std::vector<std::string> want;
    bool verbose = false;
    for (const std::string& a : args) {
        if (a.starts_with("-")) verbose = verbose || a == "-v";
        else want.push_back(a);
    }
    const FunctionTypes types = FunctionTypes::read(project.src());
    std::size_t total = 0, same = 0;
    for (const Row& row : rows(project)) {
        const std::string stem = stdfs::path(row.name).stem().string();
        if (!want.empty() && std::find(want.begin(), want.end(), row.name) == want.end() && std::find(want.begin(), want.end(), stem) == want.end()) continue;
        const CheckResult j = check(project, row, types, std::nullopt, decoder);
        if (j.kit_object) {
            out("{:<14} the kit's object\n", row.name);
            continue;
        }
        if (j.error) {
            const auto last = text::lines(text::trim(*j.error));
            out("{:<14} does not compile: {}\n", row.name, last.empty() ? std::string_view() : last.back());
            continue;
        }
        const std::size_t identical = static_cast<std::size_t>(std::count_if(j.functions.begin(), j.functions.end(), [](const FunctionResult& f) { return f.verdict == Verdict::Same; }));
        total += j.functions.size();
        same += identical;
        out("{:<14} {}  {:3} functions, {:3} identical; data {:04x}..{:04x} {}\n", row.name,
            row.code_start ? std::format("{:05x}", *row.code_start) : std::string("  -  "), j.functions.size(), identical,
            row.data_start.value_or(0), row.data_start.value_or(0) + j.data_length,
            j.data_differences.empty() ? std::string("IDENTICAL") : std::format("{} differ", j.data_differences.size()));
        if (verbose) {
            for (const FunctionResult& f : j.functions) {
                if (f.verdict == Verdict::Same) continue;
                const std::string verdict = f.verdict == Verdict::NoPlace ? "no place" : std::format("differs {}", f.differing_bytes);
                out("     {:<12} {} {}\n", f.name, f.address ? std::format("{:05x}", *f.address) : std::string("?"), verdict);
            }
            for (std::size_t i = 0; i < std::min<std::size_t>(20, j.data_differences.size()); ++i) out("     data {}\n", j.data_differences[i]);
        }
    }
    out("{} functions in the modules, {} identical\n", total, same);
    return 0;
}

}  // namespace cb::exact::modules
