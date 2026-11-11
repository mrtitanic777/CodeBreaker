// Tests of cb/formats/ne.hpp on real Windows 95 16-bit modules (testdata/): LZEXPAND.DLL (a library with 12 segments,
// relocations of every use, imported and exported names) and CHARMAP.EXE (resources).
//
// DIVERGES from the Python reader: an alignment shift whose result does not fit 63 bits is a FormatError (the Python raised
// OverflowError at the same point); `standalone` lays out with 16-bit sector numbers and says so (FormatError) when the image
// does not fit, where the Python raised struct.error.
#include "cb/formats/ne.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
namespace ne = cb::formats::ne;

namespace {

Bytes testdata(const char* name) { return fs::read_file(testing::data_path(std::string("formats/testdata/") + name)); }

std::vector<std::string> names_of(const std::vector<ne::Name>& v) {
    std::vector<std::string> out;
    for (const auto& n : v) out.push_back(n.name + ":" + std::to_string(n.ordinal));
    return out;
}

}  // namespace

CB_TEST("formats.ne.header_and_names", [] {
    auto m = ne::Module::open(testdata("lzexpand.dll"));
    const ne::Header& h = m.header();
    CB_CHECK_EQ(m.ne_offset(), std::uint64_t{128});
    CB_CHECK_EQ(h.segment_count, 12);
    CB_CHECK_EQ(h.module_ref_count, 2);
    CB_CHECK_EQ(h.alignment_shift, 4);
    CB_CHECK_EQ(h.flags, 0x8301);
    CB_CHECK_EQ(h.entry_table_offset, 0xED);
    CB_CHECK_EQ(h.entry_table_length, 0xB7);
    CB_CHECK_EQ(m.module_name(), "LZEXPAND");
    const std::vector<std::string> resident = {"LZEXPAND:0", "WEP:11", "___EXPORTEDSTUB:12"};
    CB_CHECK(names_of(m.resident_names()) == resident);
    const auto nonresident = m.nonresident_names();
    CB_CHECK_EQ(nonresident.size(), std::size_t{11});
    CB_CHECK_EQ(nonresident[0].name, "Windows file expansion library");  // the description
    CB_CHECK_EQ(nonresident[1].name, "LZREAD");
    CB_CHECK_EQ(nonresident[1].ordinal, 5);
    const std::vector<std::string> refs = {"KERNEL", "USER"};
    CB_CHECK(m.module_refs() == refs);
});

CB_TEST("formats.ne.segments_relocations_entries", [] {
    auto m = ne::Module::open(testdata("lzexpand.dll"));
    const auto segs = m.segments();
    CB_CHECK_EQ(segs.size(), std::size_t{12});
    CB_CHECK_EQ(segs[0].index, 1);
    CB_CHECK_EQ(segs[0].file_offset, std::uint64_t{768});
    CB_CHECK_EQ(segs[0].length, 456u);
    CB_CHECK_EQ(segs[0].flags, 0x1D50);
    CB_CHECK(!segs[0].is_data());
    CB_CHECK_EQ(segs[11].file_offset, std::uint64_t{1312});  // the last segment is stored early (a data segment)
    CB_CHECK(segs[11].is_data());
    const std::size_t expected_relocs[] = {6, 17, 0, 5, 0, 2, 12, 4, 3, 6, 6, 0};
    std::size_t imports_by_ordinal = 0, internals = 0;
    for (std::size_t i = 0; i < segs.size(); ++i) {
        const auto relocs = m.relocations(segs[i]);
        CB_CHECK_EQ(relocs.size(), expected_relocs[i]);
        for (const auto& r : relocs) {
            if (const auto* imp = std::get_if<ne::ImportOrdinalRef>(&r.target)) {
                ++imports_by_ordinal;
                CB_CHECK(imp->module == 1 || imp->module == 2);  // KERNEL or USER
            } else if (std::holds_alternative<ne::InternalRef>(r.target)) {
                ++internals;
            }
        }
    }
    CB_CHECK(imports_by_ordinal > 0);
    CB_CHECK(internals > 0);
    const auto entries = m.entries();
    CB_CHECK_EQ(entries.size(), std::size_t{30});
    CB_CHECK(entries[0].kind == ne::Entry::Kind::Moveable);
    CB_CHECK_EQ(entries[0].ordinal, 1u);
    CB_CHECK_EQ(entries[0].segment, 4);
    CB_CHECK_EQ(entries[0].offset, 0x15C);
    CB_CHECK_EQ(entries[12].ordinal, 13u);
    CB_CHECK_EQ(m.export_ordinals().size(), std::size_t{30});
});

CB_TEST("formats.ne.resources", [] {
    auto m = ne::Module::open(testdata("charmap.exe"));
    const auto res = m.resources();
    CB_CHECK_EQ(res.size(), std::size_t{8});
    CB_CHECK_EQ(res[0].type, "GROUP_ICON");
    CB_CHECK_EQ(res[0].id, "#111");
    CB_CHECK_EQ(res[0].id_number.value(), 111);
    CB_CHECK_EQ(res[0].type_number.value(), 14);
    CB_CHECK_EQ(res[0].file_offset, std::uint64_t{11680});
    CB_CHECK_EQ(res[0].length, std::uint64_t{48});
    CB_CHECK_EQ(res[2].type, "DIALOG");
    CB_CHECK_EQ(res[2].id, "CHARMAP");  // a named resource: the table's own string
    CB_CHECK(!res[2].id_number.has_value());
    CB_CHECK_EQ(res[7].type, "ICON");
    CB_CHECK_EQ(res[7].id, "#2");
    CB_CHECK_EQ(res[5].type, "VERSION");
});

CB_TEST("formats.ne.standalone_image_reads_back_the_same", [] {
    const Bytes original = testdata("charmap.exe");
    auto m = ne::Module::open(original);
    CB_CHECK_EQ(m.tables_end(), std::uint64_t{438});
    auto again = ne::Module::open(m.standalone());
    CB_CHECK_EQ(again.module_name(), m.module_name());
    CB_CHECK(names_of(again.nonresident_names()) == names_of(m.nonresident_names()));
    CB_CHECK_EQ(again.entries().size(), m.entries().size());
    const auto a = m.segments();
    const auto b = again.segments();
    CB_CHECK_EQ(a.size(), b.size());
    for (std::size_t i = 0; i < a.size(); ++i) {
        CB_CHECK_EQ(a[i].length, b[i].length);
        if (!a[i].file_offset) continue;
        const std::size_t n = a[i].length;
        CB_CHECK(std::equal(m.data().begin() + static_cast<std::ptrdiff_t>(a[i].file_offset), m.data().begin() + static_cast<std::ptrdiff_t>(a[i].file_offset + n),
                            again.data().begin() + static_cast<std::ptrdiff_t>(b[i].file_offset)));
        CB_CHECK_EQ(m.relocations(a[i]).size(), again.relocations(b[i]).size());
    }
    const auto ra = m.resources();
    const auto rb = again.resources();
    CB_CHECK_EQ(ra.size(), rb.size());
    for (std::size_t i = 0; i < ra.size(); ++i) {
        CB_CHECK_EQ(ra[i].type + ra[i].id, rb[i].type + rb[i].id);
        CB_CHECK_EQ(ra[i].length, rb[i].length);
    }
});

CB_TEST("formats.ne.embedded_modules_and_boot_images", [] {
    const Bytes data = testdata("lzexpand.dll");
    const auto found = ne::embedded(data);
    CB_CHECK_EQ(found.size(), std::size_t{1});
    CB_CHECK_EQ(found[0].first, std::uint64_t{128});
    CB_CHECK_EQ(found[0].second, "LZEXPAND");
    // a boot image's resident file with an empty overlay: the same module, with its offsets pointing into resident+overlay
    const auto modules = ne::open_boot_image(data, Bytes());
    CB_CHECK_EQ(modules.size(), std::size_t{1});
    CB_CHECK_EQ(modules[0].first, "LZEXPAND");
    CB_CHECK_EQ(modules[0].second.overlay_at().value(), std::uint64_t{data.size()});
    CB_CHECK_EQ(modules[0].second.module_name(), "LZEXPAND");
    // preloaded segments are where they are in the plain module
    const auto plain = ne::Module::open(data).segments();
    const auto boot = modules[0].second.segments();
    for (std::size_t i = 0; i < plain.size(); ++i)
        if (plain[i].flags & ne::kPreload) CB_CHECK_EQ(boot[i].file_offset, plain[i].file_offset);
    CB_CHECK(ne::embedded(testdata("lz32.dll")).empty());  // a PE has no NE module inside
});

CB_TEST("formats.ne.errors", [] {
    CB_CHECK_THROWS_AS(FormatError, ne::Module::open(Bytes{1, 2, 3}));
    CB_CHECK_THROWS_AS(FormatError, ne::Module::open(testdata("lz32.dll")));  // a PE: no NE signature
    CB_CHECK(!ne::Module::try_open(testdata("lz32.dll")).has_value());
    CB_CHECK(ne::Module::try_open(testdata("lzexpand.dll")).has_value());
    const Bytes whole = testdata("lzexpand.dll");
    // the NE header cut short: a truncated read
    const Bytes cut(whole.begin(), whole.begin() + 128 + 0x20);
    CB_CHECK_THROWS_AS(TruncatedError, ne::Module::open(cut));
    // tables cut off: the reader that needs them fails, with a FormatError, rather than reading outside the data
    const Bytes tables_cut(whole.begin(), whole.begin() + 128 + 0x44);
    auto m = ne::Module::open(tables_cut);
    CB_CHECK(m.resident_names().empty());  // a name table that is not in the data lists nothing
    CB_CHECK_THROWS_AS(FormatError, m.segments());
    // a relocation count that is garbage (the segment's table is not where it says): no relocations rather than an error
    Bytes garbage = whole;
    auto good = ne::Module::open(whole);
    const auto seg = good.segments()[0];
    garbage[seg.file_offset + seg.length] = 0xFF, garbage[seg.file_offset + seg.length + 1] = 0xFF;
    CB_CHECK(ne::Module::open(garbage).relocations(seg).empty());
});

// Damaged copies of a real module: every reader either answers or throws a cb::Error (never crashes, never reads outside).
CB_TEST("formats.ne.damage_never_crashes", [] {
    const Bytes whole = testdata("charmap.exe");
    for (std::size_t at = 128; at < 128 + 0x40; ++at) {
        Bytes bad = whole;
        bad[at] = static_cast<std::uint8_t>(bad[at] ^ 0xFF);
        try {
            auto m = ne::Module::open(bad);
            for (const auto& s : m.segments()) (void)m.relocations(s);
            (void)m.entries();
            (void)m.resources();
            (void)m.module_refs();
            (void)m.resident_names();
            (void)m.nonresident_names();
            (void)m.tables_end();
            (void)m.standalone();
        } catch (const Error&) {  // a damaged header is allowed to be an error; only crashes and wild reads are not
        }
    }
});
