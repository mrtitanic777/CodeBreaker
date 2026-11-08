// codebreaker/java/twophase.cpp -- C++ port of codebreaker.java.twophase (see twophase.hpp).

#include "codebreaker/java/twophase.hpp"

#include <algorithm>
#include <functional>

namespace codebreaker::java::twophase {

const py::re::Pattern _CALL = py::re::compile(py::str("(?<![\\w.>:])(?:this\\s*\\.\\s*)?([A-Za-z_]\\w*)\\s*\\("));
const py::set<py::str> _KEYWORDS = {py::str("if"),     py::str("for"),  py::str("while"), py::str("switch"),
                                    py::str("catch"),  py::str("return"), py::str("new"),  py::str("super"),
                                    py::str("this"),   py::str("synchronized")};
const py::re::Pattern _SUPER_ARGS = py::re::compile(py::str("^\\s*super\\s*\\((.*)\\)\\s*;"));

py::str _ctor_text(const ClassInfo& cls) {
    py::list<py::str> parts;
    for (const Constructor& c : cls.constructors) {
        if (c.body && py::truthy(*c.body)) {
            for (const Statement& s : *c.body) {
                parts.append(std::get<1>(s));
            }
        }
    }
    for (const Field& f : cls.instance_fields) {
        if (py::truthy(f.initializer)) {
            parts.append(*f.initializer);
        }
    }
    for (const Body& block : cls.inst_inits) {
        for (const Statement& s : block) {
            parts.append(std::get<1>(s));
        }
    }
    return py::join(py::str("\n"), parts);
}

py::set<py::str> compute(const py::list<ClassInfo>& classes, const py::set<py::str>& values) {
    py::dict<py::str, ClassInfo> by_name;
    for (const ClassInfo& c : classes) {
        by_name.set(c.name, c);
    }
    py::dict<py::str, py::list<py::str>> children;
    for (const ClassInfo& c : classes) {
        if (c.parent && by_name.contains(*c.parent)) {
            if (!children.contains(*c.parent)) {
                children.set(*c.parent, py::list<py::str>());
            }
            children.at(*c.parent).append(c.name);
        }
    }

    auto descendants = [&](const py::str& name) {
        py::list<py::str> out;
        py::list<py::str> todo;
        if (children.contains(name)) {
            todo.extend(children.at(name));
        }
        while (py::truthy(todo)) {
            py::str n = todo.pop();
            out.append(n);
            if (children.contains(n)) {
                todo.extend(children.at(n));
            }
        }
        return out;
    };

    auto instance_methods = [&](const ClassInfo& c) {
        py::set<py::str> out;
        for (const Method& m : c.methods) {
            if (!m.is_ctor && !m.is_static) {
                out.add(m.name);
            }
        }
        return out;
    };

    py::set<py::str> roots;
    for (const ClassInfo& c : classes) {
        if (c.is_interface || c.is_enum || (!py::truthy(c.constructors) && !py::truthy(c.instance_fields))) {
            continue;
        }
        py::set<py::str> called;
        for (const py::str& n : _CALL.findall(_ctor_text(c))) {
            if (!_KEYWORDS.contains(n)) {
                called.add(n);
            }
        }
        if (!py::truthy(called)) {
            continue;
        }
        py::set<py::str> own;
        const ClassInfo* k = &c;
        while (k != nullptr) {   // its own and inherited instance methods
            own.update(instance_methods(*k));
            k = (k->parent && by_name.contains(*k->parent)) ? &by_name.at(*k->parent) : nullptr;
        }
        py::set<py::str> abstract_;
        for (const Method& m : c.methods) {
            if (m.is_abstract) {
                abstract_.add(m.name);
            }
        }
        py::set<py::str> overridden;
        for (const py::str& d : descendants(c.name)) {
            overridden.update(instance_methods(by_name.at(d)));
        }
        bool hit = false;
        for (const py::str& n : called) {
            if (own.contains(n) && (overridden.contains(n) || abstract_.contains(n))) {
                hit = true;
                break;
            }
        }
        if (hit) {
            roots.add(c.name);
        }
    }

    py::set<py::str> picked;
    for (const py::str& r : roots) {
        picked.add(r);
        picked.update(descendants(r));
    }

    auto drop = [&](const py::str& name) {
        picked.discard(name);
        for (const py::str& d : descendants(name)) {
            picked.discard(d);
        }
    };

    bool changed = true;
    while (changed) {
        changed = false;
        std::vector<py::str> names(picked.begin(), picked.end());
        std::sort(names.begin(), names.end());
        for (const py::str& name : names) {
            const ClassInfo& c = by_name.at(name);
            if (values.contains(name)) {
                drop(name);
                changed = true;
                break;
            }
            if (c.parent && by_name.contains(*c.parent) && !picked.contains(*c.parent)) {
                bool with_args = false;
                for (const Constructor& k : c.constructors) {
                    if (!k.body) {
                        continue;
                    }
                    for (const Statement& s : *k.body) {
                        const py::str& t = std::get<1>(s);
                        std::optional<py::re::Match> m = _SUPER_ARGS.match(t);
                        if (m && py::truthy(m->group(1).strip())) {
                            with_args = true;
                        }
                    }
                }
                if (with_args) {
                    drop(name);
                    changed = true;
                    break;
                }
            }
        }
    }
    return picked;
}

int64_t _close(const py::str& text, int64_t i) {
    int64_t depth = 1;
    std::optional<py::str> q;
    while (i < py::len(text)) {
        // (text[i]: a negative i counts from the end, and past the front is an IndexError)
        int64_t at = i < 0 ? i + py::len(text) : i;
        if (at < 0) {
            throw py::IndexError("string index out of range");
        }
        py::str ch = text.slice(at, at + 1);
        if (q) {
            if (ch == "\\") {
                i += 1;
            } else if (ch == *q) {
                q.reset();
            }
        } else if (ch == "\"" || ch == "'") {
            q = ch;
        } else if (ch == "(") {
            depth += 1;
        } else if (ch == ")") {
            depth -= 1;
            if (depth == 0) {
                return i + 1;
            }
        }
        i += 1;
    }
    return -1;
}

std::tuple<py::str, int64_t> rewrite_news(const py::str& text, const py::set<py::str>& names) {
    if (!py::truthy(names) || !text.contains(py::str("new "))) {
        return {text, 0};
    }
    std::vector<py::str> sorted_names(names.begin(), names.end());
    std::stable_sort(sorted_names.begin(), sorted_names.end(), [](const py::str& a, const py::str& b) { return a.size() > b.size(); });
    py::list<py::str> escaped;
    for (const py::str& n : sorted_names) {
        escaped.append(py::re::escape(n));
    }
    py::re::Pattern rx = py::re::compile(py::str("\\bnew\\s+(") + py::join(py::str("|"), escaped) + py::str(")\\s*\\("));
    py::list<py::str> out;
    int64_t pos = 0;
    int64_t n = 0;
    for (const py::re::Match& m : rx.finditer(text)) {
        if (m.start() < pos) {
            continue;
        }
        int64_t end = _close(text, m.end());
        if (end < 0) {
            continue;
        }
        py::str args = text.slice(m.end(), end - 1);
        if (!py::truthy(args.strip()) && text.slice(end, end + 11) == ")->j2xInit(") {
            continue;
        }
        auto [inner, k] = rewrite_news(args, names);
        out.append(text.slice(pos, m.start()));
        out.append(py::percent("(new %s())->j2xInit(%s)", m.group(1), inner));
        pos = end;
        n += 1 + k;
    }
    out.append(text.slice(pos, py::len(text)));
    return {py::join(py::str(""), out), n};
}

std::tuple<py::str, int64_t> rewrite_stack(const py::str& text, const py::set<py::str>& names) {
    if (!py::truthy(names)) {
        return {text, 0};
    }
    std::vector<py::str> sorted_names(names.begin(), names.end());
    std::stable_sort(sorted_names.begin(), sorted_names.end(), [](const py::str& a, const py::str& b) { return a.size() > b.size(); });
    py::list<py::str> escaped;
    for (const py::str& n : sorted_names) {
        escaped.append(py::re::escape(n));
    }
    py::re::Pattern rx = py::re::compile(py::str("(?<![\\w:<*&])(") + py::join(py::str("|"), escaped) +
                                         py::str(")\\s+(_j2xleak\\d+)\\s*(?:\\((.*?)\\))?\\s*;"));
    int64_t count = 0;
    py::str result = rx.sub(
        [&](const py::re::Match& m) {
            count += 1;
            return py::percent("%s %s; %s.j2xInit(%s);", m.group(1), m.group(2), m.group(2),
                               m.group_opt(3).value_or(py::str("")));
        },
        text);
    return {result, count};
}

RunResult run(const py::str& out_dir, const py::set<py::str>& names) {
    int64_t sites = 0;
    py::list<py::str> files;
    if (!py::truthy(names)) {
        return RunResult{0, py::list<py::str>()};
    }
    std::vector<py::str> listing = py::os::listdir(out_dir).vec();
    std::sort(listing.begin(), listing.end());
    for (const py::str& fn : listing) {
        if (!fn.endswith(py::str(".cpp")) && !fn.endswith(py::str(".h"))) {
            continue;
        }
        py::str path = py::os::path::join(out_dir, fn);
        py::str text;
        {
            py::file f = py::open(path, py::str("r"), py::str("utf-8"), py::str("replace"), py::str(""));
            text = f.read_text();
            f.close();
        }
        auto [new_text, n] = rewrite_news(text, names);
        auto [new_text2, k] = rewrite_stack(new_text, names);
        if (n != 0 || k != 0) {
            py::file f = py::open(path, py::str("w"), py::str("utf-8"), std::nullopt, py::str(""));
            f.write(new_text2);
            f.close();
            sites += n + k;
            files.append(fn);
        }
    }
    return RunResult{sites, files};
}

}  // namespace codebreaker::java::twophase
