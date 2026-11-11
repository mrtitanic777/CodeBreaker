// codebreaker/port/pre_ansi_p5.hpp -- SHIM over cb/port/{csource,old_c_passes,mend}.hpp (docs/CPP_DESIGN.md 6.3): the old
// types in, the new functions called, the old types out. Deleted with its last py:: consumer (the Port's other parts).
//
// pre_ansi.hpp declares these functions (its PART 5 section) and includes this file at its end: the functions are
// defined here, inline, and the old pre_ansi_p5.cpp is gone. The new code works on bytes; the port's texts are code
// points of a Latin-1 reading of the program's files, so a text crosses as one byte a character (a character past 255,
// which no Latin-1 text has, becomes '?') and an offset means the same on both sides.
//
// The Port's members that mending reads and writes (the functions, the files, the rules and notes) are copied into a
// cb::port::MendState, the new function runs, and what it changed is copied back: the functions' fields, the rules (a
// replacement's `$1` becomes the old layer's `\g<1>`), the notes, the line fixes.
#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include "cb/compat/py_bridge.hpp"
#include "cb/port/csource.hpp"
#include "cb/port/mend.hpp"
#include "cb/port/old_c_passes.hpp"

namespace codebreaker::port::pre_ansi {

namespace p5_shim {

// ---- text: one byte a code point ----
inline std::string bytes_of(const py::str& s) {
    std::string out;
    for (char32_t c : s.u32()) out += c < 0x100 ? static_cast<char>(c) : '?';
    return out;
}
inline py::str text_of(std::string_view s) {
    std::u32string out;
    for (char c : s) out += static_cast<char32_t>(static_cast<unsigned char>(c));
    return py::str(out);
}
inline cb::port::Names names_of(const py::set<py::str>& s) {
    cb::port::Names out;
    for (const py::str& x : s) out.insert(bytes_of(x));
    return out;
}
inline std::vector<std::string> strings_of(const py::list<py::str>& l) {
    std::vector<std::string> out;
    for (const py::str& x : l) out.push_back(bytes_of(x));
    return out;
}
inline cb::port::Sources sources_of(const py::dict<py::str, py::str>& d) {
    cb::port::Sources out;
    for (const auto& kv : d.items()) out[bytes_of(std::get<0>(kv))] = bytes_of(std::get<1>(kv));
    return out;
}
// the entries of `now` that differ from `d`, written to it
inline void write_back(const py::dict<py::str, py::str>& d, const cb::port::Sources& now) {
    for (const auto& [name, text] : now) {
        const py::str key = text_of(name);
        if (!d.contains(key) || bytes_of(d.at(key)) != text) d.set(key, text_of(text));
    }
}
inline void merge_notes(const py::dict<py::str, py::list<py::str>>& into, const cb::port::Notes& notes) {
    for (const auto& [why, places] : notes) {
        for (const std::string& place : places) into.setdefault(text_of(why), py::list<py::str>()).append(text_of(place));
    }
}

// ---- the Port's state ----
inline cb::port::VariableKey key_of(const py::str& old_key) {
    const std::string k = bytes_of(old_key);
    const std::size_t a = k.find('\x01');
    if (a == std::string::npos) return {"", "", k};
    const std::size_t b = k.find('\x01', a + 1);
    return {k.substr(0, a), k.substr(a + 1, b - a - 1), k.substr(b + 1)};
}
inline py::str old_key(const cb::port::VariableKey& k) {
    if (k.function.empty()) return text_of(k.variable);
    return text_of(k.file + "\x01" + k.function + "\x01" + k.variable);
}

struct Synced {
    cb::port::MendState state;
    std::map<std::string, std::string> files_before;
};

template <class PortT>
Synced state_of(const PortT& port) {
    Synced s;
    cb::port::MendState& st = s.state;
    for (const auto& kv : port.fns.items()) {
        const auto& f = *std::get<1>(kv);
        cb::port::Function g;
        g.name = bytes_of(f.name);
        g.ret = bytes_of(f.ret);
        for (const auto& p : f.params) g.params.push_back({bytes_of(std::get<0>(p)), bytes_of(std::get<1>(p))});
        g.is_static = f.static_;
        g.returns_void = f.void_;
        g.loose = f.loose;
        g.varargs = f.varargs;
        g.ret_set = f.ret_set;
        g.int_ret = f.int_ret;
        g.keep_int = f.keep_int;
        for (const py::str& n : f.implicit) g.implicit.insert(bytes_of(n));
        g.body = bytes_of(f.body);
        if (f.file) g.file = bytes_of(*f.file);
        st.functions[g.name] = std::move(g);
    }
    st.files = sources_of(port.files);
    for (const auto& kv : port.files.items()) s.files_before[bytes_of(std::get<0>(kv))] = bytes_of(std::get<1>(kv));
    for (const auto& kv : port.asm_protos.items()) st.asm_protos[bytes_of(std::get<0>(kv))] = bytes_of(std::get<1>(kv));
    for (const py::str& n : port.profile.libc) st.libc.insert(bytes_of(n));
    st.arg_order_left = port.profile.arg_order == py::str("left");
    st.int16 = port.int16;
    for (const auto& kv : port.renames.items()) st.renames[bytes_of(std::get<0>(kv))] = bytes_of(std::get<1>(kv));
    for (const auto& kv : port.retyped.items()) st.retyped[key_of(std::get<0>(kv))] = bytes_of(std::get<1>(kv));
    for (const auto& kv : port.line_origin.items()) {
        for (const auto& ln : std::get<1>(kv).items()) {
            const auto& origin = std::get<1>(ln);
            st.line_origin[bytes_of(std::get<0>(kv))][static_cast<std::size_t>(std::get<0>(ln))] = {bytes_of(std::get<0>(origin)),
                                                                                                    static_cast<long>(std::get<1>(origin))};
        }
    }
    for (const auto& kv : port.need_headers.items()) {
        for (const py::str& h : std::get<1>(kv)) st.need_headers[bytes_of(std::get<0>(kv))].insert(bytes_of(h));
    }
    for (const auto& kv : port.param_types.items()) {
        st.param_types[{bytes_of(std::get<0>(std::get<0>(kv))), static_cast<std::size_t>(std::get<1>(std::get<0>(kv)))}] =
            bytes_of(std::get<1>(kv));
    }
    for (const auto& g : port.given_params) st.given_params.insert({bytes_of(std::get<0>(g)), static_cast<std::size_t>(std::get<1>(g))});
    for (const auto& kv : port.fptr.items()) {
        for (const py::str& t : std::get<1>(kv)) st.fptr[bytes_of(std::get<0>(kv))].insert(bytes_of(t));
    }
    for (const py::str& n : port.fptr_done) st.fptr_done.insert(bytes_of(n));
    for (const py::str& n : port.fptr_mixed) st.fptr_mixed.insert(bytes_of(n));
    return s;
}

// `$1` of a new replacement template is `\g<1>` of the old layer's; `$$` a dollar
inline py::str old_template(const std::string& t) {
    std::string out;
    for (std::size_t i = 0; i < t.size(); ++i) {
        if (t[i] == '$' && i + 1 < t.size() && t[i + 1] == '$') {
            out += '$';
            ++i;
        } else if (t[i] == '$' && i + 1 < t.size() && std::isdigit(static_cast<unsigned char>(t[i + 1])) != 0) {
            out += "\\g<";
            out += t[++i];
            out += '>';
        } else {
            out += t[i];
        }
    }
    return text_of(out);
}

template <class PortT, class DeclRuleT, class SplitT>
void apply_to(PortT& port, const Synced& before, const cb::port::MendState& st, std::size_t rules_known) {
    for (const auto& [name, g] : st.functions) {
        const auto found = port.fns.get(text_of(name));   // (a shared Fn: null when absent)
        if (!found) continue;
        auto& f = *found;
        f.ret = text_of(g.ret);
        f.void_ = g.returns_void;
        f.loose = g.loose;
        f.keep_int = g.keep_int;
        f.int_ret = g.int_ret;
        f.ret_set = g.ret_set;
        py::list<std::tuple<py::str, py::str>> params;
        for (const cb::port::Param& p : g.params) params.append(std::make_tuple(text_of(p.name), text_of(p.type)));
        f.params = params;
    }
    for (const auto& [name, text] : st.files) {
        if (before.files_before.at(name) != text) port.files.set(text_of(name), text_of(text));
    }
    for (const auto& [k, v] : st.asm_protos) port.asm_protos.set(text_of(k), text_of(v));
    merge_notes(port.mended, st.mended);
    for (const auto& [k, v] : st.renames) port.renames.set(text_of(k), text_of(v));
    for (const auto& [k, v] : st.local_retypes) {
        port.local_retypes.set(std::make_tuple(text_of(k.file), text_of(k.function), text_of(k.variable)), text_of(v));
    }
    for (const auto& [k, v] : st.retyped) port.retyped.set(old_key(k), text_of(v));
    for (std::size_t i = rules_known; i < st.decl_rules.size(); ++i) {
        const cb::port::DeclRule& r = st.decl_rules[i];
        if (r.split) {
            port.decl_rules.append(DeclRuleT(text_of(r.pattern), SplitT(text_of(r.split->name), text_of(r.split->type))));
        } else {
            port.decl_rules.append(DeclRuleT(text_of(r.pattern), old_template(r.replacement)));
        }
    }
    for (const auto& [k, v] : st.line_fixes) {
        port.line_fixes.set(std::make_tuple(text_of(k.file), text_of(k.text), static_cast<int64_t>(k.n)), text_of(v));
    }
    for (const auto& [file, headers] : st.need_headers) {
        const auto list = port.need_headers.setdefault(text_of(file), py::list<py::str>());
        for (const std::string& h : headers) {
            bool known = false;
            for (const py::str& x : list) known = known || bytes_of(x) == h;
            if (!known) list.append(text_of(h));
        }
    }
    for (const auto& [k, v] : st.param_types) port.param_types.set(std::make_tuple(text_of(k.first), static_cast<int64_t>(k.second)), text_of(v));
    for (const auto& g : st.given_params) port.given_params.add(std::make_tuple(text_of(g.first), static_cast<int64_t>(g.second)));
    for (const auto& [k, types] : st.fptr) {
        py::set<py::str> now;
        for (const std::string& t : types) now.add(text_of(t));
        port.fptr.set(text_of(k), now);
    }
    for (const std::string& n : st.fptr_done) port.fptr_done.add(text_of(n));
    for (const std::string& n : st.fptr_mixed) port.fptr_mixed.add(text_of(n));
}

}  // namespace p5_shim

// ---- the free functions of PART 5 ----

inline py::str retyped_key(const py::str& file, const py::str& function, const py::str& variable) {
    return file + py::str("\x01") + function + py::str("\x01") + variable;
}

// an old-layer rule is applied by the old layer's regular expressions
inline py::str apply_decl_rule(const DeclRule& r, const py::str& text) {
    if (r.fn.has_value()) {
        const split_decl& f = *r.fn;
        return py::re::sub(r.pattern, [&](const py::re::Match& m) { return f(m); }, text);
    }
    return py::re::sub(r.pattern, r.repl, text);
}

inline std::optional<CallAt> call_at(const py::str& line, int64_t col) {
    const auto at = cb::port::csource::call_at(p5_shim::bytes_of(line), col < 0 ? 0 : static_cast<std::size_t>(col));
    if (!at) return std::nullopt;
    return CallAt{p5_shim::text_of(at->function), static_cast<int64_t>(at->index),
                  Span{static_cast<int64_t>(at->span.start), static_cast<int64_t>(at->span.end)}};
}

inline py::str ctype(const py::str& t, const py::set<py::str>& tags) {
    cb::port::csource::TagSet set;
    for (const py::str& x : tags) set.insert(p5_shim::bytes_of(x));
    return p5_shim::text_of(cb::port::csource::ctype(p5_shim::bytes_of(t), set));
}

inline py::str block_heads(py::str t, Notes& notes, const py::str& fname, const py::set<py::str>& ptr2) {
    cb::port::Notes made;
    const std::string out = cb::port::block_heads(p5_shim::bytes_of(t), made, p5_shim::bytes_of(fname), p5_shim::names_of(ptr2));
    p5_shim::merge_notes(notes, made);
    return p5_shim::text_of(out);
}
inline py::str whole_word_stores(const py::str& t, Notes& notes, const py::str& fname) {
    cb::port::Notes made;
    const std::string out = cb::port::whole_word_stores(p5_shim::bytes_of(t), made, p5_shim::bytes_of(fname));
    p5_shim::merge_notes(notes, made);
    return p5_shim::text_of(out);
}
inline py::str file_internals(py::str t, Notes& notes, const py::str& fname) {
    cb::port::Notes made;
    const std::string out = cb::port::file_internals(p5_shim::bytes_of(t), made, p5_shim::bytes_of(fname));
    p5_shim::merge_notes(notes, made);
    return p5_shim::text_of(out);
}
inline py::str stdio_inits(py::str t, Notes& notes, const py::str& fname) {
    cb::port::Notes made;
    const std::string out = cb::port::stdio_inits(p5_shim::bytes_of(t), made, p5_shim::bytes_of(fname));
    p5_shim::merge_notes(notes, made);
    return p5_shim::text_of(out);
}
inline py::str word_copies(py::str t, Notes& notes, const py::str& fname, const py::set<py::str>& copied) {
    cb::port::Notes made;
    const std::string out = cb::port::word_copies(p5_shim::bytes_of(t), made, p5_shim::bytes_of(fname), p5_shim::names_of(copied));
    p5_shim::merge_notes(notes, made);
    return p5_shim::text_of(out);
}
inline py::str word_unions(const py::str& t, Notes& notes, const py::str& fname, const py::set<py::str>& olds) {
    cb::port::Notes made;
    const std::string out = cb::port::word_unions(p5_shim::bytes_of(t), made, p5_shim::bytes_of(fname), p5_shim::names_of(olds));
    p5_shim::merge_notes(notes, made);
    return p5_shim::text_of(out);
}

inline void old_c_ways(py::dict<py::str, py::str>& out, Notes& notes) {
    cb::port::Sources src = p5_shim::sources_of(out);
    cb::port::Notes made;
    cb::port::old_c_ways(src, made);
    p5_shim::write_back(out, src);
    p5_shim::merge_notes(notes, made);
}
inline void numbered_places(py::dict<py::str, py::str>& out, const py::list<py::str>& extra, Notes& notes) {
    cb::port::Sources src = p5_shim::sources_of(out);
    cb::port::Notes made;
    cb::port::numbered_places(src, p5_shim::strings_of(extra), made);
    p5_shim::write_back(out, src);
    p5_shim::merge_notes(notes, made);
}
inline void agree_wide(py::dict<py::str, py::str>& out, const py::list<py::str>& extra, Notes& notes) {
    cb::port::Sources src = p5_shim::sources_of(out);
    cb::port::Notes made;
    cb::port::agree_wide(src, p5_shim::strings_of(extra), made, cb::port::Rules::from_environment());
    p5_shim::write_back(out, src);
    p5_shim::merge_notes(notes, made);
}
inline std::optional<py::str> widen_member(const py::str& text, const py::str& tag, const py::str& mem) {
    const auto out = cb::port::widen_member(p5_shim::bytes_of(text), p5_shim::bytes_of(tag), p5_shim::bytes_of(mem));
    if (!out) return std::nullopt;
    return p5_shim::text_of(*out);
}

inline bool stored_wide(Port& port, py::dict<py::str, py::str>& out) {
    p5_shim::Synced before = p5_shim::state_of(port);
    cb::port::Sources src = p5_shim::sources_of(out);
    const bool changed = cb::compat::guard([&] { return cb::port::stored_wide(before.state, src, cb::port::Rules::from_environment()); });
    p5_shim::apply_to<Port, DeclRule, split_decl>(port, before, before.state, before.state.decl_rules.size());
    p5_shim::write_back(out, src);
    return changed;
}
inline bool passed_on(Port& port, py::dict<py::str, py::str>& out) {
    p5_shim::Synced before = p5_shim::state_of(port);
    cb::port::Sources src = p5_shim::sources_of(out);
    const bool changed = cb::compat::guard([&] { return cb::port::passed_on(before.state, src); });
    p5_shim::apply_to<Port, DeclRule, split_decl>(port, before, before.state, before.state.decl_rules.size());
    p5_shim::write_back(out, src);
    return changed;
}
inline std::optional<py::str> union_member_for(const py::dict<py::str, py::str>& out, const py::str& union_, const py::str& typ) {
    const auto name = cb::port::union_member_for(p5_shim::sources_of(out), p5_shim::bytes_of(union_), p5_shim::bytes_of(typ));
    if (!name) return std::nullopt;
    return p5_shim::text_of(*name);
}
inline py::set<py::str> visible_tags(const py::dict<py::str, py::str>& out, const py::str& f) {
    py::set<py::str> tags;
    for (const std::string& t : cb::port::visible_tags(p5_shim::sources_of(out), p5_shim::bytes_of(f))) tags.add(p5_shim::text_of(t));
    return tags;
}

inline bool mend(Port& port, py::dict<py::str, py::str>& out, const py::list<Diag>& diags) {
    p5_shim::Synced before = p5_shim::state_of(port);
    cb::port::Sources src = p5_shim::sources_of(out);
    std::vector<cb::port::Diag> asked;
    for (const Diag& d : diags) {
        cb::port::Diag n;
        n.file = p5_shim::bytes_of(d.file);
        n.line = static_cast<std::size_t>(d.line);
        if (d.col) n.col = static_cast<std::size_t>(*d.col);
        n.text = p5_shim::bytes_of(d.text);
        for (const py::str& note : d.notes) n.notes.push_back(p5_shim::bytes_of(note));
        asked.push_back(std::move(n));
    }
    const std::size_t rules_known = before.state.decl_rules.size();
    const bool changed = cb::compat::guard([&] { return cb::port::mend(before.state, src, asked, cb::port::Rules::from_environment()); });
    p5_shim::apply_to<Port, DeclRule, split_decl>(port, before, before.state, rules_known);
    p5_shim::write_back(out, src);
    return changed;
}

}  // namespace codebreaker::port::pre_ansi
