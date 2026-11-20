// cb-unity: alone (file-local names that other files in a batch also use)
// source/isa_sleigh.cpp -- see isa_sleigh.hpp.

#include "source/isa_sleigh.hpp"

#include <algorithm>
#include <cctype>
#include <format>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <unordered_map>

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
std::string fold_relocations(const std::string& text, bool signed_lo) {
    std::string out;
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
                    if (ha) out += std::format("0x{:x}", ((v + 0x8000) >> 16) & 0xFFFF);
                    else if (hi) out += std::format("0x{:x}", (v >> 16) & 0xFFFF);
                    else if (signed_lo && (v & 0x8000)) out += std::format("-0x{:x}", 0x10000 - (v & 0xFFFF));
                    else out += std::format("0x{:x}", v & 0xFFFF);
                    k = e + (ha ? 3 : hi ? 2 : 2);
                    continue;
                }
            }
        }
        out += text[k++];
    }
    return out;
}

class SleighSyntax final : public build::Encoder {
public:
    SleighSyntax(const std::filesystem::path& sla, const std::vector<std::pair<std::string, int>>& context) : sla_(sla), context_(context), assembler_(sla, context) {}

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
    Result<Bytes> assemble(std::string_view line, std::int64_t pc, const build::Labels& labels) const override {
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
                if (const auto hit = labels.find(word); hit != labels.end()) out += std::format("0x{:x}", static_cast<std::uint64_t>(hit->second)) + suffix;
                else {
                    unknown.push_back({word, out.size()});
                    out += word + suffix;
                }
                k = e;
            } else {
                out += text[k++];
            }
        }
        std::lock_guard hold(lock_);
        if (out.find("%lo(") != std::string::npos || out.find("%hi(") != std::string::npos || out.find("%up(") != std::string::npos) {
            for (const bool signed_lo : {true, false}) {   // (an ori's low half is unsigned, an addiu's signed: the one the instruction takes)
                const std::vector<std::vector<std::uint8_t>> folded = assembler_.assemble(fold_halves(out, pc, signed_lo), static_cast<std::uint64_t>(pc));
                if (!folded.empty()) return Bytes(folded.front().begin(), folded.front().end());
            }
        }
        if (out.find('@') != std::string::npos) {
            for (const bool signed_lo : {true, false}) {
                const std::vector<std::vector<std::uint8_t>> folded = assembler_.assemble(fold_relocations(out, signed_lo), static_cast<std::uint64_t>(pc));
                if (!folded.empty()) return Bytes(folded.front().begin(), folded.front().end());
            }
        }
        // (a program's lines are assembled again and again: a writer checks each instruction, then the build makes two passes; the engine takes
        // milliseconds for a line, so the lines it has encoded are kept, by address and text)
        const std::string memo_key = std::format("{:x}|{}", static_cast<std::uint64_t>(pc), out);
        if (const auto hit = memo_.find(memo_key); hit != memo_.end()) return hit->second;
        const std::vector<std::vector<std::uint8_t>> encodings = assembler_.assemble(out, static_cast<std::uint64_t>(pc));
        if (!encodings.empty()) {
            if (memo_.size() >= 100000) memo_.clear();
            return memo_.emplace(memo_key, Bytes(encodings.front().begin(), encodings.front().end())).first->second;
        }
        // Which unknown word is the label not defined yet? The one that, as a number, makes the line an instruction (in `JR NZ,ahead`
        // NZ is the description's and `ahead` the label): the directive layer then guesses its value and assembles again.
        for (const Unknown& u : unknown) {
            // (the places a label may be taken to be: here, just ahead, and a word-aligned place ahead, which a PC-relative load of Thumb reaches)
            for (const std::int64_t at : {pc, pc + 4, (pc & ~std::int64_t{3}) + 8}) {
                std::string guess = out;
                guess.replace(u.at, u.word.size(), std::format("0x{:x}", static_cast<std::uint64_t>(at)));
                if (!assembler_.assemble(guess, static_cast<std::uint64_t>(pc)).empty()) return Failure(build::UndefinedSymbolError(u.word));
                for (const bool signed_lo : {true, false}) {   // (the label inside a %hi or %lo, or under an @ha, @h, @l)
                    if (!assembler_.assemble(fold_halves(guess, pc, signed_lo), static_cast<std::uint64_t>(pc)).empty()) return Failure(build::UndefinedSymbolError(u.word));
                    if (!assembler_.assemble(fold_relocations(guess, signed_lo), static_cast<std::uint64_t>(pc)).empty()) return Failure(build::UndefinedSymbolError(u.word));
                }
            }
        }
        return Failure(build::AsmError(0, text, "not an instruction of the processor's description"));
    }

private:
    std::filesystem::path sla_;
    std::vector<std::pair<std::string, int>> context_;
    decompile::sleigh::Assembler assembler_;
    mutable std::unordered_map<std::string, Bytes> memo_;
    mutable std::mutex lock_;
};

}  // namespace

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
