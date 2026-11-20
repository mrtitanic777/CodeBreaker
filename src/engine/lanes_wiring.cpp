#include "engine/lanes_wiring.hpp"

#include <algorithm>

#include "core/error.hpp"

namespace cb::engine::lanes_wiring {

void Builders::add(Builder builder) {
    if (!builder.handles || !builder.build) throw UsageError("a builder needs the test for its files and its build");
    const auto same = std::find_if(builders_.begin(), builders_.end(), [&](const Builder& b) { return b.platform == builder.platform; });
    if (same != builders_.end()) *same = std::move(builder);
    else builders_.push_back(std::move(builder));
}

std::vector<std::string> Builders::platforms() const {
    std::vector<std::string> out;
    for (const Builder& b : builders_) out.push_back(b.platform);
    return out;
}

std::optional<BuildResult> Builders::delegate(const std::filesystem::path& module, const std::optional<std::filesystem::path>& outdir, const Run::Log& log) const {
    for (const Builder& b : builders_)
        if (b.handles(module)) return b.build(module, outdir, log);
    return std::nullopt;
}

Builder pe_builder(lanes::Lanes& lanes, std::function<bool(const std::filesystem::path&)> handles) {
    return {"win32", std::move(handles), [&lanes](const std::filesystem::path& module, const std::optional<std::filesystem::path>& outdir, const Run::Log& log) {
                const lanes::PeBuild b = lanes.build_pe(module, outdir, log);
                return BuildResult{"win32", b.name, b.report, b.rejected, {}, b.image};
            }};
}

Builder win16_builder(win16::Win16& win16, std::function<bool(const std::filesystem::path&)> handles) {
    return {std::string(win16::kPlatform), std::move(handles),
            [&win16](const std::filesystem::path& module, const std::optional<std::filesystem::path>& outdir, const Run::Log& log) {
                const win16::Built16 b = win16.build(module, outdir, log);
                return BuildResult{std::string(win16::kPlatform), b.name, b.report, b.rejected, b.gated, b.image};
            }};
}

std::string register_lane(Wiring& wiring, LaneRegistration r) {
    if (!r.kind) throw UsageError("a lane is registered with its Kind");
    wiring.kinds.add(r.kind, r.traits);
    lanes::Setup setup;
    setup.kind = r.kind;
    setup.traits = r.traits;
    setup.platform = r.platform;
    setup.module_stamp = std::move(r.module_stamp);
    setup.build_stamp = std::move(r.build_stamp);
    const std::string name = wiring.lanes.declare(std::move(setup));
    if (r.link) {
        if (name != "pe") throw UsageError("pe.image is the lane \"pe\"'s: the lane '" + name + "' cannot link whole");
        if (!wiring.is_pe) throw UsageError("the lane \"pe\" is linked whole: the wiring needs the test for its files");
        wiring.lanes.declare_pe_image(std::move(*r.link), r.platform, std::move(r.link_stamp));
        wiring.builders.add(pe_builder(wiring.lanes, wiring.is_pe));
    }
    return name;
}

}  // namespace cb::engine::lanes_wiring
