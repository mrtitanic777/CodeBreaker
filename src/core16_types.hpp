// core16_types.hpp -- the records and the emulator interface shared by the modules of the "core16" cycle (analyze,
// build, decompile, emulate, engine lanes, source, symbols, verify), so that their headers do not include each other in a
// circle. Each of the records is made by one of them and read by the rest; what it is and who makes it is said at it.
//
// Addresses are 64-bit signed (`Va`), as in analyze: a 16-bit module's address is (selector << 16) + offset
// (decompile::describe lays it out so), and "(seg, off)" is `SegOff`.
//
// Nothing here has code: records, and the abstract `Uc` the emulators, the shadows and the lanes call the Unicorn engine
// through (the host supplies a subclass over the unicorn library; emulate/native_trace has its own, lower-level face of
// the same engine for the 32-bit recorder).
#pragma once

#include <compare>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "core/bytes.hpp"
#include "core/ordered_map.hpp"

namespace cb::translate::ir {
struct Node;   // a node of the typed L2 model (cb/translate/ir.hpp)
}

namespace cb::core16 {

using Va = std::int64_t;

// A real-mode or protected-mode address as its two halves.
struct SegOff {
    std::uint32_t seg = 0;
    std::uint32_t off = 0;
    friend constexpr auto operator<=>(const SegOff&, const SegOff&) = default;
};

// ==== analyze: classes, structures ===================================================================================

// A C++ class found in a module: one row of analyze::classes::hierarchy(image), also what source::classes_writer makes
// for a PowerPC image (where `com`, `message_map` and `size` are not known).
struct ClassRecord {
    Va vtable = 0;                            // address of the table of virtual functions
    std::int64_t slots = 0;                   // how many virtual functions
    std::vector<Va> methods;                  // each slot's function address
    std::string name;                         // "class_<hex vtable>" when the binary does not state it
    std::vector<std::string> bases;
    std::vector<std::string> evidence;        // where each fact came from
    std::optional<Va> size;                   // the object's size
    std::optional<bool> size_exact;           // none: taken as exact
    std::vector<Va> installed_by;             // functions that install the table (constructors, destructors)
    bool com = false;                         // has the shape of a COM interface
    std::optional<Va> message_map;            // MFC message map address
};

// ==== decompile: what the native decompiler says of a function besides its C =========================================

// A structure the decompiler recovered from a variable's field accesses: its name, its bytes, and its fields in offset
// order (`type` is one word, a pointer's ending in `*`, or `^` for a 16-bit far pointer).
struct StructField {
    std::int64_t offset = 0;
    std::int64_t bytes = 0;
    std::string type;
};
struct StructNote {
    std::string name;
    std::int64_t bytes = 0;
    std::vector<StructField> fields;
};

// A global a function touches: its address, its width, and the width of what it points at.
struct GlobalNote {
    Va address = 0;
    std::int64_t bytes = 0;
    std::int64_t elem = 0;
};

// What the decompiler says of a function besides its C, and what the C back end adds when it writes the C from the
// document. The L2 document is a node of the typed model, owned by the arena of whoever decompiled (the notes do not own
// it); a pass that rewrites the tree in place is seen by every holder of the pointer.
struct FunctionNotes {
    std::vector<GlobalNote> globals;
    OrderedMap<std::string, std::int64_t> pointers;   // name -> bytes, in the order the decompiler wrote them (16-bit)
    std::vector<StructNote> structs;
    translate::ir::Node* l2 = nullptr;                // the function's L2 document; none: the decompiler wrote no document
    // The document exactly as the native program wrote it, kept when it holds an integer past 64 bits (a PowerPC constant
    // of 2^63 or more): the typed tree cannot, so everything that hands the document to the C back end uses this text.
    std::optional<std::string> l2_exact_json;
    bool placed = false;                              // the frame, shape and asm-block passes were applied to `l2`
    bool from_l2 = false;                             // the text handed on is the C back end's, written from `l2`
};

// ==== emulate: the Unicorn engine as the emulators, the shadows and the lanes use it ================================

// An engine's register ids are Unicorn's own (UC_X86_REG_*). Every method that fails in Unicorn throws EmulationError.
class Uc {
public:
    virtual ~Uc() = default;
    virtual void* handle() const = 0;                                      // the uc_engine
    virtual void mem_map(std::uint64_t address, std::uint64_t size) = 0;
    virtual void mem_unmap(std::uint64_t address, std::uint64_t size) = 0;
    virtual Bytes mem_read(std::uint64_t address, std::uint64_t size) = 0;
    virtual void mem_write(std::uint64_t address, ByteView data) = 0;
    virtual std::uint64_t reg_read(int reg) = 0;                           // an integer register
    virtual double reg_read_float(int reg) = 0;                            // ST0 (an x87 register)
    virtual void reg_write(int reg, std::uint64_t value) = 0;
    // GDTR / LDTR: written as the (selector, base, limit, flags) Unicorn takes
    struct Descriptor {
        std::uint32_t selector = 0, base = 0, limit = 0, flags = 0;
    };
    virtual void reg_write_descriptor(int reg, const Descriptor& value) = 0;
    // A saved machine state, restored with context_restore.
    struct Context {
        virtual ~Context() = default;
    };
    virtual std::unique_ptr<Context> context_save() = 0;
    virtual void context_restore(const Context& context) = 0;
    virtual void ctl_flush_tb() = 0;
    virtual void emu_start(std::uint64_t begin, std::uint64_t until, std::uint64_t timeout_us = 0, std::uint64_t count = 0) = 0;
    virtual void emu_stop() = 0;
    // A hook on every instruction in [begin, end]: called with the address and size.
    virtual void hook_add_code(std::function<void(Uc&, std::uint64_t address, std::uint32_t size)> callback, std::uint64_t begin,
                               std::uint64_t end) = 0;
    // Hooks on `in` (port, size -> the value read) and `out` (port, size, value).
    virtual void hook_add_insn_in(std::function<std::uint32_t(Uc&, std::uint32_t port, std::uint32_t size)> callback) = 0;
    virtual void hook_add_insn_out(std::function<void(Uc&, std::uint32_t port, std::uint32_t size, std::uint32_t value)> callback) = 0;
    // A hook on `syscall` (x64): called in its place, the run going on after it (a no-op where the binding has no such hook).
    virtual void hook_add_insn_syscall(std::function<void(Uc&)> callback) { (void)callback; }
    // The same reads, for less (a no-op where the binding is not one that can).
    virtual void speed_up() {}
};

}  // namespace cb::core16
