// codebreaker/exact/emu/dpmi.hpp -- C++ port of codebreaker.exact.emu.dpmi.
//
// dpmi.py -- a DPMI host for the real-mode DOS executive (exact/emu/dos.py), so the period tools
// that enter 32-bit protected mode through a DPMI host (the Win95 DDK's Phar Lap TNT ML.EXE; a DOS
// -extended Microsoft C pass) run on CodeBreaker's own Unicorn CPU instead of DOSBox-X.
//
// It plugs into dos.run() as `dpmi=True`: dos.run builds a Host, hands it the engine and the real
// -mode INT 21h handler, and routes INT 2Fh/1687h, INT 31h and the mode-switch to it. Everything runs
// in the SINGLE UC_MODE_16 engine dos.run already uses -- Unicorn (QEMU) honours the descriptor D-bit
// in protected mode, so that one engine decodes real-mode code, the 16-bit protected-mode stub and the
// 32-bit protected-mode extender correctly; only the real->protected transition needs care, and that is
// done the hardware way (a host-written trampoline runs `mov cr0` + a far jump), which is the one method
// that leaves QEMU's cached segment state consistent.
//
// What the host provides:
//   * INT 2Fh AX=1687h   report a DPMI host; hand back the mode-switch entry (ES:DI), 32-bit support,
//                        processor and version, and the host-data paragraph count.
//   * the mode-switch     on the client's FAR CALL to the entry: build a GDT (host stub + flat data +
//                        the LDT) and an LDT (selectors for the client's CS/DS/SS and its PSP), set
//                        GDTR/LDTR, and run a trampoline that turns on CR0.PE and jumps into the client
//                        now in protected mode.
//   * INT 31h services    0000/0001 allocate/free LDT descriptors; 0002 segment to descriptor; 0003
//                        selector increment; 0006/0007 get/set base; 0008 set limit; 0009 set access;
//                        000A alias; 000B/000C get/set descriptor; 000D allocate a specific descriptor;
//                        0100/0101/0102 allocate/free/resize DOS memory (on dos.py's MCB arena);
//                        0200/0201 real-mode vectors; 0202-0205 protected-mode exception and interrupt
//                        handlers (0205's are CALLED: an INT n in protected mode enters the client's
//                        handler as an interrupt gate would; 0204 hands back a host default that serves
//                        the vector itself); 0300 simulate a real-mode interrupt (routed straight back
//                        through dos.run's int21); 0305/0306 state save/restore and the RAW MODE
//                        SWITCHES (a DOS extender's own real<->protected transitions: host code at fixed
//                        addresses, driven by parameter blocks); 0400 version (PICs at 08h/70h); 0500-
//                        0503 extended memory above 1 MB; 0600-0604, 0702/0703 paging (no-ops: no
//                        paging); 0800/0801 physical mapping (identity); 0900-0902 virtual interrupt
//                        state; 0A00/0401 unsupported (CF); 0E00/0E01 the 80x87. A service not
//                        implemented raises, so the whole run falls back to DOSBox.
//
// The Win95 DDK's Phar Lap TNT ML.EXE runs to completion on this host (DOSXNT.EXE: a 32-bit DPMI
// client that relocates itself above 1 MB, raw-switches to real mode for each DOS call and hooks INT
// 21h in protected mode for its 32-bit application) with objects byte-identical to DOSBox-X's.
//
// Unicorn notes the host depends on: a CS (or EIP) write INSIDE an interrupt hook takes effect at once in
// protected mode too -- a full descriptor load, D bit and all -- so protected-mode transfers are done
// that way; but emu_start's begin address is taken as CS*16 + IP with IP masked to 16 bits, so a run is
// only ever RESTARTED (st["again"]) at a host stub whose offset is below 64 KB.
//
// A selector is keyed by `sel & 0xFFFC` (RPL stripped, the LDT bit kept) in st["desc"] so dos.py's
// lin() turns a protected-mode (selector:offset) address into the same linear byte the real-mode side
// sees -- the identity the reflected INT 21h calls rely on.
//
// In the Python the host is handed a context object by dos.run (`ctx`: the unicorn engine, the executive's
// state dict, its INT 21h handler and register readers, its arena). Here those are the Engine, State and
// Context types below; dos.run's port implements Context. UnicornEngine is an Engine over Unicorn's C API
// in the unicorn.dll the Python package carries (load_unicorn hands over its path once, as xenix does).
#pragma once

#include <optional>
#include <tuple>

#include "codebreaker/exact/emu/xenix.hpp"
#include "py.hpp"

namespace codebreaker::exact::emu::dpmi {

// unicorn.UcError: the engine refused (a memory access outside what is mapped ...).
using UcError = codebreaker::exact::emu::xenix::UcError;

// --- the host's memory, all linear (the real-mode program keeps 0..1 MB) -----------------------
inline constexpr int64_t ENTRY = 0x00000660;      // the 2-byte INT-marker the mode-switch entry (ES:DI) points at
inline constexpr int64_t TRAMP = 0x00000680;      // the real-mode -> protected-mode trampoline (hardware switch)
inline constexpr int64_t HOST = 0x00110000;       // host tables, just above the real-mode megabyte
inline constexpr int64_t GDT = HOST + 0x0000;     // global descriptor table
inline constexpr int64_t LDT = HOST + 0x0800;     // local descriptor table (the DPMI descriptors)
inline constexpr int64_t STUB = HOST + 0x2000;    // the host's protected-mode code segment (past the LDT's 512 entries)
inline constexpr int64_t POOL = 0x00200000;       // extended-memory pool (INT 31h 0501) base
inline constexpr int64_t POOLTOP = 0x01000000;    // ... and top (16 MB)
inline constexpr int64_t HOSTTOP = POOLTOP;       // map 0x110000..HOSTTOP as fresh host RAM

inline constexpr int64_t SEL_STUBCODE = 0x08;     // GDT: 16-bit code, base = STUB (the landing stub)
inline constexpr int64_t SEL_FLATDATA = 0x10;     // GDT: flat 32-bit data (host use / client flat access)
inline constexpr int64_t SEL_LDT = 0x18;          // GDT: the LDT descriptor -> LDTR
inline constexpr int64_t SEL_RMDATA = 0x20;       // GDT: 16-bit data, base 0, 64 KB (the real-mode-compatible state for a switch back)

// the raw mode switches (INT 31h 0306) and the state save/restore entries (0305): host code at fixed
// addresses, driven by parameter blocks the host fills at each switch (no code is rewritten per switch)
inline constexpr int64_t HSEG = 0x0060;           // the real-mode host segment (ENTRY / TRAMP live in it too)
inline constexpr int64_t RAW_TRAMP = 0x000006A0;  // real mode: cli ; CR0.PE=1 ; jmp SEL_STUBCODE:PM_LOAD
inline constexpr int64_t RM_SAVE = 0x000006B8;    // real mode: retf (the state save/restore routine: no state to keep)
inline constexpr int64_t RAW_RM_ENTRY = 0x000006BC;  // real mode: the INT-marker a real-mode client jumps to (to protected mode)
inline constexpr int64_t RM_LOAD = 0x000006C0;    // real mode: load DS/ES/SS:SP/FS/GS/EAX from RM_PARAMS, jmp far to the target
inline constexpr int64_t RM_PARAMS = 0x00000700;
inline constexpr int64_t PM_LOAD = 0x0100;        // STUB offsets: load the client's protected-mode state from PM_PARAMS, jmp far
inline constexpr int64_t PM_PARAMS = 0x0180;
inline constexpr int64_t RAW_PM_ENTRY = 0x0200;   // the INT-marker a protected-mode client jumps to (to real mode)
inline constexpr int64_t PM_TO_RM = 0x0210;       // real-mode-compatible segments ; CR0.PE=0 ; jmp HSEG:RM_LOAD
inline constexpr int64_t PM_SAVE = 0x0240;        // the protected-mode save/restore routine: (o32) retf
inline constexpr int64_t RAW_INT = 0xFE;          // the raw-switch INT-marker
inline constexpr int64_t DEF_BASE = 0x0400;       // STUB offsets: the host's default protected-mode handler of vector n at DEF_BASE + 4n
inline constexpr int64_t DEF_INT = 0xFD;          //   (INT FDh ; iret): INT FDh runs the host's own service for that vector
inline constexpr int64_t EXC_BASE = 0x0800;       // ... and of exception n at EXC_BASE + 4n
inline constexpr int64_t LDT_FIRST = 16;          // first LDT index the host hands out (selector (idx<<3)|4); 0-15 are kept for
                                                  //   INT 31h 000Dh, which allocates the specific selectors 04h-7Ch
inline constexpr int64_t LDT_COUNT = 512;         // LDT entries

// one 8-byte descriptor; limit > 1 MB is taken as byte-granular and scaled to 4 KB pages.
py::bytes _desc(int64_t base, int64_t limit, int64_t access, int64_t gran);

// The emulated CPU, as the host drives it. Register ids are Unicorn's own (UC_X86_REG_*); memory is the
// machine's linear space.
class Engine {
  public:
    virtual ~Engine() = default;
    virtual int64_t reg_read(int64_t reg) = 0;
    virtual void reg_write(int64_t reg, int64_t value) = 0;
    // reg_write(GDTR / LDTR / ..., (selector, base, limit, flags))
    virtual void reg_write_mmr(int64_t reg, int64_t selector, int64_t base, int64_t limit, int64_t flags) = 0;
    virtual py::bytes mem_read(int64_t address, int64_t size) = 0;
    virtual void mem_write(int64_t address, const py::bytes& data) = 0;
    virtual void mem_map(int64_t address, int64_t size, int64_t perms) = 0;   // UcError if it cannot
    virtual void emu_stop() = 0;
};

// The executive's state dict, the keys the host uses.
struct State {
    py::dict<int64_t, std::tuple<int64_t, int64_t, int64_t>> desc;   // st["desc"]: selector & 0xFFFC -> (base, limit, access)
    bool pm = false;                                                  // st["pm"]
    bool again = false;                                               // st["again"]
    py::dict<int64_t, std::tuple<int64_t, int64_t>> vec;              // st["vec"]: real-mode vector -> (segment, offset)
    std::optional<int64_t> ov_ds;                                     // st["ov_ds"] / ["ov_es"]: selectors int21 reads ds/es from
    std::optional<int64_t> ov_es;
    std::optional<int64_t> out_ds;                                    // st["out_ds"] / ["out_es"]: a segment int21 returned
    std::optional<int64_t> out_es;
};

// ctx.ArenaError: the arena refused (DOS error 7 or 9); args[0] is the DOS error.
struct ArenaError : py::Exception {
    int64_t code;
    explicit ArenaError(int64_t c) : py::Exception(std::to_string(c)), code(c) {}
    const char* type() const override { return "ArenaError"; }
};

// What dos.run hands the host (ctx): the engine and the real-mode INT 21h handler.
class Context {
  public:
    virtual ~Context() = default;
    virtual Engine& mu() = 0;
    virtual State& st() = 0;
    virtual int64_t psp() = 0;                                  // ctx.psp: the program's PSP segment
    virtual int64_t R(int64_t reg) = 0;                         // ctx.R / ctx.W: the module's register readers/writers
    virtual void W(int64_t reg, int64_t value) = 0;
    virtual void carry(bool on) = 0;
    virtual void int21() = 0;
    virtual void dos_int(int64_t n) = 0;                        // the executive's handler of real-mode interrupt n
    virtual std::optional<int64_t> alloc(int64_t paragraphs) = 0;   // None: no block that large
    virtual int64_t largest() = 0;
    virtual void free(int64_t segment) = 0;                     // may raise ArenaError
    virtual std::optional<int64_t> resize(int64_t segment, int64_t paragraphs) = 0;   // None: done; else the largest; may raise ArenaError
};

class Host {
  public:
    Host() = default;

    Context* ctx = nullptr;
    bool pm = false;
    py::dict<int64_t, std::tuple<int64_t, int64_t, int64_t>> ldt_used;   // LDT index -> (base, limit, access)
    int64_t next_ldt = LDT_FIRST;
    py::dict<int64_t, std::tuple<int64_t, int64_t>> vectors;             // real-mode vector overrides set via INT 31h 0200/0201
    py::dict<int64_t, std::tuple<int64_t, int64_t>> mem;                 // handle -> (linear base, bytes) of 0501 allocations
    int64_t pool = POOL;
    int64_t next_handle = 1;
    std::optional<int64_t> scratch_ds;                                   // two reusable selectors for INT 31h 0300 reflection
    std::optional<int64_t> scratch_es;
    py::dict<int64_t, std::tuple<int64_t, int64_t>> pm_vec;              // protected-mode interrupt handlers the client set (0205)
    py::dict<int64_t, std::tuple<int64_t, int64_t>> exc_vec;             // ... and exception handlers (0203: kept, never raised here)
    bool client32 = false;
    int64_t vif = 1;                                                     // the virtual interrupt flag (0900-0902)
    py::dict<int64_t, int64_t> segsel;                                   // 0002: real-mode segment -> its selector

    // -- wiring from dos.run ---------------------------------------------------------------------
    // ctx exposes the engine and the real-mode INT 21h handler.
    void attach(Context& ctx);

    // -- the interrupt surface dos.run routes here -----------------------------------------------
    // True if this interrupt is a DPMI one and was serviced.
    bool handle(int64_t intno);

  private:
    void _int2f_1687();
    void _switch();
    std::tuple<int64_t, int64_t, int64_t, int64_t> _seg(int64_t sel);
    void _push(const py::list<int64_t>& values);
    void _dispatch(int64_t intno);
    void _default_int();
    void _write_vector_stubs();
    void _write_raw_code();
    void _raw_switch();
    int64_t _make_ldt(int64_t base, int64_t limit, int64_t access, std::optional<int64_t> index = std::nullopt);
    int64_t _ldt_index(int64_t sel);
    void _int31();
    std::tuple<int64_t, int64_t, int64_t> _get(int64_t sel);
    int64_t _base(int64_t sel);
    void _set(int64_t sel, std::optional<int64_t> base = std::nullopt, std::optional<int64_t> limit = std::nullopt,
              std::optional<int64_t> access = std::nullopt);
    int64_t _lin_esedi();
    std::optional<int64_t> _pool_alloc(int64_t size);
    void _mem_info();
    void _vectors(int64_t fn, int64_t bx, int64_t cx, int64_t dx);
    void _simulate_rm_int();
};

// Hand over the path of unicorn.dll (the one Python's `unicorn` package carries). RuntimeError if it
// cannot be loaded.
void load_unicorn(const py::str& dll_path);

// A UC_MODE_16 x86 engine on Unicorn's C API.
class UnicornEngine : public Engine {
  public:
    UnicornEngine();                       // RuntimeError if load_unicorn has not been called
    ~UnicornEngine() override;
    UnicornEngine(const UnicornEngine&) = delete;
    UnicornEngine& operator=(const UnicornEngine&) = delete;
    int64_t reg_read(int64_t reg) override;
    void reg_write(int64_t reg, int64_t value) override;
    void reg_write_mmr(int64_t reg, int64_t selector, int64_t base, int64_t limit, int64_t flags) override;
    py::bytes mem_read(int64_t address, int64_t size) override;
    void mem_write(int64_t address, const py::bytes& data) override;
    void mem_map(int64_t address, int64_t size, int64_t perms) override;
    void emu_stop() override;
    // reg_read of a descriptor-table register -> (selector, base, limit, flags)
    std::tuple<int64_t, int64_t, int64_t, int64_t> reg_read_mmr(int64_t reg);
    // emu_start(begin, until, timeout=0, count)
    void emu_start(int64_t begin, int64_t until, int64_t timeout = 0, int64_t count = 0);

  private:
    void* uc_ = nullptr;
};

}  // namespace codebreaker::exact::emu::dpmi
