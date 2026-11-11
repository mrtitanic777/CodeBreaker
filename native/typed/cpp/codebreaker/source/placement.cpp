// codebreaker/source/placement.cpp -- typed C++ of codebreaker.source.placement (see placement.hpp).
#include "codebreaker/source/placement.hpp"

#include <algorithm>
#include <utility>
#include <vector>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/formats/dos.hpp"
#include "codebreaker/knowledge/sources.hpp"
#include "py_settings.hpp"

namespace codebreaker::source::placement {

namespace sources = codebreaker::knowledge::sources;

const py::set<py::str> _SKIP = py::set<py::str>{py::str("design"), py::str("build"), py::str(".git"), py::str("tools"), py::str("inc")};

namespace {

std::optional<sources::Rows>& tables_() {
    static std::optional<sources::Rows> t;   // _tables
    return t;
}

py::dict<py::str, py::dict<py::str, py::str>>& existing_cache_() {
    static py::dict<py::str, py::dict<py::str, py::str>> c;   // _existing
    return c;
}

py::str text_(const sources::Json& j) {
    if (j.kind != sources::Json::Str) throw py::TypeError("a component that is not text");
    return j.s;
}

py::dict<py::str, py::str> strings_(const sources::Json& j) {
    if (j.kind != sources::Json::Dict) throw py::TypeError("an entry of the table that is not a dict");
    py::dict<py::str, py::str> out;
    for (const auto& kv : j.dict.items()) out.set(std::get<0>(kv), text_(std::get<1>(kv)));
    return out;
}

py::list<std::tuple<py::str, py::str>> pairs_(const sources::Json& j) {
    if (j.kind != sources::Json::List) throw py::TypeError("prefix is not a list");
    py::list<std::tuple<py::str, py::str>> out;
    for (const sources::Json& pair : j.list) {
        if (pair.kind != sources::Json::List) throw py::TypeError("a prefix that is not a pair");
        if (pair.list.size() != 2) throw py::ValueError("a prefix that is not a pair");
        out.append(std::make_tuple(text_(pair.list[0]), text_(pair.list[1])));
    }
    return out;
}

// The entry of a table, or Python's KeyError for the key it lacks.
template <class T> const T& need_(const std::optional<T>& v, const char* key) {
    if (!v) throw py::KeyError(key);
    return *v;
}

}  // namespace

py::str tree_root() { return py::os::path::dirname(base::corpus::design_dir()); }

Table _table() {
    if (!tables_()) {
        py::str path = py::os::path::join(py::settings::checkout_root(), py::str("codebreaker"), py::str("source"), py::str("placement.json"));
        sources::Json j = sources::_json(path);
        if (j.kind != sources::Json::Dict) throw py::AttributeError("the tables are not a dict");
        tables_() = j.dict;
    }
    Table out;
    auto entry = tables_()->get(base::corpus::current());
    if (entry && entry->kind == sources::Json::Dict && !entry->dict.empty()) {   // (an empty table is the default)
        const auto& d = entry->dict;
        if (d.contains(py::str("known"))) out.known = strings_(d.at(py::str("known")));
        if (d.contains(py::str("prefix"))) out.prefix = pairs_(d.at(py::str("prefix")));
        if (d.contains(py::str("suffix"))) out.suffix = strings_(d.at(py::str("suffix")));
        if (d.contains(py::str("installed"))) out.installed = strings_(d.at(py::str("installed")));
        if (d.contains(py::str("kind"))) out.kind = strings_(d.at(py::str("kind")));
        return out;
    }
    out.known = py::dict<py::str, py::str>();
    out.prefix = py::list<std::tuple<py::str, py::str>>();
    out.suffix = py::dict<py::str, py::str>();
    out.installed = py::dict<py::str, py::str>();
    out.kind = py::dict<py::str, py::str>();
    return out;
}

py::dict<py::str, py::str> existing(const std::optional<py::str>& root_in) {
    py::str root = root_in && !root_in->empty() ? *root_in : tree_root();
    if (!existing_cache_().contains(root)) {
        py::dict<py::str, py::str> found;
        py::list<py::str> tops;
        if (py::os::path::isdir(root)) tops = py::sorted(py::os::listdir(root));
        for (const py::str& top : tops) {
            py::str base = py::os::path::join(root, top);
            if (_SKIP.contains(top) || top.startswith(py::str(".")) || !py::os::path::isdir(base)) continue;
            for (const py::str& name : py::sorted(py::os::listdir(base))) {
                if (py::os::path::isdir(py::os::path::join(base, name)))
                    found.setdefault(name.lower(), top + py::str("/") + name);
            }
        }
        existing_cache_().set(root, found);
    }
    return existing_cache_().at(root);
}

std::tuple<py::str, py::str> resolve(const py::str& module, const py::str& path) {
    py::str name = py::os::path::basename(module).lower();
    py::str stem = py::re::sub(py::str("\\.[^.]+$"), py::str(""), name);
    auto here = existing().get(stem);
    if (here && !here->empty()) return {*here, py::str("already there")};
    Table t = _table();
    if (need_(t.known, "known").contains(stem)) return {need_(t.known, "known").at(stem), py::str("known")};
    std::optional<py::str> kind;
    if (!path.empty()) kind = base::corpus::lane(path);
    const auto& kinds = need_(t.kind, "kind");   // (`kind in t["kind"]` reads the entry whatever kind is)
    if (kind && kinds.contains(*kind)) return {kinds.at(*kind) + py::str("/") + stem, py::str("kind")};
    py::str where = path.replace(py::str("\\"), py::str("/")).lower();
    for (const auto& kv : need_(t.installed, "installed").items()) {
        if (where.contains(std::get<0>(kv))) return {std::get<1>(kv) + py::str("/") + stem, py::str("installed")};
    }
    py::str ext = std::get<1>(py::os::path::splitext(name));
    if (need_(t.suffix, "suffix").contains(ext)) return {need_(t.suffix, "suffix").at(ext) + py::str("/") + stem, py::str("suffix")};
    for (const auto& pc : need_(t.prefix, "prefix")) {
        if (stem.startswith(std::get<0>(pc))) return {std::get<1>(pc) + py::str("/") + stem, py::str("prefix")};
    }
    return {py::str("unsorted/") + stem, py::str("unsorted")};
}

py::str folder(const py::str& module, const py::str& path, bool create) {
    auto [rel, how] = resolve(module, path);
    (void)how;
    py::str d = tree_root();
    for (const py::str& part : py::split(rel, py::str("/"))) d = py::os::path::join(d, part);
    if (create) py::os::makedirs(d, 0777, true);
    return d;
}

int64_t main(const py::list<py::str>& argv) {
    py::list<py::str> names;
    for (const py::str& a : argv)
        if (!a.startswith(py::str("--"))) names.append(a);
    const bool all = argv.contains(py::str("--all"));
    py::list<py::str> paths;
    if (all) {
        for (const char* k : {"pe", "ne"})
            for (const py::str& p : base::corpus::modules(py::str(k))) paths.append(p);
        for (const py::str& p : base::corpus::modules(py::str("le"), py::list<py::str>{py::str(".vxd"), py::str(".386")})) paths.append(p);
        for (const py::str& p : formats::dos::programs()) paths.append(p);
    } else {
        for (const py::str& n : names) {
            auto r = base::corpus::retail(n);
            paths.append(r && !r->empty() ? *r : n);
        }
    }
    if (paths.empty()) {
        py::print(py::str("python -m codebreaker place <module> ... | --all\n\n"
                          "Say where each module's source goes in the source tree, and how that was decided. Nothing\n"
                          "is written."));
        return 2;
    }
    py::dict<py::str, int64_t> how_many;
    for (const py::str& p : paths) {
        auto [rel, how] = resolve(py::os::path::basename(p), py::os::path::isfile(p) ? p : py::str(""));
        how_many.set(how, how_many.get(how, 0) + 1);
        if (!all || argv.contains(py::str("--list")))
            py::print(py::percent("%-16s %-32s %s", py::os::path::basename(p).lower(), rel, how));
    }
    std::vector<std::tuple<py::str, int64_t>> kvs = how_many.items();
    std::stable_sort(kvs.begin(), kvs.end(), [](const auto& a, const auto& b) { return std::get<1>(a) > std::get<1>(b); });
    py::list<py::str> parts;
    for (const auto& kv : kvs) parts.append(std::get<0>(kv) + py::str(" ") + py::to_str(std::get<1>(kv)));
    py::print(py::percent("%d modules: %s", py::len(paths), py::join(py::str(", "), parts)));
    return 0;
}

}  // namespace codebreaker::source::placement
