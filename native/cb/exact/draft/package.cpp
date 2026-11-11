#include "cb/exact/draft/package.hpp"

#include <algorithm>
#include <cctype>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"

namespace cb::exact::draft {
namespace {

namespace txt = cb::text;

constexpr std::size_t kVerdictDetail = 100;          // how much of an error message a verdict keeps
constexpr std::size_t kTableLookBack = 8;            // instructions before a jump through a table to look for its `cmp R, N`
constexpr std::size_t kInlineGotoRounds = 8;
constexpr std::size_t kInlineGotoMaxTail = 40;
constexpr std::string_view kRegisterVariables = "/* register variables";
constexpr std::uint8_t kInt = 0xCD, kWait = 0x9B, kEsc0 = 0xD8, kNop = 0x90;
constexpr std::uint8_t kEmulatorFirst = 0x34, kEmulatorLastEsc = 0x3B, kEmulatorSegEsc = 0x3C, kEmulatorWait = 0x3D;

bool is_word(char c) { return std::isalnum(static_cast<unsigned char>(c)) != 0 || c == '_'; }
bool is_lower_hex(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'); }

// "0x1c" or decimal digits, whole: the number (a constant in an operand).
std::optional<std::int64_t> number_operand(std::string_view s) {
    if (s.starts_with("0x")) {
        if (s.size() < 3 || !std::ranges::all_of(s.substr(2), is_lower_hex)) return std::nullopt;
        return txt::parse_int<std::int64_t>(s, 16);
    }
    if (s.empty() || !std::ranges::all_of(s, [](char c) { return c >= '0' && c <= '9'; })) return std::nullopt;
    return txt::parse_int<std::int64_t>(s, 10);
}

// `cs:[bx + 0x1234]` or `cs:[bx - 0x1234]`: the signed displacement.
std::optional<std::int64_t> table_displacement(std::string_view op) {
    constexpr std::string_view kOpen = "cs:[bx ";
    for (std::size_t from = 0; (from = op.find(kOpen, from)) != std::string_view::npos; from += kOpen.size()) {
        std::string_view rest = op.substr(from + kOpen.size());
        if (rest.size() < 5 || (rest[0] != '+' && rest[0] != '-') || rest[1] != ' ' || rest[2] != '0' || rest[3] != 'x') continue;
        const bool plus = rest[0] == '+';
        std::size_t end = 4;
        while (end < rest.size() && is_lower_hex(rest[end])) ++end;
        if (end == 4 || end >= rest.size() || rest[end] != ']') continue;
        const auto v = txt::parse_int<std::int64_t>(rest.substr(4, end - 4), 16);
        if (!v) return std::nullopt;
        return plus ? *v : -*v;
    }
    return std::nullopt;
}

// `reg, N` (a word, a comma, a number): the number.
std::optional<std::int64_t> compare_constant(std::string_view op) {
    const std::size_t comma = op.find(", ");
    if (comma == std::string_view::npos || comma == 0 || !std::ranges::all_of(op.substr(0, comma), is_word)) return std::nullopt;
    return number_operand(op.substr(comma + 2));
}

bool has_prefix_word(std::string_view s, std::size_t at, std::string_view word) {
    return s.substr(at).starts_with(word) && (at == 0 || !is_word(s[at - 1]));
}

std::vector<std::string> without_register_variables(std::vector<std::string> stmts) {
    std::erase_if(stmts, [](const std::string& s) { return s.starts_with(kRegisterVariables); });
    return stmts;
}

// A register parameter's load (`reg = a;`, one of the first three statements) is not a statement; the parameter is
// declared instead.
void drop_register_loads(Recovery& r, const OrderedMap<std::string, int>& regparam, bool declare_parameter) {
    for (const auto& [reg, ix] : regparam) {
        const std::string p(1, static_cast<char>('a' + ix));
        std::vector<std::string> kept;
        for (std::size_t k = 0; k < r.stmts.size(); ++k)
            if (!(k < 3 && txt::trim(r.stmts[k]) == reg + " = " + p + ";")) kept.push_back(r.stmts[k]);
        r.stmts = std::move(kept);
        if (declare_parameter && std::ranges::find(r.params, p) == r.params.end()) {
            r.params.push_back(p);
            std::ranges::sort(r.params);
            r.params.erase(std::unique(r.params.begin(), r.params.end()), r.params.end());
        }
    }
}

// `if ((X <= ...)) {` and, on the next line, `if ((X >= ...`: a long compared the compiler's way (the high words,
// then the low words without sign) written as nested ifs on one variable.
bool has_compare_ladder(std::string_view t) {
    constexpr std::string_view kIf = "if ((";
    const auto compare_op = [](std::string_view s) -> bool {   // " <= " or " >= " (with < and > too)
        return s.size() >= 4 && s[0] == ' ' && (s[1] == '<' || s[1] == '>') && s[2] == '=' && s[3] == ' ';
    };
    for (std::size_t at = 0; (at = t.find(kIf, at)) != std::string_view::npos; at += kIf.size()) {
        std::size_t w = at + kIf.size(), end = w;
        while (end < t.size() && is_word(t[end])) ++end;
        if (end == w || !compare_op(t.substr(end))) continue;
        const std::string_view var = t.substr(w, end - w);
        const std::size_t close = t.find(')', end + 4);   // [^)]* up to the first ')'
        if (close == std::string_view::npos || t.substr(close).substr(0, 5) != ")) {\n") continue;
        std::size_t next = close + 5;
        while (next < t.size() && txt::is_space(t[next])) ++next;
        if (t.substr(next).starts_with(kIf) && t.substr(next + kIf.size()).starts_with(var) &&
            compare_op(t.substr(next + kIf.size() + var.size())))
            return true;
    }
    return false;
}

// `reg = p;` lines removed from the text (the model's reload of a register parameter at a join: no statement).
std::string without_register_reload(const std::string& t, const std::string& reg, const std::string& p) {
    const std::string line = reg + " = " + p + ";";
    std::vector<std::string_view> pieces = txt::split(t, '\n');
    std::vector<std::string_view> kept;
    for (std::size_t i = 0; i < pieces.size(); ++i) {
        const std::string_view l = pieces[i];
        const bool whole = i + 1 < pieces.size();   // (the last piece has no newline after it)
        const std::string_view body = l.substr(l.find_first_not_of(" \t") == std::string_view::npos ? l.size() : l.find_first_not_of(" \t"));
        if (whole && body == line) continue;
        kept.push_back(l);
    }
    return txt::join(kept, "\n");
}

std::string first_line(std::string_view what) {
    const std::size_t nl = what.find_first_of("\r\n");
    return std::string(what.substr(0, std::min({nl, what.size(), kVerdictDetail})));
}

// The model's record of what the 8087 and the longs did in this function, and where the function's value is a long.
void note_facts(InverseModel& inv, ByteView raw, ByteView code, ModelTexts& out) {
    const ModelLast last = inv.last();   // (what the model read of the 8087's values and of the longs)
    out.fp = FpInfo{last.fptypes, last.fpret, false};
    bool has_wait_esc = false;
    for (std::size_t i = 0; i + 1 < raw.size(); ++i)
        if (raw[i] == kWait && raw[i + 1] >= kEsc0 && raw[i + 1] <= 0xDF) has_wait_esc = true;
    out.fp.fp = !equal_bytes(raw, code) || has_wait_esc;
    out.longs = text::LongInfo();   // (the longs: shifted without sign; a value widened last)
    out.longs.unsigned_names = last.unsigned_longs;
    const std::vector<x86::Insn> ins = x86::disasm(code, 0, static_cast<std::uint32_t>(code.size()), false);
    if (ins.empty() || (ins.back().mnemonic != "ret" && ins.back().mnemonic != "retf")) return;
    std::ptrdiff_t j = static_cast<std::ptrdiff_t>(ins.size()) - 2;
    while (j >= 0 && (ins[static_cast<std::size_t>(j)].mnemonic == "pop" || ins[static_cast<std::size_t>(j)].op_str == "sp, bp")) --j;
    const bool has_epilogue = static_cast<std::size_t>(j + 1) < ins.size();
    const std::uint32_t epi = has_epilogue ? ins[static_cast<std::size_t>(j + 1)].address : 0;
    const auto gives_dx = [](const x86::Insn& x) {   // (any write of dx)
        return x.mnemonic == "cwd" || x.mnemonic == "cdq" ||
               (x.mnemonic != "cmp" && x.mnemonic != "test" && x.mnemonic != "push" && (x.op_str.starts_with("dx, ") || x.op_str == "dx"));
    };
    std::vector<std::ptrdiff_t> ends{j};   // the ways out: the end, and every jump to the epilogue
    if (has_epilogue)
        for (std::size_t k = 1; k < ins.size(); ++k)
            if (ins[k].mnemonic == "jmp" && (ins[k].op_str == std::format("0x{:x}", epi) || ins[k].op_str == std::to_string(epi)))
                ends.push_back(static_cast<std::ptrdiff_t>(k) - 1);
    const auto near = [&](std::ptrdiff_t e) {   // (dx given just before the way out)
        if (e < 0) return false;
        for (std::ptrdiff_t q = std::max<std::ptrdiff_t>(0, e - 2); q <= e; ++q)
            if (gives_dx(ins[static_cast<std::size_t>(q)])) return true;
        return false;
    };
    out.longs.ret = j >= 0 && std::ranges::all_of(ends, near);
}

}  // namespace

// ---- table_hook ------------------------------------------------------------------------------------------

TableHook::TableHook(ByteView code, std::uint32_t base)
    : code_(code.begin(), code.end()), base_(base), insns_(x86::disasm(code, 0, static_cast<std::uint32_t>(code.size()), false)) {}

std::optional<std::vector<std::int64_t>> TableHook::operator()(std::uint32_t address) const {
    const auto at = std::ranges::find_if(insns_, [&](const x86::Insn& i) { return i.address == address; });
    if (at == insns_.end()) return std::nullopt;
    const std::size_t k = static_cast<std::size_t>(at - insns_.begin());
    const std::optional<std::int64_t> displacement = table_displacement(at->op_str);
    std::optional<std::int64_t> n;   // the nearest `cmp R, N` before the jump: N + 1 words in the table
    for (std::size_t j = k; j-- > (k > kTableLookBack ? k - kTableLookBack : 0) && !n;)
        if (insns_[j].mnemonic == "cmp") n = compare_constant(insns_[j].op_str);
    if (!displacement || !n || *n < 0) return std::nullopt;
    // (above 32K: printed less)
    const std::int64_t t = (*displacement & 0xFFFF) - static_cast<std::int64_t>(base_);
    if (t < 0 || t + 2 * (*n + 1) > static_cast<std::int64_t>(code_.size())) return std::nullopt;
    std::vector<std::int64_t> places;
    for (std::int64_t j = 0; j <= *n; ++j) {
        const std::size_t p = static_cast<std::size_t>(t + 2 * j);
        places.push_back((code_[p] | (code_[p + 1] << 8)) - static_cast<std::int64_t>(base_));
    }
    return places;
}

// ---- fp_native, inline_goto_arms --------------------------------------------------------------------------

Bytes fp_native(ByteView code) {
    Bytes b(code.begin(), code.end());
    bool any = false;
    for (std::size_t i = 0; i + 1 < code.size(); ++i)
        if (code[i] == kInt && code[i + 1] >= kEmulatorFirst && code[i + 1] <= kEmulatorWait) any = true;
    if (!any) return b;
    for (const x86::Insn& i : x86::disasm(code, 0, static_cast<std::uint32_t>(code.size()), true)) {
        const Bytes& raw = i.bytes;
        if (raw.size() < 2 || raw[0] != kInt || raw[1] < kEmulatorFirst || raw[1] > kEmulatorWait) continue;
        const std::size_t a = i.address;
        if (raw[1] <= kEmulatorLastEsc) {
            b[a] = kWait;
            b[a + 1] = static_cast<std::uint8_t>(kEsc0 + raw[1] - kEmulatorFirst);
        } else if (raw[1] == kEmulatorWait) {
            b[a] = kWait;
            b[a + 1] = kNop;
        } else if (raw.size() >= 3) {   // an ESC with a segment override: the segment's prefix byte
            static constexpr std::uint8_t kPrefix[4] = {0x26, 0x2E, 0x36, 0x3E};   // es cs ss ds
            b[a] = kWait;
            b[a + 1] = kPrefix[raw[2] >> 6];
            b[a + 2] = static_cast<std::uint8_t>(kEsc0 | (raw[2] & 7));
        }
    }
    return b;
}

std::vector<std::string> inline_goto_arms(std::vector<std::string> lines) {
    const auto indent_of = [](const std::string& l) { return l.size() - txt::ltrim(l).size(); };
    for (std::size_t round = 0; round < kInlineGotoRounds; ++round) {
        struct Hit {
            std::size_t k, at;
            std::string indent, label;
            std::vector<std::string> body;
        };
        std::optional<Hit> hit;
        for (std::size_t k = 0; k < lines.size() && !hit; ++k) {
            const auto label = text::goto_target(lines[k], true);
            if (!label || k == 0 || !txt::rtrim(lines[k - 1]).ends_with('{') || k + 1 >= lines.size() || !txt::trim(lines[k + 1]).starts_with('}'))
                continue;
            const std::string target(*label), want = target + ":";
            const auto found = std::ranges::find_if(lines, [&](const std::string& x) { return txt::trim(x) == want; });
            if (found == lines.end()) continue;
            const std::size_t at = static_cast<std::size_t>(found - lines.begin()), ind = indent_of(lines[at]);
            std::size_t j = at + 1;
            std::ptrdiff_t depth = 0;
            while (j < lines.size()) {   // (to the end of the label's block)
                if (depth == 0 && txt::trim(lines[j]).starts_with('}')) break;
                depth += std::ranges::count(lines[j], '{') - std::ranges::count(lines[j], '}');
                ++j;
            }
            const std::vector<std::string> tail(lines.begin() + static_cast<std::ptrdiff_t>(at) + 1, lines.begin() + static_cast<std::ptrdiff_t>(j));
            if (tail.empty() || tail.size() > kInlineGotoMaxTail || std::ranges::any_of(tail, text::label_line) || (at < k && k < j)) continue;
            const std::string arm_indent = lines[k].substr(0, indent_of(lines[k]));
            Hit h{k, at, arm_indent, target, {}};
            for (const std::string& x : tail) h.body.push_back(txt::trim(x).empty() ? x : arm_indent + (ind < x.size() ? x.substr(ind) : std::string()));
            hit = std::move(h);
        }
        if (!hit) break;
        lines.erase(lines.begin() + static_cast<std::ptrdiff_t>(hit->k));
        lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(hit->k), hit->body.begin(), hit->body.end());
        const std::string jump = "goto " + hit->label + ";";   // (the label goes when nothing goes to it now)
        const bool wanted = std::ranges::any_of(lines, [&](const std::string& x) {
            for (std::size_t p = 0; (p = x.find(jump, p)) != std::string::npos; ++p)
                if (has_prefix_word(x, p, "goto")) return true;
            return false;
        });
        if (!wanted) std::erase_if(lines, [&](const std::string& x) { return txt::trim(x) == hit->label + ":"; });
    }
    return lines;
}

// ---- model_texts --------------------------------------------------------------------------------------------

ModelTexts model_texts(InverseModel& inv, ByteView raw, std::string_view name, std::optional<std::uint32_t> base) {
    const ModelCapabilities cap = inv.capabilities();
    const Bytes code = fp_native(raw);
    const Reading reading{cap.frame_after_bp, cap.regvars_eager};   // (a draft is of 3.00 or later code: its -Gs frames, its register variables)
    ModelTexts out;

    Recovery linear;
    try {
        linear = inv.recover(code, reading);
    } catch (const std::exception&) {   // (the model declines the code with the 8087's calls rewritten: as it was)
        if (equal_bytes(raw, code)) throw;
        linear = inv.recover(raw, reading);
    }
    out.frame_size = linear.frame_size;   // (the frame's size: fill_frame)
    const OrderedMap<std::string, int> regparam = inv.last().regparam;
    out.texts.push_back({inv.to_c(name, linear), regparam});

    if (cap.structured) {
        Structuring first_how;
        first_how.compare_switches = true;   // (switches by compares read as switches)
        if (base && cap.table_switch) {      // (and by a table: where it sends each value)
            auto hook = std::make_shared<TableHook>(code, *base);
            first_how.table_switch = [hook](std::uint32_t address) { return (*hook)(address); };
        }
        Recovery r;
        try {
            r = inv.recover_structured(code, reading, first_how);
        } catch (const std::exception&) {   // (the model has no structured reading of this code: the linear text stands)
            note_facts(inv, raw, code, out);
            return out;
        }
        r.stmts = without_register_variables(std::move(r.stmts));
        drop_register_loads(r, regparam, true);
        const std::string t = inv.to_c(name, r);
        if (t != out.texts[0].text) out.texts.push_back({t, regparam});

        const ModelLast after = inv.last();
        const bool fp_fields = std::ranges::any_of(after.fptypes, [](const auto& kv) {
            return kv.first.find("->") != std::string::npos || kv.first.find('[') != std::string::npos;
        });
        if (cap.fp_casts && fp_fields) {   // (the 8087's fields at their own widths: read again with casts - a union's place used both ways)
            Structuring how;
            how.compare_switches = true;
            how.fp_casts = true;
            try {
                Recovery r2 = inv.recover_structured(code, reading, how);
                r2.stmts = without_register_variables(std::move(r2.stmts));
                drop_register_loads(r2, regparam, true);
                const std::string t2 = inv.to_c(name, r2);
                if (std::ranges::all_of(out.texts, [&](const DraftText& x) { return t2 != x.text; })) out.texts.push_back({t2, regparam});
            } catch (const std::exception&) {   // (the model cannot read it that way: the other texts stand)
            }
        }

        // A long compared the compiler's way - the high words, then the low words without sign - three nested ifs:
        // read again with the unsigned compares marked, and the ladders folded into one long comparison.
        const std::vector<x86::Insn> ins = x86::disasm(code, 0, static_cast<std::uint32_t>(code.size()), false);
        bool unsigned_compare = false;   // (or a compare without sign: the text with it marked)
        for (std::size_t i = 0; i + 1 < ins.size(); ++i) {
            const std::string_view jump = ins[i + 1].mnemonic;
            if (ins[i].mnemonic != "cmp" || (jump != "jb" && jump != "jbe" && jump != "ja" && jump != "jae")) continue;
            const std::size_t comma = ins[i].op_str.find(',');
            if (comma != std::string::npos && ins[i].op_str.find("bp", comma) == std::string::npos) unsigned_compare = true;
        }
        if (cap.fold_ladders && (unsigned_compare || has_compare_ladder(t))) {
            Structuring how;
            how.compare_switches = true;
            how.unsigned_compares = true;
            try {
                Recovery r2 = inv.recover_structured(code, reading, how);
                r2.stmts = without_register_variables(inv.fold_ladders(inline_goto_arms(std::move(r2.stmts))));
                drop_register_loads(r2, regparam, false);
                r2.params = r.params;   // (the parameters of the first structured text)
                const std::string t2 = inv.to_c(name, r2);
                if (t2 != t) out.texts.push_back({t2, regparam});
            } catch (const std::exception&) {   // (the model cannot read it that way: the other texts stand)
            }
        }
    }
    note_facts(inv, raw, code, out);
    for (const auto& [reg, ix] : regparam) {   // (a register parameter's register given the parameter: itself - the model's reload at a join, no statement)
        const std::string p(1, static_cast<char>('a' + ix));
        for (DraftText& d : out.texts) d.text = without_register_reload(d.text, reg, p);
    }
    return out;
}

// ---- the stage ---------------------------------------------------------------------------------------------

std::map<std::uint32_t, std::string> code_names(const Project& project) {
    const auto bare = [](std::string_view n) { return std::string(n.substr(std::min(n.find_first_not_of('_'), n.size()))); };
    std::map<std::uint32_t, std::string> out;
    for (const LibraryPlace& place : project.library_places()) {
        std::vector<const LibraryModule*> modules{&place.module};
        for (const LibraryModule& m : place.also) modules.push_back(&m);
        for (const LibraryModule* m : modules)
            for (const auto& [name, where] : m->publics)
                if (where.first == m->first_code_segment) out.try_emplace(place.code + where.second, bare(name));
    }
    for (const auto& [name, where] : project.symbols())
        if (where.kind == SymKind::Code && name.find(':') == std::string::npos) out.try_emplace(static_cast<std::uint32_t>(where.address), bare(name));
    return out;
}

Verdicts run(Project& project, const Options& options) {
    const std::optional<std::string> model_name = project.inverse_model_name();
    if (!model_name)
        throw UsageError(std::format("the toolchain {} names no inverse model (its profile's \"inverse\")", project.toolchain_name()));
    const std::shared_ptr<InverseModel> inv = project.load_inverse_model(*model_name);
    const std::map<std::uint32_t, std::string> names = code_names(project);
    const ByteView img = project.image();
    const std::set<std::uint32_t> have = project.source_addresses();
    Verdicts out;
    fs::make_dirs(project.path("drafts"));
    std::vector<std::pair<std::uint32_t, std::uint32_t>> todo;
    for (const auto& [a, ln] : project.functions())
        if (options.every || !have.contains(a)) todo.emplace_back(a, ln);
    if (options.limit && todo.size() > *options.limit) todo.resize(*options.limit);

    for (const auto& [a, ln] : todo) {
        if (have.contains(a)) continue;
        const std::size_t from = std::min<std::size_t>(a, img.size());
        const ByteView code = img.subspan(from, std::min<std::size_t>(ln, img.size() - from));
        ModelTexts texts;
        // An isolation boundary: one function of the run. The model may decline it; a pass may fail on it. Either
        // is that function's verdict and the run goes on.
        try {
            texts = model_texts(*inv, code, std::format("f{:05x}", a), a);
        } catch (const std::exception& e) {
            const std::string what = first_line(e.what());
            out.insert_or_assign(a, what.empty() ? "declined" : "declined: " + what);
            continue;
        }
        ProveOutcome proved;
        try {
            proved = project.prove_texts(*inv, texts, a, ln, names);
        } catch (const std::exception& e) {
            const auto* known = dynamic_cast<const Error*>(&e);
            const std::string what(e.what());
            out.insert_or_assign(a, std::format("failed: {}: {}", known != nullptr ? std::string(known->kind()) : "exception", what.substr(0, kVerdictDetail)));
            continue;
        }
        out.insert_or_assign(a, proved.verdict);
        if (proved.verdict == "identical")
            fs::write_text(project.path(std::format("f{:05x}.c", a)), std::format("/* drafted by codebreaker exact draft ({}), identical */\n{}", *model_name, proved.source));
        else
            fs::write_text(project.path(std::format("drafts/f{:05x}.c", a)),
                           std::format("/* draft ({}): {} */\n{}", *model_name, proved.verdict, proved.source));
        if (options.verbose) cb::out("{:05x} {:5d}  {}\n", a, ln, proved.verdict);
    }
    return out;
}

int run_command(Project& project, const Options& options) {
    const Verdicts verdicts = run(project, options);
    std::vector<std::pair<std::string, std::size_t>> kinds;   // (first-seen order; the summary sorts by count)
    for (const auto& [a, v] : verdicts) {
        const std::string kind = v.substr(0, v.find(':'));
        const auto it = std::ranges::find_if(kinds, [&](const auto& k) { return k.first == kind; });
        if (it == kinds.end()) kinds.emplace_back(kind, 1);
        else ++it->second;
    }
    std::ranges::stable_sort(kinds, [](const auto& x, const auto& y) { return x.second > y.second; });
    std::vector<std::string> parts;
    for (const auto& [kind, n] : kinds) parts.push_back(std::format("{} {}", n, kind));
    cb::out("{} functions drafted: {}\n", verdicts.size(), txt::join(parts, ", "));
    return kExitOk;
}

}  // namespace cb::exact::draft
