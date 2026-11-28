# Memory management in the converted programs' runtime

A converted Java program keeps Java's object model: an object is a plain C++ object reached through a raw pointer
(`World* worldObj`), a field read is a load, a call is a C++ call. Until now nothing freed an object (`System.gc()` did
nothing): every object Java would have collected and the converter's ownership analysis did not delete leaked. Arrays
(`JArray`) count their references since 9b8a19fa3 and free themselves; this document is about everything else.

## What Java needs

- Objects die when nothing reaches them, cycles included (an entity points at its world, the world lists its entities).
- Any thread may hold or hand over references (Minecraft runs a game thread, chunk and network threads; the audio callback
  runs on SDL's thread).
- Nothing in a hot path changes: field access, calls and array element access stay plain.

## The choice

| | Hot paths | Cycles | Converter changes | Verdict |
|---|---|---|---|---|
| Reference counting (smart pointers in every field and local) | every pointer store and copy becomes an interlocked increment and decrement | leak, so a cycle collector is needed on top | every field, local, parameter and return type changes; j2x must read them back | slower, and the largest change |
| Precise tracing (the emitter writes a trace function per class) | unchanged | collected | a trace method in every class, every runtime container made traceable, every golden changes; stacks still need a conservative scan | a large change, and still conservative at the roots |
| **Conservative mark-sweep over the whole C++ heap** | **unchanged** | **collected** | **none: the collector replaces `operator new`** | **chosen** |

The collector replaces the program's global `operator new`/`operator delete`. Every C++ allocation (objects, the storage of
`JList`/`JMap`/`JSet`, strings, boxes, array blocks) comes from its heap, and a block nothing reaches is reused. It is
non-moving, so raw pointers stay valid; it is conservative, so any word that looks like a pointer into a block keeps that
block alive (interior pointers too: a pointer to a base subobject, or the element pointer a `JArray` holds).
An explicit `delete` (the converter's ownership deletes, the runtime's own) still frees at once, and `JArray` blocks still free
themselves when their last reference goes: the collector only takes what nothing reaches. No destructor runs for a collected
object (Java has none); what it owned is unreachable with it and collected too.

## How it works (config/java/platform/pc/companions/J2xGc.cpp)

- **Heap.** 48 block sizes from 16 bytes to 128 KB (16-byte steps to 128, then four per power of two), each twice: blocks
  that may hold pointers, and "atomic" blocks that never do and are never scanned. A size's blocks live in segments of
  256 KB to 4 MB from the OS (VirtualAlloc), each with an allocated and a marked bitmap. Blocks over 128 KB are regions of
  their own; a few freed ones (16 MB in all) are kept for reuse. A two-level page map (64 KB pages) finds the segment of any
  address, a reciprocal multiply the block. Pointer blocks are zeroed when handed out, so none carries stale pointers.
- **Allocation without a lock.** Each thread claims free blocks a bitmap word at a time (about 16 KB of a size) under the
  heap's lock and hands them out from its own cache with no lock and no atomic operation. A free clears the block's bit with
  one atomic AND. Freed blocks are reused before the heap grows.
- **Pointer-free blocks.** `JArray<T>` of a scalar `T` (int, byte, float, ...), the NIO buffers' bytes and the decoded audio
  are allocated atomic through `j2xAllocAtomic` and `J2xAtomicAllocator` (JArray.h): chunk data, light maps, vertex, pixel and
  sample buffers are never scanned, so their numbers cannot be taken for pointers and marking skips most of the heap's bytes.
- **When.** A collection runs in the allocating thread when the live bytes (allocated minus freed) have grown by half since
  the last one, and by 64 MB at least (32 MB in a 32-bit program). `System.gc()` collects at once. Empty segments are kept
  up to what the program will fill before the next collection; the rest go back to the OS.
- **Roots.** The program image's writable sections (statics), and every attached thread's registers and stack from its stack
  pointer to its base. A thread attaches on its first allocation, when a `Thread` starts it, or through the attach hook
  (the SDL audio callback calls it); an ended thread is detached at the next collection.
- **Stop the world.** The collector holds the heap's lock, suspends every other attached thread (SuspendThread, then
  GetThreadContext, which waits for the suspension), marks, resumes them, and sweeps (`allocated &= marked` a word at a
  time). The marker calls nothing that takes a lock a suspended thread could hold (no malloc, no stdio). While a `Thread`
  is being started (its object is reachable only through the new OS thread's start argument until the thread runs)
  collections wait.
- **Switches.** `J2X_GC=0` never collects (the heap stays); `J2X_GC_LOG=1` prints each collection to stderr (pause, live
  bytes before and after, what reached them, what the heap holds from the OS); `J2X_GC_LOG=2` also where the process's
  private memory is, at each collection and each new high.

## What must hold for it to be right

- Every reference the program keeps is in a scanned place: an attached thread's stack or registers, a static, or a block of
  the heap. Memory from `malloc`, the OS or a DLL is not scanned; the runtime keeps no references there (audited: no SDL
  user data, no thread-local references, no `malloc` in the PC layer; a starting thread is covered above).
- The C++ library allocates through the replaced `operator new`, so MinGW builds link libstdc++ statically (a DLL libstdc++
  allocates strings with its own `operator new`). A pointer the collector did not allocate, handed to its `operator delete`,
  goes to `free`.
- Pointers are not hidden (no XOR-ed or compressed pointers): the converted code and the runtime have none.
- The Xbox 360 runtime shares the canonical headers. There JArray.h's hooks stay null: plain `operator new`, no collection,
  as before. The collector itself is a PC companion (Windows x86 and x64; MinGW GCC, Clang, MSVC).

## Measured (2026-11-27, a busy 16-core machine)

Allocation, a standalone test (work/jarray/gcunit), best of three, ms, collector vs. the C heap (64-bit):

| | MinGW GCC | MSVC |
|---|---|---|
| 20 M small objects made and dropped (collected) vs. made and deleted (C heap) | 430 vs. 889 | 242 vs. 960 |
| 20 M small objects made and deleted | 892 vs. 853 | 374 vs. 960 |
| 200 k `int[8192]` temporaries | 53 vs. 59 | 49 vs. 67 |
| 4 threads, 5 M small objects each | 238 vs. 362 | 162 vs. 378 |

Peak private memory, MB, before -> after, each run converting, building with MinGW, entering a world, walking, opening
the inventory and quitting cleanly (exit 0):

| Version | Direct3D 8 (32-bit) | OpenGL (64-bit) | Direct3D 11 (64-bit) |
|---|---|---|---|
| in-20091231-2255 | 1542, 3630 (killed at 600 s), 376 -> 475, 489, 479 | | |
| in-20100110 | 300, 285, 199 -> 282, 228, 228 | 344 -> 332 | 1200 -> 1327 |
| a1.2.6 | | 876 -> 513 | 802 -> 685 |
| b1.7.3 | | 832 -> 478 | 803 -> 775 |
| b1.8.1 | | 901 -> 564 | 1017 -> 837 |
| b1.9-pre5 | | 682 -> 488 | 709 -> 603 |

Pauses: the longest is 30 to 110 ms on 64-bit (marking about 150 to 250 MB of live objects), up to 275 ms on 32-bit; in
in-20091231-2255 the pauses add up to 0.2 to 0.4 % of the run. Most of a Direct3D 11 program's peak is the driver's, not the heap's.
