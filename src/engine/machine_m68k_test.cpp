// The Neo Geo and Sega CD lanes: registered in the machine lane table, a program comes back identical, and an edited program's functions are run beside
// the original's in the 68000 over the machine's memory, device accesses compared. The programs are written byte by byte here.
#include "engine/machine_m68k.hpp"

#include <algorithm>
#include <cstring>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "engine/machine_lanes.hpp"
#include "formats/segacd_test_image.hpp"
#include "verify/lanes/image_lane.hpp"

using namespace cb;
using namespace cb::engine;
namespace stdfs = std::filesystem;
namespace img = cb::formats::segacd_test_image;

namespace {

void put_long(Bytes& b, std::size_t at, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) b[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (24 - 8 * i));
}

void put_bytes(Bytes& b, std::size_t at, std::initializer_list<std::uint8_t> bytes) { std::copy(bytes.begin(), bytes.end(), b.begin() + static_cast<std::ptrdiff_t>(at)); }

// A Neo Geo P-ROM: callbacks to $400 (JSR ($C00450).L ; MOVE.W #7,($3C0000).L ; RTS), $410 (NOP ; RTS), $420 (BSR $400 ; RTS), and the second
// to $600, which writes two words of video RAM through the LSPC's ports:
//   MOVE.W #$10,($3C0000).L ; MOVE.W #1,($3C0004).L ; MOVE.W #$ABCD,($3C0002).L ; MOVE.W #$1234,($3C0002).L ; RTS
Bytes neogeo_program() {
    Bytes b(0x1000, 0xFF);
    std::fill(b.begin(), b.begin() + 0x200, std::uint8_t{0});
    put_long(b, 0, 0x0010F300);
    put_long(b, 4, 0x00C00402);
    std::memcpy(b.data() + 0x100, "NEO-GEO", 7);
    put_bytes(b, 0x122, {0x4E, 0xF9, 0, 0, 0x04, 0x00});
    put_bytes(b, 0x128, {0x4E, 0xF9, 0, 0, 0x06, 0x00});
    put_bytes(b, 0x12E, {0x4E, 0xF9, 0, 0, 0x04, 0x20});
    put_bytes(b, 0x400, {0x4E, 0xB9, 0x00, 0xC0, 0x04, 0x50, 0x33, 0xFC, 0x00, 0x07, 0x00, 0x3C, 0x00, 0x00, 0x4E, 0x75});
    put_bytes(b, 0x420, {0x61, 0x00, 0xFF, 0xDE, 0x4E, 0x75});
    put_bytes(b, 0x600, {0x33, 0xFC, 0x00, 0x10, 0x00, 0x3C, 0x00, 0x00, 0x33, 0xFC, 0x00, 0x01, 0x00, 0x3C, 0x00, 0x04, 0x33, 0xFC, 0xAB, 0xCD, 0x00, 0x3C, 0x00, 0x02,
                       0x33, 0xFC, 0x12, 0x34, 0x00, 0x3C, 0x00, 0x02, 0x4E, 0x75});
    return b;
}

const std::vector<std::string>& io_of(const verify::lanes::Effects& e) { return std::get<std::vector<std::string>>(e.back()); }

bool has_line(const std::vector<std::string>& io, std::string_view line) { return std::find(io.begin(), io.end(), line) != io.end(); }

}  // namespace

CB_TEST("engine.machine_m68k.are_registered_once_and_take_their_files", [] {
    register_m68k_lanes();
    register_m68k_lanes();   // (twice is fine)
    CB_CHECK(machine_lanes().find("neogeo") != nullptr);
    CB_CHECK(machine_lanes().find("segacd") != nullptr);
    fs::ScopedTempDir dir("cb_m68k_lanes");
    fs::write_file(dir.path() / "game-p1.p1", neogeo_program());
    fs::write_file(dir.path() / "game.iso", img::system_area());
    fs::write_file(dir.path() / "game.bin", img::raw(img::system_area()));
    fs::write_file(dir.path() / "other.bin", Bytes(0x400, 0x11));
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "game-p1.p1"), "neogeo");
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "game.iso"), "segacd");
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "game.bin"), "segacd");
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "other.bin"), "");
    CB_CHECK_EQ(neogeo_lane_spec().kind, "neogeo");
    CB_CHECK_EQ(segacd_lane_spec().kind, "segacd");
    CB_CHECK_EQ(neogeo_lane_spec().key_digits, 6u);
});

CB_TEST("engine.machine_m68k.neogeo_a_prom_comes_back_identical_and_an_edit_diverges", [] {
    fs::ScopedTempDir dir("cb_ng_lane");
    const verify::lanes::ImageLane lane(neogeo_lane_spec());
    const stdfs::path original = dir.path() / "game-p1.p1";
    fs::write_file(original, neogeo_program());
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane.prepare(original, dir.path() / "work");
    const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
    CB_CHECK(image.written.linked && image.written.same);
    CB_CHECK_EQ(image.written.functions.size(), 3u);   // user ($400), player_start ($600), demo_end ($420: it calls $400)
    const verify::lanes::Built built = lane.build(*prepared, original, dir.path() / "work");
    CB_CHECK_EQ(built.items.size(), 3u);
    CB_CHECK(built.judge(built.items.front()).verdict == verify::Verdict::Equivalent);

    CB_CHECK(lane.judge_function(false, original, original, 0x400).verdict == verify::Verdict::Equivalent);
    // an LSPC write of another value
    Bytes edited = neogeo_program();
    edited[0x409] = 0x08;   // MOVE.W #8,(REG_VRAMADDR).L
    fs::write_file(dir.path() / "edited-p1.p1", edited);
    const verify::Judgement j = lane.judge_function(false, original, dir.path() / "edited-p1.p1", 0x400);
    CB_CHECK(j.verdict == verify::Verdict::Divergent);
    CB_CHECK(!j.reason.empty());
    CB_CHECK(lane.judge_function(false, original, dir.path() / "edited-p1.p1", 0x420).verdict == verify::Verdict::Divergent);   // (it calls $400)
    CB_CHECK(lane.judge_function(false, original, dir.path() / "edited-p1.p1", 0x600).verdict == verify::Verdict::Equivalent);   // the one nobody edited
    // another BIOS call: CREDIT_DOWN instead of CREDIT_CHECK
    Bytes other_call = neogeo_program();
    other_call[0x405] = 0x56;
    fs::write_file(dir.path() / "call-p1.p1", other_call);
    CB_CHECK(lane.judge_function(false, original, dir.path() / "call-p1.p1", 0x400).verdict == verify::Verdict::Divergent);
});

CB_TEST("engine.machine_m68k.neogeo_a_bios_call_is_a_logged_write_and_the_lspc_has_a_video_ram", [] {
    fs::ScopedTempDir dir("cb_ng_effects");
    fs::write_file(dir.path() / "game-p1.p1", neogeo_program());
    const verify::lanes::ImageLaneSpec spec = neogeo_lane_spec();
    const verify::lanes::State& state = spec.states[1];   // D0 = 1
    const verify::lanes::Effects user = spec.effects(dir.path() / "game-p1.p1", 0x400, state);
    CB_CHECK_EQ(user.size(), spec.what.size());
    CB_CHECK_EQ(std::get<std::string>(user[0]), "return");
    // CREDIT_CHECK ($C00450) is the stub at slot $50: a write of D0.W to $FFE050; then the LSPC's address register gets 7
    const std::vector<std::string>& io = io_of(user);
    CB_CHECK(has_line(io, "w2 ffe050 1"));
    CB_CHECK(has_line(io, "w2 3c0000 7"));
    // the video RAM: address $10, modulo 1, two words written
    const verify::lanes::Effects vram = spec.effects(dir.path() / "game-p1.p1", 0x600, state);
    const Bytes& video = std::get<Bytes>(vram[6]);
    CB_CHECK_EQ(video.size(), 0x8800u * 2);
    CB_CHECK_EQ(video[0x20], 0xAB);
    CB_CHECK_EQ(video[0x21], 0xCD);
    CB_CHECK_EQ(video[0x22], 0x12);
    CB_CHECK_EQ(video[0x23], 0x34);
    CB_CHECK_EQ(video[0x24], 0);
    // a run with the same state is the same
    CB_CHECK(spec.effects(dir.path() / "game-p1.p1", 0x600, state) == vram);
});

CB_TEST("engine.machine_m68k.segacd_a_disc_comes_back_identical_and_edits_of_either_cpu_diverge", [] {
    fs::ScopedTempDir dir("cb_scd_lane");
    const verify::lanes::ImageLane lane(segacd_lane_spec());
    const stdfs::path original = dir.path() / "game.iso";
    fs::write_file(original, img::system_area());
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane.prepare(original, dir.path() / "work");
    const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
    CB_CHECK(image.written.linked && image.written.same);
    CB_CHECK_EQ(image.written.functions.size(), 6u);
    const verify::lanes::Built built = lane.build(*prepared, original, dir.path() / "work");
    CB_CHECK_EQ(built.items.size(), 6u);
    CB_CHECK(built.judge(built.items.front()).verdict == verify::Verdict::Equivalent);

    CB_CHECK(lane.judge_function(false, original, original, 0xFF0600).verdict == verify::Verdict::Equivalent);
    CB_CHECK(lane.judge_function(false, original, original, 0x6028).verdict == verify::Verdict::Equivalent);
    // the main CPU's IP gives word RAM to the sub CPU another way
    Bytes ip = img::system_area();
    ip[0x803] = 0x02;   // MOVE.B #2,(GA_MEMMODE_LO).L
    fs::write_file(dir.path() / "ip.iso", ip);
    CB_CHECK(lane.judge_function(false, original, dir.path() / "ip.iso", 0xFF0600).verdict == verify::Verdict::Divergent);
    CB_CHECK(lane.judge_function(false, original, dir.path() / "ip.iso", 0xFF0000).verdict == verify::Verdict::Divergent);   // (the security code jumps to it)
    CB_CHECK(lane.judge_function(false, original, dir.path() / "ip.iso", 0x6028).verdict == verify::Verdict::Equivalent);
    // the sub CPU's init asks the CD BIOS for another function
    Bytes sp = img::system_area();
    sp[0x1029] = 0x02;   // MOVEQ #2,D0
    fs::write_file(dir.path() / "sp.iso", sp);
    const verify::Judgement j = lane.judge_function(false, original, dir.path() / "sp.iso", 0x6028);
    CB_CHECK(j.verdict == verify::Verdict::Divergent);
    CB_CHECK(!j.reason.empty());
    CB_CHECK(lane.judge_function(false, original, dir.path() / "sp.iso", 0x6034).verdict == verify::Verdict::Equivalent);
});

CB_TEST("engine.machine_m68k.segacd_the_cd_bios_calls_of_the_sub_cpu_are_logged_and_the_gate_array_answers", [] {
    fs::ScopedTempDir dir("cb_scd_effects");
    fs::write_file(dir.path() / "game.iso", img::system_area());
    const verify::lanes::ImageLaneSpec spec = segacd_lane_spec();
    const verify::lanes::State& state = spec.states[0];
    // the SP's init: MOVEQ #1,D0 ; JSR ($5F22).L -- _CDBIOS is the stub at slot $22: a write of D0.W to $FFE022
    const verify::lanes::Effects init = spec.effects(dir.path() / "game.iso", 0x6028, state);
    CB_CHECK_EQ(init.size(), spec.what.size());
    CB_CHECK_EQ(std::get<std::string>(init[0]), "return");
    CB_CHECK(has_line(io_of(init), "w2 ffe022 1"));
    const Bytes& d_registers = std::get<Bytes>(init[1]);
    CB_CHECK_EQ(d_registers[3], 1);   // D0 = 1
    // the main CPU's IP: the memory mode byte is written, the handshake with the sub CPU's flag ends (it alternates), and the program jumps into word RAM,
    // which holds nothing
    const verify::lanes::Effects ip = spec.effects(dir.path() / "game.iso", 0xFF0600, state);
    CB_CHECK(has_line(io_of(ip), "w1 a12003 1"));
    CB_CHECK(has_line(io_of(ip), "r1 a1200f 0"));
    CB_CHECK(std::get<std::string>(ip[0]) != "return");   // (zeros in word RAM run as ORI.B until the step limit)
    // the same function twice is the same
    CB_CHECK(spec.effects(dir.path() / "game.iso", 0xFF0600, state) == ip);
});
