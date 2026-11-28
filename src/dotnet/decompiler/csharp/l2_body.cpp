// dotnet/decompiler/csharp/l2_body.cpp -- see l2_body.hpp.
#include "dotnet/decompiler/csharp/l2_body.hpp"

#include <stdexcept>
#include <utility>
#include <variant>

#include "dotnet/decompiler/il/instructions.hpp"

namespace cb::dotnet::decompiler::csharp {

std::unique_ptr<L2Body> L2Body::make(il::ILFunction& function, L2Lane& lane) {
    std::unique_ptr<L2Body> body(new L2Body(lane));
    const auto start = std::chrono::steady_clock::now();
    struct Clock {
        std::chrono::steady_clock::time_point start;
        std::chrono::steady_clock::duration& total;
        ~Clock() { total += std::chrono::steady_clock::now() - start; }
    } clock{start, lane.lowering_time};
    try {
        body->root_ = lane.lowering.function(function, &body->map_);
        body->function_ = &function;
    } catch (const std::logic_error&) {
        ++lane.refused;
        return nullptr;   // a state L2 cannot hold (none on the test corpus): the builders read the ILAst
    }
    ++lane.bodies;
    body->index_ = ir::TreeIndex(body->root_);
    return body;
}

void L2Body::count(bool read_from_l2, const il::ILInstruction& inst) {
    if (read_from_l2) {
        ++lane_.from_l2;
    } else {
        ++lane_.from_ilast;
        ++lane_.from_ilast_by_op[static_cast<std::size_t>(inst.op_code)];
    }
}

void L2Body::variable_changed(il::ILVariable* variable) {
    const ir::SymP* sym = map_.sym(variable);
    if (sym == nullptr) throw std::logic_error("L2Body: a variable with no symbol changed");
    lane_.lowering.restate(**sym, *variable);
    index_.invalidate();
}

void L2Body::variable_registered(il::ILFunction* function, il::ILVariable* variable) {
    ir::SymP sym = lane_.lowering.new_variable(*variable);
    auto* owner = const_cast<ir::Node*>(map_.node(function));   // (the body owns its tree)
    if (auto* m = ir::get<ir::MethodDecl>(owner)) m->locals.push_back(sym);
    else if (auto* l = ir::get<ir::Lambda>(owner)) l->locals.push_back(sym);
    else throw std::logic_error("L2Body: a variable registered in a function with no node");
    map_.link(sym, variable);
}

ir::Node* L2Body::lower_new(il::ILInstruction* inst) {
    if (const ir::Node* known = map_.node(inst)) return const_cast<ir::Node*>(known);
    const auto name_of = [&](il::ILVariable* v) {
        const ir::SymP* sym = map_.sym(v);
        if (sym == nullptr) throw std::logic_error("L2Body: an instruction names a variable with no symbol");
        return ir::make(lane_.arena, ir::Name{.sym = *sym}, (*sym)->type);
    };
    ir::Node* n;
    if (auto* ld = il::as<il::LdLoc>(inst)) {
        n = name_of(ld->variable());
    } else if (auto* lda = il::as<il::LdLoca>(inst)) {
        n = ir::make(lane_.arena, ir::Unary{.op = "&", .x = name_of(lda->variable())});
    } else if (auto* st = il::as<il::StLoc>(inst)) {
        ir::Node* target = name_of(st->variable());
        ir::TypeP type = target->type;
        n = ir::make(lane_.arena, ir::Assign{.target = target, .value = lower_new(st->value())}, std::move(type));
    } else {
        throw std::logic_error("L2Body: the builders made an instruction it does not mirror");
    }
    n->stack = static_cast<ir::Stack>(inst->result_type());
    map_.link(n, inst);
    return n;
}

void L2Body::replaced(il::ILInstruction* old, il::ILInstruction* now) {
    const ir::Node* was = map_.node(old);
    if (was == nullptr) throw std::logic_error("L2Body: an instruction with no node was replaced");
    // (a node in a statement place stands inside an ExprStmt, which stays)
    index_.replace(const_cast<ir::Node*>(was), lower_new(now));
}

void L2Body::inserted(il::Block* block, int at) {
    const ir::Node* b = map_.node(block);
    if (const auto* labeled = ir::get<ir::Labeled>(b)) b = std::get<ir::Node*>(labeled->body);   // a container's block: the Block in it
    ir::Node* stmt = lower_new(block->instructions[at]);
    if (ir::category(*stmt) == ir::Category::Expression) stmt = ir::make(lane_.arena, ir::ExprStmt{.expr = stmt});
    index_.insert_stmt(const_cast<ir::Node*>(b), static_cast<std::size_t>(at), stmt);
}

}  // namespace cb::dotnet::decompiler::csharp
