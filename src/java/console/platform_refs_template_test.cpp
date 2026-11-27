// A reference written as a template over another file (//@template, //@define, //@hole, //@set: see platform_refs.cpp), emitted by
// emit_references, and the real Direct3D Display files that are written that way.
#include <algorithm>
#include <string>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "java/console/platform_refs.hpp"

namespace {

using namespace cb;
namespace pr = cb::java::console::platform_refs;
namespace stdfs = std::filesystem;

// What emit_references wrote for `file`, its line ends made plain "\n" (the platform writes whichever it was given).
std::string tmpl_emit(const stdfs::path& ref, const std::string& file) {
    pr::EmitOptions options;
    options.platform = pr::Platform::PcOpenGl;
    options.main_class = "Minecraft";
    fs::ScopedTempDir out("cb-refs-template");
    pr::emit_references(out.path(), ref, options);
    std::string text = fs::read_text(out.path() / file);
    text.erase(std::remove(text.begin(), text.end(), '\r'), text.end());
    return text;
}

CB_TEST("java.platform_refs.a_reference_can_be_a_template_over_another_file", [] {
    fs::ScopedTempDir dir("cb-refs-tmplsrc");
    const stdfs::path ref = dir.path() / "reference";
    stdfs::create_directories(ref);
    fs::write_text(dir.path() / "base.txt", "top\r\n//@hole a\r\nmiddle @V@\r\n//@hole b\r\nbottom\r\n");
    fs::write_text(dir.path() / "mid.txt", "//@template base.txt\r\n//@define a\r\nmid a (@V@)\r\n//@hole c\r\n//@end\r\n//@define c: mid c\r\n");
    fs::write_text(ref / "Thing.reference.h", "//@template ../mid.txt\r\n//@set V 12\r\n//@define b: only b\r\n//@define c\r\nover c\r\n//@end\r\n");
    fs::write_text(ref / "Plain.reference.h", "no directives\r\nat all\r\n");
    // the derived file's define wins over the middle template's, and a file without directives is emitted as it is
    CB_CHECK(tmpl_emit(ref, "Thing.h").ends_with("top\nmid a (12)\nover c\nmiddle 12\nonly b\nbottom\n"));
    CB_CHECK(tmpl_emit(ref, "Plain.h").ends_with("no directives\nat all\n"));
});

CB_TEST("java.platform_refs.the_direct3d_displays_are_templates", [] {
    for (const char* api : {"d3d9", "d3d10", "d3d11", "d3d12"}) {
        const stdfs::path file = testing::data_path(std::string("../config/java/platform/pc/") + api + "/reference/Display.reference.cpp");
        CB_CHECK(fs::read_text(file).starts_with("//@template "));
    }
});

}  // namespace
