// translate/frontends/clang/lower_impl.hpp -- the lowering's working state, shared by lower_index.cpp (the declaration
// index, type names, constants), lower_decl.cpp (the unit, records, functions), lower_stmt.cpp and lower_expr.cpp.
// Not an interface: lower.hpp is.
#pragma once

#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

#include <nlohmann/json.hpp>

#include "core/arena.hpp"
#include "core/error.hpp"
#include "core/ordered_map.hpp"
#include "translate/frontends/clang/ctypes.hpp"
#include "translate/frontends/clang/lower.hpp"

namespace cb::translate::frontends::clang {

// ---- reading the dump ----------------------------------------------------------------------------------------------
//
// A node of the dump is a JSON object; what is absent or null reads as absent. A tree that is not what clang writes -- a
// member missing where one must be, a value of the wrong kind, an index past the children -- is a ParseError; the places
// that lower one declaration or one expression catch it and make an Unsupported of that node.
namespace tree {

[[noreturn]] void bad(const std::string& what);

// The member, or nullptr when it is absent or null.
const Json* get(const Json& node, std::string_view key);
// The member, which must be there.
const Json& require(const Json& node, std::string_view key);
// The member or an empty object: `(n.get('type') or {})`.
const Json& object(const Json& node, std::string_view key);
// The member's text; empty when absent or null; a ParseError when it is some other kind of value.
std::string text(const Json& node, std::string_view key);
std::optional<std::string> maybe_text(const Json& node, std::string_view key);
// The truth of the member: absent, null, false, 0, "" and an empty list or object are false.
bool flag(const Json& node, std::string_view key);
std::string kind(const Json& node);
// The node's children (empty when it has none).
const std::vector<Json>& inner(const Json& node);
// The child at `index` (negative: from the end); a ParseError when there is none.
const Json& child(const Json& node, long index);
// The integer member (a number or the text of one); a ParseError when it is neither.
std::int64_t integer(const Json& node, std::string_view key);
bool ends_with_attr(std::string_view kind);   // `kind.endswith('Attr')`

}  // namespace tree

// Where a node is: clang writes locations as deltas, so the index walk resolves each node's file, line and column.
struct Location {
    bool known = false;                  // the node (or its parent) had a location
    std::int32_t file = -1;              // index into the file table; -1: none
    std::int64_t line = 0;
    std::int64_t col = 0;
    bool has_begin = false;
    std::int64_t begin_line = 0;
    std::int64_t begin_col = 0;
};

struct Pos {
    std::uint32_t line = 0;
    std::uint32_t col = 0;
};

// A function's names in use, its symbols by declaration id.
struct Scope {
    explicit Scope(std::optional<std::string> owner_name) : owner(std::move(owner_name)) {}
    std::string fresh(const std::string& base);
    std::optional<std::string> owner;
    std::unordered_map<std::string, ir::SymP> syms;
    std::set<std::string> used;
    int n = 0;
};

// A statement is one node or a list of them (a declaration of several variables).
using Stmts = std::vector<ir::Node*>;

class Lowering {
public:
    Lowering(Arena& arena, const Json& tree, const Options& options);
    Lowering(const Lowering&) = delete;
    Lowering& operator=(const Lowering&) = delete;

    ir::Node* unit();
    ir::Node* unsupported_node(const std::string& kind, const Json* n, const std::optional<std::string>& raw = std::nullopt);
    // A function with no body but an Unsupported statement: what stands for a declaration that is refused.
    ir::Node* unsupported_decl(const std::string& kind, const Json& n, const std::optional<std::string>& raw);

    // ---- the index (lower_index.cpp)
    void index();
    std::string record_key(const Json& n, const std::vector<std::string>& ns);
    std::int64_t enum_const(const Json* e);
    // The parser's callback: a typedef / record / class / enum name -> an IR type (empty: not known).
    ir::TypeP name(const std::string& name, const std::vector<ir::TypeP>& args, const std::optional<std::string>& tag);
    const Json* find_record(const std::string& key);
    std::optional<std::pair<std::string, std::string>> find_typedef(const std::string& name);
    bool find_enum(const std::string& name);
    ir::TypeP record_type(const Json* n);
    ir::TypeP ty(const Json& n, std::string_view key = "type");
    ir::TypeP layout_of(const ir::TypeP& t);
    ir::TypeP record_layout(const ir::TypeP& t);
    static bool polymorphic(const Json& rec);
    static const std::vector<Json>& bases(const Json& rec);
    Pos pos(const Json& n) const;
    bool is_own(std::int32_t file) const;
    std::optional<std::int64_t> constant(const Json* n);
    const Location& loc(const Json& n) const;

    // ---- the unit and declarations (lower_decl.cpp)
    void decls(const std::vector<Json>& nodes, bool top);
    void decl(const Json& n);
    std::optional<std::string> lib_owner(const Json& node);
    const Json* member_owner(const Json& n);
    ir::Node* find_method(ir::ClassDecl& cls, const Json& n);
    const Json* body(const Json& n);
    ir::Node* record(const Json& n);
    void enum_decl(const Json& n);
    ir::Node* global_var(const Json& n);
    ir::TypeP complete_array(const ir::TypeP& t, const ir::Node* init);
    ir::SymP sym_of_function(const Json& d, const std::optional<std::string>& owner = std::nullopt);
    std::vector<ir::Node*> params(const Json& n, Scope& scope);
    std::string local_name(const Json& d, Scope& scope);
    ir::Node* function(const Json& n);
    ir::Node* method(const Json& n, const Json* owner, const std::string& access = "public");
    std::vector<ir::Node*> ctor_init(const Json& c, const Json* owner);

    // ---- statements (lower_stmt.cpp)
    Stmts stmt(const Json& n);
    // One statement; the list a declaration of several variables gives is a Block (no place of its own).
    ir::Node* as_stmt(Stmts s);
    ir::Node* switch_stmt(const Json& n, const Json& cond_n, const Json& body_n, Pos p);
    static bool case_inside(const Json& body_n);
    ir::Node* range_for(const Json& n, Pos p);
    ir::Node* local(const Json& d);
    std::vector<ir::Node*> commas(ir::Node* e);

    // ---- expressions (lower_expr.cpp)
    ir::Node* zero(const ir::TypeP& t, Pos p);
    ir::Node* expr(const Json& n);
    ir::Node* expr_unguarded(const Json& n);
    ir::Node* implicit_cast(const Json& n, const Json& x_n, const ir::TypeP& t, Pos p);
    ir::Node* default_arg(const Json& n);
    ir::Node* decl_ref(const Json& n, const ir::TypeP& t, Pos p);
    ir::Node* member(const Json& n, const ir::TypeP& t, Pos p);
    ir::Node* unary(const Json& n, const ir::TypeP& t, Pos p);
    std::vector<ir::Node*> bind_refs(std::vector<ir::Node*> args, const std::vector<ir::TypeP>& params);
    ir::Node* call(const Json& n, const ir::TypeP& t, Pos p);
    ir::Node* member_call(const Json& call_node, const Json& callee, std::size_t first_arg, const ir::TypeP& t, Pos p);
    ir::Node* operator_call(const Json& n, const ir::TypeP& t, Pos p);
    ir::Node* construct(const Json& n, const ir::TypeP& t, Pos p);
    const Json* find_ctor(const Json& cls, const std::string& ctor_type);
    ir::Node* new_expr(const Json& n, const ir::TypeP& t, Pos p);
    ir::Node* size_of(const Json& n);
    ir::Node* init_list(const Json& n, const ir::TypeP& t, Pos p);

    // ---- making nodes
    template <class K>
    ir::Node* make(K kind, ir::TypeP type, Pos p) { return ir::make(arena_, std::move(kind), std::move(type), p.line, p.col); }
    template <class K>
    ir::Node* make_stmt(K kind, Pos p) { return ir::make(arena_, std::move(kind), {}, p.line, p.col); }
    ir::Node* unary_node(const char* op, ir::Node* x, const ir::TypeP& type, Pos p);

    ir::SymP sym(ir::SymKind kind, std::string name, std::optional<std::string> owner = std::nullopt, bool is_static = false,
                 ir::TypeP type = {});

    Arena& arena_;
    const Json& tree_;
    std::string main_;
    std::set<std::string> own_;
    bool auto_own_ = false;
    std::vector<std::string> files_;                       // every file the dump names, in order of appearance
    std::unordered_map<std::string, std::int32_t> file_ids_;
    bool cpp_ = false;
    std::string language_;
    std::optional<std::string> std_;
    ctypes::Model model_;
    std::string unit_name_;
    // node -> where it is (the dump itself is not changed)
    std::unordered_map<const Json*, Location> locations_;
    std::unordered_map<std::string, const Json*> decls_;     // id -> decl node
    std::unordered_map<std::string, const Json*> parent_;    // id -> the enclosing decl node (record, function, namespace)
    std::unordered_map<std::string, std::string> qnames_;    // record id -> qualified key
    OrderedMap<std::string, const Json*> records_;           // key -> record node (the complete definition); first-seen order
    // name -> ("record" | "enum", id) | ("type", text); first-seen order (a lookup by a name's tail takes the first)
    OrderedMap<std::string, std::pair<std::string, std::string>> typedefs_;
    std::unordered_map<std::string, std::int64_t> enum_values_;     // EnumConstantDecl id -> int
    OrderedMap<std::string, std::string> scoped_enums_;      // EnumDecl id -> qualified name (C++ enum class)
    std::unordered_map<std::string, std::optional<std::string>> labels_;   // LabelDecl id -> name
    std::unordered_map<std::string, ir::TypeP> layout_cache_;
    // the declaration an out-of-line body finds: the (name, type text) of each method made, by the MethodDecl
    std::map<const ir::MethodDecl*, std::pair<std::optional<std::string>, std::string>> sigs_;
    ctypes::Parser types_;
    ctypes::Layouts sizes_;
    std::vector<ir::Node*> out_;
    std::unordered_map<std::string, ir::Node*> classes_;     // record key -> ClassDecl
    std::shared_ptr<Scope> scope_;                           // set by function() and method(); the last one stays
    std::unordered_map<std::string, std::pair<std::size_t, const Json*>> tail_memo_;
    std::optional<std::set<std::string>> enum_names_;
    int name_depth_ = 0;
};

// A path made absolute and case-normalised, as Windows compares paths.
std::string normal_path(std::string_view path);
// The directory part of such a path.
std::string directory_of(const std::string& path);

}  // namespace cb::translate::frontends::clang
