#include "cb/formats/xbox/crypto/bootchain.hpp"

#include <algorithm>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/log.hpp"
#include "cb/formats/xbox/crypto/tool_support.hpp"

namespace cb::formats::xbox::crypto::bootchain {

namespace {

constexpr std::uint64_t kCbOffset = 0x8000;
constexpr std::uint64_t kCeOffsets[] = {0x70000, 0x73000, 0x95000};
constexpr std::uint64_t kScanLimit = 0x200000;
constexpr std::uint32_t kMaxStageSize = 0x1000000;
constexpr std::size_t kHeaderSize = 16;
constexpr std::size_t kKeySize = 16;

StageKind kind_of_magic(std::uint16_t magic) {
    switch (magic) {
        case 0x4342: return StageKind::Cb;   // "CB"
        case 0x4344: return StageKind::Cd;
        case 0x4345: return StageKind::Ce;
        case 0x4346: return StageKind::Cf;
        case 0x4347: return StageKind::Cg;
        case 0x4856: return StageKind::Hv;   // "HV"
        default: return StageKind::Unknown;
    }
}

std::string name_of(StageKind kind, std::uint16_t magic) {
    switch (kind) {
        case StageKind::Cb: return "CB/2BL";
        case StageKind::Sc: return "SC/3BL";
        case StageKind::Cd: return "CD/4BL";
        case StageKind::Ce: return "CE/5BL";
        case StageKind::Cf: return "CF/6BL";
        case StageKind::Cg: return "CG/7BL";
        case StageKind::Hv: return "HV";
        case StageKind::Unknown: break;
    }
    return std::format("Unknown(0x{:04X})", magic);
}

bool has_key(StageKind kind) { return kind != StageKind::Hv && kind != StageKind::Unknown && kind != StageKind::Sc; }

// A number with thousands separators: 16,777,216.
std::string grouped(std::uint64_t n) {
    std::string digits = std::to_string(n), out;
    for (std::size_t i = 0; i < digits.size(); ++i) {
        if (i && (digits.size() - i) % 3 == 0) out += ',';
        out += digits[i];
    }
    return out;
}

}  // namespace

std::optional<StageHeader> parse_header(ByteView data, std::uint64_t offset) {
    if (offset > data.size() || data.size() - offset < kHeaderSize) return std::nullopt;
    io::ByteReader r(data, "bootchain");
    r.seek(offset);
    StageHeader h;
    h.magic = r.u16_be();
    h.build = r.u16_be();
    h.pairing = r.u16_be();
    h.flags = r.u16_be();
    h.entry_point = r.u32_be();
    h.size = r.u32_be();
    h.offset = offset;
    h.kind = kind_of_magic(h.magic);
    h.name = name_of(h.kind, h.magic);
    h.devkit = (h.pairing & 0x8000) != 0;
    h.manufacturing = (h.flags & 0x0001) != 0;
    h.cba = (h.flags & 0x0800) != 0;
    if (has_key(h.kind) && data.size() - offset >= 2 * kHeaderSize) {
        std::array<std::uint8_t, kKeySize> key{};
        std::copy_n(data.begin() + static_cast<std::ptrdiff_t>(offset + kHeaderSize), kKeySize, key.begin());
        h.key = key;
    }
    return h;
}

bool plausible(const StageHeader& header) {
    return header.kind != StageKind::Unknown && header.size != 0 && header.size <= kMaxStageSize && header.build != 0;
}

Bootchain scan(ByteView nand) {
    Bootchain chain;
    chain.nand_size = nand.size();
    std::vector<std::uint64_t> found;
    const auto already = [&found](std::uint64_t offset) { return std::find(found.begin(), found.end(), offset) != found.end(); };
    const auto add = [&](const StageHeader& h) {
        chain.stages.push_back(h);
        found.push_back(h.offset);
    };

    // CB at its usual place, CB_B after it, CD after that
    if (const auto cb = parse_header(nand, kCbOffset); cb && plausible(*cb)) {
        add(*cb);
        chain.cb_build = cb->build;
        if (auto cb_b = parse_header(nand, kCbOffset + cb->size); cb_b && plausible(*cb_b) && cb_b->kind == StageKind::Cb) {
            cb_b->name = "CB_B/2BL_B";
            add(*cb_b);
            if (const auto cd = parse_header(nand, cb_b->offset + cb_b->size); cd && plausible(*cd) && cd->kind == StageKind::Cd) {
                add(*cd);
                chain.cd_build = cd->build;
            }
        }
    }
    // CE (the kernel stage) at one of its usual places
    for (const std::uint64_t at : kCeOffsets) {
        if (already(at) || at >= nand.size()) continue;
        if (const auto ce = parse_header(nand, at); ce && plausible(*ce) && ce->kind == StageKind::Ce) {
            add(*ce);
            chain.ce_build = ce->build;
            break;
        }
    }
    // any stage left, on a 16-byte step; a second copy of a build/kind already found is not another stage
    for (std::uint64_t at = 0; at < std::min<std::uint64_t>(nand.size(), kScanLimit); at += kHeaderSize) {
        if (already(at)) continue;
        if (nand.size() - at < kHeaderSize) break;
        const auto h = parse_header(nand, at);
        if (!h || h->kind == StageKind::Unknown || !plausible(*h)) continue;
        const bool duplicate = std::any_of(chain.stages.begin(), chain.stages.end(), [&](const StageHeader& s) { return s.build == h->build && s.kind == h->kind; });
        if (!duplicate) add(*h);
    }
    std::stable_sort(chain.stages.begin(), chain.stages.end(), [](const StageHeader& a, const StageHeader& b) { return a.offset < b.offset; });
    return chain;
}

Bytes extract(ByteView nand, const StageHeader& stage) {
    if (stage.offset >= nand.size()) return {};
    const std::uint64_t end = std::min<std::uint64_t>(nand.size(), stage.offset + stage.size);
    return Bytes(nand.begin() + static_cast<std::ptrdiff_t>(stage.offset), nand.begin() + static_cast<std::ptrdiff_t>(end));
}

std::string report(const Bootchain& chain) {
    std::string out = std::format("NAND Bootchain Analysis ({} bytes)\n\n", grouped(chain.nand_size));
    if (chain.stages.empty()) return out + "  No bootloader stages found.\n";
    for (const StageHeader& s : chain.stages) {
        std::string flags;
        for (const auto& [on, label] : {std::pair{s.devkit, "DEVKIT"}, std::pair{s.manufacturing, "MFG"}, std::pair{s.cba, "CB_A"}})
            if (on) flags += (flags.empty() ? "" : ", ") + std::string(label);
        out += std::format("  {:<12} v{:<6} @ 0x{:06X}  size=0x{:06X}  entry=0x{:08X}{}\n", s.name, s.build, s.offset, s.size, s.entry_point,
                           flags.empty() ? "" : "  [" + flags + "]");
        if (s.key && std::any_of(s.key->begin(), s.key->end(), [](std::uint8_t b) { return b != 0; }))
            out += std::format("    {:<12} {}\n", "Key:", support::hex_upper(*s.key));
    }
    return out;
}

cli::Command command() {
    return cli::Command{
        "bootchain",
        "the boot chain (CB/CD/CE/CF/CG stages) of an Xbox 360 NAND dump",
        {cli::Positional{"nand", "the NAND dump"},
         cli::Option{"--extract", "", "write stage N of the report (from 0) to --output", cli::Arg::Value, "N", ""},
         cli::Option{"--output", "-o", "the file --extract writes", cli::Arg::Value, "FILE", ""}},
        [](const cli::Args& args) {
            const Bytes nand = fs::read_file(fs::from_text(args.required("nand")));
            const Bootchain chain = scan(nand);
            cb::out_text(report(chain));
            if (const std::optional<std::size_t> n = args.integer<std::size_t>("extract")) {
                const std::optional<std::string> output = args.value("output");
                if (!output) throw UsageError("--extract needs --output");
                if (*n >= chain.stages.size()) throw UsageError(std::format("there is no stage {} (found {})", *n, chain.stages.size()));
                fs::write_file(fs::from_text(*output), extract(nand, chain.stages[*n]));
            }
            return 0;
        },
        {},
        "",
        ""};
}

}  // namespace cb::formats::xbox::crypto::bootchain
