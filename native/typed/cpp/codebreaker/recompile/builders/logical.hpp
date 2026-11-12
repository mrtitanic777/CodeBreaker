// codebreaker/recompile/builders/logical.hpp -- SHIM over cb/recompile/builders/logical.hpp (docs/CPP_DESIGN.md 6.3): the old signature in
// (operand readers as std::functions over py::str), the new builder called, the statements back as a py::list. Deleted with its last
// py:: consumer (recompile/powerpc).
//
// What the builder is: logical, rotate, shift, extend and immediate boolean operations as C++ for the recompiler; see
// cb/recompile/builders/logical.hpp.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>

#include "cb/compat/py_bridge.hpp"
#include "cb/recompile/builders/logical.hpp"
#include "codebreaker/recompile/builders/floating.hpp"

namespace codebreaker::recompile::builders::logical {

// The operand helpers recompile/powerpc.py passes in (the same types as the floating builders').
using OperandNumber = floating::OperandNumber;
using MemOperand = floating::MemOperand;
using BaseExpression = floating::BaseExpression;
using AddressExpression = floating::AddressExpression;
// _rv(n, field): the register-value expression "ctx.r<n>.<field>"; n is None (std::nullopt) when the operand token was not a register.
using RegisterValue = std::function<py::str(const std::optional<int64_t>&, const py::str&)>;

const uint64_t U64 = 0xFFFFFFFFFFFFFFFFull;

// the rotate mask from bit mstart to bit mstop (the code generator's compute_mask)
inline uint64_t _compute_mask(int64_t mstart, int64_t mstop) { return cb::recompile::builders::logical::rotate_mask(mstart, mstop); }

// The C++ statements for one instruction, or None when `mn` is not a logical, rotate, shift or extend mnemonic handled here. A token the
// helper could not read, or a clrlwi count above 32, is a ParseError (as a py exception), where the old port wrote "None" into the statement.
inline std::optional<py::list<py::str>> emit(const py::str& mn, const py::list<py::str>& ops, const OperandNumber& _reg, const OperandNumber& _vreg,
                                             const OperandNumber& _freg, const OperandNumber& _imm, const OperandNumber& _cr, const MemOperand& _mem,
                                             const RegisterValue& _rv, const BaseExpression& _base_expr, const AddressExpression& _addr, int64_t va,
                                             int64_t WORD) {
    return floating::shim_::forward(&cb::recompile::builders::logical::emit, mn, ops, _reg, _vreg, _freg, _imm, _cr, _mem, _rv, _base_expr, _addr, va, WORD);
}

inline const py::str RUNTIME_HELPERS = cb::compat::to_py(cb::recompile::builders::logical::runtime_helpers());

inline const py::list<py::str> MNEMONICS = floating::shim_::mnemonic_list(cb::recompile::builders::logical::mnemonics());

// Elsewhere: the CR-bit operations crand crandc creqv crnand crnor cror crorc crxor (system).

}  // namespace codebreaker::recompile::builders::logical
