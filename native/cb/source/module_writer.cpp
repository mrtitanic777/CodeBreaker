// cb/source/module_writer.cpp -- one source file per module, every function labelled with what is known (module_writer.hpp).
//
// DIVERGES from the Python writer: the module is handed over prepared (the writer does not find it, reconstruct it or read its binary); a
// function of the runtime library with no name or library in the store is written as `?`, where the Python wrote `None`; the status line of
// each function is built as the back end's parts, not through an L2 tree of units (the tree's origins held an address the store spells as a
// string).

#include "cb/source/module_writer.hpp"

#include <algorithm>
#include <format>
#include <unordered_map>

#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/source/classes_writer.hpp"
#include "cb/symbols/demangle.hpp"

namespace cb::source::module_writer {

namespace mf = translate::backends::module_file;

namespace {

bool is_word_char(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }

bool is_identifier(std::string_view s) {
    return !s.empty() && !(s[0] >= '0' && s[0] <= '9') && std::ranges::all_of(s, is_word_char);
}

std::string status_title(verify::Class c) { return std::string(verify::title(c)); }

engine::ClassCount zero_counts() {
    engine::ClassCount counts;
    for (const verify::Class c : verify::kClassScored) counts[c] = 0;
    return counts;
}

std::string counts_text(const engine::ClassCount& counts) {
    std::vector<std::string> parts;
    for (const verify::Class c : verify::kClassScored) {
        if (counts.at(c) != 0) parts.push_back(std::format("{} {}", verify::title(c), counts.at(c)));
    }
    return text::join(parts, ", ");
}

std::vector<mf::Status> statuses(const engine::ClassCount& counts) {
    std::vector<mf::Status> out;
    for (const verify::Class c : verify::kClassScored) out.push_back({status_title(c), static_cast<std::size_t>(counts.at(c))});
    return out;
}

std::string cut(std::string_view s, std::size_t columns) { return std::string(text::utf8_prefix(s, columns)); }

// What the functions of the module are called in the file, for the classes' comments.
class FileNames final : public classes_writer::FunctionNotes {
public:
    FileNames(const Prepared& m, const std::map<std::string, engine::Best>& best, const std::map<std::string, std::pair<std::optional<std::string>, std::optional<std::string>>>& known,
              const Names& names)
        : m_(m), best_(best), known_(known), names_(names) {}

    std::string name_at(core16::Va address) const override {
        const std::string key = m_.address_key(address);
        const auto row = best_.find(key);
        const auto id = known_.find(key);
        if (row == best_.end() && id == known_.end() && m_.flat && m_.import_behind) {
            if (const std::optional<std::string> behind = m_.import_behind(address)) return *behind;
        }
        std::string current;
        if (row != best_.end() && row->second.candidate.name) current = *row->second.candidate.name;
        if (current.empty()) current = id != known_.end() ? id->second.first.value_or("") : "sub_" + key;
        if (current.empty()) current = "sub_" + key;
        const auto renamed = names_.all.find(current);
        return renamed != names_.all.end() ? renamed->second : current;
    }
    std::string status_at(core16::Va address) const override {
        const auto row = best_.find(m_.address_key(address));
        return row == best_.end() ? std::string() : status_title(row->second.cls);
    }

private:
    const Prepared& m_;
    const std::map<std::string, engine::Best>& best_;
    const std::map<std::string, std::pair<std::optional<std::string>, std::optional<std::string>>>& known_;
    const Names& names_;
};

}  // namespace

std::string_view why(std::string_view reason) {
    static const std::map<std::string, std::string, std::less<>> kWhy = {
        {"ret-differs", "returns a different value"},
        {"deps-differ", "makes different API calls, or passes them different arguments"},
        {"exit-differs", "ends differently (one side faults or runs away)"},
        {"writes-differ", "leaves different values in memory"},
        {"stack-differs", "removes a different number of bytes from the stack (convention or argument count)"},
        {"registers-differ", "changes a register it must preserve"},
        {"retail: call-into-nothing", "the original calls through a pointer the loader fills in (a thunk entry)"}};
    const auto found = kWhy.find(reason);
    return found != kWhy.end() ? std::string_view(found->second) : reason;
}

std::string describe(const engine::Best& best) {
    const engine::Candidate& c = best.candidate;
    std::optional<std::int64_t> coverage;   // "coverage=NN" among the stored limits
    for (const std::string_view limit : text::split(c.limits, ',')) {
        if (limit.starts_with("coverage=")) {
            coverage = text::parse_int<std::int64_t>(limit.substr(9));
            if (coverage) break;
        }
    }
    switch (best.cls) {
        case verify::Class::Verified: return coverage ? std::format("{}% of it exercised", *coverage) : std::string();
        case verify::Class::Weak: {
            std::vector<std::string> words;
            for (const verify::WeakLimit l : best.weak) words.emplace_back(verify::explanation(l));
            return text::join(words, "; ") + (coverage ? std::format(" ({}% ran)", *coverage) : std::string());
        }
        case verify::Class::Unverified:
        case verify::Class::NotJudged: return std::string(why(c.reason));
        case verify::Class::NoCompile: return cut(c.reason, 90);
        default: return cut(c.reason, 80);
    }
}

std::map<std::string, std::string> unique_import_names(const std::map<std::string, std::string>& found, const std::set<std::string>& used) {
    std::map<std::string, std::size_t> times;
    for (const auto& [symbol, name] : found) ++times[name];
    std::map<std::string, std::string> out;
    for (const auto& [symbol, name] : found) {
        if (times[name] == 1 && !used.contains(name) && !found.contains(name)) out[symbol] = name;
    }
    return out;
}

std::optional<ConsoleImport> parse_console_import(std::string_view symbol) {
    constexpr std::string_view kHead = "import";
    constexpr std::string_view kMid = "_ord_";
    if (!symbol.starts_with(kHead)) return std::nullopt;
    symbol.remove_prefix(kHead.size());
    const std::size_t mid = symbol.find(kMid);
    if (mid == std::string_view::npos || mid == 0) return std::nullopt;
    const std::string_view library = symbol.substr(0, mid);
    const std::string_view number = symbol.substr(mid + kMid.size());
    const auto all_digits = [](std::string_view s) { return !s.empty() && std::ranges::all_of(s, [](char c) { return c >= '0' && c <= '9'; }); };
    if (!all_digits(library) || !all_digits(number)) return std::nullopt;
    const auto index = text::parse_int<std::size_t>(library);
    if (!index) return std::nullopt;
    return ConsoleImport{*index, std::string(number)};
}

std::map<std::string, std::string> console_import_names(std::span<const std::string> symbols, std::span<const std::map<std::string, std::string>> libraries,
                                                         const std::set<std::string>& used) {
    std::map<std::string, std::string> found;
    for (const std::string& symbol : std::set<std::string>(symbols.begin(), symbols.end())) {
        const std::optional<ConsoleImport> imp = parse_console_import(symbol);
        if (!imp || imp->library >= libraries.size()) continue;
        const auto name = libraries[imp->library].find(imp->number);
        if (name != libraries[imp->library].end() && is_identifier(name->second)) found[symbol] = name->second;
    }
    std::map<std::string, std::size_t> times;
    for (const auto& [symbol, name] : found) ++times[name];
    std::map<std::string, std::string> out;
    for (const auto& [symbol, name] : found) {
        if (times[name] == 1 && !used.contains(name)) out[symbol] = name;
    }
    return out;
}

std::string spell(std::string_view text, const std::map<std::string, std::string>& names) {
    if (names.empty()) return std::string(text);
    std::string out;
    out.reserve(text.size());
    for (std::size_t i = 0; i < text.size();) {
        if (!is_word_char(text[i])) {
            out += text[i++];
            continue;
        }
        std::size_t j = i;
        while (j < text.size() && is_word_char(text[j])) ++j;
        const std::string word(text.substr(i, j - i));
        const auto found = names.find(word);
        out += found != names.end() ? found->second : word;
        i = j;
    }
    return out;
}

Names plan_names(std::span<const std::string> addresses, const std::function<std::string(const std::string&)>& candidate_name,
                 const std::map<std::string, std::string>& imports, const std::map<std::string, std::string>& exported,
                 const std::map<std::string, std::string>& recovered) {
    Names out;
    std::set<std::string> taken;
    for (const auto& [symbol, name] : imports) taken.insert(name);
    for (const auto& [address, name] : exported) taken.insert(name);
    std::set<std::string> renamed;
    for (const std::string& address : addresses) {
        std::string current = candidate_name(address);
        if (current.empty()) current = "sub_" + address;
        const auto name = recovered.find(address);
        if (name == recovered.end() || name->second.empty()) continue;
        if (current.starts_with("sub_") && !taken.contains(name->second) && !renamed.contains(name->second)) {
            out.rename[current] = name->second;
            renamed.insert(name->second);
            taken.insert(name->second);
        }
    }
    out.all = imports;
    for (const auto& [from, to] : out.rename) out.all[from] = to;   // (a recovered name replaces an import's of the same spelling)
    return out;
}

Written write_module(const Prepared& m, engine::FunctionStore& store, const std::filesystem::path& out_dir, std::size_t split, const Log& log) {
    fs::make_dirs(out_dir);
    const std::map<std::string, engine::Best> best = store.best(m.module, m.profile);
    const std::vector<std::string> addrs = store.addresses(m.module);   // every function the module is known to have
    const auto known = store.identified(m.module);                      // the runtime library's among them
    std::set<std::string> used;                                          // the names its functions have
    for (const auto& [address, row] : best) {
        if (row.candidate.name && !row.candidate.name->empty()) used.insert(*row.candidate.name);
    }
    const std::map<std::string, std::string> real = unique_import_names(m.found_imports, used);
    const Names names = plan_names(
        addrs,
        [&](const std::string& address) {
            const auto row = best.find(address);
            return row != best.end() && row->second.candidate.name ? *row->second.candidate.name : std::string();
        },
        real, m.exported, m.recovered);

    // names in place of numbers and placeholders: the module as the store holds it, through the readable pass
    std::map<std::string, std::string> texts;
    for (const auto& [address, row] : best) {
        if (row.candidate.source && !row.candidate.source->empty()) texts[address] = spell(*row.candidate.source, names.all);
    }
    const std::string stored_header = store.get_text(m.module, m.source_engine, "header");
    translate::l2::Made stored(m.name, stored_header);
    for (const std::string& address : addrs) {
        if (const auto t = texts.find(address); t != texts.end()) stored.add(address, t->second);
    }
    const std::unique_ptr<ReadablePass> words = m.readable(stored);

    engine::ClassCount counts = zero_counts();
    std::vector<mf::Part> parts;
    for (const std::string& va : addrs) {
        const auto row = best.find(va);
        const auto id = known.find(va);
        std::vector<std::string> said;
        if (const auto ex = m.exported.find(va); ex != m.exported.end()) said.push_back(std::format("exported as {}: {}", ex->second, symbols::demangle::readable(ex->second)));
        if (row == best.end()) {
            const verify::Class shown = id != known.end() ? verify::Class::Identified : verify::Class::NoSource;
            ++counts[shown];
            std::vector<std::string> facts;
            if (id != known.end()) facts.push_back(id->second.second.value_or("?"));
            facts.insert(facts.end(), said.begin(), said.end());
            facts.push_back("not measured on the current verifier");
            parts.push_back({std::format("[{}] {}@ {} -- {}", verify::title(shown), id != known.end() ? id->second.first.value_or("?") + "  " : std::string(), va,
                                         text::join(facts, "; ")),
                             std::nullopt});
            continue;
        }
        const engine::Best& b = row->second;
        const std::string detail = describe(b);
        const verify::Class shown = id != known.end() && b.cls != verify::Class::Verified && b.cls != verify::Class::Weak ? verify::Class::Identified : b.cls;
        ++counts[shown];
        if (shown == verify::Class::Identified) {
            said.insert(said.begin(), std::format("{} of {}; reconstructed below: {}{}", id->second.first.value_or("?"), id->second.second.value_or("?"),
                                                  verify::title(b.cls), detail.empty() ? std::string() : ", " + detail));
        } else {
            if (!detail.empty()) said.insert(said.begin(), detail);
            if (id != known.end()) said.push_back(std::format("this is {} of {}", id->second.first.value_or("?"), id->second.second.value_or("?")));
        }
        std::string source = "/* (no reconstruction) */";
        if (texts.contains(va)) source = words->function(va);
        source = std::string(text::trim(source));
        const std::string candidate = b.candidate.name && !b.candidate.name->empty() ? *b.candidate.name : "sub_" + va;
        const auto renamed = names.rename.find(candidate);
        const std::string shown_name = renamed != names.rename.end() ? renamed->second : candidate;
        if (shown_name.starts_with("sub_") && texts.contains(va)) {
            if (const std::string hint = words->calls_hint(texts.at(va)); !hint.empty()) said.push_back(hint);
        }
        if (!m.exported.contains(va) && b.candidate.name && words->results_unused(*b.candidate.name)) said.push_back("no caller here uses what it returns");
        parts.push_back({std::format("[{}] {}  @ {}{}", verify::title(shown), shown_name, va, said.empty() ? std::string() : " -- " + text::join(said, "; ")), source});
    }

    mf::Facts facts;
    facts.name = m.name;
    facts.stem = m.stem;
    facts.date = m.date;
    facts.digest = m.profile;
    facts.statuses = statuses(counts);
    facts.total = addrs.size();
    facts.lane = m.lane;
    facts.real = real.size();
    facts.whole = m.whole;

    std::string header = stored_header;
    if (!header.empty()) {
        header = spell(header, names.all);
        if (!real.empty()) {   // which module and number each import is
            std::vector<std::pair<std::string, std::string>> by_name;   // (name, symbol), by name
            for (const auto& [symbol, name] : real) by_name.emplace_back(name, symbol);
            std::ranges::sort(by_name);
            header += "\n/* imports by number:\n";
            for (const auto& [name, symbol] : by_name) {
                const std::size_t cut_at = symbol.rfind('_');
                const std::string library = cut_at == std::string::npos ? symbol : symbol.substr(0, cut_at);
                const std::string number = cut_at == std::string::npos ? std::string() : symbol.substr(cut_at + 1);
                header += std::format(" *   {:<28} {}.{}\n", name, library, number);
            }
            header += " */\n";
        }
        if (m.globals_note) header += m.globals_note(header);
        header = words->rename_globals(header);   // the globals as the functions now call them
        if (words->constants_used()) {
            header += std::format("\n/* SDK constants written into {}.c, with the values the SDK headers give them */\n{}", m.stem, words->definitions());
        }
        header += words->struct_definitions();
        fs::write_file(out_dir / (m.stem + ".h"),
                       as_bytes(std::format("/* {} -- types, declarations and globals used by the functions in {}.c.\n"
                                            " * Globals are bound to their addresses in the original until the module's data is\n"
                                            " * reconstructed. */\n\n{}",
                                            m.name, m.stem, header)));
    }
    facts.header = !header.empty();

    // the C, written by its back end from the status of each function and the store's facts
    const std::vector<mf::File> files = mf::c(parts, facts, split);
    for (const mf::File& f : files) fs::write_file(out_dir / f.name, as_bytes(f.text));
    Written out;
    out.file = out_dir / files.front().name;
    out.counts = counts;
    out.total = addrs.size();

    // the module's C++ classes, each virtual function tied to the function above that implements it
    const FileNames notes(m, best, known, names);
    if (const auto written = classes_writer::write(m.classes, m.name, m.stem, out_dir, notes); written && log) {
        log(std::format("{}: the module's C++ classes", fs::to_text(*written)));
    }
    if (log) log(std::format("{}: {}", fs::to_text(out.file), counts_text(counts)));
    return out;
}

Written write_assembly(const Prepared& m, engine::FunctionStore& store, const std::filesystem::path& out_dir, const Log& log) {
    fs::make_dirs(out_dir);
    Written out;
    out.counts = store.scores(m.profile, m.module);
    for (const verify::Class c : verify::kClassScored) out.counts.try_emplace(c, 0);
    for (const auto& [c, n] : out.counts) out.total += static_cast<std::size_t>(n);
    const std::string source = store.get_text(m.module, m.source_engine, "header");
    const std::map<std::string, engine::Best> best = store.best(m.module, m.profile);
    mf::Facts facts;
    facts.name = m.name;
    facts.date = m.date;
    facts.digest = m.profile;
    facts.statuses = statuses(out.counts);
    facts.total = out.total;
    facts.lane = m.lane;
    for (const auto& [address, row] : best) {   // what the judge said of the module as a whole: the first detail any function has
        if (!row.candidate.detail.empty()) {
            facts.how = row.candidate.detail;
            break;
        }
    }
    for (const std::string& va : store.addresses(m.module)) {
        const auto row = best.find(va);
        if (row == best.end() || row->second.cls == verify::Class::Verified) continue;
        facts.rows.push_back({va, row->second.candidate.name.value_or(""), status_title(row->second.cls), describe(row->second)});
    }
    std::vector<translate::backends::assembly::Unit> units;
    if (!source.empty()) units.push_back(translate::backends::assembly::MachineFragment{m.stem, source, false});
    const Result<std::string> file = mf::assembly_file(units, facts);
    std::string text = file.value();
    text = text::replace_all(text, "\n", "\r\n");
    out.file = out_dir / (m.stem + ".asm");
    fs::write_file(out.file, text::to_latin1(text));
    if (log) log(std::format("{}: {}", fs::to_text(out.file), counts_text(out.counts)));
    return out;
}

std::string scoreboard(const ScoreboardInput& in) {
    std::vector<std::string> lines = {
        "# Reconstruction scoreboard", "",
        std::format("Generated {} by `codebreaker scoreboard` (verifier settings `{}`).", in.generated, in.settings), "",
        "Only modules measured on the current verifier are listed. A function is VERIFIED when its reconstructed C, rebuilt and run beside the "
        "original, returns the same value, makes the same API calls with the same arguments, changes the same memory and leaves the stack and "
        "saved registers alike on every input tried, with at least half of it exercised. Everything else is not progress. A function "
        "IDENTIFIED as a runtime library's (the C runtime, a compiler helper) is not the module's own code and is counted apart, unless it was "
        "verified as well.",
        "",
        "| Module | Kind | Functions | Verified | Verified (weak) | Identified (runtime library) | Unverified | Not judged | Does not compile | No "
        "source | Builds whole | Runs in place |",
        "|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---|---|"};
    engine::ClassCount total = zero_counts();
    std::size_t all = 0;
    const auto percent = [](std::int64_t n, std::size_t of) { return 100.0 * static_cast<double>(n) / static_cast<double>(std::max<std::size_t>(1, of)); };
    const auto cells = [](const engine::ClassCount& c) {
        return std::format("{} | {} | {} | {} | {} | {}", c.at(verify::Class::Weak), c.at(verify::Class::Identified), c.at(verify::Class::Unverified),
                           c.at(verify::Class::NotJudged), c.at(verify::Class::NoCompile), c.at(verify::Class::NoSource));
    };
    for (const ScoreRow& r : in.modules) {
        std::size_t n = 0;
        for (const verify::Class c : verify::kClassScored) {
            n += static_cast<std::size_t>(r.counts.at(c));
            total[c] += r.counts.at(c);
        }
        all += n;
        lines.push_back(std::format("| {} | {} | {} | {} ({:.0f}%) | {} | not tested | not tested |", r.name, r.kind, n, r.counts.at(verify::Class::Verified),
                                    percent(r.counts.at(verify::Class::Verified), n), cells(r.counts)));
    }
    if (all != 0) {
        lines.push_back(std::format("| **all measured** | | {} | {} ({:.0f}%) | {} | | |", all, total.at(verify::Class::Verified),
                                    percent(total.at(verify::Class::Verified), all), cells(total)));
    }
    lines.push_back("");
    lines.push_back(std::format("{} modules measured so far (the corpus has {} 32-bit, {} 16-bit).", in.modules.size(), in.corpus_32, in.corpus_16));
    lines.push_back("");
    return text::join(lines, "\n");
}

}  // namespace cb::source::module_writer
