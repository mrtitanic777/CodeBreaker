// cb/decompile/asm_blocks.hpp -- a 16-bit function's _asm blocks placed in its L2 tree, where the instructions say.
//
//     auto said = asm_blocks::place(doc, code, start, end, fixups);   // doc edited in place
//
// A function of Microsoft C 8 holding an _asm block (analyze/origin: INT n, IN/OUT, CLI/STI, the carry -- inside the
// frame C 8 gives every such function) comes from the decompiler as C round an interrupt helper (`int21(0x4405, ...)`)
// whose carry it reads back as a variable (`int_CF`). What the programmer wrote is the interrupt's own instructions,
// inline. The tree says which statements came from which instructions (L2 origin "at", the decompiler's PrintL2), so the
// block can take their place:
//
//   the block       from the first instruction that loads what the interrupt is given (walking back from the INT over
//                   instructions the inline assembler takes that set registers, and push/pop of segment registers -- not
//                   a store to memory, a call, a jump or a place jumped to) to the INT and the pop that puts DS back; the
//                   statements made only of those instructions leave the tree, a MachineFragment (reason "inline")
//                   stands there
//   its carry       an If whose condition reads int_CF / int_ZF: the test was assembly as well (jc, cmp, je), a second
//                   block -- from the first instruction of its condition to where its then-part begins -- its jumps out
//                   of the block to C labels set at the then-part and after it
//
// `window` moves the block's two ends by instructions (the boundary search: which of the nearby choices is what was
// written, the compiler says, byte for byte -- verify/authored/match16).
//
// In the block, a place in the frame is named by the C variable there (a parameter at [bp+k], a local Stack_N at
// [bp+2-N]) so the compiler can lay the frame out as it does; one with no variable stays [bp+k]. Calls and data are named
// as in decompile/inline_asm.
//
// The tree is the L2 document as a Json (l2tree.hpp), edited in place; place answers what it did, one sentence each --
// and the tree unchanged, with the reason, where nothing could be placed.
#pragma once

#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/json_fwd.hpp"
#include "cb/core/ordered_map.hpp"
#include "cb/decompile/inline_asm.hpp"

namespace cb::decompile::asm_blocks {

using inline_asm::Decoder;
using inline_asm::Instruction;

// Offsets in the segment where the module has a relocation (an instruction that has one inside it is one this module
// cannot say as text: the inline assembler needs the symbol).
using FixupSites = std::unordered_set<std::uint32_t>;

// How far the block's two ends are moved, in instructions (negative: earlier).
struct Window {
    std::int32_t first = 0;
    std::int32_t last = 0;
};

// {bp offset: C name} of the places in the frame the tree names: the parameters (their entry-stack place) and the
// Stack_N locals, in the order the tree gives them.
using Frame = OrderedMap<std::int64_t, std::string>;
Frame frame_names(const Json& doc);

// [bp + k] / [bp - k] in an operand's text by the C name of that place.
std::string name_frame_places(std::string_view operand, const Frame& frame);

// The interrupt number of the first call of an interrupt helper (`int21`, `int21_xyz`) in the statement.
std::optional<std::uint32_t> interrupt_called(const Json& stmt);

// Does the statement's condition read a variable of those names?
bool condition_reads(const Json& stmt, std::span<const std::string_view> names);

// An instruction the inline assembler takes that only sets registers for what follows (or saves and loads a segment
// register): part of an interrupt's set-up.
bool feeds_interrupt(const Instruction& i);

// One instruction as the inline assembler takes it; jumps go to `labels` (address -> label). nullopt when it cannot be
// said as text (a jump to somewhere with no label, a call, an instruction with a relocation).
std::optional<std::string> inline_line(const Instruction& i, const Frame& frame,
                                       const std::unordered_map<std::uint32_t, std::string>& labels, const FixupSites& fixups);

// Place the blocks of the function at code[start:end] (the segment's bytes; the range is clamped to them) in `doc`.
// `decoder` is the 16-bit decoder with operand access (default: inline_asm::decode16).
std::vector<std::string> place(Json& doc, ByteView code, std::uint32_t start, std::uint32_t end,
                               const FixupSites& fixups = {}, Window window = {}, const Decoder& decoder = {});

// The locals the blocks took the last use of (in_BX, unaff_SS, the helper's temporaries) left out: declared, C 8 would
// give them room in the frame, and a codebreaker_ss() store.
void drop_unused_locals(Json& doc);

}  // namespace cb::decompile::asm_blocks
