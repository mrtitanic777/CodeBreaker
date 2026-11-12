// Tests of cb/analyze/structures.hpp over small 32-bit programs written byte by byte: an allocation with a constant size
// (an exact structure), an argument handed on to another function (two pointers, one structure), a pointer field, an
// indexed array, a global pointer, `this` in ecx, and the C text and the report made from them.
#include "cb/analyze/structures.hpp"

#include "cb/analyze/test_image.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::analyze;
namespace st = cb::analyze::structures;

namespace {

constexpr Va kCode = 0x401000;
constexpr Va kSlot = 0x402100;     // the import address of LocalAlloc

// A flat 32-bit program: the functions are `starts` (each running to the next), the bytes are one code span.
struct Program {
    Image image;
    call_graph::Graph graph;
    ConventionFacts facts;

    Program(const Bytes& bytes, std::vector<Va> starts, const std::map<Va, ConventionFact>& given = {}, Va data_at = 0, Bytes data = {})
        : image(make(bytes, starts, data_at, data)), graph(call_graph::build(image)) {
        for (Va f : image.functions()) facts[f] = given.contains(f) ? given.at(f) : ConventionFact{"cdecl", std::nullopt, 0, false, {}};
    }

    static Image make(const Bytes& bytes, const std::vector<Va>& starts, Va data_at, const Bytes& data) {
        ImageSpec spec;
        spec.kind = ImageKind::Pe;
        spec.bits = 32;
        spec.flat = true;
        Span code;
        code.va = kCode;
        code.data = bytes;
        code.executable = true;
        code.initialised = static_cast<Va>(bytes.size());
        code.name = ".text";
        spec.spans = {code};
        if (!data.empty()) {
            Span d;
            d.va = data_at;
            d.data = data;
            d.initialised = static_cast<Va>(data.size());
            d.writable = true;
            d.name = ".data";
            spec.spans.push_back(d);
        }
        for (std::size_t i = 0; i < starts.size(); ++i) {
            spec.functions.emplace_back(starts[i], i + 1 < starts.size() ? starts[i + 1] : kCode + static_cast<Va>(bytes.size()));
        }
        spec.imports = {{kSlot, "KERNEL32!LocalAlloc"}};
        spec.roots = {starts.front()};
        return Image(std::move(spec));
    }

    std::vector<st::Structure> recover() { return st::recover(image, graph, facts); }
};

}  // namespace

CB_TEST("analyze.structures.an_allocation_of_known_size_is_an_exact_structure", [] {
    test::Asm a(kCode);
    a.raw({0x6A, 0x20});                                     // push 20h        (the size)
    a.raw({0x6A, 0x00});                                     // push 0          (the flags)
    a.call_indirect(kSlot);                                  // call [LocalAlloc]
    a.raw({0x8B, 0xF0});                                     // mov esi,eax
    a.raw({0xC7, 0x06, 0x01, 0x00, 0x00, 0x00});             // mov dword [esi],1
    a.raw({0xC7, 0x46, 0x04, 0x02, 0x00, 0x00, 0x00});       // mov dword [esi+4],2
    a.raw({0x8B, 0x46, 0x08});                               // mov eax,[esi+8]
    a.ret();
    Program p(a.bytes(), {kCode});
    const std::vector<st::Structure> found = p.recover();
    CB_CHECK_EQ(found.size(), std::size_t{1});
    const st::Structure& s = found[0];
    CB_CHECK(s.name == "struct_1" && s.exact && s.size == 0x20);
    CB_CHECK_EQ(s.roots.size(), std::size_t{1});
    CB_CHECK(s.roots[0].kind == st::Root::Kind::Alloc && s.roots[0].at == kCode + 4);   // the call's address
    CB_CHECK_EQ(s.fields.size(), std::size_t{3});
    const st::Field& f0 = s.fields.at(0);
    CB_CHECK(f0.size == 4 && f0.writes == 1 && f0.reads == 0 && f0.stores == std::set<Va>{1} && f0.users == std::set<Va>{kCode});
    CB_CHECK(s.fields.at(4).stores == std::set<Va>{2});
    CB_CHECK(s.fields.at(8).reads == 1 && s.fields.at(8).writes == 0);
    CB_CHECK(s.functions == std::vector<Va>{kCode});
    const std::string c = st::layout(s);
    CB_CHECK(c.starts_with("struct struct_1 {    /* 32 bytes */\n"));
    CB_CHECK(c.find("unsigned long f_0;") != std::string::npos && c.find("/* +4  read 0, written 1 */") != std::string::npos);
    CB_CHECK(c.find("/* +8  read 1, written 0 */") != std::string::npos && c.ends_with("};"));
});

CB_TEST("analyze.structures.a_pointer_handed_on_is_one_structure", [] {
    // callee(p): ecx = p->4; edx = p->8
    test::Asm a(kCode);
    const Va callee = a.here();
    a.raw({0x8B, 0x44, 0x24, 0x04});                         // mov eax,[esp+4]
    a.raw({0x8B, 0x48, 0x04});                               // mov ecx,[eax+4]
    a.raw({0x8B, 0x50, 0x08});                               // mov edx,[eax+8]
    a.ret().pad_to(kCode + 0x20);
    // caller(q): ebx = q->0; callee(q)
    const Va caller = a.here();
    a.raw({0x8B, 0x44, 0x24, 0x04});                         // mov eax,[esp+4]
    a.raw({0x8B, 0x18});                                     // mov ebx,[eax]
    a.raw({0x50});                                           // push eax
    a.call(callee);
    a.raw({0x83, 0xC4, 0x04});                               // add esp,4
    a.ret();
    Program p(a.bytes(), {callee, caller});
    const std::vector<st::Structure> found = p.recover();
    CB_CHECK_EQ(found.size(), std::size_t{1});
    const st::Structure& s = found[0];
    CB_CHECK_EQ(s.fields.size(), std::size_t{3});            // 0 from the caller, 4 and 8 from the callee
    CB_CHECK(s.fields.contains(0) && s.fields.contains(4) && s.fields.contains(8));
    CB_CHECK(!s.exact && s.size == 12);                      // a lower bound: the end of the last field
    CB_CHECK_EQ(s.roots.size(), std::size_t{2});
    CB_CHECK(s.roots[0].kind == st::Root::Kind::Arg && s.roots[0].at == callee && s.roots[0].index == 0);
    CB_CHECK(s.roots[1].kind == st::Root::Kind::Arg && s.roots[1].at == caller && s.roots[1].index == 0);
    CB_CHECK(s.functions == (std::vector<Va>{callee, caller}));
    CB_CHECK(st::describe(p.image, s.roots[0]).starts_with("argument 0 of "));
});

CB_TEST("analyze.structures.one_field_is_not_a_structure_but_one_field_of_known_size_is", [] {
    test::Asm a(kCode);
    a.raw({0x8B, 0x44, 0x24, 0x04, 0x8B, 0x48, 0x04});      // mov eax,[esp+4] / mov ecx,[eax+4]
    a.ret();
    Program lone(a.bytes(), {kCode});
    CB_CHECK(lone.recover().empty());
    test::Asm b(kCode);
    b.raw({0x6A, 0x10, 0x6A, 0x00});
    b.call_indirect(kSlot);
    b.raw({0x8B, 0x48, 0x04});                               // mov ecx,[eax+4]
    b.ret();
    Program sized(b.bytes(), {kCode});
    const auto found = sized.recover();
    CB_CHECK_EQ(found.size(), std::size_t{1});
    CB_CHECK(found[0].exact && found[0].size == 0x10 && found[0].fields.size() == 1);
});

CB_TEST("analyze.structures.a_pointer_field_an_array_a_float_and_a_called_field", [] {
    test::Asm a(kCode);
    a.raw({0x8B, 0x44, 0x24, 0x04});                         // mov eax,[esp+4]
    a.raw({0x8B, 0x48, 0x04});                               // mov ecx,[eax+4]       (a field ...)
    a.raw({0x8B, 0x51, 0x08});                               // mov edx,[ecx+8]       (... used as an address: a pointer)
    a.raw({0x8B, 0x5C, 0x98, 0x10});                         // mov ebx,[eax+ebx*4+10h]  (an array of dwords)
    a.raw({0xD9, 0x40, 0x20});                               // fld dword [eax+20h]   (a float)
    a.raw({0xFF, 0x50, 0x30});                               // call [eax+30h]        (a function pointer)
    a.ret();
    Program p(a.bytes(), {kCode});
    const auto found = p.recover();
    CB_CHECK_EQ(found.size(), std::size_t{1});
    const st::Structure& s = found[0];
    CB_CHECK(s.fields.at(4).notes == std::set<st::Note>{st::Note::Pointer});
    CB_CHECK(s.fields.at(0x10).notes.contains(st::Note::Array) && s.fields.at(0x10).element == 4);
    CB_CHECK(s.fields.at(0x20).notes == std::set<st::Note>{st::Note::Float});
    CB_CHECK(s.fields.at(0x30).notes == std::set<st::Note>{st::Note::Called});
    CB_CHECK_EQ(st::field_type(s.fields.at(4)), "pointer");
    CB_CHECK_EQ(st::field_type(s.fields.at(0x10)), "unsigned long[]");
    CB_CHECK_EQ(st::field_type(s.fields.at(0x20)), "float");
    CB_CHECK_EQ(st::field_type(s.fields.at(0x30)), "function pointer");
    const std::string c = st::layout(s);
    CB_CHECK(c.find("void *f_4;") != std::string::npos && c.find("unsigned long f_10[1];") != std::string::npos);
    CB_CHECK(c.find("float f_20;") != std::string::npos && c.find("void (*f_30)();") != std::string::npos);
    CB_CHECK(c.find("unsigned char gap_8[8];") != std::string::npos);     // 8..0Fh: nothing known there
    CB_CHECK(c.find(", array */") != std::string::npos);
});

CB_TEST("analyze.structures.a_table_of_functions_stored_in_the_first_field_is_what_classes_look_for", [] {
    test::Asm a(kCode);
    a.raw({0x8B, 0x44, 0x24, 0x04});                         // mov eax,[esp+4]
    a.raw({0xC7, 0x00, 0x00, 0x22, 0x40, 0x00});             // mov dword [eax],402200h   (the vtable)
    a.raw({0xC7, 0x40, 0x04, 0x00, 0x00, 0x00, 0x00});       // mov dword [eax+4],0
    a.ret();
    Program p(a.bytes(), {kCode});
    const auto found = p.recover();
    CB_CHECK_EQ(found.size(), std::size_t{1});
    CB_CHECK(found[0].fields.at(0).stores == std::set<Va>{0x402200});
});

CB_TEST("analyze.structures.a_global_pointer_and_this_are_followed", [] {
    // a pointer kept in the global at 403000h: f() loads it and reads two fields; g(this) in ecx reads two
    test::Asm a(kCode);
    const Va f = a.here();
    a.raw({0x8B, 0x0D, 0x00, 0x30, 0x40, 0x00});             // mov ecx,[403000h]
    a.raw({0x8B, 0x01, 0x8B, 0x51, 0x04});                   // mov eax,[ecx] / mov edx,[ecx+4]
    a.ret().pad_to(kCode + 0x20);
    const Va g = a.here();
    a.raw({0x8B, 0x01, 0x8B, 0x51, 0x04});                   // mov eax,[ecx] / mov edx,[ecx+4]   (ecx read before written)
    a.ret();
    Bytes data(0x20, 0);
    std::map<Va, ConventionFact> facts;
    ConventionFact thiscall{"thiscall", std::nullopt, 0, false, {"ecx"}};
    facts[g] = thiscall;
    Program p(a.bytes(), {f, g}, facts, 0x403000, data);
    const auto found = p.recover();
    CB_CHECK_EQ(found.size(), std::size_t{2});
    bool global = false, this_ = false;
    for (const st::Structure& s : found) {
        for (const st::Root& r : s.roots) {
            global = global || (r.kind == st::Root::Kind::Global && r.at == 0x403000);
            this_ = this_ || (r.kind == st::Root::Kind::This && r.at == g);
        }
    }
    CB_CHECK(global && this_);
    CB_CHECK(st::describe(p.image, found[0].roots[0]).size() > 0);
});

CB_TEST("analyze.structures.the_report_counts_what_was_found", [] {
    test::Asm a(kCode);
    a.raw({0x6A, 0x20, 0x6A, 0x00});
    a.call_indirect(kSlot);
    a.raw({0x8B, 0xF0, 0xC7, 0x06, 0x01, 0x00, 0x00, 0x00, 0xC7, 0x46, 0x04, 0x02, 0x00, 0x00, 0x00, 0x8B, 0x46, 0x08});
    a.ret();
    Program p(a.bytes(), {kCode});
    const st::Report r = st::analyse(p.image, p.graph, p.facts);
    CB_CHECK_EQ(r.counts.structures, 1);
    CB_CHECK_EQ(r.counts.fields, 3);
    CB_CHECK_EQ(r.counts.with_exact_size, 1);
    CB_CHECK_EQ(r.counts.largest_fields, 3);
    CB_CHECK_EQ(r.counts.functions_touching_a_structure, 1);
    CB_CHECK_EQ(r.counts.unified_across_functions, 0);
    CB_CHECK_EQ(r.facts.size(), std::size_t{1});
    CB_CHECK(r.facts[0].name == "struct_1" && r.facts[0].size == 0x20 && r.facts[0].exact_size && r.facts[0].fields.size() == 3);
    CB_CHECK(r.facts[0].fields[0].offset == 0 && r.facts[0].fields[0].type == "unsigned long" && r.facts[0].fields[0].writes == 1);
    CB_CHECK(r.facts[0].seen_as[0].starts_with("allocated at "));
    CB_CHECK(r.text[0].starts_with("struct_1: 3 fields, 32 bytes, used by 1 functions, seen as allocated at "));
    CB_CHECK(r.text[1] == "struct struct_1 {    /* 32 bytes */" && r.text.back() == "};");
});

CB_TEST("analyze.structures.a_module_with_no_pointers_has_no_structures", [] {
    test::Sample s;
    Image image((ImageSpec(s.spec())));
    call_graph::Graph graph = call_graph::build(image);
    ConventionFacts facts;
    for (Va f : image.functions()) facts[f] = ConventionFact{"cdecl", std::nullopt, 0, false, {}};
    CB_CHECK(st::recover(image, graph, facts).empty());
    const st::Report r = st::analyse(image, graph, facts);
    CB_CHECK(r.counts.structures == 0 && r.facts.empty() && r.text.empty());
});

CB_TEST("analyze.structures.roots_are_ordered_by_kind_then_where", [] {
    st::Root alloc, arg0, arg1, field, global, result, returns, self;
    alloc.kind = st::Root::Kind::Alloc;
    arg0.kind = arg1.kind = st::Root::Kind::Arg;
    arg0.at = arg1.at = 0x401000;
    arg1.index = 1;
    global.kind = st::Root::Kind::Global;
    result.kind = st::Root::Kind::Result;
    returns.kind = st::Root::Kind::Returns;
    self.kind = st::Root::Kind::This;
    field.kind = st::Root::Kind::Field;
    field.of = std::make_shared<const st::Root>(arg0);
    field.index = 8;
    CB_CHECK(alloc < arg0 && arg0 < arg1 && arg1 < field && field < global && global < result && result < returns && returns < self);
    st::Root other = field;
    other.of = std::make_shared<const st::Root>(arg1);
    CB_CHECK(field != other && field < other);                // the same field of different pointers
    CB_CHECK(field == st::Root(field));
    CB_CHECK_EQ(std::string(st::name_of(st::Note::AddressTaken)), "address taken");
});
