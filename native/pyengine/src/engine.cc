// engine.cc -- the whole run (see engine.hpp).
#include "engine.hpp"

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <regex>

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
    if (o.log) fprintf(stderr, "solving %zu functions, %zu nested\n", p.fns.size(), p.nested.size());
    r.rounds = p.solve(40, o.log);
    auto t1 = std::chrono::steady_clock::now();
    TranslateResult tr = translate(p, o.policy, o.modules.empty() ? nullptr : &o.modules);
    auto t2 = std::chrono::steady_clock::now();
    r.seconds_solve = std::chrono::duration<double>(t1 - t0).count();
    r.seconds_lower = std::chrono::duration<double>(t2 - t1).count();
    r.modules = p.modules.size();
    for (auto& [q, f] : p.fns) {
        FnStatus st;
        st.lines = f->lines;
        auto it = tr.lowered.find(q);
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
            fns.set(q, d);
        }
        py::V doc = py::V::dict();
        doc.set("policy", o.policy);
        doc.set("extended", o.extended);
        doc.set("rounds", r.rounds);
        doc.set("functions", fns);
        write_text(out / "coverage.json", py::json_dump(doc));
        write_text(out / "coverage.md", coverage_markdown(r));
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
