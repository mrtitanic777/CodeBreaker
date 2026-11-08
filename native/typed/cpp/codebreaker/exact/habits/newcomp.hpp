// codebreaker/exact/habits/newcomp.hpp -- C++ from codebreaker.exact.habits.newcomp.
//
// newcomp.py - is a procedure of MASM 1.10 the kit compiler's code but for the later compiler's habits?
//
// MASM 1.10's main compiland was built by a state of the MS-Pascal compiler later than 2.05 and
// earlier than 3.04 (neither gives its code; tried, with the kits of Seattle Computer Products).
// For the same source it differs from the compiler of 1981 in four habits:
//
//   a value worked     later:  mov ax,M  [xor ah,ah]  shl ax,1  add ax,k ..  then  xchg R,ax,  or
//   out in AX                  push ax,  or  mov [m],al,  or  add dx,ax ..
//                      1981:   mov R,M   [and R,0FFh | xor Rh,Rh]  shl R,1  add R,k ..  push R ..
//                      The later compiler loads a value into AX (the encodings are shorter), or
//                      moves it there (xchg R,ax), and computes there; where the result is wanted in
//                      another register it is exchanged into it. The compiler of 1981 computes in the
//                      register itself. So for a stretch the two keep the same values in other
//                      registers: the one's AX is the other's DX, and - AX being taken - what the one
//                      then puts in DX the other has in CX.
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
//   VAR argument       an outer procedure's variable passed by reference right after an assignment to it:
//                      the compiler of 1981 pushes its value (an error; a call put between makes it
//                      push the address), the later one pushes the address
//
//   MUL                1981:   mov [temporary],dx  before it, never read again
//                      later:  nothing
//
//   temporaries        the two do not need the same number of them: the frame's size may differ and a
//                      temporary may have another place in it (always the same one)
//
// equivalent(want, got, fixups) reads the two procedures instruction by instruction. Two
// instructions agree if their bytes are equal (but for the bytes a linker fills in), or if, under
// the renaming of registers in force, the original's instruction is the compiled one. A renaming
// begins only where the original loads into AX what the compiled code loads into another register,
// or exchanges a register with AX where the compiled code does nothing; it is changed by an exchange
// only one of the two has, and by a load both do into different registers; a call ends it. Every
// relative jump must go to the instruction that corresponds. Anything else fails the procedure.
#pragma once

#include <optional>
#include <tuple>

#include "codebreaker/exact/habits/boolx.hpp"
#include "codebreaker/exact/habits/cmpp.hpp"
#include "codebreaker/exact/habits/symx.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::exact::habits::newcomp {

// call far 0CA2:0250, the heap's routine that frees a block
extern const py::bytes DISPOSE;
// NUM = re.compile(r"0x[0-9a-f]+|\b\d+\b") and REG = re.compile(r"\b(ax|al|ah|cx|...|di)\b") (every name of PART),
// compiled on first use
const py::re::Pattern& NUM();
const py::re::Pattern& REG();
// FAM = {family: (word, low byte, high byte)}; None where a register has no such half
using Family = std::tuple<std::optional<py::str>, std::optional<py::str>, std::optional<py::str>>;
extern const py::dict<py::str, Family> FAM;
// PART = {register name: (its family, 0 word / 1 low byte / 2 high byte)}
extern const py::dict<py::str, std::tuple<py::str, int64_t>> PART;
extern const py::str R16;                       // "ax|cx|dx|bx|si|di" (a regex alternative)
extern const py::bytes TABJMP;                  // jmp word ptr cs:[bx + table]
extern bool DEBUG;
extern py::list<int64_t> GWALK;                 // ... and where the compiled code's instructions begin
// the alignment of the last pair found equivalent: [(offset in the original, offset in the compiled)]
extern py::list<std::tuple<int64_t, int64_t>> AMAP;

py::str mask(const py::str& t);

// a relative jump or call -> (length of opcode, displacement) or None
std::optional<std::tuple<int64_t, int64_t>> rel(const py::bytes& b);

// the original's instruction with the registers renamed; None if it cannot be written (SI and DI have no halves)
std::optional<py::str> rename(const py::str& t, const py::dict<py::str, py::str>& mp);

// -> (True, [what was found]) or (False, [where it stopped])
std::tuple<bool, py::list<py::str>> equivalent(const py::bytes& want, const py::bytes& got, const py::set<int64_t>& fx);

// INIT86 of MASM 1.10 was built by yet another state of the compiler: the entry helper of MS-Pascal 3.0x (only the
// frame size behind the call, a frame four bytes smaller, mov sp,bp at the exit), the code still that of 1981 but for
// a few forms - none of the compilers in hand writes it (1981: other helper; 3.04: other registers). Compared
// instruction by instruction with the 1981 compiler's code; allowed, besides equal bytes:
//
//   the entry          call helper / dw frame            against  call helper / db args / dw frame + 4
//   a local            [bp - n]                          against  [bp - (n + 4)]
//   the exit           mov sp,bp                         against  lea sp,[bp - 4]
//   an indexed store   add di,k / mov [di],v             against  mov [di + k],v
//   a byte moved       mov dx,[..] / mov [..],dl         against  mov ax,[..] / mov [..],al
//   a far address      mov dx,k / push ds / push dx      against  mov bx,k / mov dx,ds / mov es,dx / push es / push bx
//   OR of two tests    or al,[temporary]                 against  or ax,[temporary]
std::tuple<bool, py::list<py::str>> equivalent3(const py::bytes& want, const py::bytes& got, const py::set<int64_t>& fx);

}  // namespace codebreaker::exact::habits::newcomp
