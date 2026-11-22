// The Nintendo 64 lane's R4300i on the one emulator (engine/pcode_cpu_r4300.cpp): the lane's homebrew run in every state on both cores, and judged against edited
// copies. A report with a check at the end: the lane stays off by default while it differs.
#include <chrono>
#include <cstdio>
#include <format>
#include <map>
#include <vector>

#include "base/config.hpp"
#include "core/cpu_time.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "emulate/r4300.hpp"
#include "engine/lane_kit.hpp"
#include "engine/pcode_cpu.hpp"
#include "formats/n64.hpp"
#include "verify/lanes/image_lane.hpp"

using namespace cb;
namespace stdfs = std::filesystem;

namespace {

struct R4300Forced {
    explicit R4300Forced(bool on) { engine::pcode_cpu::force(on); }
    ~R4300Forced() { engine::pcode_cpu::force(std::nullopt); }
};

}  // namespace

CB_TEST("engine.pcode_cpu.the_n64_lane_runs_homebrew_the_same_on_both_cores", [] {
    fs::ScopedTempDir dir("cb_pcode_r4300_n64");
    std::optional<fs::ScopedEnv> scoped;
    std::unique_ptr<engine::LaneKit> kit;
    try {
        const config::Config config = config::Config::load(config::current().main_checkout());
        scoped.emplace("CB_STATE", fs::to_text(dir.path() / "state"));
        kit = std::make_unique<engine::LaneKit>(config::Config::load(config.checkout_root()));
    } catch (const Error&) {
        return;
    }
    const std::shared_ptr<const verify::lanes::Kind> kind = kit->machine_lane("n64");
    const auto* lane = dynamic_cast<const verify::lanes::ImageLane*>(kind.get());
    if (lane == nullptr) return;
    const verify::lanes::ImageLaneSpec& spec = lane->spec();
    const stdfs::path root = testing::beside_checkout("homebrew-samples");
    const stdfs::path samples = stdfs::exists(root) ? root : testing::beside_checkout("test-inputs/homebrew-samples");
    unsigned functions = 0, runs = 0, differ = 0, files = 0, judged = 0, disagree = 0;
    unsigned verdicts[2][2] = {};
    double seconds[2] = {0, 0};
    std::map<std::string, unsigned> reasons;
    std::error_code ec;
    for (const auto& entry : stdfs::directory_iterator(samples / "n64" / "peterlemon", ec)) {
        if (entry.path().extension() != ".N64" && entry.path().extension() != ".n64") continue;
        if (++files > 24) break;
        const stdfs::path work = dir.path() / std::to_string(files);
        stdfs::create_directories(work);
        std::unique_ptr<verify::lanes::Prepared> prepared;
        try {
            prepared = lane->prepare(entry.path(), work / "work");
        } catch (const Error&) {
            continue;
        }
        const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
        const Bytes original_bytes = fs::read_file(entry.path());
        const formats::n64::Rom rom = formats::n64::read(entry.path());
        unsigned per_file = 0;
        for (const auto& [address, name] : image.written.functions) {
            if (per_file++ == 20) break;
            ++functions;
            const std::size_t at = 0x1000 + (address & 0x1FFFFFFFu) - (rom.load_address() & 0x1FFFFFFFu);
            for (std::size_t k = 0; k < 3 && at + k < original_bytes.size(); ++k) {   // against copies with one of the function's first three bytes turned over
                Bytes changed = original_bytes;
                changed[at + k] ^= 0xFF;
                const stdfs::path edited = work / "edited.n64";
                fs::write_file(edited, changed);
                verify::Judgement j[2];
                for (int core = 0; core < 2; ++core) {
                    R4300Forced forced(core == 1);
                    j[core] = lane->judge_function(false, entry.path(), edited, address);
                    ++verdicts[core][j[core].verdict == verify::Verdict::Equivalent ? 0 : 1];
                }
                ++judged;
                if (j[0].verdict != j[1].verdict && ++disagree <= 4)
                    std::printf("  %s: function %x byte +%zu: own core %s (%s), emulator %s (%s)\n", entry.path().filename().string().c_str(), address, k,
                                std::string(verify::word(j[0].verdict)).c_str(), j[0].reason.c_str(), std::string(verify::word(j[1].verdict)).c_str(), j[1].reason.c_str());
            }
            for (const verify::lanes::State& state : spec.states) {
                verify::lanes::Effects e[2];
                for (int core = 0; core < 2; ++core) {
                    R4300Forced forced(core == 1);
                    const double start = cb::thread_cpu_seconds();
                    e[core] = spec.effects(entry.path(), address, state);
                    seconds[core] += (cb::thread_cpu_seconds() - start);
                }
                ++runs;
                if (e[0] == e[1]) continue;
                ++differ;
                std::string what;
                for (std::size_t i = 0; i < e[0].size() && i < e[1].size(); ++i) {
                    if (e[0][i] == e[1][i]) continue;
                    what += std::format(" [{}]", i < spec.what.size() ? spec.what[i] : std::to_string(i));
                    if (const auto* a = std::get_if<std::string>(&e[0][i]))
                        if (const auto* b = std::get_if<std::string>(&e[1][i])) what += std::format(" own '{}' emulator '{}'", *a, *b);
                    if (const auto* la = std::get_if<std::vector<std::string>>(&e[0][i]))
                        if (const auto* lb = std::get_if<std::vector<std::string>>(&e[1][i]))
                            if (differ <= 8) {
                                std::size_t j = 0;
                                while (j < la->size() && j < lb->size() && (*la)[j] == (*lb)[j]) ++j;
                                std::printf("  %s %x: [%s] first line %zu own '%s' (%zu lines) emulator '%s' (%zu lines)\n", entry.path().filename().string().c_str(), address, spec.what[i].c_str(), j,
                                            j < la->size() ? (*la)[j].c_str() : "-", la->size(), j < lb->size() ? (*lb)[j].c_str() : "-", lb->size());
                            }
                }
                ++reasons[what.substr(0, 130)];
            }
        }
    }
    std::printf("pcode_cpu n64 homebrew: %u programs, %u functions, %u runs, effects differing %u; edited programs judged %u: own core equivalent %u divergent %u, emulator equivalent %u "
                "divergent %u, verdicts disagreeing %u; time own core %.2fs emulator %.2fs\n",
                files, functions, runs, differ, judged, verdicts[0][0], verdicts[0][1], verdicts[1][0], verdicts[1][1], disagree, seconds[0], seconds[1]);
    unsigned shown = 0;
    for (const auto& [k, c] : reasons)
        if (++shown <= 12) std::printf("    %u x%s\n", c, k.c_str());
    CB_CHECK_EQ(differ, 0u);
    CB_CHECK_EQ(disagree, 0u);
});
