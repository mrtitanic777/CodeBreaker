// cb/analyze/native.hpp -- a module as the analyses see it: its bytes at their addresses, its functions decoded by
// following their code, the starts it gives away, and what is held at a call.
//
// One address space per module, whatever its format: spans of bytes, function starts and their limits, the
// addresses standing for imports, and where the file says the loader writes an address. A function is decoded with
// Capstone by following its code from the start -- fall through, direct branches, recognised jump tables, never
// past the next function -- so bytes nothing reaches (tables and padding inside the code) are not read as
// instructions and produce no false references. discover() adds the starts the module gives away.
//
// This is the engine that used to be cbanalyze.dll's `image.cc`. The analyses (call_graph, strings, resources, and
// the conventions and globals passes that follow) work on an `Image` directly: there is no handle, no JSON between
// them and no copy of the module. Building an Image from a PE, NE or LE file is the job of analyze/image.hpp (it
// fills an ImageSpec from the format readers); this header knows nothing of file formats.
//
// An Image is not thread-safe: functions are decoded lazily and remembered, so even reading one may change it.
// Give each worker its own Image, or decode everything (`for (Va f : image.functions()) image.function(f)`) first.
//
// Addresses are `Va`, a signed 64-bit integer, on purpose: they are computed from signed displacements and
// compared across the two layouts (a flat 32-bit address, or a 16-bit segment's `selector << 16 | offset`), and the
// analyses' arithmetic must not wrap.
#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::analyze {

using Va = std::int64_t;

enum class ImageKind { Pe, Ne, Le };

// An operand: a register, an immediate (masked to its size) or a memory reference.
struct Operand {
    enum class Kind { Register, Immediate, Memory };
    Kind kind = Kind::Register;
    std::string reg;                      // Register: its name
    std::string base, index, seg;         // Memory: empty means none
    Va value = 0;                         // Immediate: the value; Memory: the displacement (the address when no base)
    int scale = 0;
    int size = 0;                         // bytes
};

enum class Flow { None, Call, Jump, CondJump, Return };

struct Instruction {
    Va addr = 0;
    int size = 0;
    std::string mnemonic;
    std::string text;                     // mnemonic and operands as Capstone prints them
    std::vector<Operand> operands;
    std::set<std::string> reads, writes;  // whole registers (al and ax are eax)
    Flow flow = Flow::None;
    std::optional<Va> target;             // the address a direct call or branch goes to
};

// A `jmp [table + reg*N]` the decoder understood.
struct JumpTable {
    Va site = 0;                          // the jump
    Va table = 0;                         // where the table is
    Va low = 0;                           // what is subtracted from the selector before it indexes the table
    std::vector<Va> targets;
    std::string selector;                 // what the selector is, as a register or an operand text
    std::optional<Va> default_target;     // where a selector past the table goes
    std::optional<std::vector<std::uint8_t>> index_bytes;   // a byte table that maps the selector to a target
    bool bounded = false;                 // is the number of entries known from a bound check?
};

struct Function {
    Va start = 0;
    Va limit = 0;                         // where the next function starts (the bytes it may run to)
    Va end = 0;                           // the end of the last instruction reached
    std::vector<Instruction> insns;       // in address order
    std::vector<JumpTable> tables;
};

struct Span {
    Va va = 0;
    Bytes data;
    bool executable = false;
    bool writable = false;
    Va initialised = 0;                   // how much of `data` the file holds a value for (the rest is reserved memory)
    std::string name;                     // the section's name (".rsrc", ...), as the file gives it
};

// Where the file says the loader writes an address, and what kind of address.
enum class PointerKind { Abs32, Far, Selector, Offset, Byte };
struct Pointer {
    PointerKind kind = PointerKind::Byte;
    std::optional<Va> target;
};

// Everything an Image is made from.
struct ImageSpec {
    ImageKind kind = ImageKind::Pe;
    int bits = 32;                        // 32 or 16
    bool flat = false;                    // one flat address space run in user mode (PE)
    std::vector<Span> spans;
    std::vector<std::pair<Va, Va>> functions;               // (start, limit) of each known function
    std::unordered_map<Va, std::string> function_names;     // by start; may be partial
    std::unordered_map<Va, std::string> imports;            // the address standing for an import -> "MODULE!name"
    bool has_pointers = false;            // does the file say where addresses are? (a 32-bit module with relocations)
    std::vector<std::pair<Va, Pointer>> pointers;           // in the file's order
    Va data_segment = 0;                  // 16-bit: the automatic data segment's address
    std::vector<Va> roots;                // where execution can enter from outside
};

class Image {
public:
    explicit Image(ImageSpec spec);
    Image(Image&&) noexcept;
    Image& operator=(Image&&) noexcept;
    Image(const Image&) = delete;
    Image& operator=(const Image&) = delete;
    ~Image();

    ImageKind kind() const { return kind_; }
    int bits() const { return bits_; }
    bool flat() const { return flat_; }
    Va data_segment() const { return data_segment_; }
    const std::vector<Span>& spans() const { return spans_; }
    const std::vector<Va>& roots() const { return roots_; }
    bool has_pointers() const { return has_pointers_; }
    const std::vector<std::pair<Va, Pointer>>& pointer_list() const { return pointer_list_; }
    const Pointer* pointer_at(Va va) const;

    // ---- the bytes ----
    // The span holding `va`, or nullptr.
    const Span* span(Va va) const;
    // Up to `n` bytes at `va` (fewer at the end of the span, none outside).
    Bytes read(Va va, std::int64_t n) const;
    std::optional<std::uint32_t> u16(Va va) const;
    std::optional<std::uint32_t> u32(Va va) const;
    bool is_code(Va va) const;
    // In the module, and not at an address standing for an import.
    bool is_data(Va va) const;

    // ---- functions and names ----
    const std::vector<Va>& functions() const { return functions_; }     // sorted
    bool is_function(Va va) const { return limits_.contains(va); }
    Va function_limit(Va start) const;
    // The start of the function whose bytes hold `va`.
    std::optional<Va> owner(Va va) const;
    bool is_import(Va va) const { return imports_.contains(va); }
    const std::string& import_name(Va va) const;                        // "" if none
    const std::string& function_name(Va va) const;                      // "" if none
    void set_function_name(Va va, std::string name);
    // A name for an address: the function, the import, or the address in hex.
    std::string label(Va va) const;
    // Function starts found by discover(), in the order they were found.
    const std::vector<Va>& discovered() const { return discovered_order_; }
    bool was_discovered(Va va) const { return discovered_set_.contains(va); }

    // ---- instructions ----
    // The decoded function starting at `start` (decoded once). `start` should be a function start.
    const Function& function(Va start);
    // Up to `n` instructions read straight on from `va` (no branch followed; not a function).
    Function linear(Va va, int n);
    // The decoded instruction whose bytes hold `va`, or nullptr when no function reaches it.
    const Instruction* covering(Va va);
    // Add the function starts the module gives away beyond those it was made with:
    //   * the target of a direct call made by code reached from a known start (a function called from one place
    //     only is not among the scanned ones);
    //   * an address the file marks as an address (a relocation) that points at code no known function reaches: a
    //     virtual function, a callback, a handler handed to the system.
    // Both repeat until nothing new turns up. Every function is decoded on the way. Returns the starts added, in
    // the order found; they have no name until the caller gives one (set_function_name).
    std::vector<Va> discover();

private:
    struct Decoder;
    std::unique_ptr<Function> decode(Va start);
    bool jump_table(const Function& fn, const Instruction& ins, const std::map<Va, Instruction>& seen, JumpTable& out);
    std::set<Va> held_code_addresses();
    bool looks_like_code(Va va, int most = 48);

    ImageKind kind_;
    int bits_;
    bool flat_;
    bool has_pointers_;
    Va data_segment_;
    std::vector<Span> spans_;                                   // sorted by address
    std::vector<Va> span_starts_;
    std::vector<Va> functions_;
    std::unordered_map<Va, Va> limits_;
    std::unordered_map<Va, std::string> function_names_;
    std::unordered_map<Va, std::string> imports_;
    std::vector<std::pair<Va, Pointer>> pointer_list_;
    std::unordered_map<Va, Pointer> pointers_;
    std::vector<Va> roots_;
    std::vector<Va> discovered_order_;
    std::unordered_set<Va> discovered_set_;
    std::map<Va, std::unique_ptr<Function>> decoded_;
    std::unique_ptr<Decoder> decoder_;
};

// ---- what a function holds at a call ---------------------------------------------------------------------------

// The whole register a register name is part of: al, ax -> eax.
std::string_view family(std::string_view reg);

// An argument as the instructions before a call give it.
struct Argument {
    enum class Kind { Register, Immediate, Memory, LoadAddress, CallResult };
    Kind kind = Kind::Immediate;
    std::string reg;                      // Register
    Va value = 0;                         // Immediate: the value; Memory: the displacement
    std::optional<Va> call_target;        // CallResult: what the call went to, or the slot it called through
};

// What `reg` holds just before function.insns[i], looking back through straight-line code (at most `depth`
// instructions); nothing if it cannot tell.
std::optional<Argument> definition(const Function& fn, int i, std::string_view reg, int depth = 14);

// The operands of the `n` pushes before function.insns[i], nearest first (a pushed register is replaced by what it
// holds, when that can be told).
std::vector<Argument> arguments(const Function& fn, int i, std::size_t n);

}  // namespace cb::analyze
