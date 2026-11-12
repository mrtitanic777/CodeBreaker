// cb-unity: alone (helpers named for what they do here)
#include "cb/port/tool_compilers.hpp"

#include <algorithm>

#include <nlohmann/json.hpp>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/process.hpp"
#include "cb/core/text.hpp"

namespace cb::port::tool {

namespace stdfs = std::filesystem;

namespace {

std::vector<std::string> strings_of(const Json& object, const char* key) {
    std::vector<std::string> out;
    const auto it = object.find(key);
    if (it == object.end() || !it->is_array()) return out;
    for (const Json& item : *it)
        if (item.is_string()) out.push_back(item.get<std::string>());
    return out;
}

void merge_compiler(Compiler& into, const Json& from, std::string_view name) {
    if (from.contains("find")) into.find = strings_of(from, "find");
    if (from.contains("setup")) into.setup = strings_of(from, "setup");
    if (from.contains("check")) into.check = strings_of(from, "check");
    if (from.contains("build")) into.build = strings_of(from, "build");
    if (const auto d = from.find("diag"); d != from.end() && d->is_string()) {
        into.diag = d->get<std::string>();
        auto compiled = Regex::compile(into.diag);
        if (!compiled.ok()) throw FormatError("compilers", 0, std::string(name) + ": its diag is not a pattern: " + compiled.error().what());
        into.diag_regex = std::make_shared<const Regex>(std::move(compiled.value()));
    }
}

bool wildcard_match(std::string_view pattern, std::string_view name) {
    // `*` any run, `?` any one; case does not matter (Windows names)
    std::size_t p = 0, n = 0, star = std::string_view::npos, mark = 0;
    while (n < name.size()) {
        if (p < pattern.size() && (pattern[p] == '?' || text::lower(std::string(1, pattern[p])) == text::lower(std::string(1, name[n])))) {
            ++p;
            ++n;
        } else if (p < pattern.size() && pattern[p] == '*') {
            star = p++;
            mark = n;
        } else if (star != std::string_view::npos) {
            p = star + 1;
            n = ++mark;
        } else {
            return false;
        }
    }
    while (p < pattern.size() && pattern[p] == '*') ++p;
    return p == pattern.size();
}

std::optional<stdfs::path> on_path(const std::string& name) {
    static const std::vector<std::string> kExtensions = {"", ".exe", ".cmd", ".bat", ".com"};
    std::error_code ec;
    const auto try_in = [&](const stdfs::path& dir) -> std::optional<stdfs::path> {
        for (const std::string& ext : kExtensions) {
            const stdfs::path candidate = dir / (name + ext);
            if (stdfs::is_regular_file(candidate, ec)) return candidate;
        }
        return std::nullopt;
    };
    if (name.find_first_of("/\\") != std::string::npos) return try_in(stdfs::path());
    const std::string path = fs::get_env("PATH").value_or("");
    for (const std::string_view dir : text::split(path, ';')) {
        if (dir.empty()) continue;
        if (const auto found = try_in(fs::from_text(dir))) return found;
    }
    return try_in(stdfs::current_path());
}

}  // namespace

Compilers load_compilers(const stdfs::path& compilers_json, const stdfs::path& settings_json) {
    Compilers out;
    const Json base = json::load(compilers_json);
    if (!base.is_object()) throw FormatError("compilers", 0, "the compilers file is not an object");
    for (const auto& [name, spec] : base.items()) {
        if (!spec.is_object()) continue;   // "about"
        merge_compiler(out[name], spec, name);
    }
    // the user's own, from settings.json
    if (!settings_json.empty() && stdfs::is_regular_file(settings_json)) {
        Json settings;
        try {
            settings = json::load(settings_json);
        } catch (const Error&) {
            return out;   // a settings file that is not JSON changes nothing
        }
        const auto port = settings.find("port");
        if (port != settings.end() && port->is_object()) {
            const auto mine = port->find("compilers");
            if (mine != port->end() && mine->is_object()) {
                for (const auto& [name, spec] : mine->items())
                    if (spec.is_object()) merge_compiler(out[name], spec, name);
            }
        }
    }
    return out;
}

std::vector<stdfs::path> glob_reverse(std::string_view pattern) {
    std::vector<std::string> segments;
    const std::string whole(pattern);   // (the pieces are views into it)
    for (const std::string_view s : text::split(whole, '/')) {
        for (const std::string_view t : text::split(s, '\\')) segments.emplace_back(t);
    }
    if (segments.empty()) return {};
    std::vector<stdfs::path> current;
    std::size_t first = 0;
    if (segments[0].size() == 2 && segments[0][1] == ':') {   // a drive
        current.push_back(fs::from_text(segments[0] + "/"));
        first = 1;
    } else if (segments[0].empty()) {                          // a rooted path
        current.push_back(stdfs::path("/"));
        first = 1;
    } else {
        current.push_back(stdfs::path());
    }
    std::error_code ec;
    for (std::size_t i = first; i < segments.size(); ++i) {
        const std::string& segment = segments[i];
        std::vector<stdfs::path> next;
        for (const stdfs::path& dir : current) {
            if (segment.find_first_of("*?") == std::string::npos) {
                const stdfs::path candidate = dir / fs::from_text(segment);
                if (stdfs::exists(candidate, ec)) next.push_back(candidate);
                continue;
            }
            const stdfs::path where = dir.empty() ? stdfs::path(".") : dir;
            if (!stdfs::is_directory(where, ec)) continue;
            for (const stdfs::directory_entry& e : stdfs::directory_iterator(where, ec)) {
                if (wildcard_match(segment, fs::to_text(e.path().filename()))) next.push_back(dir / e.path().filename());
            }
        }
        current = std::move(next);
    }
    std::sort(current.begin(), current.end(), [](const stdfs::path& a, const stdfs::path& b) { return fs::to_text(a) > fs::to_text(b); });
    return current;
}

std::optional<stdfs::path> find_compiler(std::span<const std::string> names, const std::optional<std::string>& given) {
    std::vector<std::string> tries;
    if (given && !given->empty()) tries.push_back(*given);
    tries.insert(tries.end(), names.begin(), names.end());
    for (const std::string& c : tries) {
        if (const auto p = on_path(c)) return p;
        const std::vector<stdfs::path> matches = glob_reverse(c);
        if (!matches.empty()) return matches.front();
    }
    return std::nullopt;
}

std::vector<std::string> expand(std::span<const std::string> tpl, const Values& values) {
    std::vector<std::string> out;
    for (const std::string& word : tpl) {
        if (word == "{srcs}") {
            out.insert(out.end(), values.srcs.begin(), values.srcs.end());
            continue;
        }
        std::string w = word;
        w = text::replace_all(w, "{cc}", values.cc);
        w = text::replace_all(w, "{inc}", values.inc);
        w = text::replace_all(w, "{out}", values.out);
        w = text::replace_all(w, "{src}", values.src);
        out.push_back(std::move(w));
    }
    return out;
}

Run run(const Compiler& spec, std::vector<std::string> argv, const stdfs::path& cwd, const stdfs::path& cc) {
    std::optional<stdfs::path> setup;
    if (!spec.setup.empty()) setup = find_compiler(spec.setup);
    if (setup) {
        std::vector<std::string> wrapped = {"cmd", "/d", "/c", "call", fs::to_text(*setup), ">nul", "&&"};
        wrapped.insert(wrapped.end(), argv.begin(), argv.end());
        argv = std::move(wrapped);
    }
    process::ProcessSpec p;
    p.argv = std::move(argv);
    p.cwd = cwd;
    // a compiler's own DLLs beside it, on the path
    const std::string existing = fs::get_env("PATH").value_or("");
    p.env["PATH"] = fs::to_text(stdfs::absolute(cc).parent_path()) + ";" + existing;
    const process::ProcessResult r = process::run(p);
    return Run{r.exit_code, std::string(as_text(r.out)), std::string(as_text(r.err))};
}

std::vector<Diagnostic> diagnostics(const Compiler& spec, std::string_view output) {
    static const Regex note_re(R"(^(?<file>[^:\n]+?(?::[\\/][^:\n]+)?):(?<line>\d+):(?<col>\d+): note: (?<text>.*)$)");
    std::vector<Diagnostic> out;
    const auto base_name = [](std::string_view path) {
        const std::size_t at = path.find_last_of("\\/");
        return std::string(at == std::string_view::npos ? path : path.substr(at + 1));
    };
    const auto number = [](std::optional<std::string_view> digits) {
        return digits ? text::parse_int<std::size_t>(*digits, 10).value_or(0) : std::size_t{0};
    };
    for (const std::string_view raw : text::lines(output)) {
        const std::string_view line = text::trim(raw);
        if (const std::optional<RegexMatch> n = note_re.match(line); n && !out.empty()) {
            // a note: what the complaint before it expected
            Diagnostic& d = out.back();
            d.notes.emplace_back(*n->named("text"));
            if (n->named("text")->find("in expansion of macro") != std::string_view::npos) {
                // the outermost use: the last such note
                if (!d.macro_at) d.macro_at = Diagnostic::Place{d.file, d.line, d.col};
                d.file = base_name(*n->named("file"));
                d.line = number(n->named("line"));
                d.col = number(n->named("col"));
            }
            continue;
        }
        if (!spec.diag_regex) continue;
        if (const std::optional<RegexMatch> m = spec.diag_regex->match(line)) {
            Diagnostic d;
            d.file = base_name(m->named("file").value_or(""));
            d.line = number(m->named("line"));
            d.col = number(m->named("col"));
            d.kind = std::string(m->named("kind").value_or(""));
            d.text = std::string(m->named("text").value_or(""));
            out.push_back(std::move(d));
        }
    }
    return out;
}

}  // namespace cb::port::tool
