// engine/lanes_wiring.hpp -- the platforms that have a lane, handed to the engine.
//
//     Wiring wiring{.kinds = kinds, .lanes = lanes, .builders = builders};
//     register_lane(wiring, {.kind = pe_lane, .traits = {.per_function = true}, .platform = "win32", .link = link_pe});
//     builders.add(win16_builder(win16, handles_ne));
//     std::optional<BuildResult> built = builders.delegate("USER.EXE", outdir, log);       // module build's first step
//
// The engine's steps are declared for every lane there is (lanes.hpp), reconstruct finds a lane by the file's kind
// (verify/reconstruct.hpp), and `module build` hands a file to the steps of the platform that takes it: three registries that
// must agree on which lanes there are. `register_lane` fills the first two from one description; `Builders` is the third: the
// platforms whose module build runs on the engine, each with how it recognises its files and what its build gives.
#pragma once

#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "engine/lanes.hpp"
#include "engine/win16.hpp"
#include "verify/reconstruct.hpp"

namespace cb::engine::lanes_wiring {

// What a module build on the engine made, whichever platform's steps made it.
struct BuildResult {
    std::string platform;
    std::string name, report;
    std::map<std::string, std::string> rejected;     // function -> why the compiler turned it down
    std::map<std::string, std::string> gated;        // function -> why it was left to the original (16-bit)
    std::optional<std::filesystem::path> image;
};

// A platform whose module build runs on the engine's steps.
struct Builder {
    std::string platform;
    std::function<bool(const std::filesystem::path&)> handles;     // is this file one of the platform's
    std::function<BuildResult(const std::filesystem::path& module, const std::optional<std::filesystem::path>& outdir, const Run::Log& log)> build;
};

class Builders {
public:
    // Replaces the builder of the same platform.
    void add(Builder builder);
    // The platforms, in the order added.
    std::vector<std::string> platforms() const;
    // The build the first platform that takes the file makes of it; none when no platform's steps take it.
    std::optional<BuildResult> delegate(const std::filesystem::path& module, const std::optional<std::filesystem::path>& outdir = std::nullopt,
                                        const Run::Log& log = nullptr) const;

private:
    std::vector<Builder> builders_;
};

// The 32-bit builder: the lane "pe"'s pe.image. `handles`: is this file a 32-bit Windows module.
Builder pe_builder(lanes::Lanes& lanes, std::function<bool(const std::filesystem::path&)> handles);
// The 16-bit builder: ne.image.
Builder win16_builder(win16::Win16& win16, std::function<bool(const std::filesystem::path&)> handles);

// A lane, described once.
struct LaneRegistration {
    std::shared_ptr<const verify::lanes::Kind> kind;
    verify::reconstruct::KindTraits traits;
    std::string platform;                                  // the platform id the steps are declared under ("win32")
    std::function<std::string()> module_stamp, build_stamp;
    // The lane "pe" also links the module whole: its pe.image. Set for that lane only.
    std::optional<lanes::PeLink> link;
    std::function<std::string()> link_stamp;
};

// What a lane is registered with.
struct Wiring {
    verify::reconstruct::Kinds& kinds;
    lanes::Lanes& lanes;
    Builders& builders;
    // Is this file a module of the lane "pe" (needed with `link`): the builder of 32-bit modules is added with it.
    std::function<bool(const std::filesystem::path&)> is_pe;
};

// Adds the lane to reconstruct's kinds and the engine's steps, and -- with `link` -- pe.image and the 32-bit builder. Registering
// the same lane again replaces it (its steps are declared again). -> the lane's name.
std::string register_lane(Wiring& wiring, LaneRegistration registration);

}  // namespace cb::engine::lanes_wiring
