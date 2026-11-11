// codebreaker/exact/identify.cpp -- typed C++ port of codebreaker.exact.identify (see identify.hpp).

#include "codebreaker/exact/identify.hpp"

#include <algorithm>
#include <set>
#include <vector>

#include "codebreaker/exact/toolchain.hpp"
#include "py_os.hpp"
#include "py_random.hpp"
#include "value.hpp"
#include "json.hpp"

namespace codebreaker::exact::identify {

namespace tcm = codebreaker::exact::toolchain;

Loader default_loader() {
    return [](const py::str& name, const std::optional<py::str>& variant) -> std::shared_ptr<ledger::Toolchain> {
        return tcm::load(name, variant);
    };
}

namespace {

// sorted() of the (address or None, path) pairs as Python compares tuples: None against a number is a TypeError
bool file_less(const File& a, const File& b) {
    const auto& x = std::get<0>(a);
    const auto& y = std::get<0>(b);
    if (x.has_value() != y.has_value()) {
        throw py::TypeError(x ? "'<' not supported between instances of 'int' and 'NoneType'"
                              : "'<' not supported between instances of 'NoneType' and 'int'");
    }
    if (x && *x != *y) return *x < *y;
    return std::get<1>(a).u32() < std::get<1>(b).u32();
}

}  // namespace

void shuffle(int64_t seed, py::list<File>& x) {
    py::random::Random rnd(seed);   // random.Random(seed).shuffle(x)
    rnd.shuffle(x);
}

py::list<Candidate> candidates(const Project& prj, const std::optional<py::list<py::str>>& tries) {
    py::list<Candidate> out;
    if (tries && !tries->empty()) {
        for (const py::str& t : *tries) {
            // nv, _, sw = t.partition("/"); n, _, v = nv.partition(":")
            py::str nv = t, sw("");
            const int64_t slash = t.find(py::str("/"));
            if (slash >= 0) {
                nv = t.slice(0, slash);
                sw = t.slice(slash + 1, py::len(t));
            }
            py::str n = nv, v("");
            const int64_t colon = nv.find(py::str(":"));
            if (colon >= 0) {
                n = nv.slice(0, colon);
                v = nv.slice(colon + 1, py::len(nv));
            }
            out.append(Candidate(n, v.empty() ? std::nullopt : std::optional<py::str>(v), sw.empty() ? std::nullopt : std::optional<py::str>(sw)));
        }
        return out;
    }
    std::set<std::u32string> ext;
    for (const auto& s : prj.sources()) ext.insert(std::get<1>(py::os::path::splitext(std::get<1>(s))).lower().u32());
    for (const py::str& n : tcm::profiles()) {
        const py::str text = py::file::open(py::os::path::join(tcm::HERE(), py::str("toolchains"), n + py::str(".json")), py::str("r")).read_text();
        const py::V p = py::json_parse(text.utf8());
        // se = (p.get("source_ext") or (".pas" if "pascal" in n else ".plm" if "plm" in n else ".c")).lower()
        py::str se(".c");
        if (n.contains(py::str("pascal"))) se = py::str(".pas");
        else if (n.contains(py::str("plm"))) se = py::str(".plm");
        const py::V& given = p.get("source_ext");
        if (given.is_str() && !given.str().empty()) se = py::str(given.str());
        se = se.lower();
        if (!ext.count(se.u32())) continue;
        // for v in [None]: (the default variant; --try names others)
        out.append(Candidate(n, std::nullopt, std::nullopt));
        const py::V& alt = p.get("try_switches");
        if (alt.is_list()) {
            for (const py::V& sw : alt.seq()) out.append(Candidate(n, std::nullopt, std::optional<py::str>(py::str(sw.str()))));
        }
    }
    return out;
}

std::tuple<int64_t, py::str> score(Project& prj, const Candidate& cand, const py::list<File>& files, const Loader& load) {
    const py::str& n = std::get<0>(cand);
    const std::optional<py::str>& v = std::get<1>(cand);
    const std::optional<py::str>& sw = std::get<2>(cand);
    std::shared_ptr<ledger::Toolchain> tc;
    try {
        tc = load(n, v);
    } catch (const py::Exception& e) {
        return {-1, py::str(e.what())};
    }
    const compare::Symbols syms = prj.symbols();
    int64_t same = 0, total = 0;
    const std::optional<py::str> old = prj.switches();
    prj.set_switches(sw ? sw : old);
    try {
        for (const File& f : files) {
            compare::Symbols learnt;
            for (const auto& r : ledger::check_file(prj, *tc, std::get<1>(f), std::get<0>(f), syms, learnt)) {
                total += 1;
                same += (std::get<4>(r) == py::str("identical")) ? 1 : 0;
            }
        }
    } catch (...) {
        prj.set_switches(old);
        throw;
    }
    prj.set_switches(old);
    return {same, py::percent(py::str("%d of %d"), same, total)};
}

int64_t main(Project& prj, const py::list<py::str>& argv, std::ostream& out, const Loader& load) {
    py::list<py::str> tries;
    for (int64_t k = 0; k < argv.size(); k++) {
        if (argv[k] == py::str("--try") && k + 1 < argv.size()) tries.append(argv[k + 1]);
    }
    int64_t n = 12;
    for (int64_t k = 0; k < argv.size(); k++) {   // argv.index("--sample")
        if (argv[k] == py::str("--sample")) {
            if (k + 1 >= argv.size()) throw py::IndexError("list index out of range");
            n = py::int_parse(argv[k + 1], 10);
            break;
        }
    }
    std::vector<File> sorted_files;
    for (const auto& s : prj.sources()) sorted_files.push_back(s);
    std::stable_sort(sorted_files.begin(), sorted_files.end(), file_less);
    py::list<File> files;
    for (const File& f : sorted_files) files.append(f);
    shuffle(0, files);
    // files = files[:n]
    py::list<File> sample;
    const int64_t count = n < 0 ? std::max<int64_t>(0, files.size() + n) : std::min<int64_t>(n, files.size());
    for (int64_t i = 0; i < count; i++) sample.append(files[i]);

    std::optional<std::tuple<int64_t, Candidate>> best;
    std::optional<py::list<py::str>> tr;
    if (tries.size()) tr = tries;
    for (const Candidate& c : candidates(prj, tr)) {
        auto [s, txt] = score(prj, c, sample, load);
        out << py::percent(py::str("   %-10s %-8s %-8s %s"), std::get<0>(c), std::get<1>(c) ? *std::get<1>(c) : py::str(""),
                           std::get<2>(c) ? *std::get<2>(c) : py::str(""), txt).utf8() << "\n";
        if (!best || s > std::get<0>(*best)) best = std::make_tuple(s, c);
    }
    if (!best || std::get<0>(*best) <= 0) {
        out << "no toolchain makes any of the sample's functions the program's\n";
        return 1;
    }
    const Candidate& cb = std::get<1>(*best);
    const py::str& name = std::get<0>(cb);
    const std::optional<py::str>& var = std::get<1>(cb);
    const std::optional<py::str>& sw = std::get<2>(cb);
    out << "the best: " << name.utf8() << (var && !var->empty() ? ":" + var->utf8() : std::string()) << (sw && !sw->empty() ? " " + sw->utf8() : std::string()) << "\n";
    bool write = false;
    for (const py::str& a : argv) {
        if (a == py::str("--write")) write = true;
    }
    if (write) {
        const py::str p = py::os::path::join(prj.dir(), py::str("project.json"));
        py::V c;
        {
            py::file f = py::file::open(p, py::str("r"));
            c = py::json_parse(f.read_text().utf8());
            f.close();
        }
        c.set("toolchain", py::V(name.utf8()));
        if (var && !var->empty()) c.set("variant", py::V(var->utf8()));
        else c.erase(py::V("variant"));
        if (sw && !sw->empty()) c.set("switches", py::V(sw->utf8()));
        py::DumpOpts dump_opts;
        dump_opts.indent = 1;   // json.dump(c, f, indent=1)
        std::string text = py::json_dump(c, dump_opts);
        py::file f = py::file::open(p, py::str("w"));
        f.write(py::str(text));
        f.close();
        out << "written to " << p.utf8() << "\n";
    }
    return 0;
}

}  // namespace codebreaker::exact::identify
