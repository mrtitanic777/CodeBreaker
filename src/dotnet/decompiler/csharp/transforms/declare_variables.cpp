// dotnet/decompiler/csharp/transforms/declare_variables.cpp -- see declare_variables.hpp.
#include "dotnet/decompiler/csharp/transforms/declare_variables.hpp"

#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <unordered_set>

#include "dotnet/decompiler/csharp/annotations.hpp"
#include "dotnet/decompiler/csharp/statement_builder.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/transforms/assign_variable_names.hpp"
#include "dotnet/decompiler/nr_extensions.hpp"
#include "dotnet/decompiler/semantics/out_var_resolve_result.hpp"
#include "dotnet/decompiler/typesystem/decompiler_type_system.hpp"

#include "dotnet/decompiler/csharp/expression_builder.hpp"
#include "dotnet/decompiler/decompile_run.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

using namespace syntax;
using il::BlockContainer;
using il::ILFunction;
using il::ILInstruction;
using il::ILVariable;
using il::VariableKind;

namespace {

// Runs a function when the scope ends (upstream's finally).
template <class F>
struct ScopeExit {
    explicit ScopeExit(F f_) : f(std::move(f_)) {}
    ~ScopeExit() { f(); }
    ScopeExit(const ScopeExit&) = delete;
    ScopeExit& operator=(const ScopeExit&) = delete;
    F f;
};

// ExpressionBuilder.HidesVariableWithName(function, name).
bool hides_variable_with_name(ILFunction* function, const std::string& name) {
    return ExpressionBuilder::hides_variable_with_name(function, name);
}

}  // namespace

DeclareVariables::InsertionPoint DeclareVariables::InsertionPoint::up() const {
    InsertionPoint result;
    result.level = level - 1;
    // Insertion points live inside a method body, so walking up always finds a parent.
    result.next_node = next_node->parent();
    return result;
}

DeclareVariables::InsertionPoint DeclareVariables::InsertionPoint::up_to(int target_level) const {
    InsertionPoint result = *this;
    while (result.level > target_level) {
        result.next_node = result.next_node->parent();
        result.level -= 1;
    }
    return result;
}

DeclareVariables::VariableToDeclare::VariableToDeclare(ILVariable* variable, VariableFacts facts_, InsertionPoint point, IdentifierExpression* use,
                                                       int order)
    : il_variable(variable), facts(std::move(facts_)), default_initialization(VariableInitKind::None), source_order(order), insertion_point(point),
      first_use(use) {
    if (facts.uses_initial_value) {
        if (facts.initial_value_is_initialized) {
            default_initialization = VariableInitKind::NeedsDefaultValue;
        } else {
            default_initialization = VariableInitKind::NeedsSkipInit;
        }
    }
}

DeclareVariables::VariableToDeclare* DeclareVariables::find_variable(ILVariable* variable) const {
    auto it = variable_dict_.find(variable);
    return it == variable_dict_.end() ? nullptr : it->second;
}

void DeclareVariables::add_variable(ILVariable* variable, std::unique_ptr<VariableToDeclare> v) {
    variable_dict_[variable] = v.get();
    variables_.push_back(std::move(v));
}

void DeclareVariables::clear_analysis_results() {
    variable_dict_.clear();
    variables_.clear();
}

void DeclareVariables::run(AstNode* root_node, TransformContext& context) {
    ScopeExit cleanup([this] {
        context_ = nullptr;
        clear_analysis_results();
    });
    if (context_ != nullptr) {
        context_ = nullptr;
        throw std::logic_error("Reentrancy in DeclareVariables?");
    }
    context_ = &context;
    clear_analysis_results();
    ensure_expression_statements_are_valid(root_node);
    find_insertion_points(root_node, 0);
    resolve_collisions();
    insert_deconstruction_variable_declarations();
    insert_variable_declarations(context);
    update_annotations(root_node);
}

void DeclareVariables::analyze(AstNode* root_node, TransformContext& context) {
    // (the analysis reads the variables' facts through the context's L2 lane)
    TransformContext* const was = context_;
    context_ = &context;
    ScopeExit restore([&] { context_ = was; });
    clear_analysis_results();
    find_insertion_points(root_node, 0);
    resolve_collisions();
}

AstNode* DeclareVariables::get_declaration_point(ILVariable* variable) {
    VariableToDeclare* v = variable_dict_.at(variable);
    while (v->replacement_due_to_collision != nullptr) {
        v = v->replacement_due_to_collision;
    }
    return v->insertion_point.next_node;
}

bool DeclareVariables::was_merged(ILVariable* variable) {
    VariableToDeclare* v = variable_dict_.at(variable);
    return v->involved_in_collision || v->removed_due_to_collision();
}

// ---- EnsureExpressionStatementsAreValid ------------------------------------------------------------------------------------------

void DeclareVariables::ensure_expression_statements_are_valid(AstNode* root_node) {
    TransformContext& context = *context_;
    for (AstNode* node : root_node->descendants_and_self().to_vector()) {
        auto* stmt = as<ExpressionStatement>(node);
        if (stmt == nullptr) continue;
        auto* dir = as<DirectionExpression>(stmt->expression());
        if (dir != nullptr && is_valid_in_statement_expression(dir->expression())) {
            context.step("Unwrap direction expression statement", stmt);
            stmt->set_expression(detach(dir->expression()));
        } else if (!is_valid_in_statement_expression(stmt->expression())) {
            // fetch ILFunction
            ILFunction* function = nullptr;
            for (AstNode* ancestor : stmt->ancestors()) {
                for (il::ArenaObject* annotation : ancestor->annotations()) {
                    auto* f = dynamic_cast<ILFunction*>(annotation);
                    if (f != nullptr && f->parent() == nullptr) {
                        function = f;
                        break;
                    }
                }
                if (function != nullptr) break;
            }
            if (function == nullptr) throw std::logic_error("Sequence contains no matching element");
            // if possible use C# 7.0 discard-assignment
            if (context.settings.discards && !hides_variable_with_name_of(context.l2, *function, "_")) {
                context.step("Assign invalid expression statement to discard", stmt);
                Expression* discard = make<IdentifierExpression>("_");  // no ResolveResult
                stmt->set_expression(make<AssignmentExpression>(discard, detach(stmt->expression())));
            } else {
                context.step("Assign invalid expression statement to temporary", stmt);
                // assign result to dummy variable
                const typesystem::IType* type = get_resolve_result(stmt->expression())->type();
                ILInstruction* value_context = nullptr;
                for (il::ArenaObject* annotation : stmt->expression()->annotations()) {
                    auto* inst = dynamic_cast<ILInstruction*>(annotation);
                    if (inst != nullptr && il::transforms::AssignVariableNames::is_supported_instruction(inst)) {
                        value_context = inst;
                        break;
                    }
                }
                const type_system::UsingScope* using_scope = context.decompile_run.using_scope;
                std::string name = il::transforms::AssignVariableNames::generate_variable_name(function, type, using_scope, value_context,
                                                                                               nullptr, true);
                ILVariable* v = function->register_variable(VariableKind::StackSlot, type, name);
                if (context.l2 != nullptr) context.l2->variable_registered(function, v);
                Expression* target = with_rr(make<IdentifierExpression>(*v->name), make<ILVariableResolveResult>(v, v->type())).expression;
                stmt->set_expression(make<AssignmentExpression>(target, detach(stmt->expression())));
            }
        }
    }
}

bool DeclareVariables::is_valid_in_statement_expression(Expression* expr) {
    if (is<InvocationExpression>(expr) || is<ObjectCreateExpression>(expr) || is<AssignmentExpression>(expr) || is<ErrorExpression>(expr))
        return true;
    if (auto* uoe = as<UnaryOperatorExpression>(expr)) {
        switch (uoe->operator_()) {
            case UnaryOperatorType::PostIncrement:
            case UnaryOperatorType::PostDecrement:
            case UnaryOperatorType::Increment:
            case UnaryOperatorType::Decrement:
            case UnaryOperatorType::Await:
                return true;
            case UnaryOperatorType::NullConditionalRewrap:
                return is_valid_in_statement_expression(uoe->expression());
            default:
                return false;
        }
    }
    return false;
}

// ---- FindInsertionPoints ---------------------------------------------------------------------------------------------------------

// Finds insertion points for all variables used within `node` and adds them to the variable dictionary.
// `node_level` == nesting depth of `node` within root node.
// Insertion point for a variable = common parent of all uses of that variable = smallest possible scope that contains all the uses of the
// variable.
void DeclareVariables::find_insertion_points(AstNode* node, int node_level) {
    BlockContainer* scope = node->annotation<BlockContainer>();
    auto* lambda = as<LambdaExpression>(node);
    if (scope != nullptr && is_relevant_scope(scope)) {
        // track loops and function bodies as scopes, for comparison with CaptureScope.
        scope_tracking_.push_back({InsertionPoint{node_level, node}, scope});
    } else if (lambda != nullptr && is<Expression>(lambda->body())) {
        // expression-bodied lambdas don't have a BlockStatement linking to the BlockContainer
        ILFunction* function = node->annotation<ILFunction>();
        scope = function != nullptr ? function_body_of(context_->l2, *function) : nullptr;
        if (scope != nullptr) {
            scope_tracking_.push_back({InsertionPoint{node_level + 1, lambda->body()}, scope});
        }
    } else {
        scope = nullptr;  // don't remove a scope if we didn't add one
    }
    ScopeExit pop_scope([&] {
        if (scope != nullptr) scope_tracking_.pop_back();
    });
    for (AstNode* child = node->first_child(); child != nullptr; child = child->next_sibling()) {
        find_insertion_points(child, node_level + 1);
    }
    if (auto* ident_expr = as<IdentifierExpression>(node)) {
        auto* rr = dynamic_cast<ILVariableResolveResult*>(get_resolve_result(ident_expr));
        ILFunction* local_function = ident_expr->annotation<ILFunction>();
        if (rr != nullptr && variable_needs_declaration(variable_of(context_->l2, *rr->variable).kind)) {
            find_insertion_point_for_variable(rr->variable, ident_expr, node_level);
        } else if (local_function != nullptr && function_kind_of(context_->l2, *local_function) == il::ILFunctionKind::LocalFunction) {
            for (ILVariable* v : captured_variables_of(context_->l2, *local_function)) {
                if (variable_needs_declaration(variable_of(context_->l2, *v).kind)) find_insertion_point_for_variable(v, ident_expr, node_level);
            }
        }
    }
}

void DeclareVariables::find_insertion_point_for_variable(ILVariable* variable, IdentifierExpression* ident_expr, int node_level) {
    InsertionPoint new_point;
    int start_index = static_cast<int>(scope_tracking_.size()) - 1;
    VariableFacts facts = variable_of(context_->l2, *variable);
    BlockContainer* capture_scope = facts.capture_scope;
    while (capture_scope != nullptr && !is_relevant_scope(capture_scope)) {
        capture_scope = enclosing_container_of(context_->l2, *capture_scope);
    }
    if (capture_scope != nullptr && start_index > 0 && capture_scope != scope_tracking_[start_index].scope) {
        while (start_index > 0 && scope_tracking_[start_index].scope != capture_scope) start_index--;
        new_point = scope_tracking_[start_index + 1].insertion_point;
    } else {
        new_point = InsertionPoint{node_level, ident_expr};
        if (facts.uses_initial_value) {
            // Uninitialized variables are logically initialized at the beginning of the function
            // Because it's possible that the variable has a loop-carried dependency,
            // declare it outside of any loops.
            while (start_index >= 0) {
                const ContainerFacts scope = container_of(context_->l2, *scope_tracking_[start_index].scope);
                if (scope.entry_incoming > 1) {
                    // declare variable outside of loop
                    new_point = scope_tracking_[start_index].insertion_point;
                } else if (scope.is_function_body) {
                    // stop at beginning of function
                    break;
                }
                start_index--;
            }
        }
    }
    if (VariableToDeclare* v = find_variable(variable)) {
        v->insertion_point = find_common_parent(v->insertion_point, new_point);
    } else {
        add_variable(variable, std::make_unique<VariableToDeclare>(variable, std::move(facts), new_point, ident_expr, static_cast<int>(variables_.size())));
    }
}

bool DeclareVariables::is_relevant_scope(BlockContainer* scope) const {
    const ContainerFacts facts = container_of(context_->l2, *scope);
    return facts.entry_incoming > 1 || facts.is_function_body;
}

bool DeclareVariables::variable_needs_declaration(VariableKind kind) {
    switch (kind) {
        case VariableKind::PinnedRegionLocal:
        case VariableKind::Parameter:
        case VariableKind::ExceptionLocal:
        case VariableKind::ExceptionStackSlot:
        case VariableKind::UsingLocal:
        case VariableKind::ForeachLocal:
        case VariableKind::PatternLocal:
            return false;
        default:
            return true;
    }
}

// Finds an insertion point in a common parent instruction.
DeclareVariables::InsertionPoint DeclareVariables::find_common_parent(InsertionPoint old_point, InsertionPoint new_point) {
    // First ensure we're looking at nodes on the same level:
    old_point = old_point.up_to(new_point.level);
    new_point = new_point.up_to(old_point.level);
    assert(new_point.level == old_point.level);
    // Then go up the tree until both points share the same parent:
    while (old_point.next_node->parent() != new_point.next_node->parent()) {
        old_point = old_point.up();
        new_point = new_point.up();
    }
    // return old_point as that one comes first in the source code
    return old_point;
}

// ---- ResolveCollisions -----------------------------------------------------------------------------------------------------------

// Some variable declarations in C# are illegal (colliding), even though the variable live ranges are not overlapping.
//
// Multiple declarations in the same block (`int i = 1; use(1); int i = 2; use(2);`), "hiding" declarations in a nested block, and nested
// blocks even if the parent block declares the variable later. This detects all these cases and combines the variable declarations to a
// single declaration that is usable for the combined scopes.
void DeclareVariables::resolve_collisions() {
    std::unordered_map<std::string, std::vector<VariableToDeclare*>> multi_dict;
    for (auto& owned : variables_) {
        VariableToDeclare* v = owned.get();
        // We can only insert variable declarations in blocks, but find_insertion_points() didn't
        // guarantee that it finds only blocks.
        // Fix that up now.
        while (!(is<BlockStatement>(v->insertion_point.next_node->parent()) || is<LambdaExpression>(v->insertion_point.next_node->parent()))) {
            auto* f = as<ForStatement>(v->insertion_point.next_node->parent());
            if (f != nullptr && v->insertion_point.next_node == f->initializers().first_or_null() && is_matching_assignment(*v, nullptr)) {
                // Special case: the initializer of a ForStatement can also declare a variable (with scope local to the for loop).
                break;
            }
            v->insertion_point = v->insertion_point.up();
        }
        // Note: 'out var', pattern matching etc. is not considered a valid insertion point here, because the scope of the
        // resulting variable is not restricted to the parent node of the insertion point, but extends to the whole BlockStatement.
        // We moved up the insertion point to the whole BlockStatement so that we can resolve collisions,
        // later we might decide to declare the variable more locally (as 'out var') instead if still possible.

        // Go through all potentially colliding variables:
        for (VariableToDeclare* prev : multi_dict[v->name()]) {
            if (prev->removed_due_to_collision()) continue;
            // Go up until both nodes are on the same level:
            InsertionPoint point1 = prev->insertion_point.up_to(v->insertion_point.level);
            InsertionPoint point2 = v->insertion_point.up_to(prev->insertion_point.level);
            assert(point1.level == point2.level);
            if (point1.next_node->parent() == point2.next_node->parent()) {
                assert(prev->type()->equals(v->type()));
                // We found a collision!
                v->involved_in_collision = true;
                prev->replacement_due_to_collision = v;
                // Continue checking other entries in multi_dict against the new position of `v`.
                if (prev->source_order < v->source_order) {
                    // Switch v's insertion point to prev's insertion point:
                    v->insertion_point = point1;
                    // Since prev was first, it has the correct source_order/first_use values for the new combined variable:
                    v->source_order = prev->source_order;
                    v->first_use = prev->first_use;
                } else {
                    // v is first in source order, so it keeps its old insertion point (and other properties), except that the
                    // insertion point is moved up to prev's level.
                    v->insertion_point = point2;
                }
                v->default_initialization =
                    static_cast<VariableInitKind>(static_cast<int>(v->default_initialization) | static_cast<int>(prev->default_initialization));
                // I think we don't need to re-check the dict entries that we already checked earlier,
                // because the new v.insertion_point only collides with another point x if either
                // the old v.insertion_point or the old prev.insertion_point already collided with x.
            }
        }
        multi_dict[v->name()].push_back(v);
    }
}

void DeclareVariables::insert_deconstruction_variable_declarations() {
    TransformContext& context = *context_;
    // Group the variables by insertion point node, in the order the nodes are first seen.
    std::vector<std::pair<AstNode*, std::vector<VariableToDeclare*>>> groups;
    for (auto& v : variables_) {
        AstNode* key = v->insertion_point.next_node;
        auto it = std::find_if(groups.begin(), groups.end(), [&](const auto& g) { return g.first == key; });
        if (it == groups.end()) {
            groups.push_back({key, {}});
            it = groups.end() - 1;
        }
        it->second.push_back(v.get());
    }
    std::unordered_set<ILVariable*> used_variables;
    for (auto& [key, g] : groups) {
        auto* expression_statement = as<ExpressionStatement>(key);
        if (expression_statement == nullptr) continue;
        auto* assignment = as<AssignmentExpression>(expression_statement->expression());
        if (assignment == nullptr || assignment->operator_() != AssignmentOperatorType::Assign) continue;
        auto* left = as<TupleExpression>(assignment->left());
        if (left == nullptr) continue;
        used_variables.clear();
        auto* deconstruct = assignment->annotation<il::DeconstructInstruction>();
        if (deconstruct == nullptr || deconstruct->init.count() > 0 || deconstruct->conversions()->instructions.count() > 0) continue;

        auto is_declarable_variable = [&](ILInstruction* inst) {
            ILVariable* v;
            ILInstruction* value;
            if (!inst->match_st_loc(&v, &value)) return false;
            if (!std::any_of(g.begin(), g.end(), [&](VariableToDeclare* vd) { return vd->il_variable == v && !vd->removed_due_to_collision(); }))
                return false;
            if (!used_variables.insert(v).second) return false;
            const typesystem::IType* expected_type = variable_of(context.l2, *static_cast<il::LdLoc*>(value)->variable()).type;
            const VariableFacts facts = variable_of(context.l2, *v);
            if (!facts.type->equals(expected_type)) return false;
            if (!(facts.kind == VariableKind::StackSlot || facts.kind == VariableKind::Local)) return false;
            return true;
        };
        bool all_declarable = true;
        for (ILInstruction* inst : deconstruct->assignments()->instructions) {
            if (!is_declarable_variable(inst)) {
                all_declarable = false;
                break;
            }
        }
        if (!all_declarable) continue;

        VariableDesignation* designation = StatementBuilder::translate_deconstruction_designation(deconstruct, false);
        context.step("Declare deconstruction variables", left);
        auto* declaration_expression = make<DeclarationExpression>(make<SimpleType>("var"), designation);
        left->replace_with(declaration_expression);
        context.end_step(declaration_expression);

        for (ILVariable* v : used_variables) {
            variable_dict_.at(v)->declared_in_deconstruction = true;
        }
    }
}

bool DeclareVariables::is_matching_assignment(VariableToDeclare& v, AssignmentExpression** assignment_out) const {
    auto* assignment = as<AssignmentExpression>(v.insertion_point.next_node);
    if (assignment == nullptr) {
        auto* statement = as<ExpressionStatement>(v.insertion_point.next_node);
        assignment = statement != nullptr ? as<AssignmentExpression>(statement->expression()) : nullptr;
        if (assignment == nullptr) {
            if (assignment_out != nullptr) *assignment_out = nullptr;
            return false;
        }
    }
    if (assignment_out != nullptr) *assignment_out = assignment;
    auto* ident_expr = as<IdentifierExpression>(assignment->left());
    return assignment->operator_() == AssignmentOperatorType::Assign && ident_expr != nullptr && ident_expr->identifier() == v.name() &&
           ident_expr->type_arguments().count() == 0;
}

bool DeclareVariables::combine_declaration_and_initializer(VariableToDeclare& v, TransformContext& context) const {
    if (v.type()->is_by_ref_like()) return true;  // by-ref-like variables always must be initialized at their declaration.

    const CSharpSlotInfo* slot = v.insertion_point.next_node->slot();
    if (slot != nullptr && slot->kind() == &Slots::for_initializer)
        return true;  // for-statement initializers always should combine declaration and initialization.

    return !context.settings.separate_local_variable_declarations;
}

void DeclareVariables::insert_variable_declarations(TransformContext& context) {
    std::vector<Replacement> replacements;
    for (auto& owned : variables_) {
        VariableToDeclare* v = owned.get();
        ILVariable* il_variable = v->il_variable;
        if (v->removed_due_to_collision() || v->declared_in_deconstruction) continue;

        AssignmentExpression* assignment = nullptr;
        DirectionExpression* dir_expr = nullptr;
        if (combine_declaration_and_initializer(*v, context) && is_matching_assignment(*v, &assignment)) {
            // 'int v; v = expr;' can be combined to 'int v = expr;'
            AstType* type;
            if (context.settings.anonymous_types && contains_anonymous_type(v->type())) {
                type = make<SimpleType>("var");
            } else {
                type = context.type_system_ast_builder.convert_type(v->type());
            }
            auto* composed_type = as<ComposedType>(type);
            if (v->facts.is_ref_read_only && composed_type != nullptr && composed_type->has_ref_specifier()) {
                composed_type->set_has_read_only_specifier(true);
            }
            if (v->facts.kind == VariableKind::PinnedLocal) {
                type->add_trailing_trivia(make<Comment>("pinned", CommentType::MultiLine));
            }
            replacements.push_back({v->insertion_point.next_node,
                                    [v, type, assignment]() -> AstNode* {
                                        Expression* right = detach(assignment->right());
                                        auto* vds = make<VariableDeclarationStatement>(type, v->name(), right);
                                        VariableInitializer* init = vds->variables().first_or_null();
                                        init->add_annotation(get_resolve_result(assignment->left()));
                                        std::vector<il::ArenaObject*> all = assignment->left()->annotations();
                                        for (il::ArenaObject* a : assignment->annotations()) all.push_back(a);
                                        for (il::ArenaObject* annotation : all) {
                                            if (dynamic_cast<ResolveResult*>(annotation) == nullptr) init->add_annotation(annotation);
                                        }
                                        return vds;
                                    },
                                    "Combine variable declaration with initializer"});
        } else if (can_be_declared_as_out_variable(*v, &dir_expr)) {
            // 'T v; SomeCall(out v);' can be combined to 'SomeCall(out T v);'
            AstType* type;
            bool is_out_var = false;
            if (context.settings.anonymous_types && contains_anonymous_type(v->type())) {
                type = make<SimpleType>("var");
                is_out_var = true;
            } else if (dir_expr->annotation<UseImplicitlyTypedOutAnnotation>() != nullptr && !is_referenced_within_declaring_call(dir_expr, *v)) {
                type = make<SimpleType>("var");
                is_out_var = true;
            } else {
                type = context.type_system_ast_builder.convert_type(v->type());
            }
            std::string name;
            // Variable is not used and discards are allowed, we can simplify this to 'out T _'.
            // TODO: if no variable named _ is declared and var is used instead of T, use out _.
            // Note: ExpressionBuilder.HidesVariableWithName produces inaccurate results, because it
            // does not take lambdas and local functions into account, that are defined in the same
            // scope as v.
            if (context.settings.discards && v->facts.load_count == 0 && v->facts.store_count == 0 && v->facts.address_count == 1) {
                name = "_";
            } else {
                name = v->name();
            }
            auto* ovd = make<OutVarDeclarationExpression>(type, name);
            ovd->variable()->add_annotation(make<ILVariableResolveResult>(il_variable));
            copy_annotations_from(ovd, dir_expr);
            if (is_out_var) {
                ovd->remove_annotations<ResolveResult>();
                ovd->add_annotation(make<semantics::OutVarResolveResult>(v->type()));
            }
            replacements.push_back({dir_expr, [ovd]() -> AstNode* { return ovd; }, "Declare out variable"});
        } else {
            // Insert a separate declaration statement.
            Expression* initializer = nullptr;
            AstType* type = context.type_system_ast_builder.convert_type(v->type());
            if (v->default_initialization == VariableInitKind::NeedsDefaultValue) {
                initializer = make<DefaultValueExpression>(clone_as(type));
            }
            auto* vds = make<VariableDeclarationStatement>(type, v->name(), initializer);
            vds->variables().first_or_null()->add_annotation(make<ILVariableResolveResult>(il_variable));
            context.step("Insert variable declaration", v->insertion_point.next_node);
            if (auto* lambda = as<LambdaExpression>(v->insertion_point.next_node->parent())) {
                assert(!is<BlockStatement>(lambda->body()));
                auto* block = make<BlockStatement>();
                block->add(make<ReturnStatement>(static_cast<Expression*>(detach(lambda->body()))));
                lambda->set_body(block);
            }
            if (is<ReturnStatement>(v->insertion_point.next_node->parent())) {
                v->insertion_point = v->insertion_point.up();
            }
            assert(v->insertion_point.next_node->slot()->kind() == &Slots::statement);
            // The insertion point is a statement within a block, so it always has a parent.
            AstNode* insertion_node = v->insertion_point.next_node;
            AstNode* insertion_parent = insertion_node->parent();
            if (insertion_parent == nullptr) throw std::logic_error("Variable insertion point has no parent.");
            if (v->default_initialization == VariableInitKind::NeedsSkipInit) {
                AstType* unsafe_type = context.type_system_ast_builder.convert_type(
                    context.type_system.find_type(typesystem::KnownTypeCode::Unsafe));
                AstNode* inserted_node;
                if (context.settings.out_variables) {
                    auto* out_var_decl = make<OutVarDeclarationExpression>(clone_as(type), v->name());
                    out_var_decl->variable()->add_annotation(make<ILVariableResolveResult>(il_variable));
                    auto* skip_init_statement = make<ExpressionStatement>();
                    auto* target = make<MemberReferenceExpression>(make<TypeReferenceExpression>(unsafe_type), "SkipInit");
                    skip_init_statement->set_expression(make<InvocationExpression>(target, std::initializer_list<Expression*>{out_var_decl}));
                    insertion_parent->insert_child_before(v->insertion_point.next_node, skip_init_statement, Slots::statement);
                    inserted_node = skip_init_statement;
                } else {
                    insertion_parent->insert_child_before(v->insertion_point.next_node, vds, Slots::statement);
                    inserted_node = vds;
                    auto* skip_init_statement = make<ExpressionStatement>();
                    auto* target = make<MemberReferenceExpression>(make<TypeReferenceExpression>(unsafe_type), "SkipInit");
                    Expression* identifier =
                        with_rr(make<IdentifierExpression>(v->name()), make<ILVariableResolveResult>(il_variable)).expression;
                    auto* direction = make<DirectionExpression>(FieldDirection::Out, identifier);
                    skip_init_statement->set_expression(make<InvocationExpression>(target, std::initializer_list<Expression*>{direction}));
                    insertion_parent->insert_child_before(v->insertion_point.next_node, skip_init_statement, Slots::statement);
                }
                context.end_step(inserted_node);
            } else {
                insertion_parent->insert_child_before(v->insertion_point.next_node, vds, Slots::statement);
                context.end_step(vds);
            }
        }
    }
    // perform replacements at end, so that we don't replace a node while it is still referenced by a VariableToDeclare
    for (auto& replacement : replacements) {
        context.step(replacement.step_description, replacement.old_node);
        AstNode* new_node = replacement.create_new_node();
        replacement.old_node->replace_with(new_node);
        context.end_step(new_node);
    }
}

// Gets whether the variable declared by `dir_expr` is referenced again within another argument of the call containing the declaration. In
// that case the declaration must use the explicit type: referencing an implicitly-typed out variable is not permitted until overload
// resolution of the declaring call has inferred its type (CS8196).
bool DeclareVariables::is_referenced_within_declaring_call(DirectionExpression* dir_expr, VariableToDeclare& v) const {
    AstNode* call = dir_expr->parent();
    if (call == nullptr) return false;
    for (AstNode* argument = call->first_child(); argument != nullptr; argument = argument->next_sibling()) {
        if (argument == dir_expr) continue;
        for (AstNode* node : argument->descendants_and_self()) {
            auto* identifier = as<IdentifierExpression>(node);
            if (identifier != nullptr && resolve_variable_to_declare(get_il_variable(identifier)) == &v) return true;
        }
    }
    return false;
}

// Maps an ILVariable to the variable declaration it will end up in, following merges performed by resolve_collisions.
DeclareVariables::VariableToDeclare* DeclareVariables::resolve_variable_to_declare(ILVariable* variable) const {
    if (variable == nullptr) return nullptr;
    VariableToDeclare* v = find_variable(variable);
    if (v == nullptr) return nullptr;
    while (v->replacement_due_to_collision != nullptr) {
        v = v->replacement_due_to_collision;
    }
    return v;
}

bool DeclareVariables::can_be_declared_as_out_variable(VariableToDeclare& v, DirectionExpression** dir_expr_out) const {
    auto* dir_expr = as<DirectionExpression>(v.first_use->parent());
    *dir_expr_out = dir_expr;
    if (dir_expr == nullptr || dir_expr->field_direction() != FieldDirection::Out) return false;
    if (!context_->settings.out_variables) return false;
    if (v.default_initialization != VariableInitKind::None) return false;
    for (AstNode* node = v.first_use; node != nullptr; node = node->parent()) {
        const CSharpSlotInfo* slot = node->slot();
        if (slot != nullptr && slot->kind() == &Slots::embedded_statement) {
            return false;
        }
        if (is<IfElseStatement>(node) || is<ExpressionStatement>(node)) {  // variable declared in if condition appears in parent scope
            return node == v.insertion_point.next_node;
        }
        if (is<Statement>(node)) {
            return false;  // other statements (e.g. while) don't allow variables to be promoted to parent scope
        }
        if (auto* lambda = as<LambdaExpression>(node)) {
            return lambda->body() == v.insertion_point.next_node;
        }
    }
    return false;
}

// Update ILVariableResolveResult annotations of all ILVariables that have been replaced by resolve_collisions.
void DeclareVariables::update_annotations(AstNode* root_node) {
    for (AstNode* node : root_node->descendants().to_vector()) {
        ILVariable* il_var;
        if (auto* id = as<IdentifierExpression>(node)) {
            il_var = get_il_variable(id);
        } else if (auto* vi = as<VariableInitializer>(node)) {
            il_var = get_il_variable(vi);
        } else {
            continue;
        }
        if (il_var == nullptr || !variable_needs_declaration(variable_of(context_->l2, *il_var).kind)) continue;
        VariableToDeclare* v = variable_dict_.at(il_var);
        if (!v->removed_due_to_collision()) continue;
        while (v->replacement_due_to_collision != nullptr) {
            v = v->replacement_due_to_collision;
        }
        node->remove_annotations<ILVariableResolveResult>();
        node->add_annotation(make<ILVariableResolveResult>(v->il_variable, v->type()));
    }
}

}  // namespace cb::dotnet::decompiler::csharp::transforms
