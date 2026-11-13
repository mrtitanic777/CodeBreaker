// Tests of cb/invert/msc6_small.hpp: Microsoft C 6.0a's leaf templates, the statement walker, the free idiom, the register variables and the
// counted loop, on the bytes the compiler gave for small functions.
//
// The expected texts of leaf, cleanup, registers, counted_loop and mask are what the Python models answered for the same bytes (run through
// the Python itself, each read over for sense). The walker's are written out by hand: the Python's answer for a walk that read was
// unbalanced JSON (a bug of native/invert/msc6_small.cc, fixed there too -- see _bugs/LIST.md), so there is nothing to copy.
//
// DIVERGES from the Python: the code ending inside a statement is a decline here, where the Python raised IndexError; an if whose jump leaves the
// body is declined, where the Python sliced backwards from the end.
#include "cb/invert/msc6_small.hpp"

#include <chrono>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

using namespace cb;
using namespace cb::invert::msc6_small;

namespace {

Bytes bytes_of(std::string_view hex) {
    Bytes out;
    for (std::size_t i = 0; i + 1 < hex.size(); i += 2) out.push_back(static_cast<std::uint8_t>(std::stoi(std::string(hex.substr(i, 2)), nullptr, 16)));
    return out;
}

std::string hex_of(ByteView b) {
    static constexpr char kDigits[] = "0123456789abcdef";
    std::string out;
    for (const std::uint8_t x : b) out += {kDigits[x >> 4], kDigits[x & 15]};
    return out;
}

constexpr std::string_view kPrologue = "1e589045558bec1e8ed8";
constexpr std::string_view kEpilogue = "4d4d8be51f5d4d";

// A leaf function: the frame, a body, the epilogue and `ca 04 00` (two parameters popped).
Bytes leaf_of(std::string_view body) { return bytes_of(std::string(kPrologue) + std::string(body) + std::string(kEpilogue) + "ca0400"); }

constexpr std::string_view kCleanup =
    "1e589045558bec1e8ed883ec02a146000b0644007411ff3646009ae70000002bc0a34600a34400a14a000b0648007411ff364a009a010100002bc0a34a00a34800a156000b0654007411ff3656"
    "009affff00002bc0a35600a354004d4d8be51f5d4dcb90";

std::string why(const Error& e) { return e.what(); }

}  // namespace

CB_TEST("invert.msc6_small.leaf_templates", [] {
    const struct {
        const char* body;
        const char* text;
        const char* shape;
    } cases[] = {
        {"", "int far pascal F(int a0, int a1)\n{\n}", "empty"},
        {"b80700", "int far pascal F(int a0, int a1)\n{\n    return 7;\n}", "return 7;"},
        {"8b4606", "int far pascal F(int a0, int a1)\n{\n    return a0;\n}", "return a0;"},
        {"a10000", "int gG;\nint far pascal F(int a0, int a1)\n{\n    return gG;\n}", "return gG;"},
        {"c70600003400", "int gG;\nvoid far pascal F(int a0, int a1)\n{\n    gG = 52;\n}", "gG = 52;"},
        {"6a059a00000000", "extern void far pascal G(int);\nvoid far pascal F(int a0, int a1)\n{\n    G(5);\n}", "T5 far-call"},
        {"ff76066a0168050" "09a00000000", "extern void far pascal G(int, int, int);\nvoid far pascal F(int a0, int a1)\n{\n    G(a0, 1, 5);\n}", "T5 far-call"},
        {"837e0600750" "5b80100eb03b80200", "int far pascal F(int a0, int a1)\n{\n    if (a0 == 0)\n        return 1;\n    return 2;\n}", "T6 if-branch"},
        {"837e06057c05b80100eb03b80200", "int far pascal F(int a0, int a1)\n{\n    if (a0 >= 5)\n        return 1;\n    return 2;\n}", "T6 if-branch"},
    };
    for (const auto& c : cases) {
        const Result<Leaf> r = leaf(leaf_of(c.body));
        CB_CHECK(r.ok());
        if (!r.ok()) continue;
        CB_CHECK_EQ(r.value().text, std::string(c.text));
        CB_CHECK_EQ(r.value().shape, std::string(c.shape));
        CB_CHECK_EQ(r.value().arguments, 2u);
    }
    const Leaf call = leaf(leaf_of("ff76066a01680500" "9a00000000")).value();
    CB_CHECK(call.call_arguments == (std::vector<std::string>{"a0", "1", "5"}));
    const Leaf store = leaf(leaf_of("c70600003400")).value();
    CB_CHECK_EQ(store.return_type, std::string("void"));
    CB_CHECK(store.globals == (std::vector<std::string>{"gG"}));
    const Leaf branch = leaf(leaf_of("837e0600750" "5b80100eb03b80200")).value();
    CB_CHECK(branch.lines == (std::vector<std::string>{"if (a0 == 0)", "    return 1;", "return 2;"}));
});

CB_TEST("invert.msc6_small.leaf_with_a_frame_and_a_trailing_nop_has_an_unused_local", [] {
    const Result<Leaf> r = leaf(bytes_of(std::string(kPrologue) + "83ec02" + std::string(kEpilogue) + "ca000090"));
    CB_CHECK(r.ok());
    CB_CHECK_EQ(r.value().text, std::string("int far pascal F(void)\n{\n    int i;\n}"));
    CB_CHECK_EQ(r.value().frame, 2u);
    CB_CHECK(r.value().trailing_nop);
    CB_CHECK_EQ(r.value().shape, std::string("int i"));
});

CB_TEST("invert.msc6_small.leaf_declines_with_a_reason", [] {
    CB_CHECK_EQ(why(leaf(bytes_of("90")).error()), std::string("prologue not the /Gsw loadds template (T0 miss)"));
    CB_CHECK_EQ(why(leaf(leaf_of("90909090")).error()), std::string("body not a modeled leaf template (4 bytes: 90909090)"));
    CB_CHECK_EQ(why(leaf(bytes_of(std::string(kPrologue) + "b80700" + std::string(kEpilogue) + "c3")).error()), std::string("no far return (ca/cb) after epilogue"));
    CB_CHECK_EQ(why(leaf(bytes_of(std::string(kPrologue) + "b80700")).error()), std::string("epilogue (T0 tail) not found"));
    // a branch whose jump leaves the body is not a template
    CB_CHECK(!leaf(leaf_of("837e0600757fb80100")).ok());
    // the code ends inside the call
    CB_CHECK(!leaf(leaf_of("6a05" "9a0000")).ok());
});

CB_TEST("invert.msc6_small.cleanup_is_the_free_idiom_repeated", [] {
    const Result<std::string> r = cleanup(bytes_of(kCleanup), {{0x44, "gpA"}, {0x48, "gpB"}, {0x54, "gpC"}});
    CB_CHECK(r.ok());
    CB_CHECK_EQ(r.value(),
                std::string("typedef unsigned int WORD;\nextern WORD far pascal GlobalFree(WORD);\nextern long gpA;\nextern long gpB;\nextern long gpC;\n\nvoid far Cleanup(void)\n{\n"
                            "    if (gpA != 0L) {\n        GlobalFree((WORD)(gpA >> 16));\n        gpA = 0L;\n    }\n"
                            "    if (gpB != 0L) {\n        GlobalFree((WORD)(gpB >> 16));\n        gpB = 0L;\n    }\n"
                            "    if (gpC != 0L) {\n        GlobalFree((WORD)(gpC >> 16));\n        gpC = 0L;\n    }\n}"));
    // a name the symbols do not give is the offset
    const Result<std::string> unnamed = cleanup(bytes_of(kCleanup), {});
    CB_CHECK(unnamed.ok());
    CB_CHECK(unnamed.value().find("extern long g_44;") != std::string::npos);
    // a block that stops short of the idiom is declined, naming where
    CB_CHECK(!cleanup(bytes_of("1e589045558bec1e8ed883ec02a146000b0644007411ff364600"), {}).ok());
    CB_CHECK(!cleanup(bytes_of("90"), {}).ok());
});

CB_TEST("invert.msc6_small.walk_reads_statements_by_the_programs_symbols", [] {
    const Globals longs{{0x44, {"gpA", Width::Long}}, {0x48, {"gpB", Width::Long}}, {0x54, {"gpC", Width::Long}}};
    const Result<Walked> cleaned = walk(bytes_of(kCleanup), longs, "GlobalFree");
    CB_CHECK(cleaned.ok());
    CB_CHECK_EQ(cleaned.value().statements, std::size_t{12});
    CB_CHECK_EQ(cleaned.value().text,
                std::string("typedef unsigned int WORD;\nextern WORD far pascal GlobalFree(WORD);\nextern long gpA;\nextern long gpB;\nextern long gpC;\n\nvoid far F(void)\n{\n"
                            "    if (gpA != 0L) {\n        GlobalFree((WORD)(gpA >> 16));\n        gpA = 0L;\n    }\n"
                            "    if (gpB != 0L) {\n        GlobalFree((WORD)(gpB >> 16));\n        gpB = 0L;\n    }\n"
                            "    if (gpC != 0L) {\n        GlobalFree((WORD)(gpC >> 16));\n        gpC = 0L;\n    }\n}"));
    CB_CHECK_EQ(cleaned.value().globals.size(), std::size_t{3});
    CB_CHECK(cleaned.value().globals[0] == (std::pair<std::string, Width>{"gpA", Width::Long}));

    // a word stored, then a far call with two arguments pushed
    const Result<Walked> stored = walk(bytes_of(std::string(kPrologue) + "c70600001200" "6a05ff76069a00000000" + std::string(kEpilogue) + "ca0400"), {{0, {"cnt", Width::Word}}}, "H");
    CB_CHECK(stored.ok());
    CB_CHECK_EQ(stored.value().text,
                std::string("typedef unsigned int WORD;\nextern WORD far pascal H(WORD);\nextern int cnt;\n\nvoid far F(void)\n{\n    cnt = 18;\n    H(5, a0);\n}"));
    CB_CHECK_EQ(stored.value().statements, std::size_t{2});

    CB_CHECK_EQ(why(walk(bytes_of("909090909090909090909090"), {}).error()), std::string("prologue miss"));
    CB_CHECK_EQ(why(walk(bytes_of(std::string(kPrologue) + "90"), {}).error()), std::string("epilogue miss"));
    CB_CHECK_EQ(why(walk(bytes_of(std::string(kPrologue) + "909090909090" + std::string(kEpilogue) + "cb"), {}).error()), std::string("unmodeled @+0: 909090909090"));
});

CB_TEST("invert.msc6_small.registers_keep_a_calls_result_in_a_register_variable", [] {
    const Result<Registers> r =
        registers(bytes_of("1e589045558bec1e8ed883ec02568b7606569a00000000508bf09a000000008bc65e4d4d8be51f5d4dca0200"), {{0, "G"}, {1, "H"}});
    CB_CHECK(r.ok());
    CB_CHECK_EQ(r.value().text, std::string("typedef unsigned int WORD;\nextern int far pascal G();\nextern int far pascal H();\n\nint far pascal F(int a)\n{\n"
                                            "    register int b;\n    b = G(a);\n    H(b);\n    return b;\n}"));
    CB_CHECK(r.value().saved == (std::vector<std::string>{"si"}));
    CB_CHECK(r.value().locals == (std::vector<std::string>{"b"}));
    CB_CHECK(r.value().statements == (std::vector<std::string>{"b = G(a);", "H(b);", "return b;"}));
    // an instruction the tracker does not model ends it, naming the instruction
    const Result<Registers> calls = registers(bytes_of("1e589045558bec1e8ed883ec0256ff76069a000000008bf00bf675042bc0eb08569a000000008bc65e4d4d8be51f5d4dca020090"), {{0, "G"}, {1, "H"}});
    CB_CHECK(!calls.ok());
    CB_CHECK_EQ(why(calls.error()), std::string("insn unmodeled: or si, si"));
    CB_CHECK_EQ(why(registers(bytes_of("909090909090909090909090")).error()), std::string("prologue miss"));
});

CB_TEST("invert.msc6_small.counted_loop_on_a_register", [] {
    const Result<CountedLoop> r = counted_loop(bytes_of(std::string(kPrologue) + "5657" "8b7e06" "2bf6" "0bff" "7e0b" "56" "9a00000000" "46" "3bf7" "7cf5" "8bc6" "5f5e" + std::string(kEpilogue) + "ca0200"), {{0, "Work"}});
    CB_CHECK(r.ok());
    CB_CHECK_EQ(r.value().text, std::string("typedef unsigned int WORD;\nextern int far pascal Work();\n\nint far pascal F(int a)\n{\n    register int i;\n    for (i = 0; i < a; i++)\n        Work(i);\n    return i;\n}"));
    CB_CHECK_EQ(r.value().limit, std::string("a"));
    CB_CHECK(r.value().body_calls == (std::vector<std::string>{"Work(i)"}));
    CB_CHECK_EQ(why(counted_loop(bytes_of("909090909090909090909090")).error()), std::string("prologue miss"));
    CB_CHECK_EQ(why(counted_loop(bytes_of(std::string(kPrologue) + "5657" "90")).error()), std::string("no di=param"));
});

CB_TEST("invert.msc6_small.mask_zeroes_what_the_linker_fills", [] {
    const Bytes cleaned = bytes_of(kCleanup);
    CB_CHECK_EQ(hex_of(mask(cleaned)),
                std::string("1e589045558bec1e8ed883ec02a100000b0600007411ff3600009a000000002bc0a30000a30000a100000b0600007411ff3600009a000000002bc0a30000a30000"
                            "a100000b0600007411ff3600009a000000002bc0a30000a300004d4d8be51f5d4dcb90"));
    CB_CHECK(mask(Bytes{}).empty());
    const Bytes none = bytes_of("1e589045558bec1e8ed8b80700");
    CB_CHECK_EQ(hex_of(mask(none)), std::string("1e589045558bec1e8ed8b80700"));   // (an immediate load is the program's own value: not masked)
});

// Every function of a real 16-bit module (the Windows 95 LZEXPAND.DLL, 46 of them) through the readers: what the Python's models answered for each --
// the text, or the reason it declined -- and the masked bytes. (The golden is the Python's own run, testdata/msc6_small_lzexpand.json.)
CB_TEST("invert.msc6_small.every_function_of_a_real_module_reads_as_the_python_read_it", [] {
    const Json rows = json::load(testing::data_path("invert/testdata/msc6_small_lzexpand.json"));
    std::size_t leaves = 0, with_registers = 0;
    for (const Json& row : rows) {
        const std::string name = row.at("name").get<std::string>();
        const Bytes code = bytes_of(row.at("code").get<std::string>());
        const auto check = [&](const char* what, const Json& want, const auto& got) {
            if (want.at(0).is_null()) {
                if (got.ok()) testing::record_failure(__FILE__, __LINE__, name + " " + what + ": read, where the Python declined: " + want.at(1).get<std::string>());
                else if (std::string(got.error().what()) != want.at(1).get<std::string>())
                    testing::record_failure(__FILE__, __LINE__, name + " " + what + ": declined with '" + got.error().what() + "', the Python with '" + want.at(1).get<std::string>() + "'");
            } else if (!got.ok()) {
                testing::record_failure(__FILE__, __LINE__, name + " " + what + ": declined with '" + got.error().what() + "', where the Python read it");
            } else if (got.value().text != want.at(0).get<std::string>()) {
                testing::record_failure(__FILE__, __LINE__, name + " " + what + ": read differently:\n" + got.value().text + "\n--- the Python:\n" + want.at(0).get<std::string>());
            }
            testing::count_check();
        };
        const Result<Leaf> leaf_read = leaf(code);
        check("leaf", row.at("leaf"), leaf_read);
        leaves += leaf_read.ok() ? 1 : 0;
        const Result<Registers> registers_read = registers(code, {{0, "G"}, {1, "H"}, {2, "K"}});
        check("registers", row.at("registers"), registers_read);
        with_registers += registers_read.ok() ? 1 : 0;
        check("loop", row.at("loop"), counted_loop(code, {{0, "G"}, {1, "H"}}));
        CB_CHECK_EQ(hex_of(mask(code)), row.at("mask").get<std::string>());
    }
    CB_CHECK_EQ(rows.size(), std::size_t{46});
    CB_CHECK_EQ(leaves, std::size_t{1});
    CB_CHECK_EQ(with_registers, std::size_t{2});
});

// CB_MSC6_SMALL_BENCH=<calls> times each reader on the bytes above (for the old-against-new timing of the report).
CB_TEST("invert.msc6_small.bench", [] {
    const std::optional<std::string> calls = fs::get_env("CB_MSC6_SMALL_BENCH");
    if (!calls || calls->empty()) return;
    const std::size_t n = std::stoul(*calls);
    const Bytes branch = leaf_of("837e0600750" "5b80100eb03b80200"), cleaned = bytes_of(kCleanup);
    const Bytes regs = bytes_of("1e589045558bec1e8ed883ec02568b7606569a00000000508bf09a000000008bc65e4d4d8be51f5d4dca0200");
    const auto time = [&](const char* what, const auto& once) {
        const auto start = std::chrono::steady_clock::now();
        std::size_t sink = 0;
        for (std::size_t i = 0; i < n; ++i) sink += once();
        const double seconds = std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
        std::cout << what << ": " << seconds / static_cast<double>(n) * 1e6 << " us per call (" << sink << ")\n";
    };
    time("leaf", [&] { return leaf(branch).value().text.size(); });
    time("cleanup", [&] { return cleanup(cleaned, {{0x44, "gpA"}, {0x48, "gpB"}, {0x54, "gpC"}}).value().size(); });
    time("walk", [&] { return walk(cleaned, {{0x44, {"gpA", Width::Long}}, {0x48, {"gpB", Width::Long}}, {0x54, {"gpC", Width::Long}}}, "F").value().text.size(); });
    time("registers", [&] { return registers(regs, {{0, "G"}, {1, "H"}}).value().text.size(); });
    time("mask", [&] { return mask(cleaned).size(); });
});
