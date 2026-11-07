// native_modules.cc -- modules replaced by their typed C++ (see native_modules.hpp).
#include "native_modules.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <set>
#include <sstream>

#include "cpp17.hpp"
#include "json.hpp"
#include "pyfront/pyfront.hpp"

namespace pyengine {

namespace fs = std::filesystem;
using py::V;

namespace {

std::string slurp(const fs::path& p) {
    std::ifstream f(p, std::ios::binary);
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

void spit(const fs::path& p, const std::string& text) {
    fs::create_directories(p.parent_path());
    std::ofstream(p, std::ios::binary) << text;
}

std::string rel_of(const std::string& module) {
    std::string r = module;
    std::replace(r.begin(), r.end(), '.', '/');
    return r;
}

std::string ns_of_module(const std::string& module) {
    std::string r;
    size_t a = 0;
    while (true) {
        size_t b = module.find('.', a);
        r += (r.empty() ? "" : "::") + cpp17::cpp_name(module.substr(a, b == std::string::npos ? std::string::npos : b - a));
        if (b == std::string::npos) break;
        a = b + 1;
    }
    return r;
}

std::string ident_of(const std::string& module) {   // codebreaker.compare.image -> codebreaker_compare_image
    std::string r = module;
    std::replace(r.begin(), r.end(), '.', '_');
    return r;
}

std::string cstr(const std::string& s) { return py::json_dump(V(s)); }

}  // namespace

bool emit_native_module(const std::string& root, const std::string& typed_dir, const std::string& module, const std::string& out_dir,
                        const std::string& judge, std::string& err) {
    std::set<std::string> agreed;   // what the judge ran beside the Python and found the same
    try {
        V j = py::json_parse(slurp(judge));
        for (auto& q : j.get("agree").seq()) agreed.insert(q.str());
    } catch (const std::exception& e) {
        err = "the judge's verdicts (" + judge + "): " + e.what();
        return false;
    }
    IslandSet s;
    if (!load_islands(typed_dir, "", s, err, true)) return false;
    // the module's top-level names: functions only (a constant or class a caller imports has no native form yet)
    fs::path py = fs::path(root) / (rel_of(module) + ".py");
    std::string src = slurp(py);
    if (src.empty()) {
        err = py.string() + ": not found (a module is emitted from its source, before the .py goes)";
        return false;
    }
    auto r = pyfront::parse(src, py.string());
    if (!r) {
        err = py.string() + ": syntax error";
        return false;
    }
    std::vector<std::string> defs;
    for (auto* st : r.module->body) {
        if (st->kind == pyfront::StmtKind::FunctionDef) defs.push_back(static_cast<const pyfront::FunctionDef*>(st)->name);
        else if (st->kind == pyfront::StmtKind::Import || st->kind == pyfront::StmtKind::ImportFrom || st->kind == pyfront::StmtKind::Pass) continue;
        else if (auto* e = pyfront::as<pyfront::ExprStmt>(st); e && e->value->kind == pyfront::ExprKind::Constant) continue;   // its docstring
        else {
            err = module + " has a top-level statement other than a def (line " + std::to_string(st->pos.lineno) +
                  "): a module of functions only can be native for now";
            return false;
        }
    }
    NativeModule nm;
    nm.module = module;
    nm.ns = ns_of_module(module);
    nm.rel = rel_of(module);
    for (auto& d : defs) {
        std::string q = module + "." + d;
        auto it = s.emit.find(q);
        if (it == s.emit.end()) {
            auto why = s.not_islands.find(q);
            err = q + " is not proven typed" + (why != s.not_islands.end() ? ": " + why->second : std::string(": not translated"));
            return false;
        }
        for (auto& m : it->second.modules)
            if (m != module) {
                err = q + " reaches " + m + ": a native module needing another module's typed C++ is not supported yet";
                return false;
            }
        if (!agreed.count(q)) {
            err = q + " has no agreeing verdict from the judge (" + judge + ")";
            return false;
        }
        nm.functions.push_back(it->second);
    }
    // the typed C++, as source: copied with a heading saying where it came from
    fs::path from = fs::path(typed_dir) / "cpp";
    for (const char* ext : {".hpp", ".cpp"}) {
        std::string text = slurp(from / (nm.rel + ext));
        if (text.empty()) {
            err = (from / (nm.rel + ext)).string() + ": missing";
            return false;
        }
        std::istringstream lines(text);
        std::string line;
        while (std::getline(lines, line))
            if (line.rfind("#include \"", 0) == 0 && line.find("py") != 10 && line.find(nm.rel) == std::string::npos) {
                err = module + "'s C++ includes " + line.substr(9) + ": another module of the program";
                return false;
            }
        spit(fs::path(out_dir) / "cpp" / (nm.rel + ext), text);
    }
    V fns = V::list();
    for (auto& is : nm.functions) {
        V f = V::dict();
        f.set("name", is.name);
        f.set("cpp", is.cpp);
        f.set("void", is.void_ret);
        V ps = V::list();
        for (auto& p : is.params) {
            V pd = V::dict();
            pd.set("name", p.name);
            pd.set("kind", p.kind);
            if (!p.spec.empty()) pd.set("spec", p.spec);
            if (!p.ctype.empty()) pd.set("ctype", p.ctype);
            ps.append(pd);
        }
        f.set("params", ps);
        fns.append(f);
    }
    V doc = V::dict();
    doc.set("module", module);
    doc.set("from", module + ".py, by pyengine's typed path (proven: translated, compiled, agreeing with the Python under the judge)");
    doc.set("functions", fns);
    spit(fs::path(out_dir) / (module + ".json"), py::json_dump(doc) + "\n");
    return true;
}

bool load_native_modules(const std::string& dir, std::vector<NativeModule>& out, std::string& err) {
    if (!fs::exists(dir)) return true;
    for (auto& e : fs::directory_iterator(dir)) {
        if (e.path().extension() != ".json") continue;
        V doc;
        try {
            doc = py::json_parse(slurp(e.path()));
        } catch (const std::exception& x) {
            err = e.path().string() + ": " + x.what();
            return false;
        }
        NativeModule nm;
        nm.module = doc.get("module").str();
        nm.ns = ns_of_module(nm.module);
        nm.rel = rel_of(nm.module);
        for (auto& f : doc.get("functions").seq()) {
            Island is;
            is.module = nm.module;
            is.name = f.get("name").str();
            is.qual = nm.module + "." + is.name;
            is.cpp = f.get("cpp").str();
            is.ns = nm.ns;
            is.void_ret = f.get("void").is_bool() && f.get("void").b;
            for (auto& p : f.get("params").seq()) {
                IslandParam ip;
                ip.name = p.get("name").str();
                ip.kind = p.get("kind").str();
                if (p.has("spec")) ip.spec = p.get("spec").str();
                if (p.has("ctype")) ip.ctype = p.get("ctype").str();
                is.params.push_back(ip);
            }
            nm.functions.push_back(is);
        }
        out.push_back(nm);
    }
    std::sort(out.begin(), out.end(), [](const NativeModule& a, const NativeModule& b) { return a.module < b.module; });
    return true;
}

std::string native_exports(const std::vector<NativeModule>& mods) {
    std::string o = "// native_exports.hpp -- the native modules' exports (pyengine --dyn-native-typed), plain C++ on both sides\n"
                    "// (native/pyengine/islands): true with the result, or false with the exception's type and message.\n"
                    "#pragma once\n#include <cstdint>\n#include <string>\n\n#include \"pyisl_val.hpp\"\n";
    for (auto& m : mods) {
        o += "\nnamespace pyisl::native::" + ident_of(m.module) + " {\n";
        for (auto& is : m.functions)
            o += "bool " + is.cpp + "(" + export_params(is) + ", std::string& exc_type_, std::string& exc_msg_);   // " + is.qual + "\n";
        o += "}  // namespace pyisl::native::" + ident_of(m.module) + "\n";
    }
    return o;
}

std::string native_typed_glue(const std::vector<NativeModule>& mods) {
    std::string o = "// native_modules.cpp -- the typed side of the program's native modules (pyengine --dyn-native-typed): each\n"
                    "// export converts its arguments to py.hpp's values, calls the module's typed function and copies the result\n"
                    "// out, or names the exception it raised. Compiled with py renamed (pyt).\n"
                    "#include \"pyisl_typed.hpp\"\n#include \"native_modules.hpp\"\n\n";
    for (auto& m : mods) o += "#include \"" + m.rel + ".hpp\"\n";
    for (auto& m : mods) {
        o += "\nnamespace pyisl::native::" + ident_of(m.module) + " {\n\n";
        for (auto& is : m.functions) {
            std::string args;
            for (auto& p : is.params) {
                std::string n = cpp17::cpp_name(p.name) + "_";
                std::string a = p.kind == "str" ? "py::str(" + n + ")" : p.kind == "bytes" ? "py::bytes(" + n + ".data(), " + n + ".size())"
                              : p.kind == "val" ? "pyisl::get<" + p.ctype + ">(" + n + ")" : n;
                args += (args.empty() ? "" : ", ") + a;
            }
            std::string call = "::" + m.ns + "::" + is.cpp + "(" + args + ")";
            o += "// " + is.qual + "\n";
            o += "bool " + is.cpp + "(" + export_params(is) + ", std::string& exc_type_, std::string& exc_msg_) {\n";
            o += "    try {\n";
            if (is.void_ret) o += "        " + call + ";\n        result_.k = pyisl::val::K::None;\n";
            else o += "        pyisl::put(result_, " + call + ");\n";
            o += "        return true;\n";
            o += "    } catch (const py::Exception& e) {\n        exc_type_ = e.type();\n        exc_msg_ = e.what();\n";
            o += "    } catch (const std::exception& e) {\n        exc_type_ = \"RuntimeError\";\n        exc_msg_ = e.what();\n    }\n";
            o += "    return false;\n}\n\n";
        }
        o += "}  // namespace pyisl::native::" + ident_of(m.module) + "\n";
    }
    return o;
}

std::string native_dyn_glue(const std::vector<NativeModule>& mods) {
    std::string o = "// native_modules_dyn.cpp -- the program's native modules (pyengine --dyn-native-typed): each module whose\n"
                    "// Python became its typed C++ is registered under its name; a function takes exactly the values its proof\n"
                    "// took (another kind is a TypeError: there is no Python body to give it to), calls the typed export and\n"
                    "// boxes the result, or raises the typed body's exception as Python's of that name.\n"
                    "#include \"py_native.hpp\"\n#include \"pyisl.hpp\"\n#include \"native_modules.hpp\"\n\nnamespace {\n\n";
    for (auto& m : mods) {
        std::string id = ident_of(m.module);
        o += "void init_" + id + "(const py::var& m) {\n";
        for (auto& is : m.functions) {
            o += "    // " + is.qual + "\n";
            o += "    py::mod_def(m, " + cstr(is.name) + ", [](const py::CallArgs& c) -> py::var {\n";
            o += "        if (c.pos.size() != " + std::to_string(is.params.size()) + " || !c.kw.empty())\n";
            o += "            pyisl::wrong_args(" + cstr(is.name + "()") + ", " + std::to_string(is.params.size()) + ");\n";
            std::string args;
            for (size_t i = 0; i < is.params.size(); i++) {
                const IslandParam& p = is.params[i];
                std::string v = "c.pos[" + std::to_string(i) + "]";
                std::string u = "a" + std::to_string(i) + "_";
                std::string bad = "pyisl::wrong_type(" + cstr(is.name + "()") + ", " + cstr(p.name) + ", " + v + ")";
                if (p.kind == "int") {
                    o += "        int64_t " + u + ";\n        if (!pyisl::int_in(" + v + ", INT64_MIN, INT64_MAX, " + u + ")) " + bad + ";\n";
                    args += u;
                } else if (p.kind == "bool") {
                    o += "        bool " + u + ";\n        if (!pyisl::boolean(" + v + ", " + u + ")) " + bad + ";\n";
                    args += u;
                } else if (p.kind == "float") {
                    o += "        double " + u + ";\n        if (!pyisl::real(" + v + ", " + u + ")) " + bad + ";\n";
                    args += u;
                } else if (p.kind == "str" || p.kind == "bytes") {
                    o += "        const std::string* " + u + " = pyisl::" + p.kind + "_arg(" + v + ");\n        if (!" + u + ") " + bad + ";\n";
                    args += "*" + u;
                } else {
                    o += "        pyisl::val " + u + ";\n        if (!pyisl::value(" + v + ", " + cstr(p.spec) + ", " + u + ")) " + bad + ";\n";
                    args += u;
                }
                args += ", ";
            }
            o += "        pyisl::val r_;\n        std::string et_, em_;\n";
            o += "        if (!pyisl::native::" + id + "::" + is.cpp + "(" + args + "r_, et_, em_)) pyisl::raise_named(et_, em_);\n";
            o += "        return pyisl::box(r_);\n    });\n";
        }
        o += "}\n\n";
    }
    for (auto& m : mods)
        o += "const bool registered_" + ident_of(m.module) + " = (py::register_native_module(" + cstr(m.module) + ", init_" + ident_of(m.module) +
             "), true);\n";
    o += "\n}  // namespace\n";
    return o;
}

}  // namespace pyengine
