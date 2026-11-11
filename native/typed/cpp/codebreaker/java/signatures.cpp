// codebreaker/java/signatures.cpp -- typed C++ port of codebreaker.java.signatures (see signatures.hpp).

#include "codebreaker/java/signatures.hpp"

#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>

#include "codebreaker/java/parse.hpp"
#include "codebreaker/java/profiles/package.hpp"
#include "codebreaker/java/transpile.hpp"
#include "py_os.hpp"

namespace codebreaker::java::signatures {

using py::str;

namespace pipeline = codebreaker::java::transpile;


const str DEFAULT_PKG("(default package)");

namespace {

str S(const char* s) { return str(s); }

void print_line(const str& line) {
    const std::string s = line.utf8() + "\n";
    std::fwrite(s.data(), 1, s.size(), stdout);
}

str e_str_(const py::Exception& e) { return str(std::string(e.what())); }

}  // namespace

py::dict<str, str> _PACKAGE_ROLES() { return profiles::active()->package_roles; }

str _package_role(const str& pkg, const py::list<str>& class_names) {
    const py::dict<str, str> roles = _PACKAGE_ROLES();
    if (roles.contains(pkg)) return roles.at(pkg);
    if (pkg == DEFAULT_PKG) return S("Classes declared without a package.");
    const str seg = std::get<2>(pkg.rpartition(S(".")));   // pkg.rsplit('.', 1)[-1]
    return py::percent(S("Subsystem: %s (%d classes)."), seg, class_names.size());
}

str _kind_word(const ir::JClass& cls) {
    if (cls.is_interface) return S("interface");
    if (cls.is_enum) return S("enum");
    return cls.is_abstract ? S("abstract class") : S("class");
}

str _inheritance(const ir::JClass& cls) {
    py::list<str> bits;
    if (cls.parent && !cls.parent->empty()) bits.append(py::percent(S("extends %s"), *cls.parent));
    if (cls.interfaces.size()) bits.append(py::percent(S("implements %s"), py::join(S(", "), cls.interfaces)));
    return bits.size() ? S(" : ") + py::join(S(" "), bits) : S("");
}

str _field_line(const ir::Field& field, const types::TypeResolver& resolver) {
    const str is_static = field.is_static ? S("static ") : S("");
    const str is_final = field.is_final ? S("final ") : S("");
    str cpp;
    try {
        cpp = resolver.field_type(field.jtype);
    } catch (const py::Exception& e) {
        cpp = py::percent(S("<resolve-error: %s>"), e_str_(e));
    }
    return py::percent(S("  - %s%s%s %s   // C++: %s"), is_static, is_final, field.jtype->repr(), field.name, cpp);
}

str _method_line(const ir::Method& method, const types::TypeResolver& resolver) {
    py::list<str> flags;
    if (method.is_static) flags.append(S("static"));
    if (method.is_abstract) flags.append(S("abstract"));
    const str prefix = flags.size() ? py::percent(S("[%s] "), py::join(S(" "), flags)) : S("");

    str jret;
    if (method.is_ctor) jret = S("(ctor)");
    else if (!method.return_type) jret = S("void");
    else jret = method.return_type->repr();
    py::list<str> jp;
    for (const auto& p : method.params) jp.append(py::percent(S("%s %s"), p->jtype->repr(), p->name));
    const str jparams = py::join(S(", "), jp);

    str cret;
    if (method.is_ctor) cret = S("(ctor)");
    else if (!method.return_type) cret = S("void");
    else {
        try {
            cret = resolver.return_type(method.return_type);
        } catch (const py::Exception& e) {
            cret = py::percent(S("<resolve-error: %s>"), e_str_(e));
        }
    }
    py::list<str> cpp_params;
    for (const auto& p : method.params) {
        try {
            cpp_params.append(py::percent(S("%s %s"), resolver.param_type(p->jtype), p->name));
        } catch (const py::Exception& e) {
            cpp_params.append(py::percent(S("<resolve-error: %s> %s"), e_str_(e), p->name));
        }
    }
    const str cparams = py::join(S(", "), cpp_params);

    return py::percent(S("  - %s%s %s(%s)   // C++: %s %s(%s)"), prefix, jret, method.name, jparams, cret, method.name, cparams);
}

void run(const str& java_dir, const str& out_path) {
    const py::list<str> paths = pipeline::discover(java_dir);

    py::list<pipeline::ClassRef> classes;
    py::list<std::tuple<str, str>> failures;
    for (const str& path : paths) {
        try {
            classes.append(parse::parse_file(path));
        } catch (const py::Exception& e) {
            failures.append(std::make_tuple(py::os::path::basename(path), e_str_(e)));
        }
    }

    const auto resolver = pipeline::build_resolver(classes);

    py::dict<str, py::list<pipeline::ClassRef>> by_pkg;
    for (const auto& c : classes) {
        const str pkg = (c->package && !c->package->empty()) ? *c->package : DEFAULT_PKG;
        if (!by_pkg.contains(pkg)) by_pkg.set(pkg, py::list<pipeline::ClassRef>());
        by_pkg.at(pkg).append(c);
    }
    std::vector<str> ordered_pkgs;
    std::vector<str> pkg_keys;
    for (const auto& kv : by_pkg.items()) pkg_keys.push_back(std::get<0>(kv));
    for (const str& pkg : pkg_keys) {
        const py::list<pipeline::ClassRef> lst = by_pkg.at(pkg);
        std::vector<pipeline::ClassRef> v;
        for (const auto& c : lst) v.push_back(c);
        std::stable_sort(v.begin(), v.end(), [](const pipeline::ClassRef& a, const pipeline::ClassRef& b) { return a->name.utf8() < b->name.utf8(); });
        py::list<pipeline::ClassRef> sorted;
        for (const auto& c : v) sorted.append(c);
        by_pkg.set(pkg, sorted);
        ordered_pkgs.push_back(pkg);
    }
    std::sort(ordered_pkgs.begin(), ordered_pkgs.end(), [](const str& a, const str& b) {
        const bool ad = a == DEFAULT_PKG, bd = b == DEFAULT_PKG;
        if (ad != bd) return !ad;
        return a.utf8() < b.utf8();
    });

    py::list<str> out;
    auto w = [&out](const str& s) { out.append(s); };

    w(S("# Game codebase signatures"));
    w(S(""));
    w(S("Generated by `python -m codebreaker java signatures` from the Java source "
        "\xE2\x80\x94 regenerate with `python -m codebreaker java signatures <java_dir>`."));
    w(S(""));
    w(py::percent(S("Classes mapped: %d (across %d subsystems/packages)."), classes.size(), by_pkg.size()));
    if (failures.size()) w(py::percent(S("Parse failures: %d (listed at the bottom)."), failures.size()));
    w(S(""));
    w(S("Each field/method shows the Java spelling and, after `// C++:`, the type "
        "the converter resolves it to (value vs `T*` pointer \xE2\x80\x94 i.e. `.` vs `->`)."));
    w(S(""));

    w(S("## Subsystem overview"));
    w(S(""));
    for (const str& pkg : ordered_pkgs) {
        const auto& cls_list = by_pkg.at(pkg);
        py::list<str> names;
        for (const auto& c : cls_list) names.append(c->name);
        w(py::percent(S("### %s"), pkg));
        w(py::percent(S("%s"), _package_role(pkg, names)));
        w(S(""));
        w(py::percent(S("Classes (%d): %s"), names.size(), py::join(S(", "), names)));
        w(S(""));
    }

    w(S("## Classes"));
    w(S(""));
    for (const str& pkg : ordered_pkgs) {
        for (const auto& cls : by_pkg.at(pkg)) {
            w(py::percent(S("### %s"), cls->name));
            w(py::percent(S("%s%s"), _kind_word(*cls), _inheritance(*cls)));
            w(py::percent(S("package: %s"), pkg));
            w(S(""));

            if (cls->is_enum && cls->enum_consts.size()) {
                py::list<str> cs;
                for (const auto& ec : cls->enum_consts) cs.append(ec->name + (ec->args.size() ? py::percent(S("(%d)"), ec->args.size()) : S("")));
                w(py::percent(S("Enum constants (%d): %s"), cls->enum_consts.size(), py::join(S(", "), cs)));
                w(S(""));
            }

            w(py::percent(S("Fields (%d):"), cls->fields.size()));
            if (cls->fields.size()) {
                for (const auto& f : cls->fields) w(_field_line(*f, *resolver));
            } else {
                w(S("  - (none)"));
            }
            w(S(""));

            w(py::percent(S("Methods (%d):"), cls->methods.size()));
            if (cls->methods.size()) {
                for (const auto& m : cls->methods) w(_method_line(*m, *resolver));
            } else {
                w(S("  - (none)"));
            }
            w(S(""));
        }
    }

    if (failures.size()) {
        w(S("## Parse failures"));
        w(S(""));
        for (const auto& f : failures) w(py::percent(S("  - %s: %s"), std::get<0>(f), std::get<1>(f)));
        w(S(""));
    }

    const str text = py::join(S("\n"), out) + S("\n");
    {
        py::file fh = py::open(out_path, S("w"), S("utf-8"));
        fh.write(text);
    }

    print_line(py::percent(S("Wrote %s"), out_path));
    print_line(py::percent(S("Classes mapped: %d across %d packages."), classes.size(), by_pkg.size()));
    for (const str& pkg : ordered_pkgs) print_line(py::percent(S("  - %s (%d classes)"), pkg, by_pkg.at(pkg).size()));
    if (failures.size()) {
        print_line(py::percent(S("Parse failures: %d"), failures.size()));
        for (const auto& f : failures) print_line(py::percent(S("  - %s: %s"), std::get<0>(f), std::get<1>(f)));
    } else {
        print_line(S("Parse failures: 0"));
    }
}

}  // namespace codebreaker::java::signatures
