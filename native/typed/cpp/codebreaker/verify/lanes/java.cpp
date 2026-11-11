// codebreaker/verify/lanes/java.cpp -- typed C++ port of codebreaker.verify.lanes.java (see java.hpp).

#include "codebreaker/verify/lanes/java.hpp"

#include "codebreaker/base/cache.hpp"
#include "codebreaker/decompile/jar.hpp"
#include "codebreaker/java/parse.hpp"
#include "codebreaker/java/textutil.hpp"
#include "codebreaker/verify/verdicts.hpp"
#include "py_settings.hpp"
#include "py_subprocess.hpp"

namespace codebreaker::verify::lanes::java {

namespace {

using py::str;
namespace V = codebreaker::verify::verdicts;
namespace cj = codebreaker::java;

str S(const char* s) { return str(s); }

const py::re::Pattern& error_pattern_() {
    static const py::re::Pattern p = py::re::compile(S("\\berror\\b"));
    return p;
}

str read_(const str& path) { return cj::textutil::read_text(path, S("replace"), false); }

}  // namespace

const str KIND = S("java");
const str SOURCE_REV = py::percent(S("java-%d"), REV);
const str STD = S("c++03");
const str CPP_STD = S("C++03");
const std::optional<str> PLATFORM = std::nullopt;
const py::list<str> FLAGS = {S("-std=") + STD, S("-fsyntax-only"), S("-w")};
const str COMPILES = S("compiles (%s %s); not run: %s");

bool is_module(const str& path) {
    if (py::os::path::isfile(path)) return path.lower().endswith(S(".java"));
    if (!py::os::path::isdir(path)) return false;
    for (const auto& w : py::os::walk(path)) {
        for (const str& f : std::get<2>(w))
            if (f.endswith(S(".java"))) return true;
    }
    return false;
}

str source_dir(const str& path) { return py::os::path::isfile(path) ? py::os::path::dirname(py::os::path::abspath(path)) : path; }

str sha(const str& path) {
    py::hashlib::Hash h = py::hashlib::sha256();
    str root = source_dir(path);
    for (const str& p : cj::transpile::discover(root)) {
        h.update(py::bytes(py::os::path::relpath(p, root).replace(S("\\"), S("/")).encode("utf-8").raw() + std::string(1, '\0')));
        {
            py::file f = py::open(p, S("rb"));
            h.update(f.read());
        }
        h.update(py::bytes(std::string(1, '\0')));
    }
    return h.hexdigest();
}

std::tuple<str, str> jdk() {
    std::optional<str> found = codebreaker::decompile::jar::find_java();
    str java = found ? *found : str();
    str javac;
    if (!java.empty()) {
        str sibling = py::os::path::join(py::os::path::dirname(java), S("javac") + S(".exe"));
        if (py::os::path::isfile(sibling)) {
            javac = sibling;
        } else {
            std::optional<str> w = py::shutil::which(S("javac"));
            javac = w ? *w : str();
        }
    }
    if (!javac.empty()) return {javac, java};
    return {str(), str()};
}

str compiler() {
    str cxx = py::settings::get_str(S("host_cxx"));
    if (!cxx.empty() && py::os::path::isfile(cxx)) return cxx;
    std::optional<str> w = py::shutil::which(S("g++"));
    return w ? *w : str();
}

std::tuple<bool, py::list<str>> check(const str& cxx, const str& include_dir, const str& unit, const std::optional<str>& prefix) {
    // -iquote, not -I: the emitted tree has headers named like the C library's (Math.h, Locale.h) and on a
    // case-insensitive disk -I would put them in front of <math.h> and <locale.h>
    py::list<str> cmd{cxx};
    cmd.extend(FLAGS);
    cmd.extend(py::list<str>{S("-iquote"), include_dir});
    if (prefix && !prefix->empty()) {
        cmd.append(S("-include"));
        cmd.append(*prefix);
    }
    cmd.append(unit);
    py::subprocess::RunOptions o;
    o.stdout_ = py::subprocess::Stream::Devnull;
    o.stderr_ = py::subprocess::Stream::Capture;
    o.text = false;
    auto r = py::subprocess::run(cmd, o);
    str err = r.stderr_bytes ? r.stderr_bytes->decode("utf-8", "replace") : str();
    py::list<str> lines;
    for (const str& ln : py::splitlines(err))
        if (error_pattern_().search(ln)) lines.append(ln);
    return {r.returncode == 0 && !py::truthy(lines), lines};
}

std::tuple<str, str> profile(const std::optional<str>&) {
    str cxx = compiler();
    if (cxx.empty()) throw py::RuntimeError(S("no C++ compiler to judge Java with (settings.json: host_cxx, or g++ on PATH)"));
    auto jd = jdk();
    const str& javac = std::get<0>(jd);
    const str& java = std::get<1>(jd);
    py::list<str> libs;
    for (const str& j : cj::probe::libraries()) libs.append(py::os::path::basename(j));   // what the game is compiled against
    // repr(("java", REV, cache.tool(cxx), FLAGS, PLATFORM, probe.REV, cache.tool(javac) if javac else "",
    //       cache.tool(java) if java else "") + ((libs,) if libs else ()))
    auto tuple_repr = [](const py::list<str>& xs) {
        py::list<str> parts;
        for (const str& x : xs) parts.append(py::repr(x));
        return S("(") + py::join(S(", "), parts) + (xs.size() == 1 ? S(",") : S("")) + S(")");
    };
    py::list<str> items{py::repr(S("java")),
                        py::to_str(REV),
                        py::repr(codebreaker::base::cache::tool(cxx)),
                        tuple_repr(FLAGS),
                        S("None"),
                        py::to_str(cj::probe::REV),
                        py::repr(!javac.empty() ? codebreaker::base::cache::tool(javac) : str()),
                        py::repr(!java.empty() ? codebreaker::base::cache::tool(java) : str())};
    if (py::truthy(libs)) items.append(tuple_repr(libs));
    str settings = S("(") + py::join(S(", "), items) + S(")");
    str digest = py::hashlib::sha1(py::bytes(settings.encode("utf-8").raw())).hexdigest().slice(0, 12);
    return {digest, settings};
}

std::optional<str> precompile(const str& cxx, const str& include_dir, const str& work) {
    if (!py::os::path::basename(cxx).lower().contains(S("g++"))) return std::nullopt;
    str folder = py::os::path::join(work, S("pch"));
    py::os::makedirs(folder, 0777, true);
    str head = py::os::path::join(folder, S("j2x_pch.h"));
    cj::textutil::write_text(head, S("#include \"stdafx.h\"\n#include \"JavaRT.h\"\n"));
    py::list<str> cmd{cxx};
    for (const str& x : FLAGS)
        if (x != S("-fsyntax-only")) cmd.append(x);
    cmd.extend(py::list<str>{S("-iquote"), include_dir, S("-x"), S("c++-header"), head, S("-o"), head + S(".gch")});
    py::subprocess::RunOptions o;
    o.stdout_ = py::subprocess::Stream::Devnull;
    o.stderr_ = py::subprocess::Stream::Devnull;
    auto r = py::subprocess::run(cmd, o);
    if (r.returncode == 0 && py::os::path::isfile(head + S(".gch"))) return head;
    return std::nullopt;
}

Prepared prepare(const str& module_path, const str& work) {
    if (module_path.lower().endswith(S(".jar"))) {
        throw py::SystemExit(py::percent(S("%s: a jar -- decompile it to Java source first (codebreaker java decompile)"),
                                         py::os::path::basename(module_path)));
    }
    str src = source_dir(module_path);
    str out = py::os::path::join(work, S("cpp"));
    for (const char* stale_name : {"probe", "javasrc"}) {   // the last run's objects and archive are of other C++
        str stale = py::os::path::join(work, S(stale_name));
        if (py::os::path::isdir(stale)) py::shutil::rmtree(stale);
    }
    if (py::os::path::isdir(out)) py::shutil::rmtree(out);
    Prepared m;
    m.out = out;
    m.summary = cj::transpile::convert(src, out, std::nullopt, nullptr, false, CPP_STD, PLATFORM);
    m.src = src;
    py::list<str> discovered = cj::transpile::discover(src);
    for (const str& p : discovered) m.java.set(std::get<0>(py::os::path::splitext(py::os::path::basename(p))), p);
    py::set<str> marker_set = py::set<str>::from(m.summary.markers);   // (the dropped empty markers)
    py::list<str> classes;
    for (const str& c : py::sorted(m.java.keys()))
        if (!marker_set.contains(c)) classes.append(c);
    py::set<str> emitted = py::set<str>::from(py::os::listdir(out));
    for (const str& c : classes) {
        py::list<str> parts;
        for (const char* ext : {".h", ".cpp"}) {
            if (emitted.contains(c + str(ext))) parts.append(read_(py::os::path::join(out, c + str(ext))));
        }
        if (py::truthy(parts)) m.texts.set(c, py::join(S("\n"), parts));
    }
    py::list<str> support;
    for (const str& f : py::sorted(emitted))
        if (f.endswith(S(".h")) && !m.texts.contains(std::get<0>(py::os::path::splitext(f)))) support.append(f);
    str header;
    for (const str& f : support) header += py::percent(S("// ---- %s\n%s\n"), f, read_(py::os::path::join(out, f)));
    m.header = header;
    m.order = py::list<str>::from(py::sorted(m.texts.keys()));
    for (const str& c : m.order) m.key.set(c, c);
    m.all_keys = py::list<str>::from(py::sorted(classes));
    m.sig = py::hashlib::sha1(py::bytes((py::percent(S("%d"), REV) + str(char32_t(0)) + m.header).encode("utf-8", "replace").raw())).hexdigest();
    return m;
}

Built build(const Prepared& m, const str&, const str& work) {
    str cxx = compiler();
    str units = py::os::path::join(work, S("units"));
    py::os::makedirs(units, 0777, true);
    py::dict<str, Item> items;
    for (const str& c : m.order) {
        str unit = py::os::path::join(m.out, c + S(".cpp"));
        if (!py::os::path::isfile(unit)) {
            unit = py::os::path::join(units, c + S(".cpp"));
            cj::textutil::write_text(unit, py::percent(S("#include \"stdafx.h\"\n#include \"%s.h\"\n"), c));
        }
        items.set(c, Item{c, unit, m.java.get(c, S(""))});
    }
    auto jd = jdk();
    str javac = std::get<0>(jd), java = std::get<1>(jd);
    str staged;
    if (!javac.empty()) {
        staged = cj::probe::stage_sources(m.src, py::os::path::join(work, S("javasrc")));
    }
    std::optional<str> prefix = precompile(cxx, m.out, work);
    cj::probe::Compiled compiled;
    if (!javac.empty()) {
        cj::probe::library(cxx, m.out, work, prefix);   // the whole program, once, before the judges link to it
        // every class's Java harness, compiled in batches here rather than one javac per class there
        compiled = cj::probe::compile_java(staged, cj::probe::plan(staged, m.out, py::list<str>::from(items.keys())), work, javac);
    }
    return Built{Context{cxx, m.out, prefix, staged, javac, java, work, compiled}, items, py::dict<str, str>()};
}

Verdict judge(const Context& context, const Item& item) {
    const str& unit = item.unit;
    const str& source = item.source;
    auto ck = check(context.cxx, context.include_dir, unit, context.prefix);
    if (!std::get<0>(ck)) {
        const py::list<str>& errors = std::get<1>(ck);
        str first = py::truthy(errors) ? errors[0] : S("the compiler rejected it");
        return Verdict{V::NO_COMPILE, first.slice(0, 200), py::list<str>(), py::join(S("\n"), errors.slice(0, 20))};
    }
    str how0 = py::os::path::basename(context.cxx), how1 = py::join(S(" "), FLAGS);
    auto compiles = [&](const str& why) { return py::percent(COMPILES, how0, how1, why); };
    if (context.javac.empty() || source.empty()) {
        return Verdict{V::INCONCLUSIVE, S("not-run"), py::list<str>(), compiles(S("no JDK (settings.json: java)"))};
    }
    py::ref<cj::ir::JClass> cls;
    try {
        cls = cj::parse::parse_file(source);
    } catch (const py::Exception& e) {   // the transpiler parsed it; say so if this did not
        return Verdict{V::INCONCLUSIVE, S("not-run"), py::list<str>(),
                       compiles(py::percent(S("could not read %s: %s"), source, str(std::string(e.what()))))};
    }
    cj::probe::RunResult r = cj::probe::run(context.staged, context.include_dir, cls, context.work, context.cxx, context.javac, context.java,
                                            context.prefix, context.compiled);
    if (!r.judged) {
        return Verdict{V::INCONCLUSIVE, S("not-run"), py::list<str>(), compiles(r.why)};
    }
    str said = py::percent(S("%d of %d methods run beside the JVM on %d inputs"), r.judged, r.total, r.cases);
    if (py::truthy(r.differs)) {
        py::list<str> lines;
        int64_t n = 0;
        for (const auto& d : r.differs) {
            if (n++ >= 12) break;
            lines.append(py::percent(S("%s(%s): Java %s, C++ %s"), std::get<0>(d), py::join(S(", "), std::get<1>(d)), std::get<2>(d), std::get<3>(d)));
        }
        return Verdict{V::DIVERGENT, S("result-differs"), py::list<str>(),
                       said + py::percent(S("; %d differ:\n"), r.differs.size()) + py::join(S("\n"), lines)};
    }
    py::list<str> limits;
    if (r.judged < r.total) limits.append(S("methods-partial"));
    return Verdict{V::EQUIVALENT, S(""), limits, said + S(": every result alike")};
}

}  // namespace codebreaker::verify::lanes::java
