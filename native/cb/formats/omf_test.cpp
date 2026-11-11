// Tests of cb/formats/omf.hpp on two real objects of Microsoft C 1.52 (testdata/fastadd.obj, txtonly.obj) and on objects and
// libraries built here from records.
//
// DIVERGES from the Python reader (see state/work/typed-port/_bugs/LIST.md): a record cut short by the end of the file is a
// TruncatedError when it is parsed (the Python took the shorter values); an index of 0 or past the end of a name/segment table is
// a FormatError (the Python indexed from the end of the list: 0 meant the last entry); a fix-up whose target is index 0 keeps the
// target Unknown.
#include "cb/formats/omf.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
namespace omf = cb::formats::omf;

namespace {

Bytes testdata(const char* name) { return fs::read_file(testing::data_path(std::string("formats/testdata/") + name)); }

Bytes cat(std::initializer_list<Bytes> parts) {
    Bytes out;
    for (const Bytes& p : parts) append(out, p);
    return out;
}

// A library of `modules` (each a whole object), page size `page`: the header record, the modules each padded to a page, and a
// dictionary stub at the end.
Bytes make_library(const std::vector<Bytes>& modules, std::size_t page) {
    Bytes out(page, 0);
    out[0] = 0xF0;
    out[1] = static_cast<std::uint8_t>(page - 3), out[2] = static_cast<std::uint8_t>((page - 3) >> 8);
    for (const Bytes& m : modules) {
        append(out, m);
        out.resize((out.size() + page - 1) / page * page, 0);
    }
    const std::uint32_t dict = static_cast<std::uint32_t>(out.size());
    for (int k = 0; k < 4; ++k) out[3 + static_cast<std::size_t>(k)] = static_cast<std::uint8_t>(dict >> (8 * k));
    out.resize(out.size() + 512, 0);  // the dictionary (not read)
    return out;
}

// The bytes of an object that only declares an external: THEADR, EXTDEF, MODEND.
Bytes object_with_extern(std::string_view title, std::string_view external) {
    return cat({omf::record(0x80, omf::name(title)), omf::record(0x8C, cat({omf::name(external), Bytes{0}})), omf::record(0x8A, Bytes{0})});
}

}  // namespace

CB_TEST("formats.omf.fastadd_real_object", [] {
    const Bytes d = testdata("fastadd.obj");
    const omf::Object o = omf::read(d);
    CB_CHECK_EQ(o.segdefs.size(), std::size_t{2});
    CB_CHECK_EQ(o.segdefs[0].name, "_TEXT");
    CB_CHECK_EQ(o.segdefs[0].class_name, "CODE");
    CB_CHECK_EQ(o.segdefs[0].length, std::int64_t{386});
    CB_CHECK_EQ(o.segdefs[1].name, "_DATA");
    CB_CHECK_EQ(o.segdefs[1].length, std::int64_t{0});
    CB_CHECK_EQ(o.publics.size(), std::size_t{1});
    CB_CHECK_EQ(o.publics[0].name, "__FASTLDADD");
    CB_CHECK_EQ(o.publics[0].segment.value(), "_TEXT");
    CB_CHECK_EQ(o.publics[0].offset, std::int64_t{6});
    CB_CHECK(o.externals.empty());
    CB_CHECK(o.fixups.empty());
    CB_CHECK(!o.start.has_value());
    CB_CHECK_EQ(o.segments.at("_TEXT").size(), std::size_t{386});
    const auto records = omf::records(d);
    CB_CHECK_EQ(records.front().type, 0x80);  // THEADR
    CB_CHECK(records.back().type == 0x8A || records.back().type == 0x8B);  // MODEND
    CB_CHECK_EQ(omf::publics(d).size(), std::size_t{1});
    CB_CHECK(omf::externs(d).empty());
    const auto functions = omf::functions(o, "_TEXT");
    CB_CHECK(!functions.empty());
    CB_CHECK_EQ(functions[0].name, "(module start)");  // the bytes before the first public
    CB_CHECK_EQ(functions[0].offset, std::int64_t{0});
    CB_CHECK_EQ(functions[1].name, "__FASTLDADD");
    CB_CHECK_EQ(functions[1].offset, std::int64_t{6});
    const auto code = omf::code_segment(d);
    CB_CHECK_EQ(code.segment.value(), "_TEXT");
    CB_CHECK(!code.code.empty());
    CB_CHECK_EQ(omf::code_segments(o).size(), std::size_t{1});
});

CB_TEST("formats.omf.listing_of_a_real_object", [] {
    const std::string expected =
        "== (object) (505 bytes)\n"
        "  segment _TEXT        class=CODE        386 bytes\n"
        "  segment _DATA        class=DATA          0 bytes\n"
        "  public  __FASTLDADD              _TEXT:0006\n";
    CB_CHECK_EQ(omf::listing(testdata("fastadd.obj")), expected);
    const std::string raw = omf::listing(testdata("fastadd.obj"), true);
    CB_CHECK(raw.starts_with("@000000 THEADR   len="));
    CB_CHECK(raw.find("SEGDEF") != std::string::npos);
    CB_CHECK(raw.find("MODEND") != std::string::npos);
});

CB_TEST("formats.omf.txtonly_publics_and_segments", [] {
    const omf::Object o = omf::read(testdata("txtonly.obj"));
    CB_CHECK_EQ(o.publics.size(), std::size_t{28});
    CB_CHECK_EQ(o.segdefs[0].name, "_GR_TEXT");
    CB_CHECK_EQ(o.segdefs[0].length, std::int64_t{5});
    CB_CHECK_EQ(o.publics[0].name, "B$Screen10");
    CB_CHECK_EQ(o.publics[4].name, "B$VWINI");
    CB_CHECK_EQ(o.publics[4].offset, std::int64_t{1});
    CB_CHECK_EQ(o.publics[10].name, "B$VYSIZ");
    CB_CHECK_EQ(o.publics[10].segment.value(), "_DATA");
    CB_CHECK_EQ(omf::code_segments(o).size(), std::size_t{1});  // _GR_TEXT ends in "_TEXT"
    const auto names = omf::names_of(testdata("txtonly.obj"));
    CB_CHECK_EQ(names.first.size(), std::size_t{28});
    CB_CHECK(names.second.empty());
});

CB_TEST("formats.omf.library_of_modules", [] {
    const Bytes a = testdata("fastadd.obj"), b = testdata("txtonly.obj");
    const Bytes lib = make_library({a, b, a}, 16);
    const auto modules = omf::library(lib);
    CB_CHECK_EQ(modules.size(), std::size_t{3});
    const ByteView title = omf::records(a)[0].payload;  // the THEADR record's name
    CB_CHECK_EQ(modules[0].first, text::from_latin1(title.subspan(1, title[0])));
    CB_CHECK_EQ(modules[0].second.size(), a.size());
    CB_CHECK_EQ(modules[1].second.size(), b.size());
    CB_CHECK(std::equal(modules[1].second.begin(), modules[1].second.end(), b.begin()));
    CB_CHECK_EQ(modules[2].first, modules[0].first + "#2");  // a name met twice
    // a plain object is a library of one
    const auto one = omf::library(a);
    CB_CHECK_EQ(one.size(), std::size_t{1});
    CB_CHECK_EQ(one[0].first, "(object)");
    CB_CHECK_EQ(omf::read(modules[1].second).publics.size(), std::size_t{28});
});

CB_TEST("formats.omf.record_checksums_and_names", [] {
    const Bytes r = omf::record(0x88, Bytes{1, 2, 3, 4, 5});
    CB_CHECK_EQ(r.size(), std::size_t{3 + 5 + 1});
    unsigned sum = 0;
    for (std::uint8_t c : r) sum += c;
    CB_CHECK_EQ(sum & 0xFF, 0u);  // the checksum makes the record sum to zero
    const auto parsed = omf::records(cat({r, omf::record(0x8A, Bytes{0})}));
    CB_CHECK_EQ(parsed.size(), std::size_t{2});
    CB_CHECK(parsed[0].type == 0x88 && parsed[0].payload.size() == 5);
    const Bytes n = omf::name("abc");
    CB_CHECK(n == (Bytes{3, 'a', 'b', 'c'}));
    CB_CHECK_THROWS_AS(FormatError, omf::name(std::string(256, 'x')));
    CB_CHECK_THROWS_AS(FormatError, omf::record(0x80, Bytes(0x10000, 0)));
});

CB_TEST("formats.omf.raw_object_reads_back", [] {
    const Bytes code{0x55, 0x8B, 0xEC, 0x5D, 0xC3};
    OrderedMap<std::string, std::int64_t> pubs;
    pubs.insert_or_assign("_g", 4);
    pubs.insert_or_assign("_f", 0);
    OrderedMap<std::string, std::int64_t> dpubs;
    dpubs.insert_or_assign("_var", 2);
    const Bytes obj = omf::raw_object("t.c", code, pubs, Bytes{1, 2, 3, 4}, dpubs);
    const omf::Object o = omf::read(obj);
    CB_CHECK_EQ(o.segdefs.size(), std::size_t{2});
    CB_CHECK_EQ(o.segdefs[0].name, "_TEXT");
    CB_CHECK_EQ(o.segdefs[0].class_name, "CODE");
    CB_CHECK_EQ(o.segdefs[1].name, "_DATA");
    CB_CHECK(o.segments.at("_TEXT") == code);
    CB_CHECK((o.segments.at("_DATA") == Bytes{1, 2, 3, 4}));
    CB_CHECK_EQ(o.groups.at("DGROUP").size(), std::size_t{1});
    CB_CHECK_EQ(o.publics.size(), std::size_t{3});
    CB_CHECK_EQ(o.publics[0].name, "_f");  // sorted by value
    CB_CHECK_EQ(o.publics[0].offset, std::int64_t{0});
    CB_CHECK_EQ(o.publics[1].name, "_g");
    CB_CHECK_EQ(o.publics[2].name, "_var");
    CB_CHECK_EQ(o.publics[2].segment.value(), "_DATA");
    const auto fb = omf::function_bytes(obj);
    CB_CHECK_EQ(fb.size(), std::size_t{2});
    CB_CHECK((fb.at("_f").code == Bytes{0x55, 0x8B, 0xEC, 0x5D}));
    CB_CHECK((fb.at("_g").code == Bytes{0xC3}));
    // a patch changes the bytes it names
    const Bytes patched = omf::patched(obj, "_TEXT+1=ff90");
    const omf::Object p = omf::read(patched);
    CB_CHECK((p.segments.at("_TEXT") == Bytes{0x55, 0xFF, 0x90, 0x5D, 0xC3}));
    CB_CHECK_THROWS_AS(UsageError, omf::patched(obj, "no spec here"));
    CB_CHECK_THROWS_AS(UsageError, omf::patched(obj, "_TEXT+1=f"));  // an odd number of hex digits
});

CB_TEST("formats.omf.bss_object_names", [] {
    OrderedMap<std::string, std::int64_t> names;
    names.insert_or_assign("_a", 0x4010);
    names.insert_or_assign("_b", 0x4000);
    const Bytes obj = omf::bss_object(names, 0x4000, 0x4100);
    const omf::Object o = omf::read(obj);
    CB_CHECK_EQ(o.segdefs.size(), std::size_t{1});
    CB_CHECK_EQ(o.segdefs[0].name, "_BSS");
    CB_CHECK_EQ(o.segdefs[0].length, std::int64_t{0x100});
    CB_CHECK_EQ(o.publics.size(), std::size_t{2});
    CB_CHECK_EQ(o.publics[0].name, "_b");
    CB_CHECK_EQ(o.publics[0].offset, std::int64_t{0});
    CB_CHECK_EQ(o.publics[1].name, "_a");
    CB_CHECK_EQ(o.publics[1].offset, std::int64_t{0x10});
});

CB_TEST("formats.omf.renaming", [] {
    const Bytes obj = object_with_extern("m", "_foo");
    CB_CHECK_EQ(omf::externs(obj).size(), std::size_t{1});
    CB_CHECK_EQ(omf::externs(obj)[0], "_foo");
    const Bytes renamed = omf::rename_externs(obj, {{"_foo", "_bar_baz"}, {"_unused", "_x"}});
    CB_CHECK_EQ(omf::externs(renamed)[0], "_bar_baz");
    CB_CHECK_EQ(omf::externs(omf::rename_externs(obj, {}))[0], "_foo");  // not in the mapping: kept
    // an EXTDEF cut after its last name (no type index): renamed, and written back without one
    const Bytes cut = cat({omf::record(0x80, omf::name("m")), omf::record(0x8C, omf::name("_foo")), omf::record(0x8A, Bytes{0})});
    CB_CHECK(omf::rename_externs(cut, {{"_foo", "_bar"}}) ==
             cat({omf::record(0x80, omf::name("m")), omf::record(0x8C, omf::name("_bar")), omf::record(0x8A, Bytes{0})}));
    // every public name through a function
    const Bytes txt = testdata("txtonly.obj");
    const Bytes upper = omf::rename_symbols(txt, [](std::string_view s) { return text::upper(s); });
    const auto names = omf::publics(upper);
    CB_CHECK_EQ(names.size(), std::size_t{28});
    CB_CHECK_EQ(names[0], "B$SCREEN10");
    CB_CHECK_THROWS_AS(FormatError, omf::rename_symbols(txt, [](std::string_view) { return std::string(300, 'x'); }));
    // an external that grows
    const Bytes grown = omf::rename_symbols(obj, [](std::string_view s) { return std::string(s) + std::string(50, '_'); });
    CB_CHECK_EQ(omf::externs(grown)[0].size(), std::size_t{54});
});

CB_TEST("formats.omf.errors", [] {
    // a.out objects are not OMF
    Bytes aout(32, 0);
    aout[0] = 0x07, aout[1] = 0x01;
    CB_CHECK(omf::is_aout(aout));
    CB_CHECK_THROWS_AS(UnsupportedError, omf::read(aout));
    // an LNAMES record then a SEGDEF naming a name that does not exist
    const Bytes bad = cat({omf::record(0x96, cat({omf::name("")})), omf::record(0x98, Bytes{0x28, 1, 0, 9, 1, 1}), omf::record(0x8A, Bytes{0})});
    CB_CHECK_THROWS_AS(FormatError, omf::read(bad));
    // LEDATA for a segment that was never defined
    CB_CHECK_THROWS_AS(FormatError, omf::read(cat({omf::record(0xA0, Bytes{1, 0, 0, 7}), omf::record(0x8A, Bytes{0})})));
    // a truncated real object: every cut reads, answers, or throws a cb::Error; the walk keeps what it got
    const Bytes d = testdata("txtonly.obj");
    for (std::size_t n = 0; n < d.size(); n += 7) {
        const ByteView prefix = ByteView(d).first(n);
        try {
            (void)omf::read(prefix);
        } catch (const Error&) {
        }
        (void)omf::walk(prefix);
        (void)omf::publics(prefix);
        (void)omf::externs(prefix);
    }
    CB_CHECK(omf::walk(ByteView(d).first(d.size() - 5)).records.size() > 3);
    CB_CHECK_THROWS_AS(FormatError, omf::library(Bytes{}));  // not even a first byte
});
