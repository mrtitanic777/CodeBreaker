// The shims at the old header paths of recompile/builders/floating and source/ne_asm/run keep the old API over the new modules. Builds at
// /W3 (the old py:: layer is not warning-clean). The other shims of this batch (disassemble, x360project, placement, assembly, segdiff)
// call old ports that cannot be linked into cb_tests; shim_check_recompile_source.cpp compiles them.
#include "codebreaker/recompile/builders/floating.hpp"
#include "codebreaker/source/ne_asm/run.hpp"

#include "cb/core/testing.hpp"

namespace old_floating = codebreaker::recompile::builders::floating;
namespace old_run = codebreaker::source::ne_asm::run;

namespace {
std::optional<int64_t> number_after(const py::str& token, const char* prefix) {
    const std::string t = token.utf8(), p = prefix;
    if (t.rfind(p, 0) != 0) return std::nullopt;
    return std::stoll(t.substr(p.size()));
}
}  // namespace

CB_TEST("compat.recompile_shims.floating_emit_keeps_the_old_signature", [] {
    const old_floating::OperandNumber freg = [](const py::str& t) { return number_after(t, "f"); };
    const old_floating::OperandNumber cr = [](const py::str& t) { return number_after(t, "cr"); };
    const old_floating::OperandNumber none;
    const old_floating::MemOperand mem;
    const old_floating::RegisterValue rv;
    const old_floating::BaseExpression base;
    const old_floating::AddressExpression addr;
    const auto emitted = old_floating::emit(py::str("fmadd"), py::list<py::str>{py::str("f1"), py::str("f2"), py::str("f3"), py::str("f4")}, none, none,
                                            freg, none, cr, mem, rv, base, addr, 0x82000000, 4);
    CB_CHECK(emitted.has_value());
    CB_CHECK_EQ(emitted->size(), 2);
    CB_CHECK_EQ((*emitted)[0].utf8(), "ctx.fpscr.disableFlushMode();");
    CB_CHECK_EQ((*emitted)[1].utf8(), "ctx.f1.f64 = ctx.f2.f64 * ctx.f3.f64 + ctx.f4.f64;");
    CB_CHECK(!old_floating::emit(py::str("add"), py::list<py::str>{py::str("r3")}, none, none, freg, none, cr, mem, rv, base, addr, 0, 4).has_value());
    CB_CHECK_EQ(old_floating::MNEMONICS.size(), 32);
    CB_CHECK_EQ(old_floating::MNEMONICS[0].utf8(), "fabs");
    CB_CHECK_EQ(old_floating::RUNTIME_HELPERS.utf8(), "#include <cmath>\n#include <climits>\n");
    // an operand that is not a register is an error the old callers see as a py exception
    bool threw = false;
    try {
        (void)old_floating::emit(py::str("fmr"), py::list<py::str>{py::str("r3"), py::str("r4")}, none, none, freg, none, cr, mem, rv, base, addr, 0, 4);
    } catch (...) {
        threw = true;
    }
    CB_CHECK(threw);
});

CB_TEST("compat.recompile_shims.ne_asm_run_dispatches_to_modules", [] {
    std::string module_run;
    int64_t extra = 0;
    const old_run::RunModule run_module = [&](const py::str& module, const py::list<py::str>& args) {
        module_run = module.utf8();
        extra = args.size();
        return int64_t{5};
    };
    CB_CHECK_EQ(old_run::main(py::list<py::str>{py::str("segdiff"), py::str("x.c"), py::str("--seg"), py::str("1")}, run_module), 5);
    CB_CHECK_EQ(module_run, "codebreaker.source.ne_asm.segdiff");
    CB_CHECK_EQ(extra, 3);
    CB_CHECK_EQ(old_run::TOOLS.size(), 11);
    CB_CHECK(old_run::DOC.utf8().find("codebreaker ne-asm <tool>") != std::string::npos);
});
