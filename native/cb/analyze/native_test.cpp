#include "cb/analyze/native.hpp"

#include "cb/analyze/test_image.hpp"
#include "cb/core/testing.hpp"

namespace {

using namespace cb::analyze;
using cb::analyze::test::Asm;
using cb::analyze::test::Sample;

std::vector<std::string> mnemonics(const Function& fn) {
    std::vector<std::string> out;
    for (const Instruction& i : fn.insns) out.push_back(i.mnemonic);
    return out;
}

CB_TEST("analyze.native.decodes_a_function_from_its_start", [] {
    const Sample s;
    Image image(s.spec());
    const Function& a = image.function(s.A);
    CB_CHECK((mnemonics(a) == std::vector<std::string>{"push", "mov", "push", "push", "call", "add", "pop", "ret"}));
    CB_CHECK_EQ(a.start, s.A);
    CB_CHECK_EQ(a.limit, s.B);
    CB_CHECK_EQ(a.end, s.A + 0x14);          // the end of the last instruction reached, not the limit
    const Instruction& call = a.insns[4];
    CB_CHECK(call.flow == Flow::Call);
    CB_CHECK(call.target.has_value() && *call.target == s.B);
    CB_CHECK_EQ(call.size, 5);
    CB_CHECK(a.insns[7].flow == Flow::Return);
    CB_CHECK(a.tables.empty());
    // operands: a register, an immediate (masked to its size), a memory reference
    const Instruction& mov = a.insns[1];
    CB_CHECK_EQ(mov.text, std::string("mov ebp, esp"));
    CB_CHECK_EQ(mov.operands.size(), std::size_t{2});
    CB_CHECK(mov.operands[0].kind == Operand::Kind::Register && mov.operands[0].reg == "ebp");
    const Instruction& push_imm = a.insns[3];
    CB_CHECK(push_imm.operands[0].kind == Operand::Kind::Immediate);
    CB_CHECK_EQ(push_imm.operands[0].value, s.hello);
    CB_CHECK_EQ(push_imm.operands[0].size, 4);
    // the registers an instruction reads and writes are whole registers
    CB_CHECK(a.insns[1].writes.contains("ebp") && a.insns[1].reads.contains("esp"));
    CB_CHECK(a.insns[0].reads.contains("ebp") && a.insns[0].writes.contains("esp"));
    // decoded once: asking again gives the same object
    CB_CHECK(&image.function(s.A) == &a);
});

CB_TEST("analyze.native.a_function_is_not_followed_past_its_limit", [] {
    const Sample s;
    ImageSpec spec = s.spec();
    spec.functions[0].second = s.A + 0x0A;   // A may only run up to its call
    Image image(std::move(spec));
    const Function& a = image.function(s.A);
    CB_CHECK_EQ(a.insns.size(), std::size_t{4});     // push ebp, mov, push 5, push hello
    CB_CHECK_EQ(a.end, s.A + 0x0A);
    CB_CHECK_EQ(a.limit, s.A + 0x0A);
});

CB_TEST("analyze.native.recognises_a_jump_table", [] {
    const Sample s;
    Image image(s.spec());
    const Function& c = image.function(s.C);
    CB_CHECK_EQ(c.tables.size(), std::size_t{1});
    const JumpTable& t = c.tables[0];
    CB_CHECK_EQ(t.table, s.jump_table);
    CB_CHECK_EQ(t.low, 0);
    CB_CHECK(t.bounded);
    CB_CHECK_EQ(t.selector, std::string("eax"));
    CB_CHECK((t.targets == std::vector<Va>{s.case_address[0], s.case_address[1], s.case_address[2]}));
    CB_CHECK(t.default_target.has_value() && *t.default_target == s.C + 0x22);   // `ja` past the last case
    CB_CHECK(!t.index_bytes.has_value());
    // every case was reached: the cases' instructions are decoded, the table's bytes are not
    CB_CHECK(c.insns.size() > 10);
    CB_CHECK(image.covering(s.case_address[2]) != nullptr);
    CB_CHECK(image.covering(s.jump_table) == nullptr);
});

CB_TEST("analyze.native.a_jump_table_that_runs_out_of_the_function_is_not_one", [] {
    const Sample s;
    ImageSpec spec = s.spec();
    // C's limit cuts the cases off: the table's entries are not inside the function any more
    for (auto& f : spec.functions)
        if (f.first == s.C) f.second = s.C + 0x10;
    Image image(std::move(spec));
    CB_CHECK(image.function(s.C).tables.empty());
});

CB_TEST("analyze.native.discovers_functions_the_code_calls", [] {
    const Sample s;
    ImageSpec spec = s.spec();
    spec.functions = {{s.A, s.kCodeEnd}};   // only A is known; B, D, ... are not
    spec.function_names[s.A] = "start";
    Image image(std::move(spec));
    CB_CHECK_EQ(image.functions().size(), std::size_t{1});
    const std::vector<Va> found = image.discover();
    // A calls B; B calls nothing; nothing else is called from what is known
    CB_CHECK((found == std::vector<Va>{s.B}));
    CB_CHECK((image.functions() == std::vector<Va>{s.A, s.B}));
    CB_CHECK_EQ(image.function_limit(s.A), s.B);             // A now ends where B starts
    CB_CHECK_EQ(image.function_limit(s.B), s.kCodeEnd);      // B runs to the end of the span
    CB_CHECK(image.was_discovered(s.B) && !image.was_discovered(s.A));
    CB_CHECK((image.discovered() == std::vector<Va>{s.B}));
    CB_CHECK(image.is_function(s.B));
    // A was decoded again against its new limit
    CB_CHECK_EQ(image.function(s.A).limit, s.B);
    // a second round finds nothing
    CB_CHECK(image.discover().empty());
});

CB_TEST("analyze.native.discovers_code_whose_address_the_file_marks", [] {
    const Sample s;
    ImageSpec spec = s.spec();
    spec.functions = {{s.A, s.kCodeEnd}};
    spec.has_pointers = true;
    // The loader writes the address of P into the data (a callback table), and also the address of a spot in the
    // middle of A, of padding, of J through a pointer that is not an abs32 address, and a pointer with no target.
    // (I and J are not offered: they begin with printable bytes, which a function never does.)
    spec.pointers = {{s.kData + 0x300, Pointer{PointerKind::Abs32, s.P}},
                     {s.kData + 0x308, Pointer{PointerKind::Abs32, s.A + 4}},        // inside A: a label, not a function
                     {s.kData + 0x310, Pointer{PointerKind::Abs32, s.A + 0x18}},    // padding: 0xCC 0xCC
                     {s.kData + 0x318, Pointer{PointerKind::Far, s.G}},              // not an abs32 address
                     {s.kData + 0x320, Pointer{PointerKind::Abs32, std::nullopt}}};
    Image image(std::move(spec));
    const std::vector<Va> found = image.discover();
    CB_CHECK((found == std::vector<Va>{s.B, s.P}));   // B by A's call, then P by the table
    CB_CHECK(image.is_function(s.P));
    CB_CHECK(!image.is_function(s.G));
    CB_CHECK(!image.is_function(s.A + 4));
    CB_CHECK(!image.is_function(s.A + 0x18));
    CB_CHECK(image.pointer_at(s.kData + 0x300) != nullptr && image.pointer_at(s.kData + 0x301) == nullptr);
});

CB_TEST("analyze.native.linear_reads_straight_on_without_following_branches", [] {
    const Sample s;
    Image image(s.spec());
    const Function three = image.linear(s.A, 3);
    CB_CHECK_EQ(three.insns.size(), std::size_t{3});
    CB_CHECK_EQ(three.insns[2].addr, s.A + 3);
    CB_CHECK(image.linear(s.A, 0).insns.empty());
    CB_CHECK(image.linear(0x900000, 5).insns.empty());          // nothing there
    // straight on through a return: no flow is followed
    CB_CHECK(image.linear(s.A + 0x13, 3).insns.size() == 3);
});

CB_TEST("analyze.native.reads_the_address_space", [] {
    const Sample s;
    Image image(s.spec());
    CB_CHECK(image.span(s.A) != nullptr && image.span(s.A)->name == ".text");
    CB_CHECK(image.span(s.hello) != nullptr && image.span(s.hello)->name == ".data");
    CB_CHECK(image.span(s.kCodeEnd) == nullptr);                 // the gap between the spans
    CB_CHECK(image.span(0) == nullptr);
    CB_CHECK(image.span(-5) == nullptr);
    CB_CHECK(image.span(s.kDataEnd) == nullptr);                 // one past the end
    CB_CHECK(image.span(s.kDataEnd - 1) != nullptr);
    CB_CHECK((image.read(s.hello, 5) == cb::to_bytes("Hello")));
    CB_CHECK_EQ(image.read(s.kDataEnd - 2, 10).size(), std::size_t{2});     // clipped at the end of the span
    CB_CHECK(image.read(s.kDataEnd, 4).empty());
    CB_CHECK(image.read(s.hello, 0).empty());
    CB_CHECK(image.read(s.hello, -3).empty());
    CB_CHECK_EQ(*image.u16(s.hello), std::uint32_t{0x6548});                // "He"
    CB_CHECK_EQ(*image.u32(s.hello), std::uint32_t{0x6C6C6548});
    CB_CHECK(!image.u32(s.kDataEnd - 3).has_value());
    CB_CHECK(!image.u16(s.kDataEnd - 1).has_value());
    CB_CHECK(image.is_code(s.A) && !image.is_code(s.hello) && !image.is_code(0));
    CB_CHECK(image.is_data(s.hello) && image.is_data(s.A));      // in the module
    CB_CHECK(!image.is_data(s.exit_slot));                       // an import's slot is not data
    CB_CHECK(image.is_import(s.exit_slot) && !image.is_import(s.hello));
});

CB_TEST("analyze.native.owner_and_covering", [] {
    const Sample s;
    Image image(s.spec());
    CB_CHECK(*image.owner(s.A) == s.A);
    CB_CHECK(*image.owner(s.A + 0x13) == s.A);
    CB_CHECK(*image.owner(s.A + 0x1F) == s.A);                   // by limit: the padding before B is A's
    CB_CHECK(*image.owner(s.B) == s.B);
    CB_CHECK(!image.owner(s.A - 1).has_value());
    CB_CHECK(!image.owner(s.hello).has_value());                 // data belongs to no function (the last limit ends)
    const Instruction* ins = image.covering(s.A + 4);            // the middle of `push 5`, which starts at A+3
    CB_CHECK(ins != nullptr && ins->addr == s.A + 3);
    CB_CHECK(image.covering(s.A + 3)->addr == s.A + 3);
    CB_CHECK(image.covering(s.A - 1) == nullptr);
});

CB_TEST("analyze.native.names_and_labels", [] {
    const Sample s;
    ImageSpec spec = s.spec();
    spec.function_names[s.A] = "start";
    Image image(std::move(spec));
    CB_CHECK_EQ(image.label(s.A), std::string("start"));
    CB_CHECK_EQ(image.label(s.exit_slot), std::string("KERNEL32!ExitProcess"));
    CB_CHECK_EQ(image.label(0x4aBc10), std::string("4abc10"));
    CB_CHECK_EQ(image.function_name(s.B), std::string());
    image.set_function_name(s.B, "helper");
    CB_CHECK_EQ(image.label(s.B), std::string("helper"));
    CB_CHECK_EQ(image.import_name(s.wsprintf_slot), std::string("USER32!wsprintfA"));
});

CB_TEST("analyze.native.family_names_the_whole_register", [] {
    CB_CHECK_EQ(family("al"), std::string_view("eax"));
    CB_CHECK_EQ(family("ah"), std::string_view("eax"));
    CB_CHECK_EQ(family("ax"), std::string_view("eax"));
    CB_CHECK_EQ(family("rax"), std::string_view("eax"));
    CB_CHECK_EQ(family("sil"), std::string_view("esi"));
    CB_CHECK_EQ(family("bp"), std::string_view("ebp"));
    CB_CHECK_EQ(family("esp"), std::string_view("esp"));
    CB_CHECK_EQ(family("ds"), std::string_view("ds"));
    CB_CHECK_EQ(family(""), std::string_view(""));
});

// ---- 16-bit ------------------------------------------------------------------------------------------------------

ImageSpec spec16(const cb::Bytes& code, Va base) {
    ImageSpec s;
    s.kind = ImageKind::Ne;
    s.bits = 16;
    Span span;
    span.va = base;
    span.data = code;
    span.executable = true;
    span.initialised = static_cast<Va>(code.size());
    span.name = "CODE";
    s.spans = {span};
    s.functions = {{base, base + static_cast<Va>(code.size())}};
    return s;
}

CB_TEST("analyze.native.sixteen_bit_targets_stay_in_their_segment_and_far_calls_name_both", [] {
    // 0x10000100: call near +0x00FD (to 0x...0200); lcall 0x1234:0x5678; ret
    Asm a(0x10000100);
    a.raw({0xE8, 0xFD, 0x00}).raw({0x9A, 0x78, 0x56, 0x34, 0x12}).raw({0xC3});
    Image image(spec16(a.bytes(), 0x10000100));
    const Function& f = image.function(0x10000100);
    CB_CHECK_EQ(f.insns.size(), std::size_t{3});
    CB_CHECK(f.insns[0].flow == Flow::Call);
    CB_CHECK(f.insns[0].target.has_value() && *f.insns[0].target == 0x10000200);
    CB_CHECK_EQ(f.insns[1].mnemonic, std::string("lcall"));
    CB_CHECK(f.insns[1].target.has_value() && *f.insns[1].target == 0x12345678);
    CB_CHECK_EQ(image.bits(), 16);
});

// ---- what is held at a call --------------------------------------------------------------------------------------

CB_TEST("analyze.native.arguments_are_the_pushes_before_a_call_nearest_first", [] {
    const Sample s;
    Image image(s.spec());
    const Function& a = image.function(s.A);
    const std::vector<Argument> args = arguments(a, 4, 2);
    CB_CHECK_EQ(args.size(), std::size_t{2});
    CB_CHECK(args[0].kind == Argument::Kind::Immediate && args[0].value == s.hello);
    CB_CHECK(args[1].kind == Argument::Kind::Immediate && args[1].value == 5);
    CB_CHECK_EQ(arguments(a, 4, 1).size(), std::size_t{1});
    CB_CHECK(arguments(a, 4, 0).empty());
    // asked for more than were pushed for the call, it goes on to the `push ebp` of the prologue
    const std::vector<Argument> more = arguments(a, 4, 9);
    CB_CHECK_EQ(more.size(), std::size_t{3});
    CB_CHECK(more[2].kind == Argument::Kind::Register && more[2].reg == "ebp");
    CB_CHECK(arguments(a, 0, 3).empty());                        // nothing before the first instruction
});

CB_TEST("analyze.native.a_pushed_register_is_replaced_by_what_it_holds", [] {
    // xor eax,eax; push eax; mov ecx,0x12345678; push ecx; push ecx's copy via edx; call B
    Asm a(0x401000);
    a.raw({0x33, 0xC0, 0x50});                         // xor eax,eax ; push eax          -> 0
    a.raw({0xB9, 0x78, 0x56, 0x34, 0x12, 0x51});       // mov ecx,0x12345678 ; push ecx    -> 0x12345678
    a.raw({0x8B, 0xD1, 0x52});                         // mov edx,ecx ; push edx           -> 0x12345678 through a copy
    a.raw({0x8D, 0x45, 0xF0, 0x50});                   // lea eax,[ebp-0x10] ; push eax    -> an address on the stack
    a.raw({0x50});                                     // push eax                         -> the same
    a.call_indirect(0x402000).raw({0x50});             // call [slot] ; push eax           -> the call's result
    a.raw({0x55});                                     // push ebp                         -> the register as it is
    a.call(0x401000).ret();
    ImageSpec spec = spec16(a.bytes(), 0x401000);      // (spec16 only to hold the bytes; it is a 32-bit image)
    spec.bits = 32;
    spec.kind = ImageKind::Pe;
    Image img32(std::move(spec));
    const Function& f = img32.function(0x401000);
    const int call_index = static_cast<int>(f.insns.size()) - 2;
    const std::vector<Argument> args = arguments(f, call_index, 9);
    // nearest first: push ebp, then the result of the call, and there the pushes stop (a call separates them)
    CB_CHECK_EQ(args.size(), std::size_t{2});
    CB_CHECK(args[0].kind == Argument::Kind::Register && args[0].reg == "ebp");
    CB_CHECK(args[1].kind == Argument::Kind::CallResult);
    CB_CHECK(args[1].call_target.has_value() && *args[1].call_target == 0x402000);
    // before the call: the earlier pushes
    const int slot_call = call_index - 3;               // call [slot]
    CB_CHECK(f.insns[static_cast<std::size_t>(slot_call)].flow == Flow::Call);
    const std::vector<Argument> earlier = arguments(f, slot_call, 9);
    CB_CHECK_EQ(earlier.size(), std::size_t{5});
    CB_CHECK(earlier[0].kind == Argument::Kind::LoadAddress);            // push eax (lea)
    CB_CHECK(earlier[1].kind == Argument::Kind::LoadAddress);            // lea eax; push eax
    CB_CHECK(earlier[2].kind == Argument::Kind::Immediate && earlier[2].value == 0x12345678);   // via edx <- ecx
    CB_CHECK(earlier[3].kind == Argument::Kind::Immediate && earlier[3].value == 0x12345678);
    CB_CHECK(earlier[4].kind == Argument::Kind::Immediate && earlier[4].value == 0);             // xor eax,eax
});

CB_TEST("analyze.native.definition_gives_up_at_a_branch", [] {
    // mov eax,5 ; jmp +0 ; push eax ; call: eax is 5 before the jump, and unknown after it
    Asm a(0x401000);
    a.raw({0xB8, 5, 0, 0, 0, 0xEB, 0x00, 0x50}).call(0x401000);
    ImageSpec spec = spec16(a.bytes(), 0x401000);
    spec.bits = 32;
    Image image(std::move(spec));
    const Function& f = image.function(0x401000);
    CB_CHECK_EQ(f.insns.size(), std::size_t{4});
    const std::optional<Argument> before = definition(f, 1, "eax");
    CB_CHECK(before.has_value() && before->kind == Argument::Kind::Immediate && before->value == 5);
    CB_CHECK(!definition(f, 2, "eax").has_value());
    CB_CHECK(!definition(f, 0, "eax").has_value());              // nothing before the first instruction
    CB_CHECK(!definition(f, 1, "ebx").has_value());              // a register nothing set
});

}  // namespace
