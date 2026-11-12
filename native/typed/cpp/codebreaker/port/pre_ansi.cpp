// codebreaker/port/pre_ansi.cpp -- SHIM over native/cb/port (docs/CPP_DESIGN.md 6.3): the old types in, the new functions
// called, the old types out. COMPAT(py): deleted with the last py:: consumer of PART 1 and PART 3 of pre_ansi (the Port
// class of pre_ansi.hpp: port/__main__, port/sanitize, once refactored, call cb::port directly).
//
// pre_ansi.py -- C of before the standard made C a compiler of today takes, the meaning kept. The module is in
// native/cb/port now: the text passes (old_c_text.hpp: widths, formats, mem_sizes, header_offsets, struct_sizes,
// old_declarations, header_data), the first reading of a program (program_prep.hpp: Port.__init__) and the structure
// passes (views.hpp: anon_tags ... offset_views, the Port methods of PART 3). What is here is conversion: a py::str is a
// text of code points read as Latin-1 (one byte each: p5_shim::bytes_of / text_of), the Port's dicts and lists become the
// new records for a call and the changes are written back, and the structure passes' caches (the views, found once for the
// whole program) are kept in the Port's P3Native.

#include <algorithm>
#include <map>
#include <set>

#include "cb/compat/py_bridge.hpp"
#include "cb/port/old_c_text.hpp"
#include "cb/port/program_prep.hpp"
#include "cb/port/views.hpp"
#include "codebreaker/base/ctext.hpp"
#include "codebreaker/port/pre_ansi.hpp"

namespace codebreaker::port::pre_ansi {

namespace newc = ::cb::port::pre_ansi;
namespace cc = ::cb::compat;
using base::ctext::blank;
using py::dict;
using py::list;
using py::set;
using py::str;

// the new code's caches of the structure passes, kept with the Port (the views are found once for the program)
struct P3Native {
    newc::ViewCaches caches;
    std::optional<set<str>> copied, ptr2;   // (the members the program copies, the char ** names: for the callers that hold a reference)
};

// ({allocator: {(struct, old size)}}: what struct_sizes found; the Python's module-level POOLS, read by modernize)
dict<str, set<std::tuple<str, int64_t>>> POOLS;

namespace {

// ---- the old types in and out -------------------------------------------------------------------------------

std::string U(const str& s) { return p5_shim::bytes_of(s); }
str P(std::string_view s) { return p5_shim::text_of(s); }

newc::Files files_of(const dict<str, str>& d) {
    newc::Files out;
    for (const auto& kv : d.items()) out.insert_or_assign(U(std::get<0>(kv)), U(std::get<1>(kv)));
    return out;
}

void put_files(dict<str, str>& d, const newc::Files& files) {
    for (const auto& kv : files) {
        const str k = P(kv.first);
        if (!d.contains(k) || U(d.at(k)) != kv.second) d.set(k, P(kv.second));
    }
}

newc::Notes notes_of(const Notes& notes) {
    newc::Notes out;
    for (const auto& kv : notes.items())
        for (const str& v : std::get<1>(kv)) out[U(std::get<0>(kv))].push_back(U(v));
    return out;
}

void put_notes(Notes& notes, const newc::Notes& now) {
    notes = Notes();
    for (const auto& kv : now) {
        list<str> l;
        for (const std::string& v : kv.second) l.append(P(v));
        notes.set(P(kv.first), l);
    }
}

// the notes a call added: appended to the old Port's
void append_notes(Notes& notes, const newc::Notes& added) {
    for (const auto& kv : added) {
        list<str>& l = notes.setdefault(P(kv.first), list<str>());
        for (const std::string& v : kv.second) l.append(P(v));
    }
}

std::vector<std::string> strings_of(const list<str>& l) {
    std::vector<std::string> out;
    for (const str& s : l) out.push_back(U(s));
    return out;
}

list<str> list_of(const std::vector<std::string>& v) {
    list<str> out;
    for (const std::string& s : v) out.append(P(s));
    return out;
}

std::set<std::string> std_set_of(const set<str>& s) {
    std::set<std::string> out;
    for (const str& x : s) out.insert(U(x));
    return out;
}

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

Profile profile_from(const newc::Profile& n) {
    Profile p;
    p.name = P(n.name);
    p.libc = list_of(n.libc);
    p.runtime_defines = list_of(n.runtime_defines);
    p.runtime_headers = list_of(n.runtime_headers);
    p.runtime_internals = list_of(n.runtime_internals);
    p.platform_header = P(n.platform_header);
    p.platform_calls = list_of(n.platform_calls);
    for (const auto& kv : n.platform_renames) p.platform_renames.set(P(kv.first), P(kv.second));
    if (n.widths_pointer) p.widths_pointer = static_cast<int64_t>(*n.widths_pointer);
    if (n.int_storage) p.int_storage = static_cast<int64_t>(*n.int_storage);
    p.printf_hex_upper = n.printf_hex_upper;
    if (n.struct_align) p.struct_align = static_cast<int64_t>(*n.struct_align);
    if (n.keywords_removed) p.keywords_removed = list_of(*n.keywords_removed);
    p.arg_order = P(n.arg_order);
    return p;
}

newc::Fn fn_of(const Fn& f) {
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
    n.body = U(f.body);
    return n;
}

Fn fn_from(const newc::Fn& n) {
    Fn f;
    f.name = P(n.name);
    f.ret = P(n.ret);
    for (const newc::Param& p : n.params) f.params.append({P(p.name), P(p.type)});
    f.static_ = n.is_static;
    f.line = static_cast<int64_t>(n.line);
    f.void_ = n.returns_void;
    f.loose = n.loose;
    f.bare = static_cast<int64_t>(n.bare);
    f.varargs = n.varargs;
    f.va_slots = n.va_slots;
    f.ret_set = n.ret_set;
    f.int_ret = n.int_ret;
    f.keep_int = n.keep_int;
    for (const std::string& x : n.implicit) f.implicit.add(P(x));
    if (n.file) f.file = P(*n.file);
    f.body = P(n.body);
    return f;
}

newc::Aliases aliases_of(const dict<str, std::tuple<str, str>>& d) {
    newc::Aliases out;
    for (const auto& kv : d.items())
        out.insert_or_assign(U(std::get<0>(kv)), newc::Alias{U(std::get<0>(std::get<1>(kv))), U(std::get<1>(std::get<1>(kv)))});
    return out;
}

void put_aliases(dict<str, std::tuple<str, str>>& d, const newc::Aliases& a) {
    d.clear();
    for (const auto& kv : a) d.set(P(kv.first), {P(kv.second.where), P(kv.second.declaration)});
}

}  // namespace

// ================================================================ the module's free functions

bool rule(const str& name) { return newc::rule_enabled(U(name)); }

Profile Profile::load(const str& path) { return profile_from(cc::guard([&] { return newc::Profile::load(cc::from_py(path)); })); }

namespace {

std::optional<std::tuple<str, list<std::tuple<str, str>>>> declared(const std::optional<newc::Declaration>& r) {
    if (!r) return std::nullopt;
    list<std::tuple<str, str>> names;
    for (const newc::Declarator& d : r->names) names.append({P(d.name), P(d.text)});
    return std::make_tuple(P(r->base), names);
}

}  // namespace

std::optional<std::tuple<str, list<std::tuple<str, str>>>> declarators(const str& decl) {
    return declared(newc::declarators(U(decl)));
}

std::optional<std::tuple<str, list<std::tuple<str, str>>>> declarators(const str& decl, const set<str>& types) {
    return declared(newc::declarators(U(decl), types.contains(str("size_t"))));
}

list<Definition> definitions(const list<str>& lines, const list<str>& code) {
    list<Definition> out;
    const std::vector<std::string> l = strings_of(lines), c = strings_of(code);
    for (const newc::Definition& d : cc::guard([&] { return newc::definitions(l, c); }))
        out.append({std::make_shared<Fn>(fn_from(d.fn)), static_cast<int64_t>(d.first), static_cast<int64_t>(d.open), static_cast<int64_t>(d.last)});
    return out;
}

std::tuple<str, str> widths(str text, str code, Notes& notes, const str& fname, bool int16) {
    (void)code;
    newc::Notes added;
    const std::string r = cc::guard([&] { return newc::widths(U(text), added, U(fname), int16); });
    append_notes(notes, added);
    const str t = P(r);
    return {t, blank(t)};
}

str formats(str text, const str& code, Notes& notes, const str& fname, bool int16, bool hex_upper) {
    (void)code;
    newc::Notes added;
    const std::string r = cc::guard([&] { return newc::formats(U(text), added, U(fname), int16, hex_upper); });
    append_notes(notes, added);
    return P(r);
}

str split_decl::operator()(const py::re::Match& m) const {
    return P(newc::SplitDecl{U(name), U(typ)}(U(m.group(1)), U(m.group(2)), U(m.group(3)), U(m.group(4))));
}

str mem_sizes(str text, const dict<str, int64_t>& olds, const dict<str, int64_t>& elems, Notes& notes, const str& fname) {
    ::cb::OrderedMap<std::string, std::uint64_t> o, e;
    for (const auto& kv : olds.items()) o.insert_or_assign(U(std::get<0>(kv)), static_cast<std::uint64_t>(std::get<1>(kv)));
    for (const auto& kv : elems.items()) e.insert_or_assign(U(std::get<0>(kv)), static_cast<std::uint64_t>(std::get<1>(kv)));
    newc::Notes added;
    const std::string r = cc::guard([&] { return newc::mem_sizes(U(text), o, e, added, U(fname)); });
    append_notes(notes, added);
    return P(r);
}

str header_offsets(str text, const asm16::Layouts& old, Notes& notes, const str& fname) {
    newc::Notes added;
    const std::string r = cc::guard([&] { return newc::header_offsets(U(text), asm16::to_new(old), added, U(fname)); });
    append_notes(notes, added);
    return P(r);
}

str struct_sizes(str text, const asm16::Layouts& old, Notes& notes, const str& fname, const dict<str, set<int64_t>>& allocs) {
    newc::Allocators known;
    for (const auto& kv : allocs.items()) {
        std::set<std::size_t> places;
        for (const int64_t i : std::get<1>(kv)) places.insert(static_cast<std::size_t>(i));
        known.insert_or_assign(U(std::get<0>(kv)), std::move(places));
    }
    newc::Notes added;
    newc::AllocatorSizes pools;
    const std::string r = cc::guard([&] { return newc::struct_sizes(U(text), asm16::to_new(old), added, U(fname), known, pools); });
    append_notes(notes, added);
    for (const auto& kv : pools) {   // (the Python's module-level POOLS: the allocators the program's own, given a structure's whole size)
        set<std::tuple<str, int64_t>>& sizes = POOLS.setdefault(P(kv.first), set<std::tuple<str, int64_t>>());
        for (const auto& [tag, n] : kv.second) sizes.add({P(tag), static_cast<int64_t>(n)});
    }
    return P(r);
}

str old_declarations(const str& text, const str& code, const set<str>& known, Notes& notes, const str& fname, const list<str>& platform) {
    (void)code;
    std::set<std::string, std::less<>> names;
    for (const str& k : known) names.insert(U(k));
    newc::Notes added;
    const std::string r = cc::guard([&] { return newc::old_declarations(U(text), names, added, U(fname), strings_of(platform)); });
    append_notes(notes, added);
    return P(r);
}

std::tuple<str, list<str>> header_data(const str& text, const str& code, const str& fname, Notes& notes) {
    (void)code;
    newc::Notes added;
    const newc::HeaderData h = cc::guard([&] { return newc::header_data(U(text), U(fname), added); });
    append_notes(notes, added);
    return {P(h.text), list_of(h.definitions)};
}

// ================================================================ the program: Port.__init__

Port::Port(const dict<str, str>& files_in, const Profile& profile_, const dict<str, str>& members) {
    profile = profile_;
    ::cb::OrderedMap<std::string, std::string> corrections;
    for (const auto& kv : members.items()) corrections.insert_or_assign(U(std::get<0>(kv)), U(std::get<1>(kv)));
    const newc::PreparedProgram p = cc::guard([&] { return newc::prepare_program(files_of(files_in), profile_of(profile_), corrections); });
    for (const auto& kv : p.originals) originals.set(P(kv.first), P(kv.second));
    for (const auto& kv : p.files) files.set(P(kv.first), P(kv.second));
    for (const auto& kv : p.left_out) left_out.set(P(kv.first), P(kv.second));
    put_notes(notes, p.notes);
    put_notes(union_notes, p.union_notes);
    member_fixes = list_of(p.member_fixes);
    for (const auto& kv : p.far_near_dropped) notes_init.set(P(kv.first), static_cast<int64_t>(kv.second));
    for (const std::string& h : p.view_headers) view_headers.add(P(h));
    for (const std::string& v : p.view_vars) view_vars.add(P(v));
    put_aliases(aliases, p.aliases);
    for (const auto& kv : p.renames) renames.set(P(kv.first), P(kv.second));
    int16 = p.int16;
}

std::optional<str> Port::_runtime(const str& name, const str& text) {
    const auto why = newc::runtime_left_out(U(name), U(text), profile_of(profile));
    if (!why) return std::nullopt;
    return P(*why);
}

// ================================================================ PART 3: the structure passes

namespace {

// The Port's pieces a structure pass works on, as the new records; commit() writes back what the pass changed.
class P3Scope {
    Port& port_;   // (first: the caches are asked for while the rest is made)
    bool with_functions_ = false;

  public:
    P3Scope(Port& port, bool with_functions)
        : port_(port),
          files(files_of(port.files)),
          originals(files_of(port.originals)),
          notes(notes_of(port.notes)),
          mended(notes_of(port.mended)),
          aliases(aliases_of(port.aliases)),
          view_vars(std_set_of(port.view_vars)),
          view_headers(std_set_of(port.view_headers)),
          view_wide(strings_of(port.notes_view_wide)),
          view_puns_refused(strings_of(port.view_puns_refused)),
          cx{files, originals, notes, mended, aliases, view_vars, view_headers, view_wide, view_puns_refused, need_headers, fns, param_types, bss, caches()} {
        for (const auto& kv : port.need_headers.items()) {
            std::set<std::string>& s = need_headers[U(std::get<0>(kv))];
            for (const str& h : std::get<1>(kv)) s.insert(U(h));
        }
        for (const auto& kv : port.bss.items()) bss.insert_or_assign(U(std::get<0>(kv)), static_cast<std::uint64_t>(std::get<1>(kv)));
        if (with_functions) {
            for (const auto& kv : port.fns.items()) fns.insert_or_assign(U(std::get<0>(kv)), fn_of(*std::get<1>(kv)));
            for (const auto& kv : port.param_types.items()) {
                const auto& key = std::get<0>(kv);
                param_types[{U(std::get<0>(key)), static_cast<std::size_t>(std::get<1>(key))}] = U(std::get<1>(kv));
            }
        }
        with_functions_ = with_functions;
    }

    void commit() {
        put_files(port_.files, files);
        put_notes(port_.notes, notes);
        put_notes(port_.mended, mended);
        put_aliases(port_.aliases, aliases);
        for (const std::string& v : view_vars) port_.view_vars.add(P(v));
        for (const std::string& h : view_headers) port_.view_headers.add(P(h));
        port_.notes_view_wide = list_of(view_wide);
        port_.view_puns_refused = list_of(view_puns_refused);
        for (const auto& [file, headers] : need_headers) {
            list<str>& have = port_.need_headers.setdefault(P(file), list<str>());
            for (const std::string& h : headers)
                if (!have.contains(P(h))) have.append(P(h));
        }
        if (with_functions_)
            for (const auto& [key, type] : param_types) {
                const std::tuple<str, int64_t> k{P(key.first), static_cast<int64_t>(key.second)};
                if (!port_.param_types.contains(k) || U(port_.param_types.at(k)) != type) port_.param_types.set(k, P(type));
            }
    }

    newc::Files files, originals;
    newc::Notes notes, mended;
    newc::Aliases aliases;
    std::set<std::string> view_vars, view_headers;
    std::vector<std::string> view_wide, view_puns_refused;
    std::map<std::string, std::set<std::string>> need_headers;
    ::cb::OrderedMap<std::string, newc::Fn> fns;
    newc::ParamTypes param_types;
    newc::BssSizes bss;
    newc::ViewContext cx;

  private:
    newc::ViewCaches& caches() {
        if (!port_.p3_native_) port_.p3_native_ = std::make_shared<P3Native>();
        return port_.p3_native_->caches;
    }
};

template <class Pass>
void run_pass(Port& port, Pass pass, bool with_functions = false) {
    P3Scope scope(port, with_functions);
    cc::guard([&] {
        pass(scope.cx);
        return 0;
    });
    scope.commit();
}

}  // namespace

void Port::anon_tags() { run_pass(*this, [](newc::ViewContext& cx) { newc::anon_tags(cx); }); }
void Port::array_tails() { run_pass(*this, [](newc::ViewContext& cx) { newc::array_tails(cx); }); }
void Port::link_words() { run_pass(*this, [](newc::ViewContext& cx) { newc::link_words(cx); }); }
void Port::fnptr_returns() { run_pass(*this, [](newc::ViewContext& cx) { newc::fnptr_returns(cx); }); }
void Port::cast_twins() { run_pass(*this, [](newc::ViewContext& cx) { newc::cast_twins(cx); }); }
void Port::twin_structs() { run_pass(*this, [](newc::ViewContext& cx) { newc::twin_structs(cx); }); }
void Port::struct_views() { run_pass(*this, [](newc::ViewContext& cx) { newc::struct_views(cx); }); }
void Port::word_columns() { run_pass(*this, [](newc::ViewContext& cx) { newc::word_columns(cx); }); }

void Port::pass_through_params() { run_pass(*this, [](newc::ViewContext& cx) { newc::pass_through_params(cx); }, true); }

str Port::bss_ends(const str& t, const str& fname) {
    str out = t;
    run_pass(*this, [&](newc::ViewContext& cx) { out = P(newc::bss_ends(cx, U(t), U(fname))); });
    return out;
}

str Port::offset_views(str t, const str& fname) {
    // (the views are found once for the whole program; a later file needs only the text and the files' headers)
    const bool found = p3_native_ && p3_native_->caches.views.has_value();
    str out = t;
    run_pass(*this, [&](newc::ViewContext& cx) { out = P(newc::offset_views(cx, U(t), U(fname))); }, !found);
    return out;
}

const set<str>& Port::copied_members() {
    if (p3_native_ && p3_native_->copied) return *p3_native_->copied;
    run_pass(*this, [&](newc::ViewContext& cx) {
        const std::set<std::string>& names = newc::copied_members(cx);
        set<str> made;
        for (const std::string& n : names) made.add(P(n));
        p3_native_->copied = made;
    });
    return *p3_native_->copied;
}

const set<str>& Port::ptr2_names() {
    if (p3_native_ && p3_native_->ptr2) return *p3_native_->ptr2;
    run_pass(*this, [&](newc::ViewContext& cx) {
        const std::set<std::string>& names = newc::ptr2_names(cx);
        set<str> made;
        for (const std::string& n : names) made.add(P(n));
        p3_native_->ptr2 = made;
    });
    return *p3_native_->ptr2;
}

std::optional<str> Port::pointee_of(const str& text, const str& var) {
    const auto r = newc::pointee_of(U(text), U(var));
    if (!r) return std::nullopt;
    return P(*r);
}

}  // namespace codebreaker::port::pre_ansi
