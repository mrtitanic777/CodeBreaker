#include "build/l2cc/l2cc.hpp"

#include <cstdint>
#include <limits>
#include <optional>
#include <string_view>
#include <variant>

#include "core/fs.hpp"

namespace cb::build::l2cc {

namespace ir = translate::ir;
using x86cc::BinaryOp;
using x86cc::BodyBuilder;
using x86cc::IntType;
using x86cc::Target;

namespace lower {   // (named: the unity batches put other files' helpers of the same names beside these)

const ir::MethodDecl* method_of(const ir::Node* function) { return ir::get<ir::MethodDecl>(function); }

std::string_view name_of(const ir::MethodDecl& m) {
    if (m.name) return *m.name;
    return m.sym ? std::string_view(m.sym->name) : std::string_view();
}

const ir::PrimType* prim_of(const ir::TypeP& type) {
    return type ? std::get_if<ir::PrimType>(&type->body) : nullptr;
}

// The C spelling of a scalar type on the target: its width decides (16-bit int and 32-bit long on wcc, 32-bit int on wcc386).
std::optional<std::string_view> c_type(const ir::TypeP& type, Target target) {
    if (type && std::holds_alternative<ir::VoidType>(type->body)) return "void";
    const ir::PrimType* p = prim_of(type);
    if (p == nullptr) return std::nullopt;
    const bool wide_int = target == Target::i386;
    switch (p->prim) {
        case ir::Prim::I8: return "signed char";
        case ir::Prim::U8: return "unsigned char";
        case ir::Prim::I16: return wide_int ? "short" : "int";
        case ir::Prim::U16: return wide_int ? "unsigned short" : "unsigned int";
        case ir::Prim::I32: return wide_int ? "int" : "long";
        case ir::Prim::U32: return wide_int ? "unsigned int" : "unsigned long";
        case ir::Prim::I64: return "long long";
        case ir::Prim::U64: return "unsigned long long";
        default: return std::nullopt;
    }
}

// The type C gives a literal of this L2 type written as plain digits with a suffix, and the largest value it holds.
std::optional<std::pair<IntType, std::uint64_t>> literal_type(const ir::TypeP& type, Target target) {
    const ir::PrimType* p = prim_of(type);
    if (p == nullptr) return std::nullopt;
    const bool wide_int = target == Target::i386;
    switch (p->prim) {
        case ir::Prim::I16: if (!wide_int) return std::pair{IntType::Int, std::uint64_t{0x7FFF}}; break;
        case ir::Prim::U16: if (!wide_int) return std::pair{IntType::UInt, std::uint64_t{0xFFFF}}; break;
        case ir::Prim::I32: return std::pair{wide_int ? IntType::Int : IntType::Long, std::uint64_t{0x7FFFFFFF}};
        case ir::Prim::U32: return std::pair{wide_int ? IntType::UInt : IntType::ULong, std::uint64_t{0xFFFFFFFF}};
        case ir::Prim::I64: return std::pair{IntType::LongLong, std::uint64_t{0x7FFFFFFFFFFFFFFF}};
        case ir::Prim::U64: return std::pair{IntType::ULongLong, std::numeric_limits<std::uint64_t>::max()};
        default: break;
    }
    return std::nullopt;
}

std::optional<std::uint64_t> literal_value(const ir::Lit& lit) {
    if (const auto* i = std::get_if<std::int64_t>(&lit.value)) {
        if (*i < 0) return std::nullopt;   // (a negative constant is C's unary minus on a literal: not lowered yet)
        return static_cast<std::uint64_t>(*i);
    }
    if (const auto* u = std::get_if<std::uint64_t>(&lit.value)) return *u;
    return std::nullopt;
}

std::optional<BinaryOp> binary_op(std::string_view op) {
    static constexpr std::pair<std::string_view, BinaryOp> ops[] = {
        {"+", BinaryOp::Add}, {"-", BinaryOp::Sub}, {"*", BinaryOp::Mul}, {"/", BinaryOp::Div}, {"%", BinaryOp::Rem},
        {"<<", BinaryOp::Shl}, {">>", BinaryOp::Shr}, {"&", BinaryOp::And}, {"|", BinaryOp::Or}, {"^", BinaryOp::Xor},
        {"==", BinaryOp::Eq}, {"!=", BinaryOp::Ne}, {"<", BinaryOp::Lt}, {"<=", BinaryOp::Le}, {">", BinaryOp::Gt},
        {">=", BinaryOp::Ge},
    };
    for (const auto& [text, value] : ops)
        if (text == op) return value;
    return std::nullopt;
}

// The statements of a function's body, in order: the list, or the statements of its one Block.
std::vector<const ir::Node*> body_statements(const ir::Suite& suite) {
    const std::vector<ir::Node*> top = ir::statements(suite);
    if (top.size() == 1) {
        if (const auto* b = ir::get<ir::Block>(top.front())) return {b->stmts.begin(), b->stmts.end()};
    }
    return {top.begin(), top.end()};
}

// What the lowering cannot take in an expression (empty: all of it).
std::string declines_expr(const ir::Node* n, Target target) {
    if (n == nullptr) return "an absent expression";
    if (const auto* name = ir::get<ir::Name>(n)) return name->sym ? "" : "a name with no symbol";
    if (const auto* lit = ir::get<ir::Lit>(n)) {
        const auto type = literal_type(n->type, target);
        const auto value = literal_value(*lit);
        if (!type) return "a literal of type " + (n->type ? std::string(n->type->kind()) : std::string("none"));
        if (!value || *value > type->second) return "a literal C writes as more than a constant";
        return "";
    }
    if (const auto* b = ir::get<ir::Binary>(n)) {
        if (!binary_op(b->op)) return "the operator " + b->op;
        if (std::string why = declines_expr(b->left, target); !why.empty()) return why;
        return declines_expr(b->right, target);
    }
    return std::string("an expression ") + std::string(ir::kind_name(*n));
}

std::string declines_stmt(const ir::Node* n, Target target) {
    if (const auto* r = ir::get<ir::Return>(n)) return r->expr ? declines_expr(r->expr, target) : "";
    if (const auto* e = ir::get<ir::ExprStmt>(n)) return declines_expr(e->expr, target);
    return std::string("a statement ") + (n ? std::string(ir::kind_name(*n)) : std::string("null"));
}

// One function's body, made by the builder from its tree.
class Lowering {
public:
    Lowering(BodyBuilder& b, Target target) : b_(b), target_(target) {}

    void body(const ir::MethodDecl& m) {
        const auto stmts = body_statements(m.body);
        for (std::size_t i = 0; i < stmts.size(); ++i) stmt(stmts[i], i + 1 == stmts.size());
    }

private:
    void stmt(const ir::Node* n, bool last) {
        if (const auto* r = ir::get<ir::Return>(n)) {
            if (r->expr != nullptr) {
                b_.return_value(expr(r->expr), last);
            } else {
                b_.return_void(last);
            }
        } else if (const auto* e = ir::get<ir::ExprStmt>(n)) {
            b_.expression(expr(e->expr));
        }
    }

    BodyBuilder::Value expr(const ir::Node* n) {
        if (const auto* name = ir::get<ir::Name>(n)) return b_.name(name->sym->name);
        if (const auto* lit = ir::get<ir::Lit>(n)) {
            return b_.integer(*literal_value(*lit), literal_type(n->type, target_)->first);
        }
        const auto* bin = ir::get<ir::Binary>(n);
        // (operands in the order the parser reduces them: the left one first)
        const BodyBuilder::Value left = expr(bin->left);
        const BodyBuilder::Value right = expr(bin->right);
        return b_.binary(*binary_op(bin->op), left, right);
    }

    BodyBuilder& b_;
    Target target_;
};

// The unit's functions, given to the front end by name.
class Bodies final : public x86cc::BodySource {
public:
    Bodies(const Unit& unit, Target target) : target_(target) {
        for (const ir::Node* f : unit.functions) by_name_.emplace(std::string(name_of(*method_of(f))), method_of(f));
    }

    bool gives(std::string_view function) const override { return by_name_.find(std::string(function)) != by_name_.end(); }

    void build(std::string_view function, BodyBuilder& builder) override {
        const ir::MethodDecl& m = *by_name_.at(std::string(function));
        Lowering(builder, target_).body(m);
        if (builder.failed() && failed_.empty()) failed_ = std::string(function) + ": " + builder.why();
    }

    const std::string& failed() const { return failed_; }

private:
    Target target_;
    std::map<std::string, const ir::MethodDecl*, std::less<>> by_name_;
    std::string failed_;
};

}  // namespace lower

using lower::method_of;
using lower::name_of;

std::string declines(const ir::Node* function, Target target) {
    const ir::MethodDecl* m = method_of(function);
    if (m == nullptr) return "not a function";
    if (head(function, target).empty()) return "a type in its signature";
    if (!m->locals.empty()) return "locals";
    for (const ir::Node* s : lower::body_statements(m->body)) {
        if (std::string why = lower::declines_stmt(s, target); !why.empty()) return why;
    }
    return "";
}

std::string head(const ir::Node* function, Target target) {
    const ir::MethodDecl* m = method_of(function);
    if (m == nullptr) return "";
    const auto ret = lower::c_type(m->ret, target);
    if (!ret) return "";
    std::string out = std::string(*ret) + " " + std::string(name_of(*m)) + "(";
    if (m->params.empty()) out += "void";
    for (std::size_t i = 0; i < m->params.size(); ++i) {
        const auto* p = ir::get<ir::Param>(m->params[i]);
        if (p == nullptr || !p->sym) return "";
        const auto type = lower::c_type(p->type ? p->type : p->sym->type, target);
        if (!type || *type == "void") return "";
        if (i > 0) out += ", ";
        out += std::string(*type) + " " + p->sym->name;
    }
    return out + ")";
}

Result compile(Target target, const Unit& unit, const std::string& file, const std::vector<std::string>& args,
               const std::filesystem::path& folder, const std::map<std::string, std::string>& environment) {
    Result result;
    for (const ir::Node* f : unit.functions) {
        if (std::string why = declines(f, target); !why.empty()) {
            const ir::MethodDecl* m = method_of(f);
            result.declined.push_back((m ? std::string(name_of(*m)) : std::string("?")) + ": " + why);
        }
    }
    if (!result.declined.empty()) return result;
    result.text = unit.header;
    for (const ir::Node* f : unit.functions) result.text += head(f, target) + " {}\n";
    fs::write_text(folder / file, result.text);
    lower::Bodies bodies(unit, target);
    std::vector<std::string> all = args;
    all.push_back(file);
    result.run = x86cc::compile(target, all, folder, environment, &bodies);
    result.failed = bodies.failed();
    return result;
}

}  // namespace cb::build::l2cc
