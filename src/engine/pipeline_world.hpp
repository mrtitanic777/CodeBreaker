// engine/pipeline_world.hpp -- the surroundings one pipeline command runs in (internal to source/pipeline_*.cpp).
//
// A World is made when a command starts: the lane kit over the settings and the corpus. What a command needs beyond it -- the function store of the
// state folder, the lanes and the reconstructor over them -- is made when first asked for, so a command that only reads (decompile, place) opens
// nothing it does not use.
#pragma once

#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <string_view>

#include "store/converted.hpp"
#include "store/function_store.hpp"
#include "engine/lane_kit.hpp"
#include "engine/pipeline.hpp"
#include "verify/reconstruct.hpp"

namespace cb::engine::pipeline {

class World {
public:
    explicit World(const Environment& environment);
    ~World();
    World(const World&) = delete;
    World& operator=(const World&) = delete;

    const config::Config& config() const { return config_; }
    engine::LaneKit& kit() { return kit_; }
    base::Corpus& corpus() { return kit_.corpus(); }
    // The function store of the state folder, opened for writing.
    store::FunctionStore& store();
    // The lanes the kit can make, and the reconstructor over them and the store.
    const verify::reconstruct::Kinds& kinds();
    const verify::reconstruct::Reconstructor& reconstructor();
    // Whether the reconstructor says on standard output what it did to each module (the line `Outcome::line` gives). Set before the first
    // `reconstructor()`; a sweep, which says its own, leaves it off.
    bool verbose = false;
    // Where a module's project folder goes (setting "converted").
    store::converted::Store converted() const;

    // A corpus name, or a folder that exists, as the path to read; nullopt when it is neither.
    std::optional<std::filesystem::path> find(std::string_view module);

private:
    config::Config config_;
    engine::LaneKit kit_;
    std::unique_ptr<store::FunctionStore> store_;
    std::optional<verify::reconstruct::Kinds> kinds_;
    std::unique_ptr<verify::reconstruct::Reconstructor> reconstructor_;
};

// The command with its handler replaced: `run` is given the world made when the command starts. The declaration (name, summary, options) is
// `declared`'s.
template <class Run>
cli::Command in_world(cli::Command declared, const Environment& environment, Run run) {
    declared.handler = [environment, run](const cli::Args& args) {
        World world(environment);
        return run(world, args);
    };
    return declared;
}

// The ratio difflib's SequenceMatcher gives two runs of bytes (autojunk off): twice the bytes in the matching blocks over the bytes of both.
double sequence_ratio(ByteView a, ByteView b);

}  // namespace cb::engine::pipeline
