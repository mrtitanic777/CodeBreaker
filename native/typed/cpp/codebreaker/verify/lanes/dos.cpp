// codebreaker/verify/lanes/dos.cpp -- typed C++ port of codebreaker.verify.lanes.dos.

#include "codebreaker/verify/lanes/dos.hpp"

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <memory>
#include <system_error>

#include "codebreaker/verify/verdicts.hpp"

namespace codebreaker::verify::lanes::dos {

namespace V = codebreaker::verify::verdicts;

const py::str KIND("dos");
const py::str SAME("the program built from this source is the original, byte for byte");

namespace {

std::shared_ptr<Packager>& packager_() {
    static std::shared_ptr<Packager> p;
    return p;
}

// P.packager, platforms.of(KIND)'s
const Packager& P() {
    if (!packager_()) throw py::RuntimeError("the dos platform's packager is not set (set_packager)");
    return *packager_();
}

// "%s:%d:%d" % (name, os.stat(exe).st_size, int(os.stat(exe).st_mtime)), or "" for a file that cannot be stat'ed (OSError)
py::str tool_stamp(const py::str& name, const py::str& exe) {
    try {
        py::os::stat_result st = py::os::stat(exe);
        return py::percent(py::str("%s:%d:%d"), name, st.st_size, static_cast<int64_t>(std::trunc(st.st_mtime)));
    } catch (const py::OSError&) {
        return py::str("");
    }
}

// sorted("%05x" % a ...) as Python orders strings
bool str_less(const py::str& a, const py::str& b) { return a.u32() < b.u32(); }

}  // namespace

void set_packager(const Packager& packager) { packager_() = std::make_shared<Packager>(packager); }

std::tuple<py::str, py::str> profile(const std::optional<py::str>& module_path) {
    (void)module_path;
    py::list<py::str> tools;
    const py::dict<py::str, py::str> t = P().tools();
    for (const auto& kv : t.items()) tools.append(tool_stamp(std::get<0>(kv), std::get<1>(kv)));
    py::list<py::str> shown;
    for (const py::str& s : tools) shown.append(py::repr(s));
    // repr(("dos", REV, tools))
    const py::str settings = py::str("(") + py::repr(KIND) + py::str(", ") + py::to_str(REV) + py::str(", [") +
                             py::join(py::str(", "), shown) + py::str("])");
    return {py::hashlib::sha1(settings.encode()).hexdigest().slice(0, 12), settings};
}

Prepared prepare(const py::str& module_path, const py::str& work) {
    Prepared m;
    m.written = P().writer(module_path, py::os::path::join(work, py::str("source")));
    const Written& r = m.written;
    py::str text("");
    if (r.source && !r.source->empty() && py::os::path::isfile(*r.source)) {
        text = py::file::open(*r.source, py::str("r"), py::str("latin1")).read_text();
    }
    const py::dict<int64_t, Function> functions = (r.functions && !r.functions->empty()) ? *r.functions : py::dict<int64_t, Function>();
    m.header = text;
    std::vector<int64_t> addrs;
    for (const auto& kv : functions.items()) addrs.push_back(std::get<0>(kv));
    std::vector<int64_t> by_addr = addrs;
    std::sort(by_addr.begin(), by_addr.end());
    for (int64_t a : by_addr) m.order.append(functions.at(a).name);
    if (!text.empty()) {
        for (int64_t a : addrs) m.key.set(functions.at(a).name, py::percent(py::str("%05x"), a));
    }
    std::vector<py::str> keys;
    for (int64_t a : addrs) keys.push_back(py::percent(py::str("%05x"), a));
    std::sort(keys.begin(), keys.end(), str_less);
    for (const py::str& k : keys) m.all_keys.append(k);
    if (!text.empty()) {
        const py::str base = py::os::path::basename(*r.source);
        for (const py::str& n : m.order) m.texts.set(n, py::percent(py::str("; %s, in %s"), n, base));
    }
    // hashlib.sha1(("%d\0%s" % (REV, text)).encode("latin1", "replace")).hexdigest()
    const py::str nul(std::u32string(1, U'\0'));
    m.sig = py::hashlib::sha1((py::percent(py::str("%d"), REV) + nul + text).encode("latin1", "replace")).hexdigest();
    return m;
}

Built build(const Prepared& m, const py::str& module_path, const py::str& work) {
    (void)module_path;
    (void)work;
    const Written& r = m.written;
    Built out{Context(false, py::str("")), {}, {}};
    if (!(r.linked && *r.linked)) {
        py::str why = (r.notes && !r.notes->empty()) ? (*r.notes)[-1] : py::str("did not build");
        why = why.slice(0, 200);
        std::get<1>(out.context) = why;
        for (const py::str& n : m.order) out.missing.set(n, why);
        return out;
    }
    if (!r.same) throw py::KeyError("'same'");
    py::str why("");
    if (!*r.same) {
        why = (r.notes && !r.notes->empty()) ? (*r.notes)[-1] : py::str("the built program differs from the original");
        why = why.slice(0, 200);
    }
    out.context = Context(*r.same, why);
    for (const py::str& n : m.order) out.items.set(n, Item(m.key.at(n)));
    return out;
}

Verdict judge(const Context& context, const Item& item) {
    (void)item;
    const bool same = std::get<0>(context);
    const py::str& why = std::get<1>(context);
    if (same) return {V::EQUIVALENT, py::str(""), py::list<py::str>(), SAME};
    return {V::DIVERGENT, py::str("image-differs"), py::list<py::str>(), why};
}

}  // namespace codebreaker::verify::lanes::dos
