// app/tools.cpp -- see tools.hpp: the table of tools, the spec parsers, search, the form's check, the command line it builds, the result folder of a
// run and the work folder's modules.
#include "app/tools.hpp"

#include <algorithm>
#include <cctype>
#include <set>
#include <system_error>

#include "core/fs.hpp"
#include "core/text.hpp"

namespace cb::app::tools {

namespace {

namespace stdfs = std::filesystem;

struct ToolRow {
    std::string_view id, title, group, description, inputs, choice, argv;
    bool developer;
};

constexpr ToolRow kToolRows[] = {
#include "tools_catalog.inc"
};

// The commands of the table that are no tool's command: why each makes no sense as a drop-and-Run form.
const std::vector<LeftOut> kLeftOut = {
    {"app", "it is the window itself"},
    {"mcp", "it is a server that talks to an assistant over its standard input and output, which a form cannot be on the other end of"},
    {"nasm", "it is the assembler for scripts and tests, with NASM's own command line"},
    {"disassemble", "it reads at an address or with a processor a person types (the command line and the assistant's disasm have it)"},
    {"assemble", "it needs the processor the source is for, typed (the command line has it; Rebuild from source assembles a source folder)"},
    {"compile", "it takes the compiler's own options, typed (the command line has it; Rebuild from source compiles a source folder)"},
    {"link", "it takes the linker's own directives, typed (the command line has it; Rebuild from source links a source folder)"},
    {"dos-run", "it runs a DOS program with the program's own command line, for scripts and checks"},
    {"demangle", "it reads a C++ name a person types, not a file (the command line and the assistant tools have it)"},
    {"dos-interrupts", "it names a service from numbers a person types, not a file (the command line has it)"},
    {"lookup", "it looks up a name a person types, not a file (the command line has it)"},
    {"crypto", "its tools each take their own console keys, images and values (the command line has them)"},
    {"xex-edit", "an edit needs the flags or the address and bytes to change, which are typed (the command line has it)"},
    {"ne-asm", "its tools each take their own arguments (the command line has them)"},
    {"media", "Disc images (disc) takes an image apart through the same readers, beside the image"},
    {"versions", "it is Compare two builds (compare), under the name the command line knows it by now"},
    {"corpus", "its steps are tools of their own here (Rebuild everything, Watch a rebuild, Scoreboard, Coverage ...)"},
    {"xbox", "its steps are tools of their own here (Xbox 360 program details, Recompiler configuration ...)"},
};

constexpr const char* kGroups[] = {"Convert", "Look inside", "Extract", "Build", "Compare", "Source", "Inspect", "Check", "Collection", "Maintenance"};
constexpr std::size_t kEverydayGroups = 5;

std::string tl_trim(std::string_view s) { return std::string(text::trim(s)); }

// {1} {2} {choice} {dir1} {stem1} {name1} {dir2} ... in `word`.
std::string tl_fill(std::string_view word, const Values& v, const Tool& tool) {
    std::string out;
    std::size_t i = 0;
    while (i < word.size()) {
        if (word[i] != '{') {
            out += word[i++];
            continue;
        }
        const std::size_t close = word.find('}', i);
        if (close == std::string_view::npos) {
            out += word.substr(i);
            break;
        }
        const std::string key(word.substr(i + 1, close - i - 1));
        i = close + 1;
        if (key == "choice") {
            if (v.choice >= 0 && static_cast<std::size_t>(v.choice) < tool.choices.size()) out += tool.choices[static_cast<std::size_t>(v.choice)].value;
            continue;
        }
        const char n = key.empty() ? '0' : key.back();
        const std::size_t index = n == '2' ? 1 : 0;
        const std::string value = index < v.inputs.size() ? tl_trim(v.inputs[index]) : std::string();
        const std::string base = key.substr(0, key.size() - 1);
        if (key == "1" || key == "2") {
            out += value;
        } else if (!value.empty()) {
            const stdfs::path p = stdfs::path(fs::from_text(value)).lexically_normal();
            if (base == "dir") out += fs::to_text(p.parent_path().empty() ? stdfs::path(".") : p.parent_path());
            else if (base == "stem") out += fs::to_text(p.stem());
            else if (base == "name") out += fs::to_text(p.filename());
        }
    }
    return out;
}

bool tl_is_program(const stdfs::path& p) {
    static const std::set<std::string, std::less<>> kExt = {".exe", ".dll", ".drv", ".vxd", ".386", ".cpl", ".scr", ".sys", ".com", ".ocx", ".acm",
                                                             ".tsk", ".mod", ".fon", ".xex", ".ax", ".tlb"};
    return kExt.contains(text::lower(fs::to_text(p.extension())));
}

}  // namespace

const std::vector<std::string>& groups() {
    static const std::vector<std::string> list(std::begin(kGroups), std::end(kGroups));
    return list;
}

bool is_developer_group(std::string_view group) {
    for (std::size_t i = kEverydayGroups; i < std::size(kGroups); ++i)
        if (group == kGroups[i]) return true;
    return false;
}

const std::vector<LeftOut>& left_out() { return kLeftOut; }

std::vector<Input> parse_inputs(std::string_view spec) {
    std::vector<Input> out;
    for (const std::string_view part : text::split(spec, ';')) {
        const std::string_view p = text::trim(part);
        if (p.size() < 3 || p[1] != ' ') continue;
        Input in;
        switch (p[0]) {
            case 'F': in.kind = InputKind::File; break;
            case 'D': in.kind = InputKind::Folder; break;
            case 'E': in.kind = InputKind::Either; break;
            case 'M': in.kind = InputKind::Module; break;
            default: continue;
        }
        const std::string rest = tl_trim(p.substr(2));
        const std::size_t colon = rest.find(':');
        in.label = tl_trim(rest.substr(0, colon));
        if (colon != std::string::npos) in.accepts = tl_trim(rest.substr(colon + 1));
        if (!in.label.empty()) out.push_back(std::move(in));
    }
    return out;
}

std::string parse_choices(std::string_view spec, std::vector<Choice>& out) {
    out.clear();
    const std::size_t eq = spec.find('=');
    if (eq == std::string_view::npos) return {};
    for (const std::string_view part : text::split(spec.substr(eq + 1), '|')) {
        const std::size_t colon = part.find(':');
        if (colon == std::string_view::npos) continue;
        out.push_back({tl_trim(part.substr(0, colon)), tl_trim(part.substr(colon + 1))});
    }
    return out.empty() ? std::string() : tl_trim(spec.substr(0, eq));
}

const std::vector<Tool>& all() {
    static const std::vector<Tool> list = [] {
        std::vector<Tool> tools;
        for (const std::string& group : groups()) {
            for (const ToolRow& row : kToolRows) {
                if (row.group != group) continue;
                Tool t;
                t.id = std::string(row.id);
                t.title = std::string(row.title);
                t.group = std::string(row.group);
                t.description = std::string(row.description);
                t.inputs = parse_inputs(row.inputs);
                t.choice_label = parse_choices(row.choice, t.choices);
                for (const std::string_view w : text::split_ws(row.argv)) t.argv.emplace_back(w);
                t.developer = row.developer;
                tools.push_back(std::move(t));
            }
        }
        return tools;
    }();
    return list;
}

const Tool* find(std::string_view id) {
    for (const Tool& t : all())
        if (t.id == id) return &t;
    return nullptr;
}

bool matches(const Tool& tool, std::string_view query) {
    const std::string hay = text::lower(tool.title + " " + std::string(tool.command()) + " " + tool.description);
    const std::string wanted = text::lower(query);
    for (const std::string_view word : text::split_ws(wanted))
        if (hay.find(word) == std::string::npos) return false;
    return true;
}

std::vector<const Tool*> search(std::string_view query, bool developer) {
    std::vector<const Tool*> out;
    for (const Tool& t : all())
        if ((developer || !t.developer) && matches(t, query)) out.push_back(&t);
    return out;
}

Values empty_values(const Tool& tool) {
    Values v;
    v.inputs.resize(tool.inputs.size());
    return v;
}

std::optional<std::string> missing(const Tool& tool, const Values& values) {
    for (std::size_t i = 0; i < tool.inputs.size(); ++i)
        if (i >= values.inputs.size() || text::trim(values.inputs[i]).empty()) return tool.inputs[i].label;
    return std::nullopt;
}

std::vector<std::string> build_argv(const Tool& tool, const Values& values) {
    std::vector<std::string> argv;
    for (const std::string& word : tool.argv) {
        const bool placeholder = word.find('{') != std::string::npos;
        std::string filled = tl_fill(word, values, tool);
        if (placeholder && filled.empty()) {   // nothing to say: the option that introduces it goes too
            if (!argv.empty() && argv.back().starts_with('-') && argv.size() > 1) argv.pop_back();
            continue;
        }
        argv.push_back(std::move(filled));
    }
    return argv;
}

bool accepts(const Input& input, bool is_folder) {
    switch (input.kind) {
        case InputKind::File: return !is_folder;
        case InputKind::Folder: return is_folder;
        case InputKind::Either: return true;
        case InputKind::Module: return false;
    }
    return false;
}

std::filesystem::path result_folder(std::string_view output) {
    std::error_code ec;
    const auto folder_of = [&](const stdfs::path& p) -> stdfs::path {
        if (stdfs::is_directory(p, ec)) return p;
        if (stdfs::is_regular_file(p, ec)) return p.parent_path();
        return {};
    };
    std::vector<std::string_view> lines = text::split(output, '\n');
    for (auto it = lines.rbegin(); it != lines.rend(); ++it) {   // "Output: <path>"
        const std::string_view line = text::trim(*it);
        if (line.starts_with("Output: ")) {
            const stdfs::path p = folder_of(fs::from_text(tl_trim(line.substr(8))));
            if (!p.empty()) return p;
        }
    }
    for (auto it = lines.rbegin(); it != lines.rend(); ++it) {   // the last absolute path that is there
        const std::string_view line = *it;
        for (std::size_t i = 0; i + 2 < line.size(); ++i) {
            const bool drive = std::isalpha(static_cast<unsigned char>(line[i])) && line[i + 1] == ':' && (line[i + 2] == '\\' || line[i + 2] == '/') &&
                               (i == 0 || !std::isalnum(static_cast<unsigned char>(line[i - 1])));
            const bool root = line[i] == '/' && (i == 0 || line[i - 1] == ' ') && i + 1 < line.size() && std::isalpha(static_cast<unsigned char>(line[i + 1]));
            if (!drive && !root) continue;
            // the longest prefix up to a quote, a parenthesis or the end of the line that is there (a path may hold spaces)
            std::size_t end = line.size();
            for (const char stop : {'"', '\'', '(', ')', '\t', ','})
                if (const std::size_t at = line.find(stop, i); at != std::string_view::npos) end = std::min(end, at);
            std::string candidate = tl_trim(line.substr(i, end - i));
            while (!candidate.empty()) {
                while (!candidate.empty() && (candidate.back() == '.' || candidate.back() == ':' || candidate.back() == ' ')) candidate.pop_back();
                const stdfs::path p = folder_of(fs::from_text(candidate));
                if (!p.empty()) return p;
                const std::size_t space = candidate.rfind(' ');
                if (space == std::string::npos) break;
                candidate.resize(space);
            }
        }
    }
    return {};
}

std::vector<std::string> modules_in(const std::filesystem::path& root) {
    std::vector<std::string> out;
    std::error_code ec;
    if (root.empty() || !stdfs::is_directory(root, ec)) return out;
    std::set<std::string> seen;
    for (stdfs::recursive_directory_iterator it(root, stdfs::directory_options::skip_permission_denied, ec), end; !ec && it != end; it.increment(ec)) {
        if (!it->is_regular_file(ec) || !tl_is_program(it->path())) continue;
        const std::string name = fs::to_text(it->path().filename());
        if (seen.insert(text::lower(name)).second) out.push_back(name);
    }
    std::sort(out.begin(), out.end(), [](const std::string& a, const std::string& b) { return text::lower(a) < text::lower(b); });
    return out;
}

std::vector<std::string> filter_modules(const std::vector<std::string>& modules, std::string_view query, std::size_t limit) {
    const std::string q = text::lower(text::trim(query));
    std::vector<std::string> first, then;
    for (const std::string& m : modules) {
        const std::string l = text::lower(m);
        if (q.empty() || l.starts_with(q)) first.push_back(m);
        else if (l.find(q) != std::string::npos) then.push_back(m);
        if (first.size() >= limit) break;
    }
    for (std::string& m : then) {
        if (first.size() >= limit) break;
        first.push_back(std::move(m));
    }
    if (first.size() > limit) first.resize(limit);
    return first;
}

}  // namespace cb::app::tools
