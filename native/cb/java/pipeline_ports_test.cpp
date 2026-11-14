// The pipeline's prune, forwarder and raycast classes (through the prune module).
#include <algorithm>
#include <set>
#include <string>
#include <vector>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/java/projects/minecraft/raycast.hpp"
#include "cb/java/prune.hpp"

namespace {

namespace pln = cb::java::prune;

struct PlnTree {
    cb::fs::ScopedTempDir dir{"cbpipeline"};
    void write(const char* name, const std::string& text) const { cb::fs::write_text(dir.path() / name, text); }
    bool exists(const char* name) const { return std::filesystem::exists(dir.path() / name); }
    std::string read(const char* name) const { return cb::fs::read_text(dir.path() / name); }
};

bool pln_has(const std::vector<std::string>& names, const char* name) { return std::ranges::find(names, name) != names.end(); }
bool pln_contains(const std::string& text, std::string_view part) { return text.find(part) != std::string::npos; }

// ---- TestPruneUnreferenced ----------------------------------------------------------------------------------------------------------------------

CB_TEST("java.pipeline.prune_cuts_unreached_keeps_reachable_and_linkcritical", [] {
    PlnTree t;
    t.write("Level.h", "#include \"Textures.h\"\nstruct Level{};\n");
    t.write("Level.cpp", "#include \"Level.h\"\nvoid f(){}\n");
    t.write("Textures.h", "#include \"BufferedImage.h\"\nstruct Textures{};\n");
    t.write("BufferedImage.h", "struct BufferedImage{};\n");
    t.write("Audio.h", "struct Audio{};\n");
    t.write("Audio.cpp", "#include \"Audio.h\"\n");
    t.write("Socket.h", "struct Socket{};\n");
    t.write("HeapTrack.cpp", "// J2X-ALWAYS-LINK\nvoid* operator new(unsigned){return 0;}\n");
    t.write("stdafx.h", "// pch\n");
    const auto dropped = pln::prune_unreferenced(t.dir.path(), {"Level"});
    CB_CHECK(pln_has(dropped, "Audio.h"));
    CB_CHECK(pln_has(dropped, "Audio.cpp"));
    CB_CHECK(pln_has(dropped, "Socket.h"));
    CB_CHECK(!pln_has(dropped, "Textures.h"));
    CB_CHECK(!pln_has(dropped, "BufferedImage.h"));
    CB_CHECK(!pln_has(dropped, "HeapTrack.cpp"));
    CB_CHECK(!t.exists("Audio.h"));
    CB_CHECK(t.exists("HeapTrack.cpp"));
    CB_CHECK(t.exists("BufferedImage.h"));
});

// A file a project overlay wrote is a root even when nothing includes it and it defines no referenced symbol.
CB_TEST("java.pipeline.prune_overlay_written_files_are_roots", [] {
    PlnTree t;
    t.write("Level.h", "struct Level{};\n");
    t.write("Level.cpp", "#include \"Level.h\"\nvoid f(){}\n");
    t.write("stdafx.h", "// pch\n");
    t.write("Kernel.cpp", "#include \"KernelConfig.h\"\nvoid Kernel_Boot(){}\n");
    t.write("KernelConfig.h", "#define J2X_KERNEL 1\n");
    t.write("UdpSocket.h", "struct UdpSocket{};\n");
    t.write("Orphan.h", "struct Orphan{};\n");
    const std::vector<std::string> applied = {"Kernel.cpp", "KernelConfig.h", "UdpSocket.h"};
    const auto dropped = pln::prune_unreferenced(t.dir.path(), {"Level"}, applied);
    for (const std::string& f : applied) {
        CB_CHECK(!pln_has(dropped, f.c_str()));
        CB_CHECK(t.exists(f.c_str()));
    }
    CB_CHECK(pln_has(dropped, "Orphan.h"));   // control: the prune still cuts real islands
});

// apply_overlays returns relative paths; a nested one roots by its file name.
CB_TEST("java.pipeline.prune_overlay_roots_accept_nested_relpaths", [] {
    PlnTree t;
    t.write("Level.h", "struct Level{};\n");
    t.write("stdafx.h", "// pch\n");
    t.write("ControllerConfig.h", "struct Pad{};\n");
    const auto dropped = pln::prune_unreferenced(t.dir.path(), {"Level"}, {"sub/ControllerConfig.h"});
    CB_CHECK(!pln_has(dropped, "ControllerConfig.h"));
    CB_CHECK(t.exists("ControllerConfig.h"));
});

CB_TEST("java.pipeline.prune_a_header_drags_its_cpp_partner_and_its_includes", [] {
    PlnTree t;
    t.write("Level.h", "#include \"Display.h\"\nstruct Level{};\n");
    t.write("Display.h", "struct Display{};\n");
    t.write("Display.cpp", "#include \"Display.h\"\n#include \"Input.h\"\n");
    t.write("Input.h", "struct Input{};\n");
    t.write("Orphan.h", "struct Orphan{};\n");
    const auto dropped = pln::prune_unreferenced(t.dir.path(), {"Level"});
    CB_CHECK(pln_has(dropped, "Orphan.h"));
    CB_CHECK(!pln_has(dropped, "Input.h"));      // reached through the Display.cpp partner
    CB_CHECK(!pln_has(dropped, "Display.cpp"));
});

// The real link bug: FileInputStream's constructor and a free function are DEFINED in FileIO.cpp, but the game names them without including FileIO.h.
CB_TEST("java.pipeline.prune_a_cross_named_implementation_is_kept_by_its_symbols", [] {
    PlnTree t;
    t.write("Level.h", "#include \"FileInputStream.h\"\nstruct Level{};\n");
    t.write("Level.cpp", "#include \"Level.h\"\nvoid f(){ FileInputStream* s = new FileInputStream(0);"
                         " InputStream* r = J2X_getResourceAsStream(\"x\"); }\n");
    t.write("FileInputStream.h", "struct FileInputStream { FileInputStream(void*); };\n");
    t.write("FileIO.h", "struct FileIO{};\n");
    t.write("FileIO.cpp", "#include \"FileIO.h\"\n"
                          "FileInputStream::FileInputStream(void*) {}\n"
                          "InputStream* J2X_getResourceAsStream(const char* n) { return 0; }\n");
    t.write("Audio.h", "struct Audio{};\n");
    t.write("Audio.cpp", "#include \"Audio.h\"\nvoid Audio::play() {}\n");
    const auto dropped = pln::prune_unreferenced(t.dir.path(), {"Level"});
    CB_CHECK(!pln_has(dropped, "FileIO.cpp"));
    CB_CHECK(!pln_has(dropped, "FileIO.h"));
    CB_CHECK(pln_has(dropped, "Audio.cpp"));
    CB_CHECK(pln_has(dropped, "Audio.h"));
});

// ---- TestCoalesceForwarders ---------------------------------------------------------------------------------------------------------------------

CB_TEST("java.pipeline.forwarder_is_folded_and_includes_are_rewritten", [] {
    PlnTree t;
    t.write("NIOBuffers.h", "struct ByteBuffer{};\n");
    t.write("ByteBuffer.h", "#ifndef BYTEBUFFER_H\n#define BYTEBUFFER_H\n#include \"NIOBuffers.h\"\n#endif\n");
    t.write("Level.cpp", "#include \"ByteBuffer.h\"\nvoid f(){ ByteBuffer b; }\n");
    const auto removed = pln::coalesce_forwarders(t.dir.path());
    CB_CHECK(pln_has(removed, "ByteBuffer.h"));
    CB_CHECK(!t.exists("ByteBuffer.h"));
    const std::string level = t.read("Level.cpp");
    CB_CHECK(pln_contains(level, "#include \"NIOBuffers.h\""));
    CB_CHECK(!pln_contains(level, "#include \"ByteBuffer.h\""));
});

CB_TEST("java.pipeline.a_chain_of_forwarders_resolves_to_the_end", [] {
    PlnTree t;
    t.write("Real.h", "struct T{};\n");
    t.write("Mid.h", "#include \"Real.h\"\n");
    t.write("Top.h", "#include \"Mid.h\"\n");
    t.write("U.cpp", "#include \"Top.h\"\n");
    const auto removed = pln::coalesce_forwarders(t.dir.path());
    CB_CHECK(std::set<std::string>(removed.begin(), removed.end()) == (std::set<std::string>{"Mid.h", "Top.h"}));
    CB_CHECK(pln_contains(t.read("U.cpp"), "#include \"Real.h\""));
});

CB_TEST("java.pipeline.a_header_with_a_body_is_not_a_forwarder", [] {
    PlnTree t;
    t.write("Real.h", "#include \"Other.h\"\nstruct Real{};\n");
    t.write("Other.h", "struct Other{};\n");
    CB_CHECK(pln::coalesce_forwarders(t.dir.path()).empty());
    CB_CHECK(t.exists("Real.h"));
});

// ---- TestGlSelectPickToRaycast ------------------------------------------------------------------------------------------------------------------

namespace rcy = cb::java::projects::minecraft::raycast;

const char* const kPlnDriver =
    "void RubyDung::pick(float a)\n"
    "{\n"
    "\td3d9::glSelectBuffer(this->selectBuffer);\n"
    "\td3d9::glRenderMode(d3d9::GL_SELECT);\n"
    "\tthis->setupPickCamera(a, this->width / 2, this->height / 2);\n"
    "\tthis->levelRenderer->pick(this->player);\n"
    "\tint hits = d3d9::glRenderMode(d3d9::GL_RENDER);\n"
    "\tif (this->player != NULL) { this->hitResult = new HitResult(1,2,3,0,1); }\n"
    "\tthis->level->setTile(0,0,0,0);\n"
    "}\n";

CB_TEST("java.pipeline.the_pick_driver_becomes_a_raycast", [] {
    const std::string out = rcy::convert_gl_pick_to_raycast(kPlnDriver);
    CB_CHECK(!pln_contains(out, "GL_SELECT"));
    CB_CHECK(!pln_contains(out, "glRenderMode"));
    CB_CHECK(!pln_contains(out, "glSelectBuffer"));
    CB_CHECK(pln_contains(out, "isSolidTile"));
    CB_CHECK(pln_contains(out, "new HitResult(bx, by, bz, 0, face)"));
    std::size_t headers = 0;
    for (std::size_t at = out.find("void RubyDung::pick(float a)"); at != std::string::npos; at = out.find("void RubyDung::pick(float a)", at + 1)) ++headers;
    CB_CHECK_EQ(headers, std::size_t{1});   // the header is preserved once
});

CB_TEST("java.pipeline.the_pick_helpers_are_emptied", [] {
    const std::string cam = rcy::convert_gl_pick_to_raycast(
        "void RubyDung::setupPickCamera(float a, int x, int y)\n{\n\td3d9::gluPickMatrix(x, y, 5.0F, 5.0F, this->viewportBuffer);\n}\n");
    CB_CHECK(!pln_contains(cam, "gluPickMatrix"));
    const std::string ren = rcy::convert_gl_pick_to_raycast(
        "void LevelRenderer::pick(Player* player)\n{\n\td3d9::glInitNames();\n\td3d9::glPushName(0);\n}\n");
    CB_CHECK(!pln_contains(ren, "glPushName"));
    CB_CHECK(!pln_contains(ren, "glInitNames"));
});

CB_TEST("java.pipeline.a_driver_without_the_game_members_is_left_alone", [] {
    const std::string alien = "void Widget::pick(float a)\n{\n\td3d9::glRenderMode(d3d9::GL_SELECT);\n\tdoStuff();\n}\n";
    CB_CHECK(pln_contains(rcy::convert_gl_pick_to_raycast(alien), "GL_SELECT"));
});

CB_TEST("java.pipeline.no_idiom_no_change", [] {
    const std::string plain = "void Foo::bar() {\n\treturn;\n}\n";
    CB_CHECK_EQ(rcy::convert_gl_pick_to_raycast(plain), plain);
});

}  // namespace
