// engine/pcode_cpu_compare.hpp -- the lanes' proof that the one emulator and a lane's own CPU core agree: the lane's homebrew run in every state of the lane on both cores, and
// each function judged against copies of the program with one of its first bytes turned over, the verdicts compared. Used by the adapters' tests.
//
//     compare_homebrew("saturn", "saturn", ".iso", file_offset);
//
// CB_PCODE_FUNCTIONS=<n> runs n functions of each program (20 by default), CB_PCODE_FILES=<n> n programs (16 by default), CB_PCODE_ONLY=<hex address> the one function.
#pragma once

#include <chrono>
#include <cstdio>
#include <format>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>

#include "base/config.hpp"
#include "core/cpu_time.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "engine/lane_kit.hpp"
#include "engine/pcode_cpu.hpp"
#include "verify/lanes/image_lane.hpp"

namespace cb::engine::pcode_compare {

struct ForcedCore {
    explicit ForcedCore(bool on) { engine::pcode_cpu::force(on); }
    ~ForcedCore() { engine::pcode_cpu::force(std::nullopt); }
};

// `file_offset(path, address)`: where in the file a function's first bytes are (SIZE_MAX: nowhere, no edited copies are judged).
inline void compare_homebrew(const char* kind_name, const char* folder, const char* extension,
                             const std::function<std::size_t(const std::filesystem::path&, std::uint32_t)>& file_offset,
                             const std::function<bool(std::uint32_t)>& wanted = {}) {   // (`wanted`: which functions are of the processor under test)
    namespace stdfs = std::filesystem;
    fs::ScopedTempDir dir(std::string("cb_pcode_") + kind_name);
    std::optional<fs::ScopedEnv> scoped;
    std::unique_ptr<engine::LaneKit> kit;
    try {
        const config::Config config = config::Config::load(config::current().main_checkout());
        scoped.emplace("CB_STATE", fs::to_text(dir.path() / "state"));
        kit = std::make_unique<engine::LaneKit>(config::Config::load(config.checkout_root()));
    } catch (const Error&) {
        return;
    }
    const std::shared_ptr<const verify::lanes::Kind> kind = kit->machine_lane(kind_name);
    const auto* lane = dynamic_cast<const verify::lanes::ImageLane*>(kind.get());
    if (lane == nullptr) return;
    const verify::lanes::ImageLaneSpec& spec = lane->spec();
    stdfs::path root = testing::beside_checkout("homebrew-samples");
    if (const auto given = fs::get_env("CB_HOMEBREW_SAMPLES"); given && !given->empty()) root = fs::from_text(*given);
    const stdfs::path samples = stdfs::exists(root) ? root : testing::beside_checkout("test-inputs/homebrew-samples");
    unsigned functions = 0, runs = 0, differ = 0, files = 0, judged = 0, disagree = 0;
    unsigned verdicts[2][2] = {};
    double seconds[2] = {0, 0};
    std::map<std::string, unsigned> reasons;
    std::map<std::string, std::string> reason_example;
    const unsigned file_limit = [] {
        const auto given = fs::get_env("CB_PCODE_FILES");
        return given && !given->empty() ? static_cast<unsigned>(std::strtoul(given->c_str(), nullptr, 10)) : 16u;
    }();
    std::error_code ec;
    for (const auto& dirent : stdfs::recursive_directory_iterator(samples / folder, ec)) {
        if (dirent.path().extension() != extension) continue;
        if (++files > file_limit) {
            --files;
            break;
        }
        const stdfs::path work = dir.path() / std::to_string(files);
        stdfs::create_directories(work);
        std::unique_ptr<verify::lanes::Prepared> prepared;
        try {
            prepared = lane->prepare(dirent.path(), work / "work");
        } catch (const Error&) {
            continue;
        }
        const auto& image = static_cast<const verify::lanes::ImageLane::PreparedImage&>(*prepared);
        const Bytes original_bytes = fs::read_file(dirent.path());
        unsigned per_file = 0;
        const std::optional<std::string> only = fs::get_env("CB_PCODE_ONLY");
        const unsigned per_file_limit = [] {
            const auto given = fs::get_env("CB_PCODE_FUNCTIONS");
            return given && !given->empty() ? static_cast<unsigned>(std::strtoul(given->c_str(), nullptr, 10)) : 20u;
        }();
        for (const auto& [address, name] : image.written.functions) {
            if (wanted && !wanted(address)) continue;
            if (per_file++ == per_file_limit) break;
            if (only && *only != std::format("{:x}", address)) continue;
            ++functions;
            const std::size_t at = file_offset(dirent.path(), address);
            for (std::size_t k = 0; k < 3 && at != SIZE_MAX && at + k < original_bytes.size(); ++k) {   // against copies with one of the function's first three bytes turned over
                Bytes changed = original_bytes;
                changed[at + k] ^= 0xFF;
                const stdfs::path edited = work / ("edited" + std::string(extension));
                fs::write_file(edited, changed);
                verify::Judgement j[2];
                for (int core = 0; core < 2; ++core) {
                    ForcedCore forced(core == 1);
                    j[core] = lane->judge_function(false, dirent.path(), edited, address);
                    ++verdicts[core][j[core].verdict == verify::Verdict::Equivalent ? 0 : 1];
                }
                ++judged;
                if (j[0].verdict != j[1].verdict && ++disagree <= 4)
                    std::printf("  %s: function %x byte +%zu: own core %s (%s), emulator %s (%s)\n", dirent.path().filename().string().c_str(), address, k,
                                std::string(verify::word(j[0].verdict)).c_str(), j[0].reason.c_str(), std::string(verify::word(j[1].verdict)).c_str(), j[1].reason.c_str());
            }
            for (const verify::lanes::State& state : spec.states) {
                verify::lanes::Effects e[2];
                for (int core = 0; core < 2; ++core) {
                    ForcedCore forced(core == 1);
                    const double start = cb::thread_cpu_seconds();
                    e[core] = spec.effects(dirent.path(), address, state);
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
                            if (differ <= 8 || only) {
                                std::size_t j = 0;
                                while (j < la->size() && j < lb->size() && (*la)[j] == (*lb)[j]) ++j;
                                std::printf("  %s %x: [%s] first line %zu own '%s' (%zu) emulator '%s' (%zu)\n", dirent.path().filename().string().c_str(), address, spec.what[i].c_str(), j,
                                            j < la->size() ? (*la)[j].c_str() : "-", la->size(), j < lb->size() ? (*lb)[j].c_str() : "-", lb->size());
                                if (only)
                                    for (std::size_t k = j > 3 ? j - 3 : 0; k < j + 3; ++k)   // (the lines around it, with CB_PCODE_ONLY)
                                        std::printf("      [%zu] own '%s' emulator '%s'\n", k, k < la->size() ? (*la)[k].c_str() : "-", k < lb->size() ? (*lb)[k].c_str() : "-");
                            }
                }
                ++reasons[what.substr(0, 130)];
                if (!reason_example.count(what.substr(0, 130))) reason_example[what.substr(0, 130)] = std::format("{} {:x} state {}", dirent.path().filename().string(), address, &state - &spec.states[0]);
            }
        }
    }
    std::printf("pcode_cpu %s homebrew: %u programs, %u functions, %u runs, effects differing %u; edited programs judged %u: own core equivalent %u divergent %u, emulator equivalent %u "
                "divergent %u, verdicts disagreeing %u; time own core %.2fs emulator %.2fs\n",
                kind_name, files, functions, runs, differ, judged, verdicts[0][0], verdicts[0][1], verdicts[1][0], verdicts[1][1], disagree, seconds[0], seconds[1]);
    unsigned shown = 0;
    for (const auto& [k, c] : reasons)
        if (++shown <= 14) std::printf("    %u x%s   e.g. %s\n", c, k.c_str(), reason_example[k].c_str());
    CB_CHECK_EQ(differ, 0u);
    CB_CHECK_EQ(disagree, 0u);
}

}  // namespace cb::engine::pcode_compare
