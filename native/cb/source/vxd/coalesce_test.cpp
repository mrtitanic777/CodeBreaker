// cb-unity: alone (test helpers named for this module)
// Tests of cb/source/vxd/coalesce on the real Windows 95 drivers of formats/testdata (splitter.vxd, srammtd.vxd, bios.vxd). The
// drivers are in the form retail's LINK writes (source lists). The form wlink writes -- one fix-up record per source, and loader
// tables laid out its own way -- is made from each by taking the image apart (Module::parts) and writing it back (write_parts), as
// the Python module's harness did; what coalescing and restoring must give back is checked by what the loader would see: the
// same (object, offset) -> target of every fix-up, the same names and entries, the same object bytes, the headers at the same places.
//
// The Python module's results on the same drivers were compared one by one with CB_VXD_COALESCE_HARNESS (the folder
// state/work/typed-port/source.vxd.coalesce of the checkout: its expected.txt and inputs/, over 54 sampled drivers and their
// variants); that test is skipped when the variable is not set. The cases that differ are listed in DIVERGES below.
//
// DIVERGES from the Python module, by design:
//   * a record is read within its page (the Python read on past the page's end into the next page, or out of the file), so a damaged
//     page fails to parse and the coalesce step is skipped where the Python made a table out of whatever it read;
//   * more than 255 sources of one target are written as several source-list records (the Python raised on `bytes([...])`, and
//     finalize skipped the step);
//   * a table that would slide before the header, a header field that does not fit 32 bits, and a data-pages offset past the end
//     of the file are errors (struct.error in the Python, or a wrap, or an IndexError);
//   * the tables are checked to lie before the data pages before anything is written (the Python wrote first);
//   * finalize says which steps it skipped, and why (the Python swallowed every Exception silently).
#include "cb/source/vxd/coalesce.hpp"

#include <algorithm>
#include <set>
#include <sstream>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/log.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace co = cb::source::vxd::coalesce;
namespace le = cb::formats::le;
using cb::Bytes;
using cb::ByteView;

namespace {

Bytes driver_bytes(const char* name) { return cb::fs::read_file(cb::testing::data_path(std::string("formats/testdata/") + name)); }
le::Module driver(const char* name) { return le::Module::from_buffer(driver_bytes(name)); }
const char* const kDrivers[] = {"splitter.vxd", "srammtd.vxd", "bios.vxd"};

std::uint32_t u32(const Bytes& b, std::size_t at) { return b.at(at) | (b.at(at + 1) << 8) | (b.at(at + 2) << 16) | (static_cast<std::uint32_t>(b.at(at + 3)) << 24); }
void set32(Bytes& b, std::size_t at, std::uint32_t v) {
    for (unsigned i = 0; i < 4; ++i) b.at(at + i) = static_cast<std::uint8_t>(v >> (8 * i));
}

// The image with every fix-up record split in one per source (the form wlink writes), and optionally other loader tables.
Bytes split_form(const le::Module& m, const std::optional<std::vector<le::NameOrdinal>>& resident = std::nullopt) {
    le::Parts parts = m.parts();
    cb::OrderedMap<std::uint32_t, std::vector<Bytes>> records;
    for (const le::FixupRecord& r : m.fixup_records()) {
        for (const std::int16_t source : r.sources) {
            le::FixupRecord one = r;
            one.source = r.source & ~0x20;
            one.source_flags = 0;
            one.sources = {source};
            records[r.page].push_back(le::encode_fixup(one));
        }
    }
    parts.fixups = records;
    if (resident) parts.resident = *resident;
    return le::write_parts(parts);
}

// LINK 2.60's loader tables: the module name padded to eight and the DDB published as a named export.
std::vector<le::NameOrdinal> padded_names(const le::Module& m) {
    std::vector<le::NameOrdinal> names = m.names().first;
    names.front().name.resize(8, ' ');
    names.push_back({m.resident_name() + "_DDB", 1});
    return names;
}

using FixupList = std::vector<std::pair<le::Place, le::Fixup>>;
FixupList sorted_fixups(const le::Module& m) {
    FixupList out = m.fixups();
    std::sort(out.begin(), out.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
    return out;
}
bool same_fixups(const le::Module& a, const le::Module& b) {
    const FixupList x = sorted_fixups(a), y = sorted_fixups(b);
    if (x.size() != y.size()) return false;
    for (std::size_t i = 0; i < x.size(); ++i) {
        if (x[i].first != y[i].first || x[i].second.index() != y[i].second.index()) return false;
        if (const auto* p = std::get_if<le::InternalFixup>(&x[i].second)) {
            const auto& q = std::get<le::InternalFixup>(y[i].second);
            if (p->object != q.object || p->target != q.target) return false;
        } else {
            const auto& p2 = std::get<le::ImportFixup>(x[i].second);
            const auto& q2 = std::get<le::ImportFixup>(y[i].second);
            if (p2.module != q2.module || p2.ordinal != q2.ordinal) return false;
        }
    }
    return true;
}
bool same_objects(const le::Module& a, const le::Module& b) {
    if (a.objects().size() != b.objects().size()) return false;
    for (std::size_t i = 0; i < a.objects().size(); ++i) {
        if (a.object_data(a.objects()[i]) != b.object_data(b.objects()[i])) return false;
    }
    return true;
}
bool same_names(const le::Module& a, const le::Module& b) {
    const auto x = a.names(), y = b.names();
    const auto eq = [](const std::vector<le::NameOrdinal>& p, const std::vector<le::NameOrdinal>& q) {
        if (p.size() != q.size()) return false;
        for (std::size_t i = 0; i < p.size(); ++i) {
            if (p[i].name != q[i].name || p[i].ordinal != q[i].ordinal) return false;
        }
        return true;
    };
    return eq(x.first, y.first) && eq(x.second, y.second) && a.entries().size() == b.entries().size();
}

// How many records the fix-up tables of a module hold.
std::size_t record_count(const le::Module& m) { return m.fixup_records().size(); }

// One page: a source-list record and a plain one.
//   plain   07 10 | soff:i16 | obj:u8 | offset:u32       (a 32-bit offset to object 1)
//   list    27 10 | count:u8 | obj:u8 | offset:u32 | soff:i16 * count
Bytes plain_record(std::uint8_t obj, std::uint32_t offset, std::int16_t soff, std::uint8_t src = 0x07, std::uint8_t flags = 0x10) {
    cb::io::ByteWriter w;
    w.u8(src), w.u8(flags), w.i16(soff), w.u8(obj), w.u32(offset);
    return w.take();
}

}  // namespace

CB_TEST("source.vxd.coalesce.parse_page_and_emit_record", [] {
    // a plain record, a source list, a selector, an import by ordinal (wide module, 8-bit ordinal), an additive record
    cb::io::ByteWriter w;
    w.u8(0x07), w.u8(0x10), w.i16(0x0123), w.u8(1), w.u32(0x400);                                   // plain
    w.u8(0x27), w.u8(0x10), w.u8(3), w.u8(2), w.u32(0x9000), w.i16(0x10), w.i16(0x20), w.i16(-4);   // list of three
    w.u8(0x02), w.u8(0x00), w.i16(0x44), w.u8(5);                                                    // a selector: the target is the object
    w.u8(0x07), w.u8(0x01 | 0x40 | 0x80), w.i16(0x50), w.u16(0x0102), w.u8(0x33);                    // import: module 0x0102, ordinal 0x33
    w.u8(0x07), w.u8(0x01), w.i16(0x60), w.u8(7), w.u16(0x0155);                                     // import with a 16-bit ordinal
    w.u8(0x07), w.u8(0x04 | 0x20), w.i16(0x70), w.u8(1), w.u16(0x1234), w.u32(0xAABBCCDD);          // additive, 32-bit addend, 16-bit offset
    const Bytes page = w.take();
    const co::Page parsed = co::parse_page(page);
    CB_CHECK(parsed.rest.empty());
    CB_CHECK_EQ(parsed.records.size(), 6u);
    const co::PageRecord& r0 = parsed.records[0];
    CB_CHECK(r0.source == 0x07 && r0.flags == 0x10 && r0.sources == std::vector<std::int16_t>{0x0123});
    CB_CHECK((r0.target == Bytes{1, 0x00, 0x04, 0x00, 0x00}));
    const co::PageRecord& r1 = parsed.records[1];
    CB_CHECK(r1.source == 0x07);   // the source-list bit is cleared
    CB_CHECK((r1.sources == std::vector<std::int16_t>{0x10, 0x20, -4}));
    CB_CHECK((r1.target == Bytes{2, 0x00, 0x90, 0x00, 0x00}));
    CB_CHECK((parsed.records[2].target == Bytes{5}) && parsed.records[2].sources.size() == 1);
    CB_CHECK((parsed.records[3].target == Bytes{0x02, 0x01, 0x33}));
    CB_CHECK((parsed.records[4].target == Bytes{7, 0x55, 0x01}));
    CB_CHECK((parsed.records[5].addend == Bytes{0xDD, 0xCC, 0xBB, 0xAA}));
    CB_CHECK((parsed.records[5].target == Bytes{1, 0x34, 0x12}));
    // emitting what was parsed gives the same bytes: plain records for one source, source lists for several
    Bytes again;
    for (const co::PageRecord& r : parsed.records) cb::append(again, co::emit_record(r));
    CB_CHECK(again == page);

    // a record of a kind not taken apart (an import by name, 0x02) and everything after it are left as they are
    cb::io::ByteWriter v;
    v.u8(0x07), v.u8(0x10), v.i16(1), v.u8(1), v.u32(8);
    const std::size_t at = v.size();
    v.u8(0x07), v.u8(0x02), v.i16(2), v.u16(1), v.u16(0x30);   // import by name
    v.u8(0x07), v.u8(0x10), v.i16(3), v.u8(1), v.u32(8);
    const Bytes with_name = v.take();
    const co::Page split = co::parse_page(with_name);
    CB_CHECK_EQ(split.records.size(), 1u);
    CB_CHECK((split.rest == Bytes(with_name.begin() + static_cast<std::ptrdiff_t>(at), with_name.end())));

    // the page is read as far as the page goes, not further
    CB_CHECK_THROWS_AS(cb::TruncatedError, (void)co::parse_page(Bytes(page.begin(), page.begin() + 6)));   // the target is cut
    CB_CHECK_THROWS_AS(cb::TruncatedError, (void)co::parse_page(Bytes{0x07}));
    CB_CHECK_THROWS_AS(cb::TruncatedError, (void)co::parse_page(Bytes{0x27, 0x10, 5, 1, 0, 0, 0, 0, 1, 0}));   // five sources promised, one there
    CB_CHECK(co::parse_page(Bytes{}).records.empty());
});

CB_TEST("source.vxd.coalesce.emit_record_shapes", [] {
    co::PageRecord r;
    r.source = 0x07, r.flags = 0x10, r.target = {1, 0, 0, 0, 0};
    // one source: a plain record
    r.sources = {0x0123};
    CB_CHECK((co::emit_record(r) == Bytes{0x07, 0x10, 0x23, 0x01, 1, 0, 0, 0, 0}));
    // several: a source list
    r.sources = {1, 2};
    CB_CHECK((co::emit_record(r) == Bytes{0x27, 0x10, 2, 1, 0, 0, 0, 0, 1, 0, 2, 0}));
    // an addend follows the target in both forms
    r.flags = 0x14, r.addend = {9, 8};
    r.sources = {5};
    CB_CHECK((co::emit_record(r) == Bytes{0x07, 0x14, 5, 0, 1, 0, 0, 0, 0, 9, 8}));
    r.sources = {5, 6};
    CB_CHECK((co::emit_record(r) == Bytes{0x27, 0x14, 2, 1, 0, 0, 0, 0, 9, 8, 5, 0, 6, 0}));
    // none: a source list of no sources (what a count of 0 parses to)
    r.sources = {};
    CB_CHECK((co::emit_record(r) == Bytes{0x27, 0x14, 0, 1, 0, 0, 0, 0, 9, 8}));
    // a list holds 255 at most: 300 sources are two records, and reading them back finds all 300 in order
    r.flags = 0x10, r.addend = {};
    r.sources.clear();
    for (int i = 0; i < 300; ++i) r.sources.push_back(static_cast<std::int16_t>(i * 3 - 100));
    const Bytes two = co::emit_record(r);
    CB_CHECK_EQ(two.size(), (3u + 5 + 255 * 2) + (3u + 5 + 45 * 2));
    const co::Page back = co::parse_page(two);
    CB_CHECK_EQ(back.records.size(), 2u);
    std::vector<std::int16_t> all = back.records[0].sources;
    all.insert(all.end(), back.records[1].sources.begin(), back.records[1].sources.end());
    CB_CHECK(all == r.sources);
    // exactly 255 is one record; 256 is two
    r.sources.assign(255, 7);
    CB_CHECK_EQ(co::parse_page(co::emit_record(r)).records.size(), 1u);
    r.sources.assign(256, 7);
    CB_CHECK_EQ(co::parse_page(co::emit_record(r)).records.size(), 2u);
});

CB_TEST("source.vxd.coalesce.a_page_of_single_records_becomes_source_lists", [] {
    // Build a driver whose first fix-up page holds six single records: three to one target (one list), two to another, one alone;
    // here: the splitter's own tables, split. Whatever retail wrote as a list, wlink's form splits and coalesce folds back.
    const le::Module original = driver("splitter.vxd");
    const Bytes split = split_form(original);
    const le::Module before = le::Module::from_buffer(Bytes(split));
    CB_CHECK(record_count(before) > record_count(original));   // splitting made more records
    const Bytes folded = co::coalesce(split);
    const le::Module after = le::Module::from_buffer(Bytes(folded));
    CB_CHECK_EQ(folded.size(), split.size());                  // the span of the tables is kept, the slack zeroed
    CB_CHECK(same_fixups(after, original));                    // every (object, offset) points where it did
    CB_CHECK(same_objects(after, original));
    CB_CHECK_EQ(record_count(after), record_count(original)); // as retail wrote it: one record per target
    // folding again changes nothing
    CB_CHECK(co::coalesce(folded) == folded);
    // the freed bytes before the data pages are zero
    const std::uint32_t data_pages = after.header().data_pages;
    const std::size_t records_end = after.lfa() + after.header().fixup_records;
    std::size_t used = 0;
    for (const le::FixupRecord& r : after.fixup_records()) used += r.raw.size();
    for (std::size_t k = records_end + used; k < data_pages; ++k) CB_CHECK_EQ(folded[k], 0);
});

CB_TEST("source.vxd.coalesce.coalesce_of_the_real_drivers", [] {
    for (const char* name : kDrivers) {
        const Bytes file = driver_bytes(name);
        const le::Module original = le::Module::from_buffer(Bytes(file));
        // a driver retail linked is already folded: the same bytes come back
        // (bios.vxd is a driver whose records retail's LINK did not write in the order coalescing groups them -- by target, first seen
        // first -- so its bytes move though what they say does not; the Python module gives the same bytes, see the harness test below)
        const Bytes same_again = co::coalesce(file);
        if (std::string_view(name) != "bios.vxd") CB_CHECK(same_again == file);
        CB_CHECK(same_fixups(le::Module::from_buffer(Bytes(same_again)), original));
        CB_CHECK(co::coalesce(same_again) == same_again);
        // wlink's form folds back to the same fix-ups and objects
        const Bytes split = split_form(original);
        const Bytes folded = co::coalesce(split);
        const le::Module after = le::Module::from_buffer(Bytes(folded));
        CB_CHECK(same_fixups(after, original));
        CB_CHECK(same_objects(after, original));
        CB_CHECK_EQ(record_count(after), record_count(original));
        CB_CHECK(folded.size() == split.size());
    }
});

CB_TEST("source.vxd.coalesce.coalesce_errors", [] {
    const Bytes file = driver_bytes("splitter.vxd");
    CB_CHECK_THROWS_AS(cb::FormatError, (void)co::coalesce(Bytes{}));
    CB_CHECK_THROWS_AS(cb::FormatError, (void)co::coalesce(Bytes(file.begin(), file.begin() + 0x60)));
    // the page table lies beyond the file
    const le::Module m = le::Module::from_buffer(Bytes(file));
    Bytes cut(file.begin(), file.begin() + static_cast<std::ptrdiff_t>(m.lfa() + m.header().fixup_pages + 2));
    CB_CHECK_THROWS_AS(cb::FormatError, (void)co::coalesce(cut));
    // a page that ends before it starts
    Bytes backwards = file;
    set32(backwards, m.lfa() + m.header().fixup_pages, 0x40);   // page 1 would start at 0x40 ...
    set32(backwards, m.lfa() + m.header().fixup_pages + 4, 0x10);   // ... and end at 0x10
    CB_CHECK_THROWS_AS(cb::FormatError, (void)co::coalesce(backwards));
    // a record that does not fit in its page
    Bytes torn = file;
    set32(torn, m.lfa() + m.header().fixup_pages + 4, 3);   // page 1 holds three bytes
    CB_CHECK_THROWS_AS(cb::TruncatedError, (void)co::coalesce(torn));
    // the data pages begin before the fix-up tables end: nothing fits
    Bytes crowded = file;
    set32(crowded, m.lfa() + 0x80, static_cast<std::uint32_t>(m.lfa() + m.header().fixup_pages + 8));
    CB_CHECK_THROWS_AS(cb::FormatError, (void)co::coalesce(crowded));
});

CB_TEST("source.vxd.coalesce.restore_entry_point", [] {
    for (const char* name : kDrivers) {
        const Bytes file = driver_bytes(name);
        const le::Module original = le::Module::from_buffer(Bytes(file));
        Bytes zeroed = file;
        set32(zeroed, original.lfa() + 0x18, 0);   // wlink leaves 0:0
        set32(zeroed, original.lfa() + 0x1c, 0);
        const Bytes restored = co::restore_entry_point(zeroed, original);
        CB_CHECK(restored == file);
        CB_CHECK_EQ(le::Module::from_buffer(Bytes(restored)).header().eip, original.header().eip);
    }
    // an entry point that was somewhere else is replaced too, and nothing else changes
    const le::Module splitter = driver("splitter.vxd");
    Bytes other = driver_bytes("splitter.vxd");
    set32(other, splitter.lfa() + 0x18, 7);
    set32(other, splitter.lfa() + 0x1c, 0x1234);
    const Bytes back = co::restore_entry_point(other, splitter);
    CB_CHECK(back == driver_bytes("splitter.vxd"));
    // what is not a driver is an error
    CB_CHECK_THROWS_AS(cb::FormatError, (void)co::restore_entry_point(Bytes(300, 7), splitter));
});

CB_TEST("source.vxd.coalesce.restore_metadata_after_the_other_linker", [] {
    for (const char* name : kDrivers) {
        const Bytes file = driver_bytes(name);
        const le::Module original = le::Module::from_buffer(Bytes(file));
        // LINK 2.60's tables: padded module name, an extra named export
        const Bytes theirs = split_form(original, padded_names(original));
        const le::Module before = le::Module::from_buffer(Bytes(theirs));
        CB_CHECK(!same_names(before, original));
        const Bytes folded = co::coalesce(theirs);
        const Bytes fixed = co::restore_metadata(folded, original);
        const le::Module after = le::Module::from_buffer(Bytes(fixed));
        CB_CHECK(same_names(after, original));                          // the resident and non-resident names and the entry table
        CB_CHECK(same_fixups(after, original));                         // the fix-up tables slid with them
        CB_CHECK(same_objects(after, original));                        // and the object data did not move
        CB_CHECK_EQ(after.header().resident_names, original.header().resident_names);
        CB_CHECK_EQ(after.header().entry_table, original.header().entry_table);
        CB_CHECK_EQ(after.header().fixup_pages, original.header().fixup_pages);
        CB_CHECK_EQ(after.header().data_pages, before.header().data_pages);   // the data pages stay where they were
        CB_CHECK_EQ(fixed.size(), folded.size());
        // asking again changes nothing
        CB_CHECK(co::restore_metadata(fixed, original) == fixed);
    }
});

CB_TEST("source.vxd.coalesce.restore_metadata_leaves_what_it_cannot_match", [] {
    const Bytes file = driver_bytes("splitter.vxd");
    const le::Module original = le::Module::from_buffer(Bytes(file));
    // an original whose objects table is another size (the resident names are at another offset): unchanged
    const le::Module bios = driver("bios.vxd");
    if (bios.header().resident_names != original.header().resident_names) CB_CHECK(co::restore_metadata(file, bios) == file);
    // no loader tables to recover: a header whose tables are in no order
    Bytes broken = file;
    set32(broken, original.lfa() + 0x68, 0);   // the fix-up page table "at" 0
    CB_CHECK(co::restore_metadata(broken, original) == broken);
    // the data pages begin past the end of the file
    Bytes lying = file;
    set32(lying, original.lfa() + 0x80, static_cast<std::uint32_t>(file.size() + 100));
    CB_CHECK_THROWS_AS(cb::FormatError, (void)co::restore_metadata(lying, original));
});

CB_TEST("source.vxd.coalesce.restore_version_resource", [] {
    const Bytes file = driver_bytes("splitter.vxd");
    const le::Module splitter = le::Module::from_buffer(Bytes(file));
    const std::size_t L = splitter.lfa();
    // an original that has a version resource and a non-resident name table: here two regions of its own bytes
    Bytes with = file;
    const std::uint32_t names_at = 0x20, names_size = 16, res_at = 0x40, res_size = 24;
    set32(with, L + 0x88, names_at), set32(with, L + 0x8c, names_size), set32(with, L + 0xb8, res_at), set32(with, L + 0xbc, res_size);
    const le::Module oracle = le::Module::from_buffer(Bytes(with));
    // a driver with none gets them appended, and the header points at the copies
    Bytes bare = file;
    for (const std::size_t field : {0x88u, 0x8cu, 0xb8u, 0xbcu}) set32(bare, L + field, 0);
    const Bytes done = co::restore_version_resource(bare, oracle);
    CB_CHECK_EQ(done.size(), bare.size() + names_size + res_size);
    CB_CHECK_EQ(u32(done, L + 0x88), bare.size());
    CB_CHECK_EQ(u32(done, L + 0x8c), names_size);
    CB_CHECK_EQ(u32(done, L + 0xb8), bare.size() + names_size);
    CB_CHECK_EQ(u32(done, L + 0xbc), res_size);
    CB_CHECK((Bytes(done.begin() + static_cast<std::ptrdiff_t>(bare.size()), done.begin() + static_cast<std::ptrdiff_t>(bare.size() + names_size)) ==
              Bytes(with.begin() + names_at, with.begin() + names_at + names_size)));
    CB_CHECK((Bytes(done.end() - res_size, done.end()) == Bytes(with.begin() + res_at, with.begin() + res_at + res_size)));
    CB_CHECK(Bytes(done.begin(), done.begin() + static_cast<std::ptrdiff_t>(bare.size())).size() == bare.size());
    // one that has a version resource is left alone
    CB_CHECK(co::restore_version_resource(with, oracle) == with);
    // an original with neither: nothing to give
    CB_CHECK(co::restore_version_resource(bare, le::Module::from_buffer(Bytes(bare))) == bare);
    // a region that lies outside the original is not copied; the one that lies inside is
    Bytes partial = file;
    set32(partial, L + 0x88, 0x20), set32(partial, L + 0x8c, 8), set32(partial, L + 0xb8, static_cast<std::uint32_t>(file.size() - 4)), set32(partial, L + 0xbc, 100);
    const Bytes half = co::restore_version_resource(bare, le::Module::from_buffer(Bytes(partial)));
    CB_CHECK_EQ(half.size(), bare.size() + 8);
    CB_CHECK_EQ(u32(half, L + 0xbc), 0u);
});

CB_TEST("source.vxd.coalesce.finalize_and_what_it_skips", [] {
    for (const char* name : kDrivers) {
        const Bytes file = driver_bytes(name);
        const le::Module original = le::Module::from_buffer(Bytes(file));
        const Bytes theirs = split_form(original, padded_names(original));
        const co::Finalized done = co::finalize(theirs, original);
        CB_CHECK(done.skipped.empty());
        const le::Module after = le::Module::from_buffer(Bytes(done.bytes));
        CB_CHECK(same_fixups(after, original) && same_objects(after, original) && same_names(after, original));
        CB_CHECK_EQ(after.header().eip, original.header().eip);
        CB_CHECK_EQ(after.header().eip_object, original.header().eip_object);
        CB_CHECK_EQ(record_count(after), record_count(original));
    }
    // a module the steps cannot read: all four are skipped, each says why, and the bytes are as they came
    const le::Module original = driver("splitter.vxd");
    const Bytes junk(200, 9);
    const co::Finalized failed = co::finalize(junk, original);
    CB_CHECK_EQ(failed.skipped.size(), 4u);
    CB_CHECK(failed.bytes == junk);
    CB_CHECK(failed.skipped[0].starts_with("coalesce: ") && failed.skipped[3].starts_with("restore_entry_point: "));
    // a data-pages offset that cannot be right defeats the metadata step and the others still run
    const Bytes file = driver_bytes("splitter.vxd");
    Bytes odd = split_form(original);
    set32(odd, original.lfa() + 0x80, static_cast<std::uint32_t>(odd.size() + 50));
    const co::Finalized some = co::finalize(odd, original);
    CB_CHECK(some.skipped.size() >= 1);
});

CB_TEST("source.vxd.coalesce.command", [] {
    cb::fs::ScopedTempDir tmp("cb-finalize");
    const le::Module original = driver("splitter.vxd");
    const Bytes built = split_form(original, padded_names(original));
    cb::fs::write_file(tmp.path() / "built.vxd", built);
    const std::string original_path = cb::testing::data_path("formats/testdata/splitter.vxd").string();
    const std::string out_path = (tmp.path() / "out.vxd").string();
    cb::testing::CapturedStd captured;
    const std::vector<std::string> args = {(tmp.path() / "built.vxd").string(), original_path, out_path};
    CB_CHECK_EQ(co::main(args), cb::kExitOk);
    const Bytes written = cb::fs::read_file(tmp.path() / "out.vxd");
    CB_CHECK(written == co::finalize(built, original).bytes);
    CB_CHECK(captured.out().find("wrote " + out_path + " (" + std::to_string(written.size()) + " bytes)") != std::string::npos);
    const std::vector<std::string> too_few = {out_path};
    CB_CHECK_EQ(co::main(too_few), cb::kExitUsage);   // the parser reports it and returns 2
    const std::vector<std::string> missing = {(tmp.path() / "nope.vxd").string(), original_path, out_path};
    CB_CHECK_THROWS_AS(cb::NotFoundError, (void)co::main(missing));
});

// The Python module's own results, when the checkout's harness folder is named: CB_VXD_COALESCE_HARNESS=<...>/state/work/typed-port/source.vxd.coalesce
CB_TEST("source.vxd.coalesce.agrees_with_the_python_module_on_its_harness", [] {
    const auto folder = cb::fs::get_env("CB_VXD_COALESCE_HARNESS");
    if (!folder) return;
    std::size_t compared = 0, differ = 0, undamaged_differ = 0;
    std::string first_differences;
    const auto has_text = [](const std::string& text, std::string_view part) { return text.find(part) != std::string::npos; };
    std::istringstream in(cb::fs::read_text(std::filesystem::path(*folder) / "expected.txt"));
    const auto digest = [](const Bytes& b) { return std::format("[{},\"{}\"]", b.size(), cb::crypto::to_hex(cb::crypto::sha1(b))); };
    for (std::string line; std::getline(in, line);) {
        const std::vector<std::string_view> fields = cb::text::split(line, '\t');
        if (fields.size() < 4 || fields[0] == "pipeline" || fields[0].starts_with("_")) continue;
        const std::string_view op = fields[0];
        if (op != "coalesce" && op != "restore_entry_point" && op != "restore_metadata" && op != "restore_version_resource" && op != "finalize") continue;
        if (!fields[1].starts_with("F:")) continue;
        std::filesystem::path input_path(std::string(fields[1].substr(2)));   // (the variants are named relative to the harness folder)
        if (input_path.is_relative()) input_path = std::filesystem::path(*folder) / input_path;
        const Bytes input = cb::fs::read_file(input_path);
        std::optional<le::Module> oracle;
        if (fields[2].starts_with("V:")) {
            const std::string_view spec = fields[2].substr(2);
            const std::size_t bar = spec.rfind('|');
            const auto members = le::open_modules(std::filesystem::path(std::string(spec.substr(0, bar))));
            oracle = members.at(static_cast<std::size_t>(*cb::text::parse_int<int>(spec.substr(bar + 1)))).second;
        }
        const std::string_view expected = fields.back();
        std::string got;
        try {
            if (op == "coalesce") got = digest(co::coalesce(input));
            else if (op == "restore_entry_point") got = digest(co::restore_entry_point(input, *oracle));
            else if (op == "restore_metadata") got = digest(co::restore_metadata(input, *oracle));
            else if (op == "restore_version_resource") got = digest(co::restore_version_resource(input, *oracle));
            else got = digest(co::finalize(input, *oracle).bytes);
        } catch (const cb::Error&) {
            got = "[\"ERR\"";
        }
        ++compared;
        const bool both_errors = got == "[\"ERR\"" && expected.starts_with("[\"ERR\"");
        if (!both_errors && got != expected) {
            ++differ;
            if (!has_text(input_path.filename().string(), "dmg_")) ++undamaged_differ;
            if (differ <= 12) first_differences += std::format("\n  {} {}  python {}  here {}", op, fields[1].substr(fields[1].rfind('\\') + 1), expected.substr(0, 60), got.substr(0, 60));
        }
    }
    CB_CHECK(compared > 1000);
    // every difference is a damaged copy, where the Python read past the end of a page (see DIVERGES)
    CB_CHECK_EQ(undamaged_differ, 0u);
    cb::err("coalesce harness: {} of {} cases differ{}\n", differ, compared, first_differences);
});
