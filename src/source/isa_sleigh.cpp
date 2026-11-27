// cb-unity: alone (file-local names that other files in a batch also use)
// source/isa_sleigh.cpp -- see isa_sleigh.hpp.

#include "source/isa_sleigh.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <cstdlib>
#include <condition_variable>
#include <format>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <unordered_map>

#include "core/parallel.hpp"
#include "core/text.hpp"
#include "decompile/engine/sleigh_assembler.hh"
#include "decompile/ppc.hpp"

namespace cb::source::isa {
namespace {

namespace dp = decompile::ppc;
namespace dz = decompile::z80;

bool word_start(char c) { return std::isalpha(static_cast<unsigned char>(c)) != 0 || c == '_' || c == '.' || c == '@'; }
bool word_char(char c) { return std::isalnum(static_cast<unsigned char>(c)) != 0 || c == '_' || c == '.' || c == '@'; }

// `%hi(x)`, `%lo(x)` and `%up(x)` (a MIPS address's halves: the high half adjusted for the low half's sign, the low half, the plain high half) as the
// numbers they are. `signed_lo`: the low half as a signed 16-bit number (an addiu or a load's), else as an unsigned one (an ori's).
std::string fold_halves(const std::string& text, std::int64_t pc, bool signed_lo) {
    std::string out;
    for (std::size_t k = 0; k < text.size();) {
        const std::string_view rest = std::string_view(text).substr(k);
        const bool hi = rest.starts_with("%hi("), lo = rest.starts_with("%lo("), up = rest.starts_with("%up(");
        if (!hi && !lo && !up) {
            out += text[k++];
            continue;
        }
        std::size_t depth = 0, e = k + 3;
        for (; e < text.size(); ++e) {
            if (text[e] == '(') ++depth;
            if (text[e] == ')' && --depth == 0) break;
        }
        if (e >= text.size()) return text;
        std::int64_t v = 0;
        try {
            v = dp::value(std::string_view(text).substr(k + 4, e - k - 4), {}, static_cast<std::uint32_t>(pc));
        } catch (const std::exception&) {
            return text;
        }
        if (hi) out += std::format("0x{:x}", ((v + 0x8000) >> 16) & 0xFFFF);
        else if (up) out += std::format("0x{:x}", (v >> 16) & 0xFFFF);
        else if (signed_lo && (v & 0x8000)) out += std::format("-0x{:x}", 0x10000 - (v & 0xFFFF));
        else out += std::format("0x{:x}", v & 0xFFFF);
        k = e + 1;
    }
    return out;
}

// `0x8000@ha`, `0x8000@h`, `0x8000@l` (a PowerPC address's halves, as `lis` and `addi` carry them) as the numbers they are; `signed_lo` as in fold_halves.
// `signed_hi`: the high halves (`@ha`, `@h`) as signed 16-bit numbers too (what `lis` takes).
std::string fold_relocations(const std::string& text, bool signed_lo, bool signed_hi) {
    std::string out;
    bool negative_lo = false;
    for (std::size_t k = 0; k < text.size();) {
        if (text[k] == '0' && k + 2 < text.size() && text[k + 1] == 'x' && (k == 0 || !word_char(text[k - 1]))) {
            std::size_t e = k + 2;
            while (e < text.size() && std::isxdigit(static_cast<unsigned char>(text[e]))) ++e;
            const std::string_view rest = std::string_view(text).substr(e);
            const bool ha = rest.starts_with("@ha"), hi = !ha && rest.starts_with("@h"), lo = rest.starts_with("@l");
            if (e > k + 2 && (ha || hi || lo)) {
                const std::optional<std::uint64_t> n = text::parse_int<std::uint64_t>(std::string_view(text).substr(k + 2, e - k - 2), 16);
                if (n) {
                    const std::int64_t v = static_cast<std::int64_t>(*n);
                    if (ha || hi) {
                        const std::int64_t half = (ha ? (v + 0x8000) >> 16 : v >> 16) & 0xFFFF;
                        if (signed_hi && (half & 0x8000)) out += std::format("-0x{:x}", 0x10000 - half);
                        else out += std::format("0x{:x}", half);
                    }
                    else if (signed_lo && (v & 0x8000)) out += std::format("-0x{:x}", 0x10000 - (v & 0xFFFF)), negative_lo = true;
                    else out += std::format("0x{:x}", v & 0xFFFF);
                    k = e + (ha ? 3 : hi ? 2 : 2);
                    continue;
                }
            }
        }
        out += text[k++];
    }
    // (the description has no `addi` of a negative number: it writes `subi` of the number's magnitude, so a negative half goes the same way)
    if (negative_lo)
        for (const auto& [add, sub] : {std::pair<std::string_view, std::string_view>{"addi ", "subi "}, {"addis ", "subis "}, {"addic ", "subic "}, {"addic. ", "subic. "}})
            if (out.starts_with(add)) {
                const std::size_t minus = out.rfind(",-0x");
                if (minus == std::string::npos) break;
                out = std::string(sub) + out.substr(add.size(), minus + 1 - add.size()) + out.substr(minus + 2);
                break;
            }
    return out;
}

// The numbers an expression names, written the way the description reads one: `0x1f` or `-0x20`.
std::string number_text(std::int64_t v) { return v < 0 ? std::format("-0x{:x}", static_cast<std::uint64_t>(-v)) : std::format("0x{:x}", static_cast<std::uint64_t>(v)); }

// The function-like halves of an address an assembly source writes in an operand: `hi(x)` (the high half, adjusted for the low half's sign, as a
// V810's `movhi` takes it), `lo(x)` (the low half as a signed 16-bit number), and ca65's `.loword(x)`, `.hiword(x)`, `.bankbyte(x)`, `.lobyte(x)`,
// `.hibyte(x)`. Names are matched without regard to case.
bool address_function(std::string_view word) {
    for (const std::string_view name : {"hi", "lo", ".loword", ".hiword", ".bankbyte", ".lobyte", ".hibyte"})
        if (text::iequals(word, name)) return true;
    return false;
}

std::optional<std::int64_t> address_function_value(std::string_view name, std::int64_t v) {
    if (text::iequals(name, "hi")) return ((v + 0x8000) >> 16) & 0xFFFF;
    if (text::iequals(name, "lo")) return static_cast<std::int16_t>(v & 0xFFFF);
    if (text::iequals(name, ".loword")) return v & 0xFFFF;
    if (text::iequals(name, ".hiword")) return (v >> 16) & 0xFFFF;
    if (text::iequals(name, ".bankbyte")) return (v >> 16) & 0xFF;
    if (text::iequals(name, ".lobyte")) return v & 0xFF;
    if (text::iequals(name, ".hibyte")) return (v >> 8) & 0xFF;
    return std::nullopt;
}

// `text` (labels already numbers) with each address function and each constant expression of its operands -- `(0x5f800+0x8000)>>16`, `0x123456 & 0xffff` --
// written as the number it is. The text is returned as it is when it has none (or one it cannot evaluate).
std::string fold_operands(const std::string& text, std::int64_t pc) {
    const std::size_t mnemonic_end = std::min(text.find_first_of(" \t"), text.size());
    std::string out = text.substr(0, mnemonic_end);
    const auto evaluate = [&](std::string_view expr) -> std::optional<std::int64_t> {
        try {
            return dp::value(expr, {}, static_cast<std::uint32_t>(pc));
        } catch (const std::exception&) {
            return std::nullopt;
        }
    };
    for (std::size_t k = mnemonic_end; k < text.size();) {
        // a function: `name(expression)`
        if (word_start(text[k]) && (k == 0 || !word_char(text[k - 1]))) {
            std::size_t e = k;
            while (e < text.size() && word_char(text[e])) ++e;
            if (e < text.size() && text[e] == '(' && address_function(std::string_view(text).substr(k, e - k))) {
                std::size_t depth = 0, close = e;
                for (; close < text.size(); ++close) {
                    if (text[close] == '(') ++depth;
                    if (text[close] == ')' && --depth == 0) break;
                }
                if (close < text.size()) {
                    const std::string folded_inner = fold_operands(" " + text.substr(e + 1, close - e - 1), pc).substr(1);
                    if (const auto inner = evaluate(folded_inner))
                        if (const auto v = address_function_value(std::string_view(text).substr(k, e - k), *inner)) {
                            out += number_text(*v);
                            k = close + 1;
                            continue;
                        }
                }
            }
            out.append(text, k, e - k);
            k = e;
            continue;
        }
        // a run of number and operator characters with an operator in it
        const auto in_run = [](char c) { return std::isxdigit(static_cast<unsigned char>(c)) != 0 || std::string_view("xX+-*/&|^<>()~").find(c) != std::string_view::npos; };
        if (in_run(text[k]) && (k == 0 || !word_char(text[k - 1]))) {
            std::size_t e = k;
            while (e < text.size() && in_run(text[e])) ++e;
            while (e > k && !word_char(text[e - 1]) && text[e - 1] != ')') --e;   // (the run ends at a number or a close)
            const std::string_view run = std::string_view(text).substr(k, e - k);
            bool operator_inside = false;
            for (std::size_t i = 1; i < run.size(); ++i)
                if (std::string_view("+*/&|^<>-").find(run[i]) != std::string_view::npos && run[i - 1] != '(') operator_inside = true;
            const bool starts_a_word = e < text.size() && word_start(text[e]);   // (`1+2r`: not a number)
            if (operator_inside && !starts_a_word && (e == k || run.front() == '(' || std::isdigit(static_cast<unsigned char>(run.front())) != 0 || run.front() == '-' || run.front() == '~')) {
                if (const auto v = evaluate(run)) {
                    out += number_text(*v);
                    k = e;
                    continue;
                }
            }
        }
        out += text[k++];
    }
    return out;
}

// `text` with each hex number of 0x8000 to 0xFFFF written as the 16-bit signed number it stands for (`lis r4,0x8200` is `lis r4,-0x7e00`: the
// description's field is signed, and an assembly source writes the half as the unsigned number it is).
std::string signed_halves(const std::string& text) {
    std::string out;
    for (std::size_t k = 0; k < text.size();) {
        if (text[k] == '0' && k + 2 < text.size() && text[k + 1] == 'x' && k > 0 && text[k - 1] == ',') {   // (an operand after a comma: the first operand of `beq 0x9000` is an address, not a signed half)
            std::size_t e = k + 2;
            while (e < text.size() && std::isxdigit(static_cast<unsigned char>(text[e]))) ++e;
            const std::optional<std::uint64_t> n = text::parse_int<std::uint64_t>(std::string_view(text).substr(k + 2, e - k - 2), 16);
            if (n && *n >= 0x8000 && *n <= 0xFFFF && !(e < text.size() && word_char(text[e]))) {
                out += std::format("-0x{:x}", 0x10000 - *n);
                k = e;
                continue;
            }
        }
        out += text[k++];
    }
    return out;
}

// `text` with the blanks around an operator of a constant expression taken out (`#IO >> 24` is `#IO>>24`: the folding below reads a run of number and operator characters).
std::string compact_operators(const std::string& text) {
    std::string out;
    for (std::size_t k = 0; k < text.size(); ++k) {
        const char c = text[k];
        if (c == ' ' || c == '\t') {
            std::size_t e = k;
            while (e < text.size() && (text[e] == ' ' || text[e] == '\t')) ++e;
            const char before = out.empty() ? '\0' : out.back();
            const char after = e < text.size() ? text[e] : '\0';
            const auto op = [](char x) { return x != '\0' && std::string_view("<>+*/&|^").find(x) != std::string_view::npos; };
            const auto joins = [](char x) { return std::isalnum(static_cast<unsigned char>(x)) != 0 || x == ')' || x == '('; };
            if (op(before) || op(after) || (after == '-' && joins(before))) {
                k = e - 1;
                continue;
            }
        }
        out += c;
    }
    return out;
}

// `mfc0 s0,12` with its coprocessor register number written as the description's name for it (`mfc0 s0,Status`); the line as it was for any other.
std::string mips_register_names(std::string_view line) {
    static const std::array<std::string_view, 32> cop0 = {"Index", "Random", "EntryLo0", "EntryLo1", "Context", "PageMask", "Wired", "HWREna", "BadVAddr", "Count", "EntryHi", "Compare", "Status", "Cause", "EPC", "PRId",
                                                          "Config", "LLAddr", "WatchLo", "WatchHi", "XContext", "cop0_reg21", "cop0_reg22", "Debug", "DEPC", "PerfCnt", "ErrCtl", "CacheErr", "TagLo", "TagHi", "ErrorEPC", "DESAVE"};
    const std::string_view t = text::trim(line);
    const std::size_t blank = t.find_first_of(" \t");
    if (blank == std::string_view::npos) return std::string(line);
    const std::string_view head = t.substr(0, blank);
    const bool control = text::iequals(head, "cfc1") || text::iequals(head, "ctc1");
    const bool cop0_move = text::iequals(head, "mfc0") || text::iequals(head, "mtc0") || text::iequals(head, "dmfc0") || text::iequals(head, "dmtc0");
    if (!control && !cop0_move) return std::string(line);
    const std::size_t comma = t.rfind(',');
    if (comma == std::string_view::npos) return std::string(line);
    std::string_view number = text::trim(t.substr(comma + 1));
    if (!number.empty() && number.front() == '$') number.remove_prefix(1);
    const std::optional<std::int64_t> n = build::parse_number(number);
    if (!n || *n < 0 || *n > 31) return std::string(line);
    std::string_view name;
    if (cop0_move) name = cop0[static_cast<std::size_t>(*n)];
    else if (*n == 0) name = "fir";
    else if (*n == 25) name = "fccr";
    else if (*n == 26) name = "fexr";
    else if (*n == 28) name = "fenr";
    else if (*n == 31) name = "fcsr";
    else return std::string(line);
    return std::string(t.substr(0, comma + 1)) + std::string(name);
}

class SleighSyntax final : public build::Encoder {
public:
    SleighSyntax(const std::filesystem::path& sla, const std::vector<std::pair<std::string, int>>& context)
        : sla_(sla), context_(context), pool_limit_(static_cast<std::size_t>(std::max(1, parallel::workers()))) {
        // (the first assembler is made now: a description that cannot be read fails here, where the encoder is asked for)
        {
            std::lock_guard one(loading_lock());
            all_.push_back(std::make_unique<decompile::sleigh::Assembler>(sla, context));
        }
        free_.push_back(all_.back().get());
        made_ = 1;
    }

    // `.context m16=1 x16=0`: the description's variables set over the ones this encoder starts from; the encoder of that context answers.
    Result<Bytes> assemble_in(std::string_view context, std::string_view line, std::int64_t pc, const build::Labels& labels) const override {
        std::vector<std::pair<std::string, int>> merged = context_;
        std::vector<std::string_view> items;
        for (std::size_t k = 0; k < context.size();) {
            while (k < context.size() && (context[k] == ' ' || context[k] == '\t' || context[k] == ',')) ++k;
            const std::size_t start = k;
            while (k < context.size() && context[k] != ' ' && context[k] != '\t' && context[k] != ',') ++k;
            if (k > start) items.push_back(context.substr(start, k - start));
        }
        for (const std::string_view item : items) {
            const std::size_t eq = item.find('=');
            const std::optional<int> v = eq == std::string_view::npos ? std::nullopt : text::parse_int<int>(item.substr(eq + 1), 10);
            if (!v) return Failure(build::AsmError(0, line, "`.context` takes name=value"));
            const std::string name(item.substr(0, eq));
            const auto it = std::ranges::find(merged, name, &std::pair<std::string, int>::first);
            if (it != merged.end()) it->second = *v;
            else merged.emplace_back(name, *v);
        }
        const build::Encoder& other = sleigh_encoder(sla_, merged);
        return &other == this ? assemble(line, pc, labels) : other.assemble(line, pc, labels);
    }

    Result<std::int64_t> value(std::string_view expr, const build::Labels& labels, std::int64_t pc) const override {
        try {
            return dp::value(expr, labels, static_cast<std::uint32_t>(pc));
        } catch (const dz::UnknownLabel& e) {
            return Failure(build::UndefinedSymbolError(e.name()));
        } catch (const ParseError& e) {
            return Failure(e.clone());
        }
    }

    // The line with each label it names replaced by the label's value (the description knows numbers and its own names, not the
    // program's labels); the instruction's shortest encoding. A line that encodes to nothing while it names a word that is no label
    // is taken to name a label not defined yet (the first pass meets labels defined further on).
    // ARM's unified syntax says some things the description writes otherwise by the state the code is in: `nop`, `push` and `pop`, and
    // a Thumb instruction's `s` (every Thumb data operation of the 16-bit set sets the flags; the description's own name for it has none).
    Result<Bytes> assemble(std::string_view line, std::int64_t pc, const build::Labels& labels) const override {
        Result<Bytes> first = assemble_arm(line, pc, labels);
        if (first.ok() || build::is_undefined(first.error())) return first;
        // MIPS names a coprocessor register by number (`mfc0 $s0, $12`, `ctc1 $t1, $31`); the description names it (Status, fcsr)
        if (const std::string named = mips_register_names(line); named != line) {
            Result<Bytes> r = assemble_arm(named, pc, labels);
            if (r.ok() || build::is_undefined(r.error())) return r;
        }
        // MIPS's `la rt, address`: the two instructions that load an address (the description has no pseudo-instructions)
        const std::string_view t = text::trim(line);
        if (text::istarts_with(t, "la ") || text::istarts_with(t, "la\t")) {
            const std::vector<std::string_view> parts = build::split_items(text::trim(t.substr(3)), true);
            if (parts.size() == 2) {
                const Result<std::int64_t> v = value(parts[1], labels, pc);
                if (!v.ok()) return Failure(v.error().clone());
                const std::uint32_t address = static_cast<std::uint32_t>(v.value());
                const std::uint32_t high = (address + 0x8000) >> 16;
                const std::int32_t low = static_cast<std::int16_t>(address & 0xFFFF);
                const std::string reg(parts[0]);
                Result<Bytes> lui = assemble_arm(std::format("lui {},0x{:x}", reg, high & 0xFFFF), pc, labels);
                if (!lui.ok()) return first;
                Result<Bytes> add = assemble_arm(std::format("addiu {},{},{}{:#x}", reg, reg, low < 0 ? "-" : "", static_cast<std::uint32_t>(low < 0 ? -low : low)), pc + 4, labels);
                if (!add.ok()) return first;
                Bytes both = lui.value();
                both.insert(both.end(), add.value().begin(), add.value().end());
                return both;
            }
        }
        return first;
    }

    Result<Bytes> assemble_arm(std::string_view line, std::int64_t pc, const build::Labels& labels) const {
        int thumb = -1;
        for (const auto& [name, value] : context_)
            if (name == "TMode") thumb = value;
        if (thumb < 0) return assemble_core(line, pc, labels);
        const std::string_view t = text::trim(line);
        if (thumb == 1 && text::istarts_with(t, "swi ")) return assemble_core("svc " + std::string(text::trim(t.substr(4))), pc, labels);   // (Thumb's description calls it svc)
        if (text::iequals(t, "nop")) return assemble_core(thumb != 0 ? "mov r8,r8" : "mov r0,r0", pc, labels);
        if (thumb == 0) {   // (`push {r4,lr}`, `popne {pc}`: the store and load of the stack, under a condition)
            const std::size_t blank = t.find_first_of(" \t");
            const std::string_view head = blank == std::string_view::npos ? t : t.substr(0, blank);
            for (const auto& [name, real] : {std::pair<std::string_view, std::string_view>{"push", "stmdb"}, {"pop", "ldmia"}})
                if (blank != std::string_view::npos && head.size() >= name.size() && text::istarts_with(head, name) && (head.size() == name.size() || head.size() == name.size() + 2))
                    return assemble_core(std::string(real) + std::string(head.substr(name.size())) + " sp!," + std::string(text::trim(t.substr(blank))), pc, labels);
        }
        Result<Bytes> r = assemble_core(line, pc, labels);
        if (r.ok() || build::is_undefined(r.error())) return r;
        if (thumb == 0) {   // `add r1, pc, #1`: the description's `adr` of the address the sum is (the pc of an ARM instruction reads eight ahead)
            std::string flat(t);
            std::erase_if(flat, [](char c) { return c == ' ' || c == '\t'; });
            const bool add = text::istarts_with(flat, "add"), sub = text::istarts_with(flat, "sub");
            const std::size_t comma = flat.find(',');
            if ((add || sub) && flat.size() > 3 && comma != std::string::npos && text::istarts_with(std::string_view(flat).substr(comma + 1), "pc,#")) {
                const std::optional<std::int64_t> imm = build::parse_number(std::string_view(flat).substr(comma + 5));
                if (imm) return assemble_core(std::format("adr {},0x{:x}", flat.substr(3, comma - 3), static_cast<std::uint64_t>(pc + 8 + (add ? *imm : -*imm))), pc, labels);
            }
            return r;
        }
        const std::size_t end = std::min(t.find_first_of(" \t"), t.size());
        if (end > 2 && (t[end - 1] == 's' || t[end - 1] == 'S')) {
            Result<Bytes> plain = assemble_core(std::string(t.substr(0, end - 1)) + std::string(t.substr(end)), pc, labels);
            if (plain.ok() || build::is_undefined(plain.error())) return plain;
        }
        return r;
    }

    Result<Bytes> assemble_core(std::string_view line, std::int64_t pc, const build::Labels& labels) const {
        const std::string_view text = text::trim(line);
        const std::size_t mnemonic_end = std::min(text.find_first_of(" \t"), text.size());
        std::string out(text.substr(0, mnemonic_end));
        struct Unknown {
            std::string word;
            std::size_t at;   // where it is in `out`
        };
        std::vector<Unknown> unknown;   // the words that are no label: the description's own names (registers, conditions) or labels not defined yet
        for (std::size_t k = mnemonic_end; k < text.size();) {
            if (word_start(text[k]) && (k == 0 || !word_char(text[k - 1]))) {
                std::size_t e = k;
                while (e < text.size() && word_char(text[e])) ++e;
                std::string word(text.substr(k, e - k));
                std::string suffix;   // (`jmp name.l`: the size suffix is not part of the name)
                if (!labels.contains(word)) {
                    // (a name's half of an address: `@ha`, `@h`, `@l`)
                    for (const std::string_view half : {"@ha", "@h", "@l"}) {
                        if (word.size() > half.size() && word.ends_with(half)) {
                            suffix = std::string(half);
                            word.resize(word.size() - half.size());
                            break;
                        }
                    }
                }
                if (suffix.empty() && !labels.contains(word)) {
                    // (a name's own size: `.b`, `.w`, `.l`, `.s`, which is the instruction's, defined or not)
                    if (const std::size_t dot = word.rfind('.'); dot != std::string::npos && dot > 0 && word.size() - dot == 2 && std::string_view("bwls").find(word[dot + 1]) != std::string_view::npos) {
                        suffix = word.substr(dot);
                        word.resize(dot);
                    }
                }
                if (e < text.size() && text[e] == '(' && address_function(word) && !labels.contains(word)) out += word;   // (`hi(x)`: a function of the operand, not a name)
                else if (const auto hit = labels.find(word); hit != labels.end()) out += std::format("0x{:x}", static_cast<std::uint64_t>(hit->second)) + suffix;
                else {
                    unknown.push_back({word, out.size()});
                    out += word + suffix;
                }
                k = e;
            } else {
                out += text[k++];
            }
        }
        // (a program's lines are assembled again and again: a writer checks each instruction, then the build makes two passes; the engine takes
        // milliseconds for a line, so the lines it has encoded are kept, by address and text)
        const std::string memo_key = std::format("{:x}|{}", static_cast<std::uint64_t>(pc), out);
        {
            std::lock_guard hold(memo_lock_);
            if (const auto hit = memo_.find(memo_key); hit != memo_.end()) return hit->second;
            // (a line that is no instruction, or names a label not defined yet, fails the same way every time it is asked at an address: a
            // writer's two passes and its guesses ask again and again, and each failure costs the guessing below)
            if (const auto bad = failed_.find(memo_key); bad != failed_.end()) {
                if (bad->second.empty()) return Failure(build::AsmError(0, text, "not an instruction of the processor's description"));
                return Failure(build::UndefinedSymbolError(bad->second));
            }
        }
        const auto remember_failure = [&](const std::string& undefined_word) {
            std::lock_guard hold(memo_lock_);
            if (failed_.size() >= kMemoLimit) failed_.clear();
            failed_[memo_key] = undefined_word;
        };
        const auto remember = [&](const std::vector<std::uint8_t>& bytes) -> Bytes {
            std::lock_guard hold(memo_lock_);
            if (memo_.size() >= kMemoLimit) memo_.clear();
            return memo_.emplace(memo_key, Bytes(bytes.begin(), bytes.end())).first->second;
        };
        // (the engine's assembler keeps state between lines: this call has one to itself, and other threads' calls have theirs)
        const Lease lease(*this);
        decompile::sleigh::Assembler& assembler_ = lease.assembler();
        if (out.find("%lo(") != std::string::npos || out.find("%hi(") != std::string::npos || out.find("%up(") != std::string::npos) {
            for (const bool signed_lo : {true, false}) {   // (an ori's low half is unsigned, an addiu's signed: the one the instruction takes)
                const std::vector<std::vector<std::uint8_t>> folded = assembler_.assemble_first(fold_halves(out, pc, signed_lo), static_cast<std::uint64_t>(pc));
                if (!folded.empty()) return remember(folded.front());
            }
        }
        if (out.find('@') != std::string::npos) {
            for (const bool signed_lo : {true, false})
                for (const bool signed_hi : {false, true}) {   // (`lis` takes its half as a signed number: 0x8000 is -0x8000)
                    const std::vector<std::vector<std::uint8_t>> folded = assembler_.assemble_first(fold_relocations(out, signed_lo, signed_hi), static_cast<std::uint64_t>(pc));
                    if (!folded.empty()) return remember(folded.front());
                }
        }
        const std::vector<std::vector<std::uint8_t>> encodings = assembler_.assemble_first(out, static_cast<std::uint64_t>(pc));
        if (!encodings.empty()) return remember(encodings.front());
        // (an operand written as an expression or a function of an address: assembled as the number it is)
        if (const std::string folded = fold_operands(compact_operators(out), pc); folded != out) {
            const std::vector<std::vector<std::uint8_t>> enc = assembler_.assemble_first(folded, static_cast<std::uint64_t>(pc));
            if (!enc.empty()) return remember(enc.front());
        }
        if (const std::string wrapped = signed_halves(out); wrapped != out) {
            const std::vector<std::vector<std::uint8_t>> enc = assembler_.assemble_first(wrapped, static_cast<std::uint64_t>(pc));
            if (!enc.empty()) return remember(enc.front());
        }
        // Which unknown word is the label not defined yet? The one that, as a number, makes the line an instruction (in `JR NZ,ahead`
        // NZ is the description's and `ahead` the label): the directive layer then guesses its value and assembles again.
        for (const Unknown& u : unknown) {
            // (the places a label may be taken to be: here, just ahead, and a word-aligned place ahead, which a PC-relative load of Thumb reaches)
            // (the place the directive layer takes first is tried first: when this line then assembles with it, the line the layer asks next is
            // this one, and its answer is kept below)
            for (const std::int64_t at : {(pc & ~std::int64_t{3}) + 8, pc, pc + 4}) {
                std::string guess = out;
                guess.replace(u.at, u.word.size(), std::format("0x{:x}", static_cast<std::uint64_t>(at)));
                const std::vector<std::vector<std::uint8_t>> plain = assembler_.assemble_first(guess, static_cast<std::uint64_t>(pc));
                bool found = !plain.empty();
                if (!found) {
                    if (const std::string folded = fold_operands(guess, pc); folded != guess) found = !assembler_.assemble_first(folded, static_cast<std::uint64_t>(pc)).empty();
                }
                if (found && !plain.empty() && guess.find_first_of("@%") == std::string::npos) {   // (a line with no half of an address in it: this answer is what assembling it gives)
                    std::lock_guard hold(memo_lock_);
                    if (memo_.size() >= kMemoLimit) memo_.clear();
                    memo_.emplace(std::format("{:x}|{}", static_cast<std::uint64_t>(pc), guess), Bytes(plain.front().begin(), plain.front().end()));
                }
                for (const bool signed_lo : {true, false}) {   // (the label inside a %hi or %lo, or under an @ha, @h, @l)
                    if (found) break;
                    found = !assembler_.assemble_first(fold_halves(guess, pc, signed_lo), static_cast<std::uint64_t>(pc)).empty();
                    for (const bool signed_hi : {false, true})
                        if (!found) found = !assembler_.assemble_first(fold_relocations(guess, signed_lo, signed_hi), static_cast<std::uint64_t>(pc)).empty();
                }
                if (found) {
                    remember_failure(u.word);
                    return Failure(build::UndefinedSymbolError(u.word));
                }
            }
        }
        remember_failure({});
        return Failure(build::AsmError(0, text, "not an instruction of the processor's description"));
    }

private:
    std::filesystem::path sla_;
    std::vector<std::pair<std::string, int>> context_;
    static constexpr std::size_t kMemoLimit = 2'000'000;   // (a few hundred bytes a line: a large program's lines all stay)

    // The engine's assembler holds the parse tables it fills as it goes and the translator it checks each encoding with: one call at a time
    // on each. There is one per thread that asks at the same time (up to the worker count), made when first needed, and handed back after
    // each line, so the threads of a writer assemble side by side.
    decompile::sleigh::Assembler& borrow() const {
        std::unique_lock hold(pool_lock_);
        for (;;) {
            if (!free_.empty()) {
                decompile::sleigh::Assembler* a = free_.back();
                free_.pop_back();
                return *a;
            }
            if (made_ < pool_limit_) break;
            pool_wake_.wait(hold);
        }
        ++made_;
        hold.unlock();
        std::unique_ptr<decompile::sleigh::Assembler> fresh;
        try {
            std::lock_guard one(loading_lock());
            fresh = std::make_unique<decompile::sleigh::Assembler>(sla_, context_);
        } catch (...) {
            std::lock_guard again(pool_lock_);
            --made_;
            pool_wake_.notify_one();
            throw;
        }
        std::lock_guard again(pool_lock_);
        all_.push_back(std::move(fresh));
        return *all_.back();
    }
    void give_back(decompile::sleigh::Assembler& a) const {
        {
            std::lock_guard hold(pool_lock_);
            free_.push_back(&a);
        }
        pool_wake_.notify_one();
    }
    class Lease {
    public:
        explicit Lease(const SleighSyntax& owner) : owner_(owner), assembler_(owner.borrow()) {}
        Lease(const Lease&) = delete;
        Lease& operator=(const Lease&) = delete;
        ~Lease() { owner_.give_back(assembler_); }
        decompile::sleigh::Assembler& assembler() const { return assembler_; }

    private:
        const SleighSyntax& owner_;
        decompile::sleigh::Assembler& assembler_;
    };

    mutable std::mutex pool_lock_;
    mutable std::condition_variable pool_wake_;
    mutable std::vector<std::unique_ptr<decompile::sleigh::Assembler>> all_;
    mutable std::vector<decompile::sleigh::Assembler*> free_;
    mutable std::size_t made_ = 0;
    std::size_t pool_limit_ = 1;
    mutable std::mutex memo_lock_;
    mutable std::unordered_map<std::string, Bytes> memo_;
    mutable std::unordered_map<std::string, std::string> failed_;   // a line that failed: the label it names that is not defined yet, or empty (no instruction)
};

}  // namespace

std::mutex& loading_lock() {
    static std::mutex lock;
    return lock;
}

const build::Encoder& sleigh_encoder(const std::filesystem::path& sla, const std::vector<std::pair<std::string, int>>& context) {
    static std::mutex lock;
    static std::map<std::string, std::unique_ptr<SleighSyntax>> made;
    std::string key = sla.generic_string();
    for (const auto& [k, v] : context) key += std::format("|{}={}", k, v);
    std::lock_guard hold(lock);
    std::unique_ptr<SleighSyntax>& e = made[key];
    if (!e) e = std::make_unique<SleighSyntax>(sla, context);
    return *e;
}

}  // namespace cb::source::isa
