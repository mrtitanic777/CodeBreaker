#include "cb/coverage/reach_command.hpp"

#include <memory>
#include <optional>

#include "cb/engine/function_store.hpp"
#include "cb/engine/lane_kit.hpp"
#include "cb/formats/detect.hpp"
#include "cb/formats/gb.hpp"
#include "cb/formats/gba.hpp"
#include "cb/formats/nds.hpp"
#include "cb/formats/n64.hpp"
#include "cb/formats/genesis.hpp"
#include "cb/formats/psx.hpp"
#include "cb/formats/msx.hpp"
#include "cb/formats/coleco.hpp"
#include "cb/formats/atari2600.hpp"
#include "cb/formats/atari5200.hpp"
#include "cb/formats/jaguar.hpp"
#include "cb/formats/threedo.hpp"
#include "cb/formats/wonderswan.hpp"
#include "cb/formats/cdi.hpp"
#include "cb/formats/vectrex.hpp"
#include "cb/formats/intv.hpp"
#include "cb/formats/vb.hpp"
#include "cb/formats/atari7800.hpp"
#include "cb/formats/cpc.hpp"
#include "cb/formats/lynx.hpp"
#include "cb/formats/pce.hpp"
#include "cb/formats/snes.hpp"
#include "cb/formats/neogeo.hpp"
#include "cb/formats/dreamcast.hpp"
#include "cb/formats/saturn.hpp"
#include "cb/formats/ppc_program.hpp"
#include "cb/formats/sega32x.hpp"
#include "cb/formats/segacd.hpp"
#include "cb/formats/nes.hpp"
#include "cb/formats/pe.hpp"
#include "cb/formats/sms.hpp"
#include "cb/formats/xex.hpp"
#include "cb/formats/zx.hpp"
#include "cb/platforms/dos.hpp"
#include "cb/platforms/gb.hpp"
#include "cb/platforms/gba.hpp"
#include "cb/platforms/nds.hpp"
#include "cb/platforms/n64.hpp"
#include "cb/platforms/genesis.hpp"
#include "cb/platforms/ps1.hpp"
#include "cb/platforms/java.hpp"
#include "cb/platforms/msx.hpp"
#include "cb/platforms/coleco.hpp"
#include "cb/platforms/atari2600.hpp"
#include "cb/platforms/atari5200.hpp"
#include "cb/platforms/jaguar.hpp"
#include "cb/platforms/threedo.hpp"
#include "cb/platforms/wonderswan.hpp"
#include "cb/platforms/cdi.hpp"
#include "cb/platforms/vectrex.hpp"
#include "cb/platforms/intv.hpp"
#include "cb/platforms/vb.hpp"
#include "cb/platforms/atari7800.hpp"
#include "cb/platforms/cpc.hpp"
#include "cb/platforms/lynx.hpp"
#include "cb/platforms/pce.hpp"
#include "cb/platforms/snes.hpp"
#include "cb/platforms/neogeo.hpp"
#include "cb/platforms/dreamcast.hpp"
#include "cb/platforms/ps2.hpp"
#include "cb/platforms/n3ds.hpp"
#include "cb/platforms/vita.hpp"
#include "cb/platforms/psp.hpp"
#include "cb/formats/ps2.hpp"
#include "cb/formats/n3ds.hpp"
#include "cb/formats/vita.hpp"
#include "cb/formats/psp.hpp"
#include "cb/platforms/saturn.hpp"
#include "cb/platforms/gamecube.hpp"
#include "cb/platforms/wii.hpp"
#include "cb/platforms/wiiu.hpp"
#include "cb/platforms/sega32x.hpp"
#include "cb/platforms/segacd.hpp"
#include "cb/platforms/nes.hpp"
#include "cb/platforms/sms.hpp"
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
    probes.is_nes = [](const std::filesystem::path& p) { return formats::nes::probe(p); };
    probes.is_sms = [](const std::filesystem::path& p) { return formats::sms::probe(p); };
    probes.is_gb = [](const std::filesystem::path& p) { return formats::gb::probe(p); };
    probes.is_genesis = [](const std::filesystem::path& p) { return formats::genesis::probe(p); };
    probes.is_ps1 = [](const std::filesystem::path& p) { return formats::psx::probe(p); };
    probes.is_gba = [](const std::filesystem::path& p) { return formats::gba::probe(p); };
    probes.is_nds = [](const std::filesystem::path& p) { return formats::nds::probe(p); };
    probes.is_n64 = [](const std::filesystem::path& p) { return formats::n64::probe(p); };
    probes.is_msx = [](const std::filesystem::path& p) { return formats::msx::probe(p); };
    probes.is_coleco = [](const std::filesystem::path& p) { return formats::coleco::probe(p); };
    probes.is_cpc = [](const std::filesystem::path& p) { return formats::cpc::probe(p); };
    probes.is_atari5200 = [](const std::filesystem::path& p) { return formats::atari5200::probe(p); };
    probes.is_jaguar = [](const std::filesystem::path& p) { return formats::jaguar::probe(p); };
    probes.is_threedo = [](const std::filesystem::path& p) { return formats::threedo::probe(p); };
    probes.is_wonderswan = [](const std::filesystem::path& p) { return formats::wonderswan::probe(p); };
    probes.is_cdi = [](const std::filesystem::path& p) { return formats::cdi::probe(p); };
    probes.is_vectrex = [](const std::filesystem::path& p) { return formats::vectrex::probe(p); };
    probes.is_intv = [](const std::filesystem::path& p) { return formats::intv::probe(p); };
    probes.is_vb = [](const std::filesystem::path& p) { return formats::vb::probe(p); };
    probes.is_atari2600 = [](const std::filesystem::path& p) { return formats::atari2600::probe(p); };
    probes.is_atari7800 = [](const std::filesystem::path& p) { return formats::atari7800::probe(p); };
    probes.is_lynx = [](const std::filesystem::path& p) { return formats::lynx::probe(p); };
    probes.is_pce = [](const std::filesystem::path& p) { return formats::pce::probe(p); };
    probes.is_snes = [](const std::filesystem::path& p) { return formats::snes::probe(p); };
    probes.is_neogeo = [](const std::filesystem::path& p) { return formats::neogeo::probe(p); };
    probes.is_segacd = [](const std::filesystem::path& p) { return formats::segacd::probe(p); };
    probes.is_sega32x = [](const std::filesystem::path& p) { return formats::sega32x::probe(p); };
    probes.is_saturn = [](const std::filesystem::path& p) { return formats::saturn::probe(p); };
    probes.is_ps2 = [](const std::filesystem::path& p) { return formats::ps2::probe(p); };
    probes.is_psp = [](const std::filesystem::path& p) { return formats::psp::probe(p); };
    probes.is_n3ds = [](const std::filesystem::path& p) { return formats::n3ds::probe(p); };
    probes.is_vita = [](const std::filesystem::path& p) { return formats::vita::probe(p); };
    probes.is_dreamcast = [](const std::filesystem::path& p) { return formats::dreamcast::probe(p); };
    probes.is_gamecube = [](const std::filesystem::path& p) { return formats::ppc_program::probe(p, formats::ppc_program::Console::GameCube); };
    probes.is_wii = [](const std::filesystem::path& p) { return formats::ppc_program::probe(p, formats::ppc_program::Console::Wii); };
    probes.is_wiiu = [](const std::filesystem::path& p) { return formats::ppc_program::probe(p, formats::ppc_program::Console::WiiU); };
    probes.is_zx = [](const std::filesystem::path& p) { return formats::zx::probe(p); };
    probes.is_java_module = [](const std::filesystem::path& p) { return verify::lanes::JavaLane::is_module(p); };
    probes.xex_basefile = [](const std::filesystem::path& p) { return p; };
    probes.xbe_to_pe = [](const std::filesystem::path& p) { return p; };
    auto registry = std::make_unique<platforms::Registry>(std::move(probes));
    registry->add(platforms::dos());
    registry->add(platforms::vxd());
    registry->add(platforms::win16());
    registry->add(platforms::win32());
    registry->add(platforms::msx());
    registry->add(platforms::coleco());
    registry->add(platforms::cpc());
    registry->add(platforms::atari5200());
    registry->add(platforms::jaguar());
    registry->add(platforms::threedo());
    registry->add(platforms::wonderswan());
    registry->add(platforms::cdi());
    registry->add(platforms::vectrex());
    registry->add(platforms::intv());
    registry->add(platforms::vb());
    registry->add(platforms::atari2600());
    registry->add(platforms::atari7800());
    registry->add(platforms::lynx());
    registry->add(platforms::pce());
    registry->add(platforms::snes());
    registry->add(platforms::neogeo());
    registry->add(platforms::segacd());
    registry->add(platforms::sega32x());
    registry->add(platforms::saturn());
    registry->add(platforms::dreamcast());
    registry->add(platforms::gamecube());
    registry->add(platforms::wii());
    registry->add(platforms::wiiu());
    registry->add(platforms::ps2());
    registry->add(platforms::psp());
    registry->add(platforms::n3ds());
    registry->add(platforms::vita());
    registry->add(platforms::zx());
    registry->add(platforms::genesis());
    registry->add(platforms::ps1());
    registry->add(platforms::gba());
    registry->add(platforms::nds());
    registry->add(platforms::n64());
    registry->add(platforms::gb());
    registry->add(platforms::sms());
    registry->add(platforms::nes());
    registry->add(platforms::xbe());
    registry->add(platforms::xbox360());
    registry->add(platforms::java());
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
    return s;
}

cli::Command command(const Environment& environment) { return reach::command(services(environment)); }

}  // namespace cb::coverage::reach_command
