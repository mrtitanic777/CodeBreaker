// codebreaker/decompile/asm_blocks.hpp -- typed C++ port of codebreaker.decompile.asm_blocks.
//
// asm_blocks.py -- a 16-bit function's _asm blocks placed in its L2 tree, where the instructions say.
//
//     doc, said = place(md, doc, code, start, end, fixups, name_at, seg, window)
//
// A function of Microsoft C 8 holding an _asm block (analyze/origin: INT n, IN/OUT, CLI/STI, the carry
// -- inside the frame C 8 gives every such function) comes from the decompiler as C round an interrupt
// helper (`int21(0x4405, ...)`) whose carry it reads back as a variable (`int_CF`). What the programmer
// wrote is the interrupt's own instructions, inline. The tree says which statements came from which
// instructions (L2 origin "at", the decompiler's PrintL2), so the block can take their place:
//
//   the block       from the first instruction that loads what the interrupt is given (walking back
//                   from the INT over instructions the inline assembler takes that set registers, and
//                   push/pop of segment registers -- not a store to memory, a call, a jump or a place
//                   jumped to) to the INT and the pop that puts DS back; the statements made only of
//                   those instructions leave the tree, a MachineFragment (reason "inline") stands there
//   its carry       an If whose condition reads int_CF / int_ZF: the test was assembly as well (jc,
//                   cmp, je), a second block -- from the first instruction of its condition to where
//                   its then-part begins -- its jumps out of the block to C labels set at the then-part
//                   and after it
//
// `window` moves the block's two ends by instructions (the boundary search: which of the nearby
// choices is what was written, the compiler says, byte for byte -- verify/authored/match16.py).
//
// In the block, a place in the frame is named by the C variable there (a parameter at [bp+k], a local
// Stack_N at [bp+2-N]) so the compiler can lay the frame out as it does; one with no variable stays
// [bp+k]. Calls and data are named by the module's fix-ups as in decompile/inline_asm.py.
// -> (the tree, what was done: [str]) -- the tree unchanged, and why, where nothing could be placed.
//
// Typing notes:
//   * THE TREE. The L2 document is JSON as the decompiler's PrintL2 writes it: dicts, lists, strings,
//     numbers, booleans and None mixed freely, and this module reads and edits it by the keys it knows
//     ("node", "body", "then", "origin", "at", "sym", ...). That is the one place where the Python
//     genuinely mixes types, so it is a tagged union spelled as a struct, Json (the same shape as
//     codebreaker::knowledge::sources::Json); only the field of `kind` means anything. Its lists and
//     dicts are py::list / py::dict, which are shared handles as Python's are: a list taken out of the
//     tree and edited is edited in the tree, and copy.deepcopy is deepcopy() (a tree read from JSON has
//     no shared parts, which is what the memo of copy.deepcopy would keep). `d.get(k)` on a value that
//     is not a dict is the AttributeError it is in Python (get()). An integer that does not fit int64_t
//     is refused when the tree is made (Python's would be a big int);
//   * `import capstone` / `platforms.decoder(16, detail=True)` have no C++ counterpart (there is no
//     port of codebreaker.platforms); the decoder is the first parameter of _decode and place:
//     md.disasm(code, address) as a std::function, as in decompile/inline_asm.hpp. The decoded
//     instruction is inline_asm::Instruction plus `access`, capstone's op.access of each operand,
//     which _feeds reads (CS_AC_WRITE is 2);
//   * the fixups are inline_asm::Fixups (the dict mixes offset keys and ("segment", offset) keys);
//     this module only asks whether an offset is a key, and inline_asm::_operands does not read the
//     fix-ups, name_at or seg at all, so `seg=None` (std::nullopt) changes nothing and passes as 0;
//   * `name_at=None` is an empty std::function (the default: no name);
//   * `code` is bytes or bytearray: one overload each. A negative `start` is not supported (capstone's
//     addresses are unsigned; every offset here is a 16-bit segment offset);
//   * the module-level compiled pattern _HELPER is an accessor (a function-local static) so that it
//     does not depend on the order translation units start in; _FLAGS and _SEGS are lists of text;
//   * names in the tree that the Python compares or puts in sets (a symbol's "name") are str or None
//     here, which is all the decompiler writes; any other type in such a place is a TypeError. Likewise
//     origin.at is a list of integers (the Python would also take a string as a set of characters);
//   * Python's ints are unbounded; int64_t is the type: a number in a bracket of an operand ([bp + k]) beyond
//     int64 raises OverflowError here (Python would leave that text as it is, no frame offset being so large).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "py.hpp"
#include "py_os.hpp"
#include "codebreaker/decompile/inline_asm.hpp"

namespace codebreaker::decompile::asm_blocks {

namespace inline_asm = ::codebreaker::decompile::inline_asm;

// A JSON value (what json.load gives, and what the L2 tree is made of); see the typing notes.
struct Json {
    enum Kind { None, Bool, Int, Float, Str, List, Dict };
    Kind kind = None;
    bool b = false;
    int64_t i = 0;
    double d = 0.0;
    py::str s;
    py::list<Json> list;
    py::dict<py::str, Json> dict;

    static Json make_none() { return Json(); }
    static Json make_bool(bool v) { Json j; j.kind = Bool; j.b = v; return j; }
    static Json make_int(int64_t v) { Json j; j.kind = Int; j.i = v; return j; }
    static Json make_float(double v) { Json j; j.kind = Float; j.d = v; return j; }
    static Json make_str(const py::str& v) { Json j; j.kind = Str; j.s = v; return j; }
    static Json make_list(const py::list<Json>& v) { Json j; j.kind = List; j.list = v; return j; }
    static Json make_dict(const py::dict<py::str, Json>& v) { Json j; j.kind = Dict; j.dict = v; return j; }
};

// Python's truth value of a JSON value (None, False, 0, 0.0, "", [] and {} are false)
bool truthy(const Json& v);
// v == "text"
bool is_str(const Json& v, const char* text);
// copy.deepcopy(v)
Json deepcopy(const Json& v);
// d.get(key): None when absent; AttributeError when d is not a dict
Json get(const Json& d, const char* key);
// d.get(key, default)
Json get(const Json& d, const char* key, const Json& dflt);

// The decoded instruction: inline_asm's, with capstone's access flags of its operands.
struct Instruction : inline_asm::Instruction {
    py::list<int64_t> access;        // op.access of each operand (the same length as `operands`)
};
inline constexpr int64_t CS_AC_WRITE = 2;
// md.disasm(code, address)
using Decoder = std::function<py::list<Instruction>(const py::bytes&, uint64_t)>;

using Frame = py::dict<int64_t, py::str>;      // {bp offset: C name}
using NameAt = inline_asm::NameAt;
using Fixups = inline_asm::Fixups;

// re.compile(r"^int([0-9a-f]{2})(_[a-z_]+)?$")
const py::re::Pattern& _HELPER();
extern const py::list<py::str> _FLAGS;
extern const py::list<py::str> _SEGS;

// Every dict of the tree, the node itself first and then those in its values, in order.
py::list<Json> _walk(const Json& n);

// Every statement list of the tree (a body, a then-part, an else-part).
py::list<py::list<Json>> _lists(const Json& n);

py::set<int64_t> _at(const Json& stmt);

// The interrupt number of the first call of an interrupt helper (`int21`, `int21_xyz`) in the statement.
std::optional<int64_t> _calls_helper(const Json& stmt);

// Does the statement's condition read a variable of those names?
bool _reads(const Json& stmt, const py::list<py::str>& names);

// {bp offset: C name} -- the parameters (their entry-stack place) and Stack_N locals.
Frame _frame_names(const Json& doc, bool far);

// [bp + k] / [bp - k] in an operand by the C name of that place.
py::str _named(const py::str& text, const Frame& frame);

py::list<Instruction> _decode(const Decoder& md, const py::bytes& code, int64_t start, int64_t end);
py::list<Instruction> _decode(const Decoder& md, const py::bytearray& code, int64_t start, int64_t end);

// An instruction the inline assembler takes that only sets registers for what follows (or saves
// and loads a segment register): part of an interrupt's set-up.
bool _feeds(const Instruction& i);

// One instruction as the inline assembler takes it; jumps to `labels`. -> text or None.
std::optional<py::str> _line(const Instruction& i, const Frame& frame, const py::dict<int64_t, py::str>& labels,
                             const Fixups& fixups, const NameAt& name_at, std::optional<int64_t> seg);

// See the module's doc. code: the segment's bytes; start, end: the function's place in it.
// -> (the tree, what was done)
std::tuple<Json, py::list<py::str>> place(const Decoder& md, const Json& doc, const py::bytes& code, int64_t start, int64_t end,
                                          const Fixups& fixups = Fixups(), const NameAt& name_at = NameAt(),
                                          std::optional<int64_t> seg = std::nullopt,
                                          const std::tuple<int64_t, int64_t>& window = std::make_tuple(0, 0));
std::tuple<Json, py::list<py::str>> place(const Decoder& md, const Json& doc, const py::bytearray& code, int64_t start, int64_t end,
                                          const Fixups& fixups = Fixups(), const NameAt& name_at = NameAt(),
                                          std::optional<int64_t> seg = std::nullopt,
                                          const std::tuple<int64_t, int64_t>& window = std::make_tuple(0, 0));

// The locals the blocks took the last use of (in_BX, unaff_SS, the helper's temporaries) left
// out: declared, C 8 would give them room in the frame, and a codebreaker_ss() store.
void _unused(const Json& doc);

}  // namespace codebreaker::decompile::asm_blocks
