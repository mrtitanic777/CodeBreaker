// codebreaker/java/reflection_factory.cpp -- typed C++ port of codebreaker.java.reflection_factory.
#include "codebreaker/java/reflection_factory.hpp"

namespace codebreaker::java::reflection_factory {

namespace {
py::str S(const char* s) { return py::str(s); }
}  // namespace

const py::str _RECV = S(R"RX(((?:this\s*->\s*)?[A-Za-z_]\w*(?:\s*->\s*[A-Za-z_]\w*|\s*\[[^\]]*\])*))RX");

const py::re::Pattern _WORLD_ARG = py::re::compile(
    S(R"RX(\(\s*([A-Za-z_]\w*)\s*\*\s*\)\s*)RX") + _RECV + S(R"RX(\s*->\s*getConstructor\s*\(\s*)RX") +
    S(R"RX(\(\s*JArrayInit<[^>]*>\(\)((?:\s*,\s*(?:Class|j2xClassOf)\(typeid\(\s*\w+\s*\)\))+)\s*\)\s*\))RX") +
    S(R"RX(\s*\.\s*newInstance\s*\(\s*\(\s*JArrayInit<[^>]*>\(\)\s*,\s*(.+?)\)\s*\))RX"));

const py::re::Pattern _PLAIN =
    py::re::compile(S(R"RX(\(\s*([A-Za-z_]\w*)\s*\*\s*\)\s*)RX") + _RECV + S(R"RX(\s*->\s*newInstance\s*\(\s*\))RX"));

const py::re::Pattern _TYPEID = py::re::compile(S(R"RX((?:Class|j2xClassOf)\(typeid\(\s*(\w+)\s*\)\))RX"));

py::re::Pattern _REG_LINE = py::re::compile(S(R"RX(\b(?:register\w*)\s*\()RX"));

py::list<py::str> _split_top(const py::str& s) {
    // Split a comma-separated arg blob at top-level commas (respecting nested parens/angles).
    py::list<py::str> out;
    int64_t depth = 0;
    py::str cur = S("");
    for (const py::str& ch : s) {
        if (ch == S("(") || ch == S("<")) {
            depth += 1;
        } else if (ch == S(")") || ch == S(">")) {
            depth -= 1;
        }
        if (ch == S(",") && depth == 0) {
            out.append(cur.strip());
            cur = S("");
        } else {
            cur += ch;
        }
    }
    if (!cur.strip().empty()) {
        out.append(cur.strip());
    }
    return out;
}

py::list<py::str> _concrete_classes(const py::str& text, const py::set<py::str>& arg_types) {
    // Concrete classes to switch over: every typeid(X) on a registration line, minus the ctor arg
    // types (e.g. World) and dynamic `typeid(*(...))` lookups (which the \w+ pattern already excludes).
    py::set<py::str> seen;
    py::list<py::str> out;
    for (const py::str& line : py::split(text, S("\n"))) {
        if (!_REG_LINE.search(line)) {
            continue;
        }
        for (const py::re::Match& m : _TYPEID.finditer(line)) {
            py::str x = m.group(1);
            if (arg_types.contains(x) || seen.contains(x)) {
                continue;
            }
            seen.add(x);
            out.append(x);
        }
    }
    return out;
}

py::str _strip_resolved_comments(const py::str& text) {
    // Drop `// J2X-REVIEW[reflection-unsupported]` lines whose next code line now calls a generated
    // factory -- the concern is resolved, so the greppable tag should go.
    py::list<py::str> lines = py::split(text, S("\n"));
    py::list<py::str> out;
    int64_t i = 0;
    for (const py::str& ln : lines) {
        if (ln.contains(S("J2X-REVIEW[reflection-unsupported]"))) {
            py::str nxt = i + 1 < lines.size() ? lines[i + 1] : S("");
            if (nxt.contains(S("_createByReflection("))) {
                i++;
                continue;
            }
        }
        out.append(ln);
        i++;
    }
    return py::join(S("\n"), out);
}

std::tuple<py::str, int64_t, std::optional<Factory>> process(const py::str& text_in, const py::str& registry) {
    // Rewrite reflection sites in `text` (one .cpp, `registry` = its class name) to a generated
    // factory. Returns (new_text, sites_rewritten, factory_or_None).
    py::str text = text_in;
    py::list<py::re::Match> world = _WORLD_ARG.finditer(text);
    py::list<py::re::Match> plain = _PLAIN.finditer(text);
    if (world.empty() && plain.empty()) {
        return {_strip_resolved_comments(text), 0, std::nullopt};
    }

    py::str ret_type;
    py::list<py::str> arg_types;
    if (!world.empty()) {                                    // World-arg registry (entities)
        py::re::Match m0 = world[0];
        ret_type = m0.group(1);
        arg_types = _TYPEID.findall(m0.group(3));            // e.g. ['World']
        py::list<py::str> arg_vals_sample = _split_top(m0.group(4));
        int64_t n_args = py::min(arg_types.size(), arg_vals_sample.size());
        arg_types = arg_types.slice(0, n_args);
    } else {                                                 // no-arg registry (tile entities / packets)
        ret_type = plain[0].group(1);
    }

    py::list<py::str> concrete = _concrete_classes(text, py::set<py::str>::from(arg_types));
    if (concrete.empty()) {
        return {text, 0, std::nullopt};
    }

    // ---- generate the factory ----
    py::list<py::str> plist{S("Class* c")};
    py::list<py::str> clist;
    for (int64_t i = 0; i < arg_types.size(); i++) {
        plist.append(arg_types[i] + S("* a") + py::str(std::to_string(i)));
        clist.append(S("a") + py::str(std::to_string(i)));
    }
    py::str params = py::join(S(", "), plist);
    py::str callargs = py::join(S(", "), clist);
    py::str fname = registry + S("_createByReflection");
    py::list<py::str> L{S(""), S("// ==== CodeBreaker-generated reflection factory (replaces newInstance/getConstructor) ===="),
                        S("// Built from the class registration table; returns a real `new` instead of the NULL stub."),
                        ret_type + S("* ") + fname + S("(") + params + S(")"), S("{"), S("\tif (!c) return NULL;")};
    for (const py::str& x : concrete) {
        L.append(S("\tif (*c == Class(typeid(") + x + S("))) return new ") + x + S("(") + callargs + S(");"));
    }
    L.extend(py::list<py::str>{S("\treturn NULL;"), S("}"), S("")});
    py::str factory = py::join(S("\n"), L);

    // ---- rewrite the call sites ----
    int64_t n = 0;

    auto _repl_world = [&](const py::re::Match& m) -> py::str {
        n += 1;
        py::str recv = m.group(2);
        py::list<py::str> vals = _split_top(m.group(4)).slice(0, arg_types.size());
        py::list<py::str> cs;
        for (int64_t i = 0; i < py::min(arg_types.size(), vals.size()); i++) {
            cs.append(S("(") + arg_types[i] + S("*)") + vals[i]);
        }
        py::str casts = py::join(S(", "), cs);
        return fname + S("(") + recv + (!casts.empty() ? (S(", ") + casts) : S("")) + S(")");
    };

    auto _repl_plain = [&](const py::re::Match& m) -> py::str {
        n += 1;
        return fname + S("(") + m.group(2) + S(")");
    };

    text = _WORLD_ARG.sub(_repl_world, text);
    text = _PLAIN.sub(_repl_plain, text);

    // ---- insert the factory after the include block (before the first method uses it) ----
    py::list<py::str> lines = py::split(text, S("\n"));
    int64_t last_inc = 0;
    int64_t i = 0;
    for (const py::str& ln : lines) {
        if (ln.lstrip().startswith(S("#include"))) {
            last_inc = i;
        }
        i++;
    }
    lines.insert(last_inc + 1, factory);
    return {_strip_resolved_comments(py::join(S("\n"), lines)), n,
            Factory{registry, ret_type, arg_types, concrete}};
}

std::optional<FactoryRef> _find_factory(const py::list<FactoryRef>& factories, const py::list<py::str>& arg_types,
                                        const py::str& cast) {
    // Pick the factory whose ctor-arg signature matches this site and whose return type is `cast`
    // (or a base of it -- e.g. EntityList's Entity* factory serving an (EntityLiving*) site).
    py::list<FactoryRef> same;
    for (const FactoryRef& fa : factories) {
        if (fa.args == arg_types) {
            same.append(fa);
        }
    }
    for (const FactoryRef& fa : same) {
        if (fa.ret == cast || cast.startswith(fa.ret)) {
            return fa;
        }
    }
    if (same.size() == 1) {                                  // unique arg-sig (e.g. the sole World-arg factory)
        return same[0];
    }
    return std::nullopt;
}

std::tuple<py::str, int64_t> _reuse(const py::str& text_in, const py::list<FactoryRef>& factories) {
    // Rewrite reflection sites in a NON-registry file using the shared factories. Returns (text, n).
    int64_t n = 0;

    auto rep_world = [&](const py::re::Match& m) -> py::str {
        py::str cast = m.group(1);
        py::str recv = m.group(2);
        py::list<py::str> arg_types = _TYPEID.findall(m.group(3));
        py::list<py::str> vals = _split_top(m.group(4)).slice(0, arg_types.size());
        std::optional<FactoryRef> fa = _find_factory(factories, arg_types, cast);
        if (!fa) {
            return m.group(0);
        }
        n += 1;
        py::list<py::str> cs;
        for (int64_t i = 0; i < py::min(arg_types.size(), vals.size()); i++) {
            cs.append(S("(") + arg_types[i] + S("*)") + vals[i]);
        }
        py::str casts = py::join(S(", "), cs);
        return S("(") + cast + S("*)") + fa->name + S("(") + recv + (!casts.empty() ? (S(", ") + casts) : S("")) + S(")");
    };

    auto rep_plain = [&](const py::re::Match& m) -> py::str {
        py::str cast = m.group(1);
        py::str recv = m.group(2);
        std::optional<FactoryRef> fa = _find_factory(factories, py::list<py::str>(), cast);
        if (!fa) {
            return m.group(0);
        }
        n += 1;
        return S("(") + cast + S("*)") + fa->name + S("(") + recv + S(")");
    };

    py::str text = _WORLD_ARG.sub(rep_world, text_in);
    text = _PLAIN.sub(rep_plain, text);
    return {_strip_resolved_comments(text), n};
}

void _write_header(const py::str& out_dir, const py::list<FactoryRef>& factories) {
    py::set<py::str> fwd{S("Class")};
    for (const FactoryRef& fa : factories) {
        fwd.add(fa.ret);
        fwd.update(fa.args);
    }
    py::list<py::str> fwds;
    for (const py::str& t : py::sorted(fwd)) {
        fwds.append(S("class ") + t + S(";"));
    }
    py::list<py::str> L{S("// ReflectionFactory.h -- CodeBreaker-generated. Declares the reflection factories so any file can"),
                        S("// construct a registered class by its Class* (replacing Java newInstance())."),
                        S("#ifndef J2X_REFLECTION_FACTORY_H"), S("#define J2X_REFLECTION_FACTORY_H"), S(""),
                        py::join(S(" "), fwds)};
    py::set<py::str> seen;
    for (const FactoryRef& fa : factories) {
        py::list<py::str> ps{S("Class* c")};
        for (int64_t i = 0; i < fa.args.size(); i++) {
            ps.append(fa.args[i] + S("* a") + py::str(std::to_string(i)));
        }
        py::str decl = fa.ret + S("* ") + fa.name + S("(") + py::join(S(", "), ps) + S(");");
        if (!seen.contains(decl)) {
            seen.add(decl);
            L.append(decl);
        }
    }
    L.extend(py::list<py::str>{S(""), S("#endif // J2X_REFLECTION_FACTORY_H"), S("")});
    py::file f = py::open(py::os::path::join(out_dir, S("ReflectionFactory.h")), S("w"), S("utf-8"));
    f.write(py::join(S("\n"), L));
    f.close();
}

py::str _add_include(const py::str& text) {
    if (text.contains(S("#include \"ReflectionFactory.h\""))) {
        return text;
    }
    py::list<py::str> lines = py::split(text, S("\n"));
    int64_t last = 0;
    int64_t i = 0;
    for (const py::str& ln : lines) {
        if (ln.lstrip().startswith(S("#include"))) {
            last = i;
        }
        i++;
    }
    lines.insert(last + 1, S("#include \"ReflectionFactory.h\""));
    return py::join(S("\n"), lines);
}

RunResult run(const py::str& out_dir, const std::optional<py::list<py::str>>& registrars) {
    // Two passes: (1) generate a factory for each registry class from its registration table;
    // (2) rewrite reflection sites in OTHER files to reuse those factories via ReflectionFactory.h.
    if (registrars && !registrars->empty()) {   // the calls that map ids/names to classes (the project's: EntityList...)
        _REG_LINE = py::re::compile(S("\\b(?:") + py::join(S("|"), *registrars) + S(")\\s*\\("));
    }
    py::list<py::str> files;
    for (const py::str& f : py::sorted(py::os::listdir(out_dir))) {
        if (f.endswith(S(".cpp"))) {
            files.append(f);
        }
    }

    auto read = [&](const py::str& f) -> std::optional<py::str> {
        try {
            py::file fh = py::open(py::os::path::join(out_dir, f), S("r"), S("utf-8"), S("replace"));
            py::str t = fh.read_text();
            fh.close();
            return t;
        } catch (const py::OSError&) {
            return std::nullopt;
        }
    };

    auto write = [&](const py::str& f, const py::str& t) {
        py::file fh = py::open(py::os::path::join(out_dir, f), S("w"), S("utf-8"));
        fh.write(t);
        fh.close();
    };

    // PASS 1: registry factories
    RunResult res{py::list<RegistryResult>(), py::list<Reused>(), 0, 0};
    py::list<FactoryRef> factories;
    py::set<py::str> registry_files;
    for (const py::str& f : files) {
        std::optional<py::str> text = read(f);
        if (!text || (!text->contains(S("newInstance")) && !text->contains(S("_createByReflection(")))) {
            continue;
        }
        auto [nw, n, fac] = process(*text, f.slice(0, f.size() - 4));
        if (nw != *text) {
            write(f, nw);
        }
        if (n && fac) {
            res.registries.append(RegistryResult{f, n, fac->registry, fac->ret, fac->args, fac->classes});
            registry_files.add(f);
            factories.append(FactoryRef{fac->registry + S("_createByReflection"), fac->ret, fac->args});
        }
    }
    // detect factories generated by a PRIOR run (so re-runs / cross-file reuse still work)
    py::re::Pattern _defre = py::re::compile(
        S(R"RX(^([A-Za-z_]\w*)\*\s+(\w+_createByReflection)\s*\(([^)]*)\)\s*$)RX"), py::re::M);
    py::set<py::str> known;
    for (const FactoryRef& fa : factories) {
        known.add(fa.name);
    }
    for (const py::str& f : files) {
        std::optional<py::str> text = read(f);
        if (!text || !text->contains(S("_createByReflection("))) {
            continue;
        }
        for (const py::re::Match& m : _defre.finditer(*text)) {
            if (known.contains(m.group(2))) {
                continue;
            }
            py::list<py::str> params;                          // drop the leading Class* c
            py::list<py::str> parts = py::split(m.group(3), S(","));
            for (int64_t k = 1; k < parts.size(); k++) {
                params.append(parts[k].strip());
            }
            py::list<py::str> args;
            for (const py::str& p : params) {
                if (p.contains(S("*"))) {
                    args.append(py::split(p, S("*"))[0].strip());
                }
            }
            known.add(m.group(2));
            factories.append(FactoryRef{m.group(2), m.group(1), args});
        }
    }

    // PASS 2: reuse factories in other files
    if (!factories.empty()) {
        _write_header(out_dir, factories);
        for (const py::str& f : files) {
            if (registry_files.contains(f)) {
                continue;
            }
            std::optional<py::str> text = read(f);
            if (!text || !text->contains(S("newInstance"))) {
                continue;
            }
            auto [nw, n] = _reuse(*text, factories);
            if (n) {
                nw = _add_include(nw);
                write(f, nw);
                res.reused.append(Reused{f, n});
            }
        }
    }
    for (const RegistryResult& r : res.registries) {
        res.total_sites += r.sites;
        res.total_classes += r.classes.size();
    }
    for (const Reused& r : res.reused) {
        res.total_sites += r.sites;
    }
    return res;
}

}  // namespace codebreaker::java::reflection_factory
