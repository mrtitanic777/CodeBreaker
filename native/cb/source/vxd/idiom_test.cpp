// cb-unity: alone (test helpers named for this module)
// Tests of cb/source/vxd/idiom. The pieces (the DDK's names, the DDB macro, the service table, the control dispatch in both of its shapes, the
// data listing, the fix-up labelling) and two whole emissions (byte-exact and loadable) are checked on recoveries made by hand over the object
// table of the real driver splitter.vxd (one object, 0x604 bytes, executable): the expected text was worked out from the emission rules
// written in the header, not recorded from a run. The real driver goes through the module end to end (the source of the real thing is
// checked by its structure: balanced procedures, CRLF line ends, the DDB macro of the driver's own fields).
//
// Behaviour pinned here: a fix-up with no offset (a 16-bit selector) is no target;
// a service-table slot that is an import fix-up makes the table unrecoverable;
// the loadable emission does not build a deferred-macro list it never reads; the
// header of a tree file is written as before, but the file's line ends are the text's own (CRLF), and a source that is
// verified is assembled by the toolchain given, not by DOS only; the volume of drivers `--verify` takes is the folder's *.vxd and
// *.386 files.
#include "cb/source/vxd/idiom.hpp"

#include <algorithm>
#include <format>
#include <optional>
#include <ranges>
#include <type_traits>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace vx = cb::source::vxd;
namespace mx = cb::source::vxd::masm;
namespace id = cb::source::vxd::idiom;
namespace le = cb::formats::le;
using cb::Bytes;

namespace {

// ---- values as text, for the failure messages of CHECK_SAME (the in-tree show() cannot format a container) ----

std::string rendered(std::string_view s);
template <class T> requires(std::is_arithmetic_v<T>) std::string rendered(T v);
std::string rendered(const vx::Place& p);
template <class A, class B> std::string rendered(const std::pair<A, B>& p);
template <class T> std::string rendered(const std::optional<T>& v);
template <class R> requires(std::ranges::range<R> && !std::is_convertible_v<const R&, std::string_view>) std::string rendered(const R& range);

std::string rendered(std::string_view s) { return "\"" + std::string(s) + "\""; }
template <class T> requires(std::is_arithmetic_v<T>) std::string rendered(T v) { return std::to_string(v); }
std::string rendered(const vx::Place& p) { return std::format("({}, {:#x})", p.object, p.offset); }
template <class A, class B> std::string rendered(const std::pair<A, B>& p) { return "(" + rendered(p.first) + ", " + rendered(p.second) + ")"; }
template <class T> std::string rendered(const std::optional<T>& v) { return v ? rendered(*v) : std::string("none"); }
template <class R> requires(std::ranges::range<R> && !std::is_convertible_v<const R&, std::string_view>) std::string rendered(const R& range) {
    std::string out = "[";
    for (const auto& item : range) out += (out.size() > 1 ? ", " : "") + rendered(item);
    return out + "]";
}

#define CHECK_SAME(actual, expected) CB_CHECK_EQ(rendered(actual), rendered(expected))


le::Module splitter() { return le::Module::open(cb::testing::data_path("formats/testdata/splitter.vxd")); }

vx::DdkNames hand_names() {
    cb::OrderedMap<std::string, std::vector<std::string>> tables;
    tables["VMM"].resize(0x40);
    tables["VMM"][0x3F] = "Get_System_Time";
    tables["VKD"] = {"VKD_Get_Version", "VKD_Other", "VKD_Third"};
    tables["TEST"] = {"TEST_Get_Version", "TEST_Same", "TEST_Same", "TEST_Other"};
    return vx::DdkNames(vx::ServiceTables(std::move(tables)), {{1, "MSG_ONE"}, {3, "MSG_THREE"}}, {});
}

vx::Instruction insn(std::uint32_t size, std::string mnemonic, std::string operands, Bytes raw) {
    vx::Instruction i;
    i.size = size;
    i.mnemonic = std::move(mnemonic);
    i.operands = std::move(operands);
    i.raw = std::move(raw);
    return i;
}

le::InternalFixup internal(std::uint32_t object, std::optional<std::uint32_t> target, std::uint8_t source_type = 7) {
    le::InternalFixup f;
    f.object = object;
    f.target = target;
    f.source_type = source_type;
    return f;
}

void put_u32(Bytes& code, std::size_t at, std::uint32_t v) {
    for (std::size_t k = 0; k < 4; ++k) code[at + k] = static_cast<std::uint8_t>(v >> (8 * k));
}

le::Ddb ddb_at(std::int64_t rva, std::string name = "TEST") {
    le::Ddb d;
    d.object = 1;
    d.rva = rva;
    d.name = std::move(name);
    d.major = 3;
    d.minor = 10;
    d.device_id = 0x1234;
    d.init_order = 0x10000000;
    return d;
}

struct Hand {
    le::Module driver = splitter();
    vx::DdkNames names = hand_names();
    mx::EmissionParts parts;
    explicit Hand(std::size_t size = 0x200) { parts.code = {Bytes(size, 0)}; }
    mx::Emission emission() { return mx::Emission(driver, names, parts); }
    void fixup(std::uint32_t at, le::InternalFixup f) { parts.fixups.insert_or_assign(vx::Place{1, at}, le::Fixup(f)); }
    // An instruction whose bytes are in the object, as they are in a real recovery.
    void place(std::uint32_t at, vx::Instruction i) {
        std::copy(i.raw.begin(), i.raw.end(), parts.code[0].begin() + at);
        parts.instructions[{1, at}] = std::move(i);
    }
    void import_fixup(std::uint32_t at) { parts.fixups.insert_or_assign(vx::Place{1, at}, le::Fixup(le::ImportFixup{1, 5})); }
};

id::PlaceSet places(std::initializer_list<vx::Place> list) { return id::PlaceSet(list.begin(), list.end()); }

std::vector<std::string> lines_of(const std::string& source) {
    std::vector<std::string> out;
    for (const std::string_view line : cb::text::lines(source)) out.emplace_back(line);
    return out;
}

// The lines after the first line that is exactly `from`, up to the next that is exactly `to` (or the end).
std::vector<std::string> between(const std::vector<std::string>& lines, const std::string& from, const std::string& to) {
    const auto begin = std::find(lines.begin(), lines.end(), from);
    if (begin == lines.end()) return {};
    const auto end = std::find(begin + 1, lines.end(), to);
    return {begin + 1, end};
}

}  // namespace

CB_TEST("source.vxd.idiom.the_devices_whose_inc_ships", [] {
    CHECK_SAME(id::device_include(0x0001).value_or(""), "VMM");
    CHECK_SAME(id::device_include(0x002C).value_or(""), "VWIN32");
    CHECK_SAME(id::device_include(0x0442).value_or(""), "VTDAPI");
    CHECK_SAME(id::device_include(0x000D).value_or(""), "VKD");
    CB_CHECK(!id::device_include(0x000A).has_value());   // VDD: no public INC
    CB_CHECK(!id::device_include(0).has_value());
    CB_CHECK(!id::device_include(0x0443).has_value());
});

CB_TEST("source.vxd.idiom.an_object_includes_the_devices_it_calls", [] {
    Hand hand;
    auto service = [](std::uint32_t device, std::uint32_t number) {
        vx::Instruction i = insn(6, "int", "20h", {0xCD, 0x20, 0, 0, 0, 0});
        i.service = vx::ServiceCall{device, number, false};
        return i;
    };
    hand.parts.instructions[{1, 0}] = service(0x000D, 2);     // VKD
    hand.parts.instructions[{1, 6}] = service(0x002C, 1);     // VWIN32
    hand.parts.instructions[{1, 12}] = service(0x000A, 1);    // VDD: no INC, no include
    hand.parts.instructions[{1, 18}] = service(0x000D, 3);    // a second VKD call: once
    hand.parts.instructions[{2, 0}] = service(0x0015, 1);     // DOSMGR in another object
    const mx::Emission code = hand.emission();
    CHECK_SAME(id::device_includes(code, 1), (std::vector<std::string>{"VKD", "VMM", "VWIN32"}));   // sorted
    CHECK_SAME(id::device_includes(code, 2), (std::vector<std::string>{"DOSMGR", "VMM"}));
    CHECK_SAME(id::device_includes(code, 9), (std::vector<std::string>{"VMM"}));
});

CB_TEST("source.vxd.idiom.a_service_call_as_a_macro", [] {
    const vx::DdkNames names = hand_names();
    CHECK_SAME(*id::service_macro(names, 1, 0x3F), "        VMMcall Get_System_Time");
    CHECK_SAME(*id::service_macro(names, 1, 0x803F), "        VMMJmp Get_System_Time");   // DL_Jmp_Mask: a tail jump, not a call
    CHECK_SAME(*id::service_macro(names, 0x0D, 2), "        VxDcall VKD_Third");
    CHECK_SAME(*id::service_macro(names, 0x0D, 0x8001), "        VxDJmp VKD_Other");
    CB_CHECK(!id::service_macro(names, 1, 0x3E).has_value());        // a service the table does not name
    CB_CHECK(!id::service_macro(names, 1, 0x100).has_value());       // past the table
    CB_CHECK(!id::service_macro(names, 0x0A, 1).has_value());        // a device whose INC does not ship
    CB_CHECK(!id::service_macro(names, 0x0E, 1).has_value());        // a device the tables do not have
    CB_CHECK(!id::service_macro(vx::DdkNames(), 1, 0x3F).has_value());   // nothing is named
});

CB_TEST("source.vxd.idiom.the_ddb_as_a_macro", [] {
    Hand hand;
    const mx::Emission plain = hand.emission();
    const le::Ddb ddb = ddb_at(0x100);
    CHECK_SAME(id::declare_virtual_device(plain, ddb), "Declare_Virtual_Device TEST, 3, 10, TEST_Control, 01234h, 010000000h");
    CHECK_SAME(id::declare_virtual_device(plain, ddb, true), "Declare_Virtual_Device TEST, 3, 10, TEST_Control, 01234h, 010000000h");   // no handlers: no trailing arguments

    // the V86 API procedure (+0x1C) and the reference data (+0x2C) are fix-ups to handlers: the loadable macro names them, leaving the
    // protected-mode slot between them blank
    hand.fixup(0x11C, internal(1, 0x40));
    hand.fixup(0x12C, internal(1, 0x50));
    hand.parts.labels[{1, 0x40}] = "V86_Api";
    hand.parts.labels[{1, 0x50}] = "Ref_Data";
    const mx::Emission with = hand.emission();
    CHECK_SAME(id::declare_virtual_device(with, ddb), "Declare_Virtual_Device TEST, 3, 10, TEST_Control, 01234h, 010000000h");   // not loadable: left out
    CHECK_SAME(id::declare_virtual_device(with, ddb, true),
                "Declare_Virtual_Device TEST, 3, 10, TEST_Control, 01234h, 010000000h, V86_Api, , Ref_Data");
    hand.fixup(0x120, internal(1, 0x44));
    hand.parts.labels[{1, 0x44}] = "Pm_Api";
    const mx::Emission all = hand.emission();
    CHECK_SAME(id::declare_virtual_device(all, ddb, true), "Declare_Virtual_Device TEST, 3, 10, TEST_Control, 01234h, 010000000h, V86_Api, Pm_Api, Ref_Data");
    // only the protected-mode one: the V86 slot is blank, and nothing follows
    Hand pm_only;
    pm_only.fixup(0x120, internal(1, 0x44));
    pm_only.parts.labels[{1, 0x44}] = "Pm_Api";
    CHECK_SAME(id::declare_virtual_device(pm_only.emission(), ddb, true), "Declare_Virtual_Device TEST, 3, 10, TEST_Control, 01234h, 010000000h, , Pm_Api");
    // a fix-up to a place without a label is no handler; an import is none either
    Hand lost;
    lost.fixup(0x11C, internal(1, 0x99));
    lost.import_fixup(0x120);
    CHECK_SAME(id::declare_virtual_device(lost.emission(), ddb, true), "Declare_Virtual_Device TEST, 3, 10, TEST_Control, 01234h, 010000000h");
});

CB_TEST("source.vxd.idiom.the_size_of_the_ddb", [] {
    Hand hand;
    const le::Ddb ddb = ddb_at(0x100);
    const std::uint8_t marker[4] = {'v', 'e', 'r', 'P'};   // `dd 'Prev'` is stored little-endian
    CHECK_SAME(id::ddb_size(hand.emission(), ddb), 0x34U);   // the classic DDB: no marker
    std::copy(marker, marker + 4, hand.parts.code[0].begin() + 0x13C);
    CHECK_SAME(id::ddb_size(hand.emission(), ddb), 0x50U);   // the marker, a zero size field (the loader fills it): the macro's full 0x50
    put_u32(hand.parts.code[0], 0x140, 0x54);
    CHECK_SAME(id::ddb_size(hand.emission(), ddb), 0x54U);
    put_u32(hand.parts.code[0], 0x140, 0x80);
    CHECK_SAME(id::ddb_size(hand.emission(), ddb), 0x80U);
    put_u32(hand.parts.code[0], 0x140, 0x81);
    CHECK_SAME(id::ddb_size(hand.emission(), ddb), 0x50U);   // out of range
    put_u32(hand.parts.code[0], 0x140, 0x33);
    CHECK_SAME(id::ddb_size(hand.emission(), ddb), 0x50U);
    // "Prev" in the other order was never recognised as a modern DDB: the bytes in that order do not count
    std::copy(std::rbegin(marker), std::rend(marker), hand.parts.code[0].begin() + 0x13C);
    CHECK_SAME(id::ddb_size(hand.emission(), ddb), 0x34U);
    // a DDB at the very end of the object: no room for the marker
    Hand small(0x120);
    CHECK_SAME(id::ddb_size(small.emission(), ddb), 0x34U);
});

CB_TEST("source.vxd.idiom.data_as_text_or_hex", [] {
    const auto lines = [](std::string_view s, std::size_t width = 60) { return id::data_lines(cb::as_bytes(s), width); };
    using V = std::vector<std::string>;
    CHECK_SAME(lines("hello world"), (V{"        db  'hello world'"}));
    CHECK_SAME(lines("it's ok"), (V{"        db  'it''s ok'"}));   // a quote is doubled
    CHECK_SAME(lines("abc"), (V{"        db  061h, 062h, 063h"}));   // three printable bytes are not a string
    CHECK_SAME(lines("abcdefg", 3), (V{"        db  'abc'", "        db  'def'", "        db  'g'"}));   // long runs wrap
    CHECK_SAME(lines(std::string_view("\0\x01" "abcd\x02\x03", 8)), (V{"        db  000h, 001h", "        db  'abcd'", "        db  002h, 003h"}));
    CHECK_SAME(lines(std::string_view("\0abc\0", 5)), (V{"        db  000h, 061h, 062h, 063h, 000h"}));
    CHECK_SAME(id::data_lines(Bytes(20, 0)).size(), 2U);   // sixteen a line
    CHECK_SAME(id::data_lines(Bytes(20, 0)).back(), "        db  000h, 000h, 000h, 000h");
    CB_CHECK(id::data_lines({}).empty());
    CHECK_SAME(lines(std::string_view("Hello\0", 6)), (V{"        db  'Hello'", "        db  000h"}));
    CHECK_SAME(lines("\x7F\x7F\x7F\x7F"), (V{"        db  07Fh, 07Fh, 07Fh, 07Fh"}));   // DEL is not printable
});

CB_TEST("source.vxd.idiom.the_segments_of_a_loadable_object", [] {
    const auto s = [](std::uint32_t object, std::uint32_t flags, std::optional<std::uint32_t> ddb = std::nullopt) { return id::object_segment(object, flags, ddb); };
    CHECK_SAME(s(1, 0x4).name, "VSEG01");
    CHECK_SAME(s(1, 0x4).class_name, "S01CODE");
    CHECK_SAME(s(12, 0x3).class_name, "S12DATA");
    CHECK_SAME(s(2, 0x4, 1U).name, "VSEG02");   // not the DDB's object
    CHECK_SAME(s(1, 0x4, 1U).name, "_LTEXT");   // the DDB's object: the DDK's own segment, merged with the macros' DDB and table
    CHECK_SAME(s(1, 0x3, 1U).class_name, "LCODE");
});

namespace {

// A recovery with a service table of three entries at 0x180, the DDB at 0x100: Service_Table_Ptr (+0x30) a fix-up to 0x180, Size (+0x34) 3.
Hand service_hand() {
    Hand hand;
    hand.parts.ddb = ddb_at(0x100);
    hand.fixup(0x130, internal(1, 0x180));
    put_u32(hand.parts.code[0], 0x134, 3);
    hand.fixup(0x180, internal(1, 0x10));
    hand.fixup(0x188, internal(1, 0x20));   // slot 1 (0x184) is null
    hand.parts.labels[{1, 0x10}] = "sub_1_0010";
    hand.parts.labels[{1, 0x20}] = "TEST_Other";
    return hand;
}

}  // namespace

CB_TEST("source.vxd.idiom.the_service_table_of_the_device", [] {
    Hand hand = service_hand();
    hand.parts.ddb->name = "VKD";   // the names are looked up by the DDB's name
    const mx::Emission code = hand.emission();
    const auto table = id::service_table(code, places({{1, 0x10}, {1, 0x20}}));
    CB_CHECK(table.has_value());
    CHECK_SAME(table->names, (std::vector<std::string>{"VKD_Get_Version", "RESERVED", "VKD_Third"}));   // a null slot is RESERVED
    CHECK_SAME(table->at, (vx::Place{1, 0x180}));
    CHECK_SAME(table->count, 3U);
    // the first handler's label is not its service name: the name is defined there as an extra; the second already IS "TEST_Other"
    // (here named for a different service, so it too gets the extra)
    CHECK_SAME(table->extra.size(), 2U);
    CHECK_SAME(table->extra.at({1, 0x10}), (std::vector<std::string>{"VKD_Get_Version"}));
    CHECK_SAME(table->extra.at({1, 0x20}), (std::vector<std::string>{"VKD_Third"}));
});

CB_TEST("source.vxd.idiom.a_service_whose_name_is_its_handlers_label_needs_no_extra", [] {
    Hand hand = service_hand();   // the DDB is "TEST": services 0 and 3 are named in the hand tables, 3 is "TEST_Other"
    hand.fixup(0x188, internal(1, 0x20));
    hand.parts.labels[{1, 0x20}] = "TEST_Same";   // slot 2 is TEST_Same
    const mx::Emission code = hand.emission();
    const auto table = id::service_table(code, places({{1, 0x10}, {1, 0x20}}));
    CB_CHECK(table.has_value());
    CHECK_SAME(table->names, (std::vector<std::string>{"TEST_Get_Version", "RESERVED", "TEST_Same"}));
    CB_CHECK(!table->extra.contains({1, 0x20}));
    CB_CHECK(table->extra.contains({1, 0x10}));
});

CB_TEST("source.vxd.idiom.the_service_table_is_left_raw_when_it_cannot_be_trusted", [] {
    const id::PlaceSet both = places({{1, 0x10}, {1, 0x20}});
    {   // a handler that is not an emitted label (it landed mid-instruction)
        Hand hand = service_hand();
        CB_CHECK(!id::service_table(hand.emission(), places({{1, 0x10}})).has_value());
    }
    {   // a slot that is not zero and has no fix-up
        Hand hand = service_hand();
        put_u32(hand.parts.code[0], 0x184, 0x1234);
        CB_CHECK(!id::service_table(hand.emission(), both).has_value());
    }
    {   // a slot that is an import fix-up
        Hand hand = service_hand();
        hand.import_fixup(0x184);
        CB_CHECK(!id::service_table(hand.emission(), both).has_value());
    }
    {   // a slot that is a selector fix-up
        Hand hand = service_hand();
        hand.fixup(0x184, internal(1, std::nullopt));
        CB_CHECK(!id::service_table(hand.emission(), both).has_value());
    }
    {   // two services with the identical name (TEST_Same twice)
        Hand hand = service_hand();
        hand.fixup(0x184, internal(1, 0x10));
        hand.fixup(0x188, internal(1, 0x20));
        CB_CHECK(!id::service_table(hand.emission(), both).has_value());
    }
    {   // no services, too many, past the object
        Hand hand = service_hand();
        put_u32(hand.parts.code[0], 0x134, 0);
        CB_CHECK(!id::service_table(hand.emission(), both).has_value());
        put_u32(hand.parts.code[0], 0x134, 8192);
        CB_CHECK(!id::service_table(hand.emission(), both).has_value());
        put_u32(hand.parts.code[0], 0x134, 0x40);   // 0x180 + 0x100 > 0x200
        CB_CHECK(!id::service_table(hand.emission(), both).has_value());
    }
    {   // no DDB; the table pointer a raw dword when it has no fix-up
        Hand hand = service_hand();
        hand.parts.ddb.reset();
        CB_CHECK(!id::service_table(hand.emission(), both).has_value());
        Hand raw = service_hand();
        raw.parts.fixups = {};
        put_u32(raw.parts.code[0], 0x130, 0x180);   // the raw dword, not a relocation
        raw.fixup(0x180, internal(1, 0x10));
        raw.fixup(0x188, internal(1, 0x20));
        CB_CHECK(id::service_table(raw.emission(), both).has_value());
    }
});

namespace {

// The control procedure as a jump table: `sub eax,1` (so message = index + 1), a range check to the default stub at 0x70, the jump through
// the table at 0x90 (three entries: handler 0x50, the default, handler 0x60), a `ret`, and the handlers.
Hand jump_table_hand() {
    Hand hand;
    hand.parts.ddb = ddb_at(0x100);
    auto& i = hand.parts.instructions;
    i[{1, 0x40}] = insn(3, "sub", "eax, 0x1", {0x83, 0xE8, 0x01});
    i[{1, 0x43}] = insn(3, "cmp", "eax, 0x2", {0x83, 0xF8, 0x02});
    i[{1, 0x46}] = insn(2, "jae", "0x70", {0x73, 0x28});
    i[{1, 0x48}] = insn(7, "jmp", "dword ptr [eax*4 + 0x90]", {0xFF, 0x24, 0x85, 0x90, 0, 0, 0});
    i[{1, 0x4F}] = insn(1, "ret", "", {0xC3});
    i[{1, 0x50}] = insn(2, "mov", "eax, 1", {0xB0, 0x01});
    i[{1, 0x60}] = insn(1, "nop", "", {0x90});
    i[{1, 0x70}] = insn(1, "ret", "", {0xC3});
    hand.parts.labels[{1, 0x40}] = "TEST_Control";
    hand.parts.labels[{1, 0x50}] = "sub_1_0050";
    hand.parts.labels[{1, 0x70}] = "sub_1_0070";
    hand.fixup(0x4B, internal(1, 0x90));
    hand.parts.jump_tables[{1, 0x90}] = {{1, 0x50}, {1, 0x70}, {1, 0x60}};
    return hand;
}

std::set<std::int64_t> offsets_of(const id::PlaceSet& skip) {
    std::set<std::int64_t> out;
    for (const vx::Place& p : skip) out.insert(p.offset);
    return out;
}

}  // namespace

CB_TEST("source.vxd.idiom.a_control_procedure_with_a_jump_table", [] {
    Hand hand = jump_table_hand();
    const mx::Emission code = hand.emission();
    const auto dispatch = id::control_dispatch(code, places({{1, 0x50}, {1, 0x60}, {1, 0x70}}));
    // messages 1 and 3 have handlers (the table index plus the base of the `sub`); the entry that is the range check's target is the default
    // stub and is dropped; message 2 is not a message the DDK names
    const std::vector<std::pair<std::string, std::string>> handlers = {{"MSG_ONE", "sub_1_0050"}, {"MSG_THREE", "loc_1_0060"}};   // 0x60 has no label: a name is made
    CHECK_SAME(dispatch.handlers, handlers);
    // what the macro regenerates: the prologue up to and including the jmp, the table (3 dwords at 0x90) and the trailing ret
    std::set<std::int64_t> expected;
    for (std::int64_t k = 0x40; k < 0x4F; ++k) expected.insert(k);
    expected.insert(0x4F);
    for (std::int64_t k = 0x90; k < 0x9C; ++k) expected.insert(k);
    CHECK_SAME(offsets_of(dispatch.skip), expected);
});

CB_TEST("source.vxd.idiom.a_handler_shared_by_two_messages_is_kept", [] {
    Hand hand = jump_table_hand();
    hand.parts.jump_tables[{1, 0x90}] = {{1, 0x50}, {1, 0x70}, {1, 0x50}};   // the same handler for messages 1 and 3
    const auto dispatch = id::control_dispatch(hand.emission(), places({{1, 0x50}}));
    CHECK_SAME(dispatch.handlers.size(), 2U);
    CHECK_SAME(dispatch.handlers.at(1).second, "sub_1_0050");
    // with no range check to name the default stub, a handler that appears twice is dropped
    Hand blind = jump_table_hand();
    blind.parts.instructions.erase({1, 0x46});
    blind.parts.jump_tables[{1, 0x90}] = {{1, 0x50}, {1, 0x70}, {1, 0x50}};
    const auto fallback = id::control_dispatch(blind.emission(), places({{1, 0x50}}));
    // (1, 0x70) is unique in the table but message 2 is not a message the DDK names: nothing is left
    CB_CHECK(fallback.handlers.empty());
});

CB_TEST("source.vxd.idiom.a_control_procedure_with_a_compare_chain", [] {
    Hand hand;
    hand.parts.ddb = ddb_at(0x100);
    auto& i = hand.parts.instructions;
    i[{1, 0x40}] = insn(3, "cmp", "eax, 0x1", {0x83, 0xF8, 0x01});
    i[{1, 0x43}] = insn(2, "je", "0x50", {0x74, 0x0B});
    i[{1, 0x45}] = insn(3, "cmp", "eax, 0x3", {0x83, 0xF8, 0x03});
    i[{1, 0x48}] = insn(2, "je", "0x60", {0x74, 0x16});
    i[{1, 0x4A}] = insn(1, "clc", "", {0xF8});
    i[{1, 0x4B}] = insn(1, "ret", "", {0xC3});
    i[{1, 0x50}] = insn(1, "nop", "", {0x90});
    i[{1, 0x60}] = insn(1, "nop", "", {0x90});
    hand.parts.labels[{1, 0x40}] = "TEST_Control";
    hand.parts.labels[{1, 0x50}] = "sub_1_0050";
    hand.parts.labels[{1, 0x60}] = "sub_1_0060";
    const id::PlaceSet emittable = places({{1, 0x50}, {1, 0x60}});
    const auto dispatch = id::control_dispatch(hand.emission(), emittable);
    CHECK_SAME(dispatch.handlers, (std::vector<std::pair<std::string, std::string>>{{"MSG_ONE", "sub_1_0050"}, {"MSG_THREE", "sub_1_0060"}}));
    // the two compare/jump pairs and the unhandled stub after them (clc, ret)
    std::set<std::int64_t> expected;
    for (std::int64_t k = 0x40; k < 0x4C; ++k) expected.insert(k);
    CHECK_SAME(offsets_of(dispatch.skip), expected);

    // a message the DDK does not name makes the whole procedure ordinary code
    hand.parts.instructions[{1, 0x45}] = insn(3, "cmp", "eax, 0x4", {0x83, 0xF8, 0x04});
    CB_CHECK(id::control_dispatch(hand.emission(), emittable).handlers.empty());
    hand.parts.instructions[{1, 0x45}] = insn(3, "cmp", "eax, 0x1", {0x83, 0xF8, 0x01});   // the same message twice
    CB_CHECK(id::control_dispatch(hand.emission(), emittable).handlers.empty());
    hand.parts.instructions[{1, 0x45}] = insn(3, "cmp", "eax, 0x3", {0x83, 0xF8, 0x03});
    // a handler that is not an emitted label
    CB_CHECK(id::control_dispatch(hand.emission(), places({{1, 0x50}})).handlers.empty());
    // one compare is not a chain
    hand.parts.instructions.erase({1, 0x45});
    CB_CHECK(id::control_dispatch(hand.emission(), emittable).handlers.empty());
});

CB_TEST("source.vxd.idiom.a_control_procedure_that_is_neither_shape_is_not_recovered", [] {
    Hand hand = jump_table_hand();
    hand.parts.labels.erase({1, 0x40});
    CB_CHECK(id::control_dispatch(hand.emission(), places({{1, 0x50}})).handlers.empty());   // no <name>_Control label
    Hand no_ddb = jump_table_hand();
    no_ddb.parts.ddb.reset();
    CB_CHECK(id::control_dispatch(no_ddb.emission(), places({{1, 0x50}})).handlers.empty());
    Hand no_table = jump_table_hand();
    no_table.parts.jump_tables.clear();   // the jmp [eax*4+..] has no table to read
    CB_CHECK(id::control_dispatch(no_table.emission(), places({{1, 0x50}})).handlers.empty());
    Hand plain;
    plain.parts.ddb = ddb_at(0x100);
    plain.parts.instructions[{1, 0x40}] = insn(1, "ret", "", {0xC3});
    plain.parts.labels[{1, 0x40}] = "TEST_Control";
    CB_CHECK(id::control_dispatch(plain.emission(), {}).handlers.empty());
    CB_CHECK(id::control_dispatch(plain.emission(), {}).skip.empty());
});

CB_TEST("source.vxd.idiom.every_fixup_target_gets_a_label", [] {
    Hand hand;
    hand.parts.code = {Bytes(0x100, 0)};
    hand.parts.instructions[{1, 0x20}] = insn(2, "nop", "", {0x90, 0x90});
    hand.parts.instructions[{1, 0x30}] = insn(4, "mov", "eax, ebx", {1, 2, 3, 4});
    hand.parts.labels[{1, 0x40}] = "already";
    hand.fixup(0x00, internal(1, 0x20));            // an instruction start: labelled
    hand.fixup(0x04, internal(1, 0x31));            // inside an instruction: not
    hand.fixup(0x08, internal(1, 0x80));            // a data byte: labelled
    hand.fixup(0x0C, internal(1, 0x80000020));      // the high-linear alias of 0x20: the same label, once
    hand.fixup(0x10, internal(1, 0x7FFFFFF0));      // far beyond the object: raw
    hand.fixup(0x14, internal(1, 0x40));            // already named: unchanged
    hand.fixup(0x18, internal(1, std::nullopt));    // a selector: no target
    hand.fixup(0x1C, internal(7, 0x10));            // an object the module does not have: labelled
    hand.import_fixup(0x24);
    mx::Emission code = hand.emission();
    id::label_fixup_targets(code);
    std::map<vx::Place, std::string> expected = {{{1, 0x20}, "loc_1_0020"}, {{1, 0x40}, "already"}, {{1, 0x80}, "loc_1_0080"}, {{7, 0x10}, "loc_7_0010"}};
    CHECK_SAME(code.labels(), expected);
});

namespace {

// The object as the byte-exact tree emits it: a DDB at 0 (0x34 bytes), twelve data bytes, a function that calls a VMM service, and a string.
Hand tree_hand() {
    Hand hand(0x51);
    hand.parts.ddb = ddb_at(0);
    hand.parts.ddb->init_order = 0;
    hand.parts.labels[{1, 0x40}] = "sub_1_0040";
    auto& i = hand.parts.instructions;
    i[{1, 0x40}] = insn(1, "push", "ebp", {0x55});
    i[{1, 0x41}] = insn(2, "mov", "ebp, esp", {0x8B, 0xEC});
    vx::Instruction call = insn(6, "int", "20h", {0xCD, 0x20, 0x3F, 0x00, 0x01, 0x00});
    call.service = vx::ServiceCall{1, 0x3F, false};
    i[{1, 0x43}] = call;
    i[{1, 0x49}] = insn(1, "pop", "ebp", {0x5D});
    i[{1, 0x4A}] = insn(1, "ret", "", {0xC3});
    const std::string_view text = "Hello";
    std::copy(text.begin(), text.end(), hand.parts.code[0].begin() + 0x4B);
    return hand;
}

}  // namespace

CB_TEST("source.vxd.idiom.a_byte_exact_source", [] {
    Hand hand = tree_hand();
    mx::Emission code = hand.emission();
    const std::string source = id::emit(code);
    CB_CHECK(source.starts_with(".586p\r\ninclude vmm.inc\r\n\r\nDeclare_Virtual_Device TEST, 3, 10, TEST_Control, 01234h, 000000000h\r\n\r\n"));
    CB_CHECK(source.ends_with("        end\r\n"));
    const std::vector<std::string> lines = lines_of(source);
    // the object's body: the DDB (the macro writes it) is skipped; twelve data bytes, then the function with the service as its macro, then the string
    const std::vector<std::string> body = {
        "        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h",
        "BeginProc sub_1_0040",
        "        push   ebp",
        "        mov    ebp, esp",
        "        VMMcall Get_System_Time",
        "        pop    ebp",
        "        ret",
        "        db  'Hello'",
        "        db  000h",
        "EndProc sub_1_0040",
    };
    const auto start = std::find(lines.begin(), lines.end(), "Declare_Virtual_Device TEST, 3, 10, TEST_Control, 01234h, 000000000h");
    CB_CHECK(start != lines.end());
    const std::vector<std::string> after(start + 2, lines.end());   // past the macro and the blank line
    CB_CHECK(after.at(0).ends_with("_SEG"));                        // VxD_LOCKED_CODE_SEG (or the object's own macro)
    CHECK_SAME(std::vector<std::string>(after.begin() + 1, after.begin() + 1 + static_cast<std::ptrdiff_t>(body.size())), body);
    // the BSS tail: the object is 0x604 bytes, the code 0x51
    CB_CHECK(std::find(after.begin(), after.end(), "        db 1459 dup (0)") != after.end());
});

CB_TEST("source.vxd.idiom.the_labels_the_macros_make_are_renamed", [] {
    Hand hand = tree_hand();
    hand.parts.labels[{1, 0x4B}] = "TEST_DDB";   // a .SYM names a routine like the DDB the macro defines
    hand.parts.labels[{1, 0x4A}] = "TEST_Service_Table";
    mx::Emission code = hand.emission();
    const std::string source = id::emit(code);
    CB_CHECK(source.find("TEST_DDB") == std::string::npos);   // ML would report A2005 on a second definition
    CB_CHECK(source.find("TEST_Service_Table") == std::string::npos);
    CB_CHECK(code.labels().at({1, 0x4B}) == "sub_1_004B");
    CB_CHECK(code.labels().at({1, 0x4A}) == "sub_1_004A");
    CB_CHECK(source.find("BeginProc sub_1_004B") != std::string::npos);
});

namespace {

// The loadable flavour: a call whose displacement is a self-relative fix-up, a load of an absolute address that is a relocation, a ret, a data
// pointer. No DDB: the segments are the objects' own FLAT ones.
Hand loadable_hand() {
    Hand hand(0x18);
    hand.place(0x00, insn(5, "call", "0x10", {0xE8, 0, 0, 0, 0}));
    hand.place(0x05, insn(5, "mov", "eax, dword ptr [0x1000]", {0xA1, 0x00, 0x10, 0x00, 0x00}));
    hand.place(0x0A, insn(1, "ret", "", {0xC3}));
    hand.place(0x10, insn(1, "ret", "", {0xC3}));
    hand.parts.instructions[{1, 0x00}].fixup = le::Fixup(internal(1, 0x10, 8));
    hand.parts.instructions[{1, 0x05}].fixup = le::Fixup(internal(1, 0x14, 7));
    hand.fixup(0x01, internal(1, 0x10, 8));
    hand.fixup(0x06, internal(1, 0x14, 7));
    hand.fixup(0x14, internal(1, 0x10, 7));
    hand.parts.labels[{1, 0x10}] = "sub_1_0010";
    hand.parts.labels[{1, 0x14}] = "loc_1_0014";
    return hand;
}

}  // namespace

CB_TEST("source.vxd.idiom.a_loadable_source", [] {
    Hand hand = loadable_hand();
    mx::Emission code = hand.emission();
    const std::string source = id::emit(code, true);
    const std::vector<std::string> lines = lines_of(source);
    const le::Object& object = hand.driver.objects().front();
    const id::SegmentName segment = id::object_segment(object.index, object.flags);
    // every object's own FLAT segment is declared before anything else, with a class of its own
    CB_CHECK(std::find(lines.begin(), lines.end(), segment.name + " SEGMENT DWORD PUBLIC _FLAT '" + segment.class_name + "'") != lines.end());
    CB_CHECK(source.find("Declare_Virtual_Device") == std::string::npos);   // no DDB here to write in place, and never the macro
    const std::vector<std::string> body = between(lines, segment.name + " SEGMENT", "never");
    const std::vector<std::string> expected = {
        "   ASSUME cs:FLAT, ds:FLAT, es:FLAT, ss:FLAT",
        "        call   sub_1_0010",                       // the self-relative displacement is the label's, so LINK writes a relocation
        "        db  0A1h",
        "        dd  offset loc_1_0014",                   // the absolute address is a relocation, not four raw bytes
        "        db  0C3h",                                // an instruction without a relocation stays its exact bytes
        "        db  000h, 000h, 000h, 000h, 000h",
        "BeginProc sub_1_0010",
        "        db  0C3h",
        "        db  000h, 000h, 000h",
        "loc_1_0014:",
        "        dd  offset sub_1_0010",                   // a data pointer
        "EndProc sub_1_0010",
    };
    CB_CHECK(body.size() >= expected.size());
    CHECK_SAME(std::vector<std::string>(body.begin(), body.begin() + static_cast<std::ptrdiff_t>(expected.size())), expected);
});

CB_TEST("source.vxd.idiom.a_pointer_whose_field_runs_past_a_short_instruction_is_one_dd", [] {
    // a data pointer decoded as code: the instruction (3 bytes) is shorter than the 4-byte relocated field that starts in it
    Hand hand(0x20);
    hand.place(0, insn(3, "add", "al, 0x10", {0x04, 0x10, 0x90}));
    hand.fixup(0x01, internal(1, 0x18));
    hand.parts.labels[{1, 0x18}] = "loc_1_0018";
    mx::Emission code = hand.emission();
    const std::vector<std::string> lines = lines_of(id::emit(code, true));
    const le::Object& object = hand.driver.objects().front();
    const std::vector<std::string> body = between(lines, id::object_segment(object.index, object.flags).name + " SEGMENT", "never");
    CHECK_SAME(body.at(1), "        db  004h");
    CHECK_SAME(body.at(2), "        dd  offset loc_1_0018");
    // the walk went past the field: the next thing is data from byte 5 on, up to the label
    CHECK_SAME(body.at(3), id::data_lines(Bytes(16, 0)).front());   // 19 bytes up to the label: sixteen a line, then three
    CHECK_SAME(body.at(4), id::data_lines(Bytes(3, 0)).front());
});

CB_TEST("source.vxd.idiom.the_real_driver_as_source", [] {
    const le::Module driver = splitter();
    cb::OrderedMap<std::string, std::vector<std::string>> tables;
    const vx::DdkNames names(vx::ServiceTables(std::move(tables)), {{0, "SYS_CRITICAL_INIT"}, {1, "DEVICE_INIT"}, {2, "INIT_COMPLETE"}}, {});
    for (const bool loadable : {false, true}) {
        mx::Emission code(vx::Disassembly::run(driver, names), names);
        const std::string source = id::emit(code, loadable);
        CB_CHECK(source.starts_with(".586p\r\ninclude vmm.inc\r\n"));
        CB_CHECK(source.ends_with("        end\r\n"));
        for (std::size_t at = source.find('\n'); at != std::string::npos; at = source.find('\n', at + 1)) CB_CHECK(at > 0 && source[at - 1] == '\r');   // CRLF only
        const std::vector<std::string> lines = lines_of(source);
        const auto begins = std::count_if(lines.begin(), lines.end(), [](const std::string& l) { return l.starts_with("BeginProc "); });
        const auto ends = std::count_if(lines.begin(), lines.end(), [](const std::string& l) { return l.starts_with("EndProc "); });
        CHECK_SAME(begins, ends);
        CB_CHECK(begins >= 5);   // the routines the recovery named from the calls and the service table
        if (loadable) {
            CB_CHECK(source.find("public Splitter_DDB") != std::string::npos);   // the DDB in place, at its real offset
            CB_CHECK(source.find("Declare_Virtual_Device") == std::string::npos);
            CB_CHECK(source.find("\r\n        dd  offset ") != std::string::npos);   // relocations are symbolic
        } else {
            CB_CHECK(source.find("Declare_Virtual_Device Splitter, 3, 10, Splitter_Control, 00498h, 080000000h") != std::string::npos);
        }
    }
});

CB_TEST("source.vxd.idiom.the_real_driver_has_a_service_table", [] {
    const le::Module driver = splitter();
    const vx::DdkNames names;
    const vx::Disassembly found = vx::Disassembly::run(driver, names);
    const mx::Emission code(found, names);
    id::PlaceSet emittable;
    for (const auto& [at, label] : code.labels()) emittable.insert(at);
    const auto table = id::service_table(code, emittable);
    CB_CHECK(table.has_value());
    CHECK_SAME(table->count, 5U);   // "service table: obj1:0x588, 5 services" in the disassembly's listing
    CHECK_SAME(table->at, (vx::Place{1, 0x588}));
    CHECK_SAME(table->names.at(0), "Splitter_Service_000");   // no DDK names here
});

CB_TEST("source.vxd.idiom.a_tree_file_has_the_header_and_the_source", [] {
    const le::Module driver = splitter();
    const vx::DdkNames names;
    cb::fs::ScopedTempDir dir("cbidiom");
    const std::filesystem::path path = id::write_tree(driver, "SPLITTER", dir.path(), names);
    CHECK_SAME(cb::fs::to_text(path.filename()), "splitter.asm");
    CHECK_SAME(cb::fs::to_text(path.parent_path().filename()), "splitter");
    const std::string text = cb::fs::read_text(path);
    CB_CHECK(text.starts_with("; SPLITTER.ASM -- SPLITTER reconstructed in idiomatic VMM DDK-macro source form.\n;\n"));
    CB_CHECK(text.find("; service names and control-dispatch handler names ARE the real DDK names.\n\r\n.586p\r\n") != std::string::npos);
    CB_CHECK(text.ends_with("        end\r\n"));
});

CB_TEST("source.vxd.idiom.every_source_goes_through_the_assembler_given", [] {
    const le::Module driver = splitter();
    const vx::DdkNames names;
    std::vector<std::size_t> batch_sizes;
    mx::Toolchain tools;
    tools.period = [&](std::span<const std::string> texts) {
        batch_sizes.push_back(texts.size());
        std::vector<mx::Assembled> out;
        for (const std::string& t : texts) out.push_back({t.find(".586p") != std::string::npos ? std::optional<Bytes>(Bytes{1}) : std::nullopt, ""});
        return out;
    };
    const std::vector<id::NamedDriver> drivers = {{"splitter", &driver}, {"again", &driver}};
    const auto result = id::verify(drivers, names, tools);
    CHECK_SAME(result.size(), 2U);
    CB_CHECK(result.at("splitter") && result.at("again"));
    CHECK_SAME(batch_sizes, (std::vector<std::size_t>{2}));   // one session for both
    // a budget smaller than a source runs each alone
    batch_sizes.clear();
    (void)id::verify(drivers, names, tools, 10);
    CHECK_SAME(batch_sizes, (std::vector<std::size_t>{1, 1}));
    // no assembler: nothing assembles
    const auto none = id::verify(drivers, names, mx::Toolchain{});
    CB_CHECK(!none.at("splitter"));
});
