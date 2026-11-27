// translate/frontends/cil/cil_names.hpp -- the CIL lane's tables, both ways: ILAst enums and the L2 names and slots they become
// (lower.cpp and lift.cpp read the same table, so a mapping cannot be written one way only). Internal to the lane.
#pragma once

#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>

#include "dotnet/decompiler/il/instructions.hpp"
#include "translate/ir.hpp"

namespace cb::translate::frontends::cil {

namespace il_ns = dotnet::decompiler::il;
namespace ts_ns = dotnet::decompiler::typesystem;
using ir::Node;

// ---- the enums that are the same list on both sides (checked here, so a reordering on either side breaks the build) -------------
static_assert(static_cast<int>(il_ns::StackType::Void) == static_cast<int>(ir::Stack::Void) &&
              static_cast<int>(il_ns::StackType::Ref) == static_cast<int>(ir::Stack::Ref) &&
              static_cast<int>(il_ns::StackType::I) == static_cast<int>(ir::Stack::I));
// ContainerKind and RegionKind list the same kinds in another order.
inline ir::RegionKind region_kind(il_ns::ContainerKind k) {
    using C = il_ns::ContainerKind;
    switch (k) {
        case C::Normal: return ir::RegionKind::Normal;
        case C::Loop: return ir::RegionKind::Loop;
        case C::Switch: return ir::RegionKind::Switch;
        case C::While: return ir::RegionKind::While;
        case C::DoWhile: return ir::RegionKind::DoWhile;
        case C::For: return ir::RegionKind::For;
    }
    return ir::RegionKind::Normal;
}
inline il_ns::ContainerKind container_kind(ir::RegionKind k) {
    using R = ir::RegionKind;
    switch (k) {
        case R::Normal: return il_ns::ContainerKind::Normal;
        case R::Loop: return il_ns::ContainerKind::Loop;
        case R::Switch: return il_ns::ContainerKind::Switch;
        case R::While: return il_ns::ContainerKind::While;
        case R::DoWhile: return il_ns::ContainerKind::DoWhile;
        case R::For: return il_ns::ContainerKind::For;
    }
    return il_ns::ContainerKind::Normal;
}
static_assert(static_cast<int>(il_ns::NopKind::Pop) == static_cast<int>(ir::EmptyKind::Pop));
static_assert(static_cast<int>(il_ns::NullCoalescingKind::NullableWithValueFallback) == static_cast<int>(ir::Coalesce::NullableValue));
static_assert(static_cast<int>(il_ns::ComparisonLiftingKind::ThreeValuedLogic) == static_cast<int>(ir::Lift::ThreeValued) &&
              static_cast<int>(il_ns::ComparisonLiftingKind::CSharp) == static_cast<int>(ir::Lift::CSharp));
static_assert(static_cast<int>(il_ns::CompoundEvalMode::EvaluatesToNewValue) == static_cast<int>(ir::Eval::New));
static_assert(static_cast<int>(il_ns::CompoundTargetKind::Dynamic) == static_cast<int>(ir::Via::Dynamic) &&
              static_cast<int>(il_ns::CompoundTargetKind::Property) == static_cast<int>(ir::Via::Accessor));
static_assert(static_cast<int>(il_ns::ConversionKind::Invalid) == static_cast<int>(ir::ConvKind::Invalid) &&
              static_cast<int>(il_ns::ConversionKind::ObjectInterior) == static_cast<int>(ir::ConvKind::ObjectInterior) &&
              static_cast<int>(il_ns::ConversionKind::StopGCTracking) == static_cast<int>(ir::ConvKind::StopGcTracking));

// ---- signs: None / Signed / Unsigned <-> absent / true / false ---------------------------------------------------------------
inline std::optional<bool> sign_to(ts_ns::Sign sign) {
    if (sign == ts_ns::Sign::None) return std::nullopt;
    return sign == ts_ns::Sign::Signed;
}
inline ts_ns::Sign sign_from(std::optional<bool> s) {
    if (!s) return ts_ns::Sign::None;
    return *s ? ts_ns::Sign::Signed : ts_ns::Sign::Unsigned;
}

// ---- operators ----------------------------------------------------------------------------------------------------------------
// A right shift is `>>>` when the instruction shifts unsigned (the sign is kept in `is_signed` too).
inline std::string_view binary_op_text(il_ns::BinaryNumericOperator op, ts_ns::Sign sign) {
    using O = il_ns::BinaryNumericOperator;
    switch (op) {
        case O::Add: return "+";
        case O::Sub: return "-";
        case O::Mul: return "*";
        case O::Div: return "/";
        case O::Rem: return "%";
        case O::BitAnd: return "&";
        case O::BitOr: return "|";
        case O::BitXor: return "^";
        case O::ShiftLeft: return "<<";
        case O::ShiftRight: return sign == ts_ns::Sign::Unsigned ? ">>>" : ">>";
        case O::None: break;
    }
    throw std::logic_error("cil lowering: a binary instruction without an operator");
}
inline std::optional<il_ns::BinaryNumericOperator> binary_op_from(std::string_view text) {
    using O = il_ns::BinaryNumericOperator;
    if (text == "+") return O::Add;
    if (text == "-") return O::Sub;
    if (text == "*") return O::Mul;
    if (text == "/") return O::Div;
    if (text == "%") return O::Rem;
    if (text == "&") return O::BitAnd;
    if (text == "|") return O::BitOr;
    if (text == "^") return O::BitXor;
    if (text == "<<") return O::ShiftLeft;
    if (text == ">>" || text == ">>>") return O::ShiftRight;
    return std::nullopt;
}
inline std::string_view comparison_text(il_ns::ComparisonKind kind) {
    using K = il_ns::ComparisonKind;
    switch (kind) {
        case K::Equality: return "==";
        case K::Inequality: return "!=";
        case K::LessThan: return "<";
        case K::LessThanOrEqual: return "<=";
        case K::GreaterThan: return ">";
        case K::GreaterThanOrEqual: return ">=";
    }
    return "==";
}
inline std::optional<il_ns::ComparisonKind> comparison_from(std::string_view text) {
    using K = il_ns::ComparisonKind;
    if (text == "==") return K::Equality;
    if (text == "!=") return K::Inequality;
    if (text == "<") return K::LessThan;
    if (text == "<=") return K::LessThanOrEqual;
    if (text == ">") return K::GreaterThan;
    if (text == ">=") return K::GreaterThanOrEqual;
    return std::nullopt;
}

// ---- block kinds <-> BlockExpr roles --------------------------------------------------------------------------------------------
inline constexpr std::string_view kBlockRoles[] = {
    "",   // ControlFlow: no role
    "array_init", "collection_init", "object_init", "stackalloc_init", "call_inline_assign", "named_args", "decon_conv", "decon_assign",
    "with_init", "interpolated",
};
static_assert(std::size(kBlockRoles) == static_cast<std::size_t>(il_ns::BlockKind::InterpolatedString) + 1);
inline std::string_view block_role(il_ns::BlockKind kind) { return kBlockRoles[static_cast<std::size_t>(kind)]; }
inline std::optional<il_ns::BlockKind> block_kind_from(const std::optional<std::string>& role) {
    if (!role) return il_ns::BlockKind::ControlFlow;
    for (std::size_t i = 1; i < std::size(kBlockRoles); ++i) {
        if (kBlockRoles[i] == *role) return static_cast<il_ns::BlockKind>(i);
    }
    return std::nullopt;
}

// ---- variable kinds <-> VarRole (+ cil.kind_exact where two kinds share a role) ---------------------------------------------------
struct VarRoleOf {
    ir::VarRole role;
    std::optional<std::string_view> exact;   // cil.kind_exact
};
inline VarRoleOf var_role(il_ns::VariableKind kind, bool is_this) {
    using K = il_ns::VariableKind;
    using R = ir::VarRole;
    switch (kind) {
        case K::Local: return {R::Local, {}};
        case K::PinnedLocal: return {R::Pinned, {}};
        case K::PinnedRegionLocal: return {R::Pinned, "pinned_region_local"};
        case K::UsingLocal: return {R::Resource, {}};
        case K::ForeachLocal: return {R::LoopVar, {}};
        case K::InitializerTarget: return {R::InitTarget, {}};
        case K::Parameter: return {is_this ? R::This : R::Param, {}};
        case K::ExceptionStackSlot: return {R::CatchVar, "exception_stack_slot"};
        case K::ExceptionLocal: return {R::CatchVar, {}};
        case K::StackSlot: return {R::Temp, {}};
        case K::NamedArgument: return {R::NamedArg, {}};
        case K::DisplayClassLocal: return {R::Captured, {}};
        case K::PatternLocal: return {R::PatternBinding, {}};
        case K::DeconstructionInitTemporary: return {R::DeconTemp, {}};
    }
    return {R::Local, {}};
}
inline il_ns::VariableKind variable_kind(ir::VarRole role, const std::string* exact) {
    using K = il_ns::VariableKind;
    using R = ir::VarRole;
    switch (role) {
        case R::Local: return K::Local;
        case R::Pinned: return exact != nullptr && *exact == "pinned_region_local" ? K::PinnedRegionLocal : K::PinnedLocal;
        case R::Resource: return K::UsingLocal;
        case R::LoopVar: return K::ForeachLocal;
        case R::InitTarget: return K::InitializerTarget;
        case R::Param:
        case R::This: return K::Parameter;
        case R::CatchVar: return exact != nullptr && *exact == "exception_stack_slot" ? K::ExceptionStackSlot : K::ExceptionLocal;
        case R::Temp: return K::StackSlot;
        case R::NamedArg: return K::NamedArgument;
        case R::Captured: return K::DisplayClassLocal;
        case R::PatternBinding: return K::PatternLocal;
        case R::DeconTemp: return K::DeconstructionInitTemporary;
    }
    return K::Local;
}

// ---- a conversion's target (PrimitiveType) <-> the node's type ----------------------------------------------------------------
// I and U are the class types System.IntPtr and System.UIntPtr; R is f64 with cil.target_r; Ref a managed pointer; None (invalid IL) void,
// Unknown any.
inline ir::TypeP conv_target_type(il_ns::PrimitiveType p) {
    using P = il_ns::PrimitiveType;
    static const ir::TypeP kI8 = ir::prim_type(ir::Prim::I8), kI16 = ir::prim_type(ir::Prim::I16), kI64 = ir::prim_type(ir::Prim::I64);
    static const ir::TypeP kU8 = ir::prim_type(ir::Prim::U8), kU16 = ir::prim_type(ir::Prim::U16), kU32 = ir::prim_type(ir::Prim::U32);
    static const ir::TypeP kU64 = ir::prim_type(ir::Prim::U64), kF32 = ir::prim_type(ir::Prim::F32), kF64 = ir::prim_type(ir::Prim::F64);
    static const ir::TypeP kI = ir::class_type("System.IntPtr"), kU = ir::class_type("System.UIntPtr");
    static const ir::TypeP kRef = ir::make_type(ir::PtrType{.to = ir::any_type(), .managed = true});
    switch (p) {
        case P::I1: return kI8;
        case P::I2: return kI16;
        case P::I4: return ir::i32_type();
        case P::I8: return kI64;
        case P::U1: return kU8;
        case P::U2: return kU16;
        case P::U4: return kU32;
        case P::U8: return kU64;
        case P::R4: return kF32;
        case P::R8:
        case P::R: return kF64;
        case P::I: return kI;
        case P::U: return kU;
        case P::Ref: return kRef;
        case P::None: return ir::void_type();   // (a conversion of invalid IL: no target)
        case P::Unknown: break;
    }
    return ir::any_type();
}
inline std::optional<il_ns::PrimitiveType> conv_target_from(const ir::Type& t, bool is_r) {
    using P = il_ns::PrimitiveType;
    if (const auto* p = std::get_if<ir::PrimType>(&t.body)) {
        switch (p->prim) {
            case ir::Prim::I8: return P::I1;
            case ir::Prim::I16: return P::I2;
            case ir::Prim::I32: return P::I4;
            case ir::Prim::I64: return P::I8;
            case ir::Prim::U8: return P::U1;
            case ir::Prim::U16: return P::U2;
            case ir::Prim::U32: return P::U4;
            case ir::Prim::U64: return P::U8;
            case ir::Prim::F32: return P::R4;
            case ir::Prim::F64: return is_r ? P::R : P::R8;
            default: return std::nullopt;
        }
    }
    if (const auto* c = std::get_if<ir::ClassType>(&t.body)) {
        if (c->name == "System.IntPtr") return P::I;
        if (c->name == "System.UIntPtr") return P::U;
    }
    if (const auto* r = std::get_if<ir::PtrType>(&t.body); r != nullptr && r->managed) return P::Ref;
    if (std::holds_alternative<ir::AnyType>(t.body)) return P::Unknown;
    if (std::holds_alternative<ir::VoidType>(t.body)) return P::None;
    return std::nullopt;
}

// ---- literals ---------------------------------------------------------------------------------------------------------------------
inline const ir::TypeP& string_type() {
    static const ir::TypeP kString = ir::class_type("System.String");
    return kString;
}
// A decimal's exact text, scale kept ("1.50"); a negative zero keeps its sign ("-0.0").
std::string decimal_text(const dotnet::decompiler::util::Decimal& d);
// The decimal of such a text; nothing for a text that is not one.
std::optional<dotnet::decompiler::util::Decimal> decimal_from(std::string_view text);

// ---- prefixes: volatile and unaligned. -----------------------------------------------------------------------------------------
inline void set_prefixes(ir::Node& n, bool is_volatile, std::uint8_t unaligned) {
    if (is_volatile) ir::set_flag(n, ir::NodeFlag::Volatile);
    if (unaligned == 1) ir::set_flag(n, ir::NodeFlag::Unaligned1);
    else if (unaligned == 2) ir::set_flag(n, ir::NodeFlag::Unaligned2);
    else if (unaligned == 4) ir::set_flag(n, ir::NodeFlag::Unaligned4);
    else if (unaligned != 0) throw std::logic_error("cil lowering: an unaligned prefix that is not 1, 2 or 4");
}
inline std::uint8_t unaligned_of(const ir::Node& n) {
    if (ir::has_flag(n, ir::NodeFlag::Unaligned1)) return 1;
    if (ir::has_flag(n, ir::NodeFlag::Unaligned2)) return 2;
    if (ir::has_flag(n, ir::NodeFlag::Unaligned4)) return 4;
    return 0;
}

}  // namespace cb::translate::frontends::cil
