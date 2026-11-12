#include "cb/exact/library.hpp"

#include <algorithm>
#include <unordered_set>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/omf.hpp"

using namespace cb;
using namespace cb::exact;
namespace stdfs = std::filesystem;
namespace omf = cb::formats::omf;

// P0.EXE of the Microsoft C 3.00 sources (testdata/data/p0: image and symbols in fixture.txt) looked for the way the
// Python module the old exact.library was run on it: with the real libraries of the msc300 kit (SLIBC.LIB, SLIBFP.LIB,
// EM.LIB, and the loose objects SSETARGV, SVARSTCK and BINMODE; the kit is found by CB_KIT_MSC300 or next to the
// checkout, and the tests that need it say so and pass where it is not). testdata/library/p0_crtlib.txt is the CRTLIB the
// Python wrote (68 lines: the modules of the library in the order the linker took them, each with where its code or
// data stands, its stand-ins and what none of the kit's modules defines), read through for what P0 links (atoi and atol
// one jump to __catox, atof's fltused, the stand-in for the module of 2014 bytes of another version); p0_summary.txt what
// the command printed after it.
//
// DIVERGES: a module placed "by the names" no longer takes a place below 0 (the Python indexed the image from its end
// there); a data check on a negative place no longer reads the image from the end either. A Member keeps no L2 module
// (the Python held its publics in one).

namespace {

stdfs::path kit_dir() {
    if (const auto env = fs::get_env("CB_KIT_MSC300")) return fs::from_text(*env);
    return "C:/Users/mrtit/Documents/GitHub/Microsoft-Assemblers-Compilers/MicrosoftC3.0/corpus";
}

struct Settings {
    std::uint32_t gap = 4096;
    std::optional<std::uint32_t> start;
    std::map<std::string, std::uint32_t> sizes;
    std::vector<library::Project::DataPad> pads;
};

struct LibProject final : library::Project {
    stdfs::path folder;
    stdfs::path source_dir;
    Bytes img;
    SymbolTable syms;
    std::uint32_t dgroup = 0;
    std::vector<LibraryFile> libs, kit_objs;
    std::vector<stdfs::path> first_objs;
    Settings settings;
    std::unordered_set<std::uint32_t> relocs;
    std::uint32_t end_of_code = 0;

    stdfs::path src() const override { return source_dir; }
    stdfs::path path(std::string_view name) const override { return source_dir / std::string(name); }
    std::uint32_t code_end() const override { return end_of_code; }
    std::uint32_t own_end() const override { return 0; }
    std::optional<std::string> switches() const override { return std::nullopt; }
    std::optional<std::string> config_text(std::string_view) const override { return std::nullopt; }
    std::vector<std::uint32_t> code_segments() const override { return {0}; }
    std::shared_ptr<const modules::Toolchain> toolchain() const override { return nullptr; }
    std::map<std::string, Bytes> headers() const override { return {}; }
    std::vector<std::string> include() const override { return {}; }
    std::vector<std::shared_ptr<const modules::Project>> also_sets() const override { return {}; }
    ByteView image() const override { return img; }
    SymbolTable symbols() const override { return syms; }
    compare::Program program() const override {
        compare::Program p;
        p.dgroup = dgroup;
        p.code_base = [](std::uint32_t) { return 0u; };
        p.relocs = &relocs;
        return p;
    }
    std::vector<LibraryFile> libraries() const override { return libs; }
    std::vector<stdfs::path> kit_objects() const override {
        std::vector<stdfs::path> out;
        for (const LibraryFile& f : kit_objs) out.push_back(f.file);
        return out;
    }
    std::vector<stdfs::path> first_objects() const override { return first_objs; }
    std::uint32_t library_gap() const override { return settings.gap; }
    std::optional<std::uint32_t> library_start() const override { return settings.start; }
    std::map<std::string, std::uint32_t> data_sizes() const override { return settings.sizes; }
    std::vector<DataPad> data_pads() const override { return settings.pads; }
};

// P0 with the kit's libraries (none when the kit is not there).
std::optional<LibProject> p0() {
    const stdfs::path kit = kit_dir() / "disk2" / "LIB";
    if (!stdfs::exists(kit / "SLIBC.LIB")) {
        std::printf("   (skipped: the msc300 kit is not at %s)\n", fs::to_text(kit).c_str());
        return std::nullopt;
    }
    LibProject p;
    const stdfs::path dir = testing::data_path("exact/testdata/data/p0");
    p.source_dir = dir / "src";
    const std::string fixture = fs::read_text(dir / "fixture.txt");
    for (std::string_view line : text::lines(fixture)) {
        const auto f = text::split(line, '\t');
        if (f[0] == "image") p.img = fs::read_file(dir / std::string(f[1]));
        else if (f[0] == "dgroup") p.dgroup = *text::parse_int<std::uint32_t>(f[1]);
        else if (f[0] == "sym") {
            const SymKind kind = f[2] == "C" ? SymKind::Code : f[2] == "D" ? SymKind::Data : f[2] == "S" ? SymKind::Segment : SymKind::Unknown;
            p.syms.insert_or_assign(std::string(f[1]), SymPlace{kind, *text::parse_int<std::int64_t>(f[3])});
        }
    }
    p.libs = {{"", kit / "SLIBC.LIB"}, {"SLIBFP", kit / "SLIBFP.LIB"}, {"EM", kit / "EM.LIB"}};
    p.kit_objs = {{"", kit / "SSETARGV.OBJ"}, {"", kit / "SVARSTCK.OBJ"}, {"", kit / "BINMODE.OBJ"}};
    p.first_objs = {kit / "BINMODE.OBJ"};
    return p;
}

// ---- small programs made of objects ------------------------------------------------------------------------------

// n bytes that look like code: a pseudo-random run made from `from` (no two runs are alike, and none holds another).
Bytes counting(std::size_t n, std::uint8_t from) {
    std::uint32_t x = 0x9E3779B9u * (from + 1u);
    Bytes b;
    for (std::size_t i = 0; i < n; ++i) {
        x ^= x << 13;
        x ^= x >> 17;
        x ^= x << 5;
        b.push_back(static_cast<std::uint8_t>(x >> 8 | 1));   // (never a zero byte: the program's zeros are where a pad is)
    }
    return b;
}

Bytes module_object(std::string_view name, const Bytes& code, std::string_view public_name) {
    OrderedMap<std::string, std::int64_t> pubs;
    pubs.insert_or_assign(std::string(public_name), 0);
    return omf::raw_object(name, code, pubs);
}

// A project whose program is `size` bytes (its data segment at `dg`), with an object per "library" (a file that is a
// single module is a library of one).
struct Small {
    fs::ScopedTempDir dir{"libtest"};
    LibProject p;
    int files = 0;

    explicit Small(std::uint32_t size = 0x6200, std::uint32_t dg = 0x6000) {
        p.source_dir = dir.path();
        p.img.assign(size, 0);
        p.dgroup = dg;
        for (std::size_t i = 0; i < size; ++i) p.img[i] = static_cast<std::uint8_t>(0xA0 + i % 7);   // (no zeros: stand-ins appear where asked)
    }
    // A library of one module, whose code stands in the program at `at` when `placed`.
    void add(const std::string& tag, const std::string& name, const Bytes& code, const std::string& pub, std::optional<std::uint32_t> at) {
        const stdfs::path f = dir.path() / (std::to_string(files++) + ".obj");
        fs::write_file(f, module_object(name, code, pub));
        p.libs.push_back({tag, f});
        if (at) std::copy(code.begin(), code.end(), p.img.begin() + *at);
    }
};

}  // namespace

CB_TEST("exact.library.p0_the_linked_libraries_found", [] {
    const auto project = p0();
    if (!project) return;
    const library::Map mp = library::locate(*project);
    const std::string golden = fs::read_text(testing::data_path("exact/testdata/library/p0_crtlib.txt"));
    CB_CHECK_EQ(text::join(library::lines(mp), "\n") + "\n", golden);
    CB_CHECK_EQ(*mp.start, 15352);
    CB_CHECK_EQ(mp.coded.size(), std::size_t(55));
    CB_CHECK_EQ(mp.donly.size(), std::size_t(3));
    CB_CHECK_EQ(mp.raws.size(), std::size_t(1));
    CB_CHECK(mp.lone.empty());
    CB_CHECK_EQ(mp.coded[0].module->label(), "amalloc");
    CB_CHECK_EQ(*mp.coded[0].code, 15352);
    CB_CHECK_EQ(*mp.coded[0].data, 5408);
    CB_CHECK_EQ(*mp.coded[0].end(), 15685);
    // atoi and atol are one jump to __catox: the one SYMBOLS names is taken
    const auto atoi = std::find_if(mp.coded.begin(), mp.coded.end(), [](const library::Placed& p) { return p.module->name == "atoi"; });
    CB_CHECK(atoi != mp.coded.end());
    CB_CHECK_EQ(atoi->why, "(by _atoi, which SYMBOLS names)");
    CB_CHECK_EQ(atoi->also.size(), std::size_t(1));
    CB_CHECK_EQ(atoi->also[0]->name, "atol");
    CB_CHECK_EQ(mp.objects.size(), std::size_t(1));
    CB_CHECK_EQ(mp.objects[0].label, "SVARSTCK.OBJ");
    CB_CHECK_EQ(mp.objects[0].start, 0x3b61);
    CB_CHECK_EQ(mp.objects[0].end - mp.objects[0].start, 151);
    CB_CHECK_EQ(mp.missing.size(), std::size_t(8));
    CB_CHECK_EQ(mp.missing.at("_malloc").named_by, "_filbuf");
    CB_CHECK(mp.missing.at("_malloc").defined_by == std::vector<std::string>{"nmalloc"});
    CB_CHECK(mp.missing.at("_main").defined_by.empty());
    CB_CHECK_EQ(mp.order.size(), std::size_t(60));   // (the 66 lines less the 6 that name a module of the same bytes)
});

CB_TEST("exact.library.p0_the_command", [] {
    auto project = p0();
    if (!project) return;
    const std::string crtlib = fs::read_text(testing::data_path("exact/testdata/library/p0_crtlib.txt"));
    const std::string summary = fs::read_text(testing::data_path("exact/testdata/library/p0_summary.txt"));
    fs::ScopedTempDir tmp("libwrite");
    project->source_dir = tmp.path();
    testing::CapturedStd cap;
    CB_CHECK_EQ(library::run(*project, {"--write"}), 0);
    CB_CHECK_EQ(cap.out(), crtlib + "\n" + summary);
    CB_CHECK_EQ(fs::read_text(tmp.path() / "CRTLIB"), crtlib);
});

CB_TEST("exact.library.the_modules_of_a_library_and_what_they_say", [] {
    Small s;
    const Bytes code = counting(24, 0x31);
    s.add("TG", "mod", code, "_entry", 0x100);
    const auto members = library::read_modules(s.p);
    CB_CHECK_EQ(members.size(), std::size_t(1));
    const library::Member& m = *members[0];
    CB_CHECK_EQ(m.label(), "TG:(object)");
    CB_CHECK_EQ(m.rank, 0);
    CB_CHECK_EQ(m.code.size(), std::size_t(1));
    CB_CHECK_EQ(m.code[0].segment, "_TEXT");
    CB_CHECK(m.code[0].bytes == code);
    CB_CHECK_EQ(m.code[0].solid(), 24);
    CB_CHECK(!m.data);
    CB_CHECK_EQ(m.pubs.at("_entry").offset, 0u);
    CB_CHECK_EQ(*m.pubs.at("_entry").segment, "_TEXT");
    CB_CHECK_EQ(m.seglen("_TEXT").length, 24u);
    CB_CHECK_EQ(m.seglen("NOPE").length, 0u);
    CB_CHECK_EQ(m.seglen("NOPE").align, 1u);
    s.p.libs.push_back({"", s.dir.path() / "missing.lib"});
    CB_CHECK_THROWS_AS(IoError, library::read_modules(s.p));
});

CB_TEST("exact.library.where_the_bytes_of_a_module_stand", [] {
    Small s;
    const Bytes code = counting(40, 0x31);
    s.add("", "m", code, "_f", 0x700);
    const auto members = library::read_modules(s.p);
    const library::Piece& piece = members[0]->code[0];
    CB_CHECK((library::find_piece(s.p.img, piece, 0, 0x6000) == std::vector<std::size_t>{0x700}));
    CB_CHECK(library::find_piece(s.p.img, piece, 0x701, 0x6000).empty());   // (out of the range it is asked to look in)
    CB_CHECK(library::find_piece(s.p.img, piece, 0, 0x710).empty());
    std::copy(code.begin(), code.end(), s.p.img.begin() + 0x1234);
    CB_CHECK((library::find_piece(s.p.img, piece, 0, 0x6000) == std::vector<std::size_t>{0x700, 0x1234}));
    // the words the linker fills in do not count
    Bytes filled = code;
    filled[10] ^= 0xff;
    filled[11] ^= 0xff;
    std::copy(filled.begin(), filled.end(), s.p.img.begin() + 0x2000);
    library::Piece holed = piece;
    holed.hole.assign(holed.bytes.size(), false);
    holed.hole[10] = holed.hole[11] = true;
    holed.hole_total = 2;
    CB_CHECK_EQ(holed.solid(), 38);
    CB_CHECK((library::find_piece(s.p.img, holed, 0, 0x6000) == std::vector<std::size_t>{0x700, 0x1234, 0x2000}));
    CB_CHECK((library::find_piece(s.p.img, piece, 0, 0x6000) == std::vector<std::size_t>{0x700, 0x1234}));
});

CB_TEST("exact.library.a_module_is_placed_where_its_code_stands", [] {
    Small s;
    s.add("", "alpha", counting(30, 0x31), "_alpha", 0x300);
    s.add("", "beta", counting(30, 0x55), "_beta", 0x31e);
    s.add("", "gamma", counting(30, 0x77), "_gamma", 0x400);
    s.add("", "unused", counting(30, 0x99), "_unused", std::nullopt);   // (not in the program)
    s.p.settings.start = 0x300;
    s.p.end_of_code = 0x41e;
    const library::Map mp = library::locate(s.p);
    CB_CHECK_EQ(mp.coded.size(), std::size_t(3));
    CB_CHECK_EQ(*mp.coded[0].code, 0x300);
    CB_CHECK_EQ(mp.coded[0].module->name, "(object)");
    CB_CHECK_EQ(*mp.coded[2].code, 0x400);
    CB_CHECK_EQ(*mp.coded[1].end(), 0x31e + 30);
    CB_CHECK(mp.donly.empty() && mp.first.empty());
    CB_CHECK_EQ(*mp.start, 0x300);
    // the gap between 0x33c and 0x400 is not zeros: it is a stand-in, between the modules in the listing
    CB_CHECK_EQ(mp.raws.size(), std::size_t(1));
    CB_CHECK_EQ(mp.raws[0].lo, 0x31e + 30);
    CB_CHECK_EQ(mp.raws[0].hi, 0x400);
    const auto listing = library::lines(mp);
    CB_CHECK_EQ(listing.size(), std::size_t(2 + 4));
    CB_CHECK(listing[2].starts_with("(object)         # code 00300 (30 bytes)"));
    CB_CHECK(listing[3].starts_with("(object)         # code 0031e (30 bytes)"));
    CB_CHECK(listing[4].starts_with("RAW lib0033c code=0033c-00400"));
    CB_CHECK(listing[5].starts_with("(object)         # code 00400 (30 bytes)"));
});

CB_TEST("exact.library.the_library_part_begins_after_the_last_long_gap", [] {
    Small s;
    s.add("", "one", counting(30, 0x31), "_one", 0x100);
    s.add("", "two", counting(30, 0x55), "_two", 0x200);
    s.add("", "far", counting(30, 0x77), "_far", 0x5000);
    s.p.end_of_code = 0x5100;
    auto locate = [&] { return library::locate(s.p); };
    {   // the 4096-byte gap before 0x5000 is the last: a lone module among the program's own is not the start
        const library::Map mp = locate();
        CB_CHECK_EQ(*mp.start, 0x5000);
        CB_CHECK_EQ(mp.coded.size(), std::size_t(1));
        CB_CHECK_EQ(mp.lone.size(), std::size_t(2));
        CB_CHECK_EQ(*mp.lone[0].code, 0x100);
    }
    s.p.settings.gap = 0x10000;   // (a gap too long to be one)
    CB_CHECK_EQ(*locate().start, 0x100);
    CB_CHECK_EQ(locate().coded.size(), std::size_t(3));
    s.p.settings.gap = 4096;
    s.p.settings.start = 0x200;   // (the project says it outright)
    {
        const library::Map mp = locate();
        CB_CHECK_EQ(*mp.start, 0x200);
        CB_CHECK_EQ(mp.coded.size(), std::size_t(2));
        CB_CHECK_EQ(mp.lone.size(), std::size_t(1));
    }
    s.p.settings.start = 0x6000;
    CB_CHECK(library::locate(s.p).coded.empty());
});

CB_TEST("exact.library.modules_of_the_same_bytes_are_one_place", [] {
    Small s;
    const Bytes code = counting(30, 0x31);
    s.add("", "first", code, "_first", 0x300);
    s.add("B", "second", code, "_second", std::nullopt);
    s.p.settings.start = 0x300;
    {   // nothing tells them apart: the earliest library's
        const library::Map mp = library::locate(s.p);
        CB_CHECK_EQ(mp.coded.size(), std::size_t(1));
        CB_CHECK_EQ(mp.coded[0].module->label(), "(object)");
        CB_CHECK_EQ(mp.coded[0].also.size(), std::size_t(1));
        CB_CHECK_EQ(mp.coded[0].also[0]->label(), "B:(object)");
        const auto listing = library::lines(mp);
        CB_CHECK(listing[2].starts_with("(object)         # code 00300 (30 bytes)"));
        CB_CHECK(listing[3].starts_with("B:(object)       # code 00300  (the same bytes as (object))"));
    }
    // the one whose public name the program's callers use is taken
    s.p.syms.insert_or_assign("_second", SymPlace{SymKind::Code, 0x300});
    const library::Map mp = library::locate(s.p);
    CB_CHECK_EQ(mp.coded[0].module->label(), "B:(object)");
    CB_CHECK_EQ(mp.coded[0].also[0]->label(), "(object)");
});

CB_TEST("exact.library.a_module_too_short_to_be_told_is_placed_by_its_name", [] {
    Small s;
    s.add("", "long", counting(30, 0x31), "_long", 0x300);
    const Bytes jump{0xe9, 0x10, 0x02};   // (a jump: three bytes tell nothing among thousands)
    s.add("", "jump", jump, "_jump", std::nullopt);
    std::copy(jump.begin(), jump.end(), s.p.img.begin() + 0x330);
    s.p.settings.start = 0x300;
    s.p.syms.insert_or_assign("_jump", SymPlace{SymKind::Code, 0x330});
    // as short as it is, the bytes are found at one place: only a name decides where more than one
    const library::Map mp = library::locate(s.p);
    CB_CHECK_EQ(mp.coded.size(), std::size_t(2));
    CB_CHECK_EQ(*mp.coded[1].code, 0x330);
    s.p.img[0x500] = jump[0];   // (and where there are several, SYMBOLS names one)
    s.p.img[0x501] = jump[1];
    s.p.img[0x502] = jump[2];
    const library::Map again = library::locate(s.p);
    CB_CHECK_EQ(again.coded.size(), std::size_t(2));
    CB_CHECK_EQ(again.coded[1].why, "(by its name in SYMBOLS)");
    CB_CHECK_EQ(*again.coded[1].code, 0x330);
});

CB_TEST("exact.library.a_library_that_is_not_there_is_an_error", [] {
    Small s;
    s.add("", "long", counting(30, 0x31), "_long", 0x300);
    s.p.settings.start = 0x300;
    s.p.libs.push_back({"", s.dir.path() / "nothere.lib"});
    CB_CHECK_THROWS_AS(IoError, library::locate(s.p));
});
