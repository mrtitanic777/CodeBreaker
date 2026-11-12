// The shims at the old header paths for java/console/{platform_config, platform_refs} and
// java/projects/minecraft/{platform_coverage, render_fix, sound_fix, applet_ping, edition} and the six edition profiles:
// they must compile with the old layer and forward to the new code. Built at /W3 (the old layer is not warning-clean).
// (The legacy patch() of the three fix passes works on the old translate IR and is compiled, not run.)
#include <string>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "codebreaker/java/console/platform_config.hpp"
#include "codebreaker/java/console/platform_refs.hpp"
#include "codebreaker/java/projects/minecraft/applet_ping.hpp"
#include "codebreaker/java/projects/minecraft/edition.hpp"
#include "codebreaker/java/projects/minecraft/platform_coverage.hpp"
#include "codebreaker/java/projects/minecraft/profiles/minecraft_alpha.hpp"
#include "codebreaker/java/projects/minecraft/profiles/minecraft_beta.hpp"
#include "codebreaker/java/projects/minecraft/profiles/minecraft_indev.hpp"
#include "codebreaker/java/projects/minecraft/profiles/minecraft_infdev.hpp"
#include "codebreaker/java/projects/minecraft/profiles/minecraft_release.hpp"
#include "codebreaker/java/projects/minecraft/profiles/minecraft_rubydung.hpp"
#include "codebreaker/java/projects/minecraft/render_fix.hpp"
#include "codebreaker/java/projects/minecraft/sound_fix.hpp"

namespace {

namespace jv = codebreaker::java;
namespace mc = codebreaker::java::projects::minecraft;
namespace stdfs = std::filesystem;
using cb::compat::from_py;
using cb::compat::to_py;

CB_TEST("java.shims_w1b18.edition_profiles", [] {
    namespace p = mc::profiles;
    CB_CHECK_EQ(from_py(p::minecraft_alpha::PROFILE().name), "minecraft_alpha");
    CB_CHECK_EQ(from_py(p::minecraft_beta::PROFILE().name), "minecraft_beta");
    CB_CHECK_EQ(from_py(p::minecraft_release::PROFILE().name), "minecraft_release");
    CB_CHECK_EQ(from_py(p::minecraft_indev::PROFILE().name), "minecraft_indev");
    CB_CHECK_EQ(from_py(p::minecraft_infdev::PROFILE().name), "minecraft_infdev");
    CB_CHECK_EQ(from_py(p::minecraft_rubydung::PROFILE().name), "minecraft_rubydung");
    CB_CHECK_EQ(p::minecraft_release::PROFILE().package_roles.size(), 19);
    CB_CHECK_EQ(p::minecraft_indev::PROFILE().static_init_order.size(), 4);
    CB_CHECK_EQ(p::minecraft_rubydung::PROFILE().nullable_getters.size(), 0);
    CB_CHECK(p::minecraft_infdev::PROFILE().nullable_getters.contains(py::str("getChunk")));
    CB_CHECK_EQ(from_py(p::minecraft_infdev::PROFILE().nullable_getter_return_type.at(py::str("getChunk"))), "Chunk");
    CB_CHECK_EQ(p::minecraft_alpha::PROFILE().vcxproj_filters.size(), 55);
    CB_CHECK(p::minecraft_alpha::PROFILE().dead_types.has_value());
    CB_CHECK_EQ(p::minecraft_beta::PROFILE().heavy_types.size(), 4);
    CB_CHECK(!p::minecraft_beta::PROFILE().optional_types.has_value());
    // the profile is a value built once: the same record each time
    CB_CHECK(&p::minecraft_alpha::PROFILE() == &p::minecraft_alpha::PROFILE());
});

CB_TEST("java.shims_w1b18.edition", [] {
    namespace e = mc::edition;
    CB_CHECK_EQ(e::EDITIONS.size(), 6);
    CB_CHECK_EQ(from_py(e::EDITIONS.at(py::str("alpha"))), "minecraft_alpha");
    CB_CHECK_EQ(from_py(*e::edition_for_version(py::str("b1.6.6"))), "beta");
    CB_CHECK(!e::edition_for_version(std::nullopt).has_value());
    CB_CHECK(!e::edition_for_version(py::str("zzz")).has_value());
    CB_CHECK_EQ(from_py(*e::profile_name(py::str("indev"))), "minecraft_indev");
    CB_CHECK(!e::profile_name(py::str("minecraft_indev")).has_value());   // only the short names are keys
    CB_CHECK(!e::profile_name(std::nullopt).has_value());
    CB_CHECK_EQ(from_py(*e::resolve(py::str("beta"), py::str("a1.2.6"))), "minecraft_beta");
    CB_CHECK_EQ(from_py(*e::resolve(py::str("minecraft_alpha"))), "minecraft_alpha");
    CB_CHECK_EQ(from_py(*e::resolve(std::nullopt, py::str("in-20100223"))), "minecraft_indev");
    CB_CHECK(!e::resolve().has_value());
    bool threw = false;
    try {
        e::resolve(py::str("gamma"));
    } catch (const py::ValueError&) {
        threw = true;   // an unknown edition is a ValueError for the old callers
    }
    CB_CHECK(threw);
    // classes: the corpus decides
    py::list<e::ClassInfo> classes;
    classes.append(std::make_shared<jv::ir::JClass>(py::str("Chunk"), std::optional<py::str>(py::str("net.minecraft.src"))));
    classes.append(std::make_shared<jv::ir::JClass>(py::str("Block")));
    CB_CHECK_EQ(from_py(*e::detect_edition(classes)), "alpha");
    CB_CHECK_EQ(from_py(*e::resolve(std::nullopt, std::nullopt, classes)), "minecraft_alpha");
    CB_CHECK(!e::detect_edition(std::nullopt).has_value());
    CB_CHECK(!e::detect_edition(py::list<e::ClassInfo>()).has_value());
});

CB_TEST("java.shims_w1b18.fix_passes_text_form", [] {
    const std::string setlevel =
        "void Minecraft::setLevel(World* var1)\n{\n\ttry {\n\tthis->a->getDocumentBase()->openStream();\n"
        "\tif(this->ok()) {\n\tthis->theWorld = var1;\n\t}\n\t} catch (Throwable*) {\n\t}\n}\n";
    auto [ping_text, ping_n] = mc::applet_ping::process(to_py(setlevel));
    CB_CHECK_EQ(ping_n, 1);
    CB_CHECK(from_py(ping_text).find("this->theWorld = var1;") != std::string::npos);

    auto [sfx_text, sfx_n, music_n] = mc::sound_fix::process(to_py(
        "SoundPoolEntry* v = this->soundPoolSounds->getRandomSoundFromSoundPool(n);\nif(v != NULL) {\nplay(v->soundName);\n}\n"));
    CB_CHECK_EQ(sfx_n, 1);
    CB_CHECK_EQ(music_n, 0);
    CB_CHECK(from_py(sfx_text).find("play(n);") != std::string::npos);

    auto [hurt_text, update_n, hurt_n] =
        mc::render_fix::process(to_py("if(var3 >= 0.0F) {\n\tvar3 /= (float)var2->maxHurtTime;\n"));
    CB_CHECK_EQ(update_n, 0);
    CB_CHECK_EQ(hurt_n, 1);
    CB_CHECK(from_py(hurt_text).find("var2->hurtTime > 0") != std::string::npos);

    // run / stranded over a folder, with the old result records
    cb::fs::ScopedTempDir dir("cb-w1b18");
    cb::fs::write_text(dir.path() / "Minecraft.cpp", setlevel);
    cb::fs::write_text(dir.path() / "Sound.cpp",
                       "SoundPoolEntry* v = this->soundPoolMusic->getRandomSound();\nif(v != NULL) {\nplay();\n}\n");
    cb::fs::write_text(dir.path() / "Render.cpp", "if(var3 >= 0.0F) {\n\tvar3 /= (float)var2->maxHurtTime;\n");
    const py::str d = to_py(cb::fs::to_text(dir.path()));
    auto ping = mc::applet_ping::run(d);
    CB_CHECK_EQ(ping.sites, 1);
    CB_CHECK_EQ(ping.files.size(), 1);
    CB_CHECK_EQ(from_py(ping.files[0].file), "Minecraft.cpp");
    CB_CHECK_EQ(ping.stranded.size(), 0);
    auto sound = mc::sound_fix::run(d);
    CB_CHECK_EQ(sound.sites, 1);
    CB_CHECK_EQ(sound.files[0].music, 1);
    auto render = mc::render_fix::run(d);
    CB_CHECK_EQ(render.hurt, 1);
    CB_CHECK_EQ(render.files[0].hurt, 1);
    CB_CHECK_EQ(mc::applet_ping::stranded(d).size(), 0);
    CB_CHECK_EQ(mc::sound_fix::stranded(d).size(), 0);
    CB_CHECK_EQ(mc::render_fix::stranded(d).size(), 0);
});

CB_TEST("java.shims_w1b18.platform_refs", [] {
    namespace pr = jv::console::platform_refs;
    auto main_class = std::make_shared<jv::ir::JClass>(py::str("RubyDung"));
    main_class->methods.append(std::make_shared<jv::ir::Method>(
        py::str("main"), std::make_shared<jv::ir::JType>(py::str("void")),
        py::list<py::ref<jv::ir::Param>>{std::make_shared<jv::ir::Param>(py::str("args"), std::make_shared<jv::ir::JType>(py::str("String"), 1))},
        jv::ir::PACKAGE, true));
    py::list<py::ref<jv::ir::JClass>> classes;
    classes.append(std::make_shared<jv::ir::JClass>(py::str("Level")));
    classes.append(main_class);
    CB_CHECK_EQ(from_py(*pr::detect_main_class(classes)), "RubyDung");
    CB_CHECK(!pr::detect_main_class(py::list<py::ref<jv::ir::JClass>>{classes[0]}).has_value());
    CB_CHECK(pr::REFERENCE_SKIP.contains(py::str("ZipFile")));
    CB_CHECK_EQ(pr::REFERENCE_REPLACES.at(py::str("Input")).size(), 3);
    CB_CHECK(from_py(pr::generate_entry_point(py::str("RubyDung"))).find("RubyDung::main(args);") != std::string::npos);
    CB_CHECK(from_py(pr::generate_pc_entry_point(py::str("RubyDung"))).find("j2xThreadsWait();") != std::string::npos);

    // a Runnable shell: the old class comes back, found by position
    auto shell = std::make_shared<jv::ir::JClass>(py::str("Minecraft"), std::nullopt, std::nullopt, py::list<py::str>{py::str("Runnable")});
    shell->methods.append(std::make_shared<jv::ir::Method>(py::str("run"), std::make_shared<jv::ir::JType>(py::str("void"))));
    shell->methods.append(std::make_shared<jv::ir::Method>(
        py::str("Minecraft"), nullptr,
        py::list<py::ref<jv::ir::Param>>{std::make_shared<jv::ir::Param>(py::str("w"), std::make_shared<jv::ir::JType>(py::str("int"))),
                                         std::make_shared<jv::ir::Param>(py::str("c"), std::make_shared<jv::ir::JType>(py::str("Canvas")))},
        jv::ir::PACKAGE, false, false, false, true));
    py::list<py::ref<jv::ir::JClass>> shells;
    shells.append(std::make_shared<jv::ir::JClass>(py::str("Level")));
    shells.append(shell);
    CB_CHECK(pr::detect_runnable_game_class(shells) == shell);
    CB_CHECK(pr::detect_runnable_game_class(classes) == nullptr);
    CB_CHECK_EQ(from_py(pr::_runnable_ctor_args(shell)), "SCREEN_WIDTH, NULL");

    // emit + verify + audit over a small layer
    cb::fs::ScopedTempDir dir("cb-w1b18");
    const stdfs::path ref = dir.path() / "reference";
    const stdfs::path out = dir.path() / "out";
    cb::fs::make_dirs(ref);
    cb::fs::make_dirs(out);
    cb::fs::write_text(ref / "GL11.reference.h", "#pragma once\n");
    cb::fs::write_text(ref / "GL11.reference.cpp", "// gl\n");
    cb::fs::write_text(ref / "GLU.reference.cpp", "// glu without a header\n");
    const py::str refs = to_py(cb::fs::to_text(ref));
    py::set<py::str> game;
    game.add(py::str("Textures"));
    const py::set<py::str> emitted = pr::emit_references(to_py(cb::fs::to_text(out)), refs, game, std::optional<py::str>(py::str("RubyDung")));
    CB_CHECK(emitted.contains(py::str("GL11")) && emitted.contains(py::str("Main")));
    CB_CHECK(pr::verify_integrated(to_py(cb::fs::to_text(out)), emitted).size() == 1);   // Main.cpp has no Main.h
    CB_CHECK_EQ(pr::emit_references_last_skipped.size(), 0);
    CB_CHECK(pr::available_references(refs).contains(py::str("GL11")));
    const auto audit = pr::audit_references(refs);
    CB_CHECK(audit.complete.contains(py::str("GL11")));
    CB_CHECK_EQ(audit.orphans.size(), 1);
    CB_CHECK_EQ(from_py(audit.orphans[0]), "GLU");
});

CB_TEST("java.shims_w1b18.platform_coverage", [] {
    namespace pcv = mc::platform_coverage;
    cb::fs::ScopedEnv checkout("CB_CHECKOUT_ROOT", cb::fs::to_text(cb::testing::data_path("../..")));   // (the settings root)
    cb::fs::ScopedTempDir dir("cb-w1b18");
    cb::fs::write_text(dir.path() / "GL11.h", "namespace GL11 { inline void glBegin(int) {} }\n");
    cb::fs::write_text(dir.path() / "Game.cpp", "void f() { GL11::glBegin(1); GL11::glMissing(); }\n");
    py::set<py::str> runtime;
    runtime.add(py::str("GL11.h"));
    const py::str d = to_py(cb::fs::to_text(dir.path()));
    auto results = pcv::analyze(d, std::nullopt, runtime);
    CB_CHECK_EQ(results.size(), 8);
    CB_CHECK_EQ(from_py(results[0].sub.key), "video");
    CB_CHECK_EQ(results[0].symbols.size(), 2);
    CB_CHECK_EQ(results[0].stub.size(), 1);
    CB_CHECK_EQ(results[0].missing.size(), 1);
    CB_CHECK_EQ(from_py(std::get<0>(results[0].missing[0])), "GL11::glMissing");
    CB_CHECK_EQ(from_py(std::get<2>(results[0].missing[0])), "MISSING");
    CB_CHECK(!results[0].ref_exists);
    // (write_report reads the xbox360_sdk setting through the sdk shim, as the old module did)
    auto written = pcv::write_report(d, std::nullopt, runtime, std::optional<py::str>(to_py(cb::fs::to_text(dir.path() / "no_sdk"))));
    CB_CHECK(from_py(std::get<0>(written)).ends_with("PLATFORM_COVERAGE.md"));
    CB_CHECK_EQ(std::get<1>(written).size(), 8);
    CB_CHECK(stdfs::exists(dir.path() / "PLATFORM_COVERAGE.md"));
});

CB_TEST("java.shims_w1b18.platform_config", [] {
    namespace pcf = jv::console::platform_config;
    cb::fs::ScopedEnv checkout("CB_CHECKOUT_ROOT", cb::fs::to_text(cb::testing::data_path("../..")));   // (the settings root)
    pcf::Config cfg = pcf::default_config();
    CB_CHECK_EQ(from_py(cfg.controller.layout), "default");
    CB_CHECK_EQ(from_py(cfg.keyboard), "xshowkeyboard");
    CB_CHECK_EQ(cfg.controller.binds.size(), 15);
    CB_CHECK_EQ(cfg.subsystems.size(), 7);
    CB_CHECK(from_py(pcf::_choices_md(cfg)).find("# PLATFORM_CHOICES.md") != std::string::npos);

    cb::fs::ScopedTempDir dir("cb-w1b18");
    const py::str d = to_py(cb::fs::to_text(dir.path()));
    py::dict<py::str, pcf::Update> updates;
    updates.set(py::str("layout"), pcf::Update(py::str("southpaw")));
    updates.set(py::str("invert_y"), pcf::Update(true));
    updates.set(py::str("look_sensitivity"), pcf::Update(int64_t(2)));
    updates.set(py::str("keyboard"), pcf::Update(py::str("stub")));
    updates.set(py::str("storage"), pcf::Update(py::str("memory")));
    updates.set(py::str("bind.jump"), pcf::Update(py::str("X")));
    updates.set(py::str("subsystem.audio"), pcf::Update(py::str("silent")));
    // listing applies the updates and writes nothing
    auto listed = pcf::run(d, std::nullopt, updates, true);
    CB_CHECK_EQ(std::get<1>(listed).size(), 0);
    CB_CHECK_EQ(from_py(std::get<0>(listed).controller.layout), "southpaw");
    // running writes the four files; no SDK is needed for stub + memory
    auto ran = pcf::run(d, std::nullopt, updates, false, std::optional<py::str>(py::str("xbox360")));
    CB_CHECK_EQ(std::get<1>(ran).size(), 4);
    CB_CHECK(!std::get<2>(ran).has_value());
    CB_CHECK(stdfs::exists(dir.path() / "Kernel.cpp") && stdfs::exists(dir.path() / "platform.config.json"));
    const pcf::Config loaded = pcf::load_config(to_py(cb::fs::to_text(dir.path() / "platform.config.json")));
    CB_CHECK_EQ(from_py(loaded.controller.layout), "southpaw");
    CB_CHECK(loaded.controller.invert_y);
    CB_CHECK_EQ(from_py(loaded.controller.binds.at(py::str("jump"))), "X");
    CB_CHECK_EQ(from_py(loaded.subsystems.at(py::str("audio"))), "silent");
    CB_CHECK(std::holds_alternative<double>(loaded.controller.look_sensitivity));
    // an unknown medium is a ValueError for the old callers
    py::dict<py::str, pcf::Update> bad;
    bad.set(py::str("keyboard"), pcf::Update(py::str("typewriter")));
    bool threw = false;
    try {
        pcf::run(d, std::nullopt, bad, true);
    } catch (const py::ValueError&) {
        threw = true;
    }
    CB_CHECK(threw);
});

}  // namespace
