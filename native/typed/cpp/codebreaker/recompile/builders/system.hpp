// codebreaker/recompile/builders/system.hpp -- SHIM over cb/recompile/builders/system.hpp (docs/CPP_DESIGN.md 6.3): the old signature in
// (operand readers as std::functions over py::str), the new builder called, the statements back as a py::list. Deleted with its last
// py:: consumer (recompile/powerpc).
//
// What the builder is: special registers, traps, barriers, the CR-bit operations and the other system instructions as C++ for the
// recompiler; see cb/recompile/builders/system.hpp. The traps decode TO, RA, RB and SIMM from the raw instruction word, the last
// argument of `emit` (WORD).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>

#include "cb/compat/py_bridge.hpp"
#include "cb/recompile/builders/system.hpp"
#include "codebreaker/recompile/builders/floating.hpp"

namespace codebreaker::recompile::builders::system {

// The operand helpers recompile/powerpc.py passes in (the same types as the floating builders').
using OperandNumber = floating::OperandNumber;
using MemOperand = floating::MemOperand;
using BaseExpression = floating::BaseExpression;
using AddressExpression = floating::AddressExpression;
// as logical's: the register number is None when the operand token was not a register
using RegisterValue = std::function<py::str(const std::optional<int64_t>&, const py::str&)>;

// The C++ statements for one instruction, or None when `mn` is not a system mnemonic handled here. A token the helper could not read, or
// a clrldi count above 64, is a ParseError (as a py exception), where the old port wrote "None" into the statement.
inline std::optional<py::list<py::str>> emit(const py::str& mn, const py::list<py::str>& ops, const OperandNumber& _reg, const OperandNumber& _vreg,
                                             const OperandNumber& _freg, const OperandNumber& _imm, const OperandNumber& _cr, const MemOperand& _mem,
                                             const RegisterValue& _rv, const BaseExpression& _base_expr, const AddressExpression& _addr, int64_t va,
                                             int64_t WORD) {
    return floating::shim_::forward(&cb::recompile::builders::system::emit, mn, ops, _reg, _vreg, _freg, _imm, _cr, _mem, _rv, _base_expr, _addr, va, WORD);
}

inline const py::str RUNTIME_HELPERS = cb::compat::to_py(cb::recompile::builders::system::runtime_helpers());

inline const py::list<py::str> MNEMONICS = floating::shim_::mnemonic_list(cb::recompile::builders::system::mnemonics());

}  // namespace codebreaker::recompile::builders::system
