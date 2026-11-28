// cb-unity: alone
#include "engine/machine_builtin.hpp"

#include <filesystem>
#include <optional>
#include <string_view>
#include <vector>

#include "engine/lane_registry.hpp"
#include "core/error.hpp"
#include "formats/coleco.hpp"
#include "formats/disc_image.hpp"
#include "formats/cpc.hpp"
#include "formats/gb.hpp"
#include "formats/gba.hpp"
#include "formats/genesis.hpp"
#include "formats/msx.hpp"
#include "formats/nes.hpp"
#include "formats/psx.hpp"
#include "formats/read.hpp"
#include "formats/sms.hpp"
#include "formats/zx.hpp"
#include "platforms/gba.hpp"
#include "platforms/package.hpp"
#include "platforms/ps1.hpp"

// What `info` says of the lane's files (formats/detect.hpp), as it was written in formats/detect.cpp.
namespace cb::formats::detect {
namespace {
std::optional<std::vector<Detected>> info_ps1(const DetectInput& in) {
    [[maybe_unused]] const ByteView file = in.file;
    [[maybe_unused]] const ByteView head = in.head;
    [[maybe_unused]] const ByteView h4 = clamped(head, 0, 4);
    [[maybe_unused]] const std::uint64_t size = in.size;
    [[maybe_unused]] const std::string_view name = in.name;
    // (a PlayStation executable is told by config/formats/psx.fmt, before this) a PlayStation disc (raw or cooked sectors): the program its SYSTEM.CNF boots
    if (size >= 17 * disc::kSector && (disc::raw_sync(head) || disc::is_image_name(std::filesystem::path(name)))) {
        try {
            const psx::Exe exe = psx::parse_disc(disc::Sectors::of(file));
            return one("PlayStation disc", std::format("boots {}{}, entry 0x{:08X}, program at 0x{:08X}, {} bytes", exe.boot_file.substr(1),
                                                       exe.boot.empty() ? std::string(" (no SYSTEM.CNF)") : std::string(), exe.header.pc0, exe.header.t_addr,
                                                       exe.data.size() - psx::kHeaderSize),
                       "playstation", "reach");
        } catch (const Error&) {   // another console's disc, or no disc
        }
    }
    return std::nullopt;
}
}  // namespace
}  // namespace cb::formats::detect

// What `info` says of the lane's files (formats/detect.hpp), as it was written in formats/detect.cpp.
namespace cb::formats::detect {
namespace {
}  // namespace
}  // namespace cb::formats::detect

// The iNES magic, which formats/detect.cpp's table of magic numbers used to hold.
namespace cb::formats::detect {
namespace {
}  // namespace
}  // namespace cb::formats::detect

namespace cb::engine {

void register_nes_lane() {
    register_lane({
        .name = "nes",
        .platform = [] { return platforms::described("nes"); },
        .probe = [](const std::filesystem::path& p) { return formats::nes::probe(p); },
        .program_extensions = {".nes"},
    });
}

void register_sms_lane() {
    register_lane({
        .name = "sms",
        .platform = [] { return platforms::described("sms"); },
        .probe = [](const std::filesystem::path& p) { return formats::sms::probe(p); },
        .program_extensions = {".sms", ".gg", ".sg"},
    });
}

void register_gb_lane() {
    register_lane({
        .name = "gb",
        .platform = [] { return platforms::described("gb"); },
        .probe = [](const std::filesystem::path& p) { return formats::gb::probe(p); },
        .program_extensions = {".gb", ".gbc"},
    });
}

void register_genesis_lane() {
    register_lane({
        .name = "genesis",
        .platform = [] { return platforms::described("genesis"); },
        .probe = [](const std::filesystem::path& p) { return formats::genesis::probe(p); },
        .program_extensions = {".md", ".gen", ".smd"},
    });
}

void register_ps1_lane() {
    register_lane({
        .name = "ps1",
        .platform = platforms::ps1,
        .probe = [](const std::filesystem::path& p) { return formats::psx::probe(p); },
        .detectors = {{.priority = 10, .detect = formats::detect::info_ps1}},
    });
}

void register_gba_lane() {
    register_lane({
        .name = "gba",
        .platform = platforms::gba,
        .probe = [](const std::filesystem::path& p) { return formats::gba::probe(p); },
        .program_extensions = {".gba"},
    });
}

void register_zx_lane() {
    register_lane({
        .name = "zx",
        .platform = [] { return platforms::described("zx"); },
        .probe = [](const std::filesystem::path& p) { return formats::zx::probe(p); },
        .program_extensions = {".sna", ".z80", ".tap"},
    });
}

void register_msx_lane() {
    register_lane({
        .name = "msx",
        .platform = [] { return platforms::described("msx"); },
        .probe = [](const std::filesystem::path& p) { return formats::msx::probe(p); },
    });
}

void register_coleco_lane() {
    register_lane({
        .name = "coleco",
        .platform = [] { return platforms::described("coleco"); },
        .probe = [](const std::filesystem::path& p) { return formats::coleco::probe(p); },
    });
}

void register_cpc_lane() {
    register_lane({
        .name = "cpc",
        .platform = [] { return platforms::described("cpc"); },
        .probe = [](const std::filesystem::path& p) { return formats::cpc::probe(p); },
    });
}

}  // namespace cb::engine
