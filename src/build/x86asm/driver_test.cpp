// Tests of x86asm's driver and the modules it ties together (the bin format, the listing, the directives, the parser, the
// command line). The expected bytes and texts are what the real nasm.exe (native/nasm/build/bin/nasm.exe) wrote for the same
// input on Windows.
#include <string>
#include <string_view>

#include "build/x86asm/driver.hpp"
#include "build/x86asm/x86asm.hpp"
#include "core/testing.hpp"

namespace {

using namespace cb;
using cb::build::x86asm::Request;
using cb::build::x86asm::Result;

// Run x86asm on one source named `name` in the folder C:/w with the arguments `args` (the file name is given as a relative name).
Result run_one(std::vector<std::string> args, std::string_view name, std::string_view source) {
    Request r;
    r.cwd = "C:/w";
    r.args = std::move(args);
    r.args.push_back(std::string(name));
    r.files["C:/w/" + std::string(name)] = to_bytes(source);
    return cb::build::x86asm::assemble(r);
}

std::string text_of(const Bytes& b) { return std::string(as_text(b)); }

CB_TEST("x86asm.driver.bin_flat_image", [] {
    const Result r = run_one({"-f", "bin", "-o", "t1.bin"}, "t1.asm",
                             "bits 16\n"
                             "org 0x100\n"
                             "start:\n"
                             "    mov ax, 1\n"
                             "    jmp start\n"
                             "    db \"hi\", 0\n"
                             "    dw start, 0x1234\n"
                             "    times 4 db 0xAA\n");
    CB_CHECK_EQ(r.exit, 0);
    CB_CHECK_EQ(r.messages, std::string());
    const Bytes want = {0xb8, 0x01, 0x00, 0xeb, 0xfb, 0x68, 0x69, 0x00, 0x00, 0x01, 0x34, 0x12, 0xaa, 0xaa, 0xaa, 0xaa};
    CB_CHECK(r.files.count("t1.bin"));
    CB_CHECK(r.files.at("t1.bin") == want);
});

CB_TEST("x86asm.driver.default_output_name_and_format", [] {
    // No -f: the default format is bin; no -o: the input's name with the format's extension (none for bin).
    const Result r = run_one({}, "prog.asm", "db 1, 2, 3\n");
    CB_CHECK_EQ(r.exit, 0);
    CB_CHECK(r.files.count("prog"));
    CB_CHECK(r.files.at("prog") == (Bytes{1, 2, 3}));
});

CB_TEST("x86asm.format_bin.sections_and_map_file", [] {
    const Result r = run_one({"-f", "bin", "-o", "t2.bin"}, "t2.asm",
                             "[map all t2.map]\n"
                             "section .text start=0x100\n"
                             "a: db 1,2,3\n"
                             "section .data follows=.text align=16\n"
                             "b: dd 0x11223344\n"
                             "section .bss nobits\n"
                             "c: resb 4\n");
    CB_CHECK_EQ(r.exit, 0);
    Bytes want(16, 0);
    want[0] = 1;
    want[1] = 2;
    want[2] = 3;
    for (const int b : {0x44, 0x33, 0x22, 0x11}) want.push_back(static_cast<std::uint8_t>(b));
    CB_CHECK(r.files.at("t2.bin") == want);
    const std::string map =
        "\r\n"
        "- NASM Map file ---------------------------------------------------------------\r\n"
        "\r\n"
        "Source file:  t2.asm\r\n"
        "Output file:  t2.bin\r\n"
        "\r\n"
        "-- Program origin -------------------------------------------------------------\r\n"
        "\r\n"
        "00000100\r\n"
        "\r\n"
        "-- Sections (summary) ---------------------------------------------------------\r\n"
        "\r\n"
        "Vstart            Start             Stop              Length    Class     Name\r\n"
        "             100               100               103  00000003  progbits  .text\r\n"
        "             110               110               114  00000004  progbits  .data\r\n"
        "             114               114               118  00000004  nobits    .bss\r\n"
        "\r\n"
        "-- Sections (detailed) --------------------------------------------------------\r\n"
        "\r\n"
        "---- Section .text ------------------------------------------------------------\r\n"
        "\r\n"
        "class:     progbits\r\n"
        "length:                   3\r\n"
        "start:                  100\r\n"
        "align:     not defined\r\n"
        "follows:   not defined\r\n"
        "vstart:                 100\r\n"
        "valign:    not defined\r\n"
        "vfollows:  not defined\r\n"
        "\r\n"
        "---- Section .data ------------------------------------------------------------\r\n"
        "\r\n"
        "class:     progbits\r\n"
        "length:                   4\r\n"
        "start:                  110\r\n"
        "align:                   10\r\n"
        "follows:   .text\r\n"
        "vstart:                 110\r\n"
        "valign:    not defined\r\n"
        "vfollows:  not defined\r\n"
        "\r\n"
        "---- Section .bss -------------------------------------------------------------\r\n"
        "\r\n"
        "class:     nobits\r\n"
        "length:                   4\r\n"
        "start:                  114\r\n"
        "align:     not defined\r\n"
        "follows:   not defined\r\n"
        "vstart:                 114\r\n"
        "valign:                   4\r\n"
        "vfollows:  not defined\r\n"
        "\r\n"
        "-- Symbols --------------------------------------------------------------------\r\n"
        "\r\n"
        "---- Section .text ------------------------------------------------------------\r\n"
        "\r\n"
        "Real              Virtual           Name\r\n"
        "             100               100  a\r\n"
        "\r\n"
        "---- Section .data ------------------------------------------------------------\r\n"
        "\r\n"
        "Real              Virtual           Name\r\n"
        "             110               110  b\r\n"
        "\r\n"
        "---- Section .bss -------------------------------------------------------------\r\n"
        "\r\n"
        "Real              Virtual           Name\r\n"
        "             114               114  c\r\n"
        "\r\n";
    CB_CHECK(r.files.count("t2.map"));
    CB_CHECK_EQ(text_of(r.files.at("t2.map")), map);
});

CB_TEST("x86asm.driver.errors_are_messages_and_no_output", [] {
    const Result r = run_one({"-f", "bin", "-o", "t3.bin"}, "t3.asm",
                             "        bits 32\n"
                             "        mov eax, undefined_sym\n"
                             "        jmp short far_label\n");
    CB_CHECK_EQ(r.exit, 1);
    CB_CHECK_EQ(r.messages, std::string("t3.asm:2: error: symbol `undefined_sym' not defined\r\n"
                                        "t3.asm:3: error: symbol `far_label' not defined\r\n"));
    CB_CHECK(!r.files.count("t3.bin"));
});

CB_TEST("x86asm.listing.list_file", [] {
    const Result r = run_one({"-f", "bin", "-o", "t4.bin", "-l", "t4.lst"}, "t4.asm",
                             "mov ax,1\n"
                             "; listing\n"
                             "label: times 2 db 5\n");
    CB_CHECK_EQ(r.exit, 0);
    CB_CHECK(r.files.count("t4.lst"));
    CB_CHECK_EQ(text_of(r.files.at("t4.lst")),
                std::string("     1 00000000 B80100                  mov ax,1\r\n"
                            "     2                                  ; listing\r\n"
                            "     3 00000003 05<rep 2h>              label: times 2 db 5\r\n"));
});

CB_TEST("x86asm.driver.command_line_errors", [] {
    {
        Request r;
        r.args = {"-f", "nosuchformat", "a.asm"};
        const Result res = cb::build::x86asm::assemble(r);
        CB_CHECK_EQ(res.exit, 1);
        CB_CHECK(res.messages.starts_with("nasm: fatal: unrecognised output format `nosuchformat' - use -hf for a list\r\n"));
    }
    {
        Request r;
        r.args = {"-f", "bin"};
        const Result res = cb::build::x86asm::assemble(r);
        CB_CHECK_EQ(res.exit, 1);
        CB_CHECK(res.messages.find("no input file specified") != std::string::npos);
    }
});

CB_TEST("x86asm.driver.intel_hex_and_srec", [] {
    const Result hex = run_one({"-f", "ith", "-o", "h.hex"}, "h.asm", "db 1,2,3\n");
    CB_CHECK_EQ(hex.exit, 0);
    CB_CHECK_EQ(text_of(hex.files.at("h.hex")), std::string(":03000000010203F7\r\n:00000001FF\r\n"));
    const Result srec = run_one({"-f", "srec", "-o", "h.srec"}, "h.asm", "db 1,2,3\n");
    CB_CHECK_EQ(srec.exit, 0);
    CB_CHECK_EQ(text_of(srec.files.at("h.srec")), std::string("S0030000FC\r\nS1060000010203F3\r\nS9030000FC\r\n"));
});

}  // namespace
