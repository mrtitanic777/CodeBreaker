#include <string>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/java/overlays.hpp"

namespace {

using namespace cb;
using namespace cb::java;
namespace stdfs = std::filesystem;
CB_TEST("java.overlays.project_key", [] {
    using java::overlays::project_key;
    CB_CHECK_EQ(project_key("out/MinecraftBeta"), "MinecraftBeta");
    CB_CHECK_EQ(project_key("out/MinecraftBeta/"), "MinecraftBeta");
    CB_CHECK_EQ(project_key("out/a/../MinecraftAlpha"), "MinecraftAlpha");
    CB_CHECK_EQ(project_key(""), "");
    CB_CHECK(!project_key(".").empty());   // "." is the current folder, named by its own name
});

CB_TEST("java.overlays.resolve", [] {
    using java::overlays::resolve_overlay_dir;
    auto r = resolve_overlay_dir("out/MinecraftBeta", "state/java/overlays");
    CB_CHECK(r.has_value());
    CB_CHECK_EQ(fs::to_text(*r), "state/java/overlays/MinecraftBeta");
    auto e = resolve_overlay_dir("out/X", "root", stdfs::path("elsewhere"));
    CB_CHECK_EQ(fs::to_text(*e), "elsewhere");                               // an explicit folder wins
    CB_CHECK(!resolve_overlay_dir("", "root"));                              // no project, no overlay
});

CB_TEST("java.overlays.apply", [] {
    fs::ScopedTempDir tmp("cbjava");
    const stdfs::path overlay = tmp.path() / "overlay";
    const stdfs::path out = tmp.path() / "out";
    fs::write_text(overlay / "A.cpp", "overlaid");
    fs::write_text(overlay / "sub" / "Socket.cpp", "new file");
    fs::write_text(overlay / "MANIFEST.md", "not copied");
    fs::write_text(out / "A.cpp", "generated");

    std::vector<std::string> log;
    auto applied = java::overlays::apply_overlays(out, overlay, [&](std::string_view s) { log.emplace_back(s); });
    CB_CHECK_EQ(applied.size(), std::size_t{2});
    CB_CHECK_EQ(applied[0], "A.cpp");
    CB_CHECK_EQ(applied[1], "sub/Socket.cpp");
    CB_CHECK_EQ(fs::read_text(out / "A.cpp"), "overlaid");                    // replaced
    CB_CHECK_EQ(fs::read_text(out / "sub" / "Socket.cpp"), "new file");       // added, folder created
    CB_CHECK(!stdfs::exists(out / "MANIFEST.md"));
    CB_CHECK_EQ(log.size(), std::size_t{3});
    CB_CHECK_EQ(log[0], "[overlay] replaced A.cpp");
    CB_CHECK_EQ(log[1], "[overlay] ADDED    sub/Socket.cpp");
    CB_CHECK(log[2].starts_with("[overlay] applied 2 project overlay file(s) (1 new) from "));

    // absent or missing overlay folder: nothing happens
    CB_CHECK(java::overlays::apply_overlays(out, std::nullopt).empty());
    CB_CHECK(java::overlays::apply_overlays(out, tmp.path() / "nope").empty());
});

}  // namespace

