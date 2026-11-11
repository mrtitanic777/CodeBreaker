// codebreaker/exact/run.cpp -- typed C++ of codebreaker.exact.run (see run.hpp).

#include "codebreaker/exact/run.hpp"

#include <algorithm>
#include <map>

#include "codebreaker/exact/bench.hpp"
#include "codebreaker/exact/bss.hpp"
#include "codebreaker/exact/carve.hpp"
#include "codebreaker/exact/data.hpp"
#include "codebreaker/exact/draft/package.hpp"
#include "codebreaker/exact/identify.hpp"
#include "codebreaker/exact/ledger.hpp"
#include "codebreaker/exact/library.hpp"
#include "codebreaker/exact/link.hpp"
#include "codebreaker/exact/modules.hpp"
#include "codebreaker/exact/names.hpp"
#include "codebreaker/exact/omf.hpp"
#include "codebreaker/exact/readable.hpp"
#include "codebreaker/exact/repair.hpp"
#include "codebreaker/exact/seed.hpp"

namespace codebreaker::exact::run {

const py::str DOC = py::str(
    "run.py -- exact: a compiled program rebuilt byte for byte from its source (docs/EXACT.md).\n"
    "\n"
    "    python -m codebreaker exact PROJECT STAGE [options]\n"
    "    python -m codebreaker exact --toolchains             the toolchain profiles there are\n"
    "    python -m codebreaker exact --compile PROFILE[:VARIANT] FILE.c [switches]\n"
    "                                                          one file compiled, its functions listed\n"
    "\n"
    "PROJECT is a folder with a project.json (codebreaker/exact/project.py). Stages:\n"
    "\n"
    "    ledger      every one-function source compiled and put against the program (exact/ledger.py)\n"
    "    carve       where the program's own functions begin and end (exact/carve.py)\n"
    "    modules     each source file put together, compiled whole, its functions and data checked (exact/modules.py)\n"
    "    draft       functions written by the compiler's inverse model, kept when the compiler proves them (exact/draft/)\n"
    "    data        a module's initialized data: where it stands, definitions drafted (exact/data.py)\n"
    "    bss         the variables without a value at their places: src/BSS (exact/bss.py)\n"
    "    link        the program linked from its sources by the period linker, compared, diagnosed (exact/link.py)\n"
    "    library     the library modules found in the program, in the linker's order; stand-ins (exact/library.py)\n");

namespace {

using py::bytes;
using py::str;

std::map<std::string, Stage>& table() {
    static std::map<std::string, Stage> t = [] {
        std::map<std::string, Stage> m;
        m["ledger"] = [](project::Project& p, const py::list<str>& a, std::ostream&) { return ledger::main(*project::as_ledger(p), a); };
        m["library"] = [](project::Project& p, const py::list<str>& a, std::ostream& o) {
            return library::main(*project::as_library(p), a, [&o](const str& s) { o << s.utf8(); });     // (library::main hands each printed line with its line end)
        };
        m["carve"] = [](project::Project& p, const py::list<str>& a, std::ostream& o) { return carve::main(p, a, o); };
        m["modules"] = [](project::Project& p, const py::list<str>& a, std::ostream&) { return modules::main(*project::as_modules(p), a, project::decoder16()); };
        m["link"] = [](project::Project& p, const py::list<str>& a, std::ostream& o) { return link::main(p, a, o); };
        m["bss"] = [](project::Project& p, const py::list<str>& a, std::ostream& o) { return bss::main(p, a, o); };
        m["data"] = [](project::Project& p, const py::list<str>& a, std::ostream&) { return data::main(*project::as_data(p), a); };
        m["draft"] = [](project::Project& p, const py::list<str>& a, std::ostream&) { return draft::main(*project::as_draft(p), a); };
        m["repair"] = [](project::Project& p, const py::list<str>& a, std::ostream& o) { return repair::main(*project::as_ledger(p), a, o); };
        m["names"] = [](project::Project& p, const py::list<str>& a, std::ostream& o) { return names::main(p, a, o); };
        m["readable"] = [](project::Project& p, const py::list<str>& a, std::ostream&) { return readable::main(*project::as_readable(p), a); };
        m["seed"] = [](project::Project& p, const py::list<str>& a, std::ostream& o) { return seed::main(p, a, o); };
        m["identify"] = [](project::Project& p, const py::list<str>& a, std::ostream& o) { return identify::main(*project::as_identify(p), a, o); };
        m["bench"] = [](project::Project& p, const py::list<str>& a, std::ostream& o) { return bench::main(p, a, o); };
        return m;
    }();
    return t;
}

const py::list<str> NAMES = py::list<str>({str("ledger"), str("library"), str("carve"), str("modules"), str("link"), str("bss"), str("data"), str("draft"),
                                           str("repair"), str("names"), str("readable"), str("seed"), str("identify"), str("bench")});

str join(const str& sep, const py::list<str>& xs) {
    std::u32string out;
    for (int64_t i = 0; i < xs.size(); ++i) {
        if (i) out += sep.u32();
        out += xs[i].u32();
    }
    return str(out);
}

}  // namespace

const py::list<str>& stage_names() { return NAMES; }

void register_stage(const str& name, const Stage& fn) { table()[name.utf8()] = fn; }

int64_t main(const py::list<str>& argv, std::ostream& out) {
    auto say = [&](const str& s) { out << s.utf8() << "\n"; };
    if (argv.empty() || argv[0] == "-h" || argv[0] == "--help") {
        say(DOC);
        return !argv.empty() ? 0 : 2;
    }
    if (argv[0] == "--toolchains") {
        for (const str& n : toolchain::profiles()) {
            auto tc = toolchain::load(n);
            const toolchain::Profile& p = tc->p;
            py::list<str> vs = py::sorted(py::list<str>::from(p.variants.keys()));
            str title = p.strings.get(str("title"), str(""));
            str vjoin = join(str(", "), vs);
            say(py::percent(str("%-12s %s  variants: %s"), n, title, !vjoin.empty() ? vjoin : str("-")));
        }
        return 0;
    }
    if (argv[0] == "--compile") {
        auto [name, sep, variant] = argv[1].partition(str(":"));
        auto tc = toolchain::load(name, !variant.empty() ? std::optional<str>(variant) : std::nullopt);
        str d = py::os::path::dirname(py::os::path::abspath(argv[2]));            // (the headers beside the file)
        py::dict<str, bytes> hs;
        for (const str& f : py::os::listdir(d)) {
            if (f.lower().endswith(str(".h"))) {
                py::file h = py::open_rb(py::os::path::join(d, f));
                hs.set(f, h.read());
                h.close();
            }
        }
        py::file sf = py::open_rb(argv[2]);
        bytes src = sf.read();
        sf.close();
        str sw = join(str(" "), argv.slice(3, argv.size()));
        auto [obj, con] = tc->compile(src, hs, py::list<str>(), !sw.empty() ? std::optional<str>(sw) : std::nullopt);
        if (!obj) {
            say(con);
            return 1;
        }
        omf::Obj o = omf::read(*obj);
        for (const str& text : omf::code_segments(o)) {
            for (const auto& f : omf::functions(o, text)) {
                int64_t off = std::get<1>(f), ln = std::get<2>(f);
                bytes code = o.seg.at(text).slice(off, off + ln);
                std::string hx;
                for (int64_t i = 0; i < code.size(); ++i) {
                    static const char* dg = "0123456789abcdef";
                    hx += dg[code[i] >> 4];
                    hx += dg[code[i] & 15];
                }
                say(py::percent(str("%-10s %-24s %5x %5d  %s"), text, std::get<0>(f), off, ln, str(hx)));
            }
        }
        return 0;
    }
    auto prj = project::load(argv[0]);
    str stage = argv.size() > 1 ? argv[1] : str("ledger");
    if (!NAMES.contains(stage)) {
        say(py::percent(str("no stage %r (stages: %s)"), stage, join(str(", "), NAMES)));
        return 2;
    }
    return table().at(stage.utf8())(*prj, argv.slice(2, argv.size()), out);
}

}  // namespace codebreaker::exact::run
