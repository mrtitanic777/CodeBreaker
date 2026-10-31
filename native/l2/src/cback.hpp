// cback.hpp -- the C back end (codebreaker/translate/backends/c.py) in C++: an L2 function document
// written as C in the decompiler's own dialect, laid out as PrintC lays it out, with c_text's passes
// done on the tree. A port line by line of the Python, which stays the reference (tools/l2c.py --made,
// the gate in docs/NATIVE.md): the same names, the same order of work, the same text.
//
//     std::string cb::function(doc, moved, far, facts)      backends/c.function (facts changed in place)
//     cb::flags, cb::module_names32, cb::type_names, cb::signature, cb::type_text ...
#pragma once
#include <deque>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "pyre.hpp"
#include "pystr.hpp"
#include "value.hpp"

namespace cb {

using py::V;
using Id = const void*;
using Strs = std::vector<std::string>;
using Opt = std::optional<std::string>;

struct Decline : std::runtime_error {
    explicit Decline(const std::string& m) : std::runtime_error(m) {}
};

inline Id id(const V& v) { return v.id(); }

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
    void atom(const std::string& text, const Opt& shown = std::nullopt, bool blank = false, const V* node = nullptr);
    void whole(const Op* tok, const std::string& raw, const Opt& shown, std::optional<bool> paren, const V* node);
    bool parens(const Op* op2);
    void emit(const Entry& e);
};

extern const Op HIDDEN, SUBSCRIPT, CALL, CAST, TYPE_SPACE, TYPE_NOSPACE, PTR, ARRAY, WHOLE;
const Op* member_op(bool arrow);
const Op* unary_op(const std::string& op);        // nullptr where none
const Op* postfix_op(const std::string& op);
const Op* binop(const std::string& op);
const Op* assign_op(const V& op);
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

using Rewrite = std::function<std::string(const V&)>;
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
    V doc;
    std::vector<V> locals;
    std::set<std::string> drop;
    std::optional<std::vector<std::pair<std::string, std::string>>> old_style;
    IdSet room, gone;
    std::map<std::string, std::string> far_pointers;
    std::unordered_map<Id, V> gap_base;
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
        V sel;
    };
    std::vector<SegPtr> seg_ptrs;
    std::set<std::string> frame;
    std::map<std::string, std::string> alias;
    V lane;
    std::unordered_map<Id, V> nodes_by_id;
    std::vector<std::shared_ptr<Flat>> keep;      // every one-line emitter made while laying out (ids stay unique)
    bool has(const char* pass) const { return moved.count(pass) != 0; }
};

extern thread_local Now* now;                      // the function being written (one per thread)

// A fresh state for one call: the Python's module-level _now as it is between calls (reset by
// function's finally), on this thread.
struct Scope {
    Now state;
    Now* was;
    Scope() : was(now) { now = &state; }
    ~Scope() { now = was; }
};

// ---------------------------------------------------------------------------------------------- helpers (Python's dict access)
inline const V& g(const V& d, const char* k) { return d.get(k); }
inline const V& at(const V& d, const char* k) { return d.at(k); }
inline bool is(const V& e, const char* node) { return e.get("node") == node; }
inline bool tr(const V& v) { return py::truthy(v); }
std::string sym_name(const V& e);                  // (e.get('sym') or {}).get('name') as a string ('' for None)
V sym_name_v(const V& e);                          // ... as the value
std::string name_of(const V& e);                   // _name(e)
std::string param_name(const V& p);                // _param_name(p)
std::vector<V> stmts(const V& s);                  // _stmts(s)
bool same(const V& a, const V& b);                 // a is b
bool in(const std::set<std::string>& s, const std::string& x);
bool in(const IdSet& s, Id x);
std::string fmt(const char* f, std::initializer_list<std::string> args);   // '%s' formatting of strings

// ---------------------------------------------------------------------------------------------- the writer (cback.cc)
struct Tok3 {
    int prec;
    std::string op;
    std::string kind;
    bool has_op;
};
Tok3 token(const V& e);
bool bracket(const Tok3& parent, const V& child, int stage);
bool arrow(const V& e);
std::string expr(const V& e);
std::string sub(const Tok3& parent, const V& child, int stage);
std::string lit(const V& e);
std::string int_text(const V& v);
std::string type_text(const V& t, const std::string& name = "", bool far = false);
struct TypeStack {
    std::vector<std::string> mods;      // "ptr", or the array count ("" for none) after "["
    std::vector<bool> is_ptr;
    std::string base;
};
TypeStack type_stack(const V& t);
bool is_far(const V& t);
void push_type(Stack& st, const V& t, const Opt& name, bool far = false, const Opt& word = std::nullopt,
               const Opt& shown_name = std::nullopt, const Opt& shown_base = std::nullopt, bool name_none = false);
void put_type(Em* em, const V& t, const Opt& name, bool far = false, const Opt& shown_name = std::nullopt,
              const Opt& shown_base = std::nullopt);
Flat* make_flat();                                 // (kept until the function is written)
std::string flat_text(const std::function<void(Em*)>& put);
void put(Em* em, const V& e);
void push(Stack& st, const V& e, std::optional<bool> paren = std::nullopt);
std::string raw_text(const V& e);                  // _raw
void rewrite(const V& e, Rewrite shown, bool room = false);
const Op* op_of(const V& e);
void cond(Em* em, const V& e, bool comma);
std::string condition(const V& e, bool inner = false);
void block(Em* em, const V& stmts);
void stmt(Em* em, const V& s);
void comment(Em* em, const std::string& text, std::optional<int> indent);
std::vector<V> locals_of(const V& body);
V ret_of(const V& doc);
std::string signature(const V& doc);
void signature_em(Em* em, const V& doc);
std::string function_text(const V& doc, const std::vector<V>& locs, Em* em = nullptr);
std::string mk_fp_text(const std::string& seg, const V& off, const std::string& comma = ", ", const Opt& shown = std::nullopt);
std::string pops(const std::string& ty);
Opt segment_register(const std::string& name, const V& t);
std::string cast_type(const V& cast);
std::string local_text(const std::string& name);
std::string decl_now(const V& v);
void near_call(const V& e);
V leading_cast(const V& e);
bool data_address(const V& e);
Opt assigned_name(const V& target);
std::string result_shown_base(const V& ret, bool& has);
struct Heading {
    std::string result;
    Strs mods, words;
    bool far_model;
};
Heading heading(const V& doc);
bool lost(const V& doc);
extern const char* ENTRY_SP;

// ---------------------------------------------------------------------------------------------- the passes (cpasses.cc)
struct NodeP {
    V e;
    V p;     // None: no parent
};
using Nodes = std::vector<NodeP>;
Nodes walk(const V& x);
void run_passes(Nodes& nodes, const std::set<std::string>& arrays);
void far_from_stack(const V& body, const std::vector<V>& locs);
std::set<std::string> frame_arrays(const std::vector<V>& locs);
std::optional<std::vector<std::pair<std::string, std::string>>> old_style(const V& doc);
std::map<std::string, std::string> far_pointers_of(const std::vector<V>& locs);
IdSet seen_now();                                  // _seen() (a copy: a later call makes a new set)
extern const std::map<std::string, int> SIZEOF;
Strs seg_layout_text(const std::vector<std::pair<int64_t, std::string>>& fields, bool& ok);

// ---------------------------------------------------------------------------------------------- the entry points (cmodule.cc)
std::string function(const V& doc, const V& moved, const V& far, const V& facts);
V flags(const V& doc, const V& facts);
V module_names32(const V& doc);
V type_names(const V& doc);
V parameter_names(const V& doc);
V struct_accesses(const V& doc, const V& param);
V takes32(const std::string& signature);
V prototypes(const V& headings, const V& order);
V segment_plan(const V& functions, const V& near, const V& where, const V& sites, const V& code,
               const V& data_segment, const V& other_data, const V& first_selector);
V caller_frame_plan(const V& reports, const V& frames);
V seg_layout(const V& fields);
V arguments_at(const std::string& text, int64_t open_paren);
V words_of(const std::string& ty);
V parameters(const std::string& text);
V argument_words(const std::string& arg, const V& types);
std::string fragment(const V& node);
V module(const V& doc, const V& moved);
extern const Strs MOVED, IN_MODULE, FLAT, FLAT_ONLY, POWERPC;

}  // namespace cb
