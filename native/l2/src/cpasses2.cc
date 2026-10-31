// cpasses2.cc -- backends/c.py's passes on the tree, continued: code-segment reads, the names and the
// heading a module is written from, the locals' layout (_Locals, _buffer_fields, the stack-segment
// stores), far reads, the 32-bit path's own and the PowerPC path's.
#include <algorithm>

#include "cback.hpp"

namespace cb {

using ParentMap = std::unordered_map<Id, V>;
bool x_unary_as_read(const V& u, const char* op, const ParentMap& parent);
bool x_star_as_read(const V& u, const ParentMap& parent);
bool x_arrow_of(const V& u, const ParentMap& parent);
bool x_unary_parent(const V& e, const ParentMap& parent, const char* op);
bool x_bare_name(const V& e, const Opt& name);
std::string x_written(const V& e, const V* under);
bool x_near_number(const std::string& inner);
std::string x_shape(const V& e);
std::optional<std::pair<V, V>> x_mk_fp(const V& e);

static bool in_whole(const V& e) { return now->whole.count(id(e)) != 0; }
static std::string mark(const V& e) {
    auto it = now->marks.find(id(e));
    return it == now->marks.end() ? std::string() : it->second;
}
static bool field_sym(const V& e) {
    const V& s = e.get("sym");
    return py::truthy(s) && s.get("kind") == "field";
}
static V dict_or_empty(const V& v) { return py::truthy(v) ? v : V::dict(); }
static std::string node_kind(const V& e) {
    const V& n = e.get("node");
    return n.is_str() ? n.str() : std::string();
}
static ParentMap parents(const Nodes& nodes) {
    ParentMap m;
    for (auto& np : nodes) m[id(np.e)] = np.p;
    return m;
}
static const V& parent_of(const ParentMap& m, const V& e) {
    auto it = m.find(id(e));
    return it == m.end() ? py::NONE : it->second;
}
static std::vector<V> live_locals() {
    std::vector<V> out;
    for (auto& v : now->locals)
        if (!now->drop.count(sym_name(v))) out.push_back(v);
    return out;
}
static bool is_ident(const std::string& s) {
    static const re::Regex R("[A-Za-z_]\\w*");
    return (bool)R.fullmatch(s);
}
static int64_t floordiv(int64_t a, int64_t b) {
    int64_t q = a / b;
    if ((a % b != 0) && ((a < 0) != (b < 0))) q--;
    return q;
}
static std::string cp_slice(const std::string& s, size_t n) {
    size_t pos = 0, k = 0;
    while (pos < s.size() && k < n) {
        py::decode_cp(s, pos);
        k++;
    }
    return s.substr(0, pos);
}
static Strs all_inserted() {
    Strs out;
    for (auto* m : {&now->before, &now->after})
        for (auto& kv : m->items)
            for (auto& t : kv.second) out.push_back(t);
    return out;
}

// ---------------------------------------------------------------------------------------------- code-segment reads
void p_code_segment_reads(Nodes& nodes, const std::set<std::string>&) {
    static const re::Regex CSW("\\s*(?:x?unknown2|u?int2|undefined2)\\s+__far\\s*\\*\\s*$"), CSN("(0x[0-9a-fA-F]+|\\d+)[uUlL]*"),
        CASTW("\\(*\\s*\\(([^()]*\\*\\s*)\\)");
    V facts = now->facts;
    const V& given = facts.get("code_places");
    if (py::truthy(given)) {
        for (auto& kv : given.items()) {
            std::string t = py::str_of(kv.second);
            int64_t k = kv.first.as_int();
            if (k < 0) k += (int64_t)nodes.size();
            if (k < 0 || k >= (int64_t)nodes.size()) py::raise("IndexError", "list index out of range");
            rewrite(nodes[(size_t)k].e, [t](const V&) { return t; });
        }
        return;
    }
    IdSet seen = seen_now();
    std::string top;
    for (auto& s : now->top->out) top += s;
    if (top.find("codebreaker_cs()") == std::string::npos) return;
    ParentMap parent = parents(nodes);
    std::unordered_map<Id, int64_t> index_of;
    for (size_t k = 0; k < nodes.size(); k++) index_of[id(nodes[k].e)] = (int64_t)k;
    std::vector<V> shown;
    for (auto& np : nodes)
        if (seen.count(id(np.e))) shown.push_back(np.e);
    std::map<std::string, std::string> types;
    std::vector<std::pair<std::string, std::vector<V>>> sets;
    auto sets_of = [&](const std::string& n) -> std::vector<V>* {
        for (auto& x : sets)
            if (x.first == n) return &x.second;
        return nullptr;
    };
    auto sets_add = [&](const std::string& n, const V& v) {
        auto* s = sets_of(n);
        if (!s) {
            sets.push_back({n, {}});
            s = &sets.back().second;
        }
        s->push_back(v);
    };
    std::set<std::string> cs;
    for (auto& v : now->locals) {
        std::string n = sym_name(v);
        if (now->drop.count(n)) continue;
        py::Parts pt = py::partition(decl_now(v), " = ");
        types[n] = pt.a.find('*') != std::string::npos ? "p *" : "v";
        if (py::strip(pt.b) == "codebreaker_cs()") cs.insert(n);
        else if (!pt.sep.empty()) {
            V call = V::dict(), target = V::dict(), sym = V::dict();
            sym.set("name", V(pt.b));
            target.set("node", V("Name"));
            target.set("sym", sym);
            call.set("node", V("Call"));
            call.set("target", target);
            call.set("args", V::list());
            sets_add(n, call);
        }
    }
    auto ident = [&](const V& n) -> Opt {
        if (n.get("node") != "Name" || field_sym(n)) return std::nullopt;
        std::string text = in_whole(n) || !mark(n).empty() ? expr(n) : local_text(name_of(n));
        return is_ident(text) ? Opt(text) : std::nullopt;
    };
    for (auto& e : shown)
        if (at(e, "node") == "Assign" && !py::truthy(e.get("op"))) {
            Opt t = ident(at(e, "target"));
            if (t) sets_add(*t, at(e, "value"));
        }
    auto opaque = [&](const V& n) {
        V x = n.get("node") == "Cast" && !in_whole(n) ? n.get("x") : V();
        if (x.is_none() || x.get("node") != "Unary" || py::truthy(x.get("postfix"))) return false;
        const V& op = x.get("op");
        if (!(op == "*" || op == "&" || op == "-" || op == "+")) return false;
        return !x_unary_as_read(x, op.str().c_str(), parent);
    };
    auto bare = [&](const V& n0) {
        V n = n0;
        while (!n.is_none() && (n.get("node") == "Paren" || n.get("node") == "Cast") && !in_whole(n) && !opaque(n)) {
            V x = n.get("x");
            n = x;
        }
        return n;
    };
    auto is_cs = [&](const V& n0) {
        V n = bare(n0);
        if (n.is_none()) return false;
        if (py::strip(expr(n)) == "codebreaker_cs()") return true;
        Opt i = ident(n);
        return i && cs.count(*i) != 0;
    };
    auto printed = [&](const V& n) {
        for (auto& np : walk(n))
            if (seen.count(id(np.e))) return true;
        return false;
    };
    auto mk_fp = [&](const V& n0) -> std::vector<V> {
        V n = bare(n0);
        if (n.is_none() || n.get("node") != "Call") return {};
        const V& args = n.get("args");
        if ((py::truthy(args) ? args.size() : 0) != 2) return {};
        const V& t = n.get("target");
        std::string fn = !t.is_none() && t.get("node") == "Name" ? name_of(t) : sym_name(n);
        if (fn == "MK_FP" && printed(args[0]) && printed(args[1])) return args.seq();
        return {};
    };
    std::vector<std::pair<V, std::string>> reads;
    std::vector<V> far;
    for (auto& np : nodes) {
        const V& e = np.e;
        if (at(e, "node") != "Call") continue;
        const V& t = e.get("target");
        Opt fn;
        if (!t.is_none() && t.get("node") == "Name") fn = name_of(t);
        else if (t.is_none()) {
            V sn = sym_name_v(e);
            if (!sn.is_none()) fn = py::str_of(sn);
        }
        std::vector<V> args;
        if (py::truthy(e.get("args")))
            for (auto& a : e.get("args").seq())
                if (printed(a)) args.push_back(a);
        if (fn && *fn == "MK_FP" && args.size() == 2 && is_cs(args[0])) {
            V up = e;
            bool called = false;
            while (!parent_of(parent, up).is_none()) {
                V p = parent_of(parent, up);
                if (p.get("node") == "Call" && same(p.get("target"), up)) called = true;
                up = p;
            }
            reads.push_back({args[1], called ? "call" : ""});
        } else if (!fn || *fn != "MK_FP") {
            for (size_t k = 0; k + 1 < args.size(); k++)
                if (is_cs(args[k + 1]) && !is_cs(args[k])) far.push_back(args[k]);
        }
    }
    for (auto& sp : now->seg_ptrs)
        if (seen.count(sp.at) && is_cs(sp.sel)) {
            V n = V::dict(), sym = V::dict();
            sym.set("kind", V("local"));
            sym.set("name", V(sp.name));
            n.set("node", V("Name"));
            n.set("sym", sym);
            far.push_back(n);
        }
    if (reads.empty() && far.empty()) return;
    V places = V::dict(), events = V::list(), unknown = V::list(), unsafe = V::list();
    std::set<std::string> visited;
    auto type_of = [&](const std::string& n, const char* dflt) {
        auto it = types.find(n);
        return it == types.end() ? std::string(dflt) : it->second;
    };
    auto pointer = [&](const V& n) {
        Opt i = ident(n);
        return type_of(i ? *i : std::string(), "").find('*') != std::string::npos;
    };
    std::function<std::set<std::pair<int64_t, int64_t>>(const V&, Opt, int)> follow;
    auto through_cs = [&](const V& t) -> std::optional<std::pair<bool, V>> {
        V arg;
        bool word;
        if (t.get("node") == "Unary" && t.get("op") == "*" && !py::truthy(t.get("postfix")) && !in_whole(t) && x_star_as_read(t, parent) &&
            !x_arrow_of(t, parent)) {
            arg = at(t, "x");
            while (arg.get("node") == "Paren") {
                V x = at(arg, "x");
                arg = x;
            }
            re::Match m = CASTW.match(expr(arg));
            word = m && CSW.match(m.group(1));
        } else if (t.get("node") == "Select" && !in_whole(t)) {
            arg = arrow(t) ? at(at(t, "obj"), "x") : at(t, "obj");
            word = false;
        } else
            return std::nullopt;
        auto args = mk_fp(arg);
        if (!args.empty()) {
            Opt i = ident(args[0]);
            if (expr(args[0]).find("codebreaker_cs()") != std::string::npos || (i && cs.count(*i))) return std::make_pair(word, args[1]);
        }
        return std::nullopt;
    };
    auto from_cs = [&](const V& t, int depth) {
        auto got = through_cs(t);
        if (!got) return;
        if (got->first) {
            for (auto& kv : follow(got->second, std::nullopt, depth + 1))
                events.append(V::tuple({V("pointers"), V::tuple({V(kv.first), V(kv.second)})}));
        } else
            unsafe.append(V(cp_slice(expr(t), 60)));
    };
    auto is_index = [&](const std::string& name) {
        if (type_of(name, "*").find('*') != std::string::npos) return false;
        auto* s = sets_of(name);
        if (!s) return false;
        for (auto& rhs : *s) {
            V rb = bare(rhs);
            if (rb.is_none() || rb.get("node") == "Lit" || rb.get("node") == "Name" || through_cs(rb)) continue;
            if (rb.get("node") == "Binary" && (rb.get("op") == "+" || rb.get("op") == "-")) continue;
            return true;
        }
        return false;
    };
    std::function<std::vector<std::pair<V, int>>(const V&, int)> terms = [&](const V& n0, int sign) -> std::vector<std::pair<V, int>> {
        V n = bare(n0);
        if (n.is_none()) return {};
        if (n.get("node") == "Binary" && (n.get("op") == "+" || n.get("op") == "-") && !in_whole(n)) {
            auto a = terms(at(n, "left"), sign), b = terms(at(n, "right"), n.get("op") == "+" ? sign : -sign);
            a.insert(a.end(), b.begin(), b.end());
            return a;
        }
        if (n.get("node") == "Unary" && n.get("op") == "-" && !py::truthy(n.get("postfix")) && !in_whole(n)) return terms(at(n, "x"), -sign);
        return {{n, sign}};
    };
    auto node_terms = [&](const V& e) {
        std::vector<std::pair<V, int>> out;
        for (auto& ts : terms(e, 1)) {
            V t = ts.first;
            if (t.get("node") == "Unary" && t.get("op") == "&" && !py::truthy(t.get("postfix")) && !in_whole(t)) {
                V a = bare(at(t, "x"));
                if (!a.is_none() && a.get("node") == "Select") {
                    V b = bare(arrow(a) ? at(at(a, "obj"), "x") : at(a, "obj"));
                    t = b;
                    if (t.is_none()) continue;
                }
            }
            out.push_back({t, ts.second});
        }
        return out;
    };
    auto number = [&](const V& t) -> std::optional<int64_t> {
        if (t.get("node") != "Lit" || !seen.count(id(t))) return std::nullopt;
        re::Match m = CSN.fullmatch(expr(t));
        if (!m) return std::nullopt;
        return py::int_parse(m.group(1), 0);
    };
    follow = [&](const V& e, Opt self_name, int depth) -> std::set<std::pair<int64_t, int64_t>> {
        std::set<std::pair<int64_t, int64_t>> found;
        if (depth > 12) return found;
        auto ts = node_terms(e);
        std::vector<std::pair<V, int64_t>> numbers;
        for (auto& tsn : ts) {
            auto v = number(tsn.first);
            if (v && ((tsn.second > 0 && *v > 0) ||
                      (*v == 0 && ts.size() == 1 && (!self_name || type_of(*self_name, "").find('*') != std::string::npos))))
                numbers.push_back({tsn.first, *v});
        }
        std::vector<V> names;
        for (auto& tsn : ts)
            if (ident(tsn.first)) names.push_back(tsn.first);
        bool steps = false;
        for (auto& t : names) steps = steps || pointer(t);
        if (!steps && self_name)
            for (auto& t : names) {
                Opt i = ident(t);
                if (i && *i == *self_name) steps = true;
            }
        for (auto& tv : numbers)
            if (!steps) {
                auto it = index_of.find(id(tv.first));
                if (it == index_of.end()) py::raise("KeyError", "node");
                places.set(V(it->second), V(tv.second));
                found.insert({it->second, tv.second});
            }
        for (auto& tsn : ts) from_cs(tsn.first, depth);
        if (!numbers.empty() && !steps) return found;
        for (auto& t : names) {
            std::string name = *ident(t);
            if ((self_name && name == *self_name) || visited.count(name)) continue;
            visited.insert(name);
            if (!sets_of(name)) unknown.append(V(name));
            if (is_index(name)) continue;
            auto* s = sets_of(name);
            if (s) {
                std::vector<V> rhss = *s;
                for (auto& rhs : rhss)
                    for (auto& x : follow(rhs, name, depth + 1)) found.insert(x);
            }
        }
        return found;
    };
    V each = V::list();
    auto pairs = [](const std::set<std::pair<int64_t, int64_t>>& got) {
        V l = V::list();
        for (auto& kv : got) l.append(V::tuple({V(kv.first), V(kv.second)}));
        return l;
    };
    for (auto& r : reads) {
        visited.clear();
        auto got = follow(r.first, std::nullopt, 0);
        if (r.second == "call")
            for (auto& kv : got) events.append(V::tuple({V("call"), V::tuple({V(kv.first), V(kv.second)})}));
        each.append(pairs(got));
    }
    for (auto& e : far) {
        visited.clear();
        each.append(pairs(follow(e, std::nullopt, 0)));
    }
    V out = V::dict();
    out.set("reads", V((long long)(reads.size() + far.size())));
    out.set("places", places);
    out.set("events", events);
    out.set("unknown", unknown);
    out.set("each", each);
    out.set("unsafe", unsafe);
    facts.set("code_reads", out);
}

// ---------------------------------------------------------------------------------------------- the names used
void p_names_used(Nodes& nodes, const std::set<std::string>&) {
    static const re::Regex STRING("\"(?:\\\\.|[^\"\\\\])*\"|'(?:\\\\.|[^'\\\\])*'|/\\*.*?\\*/", re::S), IDENT("(?<![\\w.>])[A-Za-z_]\\w*"),
        CALL_IN("(?<![\\w.>])([A-Za-z_]\\w*)\\s*\\("), THROUGH_IN("\\(\\*\\s*([a-z]+Ram[0-9a-f]{8})\\s*\\)\\s*\\("),
        SEGN("codebreaker_segment_(\\d+)"), RAM("([a-z]+)Ram([0-9a-f]{8})");
    IdSet seen = seen_now();
    V names = V::set(), called = V::set(), through = V::set();
    auto plain = [&](const V& x) { return seen.count(id(x)) && !in_whole(x); };
    for (auto& np : nodes) {
        const V& e = np.e;
        std::string n = node_kind(e);
        if (n == "Name" && !field_sym(e) && plain(e) && mark(e).empty()) {
            py::set_add(names, V(local_text(name_of(e))));
        } else if (n == "Call") {
            V t = e.get("target");
            if (t.is_none()) {
                if (plain(e)) {
                    V nm = sym_name_v(e);
                    py::set_add(names, nm);
                    py::set_add(called, nm);
                }
            } else if (t.get("node") == "Name" && seen.count(id(t))) {
                py::set_add(called, V(plain(t) ? local_text(name_of(t)) : expr(t)));
            } else {
                while (t.get("node") == "Paren" && plain(t)) {
                    V x = at(t, "x");
                    t = x;
                }
                if (t.get("node") == "Unary" && t.get("op") == "*" && !py::truthy(t.get("postfix")) && plain(t) &&
                    at(t, "x").get("node") == "Name" && seen.count(id(at(t, "x"))))
                    py::set_add(through, V(plain(at(t, "x")) ? name_of(at(t, "x")) : expr(at(t, "x"))));
            }
        }
    }
    Strs wrote;
    for (auto& kv : now->shown.items) wrote.push_back(kv.second);
    for (auto& t : all_inserted()) wrote.push_back(t);
    for (auto& d : now->declare_first) wrote.push_back(d.first + " " + d.second + ";");
    wrote.push_back(now->locals_text ? *now->locals_text : std::string());
    for (auto& text : wrote) {
        std::string pl = STRING.sub_fn([](const re::Match&) { return std::string("\"\""); }, text);
        for (auto& s : IDENT.findall(pl)) py::set_add(names, V(s));
        for (auto& s : CALL_IN.findall(pl)) py::set_add(called, V(s));
        for (auto& s : THROUGH_IN.findall(pl)) py::set_add(through, V(s));
    }
    for (auto& v : now->locals)
        if (!now->drop.count(sym_name(v))) {
            std::string pl = decl_now(v);
            for (auto& s : IDENT.findall(pl)) py::set_add(names, V(s));
            for (auto& s : CALL_IN.findall(pl)) py::set_add(called, V(s));
        }
    V globals = V::set(), ct = V::set(), segments = V::set();
    for (auto& n : names.seq()) {
        std::string s = py::truthy(n) ? py::str_of(n) : "";
        re::Match m = RAM.fullmatch(s);
        if (m) py::set_add(globals, V::tuple({V(m.group(1)), V(m.group(2))}));
        re::Match g = SEGN.fullmatch(s);
        if (g) py::set_add(segments, V(py::int_parse(g.group(1), 10)));
    }
    for (auto& n : through.seq())
        if (RAM.fullmatch(py::str_of(n))) py::set_add(ct, n);
    V out = V::dict();
    out.set("globals", globals);
    out.set("called_through", ct);
    out.set("called", called);
    out.set("mk_fp", V(py::contains(called, V("MK_FP"))));
    out.set("segments", segments);
    now->facts.set("names", out);
}

// ---------------------------------------------------------------------------------------------- the heading's facts
V arguments_at(const std::string& text, int64_t open_paren) {
    int depth = 0;
    V args = V::list();
    size_t start = (size_t)open_paren + 1, i = (size_t)open_paren;
    while (i < text.size()) {
        char ch = text[i];
        if (ch == '(') depth++;
        else if (ch == ')') {
            depth--;
            if (depth == 0) {
                std::string a = text.substr(start, i - start);
                if (!py::strip(a).empty()) args.append(V(a));
                return V::tuple({args, V((long long)(i + 1))});
            }
        } else if (ch == ',' && depth == 1) {
            args.append(V(text.substr(start, i - start)));
            start = i + 1;
        }
        i++;
    }
    return V::tuple({args, V((long long)text.size())});
}

V words_of(const std::string& ty0) {
    static const re::Regex WIDE("\\b(u?int4|xunknown4|long|unkbyte3|unkint3|unkuint3|int3|float4)\\b"),
        ONE("\\b(u?int2|xunknown2|int|unsigned|short|code|u?int1|char|xunknown1|bool)\\b");
    std::string ty = py::join(" ", py::split_ws(ty0));
    if (ty.find('*') != std::string::npos) return V(ty.find("__far") != std::string::npos ? 2 : 1);
    if (WIDE.search(ty)) return V(2);
    if (ONE.search(ty)) return V(1);
    return V();
}

V parameters(const std::string& text) {
    static const re::Regex DEF("^([^\\n#{};]*?\\b(\\w+))\\(([^)]*)\\)\\n((?:[^{;\\n]+;\\n)*)\\{", re::M), W("\\w+");
    re::Match m = DEF.search(text);
    if (!m) return V();
    std::vector<std::pair<std::string, std::string>> decl;
    auto decl_get = [&](const std::string& k, const std::string& dflt) {
        for (auto& d : decl)
            if (d.first == k) return d.second;
        return dflt;
    };
    for (auto& d0 : py::splitlines(py::strip(m.group(4)))) {
        std::string d = py::rstrip_chars(py::strip(d0), ";");
        auto nm = W.findall(d);
        if (!nm.empty()) {
            std::string last = nm.back();
            std::string val = py::strip(d.substr(0, d.rfind(last)));
            bool found = false;
            for (auto& x : decl)
                if (x.first == last) {
                    x.second = val;
                    found = true;
                }
            if (!found) decl.push_back({last, val});
        }
    }
    Strs names;
    for (auto& p : py::split(m.group(3), ","))
        if (!py::strip(p).empty()) names.push_back(py::strip(p));
    Strs head;
    auto lines = py::split(text.substr(0, (size_t)m.end(2)), "\n");
    for (size_t k = lines.size(); k-- > 0;) {
        std::string s = py::strip(lines[k]);
        if (!head.empty() && (s.empty() || py::endswith(s, ";") || py::endswith(s, "}") || py::endswith(s, "*/") || py::startswith(s, "#") ||
                              py::startswith(s, "extern ") || py::startswith(s, "/*")))
            break;
        head.insert(head.begin(), s);
    }
    V ps = V::list();
    for (auto& p : names) ps.append(V::tuple({V(p), V(decl_get(p, "int"))}));
    return V::tuple({V(py::join(" ", head)), ps});
}

V argument_words(const std::string& arg, const V& types) {
    static const re::Regex NUM("-?(0x[0-9a-fA-F]+|\\d+)"), LNUM("-?(0x[0-9a-fA-F]+|\\d+)[lL]"), CAST("\\(\\s*([A-Za-z_][\\w ]*\\**)\\s*\\)");
    std::string a = py::strip(arg);
    while (py::startswith(a, "(") && arguments_at(a, 0)[1].as_int() == (int64_t)a.size()) a = py::strip(a.substr(1, a.size() - 2));
    if (NUM.fullmatch(a)) {
        int64_t v = py::int_parse(a, 0);
        return V(-0x8000 <= v && v <= 0xFFFF ? 1 : 2);
    }
    if (LNUM.fullmatch(a)) return V(2);
    re::Match c = CAST.match(a);
    if (c && arguments_at(a, 0)[1].as_int() == c.end()) return words_of(c.group(1));
    if (is_ident(a)) {
        if (a == "unaff_SS" || a == "unaff_DS" || a == "unaff_ES" || a == "unaff_CS") return V(1);
        return types.has(V(a)) ? words_of(py::str_of(types.get(V(a)))) : V();
    }
    if (py::startswith(a, "&") || py::startswith(a, "\"")) return V(1);
    return V();
}

void p_heading_facts(Nodes& nodes, const std::set<std::string>&) {
    static const re::Regex VAR("^\\s+([A-Za-z_][\\w ]*?[\\w*])\\s*\\**\\s*(\\w+)(\\s*\\[[^\\]]*\\])?\\s*;\\s*$"), CALLED("\\b(\\w+)\\s*\\(");
    const V& doc = now->doc;
    IdSet seen = seen_now();
    std::string sig = signature(doc);
    std::string definition = sig + "\n";
    if (now->old_style) {
        for (auto& nd : *now->old_style) definition += nd.second + ";\n";
    } else
        definition += "\n";
    definition += "{";
    V params = parameters(definition);
    std::vector<V> locs = live_locals();
    std::string region;
    for (size_t k = now->declare_first.size(); k-- > 0;) region += "\n  " + now->declare_first[k].first + " " + now->declare_first[k].second + ";";
    if (now->locals_text) region += *now->locals_text;
    else
        for (auto& v : locs) region += "\n  " + decl_now(v) + ";";
    V types = V::dict();
    if (py::truthy(params))
        for (auto& pt : params[1].seq()) types.set(pt[0], pt[1]);
    for (auto& line : py::splitlines(region)) {
        re::Match mm = VAR.match(line);
        if (mm && mm.group(1) != "return" && mm.group(1) != "goto") {
            size_t star = py::count(line, "*");
            std::string t = mm.group(1);
            for (size_t k = 0; k < star; k++) t += " *";
            if (mm.has(3) && !mm.group(3).empty()) t += " *";
            types.set(V(mm.group(2)), V(t));
        }
    }
    V calls = V::list();
    auto from_text = [&](const std::string& text) {
        for (auto& mm : CALLED.finditer(text)) calls.append(V::tuple({V(mm.group(1)), arguments_at(text, mm.end() - 1)[0]}));
    };
    for (auto& np : nodes)
        if (at(np.e, "node") == "Call" && seen.count(id(np.e)) && !in_whole(np.e)) from_text(expr(np.e));
    for (auto& kv : now->shown.items) from_text(kv.second);
    for (auto& t : all_inserted()) from_text(t);
    std::string head = py::join(" ", py::split_ws(py::split(sig, "(", 1)[0]));
    V out = V::dict();
    out.set("params", params);
    out.set("types", types);
    out.set("calls", calls);
    out.set("declaration", V(head + "();"));
    out.set("fastcall", V(head.find("__fastcall16") != std::string::npos));
    now->facts.set("heading", out);
}

void p_far_concat_casts(Nodes& nodes, const std::set<std::string>&) {
    static const re::Regex R("([\\w ]+?) \\*");
    for (auto& np : nodes) {
        const V& e = np.e;
        if (at(e, "node") != "Cast" || in_whole(e) || !mark(e).empty()) continue;
        const V& v = at(e, "x");
        if (v.get("node") == "Call" && !in_whole(v) && x_bare_name(dict_or_empty(v.get("target")), std::string("CONCAT22")) &&
            R.fullmatch(type_text(e.get("type"))))
            now->marks[id(e)] = "_far";
    }
}

// ---------------------------------------------------------------------------------------------- the locals as written (_Locals)
static Opt prefix_of(const V& e);

static Opt prefix_of_id(Id i) {
    auto it = now->nodes_by_id.find(i);
    if (it == now->nodes_by_id.end()) return std::nullopt;
    return prefix_of(it->second);
}

static Opt prefix_of(const V& e) {
    auto ait = now->at.find(id(e));
    if (ait == now->at.end()) return std::nullopt;
    Flat* em = ait->second.first;
    size_t k = ait->second.second;
    std::string pre;
    for (size_t j = 0; j < k && j < em->out.size(); j++) pre += em->out[j];
    auto oit = now->owner_of.find(em);
    if (em == now->top || oit == now->owner_of.end()) return pre;
    Id owner = oit->second;
    std::string* w = now->shown.get(owner);
    std::string whole = w ? *w : std::string();
    std::string part;
    for (auto& s : em->out) part += s;
    long kk;
    if (is_ident(part)) {
        re::Match m = re::rx("(?<![\\w.])" + re::escape(part) + "\\b").search(whole);
        kk = m ? m.start() : -1;
    } else {
        size_t f = whole.find(part);
        kk = f == std::string::npos ? -1 : (long)f;
    }
    Opt up = prefix_of_id(owner);
    if (kk < 0 || !up) return pre;
    return *up + whole.substr(0, (size_t)kk) + pre;
}

struct Locals {
    ParentMap parent;
    std::string text, region;
    struct Ref {
        V e;
        std::string n, form;
    };
    std::vector<Ref> refs;

    Locals(const Nodes& nodes, const std::vector<V>& locs) {
        IdSet seen = seen_now();
        parent = parents(nodes);
        now->nodes_by_id.clear();
        for (auto& np : nodes) now->nodes_by_id[id(np.e)] = np.e;
        for (auto& s : now->top->out) text += s;
        if (now->locals_text) region = *now->locals_text;
        else {
            for (auto& v : locs) region += "\n  " + decl_now(v) + ";";
            if (!locs.empty()) region += "\n  ";
        }
        std::set<std::string> names;
        for (auto& v : locs) names.insert(sym_name(v));
        for (auto& np : nodes)
            if (seen.count(id(np.e)) && x_bare_name(np.e, std::nullopt) && names.count(name_of(np.e))) refs.push_back({np.e, name_of(np.e), "plain"});
        for (auto& np : nodes) {
            auto it = now->virtual_.find(id(np.e));
            if (it != now->virtual_.end() && seen.count(id(np.e))) refs.push_back({np.e, it->second.first, it->second.second});
        }
    }
    static Opt written(const std::string& n) {
        static const re::Regex R("[A-Za-z_]\\w*");
        re::Match m = R.match(local_text(n));
        return m ? Opt(m.group(0)) : std::nullopt;
    }
    std::vector<std::pair<V, std::string>> occurs(const std::string& name) const {
        std::vector<std::pair<V, std::string>> out;
        for (auto& r : refs) {
            Opt w = written(r.n);
            if (w && *w == name) out.push_back({r.e, r.form});
        }
        return out;
    }
    bool addressed(const std::string& name) const {
        static const re::Regex AMP("(?<!&)&\\s*\\Z");
        for (auto& ef : occurs(name)) {
            const V& e = ef.first;
            if (local_text(name) != name) continue;
            if (ef.second == "address") return true;
            if (x_unary_parent(e, parent, "&")) return true;
            const V& up = parent_of(parent, e);
            if (!up.is_none() && up.get("node") == "Index" && same(at(up, "arr"), e) && !in_whole(up) && x_unary_parent(up, parent, "&")) return true;
            if (!up.is_none() && in_whole(up)) {
                Opt pre = prefix_of(e);
                if (pre && !pre->empty() && AMP.search(*pre)) return true;
            }
        }
        return false;
    }
    bool passed(const std::string& name) const {
        static const re::Regex R("[(,]\\s*(?:\\(\\w+\\s*\\*?\\))?\\s*&?\\s*\\Z");
        for (auto& ef : occurs(name)) {
            Opt pre = ef.second == "plain" ? prefix_of(ef.first) : std::nullopt;
            if (pre && !pre->empty() && R.search(*pre)) return true;
        }
        return false;
    }
    void rename(const std::string& name, Made made) {
        now->ren[name] = made;
        now->mut++;
        for (auto& r : refs)
            if (r.n == name && r.form == "plain" && !in_whole(r.e) && r.e.get("node") == "Name") {
                std::string n = r.n;
                rewrite(r.e, [n](const V&) { return local_text(n); });
            }
    }
};

static std::unordered_map<Id, V> statement_keys(const V& body) {
    std::unordered_map<Id, V> keys;
    auto under = [&](const V& x, const V& k) {
        for (auto& np : walk(x)) keys.emplace(id(np.e), k);
    };
    std::function<void(const V&, const V&, const V&)> stmt_k;
    auto block_k = [&](const V& ss, const V& c) {
        for (auto& s : stmts(ss))
            if (s.is_dict()) stmt_k(s, !c.is_none() ? c : s, c);
    };
    stmt_k = [&](const V& s, const V& k, const V& c) {
        keys[id(s)] = k;
        std::string n = node_kind(s);
        if (n == "If") {
            under(s.get("cond"), k);
            const V& then = s.get("then");
            if (then.is_dict()) under(then, k);
            else block_k(then, V());
            const V& orelse = s.get("orelse");
            if (orelse.is_list() && orelse.size() == 1 && orelse[0].get("node") == "If") stmt_k(orelse[0], k, V());
            else block_k(orelse, V());
        } else if (n == "While" || n == "DoWhile") {
            under(s.get("cond"), k);
            block_k(s.get("body"), V());
        } else if (n == "For") {
            for (const char* part : {"init", "cond", "update"}) under(s.get(part), k);
            block_k(s.get("body"), V());
        } else if (n == "Switch") {
            under(s.get("expr"), k);
            for (auto& cs : stmts(s.get("cases"))) {
                under(cs.get("labels"), cs);
                block_k(cs.get("body"), cs);
            }
        } else if (n == "Labeled") {
            std::vector<V> body;
            for (auto& x : stmts(s.get("body")))
                if (x.is_dict()) body.push_back(x);
            for (size_t i = 0; i < body.size(); i++) stmt_k(body[i], !i || c.is_none() ? body[i] : c, c);
        } else if (n == "Block") {
            block_k(s.get("stmts"), c);
        } else
            under(s, k);
    };
    block_k(body, V());
    return keys;
}

static bool calls_with(const std::string& text, const std::string& arr) {
    static const re::Regex CALLR("[A-Za-z_]\\w*\\s*\\(");
    const re::Regex& name = re::rx("(?<![\\w.])" + re::escape(arr) + "\\b|codebreaker_frame\\." + re::escape(arr) + "\\b");
    for (auto& m : CALLR.finditer(text)) {
        int depth = 0;
        size_t k = (size_t)m.end() - 1;
        for (size_t j = k; j < text.size(); j++) {
            if (text[j] == '(') depth++;
            else if (text[j] == ')') depth--;
            if (depth == 0) {
                if (name.search(text, k + 1, j)) return true;
                break;
            }
        }
    }
    return false;
}

static int64_t elem16(const std::string& typ0) {
    std::string typ = py::strip(typ0);
    if (py::endswith(typ, "*")) return 2;
    char last = typ.empty() ? 0 : typ.back();
    if (last == '1') return 1;
    if (last == '2') return 2;
    if (last == '4') return 4;
    return typ == "char" || typ == "uchar" || typ == "byte" ? 1 : 2;
}
static int64_t elem_of_width(const std::string& w, int64_t dflt) {
    if (w == "1") return 1;
    if (w == "2") return 2;
    if (w == "4") return 4;
    return dflt;
}

static std::string resized(const std::string& line, int64_t count) {
    py::Parts p = py::rpartition(line, "[");
    if (p.sep.empty() || p.b.find(']') == std::string::npos) return line;
    return p.a + "[" + std::to_string(count) + "]" + p.b.substr(p.b.find(']') + 1);
}

static std::string far_record(Locals& lv, std::string region, int64_t place, const std::string& ss, const Nodes& nodes) {
    struct Word {
        std::string line, name;
        int64_t size;
    };
    auto scalar = [&](int64_t off) -> std::optional<Word> {
        const re::Regex& R = re::rx("(?m)^  ([\\w ]*?\\w(?:\\s*\\*+)?)\\s*\\b(\\w*Stack_" + pyx(off) + ");[ \\t]*$");
        for (auto& mm : R.finditer(region)) {
            std::string typ = mm.group(1);
            int64_t size = typ.find('*') != std::string::npos ? 2 : elem_of_width(std::string(1, py::strip(typ).empty() ? '\0' : py::strip(typ).back()), 0);
            if (size == 2) return Word{mm.group(0), mm.group(2), size};
            return std::nullopt;
        }
        return std::nullopt;
    };
    auto first = scalar(place + 2);
    if (!first || !lv.addressed(first->name)) return region;
    std::vector<std::optional<Word>> words{first, std::nullopt};
    int64_t off = place - 2;
    while (scalar(off)) {
        words.push_back(scalar(off));
        off -= 2;
    }
    std::string rec = "codebreaker_far_" + pyx(place + 2);
    Strs fields;
    for (auto& w : words) fields.push_back(!w ? std::string("    int2 codebreaker_ss;") : "  " + py::strip(w->line));
    for (auto& w : words)
        if (w) region = py::replace(region, w->line + "\n", "", 1);
    region = "\n  _Packed struct {\n" + py::join("\n", fields) + "\n  } " + rec + ";\n" + (py::startswith(region, "\n") ? region.substr(1) : region);
    for (auto& w : words)
        if (w) {
            std::string n = w->name;
            lv.rename(n, [rec, n]() { return rec + "." + n; });
        }
    for (auto& np : nodes) {
        const V& e = np.e;
        if (at(e, "node") != "ExprStmt" || !now->lines.count(id(e))) continue;
        const V& a = at(e, "expr");
        if (a.get("node") != "Assign" || py::truthy(a.get("op"))) continue;
        const V& t = at(a, "target");
        if (x_bare_name(t, first->name) || (in_whole(t) && t.get("node") == "Name" && name_of(t) == first->name)) {
            now->after[id(e)].push_back(rec + ".codebreaker_ss = " + ss + ";");
            break;
        }
    }
    return region;
}

void p_stack_segment_stores(Nodes& nodes, const std::set<std::string>&) {
    static const re::Regex ARR("  +(.+?)\\b(\\w*Stack_([0-9a-f]+)) \\[(\\d+)\\];[ \\t]*"), SSR("\\bunaff_SS\\b");
    const V& places = now->facts.get("ss_stores");
    if (!py::truthy(places)) return;
    std::vector<V> locs = live_locals();
    Locals lv(nodes, locs);
    IdSet seen = now->seen;
    std::string region = lv.region;
    auto keys = statement_keys(now->doc.get("body"));
    auto at_of = now->stmt_at;
    struct Arr {
        std::string name;
        int64_t ao, count;
        std::string typ;
    };
    std::vector<Arr> arrays;
    for (auto& line : py::split(region, "\n")) {
        re::Match m = ARR.fullmatch(line);
        if (m && py::count(m.group(1), "(") == 0 && !py::endswith(py::strip(m.group(1)), "struct {")) {
            bool have = false;
            for (auto& a : arrays) have = have || a.name == m.group(2);
            if (!have) arrays.push_back({m.group(2), py::int_parse(m.group(3), 16), py::int_parse(m.group(4), 10), py::strip(m.group(1))});
        }
    }
    std::string ss = SSR.search(lv.text) ? "unaff_SS" : "codebreaker_ss()";
    std::set<std::string> made;
    std::set<int64_t> pls;
    for (auto& p : places.seq()) pls.insert(p.as_int());
    for (int64_t place : pls) {
        bool hit_arr = false;
        for (auto& a : arrays) {
            int64_t elem = a.typ.find('*') != std::string::npos ? 2 : elem_of_width(std::string(1, py::strip(a.typ).empty() ? '\0' : py::strip(a.typ).back()), 2);
            if (!(a.ao > place && place >= a.ao - a.count * elem)) continue;
            hit_arr = true;
            int64_t atb = a.ao - place;
            std::string store = "(*(int2 *)((uint1 *)" + local_text(a.name) + " + 0x" + pyx(atb) + ")) = " + ss + ";";
            if (lv.text.find(store) != std::string::npos || made.count(store)) break;
            V best;
            for (auto& np : nodes) {
                const V& e = np.e;
                auto kit = keys.find(id(e));
                if (kit == keys.end() || !at_of.count(id(kit->second)) || !seen.count(id(e))) continue;
                const V& k = kit->second;
                bool hit;
                std::string* shown = now->shown.get(id(e));
                if (shown) hit = calls_with(*shown, a.name);
                else if (at(e, "node") == "Call") {
                    hit = false;
                    if (py::truthy(e.get("args")))
                        for (auto& x : e.get("args").seq())
                            if (calls_with("f(" + expr(x) + ")", a.name)) {
                                hit = true;
                                break;
                            }
                } else
                    continue;
                if (hit && (best.is_none() || at_of[id(k)] < at_of[id(best)])) best = k;
            }
            if (!best.is_none()) {
                now->before[id(best)].push_back(store);
                made.insert(store);
            }
            break;
        }
        if (!hit_arr) region = far_record(lv, region, place, ss, nodes);
    }
    if (region != lv.region) now->locals_text = region;
}

void p_buffer_fields(Nodes& nodes, const std::set<std::string>&) {
    static const re::Regex LOCAL16("(?m)^  (\\w+?)(\\d)?(\\s*\\*+\\s*|\\s+)(\\w*Stack_([0-9a-f]+))( \\[(\\d+)\\])?;\\s*$");
    std::vector<V> locs = live_locals();
    if (locs.empty()) return;
    Locals lv(nodes, locs);
    std::string before = lv.region;
    std::string region = before;
    auto replace = [&](const std::string& o, const std::string& n) { region = py::replace(region, o, n, 1); };
    struct D {
        int64_t off, size;
        bool is_array;
        std::string line, typ;
    };
    std::vector<std::pair<std::string, D>> decl;
    auto dfind = [&](const std::string& n) -> D* {
        for (auto& x : decl)
            if (x.first == n) return &x.second;
        return nullptr;
    };
    auto dset = [&](const std::string& n, const D& d) {
        D* x = dfind(n);
        if (x) *x = d;
        else decl.push_back({n, d});
    };
    auto dpop = [&](const std::string& n) {
        for (size_t k = 0; k < decl.size(); k++)
            if (decl[k].first == n) {
                decl.erase(decl.begin() + k);
                return;
            }
    };
    for (auto& mm : LOCAL16.finditer(region)) {
        std::string typ = mm.group(1), width = mm.has(2) ? mm.group(2) : "", stars = py::strip(mm.has(3) ? mm.group(3) : ""), name = mm.group(4),
                    off = mm.group(5), count = mm.has(7) ? mm.group(7) : "";
        int64_t elem = !stars.empty() ? 2 : !width.empty() ? elem_of_width(width, 2) : elem16(typ);
        int64_t size = !count.empty() ? py::int_parse(count, 10) * elem : elem;
        dset(name, {py::int_parse(off, 16), size, !count.empty(), mm.group(0), typ + width + (stars.empty() ? "" : " " + stars)});
    }
    std::map<int64_t, std::string> by_off;
    for (auto& nd : decl)
        if (!nd.second.is_array) by_off[nd.second.off] = nd.first;
    auto order = decl;
    std::stable_sort(order.begin(), order.end(), [](const std::pair<std::string, D>& a, const std::pair<std::string, D>& b) {
        return -a.second.off < -b.second.off;
    });
    for (auto& nd : order) {
        const std::string& name = nd.first;
        D d = nd.second;
        if (d.is_array || !dfind(name) || !lv.addressed(name)) continue;
        Strs run{name};
        int64_t atb = d.off - d.size;
        while (by_off.count(atb)) {
            const std::string& other = by_off[atb];
            D* od = dfind(other);
            int64_t osize = od ? od->size : 0;
            if (!(osize == d.size && std::find(run.begin(), run.end(), other) == run.end() && od && od->typ == d.typ && !lv.addressed(other))) break;
            run.push_back(other);
            atb -= d.size;
        }
        if (run.size() < 2) continue;
        std::string arr = "a" + name;
        std::string t = d.typ.empty() ? "int2" : d.typ;
        replace(d.line, "  " + t + " " + arr + " [" + std::to_string(run.size()) + "];");
        for (size_t k = 0; k < run.size(); k++) {
            if (k) replace(dfind(run[k])->line, "");
            int kk = (int)k;
            lv.rename(run[k], [arr, kk]() { return local_text(arr) + "[" + std::to_string(kk) + "]"; });
            dpop(run[k]);
        }
        dset(arr, {d.off, d.size * (int64_t)run.size(), true, "  " + t + " " + arr + " [" + std::to_string(run.size()) + "];", d.typ});
    }
    std::vector<std::pair<std::string, D>> arrays;
    for (auto& nd : decl)
        if (nd.second.is_array && lv.passed(nd.first)) arrays.push_back(nd);
    if (arrays.empty()) {
        if (region != before) now->locals_text = region;
        return;
    }
    auto afind = [&](const std::string& n) -> D* {
        for (auto& x : arrays)
            if (x.first == n) return &x.second;
        return nullptr;
    };
    auto snapshot = arrays;
    for (auto& ad : snapshot) {
        const std::string& owner = ad.first;
        D a = *afind(owner);
        int64_t mx = 2;
        for (auto& x : arrays)
            if (x.first != owner && x.second.off < a.off) mx = std::max(mx, x.second.off);
        int64_t need = a.off - mx;
        if (a.size < need && need <= 0x400) {
            std::string newline = resized(a.line, (need + elem16(a.typ) - 1) / elem16(a.typ));
            replace(a.line, newline);
            *afind(owner) = {a.off, need, true, newline, a.typ};
        }
    }
    auto byname = decl;
    std::sort(byname.begin(), byname.end(), [](const std::pair<std::string, D>& a, const std::pair<std::string, D>& b) { return a.first < b.first; });
    for (auto& nd : byname) {
        const std::string& name = nd.first;
        const D& d = nd.second;
        if (afind(name) || lv.occurs(name).empty()) continue;
        std::string owner;
        for (auto& x : arrays)
            if (x.second.off > d.off && d.off >= x.second.off - x.second.size) {
                owner = x.first;
                break;
            }
        if (owner.empty()) continue;
        D a = *afind(owner);
        int64_t atb = a.off - d.off;
        replace(d.line, "");
        std::string t = d.typ.empty() ? "int2" : d.typ;
        auto place = [t, owner, atb]() { return "(" + pops(t + " *") + ")((uint1 *)" + local_text(owner) + " + 0x" + pyx(atb) + ")"; };
        if (d.is_array) lv.rename(name, [place]() { return "(" + place() + ")"; });
        else lv.rename(name, [place]() { return "(*" + place() + ")"; });
        int64_t need = atb + (d.is_array ? d.size : elem16(d.typ));
        if (need > a.size) {
            std::string newline = resized(a.line, (need + elem16(a.typ) - 1) / elem16(a.typ));
            replace(a.line, newline);
            *afind(owner) = {a.off, need, true, newline, a.typ};
        }
    }
    if (arrays.size() >= 2) {
        auto ord = arrays;
        std::stable_sort(ord.begin(), ord.end(), [](const std::pair<std::string, D>& a, const std::pair<std::string, D>& b) {
            return -a.second.off < -b.second.off;
        });
        Strs fields;
        bool ok = true;
        std::optional<int64_t> atb;
        for (size_t k = 0; k < ord.size(); k++) {
            const D& d = ord[k].second;
            if (atb && *atb > d.off) fields.push_back("    uint1 codebreaker_gap" + std::to_string(k) + " [" + std::to_string(*atb - d.off) + "];");
            else if (atb && *atb < d.off) {
                ok = false;
                break;
            }
            fields.push_back("  " + py::strip(d.line));
            atb = d.off - d.size;
        }
        if (ok) {
            for (auto& x : ord) replace(x.second.line, "");
            region = "\n  _Packed struct {\n" + py::join("\n", fields) + "\n  } codebreaker_frame;\n" + (py::startswith(region, "\n") ? region.substr(1) : region);
            for (auto& x : arrays) {
                std::string n = x.first;
                lv.rename(n, [n]() { return "codebreaker_frame." + n; });
            }
        }
    }
    region += "\n";
    while (region.find("\n\n\n") != std::string::npos) region = py::replace(region, "\n\n\n", "\n\n");
    now->locals_text = region.substr(0, region.size() - 1);
}

// ---------------------------------------------------------------------------------------------- far reads, near to far
static bool far_place(const V& x) {
    static const re::Regex FP("\\(\\(\\w+ __far \\*\\)(?:MK_FP\\(.*\\)|\\(.*\\))\\)", re::S), FC("\\w+ __far \\*");
    auto it = now->whole.find(id(x));
    if (it != now->whole.end()) return (bool)FP.fullmatch(py::strip(it->second(x)));
    if (x.get("node") == "Paren") {
        const V& inner = at(x, "x");
        return inner.get("node") == "Cast" && !in_whole(inner) && (bool)FC.fullmatch(cast_type(inner));
    }
    return false;
}

void p_far_reads(Nodes& nodes, const std::set<std::string>&) {
    static const re::Regex NEAR("(\\w+(?: \\w+)?(?: ?\\*)*?) ?\\*");
    ParentMap parent = parents(nodes);
    for (auto& np : nodes) {
        const V& e = np.e;
        const V& p = np.p;
        if (at(e, "node") != "Cast" || !mark(e).empty() || !x_unary_parent(e, parent, "*") || x_arrow_of(p, parent) || !x_star_as_read(p, parent)) continue;
        if (now->ss_casts.count(id(e))) {
            if (now->has("far_addresses")) now->far_read.insert(id(e));
            continue;
        }
        if (in_whole(e)) continue;
        std::string ty = cast_type(e);
        re::Match near = NEAR.fullmatch(ty);
        if (!near || ty.find("__far") != std::string::npos || !far_place(at(e, "x"))) continue;
        if (near.group(1) + " *" == ty)
            now->marks[id(e)] = "_far";
        else {
            size_t k = near.group(1).size();
            std::string t = ty.substr(0, k) + " __far" + ty.substr(k);
            rewrite(e, [t](const V& c) { return "(" + pops(t) + ")" + x_written(at(c, "x"), &c); });
        }
    }
}

static bool parenthesized(const std::string& t0) {
    std::string t = py::strip(t0);
    if (!py::startswith(t, "(") || !py::endswith(t, ")")) return false;
    int depth = 0;
    for (size_t k = 0; k < t.size(); k++) {
        if (t[k] == '(') depth++;
        else if (t[k] == ')') depth--;
        if (depth == 0) return k == t.size() - 1;
    }
    return false;
}

void p_near_to_far(Nodes& nodes, const std::set<std::string>&) {
    static const re::Regex FAR("(\\w+(?: \\w+)?) __far \\*");
    for (auto& np : nodes) {
        const V& e = np.e;
        if (at(e, "node") != "Cast" || in_whole(e)) continue;
        re::Match far = FAR.fullmatch(cast_type(e));
        if (!far) continue;
        std::string inner = x_written(at(e, "x"), &e);
        if (!parenthesized(inner) || !x_near_number(inner)) continue;
        std::string t = far.group(1);
        rewrite(e, [t](const V& c) { return "(" + pops(cast_type(c)) + ")(" + pops(t + " *") + ")" + x_written(at(c, "x"), &c); });
    }
}

// ---------------------------------------------------------------------------------------------- the 32-bit path's own
static bool ends_statement(const V& e0, const ParentMap& parent) {
    V e = e0;
    V up = parent_of(parent, e);
    while (!up.is_none() && up.get("node") == "Assign" && same(at(up, "value"), e)) {
        e = up;
        V u2 = parent_of(parent, up);
        up = u2;
    }
    return !up.is_none() && up.get("node") == "ExprStmt" && same(at(up, "expr"), e);
}

void p_code_pointers(Nodes& nodes, const std::set<std::string>&) {
    const V& functions = now->facts.get("functions");
    if (!py::truthy(functions)) return;
    ParentMap parent = parents(nodes);
    for (auto& np : nodes) {
        const V& e = np.e;
        if (at(e, "node") == "Assign" && !py::truthy(e.get("op")) && at(e, "value").get("node") == "Name" && !in_whole(at(e, "value")) &&
            py::contains(functions, V(name_of(at(e, "value")))) && ends_statement(e, parent))
            rewrite(at(e, "value"), [](const V& x) { return "(code *)" + name_of(x); });
    }
}

static std::string raw_now(const V& x) {
    now->bare.insert(id(x));
    struct Done {
        Id i;
        ~Done() { now->bare.erase(i); }
    } done{id(x)};
    return expr(x);
}

void p_compare_as_numbers(Nodes& nodes, const std::set<std::string>&) {
    static const re::Regex PL("[A-Za-z_][\\w ]*?\\*+\\s*(\\w+)(?: \\[\\d+\\])?");
    const V& doc = now->doc;
    std::set<std::string> ptrs;
    for (auto& v : now->locals) {
        std::string n = sym_name(v);
        V nv = sym_name_v(v);
        if (!now->drop.count(nv.is_none() ? std::string("\x01None") : n)) {
            re::Match m = PL.fullmatch(decl_now(v));
            if (m) ptrs.insert(m.group(1));
        }
    }
    for (auto& p : stmts(py::truthy(doc.get("params")) ? doc.get("params") : V::list())) {
        std::string pn = param_name(p);
        if (re::rx("\\*+\\s*" + re::escape(pn) + "$").search(type_text(p.get("type"), pn))) ptrs.insert(pn);
    }
    // operand: 0 None, 1 '' (cast to a number), 2 a cast to a pointer type, 3 a variable
    struct Opd {
        int kind;
        std::string name;
    };
    std::function<Opd(const V&)> operand = [&](const V& n) -> Opd {
        if (in_whole(n)) return {0, ""};
        if (n.get("node") == "Cast") {
            std::string ct = cast_type(n);
            if (ct == "int4" || ct == "uint4") {
                const V& v = at(n, "x");
                return v.get("node") != "Cast" && operand(v).kind != 0 ? Opd{1, ""} : Opd{0, ""};
            }
        }
        if (n.get("node") == "Cast" && py::endswith(py::replace(cast_type(n), " ", ""), "*")) return {2, ""};
        if (n.get("node") == "Name" && !field_sym(n)) return {3, name_of(n)};
        return {0, ""};
    };
    IdSet seen = seen_now();
    for (auto& np : nodes) {
        const V& e = np.e;
        const V& op = e.get("op");
        if (at(e, "node") != "Binary" ||
            !(op == "<=" || op == ">=" || op == "<" || op == ">" || op == "==" || op == "!=") || in_whole(e) || !seen.count(id(e)))
            continue;
        V sides[2] = {at(e, "left"), at(e, "right")};
        Opd names[2] = {operand(sides[0]), operand(sides[1])};
        bool has_none = names[0].kind == 0 || names[1].kind == 0;
        bool has_cast = names[0].kind == 2 || names[1].kind == 2;
        bool has_empty = names[0].kind == 1 || names[1].kind == 1;
        if (has_none || (has_cast && !has_empty)) continue;
        bool ptr[2];
        for (int k = 0; k < 2; k++) ptr[k] = names[k].kind == 2 || (names[k].kind == 3 && ptrs.count(names[k].name));
        if (ptr[0] != ptr[1])
            for (int k = 0; k < 2; k++)
                if (ptr[k]) rewrite(sides[k], [](const V& x) { return "(uint4)" + raw_now(x); });
    }
}

void p_keep_frame(Nodes& nodes, const std::set<std::string>&) {
    static const re::Regex FL("(?m)^  ([A-Za-z_][\\w ]*?(?:\\s*\\*+\\s*|\\s))([a-z]+Stack_([0-9a-f]+))(?: \\[(\\d+)\\])?;[ \\t]*$"),
        AMP("&\\s*\\Z");
    std::vector<V> locs = live_locals();
    if (locs.empty()) return;
    Locals lv(nodes, locs);
    std::string region = lv.region;
    auto found = FL.finditer(region);
    if (found.empty()) return;
    std::set<std::string> names;
    for (auto& m : found) names.insert(m.group(2));
    IdSet seen = now->seen;
    bool esp = false;
    for (auto& np : nodes)
        if (at(np.e, "node") == "Name" && name_of(np.e) == ENTRY_SP && seen.count(id(np.e))) esp = true;
    if (!esp)
        for (auto& kv : now->shown.items)
            if (kv.second.find(ENTRY_SP) != std::string::npos) esp = true;
    ParentMap parent = parents(nodes);
    auto addressed = [&](const V& e) {
        if (x_unary_parent(e, parent, "&")) return true;
        const V& up = parent_of(parent, e);
        if (!up.is_none() && in_whole(up)) {
            Opt pre = prefix_of(e);
            return pre && !pre->empty() && (bool)AMP.search(*pre);
        }
        return false;
    };
    bool any_arr = false;
    for (auto& m : found) any_arr = any_arr || (m.has(4) && !m.group(4).empty());
    bool any_addr = false;
    if (!esp && !any_arr)
        for (auto& np : nodes)
            if (at(np.e, "node") == "Name" && names.count(name_of(np.e)) && seen.count(id(np.e)) && addressed(np.e)) {
                any_addr = true;
                break;
            }
    if (!(esp || any_arr || any_addr)) return;
    V sizes = dict_or_empty(now->facts.get("struct_sizes"));
    struct Item {
        int64_t off, size;
        std::string ty, name, arr;
        bool has_arr;
    };
    std::vector<Item> items;
    for (auto& m : found) {
        std::string ty = py::join(" ", py::split_ws(m.group(1)));
        int64_t size;
        if (py::endswith(ty, "*")) size = 4;
        else {
            auto it = SIZEOF.find(ty);
            if (it != SIZEOF.end()) size = it->second;
            else {
                const V& s = sizes.get(V(ty));
                if (!py::truthy(s)) return;
                size = s.as_int();
            }
        }
        bool has = m.has(4) && !m.group(4).empty();
        int64_t count = has ? py::int_parse(m.group(4), 10) : 1;
        items.push_back({py::int_parse(m.group(3), 16), size * count, ty, m.group(2), has ? m.group(4) : "", has});
    }
    std::sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
        if (a.off != b.off) return a.off > b.off;
        if (a.size != b.size) return a.size > b.size;
        if (a.ty != b.ty) return a.ty > b.ty;
        return a.name > b.name;
    });
    Strs fields;
    std::optional<int64_t> pos;
    for (auto& it : items) {
        int64_t addr = -it.off;
        if (pos) {
            int64_t gap = addr - *pos;
            if (gap < 0) return;
            if (gap) fields.push_back("uint1 _gap_" + pyx(it.off) + "[" + std::to_string(gap) + "];");
        }
        fields.push_back(it.ty + " " + it.name + (it.has_arr ? "[" + it.arr + "]" : std::string()) + ";");
        pos = addr + it.size;
    }
    if (pos && *pos < 0) fields.push_back("uint1 _rest[" + std::to_string(-*pos) + "];");
    std::string decl = "  struct { " + py::join(" ", fields) + " } _f;   /* locals at their offsets in the original's frame */";
    bool first = true;
    now->locals_text = FL.sub_fn([&](const re::Match&) {
        if (first) {
            first = false;
            return decl;
        }
        return std::string();
    }, region);
    now->frame = names;
    if (esp) now->frame.insert(ENTRY_SP);
}

static bool read_through(const V& u, const ParentMap& parent) {
    static const char* PRIM[] = {"char", "short", "int", "long", "unsigned", "signed", "double", "float", "void", "bool"};
    const V& up = parent_of(parent, u);
    if (!up.is_none() && up.get("node") == "Cast" && same(up.get("x"), u) && !in_whole(up)) {
        std::string ty = cast_type(up);
        if (ty.find('*') != std::string::npos) return true;
        auto ws = py::split_ws(ty);
        if (ws.empty()) py::raise("IndexError", "list index out of range");
        for (auto p : PRIM)
            if (ws.back() == p) return true;
        return false;
    }
    return true;
}

static std::string global_now(const std::string& name) {
    auto it = now->alias.find(name);
    return it == now->alias.end() ? name : it->second;
}

void p_globals_through(Nodes& nodes, const std::set<std::string>&) {
    static const re::Regex RAM("([a-z]+)Ram([0-9a-f]{8})");
    const V& called = now->facts.get("called_through");
    if (called.is_none()) return;
    ParentMap parent = parents(nodes);
    for (auto& np : nodes) {
        const V& e = np.e;
        const V& p = np.p;
        if (at(e, "node") != "Name" || in_whole(e) || p.is_none() || in_whole(p)) continue;
        std::string nm = name_of(e);
        if (!RAM.fullmatch(nm) || py::contains(called, V(nm)) || (nm[0] != 'i' && nm[0] != 'u' && nm[0] != 'x')) continue;
        std::string kind = nm[0] == 'i' ? "int4" : nm[0] == 'u' ? "uint4" : "xunknown4";
        if (p.get("node") == "Unary" && p.get("op") == "*" && !py::truthy(p.get("postfix")) && read_through(p, parent))
            rewrite(e, [kind](const V& x) { return "(" + kind + " *)" + global_now(name_of(x)); });
        else if (p.get("node") == "Index" && same(p.get("arr"), e))
            rewrite(e, [kind](const V& x) { return "((" + kind + " *)" + global_now(name_of(x)) + ")"; });
    }
}

void p_same_cast(Nodes& nodes, const std::set<std::string>&) {
    static const re::Regex CASTED("\\(\\s*(\\w+)\\s*\\)(.*)", re::S), INT("u?int\\d"), VAL("[\\w.>\\-]+");
    const V& pointers = now->facts.get("pointer_globals");
    if (!py::truthy(pointers)) return;
    for (auto& np : nodes) {
        const V& e = np.e;
        const V& op = e.get("op");
        if (at(e, "node") != "Binary" || !(op == "<=" || op == ">=" || op == "<" || op == ">" || op == "==" || op == "!=") || in_whole(e)) continue;
        std::pair<V, V> pairs[2] = {{at(e, "right"), at(e, "left")}, {at(e, "left"), at(e, "right")}};
        for (auto& go : pairs) {
            const V &gg = go.first, &other = go.second;
            if (gg.get("node") != "Name" || in_whole(gg) || !py::contains(pointers, V(name_of(gg)))) continue;
            Opt ty, value;
            if (in_whole(other)) {
                re::Match m = CASTED.fullmatch(expr(other));
                if (m) {
                    ty = m.group(1);
                    value = m.group(2);
                }
            } else if (other.get("node") == "Cast") {
                ty = cast_type(other);
                value = expr(at(other, "x"));
            }
            if (ty && !ty->empty() && INT.fullmatch(*ty) && VAL.fullmatch(*value)) {
                std::string t = *ty;
                rewrite(gg, [t](const V& x) { return "(" + t + ")" + name_of(x); });
                break;
            }
        }
    }
}

void p_fit_calls(Nodes& nodes, const std::set<std::string>&) {
    const V& takes = now->facts.get("takes");
    if (!py::truthy(takes)) return;
    for (auto& np : nodes) {
        const V& e = np.e;
        if (at(e, "node") != "Call" || in_whole(e)) continue;
        const V& t = e.get("target");
        if (!t.is_none() && t.get("node") == "Name" && !in_whole(t) && takes.has(V(name_of(t))) &&
            !py::eq(V((long long)(py::truthy(e.get("args")) ? e.get("args").size() : 0)), takes.get(V(name_of(t)))))
            rewrite(t, [](const V& x) { return "((int4 (*)())" + name_of(x) + ")"; });
    }
}

// ---------------------------------------------------------------------------------------------- structures (struct_text's plan)
std::pair<Opt, int64_t> struct_kind(const std::string& base, const std::string& stars) {
    static const std::map<std::string, int64_t> SIZE{{"int1", 1}, {"uint1", 1}, {"xunknown1", 1}, {"undefined1", 1}, {"undefined", 1},
                                                     {"bool", 1}, {"char", 1}, {"int2", 2}, {"uint2", 2}, {"xunknown2", 2},
                                                     {"undefined2", 2}, {"int4", 4}, {"uint4", 4}, {"xunknown4", 4},
                                                     {"undefined4", 4}, {"float4", 4}, {"int", 4}, {"int8", 8}, {"uint8", 8},
                                                     {"xunknown8", 8}, {"undefined8", 8}, {"float8", 8}};
    if (stars.size() > 1) return {base + " " + std::string(stars.size() - 1, '*'), 4};
    auto it = SIZE.find(base);
    if (it != SIZE.end()) return {base, it->second};
    return {std::nullopt, 0};
}

struct Access {
    std::string param, base, stars;
    int64_t off;
};
std::optional<Access> struct_access(const V& u, const V& params) {
    if (u.get("node") != "Unary" || u.get("op") != "*" || py::truthy(u.get("postfix")) || at(u, "x").get("node") != "Cast") return std::nullopt;
    const V& inner = at(at(u, "x"), "x");
    std::string p;
    if (inner.get("node") == "Name") p = name_of(inner);
    else if (inner.get("node") == "Binary" && inner.get("op") == "+" && at(inner, "left").get("node") == "Name" && at(inner, "right").get("node") == "Lit")
        p = name_of(at(inner, "left"));
    else
        return std::nullopt;
    if (!py::contains(params, V(p))) return std::nullopt;
    std::string q = re::escape(p);
    re::Match m = re::rx("\\((\\w+) ?(\\*+)\\)(?:\\(" + q + " \\+ (0x[0-9a-fA-F]+|\\d+)\\)|" + q + ")").fullmatch(expr(at(u, "x")));
    if (!m) return std::nullopt;
    return Access{p, m.group(1), m.group(2), m.has(3) && !m.group(3).empty() ? py::int_parse(m.group(3), 0) : 0};
}

void p_struct_fields(Nodes& nodes, const std::set<std::string>&) {
    V planned = now->facts.get("struct_fields");             // (a copy: the facts grow below)
    if (!py::truthy(planned)) return;
    V used = now->facts.setdefault(V("structs_used"), V::set());
    for (auto& np : nodes) {
        const V& e = np.e;
        if (in_whole(e)) continue;
        auto a = struct_access(e, planned);
        if (!a) continue;
        const V& plan = planned.at(V(a->param));
        V sname = plan[0], fields = plan[1];
        auto k = struct_kind(a->base, a->stars);
        const V& f = fields.get(V(a->off));
        V first = f.is_none() ? V() : f[0];
        if (!k.first || first != V(*k.first)) continue;
        py::set_add(used, sname);
        std::string s = py::str_of(sname), p = a->param;
        int64_t o = a->off;
        rewrite(e, [s, p, o](const V&) { return "((" + s + " *)" + p + ")->f_" + pyx(o); });
    }
}

// ---------------------------------------------------------------------------------------------- PowerPC
void p_wide_address(Nodes& nodes, const std::set<std::string>&) {
    static const re::Regex R("0xffffffff([89a-f][0-9a-f]{7})");
    for (auto& np : nodes) {
        const V& e = np.e;
        if (at(e, "node") == "Lit" && !in_whole(e)) {
            re::Match m = R.fullmatch(lit(e));
            if (m) {
                std::string a = m.group(1);
                rewrite(e, [a](const V&) { return "0x" + a; });
            }
        }
    }
}

void p_homed_arguments(Nodes& nodes, const std::set<std::string>&) {
    static const re::Regex HOME("_?([a-z]+)Stack(000000[0-9a-f]{2})");
    const V& doc = now->doc;
    Strs names;
    for (auto& p : stmts(py::truthy(doc.get("params")) ? doc.get("params") : V::list())) {
        auto ws = py::split_ws(type_text(p.get("type"), param_name(p)));
        if (ws.empty()) py::raise("IndexError", "list index out of range");
        names.push_back(py::lstrip_chars(ws.back(), "*"));
    }
    auto slot = [&](const std::string& name) -> Opt {
        re::Match m = HOME.fullmatch(name);
        if (!m) return std::nullopt;
        int64_t off = py::int_parse(m.group(2), 16);
        int64_t k = floordiv(off - 0x10, 8);
        if (0 <= k && k < (int64_t)names.size() && (off % 8 == 0 || off % 8 == 4)) return names[(size_t)k];
        return std::nullopt;
    };
    std::map<std::string, std::string> homes;
    for (auto& v : now->locals) {
        std::string n = sym_name(v);
        if (slot(n)) now->drop.insert(n);
    }
    for (auto& np : nodes)
        if (at(np.e, "node") == "Name") {
            Opt s = slot(name_of(np.e));
            if (s) homes[name_of(np.e)] = *s;
        }
    for (auto& h : homes) now->alias[h.first] = h.second;
    for (auto& np : nodes) {
        const V& e = np.e;
        V x = at(e, "node") == "ExprStmt" ? e.get("expr") : V();
        if (!x.is_none() && x.get("node") == "Assign" && !py::truthy(x.get("op")) && at(x, "target").get("node") == "Name" &&
            at(x, "value").get("node") == "Name" && !in_whole(x) && global_now(name_of(at(x, "target"))) == global_now(name_of(at(x, "value"))))
            now->gone.insert(id(e));
    }
}

}  // namespace cb
