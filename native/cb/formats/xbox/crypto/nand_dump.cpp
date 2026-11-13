#include "cb/formats/xbox/crypto/nand_dump.hpp"

#include <algorithm>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/log.hpp"
#include "cb/formats/xbox/crypto/tool_support.hpp"

namespace cb::formats::xbox::crypto::nand_dump {

namespace {

constexpr std::uint64_t kMiB = 1024 * 1024;

}  // namespace

std::string type_of(std::uint64_t size) {
    switch (size) {
        case 16 * kMiB: return "16MB (small block)";
        case 64 * kMiB: return "64MB (big block / jasper)";
        case 256 * kMiB: return "256MB (corona MMC)";
        case 512 * kMiB: return "512MB (corona 4GB MMC)";
        default: return std::format("Unknown ({} bytes)", size);
    }
}

Summary summarise(ByteView dump) {
    Summary summary{dump.size(), type_of(dump.size()), std::nullopt};
    if (dump.size() >= kHeaderSize) {
        io::ByteReader r(dump, "nand");
        Header h;
        h.magic = {r.u8(), r.u8()};
        h.build = r.u16_be();
        h.qfe = r.u16_be();
        h.flags = r.u16_be();
        h.entry_point = r.u32_be();
        h.size = r.u32_be();
        summary.header = h;
    }
    return summary;
}

Bytes extract_hypervisor(ByteView dump) {
    if (dump.size() < kHypervisorSize)
        throw FormatError("nand", dump.size(), std::format("NAND dump too small ({} bytes, need {})", dump.size(), kHypervisorSize));
    return Bytes(dump.begin(), dump.begin() + static_cast<std::ptrdiff_t>(kHypervisorSize));
}

cli::Command command() {
    cli::Command info{
        "info",
        "the type and header of a NAND dump",
        {cli::Positional{"dump", "the NAND dump"}},
        [](const cli::Args& args) {
            const std::string path = args.required("dump");
            const Summary s = summarise(fs::read_file(fs::from_text(path)));
            cb::out("  {:<20} {}\n  {:<20} {}\n  {:<20} {}\n", "path", path, "size", s.size, "type", s.type);
            if (s.header) {
                const Header& h = *s.header;
                cb::out("  {:<20} {}\n  {:<20} {}\n  {:<20} {}\n  {:<20} 0x{:04X}\n  {:<20} 0x{:08X}\n  {:<20} 0x{:08X}\n", "magic",
                        support::hex_upper(h.magic), "build", h.build, "qfe", h.qfe, "flags", h.flags, "entry_point", h.entry_point, "header size", h.size);
            }
            return 0;
        },
        {}, "", ""};
    const auto extract_handler = [](const cli::Args& args) {
        const Bytes hv = extract_hypervisor(fs::read_file(fs::from_text(args.required("dump"))));
        fs::write_file(fs::from_text(args.required("output")), hv);
        cb::out("extracted HV ({} bytes) -> {}\n", hv.size(), args.required("output"));
        return 0;
    };
    const std::vector<cli::Param> extract_params = {cli::Positional{"dump", "the NAND dump"}, cli::Positional{"output", "the file to write"}};
    return cli::Command{"nand",
                        "inspect an Xbox 360 NAND dump: its type and header, the hypervisor in it",
                        {},
                        {},
                        {std::move(info), cli::Command{"extract-hv", "write the first 256 KB (the hypervisor)", extract_params, extract_handler, {}, "", ""},
                         cli::Command{"hv", "the same as extract-hv", extract_params, extract_handler, {}, "", ""}},
                        "",
                        ""};
}

}  // namespace cb::formats::xbox::crypto::nand_dump
