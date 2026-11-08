// codebreaker/build/driver.cpp -- C++17 port of codebreaker.build.driver (see driver.hpp).

#include "codebreaker/build/driver.hpp"

#include <cstdio>
#include <functional>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/watcom.hpp"
#include "codebreaker/symbols/driver_services.hpp"
#include "py_settings.hpp"
#include "py_subprocess.hpp"

namespace codebreaker::build::driver {

namespace le = codebreaker::formats::le;

const py::list<std::tuple<py::str, py::str>> SEGMENT_CLASSES = {
    {py::str("LCODE"), py::str("preload")},
    {py::str("MCODE"), py::str("preload iopl")},
    {py::str("ICODE"), py::str("discardable")},
    {py::str("PCODE"), py::str("loadoncall")},
    {py::str("PDATA"), py::str("loadoncall shared")},
    {py::str("SCODE"), py::str("resident")},
    {py::str("DBOCODE"), py::str("preload conforming")},
    {py::str("16ICODE"), py::str("preload discardable")}};

const py::str _DECLARE_PATTERN = py::str("^\\s*Declare_Virtual_Device\\s+(\\w+)");
const int64_t _TIMEOUT = 300;

namespace {

// settings.state(*parts): a path inside the state folder, which is created if it is not there.
py::str state_(const py::list<py::str>& parts) {
    py::str path = py::settings::get_str(py::str("state"));
    for (const py::str& p : parts) {
        path = py::os::path::join(path, p);
    }
    py::os::makedirs(parts.empty() ? path : py::os::path::dirname(path), 0777, true);
    return path;
}

// Sets the environment of the process to `env` for the life of the object, then puts back what it changed.
class ScopedEnv {
  public:
    explicit ScopedEnv(const py::dict<py::str, py::str>& env) {
        for (const auto& kv : env.items()) {
            const std::wstring name = py::subprocess::to_wide(std::get<0>(kv));
            const std::wstring value = py::subprocess::to_wide(std::get<1>(kv));
            wchar_t probe[2];
            const DWORD n = GetEnvironmentVariableW(name.c_str(), probe, 2);
            if (n == 0 && GetLastError() == ERROR_ENVVAR_NOT_FOUND) {
                saved_.push_back({name, std::nullopt});
            } else {
                std::wstring old(n + 1, L'\0');
                const DWORD got = GetEnvironmentVariableW(name.c_str(), old.data(), static_cast<DWORD>(old.size()));
                old.resize(got);
                saved_.push_back({name, old});
            }
            SetEnvironmentVariableW(name.c_str(), value.c_str());
        }
    }
    ~ScopedEnv() {
        for (auto it = saved_.rbegin(); it != saved_.rend(); ++it) {
            SetEnvironmentVariableW(it->first.c_str(), it->second ? it->second->c_str() : nullptr);
        }
    }
    ScopedEnv(const ScopedEnv&) = delete;
    ScopedEnv& operator=(const ScopedEnv&) = delete;

  private:
    std::vector<std::pair<std::wstring, std::optional<std::wstring>>> saved_;
};

void print_(const py::str& s) {
    const std::string u = s.utf8() + "\n";
    std::fwrite(u.data(), 1, u.size(), stdout);
}

}  // namespace

py::str tool(const py::str& name) {
    py::str root = watcom::openwatcom_root();
    py::str exe = py::os::path::join(root, py::str("binnt"), name + py::str(".exe"));
    return py::os::path::isfile(exe) ? exe : watcom::ow_tool(name);
}

py::str kit_folder() {
    py::str src = symbols::driver_services::include_dir();
    if (src.empty()) {
        return py::str("");
    }
    if (!src.contains(py::str(" "))) {
        return src;
    }
    py::str dst = state_(py::list<py::str>{py::str("driver_kit")});
    if (!py::os::path::isfile(py::os::path::join(dst, py::str("VMM.INC")))) {
        py::os::makedirs(dst, 0777, true);
        for (const py::str& fn : py::os::listdir(src)) {
            if (fn.upper().endswith(py::str(".INC"))) {
                py::shutil::copyfile(py::os::path::join(src, fn), py::os::path::join(dst, fn));
            }
        }
    }
    return !dst.contains(py::str(" ")) ? dst : py::str("");
}

py::str source_of(const py::str& path) {
    if (py::os::path::isdir(path)) {
        py::str name = py::os::path::basename(py::os::path::normpath(path));
        py::str cand = py::os::path::join(path, name + py::str(".asm"));
        if (!py::os::path::isfile(cand)) {
            throw py::ValueError(py::percent("%s: no %s.asm in it", path, name).utf8());
        }
        return cand;
    }
    return path;
}

std::optional<py::str> original_of(const py::str& source) {
    py::str stem = std::get<0>(py::os::path::splitext(py::os::path::basename(source))).lower();
    for (const py::str& path : le::corpus_files()) {
        if (std::get<0>(py::os::path::splitext(py::os::path::basename(path))).lower() == stem) {
            return path;
        }
    }
    return std::nullopt;
}

std::tuple<int64_t, py::str> _run(const py::list<py::str>& cmd, const py::str& cwd) {
    try {
        ScopedEnv env(watcom::ow_env(py::str("pe")));
        py::subprocess::RunOptions o;
        o.cwd = cwd;
        o.text = false;
        py::subprocess::CompletedProcess p = py::subprocess::run(cmd, o);
        py::bytes both = *p.stdout_bytes + *p.stderr_bytes;
        return {p.returncode, both.decode("latin1", "replace").strip()};
    } catch (const py::OSError& e) {
        return {-1, py::str(e.what())};
    }
}

std::tuple<std::optional<py::str>, py::str> assemble(const py::str& source, const py::str& work) {
    py::str wasm = tool(py::str("wasm"));
    py::str kit = kit_folder();
    if (wasm.empty() || kit.empty()) {
        return {std::nullopt, wasm.empty() ? py::str("no assembler") : py::str("the driver kit's include files were not found")};
    }
    py::str obj = py::os::path::join(work, std::get<0>(py::os::path::splitext(py::os::path::basename(source))) + py::str(".obj"));
    if (py::os::path::exists(obj)) {
        py::os::remove(obj);
    }
    auto [code, log] = _run(py::list<py::str>{wasm, py::str("-zq"), py::str("-dIS_32=1"), py::str("-i=") + kit, py::str("-fo=") + obj,
                                              py::os::path::abspath(source)},
                            work);
    return {(code == 0 && py::os::path::isfile(obj)) ? std::optional<py::str>(obj) : std::nullopt, log};
}

std::tuple<std::optional<py::str>, py::str> link(const py::str& obj, const py::str& device, const py::str& out_, bool dynamic,
                                                 const std::optional<py::list<std::tuple<py::str, py::str>>>& classes, bool named,
                                                 const std::optional<py::str>& definitions) {
    py::str wlink = tool(py::str("wlink"));
    if (wlink.empty()) {
        return {std::nullopt, py::str("no linker")};
    }
    py::str out = py::os::path::abspath(out_);   // the linker runs with its folder as cwd
    py::str stem = std::get<0>(py::os::path::splitext(out));
    py::str script = stem + py::str(".lnk");
    py::list<py::str> lines = {py::str("format windows vxd") + (dynamic ? py::str(" virtdevice") : py::str("")),
                               py::str("option quiet"),
                               py::str("option map=") + (stem + py::str(".map")),
                               py::str("name ") + out,
                               py::str("file ") + obj};
    const py::str defs = definitions ? *definitions : py::str("");
    auto described = py::re::search(py::str("DESCRIPTION\\s+'((?:[^']|'')*)'"), defs);
    if (described) {
        lines.insert(2, py::str("option description '") + described->group(1) + py::str("'"));
    }
    auto exports = py::re::compile(py::str("^\\s+(\\w+)\\s+@(\\d+)"), py::re::M).findall_groups<py::str, py::str>(defs);
    py::list<py::str> directives;
    if (!exports.empty()) {
        for (const auto& e : exports) {
            directives.append(py::str("export ") + std::get<0>(e) + py::str(".") + std::get<1>(e));
        }
    } else {
        directives.append(py::str("export ") + device + py::str("_DDB") + (named ? py::str("") : py::str(".1")));
    }
    lines.set_slice(-1, -1, directives);
    for (const auto& [cls, attrs] : classes ? *classes : SEGMENT_CLASSES) {
        lines.append(py::str("segment class '") + cls + py::str("' ") + attrs);
    }
    {
        py::file f = py::open(script, py::str("w"), std::nullopt, std::nullopt, py::str("\r\n"));
        f.write(py::join(py::str("\n"), lines) + py::str("\n"));
        f.close();
    }
    if (py::os::path::exists(out)) {
        py::os::remove(out);
    }
    auto [code, log] = _run(py::list<py::str>{wlink, py::str("@") + script}, py::os::path::dirname(out));
    return {(code == 0 && py::os::path::isfile(out)) ? std::optional<py::str>(out) : std::nullopt, log};
}

// ---- The structural check ----

py::dict<int64_t, py::set<int64_t>> _patched(const le::LEFile& lf) {
    py::dict<int64_t, py::set<int64_t>> out;
    for (const auto& [obj, off, kind, to, ta] : le::references(lf)) {
        (void)kind;
        (void)to;
        (void)ta;
        py::set<int64_t>& patched = out.setdefault(obj, py::set<int64_t>());
        for (int64_t i = off; i < off + 4; ++i) {
            patched.add(i);
        }
    }
    return out;
}

namespace {

// _code_at with the patched places given (they are the same for every place of one image)
py::bytes code_at_(const le::LEFile& lf, const py::dict<int64_t, py::set<int64_t>>& patched_all, const le::Place& where, int64_t n) {
    const auto [obj, off] = where;
    py::set<int64_t> patched = patched_all.get(obj, py::set<int64_t>());
    py::bytes data = le::object_data(lf, lf.objects[obj - 1]).slice(off, off + n);
    std::string out;
    for (int64_t i = 0; i < data.size(); ++i) {
        if (!patched.contains(off + i)) {
            out.push_back(static_cast<char>(data.raw()[static_cast<size_t>(i)]));
        }
    }
    return py::bytes(out);
}

}  // namespace

py::bytes _code_at(const le::LEFile& lf, const le::Place& where, int64_t n) {
    return code_at_(lf, _patched(lf), where, n);
}

Structure structure(const le::LEFile& lf) {
    Structure s;
    std::optional<le::Descriptor> d = le::descriptor(lf);
    s.descriptor = d;
    s.services = le::service_table(lf, d);
    for (const le::Object& o : lf.objects) {
        s.objects.append(std::make_tuple(o.vsize, o.flags));
    }
    py::list<std::tuple<py::str, int64_t, int64_t, int64_t>> exports = le::exports(lf);
    for (const auto& [n, o, obj, off] : exports) {
        (void)obj;
        (void)off;
        s.exports.append(std::make_tuple(n, o));
    }
    s.export_is_descriptor = false;
    if (d) {
        for (const auto& [n, o, obj, off] : exports) {
            (void)n;
            (void)o;
            if (obj == d->obj && off == d->offset) {
                s.export_is_descriptor = true;
                break;
            }
        }
    }
    s.module = le::header_fields(lf).at(py::str("module_flags"));
    s.references = le::references(lf).size();
    return s;
}

std::tuple<py::dict<py::str, bool>, py::list<py::str>, std::optional<double>> compare(const le::LEFile& built, const le::LEFile& original) {
    Structure a = structure(built);
    Structure b = structure(original);
    py::dict<py::str, bool> checks;
    py::list<py::str> notes;

    auto fail = [&](const char* name, const py::str& text) {
        checks.set(py::str(name), false);
        notes.append(py::percent("%s: %s", py::str(name), text));
    };

    checks.set(py::str("objects"), a.objects == b.objects);
    if (!checks.at(py::str("objects"))) {
        if (a.objects.size() != b.objects.size()) {
            fail("objects", py::percent("%d objects, the original has %d", a.objects.size(), b.objects.size()));
        } else {
            py::list<int64_t> bad;
            for (int64_t i = 0; i < a.objects.size(); ++i) {
                if (!(a.objects[i] == b.objects[i])) {
                    bad.append(i + 1);
                }
            }
            int64_t i = bad[0] - 1;
            fail("objects", py::percent("%d of %d differ; object %d is %#x bytes flags %#x, original %#x flags %#x", bad.size(),
                                        b.objects.size(), bad[0], std::get<0>(a.objects[i]), std::get<1>(a.objects[i]),
                                        std::get<0>(b.objects[i]), std::get<1>(b.objects[i])));
        }
    }
    const auto& da = a.descriptor;
    const auto& db = b.descriptor;
    const py::dict<int64_t, py::set<int64_t>> pa = _patched(built);
    const py::dict<int64_t, py::set<int64_t>> pb = _patched(original);
    checks.set(py::str("descriptor"), true);
    if (!da || !db) {
        fail("descriptor", py::percent("none in the %s", !da ? py::str("rebuilt file") : py::str("original")));
    } else {
        struct Field {
            const char* name;
            std::function<py::str(const le::Descriptor&)> repr;
        };
        const std::vector<Field> fields = {
            {"name", [](const le::Descriptor& d) { return py::repr(d.name); }},
            {"kit_version", [](const le::Descriptor& d) { return py::repr(d.kit_version); }},
            {"device_id", [](const le::Descriptor& d) { return py::repr(d.device_id); }},
            {"major", [](const le::Descriptor& d) { return py::repr(d.major); }},
            {"minor", [](const le::Descriptor& d) { return py::repr(d.minor); }},
            {"flags", [](const le::Descriptor& d) { return py::repr(d.flags); }},
            {"init_order", [](const le::Descriptor& d) { return py::repr(d.init_order); }},
            {"service_count", [](const le::Descriptor& d) { return py::repr(d.service_count); }},
            {"size", [](const le::Descriptor& d) { return py::repr(d.size); }},
            {"v86_api_address", [](const le::Descriptor& d) { return py::repr(d.v86_api_address); }},
            {"pm_api_address", [](const le::Descriptor& d) { return py::repr(d.pm_api_address); }}};
        bool broke = false;
        for (const Field& f : fields) {
            const py::str ra = f.repr(*da), rb = f.repr(*db);
            if (!(ra == rb)) {
                fail("descriptor", py::percent("%s is %s, original %s", py::str(f.name), ra, rb));
                broke = true;
                break;
            }
        }
        if (!broke) {
            struct Place_ {
                const char* name;
                const std::optional<le::Place> le::Descriptor::*member;
                bool compare_code;
            };
            const std::vector<Place_> places = {{"control", &le::Descriptor::control, true},
                                                {"v86_api", &le::Descriptor::v86_api, true},
                                                {"pm_api", &le::Descriptor::pm_api, true},
                                                {"service_table", &le::Descriptor::service_table, false},
                                                {"reference_data", &le::Descriptor::reference_data, false}};
            for (const Place_& p : places) {
                const std::optional<le::Place>& x = (*da).*(p.member);
                const std::optional<le::Place>& y = (*db).*(p.member);
                if (x.has_value() != y.has_value() ||
                    (x && y && p.compare_code && code_at_(built, pa, *x, 12) != code_at_(original, pb, *y, 12))) {
                    fail("descriptor", py::percent("%s %s", py::str(p.name),
                                                   (x && y) ? py::str("starts with different code")
                                                            : py::percent("is %s, original %s", x ? py::str("set") : py::str("empty"),
                                                                          y ? py::str("set") : py::str("empty"))));
                    break;
                }
            }
        }
    }
    const auto& ta = a.services;
    const auto& tb = b.services;
    checks.set(py::str("services"), true);
    if (ta.size() != tb.size()) {
        fail("services", py::percent("%d services, the original has %d", ta.size(), tb.size()));
    } else {
        py::list<int64_t> bad;
        for (int64_t i = 0; i < ta.size(); ++i) {
            const std::optional<le::Place>& x = ta[i];
            const std::optional<le::Place>& y = tb[i];
            if (x.has_value() != y.has_value() || (x && y && code_at_(built, pa, *x, 12) != code_at_(original, pb, *y, 12))) {
                bad.append(i);
            }
        }
        if (!bad.empty()) {
            fail("services", py::percent("%d of %d differ, first service %d", bad.size(), tb.size(), bad[0]));
        }
    }
    checks.set(py::str("exports"), a.exports == b.exports && a.export_is_descriptor == b.export_is_descriptor);
    if (!checks.at(py::str("exports"))) {
        fail("exports", py::percent("%r, original %r", a.exports, b.exports));
    }
    checks.set(py::str("module"), a.module == b.module);
    if (!checks.at(py::str("module"))) {
        fail("module", py::percent("module flags %#x, original %#x", a.module, b.module));
    }
    int64_t equal = 0, total = 0;
    {
        const int64_t n = std::min(built.objects.size(), original.objects.size());
        for (int64_t k = 0; k < n; ++k) {
            const le::Object& x = built.objects[k];
            const le::Object& y = original.objects[k];
            if (x.vsize != y.vsize) {
                continue;
            }
            py::bytes ba = le::object_data(built, x);
            py::bytes bb = le::object_data(original, y);
            py::set<int64_t> skip = pa.get(x.idx, py::set<int64_t>());
            for (int64_t i : pb.get(y.idx, py::set<int64_t>())) {
                skip.add(i);
            }
            total += ba.size() - skip.size();
            for (int64_t i = 0; i < ba.size(); ++i) {
                if (ba[i] == bb[i] && !skip.contains(i)) {
                    ++equal;
                }
            }
        }
    }
    if (a.references != b.references) {
        notes.append(py::percent("places the loader patches: %d, original %d", a.references, b.references));
    }
    return {checks, notes, total != 0 ? std::optional<double>(static_cast<double>(equal) / static_cast<double>(total)) : std::nullopt};
}

BuildResult build(const py::str& source_, const std::optional<py::str>& original_, const std::optional<py::str>& out) {
    py::str source = source_of(source_);
    py::str name = std::get<0>(py::os::path::splitext(py::os::path::basename(source))).lower();
    py::str work = out ? *out : state_(py::list<py::str>{py::str("driver_build"), name});
    py::os::makedirs(work, 0777, true);
    std::optional<py::str> original;
    if (original_ && !original_->empty()) {
        original = base::corpus::retail(*original_);
    } else {
        original = original_of(source);
    }
    BuildResult r;
    r.name = name;
    r.source = source;
    r.original = original;
    py::str text;
    {
        py::file f = py::open(source, py::str("r"), py::str("latin1"));
        text = f.read_text();
        f.close();
    }
    auto m = py::re::compile(_DECLARE_PATTERN, py::re::I | py::re::M).search(text);
    if (!m) {
        r.log = py::str("the source declares no device (no Declare_Virtual_Device)");
        return r;
    }
    auto [obj, alog] = assemble(source, work);
    r.log = alog;
    if (!obj) {
        return r;
    }
    r.assembled = true;
    std::optional<le::LEFile> olf;
    if (original && !original->empty()) {
        py::list<std::tuple<py::str, le::LEFile>> mem = le::members(*original);
        const le::LEFile& first = std::get<1>(mem[0]);   // (Python's next(..., mem[0][1]) evaluates the default first)
        olf = first;
        for (const auto& [n, lf] : mem) {
            if (n.lower() == name) {
                olf = lf;
                break;
            }
        }
    }
    bool dynamic = olf && le::header_fields(*olf).at(py::str("module_flags")) == le::MODULE_DYNAMIC;
    py::str ext = (original && !original->empty() && le::members(*original).size() == 1)
                      ? std::get<1>(py::os::path::splitext(*original))
                      : py::str(".vxd");
    auto [path, log] = link(*obj, m->group(1), py::os::path::join(work, name + ext), dynamic);
    r.log = (r.log + py::str("\n") + log).strip();
    if (!path) {
        return r;
    }
    r.linked = true;
    r.path = path;
    if (olf) {
        try {
            std::tie(r.checks, r.notes, r.same) = compare(le::LEFile::from_file(*path), *olf);
        } catch (const py::Exception& e) {   // a file the reader cannot take apart
            r.notes = py::list<py::str>{py::percent("the rebuilt file could not be read: %s: %s", py::str(e.type()), py::str(e.what()))};
        }
    }
    return r;
}

py::str report(const BuildResult& r) {
    auto line_of = [](const py::str& log, int64_t at) {
        py::list<py::str> lines = py::splitlines(log);
        return lines.empty() ? py::str("?") : lines[at];
    };
    if (!r.assembled) {
        return py::percent("%-12s does not assemble: %s", r.name, line_of(r.log, 0).slice(0, 120));
    }
    if (!r.linked) {
        return py::percent("%-12s assembles, does not link: %s", r.name, line_of(r.log, -1).slice(0, 120));
    }
    if (r.checks.empty()) {
        return py::percent("%-12s built (%s)", r.name, !r.notes.empty() ? r.notes[0] : py::str("no original to check against"));
    }
    py::list<py::str> marks;
    for (const auto& [k, v] : r.checks.items()) {
        marks.append(py::percent("%s %s", k, v ? py::str("same") : py::str("DIFFERS")));
    }
    py::str same = r.same ? py::percent("  %.1f%% of paired bytes equal", 100 * *r.same) : py::str("");
    return py::percent("%-12s built  %s%s", r.name, py::join(py::str(" "), marks), same);
}

const py::str MAIN_DOC = py::str("python -m codebreaker driver-build <source folder or .asm> ... [--original FILE] [--out DIR]");

int64_t main(const py::list<py::str>& argv) {
    py::dict<py::str, std::optional<py::str>> opts;
    py::list<py::str> args;
    for (int64_t i = 0; i < argv.size(); ++i) {
        const py::str& a = argv[i];
        if (a == py::str("--original") || a == py::str("--out")) {
            opts.set(a.slice_from(2), i + 1 < argv.size() ? std::optional<py::str>(argv[i + 1]) : std::nullopt);
            ++i;
        } else {
            args.append(a);
        }
    }
    if (args.empty()) {
        print_(MAIN_DOC);
        return 2;
    }
    int64_t bad = 0;
    auto opt = [&](const char* key) -> std::optional<py::str> {
        auto v = opts.get(py::str(key));
        return v ? *v : std::nullopt;
    };
    for (const py::str& src : args) {
        BuildResult r;
        try {
            r = build(src, opt("original"), opt("out"));
        } catch (const py::ValueError& e) {
            print_(py::str(e.what()));
            ++bad;
            continue;
        }
        print_(report(r));
        for (const py::str& note : r.notes) {
            print_(py::str("    ") + note);
        }
        if (r.path) {
            print_(py::str("    -> ") + *r.path);
        }
        bool all = true;
        for (bool v : r.checks.values()) {
            all = all && v;
        }
        bad += !(r.linked && all);
    }
    return bad ? 1 : 0;
}

}  // namespace codebreaker::build::driver
