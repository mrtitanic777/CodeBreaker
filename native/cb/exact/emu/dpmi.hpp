// cb/exact/emu/dpmi.hpp -- a DPMI host for the real-mode DOS executive, so the period tools that enter 32-bit
// protected mode through a DPMI host (the Win95 DDK's Phar Lap TNT ML.EXE; a DOS-extended Microsoft C pass) run on
// CodeBreaker's own Unicorn CPU instead of DOSBox-X.
//
// The executive builds a Host over its engine and hands it the interrupts a DPMI client makes (INT 2Fh AX=1687h, INT
// 31h, and the mode switch). Everything runs in the one UC_MODE_16 engine the executive already uses: Unicorn honours
// the descriptor D bit in protected mode, so that engine decodes real-mode code, the 16-bit protected-mode stub and a
// 32-bit extender alike. Only the real-to-protected transition needs care, and it is done the hardware way: a
// host-written trampoline runs `mov cr0` and a far jump, which is the one method that leaves Unicorn's cached
// segment state consistent.
//
// What the host provides:
//   * INT 2Fh AX=1687h   report a DPMI host; hand back the mode-switch entry (ES:DI), 32-bit support, processor and
//                        version, and the host-data paragraph count.
//   * the mode switch    on the client's far call to the entry: build a GDT (host stub, flat data, the LDT) and an LDT
//                        (selectors for the client's CS, DS, SS and its PSP), set GDTR and LDTR, and run a trampoline
//                        that turns on CR0.PE and jumps into the client, now in protected mode.
//   * INT 31h            0000/0001 allocate and free LDT descriptors; 0002 segment to descriptor; 0003 selector
//                        increment; 0006/0007 get and set base; 0008 set limit; 0009 set access; 000A alias; 000B/000C
//                        get and set descriptor; 000D allocate a specific descriptor; 0100-0102 allocate, free and
//                        resize DOS memory (on the executive's arena); 0200/0201 real-mode vectors; 0202-0205
//                        protected-mode exception and interrupt handlers (an INT n in protected mode with a handler
//                        set by 0205 enters it as an interrupt gate would; 0204 hands back a host default that serves
//                        the vector itself); 0300 simulate a real-mode interrupt (INT 21h only, straight back through
//                        the executive); 0305/0306 state save/restore and the raw mode switches (a DOS extender's own
//                        real/protected transitions: host code at fixed addresses, driven by parameter blocks); 0400
//                        version (PICs at 08h/70h); 0500-0503 extended memory above 1 MB; 0600-0604 and 0702/0703
//                        paging (no paging here: accepted); 0800/0801 physical mapping (identity); 0900-0902 the
//                        virtual interrupt state; 0A00/0401 unsupported (carry set); 0E00/0E01 the 80x87. A service
//                        not implemented is an EmulationError, so the executive's run fails and the caller can fall
//                        back to DOSBox.
//
// The Win95 DDK's Phar Lap TNT ML.EXE runs to completion on this host (DOSXNT.EXE: a 32-bit DPMI client that
// relocates itself above 1 MB, raw-switches to real mode for each DOS call and hooks INT 21h in protected mode for its
// 32-bit application) with objects byte-identical to DOSBox-X's.
//
// Unicorn facts the host depends on: a CS (or EIP) write inside an interrupt hook takes effect at once in protected
// mode too (a full descriptor load, D bit and all), so protected-mode transfers are done that way; but emu_start takes
// its begin address as CS*16 + IP with IP masked to 16 bits, so a run is only ever restarted (State::again) at a host
// stub whose offset is below 64 KB.
//
// A selector is keyed by `selector & 0xFFFC` (RPL stripped, the LDT bit kept) in State::descriptors so the executive
// turns a protected-mode selector:offset into the same linear byte the real-mode side sees: the identity the reflected
// INT 21h calls rely on.
#pragma once

#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <optional>
#include <unordered_map>

#include "cb/core/bytes.hpp"

namespace cb::exact::emu::dpmi {

// ---- the host's memory, all linear (the real-mode program keeps 0..1 MB) --------------------------------------------
inline constexpr std::uint32_t kEntry = 0x00000660;      // the 2-byte INT-marker the mode-switch entry (ES:DI) points at
inline constexpr std::uint32_t kTrampoline = 0x00000680; // the real-mode to protected-mode trampoline (hardware switch)
inline constexpr std::uint32_t kHost = 0x00110000;       // host tables, just above the real-mode megabyte
inline constexpr std::uint32_t kGdt = kHost + 0x0000;    // the global descriptor table
inline constexpr std::uint32_t kLdt = kHost + 0x0800;    // the local descriptor table (the DPMI descriptors)
inline constexpr std::uint32_t kStub = kHost + 0x2000;   // the host's protected-mode code segment (past the LDT's 512 entries)
inline constexpr std::uint32_t kPool = 0x00200000;       // extended-memory pool (INT 31h 0501): its base
inline constexpr std::uint32_t kPoolTop = 0x01000000;    // ... and its top (16 MB); [kHost, kPoolTop) is host RAM

inline constexpr std::uint16_t kSelStubCode = 0x08;      // GDT: 16-bit code, base = kStub (the landing stub)
inline constexpr std::uint16_t kSelFlatData = 0x10;      // GDT: flat 32-bit data (host use, client flat access)
inline constexpr std::uint16_t kSelLdt = 0x18;           // GDT: the LDT descriptor, loaded into LDTR
inline constexpr std::uint16_t kSelRmData = 0x20;        // GDT: 16-bit data, base 0, 64 KB (the real-mode-compatible state for a switch back)

// The raw mode switches (INT 31h 0306) and the state save/restore entries (0305): host code at fixed addresses,
// driven by parameter blocks the host fills at each switch (no code is rewritten per switch).
inline constexpr std::uint16_t kHostSegment = 0x0060;    // the real-mode host segment (kEntry and kTrampoline live in it too)
inline constexpr std::uint32_t kRawTrampoline = 0x000006A0;   // real mode: cli; CR0.PE = 1; jmp kSelStubCode:kPmLoad
inline constexpr std::uint32_t kRmSave = 0x000006B8;     // real mode: retf (the state save/restore routine: no state to keep)
inline constexpr std::uint32_t kRawRmEntry = 0x000006BC; // real mode: the INT-marker a real-mode client jumps to (to protected mode)
inline constexpr std::uint32_t kRmLoad = 0x000006C0;     // real mode: load DS/ES/SS:SP/FS/GS/EAX from kRmParams, far jump to the target
inline constexpr std::uint32_t kRmParams = 0x00000700;
inline constexpr std::uint32_t kPmLoad = 0x0100;         // offsets in the stub: load the client's state from kPmParams, far jump
inline constexpr std::uint32_t kPmParams = 0x0180;
inline constexpr std::uint32_t kRawPmEntry = 0x0200;     // the INT-marker a protected-mode client jumps to (to real mode)
inline constexpr std::uint32_t kPmToRm = 0x0210;         // real-mode-compatible segments; CR0.PE = 0; jmp kHostSegment:kRmLoad
inline constexpr std::uint32_t kPmSave = 0x0240;         // the protected-mode save/restore routine: (o32) retf
inline constexpr std::uint8_t kRawInt = 0xFE;            // the raw-switch INT-marker
inline constexpr std::uint32_t kDefaultBase = 0x0400;    // stub offsets: the host's default handler of vector n is at kDefaultBase + 4n
inline constexpr std::uint8_t kDefaultInt = 0xFD;        //   (INT FDh; iret): INT FDh runs the host's own service for that vector
inline constexpr std::uint32_t kExceptionBase = 0x0800;  // ... and of exception n at kExceptionBase + 4n
inline constexpr std::uint16_t kLdtFirst = 16;           // the first LDT index the host hands out (selector (index << 3) | 4); 0-15 are kept for
                                                         //   INT 31h 000Dh, which allocates the specific selectors 04h-7Ch
inline constexpr std::uint16_t kLdtCount = 512;          // LDT entries

// One 8-byte segment descriptor. A limit above 1 MB is taken as byte-granular and scaled to 4 KB pages (the G bit is
// set); `granularity` carries the flag nibble in its high four bits (0x80 G, 0x40 D/B).
Bytes descriptor(std::uint32_t base, std::uint32_t limit, std::uint8_t access, std::uint8_t granularity);

// ---- what the host needs of the emulated machine ----------------------------------------------------------------------
// The registers it reads and writes. A 16-bit register's write leaves the rest of the 32-bit one as it was; the 32-bit
// ones are written whole.
enum class Reg { Ax, Bx, Cx, Dx, Si, Di, Sp, Ip, Cs, Ds, Es, Ss, Eax, Ebx, Ecx, Edx, Esi, Edi, Ebp, Esp, Eip, Eflags, Cr0 };
enum class TableReg { Gdtr, Ldtr };

class Engine {
public:
    virtual ~Engine() = default;
    virtual std::uint32_t reg(Reg r) = 0;
    virtual void set_reg(Reg r, std::uint32_t value) = 0;
    // GDTR and LDTR: selector (LDTR), base, limit, and the descriptor flags/access Unicorn takes.
    virtual void set_table(TableReg r, std::uint16_t selector, std::uint32_t base, std::uint32_t limit, std::uint32_t flags) = 0;
    // Linear memory. A read or write of what is not mapped throws EmulationError.
    virtual Bytes read(std::uint32_t address, std::size_t size) = 0;
    virtual void write(std::uint32_t address, ByteView data) = 0;
    // Map [address, address + size) as read/write/execute RAM; mapping what is already mapped is not an error.
    virtual void map_ram(std::uint32_t address, std::uint64_t size) = 0;
    // Stop the current emu_start (the executive restarts it at the registers the host left, when State::again is set).
    virtual void stop() = 0;
};

// A segment's base, limit and access byte as the host keeps them.
struct SegmentInfo {
    std::uint32_t base = 0;
    std::uint32_t limit = 0;
    std::uint8_t access = 0;
    friend bool operator==(const SegmentInfo&, const SegmentInfo&) = default;
};

// The executive's state, as far as the host reads and writes it.
struct State {
    // selector & 0xFFFC -> its segment: how the executive resolves a protected-mode selector:offset to a linear byte
    std::unordered_map<std::uint16_t, SegmentInfo> descriptors;
    bool protected_mode = false;   // the CPU is in protected mode
    bool again = false;            // restart the run at the registers the host left, instead of resuming
    // real-mode interrupt vectors the client has set (INT 31h 0200/0201): vector -> segment:offset
    std::unordered_map<std::uint8_t, Far16> vectors;
    // selectors the executive reads DS and ES from while it serves a reflected INT 21h (instead of the CPU registers)
    std::optional<std::uint16_t> override_ds, override_es;
    // a segment the reflected INT 21h returned (ES:BX of 35h/52h)
    std::optional<std::uint16_t> out_ds, out_es;
};

// What the executive's DOS memory arena answered to a resize.
struct ArenaResult {
    enum class Kind { Done, TooLarge, Failed };
    Kind kind = Kind::Done;
    std::uint16_t value = 0;   // TooLarge: the largest block available, in paragraphs; Failed: the DOS error (7 or 9)
};

// What the executive hands the host: the engine, its state and the real-mode services of DOS.
class Context {
public:
    virtual ~Context() = default;
    virtual Engine& engine() = 0;
    virtual State& state() = 0;
    virtual std::uint16_t psp() = 0;   // the program's PSP segment
    virtual void int21() = 0;          // the executive's INT 21h handler, on the registers as they stand
    virtual void dos_int(std::uint8_t number) = 0;   // its handler of real-mode interrupt `number`
    virtual std::optional<std::uint16_t> allocate(std::uint16_t paragraphs) = 0;   // the block's segment; none: no block that large
    virtual std::uint16_t largest() = 0;                                           // the largest free block, in paragraphs
    // Free a block; the DOS error (7 or 9) when it is not one.
    virtual std::optional<std::uint16_t> free(std::uint16_t segment) = 0;
    virtual ArenaResult resize(std::uint16_t segment, std::uint16_t paragraphs) = 0;
};

class Host {
public:
    // The host is attached to the executive's context for the life of the run (the context outlives it).
    explicit Host(Context& ctx);

    // The interrupt surface the executive routes here: true if `number` is a DPMI one and was served. Throws
    // EmulationError for a service this host does not provide, or an exception that reached its default handler.
    bool handle(std::uint8_t number);

    bool in_protected_mode() const { return protected_mode_; }

private:
    struct Gate {   // a protected-mode handler the client set: selector:offset
        std::uint16_t selector = 0;
        std::uint32_t offset = 0;
    };
    struct MemoryBlock {
        std::uint32_t base = 0, size = 0;
    };
    struct TableEntry {   // a descriptor as the table holds it: its segment, and the flag nibble (0x80 G, 0x40 D/B) as the high four bits
        SegmentInfo segment;
        std::uint8_t flags = 0;
    };

    void int2f_1687();
    void switch_to_protected();
    TableEntry table_entry(std::uint16_t selector);   // read back from the descriptor table in memory
    void push_words(std::initializer_list<std::uint32_t> values);
    void dispatch(std::uint8_t number);
    void default_interrupt();
    void write_vector_stubs();
    void write_raw_code();
    void raw_switch();
    std::uint16_t make_ldt(std::uint32_t base, std::uint32_t limit, std::uint8_t access, std::optional<std::uint16_t> index = std::nullopt);
    void int31();
    SegmentInfo get(std::uint16_t selector);
    void set(std::uint16_t selector, std::optional<std::uint32_t> base, std::optional<std::uint32_t> limit, std::optional<std::uint8_t> access);
    std::uint32_t es_edi();
    std::optional<std::uint32_t> pool_alloc(std::uint32_t size);
    void memory_info();
    void vectors(std::uint16_t fn, std::uint16_t bx, std::uint16_t cx, std::uint16_t dx);
    void simulate_real_mode_interrupt();

    // register helpers: the engine's, with the carry flag and the 16-bit result registers named
    std::uint32_t r(Reg reg) { return ctx_.engine().reg(reg); }
    void w(Reg reg, std::uint32_t value) { ctx_.engine().set_reg(reg, value); }
    void carry(bool on);

    Context& ctx_;
    bool protected_mode_ = false;
    std::unordered_map<std::uint16_t, SegmentInfo> ldt_used_;   // LDT index -> its descriptor
    std::uint16_t next_ldt_ = kLdtFirst;
    std::unordered_map<std::uint32_t, MemoryBlock> memory_;     // INT 31h 0501 allocations by handle
    std::uint32_t pool_ = kPool;
    std::uint32_t next_handle_ = 1;
    std::optional<std::uint16_t> scratch_ds_, scratch_es_;      // two reusable selectors for the reflection of INT 31h 0300
    std::unordered_map<std::uint8_t, Gate> interrupt_handlers_; // the client's protected-mode interrupt handlers (0205)
    std::unordered_map<std::uint8_t, Gate> exception_handlers_; // ... and exception handlers (0203: kept, never raised here)
    bool client32_ = false;
    std::uint8_t virtual_interrupts_ = 1;                       // the virtual interrupt flag (0900-0902)
    std::unordered_map<std::uint16_t, std::uint16_t> segment_selectors_;   // 0002: real-mode segment -> its selector
};

}  // namespace cb::exact::emu::dpmi
