#include "cb/java/console/project_scan.hpp"

#include "cb/java/console/test_support.hpp"

namespace {

using namespace cb;
using namespace cb::java::console;
namespace stdfs = std::filesystem;

CB_TEST("java.console.project_scan.resolve_matches_the_original", [] {
    const stdfs::path base = test::data("scan");
    std::string got;
    for (const char* d : {"proj1", "proj1/src/net/minecraft/client", "proj1/game/resources/lang", "proj2", "proj2/sources/a",
                          "proj3", "proj3/net", "proj3/deep", "proj4"}) {
        const auto r = project_scan::resolve(base / d);
        got += std::string("resolve ") + d + "\n";
        got += "  base " + test::rel(r.base, base) + "\n";
        got += "  jar " + test::rel(r.jar, base) + "\n";
        got += "  resources " + test::rel(r.resources, base) + "\n";
        got += "  java_src " + test::rel(r.java_src, base) + "\n";
        got += "  lang " + test::rel(r.lang, base) + "\n";
    }
    CB_CHECK_EQ(got, test::golden("scan.golden.txt"));
});

CB_TEST("java.console.project_scan.find_base_climbs_to_the_nearest_marker", [] {
    fs::ScopedTempDir tmp("cbscan");
    // nine levels deep, so that the climb of eight never leaves the temporary folder
    const stdfs::path top = tmp.path() / "a" / "b";
    const stdfs::path deep = top / "c" / "d" / "e" / "f" / "g" / "h" / "i";
    fs::make_dirs(deep);
    CB_CHECK_PATH(project_scan::find_base(deep), deep);   // no marker within reach: the picked folder is the base
    fs::make_dirs(top / "Jars");                         // a marker, whatever its case
    CB_CHECK_PATH(project_scan::find_base(deep), top);
    fs::make_dirs(top / "c" / "d" / "e" / "game");       // a nearer one wins
    CB_CHECK_PATH(project_scan::find_base(deep), top / "c" / "d" / "e");
    CB_CHECK_PATH(project_scan::find_base(deep / ".." / ".." / "."), top / "c" / "d" / "e");   // a path with dots in it
    // a marker file counts as well as a folder, and the folder itself may hold it
    fs::write_text(tmp.path() / "x" / "resources", "");
    CB_CHECK_PATH(project_scan::find_base(tmp.path() / "x"), tmp.path() / "x");
    // more than eight levels up is out of reach
    fs::make_dirs(tmp.path() / "far" / "1" / "2" / "3" / "4" / "5" / "6" / "7" / "8");
    fs::make_dirs(tmp.path() / "far" / "game");
    const stdfs::path nine = tmp.path() / "far" / "1" / "2" / "3" / "4" / "5" / "6" / "7" / "8";
    CB_CHECK_PATH(project_scan::find_base(nine), nine);
    CB_CHECK_PATH(project_scan::find_base(nine.parent_path()), tmp.path() / "far");   // eight levels: just reachable
});

CB_TEST("java.console.project_scan.jar_choice", [] {
    fs::ScopedTempDir tmp("cbscan");
    const stdfs::path base = tmp.path();
    CB_CHECK(!project_scan::find_jar(base));
    fs::write_text(base / "a" / "zeta-source.jar", "");
    fs::write_text(base / "a" / "src-sources.JAR", "");
    CB_CHECK(!project_scan::find_jar(base));                       // a source jar is never the resource jar
    fs::write_text(base / "b" / "library.jar", "");
    CB_CHECK_PATH(*project_scan::find_jar(base), base / "b" / "library.jar");   // any other jar, as a last resort
    fs::write_text(base / "c" / "Minecraft.Jar", "");
    CB_CHECK_PATH(*project_scan::find_jar(base), base / "c" / "Minecraft.Jar");  // a preferred name wins over an earlier jar
    fs::write_text(base / "d" / "deobfuscated.jar", "");
    CB_CHECK_PATH(*project_scan::find_jar(base), base / "c" / "Minecraft.Jar");  // ...the first preferred one met
    // heavy folders are not entered
    fs::ScopedTempDir other("cbscan");
    fs::write_text(other.path() / "bin" / "deobfuscated.jar", "");
    fs::write_text(other.path() / "obj" / "x.jar", "");
    fs::write_text(other.path() / "node_modules" / "y.jar", "");
    fs::write_text(other.path() / "x64" / "z.jar", "");
    CB_CHECK(!project_scan::find_jar(other.path()));
    // and neither is anything deeper than five folders
    fs::write_text(other.path() / "1" / "2" / "3" / "4" / "5" / "deep.jar", "");
    CB_CHECK_PATH(*project_scan::find_jar(other.path()), other.path() / "1" / "2" / "3" / "4" / "5" / "deep.jar");
    fs::write_text(other.path() / "1" / "2" / "3" / "4" / "5" / "6" / "deeper.jar", "");
    fs::remove_tree(other.path() / "1" / "2" / "3" / "4" / "5" / "deep.jar");
    CB_CHECK(!project_scan::find_jar(other.path()));
});

CB_TEST("java.console.project_scan.resources_and_lang", [] {
    fs::ScopedTempDir tmp("cbscan");
    const stdfs::path base = tmp.path();
    fs::make_dirs(base / "resources" / "textures");                    // a resources folder with no sound in it
    CB_CHECK(!project_scan::find_resources(base));
    fs::make_dirs(base / "game" / "Resources" / "Streaming");
    CB_CHECK_PATH(*project_scan::find_resources(base), base / "game" / "Resources");
    CB_CHECK(!project_scan::find_lang(base));
    fs::write_text(base / "x" / "lang" / "de.lang", "");
    fs::write_text(base / "x" / "other" / "en_US.lang", "");           // not under a "lang" folder
    CB_CHECK_PATH(*project_scan::find_lang(base), base / "x" / "lang" / "de.lang");   // the first one will do
    fs::write_text(base / "y" / "LANG" / "EN_us.LANG", "");
    CB_CHECK_PATH(*project_scan::find_lang(base), base / "y" / "LANG" / "EN_us.LANG");   // en_US wins, in any case
});

CB_TEST("java.console.project_scan.java_source_root", [] {
    fs::ScopedTempDir tmp("cbscan");
    const stdfs::path base = tmp.path() / "base";
    const stdfs::path picked = tmp.path() / "picked";
    fs::write_text(picked / "p" / "A.java", "");
    CB_CHECK_PATH(*project_scan::find_java_src(picked, base), picked);        // no src folder: the picked folder itself
    fs::write_text(base / "sources" / "q" / "B.java", "");
    CB_CHECK_PATH(*project_scan::find_java_src(picked, base), base / "sources");
    fs::write_text(base / "src" / "C.java", "");
    CB_CHECK_PATH(*project_scan::find_java_src(picked, base), base / "src");   // src, source, sources in that order
    fs::write_text(tmp.path() / "none" / "readme.md", "");
    CB_CHECK(!project_scan::find_java_src(tmp.path() / "none", tmp.path() / "nobase"));
    CB_CHECK(!project_scan::find_java_src(tmp.path() / "missing", tmp.path() / "nobase"));
});

}  // namespace
