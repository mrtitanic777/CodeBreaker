// cb/translate/backends/java.hpp -- the translator's Java back end: a C or C++ program (L2 from the clang front end) as Java 17.
//
// docs/TRANSLATOR_DESIGN.md, phase 5. The output comes in the design's two tiers, chosen per program by a pointer analysis
// (`Analysis`):
//
//   * idiomatic -- when every pointer is disciplined (it only ever names a whole struct, or an array it indexes; no pointer
//     arithmetic, no casts between pointer types, no unions, no address of a scalar): structs become classes, struct pointers
//     references, arrays Java arrays, malloc `new`, free nothing. C++ classes are this tier's: methods, constructors, single
//     inheritance, virtual calls. This tier is its own module (java_objects); here it is a `TierWriter` the caller supplies.
//   * faithful -- otherwise: C's memory is emulated, as Emscripten and CheerpJ do. Everything whose bytes the program can reach
//     (arrays, structs, any variable whose address is taken, the heap) lives in one linear memory (`crt.Mem`, a little-endian
//     byte array); a pointer is an int address into it; a struct is its address, its fields at offsets named in a small class
//     per struct (`Pair.b`), laid out as the source's compiler lays them out (ClassDecl::layout), so unions, punning, pointer
//     arithmetic and sizeof all mean what they meant. Variables whose address is never taken stay ordinary Java locals and
//     fields. `Faithful` and `FnWriter` below are this tier; they are also the base the idiomatic tier extends, which is why
//     their members are public and their steps virtual.
//
// Both tiers share C's arithmetic on Java's types: a char is a byte, unsigned short a char, unsigned int an int and unsigned
// long long a long, with the unsigned operations (division, comparison, widening, `>>`) written out
// (`Integer.divideUnsigned`, `x & 0xFF`, `>>>`). C's library is `crt.LibC` (printf with C's own formatting, the string and
// memory functions, malloc/free on the emulated heap, MSVC's rand), statically imported, so calls read as they did in C. Which
// Java method a C library call is, and how its arguments travel, is the `LibraryCalls` the caller supplies (java_libc, from the
// API-mapping registry); without one every library call is refused.
//
// What the back end does not write it refuses, with the reason (`Refused`): a goto that does not nest, setjmp, a library
// function the runtime does not have. A function it refuses (but the rest of the program is fine) is written as its signature
// and a body that throws, and listed in the notes -- a gap, never a silent drop. A program whose `main` is refused is refused.
//
//     Arena arena;
//     ir::Node* unit = l2::from_json(arena, doc);                  // the clang front end's Unit
//     ir::Node* units[] = {unit};
//     java::Program prog = java::program(units, "Hello", {.library = &libc});
//     prog.source;  prog.tier;  prog.notes;                        // the Java text, which tier, what was gapped or chosen
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "cb/core/arena.hpp"
#include "cb/core/error.hpp"
#include "cb/core/ordered_map.hpp"
#include "cb/translate/ir.hpp"

namespace cb::translate::backends::java {

namespace ir = cb::translate::ir;

// Where the Java sources of the runtime (`crt/*.java`) are, relative to the checkout.
inline constexpr std::string_view kRuntimeDir = "codebreaker/translate/backends/java_runtime";

// Java operator precedence (higher binds tighter).
inline constexpr int kAtom = 17, kPost = 16, kUnary = 15, kMul = 13, kAdd = 12, kShift = 11, kRel = 10, kEq = 9, kBitAnd = 8, kXor = 7,
                     kBitOr = 6, kAnd = 5, kOr = 4, kCond = 3, kAssign = 2;
// The precedence of a binary operator that has one (`*` ... `||`); none for any other.
std::optional<int> binary_precedence(std::string_view op);

// The back end does not write this program (or this function); the reason says why.
class Refused : public Error {
public:
    explicit Refused(const std::string& why) : Error(why) {}
    CB_ERROR_BOILERPLATE(Refused)
};

// A Java expression: its text, the precedence of its outermost operator, its Java type ("boolean" for a C truth value) and
// whether it is a fresh struct copy (a call's result), which a by-value argument need not copy again.
struct Ex {
    std::string code;
    int prec = kAtom;
    std::string jt;
    bool fresh = false;

    // The text, in parentheses when its operator binds looser than `min_prec`.
    std::string at(int min_prec) const;
};

// ---- C types, as the writer asks about them -----------------------------------------------------------------------------

// A type's kind with plain char resolved by its sign, and what has no Java form (`Other`). An absent type is Void.
enum class CKind : std::uint8_t { Void, Bool, I8, U8, I16, U16, I32, U32, I64, U64, F32, F64, F80, Ptr, FnPtr, Array, Class, Record, Null, Fn, Other };
CKind kind_of(const ir::TypeP& type);
bool is_int(const ir::TypeP& type);        // an integer (a bool is not)
bool is_unsigned(const ir::TypeP& type);
bool is_float(const ir::TypeP& type);
bool is_ptr(const ir::TypeP& type);        // a pointer, a function pointer or null
bool is_record(const ir::TypeP& type);     // a struct or union (by class type or by layout)
// The bits of an integer kind (a bool has 1); 0 for any other.
int bits_of(CKind kind);
// The Java primitive a scalar C kind is ("boolean", "byte", "char", ..., "double"); empty for any other.
std::string_view java_primitive(CKind kind);
// The C type's canonical spelling -- the same the clang front end names records and template instances by
// (ClassDecl::qname): `Box<f64>`, `geo.Vec`, `char*`.
std::string spell(const ir::TypeP& type);
// An expression under any grouping parentheses.
ir::Node* strip(ir::Node* expr);
// C's null pointer constant: 0, NULL ((void *) 0), nullptr.
bool is_null_constant(ir::Node* expr);
// A call of malloc, calloc or realloc.
bool from_alloc(ir::Node* expr);
// What a pointer points to (absent for any other type).
const ir::TypeP& pointee(const ir::TypeP& type);
// A C type as its declaration reads, around a name: `int *p`, `struct Rec r`.
std::string ctype_text(const ir::TypeP& type, const std::string& name);

// ---- text ---------------------------------------------------------------------------------------------------------------

// A C name as a Java identifier: every byte outside [A-Za-z0-9_] becomes `_`; a Java keyword (or a name the runtime owns) or a
// name that starts with a digit gets a trailing `_`. No name: `x`.
std::string jname(std::string_view name);
// A Java string literal for a C string held one character per byte (a character of the text is a byte of the C string).
std::string java_string(std::string_view text);
// A Java string literal for a wide (UTF-16) C string.
std::string java_wstring(std::string_view text);
// How many characters a literal's text holds (one per char of the C string; per UTF-16 unit of a wide one).
std::size_t char_count(std::string_view literal_text);
// A C string literal node (char or wide) as a Java string literal; whether it is wide.
std::string string_lit(const ir::Node& lit);
bool is_wide(const ir::Node& lit);
// An integer constant as a Java literal of Java type `jt` (the value reduced to its bits).
Ex int_lit(std::int64_t value, std::string_view jt);
Ex float_lit(double value, std::string_view jt);
// A character literal as the source spelled it, when Java spells it the same (C's alert, vertical-tab, question-mark and hex
// escapes are not Java escapes).
bool java_char_text(std::string_view text);
// Whether Java text is an integer literal, possibly parenthesised and negative: `5`, `(-3)`.
bool is_int_text(std::string_view code);
// The Java type an operand of that Java type has in arithmetic (byte, short and char widen to int), and of two operands.
std::string promoted(std::string_view jt);
std::string promoted2(std::string_view a, std::string_view b);
// The C type an operand of that type has in arithmetic (the small integers and bool are int).
ir::TypeP promote_type(const ir::TypeP& type);

// ---- analysis -------------------------------------------------------------------------------------------------------------

// The pointer analysis that picks the tier, and the facts both tiers need.
class Analysis {
public:
    explicit Analysis(std::span<ir::Node* const> units) : units(units.begin(), units.end()) {}
    Analysis& run();

    std::vector<ir::Node*> units;
    std::vector<std::string> why;            // what keeps the program from the idiomatic tier
    std::set<std::string> addressed;         // names of the scalars whose address is taken (boxed in the idiomatic tier)
    bool cpp_classes = false;
    std::unordered_set<const ir::Node*> exempt;   // free's arguments and NULL casts: pointers to void that name nothing

private:
    void visit(ir::Node* node);
};

// ---- the program ------------------------------------------------------------------------------------------------------------

enum class Tier : std::uint8_t { Auto, Idiomatic, Faithful };
std::string_view to_text(Tier tier);   // "auto", "idiomatic", "faithful"

class FnWriter;

// How a C library call is written as Java (java_libc): `name` is the called function, `call` the Call node, `args` its
// arguments. The answer is the Java text; a library function the runtime does not have is `Refused`.
class LibraryCalls {
public:
    virtual ~LibraryCalls() = default;
    virtual Ex call(FnWriter& writer, ir::Node& call, std::string_view name, std::span<ir::Node* const> args) const = 0;
};

// The idiomatic tier of a program (java_objects), asked to write it. It may throw `Refused`.
class TierWriter {
public:
    virtual ~TierWriter() = default;
    virtual std::string write() = 0;
    // The functions it wrote as gaps, `name (reason)`, and its notes.
    virtual const std::vector<std::string>& gaps() const = 0;
    virtual const std::vector<std::string>& notes() const = 0;
};
using TierFactory =
    std::function<std::unique_ptr<TierWriter>(std::span<ir::Node* const> units, std::string_view name, const Analysis& analysis)>;

struct Options {
    Tier tier = Tier::Auto;
    // The idiomatic tier; without one a program that needs it is refused, and one that merely prefers it is written faithfully.
    TierFactory idiomatic;
    // The library mapping; without one every library call is refused.
    const LibraryCalls* library = nullptr;
};

struct Program {
    std::string name;
    std::string source;                      // the Java text of class `name`
    Tier tier = Tier::Faithful;              // Idiomatic or Faithful, as written
    std::vector<std::string> notes;
};

// The L2 units of one C/C++ program as the Java source of class `name`. `Refused` for a program the back end does not write.
// Tier::Auto: idiomatic where the analysis allows it, else faithful.
Program program(std::span<ir::Node* const> units, std::string_view name, const Options& options = {});

// ---- the faithful tier --------------------------------------------------------------------------------------------------------

// What a C variable looks like on the Java side: `java` its Java name; `mem` true when it lives in the emulated memory (then
// `java` names an int holding its address).
enum class VarKind : std::uint8_t { Enum, Global, Local, Param };
struct Var {
    std::string java;
    bool mem = false;
    ir::TypeP type;
    VarKind kind = VarKind::Local;
};

// A variable's identity across the symbols that name it: a global by name, a local by its owner and name.
struct VarKey {
    bool global = false;
    std::string owner;
    std::string name;
    friend auto operator<=>(const VarKey&, const VarKey&) = default;
};
VarKey key_of(const ir::Sym& sym);

// (Java parameter types, Java result type) of a function; a struct result is an extra leading `int` parameter.
struct Sig {
    std::vector<std::string> params;
    std::string ret;
    friend bool operator==(const Sig&, const Sig&) = default;
};

// One field of a record's layout, and the layout itself.
const ir::RecordType& layout_of(const ir::TypeP& layout);

// The faithful tier: C memory emulated in crt.Mem. `init` does what a constructor would (it calls steps a subclass overrides),
// so make one, then `init()`, then `write()`.
class Faithful {
public:
    Faithful(std::span<ir::Node* const> units, std::string name, const Analysis& analysis, const LibraryCalls* library = nullptr);
    Faithful(const Faithful&) = delete;
    Faithful& operator=(const Faithful&) = delete;
    virtual ~Faithful();

    // Reads the units' declarations (records, functions, globals), checks the records and names what it will write.
    void init();

    // The whole file.
    virtual std::string write();

    // ---- the program as init found it
    std::vector<ir::Node*> units;
    std::string name;
    const Analysis& analysis;
    const LibraryCalls* library;
    std::vector<std::string> notes;
    std::int64_t ptr_size = 8;                                     // bytes (the front end's target.pointer)
    OrderedMap<std::string, ir::Node*> records;                    // qname -> ClassDecl (with layout), in the units' order
    OrderedMap<std::string, std::string> layout_names;             // qname -> Java class name of its offsets
    OrderedMap<std::string, ir::MethodDecl*> functions;            // name -> definition, in the units' order
    std::unordered_map<std::string, Var> globals;                  // name -> Var (lookup only)
    std::vector<ir::FieldDecl*> global_decls;                      // in order
    std::vector<std::pair<Sig, std::string>> fn_ifaces;            // signature -> interface name, in the order they were asked
    OrderedMap<std::string, std::string> fn_consts;                // function name -> the constant naming its address
    std::unordered_set<std::string> used_names;
    std::vector<std::string> gaps;                                 // functions written as an explicit gap: `name (reason)`
    std::unordered_map<std::string, std::vector<ir::MethodDecl*>> overloads;   // name -> definitions where C++ overloads it
    std::set<VarKey> addr_taken;                                   // variables whose address is taken
    std::vector<std::string> fields_out;
    std::vector<std::string> static_init;

    virtual void check_records();
    virtual void make_names();
    void find_addressed();
    // Every function definition, overloads together.
    std::vector<ir::MethodDecl*> all_functions() const;
    // The definition a call's symbol names (by its parameter types, where C++ overloads the name); null for a library function.
    ir::MethodDecl* function_for(const ir::Sym& sym) const;

    // ---- types
    virtual std::string jt(const ir::TypeP& type);
    // The layout of a record type; Refused for a library type the runtime does not have.
    const ir::RecordType& layout(const ir::TypeP& type);
    std::string rec_name(const ir::TypeP& type) const;
    std::int64_t size(const ir::TypeP& type);
    std::int64_t align(const ir::TypeP& type);
    // The Mem accessor suffix for a scalar type: Int, Byte, Ptr, ...
    std::string access(const ir::TypeP& type);
    std::string size_code(const ir::TypeP& type);
    const ir::RecordField& field_off(const ir::TypeP& record, std::string_view field);

    // ---- the file
    // A function written, or -- where the back end declines it -- the explicit gap: its signature and a body that throws with the
    // reason (the Java lane's J2X-GAP), listed in the notes.
    std::vector<std::string> function_or_gap(const ir::MethodDecl& f, const std::function<std::vector<std::string>()>& write,
                                             const std::string& indent = "    ", const std::optional<std::string>& owner = std::nullopt);
    virtual std::optional<std::string> gap_signature(const ir::MethodDecl& f, const std::optional<std::string>& owner);
    std::vector<std::string> layout_class(ir::ClassDecl& record);
    virtual std::vector<std::string> entry();

    // ---- globals
    virtual void declare_global(const ir::FieldDecl& g);
    // A global's initialiser as Java text where it is a constant the declaration can hold.
    std::optional<std::string> const_value(ir::Node* expr, const ir::TypeP& type);
    virtual void init_global(const ir::FieldDecl& g);

    // ---- functions
    // (Java parameter types, Java result type) of a function symbol.
    virtual Sig fn_sig(const ir::Sym& sym);
    virtual Sig fn_sig_of_type(const ir::TypeP& fn_or_fnptr);
    virtual std::string iface(const Sig& sig);
    std::string fn_const(const std::string& function);

    // The nodes the writer makes (a loop that stands for a goto) live as long as it does.
    Arena& arena() { return arena_; }

private:
    Arena arena_;
};

// A variable or memory the writer can read and write: a Java variable, or memory at an address (`java` the address text).
struct Place {
    std::string java;
    bool mem = false;
    ir::TypeP type;
    int prec = kAtom;
};

// One function's body in the faithful tier (or an initialiser: `fn` null).
class FnWriter {
public:
    FnWriter(Faithful& program, const ir::MethodDecl* fn);
    FnWriter(const FnWriter&) = delete;
    FnWriter& operator=(const FnWriter&) = delete;
    virtual ~FnWriter();

    Faithful& p;
    const ir::MethodDecl* fn;
    std::unordered_map<std::string, Var> vars;     // local name -> Var (lookup only)
    std::int64_t frame = 0;                        // frame bytes so far
    bool has_frame = false;
    int tmp = 0;
    std::unordered_set<std::string> declared;
    std::unordered_map<const ir::Node*, std::string> goto_map;   // a Goto -> `break L` / `continue L`
    std::unordered_set<const ir::Node*> labels_done;             // a Labeled placed by structure()
    bool ret_struct = false;

    // ---- the frame
    // A place for a value of this type in the function's frame: the address text.
    std::string alloc(const ir::TypeP& type, std::optional<std::int64_t> size = std::nullopt, std::optional<std::int64_t> align = std::nullopt);
    std::string temp(const ir::TypeP& type);
    std::string fresh(const std::string& base);

    // ---- the function
    virtual std::vector<std::string> write();

    // ---- statements
    // C's goto, where it nests, as Java's labelled break and continue: the statements from the first goto to a later label become
    // `L: { ... }` (goto L is break L); the statements from a label to the last goto back to it become `L: while (true) { ...;
    // break; }` (goto L is continue L).
    std::vector<ir::Node*> structure(std::vector<ir::Node*> stmts);
    // An unlabelled break or continue in `s` that leaves `s` (not inside a loop or switch of `s`).
    bool loose_jump(ir::Node* s);
    void block_stmts(std::span<ir::Node* const> stmts, std::vector<std::string>& out);
    bool abrupt(ir::Node* s);
    // Java's "can complete normally", for the statements C programs write.
    bool can_complete(ir::Node* s);
    bool can_complete(const ir::Suite& s);
    bool const_true(ir::Node* cond);
    // A break that leaves this loop (not one inside a nested loop or switch).
    bool has_break(ir::Node* body);
    bool has_break(const ir::Suite& body);
    // A statement as the body of if/while/for: its lines, indented one more.
    std::vector<std::string> sub(const ir::Suite& s);
    virtual void stmt(ir::Node* s, std::vector<std::string>& out);
    std::string loop_cond(ir::Node* cond);
    void for_(const ir::For& s, std::vector<std::string>& out);
    void switch_(const ir::Switch& s, std::vector<std::string>& out);
    virtual void local(const ir::Local& s, std::vector<std::string>& out);
    virtual void static_local(const ir::Local& s, const std::string& name);
    std::string zero(std::string_view jt) const;
    // Statements storing initialiser `init` into the memory of type `type` at `addr`.
    std::vector<std::string> init_place(const std::string& addr, const ir::TypeP& type, ir::Node* init, bool fresh = false);
    // An integer literal element of an array initialiser, written as an int for Mem.setBytes and the like.
    std::optional<std::string> small_lit(ir::Node* x, const ir::TypeP& elem);
    // The scalar leaves of a (nested) array initialiser when it covers whole rows, and their scalar type.
    std::optional<std::pair<std::vector<ir::Node*>, ir::TypeP>> flat_leaves(std::span<ir::Node* const> items, const ir::TypeP& elem);
    std::vector<std::string> wrap_call(const std::string& head, const std::vector<std::string>& vals, const std::string& tail, std::size_t width = 100);
    std::string offset(const std::string& addr, std::int64_t off) const;
    std::string field_addr(const std::string& addr, const std::string& layout_name, const ir::RecordField& field) const;
    virtual void ret(const ir::Return& s, std::vector<std::string>& out);
    virtual void expr_stmt(ir::Node* e, std::vector<std::string>& out);
    std::string expr_text(ir::Node* e);

    // ---- places
    virtual Var var(const ir::Sym& sym);
    // An lvalue: a Java variable, or memory at an address.
    virtual Place place(ir::Node* e);
    bool pure(ir::Node* e);
    // The address of an lvalue in memory (an array or struct: the value itself).
    virtual Ex addr(ir::Node* e);
    virtual Ex load(const Place& place);
    // Assign `val` (already of the place's type); the expression of the assignment.
    virtual Ex store(const Place& place, const Ex& val);

    // ---- values
    // A C condition as a Java boolean.
    Ex cond(ir::Node* e);
    virtual Ex truth(const Ex& j, const ir::TypeP& type);
    // `j`, which may be a Java boolean, as the C value of this type (1 or 0).
    Ex as_value(const Ex& j, const ir::TypeP& type);
    // An expression as Java; Refused for one the back end has no form for.
    Ex value(ir::Node* e);
    virtual Ex x_paren(ir::Node& e);
    virtual Ex x_unsupported(ir::Node& e);
    virtual Ex x_lit(ir::Node& e);
    virtual Ex x_name(ir::Node& e);
    virtual Ex x_select(ir::Node& e);
    virtual Ex x_index(ir::Node& e);
    virtual Ex x_type_name(ir::Node& e);
    virtual Ex x_address_of(ir::Node& e);
    virtual Ex x_unary(ir::Node& e);
    // Java widened a byte, short or char operand to int: back to the C type's Java type.
    Ex narrow(const Ex& j, std::string_view jt);
    virtual Ex incdec(ir::Node& e, bool as_statement = false);
    virtual Ex x_binary(ir::Node& e);
    // `left op right` of two values already written (as_value applied), the result of C type `result`: shifts, unsigned division,
    // and the rest. Pointer arithmetic and comparisons are not here.
    Ex arithmetic(std::string_view op, const Ex& left, const Ex& right, const ir::TypeP& result);
    Ex ptr_add(const Ex& base, const Ex& index, const ir::TypeP& index_type, const ir::TypeP& ptr_type, std::string_view op);
    virtual Ex compare(ir::Node& e, std::string_view op);
    virtual Ex x_assign(ir::Node& e);
    Ex compound(const Place& place, ir::Node& e);
    virtual Ex x_cond(ir::Node& e);
    virtual Ex x_cast(ir::Node& e);
    // A Java value of C type `from` as C type `to`.
    virtual Ex conv(const Ex& j, const ir::TypeP& from, const ir::TypeP& to);
    // A value of C kind `from` as a Java int or long (`to`) of the same value.
    Ex widen(const Ex& j, CKind from, std::string_view to);
    Ex retype(const Ex& j, std::string_view to);
    virtual Ex x_call(ir::Node& e);
    std::vector<std::string> args(std::span<ir::Node* const> args, const std::vector<ir::TypeP>& params, const ir::TypeP& ret);
    Ex own_call(const ir::MethodDecl& f, const std::string& java_name, std::span<ir::Node* const> args);
    virtual Ex lib_call(ir::Node& e, std::string_view name, std::span<ir::Node* const> args);
    virtual Ex x_new(ir::Node& e);
    std::string seq_lines(const std::vector<std::string>& lines, const std::string& result) const;
    virtual Ex x_new_array(ir::Node& e);
    // Expressions the faithful tier has no form for (string concatenation, array length, this): the idiomatic tier's.
    virtual Ex x_concat(ir::Node& e);
    virtual Ex x_length(ir::Node& e);
    virtual Ex x_this(ir::Node& e);
};

}  // namespace cb::translate::backends::java
