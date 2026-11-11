// codebreaker/symbols/cpp_classes.cpp -- C++17 port of codebreaker.symbols.cpp_classes (see cpp_classes.hpp).

#include "codebreaker/symbols/cpp_classes.hpp"

#include <algorithm>
#include <string>
#include <vector>

#include "codebreaker/formats/xex.hpp"
#include "codebreaker/symbols/demangle.hpp"
#include "py_json.hpp"
#include "py_os.hpp"

namespace codebreaker::symbols::cpp_classes {

namespace powerpc_image = codebreaker::formats::powerpc_image;
namespace xex = codebreaker::formats::xex;
namespace demangle = codebreaker::symbols::demangle;

namespace {

using Range = std::tuple<int64_t, int64_t>;   // (low, high)

// a type descriptor found in the data: {"mangled", "vftable", "name"}
struct TypeDescriptor {
    py::str mangled;
    int64_t vftable = 0;
    py::str name;
};

// a class hierarchy descriptor: {"attributes", "bases"}
struct Hierarchy {
    int64_t attributes = 0;
    py::list<BaseClass> bases;
};

// a complete object locator: {"offset", "td", "chd", "hierarchy"}
struct Locator {
    int64_t offset = 0, td = 0, chd = 0;
    Hierarchy hierarchy;
};

bool inside(int64_t a, const Range& r) { return std::get<0>(r) <= a && a < std::get<1>(r); }

// the set {0} | {b.mdisp for b in cls.bases if b.pdisp == -1}
py::set<int64_t> nonvirtual(const Class& cls) {
    py::set<int64_t> out = {0};
    for (const BaseClass& b : cls.bases) {
        if (b.pdisp == -1) out.add(b.mdisp);
    }
    return out;
}

}  // namespace

Classes read(const py::str& path) {
    py::ref<powerpc_image::Image> img = powerpc_image::read(xex::is_xex(path) ? xex::basefile(path) : path);
    return from_image(*img);
}

Classes from_image(const powerpc_image::Image& img) {
    py::dict<py::str, Range> secs;
    for (const auto& s : img.sections) {
        secs.set(std::get<0>(s).strip(py::str(std::string(1, '\0'))), Range(std::get<1>(s), std::get<1>(s) + std::get<2>(s)));
    }
    Range rdata = secs.get(py::str(".rdata"), Range(0, 0));
    Range text = secs.get(py::str(".text"), img.code);
    std::vector<Range> data_ranges;
    for (const auto& [n, r] : secs.items()) {
        if (n == py::str(".data") || n == py::str(".rdata")) data_ranges.push_back(r);
    }
    py::set<int64_t> starts;
    for (int64_t f : img.functions) starts.add(f);
    for (int64_t f : img.extents) starts.add(f);

    auto u32 = [&](int64_t a) -> int64_t {
        std::optional<int64_t> w = img.word(a);
        return w ? *w : 0;
    };
    auto s32 = [&](int64_t a) -> int64_t {
        int64_t v = u32(a);
        return (v & 0x80000000) ? v - (int64_t(1) << 32) : v;
    };

    // 1. type descriptors: the name 8 bytes in, after type_info's vftable and a zero word; the
    //    vftable every real descriptor shares is type_info's
    py::dict<int64_t, TypeDescriptor> tds;
    for (const Range& r : data_ranges) {
        int64_t lo = std::get<0>(r), hi = std::get<1>(r);
        py::bytes block = img.bytes_at(lo, hi - lo);
        int64_t p = 0;
        const py::bytes marker(".?A");
        const py::bytes nul(std::string(1, '\0'));
        for (;;) {
            int64_t i = block.find(marker, p);
            if (i < 0) break;
            int64_t end = block.find(nul, i);
            if (end < 0) break;
            p = end;
            int64_t td = lo + i - 8;
            if (py::mod(td, 4) || u32(td + 4) != 0) continue;
            TypeDescriptor t;
            t.mangled = block.slice(i, end).decode("ascii", "replace");
            t.vftable = u32(td);
            tds.set(td, t);
        }
    }
    if (tds.empty()) {
        return Classes();
    }
    py::dict<int64_t, int64_t> counts;
    for (const auto& [a, t] : tds.items()) {
        (void)a;
        counts.set(t.vftable, counts.get(t.vftable, 0) + 1);
    }
    int64_t type_info = 0;
    int64_t best = -1;
    for (const auto& [vf, n] : counts.items()) {   // max(counts, key=counts.get): the first of the largest
        if (n > best) {
            best = n;
            type_info = vf;
        }
    }
    {
        py::dict<int64_t, TypeDescriptor> kept;
        for (const auto& [a, t] : tds.items()) {
            if (t.vftable == type_info) {
                TypeDescriptor n = t;
                std::optional<py::str> dn = demangle::type_name(t.mangled);
                n.name = (dn && !dn->empty()) ? *dn : t.mangled;
                kept.set(a, n);
            }
        }
        tds = kept;
    }

    // 2. hierarchy descriptors and object locators (.rdata)
    auto hierarchy = [&](int64_t a) -> std::optional<Hierarchy> {
        if (!inside(a, rdata) || py::mod(a, 4) || u32(a) != 0) return std::nullopt;
        int64_t attrs = u32(a + 4), n = u32(a + 8), array = u32(a + 12);
        if (!(1 <= n && n <= 256) || !inside(array, rdata) || (attrs & ~int64_t(3))) return std::nullopt;
        Hierarchy h;
        h.attributes = attrs;
        for (int64_t k = 0; k < n; ++k) {
            int64_t bcd = u32(array + 4 * k);
            if (!inside(bcd, rdata) || !tds.contains(u32(bcd))) return std::nullopt;
            BaseClass b;
            b.name = tds.at(u32(bcd)).name;
            b.contained = u32(bcd + 4);
            b.mdisp = s32(bcd + 8);
            b.pdisp = s32(bcd + 12);
            b.vdisp = s32(bcd + 16);
            b.attributes = u32(bcd + 20);
            h.bases.append(b);
        }
        return h;
    };
    py::dict<int64_t, Locator> locators;
    for (int64_t a = std::get<0>(rdata); a < std::get<1>(rdata) - 20; a += 4) {
        if (u32(a) != 0 || !tds.contains(u32(a + 12))) continue;
        std::optional<Hierarchy> h = hierarchy(u32(a + 16));
        if (!h || !(h->bases[0].name == tds.at(u32(a + 12)).name)) continue;
        Locator loc;
        loc.offset = u32(a + 4);
        loc.td = u32(a + 12);
        loc.chd = u32(a + 16);
        loc.hierarchy = *h;
        locators.set(a, loc);
    }

    // 3. tables: a word pointing at a locator, then function pointers
    py::set<int64_t> pointers;
    for (int64_t a = std::get<0>(rdata); a < std::get<1>(rdata) - 4; a += 4) {
        if (locators.contains(u32(a))) pointers.add(a);
    }
    py::dict<py::str, Class> classes;
    for (const Locator& loc : locators.values()) {
        py::str name = tds.at(loc.td).name;
        if (!classes.contains(name)) {
            Class c;
            c.name = name;
            c.mangled = tds.at(loc.td).mangled;
            c.bases = loc.hierarchy.bases.slice_from(1);
            classes.set(name, c);
        }
    }

    auto scan = [&](int64_t q, const py::list<int64_t>& slots) -> int64_t {
        while (q < std::get<1>(rdata) && !pointers.contains(q) && starts.contains(u32(q))) {
            slots.append(u32(q));
            q += 4;
        }
        return q;
    };
    for (int64_t a : py::sorted(pointers)) {
        const Locator& loc = locators.at(u32(a));
        Class c = classes.at(tds.at(loc.td).name);
        py::list<int64_t> slots;
        scan(a + 4, slots);
        std::optional<py::str> owner;
        if (loc.offset) {
            for (const BaseClass& b : c.bases) {
                if (b.pdisp == -1 && b.mdisp == loc.offset) {
                    owner = b.name;
                    break;
                }
            }
        }
        VTable v;
        v.address = a + 4;
        v.offset = loc.offset;
        v.for_ = owner;
        v.slots = slots;
        c.vtables.append(v);
    }
    for (const Class& c : classes.values()) {
        c.vtables.sort([](const VTable& v) { return std::make_tuple(v.offset, v.address); });
    }

    // 4. the layout rule: a derived class's table for a base sub-object is at least that base's table
    auto required = [&](const Class& c, const VTable& v) -> int64_t {
        int64_t need = 1;   // a class has a table only for a virtual function
        for (const BaseClass& b : c.bases) {
            if (b.pdisp != -1 || !classes.contains(b.name)) continue;
            Class base = classes.at(b.name);
            for (const VTable& bv : base.vtables) {
                if (nonvirtual(base).contains(bv.offset) && b.mdisp + bv.offset == v.offset) {
                    need = std::max(need, py::len(bv.slots));
                }
            }
        }
        return need;
    };
    py::list<std::tuple<py::str, int64_t, int64_t, int64_t>> found;
    bool changed = true;
    while (changed) {
        changed = false;
        for (const Class& c : classes.values()) {
            for (const VTable& v : c.vtables) {
                int64_t need = required(c, v);
                int64_t q = v.address + 4 * py::len(v.slots);
                while (py::len(v.slots) < need) {
                    int64_t t = u32(q);
                    if (pointers.contains(q) || !inside(t, text)) break;   // the layout and the image disagree: stop, guess nothing
                    if (!starts.contains(t)) {
                        found.append(std::make_tuple(c.name, v.address, py::len(v.slots), t));
                        starts.add(t);
                    }
                    v.slots.append(t);
                    q += 4;
                    changed = true;
                }
                scan(q, v.slots);
            }
        }
    }

    // 5. names: the class that introduces each virtual function
    py::dict<int64_t, py::list<std::tuple<py::str, int64_t, int64_t>>> occurrences;
    for (const Class& c : classes.values()) {
        for (const VTable& v : c.vtables) {
            int64_t s = 0;
            for (int64_t f : v.slots) {
                if (!occurrences.contains(f)) occurrences.set(f, py::list<std::tuple<py::str, int64_t, int64_t>>());
                occurrences.at(f).append(std::make_tuple(c.name, v.offset, s));
                ++s;
            }
        }
    }
    py::dict<py::str, py::set<py::str>> bases_of;
    for (const auto& [n, c] : classes.items()) {
        py::set<py::str> names;
        for (const BaseClass& b : c.bases) names.add(b.name);
        names.add(n);
        bases_of.set(n, names);
    }
    py::dict<int64_t, std::tuple<py::str, py::str>> functions;
    for (const auto& [f, occ] : occurrences.items()) {
        py::set<py::str> roots;
        for (const auto& o : occ) {
            const py::str& cn = std::get<0>(o);
            bool every = true;
            for (const auto& d : occ) {
                if (!bases_of.at(std::get<0>(d)).contains(cn)) {
                    every = false;
                    break;
                }
            }
            if (every) roots.add(cn);
        }
        py::set<std::tuple<int64_t, int64_t>> root_occ;
        for (const auto& o : occ) {
            if (roots.contains(std::get<0>(o))) root_occ.add(std::make_tuple(std::get<1>(o), std::get<2>(o)));
        }
        if (py::len(roots) == 1 && py::len(root_occ) == 1) {
            auto [off, s] = *root_occ.begin();
            py::str name = *roots.begin();
            functions.set(f, std::make_tuple(name, off == 0 ? py::percent("vf%d", s) : py::percent("vf%d_off%X", s, off)));
        }
    }
    Classes out;
    out.classes = classes;
    out.functions = functions;
    out.found = found;
    return out;
}

namespace {

// json.dump(..., indent=1) of the classes file; `pad` is the indentation of the item's own line
void jstr(std::string& o, const py::str& s) { py::json::put(o, s); }
void jint(std::string& o, int64_t v) { o += std::to_string(v); }
std::string ind(int n) { return std::string(static_cast<size_t>(n), ' '); }

// a list of ints / items written one per line
template <class T, class F> void jlist(std::string& o, int depth, const py::list<T>& xs, F one) {
    if (xs.empty()) {
        o += "[]";
        return;
    }
    o += "[\n";
    bool first = true;
    for (const T& x : xs) {
        if (!first) o += ",\n";
        first = false;
        o += ind(depth + 1);
        one(x, depth + 1);
    }
    o += "\n" + ind(depth) + "]";
}

std::string classes_json(const Classes& c) {
    std::string o = "{\n \"classes\": ";
    std::vector<Class> sorted_classes = c.classes.values();
    std::stable_sort(sorted_classes.begin(), sorted_classes.end(), [](const Class& a, const Class& b) { return a.name < b.name; });
    py::list<Class> cl(sorted_classes);
    jlist(o, 1, cl, [&](const Class& x, int d) {
        o += "{\n" + ind(d + 1) + "\"name\": ";
        jstr(o, x.name);
        o += ",\n" + ind(d + 1) + "\"mangled\": ";
        jstr(o, x.mangled);
        o += ",\n" + ind(d + 1) + "\"bases\": ";
        jlist(o, d + 1, x.bases, [&](const BaseClass& b, int e) {
            o += "{\n" + ind(e + 1) + "\"name\": ";
            jstr(o, b.name);
            o += ",\n" + ind(e + 1) + "\"contained\": ";
            jint(o, b.contained);
            o += ",\n" + ind(e + 1) + "\"mdisp\": ";
            jint(o, b.mdisp);
            o += ",\n" + ind(e + 1) + "\"pdisp\": ";
            jint(o, b.pdisp);
            o += ",\n" + ind(e + 1) + "\"vdisp\": ";
            jint(o, b.vdisp);
            o += ",\n" + ind(e + 1) + "\"attributes\": ";
            jint(o, b.attributes);
            o += "\n" + ind(e) + "}";
        });
        o += ",\n" + ind(d + 1) + "\"vtables\": ";
        jlist(o, d + 1, x.vtables, [&](const VTable& v, int e) {
            o += "{\n" + ind(e + 1) + "\"address\": ";
            jint(o, v.address);
            o += ",\n" + ind(e + 1) + "\"offset\": ";
            jint(o, v.offset);
            o += ",\n" + ind(e + 1) + "\"for\": ";
            if (v.for_) jstr(o, *v.for_);
            else o += "null";
            o += ",\n" + ind(e + 1) + "\"slots\": ";
            jlist(o, e + 1, v.slots, [&](int64_t s, int) { jint(o, s); });
            o += "\n" + ind(e) + "}";
        });
        o += "\n" + ind(d) + "}";
    });
    o += ",\n \"functions\": ";
    std::vector<std::tuple<int64_t, std::tuple<py::str, py::str>>> fs = c.functions.items();
    std::sort(fs.begin(), fs.end(), [](const auto& a, const auto& b) { return std::get<0>(a) < std::get<0>(b); });
    if (fs.empty()) {
        o += "{}";
    } else {
        o += "{\n";
        bool first = true;
        for (const auto& [a, v] : fs) {
            if (!first) o += ",\n";
            first = false;
            o += "  ";
            jstr(o, py::percent("%08X", a));
            o += ": [\n   ";
            jstr(o, std::get<0>(v));
            o += ",\n   ";
            jstr(o, std::get<1>(v));
            o += "\n  ]";
        }
        o += "\n }";
    }
    o += ",\n \"found\": ";
    jlist(o, 1, c.found, [&](const std::tuple<py::str, int64_t, int64_t, int64_t>& x, int d) {
        o += "[\n" + ind(d + 1);
        jstr(o, std::get<0>(x));
        o += ",\n" + ind(d + 1);
        jint(o, std::get<1>(x));
        o += ",\n" + ind(d + 1);
        jint(o, std::get<2>(x));
        o += ",\n" + ind(d + 1);
        jint(o, std::get<3>(x));
        o += "\n" + ind(d) + "]";
    });
    o += "\n}";
    return o;
}

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

int64_t main(const py::list<py::str>& argv) {
    if (argv.empty() || argv[0].startswith(py::str("-"))) {
        py::print(py::str(kUsage));
        return 2;
    }
    Classes c = read(argv[0]);
    int64_t tables = 0;
    for (const Class& x : c.classes.values()) tables += py::len(x.vtables);
    py::print(py::percent("%s: %d classes, %d virtual function tables, %d virtual functions named by class, %d functions found by layout",
                          py::os::path::basename(argv[0]), py::len(c.classes), tables, py::len(c.functions), py::len(c.found)));
    std::vector<Class> deepest = c.classes.values();
    std::stable_sort(deepest.begin(), deepest.end(), [](const Class& a, const Class& b) { return -py::len(a.bases) < -py::len(b.bases); });
    if (deepest.size() > 8) deepest.resize(8);
    for (const Class& x : deepest) {
        py::list<py::str> names;
        for (const BaseClass& b : x.bases) names.append(b.name);
        py::str joined = py::join(py::str(", "), names);
        py::print(py::percent("  %-40s bases: %s", x.name.slice(0, 40), joined.slice(0, 100)));
    }
    if (argv.contains(py::str("-o"))) {
        py::str out = argv[argv.index(py::str("-o")) + 1];
        {
            py::file f = py::open(out, py::str("w"), py::str("utf-8"));
            f.write(py::str(classes_json(c)));
        }
        py::print(py::percent("  -> %s", out));
    }
    return 0;
}

}  // namespace codebreaker::symbols::cpp_classes
