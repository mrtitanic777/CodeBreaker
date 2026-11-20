// cb-unity: alone
// The PlayStation 4 and PlayStation 5 lanes: registered, an x86-64 ELF (and the fake SELF around it) comes back identical (where NASM is), the program's functions run on the
// x86-64 engine with their imports logged by name and their `syscall`s by number, and an edited program's functions are judged against the original's.
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "engine/machine_lanes.hpp"
#include "engine/machine_x64.hpp"
#include "formats/orbis_test_image.hpp"
#include "formats/xboxone_test_image.hpp"
#include "source/x64_writer.hpp"
#include "verify/lanes/image_lane.hpp"

using namespace cb;
using namespace cb::engine;
namespace stdfs = std::filesystem;
namespace ot = cb::formats::orbis_test;
namespace xt = cb::formats::xboxone_test;

namespace {

verify::lanes::State args(std::int64_t a0, std::int64_t a1 = 0) { return {{"a0", a0}, {"a1", a1}, {"a2", 0}, {"a3", 0}, {"a4", 0}, {"a5", 0}, {"fill", 0}}; }

std::int64_t number(const verify::lanes::Effect& e) { return std::get<std::int64_t>(e); }

}  // namespace

CB_TEST("engine.machine_x64.the_playstation_lanes_are_registered_and_take_their_files", [] {
    register_ps4_lane();
    register_ps4_lane();
    register_ps5_lane();
    CB_CHECK(machine_lanes().find("ps4") != nullptr);
    CB_CHECK(machine_lanes().find("ps5") != nullptr);
    fs::ScopedTempDir dir("cb_x64_reg");
    fs::write_file(dir.path() / "tool.elf", ot::elf());
    fs::write_file(dir.path() / "tool-ps5.elf", ot::elf());
    fs::write_file(dir.path() / "eboot.bin", ot::fself(ot::elf()));
    fs::write_file(dir.path() / "linux.elf", ot::elf("libc.so.6"));
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "tool.elf"), "ps4");
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "tool-ps5.elf"), "ps5");
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "eboot.bin"), "ps4");
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "linux.elf"), "");
    CB_CHECK_EQ(ps4_lane_spec().kind, "ps4");
    CB_CHECK_EQ(ps5_lane_spec().kind, "ps5");
});

CB_TEST("engine.machine_x64.functions_run_with_their_imports_and_syscalls_logged", [] {
    fs::ScopedTempDir dir("cb_x64_run");
    const stdfs::path file = dir.path() / "tool.elf";
    fs::write_file(file, ot::elf());
    const verify::lanes::ImageLaneSpec spec = ps4_lane_spec();
    // helper: calls sceKernelUsleep(100) (the stub returns 0), then loads the value 7
    const verify::lanes::Effects helper = spec.effects(file, static_cast<std::uint32_t>(ot::kHelper), args(0));
    CB_CHECK_EQ(std::get<std::string>(helper[0]), "returned");
    CB_CHECK_EQ(number(helper[1]), 7);
    const auto& calls = std::get<std::vector<std::string>>(helper[6]);
    CB_CHECK_EQ(calls.size(), 1u);
    CB_CHECK(calls[0].starts_with("sceKernelUsleep(64,"));
    // _start(5): helper() + 5
    const verify::lanes::Effects start = spec.effects(file, static_cast<std::uint32_t>(ot::kStart), args(5));
    CB_CHECK_EQ(std::get<std::string>(start[0]), "returned");
    CB_CHECK_EQ(number(start[1]), 12);
    // the loop the relocation's pointer names: 4 + 3 + 2 + 1
    CB_CHECK_EQ(number(spec.effects(file, static_cast<std::uint32_t>(ot::kLoop), args(4))[1]), 10);
    // a syscall is logged by number and its name
    const verify::lanes::Effects sys = spec.effects(file, static_cast<std::uint32_t>(ot::kSyscall), args(1, 2));
    const auto& sys_calls = std::get<std::vector<std::string>>(sys[6]);
    CB_CHECK_EQ(sys_calls.size(), 1u);
    CB_CHECK(sys_calls[0].starts_with("syscall write("));
    // an address with nothing mapped ends in a fault, not a crash
    CB_CHECK(std::get<std::string>(spec.effects(file, 0x3F000, args(0))[0]).starts_with("fault"));
});

CB_TEST("engine.machine_x64.an_elf_comes_back_identical_and_an_edit_diverges", [] {
    fs::ScopedTempDir dir("cb_x64_lane");
    const verify::lanes::ImageLane lane(ps4_lane_spec());
    const stdfs::path original = dir.path() / "tool.elf";
    fs::write_file(original, ot::elf());
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane.prepare(original, dir.path() / "work");
    const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
    CB_CHECK(image.written.linked && image.written.same);
    CB_CHECK_EQ(image.written.functions.size(), 3u);
    const verify::lanes::Built built = lane.build(*prepared, original, dir.path() / "work");
    CB_CHECK(built.judge(built.items.front()).verdict == verify::Verdict::Equivalent);

    Bytes edited = ot::elf();
    edited[0x1024 + 1] = 0xC8;   // helper: mov edi, 200
    fs::write_file(dir.path() / "edited.elf", edited);
    for (const std::uint32_t entry : {0x1020u, 0x1000u}) {
        CB_CHECK(lane.judge_function(false, original, original, entry).verdict == verify::Verdict::Equivalent);
        CB_CHECK(lane.judge_function(false, original, dir.path() / "edited.elf", entry).verdict == verify::Verdict::Divergent);
    }
    // a function the edit does not touch still agrees
    CB_CHECK(lane.judge_function(false, original, dir.path() / "edited.elf", 0x1040).verdict == verify::Verdict::Equivalent);
});

CB_TEST("engine.machine_x64.the_xbox_lane_is_registered_and_takes_uwp_executables_and_packages", [] {
    register_xboxone_lane();
    register_xboxone_lane();
    CB_CHECK(machine_lanes().find("xboxone") != nullptr);
    fs::ScopedTempDir dir("cb_x64_xbox_reg");
    fs::write_file(dir.path() / "app.exe", xt::pe());
    fs::write_file(dir.path() / "plain.exe", xt::pe(false));
    fs::write_file(dir.path() / "demo.msixbundle", xt::msixbundle());
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "app.exe"), "xboxone");
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "demo.msixbundle"), "xboxone");
    CB_CHECK_EQ(machine_lanes().name_of_file(dir.path() / "plain.exe"), "");
    CB_CHECK_EQ(xboxone_lane_spec().kind, "xboxone");
});

CB_TEST("engine.machine_x64.xbox_functions_run_with_the_windows_calling_convention_and_imports_logged", [] {
    fs::ScopedTempDir dir("cb_x64_xbox_run");
    const stdfs::path file = dir.path() / "app.exe";
    fs::write_file(file, xt::pe());
    const verify::lanes::ImageLaneSpec spec = xboxone_lane_spec();
    const verify::lanes::Effects entry = spec.effects(file, xt::kEntry, args(0));
    CB_CHECK_EQ(std::get<std::string>(entry[0]), "returned");
    CB_CHECK_EQ(number(entry[1]), 7);
    const auto& calls = std::get<std::vector<std::string>>(entry[6]);
    CB_CHECK_EQ(calls.size(), 1u);
    CB_CHECK(calls[0].starts_with("KERNEL32.dll!Sleep(64,"));   // the first argument is RCX
    // add(3, 4): RCX + RDX
    CB_CHECK_EQ(number(spec.effects(file, xt::kAdd, args(3, 4))[1]), 7);
    CB_CHECK(std::get<std::string>(spec.effects(file, 0x3F000, args(0))[0]).starts_with("fault"));
});

CB_TEST("engine.machine_x64.a_pe_comes_back_identical_and_an_edit_diverges", [] {
    fs::ScopedTempDir dir("cb_x64_xbox_lane");
    const verify::lanes::ImageLane lane(xboxone_lane_spec());
    const stdfs::path original = dir.path() / "app.exe";
    fs::write_file(original, xt::pe());
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane.prepare(original, dir.path() / "work");
    const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
    CB_CHECK(image.written.linked && image.written.same);
    CB_CHECK_EQ(image.written.functions.size(), 2u);

    Bytes edited = xt::pe();
    edited[0x200 + 5] = 0xC8;   // entry: mov ecx, 200
    edited[0x220 + 2] = 0x01;   // add: lea eax, [rcx + rax]
    fs::write_file(dir.path() / "edited.exe", edited);
    CB_CHECK(lane.judge_function(false, original, original, xt::kEntry).verdict == verify::Verdict::Equivalent);
    CB_CHECK(lane.judge_function(false, original, dir.path() / "edited.exe", xt::kEntry).verdict == verify::Verdict::Divergent);
    CB_CHECK(lane.judge_function(false, original, dir.path() / "edited.exe", xt::kAdd).verdict == verify::Verdict::Divergent);
});
