// codebreaker/recompile/jumptable.hpp -- compat shim for cb/recompile/jumptable.hpp (docs/CPP_DESIGN.md 6.3). No logic:
// conversion and forwarding. Deleted with its last old-layer consumer (recompile/powerpc).
//
// jumptable.py -- the switch tables behind a `bctr`, so a recompiled switch is a C++ switch.
//
//     jumptable.detect_jump_tables(insns, lo, hi, rd_u8, rd_u16, rd_u32, is_exec)
//         {bctr address: (index register, [targets])}
//
// The old port's field decoders and opcode predicates (_op, _is_lis ...) and its scan internals (_scan_bounds,
// _read_targets, TableMatch) were never anyone's interface; they live in the new module's unnamed namespace. `func_lo`
// and `func_hi` were never used by the original and are kept only so that a caller compiles.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>
#include <variant>

#include "cb/compat/py_bridge.hpp"
#include "cb/recompile/jumptable.hpp"
#include "py.hpp"

namespace codebreaker::recompile::jumptable {

inline constexpr int64_t MAX_SCAN_BACK = 64;

using Reader = std::function<std::optional<int64_t>(int64_t)>;  // rd_u8 / rd_u16 / rd_u32 (None if unmapped)
using IsExec = std::function<bool(int64_t)>;
// one instruction: (va, raw_word (int or big-endian bytes), mn, op_str)
using Insn = std::tuple<int64_t, std::variant<int64_t, py::bytes>, py::str, py::str>;
// (index register, [targets])
using JumpTable = std::tuple<int64_t, py::list<int64_t>>;

namespace detail {

// The new interface over the old callables.
class CallbackMemory final : public cb::recompile::jumptable::GuestMemory {
public:
    CallbackMemory(const Reader& u8, const Reader& u16, const Reader& u32, const IsExec& exec) : u8_(u8), u16_(u16), u32_(u32), exec_(exec) {}
    std::optional<std::uint8_t> u8(std::uint32_t a) const override { return narrow<std::uint8_t>(u8_(a)); }
    std::optional<std::uint16_t> u16(std::uint32_t a) const override { return narrow<std::uint16_t>(u16_(a)); }
    std::optional<std::uint32_t> u32(std::uint32_t a) const override { return narrow<std::uint32_t>(u32_(a)); }
    bool is_exec(std::uint32_t a) const override { return exec_(a); }

private:
    template <class T>
    static std::optional<T> narrow(const std::optional<int64_t>& v) {
        return v ? std::optional<T>(static_cast<T>(*v)) : std::nullopt;
    }
    const Reader& u8_;
    const Reader& u16_;
    const Reader& u32_;
    const IsExec& exec_;
};

// The 32-bit word of a raw word (an int, or big-endian bytes): only its low 32 bits decide whether it is a bctr.
inline std::uint32_t word_of(const std::variant<int64_t, py::bytes>& w) {
    if (const int64_t* i = std::get_if<int64_t>(&w)) return static_cast<std::uint32_t>(*i);
    std::uint32_t v = 0;
    for (const unsigned char c : std::get<py::bytes>(w).raw()) v = (v << 8) | c;
    return v;
}

inline JumpTable to_old(const cb::recompile::jumptable::JumpTable& t) {
    py::list<int64_t> targets;
    for (const std::uint32_t x : t.targets) targets.append(x);
    return JumpTable(static_cast<int64_t>(t.index_reg), targets);
}

}  // namespace detail

// Port of FunctionScanner::detect_jump_table for a single bctr address; None when there is no table.
inline std::optional<JumpTable> _detect_one(const Reader& rd_u8, const Reader& rd_u16, const Reader& rd_u32, const IsExec& is_exec,
                                            int64_t bctr_addr) {
    const detail::CallbackMemory mem(rd_u8, rd_u16, rd_u32, is_exec);
    const auto t = cb::recompile::jumptable::detect_one(mem, static_cast<std::uint32_t>(bctr_addr));
    if (!t) return std::nullopt;
    return detail::to_old(*t);
}

// For each `bctr` in the function, detect a jump table (the four kinds the module header lists). Returns
// {bctr_va: (index_reg, [targets])}.
inline py::dict<int64_t, JumpTable> detect_jump_tables(const py::list<Insn>& insns, int64_t func_lo, int64_t func_hi, const Reader& rd_u8,
                                                       const Reader& rd_u16, const Reader& rd_u32, const IsExec& is_exec) {
    (void)func_lo;
    (void)func_hi;
    const detail::CallbackMemory mem(rd_u8, rd_u16, rd_u32, is_exec);
    std::vector<cb::recompile::jumptable::Instruction> list;
    for (const Insn& i : insns) {
        list.push_back({static_cast<std::uint32_t>(std::get<0>(i)), detail::word_of(std::get<1>(i))});
    }
    const auto found = cb::recompile::jumptable::detect_jump_tables(list, mem);
    py::dict<int64_t, JumpTable> out;
    for (const Insn& i : insns) {   // in the order of the instructions, as the original's dict was
        const auto it = found.find(static_cast<std::uint32_t>(std::get<0>(i)));
        if (it != found.end()) out.set(std::get<0>(i), detail::to_old(it->second));
    }
    return out;
}

}  // namespace codebreaker::recompile::jumptable
