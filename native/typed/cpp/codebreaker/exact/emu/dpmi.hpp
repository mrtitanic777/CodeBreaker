// codebreaker/exact/emu/dpmi.hpp -- compat shim for cb/exact/emu/dpmi.hpp (docs/CPP_DESIGN.md 6.3).
//
// The DPMI host itself (Host, its State and Context, the INT 31h services) is cb::exact::emu::dpmi::Host now. What the
// rest of the old layer used from this header stays: the abstract Engine of the DOS executive's MCB arena code, the two
// exception types, and the UnicornEngine over the unicorn.dll the Python package carries.
//   COMPAT(py): UnicornEngine, load_unicorn -- remove when exact/emu/dos and build/native_dos are refactored (they reach
//   Unicorn through the DLL path the program hands over; cb::emu::unicorn loads the prebuilt DLL beside the exe instead)
#pragma once

#include <tuple>

#include "codebreaker/exact/emu/xenix.hpp"
#include "py.hpp"

namespace codebreaker::exact::emu::dpmi {

// unicorn.UcError: the engine refused (a memory access outside what is mapped ...).
using UcError = codebreaker::exact::emu::xenix::UcError;

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

// ctx.ArenaError: the arena refused (DOS error 7 or 9); args[0] is the DOS error.
struct ArenaError : py::Exception {
    int64_t code;
    explicit ArenaError(int64_t c) : py::Exception(std::to_string(c)), code(c) {}
    const char* type() const override { return "ArenaError"; }
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
