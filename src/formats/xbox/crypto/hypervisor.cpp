#include "formats/xbox/crypto/hypervisor.hpp"

#include <algorithm>
#include <format>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/io.hpp"
#include "core/log.hpp"
#include "formats/xbox/crypto/tool_support.hpp"

namespace cb::formats::xbox::crypto::hypervisor {

namespace {

constexpr std::size_t kKeysAt = 0x10100;
constexpr std::size_t kSecurityHashAt = 0x100F0;
constexpr std::size_t kRangesAt = 0x10878;

// A region of the image to take from the original: where it is in the original and where it goes in the dumped copy (the same place,
// except the boot info, which the original keeps at 0x08 and the dumped copy at 0x88).
struct CleanRegion {
    std::size_t source;
    std::size_t destination;
    std::size_t size;
};
constexpr CleanRegion kCleanRegions[] = {
    {0x08, 0x88, 0x0C},         // boot info
    {0x34, 0x34, 0x0C},         // version info
    {0x70, 0x70, 0x04},         // flags
    {0x78, 0x78, 0xFF88},       // main code, part 1
    {0x10008, 0x10008, 0x18},   // security data
    {0x10130, 0x10130, 0x6190}, // code, part 2
    {0x11018, 0x11018, 0x52BB}, // code, part 3
    {0x16F00, 0x16F00, 0x29100} // code and data, part 4
};

template <std::size_t N>
void copy_into(std::array<std::uint8_t, N>& out, ByteView image, std::size_t at) {
    const ByteView part = io::checked_range(image, at, N);
    std::copy(part.begin(), part.end(), out.begin());
}

}  // namespace

Info info(ByteView image) {
    if (image.size() < kSize) throw FormatError("hypervisor", image.size(), std::format("too small ({} bytes, the image is 0x{:X})", image.size(), kSize));
    Info out;
    out.size = image.size();
    io::ByteReader r(image, "hypervisor");
    r.seek(2);
    out.build = r.u16_be();
    out.qfe = r.u16_be();
    copy_into(out.white_key, image, kKeysAt);
    copy_into(out.aes_key, image, kKeysAt + 0x10);
    copy_into(out.hash_key, image, kKeysAt + 0x20);
    const auto is_55 = [](const auto& key) { return std::all_of(key.begin(), key.end(), [](std::uint8_t b) { return b == 0x55; }); };
    out.keys_are_55 = is_55(out.white_key) && is_55(out.aes_key) && is_55(out.hash_key);
    copy_into(out.security_hash, image, kSecurityHashAt);
    r.seek(kRangesAt);
    for (Range& range : out.hash_ranges) {
        range.address = r.u32_be();
        range.size = r.u32_be();
    }
    return out;
}

Bytes clean(ByteView dumped, ByteView original) {
    Bytes cleaned(dumped.begin(), dumped.begin() + static_cast<std::ptrdiff_t>(std::min(dumped.size(), kSize)));
    for (const CleanRegion& region : kCleanRegions) {
        if (region.source + region.size <= original.size() && region.destination + region.size <= cleaned.size())
            std::copy_n(original.begin() + static_cast<std::ptrdiff_t>(region.source), region.size, cleaned.begin() + static_cast<std::ptrdiff_t>(region.destination));
    }
    return cleaned;
}

cli::Command command() {
    cli::Command info_command{
        "info",
        "build, QFE, memory keys and hash ranges of a hypervisor image",
        {cli::Positional{"hypervisor", "the 256 KB hypervisor image"}},
        [](const cli::Args& args) {
            const Info i = info(fs::read_file(fs::from_text(args.required("hypervisor"))));
            const auto row = [](std::string_view label, const std::string& value) { cb::out("  {:<20} {}\n", label, value); };
            row("size", std::to_string(i.size));
            row("build", std::to_string(i.build));
            row("qfe", std::to_string(i.qfe));
            row("white_key", support::hex_upper(i.white_key));
            row("aes_key", support::hex_upper(i.aes_key));
            row("hash_key", support::hex_upper(i.hash_key));
            row("keys_are_55", i.keys_are_55 ? "yes" : "no");
            row("security_hash_100f0", support::hex_upper(i.security_hash));
            for (std::size_t n = 0; n < i.hash_ranges.size(); ++n)
                row(std::format("hash_range_{}", n), std::format("0x{:08X} 0x{:08X}", i.hash_ranges[n].address, i.hash_ranges[n].size));
            return 0;
        },
        {}, "", ""};
    cli::Command clean_command{
        "clean",
        "take the RGH patches out of a dumped hypervisor, using the original",
        {cli::Positional{"dumped", "the dumped hypervisor"}, cli::Positional{"base", "the original (offline) hypervisor"},
         cli::Positional{"output", "the file to write"}},
        [](const cli::Args& args) {
            const Bytes out = clean(fs::read_file(fs::from_text(args.required("dumped"))), fs::read_file(fs::from_text(args.required("base"))));
            fs::write_file(fs::from_text(args.required("output")), out);
            cb::out("cleaned HV ({} bytes) -> {}\n", out.size(), args.required("output"));
            return 0;
        },
        {}, "", ""};
    return cli::Command{"hypervisor", "analyse an Xbox 360 hypervisor image: info, and clean patches out against an original", {}, {},
                        {std::move(info_command), std::move(clean_command)}, "", ""};
}

}  // namespace cb::formats::xbox::crypto::hypervisor
