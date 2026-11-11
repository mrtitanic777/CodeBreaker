// cb/symbols/xdk_libraries.cpp -- naming statically linked XDK functions (see xdk_libraries.hpp).
#include "cb/symbols/xdk_libraries.hpp"

#include <algorithm>
#include <chrono>
#include <format>

#include <nlohmann/json.hpp>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"

namespace cb::symbols::xdk_libraries {
namespace {

using std::string;

// The code without the zero words that pad a function to its alignment: how many words are left.
std::size_t words_without_padding(ByteView code) {
    std::size_t n = code.size() / 4;
    while (n > 0 && code[4 * n - 4] == 0 && code[4 * n - 3] == 0 && code[4 * n - 2] == 0 && code[4 * n - 1] == 0) --n;
    return n;
}

std::uint32_t word_at(ByteView code, std::size_t index) {
    return (static_cast<std::uint32_t>(code[4 * index]) << 24) | (static_cast<std::uint32_t>(code[4 * index + 1]) << 16) |
           (static_cast<std::uint32_t>(code[4 * index + 2]) << 8) | static_cast<std::uint32_t>(code[4 * index + 3]);
}

struct SignatureKey {
    std::vector<std::uint32_t> words;
    std::vector<std::uint32_t> ignore;
    friend auto operator<=>(const SignatureKey&, const SignatureKey&) = default;
};

// A program's words indexed by each pair of neighbours: a function is looked for where its rarest pair of whole words
// is (a prologue's pair is everywhere; a pair the program does not have at all rules it out at once). The index is the
// (pair, position) entries sorted: millions of words build it far faster than a table of lists would.
class PairIndex {
public:
    explicit PairIndex(const std::vector<std::uint32_t>& words) : words_(words) {
        entries_.reserve(words.size());
        for (std::size_t i = 0; i + 1 < words.size(); ++i) entries_.emplace_back(key(words[i], words[i + 1]), static_cast<std::uint32_t>(i));
        std::sort(entries_.begin(), entries_.end());
    }

    // Every place, in order, where the words `sig` are, outside the bits `ignore` leaves out of each; nothing when no
    // two neighbours of it are whole (there is nothing to look it up by).
    std::vector<std::uint32_t> find(const std::vector<std::uint32_t>& sig, const std::vector<std::uint32_t>& ignore) const {
        using Range = std::pair<Entries::const_iterator, Entries::const_iterator>;
        std::optional<std::size_t> anchor;
        Range best;
        std::size_t fewest = 0;
        for (std::size_t i = 0; i + 1 < ignore.size(); ++i) {
            if (ignore[i] != 0 || ignore[i + 1] != 0) continue;
            const Range r = occurrences(sig[i], sig[i + 1]);
            const std::size_t n = static_cast<std::size_t>(r.second - r.first);
            if (!anchor || n < fewest) {
                anchor = i;
                best = r;
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

private:
    using Entries = std::vector<std::pair<std::uint64_t, std::uint32_t>>;

    static std::uint64_t key(std::uint32_t a, std::uint32_t b) { return (static_cast<std::uint64_t>(a) << 32) | b; }

    std::pair<Entries::const_iterator, Entries::const_iterator> occurrences(std::uint32_t a, std::uint32_t b) const {
        const std::uint64_t k = key(a, b);
        const auto first = std::lower_bound(entries_.begin(), entries_.end(), std::make_pair(k, std::uint32_t{0}));
        auto last = first;
        while (last != entries_.end() && last->first == k) ++last;
        return {first, last};
    }

    const std::vector<std::uint32_t>& words_;
    Entries entries_;   // sorted by pair, then by position
};

const char* kUsage =
    "codebreaker xdk-match <program.xex or image> [<XDK lib folder>] [-o names.json]\n\n"
    "Names the XDK library functions a program was statically linked with, by their code.";

}  // namespace

std::vector<Signature> make_signatures(std::span<const Library> libraries) {
    std::vector<const Library*> ordered;
    for (const Library& l : libraries) ordered.push_back(&l);
    std::stable_sort(ordered.begin(), ordered.end(), [](const Library* a, const Library* b) { return a->name < b->name; });

    std::map<SignatureKey, std::size_t> at;   // lookup into `out`
    std::vector<Signature> out;
    for (const Library* library : ordered) {
        for (const LibraryFunction& f : library->functions) {
            const std::size_t n = std::min(words_without_padding(f.code), f.ignore.size());
            if (n < 4) continue;
            SignatureKey key;
            for (std::size_t i = 0; i < n; ++i) key.words.push_back(word_at(f.code, i));
            key.ignore.assign(f.ignore.begin(), f.ignore.begin() + static_cast<std::ptrdiff_t>(n));
            auto [it, fresh] = at.try_emplace(key, out.size());
            if (fresh) {
                Signature s;
                s.words = key.words;
                s.ignore = key.ignore;
                out.push_back(std::move(s));
            }
            Signature& entry = out[it->second];
            entry.names.insert(f.name);
            entry.libs.insert(library->name);
            entry.secondary = entry.secondary || f.at != 0;
        }
    }
    return out;
}

std::string signatures_stamp(std::span<const LibraryFileInfo> files) {
    string text;
    for (const LibraryFileInfo& f : files) text += std::format("{}:{}:{};", f.name, f.size, f.modified);
    const crypto::Sha1Digest digest = crypto::sha1(as_bytes(text));
    return std::format("{}-{}", kReaderRevision, crypto::to_hex(digest).substr(0, 16));
}

std::vector<LibraryFileInfo> library_files(const std::filesystem::path& folder) {
    std::vector<LibraryFileInfo> out;
    std::error_code ec;
    for (const auto& entry : std::filesystem::directory_iterator(folder, ec)) {
        const string name = fs::to_text(entry.path().filename());
        if (!text::iends_with(name, ".lib")) continue;
        LibraryFileInfo info;
        info.name = name;
        info.size = entry.file_size(ec);
        const auto time = entry.last_write_time(ec);
        info.modified = std::chrono::duration_cast<std::chrono::seconds>(time.time_since_epoch()).count();
        out.push_back(std::move(info));
    }
    std::sort(out.begin(), out.end(), [](const auto& a, const auto& b) { return a.name < b.name; });
    return out;
}

bool falls_through(std::uint32_t w) {
    const std::uint32_t op = w >> 26;
    if (w == 0 || op == 18) return false;   // padding; b / bl / ba
    if (op == 19) {                         // unconditional blr / bctr
        const std::uint32_t xo = (w >> 1) & 0x3FF;
        if ((xo == 16 || xo == 528) && ((w >> 21) & 0x14) == 0x14) return false;
    }
    if (op == 3 || (op == 31 && ((w >> 1) & 0x3FF) == 4)) return false;   // twi / tw
    return true;
}

std::map<std::uint32_t, Found> match(const ProgramText& program, std::span<const Signature> signatures) {
    const PairIndex index(program.words);
    std::map<std::uint32_t, std::pair<std::set<string>, std::set<string>>> sets;
    std::map<std::uint32_t, std::uint32_t> sizes;
    for (const Signature& s : signatures) {
        const auto whole = std::count(s.ignore.begin(), s.ignore.end(), 0u);
        if (whole < 3) continue;   // too generic to place
        std::vector<std::uint32_t> hits;
        for (const std::uint32_t h : index.find(s.words, s.ignore)) {
            // (not the tail of another function)
            if (s.secondary || program.starts.contains(h) || h == 0 || !falls_through(program.words[h - 1])) hits.push_back(h);
        }
        if (hits.size() != 1) continue;
        const std::uint32_t address = program.address + hits[0] * 4;
        auto& [names, libs] = sets[address];
        names.insert(s.names.begin(), s.names.end());
        libs.insert(s.libs.begin(), s.libs.end());
        sizes[address] = std::max(sizes[address], static_cast<std::uint32_t>(s.words.size() * 4));
    }
    std::map<std::uint32_t, Found> out;
    for (const auto& [address, pair] : sets) {
        Found f;
        f.names.assign(pair.first.begin(), pair.first.end());
        f.libs.assign(pair.second.begin(), pair.second.end());
        f.size = sizes[address];
        out[address] = std::move(f);
    }
    return out;
}

std::string format_matches(const std::map<std::uint32_t, Found>& found) {
    Json doc = Json::object();
    for (const auto& [address, f] : found) {
        doc[std::format("{:08X}", address)] = Json{{"names", f.names}, {"libs", f.libs}, {"size", f.size}};
    }
    return doc.dump(1);
}

int run(std::span<const std::string> args, const Services& services) {
    if (args.empty() || args[0].starts_with('-')) {
        out("{}\n", kUsage);
        return kExitUsage;
    }
    std::optional<string> json_out;
    std::vector<string> rest;
    for (std::size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "-o") {
            if (i + 1 >= args.size()) throw UsageError("xdk-match: -o needs a file name");
            if (!json_out) json_out = args[i + 1];
            ++i;
        } else {
            rest.push_back(args[i]);
        }
    }
    const std::filesystem::path program_path = fs::from_text(rest[0]);
    const std::filesystem::path folder = rest.size() > 1 ? fs::from_text(rest[1]) : services.default_folder;
    std::error_code ec;
    if (!std::filesystem::is_directory(folder, ec)) {
        out("no XDK library folder: {} (setting xdk_libs)\n", fs::to_text(folder));
        return kExitFailed;
    }
    const std::map<std::uint32_t, Found> found = match(services.load_program(program_path), services.load_signatures(folder));
    std::uint64_t text_size = 0;
    std::vector<std::pair<string, std::size_t>> per_lib;   // in the order each library was first met, by address
    for (const auto& [address, f] : found) {
        text_size += f.size;
        const auto it = std::find_if(per_lib.begin(), per_lib.end(), [&](const auto& p) { return p.first == f.libs[0]; });
        if (it == per_lib.end()) per_lib.emplace_back(f.libs[0], 1);
        else ++it->second;
    }
    string digits = std::to_string(text_size);
    string grouped;
    for (std::size_t i = 0; i < digits.size(); ++i) {
        if (i > 0 && (digits.size() - i) % 3 == 0) grouped.push_back(',');
        grouped.push_back(digits[i]);
    }
    out("{}: {} library functions named ({} bytes)\n", fs::to_text(program_path.filename()), found.size(), grouped);
    std::stable_sort(per_lib.begin(), per_lib.end(), [](const auto& a, const auto& b) { return a.second > b.second; });
    for (std::size_t i = 0; i < per_lib.size() && i < 15; ++i) out("  {:>6}  {}\n", per_lib[i].second, per_lib[i].first);
    if (json_out) {
        fs::write_text(fs::from_text(*json_out), format_matches(found));
        out("  -> {}\n", *json_out);
    }
    return kExitOk;
}

}  // namespace cb::symbols::xdk_libraries
