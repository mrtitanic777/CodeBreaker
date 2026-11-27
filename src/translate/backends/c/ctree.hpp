// translate/backends/c/ctree.hpp -- the C back end on the typed tree: an L2 function, held as translate::ir nodes, written as C in
// the decompiler's own dialect, laid out as PrintC lays it out, with c_text's passes done on the tree (docs/ENGINE_UNIFICATION.md
// item 1). A port line by line of the former L2 core library's C back end (translate/l2/src/cback*, cpasses*, cmodule; deleted 2026-11-27), itself a port of
// the Python reference: the same names, the same order of work, the same text. What moved is the tree it reads: the nodes are
// translate::ir's, read by the names their documents give their slots (l2::slot), the types and symbols ir's structs; what the
// module knows of a function (the facts) stays Python-shaped values (py::V), as the module's planners hand them over.
//
//     X doc = X::node(l2::from_json(arena, json));          // a document's tree, viewed as the passes read it
//     std::string c = ctree::function(doc, moved, far, facts);
//
// X is a node, or a part of one, as the document names it: `e.get("left")` is the node in the slot "left", `e.get("type")` the
// expression's type, `t.get("kind")` a type's kind ("ptr", "i32" ...), `s.get("name")` a symbol's name. A key is present where the
// document writer (l2::to_json) writes it, so `has`, `get` (None where absent) and `at` (KeyError) answer as they did on the document.
// Identity (`id`) is the node's address: the passes' state (marks, rewrites, what a layout saw ...) is keyed on ir::Node*.
#pragma once

#include <cstdint>
#include <deque>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "core/arena.hpp"
#include "translate/ir.hpp"

#if defined(_MSC_VER)
#pragma warning(push, 3)   // (Python's value semantics, translate/support: compiled under their own flags)
#endif
#include "translate/support/pyre.hpp"
#include "translate/support/pystr.hpp"
#include "translate/support/pyvalue.hpp"
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

namespace cb::translate::ctree {

using py::V;
using Id = const void*;
using Strs = std::vector<std::string>;
using Opt = std::optional<std::string>;

struct Decline : std::runtime_error {
    explicit Decline(const std::string& m) : std::runtime_error(m) {}
};

// ---------------------------------------------------------------------------------------------- a part of the tree, as its document names it
class X {
public:
    enum class T : std::uint8_t { None, Bool, Int, UInt, Float, Str, Node, Nodes, Type, Types, Sym, Params, Param, Result, Fields, Field, Texts, Origin, Seq };

    X() : i_(0) {}
    static X node(ir::Node* n) {
        X x;
        if (n != nullptr) {
            x.t_ = T::Node;
            x.n_ = n;
        }
        return x;
    }
    static X nodes(const std::vector<ir::Node*>* v) {
        X x;
        x.t_ = T::Nodes;
        x.nodes_ = v;
        return x;
    }
    static X type(const ir::Type* t) {
        X x;
        if (t != nullptr) {
            x.t_ = T::Type;
            x.ty_ = t;
        }
        return x;
    }
    static X type(const ir::TypeP& t) { return type(t.get()); }
    static X sym(const ir::Sym* s) {
        X x;
        if (s != nullptr) {
            x.t_ = T::Sym;
            x.s_ = s;
        }
        return x;
    }
    static X str(std::string_view s) {
        X x;
        x.t_ = T::Str;
        x.sv_ = s;
        return x;
    }
    static X boolean(bool b) {
        X x;
        x.t_ = T::Bool;
        x.b_ = b;
        return x;
    }
    static X integer(std::int64_t v) {
        X x;
        x.t_ = T::Int;
        x.i_ = v;
        return x;
    }
    static X unsigned_integer(std::uint64_t v) {
        if (v <= static_cast<std::uint64_t>(INT64_MAX)) return integer(static_cast<std::int64_t>(v));
        X x;
        x.t_ = T::UInt;
        x.u_ = v;
        return x;
    }
    static X real(double v) {
        X x;
        x.t_ = T::Float;
        x.f_ = v;
        return x;
    }
    // A list made here (the new nodes a writer makes: a switch's labels as numbers ...): kept by the function being written.
    static X seq(const std::vector<X>* items) {
        X x;
        x.t_ = T::Seq;
        x.seq_ = items;
        return x;
    }

    T kind() const { return t_; }
    bool is_none() const { return t_ == T::None; }
    bool is_bool() const { return t_ == T::Bool; }
    bool is_int() const { return t_ == T::Int || t_ == T::UInt; }
    bool is_intlike() const { return t_ == T::Int || t_ == T::UInt || t_ == T::Bool; }
    bool is_float() const { return t_ == T::Float; }
    bool is_str() const { return t_ == T::Str; }
    bool is_u64() const { return t_ == T::UInt; }
    bool is_dict() const { return t_ == T::Node || t_ == T::Type || t_ == T::Sym || t_ == T::Param || t_ == T::Result || t_ == T::Field || t_ == T::Origin; }
    bool is_list() const { return t_ == T::Nodes || t_ == T::Types || t_ == T::Params || t_ == T::Fields || t_ == T::Texts || t_ == T::Seq; }
    bool is_node() const { return t_ == T::Node; }
    bool b() const { return b_; }

    ir::Node* as_node() const { return t_ == T::Node ? n_ : nullptr; }
    const ir::Type* as_type() const { return t_ == T::Type ? ty_ : nullptr; }
    const ir::Sym* as_sym() const { return t_ == T::Sym ? s_ : nullptr; }
    std::string_view view() const { return sv_; }
    std::string str() const;                     // a text's (TypeError otherwise)
    std::int64_t as_int() const;                 // an int's or a bool's
    double as_float() const { return f_; }
    std::size_t size() const;                    // len()
    std::vector<X> seq() const;                  // a list's items
    X operator[](std::size_t n) const;
    Id id() const;                               // id(): the node, type, symbol or list it is

    // as the document's dict: d.get(key), d[key], key in d
    X get(std::string_view key) const;
    X at(std::string_view key) const;
    bool has(std::string_view key) const;

    bool operator==(std::string_view s) const { return t_ == T::Str && sv_ == s; }
    bool operator==(const char* s) const { return t_ == T::Str && sv_ == s; }
    bool operator!=(const char* s) const { return !(*this == s); }

private:
    X found(std::string_view key, bool& present) const;   // the value under the key; present: whether the document writes it

    T t_ = T::None;
    union {
        bool b_;
        std::int64_t i_;
        std::uint64_t u_;
        double f_;
        ir::Node* n_;
        const std::vector<ir::Node*>* nodes_;
        const ir::Type* ty_;
        const std::vector<ir::TypeP>* tys_;
        const ir::Sym* s_;
        const std::vector<ir::FnParam>* params_;
        const ir::FnParam* param_;
        const ir::FnResult* res_;
        const std::vector<ir::RecordField>* fields_;
        const ir::RecordField* field_;
        const std::vector<std::string>* texts_;
        const ir::Origin* origin_;
        const std::vector<X>* seq_;
    };
    std::string_view sv_;
};

inline Id id(const X& v) { return v.id(); }
inline Id id(const V& v) { return v.id(); }
V to_v(const X& x);                              // a scalar of the tree as a value (None, a bool, a number, a text)

}  // namespace cb::translate::ctree

// Python's operations on a part of the tree, beside the values' own (translate/support/pyvalue.hpp): the port reads as the code it was.
namespace py {
bool truthy(const cb::translate::ctree::X& v);
std::string str_of(const cb::translate::ctree::X& v);
std::string repr(const cb::translate::ctree::X& v);
bool eq(const cb::translate::ctree::X& a, const V& b);
}  // namespace py

namespace cb::translate::ctree {

// ---------------------------------------------------------------------------------------------- the emitters
struct Em {
    virtual ~Em() {}
    virtual bool flat() const = 0;
    virtual void print(const std::string& text, const Opt& shown = std::nullopt) = 0;
    virtual void open_paren(const std::string& p) = 0;
    virtual void close_paren(const std::string& p) = 0;
    virtual void spaces(int n, int bump = 0) = 0;
    virtual void open_group() = 0;
    virtual void close_group() = 0;
    virtual void tag_line(std::optional<int> indent = std::nullopt) = 0;
    virtual void start_indent() = 0;
    virtual void stop_indent() = 0;
    virtual void start_comment() = 0;
    virtual void stop_comment() = 0;
    virtual std::string text() = 0;
};

struct Flat : Em {
    Strs out, raw;
    bool flat() const override { return true; }
    void print(const std::string& text, const Opt& shown = std::nullopt) override;
    void open_paren(const std::string& p) override;
    void close_paren(const std::string& p) override;
    void spaces(int n, int bump = 0) override;
    void open_group() override {}
    void close_group() override {}
    void tag_line(std::optional<int> indent = std::nullopt) override;
    void start_indent() override {}
    void stop_indent() override {}
    void start_comment() override {}
    void stop_comment() override {}
    std::string text() override;
};

struct Pretty : Em {
    enum { BEGIN, END, STRING, BREAK, INDENT, UNINDENT, COMMENT, UNCOMMENT };
    struct Tok {
        int cls;
        long size;
        std::string text;
        long spaces;
        long bump;
        bool absolute;
    };
    int width = 100, indent = 2;
    std::string fill = "   ";
    std::vector<Tok> toks;
    size_t lo = 0;
    std::deque<size_t> scanq;
    std::vector<long> stack;
    long room = 100, left = 1, right = 1;
    bool needbreak = false, commentmode = false;
    Strs outv;
    bool flat() const override { return false; }
    void push(int cls, long size = 0, const std::string& text = "", long sp = 0, long bump = 0, bool absolute = false);
    void scan();
    void advance();
    void line();
    void overflow();
    void printtok(const Tok& t);
    void start_();
    void string_();
    void end_();
    void break_();
    void print(const std::string& text, const Opt& shown = std::nullopt) override;
    void open_paren(const std::string& p) override;
    void close_paren(const std::string& p) override;
    void spaces(int n, int bump = 0) override;
    void open_group() override;
    void close_group() override;
    void tag_line(std::optional<int> indent = std::nullopt) override;
    void start_indent() override;
    void stop_indent() override;
    void start_comment() override;
    void stop_comment() override;
    std::string text() override;
};

// ---------------------------------------------------------------------------------------------- the core's operators
struct Op {
    std::string print1, print2;
    int stage, prec;
    bool assoc;
    std::string kind;
    int spacing, bump;
};

struct Stack {
    Em* em;
    struct Entry {
        const Op* tok;
        int placed;
        bool paren;
    };
    std::vector<Entry> ops;
    explicit Stack(Em* e) : em(e) {}
    void op(const Op* tok, std::optional<bool> paren = std::nullopt);
    void atom(const std::string& text, const Opt& shown = std::nullopt, bool blank = false, const X* node = nullptr);
    void whole(const Op* tok, const std::string& raw, const Opt& shown, std::optional<bool> paren, const X* node);
    bool parens(const Op* op2);
    void emit(const Entry& e);
};

extern const Op HIDDEN, SUBSCRIPT, CALL, CAST, TYPE_SPACE, TYPE_NOSPACE, PTR, ARRAY, WHOLE;
const Op* member_op(bool arrow);
const Op* unary_op(const std::string& op);        // nullptr where none
const Op* postfix_op(const std::string& op);
const Op* binop(const std::string& op);
const Op* assign_op(const X& op);
const std::map<std::string, int>& binary_prec();
bool associative(const std::string& op);
size_t cp_len(const std::string& s);              // len() of a str: its code points
std::string pyx(int64_t v);                        // '%x' % v

// ---------------------------------------------------------------------------------------------- the function being written
template <typename T>
struct Ordered {                                   // a dict by id, in insertion order (Python's)
    std::vector<std::pair<Id, T>> items;
    std::unordered_map<Id, size_t> at;
    T* get(Id k) {
        auto it = at.find(k);
        return it == at.end() ? nullptr : &items[it->second].second;
    }
    T& operator[](Id k) {
        auto it = at.find(k);
        if (it != at.end()) return items[it->second].second;
        at[k] = items.size();
        items.push_back({k, T()});
        return items.back().second;
    }
    void clear() {
        items.clear();
        at.clear();
    }
};

using Rewrite = std::function<std::string(const X&)>;
using Made = std::function<std::string()>;
using IdSet = std::unordered_set<Id>;

struct Now {
    std::set<std::string> moved;
    std::set<std::string> far;
    std::unordered_map<Id, std::string> marks;
    std::unordered_map<Id, Rewrite> whole;
    std::unordered_set<Id> bare;
    bool sixteen = false;
    V facts = V::dict();
    X doc;
    std::vector<X> locals;
    std::set<std::string> drop;
    std::optional<std::vector<std::pair<std::string, std::string>>> old_style;
    IdSet room, gone;
    std::map<std::string, std::string> far_pointers;
    std::unordered_map<Id, X> gap_base;
    std::unordered_map<std::string, std::string> decls;
    IdSet seen;
    bool seeing = false;
    int raw = 0;
    IdSet lines;
    std::unordered_map<Id, std::pair<Flat*, size_t>> at;
    Flat* top = nullptr;
    std::unordered_map<Id, std::pair<std::string, std::string>> virtual_;
    std::unordered_map<std::string, Made> ren;
    Opt locals_text;
    std::vector<Id> owners;
    std::unordered_map<const Em*, Id> owner_of;
    Ordered<std::string> shown;
    IdSet ss_casts, far_read;
    Ordered<Strs> before, after;
    std::unordered_map<Id, size_t> stmt_at;
    bool pops = false;
    std::vector<std::pair<std::string, std::string>> declare_first;
    struct SegPtr {
        Id at;
        std::string name;
        X sel;
    };
    std::vector<SegPtr> seg_ptrs;
    std::set<std::string> frame;
    std::map<std::string, std::string> alias;
    V lane;
    std::unordered_map<std::string, std::string> type_texts;   // (type_text's: a type's text is its own -- kept per call)
    uint64_t mut = 0;                             // (rewrites and renames made: part of the layout's key)
    uint64_t seen_key = 0;                        // (the state the last layout was made in: seen_now reuses it)
    bool seen_valid = false;
    std::vector<std::shared_ptr<Flat>> keep;      // every one-line emitter made while laying out (ids stay unique)
    Arena arena;                                  // the nodes the writer makes (a switch on a pointer as numbers ...)
    std::vector<ir::TypeP> types;                 // ... their types
    std::vector<ir::SymP> syms;                   // ... and their symbols
    bool has(const char* pass) const { return moved.count(pass) != 0; }
};

extern thread_local Now* now;                      // the function being written (one per thread)

// A fresh state for one call, on this thread.
struct Scope {
    Now state;
    Now* was;
    Scope() : was(now) { now = &state; }
    ~Scope() { now = was; }
};

// ---------------------------------------------------------------------------------------------- helpers (Python's dict access)
inline X g(const X& d, const char* k) { return d.get(k); }
inline X at(const X& d, const char* k) { return d.at(k); }
inline bool is(const X& e, const char* node) { return e.get("node") == node; }
inline bool tr(const X& v) { return py::truthy(v); }
std::string sym_name(const X& e);                  // (e.get('sym') or {}).get('name') as a string ('' for None)
V sym_name_v(const X& e);                          // ... as the value
std::string name_of(const X& e);                   // _name(e)
std::string param_name(const X& p);                // _param_name(p)
std::vector<X> stmts(const X& s);                  // _stmts(s)
bool same(const X& a, const X& b);                 // a is b
bool in(const std::set<std::string>& s, const std::string& x);
bool in(const IdSet& s, Id x);
std::string fmt(const char* f, std::initializer_list<std::string> args);   // '%s' formatting of strings

// (the passes' shared ones)
inline bool in_whole(const X& e) { return now->whole.count(id(e)) != 0; }
inline std::string mark(const X& e) {
    auto it = now->marks.find(id(e));
    return it == now->marks.end() ? std::string() : it->second;
}
bool field_sym(const X& e);
X dict_or_empty(const X& v);                       // (v or {}): an absent part as an empty one (its get answers None)
std::string node_kind(const X& e);
bool is_ident(const std::string& s);

// ---------------------------------------------------------------------------------------------- the writer (writer.cpp)
struct Tok3 {
    int prec;
    std::string op;
    std::string kind;
    bool has_op;
};
Tok3 token(const X& e);
bool bracket(const Tok3& parent, const X& child, int stage);
bool arrow(const X& e);
std::string expr(const X& e);
std::string sub(const Tok3& parent, const X& child, int stage);
std::string lit(const X& e);
std::string int_text(const V& v);
std::string type_text(const ir::Type* t, const std::string& name = "", bool far = false);
inline std::string type_text(const X& t, const std::string& name = "", bool far = false) { return type_text(t.as_type(), name, far); }
struct TypeStack {
    std::vector<std::string> mods;      // "ptr", or the array count ("" for none) after "["
    std::vector<bool> is_ptr;
    std::string base;
};
TypeStack type_stack(const ir::Type* t);
inline TypeStack type_stack(const X& t) { return type_stack(t.as_type()); }
bool is_far(const ir::Type* t);
inline bool is_far(const X& t) { return is_far(t.as_type()); }
void push_type(Stack& st, const ir::Type* t, const Opt& name, bool far = false, const Opt& word = std::nullopt,
               const Opt& shown_name = std::nullopt, const Opt& shown_base = std::nullopt, bool name_none = false);
void put_type(Em* em, const ir::Type* t, const Opt& name, bool far = false, const Opt& shown_name = std::nullopt,
              const Opt& shown_base = std::nullopt);
inline void put_type(Em* em, const X& t, const Opt& name, bool far = false, const Opt& shown_name = std::nullopt,
                     const Opt& shown_base = std::nullopt) {
    put_type(em, t.as_type(), name, far, shown_name, shown_base);
}
Flat* make_flat();                                 // (kept until the function is written)
std::string flat_text(const std::function<void(Em*)>& put);
void put(Em* em, const X& e);
void push(Stack& st, const X& e, std::optional<bool> paren = std::nullopt);
std::string raw_text(const X& e);                  // _raw
void rewrite(const X& e, Rewrite shown, bool room = false);
const Op* op_of(const X& e);
void cond(Em* em, const X& e, bool comma);
std::string condition(const X& e, bool inner = false);
void block(Em* em, const X& stmts);
void stmt(Em* em, const X& s);
void comment(Em* em, const std::string& text, std::optional<int> indent);
std::vector<X> locals_of(const X& body);
const ir::Type* ret_of(const X& doc);
const ir::FnType* fn_type_of(const X& doc);        // the function's type (its symbol's), or none
std::string signature(const X& doc);
void signature_em(Em* em, const X& doc);
std::string function_text(const X& doc, const std::vector<X>& locs, Em* em = nullptr);
std::string mk_fp_text(const std::string& seg, const X& off, const std::string& comma = ", ", const Opt& shown = std::nullopt);
std::string pops(const std::string& ty);
Opt segment_register(const std::string& name, const X& t);
std::string cast_type(const X& cast);
std::string local_text(const std::string& name);
std::string decl_now(const X& v);
void near_call(const X& e);
X leading_cast(const X& e);
bool data_address(const X& e);
Opt assigned_name(const X& target);
std::string result_shown_base(const ir::Type* ret, bool& has);
struct Heading {
    std::string result;
    Strs mods, words;
    bool far_model;
};
Heading heading(const X& doc);
bool lost(const X& doc);
Strs mods_of(const X& doc);
extern const char* ENTRY_SP;

// ---------------------------------------------------------------------------------------------- the passes (passes.cpp, passes2.cpp)
struct NodeP {
    X e;
    X p;     // None: no parent
};
using Nodes = std::vector<NodeP>;
Nodes walk(const X& x);
using ParentMap = std::unordered_map<Id, X>;
ParentMap parents(const Nodes& nodes);
const X& parent_of(const ParentMap& m, const X& e);
std::vector<X> live_locals();
void run_passes(Nodes& nodes, const std::set<std::string>& arrays);
void far_from_stack(const X& body, const std::vector<X>& locs);
std::set<std::string> frame_arrays(const std::vector<X>& locs);
std::optional<std::vector<std::pair<std::string, std::string>>> old_style(const X& doc);
std::map<std::string, std::string> far_pointers_of(const std::vector<X>& locs);
IdSet seen_now();                                  // _seen() (a copy: a later call makes a new set)
extern const std::map<std::string, int> SIZEOF;
Strs seg_layout_text(const std::vector<std::pair<int64_t, std::string>>& fields, bool& ok);

// (passes.cpp's, for passes2.cpp)
bool x_unary_as_read(const X& u, const char* op, const ParentMap& parent);
bool x_star_as_read(const X& u, const ParentMap& parent);
bool x_arrow_of(const X& u, const ParentMap& parent);
bool x_unary_parent(const X& e, const ParentMap& parent, const char* op);
bool x_bare_name(const X& e, const Opt& name);
std::string x_written(const X& e, const X* under);
bool x_near_number(const std::string& inner);
std::string x_shape(const X& e);

struct Access {
    std::string param, base, stars;
    int64_t off;
};
std::optional<Access> struct_access(const X& u, const V& params);

// the 16-bit heading's facts (passes2.cpp), asked of text
V arguments_at(const std::string& text, int64_t open_paren);
V words_of(const std::string& ty);
V parameters(const std::string& text);
V argument_words(const std::string& arg, const V& types);

// ---------------------------------------------------------------------------------------------- the entry points (module.cpp)
std::string function(const X& doc, const V& moved, const V& far, const V& facts);
V flags(const X& doc, const V& facts);
V module_names32(const X& doc);
V type_names(const X& doc);
V parameter_names(const X& doc);
V struct_accesses(const X& doc, const V& param);
V takes32(const std::string& signature);
V prototypes(const V& headings, const V& order);
V segment_plan(const V& functions, const V& near, const V& where, const V& sites, const V& code,
               const V& data_segment, const V& other_data, const V& first_selector);
V caller_frame_plan(const V& reports, const V& frames);
V seg_layout(const V& fields);
std::string fragment(const X& node);
V module(const X& doc, const V& moved);
extern const Strs MOVED, IN_MODULE, FLAT, FLAT_ONLY, POWERPC;

}  // namespace cb::translate::ctree
