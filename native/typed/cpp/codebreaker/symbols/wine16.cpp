// codebreaker/symbols/wine16.cpp -- C++ from codebreaker.symbols.wine16 (typed port).

#include "codebreaker/symbols/wine16.hpp"

namespace codebreaker::symbols::wine16 {

const py::dict<py::str, int64_t> WIDTH = {
    {py::str("word"), 2}, {py::str("s_word"), 2}, {py::str("long"), 4}, {py::str("segptr"), 4},
    {py::str("segstr"), 4}, {py::str("ptr"), 4}, {py::str("str"), 4}};
const py::set<py::str> POINTER = {py::str("ptr"), py::str("str"), py::str("segptr"), py::str("segstr")};

// (the ordinal is read as a run of non-blanks and checked to be decimal digits afterwards: the Python
// pattern's (\d+)\s+ means exactly that, and the runtime's regex \d takes ASCII digits only where
// Python's takes every Unicode decimal digit)
const py::re::Pattern _LINE = py::re::compile(py::str(
    "^\\s*(\\S+)\\s+(pascal|cdecl|varargs|stub|stdcall)\\s+((?:-\\w+(?:=\\S+)?\\s+)*)(\\w+)\\s*(?:\\(([^)]*)\\))?\\s*(?:#\\s*\\(([^)]*)\\))?"));
py::dict<py::str, py::dict<int64_t, Export>> _cache;

// int(s) of a str of Unicode decimal digits (Python's int takes them all; digit values run 0-9 in each
// range of ten, so the offset in the range, modulo 10, is the value). Python's int has no limit;
// this one raises OverflowError past int64.
static int64_t decimal_value(const py::str& digits) {
    int64_t n = 0;
    for (char32_t c : digits.u32()) {
        int64_t d = 0;
        for (const auto& r : py::unicode_::decimal_ranges) {
            if (c >= r[0] && c <= r[1]) {
                d = static_cast<int64_t>(c - r[0]) % 10;
                break;
            }
        }
        if (n > (INT64_MAX - d) / 10) {
            throw py::OverflowError("int too large for int64");
        }
        n = n * 10 + d;
    }
    return n;
}

py::str _folder() {
    return py::settings::get_str(py::str("wine16"));   // (settings.get(...) or "": "" when not set)
}

py::str _module(const py::str& spec_name) {
    py::str stem = py::split(spec_name, py::str("."))[0].upper();
    return py::dict<py::str, py::str>{{py::str("KRNL386"), py::str("KERNEL")}}.get(stem, stem);
}

py::dict<int64_t, Export> exports(const py::str& module_name) {
    py::str module = module_name.upper();
    if (!_cache.contains(module)) {
        _cache.update(_read_all());
        _cache.setdefault(module, py::dict<int64_t, Export>());
    }
    return _cache.at(module);
}

py::dict<py::str, py::dict<int64_t, Export>> _read_all() {
    py::dict<py::str, py::dict<int64_t, Export>> out;
    py::str root = _folder();
    if (!py::truthy(root) || !py::os::path::isdir(root)) {
        return out;
    }
    static const py::re::Pattern folder_kind = py::re::compile(py::str("\\.(dll|exe|drv)16$"));
    for (const py::str& d : py::os::listdir(root)) {
        if (!folder_kind.search(d)) {
            continue;
        }
        for (const py::str& f : py::os::listdir(py::os::path::join(root, d))) {
            if (f.endswith(py::str(".spec"))) {
                py::file fh = py::open(py::os::path::join(root, d, f), py::str("r"), py::str("latin1"));
                py::str text = fh.read_text();
                fh.close();
                out.setdefault(_module(f), py::dict<int64_t, Export>()).update(read_spec(text));
            }
        }
    }
    return out;
}

py::dict<int64_t, Export> read_spec(const py::str& text) {
    py::dict<int64_t, Export> out;
    for (const py::str& line : py::split(text, py::str("\n"))) {
        std::optional<py::re::Match> m = _LINE.match(line);
        if (!m) {
            continue;
        }
        py::str ordinal = m->group(1);
        if (!ordinal.isdecimal()) {
            continue;
        }
        py::str conv = m->group(2);
        std::optional<py::str> flags = m->group_opt(3);
        py::str name = m->group(4);
        std::optional<py::str> args = m->group_opt(5);
        std::optional<py::str> commented = m->group_opt(6);
        bool stub = conv == py::str("stub");
        std::optional<py::str> listed = stub ? commented : args;
        std::optional<py::list<py::str>> kinds;
        if (!listed) {
            kinds = std::nullopt;   // (nothing said of its arguments)
        } else {
            py::list<py::str> found;
            for (const py::str& a : py::split(*listed)) {
                if (py::truthy(a)) {
                    found.append(a);
                }
            }
            bool unread = false;
            for (const py::str& a : found) {
                if (!WIDTH.contains(a)) {
                    unread = true;
                }
            }
            if (unread) {
                kinds = std::nullopt;   // (a kind not read here: double, int64, ...)
            } else {
                kinds = found;
            }
        }
        out.set(decimal_value(ordinal),
                Export{name, stub ? py::str("pascal") : conv, kinds, flags.value_or(py::str("")).contains(py::str("-ret16")), stub});
    }
    return out;
}

py::str c_type(const py::str& kind) {
    if (POINTER.contains(kind)) {
        return py::str("void __far *");
    }
    return py::dict<py::str, py::str>{{py::str("word"), py::str("uint2")}, {py::str("s_word"), py::str("int2")}}.get(kind, py::str("uint4"));
}

}  // namespace codebreaker::symbols::wine16
