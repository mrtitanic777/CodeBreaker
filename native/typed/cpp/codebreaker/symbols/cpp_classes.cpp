// codebreaker/symbols/cpp_classes.cpp -- the shim's translation unit (see cpp_classes.hpp). The new module
// (cb/symbols/cpp_classes) finds the classes; this file reads the image with the old format readers, hands it over, and
// converts what comes back.

#include "codebreaker/symbols/cpp_classes.hpp"

#include <algorithm>
#include <string>
#include <vector>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/symbols/cpp_classes.hpp"
#include "codebreaker/formats/xex.hpp"
#include "py_os.hpp"

namespace codebreaker::symbols::cpp_classes {

namespace {

namespace powerpc_image = codebreaker::formats::powerpc_image;
namespace xex = codebreaker::formats::xex;
namespace cc = cb::symbols::cpp_classes;
using cb::compat::from_py;
using cb::compat::to_py;

// What the search reads of an image: the sections' places, the function starts, and the memory of .data and .rdata.
cc::Image image_of(const powerpc_image::Image& img) {
    // the sections by name: a name met twice keeps its first place and its last range
    std::vector<std::pair<py::str, std::pair<int64_t, int64_t>>> named;
    cc::Image out;
    for (const auto& s : img.sections) {
        const py::str name = std::get<0>(s).strip(py::str(std::string(1, '\0')));
        const int64_t address = std::get<1>(s), size = std::get<2>(s);
        out.sections.push_back({from_py(name), static_cast<std::uint32_t>(address), static_cast<std::uint32_t>(size)});
        const auto it = std::find_if(named.begin(), named.end(), [&](const auto& n) { return n.first == name; });
        if (it == named.end()) named.push_back({name, {address, address + size}});
        else it->second = {address, address + size};
    }
    out.code_begin = static_cast<std::uint32_t>(std::get<0>(img.code));
    out.code_end = static_cast<std::uint32_t>(std::get<1>(img.code));
    for (int64_t f : img.functions) out.function_starts.insert(static_cast<std::uint32_t>(f));
    for (int64_t f : img.extents) out.function_starts.insert(static_cast<std::uint32_t>(f));
    for (const auto& [name, range] : named) {
        if (name != py::str(".data") && name != py::str(".rdata")) continue;
        out.memory.push_back({static_cast<std::uint32_t>(range.first), from_py(img.bytes_at(range.first, range.second - range.first))});
    }
    return out;
}

Classes to_old(const cc::Classes& found) {
    Classes out;
    for (const auto& [name, c] : found.classes) {
        Class k;
        k.name = to_py(c.name);
        k.mangled = to_py(c.mangled);
        for (const cc::BaseClass& b : c.bases) k.bases.append({to_py(b.name), b.contained, b.mdisp, b.pdisp, b.vdisp, b.attributes});
        for (const cc::VTable& v : c.vtables) {
            VTable t;
            t.address = v.address;
            t.offset = v.offset;
            if (v.for_base) t.for_ = to_py(*v.for_base);
            for (std::uint32_t s : v.slots) t.slots.append(s);
            k.vtables.append(t);
        }
        out.classes.set(k.name, k);
    }
    for (const auto& [address, f] : found.functions) out.functions.set(address, std::make_tuple(to_py(f.class_name), to_py(f.label)));
    for (const cc::FoundFunction& f : found.found)
        out.found.append(std::make_tuple(to_py(f.class_name), static_cast<int64_t>(f.vtable), static_cast<int64_t>(f.slot), static_cast<int64_t>(f.address)));
    return out;
}

py::ref<powerpc_image::Image> read_image(const py::str& path) { return powerpc_image::read(xex::is_xex(path) ? xex::basefile(path) : path); }

// the first two paragraphs of the module's docstring: the usage the script prints
const char* kUsage =
    "cpp_classes.py -- the C++ classes of a PowerPC (Xbox 360) program, from Microsoft's run-time type\n"
    "information: each class's bases, its virtual function tables, and the class that introduces each\n"
    "virtual function.\n"
    "\n"
    "    python -m codebreaker classes <program.xex or image> [-o classes.json]\n"
    "    from codebreaker.symbols import cpp_classes\n"
    "    c = cpp_classes.read(image_path)\n"
    "    c.classes            {name: {\"bases\": [...], \"vtables\": [{\"address\", \"offset\", \"for\", \"slots\"}], ...}}\n"
    "    c.functions          {address: (class, \"vf3\" / \"vf3_off8\")}: virtual functions, by the class that introduces them\n"
    "    c.found              [(class, vtable, slot, address)]: functions no table of starts knew, which the layout requires";

}  // namespace

Classes read(const py::str& path) {
    py::ref<powerpc_image::Image> img = read_image(path);
    return from_image(*img);
}

Classes from_image(const powerpc_image::Image& img) {
    return cb::compat::guard([&] { return to_old(cc::find_classes(image_of(img))); });
}

int64_t main(const py::list<py::str>& argv) {
    if (argv.empty() || argv[0].startswith(py::str("-"))) {
        py::print(py::str(kUsage));
        return 2;
    }
    py::ref<powerpc_image::Image> img = read_image(argv[0]);
    const cc::Classes c = cb::compat::guard([&] { return cc::find_classes(image_of(*img)); });
    std::string summary = cc::summary(c, from_py(py::os::path::basename(argv[0])));
    if (!summary.empty() && summary.back() == '\n') summary.pop_back();
    py::print(to_py(summary));
    if (argv.contains(py::str("-o"))) {
        py::str out = argv[argv.index(py::str("-o")) + 1];
        cb::compat::guard([&] { cb::fs::write_text(cb::fs::from_text(from_py(out)), cc::to_json(c)); });
        py::print(py::percent("  -> %s", out));
    }
    return 0;
}

}  // namespace codebreaker::symbols::cpp_classes
