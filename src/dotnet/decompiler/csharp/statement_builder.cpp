// dotnet/decompiler/csharp/statement_builder.cpp -- see statement_builder.hpp.
#include "dotnet/decompiler/csharp/statement_builder.hpp"
#include "translate/frontends/cil/cil_names.hpp"
#include "dotnet/decompiler/csharp/l2_body.hpp"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <functional>
#include <stdexcept>
#include <typeinfo>
#include <unordered_set>
#include <variant>

#include "dotnet/decompiler/csharp/annotations.hpp"
#include "dotnet/decompiler/csharp/expression_builder.hpp"
#include "dotnet/decompiler/csharp/csharp_decompiler.hpp"
#include "dotnet/decompiler/csharp/resolver/csharp_resolver.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax_extensions.hpp"
#include "dotnet/decompiler/csharp/syntax/type_system_ast_builder.hpp"
#include "dotnet/decompiler/csharp/type_system/using_scope.hpp"
#include "dotnet/decompiler/decompile_run.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/pointer_arithmetic_offset.hpp"
#include "dotnet/decompiler/il/semantic_helper.hpp"
#include "dotnet/decompiler/il/il_type_extensions.hpp"
#include "dotnet/decompiler/il/transforms/assign_variable_names.hpp"
#include "dotnet/decompiler/il/transforms/il_inlining.hpp"
#include "dotnet/decompiler/nr_extensions.hpp"
#include "dotnet/decompiler/semantics/conversion_resolve_result.hpp"
#include "dotnet/decompiler/semantics/for_each_resolve_result.hpp"
#include "dotnet/decompiler/semantics/member_resolve_result.hpp"
#include "dotnet/decompiler/semantics/type_resolve_result.hpp"
#include "dotnet/decompiler/typesystem/decompiler_type_system.hpp"
#include "dotnet/decompiler/typesystem/tuple_type.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"
#include "dotnet/decompiler/util/char_info.hpp"
#include "dotnet/decompiler/util/csharp_primitive_cast.hpp"
#include "dotnet/decompiler/util/culture_compare.hpp"

namespace cb::dotnet::decompiler::csharp {

using il::make;
using syntax::AstType;
using semantics::ConstantResolveResult;
using semantics::ConversionResolveResult;
using semantics::ResolveResult;
using typesystem::ConstantValue;
using typesystem::IType;
using typesystem::KnownTypeCode;
using typesystem::SpecialType;
using typesystem::TypeKind;

namespace {

// ---- small helpers ---------------------------------------------------------------------------------------------------------------

// upstream's `new BlockStatement { stmt }`
syntax::BlockStatement* block_of(Statement* stmt) {
    auto* block = make<syntax::BlockStatement>();
    block->add(stmt);
    return block;
}

// An empty expression statement carrying only a comment after it.
syntax::EmptyStatement* comment_statement(const std::string& text) {
    auto* stmt = make<syntax::EmptyStatement>();
    stmt->add_trailing_trivia(make<syntax::Comment>(text));
    return stmt;
}

// upstream's `(T)x` on an instruction (InvalidCastException)
template <class T>
T* cast_inst(il::ILInstruction* inst) {
    T* result = il::as<T>(inst);
    if (result == nullptr) throw std::bad_cast();
    return result;
}

// KnownTypeCode of a type that is a type definition of a primitive (upstream's ReflectionHelper.GetTypeCode: the TypeCode values are the
// KnownTypeCode values up to String, except Void); None otherwise.
KnownTypeCode primitive_type_code(const IType* type) {
    const auto* def = dynamic_cast<const typesystem::ITypeDefinition*>(type);
    if (def == nullptr) return KnownTypeCode::None;
    KnownTypeCode code = def->known_type_code();
    if (code <= KnownTypeCode::String && code != KnownTypeCode::Void) return code;
    return KnownTypeCode::None;
}

ConstantValue int64_value(std::int64_t value) { return ConstantValue(std::in_place_type<std::int64_t>, value); }

bool is_fixed_size_buffer(Expression* init_expr) {
    auto* mrr = dynamic_cast<semantics::MemberResolveResult*>(get_resolve_result(init_expr));
    if (mrr == nullptr) return false;
    const auto* field = dynamic_cast<const typesystem::IField*>(mrr->member());
    const IType* element_type = nullptr;
    int element_count = 0;
    return field != nullptr && CSharpDecompiler::is_fixed_field(field, &element_type, &element_count);
}

bool is_address_of_moveable_var(Expression* init_expr) {
    auto* uoe = syntax::as<syntax::UnaryOperatorExpression>(init_expr);
    if (uoe != nullptr && uoe->operator_() == syntax::UnaryOperatorType::AddressOf) {
        auto* inst = uoe->expression()->annotation<il::ILInstruction>();
        return !(inst != nullptr && il::PointerArithmeticOffset::is_fixed_variable(inst));
    }
    return false;
}

bool equal_erased_type(const IType* a, const IType* b) { return typesystem::NormalizeTypeVisitor::type_erasure().equivalent_types(a, b); }

// OrderBy(f => f.Name): the default string comparer is the culture-sensitive one (util/culture_compare.hpp).
bool name_less(const std::string& a, const std::string& b) { return util::culture_compare(a, b) < 0; }

// The constant of a string key of a switch on string (null for no key).
ConstantValue string_key_value(const std::optional<std::string>& key) {
    if (!key.has_value()) return ConstantValue();
    return ConstantValue(std::in_place_type<std::u16string>, util::utf16(*key));
}

}  // namespace

// ---- construction ----------------------------------------------------------------------------------------------------------------

StatementBuilder::StatementBuilder(const typesystem::DecompilerTypeSystem& type_system, const typesystem::ITypeResolveContext& decompilation_context,
                                   il::ILFunction* current_function, const DecompilerSettings& settings, DecompileRun& run,
                                   const util::CancellationToken& cancellation_token, L2Body* l2)
    : decompile_run(run),
      current_function_(current_function),
      type_system_(&type_system),
      decompilation_context_(&decompilation_context),
      settings_(&settings),
      cancellation_token_(cancellation_token) {
    expr_builder_ = std::make_unique<ExpressionBuilder>(this, type_system, decompilation_context, current_function, settings, run, cancellation_token, l2);
    current_return_container = cast_inst<il::BlockContainer>(current_function->body());
    current_is_iterator = current_function->is_iterator;
    current_result_type = current_function->is_async() ? current_function->async_return_type : current_function->return_type;
}

StatementBuilder::~StatementBuilder() = default;

Statement* StatementBuilder::convert(il::ILInstruction* inst) {
    cancellation_token_.throw_if_cancellation_requested();
    if (L2Body* l2 = expr_builder().l2) {
        Statement* from_l2 = nullptr;
        if (const translate::ir::Node* node = l2->node(inst)) from_l2 = convert_from_l2(*node, *inst);
        l2->count(from_l2 != nullptr, *inst);
        if (from_l2 != nullptr) return from_l2;
    }
    return il::accept_visitor(*inst, *this);
}

// Each is the visitor below reading the same facts from the node (ILAST_TO_L2.md section 1.2): Empty (nop: its comment is the note), Throw
// (throw: the operand; rethrow: none), If in a statement place (if: the condition, then the branches; an absent or empty else branch is
// a nop), Return and Break (leave), Goto (br), YieldReturn, Sync (lock), Fixed (fixed), Try (using, try-finally, try-fault, try-catch),
// a Block or a container's Labeled block, a Region (a container: a loop, a switch, a block), and an expression in a statement place.
// An operand or a body is translated through its own instruction (which reads its node in turn).
Statement* StatementBuilder::convert_from_l2(const translate::ir::Node& n, il::ILInstruction& inst) {
    namespace ir = translate::ir;
    L2Body& l2 = *expr_builder().l2;
    // An operand or a body: the instruction its node was lifted to, while it is still where the node is (see translate_from_l2), below
    // `parent` (this instruction, or a handler).
    const auto instruction_of = [&](const ir::Node* child, const il::ILInstruction* parent = nullptr) -> il::ILInstruction* {
        while (const auto* e = ir::get<ir::ExprStmt>(child)) child = e->expr;
        il::ILInstruction* i = child != nullptr ? l2.instruction(child) : nullptr;
        const il::ILInstruction* holder = parent != nullptr ? parent : &inst;
        return i != nullptr && i->parent() == holder && holder->get_child(i->child_index()) == i ? i : nullptr;
    };
    const auto from = [&](Statement* s) {
        s->add_annotation(make<L2NodeAnnotation>(&n));
        return s;
    };
    const auto container_of = [&](const std::string& label) -> il::BlockContainer* {
        // a region's label is its entry block's: the Labeled block, in the Region the container was lowered to
        ir::Node* labeled = l2.index().block(label);
        ir::Node* region = labeled != nullptr ? l2.index().place(labeled).parent : nullptr;
        return region != nullptr ? il::as<il::BlockContainer>(l2.instruction(region)) : nullptr;
    };
    if (const auto* empty = ir::get<ir::Empty>(&n)) {
        auto* stmt = make<syntax::EmptyStatement>();
        if (empty->note && !empty->note->empty()) stmt->add_trailing_trivia(make<syntax::Comment>(*empty->note));
        return from(with_il_instruction(stmt, &inst));
    }
    if (const auto* thrown = ir::get<ir::Throw>(&n)) {
        if (thrown->expr == nullptr) return from(with_il_instruction(make<syntax::ThrowStatement>(), &inst));
        il::ILInstruction* operand = instruction_of(thrown->expr);
        if (operand == nullptr) return nullptr;
        TranslatedExpression argument = expr_builder().translate(operand);
        return from(with_il_instruction(make<syntax::ThrowStatement>(argument), &inst));
    }
    if (const auto* branch = ir::get<ir::If>(&n)) {
        il::ILInstruction* condition_inst = instruction_of(branch->cond);
        const auto* then_node = std::get_if<ir::Node*>(&branch->then);
        il::ILInstruction* then_inst = then_node != nullptr ? instruction_of(*then_node) : nullptr;
        const auto* else_node = std::get_if<ir::Node*>(&branch->orelse);
        const bool no_else = else_node == nullptr || *else_node == nullptr || ir::is<ir::Empty>(*else_node);
        il::ILInstruction* else_inst = no_else ? nullptr : instruction_of(*else_node);
        if (condition_inst == nullptr || then_inst == nullptr || (!no_else && else_inst == nullptr)) return nullptr;
        TranslatedExpression condition = expr_builder().translate_condition(condition_inst);
        Statement* true_statement = convert(then_inst);
        Statement* false_statement = no_else ? nullptr : convert(else_inst);
        return from(with_il_instruction(make<syntax::IfElseStatement>(condition, true_statement, false_statement), &inst));
    }
    if (const auto* ret = ir::get<ir::Return>(&n)) {
        // leave out of the function's body (lift.cpp: a Return leaves the body of the function it is in)
        il::ILInstruction* value = ret->expr != nullptr ? instruction_of(ret->expr) : nullptr;
        if ((ret->expr != nullptr && value == nullptr) || current_return_container == nullptr) return nullptr;
        return from(translate_leave(LeaveFacts{&inst, current_return_container, value}));
    }
    if (const auto* brk = ir::get<ir::Break>(&n)) {
        // leave out of another container: the region whose entry block the label names
        il::BlockContainer* container = brk->label ? container_of(*brk->label) : nullptr;
        il::ILInstruction* value = brk->value != nullptr ? instruction_of(brk->value) : nullptr;
        if (container == nullptr || (brk->value != nullptr && value == nullptr)) return nullptr;
        return from(translate_leave(LeaveFacts{&inst, container, value}));
    }
    if (const auto* jump = ir::get<ir::Goto>(&n)) {
        // br: the block the label names
        const ir::Node* labeled = jump->label ? l2.index().block(*jump->label) : nullptr;
        auto* block = labeled != nullptr ? il::as<il::Block>(l2.instruction(labeled)) : nullptr;
        if (block == nullptr) return nullptr;
        return from(translate_branch(BranchFacts{&inst, block}));
    }
    if (const auto* yield = ir::get<ir::YieldReturn>(&n)) {
        il::ILInstruction* value = instruction_of(yield->expr);
        if (value == nullptr) return nullptr;
        return from(translate_yield_return(YieldReturnFacts{&inst, value}));
    }
    if (const auto* sync = ir::get<ir::Sync>(&n)) {
        il::ILInstruction* on = instruction_of(sync->lock);
        il::ILInstruction* body = instruction_of(sync->body);
        if (on == nullptr || body == nullptr) return nullptr;
        return from(translate_lock(LockFacts{&inst, on, body}));
    }
    if (ir::is<ir::Switch>(&n)) {
        // switch in a statement place, outside a container of its own
        auto* self = il::as<il::SwitchInstruction>(&inst);
        std::optional<SwitchFacts> facts = self != nullptr ? l2.switch_facts(n, *self) : std::nullopt;
        if (!facts) return nullptr;
        return from(translate_switch_statement(*facts));
    }
    if (const auto* fixed = ir::get<ir::Fixed>(&n)) {
        // fixed (lift.cpp: the pinned variable a Local of the variable, its value the Local's)
        const auto* local = ir::get<ir::Local>(fixed->var);
        il::ILVariable* v = local != nullptr && local->sym ? l2.variable(local->sym.get()) : nullptr;
        il::ILInstruction* init = local != nullptr ? instruction_of(local->init) : nullptr;
        il::ILInstruction* body = instruction_of(fixed->body);
        if (v == nullptr || init == nullptr || body == nullptr) return nullptr;
        return from(translate_pinned_region(PinnedRegionFacts{&inst, v, init, body}));
    }
    if (const auto* attempt = ir::get<ir::Try>(&n)) {
        // using, try-finally, try-fault, try-catch (lift.cpp: by the resources and handlers the Try has)
        il::ILInstruction* body = instruction_of(attempt->body);
        if (body == nullptr) return nullptr;
        if (!attempt->resources.empty()) {
            const auto* resource = attempt->resources.size() == 1 ? ir::get<ir::Local>(attempt->resources.front()) : nullptr;
            auto* self = il::as<il::UsingInstruction>(&inst);
            il::ILVariable* v = resource != nullptr && resource->sym ? l2.variable(resource->sym.get()) : nullptr;
            il::ILInstruction* init = resource != nullptr ? instruction_of(resource->init) : nullptr;
            if (self == nullptr || v == nullptr || init == nullptr) return nullptr;
            const auto has = [&](std::string_view mod) { return std::find(resource->mods.begin(), resource->mods.end(), mod) != resource->mods.end(); };
            return from(translate_using(UsingFacts{self, v, init, body, has("await"), has("ref_struct")}));
        }
        if (attempt->finally_body != nullptr || attempt->fault_body != nullptr) {
            const bool fault = attempt->finally_body == nullptr;
            il::ILInstruction* handler = instruction_of(fault ? attempt->fault_body : attempt->finally_body);
            if (handler == nullptr) return nullptr;
            const TryHandlerFacts facts{&inst, body, handler};
            return from(fault ? translate_try_fault(facts) : translate_try_finally(facts));
        }
        TryCatchFacts facts{&inst, body, {}};
        facts.handlers.reserve(attempt->catches.size());
        for (const ir::Node* c : attempt->catches) {
            const auto* katch = ir::get<ir::Catch>(c);
            auto* handler = il::as<il::TryCatchHandler>(l2.instruction(c));
            il::ILVariable* v = katch != nullptr && katch->sym ? l2.variable(katch->sym.get()) : nullptr;
            il::ILInstruction* filter = katch != nullptr && handler != nullptr ? instruction_of(katch->filter, handler) : nullptr;
            il::ILInstruction* handler_body = katch != nullptr && handler != nullptr ? instruction_of(katch->body, handler) : nullptr;
            if (handler == nullptr || v == nullptr || filter == nullptr || handler_body == nullptr) return nullptr;
            facts.handlers.push_back({handler, v, filter, handler_body});
        }
        return from(translate_try_catch(facts));
    }
    // a container's block: the Labeled, its statements the Block in it
    const ir::Node* block_node = &n;
    if (const auto* labeled = ir::get<ir::Labeled>(&n)) {
        const auto* body = std::get_if<ir::Node*>(&labeled->body);
        block_node = body != nullptr ? *body : nullptr;
    }
    if (ir::is<ir::Block>(block_node) || (ir::is<ir::BlockExpr>(block_node) && !ir::get<ir::BlockExpr>(block_node)->role)) {
        // a control-flow block (lift.cpp: a Block, or a BlockExpr without a role whose result is the final instruction); a block of
        // another kind is an expression
        auto* block = il::as<il::Block>(&inst);
        if (block == nullptr) return nullptr;
        BlockFacts facts{block, nullptr};
        if (const auto* e = ir::get<ir::BlockExpr>(block_node); e != nullptr && e->result != nullptr && !ir::is<ir::Empty>(e->result)) {
            facts.final_instruction = l2.instruction(e->result);
            if (facts.final_instruction == nullptr) return nullptr;
        }
        return from(translate_block(facts));
    }
    if (const auto* region = ir::get<ir::Region>(&n)) {
        // a container (lift.cpp: a Region of Labeled blocks, the first the entry): a loop when it is one whose entry is jumped to again
        // (the region's own entry counts one), a switch when its entry holds a switch alone, else a block of statements
        auto* container = il::as<il::BlockContainer>(&inst);
        const auto* entry = region->blocks.empty() ? nullptr : ir::get<ir::Labeled>(region->blocks.front());
        if (container == nullptr || entry == nullptr || !entry->label) return nullptr;
        BlockContainerFacts facts{container, region->kind != ir::RegionKind::Normal && l2.index().incoming(*entry->label) > 1, nullptr};
        const ir::Node* entry_body = std::get_if<ir::Node*>(&entry->body) != nullptr ? std::get<ir::Node*>(entry->body) : nullptr;
        const std::vector<ir::Node*>* stmts = nullptr;
        if (const auto* b = ir::get<ir::Block>(entry_body)) stmts = &b->stmts;
        else if (const auto* e = ir::get<ir::BlockExpr>(entry_body)) stmts = &e->stmts;
        if (stmts == nullptr) return nullptr;
        if (stmts->size() == 1 && ir::is<ir::Switch>(stmts->front())) {
            facts.switch_inst = il::as<il::SwitchInstruction>(l2.instruction(stmts->front()));
            if (facts.switch_inst == nullptr) return nullptr;
            facts.switch_facts = l2.switch_facts(*stmts->front(), *facts.switch_inst);
            if (!facts.switch_facts) return nullptr;
        }
        return from(translate_block_container(facts));
    }
    // An expression in a statement place (the kinds the visitor gives an ExpressionStatement: stloc and stobj without a top-level ref).
    if (const auto* assign = ir::get<ir::Assign>(&n)) {
        const auto* deref = ir::get<ir::Unary>(assign->target);
        const bool plain = !assign->op && !assign->eval && !assign->sym && (ir::is<ir::Name>(assign->target) || (deref != nullptr && deref->op == "*"));
        return from(translate_expression_statement(inst, plain));
    }
    // isinst is an `is`; initblk, cpblk and ckfinite have statements of their own (lift.cpp: the intrinsics' operands in order)
    if (const auto* cast = ir::get<ir::Cast>(&n); cast != nullptr && cast->conv == ir::ConvKind::TryCast) {
        il::ILInstruction* argument = instruction_of(cast->x);
        const IType* type = translate::frontends::cil::clr_type(n.type);
        if (argument == nullptr || type == nullptr) return nullptr;
        return from(translate_is_statement(TypedUnaryFacts{&inst, argument, type}));
    }
    if (const auto* intrinsic = ir::get<ir::Intrinsic>(&n)) {
        const bool copy = intrinsic->name == "mem.copy", fill = intrinsic->name == "mem.fill", finite = intrinsic->name == "cil.ckfinite";
        if (copy || fill || finite) {
            std::vector<il::ILInstruction*> args;
            for (const ir::Node* a : intrinsic->args) {
                il::ILInstruction* x = instruction_of(a);
                if (x == nullptr) return nullptr;
                args.push_back(x);
            }
            if (args.size() != (finite ? 1u : 3u)) return nullptr;
            if (finite) return from(translate_ckfinite(MemoryBlockFacts{&inst, args[0], nullptr, nullptr, 0}));
            const MemoryBlockFacts facts{&inst, args[0], args[1], args[2], translate::frontends::cil::unaligned_of(n)};
            return from(copy ? translate_cpblk(facts) : translate_initblk(facts));
        }
    }
    if (ir::category(n) == ir::Category::Expression || ir::is<ir::Destructure>(&n)) return from(translate_expression_statement(inst, false));
    return nullptr;
}

TranslatedStatement StatementBuilder::translate_expression_statement(il::ILInstruction& inst, bool strip_ref) {
    TranslatedExpression expr = expr_builder().translate(&inst);
    // strip top-level ref on ref re-assignment
    if (strip_ref) {
        if (auto* dir_expr = syntax::as<syntax::DirectionExpression>(expr.expression)) expr = expr.unwrap_child(dir_expr->expression());
    }
    return with_il_instruction(make<syntax::ExpressionStatement>(expr), &inst);
}

syntax::BlockStatement* StatementBuilder::convert_as_block(il::ILInstruction* inst) {
    Statement* stmt = with_il_instruction(convert(inst), inst);
    auto* block = syntax::as<syntax::BlockStatement>(stmt);
    return block != nullptr ? block : block_of(stmt);
}

TranslatedStatement StatementBuilder::default_visit(il::ILInstruction& inst) {
    return with_il_instruction(make<syntax::ExpressionStatement>(expr_builder().translate(&inst)), &inst);
}

TranslatedStatement StatementBuilder::visit_is_inst(il::IsInst& inst) { return translate_is_statement(TypedUnaryFacts::of(inst)); }

TranslatedStatement StatementBuilder::translate_is_statement(const TypedUnaryFacts& inst) {
    // isinst on top-level (unused result) can be translated in general (even for value types) by using "is" instead of "as". This can
    // happen when the result of "expr is T" is unused and the C# compiler optimizes away the null check portion of the "is" operator.
    TranslatedExpression arg = expr_builder().translate(inst.argument());
    arg = ExpressionBuilder::unwrap_boxing_conversion(arg);
    AstType* type = expr_builder().convert_type(inst.type());
    auto* is_expr = make<syntax::IsExpression>(arg, type);
    TranslatedExpression is_translated =
        with_rr(is_expr, make<ResolveResult>(decompilation_context_->compilation().find_type(KnownTypeCode::Boolean))).with_il_instruction(inst.self);
    return with_il_instruction(make<syntax::ExpressionStatement>(is_translated), inst.self);
}

TranslatedStatement StatementBuilder::visit_st_loc(il::StLoc& inst) {
    TranslatedExpression expr = expr_builder().translate(&inst);
    // strip top-level ref on ref re-assignment
    if (auto* dir_expr = syntax::as<syntax::DirectionExpression>(expr.expression)) expr = expr.unwrap_child(dir_expr->expression());
    return with_il_instruction(make<syntax::ExpressionStatement>(expr), &inst);
}

TranslatedStatement StatementBuilder::visit_st_obj(il::StObj& inst) {
    TranslatedExpression expr = expr_builder().translate(&inst);
    // strip top-level ref on ref re-assignment
    if (auto* dir_expr = syntax::as<syntax::DirectionExpression>(expr.expression)) expr = expr.unwrap_child(dir_expr->expression());
    return with_il_instruction(make<syntax::ExpressionStatement>(expr), &inst);
}

TranslatedStatement StatementBuilder::visit_nop(il::Nop& inst) {
    auto* stmt = make<syntax::EmptyStatement>();
    if (!inst.comment.empty()) stmt->add_trailing_trivia(make<syntax::Comment>(inst.comment));
    return with_il_instruction(stmt, &inst);
}

TranslatedStatement StatementBuilder::visit_if_instruction(il::IfInstruction& inst) {
    TranslatedExpression condition = expr_builder().translate_condition(inst.condition());
    Statement* true_statement = convert(inst.true_inst());
    Statement* false_statement = inst.false_inst()->op_code == il::OpCode::Nop ? nullptr : convert(inst.false_inst());
    return with_il_instruction(make<syntax::IfElseStatement>(condition, true_statement, false_statement), &inst);
}

// ---- switch ----------------------------------------------------------------------------------------------------------------------

std::vector<ConstantResolveResult*> StatementBuilder::create_typed_case_label(std::int64_t i, const IType* type,
                                                                              const std::vector<std::pair<std::optional<std::string>, int>>* map) {
    ConstantValue value;
    // unpack nullable type, if necessary: we need to do this in all cases, because there are nullable bools and enum types as well.
    type = typesystem::nullable_type::get_underlying_type(type);

    if (typesystem::is_known_type(type, KnownTypeCode::Boolean)) {
        value = ConstantValue(std::in_place_type<bool>, i != 0);
    } else if (map != nullptr) {
        assert(typesystem::is_known_type(type, KnownTypeCode::String));
        std::vector<ConstantResolveResult*> result;
        for (const auto& entry : *map) {
            if (entry.second == i) result.push_back(make<ConstantResolveResult>(type, string_key_value(entry.first)));
        }
        return result;
    } else if (type->kind() == TypeKind::Enum) {
        const IType* enum_type = type->get_definition()->enum_underlying_type();
        KnownTypeCode type_code = primitive_type_code(enum_type);
        if (type_code != KnownTypeCode::None) {
            value = util::csharp_primitive_cast(type_code, int64_value(i), false);
        } else {
            value = int64_value(i);
        }
    } else {
        KnownTypeCode type_code = primitive_type_code(type);
        if (type_code != KnownTypeCode::None) {
            value = util::csharp_primitive_cast(type_code, int64_value(i), false);
        } else {
            value = int64_value(i);
        }
    }
    return {make<ConstantResolveResult>(type, value)};
}

TranslatedStatement StatementBuilder::visit_switch_instruction(il::SwitchInstruction& inst) {
    return translate_switch_statement(SwitchFacts::of(inst));
}

TranslatedStatement StatementBuilder::translate_switch_statement(const SwitchFacts& inst) {
    return with_il_instruction(translate_switch(nullptr, &inst), inst.self);
}

syntax::SwitchStatement* StatementBuilder::translate_switch(il::BlockContainer* switch_container, const SwitchFacts* inst) {
    il::BlockContainer* old_break_target = break_target_;
    break_target_ = switch_container;  // 'break' within a switch would only leave the switch
    auto old_case_label_mapping = std::move(case_label_mapping_);
    case_label_mapping_.emplace();

    auto [value, type, str_to_int] = expr_builder().translate_switch_value(*inst, false);

    const SwitchSectionFacts* default_section = inst->get_default_section();

    auto* stmt = make<syntax::SwitchStatement>();
    stmt->set_expression(value);
    std::vector<std::pair<const SwitchSectionFacts*, syntax::SwitchSection*>> translation_dictionary;
    // whether the branch can be translated as the case's own block: all branches to the block are in the switch container
    auto can_inline_target_block = [&](il::Branch* br) {
        if (br->target_container() != switch_container) return false;
        for (il::ILInstruction* d : switch_container->descendants()) {
            auto* b = il::as<il::Branch>(d);
            if (b != nullptr && b->target_block() == br->target_block() && il::BlockContainer::find_closest_switch_container(b) != switch_container)
                return false;
        }
        return true;
    };
    // initialize C# switch sections.
    for (const SwitchSectionFacts* section : inst->sections()) {
        // This is used in the block-label mapping.
        ConstantResolveResult* first_value_resolve_result;
        auto* ast_section = make<syntax::SwitchSection>();
        // Create case labels:
        if (section == default_section) {
            ast_section->case_labels().add(make<syntax::CaseLabel>());
            first_value_resolve_result = nullptr;
        } else {
            std::vector<ConstantResolveResult*> values;
            const auto* map = str_to_int != nullptr ? &str_to_int->map() : nullptr;
            for (std::int64_t label_value : section->labels().values()) {
                for (ConstantResolveResult* label : create_typed_case_label(label_value, type, map)) values.push_back(label);
            }
            if (section->has_null_label()) {
                ast_section->case_labels().add(make<syntax::CaseLabel>(make<syntax::NullReferenceExpression>()));
                first_value_resolve_result = make<ConstantResolveResult>(&SpecialType::null_type(), ConstantValue());
            } else {
                assert(!values.empty());
                first_value_resolve_result = values[0];
            }
            for (ConstantResolveResult* label : values) {
                ast_section->case_labels().add(make<syntax::CaseLabel>(expr_builder().convert_constant_value(label, true)));
            }
        }
        if (auto* br = il::as<il::Branch>(section->body())) {
            // we can only inline the block, if all branches are in the switchContainer.
            if (can_inline_target_block(br)) {
                if (!case_label_mapping_->emplace(br->target_block(), first_value_resolve_result).second) throw std::invalid_argument("duplicate block");
            }
        }
        translation_dictionary.emplace_back(section, ast_section);
        stmt->switch_sections().add(ast_section);
    }
    for (const SwitchSectionFacts* section : inst->sections()) {
        syntax::SwitchSection* ast_section = nullptr;
        for (const auto& entry : translation_dictionary) {
            if (entry.first == section) ast_section = entry.second;
        }
        if (auto* br = il::as<il::Branch>(section->body())) {
            // we can only inline the block, if all branches are in the switchContainer.
            if (can_inline_target_block(br))
                convert_switch_section_body(ast_section, br->target_block());
            else
                convert_switch_section_body(ast_section, section->body());
            continue;
        }
        if (auto* leave = il::as<il::Leave>(section->body())) {
            if (ast_section->case_labels().count() == 1 && ast_section->case_labels()[0]->expression() == nullptr &&
                leave->target_container() == switch_container) {
                stmt->switch_sections().remove(ast_section);
                continue;
            }
        }
        convert_switch_section_body(ast_section, section->body());
    }
    if (switch_container != nullptr && stmt->switch_sections().count() > 0) {
        // Translate any remaining blocks:
        auto& last_section_statements = stmt->switch_sections().last_or_null()->statements();
        for (int block_index = 1; block_index < switch_container->blocks.count(); block_index++) {
            il::Block* block = switch_container->blocks[block_index];
            if (case_label_mapping_->contains(block)) continue;
            last_section_statements.add(make<syntax::LabelStatement>(ensure_unique_label(block)));
            std::vector<il::ILInstruction*> nested_instructions = block->instructions.items();
            for (il::ILInstruction* nested_inst : nested_instructions) {
                Statement* nested_stmt = convert(nested_inst);
                if (auto* b = syntax::as<syntax::BlockStatement>(nested_stmt)) {
                    for (Statement* nested : b->statements().to_vector()) last_section_statements.add(syntax::detach(nested));
                } else {
                    last_section_statements.add(nested_stmt);
                }
            }
            assert(block->final_instruction()->op_code == il::OpCode::Nop);
        }
        auto end_label = end_container_labels_.find(switch_container);
        if (end_label != end_container_labels_.end()) {
            last_section_statements.add(make<syntax::LabelStatement>(end_label->second));
            last_section_statements.add(make<syntax::BreakStatement>());
        }
    }

    break_target_ = old_break_target;
    case_label_mapping_ = std::move(old_case_label_mapping);
    return stmt;
}

void StatementBuilder::convert_switch_section_body(syntax::SwitchSection* ast_section, il::ILInstruction* body_inst) {
    Statement* body = convert(body_inst);
    ast_section->statements().add(body);
    if (!body_inst->has_flag(il::InstructionFlags::EndPointUnreachable)) {
        // we need to insert 'break;'
        auto* block = syntax::as<syntax::BlockStatement>(body);
        if (block != nullptr) {
            block->add(static_cast<Statement*>(make<syntax::BreakStatement>()));
        } else {
            ast_section->statements().add(make<syntax::BreakStatement>());
        }
    }
}

// ---- branches, leaves, jumps -----------------------------------------------------------------------------------------------------

TranslatedStatement StatementBuilder::visit_branch(il::Branch& inst) { return translate_branch(BranchFacts::of(inst)); }

TranslatedStatement StatementBuilder::translate_branch(const BranchFacts& inst) {
    if (inst.target_block() == continue_target_) {
        continue_count_++;
        return with_il_instruction(make<syntax::ContinueStatement>(), inst.self);
    }
    if (case_label_mapping_.has_value()) {
        auto found = case_label_mapping_->find(inst.target_block());
        if (found != case_label_mapping_->end()) {
            ConstantResolveResult* label = found->second;
            if (label == nullptr) return with_il_instruction(make<syntax::GotoDefaultStatement>(), inst.self);
            auto* goto_case = make<syntax::GotoCaseStatement>();
            goto_case->set_label_expression(expr_builder().convert_constant_value(label, true));
            return with_il_instruction(goto_case, inst.self);
        }
    }
    return with_il_instruction(make<syntax::GotoStatement>(ensure_unique_label(inst.target_block())), inst.self);
}

TranslatedStatement StatementBuilder::visit_leave(il::Leave& inst) { return translate_leave(LeaveFacts::of(inst)); }

TranslatedStatement StatementBuilder::translate_leave(const LeaveFacts& inst) {
    if (inst.target_container() == break_target_) return with_il_instruction(make<syntax::BreakStatement>(), inst.self);
    if (inst.target_container() == current_return_container) {
        if (current_is_iterator) {
            return with_il_instruction(make<syntax::YieldBreakStatement>(), inst.self);
        } else if (inst.has_value()) {
            bool is_lambda_or_expr_tree =
                current_function_->kind() == il::ILFunctionKind::ExpressionTree || current_function_->kind() == il::ILFunctionKind::Delegate;
            TranslatedExpression expr =
                expr_builder().translate(inst.value(), current_result_type).convert_to(current_result_type, expr_builder(), false, true);
            auto is_possible_loss_of_type_information = [](const IType* given_type, const IType* expected_type) {
                if (contains_anonymous_type(expected_type)) return false;
                if (typesystem::NormalizeTypeVisitor::ignore_nullability().equivalent_types(given_type, expected_type)) return false;
                if (const auto* tuple = dynamic_cast<const typesystem::TupleType*>(expected_type); tuple != nullptr && !tuple->element_names().empty())
                    return true;
                if (expected_type == &SpecialType::dynamic()) return true;
                if (given_type == &SpecialType::null_type()) return true;
                return false;
            };
            if (is_lambda_or_expr_tree && is_possible_loss_of_type_information(expr.type(), current_result_type)) {
                auto* cast = make<syntax::CastExpression>(expr_builder().convert_type(current_result_type), expr);
                expr = with_rr(cast, make<ConversionResolveResult>(current_result_type, expr.resolve_result, semantics::Conversion::identity_conversion()))
                           .without_il_instruction();
            }
            return with_il_instruction(make<syntax::ReturnStatement>(expr), inst.self);
        } else {
            return with_il_instruction(make<syntax::ReturnStatement>(), inst.self);
        }
    }
    std::string label;
    auto existing = end_container_labels_.find(inst.target_container());
    if (existing == end_container_labels_.end()) {
        label = "end_" + inst.target_label();
        auto duplicate = duplicate_labels_.find(label);
        if (duplicate == duplicate_labels_.end()) {
            duplicate_labels_.emplace(label, 1);
        } else {
            int count = duplicate->second;
            duplicate->second++;
            label += "_" + std::to_string(count + 1);
        }
        end_container_labels_.emplace(inst.target_container(), label);
    } else {
        label = existing->second;
    }
    return with_il_instruction(make<syntax::GotoStatement>(label), inst.self);
}

TranslatedStatement StatementBuilder::visit_throw(il::Throw& inst) {
    TranslatedExpression argument = expr_builder().translate(inst.argument());
    return with_il_instruction(make<syntax::ThrowStatement>(argument), &inst);
}

TranslatedStatement StatementBuilder::visit_rethrow(il::Rethrow& inst) { return with_il_instruction(make<syntax::ThrowStatement>(), &inst); }

TranslatedStatement StatementBuilder::visit_yield_return(il::YieldReturn& inst) { return translate_yield_return(YieldReturnFacts::of(inst)); }

TranslatedStatement StatementBuilder::translate_yield_return(const YieldReturnFacts& inst) {
    const IType* element_type = current_function_->async_return_type;
    if (element_type == nullptr) {
        std::optional<bool> is_generic;
        element_type = typesystem::get_element_type_from_ienumerable(current_function_->return_type, *type_system_, true, is_generic);
    }
    TranslatedExpression expr = expr_builder().translate(inst.value(), element_type).convert_to(element_type, expr_builder(), false, true);
    auto* stmt = make<syntax::YieldReturnStatement>();
    stmt->set_expression(expr);
    return with_il_instruction(stmt, inst.self);
}

// ---- try, lock --------------------------------------------------------------------------------------------------------------------

syntax::TryCatchStatement* StatementBuilder::make_try_catch(il::ILInstruction* try_block) {
    Statement* try_block_converted = convert(try_block);
    auto* try_catch = syntax::as<syntax::TryCatchStatement>(try_block_converted);
    if (try_catch != nullptr && try_catch->finally_block() == nullptr) return try_catch;  // extend existing try-catch
    try_catch = make<syntax::TryCatchStatement>();
    auto* block = syntax::as<syntax::BlockStatement>(try_block_converted);
    try_catch->set_try_block(block != nullptr ? block : block_of(try_block_converted));
    return try_catch;
}

TranslatedStatement StatementBuilder::visit_try_catch(il::TryCatch& inst) { return translate_try_catch(TryCatchFacts::of(inst)); }

TranslatedStatement StatementBuilder::translate_try_catch(const TryCatchFacts& inst) {
    auto* try_catch = make<syntax::TryCatchStatement>();
    try_catch->set_try_block(convert_as_block(inst.try_block()));
    for (const TryCatchFacts::Handler& handler : inst.handlers) {
        auto* catch_clause = make<syntax::CatchClause>();
        catch_clause->add_annotation(handler.self);
        il::ILVariable* v = handler.variable();
        if (v != nullptr) {
            catch_clause->add_annotation(make<ILVariableResolveResult>(v, v->type()));
            if (v->store_count() > 1 || v->load_count() > 0 || v->address_count() > 0) {
                catch_clause->set_variable_name(v->name.value_or(std::string()));
                catch_clause->set_type(expr_builder().convert_type(v->type()));
            } else if (!typesystem::is_known_type(v->type(), KnownTypeCode::Object)) {
                catch_clause->set_type(expr_builder().convert_type(v->type()));
            }
        }
        if (!handler.filter()->match_ldc_i4(1)) catch_clause->set_condition(expr_builder().translate_condition(handler.filter()));
        catch_clause->set_body(convert_as_block(handler.body()));
        try_catch->catch_clauses().add(catch_clause);
    }
    return with_il_instruction(try_catch, inst.self);
}

TranslatedStatement StatementBuilder::visit_try_finally(il::TryFinally& inst) { return translate_try_finally(TryHandlerFacts::of(inst)); }

TranslatedStatement StatementBuilder::translate_try_finally(const TryHandlerFacts& inst) {
    syntax::TryCatchStatement* try_catch = make_try_catch(inst.try_block());
    try_catch->set_finally_block(convert_as_block(inst.finally_block()));
    return with_il_instruction(try_catch, inst.self);
}

TranslatedStatement StatementBuilder::visit_try_fault(il::TryFault& inst) { return translate_try_fault(TryHandlerFacts::of(inst)); }

TranslatedStatement StatementBuilder::translate_try_fault(const TryHandlerFacts& inst) {
    auto* try_catch = make<syntax::TryCatchStatement>();
    try_catch->set_try_block(convert_as_block(inst.try_block()));
    syntax::BlockStatement* fault_block = convert_as_block(inst.fault_block());
    syntax::EmptyStatement* try_fault_statement = comment_statement("try-fault");
    fault_block->statements().insert_before(fault_block->statements().first_or_null(), try_fault_statement);
    fault_block->add(static_cast<Statement*>(make<syntax::ThrowStatement>()));
    auto* catch_clause = make<syntax::CatchClause>();
    catch_clause->set_body(fault_block);
    try_catch->catch_clauses().add(catch_clause);
    return with_il_instruction(try_catch, inst.self);
}

TranslatedStatement StatementBuilder::visit_lock_instruction(il::LockInstruction& inst) { return translate_lock(LockFacts::of(inst)); }

TranslatedStatement StatementBuilder::translate_lock(const LockFacts& inst) {
    auto* stmt = make<syntax::LockStatement>();
    stmt->set_expression(expr_builder().translate(inst.on_expression()));
    stmt->set_embedded_statement(convert_as_block(inst.body()));
    return with_il_instruction(stmt, inst.self);
}

// ---- using and foreach -----------------------------------------------------------------------------------------------------------

TranslatedStatement StatementBuilder::visit_using_instruction(il::UsingInstruction& inst) { return translate_using(UsingFacts::of(inst)); }

TranslatedStatement StatementBuilder::translate_using(const UsingFacts& inst) {
    Expression* resource = expr_builder().translate(inst.resource_expression()).expression;
    Statement* transformed = transform_to_foreach(inst.self, resource);
    if (transformed != nullptr) return with_il_instruction(transformed, inst.self);
    AstNode* using_init = resource;
    il::ILVariable* var = inst.variable();
    KnownTypeCode known_type_code;
    const IType* dispose_type;
    std::string dispose_type_method_name;
    const typesystem::ICompilation& compilation = decompilation_context_->compilation();
    if (inst.is_async()) {
        known_type_code = KnownTypeCode::IAsyncDisposable;
        dispose_type = compilation.find_type(KnownTypeCode::IAsyncDisposable);
        dispose_type_method_name = "DisposeAsync";
    } else {
        known_type_code = KnownTypeCode::IDisposable;
        dispose_type = compilation.find_type(KnownTypeCode::IDisposable);
        dispose_type_method_name = "Dispose";
    }
    auto is_valid_in_csharp = [&](const UsingFacts* using_inst, KnownTypeCode code) {
        if (using_inst->resource_expression()->match_ld_null()) return true;
        if (using_inst->is_ref_struct()) return true;
        for (const IType* b : typesystem::get_all_base_types(typesystem::nullable_type::get_underlying_type(var->type()))) {
            if (typesystem::is_known_type(b, code)) return true;
        }
        return false;
    };
    if (!is_valid_in_csharp(&inst, known_type_code)) {
        assert(var->kind() == il::VariableKind::UsingLocal);
        var->set_kind(il::VariableKind::Local);
        il::ILVariable* dispose_variable = current_function_->register_variable(
            il::VariableKind::Local, dispose_type,
            il::transforms::AssignVariableNames::generate_variable_name(current_function_, dispose_type, decompile_run.using_scope));
        if (L2Body* l2 = expr_builder().l2) {
            l2->variable_changed(var);
            l2->variable_registered(current_function_, dispose_variable);
        }
        Expression* dispose_invocation = make<syntax::InvocationExpression>(
            make<syntax::MemberReferenceExpression>(expr_builder().convert_variable(dispose_variable).expression, dispose_type_method_name));
        if (inst.is_async()) {
            dispose_invocation = make<syntax::UnaryOperatorExpression>(syntax::UnaryOperatorType::Await, dispose_invocation);
        }
        auto* result = make<syntax::BlockStatement>();
        result->add(static_cast<Statement*>(make<syntax::ExpressionStatement>(make<syntax::AssignmentExpression>(
            expr_builder().convert_variable(var).expression, syntax::detach(resource)))));
        auto* try_catch = make<syntax::TryCatchStatement>();
        try_catch->set_try_block(convert_as_block(inst.body()));
        auto* finally_block = make<syntax::BlockStatement>();
        {
            Expression* dispose_target = expr_builder().convert_variable(dispose_variable).expression;
            Expression* source = expr_builder().convert_variable(var).expression;
            AstType* dispose_ast_type = expr_builder().convert_type(dispose_type);
            finally_block->add(static_cast<Statement*>(
                make<syntax::ExpressionStatement>(make<syntax::AssignmentExpression>(dispose_target, make<syntax::AsExpression>(source, dispose_ast_type)))));
            auto* if_else = make<syntax::IfElseStatement>();
            if_else->set_condition(make<syntax::BinaryOperatorExpression>(expr_builder().convert_variable(dispose_variable).expression,
                                                                            syntax::BinaryOperatorType::InEquality, make<syntax::NullReferenceExpression>()));
            if_else->set_true_statement(make<syntax::ExpressionStatement>(dispose_invocation));
            finally_block->add(static_cast<Statement*>(if_else));
        }
        try_catch->set_finally_block(finally_block);
        result->add(static_cast<Statement*>(try_catch));
        return with_il_instruction(result, inst.self);
    } else {
        if (var->load_count() > 0 || var->address_count() > 0) {
            AstType* type = settings_->anonymous_types && contains_anonymous_type(var->type()) ? static_cast<AstType*>(make<syntax::SimpleType>("var"))
                                                                                                 : expr_builder().convert_type(var->type());
            auto* vds = make<syntax::VariableDeclarationStatement>(type, var->name.value_or(std::string()), resource);
            vds->variables().first_or_null()->add_annotation(make<ILVariableResolveResult>(var, var->type()));
            using_init = vds;
        }
        auto* using_stmt = make<syntax::UsingStatement>();
        using_stmt->set_resource_acquisition(using_init);
        using_stmt->set_is_async(inst.is_async());
        using_stmt->set_embedded_statement(convert_as_block(inst.body()));
        return with_il_instruction(using_stmt, inst.self);
    }
}

bool StatementBuilder::match_get_enumerator_pattern(Expression* resource, Match* m, bool* is_async) {
    namespace pm = syntax::pattern_matching;
    *is_async = false;
    // The patterns are made on every call: syntax nodes live in the arena of the run.
    // collection.GetEnumerator() / collection.GetAsyncEnumerator()
    auto* get_enumerator_choice = make<pm::Choice>();
    get_enumerator_choice->add(make<syntax::MemberReferenceExpression>(pm::to_expression(make<pm::AnyNode>("collection")), "GetEnumerator"));
    get_enumerator_choice->add(make<syntax::MemberReferenceExpression>(pm::to_expression(make<pm::AnyNode>("collection")), "GetAsyncEnumerator"));
    Expression* get_enumerator_pattern = make<syntax::InvocationExpression>(pm::to_expression(get_enumerator_choice));
    if (settings_->extension_methods && settings_->for_each_with_get_enumerator_extension) {
        // Check if the resource expression matches the GetEnumerator pattern ...
        *m = pm::match(get_enumerator_pattern, resource);
        if (!m->success()) {
            // ... or the extension GetEnumeratorPattern.
            auto* extension_choice = make<pm::Choice>();
            extension_choice->add(make<syntax::MemberReferenceExpression>(pm::to_expression(make<pm::AnyNode>("type")), "GetEnumerator"));
            extension_choice->add(make<syntax::MemberReferenceExpression>(pm::to_expression(make<pm::AnyNode>("type")), "GetAsyncEnumerator"));
            Expression* extension_get_enumerator_pattern = make<syntax::InvocationExpression>(
                pm::to_expression(extension_choice), std::vector<Expression*>{pm::to_expression(make<pm::AnyNode>("collection"))});
            *m = pm::match(extension_get_enumerator_pattern, resource);
            if (!m->success()) return false;
            // Validate that the invocation is an extension method invocation.
            const auto* method = dynamic_cast<const typesystem::IMethod*>(get_symbol(resource));
            if (!(method != nullptr && expr_builder().resolver->can_transform_to_extension_method_call(method, true))) return false;
        }
    } else {
        // Check if the resource expression matches the GetEnumerator pattern.
        *m = pm::match(get_enumerator_pattern, resource);
        if (!m->success()) return false;
    }
    *is_async = static_cast<syntax::MemberReferenceExpression*>(static_cast<syntax::InvocationExpression*>(resource)->target())->member_name() ==
                "GetAsyncEnumerator";
    return true;
}

Statement* StatementBuilder::transform_to_foreach(il::UsingInstruction* inst, Expression* resource) {
    if (!settings_->for_each_statement) return nullptr;
    Match m;
    bool is_async;
    if (!match_get_enumerator_pattern(resource, &m, &is_async)) return nullptr;
    // The using body must be a BlockContainer.
    auto* container = il::as<il::BlockContainer>(inst->body());
    if (container == nullptr) return nullptr;
    if (is_async != inst->is_async()) return nullptr;
    // If there's another BlockContainer nested in this container and it only has one child block, unwrap it. If there's an extra leave
    // inside the block, extract it into optionalReturnAfterLoop.
    il::Leave* optional_leave_after_loop;
    il::BlockContainer* loop_container = unwrap_nested_container_if_possible(container, &optional_leave_after_loop);
    // The using-variable is the enumerator.
    return transform_to_foreach(container, loop_container, optional_leave_after_loop, inst->variable(), is_async, m, inst->resource_expression());
}

// Transforms the bare pattern 'enumerator = collection.GetEnumerator(); while (enumerator.MoveNext()) { ... }' (without any
// using/try-finally) into a foreach statement. The compiler emits this shape when the enumerator type can never require disposal: a struct
// or a sealed class that does not implement IDisposable. The pattern spans two consecutive instructions; on success, i is advanced past
// the loop container.
Statement* StatementBuilder::transform_to_foreach_without_dispose(il::Block* block, int* i) {
    il::StLoc* store_inst = nullptr;
    il::BlockContainer* loop_container = nullptr;
    if (L2Body* l2 = expr_builder().l2) {
        const auto m = l2->match_store_then_while(*block, *i);
        if (!m) return nullptr;
        store_inst = m->first;
        loop_container = m->second;
    } else {
        store_inst = il::as<il::StLoc>(block->instructions[*i]);
        if (store_inst == nullptr || !(*i + 1 < block->instructions.count())) return nullptr;
        loop_container = il::as<il::BlockContainer>(block->instructions[*i + 1]);
        if (loop_container == nullptr || loop_container->kind() != il::ContainerKind::While) return nullptr;
    }
    Statement* transformed = transform_to_foreach_without_dispose(store_inst, loop_container);
    if (transformed == nullptr) return nullptr;
    (*i)++;
    return with_il_instruction(transformed, loop_container).statement;
}

Statement* StatementBuilder::transform_to_foreach_without_dispose(il::StLoc* store_inst, il::BlockContainer* loop_container) {
    if (!settings_->for_each_statement) return nullptr;
    il::ILVariable* enumerator_var = store_inst->variable();
    if (!(enumerator_var->kind() == il::VariableKind::Local || enumerator_var->kind() == il::VariableKind::StackSlot)) return nullptr;
    if (!enumerator_type_can_never_be_disposable(enumerator_var->type())) return nullptr;
    // The enumerator variable must not be used outside of the loop.
    if (!variable_is_only_used_in_block(store_inst, loop_container, loop_container)) return nullptr;
    Expression* resource = expr_builder().translate(store_inst->value()).expression;
    Match m;
    bool is_async;
    if (!match_get_enumerator_pattern(resource, &m, &is_async)) return nullptr;
    // Async enumerators always implement IAsyncDisposable, so they cannot occur in this pattern.
    if (is_async) return nullptr;
    return transform_to_foreach(loop_container, loop_container, nullptr, enumerator_var, false, m, store_inst->value());
}

bool StatementBuilder::enumerator_type_can_never_be_disposable(const IType* type) {
    const typesystem::ITypeDefinition* type_def = type->get_definition();
    if (type_def == nullptr) return false;
    switch (type_def->kind()) {
        case TypeKind::Struct:
            // A ref struct may use pattern-based disposal via a Dispose method.
            if (type_def->is_by_ref_like()) return false;
            break;
        case TypeKind::Class:
            // A non-sealed class would be enumerated with a 'finally { (enumerator as IDisposable)?.Dispose(); }' block instead.
            if (!type_def->is_sealed()) return false;
            break;
        default: return false;
    }
    for (const IType* t : typesystem::get_all_base_types(type)) {
        if (typesystem::is_known_type(t, KnownTypeCode::IDisposable)) return false;
    }
    return true;
}

Statement* StatementBuilder::transform_to_foreach(il::BlockContainer* container, il::BlockContainer* loop_container, il::Leave* optional_leave_after_loop,
                                                  il::ILVariable* enumerator_var, bool is_async, const Match& m, il::ILInstruction* resource_expression) {
    namespace pm = syntax::pattern_matching;
    // Detect whether we're dealing with a while loop with multiple embedded statements.
    L2Body* const body_l2 = expr_builder().l2;
    if ((body_l2 != nullptr ? body_l2->container_kind(*loop_container) : loop_container->kind()) != il::ContainerKind::While) return nullptr;
    il::ILInstruction* condition_inst;
    il::Block* body;
    if (!loop_container->match_condition_block(loop_container->entry_point(), &condition_inst, &body)) return nullptr;
    // The loop condition must be a call to enumerator.MoveNext()
    TranslatedExpression condition = expr_builder().translate_condition(condition_inst);
    // enumerator.MoveNext() / await enumerator.MoveNextAsync()
    auto* move_next_choice = make<pm::Choice>();
    move_next_choice->add(make<syntax::InvocationExpression>(make<syntax::MemberReferenceExpression>(
        pm::with_name(make<syntax::IdentifierExpression>(std::string(pm::Pattern::any_string)), "enumerator"), "MoveNext")));
    move_next_choice->add(make<syntax::UnaryOperatorExpression>(
        syntax::UnaryOperatorType::Await,
        make<syntax::InvocationExpression>(make<syntax::MemberReferenceExpression>(
            pm::with_name(make<syntax::IdentifierExpression>(std::string(pm::Pattern::any_string)), "enumerator"), "MoveNextAsync"))));
    Expression* move_next_condition_pattern = pm::to_expression(move_next_choice);
    Match m2 = pm::match(move_next_condition_pattern, condition.expression);
    if (!m2.success()) return nullptr;
    auto* await_condition = syntax::as<syntax::UnaryOperatorExpression>(condition.expression);
    if ((await_condition != nullptr && await_condition->operator_() == syntax::UnaryOperatorType::Await) != is_async) return nullptr;
    // Check enumerator variable references.
    il::ILVariable* enumerator_var2 = get_il_variable(m2.get<syntax::IdentifierExpression>("enumerator").front());
    if (enumerator_var2 != enumerator_var) return nullptr;
    // Detect which foreach-variable transformation is necessary/possible.
    il::CallInstruction* single_getter;
    il::ILVariable* foreach_variable;
    RequiredGetCurrentTransformation transformation =
        detect_get_current_transformation(container, body, loop_container, enumerator_var, condition_inst, &single_getter, &foreach_variable);
    if (transformation == RequiredGetCurrentTransformation::NoForeach) return nullptr;
    // Extract in-expression
    Expression* collection_expr = m.get<Expression>("collection").front();
    // Special case: foreach (var item in this) is decompiled as foreach (var item in base) but a base reference is not valid in this context.
    Expression* dynamic_expr;
    if (syntax::is<syntax::BaseReferenceExpression>(collection_expr)) {
        collection_expr = copy_annotations_from(make<syntax::ThisReferenceExpression>(), collection_expr);
    } else if (is_dynamic_cast_to_ienumerable(collection_expr, &dynamic_expr)) {
        collection_expr = syntax::detach(dynamic_expr);
    }
    // Handle explicit casts: this is the case if an explicit type different from the collection-item-type was used. For example:
    // foreach (ClassA item in nonGenericEnumerable)
    const IType* type = (body_l2 != nullptr ? body_l2->call_method(*single_getter) : single_getter->method)->return_type();
    il::ILInstruction* inst_to_replace = single_getter;
    bool use_var = false;
    std::optional<std::pair<il::ILInstruction*, const IType*>> cast;
    if (body_l2 != nullptr) {
        cast = body_l2->cast_around(*single_getter);
    } else if (auto* cc = il::as<il::CastClass>(single_getter->parent())) {
        cast = std::make_pair(static_cast<il::ILInstruction*>(cc), cc->type());
    } else if (auto* ua = il::as<il::UnboxAny>(single_getter->parent())) {
        cast = std::make_pair(static_cast<il::ILInstruction*>(ua), ua->type());
    }
    if (cast) {
        type = cast->second;
        inst_to_replace = cast->first;
    } else {
        int tuple_cardinality;
        if (typesystem::TupleType::is_tuple_compatible(type, tuple_cardinality)) {
            // foreach with get_Current returning a tuple type, let's check which type "var" would infer:
            semantics::ForEachResolveResult* foreach_rr = expr_builder().resolver->resolve_foreach(get_resolve_result(collection_expr));
            if (equal_erased_type(type, foreach_rr->element_type)) {
                type = foreach_rr->element_type;
                use_var = true;
            }
        }
    }

    syntax::VariableDesignation* designation = nullptr;
    const type_system::UsingScope* using_scope = decompile_run.using_scope;

    // Handle the required foreach-variable transformation:
    switch (transformation) {
        case RequiredGetCurrentTransformation::UseExistingVariable:
            if (foreach_variable->type()->kind() != TypeKind::Dynamic) foreach_variable->set_type(type);
            foreach_variable->set_kind(il::VariableKind::ForeachLocal);
            foreach_variable->name = il::transforms::AssignVariableNames::generate_foreach_variable_name(
                current_function_, collection_expr->annotation<il::ILInstruction>(), using_scope, foreach_variable);
            if (L2Body* l2 = expr_builder().l2) l2->variable_changed(foreach_variable);
            break;
        case RequiredGetCurrentTransformation::IntroduceNewVariable:
            foreach_variable = current_function_->register_variable(
                il::VariableKind::ForeachLocal, type,
                il::transforms::AssignVariableNames::generate_foreach_variable_name(current_function_, collection_expr->annotation<il::ILInstruction>(),
                                                                                    using_scope));
            if (L2Body* l2 = expr_builder().l2) l2->variable_registered(current_function_, foreach_variable);
            {
                auto* load = make<il::LdLoc>(foreach_variable);
                inst_to_replace->replace_with(load);
                if (L2Body* l2 = expr_builder().l2) l2->replaced(inst_to_replace, load);
            }
            body->instructions.insert(0, make<il::StLoc>(foreach_variable, inst_to_replace));
            if (L2Body* l2 = expr_builder().l2) l2->inserted(body, 0);
            break;
        case RequiredGetCurrentTransformation::IntroduceNewVariableAndLocalCopy: {
            foreach_variable = current_function_->register_variable(
                il::VariableKind::ForeachLocal, type,
                il::transforms::AssignVariableNames::generate_foreach_variable_name(current_function_, collection_expr->annotation<il::ILInstruction>(),
                                                                                    using_scope));
            il::ILVariable* local_copy_variable = current_function_->register_variable(
                il::VariableKind::Local, type,
                il::transforms::AssignVariableNames::generate_variable_name(current_function_, type, using_scope));
            L2Body* l2 = expr_builder().l2;
            if (l2 != nullptr) {
                l2->variable_registered(current_function_, foreach_variable);
                l2->variable_registered(current_function_, local_copy_variable);
            }
            {
                il::ILInstruction* parent = inst_to_replace->parent();
                auto* address = make<il::LdLoca>(local_copy_variable);
                parent->replace_with(address);
                if (l2 != nullptr) l2->replaced(parent, address);
            }
            body->instructions.insert(0, make<il::StLoc>(local_copy_variable, make<il::LdLoc>(foreach_variable)));
            if (l2 != nullptr) l2->inserted(body, 0);
            body->instructions.insert(0, make<il::StLoc>(foreach_variable, inst_to_replace));
            if (l2 != nullptr) l2->inserted(body, 0);
            break;
        }
        case RequiredGetCurrentTransformation::Deconstruction:
            use_var = true;
            designation = translate_deconstruction_designation(cast_inst<il::DeconstructInstruction>(body->instructions[0]), true, expr_builder().l2);
            break;
        case RequiredGetCurrentTransformation::NoForeach: break;
    }

    if (designation == nullptr) {
        auto* single = make<syntax::SingleVariableDesignation>(foreach_variable->name.value_or(std::string()));
        // Add the variable annotation for highlighting
        single->add_annotation(make<ILVariableResolveResult>(foreach_variable, foreach_variable->type()));
        designation = single;
    }

    // Convert the modified body to C# AST:
    syntax::BlockStatement* while_loop_block = convert_as_block(container);
    auto* while_loop = syntax::cast<syntax::WhileStatement>(while_loop_block->statements().first_or_null());
    auto* foreach_body = syntax::cast<syntax::BlockStatement>(syntax::detach(while_loop->embedded_statement()));

    // Remove the first statement, as it is the foreachVariable = enumerator.Current; statement.
    Statement* first_statement = foreach_body->statements().first_or_null();
    if (syntax::is<syntax::LabelStatement>(first_statement)) {
        // skip the entry-point label, if any; the assignment statement always follows it
        first_statement = syntax::get_next_statement(first_statement);
    }
    assert(syntax::is<syntax::ExpressionStatement>(first_statement));
    first_statement->remove();

    if (settings_->anonymous_types && contains_anonymous_type(type)) use_var = true;

    // Construct the foreach loop.
    auto* foreach_stmt = make<syntax::ForeachStatement>();
    foreach_stmt->set_is_async(is_async);
    foreach_stmt->set_variable_type(use_var ? static_cast<AstType*>(make<syntax::SimpleType>("var")) : expr_builder().convert_type(foreach_variable->type()));
    foreach_stmt->set_variable_designation(designation);
    foreach_stmt->set_in_expression(syntax::detach(collection_expr));
    foreach_stmt->set_embedded_statement(foreach_body);
    foreach_stmt->add_annotation(make<ForeachAnnotation>(resource_expression, condition_inst, single_getter));
    copy_annotations_from(foreach_stmt, while_loop);
    // If there was an optional return statement, return it as well. If there were labels or any other statements in the whileLoopBlock,
    // move them after the foreach loop.
    if (optional_leave_after_loop != nullptr || while_loop_block->statements().count() > 1) {
        auto* block = make<syntax::BlockStatement>();
        block->statements().add(foreach_stmt);
        if (optional_leave_after_loop != nullptr) block->statements().add(il::accept_visitor(*optional_leave_after_loop, *this));
        if (while_loop_block->statements().count() > 1) {
            std::vector<Statement*> remaining = while_loop_block->statements().to_vector();
            std::size_t pos = 1;
            // SkipWhile: leading statements annotated with the optional leave were already moved after the loop
            while (pos < remaining.size()) {
                bool annotated = false;
                for (il::ArenaObject* a : remaining[pos]->annotations()) {
                    if (a == static_cast<il::ArenaObject*>(optional_leave_after_loop)) annotated = true;
                }
                if (!annotated) break;
                pos++;
            }
            for (; pos < remaining.size(); pos++) block->statements().add(syntax::detach(remaining[pos]));
        }
        return block;
    }
    return foreach_stmt;
}

syntax::VariableDesignation* StatementBuilder::translate_deconstruction_designation(il::DeconstructInstruction* inst, bool is_foreach, L2Body* l2) {
    il::Block* assignments = inst->assignments();
    int assignment_pos = 0;

    std::function<syntax::VariableDesignation*(il::MatchInstruction*)> construct_designation = [&](il::MatchInstruction* match_instruction) {
        auto* designations = make<syntax::ParenthesizedVariableDesignation>();
        for (il::ILInstruction* sub_pattern_inst : match_instruction->sub_patterns()) {
            auto* sub_pattern = cast_inst<il::MatchInstruction>(sub_pattern_inst);
            if (sub_pattern->is_var()) {
                auto* designation = make<syntax::SingleVariableDesignation>();
                if (sub_pattern->has_designator()) {
                    il::ILVariable* v = cast_inst<il::StLoc>(assignments->instructions[assignment_pos])->variable();
                    if (is_foreach) {
                        v->set_kind(il::VariableKind::ForeachLocal);
                        if (l2 != nullptr) l2->variable_changed(v);
                    }
                    designation->set_identifier(v->name.value_or(std::string()));
                    designation->add_annotation(make<ILVariableResolveResult>(v));
                    assignment_pos++;
                } else {
                    designation->set_identifier("_");
                }
                designations->variable_designations().add(designation);
            } else {
                designations->variable_designations().add(construct_designation(sub_pattern));
            }
        }
        designations->add_annotation(match_instruction);
        return static_cast<syntax::VariableDesignation*>(designations);
    };
    return construct_designation(inst->pattern());
}

bool StatementBuilder::is_dynamic_cast_to_ienumerable(Expression* expr, Expression** dynamic_expr) {
    auto* cast = syntax::as<syntax::CastExpression>(expr);
    if (cast == nullptr) {
        *dynamic_expr = nullptr;
        return false;
    }
    *dynamic_expr = cast->expression();
    auto* crr = dynamic_cast<ConversionResolveResult*>(get_resolve_result(expr));
    if (crr == nullptr) return false;
    if (!typesystem::is_known_type(crr->type(), KnownTypeCode::IEnumerable)) return false;
    return crr->input->type()->kind() == TypeKind::Dynamic;
}

// Unwraps a nested BlockContainer, if container contains only a single block, and that single block contains only a BlockContainer followed
// by a Leave instruction. If the leave instruction is a return that carries a value, the container is unwrapped only if the value has no
// side-effects. Otherwise returns the unmodified container. If the leave is a return/break and has no side-effects, we can move the return
// out of the using-block and put it after the loop (optional_leave_inst), otherwise it is null.
il::BlockContainer* StatementBuilder::unwrap_nested_container_if_possible(il::BlockContainer* container, il::Leave** optional_leave_inst) {
    *optional_leave_inst = nullptr;
    // Check block structure:
    if (container->blocks.count() != 1) return container;
    il::Block* nested_block = container->blocks[0];
    if (nested_block->instructions.count() != 2) return container;
    auto* nested_container = il::as<il::BlockContainer>(nested_block->instructions[0]);
    auto* leave = il::as<il::Leave>(nested_block->instructions[1]);
    if (nested_container == nullptr || leave == nullptr) return container;
    // If the leave has no value, just unwrap the BlockContainer.
    if (leave->match_leave(container)) return nested_container;
    // If the leave is a return/break, we can move it out of the using-block and put it after the loop (but only if the value doesn't have
    // side-effects)
    if (il::SemanticHelper::is_pure(leave->value()->flags())) {
        *optional_leave_inst = leave;
        return nested_container;
    }
    return container;
}

// Determines whether enumerator is only used once inside loop_body for accessing the Current property. single_getter is the call
// instruction invoking Current's getter; foreach_variable the foreach variable, if a suitable was found (only assigned once, its
// assignment is the first statement in loop_body).
StatementBuilder::RequiredGetCurrentTransformation StatementBuilder::detect_get_current_transformation(
    il::BlockContainer* using_container, il::Block* loop_body, il::BlockContainer* loop_container, il::ILVariable* enumerator,
    il::ILInstruction* move_next_usage, il::CallInstruction** single_getter, il::ILVariable** foreach_variable) {
    *single_getter = nullptr;
    *foreach_variable = nullptr;
    std::vector<il::ILInstruction*> loads;
    for (il::LdLoc* ld : enumerator->load_instructions()) {
        if (!ld->is_descendant_of(move_next_usage)) loads.push_back(ld);
    }
    for (il::LdLoca* ld : enumerator->address_instructions()) {
        if (!ld->is_descendant_of(move_next_usage)) loads.push_back(ld);
    }
    // enumerator is used in multiple locations or not in conjunction with get_Current => no foreach
    if (loads.size() != 1 || !parent_is_current_getter(loads[0])) return RequiredGetCurrentTransformation::NoForeach;
    *single_getter = cast_inst<il::CallInstruction>(loads[0]->parent());
    // singleGetter is not part of the first instruction in body or cannot be uninlined => no foreach
    if (!((*single_getter)->is_descendant_of(loop_body->instructions[0]) &&
          il::transforms::ILInlining::can_uninline(*single_getter, loop_body->instructions[0]))) {
        return RequiredGetCurrentTransformation::NoForeach;
    }
    if (auto* deconstruction = il::as<il::DeconstructInstruction>(loop_body->instructions[0]);
        deconstruction != nullptr && can_be_deconstructed_in_foreach(deconstruction, *single_getter, using_container, loop_container)) {
        return RequiredGetCurrentTransformation::Deconstruction;
    }
    il::ILInstruction* inst = *single_getter;
    // in some cases, i.e. foreach variable with explicit type different from the collection-item-type, the result of call get_Current is
    // casted.
    while (il::is<il::UnboxAny>(inst->parent()) || il::is<il::CastClass>(inst->parent())) inst = inst->parent();
    // One variable was found.
    if (auto* stloc = il::as<il::StLoc>(inst->parent());
        stloc != nullptr && (stloc->variable()->kind() == il::VariableKind::Local || stloc->variable()->kind() == il::VariableKind::StackSlot)) {
        // Must be a plain assignment expression and variable must only be used in 'body' + only assigned once.
        if (stloc->parent() == loop_body && variable_is_only_used_in_block(stloc, using_container, loop_container)) {
            *foreach_variable = stloc->variable();
            return RequiredGetCurrentTransformation::UseExistingVariable;
        }
    }
    // In optimized Roslyn code it can happen that the foreach variable is referenced via addressof. We only do this unwrapping if where
    // dealing with a custom struct type.
    if (current_is_struct_setter_target(inst, *single_getter)) return RequiredGetCurrentTransformation::IntroduceNewVariableAndLocalCopy;
    // No suitable variable was found: we need a new one.
    return RequiredGetCurrentTransformation::IntroduceNewVariable;
}

bool StatementBuilder::can_be_deconstructed_in_foreach(il::DeconstructInstruction* deconstruction, il::ILInstruction* single_getter,
                                                       il::BlockContainer* using_container, il::BlockContainer* loop_container) {
    il::ILInstruction* tested_operand = deconstruction->pattern()->tested_operand();
    if (tested_operand != single_getter) {
        auto* address_of = il::as<il::AddressOf>(tested_operand);
        if (!(address_of != nullptr && address_of->value() == single_getter)) return false;
    }
    if (deconstruction->init.count() > 0) return false;
    if (deconstruction->conversions()->instructions.count() > 0) return false;
    const IType* operand_type = il::infer_type(single_getter, &decompilation_context_->compilation());
    const IType* expected_type = deconstruction->pattern()->variable()->type();
    if (!typesystem::NormalizeTypeVisitor::type_erasure().equivalent_types(operand_type, expected_type)) return false;
    std::unordered_set<il::ILVariable*, il::ILVariableHash, il::ILVariableEqualityComparer> used_variables;
    for (il::ILInstruction* item : deconstruction->assignments()->instructions) {
        il::ILVariable* v;
        il::ILInstruction* value;
        if (!item->match_st_loc(&v, &value)) return false;
        expected_type = cast_inst<il::LdLoc>(value)->variable()->type();
        if (!typesystem::NormalizeTypeVisitor::type_erasure().equivalent_types(v->type(), expected_type)) return false;
        if (!(v->kind() == il::VariableKind::StackSlot || v->kind() == il::VariableKind::Local)) return false;
        if (!variable_is_only_used_in_block(cast_inst<il::StLoc>(item), using_container, loop_container)) return false;
        if (!(v->capture_scope == nullptr || v->capture_scope == using_container)) return false;
        if (!used_variables.insert(v).second) return false;
    }
    return true;
}

// Determines whether store_inst.Variable is only assigned once and used only inside using_container. Loads by reference (ldloca) are only
// allowed in the context of this pointer in call instructions, or as target of ldobj. (This only applies to value types.)
bool StatementBuilder::variable_is_only_used_in_block(il::StLoc* store_inst, il::BlockContainer* using_container, il::BlockContainer* loop_container) {
    il::ILVariable* variable = store_inst->variable();
    for (il::LdLoc* ld : variable->load_instructions()) {
        if (!ld->is_descendant_of(using_container)) return false;
    }
    auto address_use_allowed = [&](il::LdLoca* la) {
        if (!la->is_descendant_of(using_container)) return false;
        if (il::transforms::ILInlining::is_used_as_this_pointer_in_call(la) && !is_target_of_setter_call(la, la->variable()->type())) return true;
        il::ILInstruction* current = la->parent();
        while (auto* next = il::as<il::LdFlda>(current)) current = next->parent();
        return il::is<il::LdObj>(current);
    };
    for (il::LdLoca* inst : variable->address_instructions()) {
        if (!address_use_allowed(inst)) return false;
    }
    for (il::IStoreInstruction* st : variable->store_instructions()) {
        if (st->as_instruction() != store_inst) return false;
    }
    if (!(variable->capture_scope == nullptr || variable->capture_scope == loop_container)) return false;
    return true;
}

// Returns true if single_getter is a value type and its address is used as setter target.
bool StatementBuilder::current_is_struct_setter_target(il::ILInstruction* inst, il::CallInstruction* single_getter) {
    auto* addr = il::as<il::AddressOf>(inst->parent());
    if (addr == nullptr) return false;
    return is_target_of_setter_call(addr, single_getter->method->return_type());
}

bool StatementBuilder::is_target_of_setter_call(il::ILInstruction* inst, const IType* target_type) {
    if (inst->child_index() != 0) return false;
    if (target_type->is_reference_type().value_or(false)) return false;
    switch (inst->parent()->op_code) {
        case il::OpCode::Call:
        case il::OpCode::CallVirt: {
            const typesystem::IMethod* target_method = cast_inst<il::CallInstruction>(inst->parent())->method;
            if (!target_method->is_accessor() || target_method->is_static()) return false;
            if (dynamic_cast<const typesystem::IProperty*>(target_method->accessor_owner()) != nullptr)
                return target_method->accessor_kind() == typesystem::MethodSemanticsAttributes::Setter;
            return true;
        }
        default: return false;
    }
}

bool StatementBuilder::parent_is_current_getter(il::ILInstruction* inst) {
    auto* cv = il::as<il::CallInstruction>(inst->parent());
    return cv != nullptr && cv->method->is_accessor() && cv->method->accessor_kind() == typesystem::MethodSemanticsAttributes::Getter;
}

// ---- fixed ------------------------------------------------------------------------------------------------------------------------

TranslatedStatement StatementBuilder::visit_pinned_region(il::PinnedRegion& inst) { return translate_pinned_region(PinnedRegionFacts::of(inst)); }

TranslatedStatement StatementBuilder::translate_pinned_region(const PinnedRegionFacts& inst) {
    auto* fixed_stmt = make<syntax::FixedStatement>();
    fixed_stmt->set_type(expr_builder().convert_type(inst.variable()->type()));
    Expression* init_expr;
    if (auto* gpr = il::as<il::GetPinnableReference>(inst.init())) {
        if (gpr->method() != nullptr) {
            const IType* expected_type = gpr->method()->is_static() ? gpr->method()->parameters()[0]->type() : gpr->method()->declaring_type();
            init_expr = expr_builder().translate(gpr->argument(), expected_type).convert_to(expected_type, expr_builder());
        } else {
            init_expr = expr_builder().translate(gpr->argument());
        }
    } else {
        const IType* ref_type = inst.variable()->type();
        if (const auto* pointer_type = dynamic_cast<const typesystem::PointerType*>(ref_type)) {
            ref_type = typesystem::make_by_reference_type(pointer_type->element_type());
        }
        init_expr = expr_builder().translate(inst.init(), ref_type).convert_to(ref_type, expr_builder());
        if (auto* dir_expr = syntax::as<syntax::DirectionExpression>(init_expr)) {
            auto* uoe = syntax::as<syntax::UnaryOperatorExpression>(dir_expr->expression());
            if (uoe != nullptr && uoe->operator_() == syntax::UnaryOperatorType::Dereference) {
                init_expr = syntax::detach(uoe->expression());
            } else {
                init_expr = with_rr(make<syntax::UnaryOperatorExpression>(syntax::UnaryOperatorType::AddressOf, syntax::detach(dir_expr->expression())),
                                    make<ResolveResult>(inst.variable()->type()))
                                .expression;
            }
        }
        const auto* brt = dynamic_cast<const typesystem::ByReferenceType*>(ref_type);
        if (get_resolve_result(init_expr)->type()->kind() == TypeKind::Pointer && !is_address_of_moveable_var(init_expr) &&
            !is_fixed_size_buffer(init_expr) && brt != nullptr) {
            // C# doesn't allow pinning an already-unmanaged pointer
            //   fixed (int* ptr = existing_ptr) {} -> invalid
            //   fixed (int* ptr = &existing_ptr->field) {} -> invalid
            //   fixed (int* ptr = &local_var) {} -> invalid
            // We work around this by instead doing:
            //   fixed (int* ptr = &Unsafe.AsRef<int>(existing_ptr))
            TranslatedExpression as_ref_call = expr_builder().call_unsafe_intrinsic("AsRef", {init_expr}, brt->element_type(), nullptr, {brt->element_type()});
            init_expr = with_rr(make<syntax::UnaryOperatorExpression>(syntax::UnaryOperatorType::AddressOf, as_ref_call),
                                make<ResolveResult>(inst.variable()->type()))
                            .expression;
        }
    }
    auto* initializer = make<syntax::VariableInitializer>(inst.variable()->name.value_or(std::string()), init_expr);
    with_il_variable(initializer, inst.variable());
    fixed_stmt->variables().add(initializer);
    fixed_stmt->set_embedded_statement(convert(inst.body()));
    return with_il_instruction(fixed_stmt, inst.self);
}

// ---- blocks and containers -------------------------------------------------------------------------------------------------------

TranslatedStatement StatementBuilder::visit_block(il::Block& block) {
    if (block.kind != il::BlockKind::ControlFlow) return default_visit(block);
    return translate_block(BlockFacts{&block, block.final_instruction()->op_code != il::OpCode::Nop ? block.final_instruction() : nullptr});
}

TranslatedStatement StatementBuilder::translate_block(const BlockFacts& facts) {
    il::Block& block = *facts.self;   // (its instructions are read as the foreach pattern changes them)
    // Block without container
    auto* block_statement = make<syntax::BlockStatement>();
    for (int i = 0; i < block.instructions.count(); i++) {
        if (Statement* foreach_stmt = transform_to_foreach_without_dispose(&block, &i)) {
            block_statement->add(foreach_stmt);
            continue;
        }
        block_statement->add(convert(block.instructions[i]));
    }
    if (facts.final_instruction != nullptr) block_statement->add(convert(facts.final_instruction));
    return with_il_instruction(block_statement, &block);
}

TranslatedStatement StatementBuilder::visit_block_container(il::BlockContainer& container) {
    BlockContainerFacts facts{&container, container.kind() != il::ContainerKind::Normal && container.entry_point()->incoming_edge_count() > 1, nullptr};
    if (container.entry_point()->instructions.count() == 1) facts.switch_inst = il::as<il::SwitchInstruction>(container.entry_point()->instructions[0]);
    return translate_block_container(facts);
}

TranslatedStatement StatementBuilder::translate_block_container(const BlockContainerFacts& facts) {
    il::BlockContainer& container = *facts.self;
    if (facts.is_loop) {
        il::Block* old_continue_target = continue_target_;
        int old_continue_count = continue_count_;
        il::BlockContainer* old_break_target = break_target_;
        Statement* loop = convert_loop(&container);
        loop->add_annotation(&container);
        continue_target_ = old_continue_target;
        continue_count_ = old_continue_count;
        break_target_ = old_break_target;
        return with_il_instruction(loop, &container);
    } else if (facts.switch_inst != nullptr) {
        const SwitchFacts switch_facts = facts.switch_facts ? *facts.switch_facts : SwitchFacts::of(*facts.switch_inst);
        return with_il_instruction(translate_switch(&container, &switch_facts), &container);
    } else {
        syntax::BlockStatement* block_stmt = convert_block_container(&container, false);
        return with_il_instruction(block_stmt, &container);
    }
}

Statement* StatementBuilder::convert_loop(il::BlockContainer* container) {
    il::ILInstruction* condition;
    il::Block* loop_body;
    syntax::BlockStatement* block_statement;
    continue_count_ = 0;
    break_target_ = container;
    // the blocks of the container from `from` up to (not including) `to`, without `except`
    auto blocks_of = [&](int from, int to, il::Block* except) {
        std::vector<il::Block*> result;
        for (int i = from; i < to; i++) {
            if (container->blocks[i] != except) result.push_back(container->blocks[i]);
        }
        return result;
    };
    switch (container->kind()) {
        case il::ContainerKind::Loop: {
            continue_target_ = container->entry_point();
            block_statement = convert_block_container(container, true);
            assert(continue_count_ < container->entry_point()->incoming_edge_count());
            assert(syntax::is<syntax::LabelStatement>(block_statement->statements().first_or_null()));
            if (container->entry_point()->incoming_edge_count() == continue_count_ + 1) {
                // Remove the entrypoint label if all jumps to the label were replaced with 'continue;' statements
                block_statement->statements().first_or_null()->remove();
            }

            if (syntax::is<syntax::ContinueStatement>(block_statement->statements().last_or_null()))
                block_statement->statements().last_or_null()->remove();
            declare_local_functions(current_function_, container, block_statement);
            return make<syntax::WhileStatement>(make<syntax::PrimitiveExpression>(ConstantValue(std::in_place_type<bool>, true)), block_statement);
        }
        case il::ContainerKind::While: {
            continue_target_ = container->entry_point();
            if (!container->match_condition_block(continue_target_, &condition, &loop_body))
                throw std::runtime_error("Invalid condition block in while loop.");
            block_statement = convert_as_block(loop_body);
            if (!loop_body->has_flag(il::InstructionFlags::EndPointUnreachable)) block_statement->add(static_cast<Statement*>(make<syntax::BreakStatement>()));
            block_statement = convert_block_container(block_statement, container, blocks_of(1, container->blocks.count(), loop_body), true);
            assert(continue_count_ < container->entry_point()->incoming_edge_count());
            if (continue_count_ + 1 < container->entry_point()->incoming_edge_count()) {
                // There's an incoming edge to the entry point (=while condition) that wasn't represented as "continue;" -> emit a real
                // label. We'll also remove any "continue;" in front of the label, as it's redundant.
                if (syntax::is<syntax::ContinueStatement>(block_statement->statements().last_or_null()))
                    block_statement->statements().last_or_null()->remove();
                block_statement->add(static_cast<Statement*>(make<syntax::LabelStatement>(ensure_unique_label(container->entry_point()))));
            }

            if (syntax::is<syntax::ContinueStatement>(block_statement->statements().last_or_null()))
                block_statement->statements().last_or_null()->remove();
            declare_local_functions(current_function_, container, block_statement);
            return make<syntax::WhileStatement>(expr_builder().translate_condition(condition), block_statement);
        }
        case il::ContainerKind::DoWhile: {
            continue_target_ = container->blocks[container->blocks.count() - 1];
            il::Block* ignored_body;
            if (!container->match_condition_block(continue_target_, &condition, &ignored_body))
                throw std::runtime_error("Invalid condition block in do-while loop.");
            block_statement = convert_block_container(make<syntax::BlockStatement>(), container, blocks_of(0, container->blocks.count() - 1, nullptr), true);
            if (container->entry_point()->incoming_edge_count() == 2) {
                // Remove the entry-point label, if there are only two jumps to the entry-point: from outside the loop and from the
                // condition-block.
                block_statement->statements().first_or_null()->remove();
            }
            if (syntax::is<syntax::ContinueStatement>(block_statement->statements().last_or_null()))
                block_statement->statements().last_or_null()->remove();
            if (continue_target_->incoming_edge_count() > continue_count_) {
                // if there are branches to the condition block, that were not converted to continue statements, we have to introduce an
                // extra label.
                block_statement->add(static_cast<Statement*>(make<syntax::LabelStatement>(ensure_unique_label(continue_target_))));
            }
            declare_local_functions(current_function_, container, block_statement);
            if (block_statement->statements().count() == 0) {
                auto* while_stmt = make<syntax::WhileStatement>();
                while_stmt->set_condition(expr_builder().translate_condition(condition));
                while_stmt->set_embedded_statement(block_statement);
                return while_stmt;
            }
            auto* do_while = make<syntax::DoWhileStatement>();
            do_while->set_embedded_statement(block_statement);
            do_while->set_condition(expr_builder().translate_condition(condition));
            return do_while;
        }
        case il::ContainerKind::For: {
            continue_target_ = container->blocks[container->blocks.count() - 1];
            if (!container->match_condition_block(container->entry_point(), &condition, &loop_body))
                throw std::runtime_error("Invalid condition block in for loop.");
            block_statement = convert_as_block(loop_body);
            if (!loop_body->has_flag(il::InstructionFlags::EndPointUnreachable)) block_statement->add(static_cast<Statement*>(make<syntax::BreakStatement>()));
            if (!container->match_increment_block(continue_target_)) throw std::runtime_error("Invalid increment block in for loop.");
            block_statement = convert_block_container(block_statement, container, blocks_of(1, container->blocks.count() - 1, loop_body), true);
            auto* for_stmt = make<syntax::ForStatement>();
            for_stmt->set_condition(expr_builder().translate_condition(condition));
            for_stmt->set_embedded_statement(block_statement);
            if (syntax::is<syntax::ContinueStatement>(block_statement->statements().last_or_null()))
                block_statement->statements().last_or_null()->remove();
            for (int i = 0; i < continue_target_->instructions.count() - 1; i++) {
                for_stmt->iterators().add(convert(continue_target_->instructions[i]));
            }
            if (continue_target_->incoming_edge_count() > continue_count_)
                block_statement->add(static_cast<Statement*>(make<syntax::LabelStatement>(ensure_unique_label(continue_target_))));
            declare_local_functions(current_function_, container, block_statement);
            return for_stmt;
        }
        default: throw std::out_of_range("container kind");
    }
}

syntax::BlockStatement* StatementBuilder::convert_block_container(il::BlockContainer* container, bool is_loop) {
    std::vector<il::Block*> blocks = container->blocks.items();
    syntax::BlockStatement* block_statement = convert_block_container(make<syntax::BlockStatement>(), container, blocks, is_loop);
    declare_local_functions(current_function_, container, block_statement);
    if (current_function_->body() == container) {
        if (emit_as_ref_read_only) {
            auto* method_decl = make<syntax::MethodDeclaration>();
            if (settings_->static_local_functions) method_decl->set_modifiers(syntax::Modifiers::Static);

            auto* return_type = make<syntax::ComposedType>();
            return_type->set_has_read_only_specifier(true);
            return_type->set_has_ref_specifier(true);
            return_type->set_base_type(make<syntax::SimpleType>("T"));
            method_decl->set_return_type(return_type);
            method_decl->set_name("ILSpyHelper_AsRefReadOnly");
            method_decl->type_parameters().add(make<syntax::TypeParameterDeclaration>("T"));
            auto* parameter = make<syntax::ParameterDeclaration>();
            parameter->set_parameter_modifier(typesystem::ReferenceKind::In);
            parameter->set_type(make<syntax::SimpleType>("T"));
            parameter->set_name("temp");
            method_decl->parameters().add(parameter);

            auto* body = make<syntax::BlockStatement>();
            method_decl->set_body(body);
            body->add(static_cast<Statement*>(comment_statement(
                "ILSpy generated this function to help ensure overload resolution can pick the overload using 'in'")));
            body->add(static_cast<Statement*>(make<syntax::ReturnStatement>(
                make<syntax::DirectionExpression>(syntax::FieldDirection::Ref, make<syntax::IdentifierExpression>("temp")))));

            block_statement->statements().add(make<syntax::LocalFunctionDeclarationStatement>(method_decl));
        }
    }
    return block_statement;
}

void StatementBuilder::declare_local_functions(il::ILFunction* current_function, il::BlockContainer* container,
                                               syntax::BlockStatement* block_statement) {
    std::vector<il::ILFunction*> local_functions = current_function->local_functions.items();
    std::stable_sort(local_functions.begin(), local_functions.end(), [](il::ILFunction* a, il::ILFunction* b) { return name_less(a->name, b->name); });
    for (il::ILFunction* local_function : local_functions) {
        if (local_function->declaration_scope != container) continue;
        block_statement->add(static_cast<Statement*>(translate_function(local_function)));
    }
}

syntax::LocalFunctionDeclarationStatement* StatementBuilder::translate_function(il::ILFunction* function) {
    syntax::TypeSystemAstBuilder& ast_builder = expr_builder().ast_builder;
    auto* method = syntax::cast<syntax::MethodDeclaration>(ast_builder.convert_entity(function->reduced_method));

    std::unordered_map<int, il::ILVariable*> variables;
    for (il::ILVariable* v : function->variables()) {
        if (v->kind() == il::VariableKind::Parameter) {
            if (!variables.emplace(*v->index(), v).second) throw std::invalid_argument("duplicate parameter index");
        }
    }

    int parameter_index = 0;
    for (syntax::ParameterDeclaration* p : method->parameters()) {
        auto found = variables.find(parameter_index);
        if (found != variables.end()) p->set_name(found->second->name.value_or(std::string()));
        parameter_index++;
    }

    if (function->method->has_body()) {
        StatementBuilder nested_builder(*type_system_, *decompilation_context_, function, *settings_, decompile_run, cancellation_token_, expr_builder().l2);

        method->set_body(nested_builder.convert_as_block(function->body()));

        Statement* warning_anchor = method->body()->statements().first_or_null();
        for (const std::string& warning : function->warnings) {
            syntax::EmptyStatement* warning_statement = comment_statement(warning);
            if (warning_anchor != nullptr)
                method->body()->statements().insert_before(warning_anchor, warning_statement);
            else
                method->body()->statements().add(warning_statement);
        }
    } else {
        method->set_modifiers(method->modifiers() | syntax::Modifiers::Extern);
    }

    CSharpDecompiler::add_annotations_to_declaration(function->reduced_method, method, function);
    CSharpDecompiler::clean_up_method_declaration(method, method->body(), function, function->method->has_body());
    CSharpDecompiler::remove_attribute(method, typesystem::KnownAttribute::CompilerGenerated);
    auto* stmt = make<syntax::LocalFunctionDeclarationStatement>(method);
    stmt->add_annotation(make<semantics::MemberResolveResult>(nullptr, function->reduced_method));
    with_il_instruction(stmt, function);
    return stmt;
}

syntax::BlockStatement* StatementBuilder::convert_block_container(syntax::BlockStatement* block_statement, il::BlockContainer* container,
                                                                  const std::vector<il::Block*>& blocks, bool is_loop) {
    for (il::Block* block : blocks) {
        if (block->incoming_edge_count() > 1 || block != container->entry_point()) {
            // If there are any incoming branches to this block, add a label:
            block_statement->add(static_cast<Statement*>(make<syntax::LabelStatement>(ensure_unique_label(block))));
        }
        for (int i = 0; i < block->instructions.count(); i++) {
            il::ILInstruction* inst = block->instructions[i];
            auto* leave = il::as<il::Leave>(inst);
            if (!is_loop && leave != nullptr && is_final_leave(leave)) {
                // skip the final 'leave' instruction and just fall out of the BlockStatement
                block_statement->add_annotation(make<ImplicitReturnAnnotation>(leave));
                continue;
            }
            Statement* stmt = transform_to_foreach_without_dispose(block, &i);
            if (stmt == nullptr) stmt = convert(inst);
            if (auto* b = syntax::as<syntax::BlockStatement>(stmt)) {
                for (Statement* nested : b->statements().to_vector()) block_statement->add(syntax::detach(nested));
            } else {
                block_statement->add(syntax::detach(stmt));
            }
        }
        if (block->final_instruction()->op_code != il::OpCode::Nop) block_statement->add(convert(block->final_instruction()));
    }
    auto end_label = end_container_labels_.find(container);
    if (end_label != end_container_labels_.end()) {
        if (is_loop && !syntax::is<syntax::ContinueStatement>(block_statement->statements().last_or_null())) {
            block_statement->add(static_cast<Statement*>(make<syntax::ContinueStatement>()));
        }
        block_statement->add(static_cast<Statement*>(make<syntax::LabelStatement>(end_label->second)));
        if (is_loop) block_statement->add(static_cast<Statement*>(make<syntax::BreakStatement>()));
    }
    return block_statement;
}

std::string StatementBuilder::ensure_unique_label(il::Block* block) {
    auto existing = labels_.find(block);
    if (existing != labels_.end()) return existing->second;
    std::string block_label = block->label();
    auto duplicate = duplicate_labels_.find(block_label);
    if (duplicate == duplicate_labels_.end()) {
        labels_.emplace(block, block_label);
        duplicate_labels_.emplace(block_label, 1);
        return block_label;
    }
    std::string label = block_label + "_" + std::to_string(duplicate->second + 1);
    duplicate->second++;
    labels_.emplace(block, label);
    return label;
}

bool StatementBuilder::is_final_leave(il::Leave* leave) {
    if (!leave->value()->match_nop()) return false;
    auto* block = static_cast<il::Block*>(leave->parent());
    if (leave->child_index() != block->instructions.count() - 1 || block->final_instruction()->op_code != il::OpCode::Nop) return false;
    auto* container = static_cast<il::BlockContainer*>(block->parent());
    return block->child_index() == container->blocks.count() - 1 && container == leave->target_container();
}

// ---- the IL block instructions ---------------------------------------------------------------------------------------------------

TranslatedStatement StatementBuilder::visit_initblk(il::Initblk& inst) { return translate_initblk(MemoryBlockFacts::of(inst)); }

TranslatedStatement StatementBuilder::translate_initblk(const MemoryBlockFacts& inst) {
    TranslatedExpression address = expr_builder().translate(inst.address());
    TranslatedExpression value = expr_builder().translate(inst.value());
    TranslatedExpression size = expr_builder().translate(inst.size());
    auto* stmt = make<syntax::ExpressionStatement>(expr_builder().call_unsafe_intrinsic(
        inst.unaligned_prefix() != 0 ? "InitBlockUnaligned" : "InitBlock", {address, value, size},
        decompilation_context_->compilation().find_type(KnownTypeCode::Void), inst.self));
    stmt->add_leading_trivia(make<syntax::Comment>(" IL initblk instruction"));
    return with_il_instruction(stmt, inst.self);
}

TranslatedStatement StatementBuilder::visit_cpblk(il::Cpblk& inst) { return translate_cpblk(MemoryBlockFacts::of(inst)); }

TranslatedStatement StatementBuilder::translate_cpblk(const MemoryBlockFacts& inst) {
    TranslatedExpression dest = expr_builder().translate(inst.dest_address());
    TranslatedExpression source = expr_builder().translate(inst.source_address());
    TranslatedExpression size = expr_builder().translate(inst.size());
    auto* stmt = make<syntax::ExpressionStatement>(expr_builder().call_unsafe_intrinsic(
        inst.unaligned_prefix() != 0 ? "CopyBlockUnaligned" : "CopyBlock", {dest, source, size},
        decompilation_context_->compilation().find_type(KnownTypeCode::Void), inst.self));
    stmt->add_leading_trivia(make<syntax::Comment>(" IL cpblk instruction"));
    return with_il_instruction(stmt, inst.self);
}

TranslatedStatement StatementBuilder::visit_ckfinite(il::Ckfinite& inst) { return translate_ckfinite(MemoryBlockFacts::of(inst)); }

TranslatedStatement StatementBuilder::translate_ckfinite(const MemoryBlockFacts& inst) {
    auto* is_finite_target = make<syntax::MemberReferenceExpression>();
    is_finite_target->set_target(make<syntax::TypeReferenceExpression>(make<syntax::PrimitiveType>("float")));
    is_finite_target->set_member_name("IsFinite");
    TranslatedExpression argument = expr_builder().translate(inst.argument());
    auto* is_finite_call = make<syntax::InvocationExpression>(is_finite_target, std::vector<Expression*>{argument});
    const IType* arithmetic_exception =
        typesystem::find_type(decompilation_context_->compilation(), typesystem::FullTypeName(std::string_view("System.ArithmeticException")));
    auto* arithmetic_exception_syntax = make<syntax::SimpleType>("ArithmeticException");
    arithmetic_exception_syntax->add_annotation(make<semantics::TypeResolveResult>(arithmetic_exception));
    auto* if_else = make<syntax::IfElseStatement>();
    if_else->set_condition(make<syntax::UnaryOperatorExpression>(syntax::UnaryOperatorType::Not, is_finite_call));
    if_else->set_true_statement(make<syntax::ThrowStatement>(make<syntax::ObjectCreateExpression>(arithmetic_exception_syntax)));
    return with_il_instruction(if_else, inst.self);
}

}  // namespace cb::dotnet::decompiler::csharp
