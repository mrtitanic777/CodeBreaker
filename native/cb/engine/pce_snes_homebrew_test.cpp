// The PC Engine and Super Nintendo lanes over real programs: open-source homebrew with published source (docs/TESTED_SOFTWARE.md), kept outside the repository
// in GitHub/homebrew-samples (or where CB_HOMEBREW_SAMPLES says). Each is written as source and built again, the file must come back byte for byte, and its
// functions are judged: each agrees with itself, and with its first byte turned over some function of the first few does something else. A program that
// is not on this machine is skipped.
#include <algorithm>
#include <iostream>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/engine/machine_pce.hpp"
#include "cb/engine/machine_snes.hpp"
#include "cb/formats/pce.hpp"
#include "cb/formats/snes.hpp"
#include "cb/source/snes_writer.hpp"
#include "cb/verify/lanes/image_lane.hpp"

using namespace cb;
namespace stdfs = std::filesystem;

namespace {

std::optional<stdfs::path> sample(std::string_view relative) {
    stdfs::path root = cb::testing::beside_checkout("homebrew-samples");
    if (const auto env = fs::get_env("CB_HOMEBREW_SAMPLES"); env && !env->empty()) root = fs::from_text(*env);
    const stdfs::path file = root / std::string(relative);
    if (!stdfs::is_regular_file(file)) return std::nullopt;
    return file;
}

// A program written, rebuilt and judged; `offset_of` is the file offset of a function's first byte.
template <class OffsetOf>
void judge(const verify::lanes::ImageLane& lane, const stdfs::path& file, std::string_view label, std::size_t most, const OffsetOf& offset_of) {
    fs::ScopedTempDir dir("cb_pce_snes_homebrew");
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane.prepare(file, dir.path() / "work");
    const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
    std::cout << label << ": " << image.written.functions.size() << " functions" << (image.written.same ? ", identical" : ", DIFFERENT") << "\n";
    CB_CHECK_EQ(image.written.notes.empty() ? std::string(label) : image.written.notes.front(), std::string(label));
    CB_CHECK(image.written.linked);
    CB_CHECK(image.written.same);
    CB_CHECK(!image.written.functions.empty());

    const Bytes bytes = fs::read_file(file);
    std::size_t judged = 0, diverged = 0;
    for (const auto& [address, name] : image.written.functions) {
        if (judged == most) break;
        const std::optional<std::size_t> offset = offset_of(address);
        if (!offset || *offset >= bytes.size()) continue;
        CB_CHECK(lane.judge_function(false, file, file, address).verdict == verify::Verdict::Equivalent);
        Bytes edited = bytes;
        edited[*offset] = static_cast<std::uint8_t>(edited[*offset] ^ 0xFF);
        const stdfs::path changed = dir.path() / ("edited" + file.extension().string());
        fs::write_file(changed, edited);
        ++judged;
        if (lane.judge_function(false, file, changed, address).verdict == verify::Verdict::Divergent) ++diverged;
    }
    std::cout << "  judged " << judged << ", " << diverged << " diverged when their first byte was turned over\n";
    CB_CHECK(judged != 0);
    CB_CHECK(diverged != 0);
}

void judge_pce(std::string_view relative) {
    const auto file = sample(relative);
    if (!file) return;   // (not on this machine)
    engine::register_pce_lane();
    const std::size_t header = formats::pce::read(*file).header.size();
    const verify::lanes::ImageLane lane(engine::pce_lane_spec());
    judge(lane, *file, relative, 12, [&](std::uint32_t address) -> std::optional<std::size_t> {
        return header + static_cast<std::size_t>(address >> 16) * formats::pce::kBankSize + (address & 0x1FFF);
    });
}

void judge_snes(std::string_view relative) {
    const auto file = sample(relative);
    if (!file) return;
    engine::register_snes_lane();
    const formats::snes::Rom rom = formats::snes::read(*file);
    const verify::lanes::ImageLane lane(engine::snes_lane_spec());
    judge(lane, *file, relative, 12, [&](std::uint32_t address) -> std::optional<std::size_t> {
        const auto offset = rom.offset(address & source::snes_writer::kAddressMask);
        if (!offset) return std::nullopt;
        return rom.copier_header.size() + *offset;
    });
}

}  // namespace

// Dshadoff's PCE Mouse Test (MIT): a HuCard behind a 512-byte header, written in PCEAS assembly.
CB_TEST("engine.pce.homebrew.mouse_test", [] { judge_pce("pce/dshadoff-pce-mouse-test/pce_mouse.pce"); });

// Langel's pce-dev-kit demos (Unlicense): small HuCards behind a header.
CB_TEST("engine.pce.homebrew.pce_dev_kit", [] {
    for (const char* name : {"BALL", "GAMELIFE", "MML", "SAMPLE1"}) judge_pce(std::string("pce/langel-pce-dev-kit/") + name + ".PCE");
});

// Donth77's PVSnesLib examples (MIT): LoROM cartridges whose code a C compiler wrote.
CB_TEST("engine.snes.homebrew.pvsneslib_examples", [] {
    for (const char* name : {"gothicvania", "hello-world-region"}) judge_snes(std::string("snes/donth77-snes-homebrew/") + name + ".sfc");
});

// OpenSNES's examples (MIT): C programs, LoROM and one HiROM.
CB_TEST("engine.snes.homebrew.opensnes_examples", [] {
    for (const char* name : {"simple_sprite", "controller", "blocks", "breakout", "hirom_demo", "mode7_racing", "rpg", "shmup", "hdma_wave"})
        judge_snes(std::string("snes/opensnes-examples/extracted/opensnes-examples_v0.55.0/examples/bin/") + name + ".sfc");
});

// Every example the OpenSNES release holds (65 cartridges, one of them HiROM, some with coprocessors the lane does not model): each is written as source and
// built again, and must come back byte for byte.
CB_TEST("engine.snes.homebrew.every_opensnes_example_rebuilds", [] {
    const auto any = sample("snes/opensnes-examples/extracted/opensnes-examples_v0.55.0/examples/bin/simple_sprite.sfc");
    if (!any) return;
    std::vector<stdfs::path> files;
    for (const auto& entry : stdfs::directory_iterator(any->parent_path())) {
        if (entry.path().extension() == ".sfc") files.push_back(entry.path());
    }
    std::sort(files.begin(), files.end());
    fs::ScopedTempDir dir("cb_opensnes_all");
    std::size_t identical = 0, functions = 0, code = 0, bytes = 0;
    for (const stdfs::path& file : files) {
        const source::image_writer::Written r = source::snes_writer::reconstruct(file, dir.path() / file.stem());
        if (r.same) ++identical;
        else std::cout << file.filename().string() << ": NOT identical\n";
        CB_CHECK(r.same);
        functions += r.functions.size();
        code += r.code_bytes;
        bytes += r.image_bytes;
    }
    std::cout << identical << " of " << files.size() << " cartridges rebuild identical: " << functions << " functions, " << code << " of " << bytes << " bytes as code\n";
    CB_CHECK_EQ(identical, files.size());
});
