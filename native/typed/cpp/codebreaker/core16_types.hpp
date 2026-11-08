// codebreaker/core16_types.hpp -- records shared by the 33 modules of the "core16" import cycle
// (analyze.*, build.*, decompile.*, emulate.*, engine.lanes, source.*, symbols.*, verify.*), so that their
// headers do not include each other in a circle.  Each group edits only its own section.
//
// Conventions: an address is an int64_t; a 16-bit module's address is (selector << 16) + offset (decompile::describe);
// "(seg, off)" is std::tuple<int64_t, int64_t>.
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <utility>
#include <variant>

#include "codebreaker/translate/ir.hpp"   // (group 4: FunctionNotes holds an L2 document, translate::ir::Dict)
#include "py.hpp"

namespace codebreaker::core16 {

using SegOff = std::tuple<int64_t, int64_t>;

// ==== GROUP 1 (analyze.*, decompile.frame16, decompile.shape16) types -- append below ====

// (Group 1 uses G4's core16::L2Doc = translate::ir::Dict for the L2 document of frame16 / shape16.)

// ==== GROUP 2 (symbols.*, source.*, engine.lanes) types -- append below ====

// A C++ class found in a module: one row of analyze::classes::hierarchy(img) (the dict with keys vtable, slots,
// methods, name, bases, evidence, size, size_exact, installed_by, com, message_map), also what
// source::classes_writer::_powerpc_tables makes for a PowerPC image (there without com/message_map/size).
// analyze.classes (group 1) should return py::list<ClassRecord> from hierarchy().
struct ClassRecord {
    int64_t vtable = 0;                       // address of the table of virtual functions
    int64_t slots = 0;                        // how many virtual functions
    py::list<int64_t> methods;                // each slot's function address
    py::str name;                             // "class_<hex vtable>" when the binary does not state it
    py::list<py::str> bases;
    py::list<py::str> evidence;               // where each fact came from
    std::optional<int64_t> size;              // the object's size
    std::optional<bool> size_exact;           // None: taken as exact (the writer's default True)
    py::list<int64_t> installed_by;           // functions that install the table (constructors/destructors)
    bool com = false;                         // has the shape of a COM interface
    std::optional<int64_t> message_map;       // MFC message map address
};
// ==== GROUP 3 (decompile.describe, emulate.pcode, verify.gate16.contract16) types -- append below ====

// (Group 3 declares no shared record of its own: Description::notes holds group 4's FunctionNotes, below.)
// ==== GROUP 4 (decompile.c_text, decompile.decompiler, build.functions) types -- append below ====

// The L2 document of one decompiled function: translate::ir::Dict (a py::dict<py::str, ir::Val>, shared by reference
// as the Python's dict is, so a pass that rewrites a node in place is seen by every holder). The ported C back end (translate/backends/c.hpp, native_l2.hpp) takes ir::Dict, so the
// notes below carry that.
using L2Doc = translate::ir::Dict;

// A structure the native decompiler recovered from a variable's field accesses:
// [name, bytes, [[offset, bytes, type], ...]] (decompiler.split_notes /STRUCT_FIELD). `type` is one word, a pointer's
// ending in `*` (or `^`, a 16-bit far pointer).
struct StructField {
    int64_t offset = 0;
    int64_t bytes = 0;
    py::str type;
};
struct StructNote {
    py::str name;
    int64_t bytes = 0;
    py::list<StructField> fields;
};
// A global a function touches: [address, bytes, bytes pointed at] (the decompiler's /*@G ...*/ note)
struct GlobalNote {
    int64_t address = 0;
    int64_t bytes = 0;
    int64_t elem = 0;
};
// What the native decompiler says of a function besides its C (decompiler.split_notes), and what the C back end adds
// when it writes the C from the document (c_from_l2): the Python's "facts" dict, whose keys are fixed.
struct FunctionNotes {
    py::list<GlobalNote> globals;                     // "globals"
    py::dict<py::str, int64_t> pointers;              // "pointers": {name: bytes} (16-bit; `return` its result)
    py::list<StructNote> structs;                     // "structs"
    std::optional<L2Doc> l2;                          // "l2": its L2 document (None: no document)
    bool placed = false;                              // "placed": frame16/shape16/asm_blocks passes were applied to l2
    bool from_l2 = false;                             // "from_l2": the text handed on is the C back end's
};
// ==== GROUP 5 (build.msc6, build.asm16, verify.gate16.isolated16, verify.reconstruct, verify.lanes.win16) types -- append below ====
// ==== GROUP 6 (build.module, verify.lanes.win32) types -- append below ====
// ==== GROUP 7 (emulate.emulator, emulate.native_shadow, emulate.shadow, emulate.windows16) types -- append below ====

// ---- Uc: the Unicorn engine, as the unicorn package's `Uc` gives it ------------------------------------
// Python's `uc` (unicorn.Uc(UC_ARCH_X86, UC_MODE_32), optionally sped up by emulate/fast_uc.py) is an object the
// emulators, the shadows and the lanes pass around and call methods on; C++ has no unicorn package, so the host
// supplies a subclass wrapping the unicorn C library (its `Uc* uc` is `handle()`, Python's `uc._uch`).  Registers
// are named by Unicorn's integer ids (unicorn.x86_const.UC_X86_REG_*; emulator::_uc_reg(name) looks one up).
// Every method that fails in Unicorn throws emulate::native_trace::UcError (unicorn.UcError).
class Uc {
  public:
    virtual ~Uc() = default;
    virtual void* handle() const = 0;                                          // uc._uch
    virtual void mem_map(int64_t address, int64_t size) = 0;
    virtual void mem_unmap(int64_t address, int64_t size) = 0;
    virtual py::bytes mem_read(int64_t address, int64_t size) = 0;
    virtual void mem_write(int64_t address, const py::bytes& data) = 0;
    virtual int64_t reg_read(int64_t reg) = 0;                                 // an integer register
    virtual double reg_read_float(int64_t reg) = 0;                            // ST0 (an x87 register)
    virtual void reg_write(int64_t reg, int64_t value) = 0;
    // GDTR / LDTR: written as the (selector, base, limit, flags) tuple Unicorn takes
    virtual void reg_write_descriptor(int64_t reg, const std::tuple<int64_t, int64_t, int64_t, int64_t>& value) = 0;
    virtual std::shared_ptr<void> context_save() = 0;                          // uc.context_save()
    virtual void context_restore(const std::shared_ptr<void>& context) = 0;
    virtual void ctl_flush_tb() = 0;
    virtual void emu_start(int64_t begin, int64_t until, int64_t timeout = 0, int64_t count = 0) = 0;
    virtual void emu_stop() = 0;
    // uc.hook_add(UC_HOOK_CODE, callback, None, begin, end): the callback gets (uc, address, size)
    virtual void hook_add_code(const std::function<void(Uc&, int64_t address, int64_t size)>& callback, int64_t begin,
                               int64_t end) = 0;
    // emulate/fast_uc.py speed_up(uc): the same reads, for less (a no-op where the binding is not the expected one)
    virtual void speed_up() {}
};

}  // namespace codebreaker::core16
