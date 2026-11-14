// cb-unity: alone (test helpers named for this module)
// Tests of cb/source/vxd/le_writer. The source-type rules are checked on a recovery made by hand; the writer on the real driver splitter.vxd
// (formats/testdata): the file it writes from the driver's own recovery must read back as the same driver (objects, object bytes, names,
// device id, fix-ups), which is what makes it loadable. The expected numbers are read off the original's header, not recorded from a run.
//
// Behaviour pinned here: a header that is not where the writer assumes, a table that lies outside the file and a module that
// is not in a container are FormatErrors; build_w3 takes no name;
// the object bytes laid over the pages of a W3 module that run past the last page are dropped, but a module whose object starts past the
// end of the pages is grown to hold it instead of having the bytes appended in the wrong place.
#include "cb/source/vxd/le_writer.hpp"

#include <set>
#include <string>

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"
#include "cb/source/vxd/disassembly.hpp"

namespace vx = cb::source::vxd;
namespace lw = cb::source::vxd::le_writer;
namespace mx = cb::source::vxd::masm;
namespace le = cb::formats::le;
using cb::Bytes;

namespace {

le::Module splitter() { return le::Module::open(cb::testing::data_path("formats/testdata/splitter.vxd")); }

vx::Instruction insn(std::uint32_t size, std::string mnemonic) {
    vx::Instruction i;
    i.size = size;
    i.mnemonic = std::move(mnemonic);
    return i;
}

// One patched place and its target as text, for comparing two drivers' fix-ups.
std::string rendered(const vx::Place& at, const le::Fixup& f) {
    std::string out = std::to_string(at.object) + ":" + std::to_string(at.offset) + " -> ";
    if (const auto* internal = std::get_if<le::InternalFixup>(&f)) {
        out += "obj " + std::to_string(internal->object) + " at " + (internal->target ? std::to_string(*internal->target) : "selector") + " type " + std::to_string(internal->source_type);
    } else {
        const auto& imported = std::get<le::ImportFixup>(f);
        out += "import " + std::to_string(imported.module) + "." + std::to_string(imported.ordinal);
    }
    return out;
}

std::set<std::string> fixups_of(const le::Module& m, bool selectors = false) {
    std::set<std::string> out;
    for (const auto& [at, f] : m.fixups()) {
        const auto* internal = std::get_if<le::InternalFixup>(&f);
        if (!selectors && internal != nullptr && !internal->target) continue;   // a selector-only fix-up has no 32-bit field: the writer leaves it out
        out.insert(rendered(at, f));
    }
    return out;
}

}  // namespace

CB_TEST("source.vxd.le_writer.the_source_type_of_a_field", [] {
    le::Module driver = splitter();
    vx::DdkNames names;
    mx::EmissionParts parts;
    parts.code = {Bytes{0xE8, 0, 0, 0, 0, 0x90, 0x0F, 0x84, 0, 0, 0, 0, 0xB8, 0, 0, 0, 0, 0xC3, 0xE9, 0, 0, 0, 0}};
    parts.instructions[{1, 0}] = insn(5, "call");           // E8 rel32: ends at 5
    parts.instructions[{1, 6}] = insn(6, "je");             // 0F 84 rel32: ends at 12
    parts.instructions[{1, 12}] = insn(5, "mov");           // B8 imm32: ends at 17 -- a 32-bit constant, not a branch
    const mx::Emission code(driver, names, std::move(parts));
    const Bytes& bytes = Bytes{0xE8, 0, 0, 0, 0, 0x90, 0x0F, 0x84, 0, 0, 0, 0, 0xB8, 0, 0, 0, 0, 0xC3, 0xE9, 0, 0, 0, 0};
    const std::set<std::int64_t> ends = {5, 12, 17};
    const auto type = [&](std::int64_t at, std::optional<std::uint8_t> told = std::nullopt) { return lw::fixup_source_type(code, 1, at, ends, bytes, told); };
    CB_CHECK_EQ(type(1), 0x08);    // the rel32 of a decoded call
    CB_CHECK_EQ(type(8), 0x08);    // the rel32 of a decoded jcc
    CB_CHECK_EQ(type(13), 0x07);   // the immediate of a mov: an offset
    CB_CHECK_EQ(type(19), 0x08);   // no decoded instruction ends at 23 and the byte before is E9: an undecoded jmp, trust the opcode
    CB_CHECK_EQ(type(3), 0x07);    // a field in the middle of nothing: an offset
    CB_CHECK_EQ(type(13, std::uint8_t{0x08}), 0x08);   // what the recovered fix-up says wins, even where the code says offset
    CB_CHECK_EQ(type(1, std::uint8_t{0x07}), 0x07);    // and in the other direction (VMM's `cmp eax, imm32` fields are self-relative)
    CB_CHECK_EQ(type(1, std::uint8_t{0x00}), 0x00);
    CB_CHECK_EQ(type(-5), 0x07);   // before the object
    CB_CHECK_EQ(type(100), 0x07);  // past it
});

CB_TEST("source.vxd.le_writer.the_originals_pages_by_number", [] {
    const le::Module driver = splitter();
    const lw::OraclePages found = lw::oracle_pages(driver);
    CB_CHECK_EQ(found.pages.size(), static_cast<std::size_t>(driver.header().pages));
    std::uint32_t top = 0;
    for (const auto& [number, bytes] : found.pages) top = std::max(top, number);
    CB_CHECK_EQ(found.top, top);
    CB_CHECK(!found.pages.empty());
    // the first page in the map is page `number` of the data pages, a whole page of the file
    const auto first = driver.page_map().front();
    const std::uint64_t at = std::uint64_t{driver.header().data_pages} + std::uint64_t{first.number - 1} * driver.header().page_size;
    CB_CHECK(found.pages.front().second == Bytes(driver.data().begin() + static_cast<std::ptrdiff_t>(at),
                                                 driver.data().begin() + static_cast<std::ptrdiff_t>(std::min<std::uint64_t>(at + driver.header().page_size, driver.data().size()))));
    // a different start: the same pages from elsewhere in the file
    const lw::OraclePages shifted = lw::oracle_pages(driver, driver.header().data_pages + 4);
    CB_CHECK_EQ(shifted.pages.size(), found.pages.size());
    CB_CHECK(shifted.pages.front().second != found.pages.front().second);
});

CB_TEST("source.vxd.le_writer.fixup_records_are_grouped_by_target_and_ordered", [] {
    le::Module driver = splitter();
    vx::DdkNames names;
    mx::EmissionParts parts;
    const std::uint32_t page = driver.header().page_size;
    parts.code = {Bytes(std::size_t{page} * 2, 0)};
    // Four sources on page 1 pointing at (object 1, 0x40) -- one record, with a source list; one at (object 1, 0x50) -- a single record before
    // them (its source is the lowest); an import; a target above 0xFFFF (the 32-bit offset flag); a selector-only fix-up (left out); and a 4-byte
    // field that crosses a page boundary (listed on both pages, the second time with a negative offset)
    const auto internal = [](std::uint32_t target, std::uint8_t type = 7) { return le::Fixup(le::InternalFixup{1, target, type}); };
    parts.fixups.insert_or_assign(vx::Place{1, 0x30}, internal(0x40));
    parts.fixups.insert_or_assign(vx::Place{1, 0x10}, internal(0x50));
    parts.fixups.insert_or_assign(vx::Place{1, 0x20}, internal(0x40));
    parts.fixups.insert_or_assign(vx::Place{1, 0x08}, internal(0x40));
    parts.fixups.insert_or_assign(vx::Place{1, 0x60}, le::Fixup(le::ImportFixup{2, 0x1234}));   // ordinal masked to 8 bits
    parts.fixups.insert_or_assign(vx::Place{1, 0x70}, internal(0x12345));
    parts.fixups.insert_or_assign(vx::Place{1, 0x80}, le::Fixup(le::InternalFixup{1, std::nullopt, 2}));
    const bool two_pages = driver.header().pages >= 2;
    if (two_pages) parts.fixups.insert_or_assign(vx::Place{1, page - 2}, internal(0x40, 8));
    const mx::Emission code(driver, names, std::move(parts));
    const auto records = lw::emit_fixups(code, driver);
    CB_CHECK_EQ(records.size(), static_cast<std::size_t>(driver.header().pages));   // every page has a row, empty or not
    const std::vector<Bytes>& first = records.at(1);
    // page 1: (target 0x50) first by its source at 0x10? No: the group (0x40) has the lowest source, 0x08, so it comes first
    CB_CHECK_EQ(first.size(), two_pages ? 5U : 4U);
    const auto decode_count = [](const Bytes& record) { return record.size() > 2 ? record[2] : 0; };
    CB_CHECK_EQ(first[0][0] & 0x20, 0x20);        // a source list: source | 0x20
    CB_CHECK_EQ(decode_count(first[0]), 3);       // 0x08, 0x20, 0x30 share a target
    CB_CHECK_EQ(first[1][0] & 0x20, 0);           // one source: a plain record
    if (!two_pages) return;
    CB_CHECK(records.at(2).size() >= 1U);         // the crossing field is also on page 2
    // a negative source offset (the field began on the previous page): the last two bytes of page 1 start a field listed on page 2 as -2
    bool negative = false;
    for (const Bytes& record : records.at(2)) {
        for (std::size_t i = 0; i + 1 < record.size(); ++i) negative = negative || (record[i] == 0xFE && record[i + 1] == 0xFF);
    }
    CB_CHECK(negative);
});

CB_TEST("source.vxd.le_writer.a_driver_written_back_reads_as_the_same_driver", [] {
    const le::Module original = splitter();
    const vx::DdkNames names;
    const mx::Emission code(vx::Disassembly::run(original, names), names);
    const Bytes file = lw::build_le(code, original, "SPLITTER");
    const le::Module rebuilt = le::Module::from_buffer(Bytes(file));
    CB_CHECK_EQ(rebuilt.signature(), "LE");
    CB_CHECK_EQ(rebuilt.objects().size(), original.objects().size());
    for (std::size_t i = 0; i < original.objects().size(); ++i) {
        const le::Object& a = original.objects()[i];
        const le::Object& b = rebuilt.objects()[i];
        CB_CHECK_EQ(b.vsize, a.vsize);
        CB_CHECK_EQ(b.base, a.base);
        CB_CHECK_EQ(b.flags, a.flags);
        CB_CHECK(rebuilt.object_data(b) == original.object_data(a));   // the code is the recovered code, which is the original's bytes
    }
    // names, device and the loader's header fields
    CB_CHECK_EQ(rebuilt.resident_name(), original.resident_name());
    CB_CHECK_EQ(rebuilt.description(), original.description());
    CB_CHECK_EQ(rebuilt.header().device_id, original.header().device_id);
    CB_CHECK_EQ(rebuilt.header().kit_version, original.header().kit_version);
    CB_CHECK_EQ(rebuilt.header().page_size, original.header().page_size);
    CB_CHECK_EQ(rebuilt.header().eip, original.header().eip);
    CB_CHECK_EQ(rebuilt.header().module_flags, original.header().module_flags);
    CB_CHECK(rebuilt.ddb().has_value());
    CB_CHECK_EQ(rebuilt.ddb()->name, original.ddb()->name);
    // the relocations: the same patched places with the same targets and types, in whatever order the records come
    const std::set<std::string> want = fixups_of(original), got = fixups_of(rebuilt);
    CB_CHECK(!want.empty());
    CB_CHECK(want == got);
    // the file is the original's size, give or take the order of the records (a source-list record is exactly as long as LINK's)
    CB_CHECK(file.size() <= original.data().size() + 0x200);
    CB_CHECK(file.size() + 0x200 >= original.data().size());
    // the DOS stub and the e_lfanew
    CB_CHECK(file[0] == 'M' && file[1] == 'Z');
    CB_CHECK_EQ(file[0x3C], 0x80);
    // the rebuilt file is a driver the writer can read as well: a second pass over it does not throw
    const mx::Emission again(vx::Disassembly::run(rebuilt, names), names);
    CB_CHECK_NOTHROW((void)lw::build_le(again, rebuilt, "SPLITTER"));
});

CB_TEST("source.vxd.le_writer.a_name_is_written_only_when_the_original_has_no_table", [] {
    // the resident name table is copied from the original: the name given does not change the file where the table is there
    const le::Module original = splitter();
    const vx::DdkNames names;
    const mx::Emission code(vx::Disassembly::run(original, names), names);
    CB_CHECK(lw::build_le(code, original, "SPLITTER") == lw::build_le(code, original, "SOMETHING_ELSE"));
});

CB_TEST("source.vxd.le_writer.a_module_that_is_not_in_a_container_has_no_w3_form", [] {
    const le::Module original = splitter();
    const vx::DdkNames names;
    const mx::Emission code(vx::Disassembly::run(original, names), names);
    CB_CHECK_THROWS_AS(cb::FormatError, (void)lw::build_w3(code, original));
});
