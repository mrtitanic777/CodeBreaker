// codebreaker/recompile/builders/arith.hpp -- SHIM over cb/recompile/builders/arith.hpp (docs/CPP_DESIGN.md 6.3): the old signature in
// (operand readers as std::functions over py::str), the new builder called, the statements back as a py::list. Deleted with its last
// py:: consumer (recompile/powerpc).
//
// What the builder is: integer arithmetic and comparison as C++ for the recompiler; see cb/recompile/builders/arith.hpp.
#pragma once

#include <cstdint>
#include <optional>

#include "cb/compat/py_bridge.hpp"
#include "cb/recompile/builders/arith.hpp"
#include "codebreaker/recompile/builders/floating.hpp"

namespace codebreaker::recompile::builders::arith {

// The operand helpers recompile/powerpc.py passes in (the same types as the floating builders').
using OperandNumber = floating::OperandNumber;
using MemOperand = floating::MemOperand;
using RegisterValue = floating::RegisterValue;
using BaseExpression = floating::BaseExpression;
using AddressExpression = floating::AddressExpression;

// The C++ statements for one instruction, or None when `mn` is not an integer arithmetic or comparison mnemonic handled here. A
// token the helper could not read is a ParseError (as a py exception), where the old port wrote "None" into the statement.
inline std::optional<py::list<py::str>> emit(const py::str& mn, const py::list<py::str>& ops, const OperandNumber& _reg, const OperandNumber& _vreg,
                                             const OperandNumber& _freg, const OperandNumber& _imm, const OperandNumber& _cr, const MemOperand& _mem,
                                             const RegisterValue& _rv, const BaseExpression& _base_expr, const AddressExpression& _addr, int64_t va,
                                             int64_t WORD) {
    return floating::shim_::forward(&cb::recompile::builders::arith::emit, mn, ops, _reg, _vreg, _freg, _imm, _cr, _mem, _rv, _base_expr, _addr, va, WORD);
}

inline const py::str RUNTIME_HELPERS = cb::compat::to_py(cb::recompile::builders::arith::runtime_helpers());

inline const py::list<py::str> MNEMONICS = floating::shim_::mnemonic_list(cb::recompile::builders::arith::mnemonics());

}  // namespace codebreaker::recompile::builders::arith
