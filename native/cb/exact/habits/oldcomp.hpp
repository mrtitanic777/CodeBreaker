// cb/exact/habits/oldcomp.hpp -- is a procedure of M86.EXE the kit compiler's code but for the older compiler's habits?
//
// M86.EXE was built by a compiler a little older than the one in the kit (PAS1/PAS2 of November 1981). For the same
// source the two differ in a few places, each a fixed pattern:
//
//   FOR entry test     older:  mov dx,[lo]  cmp dx,[hi]  ja skip  xchg dx,ax
//                      kit:    mov dx,[hi]  cmp dx,[lo]  jb skip  mov ax,[lo]
//   a >= b, a > b      older:  mov dx,[a]   cmp dx,[b]   mov cx,0  jb / jbe
//   as a value         kit:    mov dx,[b]   cmp dx,[a]   mov cx,0  ja / jae
//   upper bound of a   older:  lea di,[bp+d]  push / mov dx, [di+2]
//   string parameter   kit:    push / mov dx, [bp+d+2]
//   RETURN             older:  lea sp,[bp-n]  pop bp  ret       (the exit, in place)
//                      kit:    jmp exit
//   a pointer kept     older:  mov [bp-t],di  call ...  mov di,[bp-t]      (and a frame two bytes larger)
//   over a call        kit:    call ...  mov di,[bp]  mov di,[di+d]
//   a value kept       older:  add dx,[bp+d]  xchg dx,ax                  (DX still holds [bp+a])
//   in DX              kit:    mov ax,[bp+a]  add ax,[bp+d]
//   a jump out of      older:  jcc target                 (the code is two bytes shorter, still in reach)
//   short reach        kit:    jncc +3 ; jmp target
//
// `equivalent` lays the two procedures side by side byte for byte. Every difference must be one of these patterns, or
// the displacement of a relative jump or call; and every such jump must go to the same place once the two procedures
// are aligned. Nothing is decoded loosely: a byte that fits no pattern fails the procedure. A pattern that needs bytes
// beyond the end of a procedure does not match (a truncated procedure fails, it is not an error).
#pragma once

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::exact::habits::oldcomp {

struct Verdict {
    bool equivalent = false;
    std::vector<std::string> found;   // when equivalent: one line per habit of the older compiler met
    std::string stopped;              // otherwise: where the comparison stopped, and why
};

// `want` is the original's procedure, `got` the kit compiler's. `fixups` are offsets in `got` of bytes the linker
// fills in (relocations): they are taken as equal to whatever the original has there.
Verdict equivalent(ByteView want, ByteView got, const std::set<std::size_t>& fixups);

}  // namespace cb::exact::habits::oldcomp
