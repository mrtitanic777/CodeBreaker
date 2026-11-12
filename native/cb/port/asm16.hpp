// cb/port/asm16.hpp -- a program's own 16-bit assembly language, made assembly language for a 64-bit machine.
//
// The routines a program wrote in assembly language stay in assembly language: every instruction, label, loop and string
// operation as it was, the arithmetic on values in the 16 and 8 bits it had. What a 64-bit machine changes is changed and
// nothing else:
//
//   pointers     a register that holds a pointer at that place (it was loaded from a pointer of the C program, is an
//                address, or comes from one) is used whole (rsi for si); one that holds a value keeps its 16 or 8 bits;
//                a value used as an index is widened into a scratch register (r11) first
//   structures   [bx+14] is a member of the C program's structure bx points to: its place in the 64-bit layout
//   segments     ds, es and their loads go (one flat memory); les/lds are loads of a pointer; far is near
//   calls        inside the module the old way, on the stack, each slot 8 bytes; a routine the C program calls gets an
//                entry of the C of today (its arguments in rcx, rdx, r8, r9) that pushes them as the old caller did,
//                cdecl or pascal, and keeps the registers the C of today keeps
//   DOS          INT 21h is a call of the platform layer (dos_int21), the registers in a block
//
// The kinds of the registers are followed through each routine (a pass to a fixed point over its jumps and calls); the
// C program's types come from its headers -- the old layout of each structure (16-bit pointers, far ones 4 bytes) from
// the source as it was, the new one as the port lays it out (`CTypes`).
//
// Reading goes `parse` (lines) -> `Port` (one module: its labels, code and data) -> `Port::run` (the kinds) ->
// `Port::emit` (the module for ml64). Anything the 64-bit port cannot carry over is an `Asm16Error` naming the line.
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/error.hpp"
#include "cb/core/ordered_map.hpp"
#include "cb/port/pre_ansi_types.hpp"
#include "cb/translate/backends/asm.hpp"

namespace cb::port::asm16 {

// The assembly cannot be ported (an instruction with no rule, a call outside what is known, a read that is not an
// address), or the source is malformed. The message names the line.
class Asm16Error : public Error {
public:
    explicit Asm16Error(const std::string& what) : Error(what) {}
    CB_ERROR_BOILERPLATE(Asm16Error)
};

// ---- the source -----------------------------------------------------------------------------------------------------

inline constexpr std::array<std::string_view, 8> kReg16 = {"ax", "bx", "cx", "dx", "si", "di", "bp", "sp"};
// The 64-bit register of a 16-bit one ("ax" -> "rax"); empty for any other name.
std::string_view reg64(std::string_view reg16);
// The 16-bit register an 8-bit one is half of ("ah" -> "ax"); empty for any other name.
std::string_view reg8_parent(std::string_view reg8);
bool is_reg16(std::string_view name);
bool is_reg8(std::string_view name);
bool is_segment_register(std::string_view name);   // ds es ss cs
bool is_jump(std::string_view op);                 // every jump and loop
bool is_string_op(std::string_view op);            // lodsb ... scasw
// The calling words an old C compiler had (near far huge pascal cdecl fortran, with and without the underscore).
bool is_calling_word(std::string_view word);

// The text of a line without its `;` comment (a `;` inside quotes is not one).
std::string strip_comment(std::string_view line);
// The operands of an instruction, split at the commas that are not inside brackets, parentheses or quotes, each trimmed.
std::vector<std::string> split_ops(std::string_view text);
// A number as the assembler wrote it: `0ffh`, `12`, `0x1F`; nullopt for anything else (and for a decimal too big for 64 bits).
std::optional<std::int64_t> number(std::string_view text);

struct Directive {
    std::string kind;                    // upper case: SEGMENT ENDS PROC EQU DB PUBLIC ...
    std::optional<std::string> name;     // the name before it (`foo EQU 5`), when it has one
    std::string rest;                    // what follows the keyword
};

struct Line {
    int no = 0;                          // 1-based
    std::string text;
    std::optional<std::string> label;
    std::optional<std::string> op;       // the mnemonic, lower case
    std::optional<std::string> prefix;   // rep repe repz repne repnz
    std::vector<std::string> args;
    std::optional<Directive> directive;
    std::optional<std::string> seg;      // the segment it is in (set on the lines of the code segments)
};

// A source text as lines: label, prefix (rep...), mnemonic and operands -- or a directive. Throws Asm16Error for a prefix
// with nothing after it.
std::vector<Line> parse(std::string_view text);

// The module's code segments: of class CODE (Lattice's PROG), named ..TEXT, or what ASSUME puts in CS -- itself or
// through its group.
std::set<std::string> code_segments(std::span<const Line> lines);

// (length, mnemonic, operands) of the 8086's moves to and from a direct address (8B/89/8A/88 with mod 00 r/m 110,
// A0-A3) written as bytes; nullopt for anything else.
struct DirectMove {
    std::size_t length = 0;
    std::string op;
    std::vector<std::string> args;
};
std::optional<DirectMove> decode_direct(cb::ByteView bytes);

// The module's equates (`name EQU value`, `name = value`) and the sizes of its variables (a bare name of one is its memory).
struct Equates {
    std::map<std::string, std::string> values;
    std::map<std::string, int> sizes;
};

// A memory operand: [bx+si+14], word ptr es:foo[bx], ...
struct Mem {
    std::optional<int> size;             // bytes, when the text gives `byte ptr` ...
    std::optional<std::string> seg;      // es ds ss cs
    std::optional<std::string> sym;
    std::vector<std::string> regs;       // the 16-bit registers added
    std::int64_t disp = 0;
    std::string text;                    // as written
};

struct Operand {
    enum class Kind { Reg, Imm, Off, Mem, Sym };
    Kind kind = Kind::Sym;
    std::string text;                    // Reg: the register (lower case); Imm / Sym: the text; Off: the symbol
    std::int64_t disp = 0;               // Off: `offset sym+4`
    asm16::Mem mem;                      // Mem
    bool is_reg(std::string_view name) const { return kind == Kind::Reg && text == name; }
    bool same_register(const Operand& o) const { return kind == Kind::Reg && o.kind == Kind::Reg && text == o.text; }
};
// An operand as the instruction wrote it. Throws Asm16Error for a memory operand with two symbols.
Operand operand(std::string_view text, const Equates* equ = nullptr);

// ---- the old assembler's listing ------------------------------------------------------------------------------------

// What the program's own assembler (MASM 5 under CodeBreaker's emulator) says of where everything went.
struct ListedLine {
    std::string seg;
    std::uint32_t offset = 0;
    std::uint32_t length = 0;            // bytes the line placed
    std::string text;                    // the source line's start (24 characters, spaces collapsed)
    cb::Bytes bytes;
};
struct Place {
    std::string seg;
    std::uint32_t offset = 0;
};
struct Listing {
    OrderedMap<std::string, Place> labels;   // in the order the listing names them: of two at one place, the first
    std::vector<ListedLine> lines;
};
// The listing's text (a .LST) as places.
Listing parse_listing(std::string_view lst);
// Runs the assembler on `source` and reads its listing: `run_assembler(folder, args)` is the caller's (the DOS emulator's)
// business; it is handed a folder that holds ASM.EXE (the assembler) and T.ASM (the source, CRLF) and leaves T.LST there.
using AssemblerRunner = std::function<void(const std::filesystem::path& folder, std::string_view args)>;
Listing listing_offsets(const std::filesystem::path& source, const std::filesystem::path& assembler,
                        const AssemblerRunner& run_assembler, std::string_view args = " /ML T.ASM,T.OBJ,T.LST;");

// ---- the C program's types ------------------------------------------------------------------------------------------

struct VarType {
    std::string base;                    // `struct a`, `char` (storage words left out)
    std::string declarator;              // `*p`, `buf[10]`
};

// The member of a structure at an old offset, with its new place.
struct Field {
    std::string name;
    std::int64_t old_offset = 0, old_size = 0;
    std::int64_t new_offset = 0, new_size = 0;
    bool is_pointer = false;
    std::string type;                    // what it points to (or its type)
};

// The old and new sizes of one element.
struct ElementSizes {
    std::int64_t old_size = 1, new_size = 1;
};

// The globals' types and the structures' layouts, old (as the 8086 had them) and new (as the port has them).
class CTypes {
public:
    CTypes() = default;
    // `port_texts`: the port's C (new layouts, the globals); `original_texts`: the sources as they were (old layouts, far
    // pointers).
    CTypes(std::span<const std::string> port_texts, std::span<const std::string> original_texts);

    OrderedMap<std::string, VarType> vars;       // the globals the port's files define, in order
    pre_ansi::StructLayouts old_layouts;         // 16-bit pointers, far ones 4 bytes
    pre_ansi::StructLayouts new_layouts;         // 64-bit pointers
    std::set<std::string> far_ptrs;              // globals whose pointers were far: 4 bytes each

    const VarType* var(std::string_view name) const;
    // The type a pointer variable points to ("struct fcb", "char"); nullopt when it is no pointer.
    std::optional<std::string> pointee(std::string_view name) const;
    bool is_pointer(std::string_view name) const;
    // Old and new bytes of an element of a global array (or of the variable); nullopt when not known.
    std::optional<ElementSizes> elem_sizes(std::string_view name) const;
    // What the address of the member that begins at an old offset points to (a nested structure: its `struct s.inner`).
    std::optional<std::string> member_type(std::string_view structure, std::int64_t old_offset) const;
    // The member at an old offset (one that begins there, else one around it; inside nested structures and unions).
    std::optional<Field> field(std::string_view structure, std::int64_t old_offset) const;

private:
    std::optional<Field> field_in(const std::string& path, const pre_ansi::StructLayout& o, const pre_ansi::StructLayout& n,
                                  std::int64_t old_offset) const;
};

// How a C function takes its arguments and gives its result.
struct CArg {
    enum class Kind { Val, Long, Ptr, Double };
    int size = 2;                        // old bytes
    Kind kind = Kind::Val;
    friend bool operator==(const CArg&, const CArg&) = default;
};
struct CSig {
    enum class Ret { Val, Long, Ptr, Void };
    bool pascal = false;
    std::vector<CArg> args;
    Ret ret = Ret::Val;
    std::optional<std::string> to;       // what the address it gives points to
    friend bool operator==(const CSig&, const CSig&) = default;
};
// {C name: its signature} of the functions a C program defines in the old way (the arguments' types declared after the
// head); the macros made of calling words (`#define NEAR near pascal`) are read as they were.
OrderedMap<std::string, CSig> c_functions(std::span<const std::string> texts);

// The routines of the old C library the assembly calls, as today's C library has them.
struct RuntimeCall {
    std::string c_name;
    CSig sig;
};
const RuntimeCall* runtime_call(std::string_view old_name);

// The name the ported routine itself has (a routine called from C has the C name for its entry): "@" + label when the C
// name would be the label.
std::string inner_name(std::string_view label, const std::set<std::string>& c_names);

// ---- what a register holds ------------------------------------------------------------------------------------------

enum class Kind {
    Val,      // a number, 16 bits
    Ptr,      // an address
    Opq,      // unknown: possibly an address, kept whole
    Frame,    // sp as it was at entry + old and new offsets
    Seg,      // a segment
    Ret,      // a return address (on the stack)
};

// Where a pointer points, besides the type it has: a table of the code segment, or an old address of the data segment.
enum class Region { Any, Code, DataSegment };

// The routine whose stack a frame register points into, and the pushes it had made (old size, new size) when it called.
struct FrameOwner {
    std::optional<std::string> routine;
    std::vector<std::pair<std::int64_t, std::int64_t>> stack;
    friend bool operator==(const FrameOwner&, const FrameOwner&) = default;
};

// Where a value came from, for the arguments a routine reads: its own entry's register, a stack slot, a segment's pointer.
struct Origin {
    enum class Source { In, Param, SegOf };
    Source source = Source::In;
    std::string a;                       // In: the routine; Param: the register; SegOf: the register
    std::string b;                       // In: the register
    std::int64_t n = 0;                  // Param: the old offset
    friend bool operator==(const Origin&, const Origin&) = default;
    friend auto operator<=>(const Origin&, const Origin&) = default;
};

// A set of origins shared by the kinds that hold it (copy on write: the kinds are copied at every step).
class OriginSet {
public:
    OriginSet() = default;
    explicit OriginSet(std::set<Origin> s) : p_(s.empty() ? nullptr : std::make_shared<const std::set<Origin>>(std::move(s))) {}
    const std::set<Origin>& get() const {
        static const std::set<Origin> kNone;
        return p_ ? *p_ : kNone;
    }
    bool empty() const { return !p_; }
    OriginSet with(const Origin& o) const;
    OriginSet united(const OriginSet& other) const;
    bool contains_all(const OriginSet& other) const;   // a superset of `other`
    friend bool operator==(const OriginSet& a, const OriginSet& b) { return a.p_ == b.p_ || a.get() == b.get(); }

private:
    std::shared_ptr<const std::set<Origin>> p_;
};

struct K {
    Kind kind = Kind::Val;
    std::optional<std::string> to;        // Ptr, Opq: the C type pointed to (`char`, `struct s *`); empty counts as none
    Region region = Region::Any;          // Ptr: a table of the code, or an old data segment address (`ds_address`)
    std::uint32_t ds_address = 0;
    std::optional<FrameOwner> owner;      // Frame: whose stack
    std::int64_t old = 0, new_ = 0;       // Frame: sp as it was at entry + these
    OriginSet origin;

    static K val(OriginSet origin = {}) { return K{.kind = Kind::Val, .origin = std::move(origin)}; }
    static K ptr(std::optional<std::string> to = std::nullopt);
    static K code_ptr() { return K{.kind = Kind::Ptr, .region = Region::Code}; }
    static K data_ptr(std::uint32_t address) { return K{.kind = Kind::Ptr, .region = Region::DataSegment, .ds_address = address}; }
    static K opq(OriginSet origin = {}) { return K{.kind = Kind::Opq, .origin = std::move(origin)}; }
    static K seg(OriginSet origin = {}) { return K{.kind = Kind::Seg, .origin = std::move(origin)}; }

    bool wide() const { return kind == Kind::Ptr || kind == Kind::Opq || kind == Kind::Frame; }
    // The kind without its origins (what the fixed point compares).
    bool same_kind(const K& o) const;
    // The type it points to is known (a table of the code, a data segment address, or a C type).
    bool points_somewhere() const { return region != Region::Any || (to && !to->empty()); }
    bool points_to_struct() const {   // `struct s` / `union u` (not a pointer to pointers)
        return region == Region::Any && to && !to->empty() && (to->starts_with("struct") || to->starts_with("union")) && !to->ends_with("*");
    }
    friend bool operator==(const K&, const K&) = default;
};

// {struct: the type of its first member, a pointer}: a structure that begins with a pointer.
using FirstMembers = std::map<std::string, std::string>;
// What two paths leave in a register. `a` absent: `b`.
std::optional<K> join(const std::optional<K>& a, const std::optional<K>& b, const FirstMembers& first_members);

struct StackItem {
    std::int64_t old_size = 0, new_size = 0;
    std::optional<K> k;
    friend bool operator==(const StackItem&, const StackItem&) = default;
};
struct State {
    std::array<std::optional<K>, 8> regs;     // by kReg16
    std::optional<K> es, ds;                  // what the segment registers last held (a far load's other half)
    std::vector<StackItem> stack;             // from the entry down
    std::int64_t sp_old = 0, sp_new = 0;
    std::set<std::string> routines;           // the routines this code runs for: a ret returns theirs

    const K* reg(std::string_view name) const;     // a 16-bit register's kind
    std::optional<K>& slot(std::string_view name);
};
// Whether two states are the same for the fixed point: registers (kind, where it points, origins), the stack's kinds (not
// their origins), the stack pointers and the routines.
bool same_state(const State& a, const State& b);
// The state where two paths meet; `a` absent: a copy of `b`.
State merge(const State* a, const State& b, const FirstMembers& first_members);

// ---- one module ----------------------------------------------------------------------------------------------------

// What the caller knows of the program around the module.
struct PortSettings {
    std::vector<std::string> c_names;            // the names the C program calls (to match the routines' public names)
    std::set<std::string> pascal;                // those it calls the pascal way
    OrderedMap<std::string, CSig> c_funcs;       // the C program's functions (c_functions)
    std::set<std::string> asm_routines;          // the public routines of the program's other assembly modules
    bool underscore = true;                      // a routine without a leading underscore is pascal
};

struct Param {
    std::int64_t old_offset = 0;                 // from the entry's sp
    std::int64_t size = 0;                       // old bytes
};
struct ParamType {
    int size = 2;                                // old bytes of the C argument
    std::optional<std::string> to;               // what it points to, when it is a pointer
};
struct Alias {                                   // a name the old linker gave a place inside another variable
    std::string owner;
    std::int64_t offset = 0;
};
using Rt = std::optional<std::string>;           // a routine: its label (absent: before the first one)

// The port of one assembler module.
class Port {
public:
    // `doc`: the module as an L2 document (`module(name, text)` makes one of a source file); `ctypes` outlives the Port.
    Port(const translate::backends::assembly::Document& doc, const CTypes& ctypes, PortSettings settings = {});

    // ---- what the caller tells it before `run` (all optional)
    std::set<std::string> used_outside;                                    // its routines the other modules call
    std::map<std::string, std::vector<ParamType>> param_types;            // a routine: its C arguments' old sizes and types
    std::map<std::string, Alias> aliases;                                  // C name: where inside another variable it is
    std::vector<std::pair<std::uint32_t, std::string>> data_map;           // (old address in the data segment, name), sorted
    std::set<Rt> far;                                                      // the far routines (a retf is added to them)
    Listing offsets;                                                       // the old assembler's listing
    std::map<std::string, std::string> cname;                              // a public routine: the name the C program calls
    bool all_labels_public = false;                                        // every label PUBLIC in the text (a debugger names them)

    // ---- what it found in the source
    const OrderedMap<std::string, std::size_t>& labels() const { return labels_; }      // label -> index into code()
    const std::vector<Line>& code() const { return code_; }                             // the instructions of the code segments
    const std::vector<std::pair<std::string, Line>>& data() const { return data_; }     // (segment, line)
    const std::set<std::string>& public_names() const { return public_; }
    const std::map<std::string, std::string>& externs() const { return extrn_; }        // name -> NEAR FAR PROC BYTE WORD ...
    const Equates& equates() const { return equ_; }
    const std::set<std::string>& code_segment_names() const { return code_seg_; }
    const std::set<std::string>& pascal_routines() const { return pascal_; }
    const std::vector<std::string>& notes() const { return notes_; }

    // The kinds, to a fixed point; then the arguments each routine reads. Returns this.
    Port& run();
    // The module for ml64: data, an entry of today's C for each routine it calls, the routines. `long_returns`: the routines
    // (labels) that give a long in dx:ax.
    std::string emit(const std::set<std::string>& long_returns = {});

    // ---- what `run` and `emit` found
    const std::set<std::string>& called() const { return called_; }
    const std::vector<std::optional<State>>& before() const { return before_; }   // by line of code(): the state before it
    const std::set<std::pair<std::string, std::int64_t>>& own_pointers() const { return own_ptr_; }
    const std::map<Rt, std::vector<Param>>& params() const { return params_; }
    // The state a routine leaves, all its returns merged; nullopt when it never returns.
    std::optional<State> exit_of(const std::string& label) const;
    // The 16-bit registers a routine gives a value (a push and pop of one around it not counted).
    std::set<std::string> writes(const std::string& label);
    // The public routine (or label called) a line belongs to: the nearest such label above it.
    Rt routine(std::size_t line) const;
    // Lines no path reaches (1-based source lines), said in the report.
    const std::vector<int>& unreached() const { return unreached_; }
    const std::map<std::string, CSig>& c_calls() const { return c_calls_; }
    const std::set<std::string>& asm_calls() const { return asm_calls_; }
    const std::set<std::string>& data_used() const { return data_used_; }
    bool used_dos() const { return used_int21_; }
    bool used_null_segment() const { return used_null_; }

    // ---- the pieces (public: the tests and the caller's reports read them)
    // (label, distance) of a code address in a segment, by the old listing.
    struct CodePlace {
        std::string label;
        std::int64_t distance = 0;
    };
    std::optional<CodePlace> code_label(const std::optional<std::string>& seg, std::int64_t value) const;
    // The C name of an assembler symbol (_x is C's x).
    static std::string c_name(std::string_view sym);
    // The label as the ported text names it (`@label` when the C name is the label).
    std::string local(const std::string& label) const;
    // The name of a variable in the ported text, through its alias.
    std::string sym(const std::string& s) const;
    // (old, new) size of what a pointer's kind points to; (1, 1) when not known.
    ElementSizes element_sizes(const K& k) const;
    // A routine outside the module called: another module's (`Asm`) or the C program's or the C library's (`C`).
    struct Callee {
        enum class Kind { Asm, C } kind = Kind::C;
        std::string name;
        CSig sig;
    };
    std::optional<Callee> callee(const std::string& target) const;
    // The old stack's words of a C function's arguments: (argument, old size, kind, first slot) lowest address first, and the
    // number of slots (pascal pushed the first argument first).
    struct Slot {
        std::size_t arg = 0;
        int size = 2;
        CArg::Kind kind = CArg::Kind::Val;
        std::size_t first = 0;
    };
    static std::pair<std::vector<Slot>, std::size_t> slots(const CSig& sig);
    // The call of a C function from the old code, as lines of ml64 text.
    static std::vector<std::string> c_thunk(const std::string& name, const CSig& sig);

private:
    struct Flow {
        enum class Kind { Next, Ret, Jump, Call } kind = Kind::Next;
        std::string label;
    };

    // reading
    void place_data();
    std::optional<CodePlace> data_address(const std::string& seg, std::int64_t v) const;
    std::set<std::string> table_targets() const;
    bool cs_address(std::size_t i, std::string_view r) const;
    void prepare();
    void rewrite_bytes_as_jumps();
    void rewrite_bytes_as_instructions();
    const std::vector<Operand>& ops(std::size_t i) const;
    // kinds
    State entry_state(const std::string& routine) const;
    void analyse();
    void flow_to(std::size_t j, const State& st, std::deque<std::size_t>& work, std::vector<char>& queued);
    std::pair<K, std::optional<Field>> kind_of_mem(const Mem& m, const State& st);
    Mem absolute(const Mem& m);
    std::optional<std::string> data_var_name(std::uint32_t addr) const;
    std::optional<std::pair<std::string, std::int64_t>> data_var(std::int64_t addr) const;
    std::optional<std::string> data_at(std::optional<std::int64_t> v) const;
    bool ds_base(std::size_t i, std::string_view r) const;
    Mem ds_mem(const Mem& m, const K& k);
    std::map<std::int64_t, std::string> param_place(const Rt& routine) const;
    std::pair<State, std::vector<Flow>> step(std::size_t i, State st);
    std::map<std::string, std::int64_t> own_data() const;
    std::set<std::pair<std::string, std::int64_t>> own_slots();
    bool used_as_address(const std::string& sym, std::int64_t d);
    bool address_use(std::size_t j0, std::string_view r, int depth, std::set<std::size_t>& seen);
    std::int64_t own_place(const std::string& sym, std::int64_t disp) const;
    void find_params();
    std::int64_t retsize(const Rt& r) const;
    std::pair<std::size_t, std::int64_t> slot(const Rt& r, std::int64_t e) const;
    // writing
    std::vector<std::string> code_data(std::size_t index);
    std::vector<std::string> data_text();
    std::vector<std::string> wrapper(const std::string& p, bool long_return);
    std::string reg(std::string_view r, const State& st, std::optional<bool> wide = std::nullopt) const;
    std::string sym_any(const std::string& s) const;
    std::string mem(const Mem& m, const State& st, int size, std::size_t i, std::vector<std::string>& pre);
    void as_address(std::size_t i, const std::optional<K>& k);
    std::int64_t stack_place(const State& st, std::int64_t e) const;
    int access_size(const std::vector<Operand>& a, const State& st, std::size_t which);
    std::vector<std::string> instr(std::size_t i, const State& st);
    std::int64_t sp_new_bytes(const State& st, std::int64_t n, std::string_view op) const;

    const CTypes& ct_;
    PortSettings settings_;
    std::set<std::string> c_names_all_;
    FirstMembers first_members_;
    std::string source_;
    std::set<std::string> public_;
    std::map<std::string, std::string> extrn_;
    Equates equ_;
    std::set<std::string> code_seg_;
    OrderedMap<std::string, std::size_t> labels_;
    std::vector<Line> code_;
    std::vector<std::pair<std::string, Line>> data_;
    std::set<std::string> pascal_;
    std::vector<std::string> notes_;
    std::map<std::size_t, std::pair<std::uint32_t, std::uint32_t>> dline_;     // table line of the code: (offset, bytes)
    std::set<std::string> called_;
    std::vector<std::optional<State>> before_;
    std::map<Rt, std::vector<Param>> params_;
    std::map<Rt, std::set<std::int64_t>> ptr_args_;
    std::map<Rt, std::vector<State>> exits_, exits_new_;
    std::set<std::pair<std::string, std::int64_t>> own_ptr_;
    std::map<std::string, std::set<std::string>> writes_;
    mutable std::vector<std::optional<std::vector<Operand>>> ops_cache_;
    std::set<std::string> asm_calls_;
    std::map<std::string, CSig> c_calls_;
    std::set<std::string> data_used_;
    std::vector<int> unreached_;
    bool used_int21_ = false;
    bool used_null_ = false;
};

// An assembler module's source file as its L2 document: one machine fragment, its text as written.
translate::backends::assembly::Document module(std::string_view name, std::string_view text);

}  // namespace cb::port::asm16
