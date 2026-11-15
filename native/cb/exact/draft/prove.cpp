#include "cb/exact/draft/prove.hpp"

#include <algorithm>
#include <cmath>
#include <format>

#include <nlohmann/json.hpp>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/exact/draft/text.hpp"
#include "cb/exact/x86.hpp"

namespace cb::exact::draft::prove {

namespace {

// The kinds of difference and the refiners each asks for first (aim's kinds; the rest of the refiners come after them).
const std::map<std::string, std::set<std::string>, std::less<>>& prove_aim_kinds() {
    static const std::map<std::string, std::set<std::string>, std::less<>> kinds = {
        {"frame", {"frame_names", "registers_last", "frame_more", "lead_filler", "register_slots"}},
        {"width", {"pointer_widths", "char_widens", "unsigned_registers", "unmasked", "byte_heads", "unsigned_fields", "high_words", "word_offsets", "scaled_places"}},
        {"sign", {"unsigned_locals", "unsigned_fields", "unsigned_registers"}},
        {"order", {"adjacent_swaps", "return_cases_first", "empty_cases_first", "default_after", "default_last", "switch_breaks", "flat_returns",
                   "guard_returns", "guard_shared", "guard_flat", "shared_tails", "push_tails", "loop_tests", "or_loops", "loop_guards", "for_loops",
                   "label_sinks", "shared_exit", "case_guards", "chain_tails", "common_tails", "heads_variant", "no_default", "assign_loops", "tail_continues"}},
        {"shape", {"assign_in_args", "return_sources", "compound_assigns", "chain_assigns", "store_increments", "pre_steps", "pre_increments",
                   "assign_in_tests", "and_returns", "register_temps", "split_ternaries", "compare_results", "const_tests", "register_reads",
                   "register_copies", "word_offsets", "call_pairs", "string_variants", "address_variants"}},
    };
    return kinds;
}

// the rewrites of a statement: kept only where the code differs
bool prove_line_kind(std::string_view refiner) {
    const auto& shape = prove_aim_kinds().find("shape")->second;
    return shape.contains(std::string(refiner));
}

bool prove_word(char c) { return std::isalnum(static_cast<unsigned char>(c)) || c == '_'; }

std::vector<std::string> prove_split_lines(std::string_view s) {
    std::vector<std::string> out;
    std::size_t at = 0;
    for (;;) {
        const std::size_t nl = s.find('\n', at);
        if (nl == std::string_view::npos) {
            out.emplace_back(s.substr(at));
            return out;
        }
        out.emplace_back(s.substr(at, nl - at));
        at = nl + 1;
    }
}

std::string prove_join_lines(const std::vector<std::string>& lines) {
    std::string out;
    for (std::size_t i = 0; i < lines.size(); ++i) out += (i ? "\n" : "") + lines[i];
    return out;
}

// the compiler's errors as (line, message): `(12) : error C2065: text`, a warning left out
std::vector<std::pair<std::size_t, std::string>> prove_errors(std::string_view console) {
    static const Regex line(R"(\((\d+)\) : (?:error \w+: )?(.*)$)", RegexFlags::Multiline);
    std::vector<std::pair<std::size_t, std::string>> out;
    line.for_each_match(console, [&](const RegexMatch& m) {
        const std::string message(m.group(2));
        if (message.find("warning") == std::string::npos)
            if (const auto n = cb::text::parse_int<std::size_t>(m.group(1), 10)) out.emplace_back(*n, message);
        return true;
    });
    return out;
}

// the identifiers of a line, each once, in the order they appear
std::vector<std::string> prove_identifiers(std::string_view line) {
    std::vector<std::string> out;
    for (std::size_t i = 0; i < line.size();) {
        if (!prove_word(line[i])) {
            ++i;
            continue;
        }
        std::size_t j = i;
        while (j < line.size() && prove_word(line[j])) ++j;
        if (!std::isdigit(static_cast<unsigned char>(line[i]))) {
            const std::string name(line.substr(i, j - i));
            if (std::find(out.begin(), out.end(), name) == out.end()) out.push_back(name);
        }
        i = j;
    }
    return out;
}

// the source with a pointer declaration `[register ][unsigned ]char *x;` made `int x;` (the lines that have it; none: nullopt)
std::optional<std::string> prove_pointer_to_int(std::string_view source, const std::string& x, bool allow_unsigned) {
    std::vector<std::string> lines = prove_split_lines(source);
    bool changed = false;
    for (std::string& line : lines) {
        std::string_view rest = line;
        const std::size_t indent = rest.find_first_not_of(" \t");
        if (indent == std::string_view::npos) continue;
        const std::string_view lead = rest.substr(0, indent);
        rest.remove_prefix(indent);
        const bool reg = rest.starts_with("register ");
        if (reg) rest.remove_prefix(9);
        if (allow_unsigned && rest.starts_with("unsigned ")) rest.remove_prefix(9);
        if (rest != "char *" + x + ";") continue;
        line = std::string(lead) + (reg ? "register " : "") + "int " + x + ";";
        changed = true;
    }
    if (!changed) return std::nullopt;
    return prove_join_lines(lines);
}

}  // namespace

// ---- verdicts ------------------------------------------------------------------------------------------------------

std::string Verdict::text() const {
    switch (kind) {
        case Kind::Identical: return "identical";
        case Kind::Differs:
            return length ? std::format("differs: {} bytes, length {} not {}", bytes, length->first, length->second)
                          : std::format("differs: {} bytes", bytes);
        case Kind::DoesNotCompile: return "does not compile: " + detail;
        case Kind::RegistersLeft: return "registers left: " + detail;
        case Kind::NoFunction: return "no function compiled";
    }
    return {};
}

Verdict Verdict::registers_left(const std::vector<std::string>& regs) {
    std::string joined;
    for (const std::string& r : regs) joined += (joined.empty() ? "" : " ") + r;
    return {Kind::RegistersLeft, 0, std::nullopt, joined};
}

Rank rank(const Verdict& v) {
    if (v.kind == Verdict::Kind::Differs) {
        const std::int64_t gap = v.length ? std::abs(static_cast<std::int64_t>(v.length->first) - static_cast<std::int64_t>(v.length->second)) : 0;
        return {0, gap ? 1 : 0, static_cast<std::int64_t>(v.bytes) + gap};   // (what is missing counts as differing)
    }
    return {v.kind == Verdict::Kind::DoesNotCompile ? 1 : 2, 0, 0};
}

bool hopeless(const Verdict& v, std::uint32_t function_length) {
    if (!v.differs() || function_length == 0) return false;
    const std::int64_t gap = v.length ? std::abs(static_cast<std::int64_t>(v.length->first) - static_cast<std::int64_t>(v.length->second)) : 0;
    return static_cast<double>(gap) > std::max(16.0, 0.25 * function_length);
}

// ---- mending ---------------------------------------------------------------------------------------------------------

std::optional<std::string> compile_fix(std::string_view source, std::string_view console) {
    const auto errors = prove_errors(console);
    if (errors.empty()) return std::nullopt;
    const auto& [n, message] = errors.front();
    std::vector<std::string> lines = prove_split_lines(source);
    if (n == 0 || n > lines.size()) return std::nullopt;
    const std::string& l = lines[n - 1];
    std::string fixed;
    static const Regex left_of(R"(left of .->f(\d+). must have a struct)");
    static const Regex member(R"(((?:\((?:[^()]|\((?:[^()]|\([^()]*\))*\))*\)|[A-Za-z_]\w*(?:\[[^\]]*\])?)(?:(?:\.|->)f\d+)*)->f(\d+)\b)");
    static const Regex indirection(R"((?<![\w)\]])\*(?!\()(\s*[A-Za-z_]\w*))");
    static const Regex subscript(R"(\b([A-Za-z_]\w*)\[)");
    if (const auto m = left_of.search(message)) {
        const std::string f(m->group(1));
        std::string out;
        bool done = false;
        std::size_t at = 0;
        member.for_each_match(l, [&](const RegexMatch& x) {
            if (x.group(2) != f) return true;   // another member's: the first of this one is mended
            out += l.substr(at, x.start() - at) + "*(int *)((char *)" + std::string(x.group(1)) + " + " + f + ")";
            at = x.end();
            done = true;
            return false;
        });
        fixed = done ? out + l.substr(at) : l;
    } else if (message.find("illegal indirection") != std::string::npos) {
        fixed = indirection.replace_first(l, "*(char *)$1");
    } else if (message.find("subscript on non-array") != std::string::npos) {
        fixed = subscript.replace_first(l, "((int *)$1)[");
    } else {
        return std::nullopt;
    }
    if (fixed == l) return std::nullopt;
    lines[n - 1] = fixed;
    return prove_join_lines(lines);
}

std::vector<std::string> compile_fixes(std::string_view source, std::string_view console, const BytePointers& byte_pointers) {
    const auto errors = prove_errors(console);
    if (errors.empty()) return {};
    const auto& [n, message] = errors.front();
    std::vector<std::string> lines = prove_split_lines(source);
    if (n == 0 || n > lines.size()) return {};
    const std::string& l = lines[n - 1];
    std::vector<std::string> out;
    static const Regex indirection(R"((?<![\w)\]])\*(?!\()\s*(?:\+\+|--)?([A-Za-z_]\w*))");
    static const Regex subscript(R"(\b([A-Za-z_]\w*)\[)");
    const bool illegal = message.find("illegal indirection") != std::string::npos;
    std::optional<std::string> pointer;
    if (illegal) {
        if (const auto m = indirection.search(l)) pointer = std::string(m->group(1));
    } else if (message.find("subscript on non-array") != std::string::npos) {
        if (const auto m = subscript.search(l)) pointer = std::string(m->group(1));
    }
    if (pointer) {
        const bool bytes = byte_pointers.contains(*pointer);   // (read a byte at a time: char)
        for (const char* type : bytes ? std::initializer_list<const char*>{"char", "int"} : std::initializer_list<const char*>{"int", "char"})
            if (auto d = text::declare_pointer(source, *pointer, type)) out.push_back(std::move(*d));
        if (illegal && !bytes) {   // (or a word through a cast in place: the variable stays an int)
            std::string fixed;
            bool done = false;
            for (std::size_t i = 0; i < l.size() && !done; ++i) {
                if (l[i] != '*' || (i > 0 && (prove_word(l[i - 1]) || l[i - 1] == ')' || l[i - 1] == ']'))) continue;
                if (i + 1 < l.size() && l[i + 1] == '(') continue;
                std::size_t j = i + 1;
                while (j < l.size() && std::isspace(static_cast<unsigned char>(l[j]))) ++j;
                if (l.compare(j, pointer->size(), *pointer) != 0) continue;
                if (j + pointer->size() < l.size() && prove_word(l[j + pointer->size()])) continue;
                fixed = l.substr(0, i) + "*(int *)" + l.substr(i + 1);
                done = true;
            }
            if (done && fixed != l) {
                std::vector<std::string> changed = lines;
                changed[n - 1] = fixed;
                out.push_back(prove_join_lines(changed));
            }
        }
    }
    if (message.find("bad left operand") != std::string::npos || message.find("bad right operand") != std::string::npos) {
        // (a pointer in arithmetic only ints take: an int)
        for (const std::string& x : prove_identifiers(l))
            if (auto d = prove_pointer_to_int(source, x, true)) out.push_back(std::move(*d));
    }
    if (message.find("2 pointers") != std::string::npos) {   // (two pointers added: one of them an int -- each way)
        for (const std::string& x : prove_identifiers(l))
            if (auto d = prove_pointer_to_int(source, x, false)) out.push_back(std::move(*d));
    }
    if (auto one = compile_fix(source, console)) out.push_back(std::move(*one));
    return out;
}

std::string canon(std::string_view source) {
    // the spacing one ...
    std::string t;
    bool blank = false;
    for (char c : source) {
        if (c == ' ' || c == '\t') {
            if (!blank) t += ' ';
            blank = true;
        } else {
            t += c;
            blank = false;
        }
    }
    // ... and a doubled pair of parentheses one: ((x)) with no parentheses inside x beyond one level
    static const Regex doubled(R"(\(\(([^()]*(?:\([^()]*\)[^()]*)*)\)\))");
    for (int i = 0; i < 6; ++i) {
        const std::string next = doubled.replace_all(t, "($1)");
        if (next == t) break;
        t = next;
    }
    return t;
}

DeclSig decl_sig(std::string_view source) {
    const text::BodySplit parts = text::split_body(source);
    // the declarations of the head after its first line, and of the body's first paragraph
    std::string head_rest = parts.head;
    if (const std::size_t nl = head_rest.find('\n'); nl != std::string::npos) head_rest = head_rest.substr(nl + 1);
    std::string first_paragraph = parts.body;
    if (const std::size_t blank = first_paragraph.find("\n\n"); blank != std::string::npos) first_paragraph.resize(blank);
    const std::string scanned = head_rest + first_paragraph;

    static const Regex declaration(R"(^[ \t]*((?:register )?(?:unsigned )?(?:char|int|long|short|struct \w+|double|float)\b[^;(]*);)", RegexFlags::Multiline);
    static const Regex name(R"(\b(?:[a-z]|v\d+\w*|r_\w+|_p\w*)\b(?=[\[;, ]|$))");
    std::vector<std::string> sig;
    declaration.for_each_match(scanned, [&](const RegexMatch& m) {
        sig.push_back(name.replace_all(m.group(1), "X"));   // (names aside)
        return true;
    });
    std::sort(sig.begin(), sig.end());
    static const Regex returns(R"(\breturn [^;])");
    return {std::move(sig), returns.test(parts.body)};
}

std::optional<bool> probes(ByteView code) {
    const std::vector<x86::Insn> ins = x86::disasm(code, 0, static_cast<std::uint32_t>(std::min<std::size_t>(code.size(), 16)), false);
    if (ins.size() < 2 || ins[0].mnemonic != "push" || ins[0].op_str != "bp" || ins[1].op_str != "bp, sp") return std::nullopt;
    return ins.size() > 3 && ins[2].op_str.starts_with("ax, ") && (ins[2].mnemonic == "mov" || ins[2].mnemonic == "xor") && ins[3].mnemonic == "call";
}

std::string with_switches(std::string_view source, const Settings& settings) {
    std::vector<std::string> want;
    for (std::string_view s : cb::text::split_ws(settings.switches)) want.emplace_back(s);
    const std::vector<std::string> original = want;
    const auto has = [&](const std::string& s) { return std::find(want.begin(), want.end(), s) != want.end(); };
    // (code of the 8087's: compiled for it -- the profile's "fp_switch")
    if (settings.fp && settings.fp_switch && !has(*settings.fp_switch)) want.push_back(*settings.fp_switch);
    // (a stack probe in the prologue, or none: the switch that leaves it out -- "probe_switch" -- as it shows)
    if (settings.probe_switch) {
        if (settings.probes && *settings.probes) std::erase(want, *settings.probe_switch);
        else if (settings.probes && !*settings.probes && !has(*settings.probe_switch)) want.push_back(*settings.probe_switch);
    }
    if (!settings.file_switches || want == original) return std::string(source);
    std::string joined;
    for (const std::string& w : want) joined += (joined.empty() ? "" : " ") + w;
    return "/* " + *settings.file_switches + ": " + joined + " */\n" + std::string(source);
}

// ---- Priors ------------------------------------------------------------------------------------------------------------

Priors::Priors(std::filesystem::path file) : file_(std::move(file)) {
    if (file_.empty()) return;
    try {
        const Json j = json::load(file_);
        if (j.is_object())
            for (const auto& [kind, table] : j.items())
                if (table.is_object())
                    for (const auto& [refiner, n] : table.items())
                        if (n.is_number_integer()) wins_[kind][refiner] = n.get<std::int64_t>();
    } catch (const Error&) {   // no history yet, or one that cannot be read: the search starts without
    }
}

std::int64_t Priors::wins(std::string_view kind, std::string_view refiner) const {
    const std::lock_guard<std::mutex> lock(mutex_);
    const auto k = wins_.find(kind);
    if (k == wins_.end()) return 0;
    const auto r = k->second.find(std::string(refiner));
    return r == k->second.end() ? 0 : r->second;
}

void Priors::learn(const std::vector<std::string>& kinds, std::string_view refiner) {
    const std::lock_guard<std::mutex> lock(mutex_);
    for (const std::string& kind : kinds.empty() ? std::vector<std::string>{"shape"} : kinds) ++wins_[kind][std::string(refiner)];
    if (file_.empty()) return;
    try {   // (merged with what other processes wrote meanwhile: the larger count of each wins)
        const Json disk = json::load(file_);
        if (disk.is_object())
            for (const auto& [kind, table] : disk.items())
                if (table.is_object())
                    for (const auto& [name, n] : table.items())
                        if (n.is_number_integer()) {
                            auto& mine = wins_[kind][name];
                            mine = std::max(mine, n.get<std::int64_t>());
                        }
    } catch (const Error&) {
    }
    Json out = Json::object();
    for (const auto& [kind, table] : wins_) {
        Json t = Json::object();
        for (const auto& [name, n] : table) t[name] = n;
        out[kind] = std::move(t);
    }
    try {
        fs::make_dirs(file_.parent_path());
        const std::filesystem::path temp = file_.string() + "." + std::to_string(fs::process_id()) + ".tmp";
        json::save(temp, out, json::Style::Compact);
        std::error_code ec;
        std::filesystem::rename(temp, file_, ec);   // (a replace: the old file is gone only when the new one is whole)
        if (ec) std::filesystem::remove(temp, ec);
    } catch (const Error&) {   // the history is a convenience: a state folder that cannot be written is not a failure
    }
}

// ---- Proof -------------------------------------------------------------------------------------------------------------

Proof::Proof(Settings settings, Collaborators collaborators, Priors* priors, std::uint32_t function_length)
    : settings_(std::move(settings)), c_(std::move(collaborators)), priors_(priors), length_(function_length) {
    CB_ASSERT(c_.compile != nullptr && c_.judge != nullptr, "proving needs a compiler and a comparison");
}

Outcome Proof::prove(const std::string& source, int tries, int budget) {
    std::vector<std::string> frontier = {source};
    std::optional<Outcome> best;
    std::optional<std::pair<std::string, std::string>> first_console;   // (the source the compiler first refused, what it said)
    const BytePointers bytes = c_.byte_pointers ? c_.byte_pointers() : BytePointers{};
    for (int depth = 0; depth <= tries; ++depth) {
        std::vector<std::string> next;
        for (const std::string& s : frontier) {
            if (budget <= 0) break;
            --budget;
            ++spent_;
            const batch::CompileOutcome made = c_.compile(s);
            if (!made.object) {
                if (!first_console) first_console = {s, made.console};
                if (depth < tries) {
                    std::vector<std::string> fixes = compile_fixes(s, made.console, bytes);
                    if (fixes.size() > 3) fixes.resize(3);
                    next.insert(next.end(), fixes.begin(), fixes.end());
                }
                continue;
            }
            const Verdict v = c_.judge(*made.object, 0);
            if (v.identical()) return {s, v};
            if (!best || rank(v) < rank(best->verdict)) best = Outcome{s, v};
        }
        if (next.empty() || budget <= 0) break;
        frontier = std::move(next);
    }
    if (best) return *best;
    const auto [s, console] = first_console ? *first_console : std::make_pair(source, std::string());
    // the error, not a warning before it, and not the note of which pass stopped
    std::vector<std::string> kept;
    for (std::string_view line : cb::text::lines(cb::text::trim(console)))
        if (line.find("(the pass") == std::string_view::npos) kept.emplace_back(line);
    std::vector<std::string> errors;
    for (const std::string& l : kept)
        if (l.find("warning") == std::string::npos) errors.push_back(l);
    const std::vector<std::string>& shown = errors.empty() ? kept : errors;
    std::string last = shown.empty() ? std::string() : (errors.empty() ? shown.back() : shown.front());
    if (last.size() > 100) last.resize(100);
    return {s, Verdict::not_compiled(last)};
}

std::map<std::string, Verdict> Proof::batched(const std::vector<std::string>& sources) {
    // {source: verdict} of those of `sources` a batch compiled (one run of the compiler, counted as two compiles); the rest are
    // left to prove()
    std::map<std::string, Verdict> out;
    if (sources.size() < 2 || !c_.compile_many) return out;
    std::map<std::size_t, std::pair<Bytes, std::size_t>> got;
    try {
        got = c_.compile_many(sources);
    } catch (const Error&) {   // the compiler refused the batch: each is proved alone
        return out;
    }
    spent_ += 2;
    for (const auto& [i, placed] : got) {
        try {
            out.emplace(sources.at(i), c_.judge(placed.first, placed.second));
        } catch (const Error&) {   // one the comparison cannot read is left to be proved alone
        }
    }
    return out;
}

std::optional<aim::Diagnosis> Proof::aimed(const std::string& source) {
    if (const auto it = aims_.find(source); it != aims_.end()) return it->second;
    std::optional<aim::Diagnosis> d;
    if (c_.diagnose) {
        try {
            d = c_.diagnose(source);
        } catch (const Error&) {   // a candidate that cannot be diagnosed is ordered without the aim
        }
    }
    aims_[source] = d;
    return d;
}

Proof::Ordered Proof::ordered(const std::string& source) {
    // the refiners in the order a reading's difference asks for: the kinds diagnosed first (by their wins so far), the rest after
    const auto d = settings_.aim ? aimed(source) : std::nullopt;
    std::vector<std::pair<std::string, std::size_t>> counted;
    if (d)
        for (const auto& [kind, n] : d->kinds) counted.emplace_back(std::string(aim::kind_name(kind)), n);
    std::stable_sort(counted.begin(), counted.end(), [](const auto& a, const auto& b) { return a.second > b.second; });
    Ordered out;
    for (const auto& [kind, n] : counted) out.kinds.push_back(kind);
    const std::vector<std::string> scored = out.kinds.empty() ? std::vector<std::string>{"shape"} : out.kinds;
    const auto score = [&](const Refiner* r) {
        std::int64_t s = 0;
        if (priors_)
            for (const std::string& k : scored) s += priors_->wins(k, r->name);
        return -s;
    };
    std::vector<const Refiner*> first, rest;
    for (const Refiner& r : c_.refiners) {
        const bool asked = std::any_of(out.kinds.begin(), out.kinds.end(), [&](const std::string& k) {
            const auto it = prove_aim_kinds().find(k);
            return it != prove_aim_kinds().end() && it->second.contains(r.name);
        });
        (asked ? first : rest).push_back(&r);
    }
    std::stable_sort(first.begin(), first.end(), [&](const Refiner* a, const Refiner* b) { return score(a) < score(b); });
    std::stable_sort(rest.begin(), rest.end(), [&](const Refiner* a, const Refiner* b) { return score(a) < score(b); });
    // (the lines of the differences, when the differences are of the statements' shape alone)
    std::optional<std::vector<std::uint32_t>> lines;
    if (d && !out.kinds.empty() && std::all_of(out.kinds.begin(), out.kinds.end(), [](const std::string& k) { return k == "shape"; })) {
        lines = d->lines;
        if (lines->size() > 6) lines->resize(6);
    }
    for (const Refiner* r : first) out.list.push_back({r, prove_line_kind(r->name) ? lines : std::nullopt});
    for (const Refiner* r : rest) out.list.push_back({r, std::nullopt});
    return out;
}

namespace {

// does `alt` change `s` at one of these source lines (or within one of them)
bool prove_touches(const std::string& s, const std::string& alt, const std::vector<std::uint32_t>& lines) {
    if (lines.empty()) return true;
    for (const aim::Opcode& op : aim::opcodes(prove_split_lines(s), prove_split_lines(alt))) {
        if (op.tag == aim::Opcode::Tag::Equal) continue;
        for (std::uint32_t l : lines) {
            const std::int64_t at = static_cast<std::int64_t>(l) - 1;
            if (static_cast<std::int64_t>(op.a1) - 1 <= at && at <= static_cast<std::int64_t>(std::max(op.a2, op.a1 + 1))) return true;
        }
    }
    return false;
}

}  // namespace

Outcome Proof::refine(const Reading& best, int rounds, std::optional<std::int64_t> budget_in, const std::vector<Reading>& seeds) {
    const std::int64_t budget = budget_in.value_or(settings_.refine_budget);
    const std::size_t beam_size = settings_.beam;
    std::vector<Reading> beam;
    std::set<std::string> seen;
    std::vector<Reading> start = {best};
    start.insert(start.end(), seeds.begin(), seeds.end());
    for (const Reading& r : start)
        if (seen.insert(canon(r.source)).second) beam.push_back(r);
    // the nearest beam_size readings, and the nearest of each other kind of reading -- its declarations -- up to beam_size more
    const auto keep = [&](std::vector<Reading> xs) {
        std::stable_sort(xs.begin(), xs.end(), [](const Reading& a, const Reading& b) { return a.rank < b.rank; });
        std::vector<Reading> out(xs.begin(), xs.begin() + static_cast<std::ptrdiff_t>(std::min(beam_size, xs.size())));
        std::set<DeclSig> kinds;
        for (const Reading& r : out) kinds.insert(decl_sig(r.source));
        for (std::size_t i = beam_size; i < xs.size(); ++i) {
            const DeclSig k = decl_sig(xs[i].source);
            if (!kinds.contains(k) && out.size() < 2 * beam_size) {
                kinds.insert(k);
                out.push_back(xs[i]);
            }
        }
        return out;
    };
    beam = keep(beam);
    std::int64_t spent = 0;
    Reading top = beam.front();
    for (int round = 0; round < rounds; ++round) {
        std::vector<Reading> pool;
        for (const Reading& reading : beam) {
            const Ordered order = settings_.aim ? ordered(reading.source) : [&] {
                Ordered plain;
                for (const Refiner& r : c_.refiners) plain.list.push_back({&r, std::nullopt});
                return plain;
            }();
            // the rewrites of this reading, in the aim's order: those at the lines that differ, then the rest
            std::vector<std::pair<const Refiner*, std::string>> work, deferred;
            for (const auto& [refiner, lines] : order.list) {
                std::vector<std::string> alts;
                try {
                    alts = refiner->rewrite(reading.source);
                } catch (const Error&) {   // a rewrite that cannot read this reading makes none
                    continue;
                }
                for (std::string& alt : alts) {
                    if (alt.empty() || alt == reading.source || !seen.insert(canon(alt)).second) continue;
                    (lines && !prove_touches(reading.source, alt, *lines) ? deferred : work).emplace_back(refiner, std::move(alt));
                }
            }
            work.insert(work.end(), deferred.begin(), deferred.end());
            const std::size_t chunk_size = std::max<std::size_t>(1, settings_.batch);
            for (std::size_t w0 = 0; w0 < work.size(); w0 += chunk_size) {
                if (spent >= budget) break;
                const std::size_t w1 = std::min(work.size(), w0 + chunk_size);
                std::vector<std::string> chunk;
                for (std::size_t i = w0; i < w1; ++i) chunk.push_back(work[i].second);
                const std::map<std::string, Verdict> known = settings_.batch ? batched(chunk) : std::map<std::string, Verdict>{};
                spent += known.empty() ? 0 : 2;
                for (std::size_t i = w0; i < w1; ++i) {
                    const auto& [refiner, alt] = work[i];
                    Verdict v2;
                    std::string got = alt;
                    if (const auto it = known.find(alt); it != known.end()) {
                        v2 = it->second;
                    } else {
                        if (spent >= budget) break;
                        ++spent;
                        const Outcome o = prove(alt, 2, 3);
                        v2 = o.verdict;
                        got = o.source;
                    }
                    if (v2.identical()) {
                        if (settings_.aim && priors_) priors_->learn(order.kinds, refiner->name);
                        return {got, v2};
                    }
                    if (v2.differs()) {
                        if (settings_.aim && priors_ && rank(v2) < reading.rank) priors_->learn(order.kinds, refiner->name);
                        pool.push_back({rank(v2), got, v2});
                    }
                }
            }
        }
        if (pool.empty()) break;
        std::vector<Reading> merged_in = beam;
        merged_in.insert(merged_in.end(), pool.begin(), pool.end());
        std::vector<Reading> merged = keep(merged_in);
        const bool nothing_new = std::all_of(merged.begin(), merged.end(), [&](const Reading& m) {
            return std::any_of(beam.begin(), beam.end(), [&](const Reading& b) { return b.source == m.source && b.verdict == m.verdict; });
        });
        if (!(merged.front().rank < top.rank) && nothing_new) break;   // (nothing nearer, nothing new: the search is done)
        beam = std::move(merged);
        if (beam.front().rank < top.rank) top = beam.front();
        if (spent >= budget) break;
    }
    return {top.source, top.verdict};
}

Outcome Proof::prove_best(const std::string& text, const candidates::RegisterParams& regparam) {
    CB_ASSERT(c_.candidates != nullptr, "proving a text needs the candidates made of it");
    std::vector<candidates::Candidate> made = c_.candidates(text, regparam);
    // each text once -- spacing and doubled parentheses aside; the first readings first, and at most max_candidates of them
    std::set<std::string> distinct;
    std::vector<candidates::Candidate> cands;
    for (candidates::Candidate& c : made)
        if (distinct.insert(canon(c.source)).second) cands.push_back(std::move(c));
    if (cands.size() > settings_.max_candidates) cands.resize(settings_.max_candidates);
    for (candidates::Candidate& c : cands) c.source = with_switches(c.source, settings_);

    std::optional<Reading> best;
    std::vector<Reading> tops;   // (every reading that compiled, near or far: the beam's seeds)
    spent_ = 0;
    std::map<std::string, Verdict> known;   // (verdicts a batch gave)
    for (std::size_t ci = 0; ci < cands.size(); ++ci) {
        std::string source = cands[ci].source;
        Verdict v;
        if (!cands[ci].registers_left.empty()) {
            v = Verdict::registers_left(cands[ci].registers_left);
        } else if (spent_ >= settings_.function_budget && best) {
            break;   // (compiled enough for one function: the best so far)
        } else {
            if (settings_.batch && !known.contains(source)) {   // (the next ones compiled together, one run of the compiler)
                std::vector<std::string> ahead;
                for (std::size_t j = ci; j < cands.size() && ahead.size() < settings_.batch; ++j)
                    if (cands[j].registers_left.empty() && !known.contains(cands[j].source)) ahead.push_back(cands[j].source);
                for (auto& [s, verdict] : batched(ahead)) known.insert_or_assign(s, std::move(verdict));
            }
            if (const auto it = known.find(source); it != known.end()) {
                v = it->second;
            } else {
                const Outcome o = prove(source);
                v = o.verdict;
                source = o.source;   // (as the compiler's errors mended it)
            }
            if (!v.identical() && c_.string_variants) {   // (a number that is a string's place: the string)
                for (const std::string& alt : c_.string_variants(source)) {
                    const Outcome o = prove(alt);
                    if (o.verdict.identical()) {
                        source = o.source;
                        v = o.verdict;
                        break;
                    }
                }
            }
        }
        if (v.identical()) return {source, v};
        const Rank r = rank(v);
        if (v.differs()) tops.push_back({r, source, v});
        if (!best || r < best->rank) best = Reading{r, source, v};
    }
    if (!best) return {"", Verdict::no_function()};
    if (best->verdict.differs() && hopeless(best->verdict, length_)) return {best->source, best->verdict};   // (too far for rewrites to close: the reading's own fault)
    if (best->verdict.differs()) {   // (the nearest that compile, rewritten -- a beam of them)
        std::stable_sort(tops.begin(), tops.end(), [](const Reading& a, const Reading& b) { return a.rank < b.rank; });
        std::vector<Reading> seeds(tops.begin(), tops.begin() + static_cast<std::ptrdiff_t>(std::min(settings_.beam, tops.size())));
        // (and the nearest of each other way of declaring the locals -- a buffer, a pointer's type: the beam starts from each
        // reading, not only the nearest)
        std::map<DeclSig, Reading> kinds;
        std::vector<DeclSig> order;
        for (const Reading& r : tops)
            if (kinds.emplace(decl_sig(r.source), r).second) order.push_back(decl_sig(r.source));
        std::size_t added = 0;
        for (const DeclSig& sig : order) {
            if (added >= settings_.beam) break;
            const Reading& r = kinds.at(sig);
            if (std::none_of(seeds.begin(), seeds.end(), [&](const Reading& s) { return s.source == r.source && s.verdict == r.verdict; })) {
                seeds.push_back(r);
                ++added;
            }
        }
        return refine(*best, 6, std::nullopt, seeds);
    }
    return {best->source, best->verdict};
}

Outcome Proof::prove_texts(const std::vector<std::pair<std::string, candidates::RegisterParams>>& texts_in) {
    std::vector<std::pair<std::string, candidates::RegisterParams>> texts = texts_in;
    for (const auto& [body, regparam] : texts_in) {
        // a register loaded from a parameter and then given a new value: maybe a local the parameter started -- the compiler says
        std::set<std::string> given;
        for (const auto& [reg, index] : regparam) {
            const std::string assigned = reg + " = ";
            for (std::string_view line : cb::text::lines(body))
                if (cb::text::ltrim(line).starts_with(assigned)) {
                    given.insert(reg);
                    break;
                }
        }
        for (const std::string& g : given) {
            candidates::RegisterParams without;
            for (const auto& [reg, index] : regparam)
                if (reg != g) without.insert_or_assign(reg, index);
            texts.emplace_back(body, without);
        }
        if (given.size() > 1) {
            candidates::RegisterParams without;
            for (const auto& [reg, index] : regparam)
                if (!given.contains(reg)) without.insert_or_assign(reg, index);
            texts.emplace_back(body, without);
        }
    }
    std::optional<std::pair<Rank, Outcome>> best;
    for (const auto& [body, regparam] : texts) {
        Outcome o = prove_best(body, regparam);
        if (o.verdict.identical()) return o;
        const Rank r = rank(o.verdict);
        if (!best || r < best->first) best = std::make_pair(r, std::move(o));
    }
    if (!best) return {"", Verdict::no_function()};
    return best->second;
}

}  // namespace cb::exact::draft::prove
