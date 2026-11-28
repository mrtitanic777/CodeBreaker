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
    CB_CHECK_EQ(pe_declined(0x140000D0), std::string("the user operation vectorMultiplyAddFloatingPoint"));          // vmaddfp128
    CB_CHECK_EQ(pe_declined(0x7C0004AC), std::string("the user operation sync"));                                    // sync
    CB_CHECK_EQ(pe_declined(0xFC22193A), std::string("a fused multiply-add"));                                       // fmadd f1,f2,f4,f3
    CB_CHECK_EQ(pe_declined(0x7FFFFB78), std::string("db16cyc"));
    CB_CHECK(pe_declined(0x7C651828) != std::string("(written)"));                                                   // lwarx r3,r5,r3: the reservation
});

CB_TEST("recompile.pcode_emit.a_store_before_eieio_is_left_to_the_recompiler", [] {
    std::string why;
    CB_CHECK(!pe_translator().instruction(0x82000000, 0x90640000, 0x7C0006AC, pe_ctx_names(), &why).has_value());   // stw r3,0(r4); eieio
    CB_CHECK_EQ(why, std::string("a store to memory-mapped I/O (eieio follows)"));
    CB_CHECK(pe_translator().instruction(0x82000000, 0x90640000, 0x60000000, pe_ctx_names()).has_value());
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
