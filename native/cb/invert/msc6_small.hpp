// cb/invert/msc6_small.hpp -- the small readers of Microsoft C 6.0a's code: a function's bytes back to the C it was compiled from, where the
// function is one the compiler turns out the same way every time.
//
//     leaf      one of the compiler's leaf templates: return a constant, a parameter or a global, store a constant, one far call, an if of those
//     walk      a statement walker driven by what the program's data segment names: returns, ifs, far calls, stores into long and word globals
//     cleanup   the long-global free idiom, `if (p != 0L) { GlobalFree((WORD)(p >> 16)); p = 0L; }` over and over (LZEXPAND's Cleanup)
//     registers the register variables (SI and DI) and the value kept both as a call's result and as an argument; the counted loop on a register
//     mask      the bytes with the places the linker fills in zeroed: what a recompiled function is compared to the original under
//
// Every reader knows one frame -- the medium model's loadds far frame of /AM /G2 /Gsw, `push ds; pop ax; nop; inc bp; push bp; mov bp,sp; push ds;
// mov ds,ax`, left through `dec bp; dec bp; mov sp,bp; pop ds; pop bp; dec bp` and a far return -- and reads nothing else: a function that is not in
// that frame, or has an instruction a reader does not model, is DECLINED with the reason and no text. A reader never guesses.
//
//     const Result<Leaf> r = msc6_small::leaf(code);
//     if (r.ok()) use(r.value().text); else say(r.error().what());     // r.error() is a Declined
//
// The text is the C of /AM /G2 /Gsw /Ox, in names the readers make up (a0 a1 ... for the parameters, gG for a global it has no name for); the
// recompiled text and the original agree under `mask` when the reading was right.
#pragma once

#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/error.hpp"

namespace cb::invert::msc6_small {

// A function the reader does not cover: the reason is the message.
class Declined : public Error {
public:
    using Error::Error;
    CB_ERROR_BOILERPLATE(Declined)
};

// ---- the program's data ------------------------------------------------------------------------------------------------------

enum class Width { Word, Long };

// A global the program's symbols name, by the offset of its (low) word in the data segment.
struct Global {
    std::string name;
    Width width = Width::Word;
};
using Globals = std::map<std::uint32_t, Global>;

// The routines a reader names, by the order its calls come in the function.
using Callees = std::map<std::uint32_t, std::string>;

// ---- what a reader finds -------------------------------------------------------------------------------------------------------

// A leaf template. `shape` names it ("return 7;", "T5 far-call", "T6 if-branch", "empty", "int i").
struct Leaf {
    std::string text;
    std::uint32_t arguments = 0;           // parameters, as the far return's count says
    std::string shape;
    std::uint32_t frame = 0;               // the frame's bytes (`sub sp,N`); a template that is not a store or a return only
    bool trailing_nop = false;
    std::string return_type = "int";
    std::vector<std::string> globals;      // the globals the text names
    std::vector<std::string> call_arguments;   // a far call's arguments, as written
    std::vector<std::string> lines;        // an if of leaves: its statements
};

struct Walked {
    std::string text;
    std::size_t statements = 0;
    std::vector<std::pair<std::string, Width>> globals;   // in the order the walk named them
};

struct Registers {
    std::string text;
    std::vector<std::string> saved;        // the registers the function saves (si, di)
    std::vector<std::string> locals;       // the register variables as the text names them
    std::vector<std::string> statements;
};

struct CountedLoop {
    std::string text;
    std::string limit;                     // the parameter that bounds the loop
    std::vector<std::string> body_calls;
};

// ---- the readers ------------------------------------------------------------------------------------------------------------------

Result<Leaf> leaf(ByteView code);
Result<Walked> walk(ByteView code, const Globals& globals, std::string_view callee = "G");
// `names`: the long globals' names by the offset of their low words.
Result<std::string> cleanup(ByteView code, const std::map<std::uint32_t, std::string>& names);
Result<Registers> registers(ByteView code, const Callees& callees = {});
Result<CountedLoop> counted_loop(ByteView code, const Callees& callees = {});

// The bytes with the operands the linker fills in zeroed: a far call's target, a near call's displacement, the immediate of `mov ax,[m]`,
// `mov [m],ax` and `push imm16`, and the displacement of a direct [imm16] operand.
Bytes mask(ByteView code);

}  // namespace cb::invert::msc6_small
