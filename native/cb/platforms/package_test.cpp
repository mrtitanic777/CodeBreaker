#include "cb/platforms/package.hpp"

#include <set>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/platforms/dos.hpp"
#include "cb/platforms/java.hpp"
#include "cb/platforms/msx.hpp"

// DIVERGES from codebreaker/platforms/__init__.py and its port:
//   * a descriptor is data: the Refs (module and function names of other areas: the opener, the runner, the compiler,
//     the writer ...) are not carried -- those layers key their own tables on platforms::Id. runners() answers the
//     names of the platforms that run code (the old one answered the (module, function) pairs); lanes() the lane's
//     name ("win32", not "codebreaker.verify.lanes.win32"); step_sets() an enum;
//   * recognising a file asks the formats layer through Probes, wired once at start-up (an unwired probe is an
//     InternalError); a probe that finds the file damaged or gone is "no" (IoError/FormatError), other errors surface;
//   * zx is recognised by the formats probe alone (the port added an extension gate and a 16-byte fallback);
//   * no module-level registry: a Registry object is built by the program and passed down.

using namespace cb;
using namespace cb::platforms;

namespace {

Platform stand_in(Id id, const char* name, std::optional<std::uint16_t> machine = std::nullopt, bool runs = false) {
    Platform p;
    p.id = id;
    p.name = name;
    p.title = name;
    p.lane = name;
    p.machine = machine;
    p.steps = Steps::Lanes;
    p.emulator.runs_code = runs;
    return p;
}

// Probes answering from a table of what each path "is", the way the formats layer answers from a file's header.
struct Files {
    std::map<std::string, ExeKind> classes;       // by MZ header
    std::map<std::string, ExeKind> kinds;         // that, and headerless DOS
    std::map<std::string, std::uint16_t> machines;
    std::set<std::string> nes, sms, gb, genesis, msx, zx, xex, drivers, java_modules;

    Probes probes() const {
        auto in = [](const std::set<std::string>& s) {
            return [&s](const std::filesystem::path& p) { return s.contains(fs::to_text(p)); };
        };
        Probes pr;
        pr.executable_class = [this](const std::filesystem::path& p) -> std::optional<ExeKind> {
            auto it = classes.find(fs::to_text(p));
            return it == classes.end() ? std::nullopt : std::optional<ExeKind>(it->second);
        };
        pr.kind = [this](const std::filesystem::path& p) -> std::optional<ExeKind> {
            auto it = kinds.find(fs::to_text(p));
            return it == kinds.end() ? std::nullopt : std::optional<ExeKind>(it->second);
        };
        pr.pe_machine = [this](const std::filesystem::path& p) -> std::optional<std::uint16_t> {
            auto it = machines.find(fs::to_text(p));
            return it == machines.end() ? std::nullopt : std::optional<std::uint16_t>(it->second);
        };
        pr.holds_drivers = in(drivers);
        pr.is_xex = in(xex);
        pr.is_nes = in(nes);
        pr.is_sms = in(sms);
        pr.is_gb = in(gb);
        pr.is_genesis = in(genesis);
        pr.is_msx = in(msx);
        pr.is_zx = in(zx);
        pr.is_java_module = in(java_modules);
        pr.xex_basefile = [](const std::filesystem::path& p) { return std::filesystem::path(fs::to_text(p) + ".base"); };
        pr.xbe_to_pe = [](const std::filesystem::path& p) { return std::filesystem::path(fs::to_text(p) + ".pe"); };
        return pr;
    }
};

// A registry with every platform added in a scrambled order.
Registry full_registry(const Probes& probes) {
    Registry r(probes);
    r.add(dos());
    r.add(stand_in(Id::Vxd, "le"));
    r.add(stand_in(Id::Win16, "ne", std::nullopt, true));
    r.add(win32());
    r.add(msx());
    r.add(zx());
    r.add(stand_in(Id::Genesis, "genesis", std::nullopt, true));
    r.add(stand_in(Id::Gb, "gb", std::nullopt, true));
    r.add(stand_in(Id::Sms, "sms", std::nullopt, true));
    r.add(stand_in(Id::Nes, "nes", std::nullopt, true));
    r.add(xbe());
    r.add(xbox360());
    r.add(java());
    return r;
}

}  // namespace

CB_TEST("platforms.package.ids", [] {
    const std::vector<std::string> expected = {"java", "xbox360", "xbox", "nes", "sms", "gb", "genesis", "zx", "msx", "win32", "win16", "vxd", "dos"};
    CB_CHECK_EQ(kOrder.size(), expected.size());
    for (std::size_t i = 0; i < kOrder.size(); ++i) {
        CB_CHECK_EQ(std::string(id_text(kOrder[i])), expected[i]);
        CB_CHECK(parse_id(expected[i]) == kOrder[i]);
    }
    CB_CHECK(!parse_id("pe"));        // "pe" is a platform's name, not a descriptor
    CB_CHECK(!parse_id(""));
});

CB_TEST("platforms.package.registry_orders_and_finds", [] {
    Files files;
    Registry r = full_registry(files.probes());
    CB_CHECK(r.complete());
    // harvested from the old harness (all()): [id, name] in the order platforms are tried
    const std::vector<std::pair<std::string, std::string>> expected = {
        {"java", "java"}, {"xbox360", "powerpc"}, {"xbox", "xbe"}, {"nes", "nes"}, {"sms", "sms"}, {"gb", "gb"}, {"genesis", "genesis"},
        {"zx", "zx"},     {"msx", "msx"},         {"win32", "pe"}, {"win16", "ne"}, {"vxd", "le"},   {"dos", "dos"}};
    CB_CHECK_EQ(r.all().size(), expected.size());
    for (std::size_t i = 0; i < expected.size(); ++i) {
        CB_CHECK_EQ(std::string(id_text(r.all()[i].id)), expected[i].first);
        CB_CHECK_EQ(r.all()[i].name, expected[i].second);
    }
    // named(): by name, else by descriptor id (harvested)
    CB_CHECK_EQ(r.named("pe")->name, "pe");
    CB_CHECK_EQ(r.named("win32")->name, "pe");
    CB_CHECK_EQ(r.named("xbox360")->name, "powerpc");
    CB_CHECK_EQ(r.named("xbox")->name, "xbe");
    CB_CHECK_EQ(r.named("vxd")->name, "le");
    CB_CHECK(r.named("mz") == nullptr);
    CB_CHECK(r.named("") == nullptr);
    CB_CHECK_EQ(r.of("win16").name, "ne");
    CB_CHECK_THROWS_AS(UsageError, (void)r.of("nope"));
    // for_machine
    CB_CHECK_EQ(r.for_machine(332)->name, "pe");
    CB_CHECK_EQ(r.for_machine(498)->name, "powerpc");
    CB_CHECK(r.for_machine(34404) == nullptr);
    CB_CHECK(r.for_machine(0) == nullptr);
    CB_CHECK(r.for_machine(-1) == nullptr);
    CB_CHECK(r.for_machine(332 + 65536) == nullptr);       // not a 16-bit machine type
    CB_CHECK_THROWS_AS(UsageError, r.add(java()));
});

CB_TEST("platforms.package.lanes_runners_steps", [] {
    Files files;
    Registry r = full_registry(files.probes());
    const auto lanes = r.lanes();
    CB_CHECK_EQ(lanes.size(), 12u);                         // xbe has no lane: it is handed the PE image
    CB_CHECK(lanes[0] == (std::pair<std::string, std::string>{"java", "java"}));
    CB_CHECK(lanes[1] == (std::pair<std::string, std::string>{"powerpc", "powerpc"}));
    CB_CHECK(lanes[8] == (std::pair<std::string, std::string>{"pe", "win32"}));
    const std::vector<std::string> runners = r.runners();
    // (win32 and powerpc run; the stand-ins above say who else does)
    CB_CHECK(std::find(runners.begin(), runners.end(), "powerpc") != runners.end());
    CB_CHECK(std::find(runners.begin(), runners.end(), "pe") != runners.end());
    CB_CHECK(std::find(runners.begin(), runners.end(), "msx") != runners.end());
    CB_CHECK(std::find(runners.begin(), runners.end(), "dos") == runners.end());
    CB_CHECK(std::find(runners.begin(), runners.end(), "java") == runners.end());   // the JVM runs it, not us
    CB_CHECK(std::find(runners.begin(), runners.end(), "xbe") == runners.end());
    const auto steps = r.step_sets();
    CB_CHECK_EQ(steps.size(), 12u);
    for (const auto& [id, s] : steps) CB_CHECK(s == Steps::Lanes);
});

CB_TEST("platforms.package.win32_descriptor", [] {
    const Platform p = win32();   // harvested from the old harness, field by field
    CB_CHECK_EQ(p.name, "pe");
    CB_CHECK_EQ(p.title, "32-bit Windows");
    CB_CHECK_EQ(p.family, "x86");
    CB_CHECK(p.source == SourceLanguage::C);
    CB_CHECK_EQ(*p.machine, 0x14C);
    CB_CHECK_EQ(*p.lane, "win32");
    CB_CHECK_EQ(p.loader.container, "PE");
    CB_CHECK_EQ(p.cpu.language, "x86:LE:32:default:windows");
    CB_CHECK_EQ(*p.cpu.bits, 32u);
    CB_CHECK(*p.cpu.endian == Endian::Little);
    const auto& cs = std::get<CapstoneDecoder>(p.cpu.decoder);
    CB_CHECK(cs.arch == Arch::X86);
    CB_CHECK_EQ(cs.bits, 32u);
    CB_CHECK_EQ(p.cpu.stack, "ESP");
    CB_CHECK_EQ(p.cpu.registers.size(), 8u);
    CB_CHECK((p.convention.names == std::vector<std::string>{"__stdcall", "__cdecl", "__fastcall", "__thiscall"}));
    CB_CHECK_EQ(std::get<unsigned>(p.convention.arguments), 4u);
    CB_CHECK_EQ(*p.convention.word, 4u);
    CB_CHECK(*p.convention.order == CallOrder::C);
    CB_CHECK(*p.convention.pops == Pops::Either);
    CB_CHECK((p.convention.results == std::vector<std::string>{"EAX", "EDX:EAX"}));
    CB_CHECK((p.convention.preserved == std::vector<std::string>{"EBX", "ESI", "EDI", "EBP"}));
    CB_CHECK(p.emulator.engine == Engine::Unicorn);
    CB_CHECK(p.emulator.runs_code);
    CB_CHECK(*p.compiler.language == Language::C);
    CB_CHECK_EQ(p.compiler.toolchain, "openwatcom");
});

CB_TEST("platforms.package.xbox360_descriptor", [] {
    const Platform p = xbox360();
    CB_CHECK_EQ(p.name, "powerpc");
    CB_CHECK_EQ(p.title, "Xbox 360 (PowerPC)");
    CB_CHECK_EQ(*p.machine, 498);
    CB_CHECK_EQ(p.cpu.language, "PowerPC:BE:64:Xenon:homed");
    CB_CHECK_EQ(*p.cpu.bits, 64u);
    CB_CHECK(*p.cpu.endian == Endian::Big);
    CB_CHECK(std::get<OwnDecoder>(p.cpu.decoder) == OwnDecoder::PowerPc);
    CB_CHECK_EQ(p.cpu.registers.size(), 32u);
    CB_CHECK_EQ(p.cpu.registers[31], "r31");
    CB_CHECK_EQ(p.cpu.stack, "r1");
    CB_CHECK_EQ(p.convention.preserved.size(), 18u);        // r14..r31
    CB_CHECK_EQ(p.convention.preserved.front(), "r14");
    CB_CHECK_EQ(std::get<unsigned>(p.convention.arguments), 8u);
    CB_CHECK((p.convention.results == std::vector<std::string>{"r3", "f1"}));
    CB_CHECK(p.loader.readable == Readable::XexBasefile);
    CB_CHECK_EQ(p.loader.container, "XEX2 -> PE");
    CB_CHECK(p.emulator.engine == Engine::OwnInterpreter);
    CB_CHECK_EQ(p.compiler.toolchain, "the Xbox 360 SDK");
});

CB_TEST("platforms.package.small_descriptors", [] {
    const Platform x = xbe();
    CB_CHECK_EQ(x.name, "xbe");
    CB_CHECK(!x.lane);
    CB_CHECK(x.steps == Steps::None);
    CB_CHECK(!x.machine);
    CB_CHECK(!x.emulator.runs_code);
    CB_CHECK(!x.compiler.language);
    CB_CHECK(x.loader.readable == Readable::XbeToPe);

    const Platform d = dos();
    CB_CHECK_EQ(d.cpu.language, "x86:LE:16:Real Mode:default");
    CB_CHECK_EQ(d.cpu.registers.size(), 12u);
    CB_CHECK(std::holds_alternative<std::monostate>(d.convention.arguments));
    CB_CHECK((d.convention.names == std::vector<std::string>{"__cdecl16near", "__cdecl16far"}));
    CB_CHECK(!d.emulator.runs_code);
    CB_CHECK(d.emulator.engine == Engine::None);
    CB_CHECK_EQ(d.compiler.toolchain, "openwatcom wasm");
    CB_CHECK(*d.source == SourceLanguage::Asm);

    const Platform j = java();
    CB_CHECK_EQ(j.cpu.language, "");
    CB_CHECK(!j.cpu.bits);
    CB_CHECK(std::get<OwnDecoder>(j.cpu.decoder) == OwnDecoder::Jar);
    CB_CHECK(j.emulator.engine == Engine::Jvm);
    CB_CHECK(!j.emulator.runs_code);
    CB_CHECK_EQ(source_text(*j.source), "java -> c++");
    CB_CHECK_EQ(language_text(*j.compiler.language), "c++");
    CB_CHECK(*j.convention.pops == Pops::Callee);
});

CB_TEST("platforms.package.machine_platforms", [] {
    const Platform m = msx();       // harvested
    CB_CHECK_EQ(m.name, "msx");
    CB_CHECK_EQ(m.title, "MSX");
    CB_CHECK_EQ(m.family, "z80");
    CB_CHECK_EQ(*m.lane, "msx");
    CB_CHECK(m.steps == Steps::Lanes);
    CB_CHECK_EQ(m.loader.container, "cartridge image (.rom)");
    CB_CHECK_EQ(m.cpu.language, "z80:LE:16:default");
    CB_CHECK_EQ(*m.cpu.bits, 8u);
    CB_CHECK(std::get<OwnDecoder>(m.cpu.decoder) == OwnDecoder::Z80);
    CB_CHECK_EQ(m.cpu.stack, "SP");
    CB_CHECK((m.cpu.registers == std::vector<std::string>{"A", "F", "BC", "DE", "HL", "IX", "IY", "SP"}));
    CB_CHECK((m.convention.names == std::vector<std::string>{"registers"}));
    CB_CHECK(std::holds_alternative<InRegisters>(m.convention.arguments));
    CB_CHECK(*m.convention.order == CallOrder::Registers);
    CB_CHECK(*m.convention.pops == Pops::Caller);
    CB_CHECK((m.convention.results == std::vector<std::string>{"A", "F", "BC", "DE", "HL", "IX", "IY"}));
    CB_CHECK(m.emulator.engine == Engine::OwnInterpreter);
    CB_CHECK(m.emulator.runs_code);
    CB_CHECK(*m.compiler.language == Language::Asm);
    CB_CHECK_EQ(m.compiler.toolchain, "CodeBreaker's own assembler, or sjasmplus / pasmo");

    const Platform z = zx();
    CB_CHECK_EQ(z.title, "ZX Spectrum");
    CB_CHECK_EQ(z.loader.container, "snapshot or tape (.sna, .z80, .tap)");
    // machine() with the default toolchain
    MachineSpec s;
    s.id = Id::Nes;
    s.title = "NES";
    s.family = "6502";
    s.container = "iNES (.nes)";
    s.results = {"A", "X", "Y", "P"};
    const Platform n = machine(s);
    CB_CHECK_EQ(n.name, "nes");
    CB_CHECK_EQ(n.compiler.toolchain, "CodeBreaker's own assembler");
});

CB_TEST("platforms.package.identify_by_probes", [] {
    Files files;
    files.classes = {{"a.exe", ExeKind::Pe}, {"x360.exe", ExeKind::Pe}, {"w.exe", ExeKind::Ne}, {"d.exe", ExeKind::Dos}, {"drv.vxd", ExeKind::Le}};
    files.machines = {{"a.exe", std::uint16_t{0x14C}}, {"x360.exe", std::uint16_t{0x1F2}}};
    files.kinds = {{"d.exe", ExeKind::Dos}, {"prog.com", ExeKind::Dos}};
    files.nes = {"game.nes"};
    files.sms = {"game.sms"};
    files.gb = {"game.gb"};
    files.genesis = {"game.md"};
    files.msx = {"game.rom"};
    files.zx = {"snap.sna"};
    files.xex = {"game.xex"};
    files.drivers = {"drv.vxd", "vmm32.vxd"};
    fs::ScopedTempDir dir("cb-platforms");
    const std::string folder = fs::to_text(dir.path() / "src");
    fs::make_dirs(folder);
    files.java_modules = {folder, "One.java"};
    const Registry r = full_registry(files.probes());
    auto who = [&](const char* path) { const Platform* p = r.identify(path); return p ? p->name : std::string("none"); };
    CB_CHECK_EQ(who("game.nes"), "nes");
    CB_CHECK_EQ(who("game.sms"), "sms");
    CB_CHECK_EQ(who("game.gb"), "gb");
    CB_CHECK_EQ(who("game.md"), "genesis");
    CB_CHECK_EQ(who("game.rom"), "msx");
    CB_CHECK_EQ(who("snap.sna"), "zx");
    CB_CHECK_EQ(who("game.xex"), "powerpc");
    CB_CHECK_EQ(who("x360.exe"), "powerpc");               // a PE image of the Xenon machine type
    CB_CHECK_EQ(who("a.exe"), "pe");
    CB_CHECK_EQ(who("w.exe"), "ne");
    CB_CHECK_EQ(who("d.exe"), "dos");
    CB_CHECK_EQ(who("prog.com"), "dos");                   // no MZ header, DOS all the same
    CB_CHECK_EQ(who("drv.vxd"), "le");
    CB_CHECK_EQ(who("vmm32.vxd"), "le");
    CB_CHECK_EQ(who("One.java"), "java");
    CB_CHECK_EQ(who(folder.c_str()), "java");              // a folder the Java lane takes
    CB_CHECK_EQ(who(fs::to_text(dir.path()).c_str()), "none");   // a folder it does not
    CB_CHECK_EQ(who("text.txt"), "none");
    CB_CHECK_EQ(who("short.nes"), "none");
    CB_CHECK_EQ(who(""), "none");
    // a .jar is Java by its name, whatever the case, without asking anyone
    CB_CHECK_EQ(who("x.jar"), "java");
    CB_CHECK_EQ(who("Foo.JAR"), "java");
});

CB_TEST("platforms.package.identify_xbe_reads_the_magic", [] {
    fs::ScopedTempDir dir("cb-platforms");
    fs::write_text(dir.path() / "a.xbe", "XBEH-and-the-rest");
    fs::write_text(dir.path() / "b.xbe", "MZ-not-an-xbe");
    fs::write_text(dir.path() / "empty.xbe", "");
    fs::write_text(dir.path() / "three.xbe", "XBE");
    Files files;
    const Registry r = full_registry(files.probes());
    auto who = [&](const char* name) { return r.recognises(r.of("xbox"), dir.path() / name); };
    CB_CHECK(who("a.xbe"));
    CB_CHECK(!who("b.xbe"));
    CB_CHECK(!who("empty.xbe"));                           // harvested: empty.xbe is not one
    CB_CHECK(!who("three.xbe"));
    CB_CHECK(!who("missing.xbe"));                         // an unreadable file is "no"
    // and it is readable as the PE image it becomes
    CB_CHECK_EQ(fs::to_text(r.readable(r.of("xbox"), dir.path() / "a.xbe")), fs::to_text(dir.path() / "a.xbe") + ".pe");
    CB_CHECK_EQ(fs::to_text(r.readable(r.of("xbox"), dir.path() / "b.xbe")), fs::to_text(dir.path() / "b.xbe"));
});

CB_TEST("platforms.package.readable", [] {
    Files files;
    files.xex = {"game.xex"};
    const Registry r = full_registry(files.probes());
    CB_CHECK_EQ(fs::to_text(r.readable(r.of("powerpc"), "game.xex")), "game.xex.base");   // a .xex: its basefile
    CB_CHECK_EQ(fs::to_text(r.readable(r.of("powerpc"), "game.exe")), "game.exe");        // a bare PE: itself
    CB_CHECK_EQ(fs::to_text(r.readable(r.of("pe"), "a.exe")), "a.exe");
});

CB_TEST("platforms.package.probe_failures", [] {
    Files files;
    Probes pr = files.probes();
    // a probe that finds the file damaged or gone answers "no" for that platform
    pr.is_nes = [](const std::filesystem::path&) -> bool { throw FormatError("nes", 4, "bad header"); };
    pr.is_sms = [](const std::filesystem::path& p) -> bool { throw NotFoundError(p, 2); };
    const Registry r = full_registry(pr);
    CB_CHECK(!r.recognises(r.of("nes"), "x.nes"));
    CB_CHECK(!r.recognises(r.of("sms"), "x.sms"));
    // a probe that was never wired is a start-up bug and is loud
    Registry unwired{Probes{}};
    unwired.add(msx());
    CB_CHECK_THROWS_AS(InternalError, (void)unwired.recognises(unwired.of("msx"), "x.rom"));
    // other errors are not "no"
    pr.is_gb = [](const std::filesystem::path&) -> bool { throw ToolError("x", 1, "", "boom"); };
    const Registry r2 = full_registry(pr);
    CB_CHECK_THROWS_AS(ToolError, (void)r2.recognises(r2.of("gb"), "x.gb"));
});
