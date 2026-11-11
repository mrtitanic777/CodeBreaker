// codebreaker/source/classes_writer.cpp -- typed C++ port of codebreaker.source.classes_writer (see classes_writer.hpp).

#include "codebreaker/source/classes_writer.hpp"

#include <algorithm>
#include <map>
#include <set>

#include "codebreaker/analyze/classes.hpp"
#include "codebreaker/analyze/image.hpp"
#include "codebreaker/formats/detect.hpp"
#include "codebreaker/formats/pe.hpp"
#include "codebreaker/formats/powerpc_image.hpp"
#include "codebreaker/symbols/ordinals.hpp"

namespace codebreaker::source::classes_writer {

namespace {

// big-endian word `i` of `data`
int64_t be_word(const py::bytes& data, int64_t i) {
    const std::string& d = data.raw();
    size_t p = static_cast<size_t>(i) * 4;
    return (static_cast<int64_t>(static_cast<unsigned char>(d[p])) << 24) | (static_cast<int64_t>(static_cast<unsigned char>(d[p + 1])) << 16) |
           (static_cast<int64_t>(static_cast<unsigned char>(d[p + 2])) << 8) | static_cast<int64_t>(static_cast<unsigned char>(d[p + 3]));
}

struct Imports {
    std::shared_ptr<formats::pe::PEFile> pe;
    py::dict<uint64_t, std::tuple<py::str, py::str>> slots;
};
std::map<std::string, Imports> _IMPORTS;

}  // namespace

py::list<core16::ClassRecord> _powerpc_tables(const py::str& path) {
    // [{vtable, methods}] -- runs of two or more words in a PowerPC image's data that are each the address of one of
    // its functions.
    auto img = formats::powerpc_image::read(path);
    std::set<int64_t> starts(img->functions.begin(), img->functions.end());
    std::tuple<int64_t, int64_t> code = img->code;
    // The addresses kept in an extracted image's data are for the base it was linked at, which need not be where it is
    // read to here. The table of function entries (.pdata) says by how much they differ: its first word is the first
    // function's address, as linked.
    int64_t delta = 0;
    for (const auto& s : img->sections) {
        const py::str& name = std::get<0>(s);
        int64_t va = std::get<1>(s), n = std::get<2>(s);
        if (name == py::str(".pdata") && n >= 8 && !starts.empty()) {
            py::bytes raw = img->bytes_at(va, n);
            std::set<int64_t> linked_set;
            for (int64_t i = 0; i < raw.size() / 8; i++) linked_set.insert(be_word(raw, 2 * i));
            linked_set.erase(0);
            std::vector<int64_t> linked(linked_set.begin(), linked_set.end());
            std::pair<int64_t, int64_t> best{0, 0};
            int64_t ns = 0;
            for (int64_t st : starts) {                      // which of the first few is the table's first
                if (ns++ >= 8) break;
                for (size_t k = 0; k < std::min<size_t>(8, linked.size()); k++) {
                    int64_t d = (st - linked[k]) & 0xFFFFFFFFLL;
                    int64_t hits = 0;
                    for (int64_t x : linked)
                        if (starts.count((x + d) & 0xFFFFFFFFLL)) hits += 1;
                    best = std::max(best, std::make_pair(hits, d));
                }
            }
            if (best.first * 2 >= static_cast<int64_t>(linked.size())) delta = best.second;
        }
    }
    std::set<int64_t> shifted;
    for (int64_t s : starts) shifted.insert((s - delta) & 0xFFFFFFFFLL);
    starts = shifted;
    py::list<core16::ClassRecord> out;
    for (const auto& s : img->sections) {
        const py::str& name = std::get<0>(s);
        int64_t va = std::get<1>(s), n = std::get<2>(s);
        if (n <= 0 || (std::get<0>(code) <= va && va < std::get<1>(code)) || name == py::str(".pdata") || name == py::str(".reloc") ||
            name == py::str(".edata") || name == py::str(".idata"))
            continue;
        py::bytes data = img->bytes_at(va, n);
        std::vector<int64_t> words;
        if (data.size() >= 8)
            for (int64_t i = 0; i < data.size() / 4; i++) words.push_back(be_word(data, i));
        words.push_back(0);
        std::vector<int64_t> run;
        for (size_t i = 0; i < words.size(); i++) {
            int64_t w = words[i];
            if (starts.count(w)) {
                run.push_back(w);
                continue;
            }
            if (run.size() >= 2) {
                core16::ClassRecord c;
                c.vtable = va + 4 * (static_cast<int64_t>(i) - static_cast<int64_t>(run.size()));
                for (int64_t m : run) c.methods.append((m + delta) & 0xFFFFFFFFLL);
                c.name = py::percent(py::str("class_%x"), c.vtable);
                c.evidence.append(py::percent(py::str("a table of %d of the image's own functions, in %s: a virtual table, or another table of "
                                                      "functions (callbacks) -- the image does not say which"),
                                              static_cast<int64_t>(run.size()), name));
                out.append(c);
            }
            run.clear();
        }
    }
    return out;
}

std::optional<py::str> import_behind(const py::str& path, int64_t address) {
    // The import a 32-bit module's function is nothing but a jump to (`jmp [slot]`), as LIBRARY.name or LIBRARY.number
    // -- or None. A class whose functions are another library's (MFC's) has such jumps in its table.
    try {
        std::string k = path.utf8();
        auto it = _IMPORTS.find(k);
        if (it == _IMPORTS.end()) {
            auto pe = std::make_shared<formats::pe::PEFile>(formats::pe::open_pe(path));
            it = _IMPORTS.emplace(k, Imports{pe, pe->imports()}).first;
        }
        const formats::pe::PEFile& pe = *it->second.pe;
        std::optional<int64_t> off = pe.file_offset(address - static_cast<int64_t>(pe.image_base));
        if (!off || pe.data.slice(*off, *off + 2) != py::bytes(std::string("\xff\x25", 2))) return std::nullopt;
        auto found = it->second.slots.get(static_cast<uint64_t>(py::unpack_from1<int64_t>("<I", pe.data, *off + 2)));
        if (!found || std::get<0>(*found).empty()) return std::nullopt;
        py::str stem = std::get<0>(py::os::path::splitext(std::get<0>(*found))).upper();
        const py::str& name = std::get<1>(*found);
        if (!name.empty() && name.startswith(py::str("#")) && name.slice(1, name.size()).isdigit()) {
            std::optional<py::str> id = symbols::ordinals::identifier(stem, py::int_parse(name.slice(1, name.size()), 10));
            return py::percent(py::str("%s.%s"), stem, (id && !id->empty()) ? *id : name.slice(1, name.size()));
        }
        return py::percent(py::str("%s.%s"), stem, name);
    } catch (const std::exception&) {
        return std::nullopt;
    }
}

py::list<core16::ClassRecord> found(const py::str& path, const py::str& kind) {
    // The classes of a module, as analyze/classes.py's hierarchy gives them (or the PowerPC tables).
    if (kind == py::str("powerpc")) return _powerpc_tables(path);
    if (!formats::detect::flat(kind)) return {};
    auto img = analyze::image::load(path);
    py::list<core16::ClassRecord> out;
    for (const auto& c : *analyze::classes::hierarchy(*img)) out.append(static_cast<const core16::ClassRecord&>(c));
    return out;
}

std::optional<py::str> write(const py::str& path, const py::str& kind, const py::str& out_dir, const py::str& stem,
                             const std::function<py::str(int64_t)>& name_at, const std::function<py::str(int64_t)>& status_at) {
    // `name_at(address)` -> the name a function has in the written source; `status_at(address)` -> its verdict's title,
    // or "". -> the path written, or None.
    py::list<core16::ClassRecord> classes;
    try {
        classes = found(path, kind);
    } catch (const std::exception&) {
        return std::nullopt;
    }
    if (classes.empty()) return std::nullopt;
    py::dict<int64_t, py::str> names;
    int64_t named = 0;
    for (const auto& c : classes) {
        names.set(c.vtable, c.name);
        if (!c.name.startswith(py::str("class_"))) named += 1;
    }
    py::list<py::str> lines;
    lines.append(py::percent(py::str("// %s -- the C++ classes this module shows, as class declarations."), py::os::path::basename(path)));
    lines.append(py::str("//"));
    lines.append(py::percent(py::str("// Read beside %s.c. Not compiled: each virtual function is the reconstructed function named"), stem));
    lines.append(py::str("// in its comment, where its real parameters are. A class or function name the binary does"));
    lines.append(py::str("// not state is a placeholder (class_<address of its table>, vf_<slot>)."));
    lines.append(py::str("//"));
    lines.append(py::percent(py::str("// %d classes; %d named by the binary itself."), static_cast<int64_t>(classes.size()), named));
    lines.append(py::str(""));
    for (const auto& c : classes) {
        // (a base known only as a numbered import of another library is written as that)
        py::list<py::str> bases;
        for (const py::str& b : c.bases) {
            if (b.empty()) continue;
            if (b.replace(py::str("_"), py::str("a")).isalnum() && !b.slice(0, 1).isdigit()) {
                bases.append(b);
            } else {
                py::str keep;
                for (char32_t ch : b.u32())
                    if (py::str(ch).isalnum()) keep = keep + py::str(ch);
                bases.append(py::str("imported_class_") + keep);
            }
        }
        py::list<py::str> facts;
        facts.append(py::percent(py::str("%d virtual functions"), static_cast<int64_t>(c.methods.size())));
        facts.append(py::percent(py::str("table at %x"), c.vtable));
        if (c.size && *c.size) {
            facts.append(py::percent(py::str("object of %#x bytes%s"), *c.size, (!c.size_exact || *c.size_exact) ? py::str("") : py::str(" at least")));
        }
        if (c.com) facts.append(py::str("has the shape of a COM interface (True)"));
        if (c.message_map && *c.message_map) facts.append(py::percent(py::str("message map at %x"), *c.message_map));
        lines.append(py::str("// ") + py::join(py::str("; "), facts));
        for (const py::str& e : c.evidence) lines.append(py::percent(py::str("//   %s"), e));
        if (!c.installed_by.empty()) {
            py::list<py::str> who;
            for (int64_t k = 0; k < std::min<int64_t>(8, c.installed_by.size()); k++) who.append(name_at(c.installed_by[k]));
            lines.append(py::percent(py::str("//   constructed or destroyed in: %s"), py::join(py::str(", "), who)));
        }
        py::list<py::str> direct;
        if (!bases.empty()) direct.append(bases[0]);
        py::str inherits;
        if (!direct.empty()) inherits = py::str(" : public ") + py::join(py::str(", public "), direct);
        lines.append(py::percent(py::str("class %s%s {"), c.name, inherits));
        if (bases.size() > 1) lines.append(py::percent(py::str("    // further up: %s"), py::join(py::str(" : "), bases.slice(1, bases.size()))));
        lines.append(py::str("public:"));
        for (int64_t slot = 0; slot < c.methods.size(); slot++) {
            int64_t f = c.methods[slot];
            py::str status = status_at(f);
            lines.append(py::percent(py::str("    virtual int4 vf_%d();%s// %s%s"), slot,
                                     py::str(std::string(static_cast<size_t>(std::max<int64_t>(1, 6 - static_cast<int64_t>(std::to_string(slot).size()))), ' ')),
                                     name_at(f), !status.empty() ? py::percent(py::str("  [%s]"), status) : py::str("")));
        }
        lines.append(py::str("};"));
        lines.append(py::str(""));
    }
    py::str out = py::os::path::join(out_dir, stem + py::str("_classes.hpp"));
    py::file f = py::open(out, py::str("w"), py::str("utf-8"), std::nullopt, py::str("\n"));
    f.write(py::join(py::str("\n"), lines));
    f.close();
    return out;
}

}  // namespace codebreaker::source::classes_writer
