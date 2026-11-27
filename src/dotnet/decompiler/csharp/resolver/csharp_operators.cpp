#include "dotnet/decompiler/csharp/resolver/csharp_operators.hpp"

#include <cmath>
#include <cstdint>
#include <functional>
#include <limits>
#include <string>
#include <type_traits>

#include "dotnet/decompiler/csharp/resolver/csharp_resolver.hpp"
#include "dotnet/decompiler/typesystem/specialized_member.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"
#include "dotnet/decompiler/util/decimal.hpp"

namespace cb::dotnet::decompiler::csharp::resolver {

using typesystem::KnownTypeCode;
using typesystem::TypeParameterSubstitution;
using util::Decimal;
namespace nullable_type = typesystem::nullable_type;

using OperatorMethod = CSharpOperators::OperatorMethod;
using UnaryOperatorMethod = CSharpOperators::UnaryOperatorMethod;
using BinaryOperatorMethod = CSharpOperators::BinaryOperatorMethod;

namespace {

int code_value(KnownTypeCode c) { return static_cast<int>(c); }

// ---- the arithmetic of C# on constants ----------------------------------------------------------------------------------------
// Integers wrap (unchecked) or throw OverflowError (checked); shift counts are masked to the operand's width; floating point has no
// overflow.

template <class T>
using Unsigned = std::make_unsigned_t<T>;

template <class T>
T wrap_add(T a, T b) {
    return static_cast<T>(static_cast<Unsigned<T>>(static_cast<Unsigned<T>>(a) + static_cast<Unsigned<T>>(b)));
}
template <class T>
T wrap_sub(T a, T b) {
    return static_cast<T>(static_cast<Unsigned<T>>(static_cast<Unsigned<T>>(a) - static_cast<Unsigned<T>>(b)));
}
template <class T>
T wrap_mul(T a, T b) {
    return static_cast<T>(static_cast<Unsigned<T>>(static_cast<Unsigned<T>>(a) * static_cast<Unsigned<T>>(b)));
}

struct Add {
    template <class T>
    static T unchecked(T a, T b) {
        if constexpr (std::is_integral_v<T>) {
            return wrap_add(a, b);
        } else if constexpr (std::is_same_v<T, Decimal>) {
            return Decimal::add(a, b);
        } else {
            return a + b;
        }
    }
    template <class T>
    static T checked(T a, T b) {
        if constexpr (std::is_integral_v<T>) {
            const T r = wrap_add(a, b);
            if constexpr (std::is_signed_v<T>) {
                if (((a ^ r) & (b ^ r)) < 0) throw OverflowError();
            } else {
                if (r < a) throw OverflowError();
            }
            return r;
        } else if constexpr (std::is_same_v<T, Decimal>) {
            return Decimal::add(a, b);
        } else {
            return a + b;
        }
    }
};

struct Sub {
    template <class T>
    static T unchecked(T a, T b) {
        if constexpr (std::is_integral_v<T>) {
            return wrap_sub(a, b);
        } else if constexpr (std::is_same_v<T, Decimal>) {
            return Decimal::subtract(a, b);
        } else {
            return a - b;
        }
    }
    template <class T>
    static T checked(T a, T b) {
        if constexpr (std::is_integral_v<T>) {
            const T r = wrap_sub(a, b);
            if constexpr (std::is_signed_v<T>) {
                if (((a ^ b) & (a ^ r)) < 0) throw OverflowError();
            } else {
                if (a < b) throw OverflowError();
            }
            return r;
        } else if constexpr (std::is_same_v<T, Decimal>) {
            return Decimal::subtract(a, b);
        } else {
            return a - b;
        }
    }
};

struct Mul {
    template <class T>
    static T unchecked(T a, T b) {
        if constexpr (std::is_integral_v<T>) {
            return wrap_mul(a, b);
        } else if constexpr (std::is_same_v<T, Decimal>) {
            return Decimal::multiply(a, b);
        } else {
            return a * b;
        }
    }
    template <class T>
    static T checked(T a, T b) {
        if constexpr (std::is_integral_v<T>) {
            if (a == 0 || b == 0) return 0;
            if constexpr (std::is_signed_v<T>) {
                constexpr T min = std::numeric_limits<T>::min();
                if ((a == -1 && b == min) || (b == -1 && a == min)) throw OverflowError();
            }
            const T r = wrap_mul(a, b);
            if (r / b != a) throw OverflowError();
            return r;
        } else if constexpr (std::is_same_v<T, Decimal>) {
            return Decimal::multiply(a, b);
        } else {
            return a * b;
        }
    }
};

struct Div {
    template <class T>
    static T apply(T a, T b) {
        if constexpr (std::is_integral_v<T>) {
            if (b == 0) throw DivideByZeroError();
            if constexpr (std::is_signed_v<T>) {
                if (a == std::numeric_limits<T>::min() && b == -1) throw OverflowError();
            }
            return a / b;
        } else if constexpr (std::is_same_v<T, Decimal>) {
            return Decimal::divide(a, b);
        } else {
            return a / b;
        }
    }
};

struct Rem {
    template <class T>
    static T apply(T a, T b) {
        if constexpr (std::is_integral_v<T>) {
            if (b == 0) throw DivideByZeroError();
            if constexpr (std::is_signed_v<T>) {
                if (a == std::numeric_limits<T>::min() && b == -1) throw OverflowError();
            }
            return a % b;
        } else if constexpr (std::is_same_v<T, Decimal>) {
            return Decimal::remainder(a, b);
        } else {
            return std::fmod(a, b);
        }
    }
};

template <class T>
T shift_left(T a, std::int32_t b) {
    constexpr int mask = static_cast<int>(sizeof(T) * 8) - 1;
    return static_cast<T>(static_cast<Unsigned<T>>(static_cast<Unsigned<T>>(a) << (b & mask)));
}
template <class T>
T shift_right(T a, std::int32_t b) {
    constexpr int mask = static_cast<int>(sizeof(T) * 8) - 1;
    return static_cast<T>(a >> (b & mask));
}
template <class T>
T unsigned_shift_right(T a, std::int32_t b) {
    constexpr int mask = static_cast<int>(sizeof(T) * 8) - 1;
    return static_cast<T>(static_cast<Unsigned<T>>(a) >> (b & mask));
}

template <class T>
T negate_unchecked(T a) {
    if constexpr (std::is_integral_v<T>) {
        return static_cast<T>(static_cast<Unsigned<T>>(0) - static_cast<Unsigned<T>>(a));
    } else {
        return -a;
    }
}
template <class T>
T negate_checked(T a) {
    if constexpr (std::is_integral_v<T>) {
        if (a == std::numeric_limits<T>::min()) throw OverflowError();
        return static_cast<T>(-a);
    } else {
        return -a;
    }
}

// ---- decimal ------------------------------------------------------------------------------------------------------------------

// decimal.Equals / ==: by value (1.0m equals 1.00m), as Decimal.Compare says.
bool decimal_equals(const Decimal& a, const Decimal& b) { return Decimal::compare(a, b) == 0; }

// ---- constants as the operators take them -------------------------------------------------------------------------------------

template <class T>
struct TypeCodeOf;
template <>
struct TypeCodeOf<std::int32_t> { static constexpr KnownTypeCode value = KnownTypeCode::Int32; };
template <>
struct TypeCodeOf<std::uint32_t> { static constexpr KnownTypeCode value = KnownTypeCode::UInt32; };
template <>
struct TypeCodeOf<std::int64_t> { static constexpr KnownTypeCode value = KnownTypeCode::Int64; };
template <>
struct TypeCodeOf<std::uint64_t> { static constexpr KnownTypeCode value = KnownTypeCode::UInt64; };
template <>
struct TypeCodeOf<float> { static constexpr KnownTypeCode value = KnownTypeCode::Single; };
template <>
struct TypeCodeOf<double> { static constexpr KnownTypeCode value = KnownTypeCode::Double; };
template <>
struct TypeCodeOf<Decimal> { static constexpr KnownTypeCode value = KnownTypeCode::Decimal; };
template <>
struct TypeCodeOf<bool> { static constexpr KnownTypeCode value = KnownTypeCode::Boolean; };

bool is_null(const ConstantValue& value) { return std::holds_alternative<std::monostate>(value); }

// (T)resolver.CSharpPrimitiveCast(code of T, value)
template <class T>
T cast_to(const CSharpResolver& resolver, const ConstantValue& value) {
    return std::get<T>(resolver.csharp_primitive_cast(TypeCodeOf<T>::value, value));
}

template <class T>
ConstantValue to_constant(T value) {
    return ConstantValue(std::in_place_type<T>, std::move(value));
}

// ---- unary operator classes ---------------------------------------------------------------------------------------------------

class LiftedUnaryOperatorMethod final : public UnaryOperatorMethod, public ILiftedOperator {
public:
    LiftedUnaryOperatorMethod(CSharpOperators& operators, const UnaryOperatorMethod* base_method) : UnaryOperatorMethod(operators.compilation()), base_method_(base_method) {
        return_type_value = nullable_type::create(base_method->compilation(), base_method->return_type());
        parameters_value.push_back(operators.make_nullable_parameter(base_method->parameters()[0]));
    }

    const std::vector<const IParameter*>& non_lifted_parameters() const override { return base_method_->parameters(); }
    const IType* non_lifted_return_type() const override { return base_method_->return_type(); }

private:
    const UnaryOperatorMethod* base_method_;
};

template <class T>
class LambdaUnaryOperatorMethod final : public UnaryOperatorMethod {
public:
    LambdaUnaryOperatorMethod(CSharpOperators& operators, std::function<T(T)> func, bool evaluable = true)
        : UnaryOperatorMethod(operators.compilation()), func_(std::move(func)), evaluable_(evaluable) {
        return_type_value = operators.compilation().find_type(TypeCodeOf<T>::value);
        parameters_value.push_back(operators.make_parameter(TypeCodeOf<T>::value));
    }

    bool can_evaluate_at_compile_time() const override { return evaluable_; }

    ConstantValue invoke(const CSharpResolver& resolver, const ConstantValue& input) const override {
        if (is_null(input)) return ConstantValue();
        return to_constant<T>(func_(cast_to<T>(resolver, input)));
    }

    std::unique_ptr<OperatorMethod> lift(CSharpOperators& operators) const override { return std::make_unique<LiftedUnaryOperatorMethod>(operators, this); }

private:
    std::function<T(T)> func_;
    bool evaluable_;
};

// ---- binary operator classes --------------------------------------------------------------------------------------------------

class LiftedBinaryOperatorMethod final : public BinaryOperatorMethod, public ILiftedOperator {
public:
    LiftedBinaryOperatorMethod(CSharpOperators& operators, const BinaryOperatorMethod* base_method)
        : BinaryOperatorMethod(operators.compilation()), base_method_(base_method) {
        return_type_value = nullable_type::create(operators.compilation(), base_method->return_type());
        parameters_value.push_back(operators.make_nullable_parameter(base_method->parameters()[0]));
        parameters_value.push_back(operators.make_nullable_parameter(base_method->parameters()[1]));
    }

    const std::vector<const IParameter*>& non_lifted_parameters() const override { return base_method_->parameters(); }
    const IType* non_lifted_return_type() const override { return base_method_->return_type(); }

private:
    const BinaryOperatorMethod* base_method_;
};

// A binary operator whose operands are cast to T1 and T2 and whose result is a T1.
template <class T1, class T2>
class LambdaBinaryOperatorMethod final : public BinaryOperatorMethod {
public:
    LambdaBinaryOperatorMethod(CSharpOperators& operators, std::function<T1(T1, T2)> func, bool evaluable = true)
        : LambdaBinaryOperatorMethod(operators, func, func, evaluable) {}

    LambdaBinaryOperatorMethod(CSharpOperators& operators, std::function<T1(T1, T2)> checked_func, std::function<T1(T1, T2)> unchecked_func, bool evaluable = true)
        : BinaryOperatorMethod(operators.compilation()), checked_func_(std::move(checked_func)), unchecked_func_(std::move(unchecked_func)), evaluable_(evaluable) {
        return_type_value = operators.compilation().find_type(TypeCodeOf<T1>::value);
        parameters_value.push_back(operators.make_parameter(TypeCodeOf<T1>::value));
        parameters_value.push_back(operators.make_parameter(TypeCodeOf<T2>::value));
    }

    bool can_evaluate_at_compile_time() const override { return evaluable_; }

    ConstantValue invoke(const CSharpResolver& resolver, const ConstantValue& lhs, const ConstantValue& rhs) const override {
        if (is_null(lhs) || is_null(rhs)) return ConstantValue();
        const std::function<T1(T1, T2)>& func = resolver.check_for_overflow() ? checked_func_ : unchecked_func_;
        const T1 left = cast_to<T1>(resolver, lhs);
        const T2 right = cast_to<T2>(resolver, rhs);
        return to_constant<T1>(func(left, right));
    }

    std::unique_ptr<OperatorMethod> lift(CSharpOperators& operators) const override { return std::make_unique<LiftedBinaryOperatorMethod>(operators, this); }

private:
    std::function<T1(T1, T2)> checked_func_;
    std::function<T1(T1, T2)> unchecked_func_;
    bool evaluable_;
};

// "string + string", "string + object", "object + string"
class StringConcatenation final : public BinaryOperatorMethod {
public:
    StringConcatenation(CSharpOperators& operators, KnownTypeCode p1, KnownTypeCode p2) : BinaryOperatorMethod(operators.compilation()) {
        can_evaluate_at_compile_time_ = p1 == KnownTypeCode::String && p2 == KnownTypeCode::String;
        return_type_value = operators.compilation().find_type(KnownTypeCode::String);
        parameters_value.push_back(operators.make_parameter(p1));
        parameters_value.push_back(operators.make_parameter(p2));
    }

    bool can_evaluate_at_compile_time() const override { return can_evaluate_at_compile_time_; }

    ConstantValue invoke(const CSharpResolver&, const ConstantValue& lhs, const ConstantValue& rhs) const override {
        return to_constant<std::u16string>(as_text(lhs) + as_text(rhs));
    }

private:
    // string.Concat of an object: null is empty; only constants of type string get here.
    static std::u16string as_text(const ConstantValue& value) {
        if (is_null(value)) return {};
        if (const auto* text = std::get_if<std::u16string>(&value)) return *text;
        throw std::logic_error("string concatenation of a constant that is not a string");
    }

    bool can_evaluate_at_compile_time_ = false;
};

class LiftedEqualityOperatorMethod;

// == and != on one operand type (Object and String compare references/values).
class EqualityOperatorMethod final : public BinaryOperatorMethod {
public:
    EqualityOperatorMethod(CSharpOperators& operators, KnownTypeCode type_code, bool negate_)
        : BinaryOperatorMethod(operators.compilation()), type(type_code), negate(negate_) {
        return_type_value = operators.compilation().find_type(KnownTypeCode::Boolean);
        parameters_value.push_back(operators.make_parameter(type_code));
        parameters_value.push_back(operators.make_parameter(type_code));
    }

    const KnownTypeCode type;
    const bool negate;

    bool can_evaluate_at_compile_time() const override { return type != KnownTypeCode::Object; }

    ConstantValue invoke(const CSharpResolver& resolver, const ConstantValue& lhs_value, const ConstantValue& rhs_value) const override {
        if (is_null(lhs_value) && is_null(rhs_value)) return to_constant<bool>(!negate);   // ==: true; !=: false
        if (is_null(lhs_value) || is_null(rhs_value)) return to_constant<bool>(negate);    // ==: false; !=: true
        const ConstantValue lhs = resolver.csharp_primitive_cast(type, lhs_value);
        const ConstantValue rhs = resolver.csharp_primitive_cast(type, rhs_value);
        bool equal;
        if (const auto* l = std::get_if<Decimal>(&lhs); l != nullptr) {
            const auto* r = std::get_if<Decimal>(&rhs);
            equal = r != nullptr && decimal_equals(*l, *r);
        } else {
            // float and double compare as IEEE numbers (NaN is not equal to NaN), the others by value
            equal = lhs == rhs;
        }
        return to_constant<bool>(equal != negate);
    }

    std::unique_ptr<OperatorMethod> lift(CSharpOperators& operators) const override;
};

class LiftedEqualityOperatorMethod final : public BinaryOperatorMethod, public ILiftedOperator {
public:
    LiftedEqualityOperatorMethod(CSharpOperators& operators, const EqualityOperatorMethod* base_method)
        : BinaryOperatorMethod(operators.compilation()), base_method_(base_method) {
        return_type_value = base_method->return_type();
        const IParameter* p = operators.make_nullable_parameter(base_method->parameters()[0]);
        parameters_value.push_back(p);
        parameters_value.push_back(p);
    }

    bool can_evaluate_at_compile_time() const override { return base_method_->can_evaluate_at_compile_time(); }

    ConstantValue invoke(const CSharpResolver& resolver, const ConstantValue& lhs, const ConstantValue& rhs) const override {
        return base_method_->invoke(resolver, lhs, rhs);
    }

    const std::vector<const IParameter*>& non_lifted_parameters() const override { return base_method_->parameters(); }
    const IType* non_lifted_return_type() const override { return base_method_->return_type(); }

private:
    const EqualityOperatorMethod* base_method_;
};

std::unique_ptr<OperatorMethod> EqualityOperatorMethod::lift(CSharpOperators& operators) const {
    if (type == KnownTypeCode::Object || type == KnownTypeCode::String) return nullptr;
    return std::make_unique<LiftedEqualityOperatorMethod>(operators, this);
}

// <, <=, >, >= on T1 and T2
template <class T1, class T2>
class RelationalOperatorMethod final : public BinaryOperatorMethod {
public:
    RelationalOperatorMethod(CSharpOperators& operators, std::function<bool(T1, T2)> func, bool evaluable = true)
        : BinaryOperatorMethod(operators.compilation()), func_(std::move(func)), evaluable_(evaluable) {
        return_type_value = operators.compilation().find_type(KnownTypeCode::Boolean);
        parameters_value.push_back(operators.make_parameter(TypeCodeOf<T1>::value));
        parameters_value.push_back(operators.make_parameter(TypeCodeOf<T2>::value));
    }

    bool can_evaluate_at_compile_time() const override { return evaluable_; }

    ConstantValue invoke(const CSharpResolver& resolver, const ConstantValue& lhs, const ConstantValue& rhs) const override {
        if (is_null(lhs) || is_null(rhs)) return ConstantValue();
        const T1 left = cast_to<T1>(resolver, lhs);
        const T2 right = cast_to<T2>(resolver, rhs);
        return to_constant<bool>(func_(left, right));
    }

    std::unique_ptr<OperatorMethod> lift(CSharpOperators& operators) const override {
        auto lifted = std::make_unique<LiftedBinaryOperatorMethod>(operators, this);
        lifted->return_type_value = return_type_value;   // don't lift the return type for relational operators
        return lifted;
    }

private:
    std::function<bool(T1, T2)> func_;
    bool evaluable_;
};

// ---- making the operators of a list -------------------------------------------------------------------------------------------

template <class T>
OperatorMethod* unary(CSharpOperators& o, std::function<T(T)> f) {
    return o.own(std::make_unique<LambdaUnaryOperatorMethod<T>>(o, std::move(f)));
}

// A unary operator on decimal.
OperatorMethod* decimal_unary(CSharpOperators& o, std::function<Decimal(Decimal)> f) {
    return o.own(std::make_unique<LambdaUnaryOperatorMethod<Decimal>>(o, std::move(f), true));
}

template <class T1, class T2 = T1>
OperatorMethod* binary(CSharpOperators& o, std::type_identity_t<std::function<T1(T1, T2)>> checked_func,
                       std::type_identity_t<std::function<T1(T1, T2)>> unchecked_func) {
    return o.own(std::make_unique<LambdaBinaryOperatorMethod<T1, T2>>(o, std::move(checked_func), std::move(unchecked_func)));
}

template <class T1, class T2 = T1>
OperatorMethod* binary(CSharpOperators& o, std::type_identity_t<std::function<T1(T1, T2)>> f) {
    return o.own(std::make_unique<LambdaBinaryOperatorMethod<T1, T2>>(o, std::move(f)));
}

template <class T>
OperatorMethod* relational(CSharpOperators& o, std::function<bool(T, T)> f) {
    return o.own(std::make_unique<RelationalOperatorMethod<T, T>>(o, std::move(f)));
}

// The seven arithmetic operand types of an operator (int, uint, long, ulong, float, double, decimal) for a binary operator struct
// with checked and unchecked forms.
template <class Op>
std::vector<OperatorMethod*> arithmetic_with_overflow_check(CSharpOperators& o) {
    return {
        binary<std::int32_t>(o, &Op::template checked<std::int32_t>, &Op::template unchecked<std::int32_t>),
        binary<std::uint32_t>(o, &Op::template checked<std::uint32_t>, &Op::template unchecked<std::uint32_t>),
        binary<std::int64_t>(o, &Op::template checked<std::int64_t>, &Op::template unchecked<std::int64_t>),
        binary<std::uint64_t>(o, &Op::template checked<std::uint64_t>, &Op::template unchecked<std::uint64_t>),
        binary<float>(o, &Op::template checked<float>, &Op::template unchecked<float>),
        binary<double>(o, &Op::template checked<double>, &Op::template unchecked<double>),
        binary<Decimal>(o, &Op::template checked<Decimal>, &Op::template unchecked<Decimal>),
    };
}

// The same for an operator with one form (division, remainder).
template <class Op>
std::vector<OperatorMethod*> arithmetic(CSharpOperators& o) {
    return {
        binary<std::int32_t>(o, &Op::template apply<std::int32_t>, &Op::template apply<std::int32_t>),
        binary<std::uint32_t>(o, &Op::template apply<std::uint32_t>, &Op::template apply<std::uint32_t>),
        binary<std::int64_t>(o, &Op::template apply<std::int64_t>, &Op::template apply<std::int64_t>),
        binary<std::uint64_t>(o, &Op::template apply<std::uint64_t>, &Op::template apply<std::uint64_t>),
        binary<float>(o, &Op::template apply<float>, &Op::template apply<float>),
        binary<double>(o, &Op::template apply<double>, &Op::template apply<double>),
        binary<Decimal>(o, &Op::template apply<Decimal>, &Op::template apply<Decimal>),
    };
}

// The shifts of int, uint, long and ulong by an int.
std::vector<OperatorMethod*> shifts(CSharpOperators& o, std::int32_t (*i)(std::int32_t, std::int32_t), std::uint32_t (*u)(std::uint32_t, std::int32_t),
                                    std::int64_t (*l)(std::int64_t, std::int32_t), std::uint64_t (*ul)(std::uint64_t, std::int32_t)) {
    return {
        binary<std::int32_t, std::int32_t>(o, i),
        binary<std::uint32_t, std::int32_t>(o, u),
        binary<std::int64_t, std::int32_t>(o, l),
        binary<std::uint64_t, std::int32_t>(o, ul),
    };
}

template <template <class> class Cmp>
std::vector<OperatorMethod*> relationals(CSharpOperators& o) {
    return {
        relational<std::int32_t>(o, Cmp<std::int32_t>()),
        relational<std::uint32_t>(o, Cmp<std::uint32_t>()),
        relational<std::int64_t>(o, Cmp<std::int64_t>()),
        relational<std::uint64_t>(o, Cmp<std::uint64_t>()),
        relational<float>(o, Cmp<float>()),
        relational<double>(o, Cmp<double>()),
        relational<Decimal>(o, [](Decimal a, Decimal b) { return Cmp<int>()(Decimal::compare(a, b), 0); }),
    };
}

// ---- user-defined operators, lifted -------------------------------------------------------------------------------------------

// The lifted version of a user-defined operator: the operator method with every operand and the result made nullable (a comparison
// operator keeps its bool result).
class LiftedUserDefinedOperator final : public virtual IMethod, public virtual ILiftedOperator {
public:
    // `definition` is the non-lifted method's definition (found by the caller: a dynamic_cast to IMethod is not safe in a constructor).
    LiftedUserDefinedOperator(const IMethod* non_lifted_method, const IMethod* definition) : non_lifted_operator_(non_lifted_method) {
        const ICompilation& compilation = non_lifted_method->compilation();
        TypeParameterSubstitution substitution = non_lifted_method->substitution();
        // (upstream makes the parameters from the definition's, substituted)
        for (const IParameter* p : definition->parameters()) {
            const IType* new_type = nullable_type::create(compilation, p->type()->accept_visitor(substitution));
            parameters_.push_back(std::make_unique<typesystem::SpecializedParameter>(p, new_type, this));
        }
        for (const auto& p : parameters_) parameter_pointers_.push_back(p.get());
        // Comparison operators keep the 'bool' return type even when lifted.
        if (CSharpOperators::is_comparison_operator(non_lifted_method))
            return_type_ = non_lifted_method->return_type();
        else
            return_type_ = nullable_type::create(compilation, non_lifted_method->return_type()->accept_visitor(substitution));
    }

    const std::vector<const IParameter*>& non_lifted_parameters() const override { return non_lifted_operator_->parameters(); }
    const IType* non_lifted_return_type() const override { return non_lifted_operator_->return_type(); }
    const IMethod* non_lifted_operator() const { return non_lifted_operator_; }

    // what is lifted: the parameters and the return type
    const std::vector<const IParameter*>& parameters() const override { return parameter_pointers_; }
    const IType* return_type() const override { return return_type_; }

    // everything else is the non-lifted operator's
    std::string full_name() const override { return non_lifted_operator_->full_name(); }
    std::string name() const override { return non_lifted_operator_->name(); }
    std::string reflection_name() const override { return non_lifted_operator_->reflection_name(); }
    std::string namespace_() const override { return non_lifted_operator_->namespace_(); }
    typesystem::SymbolKind symbol_kind() const override { return non_lifted_operator_->symbol_kind(); }
    const ICompilation& compilation() const override { return non_lifted_operator_->compilation(); }
    typesystem::srm::EntityHandle metadata_token() const override { return non_lifted_operator_->metadata_token(); }
    const typesystem::ITypeDefinition* declaring_type_definition() const override { return non_lifted_operator_->declaring_type_definition(); }
    const IType* declaring_type() const override { return non_lifted_operator_->declaring_type(); }
    const typesystem::IModule* parent_module() const override { return non_lifted_operator_->parent_module(); }
    std::vector<const typesystem::IAttribute*> get_attributes() const override { return non_lifted_operator_->get_attributes(); }
    bool has_attribute(typesystem::KnownAttribute attribute) const override { return non_lifted_operator_->has_attribute(attribute); }
    const typesystem::IAttribute* get_attribute(typesystem::KnownAttribute attribute) const override { return non_lifted_operator_->get_attribute(attribute); }
    typesystem::Accessibility accessibility() const override { return non_lifted_operator_->accessibility(); }
    bool is_static() const override { return non_lifted_operator_->is_static(); }
    bool is_abstract() const override { return non_lifted_operator_->is_abstract(); }
    bool is_sealed() const override { return non_lifted_operator_->is_sealed(); }
    const IMember* member_definition() const override { return non_lifted_operator_->member_definition(); }
    std::vector<const IMember*> explicitly_implemented_interface_members() const override { return non_lifted_operator_->explicitly_implemented_interface_members(); }
    bool is_explicit_interface_implementation() const override { return non_lifted_operator_->is_explicit_interface_implementation(); }
    bool is_virtual() const override { return non_lifted_operator_->is_virtual(); }
    bool is_override() const override { return non_lifted_operator_->is_override(); }
    bool is_overridable() const override { return non_lifted_operator_->is_overridable(); }
    const TypeParameterSubstitution& substitution() const override { return non_lifted_operator_->substitution(); }
    const IMember* specialize(const TypeParameterSubstitution& new_substitution) const override { return non_lifted_operator_->specialize(new_substitution); }
    std::vector<const typesystem::IAttribute*> get_return_type_attributes() const override { return non_lifted_operator_->get_return_type_attributes(); }
    bool return_type_is_ref_read_only() const override { return non_lifted_operator_->return_type_is_ref_read_only(); }
    bool is_init_only() const override { return non_lifted_operator_->is_init_only(); }
    bool this_is_ref_read_only() const override { return non_lifted_operator_->this_is_ref_read_only(); }
    const std::vector<const typesystem::ITypeParameter*>& type_parameters() const override { return non_lifted_operator_->type_parameters(); }
    const std::vector<const IType*>& type_arguments() const override { return non_lifted_operator_->type_arguments(); }
    bool is_extension_method() const override { return non_lifted_operator_->is_extension_method(); }
    bool is_local_function() const override { return non_lifted_operator_->is_local_function(); }
    bool is_constructor() const override { return non_lifted_operator_->is_constructor(); }
    bool is_destructor() const override { return non_lifted_operator_->is_destructor(); }
    bool is_operator() const override { return non_lifted_operator_->is_operator(); }
    bool has_body() const override { return non_lifted_operator_->has_body(); }
    bool is_accessor() const override { return non_lifted_operator_->is_accessor(); }
    const IMember* accessor_owner() const override { return non_lifted_operator_->accessor_owner(); }
    typesystem::MethodSemanticsAttributes accessor_kind() const override { return non_lifted_operator_->accessor_kind(); }
    const IMethod* reduced_from() const override { return non_lifted_operator_->reduced_from(); }
    const IMethod* specialize_method(const TypeParameterSubstitution& new_substitution) const override {
        return non_lifted_operator_->specialize_method(new_substitution);
    }
    std::string to_string() const override { return "[LiftedUserDefinedOperator " + non_lifted_operator_->to_string() + "]"; }

    bool equals(const IMember* other, typesystem::TypeVisitor& type_normalization) const override {
        const auto* op = dynamic_cast<const LiftedUserDefinedOperator*>(other);
        return op != nullptr && non_lifted_operator_->equals(op->non_lifted_operator_, type_normalization);
    }
    bool equals(const IMember* other) const override {
        const auto* op = dynamic_cast<const LiftedUserDefinedOperator*>(other);
        return op != nullptr && non_lifted_operator_->equals(op->non_lifted_operator_);
    }
    int hash_code() const override { return non_lifted_operator_->hash_code() ^ 0x7191254; }

private:
    const IMethod* non_lifted_operator_;
    std::vector<std::unique_ptr<typesystem::SpecializedParameter>> parameters_;
    std::vector<const IParameter*> parameter_pointers_;
    const IType* return_type_ = nullptr;
};

}  // namespace

// ---- OperatorMethod -----------------------------------------------------------------------------------------------------------

CSharpOperators::OperatorMethod::OperatorMethod(const ICompilation& compilation) : FakeMember(compilation) {
    name_value = "operator";
    declaring_type_value = &typesystem::SpecialType::unknown_type();
    is_static_value = true;
    accessibility_value = typesystem::Accessibility::Public;
}

const typesystem::IModule* CSharpOperators::OperatorMethod::parent_module() const { return &compilation().main_module(); }

const typesystem::IMember* CSharpOperators::OperatorMethod::specialize(const TypeParameterSubstitution& substitution) const {
    if (TypeParameterSubstitution::identity().equals(substitution)) return this;
    throw std::logic_error("OperatorMethod::specialize: not supported");   // NotSupportedException
}

std::string CSharpOperators::OperatorMethod::to_string() const {
    std::string b = return_type_value->to_string() + " operator(";
    for (std::size_t i = 0; i < parameters_value.size(); i++) {
        if (i > 0) b += ", ";
        b += parameters_value[i]->type()->to_string();
    }
    b += ')';
    return b;
}

std::unique_ptr<CSharpOperators::OperatorMethod> CSharpOperators::OperatorMethod::lift(CSharpOperators&) const { return nullptr; }

ConstantValue CSharpOperators::UnaryOperatorMethod::invoke(const CSharpResolver&, const ConstantValue&) const {
    throw std::logic_error("UnaryOperatorMethod::invoke: not supported");   // NotSupportedException
}

ConstantValue CSharpOperators::BinaryOperatorMethod::invoke(const CSharpResolver&, const ConstantValue&, const ConstantValue&) const {
    throw std::logic_error("BinaryOperatorMethod::invoke: not supported");   // NotSupportedException
}

// ---- construction -------------------------------------------------------------------------------------------------------------

CSharpOperators::CSharpOperators(const ICompilation& compilation) : compilation_(compilation) {
    // InitParameterArrays
    for (int i = code_value(KnownTypeCode::Object); i <= code_value(KnownTypeCode::String); i++) {
        owned_parameters_.push_back(std::make_unique<typesystem::DefaultParameter>(compilation.find_type(static_cast<KnownTypeCode>(i)), std::string()));
        normal_parameters_[static_cast<std::size_t>(i - code_value(KnownTypeCode::Object))] = owned_parameters_.back().get();
    }
    for (int i = code_value(KnownTypeCode::Boolean); i <= code_value(KnownTypeCode::Decimal); i++) {
        const IType* type = nullable_type::create(compilation, compilation.find_type(static_cast<KnownTypeCode>(i)));
        owned_parameters_.push_back(std::make_unique<typesystem::DefaultParameter>(type, std::string()));
        nullable_parameters_[static_cast<std::size_t>(i - code_value(KnownTypeCode::Boolean))] = owned_parameters_.back().get();
    }
}

CSharpOperators& CSharpOperators::get(const ICompilation& compilation) {
    // 201 is this class's key among the objects a compilation's arena keeps (200 is CSharpConversions, 202 LiftedUserDefinedOperator).
    typesystem::TypeKey key{201, {&compilation}, {}, {}};
    const CSharpOperators* operators = compilation.arena().intern<CSharpOperators>(
        std::move(key), [&] { return std::unique_ptr<CSharpOperators>(new CSharpOperators(compilation)); });
    return const_cast<CSharpOperators&>(*operators);
}

const IParameter* CSharpOperators::make_parameter(KnownTypeCode code) const {
    return normal_parameters_[static_cast<std::size_t>(code_value(code) - code_value(KnownTypeCode::Object))];
}

const IParameter* CSharpOperators::make_nullable_parameter(const IParameter* normal_parameter) const {
    for (int i = code_value(KnownTypeCode::Boolean); i <= code_value(KnownTypeCode::Decimal); i++) {
        if (normal_parameter == normal_parameters_[static_cast<std::size_t>(i - code_value(KnownTypeCode::Object))])
            return nullable_parameters_[static_cast<std::size_t>(i - code_value(KnownTypeCode::Boolean))];
    }
    throw std::invalid_argument("normal_parameter");
}

CSharpOperators::OperatorMethod* CSharpOperators::own(std::unique_ptr<OperatorMethod> method) {
    const std::lock_guard lock(mutex_);
    owned_methods_.push_back(std::move(method));
    return owned_methods_.back().get();
}

std::vector<OperatorMethod*> CSharpOperators::lift(std::vector<OperatorMethod*> methods) {
    std::vector<OperatorMethod*> result = methods;
    for (OperatorMethod* method : methods) {
        std::unique_ptr<OperatorMethod> lifted = method->lift(*this);
        if (lifted != nullptr) result.push_back(own(std::move(lifted)));
    }
    return result;
}

template <class Make>
const std::vector<OperatorMethod*>& CSharpOperators::lazy(std::optional<std::vector<OperatorMethod*>>& slot, Make&& make) {
    const std::lock_guard lock(mutex_);
    if (!slot.has_value()) slot = make();
    return *slot;
}

// ---- unary operator definitions -----------------------------------------------------------------------------------------------

const std::vector<OperatorMethod*>& CSharpOperators::unary_plus_operators() {
    return lazy(unary_plus_operators_, [&] {
        return lift({
            unary<std::int32_t>(*this, [](std::int32_t i) { return i; }),
            unary<std::uint32_t>(*this, [](std::uint32_t i) { return i; }),
            unary<std::int64_t>(*this, [](std::int64_t i) { return i; }),
            unary<std::uint64_t>(*this, [](std::uint64_t i) { return i; }),
            unary<float>(*this, [](float i) { return i; }),
            unary<double>(*this, [](double i) { return i; }),
            decimal_unary(*this, [](Decimal i) { return i; }),
        });
    });
}

const std::vector<OperatorMethod*>& CSharpOperators::unchecked_unary_minus_operators() {
    return lazy(unchecked_unary_minus_operators_, [&] {
        return lift({
            unary<std::int32_t>(*this, [](std::int32_t i) { return negate_unchecked(i); }),
            unary<std::int64_t>(*this, [](std::int64_t i) { return negate_unchecked(i); }),
            unary<float>(*this, [](float i) { return negate_unchecked(i); }),
            unary<double>(*this, [](double i) { return negate_unchecked(i); }),
            decimal_unary(*this, [](Decimal i) { return Decimal::negate(i); }),
        });
    });
}

const std::vector<OperatorMethod*>& CSharpOperators::checked_unary_minus_operators() {
    return lazy(checked_unary_minus_operators_, [&] {
        return lift({
            unary<std::int32_t>(*this, [](std::int32_t i) { return negate_checked(i); }),
            unary<std::int64_t>(*this, [](std::int64_t i) { return negate_checked(i); }),
            unary<float>(*this, [](float i) { return negate_checked(i); }),
            unary<double>(*this, [](double i) { return negate_checked(i); }),
            decimal_unary(*this, [](Decimal i) { return Decimal::negate(i); }),
        });
    });
}

const std::vector<OperatorMethod*>& CSharpOperators::logical_negation_operators() {
    return lazy(logical_negation_operators_, [&] { return lift({unary<bool>(*this, [](bool b) { return !b; })}); });
}

const std::vector<OperatorMethod*>& CSharpOperators::bitwise_complement_operators() {
    return lazy(bitwise_complement_operators_, [&] {
        return lift({
            unary<std::int32_t>(*this, [](std::int32_t i) { return static_cast<std::int32_t>(~i); }),
            unary<std::uint32_t>(*this, [](std::uint32_t i) { return static_cast<std::uint32_t>(~i); }),
            unary<std::int64_t>(*this, [](std::int64_t i) { return static_cast<std::int64_t>(~i); }),
            unary<std::uint64_t>(*this, [](std::uint64_t i) { return static_cast<std::uint64_t>(~i); }),
        });
    });
}

// ---- arithmetic operators -----------------------------------------------------------------------------------------------------

const std::vector<OperatorMethod*>& CSharpOperators::multiplication_operators() {
    return lazy(multiplication_operators_, [&] { return lift(arithmetic_with_overflow_check<Mul>(*this)); });
}

const std::vector<OperatorMethod*>& CSharpOperators::division_operators() {
    return lazy(division_operators_, [&] { return lift(arithmetic<Div>(*this)); });
}

const std::vector<OperatorMethod*>& CSharpOperators::remainder_operators() {
    return lazy(remainder_operators_, [&] { return lift(arithmetic<Rem>(*this)); });
}

const std::vector<OperatorMethod*>& CSharpOperators::addition_operators() {
    return lazy(addition_operators_, [&] {
        std::vector<OperatorMethod*> methods = arithmetic_with_overflow_check<Add>(*this);
        methods.push_back(own(std::make_unique<StringConcatenation>(*this, KnownTypeCode::String, KnownTypeCode::String)));
        methods.push_back(own(std::make_unique<StringConcatenation>(*this, KnownTypeCode::String, KnownTypeCode::Object)));
        methods.push_back(own(std::make_unique<StringConcatenation>(*this, KnownTypeCode::Object, KnownTypeCode::String)));
        return lift(std::move(methods));
    });
}

const std::vector<OperatorMethod*>& CSharpOperators::subtraction_operators() {
    return lazy(subtraction_operators_, [&] { return lift(arithmetic_with_overflow_check<Sub>(*this)); });
}

const std::vector<OperatorMethod*>& CSharpOperators::shift_left_operators() {
    return lazy(shift_left_operators_, [&] {
        return lift(shifts(*this, &shift_left<std::int32_t>, &shift_left<std::uint32_t>, &shift_left<std::int64_t>, &shift_left<std::uint64_t>));
    });
}

const std::vector<OperatorMethod*>& CSharpOperators::shift_right_operators() {
    return lazy(shift_right_operators_, [&] {
        return lift(shifts(*this, &shift_right<std::int32_t>, &shift_right<std::uint32_t>, &shift_right<std::int64_t>, &shift_right<std::uint64_t>));
    });
}

const std::vector<OperatorMethod*>& CSharpOperators::unsigned_shift_right_operators() {
    return lazy(unsigned_shift_right_operators_, [&] {
        return lift(shifts(*this, &unsigned_shift_right<std::int32_t>, &unsigned_shift_right<std::uint32_t>, &unsigned_shift_right<std::int64_t>,
                           &unsigned_shift_right<std::uint64_t>));
    });
}

// ---- equality operators -------------------------------------------------------------------------------------------------------

namespace {

// C# 4.0 spec: 7.10 Relational and type-testing operators
constexpr KnownTypeCode value_equality_operators_for[] = {
    KnownTypeCode::Int32, KnownTypeCode::UInt32, KnownTypeCode::Int64, KnownTypeCode::UInt64,
    KnownTypeCode::Single, KnownTypeCode::Double, KnownTypeCode::Decimal, KnownTypeCode::Boolean,
};

}  // namespace

const std::vector<OperatorMethod*>& CSharpOperators::value_equality_operators() {
    return lazy(value_equality_operators_, [&] {
        std::vector<OperatorMethod*> methods;
        for (const KnownTypeCode c : value_equality_operators_for) methods.push_back(own(std::make_unique<EqualityOperatorMethod>(*this, c, false)));
        return lift(std::move(methods));
    });
}

const std::vector<OperatorMethod*>& CSharpOperators::value_inequality_operators() {
    return lazy(value_inequality_operators_, [&] {
        std::vector<OperatorMethod*> methods;
        for (const KnownTypeCode c : value_equality_operators_for) methods.push_back(own(std::make_unique<EqualityOperatorMethod>(*this, c, true)));
        return lift(std::move(methods));
    });
}

const std::vector<OperatorMethod*>& CSharpOperators::reference_equality_operators() {
    return lazy(reference_equality_operators_, [&] {
        return lift({
            own(std::make_unique<EqualityOperatorMethod>(*this, KnownTypeCode::Object, false)),
            own(std::make_unique<EqualityOperatorMethod>(*this, KnownTypeCode::String, false)),
        });
    });
}

const std::vector<OperatorMethod*>& CSharpOperators::reference_inequality_operators() {
    return lazy(reference_inequality_operators_, [&] {
        return lift({
            own(std::make_unique<EqualityOperatorMethod>(*this, KnownTypeCode::Object, true)),
            own(std::make_unique<EqualityOperatorMethod>(*this, KnownTypeCode::String, true)),
        });
    });
}

// ---- relational operators -----------------------------------------------------------------------------------------------------

const std::vector<OperatorMethod*>& CSharpOperators::less_than_operators() {
    return lazy(less_than_operators_, [&] { return lift(relationals<std::less>(*this)); });
}

const std::vector<OperatorMethod*>& CSharpOperators::less_than_or_equal_operators() {
    return lazy(less_than_or_equal_operators_, [&] { return lift(relationals<std::less_equal>(*this)); });
}

const std::vector<OperatorMethod*>& CSharpOperators::greater_than_operators() {
    return lazy(greater_than_operators_, [&] { return lift(relationals<std::greater>(*this)); });
}

const std::vector<OperatorMethod*>& CSharpOperators::greater_than_or_equal_operators() {
    return lazy(greater_than_or_equal_operators_, [&] { return lift(relationals<std::greater_equal>(*this)); });
}

// ---- bitwise operators --------------------------------------------------------------------------------------------------------

const std::vector<OperatorMethod*>& CSharpOperators::logical_and_operators() {
    return lazy(logical_and_operators_, [&] { return std::vector<OperatorMethod*>{binary<bool>(*this, [](bool a, bool b) { return a && b; })}; });
}

const std::vector<OperatorMethod*>& CSharpOperators::bitwise_and_operators() {
    return lazy(bitwise_and_operators_, [&] {
        return lift({
            binary<std::int32_t>(*this, [](std::int32_t a, std::int32_t b) { return static_cast<std::int32_t>(a & b); }),
            binary<std::uint32_t>(*this, [](std::uint32_t a, std::uint32_t b) { return static_cast<std::uint32_t>(a & b); }),
            binary<std::int64_t>(*this, [](std::int64_t a, std::int64_t b) { return static_cast<std::int64_t>(a & b); }),
            binary<std::uint64_t>(*this, [](std::uint64_t a, std::uint64_t b) { return static_cast<std::uint64_t>(a & b); }),
            logical_and_operators()[0],
        });
    });
}

const std::vector<OperatorMethod*>& CSharpOperators::logical_or_operators() {
    return lazy(logical_or_operators_, [&] { return std::vector<OperatorMethod*>{binary<bool>(*this, [](bool a, bool b) { return a || b; })}; });
}

const std::vector<OperatorMethod*>& CSharpOperators::bitwise_or_operators() {
    return lazy(bitwise_or_operators_, [&] {
        return lift({
            binary<std::int32_t>(*this, [](std::int32_t a, std::int32_t b) { return static_cast<std::int32_t>(a | b); }),
            binary<std::uint32_t>(*this, [](std::uint32_t a, std::uint32_t b) { return static_cast<std::uint32_t>(a | b); }),
            binary<std::int64_t>(*this, [](std::int64_t a, std::int64_t b) { return static_cast<std::int64_t>(a | b); }),
            binary<std::uint64_t>(*this, [](std::uint64_t a, std::uint64_t b) { return static_cast<std::uint64_t>(a | b); }),
            logical_or_operators()[0],
        });
    });
}

const std::vector<OperatorMethod*>& CSharpOperators::bitwise_xor_operators() {
    return lazy(bitwise_xor_operators_, [&] {
        return lift({
            binary<std::int32_t>(*this, [](std::int32_t a, std::int32_t b) { return static_cast<std::int32_t>(a ^ b); }),
            binary<std::uint32_t>(*this, [](std::uint32_t a, std::uint32_t b) { return static_cast<std::uint32_t>(a ^ b); }),
            binary<std::int64_t>(*this, [](std::int64_t a, std::int64_t b) { return static_cast<std::int64_t>(a ^ b); }),
            binary<std::uint64_t>(*this, [](std::uint64_t a, std::uint64_t b) { return static_cast<std::uint64_t>(a ^ b); }),
            binary<bool>(*this, [](bool a, bool b) { return a != b; }),
        });
    });
}

// ---- user-defined operators ---------------------------------------------------------------------------------------------------

const IMethod* CSharpOperators::lift_user_defined_operator(const IMethod* m) {
    if (is_comparison_operator(m)) {
        if (!typesystem::is_known_type(m->return_type(), KnownTypeCode::Boolean)) return nullptr;   // cannot lift this operator
    } else {
        if (!nullable_type::is_non_nullable_value_type(m->return_type())) return nullptr;   // cannot lift this operator
    }
    for (const IParameter* p : m->parameters()) {
        if (!nullable_type::is_non_nullable_value_type(p->type())) return nullptr;   // cannot lift this operator
    }
    // One lifted operator per operator method, so that the same operator is the same object (upstream's equality does the same).
    typesystem::TypeKey key{202, {m}, {}, {}};
    return m->compilation().arena().intern<LiftedUserDefinedOperator>(std::move(key), [&] {
        const auto* definition = dynamic_cast<const IMethod*>(m->member_definition());
        return std::make_unique<LiftedUserDefinedOperator>(m, definition);
    });
}

bool CSharpOperators::is_comparison_operator(const IMethod* m) {
    if (!m->is_operator() || m->parameters().size() != 2) return false;
    const std::string name = m->name();
    return name == "op_Equality" || name == "op_Inequality" || name == "op_GreaterThan" || name == "op_LessThan" || name == "op_GreaterThanOrEqual" ||
           name == "op_LessThanOrEqual";
}

}  // namespace cb::dotnet::decompiler::csharp::resolver
