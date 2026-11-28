// cb-unity: alone (file-local names that other files in a batch also use)
// The SLEIGH translator must give an instruction or a bad-instruction report for every word, never fault. Two descriptions had constructors that named a display-only table
// (one that exports nothing) as a value in their p-code: AARCH64's BFDOT, SUDOT and USDOT "by element" (their element operand) and mipsmicro.sinc's DI, EI, MFC0, MTC0, SYNC,
// DVP, EVP, BC2F and BC2T (their selector, stype and condition-code operands, reached when ISA_MODE is 1, microMIPS). The translator then read the operand's handle, which a
// reused parser state still held from an earlier decode, and dereferenced an address space that was null (or wild): an access violation inside oneInstruction on about one word in
// 200,000 of AArch64 (e.g. 0x4fa0f944, 0x4f82fbcb, 0x0fb2f80c) and on mips64le words such as 0x9eac42b4 once a jump had set ISA_MODE.
//
// Fixed in three places: the descriptions export the value the p-code uses (AARCH64instructions.sinc `Re_VPR128.2H.vIndexHL32`, `Re_VPR128.4B.vIndexHL32`; mipsmicro.sinc DIDISP,
// CPSEL, STYPE, COP2CC; ARMv8.sinc VPmask), the translator clears the handle of a constructor that exports nothing and refuses a varnode that has no address space (a
// BadDataError, so a decoder reports a bad instruction), and the SLEIGH compiler rejects the pattern ("Operand 'x' is used as a value but its table 'x' exports nothing").
// These tests run the words that crashed and then thousands of random ones through each description's translator, by the reader (which builds every instruction's p-code).
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <memory>
#include <optional>
#include <random>
#include <string>
#include <vector>

#include "base/config.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "decompile/engine/pcode_emulator.hh"
#include "decompile/engine/sleigh_reader.hh"

namespace cb {
namespace {

namespace sl = decompile::sleigh;

std::filesystem::path languages_dir() {
    if (const std::optional<std::string> env = fs::get_env("CB_LANGUAGES_DIR"); env && !env->empty()) return fs::from_text(*env);
    return config::current().checkout_root() / "src" / "decompile" / "engine" / "languages";
}

std::vector<std::uint8_t> le_bytes(std::uint32_t w) {
    return {static_cast<std::uint8_t>(w), static_cast<std::uint8_t>(w >> 8), static_cast<std::uint8_t>(w >> 16), static_cast<std::uint8_t>(w >> 24)};
}

// The words the survey of 8 million random AArch64 words found the translator faulting on (written as the bytes it printed, in memory order).
const std::uint32_t kAarch64Crashed[] = {
    0x0f8bf001, 0x0fa5f107, 0x4fadf807, 0x4f45f108, 0x0f66f925, 0x0f55f82c, 0x0fb0f02d, 0x4faff03e, 0x4f71f146, 0x0f96f847, 0x0f8df848, 0x0f66f84b,
    0x4f62f059, 0x0f5cf95b, 0x4f9cf274, 0x4f88f880, 0x0f7ef981, 0x0f9efa8d, 0x0fa4f993, 0x0f48f296, 0x4f99f39d, 0x4f81f0a2, 0x0fa8f2a5, 0x0faef3b2,
    0x0f94fab7, 0x0f99f8ba, 0x4f9ff0c4, 0x0fbefada, 0x0fbcf1db, 0x0f95f9dd, 0x4f51f2e5, 0x0f74f2e7, 0x4fbef9ec, 0x0f9ef3f0, 0x4f80f1f6, 0x0f58fbfa,
    0x4fa0f944, 0x4f82fbcb, 0x0fb2f80c,
};

// By-element dot products of 8 bit and 16 bit groups: what the words above are.
bool is_dot(const std::string& mnemonic) { return mnemonic == "usdot" || mnemonic == "sudot" || mnemonic == "bfdot"; }

}  // namespace

CB_TEST("emulate.pcode_translator.aarch64_words_that_crashed_decode_as_dot_products_by_element", [] {
    const sl::Reader reader(languages_dir() / "AARCH64.sla", {});
    for (const std::uint32_t w : kAarch64Crashed) {
        const std::vector<std::uint8_t> code = le_bytes(w);
        const std::vector<sl::Instruction> got = reader.read(code.data(), code.size(), 0x1000, 1);
        CB_CHECK_EQ(got.size(), std::size_t{1});
        if (got.empty()) continue;
        CB_CHECK_EQ(got[0].length, std::size_t{4});
        if (!is_dot(got[0].mnemonic)) std::printf("  %08x read as %s %s\n", w, got[0].mnemonic.c_str(), got[0].operands.c_str());
        CB_CHECK(is_dot(got[0].mnemonic));
    }
    // The element is a group of Vm: V<m>.4B[index] for USDOT and SUDOT, V<m>.2H[index] for BFDOT, with index H:L (0 to 3) and Rm = M:Rm.
    const auto text = [&](std::uint32_t w) {
        const std::vector<std::uint8_t> code = le_bytes(w);
        const std::vector<sl::Instruction> got = reader.read(code.data(), code.size(), 0x1000, 1);
        return got.empty() ? std::string() : got[0].mnemonic + " " + got[0].operands;
    };
    CB_CHECK_EQ(text(0x4fa0f944), std::string("usdot v4.4S, v10.16B, v0.4B[0x3]"));
    CB_CHECK_EQ(text(0x4f80f000), std::string("usdot v0.4S, v0.16B, v0.4B[0x0]"));
    CB_CHECK_EQ(text(0x0f80f800), std::string("usdot v0.2S, v0.8B, v0.4B[0x2]"));
    CB_CHECK_EQ(text(0x4f00f000), std::string("sudot v0.4S, v0.16B, v0.4B[0x0]"));   // (SUDOT by element had no encoding at all before)
    CB_CHECK_EQ(text(0x4f40f000), std::string("bfdot v0.4S, v0.8H, v0.2H[0x0]"));
    CB_CHECK_EQ(text(0x0f40f800), std::string("bfdot v0.2S, v0.4H, v0.2H[0x2]"));
    CB_CHECK_EQ(text(0x4f70f9ed), std::string("bfdot v13.4S, v15.8H, v16.2H[0x3]"));   // Rm = M:Rm = 16
});

// Random words through the translator: the reader decodes each one and builds its p-code. A fault ends the test run, so passing is the check.
CB_TEST("emulate.pcode_translator.random_words_never_fault", [] {
    struct Case {
        const char* sla;
        std::vector<std::pair<std::string, int>> context;
        bool big_endian;
    };
    const Case cases[] = {
        {"AARCH64.sla", {}, false},
        {"mips64le.sla", {}, false},
        {"mips64le.sla", {{"ISA_MODE", 1}}, false},   // microMIPS: the mode the unexported operands were reached in
        {"mips32le.sla", {{"ISA_MODE", 1}}, false},
        {"mips64be.sla", {{"ISA_MODE", 1}}, true},
        {"ARM7_le.sla", {{"TMode", 1}}, false},
    };
    std::mt19937_64 rng(20261201);
    for (const Case& c : cases) {
        const sl::Reader reader(languages_dir() / c.sla, c.context);
        std::uint64_t decoded = 0;
        for (int i = 0; i < 40000; ++i) {
            const std::uint32_t w = static_cast<std::uint32_t>(rng());
            std::vector<std::uint8_t> code = le_bytes(w);
            if (c.big_endian) code = {code[3], code[2], code[1], code[0]};
            code.resize(16, 0);
            const std::vector<sl::Instruction> got = reader.read(code.data(), code.size(), 0x1000, 1);
            if (!got.empty() && got[0].length != 0) ++decoded;
        }
        CB_CHECK(decoded > 1000);
    }
});

// The same through the emulator: a bad instruction is a status, an instruction that decodes runs or reports an error, nothing faults.
CB_TEST("emulate.pcode_translator.the_emulator_steps_over_words_that_crashed", [] {
    sl::Emulator a64(languages_dir() / "AARCH64.sla", {});
    for (const std::uint32_t w : kAarch64Crashed) {
        const std::vector<std::uint8_t> code = le_bytes(w);
        a64.write(0x1000, code.data(), code.size());
        a64.setPc(0x1000);
        (void)a64.step();
    }
    sl::Emulator micro(languages_dir() / "mips64le.sla", {{"ISA_MODE", 1}});
    const std::uint32_t micro_words[] = {0x9eac42b4, 0x52e1a842, 0x42b4d1e0, 0x2402b442, 0x4f0f4280};
    for (const std::uint32_t w : micro_words) {
        const std::vector<std::uint8_t> code = le_bytes(w);
        micro.write(0x1000, code.data(), code.size());
        micro.setPc(0x1000);
        (void)micro.step();
    }
    CB_CHECK(true);
});

// The words of mips64le that crashed the translator once a jump had set ISA_MODE (microMIPS): as the survey printed them, in memory order.
CB_TEST("emulate.pcode_translator.micromips_words_that_crashed_decode_or_are_rejected", [] {
    const char* words[] = {"b442ac9e", "80420f4f", "80423ef0", "80428a5e", "844267ce", "8442d30a", "88420833", "88428cb8", "8c4211e2", "8c42302a", "90427c2a", "9042cd33",
                           "94426356", "94428ccc", "9842074c", "98426c93", "9842cf70", "9842ff02", "9c42123c", "9c421c91", "9c422ade", "9c4274cf", "a0425ad0", "a4421dd7",
                           "a4426852", "a8422980", "a8422ad4", "a8428bf6", "a842c3c9", "a842f4e0", "ac42d941", "ac42fc95", "b04220c7", "b442501c", "b442dcbd", "b8421826",
                           "b8429a3a", "b842c5df", "bc42bfd5", "bc42e766", "d101fc32"};
    for (const char* arch : {"mips64le.sla", "mips32le.sla"}) {
        const sl::Reader reader(languages_dir() / arch, {{"ISA_MODE", 1}});
        sl::Emulator emu(languages_dir() / arch, {{"ISA_MODE", 1}});
        for (const char* w : words) {
            std::vector<std::uint8_t> code(16, 0);
            for (int i = 0; i < 4; ++i) code[static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(std::stoul(std::string(w + 2 * i, 2), nullptr, 16));
            const std::vector<sl::Instruction> got = reader.read(code.data(), code.size(), 0x1000, 1);
            CB_CHECK_EQ(got.size(), std::size_t{1});
            emu.write(0x1000, code.data(), code.size());
            emu.setPc(0x1000);
            const sl::Emulator::Status status = emu.step();
            CB_CHECK(status == sl::Emulator::Status::Ok || status == sl::Emulator::Status::BadInstruction || status == sl::Emulator::Status::Error);
        }
    }
});

// A description that names a table which exports nothing as a value (the defect behind the crashes: the SLEIGH compiler refuses it now, so src/engine/test_data/exportless.sla
// was made with a copy of the compiler without that check): the translator must say "bad instruction" for the words that use it, however the parse state was left by the
// decodes before, and go on decoding the others.
CB_TEST("emulate.pcode_translator.a_description_that_names_an_exportless_table_as_a_value_is_a_bad_instruction", [] {
    const std::filesystem::path sla = config::current().checkout_root() / "src" / "engine" / "test_data" / "exportless.sla";
    sl::Emulator emu(sla, {});
    std::mt19937 rng(5);
    const std::uint8_t bytes[] = {0x21, 0x01, 0x11, 0x02};   // good, bad m0, bad m1, fine
    for (int i = 0; i < 400; ++i) {
        const std::uint8_t b = bytes[rng() % 4];
        emu.write(0x100, &b, 1);
        emu.setPc(0x100);
        const sl::Emulator::Status status = emu.step();
        const bool bad = b == 0x01 || b == 0x11;
        CB_CHECK(status == (bad ? sl::Emulator::Status::BadInstruction : sl::Emulator::Status::Ok));
    }
    const sl::Reader reader(sla, {});
    for (const std::uint8_t b : bytes) {
        const std::uint8_t code[2] = {b, 0};
        const std::vector<sl::Instruction> got = reader.read(code, 2, 0x100, 1);
        CB_CHECK_EQ(got.size(), std::size_t{1});
    }
});

}  // namespace cb
