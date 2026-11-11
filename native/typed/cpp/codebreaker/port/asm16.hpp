// codebreaker/port/asm16.hpp -- typed C++ port of codebreaker.port.asm16 (see pre_ansi.hpp for the sibling).
//
// asm16.py -- a program's own 16-bit assembly language, made assembly language for a 64-bit machine.
// (The module docstring is kept in asm16.cpp.)
//
// TYPING NOTES
//   * A structure's layout (an OrderedDict of "member": (offset, size, type, declarator, is pointer) plus the key
//     "@size") is a Layout record: `members` in order, and `size` (the "@size" entry).
//   * Layouts (the Python dict subclass) is the record Layouts: `map` {"struct S": Layout, and the nested
//     "struct S.member" entries the nesting adds} and `inner` {"struct S.member": (is a union, its Layout)}.
#pragma once

#include <cstdint>
#include <deque>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <unordered_map>
#include <tuple>
#include <utility>
#include <vector>

#include "py.hpp"
#include "py_os.hpp"

#include "codebreaker/translate/backends/asm.hpp"

namespace codebreaker::port::asm16 {

// one member of a layout: (offset, size, type text, declarator text, is a pointer)
struct Member {
    int64_t off = 0;
    int64_t size = 0;
    py::str type;
    py::str decl;
    bool isptr = false;
    friend bool operator==(const Member& a, const Member& b) {
        return a.off == b.off && a.size == b.size && a.type == b.type && a.decl == b.decl && a.isptr == b.isptr;
    }
};

// {member: Member..., "@size": n}
struct Layout {
    std::vector<std::pair<py::str, Member>> members;   // (in their order)
    int64_t size = 0;                                  // ("@size")
    bool has(const py::str& name) const {
        for (const auto& kv : members) if (kv.first == name) return true;
        return false;
    }
    // the member, or nullptr
    const Member* find(const py::str& name) const {
        for (const auto& kv : members) if (kv.first == name) return &kv.second;
        return nullptr;
    }
    Member& set(const py::str& name, const Member& m) {
        for (auto& kv : members) if (kv.first == name) { kv.second = m; return kv.second; }
        members.emplace_back(name, m);
        return members.back().second;
    }
    friend bool operator==(const Layout& a, const Layout& b) { return a.size == b.size && a.members == b.members; }
};

// the structures' layouts; .inner: {"struct S.member": (is a union, its layout)} of those inside them
struct Layouts {
    py::dict<py::str, Layout> map;
    py::dict<py::str, std::tuple<bool, Layout>> inner;
    bool contains(const py::str& k) const { return map.contains(k); }
    const Layout& at(const py::str& k) const { return map.at(k); }
    // layouts.get(k): nullptr when absent
    const Layout* get(const py::str& k) const {
        if (!map.contains(k)) return nullptr;
        return &map.at(k);
    }
};

// the member of a structure at an old offset: (name, old offset, old size, new offset, new size, is pointer, its type)
using Field = std::tuple<py::str, int64_t, int64_t, int64_t, int64_t, bool, py::str>;

// the globals' types and the structures' layouts, old (as the 8086 had them) and new (as the port has them)
class CTypes {
  public:
    // (CTypes.__new__(CTypes) with .old and .new set: pre_ansi's cast_twins does this)
    CTypes() = default;
    CTypes(const py::list<py::str>& port_texts, const py::list<py::str>& orig_texts);

    py::dict<py::str, std::tuple<py::str, py::str>> vars;
    Layouts old;
    Layouts new_;                       // (the Python's .new)
    py::set<py::str> far_ptrs;

    std::optional<std::tuple<int64_t, int64_t>> elem_sizes(const py::str& name) const;
    // CTypes._layouts(texts, ptr, far)
    static Layouts _layouts(const py::list<py::str>& texts, int64_t ptr, int64_t far);
    std::optional<std::tuple<py::str, py::str>> var(const py::str& name) const;
    std::optional<py::str> pointee(const py::str& name) const;
    bool is_pointer(const py::str& name) const;
    std::optional<py::str> member_type(const py::str& structure, int64_t off_old) const;
    std::optional<Field> field(const py::str& structure, int64_t off_old) const;
};

// ---------------------------------------------------------------- the rest of the module
//
// More typing notes:
//   * An operand is ('reg', name) ('imm', text) ('off', symbol, disp) ('mem', Mem) ('sym', name): the record Operand
//     (kind; text = the name / text / symbol; disp; mem).
//   * A Line is shared (the Python's Line objects are referred to from the lines, the code and the data): LineP.
//     Its directive is (kind, name or None, rest): the record Directive.
//   * The old assembler's listing (Port.offsets: {label: (segment, offset)} with the key "@lines" holding
//     [(segment, offset, bytes, text, data)]) is the record Listing.
//   * What a register holds (K): `to` is what a pointer points to (a str); a frame register's owner (the Python keeps
//     it in the same `to`, as a tuple) is `owner`. Kinds' origins (frozenset of tuples) are std::set<Origin>.
//   * The routine of a line may be None: std::optional<str> (Rt) wherever the Python keys a dict by one.
//   * Port.doc is the module as the back end's document (asm_::Document): module(name, text) makes one through the
//     L2 model (l2.module / l2.fragment) as the Python does.

extern const py::list<py::str> R16;
extern const py::dict<py::str, py::str> R8;
extern const py::dict<py::str, py::str> R64;
extern const py::set<py::str> SEG;
extern const py::set<py::str> JUMPS;
extern const py::set<py::str> STRING;
extern const py::dict<py::str, int64_t> SIZES;
extern const py::list<py::str> R16_CODE;
extern const py::list<py::str> R8_CODE;
extern const py::set<py::str> CALL_WORDS;

struct Asm16Error : py::Exception {
    Asm16Error() : py::Exception() {}
    explicit Asm16Error(const std::string& m) : py::Exception(m) {}
    explicit Asm16Error(const char* m) : py::Exception(m) {}
    explicit Asm16Error(const py::str& m) : py::Exception(m.utf8()) {}
    const char* type() const override { return "Asm16Error"; }
};

// ---- reading the source
py::str strip_comment(const py::str& l);
py::list<py::str> split_ops(const py::str& s);

// (kind, name or None, the rest) of a directive line
struct Directive {
    py::str kind;
    std::optional<py::str> name;
    py::str rest;
};

struct Line {
    int64_t no = 0;
    py::str text;
    std::optional<py::str> label;
    std::optional<py::str> op;
    std::optional<py::str> prefix;
    py::list<py::str> args;
    std::optional<Directive> directive;
    std::optional<py::str> seg;       // (set on the lines of the code segments)
    Line(int64_t no_, const py::str& text_) : no(no_), text(text_) {}
};
using LineP = std::shared_ptr<Line>;

// -> [Line]: label, prefix (rep...), mnemonic, operands - or a directive
py::list<LineP> parse(const py::str& text);

// the listing of the program's own assembler: {label: (segment, offset)} and its lines
struct ListLine {
    py::str seg;
    int64_t offset = 0;
    int64_t nbytes = 0;
    py::str text;          // (the source line's start)
    py::bytes data;
};
struct Listing {
    py::dict<py::str, std::tuple<py::str, int64_t>> labels;
    py::list<ListLine> lines;     // the "@lines" entry
};
Listing listing_offsets(const py::str& path, const py::str& assembler,
                        const py::str& args = py::str(" /ML T.ASM,T.OBJ,T.LST;"));

struct Mem {
    std::optional<int64_t> size;
    std::optional<py::str> seg;
    std::optional<py::str> sym;
    py::list<py::str> regs;
    int64_t disp = 0;
    py::str text;
    Mem() = default;
    Mem(std::optional<int64_t> size_, std::optional<py::str> seg_, std::optional<py::str> sym_,
        const py::list<py::str>& regs_, int64_t disp_, const py::str& text_)
        : size(size_), seg(seg_), sym(sym_), regs(regs_), disp(disp_), text(text_) {}
};

// (length, mnemonic, [operands]) of the 8086's moves to and from a direct address written as bytes - None otherwise
std::optional<std::tuple<int64_t, py::str, py::list<py::str>>> decode_direct(const py::bytes& b);
py::set<py::str> code_segments(const py::list<LineP>& lines);
std::optional<int64_t> number(const py::str& s);

// the module's equates; .data: its variables' sizes (a bare name of one is its memory)
struct Equates {
    py::dict<py::str, py::str> map;
    py::dict<py::str, int64_t> data;
};

// ('reg', name) ('imm', value text) ('off', symbol, disp) ('mem', Mem) ('sym', name)
struct Operand {
    py::str kind;
    py::str text;
    int64_t disp = 0;
    bool disp_none = false;     // ('off' whose displacement the Python leaves None: offset x+zz)
    std::shared_ptr<Mem> mem;
    friend bool operator==(const Operand& a, const Operand& b) {
        return a.kind == b.kind && a.text == b.text && a.disp == b.disp && a.disp_none == b.disp_none && a.mem == b.mem;
    }
};
Operand operand(const py::str& a, const Equates* equ = nullptr);

// ---- kinds
struct FrameOwner {
    std::optional<py::str> routine;
    std::vector<std::pair<int64_t, int64_t>> stack;
    friend bool operator==(const FrameOwner& a, const FrameOwner& b) { return a.routine == b.routine && a.stack == b.stack; }
};

struct Origin {
    py::str kind;     // "in" (a, b), "param" (a, n), "segof" (a)
    py::str a;
    py::str b;
    int64_t n = 0;
    friend bool operator==(const Origin& x, const Origin& y) { return x.kind == y.kind && x.a == y.a && x.b == y.b && x.n == y.n; }
    friend bool operator<(const Origin& x, const Origin& y) {
        return std::tie(x.kind, x.a, x.b, x.n) < std::tie(y.kind, y.a, y.b, y.n);
    }
};

// a frozenset of origins, shared between the kinds that hold it (the Python's K objects are shared references; the
// copy of a K is cheap here for the same reason): copy on write
class OriginSet {
    std::shared_ptr<const std::set<Origin>> p_;
    static const std::set<Origin>& none() {
        static const std::set<Origin> e;
        return e;
    }

  public:
    OriginSet() = default;
    OriginSet(const std::set<Origin>& s) : p_(s.empty() ? nullptr : std::make_shared<const std::set<Origin>>(s)) {}
    const std::set<Origin>& get() const { return p_ ? *p_ : none(); }
    operator const std::set<Origin>&() const { return get(); }
    std::set<Origin>::const_iterator begin() const { return get().begin(); }
    std::set<Origin>::const_iterator end() const { return get().end(); }
    bool empty() const { return !p_ || p_->empty(); }
    void insert(const Origin& o) {
        auto n = std::make_shared<std::set<Origin>>(get());
        n->insert(o);
        p_ = n;
    }
    template <class It> void insert(It first, It last) {
        auto n = std::make_shared<std::set<Origin>>(get());
        n->insert(first, last);
        p_ = n;
    }
    friend bool operator==(const OriginSet& a, const OriginSet& b) { return a.p_ == b.p_ || a.get() == b.get(); }
    friend bool operator!=(const OriginSet& a, const OriginSet& b) { return !(a == b); }
};

// what a register holds: 'val' (a number, 16 bits), 'ptr' (an address; to = what it points to), 'opq' (unknown:
// possibly an address - kept whole), 'frame' (sp as it was at entry + old, new offsets), 'seg'
struct K {
    py::str kind;
    std::optional<py::str> to;
    std::optional<FrameOwner> owner;
    int64_t old = 0;
    int64_t new_ = 0;
    OriginSet origin;
    K() = default;
    explicit K(const py::str& kind_, const std::optional<py::str>& to_ = std::nullopt, int64_t old_ = 0, int64_t new__ = 0,
               const OriginSet& origin_ = OriginSet())
        : kind(kind_), to(to_), old(old_), new_(new__), origin(origin_) {}
    bool wide() const { return kind == "ptr" || kind == "opq" || kind == "frame"; }
    bool same_key(const K& o) const {
        return kind == o.kind && to == o.to && owner == o.owner && old == o.old && new_ == o.new_;
    }
};
extern const K VAL;

struct CSig {                      // (pascal, [(the old size of each argument, val|long|ptr|double)], what it returns, to)
    bool pascal = false;
    py::list<std::tuple<int64_t, py::str>> args;
    py::str ret;
    std::optional<py::str> to;
};
extern const py::dict<py::str, CSig> RUNTIME_C;

py::dict<py::str, CSig> c_functions(const py::list<py::str>& texts);
py::str inner_name(const py::str& label, const py::set<py::str>& cnames);
extern py::dict<py::str, py::str> FIRST_MEMBER;     // (struct: the type of its first member, a pointer)
std::optional<K> join(const std::optional<K>& a, const std::optional<K>& b);

struct StackItem {
    int64_t old = 0;
    int64_t new_ = 0;
    std::optional<K> k;
};
struct State {
    std::map<py::str, K> r;                       // (16-bit name: K)
    std::vector<StackItem> stack;                 // (from entry down)
    int64_t sp_old = 0, sp_new = 0;
    std::set<py::str> routines;                   // (the routines this code runs for: a ret returns theirs)
    bool same_key(const State& o) const;
};
State merge(const std::optional<State>& a, const State& b);
State merge(const State* a, const State& b);
void merge_into(State& a, const State& b);

// An assembler module's source file as its L2 Module: one MachineFragment, its text as written.
codebreaker::translate::backends::asm_::Document module(const py::str& name, const py::str& text);

using Rt = std::optional<py::str>;                // (a routine: its label, or None)

// the control flow out of an instruction
struct Flow {
    enum Kind { Next, Ret, Jump, Call } kind = Next;
    py::str label;
};

struct Callee {                    // ("asm", name) or ("c", C name, sig)
    py::str kind;
    py::str name;
    CSig sig;
};

// The port of one assembler module.
class Port {
  public:
    Listing offsets;                               // ({label: (segment, offset)}: the old assembler's listing)
    py::dict<py::str, py::list<std::tuple<int64_t, std::optional<py::str>>>> param_types;
    py::list<std::tuple<int64_t, py::str>> data_map;   // ([(old address in the data segment, name)], sorted)
    py::dict<py::str, std::tuple<py::str, int64_t>> aliases;   // (C name: (owner, byte offset))
    std::set<Rt> far;
    std::set<std::tuple<py::str, int64_t>> own_ptr;
    std::map<Rt, std::vector<State>> exits;
    std::map<Rt, std::vector<State>> exits_new;

    codebreaker::translate::backends::asm_::Document doc;
    py::list<LineP> lines;
    py::dict<py::str, CSig> cfuncs;
    py::set<py::str> data_used;
    py::set<py::str> asm_routines;
    py::set<py::str> cnames_all;
    py::set<py::str> used_outside;
    py::dict<py::str, CSig> c_calls;
    CTypes ct;
    py::set<py::str> public_;                          // (the Python's self.public)
    py::dict<py::str, py::str> extrn;
    Equates equ;
    py::set<py::str> code_seg;
    py::dict<py::str, int64_t> labels;
    std::vector<std::pair<py::str, int64_t>> label_list;   // (labels, in their order: for the loops that look at all)
    py::list<LineP> code;
    py::list<std::tuple<py::str, LineP>> data;
    py::dict<py::str, py::str> cname;
    py::set<py::str> pascal;
    py::list<py::str> notes;
    py::dict<int64_t, std::tuple<int64_t, int64_t>> dline;
    py::set<py::str> called;
    std::map<int64_t, State> before;
    std::map<Rt, std::vector<std::tuple<int64_t, int64_t>>> params;
    std::map<Rt, py::set<int64_t>> ptr_args;
    py::set<py::str> asm_calls;
    py::list<int64_t> unreached;
    bool used_int21 = false;
    bool used_null = false;
    std::map<py::str, py::set<py::str>> _writes;
    std::unordered_map<const Line*, py::list<Operand>> opcache_;

    // doc: the 16-bit module, as its L2 Module (module(name, text) makes one of a source file); ctypes: CTypes; cnames: the
    // names the C program calls (to match the routines' public names, in capitals for pascal ones); pascal_: those it
    // calls the pascal way; cfuncs: the C program's functions (c_functions); asm_routines: the public routines of the
    // program's other assembly modules
    Port(const codebreaker::translate::backends::asm_::Document& doc, const CTypes& ctypes,
         const py::list<py::str>& cnames = py::list<py::str>(), const py::set<py::str>& pascal_ = py::set<py::str>(),
         const std::optional<py::dict<py::str, CSig>>& cfuncs = std::nullopt,
         const py::list<py::str>& asm_routines = py::list<py::str>(), bool underscore = true);

    std::optional<std::tuple<py::str, int64_t>> code_label(const std::optional<py::str>& seg, int64_t value) const;
    void place_data();
    std::optional<std::tuple<py::str, int64_t>> data_address(const py::str& seg, int64_t v) const;
    py::set<py::str> table_targets() const;
    bool cs_address(int64_t i, const py::str& r) const;
    State entry_state(const py::str& routine) const;
    void analyse();
    void _flow(int64_t j, const State& st, std::deque<int64_t>& work);
    Mem absolute(const Mem& m);
    std::tuple<K, std::optional<Field>> kind_of_mem(const Mem& m, const State& st);
    py::dict<int64_t, py::str> param_place(const Rt& routine) const;
    std::tuple<int64_t, int64_t> element_sizes(const K& k) const;
    std::optional<std::tuple<py::str, int64_t>> data_var(int64_t addr) const;
    std::optional<py::str> data_at(std::optional<int64_t> v) const;
    bool ds_base(int64_t i, const py::str& r) const;
    Mem ds_mem(const Mem& m, const K& k);
    std::optional<Callee> callee(const py::str& t) const;
    static std::tuple<py::list<std::tuple<int64_t, int64_t, py::str, int64_t>>, int64_t> slots(const CSig& sig);
    py::list<py::str> c_thunk(const py::str& name, const CSig& sig) const;
    py::str c(const py::str& sym) const;
    std::tuple<State, py::list<Flow>> step(int64_t i, const LineP& l, State st);
    py::set<py::str> writes(const py::str& label, py::set<py::str>* seen = nullptr);
    Rt routine(int64_t i) const;
    std::optional<State> exit_of(const py::str& label) const;
    Port& run();
    py::dict<py::str, int64_t> own_data() const;
    std::set<std::tuple<py::str, int64_t>> own_slots();
    bool used_as_address(const py::str& sym, int64_t d);
    bool address_use(int64_t j0, const py::str& r, int64_t depth, std::set<int64_t>& seen);
    int64_t own_place(const py::str& sym, int64_t disp) const;
    Port& prepare();
    py::list<Operand> operands_of(const LineP& l);
    void _params();
    int64_t retsize(const Rt& r) const;
    std::tuple<int64_t, int64_t> slot(const Rt& r, int64_t e) const;
    py::str emit(const py::set<py::str>& longret = py::set<py::str>());
    py::list<py::str> code_data(const LineP& l);
    py::str local(const py::str& label) const;
    py::list<py::str> data_text();
    py::list<py::str> wrapper(const py::str& p, bool longret);
    py::str reg(const py::str& r, const State& st, std::optional<bool> wide = std::nullopt) const;
    py::str sym_any(const py::str& s) const;
    py::str sym(const py::str& s) const;
    py::str mem(const Mem& m, const State& st, int64_t size, int64_t i, py::list<py::str>& pre);
    void as_address(int64_t i, const std::optional<K>& k);
    int64_t stack_place(const State& st, int64_t e) const;
    int64_t access_size(const LineP& l, const py::list<Operand>& a, const State& st, int64_t which);
    py::list<py::str> instr(int64_t i, const LineP& l, const State& st);
    int64_t sp_new_bytes(const State& st, int64_t n, const py::str& op) const;
};

}  // namespace codebreaker::port::asm16
