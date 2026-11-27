// cb-unity: alone (file-local names that other files in a batch also use)
// The Nintendo 64 lane over real programs: open-source homebrew with published source (docs/TESTED_SOFTWARE.md), kept outside the repository in
// GitHub/homebrew-samples/n64 (or where CB_HOMEBREW_SAMPLES says). Each is written as source and built again (what comes out must be the original,
// byte for byte), and every function is run in the interpreter, in the original and in the rebuilt ROM, from each state of the lane: the two must
// show the same. A program that is not on this machine is skipped. CB_N64_KEEP names a folder the source is copied to; CB_N64_ALL runs every
// ROM under the folder (the libdragon game too, which is 5.8 MB) instead of the few small ones the unit test takes.
#include <iostream>
#include <map>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "emulate/r4300.hpp"
#include "engine/machine_n64.hpp"
#include "formats/n64.hpp"

namespace cb::engine {
namespace {

std::filesystem::path samples() {
    std::filesystem::path root = cb::testing::beside_checkout("homebrew-samples");
    if (const auto env = fs::get_env("CB_HOMEBREW_SAMPLES"); env && !env->empty()) root = fs::from_text(*env);
    return root / "n64";
}

void judge(const std::filesystem::path& file) {
    const verify::lanes::ImageLaneSpec spec = n64_lane_spec();
    fs::ScopedTempDir dir("cb_n64_homebrew");
    const verify::lanes::Written w = spec.writer(file, dir.path());
    std::size_t same = 0, runs = 0;
    std::map<std::string, std::size_t> ends;
    const std::filesystem::path built = w.built.empty() ? file : w.built;
    for (const auto& [address, name] : w.functions) {
        for (const verify::lanes::State& state : {spec.states[0], spec.states[1]}) {
            const verify::lanes::Effects a = spec.effects(file, address, state), b = spec.effects(built, address, state);
            ++runs;
            same += a == b ? 1 : 0;
            const std::string& end = std::get<std::string>(a[0]);
            ++ends[end.starts_with("stopped") ? end.substr(0, end.find(" at ")) : end];
        }
    }
    // and the program as a whole, from its entry point as the boot code leaves the console (the boot code itself is not run): it runs until it waits
    // for something this model does not do (an interrupt, the RDP) and so spins, and never stops at an instruction or an address it does not know
    const formats::n64::Rom rom = formats::n64::read(file);
    CB_CHECK(rom.checksums_match());   // (the boot code's own algorithm gives the header's CRCs)
    emulate::r4300::Bus bus(rom.memory(), rom.data);
    emulate::r4300::RunOptions whole;
    whole.max_steps = 3000000;
    const emulate::r4300::RunResult entry_run = emulate::r4300::run(bus, rom.load_address(), whole);
    std::cout << file.filename().string() << " from the entry: " << entry_run.exit() << " after " << entry_run.steps << " steps, " << entry_run.io.size() << " register accesses\n";
    CB_CHECK(entry_run.end != emulate::r4300::End::Stopped || (entry_run.fault->kind != emulate::r4300::Fault::Kind::NoInstruction && entry_run.fault->kind != emulate::r4300::Fault::Kind::NotRun));
    std::cout << file.filename().string() << ": " << w.functions.size() << " functions, " << (w.same ? "identical" : "DIFFERENT") << ", " << same << " of " << runs << " runs the same;";
    for (const auto& [end, count] : ends) std::cout << " " << end << " " << count << ",";
    std::cout << "\n";
    if (const auto keep = fs::get_env("CB_N64_KEEP"); keep && !keep->empty()) {   // (look at what was written)
        fs::make_dirs(fs::from_text(*keep));
        fs::write_file(fs::from_text(*keep) / (file.filename().string() + ".s"), fs::read_file(w.source));
    }
    CB_CHECK(w.notes.empty());
    CB_CHECK(w.linked);
    CB_CHECK(w.same);
    CB_CHECK_EQ(same, runs);
}

CB_TEST("engine.machine_n64.homebrew", [] {
    const std::filesystem::path root = samples();
    if (!std::filesystem::is_directory(root)) return;   // (not on this machine)
    if (const auto all = fs::get_env("CB_N64_ALL"); all && !all->empty()) {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(root))
            if (entry.is_regular_file()) judge(entry.path());
        return;
    }
    for (const char* name : {"peterlemon/HelloWorld_16BPP_HelloWorldCPU320x240_HelloWorldCPU16BPP320X240.N64", "peterlemon/CPUTest_CP1_ADD_CP1ADD.N64", "peterlemon/CPUTest_CPU_DMULT_CPUDMULT.N64",
                             "peterlemon/Compress_LZ77_LZ77Decode_LZ77Decode.N64"}) {
        const std::filesystem::path file = root / name;
        if (std::filesystem::is_regular_file(file)) judge(file);
    }
});

// The RSP's and the RDP's programs: in PeterLemon's RSP tests a function starts a microcode task that runs to its BREAK; in his RDP tests a
// function hands the RDP a display list, read as its commands. (Judged as the others are, too.)
CB_TEST("engine.machine_n64.rsp_tasks_run_and_rdp_lists_are_read", [] {
    const std::filesystem::path root = samples();
    if (!std::filesystem::is_directory(root)) return;   // (not on this machine)
    const verify::lanes::ImageLaneSpec spec = n64_lane_spec();
    const auto tasks_and_lists = [&](const std::filesystem::path& file) {
        fs::ScopedTempDir dir("cb_n64_rsp");
        const verify::lanes::Written w = spec.writer(file, dir.path());
        std::size_t broke = 0, commands = 0;
        for (const auto& [address, name] : w.functions) {
            const verify::lanes::Effects e = spec.effects(file, address, spec.states[0]);
            for (const std::string& task : std::get<std::vector<std::string>>(e[10]))
                if (task.ends_with("break")) ++broke;
            commands += std::get<std::vector<std::string>>(e[11]).size();
        }
        std::cout << file.filename().string() << ": " << broke << " RSP tasks to their BREAK, " << commands << " RDP commands\n";
        return std::pair{broke, commands};
    };
    const std::filesystem::path rsp = root / "peterlemon" / "RSP_DCT_FastDCTBlockDecode_RSPFastDCTBlockDecode.N64";
    if (std::filesystem::is_regular_file(rsp)) {
        CB_CHECK(tasks_and_lists(rsp).first > 0);
        judge(rsp);
    }
    const std::filesystem::path rdp = root / "peterlemon" / "RDP_AlphaCompare_AlphaCompare.N64";
    if (std::filesystem::is_regular_file(rdp)) CB_CHECK(tasks_and_lists(rdp).second > 0);
});

}  // namespace
}  // namespace cb::engine
