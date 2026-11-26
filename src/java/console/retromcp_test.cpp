// Tests of java/console/retromcp.hpp: a RetroMCP workspace made in the test, in the shape RetroMCP leaves (options.cfg, conf/, libraries/ with its
// maven tree, -sources jars and natives, minecraft/ with src, src_original, jars, md5, game), found from every folder a person might drop.
#include "java/console/retromcp.hpp"

#include "java/console/test_support.hpp"

namespace {

using namespace cb;
using namespace cb::java::console;
namespace stdfs = std::filesystem;

// A small client workspace of Minecraft in-20100213 under `root`; nothing in it is the game's (one-line classes, empty jars).
void rm_make_workspace(const stdfs::path& root) {
    fs::write_text(root / "options.cfg", "side=ANY\nlang=ENGLISH\nsetup=in-20100213\nresources=false\n");
    fs::write_text(root / "conf" / "version.json", R"({"id": "in-20100213", "mainClass": "org.mcphackers.launchwrapper.Launch", "assetIndex": {"id": "20100212"}})");
    fs::write_text(root / "conf" / "mappings.tiny", "tiny\t2\t0\tnamed\tclient\n");
    fs::write_text(root / "libraries" / "org" / "lwjgl" / "lwjgl" / "lwjgl" / "2.9.4" / "lwjgl-2.9.4.jar", "PK");
    fs::write_text(root / "libraries" / "com" / "paulscode" / "soundsystem" / "20120107" / "soundsystem-20120107.jar", "PK");
    fs::write_text(root / "libraries" / "com" / "paulscode" / "soundsystem" / "20120107" / "soundsystem-20120107-sources.jar", "PK");
    fs::write_text(root / "libraries" / "net" / "java" / "jinput" / "jinput-platform" / "2.0.5" / "jinput-platform-2.0.5-natives-windows.jar", "PK");
    fs::write_text(root / "libraries" / "natives" / "lwjgl.dll", "MZ");
    fs::write_text(root / "libraries" / "natives" / "META-INF" / "x.jar", "PK");
    const stdfs::path mc = root / "minecraft";
    fs::write_text(mc / "src" / "net" / "minecraft" / "client" / "CanvasMinecraftApplet.java",
                   "package net.minecraft.client;\npublic class CanvasMinecraftApplet extends MinecraftApplet {}\n");
    fs::write_text(mc / "src" / "net" / "minecraft" / "client" / "MinecraftApplet.java",
                   "package net.minecraft.client;\nimport java.applet.Applet;\npublic class MinecraftApplet extends Applet {}\n");
    fs::write_text(mc / "src" / "net" / "minecraft" / "client" / "Minecraft.java", "package net.minecraft.client;\npublic class Minecraft implements Runnable {}\n");
    fs::write_text(mc / "src" / "util" / "MathHelper.java", "package util;\npublic class MathHelper {}\n");
    fs::write_text(mc / "src_original" / "net" / "minecraft" / "client" / "Minecraft.java", "package net.minecraft.client;\npublic class Minecraft {}\n");
    fs::write_text(mc / "jars" / "deobfuscated.jar", "PK");
    fs::write_text(mc / "jars" / "deobfuscated-source.jar", "PK");
    fs::write_text(mc / "md5" / "original.md5", "util/MathHelper 00\n");
    fs::write_text(mc / "game" / "assets" / "indexes" / "20100212.json", "{}");
    fs::write_text(mc / ".classpath", "<classpath/>");
}

CB_TEST("java.console.retromcp.a_workspace_is_found_from_any_folder_inside_it", [] {
    fs::ScopedTempDir tmp("cbrmcp");
    const stdfs::path root = tmp.path() / "Downloads";
    rm_make_workspace(root);
    const stdfs::path mc = root / "minecraft";
    for (const stdfs::path& dropped : {root, mc, mc / "src", mc / "src" / "net" / "minecraft" / "client", mc / "src_original" / "net", mc / "game" / "assets",
                                       mc / "jars", root / "conf", root / "libraries" / "natives", mc / "src" / "util" / "MathHelper.java"}) {
        const std::optional<retromcp::Workspace> w = retromcp::find(dropped);
        CB_CHECK(w.has_value());
        if (!w) continue;
        CB_CHECK_PATH(w->root, root);
        CB_CHECK_PATH(w->project, mc);
        CB_CHECK_EQ(w->side, "client");
        CB_CHECK_PATH(w->java_src, mc / "src");   // the whole source root, never src_original, never a package
        CB_CHECK_EQ(w->java_files, std::size_t{4});
        CB_CHECK(w->original_src && *w->original_src == mc / "src_original");
        CB_CHECK(w->jar && *w->jar == mc / "jars" / "deobfuscated.jar");
        CB_CHECK(w->natives && *w->natives == root / "libraries" / "natives");
        CB_CHECK_EQ(w->version, "in-20100213");
        CB_CHECK_EQ(w->launcher_main, "org.mcphackers.launchwrapper.Launch");
        CB_CHECK_EQ(w->entry_class, "net.minecraft.client.MinecraftApplet");
        // the libraries: neither -sources nor natives jars, nor anything under natives/, in path order
        CB_CHECK_EQ(w->libraries.size(), std::size_t{2});
        if (w->libraries.size() == 2) {
            CB_CHECK_PATH(w->libraries[0], root / "libraries" / "com" / "paulscode" / "soundsystem" / "20120107" / "soundsystem-20120107.jar");
            CB_CHECK_PATH(w->libraries[1], root / "libraries" / "org" / "lwjgl" / "lwjgl" / "lwjgl" / "2.9.4" / "lwjgl-2.9.4.jar");
        }
    }
});

CB_TEST("java.console.retromcp.a_main_method_is_the_entry_before_an_applet", [] {
    fs::ScopedTempDir tmp("cbrmcp");
    const stdfs::path src = tmp.path() / "src";
    fs::write_text(src / "a" / "Game.java", "package a;\npublic class Game extends Applet {}\n");
    fs::write_text(src / "z" / "Start.java", "package z;\npublic class Start {\n    public static void main(String[] args) {}\n}\n");
    CB_CHECK_EQ(retromcp::find_entry_class(src), "z.Start");
    fs::ScopedTempDir none("cbrmcp");
    fs::write_text(none.path() / "A.java", "class A {}");
    CB_CHECK_EQ(retromcp::find_entry_class(none.path()), "");
});

CB_TEST("java.console.retromcp.the_server_project_and_what_is_not_a_workspace", [] {
    fs::ScopedTempDir tmp("cbrmcp");
    const stdfs::path root = tmp.path() / "ws";
    rm_make_workspace(root);
    fs::write_text(root / "minecraft_server" / "src" / "net" / "minecraft" / "server" / "MinecraftServer.java",
                   "package net.minecraft.server;\npublic class MinecraftServer {\n    public static void main(String[] a) {}\n}\n");
    const auto server = retromcp::find(root / "minecraft_server" / "src" / "net");
    CB_CHECK(server && server->side == "server" && server->entry_class == "net.minecraft.server.MinecraftServer");
    CB_CHECK(server && !server->jar);   // (no jars/ in it)
    const auto at_root = retromcp::find(root);   // dropped at the root: the client
    CB_CHECK(at_root && at_root->side == "client");

    // no options.cfg and no conf/version.json: a folder of Java source, not a workspace
    fs::ScopedTempDir plain("cbrmcp");
    fs::write_text(plain.path() / "minecraft" / "src" / "A.java", "class A {}");
    CB_CHECK(!retromcp::find(plain.path()).has_value());
    CB_CHECK(!retromcp::find(plain.path() / "minecraft" / "src").has_value());
    // the markers with no project holding source: not one either
    fs::ScopedTempDir empty("cbrmcp");
    fs::write_text(empty.path() / "options.cfg", "setup=a1.2.6\n");
    fs::make_dirs(empty.path() / "minecraft" / "src");
    CB_CHECK(!retromcp::find(empty.path()).has_value());
    // without options.cfg, conf/version.json alone marks it; the version is then the manifest's
    fs::ScopedTempDir manifest_only("cbrmcp");
    fs::write_text(manifest_only.path() / "conf" / "version.json", R"({"id": "b1.7.3"})");
    fs::write_text(manifest_only.path() / "minecraft" / "src" / "A.java", "class A {}");
    const auto m = retromcp::find(manifest_only.path() / "minecraft");
    CB_CHECK(m && m->version == "b1.7.3" && m->launcher_main.empty());
});

CB_TEST("java.console.retromcp.the_downloaded_sounds_are_copied_by_their_names", [] {
    fs::ScopedTempDir tmp("cbrmcp");
    const stdfs::path root = tmp.path() / "ws";
    rm_make_workspace(root);
    fs::write_text(root / "conf" / "version.json", R"({"id": "in-20100213", "assetIndex": {"id": "20100212"}})");
    const stdfs::path assets = root / "minecraft" / "game" / "assets";
    fs::write_text(assets / "indexes" / "20100212.json",
                   R"({"objects": {"newsound/step/grass1.ogg": {"hash": "aa11", "size": 3}, "sound/step/stone1.ogg": {"hash": "bb22", "size": 4},
                                   "../escape.ogg": {"hash": "cc33", "size": 1}},
                       "custom": {"music/calm1.ogg": {"hash": "dd44", "size": 9, "url": "https://example.invalid/dd44"}}})");
    fs::write_text(assets / "objects" / "aa" / "aa11", "one");
    fs::write_text(assets / "objects" / "bb" / "bb22", "four");
    fs::write_text(assets / "objects" / "cc" / "cc33", "x");
    const std::optional<retromcp::Workspace> w = retromcp::find(root / "minecraft");
    CB_CHECK(w && w->asset_index && *w->asset_index == assets / "indexes" / "20100212.json");
    if (!w) return;
    const stdfs::path dest = tmp.path() / "Content" / "resources";
    const retromcp::ResourceCopy c = retromcp::copy_resources(*w, dest);
    CB_CHECK_EQ(c.copied, std::size_t{2});
    CB_CHECK_EQ(c.missing, std::size_t{1});   // the music RetroMCP never downloaded
    CB_CHECK_EQ(fs::read_text(dest / "newsound" / "step" / "grass1.ogg"), "one");
    CB_CHECK_EQ(fs::read_text(dest / "sound" / "step" / "stone1.ogg"), "four");
    CB_CHECK(!stdfs::exists(tmp.path() / "Content" / "escape.ogg"));   // never outside the folder
    // with a fetcher: the music the workspace never downloaded is fetched from its url, checked by hash, kept in the workspace, copied
    fs::write_text(assets / "indexes" / "20100212.json",
                   R"({"objects": {"newsound/step/grass1.ogg": {"hash": "aa11", "size": 3}},
                       "custom": {"music/calm1.ogg": {"hash": "3a01be17246d588caf9a649f8a04e3e5d629db94", "size": 5, "url": "https://example.invalid/calm1"},
                                  "music/calm2.ogg": {"hash": "0000000000000000000000000000000000000000", "size": 5, "url": "https://example.invalid/calm2"},
                                  "index.txt": {"hash": "1111111111111111111111111111111111111111", "size": 1, "url": "https://example.invalid/index"}}})");
    std::vector<std::string> asked;
    const retromcp::Fetch fake = [&asked](const std::string& url) {
        asked.push_back(url);
        return std::string("music");   // SHA-1 3a01be17... for calm1; not calm2's hash
    };
    const std::optional<retromcp::Workspace> w2 = retromcp::find(root);
    CB_CHECK(w2.has_value());
    if (w2) {
        const stdfs::path dest2 = tmp.path() / "Content2" / "resources";
        const retromcp::ResourceCopy f = retromcp::copy_resources(*w2, dest2, fake);
        CB_CHECK(asked == std::vector<std::string>({"https://example.invalid/calm1", "https://example.invalid/calm2"}));   // (never index.txt)
        CB_CHECK_EQ(f.fetched, std::size_t{1});
        CB_CHECK_EQ(f.copied, std::size_t{2});
        CB_CHECK_EQ(f.missing, std::size_t{1});    // calm2 (its bytes were not the index's); index.txt is no sound, so not missed
        CB_CHECK_EQ(f.failed.size(), std::size_t{1});
        CB_CHECK_EQ(fs::read_text(dest2 / "music" / "calm1.ogg"), "music");
        CB_CHECK(stdfs::is_regular_file(assets / "objects" / "3a" / "3a01be17246d588caf9a649f8a04e3e5d629db94"));   // kept in the workspace
        CB_CHECK(!stdfs::exists(dest2 / "music" / "calm2.ogg"));
        asked.clear();
        retromcp::copy_resources(*w2, dest2, fake);   // fetched once: the second run finds it in the workspace
        CB_CHECK(asked == std::vector<std::string>({"https://example.invalid/calm2"}));
    }

    // an index with nothing in it ({}): nothing
    fs::ScopedTempDir bare("cbrmcp");
    rm_make_workspace(bare.path());
    const auto b = retromcp::find(bare.path());
    CB_CHECK(b && b->asset_index && retromcp::copy_resources(*b, bare.path() / "out").copied == 0);
});

// Beta 1.6's slip: the mappings name EntityLiving.numTicksToChaseTarget (client aC), the source renamed the references, but the field is
// Entity's `aC` (the entity id) and is declared under that name. The repair points the references at it; a call of that name, a longer
// name and a field the source does declare are left alone.
CB_TEST("java.console.retromcp.field_repairs", [] {
    fs::ScopedTempDir tmp("cbrmcp");
    const stdfs::path src = tmp.path() / "src" / "net" / "minecraft" / "src";
    fs::write_text(tmp.path() / "mappings.tiny",
                   "tiny\t2\t0\tnamed\tclient\tserver\n"
                   "c\tnet/minecraft/src/Entity\tpk\tlx\n"
                   "\tf\tI\tentityLabel\tbX\tbY\n"
                   "c\tnet/minecraft/src/EntityLiving\tlo\thi\n"
                   "\tf\tI\tnumTicksToChaseTarget\taC\taF\n"
                   "\tf\tI\thealth\taZ\tbA\n"
                   "\tf\tI\tunused\tzz\tzz\n");
    fs::write_text(src / "Entity.java", "package net.minecraft.src;\n\npublic abstract class Entity {\n\tpublic int aC = nextEntityID++;\n"
                                        "\tprivate static int nextEntityID = 0;\n\tpublic int entityLabel;\n\n\tpublic int getId() {\n\t\treturn this.aC;\n\t}\n}\n");
    fs::write_text(src / "EntityLiving.java", "package net.minecraft.src;\n\npublic abstract class EntityLiving extends Entity {\n\tpublic int health;\n"
                                              "\tpublic int numTicksToChaseTargetX() {\n\t\treturn 0;\n\t}\n}\n");
    fs::write_text(src / "RenderLiving.java", "package net.minecraft.src;\n\npublic class RenderLiving {\n\tvoid a(EntityLiving var1) {\n"
                                              "\t\tString s = Integer.toString(var1.numTicksToChaseTarget);\n\t\tvar1.numTicksToChaseTargetX();\n"
                                              "\t\tvar1.health = var1.entityLabel;\n\t}\n}\n");
    const auto repairs = retromcp::find_field_repairs(tmp.path() / "mappings.tiny", tmp.path() / "src", "client");
    CB_CHECK_EQ(repairs.size(), std::size_t{1});   // (health and entityLabel are declared; unused is referenced nowhere)
    if (repairs.size() == 1) {
        CB_CHECK_EQ(repairs[0].owner, "EntityLiving");
        CB_CHECK_EQ(repairs[0].named, "numTicksToChaseTarget");
        CB_CHECK_EQ(repairs[0].declared, "aC");
        CB_CHECK_EQ(repairs[0].holder, "Entity");
    }
    const auto server = retromcp::find_field_repairs(tmp.path() / "mappings.tiny", tmp.path() / "src", "server");
    CB_CHECK(server.empty());   // the server's name (aF) is declared nowhere here
    const stdfs::path out = tmp.path() / "fixed";
    CB_CHECK_EQ(retromcp::write_repaired_source(tmp.path() / "src", out, repairs), std::size_t{1});
    const std::string fixed = fs::read_text(out / "net" / "minecraft" / "src" / "RenderLiving.java");
    CB_CHECK(fixed.find("Integer.toString(var1.aC)") != std::string::npos);
    CB_CHECK(fixed.find("var1.numTicksToChaseTargetX();") != std::string::npos);
    CB_CHECK(fs::read_text(out / "net" / "minecraft" / "src" / "Entity.java") == fs::read_text(src / "Entity.java"));   // copied as is
    CB_CHECK(retromcp::find_field_repairs(tmp.path() / "absent.tiny", tmp.path() / "src", "client").empty());
});

}  // namespace
