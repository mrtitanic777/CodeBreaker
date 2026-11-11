// Tests of cb/formats/le.hpp on real Windows 95 VxDs (testdata/): SPLITTER.VXD (one object, one page), BIOS.VXD (four objects,
// 33 pages, a service table) and, when the Windows 95 corpus is present, VMM32.VXD (a W3 container). The W4 page compression is
// tested on bytes of the real drivers.
//
// DIVERGES from the Python reader (see state/work/typed-port/_bugs/LIST.md): a fix-up value cut off by the end of the file is a
// TruncatedError (the Python took the shorter value); an object that starts at page 0 and has pages is a FormatError (the Python
// indexed the page map from the end); `file_offset` of an offset before the object is nullopt (the Python floored it into an
// earlier page); a W4 copy that reaches before the page is a FormatError (a distance of 0 was the Python's out[0]); the W3
// directory's member count is the 16-bit field (the Python class read 32 bits, the Python function 16).
#include "cb/formats/le.hpp"

#include <algorithm>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
namespace le = cb::formats::le;

namespace {

Bytes testdata(const char* name) { return fs::read_file(testing::data_path(std::string("formats/testdata/") + name)); }

std::optional<Bytes> corpus_file(const char* relative) {
    const std::filesystem::path p = std::filesystem::path("C:/Users/mrtit/Documents/GitHub/Windows95/design/corpus/files") / relative;
    if (!std::filesystem::exists(p)) return std::nullopt;
    return fs::read_file(p);
}

// a deterministic stream of bytes with repeats, so that compression has something to find
Bytes sample(std::size_t n, std::uint32_t seed) {
    Bytes b;
    std::uint32_t x = seed;
    while (b.size() < n) {
        x = x * 1664525u + 1013904223u;
        const std::size_t run = 1 + (x >> 28);
        const std::uint8_t v = static_cast<std::uint8_t>(x >> 16);
        for (std::size_t k = 0; k < run && b.size() < n; ++k) b.push_back(static_cast<std::uint8_t>(v + (k % 3)));
        if (b.size() > 40 && (x & 0x300) == 0) {  // copy something from earlier
            const std::size_t from = (x >> 8) % (b.size() - 20);
            for (std::size_t k = 0; k < 20 && b.size() < n; ++k) b.push_back(b[from + k]);
        }
    }
    return b;
}

}  // namespace

CB_TEST("formats.le.splitter_header_objects_names", [] {
    auto m = le::Module::from_buffer(testdata("splitter.vxd"));
    const le::Header& h = m.header();
    CB_CHECK_EQ(m.lfa(), std::uint64_t{128});
    CB_CHECK_EQ(m.signature(), "LE");
    CB_CHECK_EQ(h.cpu, 2);  // 386
    CB_CHECK_EQ(h.os, 4);   // Windows VxD
    CB_CHECK_EQ(h.pages, 1u);
    CB_CHECK_EQ(h.page_size, 4096u);
    CB_CHECK_EQ(h.last_page_bytes, 1516u);
    CB_CHECK_EQ(h.data_pages, 512u);
    CB_CHECK_EQ(h.device_id, 0x498);
    CB_CHECK_EQ(m.objects().size(), std::size_t{1});
    const le::Object& o = m.objects()[0];
    CB_CHECK_EQ(o.index, 1u);
    CB_CHECK_EQ(o.vsize, 0x604u);
    CB_CHECK_EQ(o.flags, 0x2045u);
    CB_CHECK(o.executable() && o.readable() && !o.writable());
    CB_CHECK_EQ(m.resident_name(), "Splitter");
    CB_CHECK_EQ(m.description(), "Splitter Virtual Device\t(Version 4.0)");
    CB_CHECK(m.import_modules().empty());
    CB_CHECK(!m.is_member());
    const auto exports = m.exports();
    CB_CHECK_EQ(exports.size(), std::size_t{1});
    CB_CHECK_EQ(exports[0].name.value(), "Splitter_DDB");
    CB_CHECK_EQ(exports[0].ordinal, 1u);
    CB_CHECK_EQ(exports[0].object, 1u);
    CB_CHECK_EQ(exports[0].offset, 0x59Cu);
    const auto pm = m.page_map();
    CB_CHECK_EQ(pm.size(), std::size_t{1});
    CB_CHECK_EQ(pm[0].number, 1u);
    CB_CHECK_EQ(pm[0].kind, le::kPagePresent);
});

CB_TEST("formats.le.splitter_pages_and_addresses", [] {
    const Bytes file = testdata("splitter.vxd");
    auto m = le::Module::from_buffer(file);
    const Bytes data = m.object_data(m.objects()[0]);
    CB_CHECK_EQ(data.size(), std::size_t{0x604});
    CB_CHECK(std::equal(data.begin(), data.begin() + 1516, file.begin() + 512));  // the page is at data_pages; its last 1516 bytes are stored
    CB_CHECK(std::all_of(data.begin() + 1516, data.end(), [](std::uint8_t b) { return b == 0; }));  // the rest of the object is zero-filled
    CB_CHECK_EQ(m.file_offset(1, 0).value(), std::uint64_t{512});
    CB_CHECK_EQ(m.file_offset(1, 0x10).value(), std::uint64_t{512 + 0x10});
    CB_CHECK(!m.file_offset(1, 0x604).has_value());  // past the object
    CB_CHECK(!m.file_offset(2, 0).has_value());      // no such object
    CB_CHECK(!m.file_offset(1, -4).has_value());
    const auto owner = m.page_to_object(1);
    CB_CHECK(owner.has_value());
    CB_CHECK_EQ(owner->first->index, 1u);
    CB_CHECK_EQ(owner->second, std::uint64_t{0});
    CB_CHECK(!m.page_to_object(7).has_value());
});

CB_TEST("formats.le.splitter_fixups", [] {
    auto m = le::Module::from_buffer(testdata("splitter.vxd"));
    CB_CHECK_EQ(m.fixups().size(), std::size_t{30});
    CB_CHECK_EQ(m.fixup_records().size(), std::size_t{13});
    CB_CHECK_EQ(m.references().size(), std::size_t{30});
    CB_CHECK_EQ(m.pointers().size(), std::size_t{30});
    for (const auto& r : m.fixup_records()) {
        CB_CHECK_EQ(r.page, 1u);
        CB_CHECK(!r.raw.empty());
        CB_CHECK(!r.sources.empty());
        CB_CHECK(r.kind == le::FixupRecord::Kind::Internal || r.kind == le::FixupRecord::Kind::Import);
    }
    // every internal reference has a place inside the object and a target inside an object
    for (const auto& ref : m.references()) {
        CB_CHECK_EQ(ref.at.object, 1u);
        CB_CHECK(ref.at.offset >= 0 && ref.at.offset < 0x604);
        CB_CHECK_EQ(ref.target_object, 1u);
    }
    // an encoded record reads back as stored
    for (const auto& r : m.fixup_records()) CB_CHECK(le::encode_fixup(r) == r.raw);
});

CB_TEST("formats.le.splitter_descriptor_and_services", [] {
    auto m = le::Module::from_buffer(testdata("splitter.vxd"));
    const auto d = m.descriptor();
    CB_CHECK(d.has_value());
    CB_CHECK_EQ(d->name, "Splitter");
    CB_CHECK_EQ(d->where.object, 1u);
    CB_CHECK_EQ(d->where.offset, std::int64_t{0x59C});
    CB_CHECK_EQ(d->size, std::size_t{0x50});
    CB_CHECK_EQ(d->bytes.size(), std::size_t{0x50});
    CB_CHECK_EQ(d->device_id, 0x498);
    CB_CHECK_EQ(d->major, 3);
    CB_CHECK_EQ(d->minor, 10);
    CB_CHECK_EQ(d->service_count, 5u);
    CB_CHECK(d->at_control.has_value());
    CB_CHECK_EQ(d->at_control->object, 1u);
    CB_CHECK_EQ(d->at_control->offset, std::int64_t{0x99});
    const auto table = m.service_table(*d);
    CB_CHECK_EQ(table.size(), std::size_t{5});
    for (const auto& place : table) CB_CHECK(place.has_value());
    CB_CHECK_EQ(m.service_calls().size(), std::size_t{40});
    const auto ddb = m.ddb();
    CB_CHECK(ddb.has_value());
    CB_CHECK_EQ(ddb->name, "Splitter");
    CB_CHECK_EQ(ddb->rva, std::int64_t{0x59C});
    for (const auto& c : m.service_calls()) {
        CB_CHECK(c.device > 0 && c.device < 0x600);
        CB_CHECK(c.service < 0x400);
    }
});

CB_TEST("formats.le.bios_multiple_objects", [] {
    auto m = le::Module::from_buffer(testdata("bios.vxd"));
    const le::Header& h = m.header();
    CB_CHECK_EQ(h.pages, 33u);
    CB_CHECK_EQ(h.page_size, 512u);
    CB_CHECK_EQ(m.objects().size(), std::size_t{4});
    CB_CHECK_EQ(m.objects()[1].vsize, 0x346Au);
    CB_CHECK_EQ(m.objects()[1].page_count, 27u);
    CB_CHECK_EQ(m.objects()[2].first_page, 29u);
    CB_CHECK(m.objects()[2].writable());
    CB_CHECK_EQ(m.resident_name(), "BIOS");
    CB_CHECK_EQ(m.fixups().size(), std::size_t{269});
    CB_CHECK_EQ(m.fixup_records().size(), std::size_t{153});
    CB_CHECK_EQ(m.service_calls().size(), std::size_t{85});
    const auto d = m.descriptor();
    CB_CHECK(d.has_value());
    CB_CHECK_EQ(d->name, "BIOS");
    CB_CHECK_EQ(d->service_count, 4u);
    const auto table = m.service_table();
    CB_CHECK_EQ(table.size(), std::size_t{4});
    for (const auto& place : table) CB_CHECK_EQ(place->object, 2u);  // the services are routines in the code object
    // a page maps to the object it belongs to
    CB_CHECK_EQ(m.page_to_object(3)->first->index, 2u);
    CB_CHECK_EQ(m.page_to_object(3)->second, std::uint64_t{512});
    CB_CHECK_EQ(m.page_to_object(30)->first->index, 3u);
    // every object loads to its virtual size
    for (const auto& o : m.objects()) CB_CHECK_EQ(m.object_data(o).size(), std::size_t{o.vsize});
});

// Taking an image apart and putting it back gives the same bytes for these drivers (the Python round trip does too).
CB_TEST("formats.le.parts_roundtrip", [] {
    for (const char* name : {"splitter.vxd", "bios.vxd"}) {
        const Bytes file = testdata(name);
        auto m = le::Module::from_buffer(file);
        const le::Parts parts = m.parts();
        CB_CHECK_EQ(parts.objects.size(), m.objects().size());
        CB_CHECK_EQ(parts.pages.size(), std::size_t{m.header().pages});
        CB_CHECK_EQ(parts.stub.size(), std::size_t{128});
        CB_CHECK_EQ(parts.header.size(), le::kHeaderSize);
        CB_CHECK(le::write_parts(parts) == file);
    }
    // an edit to the parts shows in the written image: rename the module
    const Bytes file = testdata("splitter.vxd");
    le::Parts parts = le::Module::from_buffer(file).parts();
    auto names = std::get<std::vector<le::NameOrdinal>>(parts.resident);
    names[0].name = "Splitter";
    parts.resident = names;
    CB_CHECK(le::Module::from_buffer(le::write_parts(parts)).resident_name() == "Splitter");
    // a field that does not fit is an error
    parts.fixups.clear();
    parts.fixups.insert_or_assign(1, std::vector<Bytes>{});
    parts.import_modules = {std::string(300, 'x')};
    CB_CHECK_THROWS_AS(FormatError, le::write_parts(parts));
});

CB_TEST("formats.le.members_and_names_helpers", [] {
    const Bytes bios = testdata("bios.vxd");
    const auto mods = le::modules(bios, "bios.vxd");
    CB_CHECK_EQ(mods.size(), std::size_t{1});
    CB_CHECK_EQ(mods[0].first, "bios.vxd");
    // names_at is tolerant: it stops at the first entry that does not fit
    const Bytes table{0, 3, 'a', 'b', 'c', 7, 0, 9, 'x'};  // [0]: not a position; then "abc" #7; then an entry that does not fit
    const auto names = le::names_at(table, 1, std::nullopt);
    CB_CHECK_EQ(names.size(), std::size_t{1});
    CB_CHECK_EQ(names[0].name, "abc");
    CB_CHECK_EQ(names[0].ordinal, 7);
    CB_CHECK(le::names_at(table, 0, std::nullopt).empty());  // position 0 is never a name
    CB_CHECK_THROWS_AS(FormatError, le::modules(Bytes(0x100, 0), "zeros"));
});

CB_TEST("formats.le.w3_container_of_the_real_vmm32", [] {
    auto vmm = corpus_file("WINDOWS/SYSTEM/vmm32.vxd");
    if (!vmm) return;  // the Windows 95 corpus is not on this machine
    const auto c = le::container(*vmm);
    CB_CHECK(c.has_value());
    CB_CHECK(c->kind == le::Container::Kind::W3);
    CB_CHECK(!c->members.empty());
    const auto mods = le::modules(*vmm, "vmm32.vxd");
    CB_CHECK(!mods.empty() && mods.size() <= c->members.size());
    for (const auto& [name, module] : mods) {
        CB_CHECK(!name.empty());
        CB_CHECK(module.is_member());
        CB_CHECK(!module.objects().empty());
    }
    // the directory written back reads the same
    const Bytes directory = le::pack_directory(*c);
    CB_CHECK_EQ(directory.size(), std::size_t{0x10 + 16 * c->members.size()});
    CB_CHECK(std::equal(directory.begin(), directory.end(), vmm->begin() + static_cast<std::ptrdiff_t>(c->at)));
    // a W3 file is its own expansion
    CB_CHECK(le::expand(*vmm) == *vmm);
});

CB_TEST("formats.le.w4_pages_compress_and_expand", [] {
    for (std::size_t n : {std::size_t{512}, std::size_t{1000}, std::size_t{4096}, std::size_t{0x2000}}) {
        const Bytes page = sample(n, static_cast<std::uint32_t>(n));
        const Bytes packed = le::compress_page(page);
        CB_CHECK(le::expand_page(packed, n) == page);
    }
    const Bytes zeros(0x2000, 0);
    CB_CHECK(le::compress_page(zeros).size() < 200);  // long runs compress hard
    // a whole container: stub, then a W4 header in the model's layout, then pages
    Bytes like(0x40 + 16, 0);
    like[0] = 'M', like[1] = 'Z', like[0x3C] = 0x40;
    like[0x40] = 'W', like[0x41] = '4', like[0x42] = 1;
    like[0x44] = 0x00, like[0x45] = 0x20;  // 0x2000-byte pages
    Bytes data(0x40, 0);
    data[0] = 'M', data[1] = 'Z', data[0x3C] = 0x40;
    const Bytes body = sample(30000, 77);
    data.insert(data.end(), body.begin(), body.end());
    const Bytes packed = le::compress(data, like);
    const auto c = le::container(packed);
    CB_CHECK(c.has_value());
    CB_CHECK(c->kind == le::Container::Kind::W4);
    const Bytes expanded = le::expand(packed);
    Bytes expected = data;
    expected.resize(0x40 + ((body.size() + 0x1FFF) / 0x2000) * 0x2000, 0);  // the last page is padded
    CB_CHECK(expanded == expected);
    // the compressed container is smaller than the data for compressible pages
    CB_CHECK(packed.size() < data.size());
});

CB_TEST("formats.le.errors_and_damage", [] {
    CB_CHECK_THROWS_AS(FormatError, le::Module::from_buffer(testdata("lz32.dll")));  // a PE
    CB_CHECK_THROWS_AS(TruncatedError, le::Module::from_buffer(Bytes{'M', 'Z'}));
    const Bytes whole = testdata("splitter.vxd");
    const Bytes cut(whole.begin(), whole.begin() + 128 + 0x30);
    CB_CHECK_THROWS_AS(TruncatedError, le::Module::parse(cut, 128));
    CB_CHECK_THROWS_AS(FormatError, le::expand_page(Bytes{0xFF, 0xFF}, 100));  // runs out of bits
    CB_CHECK_THROWS_AS(FormatError, le::compress(Bytes(100, 0), Bytes(100, 0)));  // `like` is not a container
    // an object that claims pages starting at page 0
    Bytes bad = whole;
    const std::uint64_t first_page_field = 128 + std::uint64_t{le::Module::from_buffer(whole).header().object_table} + 12;
    bad[first_page_field] = 0;
    CB_CHECK_THROWS_AS(FormatError, le::Module::from_buffer(bad).object_data(le::Module::from_buffer(bad).objects()[0]));
    // damage anywhere in the headers: every reader answers or throws a cb::Error
    for (std::size_t at = 128; at < 128 + le::kHeaderSize; ++at) {
        Bytes damaged = whole;
        damaged[at] = static_cast<std::uint8_t>(damaged[at] ^ 0x5A);
        try {
            auto m = le::Module::from_buffer(damaged);
            (void)m.exports();
            (void)m.fixups();
            (void)m.fixup_records();
            (void)m.references();
            (void)m.descriptor();
            (void)m.ddb();
            (void)m.names();
            (void)m.service_calls();
            (void)m.parts();
        } catch (const Error&) {
        }
    }
});
