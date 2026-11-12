#include "cb/recompile/discover.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <string_view>

#include "cb/core/cli.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"

namespace cb::recompile::discover {
namespace {

constexpr std::uint32_t kWord = 4;
constexpr std::uint32_t kExecutable = 0x20000000;   // section characteristics: contains code
constexpr std::uint32_t kBlr = 0x4E800020, kBctr = 0x4E800420;
constexpr std::uint32_t kMaxRunWithoutRecord = 0x4000;   // how far to read a function that has no .pdata record

// What a function's first instruction plausibly is.
constexpr auto kPrologue = std::to_array<std::string_view>({"mflr", "stwu", "stw", "std", "mr", "li", "lis", "or", "addi", "stmw", "b", "clrlwi", "rlwinm",
                                                            "cmpwi", "cmplwi", "lwz", "add"});

// the extended opcodes (opcode 19) that end a run: bclr, bcctr, rfid
constexpr auto kRunEnders = std::to_array<std::uint32_t>({16, 528, 18});

std::uint32_t word_in(ByteView bytes, std::size_t offset) {
    return (static_cast<std::uint32_t>(bytes[offset]) << 24) | (static_cast<std::uint32_t>(bytes[offset + 1]) << 16) |
           (static_cast<std::uint32_t>(bytes[offset + 2]) << 8) | bytes[offset + 3];
}

// Whether an address lies strictly inside a function of known size (not at its start).
bool inside_known(const Functions& known, std::uint32_t address) {
    auto next = known.upper_bound(address);   // the first start after `address`
    if (next == known.begin()) return false;
    const auto& [start, size] = *std::prev(next);
    return start < address && address - start < size;
}

// Whether `address` is a candidate worth the disassembler: aligned, in the code, and not already a function or in one.
bool unclaimed(const Image& image, const Functions& known, std::uint32_t address) {
    return !(address & (kWord - 1)) && address >= image.code_begin && address < image.code_end && !known.contains(address) && !inside_known(known, address);
}

bool is_data_section(const Image::Section& s) { return !(s.characteristics & kExecutable) && s.name != ".text"; }

}  // namespace

bool plausible_start(Disassembler& dis, const Image& image, std::uint32_t address) {
    if (address < image.code_begin) return false;
    const std::size_t offset = address - image.code_begin;
    const std::vector<disassemble::Row> rows = dis.listing(image.code, image.code_begin, offset, 1);
    if (rows.empty() || rows.front().mnemonic == ".long") return false;
    if (std::ranges::find(kPrologue, rows.front().mnemonic) != kPrologue.end()) return true;
    if (offset >= kWord) {
        // the word before ends a function: blr, bctr, padding, or a plain branch
        const std::uint32_t before = word_in(image.code, offset - kWord);
        return before == 0 || before == kBlr || before == kBctr || (before >> 26 == 18 && !(before & 3));
    }
    return false;
}

std::vector<std::uint32_t> from_pointers(const Image& image, Disassembler& dis, const Functions& known) {
    std::set<std::uint32_t> found;
    for (const Image::Section& section : image.sections) {
        if (!is_data_section(section)) continue;
        for (std::size_t o = 0; o + kWord <= section.bytes.size(); o += kWord) {
            const std::uint32_t address = word_in(section.bytes, o);
            if (unclaimed(image, known, address) && plausible_start(dis, image, address)) found.insert(address);
        }
    }
    return {found.begin(), found.end()};
}

std::vector<std::uint32_t> from_constants(const Image& image, Disassembler& dis, const Functions& known) {
    std::array<std::optional<std::uint32_t>, 32> high;   // register -> the value its last lis gave it
    std::set<std::uint32_t> found;
    for (std::size_t o = 0; o + kWord <= image.code.size(); o += kWord) {
        const std::uint32_t w = word_in(image.code, o);
        const std::uint32_t op = w >> 26, rd = (w >> 21) & 31, ra = (w >> 16) & 31, imm = w & 0xFFFF;
        std::uint32_t address;
        if (op == 15 && ra == 0) {   // lis rD, imm
            high[rd] = imm << 16;
            continue;
        }
        if (op == 14 && high[ra]) {   // addi rD, rA, simm
            address = *high[ra] + static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(imm)));
        } else if (op == 24 && high[rd]) {   // ori rA, rS, uimm (rS is bits 21-25)
            address = *high[rd] | imm;
        } else {
            continue;
        }
        if (unclaimed(image, known, address) && !found.contains(address) && plausible_start(dis, image, address)) found.insert(address);
    }
    return {found.begin(), found.end()};
}

Discovery discover(const Image& image, Disassembler& dis, const Options& options) {
    Discovery out;
    Functions& funcs = out.functions;
    Sources& sources = out.sources;
    for (const std::uint32_t a : options.seeds) {
        if (funcs.emplace(a, 0).second) ++sources.seed;
    }
    for (const auto& [start, size] : image.extents) funcs[start] = size;
    sources.pdata = image.extents.size();
    if (image.entry != 0 && !funcs.contains(image.entry)) {
        funcs[image.entry] = 0;
        sources.entry = 1;
    }
    auto sized = [&funcs] {
        Functions known;
        for (const auto& [start, size] : funcs) {
            if (size != 0) known.emplace(start, size);
        }
        return known;
    };
    if (options.pointer_scan) {
        for (const std::uint32_t a : from_pointers(image, dis, sized())) {
            if (funcs.emplace(a, 0).second) ++sources.pointers;
        }
        for (const std::uint32_t a : from_constants(image, dis, sized())) {
            if (funcs.emplace(a, 0).second) ++sources.constants;
        }
    }

    // follow every bl from every function to a fixed point (raw opcode bits: a bl is opcode 18 with LK)
    const std::uint32_t lo = image.code_begin, hi = image.code_end;
    std::vector<std::uint32_t> work;
    std::set<std::uint32_t> seen;
    for (const auto& entry : funcs) {
        work.push_back(entry.first);
        seen.insert(entry.first);
    }
    while (!work.empty()) {
        const std::uint32_t start = work.back();
        work.pop_back();
        if (start < lo || start >= hi) continue;
        const std::uint32_t size = funcs[start];
        const std::uint64_t end = size ? std::min<std::uint64_t>(std::uint64_t{start} + size, hi) : std::min<std::uint64_t>(hi, std::uint64_t{start} + kMaxRunWithoutRecord);
        for (std::uint64_t at = start; at + kWord <= end && at - lo + kWord <= image.code.size(); at += kWord) {
            const std::uint32_t w = word_in(image.code, static_cast<std::size_t>(at - lo));
            const std::uint32_t op = w >> 26;
            if (op == 18) {
                std::int64_t displacement = w & 0x03FFFFFC;
                if (displacement & 0x02000000) displacement -= 0x04000000;
                if (w & 2) continue;   // absolute: not followed
                const std::int64_t target = static_cast<std::int64_t>(at) + displacement;
                if (w & 1) {
                    if (target >= lo && target < hi && !seen.contains(static_cast<std::uint32_t>(target)) && !image.imports.contains(static_cast<std::uint32_t>(target))) {
                        const std::uint32_t t = static_cast<std::uint32_t>(target);
                        seen.insert(t);
                        funcs.emplace(t, 0);
                        work.push_back(t);
                        ++sources.calls;
                    }
                } else if (!size) {
                    break;   // a plain branch ends a run with no record
                }
            } else if (!size && op == 19 && std::ranges::find(kRunEnders, (w >> 1) & 0x3FF) != kRunEnders.end()) {
                break;
            }
        }
    }
    return out;
}

int main(const ImageReader& read_image, const VectorDecoder& vector, std::span<const std::string> args) {
    const cli::Command command{
        "discover",
        "every function of an Xbox 360 image, for the recompiler",
        {cli::Positional{"image", "a PowerPC image or an .xex"}, cli::Option{"--list", "", "list every function with its size"}},
        [&](const cli::Args& a) -> int {
            const Image image = read_image(fs::from_text(a.required("image")));
            Disassembler dis(vector);
            const Discovery found = discover(image, dis);
            const Sources& s = found.sources;
            cb::out("{} functions: pdata {}, entry {}, calls {}, pointers {}, constants {}, seed {}\n", found.functions.size(), s.pdata, s.entry, s.calls,
                    s.pointers, s.constants, s.seed);
            if (a.flag("--list")) {
                for (const auto& [address, size] : found.functions) cb::out("  {:08X}  {}\n", address, size);
            }
            return 0;
        },
        {}, {}, {}};
    const cli::Cli program("codebreaker", {command});
    std::vector<std::string> line = {"discover"};
    line.insert(line.end(), args.begin(), args.end());
    return program.run(line);
}

}  // namespace cb::recompile::discover
