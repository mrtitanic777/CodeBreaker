#include "cb/formats/xbox/crypto/xdk_extract.hpp"

#include <algorithm>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/log.hpp"

namespace cb::formats::xbox::crypto::xdk_extract {

namespace {

constexpr std::uint16_t kDosMagic = 0x5A4D;          // "MZ"
constexpr std::uint32_t kPeSignature = 0x00004550;   // "PE\0\0"
constexpr std::uint16_t kMachineI386 = 0x14C;
constexpr std::size_t kNewHeaderPointerAt = 0x3C;
constexpr std::size_t kSectionHeaderSize = 40;
constexpr std::array<std::uint8_t, 4> kCabMagic = {'M', 'S', 'C', 'F'};

}  // namespace

std::uint64_t end_of_image(ByteView exe) {
    io::ByteReader r(exe, "xdk exe");
    if (r.u16() != kDosMagic) throw FormatError("xdk exe", 0, "invalid DOS header");
    r.seek(kNewHeaderPointerAt);
    r.seek(r.u32());
    if (r.u32() != kPeSignature) throw FormatError("xdk exe", r.pos() - 4, "invalid or corrupt executable");
    if (r.u16() != kMachineI386) throw FormatError("xdk exe", r.pos() - 2, "the input file isn't an i386 executable");
    const std::uint16_t sections = r.u16();
    r.skip(12);   // time stamp, symbol table, symbol count
    const std::uint16_t optional_size = r.u16();
    r.skip(2 + optional_size);   // characteristics and the optional header
    std::uint64_t top = 0;
    for (std::uint16_t i = 0; i < sections; ++i) {
        io::ByteReader section = r.sub(kSectionHeaderSize, "xdk exe section");
        section.seek(16);
        const std::uint32_t raw_size = section.u32();
        const std::uint32_t raw_at = section.u32();
        top = std::max<std::uint64_t>(top, std::uint64_t{raw_at} + raw_size);
    }
    return top;
}

std::vector<Cabinet> find_cabinets(ByteView exe) {
    std::vector<Cabinet> cabinets;
    std::uint64_t at = end_of_image(exe);
    for (;;) {
        const auto head = io::try_range(exe, at, 12);
        if (!head || !std::equal(kCabMagic.begin(), kCabMagic.end(), head->begin())) break;
        io::ByteReader r(*head, "cabinet");
        r.skip(8);
        const std::uint32_t size = r.u32();
        if (size == 0) break;
        cabinets.push_back({at, size});
        at += size;
    }
    return cabinets;
}

std::vector<std::filesystem::path> extract(const std::filesystem::path& exe, const std::filesystem::path& out_dir, const std::string& prefix) {
    const fs::MappedFile file(exe);
    const ByteView data = file.view();
    fs::make_dirs(out_dir);
    cb::out("Start of data: 0x{:04X}\n", end_of_image(data));
    std::vector<std::filesystem::path> written;
    for (const Cabinet& cabinet : find_cabinets(data)) {
        const std::string name = std::format("{}_{}.cab", prefix, written.size());
        cb::out("Extracting \"{}\"...\n", name);
        // a cabinet that claims more than the file holds is written as far as the file goes
        const std::uint64_t end = std::min<std::uint64_t>(data.size(), cabinet.offset + cabinet.size);
        fs::write_file(out_dir / name, data.subspan(cabinet.offset, end - cabinet.offset));
        written.push_back(out_dir / name);
    }
    return written;
}

cli::Command command() {
    return cli::Command{"xdk_extract",
                        "extract the cabinets of an XDK installer (XDKRecovery.../XDKSetup...exe)",
                        {cli::Positional{"input", "the installer executable"}, cli::Positional{"output", "the folder to write the cabinets to"},
                         cli::Option{"--prefix", "-p", "the prefix of the cabinet file names (default data)", cli::Arg::Value, "PREFIX", "data"}},
                        [](const cli::Args& args) {
                            extract(fs::from_text(args.required("input")), fs::from_text(args.required("output")), args.required("prefix"));
                            return 0;
                        },
                        {},
                        "",
                        ""};
}

}  // namespace cb::formats::xbox::crypto::xdk_extract
