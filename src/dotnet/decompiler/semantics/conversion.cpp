#include "dotnet/decompiler/semantics/conversion.hpp"

#include <algorithm>
#include <cstdint>
#include <functional>
#include <stdexcept>

namespace cb::dotnet::decompiler::semantics {

using typesystem::IMethod;

namespace {

// The conversion that is not one.
class InvalidConversion final : public Conversion {
public:
    bool is_valid() const override { return false; }
    std::string to_string() const override { return "None"; }
};

// Numeric conversions and enumeration conversions (implicit or explicit, lifted or not).
class NumericOrEnumerationConversion final : public Conversion {
public:
    NumericOrEnumerationConversion(bool is_implicit, bool is_lifted, bool is_enumeration = false)
        : is_implicit_(is_implicit), is_lifted_(is_lifted), is_enumeration_(is_enumeration) {}

    bool is_implicit() const override { return is_implicit_; }
    bool is_explicit() const override { return !is_implicit_; }
    bool is_numeric_conversion() const override { return !is_enumeration_; }
    bool is_enumeration_conversion() const override { return is_enumeration_; }
    bool is_lifted() const override { return is_lifted_; }

    std::string to_string() const override {
        return std::string(is_implicit_ ? "implicit" : "explicit") + (is_lifted_ ? " lifted" : "") + (is_enumeration_ ? " enumeration" : " numeric") +
               " conversion";
    }
    bool equals(const Conversion* other) const override {
        const auto* o = dynamic_cast<const NumericOrEnumerationConversion*>(other);
        return o != nullptr && is_implicit_ == o->is_implicit_ && is_lifted_ == o->is_lifted_ && is_enumeration_ == o->is_enumeration_;
    }
    int hash_code() const override { return (is_implicit_ ? 1 : 0) + (is_lifted_ ? 2 : 0) + (is_enumeration_ ? 4 : 0); }

private:
    bool is_implicit_;
    bool is_lifted_;
    bool is_enumeration_;
};

// The conversions with no data but their kind: identity, null literal, constant expression, reference, dynamic, nullable, pointer, boxing ...
class BuiltinConversion final : public Conversion {
public:
    BuiltinConversion(bool is_implicit, std::uint8_t type) : is_implicit_(is_implicit), type_(type) {}

    bool is_implicit() const override { return is_implicit_; }
    bool is_explicit() const override { return !is_implicit_; }
    bool is_identity_conversion() const override { return type_ == 0; }
    bool is_null_literal_conversion() const override { return type_ == 1; }
    bool is_constant_expression_conversion() const override { return type_ == 2; }
    bool is_reference_conversion() const override { return type_ == 3; }
    bool is_dynamic_conversion() const override { return type_ == 4; }
    bool is_nullable_conversion() const override { return type_ == 5; }
    bool is_pointer_conversion() const override { return type_ == 6; }
    bool is_boxing_conversion() const override { return type_ == 7; }
    bool is_unboxing_conversion() const override { return type_ == 8; }
    bool is_try_cast() const override { return type_ == 9; }
    bool is_interpolated_string_conversion() const override { return type_ == 10; }
    bool is_throw_expression_conversion() const override { return type_ == 11; }
    bool is_inline_array_conversion() const override { return type_ == 12; }
    bool is_implicit_span_conversion() const override { return type_ == 13; }

    std::string to_string() const override {
        std::string name;
        switch (type_) {
            case 0: return "identity conversion";
            case 1: return "null-literal conversion";
            case 2: name = "constant-expression"; break;
            case 3: name = "reference"; break;
            case 4: name = "dynamic"; break;
            case 5: name = "nullable"; break;
            case 6: name = "pointer"; break;
            case 7: return "boxing conversion";
            case 8: return "unboxing conversion";
            case 9: return "try cast";
            case 10: return "interpolated string";
            case 11: return "throw-expression conversion";
            case 12: return "inline array conversion";
            case 13: return "implicit span conversion";
        }
        return std::string(is_implicit_ ? "implicit " : "explicit ") + name + " conversion";
    }

private:
    bool is_implicit_;
    std::uint8_t type_;
};

// A conversion through a user-defined operator, with a standard conversion before and after it.
class UserDefinedConv final : public Conversion {
public:
    UserDefinedConv(bool is_implicit, const IMethod* method, const Conversion* before, const Conversion* after, bool is_lifted, bool is_ambiguous)
        : method_(method), is_lifted_(is_lifted), before_(before), after_(after), is_implicit_(is_implicit), is_valid_(!is_ambiguous) {}

    bool is_valid() const override { return is_valid_; }
    bool is_implicit() const override { return is_implicit_; }
    bool is_explicit() const override { return !is_implicit_; }
    bool is_lifted() const override { return is_lifted_; }
    bool is_user_defined() const override { return true; }
    const Conversion* conversion_before_user_defined_operator() const override { return before_; }
    const Conversion* conversion_after_user_defined_operator() const override { return after_; }
    const IMethod* method() const override { return method_; }

    bool equals(const Conversion* other) const override {
        const auto* o = dynamic_cast<const UserDefinedConv*>(other);
        return o != nullptr && is_lifted_ == o->is_lifted_ && is_implicit_ == o->is_implicit_ && is_valid_ == o->is_valid_ &&
               typesystem::member_equals(method_, o->method_);
    }
    int hash_code() const override {
        // (unsigned arithmetic: upstream's unchecked)
        return static_cast<int>(static_cast<std::uint32_t>(method_->hash_code()) + static_cast<std::uint32_t>(is_lifted_ ? 31 : 27) +
                                static_cast<std::uint32_t>(is_implicit_ ? 71 : 61) + static_cast<std::uint32_t>(is_valid_ ? 107 : 109));
    }
    std::string to_string() const override {
        return std::string(is_implicit_ ? "implicit" : "explicit") + (is_lifted_ ? " lifted" : "") + (is_valid_ ? "" : " ambiguous") +
               "user-defined conversion (" + method_->to_string() + ")";
    }

private:
    const IMethod* method_;
    bool is_lifted_;
    const Conversion* before_;
    const Conversion* after_;
    bool is_implicit_;
    bool is_valid_;
};

// A method group converted to a delegate.
class MethodGroupConv final : public Conversion {
public:
    MethodGroupConv(const IMethod* method, bool is_virtual_method_lookup, bool delegate_captures_first_argument, bool is_valid)
        : method_(method), is_virtual_method_lookup_(is_virtual_method_lookup), delegate_captures_first_argument_(delegate_captures_first_argument),
          is_valid_(is_valid) {}

    bool is_valid() const override { return is_valid_; }
    bool is_implicit() const override { return true; }
    bool is_method_group_conversion() const override { return true; }
    bool is_virtual_method_lookup() const override { return is_virtual_method_lookup_; }
    bool delegate_captures_first_argument() const override { return delegate_captures_first_argument_; }
    const IMethod* method() const override { return method_; }

    bool equals(const Conversion* other) const override {
        const auto* o = dynamic_cast<const MethodGroupConv*>(other);
        return o != nullptr && typesystem::member_equals(method_, o->method_);
    }
    int hash_code() const override { return method_->hash_code(); }

private:
    const IMethod* method_;
    bool is_virtual_method_lookup_;
    bool delegate_captures_first_argument_;
    bool is_valid_;
};

// A tuple converted element by element.
class TupleConv final : public Conversion {
public:
    explicit TupleConv(std::vector<const Conversion*> element_conversions) : element_conversions_(std::move(element_conversions)) {
        is_implicit_ = std::all_of(element_conversions_.begin(), element_conversions_.end(), [](const Conversion* c) { return c->is_implicit(); });
    }

    bool is_implicit() const override { return is_implicit_; }
    bool is_explicit() const override { return !is_implicit_; }
    const std::vector<const Conversion*>& element_conversions() const override { return element_conversions_; }
    bool is_tuple_conversion() const override { return true; }

    bool equals(const Conversion* other) const override {
        const auto* o = dynamic_cast<const TupleConv*>(other);
        if (o == nullptr || element_conversions_.size() != o->element_conversions_.size()) return false;
        for (std::size_t i = 0; i < element_conversions_.size(); i++)
            if (!conversion_equals(element_conversions_[i], o->element_conversions_[i])) return false;
        return true;
    }
    int hash_code() const override {
        std::uint32_t hash = 0;
        for (const Conversion* conv : element_conversions_) {
            hash *= 31;
            hash += static_cast<std::uint32_t>(conv->hash_code());
        }
        return static_cast<int>(hash);
    }
    // (sic: the doubled space is upstream's)
    std::string to_string() const override { return std::string(is_implicit_ ? "implicit " : "explicit ") + " tuple conversion"; }

private:
    std::vector<const Conversion*> element_conversions_;
    bool is_implicit_ = false;
};

// The singletons, made on first use (upstream's static readonly fields).
struct Singletons {
    InvalidConversion none;
    BuiltinConversion identity{true, 0};
    NumericOrEnumerationConversion implicit_numeric{true, false};
    NumericOrEnumerationConversion explicit_numeric{false, false};
    NumericOrEnumerationConversion implicit_lifted_numeric{true, true};
    NumericOrEnumerationConversion explicit_lifted_numeric{false, true};
    NumericOrEnumerationConversion enumeration[4] = {{true, false, true}, {true, true, true}, {false, false, true}, {false, true, true}};
    BuiltinConversion null_literal{true, 1};
    BuiltinConversion implicit_constant_expression{true, 2};
    BuiltinConversion implicit_reference{true, 3};
    BuiltinConversion explicit_reference{false, 3};
    BuiltinConversion implicit_dynamic{true, 4};
    BuiltinConversion explicit_dynamic{false, 4};
    BuiltinConversion implicit_nullable{true, 5};
    BuiltinConversion explicit_nullable{false, 5};
    BuiltinConversion implicit_pointer{true, 6};
    BuiltinConversion explicit_pointer{false, 6};
    BuiltinConversion boxing{true, 7};
    BuiltinConversion unboxing{false, 8};
    BuiltinConversion try_cast{false, 9};
    BuiltinConversion implicit_interpolated_string{true, 10};
    BuiltinConversion throw_expression{true, 11};
    BuiltinConversion inline_array{true, 12};
    BuiltinConversion implicit_span{true, 13};
};

const Singletons& singletons() {
    static const Singletons instance;
    return instance;
}

}  // namespace

const Conversion* Conversion::none() { return &singletons().none; }
const Conversion* Conversion::identity_conversion() { return &singletons().identity; }
const Conversion* Conversion::implicit_numeric_conversion() { return &singletons().implicit_numeric; }
const Conversion* Conversion::explicit_numeric_conversion() { return &singletons().explicit_numeric; }
const Conversion* Conversion::implicit_lifted_numeric_conversion() { return &singletons().implicit_lifted_numeric; }
const Conversion* Conversion::explicit_lifted_numeric_conversion() { return &singletons().explicit_lifted_numeric; }
const Conversion* Conversion::enumeration_conversion(bool is_implicit, bool is_lifted) {
    return &singletons().enumeration[(is_implicit ? 0 : 2) + (is_lifted ? 1 : 0)];
}
const Conversion* Conversion::null_literal_conversion() { return &singletons().null_literal; }
const Conversion* Conversion::implicit_constant_expression_conversion() { return &singletons().implicit_constant_expression; }
const Conversion* Conversion::implicit_reference_conversion() { return &singletons().implicit_reference; }
const Conversion* Conversion::explicit_reference_conversion() { return &singletons().explicit_reference; }
const Conversion* Conversion::implicit_dynamic_conversion() { return &singletons().implicit_dynamic; }
const Conversion* Conversion::explicit_dynamic_conversion() { return &singletons().explicit_dynamic; }
const Conversion* Conversion::implicit_nullable_conversion() { return &singletons().implicit_nullable; }
const Conversion* Conversion::explicit_nullable_conversion() { return &singletons().explicit_nullable; }
const Conversion* Conversion::implicit_pointer_conversion() { return &singletons().implicit_pointer; }
const Conversion* Conversion::explicit_pointer_conversion() { return &singletons().explicit_pointer; }
const Conversion* Conversion::boxing_conversion() { return &singletons().boxing; }
const Conversion* Conversion::unboxing_conversion() { return &singletons().unboxing; }
const Conversion* Conversion::try_cast() { return &singletons().try_cast; }
const Conversion* Conversion::implicit_interpolated_string_conversion() { return &singletons().implicit_interpolated_string; }
const Conversion* Conversion::throw_expression_conversion() { return &singletons().throw_expression; }
const Conversion* Conversion::inline_array_conversion() { return &singletons().inline_array; }
const Conversion* Conversion::implicit_span_conversion() { return &singletons().implicit_span; }

const Conversion* Conversion::user_defined_conversion(const IMethod* operator_method, bool is_implicit, const Conversion* conversion_before_user_defined_operator,
                                                      const Conversion* conversion_after_user_defined_operator, bool is_lifted, bool is_ambiguous) {
    if (operator_method == nullptr) throw std::invalid_argument("operatorMethod");
    return il::make<UserDefinedConv>(is_implicit, operator_method, conversion_before_user_defined_operator, conversion_after_user_defined_operator,
                                     is_lifted, is_ambiguous);
}

const Conversion* Conversion::method_group_conversion(const IMethod* chosen_method, bool is_virtual_method_lookup, bool delegate_captures_first_argument) {
    if (chosen_method == nullptr) throw std::invalid_argument("chosenMethod");
    return il::make<MethodGroupConv>(chosen_method, is_virtual_method_lookup, delegate_captures_first_argument, true);
}

const Conversion* Conversion::invalid_method_group_conversion(const IMethod* chosen_method, bool is_virtual_method_lookup,
                                                              bool delegate_captures_first_argument) {
    if (chosen_method == nullptr) throw std::invalid_argument("chosenMethod");
    return il::make<MethodGroupConv>(chosen_method, is_virtual_method_lookup, delegate_captures_first_argument, false);
}

const Conversion* Conversion::tuple_conversion(std::vector<const Conversion*> conversions) { return il::make<TupleConv>(std::move(conversions)); }

const std::vector<const Conversion*>& Conversion::element_conversions() const {
    static const std::vector<const Conversion*> none;
    return none;
}

// (upstream: object.GetHashCode, the identity)
int Conversion::hash_code() const { return static_cast<int>(std::hash<const void*>{}(this)); }

}  // namespace cb::dotnet::decompiler::semantics
