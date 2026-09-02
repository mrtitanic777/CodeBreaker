# MSC 6.0a codegen idiom rulebook (grows with every DLL)

Maps disassembly patterns back to the C the engineers wrote, so it compiles byte-identical
under `cl /c /AM /G2 /Gsw /Ox`.  **Append new rules whenever an agent discovers one** — this
file is why later DLLs cost fewer tokens than earlier ones.

## Compiler invocation (fixed)
`cl /c /AM /G2 /Gsw /Ox <mod>.c` — medium model (far code / near data), 286 codegen (`PUSH imm`,
two-immediate DWORD arg pushes), windows loadds prologue + no stack check, max opt.  Linker: NE
stamps 5.20.  Every far function: prologue `1e 58 90 45 55 8bec 1e 8ed8` (the `1e5890` is patched
to `MOV AX,DGROUP` by a load-time fixup), epilogue `4d4d 8be5 1f 5d 4d` + `ca nn 00` (RETF n, pascal)
or `cb` (RETF, cdecl/void).

## Frame / locals
- `SUB SP,2` with no [bp-x] locals = the +2 register-allocation overhead — reproduce with an unused
  `int i;` first local IF the asm shows SUB SP,2 and you otherwise get SUB SP,0.
- `/Gsw` adds a fixed +2 to the frame when locals exist (1 int -> SUB SP,4, 2 int -> SUB SP,6);
  `/Ox` elides an unused local's own space, leaving just the +2 -> SUB SP,2 (+ a trailing NOP).

## Pointers / handles
- Buffer global holding a GlobalAlloc handle in its HIGH word: declare `DWORD g;`, assign
  `g = (DWORD)GlobalAlloc(flags, size) << 16;` — MSC peepholes `(long)x<<16` stored to memory into
  `MOV word[lo],0` (immediate) + `MOV[hi],AX`, no DX/shift, no temp.  `GlobalFree((WORD)(g>>16))` =
  the handle.  Far-ptr null test `if(g)` = `MOV AX,[hi]; OR AX,[lo]` (hi word first).
- For the SAME global used with pointer arithmetic in another segment, declare it `BYTE far *g`
  there.  `g + k` = offset+k, same segment.  `a - b` (both far ptr) = offset subtraction (signed int).
- `FP_OFF(p)`=`*((WORD*)&(p))`, `FP_SEG(p)`=`*((WORD*)&(p)+1)` — lvalue word access to a far pointer.
- GlobalLock returns `void far*` (DX:AX); code that null-checks (`OR DX,AX`) then splits offset→reg /
  segment→[bp-n] stack temp = a transient far-ptr deref where the offset coalesces into a live SI/DI.
  Declare the far-ptr in the INNERMOST scope with immediate use (no function-scope named far-ptr, or
  it gets a full 4-byte stack slot).

## Control flow
- Alloc-shrink retry loop that must keep a TOP `CMP` test + flag-driven `JNZ` back-edge:
  `do { if(cond) break; ...; a-=k; b-=k; } while(b != 0);`.  Plain `while`/`for`/`goto` get rotated
  or jmp-to-bottom by /Ox and will NOT match.
- Shared exit constant (several paths `return 1`): use explicit `goto ret1;` per path + one
  `ret1: return 1;` at the end.  BUT — see the tail-merge note: sometimes the OPPOSITE is right.
- **Tail-merge (LZSEEK):** a shared error block like `GlobalUnlock(); return E;` reached from two
  places is sometimes best written as TWO DUPLICATED `return` statements — MSC's tail-merge then
  emits the single shared block in the exact original position; sharing via `goto` puts it elsewhere.
  Try both when a shared block lands in the wrong spot.
- Cold return placed out-of-line mid-function (e.g. Refill's `return 0x1f4`): MSC basic-block
  ordering — driven by an `if(x!=k){errors} success;` shape (forward JZ puts the success `return 1`
  block last) [seg8 WriteHdr].  Hard to force exactly; may be residual.
- Open-fail path returns the error WITHOUT calling the cleanup (jumps PAST it); cleanup path does the
  cleanup then FALLS INTO the shared error return.  Two labels: `goto ret_err;` (skip) vs
  `errclose: cleanup(); ret_err: return E;`.

## Signedness (drives Jcc choice)
- Loop counters and index compares that use `JL`/`JLE`/`JG` (7c/7e/7f) must be SIGNED `int`.
  `WORD`/`unsigned` gives `JB`/`JBE` (72/76).  Handle-table index `< 0x10` wrap is signed.
- `unsigned char c` for a byte compared unsigned (`JC`/`JBE`); signed `char far*` derefs give `JL`.

## Statement ordering / register allocation
- Eval order is literal.  A call whose result must sit BETWEEN two stores → use the DWORD<<16 idiom
  (not a named temp; a named temp becomes a register var → spurious `PUSH SI/DI` + frame shift).
- The original DLL NEVER uses byte memory-add.  `shift += 8` on a byte global compiles under /Ox to
  `add byte[m],8`; force `mov al,[m]; add al,8; mov[m],al` by writing through a temp:
  `t = shift; t += 8; shift = t;` (the temp optimizes into AL, no stack slot).
- Representing a length as an explicit `int n` can force MSC to keep a pointer's segment in memory and
  reload via `LES` (matches spills the original has) instead of caching it in DI.
- `register` on a far-pointer walker forces it into SI with `LES` load.
- A double-assigned call result (assigned twice, e.g. open then reopen) — MSC may spill it to memory
  where the original kept it in DI; getting MSC to enregister a double-assigned value the original way
  is the hardest residual class (see LZOPENFILE, LZREAD).

## Structs
- `#pragma pack(1)` for on-disk / header structs.  Field type controls the access width; a `char`
  field passed to a function must be byte-typed or MSC emits a spurious `SUB AH,AH`.
- Over-sizing a local buffer by 2 shifts `SUB SP,n` — match the exact declared size from the frame.

## Calling convention (read it off the asm)
- `ADD SP,n` after a `CALLF` = that callee is CDECL → declare `extern T far Name(...)` (no `pascal`).
- No cleanup after the call + callee ends `RETF n` (`ca nn 00`) = PASCAL → `extern T far pascal Name`.
- Intra-segment far call = `PUSH CS; CALL near` (a fixup); inter-segment = `CALLF` (reloc).  Define
  same-segment functions in ONE .c in the SAME ORDER as the asm and the near calls resolve themselves.

## Known residual classes (may need exact MSC sub-version or a 2-byte final-binary patch)
- `CMP reg,-1` on a DEAD register → /Ox emits `INC reg` (1 vs 3 bytes); original used `CMP`.
- Basic-block placement of cold returns.
- Enregistration of double-assigned call results / far-pointer params the original kept in registers.
These do not indicate wrong C — the algorithm is faithful; only MSC's optimizer choice differs.
