// dotnet/decompiler/il/control_flow/symbolic_execution.hpp -- evaluates the little expressions a state machine's MoveNext tests its
// state with (`state - 3`, `state == 2`, `this.state`) symbolically, so each branch can be given the set of states that take it.
//
//     SymbolicEvaluationContext eval(state_field);
//     SymbolicValue v = eval.eval(comp_instruction).as_bool();
//     if (v.type == SymbolicValueType::StateInSet) ... v.value_set ...
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.ControlFlow: SymbolicAnalysisFailedException, SymbolicValueType, SymbolicValue,
// SymbolicEvaluationContext, from ControlFlow/SymbolicExecution.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in
// dotnet/decompiler).
#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

#include "dotnet/decompiler/il/instructions.hpp"
#include "dotnet/decompiler/util/long_set.hpp"

namespace cb::dotnet::decompiler::il::control_flow {

// Thrown when the compiler's output is something else than expected: it aborts the analysis and makes the whole transform fail.
class SymbolicAnalysisFailedException : public std::runtime_error {
public:
    SymbolicAnalysisFailedException() : std::runtime_error("") {}
    explicit SymbolicAnalysisFailedException(const std::string& message) : std::runtime_error(message) {}
};

enum class SymbolicValueType : std::uint8_t {
    // Unknown value
    Unknown,
    // int: Constant (result of ldc.i4)
    IntegerConstant,
    // int: State + Constant
    State,
    // This pointer (result of ldarg.0)
    This,
    // bool: ValueSet.Contains(State)
    StateInSet,
};

// The name of the type, as .NET writes an enum value.
const char* to_string(SymbolicValueType type);

struct SymbolicValue {
    int constant = 0;
    SymbolicValueType type = SymbolicValueType::Unknown;
    util::LongSet value_set;

    SymbolicValue() = default;
    explicit SymbolicValue(SymbolicValueType type_, int constant_ = 0) : constant(constant_), type(type_) {}
    SymbolicValue(SymbolicValueType type_, util::LongSet value_set_) : type(type_), value_set(std::move(value_set_)) {}

    SymbolicValue as_bool() const;
    std::string to_string() const;
};

class SymbolicEvaluationContext {
public:
    explicit SymbolicEvaluationContext(const IField* state_field, bool legacy_visual_basic = false)
        : state_field_(state_field), legacy_visual_basic_(legacy_visual_basic) {}

    void add_state_variable(ILVariable* v);
    const std::vector<ILVariable*>& state_variables() const { return state_variables_; }

    SymbolicValue eval(ILInstruction* inst);

private:
    const IField* state_field_;
    bool legacy_visual_basic_;
    std::vector<ILVariable*> state_variables_;
};

}  // namespace cb::dotnet::decompiler::il::control_flow
