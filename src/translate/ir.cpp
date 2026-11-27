// translate/ir.cpp -- the translator's intermediate representation (see ir.hpp).
#include "translate/ir.hpp"

#include <algorithm>
#include <array>
#include <functional>
#include <iterator>
#include <type_traits>

namespace cb::translate::ir {
namespace {

using std::string;
using std::string_view;

template <class K, class... Ts> inline constexpr bool kOneOf = (std::is_same_v<K, Ts> || ...);

// A boost-style combine; the hash only has to agree for equal types.
std::size_t combine(std::size_t seed, std::size_t value) { return seed ^ (value + 0x9E3779B97F4A7C15ull + (seed << 6) + (seed >> 2)); }

}  // namespace

// ---- enums as text -------------------------------------------------------------------------------------------------------

std::string_view to_text(Prim prim) {
    constexpr string_view names[] = {"bool", "i8", "i16", "i32", "i64", "u8", "u16", "u32", "u64", "f32", "f64", "f80", "char"};
    return names[static_cast<std::size_t>(prim)];
}

std::string_view to_text(AddressSpace space) {
    constexpr string_view names[] = {"flat", "near", "far", "huge", "real", "io"};
    return names[static_cast<std::size_t>(space)];
}

std::string_view to_text(MemoryModel memory) {
    constexpr string_view names[] = {"gc", "owned", "borrowed", "stack", "manual", "emulated"};
    return names[static_cast<std::size_t>(memory)];
}

std::string_view to_text(Overflow overflow) {
    constexpr string_view names[] = {"wrap", "saturate", "trap"};
    return names[static_cast<std::size_t>(overflow)];
}

std::string_view to_text(CallingConvention conv) {
    constexpr string_view names[] = {"cdecl", "pascal", "stdcall", "fastcall", "thiscall", "register", "interrupt"};
    return names[static_cast<std::size_t>(conv)];
}

std::string_view to_text(SymKind kind) {
    constexpr string_view names[] = {"local", "param", "field", "method", "ctor", "class", "enumconst", "typevar",
                                     "package", "global", "function", "label", "property", "event"};
    return names[static_cast<std::size_t>(kind)];
}

std::string_view to_text(PassMode mode) {
    constexpr string_view names[] = {"", "ref", "out", "in"};
    return names[static_cast<std::size_t>(mode)];
}

// What a bytecode or a binary states: the names are the document's, and the index of a name is the value of the enumerator.
namespace {

constexpr string_view kStackNames[] = {"unknown", "i4", "i", "i8", "f4", "f8", "o", "ref", "void"};
constexpr string_view kLiftNames[] = {"none", "csharp", "three_valued"};
constexpr string_view kCoalesceNames[] = {"ref", "nullable", "nullable_value"};
constexpr string_view kConvNames[] = {"invalid",         "nop",          "int_to_float",       "float_to_int", "float_precision", "sign_extend",
                                      "zero_extend",     "truncate",     "stop_gc_tracking",   "start_gc_tracking", "object_interior", "box",
                                      "unbox",           "unbox_any",    "ref_cast",           "try_cast",     "expr_tree"};
constexpr string_view kEvalNames[] = {"old", "new"};
constexpr string_view kViaNames[] = {"address", "accessor", "dynamic"};
constexpr string_view kFormNames[] = {"default", "virtual", "direct", "indirect"};
constexpr string_view kRegionNames[] = {"normal", "loop", "while", "do_while", "for", "switch"};
constexpr string_view kEmptyNames[] = {"normal", "pop"};
constexpr string_view kRoleNames[] = {"local",    "param",   "this",     "temp",        "catch_var",  "loop_var", "resource",
                                      "pinned",   "pattern_binding", "captured", "init_target", "named_arg", "decon_temp"};
constexpr string_view kEntityKindNames[] = {"type", "method", "field", "property", "event", "parameter", "module"};
constexpr string_view kFlagNames[] = {"volatile",    "unaligned1", "unaligned2",    "unaligned4",     "read_only",         "tail",
                                      "stack_empty", "stack_adjust", "delay_exc",   "from_end",       "rvalue_addr",       "if_ref",
                                      "inline_array", "pinnable_ref", "utf8_literal", "compiler_generated", "has_context"};
constexpr NodeFlag kFlags[] = {NodeFlag::Volatile,    NodeFlag::Unaligned1,  NodeFlag::Unaligned2,  NodeFlag::Unaligned4,
                               NodeFlag::ReadOnly,    NodeFlag::Tail,        NodeFlag::StackEmpty,  NodeFlag::StackAdjust,
                               NodeFlag::DelayExc,    NodeFlag::FromEnd,     NodeFlag::RValueAddr,  NodeFlag::IfRef,
                               NodeFlag::InlineArray, NodeFlag::PinnableRef, NodeFlag::Utf8Literal, NodeFlag::CompilerGenerated,
                               NodeFlag::HasContext};
static_assert(std::size(kFlags) == std::size(kFlagNames));

template <class E, std::size_t N> std::optional<E> by_name(string_view text, const string_view (&names)[N]) {
    for (std::size_t i = 0; i < N; ++i) {
        if (names[i] == text) return static_cast<E>(i);
    }
    return std::nullopt;
}

}  // namespace

std::string_view to_text(Stack v) { return kStackNames[static_cast<std::size_t>(v)]; }
std::string_view to_text(Lift v) { return kLiftNames[static_cast<std::size_t>(v)]; }
std::string_view to_text(Coalesce v) { return kCoalesceNames[static_cast<std::size_t>(v)]; }
std::string_view to_text(ConvKind v) { return kConvNames[static_cast<std::size_t>(v)]; }
std::string_view to_text(Eval v) { return kEvalNames[static_cast<std::size_t>(v)]; }
std::string_view to_text(Via v) { return kViaNames[static_cast<std::size_t>(v)]; }
std::string_view to_text(CallForm v) { return kFormNames[static_cast<std::size_t>(v)]; }
std::string_view to_text(RegionKind v) { return kRegionNames[static_cast<std::size_t>(v)]; }
std::string_view to_text(EmptyKind v) { return kEmptyNames[static_cast<std::size_t>(v)]; }
std::string_view to_text(VarRole v) { return kRoleNames[static_cast<std::size_t>(v)]; }
std::string_view to_text(EntityKind v) { return kEntityKindNames[static_cast<std::size_t>(v)]; }
std::string_view to_text(NodeFlag flag) {
    for (std::size_t i = 0; i < std::size(kFlags); ++i) {
        if (kFlags[i] == flag) return kFlagNames[i];
    }
    return "";
}
std::span<const NodeFlag> all_node_flags() { return kFlags; }

template <> std::optional<Stack> from_text<Stack>(std::string_view text) { return by_name<Stack>(text, kStackNames); }
template <> std::optional<Lift> from_text<Lift>(std::string_view text) { return by_name<Lift>(text, kLiftNames); }
template <> std::optional<Coalesce> from_text<Coalesce>(std::string_view text) { return by_name<Coalesce>(text, kCoalesceNames); }
template <> std::optional<ConvKind> from_text<ConvKind>(std::string_view text) { return by_name<ConvKind>(text, kConvNames); }
template <> std::optional<Eval> from_text<Eval>(std::string_view text) { return by_name<Eval>(text, kEvalNames); }
template <> std::optional<Via> from_text<Via>(std::string_view text) { return by_name<Via>(text, kViaNames); }
template <> std::optional<CallForm> from_text<CallForm>(std::string_view text) { return by_name<CallForm>(text, kFormNames); }
template <> std::optional<RegionKind> from_text<RegionKind>(std::string_view text) { return by_name<RegionKind>(text, kRegionNames); }
template <> std::optional<EmptyKind> from_text<EmptyKind>(std::string_view text) { return by_name<EmptyKind>(text, kEmptyNames); }
template <> std::optional<VarRole> from_text<VarRole>(std::string_view text) { return by_name<VarRole>(text, kRoleNames); }
template <> std::optional<NodeFlag> from_text<NodeFlag>(std::string_view text) {
    for (std::size_t i = 0; i < std::size(kFlags); ++i) {
        if (kFlagNames[i] == text) return kFlags[i];
    }
    return std::nullopt;
}

// ---- entities and info ---------------------------------------------------------------------------------------------------

bool same_entity(const EntityP& a, const EntityP& b) {
    if (!a || !b) return false;
    if (a.get() == b.get()) return true;
    return a->family() == b->family() && a->kind() == b->kind() && a->key() == b->key();
}

bool same_entity(const TypeP& a, const TypeP& b) { return a && b && same_entity(a->entity, b->entity); }

const InfoValue* Info::find(std::string_view key) const {
    const auto it = std::lower_bound(items_.begin(), items_.end(), key, [](const Item& item, std::string_view k) { return item.first < k; });
    return it != items_.end() && it->first == key ? &it->second : nullptr;
}

void Info::set(std::string key, InfoValue value) {
    const auto it = std::lower_bound(items_.begin(), items_.end(), std::string_view(key), [](const Item& item, std::string_view k) { return item.first < k; });
    if (it != items_.end() && it->first == key) it->second = std::move(value);
    else items_.insert(it, Item{std::move(key), std::move(value)});
}

bool Info::erase(std::string_view key) {
    const auto it = std::lower_bound(items_.begin(), items_.end(), key, [](const Item& item, std::string_view k) { return item.first < k; });
    if (it == items_.end() || it->first != key) return false;
    items_.erase(it);
    return true;
}

namespace {

bool same_info_value(const InfoValue& a, const InfoValue& b) {
    if (a.index() != b.index()) return false;
    return std::visit(
        [&b](const auto& x) {
            using X = std::decay_t<decltype(x)>;
            const X& y = std::get<X>(b);
            if constexpr (std::is_same_v<X, EntityP>) return x ? same_entity(x, y) : !y;
            else if constexpr (std::is_same_v<X, SymP>) return x.get() == y.get();   // (a symbol is one object)
            else return x == y;
        },
        a);
}

}  // namespace

bool operator==(const Info& a, const Info& b) {
    if (a.items_.size() != b.items_.size()) return false;
    for (std::size_t i = 0; i < a.items_.size(); ++i) {
        if (a.items_[i].first != b.items_[i].first || !same_info_value(a.items_[i].second, b.items_[i].second)) return false;
    }
    return true;
}

bool operator==(const InfoP& a, const InfoP& b) {
    if (!a || !b) return !a && !b;
    return *a.p_ == *b.p_;
}

namespace {

constexpr InfoKey kInfoKeys[] = {
    {"cil.async_catch_offset", InfoOn::Node, InfoValueKind::Int, "MethodDecl Lambda", false,
     "of an async method: the IL offset of the state machine's catch handler (ILSpy's AsyncDebugInfo.CatchHandlerOffset; present: the info is not the default)"},
    {"cil.async_debug", InfoOn::Node, InfoValueKind::NodeRefs, "MethodDecl Lambda", false,
     "the Await nodes of an async method (ILSpy's AsyncDebugInfo), by their place in a walk of the function"},
    {"cil.async_return_type", InfoOn::Node, InfoValueKind::Type, "MethodDecl Lambda", false, "the type an async method's task gives (ILFunction.AsyncReturnType)"},
    {"cil.calli_sig", InfoOn::Node, InfoValueKind::Type, "Call", false, "a calli's signature (the FnPtrType of CallIndirect.FunctionPointerType); the target's own type is a native int"},
    {"cil.capture_scope", InfoOn::Sym, InfoValueKind::String, "", true, "the region (the entry label of a Region) a closure variable is captured in (ILVariable.CaptureScope); other lanes' closures have none yet"},
    {"cil.class_tparams", InfoOn::Node, InfoValueKind::Types, "MethodDecl Lambda", false,
     "the class type parameters in scope where they are not the method's own (ILFunction.GenericContext of a lambda, local function or state machine); with cil.method_tparams"},
    {"cil.code_size", InfoOn::Node, InfoValueKind::Int, "MethodDecl Lambda", false, "the size in bytes of the IL body"},
    {"cil.constrained", InfoOn::Node, InfoValueKind::Type, "Call", false, "the `constrained.` prefix's type of a call"},
    {"cil.dynamic", InfoOn::Type, InfoValueKind::Bool, "", false, "an `object` that was annotated `dynamic` (DynamicType is the L2 type: this keeps the object origin)"},
    {"cil.helper_count", InfoOn::Node, InfoValueKind::Int, "MethodDecl Lambda", false,
     "how many helper variables the function has named I_0, I_1 ... (ILFunction.RegisterVariable's counter), so a variable made later gets the next name"},
    {"cil.kind_exact", InfoOn::Sym, InfoValueKind::String, "", false, "the exact ILSpy VariableKind, where VarRole merges two (PinnedLocal vs PinnedRegionLocal, ExceptionStackSlot vs ExceptionLocal)"},
    {"cil.locals_sig_len", InfoOn::Node, InfoValueKind::Int, "MethodDecl Lambda", false, "the length of the method's local variable signature"},
    {"cil.method_tparams", InfoOn::Node, InfoValueKind::Types, "MethodDecl Lambda", false,
     "the method type parameters in scope where they are not the method's own; with cil.class_tparams"},
    {"cil.mono_state_machine", InfoOn::Node, InfoValueKind::Bool, "MethodDecl Lambda", false, "a state machine the Mono compiler made"},
    {"cil.move_next", InfoOn::Node, InfoValueKind::Entity, "MethodDecl Lambda", false, "the MoveNext method of the state machine this function was made from"},
    {"cil.name", InfoOn::Node, InfoValueKind::String, "Lambda", false, "a lambda's name where it is not its method's (ILFunction.Name)"},
    {"cil.native_int", InfoOn::Type, InfoValueKind::Bool, "", false, "`nint`/`nuint`, not the spelling `IntPtr`/`UIntPtr`"},
    {"cil.null_key", InfoOn::Node, InfoValueKind::Int, "Intrinsic", false, "of a cil.string_to_int: the index of the key that is null"},
    {"cil.nullability", InfoOn::Type, InfoValueKind::String, "", true, "oblivious, annotated or not_annotated: C#'s `string?` beyond ClassType.nullable (Kotlin, Swift and TypeScript state it too)"},
    {"cil.pinned", InfoOn::Type, InfoValueKind::Bool, "", false, "a pinned local's type"},
    {"cil.reduced_method", InfoOn::Node, InfoValueKind::Entity, "MethodDecl Lambda", false, "the local function's reduced method (ILFunction.ReducedMethod)"},
    {"cil.ref_kind", InfoOn::Type, InfoValueKind::String, "", false, "ref, in, out or ref_readonly, on a managed PtrType"},
    {"cil.remove_if_redundant", InfoOn::Sym, InfoValueKind::Bool, "", false, "ILVariable.RemoveIfRedundant"},
    {"cil.seq_points", InfoOn::Node, InfoValueKind::Ints, "MethodDecl Lambda", false, "the IL offsets that are sequence point candidates"},
    {"cil.severity", InfoOn::Node, InfoValueKind::String, "Unsupported", false, "of an il.invalid_expression node: the severity of its message, where it is not Error"},
    {"cil.spec_range", InfoOn::Node, InfoValueKind::Ints, "Catch", false, "the IL range [start, end) of the exception specifier of a catch handler"},
    {"cil.state_machine_field", InfoOn::Sym, InfoValueKind::Entity, "", false, "the field of the state machine a variable stands for (ILVariable.StateMachineField)"},
    {"cil.target_r", InfoOn::Node, InfoValueKind::Bool, "Cast", false, "of a conversion to f64: the target was the CIL type R, not R8"},
    {"cil.tuple_ctor", InfoOn::Type, InfoValueKind::Bool, "", false, "a ValueTuple<...> and its tuple view"},
    {"cil.unknown_ref", InfoOn::Type, InfoValueKind::String, "", false, "an UnknownType's FullTypeName"},
    {"cil.vb_state_machine", InfoOn::Node, InfoValueKind::Bool, "MethodDecl Lambda", false, "a state machine the legacy Visual Basic compiler made"},
};

constexpr IntrinsicName kIntrinsics[] = {
    {"cil.arglist", "none", "i", "", "the CIL's vararg handle: no general operation names it", "cil", "translate.l2.intrinsics.every_name_round_trips"},
    {"cil.ckfinite", "args[0]: a float", "arg", "", "throws when its operand is NaN or infinite, else gives it back: no general operator", "cil", "translate.l2.intrinsics.every_name_round_trips"},
    {"cil.deconstruct_result", "args[0]: the deconstruct call's value; imms[0]: the result's index", "stated", "", "a piece of a C# deconstruction's call: tied to the Destructure's call shape", "cil", "translate.l2.intrinsics.every_name_round_trips"},
    {"cil.ldtoken_member", "syms[0]: the member", "o", "", "a runtime handle of a field or method: reflection, no general node", "cil", "translate.l2.intrinsics.every_name_round_trips"},
    {"cil.ldtoken_type", "targs[0]: the type", "o", "", "a runtime type handle: reflection, no general node (ClassLit is the language's `T.class`)", "cil", "translate.l2.intrinsics.every_name_round_trips"},
    {"cil.makeref", "args[0]: an address; targs[0]: the type", "o", "", "CIL's typed reference (TypedReference): no other lane has one", "cil", "translate.l2.intrinsics.every_name_round_trips"},
    {"cil.reftype", "args[0]: a typed reference", "o", "", "CIL's typed reference: its type handle", "cil", "translate.l2.intrinsics.every_name_round_trips"},
    {"cil.refvalue", "args[0]: a typed reference; targs[0]: the type", "ref", "", "CIL's typed reference: the address it holds", "cil", "translate.l2.intrinsics.every_name_round_trips"},
    {"cil.string_to_int", "args[0]: a string; strs: the keys; imms: their values; targs[0]: the expected type", "i4", "", "a compiler's string switch lowered to a lookup: C# only", "cil", "translate.l2.intrinsics.every_name_round_trips"},
    {"cs.dynamic.binary", "args: the operands; imms[0]: binder flags; imms[1]: the operation; argmeta: one per operand", "o", "has_context", "a C# `dynamic` binder call site: the C# runtime binder resolves it at run time (DynamicType types the value; this is the site)", "cil", "translate.l2.intrinsics.every_name_round_trips"},
    {"cs.dynamic.compound_assign", "args: the target, then the value; imms[0]: binder flags; imms[1]: the operation; imms[2]: the target kind (address accessor dynamic); imms[3]: the eval mode (old new); argmeta: one per operand", "o", "has_context", "a C# `dynamic` binder call site", "cil", "translate.l2.intrinsics.every_name_round_trips"},
    {"cs.dynamic.convert", "args[0]: the value; targs[0]: the type; imms[0]: binder flags", "stated", "has_context", "a C# `dynamic` binder call site", "cil", "translate.l2.intrinsics.every_name_round_trips"},
    {"cs.dynamic.get_index", "args: the target, then the indices; imms[0]: binder flags; argmeta: one per operand", "o", "has_context", "a C# `dynamic` binder call site", "cil", "translate.l2.intrinsics.every_name_round_trips"},
    {"cs.dynamic.get_member", "args[0]: the target; strs[0]: the name; imms[0]: binder flags; argmeta", "o", "has_context", "a C# `dynamic` binder call site (Python attribute access is the general form: a Select on a dynamic target)", "cil", "translate.l2.intrinsics.every_name_round_trips"},
    {"cs.dynamic.invoke", "args: the target, then the arguments; imms[0]: binder flags; argmeta", "o", "has_context", "a C# `dynamic` binder call site", "cil", "translate.l2.intrinsics.every_name_round_trips"},
    {"cs.dynamic.invoke_constructor", "args: the arguments; targs: the type, then the context; imms[0]: binder flags; argmeta", "o", "has_context", "a C# `dynamic` binder call site", "cil", "translate.l2.intrinsics.every_name_round_trips"},
    {"cs.dynamic.invoke_member", "args: the target, then the arguments; strs[0]: the name; targs: the type arguments; imms[0]: binder flags; argmeta", "o", "has_context", "a C# `dynamic` binder call site", "cil", "translate.l2.intrinsics.every_name_round_trips"},
    {"cs.dynamic.is_event", "args[0]: the target; strs[0]: the name; imms[0]: binder flags", "i4", "has_context", "a C# `dynamic` binder call site", "cil", "translate.l2.intrinsics.every_name_round_trips"},
    {"cs.dynamic.logic", "args: the operands; imms[0]: binder flags; imms[1]: the operation; argmeta", "o", "has_context", "a C# `dynamic` binder call site", "cil", "translate.l2.intrinsics.every_name_round_trips"},
    {"cs.dynamic.set_index", "args: the target, the indices, then the value; imms[0]: binder flags; argmeta", "o", "has_context", "a C# `dynamic` binder call site", "cil", "translate.l2.intrinsics.every_name_round_trips"},
    {"cs.dynamic.set_member", "args: the target, then the value; strs[0]: the name; imms[0]: binder flags; argmeta", "o", "has_context", "a C# `dynamic` binder call site", "cil", "translate.l2.intrinsics.every_name_round_trips"},
    {"cs.dynamic.unary", "args[0]: the operand; imms[0]: binder flags; imms[1]: the operation; argmeta", "o", "has_context", "a C# `dynamic` binder call site", "cil", "translate.l2.intrinsics.every_name_round_trips"},
    {"debug.break", "none", "void", "", "a debugger break (`__debugbreak`, int3): no syntax in most languages", "cil native", "translate.l2.intrinsics.every_name_round_trips"},
    {"mem.copy", "args[0]: the destination; args[1]: the source; args[2]: the size in bytes", "void", "volatile unaligned1 unaligned2 unaligned4", "a block copy (`memcpy`, `rep movs`, CIL cpblk): a call in C and C++, nothing in Java", "cil clang", "translate.l2.intrinsics.every_name_round_trips"},
    {"mem.fill", "args[0]: the address; args[1]: the byte; args[2]: the size in bytes", "void", "volatile unaligned1 unaligned2 unaligned4", "a block fill (`memset`, `rep stos`, CIL initblk): a call in C and C++, nothing in Java", "cil clang", "translate.l2.intrinsics.every_name_round_trips"},
};

}  // namespace

std::span<const InfoKey> info_keys() { return kInfoKeys; }

const InfoKey* find_info_key(std::string_view key) {
    const auto it = std::lower_bound(std::begin(kInfoKeys), std::end(kInfoKeys), key, [](const InfoKey& k, std::string_view s) { return k.key < s; });
    return it != std::end(kInfoKeys) && it->key == key ? &*it : nullptr;
}

std::span<const IntrinsicName> intrinsic_names() { return kIntrinsics; }

const IntrinsicName* find_intrinsic(std::string_view name) {
    const auto it = std::lower_bound(std::begin(kIntrinsics), std::end(kIntrinsics), name, [](const IntrinsicName& n, std::string_view s) { return n.name < s; });
    return it != std::end(kIntrinsics) && it->name == name ? &*it : nullptr;
}

// ---- types ---------------------------------------------------------------------------------------------------------------

bool operator==(const TypeP& a, const TypeP& b) {
    if (a.get() == b.get()) return true;
    if (a.get() == nullptr || b.get() == nullptr) return false;
    return *a == *b;
}

bool Type::is_ref() const {
    return std::holds_alternative<ClassType>(body) || std::holds_alternative<ArrayType>(body) ||
           std::holds_alternative<TypeVarType>(body) || std::holds_alternative<NullType>(body) ||
           std::holds_alternative<WildcardType>(body);
}

std::string_view Type::kind() const {
    struct Visitor {
        string_view operator()(const PrimType& t) const { return to_text(t.prim); }
        string_view operator()(const VoidType&) const { return "void"; }
        string_view operator()(const NullType&) const { return "null"; }
        string_view operator()(const AnyType&) const { return "any"; }
        string_view operator()(const DynamicType&) const { return "dynamic"; }
        string_view operator()(const ClassType&) const { return "class"; }
        string_view operator()(const TypeVarType&) const { return "typevar"; }
        string_view operator()(const WildcardType&) const { return "wildcard"; }
        string_view operator()(const ArrayType&) const { return "array"; }
        string_view operator()(const PtrType&) const { return "ptr"; }
        string_view operator()(const SelectorType&) const { return "selector"; }
        string_view operator()(const FnType&) const { return "fn"; }
        string_view operator()(const FnPtrType&) const { return "fnptr"; }
        string_view operator()(const RecordType&) const { return "record"; }
        string_view operator()(const TupleType&) const { return "tuple"; }
        string_view operator()(const ModType&) const { return "mod"; }
    };
    return std::visit(Visitor{}, body);
}

std::string Type::simple() const {
    const string* name = nullptr;
    if (const auto* c = std::get_if<ClassType>(&body)) name = &c->name;
    else if (const auto* v = std::get_if<TypeVarType>(&body)) name = &v->name;
    else if (const auto* r = std::get_if<RecordType>(&body)) name = r->name ? &*r->name : nullptr;
    if (name == nullptr || name->empty()) return string(kind());
    return name->substr(name->rfind('.') == string::npos ? 0 : name->rfind('.') + 1);
}

std::size_t Type::hash() const {
    std::size_t h = std::hash<string_view>{}(kind());
    if (const auto* c = std::get_if<ClassType>(&body)) {
        h = combine(h, std::hash<string>{}(c->name));
        for (const TypeP& a : c->args) h = combine(h, a ? a->hash() : 0);
    } else if (const auto* v = std::get_if<TypeVarType>(&body)) {
        h = combine(h, std::hash<string>{}(v->name));
    } else if (const auto* r = std::get_if<RecordType>(&body)) {
        h = combine(h, r->name ? std::hash<string>{}(*r->name) : 0);
    } else if (const auto* a = std::get_if<ArrayType>(&body)) {
        h = combine(h, a->elem ? a->elem->hash() : 0);
    } else if (const auto* p = std::get_if<PtrType>(&body)) {
        h = combine(h, p->to ? p->to->hash() : 0);
    } else if (const auto* t = std::get_if<TupleType>(&body)) {
        for (const TypeP& e : t->elems) h = combine(h, e ? e->hash() : 0);
    } else if (const auto* m = std::get_if<ModType>(&body)) {
        h = combine(h, m->inner ? m->inner->hash() : 0);
    }
    return h;
}

std::pair<std::size_t, TypeP> dims(const TypeP& type) {
    std::size_t n = 0;
    TypeP t = type;
    while (t) {
        const auto* a = std::get_if<ArrayType>(&t->body);
        if (a == nullptr) break;
        ++n;
        t = a->elem;
    }
    return {n, t};
}

std::string to_text(const Type& type) {
    if (const auto* a = std::get_if<ArrayType>(&type.body)) return (a->elem ? to_text(*a->elem) : string("?")) + "[]";
    if (const auto* c = std::get_if<ClassType>(&type.body)) {
        string out = c->name;
        if (!c->args.empty()) {
            out += '<';
            for (std::size_t i = 0; i < c->args.size(); ++i) {
                if (i > 0) out += ", ";
                out += c->args[i] ? to_text(*c->args[i]) : string("?");
            }
            out += '>';
        }
        return out;
    }
    if (const auto* v = std::get_if<TypeVarType>(&type.body)) return v->name;
    if (const auto* w = std::get_if<WildcardType>(&type.body)) {
        if (!w->bound) return "?";
        return string("? ") + (w->bound->mode == BoundMode::Extends ? "extends " : "super ") + (w->bound->type ? to_text(*w->bound->type) : string("?"));
    }
    if (const auto* t = std::get_if<TupleType>(&type.body)) {
        string out = "(";
        for (std::size_t i = 0; i < t->elems.size(); ++i) {
            if (i > 0) out += ", ";
            out += t->elems[i] ? to_text(*t->elems[i]) : string("?");
            if (i < t->names.size() && t->names[i]) out += " " + *t->names[i];
        }
        return out + ")";
    }
    if (const auto* m = std::get_if<ModType>(&type.body)) return m->inner ? to_text(*m->inner) : string("?");
    return string(type.kind());
}

TypeP make_type(Type::Body body) { return TypeP(std::make_shared<const Type>(Type{std::move(body), nullptr, InfoP{}})); }

TypeP make_type(Type::Body body, EntityP entity, InfoP info) {
    return TypeP(std::make_shared<const Type>(Type{std::move(body), std::move(entity), std::move(info)}));
}

Stack stack_of(const Type& type) {
    struct Visitor {
        Stack operator()(const PrimType& t) const {
            switch (t.prim) {
                case Prim::Bool: case Prim::I8: case Prim::I16: case Prim::I32: case Prim::U8: case Prim::U16: case Prim::U32: case Prim::Char: return Stack::I4;
                case Prim::I64: case Prim::U64: return Stack::I8;
                case Prim::F32: return Stack::F4;
                case Prim::F64: case Prim::F80: return Stack::F8;
            }
            return Stack::Unknown;
        }
        Stack operator()(const VoidType&) const { return Stack::Void; }
        Stack operator()(const PtrType& t) const { return t.managed ? Stack::Ref : Stack::I; }
        Stack operator()(const FnPtrType&) const { return Stack::I; }
        Stack operator()(const NullType&) const { return Stack::O; }
        Stack operator()(const DynamicType&) const { return Stack::O; }
        Stack operator()(const ClassType&) const { return Stack::O; }
        Stack operator()(const TypeVarType&) const { return Stack::O; }
        Stack operator()(const ArrayType&) const { return Stack::O; }
        Stack operator()(const ModType& t) const { return t.inner ? stack_of(*t.inner) : Stack::Unknown; }
        Stack operator()(const AnyType&) const { return Stack::Unknown; }
        Stack operator()(const WildcardType&) const { return Stack::O; }
        Stack operator()(const SelectorType&) const { return Stack::Unknown; }
        Stack operator()(const FnType&) const { return Stack::Unknown; }
        Stack operator()(const RecordType&) const { return Stack::Unknown; }
        Stack operator()(const TupleType&) const { return Stack::Unknown; }
    };
    return std::visit(Visitor{}, type.body);
}

TypeP prim_type(Prim prim, std::optional<bool> is_signed, std::optional<CharEncoding> encoding) {
    return make_type(PrimType{prim, is_signed, encoding});
}

TypeP class_type(std::string name, std::vector<TypeP> args) {
    ClassType c;
    c.name = std::move(name);
    c.args = std::move(args);
    return make_type(std::move(c));
}

TypeP typevar_type(std::string name) { return make_type(TypeVarType{std::move(name)}); }

TypeP pointer_to(TypeP to, AddressSpace space, std::optional<SegmentRef> seg) {
    PtrType p;
    p.to = std::move(to);
    p.space = space;
    p.seg = std::move(seg);
    return make_type(std::move(p));
}

TypeP array_of(TypeP elem, std::size_t dims) {
    for (std::size_t i = 0; i < dims; ++i) {
        ArrayType a;
        a.elem = std::move(elem);
        elem = make_type(std::move(a));
    }
    return elem;
}

const TypeP& void_type() {
    static const TypeP t = make_type(VoidType{});
    return t;
}
const TypeP& null_type() {
    static const TypeP t = make_type(NullType{});
    return t;
}
const TypeP& any_type() {
    static const TypeP t = make_type(AnyType{});
    return t;
}
const TypeP& bool_type() {
    static const TypeP t = prim_type(Prim::Bool);
    return t;
}
const TypeP& i32_type() {
    static const TypeP t = prim_type(Prim::I32);
    return t;
}
const TypeP& decimal_type() {
    static const TypeP t = class_type("System.Decimal");
    return t;
}

// ---- symbols -------------------------------------------------------------------------------------------------------------

std::optional<std::string> Sym::owner_simple() const {
    if (!owner || owner->empty()) return std::nullopt;
    const std::size_t dot = owner->rfind('.');
    return owner->substr(dot == string::npos ? 0 : dot + 1);
}

SymP make_sym(SymKind kind, std::string name, std::optional<std::string> owner, bool is_static, TypeP type) {
    auto s = std::make_shared<Sym>();
    s->kind = kind;
    s->name = std::move(name);
    s->owner = std::move(owner);
    s->is_static = is_static;
    s->type = std::move(type);
    return s;
}

// ---- nodes ---------------------------------------------------------------------------------------------------------------

std::string_view kind_name(const Node& node) {
    return visit(node, [](const auto& k) { return std::decay_t<decltype(k)>::kName; });
}

Category category(const Node& node) {
    return visit(
        node, [](const auto& k) {
            using K = std::decay_t<decltype(k)>;
            if constexpr (kOneOf<K, Module, Unit, ClassDecl, FieldDecl, MethodDecl, Param, EnumConst, Property, TypeParam>) {
                return Category::Declaration;
            } else if constexpr (kOneOf<K, Block, Local, ExprStmt, If, While, DoWhile, For, ForEach, Switch, Case, Break, Continue, Return,
                                        Throw, Try, Catch, Labeled, Goto, MachineFragment, Sync, Empty, Comment, LocalClass, Assert,
                                        CtorCall, Yield, YieldReturn, YieldBreak, Region, Fixed, Destructure>) {
                return Category::Statement;
            } else {
                return Category::Expression;
            }
        });
}

Stack stack_of(const Node& node) {
    if (node.stack != Stack::Unknown) return node.stack;
    return node.type ? stack_of(*node.type) : Stack::Unknown;
}

std::vector<Node*> children(const Node& node) {
    std::vector<Node*> out;
    // (the slots are read, not changed: the walk only needs the same accessor a rewriting pass uses)
    visit(const_cast<Node&>(node), [&out](auto& k) { k.each_child([&out](Node*& child) { out.push_back(child); }); });
    return out;
}

std::vector<Node*> walk(Node* root) {
    std::vector<Node*> out;
    if (root == nullptr) return out;
    std::vector<Node*> stack{root};
    while (!stack.empty()) {
        Node* n = stack.back();
        stack.pop_back();
        out.push_back(n);
        const std::vector<Node*> kids = children(*n);
        stack.insert(stack.end(), kids.rbegin(), kids.rend());
    }
    return out;
}

std::vector<Node*> unsupported(Node* root) {
    std::vector<Node*> out;
    for (Node* n : walk(root)) {
        if (is<Unsupported>(n)) out.push_back(n);
    }
    return out;
}

std::vector<Node*> statements(const Suite& suite) {
    if (const auto* one = std::get_if<Node*>(&suite)) return *one != nullptr ? std::vector<Node*>{*one} : std::vector<Node*>{};
    return std::get<std::vector<Node*>>(suite);
}

bool present(const Suite& suite) {
    if (const auto* one = std::get_if<Node*>(&suite)) return *one != nullptr;
    return !std::get<std::vector<Node*>>(suite).empty();
}

#define CB_IR_KINDS(X)                                                                                                         \
    X(Module) X(Unit) X(ClassDecl) X(FieldDecl) X(MethodDecl) X(Param) X(EnumConst) X(Property) X(TypeParam) X(Block) X(Local) \
    X(ExprStmt) X(If) X(While) X(DoWhile) X(For) X(ForEach) X(Switch) X(Case) X(Break) X(Continue) X(Return) X(Throw) X(Try)  \
    X(Catch) X(Labeled) X(Goto) X(MachineFragment) X(Sync) X(Empty) X(Comment) X(LocalClass) X(Assert) X(CtorCall) X(Yield) X(YieldReturn) X(YieldBreak) X(Await) \
    X(Lit) X(Name) X(Select) X(TypeName) X(This) X(Super) X(Call) X(New) X(NewArray) X(Index) X(Length) X(Unary) X(Binary)    \
    X(Concat) X(Assign) X(Cond) X(Cast) X(InstanceOf) X(ClassLit) X(Lambda) X(MethodRef) X(SwitchExpr) X(AddressOf)           \
    X(FarAddress) X(Piece) X(Join) X(Paren) X(Unsupported) X(Region) X(BlockExpr) X(Fixed) X(ThrowExpr) X(OptionalChain)            \
    X(OptionalUnwrap) X(PatternMatch) X(Destructure) X(Default) X(SizeOf) X(StackAlloc) X(Intrinsic)

Node* make_node(Arena& arena, std::string_view name) {
#define CB_IR_MAKE(K) \
    if (name == K::kName) return make(arena, K{});
    CB_IR_KINDS(CB_IR_MAKE)
#undef CB_IR_MAKE
    return nullptr;
}

Node* make_decimal(Arena& arena, std::string text) {
    Lit lit;
    lit.value = text;
    lit.text = std::move(text);
    return make(arena, std::move(lit), decimal_type());
}

Node* clone(Arena& arena, const Node* root) {
    if (root == nullptr) return nullptr;
    Node* copy = arena.make<Node>(*root);
    visit(*copy, [&arena](auto& k) { k.each_child([&arena](Node*& child) { child = clone(arena, child); }); });
    if (auto* test = get<InstanceOf>(copy)) test->binding = clone(arena, test->binding);   // (owned, though a walk does not enter it)
    return copy;
}

// ---- the Java front end's operator names ----------------------------------------------------------------------------------

std::optional<std::string_view> binary_operator(std::string_view java_name) {
    struct Entry {
        string_view name;
        string_view op;
    };
    static constexpr Entry kOperators[] = {
        {"PLUS", "+"},   {"MINUS", "-"},   {"MULTIPLY", "*"},  {"DIVIDE", "/"},           {"REMAINDER", "%"},
        {"LEFT_SHIFT", "<<"}, {"RIGHT_SHIFT", ">>"}, {"UNSIGNED_RIGHT_SHIFT", ">>>"}, {"AND", "&"}, {"OR", "|"},
        {"XOR", "^"},    {"CONDITIONAL_AND", "&&"}, {"CONDITIONAL_OR", "||"}, {"EQUAL_TO", "=="}, {"NOT_EQUAL_TO", "!="},
        {"LESS_THAN", "<"}, {"LESS_THAN_EQUAL", "<="}, {"GREATER_THAN", ">"}, {"GREATER_THAN_EQUAL", ">="},
    };
    for (const Entry& e : kOperators) {
        if (e.name == java_name) return e.op;
    }
    return std::nullopt;
}

std::optional<UnaryOperator> unary_operator(std::string_view java_name) {
    struct Entry {
        string_view name;
        UnaryOperator op;
    };
    static constexpr Entry kOperators[] = {
        {"UNARY_MINUS", {"-", false}},          {"UNARY_PLUS", {"+", false}},           {"LOGICAL_COMPLEMENT", {"!", false}},
        {"BITWISE_COMPLEMENT", {"~", false}},   {"PREFIX_INCREMENT", {"++", false}},    {"PREFIX_DECREMENT", {"--", false}},
        {"POSTFIX_INCREMENT", {"++", true}},    {"POSTFIX_DECREMENT", {"--", true}},
    };
    for (const Entry& e : kOperators) {
        if (e.name == java_name) return e.op;
    }
    return std::nullopt;
}

}  // namespace cb::translate::ir
