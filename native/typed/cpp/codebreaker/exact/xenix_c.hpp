// codebreaker/exact/xenix_c.hpp -- typed C++ port of codebreaker.exact.xenix_c.
//
// xenix_c.py -- the C compiler of XENIX for the 8086 (cpp, c0, c1, c2, as; 1982-83), as a toolchain.
//
// The profile (toolchains/xenixc.json) names this module ("plugin"); what cannot be data is here. The
// compiler that survives is the Altos ACS 8600 XENIX 2.3 diskette's; the programs Microsoft built
// with it (LINK 2.40/2.44, LIB 2.20/2.40, 1983-84) were made by another revision. The variant
// "microsoft" is that revision, as far as their bytes tell:
//
//   - c1 without its stack probe (the limit word it keeps: 32767)
//   - c1's assembly revised before c2 reads it (rules U D M B: where a value is made)
//   - c2: at an unconditional jump its two `mov reg,reg` passes give up (and one of them also at the
//     note that ends a function); a jump or call through a pointer counts 6 more to its reckoning of
//     how far a jump goes; a constant is not taken from a register a variable lives in; a call through
//     a pointer uses every register
//   - c2's block moves: a loop's block is left where it is, a block a conditional jump leads to over
//     one other changes places with it (hooks at two places of codemove)
//
// Run under the XENIX emulator (emu/xenix.py). The object is a Version 7 a.out (exact/aout.py).
//
// Typing notes:
//   * The Python class XenixC is a toolchain of its own (it does not subclass exact.toolchain.Toolchain); here it derives
//     from the typed Toolchain only so that toolchain::load() can hand it out as a plugin ("codebreaker.exact.xenix_c:XenixC":
//     register_plugin() below). The base is constructed with no variant, as load() would not know it; its own compile() is
//     not used. `name`, `variant` and `p` are the base's; `key` is this class' own.
//   * The key is made from repr(sorted(profile.items())): the profile is read again from toolchains/NAME.json (its keys in the
//     file's order, as the Python's dict has them), since the typed Profile does not keep that order.
//   * The patches _c1 and _c2 change a bytearray in place and return it; here they take the bytes and return the patched bytes.
//   * The c2 hooks (c2_hooks) work on the emulator through xenix::Machine (the Python's `uc`); `pending` is shared by the two hooks.
//   * compile() with `headers=None` is the optional; `switches` is accepted and not used (as in the Python).
//     The cached value is the tuple (object or None, log) under cache kind "exact".
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>

#include "codebreaker/exact/emu/xenix.hpp"
#include "codebreaker/exact/toolchain.hpp"
#include "py.hpp"

namespace codebreaker::exact::xenix_c {

namespace xenix = codebreaker::exact::emu::xenix;

extern const py::str CACHE_VERSION;   // "x1"

// ---- c1's assembly, revised (Microsoft's c1)
extern const py::list<std::tuple<py::str, py::str>> PAIRS;             // (low, high) registers of the pairs a long lives in
extern const py::str OPS;                                             // the one-operand and shift mnemonics, as a regex alternation

// an unsigned word made a long: this c1 loads the low register of the pair and clears the high one;
// Microsoft's made the long in AX:DX and moved it to the pair if it was wanted in one
py::str rule_u(const py::str& t);
// a ++ or -- put off until its byte is fetched: Microsoft's c1 does it when the byte is the value wanted
py::str rule_d(const py::str& t);
// op (R),M with M in memory (no 8086 instruction): Microsoft's c1 brought M to AX first
py::str rule_m(const py::str& t);
// a char made an int OR'ed with a constant: Microsoft's c1 did it on the word
py::str rule_b(const py::str& t);

// RULES = {"U": rule_u, "D": rule_d, "M": rule_m, "B": rule_b}
extern const py::dict<py::str, std::function<py::str(const py::str&)>> RULES;

// ---- c2's block moves (Microsoft's c2), from outside: hooks at codemove
extern const int64_t JBR, CBR, JMP, LABEL;

// the emulator's list of c2's nodes (op, sub-op, forward, backward, reference links), read from segment `ds`
class _List {
  public:
    _List(xenix::Machine& uc, int64_t ds);
    xenix::Machine& uc;
    int64_t b;
    int64_t w(int64_t a) const;
    void setw(int64_t a, int64_t v) const;
    int64_t op(int64_t p) const;
    int64_t sub(int64_t p) const;
    int64_t forw(int64_t p) const;
    int64_t back(int64_t p) const;
    int64_t ref(int64_t p) const;
    void link(int64_t a, int64_t b_) const;
    void unlink(int64_t p) const;
};

// (L, bend, t) of the one block a conditional jump leads over, or None
std::optional<std::tuple<int64_t, int64_t, int64_t>> _one_block(const _List& l, int64_t p1);
// the block moved and the conditional jump that leads to it: q, or None (False: nothing was changed)
std::optional<int64_t> _swap(const _List& l, int64_t p1);
// {address: hook} of codemove: "R" leaves a loop's block where it is, "S" swaps a block over one other
py::dict<int64_t, xenix::Hook> c2_hooks(const py::str& mode = py::str("RS"));

// ---- the passes, changed as Microsoft's
py::bytes _c1(const py::bytes& b);    // RuntimeError if this is not the c1 the patch was written for
py::bytes _c2(const py::bytes& c2);   // (the same, for c2)

class XenixC : public toolchain::Toolchain {
  public:
    XenixC(const toolchain::Profile& profile, const std::optional<py::str>& variant = std::nullopt);

    py::str key;       // sha1(repr(sorted(profile.items())) + variant + CACHE_VERSION)[:12]
    py::str rules;     // the variant's c1 rules ("UDMB")
    bool patch;        // the variant's patched c1 and c2
    py::str c2move;    // the variant's c2 hooks ("RS")

    // Toolchain::compile as the modules use it (headers given, cache on)
    std::tuple<std::optional<py::bytes>, py::str> compile(const py::bytes& src, const py::dict<py::str, py::bytes>& headers,
                                                          const py::list<py::str>& include,
                                                          const std::optional<py::str>& switches) const override;

    // compile(src, headers=None, include=(), switches=None, cache=True) -> (the object file or None, what the passes printed)
    std::tuple<std::optional<py::bytes>, py::str> compile(const py::bytes& src,
                                                          const std::optional<py::dict<py::str, py::bytes>>& headers = std::nullopt,
                                                          const py::list<py::str>& include = py::list<py::str>(),
                                                          const std::optional<py::str>& switches = std::nullopt, bool cache = true) const;

    // the object, with the headers (name, bytes) as given
    std::tuple<std::optional<py::bytes>, py::str> _compile(const py::bytes& src,
                                                           const py::list<std::tuple<py::str, py::bytes>>& headers) const;

  private:
    struct Raw;
    std::shared_ptr<const Raw> raw_;
};

// puts XenixC where toolchain::load() finds it: the plugin "codebreaker.exact.xenix_c:XenixC"
void register_plugin();

}  // namespace codebreaker::exact::xenix_c
