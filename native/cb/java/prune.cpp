#include "cb/java/prune.hpp"

#include <algorithm>
#include <format>
#include <map>

#include "cb/core/fs.hpp"
#include "cb/core/parallel.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/java/textutil.hpp"

namespace cb::java::prune {

namespace stdfs = std::filesystem;

namespace {

using Names = std::set<std::string, std::less<>>;

bool prn_is_word(char c) { return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_'; }

// The text without its block comments and then its line comments (a `//` that stood inside a block comment is gone with it; one in a
// string literal goes too: the emitted code is scanned for names, and a name in a comment must not keep a file alive).
std::string prn_strip_comments(std::string_view text) {
    std::string without_blocks;
    std::size_t i = 0;
    while (i < text.size()) {
        if (text.substr(i).starts_with("/*")) {
            const std::size_t end = text.find("*/", i + 2);
            if (end != std::string_view::npos) {
                i = end + 2;
                continue;
            }
        }
        without_blocks += text[i++];
    }
    std::string out;
    i = 0;
    while (i < without_blocks.size()) {
        if (without_blocks.compare(i, 2, "//") == 0) {
            const std::size_t end = without_blocks.find('\n', i);
            i = end == std::string::npos ? without_blocks.size() : end;
            continue;
        }
        out += without_blocks[i++];
    }
    return out;
}

// One `#include "name.h"` directive: where it starts and ends, and the header it names. `#`, optional blanks, `include`, blanks, then a
// quoted name that ends in `.h`.
struct PrnInclude {
    std::size_t begin = 0;
    std::size_t end = 0;
    std::string name;
};

std::vector<PrnInclude> prn_includes(std::string_view text) {
    std::vector<PrnInclude> found;
    for (std::size_t at = text.find('#'); at != std::string_view::npos; at = text.find('#', at + 1)) {
        std::size_t i = at + 1;
        while (i < text.size() && text::is_space(text[i])) ++i;
        if (!text.substr(i).starts_with("include")) continue;
        i += 7;
        const std::size_t blanks = i;
        while (i < text.size() && text::is_space(text[i])) ++i;
        if (i == blanks || i >= text.size() || text[i] != '"') continue;
        const std::size_t close = text.find('"', i + 1);
        if (close == std::string_view::npos) continue;
        const std::string_view name = text.substr(i + 1, close - i - 1);
        if (name.size() < 3 || !name.ends_with(".h")) continue;
        found.push_back({at, close + 1, std::string(name)});
        at = close;
    }
    return found;
}

// A whole line that is only `#include "x.h"`
bool prn_only_include(std::string_view line) {
    const std::vector<PrnInclude> found = prn_includes(line);
    return found.size() == 1 && text::trim(line.substr(0, found[0].begin)).empty() && text::trim(line.substr(found[0].end)).empty();
}

// An include guard or pragma line: `#ifndef`, `#define`, `#endif`, `#pragma`
bool prn_is_guard(std::string_view line) {
    std::string_view s = text::ltrim(line);
    if (!s.starts_with('#')) return false;
    s = text::ltrim(s.substr(1));
    if (s.starts_with("ifndef") || s.starts_with("define") || s.starts_with("endif")) return true;
    return s.starts_with("pragma") && (s.size() == 6 || !prn_is_word(s[6]));
}

std::string prn_base(const std::string& file) { return file.ends_with(".h") ? file.substr(0, file.size() - 2) : file.substr(0, file.size() - 4); }

// The .h and .cpp files directly in the folder, sorted.
Names prn_emitted_files(const stdfs::path& dir) {
    Names files;
    std::error_code ec;
    for (const auto& entry : stdfs::directory_iterator(dir, ec)) {
        if (!entry.is_regular_file(ec)) continue;
        const std::string name = fs::to_text(entry.path().filename());
        if (name.ends_with(".h") || name.ends_with(".cpp")) files.insert(name);
    }
    return files;
}

// The identifiers of the text: `[A-Za-z_]\w*` runs
void prn_add_identifiers(std::string_view text, Names& into) {
    for (std::size_t i = 0; i < text.size();) {
        if (!prn_is_word(text[i])) { ++i; continue; }
        std::size_t j = i;
        while (j < text.size() && prn_is_word(text[j])) ++j;
        if (!(text[i] >= '0' && text[i] <= '9')) into.insert(std::string(text.substr(i, j - i)));
        i = j;
    }
}

}  // namespace

std::set<std::string> defined_symbols(std::string_view text) {
    // (the step budget is raised: a free-function pattern scans lazily across whole files)
    constexpr std::uint64_t kBudget = 500'000'000;
    static const Regex type_name(R"(\b(?:class|struct)\s+([A-Za-z_]\w*))", RegexFlags::None, kBudget);
    // `Class::member(args) {` -- a method, constructor or operator definition (it has a body)
    static const Regex method_owner(R"(\b([A-Za-z_]\w*)::[~A-Za-z_]\w*\s*\([^;{}]*\)\s*(?:const\s*)?\{)", RegexFlags::None, kBudget);
    // `<ret> name(args) {` at column 0. No `:` is allowed in the prefix, so a method definition (which has `Class::`) is not captured
    // again as its bare method name.
    static const Regex free_function(R"(^[A-Za-z_][\w<>,\*&\s]*?\b([A-Za-z_]\w*)\s*\([^;{}]*\)\s*(?:const\s*)?\{)", RegexFlags::Multiline, kBudget);
    std::set<std::string> symbols;
    for (const Regex* re : {&type_name, &method_owner, &free_function}) {
        re->for_each_match(text, [&symbols](const RegexMatch& m) {
            symbols.insert(std::string(m.group(1)));
            return true;
        });
    }
    return symbols;
}

std::vector<std::string> coalesce_forwarders(const stdfs::path& out_dir, const Progress& progress) {
    std::map<std::string, std::string> raw;
    {
        const Names files = prn_emitted_files(out_dir);
        const std::vector<std::string> names(files.begin(), files.end());
        std::vector<std::string> texts(names.size());
        parallel::parallel_for(names.size(), [&](std::size_t i) { texts[i] = textutil::read_text(out_dir / fs::from_text(names[i]), textutil::Decode::Strict); });
        for (std::size_t i = 0; i < names.size(); ++i) raw[names[i]] = std::move(texts[i]);
    }

    std::map<std::string, std::string> forwarders;   // forwarder file -> the single header it #includes
    for (const auto& [name, text] : raw) {
        if (!name.ends_with(".h")) continue;
        const std::string body = prn_strip_comments(text);
        const std::vector<PrnInclude> includes = prn_includes(body);
        std::size_t meaningful = 0;
        bool only_includes = true;
        for (const std::string_view line : text::lines(body)) {
            if (text::trim(line).empty() || prn_is_guard(line)) continue;
            ++meaningful;
            only_includes = only_includes && prn_only_include(line);
        }
        if (includes.size() == 1 && meaningful > 0 && only_includes) forwarders[name] = includes[0].name;
    }
    if (forwarders.empty()) return {};

    // follow forwarder chains to the real (non-forwarder) target
    const auto resolve = [&forwarders](std::string header) {
        Names seen;
        for (auto it = forwarders.find(header); it != forwarders.end() && !seen.contains(header); it = forwarders.find(header)) {
            seen.insert(header);
            header = it->second;
        }
        return header;
    };
    std::map<std::string, std::string> alias;
    for (const auto& [name, target] : forwarders) alias[name] = resolve(name);

    for (const auto& [name, text] : raw) {
        if (forwarders.contains(name)) continue;
        std::string rewritten;
        std::size_t copied = 0;
        for (const PrnInclude& inc : prn_includes(text)) {
            rewritten.append(text, copied, inc.begin - copied);
            const auto a = alias.find(inc.name);
            rewritten += "#include \"" + (a != alias.end() ? a->second : inc.name) + "\"";
            copied = inc.end;
        }
        rewritten.append(text, copied, std::string::npos);
        if (rewritten != text) textutil::write_text(out_dir / fs::from_text(name), rewritten);
    }

    std::vector<std::string> removed;
    for (const auto& [name, target] : forwarders) {
        std::error_code ec;
        if (stdfs::remove(out_dir / fs::from_text(name), ec)) removed.push_back(name);
    }
    if (!removed.empty() && progress) {
        progress(std::format("[coalesce] folded {} forwarder header(s) into their targets: {}", removed.size(), text::join(removed, ", ")));
    }
    return removed;
}

std::vector<std::string> prune_unreferenced(const stdfs::path& out_dir, const Names& game_classes, const std::vector<std::string>& overlay_files,
                                            const Progress& progress) {
    const Names files = prn_emitted_files(out_dir);
    std::map<std::string, Names> includes, provides;
    std::map<std::string, std::string> raw, code;
    {
        // each file is read and scanned on its own, so the files are taken side by side (the folder holds a thousand of them)
        const std::vector<std::string> names(files.begin(), files.end());
        struct Scanned {
            std::string raw, code;
            Names includes, provides;
        };
        std::vector<Scanned> scanned(names.size());
        parallel::parallel_for(names.size(), [&](std::size_t i) {
            Scanned& s = scanned[i];
            s.raw = textutil::try_read_text(out_dir / fs::from_text(names[i])).value_or("");
            for (const PrnInclude& inc : prn_includes(s.raw)) s.includes.insert(inc.name);
            s.code = prn_strip_comments(s.raw);
            const std::set<std::string> symbols = defined_symbols(s.code);
            s.provides = Names(symbols.begin(), symbols.end());
        });
        for (std::size_t i = 0; i < names.size(); ++i) {
            raw[names[i]] = std::move(scanned[i].raw);
            code[names[i]] = std::move(scanned[i].code);
            includes[names[i]] = std::move(scanned[i].includes);
            provides[names[i]] = std::move(scanned[i].provides);
        }
    }

    // The build-critical spine and entry glue are always roots. They are reachable from the game anyway (it includes JavaRT and the
    // rest); listing them is insurance, so a gap in the scan can never drop the foundation.
    static const Names kFoundation = {"stdafx", "JavaRT", "JavaString", "JArray", "JList", "JMap", "JSet", "JObject", "JCat", "Common", "j2x_init", "j2x_rtti", "Main"};
    Names roots = game_classes;
    roots.insert(kFoundation.begin(), kFoundation.end());
    for (const std::string& f : files) {   // link-critical files declare themselves (HeapTrack, ...)
        if (raw[f].find("J2X-ALWAYS-LINK") != std::string::npos) roots.insert(prn_base(f));
    }
    // Overlay-written files are roots by definition. Only the base name matters: the prune works on the flat listing.
    for (const std::string& relative : overlay_files) {
        const std::string name = fs::to_text(fs::from_text(relative).filename());
        if (name.ends_with(".h") || name.ends_with(".cpp")) roots.insert(prn_base(name));
    }

    Names reachable;
    const auto walk = [&](std::vector<std::string> stack) {
        while (!stack.empty()) {
            const std::string f = std::move(stack.back());
            stack.pop_back();
            if (reachable.contains(f) || !files.contains(f)) continue;
            reachable.insert(f);
            if (f.ends_with(".h")) {   // a reached header drags its .cpp partner (the same translation unit)
                const std::string cpp = prn_base(f) + ".cpp";
                if (files.contains(cpp) && !reachable.contains(cpp)) stack.push_back(cpp);
            }
            for (const std::string& inc : includes[f]) {
                if (files.contains(inc) && !reachable.contains(inc)) stack.push_back(inc);
            }
        }
    };
    std::vector<std::string> seeds;
    for (const std::string& f : files) {
        if (roots.contains(prn_base(f))) seeds.push_back(f);
    }
    walk(seeds);

    // Symbol-defence fixpoint: keep any file that DEFINES a name referenced by already-reachable code (it catches the cross-named
    // implementation files the include graph misses), then walk their includes.
    Names referenced, scanned;
    for (bool changed = true; changed;) {
        changed = false;
        for (const std::string& f : reachable) {
            if (scanned.insert(f).second) prn_add_identifiers(code[f], referenced);
        }
        for (const std::string& f : files) {
            if (reachable.contains(f)) continue;
            if (std::ranges::any_of(provides[f], [&referenced](const std::string& symbol) { return referenced.contains(symbol); })) {
                walk({f});
                changed = true;
            }
        }
    }

    std::vector<std::string> dropped;
    Names subsystems;
    for (const std::string& f : files) {
        if (reachable.contains(f) || roots.contains(prn_base(f))) continue;
        std::error_code ec;
        if (stdfs::remove(out_dir / fs::from_text(f), ec)) {
            dropped.push_back(f);
            subsystems.insert(prn_base(f));
        }
    }
    if (!dropped.empty() && progress) {
        const std::vector<std::string> names(subsystems.begin(), subsystems.end());
        progress(std::format("[prune] cut {} file(s) in dead subsystems no live #include reaches ({} subsystem(s)): {}", dropped.size(),
                             subsystems.size(), text::join(names, ", ")));
    }
    return dropped;
}

}  // namespace cb::java::prune
