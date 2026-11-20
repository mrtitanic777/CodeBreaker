// Tests of java/port.hpp: what `java port` makes of each kind of input, the platform words, the version in a jar's name, the project's name, and the
// unpacking of a source jar.
#include "java/port.hpp"

#include "core/crypto.hpp"
#include "core/fs.hpp"
#include "core/io.hpp"
#include "core/testing.hpp"

namespace cb::java::port {
namespace {

namespace stdfs = std::filesystem;

// A stored zip of these (name, text) members: enough of a jar for the planner.
void pt_write_jar(const stdfs::path& path, const std::vector<std::pair<std::string, std::string>>& members) {
    io::ByteWriter w;
    std::vector<std::uint32_t> offsets;
    for (const auto& [name, data] : members) {
        offsets.push_back(static_cast<std::uint32_t>(w.size()));
        w.bytes(as_bytes("PK\x03\x04"));
        w.u16(20);
        w.u16(0);
        w.u16(0);
        w.u32(0);
        w.u32(crypto::crc32(as_bytes(data)));
        w.u32(static_cast<std::uint32_t>(data.size()));
        w.u32(static_cast<std::uint32_t>(data.size()));
        w.u16(static_cast<std::uint16_t>(name.size()));
        w.u16(0);
        w.bytes(as_bytes(name));
        w.bytes(as_bytes(data));
    }
    const std::size_t directory = w.size();
    for (std::size_t i = 0; i < members.size(); ++i) {
        const auto& [name, data] = members[i];
        w.bytes(as_bytes("PK\x01\x02"));
        w.u16(20);
        w.u16(20);
        w.u16(0);
        w.u16(0);
        w.u32(0);
        w.u32(crypto::crc32(as_bytes(data)));
        w.u32(static_cast<std::uint32_t>(data.size()));
        w.u32(static_cast<std::uint32_t>(data.size()));
        w.u16(static_cast<std::uint16_t>(name.size()));
        w.u16(0);
        w.u16(0);
        w.u16(0);
        w.u16(0);
        w.u32(0);
        w.u32(offsets[i]);
        w.bytes(as_bytes(name));
    }
    const std::size_t size = w.size() - directory;
    w.bytes(as_bytes("PK\x05\x06"));
    w.u16(0);
    w.u16(0);
    w.u16(static_cast<std::uint16_t>(members.size()));
    w.u16(static_cast<std::uint16_t>(members.size()));
    w.u32(static_cast<std::uint32_t>(size));
    w.u32(static_cast<std::uint32_t>(directory));
    w.u16(0);
    const Bytes bytes = w.take();
    fs::make_dirs(path.parent_path());
    fs::write_file(path, ByteView(bytes.data(), bytes.size()));
}

bool pt_has(const Plan& p, std::string_view words) {
    for (const std::string& s : p.steps)
        if (s.find(words) != std::string::npos) return true;
    return false;
}

CB_TEST("java.port.the_platform_words", [] {
    CB_CHECK(parse_platform("pc") == Platform::Pc && parse_platform("PC") == Platform::Pc && parse_platform("pc_opengl") == Platform::Pc);
    CB_CHECK(parse_platform("xbox360") == Platform::Xbox360 && parse_platform("Xbox 360") == Platform::Xbox360 && parse_platform("360") == Platform::Xbox360);
    CB_CHECK(!parse_platform("ps3") && !parse_platform(""));
    CB_CHECK(platform_id(Platform::Pc) == "pc_opengl" && platform_id(Platform::Xbox360) == "xbox360");
});

CB_TEST("java.port.the_version_in_a_jars_name", [] {
    CB_CHECK_EQ(version_from_name("a1.2.6"), "a1.2.6");
    CB_CHECK_EQ(version_from_name("minecraft-b1.7.3"), "b1.7.3");
    CB_CHECK_EQ(version_from_name("c0.30_01c"), "c0.30_01c");
    CB_CHECK_EQ(version_from_name("inf-20100618"), "inf-20100618");
    CB_CHECK_EQ(version_from_name("Minecraft 1.2.5"), "1.2.5");
    CB_CHECK_EQ(version_from_name("deobfuscated"), "");
    CB_CHECK_EQ(version_from_name("game2"), "");   // (a lone number is not a version)
});

CB_TEST("java.port.the_projects_name_is_an_identifier", [] {
    CB_CHECK_EQ(project_name("a1.2.6"), "a1_2_6");
    CB_CHECK_EQ(project_name("My Game"), "My_Game");
    CB_CHECK_EQ(project_name("1.2.5"), "P1_2_5");
});

CB_TEST("java.port.a_source_folder_is_converted_beside_itself", [] {
    fs::ScopedTempDir dir{"cb_port_tree"};
    const stdfs::path tree = dir.path() / "MyGame";
    fs::write_text(tree / "net" / "game" / "Main.java", "class Main {}");
    const Plan p = plan(tree, Platform::Pc);
    CB_CHECK(p.problem.empty());
    CB_CHECK(p.kind == SourceKind::JavaTree);
    CB_CHECK(p.java_src && fs::to_text(*p.java_src).find("MyGame") != std::string::npos);
    CB_CHECK(p.root == dir.path() / "MyGame-converted" && p.project == p.root / "MyGame");
    CB_CHECK(pt_has(p, "for PC") && pt_has(p, "CMake") && !pt_has(p, "Visual Studio") && !pt_has(p, "Decompile"));
    const Plan x = plan(tree, Platform::Xbox360);
    CB_CHECK(pt_has(x, "Visual Studio") && pt_has(x, "achievements") && pt_has(x, "MSBuild") && !pt_has(x, "audio"));   // (no sounds to bank)
    CB_CHECK(plan(tree, Platform::Pc, dir.path() / "elsewhere").root == dir.path() / "elsewhere");
});

CB_TEST("java.port.a_decompiled_game_folder_brings_its_jar_and_sounds", [] {
    fs::ScopedTempDir dir{"cb_port_game"};
    const stdfs::path base = dir.path() / "mcp";
    fs::write_text(base / "src" / "net" / "minecraft" / "client" / "Minecraft.java", "class Minecraft {}");
    pt_write_jar(base / "jars" / "minecraft.jar", {{"terrain.png", "png"}, {"net/minecraft/client/Minecraft.class", "cafe"}});
    fs::write_text(base / "game" / "resources" / "newsound" / "step" / "grass1.ogg", "ogg");
    const Plan p = plan(base, Platform::Xbox360);
    CB_CHECK(p.problem.empty());
    CB_CHECK(p.kind == SourceKind::GameFolder);
    CB_CHECK(p.java_src && p.jar && p.resources);
    CB_CHECK(pt_has(p, "Content") && pt_has(p, "audio banks") && !pt_has(p, "Decompile"));
});

CB_TEST("java.port.a_jar_is_unpacked_or_decompiled", [] {
    fs::ScopedTempDir dir{"cb_port_jar"};
    {   // a jar of source: unpacked, nothing decompiled
        const stdfs::path jar = dir.path() / "one" / "game-sources.jar";
        pt_write_jar(jar, {{"net/game/Main.java", "class Main {}"}, {"net/game/Util.java", "class Util {}"}, {"META-INF/MANIFEST.MF", "x"}});
        const Plan p = plan(jar, Platform::Pc);
        CB_CHECK(p.problem.empty() && p.kind == SourceKind::SourceJar && pt_has(p, "Unpack"));
        CB_CHECK(count_jar(jar).java == 2 && count_jar(jar).classes == 0);
        const stdfs::path out = dir.path() / "one" / "unpacked";
        CB_CHECK_EQ(extract_java_sources(jar, out), 2u);
        CB_CHECK_EQ(fs::read_text(out / "net" / "game" / "Main.java"), "class Main {}");
    }
    {   // a game jar named after its version: decompiled first
        const stdfs::path jar = dir.path() / "two" / "a1.2.6.jar";
        pt_write_jar(jar, {{"net/minecraft/client/Minecraft.class", "cafe"}, {"terrain.png", "png"}});
        const Plan p = plan(jar, Platform::Xbox360);
        CB_CHECK(p.problem.empty() && p.kind == SourceKind::ClassJar);
        CB_CHECK_EQ(p.mc_version, "a1.2.6");
        CB_CHECK(p.jar && *p.jar == p.input && pt_has(p, "Decompile Minecraft a1.2.6") && pt_has(p, "Content"));
        CB_CHECK(p.root == dir.path() / "two" / "a1.2.6-converted" && p.name == "a1_2_6");
    }
    {   // a game jar whose name says nothing: it says what to drop instead
        const stdfs::path jar = dir.path() / "three" / "game.jar";
        pt_write_jar(jar, {{"Main.class", "cafe"}});
        const Plan p = plan(jar, Platform::Pc);
        CB_CHECK(!p.problem.empty() && p.problem.find("decompiled game folder") != std::string::npos);
    }
});

CB_TEST("java.port.what_is_not_a_java_program_is_said", [] {
    fs::ScopedTempDir dir{"cb_port_none"};
    fs::write_text(dir.path() / "empty" / "readme.txt", "x");
    CB_CHECK(!plan(dir.path() / "empty", Platform::Pc).problem.empty());
    CB_CHECK(!plan(dir.path() / "missing", Platform::Pc).problem.empty());
    fs::write_text(dir.path() / "notes.txt", "x");
    CB_CHECK(!plan(dir.path() / "notes.txt", Platform::Pc).problem.empty());
    fs::write_text(dir.path() / "broken.jar", "not a zip");
    CB_CHECK(plan(dir.path() / "broken.jar", Platform::Pc).problem.find("cannot be read") != std::string::npos);
});

}  // namespace
}  // namespace cb::java::port
