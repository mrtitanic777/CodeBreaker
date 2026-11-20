// cb/app/tools.cpp -- see tools.hpp: the table of tools, the argument-spec and usage parsers, search, form validation and the command line a form builds.
#include "cb/app/tools.hpp"

#include <algorithm>
#include <cctype>

#include "cb/app/commands.hpp"
#include "cb/core/text.hpp"

namespace cb::app::tools {

namespace {

struct ToolRow {
    std::string_view name, title, group, description, spec;
};

constexpr ToolRow kToolRows[] = {
#include "tools_catalog.inc"
};

// The commands of the table that are not tools: the reason each makes no sense as a form.
const std::vector<LeftOut> kLeftOut = {
    {"app", "it is the window itself"},
    {"nasm", "it is the assembler for scripts and tests, with NASM's own command line, not a step people take in the window"},
    {"mcp", "it is a server that talks to an assistant over its standard input and output, which a form cannot be on the other end of"},
    {"inspect", "its steps are the tools of its own group, each under its plain-word title, which the form runs by name"},
    {"extract", "its steps are the tools of its own group, each under its plain-word title, which the form runs by name"},
    {"check", "its steps are the tools of its own group, each under its plain-word title, which the form runs by name"},
    {"compare", "its steps are the tools of its own group, each under its plain-word title, which the form runs by name"},
    {"corpus", "its steps are the tools of its own group, each under its plain-word title, which the form runs by name"},
    {"lookup", "its steps are the tools of its own group, each under its plain-word title, which the form runs by name"},
    {"xbox", "its steps are the tools of its own group, each under its plain-word title, which the form runs by name"},
};

constexpr const char* kGroups[] = {"Convert", "Inspect", "Extract", "Build", "Check", "Compare", "Collection", "Look up", "Xbox 360", "Maintenance"};

std::string tl_trim(std::string_view s) { return std::string(text::trim(s)); }

std::string tl_label(std::string_view raw) {
    std::string out(raw);
    for (char& c : out)
        if (c == '_' || c == '-') c = ' ';
    if (!out.empty()) out[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(out[0])));
    return out;
}

// A guess at the kind of a value from the word the usage line names it by.
ArgKind tl_kind_of(std::string_view word) {
    const std::string w = text::lower(word);
    for (const char* folder : {"folder", "dir"})
        if (w.find(folder) != std::string::npos) return ArgKind::Folder;
    for (const char* file : {"file", "program", "image", "xex", "driver", "binary", "package", "config", "exe", "dll", "toml", ".asm", "path"})
        if (w.find(file) != std::string::npos) return ArgKind::File;
    if (w == "f" || w == "out" || w == "ini" || w == "in") return ArgKind::File;
    return ArgKind::Text;
}

}  // namespace

const std::vector<std::string>& groups() {
    static const std::vector<std::string> list(std::begin(kGroups), std::end(kGroups));
    return list;
}

const std::vector<LeftOut>& left_out() { return kLeftOut; }

std::vector<Arg> parse_spec(std::string_view spec) {
    std::vector<Arg> args;
    for (const std::string_view part : text::split(spec, ';')) {
        std::string_view p = text::trim(part);
        if (p.empty()) continue;
        Arg a;
        a.required = true;
        while (!p.empty() && (p[0] == '?' || p[0] == '*')) {
            if (p[0] == '?') a.required = false;
            if (p[0] == '*') a.list = true;
            p.remove_prefix(1);
        }
        if (p.size() < 2) continue;
        switch (p[0]) {
            case 'F': a.kind = ArgKind::File; break;
            case 'D': a.kind = ArgKind::Folder; break;
            case 'T': a.kind = ArgKind::Text; break;
            case 'W': a.kind = ArgKind::Words; break;
            case 'C': a.kind = ArgKind::Choice; break;
            case 'B': a.kind = ArgKind::Toggle; break;
            default: continue;
        }
        std::string rest = tl_trim(p.substr(1));
        const std::size_t flag_at = rest.rfind(" -");
        if (flag_at != std::string::npos && rest.find(' ', flag_at + 1) == std::string::npos) {
            a.flag = rest.substr(flag_at + 1);
            rest = tl_trim(rest.substr(0, flag_at));
        }
        if (const std::size_t eq = rest.find('='); eq != std::string::npos) {
            const std::string choice_text = rest.substr(eq + 1);
            for (const std::string_view c : text::split(choice_text, '|')) a.choices.emplace_back(c);
            rest = rest.substr(0, eq);
        }
        a.label = tl_label(rest);
        if (a.kind == ArgKind::Toggle) a.required = false;
        args.push_back(std::move(a));
    }
    return args;
}

// Top-level pieces of a usage line after the command name: "<x>", "[...]" (balanced), "a|b", "..." and bare words.
std::vector<Arg> parse_usage(std::string_view usage) {
    std::vector<std::string> pieces;
    std::string current;
    int depth = 0;
    for (const char c : usage) {
        if (c == '[' || c == '<') ++depth;
        if (c == ']' || c == '>') depth = std::max(0, depth - 1);
        if (c == ' ' && depth == 0) {
            if (!current.empty()) pieces.push_back(std::move(current));
            current.clear();
        } else {
            current += c;
        }
    }
    if (!current.empty()) pieces.push_back(std::move(current));
    std::vector<Arg> args;
    for (std::size_t i = 1; i < pieces.size(); ++i) {   // (piece 0 is the command)
        const std::string& piece = pieces[i];
        if (piece == "..." || piece == "|") {
            if (!args.empty()) args.back().list = true;
            continue;
        }
        Arg a;
        std::string inner = piece;
        a.required = true;
        if (inner.front() == '[' && inner.back() == ']') {
            a.required = false;
            inner = inner.substr(1, inner.size() - 2);
        }
        if (inner.size() >= 2 && inner.front() == '<' && inner.back() == '>') inner = inner.substr(1, inner.size() - 2);
        if (inner.rfind("--", 0) == 0 || inner.rfind('-', 0) == 0) {
            // [--flag] toggle, [--opt VALUE] option; "--a|--b" is two toggles
            const std::size_t space = inner.find(' ');
            const std::string flag = inner.substr(0, space);
            if (space == std::string::npos) {
                for (const std::string_view f : text::split(flag, '|')) {
                    Arg t;
                    t.kind = ArgKind::Toggle;
                    t.flag = std::string(f);
                    t.label = tl_label(f.substr(f.rfind('-') + 1));
                    args.push_back(std::move(t));
                }
                continue;
            }
            const std::string value = tl_trim(inner.substr(space + 1));
            a.flag = flag;
            a.label = tl_label(flag.substr(flag.rfind('-') + 1));
            a.kind = tl_kind_of(value);
            a.required = false;
            args.push_back(std::move(a));
            continue;
        }
        if (inner.find('|') != std::string::npos && inner.find(' ') == std::string::npos) {
            a.kind = ArgKind::Choice;
            for (const std::string_view c : text::split(inner, '|')) a.choices.emplace_back(c);
            a.label = "Action";
        } else {
            a.label = tl_label(inner);
            a.kind = tl_kind_of(inner);
        }
        args.push_back(std::move(a));
    }
    return args;
}

const std::vector<Tool>& all() {
    static const std::vector<Tool> list = [] {
        std::vector<Tool> tools;
        for (const std::string& group : groups()) {
            for (const ToolRow& row : kToolRows) {
                if (row.group != group) continue;
                Tool t;
                t.name = std::string(row.name);
                t.title = std::string(row.title);
                t.group = std::string(row.group);
                t.description = std::string(row.description);
                if (const CommandDoc* doc = find_command_doc(row.name)) t.usage = std::string(doc->synopsis);
                t.args = row.spec == "=" ? parse_usage(t.usage) : parse_spec(row.spec);
                tools.push_back(std::move(t));
            }
        }
        return tools;
    }();
    return list;
}

const Tool* find(std::string_view name) {
    for (const Tool& t : all())
        if (t.name == name) return &t;
    return nullptr;
}

bool matches(const Tool& tool, std::string_view query) {
    const std::string hay = text::lower(tool.title + " " + tool.name + " " + tool.description);
    const std::string wanted = text::lower(query);
    for (const std::string_view word : text::split_ws(wanted))
        if (hay.find(word) == std::string::npos) return false;
    return true;
}

std::vector<const Tool*> search(std::string_view query) {
    std::vector<const Tool*> out;
    for (const Tool& t : all())
        if (matches(t, query)) out.push_back(&t);
    return out;
}

Values empty_values(const Tool& tool) {
    Values v;
    for (const Arg& a : tool.args) v.text.push_back(a.kind == ArgKind::Choice && a.required && !a.choices.empty() ? a.choices.front() : std::string());
    return v;
}

std::optional<std::string> missing(const Tool& tool, const Values& values) {
    for (std::size_t i = 0; i < tool.args.size(); ++i) {
        const Arg& a = tool.args[i];
        const std::string& v = i < values.text.size() ? values.text[i] : std::string();
        if (a.required && a.kind != ArgKind::Toggle && text::trim(v).empty()) return a.label;
    }
    return std::nullopt;
}

std::vector<std::string> split_entries(std::string_view value, bool on_semicolon) {
    std::vector<std::string> out;
    if (on_semicolon) {
        for (const std::string_view e : text::split(value, ';')) {
            const std::string t = tl_trim(e);
            if (!t.empty()) out.push_back(t);
        }
    } else {
        for (const std::string_view e : text::split_ws(value)) out.emplace_back(e);
    }
    return out;
}

std::vector<std::string> build_argv(const Tool& tool, const Values& values) {
    std::vector<std::string> argv = {tool.name};
    // options first-come in spec order, but positionals keep their order among themselves; an option is written before the positionals that follow it
    for (std::size_t i = 0; i < tool.args.size(); ++i) {
        const Arg& a = tool.args[i];
        const std::string v = i < values.text.size() ? tl_trim(values.text[i]) : std::string();
        if (a.kind == ArgKind::Toggle) {
            if (!v.empty()) argv.push_back(a.flag);
            continue;
        }
        if (v.empty()) continue;
        if (!a.flag.empty()) argv.push_back(a.flag);
        if (a.kind == ArgKind::Words) {
            for (std::string& e : split_entries(v, false)) argv.push_back(std::move(e));
        } else if (a.list) {
            for (std::string& e : split_entries(v, true)) argv.push_back(std::move(e));
        } else {
            argv.push_back(v);
        }
    }
    return argv;
}

}  // namespace cb::app::tools
