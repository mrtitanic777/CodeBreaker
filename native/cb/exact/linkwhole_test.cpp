#include "cb/exact/linkwhole.hpp"

#include <map>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/omf.hpp"

using namespace cb;
using namespace cb::exact;
namespace stdfs = std::filesystem;
namespace omf = cb::formats::omf;

// The expected values below are what the Python module this one replaced answered on the same inputs (its harness on
// the LINK 1.02 .. 2.44 and LIB projects of the Microsoft-Assemblers-Compilers research), the ones the tests here can
// make without a compiler: the names of the objects, what a project accepts to differ, the object's own records. Each
// was read through for what the linker would be given.
//
// DIVERGES: the Python raised TypeError at compare time for "tail_zeros": "lots" (a text that is no number); here the
// entry is refused when it is read (ParseError). "tail_zeros": "" is, as there, "nothing accepted". A link entry that
// names "emulate": "" counts as naming none (the Python: wanted, then a KeyError for "linker").

namespace {

using linkwhole::LinkConfig;

Bytes object_with(std::string_view title, const Bytes& code, std::string_view public_name) {
    OrderedMap<std::string, std::int64_t> pubs;
    pubs.insert_or_assign(std::string(public_name), 0);
    return omf::raw_object(title, code, pubs);
}

// n bytes that look like code: a pseudo-random run made from `from` (no two runs are alike, and none holds another).
Bytes counting(std::size_t n, std::uint8_t from) {
    std::uint32_t x = 0x9E3779B9u * (from + 1u);
    Bytes b;
    for (std::size_t i = 0; i < n; ++i) {
        x ^= x << 13;
        x ^= x >> 17;
        x ^= x << 5;
        b.push_back(static_cast<std::uint8_t>(x >> 8));
    }
    return b;
}

// Compiles a file to the object its text names: "obj:NAME:N" is an object of N bytes with the public NAME.
struct FakeToolchain final : modules::Toolchain {
    mutable std::vector<std::string> compiled;
    bool fail = false;
    std::optional<std::string> profile(std::string_view) const override { return std::nullopt; }
    modules::CompileResult compile(ByteView source, const modules::CompileOptions&) const override {
        const std::string said(as_text(source));
        compiled.push_back(said);
        if (fail) return {std::nullopt, "t.p(1): error\n" + std::string(2000, 'x') + "the end"};
        const auto parts = text::split(said, ':');
        const std::size_t n = text::parse_int<std::size_t>(parts.at(2)).value();
        return {object_with(said, counting(n, static_cast<std::uint8_t>(parts.at(1)[0])), parts.at(1)), "ok"};
    }
};

struct FakeDos final : linkwhole::DosHost {
    mutable std::vector<std::string> commands;
    mutable std::vector<std::string> files;          // the folder's files when the linker was run
    mutable std::string response;                    // L.LNK, when there was one
    mutable std::optional<std::uint32_t> psp;
    mutable std::map<std::string, Bytes> objects;    // NAME.OBJ as the linker saw it
    Bytes program = Bytes{'M', 'Z', 1, 2, 3};
    std::string console = "Microsoft (R) Overlay Linker  Version 3.00\r\nCopyright\r\nWarning: no stack segment\r\n";
    std::string run(const stdfs::path&, const stdfs::path& workdir, std::string_view command_line, std::optional<std::uint32_t> psp_in) const override {
        commands.emplace_back(command_line);
        psp = psp_in;
        for (const auto& e : stdfs::directory_iterator(workdir)) {
            files.push_back(e.path().filename().string());
            if (e.path().extension() == ".OBJ") objects[e.path().filename().string()] = fs::read_file(e.path());
        }
        if (stdfs::exists(workdir / "L.LNK")) response = fs::read_text(workdir / "L.LNK");
        fs::write_file(workdir / "NEW.EXE", program);
        fs::write_text(workdir / "NEW.MAP", "map\n");
        return console;
    }
};

struct FakeSet final : linkwhole::Project {
    stdfs::path folder;
    std::shared_ptr<FakeToolchain> tc = std::make_shared<FakeToolchain>();
    std::vector<std::pair<std::uint32_t, stdfs::path>> files;
    LinkConfig config;
    Bytes img, original;
    std::vector<std::shared_ptr<const linkwhole::Project>> others;
    std::shared_ptr<FakeToolchain> found_tc = std::make_shared<FakeToolchain>();

    stdfs::path dir() const override { return folder; }
    stdfs::path src() const override { return folder; }
    std::vector<std::pair<std::uint32_t, stdfs::path>> sources() const override { return files; }
    std::shared_ptr<const modules::Toolchain> toolchain() const override { return tc; }
    std::map<std::string, Bytes> headers() const override { return {}; }
    std::vector<std::string> include() const override { return {}; }
    std::optional<std::string> switches() const override { return std::nullopt; }
    std::vector<std::shared_ptr<const linkwhole::Project>> sets() const override {
        std::vector<std::shared_ptr<const linkwhole::Project>> out{std::shared_ptr<const linkwhole::Project>(std::shared_ptr<void>(), this)};
        out.insert(out.end(), others.begin(), others.end());
        return out;
    }
    const LinkConfig& link() const override { return config; }
    ByteView image() const override { return img; }
    ByteView file() const override { return original; }
    std::string exe() const override { return "x/PROG.EXE"; }
    SymbolTable symbols() const override { return {}; }
    stdfs::path resolve(std::string_view spec) const override { return folder / std::string(spec); }
    std::shared_ptr<const modules::Toolchain> load_toolchain(std::string_view, const std::optional<std::string>&) const override { return found_tc; }
};

// A folder of source files: each file's text says what it compiles to.
struct Sources {
    fs::ScopedTempDir dir{"lwtest"};
    void add(const std::string& name, const std::string& text) { fs::write_text(dir.path() / name, text); }
};

Bytes program_file() {   // an MZ-shaped file of 152 bytes whose last 30 are zero: what the compare tests hold things to
    Bytes ref{0x4D, 0x5A, 0x10, 0, 0x03, 0, 0, 0, 0x04, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    for (std::uint8_t b = 40; b < 140; ++b) ref.push_back(b);
    ref.resize(ref.size() + 30, 0);
    return ref;
}

LinkConfig accept_config(const std::string& json) { return linkwhole::parse_link_config(json); }

}  // namespace

CB_TEST("exact.linkwhole.the_link_entry_is_read", [] {
    const LinkConfig c = linkwhole::parse_link_config(R"({
        "linker": "image/LINK.EXE", "switches": "/STACK:3072", "output": "LC", "map": false, "names": "pairs:E",
        "placed": "p([0-9a-f]+)\\.plm", "last": ["z.plm"], "find": [{"file": "m.asm", "toolchain": "masm", "variant": "4"}, {"file": "n.asm", "toolchain": "masm"}],
        "first": [["CRT0", "kit/CRT0.OBJ"]], "libraries": ["KIT:lib/SLIBC.LIB"],
        "class": {"INIT": {"segments": ["CODE", "DATA"], "files": ["a.c"]}, "LATE": {"files": ["b.c"]}},
        "accept": [{"tail_zeros": "record", "why": "r"}, {"tail_zeros": 128}, {"header": ["0x0a", 14], "why": "h"}], "psp": "0x1234"})");
    CB_CHECK(linkwhole::wanted(c));
    CB_CHECK_EQ(*c.linker, "image/LINK.EXE");
    CB_CHECK_EQ(c.switches, "/STACK:3072");
    CB_CHECK_EQ(c.output, "LC");
    CB_CHECK(!c.map);
    CB_CHECK(c.names->kind == linkwhole::NameScheme::Kind::Pairs);
    CB_CHECK_EQ(c.names->prefix, 'E');
    CB_CHECK_EQ(c.find.size(), std::size_t(2));
    CB_CHECK_EQ(*c.find[0].variant, "4");
    CB_CHECK(!c.find[1].variant);
    CB_CHECK_EQ(c.first[0].name, "CRT0");
    CB_CHECK_EQ(c.libraries[0], "KIT:lib/SLIBC.LIB");
    CB_CHECK_EQ(c.classes.at("INIT").segments.size(), std::size_t(2));
    CB_CHECK_EQ(c.classes.at("LATE").files[0], "b.c");
    CB_CHECK(c.accept[0].tail_zeros->fill_record);
    CB_CHECK_EQ(c.accept[1].tail_zeros->count, 128u);
    CB_CHECK_EQ(c.accept[2].header->from, 10u);
    CB_CHECK_EQ(c.accept[2].header->to, 14u);
    CB_CHECK_EQ(c.accept[2].why, "h");
    CB_CHECK_EQ(*c.psp, 0x1234u);
});

CB_TEST("exact.linkwhole.what_makes_a_project_linked_this_way", [] {
    CB_CHECK(linkwhole::wanted(linkwhole::parse_link_config(R"({"linker": "L.EXE"})")));
    CB_CHECK(linkwhole::wanted(linkwhole::parse_link_config(R"({"emulate": "link24"})")));
    CB_CHECK(linkwhole::wanted(linkwhole::parse_link_config(R"({"linker": "a", "emulate": "b"})")));
    CB_CHECK(!linkwhole::wanted(linkwhole::parse_link_config(R"({"accept": []})")));
    CB_CHECK(!linkwhole::wanted(linkwhole::parse_link_config("{}")));
    CB_CHECK(!linkwhole::wanted(linkwhole::parse_link_config("")));
    CB_CHECK(!linkwhole::wanted(linkwhole::parse_link_config("null")));
    CB_CHECK(!linkwhole::wanted(linkwhole::parse_link_config(R"({"linker": null})")));
});

CB_TEST("exact.linkwhole.a_damaged_link_entry_is_refused", [] {
    CB_CHECK_THROWS_AS(ParseError, linkwhole::parse_link_config(R"([1, 2])"));
    CB_CHECK_THROWS_AS(ParseError, linkwhole::parse_link_config(R"({"linker": 3})"));
    CB_CHECK_THROWS_AS(ParseError, linkwhole::parse_link_config(R"({"accept": [{"tail_zeros": "lots"}]})"));
    CB_CHECK_THROWS_AS(ParseError, linkwhole::parse_link_config(R"({"accept": [{"header": [1]}]})"));
    CB_CHECK_THROWS_AS(ParseError, linkwhole::parse_link_config(R"({"psp": -1})"));
    CB_CHECK_THROWS_AS(ParseError, linkwhole::parse_link_config(R"({"names": "pairs:AB"})"));
    CB_CHECK_THROWS_AS(ParseError, linkwhole::parse_link_config(R"({"find": [{"file": "a"}]})"));
});

CB_TEST("exact.linkwhole.the_names_of_the_objects", [] {
    const auto name = [](const std::string& scheme, std::size_t k, const std::string& file = "P08D0F.PLM") {
        return linkwhole::object_name(scheme.empty() ? std::nullopt : std::optional(linkwhole::parse_link_config("{\"names\": \"" + scheme + "\"}").names.value()), k, file);
    };
    CB_CHECK_EQ(name("", 0), "MA");
    CB_CHECK_EQ(name("", 25), "MZ");
    CB_CHECK_EQ(name("", 26), "M0");
    CB_CHECK_EQ(name("", 35), "M9");
    CB_CHECK_EQ(name("", 36), "M36");
    CB_CHECK_EQ(name("", 100), "M100");
    CB_CHECK_EQ(name("sequence:Q", 1), "QB");
    CB_CHECK_EQ(name("sequence:Q", 37), "Q37");
    CB_CHECK_EQ(name("sequence:", 2), "MC");
    CB_CHECK_EQ(name("pairs:D", 0), "DA");
    CB_CHECK_EQ(name("pairs:D", 27), "EB");
    CB_CHECK_EQ(name("pairs:E", 26), "FA");
    CB_CHECK_EQ(name("pairs", 52), "FA");
    CB_CHECK_EQ(name("files", 0), "P08D0F");
    CB_CHECK_EQ(name("files", 0, "my-file_2.c"), "MYFILE2");
    CB_CHECK_EQ(name("files", 0, "toolongfilename123.asm"), "TOOLONGF");
    CB_CHECK_EQ(name("other:Z", 3), "ZD");   // (a kind it does not know is a sequence)
    CB_CHECK_EQ(linkwhole::object_name(std::nullopt, 28, "x.c"), "M2");
});

CB_TEST("exact.linkwhole.the_file_against_the_original", [] {
    const Bytes ref = program_file();
    const auto cmp = [&](const std::string& entry, Bytes exe) { return linkwhole::compare(accept_config(entry), ref, exe); };
    const auto with = [&](auto edit) {
        Bytes b = ref;
        edit(b);
        return b;
    };
    const Bytes cut30(ref.begin(), ref.end() - 30);
    // nothing accepted
    CB_CHECK(cmp("{}", ref).identical);
    auto r = cmp("{}", cut30);
    CB_CHECK(!r.identical);
    CB_CHECK_EQ(r.notes.at(0), "the file is 122 bytes, the original 152");
    r = cmp("{}", with([](Bytes& b) { b[10] = 99; }));
    CB_CHECK_EQ(r.notes.at(0), "1 bytes differ, the first at file offset 0000a (the header)");
    r = cmp("{}", with([](Bytes& b) { b[0x3c] = 0xee; }));
    CB_CHECK_EQ(r.notes.at(0), "1 bytes differ, the first at file offset 0003c (the header)");   // (the header is 0x40 bytes: 4 paragraphs)
    // 30 zero bytes more at the end
    r = cmp(R"({"accept": [{"tail_zeros": 30, "why": "thirty"}]})", cut30);
    CB_CHECK(r.identical);
    CB_CHECK_EQ(r.notes.at(0), "the original has 30 zero bytes more at its end (thirty)");
    CB_CHECK(!cmp(R"({"accept": [{"tail_zeros": 30}]})", Bytes(ref.begin(), ref.end() - 29)).identical);
    CB_CHECK(!cmp(R"({"accept": [{"tail_zeros": 128, "why": "a record"}]})", cut30).identical);
    CB_CHECK(!cmp(R"({"accept": [{"tail_zeros": 0}]})", cut30).identical);
    CB_CHECK(!cmp(R"({"accept": [{"tail_zeros": ""}]})", cut30).identical);
    CB_CHECK(!cmp(R"({"accept": [{"tail_zeros": 30}]})", Bytes(ref.begin(), ref.end() - 31)).identical);
    // as many as fill the last 128-byte record: 122 -> 6 would be needed, the original has 30
    CB_CHECK(!cmp(R"({"accept": [{"tail_zeros": "record"}]})", cut30).identical);
    Bytes ref_record(100, 7);
    ref_record.resize(128, 0);
    CB_CHECK(linkwhole::compare(accept_config(R"({"accept": [{"tail_zeros": "record"}]})"), ref_record, Bytes(100, 7)).identical);
    // header bytes: accepted when they differ (with the checksum)
    const std::string hdr = R"({"accept": [{"header": [10, 14], "why": "load addresses"}]})";
    r = cmp(hdr, with([](Bytes& b) { b[10] = 99; }));
    CB_CHECK(r.identical);
    CB_CHECK_EQ(r.notes.at(0), "header bytes 0a..0d and the checksum (load addresses)");
    CB_CHECK(cmp(hdr, with([](Bytes& b) { b[12] = 0x7f; b[13] = 0x7e; })).identical);
    CB_CHECK(!cmp(hdr, with([](Bytes& b) { b[0x12] = 0x55; })).notes.empty());   // (the checksum alone is not accepted as a header byte)
    CB_CHECK(!cmp(hdr, with([](Bytes& b) { b[0x12] = 0x55; })).identical);
    CB_CHECK(cmp(R"({"accept": [{"header": ["0x0a", "0x10"], "why": "text numbers"}]})", with([](Bytes& b) { b[10] = 99; b[0x12] = 1; })).identical);
    CB_CHECK(!cmp(hdr, with([](Bytes& b) { b[60] = 0xee; })).identical);
    // both entries at once: a short file with differing header bytes
    r = cmp(R"({"accept": [{"tail_zeros": 30, "why": "z"}, {"header": [10, 14], "why": "h"}]})", with([](Bytes& b) { b[10] = 99; b.resize(122); }));
    CB_CHECK(r.identical);
    CB_CHECK_EQ(r.notes.size(), std::size_t(2));
    // files that are not programs at all
    CB_CHECK(!cmp("{}", Bytes{}).identical);
    CB_CHECK_EQ(cmp("{}", Bytes{}).notes.at(0), "the file is 0 bytes, the original 152");
    const Bytes tiny{'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I'};
    Bytes other = tiny;
    other.back() = 'J';
    CB_CHECK_EQ(linkwhole::compare({}, tiny, other).notes.at(0), "1 bytes differ, the first at file offset 00008 (image 00008)");
});

CB_TEST("exact.linkwhole.the_objects_own_records", [] {
    const Bytes obj = object_with("t.c", counting(10, 1), "_f");
    CB_CHECK(equal_bytes(linkwhole::own_records(obj), obj));
    Bytes padded = obj;
    padded.resize(obj.size() + 300, 0);   // (an assembler's file padded to its disk sector)
    CB_CHECK(equal_bytes(linkwhole::own_records(padded), obj));
    CB_CHECK_EQ(linkwhole::own_records(Bytes{}).size(), std::size_t(0));
    const Bytes no_end{0x80, 0x02, 0x00, 0x00, 0x00};
    CB_CHECK_EQ(linkwhole::own_records(no_end).size(), no_end.size());
    const Bytes cut(obj.begin(), obj.begin() + static_cast<std::ptrdiff_t>(obj.size() / 2));
    CB_CHECK_EQ(linkwhole::own_records(cut).size(), cut.size());
});

CB_TEST("exact.linkwhole.segments_given_a_class", [] {
    const Bytes obj = object_with("t.c", counting(10, 1), "_f");
    const omf::Object before = omf::read(obj);
    CB_CHECK_EQ(before.segdefs.at(0).name, "_TEXT");
    CB_CHECK_EQ(before.segdefs.at(0).class_name, "CODE");
    // the segment is named as it is, or by a name that ends in it
    for (const std::string& want : {std::string("_TEXT"), std::string("_text"), std::string("TEXT")}) {
        const Bytes again = linkwhole::reclass(obj, "INIT", {want});
        const omf::Object o = omf::read(again);
        CB_CHECK_EQ(o.segdefs.at(0).class_name, "INIT");
        CB_CHECK(o.publics.at(0).name == "_f" && o.segments.at("_TEXT").size() >= 10);
    }
    // a segment of no such name keeps its class (and the object is written again, with its checksums right)
    const Bytes same = linkwhole::reclass(obj, "INIT", {"DATA"});
    CB_CHECK_EQ(omf::read(same).segdefs.at(0).class_name, "CODE");
    CB_CHECK_EQ(same.size(), obj.size() + 5);   // (the class's name is in the first LNAMES now: 1 + 4 bytes)
    const Bytes none = linkwhole::reclass(obj, "INIT", {});
    CB_CHECK_EQ(omf::read(none).segdefs.at(0).class_name, "CODE");
    // a record that runs past the end
    CB_CHECK_THROWS_AS(FormatError, linkwhole::reclass(Bytes(obj.begin(), obj.begin() + 20), "INIT", {"_TEXT"}));
    CB_CHECK_EQ(linkwhole::reclass(Bytes{}, "INIT", {"_TEXT"}).size(), std::size_t(0));
});

CB_TEST("exact.linkwhole.the_place_of_an_objects_code", [] {
    FakeSet p;
    const Bytes code = counting(24, 0x30);
    const Bytes obj = object_with("t.c", code, "_f");
    p.img = counting(200, 0x90);
    std::copy(code.begin(), code.end(), p.img.begin() + 77);
    CB_CHECK_EQ(*linkwhole::place_of(p, obj), 77u);
    // the code twice: no place; the code absent: none
    std::copy(code.begin(), code.end(), p.img.begin() + 150);
    CB_CHECK(!linkwhole::place_of(p, obj));
    CB_CHECK(!linkwhole::place_of(p, object_with("t.c", counting(24, 0x11), "_g")));
    // too few bytes to tell by
    p.img = counting(200, 0x90);
    const Bytes few = counting(7, 0x30);
    std::copy(few.begin(), few.end(), p.img.begin() + 40);
    CB_CHECK(!linkwhole::place_of(p, object_with("t.c", few, "_h")));
    const Bytes eight = counting(8, 0x30);
    std::copy(eight.begin(), eight.end(), p.img.begin() + 40);
    CB_CHECK_EQ(*linkwhole::place_of(p, object_with("t.c", eight, "_h")), 40u);
});

CB_TEST("exact.linkwhole.the_objects_in_the_order_of_their_places", [] {
    Sources s;
    s.add("b.plm", "obj:B:12");
    s.add("a.plm", "obj:A:12");
    s.add("c.plm", "obj:C:12");
    s.add("z.plm", "obj:Z:12");
    s.add("p0400.inc", "obj:P:12");
    s.add("p0100.inc", "obj:Q:12");
    s.add("p0200.txt", "obj:R:12");   // (a name the pattern does not take)
    s.add("find.asm", "obj:F:20");
    FakeSet p;
    p.folder = s.dir.path();
    p.files = {{0x300, s.dir.path() / "b.plm"}, {0x100, s.dir.path() / "a.plm"}, {0x200, s.dir.path() / "c.plm"}};
    p.config.placed = "p([0-9a-f]+)\\.INC";
    p.config.last = {"z.plm"};
    p.config.find = {{"find.asm", "masm", std::nullopt}};
    // the program holds find.asm's code at 0x180
    p.img = counting(0x800, 0x90);
    const Bytes fcode = counting(20, 0x46);
    std::copy(fcode.begin(), fcode.end(), p.img.begin() + 0x180);
    const auto objs = linkwhole::placed_objects(p);
    std::vector<std::string> names;
    std::vector<std::uint32_t> places;
    for (const auto& o : objs) {
        names.push_back(o.name);
        places.push_back(o.place);
    }
    CB_CHECK((names == std::vector<std::string>{"a.plm", "p0100.inc", "find.asm", "c.plm", "b.plm", "p0400.inc", "z.plm"}));
    CB_CHECK((places == std::vector<std::uint32_t>{0x100, 0x100, 0x180, 0x200, 0x300, 0x400, linkwhole::kLastPlace}));
    // the objects are what their toolchains made of them
    CB_CHECK_EQ(omf::read(objs[0].object).publics.at(0).name, "A");
    CB_CHECK_EQ(omf::read(objs[2].object).publics.at(0).name, "F");
    CB_CHECK_EQ(p.found_tc->compiled.size(), std::size_t(1));   // (the "find" file by its own toolchain, the rest by the set's)
    CB_CHECK_EQ(p.tc->compiled.size(), std::size_t(6));
});

CB_TEST("exact.linkwhole.a_file_that_will_not_compile_or_be_found", [] {
    Sources s;
    s.add("a.plm", "obj:A:12");
    FakeSet p;
    p.folder = s.dir.path();
    p.files = {{0x100, s.dir.path() / "a.plm"}};
    p.tc->fail = true;
    try {
        (void)linkwhole::placed_objects(p);
        CB_CHECK(false);
    } catch (const linkwhole::LinkError& e) {
        const std::string what = e.what();
        CB_CHECK(what.starts_with("a.plm does not compile:"));
        CB_CHECK(what.ends_with("the end"));
        CB_CHECK(what.size() < 1600);   // (the last 1500 characters of what the compiler said)
    }
    p.tc->fail = false;
    s.add("find.asm", "obj:F:20");
    p.img = counting(0x100, 0x90);
    p.config.find = {{"find.asm", "masm", std::nullopt}};
    CB_CHECK_THROWS_AS(linkwhole::LinkError, linkwhole::placed_objects(p));
    p.found_tc->fail = true;
    CB_CHECK_THROWS_AS(linkwhole::LinkError, linkwhole::placed_objects(p));
});

CB_TEST("exact.linkwhole.the_modules_a_file_lists", [] {
    Sources s;
    fs::ScopedTempDir other("lwother");
    s.add("order", "# the order\n\nb.plm odd  # why\n   \na.plm\n");
    s.add("a.plm", "obj:A:12");
    fs::write_text(other.path() / "b.plm", "obj:B:14");
    FakeSet p;
    p.folder = s.dir.path();
    auto second = std::make_shared<FakeSet>();
    second->folder = other.path();
    p.others = {second};
    p.config.order = "order";
    const auto mods = linkwhole::listed(p, p.config);
    CB_CHECK_EQ(mods.size(), std::size_t(2));
    CB_CHECK_EQ(mods[0].index, std::size_t(0));
    CB_CHECK_EQ(mods[0].name, "b.plm");
    CB_CHECK(mods[0].odd);
    CB_CHECK_EQ(mods[1].name, "a.plm");
    CB_CHECK(!mods[1].odd);
    CB_CHECK_EQ(second->tc->compiled.size(), std::size_t(1));   // (b.plm is the second set's: its toolchain compiled it)
    CB_CHECK_EQ(p.tc->compiled.size(), std::size_t(1));
    s.add("order", "missing.plm\n");
    CB_CHECK_THROWS_AS(linkwhole::LinkError, linkwhole::listed(p, p.config));
    CB_CHECK(linkwhole::listed(p, LinkConfig{}).empty());
});

CB_TEST("exact.linkwhole.the_linker_is_run_on_the_objects", [] {
    Sources s;
    s.add("a.plm", "obj:A:12");
    s.add("b.plm", "obj:B:12");
    s.add("LINK.EXE", "the linker");
    s.add("CRT0.OBJ", "crt0");
    s.add("SLIB.LIB", "lib");
    FakeSet p;
    p.folder = s.dir.path();
    p.files = {{0x100, s.dir.path() / "a.plm"}, {0x200, s.dir.path() / "b.plm"}};
    p.config = accept_config(R"({"linker": "LINK.EXE", "switches": "/NOI", "first": [["CRT0", "CRT0.OBJ"]], "libraries": ["SLIB.LIB"],
                                "class": {"INIT": {"segments": ["_TEXT"], "files": ["b.plm"]}}, "psp": 4096})");
    const auto objs = linkwhole::placed_objects(p);
    FakeDos dos;
    const linkwhole::LinkOutput r = linkwhole::link(p, objs, dos);
    CB_CHECK_EQ(dos.commands.at(0), "CRT0+MA+MB,NEW,NEW,SLIB/NOI;");
    CB_CHECK_EQ(*dos.psp, 4096u);
    CB_CHECK(dos.objects.contains("MA.OBJ") && dos.objects.contains("MB.OBJ") && dos.objects.contains("CRT0.OBJ"));
    CB_CHECK_EQ(omf::read(dos.objects["MA.OBJ"]).segdefs.at(0).class_name, "CODE");   // (a.plm has no class of its own)
    CB_CHECK_EQ(omf::read(dos.objects["MB.OBJ"]).segdefs.at(0).class_name, "INIT");
    CB_CHECK((*r.exe == Bytes{'M', 'Z', 1, 2, 3}));
    CB_CHECK_EQ(r.map, "map\n");
    CB_CHECK(r.console.find('\r') == std::string::npos);
    CB_CHECK(r.console.find("no stack segment") != std::string::npos);
    // a map of NUL, and a command line too long for DOS goes to a response file one answer a line
    p.config.map = false;
    p.config.names = linkwhole::NameScheme{linkwhole::NameScheme::Kind::Files, 'M'};
    std::vector<linkwhole::PlacedObject> many;
    for (int i = 0; i < 20; ++i) many.push_back({static_cast<std::uint32_t>(i), "module" + std::to_string(i) + ".plm", objs[0].object});
    FakeDos dos2;
    (void)linkwhole::link(p, many, dos2);
    CB_CHECK_EQ(dos2.commands.at(0), "@L.LNK");
    CB_CHECK(dos2.response.starts_with("CRT0+MODULE0+MODULE1+"));
    CB_CHECK(dos2.response.ends_with("\r\nNEW\r\nNUL\r\nSLIB/NOI;\r\n"));
    CB_CHECK(dos2.response.find("MODULE19") != std::string::npos);
    // no linker named
    p.config.linker.reset();
    CB_CHECK_THROWS_AS(linkwhole::LinkError, linkwhole::link(p, objs, dos));
});

CB_TEST("exact.linkwhole.a_kept_folder_stays", [] {
    Sources s;
    s.add("a.plm", "obj:A:12");
    s.add("LINK.EXE", "the linker");
    FakeSet p;
    p.folder = s.dir.path();
    p.files = {{0x100, s.dir.path() / "a.plm"}};
    p.config = accept_config(R"({"linker": "LINK.EXE", "output": "NEW"})");
    FakeDos dos;
    fs::ScopedTempDir keep("lwkeep");
    (void)linkwhole::link(p, linkwhole::placed_objects(p), dos, keep.path() / "work");
    CB_CHECK(stdfs::exists(keep.path() / "work" / "NEW.EXE"));
    CB_CHECK(stdfs::exists(keep.path() / "work" / "MA.OBJ"));
});

CB_TEST("exact.linkwhole.the_command", [] {
    Sources s;
    s.add("a.plm", "obj:A:12");
    s.add("LINK.EXE", "the linker");
    FakeSet p;
    p.folder = s.dir.path();
    p.files = {{0x100, s.dir.path() / "a.plm"}};
    p.config = accept_config(R"({"linker": "LINK.EXE"})");
    FakeDos dos;
    p.original = dos.program;
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(linkwhole::run(p, {"-v"}, dos), 0);
        const std::string out = cap.out();
        CB_CHECK(out.find("   00100 a.plm\n") != std::string::npos);
        CB_CHECK(out.find("   LINK: Warning: no stack segment\n") != std::string::npos);
        CB_CHECK(out.find("Copyright") == std::string::npos);
        CB_CHECK(out.find("PROG.EXE linked from 1 modules: 5 bytes; the FILE IDENTICAL\n") != std::string::npos);
    }
    {   // not the original
        p.original = Bytes{'M', 'Z', 1, 2, 9};
        testing::CapturedStd cap;
        CB_CHECK_EQ(linkwhole::run(p, {}, dos), 1);
        const std::string out = cap.out();
        CB_CHECK(out.find("   00100 a.plm") == std::string::npos);   // (no listing without -v)
        CB_CHECK(out.find("1 bytes differ, the first at file offset 00004 (image 00004)") != std::string::npos);
        CB_CHECK(out.find("PROG.EXE linked from 1 modules: 5 bytes; not identical\n") != std::string::npos);
    }
    {   // a linker that writes nothing
        struct Silent final : linkwhole::DosHost {
            std::string run(const stdfs::path&, const stdfs::path&, std::string_view, std::optional<std::uint32_t>) const override { return ""; }
        } silent;
        testing::CapturedStd cap;
        CB_CHECK_EQ(linkwhole::run(p, {}, silent), 1);
        CB_CHECK_EQ(cap.out(), "no program was written\n");
    }
    CB_CHECK_THROWS_AS(UsageError, linkwhole::run(p, {"--keep"}, dos));
    p.config = accept_config(R"({"emulate": "link99"})");
    CB_CHECK_THROWS_AS(UnsupportedError, linkwhole::run(p, {}, dos));
});
