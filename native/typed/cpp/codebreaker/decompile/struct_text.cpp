// codebreaker/decompile/struct_text.cpp -- C++ port of codebreaker/decompile/struct_text.py
// (see struct_text.hpp for the module's description and for how its three collaborators are handed in).

#include "codebreaker/decompile/struct_text.hpp"

namespace codebreaker::decompile::struct_text {

const py::dict<py::str, int64_t> SIZE_ = {
    {"int1", 1}, {"uint1", 1}, {"xunknown1", 1}, {"undefined1", 1}, {"undefined", 1}, {"bool", 1}, {"char", 1},
    {"int2", 2}, {"uint2", 2}, {"xunknown2", 2}, {"undefined2", 2},
    {"int4", 4}, {"uint4", 4}, {"xunknown4", 4}, {"undefined4", 4}, {"float4", 4}, {"int", 4},
    {"int8", 8}, {"uint8", 8}, {"xunknown8", 8}, {"undefined8", 8}, {"float8", 8}};

namespace {

py::str repeat_(char32_t c, int64_t n) {
    return py::str(std::u32string(static_cast<size_t>(n > 0 ? n : 0), c));
}

// int(text) for the digits of a structure's name (text is all digits). ASCII digits only: another
// script's digits (which Python's int() would also read) raise ValueError here.
int64_t int_of_digits_(const py::str& text) {
    int64_t v = 0;
    for (char32_t c : text.u32()) {
        if (c < U'0' || c > U'9') throw py::ValueError("invalid literal for int() with base 10: " + text.utf8());
        v = v * 10 + static_cast<int64_t>(c - U'0');
    }
    return v;
}

// One kind of access to an offset and how many times the module made it.
struct Vote {
    py::str type;
    int64_t size = 0;
    int64_t count = 0;
};

}  // namespace

// (the field's type as written, its size) for an access `*(base stars)...`.
std::tuple<std::optional<py::str>, int64_t> kind(const py::str& base, const py::str& stars) {
    if (py::len(stars) > 1) {
        return {base + " " + repeat_(U'*', py::len(stars) - 1), 4};
    }
    if (SIZE_.contains(base)) {
        return {base, SIZE_.at(base)};
    }
    return {std::nullopt, 0};
}

// {function: {parameter: (structure, {offset: (type, size)})}} -- the accesses through each
// function's parameters that are a structure's fields, decided over the module: `docs`
// {function: its L2 document} (here the set of functions that have one), `declared` {function: its
// heading as written}. On any trouble in the analysis, nothing: this is for reading, never for meaning.
Plan plan(const py::str& module_path, const py::set<py::str>& docs, const py::dict<int64_t, py::str>& by_va,
          const py::dict<py::str, py::str>& declared, const Analysis& analysis) {
    py::list<Structure> found;
    try {
        found = analysis.recover(module_path);
    } catch (const std::exception&) {
        return Plan();
    }
    py::dict<int64_t, py::str> name_at = by_va.copy();
    py::dict<std::tuple<py::str, py::str>, py::str> owner;   // (function name, parameter name) -> structure
    for (const Structure& s : found) {
        for (const Root& root : s.roots) {
            if (root.kind == "arg" && name_at.contains(root.va) && docs.contains(name_at.at(root.va))) {
                py::str fn = name_at.at(root.va);
                py::list<py::str> params = analysis.parameter_names(fn);
                py::str head = declared.get(fn, "");
                // (a function that takes `this` in a register has it first in its list, before
                // the arguments on the stack that the analysis counts from 0 -- as its heading is
                // written, where the module's C leaves the convention's word out)
                int64_t shift = (head.find("__thiscall") >= 0 || head.find("__fastcall") >= 0) ? 1 : 0;
                if (!root.index) throw py::IndexError("tuple index out of range");
                if (0 <= *root.index + shift && *root.index + shift < py::len(params)) {
                    owner.setdefault({fn, params[*root.index + shift]}, s.name);
                }
            } else if (root.kind == "this" && name_at.contains(root.va) && docs.contains(name_at.at(root.va))) {
                // the object a member function is called on: its first parameter, in ECX
                py::str fn = name_at.at(root.va);
                py::str head = declared.get(fn, "");
                py::list<py::str> params = analysis.parameter_names(fn);
                if (!params.empty() && (head.find("__thiscall") >= 0 || head.find("__fastcall") >= 0)) {
                    owner.setdefault({fn, params[0]}, s.name);
                }
            }
        }
    }
    if (owner.empty()) {
        return Plan();
    }
    // what type each field is read as, over the whole module
    py::dict<py::str, py::dict<int64_t, py::dict<Field, int64_t>>> votes;
    for (const auto& item : owner.items()) {
        const py::str& fn = std::get<0>(std::get<0>(item));
        const py::str& param = std::get<1>(std::get<0>(item));
        const py::str& sname = std::get<1>(item);
        for (const Access& a : analysis.struct_accesses(fn, param)) {
            auto [k, size] = kind(a.base, a.stars);
            if (k && !k->empty()) {
                py::dict<Field, int64_t>& c = votes.setdefault(sname, py::dict<int64_t, py::dict<Field, int64_t>>())
                                                  .setdefault(a.offset, py::dict<Field, int64_t>());
                c[Field(*k, size)] = c.get(Field(*k, size), 0) + 1;
            }
        }
    }
    py::dict<py::str, Fields> fields;   // structure -> {offset: (type, size)}
    for (const auto& vi : votes.items()) {
        const py::str& sname = std::get<0>(vi);
        const py::dict<int64_t, py::dict<Field, int64_t>>& by_off = std::get<1>(vi);
        int64_t at = 0;
        Fields chosen;
        for (int64_t off : py::sorted(by_off)) {
            // the vote with the most accesses; ties go to the greater (type, size)
            std::optional<Vote> best;
            for (const auto& kv : by_off.at(off).items()) {
                Vote v{std::get<0>(std::get<0>(kv)), std::get<1>(std::get<0>(kv)), std::get<1>(kv)};
                if (!best || std::make_tuple(v.count, v.type, v.size) > std::make_tuple(best->count, best->type, best->size)) {
                    best = v;
                }
            }
            if (off < at) {
                continue;   // inside the field before it
            }
            chosen.set(off, Field(best->type, best->size));
            at = off + best->size;
        }
        if (py::len(chosen) >= 2) {
            fields.set(sname, chosen);
        }
    }
    Plan out;
    for (const auto& item : owner.items()) {
        const py::str& fn = std::get<0>(std::get<0>(item));
        const py::str& param = std::get<1>(std::get<0>(item));
        const py::str& sname = std::get<1>(item);
        if (fields.contains(sname)) {
            out.setdefault(fn, py::dict<py::str, Planned>()).set(param, Planned(sname, fields.at(sname)));
        }
    }
    return out;
}

// The header's declarations of the structures `used` (the ones a function was written with a
// field of), each field at its offset, gaps as padding (a line's end after them, whether or not
// there are any: the header the module had is followed by it wherever a plan was made).
py::str declarations(const Plan& planned, const py::set<py::str>& used) {
    py::dict<py::str, Fields> fields;
    for (const auto& by_param : planned.values()) {
        for (const Planned& p : by_param.values()) {
            fields.set(std::get<0>(p), std::get<1>(p));
        }
    }
    // (sorted by the number a name ends in; a name without "_" is an IndexError, as rsplit(...)[1] is)
    py::list<py::str> order = py::sorted(used, [](const py::str& n) -> int64_t {
        int64_t cut = n.rfind("_");
        if (cut < 0) throw py::IndexError("list index out of range");
        py::str tail = n.slice_from(cut + 1);
        return tail.isdigit() ? int_of_digits_(tail) : 0;
    });
    py::list<py::str> decls;
    for (const py::str& sname : order) {
        py::list<py::str> lines;
        lines.append(py::percent("typedef struct %s {", sname));
        int64_t at = 0;
        const Fields& f = fields.at(sname);
        for (int64_t off : py::sorted(f)) {
            const Field& field = f.at(off);
            const py::str& k = std::get<0>(field);
            int64_t size = std::get<1>(field);
            if (off > at) {
                lines.append(py::percent("  uint1 _gap_%x[%d];", at, off - at));
            }
            int64_t star = k.count("*");
            lines.append(py::percent("  %s %sf_%x;", k.rstrip("* "), repeat_(U'*', star), off));
            at = off + size;
        }
        lines.append(py::percent("} %s;", sname));
        decls.append(py::join("\n", lines));
    }
    return py::join("\n", decls) + "\n";
}

}  // namespace codebreaker::decompile::struct_text
