#include "cb/platforms/vxd.hpp"

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

// DIVERGES from codebreaker/platforms/vxd.py: the descriptor is data only (the Python's Ref slots -- the LE reader, the
// driver emulator, build/driver's tool, source/driver_writer -- are the layers' own tables now, keyed on Id::Vxd);
// tools() is the build layer's. entries() is vxd_entry_points().

using namespace cb;
using namespace cb::platforms;

namespace {
std::filesystem::path driver(const char* name) { return testing::data_path(std::string("formats/testdata/") + name); }
}  // namespace

CB_TEST("platforms.vxd.descriptor", [] {
    const Platform p = vxd();
    CB_CHECK(p.id == Id::Vxd);
    CB_CHECK_EQ(p.name, "le");
    CB_CHECK_EQ(p.title, "device drivers");
    CB_CHECK_EQ(p.family, "x86");
    CB_CHECK(*p.source == SourceLanguage::Asm);
    CB_CHECK_EQ(*p.lane, "driver");
    CB_CHECK(p.steps == Steps::Lanes);
    CB_CHECK_EQ(p.loader.container, "LE");
    CB_CHECK_EQ(p.cpu.language, "x86:LE:32:default:windows");
    CB_CHECK_EQ(*p.cpu.bits, 32u);
    CB_CHECK(std::get<CapstoneDecoder>(p.cpu.decoder).bits == 32u);
    CB_CHECK_EQ(p.cpu.registers.size(), 8u);
    CB_CHECK_EQ(p.cpu.stack, "ESP");
    CB_CHECK((p.convention.names == std::vector<std::string>{"registers"}));
    CB_CHECK(std::holds_alternative<InRegisters>(p.convention.arguments));
    CB_CHECK_EQ(*p.convention.word, 4u);
    CB_CHECK(*p.convention.order == CallOrder::Registers);
    CB_CHECK((p.convention.results == std::vector<std::string>{"EAX", "CF", "ZF"}));
    CB_CHECK(*p.convention.pops == Pops::Callee);
    CB_CHECK((p.convention.preserved == std::vector<std::string>{"EBX", "ECX", "EDX", "ESI", "EDI", "EBP"}));
    CB_CHECK(p.emulator.engine == Engine::Unicorn);
    CB_CHECK(*p.compiler.language == Language::Asm);
    CB_CHECK_EQ(p.compiler.toolchain, "openwatcom wasm, the DDK's macros");
});

CB_TEST("platforms.vxd.entry_points_of_real_drivers", [] {
    // splitter.vxd: a control procedure at object 1 offset 0x99 and five services
    const auto splitter = vxd_entry_points(driver("splitter.vxd"));
    CB_CHECK_EQ(splitter.entries().front().first, "control");
    CB_CHECK_EQ(splitter.at("control").object, 1u);
    CB_CHECK_EQ(splitter.at("control").offset, std::int64_t{0x99});
    std::size_t services = 0;
    for (const auto& [label, place] : splitter) {
        CB_CHECK(label == "control" || label == "v86_api" || label == "pm_api" || label.starts_with("service_"));
        CB_CHECK(place.object >= 1);
        if (label.starts_with("service_")) ++services;
    }
    CB_CHECK_EQ(services, 5u);
    CB_CHECK(splitter.contains("service_0") && splitter.contains("service_4") && !splitter.contains("service_5"));

    // bios.vxd: four services, each a routine in the code object (object 2)
    const auto bios = vxd_entry_points(driver("bios.vxd"));
    std::size_t bios_services = 0;
    for (const auto& [label, place] : bios)
        if (label.starts_with("service_")) {
            ++bios_services;
            CB_CHECK_EQ(place.object, 2u);
        }
    CB_CHECK_EQ(bios_services, 4u);
});

CB_TEST("platforms.vxd.not_a_driver_is_an_error", [] {
    const fs::ScopedTempDir dir("cbvxd");
    const auto text = dir.path() / "text.vxd";
    fs::write_file(text, as_bytes("not a driver, just a line of text for the reader to refuse"));
    CB_CHECK_THROWS_AS(FormatError, (void)vxd_entry_points(text));
    const auto empty = dir.path() / "empty.vxd";
    fs::write_file(empty, {});
    CB_CHECK_THROWS_AS(FormatError, (void)vxd_entry_points(empty));
    CB_CHECK_THROWS_AS(NotFoundError, (void)vxd_entry_points(dir.path() / "missing.vxd"));
    // a 16-bit NE module holds no LE image
    CB_CHECK_THROWS_AS(FormatError, (void)vxd_entry_points(driver("lzexpand.dll")));
    // a truncated driver: every cut is a clean error, never a crash
    const Bytes whole = fs::read_file(driver("splitter.vxd"));
    for (std::size_t cut : {std::size_t{0}, std::size_t{20}, std::size_t{64}, std::size_t{0x90}, std::size_t{0x200}, whole.size() / 2}) {
        const auto p = dir.path() / "cut.vxd";
        fs::write_file(p, ByteView(whole).first(cut));
        try {
            (void)vxd_entry_points(p);
        } catch (const FormatError&) {   // the expected answer for a cut file (some cuts still read: also fine)
        }
    }
});

CB_TEST("platforms.vxd.recognised_through_the_registry_by_drivers_probe", [] {
    Probes probes;
    probes.holds_drivers = [](const std::filesystem::path& p) { return p.extension() == ".vxd"; };
    Registry registry(probes);
    registry.add(vxd());
    CB_CHECK(registry.identify("a.vxd") != nullptr);
    CB_CHECK(registry.identify("a.exe") == nullptr);
    CB_CHECK_EQ(registry.lanes().size(), 1u);
    CB_CHECK_EQ(registry.lanes()[0].second, "driver");
});
