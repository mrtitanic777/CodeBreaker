// The Neo Geo and Sega CD writers over real programs: open-source homebrew with published source (docs/TESTED_SOFTWARE.md), kept outside the repository in
// GitHub/homebrew-samples (or where CB_HOMEBREW_SAMPLES says). Each is written as source and built again; what comes out must be the original program,
// byte for byte. A program that is not on this machine is skipped.

#include <iostream>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "engine/machine_m68k.hpp"
#include "formats/neogeo.hpp"
#include "formats/segacd.hpp"
#include "source/neogeo_writer.hpp"
#include "source/segacd_writer.hpp"
#include "verify/lanes/image_lane.hpp"

namespace cb::source {
namespace {

std::optional<std::filesystem::path> sample(std::string_view relative) {
    std::filesystem::path root = cb::testing::beside_checkout("homebrew-samples");
    if (const auto env = fs::get_env("CB_HOMEBREW_SAMPLES"); env && !env->empty()) root = fs::from_text(*env);
    const std::filesystem::path file = root / std::string(relative);
    if (!std::filesystem::is_regular_file(file)) return std::nullopt;
    return file;
}

template <class Writer>
void rebuild(std::string_view relative, std::size_t functions_at_least, Writer&& writer) {
    const auto file = sample(relative);
    if (!file) return;   // (not on this machine)
    fs::ScopedTempDir dir("cb_m68k_homebrew");
    const image_writer::Written r = writer(*file, dir.path());
    std::cout << relative << ": " << r.functions.size() << " functions, " << r.code_bytes << " code bytes of " << r.image_bytes << (r.same ? ", identical" : ", DIFFERENT") << "\n";
    if (const auto keep = fs::get_env("CB_M68K_KEEP"); keep && !keep->empty()) {   // (look at what was written)
        fs::make_dirs(fs::from_text(*keep));
        fs::write_file(fs::from_text(*keep) / (std::filesystem::path(std::string(relative)).filename().string() + ".asm"), fs::read_file(r.source));
    }
    CB_CHECK_EQ(r.notes.empty() ? std::string(relative) : r.notes.front(), std::string(relative));   // (why it did not rebuild, when it did not)
    CB_CHECK(r.linked);
    CB_CHECK(r.same);
    CB_CHECK(r.functions.size() >= functions_at_least);
}

// Neo Geo P-ROMs: Refrain's (word-swapped, built with GCC, 1 MB) and the DATlib demo's (68000 in the order the CPU sees it).
CB_TEST("source.m68k_consoles.homebrew.neo_geo", [] {
    rebuild("neogeo/refrain-202-p1.p1", 12, neogeo_writer::reconstruct);
    rebuild("neogeo/datdemo-dev_p1.rom", 10, neogeo_writer::reconstruct);
});

// The Z80 sound program of the Neo Geo: Mezz-Estate's audio driver (MIT), 24 KB of Z80 written for the M-ROM.
CB_TEST("source.m68k_consoles.homebrew.neo_geo_sound", [] { rebuild("neogeo/mezz-estate-driver.m1", 3, neogeo_writer::reconstruct_sound); });

// Sega CD: ProjectCD's minimal disc (an IP that starts the sub CPU's program, and the SP) and the boot sectors it was built from.
CB_TEST("source.m68k_consoles.homebrew.sega_cd", [] {
    rebuild("segacd/minimalcd.iso", 5, segacd_writer::reconstruct);
    rebuild("segacd/projectcd-bootsect.bin", 8, segacd_writer::reconstruct);
});

// The lanes judge the real programs' functions: each is run in the 68000 over the machine's memory from every starting state and agrees with itself
// (the run is deterministic, the BIOS stubbed, the devices answering as an idle machine); a copy whose function begins with MOVEQ #$55,D0 does not.
// `patch` gives the program with the first word at `entry` replaced.
template <class Spec, class Patch>
void judge(std::string_view relative, Spec&& spec, Patch&& patch) {
    const auto file = sample(relative);
    if (!file) return;
    fs::ScopedTempDir dir("cb_m68k_judge");
    const verify::lanes::ImageLane lane(spec());
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane.prepare(*file, dir.path() / "work");
    const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
    CB_CHECK(image.written.same);
    std::size_t returned = 0, diverged = 0;
    for (const auto& [entry, name] : image.written.functions) {
        CB_CHECK(lane.judge_function(false, *file, *file, entry).verdict == verify::Verdict::Equivalent);
        const std::filesystem::path edited = dir.path() / "edited.bin";
        patch(*file, entry, edited);
        if (lane.judge_function(false, *file, edited, entry).verdict == verify::Verdict::Divergent) ++diverged;
        const verify::lanes::Effects shown = spec().effects(*file, entry, spec().states[1]);
        if (std::get<std::string>(shown[0]) == "return") ++returned;
    }
    std::cout << relative << ": " << image.written.functions.size() << " functions judged, " << returned << " return, " << diverged << " diverge when edited" << std::endl;
    CB_CHECK_EQ(diverged, image.written.functions.size());
}

CB_TEST("source.m68k_consoles.homebrew.the_lanes_judge_the_functions", [] {
    const auto neo_patch = [](const std::filesystem::path& file, std::uint32_t entry, const std::filesystem::path& out) {
        formats::neogeo::Rom rom = formats::neogeo::read(file);
        rom.data[entry] = 0x70;
        rom.data[entry + 1] = 0x55;
        fs::write_file(out, rom.data);
    };
    judge("neogeo/refrain-202-p1.p1", engine::neogeo_lane_spec, neo_patch);
    judge("neogeo/datdemo-dev_p1.rom", engine::neogeo_lane_spec, neo_patch);
    const auto cd_patch = [](const std::filesystem::path& file, std::uint32_t entry, const std::filesystem::path& out) {
        const formats::segacd::Disc disc = formats::segacd::read(file);
        Bytes boot = disc.boot;
        const std::uint32_t at = formats::segacd::Disc::is_sub(entry) ? disc.sub_begin + (entry - formats::segacd::kSubBase) : disc.main_begin + (entry - formats::segacd::kMainBase);
        boot[at] = 0x70;
        boot[at + 1] = 0x55;
        fs::write_file(out, boot);
    };
    judge("segacd/minimalcd.iso", engine::segacd_lane_spec, cd_patch);
    judge("segacd/projectcd-bootsect.bin", engine::segacd_lane_spec, cd_patch);
});

}  // namespace
}  // namespace cb::source
