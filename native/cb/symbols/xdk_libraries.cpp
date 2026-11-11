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

const char* kUsage =
    "codebreaker xdk-match <program.xex or image> [<XDK lib folder>] [-o names.json]\n\n"
    "Names the XDK library functions a program was statically linked with, by their code.";

}  // namespace

std::vector<Library> read_libraries(const std::filesystem::path& folder) {
    std::vector<Library> out;
    for (const LibraryFileInfo& file : library_files(folder)) {
        libraries::Library lib;
        try {
            lib = libraries::read_file(folder / fs::from_text(file.name));
        } catch (const Error&) {   // one file of the folder that cannot be read gives nothing
            continue;
        }
        if (lib.machine != libraries::Machine::PowerPc) continue;
        out.push_back({file.name, std::move(lib.functions)});
    }
    return out;
}

std::vector<Signature> make_signatures(std::span<const Library> libraries) {
    std::vector<const Library*> ordered;
    for (const Library& l : libraries) ordered.push_back(&l);
    std::stable_sort(ordered.begin(), ordered.end(), [](const Library* a, const Library* b) { return a->name < b->name; });

    std::map<SignatureKey, std::size_t> at;   // lookup into `out`
    std::vector<Signature> out;
    for (const Library* library : ordered) {
        for (const libraries::Function& f : library->functions) {
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
    const libraries::Index index(program.words);
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
