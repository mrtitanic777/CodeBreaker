// codebreaker/exact/readable.cpp -- C++ port of codebreaker.exact.readable.

#include "codebreaker/exact/readable.hpp"

#include <filesystem>
#include <random>

namespace codebreaker::exact::readable {

namespace {

// open(p, encoding="latin1", newline="").read()
py::str read_latin1(const py::str& p) {
    py::file f = py::open_rb(p);
    py::bytes b = f.read();
    f.close();
    return b.decode("latin1");
}

// open(p, "w", encoding="latin1", newline="").write(t)
void write_latin1(const py::str& p, const py::str& t) {
    py::bytes b = t.encode("latin1");   // (UnicodeEncodeError past latin1, as Python's)
    py::file f = py::open(p, py::str("wb"));
    f.write(b);
    f.close();
}

int64_t count_identical(const Counts& d) {
    int64_t n = 0;
    for (const auto& kv : d.items()) {
        if (std::get<1>(kv) == py::str("identical")) ++n;
    }
    return n;
}

}  // namespace

const py::re::Pattern& ADDRNAME() {
    static const py::re::Pattern p = py::re::compile(py::str(R"(\b(?:_?f([0-9a-f]{5})|g_([0-9a-f]{1,4}))\b)"));
    return p;
}

Names mapping(const Symbols& symbols) {
    Names out;
    static const py::re::Pattern ident = py::re::compile(py::str(R"([A-Za-z_]\w*)"));
    for (const auto& kv : symbols.items()) {
        const py::str& n = std::get<0>(kv);
        const py::str& t = std::get<0>(std::get<1>(kv));
        int64_t a = std::get<1>(std::get<1>(kv));
        if (n.contains(py::str(":")) || n.contains(py::str("@")) || ADDRNAME().fullmatch(n.lstrip(py::str("_"))) ||
            ADDRNAME().fullmatch(n)) {
            continue;
        }
        py::str c = n.startswith(py::str("_")) ? n.slice_from(1) : n;   // (the C name of a public _name)
        if (!ident.fullmatch(c)) {
            continue;
        }
        if (t == py::str("C") || t == py::str("?")) {
            out.setdefault(Place(py::str("f"), a), c);
        }
        if ((t == py::str("D") || t == py::str("?")) && a < 0x10000) {
            out.setdefault(Place(py::str("g"), a), c);
        }
    }
    return out;
}

Names mapping(const Project& prj) { return mapping(prj.symbols()); }

py::str rename(const py::str& text, const Names& names) {
    static const py::re::Pattern word = py::re::compile(py::str(R"(\b[A-Za-z_]\w*\b)"));
    static const py::re::Pattern literal = py::re::compile(
        py::str(R"((/\*.*?\*/|"(?:[^"\\\n]|\\.)*"|'(?:[^'\\\n]|\\.)*'))"), py::re::S);
    py::set<py::str> have;
    for (const auto& w : word.findall(text)) have.add(w);
    py::dict<py::str, Place> done;

    auto one = [&](const py::re::Match& m) -> py::str {
        std::optional<py::str> f = m.group_opt(1);
        Place k = f ? Place(py::str("f"), py::int_from_str(*f, 16)) : Place(py::str("g"), py::int_from_str(m.group(2), 16));
        std::optional<py::str> n = names.get(k);
        if (n && !n->u32().empty()) {
            std::optional<Place> d = done.get(*n);
            if (!have.contains(*n) || (d && *d == k)) {
                done.set(*n, k);
                return *n;
            }
        }
        return m.group(0);
    };

    py::list<py::str> parts = literal.split(text);
    std::u32string out;
    int64_t k = 0;
    for (const auto& p : parts) {
        // (not in a remark or a string: another program's place, or text)
        out += ((k % 2) ? p : ADDRNAME().sub(one, p)).u32();
        ++k;
    }
    return py::str(out);
}

Counts counts(const py::list<Row>& rows) {
    Counts out;
    for (const auto& r : rows) {
        out.set(std::make_tuple(r.file, r.name), r.verdict);
    }
    return out;
}

Counts counts(const Project& prj) { return counts(prj.ledger_run()); }

int64_t main(const Project& prj, const py::list<py::str>& argv) {
    Names names = mapping(prj);
    if (names.empty()) {
        py::print(py::str("SYMBOLS has no names for the address-names (the names stage puts them there)"));
        return 1;
    }
    py::list<py::str> files;
    for (const auto& s : prj.sources()) files.append(std::get<1>(s));
    py::dict<py::str, py::str> fresh;
    for (const auto& p : files) {
        py::str t = read_latin1(p);
        py::str r = rename(t, names);
        if (r != t) {
            fresh.set(p, r);
        }
    }
    py::print(py::str(std::to_string(fresh.size()) + " of " + std::to_string(files.size()) + " files would take names (" +
                      std::to_string(names.size()) + " names known)"));
    if (fresh.empty()) {
        return 0;
    }
    Counts before = counts(prj);
    py::str w = py::tempfile::mkdtemp(std::nullopt, py::str("readable_"));
    Counts after;
    try {   // (the project again, its sources renamed)
        namespace fs = std::filesystem;
        py::str src = py::os::path::join(w, py::str("src"));
        std::error_code ec;
        fs::copy(fs::u8path(prj.src().utf8()), fs::u8path(src.utf8()), fs::copy_options::recursive, ec);
        if (ec) throw py::OSError(ec.message());
        for (const auto& kv : fresh.items()) {
            write_latin1(py::os::path::join(src, py::os::path::relpath(std::get<0>(kv), prj.src())), std::get<1>(kv));
        }
        std::unique_ptr<Project> twin = prj.with_src(src);
        after = counts(*twin);
    } catch (...) {
        py::shutil::rmtree(w, true);
        throw;
    }
    py::shutil::rmtree(w, true);
    // (Python also builds same_a = {(f, n) ...} here and never reads it: nothing to carry over)
    int64_t same_b = count_identical(before);
    int64_t same_after = count_identical(after);
    int64_t lost = same_b - same_after;
    py::print(py::str("identical before: " + std::to_string(same_b) + ", after the renaming: " + std::to_string(same_after)));
    if (lost > 0) {
        py::print(py::str("not kept: the renaming loses " + std::to_string(lost)));
        return 1;
    }
    if (argv.contains(py::str("--write"))) {
        for (const auto& kv : fresh.items()) {
            write_latin1(std::get<0>(kv), std::get<1>(kv));
        }
        py::print(py::str("written: " + std::to_string(fresh.size()) + " files"));
    }
    return 0;
}

}  // namespace codebreaker::exact::readable
