// codebreaker/source/vxd/services.cpp -- typed C++ of codebreaker.source.vxd.services (see services.hpp).
#include "codebreaker/source/vxd/services.hpp"

#include <filesystem>
#include <list>

#include "codebreaker/base/corpus.hpp"
#include "py_settings.hpp"

namespace codebreaker::source::vxd::services {

namespace {

// ---- glob.glob (the runtime has none) ---------------------------------------------------------

bool has_magic_(const py::str& s) {
    for (char32_t c : s.u32())
        if (c == U'*' || c == U'?' || c == U'[') return true;
    return false;
}

bool lexists_(const py::str& p) {
    if (p.u32().find(U'\0') != std::u32string::npos) return false;
    std::error_code ec;
    std::filesystem::file_status st = std::filesystem::symlink_status(std::filesystem::u8path(p.utf8()), ec);
    return !ec && st.type() != std::filesystem::file_type::not_found;
}

// fnmatch's pattern against a name, both already normcase'd
bool fn_match_(const std::u32string& p, size_t pi, const std::u32string& n, size_t ni) {
    while (pi < p.size()) {
        char32_t c = p[pi];
        if (c == U'*') {
            while (pi < p.size() && p[pi] == U'*') pi++;
            if (pi == p.size()) return true;
            for (size_t k = ni; k <= n.size(); k++)
                if (fn_match_(p, pi, n, k)) return true;
            return false;
        }
        if (c == U'?') {
            if (ni >= n.size()) return false;
            pi++;
            ni++;
            continue;
        }
        if (c == U'[') {
            size_t j = pi + 1;
            if (j < p.size() && p[j] == U'!') j++;
            if (j < p.size() && p[j] == U']') j++;
            while (j < p.size() && p[j] != U']') j++;
            if (j >= p.size()) {   // no closing bracket: a literal [
                if (ni >= n.size() || n[ni] != U'[') return false;
                pi++;
                ni++;
                continue;
            }
            if (ni >= n.size()) return false;
            size_t a = pi + 1;
            bool neg = false;
            if (p[a] == U'!') {
                neg = true;
                a++;
            }
            bool hit = false;
            for (size_t i = a; i < j; i++) {
                if (i + 2 < j && p[i + 1] == U'-') {
                    if (p[i] <= n[ni] && n[ni] <= p[i + 2]) hit = true;
                    i += 2;
                } else if (p[i] == n[ni]) {
                    hit = true;
                }
            }
            if (hit == neg) return false;
            pi = j + 1;
            ni++;
            continue;
        }
        if (ni >= n.size() || n[ni] != c) return false;
        pi++;
        ni++;
    }
    return ni == n.size();
}

bool fnmatch_(const py::str& name, const py::str& pat) {
    return fn_match_(py::os::path::normcase(pat).u32(), 0, py::os::path::normcase(name).u32(), 0);
}

bool ishidden_(const py::str& s) { return !s.empty() && s.u32()[0] == U'.'; }

// glob._glob1(dirname, pattern): the names in dirname matching a wildcard pattern
py::list<py::str> glob1_(const py::str& dirname, const py::str& pattern) {
    py::list<py::str> names;
    try {
        names = py::os::listdir(dirname.empty() ? py::os::curdir : dirname);
    } catch (const py::OSError&) {
        return py::list<py::str>{};
    }
    py::list<py::str> out;
    bool hidden_ok = ishidden_(pattern);
    for (const py::str& n : names)
        if ((hidden_ok || !ishidden_(n)) && fnmatch_(n, pattern)) out.append(n);
    return out;
}

// glob._glob0(dirname, basename): a name that is not a wildcard
py::list<py::str> glob0_(const py::str& dirname, const py::str& basename) {
    py::list<py::str> out;
    if (basename.empty()) {
        if (py::os::path::isdir(dirname)) out.append(basename);
    } else if (lexists_(py::os::path::join(dirname, basename))) {
        out.append(basename);
    }
    return out;
}

// glob.glob(pathname)
py::list<py::str> glob(const py::str& pathname) {
    py::list<py::str> out;
    auto [dirname, basename] = py::os::path::split(pathname);
    if (!has_magic_(pathname)) {
        if (!basename.empty()) {
            if (lexists_(pathname)) out.append(pathname);
        } else if (py::os::path::isdir(dirname)) {
            out.append(pathname);
        }
        return out;
    }
    if (dirname.empty()) {
        for (const py::str& n : glob1_(py::str(""), basename)) out.append(n);
        return out;
    }
    py::list<py::str> dirs;
    if (dirname != pathname && has_magic_(dirname)) dirs = glob(dirname);
    else dirs = py::list<py::str>{dirname};
    for (const py::str& d : dirs) {
        py::list<py::str> names = has_magic_(basename) ? glob1_(d, basename) : glob0_(d, basename);
        for (const py::str& n : names) out.append(py::os::path::join(d, n));
    }
    return out;
}

// open(path, encoding="latin1").read(): text mode, so \r\n and a lone \r are \n
py::str read_latin1_text_(const py::str& path) {
    py::bytes raw;
    {
        py::file f = py::open_rb(path);
        raw = f.read();
    }
    return raw.decode("latin1").replace(py::str("\r\n"), py::str("\n")).replace(py::str("\r"), py::str("\n"));
}

}  // namespace

py::str HERE() {
    static const py::str h = py::os::path::join(py::settings::checkout_root(), py::str("codebreaker"), py::str("source"), py::str("vxd"));
    return h;
}

py::str ROOT() {
    static const py::str r = py::os::path::dirname(py::os::path::dirname(HERE()));
    return r;
}

py::list<py::str> _candidates() {
    py::str dr = py::os::path::dirname(py::settings::checkout_root());   // dirname(settings.ROOT)
    py::list<py::str> out;
    out.append(base::corpus::reference(py::list<py::str>{py::str("Microsoft Windows 95 DDK"), py::str("INC32")}));   // the moved reference SDK/DDK tree
    // the sibling Win95 source repo (data_root is the checkouts' parent), tried directly so the
    // bounded fallback below is not needed in the normal layout
    out.append(py::os::path::join(dr, py::str("Windows95"), py::str("design"), py::str("reference"),
                                  py::str("Microsoft Windows 95 DDK"), py::str("INC32")));
    out.append(dr + py::str("/docs/reference/win95ddk/INC32"));
    out.append(dr + py::str("/docs/reference/loganxp/win95ddk/INC32"));
    return out;
}

const py::set<py::str> _PRUNE = py::set<py::str>{py::str(".git"), py::str(".hg"), py::str(".svn"), py::str("node_modules"),
                                                 py::str("__pycache__"), py::str(".venv"), py::str("venv"),
                                                 py::str("open-watcom-v2"), py::str("binaries"), py::str("corpus")};

std::optional<py::str> _find_inc_dir_in(const py::list<py::str>& candidates) {
    for (const py::str& p : candidates)
        if (!p.empty() && py::os::path::isdir(p)) return p;
    // Bounded fallback. The old code did glob("**", recursive=True) over design_root(),
    // which -- because data_root()/design_root() default to the PARENT of the checkout --
    // crawled every sibling repo on disk and appeared to hang. Walk a shallow depth only,
    // pruning VCS/build/corpus dirs, so a missing config degrades gracefully instead of
    // scanning the whole tree.
    py::str root = base::corpus::design_dir();
    if (root.empty() || !py::os::path::isdir(root)) return std::nullopt;
    int64_t base_depth = root.rstrip(py::str("\\")).count(py::str("\\"));
    for (const auto& entry : py::os::walk(root)) {
        const py::str& dirpath = std::get<0>(entry);
        py::list<py::str> dirs = std::get<1>(entry);
        const py::list<py::str>& files = std::get<2>(entry);
        if (dirpath.count(py::str("\\")) - base_depth >= 4) {   // cap depth
            dirs.clear();
            continue;
        }
        py::list<py::str> kept;
        for (const py::str& d : dirs)
            if (!_PRUNE.contains(d) && !d.startswith(py::str("."))) kept.append(d);
        dirs.clear();
        for (const py::str& d : kept) dirs.append(d);
        if (py::os::path::basename(dirpath).upper() == py::str("INC32") && files.contains(py::str("VMM.INC"))) return dirpath;
    }
    return std::nullopt;
}

std::optional<py::str> _find_inc_dir() { return _find_inc_dir_in(_candidates()); }

namespace {

// functools.lru_cache(maxsize=8) of _parse, most recently used first
using ParseCache = std::list<std::pair<py::str, py::dict<py::str, py::list<py::str>>>>;
ParseCache& parse_cache_() {
    static ParseCache c;
    return c;
}

py::dict<py::str, py::list<py::str>> parse_uncached_(const py::str& inc_dir) {
    py::dict<py::str, py::list<py::str>> tables;
    for (const py::str& fp : glob(py::os::path::join(inc_dir, py::str("*.INC")))) {
        py::str txt;
        try {
            txt = read_latin1_text_(fp);
        } catch (const py::OSError&) {
            continue;
        }
        for (const py::re::Match& m : py::re::finditer(py::str("Begin_Service_Table\\s+(\\w+)(.*?)(?:End_Service_Table|\\Z)"),
                                                       txt, py::re::S | py::re::I)) {
            py::str dev = m.group(1).upper();
            py::list<py::str> names = py::re::findall(py::str("^\\s*") + py::re::escape(dev) + py::str("_Service\\s+(\\w+)"),
                                                      m.group(2), py::re::M | py::re::I);
            if (!names.empty() && !tables.contains(dev)) tables.set(dev, names);   // first (canonical) table wins
        }
    }
    return tables;
}

}  // namespace

py::dict<py::str, py::list<py::str>> _parse(const py::str& inc_dir) {
    ParseCache& c = parse_cache_();
    for (auto it = c.begin(); it != c.end(); ++it) {
        if (it->first == inc_dir) {
            c.splice(c.begin(), c, it);
            return c.front().second;
        }
    }
    py::dict<py::str, py::list<py::str>> t = parse_uncached_(inc_dir);
    c.emplace_front(inc_dir, t);
    if (c.size() > 8) c.pop_back();
    return t;
}

std::optional<py::list<py::str>> Services::names(const std::optional<py::str>& device) const {
    py::str key = (device && !device->empty() ? *device : py::str("")).upper();   // (device or "").upper()
    return tables.get(key);
}

std::optional<py::str> Services::name(const std::optional<py::str>& device, const std::optional<int64_t>& ordinal) const {
    std::optional<py::list<py::str>> ns = names(device);
    if (!ns || ns->empty() || !ordinal) return std::nullopt;
    for (int64_t o : {*ordinal, *ordinal & 0x7FFF}) {
        if (0 <= o && o < py::len(*ns)) return (*ns)[o];
    }
    return std::nullopt;
}

Services load(const std::optional<py::str>& inc_dir_in) {
    std::optional<py::str> inc_dir = inc_dir_in;
    if (!inc_dir || inc_dir->empty()) inc_dir = _find_inc_dir();
    return Services(inc_dir && !inc_dir->empty() ? _parse(*inc_dir) : py::dict<py::str, py::list<py::str>>());
}

}  // namespace codebreaker::source::vxd::services
