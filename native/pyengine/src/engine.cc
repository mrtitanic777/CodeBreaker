// engine.cc -- the whole run (see engine.hpp).
#include "engine.hpp"

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <regex>
#include <sstream>
#include <set>

#include "cpp17.hpp"
#include "json.hpp"
#include "lower.hpp"

namespace pyengine {

namespace fs = std::filesystem;

static std::string package_of(const std::string& q) {
    std::vector<std::string> parts;
    size_t a = 0;
    for (;;) {
        size_t b = q.find('.', a);
        parts.push_back(q.substr(a, b == std::string::npos ? std::string::npos : b - a));
        if (b == std::string::npos) break;
        a = b + 1;
    }
    return parts.size() > 2 ? parts[1] : "(top)";
}

std::string reason_key(const std::string& why) {
    auto c = why.find(':');
    std::string cls = why.substr(0, c), d = c == std::string::npos ? "" : why.substr(c + 1);
    auto trunc = [](const std::string& s, size_t n) { return s.size() > n ? s.substr(0, n) : s; };
    auto strip = [](std::string s) {
        size_t a = s.find_first_not_of(" \t\n");
        size_t b = s.find_last_not_of(" \t\n");
        return a == std::string::npos ? std::string() : s.substr(a, b - a + 1);
    };
    if (cls == "stdlib" || cls == "extern" || cls == "dynamic" || cls == "syntax") {
        auto p = d.find('(');
        if (p != std::string::npos) d = d.substr(0, p);
        d = strip(d);
        d = strip(std::regex_replace(d, std::regex("\\bof \\w+$"), ""));
        return cls + ": " + trunc(d, 70);
    }
    if (cls == "calls-refused") return "calls-refused";
    if (cls == "int-range") {
        d = std::regex_replace(d, std::regex(" in \\[.*"), "");
        return "int-range: " + d.substr(0, d.find(' '));
    }
    if (cls == "untyped") {
        if (d.rfind("parameter", 0) == 0)
            return std::string("untyped: parameter ") +
                   (d.find("callers pass") != std::string::npos ? "(its callers pass values of no type)" : "(no annotation, no typed call site)");
        if (d.rfind("attribute", 0) == 0) return "untyped: attribute also stored a value of no type";
        return "untyped: " + trunc(std::regex_replace(d, std::regex("[\\w.]+'?\\s*$"), ""), 50);
    }
    if (cls == "conflict") {
        d = std::regex_replace(d, std::regex("'[^']*'"), "'\xE2\x80\xA6'");
        d = std::regex_replace(d, std::regex("\\b\\d+\\b"), "N");
        return "conflict: " + trunc(d, 70);
    }
    return cls + ": " + trunc(d, 60);
}

static void write_text(const fs::path& p, const std::string& s) {
    fs::create_directories(p.parent_path());
    std::ofstream f(p, std::ios::binary);
    f << s;
}

// ---------------------------------------------------------------------------------------------- the back end
// Every translated function must be one the C++17 back end writes: a unit is written, a function the
// back end declines is refused ("backend: <why>") and taken out of the unit, a function calling a
// refused one is refused in turn ("calls-refused"), until every unit is written whole.
static void refuse(EngineResult& r, const std::string& q, const std::string& why) {
    auto it = r.fns.find(q);
    if (it == r.fns.end() || it->second.status != "translated") return;
    it->second.status = "refused";
    it->second.reason = why;
    it->second.l2.clear();
    r.translated--;
    r.translated_lines -= it->second.lines;
}

static std::string sv(const py::V& v) { return v.is_str() ? v.str() : std::string(); }

// take the MethodDecls of q (every specialized copy) out of a unit, leaving a Comment as the front end does
static bool drop_method(py::V& unit, const std::string& q, const std::string& why) {
    bool any = false;
    const py::V& types = unit.get("types");
    if (!types.is_list()) return false;
    for (auto& t : types.seq()) {
        const py::V& mem = t.get("members");
        if (!mem.is_list()) continue;
        auto& v = mem.seq();
        for (size_t i = 0; i < v.size(); i++) {
            if (sv(v[i].get("node")) != "MethodDecl") continue;
            const py::V& s = v[i].get("sym");
            if (sv(s.get("owner")) + "." + sv(s.get("name")) != q) continue;
            py::V c = py::V::dict();
            c.set("node", std::string("Comment"));
            c.set("text", "not translated: " + q.substr(q.rfind('.') + 1) + " -- " + why);
            if (any) {
                v.erase(v.begin() + i);
                i--;
            } else {
                v[i] = c;
            }
            any = true;
        }
    }
    return any;
}

static std::string mod_rel(const std::string& m) {
    std::string rel = m;
    for (char& c : rel)
        if (c == '.') c = '/';
    return rel;
}

static void backend_pass(const EngineOptions& o, EngineResult& r, const std::string& api,
                         const std::map<std::string, std::string>& judged) {
    std::map<std::string, py::V> docs;
    std::set<std::string> touched, lang_added;
    for (auto& [m, text] : r.units) {
        py::V d = py::json_parse(text);
        if (d.get("lang").is_none()) {
            d.set("lang", std::string("python"));   // (as the shim does)
            lang_added.insert(m);
        }
        docs[m] = d;
    }
    auto module_of = [&](const std::string& q) -> std::string {
        std::string best;
        for (auto& [m, d] : docs)
            if (q.size() > m.size() && q.compare(0, m.size(), m) == 0 && q[m.size()] == '.' && m.size() > best.size()) best = m;
        return best;
    };
    auto translated_in = [&](const std::string& m) {
        std::vector<std::string> v;
        std::string pre = m + ".";
        for (auto it = r.fns.lower_bound(pre); it != r.fns.end() && it->first.compare(0, pre.size(), pre) == 0; ++it)
            if (it->second.status == "translated" && module_of(it->first) == m) v.push_back(it->first);
        return v;
    };
    std::map<std::string, py::V> written;
    // what the judge found does not compile or does not behave as the Python (hints/refuse.json)
    std::vector<std::string> pending;
    for (auto& [q, why] : judged) {
        auto it = r.fns.find(q);
        if (it == r.fns.end() || it->second.status != "translated") continue;
        std::string m = module_of(q);
        refuse(r, q, "judge: " + why);
        if (!m.empty() && drop_method(docs[m], q, "judge: " + why)) touched.insert(m);
        pending.push_back(q);
    }
    for (bool changed = true; changed;) {
        changed = false;
        std::vector<std::string> refused_now;
        refused_now.swap(pending);
        for (auto& [m, doc] : docs) {
            if (translated_in(m).empty()) {
                written.erase(m);
                continue;
            }
            for (;;) {
                try {
                    written[m] = cpp17::write_unit(doc, api);
                    break;
                } catch (const cpp17::Decline& e) {
                    std::string why = std::string("backend: ") + e.what();
                    if (!e.where.empty() && r.fns.count(e.where) && drop_method(doc, e.where, why)) {
                        touched.insert(m);
                        refuse(r, e.where, why);
                        refused_now.push_back(e.where);
                    } else {
                        for (auto& q : translated_in(m)) {   // (not one function's: the unit's own)
                            refuse(r, q, why);
                            refused_now.push_back(q);
                        }
                        written.erase(m);
                        break;
                    }
                    changed = true;
                } catch (const std::exception& e) {
                    for (auto& q : translated_in(m)) refuse(r, q, std::string("backend: ") + e.what());
                    written.erase(m);
                    break;
                }
            }
        }
        // calls to what is refused now
        for (auto& q : refused_now) {
            auto dot = q.rfind('.');
            std::string needle = "\"name\": \"" + q.substr(dot + 1) + "\", \"owner\": \"" + q.substr(0, dot) + "\"";
            for (auto& [c, st] : r.fns) {
                if (st.status != "translated" || c == q || st.l2.find(needle) == std::string::npos) continue;
                std::string why = "calls-refused: " + q + " (" + r.fns[q].reason + ")";
                std::string cm = module_of(c);
                refuse(r, c, why);
                if (!cm.empty() && drop_method(docs[cm], c, why)) touched.insert(cm);
                changed = true;
            }
        }
    }
    // a module none of whose functions translate is still written when it can be: others include its
    // header for its records, classes and names
    for (auto& [m, doc] : docs) {
        if (o.cpp.empty() || written.count(m)) continue;
        try {
            written[m] = cpp17::write_unit(doc, api);
        } catch (const std::exception&) {
        }
    }
    for (auto& m : touched) {
        if (lang_added.count(m)) docs[m].erase(py::V(std::string("lang")));
        r.units[m] = py::json_dump(docs[m]);
    }
    if (o.cpp.empty()) return;
    py::V spans = py::V::dict();
    for (auto& [m, out] : written) {
        write_text(fs::path(o.cpp) / (mod_rel(m) + ".hpp"), out[0].str());
        write_text(fs::path(o.cpp) / (mod_rel(m) + ".cpp"), out[1].str());
        spans.set(m, out[2]);
    }
    write_text(fs::path(o.cpp) / "spans.json", py::json_dump(spans));
}

int run_engine(const EngineOptions& o, EngineResult& r) {
    auto t0 = std::chrono::steady_clock::now();
    fs::path root = fs::absolute(fs::path(o.root)).make_preferred();
    std::string rs = root.string();
    while (rs.size() > 3 && (rs.back() == '\\' || rs.back() == '/')) rs.pop_back();
    std::string package = o.package.empty() ? fs::path(rs).filename().string() : o.package;
    Program p;
    p.extended = o.extended;
    p.int_policy = o.policy;
    if (o.log) fprintf(stderr, "loading %s\n", rs.c_str());
    p.add_tree(rs, package);
    if (o.log) fprintf(stderr, "indexing %zu modules\n", p.modules.size());
    p.index();
    // (extended) the hints: hand-written parameter types, and functions the judge refused
    std::map<std::string, std::string> judged_refused;
    if (o.extended) {
        fs::path hd = o.hints.empty() ? root.parent_path() / "native" / "pyengine" / "hints" : fs::path(o.hints);
        auto load = [&](const char* name) {
            std::ifstream f(hd / name, std::ios::binary);
            if (!f) return py::V();
            std::stringstream ss;
            ss << f.rdbuf();
            return py::json_parse(ss.str());
        };
        py::V th = load("types.json");
        if (th.is_dict())
            for (auto& [q, ps] : th.items())
                if (ps.is_dict())
                    for (auto& [name, text] : ps.items())
                        if (name.is_str() && text.is_str() && name.str().rfind("#", 0) != 0) p.type_hints[q.str()][name.str()] = text.str();
        for (std::string f : {std::string("refuse.json"), "refuse." + o.policy + ".json"}) {   // (both policies; this one's)
            py::V rf = load(f.c_str());
            if (rf.is_dict())
                for (auto& [q, why] : rf.items())
                    if (why.is_str()) judged_refused[q.str()] = why.str();
        }
    }
    if (o.extended && !o.stubs.empty()) {   // (new) --stubs: <module>.pyi files, read as parameter hints
        std::vector<fs::path> files;
        for (auto& e : fs::directory_iterator(fs::path(o.stubs)))
            if (e.is_regular_file() && e.path().extension() == ".pyi") files.push_back(e.path());
        std::sort(files.begin(), files.end());
        for (auto& fpath : files) {
            std::ifstream f(fpath, std::ios::binary);
            std::stringstream ss;
            ss << f.rdbuf();
            p.load_stub(fpath.stem().string(), ss.str());
        }
    }
    // (extended) the functions a native format answers, typed by its questions.cc (answers.cc)
    if (o.extended) bind_native_answers(p, (root.parent_path() / "native" / "formats").string(), o.log ? stderr : nullptr);
    if (o.log) fprintf(stderr, "solving %zu functions, %zu nested\n", p.fns.size(), p.nested.size());
    r.rounds = p.solve(40, o.log);
    auto t1 = std::chrono::steady_clock::now();
    TranslateResult tr = translate(p, o.policy, o.modules.empty() ? nullptr : &o.modules);
    auto t2 = std::chrono::steady_clock::now();
    r.seconds_solve = std::chrono::duration<double>(t1 - t0).count();
    r.seconds_lower = std::chrono::duration<double>(t2 - t1).count();
    r.modules = p.modules.size();
    r.stub_files = (size_t)p.stub_files;
    for (auto& n : p.stub_notes) {
        if (n.status == "applied") r.stub_applied++;
        else if (n.status == "agree") r.stub_agree++;
        else if (n.status == "conflict") r.stub_conflicts++;
        else r.stub_ignored++;
    }
    for (auto& [q, f] : p.fns) {
        FnStatus st;
        st.lines = f->lines;
        {   // the ranges the callers give its int parameters (what the translation may assume: the judge's inputs)
            auto pi = p.params.find(q);
            if (pi != p.params.end()) {
                py::V rg = py::V::dict();
                for (auto& [name, t] : pi->second)
                    if (t.kind == TK::Int) rg.set(name, py::V::list({py::V(bstr(t.lo())), py::V(bstr(t.hi()))}));
                if (rg.size()) st.ranges = py::json_dump(rg);
            }
        }
        auto it = tr.lowered.find(q);
        if (it != tr.lowered.end()) {   // (the report's blockers: every issue, and the calls)
            std::set<std::string> seen;
            for (auto& i : it->second.all)
                if (seen.insert(i.cls + ":" + i.detail).second) st.all.push_back(i.cls + ":" + i.detail);
            if (it->second.refused && seen.insert(it->second.refused->issue.reason()).second) st.all.push_back(it->second.refused->issue.reason());
            st.calls.assign(it->second.calls.begin(), it->second.calls.end());
        }
        auto so = p.specs_of.find(q);
        if ((it == tr.lowered.end() || !it->second.md) && so != p.specs_of.end()) {   // (new) translated as its overloads
            bool all = true;
            std::string l2, why;
            for (Fn* c : so->second) {
                auto ci = tr.lowered.find(c->qname);
                if (ci == tr.lowered.end() || !ci->second.md) {
                    all = false;
                    if (why.empty() && ci != tr.lowered.end() && ci->second.refused) why = ci->second.refused->issue.reason();
                } else {
                    l2 += (l2.empty() ? "" : "\n") + py::json_dump(to_json(ci->second.md));
                }
            }
            if (all) {
                st.status = "translated";
                st.l2 = l2;
                r.translated++;
                r.translated_lines += f->lines;
                r.functions++;
                r.lines += f->lines;
                r.fns[q] = std::move(st);
                continue;
            }
        }
        if (it != tr.lowered.end() && it->second.md) {
            st.status = "translated";
            st.l2 = py::json_dump(to_json(it->second.md));
            r.translated++;
            r.translated_lines += f->lines;
        } else {
            st.status = "refused";
            st.reason = (it != tr.lowered.end() && it->second.refused) ? it->second.refused->issue.reason() : "frontend-error:not lowered";
        }
        r.functions++;
        r.lines += f->lines;
        r.fns[q] = std::move(st);
    }
    for (auto& [m, mr] : tr.modules)
        if (mr.unit) r.units[m] = py::json_dump(to_json(mr.unit));
    if (o.extended && o.backend) backend_pass(o, r, o.api.empty() ? (root / "java" / "api_maps").string() : o.api, judged_refused);
    if (!o.out.empty()) {
        fs::path out(o.out);
        for (auto& [m, text] : r.units) write_text(out / "l2" / (m + ".json"), text);
        py::V fns = py::V::dict();
        for (auto& [q, st] : r.fns) {
            py::V d = py::V::dict();
            d.set("status", st.status);
            d.set("reason", st.reason);
            d.set("lines", st.lines);
            d.set("package", package_of(q));
            if (!st.l2.empty()) d.set("l2", st.l2);
            if (!st.ranges.empty() && st.status == "translated") d.set("ranges", py::json_parse(st.ranges));
            if (st.status != "translated") {   // (the blockers: every one, and the calls whose refusal cascades)
                py::V all = py::V::list(), calls = py::V::list();
                for (auto& x : st.all) all.append(py::V(x));
                for (auto& x : st.calls) calls.append(py::V(x));
                d.set("all", all);
                d.set("calls", calls);
            }
            fns.set(q, d);
        }
        py::V doc = py::V::dict();
        doc.set("policy", o.policy);
        doc.set("extended", o.extended);
        doc.set("rounds", r.rounds);
        doc.set("functions", fns);
        write_text(out / "coverage.json", py::json_dump(doc));
        write_text(out / "coverage.md", coverage_markdown(r));
        if (!o.stubs.empty()) {   // (new) what each stub parameter became: applied | agree | conflict | ignored, and why
            py::V notes = py::V::list({});
            for (auto& n : p.stub_notes) {
                py::V d = py::V::dict();
                d.set("function", n.fn);
                d.set("parameter", n.param);
                d.set("stub", n.stub);
                d.set("status", n.status);
                d.set("detail", n.detail);
                notes.append(d);
            }
            py::V rep = py::V::dict();
            rep.set("stub_files", (long long)r.stub_files);
            rep.set("applied", (long long)r.stub_applied);
            rep.set("agree", (long long)r.stub_agree);
            rep.set("conflicts", (long long)r.stub_conflicts);
            rep.set("ignored", (long long)r.stub_ignored);
            rep.set("notes", notes);
            write_text(out / "stubs_report.json", py::json_dump(rep));
        }
    }
    return 0;
}

std::string coverage_markdown(const EngineResult& r) {
    struct Row {
        long functions = 0, lines = 0, translated = 0, tlines = 0;
    };
    std::map<std::string, Row> rows;
    std::map<std::string, std::pair<long, long>> classes;
    std::map<std::string, long> reasons;
    for (auto& [q, st] : r.fns) {
        Row& row = rows[package_of(q)];
        row.functions++;
        row.lines += st.lines;
        if (st.status == "translated") {
            row.translated++;
            row.tlines += st.lines;
        } else {
            std::string c = st.reason.substr(0, st.reason.find(':'));
            classes[c].first++;
            classes[c].second += st.lines;
            reasons[reason_key(st.reason)]++;
        }
    }
    auto pct = [](long a, long b) {
        char buf[32];
        if (!b) return std::string("-");
        snprintf(buf, sizeof buf, "%.1f%%", 100.0 * a / b);
        return std::string(buf);
    };
    std::vector<std::pair<std::string, Row>> sorted(rows.begin(), rows.end());
    std::stable_sort(sorted.begin(), sorted.end(), [](auto& a, auto& b) { return a.second.lines > b.second.lines; });
    std::string s = "| package | functions | lines | translated | % lines |\n|---|---|---|---|---|\n";
    Row tot;
    for (auto& [pkg, row] : sorted) {
        s += "| " + pkg + " | " + std::to_string(row.functions) + " | " + std::to_string(row.lines) + " | " + std::to_string(row.translated) +
             " | " + pct(row.tlines, row.lines) + " |\n";
        tot.functions += row.functions;
        tot.lines += row.lines;
        tot.translated += row.translated;
        tot.tlines += row.tlines;
    }
    s += "| **all** | " + std::to_string(tot.functions) + " | " + std::to_string(tot.lines) + " | " + std::to_string(tot.translated) + " | " +
         pct(tot.tlines, tot.lines) + " |\n\n| refusal class | functions | lines |\n|---|---|---|\n";
    std::vector<std::pair<std::string, std::pair<long, long>>> cs(classes.begin(), classes.end());
    std::stable_sort(cs.begin(), cs.end(), [](auto& a, auto& b) { return a.second.first > b.second.first; });
    for (auto& [c, n] : cs) s += "| " + c + " | " + std::to_string(n.first) + " | " + std::to_string(n.second) + " |\n";
    s += "\n| top refusal reasons | functions |\n|---|---|\n";
    std::vector<std::pair<std::string, long>> rs(reasons.begin(), reasons.end());
    std::stable_sort(rs.begin(), rs.end(), [](auto& a, auto& b) { return a.second > b.second; });
    for (size_t i = 0; i < rs.size() && i < 40; i++) {
        std::string k = rs[i].first;
        std::replace(k.begin(), k.end(), '|', '/');
        s += "| " + k + " | " + std::to_string(rs[i].second) + " |\n";
    }
    return s;
}

}  // namespace pyengine
