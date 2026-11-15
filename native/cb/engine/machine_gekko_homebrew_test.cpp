// cb-unity: alone (file-local names that other files in a batch also use)
// The GameCube, Wii and Wii U lanes over real programs: open-source homebrew with published source (docs/TESTED_SOFTWARE.md), kept outside the
// repository in GitHub/homebrew-samples/{gamecube,wii,wiiu} (or where CB_HOMEBREW_SAMPLES says). Each is written as source and built again (what
// comes out must be the original, byte for byte), and its functions are run in the interpreter, in the original and in the rebuilt program, from
// two states of the lane: the two must show the same. A program that is not on this machine is skipped. CB_GEKKO_KEEP names a folder the source is
// copied to; CB_GEKKO_ALL runs every program under the folders (the large ones too) instead of the few small ones the unit test takes.
#include <format>
#include <iostream>
#include <map>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/engine/machine_gekko.hpp"
#include "cb/decompile/ppc.hpp"
#include "cb/emulate/powerpc.hpp"
#include "cb/formats/ppc_program.hpp"

namespace cb::engine {
namespace {

std::filesystem::path samples() {
    std::filesystem::path root = cb::testing::beside_checkout("homebrew-samples");
    if (const auto env = fs::get_env("CB_HOMEBREW_SAMPLES"); env && !env->empty()) root = fs::from_text(*env);
    return root;
}

constexpr std::size_t kMaxFunctions = 60;   // the functions of a program that are run (the first ones, by address)


// Which instructions stop runs as unsupported (CB_GEKKO_UNSUPPORTED=1): the first word each stopped run could not execute, by mnemonic.
void unsupported_report(const std::filesystem::path& file, const verify::lanes::ImageLaneSpec& spec, const std::map<std::uint32_t, std::string>& functions) {
    const auto program = formats::ppc_program::read(file);
    if (!program) return;
    std::vector<emulate::powerpc::Region> regions;
    for (const auto& piece : program->loaded()) regions.push_back({piece.address, Bytes(piece.bytes.begin(), piece.bytes.end())});
    const emulate::powerpc::PageImage image = emulate::powerpc::PageImage::of(regions);
    std::map<std::string, std::size_t> seen;
    for (const auto& [address, name] : functions) {
        emulate::powerpc::RunOptions options;
        options.paired_singles = true;
        options.shared = image;
        const auto run = emulate::powerpc::run({}, address, {}, options);
        if (run.exit_text() != "exception:unsupported") continue;
        const std::uint32_t w = static_cast<std::uint32_t>(run.machine->read(run.pc, 4));
        const auto insn = decompile::ppc::decode_word(w, run.pc);
        ++seen[insn ? insn->mnemonic : std::format("?{:08x}", w)];
    }
    (void)spec;
    for (const auto& [m, n] : seen) std::cout << "  unsupported " << m << " x" << n << std::endl;
}

void judge(const std::string& console, const std::filesystem::path& file) {
    const verify::lanes::ImageLaneSpec spec = gekko_lane_spec(console);
    CB_CHECK(formats::ppc_program::probe(file, console == "gamecube" ? formats::ppc_program::Console::GameCube : console == "wii" ? formats::ppc_program::Console::Wii : formats::ppc_program::Console::WiiU));
    fs::ScopedTempDir dir("cb_gekko_homebrew");
    const verify::lanes::Written w = spec.writer(file, dir.path());
    std::size_t same = 0, runs = 0, taken = 0;
    std::map<std::string, std::size_t> ends;
    const std::filesystem::path built = w.built.empty() ? file : w.built;
    for (const auto& [address, name] : w.functions) {
        if (taken++ == kMaxFunctions) break;
        for (const verify::lanes::State& state : {spec.states[0], spec.states[1]}) {
            const verify::lanes::Effects a = spec.effects(file, address, state), b = spec.effects(built, address, state);
            ++runs;
            same += a == b ? 1 : 0;
            ++ends[std::get<std::string>(a[0])];
        }
    }
    std::cout << console << " " << file.filename().string() << " (read as " << [&] { const auto p = formats::ppc_program::read(file); return p ? std::string(formats::ppc_program::console_text(p->console)) : std::string("nothing"); }() << ")" << ": " << w.functions.size() << " functions, " << (w.same ? "identical" : "DIFFERENT") << ", " << same << " of " << runs << " runs the same;";
    for (const auto& [end, count] : ends) std::cout << " " << end << " " << count << ",";
    std::cout << "\n";
    if (const auto u = fs::get_env("CB_GEKKO_UNSUPPORTED"); u && !u->empty()) unsupported_report(file, spec, w.functions);
    if (const auto keep = fs::get_env("CB_GEKKO_KEEP"); keep && !keep->empty()) {   // (look at what was written)
        fs::make_dirs(fs::from_text(*keep));
        fs::write_file(fs::from_text(*keep) / (file.filename().string() + ".s"), fs::read_file(w.source));
    }
    CB_CHECK(w.notes.empty());
    CB_CHECK(w.linked);
    CB_CHECK(w.same);
    CB_CHECK(!w.functions.empty());
    CB_CHECK_EQ(same, runs);
}

void judge_console(const std::string& console, std::initializer_list<const char*> small) {
    const std::filesystem::path root = samples() / console;
    if (!std::filesystem::is_directory(root)) return;   // (not on this machine)
    if (const auto all = fs::get_env("CB_GEKKO_ALL"); all && !all->empty()) {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(root)) {
            if (!entry.is_regular_file()) continue;
            const std::string ext = fs::to_text(entry.path().extension());
            if (ext == ".dol" || ext == ".elf" || ext == ".rpx" || ext == ".wps" || ext == ".rpl") judge(console, entry.path());
        }
        return;
    }
    for (const char* name : small) {
        const std::filesystem::path file = root / name;
        if (std::filesystem::is_regular_file(file)) judge(console, file);
    }
}

CB_TEST("engine.machine_gekko.homebrew_gamecube", [] { judge_console("gamecube", {"Wii-donut.c-gc.dol", "beanz_gc_monitor.dol", "beanz_gc_monitor.elf", "sudokul-gamecube/SuDokuL-gamecube/SuDokuL/SuDokuL.dol"}); });

CB_TEST("engine.machine_gekko.homebrew_wii", [] { judge_console("wii", {"wii-donut/apps/Wii-donut/boot.dol", "doodle-wii.dol"}); });

CB_TEST("engine.machine_gekko.homebrew_wiiu", [] { judge_console("wiiu", {"gamepadtopro.wps", "MultiDRCSpaceDemo.rpx"}); });

}  // namespace
}  // namespace cb::engine
