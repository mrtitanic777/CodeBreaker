// Tests of cb/analyze/classes.hpp on a small 32-bit module written byte by byte: three virtual tables (one named by run-time type information
// with a base, one by an MFC class record, one that states nothing and is inferred to derive from the first), a message map found the way MFC
// lays it out and tied to its class through a getter, a C++ try/catch table, a `__try` scope table, and the report made of them.
//
// DIVERGES from the Python module and its port:
//  - results are values (vectors of records) made from the image, the call graph and the calling-convention facts it is handed, and are not
//    kept in the image's memo; the hierarchy takes the structures recovered for the module as an argument;
//  - a message map entry is a record (message, code, id, last id, signature, handler), not a 6-int tuple;
//  - a 16-bit module has no classes here (as before), and every answer is empty for it.
#include "cb/analyze/classes.hpp"

#include "cb/analyze/test_image.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::analyze;
namespace cl = cb::analyze::classes;

namespace {

constexpr Va kCode = 0x401000, kData = 0x403000;
// functions
constexpr Va kCtorFoo = 0x401000, kB = 0x401010, kC = 0x401020, kD = 0x401030, kGetMap = 0x401040, kHandler = 0x401050, kCatch = 0x401060,
             kCtorMfc = 0x401070, kGetRecord = 0x401080, kCtorDerived = 0x4010A0, kEnd = 0x4010B0;
// data
constexpr Va kTableFoo = 0x403010, kTableMfc = 0x403020, kTableDerived = 0x403030, kLocator = 0x403100, kTypeFoo = 0x403200, kHierarchy = 0x403300,
             kBaseArray = 0x403340, kBaseEntry = 0x403360, kTypeBar = 0x403380, kMapEntries = 0x403400, kMapRecord = 0x403480, kEh = 0x403500,
             kUnwind = 0x403540, kTryMap = 0x403560, kHandlers = 0x403580, kRecordBase = 0x403600, kRecordDerived = 0x403614, kNameBase = 0x403680,
             kNameDerived = 0x403690, kScopes = 0x4036C0, kDataEnd = 0x403700;

struct Data {
    Bytes bytes = Bytes(kDataEnd - kData, 0);
    void word(Va at, std::uint32_t v) {
        for (int i = 0; i < 4; ++i) bytes[static_cast<std::size_t>(at - kData) + i] = static_cast<std::uint8_t>(v >> (8 * i));
    }
    void text(Va at, const std::string& s) {
        for (std::size_t i = 0; i < s.size(); ++i) bytes[static_cast<std::size_t>(at - kData) + i] = static_cast<std::uint8_t>(s[i]);
    }
};

Bytes code_bytes() {
    test::Asm a(kCode);
    auto store_table = [&](Va table) { a.raw({0xC7, 0x01}).dword(static_cast<std::uint32_t>(table)).ret(); };   // mov dword [ecx], table ; ret
    store_table(kTableFoo);
    a.pad_to(kB).ret().pad_to(kC).ret().pad_to(kD).ret().pad_to(kGetMap);
    a.raw({0xB8}).dword(static_cast<std::uint32_t>(kMapRecord)).ret().pad_to(kHandler);   // mov eax, offset map ; ret
    a.ret().pad_to(kCatch).ret().pad_to(kCtorMfc);
    store_table(kTableMfc);
    a.pad_to(kGetRecord).raw({0xB8}).dword(static_cast<std::uint32_t>(kRecordDerived)).ret().pad_to(kCtorDerived);
    store_table(kTableDerived);
    a.pad_to(kEnd);
    return a.bytes();
}

Bytes data_bytes() {
    Data d;
    // Foo: [GetMessageMap, C, D] with run-time type information before the table; one base, Bar
    d.word(kTableFoo - 4, static_cast<std::uint32_t>(kLocator));
    d.word(kTableFoo, kGetMap);
    d.word(kTableFoo + 4, kC);
    d.word(kTableFoo + 8, kD);
    d.word(kLocator + 12, kTypeFoo);
    d.word(kLocator + 16, kHierarchy);
    d.text(kTypeFoo + 8, ".?AVFoo@@");
    d.word(kHierarchy + 8, 2);
    d.word(kHierarchy + 12, kBaseArray);
    d.word(kBaseArray, 0);
    d.word(kBaseArray + 4, kBaseEntry);
    d.word(kBaseEntry, kTypeBar);
    d.text(kTypeBar + 8, ".?AVBar@@");
    // the MFC class: [a function that returns its CRuntimeClass record, B, C]
    d.word(kTableMfc, kGetRecord);
    d.word(kTableMfc + 4, kB);
    d.word(kTableMfc + 8, kC);
    // a third table that shares Foo's slots and has one more
    d.word(kTableDerived, kGetMap);
    d.word(kTableDerived + 4, kC);
    d.word(kTableDerived + 8, kD);
    d.word(kTableDerived + 12, kHandler);
    // the message map: ON_COMMAND(100), ON_COMMAND_RANGE(101..110), WM_PAINT, then the zero entry; the AFX_MSGMAP record that points at it
    const std::uint32_t entries[][6] = {{0x111, 0, 100, 100, 5, kHandler}, {0x111, 0, 101, 110, 5, kHandler}, {0x0F, 0, 0, 0, 12, kHandler}};
    for (int i = 0; i < 3; ++i)
        for (int k = 0; k < 6; ++k) d.word(kMapEntries + 24 * i + 4 * k, entries[i][k]);
    d.word(kMapRecord, 0);
    d.word(kMapRecord + 4, kMapEntries);
    // a try block with one catch (...) and one cleanup
    d.word(kEh, 0x19930520);
    d.word(kEh + 4, 1);
    d.word(kEh + 8, kUnwind);
    d.word(kEh + 12, 1);
    d.word(kEh + 16, kTryMap);
    d.word(kUnwind, 0xFFFFFFFF);
    d.word(kUnwind + 4, kCatch);
    d.word(kTryMap + 12, 1);
    d.word(kTryMap + 16, kHandlers);
    d.word(kHandlers + 12, kCatch);
    // MFC's records: CBase, and CDerived with CBase as its base
    d.word(kRecordBase, kNameBase);
    d.word(kRecordBase + 4, 0x20);
    d.word(kRecordBase + 8, 0xFFFF);
    d.word(kRecordDerived, kNameDerived);
    d.word(kRecordDerived + 4, 0x40);
    d.word(kRecordDerived + 8, 0xFFFF);
    d.word(kRecordDerived + 12, kHandler);
    d.word(kRecordDerived + 16, kRecordBase);
    d.text(kNameBase, "CBase");
    d.text(kNameDerived, "CDerived");
    // a __try scope table: a __finally, then an __except inside it
    d.word(kScopes, 0xFFFFFFFF);
    d.word(kScopes + 4, 0);
    d.word(kScopes + 8, kCatch);
    d.word(kScopes + 12, 0);
    d.word(kScopes + 16, kHandler);
    d.word(kScopes + 20, kCatch);
    return d.bytes;
}

struct Program {
    Image image;
    call_graph::Graph graph;
    ConventionFacts facts;

    explicit Program(int bits = 32) : image(make(bits)), graph(call_graph::build(image)) {
        for (Va f : image.functions()) facts[f] = ConventionFact{"stdcall", std::nullopt, 0, false, {}};
        // the COM shape: the first three virtual functions remove 12, 4 and 4 bytes of arguments
        facts[kGetMap].removes = 12;
        facts[kC].removes = 4;
        facts[kD].removes = 4;
    }
    static Image make(int bits) {
        ImageSpec spec;
        spec.kind = ImageKind::Pe;
        spec.bits = bits;
        spec.flat = true;
        Span code;
        code.va = kCode;
        code.data = code_bytes();
        code.executable = true;
        code.initialised = static_cast<Va>(code.data.size());
        code.name = ".text";
        Span data;
        data.va = kData;
        data.data = data_bytes();
        data.writable = true;
        data.initialised = static_cast<Va>(data.data.size());
        data.name = ".data";
        spec.spans = {code, data};
        const Va starts[] = {kCtorFoo, kB, kC, kD, kGetMap, kHandler, kCatch, kCtorMfc, kGetRecord, kCtorDerived};
        for (std::size_t i = 0; i < std::size(starts); ++i) spec.functions.emplace_back(starts[i], i + 1 < std::size(starts) ? starts[i + 1] : kEnd);
        spec.function_names = {{kHandler, "OnFoo"}, {kCatch, "CatchAll"}, {kGetMap, "GetMessageMap"}};
        spec.roots = {kCtorFoo};
        return Image(std::move(spec));
    }
    cl::Inputs inputs() { return {image, graph, facts}; }
};

const cl::Vtable* table_at(const std::vector<cl::Vtable>& v, Va at) {
    for (const cl::Vtable& t : v)
        if (t.at == at) return &t;
    return nullptr;
}

}  // namespace

CB_TEST("analyze.classes.virtual_tables", [] {
    Program p;
    const std::vector<cl::Vtable> tables = cl::find_vtables(p.inputs());
    CB_CHECK_EQ(tables.size(), std::size_t(3));
    const cl::Vtable* foo = table_at(tables, kTableFoo);
    CB_CHECK(foo != nullptr);
    CB_CHECK((foo->slots == std::vector<Va>{kGetMap, kC, kD}));
    CB_CHECK((foo->installed_by == std::vector<Va>{kCtorFoo}));
    CB_CHECK((foo->at_offsets == std::vector<std::int64_t>{0}));
    CB_CHECK(foo->com && !foo->dispatch);   // removes 12, 4, 4
    const cl::Vtable* mfc = table_at(tables, kTableMfc);
    CB_CHECK(mfc != nullptr && !mfc->com);
    CB_CHECK((mfc->slots == std::vector<Va>{kGetRecord, kB, kC}));
    const cl::Vtable* third = table_at(tables, kTableDerived);
    CB_CHECK(third != nullptr && third->slots.size() == 4);
    CB_CHECK(tables[0].at < tables[1].at && tables[1].at < tables[2].at);   // by address
});

CB_TEST("analyze.classes.a_16_bit_module_has_no_classes", [] {
    Program p(16);
    CB_CHECK(cl::find_vtables(p.inputs()).empty());
    CB_CHECK(cl::find_runtime_classes(p.image).empty());
    CB_CHECK(cl::find_message_maps(p.image, {}).empty());
    CB_CHECK(cl::find_exception_tables(p.image, p.graph).cxx.empty());
});

CB_TEST("analyze.classes.decorated_names", [] {
    CB_CHECK_EQ(cl::decorated_name(".?AVFoo@@").value_or("none"), "Foo");
    CB_CHECK_EQ(cl::decorated_name(".?AVInner@Outer@@").value_or("none"), "Outer::Inner");   // reversed
    CB_CHECK_EQ(cl::decorated_name(".?AUPoint@@").value_or("none"), "Point");
    CB_CHECK(!cl::decorated_name("CFoo").has_value());
    CB_CHECK(!cl::decorated_name(".?AV").has_value());     // too short
    CB_CHECK_EQ(cl::decorated_name(".?AVa b@@").value_or("none"), "a b@@");   // not a name: the rest as it is
});

CB_TEST("analyze.classes.run_time_type_information", [] {
    Program p;
    CB_CHECK_EQ(cl::type_name(p.image, kTypeFoo).value_or("none"), "Foo");
    CB_CHECK(!cl::type_name(p.image, 0).has_value());
    CB_CHECK(!cl::type_name(p.image, kDataEnd + 0x1000).has_value());
    const auto info = cl::rtti(p.image, kTableFoo);
    CB_CHECK(info.has_value());
    CB_CHECK_EQ(info->name, "Foo");
    CB_CHECK((info->bases == std::vector<std::string>{"Bar"}));
    CB_CHECK(!cl::rtti(p.image, kTableMfc).has_value());   // nothing before it
});

CB_TEST("analyze.classes.mfc_records_and_getters", [] {
    Program p;
    const std::map<Va, Va> getters = cl::returns_address(p.image);
    CB_CHECK_EQ(getters.at(kGetMap), kMapRecord);
    CB_CHECK_EQ(getters.at(kGetRecord), kRecordDerived);
    CB_CHECK(!getters.contains(kB));
    CB_CHECK_EQ(cl::returned_address(p.image, kGetMap).value_or(0), kMapRecord);
    const std::vector<cl::RuntimeClass> records = cl::find_runtime_classes(p.image);
    CB_CHECK_EQ(records.size(), std::size_t(2));   // CBase is in because CDerived has it as its base
    CB_CHECK_EQ(records[0].name, "CBase");
    CB_CHECK_EQ(records[0].size, 0x20);
    CB_CHECK(!records[0].schema.has_value() && !records[0].creatable && !records[0].base.has_value());
    CB_CHECK_EQ(records[1].name, "CDerived");
    CB_CHECK_EQ(records[1].at, kRecordDerived);
    CB_CHECK(records[1].creatable);
    CB_CHECK_EQ(records[1].base.value_or("none"), "CBase");
    CB_CHECK_EQ(cl::class_of_import("?classCWnd@CWnd@@2UCRuntimeClass@@B"), "CWnd");
    CB_CHECK_EQ(cl::class_of_import("MFC42!classCView"), "classCView");
});

CB_TEST("analyze.classes.message_maps", [] {
    Program p;
    const std::vector<cl::Vtable> tables = cl::find_vtables(p.inputs());
    const std::vector<cl::MessageMap> maps = cl::find_message_maps(p.image, tables);
    CB_CHECK_EQ(maps.size(), std::size_t(1));
    const cl::MessageMap& m = maps[0];
    CB_CHECK_EQ(m.at, kMapEntries);
    CB_CHECK_EQ(m.entries.size(), std::size_t(3));
    CB_CHECK_EQ(m.map.value_or(0), kMapRecord);
    CB_CHECK((m.class_tables == std::vector<Va>{kTableFoo, kTableDerived}));   // both tables' first function returns the record
    CB_CHECK(!m.base_map.has_value());
    CB_CHECK_EQ(cl::macro_of(p.image, m.entries[0]), "ON_COMMAND(100, OnFoo)");
    CB_CHECK_EQ(cl::macro_of(p.image, m.entries[1]), "ON_COMMAND_RANGE(101, 110, OnFoo)");
    CB_CHECK_EQ(cl::macro_of(p.image, m.entries[2]), "ON_MESSAGE(WM_PAINT, OnFoo)");
    // the other macros an entry can be
    const cl::MapEntry update{0x111, 0xFFFFFFFFLL, 7, 7, 5, kHandler}, control{0x111, 0x300, 7, 7, 5, kHandler}, notify{0x4E, 0xFFFFFF9CLL, 9, 9, 5, kHandler};
    CB_CHECK_EQ(cl::macro_of(p.image, update), "ON_UPDATE_COMMAND_UI(7, OnFoo)");
    CB_CHECK_EQ(cl::macro_of(p.image, control), "ON_CONTROL(0x300, 7, OnFoo)");
    CB_CHECK_EQ(cl::macro_of(p.image, notify), "ON_NOTIFY(-100, 9, OnFoo)");   // the code is signed
    CB_CHECK_EQ(cl::macro_of(p.image, {0xC000, 0, 3, 3, 0x402000, kHandler}), "ON_REGISTERED_MESSAGE(*(UINT *)0x402000, OnFoo)");
    CB_CHECK_EQ(cl::macro_of(p.image, {0x0F, 0, 0, 0, 5, kHandler}), "ON_WM_PAINT() -> OnFoo");   // (a signature that is not a plain handler's)
    CB_CHECK_EQ(cl::macro_of(p.image, {0x7FFF, 0, 0, 0, 12, kHandler}), "ON_MESSAGE(0x7fff, OnFoo)");
    CB_CHECK_EQ(cl::id_range(5, 5), "5");
    CB_CHECK_EQ(cl::id_range(5, 9), "5, 9");
});

CB_TEST("analyze.classes.hierarchy_names_bases_and_evidence", [] {
    Program p;
    const cl::Model model = cl::build_model(p.inputs());
    // a recovered structure whose first field holds Foo's table: its layout is Foo's
    structures::Structure s;
    s.name = "struct_1";
    s.size = 24;
    s.exact = true;
    s.fields[0].stores.insert(kTableFoo);
    s.fields[4];
    const std::vector<structures::Structure> recovered = {s};
    const std::vector<cl::Class> tree = cl::hierarchy(p.image, model, recovered);
    CB_CHECK_EQ(tree.size(), std::size_t(3));
    // by table address
    CB_CHECK_EQ(tree[0].vtable, kTableFoo);
    CB_CHECK_EQ(tree[0].name, "Foo");
    CB_CHECK((tree[0].bases == std::vector<std::string>{"Bar"}));
    CB_CHECK((tree[0].evidence == std::vector<std::string>{"run-time type information"}));
    CB_CHECK_EQ(tree[0].slots, 3);
    CB_CHECK(tree[0].com);
    CB_CHECK_EQ(tree[0].layout.value_or("none"), "struct_1");
    CB_CHECK_EQ(tree[0].fields.value_or(0), 2);
    CB_CHECK_EQ(tree[0].size.value_or(0), 24);
    CB_CHECK(tree[0].size_exact.value_or(false));
    CB_CHECK_EQ(tree[0].message_map.value_or(0), kMapEntries);
    CB_CHECK((tree[0].installed_by == std::vector<Va>{kCtorFoo}));
    // named by MFC's record: its size and its chain of bases
    CB_CHECK_EQ(tree[1].vtable, kTableMfc);
    CB_CHECK_EQ(tree[1].name, "CDerived");
    CB_CHECK_EQ(tree[1].size.value_or(0), 0x40);
    CB_CHECK((tree[1].bases == std::vector<std::string>{"CBase"}));
    CB_CHECK_EQ(tree[1].evidence[0], "MFC class record at 403614");
    // it states nothing: Foo is its base because it shares Foo's slots
    CB_CHECK_EQ(tree[2].vtable, kTableDerived);
    CB_CHECK_EQ(tree[2].name, "class_403030");
    CB_CHECK((tree[2].bases == std::vector<std::string>{"Foo"}));
    CB_CHECK_EQ(tree[2].evidence[0], "probably: 3 of its first 3 virtual functions are those of the table at 403010");
    // no tables, no classes
    CB_CHECK(cl::hierarchy(p.image, cl::Model{}, {}).empty());
});

CB_TEST("analyze.classes.exception_tables", [] {
    Program p;
    const std::vector<cl::CxxTable> tables = cl::find_cxx_tables(p.image);
    CB_CHECK_EQ(tables.size(), std::size_t(1));
    CB_CHECK_EQ(tables[0].at, kEh);
    CB_CHECK_EQ(tables[0].states, 1);
    CB_CHECK_EQ(tables[0].cleanups, 1);
    CB_CHECK_EQ(tables[0].tries.size(), std::size_t(1));
    CB_CHECK(tables[0].tries[0].low == 0 && tables[0].tries[0].high == 0);
    CB_CHECK_EQ(tables[0].tries[0].catches.size(), std::size_t(1));
    CB_CHECK_EQ(tables[0].tries[0].catches[0].type, "...");   // no type: catch (...)
    CB_CHECK_EQ(tables[0].tries[0].catches[0].handler, kCatch);
    const std::vector<cl::Scope> scopes = cl::read_scope_table(p.image, kScopes);
    CB_CHECK_EQ(scopes.size(), std::size_t(2));
    CB_CHECK(!scopes[0].enclosing.has_value() && !scopes[0].filter.has_value() && scopes[0].handler == kCatch);   // a __finally
    CB_CHECK(scopes[1].enclosing.value_or(-1) == 0 && scopes[1].filter.value_or(0) == kHandler);                  // an __except inside it
    CB_CHECK(cl::read_scope_table(p.image, kData).empty());   // nothing that is code there
    const cl::ExceptionTables all = cl::find_exception_tables(p.image, p.graph);
    CB_CHECK_EQ(all.cxx.size(), std::size_t(1));
    CB_CHECK(all.frames.empty());   // (no function sets a frame up)
});

CB_TEST("analyze.classes.the_report", [] {
    Program p;
    const cl::Analysis a = cl::analyse(p.inputs(), {});
    CB_CHECK_EQ(a.counts.at("virtual_tables"), 3);
    CB_CHECK_EQ(a.counts.at("virtual_functions"), 10);
    CB_CHECK_EQ(a.counts.at("com_shaped_tables"), 2);   // (Foo, and the table that begins as Foo's does)
    CB_CHECK_EQ(a.counts.at("constructors_or_destructors"), 3);
    CB_CHECK_EQ(a.counts.at("classes_named_by_the_binary"), 2);
    CB_CHECK_EQ(a.counts.at("classes_with_stated_bases"), 2);
    CB_CHECK_EQ(a.counts.at("classes_with_inferred_base"), 1);
    CB_CHECK_EQ(a.counts.at("mfc_class_records"), 2);
    CB_CHECK_EQ(a.counts.at("message_maps"), 1);
    CB_CHECK_EQ(a.counts.at("message_map_entries"), 3);
    CB_CHECK_EQ(a.counts.at("message_maps_tied_to_a_class"), 1);
    CB_CHECK_EQ(a.counts.at("cxx_exception_tables"), 1);
    CB_CHECK_EQ(a.counts.at("try_blocks"), 1);
    CB_CHECK_EQ(a.counts.at("catch_handlers"), 1);
    CB_CHECK_EQ(a.counts.at("exception_frames"), 0);
    // the counts come in the order the report prints them
    CB_CHECK_EQ(a.counts.begin()->first, "virtual_tables");
    // facts: three classes, two MFC records, one message map, one try table
    CB_CHECK_EQ(a.facts.size(), std::size_t(7));
    const auto& foo = std::get<cl::ClassFact>(a.facts[0]);
    CB_CHECK_EQ(foo.name, "Foo");
    CB_CHECK(foo.com_interface);
    CB_CHECK_EQ(foo.methods[0], "GetMessageMap");
    CB_CHECK_EQ(foo.constructors_or_destructors.size(), std::size_t(1));
    const auto& map = std::get<cl::MessageMapFact>(a.facts[5]);
    CB_CHECK_EQ(map.class_name.value_or("none"), "Foo");   // the first class whose table is one of the map's
    CB_CHECK_EQ(map.entries.size(), std::size_t(3));
    CB_CHECK_EQ(map.entries[0], "ON_COMMAND(100, OnFoo)");
    const auto& eh = std::get<cl::CxxTableFact>(a.facts[6]);
    CB_CHECK_EQ(eh.try_blocks[0].states, "0..0");
    CB_CHECK_EQ(eh.try_blocks[0].catches[0], "catch (...) at 401060");
    // the text lines: a class per line, then the first message maps with their macros
    CB_CHECK_EQ(a.text[0], "class Foo : Bar  table 403010, 3 virtual, size unknown  [run-time type information]");
    CB_CHECK_EQ(a.text[1], "class CDerived : CBase  table 403020, 3 virtual, 64 bytes  [MFC class record at 403614]");
    CB_CHECK_EQ(a.text[3], "message map 403400 of Foo:");
    CB_CHECK_EQ(a.text[4], "    ON_COMMAND(100, OnFoo)");
});
