// cb-unity: alone (file-local names that other files in a batch also use)
// source/isa_sleigh.cpp -- see isa_sleigh.hpp.

#include "source/isa_sleigh.hpp"

#include <cctype>
#include <format>
#include <map>
#include <memory>
#include <mutex>
#include <optional>

#include "core/text.hpp"
#include "decompile/engine/sleigh_assembler.hh"
#include "decompile/ppc.hpp"

namespace cb::source::isa {
namespace {

namespace dp = decompile::ppc;
namespace dz = decompile::z80;

bool word_start(char c) { return std::isalpha(static_cast<unsigned char>(c)) != 0 || c == '_' || c == '.' || c == '@'; }
bool word_char(char c) { return std::isalnum(static_cast<unsigned char>(c)) != 0 || c == '_' || c == '.' || c == '@'; }

class SleighSyntax final : public build::Encoder {
public:
    SleighSyntax(const std::filesystem::path& sla, const std::vector<std::pair<std::string, int>>& context) : assembler_(sla, context) {}

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
        std::optional<std::string> unknown;
        for (std::size_t k = mnemonic_end; k < text.size();) {
            if (word_start(text[k]) && (k == 0 || !word_char(text[k - 1]))) {
                std::size_t e = k;
                while (e < text.size() && word_char(text[e])) ++e;
                const std::string word(text.substr(k, e - k));
                if (const auto hit = labels.find(word); hit != labels.end()) out += std::format("0x{:x}", static_cast<std::uint64_t>(hit->second));
                else {
                    out += word;
                    if (!unknown) unknown = word;
                }
                k = e;
            } else {
                out += text[k++];
            }
        }
        std::lock_guard hold(lock_);
        const std::vector<std::vector<std::uint8_t>> encodings = assembler_.assemble(out, static_cast<std::uint64_t>(pc));
        if (!encodings.empty()) return Bytes(encodings.front().begin(), encodings.front().end());
        if (unknown) return Failure(build::UndefinedSymbolError(*unknown));
        return Failure(build::AsmError(0, text, "not an instruction of the processor's description"));
    }

private:
    decompile::sleigh::Assembler assembler_;
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
