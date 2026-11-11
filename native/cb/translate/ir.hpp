// cb/translate/ir.hpp -- the translator's intermediate representation: one typed tree for every source language.
//
// Front ends build it, passes rewrite it, back ends read it (docs/TRANSLATOR_DESIGN.md). It keeps the source's intent --
// a lambda stays a lambda, a for-each stays a for-each, string concatenation is a Concat -- and leaves lowering to the
// back end, which knows what its target can say.
//
// Every name is resolved: a Name or a Call carries the Sym it refers to, as the source language's own compiler decided
// (overloads, implicit `this`, static or not). Every expression carries its type. Every node carries the line and column
// it came from, so ledgers and the judge point at the original. A construct a front end cannot lower yet becomes an
// `Unsupported` node (with its kind), never a guess: a back end that meets one declines the method, and the old path
// handles it.
//
//     Arena arena;                                                    // one tree, one arena: nodes live as long as it does
//     Node* x = ir::make(arena, ir::Name{.sym = sym}, ir::i32_type()); // an expression of type i32
//     Node* sum = ir::make(arena, ir::Binary{.op = "+", .left = x, .right = one}, ir::i32_type());
//     if (auto* b = ir::get<ir::Binary>(sum)) ...                     // a node is one of the structs below
//     for (Node* n : ir::walk(root)) ...                              // depth first, in source order
//
// The shape follows docs/CPP_DESIGN.md: the node kinds are a closed set, so a node holds a `std::variant` of one struct
// per kind (visited with std::visit, no class hierarchy), each with typed members; the nodes of a tree are made in an
// arena and point at each other with plain pointers (a pass that rewrites a tree allocates new nodes in the same arena
// and rewires the slots; nothing to free); types are small immutable values shared through `TypeP`; a `Sym` is the one
// thing a declaration and its uses really share, so it is a `shared_ptr`. The kinds and slots are those of L2, the
// document form of this tree (codebreaker/translate/l2.schema.json): reading and writing L2 JSON is the edge of the
// program, not this header's business.
//
// A slot that is absent is a null `Node*`, an empty `TypeP`, an empty vector or a disengaged optional; a list slot has no
// "present but empty" form apart from absent. A statement body slot (the branch of an `If`, the body of a loop or a
// method) holds a statement or a list of statements (`Suite`). The slots a walk follows (a node's children) are those
// each struct's `each_child` names, in source order.
#pragma once

#include <compare>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

#include "cb/core/arena.hpp"

namespace cb::translate::ir {

// ---- types ------------------------------------------------------------------------------------------------------------

// Language-neutral primitive kinds. Java's char is u16, byte i8; C's are mapped by width and sign. `Char` is C's plain
// char: its signedness is stated per target (PrimType::is_signed), never assumed.
enum class Prim : std::uint8_t { Bool, I8, I16, I32, I64, U8, U16, U32, U64, F32, F64, F80, Char };
std::string_view to_text(Prim prim);   // "bool", "i8", ..., "char"

// A character in this encoding rather than a number (C#'s char beside ushort).
enum class CharEncoding : std::uint8_t { Utf16, Utf8, Utf32 };

// docs/L2_BINARY_TYPES.md: a pointer's address space.
enum class AddressSpace : std::uint8_t { Flat, Near, Far, Huge, Real, Io };
std::string_view to_text(AddressSpace space);

// The memory model of an allocation.
enum class MemoryModel : std::uint8_t { Gc, Owned, Borrowed, Stack, Manual, Emulated };
std::string_view to_text(MemoryModel memory);

// An operator's integer rule; absent: the source language's own.
enum class Overflow : std::uint8_t { Wrap, Saturate, Trap };
std::string_view to_text(Overflow overflow);

// Which segment a near pointer is relative to, or what a selector names: a segment register, a segment of the module
// (1-based), or a segment (or the offset) of an imported symbol.
enum class SegmentRegister : std::uint8_t { Ds, Ss, Cs, Es, Unknown };
struct ModuleSegment {
    std::uint32_t index = 1;
    friend bool operator==(const ModuleSegment&, const ModuleSegment&) = default;
};
enum class ImportPart : std::uint8_t { Segment, Offset };
struct ImportedSegment {
    std::string import;
    std::optional<ImportPart> part;
    friend bool operator==(const ImportedSegment&, const ImportedSegment&) = default;
};
using SegmentRef = std::variant<SegmentRegister, ModuleSegment, ImportedSegment>;

// Where an original function keeps a parameter or its result.
struct NoLocation {
    friend bool operator==(const NoLocation&, const NoLocation&) = default;
};
struct StackSlot {
    std::int64_t offset = 0;
    friend bool operator==(const StackSlot&, const StackSlot&) = default;
};
struct Register {
    std::string name;
    friend bool operator==(const Register&, const Register&) = default;
};
struct RegisterPair {   // two registers (dx:ax), high first
    std::vector<std::string> names;
    friend bool operator==(const RegisterPair&, const RegisterPair&) = default;
};
enum class Flag : std::uint8_t { CF, ZF, SF, OF };
struct FlagBit {
    Flag flag = Flag::CF;
    friend bool operator==(const FlagBit&, const FlagBit&) = default;
};
using Location = std::variant<NoLocation, StackSlot, Register, RegisterPair, FlagBit>;

enum class CallingConvention : std::uint8_t { Cdecl, Pascal, Stdcall, Fastcall, Thiscall, Register, Interrupt };
std::string_view to_text(CallingConvention conv);

struct Type;

// A type, or none. Types are immutable once made and shared freely; `==` compares what they say (deeply), not whether
// they are the same object. An absent type is the empty TypeP, and equals another absent one.
class TypeP {
public:
    TypeP() = default;
    explicit TypeP(std::shared_ptr<const Type> p) : p_(std::move(p)) {}
    const Type* get() const { return p_.get(); }
    const Type* operator->() const { return p_.get(); }
    const Type& operator*() const { return *p_; }
    explicit operator bool() const { return p_ != nullptr; }
    friend bool operator==(const TypeP& a, const TypeP& b);

private:
    std::shared_ptr<const Type> p_;
};

struct PrimType {
    Prim prim = Prim::I32;
    std::optional<bool> is_signed;               // for Char: stated, not assumed
    std::optional<CharEncoding> encoding;
    friend bool operator==(const PrimType&, const PrimType&) = default;
};
struct VoidType {
    friend bool operator==(const VoidType&, const VoidType&) = default;
};
struct NullType {   // the type of `null`
    friend bool operator==(const NullType&, const NullType&) = default;
};
struct AnyType {    // not resolved
    friend bool operator==(const AnyType&, const AnyType&) = default;
};
struct DynamicType {   // known only at run time
    friend bool operator==(const DynamicType&, const DynamicType&) = default;
};
// A nominal type (a class, interface, enum, struct by name) with its type arguments.
struct ClassType {
    std::string name;                            // qualified
    std::vector<TypeP> args;
    bool nullable = false;                       // a reference that may be null, as the source declared it (C#'s string?)
    // What the Python front end says of a class: a record it made of a fixed-key dict (a value struct unless `ref`), held
    // by reference (shared) or as a value it copies, an exception class (made and thrown as a value). As given, false too.
    std::optional<bool> record;
    std::optional<bool> ref;
    std::optional<bool> exception;
    friend bool operator==(const ClassType&, const ClassType&) = default;
};
struct TypeVarType {
    std::string name;
    friend bool operator==(const TypeVarType&, const TypeVarType&) = default;
};
enum class BoundMode : std::uint8_t { Extends, Super };
struct WildcardBound {
    BoundMode mode = BoundMode::Extends;
    TypeP type;
    friend bool operator==(const WildcardBound&, const WildcardBound&) = default;
};
struct WildcardType {
    std::optional<WildcardBound> bound;
    friend bool operator==(const WildcardType&, const WildcardType&) = default;
};
struct ArrayType {
    TypeP elem;
    std::optional<std::uint64_t> count;          // a fixed-size array (C)
    std::uint32_t rank = 1;                      // a rectangular array of this many dimensions (C#'s int[,]); jagged arrays nest
    friend bool operator==(const ArrayType&, const ArrayType&) = default;
};
struct PtrType {
    TypeP to;
    AddressSpace space = AddressSpace::Flat;
    std::optional<SegmentRef> seg;               // near pointers: the segment the offset is in
    bool ref = false;                            // a C++ reference (T&, T&&): never null nor reseated; its reads and bindings are explicit
    friend bool operator==(const PtrType&, const PtrType&) = default;
};
struct SelectorType {
    std::optional<SegmentRef> of;
    friend bool operator==(const SelectorType&, const SelectorType&) = default;
};
struct FnParam {
    std::optional<std::string> name;
    TypeP type;
    Location at;
    friend bool operator==(const FnParam&, const FnParam&) = default;
};
struct FnResult {
    TypeP type;                                  // absent: no value
    Location at;
    friend bool operator==(const FnResult&, const FnResult&) = default;
};
struct FnType {
    CallingConvention conv = CallingConvention::Cdecl;
    std::optional<AddressSpace> distance;        // flat, near or far
    std::optional<std::int64_t> pops;            // argument bytes the callee removes; absent: not known
    bool loadds = false;
    std::vector<FnParam> params;
    FnResult result;
    std::vector<std::string> preserves;          // registers the callee keeps
    bool variadic = false;
    friend bool operator==(const FnType&, const FnType&) = default;
};
struct FnPtrType {
    AddressSpace space = AddressSpace::Flat;     // flat, near or far
    TypeP fn;                                    // a FnType
    friend bool operator==(const FnPtrType&, const FnPtrType&) = default;
};
enum class RecordRole : std::uint8_t { Struct, Union, Frame, Segment };
struct RecordField {
    std::string name;
    std::uint64_t offset = 0;
    TypeP type;
    bool pad = false;
    friend bool operator==(const RecordField&, const RecordField&) = default;
};
// An explicit layout: offsets fixed, gaps explicit (structures, frames, data segments).
struct RecordType {
    std::optional<std::string> name;
    std::uint64_t size = 0;
    std::optional<RecordRole> role;
    std::optional<bool> packed;
    std::vector<RecordField> fields;
    friend bool operator==(const RecordType&, const RecordType&) = default;
};

struct Type {
    using Body = std::variant<PrimType, VoidType, NullType, AnyType, DynamicType, ClassType, TypeVarType, WildcardType, ArrayType,
                              PtrType, SelectorType, FnType, FnPtrType, RecordType>;
    Body body;

    friend bool operator==(const Type&, const Type&) = default;

    // Whether it is one of the primitive kinds (void is not).
    bool is_prim() const { return std::holds_alternative<PrimType>(body); }
    // Whether a value of it is a reference that may be null or shared: a class, array, type variable, null or wildcard.
    bool is_ref() const;
    // The kind as L2 spells it: "i32", "void", "class", "ptr", "fnptr" ...
    std::string_view kind() const;
    // The unqualified name of a class, type variable or named record (`java.util.List` -> `List`), else the kind.
    std::string simple() const;
    // A hash of what `==` compares first: the kind, the name, the arguments and the element type.
    std::size_t hash() const;
};

// How many array dimensions a type has (jagged arrays nest) and what the innermost element is.
std::pair<std::size_t, TypeP> dims(const TypeP& type);

// The type as the source would write it: `i32`, `List<String>`, `i32[]`, `? extends T`, `ptr`, `fn` ...
std::string to_text(const Type& type);

TypeP make_type(Type::Body body);
TypeP prim_type(Prim prim, std::optional<bool> is_signed = std::nullopt, std::optional<CharEncoding> encoding = std::nullopt);
TypeP class_type(std::string name, std::vector<TypeP> args = {});
TypeP typevar_type(std::string name);
// A pointer to `to`; `seg` only for a near pointer.
TypeP pointer_to(TypeP to, AddressSpace space = AddressSpace::Flat, std::optional<SegmentRef> seg = std::nullopt);
// `dims` nested array types around `elem` (a jagged array: each level one dimension).
TypeP array_of(TypeP elem, std::size_t dims = 1);

const TypeP& void_type();
const TypeP& null_type();
const TypeP& any_type();
const TypeP& bool_type();
const TypeP& i32_type();

// ---- symbols ----------------------------------------------------------------------------------------------------------

enum class SymKind : std::uint8_t {
    Local, Param, Field, Method, Ctor, Class, EnumConst, TypeVar, Package, Global, Function, Label,
    Property,   // C#: a member read and written through accessors
    Event,
};
std::string_view to_text(SymKind kind);

// How a parameter is passed where it is not by value.
enum class PassMode : std::uint8_t { ByValue, Ref, Out, In };
std::string_view to_text(PassMode mode);

// What a name refers to, as the source language resolved it. For a method or constructor: its parameter types, result and
// whether it takes a variable number of arguments, and how each parameter is passed. `owner` is the qualified name of the
// declaring type (or of the method, for a local). `flags` is what else the source says of it: "extension" (a static
// method called as if on its first argument), "indexer", "operator", "readonly", "virtual", "abstract", "override".
//
// A symbol is shared by the declaration and every use of it, and a pass may complete it (give a local its type), so it is
// a shared, mutable object: two nodes that name one `Sym` mean the same thing.
struct Sym {
    SymKind kind = SymKind::Local;
    std::string name;
    std::optional<std::string> owner;
    bool is_static = false;
    TypeP type;
    std::vector<TypeP> params;
    TypeP ret;
    bool varargs = false;
    std::vector<PassMode> refs;
    std::vector<std::string> flags;

    // The unqualified name of the owner.
    std::optional<std::string> owner_simple() const;
};
using SymP = std::shared_ptr<Sym>;
SymP make_sym(SymKind kind, std::string name, std::optional<std::string> owner = std::nullopt, bool is_static = false, TypeP type = {});

// ---- nodes ------------------------------------------------------------------------------------------------------------

struct Node;

// A statement body: one statement (a Block usually) or a list of them (the decompiler's and the Python front end's bodies
// are bare lists). Null: absent.
using Suite = std::variant<Node*, std::vector<Node*>>;

// Where in a binary a node came from (docs/L2_BINARY_TYPES.md section 8); a node of source has none.
struct OriginSegment {
    std::string module;
    std::int64_t segment = 0;
    std::int64_t offset = 0;
    friend bool operator==(const OriginSegment&, const OriginSegment&) = default;
};
struct OriginVa {
    std::string module;
    std::uint64_t va = 0;
    friend bool operator==(const OriginVa&, const OriginVa&) = default;
};
struct OriginAddresses {   // a statement of a decompiled function: the addresses of the instructions it was made from
    std::vector<std::uint64_t> at;
    friend bool operator==(const OriginAddresses&, const OriginAddresses&) = default;
};
using Origin = std::variant<OriginSegment, OriginVa, OriginAddresses>;

// The target a C or C++ unit's compiler was asked for, and the widths it gave the types whose width is the target's.
struct CompilerTarget {
    std::optional<std::string> triple;
    std::optional<std::uint32_t> pointer;        // bits
    std::optional<std::uint32_t> long_bits;
    std::optional<std::uint32_t> wchar;
    std::optional<bool> char_signed;
    std::optional<std::string> language;
    std::optional<std::string> standard;         // the language standard (c++17, c11 ...)
    friend bool operator==(const CompilerTarget&, const CompilerTarget&) = default;
};

// A literal's value: null, a boolean, a number or text. An integer above INT64_MAX is a std::uint64_t. A float that is not
// finite (NaN, infinity) is a double like any other here; L2 keeps it as text.
using LitValue = std::variant<std::monostate, bool, std::int64_t, std::uint64_t, double, std::string>;

// A generic argument of a call is a type -- or, where the Python front end has nothing better to say, a struct format
// string or a count that the call's lowering reads back.
using TypeArg = std::variant<TypeP, std::string, std::int64_t>;

// -- declarations
struct Module {   // a whole program, or one binary's module
    static constexpr std::string_view kName = "Module";
    std::vector<Node*> units;
    std::optional<std::string> header;           // its declarations as text, where they are not yet a tree
    template <class F> void each_child(F&& f);
};
struct Unit {
    static constexpr std::string_view kName = "Unit";
    std::optional<std::string> path;
    std::optional<std::string> package;
    std::vector<std::string> imports;
    std::vector<Node*> types;
    std::optional<std::string> lang;             // the source language ("java", "csharp" ...) whose semantics the unit's library calls,
                                                 // string conversions (Concat) and enum and struct kinds carry; a back end maps them by it
    std::optional<CompilerTarget> target;        // a C/C++ unit's target
    template <class F> void each_child(F&& f);
};
struct ClassDecl {
    static constexpr std::string_view kName = "ClassDecl";
    std::optional<std::string> name;
    std::optional<std::string> qname;
    std::optional<std::string> kind;             // class interface enum record annotation, also struct (a value type, copied on
                                                 // assignment), "record struct", delegate (a named function type whose one member is its Invoke)
    std::vector<std::string> mods;
    std::vector<std::string> typeparams;
    TypeP extends;
    std::vector<TypeP> implements;
    std::vector<Node*> members;
    std::optional<std::string> outer;            // the qualified name of the enclosing type
    std::vector<Node*> tparams;                  // the type parameters with their bounds, as TypeParam
    TypeP layout;                                // a C/C++ struct or union whose bytes the source can see: its RecordType
    template <class F> void each_child(F&& f);
};
struct FieldDecl {
    static constexpr std::string_view kName = "FieldDecl";
    SymP sym;
    TypeP type;
    Node* init = nullptr;
    std::vector<std::string> mods;
    template <class F> void each_child(F&& f);
};
// kind: method ctor static_init init, or function (a free function: C, a decompiled binary), or an accessor (get set init).
// A function whose type carries a calling convention (an FnType) has it on sym->type, where callers read it too. `comments`
// are the text of each comment written above it (a decompiler's warnings); `text` is its source as text where its tree is
// not yet written (the C a function store holds).
struct MethodDecl {
    static constexpr std::string_view kName = "MethodDecl";
    SymP sym;
    std::optional<std::string> name;
    std::optional<std::string> kind;
    std::vector<Node*> params;
    TypeP ret;
    Suite body;
    std::vector<std::string> mods;
    std::vector<TypeP> throws;
    std::vector<std::string> typeparams;
    std::vector<std::string> comments;
    std::optional<std::string> text;
    std::vector<Node*> tparams;
    template <class F> void each_child(F&& f);
};
struct Param {
    static constexpr std::string_view kName = "Param";
    SymP sym;
    TypeP type;
    std::vector<std::string> mods;
    template <class F> void each_child(F&&) {}
};
struct EnumConst {
    static constexpr std::string_view kName = "EnumConst";
    SymP sym;
    std::vector<Node*> args;
    Node* body = nullptr;
    SymP ctor;
    std::optional<std::int64_t> value;           // an enum that is a named integer (C, C#) gives each constant's number; Java's are objects
    template <class F> void each_child(F&& f);
};
// A member read and written through accessors (C# properties and indexers): the accessors are MethodDecls of kind get / set
// / init, each with no body where the compiler supplies it (an auto-property's field).
struct Property {
    static constexpr std::string_view kName = "Property";
    SymP sym;
    TypeP type;
    std::vector<std::string> mods;
    std::vector<Node*> params;
    std::vector<Node*> accessors;
    Node* init = nullptr;
    template <class F> void each_child(F&& f);
};
struct TypeParam {   // bounds: the types it must extend or implement; flags: class, struct, new
    static constexpr std::string_view kName = "TypeParam";
    std::optional<std::string> name;
    std::vector<TypeP> bounds;
    std::vector<std::string> flags;
    std::optional<std::string> variance;
    template <class F> void each_child(F&&) {}
};

// -- statements
struct Block {
    static constexpr std::string_view kName = "Block";
    std::vector<Node*> stmts;
    template <class F> void each_child(F&& f);
};
struct Local {
    static constexpr std::string_view kName = "Local";
    SymP sym;
    TypeP type;
    Node* init = nullptr;
    std::vector<std::string> mods;
    template <class F> void each_child(F&& f);
};
struct ExprStmt {
    static constexpr std::string_view kName = "ExprStmt";
    Node* expr = nullptr;
    template <class F> void each_child(F&& f);
};
struct If {
    static constexpr std::string_view kName = "If";
    Node* cond = nullptr;
    Suite then;
    Suite orelse;
    template <class F> void each_child(F&& f);
};
struct While {
    static constexpr std::string_view kName = "While";
    Node* cond = nullptr;
    Suite body;
    template <class F> void each_child(F&& f);
};
struct DoWhile {
    static constexpr std::string_view kName = "DoWhile";
    Suite body;
    Node* cond = nullptr;
    template <class F> void each_child(F&& f);
};
struct For {
    static constexpr std::string_view kName = "For";
    std::vector<Node*> init;
    Node* cond = nullptr;
    std::vector<Node*> update;
    Suite body;
    template <class F> void each_child(F&& f);
};
struct ForEach {   // var: a Local without init
    static constexpr std::string_view kName = "ForEach";
    Node* var = nullptr;
    Node* iterable = nullptr;
    Node* body = nullptr;
    template <class F> void each_child(F&& f);
};
struct Switch {
    static constexpr std::string_view kName = "Switch";
    Node* expr = nullptr;
    std::vector<Node*> cases;
    template <class F> void each_child(F&& f);
};
struct Case {   // the body falls through as in C
    static constexpr std::string_view kName = "Case";
    std::vector<Node*> labels;
    bool is_default = false;
    std::vector<Node*> body;
    template <class F> void each_child(F&& f);
};
struct Break {
    static constexpr std::string_view kName = "Break";
    std::optional<std::string> label;
    template <class F> void each_child(F&&) {}
};
struct Continue {
    static constexpr std::string_view kName = "Continue";
    std::optional<std::string> label;
    template <class F> void each_child(F&&) {}
};
struct Return {
    static constexpr std::string_view kName = "Return";
    Node* expr = nullptr;
    template <class F> void each_child(F&& f);
};
struct Throw {
    static constexpr std::string_view kName = "Throw";
    Node* expr = nullptr;
    template <class F> void each_child(F&& f);
};
struct Try {
    static constexpr std::string_view kName = "Try";
    std::vector<Node*> resources;
    Node* body = nullptr;
    std::vector<Node*> catches;
    Node* finally_body = nullptr;
    template <class F> void each_child(F&& f);
};
struct Catch {   // several types: a multi-catch
    static constexpr std::string_view kName = "Catch";
    SymP sym;
    std::vector<TypeP> types;
    Node* body = nullptr;
    template <class F> void each_child(F&& f);
};
struct Labeled {
    static constexpr std::string_view kName = "Labeled";
    std::optional<std::string> label;
    Suite body;
    template <class F> void each_child(F&& f);
};
struct Goto {   // C, and whatever structuring leaves in a binary
    static constexpr std::string_view kName = "Goto";
    std::optional<std::string> label;
    template <class F> void each_child(F&&) {}
};
// What stays assembly (docs/L2_BINARY_TYPES.md section 7), as a statement, a member or a whole function: its length in
// bytes, the reason (hand-written, register-arguments, ds-left-changed ...), why, the assembly, the function it stands for,
// the assembler it is written for (wasm, masm, nasm) and, where `asm` is absent, the fragment whose assembly holds it too (a
// run of functions assembled as one file).
struct MachineFragment {
    static constexpr std::string_view kName = "MachineFragment";
    std::optional<std::uint64_t> length;
    std::optional<std::string> reason;
    std::optional<std::string> why;
    std::optional<std::string> assembly;
    std::optional<std::string> name;
    std::optional<std::string> syntax;
    std::optional<std::string> within;
    template <class F> void each_child(F&&) {}
};
struct Sync {
    static constexpr std::string_view kName = "Sync";
    Node* lock = nullptr;
    Node* body = nullptr;
    template <class F> void each_child(F&& f);
};
struct Empty {
    static constexpr std::string_view kName = "Empty";
    template <class F> void each_child(F&&) {}
};
struct Comment {   // a comment at its place among the statements (a warning)
    static constexpr std::string_view kName = "Comment";
    std::optional<std::string> text;
    template <class F> void each_child(F&&) {}
};
struct LocalClass {
    static constexpr std::string_view kName = "LocalClass";
    Node* decl = nullptr;
    template <class F> void each_child(F&& f);
};
struct Assert {
    static constexpr std::string_view kName = "Assert";
    Node* cond = nullptr;
    Node* detail = nullptr;
    template <class F> void each_child(F&& f);
};
struct CtorCall {   // this(...) / super(...)
    static constexpr std::string_view kName = "CtorCall";
    std::optional<std::string> kind;
    SymP sym;
    std::vector<Node*> args;
    Node* target = nullptr;
    template <class F> void each_child(F&& f);
};
struct Yield {   // a switch expression's arm gives its value
    static constexpr std::string_view kName = "Yield";
    Node* expr = nullptr;
    template <class F> void each_child(F&& f);
};

// -- expressions (a Node's `type` is the expression's type)
struct Lit {
    static constexpr std::string_view kName = "Lit";
    LitValue value;
    std::optional<std::string> text;             // as the source spelled it (base, suffix, character escape)
    template <class F> void each_child(F&&) {}
};
struct Name {   // a local or parameter
    static constexpr std::string_view kName = "Name";
    SymP sym;
    template <class F> void each_child(F&&) {}
};
struct Select {   // a field: obj is an expression, This, or TypeName for a static
    static constexpr std::string_view kName = "Select";
    Node* obj = nullptr;
    SymP sym;
    template <class F> void each_child(F&& f);
};
struct TypeName {   // a type used as a qualifier (Block.stone); the node's type says which
    static constexpr std::string_view kName = "TypeName";
    template <class F> void each_child(F&&) {}
};
struct This {
    static constexpr std::string_view kName = "This";
    TypeP qualifier;
    template <class F> void each_child(F&&) {}
};
struct Super {
    static constexpr std::string_view kName = "Super";
    TypeP qualifier;
    template <class F> void each_child(F&&) {}
};
struct Call {   // no target: own method, implicit
    static constexpr std::string_view kName = "Call";
    Node* target = nullptr;
    SymP sym;
    std::vector<Node*> args;
    std::vector<TypeArg> typeargs;
    template <class F> void each_child(F&& f);
};
struct New {   // body: an anonymous ClassDecl; outer: the enclosing instance
    static constexpr std::string_view kName = "New";
    SymP sym;
    std::vector<Node*> args;
    Node* body = nullptr;
    Node* outer = nullptr;
    std::optional<MemoryModel> memory;
    template <class F> void each_child(F&& f);   // outer, args, body
};
struct NewArray {   // elem: the element type
    static constexpr std::string_view kName = "NewArray";
    TypeP elem;
    std::vector<Node*> dims;
    std::vector<Node*> init;
    template <class F> void each_child(F&& f);
};
struct Index {   // sym: an indexer (C#), when not an array; index: one expression, or the list of an index with several arguments
    static constexpr std::string_view kName = "Index";
    Node* arr = nullptr;
    Suite index;
    SymP sym;
    template <class F> void each_child(F&& f);
};
struct Length {   // an array's length
    static constexpr std::string_view kName = "Length";
    Node* arr = nullptr;
    template <class F> void each_child(F&& f);
};
struct Unary {   // op: - + ! ~ ++ -- (and * & delete ...)
    static constexpr std::string_view kName = "Unary";
    std::string op;
    Node* x = nullptr;
    bool postfix = false;
    std::optional<Overflow> overflow;
    template <class F> void each_child(F&& f);
};
// op: + - * / % << >> >>> & | ^ && || == != < <= > >= and ?? (the left operand unless it is null, the right evaluated only
// then). `is_signed` is for a comparison in a binary: from the instruction (jl vs jb), not from the operand types.
struct Binary {
    static constexpr std::string_view kName = "Binary";
    std::string op;
    Node* left = nullptr;
    Node* right = nullptr;
    std::optional<Overflow> overflow;
    std::optional<bool> is_signed;
    template <class F> void each_child(F&& f);
};
struct Concat {   // string concatenation, flattened
    static constexpr std::string_view kName = "Concat";
    std::vector<Node*> parts;
    template <class F> void each_child(F&& f);
};
struct Assign {   // op: absent for a plain assignment, else the compound operator
    static constexpr std::string_view kName = "Assign";
    std::optional<std::string> op;
    Node* target = nullptr;
    Node* value = nullptr;
    std::optional<Overflow> overflow;
    template <class F> void each_child(F&& f);
};
struct Cond {
    static constexpr std::string_view kName = "Cond";
    Node* cond = nullptr;
    Node* then = nullptr;
    Node* orelse = nullptr;
    template <class F> void each_child(F&& f);
};
struct Cast {   // the node's type is the target type; `implicit`: a conversion the source language makes by itself (C's promotions, array decay)
    static constexpr std::string_view kName = "Cast";
    Node* x = nullptr;
    std::optional<Overflow> overflow;
    bool implicit = false;
    template <class F> void each_child(F&& f);
};
struct InstanceOf {   // test: the type tested; binding: a pattern's Local (not a child: the walk does not enter it)
    static constexpr std::string_view kName = "InstanceOf";
    Node* x = nullptr;
    TypeP test;
    Node* binding = nullptr;
    template <class F> void each_child(F&& f);
};
struct ClassLit {   // X.class
    static constexpr std::string_view kName = "ClassLit";
    TypeP of;
    template <class F> void each_child(F&&) {}
};
struct Lambda {   // body: an expression or a Block
    static constexpr std::string_view kName = "Lambda";
    std::vector<Node*> params;
    Node* body = nullptr;
    template <class F> void each_child(F&& f);
};
struct MethodRef {   // mode: invoke / new
    static constexpr std::string_view kName = "MethodRef";
    Node* target = nullptr;
    SymP sym;
    std::optional<std::string> mode;
    template <class F> void each_child(F&& f);
};
struct SwitchExpr {   // cases: each arm ends in a Yield or a Throw
    static constexpr std::string_view kName = "SwitchExpr";
    Node* expr = nullptr;
    std::vector<Node*> cases;
    template <class F> void each_child(F&& f);
};
struct AddressOf {   // a constant code or data address: by symbol, never a number
    static constexpr std::string_view kName = "AddressOf";
    SymP sym;
    template <class F> void each_child(F&&) {}
};
struct FarAddress {   // selector:offset, typed ptr far (MK_FP)
    static constexpr std::string_view kName = "FarAddress";
    Node* selector = nullptr;
    Node* offset = nullptr;
    template <class F> void each_child(F&& f);
};
struct Piece {   // `width` bytes of x from byte `byte`
    static constexpr std::string_view kName = "Piece";
    Node* x = nullptr;
    std::uint32_t byte = 0;
    std::uint32_t width = 0;
    template <class F> void each_child(F&& f);
};
struct Join {   // hi:lo as one wider value
    static constexpr std::string_view kName = "Join";
    Node* hi = nullptr;
    Node* lo = nullptr;
    template <class F> void each_child(F&& f);
};
struct Paren {   // kept: back ends print the grouping the source chose
    static constexpr std::string_view kName = "Paren";
    Node* x = nullptr;
    template <class F> void each_child(F&& f);
};
struct Unsupported {   // a construct the front end cannot lower yet
    static constexpr std::string_view kName = "Unsupported";
    std::optional<std::string> kind;
    std::optional<std::string> raw;
    template <class F> void each_child(F&&) {}
};

using Kind = std::variant<
    // declarations
    Module, Unit, ClassDecl, FieldDecl, MethodDecl, Param, EnumConst, Property, TypeParam,
    // statements
    Block, Local, ExprStmt, If, While, DoWhile, For, ForEach, Switch, Case, Break, Continue, Return, Throw, Try, Catch,
    Labeled, Goto, MachineFragment, Sync, Empty, Comment, LocalClass, Assert, CtorCall, Yield,
    // expressions
    Lit, Name, Select, TypeName, This, Super, Call, New, NewArray, Index, Length, Unary, Binary, Concat, Assign, Cond, Cast,
    InstanceOf, ClassLit, Lambda, MethodRef, SwitchExpr, AddressOf, FarAddress, Piece, Join, Paren, Unsupported>;

// A node: its place in the source, where in a binary it came from, the type of an expression (empty for a node that is not
// one), and what kind of node it is.
struct Node {
    std::uint32_t line = 0;
    std::uint32_t col = 0;
    std::optional<Origin> origin;
    TypeP type;
    Kind kind;
};

enum class Category : std::uint8_t { Declaration, Statement, Expression };

// ---- reading a tree ---------------------------------------------------------------------------------------------------

// The node as a K (a pointer to its struct), or null when it is another kind (or the node is null).
template <class K> K* get(Node* node) { return node == nullptr ? nullptr : std::get_if<K>(&node->kind); }
template <class K> const K* get(const Node* node) { return node == nullptr ? nullptr : std::get_if<K>(&node->kind); }
template <class K> bool is(const Node* node) { return get<K>(node) != nullptr; }

// The kind as L2 spells it: "Block", "Binary" ...
std::string_view kind_name(const Node& node);
Category category(const Node& node);

// The nodes a node holds in the slots a walk follows, in source order.
std::vector<Node*> children(const Node& node);
// Every node under (and including) `root`, depth first, in source order. A null root has none.
std::vector<Node*> walk(Node* root);
// The Unsupported nodes under `root`: what keeps it from going through the new path.
std::vector<Node*> unsupported(Node* root);

// A statement body as a list of statements: the list itself, or the one statement; none for an absent one.
std::vector<Node*> statements(const Suite& suite);
// Whether a body is present: a statement, or a list that is not empty.
bool present(const Suite& suite);

// ---- making a tree ----------------------------------------------------------------------------------------------------

// A new node of kind K in the arena, typed as given (an expression's type; leave it empty for a node that is not one).
template <class K>
Node* make(Arena& arena, K kind, TypeP type = {}, std::uint32_t line = 0, std::uint32_t col = 0) {
    Node* node = arena.make<Node>();
    node->line = line;
    node->col = col;
    node->type = std::move(type);
    node->kind = std::move(kind);
    return node;
}

// A new node of the named kind ("Block", "Binary" ...) with every slot empty; null for a name that is no kind.
Node* make_node(Arena& arena, std::string_view kind_name);

// A deep copy of a tree in `arena` (which may be another than the original's). Types and symbols are shared, not copied.
Node* clone(Arena& arena, const Node* root);

// ---- the operator names of the Java front end -------------------------------------------------------------------------

// The operator a JavaParser binary-operator name stands for: "PLUS" -> "+", "UNSIGNED_RIGHT_SHIFT" -> ">>>".
std::optional<std::string_view> binary_operator(std::string_view java_name);
struct UnaryOperator {
    std::string_view op;
    bool postfix = false;
};
// The same for a unary one: "POSTFIX_INCREMENT" -> {"++", postfix}.
std::optional<UnaryOperator> unary_operator(std::string_view java_name);

// ---- the child slots, in source order --------------------------------------------------------------------------------------

namespace detail {

template <class F> void each_slot(Node*& slot, F& f) {
    if (slot != nullptr) f(slot);
}
template <class F> void each_slot(std::vector<Node*>& slot, F& f) {
    for (Node*& n : slot) {
        if (n != nullptr) f(n);
    }
}
template <class F> void each_slot(Suite& slot, F& f) {
    if (auto* one = std::get_if<Node*>(&slot)) each_slot(*one, f);
    else each_slot(std::get<std::vector<Node*>>(slot), f);
}
template <class F, class... Slots> void each_slots(F& f, Slots&... slots) { (each_slot(slots, f), ...); }

}  // namespace detail

// Each `each_child(f)` calls f(Node*&) for every non-null node slot, in the order a walk follows them, so a pass can rewire
// a child in place.
#define CB_IR_EACH_CHILD(Type, ...) \
    template <class F> void Type::each_child(F&& f) { detail::each_slots(f, __VA_ARGS__); }

CB_IR_EACH_CHILD(Module, units)
CB_IR_EACH_CHILD(Unit, types)
CB_IR_EACH_CHILD(ClassDecl, members)
CB_IR_EACH_CHILD(FieldDecl, init)
CB_IR_EACH_CHILD(MethodDecl, params, body)
CB_IR_EACH_CHILD(EnumConst, args, body)
CB_IR_EACH_CHILD(Property, params, accessors, init)
CB_IR_EACH_CHILD(Block, stmts)
CB_IR_EACH_CHILD(Local, init)
CB_IR_EACH_CHILD(ExprStmt, expr)
CB_IR_EACH_CHILD(If, cond, then, orelse)
CB_IR_EACH_CHILD(While, cond, body)
CB_IR_EACH_CHILD(DoWhile, body, cond)
CB_IR_EACH_CHILD(For, init, cond, update, body)
CB_IR_EACH_CHILD(ForEach, var, iterable, body)
CB_IR_EACH_CHILD(Switch, expr, cases)
CB_IR_EACH_CHILD(Case, labels, body)
CB_IR_EACH_CHILD(Return, expr)
CB_IR_EACH_CHILD(Throw, expr)
CB_IR_EACH_CHILD(Try, resources, body, catches, finally_body)
CB_IR_EACH_CHILD(Catch, body)
CB_IR_EACH_CHILD(Labeled, body)
CB_IR_EACH_CHILD(Sync, lock, body)
CB_IR_EACH_CHILD(LocalClass, decl)
CB_IR_EACH_CHILD(Assert, cond, detail)
CB_IR_EACH_CHILD(CtorCall, args, target)
CB_IR_EACH_CHILD(Yield, expr)
CB_IR_EACH_CHILD(Select, obj)
CB_IR_EACH_CHILD(Call, target, args)
CB_IR_EACH_CHILD(New, outer, args, body)
CB_IR_EACH_CHILD(NewArray, dims, init)
CB_IR_EACH_CHILD(Index, arr, index)
CB_IR_EACH_CHILD(Length, arr)
CB_IR_EACH_CHILD(Unary, x)
CB_IR_EACH_CHILD(Binary, left, right)
CB_IR_EACH_CHILD(Concat, parts)
CB_IR_EACH_CHILD(Assign, target, value)
CB_IR_EACH_CHILD(Cond, cond, then, orelse)
CB_IR_EACH_CHILD(Cast, x)
CB_IR_EACH_CHILD(InstanceOf, x)
CB_IR_EACH_CHILD(Lambda, params, body)
CB_IR_EACH_CHILD(MethodRef, target)
CB_IR_EACH_CHILD(SwitchExpr, expr, cases)
CB_IR_EACH_CHILD(FarAddress, selector, offset)
CB_IR_EACH_CHILD(Piece, x)
CB_IR_EACH_CHILD(Join, hi, lo)
CB_IR_EACH_CHILD(Paren, x)

#undef CB_IR_EACH_CHILD

}  // namespace cb::translate::ir
