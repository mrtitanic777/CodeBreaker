// codebreaker/formats/xex_imports.cpp -- typed C++ of codebreaker.formats.xex_imports (see xex_imports.hpp).
#include "codebreaker/formats/xex_imports.hpp"

#include <cstdio>
#include <map>
#include <string>

#include "codebreaker/formats/powerpc_image.hpp"
#include "codebreaker/formats/xex.hpp"
#include "codebreaker/knowledge/sources.hpp"

namespace codebreaker::formats::xex_imports {

namespace sources = codebreaker::knowledge::sources;

py::dict<py::str, py::dict<int64_t, py::str>> _names() {
    // knowledge.table("xbox360", "ordinal_names"): the authored kind (its rows, from the base when it is open)
    sources::Rows table;
    if (sources::store_open()) {
        table = sources::authored(py::str("xbox360"), py::str("ordinal_names"));
    } else {
        const sources::Source& src = sources::AUTHORED().at(std::make_tuple(py::str("xbox360"), py::str("ordinal_names")));
        try {
            table = src.read(src.file);
        } catch (const py::OSError&) {
            table = sources::Rows();
        } catch (const py::ValueError&) {
            table = sources::Rows();
        }
    }
    py::dict<py::str, py::dict<int64_t, py::str>> out;
    for (const auto& kv : table.items()) {
        const py::str& lib = std::get<0>(kv);
        const sources::Json& names = std::get<1>(kv);
        if (lib.startswith(py::str("_"))) continue;
        if (names.kind != sources::Json::Dict) throw py::AttributeError("the names of a library are not a dict");
        py::dict<int64_t, py::str> by_ordinal;
        for (const auto& nv : names.dict.items()) {
            const sources::Json& v = std::get<1>(nv);
            if (v.kind != sources::Json::Str) throw py::TypeError("a name that is not text");
            by_ordinal.set(py::int_parse(std::get<0>(nv), 10), v.s);
        }
        out.set(lib, by_ordinal);
    }
    return out;
}

namespace {

struct Entry {
    std::optional<int64_t> slot, thunk;
};

// struct.unpack_from(">I" / ">HH" / ">NI", data, off) with the bounds Python checks
void need(const py::bytes& data, int64_t off, int64_t size) {
    if (off < 0 ? off + py::len(data) < 0 : off > py::len(data)) throw py::StructError("offset out of range");
    if (py::len(data) - off < size) throw py::StructError("unpack_from requires a larger buffer");
}

}  // namespace

py::list<Import> imports(const py::str& path) {
    py::bytes data;
    {
        py::file f = py::open_rb(path);
        data = f.read();
    }
    std::optional<int64_t> o;
    for (const auto& e : xex::optional_headers(data)) {
        if (std::get<1>(e) == IMPORT_HEADER) o = std::get<2>(e);        // (a dict: the last of a key wins)
    }
    if (!o) return py::list<Import>();
    py::ref<powerpc_image::Image> img = powerpc_image::read(xex::basefile(path));
    auto [total, strsize, nlibs] = py::unpack_from<int64_t, int64_t, int64_t>(">III", data, *o);
    py::list<py::str> libs;
    int64_t p = *o + 12;
    while (py::len(libs) < nlibs) {
        int64_t end = data.index(py::bytes(std::string(1, '\0')), p);
        libs.append(data.slice(p, end).decode("latin-1"));
        p += (end - p + 1 + 3) & ~static_cast<int64_t>(3);
    }
    p = *o + 12 + strsize;
    std::map<std::pair<std::u32string, int64_t>, Entry> found;      // sorted() of its items: (library, ordinal)
    while (p < *o + total) {
        int64_t size = py::unpack_from1<int64_t>(">I", data, p);
        if (size == 0) break;
        auto [index, count] = py::unpack_from<int64_t, int64_t>(">HH", data, p + 0x24);
        py::str lib = libs[index & 0xFF];
        need(data, p + 0x28, 4 * count);
        for (int64_t i = 0; i < count; i++) {
            int64_t rec = py::unpack_from1<int64_t>(">I", data, p + 0x28 + 4 * i);
            std::optional<int64_t> word = img->word(rec);
            if (!word) continue;
            Entry& entry = found[{lib.u32(), *word & 0xFFFF}];
            if (*word >> 24) entry.thunk = rec;
            else entry.slot = rec;
        }
        p += size;
    }
    auto names = _names();
    py::list<Import> out;
    for (const auto& kv : found) {
        py::str lib(kv.first.first);
        int64_t ordinal = kv.first.second;
        const Entry& e = kv.second;
        py::str name;
        if (names.contains(lib) && names.at(lib).contains(ordinal)) name = names.at(lib).at(ordinal);
        py::str kind = e.thunk ? py::str("function") : py::str("variable");
        if (name.empty()) name = py::split(lib, py::str("."))[0] + py::str("_") + py::to_str(ordinal);
        out.append(Import(lib, ordinal, name, kind, e.slot, e.thunk));
    }
    return out;
}

static py::str hex8(int64_t v) {
    char b[32];
    std::snprintf(b, sizeof b, "0x%08llX", static_cast<unsigned long long>(v));
    return py::str(b);
}

int64_t main(const py::list<py::str>& argv) {
    if (argv.empty()) {
        py::print(py::str("xex_imports.py -- an Xbox 360 module's imports, one per line: library, ordinal, name, kind, slot, thunk.") +
                  py::str("\n\n") +
                  py::str("    python -m codebreaker xex-imports <module.xex>\n"
                          "    from codebreaker.formats import xex_imports\n"
                          "    xex_imports.imports(path)       [(library, ordinal, name, kind, slot address, thunk address or None)]"));
        return 2;
    }
    py::print(py::str("library\tordinal\tname\tkind\tslot\tthunk"));
    for (const Import& im : imports(argv[0])) {
        const auto& slot = std::get<4>(im);
        const auto& thunk = std::get<5>(im);
        py::print(std::get<0>(im) + py::str("\t") + py::to_str(std::get<1>(im)) + py::str("\t") + std::get<2>(im) +
                  py::str("\t") + std::get<3>(im) + py::str("\t") + (slot && *slot ? hex8(*slot) : py::str("")) +
                  py::str("\t") + (thunk && *thunk ? hex8(*thunk) : py::str("")));
    }
    return 0;
}

}  // namespace codebreaker::formats::xex_imports
