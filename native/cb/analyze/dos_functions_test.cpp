// Tests of cb/analyze/dos_functions.hpp: small real-mode programs written byte by byte (a call, a jump table, an installed
// interrupt handler, a far pointer in data, a prologue nothing calls) and real DOS programs of Windows 95 (testdata/ of
// cb/formats: a .COM file, EXEPACKed and PKLITEd programs' siblings, a device driver, an EXE carrying a driver header).
//
// The expected figures for the real programs are the recorded figures for the same
// bytes (133 programs of the corpus and 700 made-up ones were checked when this was written).
//
// A relocation at the last byte of the image, or a program with no data segment that jumps or calls through memory: the far
// pointer is not one, and the table or variable is not followed.
#include "cb/analyze/dos_functions.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
namespace df = cb::analyze::dos_functions;
namespace dos = cb::formats::dos;

namespace {

constexpr std::uint32_t kBase = dos::kLoadSegment * 16;   // where a program's image is loaded, linearly

Bytes testdata(const char* name) { return fs::read_file(testing::data_path(std::string("formats/testdata/") + name)); }

// A program from its image: one span over all of it, one entry at its start.
dos::Program program_of(Bytes image, dos::Entry::Call call = dos::Entry::Call::Near) {
    dos::Program p;
    p.kind = dos::Kind::Mz;
    p.base = kBase;
    p.spans.push_back({kBase, static_cast<std::uint32_t>(image.size()), "image"});
    p.entries.push_back({kBase, "start", call});
    p.image = std::move(image);
    return p;
}

Bytes bytes_of(std::initializer_list<int> list) {
    Bytes out;
    for (int b : list) out.push_back(static_cast<std::uint8_t>(b));
    return out;
}

// Put `data` at `at` in a zero-filled image of `size`.
void place(Bytes& image, std::size_t at, const Bytes& data) { std::copy(data.begin(), data.end(), image.begin() + static_cast<std::ptrdiff_t>(at)); }

std::size_t count_with(const df::Analysis& a, df::Evidence e) {
    std::size_t n = 0;
    for (const auto& [va, f] : a.functions) n += f.evidence == e ? 1 : 0;
    return n;
}

}  // namespace

CB_TEST("analyze.dos_functions.a_call_and_the_end_of_the_program", [] {
    // 0:   call 10h            E8 0D 00
    // 3:   mov ah, 4Ch         B4 4C
    // 5:   int 21h             CD 21          (ends the program)
    // 10h: push bp / mov bp,sp / mov ah,9 / int 21h / pop bp / ret      55 8B EC B4 09 CD 21 5D C3
    Bytes image(31, 0);
    place(image, 0, bytes_of({0xE8, 0x0D, 0x00, 0xB4, 0x4C, 0xCD, 0x21}));
    place(image, 0x10, bytes_of({0x55, 0x8B, 0xEC, 0xB4, 0x09, 0xCD, 0x21, 0x5D, 0xC3}));
    const dos::Program p = program_of(image);
    const df::Analysis a = df::discover(p);
    CB_CHECK_EQ(a.functions.size(), std::size_t{2});
    const df::Function& start = a.functions.at(kBase);
    CB_CHECK(start.name == "start" && start.evidence == df::Evidence::Entry && start.distance == df::Distance::Near && start.sure);
    CB_CHECK(start.cs == kBase && start.size == 7);
    CB_CHECK_EQ(start.removes, -1);                        // it never returns
    const df::Function& callee = a.functions.at(kBase + 0x10);
    CB_CHECK(callee.name == "sub_00010" && callee.evidence == df::Evidence::Called && callee.distance == df::Distance::Near);
    CB_CHECK(callee.size == 9 && callee.removes == 0 && !callee.interrupt);
    // the two `int 21h`, each with the service number loaded in the same run
    CB_CHECK_EQ(a.interrupts.size(), std::size_t{2});
    CB_CHECK(a.interrupts[0].address == kBase + 5 && a.interrupts[0].number == 0x21 && a.interrupts[0].ah == 0x4C && !a.interrupts[0].al);
    CB_CHECK(a.interrupts[0].function == kBase);
    CB_CHECK(a.interrupts[1].address == kBase + 0x15 && a.interrupts[1].ah == 9 && a.interrupts[1].function == kBase + 0x10);
    CB_CHECK(a.tables.empty() && a.vectors.empty());
    // 7 + 9 bytes of code; the rest of the image is nothing found
    CB_CHECK(a.census.code == 16 && a.census.guessed_code == 0 && a.census.jump_tables == 0 && a.census.not_classified == 15 &&
             a.census.total == 31);
});

CB_TEST("analyze.dos_functions.a_jump_table_is_followed_by_the_function_that_jumps", [] {
    // 0:   jmp [bx+40h]        FF A7 40 00
    // 10h, 14h, 18h:  inc ax / dec ax / ret          40 48 C3
    // 40h: dw 10h, 14h, 18h, 0  (the zero is the segment itself: the table ends)
    Bytes image(0x48, 0);
    place(image, 0, bytes_of({0xFF, 0xA7, 0x40, 0x00}));
    for (std::size_t at : {0x10, 0x14, 0x18}) place(image, at, bytes_of({0x40, 0x48, 0xC3}));
    place(image, 0x40, bytes_of({0x10, 0x00, 0x14, 0x00, 0x18, 0x00, 0x00, 0x00}));
    const df::Analysis a = df::discover(program_of(image));
    CB_CHECK_EQ(a.functions.size(), std::size_t{1});          // the targets are part of the function that jumps to them
    CB_CHECK_EQ(a.functions.at(kBase).size, 13u);             // 4 + three of 3
    CB_CHECK_EQ(a.functions.at(kBase).removes, 0);
    CB_CHECK_EQ(a.tables.size(), std::size_t{1});
    CB_CHECK_EQ(a.tables.at(kBase + 0x40), std::size_t{3});
    CB_CHECK_EQ(a.census.jump_tables, 6u);
});

CB_TEST("analyze.dos_functions.a_called_table_makes_functions", [] {
    // 0:   call [bx+40h]       FF 97 40 00        then  mov ah,4Ch / int 21h
    // 10h, 14h: inc ax / dec ax / ret ; 40h: dw 10h, 14h, 0
    Bytes image(0x46, 0);
    place(image, 0, bytes_of({0xFF, 0x97, 0x40, 0x00, 0xB4, 0x4C, 0xCD, 0x21}));
    for (std::size_t at : {0x10, 0x14}) place(image, at, bytes_of({0x40, 0x48, 0xC3}));
    place(image, 0x40, bytes_of({0x10, 0x00, 0x14, 0x00, 0x00, 0x00}));
    const df::Analysis a = df::discover(program_of(image));
    CB_CHECK_EQ(a.functions.size(), std::size_t{3});
    CB_CHECK_EQ(count_with(a, df::Evidence::Table), std::size_t{2});
    CB_CHECK(a.functions.at(kBase + 0x10).evidence == df::Evidence::Table && a.functions.at(kBase + 0x14).sure);
    CB_CHECK_EQ(a.tables.at(kBase + 0x40), std::size_t{2});
});

CB_TEST("analyze.dos_functions.an_installed_interrupt_handler_is_a_function", [] {
    // 0:   mov ax,1000h / mov ds,ax / mov ah,25h / mov al,1Ch / mov dx,20h / int 21h / mov ah,4Ch / int 21h
    // 20h: push ax / pop ax / iret      the handler
    Bytes image(0x23, 0);
    place(image, 0, bytes_of({0xB8, 0x00, 0x10, 0x8E, 0xD8, 0xB4, 0x25, 0xB0, 0x1C, 0xBA, 0x20, 0x00, 0xCD, 0x21, 0xB4, 0x4C, 0xCD, 0x21}));
    place(image, 0x20, bytes_of({0x50, 0x58, 0xCF}));
    const df::Analysis a = df::discover(program_of(image));
    CB_CHECK_EQ(a.vectors.size(), std::size_t{1});
    CB_CHECK_EQ(a.vectors.at(kBase + 0x20), std::uint8_t{0x1C});
    const df::Function& handler = a.functions.at(kBase + 0x20);
    CB_CHECK(handler.name == "int_1C_handler_00020" && handler.evidence == df::Evidence::Vector);
    CB_CHECK(handler.distance == df::Distance::Far && handler.interrupt && handler.removes == 0);
    CB_CHECK_EQ(a.interrupts.size(), std::size_t{2});
    CB_CHECK(a.interrupts[0].ah == 0x25 && a.interrupts[0].al == 0x1C);
});

CB_TEST("analyze.dos_functions.a_store_into_the_interrupt_table_installs_a_handler", [] {
    // 0:   xor ax,ax / mov es,ax / mov word es:[70h*4], 20h / mov word es:[70h*4+2], 1000h / int 20h
    // 20h: push ax / pop ax / iret
    Bytes image(0x23, 0);
    place(image, 0, bytes_of({0x33, 0xC0, 0x8E, 0xC0, 0x26, 0xC7, 0x06, 0xC0, 0x01, 0x20, 0x00, 0x26, 0xC7, 0x06, 0xC2, 0x01, 0x00, 0x10, 0xCD, 0x20}));
    place(image, 0x20, bytes_of({0x50, 0x58, 0xCF}));
    const df::Analysis a = df::discover(program_of(image));
    CB_CHECK_EQ(a.vectors.size(), std::size_t{1});
    CB_CHECK_EQ(a.vectors.at(kBase + 0x20), std::uint8_t{0x70});
    CB_CHECK(a.functions.at(kBase + 0x20).evidence == df::Evidence::Vector);
});

CB_TEST("analyze.dos_functions.a_far_pointer_in_data_is_a_guess", [] {
    // 0: mov ah,4Ch / int 21h ; 20h: inc ax / dec ax / ret ; 40h: dw 20h, 1000h (a relocated segment)
    Bytes image(0x44, 0);
    place(image, 0, bytes_of({0xB4, 0x4C, 0xCD, 0x21}));
    place(image, 0x20, bytes_of({0x40, 0x48, 0xC3}));
    place(image, 0x40, bytes_of({0x20, 0x00, 0x00, 0x10}));
    dos::Program p = program_of(image, dos::Entry::Call::Far);
    p.relocations = {kBase + 0x42};
    const df::Analysis a = df::discover(p);
    CB_CHECK_EQ(a.functions.size(), std::size_t{2});
    const df::Function& guessed = a.functions.at(kBase + 0x20);
    CB_CHECK(guessed.evidence == df::Evidence::Pointer && !guessed.sure && guessed.distance == df::Distance::Near);
    CB_CHECK(a.functions.at(kBase).distance == df::Distance::Far);
    CB_CHECK(a.census.guessed_code == 3 && a.census.code == 4);
    // a relocation on the image's last byte is no far pointer (and must not be read past the end)
    p.relocations = {kBase + 0x42, kBase + 0x43};
    CB_CHECK_NOTHROW(df::discover(p));
});

CB_TEST("analyze.dos_functions.a_prologue_in_bytes_nothing_reaches_is_a_guess", [] {
    // 0: mov ah,4Ch / int 21h ; 20h: push bp / mov bp,sp / inc ax / pop bp / ret
    Bytes image(0x28, 0);
    place(image, 0, bytes_of({0xB4, 0x4C, 0xCD, 0x21}));
    place(image, 0x20, bytes_of({0x55, 0x8B, 0xEC, 0x40, 0x5D, 0xC3}));
    const df::Analysis a = df::discover(program_of(image));
    CB_CHECK_EQ(a.functions.size(), std::size_t{2});
    const df::Function& f = a.functions.at(kBase + 0x20);
    CB_CHECK(f.evidence == df::Evidence::Prologue && !f.sure);
}  );

CB_TEST("analyze.dos_functions.what_follows_a_return_is_a_guess", [] {
    // 0: call 10h ; 3: mov ah,4Ch / int 21h ; 10h: inc ax / dec ax / ret ; 13h: inc bx / dec bx / ret (no prologue, no caller)
    Bytes image(0x1A, 0);
    place(image, 0, bytes_of({0xE8, 0x0D, 0x00, 0xB4, 0x4C, 0xCD, 0x21}));
    place(image, 0x10, bytes_of({0x40, 0x48, 0xC3}));
    place(image, 0x13, bytes_of({0x43, 0x4B, 0xC3}));
    const df::Analysis a = df::discover(program_of(image));
    CB_CHECK_EQ(a.functions.size(), std::size_t{3});
    CB_CHECK(a.functions.at(kBase + 0x13).evidence == df::Evidence::Follows && !a.functions.at(kBase + 0x13).sure);
});

CB_TEST("analyze.dos_functions.far_returns_make_a_far_function_and_remove_their_arguments", [] {
    // 0: call far 1000h:10h (relocated) ; 10h: push bp / mov bp,sp / pop bp / retf 4
    Bytes image(0x18, 0);
    place(image, 0, bytes_of({0x9A, 0x10, 0x00, 0x00, 0x10, 0xB4, 0x4C, 0xCD, 0x21}));
    place(image, 0x10, bytes_of({0x55, 0x8B, 0xEC, 0x5D, 0xCA, 0x04, 0x00}));
    dos::Program p = program_of(image);
    p.relocations = {kBase + 3};
    const df::Analysis a = df::discover(p);
    const df::Function& f = a.functions.at(kBase + 0x10);
    CB_CHECK(f.distance == df::Distance::Far && f.removes == 4 && f.evidence == df::Evidence::Called);
    // a far call whose segment the file does not relocate is not to this program: the function is found, if at all, by its prologue
    p.relocations = {kBase + 0x14};
    const df::Analysis loose = df::discover(p);
    CB_CHECK(loose.functions.at(kBase + 0x10).evidence == df::Evidence::Prologue && !loose.functions.at(kBase + 0x10).sure);
}  );

CB_TEST("analyze.dos_functions.a_program_with_nothing_in_it_has_no_functions", [] {
    const df::Analysis none = df::discover(program_of(Bytes(64, 0)));
    CB_CHECK(none.functions.size() == 1 && none.functions.begin()->second.size == 0 && none.interrupts.empty());   // (the entry is a function whatever is there)
    CB_CHECK(none.census.total == 64 && none.census.not_classified == 64);
    CB_CHECK(df::discover(dos::Program{}).functions.empty());       // (no entry, no function)
    // random-looking bytes never read outside the image
    Bytes noise(300);
    std::uint32_t x = 12345;
    for (std::uint8_t& b : noise) {
        x = x * 1103515245u + 12345u;
        b = static_cast<std::uint8_t>(x >> 16);
    }
    CB_CHECK_NOTHROW(df::discover(program_of(noise)));
    CB_CHECK_NOTHROW(df::discover(program_of(noise, dos::Entry::Call::Far)));
});

CB_TEST("analyze.dos_functions.the_report_lists_what_was_found", [] {
    Bytes image(31, 0);
    place(image, 0, bytes_of({0xE8, 0x0D, 0x00, 0xB4, 0x4C, 0xCD, 0x21}));
    place(image, 0x10, bytes_of({0x55, 0x8B, 0xEC, 0xB4, 0x09, 0xCD, 0x21, 0x5D, 0xC3}));
    dos::Program p = program_of(image);
    p.notes = {"hand-made"};
    const df::Analysis a = df::discover(p);
    const auto services = symbols::dos_interrupts::ServiceTable::load(testing::data_path("symbols/testdata/dos_interrupts.json"));
    const std::vector<std::string> brief = df::report("demo.exe", p, a, false, services);
    CB_CHECK_EQ(brief.size(), std::size_t{5});
    CB_CHECK_EQ(brief[0], "demo.exe: mz, 31 bytes loaded");
    CB_CHECK_EQ(brief[1], "2 functions: 1 called, 1 entry; 0 of them rest on a guess");
    CB_CHECK_EQ(brief[2], "bytes: code 16, guessed code 0, jump tables 0, not classified 15, total 31");
    CB_CHECK_EQ(brief[3], "2 interrupt calls, 2 with the service named");
    CB_CHECK_EQ(brief[4], "note: hand-made");
    const std::vector<std::string> all = df::report("demo.exe", p, a, true, services);
    CB_CHECK_EQ(all.size(), std::size_t{7});
    // (the name takes 26 columns, the size 5)
    CB_CHECK_EQ(all[5], "  10000 start" + std::string(22, ' ') + "near removes -1      7 bytes  entry");
    CB_CHECK_EQ(all[6], "  10010 sub_00010" + std::string(18, ' ') + "near removes  0      9 bytes  called");
}  );

// ---- real programs --------------------------------------------------------------------------------------------------

namespace {

struct Expect {
    const char* file;
    std::size_t functions, sure;
    std::size_t entry, called, follows, prologue, variable, vector;
    std::size_t interrupts;
    std::uint64_t code, guessed, tables, unclassified, total;
    std::uint32_t first_address;
    const char* first_name;
};

void check_real(const Expect& e) {
    const Bytes data = testdata(e.file);
    const dos::Program p = dos::read_program(data, e.file);
    const df::Analysis a = df::discover(p);
    const std::string what = std::string(e.file) + ": ";
    CB_CHECK_EQ(a.functions.size(), e.functions);
    std::size_t sure = 0;
    for (const auto& [va, f] : a.functions) sure += f.sure ? 1 : 0;
    CB_CHECK_EQ(sure, e.sure);
    CB_CHECK_EQ(count_with(a, df::Evidence::Entry), e.entry);
    CB_CHECK_EQ(count_with(a, df::Evidence::Called), e.called);
    CB_CHECK_EQ(count_with(a, df::Evidence::Follows), e.follows);
    CB_CHECK_EQ(count_with(a, df::Evidence::Prologue), e.prologue);
    CB_CHECK_EQ(count_with(a, df::Evidence::Variable), e.variable);
    CB_CHECK_EQ(count_with(a, df::Evidence::Vector), e.vector);
    CB_CHECK_EQ(a.interrupts.size(), e.interrupts);
    CB_CHECK_EQ(a.census.code, e.code);
    CB_CHECK_EQ(a.census.guessed_code, e.guessed);
    CB_CHECK_EQ(a.census.jump_tables, e.tables);
    CB_CHECK_EQ(a.census.not_classified, e.unclassified);
    CB_CHECK_EQ(a.census.total, e.total);
    CB_CHECK_EQ(a.functions.begin()->first, e.first_address);
    CB_CHECK_EQ(a.functions.begin()->second.name, std::string(e.first_name));
    (void)what;
}

}  // namespace

CB_TEST("analyze.dos_functions.choice_com", [] {
    check_real({"choice.com", 22, 22, 1, 21, 0, 0, 0, 0, 39, 2366, 0, 0, 2809, 5175, 65792, "entry"});
    const df::Analysis a = df::discover(dos::read_program(testdata("choice.com"), "choice.com"));
    const df::Function& entry = a.functions.at(65792);
    CB_CHECK(entry.distance == df::Distance::Near && entry.removes == -1 && entry.size == 657);   // it ends in `int 21h`
    // its first `int 21h` asks for the DOS version (AH=30h), with AL = 1
    CB_CHECK(a.interrupts[0].address == 66085 && a.interrupts[0].number == 0x21 && a.interrupts[0].ah == 0x30 && a.interrupts[0].al == 1);
});

CB_TEST("analyze.dos_functions.a_device_driver", [] {
    check_real({"aspi2hlp.sys", 10, 7, 2, 7, 1, 0, 0, 0, 1, 567, 278, 0, 260, 1105, 65576, "ASPI2HL_strategy"});
    const df::Analysis a = df::discover(dos::read_program(testdata("aspi2hlp.sys"), "aspi2hlp.sys"));
    const df::Function& strategy = a.functions.at(65576);
    CB_CHECK(strategy.evidence == df::Evidence::Entry && strategy.distance == df::Distance::Far && strategy.sure);   // the driver's entries are far
});

CB_TEST("analyze.dos_functions.find_exe", [] {
    check_real({"find.exe", 61, 60, 1, 59, 1, 0, 0, 0, 36, 4925, 14, 0, 7077, 12016, 65842, "sub_00132"});
});

CB_TEST("analyze.dos_functions.xcopy_exe", [] {
    check_real({"xcopy.exe", 34, 34, 1, 33, 0, 0, 0, 0, 35, 2713, 0, 0, 1223, 3936, 65536, "sub_00000"});
});

CB_TEST("analyze.dos_functions.setver_exe_has_every_kind_of_evidence_but_a_table", [] {
    check_real({"setver.exe", 100, 91, 3, 86, 4, 5, 1, 1, 44, 7770, 162, 0, 8495, 16427, 65566, "SETVERXX_strategy"});
    const df::Analysis a = df::discover(dos::read_program(testdata("setver.exe"), "setver.exe"));
    CB_CHECK_EQ(a.vectors.size(), std::size_t{1});
    CB_CHECK_EQ(a.vectors.at(73162), std::uint8_t{0});                  // a handler for INT 0 (divide error)
    CB_CHECK(a.functions.rbegin()->second.cs == 67728);                 // code in a second segment
});

CB_TEST("analyze.dos_functions.the_same_program_gives_the_same_answer", [] {
    const dos::Program p = dos::read_program(testdata("setver.exe"), "setver.exe");
    const df::Analysis a = df::discover(p), b = df::discover(p);
    CB_CHECK_EQ(a.functions.size(), b.functions.size());
    CB_CHECK(a.interrupts.size() == b.interrupts.size() && a.census.code == b.census.code && a.vectors == b.vectors);
    auto it = b.functions.begin();
    for (const auto& [va, f] : a.functions) {
        CB_CHECK(f.address == it->second.address && f.name == it->second.name && f.size == it->second.size);
        ++it;
    }
});
