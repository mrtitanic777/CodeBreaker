// codebreaker/port/pre_ansi_p4.cpp -- SHIM over native/cb/port (docs/CPP_DESIGN.md 6.3): the old types in, the new
// functions called, the old types out. COMPAT(py): deleted with the last py:: consumer of PART 4 of pre_ansi (the
// Port class of pre_ansi.hpp: the pre_ansi, asm16 and sanitize modules, once refactored, call cb::port directly).
//
// The module this shim stands for is PART 4 of codebreaker.port.pre_ansi (Python lines 3077-4261): the Port methods
// bss_views .. _same_def and the free functions split_view_lists .. said. Its code is in native/cb/port/*.
//
// What is here besides conversion: the two layouts the old callers get as asm16::Layouts (old_layouts,
// old_layouts_for; the new code lays out the same structures itself but the old layer's Layouts carries what its
// callers read: member types and declarators -- goes with asm16), `said` (it must hand back an old py::re::Match),
// and the code-point offsets of the old py::str where a function returns or takes one (call_args, _calls,
// object_before).

#include <algorithm>
#include <map>
#include <set>

#include "cb/compat/py_bridge.hpp"
#include "cb/port/bss.hpp"
#include "cb/port/c_source.hpp"
#include "cb/port/complaints.hpp"
#include "cb/port/frames.hpp"
#include "cb/port/headers.hpp"
#include "cb/port/old_sizes.hpp"
#include "cb/port/original_program.hpp"
#include "cb/port/rewrite.hpp"
#include "cb/port/struct_layout.hpp"
#include "cb/port/union_views.hpp"

#include "codebreaker/base/ctext.hpp"
#include "codebreaker/port/pre_ansi.hpp"

namespace codebreaker::port::pre_ansi {

namespace newc = ::cb::port::pre_ansi;
namespace cc = ::cb::compat;
using asm16::Layouts;
using py::list;
using py::str;
using OrderedFns = ::cb::OrderedMap<std::string, newc::Fn>;

// the new code's state for the BSS list and the original program: made when first needed, from the Port's originals
struct P4Native {
    std::unique_ptr<newc::OriginalProgram> original;
    std::unique_ptr<newc::BssPlan> plan;
    newc::BssSizes sizes;
    std::vector<newc::DeclRule> rules;   // decl_rules converted so far (the list only grows)
};

namespace {

// ---- text and containers ------------------------------------------------------------------------------------

std::string U(const str& s) { return cc::from_py(s); }
str P(std::string_view s) { return cc::to_py(s); }

newc::Files files_of(const py::dict<str, str>& d) {
    newc::Files out;
    for (const auto& kv : d.items()) out.insert_or_assign(U(std::get<0>(kv)), U(std::get<1>(kv)));
    return out;
}

std::vector<std::string> strings_of(const list<str>& l) { return cc::from_py(l); }

template <class T>
std::optional<unsigned> unsigned_of(const std::optional<T>& v) {
    if (!v) return std::nullopt;
    return static_cast<unsigned>(*v);
}

newc::Profile profile_of(const Profile& p) {
    newc::Profile n;
    n.name = U(p.name);
    n.libc = strings_of(p.libc);
    n.runtime_defines = strings_of(p.runtime_defines);
    n.runtime_headers = strings_of(p.runtime_headers);
    n.runtime_internals = strings_of(p.runtime_internals);
    n.platform_header = U(p.platform_header);
    n.platform_calls = strings_of(p.platform_calls);
    for (const auto& kv : p.platform_renames.items()) n.platform_renames[U(std::get<0>(kv))] = U(std::get<1>(kv));
    n.widths_pointer = unsigned_of(p.widths_pointer);
    n.int_storage = unsigned_of(p.int_storage);
    n.printf_hex_upper = p.printf_hex_upper;
    n.struct_align = unsigned_of(p.struct_align);
    if (p.keywords_removed) n.keywords_removed = strings_of(*p.keywords_removed);
    n.arg_order = U(p.arg_order);
    return n;
}

newc::Fn fn_of(const Fn& f, bool with_body) {
    newc::Fn n;
    n.name = U(f.name);
    n.ret = U(f.ret);
    for (const auto& pr : f.params) n.params.push_back({U(std::get<0>(pr)), U(std::get<1>(pr))});
    n.is_static = f.static_;
    n.line = static_cast<std::size_t>(f.line);
    n.returns_void = f.void_;
    n.loose = f.loose;
    n.bare = static_cast<std::size_t>(f.bare);
    n.varargs = f.varargs;
    n.va_slots = f.va_slots;
    n.ret_set = f.ret_set;
    n.int_ret = f.int_ret;
    n.keep_int = f.keep_int;
    for (const str& x : f.implicit) n.implicit.insert(U(x));
    if (f.file) n.file = U(*f.file);
    if (with_body) n.body = U(f.body);
    return n;
}

void update_fn(Fn& f, const newc::Fn& n) {
    f.ret = P(n.ret);
    f.params = list<std::tuple<str, str>>();
    for (const newc::Param& p : n.params) f.params.append({P(p.name), P(p.type)});
    f.static_ = n.is_static;
    f.void_ = n.returns_void;
    f.loose = n.loose;
    f.bare = static_cast<int64_t>(n.bare);
    f.varargs = n.varargs;
    f.va_slots = n.va_slots;
    f.ret_set = n.ret_set;
    f.int_ret = n.int_ret;
    f.keep_int = n.keep_int;
    f.implicit = py::set<str>();
    for (const std::string& x : n.implicit) f.implicit.add(P(x));
    f.body = P(n.body);
}

OrderedFns fns_of(const py::dict<str, FnP>& d, bool with_body) {
    OrderedFns out;
    for (const auto& kv : d.items()) out.insert_or_assign(U(std::get<0>(kv)), fn_of(*std::get<1>(kv), with_body));
    return out;
}

newc::Aliases aliases_of(const py::dict<str, std::tuple<str, str>>& d) {
    newc::Aliases out;
    for (const auto& kv : d.items()) out.insert_or_assign(U(std::get<0>(kv)), newc::Alias{U(std::get<0>(std::get<1>(kv))), U(std::get<1>(std::get<1>(kv)))});
    return out;
}

void put_aliases(const py::dict<str, std::tuple<str, str>>& d, const newc::Aliases& a) {
    d.clear();
    for (const auto& kv : a) d.set(P(kv.first), {P(kv.second.where), P(kv.second.declaration)});
}

// the notes a call added: appended to the old Port's
void append_notes(Notes& notes, const newc::Notes& added) {
    for (const auto& kv : added) {
        list<str>& l = notes.setdefault(P(kv.first), list<str>());
        for (const std::string& v : kv.second) l.append(P(v));
    }
}

newc::Notes notes_of(const Notes& notes) {
    newc::Notes out;
    for (const auto& kv : notes.items())
        for (const str& v : std::get<1>(kv)) out[U(std::get<0>(kv))].push_back(U(v));
    return out;
}

// ---- layouts the old callers get ---------------------------------------------------------------------------

std::string joined(const std::vector<std::string>& v) {
    std::string out;
    for (const std::string& s : v) out += (out.empty() ? "" : "\n") + s;
    return out;
}

// ---- the old py::str counts code points; the new code counts bytes -------------------------------------------

bool is_ascii(const std::string& s) {
    return std::all_of(s.begin(), s.end(), [](char c) { return static_cast<unsigned char>(c) < 0x80; });
}
std::size_t cp_of_byte(const std::string& s, std::size_t byte) {
    std::size_t n = 0;
    for (std::size_t i = 0; i < byte && i < s.size(); ++i) n += (static_cast<unsigned char>(s[i]) & 0xC0) != 0x80;
    return n;
}
std::size_t byte_of_cp(const std::string& s, std::size_t cp) {
    std::size_t n = 0, i = 0;
    while (i < s.size() && n < cp) {
        ++i;
        while (i < s.size() && (static_cast<unsigned char>(s[i]) & 0xC0) == 0x80) ++i;
        ++n;
    }
    return i;
}

// ---- Python's replacement templates and the new regex's ---------------------------------------------------------

// \g<1>, \1, \\ -> ${1}, ${1}, \ ; a literal '$' is '$$'
std::string template_from_python(const std::string& t) {
    std::string out;
    for (std::size_t i = 0; i < t.size(); ++i) {
        if (t[i] == '$') {
            out += "$$";
        } else if (t[i] == '\\' && i + 1 < t.size()) {
            const char n = t[i + 1];
            if (n == 'g' && i + 2 < t.size() && t[i + 2] == '<') {
                const std::size_t close = t.find('>', i + 3);
                out += "${" + t.substr(i + 3, close - i - 3) + "}";
                i = close;
            } else if (n >= '0' && n <= '9') {
                out += std::string("${") + n + "}";
                ++i;
            } else {
                out.push_back(n == '\\' ? '\\' : n);
                ++i;
            }
        } else {
            out.push_back(t[i]);
        }
    }
    return out;
}

// ${1} -> \g<1>, $$ -> $
std::string template_to_python(const std::string& t) {
    std::string out;
    for (std::size_t i = 0; i < t.size(); ++i) {
        if (t[i] == '$' && i + 1 < t.size() && t[i + 1] == '$') {
            out.push_back('$');
            ++i;
        } else if (t[i] == '$' && i + 1 < t.size() && t[i + 1] == '{') {
            const std::size_t close = t.find('}', i);
            out += "\\g<" + t.substr(i + 2, close - i - 2) + ">";
            i = close;
        } else {
            out.push_back(t[i]);
        }
    }
    return out;
}

newc::DeclRule rule_of(const DeclRule& r) {
    if (r.fn) return newc::DeclRule(U(r.pattern), newc::SplitDecl{U(r.fn->name), U(r.fn->typ)});
    return newc::DeclRule(U(r.pattern), template_from_python(U(r.repl)));
}

}  // namespace

// P4Native needs the Port's pieces: made on demand, kept in the Port
namespace {

P4Native& native_of(Port& port) {
    if (!port.p4_native_) {
        port.p4_native_ = std::make_shared<P4Native>();
        const newc::Profile profile = profile_of(port.profile);
        port.p4_native_->original = std::make_unique<newc::OriginalProgram>(files_of(port.originals), profile.old_pointer_bytes());
        port.p4_native_->plan = std::make_unique<newc::BssPlan>(*port.p4_native_->original, profile.runtime_internals);
    }
    P4Native& n = *port.p4_native_;
    // the BSS list is set from outside: the plan is told when it changes
    newc::BssSizes now;
    for (const auto& kv : port.bss.items()) now.insert_or_assign(U(std::get<0>(kv)), static_cast<std::uint64_t>(std::get<1>(kv)));
    if (!(now == n.sizes)) {
        n.sizes = now;
        n.plan->set_sizes(now);
    }
    return n;
}

newc::FinishRules rules_of(Port& port, const std::string& only_file) {
    P4Native& n = native_of(port);
    while (static_cast<int64_t>(n.rules.size()) < port.decl_rules.size()) n.rules.push_back(rule_of(port.decl_rules[static_cast<int64_t>(n.rules.size())]));
    newc::FinishRules r;
    for (const auto& kv : port.renames.items()) r.renames[U(std::get<0>(kv))] = U(std::get<1>(kv));
    r.decl_rules = n.rules;
    for (const auto& kv : port.local_retypes.items()) {
        const auto& key = std::get<0>(kv);
        r.local_retypes.push_back({U(std::get<0>(key)), U(std::get<1>(key)), U(std::get<2>(key)), U(std::get<1>(kv))});
    }
    for (const auto& kv : port.array_sizes.items()) r.array_sizes[U(std::get<0>(kv))] = static_cast<std::uint64_t>(std::get<1>(kv));
    for (const auto& kv : port.line_fixes.items()) {
        const auto& key = std::get<0>(kv);
        const std::string file = U(std::get<0>(key));
        if (!only_file.empty() && file != only_file) continue;
        const int64_t nth = std::get<2>(key);
        r.line_fixes[newc::LineFixKey{file, U(std::get<1>(key)), nth < 0 ? std::nullopt : std::optional<std::size_t>(static_cast<std::size_t>(nth))}] = U(std::get<1>(kv));
    }
    for (const NpRule& np : port.np_rules) {
        newc::NullRead::Kind kind = newc::NullRead::Kind::Plain;
        if (np.kind) {
            const std::string k = U(*np.kind);
            kind = k == "var" ? newc::NullRead::Kind::Var : k == "assign" ? newc::NullRead::Kind::Assign : k == "mem" ? newc::NullRead::Kind::Mem : kind;
        }
        r.null_reads.push_back({U(np.file), U(np.var), U(np.typ), kind});
    }
    return r;
}

void record_origin(Port& port, const std::string& file, const std::vector<newc::LineOrigin>& origin) {
    py::dict<int64_t, std::tuple<str, int64_t>> d;
    int64_t i = 0;
    for (const newc::LineOrigin& o : origin) d.set(++i, {P(o.text), static_cast<int64_t>(o.nth)});
    port.line_origin.set(P(file), d);
}

}  // namespace

// ---------------------------------------------------------------- constants

const py::dict<str, int64_t> SIZES = [] {
    py::dict<str, int64_t> d;
    for (const auto& e : newc::scalar_sizes()) d.set(P(e.first), e.second);
    return d;
}();

const py::dict<str, int64_t> OLD_SCALARS = [] {
    py::dict<str, int64_t> d;
    for (const auto& e : newc::old_scalar_sizes()) d.set(P(e.first), e.second);
    return d;
}();

const py::str CAST = P(newc::cast_pattern());

namespace {
const char* const kKinds[] = {"libname", "argtype", "fnptr", "intptr", "assign", "retptr", "retint", "retmix", "callptr",
                              "neg", "member", "void", "ptrcast", "incomplete", "unionset", "unionarg", "seqpt", "intcast", "arity"};
}

const py::list<std::tuple<str, list<str>>> SAYS = [] {
    py::list<std::tuple<str, list<str>>> out;
    for (const newc::ComplaintPatterns& c : newc::complaint_table()) {
        list<str> ps;
        for (const std::string_view p : c.patterns) ps.append(P(p));
        out.append({P(kKinds[static_cast<int>(c.kind)]), ps});
    }
    return out;
}();

// (the kind of a complaint, the match) - or (None, None)
std::tuple<std::optional<str>, std::optional<py::re::Match>> said(const str& text) {
    for (const auto& kv : SAYS) {
        for (const str& p : std::get<1>(kv)) {
            auto m = py::re::search(p, text);
            if (m) return {std::get<0>(kv), m};
        }
    }
    return {std::nullopt, std::nullopt};
}

// ---------------------------------------------------------------- the free functions

str split_view_lists(str text) { return P(cc::guard([&] { return newc::split_view_lists(U(text)); })); }

str union_views(str text, Notes& notes, const str& fname, const py::dict<str, std::tuple<int64_t, int64_t>>& sizes) {
    newc::SizeChanges changes;
    for (const auto& kv : sizes.items())
        changes[U(std::get<0>(kv))] = {static_cast<std::uint32_t>(std::get<0>(std::get<1>(kv))), static_cast<std::uint32_t>(std::get<1>(std::get<1>(kv)))};
    newc::Notes added;
    const std::string result = cc::guard([&] { return newc::union_views(U(text), added, U(fname), changes); });
    append_notes(notes, added);
    return P(result);
}

py::bytes c_string_bytes(const str& lit) {
    const ::cb::Bytes bytes = cc::guard([&] { return newc::c_string_bytes(U(lit)); });
    return cc::to_py(::cb::ByteView(bytes));
}

std::optional<int64_t> size_of(const str& base, const str& decl, const str& name, int64_t ptr, const asm16::Layouts& structs) {
    const newc::StructSizeFn size = [&structs](std::string_view tag) -> std::optional<std::uint64_t> {
        const asm16::Layout* l = structs.get(P(tag));
        if (l == nullptr) return std::nullopt;
        return static_cast<std::uint64_t>(l->size);
    };
    const auto r = newc::size_of(U(base), U(decl), U(name), static_cast<unsigned>(ptr), size);
    if (!r) return std::nullopt;
    return static_cast<int64_t>(*r);
}

py::dict<str, SimpleLayout> layouts(const list<str>& texts, int64_t ptr) {
    const newc::StructLayouts lay = newc::lay_out_structs(cc::from_py(texts), static_cast<unsigned>(ptr), static_cast<unsigned>(ptr));
    py::dict<str, SimpleLayout> out;
    for (const auto& kv : lay) {
        SimpleLayout s;
        for (const newc::Member& m : kv.second.members) s.members.set(P(m.name), {static_cast<int64_t>(m.offset), static_cast<int64_t>(m.size)});
        s.size = kv.second.size;
        out.set(P(kv.first), s);
    }
    return out;
}

void frames(Port& port, py::dict<str, str>& out) {
    newc::Files files = files_of(out);
    const OrderedFns fns = fns_of(port.fns, true);
    newc::Notes added;
    cc::guard([&] {
        newc::frames(files, fns, port.profile.widths_pointer ? static_cast<unsigned>(*port.profile.widths_pointer / 8) : 0, {}, added);
        return 0;
    });
    for (const auto& kv : files) out.set(P(kv.first), P(kv.second));
    append_notes(port.notes, added);
}

list<list<Span>> _calls(const str& body, const str& g) {
    const std::string b = U(body);
    list<list<Span>> out;
    for (const auto& spans : newc::calls_of(b, U(g))) {
        list<Span> l;
        for (const newc::Span& s : spans) l.append({static_cast<int64_t>(cp_of_byte(b, s.begin)), static_cast<int64_t>(cp_of_byte(b, s.end))});
        out.append(l);
    }
    return out;
}

list<DefinitionAny> definitions_any(const list<str>& lines, const list<str>& code) {
    list<DefinitionAny> out;
    for (const auto& d : newc::definitions_any(cc::from_py(lines), cc::from_py(code)))
        out.append({P(d.name), static_cast<int64_t>(d.first), static_cast<int64_t>(d.open), static_cast<int64_t>(d.last)});
    return out;
}

py::dict<str, LocalDecl> _locals(const list<str>& lines, const list<str>& code, int64_t j, int64_t k, int64_t ptr) {
    (void)lines;
    py::dict<str, LocalDecl> out;
    for (const auto& kv : newc::local_decls(cc::from_py(code), static_cast<std::size_t>(j), static_cast<std::size_t>(k), static_cast<unsigned>(ptr))) {
        LocalDecl d;
        d.base = P(kv.second.base);
        d.decl = P(kv.second.declarator);
        if (kv.second.size) d.size = static_cast<int64_t>(*kv.second.size);
        d.line = static_cast<int64_t>(kv.second.line);
        d.reg = kv.second.is_register;
        out.set(P(kv.first), d);
    }
    return out;
}

py::dict<str, list<str>> members(const list<str>& texts) {
    py::dict<str, list<str>> out;
    for (const auto& kv : newc::struct_members(cc::from_py(texts))) out.set(P(kv.first), cc::to_py(kv.second));
    return out;
}

int64_t object_before(const str& line, int64_t at) {
    const std::string l = U(line);
    const std::size_t byte = is_ascii(l) ? static_cast<std::size_t>(at) : byte_of_cp(l, static_cast<std::size_t>(at));
    const auto r = newc::object_before(l, byte);
    if (!r) return -1;
    return static_cast<int64_t>(is_ascii(l) ? *r : cp_of_byte(l, *r));
}

str abstract(const str& decl, const str& name) { return P(newc::abstract_type(U(decl), U(name))); }

std::optional<list<Span>> call_args(const str& line, const str& fn, bool partial) {
    const std::string l = U(line);
    const auto r = newc::call_args(l, U(fn), partial);
    if (!r) return std::nullopt;
    list<Span> out;
    for (const newc::Span& s : *r) out.append({static_cast<int64_t>(cp_of_byte(l, s.begin)), static_cast<int64_t>(cp_of_byte(l, s.end))});
    return out;
}

bool truthy(const str& body, const str& name) { return newc::only_tested(U(body), U(name)); }

// ---------------------------------------------------------------- the Port

void Port::bss_views() {
    P4Native& n = native_of(*this);
    newc::Aliases al = aliases_of(aliases);
    std::set<std::string> vh = cc::from_py(view_headers);
    newc::Notes added;
    cc::guard([&] {
        n.plan->bss_views(files_of(files), fns_of(fns, false), al, vh, added);
        return 0;
    });
    put_aliases(aliases, al);
    for (const std::string& h : vh) view_headers.add(P(h));
    append_notes(notes, added);
}

const py::dict<str, py::set<int64_t>>& Port::allocators() {
    if (!_allocs) {
        P4Native& n = native_of(*this);
        py::dict<str, py::set<int64_t>> out;
        for (const auto& kv : cc::guard([&]() -> const newc::Allocators& { return n.original->allocators(files_of(files)); })) {
            py::set<int64_t> places;
            for (const std::size_t p : kv.second) places.add(static_cast<int64_t>(p));
            out.set(P(kv.first), places);
        }
        _allocs = out;
    }
    return *_allocs;
}

py::dict<str, std::tuple<str, str>> Port::bss_decls() {
    P4Native& n = native_of(*this);
    std::set<std::string> names;
    for (const auto& kv : n.sizes) names.insert(kv.first);
    py::dict<str, std::tuple<str, str>> out;
    for (const auto& kv : n.original->extern_decls(names)) out.set(P(kv.first), {P(kv.second.first), P(kv.second.second)});
    return out;
}

bool Port::join_bss(const str& var) {
    P4Native& n = native_of(*this);
    newc::Aliases al = aliases_of(aliases);
    std::vector<newc::DeclRule> added;
    const bool joined_now = cc::guard([&] { return n.plan->join_bss(U(var), al, added); });
    put_aliases(aliases, al);
    for (const newc::DeclRule& r : added) decl_rules.append(DeclRule(P(r.pattern()), P(template_to_python(r.replacement()))));
    return joined_now;
}

list<str> Port::bss_definitions() {
    P4Native& n = native_of(*this);
    newc::Aliases al = aliases_of(aliases);
    const std::vector<std::string> out = cc::guard([&] { return n.plan->definitions(al); });
    put_aliases(aliases, al);
    return cc::to_py(out);
}

const py::dict<str, int64_t>& Port::old_elem_sizes() {
    if (!_old_elems) {
        P4Native& n = native_of(*this);
        py::dict<str, int64_t> out;
        for (const auto& kv : n.original->elem_sizes()) out.set(P(kv.first), static_cast<int64_t>(kv.second));
        _old_elems = out;
    }
    return *_old_elems;
}

py::dict<str, int64_t> Port::old_var_sizes() {
    P4Native& n = native_of(*this);
    ::cb::OrderedMap<std::string, std::uint64_t> asm_sizes_now;
    for (const auto& kv : asm_sizes.items()) asm_sizes_now.insert_or_assign(U(std::get<0>(kv)), static_cast<std::uint64_t>(std::get<1>(kv)));
    py::dict<str, int64_t> out;
    for (const auto& kv : n.original->var_sizes(asm_sizes_now)) out.set(P(kv.first), static_cast<int64_t>(kv.second));
    return out;
}

const Layouts& Port::old_layouts() {
    if (!_old) {
        list<str> texts;
        for (const auto& kv : originals.items()) texts.append(std::get<1>(kv));
        _old = asm16::CTypes::_layouts(texts, (profile.widths_pointer ? *profile.widths_pointer : 16) / 8, 4);
    }
    return *_old;
}

const Layouts& Port::old_layouts_for(const str& fname) {
    P4Native& n = native_of(*this);
    const newc::OriginalProgram::LayoutInput in = n.original->layout_input(U(fname));
    if (in.order.empty()) return old_layouts();
    const str key = P(joined(in.order));
    if (!_lay_cache.contains(key)) {
        _lay_cache.set(key, asm16::CTypes::_layouts(cc::to_py(in.texts), (profile.widths_pointer ? *profile.widths_pointer : 16) / 8, 4));
    }
    return _lay_cache.at(key);
}

str Port::finish(const str& fname, str text) {
    const std::string file = U(fname);
    const newc::FinishRules r = rules_of(*this, file);
    const newc::Finished f = cc::guard([&] { return newc::finish(r, file, U(text)); });
    record_origin(*this, file, f.origin);
    return P(f.text);
}

str Port::_preprocessor(const str& text, const str& fname) {
    std::set<std::string, std::less<>> out_of_tree;
    for (const str& k : left_out.keys()) out_of_tree.insert(U(k));
    newc::Notes added;
    const std::string result = cc::guard([&] { return newc::preprocess(U(text), U(fname), profile_of(profile), out_of_tree, added); });
    append_notes(notes, added);
    return P(result);
}

str Port::_definitions(const str& text, const str& fname) {
    OrderedFns nfns = fns_of(fns, false);
    newc::ParamTypes types;
    for (const auto& kv : param_types.items()) types[{U(std::get<0>(std::get<0>(kv))), static_cast<std::size_t>(std::get<1>(std::get<0>(kv)))}] = U(std::get<1>(kv));
    newc::Notes added;
    const newc::RewrittenFile r = cc::guard([&] { return newc::rewrite_definitions(U(text), U(fname), nfns, types, added); });
    for (const std::string& name : r.functions) {
        const str key = P(name);
        if (!fns.contains(key)) fns.set(key, std::make_shared<Fn>());
        Fn& f = *fns.at(key);
        f.name = key;
        f.line = static_cast<int64_t>(nfns.at(name).line);
        update_fn(f, nfns.at(name));
    }
    append_notes(notes, added);
    return P(r.text);
}

py::dict<str, std::optional<str>> Port::headers() {
    P4Native& n = native_of(*this);
    const OrderedFns nfns = fns_of(fns, false);
    const newc::Files nfiles = files_of(files);
    std::vector<std::pair<std::string, std::string>> data_defs, asm_defs;
    for (const auto& d : data) data_defs.emplace_back(U(std::get<0>(d)), U(std::get<1>(d)));
    for (const auto& d : asm_data) asm_defs.emplace_back(U(std::get<0>(d)), U(std::get<1>(d)));
    std::map<std::string, std::string> protos;
    for (const auto& kv : asm_protos.items()) protos[U(std::get<0>(kv))] = U(std::get<1>(kv));
    const newc::Profile prof = profile_of(profile);
    const std::set<std::string> vh = cc::from_py(view_headers);
    newc::Aliases al = aliases_of(aliases);
    const newc::FinishRules r = rules_of(*this, "");
    const newc::WidthsFn widths_fn = [](std::string_view text, std::string_view file, bool i16) {
        const str t = P(text);
        Notes scratch;
        return U(std::get<0>(widths(t, ::codebreaker::base::ctext::blank(t), scratch, P(file), i16)));
    };
    const newc::HeaderInputs in{nfns, nfiles, data_defs, asm_defs, protos, prof, vh, int16};
    const newc::Headers h = cc::guard([&] { return newc::make_headers(in, al, *n.plan, r, widths_fn); });
    put_aliases(aliases, al);
    py::dict<str, std::optional<str>> out;
    for (const auto& kv : h.files) {
        if (kv.second) out.set(P(kv.first), P(*kv.second)); else out.set(P(kv.first), std::nullopt);
    }
    for (const auto& kv : h.origins) record_origin(*this, kv.first, kv.second);
    return out;
}

bool Port::_same_def(const str& tag, const str& h1, const str& h2) {
    return newc::same_definition(files_of(files), U(tag), U(h1), U(h2));
}

}  // namespace codebreaker::port::pre_ansi
