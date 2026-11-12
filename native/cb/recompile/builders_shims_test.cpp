// The shims at the old header paths (native/typed/cpp/codebreaker/recompile/builders/{arith,logical,memory,system,vector}.hpp) over the
// new builders: they must compile with the old layer and forward to the new code. Built at /W3 (the old layer is not warning-clean).
#include <optional>
#include <string>

#include "cb/core/testing.hpp"
#include "codebreaker/recompile/builders/arith.hpp"
#include "codebreaker/recompile/builders/logical.hpp"
#include "codebreaker/recompile/builders/memory.hpp"
#include "codebreaker/recompile/builders/system.hpp"
#include "codebreaker/recompile/builders/vector.hpp"

namespace {

namespace bld = codebreaker::recompile::builders;
using cb::compat::from_py;
using cb::compat::to_py;

// powerpc.py's operand helpers, for the tokens "r3" "f4" "v5" "cr6" "0x10" and "-0x70(r1)"
std::optional<int64_t> number_after(const py::str& token, const char* prefix) {
    const std::string t = from_py(token);
    const std::string p = prefix;
    if (t.size() <= p.size() || t.compare(0, p.size(), p) != 0) return std::nullopt;
    int64_t n = 0;
    for (std::size_t i = p.size(); i < t.size(); ++i) {
        if (t[i] < '0' || t[i] > '9') return std::nullopt;
        n = n * 10 + (t[i] - '0');
    }
    return n;
}

const bld::floating::OperandNumber g_reg = [](const py::str& t) { return number_after(t, "r"); };
const bld::floating::OperandNumber g_vreg = [](const py::str& t) { return number_after(t, "v"); };
const bld::floating::OperandNumber g_freg = [](const py::str& t) { return number_after(t, "f"); };
const bld::floating::OperandNumber g_cr = [](const py::str& t) { return number_after(t, "cr"); };
const bld::floating::OperandNumber g_imm = [](const py::str& t) -> std::optional<int64_t> {
    try {
        return std::stoll(from_py(t), nullptr, 0);
    } catch (...) {   // (a token that is not a number)
        return std::nullopt;
    }
};
const bld::floating::MemOperand g_mem = [](const py::str& t) -> std::optional<std::tuple<int64_t, int64_t>> {
    const std::string s = from_py(t);
    const std::size_t open = s.find('(');
    if (open == std::string::npos) return std::nullopt;
    const auto base = number_after(to_py(s.substr(open + 1, s.size() - open - 2)), "r");
    if (!base) return std::nullopt;
    return std::make_tuple(std::stoll(s.substr(0, open), nullptr, 0), *base);
};
const bld::floating::RegisterValue g_rv = [](int64_t n, const py::str& field) { return to_py("ctx.r" + std::to_string(n) + "." + from_py(field)); };
const bld::logical::RegisterValue g_rv_opt = [](const std::optional<int64_t>& n, const py::str& field) {
    return to_py("ctx.r" + (n ? std::to_string(*n) : std::string("None")) + "." + from_py(field));
};
const bld::floating::BaseExpression g_base = [](int64_t b) { return to_py(b == 0 ? "0" : "ctx.r" + std::to_string(b) + ".u32"); };
const bld::floating::AddressExpression g_addr = [](int64_t d, int64_t b) { return to_py(std::to_string(d) + " + " + std::to_string(b)); };

py::list<py::str> tokens(std::initializer_list<const char*> items) {
    py::list<py::str> out;
    for (const char* i : items) out.append(py::str(i));
    return out;
}

std::vector<std::string> plain(const std::optional<py::list<py::str>>& statements) {
    std::vector<std::string> out;
    if (!statements) return {"<none>"};
    for (const py::str& s : *statements) out.push_back(from_py(s));
    return out;
}

}  // namespace

CB_TEST("recompile.shims.builders_arith_and_logical", [] {
    CB_CHECK(plain(bld::arith::emit(py::str("add"), tokens({"r3", "r4", "r5"}), g_reg, g_vreg, g_freg, g_imm, g_cr, g_mem, g_rv, g_base, g_addr, 0, 0)) ==
             std::vector<std::string>{"ctx.r3.u64 = ctx.r4.u64 + ctx.r5.u64;"});
    CB_CHECK(plain(bld::arith::emit(py::str("ori"), tokens({"r3", "r4", "5"}), g_reg, g_vreg, g_freg, g_imm, g_cr, g_mem, g_rv, g_base, g_addr, 0, 0)) ==
             std::vector<std::string>{"<none>"});
    CB_CHECK(plain(bld::logical::emit(py::str("ori"), tokens({"r3", "r4", "5"}), g_reg, g_vreg, g_freg, g_imm, g_cr, g_mem, g_rv_opt, g_base, g_addr, 0, 0)) ==
             std::vector<std::string>{"ctx.r3.u64 = ctx.r4.u64 | 5;"});
    CB_CHECK_EQ(bld::arith::MNEMONICS.size(), 34);
    CB_CHECK_EQ(bld::logical::MNEMONICS.size(), 41);
    CB_CHECK_EQ(bld::logical::_compute_mask(32, 63), 0xFFFFFFFFull);
    CB_CHECK_EQ(from_py(bld::arith::RUNTIME_HELPERS), "");
    // a token the helper cannot read is an error (the old port wrote "None" into the statement)
    CB_CHECK_THROWS_AS(py::ValueError, (void)bld::arith::emit(py::str("add"), tokens({"r3", "r4", "v5"}), g_reg, g_vreg, g_freg, g_imm, g_cr, g_mem, g_rv, g_base, g_addr, 0, 0));
});

CB_TEST("recompile.shims.builders_memory_system_vector", [] {
    CB_CHECK(plain(bld::memory::emit(py::str("lwz"), tokens({"r3", "-0x70(r1)"}), g_reg, g_vreg, g_freg, g_imm, g_cr, g_mem, g_rv_opt, g_base, g_addr, 0, 0)) ==
             std::vector<std::string>{"ctx.r3.u64 = PPC_LOAD_U32(ctx.r1.u32 + -112);"});
    CB_CHECK(from_py(bld::memory::RUNTIME_HELPERS).find("VectorMaskL") != std::string::npos);
    CB_CHECK_EQ(bld::memory::MNEMONICS.size(), 91);
    // the trap decodes the raw word (the last argument): tweqi r3, 5
    const int64_t tweqi = (3 << 26) | (4 << 21) | (3 << 16) | 5;
    CB_CHECK(plain(bld::system::emit(py::str("tweqi"), tokens({}), g_reg, g_vreg, g_freg, g_imm, g_cr, g_mem, g_rv_opt, g_base, g_addr, 0, tweqi)) ==
             std::vector<std::string>{"if (ctx.r3.s32 == 5) ppc_trap(ctx, base, 0);"});
    // a barrier emits nothing, and is still the builder's
    CB_CHECK(plain(bld::system::emit(py::str("sync"), tokens({}), g_reg, g_vreg, g_freg, g_imm, g_cr, g_mem, g_rv_opt, g_base, g_addr, 0, 0)).empty());
    CB_CHECK_EQ(bld::system::MNEMONICS.size(), 95);
    CB_CHECK_EQ(plain(bld::vector::emit(py::str("vaddfp"), tokens({"v1", "v2", "v3"}), g_reg, g_vreg, g_freg, g_imm, g_cr, g_mem, g_rv, g_base, g_addr, 0, 0)).size(), 2u);
    CB_CHECK_EQ(bld::vector::MNEMONICS.size(), 172);
    CB_CHECK(from_py(bld::vector::RUNTIME_HELPERS).find("simde_mm_vctsxs") != std::string::npos);
});
