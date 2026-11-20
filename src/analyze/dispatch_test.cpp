// Tests of cb/analyze/dispatch.hpp over small programs written byte by byte: a compare chain on the message argument of a
// window procedure (with a command dispatch on wParam), a subtract chain, a jump table, strings compared with literals,
// DOS/BIOS/DPMI interrupts in 16-bit code, and a driver's control procedure.
#include "analyze/dispatch.hpp"

#include "analyze/image.hpp"
#include "analyze/test_image.hpp"
#include "core/testing.hpp"

using namespace cb;
using namespace cb::analyze;
namespace dispatch = cb::analyze::dispatch;

namespace {

constexpr Va kCode = 0x401000;

Span code_span(Va va, const Bytes& bytes) {
    Span s;
    s.va = va;
    s.data = bytes;
    s.executable = true;
    s.initialised = static_cast<Va>(bytes.size());
    s.name = ".text";
    return s;
}

// A flat 32-bit program of one function that spans all `bytes`.
Image program(const Bytes& bytes, std::vector<std::pair<Va, Va>> functions = {}) {
    ImageSpec spec;
    spec.kind = ImageKind::Pe;
    spec.bits = 32;
    spec.flat = true;
    spec.spans = {code_span(kCode, bytes)};
    if (functions.empty()) functions = {{kCode, kCode + static_cast<Va>(bytes.size())}};
    spec.functions = functions;
    spec.roots = {functions.front().first};
    return Image(std::move(spec));
}

// je/jne to `target` (short)
void jcc(test::Asm& a, int opcode, Va target) {
    a.raw({opcode, static_cast<int>((target - (a.here() + 2)) & 0xFF)});
}

// The window procedure: `switch (message) { case WM_DESTROY, WM_CLOSE, WM_COMMAND }` and in WM_COMMAND `switch (wParam)`.
Bytes window_procedure() {
    test::Asm a(kCode);
    a.raw({0x55, 0x8B, 0xEC});                       // push ebp / mov ebp,esp
    a.raw({0x8B, 0x45, 0x0C});                       // mov eax,[ebp+0Ch]
    a.raw({0x3D, 0x11, 0x01, 0x00, 0x00});           // cmp eax,111h
    jcc(a, 0x74, kCode + 0x40);
    a.raw({0x83, 0xF8, 0x10});                       // cmp eax,10h
    jcc(a, 0x74, kCode + 0x50);
    a.raw({0x83, 0xF8, 0x02});                       // cmp eax,2
    jcc(a, 0x74, kCode + 0x60);
    a.raw({0x33, 0xC0, 0x5D, 0xC2, 0x10, 0x00});     // xor eax,eax / pop ebp / ret 10h
    a.pad_to(kCode + 0x40);
    // WM_COMMAND: switch (wParam) { 100, 101, 102 }
    a.raw({0x83, 0x7D, 0x10, 0x64});                 // cmp dword [ebp+10h],100
    jcc(a, 0x74, kCode + 0x70);
    a.raw({0x83, 0x7D, 0x10, 0x65});                 // cmp dword [ebp+10h],101
    jcc(a, 0x74, kCode + 0x70);
    a.raw({0x83, 0x7D, 0x10, 0x66});                 // cmp dword [ebp+10h],102
    jcc(a, 0x74, kCode + 0x78);
    a.raw({0xC2, 0x10, 0x00});
    a.pad_to(kCode + 0x50);
    a.raw({0x33, 0xC0, 0xC2, 0x10, 0x00});           // WM_CLOSE
    a.pad_to(kCode + 0x60);
    a.raw({0x33, 0xC0, 0xC2, 0x10, 0x00});           // WM_DESTROY
    a.pad_to(kCode + 0x70);
    a.raw({0xB0, 0x01, 0xC2, 0x10, 0x00});
    a.pad_to(kCode + 0x78);
    a.raw({0xB0, 0x02, 0xC2, 0x10, 0x00});
    a.pad_to(kCode + 0x80);
    return a.bytes();
}

ConventionFacts window_facts() {
    ConventionFact f;
    f.convention = "stdcall";
    f.procedure = "window procedure";
    f.removes = 16;
    f.frame = true;
    return {{kCode, f}};
}

}  // namespace

CB_TEST("analyze.dispatch.the_message_names", [] {
    CB_CHECK_EQ(dispatch::message_name(0x111).value(), std::string_view("WM_COMMAND"));
    CB_CHECK_EQ(dispatch::message_name(0x0F).value(), std::string_view("WM_PAINT"));
    CB_CHECK(!dispatch::message_name(0x4).has_value());           // (there is no message 4)
    CB_CHECK(!dispatch::message_name(-1).has_value());
    CB_CHECK_EQ(dispatch::control_message_name(0).value(), std::string_view("Sys_Critical_Init"));
    CB_CHECK_EQ(dispatch::control_message_name(0x32).value(), std::string_view("Kernel32_Shutdown"));
    CB_CHECK(!dispatch::control_message_name(0x33).has_value());
});

CB_TEST("analyze.dispatch.a_compare_chain_is_a_switch_on_what_the_register_was_loaded_from", [] {
    Image image = program(window_procedure());
    const std::vector<dispatch::Switch> sw = dispatch::switches(image);
    CB_CHECK_EQ(sw.size(), std::size_t{2});                       // the message switch and the command switch
    const dispatch::Switch& message = sw[0];
    CB_CHECK(message.function == kCode && message.on == "[ebp+0xc]");
    CB_CHECK(message.key && message.key->kind == dispatch::SwitchKey::Kind::Memory && message.key->base == "ebp" &&
             !message.key->index && message.key->disp == 0x0C);
    CB_CHECK(message.cases == (std::map<Va, Va>{{2, kCode + 0x60}, {0x10, kCode + 0x50}, {0x111, kCode + 0x40}}));
    CB_CHECK(!message.table && !message.site && !message.default_target && message.bounded);
    const dispatch::Switch& command = sw[1];
    CB_CHECK(command.on == "[ebp+0x10]");
    CB_CHECK(command.cases == (std::map<Va, Va>{{100, kCode + 0x70}, {101, kCode + 0x70}, {102, kCode + 0x78}}));
});

CB_TEST("analyze.dispatch.a_chain_needs_three_cases_to_be_reported", [] {
    test::Asm a(kCode);
    a.raw({0x8B, 0x44, 0x24, 0x04});                  // mov eax,[esp+4]
    a.raw({0x83, 0xF8, 0x01});
    jcc(a, 0x74, kCode + 0x20);
    a.raw({0x83, 0xF8, 0x02});
    jcc(a, 0x74, kCode + 0x20);
    a.raw({0xC3});
    a.pad_to(kCode + 0x20);
    a.raw({0xC3});
    Image image = program(a.bytes());
    CB_CHECK(dispatch::switches(image).empty());
    // ... but the chain itself is there
    const dispatch::Chains chains = dispatch::chains(image.function(kCode));
    CB_CHECK_EQ(chains.size(), std::size_t{1});
    CB_CHECK_EQ(chains.begin()->second.cases.size(), std::size_t{2});
    // `jne` makes the case the instruction after it, not its target
    test::Asm b(kCode);
    b.raw({0x8B, 0x44, 0x24, 0x04, 0x83, 0xF8, 0x07});
    jcc(b, 0x75, kCode + 0x20);                       // jne
    b.raw({0xC3});
    b.pad_to(kCode + 0x20);
    b.raw({0xC3});
    Image other = program(b.bytes());
    const dispatch::Chains kept = dispatch::chains(other.function(kCode));
    CB_CHECK_EQ(kept.begin()->second.cases.at(7), kCode + 9);   // equal falls through to the `ret` after the jne
});

CB_TEST("analyze.dispatch.a_subtract_chain_counts_what_was_taken_off", [] {
    test::Asm a(kCode);
    a.raw({0x83, 0xE8, 0x05});                        // sub eax,5
    jcc(a, 0x74, kCode + 0x20);                       // je -> 5
    a.raw({0x48});                                    // dec eax
    jcc(a, 0x74, kCode + 0x28);                       // je -> 6
    a.raw({0x83, 0xE8, 0x10});                        // sub eax,10h
    jcc(a, 0x74, kCode + 0x30);                       // je -> 16h
    a.raw({0xC3});
    a.pad_to(kCode + 0x20);
    a.raw({0xC3});
    a.pad_to(kCode + 0x28);
    a.raw({0xC3});
    a.pad_to(kCode + 0x30);
    a.raw({0xC3});
    Image image = program(a.bytes());
    const std::vector<dispatch::Switch> sw = dispatch::switches(image);
    CB_CHECK_EQ(sw.size(), std::size_t{1});
    CB_CHECK(sw[0].on == "eax" && sw[0].key && sw[0].key->kind == dispatch::SwitchKey::Kind::Register && sw[0].key->base == "eax");
    CB_CHECK(sw[0].cases == (std::map<Va, Va>{{5, kCode + 0x20}, {6, kCode + 0x28}, {0x16, kCode + 0x30}}));
});

CB_TEST("analyze.dispatch.a_jump_table_is_a_switch_with_a_default", [] {
    test::Sample s;
    Image image((ImageSpec(s.spec())));
    const std::vector<dispatch::Switch> sw = dispatch::switches(image);
    CB_CHECK_EQ(sw.size(), std::size_t{1});
    const dispatch::Switch& t = sw[0];
    CB_CHECK(t.function == s.C && t.on == "[esp+0x4]" && t.table == s.jump_table && t.bounded);
    CB_CHECK(t.cases == (std::map<Va, Va>{{0, s.case_address[0]}, {1, s.case_address[1]}, {2, s.case_address[2]}}));
    CB_CHECK(t.default_target == s.case_address[2] + 6);
    CB_CHECK(t.site.has_value());
    CB_CHECK(t.key && t.key->base == "esp" && t.key->disp == 4);
});

CB_TEST("analyze.dispatch.the_message_argument_makes_a_window_procedure", [] {
    Image image = program(window_procedure());
    const std::vector<dispatch::Switch> sw = dispatch::switches(image);
    const std::vector<dispatch::MessageDispatch> found = dispatch::messages(image, sw, window_facts());
    CB_CHECK_EQ(found.size(), std::size_t{1});
    const dispatch::MessageDispatch& m = found[0];
    CB_CHECK(m.function == kCode && m.procedure == "window procedure" && m.why == "switch on the message argument");
    CB_CHECK(m.messages == (std::map<Va, Va>{{2, kCode + 0x60}, {0x10, kCode + 0x50}, {0x111, kCode + 0x40}}));
    CB_CHECK(m.commands == (std::map<Va, Va>{{100, kCode + 0x70}, {101, kCode + 0x70}, {102, kCode + 0x78}}));
});

namespace {

// A procedure that loads the argument at [ebp+`disp`] and compares it with each of `cases` in turn.
Bytes chain_on_argument(int disp, std::initializer_list<int> cases) {
    test::Asm a(kCode);
    a.raw({0x55, 0x8B, 0xEC, 0x8B, 0x45, disp});                // push ebp / mov ebp,esp / mov eax,[ebp+disp]
    for (int v : cases) {
        a.raw({0x3D, v & 0xFF, (v >> 8) & 0xFF, 0x00, 0x00});   // cmp eax,v
        jcc(a, 0x74, kCode + 0x60);
    }
    a.raw({0xC3});
    a.pad_to(kCode + 0x60);
    a.raw({0xC3});
    return a.bytes();
}

}  // namespace

CB_TEST("analyze.dispatch.no_convention_no_window_procedure_unless_the_cases_say_so", [] {
    // the message argument with two message numbers above the small integers is enough, convention or not
    Image image = program(window_procedure());
    const std::vector<dispatch::Switch> sw = dispatch::switches(image);
    ConventionFacts plain = window_facts();
    plain[kCode].procedure.reset();
    plain[kCode].removes = 0;
    CB_CHECK_EQ(dispatch::messages(image, sw, plain).size(), std::size_t{1});
    // the same cases on the first argument are not a message dispatch (two numbers that only look like messages) ...
    Image first = program(chain_on_argument(8, {0x111, 0x10, 0x02}));
    const auto s1 = dispatch::switches(first);
    CB_CHECK_EQ(s1.size(), std::size_t{1});
    CB_CHECK(dispatch::messages(first, s1, {}).empty());
    // ... unless a procedure is known to be one
    ConventionFacts known;
    known[kCode].procedure = "dialog procedure";
    const auto dialog = dispatch::messages(first, s1, known);
    CB_CHECK_EQ(dialog.size(), std::size_t{1});
    CB_CHECK_EQ(dialog[0].why, "a dialog procedure");
    // ... or the cases are nearly all message numbers, enough of them above the small integers, one a message every window handles
    Image many = program(chain_on_argument(8, {0x0F, 0x10, 0x111, 0x81}));
    const auto s2 = dispatch::switches(many);
    const auto found = dispatch::messages(many, s2, {});
    CB_CHECK_EQ(found.size(), std::size_t{1});
    CB_CHECK_EQ(found[0].why, "cases are message numbers");
    CB_CHECK_EQ(found[0].messages.size(), std::size_t{4});
});

CB_TEST("analyze.dispatch.a_driver_has_no_message_dispatch", [] {
    ImageSpec spec;
    spec.kind = ImageKind::Le;
    spec.bits = 32;
    spec.spans = {code_span(0x01000000, window_procedure())};
    spec.functions = {{0x01000000, 0x01000080}};
    Image image(std::move(spec));
    const auto sw = dispatch::switches(image);
    CB_CHECK(dispatch::messages(image, sw, {}).empty());
});

CB_TEST("analyze.dispatch.strings_compared_with_literals_are_keywords", [] {
    test::Asm a(kCode);
    constexpr Va slot = 0x402100;
    for (Va s : {Va{0x402000}, Va{0x402010}}) {
        a.raw({0x50});                                // push eax (the string compared)
        a.push_imm(static_cast<std::uint32_t>(s));    // push "keyword"
        a.call_indirect(slot);                        // call [lstrcmpiA]
        a.raw({0x85, 0xC0});                          // test eax,eax
    }
    a.ret();
    ImageSpec spec;
    spec.kind = ImageKind::Pe;
    spec.bits = 32;
    spec.flat = true;
    spec.spans = {code_span(kCode, a.bytes())};
    Span data;
    data.va = 0x402000;
    data.data.assign(0x200, 0);
    data.initialised = 0x200;
    data.name = ".data";
    const std::string alpha = "alpha", beta = "beta";
    std::copy(alpha.begin(), alpha.end(), data.data.begin());
    std::copy(beta.begin(), beta.end(), data.data.begin() + 0x10);
    spec.spans.push_back(data);
    spec.functions = {{kCode, kCode + static_cast<Va>(a.bytes().size())}};
    spec.imports = {{slot, "KERNEL32!lstrcmpiA"}};
    spec.roots = {kCode};
    Image image(std::move(spec));
    const dispatch::Keywords kw = dispatch::keywords(image);
    CB_CHECK_EQ(kw.functions.size(), std::size_t{1});
    CB_CHECK(kw.functions[0].function == kCode && kw.functions[0].compares_with == "lstrcmpi");
    CB_CHECK(kw.functions[0].keywords == (std::vector<std::string>{"alpha", "beta"}));
    CB_CHECK(kw.tables.empty());
});

CB_TEST("analyze.dispatch.one_literal_is_not_a_dispatch", [] {
    test::Asm a(kCode);
    constexpr Va slot = 0x402100;
    for (int n = 0; n < 2; ++n) {
        a.raw({0x50});
        a.push_imm(0x402000);
        a.call_indirect(slot);
    }
    a.ret();
    ImageSpec spec;
    spec.kind = ImageKind::Pe;
    spec.bits = 32;
    spec.flat = true;
    spec.spans = {code_span(kCode, a.bytes())};
    Span data;
    data.va = 0x402000;
    data.data.assign(0x200, 0);
    data.initialised = 0x200;
    data.name = ".data";
    std::copy_n("same", 4, data.data.begin());
    spec.spans.push_back(data);
    spec.functions = {{kCode, kCode + static_cast<Va>(a.bytes().size())}};
    spec.imports = {{slot, "KERNEL32!lstrcmpA"}};
    spec.roots = {kCode};
    Image image(std::move(spec));
    CB_CHECK(dispatch::keywords(image).functions.empty());     // the same word twice
});

namespace {

// 16-bit code in the first segment of a Windows module.
Image dos_program(const Bytes& bytes) {
    ImageSpec spec;
    spec.kind = ImageKind::Ne;
    spec.bits = 16;
    const Va base = image::ne_address(1, 0);
    spec.spans = {code_span(base, bytes)};
    spec.functions = {{base, base + static_cast<Va>(bytes.size())}};
    spec.roots = {base};
    return Image(std::move(spec));
}

}  // namespace

CB_TEST("analyze.dispatch.interrupts_get_their_service_names", [] {
    Bytes code = {0xB4, 0x09, 0xCD, 0x21,                        // mov ah,9 / int 21h
                  0xB8, 0x00, 0x4C, 0xCD, 0x21,                  // mov ax,4C00h / int 21h
                  0xB8, 0x87, 0x16, 0xCD, 0x2F,                  // mov ax,1687h / int 2Fh
                  0xCD, 0x10,                                    // int 10h (no number in sight)
                  0xB8, 0x00, 0x04, 0xCD, 0x31,                  // mov ax,400h / int 31h
                  0xCD, 0x5F,                                    // int 5Fh (not in the table)
                  0xB4, 0x5D, 0xCD, 0x21,                        // mov ah,5Dh / int 21h (a service not in the table)
                  0xCD, 0x21,                                    // int 21h (no number in sight)
                  0xC3};
    Image image = dos_program(code);
    const std::vector<dispatch::Interrupt> ints = dispatch::interrupts(image);
    CB_CHECK_EQ(ints.size(), std::size_t{8});
    const Va base = image::ne_address(1, 0);
    CB_CHECK(ints[0].site == base + 2 && ints[0].number == 0x21 && ints[0].code == 9u && ints[0].name == "DOS: display string" && ints[0].named);
    CB_CHECK(ints[1].code == 0x4Cu && ints[1].name == "DOS: exit");
    CB_CHECK(ints[2].number == 0x2F && ints[2].code == 0x1687u && ints[2].name == "multiplex function 1687h" && ints[2].named);
    CB_CHECK(ints[3].number == 0x10 && !ints[3].code && ints[3].name == "video" && ints[3].named);
    CB_CHECK(ints[4].number == 0x31 && ints[4].code == 0x400u && ints[4].name == "DPMI: get version" && ints[4].named);
    CB_CHECK(ints[5].number == 0x5F && ints[5].name == "interrupt 5Fh" && !ints[5].named);
    CB_CHECK(ints[6].code == 0x5Du && ints[6].name == "DOS: function 5Dh" && ints[6].named);
    CB_CHECK(!ints[7].code && ints[7].name == "DOS: function ?" && !ints[7].named);
    for (const dispatch::Interrupt& i : ints) CB_CHECK(i.function == base);
});

CB_TEST("analyze.dispatch.the_report_counts_and_describes", [] {
    Image image = program(window_procedure());
    const dispatch::Report r = dispatch::analyse(image, window_facts());
    CB_CHECK_EQ(r.counts.compare_chains, 2);
    CB_CHECK_EQ(r.counts.compare_chain_cases, 6);
    CB_CHECK_EQ(r.counts.jump_tables, 0);
    CB_CHECK_EQ(r.counts.switches_on_an_argument, 2);
    CB_CHECK_EQ(r.counts.message_dispatchers, 1);
    CB_CHECK_EQ(r.counts.messages_handled, 3);
    CB_CHECK_EQ(r.counts.named_messages, 3);
    CB_CHECK_EQ(r.counts.command_dispatchers, 1);
    CB_CHECK_EQ(r.counts.command_ids, 3);
    CB_CHECK_EQ(r.counts.keyword_dispatchers, 0);
    CB_CHECK_EQ(r.counts.interrupt_sites, 0);
    CB_CHECK_EQ(r.facts.size(), std::size_t{3});                       // two switches and the message dispatch
    const auto& m = std::get<dispatch::MessageFact>(r.facts[2]);
    CB_CHECK(m.function == kCode && m.why == "switch on the message argument" && m.procedure == "window procedure");
    CB_CHECK_EQ(m.messages.size(), std::size_t{3});
    CB_CHECK(m.messages[0].value == 2 && m.messages[0].name == "WM_DESTROY" && m.messages[0].handler == kCode + 0x60);
    CB_CHECK(m.messages[2].name == "WM_COMMAND");
    CB_CHECK(m.commands.size() == 3 && m.commands[0].first == 100);
    CB_CHECK_EQ(r.text.size(), std::size_t{1});
    CB_CHECK(r.text[0].ends_with("(switch on the message argument): WM_DESTROY, WM_CLOSE, WM_COMMAND"));
    const auto& s = std::get<dispatch::SwitchFact>(r.facts[0]);
    CB_CHECK(s.function == kCode && s.on == "[ebp+0xc]" && s.cases.size() == 3 && s.cases[0].first == 2);
});

CB_TEST("analyze.dispatch.a_driver_control_procedure_names_its_system_messages", [] {
    // 01000000: the control procedure -- cmp eax,0 / je ; cmp eax,1 / je ; cmp eax,2 / je   (Sys_Critical_Init, Device_Init, Init_Complete)
    // 01000100: the device descriptor block, whose control procedure is at +18h
    test::Asm a(0x01000000);
    a.raw({0x83, 0xF8, 0x00});
    jcc(a, 0x74, 0x01000030);
    a.raw({0x83, 0xF8, 0x01});
    jcc(a, 0x74, 0x01000034);
    a.raw({0x83, 0xF8, 0x02});
    jcc(a, 0x74, 0x01000038);
    a.raw({0xC3});
    a.pad_to(0x01000030);
    a.raw({0xC3});
    a.pad_to(0x01000034);
    a.raw({0xC3});
    a.pad_to(0x01000038);
    a.raw({0xC3});
    a.pad_to(0x01000100);
    a.pad_to(0x01000118, 0);
    a.dword(0x01000000);
    a.pad_to(0x01000130, 0);
    ImageSpec spec;
    spec.kind = ImageKind::Le;
    spec.bits = 32;
    spec.spans = {code_span(0x01000000, a.bytes())};
    spec.functions = {{0x01000000, 0x01000100}};
    spec.roots = {0x01000000};
    Image image(std::move(spec));
    const dispatch::DeviceBlock device{0x01000100, "DEMO"};
    const auto sw = dispatch::switches(image);
    const auto ctl = dispatch::control_messages(image, sw, device);
    CB_CHECK(ctl.has_value());
    CB_CHECK_EQ(ctl->procedure, Va{0x01000000});
    CB_CHECK_EQ(ctl->handles.size(), std::size_t{3});
    CB_CHECK(ctl->handles[0].name == "Sys_Critical_Init" && ctl->handles[0].handler == 0x01000030);
    CB_CHECK(ctl->handles[1].name == "Device_Init" && ctl->handles[2].name == "Init_Complete" && ctl->handles[2].handler == 0x01000038);
    // not a driver, or no device block, or a control procedure that is not a function
    CB_CHECK(!dispatch::control_messages(image, sw, std::nullopt).has_value());
    CB_CHECK(!dispatch::control_messages(image, sw, dispatch::DeviceBlock{0x01000104, "X"}).has_value());
    const dispatch::Report r = dispatch::analyse(image, {}, device);
    CB_CHECK_EQ(r.counts.driver_control_messages, 3);
    const auto& fact = std::get<dispatch::DriverControlFact>(r.facts.back());
    CB_CHECK(fact.procedure == 0x01000000 && fact.device == "DEMO" && fact.handles.size() == 3);
    CB_CHECK(r.text.back().find("device DEMO control procedure") == 0);
    CB_CHECK(r.text.back().ends_with("handles: Sys_Critical_Init, Device_Init, Init_Complete"));
});

CB_TEST("analyze.dispatch.a_module_with_nothing_to_dispatch_reports_nothing", [] {
    test::Sample s;
    Image image((ImageSpec(s.spec())));
    // (the sample's one switch is a jump table)
    const dispatch::Report r = dispatch::analyse(image, {});
    CB_CHECK_EQ(r.counts.jump_tables, 1);
    CB_CHECK_EQ(r.counts.jump_table_cases, 3);
    CB_CHECK_EQ(r.counts.jump_tables_with_bound, 1);
    CB_CHECK(r.counts.message_dispatchers == 0 && r.counts.keyword_dispatchers == 0 && r.counts.interrupt_sites == 0);
    CB_CHECK(r.text.empty());
});

CB_TEST("analyze.dispatch.switch_keys_are_ordered_and_hashed", [] {
    dispatch::SwitchKey reg, mem, indexed;
    reg.base = "eax";
    mem.kind = dispatch::SwitchKey::Kind::Memory;
    mem.base = "ebp";
    mem.disp = 8;
    indexed = mem;
    indexed.index = "eax";
    CB_CHECK(reg != mem && mem != indexed && reg < mem);
    CB_CHECK(dispatch::SwitchKeyHash()(mem) == dispatch::SwitchKeyHash()(dispatch::SwitchKey(mem)));
    CB_CHECK(dispatch::SwitchKeyHash()(mem) != dispatch::SwitchKeyHash()(indexed));
    dispatch::Chains chains;
    chains.try_emplace(mem, dispatch::Chain{"x", {}});
    CB_CHECK(chains.contains(dispatch::SwitchKey(mem)) && !chains.contains(indexed));
});
