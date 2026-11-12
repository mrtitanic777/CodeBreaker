// codebreaker/port/asm16.hpp -- COMPAT SHIM over cb/port/asm16.hpp (docs/CPP_DESIGN.md 6.3). Deleted with its last py::
// consumer.
//
// asm16 is refactored: reading a module, the C program's types, the kinds of the registers and the text for ml64 live in
// native/cb/port/asm16*.cpp. This header keeps the old names, records and signatures that the ports which have not been
// refactored use (port/__main__ ports the program's assembly modules; port/asmdata, pre_ansi and sanitize read the
// structures' layouts and the lines of a module):
//
//     parse, strip_comment, split_ops, number, code_segments, CALL_WORDS
//     CTypes (vars, old, new_, far_ptrs; _layouts, field, ...)        c_functions / CSig
//     Port (public_, labels, extrn, cname, ... ; run, emit, exit_of)   listing_offsets, module
//
// A Port here holds the new one: it is built when the shim's Port is, the old fields hold what the module says and what the
// caller sets, and `run` hands the caller's settings (used_outside, param_types, aliases, data_map, far, offsets, cname) to the
// new Port before it runs. Nothing is decided here.
//
// COMPAT(exact/emu/dos): listing_offsets (in asm16.cpp, the one thing left of the old translation unit) runs the old assembler
// under the old emulator wrapper (codebreaker::exact::emu::dos) until that is refactored; the new code is handed it as a callback.
//
// What a caller sees that is different, on purpose: the Python exception names and messages of a malformed module
// (a prefix with no instruction after it, an operand sum of two symbols: Asm16Error here); `offset sym+x` with a displacement
// that is no number is an Asm16Error (it was written short); a push, add, sub or cmp of `offset sym+4` keeps the +4;
// the routine order of `cname` is by name (it was the order of a set of names).
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <tuple>
#include <utility>
#include <vector>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/port/asm16.hpp"
#include "cb/port/struct_layout.hpp"
#include "codebreaker/translate/backends/asm.hpp"

namespace codebreaker::port::asm16 {

namespace newp = ::cb::port::asm16;
namespace compat = ::cb::compat;

struct Asm16Error : py::Exception {
    Asm16Error() : py::Exception() {}
    explicit Asm16Error(const std::string& m) : py::Exception(m) {}
    explicit Asm16Error(const char* m) : py::Exception(m) {}
    explicit Asm16Error(const py::str& m) : py::Exception(m.utf8()) {}
    const char* type() const override { return "Asm16Error"; }
};

// Runs f; an Asm16Error of the new code reaches the caller as the old one, any other cb::Error as the matching py:: exception.
template <class F>
decltype(auto) guarded(F&& f) {
    try {
        return std::forward<F>(f)();
    } catch (const newp::Asm16Error& e) {
        throw Asm16Error(std::string(e.what()));
    } catch (const ::cb::Error& e) {
        compat::rethrow_as_py(e);
    }
}

// ---- reading the source

inline const py::set<py::str> CALL_WORDS = {py::str("near"),    py::str("far"),   py::str("huge"),  py::str("pascal"), py::str("cdecl"),
                                            py::str("fortran"), py::str("_near"), py::str("_far"), py::str("_pascal"), py::str("_cdecl")};

inline py::str strip_comment(const py::str& l) { return compat::to_py(newp::strip_comment(compat::from_py(l))); }
inline py::list<py::str> split_ops(const py::str& s) { return compat::to_py(newp::split_ops(compat::from_py(s))); }
inline std::optional<int64_t> number(const py::str& s) { return newp::number(compat::from_py(s)); }

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
    std::optional<py::str> seg;
    Line(int64_t no_, const py::str& text_) : no(no_), text(text_) {}
};
using LineP = std::shared_ptr<Line>;

inline LineP to_old(const newp::Line& l) {
    LineP out = std::make_shared<Line>(l.no, compat::to_py(l.text));
    out->label = compat::to_py(l.label);
    out->op = compat::to_py(l.op);
    out->prefix = compat::to_py(l.prefix);
    out->args = compat::to_py(l.args);
    if (l.directive) out->directive = Directive{compat::to_py(l.directive->kind), compat::to_py(l.directive->name), compat::to_py(l.directive->rest)};
    out->seg = compat::to_py(l.seg);
    return out;
}

// -> [Line]: label, prefix (rep...), mnemonic, operands - or a directive
inline py::list<LineP> parse(const py::str& text) {
    py::list<LineP> out;
    for (const newp::Line& l : guarded([&] { return newp::parse(compat::from_py(text)); })) out.append(to_old(l));
    return out;
}

inline py::set<py::str> code_segments(const py::list<LineP>& lines) {
    // (the new reader takes lines by its own record: the old ones are read back into it)
    std::vector<newp::Line> back;
    for (const LineP& l : lines) {
        newp::Line n;
        n.no = static_cast<int>(l->no);
        n.text = compat::from_py(l->text);
        n.label = compat::from_py(l->label);
        n.op = compat::from_py(l->op);
        n.prefix = compat::from_py(l->prefix);
        n.args = compat::from_py(l->args);
        if (l->directive) n.directive = newp::Directive{compat::from_py(l->directive->kind), compat::from_py(l->directive->name), compat::from_py(l->directive->rest)};
        n.seg = compat::from_py(l->seg);
        back.push_back(std::move(n));
    }
    return compat::to_py(newp::code_segments(back));
}

// ---- the old assembler's listing

struct ListLine {
    py::str seg;
    int64_t offset = 0;
    int64_t nbytes = 0;
    py::str text;
    py::bytes data;
};
struct Listing {
    py::dict<py::str, std::tuple<py::str, int64_t>> labels;
    py::list<ListLine> lines;
};

inline Listing to_old(const newp::Listing& l) {
    Listing out;
    for (const auto& [name, place] : l.labels) out.labels.set(compat::to_py(name), std::make_tuple(compat::to_py(place.seg), static_cast<int64_t>(place.offset)));
    for (const newp::ListedLine& x : l.lines)
        out.lines.append(ListLine{compat::to_py(x.seg), x.offset, x.length, compat::to_py(x.text), compat::to_py(::cb::ByteView(x.bytes))});
    return out;
}
inline newp::Listing to_new(const Listing& l) {
    newp::Listing out;
    for (const auto& kv : l.labels.items()) {
        const auto& [seg, offset] = std::get<1>(kv);
        out.labels.insert_or_assign(compat::from_py(std::get<0>(kv)), newp::Place{compat::from_py(seg), static_cast<std::uint32_t>(offset)});
    }
    for (const ListLine& x : l.lines)
        out.lines.push_back({compat::from_py(x.seg), static_cast<std::uint32_t>(x.offset), static_cast<std::uint32_t>(x.nbytes), compat::from_py(x.text),
                             compat::from_py(x.data)});
    return out;
}

// {label: (segment, offset)} as the program's own assembler placed them (its listing; the assembler runs under CodeBreaker's
// emulator, nothing of it on the host). COMPAT(exact/emu/dos): defined in asm16.cpp, which has the old emulator wrapper (its
// header pulls in <windows.h>, whose macros this header's names must not meet).
Listing listing_offsets(const py::str& path, const py::str& assembler, const py::str& args = py::str(" /ML T.ASM,T.OBJ,T.LST;"));

// ---- the C program's types

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
    const Layout* get(const py::str& k) const {
        if (!map.contains(k)) return nullptr;
        return &map.at(k);
    }
};

// the member of a structure at an old offset: (name, old offset, old size, new offset, new size, is pointer, its type)
using Field = std::tuple<py::str, int64_t, int64_t, int64_t, int64_t, bool, py::str>;

inline Layout to_old(const ::cb::port::pre_ansi::StructLayout& l) {
    Layout out;
    for (const auto& m : l.members) out.members.emplace_back(compat::to_py(m.name), Member{m.offset, m.size, compat::to_py(m.type), compat::to_py(m.declarator), m.pointer});
    out.size = l.size;
    return out;
}
inline Layouts to_old(const ::cb::port::pre_ansi::StructLayouts& ls) {
    Layouts out;
    for (const auto& [name, layout] : ls) {
        out.map.set(compat::to_py(name), to_old(layout));
        if (name.find('.') != std::string::npos) out.inner.set(compat::to_py(name), std::make_tuple(layout.is_union, to_old(layout)));
    }
    return out;
}
inline ::cb::port::pre_ansi::StructLayouts to_new(const Layouts& ls) {
    ::cb::port::pre_ansi::StructLayouts out;
    for (const auto& kv : ls.map.items()) {
        ::cb::port::pre_ansi::StructLayout l;
        for (const auto& [name, m] : std::get<1>(kv).members)
            l.members.push_back({compat::from_py(name), static_cast<std::uint32_t>(m.off), static_cast<std::uint32_t>(m.size), compat::from_py(m.decl), m.isptr, compat::from_py(m.type)});
        l.size = static_cast<std::uint32_t>(std::get<1>(kv).size);
        if (ls.inner.contains(std::get<0>(kv))) l.is_union = std::get<0>(ls.inner.at(std::get<0>(kv)));
        out.insert_or_assign(compat::from_py(std::get<0>(kv)), std::move(l));
    }
    return out;
}
inline Field to_old(const newp::Field& f) {
    return Field{compat::to_py(f.name), f.old_offset, f.old_size, f.new_offset, f.new_size, f.is_pointer, compat::to_py(f.type)};
}

// the globals' types and the structures' layouts, old (as the 8086 had them) and new (as the port has them)
class CTypes {
  public:
    // (CTypes.__new__(CTypes) with .old and .new set: pre_ansi's cast_twins does this)
    CTypes() = default;
    CTypes(const py::list<py::str>& port_texts, const py::list<py::str>& orig_texts) {
        const newp::CTypes made = guarded([&] {
            const std::vector<std::string> p = compat::from_py(port_texts), o = compat::from_py(orig_texts);
            return newp::CTypes(p, o);
        });
        for (const auto& [name, v] : made.vars) vars.set(compat::to_py(name), std::make_tuple(compat::to_py(v.base), compat::to_py(v.declarator)));
        old = to_old(made.old_layouts);
        new_ = to_old(made.new_layouts);
        for (const std::string& f : made.far_ptrs) far_ptrs.add(compat::to_py(f));
    }

    py::dict<py::str, std::tuple<py::str, py::str>> vars;
    Layouts old;
    Layouts new_;                       // (the Python's .new)
    py::set<py::str> far_ptrs;

    // the new CTypes these are, for what the new code does with them
    newp::CTypes to_new() const {
        newp::CTypes out;
        for (const auto& kv : vars.items())
            out.vars.insert_or_assign(compat::from_py(std::get<0>(kv)),
                                      newp::VarType{compat::from_py(std::get<0>(std::get<1>(kv))), compat::from_py(std::get<1>(std::get<1>(kv)))});
        out.old_layouts = codebreaker::port::asm16::to_new(old);
        out.new_layouts = codebreaker::port::asm16::to_new(new_);
        for (const py::str& f : far_ptrs) out.far_ptrs.insert(compat::from_py(f));
        return out;
    }

    std::optional<std::tuple<int64_t, int64_t>> elem_sizes(const py::str& name) const {
        const auto e = to_new().elem_sizes(compat::from_py(name));
        if (!e) return std::nullopt;
        return std::make_tuple(e->old_size, e->new_size);
    }
    // CTypes._layouts(texts, ptr, far)
    static Layouts _layouts(const py::list<py::str>& texts, int64_t ptr, int64_t far) {
        const std::vector<std::string> t = compat::from_py(texts);
        return to_old(::cb::port::pre_ansi::lay_out_structs(t, static_cast<unsigned>(ptr), static_cast<unsigned>(far)));
    }
    std::optional<std::tuple<py::str, py::str>> var(const py::str& name) const {
        if (!vars.contains(name)) return std::nullopt;
        return vars.at(name);
    }
    std::optional<py::str> pointee(const py::str& name) const {
        if (!is_pointer(name)) return std::nullopt;
        return compat::to_py(to_new().pointee(compat::from_py(name)).value_or(""));
    }
    bool is_pointer(const py::str& name) const { return vars.contains(name) && std::get<1>(vars.at(name)).contains(py::str("*")); }
    std::optional<py::str> member_type(const py::str& structure, int64_t off_old) const {
        return compat::to_py(to_new().member_type(compat::from_py(structure), off_old));
    }
    std::optional<Field> field(const py::str& structure, int64_t off_old) const {
        const auto f = to_new().field(compat::from_py(structure), off_old);
        if (!f) return std::nullopt;
        return to_old(*f);
    }
};

// ---- C functions

struct CSig {                      // (pascal, [(the old size of each argument, val|long|ptr|double)], what it returns, to)
    bool pascal = false;
    py::list<std::tuple<int64_t, py::str>> args;
    py::str ret;
    std::optional<py::str> to;
};

inline CSig to_old(const newp::CSig& s) {
    static const char* const kinds[] = {"val", "long", "ptr", "double"};
    static const char* const rets[] = {"val", "long", "ptr", "void"};
    CSig out;
    out.pascal = s.pascal;
    for (const newp::CArg& a : s.args) out.args.append(std::make_tuple(static_cast<int64_t>(a.size), py::str(kinds[static_cast<int>(a.kind)])));
    out.ret = py::str(rets[static_cast<int>(s.ret)]);
    out.to = compat::to_py(s.to);
    return out;
}
inline newp::CSig to_new(const CSig& s) {
    newp::CSig out;
    out.pascal = s.pascal;
    for (const auto& a : s.args) {
        const std::string k = std::get<1>(a).utf8();
        out.args.push_back({static_cast<int>(std::get<0>(a)), k == "long" ? newp::CArg::Kind::Long : k == "ptr" ? newp::CArg::Kind::Ptr : k == "double" ? newp::CArg::Kind::Double : newp::CArg::Kind::Val});
    }
    const std::string r = s.ret.utf8();
    out.ret = r == "long" ? newp::CSig::Ret::Long : r == "ptr" ? newp::CSig::Ret::Ptr : r == "void" ? newp::CSig::Ret::Void : newp::CSig::Ret::Val;
    out.to = compat::from_py(s.to);
    return out;
}

inline py::dict<py::str, CSig> c_functions(const py::list<py::str>& texts) {
    py::dict<py::str, CSig> out;
    const std::vector<std::string> t = compat::from_py(texts);
    for (const auto& [name, sig] : guarded([&] { return newp::c_functions(t); })) out.set(compat::to_py(name), to_old(sig));
    return out;
}

// ---- what a register holds (only what a caller reads of a routine's exit)

struct K {
    py::str kind;
    std::optional<py::str> to;
    int64_t old = 0;
    int64_t new_ = 0;
    bool wide() const { return kind == "ptr" || kind == "opq" || kind == "frame"; }
};
struct State {
    std::map<py::str, K> r;                       // (16-bit name: K)
};

// An assembler module's source file as its L2 Module: one MachineFragment, its text as written.
inline codebreaker::translate::backends::asm_::Document module(const py::str& name, const py::str& text) {
    namespace asm_ = codebreaker::translate::backends::asm_;
    asm_::Unit unit;
    unit.node = py::str("MachineFragment");
    unit.asm_ = text;
    unit.name = name;
    unit.syntax = py::str("masm");
    asm_::Document d;
    d.node = py::str("Module");
    py::list<asm_::Unit> us;
    us.append(unit);
    d.units = us;
    return d;
}

// ---- one module

// The port of one assembler module.
class Port {
  public:
    Listing offsets;                                           // ({label: (segment, offset)}: the old assembler's listing)
    py::dict<py::str, py::list<std::tuple<int64_t, std::optional<py::str>>>> param_types;
    py::list<std::tuple<int64_t, py::str>> data_map;           // ([(old address in the data segment, name)], sorted)
    py::dict<py::str, std::tuple<py::str, int64_t>> aliases;   // (C name: (owner, byte offset))
    std::set<std::optional<py::str>> far;
    py::set<py::str> used_outside;                             // (its routines the other modules call)
    py::set<py::str> public_;
    py::dict<py::str, int64_t> labels;
    py::dict<py::str, py::str> extrn;
    py::dict<py::str, py::str> cname;                          // (a public routine: the name the C program calls)
    py::list<int64_t> unreached;

    // doc: the 16-bit module, as its L2 Module (module(name, text) makes one of a source file); ctypes: CTypes; cnames: the names
    // the C program calls (to match the routines' public names, in capitals for pascal ones); pascal_: those it calls the pascal
    // way; cfuncs: the C program's functions (c_functions); asm_routines: the public routines of the program's other assembly
    // modules
    Port(const codebreaker::translate::backends::asm_::Document& doc, const CTypes& ctypes, const py::list<py::str>& cnames = py::list<py::str>(),
         const py::set<py::str>& pascal_ = py::set<py::str>(), const std::optional<py::dict<py::str, CSig>>& cfuncs = std::nullopt,
         const py::list<py::str>& asm_routines = py::list<py::str>(), bool underscore = true)
        : ct_(std::make_unique<newp::CTypes>(ctypes.to_new())) {
        newp::PortSettings s;
        s.c_names = compat::from_py(cnames);
        for (const py::str& p : pascal_) s.pascal.insert(compat::from_py(p));
        if (cfuncs)
            for (const auto& kv : cfuncs->items()) s.c_funcs.insert_or_assign(compat::from_py(std::get<0>(kv)), to_new(std::get<1>(kv)));
        for (const py::str& r : asm_routines) s.asm_routines.insert(compat::from_py(r));
        s.underscore = underscore;
        const std::string text = compat::from_py(codebreaker::translate::backends::asm_::module(doc));
        impl_ = guarded([&] { return std::make_unique<newp::Port>(newp::module("module", text), *ct_, std::move(s)); });
        for (const std::string& p : impl_->public_names()) public_.add(compat::to_py(p));
        for (const auto& [l, i] : impl_->labels()) labels.set(compat::to_py(l), static_cast<int64_t>(i));
        for (const auto& [n, k] : impl_->externs()) extrn.set(compat::to_py(n), compat::to_py(k));
        for (const auto& [p, c] : impl_->cname) cname.set(compat::to_py(p), compat::to_py(c));
    }

    Port& run() {
        impl_->used_outside = compat::from_py(used_outside);
        impl_->param_types.clear();
        for (const auto& kv : param_types.items()) {
            std::vector<newp::ParamType> v;
            for (const auto& [size, to] : std::get<1>(kv)) v.push_back({static_cast<int>(size), compat::from_py(to)});
            impl_->param_types[compat::from_py(std::get<0>(kv))] = std::move(v);
        }
        impl_->aliases.clear();
        for (const auto& kv : aliases.items())
            impl_->aliases[compat::from_py(std::get<0>(kv))] = newp::Alias{compat::from_py(std::get<0>(std::get<1>(kv))), std::get<1>(std::get<1>(kv))};
        impl_->data_map.clear();
        for (const auto& [a, n] : data_map) impl_->data_map.emplace_back(static_cast<std::uint32_t>(a), compat::from_py(n));
        impl_->far.clear();
        for (const std::optional<py::str>& f : far) impl_->far.insert(compat::from_py(f));
        impl_->offsets = to_new(offsets);
        impl_->cname.clear();
        for (const auto& kv : cname.items()) impl_->cname[compat::from_py(std::get<0>(kv))] = compat::from_py(std::get<1>(kv));
        impl_->all_labels_public = ::cb::fs::get_env("CB_ASM16_DEBUG") == std::optional<std::string>("1");   // (every label public: a debugger names them)
        guarded([&] { impl_->run(); });
        far.clear();
        for (const newp::Rt& f : impl_->far) far.insert(compat::to_py(f));
        return *this;
    }

    // the module for ml64: data, an entry of today's C for each routine it calls, the routines
    py::str emit(const py::set<py::str>& longret = py::set<py::str>()) {
        std::set<std::string> lr = compat::from_py(longret);
        const std::string text = guarded([&] { return impl_->emit(lr); });
        unreached = compat::to_py(std::vector<int64_t>(impl_->unreached().begin(), impl_->unreached().end()));
        return compat::to_py(text);
    }

    // what a routine leaves (all its returns merged): its registers' kinds; none when it never returns
    std::optional<State> exit_of(const py::str& label) const {
        const auto s = impl_->exit_of(compat::from_py(label));
        if (!s) return std::nullopt;
        static const char* const kinds[] = {"val", "ptr", "opq", "frame", "seg", "ret"};
        State out;
        for (const std::string_view r : newp::kReg16) {
            const newp::K* k = s->reg(r);
            if (!k) continue;
            out.r[py::str(std::string(r))] = K{py::str(kinds[static_cast<int>(k->kind)]), compat::to_py(k->to), k->old, k->new_};
        }
        return out;
    }

  private:
    std::unique_ptr<newp::CTypes> ct_;
    std::unique_ptr<newp::Port> impl_;
};

}  // namespace codebreaker::port::asm16
