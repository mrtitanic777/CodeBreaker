// cb-unity: alone (file-local names that other files in a batch also use)
// recompile/pcode_emit: an Xbox 360 instruction written as C++ from its p-code, and what it declines. The statements are checked by
// text here; that they compute what the console computes is the instruction tests' proof (recompile.x360.xenias_instruction_tests_*,
// slow), and that titles play is step 3's.
#include <cstdint>
#include <filesystem>
#include <format>
#include <optional>
#include <string>

#include "base/config.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "recompile/pcode_emit.hpp"

namespace cb {
namespace {

namespace pe = recompile::pcode_emit;

std::filesystem::path pe_xenon_sla() {
    std::filesystem::path dir = config::current().checkout_root() / "src" / "decompile" / "engine" / "languages";
    if (const std::optional<std::string> env = fs::get_env("CB_LANGUAGES_DIR"); env && !env->empty()) dir = fs::from_text(*env);
    return dir / "ppc_64_xenonexact_be.sla";
}

// Every register a field of the context, as a title's configuration has it.
pe::Names pe_ctx_names() {
    pe::Names n;
    n.r = [](std::size_t i) { return std::format("ctx.r{}", i); };
    n.f = [](std::size_t i) { return std::format("ctx.f{}", i); };
    n.v = [](std::size_t i) { return std::format("ctx.v{}", i); };
    n.cr = [](std::size_t i) { return std::format("ctx.cr{}", i); };
    n.ctr = [] { return std::string("ctx.ctr"); };
    n.xer = [] { return std::string("ctx.xer"); };
    n.lr = [] { return std::string("ctx.lr"); };
    return n;
}

const pe::Translator& pe_translator() {
    static const pe::Translator t(pe_xenon_sla());
    return t;
}

std::optional<std::string> pe_text(std::uint32_t word, std::string* why = nullptr) {
    const std::optional<pe::Statements> s = pe_translator().instruction(0x82000000, word, 0x60000000, pe_ctx_names(), why);
    if (!s) return std::nullopt;
    return s->text;
}

std::string pe_declined(std::uint32_t word) {
    std::string why;
    return pe_text(word, &why) ? std::string("(written)") : why;
}

}  // namespace

CB_TEST("recompile.pcode_emit.integer_instructions_are_statements_on_the_context", [] {
    if (fs::get_env("CB_PCODE_EMIT_DUMP")) {
        for (std::uint32_t w : {0x7C642A14u, 0x80640008u, 0x9421FFF0u, 0x2F030000u, 0x7C6803A6u, 0x7C6802A6u, 0x7C632215u, 0x5463103Au, 0xFC22182Au, 0x7C641838u})
            std::printf("%08X\n%s\n", w, pe_text(w).value_or("(declined: " + pe_declined(w) + ")\n").c_str());
    }
    CB_CHECK_EQ(pe_text(0x7C642A14).value_or("?"), std::string("\tctx.r3.u64 = uint64_t(uint64_t(ctx.r4.u64) + ctx.r5.u64);\n"));   // add r3,r4,r5
    CB_CHECK_EQ(pe_text(0x7C6803A6).value_or("?"), std::string("\tctx.lr = ctx.r3.u64;\n"));                                         // mtlr r3
});

CB_TEST("recompile.pcode_emit.what_the_recompiler_writes_itself_is_declined", [] {
    CB_CHECK_EQ(pe_declined(0x48000101), std::string("a call"));                                                     // bl
    CB_CHECK_EQ(pe_declined(0x4E800020), std::string("a return"));                                                   // blr
    CB_CHECK_EQ(pe_declined(0x4E800420), std::string("an indirect branch"));                                         // bctr
    CB_CHECK_EQ(pe_declined(0x419A0010), std::string("a branch"));                                                   // beq cr6
    CB_CHECK_EQ(pe_declined(0x10000502), std::string("the user operation vectorAverageSignedByte"));          // vmaddfp128
    CB_CHECK(pe_declined(0x7C651828) != std::string("(written)"));                                                   // lwarx r3,r5,r3: the reservation
    CB_CHECK(pe_declined(0xFDFE0D8E) != std::string("(written)"));   // mtfsf 0xFF,f1: the FPSCR's controls are ctx.fpscr's (the rounding mode)
    CB_CHECK(pe_declined(0xFC00010C) != std::string("(written)"));   // mtfsfi cr0,0
});

// rldimi's mask is a value its subtable computes and exports as a cell of the constant space: the cell's value is its address, never a
// load from guest memory (Minecraft read address 0xFFFFFFFF00000000 for rldimi r24,r9,32,0 until this was so).
CB_TEST("recompile.pcode_emit.a_cell_of_the_constant_space_is_its_address", [] {
    const std::optional<std::string> t = pe_text(0x7938000E);   // rldimi r24,r9,32,0
    CB_CHECK(t.has_value());
    if (!t) return;
    CB_CHECK(t->find("PPC_LOAD") == std::string::npos);
    CB_CHECK(t->find("0xFFFFFFFF00000000") != std::string::npos);
});

// With the recompiler's ways out, a branch, call or return is written: the condition from the p-code, the target as the recompiler names it.
CB_TEST("recompile.pcode_emit.the_ways_out_are_the_recompilers", [] {
    pe::Names n = pe_ctx_names();
    n.jump = [](std::uint32_t t) { return std::format("\tgoto loc_{:X};\n", t); };
    n.call = [](std::uint32_t t) { return std::format("\tsub_{:X}(ctx, base);\n", t); };
    n.call_indirect = [](const std::string& v) { return std::format("\tPPC_CALL_INDIRECT_FUNC({});\n", v); };
    n.jump_indirect = [](const std::string& v) { return std::format("\tPPC_CALL_INDIRECT_FUNC({});\n\treturn;\n", v); };
    const auto text = [&](std::uint32_t w) { return pe_translator().instruction(0x82000000, w, 0x60000000, n).value_or(pe::Statements{"(declined)"}).text; };
    CB_CHECK(text(0x48000101).find("sub_82000100(ctx, base);") != std::string::npos);   // bl
    CB_CHECK(text(0x48000101).find("ctx.lr = ") != std::string::npos);
    CB_CHECK(text(0x4E800020).find("return;") != std::string::npos);                    // blr
    CB_CHECK(text(0x4E400020).find("return;") != std::string::npos);                    // bdzlr: a branch through LR is a return
    CB_CHECK(text(0x4E400020).find("PPC_CALL_INDIRECT_FUNC") == std::string::npos);
    CB_CHECK(text(0x4E800420).find("PPC_CALL_INDIRECT_FUNC(uint32_t(ctx.ctr.u64))") != std::string::npos);   // bctr
    CB_CHECK(text(0x419A0010).find("goto loc_82000010;") != std::string::npos);         // beq cr6
    CB_CHECK(text(0x4200FFF8).find("goto loc_81FFFFF8;") != std::string::npos);         // bdnz -8
    CB_CHECK(text(0x4200FFF8).find("uint32_t") != std::string::npos);                   // (the low word of CTR is tested)
});

// The user operations of the Xenon's description (ppc_64_xenonexact_be): the fused multiply-adds round once (XenonRecomp's helpers, the
// console's results), the reservation is a compare and exchange, the barriers and hints are nothing, db16cyc the runtime's spin hint.
CB_TEST("recompile.pcode_emit.the_user_operations_are_the_runtimes", [] {
    pe::Names n = pe_ctx_names();
    n.reserved = [] { return std::string("ctx.reserved"); };
    const auto text = [&](std::uint32_t w) { return pe_translator().instruction(0x82000000, w, 0x60000000, n).value_or(pe::Statements{"(declined)"}).text; };
    CB_CHECK(text(0xFC22193A).find("ppc_fmadd(") != std::string::npos);                     // fmadd f1,f2,f4,f3
    CB_CHECK(text(0xEC22193E).find("ppc_fmadds(") != std::string::npos);                    // fnmadds f1,f2,f4,f3
    CB_CHECK(text(0xEC22193E).find(", true)") != std::string::npos);
    CB_CHECK(text(0x7C651828).find("ctx.reserved.u32 = *(uint32_t*)PPC_HOST(") != std::string::npos);   // lwarx r3,r5,r3
    CB_CHECK(text(0x7C65192D).find("__sync_bool_compare_and_swap(") != std::string::npos);  // stwcx. r3,r5,r3
    CB_CHECK(text(0x7C0004AC).find("(declined)") == std::string::npos);                     // sync: nothing to do
    CB_CHECK_EQ(text(0x7FFFFB78), std::string("\tPPC_SPIN_HINT();\n"));                     // db16cyc
    CB_CHECK(text(0x7C6C42E6).find("PPC_QUERY_TIMEBASE()") != std::string::npos);           // mftb r3
    // a vector operation: XenonRecomp's SIMDe form on the context's vectors, flagged for the VMX flush mode
    const std::optional<pe::Statements> v = pe_translator().instruction(0x82000000, 0x140000D0, 0x60000000, n);   // vmaddfp128 v0,v0,v0,v0
    CB_CHECK(v.has_value() && v->vmx && v->text.find("simde_mm_add_ps(simde_mm_mul_ps(") != std::string::npos);
});

CB_TEST("recompile.pcode_emit.a_store_before_eieio_goes_to_memory_mapped_io", [] {
    const std::optional<pe::Statements> mm = pe_translator().instruction(0x82000000, 0x90640000, 0x7C0006AC, pe_ctx_names());   // stw r3,0(r4); eieio
    CB_CHECK(mm.has_value() && mm->text.find("PPC_MM_STORE_U32(") != std::string::npos);
    const std::optional<pe::Statements> plain = pe_translator().instruction(0x82000000, 0x90640000, 0x60000000, pe_ctx_names());
    CB_CHECK(plain.has_value() && plain->text.find("PPC_STORE_U32(") != std::string::npos && plain->text.find("MM_") == std::string::npos);
});

CB_TEST("recompile.pcode_emit.floating_point_says_so_and_drops_the_fpscr_bits", [] {
    const std::optional<pe::Statements> s = pe_translator().instruction(0x82000000, 0xFC22182A, 0x60000000, pe_ctx_names());   // fadd f1,f2,f3
    CB_CHECK(s.has_value());
    if (!s) return;
    CB_CHECK(s->fpu);
    CB_CHECK(s->text.find("ctx.f1.u64 = ") != std::string::npos);
    CB_CHECK(s->text.find("fp_") == std::string::npos);
});

}  // namespace cb
