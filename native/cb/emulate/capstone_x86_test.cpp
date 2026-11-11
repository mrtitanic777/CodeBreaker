// cb/emulate/capstone_x86_test.cpp -- the per-thread x86 engines decode in the width asked for.
#include "cb/emulate/capstone_x86.hpp"

#include <string>
#include <thread>

#include "cb/core/capstone_api.hpp"
#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::emulate;

namespace {

// The first instruction of `code`: its text and length ("" when it does not decode).
std::pair<std::string, std::size_t> first(csh h, const std::uint8_t* code, std::size_t n) {
    const auto& cs = disasm::capstone::api();
    cs_insn* insn = nullptr;
    const std::size_t count = cs.cs_disasm(h, code, n, 0x1000, 1, &insn);
    if (count == 0) return {"", 0};
    std::string text = std::string(insn[0].mnemonic) + " " + insn[0].op_str;
    const std::size_t size = insn[0].size;
    cs.cs_free(insn, count);
    return {text, size};
}

}  // namespace

CB_TEST("emulate.capstone_x86.decodes_in_the_width_asked_for", [] {
    const std::uint8_t code[] = {0x40, 0x90, 0x90, 0x90};    // inc eax (16/32-bit mode) / REX prefix (64-bit)
    auto [t32, n32] = first(capstone_x86::engine(32, false), code, sizeof code);
    CB_CHECK_EQ(t32, "inc eax");
    CB_CHECK_EQ(n32, 1u);
    auto [t16, n16] = first(capstone_x86::engine(16, false), code, sizeof code);
    CB_CHECK_EQ(t16, "inc ax");
    auto [t64, n64] = first(capstone_x86::engine(64, false), code, sizeof code);
    CB_CHECK_EQ(t64, "nop ");                                  // 40 90 = REX nop
    CB_CHECK_EQ(n64, 2u);
});

CB_TEST("emulate.capstone_x86.detail_gives_operands", [] {
    const std::uint8_t code[] = {0x8B, 0x45, 0x08};            // mov eax, [ebp+8]
    const auto& cs = disasm::capstone::api();
    for (bool detail : {false, true}) {
        csh h = capstone_x86::engine(32, detail);
        cs_insn* insn = nullptr;
        const std::size_t count = cs.cs_disasm(h, code, sizeof code, 0, 1, &insn);
        CB_CHECK_EQ(count, 1u);
        CB_CHECK_EQ(insn[0].detail != nullptr, detail);
        if (detail) CB_CHECK_EQ(insn[0].detail->x86.op_count, 2);
        cs.cs_free(insn, count);
    }
});

CB_TEST("emulate.capstone_x86.one_engine_per_kind_per_thread", [] {
    CB_CHECK_EQ(capstone_x86::engine(32, false), capstone_x86::engine(32, false));
    CB_CHECK_NE(capstone_x86::engine(32, false), capstone_x86::engine(32, true));
    CB_CHECK_NE(capstone_x86::engine(32, false), capstone_x86::engine(16, false));
    const std::size_t mine = capstone_x86::engine(32, false);
    std::size_t theirs = 0;
    std::string text;
    std::thread t([&] {
        theirs = capstone_x86::engine(32, false);
        const std::uint8_t code[] = {0x90};
        text = first(theirs, code, 1).first;                   // another thread's engine decodes on its own
    });
    t.join();
    CB_CHECK_EQ(text, "nop ");
    CB_CHECK_NE(theirs, mine);
});

CB_TEST("emulate.capstone_x86.rejects_other_widths", [] {
    CB_CHECK_THROWS_AS(UsageError, (void)capstone_x86::engine(8, false));
    CB_CHECK_THROWS_AS(UsageError, (void)capstone_x86::engine(33, true));
    CB_CHECK_THROWS_AS(UsageError, (void)capstone_x86::engine(0, false));
});

CB_TEST("emulate.capstone_x86.names_its_dll", [] {
    CB_CHECK(!capstone_x86::dll_path().empty());
    CB_CHECK_EQ(capstone_x86::dll_path().filename().string(), "capstone.dll");
});
