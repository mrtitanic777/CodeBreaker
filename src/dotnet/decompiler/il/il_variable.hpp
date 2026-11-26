// dotnet/decompiler/il/il_variable.hpp -- a variable of the ILAst (a parameter, a local, a stack slot, an exception variable ...) with
// the instructions that load, store and take its address, kept current by the instructions themselves while they are connected; and
// a function's collection of variables.
//
//     ILVariable* v = make<ILVariable>(VariableKind::Local, type, 0);
//     function->variables().add(v);
//     v->load_count(); v->is_single_definition();
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL: VariableKind, VariableKindExtensions, ILVariable, ILVariableEqualityComparer,
// ILVariableCollection), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "dotnet/decompiler/il/il_instruction.hpp"

namespace cb::dotnet::decompiler::il {

class LdLoc;
class LdLoca;

enum class VariableKind : std::uint8_t {
    Local,
    PinnedLocal,
    PinnedRegionLocal,
    UsingLocal,
    ForeachLocal,
    InitializerTarget,
    Parameter,
    ExceptionStackSlot,
    ExceptionLocal,
    StackSlot,
    NamedArgument,
    DisplayClassLocal,
    PatternLocal,
    DeconstructionInitTemporary,
};

bool is_local(VariableKind kind);

class ILVariable : public ArenaObject {
public:
    ILVariable(VariableKind kind, const IType* type, std::optional<int> index = std::nullopt);
    ILVariable(VariableKind kind, const IType* type, StackType stack_type, std::optional<int> index = std::nullopt);

    VariableKind kind() const { return kind_; }
    void set_kind(VariableKind value);
    const StackType stack_type;
    const IType* type() const { return type_; }
    // The new type must have the variable's stack type.
    void set_type(const IType* value);
    bool is_ref_read_only = false;
    // The parameter index (-1 for `this`), the local's slot, the exception stack slot's index; none for others.
    std::optional<int> index() const { return index_; }
    bool is_this() const { return kind_ == VariableKind::Parameter && index_.has_value() && *index_ < 0; }

    std::optional<std::string> name;
    bool has_generated_name = false;
    ILFunction* function() const { return function_; }
    // The container the variable is captured in (a closure's scope), if any.
    BlockContainer* capture_scope = nullptr;
    int index_in_function() const { return index_in_function_; }

    int load_count() const { return static_cast<int>(load_instructions_.size()); }
    const std::vector<LdLoc*>& load_instructions() const { return load_instructions_; }
    int store_count() const { return (uses_initial_value_ ? 1 : 0) + static_cast<int>(store_instructions_.size()); }
    const std::vector<IStoreInstruction*>& store_instructions() const { return store_instructions_; }
    int address_count() const { return static_cast<int>(address_instructions_.size()); }
    const std::vector<LdLoca*>& address_instructions() const { return address_instructions_; }

    void add_load_instruction(LdLoc* inst);
    void add_store_instruction(IStoreInstruction* inst);
    void add_address_instruction(LdLoca* inst);
    void remove_load_instruction(LdLoc* inst);
    void remove_store_instruction(IStoreInstruction* inst);
    void remove_address_instruction(LdLoca* inst);

    // Whether the variable's value on entry is initialized (zeroed locals, parameters).
    bool initial_value_is_initialized() const { return initial_value_is_initialized_; }
    void set_initial_value_is_initialized(bool value);
    // Whether the initial value may be read (counts as a store).
    bool uses_initial_value() const { return uses_initial_value_; }
    void set_uses_initial_value(bool value);

    bool is_single_definition() const { return store_count() == 1 && address_count() == 0; }
    bool is_dead() const { return store_instructions_.empty() && load_count() == 0 && address_count() == 0; }

    // The state machine field this variable came from (yield return, async).
    const IField* state_machine_field = nullptr;
    bool remove_if_redundant = false;

    std::string to_string() const { return name.value_or(""); }
    void write_definition_to(ITextOutput& output) const;
    void write_to(ITextOutput& output) const;
    bool is_used_within(ILInstruction* inst) const;
    void check_invariant() const;

private:
    friend class ILVariableCollection;

    VariableKind kind_;
    const IType* type_;
    std::optional<int> index_;
    ILFunction* function_ = nullptr;
    int index_in_function_ = 0;
    std::vector<LdLoc*> load_instructions_;
    std::vector<IStoreInstruction*> store_instructions_;
    std::vector<LdLoca*> address_instructions_;
    bool initial_value_is_initialized_ = false;
    bool uses_initial_value_ = false;
};

// Variables equal when they are the same slot of the same function (upstream's ILVariableEqualityComparer): the same parameter or
// local index, or the same state machine field; stack slots and pattern locals equal only themselves.
struct ILVariableEqualityComparer {
    bool operator()(const ILVariable* x, const ILVariable* y) const;
};
struct ILVariableHash {
    std::size_t operator()(const ILVariable* v) const;
};

// A function's variables: adding sets the variable's function and index in it; removing moves the last variable into the gap.
class ILVariableCollection {
public:
    explicit ILVariableCollection(ILFunction* scope) : scope_(scope) {}
    ILVariableCollection(const ILVariableCollection&) = delete;
    ILVariableCollection& operator=(const ILVariableCollection&) = delete;

    ILVariable* operator[](int index) const { return list_.at(static_cast<std::size_t>(index)); }
    int count() const { return static_cast<int>(list_.size()); }
    auto begin() const { return list_.begin(); }
    auto end() const { return list_.end(); }
    const std::vector<ILVariable*>& items() const { return list_; }

    // false when the variable is already in this function (an error when in another)
    bool add(ILVariable* item);
    void clear();
    bool contains(const ILVariable* item) const;
    bool remove(ILVariable* item);
    // Removes the dead variables (not display class locals, nor parameters but a nested function's `this`).
    void remove_dead();

private:
    void remove_at(int index);

    ILFunction* scope_;
    std::vector<ILVariable*> list_;
};

}  // namespace cb::dotnet::decompiler::il
