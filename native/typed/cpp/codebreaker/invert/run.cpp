// codebreaker/invert/run.cpp -- C++17 port of codebreaker/invert/run.py (see run.hpp).
//
// Build with the msc6/msvc/model ports of invert/, native/invert's msc6*.cc and the ports a module build goes through
// (verify/lanes/win16, verify/authored/match, build/msc6, formats/ne, symbols/prototypes, base/corpus, base/commands ...).

#include "codebreaker/invert/run.hpp"

#include <algorithm>
#include <filesystem>
#include <mutex>

#include "codebreaker/base/capstone_dll.hpp"
#include "codebreaker/base/commands.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/msc6.hpp"
#include "codebreaker/formats/ne.hpp"
#include "codebreaker/invert/grammars.hpp"
#include "codebreaker/invert/msc6.hpp"
#include "codebreaker/invert/segment_thunk.hpp"
#include "codebreaker/invert/whole_function.hpp"
#include "codebreaker/symbols/prototypes.hpp"
#include "codebreaker/verify/authored/match.hpp"
#include "codebreaker/verify/lanes/win16.hpp"
#include "cs.h"
#include "flow16.h"
#include "json.hpp"
#include "msc6.h"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::invert::run {

namespace match = codebreaker::verify::authored::match;
namespace msc6c = codebreaker::build::msc6;

const py::str DOC = py::str(
    "run.py -- read a module's functions back into the source its compiler was given.\n"
    "\n"
    "    python -m codebreaker invert <16-bit module> [--first N] [--flags \"/AM /G2 /Gsw /Ox\"] [--show NAME]\n"
    "    python -m codebreaker invert --early FILE OFFSET LENGTH     one function of a program built with the\n"
    "                                                                C compiler of 1983-84 (offset in hex)\n"
    "    python -m codebreaker invert --check [MODEL]                each model against its own compiler\n"
    "    python -m codebreaker invert --tool NAME [arguments]        a model's own command line: %s\n"
    "\n"
    "The decompiler gives source that BEHAVES like a function. A compiler model gives the source\n"
    "that was WRITTEN, for the functions it covers: it knows which instructions the compiler emits\n"
    "for which statement, so it reads the statements off the instructions instead of searching for\n"
    "them (verify/authored/repair.py searches). What it says is then put to the compiler itself: the text is\n"
    "compiled by the real Microsoft C 6 and its bytes compared with the original's, outside the\n"
    "places a linker fills in.\n"
    "\n"
    "    recognised    the model read the whole function\n"
    "    identical     ... and the real compiler turns that text back into the function's bytes\n"
    "    declined      the function has something the model does not cover; it says nothing\n"
    "\n"
    "    assembly      a segment-switch thunk: generated code, written as the assembly that makes it\n"
    "                  (invert/segment_thunk.py rebuilds its bytes exactly)\n"
    "\n"
    "A model never guesses: a function it does not cover is declined, with the reason. Each function\n"
    "is compiled by the compiler whose frame it has (compiler_of): Microsoft C 6, or Microsoft C/C++ 8\n"
    "(settings: msvc152) for the modules Visual C++ 1.52c built.\n"
    "\n"
    "    from codebreaker.invert import run\n"
    "    run.module(path)      {name: (verdict, text or reason)}\n");

const py::list<py::str> FLAGS = {py::str("/AM /G2 /Gsw /Ox"), py::str("/AM /G2 /Gsw /Os"), py::str("/AM /G2 /Gsw"), py::str("/AM /G2 /Gsw /Ot")};
const py::list<py::str> FLAGS_MSVC152 = {py::str("/AM /G2 /Gsw"), py::str("/AM /G2 /Gsw /Od"), py::str("/AM /G2 /Gsw /Os"), py::str("/AM /G2 /Gsw /Ox")};

py::str compiler_of(const py::bytes& code) {
    if (code.slice(0, 2) == py::bytes::fromhex(py::str("8cd8")) || code.startswith(py::bytes::fromhex(py::str("558bec1e8ed8"))) ||
        code.contains(py::bytes::fromhex(py::str("8d66fe1f5d"))))
        return py::str("msvc152");
    return py::str("msc6");
}

const py::dict<py::str, py::str> TOOLS = {
    {py::str("early-c"), py::str("codebreaker.invert.early_c")},                       // FILE OFFSET LENGTH -> C
    {py::str("early-c-check"), py::str("codebreaker.invert.early_c_check")},           // ... and put the text to Microsoft C 2.03
    {py::str("early-c-frame"), py::str("codebreaker.invert.early_c_frame")},
    {py::str("early-c-round-trip"), py::str("codebreaker.invert.early_c_round_trip")},  // compile -> read -> compile, for locals
    {py::str("early-c-frame-probe"), py::str("codebreaker.invert.early_c_frame_probe")},   // how the compiler places locals
    {py::str("early-c-order-probe"), py::str("codebreaker.invert.early_c_order_probe")},   // how it orders loads and stores
    {py::str("early-c-forward"), py::str("codebreaker.invert.early_c_forward")},       // the forward model against a quad file and its object
    {py::str("quads"), py::str("codebreaker.invert.quad_dump")},                       // list a quad file
    {py::str("quad-align"), py::str("codebreaker.invert.quad_align")},                 // a quad file beside its object, per function
    {py::str("omf"), py::str("codebreaker.formats.omf")},                              // list an object file or library
    {py::str("msc6"), py::str("codebreaker.invert.msc6")},
    {py::str("msc6-templates"), py::str("codebreaker.invert.msc6_templates")},
    {py::str("msc6-function"), py::str("codebreaker.invert.msc6_function")},
    {py::str("msc6-registers"), py::str("codebreaker.invert.msc6_registers")},
    {py::str("msc6-walk"), py::str("codebreaker.invert.msc6_walk")},
    {py::str("msc6-idioms"), py::str("codebreaker.invert.msc6_idioms")},               // what each compiler switch changes
    {py::str("known-differences"), py::str("codebreaker.invert.known_differences")},
    {py::str("msvc2"), py::str("codebreaker.invert.msvc2")},
    {py::str("msvc2-forward"), py::str("codebreaker.invert.msvc2_forward")},
};
const py::list<py::str> CHECKS = {py::str("early-c-frame"), py::str("early-c-round-trip"), py::str("msc6-function"), py::str("msc6-registers"),
                                  py::str("msc6-walk"), py::str("msc6"), py::str("msvc2-forward"), py::str("msvc2")};

namespace {

py::list<py::str> sorted_names(const py::dict<py::str, py::str>& d) {
    std::vector<py::str> keys;
    for (const auto& kv : d.items()) keys.push_back(std::get<0>(kv));
    std::sort(keys.begin(), keys.end(), [](const py::str& a, const py::str& b) { return a.u32() < b.u32(); });
    return py::list<py::str>(keys);
}

}  // namespace

int64_t tool(const py::str& name, const py::list<py::str>& arguments) {
    if (!TOOLS.contains(name))
        throw py::SystemExit(py::percent(py::str("no model %r (have: %s)"), name, py::join(py::str(", "), sorted_names(TOOLS))));
    return base::commands::run_module(TOOLS.at(name), arguments);
}

const py::set<py::str> _LONG = {py::str("DWORD"), py::str("LONG"), py::str("ULONG"), py::str("long"), py::str("unsigned long"),
                                py::str("LPARAM"), py::str("LRESULT"), py::str("COLORREF"), py::str("FARPROC")};

py::str _kind(const py::str& c_type) {
    py::str t = c_type.replace(py::str("const "), py::str("")).strip();
    if (_LONG.contains(t)) return py::str("long");
    if (t.startswith(py::str("LP")) || t.contains(py::str("FAR")) || t.contains(py::str("far")) || t.endswith(py::str("*")) || t == py::str("HPSTR") ||
        t == py::str("PSTR"))
        return py::str("far");
    return py::str("int");
}

std::optional<std::tuple<py::str, py::list<std::tuple<py::str, py::str>>>> signature(const py::str& name) {
    auto p = symbols::prototypes::of(name, 16);
    if (!p) return std::nullopt;
    py::str returns = p->returns.strip();
    py::str k = _kind(returns);
    returns = returns == py::str("void") ? py::str("void") : (k == py::str("long") || k == py::str("far")) ? py::str("long") : py::str("int");
    py::list<std::tuple<py::str, py::str>> params;
    int64_t i = 0;
    for (const auto& tn : p->parameters.vec()) {
        const py::str& t = std::get<0>(tn);
        const py::str& n = std::get<1>(tn);
        if (!(t.strip() == py::str("void") || t.strip().empty()))
            params.append(std::make_tuple(n.empty() ? py::percent(py::str("p%d"), i) : n, _kind(t)));
        ++i;
    }
    return std::make_tuple(returns, params);
}

py::dict<std::tuple<int64_t, int64_t>, py::list<std::tuple<py::str, py::str>>> _segment_signatures(const py::str& module_path) {
    static std::once_flag once;
    std::call_once(once, [] {
        std::string err;
        std::wstring dll = std::filesystem::u8path(base::capstone_dll::find().utf8()).wstring();
        if (!cbinv::cs_load(dll.c_str(), err)) throw py::OSError(err);
    });
    formats::ne::NEFile nef(module_path);
    py::dict<std::tuple<int64_t, int64_t>, py::list<std::tuple<py::str, py::str>>> out;
    for (const formats::ne::Segment& s : nef.segments().vec()) {
        if (!s.length) continue;
        try {
            py::bytes segb = nef.data.slice(s.file_off, s.file_off + s.length);
            std::vector<uint8_t> raw(segb.raw().begin(), segb.raw().end());
            py::V sigs = py::json_parse(cbinv::msc6_segment_sigs(raw));
            for (const py::V& row : sigs.seq()) {
                py::list<std::tuple<py::str, py::str>> params;
                for (const py::V& p : row[1].seq()) params.append(std::make_tuple(py::str(p[0].str()), py::str(p[1].str())));
                out[std::make_tuple(s.idx, static_cast<int64_t>(row[0].as_int()))] = params;
            }
        } catch (const py::Exception&) {
            continue;
        } catch (const cbinv::PyError&) {
            continue;
        } catch (const std::exception&) {
            continue;
        }
    }
    return out;
}

py::dict<int64_t, std::tuple<py::str, py::str>> _globals(const py::bytes& code) {
    auto u16 = [&](int64_t i) { return static_cast<int64_t>(code[i]) | (static_cast<int64_t>(code[i + 1]) << 8); };
    py::dict<int64_t, std::tuple<py::str, py::str>> out;
    int64_t n = code.size();
    auto hexs = [](int64_t v) { return py::percent(py::str("%x"), v); };
    for (int64_t i = 0; i < std::max<int64_t>(0, n - 6); ++i) {
        if (code[i] == 0xA1 && code[i + 3] == 0x0B && code[i + 4] == 0x06 && u16(i + 1) == u16(i + 5) + 2)
            out[u16(i + 5)] = std::make_tuple(py::str("gL_") + hexs(u16(i + 5)), py::str("long"));
        if (code[i] == 0x2b && code[i + 1] == 0xc0 && code[i + 2] == 0xA3 && code[i + 5] == 0xA3 && i + 8 <= n)
            if (!out.contains(u16(i + 6))) out[u16(i + 6)] = std::make_tuple(py::str("gL_") + hexs(u16(i + 6)), py::str("long"));
    }
    for (int64_t i = 0; i < std::max<int64_t>(0, n - 3); ++i) {
        if ((code[i] == 0xc7 && code[i + 1] == 0x06) || (code[i] == 0xff && code[i + 1] == 0x36)) {
            int64_t off = u16(i + 2);
            if (!out.contains(off) && !out.contains(off - 2)) out[off] = std::make_tuple(py::str("gW_") + hexs(off), py::str("word"));
        }
    }
    return out;
}

py::dict<py::str, Result> module(const py::str& module_path, int64_t limit, const std::optional<py::str>& flags, const Log& log) {
    using Tries = std::tuple<int64_t, py::list<py::str>>;
    py::str name = py::os::path::basename(module_path).lower();
    py::str state = py::os::path::join(py::settings::get_str(py::str("state")), py::str("invert"), name);
    py::os::makedirs(state, 0777, true);   // (settings.state makes the folder the path is in)
    verify::lanes::win16::Prepared m = verify::lanes::win16::prepare(module_path, py::os::path::join(state, py::str("source")));
    py::dict<int64_t, py::str> by_va = m.by_va();
    match::Where16 where;
    for (const auto& kv : by_va.items()) {
        const auto& w = m.where.at(std::get<0>(kv));
        if (!w) throw py::TypeError("'NoneType' object is not subscriptable");
        where[std::get<0>(kv)] = *w;
    }
    formats::ne::NEFile nef(module_path);
    const auto& originals16 = std::get<match::Originals16>(match::ARCH().at(py::str("x86-16")).originals);
    py::dict<int64_t, match::ByteMasked> originals = originals16(module_path, where, verify::lanes::win16::_spans(nef, where));
    py::dict<py::str, Result> out;
    py::dict<py::str, Tries> texts;
    auto called = _segment_signatures(module_path);
    std::vector<std::tuple<int64_t, py::str>> items;
    for (const auto& kv : by_va.items()) items.emplace_back(std::get<0>(kv), std::get<1>(kv));
    std::sort(items.begin(), items.end(), [](const auto& a, const auto& b) { return std::get<0>(a) < std::get<0>(b); });
    for (const auto& vf : items) {
        int64_t va = std::get<0>(vf);
        const py::str& fn = std::get<1>(vf);
        if (!originals.contains(va) || (limit && out.size() >= limit)) continue;
        py::bytes code = std::get<0>(originals.at(va));
        std::optional<segment_thunk::Thunk> thunk = segment_thunk::verify(code);
        if (thunk) {
            out[fn] = Result(py::str("assembly"), segment_thunk::emit_asm(*thunk, fn), std::nullopt);
            continue;
        }
        py::list<py::str> tries;
        try {   // a function that is one known shape
            for (const auto& c : whole_function::candidates(code).vec()) tries.append(std::get<1>(c));
        } catch (const py::Exception&) {
        }
        auto sdk = signature(fn);
        py::str returns("int");
        std::optional<py::list<std::tuple<py::str, py::str>>> params;
        const auto& seg_off = m.where.at(va);
        if (sdk) {
            returns = std::get<0>(*sdk);
            params = std::get<1>(*sdk);
        } else if (called.contains(*seg_off)) {
            params = called.at(*seg_off);
        }
        py::str why;
        std::vector<std::optional<py::list<std::tuple<py::str, py::str>>>> attempts;
        if (params && !params->empty()) attempts = {params, std::nullopt};
        else attempts = {std::nullopt};
        for (const auto& p : attempts) {   // the parameters known, then the model's own reading
            std::optional<py::str> text;
            try {
                msc6::Inverted r = msc6::invert(code, py::dict<int64_t, py::str>(), _globals(code), returns, p, std::nullopt, std::get<1>(*seg_off));
                text = r.text;
                why = r.why;
            } catch (const py::Exception& e) {   // a model that trips is a model that declines
                text = std::nullopt;
                why = py::str("the model failed: ") + py::str(e.type());
            }
            if (text && !tries.contains(*text)) tries.append(*text);
        }
        if (tries.empty()) {
            if (code.contains(py::bytes::fromhex(py::str("9a")))) {   // shapes the model does not read, by search
                py::dict<py::str, int64_t> ctx{{py::str("nargs"), std::max<int64_t>(0, params ? params->size() : 0)}};
                for (grammars::Grammar g : grammars::GRAMMARS.vec())
                    for (const py::str& t : g(ctx).vec()) tries.append(t);
            }
            if (!tries.empty()) {
                texts[fn] = Tries(va, tries);
                out[fn] = Result(py::str("declined"), why, std::nullopt);
                continue;
            }
        }
        if (tries.empty()) {
            out[fn] = Result(py::str("declined"), why, std::nullopt);
        } else {
            out[fn] = Result(py::str("recognised"), tries[-1], std::nullopt);
            texts[fn] = Tries(va, tries);
        }
    }
    // by compiler, in the order the compilers sort
    std::map<std::string, py::dict<py::str, Tries>> by_compiler;
    for (const auto& kv : texts.items()) by_compiler[compiler_of(std::get<0>(originals.at(std::get<0>(std::get<1>(kv))))).utf8()][std::get<0>(kv)] = std::get<1>(kv);
    for (auto& wg : by_compiler) {
        py::str which(wg.first);
        py::dict<py::str, Tries>& group = wg.second;
        if (!msc6c::available(which)) {
            if (log) log(py::percent(py::str("  %d functions are %s's; it is not set up (settings.json: %s)"), group.size(), which, which));
            continue;
        }
        py::str work = py::os::path::join(state, py::str("work"));
        py::dict<py::str, Tries> left;   // dict(group): a copy (a py::dict is shared by assignment)
        for (const auto& kv : group.items()) left[std::get<0>(kv)] = std::get<1>(kv);
        py::list<py::str> flag_sets = flags ? py::list<py::str>{*flags} : which == py::str("msvc152") ? FLAGS_MSVC152 : FLAGS;
        for (const py::str& fl : flag_sets.vec()) {
            py::dict<msc6c::Key, py::str> sources;
            for (const auto& fv : left.items())
                for (int64_t k = 0; k < std::get<1>(std::get<1>(fv)).size(); ++k)
                    sources[msc6c::Key(std::make_tuple(std::variant<py::str, int64_t>(std::get<0>(fv)), k))] = std::get<1>(std::get<1>(fv))[k];
            auto built = msc6c::compile_many(sources, py::str(""), fl, work, 900, which);
            std::vector<py::str> names;
            for (const auto& fv : left.items()) names.push_back(std::get<0>(fv));
            for (const py::str& fn : names) {
                const Tries& vt = left.at(fn);
                for (int64_t k = 0; k < std::get<1>(vt).size(); ++k) {
                    msc6c::Key key(std::make_tuple(std::variant<py::str, int64_t>(fn), k));
                    if (!built.contains(key) || !built.at(key)) continue;
                    const auto& orig = originals.at(std::get<0>(vt));
                    auto d = match::distance(match::Masked(std::get<0>(*built.at(key)), match::Mask(std::get<1>(*built.at(key)))),
                                             match::Masked(std::get<0>(orig), match::Mask(std::get<1>(orig))));
                    if (std::get<0>(d) == py::str("identical")) {
                        out[fn] = Result(py::str("identical"), std::get<1>(vt)[k], fl);
                        left.pop(fn);
                        break;
                    }
                }
            }
            if (log) log(py::percent(py::str("  %-8s %-20s %d of %d now identical"), which, fl, group.size() - left.size(), group.size()));
            if (left.empty()) break;
        }
    }
    return out;
}

int64_t main(const py::list<py::str>& argv) {
    auto index_of = [&](const char* flag) -> int64_t {
        for (int64_t i = 0; i < argv.size(); ++i)
            if (argv[i] == py::str(flag)) return i;
        return -1;
    };
    std::optional<py::str> flags;
    int64_t i = index_of("--flags");
    if (i >= 0) flags = argv[i + 1];
    int64_t limit = 0;
    i = index_of("--first");
    if (i >= 0) limit = py::int_parse(argv[i + 1]);
    std::optional<py::str> show;
    i = index_of("--show");
    if (i >= 0) show = argv[i + 1];
    i = index_of("--tool");
    if (i >= 0) {
        py::list<py::str> rest;
        for (int64_t k = i + 2; k < argv.size(); ++k) rest.append(argv[k]);
        return tool(i + 1 < argv.size() ? argv[i + 1] : py::str(""), rest);
    }
    i = index_of("--early");
    if (i >= 0) {
        py::list<py::str> rest;
        for (int64_t k = i + 1; k < argv.size(); ++k) rest.append(argv[k]);
        return tool(py::str("early-c"), rest);
    }
    if (index_of("--check") >= 0) {
        py::list<py::str> named;
        for (const py::str& a : argv.vec())
            if (!a.startswith(py::str("--"))) named.append(a);
        int64_t worst = 0;
        for (const py::str& name : (named.empty() ? CHECKS : named).vec()) {
            py::print(py::percent(py::str("== %s"), name));
            worst = std::max(worst, tool(name, py::list<py::str>()));
        }
        return worst;
    }
    py::list<py::str> args;
    for (const py::str& a : argv.vec()) {
        if (a.startswith(py::str("--"))) continue;
        if (flags && a == *flags) continue;
        if (a == py::str(std::to_string(limit))) continue;
        if (show && a == *show) continue;
        args.append(a);
    }
    if (args.empty()) {
        py::print(py::percent(DOC, py::join(py::str(", "), sorted_names(TOOLS))));
        return 2;
    }
    py::str path = args[0];
    if (!py::os::path::isfile(path)) {
        auto r = base::corpus::retail(args[0]);
        if (!r) throw py::TypeError("expected str, bytes or os.PathLike object, not NoneType");
        path = *r;
    }
    auto results = module(path, limit, flags, [](const py::str& s) { py::print(s); });
    if (show) {
        if (results.contains(*show)) py::print(std::get<1>(results.at(*show)));
        else py::print(py::percent(py::str("no function %r"), *show));
        return 0;
    }
    py::dict<py::str, int64_t> counts;
    py::dict<py::str, int64_t> reasons;
    for (const auto& kv : results.items()) {
        const Result& r = std::get<1>(kv);
        counts[std::get<0>(r)] = counts.contains(std::get<0>(r)) ? counts.at(std::get<0>(r)) + 1 : 1;
        if (std::get<0>(r) == py::str("declined")) {
            py::str why = py::split(std::get<1>(r), py::str(" @"))[0].slice(0, 60);
            reasons[why] = reasons.contains(why) ? reasons.at(why) + 1 : 1;
        }
    }
    py::list<py::str> parts;
    for (const char* k : {"identical", "recognised", "declined"})
        if (counts.contains(py::str(k)) && counts.at(py::str(k))) parts.append(py::percent(py::str("%s %d"), py::str(k), counts.at(py::str(k))));
    py::print(py::percent(py::str("%s: %d functions: %s"), py::os::path::basename(path), results.size(), py::join(py::str(", "), parts)));
    std::vector<std::tuple<py::str, int64_t>> rs;
    for (const auto& kv : reasons.items()) rs.emplace_back(std::get<0>(kv), std::get<1>(kv));
    std::stable_sort(rs.begin(), rs.end(), [](const auto& a, const auto& b) { return std::get<1>(a) > std::get<1>(b); });
    for (size_t k = 0; k < rs.size() && k < 8; ++k)
        py::print(py::percent(py::str("  declined %4d  %s"), std::get<1>(rs[k]), std::get<0>(rs[k])));
    return 0;
}

}  // namespace codebreaker::invert::run
