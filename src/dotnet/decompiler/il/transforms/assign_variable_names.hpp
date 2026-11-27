// dotnet/decompiler/il/transforms/assign_variable_names.hpp -- gives every variable and local function a name: the name the compiler
// kept where it is valid, else one made from how the variable is used (the property or field it is read from, the parameter it is
// passed to, the type), numbered where names clash; loop counters get i, j, k ...; and the foreach variable is named after the singular
// of the collection.
//
//     AssignVariableNames transform;
//     transform.run(function, context);          // sets ILVariable::name and ILFunction::name
//     AssignVariableNames::generate_variable_name(function, type);   // "obj", "list", "item" ...
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: AssignVariableNames, AssignVariableNames.VariableScope, from
// AssignVariableNames.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler); the singular forms are
// made by the vendored Humanizer (dotnet/decompiler/humanizer). Not yet ported (it waits for the C# stage): the C# UsingScope of the
// context (the lower-case type names the file's using declarations bring in, and the `usingScope` parameter of the generate_*
// functions that hand it on). Copies of three small C# stage helpers sit in the .cxx: CSharpDecompiler.IsTransparentIdentifier,
// CSharpDecompiler.IsWindowsFormsInitializeComponentMethod, TransformFieldAndConstructorInitializers.IsGeneratedPrimaryConstructorBackingField,
// the C# keywords of CSharpOutputVisitor.IsKeyword and TextWriterTokenWriter.ContainsNonPrintableIdentifierChar.
#pragma once

#include <cstdint>
#include <deque>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "dotnet/decompiler/il/il_visitor.hpp"
#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::transforms {

// C#'s Unit: a result that is nothing.
struct Unit {};

class AssignVariableNames final : public IILTransform {
public:
    class VariableScope;

    void run(ILFunction* function, ILTransformContext& context) override;
    std::string name() const override { return "AssignVariableNames"; }

    // Whether the name is a C# identifier a variable may have (a letter or underscore, then letters, digits and underscores).
    static bool is_valid_name(const std::string& var_name);
    // Whether GetNameFromInstruction can make a name of the instruction (must be in sync with it).
    static bool is_supported_instruction(const ILInstruction* arg);
    // The names for a foreach variable (the singular of the collection's name, "item") and for other generated variables; the number
    // that keeps them apart from the names in the function is appended.
    static std::string generate_foreach_variable_name(ILFunction* function, ILInstruction* value_context, ILVariable* existing_variable = nullptr,
                                                      bool must_resolve_conflicts = false);
    static std::string generate_variable_name(ILFunction* function, const IType* type, ILInstruction* value_context = nullptr,
                                              ILVariable* existing_variable = nullptr, bool must_resolve_conflicts = false);

    // The names in use in one function (and, through its parent, in the functions that contain it).
    class VariableScope {
    public:
        VariableScope(ILFunction* function, ILTransformContext& context, VariableScope* parent_scope = nullptr);

        void add(std::uint32_t local_function_token, const std::string& name);
        // The name a local function (by its method's metadata token), a variable or a slot of a function's local signature has, if any.
        std::optional<std::string> try_get_existing_name_of_local_function(std::uint32_t local_function_token) const;
        std::optional<std::string> try_get_existing_name(ILVariable* v) const;
        std::optional<std::string> try_get_existing_name(ILFunction* function, int index) const;
        void assign_name_to_local_signature_index(ILFunction* function, int index, const std::string& name);
        bool is_reserved_variable_name(const std::string& name, int* index) const;
        void reserve_variable_name(const std::string& name, int index = 1);
        std::string next_display_class_local();
        bool is_loop_counter(ILVariable* v) const;
        std::string assign_name_if_unassigned(ILVariable* v);
        std::string assign_name(ILVariable* v);

    private:
        std::string generate_name_for_variable(ILVariable* variable);

        ILTransformContext* context_;
        VariableScope* parent_scope_;
        ILFunction* function_;
        std::unordered_map<std::uint32_t, std::string> local_functions_;
        std::unordered_map<ILVariable*, std::string, ILVariableHash, ILVariableEqualityComparer> variable_mapping_;
        std::vector<std::optional<std::string>> assigned_local_signature_indices_;

        std::shared_ptr<const std::unordered_set<std::string>> current_lower_case_type_or_member_names_;
        std::unordered_map<std::string, int> reserved_variable_names_;
        std::unordered_set<ILVariable*> loop_counters_;
        int num_display_class_locals_ = 0;
    };

private:
    // Upstream's AssignVariableNames is the visitor itself (an ILVisitor<VariableScope, Unit>); a C++ class cannot derive from a visitor
    // over its own nested class, so the visitor is this nested class.
    class Visitor final : public ILVisitorCT<VariableScope*, Unit> {
    public:
        explicit Visitor(AssignVariableNames& owner) : owner_(owner) {}
        Unit visit_children(ILInstruction& inst, VariableScope* context);

        Unit default_visit(ILInstruction& inst, VariableScope* context) override;
        Unit visit_il_function(ILFunction& function, VariableScope* context) override;
        Unit visit_call(Call& inst, VariableScope* context) override;
        Unit visit_ld_ftn(LdFtn& inst, VariableScope* context) override;

    private:
        AssignVariableNames& owner_;
    };

    static void assign_name_to_local_function(ILFunction* function, VariableScope& parent_context);

    ILTransformContext* context_ = nullptr;
    // upstream's Queue<(ILFunction function, VariableScope parentScope)>
    std::deque<std::pair<ILFunction*, VariableScope*>> work_list_;
    std::vector<std::unique_ptr<VariableScope>> scopes_;  // the scopes of the work list, kept until the run ends
};

}  // namespace cb::dotnet::decompiler::il::transforms
