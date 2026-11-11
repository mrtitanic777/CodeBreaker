#include "cb/build/period16.hpp"

#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace {

using namespace cb;
using namespace cb::build;
namespace stdfs = std::filesystem;

void put16(Bytes& b, std::size_t at, unsigned v) {
    b[at] = static_cast<std::uint8_t>(v);
    b[at + 1] = static_cast<std::uint8_t>(v >> 8);
}
void put32(Bytes& b, std::size_t at, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) b[at + i] = static_cast<std::uint8_t>(v >> (8 * i));
}
unsigned get16(const Bytes& b, std::size_t at) { return b[at] | (b[at + 1] << 8); }
std::uint32_t get32(const Bytes& b, std::size_t at) { return get16(b, at) | (std::uint32_t{get16(b, at + 2)} << 16); }

// An NE file: a stub of `stub` bytes, the header (0x40) at lfa = stub, a segment table, then the segments at
// sector boundaries (shift 4: 16-byte sectors), a non-resident name table and a fast-load area.
Bytes make_ne(std::size_t stub, const std::vector<Bytes>& segments, unsigned shift = 4) {
    const std::size_t unit = std::size_t{1} << shift;
    Bytes b(stub + 0x40 + 8 * segments.size(), 0);
    b[0] = 'M';
    b[1] = 'Z';
    put32(b, 0x3C, static_cast<std::uint32_t>(stub));
    b[stub] = 'N';
    b[stub + 1] = 'E';
    put16(b, stub + 0x1C, static_cast<unsigned>(segments.size()));
    put16(b, stub + 0x22, 0x40);
    put16(b, stub + 0x32, shift);
    const std::size_t nonres = b.size();
    b.insert(b.end(), 20, 0x4E);   // the non-resident names
    put32(b, stub + 0x2C, static_cast<std::uint32_t>(nonres));
    b.resize((b.size() + unit - 1) / unit * unit, 0);
    put16(b, stub + 0x38, static_cast<unsigned>(b.size() / unit));   // the fast-load area's sector
    b.insert(b.end(), unit, 0x77);
    for (std::size_t i = 0; i < segments.size(); ++i) {
        put16(b, stub + 0x40 + 8 * i, static_cast<unsigned>(b.size() / unit));
        put16(b, stub + 0x40 + 8 * i + 2, static_cast<unsigned>(segments[i].size()));
        b.insert(b.end(), segments[i].begin(), segments[i].end());
        b.resize((b.size() + unit - 1) / unit * unit, 0);
    }
    return b;
}

class FakeRunner : public DosRunner {
public:
    DosRunResult run(const DosRunSpec& spec) const override {
        seen = spec;
        ++calls;
        if (make_output && spec.work && !spec.artifacts.empty()) fs::write_file(*spec.work / spec.artifacts[0], Bytes{9});
        DosRunResult r;
        r.work = spec.work;
        return r;
    }
    mutable DosRunSpec seen;
    mutable int calls = 0;
    bool make_output = true;
};

struct Fixture {
    fs::ScopedTempDir dir{"period16"};
    period16::Tools tools;
    stdfs::path folder;
    Fixture() {
        for (const char* tool : {"dosbox", "msc6", "msvc152", "kit", "ddk"}) fs::make_dirs(dir.path() / tool);
        fs::make_dirs(dir.path() / "msvc152" / "BIN");
        fs::write_file(dir.path() / "msvc152" / "BIN" / "WINSTUB.EXE", Bytes{1});
        fs::write_file(dir.path() / "kit" / "MDLLCEW.LIB", Bytes{1});
        fs::write_file(dir.path() / "kit" / "LIBW.LIB", Bytes{2});
        tools.dosbox_x = dir.path() / "dosbox";
        tools.msc6 = dir.path() / "msc6";
        tools.msvc152 = dir.path() / "msvc152";
        tools.win31_kit = dir.path() / "kit";
        folder = dir.path() / "src";
        fs::make_dirs(folder);
        fs::write_text(folder / "a.c", "int a;");
        fs::write_text(folder / "b.C", "int b;");
        fs::write_text(folder / "a.h", "");
        fs::write_text(folder / "lz.def", "LIBRARY LZ");
        fs::write_text(folder / "notes.txt", "not copied");
        fs::write_text(folder / "z.lnk", "ignored: a later response file");
        fs::write_text(folder / "lz.lnk", "libentry.obj+\nA.OBJ+\nB.obj+\nextra.obj\nlzexpand.dll\nlzexpand.map\nlibw.lib mdllcew.lib\n");
    }
};

}  // namespace

CB_TEST("build.period16.build_compiles_each_module_and_links", [] {
    Fixture f;
    f.tools.libentry_fallback = f.dir.path() / "ddk";
    fs::write_file(f.dir.path() / "ddk" / "LIBENTRY.OBJ", Bytes{3});
    FakeRunner runner;
    const auto dll = period16::build(runner, f.tools, f.folder, f.dir.path() / "work");
    CB_CHECK(dll.has_value());
    CB_CHECK(fs::same_path(*dll, f.dir.path() / "work" / "lzexpand.dll"));
    const DosRunSpec& s = runner.seen;
    // the source files came over, the others did not; the donor stub and the kit's libraries too
    const stdfs::path work = f.dir.path() / "work";
    for (const char* name : {"a.c", "b.C", "a.h", "lz.def", "lz.lnk", "z.lnk", "WINSTUB.EXE", "MDLLCEW.LIB", "LIBW.LIB", "LIBENTRY.OBJ"}) {
        if (!stdfs::exists(work / name)) CB_CHECK_EQ(std::string(name), "");   // names the missing file
    }
    CB_CHECK(!stdfs::exists(work / "notes.txt"));
    // modules that have a .c are compiled (extra.obj has none and libentry comes from the kit), in the .lnk's order
    const std::vector<std::string> expected{"del A.obj", "del B.obj", "del lzexpand.dll", "del done.txt",
                                            "cl /c /AM /G2 /Gsw /Ox A.c >> CL.TXT", "cl /c /AM /G2 /Gsw /Ox B.c >> CL.TXT",
                                            "link @lz.lnk > LINK.TXT", "rc lzexpand.dll > RC.TXT"};
    CB_CHECK(s.commands == expected);
    CB_CHECK_EQ(s.mounts.size(), 3u);
    CB_CHECK_EQ(s.mounts[0].drive, "c");
    CB_CHECK_EQ(s.mounts[1].drive, "e");
    CB_CHECK_EQ(s.mounts[2].drive, "d");
    CB_CHECK(fs::same_path(s.mounts[2].folder, work));
    CB_CHECK_EQ(s.variables.at("LIB"), "D:\\;E:\\LIB");
    CB_CHECK_EQ(s.timeout.count(), 600);
    CB_CHECK_EQ(s.artifacts.at(0), "lzexpand.dll");
    CB_CHECK(fs::same_path(*s.work, work));
});

CB_TEST("build.period16.no_dll_made_is_none", [] {
    Fixture f;
    FakeRunner runner;
    runner.make_output = false;
    CB_CHECK(!period16::build(runner, f.tools, f.folder, f.dir.path() / "work2").has_value());
    // an own work folder is created when none is given, and kept
    FakeRunner runner2;
    const auto dll = period16::build(runner2, f.tools, f.folder);
    CB_CHECK(dll.has_value());
    CB_CHECK(stdfs::exists(*dll));
    fs::remove_tree(dll->parent_path());
});

CB_TEST("build.period16.build_errors", [] {
    Fixture f;
    FakeRunner runner;
    period16::Tools t = f.tools;
    t.msc6 = f.dir.path() / "nope";
    CB_CHECK_THROWS_AS(NotFoundError, (void)period16::build(runner, t, f.folder));
    t = f.tools;
    t.dosbox_x.clear();
    CB_CHECK_THROWS_AS(NotFoundError, (void)period16::build(runner, t, f.folder));
    CB_CHECK_THROWS_AS(UsageError, (void)period16::build(runner, f.tools, f.folder, std::nullopt, "/c & x"));
    fs::ScopedTempDir empty("p16empty");
    CB_CHECK_THROWS_AS(NotFoundError, (void)period16::build(runner, f.tools, empty.path()));
    CB_CHECK_THROWS_AS(NotFoundError, (void)period16::response_file(empty.path()));
    CB_CHECK_THROWS_AS(NotFoundError, (void)period16::response_file(empty.path() / "missing"));
    CB_CHECK_EQ(runner.calls, 0);
    CB_CHECK_EQ(period16::response_file(f.folder), "lz.lnk");   // the first by name
});

CB_TEST("build.period16.kit_and_libentry_lookup", [] {
    Fixture f;
    CB_CHECK(!period16::find_libentry(f.tools).has_value());
    fs::write_file(f.dir.path() / "kit" / "LIBENTRY.OBJ", Bytes{1});
    CB_CHECK(fs::same_path(*period16::find_libentry(f.tools), f.dir.path() / "kit" / "LIBENTRY.OBJ"));
    fs::make_dirs(f.dir.path() / "toolchains" / "period" / "libs" / "win31-kit");
    CB_CHECK(!period16::find_kit(f.dir.path()).has_value());
    fs::write_file(f.dir.path() / "toolchains" / "period" / "libs" / "win31-kit" / "MDLLCEW.LIB", Bytes{1});
    CB_CHECK(period16::find_kit(f.dir.path()).has_value());
});

CB_TEST("build.period16.segments_and_compare", [] {
    const Bytes s1(100, 0xA1), s2(40, 0xA2), s3(70, 0xA3);
    const Bytes built = make_ne(0x80, {s1, s2, s3});
    const auto segs = period16::segments(built);
    CB_CHECK_EQ(segs.size(), 3u);
    CB_CHECK_EQ(segs[0].length, 100u);
    CB_CHECK_EQ(segs[1].bytes.size(), 40u);
    CB_CHECK_EQ(segs[2].bytes[0], 0xA3);

    // an original with the second segment different and a fourth that the build lacks
    Bytes s2b = s2;
    s2b[5] = 0x00;
    const Bytes original = make_ne(0x100, {s1, s2b, s3, Bytes(10, 0xA4)});
    const period16::Comparison c = period16::compare(built, original);
    CB_CHECK_EQ(c.identical, 2u);
    CB_CHECK_EQ(c.total, 4u);
    CB_CHECK_EQ(c.lines.size(), 6u);
    CB_CHECK_EQ(c.lines[0], std::format("built: 3 segments, {} bytes   original: 4 segments, {} bytes", built.size(), original.size()));
    CB_CHECK_EQ(c.lines[1], "  segment 1  built 100   original 100   identical");
    CB_CHECK_EQ(c.lines[2], "  segment 2  built 40    original 40    same length, different");
    CB_CHECK_EQ(c.lines[3], "  segment 3  built 70    original 70    identical");
    CB_CHECK_EQ(c.lines[4], "  segment 4  built -     original 10    different length");
    CB_CHECK_EQ(c.lines[5], "2 of 4 segments identical");
    // a damaged table is an error, not a short answer
    Bytes cut(built.begin(), built.begin() + 0x80 + 0x40 + 4);
    CB_CHECK_THROWS_AS(FormatError, (void)period16::segments(cut));                       // its first segment lies outside
    CB_CHECK_THROWS_AS(TruncatedError, (void)period16::segments(Bytes(built.begin(), built.begin() + 0x80 + 0x1E)));   // its header is cut
    Bytes lying = built;
    put16(lying, 0x80 + 0x40 + 2, 60000);   // a length far past the file
    CB_CHECK_THROWS_AS(FormatError, (void)period16::segments(lying));
    // sector 0: no image in the file (the port read the file's first bytes)
    Bytes none = built;
    put16(none, 0x80 + 0x40, 0);
    CB_CHECK(period16::segments(none)[0].bytes.empty());
});

CB_TEST("build.period16.normalize_with_whole_sector_difference", [] {
    const std::vector<Bytes> segs{Bytes(100, 1), Bytes(50, 2)};
    const Bytes built = make_ne(1024, segs, 9);
    const Bytes donor = make_ne(512, {Bytes(10, 5)}, 9);
    const Bytes out = period16::normalize(built, donor);
    CB_CHECK_EQ(out.size(), built.size() - 512);
    CB_CHECK_EQ(get32(out, 0x3C), 512u);
    CB_CHECK_EQ(out[512], 'N');
    const auto original_segments = period16::segments(built);
    const auto moved = period16::segments(out);
    CB_CHECK_EQ(moved.size(), 2u);
    for (std::size_t i = 0; i < 2; ++i) CB_CHECK(std::equal(original_segments[i].bytes.begin(), original_segments[i].bytes.end(), moved[i].bytes.begin()));
    // the non-resident name table moved by the whole difference (bytes), the fast-load area by sectors
    CB_CHECK_EQ(get32(out, 512 + 0x2C), get32(built, 1024 + 0x2C) - 512);
    CB_CHECK_EQ(get16(out, 512 + 0x38), get16(built, 1024 + 0x38) - 1);
    CB_CHECK_EQ(out[get32(out, 512 + 0x2C)], 0x4E);
    // an MZ donor with a PE header: the stub is what lies before e_lfanew
    Bytes pe_donor(0x400, 0xCC);
    pe_donor[0] = 'M';
    pe_donor[1] = 'Z';
    put32(pe_donor, 0x3C, 512);
    CB_CHECK_EQ(get32(period16::normalize(built, pe_donor), 0x3C), 512u);
    // a raw stub (no MZ): all of it
    Bytes raw(512, 0xDD);
    CB_CHECK_EQ(get32(period16::normalize(built, raw), 0x3C), 512u);
});

CB_TEST("build.period16.normalize_errors", [] {
    const Bytes built = make_ne(1024, {Bytes(100, 1)}, 9);
    CB_CHECK_THROWS_AS(FormatError, (void)period16::normalize(built, Bytes(100, 0xAA)));          // not a multiple of 16
    CB_CHECK_THROWS_AS(FormatError, (void)period16::normalize(built, Bytes(2048, 0xAA)));         // longer than the built stub
    CB_CHECK_THROWS_AS(FormatError, (void)period16::normalize(built, Bytes(1024 - 16, 0xAA)));    // not whole sectors
    CB_CHECK_THROWS_AS(TruncatedError, (void)period16::normalize(Bytes(10, 0), Bytes(512, 0)));
});
