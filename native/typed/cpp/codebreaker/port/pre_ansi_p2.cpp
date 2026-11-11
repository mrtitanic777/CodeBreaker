// codebreaker/port/pre_ansi_p2.cpp -- SHIM over native/cb/port (docs/CPP_DESIGN.md 6.3): the old types in, the new
// functions called, the old types out. COMPAT(py): deleted with the last py:: consumer of PART 2 of pre_ansi (the Port
// class of pre_ansi.hpp: the pre_ansi, asm16 and sanitize modules, once refactored, call cb::port directly).
//
// The module this shim stands for is PART 2 of codebreaker.port.pre_ansi (Python lines 886-2145): the Port methods
// agree_globals .. _globals. The passes are in native/cb/port/old_c_*.cpp (old_c_data.hpp). What is here besides
// conversion: the once-only guards the Python kept in the Port (`_agreed` ...), and `modernize`'s order of work, which
// calls the parts of the port that are not refactored yet (PART 1, 3 and 5 are still over py::) between the new steps.

#include <algorithm>
#include <functional>

#include "cb/compat/py_bridge.hpp"
#include "cb/port/old_c_data.hpp"
#include "codebreaker/base/ctext.hpp"
#include "codebreaker/port/pre_ansi.hpp"

namespace codebreaker::port::pre_ansi {

namespace newc = ::cb::port::pre_ansi;
namespace cc = ::cb::compat;
using base::ctext::blank;
using base::ctext::sub_code;
using py::dict;
using py::list;
using py::set;
using py::str;

// the new code's facts about the original program, kept with the Port (its allocators are worked out once, as the
// Python kept them in `_allocs`)
struct P2Native {
    std::unique_ptr<newc::OriginalProgram> original;
};

namespace {

str S(const char* s) { return str(s); }
std::string U(const str& s) { return cc::from_py(s); }
str P(std::string_view s) { return cc::to_py(s); }

// ---- the old types in and out -------------------------------------------------------------------------------

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

list<str> list_of(const std::vector<std::string>& v) {
    list<str> out;
    for (const std::string& s : v) out.append(P(s));
    return out;
}

void append_list(list<str>& l, const std::vector<std::string>& v) {
    for (const std::string& s : v) l.append(P(s));
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

// \g<1>, \1 -> ${1}; a literal '$' is '$$'
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
    if (f.file) n.file = U(*f.file);
    return n;
}

::cb::OrderedMap<std::string, newc::Fn> fns_of(const dict<str, FnP>& d) {
    ::cb::OrderedMap<std::string, newc::Fn> out;
    for (const auto& kv : d.items()) out.insert_or_assign(U(std::get<0>(kv)), fn_of(*std::get<1>(kv)));
    return out;
}

// ---- the state of the passes -------------------------------------------------------------------------------

newc::OriginalProgram& original_of(Port& port) {
    if (!port.p2_native_) {
        port.p2_native_ = std::make_shared<P2Native>();
        port.p2_native_->original = std::make_unique<newc::OriginalProgram>(
            files_of(port.originals), static_cast<unsigned>((port.profile.widths_pointer ? *port.profile.widths_pointer : 16) / 8));
    }
    return *port.p2_native_->original;
}

// The old Port's facts the passes read, as the new code has them.
newc::ProgramState state_of(Port& port) {
    newc::ProgramState st(original_of(port));
    st.files = files_of(port.files);
    st.originals = files_of(port.originals);
    st.notes = notes_of(port.notes);
    st.pointer_bytes = static_cast<unsigned>((port.profile.widths_pointer ? *port.profile.widths_pointer : 16) / 8);
    for (const auto& [f, decls] : port.extra_decls.items())
        for (const str& d : decls) st.extra_decls[U(f)].insert(U(d));
    st.aliases = aliases_of(port.aliases);
    for (const auto& kv : port.ds.items()) st.ds.insert_or_assign(U(std::get<0>(kv)), static_cast<std::uint64_t>(std::get<1>(kv)));
    for (const auto& kv : port.bss.items()) st.bss[U(std::get<0>(kv))] = static_cast<std::uint64_t>(std::get<1>(kv));
    for (const auto& kv : port.reach.items()) st.reach[U(std::get<0>(kv))] = static_cast<std::uint64_t>(std::get<1>(kv));
    return st;
}

// What a pass left, in the old Port: files, notes, extra declarations, aliases, BSS sizes, new declaration rules.
void put_state(Port& port, const newc::ProgramState& st, std::size_t rules_before) {
    put_files(port.files, st.files);
    put_notes(port.notes, st.notes);
    port.extra_decls = dict<str, set<str>>();
    for (const auto& [f, decls] : st.extra_decls) {
        set<str> s;
        for (const std::string& d : decls) s.add(P(d));
        port.extra_decls.set(P(f), s);
    }
    put_aliases(port.aliases, st.aliases);
    for (const auto& kv : st.bss) port.bss.set(P(kv.first), static_cast<int64_t>(kv.second));
    for (std::size_t i = rules_before; i < st.decl_rules.size(); ++i)
        port.decl_rules.append(DeclRule(P(st.decl_rules[i].pattern()), P(template_to_python(st.decl_rules[i].replacement()))));
}

// The allocators are worked out once, at the first call, as the Python kept them in `_allocs`: the old callers'
// `allocators()` (PART 4's shim) answer from the same place. size_tables and shared_blocks ask for them first.
void keep_allocators(Port& port) {
    if (port._allocs) return;
    dict<str, set<int64_t>> out;
    for (const auto& kv : original_of(port).allocators(files_of(port.files))) {   // (worked out already: the answer is kept)
        set<int64_t> places;
        for (const std::size_t p : kv.second) places.add(static_cast<int64_t>(p));
        out.set(P(kv.first), places);
    }
    port._allocs = out;
}

}  // namespace

// ================================================================ the passes
void Port::agree_globals() {
    if (_agreed) return;
    _agreed = true;
    newc::ProgramState st = state_of(*this);
    cc::guard([&] { newc::agree_globals(st); });
    put_state(*this, st, 0);
    agree_notes = list_of(st.lists.agreed);
    append_list(long_notes, st.lists.longs);
}

bool Port::joins_next(const str& var) { return newc::joins_next(files_of(files), U(var)); }

void Port::fixed_allocators() {
    if (_fixed) return;
    _fixed = true;
    newc::ProgramState st = state_of(*this);
    cc::guard([&] { newc::fixed_allocators(st); });
    put_state(*this, st, 0);
    append_list(alloc_notes, st.lists.allocators);
}

void Port::array_bounds() {
    if (_bounds_done) return;
    _bounds_done = true;
    newc::ProgramState st = state_of(*this);
    cc::guard([&] { newc::array_bounds(st); });
    put_state(*this, st, 0);
}

void Port::word_stacks() {
    if (_stacks_done) return;
    _stacks_done = true;
    newc::ProgramState st = state_of(*this);
    cc::guard([&] { newc::word_stacks(st); });
    put_state(*this, st, 0);
}

void Port::merge_tables() {
    if (_merged == reach.size() + 1) return;   // (and where a run read one past its end: sanitize)
    _merged = reach.size() + 1;
    newc::ProgramState st = state_of(*this);
    cc::guard([&] { newc::merge_tables(st); });
    put_state(*this, st, 0);
    append_list(table_notes, st.lists.tables);
}

void Port::numbered_tables() {
    if (_numbered_done) return;
    _numbered_done = true;
    newc::ProgramState st = state_of(*this);
    cc::guard([&] { newc::numbered_tables(st); });
    put_state(*this, st, 0);
}

void Port::size_tables() {
    if (_size_tables_done) return;
    _size_tables_done = true;
    newc::ProgramState st = state_of(*this);
    cc::guard([&] { newc::size_tables(st); });
    put_state(*this, st, 0);
    append_list(notes_size_tables, st.lists.size_tables);
    keep_allocators(*this);
}

void Port::pool_blocks() {
    if (_pools_done) return;
    _pools_done = true;
    newc::ProgramState st = state_of(*this);
    cc::guard([&] { newc::pool_blocks(st); });
    put_state(*this, st, 0);
    append_list(notes_pools, st.lists.pools);
}

void Port::local_copies() {
    if (_local_copies_done) return;
    _local_copies_done = true;
    newc::ProgramState st = state_of(*this);
    cc::guard([&] { newc::local_copies(st); });
    put_state(*this, st, 0);
    append_list(notes_local_copies, st.lists.local_copies);
}

void Port::byte_places() {
    if (_byte_places_done) return;
    _byte_places_done = true;
    newc::ProgramState st = state_of(*this);
    cc::guard([&] { newc::byte_places(st); });
    put_state(*this, st, 0);
    append_list(notes_byte_places, st.lists.byte_places);
}

void Port::shared_blocks() {
    if (_shared_blocks_done) return;
    _shared_blocks_done = true;
    newc::ProgramState st = state_of(*this);
    cc::guard([&] { newc::shared_blocks(st); });
    put_state(*this, st, 0);
    append_list(notes_shared, st.lists.shared_blocks);
    keep_allocators(*this);
}

void Port::word_copies() {
    if (_word_copies_done) return;
    _word_copies_done = true;
    newc::ProgramState st = state_of(*this);
    cc::guard([&] { newc::first_word_copies(st); });
    put_state(*this, st, 0);
    append_list(notes_word_copies, st.lists.word_copies);
}

// ================================================================ modernize
// (the order of work; the new steps are in native/cb/port, the rest is the parts of the port not refactored yet)
py::dict<py::str, py::str> Port::modernize() {
    // (the rules of the first round, each by its name)
    if (rule(S("word_copies"))) word_copies();
    if (rule(S("shared_blocks"))) shared_blocks();
    if (rule(S("byte_places"))) byte_places();
    if (rule(S("local_copies"))) local_copies();
    if (rule(S("pool_blocks"))) pool_blocks();
    if (rule(S("size_tables"))) size_tables();
    if (rule(S("numbered_tables"))) numbered_tables();
    array_bounds();
    word_stacks();
    merge_tables();
    fixed_allocators();
    agree_globals();

    // the report's notes: those the passes and the other parts left, in the order the report lists them
    {
        newc::PassNotes lists;
        lists.agreed = cc::from_py(agree_notes);
        lists.longs = cc::from_py(long_notes);
        lists.allocators = cc::from_py(alloc_notes);
        lists.tables = cc::from_py(table_notes);
        lists.word_copies = cc::from_py(notes_word_copies);
        lists.shared_blocks = cc::from_py(notes_shared);
        lists.byte_places = cc::from_py(notes_byte_places);
        lists.local_copies = cc::from_py(notes_local_copies);
        lists.pools = cc::from_py(notes_pools);
        lists.size_tables = cc::from_py(notes_size_tables);
        const newc::Notes mended_n = notes_of(mended), unions = notes_of(union_notes);
        const std::vector<std::string> view_wide = cc::from_py(notes_view_wide), puns = cc::from_py(view_puns_refused),
                                       fixes = cc::from_py(member_fixes);
        ::cb::OrderedMap<std::string, std::size_t> dropped;
        for (const auto& kv : notes_init.items()) dropped.insert_or_assign(U(std::get<0>(kv)), static_cast<std::size_t>(std::get<1>(kv)));
        put_notes(notes, newc::gather_notes({mended_n, lists, view_wide, puns, unions, fixes, dropped}));
    }
    data = list<std::tuple<str, str>>();
    set<str> known_lib = set<str>::from(profile.libc) | set<str>::from(profile.platform_calls);
    for (const auto& n : py::sorted(files.keys())) {   // (the definitions first: every name the program has)
        if (n.endswith(S(".c"))) {
            str t = files.at(n);
            for (const auto& d : definitions(py::split(t, S("\n")), py::split(blank(t), S("\n")))) {
                FnP& fp = fns.setdefault(std::get<0>(d)->name, std::get<0>(d));
                fp->file = n;
            }
        }
    }
    bss_views();
    pass_through_params();
    set<str> known = known_lib | set<str>::from(fns.keys()) | set<str>::from(asm_protos.keys());
    {   // (pascal names were made capitals for the linker: a call in capitals and a definition in small letters were one function)
        std::vector<std::string> names;
        for (const str& f : fns.keys()) names.push_back(U(f));
        for (const str& f : asm_protos.keys()) names.push_back(U(f));
        std::set<std::string, std::less<>> known_std;
        for (const str& k : known) known_std.insert(U(k));
        std::map<std::string, std::string> renames_now;
        for (const auto& kv : renames.items()) renames_now[U(std::get<0>(kv))] = U(std::get<1>(kv));
        newc::Notes n = notes_of(notes);
        cc::guard([&] { newc::rename_pascal_calls(files_of(files), names, known_std, renames_now, n); });
        put_notes(notes, n);
        for (const auto& kv : renames_now) renames.set(P(kv.first), P(kv.second));
    }
    {   // (the callers' declarations say what a function gives, where its definition left it old C's int)
        ::cb::OrderedMap<std::string, newc::Fn> nfns = fns_of(fns);
        newc::Notes n = notes_of(notes);
        cc::guard([&] { newc::declared_return_types(files_of(files), nfns, n); });
        put_notes(notes, n);
        for (const auto& kv : nfns) {
            FnP f = fns.at(P(kv.first));
            if (kv.second.ret_set && !f->ret_set) {
                f->ret = P(kv.second.ret);
                f->ret_set = true;
            }
        }
    }
    dict<str, str> outd;
    for (const auto& n : py::sorted(files.keys())) {
        str t = files.at(n);
        t = _preprocessor(t, n);
        t = sub_code(t, py::re::compile(S(R"PY((?m)^([ \t]*(?:register|auto|static)\s+)(?=\**\s*\w+\s*(?:\[[^\]]*\]\s*)?(?:[,;=]))(?!(?:unsigned|signed|int|char|short|long|float|double|void|const|volatile|register|static)\b))PY")),
                     [](const py::re::Match& m) { return m.group(1) + S("int "); });   // (register i; - old C's int, unsaid)
        str code = blank(t);
        set<str> macro_names;
        for (const auto& m : py::re::finditer(S(R"PY((?m)^\s*#\s*define\s+(\w+)\s+(\w+)\s*$)PY"), t))
            if (known.contains(m.group(2))) macro_names.add(m.group(1));   // (#define yylex f049ab: f049ab's)
        list<str> plat = profile.platform_calls.copy();
        plat.extend(aliases.keys());
        for (const auto& [w, d] : aliases.values()) plat.append(py::re::search(S(R"PY([A-Za-z_]\w*)PY"), w)->group());
        t = old_declarations(t, code, known | macro_names, notes, n, plat);
        if (n.endswith(S(".h"))) {
            auto [t2, defs] = header_data(t, blank(t), n, notes);
            t = t2;
            for (const auto& d : defs) data.append({n, d});
        }
        outd.set(n, t);
    }
    std::map<std::string, std::string> tags;   // (a structure: the header that defines it, for the passes that come after)
    {   // (the headers the files need for the structures they use; the declarations a mend made them need)
        newc::Files out = files_of(outd);
        std::map<std::string, std::vector<std::string>> need;
        for (const auto& kv : need_headers.items()) need[U(std::get<0>(kv))] = cc::from_py(std::get<1>(kv));
        newc::ExtraDecls extra;
        for (const auto& [f, decls] : extra_decls.items())
            for (const str& d : decls) extra[U(f)].insert(U(d));
        newc::Notes n = notes_of(notes);
        tags = cc::guard([&] { return newc::add_missing_includes(out, need, extra, n); });
        put_notes(notes, n);
        put_files(outd, out);
    }
    for (const auto& [n, t] : outd.items())
        if (n.endswith(S(".c"))) outd.set(n, _definitions(t, n));
    for (const auto& [n, t0] : outd.items()) {
        str t = t0;
        str code = blank(t);
        std::tie(t, code) = widths(t, code, notes, n, int16);
        t = word_unions(t, notes, n, old_records);
        t = pre_ansi::word_copies(t, notes, n, copied_members());
        t = stdio_inits(t, notes, n);
        t = file_internals(t, notes, n);
        t = whole_word_stores(t, notes, n);
        t = offset_views(t, n);
        t = block_heads(t, notes, n, ptr2_names());
        t = bss_ends(t, n);
        t = formats(t, code, notes, n, int16, profile.printf_hex_upper);
        t = struct_sizes(t, old_layouts_for(n), notes, n, allocators());
        t = header_offsets(t, old_layouts_for(n), notes, n);
        t = mem_sizes(t, old_var_sizes(), old_elem_sizes(), notes, n);
        outd.set(n, t);
    }
    {   // (the program's allocators: the old sizes they keep their blocks by, the structures' sizes now)
        newc::Files out = files_of(outd);
        newc::AllocatorSizes sizes;
        for (const auto& [fn_, pools] : POOLS.items()) {
            auto& set_ = sizes[U(fn_)];
            for (const auto& [st_, old_n] : pools) set_.insert({U(st_), static_cast<std::uint64_t>(old_n)});
        }
        newc::Notes n = notes_of(notes);
        cc::guard([&] { newc::allocator_block_sizes(out, fns_of(fns), sizes, tags, n); });
        put_notes(notes, n);
        put_files(outd, out);
    }
    frames(*this, outd);
    loose_calls(outd);
    {   // (each array the program defines: its elements)
        newc::Files texts = files_of(outd);
        texts.insert_or_assign("bss.c", U(py::join(S("\n"), bss_definitions())));
        std::vector<newc::DeclRule> rules;
        for (const DeclRule& r : decl_rules) rules.push_back(rule_of(r));
        array_sizes = dict<str, int64_t>();
        for (const auto& kv : cc::guard([&] { return newc::array_sizes_of(texts, rules); })) array_sizes.set(P(kv.first), static_cast<int64_t>(kv.second));
    }
    {   // (every header of the program's: once - port.h and data.c include some of them too)
        newc::Files out = files_of(outd);
        newc::guard_headers(out);
        put_files(outd, out);
    }
    for (const auto& [n, t] : outd.items()) outd.set(n, finish(n, t));
    list<str> bss_ = bss_definitions();
    agree_wide(outd, bss_, notes);
    if (rule(S("numbered_places"))) numbered_places(outd, bss_, notes);
    if (rule(S("old_c_ways"))) old_c_ways(outd, notes);
    out = outd;
    return outd;
}

// ================================================================ loose_calls
void Port::loose_calls(py::dict<py::str, py::str>& out_) {
    newc::Files texts = files_of(out_);
    std::map<std::string, std::string> renames_now;
    for (const auto& kv : renames.items()) renames_now[U(std::get<0>(kv))] = U(std::get<1>(kv));
    newc::Notes n = notes_of(notes);
    cc::guard([&] { newc::loose_calls(texts, fns_of(fns), renames_now, n); });
    put_notes(notes, n);
    put_files(out_, texts);
}

// ================================================================ _globals
list<std::tuple<int64_t, str, str, str, std::optional<str>>> Port::_globals(const str& text) {
    list<std::tuple<int64_t, str, str, str, std::optional<str>>> out_;
    for (const newc::GlobalDefinition& g : cc::guard([&] { return newc::global_definitions(U(text)); }))
        out_.append({static_cast<int64_t>(g.line), P(g.name), P(g.base), P(g.shape),
                     g.initializer ? std::optional<str>(P(*g.initializer)) : std::nullopt});
    return out_;
}

}  // namespace codebreaker::port::pre_ansi
