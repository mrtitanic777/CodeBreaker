// dotnet/decompiler/il/transforms/transform_display_class_usage.hpp -- turns the fields of the compiler's closure classes (display
// classes) into local variables: a display class that is created by an empty constructor, never passed to another method and never
// the target of an invocation is scalar-replaced, each of its fields becoming a variable of the function that declared it (captured by
// the lambdas and local functions that use it).
//
//     stloc dc(newobj DisplayClass())                   stloc x(...)
//     stfld DisplayClass::x(ldloc dc, ...)       =>     ... ldloc x ...
//     ... ldfld DisplayClass::x(ldloc dc) ...
//
//     TransformDisplayClassUsage transform;
//     transform.run(function, context);
//
// This is a post-processing step of DelegateConstruction, LocalFunctionDecompiler and TransformExpressionTrees.
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: TransformDisplayClassUsage, from TransformDisplayClassUsage.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "dotnet/decompiler/il/il_visitor.hpp"
#include "dotnet/decompiler/il/transforms/il_transform.hpp"
#include "dotnet/decompiler/util/insertion_ordered_map.hpp"

namespace cb::dotnet::decompiler::typesystem {
class ITypeDefinition;
}

namespace cb::dotnet::decompiler::il::transforms {

using typesystem::ITypeDefinition;

class TransformDisplayClassUsage final : public ILVisitor, public IILTransform {
public:
    TransformDisplayClassUsage();
    ~TransformDisplayClassUsage() override;

    void run(ILFunction* function, ILTransformContext& context) override;
    std::string name() const override { return "TransformDisplayClassUsage"; }

    // Whether `method` is a parameterless constructor of the main module that only calls the base object constructor.
    static bool validate_constructor(ILTransformContext& context, const IMethod* method);
    // Whether `variable` holds a closure (a display class instance or a display struct): the closure's type and the instruction that
    // initializes it (its store, or the statement of its first address use).
    static bool is_closure(ILTransformContext& context, ILVariable* variable, const ITypeDefinition** closure_type, ILInstruction** initializer);
    static bool is_potential_closure(ILTransformContext& context, NewObj* inst);
    static bool is_potential_closure(ILTransformContext& context, const ITypeDefinition* potential_display_class);
    // A compiler-generated class or struct of the same type tree as the decompiled type.
    static bool is_potential_closure(const ITypeDefinition* decompiled_type_definition, const ITypeDefinition* potential_display_class,
                                     bool allow_type_implementing_interfaces = false);

    // ILVisitor
    void default_visit(ILInstruction& inst) override;
    void visit_il_function(ILFunction& function) override;
    void visit_st_loc(StLoc& inst) override;
    void visit_st_obj(StObj& inst) override;
    void visit_ld_obj(LdObj& inst) override;
    void visit_ld_flda(LdFlda& inst) override;

private:
    class DisplayClass;

    // A field of a display class that becomes a variable.
    class VariableToDeclare {
    public:
        VariableToDeclare(DisplayClass* container, const IField* field, ILVariable* declared_variable = nullptr);

        std::string name() const;
        bool can_propagate() const { return can_propagate_; }
        bool uses_initial_value = false;
        std::vector<ILInstruction*> initializers;  // a set upstream; only membership is asked

        bool has_initializer(ILInstruction* inst) const;
        void propagate(ILVariable* variable);
        ILVariable* get_or_declare();

    private:
        DisplayClass* container_;
        const IField* field_;
        ILVariable* declared_variable_;
        bool can_propagate_ = false;
    };

    class DisplayClass {
    public:
        DisplayClass(ILVariable* variable, const ITypeDefinition* type) : variable(variable), type(type) {}
        ILVariable* const variable;
        const ITypeDefinition* const type;
        util::InsertionOrderedMap<const IField*, VariableToDeclare*> variables_to_declare;
        BlockContainer* capture_scope = nullptr;
        ILInstruction* initializer = nullptr;
    };

    void clear_state();
    void analyze_function(ILFunction* function);
    bool validate_display_class_uses(ILVariable* v, DisplayClass* display_class);
    bool validate_use(ILVariable* v, DisplayClass* container, ILInstruction* use);
    static bool is_plain_read_or_initializer_store(LdFlda* ldflda, VariableToDeclare* variable);
    DisplayClass* analyze_variable(ILVariable* v);
    DisplayClass* detect_display_class(ILVariable* v);
    void handle_init_block(Block* init_block, int start_index, DisplayClass* result, ILVariable* target_variable);
    static Block* find_display_struct_init_block(ILVariable* v);
    DisplayClass* detect_display_class_initializer(ILVariable* v);
    bool validate_display_class_definition(const ITypeDefinition* definition);
    VariableToDeclare* add_variable(DisplayClass* result, StObj* statement, const IField* field);
    ILVariable* resolve_variable_to_propagate(ILInstruction* value, const IType* expected_type = nullptr);
    void transform(ILFunction* function);
    static bool is_closure_init(ILTransformContext& context, StLoc* inst, const ITypeDefinition** closure_type);
    bool is_mono_nested_capture_scope(const ITypeDefinition* closure_type);
    DisplayClass* handle_mono_state_machine(ILFunction* function, ILVariable* this_variable);
    bool is_display_class_load(ILInstruction* target, ILVariable** variable);
    bool is_display_class_field_access(ILInstruction* inst, ILVariable** display_class_var, DisplayClass** display_class, const IField** field);

    DisplayClass* find_display_class(ILVariable* v) const;
    DisplayClass* make_display_class(ILVariable* variable, const ITypeDefinition* type);
    VariableToDeclare* make_variable_to_declare(DisplayClass* container, const IField* field, ILVariable* declared_variable = nullptr);

    ILTransformContext* context_ = nullptr;
    const ITypeDefinition* current_type_definition_ = nullptr;  // upstream's decompilationContext.CurrentTypeDefinition
    util::InsertionOrderedMap<ILVariable*, DisplayClass*> display_classes_;
    util::InsertionOrderedMap<ILVariable*, ILVariable*> display_class_copy_map_;
    std::vector<std::unique_ptr<DisplayClass>> owned_display_classes_;
    std::vector<std::unique_ptr<VariableToDeclare>> owned_variables_;
    std::vector<ILFunction*> current_functions_;  // a stack: the last is the top
};

}  // namespace cb::dotnet::decompiler::il::transforms
