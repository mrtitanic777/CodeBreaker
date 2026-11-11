#include "cb/java/console/xdk_scan.hpp"

#include <format>

#include "cb/java/console/test_support.hpp"

namespace {

using namespace cb;
using namespace cb::java::console;
namespace stdfs = std::filesystem;

// The golden's form of a detect() result (what the original module answered on the same headers).
std::string render(const std::string& heading, const xdk_scan::ScanResult& r, const stdfs::path& base) {
    std::string out = heading + "\n  include " + test::rel(r.include, base) + "\n";
    for (const auto& [name, header] : r.funcs) out += std::format("  func {} {}\n", name, header);
    for (const auto& d : r.found) {
        std::string fns;
        for (const std::string& f : d.functions) fns += (fns.empty() ? "" : ",") + f;
        out += std::format("  found {} header={} count={} fns={}\n", d.needs, d.header, d.func_count, fns);
    }
    for (const auto& d : r.missing) out += std::format("  missing {}\n", d.needs);
    return out;
}

CB_TEST("java.console.xdk_scan.detect_matches_the_original", [] {
    const stdfs::path base = test::data("xdk");
    std::string got;
    const std::pair<const char*, std::optional<stdfs::path>> roots[] = {
        {"detect sdk1", base / "sdk1"},
        {"detect sdk2", base / "sdk2"},
        {"detect sdk3", base / "sdk3"},
        {"detect emptydir", base / "emptydir"},
        {"detect nonexistent", base / "nonexistent"},
        {"detect sdk1/include/xbox/bad.h", base / "sdk1" / "include" / "xbox" / "bad.h"},   // a file, not a folder
        {"detect <none>", std::nullopt},
        {"detect <empty>", stdfs::path()},
    };
    for (const auto& [heading, root] : roots) got += render(heading, xdk_scan::detect(root), base);
    CB_CHECK_EQ(got, test::golden("xdk.golden.txt"));
});

CB_TEST("java.console.xdk_scan.entry_points", [] {
    using xdk_scan::entry_points_in;
    auto names = [](std::string_view text) {
        std::string out;
        for (std::string_view n : entry_points_in(text)) out += (out.empty() ? "" : " ") + std::string(n);
        return out;
    };
    // every prefix, and what is too short to count
    CB_CHECK_EQ(names("XamFoo(1); XFoo(2); D3DXFoo(3); MmFoo(4); KeFoo(5); NtFoo(6); VdFoo(7);"),
                "XamFoo XFoo D3DXFoo MmFoo KeFoo NtFoo VdFoo");
    CB_CHECK_EQ(names("X(1); XA(2); Xam(3); XamA(4); D3DX(5); Mm(6); MmA(7); KeA(8);"), "");
    CB_CHECK_EQ(names("XAb(1); XamAb(2); D3DXa(3); MmAb(4);"), "XAb XamAb D3DXa MmAb");
    CB_CHECK_EQ(names("Xfoo(1); xFoo(2); fooXBar(3); XamfooBar(4); Mmfoo(5);"), "");   // case and boundaries matter
    // white space before the parenthesis, including a line break; a call must follow
    CB_CHECK_EQ(names("XFoo (1); XBar\t(2); XCrlf\r\n(3); XBaz\n\n  (4); XNoCall; XDecl;"), "XFoo XBar XCrlf XBaz");
    CB_CHECK_EQ(names("XFoo2(1) XFoo_3(2) X_Foo(3)"), "XFoo2 XFoo_3");   // digits and underscores are part of the name
    CB_CHECK_EQ(names("a.XFoo(1) b->XBar(2) ::XamBaz(3)"), "XFoo XBar XamBaz");   // member and scope syntax
    // DIVERGES: an identifier is ASCII letters, digits and underscore; the Python's \w took any Unicode letter, so
    // XFoo<e-acute>bar( was one name there. Here it ends at the first non-ASCII byte, and "bar(" is no entry point.
    CB_CHECK_EQ(names("XFoo\xC3\xA9" "bar(1);"), "");
    CB_CHECK_EQ(names(""), "");
});

CB_TEST("java.console.xdk_scan.include_dir_preference", [] {
    fs::ScopedTempDir tmp("cbxdk");
    const stdfs::path root = tmp.path() / "sdk";
    CB_CHECK(!xdk_scan::sdk_include_dir(root));                      // not there at all
    CB_CHECK(!xdk_scan::sdk_include_dir(stdfs::path()));             // no SDK given
    fs::make_dirs(root);
    CB_CHECK_PATH(*xdk_scan::sdk_include_dir(root), root);             // the headers are in the root itself
    fs::make_dirs(root / "include");
    CB_CHECK_PATH(*xdk_scan::sdk_include_dir(root), root / "include");
    fs::make_dirs(root / "include" / "xbox");
    CB_CHECK_PATH(*xdk_scan::sdk_include_dir(root), root / "include" / "xbox");   // the 360 folder wins
});

CB_TEST("java.console.xdk_scan.patterns", [] {
    const auto& all = xdk_scan::patterns();
    CB_CHECK_EQ(all.size(), std::size_t{13});
    std::size_t requirements = 0, equivalents = 0, opportunities = 0;
    for (const auto& p : all) {
        using K = xdk_scan::Kind;
        requirements += p.kind == K::Requirement;
        equivalents += p.kind == K::Equivalent;
        opportunities += p.kind == K::Opportunity;
        // a requirement says how serious it is, an opportunity how big; nothing else says either
        CB_CHECK_EQ(p.sev.has_value(), p.kind == K::Requirement);
        CB_CHECK_EQ(p.effort.has_value(), p.kind == K::Opportunity);
        CB_CHECK(!p.title.empty() && !p.why.empty() && p.family.starts_with(p.needs.substr(0, 2)));
    }
    CB_CHECK_EQ(requirements, std::size_t{5});
    CB_CHECK_EQ(equivalents, std::size_t{1});
    CB_CHECK_EQ(opportunities, std::size_t{7});
    CB_CHECK_EQ(all.front().needs, "XContentCreate");
    CB_CHECK(all.front().sev == xdk_scan::Severity::Correctness);
});

CB_TEST("java.console.xdk_scan.family_listing_is_capped", [] {
    fs::ScopedTempDir tmp("cbxdk");
    std::string header;
    for (int i = 0; i < 20; ++i) header += std::format("XContentFn{:02}(a);\n", i);
    header += "XContentCreate(b);\n";
    fs::write_text(tmp.path() / "include" / "x.h", header);
    const auto r = xdk_scan::detect(tmp.path());
    CB_CHECK_EQ(r.found.size(), std::size_t{1});
    CB_CHECK_EQ(r.found[0].func_count, std::size_t{21});
    CB_CHECK_EQ(r.found[0].functions.size(), xdk_scan::kMaxListed);   // listed sorted, the rest only counted
    CB_CHECK_EQ(r.found[0].functions.front(), "XContentCreate");
});

}  // namespace
