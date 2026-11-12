// cb/exact/habits/newcomp.hpp -- is a procedure of MASM 1.10 the kit compiler's code but for the later compiler's habits?
//
// MASM 1.10's main compiland was built by a state of the MS-Pascal compiler later than 2.05 and earlier than 3.04
// (neither gives its code; tried, with the kits of Seattle Computer Products). For the same source it differs from the
// compiler of 1981 in these habits:
//
//   a value worked     later:  mov ax,M  [xor ah,ah]  shl ax,1  add ax,k ..  then  xchg R,ax,  or
//   out in AX                  push ax,  or  mov [m],al,  or  add dx,ax ..
//                      1981:   mov R,M   [and R,0FFh | xor Rh,Rh]  shl R,1  add R,k ..  push R ..
//                      The later compiler loads a value into AX (the encodings are shorter), or moves it there
//                      (xchg R,ax), and computes there; where the result is wanted in another register it is
//                      exchanged into it. The compiler of 1981 computes in the register itself. So for a stretch the
//                      two keep the same values in other registers: the one's AX is the other's DX, and - AX being
//                      taken - what the one then puts in DX the other has in CX.
//
//   DISPOSE(P)         later:  the address of P pushed, call far 0CA2:0250 (the library frees the block)
//                      1981:   mov si,[P]  or byte ptr [si-2],1   (the block's free bit set in line)
//
//   FOR I := c1 TO c2  later:  xor dx,dx  shr dx,1  jb past the loop   (the entry test, on constants)
//                      1981:   nothing: the constants say the loop runs
//
//   CASE               later:  sub bx,lo  ja +3  jmp case-lo  shl bx,1  jmp cs:[bx+table]
//                      1981:   sub bx,lo  shl bx,1  jmp cs:[bx+table]
//
//   a shift count      later:  the value into AX, then  mov cx,n  shl ax,cl
//                      1981:   mov cx,n  first, then the value into R,  shl R,cl
//
//   a pointer in DX    later:  mov di,dx   (the value is still there from the statement before)
//                      1981:   the variable loaded again
//
//   an outer frame     later:  mov di,[bp]  mov di,[di]  again before each use
//                      1981:   SI still has it from the statement before
//
//   VAR argument       an outer procedure's variable passed by reference right after an assignment to it: the
//                      compiler of 1981 pushes its value (an error; a call put between makes it push the address),
//                      the later one pushes the address
//
//   MUL                1981:   mov [temporary],dx  before it, never read again
//                      later:  nothing
//
//   temporaries        the two do not need the same number of them: the frame's size may differ and a temporary may
//                      have another place in it (always the same one)
//
// `equivalent` reads the two procedures instruction by instruction. Two instructions agree if their bytes are equal
// (but for the bytes a linker fills in), or if, under the renaming of registers in force, the original's instruction is
// the compiled one. A renaming begins only where the original loads into AX what the compiled code loads into another
// register, or exchanges a register with AX where the compiled code does nothing; it is changed by an exchange only
// one of the two has, and by a load both do into different registers; a call ends it. Every relative jump must go to
// the instruction that corresponds. Anything else fails the procedure.
#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/exact/habits/cmpp.hpp"

namespace cb::exact::habits::newcomp {

// What the comparison found. When `same`, `notes` say which habits explain the differences (never empty: a procedure
// that differs in nothing but jump displacements is not one this comparer explains); when not, `notes` hold one line
// saying where it stopped.
struct Finding {
    bool same = false;
    std::vector<std::string> notes;
    // When `same`: where each instruction of the original is in the compiled code, (offset in the original, offset in
    // the compiled) in ascending order, and where the compiled code's instructions begin.
    std::vector<std::pair<std::uint32_t, std::uint32_t>> alignment;
    std::vector<std::uint32_t> compiled_starts;
};

// The procedure `want` of the original and `got` the compiled one. `linker_words` are the offsets in `got` of the bytes
// a linker fills in (what the comparison does not hold to the original). `options` say what follows the entry helper's
// call (the frame data, which is not code).
Finding equivalent(ByteView want, ByteView got, const std::set<std::uint32_t>& linker_words, const cmpp::Options& options = {});

// INIT86 of MASM 1.10 was built by yet another state of the compiler: the entry helper of MS-Pascal 3.0x (only the
// frame size behind the call, a frame four bytes smaller, mov sp,bp at the exit), the code still that of 1981 but for a
// few forms - none of the compilers in hand writes it (1981: other helper; 3.04: other registers). Compared instruction
// by instruction with the 1981 compiler's code; allowed, besides equal bytes:
//
//   the entry          call helper / dw frame            against  call helper / db args / dw frame + 4
//   a local            [bp - n]                          against  [bp - (n + 4)]
//   the exit           mov sp,bp                         against  lea sp,[bp - 4]
//   an indexed store   add di,k / mov [di],v             against  mov [di + k],v
//   a byte moved       mov dx,[..] / mov [..],dl         against  mov ax,[..] / mov [..],al
//   a far address      mov dx,k / push ds / push dx      against  mov bx,k / mov dx,ds / mov es,dx / push es / push bx
//   OR of two tests    or al,[temporary]                 against  or ax,[temporary]
Finding equivalent3(ByteView want, ByteView got, const std::set<std::uint32_t>& linker_words, const cmpp::Options& options = {});

// ---- the readers the comparison is made of -----------------------------------------------------------------------------

// The instruction text with every number written as N (and "- N" as "+ N"): the shape of an instruction whose numbers a
// linker may have changed.
std::string mask(std::string_view text);

// A relative jump or call: the length of its opcode and its displacement.
struct Relative {
    int opcode_length = 0;
    std::int64_t displacement = 0;
};
std::optional<Relative> relative(ByteView instruction);

// The instruction with its registers renamed (`renaming` maps a register's family - ax, cx, dx, bx, si, di - to the one
// that holds its value); none when it cannot be written: SI and DI have no halves.
std::optional<std::string> rename(std::string_view instruction, const std::map<std::string, std::string>& renaming);

}  // namespace cb::exact::habits::newcomp
