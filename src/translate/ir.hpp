// translate/ir.hpp -- the translator's intermediate representation: one typed tree for every source language.
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
//
// What a lane that lowers a bytecode (CIL, the JVM's) or a binary knows and a source front end does not -- the kind of value
// on the evaluation stack, an instruction's volatile/unaligned/tail prefix, the conversion an instruction makes, an entity of
// the lane's own type system -- has a place too (docs: work/ilspy-port/l2/ILAST_TO_L2.md): `Node::stack` and `Node::flags`,
// a few optional slots on the kinds that have them, entities (`Entity`: an opaque handle to the lane's type-system object) on
// `Type` and `Sym`, and `Info`, a keyed bag for the residue that only one lane has. All of it is optional, absent is the plain
// form, and an `Info` key or an `Intrinsic` name must be in the tables of this header (and of the schema): a lane cannot
// invent one silently.
#pragma once

#include <compare>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <type_traits>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

#include "core/arena.hpp"

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

// ---- what a bytecode or a binary states of a value or an operation -----------------------------------------------------------

// The kind of a value on the machine's evaluation stack, or in a register or slot: CIL's int32 / native int / int64 / F /
// object reference / managed reference, the JVM's int / long / float / double / reference. A source language has one `int`
// where the stack has I4 for bool, byte, char and int alike, so for a lowered bytecode this is the fact and a node's type is
// only a hint; `stack_of` below derives it where a lane did not state it. Unknown: not stated.
enum class Stack : std::uint8_t { Unknown, I4, I, I8, F4, F8, O, Ref, Void };
std::string_view to_text(Stack stack);   // "unknown", "i4", "i", "i8", "f4", "f8", "o", "ref", "void"

// A null-propagating ("lifted") operator: C#'s `a + b` over int?, VB, Kotlin-style; three-valued: SQL's and C#'s bool? `&`.
enum class Lift : std::uint8_t { None, CSharp, ThreeValued };
std::string_view to_text(Lift lift);     // "none", "csharp", "three_valued"

// The flavour of a `??` (a Binary of that operator): over references, over a nullable, or a nullable with a value fallback.
enum class Coalesce : std::uint8_t { Ref, Nullable, NullableValue };
std::string_view to_text(Coalesce coalesce);   // "ref", "nullable", "nullable_value"

// What a conversion instruction does (a Cast's `conv`). The first eight are general (a C cast, the JVM's i2l, a binary's
// widening; `invalid`: a conversion the lane could not make sense of); the GC kinds are CIL's; box .. expr_tree are the CLR's
// and the CLR's expression trees'.
enum class ConvKind : std::uint8_t {
    Invalid, Nop, IntToFloat, FloatToInt, FloatPrecision, SignExtend, ZeroExtend, Truncate,
    StopGcTracking, StartGcTracking, ObjectInterior,
    Box, Unbox, UnboxAny, RefCast /*checked*/, TryCast /*`as`: null on failure*/, ExprTree,
};
std::string_view to_text(ConvKind conv);       // "invalid", "nop", "int_to_float", ..., "try_cast", "expr_tree"

// A compound assignment (`x += y`, `x++`): whether it gives the old value (a post-increment) or the new one; and what the
// target is: an address (`*p`, a field's address), an accessor (a property: a getter call), a dynamic member.
enum class Eval : std::uint8_t { Old, New };
std::string_view to_text(Eval eval);         // "old", "new"
enum class Via : std::uint8_t { Address, Accessor, Dynamic };
std::string_view to_text(Via via);           // "address", "accessor", "dynamic"

// How a call picks its callee (a Call's `form`): by the symbol as written (default), virtually, directly (the JVM's
// invokespecial, C++'s `Base::f()`; CIL `call`), or through a function pointer (CIL's calli: the target is the pointer).
enum class CallForm : std::uint8_t { Default, Virtual, Direct, Indirect };
std::string_view to_text(CallForm form);     // "default", "virtual", "direct", "indirect"

// What a Region is: unstructured blocks (normal), or a loop / switch the lane could name but not structure further.
enum class RegionKind : std::uint8_t { Normal, Loop, While, DoWhile, For, Switch };
std::string_view to_text(RegionKind kind);   // "normal", "loop", "while", "do_while", "for", "switch"

// A Nop is plain or a `pop` (a value the code discarded).
enum class EmptyKind : std::uint8_t { Normal, Pop };
std::string_view to_text(EmptyKind kind);    // "normal", "pop"

// What a variable is, in the meaning other languages share (ILSpy's 14 kinds merge into these; Info "cil.kind_exact" keeps
// the exact one where two merge).
enum class VarRole : std::uint8_t {
    Local, Param, This, Temp, CatchVar, LoopVar, Resource, Pinned, PatternBinding, Captured, InitTarget, NamedArg, DeconTemp,
};
std::string_view to_text(VarRole role);      // "local", "param", "this", "temp", "catch_var", ... "decon_temp"

// A node's prefix and property bits (`Node::flags`), written as a list of names in a document.
enum class NodeFlag : std::uint32_t {
    Volatile = 1, Unaligned1 = 2, Unaligned2 = 4, Unaligned4 = 8, ReadOnly = 16, Tail = 32,                   // general
    StackEmpty = 64, StackAdjust = 128, DelayExc = 256, FromEnd = 512, RValueAddr = 1024, IfRef = 2048,        // CIL's
    InlineArray = 4096, PinnableRef = 8192, Utf8Literal = 16384, CompilerGenerated = 32768, HasContext = 65536,
};
constexpr std::uint32_t bit(NodeFlag flag) { return static_cast<std::uint32_t>(flag); }
std::string_view to_text(NodeFlag flag);     // "volatile", "unaligned1", ..., "has_context"
std::span<const NodeFlag> all_node_flags();  // every flag, in bit order

// The enum named by a document's text; nothing for a name it has not. Defined for Stack, Lift, Coalesce, ConvKind, Eval, Via,
// CallForm, RegionKind, EmptyKind, VarRole and NodeFlag.
template <class E> std::optional<E> from_text(std::string_view text);
#define CB_IR_DECLARE_FROM_TEXT(E) template <> std::optional<E> from_text<E>(std::string_view text);
CB_IR_DECLARE_FROM_TEXT(Stack) CB_IR_DECLARE_FROM_TEXT(Lift) CB_IR_DECLARE_FROM_TEXT(Coalesce) CB_IR_DECLARE_FROM_TEXT(ConvKind)
CB_IR_DECLARE_FROM_TEXT(Eval) CB_IR_DECLARE_FROM_TEXT(Via) CB_IR_DECLARE_FROM_TEXT(CallForm) CB_IR_DECLARE_FROM_TEXT(RegionKind)
CB_IR_DECLARE_FROM_TEXT(EmptyKind) CB_IR_DECLARE_FROM_TEXT(VarRole) CB_IR_DECLARE_FROM_TEXT(NodeFlag)
#undef CB_IR_DECLARE_FROM_TEXT

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

// ---- entities ---------------------------------------------------------------------------------------------------------------

// An object of a lane's own type system (a CLR IType, IMethod, IField ...) a type or a symbol was made from: an opaque handle.
// This header knows only the handle; the lane defines the class (`ClrEntity`) and the builders that need the object itself ask the
// entity for it. The name, owner and parameters other lanes read are filled in the Type and Sym as well, so a back end that ignores
// entities still works. `key` is canonical, stable and resolvable by the lane's `EntityScope`: it is the entity's text in a document.
enum class EntityKind : std::uint8_t { Type, Method, Field, Property, Event, Parameter, Module };
std::string_view to_text(EntityKind kind);   // "type", "method", "field", "property", "event", "parameter", "module"
class Entity {
public:
    virtual ~Entity() = default;
    virtual std::string_view family() const = 0;   // the lane: "clr"
    virtual EntityKind kind() const = 0;
    virtual std::string key() const = 0;
};
using EntityP = std::shared_ptr<const Entity>;
// Whether two handles are the same entity: the same family and kind and key (not the same object). Either absent: not the same.
bool same_entity(const EntityP& a, const EntityP& b);

// What turns a document's entity text back into an entity: one family's compilation (the CLR lane's holds the type system the
// entities live in, which keeps them alive for as long as a Module that holds the scope). `resolve` returns the entity whose
// `key()` is `key`, or null for one it cannot make; the document reader names the key in its error.
class EntityScope {
public:
    virtual ~EntityScope() = default;
    virtual std::string_view family() const = 0;
    virtual EntityP resolve(std::string_view key) const = 0;
};

struct Type;
struct Sym;
using SymP = std::shared_ptr<Sym>;

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

// ---- info: the facts only one lane has -------------------------------------------------------------------------------------------

// A list of nodes by their place in a walk of the subtree whose root holds the info (the root is 0).
struct NodeRefs {
    std::vector<std::uint32_t> at;
    friend bool operator==(const NodeRefs&, const NodeRefs&) = default;
};
// (a string is a std::string: a `const char*` would be a bool)
using InfoValue = std::variant<bool, std::int64_t, std::string, TypeP, EntityP, SymP, std::vector<std::int64_t>, std::vector<std::string>, NodeRefs,
                               std::vector<TypeP>>;

// A small list of (key, value), sorted by key. An `Info` of a node, type or symbol is L2 like a slot is: every key is in `info_keys()`
// (and the schema's InfoKeys), with its value kind and what it may sit on; a document with another key is refused.
class Info {
public:
    using Item = std::pair<std::string, InfoValue>;
    const InfoValue* find(std::string_view key) const;
    void set(std::string key, InfoValue value);
    bool erase(std::string_view key);
    bool empty() const { return items_.empty(); }
    const std::vector<Item>& items() const { return items_; }
    friend bool operator==(const Info&, const Info&);

private:
    std::vector<Item> items_;
};

// An Info, or none: one pointer, no allocation until something is set; a copy is a copy of what it holds (as `Boxed`).
class InfoP {
public:
    InfoP() = default;
    InfoP(const InfoP& other) : p_(other.p_ ? std::make_unique<Info>(*other.p_) : nullptr) {}
    InfoP& operator=(const InfoP& other) {
        if (this != &other) p_ = other.p_ ? std::make_unique<Info>(*other.p_) : nullptr;
        return *this;
    }
    InfoP(InfoP&&) noexcept = default;
    InfoP& operator=(InfoP&&) noexcept = default;
    explicit operator bool() const { return p_ != nullptr; }
    const Info* get() const { return p_.get(); }
    const InfoValue* find(std::string_view key) const { return p_ ? p_->find(key) : nullptr; }
    // The value under `key` as a T (bool, std::int64_t, std::string, TypeP, EntityP, SymP, ...), or null if there is none or it is another kind.
    template <class T> const T* get_as(std::string_view key) const {
        const InfoValue* v = find(key);
        return v != nullptr ? std::get_if<T>(v) : nullptr;
    }
    void set(std::string key, InfoValue value) {
        if (!p_) p_ = std::make_unique<Info>();
        p_->set(std::move(key), std::move(value));
    }
    void erase(std::string_view key) {
        if (p_ && p_->erase(key) && p_->empty()) p_.reset();
    }
    friend bool operator==(const InfoP& a, const InfoP& b);

private:
    std::unique_ptr<Info> p_;
};

// What a key's value is, and what it may sit on.
enum class InfoValueKind : std::uint8_t { Bool, Int, String, Type, Entity, Sym, Ints, Strings, NodeRefs, Types };
enum class InfoOn : std::uint8_t { Node, Type, Sym };
struct InfoKey {
    std::string_view key;          // namespaced: "cil.code_size"
    InfoOn on;
    InfoValueKind value;
    std::string_view kinds;        // for a node: the kinds it may sit on, space separated
    bool general;                  // a fact other lanes could state too, kept here until a second lane needs it as a slot
    std::string_view doc;
};
// Every key a document may carry, sorted by key (the schema's `$defs/InfoKeys` is this table).
std::span<const InfoKey> info_keys();
const InfoKey* find_info_key(std::string_view key);

// An `Intrinsic` node's names: an operation with no general node. A name needs a row here, with the operands, the result's stack
// kind, the flags it reads, the reason it is not a general node, the lanes that use it and the test that covers it; a name with no
// prefix (`mem.copy`) is allowed only where two lanes use it. A review at the end of stage 14 lists the `cil.*` names: any that a
// second lane needs becomes a node. Sorted by name (the schema's `$defs/IntrinsicNames` is this table).
struct IntrinsicName {
    std::string_view name;
    std::string_view operands;     // args (then targs, syms, strs, imms), in words
    std::string_view result;       // the result's stack kind: i4 i i8 f4 f8 o ref void, or "arg" (as the first operand)
    std::string_view flags;        // the NodeFlags the node may carry ("" none)
    std::string_view why;          // why it is not a general node
    std::string_view lanes;        // the lanes that use it, space separated
    std::string_view test;         // the test that covers it
};
std::span<const IntrinsicName> intrinsic_names();
const IntrinsicName* find_intrinsic(std::string_view name);

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
    std::optional<std::uint32_t> index;          // its place in the owner's list of type parameters (generics in C#, Java, Rust, C++)
    std::optional<std::string> owner;            // the declaring type or method
    bool of_method = false;                      // a method's type parameter, not a type's
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
    bool managed = false;                        // a reference the runtime tracks (a CLR managed/interior pointer, Rust's, Go's); `ref` is "never null"
    friend bool operator==(const PtrType&, const PtrType&) = default;
};
struct SelectorType {
    std::optional<SegmentRef> of;
    friend bool operator==(const SelectorType&, const SelectorType&) = default;
};
// A tuple (Python, Rust, C#, Swift, Kotlin): the element types and, where the source names them, the names.
struct TupleType {
    std::vector<TypeP> elems;
    std::vector<std::optional<std::string>> names;
    friend bool operator==(const TupleType&, const TupleType&) = default;
};
// A type with a custom modifier (the CLR's modreq / modopt): kept so a lowered signature round-trips.
struct ModType {
    TypeP inner;
    bool required = false;
    TypeP modifier;
    friend bool operator==(const ModType&, const ModType&) = default;
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
    bool has_this = false;                       // a member function's type: the first parameter is the object (C++ member pointers, thiscall)
    bool explicit_this = false;                  // ... which the signature spells out (CIL's calli with `explicit this`)
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
                              PtrType, SelectorType, FnType, FnPtrType, RecordType, TupleType, ModType>;
    Body body;
    EntityP entity;                              // the lane's type-system object this was made from, if any
    InfoP info;                                  // facts only one lane has (Type keys of `info_keys()`)

    // `==` compares what the type says: the body, deeply -- not the entity, not the info. A CIL `int` (an entity of the CLR's
    // System.Int32, say) equals the Java front end's `i32`, and hashes the same; `same_entity` compares entities.
    friend bool operator==(const Type& a, const Type& b) { return a.body == b.body; }

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
// The same, made from an entity of the lane's type system and with the lane's facts.
TypeP make_type(Type::Body body, EntityP entity, InfoP info = {});
// The same entity: the same family and kind and key (not both absent).
bool same_entity(const TypeP& a, const TypeP& b);
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
// .NET's decimal (a 96-bit mantissa, scale 0 to 28): the class type `System.Decimal`. It is not Python's Decimal or SQL's DECIMAL(p,s),
// so there is no primitive for it; each back end maps the class by name (as it maps System.String).
const TypeP& decimal_type();

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
    // What a lane that lowers a bytecode or a binary states of a variable (a local, a parameter): all optional. These small ones sit
    // here, where `kind` leaves seven bytes of padding, so a symbol that states none of it costs nothing (a symbol is made for every use
    // of a variable by the document reader: the bigger members are at the end and kept few).
    std::optional<VarRole> role;                 // what the variable is
    Stack stack = Stack::Unknown;                // the stack kind it stores
    bool generated_name = false;                 // the decompiler invented the name (Fernflower's var3, ILSpy's `num`)
    bool ref_readonly = false;
    bool entry_initialized = false;              // its value on entry is initialised
    bool entry_used = false;                     // ... and used
    std::string name;
    std::optional<std::string> owner;
    bool is_static = false;
    TypeP type;
    std::vector<TypeP> params;
    TypeP ret;
    bool varargs = false;
    std::vector<PassMode> refs;
    std::vector<std::string> flags;

    std::optional<std::int32_t> index;           // a parameter's index (-1 = this), a JVM local slot, a catch's stack index, a frame slot
    EntityP entity;                              // the lane's type-system object (a method, a field), if any
    InfoP info;                                  // Sym keys of `info_keys()` (the region a closure variable is captured in is "cil.capture_scope")

    // The unqualified name of the owner.
    std::optional<std::string> owner_simple() const;
};
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
struct OriginSpan {   // [start, end) bytes of the bytecode of the enclosing method a node was made from (CIL, JVM)
    std::uint32_t start = 0;
    std::uint32_t end = 0;
    friend bool operator==(const OriginSpan&, const OriginSpan&) = default;
};
using Origin = std::variant<OriginSegment, OriginVa, OriginAddresses, OriginSpan>;

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
    std::shared_ptr<const EntityScope> scope;    // what the entities of its types and symbols live in (kept alive with the module); not in a document
    std::vector<std::string> scope_refs;         // what the scope was made from (the assemblies), in a document, so a reader can make the scope
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
    // What a lowered body keeps of its frame (a lane that has one): every variable the body names, in the lane's order; those it
    // captures from the enclosing function; the local functions it declares (MethodDecls); the region (a label) it is declared in.
    std::vector<SymP> locals;
    std::vector<SymP> captures;
    std::vector<Node*> nested;
    std::optional<std::string> scope;
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
struct Switch {   // `lifted`: over a nullable (C#, VB); `of`: the type of the switched value, where the lane states it
    static constexpr std::string_view kName = "Switch";
    Node* expr = nullptr;
    std::vector<Node*> cases;
    bool lifted = false;
    TypeP of;
    template <class F> void each_child(F&& f);
};
// `labels`: the case's constants. `ranges`: the values it takes as closed intervals [low, high] (C's `case a ... b`, Rust's `1..=5`, a
// jump table of a binary), written as the lane has them; `null_label`: also for null (Java 21's `case null`).
struct Case {   // the body falls through as in C
    static constexpr std::string_view kName = "Case";
    std::vector<Node*> labels;
    bool is_default = false;
    std::vector<Node*> body;
    std::vector<std::pair<std::int64_t, std::int64_t>> ranges;
    bool null_label = false;
    template <class F> void each_child(F&& f);
};
struct Break {   // `value`: the value of the construct it leaves (Rust's `break 'a v`, Kotlin, a labelled block of an expression)
    static constexpr std::string_view kName = "Break";
    std::optional<std::string> label;
    Node* value = nullptr;
    template <class F> void each_child(F&& f);
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
    Node* fault_body = nullptr;                  // runs only when the body throws, then the exception goes on (CIL's fault; Java compilers write a catch-all that rethrows)
    template <class F> void each_child(F&& f);
};
struct Catch {   // several types: a multi-catch; `filter`: a condition that must hold too (C# `when`, VB `When`), read before the body
    static constexpr std::string_view kName = "Catch";
    SymP sym;
    std::vector<TypeP> types;
    Node* body = nullptr;
    Node* filter = nullptr;
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
struct Empty {   // `note`: a comment it carries; `kind`: a plain Nop, or a `pop` (a discarded value)
    static constexpr std::string_view kName = "Empty";
    std::optional<std::string> note;
    EmptyKind kind = EmptyKind::Normal;
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
// An iterator method (its MethodDecl has the mod "iterator") hands out its sequence with these: `yield return expr` gives the next
// element and stops where it is until the consumer asks for another; `yield break` ends the sequence. Python's `yield` is the same.
struct YieldReturn {
    static constexpr std::string_view kName = "YieldReturn";
    Node* expr = nullptr;
    template <class F> void each_child(F&& f);
};
struct YieldBreak {
    static constexpr std::string_view kName = "YieldBreak";
    template <class F> void each_child(F&&) {}
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
struct Call {   // no target: own method, implicit; `form`: how the callee is picked (Indirect: `target` is the function pointer, `sym` is absent)
    static constexpr std::string_view kName = "Call";
    Node* target = nullptr;
    SymP sym;
    std::vector<Node*> args;
    std::vector<TypeArg> typeargs;
    CallForm form = CallForm::Default;
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
struct Index {   // sym: an indexer (C#), when not an array; index: one expression, or the list of an index with several arguments; elem: the element type the lane states
    static constexpr std::string_view kName = "Index";
    Node* arr = nullptr;
    Suite index;
    SymP sym;
    TypeP elem;
    template <class F> void each_child(F&& f);
};
struct Length {   // an array's length
    static constexpr std::string_view kName = "Length";
    Node* arr = nullptr;
    template <class F> void each_child(F&& f);
};
struct Unary {   // op: - + ! ~ ++ -- (and * & delete ...); left_stk: the operand's stack kind, where it is not the node's; lift: null-propagating
    static constexpr std::string_view kName = "Unary";
    std::string op;
    Node* x = nullptr;
    bool postfix = false;
    std::optional<Overflow> overflow;
    Stack left_stk = Stack::Unknown;
    Lift lift = Lift::None;
    template <class F> void each_child(F&& f);
};
// op: + - * / % << >> >>> & | ^ && || == != < <= > >= and ?? (the left operand unless it is null, the right evaluated only
// then). `is_signed` is the sign the operator uses (a comparison's jl vs jb; a division's, a right shift's, a CIL `add.ovf` vs
// `add.ovf.un`): from the instruction, not from the operand types; absent: the operator's default (what a producer that states
// it only on comparisons leaves). `left_stk`, `right_stk`: the operands' stack kinds (a JVM's int vs long operand, a CIL's I4 vs I);
// `lift`: null-propagating; `coalesce`: the flavour of a `??`; `sym`: the operator's method where the operator is user-defined.
struct Binary {
    static constexpr std::string_view kName = "Binary";
    std::string op;
    Node* left = nullptr;
    Node* right = nullptr;
    std::optional<Overflow> overflow;
    std::optional<bool> is_signed;
    Stack left_stk = Stack::Unknown;
    Stack right_stk = Stack::Unknown;
    Lift lift = Lift::None;
    std::optional<Coalesce> coalesce;
    SymP sym;
    template <class F> void each_child(F&& f);
};
struct Concat {   // string concatenation, flattened
    static constexpr std::string_view kName = "Concat";
    std::vector<Node*> parts;
    template <class F> void each_child(F&& f);
};
// op: absent for a plain assignment, else the compound operator. A compound assignment also says what it gives (`eval`: the old value
// of `x++`, the new one of `++x`), what its target is (`via`: an address expression, a property's getter call, a dynamic member:
// the target is kept as the lane has it), the operator's sign, operand stack kinds and lifting as a Binary's, and `sym`: a
// user-defined operator's method (op absent then).
struct Assign {
    static constexpr std::string_view kName = "Assign";
    std::optional<std::string> op;
    Node* target = nullptr;
    Node* value = nullptr;
    std::optional<Overflow> overflow;
    std::optional<Eval> eval;
    std::optional<Via> via;
    std::optional<bool> is_signed;
    Stack left_stk = Stack::Unknown;
    Stack right_stk = Stack::Unknown;
    Lift lift = Lift::None;
    SymP sym;
    template <class F> void each_child(F&& f);
};
struct Cond {
    static constexpr std::string_view kName = "Cond";
    Node* cond = nullptr;
    Node* then = nullptr;
    Node* orelse = nullptr;
    template <class F> void each_child(F&& f);
};
// the node's type is the target type; `implicit`: a conversion the source language makes by itself (C's promotions, array decay);
// `conv`: what the conversion does; `from_stk`, `from_signed`: the operand's stack kind and sign; `lift`: null-propagating
struct Cast {
    static constexpr std::string_view kName = "Cast";
    Node* x = nullptr;
    std::optional<Overflow> overflow;
    bool implicit = false;
    std::optional<ConvKind> conv;
    Stack from_stk = Stack::Unknown;
    std::optional<bool> from_signed;
    Lift lift = Lift::None;
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
// body: an expression or a Block; locals, captures, scope, comments, nested: as a MethodDecl's. `sym`: the method the compiler made of
// it (C#'s <M>b__0_0, the JVM's lambda$main$0), where the lane knows it; `mods`: async, static, expression_tree ...; `ret`: its result type.
struct Lambda {
    static constexpr std::string_view kName = "Lambda";
    std::vector<Node*> params;
    Node* body = nullptr;
    std::vector<SymP> locals;
    std::vector<SymP> captures;
    std::optional<std::string> scope;
    std::vector<std::string> comments;
    SymP sym;
    std::vector<std::string> mods;
    std::vector<Node*> nested;
    TypeP ret;
    template <class F> void each_child(F&& f);
};
struct MethodRef {   // mode: invoke / new / virtual_ptr / virtual_delegate; of: the delegate type, where the lane states it
    static constexpr std::string_view kName = "MethodRef";
    Node* target = nullptr;
    SymP sym;
    std::optional<std::string> mode;
    TypeP of;
    template <class F> void each_child(F&& f);
};
// `await x` in an async method (its MethodDecl has the mod "async"): the method stops here until the task x is done, then goes on with
// its result (or throws its exception). The type of the node is the task's result.
struct Await {   // awaiter, result: the pattern's GetAwaiter and GetResult methods, where the lane resolved them
    static constexpr std::string_view kName = "Await";
    Node* x = nullptr;
    SymP awaiter;
    SymP result;
    template <class F> void each_child(F&& f);
};
struct SwitchExpr {   // cases: each arm ends in a Yield or a Throw; `lifted`, `of`: as a Switch's
    static constexpr std::string_view kName = "SwitchExpr";
    Node* expr = nullptr;
    std::vector<Node*> cases;
    bool lifted = false;
    TypeP of;
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

// -- the kinds of a lowered bytecode or binary (docs: work/ilspy-port/l2/ILAST_TO_L2.md section 2.6)

// What stays unstructured: blocks (each a Labeled) that jump to each other by Goto and leave by Break, where the lane could not
// make them a loop or a branch: a binary decompiler's goto clusters, Fernflower's irreducible leftovers, a CIL container. A statement,
// or an expression whose value (its `stack`) the Breaks that leave it give. `entry`: the label control enters at (absent: the first block).
struct Region {
    static constexpr std::string_view kName = "Region";
    RegionKind kind = RegionKind::Normal;
    std::vector<Node*> blocks;
    std::optional<std::string> entry;
    template <class F> void each_child(F&& f);
};
// Statements with a value (GNU's `({ ... })`, Rust, Kotlin and Scala blocks): the statements, then `result` (absent: no value). `role`: what
// the lane made it for (an initialiser, an inlined call), where it says.
struct BlockExpr {
    static constexpr std::string_view kName = "BlockExpr";
    std::optional<std::string> role;
    std::vector<Node*> stmts;
    Node* result = nullptr;
    template <class F> void each_child(F&& f);
};
// C#'s `fixed (T* p = e) body`, C++/CLI's pin_ptr: `var` is a Local with its initialiser.
struct Fixed {
    static constexpr std::string_view kName = "Fixed";
    Node* var = nullptr;
    Node* body = nullptr;
    template <class F> void each_child(F&& f);
};
struct ThrowExpr {   // a throw where a value is wanted (C#, Kotlin, Rust, Scala); x absent: a rethrow
    static constexpr std::string_view kName = "ThrowExpr";
    Node* x = nullptr;
    template <class F> void each_child(F&& f);
};
// `a?.b`: an OptionalChain is the scope, an OptionalUnwrap its leaf: yields null for the whole chain if its operand is null. `ref_in`: the operand is a reference.
struct OptionalChain {
    static constexpr std::string_view kName = "OptionalChain";
    Node* x = nullptr;
    template <class F> void each_child(F&& f);
};
struct OptionalUnwrap {
    static constexpr std::string_view kName = "OptionalUnwrap";
    Node* x = nullptr;
    bool ref_in = false;
    template <class F> void each_child(F&& f);
};
// A pattern (Java 21's record patterns, Python `match`, Rust, Scala, C#): test `tested` against a type (`test`, absent: any), bind it to `var`,
// deconstruct through `deconstruct` (a method) or as a tuple, and match `subs` (nested PatternMatch or comparisons). InstanceOf is the degenerate case.
struct PatternMatch {
    static constexpr std::string_view kName = "PatternMatch";
    SymP var;
    Node* tested = nullptr;
    TypeP test;
    bool check_not_null = false;
    SymP deconstruct;
    bool is_tuple = false;
    std::vector<Node*> subs;
    template <class F> void each_child(F&& f);
};
// Tuple unpacking with conversions (Python, JS, Rust): the temporaries it stores first (`init`: Assigns), the pattern, then the
// conversions and the assignments (BlockExprs).
struct Destructure {
    static constexpr std::string_view kName = "Destructure";
    std::vector<Node*> init;
    Node* pattern = nullptr;
    Node* conversions = nullptr;
    Node* assignments = nullptr;
    template <class F> void each_child(F&& f);
};
struct Default {   // the default value of the node's type (C# `default`, Kotlin, Go's zero value, C++ `T{}`)
    static constexpr std::string_view kName = "Default";
    template <class F> void each_child(F&&) {}
};
struct SizeOf {   // the size in bytes of a type (C, C#)
    static constexpr std::string_view kName = "SizeOf";
    TypeP of;
    template <class F> void each_child(F&&) {}
};
struct StackAlloc {   // memory on the stack (alloca, C# `stackalloc`): `count` elements of `elem` (absent: `count` bytes); `span`: a span of them, not a pointer
    static constexpr std::string_view kName = "StackAlloc";
    TypeP elem;
    Node* count = nullptr;
    bool span = false;
    template <class F> void each_child(F&& f);
};
// An operation of one lane with no general node (CIL's ckfinite, C#'s dynamic call sites) or a shared one with no syntax (mem.copy): `name` is in
// `intrinsic_names()`, which is also the rule for adding one. Operands are `args`; the rest are what the name's row says (types, symbols, strings,
// integers, and per-operand metadata).
struct ArgMeta {
    std::optional<std::string> name;
    std::uint32_t flags = 0;
    TypeP type;
    friend bool operator==(const ArgMeta&, const ArgMeta&) = default;
};
struct Intrinsic {
    static constexpr std::string_view kName = "Intrinsic";
    std::string name;
    std::vector<Node*> args;
    std::vector<TypeP> targs;
    std::vector<SymP> syms;
    std::vector<std::string> strs;
    std::vector<std::int64_t> imms;
    std::vector<ArgMeta> argmeta;
    template <class F> void each_child(F&& f);
};

// The declarations that carry many slots (a class, a method, a unit, a property, a type parameter, an assembly fragment) are
// large and rare; the statements and expressions that make up almost every tree are small. A node holds the large ones
// through a `Boxed`, so that a node of the common kinds is no bigger than the biggest of them (and a tree of a million
// expressions is not made of the size of a ClassDecl). `get<K>` and `visit` hide the box.
template <class T>
class Boxed {
public:
    Boxed() : p_(std::make_unique<T>()) {}
    explicit Boxed(T value) : p_(std::make_unique<T>(std::move(value))) {}
    Boxed(const Boxed& other) : p_(std::make_unique<T>(*other.p_)) {}   // a copy of a node is a copy of what it holds
    Boxed& operator=(const Boxed& other) {
        if (this != &other) p_ = std::make_unique<T>(*other.p_);
        return *this;
    }
    Boxed(Boxed&&) noexcept = default;
    Boxed& operator=(Boxed&&) noexcept = default;
    T* get() { return p_.get(); }
    const T* get() const { return p_.get(); }

private:
    std::unique_ptr<T> p_;
};

template <class K, class... Ts> inline constexpr bool kOneOfKinds = (std::is_same_v<K, Ts> || ...);
template <class K> inline constexpr bool kBoxedKind =
    kOneOfKinds<K, Module, Unit, ClassDecl, MethodDecl, Property, TypeParam, MachineFragment, Lambda, PatternMatch, Intrinsic>;
template <class K> using Stored = std::conditional_t<kBoxedKind<K>, Boxed<K>, K>;

using Kind = std::variant<
    // declarations
    Boxed<Module>, Boxed<Unit>, Boxed<ClassDecl>, FieldDecl, Boxed<MethodDecl>, Param, EnumConst, Boxed<Property>, Boxed<TypeParam>,
    // statements
    Block, Local, ExprStmt, If, While, DoWhile, For, ForEach, Switch, Case, Break, Continue, Return, Throw, Try, Catch,
    Labeled, Goto, Boxed<MachineFragment>, Sync, Empty, Comment, LocalClass, Assert, CtorCall, Yield, YieldReturn, YieldBreak,
    // expressions
    Lit, Name, Select, TypeName, This, Super, Call, New, NewArray, Index, Length, Unary, Binary, Concat, Assign, Cond, Cast,
    InstanceOf, ClassLit, Boxed<Lambda>, MethodRef, Await, SwitchExpr, AddressOf, FarAddress, Piece, Join, Paren, Unsupported,
    // the kinds of a lowered bytecode or binary (at the end, so the others keep their place in the variant)
    Region, BlockExpr, Fixed, ThrowExpr, OptionalChain, OptionalUnwrap, Boxed<PatternMatch>, Destructure, Default, SizeOf, StackAlloc,
    Boxed<Intrinsic>>;

// A node: its place in the source, where in a binary it came from (shared, immutable, and absent for source), the type of
// an expression (empty for a node that is not one), and what kind of node it is. A lowered bytecode or binary adds the stack kind of
// the node's value (`stack`: Unknown where not stated, see `stack_of`), its prefix bits (`flags`: NodeFlag bits) and `info`, the
// facts only that lane has (null unless something is set: no allocation in the common case).
struct Node {
    std::uint32_t line = 0;
    std::uint32_t col = 0;
    std::uint32_t flags = 0;
    Stack stack = Stack::Unknown;
    std::shared_ptr<const Origin> origin;
    TypeP type;
    InfoP info;
    Kind kind;
};

enum class Category : std::uint8_t { Declaration, Statement, Expression };

// ---- reading a tree ---------------------------------------------------------------------------------------------------

// The node as a K (a pointer to its struct), or null when it is another kind (or the node is null).
template <class K> K* get(Node* node) {
    if (node == nullptr) return nullptr;
    if constexpr (kBoxedKind<K>) {
        auto* boxed = std::get_if<Boxed<K>>(&node->kind);
        return boxed != nullptr ? boxed->get() : nullptr;
    } else {
        return std::get_if<K>(&node->kind);
    }
}
template <class K> const K* get(const Node* node) {
    if (node == nullptr) return nullptr;
    if constexpr (kBoxedKind<K>) {
        const auto* boxed = std::get_if<Boxed<K>>(&node->kind);
        return boxed != nullptr ? boxed->get() : nullptr;
    } else {
        return std::get_if<K>(&node->kind);
    }
}
template <class K> bool is(const Node* node) { return get<K>(node) != nullptr; }

// Calls f with the node's struct (a `Block&`, a `ClassDecl&` ...): the closed set of kinds visited as std::visit would,
// with the boxes opened. f must accept every kind.
template <class F> decltype(auto) visit(Node& node, F&& f) {
    return std::visit(
        [&f](auto& stored) -> decltype(auto) {
            if constexpr (requires { stored.get(); }) return f(*stored.get());
            else return f(stored);
        },
        node.kind);
}
template <class F> decltype(auto) visit(const Node& node, F&& f) {
    return std::visit(
        [&f](const auto& stored) -> decltype(auto) {
            if constexpr (requires { stored.get(); }) return f(*stored.get());
            else return f(stored);
        },
        node.kind);
}

// The kind as L2 spells it: "Block", "Binary" ...
std::string_view kind_name(const Node& node);
Category category(const Node& node);

// The stack kind of a node's value: `node.stack` where the lane stated it, else derived from the node's type (bool, the 8, 16 and 32
// bit integers and char: I4; the 64 bit ones: I8; f32: F4; f64 and f80: F8; pointers and function pointers: I, a managed pointer: Ref; classes, arrays,
// null, type variables, dynamic: O; void: Void), else Unknown. For a lowered bytecode `node.stack` is the fact and the type a hint: do not derive.
Stack stack_of(const Node& node);
Stack stack_of(const Type& type);

inline bool has_flag(const Node& node, NodeFlag flag) { return (node.flags & bit(flag)) != 0; }
inline void set_flag(Node& node, NodeFlag flag, bool on = true) { node.flags = on ? (node.flags | bit(flag)) : (node.flags & ~bit(flag)); }

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
// (The kind is made in place: a default node would make the variant's first alternative, a boxed Module, only to throw it away.)
template <class K>
Node* make(Arena& arena, K kind, TypeP type = {}, std::uint32_t line = 0, std::uint32_t col = 0) {
    return arena.make<Node>(Node{.line = line, .col = col, .type = std::move(type), .kind = Kind(std::in_place_type<Stored<K>>, std::move(kind))});
}

// A new node of the named kind ("Block", "Binary" ...) with every slot empty; null for a name that is no kind.
Node* make_node(Arena& arena, std::string_view kind_name);

// A decimal literal (the type `System.Decimal`, `decimal_type()`) with its exact text, scale kept: "1.50" is not "1.5".
Node* make_decimal(Arena& arena, std::string text);

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
CB_IR_EACH_CHILD(MethodDecl, params, body, nested)
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
CB_IR_EACH_CHILD(Try, resources, body, catches, finally_body, fault_body)
CB_IR_EACH_CHILD(Catch, filter, body)
CB_IR_EACH_CHILD(Break, value)
CB_IR_EACH_CHILD(Labeled, body)
CB_IR_EACH_CHILD(Sync, lock, body)
CB_IR_EACH_CHILD(LocalClass, decl)
CB_IR_EACH_CHILD(Assert, cond, detail)
CB_IR_EACH_CHILD(CtorCall, args, target)
CB_IR_EACH_CHILD(Yield, expr)
CB_IR_EACH_CHILD(YieldReturn, expr)
CB_IR_EACH_CHILD(Await, x)
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
CB_IR_EACH_CHILD(Lambda, params, body, nested)
CB_IR_EACH_CHILD(MethodRef, target)
CB_IR_EACH_CHILD(SwitchExpr, expr, cases)
CB_IR_EACH_CHILD(FarAddress, selector, offset)
CB_IR_EACH_CHILD(Piece, x)
CB_IR_EACH_CHILD(Join, hi, lo)
CB_IR_EACH_CHILD(Paren, x)
CB_IR_EACH_CHILD(Region, blocks)
CB_IR_EACH_CHILD(BlockExpr, stmts, result)
CB_IR_EACH_CHILD(Fixed, var, body)
CB_IR_EACH_CHILD(ThrowExpr, x)
CB_IR_EACH_CHILD(OptionalChain, x)
CB_IR_EACH_CHILD(OptionalUnwrap, x)
CB_IR_EACH_CHILD(PatternMatch, tested, subs)
CB_IR_EACH_CHILD(Destructure, init, pattern, conversions, assignments)
CB_IR_EACH_CHILD(StackAlloc, count)
CB_IR_EACH_CHILD(Intrinsic, args)

#undef CB_IR_EACH_CHILD

}  // namespace cb::translate::ir
