// Tests of cb/symbols/dos_interrupts on the project's own table (testdata/dos_interrupts.json, a copy of
// codebreaker/symbols/dos_interrupts.json). The expected names were checked against the documented interfaces
// (Ralf Brown's list: INT 21h AH=3Dh open file, INT 2Fh AX=1600h Windows enhanced mode check, INT 33h mouse).
// The Python module's whole harness (33,000 name, describe and annotate cases, state/work/typed-port) also gave the
// same answers from this code for every number in 0..2^32.
//
// DIVERGES from the Python module, by design: annotate() treats a letter outside ASCII before `swi(` as a boundary
// (Python's \b took a Latin-1 or CJK letter for part of a word), and an interrupt, AH or AL below 0 or above 2^32-1
// cannot be asked (they were Python ints). The command reads a bad number as a usage error.
#include "cb/symbols/dos_interrupts.hpp"

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace di = cb::symbols::dos_interrupts;

namespace {

const di::ServiceTable& table() {
    static const di::ServiceTable t = di::ServiceTable::load(cb::testing::data_path("symbols/testdata/dos_interrupts.json"));
    return t;
}

}  // namespace

CB_TEST("symbols.dos_interrupts.names_services", [] {
    di::Service s = table().name(0x21, 0x3D);
    CB_CHECK_EQ(s.name, "DOS_OpenFile");
    CB_CHECK(s.exact);
    CB_CHECK(s.arguments.starts_with("AL = access mode"));

    s = table().name(0x10, 0x00);
    CB_CHECK_EQ(s.name, "Video_SetVideoMode");
    CB_CHECK_EQ(s.arguments, "AL = mode");

    // a subfunction table wins over the plain one when AL is known
    s = table().name(0x2F, 0x16, 0x00);
    CB_CHECK_EQ(s.name, "Multiplex_WindowsEnhancedModeCheck");
    CB_CHECK(s.exact);
    s = table().name(0x21, 0x44, 0x00);
    CB_CHECK_EQ(s.name, "DOS_IoctlGetDeviceInformation");
    // an AL the subfunction table lacks falls back to the service itself
    s = table().name(0x21, 0x44, 0xEE);
    CB_CHECK_EQ(s.name, "DOS_Ioctl");
});

CB_TEST("symbols.dos_interrupts.not_every_service_is_named", [] {
    di::Service s = table().name(0x21);   // AH not known here
    CB_CHECK_EQ(s.name, "DOS");
    CB_CHECK_EQ(s.arguments, "service number not a constant here");
    CB_CHECK(!s.exact);

    s = table().name(0x10, 0x77);   // a service outside the table
    CB_CHECK_EQ(s.name, "Video_AH_77h");
    CB_CHECK(!s.exact);

    s = table().name(0x99, 0x01);   // an interrupt outside it
    CB_CHECK_EQ(s.name, "int_99h");
    CB_CHECK_EQ(s.arguments, "");
    CB_CHECK(!s.exact);

    s = table().name(0x20);   // an interrupt that is one service
    CB_CHECK(s.exact);
});

CB_TEST("symbols.dos_interrupts.the_mouse_selects_by_all_of_ax", [] {
    di::Service s = table().name(0x33, 0x00, 0x00);   // AX = 0000h: reset
    CB_CHECK(s.exact);
    CB_CHECK(s.name.starts_with("Mouse_"));
    s = table().name(0x33, 0x01, 0x00);   // AH != 0: not a mouse function number
    CB_CHECK(!s.exact);
    s = table().name(0x33, 0x00);   // AL not known
    CB_CHECK(!s.exact);
});

CB_TEST("symbols.dos_interrupts.describe", [] {
    di::Interrupt i;
    i.number = 0x21;
    i.ah = 0x3D;
    i.address = 0x100;
    CB_CHECK(table().describe(i).starts_with("int 21h AH=3Dh  DOS_OpenFile(AL = access mode"));
    di::Interrupt j;
    j.number = 0x99;
    j.ah = 1;
    j.al = 2;
    CB_CHECK_EQ(table().describe(j), "int 99h AH=01h AL=02h  int_99h");
    di::Interrupt k;
    k.number = 0x10;
    CB_CHECK_EQ(table().describe(k), "int 10h  Video(service number not a constant here)");
});

CB_TEST("symbols.dos_interrupts.annotate", [] {
    di::Interrupt open;
    open.number = 0x21;
    open.ah = 0x3D;
    open.address = 0x100;
    di::Interrupt other;
    other.number = 0x10;
    other.ah = 0x00;
    other.address = 0x120;
    const std::vector<di::Interrupt> calls = {open, other};
    const std::string text = "void f() { swi(0x21); swi(0x10); swi(0x21 ); xswi(0x21); swi(0x33); }";
    const std::string out = table().annotate(text, calls);
    CB_CHECK(out.starts_with("/* interrupt calls, in address order:\n     00100  int 21h AH=3Dh  DOS_OpenFile(AL = access mode"));
    CB_CHECK(out.find(" */\nvoid f() { swi(0x21) /* DOS_OpenFile */; swi(0x10) /* Video_SetVideoMode */; swi(0x21 ); xswi(0x21); swi(0x33); }") !=
             std::string::npos);

    // an interrupt asked for two different services in the function is not named inline
    di::Interrupt close = open;
    close.ah = 0x3E;
    close.address = 0x110;
    const std::vector<di::Interrupt> two = {open, close};
    const std::string both = table().annotate("swi(0x21);", two);
    CB_CHECK(both.ends_with(" */\nswi(0x21);"));

    // no calls: the text as it is
    CB_CHECK_EQ(table().annotate("swi(0x21);", {}), "swi(0x21);");
});

CB_TEST("symbols.dos_interrupts.counts", [] {
    CB_CHECK_EQ(table().interrupt_count(), std::size_t{27});
    CB_CHECK(table().service_count() > 300);
});

CB_TEST("symbols.dos_interrupts.bad_tables", [] {
    CB_CHECK_THROWS_AS(cb::ParseError, (void)di::ServiceTable::parse("{"));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)di::ServiceTable::parse("[]"));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)di::ServiceTable::parse(R"({"zz": {"name": "X"}})"));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)di::ServiceTable::parse(R"({"21": {"arguments": "x"}})"));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)di::ServiceTable::parse(R"({"21": {"name": "X", "services": {"01": ["only-one"]}}})"));
    CB_CHECK_THROWS_AS(cb::NotFoundError, (void)di::ServiceTable::load(cb::testing::data_path("symbols/testdata/no-such-table.json")));
    const di::ServiceTable tiny = di::ServiceTable::parse(R"({"21": {"name": "DOS", "services": {"01": ["Read", "AL = n"]}}})");
    CB_CHECK_EQ(tiny.interrupt_count(), std::size_t{1});
    CB_CHECK_EQ(tiny.name(0x21, 1).name, "DOS_Read");
});

CB_TEST("symbols.dos_interrupts.command", [] {
    {
        cb::testing::CapturedStd cap;
        CB_CHECK_EQ(di::run(table(), {}), 0);
        CB_CHECK_EQ(cap.out(), "27 interrupts, " + std::to_string(table().service_count()) + " services\n");
    }
    {
        cb::testing::CapturedStd cap;
        const std::vector<std::string> args = {"21", "3d"};
        CB_CHECK_EQ(di::run(table(), args), 0);
        CB_CHECK(cap.out().starts_with("DOS_OpenFile(AL = access mode"));
    }
    {
        cb::testing::CapturedStd cap;
        const std::vector<std::string> args = {"10", "100"};
        CB_CHECK_EQ(di::run(table(), args), 0);
        CB_CHECK_EQ(cap.out(), "Video_AH_100h   [not exact]\n");
    }
    {
        cb::testing::CapturedStd cap;
        const std::vector<std::string> args = {"21", "zz"};
        CB_CHECK_EQ(di::run(table(), args), cb::kExitUsage);
        CB_CHECK_EQ(cap.out(), "");
    }
});
