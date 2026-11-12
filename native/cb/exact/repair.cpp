#include "cb/exact/repair.hpp"

#include <algorithm>
#include <format>
#include <map>
#include <set>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/exact/omf.hpp"

namespace cb::exact::repair {

namespace stdfs = std::filesystem;

namespace {

// "%+d"
std::string repair_signed(std::int64_t n) { return (n >= 0 ? "+" : "") + std::to_string(n); }

bool repair_word_char(char c) { return std::isalnum(static_cast<unsigned char>(c)) || c == '_'; }

// Every member of an enumeration named by ordinals: an upper-case family and its number (DK0, DK46, TK12), as a whole word.
struct RepairMember {
    std::size_t start = 0, end = 0;
    std::string family;
    std::int64_t number = 0;
    std::string token;
};

std::vector<RepairMember> repair_members(std::string_view text) {
    static const Regex pattern(R"(\b([A-Z][A-Z_]*[A-Z_])([1-9]\d{0,2}|0)\b)");
    std::vector<RepairMember> out;
    pattern.for_each_match(text, [&](const RegexMatch& m) {
        out.push_back({m.start(), m.end(), std::string(m.group(1)), text::parse_int<std::int64_t>(m.group(2), 10).value_or(0), std::string(m.whole())});
        return true;
    });
    return out;
}

// The decimal `value` as a whole number in the text -- not part of a name or a fraction -- by where it is.
std::vector<std::size_t> repair_literals(std::string_view text, std::int64_t value) {
    const std::string digits = std::to_string(value);
    std::vector<std::size_t> out;
    for (std::size_t at = text.find(digits); at != std::string_view::npos; at = text.find(digits, at + 1)) {
        const bool before = at > 0 && (repair_word_char(text[at - 1]) || text[at - 1] == '.');
        const std::size_t end = at + digits.size();
        const bool after = end < text.size() && (repair_word_char(text[end]) || text[end] == '.');
        if (!before && !after) out.push_back(at);
    }
    return out;
}

// The text with each member `pick` chooses moved by d.
std::string repair_move_members(std::string_view text, const std::vector<RepairMember>& members, std::int64_t d,
                                const std::function<bool(const RepairMember&)>& pick) {
    std::string out;
    std::size_t at = 0;
    for (const RepairMember& m : members) {
        if (!pick(m)) continue;
        out.append(text.substr(at, m.start - at));
        out += m.family + std::to_string(m.number + d);
        at = m.end;
    }
    out.append(text.substr(at));
    return out;
}

// The verdict of each function a set of rows names (a later row of the same name wins).
struct RepairVerdicts {
    std::map<std::string, pascal_join::Verdict> by_function;
    explicit RepairVerdicts(const std::vector<ledger::Row>& rows) {
        for (const ledger::Row& r : rows) by_function[r.function.value_or("")] = r.verdict;
    }
    std::optional<pascal_join::Verdict> of(const std::optional<std::string>& function) const {
        const auto it = by_function.find(function.value_or(""));
        return it == by_function.end() ? std::nullopt : std::optional(it->second);
    }
};

std::string repair_first_word(std::string_view s) {
    const auto words = text::split_ws(s);
    return words.empty() ? std::string() : std::string(words.front());
}

ledger::Row repair_ledger_row(const pascal_join::Row& r) {
    return ledger::Row{r.label, r.procedure, r.address, r.length, r.verdict, r.notes, {}};
}

// Does the first line declare `PROCEDURE P<h>` or `FUNCTION P<h>` (any case, as a whole word)?
bool repair_declares_piece(std::string_view line, std::string_view h) {
    std::string_view s = text::ltrim(line);
    const std::size_t word = s.find_first_of(" \t");
    if (word == std::string_view::npos) return false;
    if (!text::iequals(s.substr(0, word), "PROCEDURE") && !text::iequals(s.substr(0, word), "FUNCTION")) return false;
    s = text::ltrim(s.substr(word));
    const std::string name = "P" + std::string(h);
    if (s.size() < name.size() || !text::iequals(s.substr(0, name.size()), name)) return false;
    return s.size() == name.size() || !repair_word_char(s[name.size()]);
}

}  // namespace

std::optional<Shift> shifts(const std::vector<std::string>& notes) {
    static const Regex note(R"(\? (\d+) values? (?:each )?([+-]\d+) in the program \(([^)]*)\))");
    for (const std::string& n : notes) {
        const auto m = note.search(n);
        if (!m) continue;
        Shift out;
        std::string_view by = m->group(2);
        if (by.starts_with('+')) by.remove_prefix(1);
        out.by = text::parse_int<std::int64_t>(by, 10).value_or(0);
        for (std::string_view piece : text::split(m->group(3), ',')) {
            std::string_view s = text::trim(piece);
            while (s.starts_with('+')) s.remove_prefix(1);
            if (s.empty()) continue;
            if (const auto off = text::parse_int<std::int64_t>(s, 16)) out.offsets.push_back(*off);
        }
        return out;
    }
    return std::nullopt;
}

std::vector<Candidate> candidates(std::string_view text, std::span<const std::int64_t> mine, std::int64_t d) {
    std::vector<Candidate> out;
    std::set<std::int64_t> want;
    for (std::int64_t v : mine) want.insert(v & 0xFF);
    const std::vector<RepairMember> members = repair_members(text);

    // the families that have a member numbered as a wanted value, in the order they appear, with those members
    std::vector<std::pair<std::string, std::set<std::string>>> named;
    for (const RepairMember& m : members) {
        if (!want.contains(m.number & 0xFF)) continue;
        auto it = std::find_if(named.begin(), named.end(), [&](const auto& f) { return f.first == m.family; });
        if (it == named.end()) it = named.insert(named.end(), {m.family, {}});
        it->second.insert(m.token);
    }
    for (const auto& [family, these] : named) {
        const auto in_family = [&](const RepairMember& m) { return m.family == family; };
        const std::string alone = repair_move_members(text, members, d, [&](const RepairMember& m) { return in_family(m) && these.contains(m.token); });
        if (alone != text) {
            std::string shown;
            for (const std::string& t : these) shown += (shown.empty() ? "" : ", ") + t;   // sorted
            out.push_back({shown + " moved by " + repair_signed(d), alone});
        }
        const std::string every = repair_move_members(text, members, d, in_family);
        if (every != text && every != alone) out.push_back({"every " + family + "<n> moved by " + repair_signed(d), every});
    }
    for (std::int64_t v : want) {
        const std::vector<std::size_t> hits = repair_literals(text, v);
        const std::string from = std::to_string(v), to = std::to_string(v + d);
        for (std::size_t at : hits) {
            const auto line = std::count(text.begin(), text.begin() + static_cast<std::ptrdiff_t>(at), '\n') + 1;
            out.push_back({std::format("the literal {} made {} (line {})", v, v + d, line),
                           std::string(text.substr(0, at)) + to + std::string(text.substr(at + from.size()))});
        }
        if (hits.size() > 1) {
            std::string all;
            std::size_t at = 0;
            for (std::size_t hit : hits) {
                all.append(text.substr(at, hit - at));
                all += to;
                at = hit + from.size();
            }
            all.append(text.substr(at));
            out.push_back({std::format("every literal {} made {}", v, v + d), all});
        }
    }
    return out;
}

pascal_join::Project only(const pascal_join::Project& project, const std::string& name) {
    pascal_join::Project p = project;
    OrderedMap<std::string, pascal_join::CompilandConfig> one;
    one.insert_or_assign(name, project.config.compilands.at(name));
    p.config.compilands = std::move(one);
    return p;
}

std::optional<stdfs::path> piece_of(const pascal_join::Compiland& compiland, std::string_view h) {
    const std::string wanted = "p" + text::lower(h) + ".pas";
    for (const stdfs::path& dir : compiland.directories()) {
        std::error_code ec;
        for (stdfs::recursive_directory_iterator it(dir, ec), end; !ec && it != end; it.increment(ec)) {
            if (!it->is_regular_file(ec)) continue;
            // (nothing is looked for deeper than three folders below the compiland's own)
            std::size_t depth = 0;
            for (const auto& part : it->path().parent_path().lexically_relative(dir)) depth += part != "." && !part.empty();
            if (depth > 3) continue;
            if (text::lower(fs::to_text(it->path().filename())) != wanted) continue;
            std::string first = fs::read_text(it->path());
            first = first.substr(0, first.find('\n'));
            if (repair_declares_piece(first, h)) return it->path();
        }
    }
    return std::nullopt;
}

namespace {

// The part of the object where the program's own code is: the first segment with bytes that is not data or heap.
std::optional<std::string> repair_code_segment_of(const omf::Object& o) {
    for (const omf::SegDef& sd : o.segdefs) {
        const std::string& x = sd.name;
        if (x == "HEAP" || x == "STACK" || x == "DATA" || x == "CONST" || x == "MEMORY") continue;
        if (const auto* filled = o.filled.find(x); filled && *filled) return x;
    }
    return std::nullopt;
}

std::vector<Job> repair_pascal_jobs(const ledger::Project& project, const pascal_join::Project& pp) {
    std::vector<Job> out;
    for (const pascal_join::Row& r : pascal_join::run(pp, false)) {
        if (r.verdict != pascal_join::Verdict::Differs) continue;
        const std::optional<Shift> s = shifts(r.notes);
        if (!s || !r.procedure || !r.procedure->starts_with('P')) continue;
        const std::string name = repair_first_word(r.label);
        const auto* config = pp.config.compilands.find(name);
        if (!config) continue;
        const pascal_join::Compiland c(pp, name, *config);
        const std::string h = r.procedure->substr(1);
        const auto parts = c.parts();
        const auto part = std::find_if(parts.begin(), parts.end(), [&](const auto& p) {
            const auto names = c.names_of(p);
            return std::find(names.begin(), names.end(), h) != names.end();
        });
        if (part == parts.end()) continue;
        const pascal_join::CompileResult built = pp.toolchain->compile(as_bytes(c.source(*part)), "T.PAS");
        if (!built.object) continue;   // it does not compile alone: nothing to read the compiler's bytes from
        const omf::Object o = omf::read(*built.object);
        const auto seg = repair_code_segment_of(o);
        if (!seg) continue;
        std::optional<std::int64_t> start;
        for (const omf::FunctionSpan& f : omf::functions(o, *seg))
            if (text::upper(f.name) == "P" + h) start = f.offset;
        if (!start) continue;
        const Bytes& code = o.segments.at(*seg);
        Job j;
        for (std::int64_t k : s->offsets) {
            if (*start + k < 0 || static_cast<std::size_t>(*start + k) >= code.size()) break;
            j.mine.push_back(code[static_cast<std::size_t>(*start + k)]);
        }
        if (j.mine.size() != s->offsets.size()) continue;
        const auto found = piece_of(c, h);
        if (!found) continue;
        const auto& dirs = c.directories();
        const auto base = std::find_if(dirs.begin(), dirs.end(), [&](const stdfs::path& d) { return fs::to_text(*found).starts_with(fs::to_text(d)); });
        if (base == dirs.end() || dirs.empty()) continue;
        j.function = r.procedure;
        if (r.address) j.at = *r.address;
        j.source = *found;
        j.dest = dirs.front() / found->lexically_relative(*base);
        j.d = s->by;
        j.unit = name;
        j.check = [pp, name]() {
            std::vector<ledger::Row> rows;
            for (const pascal_join::Row& row : pascal_join::run(only(pp, name), false)) rows.push_back(repair_ledger_row(row));
            return rows;
        };
        out.push_back(std::move(j));
    }
    (void)project;
    return out;
}

std::vector<Job> repair_sourced_jobs(const ledger::Project& project) {
    std::vector<Job> out;
    for (const std::shared_ptr<const ledger::Project>& set : project.sets()) {
        const std::vector<ledger::LabelledToolchain> toolchains = set->toolchains();
        if (toolchains.empty()) continue;
        const SymbolTable symbols = set->symbols();
        for (const ledger::SourceFile& source : set->sources()) {
            const auto per_file = [set, toolchains, symbols, source]() {
                std::vector<ledger::Attempt> attempts;
                for (const ledger::LabelledToolchain& lt : toolchains) {
                    SymbolTable learnt;
                    attempts.push_back({lt.label, ledger::check_file(*set, *lt.toolchain, source.path, source.at, symbols, learnt)});
                }
                return ledger::merge(attempts);
            };
            for (const ledger::Row& r : per_file()) {
                if (r.verdict != pascal_join::Verdict::Differs) continue;
                const std::optional<Shift> s = shifts(r.notes);
                if (!s) continue;
                const modules::CompileResult built =
                    toolchains.front().toolchain->compile(fs::read_file(source.path), {set->headers(), set->include(), set->switches()});
                if (!built.object) continue;
                const omf::Object o = omf::read(*built.object);
                std::optional<std::pair<std::string, std::int64_t>> hit;
                for (const std::string& seg : omf::code_segments(o)) {
                    for (const omf::FunctionSpan& f : omf::functions(o, seg))
                        if (r.function && f.name == *r.function) {
                            hit = std::make_pair(seg, f.offset);
                            break;
                        }
                    if (hit) break;
                }
                if (!hit) continue;
                const Bytes& code = o.segments.at(hit->first);
                Job j;
                for (std::int64_t k : s->offsets) {
                    if (hit->second + k < 0 || static_cast<std::size_t>(hit->second + k) >= code.size()) break;
                    j.mine.push_back(code[static_cast<std::size_t>(hit->second + k)]);
                }
                if (j.mine.size() != s->offsets.size()) continue;
                j.function = r.function;
                if (r.address) j.at = *r.address;
                j.source = j.dest = source.path;
                j.d = s->by;
                j.check = per_file;
                out.push_back(std::move(j));
            }
        }
    }
    return out;
}

}  // namespace

std::vector<Job> jobs(const ledger::Project& project) {
    if (const pascal_join::Project* pascal = project.compilands()) return repair_pascal_jobs(project, *pascal);
    return repair_sourced_jobs(project);
}

int command(const ledger::Project& project, const std::vector<std::string>& args, const Sink& say) {
    return mend(jobs(project), std::find(args.begin(), args.end(), "--write") != args.end(), say);
}

int mend(const std::vector<Job>& todo, bool write, const Sink& say) {
    const auto report = [&](const std::string& line) {
        if (say) say(line);
        else out("{}\n", line);
    };
    int fixed = 0;
    for (const Job& j : todo) {
        const RepairVerdicts before(j.check());
        const std::string text = fs::read_text(j.source);
        std::optional<std::string> kept;
        for (const Candidate& cand : candidates(text, j.mine, j.d)) {
            fs::make_dirs(j.dest.parent_path());
            std::error_code ec;
            std::optional<std::string> had;
            if (stdfs::exists(j.dest, ec)) had = fs::read_text(j.dest);
            fs::write_text(j.dest, cand.text);
            const RepairVerdicts after(j.check());
            // the function is the program's now, and nothing that was identical stopped being so
            bool good = after.of(j.function) == std::optional(pascal_join::Verdict::Identical);
            for (const auto& [function, verdict] : before.by_function) {
                if (!good) break;
                if (verdict != pascal_join::Verdict::Identical) continue;
                const auto it = after.by_function.find(function);
                if (it == after.by_function.end() || it->second != pascal_join::Verdict::Identical) good = false;
            }
            if (good && write) {
                kept = cand.what;
                break;
            }
            // (not kept, or only asked: as it was)
            if (!had) stdfs::remove(j.dest, ec);
            else fs::write_text(j.dest, *had);
            if (good) {
                kept = cand.what + " (not written: --write)";
                break;
            }
        }
        report(std::format("{} {}: {}", j.function.value_or("None"), j.at ? std::format("{:05x}", *j.at) : std::string("-----"),
                           kept ? *kept : "no change of those tried makes it the program's"));
        if (kept) ++fixed;
    }
    report(std::format("{} of {} mended", fixed, todo.size()));
    return 0;
}

}  // namespace cb::exact::repair
