// dotnet/decompiler/csharp/transforms/declare_variables.hpp -- DeclareVariables: inserts the declarations of the local variables the ILAst
// had without any. Each variable is declared at the smallest scope that holds all its uses; declarations that would collide (the same
// name twice in a block, or hiding one in a nested block) are merged into one; `T v; v = expr;` becomes `T v = expr;` and
// `T v; Call(out v);` becomes `Call(out T v);` where C# allows it.
//
//     DeclareVariables transform;
//     transform.run(root_node, context);                  // inserts the declarations
//     transform.analyze(root_node);                       // or only works out where they would go (the tree is not changed) ...
//     AstNode* at = transform.get_declaration_point(v);   // ... and answers where, until clear_analysis_results()
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Transforms: DeclareVariables, from DeclareVariables.cs), Copyright AlphaSierraPapa for
// the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "dotnet/decompiler/csharp/l2_body.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"
#include "dotnet/decompiler/csharp/transforms/iast_transform.hpp"
#include "dotnet/decompiler/csharp/transforms/transform_context.hpp"
#include "dotnet/decompiler/il/instructions.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

// Insert variable declarations.
class DeclareVariables final : public IAstTransform {
public:
    DeclareVariables() = default;
    DeclareVariables(const DeclareVariables&) = delete;
    DeclareVariables& operator=(const DeclareVariables&) = delete;

    // std::logic_error when called again while it runs.
    void run(syntax::AstNode* root_node, TransformContext& context) override;

    // Analyze the input AST (containing undeclared variables) for where those variables would be declared by this transform.
    // Analysis does not modify the AST.
    void analyze(syntax::AstNode* root_node, TransformContext& context);

    // The position where the declaration for the variable will be inserted: the node the declaration goes before.
    syntax::AstNode* get_declaration_point(il::ILVariable* variable);

    // Determines whether a variable was merged with other variables.
    bool was_merged(il::ILVariable* variable);

    void clear_analysis_results();

    static bool variable_needs_declaration(il::VariableKind kind);

private:
    // A position immediately before next_node: either an ExpressionStatement in a BlockStatement, or an initializer in a for-loop.
    struct InsertionPoint {
        // The nesting level of next_node within the AST. Used to speed up find_common_parent().
        int level = 0;
        syntax::AstNode* next_node = nullptr;

        // Go up one level.
        InsertionPoint up() const;
        InsertionPoint up_to(int target_level) const;
    };

    enum class VariableInitKind : int {
        None = 0,
        NeedsDefaultValue = 1,
        NeedsSkipInit = 2,
    };

    struct VariableToDeclare {
        il::ILVariable* il_variable;
        // What the transform reads of the variable (from L2: csharp/l2_body.hpp), as it is when the variable is first found.
        VariableFacts facts;
        const typesystem::IType* type() const { return facts.type; }
        // Variables reaching this transform have already been assigned a name.
        const std::string& name() const { return *facts.name; }

        // Whether the variable needs to be default-initialized.
        VariableInitKind default_initialization;

        // Integer value that can be used to compare two VariableToDeclare to determine which variable was used first in the source code:
        // the variable with the lower source_order has the insertion point that comes first.
        int source_order;

        // The insertion point, i.e. the node before which the variable declaration should be inserted.
        InsertionPoint insertion_point;

        // The first use of the variable.
        syntax::IdentifierExpression* first_use;

        VariableToDeclare* replacement_due_to_collision = nullptr;
        bool involved_in_collision = false;
        bool removed_due_to_collision() const { return replacement_due_to_collision != nullptr; }
        bool declared_in_deconstruction = false;

        VariableToDeclare(il::ILVariable* variable, VariableFacts facts, InsertionPoint insertion_point, syntax::IdentifierExpression* first_use,
                          int source_order);
    };

    struct ScopeEntry {
        InsertionPoint insertion_point;
        il::BlockContainer* scope;
    };

    struct Replacement {
        syntax::AstNode* old_node;
        std::function<syntax::AstNode*()> create_new_node;
        std::string step_description;
    };

    VariableToDeclare* find_variable(il::ILVariable* variable) const;
    void add_variable(il::ILVariable* variable, std::unique_ptr<VariableToDeclare> v);

    void ensure_expression_statements_are_valid(syntax::AstNode* root_node);
    static bool is_valid_in_statement_expression(syntax::Expression* expr);

    void find_insertion_points(syntax::AstNode* node, int node_level);
    void find_insertion_point_for_variable(il::ILVariable* variable, syntax::IdentifierExpression* ident_expr, int node_level);
    bool is_relevant_scope(il::BlockContainer* scope) const;
    static InsertionPoint find_common_parent(InsertionPoint old_point, InsertionPoint new_point);

    void resolve_collisions();
    void insert_deconstruction_variable_declarations();
    bool is_matching_assignment(VariableToDeclare& v, syntax::AssignmentExpression** assignment) const;
    bool combine_declaration_and_initializer(VariableToDeclare& v, TransformContext& context) const;
    void insert_variable_declarations(TransformContext& context);
    bool is_referenced_within_declaring_call(syntax::DirectionExpression* dir_expr, VariableToDeclare& v) const;
    VariableToDeclare* resolve_variable_to_declare(il::ILVariable* variable) const;
    bool can_be_declared_as_out_variable(VariableToDeclare& v, syntax::DirectionExpression** dir_expr) const;
    void update_annotations(syntax::AstNode* root_node);

    // The variables in the order they were found (a Dictionary enumerates in insertion order, which shows in the output).
    std::vector<std::unique_ptr<VariableToDeclare>> variables_;
    std::unordered_map<il::ILVariable*, VariableToDeclare*> variable_dict_;
    std::vector<ScopeEntry> scope_tracking_;
    TransformContext* context_ = nullptr;
};

}  // namespace cb::dotnet::decompiler::csharp::transforms
