// codebreaker/translate/backends/module_file.cpp -- C++17 port of codebreaker.translate.backends.module_file
// (see module_file.hpp).
#include "codebreaker/translate/backends/module_file.hpp"

namespace codebreaker::translate::backends::module_file {

using backend_asm::Decline;

static py::str S(const char* s) {
    return py::str(s);
}

py::list<py::str> _counts(const Facts& facts, const py::str& lead) {
    int64_t total = facts.total;
    py::list<py::str> out;
    for (const auto& c : facts.order) {
        int64_t count = facts.counts.at(c);
        out.append(py::percent("%s  %-28s %5d  %5.1f%%", lead, facts.titles.at(c), count,
                               100.0 * static_cast<double>(count) / static_cast<double>(py::max(int64_t(1), total))));
    }
    return out;
}

py::list<py::str> banner(const Facts& facts) {
    const py::str& name = facts.name;
    const py::str& stem = facts.stem;
    int64_t total = facts.total;
    const py::str& kind = facts.kind;
    py::list<py::str> out;
    out.append(py::percent("/* %s -- reconstructed source.", name));
    out.append(S(" *"));
    out.append(py::percent(" * STATUS, generated %s by CodeBreaker (verifier settings %s).", facts.date, facts.digest));
    out.append(py::percent(" * %d functions:", total));
    out.extend(_counts(facts, S(" * ")));
    const std::optional<Whole>& whole = facts.whole;
    if (whole.has_value() && whole->linked) {
        out.append(S(" *"));
        out.append(py::percent(
            " * Builds as a whole module: YES (%s). Every original export is present; a function the",
            whole->exports_same == whole->exports_total
                ? S("all the original's exports match")
                : py::percent("%d of %d exports match", whole->exports_same, whole->exports_total)));
        out.append(S(" * compiler rejected, or a call to code with no reconstruction, is a stand-in that returns 0"));
        out.append(py::percent(" * (%d rejected, %d absent). The built image is %s.", whole->rejected, whole->absent,
                               whole->built));
        out.append(S(" * Runs in place of the original: NOT TESTED as a whole."));
        out.append(S(" *"));
    } else {
        out.append(S(" *"));
        out.append(S(" * Builds as a whole module: ") +
                   (whole.has_value() ? py::percent("NOT YET (%s).", whole->error)
                                      : py::percent("not built here (run `codebreaker build %s`).", name)));
        out.append(S(" * Functions are compiled and checked one at a time; the module's data (globals, tables,"));
        out.append(S(" * resources) is referred to by its address in the original unless the whole module is built."));
        out.append(S(" * Runs in place of the original: NOT TESTED."));
        out.append(S(" *"));
        out.append(py::percent(" * VERIFIED = rebuilt from this text with %s and run beside the original in %s:",
                               kind == "powerpc" ? S("a PowerPC compiler") : S("OpenWatcom"),
                               kind == "powerpc" ? S("a PowerPC interpreter") : S("an x86 emulator")));
        out.append(S(" * same return value, same calls out of the module with the same arguments, same memory changed,"));
        out.append(S(" * same stack and saved registers, on every input tried. It is evidence, not proof. Names of"));
        out.append(S(" * internal functions (sub_...) and locals are placeholders; no original names are claimed."));
        out.append(S(" * Written in after judging, changing no code: the SDK's names for constants passed to"));
        out.append(py::percent(" * Windows (with their values in %s.h), and names for locals, parameters and globals", stem));
        out.append(S(" * taken from the Windows functions they are passed to or hold the result of."));
        out.append(S(" * IDENTIFIED = the function's bytes are those of a function in a library that came with the"));
        out.append(S(" * compiler; it is named after that function. It is reconstructed and judged all the same."));
    }
    if (facts.real != 0) {
        out.append(py::percent(" * Imports by number are written with their real names (%d of them; %s.h lists the",
                               facts.real, stem));
        out.append(S(" * module and number of each). The text that was built and judged calls them MODULE_number."));
    }
    out.append(S(" */"));
    out.append(S(""));
    if (facts.header) {
        out.append(py::percent("#include \"%s.h\"", stem));
    }
    out.append(S(""));
    return out;
}

py::str unit(const Unit& u) {
    const std::optional<py::str>& kind = u.node;
    if (kind.has_value() && *kind == "MethodDecl") {
        py::str first = (u.comments.has_value() && u.comments->size() > 0) ? (*u.comments)[0] : S("");
        py::str text = (u.text.has_value() && !u.text->empty()) ? *u.text : S("");
        return py::percent("/* %s */\n%s\n", first, text);
    }
    if (kind.has_value() && *kind == "Comment") {
        py::str text = (u.text.has_value() && !u.text->empty()) ? *u.text : S("");
        return py::percent("/* %s */\n", text);
    }
    throw Decline(py::percent("document %s", kind));
}

py::list<std::tuple<py::str, py::str>> c(const Document& doc, const Facts& facts, int64_t split) {
    if (!(doc.node.has_value() && *doc.node == "Module")) {
        throw Decline(py::percent("document %s", doc.node));
    }
    py::list<py::str> parts;
    for (const auto& u : doc.units) {
        parts.append(unit(u));
    }
    py::list<py::str> head = banner(facts);
    const py::str& name = facts.name;
    const py::str& stem = facts.stem;
    // pieces = [parts] if not split or len(parts) <= split else [parts[k:k + split] for k in range(0, len(parts), split)]
    py::list<py::list<py::str>> pieces;
    if (split == 0 || parts.size() <= split) {
        pieces.append(parts);
    } else if (split > 0) {
        for (int64_t k = 0; k < parts.size(); k += split) {
            pieces.append(parts.slice(k, k + split));
        }
    }  // a negative split: range(0, len, split) is empty, so no pieces
    py::list<std::tuple<py::str, py::str>> out;
    for (int64_t n = 0; n < pieces.size(); n++) {
        const py::list<py::str>& piece = pieces[n];
        py::str target = pieces.size() == 1 ? stem + S(".c") : py::percent("%s_%d.c", stem, n + 1);
        py::list<py::str> top;
        if (n == 0) {
            top = head;
        } else {
            top.append(py::percent("/* %s -- reconstructed source, part %d of %d (the status is in %s_1.c). */", name, n + 1,
                                   pieces.size(), stem));
            top.append(S(""));
            for (const auto& ln : head) {
                if (ln.startswith(S("#include"))) top.append(ln);
            }
            top.append(S(""));
        }
        out.append(std::make_tuple(target, py::join(S("\n"), top) + S("\n") + py::join(S("\n"), piece)));
    }
    return out;
}

py::list<py::str> asm_banner(const Facts& facts) {
    py::list<py::str> out;
    out.append(py::percent("%s -- reconstructed source.", facts.name));
    out.append(S(""));
    out.append(py::percent("STATUS, generated %s by CodeBreaker (verifier settings %s).", facts.date, facts.digest));
    out.append(py::percent("%d entry points or functions:", facts.total));
    for (const auto& x : _counts(facts, S("; "))) {
        out.append(x.slice_from(2));
    }
    if (facts.how.has_value() && !facts.how->empty()) {
        out.append(S(""));
        out.append(facts.how->slice_to(160));
    }
    for (const auto& row : facts.rows) {
        out.append(py::percent("  %-10s %-28s %s %s", row.va, row.name, row.title, row.what));
    }
    out.append(S(""));
    return out;
}

py::str asm_(const backend_asm::Document& doc, const Facts& facts) {
    py::list<backend_asm::Unit> units;
    for (const auto& t : asm_banner(facts)) {
        backend_asm::Unit u;
        u.node = S("Comment");
        u.text = t;
        units.append(u);
    }
    // units += [u for u in doc.get("units") or []] or [{"node": "MachineFragment", "asm": ...}]
    py::list<backend_asm::Unit> doc_units;
    if (const auto* list = std::get_if<py::list<backend_asm::Unit>>(&doc.units)) {
        doc_units = *list;
    } else {
        // the Python tolerates a single unit dict here: iterating a non-empty one yields its keys (str),
        // which the assembly back end cannot write; an empty one is falsy
        if (!std::get<backend_asm::Unit>(doc.units).empty()) {
            throw py::AttributeError("'str' object has no attribute 'get'");
        }
    }
    if (doc_units.size() > 0) {
        units.extend(doc_units);
    } else {
        backend_asm::Unit u;
        u.node = S("MachineFragment");
        u.asm_ = S("; (no source could be written)\n");
        units.append(u);
    }
    backend_asm::Document module_doc;
    module_doc.node = S("Module");
    module_doc.units = units;
    return backend_asm::module(module_doc);
}

}  // namespace codebreaker::translate::backends::module_file
