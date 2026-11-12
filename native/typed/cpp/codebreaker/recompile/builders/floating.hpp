// codebreaker/recompile/builders/floating.hpp -- SHIM over cb/recompile/builders/floating.hpp (docs/CPP_DESIGN.md 6.3): the old
// signature in (operand readers as std::functions over py::str), the new builder called, the statements back as a py::list.
// Deleted with its last py:: consumer (recompile/powerpc and the other builders).
//
// What the builder is: scalar floating point -- arithmetic, multiply-add, sign, conversion, compare, select -- as C++ for the
// recompiler; see cb/recompile/builders/floating.hpp.
#pragma once

#include <cstdint>
#include <functional>
#include <span>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/recompile/builders/floating.hpp"

namespace codebreaker::recompile::builders::floating {

// The operand helpers recompile/powerpc.py passes in.
using OperandNumber = std::function<std::optional<int64_t>(const py::str&)>;
using MemOperand = std::function<std::optional<std::tuple<int64_t, int64_t>>(const py::str&)>;
using RegisterValue = std::function<py::str(int64_t, const py::str&)>;
using BaseExpression = std::function<py::str(int64_t)>;
using AddressExpression = std::function<py::str(int64_t, int64_t)>;

namespace shim_ {

// The new builders' Operands over the old helpers: conversion only.
class Helpers : public cb::recompile::builders::Operands {
public:
    Helpers(const OperandNumber& reg, const OperandNumber& vreg, const OperandNumber& freg, const OperandNumber& imm, const OperandNumber& cr,
            const MemOperand& mem, const RegisterValue& rv, const BaseExpression& base, const AddressExpression& addr)
        : reg_(reg), vreg_(vreg), freg_(freg), imm_(imm), cr_(cr), mem_(mem), rv_(rv), base_(base), addr_(addr) {}

    std::optional<int> reg(std::string_view t) const override { return number(reg_, t); }
    std::optional<int> vreg(std::string_view t) const override { return number(vreg_, t); }
    std::optional<int> freg(std::string_view t) const override { return number(freg_, t); }
    std::optional<int> cr(std::string_view t) const override { return number(cr_, t); }
    std::optional<std::int64_t> imm(std::string_view t) const override { return imm_ ? imm_(cb::compat::to_py(t)) : std::nullopt; }
    std::optional<cb::recompile::builders::MemoryOperand> mem(std::string_view t) const override {
        const auto m = mem_ ? mem_(cb::compat::to_py(t)) : std::nullopt;
        if (!m) return std::nullopt;
        return cb::recompile::builders::MemoryOperand{std::get<0>(*m), static_cast<int>(std::get<1>(*m))};
    }
    std::string register_value(int n, std::string_view field) const override { return cb::compat::from_py(rv_(n, cb::compat::to_py(field))); }
    std::string base_expression(int base) const override { return cb::compat::from_py(base_(base)); }
    std::string address(std::int64_t displacement, int base) const override { return cb::compat::from_py(addr_(displacement, base)); }

private:
    static std::optional<int> number(const OperandNumber& f, std::string_view token) {
        const auto n = f ? f(cb::compat::to_py(token)) : std::nullopt;
        return n ? std::optional<int>(static_cast<int>(*n)) : std::nullopt;
    }
    const OperandNumber &reg_, &vreg_, &freg_, &imm_, &cr_;
    const MemOperand& mem_;
    const RegisterValue& rv_;
    const BaseExpression& base_;
    const AddressExpression& addr_;
};

// The body every builder shim shares (arith, logical, memory, system, vector too): the old call in, the new `emit` called, the
// statements back as a py::list. `rv` is the register-value helper in either of the old shapes (the register number as an int64_t,
// or as an optional of one); `word` is the raw instruction word (the trap builders decode it).
template <class NewEmit, class Rv>
inline std::optional<py::list<py::str>> forward(NewEmit new_emit, const py::str& mn, const py::list<py::str>& ops, const OperandNumber& _reg,
                                                const OperandNumber& _vreg, const OperandNumber& _freg, const OperandNumber& _imm,
                                                const OperandNumber& _cr, const MemOperand& _mem, const Rv& rv, const BaseExpression& _base_expr,
                                                const AddressExpression& _addr, int64_t va, int64_t word) {
    return cb::compat::guard([&]() -> std::optional<py::list<py::str>> {
        std::vector<std::string> tokens;
        for (const py::str& t : ops) tokens.push_back(cb::compat::from_py(t));
        const std::string mnemonic = cb::compat::from_py(mn);
        const RegisterValue register_value = [&rv](int64_t n, const py::str& field) { return rv(n, field); };
        const Helpers helpers(_reg, _vreg, _freg, _imm, _cr, _mem, register_value, _base_expr, _addr);
        const auto statements =
            new_emit(cb::recompile::builders::Instruction{mnemonic, tokens, static_cast<std::uint32_t>(va), static_cast<std::uint32_t>(word)}, helpers);
        if (!statements) return std::nullopt;
        return cb::compat::to_py(*statements);
    });
}

// The mnemonics of a builder as the old list.
inline py::list<py::str> mnemonic_list(std::span<const std::string_view> names) {
    py::list<py::str> out;
    for (const std::string_view m : names) out.append(cb::compat::to_py(m));
    return out;
}

}  // namespace shim_

// The C++ statements for one instruction, or None when `mn` is not a scalar floating-point mnemonic handled here. Only _freg and
// _cr are used; the other helpers and `va`, `WORD` are part of the builders' common signature.
inline std::optional<py::list<py::str>> emit(const py::str& mn, const py::list<py::str>& ops, const OperandNumber& _reg,
                                             const OperandNumber& _vreg, const OperandNumber& _freg, const OperandNumber& _imm,
                                             const OperandNumber& _cr, const MemOperand& _mem, const RegisterValue& _rv,
                                             const BaseExpression& _base_expr, const AddressExpression& _addr, int64_t va, int64_t /*WORD*/) {
    return cb::compat::guard([&]() -> std::optional<py::list<py::str>> {
        std::vector<std::string> tokens;
        for (const py::str& t : ops) tokens.push_back(cb::compat::from_py(t));
        const std::string mnemonic = cb::compat::from_py(mn);
        const shim_::Helpers helpers(_reg, _vreg, _freg, _imm, _cr, _mem, _rv, _base_expr, _addr);
        const auto statements = cb::recompile::builders::floating::emit({mnemonic, tokens, static_cast<std::uint32_t>(va)}, helpers);
        if (!statements) return std::nullopt;
        return cb::compat::to_py(*statements);
    });
}

// Standard headers relied on by the emitted C++ (isnan/sqrt from <cmath>, LLONG_MAX/INT_MAX from <climits>).
inline const py::str RUNTIME_HELPERS = cb::compat::to_py(cb::recompile::builders::floating::runtime_helpers());

inline const py::list<py::str> MNEMONICS = [] {
    py::list<py::str> out;
    for (const std::string_view m : cb::recompile::builders::floating::mnemonics()) out.append(cb::compat::to_py(m));
    return out;
}();

}  // namespace codebreaker::recompile::builders::floating
