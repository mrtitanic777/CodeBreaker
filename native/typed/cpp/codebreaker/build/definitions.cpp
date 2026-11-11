// codebreaker/build/definitions.cpp -- C++17 port of codebreaker.build.definitions (see definitions.hpp).

#include "codebreaker/build/definitions.hpp"

#include <algorithm>
#include <vector>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/formats/ne.hpp"
#include "codebreaker/formats/pe.hpp"
#include "py_os.hpp"

namespace codebreaker::build::definitions {

namespace corpus = codebreaker::base::corpus;
namespace ne = codebreaker::formats::ne;
namespace pe = codebreaker::formats::pe;

namespace {

// d["key"] of a record whose key may be absent: KeyError when it is
int64_t key(const std::optional<int64_t>& v) {
    if (!v) throw py::KeyError("key not found");
    return *v;
}

// "NE" test of a path: corpus.lane(path) == "ne"
bool is_ne(const py::str& path) {
    std::optional<py::str> lane = corpus::lane(path);
    return lane && *lane == py::str("ne");
}

// os.path.splitext(os.path.basename(path))[0]
py::str stem_of(const py::str& path) { return std::get<0>(py::os::path::splitext(py::os::path::basename(path))); }

}  // namespace

py::str Export::repr() const {
    return py::percent("<export %s @%d %s>", name && !name->empty() ? *name : py::str("(no name)"), ordinal, kind);
}

bool is_library(const py::str& path) {
    if (is_ne(path)) {
        return (ne::NEFile(path).h.flags & 0x8000) != 0;
    }
    return pe::open_pe(path).is_dll();
}

py::str module_name(const py::str& path) {
    py::str stem = stem_of(path).upper();
    try {
        if (is_ne(path)) {   // (a name field padded with NULs or spaces)
            py::str nm = ne::NEFile(path).module_name().replace(py::chr(0), py::str(" ")).strip();
            return nm.empty() ? stem : nm;
        }
        py::str nm = std::get<0>(py::os::path::splitext(pe::open_pe(path).module_name())).upper();
        return nm.empty() ? stem : nm;
    } catch (const py::ValueError&) {
        return stem;
    } catch (const py::IndexError&) {
        return stem;
    } catch (const py::StructError&) {
        return stem;
    }
}

py::str description(const py::str& path) {
    if (!is_ne(path)) {
        return py::str("");
    }
    py::list<std::tuple<py::str, int64_t>> names = ne::NEFile(path).nonresident_names();
    return names.empty() ? py::str("") : std::get<0>(names[0]).replace(py::chr(0), py::str(" ")).strip();
}

py::list<Export> exports(const py::str& path) {
    py::list<Export> out;
    if (is_ne(path)) {
        ne::NEFile nef(path);
        py::dict<int64_t, ne::Segment> segs;
        for (const ne::Segment& s : nef.segments()) {
            segs.set(s.idx, s);
        }
        py::dict<int64_t, std::tuple<py::str, bool>> names;
        py::list<std::tuple<bool, py::list<std::tuple<py::str, int64_t>>>> tables;
        tables.append(std::make_tuple(true, nef.resident_names().slice_from(1)));
        tables.append(std::make_tuple(false, nef.nonresident_names().slice_from(1)));
        for (const auto& [resident, table] : tables) {
            for (const auto& [name, ordinal] : table) {
                names.setdefault(ordinal, std::make_tuple(name, resident));
            }
        }
        for (const ne::Entry& e : nef.entries()) {
            if (!(e.kind == py::str("FIXED") || e.kind == py::str("MOVEABLE"))) {
                continue;
            }
            std::optional<py::str> name;
            bool resident = false;
            if (names.contains(e.ordinal)) {
                const auto& nr = names.at(e.ordinal);
                name = std::get<0>(nr);
                resident = std::get<1>(nr);
            }
            if ((!name || name->empty()) && !(key(e.flags) & 1)) {   // an entry point for the module's own far
                continue;                                             // calls between segments, not an export
            }
            std::optional<ne::Segment> seg;
            if (segs.contains(key(e.seg))) seg = segs.at(key(e.seg));
            py::str kind;
            if (key(e.seg) == 0xFE) {   // a constant, not a place in the module
                kind = py::str("data");
            } else {
                kind = (!seg || seg->kind == py::str("DATA")) ? py::str("data") : py::str("code");
            }
            out.append(Export(e.ordinal, name, std::make_tuple(key(e.seg), key(e.offset)), kind, resident, py::str(""),
                              (key(e.flags) & 2) != 0));
        }
        return out;
    }
    pe::PEFile p = pe::open_pe(path);
    int64_t lo = 0, size = 0;
    if (!p.datadirs.empty()) {
        lo = std::get<0>(p.datadirs[0]);
        size = std::get<1>(p.datadirs[0]);
    }
    std::vector<std::tuple<int64_t, int64_t>> code;
    for (const pe::Section& s : p.sections) {
        if (s.chars & 0x20000020) {
            code.emplace_back(s.vaddr, s.vaddr + std::max(s.vsize, s.rawsize));
        }
    }
    for (const auto& [ordinal, name, rva] : p.exports()) {
        if (lo <= rva && rva < lo + size) {   // inside the export table: a forwarder's text
            std::optional<int64_t> off = p._rva_to_off(rva);
            int64_t from = off ? *off : 0;   // (a None start of data[off:] and of index(b"\0", off) is 0)
            int64_t at = p.data.index(py::bytes(std::string(1, '\0')), from);
            py::str target = p.data.slice(from, at).decode("latin-1");
            out.append(Export(ordinal, name, uint64_t(0), py::str("forward"), false, target));
        } else {
            bool in_code = false;
            for (const auto& [a, b] : code) {
                if (a <= rva && rva < b) in_code = true;
            }
            out.append(Export(ordinal, name, p.image_base + static_cast<uint64_t>(rva), in_code ? py::str("code") : py::str("data")));
        }
    }
    std::vector<Export> sorted_out(out.vec());
    std::stable_sort(sorted_out.begin(), sorted_out.end(), [](const Export& a, const Export& b) { return a.ordinal < b.ordinal; });
    return py::list<Export>(sorted_out);
}

py::set<Import> imports(const py::str& path) {
    py::set<Import> out;
    if (is_ne(path)) {
        ne::NEFile nef(path);
        py::list<py::str> refs = nef.module_refs();
        for (const ne::Segment& s : nef.segments()) {
            for (const ne::Relocation& r : nef.relocations(s)) {
                if (!(r.kind == py::str("IMPORTORD") || r.kind == py::str("IMPORTNAME"))) {
                    continue;
                }
                int64_t module = key(r.module);
                py::str mod = (0 < module && module <= py::len(refs)) ? refs[module - 1].upper() : py::str("?");
                if (r.kind == py::str("IMPORTORD")) {
                    out.add(Import(mod, key(r.ordinal)));
                } else {
                    out.add(Import(mod, nef.imported_name(key(r.name_off))));
                }
            }
        }
        return out;
    }
    for (const auto& [slot, dn] : pe::open_pe(path).imports().items()) {
        (void)slot;
        const py::str& dll = std::get<0>(dn);
        const py::str& name = std::get<1>(dn);
        py::str mod = std::get<0>(py::os::path::splitext(dll)).upper();
        if (name.startswith(py::str("#"))) {
            out.add(Import(mod, py::int_parse(name.slice_from(1))));
        } else {
            out.add(Import(mod, name));
        }
    }
    return out;
}

py::str definition_file(const py::str& path) {
    bool is_ne_module = is_ne(path);
    py::list<py::str> lines;
    lines.append(py::percent("; %s -- module definition, read from the original's own tables.", py::os::path::basename(path).lower()));
    lines.append(py::percent("%s %s", is_library(path) ? py::str("LIBRARY") : py::str("NAME"), module_name(path)));
    py::str desc = description(path);
    if (!desc.empty()) {
        lines.append(py::percent("DESCRIPTION '%s'", desc.replace(py::str("'"), py::str("''"))));
    }
    if (is_ne_module) {
        ne::NEFile nef(path);
        lines.append(py::str("EXETYPE WINDOWS"));
        lines.append(py::percent("HEAPSIZE %d", nef.h.heap));
        if (nef.h.stack) {
            lines.append(py::percent("STACKSIZE %d", nef.h.stack));
        }
    }
    py::list<Export> found = exports(path);
    if (!found.empty()) {
        lines.append(py::str("EXPORTS"));
    }
    for (const Export& e : found) {
        bool has_name = e.name && !e.name->empty();
        py::str text = py::percent("    %-32s @%d", has_name ? *e.name : py::percent("ordinal_%d", e.ordinal), e.ordinal);
        if (e.kind == py::str("forward")) {
            text = py::percent("    %s = %s @%d", e.name ? *e.name : py::str("None"), e.target, e.ordinal);
        }
        if (!has_name) {
            text = text + py::str(" NONAME");
        }
        if (is_ne_module && e.resident) {
            text = text + py::str(" RESIDENTNAME");
        }
        if (e.kind == py::str("data")) {
            text = text + (!is_ne_module ? py::str(" DATA") : py::str("        ; data"));
        }
        lines.append(text.rstrip());
    }
    py::set<Import> imported = imports(path);
    std::vector<Import> taken;
    for (const Import& i : imported) taken.push_back(i);
    auto text_of = [](const Import& i) -> py::str {
        const auto& v = std::get<1>(i);
        return std::holds_alternative<py::str>(v) ? std::get<py::str>(v) : py::to_str(std::get<int64_t>(v));
    };
    std::stable_sort(taken.begin(), taken.end(), [&](const Import& a, const Import& b) {
        return std::make_tuple(std::get<0>(a), text_of(a)) < std::make_tuple(std::get<0>(b), text_of(b));
    });
    if (!taken.empty()) {
        lines.append(py::str("IMPORTS"));
        for (const Import& i : taken) {
            lines.append(py::percent("    %s.%s", std::get<0>(i), text_of(i)));
        }
    }
    return py::join(py::str("\n"), lines) + py::str("\n");
}

std::tuple<py::list<py::str>, py::list<std::tuple<Export, py::str>>> export_directives(
    const py::str& path, const std::function<std::optional<py::str>(const Export&)>& symbol_at) {
    bool is_ne_module = is_ne(path);
    py::list<py::str> lines;
    py::list<std::tuple<Export, py::str>> missing;
    for (const Export& e : exports(path)) {
        if (e.kind == py::str("forward")) {
            missing.append(std::make_tuple(e, py::percent("forwarded to %s: this linker cannot forward", e.target)));
            continue;
        }
        std::optional<py::str> symbol = symbol_at(e);
        if (!symbol || symbol->empty()) {
            missing.append(std::make_tuple(e, e.kind == py::str("data") ? py::str("exported data is not a symbol of its own")
                                                                         : py::str("no function was reconstructed at its address")));
            continue;
        }
        py::str name = (e.name && !e.name->empty()) ? *e.name : py::percent("ordinal_%d", e.ordinal);
        if (!py::re::fullmatch(py::str("[A-Za-z_][\\w]*"), name)) {
            name = py::percent("'%s'", name);
        }
        // (NODATA: the entry is not marked as using the module's data segment -- the loader then
        // leaves its prologue alone, and the function runs on its caller's DS, as the original's)
        lines.append(py::percent("export %s.%d='%s'%s%s", name, e.ordinal, *symbol,
                                 is_ne_module && e.resident ? py::str(" resident") : py::str(""),
                                 is_ne_module && e.kind == py::str("code") && !e.shared_data ? py::str(" nodata") : py::str("")));
    }
    return std::make_tuple(lines, missing);
}

py::str public_header(const py::str& path, const std::function<std::optional<py::str>(const Export&)>& declaration_at) {
    py::str stem = py::re::sub(py::str("\\W"), py::str("_"), stem_of(path)).upper();
    py::str base = py::os::path::basename(path).lower();
    int64_t dot = base.rfind(py::str("."));
    py::str head = dot >= 0 ? base.slice(0, dot) : base;   // (rsplit(".", 1)[0])
    py::list<py::str> out;
    out.append(py::percent("/* %s -- the functions %s exports, as reconstructed. Argument names and types are the",
                           head + py::str("_public.h"), module_name(path)));
    out.append(py::str(" * decompiler's; the export names and ordinals are the original's. */"));
    out.append(py::percent("#ifndef %s_PUBLIC_H", stem));
    out.append(py::percent("#define %s_PUBLIC_H", stem));
    out.append(py::str(""));
    for (const Export& e : exports(path)) {
        py::str label = py::percent("%s @%d", (e.name && !e.name->empty()) ? *e.name : py::str("(by ordinal only)"), e.ordinal);
        std::optional<py::str> decl;
        if (e.kind == py::str("code")) decl = declaration_at(e);
        if (decl && !decl->empty()) {
            out.append(py::percent("%-72s /* %s */", *decl, label));
        } else {
            py::str what = e.kind == py::str("forward")  ? py::str("forwarded to ") + e.target
                           : e.kind == py::str("data")   ? py::str("data")
                                                         : py::str("no reconstruction at its address");
            out.append(py::percent("/* %s: %s */", label, what));
        }
    }
    out.append(py::str(""));
    out.append(py::str("#endif"));
    out.append(py::str(""));
    return py::join(py::str("\n"), out);
}

}  // namespace codebreaker::build::definitions
