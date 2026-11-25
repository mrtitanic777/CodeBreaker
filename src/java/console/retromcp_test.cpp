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

}  // namespace
