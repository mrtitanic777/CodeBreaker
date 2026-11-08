// codebreaker/java/emit/std_transforms.cpp -- C++17 port of codebreaker.java.emit.std_transforms.
#include "codebreaker/java/emit/std_transforms.hpp"

#include "codebreaker/java/textutil.hpp"

namespace codebreaker::java::emit::std_transforms {

const py::re::Pattern& _NULL_WORD() {
    static const py::re::Pattern p = py::re::compile(py::str(R"PAT(\bNULL\b)PAT"));
    return p;
}

// from codebreaker.java.textutil import STR_LITERAL as _STR_LITERAL
static const py::re::Pattern& _STR_LITERAL() {
    return textutil::STR_LITERAL();
}

const py::re::Pattern& _ITER_FOR() {
    static const py::re::Pattern p = py::re::compile(py::str(
        R"PAT(for\s*\(\s*)PAT"
        R"PAT(((?:const\s+)?(?:std::)?(?:map|set|vector|list|deque)<[^>]*(?:<[^>]*>)*>\s*::\s*)PAT"
        R"PAT((?:const_)?iterator)\s+)PAT"
        R"PAT((\w+))PAT"));
    return p;
}

const py::re::Pattern& _ITER_DECL() {
    static const py::re::Pattern p = py::re::compile(
        py::str(
            R"PAT(^(\s*)((?:const\s+)?(?:std::)?(?:map|set|vector|list|deque)<[^>]*(?:<[^>]*>)*>\s*::\s*)PAT"
            R"PAT((?:const_)?iterator)\s+(\w+\s*=))PAT"),
        py::re::MULTILINE);
    return p;
}

const py::re::Pattern& _STATIC_CONST_INT() {
    static const py::re::Pattern p = py::re::compile(py::str(R"PAT(\bstatic\s+const\s+int\b)PAT"));
    return p;
}

py::str _sub_outside_literals(const py::re::Pattern& rx, const py::str& repl, const py::str& s) {
    py::list<py::str> out;
    int64_t last = 0;
    for (const auto& m : _STR_LITERAL().finditer(s)) {
        py::str before = s.slice(last, m.start());
        out.append(rx.sub(repl, before));
        out.append(m.group(0));
        last = m.end();
    }
    out.append(rx.sub(repl, s.slice_from(last)));
    return py::join(py::str(""), out);
}

py::str _annotate_first(const py::str& text, const py::str& pattern, const py::str& annotation) {
    py::list<py::str> lines = py::split(text, py::str("\n"));
    for (int64_t i = 0; i < lines.size(); i++) {
        py::str line = lines[i];
        if (line.contains(pattern)) {
            // '//' not in line.split(pattern, 1)[-1]
            py::list<py::str> parts = py::split(line, pattern, 1);
            if (!parts[-1].contains(py::str("//"))) {
                lines[i] = line.rstrip() + py::str("  ") + annotation;
                break;
            }
        }
    }
    return py::join(py::str("\n"), lines);
}

// Enum-constant references that collide with C/C++ macros. The declaration is suffixed with `_`
// (naming.enum_member); a reference `EnumX::NULL` reaches emission through several paths (method
// bodies, static-field initializers, enum-const args), so we normalize ALL of them here in one
// always-on pass. Only UPPERCASE macro names appear after `::` as enum constants, so this is safe.
const py::re::Pattern& _RESERVED_REF() {
    static const py::re::Pattern p = py::re::compile(py::str(
        R"PAT(::(TRUE|FALSE|NULL|NAN|INFINITY|ERROR|DELETE|IN|OUT|OPTIONAL|CONST|VOID|NEAR|FAR|THIS|)PAT"
        R"PAT(INTERFACE|DOMAIN)\b)PAT"));
    return p;
}

py::str sanitize_reserved_members(const py::str& text) {
    return _RESERVED_REF().sub([](const py::re::Match& m) { return py::str("::") + m.group(1) + py::str("_"); }, text);
}

py::str apply_std_transforms(const py::str& text_in, const std::optional<CppStandard>& cpp_std) {
    py::str text = text_in;
    if (!cpp_std.has_value() || cpp_std->year < 2011) {
        return text;
    }

    bool had_null = _NULL_WORD().search(text).has_value();
    text = _sub_outside_literals(_NULL_WORD(), py::str("nullptr"), text);
    if (had_null) {
        text = _annotate_first(text, py::str("nullptr"), py::str("// [C++11] nullptr replaces NULL"));
    }

    bool had_iter = _ITER_FOR().search(text).has_value() || _ITER_DECL().search(text).has_value();
    text = _ITER_FOR().sub([](const py::re::Match& m) { return py::str("for(auto ") + m.group(2); }, text);
    text = _ITER_DECL().sub([](const py::re::Match& m) { return m.group(1) + py::str("auto ") + m.group(3); }, text);
    if (had_iter) {
        text = _annotate_first(text, py::str("auto "), py::str("// [C++11] auto type deduction"));
    }

    bool had_const = _STATIC_CONST_INT().search(text).has_value();
    text = _sub_outside_literals(_STATIC_CONST_INT(), py::str("static constexpr int"), text);
    if (had_const) {
        text = _annotate_first(text, py::str("constexpr"), py::str("// [C++11] constexpr replaces const"));
    }

    return text;
}

}  // namespace codebreaker::java::emit::std_transforms
