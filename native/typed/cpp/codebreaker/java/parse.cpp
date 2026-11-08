// codebreaker/java/parse.cpp -- typed C++ port of codebreaker.java.parse.

#include "codebreaker/java/parse.hpp"

#include <algorithm>

#include "codebreaker/java/textutil.hpp"

namespace codebreaker::java::parse {

using py::str;

const py::set<str>& _MODIFIERS() {
    static const py::set<str> mods{str("public"), str("protected"), str("private"), str("static"),
                                   str("final"), str("abstract"), str("native"), str("synchronized"),
                                   str("transient"), str("volatile"), str("strictfp")};
    return mods;
}

const py::re::Pattern& _TYPE_DECL() {
    static const py::re::Pattern pattern = py::re::compile(str(
        R"PAT((?P<mods>(?:\b(?:public|final|abstract|strictfp)\b\s+)*)(?P<kind>class|interface|enum)\s+(?P<name>\w+)(?P<rest>[^{]*)\{)PAT"));
    return pattern;
}

// ---------------------------------------------------------------------------
// Lexical helpers
// ---------------------------------------------------------------------------
str _blank_comments(const str& src) {
    std::u32string out;
    int64_t i = 0;
    const int64_t n = src.size();
    bool in_str = false;
    bool in_chr = false;
    while (i < n) {
        char32_t c = src.at_(i);
        bool has_nxt = i + 1 < n;
        char32_t nxt = has_nxt ? src.at_(i + 1) : U'\0';
        if (in_str) {
            out.push_back(c);
            if (c == U'\\' && has_nxt) {
                out.push_back(nxt);
                i += 2;
                continue;
            }
            if (c == U'"') {
                in_str = false;
            }
            i += 1;
            continue;
        }
        if (in_chr) {
            out.push_back(c);
            if (c == U'\\' && has_nxt) {
                out.push_back(nxt);
                i += 2;
                continue;
            }
            if (c == U'\'') {
                in_chr = false;
            }
            i += 1;
            continue;
        }
        if (c == U'"') {
            in_str = true;
            out.push_back(c);
            i += 1;
            continue;
        }
        if (c == U'\'') {
            in_chr = true;
            out.push_back(c);
            i += 1;
            continue;
        }
        if (c == U'/' && has_nxt && nxt == U'/') {
            while (i < n && src.at_(i) != U'\n') {
                out.push_back(U' ');
                i += 1;
            }
            continue;
        }
        if (c == U'/' && has_nxt && nxt == U'*') {
            while (i < n && !(src.at_(i) == U'*' && i + 1 < n && src.at_(i + 1) == U'/')) {
                out.push_back(src.at_(i) == U'\n' ? U'\n' : U' ');
                i += 1;
            }
            out.append(U"  ");
            i += 2;
            continue;
        }
        out.push_back(c);
        i += 1;
    }
    return str(out);
}

int64_t _match_brace(const str& s, int64_t open_idx) {
    int64_t depth = 0;
    int64_t i = open_idx;
    const int64_t n = s.size();
    bool in_str = false;
    bool in_chr = false;
    while (i < n) {
        char32_t c = s.at_(i);
        if (in_str) {
            if (c == U'\\') {
                i += 2;
                continue;
            }
            if (c == U'"') {
                in_str = false;
            }
        } else if (in_chr) {
            if (c == U'\\') {
                i += 2;
                continue;
            }
            if (c == U'\'') {
                in_chr = false;
            }
        } else if (c == U'"') {
            in_str = true;
        } else if (c == U'\'') {
            in_chr = true;
        } else if (c == U'{') {
            depth += 1;
        } else if (c == U'}') {
            depth -= 1;
            if (depth == 0) {
                return i + 1;
            }
        }
        i += 1;
    }
    return n;
}

py::list<str> _split_top_commas(const str& s) {
    py::list<str> parts;
    int64_t depth = 0;
    std::u32string buf;
    bool in_str = false;
    bool in_chr = false;
    for (int64_t k = 0; k < s.size(); ++k) {
        char32_t c = s.at_(k);
        if (in_str) {
            buf.push_back(c);
            if (c == U'"') {
                in_str = false;
            }
            continue;
        }
        if (in_chr) {
            buf.push_back(c);
            if (c == U'\'') {
                in_chr = false;
            }
            continue;
        }
        if (c == U'"') {
            in_str = true;
            buf.push_back(c);
            continue;
        }
        if (c == U'\'') {
            in_chr = true;
            buf.push_back(c);
            continue;
        }
        if (c == U'(' || c == U'<' || c == U'[') {
            depth += 1;
        } else if (c == U')' || c == U'>' || c == U']') {
            depth -= 1;
        }
        if (c == U',' && depth == 0) {
            parts.append(str(buf));
            buf.clear();
        } else {
            buf.push_back(c);
        }
    }
    if (!str(buf).strip().empty()) {
        parts.append(str(buf));
    }
    py::list<str> out;
    for (const str& p : parts) {
        str t = p.strip();
        if (!t.empty()) {
            out.append(t);
        }
    }
    return out;
}

// ---------------------------------------------------------------------------
// Type / parameter parsing
// ---------------------------------------------------------------------------
py::ref<ir::JType> parse_type(const str& s_in) {
    str s = s_in.strip();
    int64_t dims = 0;
    while (s.endswith(str("[]"))) {
        dims += 1;
        s = s.slice(0, s.size() - 2).rstrip();
    }
    py::list<py::ref<ir::JType>> args;
    int64_t lt = s.find(str("<"));
    if (lt != -1 && s.endswith(str(">"))) {
        str inner = s.slice(lt + 1, -1);
        for (const str& a : _split_top_commas(inner)) {
            args.append(parse_type(a));
        }
        s = s.slice(0, lt).rstrip();
    }
    py::list<str> dotted = py::split(s, str("."));
    str name = dotted[-1];  // drop package qualifier; emit uses simple names
    return std::make_shared<ir::JType>(name, dims, args);
}

py::list<py::ref<ir::Param>> _parse_params(const str& s) {
    py::list<py::ref<ir::Param>> out;
    for (const str& raw : _split_top_commas(s)) {
        py::list<str> all_toks = py::split(raw.replace(str("..."), str("[]")));  // varargs -> array
        bool is_final = all_toks.contains(str("final"));
        py::list<str> toks;
        for (const str& t : all_toks) {
            if (t != str("final")) {
                toks.append(t);
            }
        }
        if (toks.size() < 2) {
            continue;
        }
        str name = toks[-1];
        str type_str = py::join(str(" "), toks.slice(0, -1));
        out.append(std::make_shared<ir::Param>(name, parse_type(type_str), is_final));
    }
    return out;
}

std::tuple<py::set<str>, py::list<str>> _split_mods(const py::list<str>& tokens) {
    py::set<str> mods;
    py::list<str> rest;
    for (const str& t : tokens) {
        if (_MODIFIERS().contains(t)) {
            mods.add(t);
        } else {
            rest.append(t);
        }
    }
    return {mods, rest};
}

str _access_of(const py::set<str>& mods) {
    if (mods.contains(str("public"))) {
        return ir::PUBLIC;
    }
    if (mods.contains(str("protected"))) {
        return ir::PROTECTED;
    }
    if (mods.contains(str("private"))) {
        return ir::PRIVATE;
    }
    return ir::PACKAGE;
}

// ---------------------------------------------------------------------------
// Member parsing
// ---------------------------------------------------------------------------
int64_t _line_of(const str& src, int64_t offset) {
    // src.count('\n', 0, offset)
    int64_t end = std::min<int64_t>(offset, src.size());
    int64_t n = 0;
    for (int64_t i = 0; i < end; ++i) {
        if (src.at_(i) == U'\n') {
            n += 1;
        }
    }
    return n + 1;
}

py::list<py::ref<ir::Field>> _parse_field_decl(const str& decl, const str& class_name, int64_t line) {
    (void)class_name;
    int64_t eq = _toplevel_index(decl, str("="));
    str head = (eq != -1 ? decl.slice(0, eq) : decl).strip();
    std::optional<str> init;
    if (eq != -1) {
        init = decl.slice(eq + 1, decl.size()).strip();
    }
    auto [mods, rest] = _split_mods(py::split(head));
    if (rest.size() < 2) {
        return py::list<py::ref<ir::Field>>();
    }
    str type_str = py::join(str(" "), rest.slice(0, -1));  // everything before the declarator name
    str name = rest[-1].rstrip(str(","));
    py::list<py::ref<ir::Field>> out;
    out.append(std::make_shared<ir::Field>(name, parse_type(type_str), _access_of(mods), mods.contains(str("static")),
                                           mods.contains(str("final")), init, line));
    return out;
}

int64_t _toplevel_index(const str& s, const str& ch) {
    int64_t depth = 0;
    bool in_str = false;
    bool in_chr = false;
    const bool single = ch.size() == 1;
    const char32_t target = single ? ch.at_(0) : U'\0';
    for (int64_t i = 0; i < s.size(); ++i) {
        char32_t c = s.at_(i);
        if (in_str) {
            if (c == U'"') {
                in_str = false;
            }
            continue;
        }
        if (in_chr) {
            if (c == U'\'') {
                in_chr = false;
            }
            continue;
        }
        if (c == U'"') {
            in_str = true;
            continue;
        }
        if (c == U'\'') {
            in_chr = true;
            continue;
        }
        if (c == U'(' || c == U'<' || c == U'[') {
            depth += 1;
        } else if (c == U')' || c == U'>' || c == U']') {
            depth -= 1;
        } else if (single && c == target && depth == 0) {
            return i;
        }
    }
    return -1;
}

py::ref<ir::Method> _parse_signature(const str& header, const str& class_name) {
    int64_t lp = header.find(str("("));
    int64_t rp = header.rfind(str(")"));
    py::list<py::ref<ir::Param>> params;
    if (lp != -1 && rp > lp) {
        params = _parse_params(header.slice(lp + 1, rp));
    }
    py::list<str> pre = py::split(header.slice(0, lp));
    auto [mods, rest] = _split_mods(pre);
    str name = rest[-1];
    str ret = py::join(str(" "), rest.slice(0, -1));
    bool is_ctor = (name == class_name && ret.empty());
    py::ref<ir::JType> return_type;
    if (is_ctor) {
        return_type = nullptr;
    } else if (!ret.empty()) {
        return_type = parse_type(ret);
    } else {
        return_type = std::make_shared<ir::JType>(str("void"));
    }
    return std::make_shared<ir::Method>(name, return_type, params, _access_of(mods), mods.contains(str("static")),
                                        mods.contains(str("abstract")), mods.contains(str("final")), is_ctor);
}

bool _looks_like_method(const str& decl) {
    int64_t lp = decl.find(str("("));
    if (lp == -1) {
        return false;
    }
    int64_t eq = _toplevel_index(decl, str("="));
    return eq == -1 || lp < eq;
}

py::list<MemberItem> _iter_members(const str& body, int64_t body_offset, const str& src) {
    py::list<MemberItem> items;
    int64_t i = 0;
    const int64_t n = body.size();
    int64_t start = 0;
    bool in_str = false;
    bool in_chr = false;
    bool seen_eq = false;  // a '=' since the member start -> initializer
    while (i < n) {
        char32_t c = body.at_(i);
        if (in_str) {
            if (c == U'\\') {
                i += 2;
                continue;
            }
            if (c == U'"') {
                in_str = false;
            }
            i += 1;
            continue;
        }
        if (in_chr) {
            if (c == U'\\') {
                i += 2;
                continue;
            }
            if (c == U'\'') {
                in_chr = false;
            }
            i += 1;
            continue;
        }
        if (c == U'"') {
            in_str = true;
            i += 1;
            continue;
        }
        if (c == U'\'') {
            in_chr = true;
            i += 1;
            continue;
        }
        if (c == U'=') {
            seen_eq = true;
            i += 1;
            continue;
        }
        if (c == U';') {
            str decl = body.slice(start, i).strip();
            if (!decl.empty()) {
                items.append(MemberItem{str("decl"), decl, str(), _line_of(src, body_offset + start), 0});
            }
            i += 1;
            start = i;
            seen_eq = false;
            continue;
        }
        if (c == U'{') {
            if (seen_eq) {  // field initializer `= ... { ... }` (array/aggregate),
                i = _match_brace(body, i);  // not a method body -- skip the braces, keep the field
                continue;
            }
            str header = body.slice(start, i).strip();
            int64_t close = _match_brace(body, i);
            str inner = body.slice(i + 1, close - 1);
            py::set<str> htoks = py::set<str>::from(py::split(header));
            bool has_type_kw = htoks.contains(str("class")) || htoks.contains(str("interface")) || htoks.contains(str("enum"));
            bool subset = true;  // htoks <= _MODIFIERS
            for (const str& t : py::split(header)) {
                if (!_MODIFIERS().contains(t)) {
                    subset = false;
                    break;
                }
            }
            if (has_type_kw) {
                // inner type -- skip (deferred)
            } else if (htoks.contains(str("static")) && subset) {
                items.append(MemberItem{str("static_init"), inner, str(), _line_of(src, body_offset + i + 1), 0});  // static { ... }
            } else if (header.empty()) {
                items.append(MemberItem{str("inst_init"), inner, str(), _line_of(src, body_offset + i + 1), 0});  // instance { ... }
            } else if (subset) {
                // other modifier-only block -- skip
            } else {
                items.append(MemberItem{str("method"), header, inner, _line_of(src, body_offset + start),
                                        _line_of(src, body_offset + i + 1)});
            }
            i = close;
            start = i;
            seen_eq = false;
            continue;
        }
        i += 1;
    }
    return items;
}

py::list<std::tuple<int64_t, str>> _body_lines(const str& src, int64_t start_line, int64_t end_line) {
    py::list<str> lines = py::split(src, str("\n"));
    py::list<std::tuple<int64_t, str>> out;
    for (int64_t ln = start_line; ln < std::min<int64_t>(end_line, lines.size()) + 1; ++ln) {
        out.append(std::make_tuple(ln, lines[ln - 1]));
    }
    return out;
}

// ---------------------------------------------------------------------------
// Top-level entry
// ---------------------------------------------------------------------------
// Add one parsed member (shared by the class body and the enum tail, which differ only in the method
// signature's class name and the `abstract` marking of interface methods).
static void add_member_(const py::ref<ir::JClass>& cls, const MemberItem& item, const str& class_name,
                        const str& src, bool mark_interface_abstract) {
    if (item.kind == str("decl")) {
        if (_looks_like_method(item.text)) {  // abstract / interface method
            py::ref<ir::Method> meth = _parse_signature(item.text, class_name);
            if (mark_interface_abstract && cls->is_interface) {
                meth->is_abstract = true;
            }
            cls->methods.append(meth);
        } else {
            cls->fields.extend(_parse_field_decl(item.text, class_name, item.line));
        }
    } else if (item.kind == str("static_init") || item.kind == str("inst_init")) {  // static { } / instance { } block
        py::list<std::tuple<int64_t, str>> block = _body_lines(src, item.line, item.line + item.text.count(str("\n")));
        if (item.kind == str("static_init")) {
            cls->static_inits.append(block);
        } else {
            cls->inst_inits.append(block);
        }
    } else {
        py::ref<ir::Method> meth = _parse_signature(item.text, class_name);
        meth->line = item.line;
        // body lines from the ORIGINAL source (real text + real line numbers)
        meth->body = _body_lines(src, item.bline, item.bline + item.inner.count(str("\n")));
        cls->methods.append(meth);
    }
}

py::ref<ir::JClass> parse_source(const str& src, const std::optional<str>& source_path) {
    str blanked = _blank_comments(src);

    std::optional<str> package;
    {
        auto pkg_m = py::re::search(str(R"PAT(\bpackage\s+([\w.]+)\s*;)PAT"), blanked);
        if (pkg_m) {
            package = pkg_m->group(1);
        }
    }

    auto m = _TYPE_DECL().search(blanked);
    if (!m) {
        // 'no type declaration found' + (' in %s' % source_path if source_path else '')
        str msg("no type declaration found");
        if (source_path && !source_path->empty()) {
            msg = msg + str(" in ") + *source_path;
        }
        throw py::ValueError(msg.utf8());
    }

    str mods = m->group(1);
    str kind = m->group(2);
    str name = m->group(3);
    str rest = m->group(4);

    std::optional<str> parent;
    {
        auto pm = py::re::search(str(R"PAT(\bextends\s+([\w.]+))PAT"), rest);
        if (pm) {
            parent = py::split(pm->group(1), str("."))[-1];
        }
    }
    py::list<str> interfaces;
    {
        auto im = py::re::search(str(R"PAT(\bimplements\s+([\w.,\s]+))PAT"), rest);
        if (im) {
            for (const str& x : py::split(im->group(1), str(","))) {
                if (!x.strip().empty()) {
                    interfaces.append(py::split(x, str("."))[-1].strip());
                }
            }
        }
    }

    py::ref<ir::JClass> cls = std::make_shared<ir::JClass>(name, package, parent, interfaces, kind == str("interface"),
                                                           mods.contains(str("abstract")), kind == str("enum"),
                                                           source_path);

    int64_t open_brace = m->end() - 1;
    int64_t close = _match_brace(blanked, open_brace);
    str body = blanked.slice(open_brace + 1, close - 1);
    int64_t body_offset = open_brace + 1;

    if (cls->is_enum) {
        _parse_enum_body(cls, body, body_offset, blanked, src);
        return cls;
    }

    for (const MemberItem& item : _iter_members(body, body_offset, blanked)) {
        add_member_(cls, item, name, src, true);
    }
    return cls;
}

void _parse_enum_body(const py::ref<ir::JClass>& cls, const str& body, int64_t body_offset, const str& blanked,
                      const str& src) {
    int64_t semi = _toplevel_index(body, str(";"));
    str consts_part = semi != -1 ? body.slice(0, semi) : body;
    for (const str& raw : _split_top_commas(consts_part)) {
        int64_t lp = raw.find(str("("));
        str nm;
        py::list<str> args;
        if (lp != -1 && raw.rstrip().endswith(str(")"))) {
            nm = raw.slice(0, lp).strip();
            args = _split_top_commas(raw.slice(lp + 1, raw.rfind(str(")"))));
        } else {
            nm = raw.strip();
        }
        if (!nm.empty()) {
            cls->enum_consts.append(std::make_shared<ir::EnumConst>(nm, args));
        }
    }
    // members after the ';' (enum methods/fields) -- parse like a class body. Bodies are captured
    // from the ORIGINAL src (real text + line numbers) so typed-enum ctors/methods translate; a
    // constant-only enum ignores them (header-only int constants).
    if (semi != -1) {
        str tail = body.slice(semi + 1, body.size());
        for (const MemberItem& item : _iter_members(tail, body_offset + semi + 1, blanked)) {
            add_member_(cls, item, cls->name, src, false);
        }
    }
}

py::ref<ir::JClass> parse_file(const str& path) {
    str src = textutil::read_text(path, str("replace"), false);
    return parse_source(src, path);
}

}  // namespace codebreaker::java::parse
