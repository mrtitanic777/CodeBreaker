// cb/symbols/libraries/package.cpp -- libraries of functions to find in a program (see package.hpp).
#include "cb/symbols/libraries/package.hpp"

#include <algorithm>
#include <iterator>
#include <optional>

#include "cb/compare/mask.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/symbols/libraries/coff.hpp"
#include "cb/symbols/libraries/omf.hpp"

namespace cb::symbols::libraries {
namespace {

constexpr std::string_view kArchiveMagic = "!<arch>\n";

std::optional<Machine> machine_of(std::uint16_t coff_machine) {
    switch (coff_machine) {
        case coff::kI386: return Machine::X86_32;
        case coff::kPowerPc: return Machine::PowerPc;
        default: return std::nullopt;
    }
}

bool starts_with(ByteView data, std::string_view prefix) {
    return data.size() >= prefix.size() && as_text(data.subspan(0, prefix.size())) == prefix;
}

}  // namespace

Library read(ByteView data) {
    Library lib;
    if (starts_with(data, kArchiveMagic)) {
        std::optional<Machine> machine;
        for (const coff::Member& member : coff::members(data)) {
            std::vector<Function> found;
            try {
                found = coff::functions(member.body);
            } catch (const FormatError&) {   // one object of an archive that is damaged: it gives nothing
                continue;
            }
            if (!found.empty() && !machine && member.body.size() >= 2) {
                machine = machine_of(static_cast<std::uint16_t>(member.body[0] | (member.body[1] << 8)));
            }
            lib.functions.insert(lib.functions.end(), std::make_move_iterator(found.begin()), std::make_move_iterator(found.end()));
        }
        lib.machine = machine.value_or(Machine::X86_32);
        return lib;
    }
    if (starts_with(data, std::string_view("\x4c\x01", 2))) {
        lib.machine = Machine::X86_32;
        lib.functions = coff::functions(data);
        return lib;
    }
    if (starts_with(data, std::string_view("\xf2\x01", 2))) {
        lib.machine = Machine::PowerPc;
        lib.functions = coff::functions(data);
        return lib;
    }
    lib.machine = Machine::X86_16;
    lib.functions = omf::functions(data);
    return lib;
}

Library read_file(const std::filesystem::path& path) { return read(fs::read_file(path)); }

bool fits(const Function& function, ByteView code, ByteView code_mask) {
    const std::size_t n = function.code.size();
    if (n > code.size()) return false;
    // their mask, as long as the bytes compared: what it does not cover leaves nothing out
    const ByteView theirs = code_mask.subspan(0, std::min(n, code_mask.size()));
    const compare::mask::ByteMask masks[] = {function.mask, theirs};
    return compare::mask::same(function.code, code.subspan(0, n), masks);
}

Index::Index(std::span<const std::uint32_t> words) : words_(words) {
    entries_.reserve(words.size());
    for (std::size_t i = 0; i + 1 < words.size(); ++i) {
        entries_.emplace_back((static_cast<std::uint64_t>(words[i]) << 32) | words[i + 1], static_cast<std::uint32_t>(i));
    }
    std::sort(entries_.begin(), entries_.end());
}

std::pair<Index::Entries::const_iterator, Index::Entries::const_iterator> Index::occurrences(std::uint32_t a, std::uint32_t b) const {
    const std::uint64_t key = (static_cast<std::uint64_t>(a) << 32) | b;
    const auto first = std::lower_bound(entries_.begin(), entries_.end(), std::make_pair(key, std::uint32_t{0}));
    auto last = first;
    while (last != entries_.end() && last->first == key) ++last;
    return {first, last};
}

std::vector<std::uint32_t> Index::find(std::span<const std::uint32_t> sig, std::span<const std::uint32_t> ignore) const {
    if (sig.size() != ignore.size()) throw UsageError("libraries::Index::find: `ignore` must be as long as the words");
    std::optional<std::size_t> anchor;
    std::pair<Entries::const_iterator, Entries::const_iterator> best;
    std::size_t fewest = 0;
    for (std::size_t i = 0; i + 1 < ignore.size(); ++i) {
        if (ignore[i] != 0 || ignore[i + 1] != 0) continue;
        const auto range = occurrences(sig[i], sig[i + 1]);
        const std::size_t n = static_cast<std::size_t>(range.second - range.first);
        if (!anchor || n < fewest) {
            anchor = i;
            best = range;
            fewest = n;
            if (n == 0) break;
        }
    }
    if (!anchor || fewest == 0) return {};
    std::vector<std::uint32_t> hits;
    for (auto it = best.first; it != best.second; ++it) {
        const std::uint32_t p = it->second;
        if (p < *anchor) continue;
        const std::size_t start = p - *anchor;
        if (start + sig.size() > words_.size()) continue;
        bool same = true;
        for (std::size_t k = 0; k < sig.size() && same; ++k) same = ((words_[start + k] ^ sig[k]) & ~ignore[k]) == 0;
        if (same) hits.push_back(static_cast<std::uint32_t>(start));
    }
    return hits;
}

}  // namespace cb::symbols::libraries
