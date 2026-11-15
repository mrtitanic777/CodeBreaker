#include "cb/coverage/reach_command.hpp"

#include <memory>
#include <optional>

#include "cb/engine/function_store.hpp"
#include "cb/engine/lane_kit.hpp"
#include "cb/engine/lane_registry.hpp"
#include "cb/formats/detect.hpp"
#include "cb/formats/pe.hpp"
#include "cb/formats/xex.hpp"
#include "cb/platforms/dos.hpp"
#include "cb/platforms/java.hpp"
#include "cb/platforms/vxd.hpp"
#include "cb/platforms/win16.hpp"
#include "cb/verify/lanes/java.hpp"
#include "cb/verify/reconstruct.hpp"
#include "cb/verify/verdicts.hpp"

namespace cb::coverage::reach_command {

namespace {

std::optional<platforms::ExeKind> exe_kind_of(std::optional<formats::detect::Lane> lane) {
    if (!lane) return std::nullopt;
    switch (*lane) {
    case formats::detect::Lane::Pe: return platforms::ExeKind::Pe;
    case formats::detect::Lane::Ne: return platforms::ExeKind::Ne;
    case formats::detect::Lane::Le: return platforms::ExeKind::Le;
    case formats::detect::Lane::Dos: return platforms::ExeKind::Dos;
    }
    return std::nullopt;
}

// What the tally of a program is asked of: the lanes (the program's identity) and the function store (its verdicts). Made on the first ask.
class TallyDesk {
public:
    explicit TallyDesk(const Environment& environment)
        : kit_(environment.config, environment.corpus),
          store_(environment.config.state("functions.sqlite"), environment.config.state("db"), true) {
        kit_.add_lanes(kinds_);
        reconstructor_.emplace(kit_.reconstruct_services(kinds_, store_));
    }

    reach::Tally tally(const std::filesystem::path& program) {
        const verify::reconstruct::Identity id = reconstructor_->identity(program);
        reach::Tally out;
        for (const auto& [cls, n] : store_.tally(id.profile, id.sha)) out[std::string(verify::word(cls))] = n;
        return out;
    }

private:
    engine::LaneKit kit_;
    verify::reconstruct::Kinds kinds_;
    engine::FunctionStore store_;
    std::optional<verify::reconstruct::Reconstructor> reconstructor_;
};

}  // namespace

std::unique_ptr<platforms::Registry> standard_registry() {
    engine::register_all_lanes();   // (the platform lanes: each gives its descriptor and its probe, engine/lane_registry.hpp)
    platforms::Probes probes;
    probes.executable_class = [](const std::filesystem::path& p) { return exe_kind_of(formats::detect::lane(p)); };
    probes.kind = [](const std::filesystem::path& p) { return exe_kind_of(formats::detect::kind(p)); };
    probes.pe_machine = [](const std::filesystem::path& p) -> std::optional<std::uint16_t> {
        try {
            return static_cast<std::uint16_t>(formats::pe::Image::open(p).header().machine);
        } catch (const Error&) {
            return std::nullopt;
        }
    };
    probes.holds_drivers = [](const std::filesystem::path& p) { return formats::detect::holds_drivers(p); };
    probes.is_xex = [](const std::filesystem::path& p) { return formats::xex::probe(p); };
    probes.is_java_module = [](const std::filesystem::path& p) { return verify::lanes::JavaLane::is_module(p); };
    probes.xex_basefile = [](const std::filesystem::path& p) { return p; };
    probes.xbe_to_pe = [](const std::filesystem::path& p) { return p; };
    auto registry = std::make_unique<platforms::Registry>(std::move(probes));
    registry->add(platforms::dos());
    registry->add(platforms::vxd());
    registry->add(platforms::win16());
    registry->add(platforms::win32());
    registry->add(platforms::xbe());
    registry->add(platforms::xbox360());
    registry->add(platforms::java());
    for (const auto& lane : engine::registered_lanes()) registry->add(engine::lane_platform(*lane));   // (the registered lanes, in the order of engine/lane_list.cpp)
    return registry;
}

reach::Services services(const Environment& environment) {
    const std::shared_ptr<platforms::Registry> registry = standard_registry();
    const auto desk = std::make_shared<std::optional<TallyDesk>>();   // made by the first tally
    reach::Services s;
    // keep the registry alive as long as the function that reads it
    s.identify_platform = [registry, identify = reach::identify_with(*registry)](const std::filesystem::path& p) { return identify(p); };
    s.tally = [environment, desk](const std::filesystem::path& p) {
        if (!desk->has_value()) desk->emplace(environment);
        return (*desk)->tally(p);
    };
    s.state_dir = environment.config.state();
    s.checkout_root = environment.config.checkout_root();
    s.program_extensions = engine::lane_program_extensions();
    return s;
}

cli::Command command(const Environment& environment) { return reach::command(services(environment)); }

}  // namespace cb::coverage::reach_command
