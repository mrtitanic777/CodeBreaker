// cmodule.cc -- backends/c.py's entry points: function (one function's C, the passes it is given),
// flags, signature, and what the module decides from every function (module_names32, type_names,
// prototypes, segment_plan, caller_frame_plan, seg_layout ...), fragment and module.
#include <algorithm>
#include <chrono>

#include "cback.hpp"

namespace cb {

#ifdef CBL2_PROFILE
void prof_add(const std::string& k, double t);
#endif

const Strs MOVED{"far", "near_calls", "far_from_stack", "frame_through_ss", "pieces", "segment_registers", "null_is_zero",
                 "global_pointer_steps", "one_name_for_globals", "far_result", "model_word", "joined_arguments", "far_arguments",
                 "void_result", "byte_result", "export", "loadds", "selector_structs", "struct_gaps", "own_segment_calls",
                 "open_arguments", "interrupts", "far_addresses", "far_casts", "entry_sp", "stack_arguments", "far_locals",
                 "far_concat_casts", "buffer_fields", "far_reads", "near_to_far", "stack_segment_stores", "callee_pops",
                 "caller_frame_slots", "selector_imports", "segment_values", "code_segment_reads", "names_used", "heading_facts",
                 "int_result", "register_arguments", "code_pointers", "compare_as_numbers", "keep_frame", "globals_through",
                 "global_widths", "same_cast", "fit_calls", "struct_fields", "wide_address", "homed_arguments", "uint8_result"};
const Strs IN_MODULE{"model_word", "joined_arguments", "far_arguments", "void_result", "byte_result", "export", "loadds",
                     "selector_structs", "struct_gaps", "own_segment_calls", "open_arguments", "interrupts", "far_addresses",
                     "far_casts", "entry_sp", "stack_arguments", "far_locals", "far_concat_casts", "buffer_fields", "far_reads",
                     "near_to_far", "stack_segment_stores", "callee_pops", "caller_frame_slots", "selector_imports",
                     "segment_values", "code_segment_reads", "names_used", "heading_facts", "int_result", "register_arguments",
                     "code_pointers", "compare_as_numbers", "keep_frame", "globals_through", "global_widths", "same_cast",
                     "fit_calls", "struct_fields", "wide_address", "homed_arguments", "uint8_result", "open_arguments"};
const Strs FLAT{"pieces", "null_is_zero", "struct_gaps", "int_result", "register_arguments", "code_pointers", "compare_as_numbers",
                "keep_frame", "globals_through", "global_widths", "same_cast", "fit_calls", "struct_fields"};
const Strs FLAT_ONLY{"int_result", "register_arguments", "code_pointers", "compare_as_numbers", "keep_frame", "globals_through",
                     "global_widths", "same_cast", "fit_calls", "struct_fields", "wide_address", "homed_arguments", "uint8_result"};
const Strs POWERPC{"wide_address", "struct_gaps", "pieces", "compare_as_numbers", "code_pointers", "homed_arguments", "uint8_result",
                   "open_arguments"};

static bool in_list(const Strs& l, const std::string& x) { return std::find(l.begin(), l.end(), x) != l.end(); }


std::optional<std::vector<std::pair<std::string, std::string>>> old_style(const V& doc) {
    static const re::Regex F("__fastcall\\w+"), LASTNAME("([A-Za-z_]\\w*)\\s*(?:\\[\\d*\\])?$");
    const V& sym = doc.get("sym");
    V ft = py::truthy(sym) && py::truthy(sym.get("type")) ? sym.get("type") : V::dict();
    if (py::truthy(ft.get("variadic")) || lost(doc)) return std::nullopt;
    bool fast = false;
    if (py::truthy(doc.get("mods")))
        for (auto& m : doc.get("mods").seq()) fast = fast || F.fullmatch(py::str_of(m));
    if (fast && !type_stack(ret_of(doc)).mods.empty()) return std::nullopt;
    std::vector<std::pair<std::string, std::string>> out;
    for (auto& p : stmts(py::truthy(doc.get("params")) ? doc.get("params") : V::list())) {
        std::string pn = param_name(p);
        std::string decl = type_text(p.get("type"), pn, now->far.count(pn) != 0);
        re::Match m = LASTNAME.search(decl);
        if (decl.find('(') != std::string::npos || decl.find(')') != std::string::npos || !m) return std::nullopt;
        out.push_back({m.group(1), decl});
    }
    return out;
}

std::map<std::string, std::string> far_pointers_of(const std::vector<V>& locs) {
    static const re::Regex R("([A-Za-z_]\\w*(?: [A-Za-z_]\\w*)?)(?: __far)? \\*(\\w+)");
    std::map<std::string, std::string> out;
    for (auto& v : locs) {
        std::string n = sym_name(v);
        re::Match m = R.fullmatch(type_text(v.get("type"), n, now->far.count(n) != 0));
        if (m) out[m.group(2)] = m.group(1);
    }
    return out;
}

static std::string function_in_scope(const V& doc, const V& moved_v, const V& far, const V& facts) {
    if (doc.get("node") != "MethodDecl") throw Decline("document " + py::str_of(doc.get("node")));
    const V& origin = doc.get("origin");
    bool sixteen = py::truthy(origin) && py::contains(origin, V("segment"));
    std::vector<V> locs = locals_of(doc.get("body"));
    Strs moved;
    for (auto& p : moved_v.seq()) moved.push_back(py::str_of(p));
    if (facts.is_none()) {
        Strs kept;
        for (auto& p : moved)
            if (!in_list(IN_MODULE, p)) kept.push_back(p);
        moved = kept;
    }
    now->facts = facts.is_none() ? V::dict() : facts;
    now->doc = doc;
    now->lane = facts.is_none() ? V() : facts.get("lane");
    const Strs& flat = now->lane == "powerpc" ? POWERPC : FLAT;
    for (auto& p : moved)
        if (sixteen ? !in_list(FLAT_ONLY, p) : in_list(flat, p)) now->moved.insert(p);
    now->sixteen = sixteen;
    now->locals = locs;
    if (now->has("far")) {
        if (!far.is_none()) {
            for (auto& n : far.seq()) now->far.insert(py::str_of(n));
        } else {
            for (auto& p : stmts(py::truthy(doc.get("params")) ? doc.get("params") : V::list()))
                if (is_far(p.get("type"))) now->far.insert(param_name(p));
            for (auto& v : locs)
                if (is_far(v.get("type"))) {
                    V n = sym_name_v(v);
                    now->far.insert(n.is_none() ? std::string("\x01None") : py::str_of(n));
                }
        }
    }
    if (now->has("open_arguments")) now->old_style = old_style(doc);
    if (now->has("far_addresses")) now->far_pointers = far_pointers_of(locs);
    if (now->has("global_widths")) {
        const V& al = now->facts.get("aliases");
        if (py::truthy(al))
            for (auto& kv : al.items()) now->alias[py::str_of(kv.first)] = py::str_of(kv.second);
    }
    if (now->has("far_from_stack") && !now->far.empty()) far_from_stack(doc.get("body"), locs);
    Nodes nodes = walk(doc.get("body"));
    std::set<std::string> arrays = frame_arrays(locs);
    run_passes(nodes, arrays);
#ifdef CBL2_PROFILE
    auto t_sig = std::chrono::steady_clock::now();
    struct AddAt {
        std::chrono::steady_clock::time_point t;
        ~AddAt() { prof_add("(final text)", std::chrono::duration<double>(std::chrono::steady_clock::now() - t).count()); }
    } add_at{t_sig};
#endif
    if (!sixteen && !facts.is_none()) {
        Pretty em;
        em.open_group();
        em.tag_line();
        signature_em(&em, doc);
        em.close_group();
        facts.set("declared", V(py::join(" ", py::split_ws(em.text()))));
    }
    return function_text(doc, locs);
}

std::string function(const V& doc, const V& moved, const V& far, const V& facts) {
    Scope s;
    return function_in_scope(doc, moved, far, facts);
}

V flags(const V& doc, const V& facts0) {
    Scope s;
    for (auto& p : MOVED)
        if (!in_list(FLAT_ONLY, p)) now->moved.insert(p);
    V facts = py::truthy(facts0) ? facts0 : V::dict();
    now->facts = facts;
    now->doc = doc;
    V ret = ret_of(doc);
    Heading h = heading(doc);
    bool far_result = is_far(ret);
    bool has;
    std::string shown = result_shown_base(ret, has);
    V out = V::set();
    std::string sr = has && !shown.empty() ? shown : h.result;
    std::string first;
    if (!sr.empty()) {
        auto ws = py::split_ws(sr);
        if (ws.empty()) py::raise("IndexError", "list index out of range");
        first = ws[0];
    }
    std::string base = type_stack(ret).base;
    if (base == "void" && first == "xunknown2") py::set_add(out, V("novalue"));
    if (far_result || py::truthy(facts.get("far_return"))) py::set_add(out, V("wide"));
    if (!h.far_model) py::set_add(out, V("near"));
    else if (py::truthy(facts.get("export"))) py::set_add(out, V("exported_prologue"));
    std::string written = type_text(ret, "", far_result);
    if (has && !shown.empty()) written = shown + written.substr(std::min(written.size(), base.size()));
    Strs b{written};
    b.insert(b.end(), h.mods.begin(), h.mods.end());
    b.insert(b.end(), h.words.begin(), h.words.end());
    std::string before = py::join(" ", b);
    bool wide_first = first == "xunknown4" || first == "int4" || first == "uint4" || first == "undefined4" || first == "float4";
    if (wide_first || (before.find('*') != std::string::npos && before.find("__far") != std::string::npos)) py::set_add(out, V("wide"));
    return out;
}

std::string signature_of(const V& doc) {
    Scope s;
    return signature(doc);
}

V module_names32(const V& doc) {
    static const re::Regex RAM("([a-z]+)Ram([0-9a-f]{8})");
    Scope s;
    V names = V::set(), calls = V::set(), through = V::set(), arity = V::list();
    for (auto& np : walk(doc.get("body"))) {
        const V& e = np.e;
        if (at(e, "node") == "Name")
            py::set_add(names, V(name_of(e)));
        else if (at(e, "node") == "Call") {
            const V& t = e.get("target");
            V n;
            if (t.is_none()) {
                n = sym_name_v(e);
                if (py::truthy(n)) {
                    py::set_add(names, n);
                    py::set_add(calls, n);
                }
            } else if (t.get("node") == "Name") {
                n = V(name_of(t));
                py::set_add(calls, n);
            } else if (t.get("node") == "Select") {
                n = V(name_of(t));
            } else if (t.get("node") == "Unary" && t.get("op") == "*" && !py::truthy(t.get("postfix")) && at(t, "x").get("node") == "Name" &&
                       RAM.fullmatch(name_of(at(t, "x")))) {
                py::set_add(through, V(name_of(at(t, "x"))));
            }
            if (py::truthy(n)) arity.append(V::tuple({n, V((long long)(py::truthy(e.get("args")) ? e.get("args").size() : 0))}));
        }
    }
    V out = V::dict();
    out.set("names", names);
    out.set("calls", calls);
    out.set("called_through", through);
    out.set("arity", arity);
    return out;
}

V type_names(const V& doc) {
    V out = V::set();
    std::vector<V> todo{doc.get("ret")};
    for (auto& p : stmts(py::truthy(doc.get("params")) ? doc.get("params") : V::list())) todo.push_back(p.get("type"));
    for (auto& np : walk(doc.get("body")))
        if (at(np.e, "node") == "Local" || at(np.e, "node") == "Cast") todo.push_back(np.e.get("type"));
    while (!todo.empty()) {
        V t = todo.back();
        todo.pop_back();
        if (!t.is_dict()) continue;
        if (t.get("kind") == "class" && t.get("name").is_str()) py::set_add(out, t.get("name"));
        todo.push_back(t.get("to"));
        todo.push_back(t.get("elem"));
        V fn = t.get("kind") == "fnptr" ? t.get("fn") : V();
        if (fn.is_dict()) {
            for (auto& p : stmts(py::truthy(fn.get("params")) ? fn.get("params") : V::list())) todo.push_back(p.get("type"));
            const V& r = fn.get("result");
            todo.push_back(py::truthy(r) ? r.get("type") : V());
        }
    }
    return out;
}

V parameter_names(const V& doc) {
    Scope s;
    Strs decls;
    for (auto& p : stmts(py::truthy(doc.get("params")) ? doc.get("params") : V::list())) decls.push_back(type_text(p.get("type"), param_name(p)));
    const V& sym = doc.get("sym");
    V ft = py::truthy(sym) && py::truthy(sym.get("type")) ? sym.get("type") : V::dict();
    if (py::truthy(ft.get("variadic"))) decls.push_back("...");
    for (auto& d : decls)
        if (d.find('(') != std::string::npos || d.find(')') != std::string::npos) return V::list();
    V out = V::list();
    for (auto& d : decls) out.append(V(py::lstrip_chars(py::split_ws(py::strip(d)).back(), "*")));
    return out;
}

struct Access {
    std::string param, base, stars;
    int64_t off;
};
std::optional<Access> struct_access(const V& u, const V& params);

V struct_accesses(const V& doc, const V& param) {
    Scope s;
    V out = V::list();
    V params = V::tuple({param});
    for (auto& np : walk(doc.get("body"))) {
        auto a = struct_access(np.e, params);
        if (a) out.append(V::tuple({V(a->base), V(a->stars), V(a->off)}));
    }
    return out;
}

V takes32(const std::string& head) {
    if (head.find("__cdecl") != std::string::npos || head.find('(') == std::string::npos) return V();
    std::string inside;
    if (head.find(')') != std::string::npos) {
        size_t a = head.find('(') + 1, b = head.rfind(')');
        inside = py::strip(b >= a ? head.substr(a, b - a) : std::string());
    }
    if (inside.empty() || inside == "void") return V(0);
    return V((long long)py::count(inside, ",") + 1);
}

V words_of(const std::string& ty);
V argument_words(const std::string& arg, const V& types);

V prototypes(const V& headings, const V& order) {
    static const re::Regex HALVES("\\b(unaff_[SD]S|codebreaker_[sd]s\\(\\))|>>\\s*0x10");
    std::vector<std::pair<V, V>> params;      // name -> its params (head, [(p, t)]) or None
    auto pget = [&](const V& n) -> const V* {
        for (auto& x : params)
            if (x.first == n) return &x.second;
        return nullptr;
    };
    for (auto& n : order.seq()) {
        V h = py::truthy(headings.get(n)) ? headings.get(n) : V::dict();
        V p = h.get("params");
        bool found = false;
        for (auto& x : params)
            if (x.first == n) {
                x.second = p;
                found = true;
            }
        if (!found) params.push_back({n, p});
    }
    std::vector<std::pair<V, std::vector<V>>> want;
    for (auto& np : params) {
        const V& ps = np.second;
        if (!py::truthy(ps) || ps[0].str().find("...") != std::string::npos) continue;
        std::vector<V> ws;
        for (auto& pt : ps[1].seq()) ws.push_back(words_of(py::str_of(pt[1])));
        want.push_back({np.first, ws});
    }
    std::vector<std::pair<V, bool>> ok;
    for (auto& w : want) {
        bool all = true;
        for (auto& x : w.second) all = all && !x.is_none();
        ok.push_back({w.first, all});
    }
    auto okget = [&](const V& n) -> bool* {
        for (auto& x : ok)
            if (x.first == n) return &x.second;
        return nullptr;
    };
    auto wantget = [&](const V& n) -> std::vector<V>* {
        for (auto& x : want)
            if (x.first == n) return &x.second;
        return nullptr;
    };
    for (auto& caller : order.seq()) {
        V h = py::truthy(headings.get(caller)) ? headings.get(caller) : V::dict();
        V types = py::truthy(h.get("types")) ? h.get("types") : V::dict();
        const V& calls = h.get("calls");
        if (!py::truthy(calls)) continue;
        for (auto& call : calls.seq()) {
            V callee = call[0], args = call[1];
            bool* good = okget(callee);
            if (!good || !*good) continue;
            std::vector<V>& w = *wantget(callee);
            bool bad = args.size() != w.size();
            if (!bad)
                for (size_t k = 0; k < w.size(); k++)
                    if (argument_words(py::str_of(args[k]), types) != w[k]) {
                        bad = true;
                        break;
                    }
            if (bad) *good = false;
            else if ((*pget(callee))[0].str().find("__stdcall") != std::string::npos) {
                for (auto& a : args.seq())
                    if (HALVES.search(py::str_of(a))) {
                        *good = false;
                        break;
                    }
            }
        }
    }
    V out = V::dict();
    for (auto& x : ok) {
        if (!x.second) continue;
        const V& ps = *pget(x.first);
        Strs listed;
        for (auto& pt : ps[1].seq()) listed.push_back(py::str_of(pt[1]) + " " + py::str_of(pt[0]));
        std::string l = py::join(", ", listed);
        out.set(x.first, V(py::join(" ", py::split_ws(ps[0].str())) + "(" + (l.empty() ? "void" : l) + ");"));
    }
    return out;
}

V segment_plan(const V& functions, const V& near, const V& where, const V& sites, const V& code, const V& data_segment,
               const V& other_data, const V& first_selector) {
    int64_t fs = first_selector.as_int();
    V own = V::dict();
    for (auto& f : functions.seq()) {
        V va = f[0], n = f[1];
        const V& w = where.get(va);
        V sg = py::truthy(w) ? w[0] : V();
        if (!sg.is_none() && !py::contains(near, n)) own.setdefault(sg, n);
    }
    V sel_expr = V::dict();
    std::vector<V> segs;
    if (code.is_dict())
        for (auto& kv : code.items()) segs.push_back(kv.first);
    else if (!code.is_none())
        segs = code.seq();
    if (py::truthy(data_segment)) segs.push_back(data_segment);
    py::sort_values(segs);
    segs.erase(std::unique(segs.begin(), segs.end(), [](const V& a, const V& b) { return py::eq(a, b); }), segs.end());
    for (auto& sg : segs) {
        int64_t made_up = fs + 8 * (sg.as_int() - 1);
        if (py::eq(sg, data_segment)) sel_expr.set(V(made_up), V("codebreaker_ds()"));
        else if (own.has(sg)) sel_expr.set(V(made_up), V("((uint2)((uint4)(void __far *)&" + py::str_of(own.get(sg)) + " >> 16))"));
    }
    std::vector<V> others = other_data.is_none() ? std::vector<V>() : other_data.seq();
    py::sort_values(others);
    for (auto& sg : others)
        sel_expr.set(V(fs + 8 * (sg.as_int() - 1)),
                     V("((uint2)((uint4)(void __far *)codebreaker_segment_" + std::to_string(sg.as_int()) + " >> 16))"));
    if (!py::truthy(sel_expr)) return V::dict();
    V starts = V::dict();
    for (auto& f : functions.seq()) {
        const V& w = where.get(f[0]);
        if (py::truthy(w)) starts.setdefault(w[0], V::list()).append(w[1]);
    }
    for (auto& kv : starts.items()) py::sort_values(kv.second.seq());
    V at_place = V::dict();
    for (auto& f : functions.seq()) {
        const V& w = where.get(f[0]);
        if (py::truthy(w)) at_place.set(w, f[1]);
    }
    V plan = V::dict();
    for (auto& f : functions.seq()) {
        const V& w = where.get(f[0]);
        if (!py::truthy(w)) continue;
        V sg = w[0];
        int64_t off = w[1].as_int();
        int64_t end = 0x10000;
        for (auto& o : starts.at(sg).seq())
            if (o.as_int() > off) {
                end = o.as_int();
                break;
            }
        std::set<int64_t> wanted;
        const V& ss = sites.get(sg);
        if (py::truthy(ss))
            for (auto& ot : ss.seq()) {
                int64_t o = ot[0].as_int();
                if (off <= o && o < end) wanted.insert(fs + 8 * (ot[1].as_int() - 1));
            }
        V here = V::dict();
        for (auto& kv : sel_expr.items())
            if (wanted.count(kv.first.as_int())) here.set(kv.first, kv.second);
        if (py::truthy(here)) {
            V values = V::dict(), seg_of = V::dict(), places = V::dict();
            std::set<int64_t> segs_here;
            for (auto& kv : here.items()) {
                int64_t v = kv.first.as_int();
                values.set(V("0x" + pyx(v)), kv.second);
                seg_of.set(kv.second, V((v - fs) / 8 + 1));
                segs_here.insert((v - fs) / 8 + 1);
            }
            for (auto& kv : at_place.items())
                if (segs_here.count(kv.first[0].as_int())) places.set(kv.first, kv.second);
            V p = V::dict();
            p.set("segment_values", values);
            p.set("segment_of", seg_of);
            p.set("segment_places", places);
            plan.set(f[1], p);
        }
    }
    return plan;
}

V caller_frame_plan(const V& reports, const V& frames) {
    V plan = V::dict();
    std::map<std::string, std::string> region;
    for (auto& kv : reports.items())
        if (py::truthy(kv.second)) region[py::str_of(kv.first)] = py::str_of(kv.second.at("region"));
    for (auto& nr : reports.items()) {
        const V& name = nr.first;
        V rep = py::truthy(nr.second) ? nr.second : V::dict();
        const V& slots = rep.get("slots");
        if (!py::truthy(slots)) continue;
        std::vector<std::pair<V, V>> callers;
        for (auto& kv : reports.items())
            if (!py::eq(kv.first, name) && py::truthy(kv.second) && py::contains(kv.second.at("named"), name)) callers.push_back(kv);
        bool skip = callers.empty();
        for (auto& c : callers) {
            const V& r = c.second;
            const V& nm = r.at("named").at(name);
            if (!py::truthy(frames.get(c.first)) || py::truthy(r.at("bp")) || nm[0] != nm[1]) skip = true;
        }
        if (skip) continue;
        std::vector<std::pair<std::pair<V, V>, V>> found;
        bool ok = true;
        for (auto& c : callers) {
            const V& r = c.second;
            for (auto& lt : slots.items()) {
                V got = r.at("locals").get(V(lt.first.as_int() + 2));
                V width;
                if (py::truthy(got) && py::str_of(got[0]).find('*') != std::string::npos) width = V(2);
                else if (py::truthy(got)) {
                    auto it = SIZEOF.find(py::strip(py::str_of(got[0])));
                    width = it == SIZEOF.end() ? V() : V(it->second);
                }
                auto sz = SIZEOF.find(py::str_of(lt.second));
                if ((got.is_none() && py::truthy(r.at("arrays"))) ||
                    (!got.is_none() && (sz == SIZEOF.end() ? (py::raise("KeyError", py::repr(lt.second)), true) : width != V(sz->second))))
                    ok = false;
                found.push_back({{c.first, lt.first}, py::truthy(got) ? got[1] : V()});
            }
        }
        if (!ok) continue;
        V mine = plan.setdefault(name, V::dict());
        mine.set("frame_slots", py::copy_dict(slots));
        for (auto& c : callers) {
            const V& n = c.first;
            Strs args;
            V sorted_slots = py::sorted(slots);
            for (auto& less : sorted_slots.seq()) {
                V f;
                for (auto& x : found)
                    if (py::eq(x.first.first, n) && py::eq(x.first.second, less)) f = x.second;
                std::string local = py::truthy(f) ? py::str_of(f) : "xStack_" + pyx(less.as_int() + 2);
                std::string ty = py::str_of(slots.at(less));
                std::string& reg = region[py::str_of(n)];
                if (!re::rx("(?m)^\\s+[\\w ]+?\\**\\s*" + local + ";$").search(reg)) {
                    V pn = plan.setdefault(n, V::dict());
                    pn.setdefault(V("frame_declare"), V::list()).append(V::tuple({V(ty), V(local)}));
                    reg = "\n  " + ty + " " + local + ";" + reg;
                }
                args.push_back("(" + ty + " __far *)((uint1 __far *)MK_FP(codebreaker_ss(), &" + local + "))");
            }
            V pn = plan.setdefault(n, V::dict());
            pn.setdefault(V("frame_calls"), V::dict()).set(name, V(py::join(",", args)));
        }
    }
    return plan;
}

V seg_layout(const V& fields) {
    std::vector<std::pair<int64_t, std::string>> f;
    for (auto& kv : fields.items()) f.push_back({kv.first.as_int(), py::str_of(kv.second)});
    bool ok;
    Strs body = seg_layout_text(f, ok);
    if (!ok) return V();
    V out = V::list();
    for (auto& s : body) out.append(V(s));
    return out;
}

std::string fragment(const V& node) {
    if (node.get("node") != "MachineFragment") throw Decline("document " + py::str_of(node.get("node")));
    V o = py::truthy(node.get("origin")) ? node.get("origin") : V::dict();
    std::string where;
    if (o.has("segment")) where = " " + std::to_string(o.at("segment").as_int()) + ":" + py::hexpad((uint64_t)o.at("offset").as_int(), 4, true);
    else if (o.has("va")) where = " " + py::hexpad((uint64_t)o.at("va").as_int(), 8, true);
    std::string held = py::truthy(node.get("within")) ? " (assembled with " + py::str_of(node.get("within")) + ")" : "";
    std::string why = py::replace(py::truthy(node.get("reason")) ? py::str_of(node.get("reason")) : std::string(), "*/", "* /");
    std::string name = py::truthy(node.get("name")) ? py::str_of(node.get("name")) : "a fragment";
    return "/* " + name + where + ": kept as its instructions" + held + " -- " + why + " */\n";
}

V module(const V& doc, const V& moved) {
    if (doc.get("node") != "Module") throw Decline("document " + py::str_of(doc.get("node")));
    V units = py::truthy(doc.get("units")) ? doc.get("units") : V::list();
    std::vector<V> us = units.is_list() ? units.seq() : std::vector<V>{units};
    Strs parts;
    V asm_ = V::dict();
    for (auto& u : us) {
        if (u.get("node") == "MachineFragment") {
            parts.push_back(fragment(u));
            if (py::truthy(u.get("asm"))) asm_.set(py::truthy(u.get("name")) ? u.get("name") : V((long long)asm_.size()), u.get("asm"));
        } else if (u.get("node") == "MethodDecl" && !u.get("body").is_none()) {
            try {
                parts.push_back(function(u, moved, V(), V()));
            } catch (Decline& e) {
                parts.push_back("/* " + (py::truthy(u.get("name")) ? py::str_of(u.get("name")) : std::string("?")) + ": not written from L2 (" + e.what() + ") */\n");
            }
        } else
            parts.push_back("/* " + (py::truthy(u.get("name")) ? py::str_of(u.get("name")) : std::string("?")) +
                            ": not in L2 (its C is the decompiler's printed C) */\n");
    }
    return V::tuple({V(py::join("\n", parts)), asm_});
}

}  // namespace cb
