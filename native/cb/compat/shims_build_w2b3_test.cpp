// Tests of the staged shims of the build lane's third batch (compat/shims/codebreaker/build/{masm,sm83_asm}.hpp): the old API over
// the new modules. Built at /W3 (the old layer is not warning-clean), like shims_build_w1b5_test.cpp. What the masm shim asks of old
// code that is not linked here (the DOS executive, the DOSBox runner, the settings) is compile-checked by including it; its logic is the new
// module's own, tested in cb/build/masm_test.cpp (the entry points are compile-checked in shim_check_w2b3_staged.cpp: they cannot link here).
// sm83_asm needs nothing old and runs.
#include "shims/codebreaker/build/masm.hpp"
#include "shims/codebreaker/build/sm83_asm.hpp"

#include <type_traits>

#include "cb/core/testing.hpp"

namespace old_masm = codebreaker::build::masm;
namespace old_gb = codebreaker::build::sm83_asm;
namespace old_z80 = codebreaker::build::z80_asm;

CB_TEST("compat.shims.build_masm_surface", [] {
    // the selftest source is the module's own
    CB_CHECK(old_masm::SELF_ASM.utf8().starts_with("_TEXT   segment byte public 'CODE'\n"));
    CB_CHECK(old_masm::SELF_ASM.utf8().ends_with("        end\n"));
    // an Assembly and an Assembled are the native_dos shim's
    static_assert(std::is_same_v<old_masm::Assembly, codebreaker::build::native_dos::Assembly>);
    static_assert(std::is_same_v<old_masm::Assembled, codebreaker::build::native_dos::Assembled>);
});

CB_TEST("compat.shims.build_sm83_asm", [] {
    CB_CHECK_EQ(old_gb::BANK, 0x4000);
    const py::bytes rom = old_gb::build(py::str("SECTION \"a\", ROM0[$0]\nnop\nld a,$12\nlp: jp lp"), 8);
    const std::string bytes = rom.raw();
    CB_CHECK_EQ(bytes.size(), 8u);
    CB_CHECK_EQ(static_cast<unsigned char>(bytes[0]), 0x00u);
    CB_CHECK_EQ(static_cast<unsigned char>(bytes[1]), 0x3Eu);
    CB_CHECK_EQ(static_cast<unsigned char>(bytes[2]), 0x12u);
    CB_CHECK_EQ(static_cast<unsigned char>(bytes[3]), 0xC3u);
    CB_CHECK_EQ(static_cast<unsigned char>(bytes[4]), 0x03u);   // lp is at 3
    CB_CHECK_EQ(static_cast<unsigned char>(bytes[7]), 0xFFu);
    // a refused line is the old AsmError ("line N: ..." message), a ValueError of the old layer
    CB_CHECK_THROWS_AS(old_z80::AsmError, (void)old_gb::build(py::str("SECTION \"a\", ROM0[$0]\nbogus a,b"), 8));
    try {
        (void)old_gb::build(py::str("SECTION \"a\", ROM0[$0]\nnop\nbogus a,b"), 8);
        CB_CHECK(false);
    } catch (const old_z80::AsmError& e) {
        CB_CHECK(std::string(e.what()).find("bogus a,b") != std::string::npos);
    }
    // bytes past the ROM; a negative size is an empty ROM
    CB_CHECK_THROWS_AS(old_z80::AsmError, (void)old_gb::build(py::str("SECTION \"a\", ROM0[$0]\nds 9"), 8));
    CB_CHECK_THROWS_AS(old_z80::AsmError, (void)old_gb::build(py::str("SECTION \"a\", ROM0[$0]\nnop"), -4));
    CB_CHECK_EQ(old_gb::build(py::str(""), -4).raw().size(), 0u);
});
