// cb/translate/frontends/python/lower_internal.hpp -- what the files of the lowering share: the L2 types of the Python model, the
// text readers, and `Lowerer`, which lowers one function (its statements are lower_stmt.cpp, its locals lower_locals.cpp, its
// comprehensions and records lower_comp.cpp, its expressions lower_expr.cpp, its calls lower_call.cpp). Not for use outside
// the lowering: FnLower (lower.hpp) is the way in.
#pragma once

#include <functional>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "cb/translate/frontends/python/infer_util.hpp"
#include "cb/translate/frontends/python/lower.hpp"

namespace cb::translate::frontends::python::lower {

using ir::Node;
using ir::TypeP;
namespace pt = pytypes;

// The owner of the calls that stand for Python's own operations.
inline constexpr std::string_view kPy = "py";

// ---- the L2 types of the Python model ---------------------------------------------------------------------------------------
const TypeP& i64_type();
const TypeP& bool_type();
const TypeP& f64_type();
const TypeP& none_type();    // the type of `None` and of what returns nothing (void)
const TypeP& str_type();     // py.str
const TypeP& bytes_type();   // py.bytes
TypeP py_class(std::string_view name, std::vector<TypeP> args = {});   // "py.list" etc.
TypeP optional_type(const TypeP& inner);                                // py.optional<inner>
bool is_opt(const TypeP& t);
// The class a type names, or null.
const ir::ClassType* class_type_of(const TypeP& t);
bool names_class(const TypeP& t, std::string_view name);
bool is_ref_class(const TypeP& t);
bool is_record_class(const TypeP& t);
// The type's L2 kind ("i64", "f64", "class" ...) as a string; "" for an absent type.
std::string_view kind_of(const TypeP& t);

// ---- the text of the source ---------------------------------------------------------------------------------------------------
// inspect.cleandoc: a docstring with its common indentation and its blank edge lines taken off.
std::string clean_docstring(std::string_view raw);
// The source text a node spans (one or several lines), or nullopt when its position does not say.
std::optional<std::string> source_segment(std::string_view source, const syntax::Pos& pos);
// A value of a default as the `default=...` modifier of a parameter writes it (JSON).
std::string default_text(const syntax::ConstValue& value);

// ---- one function ---------------------------------------------------------------------------------------------------------------

class Lowerer {
public:
    Lowerer(Context& context, const Fn& function, infer::Eval& evaluation);
    Lowerer(Context& context, const Module& module, infer::Eval& evaluation);

    Context& ctx;
    Program& p;
    infer::Eval& ev;
    // What is lowered: a function, or a module's top level (fn is then null).
    const Fn* fn = nullptr;
    const Module* module = nullptr;
    const Class* cls = nullptr;
    std::string qname;
    program::FnKind kind = program::FnKind::Function;
    std::vector<std::string> params, kwonly;
    const std::string& source;

    std::vector<Node*> pre;                                           // statements an expression needs before its own statement
    int tmp = 0;
    std::unordered_map<const syntax::Stmt*, std::vector<std::string>> decl_at;       // the names declared just before a statement
    std::unordered_map<const syntax::Stmt*, std::string> decl_init;                  // an Assign: the name it declares
    std::unordered_map<const syntax::Stmt*, std::set<std::string>> decl_tuple;       // an Assign: the names an unpacking declares
    std::unordered_map<std::string, TypeP> local_types;
    std::unordered_set<std::string> declared;
    std::unordered_set<std::string> loopvars;
    std::set<std::string> calls;                                      // the program's functions it calls
    std::set<std::string> classes_made;
    std::set<std::pair<std::string, std::string>> globals_used;
    TypeP ret;
    const syntax::Expr* in_cond = nullptr;

    // -- making nodes
    template <class K>
    Node* mk(K kind_value, TypeP type = {}, std::uint32_t line = 0) {
        return ir::make(ctx.arena(), std::move(kind_value), std::move(type), line);
    }
    Node* lit(ir::LitValue value, const TypeP& type, std::optional<std::string> text = std::nullopt);
    Node* lit_none(const TypeP& type) { return lit(std::monostate{}, type); }
    // A call of one of Python's own operations.
    Node* py_call(std::string_view name, std::vector<Node*> args, const TypeP& type, Node* target = nullptr, std::uint32_t line = 0,
                  std::vector<ir::TypeArg> typeargs = {});
    ir::SymP local_sym(const std::string& name, const TypeP& type) const;
    Node* local_decl(const std::string& name, const TypeP& type, Node* init = nullptr, std::vector<std::string> mods = {}, std::uint32_t line = 0);
    Node* name_expr(const std::string& name);
    Node* block_of(std::vector<Node*> stmts);
    Node* if_node(Node* cond, Node* then, Node* orelse, std::uint32_t line = 0);
    Node* not_of(Node* cond);                              // !(cond)
    Node* assign_stmt(Node* target, Node* value, const TypeP& type, std::uint32_t line = 0);
    Node* expr_stmt(Node* expression, std::uint32_t line = 0);
    Node* class_name_node(std::string_view class_name);    // a TypeName naming a class: the qualifier of a static
    [[noreturn]] void refuse(RefusalClass cls, std::string detail, std::uint32_t line = 0) const;

    // -- types and positions
    const Type& type_at(const void* node, std::uint32_t line = 0) const;   // Refused when no type reached the node
    TypeP ty(const Type& t, const syntax::Node* node = nullptr, std::string_view what = "value") { return ctx.type_of(t, line_of(node), what); }
    const infer::Res* res_at(const void* node) const;
    static std::uint32_t line_of(const syntax::Node* node) { return node ? static_cast<std::uint32_t>(node->pos.lineno) : 0; }

    // -- the function
    Node* lower();
    Node* value_of(const syntax::Expr* expression, const TypeP& want);

    // -- where each local is declared (lower_locals.cpp)
    void plan_locals();
    static std::vector<std::string> target_names(const syntax::Expr* target);
    static std::vector<pyast::AnyNode> own_nodes(const syntax::Stmt* st);

    // -- statements (lower_stmt.cpp)
    Node* block(const std::vector<syntax::Stmt*>& stmts);
    std::vector<Node*> stmt(const syntax::Stmt* st);
    std::vector<Node*> s_expr(const syntax::ExprStmt* st);
    std::vector<Node*> s_assign(const syntax::Assign* st);
    std::vector<Node*> s_ann_assign(const syntax::AnnAssign* st);
    std::vector<Node*> s_aug_assign(const syntax::AugAssign* st);
    std::vector<Node*> s_return(const syntax::Return* st);
    std::vector<Node*> s_raise(const syntax::Raise* st);
    std::vector<Node*> s_assert(const syntax::Assert* st);
    std::vector<Node*> s_if(const syntax::If* st);
    std::vector<Node*> s_while(const syntax::While* st);
    std::vector<Node*> s_for(const syntax::For* st);
    std::vector<Node*> s_try(const syntax::Try* st);
    std::vector<Node*> s_with(const syntax::With* st);
    Node* expr_of_target(const syntax::Expr* target, Node* lowered);
    std::vector<Node*> store(const syntax::Expr* target, Node* value, const syntax::Stmt* st);
    std::vector<Node*> unpack(const syntax::Expr* target, Node* value, const syntax::Stmt* st);
    Node* target_expr(const syntax::Expr* target);
    TypeP elem_type(const TypeP& t, std::uint32_t line);
    bool loop_local(const syntax::For* st, const std::string& name);
    std::vector<Node*> range_loop(const syntax::For* st);
    bool pure(const syntax::Expr* node, const syntax::Stmt* loop);

    // -- comprehensions and records (lower_comp.cpp)
    std::string new_tmp(const std::string& base);
    Node* empty(const TypeP& t);
    std::vector<Node*> comp_into(const syntax::Name* target, const syntax::Expr* comp);
    std::vector<Node*> comp_loops(const syntax::Expr* comp, const std::string& into, const TypeP& t);
    std::vector<Node*> gen_loops(const std::vector<syntax::comprehension*>& gens, std::size_t k, const std::function<Node*()>& innermost);
    Node* hoist_comp(const syntax::Expr* node, std::string_view what);
    Node* gen_as_list(const syntax::GeneratorExp* gen);
    Node* any_all(const syntax::Call* node, std::string_view which);
    bool is_record_literal(const syntax::Expr* node) const;
    std::vector<Node*> record_builder(const syntax::Assign* st, const syntax::Name* target);
    Node* record_value(const syntax::Expr* node);
    std::vector<std::pair<std::string, Node*>> spread_fields(Node* src, const syntax::Expr* node);
    Node* field_ref(Node* obj, const std::string& key, const syntax::Expr* node, std::uint32_t line, bool store = false, bool raw = false);

    // -- expressions (lower_expr.cpp)
    Node* cond(const syntax::Expr* node);
    Node* truthy(Node* e);
    Node* coerce(Node* e, const TypeP& want);
    Node* expr(const syntax::Expr* node);
    Node* e_constant(const syntax::Constant* n);
    Node* e_name(const syntax::Name* n);
    Node* narrowed(Node* e, const TypeP& flow);
    Node* e_attribute(const syntax::Attribute* n);
    Node* e_subscript(const syntax::Subscript* n);
    Node* e_bin_op(const syntax::BinOp* n);
    Node* binop(const syntax::Node* n, syntax::OpKind op, Node* a, Node* b, const Type& ta, const Type& tb, const Type& tr);
    Node* as_double(Node* e);
    Node* int_op(const syntax::Node* n, std::string_view op, Node* a, Node* b, const Type& ta, const Type& tb, const Type& tr, const TypeP& rt);
    Node* e_unary_op(const syntax::UnaryOp* n);
    Node* paren(Node* x);
    Node* e_bool_op(const syntax::BoolOp* n);
    Node* paren_bool(Node* e, std::string_view op);
    Node* e_compare(const syntax::Compare* n);
    Node* compare(const syntax::Node* n, syntax::CmpOpKind op, Node* a, Node* b, const syntax::Expr* an, const syntax::Expr* bn);
    Node* e_if_exp(const syntax::IfExp* n);
    Node* e_joined_str(const syntax::JoinedStr* n);
    Node* e_tuple(const syntax::Tuple* n);
    Node* e_list(const syntax::List* n);
    Node* e_set(const syntax::Set* n);
    Node* e_dict(const syntax::Dict* n);
    Node* lambda_expr(const syntax::Lambda* lam, const std::vector<TypeP>& ptypes);
    bool effects(Node* e) const;

    // -- calls (lower_call.cpp)
    Node* e_call(const syntax::Call* n);
    TypeP call_ret(const Fn& f, const syntax::Call* n);
    std::vector<Node*> call_args(const syntax::Call* n, const Fn& f, bool bound);
    Node* default_lit(const syntax::Constant* d);
    Node* builtin_call(const syntax::Call* n, const std::string& name, const Type& t);
    Node* method_call(const syntax::Call* n, const std::string& name, const Type& t);
    Node* sort_call(const syntax::Call* n, Node* recv);
    Node* std_call(const syntax::Call* n, const infer::Res& r, const Type& t);
    // The arguments of a call, a generator expression among them as the list of its items.
    std::vector<Node*> lowered_args(const syntax::Call* n);

    // The keyword of a call by name, or null.
    static const syntax::keyword* keyword_named(const syntax::Call* n, std::string_view name);
    TypeP type_or_void(const Type& t, const syntax::Node* node) { return t.kind == pt::Kind::None ? none_type() : ty(t, node); }
};

}  // namespace cb::translate::frontends::python::lower
