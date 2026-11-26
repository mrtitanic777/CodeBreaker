// translate/backends/cpp03.hpp -- the translator's C++03 back end: method bodies, constructors and initialisers from the IR.
//
// The Java converter (cb/java) writes the declarations: headers, signatures, fields. This back end writes every body from the
// typed tree, in the converter's conventions -- its TypeResolver decides pointer or value, `s_` names statics, arrays are
// JArray<T>, Strings JavaString (a char per byte), concatenation JCat -- so a body fits the declarations it is written into.
// `Bodies` is the converter's `emit::source::BodySource`: the source emitter asks it per member.
//
// It declines rather than guesses: anything it does not handle is a `Decline`, and the converter writes an explicit, ledgered gap
// there (J2X-GAP: the body throws UnsupportedOperationException). Whether what it writes is right is decided by the judge, class by
// class. The C++17 dialect (`Dialect::Cpp17`) is the same writer with the forms C++17 has a word for.
//
//     cpp03::Context ctx(resolver, type_profile, runtime, classes);        // the conversion's facts
//     ctx.ledger = &ledger;  ctx.char_map = project.char_map;
//     cpp03::Bodies bodies(ctx, arena, units, cpp03::Dialect::Cpp03);       // lowers the modern forms, indexes the classes
//     source::Options options;  options.bodies = &bodies;                   // ... and the emitter calls bodies.body(cls, method)
//
// Ownership: the units belong to the caller and so does their arena. `Bodies` lowers the modern forms (translate/desugar) in that
// arena -- the units are rewritten in place, as desugar does -- and never takes a node of anyone else's. Nothing the back end
// writes is kept in the tree.
//
// The modules: cpp03.cpp (the context and the shared tables), cpp03_text.cpp (the C++ text rules: literals, boxing, null),
// cpp03_runtime.cpp (what the runtime's headers declare), cpp03_analysis.cpp (the questions the writers ask of a tree),
// cpp03_stmt.cpp (statements), cpp03_own.cpp (the no-GC ownership rules), cpp03_expr.cpp and cpp03_call.cpp (expressions),
// cpp03_ctor.cpp (constructors and initialisers), cpp03_bodies.cpp (the body source), cpp03_cpp17.cpp (the C++17 dialect).
#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "core/arena.hpp"
#include "core/error.hpp"
#include "core/ordered_map.hpp"
#include "core/regex.hpp"
#include "java/emit/source.hpp"
#include "java/ir.hpp"
#include "java/ledger.hpp"
#include "java/runtime.hpp"
#include "java/transpile.hpp"
#include "java/typemap.hpp"
#include "java/types.hpp"
#include "translate/ir.hpp"

namespace cb::translate::backends::cpp03 {

namespace ir = cb::translate::ir;
namespace jir = cb::java::ir;

// This back end does not write that body; the converter's text path (or a J2X-GAP) does. The reason is the message. It is thrown
// from deep inside a body and caught where one member is isolated from the rest (Bodies), never anywhere else: a Result at every
// expression would say nothing the exception does not.
class Decline : public Error {
public:
    explicit Decline(const std::string& reason) : Error(reason) {}
    CB_ERROR_BOILERPLATE(Decline)
};

enum class Dialect { Cpp03, Cpp17 };

using Lines = std::vector<std::string>;

// An expression written: its C++ text and its C++ type.
struct Code {
    std::string code;
    std::string ctype;
};

// An IR type as the converter's JType (simple names, as its resolver keys on them). Declines a type C++03 has no word for.
jir::JType jtype(const ir::TypeP& type);

// 'T a, const JavaString& b' -> [('T', 'a'), ('const JavaString&', 'b')].
std::vector<std::pair<std::string, std::string>> split_params(std::string_view params);

// Whether a C++ type is a pointer, and what `JArray<Block* >` holds (`Block*`; a Decline for a type that is no array).
bool is_pointer(std::string_view ctype);
std::string array_element(std::string_view ctype);
// The class a pointer type points to (`Block*` -> `Block`), if it is a plain class name.
std::optional<std::string> pointee_class(std::string_view ctype);
// Whether a cast between two of the program's classes crosses the hierarchy: neither is the other or extends or implements it
// (a TileEntity to IInventory, which only its subclass implements). Such a cast is a dynamic_cast; a C-style one would
// reinterpret the pointer. `find` gives the program's parsed class of a name (null: not the program's).
bool is_cross_cast(std::string_view from, std::string_view to, const std::function<const jir::JClass*(std::string_view)>& find);

// ---------------------------------------------------------------------------------------------- what the writers share
// What the bodies of one conversion share: the converter's resolver and parsed classes, the runtime it will write beside them,
// the ledger it keeps, the font of the program. Everything it refers to outlives it. It holds the caches of the runtime-header
// reads, so one Context serves one conversion on one thread.
class Context {
public:
    Context(const cb::java::types::TypeResolver& resolver, const cb::java::typemap::TypeProfile& profile,
            const cb::java::runtime::Runtime& runtime, std::span<const jir::JClass> classes);
    Context(const Context&) = delete;
    Context& operator=(const Context&) = delete;

    const cb::java::types::TypeResolver& resolver;
    const cb::java::typemap::TypeProfile& profile;      // the unsigned byte fields
    const cb::java::runtime::Runtime& runtime;
    cb::java::ledger::Ledger* ledger = nullptr;          // the conversion's ledger, when it keeps one
    std::map<std::uint32_t, std::uint32_t> char_map; // char code -> the 8-bit code the program draws (the project's font)

    // The program's classes by qualified name (set by Bodies from the units): the game's own `Container` is not java.awt.Container.
    // Until it is set a class counts as the program's if its simple name is a parsed class's.
    void set_program_classes(std::set<std::string, std::less<>> qualified_names);
    bool is_game(const std::optional<std::string>& qualified) const;

    // The converter's parsed class of that simple name, its constructor taking these IR parameter types, and its method
    // `owner.name(params)` (IR param types), searching up the parents (null: none).
    const jir::JClass* parsed_class(std::string_view simple) const;
    const jir::Method* parsed_ctor(std::string_view simple, std::span<const ir::TypeP> params) const;
    const jir::Method* parsed_method(std::string_view owner, std::string_view name, std::span<const ir::TypeP> params) const;
    std::optional<std::string> field_ctype(std::string_view owner, std::string_view name) const;

    // What the runtime's own header for `owner` declares: the C++ return type of a method, and the (type, spelling) of a field --
    // `s_name` or `name`. Each header is scanned once per name.
    const std::string& runtime_header(std::string_view owner) const;
    std::optional<std::string> runtime_return(std::string_view owner, std::string_view name) const;
    std::optional<std::pair<std::string, std::string>> runtime_field(std::string_view owner, std::string_view name) const;

private:
    std::map<std::string, const jir::JClass*, std::less<>> classes_;
    std::set<std::string, std::less<>> program_;
    mutable std::map<std::string, std::optional<std::string>, std::less<>> return_memo_;
    mutable std::map<std::string, std::optional<std::pair<std::string, std::string>>, std::less<>> field_memo_;
    mutable std::map<std::string, std::unique_ptr<Regex>, std::less<>> return_patterns_;
};

// ---------------------------------------------------------------------------------------------- the writer of one method
// One method's body. `method` is the IR MethodDecl (null for a field initialiser or an enum constant), `parsed` the converter's
// parsed method (for the parameter types its header declared; null: none).
class Writer {
public:
    Writer(Context& ctx, const ir::Node& cls, const ir::Node* method, const jir::Method* parsed);
    virtual ~Writer() = default;
    Writer(const Writer&) = delete;
    Writer& operator=(const Writer&) = delete;

    // ---- types
    std::string ctype(const ir::TypeP& type, cb::java::types::Use use = cb::java::types::Use::Local) const;

    // ---- statements
    Lines body();
    void stmts(std::span<const ir::Node* const> list, Lines& out, std::string_view ind);
    void block(const ir::Suite& suite, Lines& out, std::string_view ind);
    void block(const ir::Node* statement, Lines& out, std::string_view ind);
    // A loop's body; when the loop is labelled and continued by label, the body sits in its own block with `label_cont:` after it (so
    // the goto crosses no initialisation).
    void loop_body(const ir::Suite& body, Lines& out, std::string_view ind);
    void loop_body(const ir::Node* body, Lines& out, std::string_view ind);
    void stmt(const ir::Suite& suite, Lines& out, std::string_view ind);
    void stmt(const ir::Node* statement, Lines& out, std::string_view ind);
    // A condition, without the parentheses Java writes around it (`if(x)`, as the text path: the console passes match that shape).
    std::string cond(const ir::Node* e);

    // ---- expressions -> (code, C++ type)
    std::string name(std::string_view n) const;
    Code expr(const ir::Node* e);
    Code expr(const ir::Suite& e);
    // An argument. A concatenation goes as a JavaString: as a JCat it converts to JavaString and to JObject, and a library
    // constructor taking either is ambiguous (new File(dir + "/x")).
    std::string arg(const ir::Node* a);
    // The arguments without the decompiler's EmptyN marker ones, which the converter strips from declarations and calls alike.
    std::vector<const ir::Node*> unmarked(std::span<ir::Node* const> args) const;

    // The names the writer has declared so far (name -> C++ type) and the names written as something else (inlined constructors).
    std::unordered_map<std::string, std::string> vars;
    std::unordered_map<std::string, std::string> alias;
    bool two_phase = false;       // writing a j2xInit: `return;` is `return this;`
    std::string ret;              // the C++ return type

protected:
    // The statement written: the dialects differ here.
    virtual void write_statement(const ir::Node* s, Lines& out, std::string_view ind);
    virtual Code write_binary(const ir::Node& e, const ir::Binary& b);
    virtual bool handles(const ir::Node* e) const;
    virtual Code dispatch(const ir::Node* e);

    // ---- statements
    void write_labeled(const ir::Labeled& s, Lines& out, std::string_view ind);
    void write_local(const ir::Local& s, Lines& out, std::string_view ind);
    void write_for(const ir::For& s, Lines& out, std::string_view ind);
    void write_switch(const ir::Switch& s, Lines& out, std::string_view ind);
    void write_try(const ir::Try& s, Lines& out, std::string_view ind);
    // A finally that a return, break or continue leaves through: the jump records what it was (and the value) and goes to the
    // finally; after it, the recorded jump is made.
    void finally_with_jumps(const ir::Try& s, Lines& out, std::string_view ind);
    // A try and its catches (no finally). Where a catch is one a NullPointerException reaches, every dereference in the try is
    // null-checked to throw Java's exception (j2xNN), so C++ catches it where Java would, instead of crashing.
    void try_catch(const ir::Try& s, Lines& out, std::string_view ind);
    void note(cb::java::ledger::Category category, cb::java::ledger::Severity severity, std::string_view rule, std::string text);
    // Two call statements written as the console needs them, or none (see cpp03_stmt.cpp).
    std::optional<std::string> kept_call(const ir::Node* e);

    // ---- no-GC ownership (Java's collector, by hand)
    const std::set<std::string, std::less<>>& owned_scalar();
    void free_owned_reassign(const ir::Node* e, Lines& out, std::string_view ind);
    void free_owned_return(const ir::Local& s, std::string_view ct, std::string_view name, Lines& out, std::string_view ind);
    void hoist_owned_args(const ir::Node* root, Lines& out, std::string_view ind);
    // Reflection has no C++ equivalent -- the runtime's stubs return NULL -- so a statement using it is marked for a hand-written
    // factory (J2X-REVIEW, and a HIGH ledger entry).
    void review(const ir::Node* e, Lines& out, std::string_view ind);

    // ---- expressions
    // Java evaluates operands left to right; C++ leaves the order of a call's arguments, of a binary operator's operands and of an
    // assignment's sides unspecified (g++ takes arguments right to left). When the order can change the result, each operand is
    // evaluated into a temporary first, in Java's order, with the comma operator. Returns that sequence, or "".
    std::string sequenced(const ir::Node* e);
    Code write_lit(const ir::Node& e, const ir::Lit& lit);
    Code write_name(const ir::Name& e);
    Code write_select(const ir::Node& e, const ir::Select& s);
    Code write_length(const ir::Length& e);
    Code write_index(const ir::Node& node, const ir::Index& e);
    Code write_unary(const ir::Node& e, const ir::Unary& u);
    Code write_concat(const ir::Concat& e);
    Code write_assign(const ir::Assign& e);
    Code write_cond(const ir::Node& e, const ir::Cond& c);
    std::string arm(const std::string& code, const std::string& ctype, const std::string& target);
    Code write_cast(const ir::Node& e, const ir::Cast& c);
    Code write_instance_of(const ir::InstanceOf& e);
    Code write_class_lit(const ir::ClassLit& e);
    Code write_new(const ir::Node& e, const ir::New& n);
    Code write_new_array(const ir::Node& e, const ir::NewArray& n);
    // `recv` written as the receiver of a member access: null-checked inside a try that catches a NullPointerException.
    std::string checked(const std::string& recv, const std::string& rtype) const;

    // ---- calls
    Code write_call(const ir::Node& e, const ir::Call& c);
    // java.lang.Object's own methods, as the converter lowers them.
    Code object_method(const ir::Call& c);
    // A varargs call's arguments: the trailing ones packed into the array the declaration takes.
    std::string varargs(const ir::Call& c, std::span<const ir::Node* const> args, std::string_view owner);
    // A library varargs call (String.format, Arrays.asList, Constructor.newInstance): the trailing arguments packed into the array
    // type the Java declaration takes, as the text path writes it.
    std::string library_varargs(const ir::Call& c, std::span<const ir::Node* const> args);
    std::string library_field(std::string_view owner, std::string_view name, const ir::TypeP& type) const;
    // The C++ return type of a runtime (library) method: what the runtime's own header says, then its stub registry, else the
    // resolver's mapping of the Java type.
    std::string library_return(std::string_view owner, std::string_view name, const ir::TypeP& ret_type) const;

    Context& ctx_;
    const cb::java::types::TypeResolver& r_;
    const ir::ClassDecl& cls_;
    const ir::MethodDecl* m_;
    const jir::Method* parsed_;
    std::string cname_;
    std::optional<std::string> parent_;
    std::uint32_t method_line_ = 0;

    int npe_ = 0;                                           // inside a try whose catch a NullPointerException reaches
    std::optional<std::string> cont_next_;                  // the label whose loop body is being written
    int loops_ = 0;                                         // loops and switches around the statement
    std::vector<std::pair<int, int>> fin_;                  // enclosing finally-with-jumps: (n, loops)
    int fin_count_ = 0;
    int leaks_ = 0;                                         // _j2xleakN stack temporaries written
    std::span<const ir::Node* const> after_;                // the statements after the one being written
    std::unordered_map<const ir::Node*, Code> subst_;       // an expression already written into a temporary
    Lines pending_;                                         // declarations the current statement needs first
    bool order_ = false;                                    // in a statement: keep Java's operand order
    int temps_ = 0;
    int arg_temps_ = 0;                                     // _j2xarg_N owned temporaries hoisted
    bool owning_ = false;
    bool lvalue_ = false;
    std::set<std::string> nullable_boxed_;                  // boxed locals (Integer, Byte ...) the method compares with null: kept JObject                                   // the expression being written is assigned to (an Index there stays an lvalue)
    std::optional<std::set<std::string, std::less<>>> owned_scalar_;

};

// The C++17 dialect: the same writer with the forms C++17 has where the language has them -- a for-each over a container as a
// range-for, a switch expression as an immediately invoked lambda, `auto` where the initialiser spells the type, nullptr against
// pointers.
class Writer17 : public Writer {
public:
    using Writer::Writer;

protected:
    void write_statement(const ir::Node* s, Lines& out, std::string_view ind) override;
    // A comparison with null: nullptr against a pointer. Against the runtime's value types (JavaString, JList, JObject: their null
    // is a state, compared with 0) the null stays what C++03 wrote.
    Code write_binary(const ir::Node& e, const ir::Binary& b) override;
    bool handles(const ir::Node* e) const override;
    Code dispatch(const ir::Node* e) override;

private:
    // The initialiser names the local's own type: `new T(..)` of the same T, or a cast to it.
    bool spelled(const ir::Node& node, const ir::Local& s) const;
    // `auto x = new T(...)` / `auto x = (T*)y`: the type is written once, where it is made.
    void auto_local(const ir::Node& node, const ir::Local& s, Lines& out, std::string_view ind);
    // for (T x : xs): a range-for over a JList (a std::vector, iterated in order as Java's Iterator does); over an array, the
    // counted loop (JArray has no iterators).
    void write_foreach(const ir::ForEach& s, Lines& out, std::string_view ind);
    // A switch expression as an immediately invoked lambda: each `yield v` is `return v`.
    Code write_switch_expr(const ir::Node& e, const ir::SwitchExpr& s);
};

std::unique_ptr<Writer> make_writer(Dialect dialect, Context& ctx, const ir::Node& cls, const ir::Node* method,
                                    const jir::Method* parsed);

// ---------------------------------------------------------------------------------------------- what a tree says
// Questions the writers (and the C back end, which orders operands the same way) ask of a tree. Every one is a plain function of
// the nodes below its argument.
const ir::Node* strip_paren(const ir::Node* e);
// Does evaluating `n` change anything (a call into the program, an allocation, an assignment, ++/--)?
bool has_effects(const ir::Node* n);
// Names (locals, fields) that evaluating `n` assigns or increments, and names it reads.
std::set<std::string, std::less<>> names_written(const ir::Node* n);
std::set<std::string, std::less<>> names_read(const ir::Node* n);
// Whether the order of evaluating these operands can change the result.
bool order_matters(std::span<const ir::Node* const> nodes);
bool is_constant(const ir::Node* n);
// The operands of `e` whose evaluation order Java fixes (left to right) and C++ does not.
std::vector<const ir::Node*> operands(const ir::Node* e);
// Whether statement `s` never completes normally (Java's rule, for return and throw).
bool is_abrupt(const ir::Node* s);
// Whether any of these statement trees returns, or breaks or continues out of them.
bool leaves(std::span<const ir::Node* const> stmts);
// Whether the tree holds a construct the front end could not lower.
bool has_unsupported(const ir::Node* root);

// ---------------------------------------------------------------------------------------------- the text rules
// A boxed JObject where a primitive is wanted: its explicit accessor.
std::string unbox(const std::string& code, std::string_view ctype, std::string_view target);
// Java's null as a value of C++ type `ctype`: NULL for a pointer; a value type's own null.
std::string null_of(std::string_view ctype);
// `code` (of C++ type `ctype`) where a `target` is wanted: Java's null becomes the target's null.
std::string coerce(const std::string& code, std::string_view target);
// An initial value for a temporary of C++ type `ctype` (it is assigned before it is read).
std::string zero_of(std::string_view ctype);
// `text` made safe inside a /* */ comment.
std::string comment_safe(std::string_view text);
// A float literal as the C++ source writes it (`0.5F`, `1.0e-05`): the shortest text that reads back, plain decimal for
// 1e-4 <= |x| < 1e16, `d.d e+XX` otherwise. A Decline for a value that is not finite.
std::string float_literal(const ir::LitValue& value, std::string_view suffix);
// A Java string as a C++ string literal over the runtime's 8-bit chars: the program's font applied, controls and 0x7F-0xFF as octal
// escapes, what is outside the 8 bits `?`.
std::string c_string_literal(std::string_view utf8, const std::map<std::uint32_t, std::uint32_t>& char_map);
// A C++17 dialect rule: the NULLs written where the target is one of the runtime's value types (a JavaString, JList or JObject
// compared with or made from null) are spelled __null, what NULL is to GCC and clang, so they resolve as they did under C++03.
std::string keep_null(std::string_view text);

// ---------------------------------------------------------------------------------------------- constructors and initialisers
// A constructor from the IR: the call it began with (`super`, `this` or none), the arguments of it, and the body lines. Ordinary
// classes: super(args) is the base initialiser, so its arguments come back separately; this(args) is inlined (C++03 has no
// delegation). Two-phase classes: super(args) is Parent::j2xInit(args), this(args) is C::j2xInit(args), the instance initialisers
// are the caller's.
struct CtorParts {
    std::optional<std::string> kind;
    std::optional<std::string> args;
    Lines body;
};
CtorParts ctor_parts(Dialect dialect, Context& ctx, const ir::Node& cls, const ir::Node& method, const jir::Method* parsed,
                     bool two_phase);
// A field initialiser's C++ expression, converted to the field's type.
std::string field_init(Dialect dialect, Context& ctx, const ir::Node& cls, const ir::Node& field);
// An initialiser block's statements (static {} or instance {}).
Lines block_lines(Dialect dialect, Context& ctx, const ir::Node& cls, const ir::Node& method);
// The C++ body lines of IR method `m`, or a Decline.
Lines method_body(Dialect dialect, Context& ctx, const ir::Node& cls, const ir::Node& method, const jir::Method* parsed);

// ---------------------------------------------------------------------------------------------- the body source
// The tree's member bodies for one conversion, looked up by the converter as it writes each member (emit/source). Counts what it
// wrote and why it declined the rest. A member the units do not hold, or one this back end declines, answers nullopt.
class Bodies : public cb::java::emit::source::BodySource {
public:
    // `units` are the Unit nodes of the lowered program; the modern forms (switch on a string, instanceof patterns, and for C++03
    // the for-each and the switch expression) are lowered first, in `arena`, which must be the arena `units` were made in.
    Bodies(Context& ctx, Arena& arena, std::span<ir::Node* const> units, Dialect dialect);

    std::optional<std::string> field_init(const jir::JClass& cls, const jir::Field& field) override;
    std::optional<std::vector<std::string>> array_items(const jir::JClass& cls, const jir::Field& field) override;
    std::optional<std::vector<std::string>> instance_inits(const jir::JClass& cls) override;
    std::optional<cb::java::emit::source::CtorBody> ctor(const jir::JClass& cls, const jir::Method* ctor, bool two_phase) override;
    std::optional<std::vector<std::string>> enum_args(const jir::JClass& cls, const jir::EnumConst& constant) override;
    std::optional<std::vector<std::vector<std::string>>> static_blocks(const jir::JClass& cls) override;
    std::optional<std::vector<std::string>> body(const jir::JClass& cls, const jir::Method& method) override;

    std::size_t desugared() const { return desugared_; }                   // how many modern forms were lowered
    std::size_t class_count() const { return decls_.size(); }
    std::vector<std::string> class_names() const;                          // sorted
    std::size_t written() const { return written_; }                       // method bodies written
    std::size_t written_ctors() const { return written_ctors_; }
    // Why members were declined, how many: by the first word of the reason, and by the whole reason (the work list), each in the
    // order first met.
    const OrderedMap<std::string, std::size_t>& declined() const { return declined_; }
    const OrderedMap<std::string, std::size_t>& reasons() const { return reasons_; }

private:
    // (the class, the method) of the IR member matching the converter's parsed one (method null: none)
    const ir::Node* find_class(std::string_view name) const;
    const ir::Node* find_method(const ir::Node& cls, const jir::Method& parsed) const;
    const ir::Node* find_ctor(const ir::Node& cls, const jir::Method* parsed) const;
    void count(OrderedMap<std::string, std::size_t>& into, const std::string& key) { ++into[key]; }

    Context& ctx_;
    Dialect dialect_;
    std::size_t desugared_ = 0;
    std::unordered_map<std::string, const ir::Node*> decls_;   // by simple name: the first class of a name wins
    std::size_t written_ = 0;
    std::size_t written_ctors_ = 0;
    OrderedMap<std::string, std::size_t> declined_;
    OrderedMap<std::string, std::size_t> reasons_;
};

// ---------------------------------------------------------------------------------------------- the converter's translator
// What lowering the Java sources gave: the units (in the arena the lowering was handed) with the project's IR fixes made on them, and
// the reports of those fixes.
struct Lowered {
    std::vector<ir::Node*> units;
    cb::java::projects::Patched patched;
};
// The Java front end and the project's fixes, as a stage of its own: javac's typed trees of `request.java_dir` lowered to the IR, in
// the arena, with the project's `ir_patches` applied. The back end does not read Java; whoever owns the front end hands it in.
using Lowering = std::function<Lowered(Arena& arena, const cb::java::transpile::TranslateRequest& request)>;

// The C++03 and C++17 back ends as the converter's `BodyTranslator`: lowers the program (the Lowering), writes its bodies, answers
// the source emitter per member and tallies what was declined. The C++ standard comes from the request (a year of 2017 or later asks
// for the C++17 forms).
class Translator : public cb::java::transpile::BodyTranslator {
public:
    Translator(const cb::java::runtime::Runtime& runtime, Lowering lowering) : runtime_(runtime), lowering_(std::move(lowering)) {}
    cb::java::transpile::TranslatedBodies translate(const cb::java::transpile::TranslateRequest& request) override;

private:
    const cb::java::runtime::Runtime& runtime_;
    Lowering lowering_;
};

}  // namespace cb::translate::backends::cpp03
