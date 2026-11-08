// islands.cc -- which of the typed path's functions become typed islands of the dynamic build, and the
// typed side's glue (see islands.hpp).
#include "islands.hpp"

#include <filesystem>
#include <fstream>
#include <functional>
#include <set>
#include <sstream>

#include "cpp17.hpp"
#include "json.hpp"

namespace pyengine {

namespace fs = std::filesystem;
using py::V;

namespace {

std::string read_text(const fs::path& p) {
    std::ifstream f(p, std::ios::binary);
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

std::string sget(const V& d, const char* k) {
    const V& v = d.get(k);
    return v.is_str() ? v.str() : std::string();
}

std::string tname(const V& t) { return sget(t, "kind") == "class" ? sget(t, "name") : sget(t, "kind"); }
const std::vector<V>& targs(const V& t) {
    static const std::vector<V> none;
    const V& a = t.get("args");
    return a.is_seq() ? a.seq() : none;
}
std::string type_text(const V& t) {
    std::string n = tname(t), o = n;
    auto& a = targs(t);
    if (!a.empty()) {
        o += "[";
        for (size_t i = 0; i < a.size(); i++) o += (i ? ", " : "") + type_text(a[i]);
        o += "]";
    }
    return o;
}

// a value: nothing anyone else holds can change it (and the typed side's copy is its equal)
bool value_type(const V& t) {
    std::string n = tname(t);
    if (n == "i64" || n == "bool" || n == "f64" || n == "py.str" || n == "py.bytes") return true;
    if (n == "py.optional" || n == "py.tuple" || n == "py.variant") {
        for (auto& x : targs(t)) if (!value_type(x)) return false;
        return !targs(t).empty();
    }
    return false;
}
// a result the wrapper can copy out: a value, or a fresh list / dict / bytearray of values
bool result_type(const V& t) {
    std::string n = tname(t);
    if (n == "void" || value_type(t) || n == "py.bytearray") return true;
    if (n == "py.list") return targs(t).size() == 1 && value_type(targs(t)[0]);
    if (n == "py.dict") return targs(t).size() == 2 && value_type(targs(t)[0]) && value_type(targs(t)[1]);
    return false;
}
// a parameter of a val kind: None-or and tuples of str, bytes, bool, float (an int inside has no range recorded)
bool val_param(const V& t, std::string& spec, std::string& ctype) {
    std::string n = tname(t);
    if (n == "py.str") { spec += "s"; ctype = "py::str"; return true; }
    if (n == "py.bytes") { spec += "b"; ctype = "py::bytes"; return true; }
    if (n == "bool") { spec += "B"; ctype = "bool"; return true; }
    if (n == "f64") { spec += "f"; ctype = "double"; return true; }
    if (n == "py.optional" && targs(t).size() == 1) {
        std::string inner;
        spec += "?";
        if (!val_param(targs(t)[0], spec, inner)) return false;
        ctype = "std::optional<" + inner + ">";
        return true;
    }
    if (n == "py.tuple" && !targs(t).empty()) {
        spec += "(";
        std::string list;
        for (auto& x : targs(t)) {
            std::string c;
            if (!val_param(x, spec, c)) return false;
            list += (list.empty() ? "" : ", ") + c;
        }
        spec += ")";
        ctype = "std::tuple<" + list + ">";
        return true;
    }
    return false;
}

// the typed library calls whose effect a caller could see beyond the result, or whose answer is not the
// dynamic runtime's state: files written, the environment, the clock
bool effect_call(const std::string& n) {
    static const std::set<std::string> reads = {"os.path.join", "os.path.isdir", "os.path.isfile", "os.path.exists", "os.path.abspath",
                                                "os.path.basename", "os.path.dirname", "os.path.splitext", "os.path.normpath",
                                                "os.path.getsize", "os.listdir", "os.walk"};
    if (n.rfind("os.", 0) == 0) return !reads.count(n);
    for (const char* p : {"shutil.", "time.", "sys.", "subprocess.", "random.", "print", "input"})
        if (n.rfind(p, 0) == 0) return true;
    return n == "open" || n == "file.write" || n == "file.truncate" || n == "file.flush";
}

std::vector<V> l2_lines(const std::string& text) {
    std::vector<V> out;
    size_t st = 0;
    while (st < text.size()) {
        size_t e = text.find('\n', st);
        if (e == std::string::npos) e = text.size();
        std::string ln = text.substr(st, e - st);
        st = e + 1;
        if (ln.find_first_not_of(" \r\t") == std::string::npos) continue;
        out.push_back(py::json_parse(ln));
    }
    return out;
}

struct Analysis {
    fs::path dir;
    const V* fns = nullptr;                                  // coverage.json's functions
    std::map<std::string, std::vector<V>> l2;                // qualname -> its L2 declarations
    std::map<std::string, V> units;                          // module -> its unit (l2/<module>.json)
    std::map<std::string, std::string> body_verdict;         // qualname -> "" (fine) or why not
    std::set<std::string> visiting;
    // what each typed function needs linked: "fn:" a function it calls, "mod:" a module whose names it reads
    std::map<std::string, std::set<std::string>> uses;
    std::set<std::string>* reads_ = nullptr;   // (constant(): the modules a constant's value reads, for its reader)

    // the module a class or function of the package is in (the longest prefix with a unit)
    std::string module_of(const std::string& q) {
        std::string m = q;
        while (!m.empty()) {
            if (unit(m)) return m;
            size_t d = m.rfind('.');
            if (d == std::string::npos) break;
            m.resize(d);
        }
        return std::string();
    }
    // the typed modules an island needs compiled: its own, and every module its calls and constants reach
    // (a module's C++ is compiled whole: every typed function in it needs what it calls linked too)
    std::map<std::string, std::vector<std::string>> in_module;   // module -> its translated functions
    void add_module(const std::string& m, std::set<std::string>& mods, std::set<std::string>& seen) {
        if (m.empty() || !mods.insert(m).second) return;
        if (in_module.empty())
            for (auto& [k, d] : l2) in_module[module_of(k)].push_back(k);
        for (auto& k : in_module[m]) {
            body(k);   // (fills uses[k])
            modules_of(k, mods, seen);
        }
    }
    void modules_of(const std::string& q, std::set<std::string>& mods, std::set<std::string>& seen) {
        if (!seen.insert(q).second) return;
        add_module(module_of(q), mods, seen);
        for (auto& u : uses[q]) {
            if (u.compare(0, 4, "mod:") == 0) add_module(u.substr(4), mods, seen);
            else modules_of(u.substr(3), mods, seen);
        }
    }

    const V* unit(const std::string& mod) {
        auto it = units.find(mod);
        if (it != units.end()) return it->second.is_dict() ? &it->second : nullptr;
        V u;
        fs::path p = dir / "l2" / (mod + ".json");
        if (fs::exists(p)) {
            try { u = py::json_parse(read_text(p)); } catch (...) {}
        }
        return units.emplace(mod, u).first->second.is_dict() ? &units[mod] : nullptr;
    }
    // a module name's declaration in its unit (FieldDecl), or null
    const V* field_decl(const std::string& mod, const std::string& name) {
        const V* u = unit(mod);
        if (!u) return nullptr;
        const V* found = nullptr;
        std::function<void(const V&)> walk = [&](const V& n) {
            if (found) return;
            if (n.is_dict()) {
                if (sget(n, "node") == "FieldDecl" && n.get("sym").is_dict() && sget(n.get("sym"), "name") == name &&
                    sget(n.get("sym"), "owner") == mod) {
                    found = &n;
                    return;
                }
                for (auto& kv : n.items()) walk(kv.second);
            } else if (n.is_seq())
                for (auto& x : n.seq()) walk(x);
        };
        walk(*u);
        return found;
    }
    bool has_mod(const V& decl, const char* m) {
        const V& ms = decl.get("mods");
        if (!ms.is_seq()) return false;
        for (auto& x : ms.seq()) if (x.is_str() && x.str() == m) return true;
        return false;
    }
    // a module name an island may read: a constant of a value type (or a compiled pattern) set from literals
    std::string constant(const std::string& mod, const std::string& name, int depth = 0) {
        const V* d = field_decl(mod, name);
        if (!d) return "module name " + mod + "." + name + " is not a typed module constant";
        if (reads_) reads_->insert("mod:" + mod);
        if (!has_mod(*d, "const")) return "reads module name " + mod + "." + name + ", which is not const";
        const V& t = d->get("type");
        if (!value_type(t) && tname(t) != "py.re.Pattern") return "reads module name " + mod + "." + name + " of type " + type_text(t);
        std::string why;
        std::function<void(const V&)> walk = [&](const V& n) {
            if (!why.empty()) return;
            if (n.is_dict()) {
                std::string k = sget(n, "node");
                if (!k.empty()) {
                    static const std::set<std::string> ok = {"Lit", "Unary", "Binary", "Paren", "Concat", "Cast", "Cond", "TypeName"};
                    if (k == "Name") {
                        const V& s = n.get("sym");
                        if (sget(s, "kind") != "global" || depth > 8) why = "module name " + name + " is set from " + sget(s, "name");
                        else why = constant(sget(s, "owner"), sget(s, "name"), depth + 1);
                        return;
                    }
                    if (k == "Call") {
                        const V& s = n.get("sym");
                        std::string f = sget(s, "name");
                        if (sget(s, "owner") != "py" || !(f == "tuple" || f == "re.compile" || f == "kw")) {
                            why = "module name " + mod + "." + name + " is set by a call of " + f;
                            return;
                        }
                    } else if (!ok.count(k)) {
                        why = "module name " + mod + "." + name + " is set by a " + k;
                        return;
                    }
                }
                for (auto& kv : n.items()) if (kv.first.is_str() && kv.first.str() != "type" && kv.first.str() != "sym") walk(kv.second);
            } else if (n.is_seq())
                for (auto& x : n.seq()) walk(x);
        };
        walk(d->get("init"));
        return why;
    }
    // "" when the body of qualname, and of every typed function it calls, has no effect beyond its result
    std::string body(const std::string& q) {
        auto it = body_verdict.find(q);
        if (it != body_verdict.end()) return it->second;
        if (visiting.count(q)) return "";   // (a cycle: judged where it started)
        auto li = l2.find(q);
        if (li == l2.end()) return "calls " + q + ", which is not translated";
        visiting.insert(q);
        std::string why;   // (the first reason; the walk goes on: uses[q] is every call, for linking)
        auto note = [&](const std::string& w) { if (why.empty() && !w.empty()) why = w; };
        std::function<void(const V&)> walk = [&](const V& n) {
            if (n.is_dict()) {
                const V& s = n.get("sym");
                std::string node = sget(n, "node");
                if (s.is_dict() && !node.empty() && node != "Param" && node != "Local") {
                    std::string kind = sget(s, "kind"), owner = sget(s, "owner"), name = sget(s, "name");
                    if (kind == "global" && !owner.empty()) {
                        reads_ = &uses[q];
                        note(constant(owner, name));
                        reads_ = nullptr;
                    } else if ((kind == "function" || kind == "method") && owner == "py") {
                        if (effect_call(name)) note("calls " + name);
                    } else if ((kind == "function" || kind == "method") && !owner.empty()) {
                        std::string callee = owner + "." + name;
                        const V& st = fns->get(V(callee));
                        if (!st.is_dict() || sget(st, "status") != "translated") note("calls " + callee + ", which is not translated");
                        else {
                            uses[q].insert("fn:" + callee);
                            std::string w = body(callee);
                            if (!w.empty()) note(w + " (in " + callee + ")");
                        }
                    } else if (kind == "ctor" && !owner.empty() && owner != "py") {
                        // a class of the package: its __init__ runs (a record without one is only its fields)
                        std::string init = owner + ".__init__";
                        std::string cm = module_of(owner);
                        if (!cm.empty()) uses[q].insert("mod:" + cm);
                        const V& st = fns->get(V(init));
                        if (st.is_dict() && sget(st, "status") == "translated") uses[q].insert("fn:" + init);
                        if (st.is_dict() && sget(st, "status") != "translated") note("makes a " + owner + ", whose __init__ is not translated");
                        else if (st.is_dict()) {
                            std::string w = body(init);
                            if (!w.empty()) note(w + " (in " + init + ")");
                        }
                    }
                }
                for (auto& kv : n.items()) if (kv.first.is_str() && kv.first.str() != "type" && kv.first.str() != "sym") walk(kv.second);
            } else if (n.is_seq())
                for (auto& x : n.seq()) walk(x);
        };
        for (auto& d : li->second) walk(d);
        visiting.erase(q);
        body_verdict[q] = why;
        return why;
    }
};

std::string ns_of(const std::string& module) {
    std::string o;
    size_t st = 0;
    while (st <= module.size()) {
        size_t e = module.find('.', st);
        if (e == std::string::npos) e = module.size();
        o += (o.empty() ? "" : "::") + cpp17::cpp_name(module.substr(st, e - st));
        st = e + 1;
    }
    return o;
}

std::string jstr(const std::string& s) { return py::json_dump(V(s)); }

}  // namespace

bool load_islands(const std::string& typed_dir, const std::string& verdicts, IslandSet& out, std::string& err, bool ints_unranged) {
    Analysis a;
    a.dir = fs::path(typed_dir);
    out.typed_dir = fs::absolute(a.dir).lexically_normal().generic_string();
    V cov;
    try {
        cov = py::json_parse(read_text(a.dir / "coverage.json"));
    } catch (const std::exception& e) {
        err = "cannot read " + (a.dir / "coverage.json").string() + ": " + e.what();
        return false;
    }
    if (!fs::exists(a.dir / "cpp")) {
        err = (a.dir / "cpp").string() + " is missing (pyengine <package> --out DIR --cpp DIR/cpp)";
        return false;
    }
    const V& fns = cov.get("functions");
    if (!fns.is_dict()) {
        err = "coverage.json has no functions";
        return false;
    }
    a.fns = &fns;
    for (auto& [k, st] : fns.items())
        if (sget(st, "status") == "translated") a.l2[k.str()] = l2_lines(sget(st, "l2"));
    V verd;
    if (!verdicts.empty()) {
        try {
            verd = py::json_parse(read_text(verdicts)).get("islands");
        } catch (const std::exception& e) {
            err = "cannot read " + verdicts + ": " + e.what();
            return false;
        }
    }
    for (auto& [q, decls] : a.l2) {
        const V& st = fns.get(V(q));
        auto no = [&](const std::string& why) { out.not_islands[q] = why; };
        const V& d = decls.front();
        const V& sym = d.get("sym");
        if (sget(sym, "kind") != "function") { no("a method (an island is a module function)"); continue; }
        if (decls.size() != 1) { no("translated as " + std::to_string(decls.size()) + " overloads"); continue; }
        Island is;
        is.qual = q;
        is.module = sget(sym, "owner");
        is.name = sget(d, "name");
        if (is.module + "." + is.name != q) { no("not a module function"); continue; }
        is.ns = ns_of(is.module);
        is.cpp = cpp17::cpp_name(is.name);
        is.line = d.get("line").is_int() ? (int)d.get("line").as_int() : 0;
        const V& ranges = st.get("ranges");
        std::string why;
        const V& ps = d.get("params");
        if (ps.is_seq())
            for (auto& p : ps.seq()) {
                IslandParam ip;
                ip.name = sget(p.get("sym"), "name");
                const V& t = p.get("type");
                std::string n = tname(t);
                if (n == "i64") {
                    ip.kind = "int";
                    const V& r = ranges.get(V(ip.name));
                    if ((!r.is_seq() || r.size() != 2) && ints_unranged) {
                        is.params.push_back(ip);   // (lo, hi: all of int64)
                        continue;
                    }
                    if (!r.is_seq() || r.size() != 2) { why = "no range recorded for int parameter " + ip.name; break; }
                    try {
                        ip.lo = std::stoll(r[0].str());
                        ip.hi = std::stoll(r[1].str());
                    } catch (...) { why = "the range of " + ip.name + " is past int64"; break; }
                } else if (n == "bool") ip.kind = "bool";
                else if (n == "f64") ip.kind = "float";
                else if (n == "py.str") ip.kind = "str";
                else if (n == "py.bytes") ip.kind = "bytes";
                else if (val_param(t, ip.spec, ip.ctype)) ip.kind = "val";
                else { why = "parameter " + ip.name + " is " + type_text(t) + " (not a value the wrapper can copy in)"; break; }
                is.params.push_back(ip);
            }
        if (!why.empty()) { no(why); continue; }
        const V& ret = d.get("ret");
        if (!result_type(ret)) { no("returns " + type_text(ret) + " (not a value the wrapper can copy out)"); continue; }
        is.void_ret = tname(ret) == "void";
        why = a.body(q);
        if (!why.empty()) { no(why); continue; }
        {
            std::set<std::string> mods, seen;
            a.modules_of(q, mods, seen);
            for (auto& m : mods) {
                std::string rel = m;
                for (auto& c : rel) if (c == '.') c = '/';
                if (!fs::exists(a.dir / "cpp" / (rel + ".cpp"))) { why = "the typed C++ of " + m + " is missing"; break; }
                is.modules.push_back(m);
            }
        }
        if (!why.empty()) { no(why); continue; }
        if (verd.is_dict()) {
            const V& v = verd.get(V(q));
            std::string vv = v.is_dict() ? sget(v, "verdict") : std::string();
            if (vv != "agree") {
                out.held[q] = vv.empty() ? std::string("no verdict (the judge has not run it)") : vv + (sget(v, "why").empty() ? "" : ": " + sget(v, "why"));
                continue;
            }
        }
        out.emit[q] = is;
    }
    return true;
}

std::string export_params(const Island& is) {
    std::string o;
    for (auto& p : is.params) {
        std::string t = p.kind == "int" ? "int64_t" : p.kind == "bool" ? "bool" : p.kind == "float" ? "double"
                      : p.kind == "val" ? "const pyisl::val&" : "const std::string&";
        o += t + " " + cpp17::cpp_name(p.name) + "_, ";
    }
    return o + "pyisl::val& result_";
}

std::string islands_header(const IslandSet& s) {
    std::string o = "// islands.hpp -- the typed islands of this program (pyengine --dynamic --dyn-typed): one export per\n"
                    "// function the typed path proved, plain C++ on both sides (native/pyengine/islands). Each answers\n"
                    "// true with its result in result_, or false: the dynamic body runs instead.\n"
                    "#pragma once\n#include <cstdint>\n#include <string>\n\n#include \"pyisl_val.hpp\"\n\n";
    std::string cur;
    for (auto& [q, is] : s.emit) {
        if (is.ns != cur) {
            if (!cur.empty()) o += "}  // namespace pyisl::island::" + cur + "\n\n";
            cur = is.ns;
            o += "namespace pyisl::island::" + cur + " {\n";
        }
        o += "bool " + is.cpp + "(" + export_params(is) + ");   // " + q + "\n";
    }
    if (!cur.empty()) o += "}  // namespace pyisl::island::" + cur + "\n";
    return o;
}

std::string islands_glue(const IslandSet& s) {
    std::string o = "// islands.cpp -- the typed side of this program's typed islands: each export converts its arguments to\n"
                    "// py.hpp's values, calls the typed function the typed path wrote and copies the result out. Compiled\n"
                    "// with py renamed (pyt): the dynamic runtime's py is another namespace.\n"
                    "#include \"pyisl_typed.hpp\"\n#include \"islands.hpp\"\n\n";
    std::set<std::string> mods;
    for (auto& [q, is] : s.emit) mods.insert(is.module);
    for (auto& m : mods) {
        std::string rel = m;
        for (auto& c : rel) if (c == '.') c = '/';
        o += "#include \"" + rel + ".hpp\"\n";
    }
    o += "\n";

    std::string cur;
    for (auto& [q, is] : s.emit) {
        if (is.ns != cur) {
            if (!cur.empty()) o += "}  // namespace pyisl::island::" + cur + "\n\n";
            cur = is.ns;
            o += "namespace pyisl::island::" + cur + " {\n\n";
        }
        std::string args;
        for (auto& p : is.params) {
            std::string n = cpp17::cpp_name(p.name) + "_";
            std::string a = p.kind == "str" ? "py::str(" + n + ")" : p.kind == "bytes" ? "py::bytes(" + n + ".data(), " + n + ".size())"
                          : p.kind == "val" ? "pyisl::get<" + p.ctype + ">(" + n + ")" : n;
            args += (args.empty() ? "" : ", ") + a;
        }
        std::string call = "::" + is.ns + "::" + is.cpp + "(" + args + ")";
        std::string counter = cpp17::cpp_name(is.name) + "_count";
        o += "// " + q + "\n";
        o += "static pyisl::counter " + counter + "(\"" + q + "\");\n";
        o += "bool " + is.cpp + "(" + export_params(is) + ") {\n";
        o += "    try {\n";
        if (is.void_ret) o += "        " + call + ";\n        result_.k = pyisl::val::K::None;\n";
        else o += "        pyisl::put(result_, " + call + ");\n";
        o += "        " + counter + ".typed++;\n";
        o += "        return true;\n    } catch (...) {\n        " + counter + ".raised++;\n"
             "        return false;   // (the dynamic body raises Python's exception)\n    }\n}\n\n";
    }
    if (!cur.empty()) o += "}  // namespace pyisl::island::" + cur + "\n";
    return o;
}

std::string islands_report(const IslandSet& s) {
    std::string o = "{\"typed\": " + jstr(s.typed_dir) + ",\n \"islands\": {";
    bool first = true;
    for (auto& [q, is] : s.emit) {
        o += std::string(first ? "\n" : ",\n") + "  " + jstr(q) + ": {\"line\": " + std::to_string(is.line) + ", \"params\": [";
        for (size_t i = 0; i < is.params.size(); i++) {
            auto& p = is.params[i];
            o += (i ? ", " : "") + jstr(p.name + ": " + p.kind + (p.kind == "int" ? "[" + std::to_string(p.lo) + ".." + std::to_string(p.hi) + "]" : "") +
                                        (p.kind == "val" ? " " + p.spec : ""));
        }
        o += "]}";
        first = false;
    }
    o += "},\n \"held\": {";
    first = true;
    for (auto& [q, why] : s.held) {
        o += std::string(first ? "\n" : ",\n") + "  " + jstr(q) + ": " + jstr(why);
        first = false;
    }
    o += "},\n \"not_islands\": {";
    first = true;
    for (auto& [q, why] : s.not_islands) {
        o += std::string(first ? "\n" : ",\n") + "  " + jstr(q) + ": " + jstr(why);
        first = false;
    }
    return o + "}}\n";
}

std::string islands_funcs(const IslandSet& s) {
    std::string o;
    for (auto& [q, is] : s.emit) o += is.module + ":" + is.name + "\n";
    return o;
}

// ------------------------------------------------------------------------------------------ the judge
namespace {
// function -> its argument lists -> the outcomes met (" -> <hash>" or " raise <type>")
using Calls = std::map<std::string, std::map<std::string, std::set<std::string>>>;
size_t read_traces(const fs::path& dir, const std::set<std::string>& wanted, Calls& out) {
    size_t files = 0;
    std::error_code ec;
    for (auto it = fs::directory_iterator(dir, ec); !ec && it != fs::directory_iterator(); it.increment(ec)) {
        if (!it->is_regular_file()) continue;
        files++;
        std::ifstream f(it->path(), std::ios::binary);
        std::string l;
        while (std::getline(f, l)) {
            while (!l.empty() && (l.back() == '\r' || l.back() == '\n')) l.pop_back();
            size_t p = l.find('('), e = l.find(')', p == std::string::npos ? 0 : p);
            if (p == std::string::npos || e == std::string::npos) continue;
            std::string fn = l.substr(0, p);
            if (!wanted.count(fn)) continue;
            out[fn][l.substr(p, e + 1 - p)].insert(l.substr(e + 1));
        }
    }
    return files;
}
std::string outcomes(const std::set<std::string>& s) {
    std::string o;
    for (auto& x : s) o += (o.empty() ? "" : " |") + x;
    return o;
}
}  // namespace

int judge_islands(const std::string& islands_json, const std::string& on_dir, const std::string& off_dir, const std::string& out) {
    V doc;
    try {
        doc = py::json_parse(read_text(islands_json));
    } catch (const std::exception& e) {
        fprintf(stderr, "cannot read %s: %s\n", islands_json.c_str(), e.what());
        return 2;
    }
    const V& isl = doc.get("islands");
    if (!isl.is_dict()) {
        fprintf(stderr, "%s has no islands\n", islands_json.c_str());
        return 2;
    }
    // module:qualname of each island (an island is a module function: its qualname is its name)
    std::map<std::string, std::string> key_of;
    std::set<std::string> wanted;
    for (auto& [q, v] : isl.items()) {
        std::string qs = q.str();
        size_t d = qs.rfind('.');
        std::string k = qs.substr(0, d) + ":" + qs.substr(d + 1);
        key_of[qs] = k;
        wanted.insert(k);
    }
    Calls on, off;
    size_t fon = read_traces(on_dir, wanted, on), foff = read_traces(off_dir, wanted, off);
    size_t agree = 0, differ = 0, not_run = 0;
    std::string o = "{\"judged\": {\"with islands\": " + jstr(on_dir) + ", \"without\": " + jstr(off_dir) + ", \"trace files\": [" +
                    std::to_string(fon) + ", " + std::to_string(foff) + "]},\n \"islands\": {";
    bool first = true;
    for (auto& [q, k] : key_of) {
        auto& a = on[k];
        auto& b = off[k];
        size_t common = 0, diffs = 0;
        std::string why;
        for (auto& [args, outs] : a) {
            auto it = b.find(args);
            if (it == b.end()) continue;
            common++;
            if (outs != it->second) {
                if (!diffs) why = "arguments " + args + ": typed" + outcomes(outs) + ", dynamic" + outcomes(it->second);
                diffs++;
            }
        }
        std::string verdict;
        if (diffs) {
            verdict = "differs";
            why = std::to_string(diffs) + " of " + std::to_string(common) + " argument lists; first: " + why;
            differ++;
        } else if (common) {
            verdict = "agree";
            why = std::to_string(common) + " argument lists";
            agree++;
        } else {
            verdict = "not run";
            why = a.empty() && b.empty() ? "no calls in the suites" : "no argument list met in both runs (" + std::to_string(a.size()) + " / " + std::to_string(b.size()) + ")";
            not_run++;
        }
        if (verdict == "differs") printf("DIFFERS %s: %s\n", q.c_str(), why.c_str());
        o += std::string(first ? "\n" : ",\n") + "  " + jstr(q) + ": {\"verdict\": " + jstr(verdict) + ", \"why\": " + jstr(why) + "}";
        first = false;
    }
    o += "}}\n";
    std::ofstream f(out, std::ios::binary);
    f << o;
    printf("islands: %zu agree, %zu differ, %zu not run (trace files: %zu with islands, %zu without)\n", agree, differ, not_run, fon, foff);
    return differ ? 1 : 0;
}

}  // namespace pyengine
