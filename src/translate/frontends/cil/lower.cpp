// translate/frontends/cil/lower.cpp -- the ILAst into L2 (see cil.hpp; the mapping is ILAST_TO_L2.md section 1).
#include <algorithm>
#include <bit>
#include <cmath>
#include <cstring>
#include <format>
#include <stdexcept>
#include <unordered_map>

#include "dotnet/decompiler/il/instructions.hpp"
#include "dotnet/decompiler/typesystem/tuple_type.hpp"
#include "dotnet/decompiler/typesystem/type_provider.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"
#include "translate/frontends/cil/cil.hpp"
#include "translate/frontends/cil/cil_names.hpp"
#include "translate/frontends/cil/flat_map.hpp"

namespace cb::translate::frontends::cil {

namespace cil_lowering {

namespace il = dotnet::decompiler::il;
namespace ts = dotnet::decompiler::typesystem;
using ir::Node;

enum class Ctx : std::uint8_t { Stmt, Expr };

// A Nop the ILAst makes where a slot is empty (an `if` without `else`, a `leave` without a value, a block's final instruction): no
// comment, kind Normal, no IL range. Any other Nop is lowered (an Empty).
bool is_plain_nop(const il::ILInstruction* inst) {
    const auto* nop = il::as<il::Nop>(inst);
    return nop != nullptr && nop->comment.empty() && nop->kind == il::NopKind::Normal && nop->start_il_offset() == 0 && nop->end_il_offset() == 0;
}

}  // namespace cil_lowering

struct Lowering::State {
    Arena& arena;
    LoweringStats stats;
    LiftMap* links = nullptr;   // what was made from what, when asked for (the function lowered last)
    FlatMap<const void*, ir::EntityP> entities;
    FlatMap<const ts_ns::IType*, ir::TypeP> types;
    FlatMap<const void*, ir::SymP> members;        // IMethod / IField / IMember / IParameter
    struct MethodFacts {
        ir::TypeP owner;                                       // the declaring type (a constructor call's type)
        std::vector<ir::TypeArg> typeargs;
    };
    std::unordered_map<const ts_ns::IMethod*, MethodFacts> method_facts;
    // what lives only as long as the function's ILArena: its variables, and the members a transform made in it (the dynamic awaiter
    // methods of AsyncAwaitDecompiler, declared on `dynamic`); the rest belongs to the type system and is shared by every function
    FlatMap<const void*, ir::SymP> function_members;
    FlatMap<const void*, ir::EntityP> function_entities;
    std::unordered_map<const ts_ns::IMethod*, MethodFacts> function_facts;
    FlatMap<const il_ns::ILVariable*, ir::SymP> variables;
    // The function's spans: one shared block of Origins per function, each node's `origin` an aliasing pointer into it (no allocation
    // per span), the same span the same Origin ([start, end) as start << 32 | end).
    struct Spans {
        std::vector<std::unique_ptr<ir::Origin[]>> chunks;   // of 16, 32, 64 ... (most functions are small)
        std::size_t used = 0;                                 // of the last chunk
        std::size_t size = 0;                                 // the last chunk's
    };
    std::shared_ptr<Spans> spans;
    FlatMap<std::uint64_t, const ir::Origin*> origins;
    // per top-level function
    FlatMap<const il_ns::Block*, std::string> labels;
    FlatMap<std::uint64_t, int> label_uses;                   // by the block's start offset (+ 1): the blocks labelled from it so far
    std::vector<il_ns::ILInstruction*> walk_order;
    std::vector<const il_ns::ILFunction*> functions;          // the functions being lowered, innermost last

    explicit State(Arena& a) : arena(a) {}

    ir::EntityP entity(ir::EntityKind kind, const void* object, bool shared = true);
    static bool of_function(const ts_ns::IMethod* m) {
        const ts_ns::IType* d = m->declaring_type();
        return d != nullptr && d->kind() == ts_ns::TypeKind::Dynamic;
    }
    ir::TypeP type(const ts_ns::IType* t);
    ir::TypeP stack_type(il_ns::StackType st);
    ir::TypeP prim_target(il_ns::PrimitiveType p, bool* is_r);
    ir::SymP method(const ts_ns::IMethod* m);
    MethodFacts& facts(const ts_ns::IMethod* m) {
        auto [it, fresh] = (of_function(m) ? function_facts : method_facts).try_emplace(m);
        if (fresh) {
            it->second.owner = type(m->declaring_type());
            for (const ts_ns::IType* ta : m->type_arguments()) it->second.typeargs.emplace_back(type(ta));
        }
        return it->second;
    }
    const ir::TypeP& method_owner(const ts_ns::IMethod* m) { return facts(m).owner; }
    const std::vector<ir::TypeArg>& method_typeargs(const ts_ns::IMethod* m) { return facts(m).typeargs; }
    ir::SymP field(const ts_ns::IField* f);
    ir::SymP member(const ts_ns::IMember* m);
    ir::SymP parameter(const ts_ns::IParameter* p);
    ir::SymP variable(const il_ns::ILVariable* v);
    void fill_variable(ir::Sym& sym, const il_ns::ILVariable& v);
    std::shared_ptr<const ir::Origin> origin(const il_ns::ILInstruction& inst);

    void assign_labels(const il_ns::ILFunction& root);
    const std::string& label_of(const il_ns::Block* block);
    std::string container_label(const il_ns::BlockContainer* container);

    template <class K> ir::Node* make(K kind, const il_ns::ILInstruction& src, ir::TypeP type = {}) {
        Node* n = ir::make(arena, std::move(kind), std::move(type));
        n->origin = origin(src);
        n->stack = static_cast<ir::Stack>(src.result_type());
        ++stats.instructions;
        ++stats.nodes;
        if (links != nullptr) links->link(n, const_cast<il_ns::ILInstruction*>(&src));   // (the map's instructions are the reader's to change)
        return n;
    }
    template <class K> ir::Node* helper(K kind, ir::TypeP type = {}) {
        Node* n = ir::make(arena, std::move(kind), std::move(type));
        ++stats.nodes;
        return n;
    }

    Node* function(const il_ns::ILFunction& f);
    Node* lower(const il_ns::ILInstruction* inst, cil_lowering::Ctx ctx);
    Node* stmt(const il_ns::ILInstruction* inst);
    Node* expr(const il_ns::ILInstruction* inst) { return lower(inst, cil_lowering::Ctx::Expr); }
    Node* optional_expr(const il_ns::ILInstruction* inst) { return cil_lowering::is_plain_nop(inst) ? nullptr : expr(inst); }
    std::vector<Node*> exprs(const std::vector<il_ns::ILInstruction*>& insts);
    Node* name(const il_ns::ILVariable* v) {
        ir::SymP sym = variable(v);
        ir::TypeP t = sym->type;
        return helper(ir::Name{.sym = std::move(sym)}, std::move(t));
    }
    Node* container(const il_ns::BlockContainer& c);
    Node* block(const il_ns::Block& b, cil_lowering::Ctx ctx);
    Node* call(const il_ns::CallInstruction& c);
    Node* dynamic(const il_ns::ILInstruction& inst);
    void flags_of_call(Node& n, const il_ns::CallInstruction& c);
};

namespace cil_lowering {

using State = Lowering::State;

ir::EntityKind member_kind(const ts::IMember* m) {
    switch (m->symbol_kind()) {
        case ts::SymbolKind::Field: return ir::EntityKind::Field;
        case ts::SymbolKind::Property:
        case ts::SymbolKind::Indexer: return ir::EntityKind::Property;
        case ts::SymbolKind::Event: return ir::EntityKind::Event;
        default: return ir::EntityKind::Method;
    }
}

std::vector<std::pair<std::int64_t, std::int64_t>> ranges_of(const dotnet::decompiler::util::LongSet& set) {
    std::vector<std::pair<std::int64_t, std::int64_t>> out;
    out.reserve(set.intervals().size());
    for (const auto& i : set.intervals()) out.emplace_back(i.start, i.inclusive_end());
    return out;
}

ir::ArgMeta arg_meta(State& s, const il::CSharpArgumentInfo& info) {
    ir::ArgMeta m;
    if (!info.name.empty()) m.name = info.name;
    m.flags = static_cast<std::uint32_t>(info.flags);
    m.type = s.type(info.compile_time_type);
    return m;
}


}  // namespace cil_lowering

// ---- entities, types, symbols ----------------------------------------------------------------------------------------------------

ir::EntityP Lowering::State::entity(ir::EntityKind kind, const void* object, bool shared) {
    if (object == nullptr) return {};
    auto [e, fresh] = (shared ? entities : function_entities).get(object);
    if (fresh) e = std::make_shared<ClrEntity>(kind, object);
    return e;
}

std::shared_ptr<const ir::Origin> Lowering::State::origin(const il_ns::ILInstruction& inst) {
    const auto start = static_cast<std::uint32_t>(inst.start_il_offset());
    const auto end = static_cast<std::uint32_t>(inst.end_il_offset());
    if (start == 0 && end == 0) return {};
    auto [o, fresh] = origins.get((static_cast<std::uint64_t>(start) << 32) | end);
    if (fresh) {
        if (!spans) spans = std::make_shared<Spans>();
        if (spans->used == spans->size) {
            spans->size = spans->size == 0 ? 16 : std::min<std::size_t>(spans->size * 2, 4096);
            spans->chunks.push_back(std::make_unique<ir::Origin[]>(spans->size));
            spans->used = 0;
        }
        ir::Origin* made = &spans->chunks.back()[spans->used++];
        *made = ir::OriginSpan{start, end};
        o = made;
    }
    return std::shared_ptr<const ir::Origin>(spans, o);
}

ir::TypeP Lowering::State::type(const ts_ns::IType* t) {
    namespace ts = ts_ns;
    if (t == nullptr) return {};
    if (const ir::TypeP* known = types.find(t)) return *known;
    ir::Type::Body body = ir::AnyType{};
    switch (t->kind()) {
        case ts::TypeKind::Void: body = ir::VoidType{}; break;
        case ts::TypeKind::Null: body = ir::NullType{}; break;
        case ts::TypeKind::Dynamic: body = ir::DynamicType{}; break;
        case ts::TypeKind::Unknown:
        case ts::TypeKind::None:
        case ts::TypeKind::UnboundTypeArgument: body = ir::AnyType{}; break;
        case ts::TypeKind::TypeParameter:
            if (const auto* tp = dynamic_cast<const ts::ITypeParameter*>(t)) {
                body = ir::TypeVarType{.name = tp->name(), .index = static_cast<std::uint32_t>(tp->index()), .owner = std::nullopt,
                                       .of_method = tp->owner_type() == ts::SymbolKind::Method};
            }
            break;
        case ts::TypeKind::Array:
            if (const auto* a = dynamic_cast<const ts::ArrayType*>(t))
                body = ir::ArrayType{.elem = type(a->element_type()), .rank = static_cast<std::uint32_t>(a->dimensions())};
            break;
        case ts::TypeKind::Pointer:
            if (const auto* p = dynamic_cast<const ts::PointerType*>(t)) body = ir::PtrType{.to = type(p->element_type())};
            break;
        case ts::TypeKind::ByReference:
            if (const auto* p = dynamic_cast<const ts::ByReferenceType*>(t)) body = ir::PtrType{.to = type(p->element_type()), .managed = true};
            break;
        case ts::TypeKind::ModOpt:
        case ts::TypeKind::ModReq:
            if (const auto* m = dynamic_cast<const ts::ModifiedType*>(t))
                body = ir::ModType{.inner = type(m->element_type()), .required = t->kind() == ts::TypeKind::ModReq, .modifier = type(m->modifier())};
            break;
        case ts::TypeKind::Tuple:
            if (const auto* tu = dynamic_cast<const ts::TupleType*>(t)) {
                ir::TupleType tt;
                for (const ts::IType* e : tu->element_types()) tt.elems.push_back(type(e));
                tt.names = tu->element_names();
                body = std::move(tt);
            }
            break;
        case ts::TypeKind::FunctionPointer:
            if (const auto* fp = dynamic_cast<const ts::FunctionPointerType*>(t)) {
                ir::FnType fn;
                for (const ts::IType* p : fp->parameter_types) fn.params.push_back(ir::FnParam{.type = type(p)});
                fn.result.type = type(fp->return_type);
                body = ir::FnPtrType{.fn = ir::make_type(std::move(fn))};
            }
            break;
        case ts::TypeKind::NInt: body = ir::ClassType{.name = "System.IntPtr"}; break;
        case ts::TypeKind::NUInt: body = ir::ClassType{.name = "System.UIntPtr"}; break;
        default: {
            const ts::ITypeDefinition* def = t->get_definition();
            const ts::KnownTypeCode code = def != nullptr ? def->known_type_code() : ts::KnownTypeCode::None;
            switch (code) {
                case ts::KnownTypeCode::Boolean: body = ir::PrimType{.prim = ir::Prim::Bool}; break;
                case ts::KnownTypeCode::Char: body = ir::PrimType{.prim = ir::Prim::U16, .encoding = ir::CharEncoding::Utf16}; break;
                case ts::KnownTypeCode::SByte: body = ir::PrimType{.prim = ir::Prim::I8}; break;
                case ts::KnownTypeCode::Byte: body = ir::PrimType{.prim = ir::Prim::U8}; break;
                case ts::KnownTypeCode::Int16: body = ir::PrimType{.prim = ir::Prim::I16}; break;
                case ts::KnownTypeCode::UInt16: body = ir::PrimType{.prim = ir::Prim::U16}; break;
                case ts::KnownTypeCode::Int32: body = ir::PrimType{.prim = ir::Prim::I32}; break;
                case ts::KnownTypeCode::UInt32: body = ir::PrimType{.prim = ir::Prim::U32}; break;
                case ts::KnownTypeCode::Int64: body = ir::PrimType{.prim = ir::Prim::I64}; break;
                case ts::KnownTypeCode::UInt64: body = ir::PrimType{.prim = ir::Prim::U64}; break;
                case ts::KnownTypeCode::Single: body = ir::PrimType{.prim = ir::Prim::F32}; break;
                case ts::KnownTypeCode::Double: body = ir::PrimType{.prim = ir::Prim::F64}; break;
                case ts::KnownTypeCode::Void: body = ir::VoidType{}; break;
                default: {
                    ir::ClassType c{.name = t->full_name()};
                    for (const ts::IType* arg : t->type_arguments()) c.args.push_back(type(arg));
                    c.nullable = t->nullability() == ts::Nullability::Nullable;
                    body = std::move(c);
                }
            }
        }
    }
    ir::TypeP made = ir::make_type(std::move(body), entity(ir::EntityKind::Type, t));
    types.get(t).value = made;
    return made;
}

// The canonical type of a stack kind (a hint where the instruction states no type of its own); shared, with no entity.
ir::TypeP Lowering::State::stack_type(il_ns::StackType st) {
    static const ir::TypeP kI = ir::class_type("System.IntPtr");
    static const ir::TypeP kI8 = ir::prim_type(ir::Prim::I64);
    static const ir::TypeP kF4 = ir::prim_type(ir::Prim::F32);
    static const ir::TypeP kF8 = ir::prim_type(ir::Prim::F64);
    static const ir::TypeP kO = ir::class_type("System.Object");
    static const ir::TypeP kRef = ir::make_type(ir::PtrType{.to = ir::any_type(), .managed = true});
    switch (st) {
        case il_ns::StackType::I4: return ir::i32_type();
        case il_ns::StackType::I: return kI;
        case il_ns::StackType::I8: return kI8;
        case il_ns::StackType::F4: return kF4;
        case il_ns::StackType::F8: return kF8;
        case il_ns::StackType::O: return kO;
        case il_ns::StackType::Ref: return kRef;
        case il_ns::StackType::Void: return ir::void_type();
        default: return {};
    }
}

ir::TypeP Lowering::State::prim_target(il_ns::PrimitiveType p, bool* is_r) {
    *is_r = p == il_ns::PrimitiveType::R;
    return conv_target_type(p);
}

ir::SymP Lowering::State::method(const ts_ns::IMethod* m) {
    namespace ts = ts_ns;
    if (m == nullptr) return {};
    const bool shared = !of_function(m);
    auto& cache = shared ? members : function_members;
    if (const ir::SymP* known = cache.find(m)) return *known;
    auto sym = std::make_shared<ir::Sym>();
    sym->kind = m->is_constructor() ? ir::SymKind::Ctor : ir::SymKind::Method;
    sym->name = m->name();
    if (const ts::IType* d = m->declaring_type()) sym->owner = d->full_name();
    sym->is_static = m->is_static();
    for (const ts::IParameter* p : m->parameters()) {
        sym->params.push_back(type(p->type()));
        switch (p->reference_kind()) {
            case ts::ReferenceKind::Ref:
            case ts::ReferenceKind::RefReadOnly: sym->refs.push_back(ir::PassMode::Ref); break;
            case ts::ReferenceKind::Out: sym->refs.push_back(ir::PassMode::Out); break;
            case ts::ReferenceKind::In: sym->refs.push_back(ir::PassMode::In); break;
            default: sym->refs.push_back(ir::PassMode::ByValue); break;
        }
    }
    bool any_ref = false;
    for (ir::PassMode r : sym->refs) any_ref = any_ref || r != ir::PassMode::ByValue;
    if (!any_ref) sym->refs.clear();
    sym->ret = type(m->return_type());
    if (m->is_extension_method()) sym->flags.emplace_back("extension");
    if (m->is_operator()) sym->flags.emplace_back("operator");
    if (m->is_virtual()) sym->flags.emplace_back("virtual");
    if (m->is_abstract()) sym->flags.emplace_back("abstract");
    if (m->is_override()) sym->flags.emplace_back("override");
    if (m->is_accessor()) sym->flags.emplace_back("accessor");
    sym->entity = entity(ir::EntityKind::Method, m, shared);
    cache.get(m).value = sym;
    return sym;
}

ir::SymP Lowering::State::field(const ts_ns::IField* f) {
    if (f == nullptr) return {};
    if (const ir::SymP* known = members.find(f)) return *known;
    auto sym = std::make_shared<ir::Sym>();
    sym->kind = ir::SymKind::Field;
    sym->name = f->name();
    if (const ts_ns::IType* d = f->declaring_type()) sym->owner = d->full_name();
    sym->is_static = f->is_static();
    sym->type = type(f->return_type());
    if (f->is_read_only()) sym->flags.emplace_back("readonly");
    sym->entity = entity(ir::EntityKind::Field, f);
    members.get(f).value = sym;
    return sym;
}

ir::SymP Lowering::State::member(const ts_ns::IMember* m) {
    if (m == nullptr) return {};
    const ir::EntityKind kind = cil_lowering::member_kind(m);
    if (kind == ir::EntityKind::Method) return method(dynamic_cast<const ts_ns::IMethod*>(m));
    if (kind == ir::EntityKind::Field) return field(dynamic_cast<const ts_ns::IField*>(m));
    if (const ir::SymP* known = members.find(m)) return *known;
    auto sym = std::make_shared<ir::Sym>();
    sym->kind = kind == ir::EntityKind::Event ? ir::SymKind::Event : ir::SymKind::Property;
    sym->name = m->name();
    if (const ts_ns::IType* d = m->declaring_type()) sym->owner = d->full_name();
    sym->is_static = m->is_static();
    sym->type = type(m->return_type());
    sym->entity = entity(kind, m);
    members.get(m).value = sym;
    return sym;
}

ir::SymP Lowering::State::parameter(const ts_ns::IParameter* p) {
    if (p == nullptr) return {};
    if (const ir::SymP* known = members.find(p)) return *known;
    auto sym = std::make_shared<ir::Sym>();
    sym->kind = ir::SymKind::Param;
    sym->name = p->name();
    sym->type = type(p->type());
    sym->entity = entity(ir::EntityKind::Parameter, p);
    members.get(p).value = sym;
    return sym;
}

ir::SymP Lowering::State::variable(const il_ns::ILVariable* v) {
    if (v == nullptr) return {};
    if (const ir::SymP* known = variables.find(v)) return *known;
    auto sym = std::make_shared<ir::Sym>();
    variables.get(v).value = sym;
    fill_variable(*sym, *v);
    if (links != nullptr) links->link(sym, const_cast<il_ns::ILVariable*>(v));
    return sym;
}

void Lowering::State::fill_variable(ir::Sym& sym, const il_ns::ILVariable& v) {
    using il_ns::VariableKind;
    const VarRoleOf role = var_role(v.kind(), v.is_this());
    sym.kind = role.role == ir::VarRole::Param || role.role == ir::VarRole::This ? ir::SymKind::Param : ir::SymKind::Local;
    sym.role = role.role;
    if (role.exact) sym.info.set("cil.kind_exact", std::string(*role.exact));
    sym.stack = static_cast<ir::Stack>(v.stack_type);
    sym.name = v.name.value_or("");
    if (v.name.has_value() && v.name->empty()) throw std::logic_error("cil lowering: a variable with an empty name");
    sym.type = type(v.type());
    if (v.index().has_value()) sym.index = *v.index();
    sym.generated_name = v.has_generated_name;
    sym.ref_readonly = v.is_ref_read_only;
    sym.entry_initialized = v.initial_value_is_initialized();
    sym.entry_used = v.uses_initial_value();
    if (v.capture_scope != nullptr) sym.info.set("cil.capture_scope", container_label(v.capture_scope));
    if (v.state_machine_field != nullptr) sym.info.set("cil.state_machine_field", entity(ir::EntityKind::Field, v.state_machine_field));
    if (v.remove_if_redundant) sym.info.set("cil.remove_if_redundant", true);
}

// ---- labels ---------------------------------------------------------------------------------------------------------------------

void Lowering::State::assign_labels(const il_ns::ILFunction& root) {
    labels.clear();
    label_uses.clear();
    std::vector<il_ns::ILInstruction*>& order = walk_order;
    order.assign(1, const_cast<il_ns::ILFunction*>(&root));
    // preorder: a block gets its label before the blocks inside it. A label is the block's IL_xxxx, which names its start offset; the
    // second block from one offset gets IL_xxxx_2, the third _3 (a suffixed label never equals an unsuffixed one).
    while (!order.empty()) {
        il_ns::ILInstruction* inst = order.back();
        order.pop_back();
        if (auto* c = il_ns::as<il_ns::BlockContainer>(inst)) {
            for (il_ns::Block* b : c->blocks) {
                const int uses = ++label_uses.get(static_cast<std::uint64_t>(static_cast<std::uint32_t>(b->start_il_offset())) + 1).value;
                std::string label = b->label();
                if (uses > 1) {
                    ++stats.labels_uniquified;
                    label += '_';
                    label += std::to_string(uses);
                }
                labels.get(b).value = std::move(label);
            }
        }
        const int n = inst->get_child_count();
        for (int i = n - 1; i >= 0; --i) {
            if (il_ns::ILInstruction* child = inst->get_child(i)) order.push_back(child);
        }
    }
}

const std::string& Lowering::State::label_of(const il_ns::Block* block) {
    // (the table is complete before the first look-up: no insertion moves what this returns)
    const std::string* label = labels.find(block);
    if (label == nullptr) throw std::logic_error("cil lowering: a branch to a block that is not in the function");
    return *label;
}

std::string Lowering::State::container_label(const il_ns::BlockContainer* c) {
    if (c->blocks.count() == 0) throw std::logic_error("cil lowering: a container without blocks");
    return label_of(c->blocks[0]);
}

// ---- functions --------------------------------------------------------------------------------------------------------------------

Node* Lowering::State::function(const il_ns::ILFunction& f) {
    namespace il = il_ns;
    using il::ILFunctionKind;
    ++stats.functions;
    functions.push_back(&f);
    for (const il::ILVariable* v : f.variables()) variable(v);
    std::vector<Node*> params;
    for (const ts_ns::IParameter* p : f.parameters) params.push_back(helper(ir::Param{.sym = parameter(p), .type = parameter(p)->type}));
    std::vector<ir::SymP> locals;
    locals.reserve(static_cast<std::size_t>(f.variables().count()));
    for (const il::ILVariable* v : f.variables()) locals.push_back(variable(v));
    std::vector<ir::SymP> captures;
    for (const il::ILVariable* v : f.captured_variables) captures.push_back(variable(v));
    std::vector<std::string> mods;
    if (f.kind() == ILFunctionKind::LocalFunction) mods.emplace_back("local");
    if (f.kind() == ILFunctionKind::ExpressionTree) mods.emplace_back("expression_tree");
    if (f.is_async()) mods.emplace_back("async");
    if (f.is_iterator) mods.emplace_back("iterator");
    std::optional<std::string> scope;
    if (f.declaration_scope != nullptr) scope = container_label(f.declaration_scope);
    Node* body = f.body() != nullptr ? stmt(f.body()) : nullptr;
    std::vector<Node*> nested;
    for (const il::ILFunction* lf : f.local_functions) nested.push_back(function(*lf));

    Node* n;
    const bool lambda = f.kind() == ILFunctionKind::Delegate || f.kind() == ILFunctionKind::ExpressionTree;
    if (lambda) {
        n = make(ir::Lambda{.params = std::move(params), .body = body, .locals = std::move(locals), .captures = std::move(captures),
                            .scope = std::move(scope), .comments = f.warnings, .sym = method(f.method), .mods = std::move(mods),
                            .nested = std::move(nested), .ret = type(f.return_type)},
                 f, type(f.delegate_type));
    } else {
        n = make(ir::MethodDecl{.sym = method(f.method), .name = f.name, .kind = std::string(f.method != nullptr && f.method->is_constructor() ? "ctor" : "method"),
                                .params = std::move(params), .ret = type(f.return_type), .body = body, .mods = std::move(mods),
                                .comments = f.warnings, .locals = std::move(locals), .captures = std::move(captures),
                                .nested = std::move(nested), .scope = std::move(scope)},
                 f, type(f.delegate_type));
    }
    if (lambda && f.method == nullptr && f.name.empty()) {
        // nothing: the name of an expression tree is empty
    } else if (lambda && (f.method == nullptr || f.name != f.method->name())) {
        n->info.set("cil.name", f.name);
    }
    if (f.code_size != 0) n->info.set("cil.code_size", static_cast<std::int64_t>(f.code_size));
    if (f.local_variable_signature_length != 0) n->info.set("cil.locals_sig_len", static_cast<std::int64_t>(f.local_variable_signature_length));
    if (f.helper_variable_count() != 0) n->info.set("cil.helper_count", static_cast<std::int64_t>(f.helper_variable_count()));
    if (f.state_machine_compiled_with_mono) n->info.set("cil.mono_state_machine", true);
    if (f.state_machine_compiled_with_legacy_visual_basic) n->info.set("cil.vb_state_machine", true);
    if (f.async_return_type != nullptr) n->info.set("cil.async_return_type", type(f.async_return_type));
    if (f.move_next_method != nullptr) n->info.set("cil.move_next", entity(ir::EntityKind::Method, f.move_next_method));
    if (f.reduced_method != nullptr) n->info.set("cil.reduced_method", entity(ir::EntityKind::Method, f.reduced_method));
    if (f.sequence_point_candidates.has_value()) {
        std::vector<std::int64_t> points(f.sequence_point_candidates->begin(), f.sequence_point_candidates->end());
        n->info.set("cil.seq_points", std::move(points));
    }
    if (!f.async_debug_info.awaits_is_default) {
        n->info.set("cil.async_catch_offset", static_cast<std::int64_t>(f.async_debug_info.catch_handler_offset));
        if (!f.async_debug_info.awaits.empty()) throw std::logic_error("cil lowering: async debug info with awaits");
    }
    // the generic context: derived (the method's own, or the enclosing function's for one without a method) unless it is not
    const ts_ns::GenericContext derived = f.method != nullptr ? ts_ns::GenericContext::of(*f.method)
                                          : functions.size() > 1 ? functions[functions.size() - 2]->generic_context
                                                                 : ts_ns::GenericContext();
    if (f.generic_context.class_type_parameters != derived.class_type_parameters ||
        f.generic_context.method_type_parameters != derived.method_type_parameters) {
        const auto list = [&](const std::vector<const ts_ns::ITypeParameter*>* tps) {
            std::vector<ir::TypeP> out;
            if (tps != nullptr) {
                for (const ts_ns::ITypeParameter* tp : *tps) out.push_back(type(tp));
            }
            return out;
        };
        n->info.set("cil.class_tparams", list(f.generic_context.class_type_parameters));
        n->info.set("cil.method_tparams", list(f.generic_context.method_type_parameters));
    }
    functions.pop_back();
    return n;
}

// ---- instructions -----------------------------------------------------------------------------------------------------------------

Node* Lowering::State::stmt(const il_ns::ILInstruction* inst) {
    Node* n = lower(inst, cil_lowering::Ctx::Stmt);
    if (ir::category(*n) == ir::Category::Expression) n = helper(ir::ExprStmt{.expr = n});
    return n;
}

std::vector<Node*> Lowering::State::exprs(const std::vector<il_ns::ILInstruction*>& insts) {
    std::vector<Node*> out;
    out.reserve(insts.size());
    for (const il_ns::ILInstruction* i : insts) out.push_back(expr(i));
    return out;
}

Node* Lowering::State::container(const il_ns::BlockContainer& c) {
    ir::Region region{.kind = region_kind(c.kind())};
    region.blocks.reserve(static_cast<std::size_t>(c.blocks.count()));
    for (const il_ns::Block* b : c.blocks) {
        if (b->kind != il_ns::BlockKind::ControlFlow) throw std::logic_error("cil lowering: a container's block is not a control-flow block");
        Node* body = block(*b, cil_lowering::Ctx::Stmt);
        Node* labeled = ir::make(arena, ir::Labeled{.label = label_of(b), .body = body});
        ++stats.nodes;
        if (links != nullptr) links->link(labeled, const_cast<il_ns::Block*>(b));   // the block is the Labeled, as the lifting links it
        // the Labeled is the block: its range, its stack kind (Void); the Block inside it carries the statements only
        labeled->origin = body->origin;
        labeled->stack = body->stack;
        body->origin.reset();
        body->stack = ir::Stack::Unknown;
        region.blocks.push_back(labeled);
    }
    if (c.entry_point() != (c.blocks.count() > 0 ? c.blocks[0] : nullptr)) throw std::logic_error("cil lowering: a container whose entry is not its first block");
    return make(std::move(region), c);
}

Node* Lowering::State::block(const il_ns::Block& b, cil_lowering::Ctx ctx) {
    std::vector<Node*> stmts;
    stmts.reserve(static_cast<std::size_t>(b.instructions.count()));
    for (const il_ns::ILInstruction* i : b.instructions) stmts.push_back(stmt(i));
    const bool plain_final = cil_lowering::is_plain_nop(b.final_instruction());
    if (b.kind == il_ns::BlockKind::ControlFlow && ctx == cil_lowering::Ctx::Stmt && plain_final) return make(ir::Block{.stmts = std::move(stmts)}, b);
    ir::BlockExpr e{.stmts = std::move(stmts), .result = plain_final ? nullptr : expr(b.final_instruction())};
    if (b.kind != il_ns::BlockKind::ControlFlow) e.role = std::string(block_role(b.kind));
    return make(std::move(e), b, stack_type(b.result_type()));
}

void Lowering::State::flags_of_call(Node& n, const il_ns::CallInstruction& c) {
    if (c.is_tail) ir::set_flag(n, ir::NodeFlag::Tail);
    if (c.il_stack_was_empty) ir::set_flag(n, ir::NodeFlag::StackEmpty);
    if (c.constrained_to != nullptr) n.info.set("cil.constrained", type(c.constrained_to));
}

Node* Lowering::State::call(const il_ns::CallInstruction& c) {
    namespace il = il_ns;
    std::vector<Node*> args;
    Node* target = nullptr;
    const auto& arguments = c.arguments();
    int first = 0;
    const ir::SymP sym = method(c.method);
    if (c.op_code != il::OpCode::NewObj && !sym->is_static && arguments.count() > 0) {
        target = expr(arguments[0]);
        first = 1;
    }
    for (int i = first; i < arguments.count(); ++i) args.push_back(expr(arguments[i]));
    Node* n;
    if (c.op_code == il::OpCode::NewObj) {
        n = make(ir::New{.sym = sym, .args = std::move(args)}, c, method_owner(c.method));
    } else {
        ir::Call k{.target = target, .sym = sym, .args = std::move(args),
                   .form = c.op_code == il::OpCode::CallVirt ? ir::CallForm::Virtual : ir::CallForm::Direct};
        k.typeargs = method_typeargs(c.method);
        n = make(std::move(k), c, sym->ret);
    }
    flags_of_call(*n, c);
    return n;
}

Node* Lowering::State::dynamic(const il_ns::ILInstruction& inst) {
    namespace il = il_ns;
    const auto& d = static_cast<const il::DynamicInstruction&>(inst);
    ir::Intrinsic k;
    ir::TypeP node_type;
    const auto binder = static_cast<std::int64_t>(d.binder_flags());
    switch (inst.op_code) {
        case il::OpCode::DynamicLogicOperatorInstruction:
        case il::OpCode::DynamicBinaryOperatorInstruction: {
            const bool logic = inst.op_code == il::OpCode::DynamicLogicOperatorInstruction;
            const auto* lo = logic ? &static_cast<const il::DynamicLogicOperatorInstruction&>(inst) : nullptr;
            const auto* bo = logic ? nullptr : &static_cast<const il::DynamicBinaryOperatorInstruction&>(inst);
            k.name = logic ? "cs.dynamic.logic" : "cs.dynamic.binary";
            k.args = {expr(logic ? lo->left() : bo->left()), expr(logic ? lo->right() : bo->right())};
            k.imms = {binder, static_cast<std::int64_t>(logic ? lo->operation() : bo->operation())};
            k.argmeta = {cil_lowering::arg_meta(*this, logic ? lo->left_argument_info() : bo->left_argument_info()),
                         cil_lowering::arg_meta(*this, logic ? lo->right_argument_info() : bo->right_argument_info())};
            break;
        }
        case il::OpCode::DynamicUnaryOperatorInstruction: {
            const auto& u = static_cast<const il::DynamicUnaryOperatorInstruction&>(inst);
            k.name = "cs.dynamic.unary";
            k.args = {expr(u.operand())};
            k.imms = {binder, static_cast<std::int64_t>(u.operation())};
            k.argmeta = {cil_lowering::arg_meta(*this, u.operand_argument_info())};
            break;
        }
        case il::OpCode::DynamicConvertInstruction: {
            const auto& c = static_cast<const il::DynamicConvertInstruction&>(inst);
            k.name = "cs.dynamic.convert";
            k.args = {expr(c.argument())};
            k.targs = {type(c.type())};
            k.imms = {binder};
            break;
        }
        case il::OpCode::DynamicGetMemberInstruction: {
            const auto& g = static_cast<const il::DynamicGetMemberInstruction&>(inst);
            k.name = "cs.dynamic.get_member";
            k.args = {expr(g.target())};
            k.strs = {g.name()};
            k.imms = {binder};
            k.argmeta = {cil_lowering::arg_meta(*this, g.target_argument_info())};
            break;
        }
        case il::OpCode::DynamicSetMemberInstruction: {
            const auto& sm = static_cast<const il::DynamicSetMemberInstruction&>(inst);
            k.name = "cs.dynamic.set_member";
            k.args = {expr(sm.target()), expr(sm.value())};
            k.strs = {sm.name()};
            k.imms = {binder};
            k.argmeta = {cil_lowering::arg_meta(*this, sm.target_argument_info()), cil_lowering::arg_meta(*this, sm.value_argument_info())};
            break;
        }
        case il::OpCode::DynamicIsEventInstruction: {
            const auto& e = static_cast<const il::DynamicIsEventInstruction&>(inst);
            k.name = "cs.dynamic.is_event";
            k.args = {expr(e.argument())};
            k.strs = {e.name()};
            k.imms = {binder};
            break;
        }
        default: {
            const auto& a = static_cast<const il::DynamicArgumentsInstruction&>(inst);
            switch (inst.op_code) {
                case il::OpCode::DynamicGetIndexInstruction: k.name = "cs.dynamic.get_index"; break;
                case il::OpCode::DynamicSetIndexInstruction: k.name = "cs.dynamic.set_index"; break;
                case il::OpCode::DynamicInvokeInstruction: k.name = "cs.dynamic.invoke"; break;
                case il::OpCode::DynamicInvokeMemberInstruction: {
                    const auto& m = static_cast<const il::DynamicInvokeMemberInstruction&>(inst);
                    k.name = "cs.dynamic.invoke_member";
                    k.strs = {m.name()};
                    for (const ts_ns::IType* t : m.type_arguments()) k.targs.push_back(type(t));
                    break;
                }
                case il::OpCode::DynamicInvokeConstructorInstruction:
                    k.name = "cs.dynamic.invoke_constructor";
                    node_type = type(static_cast<const il::DynamicInvokeConstructorInstruction&>(inst).constructed_type());
                    break;
                default: throw std::logic_error("cil lowering: an unknown dynamic instruction");
            }
            for (const il::ILInstruction* arg : a.arguments()) k.args.push_back(expr(arg));
            k.imms = {binder};
            for (const il::CSharpArgumentInfo& info : a.argument_info()) k.argmeta.push_back(cil_lowering::arg_meta(*this, info));
        }
    }
    const bool context = d.calling_context() != nullptr;
    if (context) k.targs.push_back(type(d.calling_context()));
    Node* n = make(std::move(k), inst, std::move(node_type));
    if (context) ir::set_flag(*n, ir::NodeFlag::HasContext);
    return n;
}

Node* Lowering::State::lower(const il_ns::ILInstruction* inst_ptr, cil_lowering::Ctx ctx) {
    namespace il = il_ns;
    using il::OpCode;
    using cil_lowering::Ctx;
    if (inst_ptr == nullptr) throw std::logic_error("cil lowering: a null instruction");
    const il::ILInstruction& inst = *inst_ptr;
    switch (inst.op_code) {
        case OpCode::InvalidBranch: {
            const auto& i = static_cast<const il::InvalidBranch&>(inst);
            ir::Unsupported k{.kind = std::string("il.invalid_branch")};
            if (!i.message.empty()) k.raw = i.message;
            return make(std::move(k), inst);
        }
        case OpCode::InvalidExpression: {
            const auto& i = static_cast<const il::InvalidExpression&>(inst);
            ir::Unsupported k{.kind = std::string("il.invalid_expression")};
            if (!i.message.empty()) k.raw = i.message;
            Node* n = make(std::move(k), inst);
            if (i.severity != "Error") n->info.set("cil.severity", i.severity);
            return n;
        }
        case OpCode::Nop: {
            const auto& i = static_cast<const il::Nop&>(inst);
            ir::Empty k{.kind = static_cast<ir::EmptyKind>(i.kind)};
            if (!i.comment.empty()) k.note = i.comment;
            return make(std::move(k), inst);
        }
        case OpCode::ILFunction: return function(static_cast<const il::ILFunction&>(inst));
        case OpCode::BlockContainer: return container(static_cast<const il::BlockContainer&>(inst));
        case OpCode::Block: return block(static_cast<const il::Block&>(inst), ctx);
        case OpCode::PinnedRegion: {
            const auto& i = static_cast<const il::PinnedRegion&>(inst);
            Node* var = helper(ir::Local{.sym = variable(i.variable()), .type = variable(i.variable())->type, .init = expr(i.init())});
            return make(ir::Fixed{.var = var, .body = stmt(i.body())}, inst);
        }
        case OpCode::BinaryNumericInstruction: {
            const auto& i = static_cast<const il::BinaryNumericInstruction&>(inst);
            ir::Binary k{.op = std::string(binary_op_text(i.op, i.sign)), .left = expr(i.left()), .right = expr(i.right())};
            if (i.check_for_overflow) k.overflow = ir::Overflow::Trap;
            k.is_signed = sign_to(i.sign);
            k.left_stk = static_cast<ir::Stack>(i.left_input_type);
            k.right_stk = static_cast<ir::Stack>(i.right_input_type);
            if (i.is_lifted()) k.lift = ir::Lift::CSharp;
            return make(std::move(k), inst, stack_type(inst.result_type()));
        }
        case OpCode::NumericCompoundAssign: {
            const auto& i = static_cast<const il::NumericCompoundAssign&>(inst);
            ir::Assign k{.op = std::string(binary_op_text(i.op, i.sign)), .target = expr(i.target()), .value = expr(i.value())};
            if (i.check_for_overflow) k.overflow = ir::Overflow::Trap;
            k.eval = static_cast<ir::Eval>(i.eval_mode);
            k.via = static_cast<ir::Via>(i.target_kind);
            k.is_signed = sign_to(i.sign);
            k.left_stk = static_cast<ir::Stack>(i.left_input_type);
            k.right_stk = static_cast<ir::Stack>(i.right_input_type);
            if (i.is_lifted()) k.lift = ir::Lift::CSharp;
            return make(std::move(k), inst, type(i.type()));
        }
        case OpCode::UserDefinedCompoundAssign: {
            const auto& i = static_cast<const il::UserDefinedCompoundAssign&>(inst);
            ir::Assign k{.target = expr(i.target()), .value = expr(i.value())};
            k.eval = static_cast<ir::Eval>(i.eval_mode);
            k.via = static_cast<ir::Via>(i.target_kind);
            k.sym = method(i.method);
            return make(std::move(k), inst, type(i.method->return_type()));
        }
        case OpCode::DynamicCompoundAssign: {
            const auto& i = static_cast<const il::DynamicCompoundAssign&>(inst);
            ir::Intrinsic k{.name = "cs.dynamic.compound_assign", .args = {expr(i.target()), expr(i.value())}};
            k.imms = {static_cast<std::int64_t>(i.binder_flags()), static_cast<std::int64_t>(i.operation()), static_cast<std::int64_t>(i.target_kind),
                      static_cast<std::int64_t>(i.eval_mode)};
            k.argmeta = {cil_lowering::arg_meta(*this, i.target_argument_info()), cil_lowering::arg_meta(*this, i.value_argument_info())};
            return make(std::move(k), inst);
        }
        case OpCode::BitNot: {
            const auto& i = static_cast<const il::BitNot&>(inst);
            ir::Unary k{.op = "~", .x = expr(i.argument())};
            k.left_stk = static_cast<ir::Stack>(i.underlying_result_type());
            if (i.is_lifted()) k.lift = ir::Lift::CSharp;
            return make(std::move(k), inst, stack_type(inst.result_type()));
        }
        case OpCode::Arglist: return make(ir::Intrinsic{.name = "cil.arglist"}, inst);
        case OpCode::Branch: return make(ir::Goto{.label = label_of(static_cast<const il::Branch&>(inst).target_block())}, inst);
        case OpCode::Leave: {
            const auto& i = static_cast<const il::Leave&>(inst);
            Node* value = optional_expr(i.value());
            if (i.target_container() == functions.back()->body()) return make(ir::Return{.expr = value}, inst);
            return make(ir::Break{.label = container_label(i.target_container()), .value = value}, inst);
        }
        case OpCode::IfInstruction: {
            const auto& i = static_cast<const il::IfInstruction&>(inst);
            if (ctx == Ctx::Stmt) {
                ir::If k{.cond = expr(i.condition()), .then = stmt(i.true_inst())};
                if (!cil_lowering::is_plain_nop(i.false_inst())) k.orelse = stmt(i.false_inst());
                return make(std::move(k), inst);
            }
            return make(ir::Cond{.cond = expr(i.condition()), .then = expr(i.true_inst()), .orelse = optional_expr(i.false_inst())}, inst,
                        stack_type(inst.result_type()));
        }
        case OpCode::NullCoalescingInstruction: {
            const auto& i = static_cast<const il::NullCoalescingInstruction&>(inst);
            ir::Binary k{.op = "??", .left = expr(i.value_inst()), .right = expr(i.fallback_inst())};
            k.coalesce = static_cast<ir::Coalesce>(i.kind);
            k.left_stk = static_cast<ir::Stack>(i.underlying_result_type);
            return make(std::move(k), inst, stack_type(inst.result_type()));
        }
        case OpCode::SwitchInstruction: {
            const auto& i = static_cast<const il::SwitchInstruction&>(inst);
            std::vector<Node*> cases;
            cases.reserve(static_cast<std::size_t>(i.sections.count()));
            for (const il::SwitchSection* sec : i.sections) {
                Node* body;
                if (ctx == Ctx::Stmt) {
                    body = stmt(sec->body());
                } else {
                    body = expr(sec->body());
                    if (ir::category(*body) == ir::Category::Expression) body = helper(ir::Yield{.expr = body});
                }
                Node* c = make(ir::Case{.body = {body}, .ranges = cil_lowering::ranges_of(sec->labels()), .null_label = sec->has_null_label()}, *sec);
                if (sec->is_compiler_generated_default_section()) ir::set_flag(*c, ir::NodeFlag::CompilerGenerated);
                cases.push_back(c);
            }
            if (ctx == Ctx::Stmt)
                return make(ir::Switch{.expr = expr(i.value()), .cases = std::move(cases), .lifted = i.is_lifted, .of = type(i.type)}, inst);
            return make(ir::SwitchExpr{.expr = expr(i.value()), .cases = std::move(cases), .lifted = i.is_lifted, .of = type(i.type)}, inst,
                        stack_type(inst.result_type()));
        }
        case OpCode::TryCatch: {
            const auto& i = static_cast<const il::TryCatch&>(inst);
            ir::Try k{.body = stmt(i.try_block())};
            for (const il::TryCatchHandler* h : i.handlers) {
                ir::Catch c{.sym = variable(h->variable()), .types = {variable(h->variable())->type}, .body = stmt(h->body()), .filter = expr(h->filter())};
                Node* cn = make(std::move(c), *h);
                const dotnet::decompiler::util::Interval spec = h->exception_specifier_il_range();
                if (spec.start != 0 || spec.end != 0) cn->info.set("cil.spec_range", std::vector<std::int64_t>{spec.start, spec.end});
                k.catches.push_back(cn);
            }
            return make(std::move(k), inst);
        }
        case OpCode::TryFinally: {
            const auto& i = static_cast<const il::TryFinally&>(inst);
            return make(ir::Try{.body = stmt(i.try_block()), .finally_body = stmt(i.finally_block())}, inst);
        }
        case OpCode::TryFault: {
            const auto& i = static_cast<const il::TryFault&>(inst);
            return make(ir::Try{.body = stmt(i.try_block()), .fault_body = stmt(i.fault_block())}, inst);
        }
        case OpCode::LockInstruction: {
            const auto& i = static_cast<const il::LockInstruction&>(inst);
            return make(ir::Sync{.lock = expr(i.on_expression()), .body = stmt(i.body())}, inst);
        }
        case OpCode::UsingInstruction: {
            const auto& i = static_cast<const il::UsingInstruction&>(inst);
            ir::Local res{.sym = variable(i.variable()), .type = variable(i.variable())->type, .init = expr(i.resource_expression())};
            if (i.is_async()) res.mods.emplace_back("await");
            if (i.is_ref_struct()) res.mods.emplace_back("ref_struct");
            Node* resource = helper(std::move(res));
            return make(ir::Try{.resources = {resource}, .body = stmt(i.body())}, inst);
        }
        case OpCode::DebugBreak: return make(ir::Intrinsic{.name = "debug.break"}, inst);
        case OpCode::Comp: {
            const auto& i = static_cast<const il::Comp&>(inst);
            ir::Binary k{.op = std::string(comparison_text(i.kind())), .left = expr(i.left()), .right = expr(i.right())};
            k.is_signed = sign_to(i.sign);
            k.left_stk = static_cast<ir::Stack>(i.input_type);
            k.right_stk = static_cast<ir::Stack>(i.input_type);
            k.lift = static_cast<ir::Lift>(i.lifting_kind);
            return make(std::move(k), inst, stack_type(inst.result_type()));
        }
        case OpCode::Call:
        case OpCode::CallVirt:
        case OpCode::NewObj: return call(static_cast<const il::CallInstruction&>(inst));
        case OpCode::CallIndirect: {
            const auto& i = static_cast<const il::CallIndirect&>(inst);
            ir::Call k{.target = expr(i.function_pointer()), .form = ir::CallForm::Indirect};
            for (const il::ILInstruction* a : i.arguments) k.args.push_back(expr(a));
            Node* n = make(std::move(k), inst, stack_type(inst.result_type()));
            ir::FnType fn{.has_this = i.is_instance(), .explicit_this = i.has_explicit_this()};
            ir::TypeP sig = ir::make_type(ir::FnPtrType{.fn = ir::make_type(std::move(fn))},
                                          entity(ir::EntityKind::Type, static_cast<const ts_ns::IType*>(i.function_pointer_type())));
            n->info.set("cil.calli_sig", std::move(sig));
            return n;
        }
        case OpCode::Ckfinite:
            return make(ir::Intrinsic{.name = "cil.ckfinite", .args = {expr(static_cast<const il::Ckfinite&>(inst).argument())}}, inst);
        case OpCode::Conv: {
            const auto& i = static_cast<const il::Conv&>(inst);
            ir::Cast k{.x = expr(i.argument())};
            if (i.check_for_overflow) k.overflow = ir::Overflow::Trap;
            k.conv = static_cast<ir::ConvKind>(i.kind);
            k.from_stk = static_cast<ir::Stack>(i.input_type);
            k.from_signed = sign_to(i.input_sign);
            if (i.is_lifted()) k.lift = ir::Lift::CSharp;
            bool is_r = false;
            Node* n = make(std::move(k), inst, prim_target(i.target_type, &is_r));
            if (is_r) n->info.set("cil.target_r", true);
            return n;
        }
        case OpCode::LdLoc: {
            ir::SymP sym = variable(static_cast<const il::LdLoc&>(inst).variable());
            ir::TypeP t = sym->type;
            return make(ir::Name{.sym = std::move(sym)}, inst, std::move(t));
        }
        case OpCode::LdLoca: return make(ir::Unary{.op = "&", .x = name(static_cast<const il::LdLoca&>(inst).variable())}, inst);
        case OpCode::StLoc: {
            const auto& i = static_cast<const il::StLoc&>(inst);
            Node* target = name(i.variable());
            ir::TypeP t = target->type;
            Node* n = make(ir::Assign{.target = target, .value = expr(i.value())}, inst, std::move(t));
            if (i.is_stack_adjustment) ir::set_flag(*n, ir::NodeFlag::StackAdjust);
            if (i.il_stack_was_empty) ir::set_flag(*n, ir::NodeFlag::StackEmpty);
            return n;
        }
        case OpCode::AddressOf: {
            const auto& i = static_cast<const il::AddressOf&>(inst);
            Node* n = make(ir::Unary{.op = "&", .x = expr(i.value())}, inst, type(i.type()));
            ir::set_flag(*n, ir::NodeFlag::RValueAddr);
            return n;
        }
        case OpCode::ThreeValuedBoolAnd:
        case OpCode::ThreeValuedBoolOr: {
            const auto& i = static_cast<const il::BinaryInstruction&>(inst);
            ir::Binary k{.op = inst.op_code == OpCode::ThreeValuedBoolAnd ? "&&" : "||", .left = expr(i.left()), .right = expr(i.right())};
            k.lift = ir::Lift::ThreeValued;
            return make(std::move(k), inst, stack_type(inst.result_type()));
        }
        case OpCode::NullableUnwrap: {
            const auto& i = static_cast<const il::NullableUnwrap&>(inst);
            return make(ir::OptionalUnwrap{.x = expr(i.argument()), .ref_in = i.ref_input}, inst, stack_type(inst.result_type()));
        }
        case OpCode::NullableRewrap:
            return make(ir::OptionalChain{.x = expr(static_cast<const il::NullableRewrap&>(inst).argument())}, inst, stack_type(inst.result_type()));
        case OpCode::LdStr: return make(ir::Lit{.value = static_cast<const il::LdStr&>(inst).value}, inst, string_type());
        case OpCode::LdStrUtf8: {
            Node* n = make(ir::Lit{.value = static_cast<const il::LdStrUtf8&>(inst).value}, inst, stack_type(il::StackType::O));
            ir::set_flag(*n, ir::NodeFlag::Utf8Literal);
            return n;
        }
        case OpCode::LdcI4: return make(ir::Lit{.value = static_cast<std::int64_t>(static_cast<const il::LdcI4&>(inst).value)}, inst, ir::i32_type());
        case OpCode::LdcI8: return make(ir::Lit{.value = static_cast<const il::LdcI8&>(inst).value}, inst, stack_type(il::StackType::I8));
        case OpCode::LdcF4:
        case OpCode::LdcF8: {
            const bool single = inst.op_code == OpCode::LdcF4;
            const double v = single ? static_cast<double>(static_cast<const il::LdcF4&>(inst).value) : static_cast<const il::LdcF8&>(inst).value;
            ir::Lit k{.value = v};
            // a NaN (its payload) and a negative zero keep their bits as the text: the float's own bits for an F4
            if (std::isnan(v) || (v == 0 && std::signbit(v))) {
                k.text = single ? std::format("0x{:08x}", std::bit_cast<std::uint32_t>(static_cast<const il::LdcF4&>(inst).value))
                                : std::format("0x{:016x}", std::bit_cast<std::uint64_t>(v));
            }
            return make(std::move(k), inst, stack_type(single ? il::StackType::F4 : il::StackType::F8));
        }
        case OpCode::LdcDecimal:
            return make(ir::Lit{.value = decimal_text(static_cast<const il::LdcDecimal&>(inst).value)}, inst, ir::decimal_type());
        case OpCode::LdNull: return make(ir::Lit{}, inst, ir::null_type());
        case OpCode::LdFtn: return make(ir::AddressOf{.sym = method(static_cast<const il::LdFtn&>(inst).method())}, inst, stack_type(il::StackType::I));
        case OpCode::LdVirtFtn: {
            const auto& i = static_cast<const il::LdVirtFtn&>(inst);
            return make(ir::MethodRef{.target = expr(i.argument()), .sym = method(i.method()), .mode = std::string("virtual_ptr")}, inst,
                        stack_type(il::StackType::I));
        }
        case OpCode::LdVirtDelegate: {
            const auto& i = static_cast<const il::LdVirtDelegate&>(inst);
            return make(ir::MethodRef{.target = expr(i.argument()), .sym = method(i.method()), .mode = std::string("virtual_delegate"), .of = type(i.type())},
                        inst, type(i.type()));
        }
        case OpCode::LdTypeToken:
            return make(ir::Intrinsic{.name = "cil.ldtoken_type", .targs = {type(static_cast<const il::LdTypeToken&>(inst).type())}}, inst);
        case OpCode::LdMemberToken:
            return make(ir::Intrinsic{.name = "cil.ldtoken_member", .syms = {member(static_cast<const il::LdMemberToken&>(inst).member())}}, inst);
        case OpCode::LocAlloc: return make(ir::StackAlloc{.count = expr(static_cast<const il::LocAlloc&>(inst).argument())}, inst);
        case OpCode::LocAllocSpan: {
            const auto& i = static_cast<const il::LocAllocSpan&>(inst);
            return make(ir::StackAlloc{.elem = type(i.type()), .count = expr(i.argument()), .span = true}, inst);
        }
        case OpCode::Cpblk: {
            const auto& i = static_cast<const il::Cpblk&>(inst);
            Node* n = make(ir::Intrinsic{.name = "mem.copy", .args = {expr(i.dest_address()), expr(i.source_address()), expr(i.size())}}, inst);
            set_prefixes(*n, i.is_volatile(), i.unaligned_prefix());
            return n;
        }
        case OpCode::Initblk: {
            const auto& i = static_cast<const il::Initblk&>(inst);
            Node* n = make(ir::Intrinsic{.name = "mem.fill", .args = {expr(i.address()), expr(i.value()), expr(i.size())}}, inst);
            set_prefixes(*n, i.is_volatile(), i.unaligned_prefix());
            return n;
        }
        case OpCode::LdFlda: {
            const auto& i = static_cast<const il::LdFlda&>(inst);
            Node* sel = helper(ir::Select{.obj = expr(i.target()), .sym = field(i.field())}, field(i.field())->type);
            Node* n = make(ir::Unary{.op = "&", .x = sel}, inst);
            if (i.delay_exceptions) ir::set_flag(*n, ir::NodeFlag::DelayExc);
            return n;
        }
        case OpCode::LdsFlda: {
            const auto& i = static_cast<const il::LdsFlda&>(inst);
            Node* owner = helper(ir::TypeName{}, type(i.field()->declaring_type()));
            Node* sel = helper(ir::Select{.obj = owner, .sym = field(i.field())}, field(i.field())->type);
            return make(ir::Unary{.op = "&", .x = sel}, inst);
        }
        case OpCode::CastClass:
        case OpCode::IsInst:
        case OpCode::Box:
        case OpCode::Unbox:
        case OpCode::UnboxAny: {
            const auto& u = static_cast<const il::UnaryInstruction&>(inst);
            const ts_ns::IType* operand = nullptr;
            ir::ConvKind conv = ir::ConvKind::UnboxAny;
            switch (inst.op_code) {
                case OpCode::CastClass: conv = ir::ConvKind::RefCast; operand = static_cast<const il::CastClass&>(inst).type(); break;
                case OpCode::IsInst: conv = ir::ConvKind::TryCast; operand = static_cast<const il::IsInst&>(inst).type(); break;
                case OpCode::Box: conv = ir::ConvKind::Box; operand = static_cast<const il::Box&>(inst).type(); break;
                case OpCode::Unbox: conv = ir::ConvKind::Unbox; operand = static_cast<const il::Unbox&>(inst).type(); break;
                default: operand = static_cast<const il::UnboxAny&>(inst).type(); break;
            }
            return make(ir::Cast{.x = expr(u.argument()), .conv = conv}, inst, type(operand));
        }
        case OpCode::LdObj: {
            const auto& i = static_cast<const il::LdObj&>(inst);
            Node* n = make(ir::Unary{.op = "*", .x = expr(i.target())}, inst, type(i.type()));
            set_prefixes(*n, i.is_volatile(), i.unaligned_prefix());
            return n;
        }
        case OpCode::LdObjIfRef: {
            const auto& i = static_cast<const il::LdObjIfRef&>(inst);
            Node* n = make(ir::Unary{.op = "*", .x = expr(i.target())}, inst, type(i.type()));
            ir::set_flag(*n, ir::NodeFlag::IfRef);
            return n;
        }
        case OpCode::StObj: {
            const auto& i = static_cast<const il::StObj&>(inst);
            Node* deref = helper(ir::Unary{.op = "*", .x = expr(i.target())}, type(i.type()));
            set_prefixes(*deref, i.is_volatile(), i.unaligned_prefix());
            return make(ir::Assign{.target = deref, .value = expr(i.value())}, inst, type(i.type()));
        }
        case OpCode::NewArr: {
            const auto& i = static_cast<const il::NewArr&>(inst);
            ir::NewArray k{.elem = type(i.type())};
            for (const il::ILInstruction* d : i.indices()) k.dims.push_back(expr(d));
            return make(std::move(k), inst);
        }
        case OpCode::DefaultValue: {
            const auto& i = static_cast<const il::DefaultValue&>(inst);
            Node* n = make(ir::Default{}, inst, type(i.type()));
            if (i.il_stack_was_empty) ir::set_flag(*n, ir::NodeFlag::StackEmpty);
            return n;
        }
        case OpCode::Throw: {
            Node* x = expr(static_cast<const il::Throw&>(inst).argument());
            return ctx == Ctx::Stmt ? make(ir::Throw{.expr = x}, inst) : make(ir::ThrowExpr{.x = x}, inst);
        }
        case OpCode::Rethrow: return ctx == Ctx::Stmt ? make(ir::Throw{}, inst) : make(ir::ThrowExpr{}, inst);
        case OpCode::SizeOf: return make(ir::SizeOf{.of = type(static_cast<const il::SizeOf&>(inst).type())}, inst, ir::i32_type());
        case OpCode::LdLen: return make(ir::Length{.arr = expr(static_cast<const il::LdLen&>(inst).array())}, inst, stack_type(inst.result_type()));
        case OpCode::LdElema:
        case OpCode::LdElemaInlineArray: {
            const bool inline_array = inst.op_code == OpCode::LdElemaInlineArray;
            const auto* e = inline_array ? nullptr : &static_cast<const il::LdElema&>(inst);
            const auto* ia = inline_array ? &static_cast<const il::LdElemaInlineArray&>(inst) : nullptr;
            std::vector<Node*> idx;
            for (const il::ILInstruction* x : inline_array ? ia->indices() : e->indices()) idx.push_back(expr(x));
            const ts_ns::IType* elem_type = inline_array ? ia->type() : e->type();
            Node* index = helper(ir::Index{.arr = expr(inline_array ? ia->array() : e->array()), .index = std::move(idx), .elem = type(elem_type)},
                                 type(elem_type));
            Node* n = make(ir::Unary{.op = "&", .x = index}, inst);
            if (inline_array) {
                ir::set_flag(*n, ir::NodeFlag::InlineArray);
                if (ia->is_read_only) ir::set_flag(*n, ir::NodeFlag::ReadOnly);
            } else {
                if (e->with_system_index) ir::set_flag(*n, ir::NodeFlag::FromEnd);
                if (e->delay_exceptions) ir::set_flag(*n, ir::NodeFlag::DelayExc);
                if (e->is_read_only) ir::set_flag(*n, ir::NodeFlag::ReadOnly);
            }
            return n;
        }
        case OpCode::GetPinnableReference: {
            const auto& i = static_cast<const il::GetPinnableReference&>(inst);
            Node* n = make(ir::Call{.target = expr(i.argument()), .sym = method(i.method())}, inst);
            ir::set_flag(*n, ir::NodeFlag::PinnableRef);
            return n;
        }
        case OpCode::StringToInt: {
            const auto& i = static_cast<const il::StringToInt&>(inst);
            ir::Intrinsic k{.name = "cil.string_to_int", .args = {expr(i.argument())}, .targs = {type(i.expected_type())}};
            std::optional<std::int64_t> null_key;
            for (std::size_t n = 0; n < i.map().size(); ++n) {
                const auto& [key, value] = i.map()[n];
                if (!key.has_value()) {
                    if (null_key) throw std::logic_error("cil lowering: a string switch with two null keys");
                    null_key = static_cast<std::int64_t>(n);
                }
                k.strs.push_back(key.value_or(""));
                k.imms.push_back(value);
            }
            Node* n = make(std::move(k), inst);
            if (null_key) n->info.set("cil.null_key", *null_key);
            return n;
        }
        case OpCode::ExpressionTreeCast: {
            const auto& i = static_cast<const il::ExpressionTreeCast&>(inst);
            ir::Cast k{.x = expr(i.argument()), .conv = ir::ConvKind::ExprTree};
            if (i.is_checked()) k.overflow = ir::Overflow::Trap;
            return make(std::move(k), inst, type(i.type()));
        }
        case OpCode::UserDefinedLogicOperator: {
            const auto& i = static_cast<const il::UserDefinedLogicOperator&>(inst);
            ir::Binary k{.op = i.method()->name() == "op_BitwiseAnd" ? "&&" : "||", .left = expr(i.left()), .right = expr(i.right())};
            k.sym = method(i.method());
            return make(std::move(k), inst, type(i.method()->return_type()));
        }
        case OpCode::DynamicLogicOperatorInstruction:
        case OpCode::DynamicBinaryOperatorInstruction:
        case OpCode::DynamicUnaryOperatorInstruction:
        case OpCode::DynamicConvertInstruction:
        case OpCode::DynamicGetMemberInstruction:
        case OpCode::DynamicSetMemberInstruction:
        case OpCode::DynamicGetIndexInstruction:
        case OpCode::DynamicSetIndexInstruction:
        case OpCode::DynamicInvokeMemberInstruction:
        case OpCode::DynamicInvokeConstructorInstruction:
        case OpCode::DynamicInvokeInstruction:
        case OpCode::DynamicIsEventInstruction: return dynamic(inst);
        case OpCode::MatchInstruction: {
            const auto& i = static_cast<const il::MatchInstruction&>(inst);
            if (i.is_deconstruct_call != (i.method() != nullptr)) throw std::logic_error("cil lowering: a match whose method is not its deconstruct call");
            ir::PatternMatch k{.var = variable(i.variable()), .tested = expr(i.tested_operand())};
            if (i.check_type) k.test = variable(i.variable())->type;
            k.check_not_null = i.check_not_null;
            k.deconstruct = method(i.method());
            k.is_tuple = i.is_deconstruct_tuple;
            for (const il::ILInstruction* sp : i.sub_patterns()) k.subs.push_back(expr(sp));
            return make(std::move(k), inst, ir::i32_type());
        }
        case OpCode::MakeRefAny: {
            const auto& i = static_cast<const il::MakeRefAny&>(inst);
            return make(ir::Intrinsic{.name = "cil.makeref", .args = {expr(i.argument())}, .targs = {type(i.type())}}, inst);
        }
        case OpCode::RefAnyType:
            return make(ir::Intrinsic{.name = "cil.reftype", .args = {expr(static_cast<const il::RefAnyType&>(inst).argument())}}, inst);
        case OpCode::RefAnyValue: {
            const auto& i = static_cast<const il::RefAnyValue&>(inst);
            return make(ir::Intrinsic{.name = "cil.refvalue", .args = {expr(i.argument())}, .targs = {type(i.type())}}, inst);
        }
        case OpCode::YieldReturn: return make(ir::YieldReturn{.expr = expr(static_cast<const il::YieldReturn&>(inst).value())}, inst);
        case OpCode::Await: {
            const auto& i = static_cast<const il::Await&>(inst);
            return make(ir::Await{.x = expr(i.value()), .awaiter = method(i.get_awaiter_method), .result = method(i.get_result_method)}, inst,
                        i.get_result_method != nullptr ? type(i.get_result_method->return_type()) : ir::TypeP{});
        }
        case OpCode::DeconstructInstruction: {
            const auto& i = static_cast<const il::DeconstructInstruction&>(inst);
            ir::Destructure k;
            for (const il::StLoc* s : i.init) k.init.push_back(expr(s));
            k.pattern = expr(i.pattern());
            k.conversions = expr(i.conversions());
            k.assignments = expr(i.assignments());
            return make(std::move(k), inst);
        }
        case OpCode::DeconstructResultInstruction: {
            const auto& i = static_cast<const il::DeconstructResultInstruction&>(inst);
            return make(ir::Intrinsic{.name = "cil.deconstruct_result", .args = {expr(i.argument())}, .imms = {i.index()}}, inst);
        }
        case OpCode::SwitchSection:
        case OpCode::TryCatchHandler:
        case OpCode::AnyNode: break;
    }
    throw std::logic_error(std::format("cil lowering: an instruction of opcode {} out of place", static_cast<int>(inst.op_code)));
}

// ---- the public face --------------------------------------------------------------------------------------------------------------

Lowering::Lowering(Arena& arena) : s_(std::make_unique<State>(arena)) {}
Lowering::~Lowering() = default;
const LoweringStats& Lowering::stats() const { return s_->stats; }
ir::TypeP Lowering::type(const dotnet::decompiler::typesystem::IType* type) { return s_->type(type); }
ir::SymP Lowering::new_variable(const dotnet::decompiler::il::ILVariable& variable) {
    // (a variable made after the transforms is plain: no capture scope or state machine field, which name the original ILAst's objects)
    if (variable.capture_scope != nullptr || variable.state_machine_field != nullptr)
        throw std::logic_error("cil lowering: a new variable with a capture scope or a state machine field");
    return s_->variable(&variable);
}
ir::SymP Lowering::symbol_of(const dotnet::decompiler::il::ILVariable& variable) const {
    const ir::SymP* sym = s_->variables.find(&variable);
    return sym != nullptr ? *sym : nullptr;
}
void Lowering::restate(ir::Sym& sym, const dotnet::decompiler::il::ILVariable& variable) {
    // what the builders change of a variable: its kind, type and name
    const VarRoleOf role = var_role(variable.kind(), variable.is_this());
    sym.kind = role.role == ir::VarRole::Param || role.role == ir::VarRole::This ? ir::SymKind::Param : ir::SymKind::Local;
    sym.role = role.role;
    if (role.exact) sym.info.set("cil.kind_exact", std::string(*role.exact));
    else sym.info.erase("cil.kind_exact");
    sym.type = s_->type(variable.type());
    sym.name = variable.name.value_or("");
    sym.generated_name = variable.has_generated_name;
}

ir::Node* Lowering::function(const dotnet::decompiler::il::ILFunction& function, LiftMap* map) {
    s_->links = map;
    s_->functions.clear();
    s_->variables.clear();
    s_->function_members.clear();
    s_->function_entities.clear();
    s_->function_facts.clear();
    s_->origins.clear();
    s_->spans.reset();
    s_->assign_labels(function);
    return s_->function(function);
}

}  // namespace cb::translate::frontends::cil
