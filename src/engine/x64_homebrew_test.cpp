// cb-unity: alone
// The PlayStation 4 and 5 lanes over real programs: open-source homebrew (docs/TESTED_SOFTWARE.md), kept outside the repository in GitHub/homebrew-samples (or where
// CB_HOMEBREW_SAMPLES says). Each is written as source and built again, the file must come back byte for byte (the ELF a fake SELF holds, for a SELF), and its functions are
// judged: each agrees with itself, and with its first byte turned over some function does something else. A program that is not on this machine is skipped.
// Nothing here runs a program on the console or the host: the lane's functions run in an emulator over memory the lane lays out.
#include <algorithm>
#include <iostream>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "engine/machine_lanes.hpp"
#include "engine/machine_x64.hpp"
#include "formats/orbis.hpp"
#include "formats/xboxone.hpp"
#include "source/x64_writer.hpp"
#include "verify/lanes/image_lane.hpp"

using namespace cb;
namespace stdfs = std::filesystem;

namespace {

stdfs::path sample(std::string_view relative) {
    stdfs::path root = cb::testing::beside_checkout("homebrew-samples");
    if (const auto env = fs::get_env("CB_HOMEBREW_SAMPLES"); env && !env->empty()) root = fs::from_text(*env);
    return root / std::string(relative);
}

// Where the function's first bytes are in the file the lane reads (a SELF holds the segments elsewhere than the ELF it was made of): found by the bytes themselves.
std::optional<std::size_t> file_offset_of(const Bytes& file, ByteView pattern) {
    const auto it = std::search(file.begin(), file.end(), pattern.begin(), pattern.end());
    return it == file.end() ? std::nullopt : std::optional<std::size_t>(static_cast<std::size_t>(it - file.begin()));
}

// The lane named `kind` over the program at `file`: identical rebuild, functions judged against themselves, and with a byte turned over, some function differs.
void judge(const char* kind, const stdfs::path& file, const std::function<ByteView(std::uint32_t, std::size_t)>& bytes_at, std::size_t most) {
    if (!stdfs::is_regular_file(file)) return;   // (not on this machine)
    engine::register_ps4_lane();
    engine::register_ps5_lane();
    engine::register_xboxone_lane();
    CB_CHECK_EQ(engine::machine_lanes().name_of_file(file), kind);
    const std::shared_ptr<const engine::MachineLane> machine = engine::machine_lanes().find(kind);
    const verify::lanes::ImageLane lane(machine->make_spec());
    fs::ScopedTempDir dir("cb_x64_homebrew");
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane.prepare(file, dir.path() / "work");
    const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
    std::cout << file.filename().string() << ": " << image.written.functions.size() << " functions" << (image.written.same ? ", identical" : ", DIFFERENT") << "\n";
    for (const std::string& note : image.written.notes) std::cout << "  note: " << note << "\n";
    CB_CHECK(image.written.linked);
    CB_CHECK(image.written.same);
    CB_CHECK(!image.written.functions.empty());

    const Bytes bytes = fs::read_file(file);
    std::size_t judged = 0, diverged = 0;
    for (const auto& [address, name] : image.written.functions) {
        if (judged == most) break;
        const ByteView head = bytes_at(address, 16);
        if (head.size() < 16) continue;
        const std::optional<std::size_t> offset = file_offset_of(bytes, head);
        if (!offset) continue;
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

void judge_orbis(const char* kind, std::string_view relative, std::size_t most = 10) {
    const stdfs::path file = sample(relative);
    if (!stdfs::is_regular_file(file)) return;
    const formats::orbis::Program program = formats::orbis::read(file);
    judge(kind, file, [&](std::uint32_t address, std::size_t n) -> ByteView {
        const auto o = program.offset_of(address);
        return o ? ByteView(program.file).subspan(*o, std::min<std::size_t>(n, program.file.size() - *o)) : ByteView();
    }, most);
}

// A package: its PE32+ is written out as a loose file (edited copies cannot be made inside a compressed package) and judged there; the package itself must come back too.
void judge_xboxone(std::string_view relative, const std::string& member, std::size_t most) {
    const stdfs::path package = sample(relative);
    if (!stdfs::is_regular_file(package)) return;
    const formats::xboxone::Program program = formats::xboxone::read(package, member);
    std::cout << package.filename().string() << ": " << program.container << " " << program.identity << " " << program.version << ", " << program.member << ", " << program.file.size() << " bytes\n";
    fs::ScopedTempDir dir("cb_xbox_homebrew");
    const stdfs::path loose = dir.path() / stdfs::path(program.member).filename();
    fs::write_file(loose, program.file);
    const formats::pe::Image image = formats::xboxone::image_of(program);
    judge("xboxone", loose, [&](std::uint32_t rva, std::size_t n) -> ByteView {
        const auto o = image.rva_to_offset(rva);
        return o ? ByteView(program.file).subspan(*o, std::min<std::size_t>(n, program.file.size() - *o)) : ByteView();
    }, most);
}

}  // namespace

// OpenOrbis's sample programs (GPL-3.0, OpenOrbis/OpenOrbis-PS4-Toolchain, release v0.5.3 samples-elf-llvm-18): plain ELFs the toolchain links, named imports.
CB_TEST("engine.x64.homebrew.ps4.openorbis_hello_world_elf", [] { judge_orbis("ps4", "ps4/elf18/samples/hello_world/hello_world/x64/Debug/hello_world.elf"); });
// The same program through OpenOrbis's own create-fself (built from OpenOrbis/create-fself, GPL-3.0): an Orbis ELF in a fake SELF, imports by NID.
CB_TEST("engine.x64.homebrew.ps4.openorbis_hello_world_fself", [] { judge_orbis("ps4", "ps4/fself/hello_world-eboot.bin"); });
// ps5-payload-dev (GPL-3.0): payloads for the PS5 and the PS4, a file server, a kernel log server and the ELF loader.
CB_TEST("engine.x64.homebrew.ps4.ps5_payload_ftpsrv_ps4", [] { judge_orbis("ps4", "ps5/ftpsrv-ps4.elf"); });
CB_TEST("engine.x64.homebrew.ps5.ps5_payload_ftpsrv_ps5", [] { judge_orbis("ps5", "ps5/ftpsrv-ps5.elf"); });
CB_TEST("engine.x64.homebrew.ps5.ps5_payload_klogsrv_ps5", [] { judge_orbis("ps5", "ps5/klogsrv-ps5.elf"); });
CB_TEST("engine.x64.homebrew.ps5.ps5_payload_elfldr_ps5", [] { judge_orbis("ps5", "ps5/elfldr-ps5.elf"); });

// Moonlight UWP (GPL-3.0, TheElixZammuto/moonlight-xbox, release 1.19.0): the Xbox One / Series game-streaming client, an .msixbundle of the x64 package; its app executable.
CB_TEST("engine.x64.homebrew.xboxone.moonlight_uwp_app_executable", [] { judge_xboxone("xboxone/moonlight-xbox-dx_1.19.0.0_x64.msixbundle", "", 10); });
// ... and one of the DLLs the package carries (Brotli's decoder, MIT), small enough to read as a whole.
CB_TEST("engine.x64.homebrew.xboxone.moonlight_uwp_brotli_decoder", [] { judge_xboxone("xboxone/moonlight-xbox-dx_1.19.0.0_x64.msixbundle", "brotlidec.dll", 10); });
