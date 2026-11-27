// translate/frontends/cil/lift.cpp -- L2 back into the ILAst (see cil.hpp): the inverse of lower.cpp, node by node.
#include <algorithm>
#include <bit>
#include <cstring>
#include <format>
#include <stdexcept>
#include <unordered_map>

#include "dotnet/decompiler/il/instructions.hpp"
#include "dotnet/decompiler/typesystem/tuple_type.hpp"
#include "dotnet/decompiler/typesystem/type_provider.hpp"
#include "translate/frontends/cil/cil.hpp"
#include "translate/frontends/cil/cil_names.hpp"

namespace cb::translate::frontends::cil {

namespace cil_lifting {

namespace il = dotnet::decompiler::il;
namespace ts = dotnet::decompiler::typesystem;
using ir::Node;

// A generic context's own list of type parameters (the ILAst's are arena objects of the transforms that made them).
struct TypeParameterList final : il::ArenaObject {
    std::vector<const ts::ITypeParameter*> items;
};

[[noreturn]] void refuse(const Node* n, std::string_view why) {
    throw std::logic_error(std::format("cil lifting: {} ({})", why, n != nullptr ? ir::kind_name(*n) : std::string_view("null")));
}

// What a lowering wraps around an instruction: an ExprStmt in a statement place, a Yield around a switch expression's arm.
const Node* unwrap(const Node* n) {
    while (n != nullptr) {
        if (const auto* e = ir::get<ir::ExprStmt>(n)) n = e->expr;
        else if (const auto* y = ir::get<ir::Yield>(n)) n = y->expr;
        else break;
    }
    return n;
}

const Node* one(const ir::Suite& suite) {
    if (const auto* p = std::get_if<Node*>(&suite)) return *p;
    const auto& list = std::get<std::vector<Node*>>(suite);
    if (list.size() != 1) refuse(nullptr, "a body of more than one statement");
    return list.front();
}

dotnet::decompiler::util::Interval range_of(const Node& n) {
    dotnet::decompiler::util::Interval iv;
    if (n.origin) {
        if (const auto* span = std::get_if<ir::OriginSpan>(n.origin.get())) {
            iv.start = static_cast<int>(span->start);
            iv.end = static_cast<int>(span->end);
        }
    }
    return iv;
}

il::StackType stack(const Node& n) { return static_cast<il::StackType>(n.stack); }

il::CSharpArgumentInfo arg_info(const ir::ArgMeta& m) {
    il::CSharpArgumentInfo info;
    info.name = m.name.value_or("");
    info.flags = static_cast<il::CSharpArgumentInfoFlags>(m.flags);
    info.compile_time_type = clr_type(m.type);
    return info;
}

std::int64_t info_int(const Node& n, std::string_view key, std::int64_t otherwise = 0) {
    const auto* v = n.info.get_as<std::int64_t>(key);
    return v != nullptr ? *v : otherwise;
}

}  // namespace cil_lifting

struct Lifting::State {
    struct Function {
        const ir::Node* body = nullptr;                  // the lowered body (unwrapped)
        il_ns::BlockContainer* container = nullptr;      // ... lifted: what a Return leaves
        ts_ns::GenericContext context;
    };
    std::unordered_map<const ir::Sym*, il_ns::ILVariable*> variables;
    std::unordered_map<std::string, il_ns::Block*> blocks;
    std::unordered_map<std::string, il_ns::BlockContainer*> containers;
    std::vector<Function> functions;
    std::vector<std::pair<il_ns::ILVariable*, std::string>> capture_scopes;
    std::vector<std::pair<il_ns::ILFunction*, std::string>> declaration_scopes;

    il_ns::ILVariable* variable(const ir::SymP& sym);
    il_ns::ILFunction* function(const ir::Node* n);
    il_ns::ILInstruction* lift(const ir::Node* n);
    il_ns::ILInstruction* lift_core(const ir::Node& n);
    il_ns::ILInstruction* optional(const ir::Node* n) { return n != nullptr ? lift(n) : nullptr; }
    std::vector<il_ns::ILInstruction*> lift_all(const std::vector<ir::Node*>& nodes);
    il_ns::BlockContainer* region(const ir::Node& n);
    void fill_block(il_ns::Block& b, const ir::Node* body);
    il_ns::Block* block_of(const ir::Node& n);
    il_ns::ILInstruction* binary(const ir::Node& n, const ir::Binary& k);
    il_ns::ILInstruction* assign(const ir::Node& n, const ir::Assign& k);
    il_ns::ILInstruction* unary(const ir::Node& n, const ir::Unary& k);
    il_ns::ILInstruction* cast(const ir::Node& n, const ir::Cast& k);
    il_ns::ILInstruction* call(const ir::Node& n, const ir::Call& k);
    il_ns::ILInstruction* intrinsic(const ir::Node& n, const ir::Intrinsic& k);
    il_ns::ILInstruction* lit(const ir::Node& n, const ir::Lit& k);
    il_ns::ILInstruction* try_(const ir::Node& n, const ir::Try& k);
    il_ns::ILInstruction* switch_(const ir::Node& n, const ir::Node* value, const std::vector<Node*>& cases, bool lifted, const ir::TypeP& of);
};

using cil_lifting::refuse;

// ---- variables and functions --------------------------------------------------------------------------------------------------

il_ns::ILVariable* Lifting::State::variable(const ir::SymP& sym) {
    namespace il = il_ns;
    if (!sym) refuse(nullptr, "a variable without a symbol");
    if (auto it = variables.find(sym.get()); it != variables.end()) return it->second;
    if (!sym->role) refuse(nullptr, std::format("a variable without a role: {}", sym->name));
    const std::string* exact = sym->info.get_as<std::string>("cil.kind_exact");
    const il::VariableKind kind = variable_kind(*sym->role, exact);
    std::optional<int> index;
    if (sym->index) index = *sym->index;
    auto* v = il::make<il::ILVariable>(kind, clr_type(sym->type), static_cast<il::StackType>(sym->stack), index);
    if (!sym->name.empty()) v->name = sym->name;
    v->has_generated_name = sym->generated_name;
    v->is_ref_read_only = sym->ref_readonly;
    v->set_initial_value_is_initialized(sym->entry_initialized);
    v->set_uses_initial_value(sym->entry_used);
    if (const auto* scope = sym->info.get_as<std::string>("cil.capture_scope")) capture_scopes.emplace_back(v, *scope);
    if (const auto* f = sym->info.get_as<ir::EntityP>("cil.state_machine_field")) v->state_machine_field = clr_field(*f);
    if (const auto* r = sym->info.get_as<bool>("cil.remove_if_redundant")) v->remove_if_redundant = *r;
    variables.emplace(sym.get(), v);
    return v;
}

il_ns::ILFunction* Lifting::State::function(const ir::Node* n) {
    namespace il = il_ns;
    namespace ts = ts_ns;
    const auto* md = ir::get<ir::MethodDecl>(n);
    const auto* lm = ir::get<ir::Lambda>(n);
    if (md == nullptr && lm == nullptr) refuse(n, "a function that is not a MethodDecl or Lambda");
    const ir::SymP& sym = md != nullptr ? md->sym : lm->sym;
    const ts::IMethod* method = clr_method(sym);
    const std::vector<std::string>& mods = md != nullptr ? md->mods : lm->mods;
    const auto has_mod = [&](std::string_view m) { return std::find(mods.begin(), mods.end(), m) != mods.end(); };

    Function fn;
    fn.body = cil_lifting::unwrap(md != nullptr ? (std::holds_alternative<Node*>(md->body) ? std::get<Node*>(md->body) : nullptr) : lm->body);
    if (n->info.find("cil.class_tparams") != nullptr || n->info.find("cil.method_tparams") != nullptr) {
        const auto list = [&](std::string_view key) {
            auto* l = il::ILArena::current().make<cil_lifting::TypeParameterList>();
            if (const auto* types = n->info.get_as<std::vector<ir::TypeP>>(key)) {
                for (const ir::TypeP& t : *types) l->items.push_back(dynamic_cast<const ts::ITypeParameter*>(clr_type(t)));
            }
            return &l->items;
        };
        fn.context = ts::GenericContext(list("cil.class_tparams"), list("cil.method_tparams"));
    } else if (method != nullptr) {
        fn.context = ts::GenericContext::of(*method);
    } else if (!functions.empty()) {
        fn.context = functions.back().context;
    }
    functions.push_back(fn);

    const std::vector<ir::SymP>& locals = md != nullptr ? md->locals : lm->locals;
    std::vector<il::ILVariable*> vars;
    vars.reserve(locals.size());
    for (const ir::SymP& s : locals) vars.push_back(variable(s));
    il::ILInstruction* body = lift(functions.back().body);
    std::vector<il::ILFunction*> nested;
    for (const Node* lf : md != nullptr ? md->nested : lm->nested) nested.push_back(function(lf));
    const ts::GenericContext context = functions.back().context;
    functions.pop_back();

    il::ILFunctionKind kind = md != nullptr ? (has_mod("local") ? il::ILFunctionKind::LocalFunction : il::ILFunctionKind::TopLevelFunction)
                                            : (has_mod("expression_tree") ? il::ILFunctionKind::ExpressionTree : il::ILFunctionKind::Delegate);
    const ts::IType* return_type = clr_type(md != nullptr ? md->ret : lm->ret);
    std::vector<const ts::IParameter*> parameters;
    for (const Node* p : md != nullptr ? md->params : lm->params) {
        const auto* param = ir::get<ir::Param>(p);
        if (param == nullptr) refuse(p, "a parameter that is not a Param");
        parameters.push_back(clr_parameter(param->sym));
    }
    const int code_size = static_cast<int>(cil_lifting::info_int(*n, "cil.code_size"));
    il::ILFunction* f = method != nullptr ? il::make<il::ILFunction>(method, code_size, context, body, kind)
                                          : il::make<il::ILFunction>(return_type, parameters, context, body, kind);
    f->set_il_range(cil_lifting::range_of(*n));
    if (md != nullptr) {
        f->name = md->name.value_or("");
    } else if (const auto* name = n->info.get_as<std::string>("cil.name")) {
        f->name = *name;
    } else {
        f->name = method != nullptr ? method->name() : std::string();
    }
    f->return_type = return_type;
    f->parameters = std::move(parameters);
    f->local_variable_signature_length = static_cast<int>(cil_lifting::info_int(*n, "cil.locals_sig_len"));
    f->is_iterator = has_mod("iterator");
    if (const auto* b = n->info.get_as<bool>("cil.mono_state_machine")) f->state_machine_compiled_with_mono = *b;
    if (const auto* b = n->info.get_as<bool>("cil.vb_state_machine")) f->state_machine_compiled_with_legacy_visual_basic = *b;
    if (const auto* t = n->info.get_as<ir::TypeP>("cil.async_return_type")) f->async_return_type = clr_type(*t);
    if (const auto* e = n->info.get_as<ir::EntityP>("cil.move_next")) f->move_next_method = clr_method(*e);
    if (const auto* e = n->info.get_as<ir::EntityP>("cil.reduced_method")) f->reduced_method = clr_method(*e);
    if (const auto* points = n->info.get_as<std::vector<std::int64_t>>("cil.seq_points"))
    {
        std::vector<int> offsets;
        offsets.reserve(points->size());
        for (std::int64_t p : *points) offsets.push_back(static_cast<int>(p));
        f->sequence_point_candidates = std::move(offsets);
    }
    if (const auto* offset = n->info.get_as<std::int64_t>("cil.async_catch_offset")) {
        f->async_debug_info.catch_handler_offset = static_cast<int>(*offset);
        f->async_debug_info.awaits_is_default = false;
    }
    f->delegate_type = clr_type(n->type);
    f->warnings = md != nullptr ? md->comments : lm->comments;
    for (const ir::SymP& c : md != nullptr ? md->captures : lm->captures) f->captured_variables.insert(variable(c));
    for (il::ILVariable* v : vars) f->variables().add(v);
    for (il::ILFunction* lf : nested) f->local_functions.add(lf);
    const std::optional<std::string>& scope = md != nullptr ? md->scope : lm->scope;
    if (scope) declaration_scopes.emplace_back(f, *scope);
    return f;
}

// ---- control flow -----------------------------------------------------------------------------------------------------------

il_ns::BlockContainer* Lifting::State::region(const ir::Node& n) {
    namespace il = il_ns;
    const auto& k = *ir::get<ir::Region>(&n);
    auto* c = il::make<il::BlockContainer>(container_kind(k.kind), cil_lifting::stack(n));
    c->set_il_range(cil_lifting::range_of(n));
    if (!functions.empty() && functions.back().body == &n) functions.back().container = c;
    std::vector<il::Block*> made;
    made.reserve(k.blocks.size());
    for (const Node* b : k.blocks) {
        const auto* lab = ir::get<ir::Labeled>(b);
        if (lab == nullptr || !lab->label) refuse(b, "a region's block that is not a Labeled");
        auto* block = il::make<il::Block>(il::BlockKind::ControlFlow);
        block->set_il_range(cil_lifting::range_of(*b));
        blocks[*lab->label] = block;
        made.push_back(block);
        c->blocks.add(block);
    }
    if (!k.blocks.empty()) containers[*ir::get<ir::Labeled>(k.blocks.front())->label] = c;
    for (std::size_t i = 0; i < k.blocks.size(); ++i) fill_block(*made[i], cil_lifting::one(ir::get<ir::Labeled>(k.blocks[i])->body));
    return c;
}

void Lifting::State::fill_block(il_ns::Block& b, const ir::Node* body) {
    body = cil_lifting::unwrap(body);
    if (const auto* blk = ir::get<ir::Block>(body)) {
        for (const Node* s : blk->stmts) b.instructions.add(lift(s));
        return;
    }
    if (const auto* e = ir::get<ir::BlockExpr>(body)) {
        for (const Node* s : e->stmts) b.instructions.add(lift(s));
        if (e->result != nullptr) b.set_final_instruction(lift(e->result));
        return;
    }
    refuse(body, "a block body that is not a Block or BlockExpr");
}

il_ns::Block* Lifting::State::block_of(const ir::Node& n) {
    namespace il = il_ns;
    il::BlockKind kind = il::BlockKind::ControlFlow;
    if (const auto* e = ir::get<ir::BlockExpr>(&n)) {
        const auto k = block_kind_from(e->role);
        if (!k) refuse(&n, "a block of an unknown role");
        kind = *k;
    }
    auto* b = il::make<il::Block>(kind);
    b->set_il_range(cil_lifting::range_of(n));
    fill_block(*b, &n);
    return b;
}

il_ns::ILInstruction* Lifting::State::switch_(const ir::Node& n, const ir::Node* value, const std::vector<Node*>& cases, bool lifted,
                                               const ir::TypeP& of) {
    namespace il = il_ns;
    auto* s = il::make<il::SwitchInstruction>(lift(value));
    s->is_lifted = lifted;
    s->type = clr_type(of);
    s->set_result_type(cil_lifting::stack(n));
    for (const Node* cn : cases) {
        const auto* c = ir::get<ir::Case>(cn);
        if (c == nullptr || c->body.size() != 1) refuse(cn, "a switch section that is not a Case of one statement");
        auto* sec = il::make<il::SwitchSection>();
        sec->set_il_range(cil_lifting::range_of(*cn));
        std::vector<dotnet::decompiler::util::LongInterval> intervals;
        intervals.reserve(c->ranges.size());
        for (const auto& [low, high] : c->ranges) intervals.push_back(dotnet::decompiler::util::LongInterval::inclusive(low, high));
        sec->set_labels(dotnet::decompiler::util::LongSet(intervals));
        sec->set_has_null_label(c->null_label);
        sec->set_is_compiler_generated_default_section(ir::has_flag(*cn, ir::NodeFlag::CompilerGenerated));
        sec->set_body(lift(c->body.front()));
        s->sections.add(sec);
    }
    return s;
}

il_ns::ILInstruction* Lifting::State::try_(const ir::Node& n, const ir::Try& k) {
    namespace il = il_ns;
    il::ILInstruction* body = lift(k.body);
    if (!k.resources.empty()) {
        const auto* res = ir::get<ir::Local>(k.resources.front());
        if (k.resources.size() != 1 || res == nullptr) refuse(&n, "a using of more than one resource");
        auto* u = il::make<il::UsingInstruction>(variable(res->sym), lift(res->init), body);
        u->set_is_async(std::find(res->mods.begin(), res->mods.end(), "await") != res->mods.end());
        u->set_is_ref_struct(std::find(res->mods.begin(), res->mods.end(), "ref_struct") != res->mods.end());
        return u;
    }
    if (k.finally_body != nullptr) return il::make<il::TryFinally>(body, lift(k.finally_body));
    if (k.fault_body != nullptr) return il::make<il::TryFault>(body, lift(k.fault_body));
    auto* t = il::make<il::TryCatch>(body);
    for (const Node* cn : k.catches) {
        const auto* c = ir::get<ir::Catch>(cn);
        if (c == nullptr) refuse(cn, "a handler that is not a Catch");
        auto* h = il::make<il::TryCatchHandler>(lift(c->filter), lift(c->body), variable(c->sym));
        h->set_il_range(cil_lifting::range_of(*cn));
        if (const auto* spec = cn->info.get_as<std::vector<std::int64_t>>("cil.spec_range"); spec != nullptr && spec->size() == 2)
            h->add_exception_specifier_il_range(dotnet::decompiler::util::Interval(static_cast<int>((*spec)[0]), static_cast<int>((*spec)[1])));
        t->handlers.add(h);
    }
    return t;
}

// ---- expressions ----------------------------------------------------------------------------------------------------------------

il_ns::ILInstruction* Lifting::State::binary(const ir::Node& n, const ir::Binary& k) {
    namespace il = il_ns;
    il::ILInstruction* left = lift(k.left);
    il::ILInstruction* right = lift(k.right);
    if (k.op == "??") {
        auto* c = il::make<il::NullCoalescingInstruction>(static_cast<il::NullCoalescingKind>(k.coalesce.value_or(ir::Coalesce::Ref)), left, right);
        c->underlying_result_type = static_cast<il::StackType>(k.left_stk);
        return c;
    }
    if (k.sym) return il::make<il::UserDefinedLogicOperator>(clr_method(k.sym), left, right);
    if (k.lift == ir::Lift::ThreeValued && (k.op == "&&" || k.op == "||")) {
        if (k.op == "&&") return il::make<il::ThreeValuedBoolAnd>(left, right);
        return il::make<il::ThreeValuedBoolOr>(left, right);
    }
    if (const auto cmp = comparison_from(k.op)) {
        return il::make<il::Comp>(*cmp, static_cast<il::ComparisonLiftingKind>(k.lift), static_cast<il::StackType>(k.left_stk), sign_from(k.is_signed),
                                  left, right);
    }
    const auto op = binary_op_from(k.op);
    if (!op) refuse(&n, std::format("a binary operator {}", k.op));
    return il::make<il::BinaryNumericInstruction>(*op, left, right, static_cast<il::StackType>(k.left_stk), static_cast<il::StackType>(k.right_stk),
                                                  k.overflow == ir::Overflow::Trap, sign_from(k.is_signed), k.lift == ir::Lift::CSharp);
}

il_ns::ILInstruction* Lifting::State::assign(const ir::Node& n, const ir::Assign& k) {
    namespace il = il_ns;
    if (k.eval) {
        il::ILInstruction* target = lift(k.target);
        il::ILInstruction* value = lift(k.value);
        const auto eval = static_cast<il::CompoundEvalMode>(*k.eval);
        const auto via = static_cast<il::CompoundTargetKind>(k.via.value_or(ir::Via::Address));
        if (k.sym) return il::make<il::UserDefinedCompoundAssign>(clr_method(k.sym), eval, target, via, value);
        const auto op = k.op ? binary_op_from(*k.op) : std::nullopt;
        if (!op) refuse(&n, "a compound assignment without an operator");
        const auto* binary = il::make<il::BinaryNumericInstruction>(*op, il::make<il::Nop>(), il::make<il::Nop>(), static_cast<il::StackType>(k.left_stk),
                                                                    static_cast<il::StackType>(k.right_stk), k.overflow == ir::Overflow::Trap,
                                                                    sign_from(k.is_signed), k.lift == ir::Lift::CSharp);
        return il::make<il::NumericCompoundAssign>(binary, target, via, value, clr_type(n.type), eval);
    }
    if (const auto* name = ir::get<ir::Name>(k.target)) {
        auto* s = il::make<il::StLoc>(variable(name->sym), lift(k.value));
        s->is_stack_adjustment = ir::has_flag(n, ir::NodeFlag::StackAdjust);
        s->il_stack_was_empty = ir::has_flag(n, ir::NodeFlag::StackEmpty);
        return s;
    }
    const auto* deref = ir::get<ir::Unary>(k.target);
    if (deref == nullptr || deref->op != "*") refuse(&n, "an assignment to what is neither a variable nor an address");
    auto* s = il::make<il::StObj>(lift(deref->x), lift(k.value), clr_type(k.target->type));
    s->set_is_volatile(ir::has_flag(*k.target, ir::NodeFlag::Volatile));
    s->set_unaligned_prefix(unaligned_of(*k.target));
    return s;
}

il_ns::ILInstruction* Lifting::State::unary(const ir::Node& n, const ir::Unary& k) {
    namespace il = il_ns;
    if (k.op == "~") return il::make<il::BitNot>(lift(k.x), k.lift == ir::Lift::CSharp, static_cast<il::StackType>(k.left_stk));
    if (k.op == "*") {
        if (ir::has_flag(n, ir::NodeFlag::IfRef)) return il::make<il::LdObjIfRef>(lift(k.x), clr_type(n.type));
        auto* l = il::make<il::LdObj>(lift(k.x), clr_type(n.type));
        l->set_is_volatile(ir::has_flag(n, ir::NodeFlag::Volatile));
        l->set_unaligned_prefix(unaligned_of(n));
        return l;
    }
    if (k.op != "&") refuse(&n, std::format("a unary operator {}", k.op));
    if (ir::has_flag(n, ir::NodeFlag::RValueAddr)) return il::make<il::AddressOf>(lift(k.x), clr_type(n.type));
    if (const auto* name = ir::get<ir::Name>(k.x)) return il::make<il::LdLoca>(variable(name->sym));
    if (const auto* sel = ir::get<ir::Select>(k.x)) {
        const ts_ns::IField* field = clr_field(sel->sym);
        if (ir::is<ir::TypeName>(sel->obj)) return il::make<il::LdsFlda>(field);
        auto* l = il::make<il::LdFlda>(lift(sel->obj), field);
        l->delay_exceptions = ir::has_flag(n, ir::NodeFlag::DelayExc);
        return l;
    }
    if (const auto* idx = ir::get<ir::Index>(k.x)) {
        il::ILInstruction* array = lift(idx->arr);
        std::vector<il::ILInstruction*> indices;
        for (const Node* i : ir::statements(idx->index)) indices.push_back(lift(i));
        if (ir::has_flag(n, ir::NodeFlag::InlineArray)) {
            auto* l = il::make<il::LdElemaInlineArray>(clr_type(idx->elem), array, indices);
            l->is_read_only = ir::has_flag(n, ir::NodeFlag::ReadOnly);
            return l;
        }
        auto* l = il::make<il::LdElema>(clr_type(idx->elem), array, indices);
        l->with_system_index = ir::has_flag(n, ir::NodeFlag::FromEnd);
        l->delay_exceptions = ir::has_flag(n, ir::NodeFlag::DelayExc);
        l->is_read_only = ir::has_flag(n, ir::NodeFlag::ReadOnly);
        return l;
    }
    refuse(&n, "an address of what is no variable, field or element");
}

il_ns::ILInstruction* Lifting::State::cast(const ir::Node& n, const ir::Cast& k) {
    namespace il = il_ns;
    il::ILInstruction* x = lift(k.x);
    const ts_ns::IType* type = clr_type(n.type);
    const ir::ConvKind conv = k.conv.value_or(ir::ConvKind::Invalid);
    switch (conv) {
        case ir::ConvKind::Box: return il::make<il::Box>(x, type);
        case ir::ConvKind::Unbox: return il::make<il::Unbox>(x, type);
        case ir::ConvKind::UnboxAny: return il::make<il::UnboxAny>(x, type);
        case ir::ConvKind::RefCast: return il::make<il::CastClass>(x, type);
        case ir::ConvKind::TryCast: return il::make<il::IsInst>(x, type);
        case ir::ConvKind::ExprTree: return il::make<il::ExpressionTreeCast>(type, x, k.overflow == ir::Overflow::Trap);
        default: break;
    }
    const auto target = n.type ? conv_target_from(*n.type, n.info.get_as<bool>("cil.target_r") != nullptr) : std::nullopt;
    if (!target) refuse(&n, "a conversion to a type that is no CIL primitive");
    auto* c = il::make<il::Conv>(x, static_cast<il::StackType>(k.from_stk), sign_from(k.from_signed), *target, k.overflow == ir::Overflow::Trap,
                                 k.lift == ir::Lift::CSharp);
    c->kind = static_cast<il::ConversionKind>(conv);
    c->input_sign = sign_from(k.from_signed);
    return c;
}

il_ns::ILInstruction* Lifting::State::call(const ir::Node& n, const ir::Call& k) {
    namespace il = il_ns;
    if (ir::has_flag(n, ir::NodeFlag::PinnableRef)) return il::make<il::GetPinnableReference>(lift(k.target), clr_method(k.sym));
    if (k.form == ir::CallForm::Indirect) {
        il::ILInstruction* pointer = lift(k.target);
        std::vector<il::ILInstruction*> args = lift_all(k.args);
        const auto* sig = n.info.get_as<ir::TypeP>("cil.calli_sig");
        if (sig == nullptr || !*sig) refuse(&n, "an indirect call without its signature");
        const auto* ptr = std::get_if<ir::FnPtrType>(&(*sig)->body);
        const auto* fn = ptr != nullptr && ptr->fn ? std::get_if<ir::FnType>(&ptr->fn->body) : nullptr;
        if (fn == nullptr) refuse(&n, "an indirect call's signature that is no function pointer");
        const auto* type = dynamic_cast<const ts_ns::FunctionPointerType*>(clr_type(*sig));
        return il::make<il::CallIndirect>(fn->has_this, fn->explicit_this, type, pointer, args);
    }
    const ts_ns::IMethod* method = clr_method(k.sym);
    il::CallInstruction* c = k.form == ir::CallForm::Virtual ? static_cast<il::CallInstruction*>(il::make<il::CallVirt>(method))
                                                             : static_cast<il::CallInstruction*>(il::make<il::Call>(method));
    if (k.target != nullptr) c->arguments().add(lift(k.target));
    for (const Node* a : k.args) c->arguments().add(lift(a));
    c->is_tail = ir::has_flag(n, ir::NodeFlag::Tail);
    c->il_stack_was_empty = ir::has_flag(n, ir::NodeFlag::StackEmpty);
    if (const auto* t = n.info.get_as<ir::TypeP>("cil.constrained")) c->constrained_to = clr_type(*t);
    return c;
}

il_ns::ILInstruction* Lifting::State::lit(const ir::Node& n, const ir::Lit& k) {
    namespace il = il_ns;
    const auto bits = [&]() -> std::optional<std::uint64_t> {
        if (!k.text || k.text->size() < 3) return std::nullopt;
        return std::stoull(k.text->substr(2), nullptr, 16);
    };
    switch (n.stack) {
        case ir::Stack::I4: return il::make<il::LdcI4>(static_cast<std::int32_t>(std::get<std::int64_t>(k.value)));
        case ir::Stack::I8: return il::make<il::LdcI8>(std::get<std::int64_t>(k.value));
        case ir::Stack::F4: {
            if (const auto b = bits()) return il::make<il::LdcF4>(std::bit_cast<float>(static_cast<std::uint32_t>(*b)));
            return il::make<il::LdcF4>(static_cast<float>(std::get<double>(k.value)));
        }
        case ir::Stack::F8: {
            if (const auto b = bits()) return il::make<il::LdcF8>(std::bit_cast<double>(*b));
            return il::make<il::LdcF8>(std::get<double>(k.value));
        }
        case ir::Stack::O: {
            if (std::holds_alternative<std::monostate>(k.value)) return il::make<il::LdNull>();
            const std::string& s = std::get<std::string>(k.value);
            if (n.type && n.type == ir::decimal_type()) {
                const auto d = decimal_from(s);
                if (!d) refuse(&n, "a decimal literal that is not one");
                return il::make<il::LdcDecimal>(*d);
            }
            if (ir::has_flag(n, ir::NodeFlag::Utf8Literal)) return il::make<il::LdStrUtf8>(s);
            return il::make<il::LdStr>(s);
        }
        default: break;
    }
    refuse(&n, "a literal of no stack kind");
}

il_ns::ILInstruction* Lifting::State::intrinsic(const ir::Node& n, const ir::Intrinsic& k) {
    namespace il = il_ns;
    const std::string& name = k.name;
    const auto arg = [&](std::size_t i) {
        if (i >= k.args.size()) refuse(&n, std::format("{} without operand {}", name, i));
        return lift(k.args[i]);
    };
    const auto targ = [&](std::size_t i) { return i < k.targs.size() ? clr_type(k.targs[i]) : nullptr; };
    const auto imm = [&](std::size_t i) { return i < k.imms.size() ? k.imms[i] : 0; };
    const auto str = [&](std::size_t i) { return i < k.strs.size() ? k.strs[i] : std::string(); };
    const auto meta = [&](std::size_t i) { return i < k.argmeta.size() ? cil_lifting::arg_info(k.argmeta[i]) : il::CSharpArgumentInfo{}; };
    if (name == "cil.arglist") return il::make<il::Arglist>();
    if (name == "debug.break") return il::make<il::DebugBreak>();
    if (name == "cil.ckfinite") return il::make<il::Ckfinite>(arg(0));
    if (name == "mem.copy" || name == "mem.fill") {
        il::ILInstruction* a0 = arg(0);
        il::ILInstruction* a1 = arg(1);
        il::ILInstruction* a2 = arg(2);
        if (name == "mem.copy") {
            auto* c = il::make<il::Cpblk>(a0, a1, a2);
            c->set_is_volatile(ir::has_flag(n, ir::NodeFlag::Volatile));
            c->set_unaligned_prefix(unaligned_of(n));
            return c;
        }
        auto* c = il::make<il::Initblk>(a0, a1, a2);
        c->set_is_volatile(ir::has_flag(n, ir::NodeFlag::Volatile));
        c->set_unaligned_prefix(unaligned_of(n));
        return c;
    }
    if (name == "cil.ldtoken_type") return il::make<il::LdTypeToken>(targ(0));
    if (name == "cil.ldtoken_member") return il::make<il::LdMemberToken>(k.syms.empty() ? nullptr : clr_member(k.syms[0]));
    if (name == "cil.makeref") return il::make<il::MakeRefAny>(arg(0), targ(0));
    if (name == "cil.reftype") return il::make<il::RefAnyType>(arg(0));
    if (name == "cil.refvalue") return il::make<il::RefAnyValue>(arg(0), targ(0));
    if (name == "cil.deconstruct_result") return il::make<il::DeconstructResultInstruction>(static_cast<int>(imm(0)), cil_lifting::stack(n), arg(0));
    if (name == "cil.string_to_int") {
        const std::int64_t null_key = cil_lifting::info_int(n, "cil.null_key", -1);
        std::vector<il::StringToInt::MapEntry> map;
        map.reserve(k.strs.size());
        for (std::size_t i = 0; i < k.strs.size(); ++i) {
            std::optional<std::string> key;
            if (static_cast<std::int64_t>(i) != null_key) key = k.strs[i];
            map.emplace_back(std::move(key), static_cast<int>(imm(i)));
        }
        return il::make<il::StringToInt>(arg(0), std::move(map), targ(0));
    }
    if (name.starts_with("cs.dynamic.")) {
        const auto binder = static_cast<il::CSharpBinderFlags>(imm(0));
        const bool has_context = ir::has_flag(n, ir::NodeFlag::HasContext);
        const ts_ns::IType* context = has_context && !k.targs.empty() ? clr_type(k.targs.back()) : nullptr;
        const std::size_t own_targs = k.targs.size() - (has_context ? 1 : 0);
        const auto all_args = [&]() { return lift_all(k.args); };
        const auto all_meta = [&]() {
            std::vector<il::CSharpArgumentInfo> out;
            for (const ir::ArgMeta& m : k.argmeta) out.push_back(cil_lifting::arg_info(m));
            return out;
        };
        const std::string_view kind = std::string_view(name).substr(std::strlen("cs.dynamic."));
        if (kind == "compound_assign") {
            il::ILInstruction* target = arg(0);
            il::ILInstruction* value = arg(1);
            return il::make<il::DynamicCompoundAssign>(static_cast<il::ExpressionType>(imm(1)), binder, target, meta(0), value, meta(1),
                                                       static_cast<il::CompoundTargetKind>(imm(2)));
        }
        if (kind == "logic" || kind == "binary") {
            il::ILInstruction* left = arg(0);
            il::ILInstruction* right = arg(1);
            const auto op = static_cast<il::ExpressionType>(imm(1));
            if (kind == "logic") return il::make<il::DynamicLogicOperatorInstruction>(binder, op, context, meta(0), left, meta(1), right);
            return il::make<il::DynamicBinaryOperatorInstruction>(binder, op, context, meta(0), left, meta(1), right);
        }
        if (kind == "unary") return il::make<il::DynamicUnaryOperatorInstruction>(binder, static_cast<il::ExpressionType>(imm(1)), context, meta(0), arg(0));
        if (kind == "convert") return il::make<il::DynamicConvertInstruction>(binder, targ(0), context, arg(0));
        if (kind == "get_member") return il::make<il::DynamicGetMemberInstruction>(binder, str(0), context, meta(0), arg(0));
        if (kind == "set_member") {
            il::ILInstruction* target = arg(0);
            il::ILInstruction* value = arg(1);
            return il::make<il::DynamicSetMemberInstruction>(binder, str(0), context, meta(0), target, meta(1), value);
        }
        if (kind == "is_event") return il::make<il::DynamicIsEventInstruction>(binder, str(0), context, arg(0));
        if (kind == "get_index") return il::make<il::DynamicGetIndexInstruction>(binder, context, all_meta(), all_args());
        if (kind == "set_index") return il::make<il::DynamicSetIndexInstruction>(binder, context, all_meta(), all_args());
        if (kind == "invoke") return il::make<il::DynamicInvokeInstruction>(binder, context, all_meta(), all_args());
        if (kind == "invoke_member") {
            std::vector<const ts_ns::IType*> type_args;
            for (std::size_t i = 0; i < own_targs; ++i) type_args.push_back(clr_type(k.targs[i]));
            return il::make<il::DynamicInvokeMemberInstruction>(binder, str(0), std::move(type_args), context, all_meta(), all_args());
        }
        if (kind == "invoke_constructor") return il::make<il::DynamicInvokeConstructorInstruction>(binder, clr_type(n.type), context, all_meta(), all_args());
    }
    refuse(&n, std::format("an intrinsic {}", name));
}

// ---- the dispatch -------------------------------------------------------------------------------------------------------------

std::vector<il_ns::ILInstruction*> Lifting::State::lift_all(const std::vector<ir::Node*>& nodes) {
    std::vector<il_ns::ILInstruction*> out;
    out.reserve(nodes.size());
    for (const Node* n : nodes) out.push_back(lift(n));
    return out;
}

il_ns::ILInstruction* Lifting::State::lift(const ir::Node* node) {
    const Node* n = cil_lifting::unwrap(node);
    if (n == nullptr) refuse(node, "a missing instruction");
    il_ns::ILInstruction* inst = lift_core(*n);
    // a function, a container and a block set their range where they are made; the rest here
    if (!il_ns::is<il_ns::ILFunction>(inst) && !il_ns::is<il_ns::BlockContainer>(inst) && !il_ns::is<il_ns::Block>(inst))
        inst->set_il_range(cil_lifting::range_of(*n));
    return inst;
}

il_ns::ILInstruction* Lifting::State::lift_core(const ir::Node& n) {
    namespace il = il_ns;
    return ir::visit(n, [&](const auto& k) -> il::ILInstruction* {
        using K = std::decay_t<decltype(k)>;
        if constexpr (std::is_same_v<K, ir::MethodDecl> || std::is_same_v<K, ir::Lambda>) {
            return function(&n);
        } else if constexpr (std::is_same_v<K, ir::Region>) {
            return region(n);
        } else if constexpr (std::is_same_v<K, ir::Block> || std::is_same_v<K, ir::BlockExpr>) {
            return block_of(n);
        } else if constexpr (std::is_same_v<K, ir::Unsupported>) {
            if (k.kind == "il.invalid_branch") {
                auto* i = il::make<il::InvalidBranch>(k.raw.value_or(""));
                i->expected_result_type = cil_lifting::stack(n);
                return i;
            }
            if (k.kind != "il.invalid_expression") refuse(&n, "an Unsupported of another lane");
            auto* i = il::make<il::InvalidExpression>(k.raw.value_or(""));
            i->expected_result_type = cil_lifting::stack(n);
            if (const auto* sev = n.info.get_as<std::string>("cil.severity")) i->severity = *sev;
            return i;
        } else if constexpr (std::is_same_v<K, ir::Empty>) {
            auto* nop = il::make<il::Nop>();
            nop->comment = k.note.value_or("");
            nop->kind = static_cast<il::NopKind>(k.kind);
            return nop;
        } else if constexpr (std::is_same_v<K, ir::Fixed>) {
            const auto* var = ir::get<ir::Local>(k.var);
            if (var == nullptr) refuse(&n, "a fixed statement without its Local");
            il::ILInstruction* init = lift(var->init);
            return il::make<il::PinnedRegion>(variable(var->sym), init, lift(k.body));
        } else if constexpr (std::is_same_v<K, ir::Binary>) {
            return binary(n, k);
        } else if constexpr (std::is_same_v<K, ir::Assign>) {
            return assign(n, k);
        } else if constexpr (std::is_same_v<K, ir::Unary>) {
            return unary(n, k);
        } else if constexpr (std::is_same_v<K, ir::Cast>) {
            return cast(n, k);
        } else if constexpr (std::is_same_v<K, ir::Call>) {
            return call(n, k);
        } else if constexpr (std::is_same_v<K, ir::New>) {
            auto* c = il::make<il::NewObj>(clr_method(k.sym));
            for (const Node* a : k.args) c->arguments().add(lift(a));
            c->is_tail = ir::has_flag(n, ir::NodeFlag::Tail);
            c->il_stack_was_empty = ir::has_flag(n, ir::NodeFlag::StackEmpty);
            if (const auto* t = n.info.get_as<ir::TypeP>("cil.constrained")) c->constrained_to = clr_type(*t);
            return c;
        } else if constexpr (std::is_same_v<K, ir::Intrinsic>) {
            return intrinsic(n, k);
        } else if constexpr (std::is_same_v<K, ir::Lit>) {
            return lit(n, k);
        } else if constexpr (std::is_same_v<K, ir::Name>) {
            return il::make<il::LdLoc>(variable(k.sym));
        } else if constexpr (std::is_same_v<K, ir::Goto>) {
            auto it = k.label ? blocks.find(*k.label) : blocks.end();
            if (it == blocks.end()) refuse(&n, "a goto to a block not lifted");
            return il::make<il::Branch>(it->second);
        } else if constexpr (std::is_same_v<K, ir::Break>) {
            auto it = k.label ? containers.find(*k.label) : containers.end();
            if (it == containers.end()) refuse(&n, "a break out of a region not lifted");
            return il::make<il::Leave>(it->second, optional(k.value));
        } else if constexpr (std::is_same_v<K, ir::Return>) {
            if (functions.empty() || functions.back().container == nullptr) refuse(&n, "a return outside a function's body");
            return il::make<il::Leave>(functions.back().container, optional(k.expr));
        } else if constexpr (std::is_same_v<K, ir::If>) {
            il::ILInstruction* cond = lift(k.cond);
            il::ILInstruction* then = lift(cil_lifting::one(k.then));
            il::ILInstruction* orelse = ir::present(k.orelse) ? lift(cil_lifting::one(k.orelse)) : nullptr;
            return il::make<il::IfInstruction>(cond, then, orelse);
        } else if constexpr (std::is_same_v<K, ir::Cond>) {
            il::ILInstruction* cond = lift(k.cond);
            il::ILInstruction* then = lift(k.then);
            return il::make<il::IfInstruction>(cond, then, optional(k.orelse));
        } else if constexpr (std::is_same_v<K, ir::Switch>) {
            return switch_(n, k.expr, k.cases, k.lifted, k.of);
        } else if constexpr (std::is_same_v<K, ir::SwitchExpr>) {
            return switch_(n, k.expr, k.cases, k.lifted, k.of);
        } else if constexpr (std::is_same_v<K, ir::Try>) {
            return try_(n, k);
        } else if constexpr (std::is_same_v<K, ir::Sync>) {
            il::ILInstruction* on = lift(k.lock);
            return il::make<il::LockInstruction>(on, lift(k.body));
        } else if constexpr (std::is_same_v<K, ir::Throw> || std::is_same_v<K, ir::ThrowExpr>) {
            const Node* x;
            if constexpr (std::is_same_v<K, ir::Throw>) x = k.expr;
            else x = k.x;
            if (x == nullptr) return il::make<il::Rethrow>();
            auto* t = il::make<il::Throw>(lift(x));
            t->result_type_ = cil_lifting::stack(n);
            return t;
        } else if constexpr (std::is_same_v<K, ir::AddressOf>) {
            return il::make<il::LdFtn>(clr_method(k.sym));
        } else if constexpr (std::is_same_v<K, ir::MethodRef>) {
            il::ILInstruction* target = lift(k.target);
            if (k.mode == "virtual_ptr") return il::make<il::LdVirtFtn>(target, clr_method(k.sym));
            if (k.mode == "virtual_delegate") return il::make<il::LdVirtDelegate>(target, clr_type(k.of), clr_method(k.sym));
            refuse(&n, "a method reference of another mode");
        } else if constexpr (std::is_same_v<K, ir::StackAlloc>) {
            if (k.span) return il::make<il::LocAllocSpan>(lift(k.count), clr_type(k.elem));
            return il::make<il::LocAlloc>(lift(k.count));
        } else if constexpr (std::is_same_v<K, ir::NewArray>) {
            return il::make<il::NewArr>(clr_type(k.elem), lift_all(k.dims));
        } else if constexpr (std::is_same_v<K, ir::Default>) {
            auto* d = il::make<il::DefaultValue>(clr_type(n.type));
            d->il_stack_was_empty = ir::has_flag(n, ir::NodeFlag::StackEmpty);
            return d;
        } else if constexpr (std::is_same_v<K, ir::SizeOf>) {
            return il::make<il::SizeOf>(clr_type(k.of));
        } else if constexpr (std::is_same_v<K, ir::Length>) {
            return il::make<il::LdLen>(cil_lifting::stack(n), lift(k.arr));
        } else if constexpr (std::is_same_v<K, ir::OptionalUnwrap>) {
            return il::make<il::NullableUnwrap>(cil_lifting::stack(n), lift(k.x), k.ref_in);
        } else if constexpr (std::is_same_v<K, ir::OptionalChain>) {
            return il::make<il::NullableRewrap>(lift(k.x));
        } else if constexpr (std::is_same_v<K, ir::YieldReturn>) {
            return il::make<il::YieldReturn>(lift(k.expr));
        } else if constexpr (std::is_same_v<K, ir::Await>) {
            auto* a = il::make<il::Await>(lift(k.x));
            a->get_awaiter_method = clr_method(k.awaiter);
            a->get_result_method = clr_method(k.result);
            return a;
        } else if constexpr (std::is_same_v<K, ir::PatternMatch>) {
            il::ILInstruction* tested = lift(k.tested);
            std::vector<il::ILInstruction*> subs = lift_all(k.subs);
            auto* m = il::make<il::MatchInstruction>(variable(k.var), clr_method(k.deconstruct), tested, subs);
            m->is_deconstruct_call = k.deconstruct != nullptr;
            m->is_deconstruct_tuple = k.is_tuple;
            m->check_type = static_cast<bool>(k.test);
            m->check_not_null = k.check_not_null;
            return m;
        } else if constexpr (std::is_same_v<K, ir::Destructure>) {
            auto* d = il::make<il::DeconstructInstruction>();
            for (const Node* i : k.init) {
                auto* s = il::as<il::StLoc>(lift(i));
                if (s == nullptr) refuse(i, "a deconstruction's init that is not a store");
                d->init.add(s);
            }
            auto* pattern = il::as<il::MatchInstruction>(lift(k.pattern));
            auto* conversions = il::as<il::Block>(lift(k.conversions));
            auto* assignments = il::as<il::Block>(lift(k.assignments));
            if (pattern == nullptr || conversions == nullptr || assignments == nullptr) refuse(&n, "a deconstruction of the wrong parts");
            d->set_pattern(pattern);
            d->set_conversions(conversions);
            d->set_assignments(assignments);
            return d;
        } else {
            refuse(&n, "a node the CIL lowering does not make");
        }
    });
}

// ---- the public face --------------------------------------------------------------------------------------------------------------

Lifting::Lifting() : s_(std::make_unique<State>()) {}
Lifting::~Lifting() = default;

dotnet::decompiler::il::ILFunction* Lifting::function(const ir::Node* root) {
    State& s = *s_;
    s.variables.clear();
    s.blocks.clear();
    s.containers.clear();
    s.functions.clear();
    s.capture_scopes.clear();
    s.declaration_scopes.clear();
    il_ns::ILFunction* f = s.function(root);
    for (const auto& [v, label] : s.capture_scopes) {
        auto it = s.containers.find(label);
        if (it == s.containers.end()) refuse(root, "a capture scope that names no region");
        v->capture_scope = it->second;
    }
    for (const auto& [fn, label] : s.declaration_scopes) {
        auto it = s.containers.find(label);
        if (it == s.containers.end()) refuse(root, "a declaration scope that names no region");
        fn->declaration_scope = it->second;
    }
    f->add_ref();   // the root is connected: the use lists and counts are made
    return f;
}

}  // namespace cb::translate::frontends::cil
