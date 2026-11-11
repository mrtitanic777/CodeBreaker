// codebreaker/exact/bss.cpp -- typed C++ of codebreaker.exact.bss (see bss.hpp).

#include "codebreaker/exact/bss.hpp"

#include <algorithm>

namespace codebreaker::exact::bss {

namespace {

using py::bytes;
using py::str;
namespace re = py::re;

// glob.glob(src/PREFIXorNothing*EXT) on Windows: the names are matched without regard to case; a name that begins with a dot is
// not matched by a leading '*'
py::list<str> glob_files(const str& dir, const std::optional<char32_t>& first, const py::list<str>& exts) {
    py::list<str> out;
    for (const str& f : py::sorted(py::os::listdir(dir))) {
        str lf = f.lower();
        if (first) {
            if (lf.empty() || lf.u32()[0] != *first) continue;
        } else if (f.startswith(str("."))) {
            continue;
        }
        bool ok = false;
        for (const str& e : exts) ok = ok || (lf.endswith(e) && lf.size() >= 1 + e.size());
        if (ok) out.append(py::os::path::join(dir, f));
    }
    return out;
}

str read_latin1(const str& p) {
    py::file f = py::open(p, str("r"), str("latin1"));
    str t = f.read_text();
    f.close();
    return t;
}

}  // namespace

Proposal propose(project::Project& prj) {
    const bytes& img = prj.image;
    const int64_t dg = prj.dgroup;
    const py::list<int64_t>& h = prj.header;
    int64_t edata = img.size() - dg;
    int64_t stack = !h.empty() ? h[7] * 16 - dg : edata;
    if (prj.packed) {                                       // (an image unpacked from EXEPACK holds its variables as zeros: the packed file says where its data ended, and where its stack is)
        edata = prj.packed->dest * 16 - dg;
        stack = prj.packed->ss * 16 - dg;
    }
    library::MapP mp = library::locate(*project::as_library(prj));
    py::set<str> lib_defined;
    for (const auto* grp : {&mp->coded, &mp->donly, &mp->first}) {
        for (const library::PlacedP& p : *grp) {
            py::list<library::MemberP> ms;
            ms.append(p->m);
            for (const auto& a : p->also) ms.append(a);
            for (const library::MemberP& m : ms) {
                for (const str& n : m->pubs.keys()) lib_defined.add(n);
            }
        }
    }
    py::set<str> linker;
    const auto& lk = project::profile_parts(*prj.toolchain()).link;
    if (lk) {
        for (const str& n : lk->names) linker.add(n);
    } else {
        linker.add(str("_edata"));
        linker.add(str("_end"));
    }
    py::set<str> funcs;
    for (const str& f : glob_files(prj.src, std::optional<char32_t>(U'f'), py::list<str>({str(".c")}))) funcs.add(str("_") + py::os::path::basename(f).slice(0, -2));
    auto skip = [&](const str& n) {
        return lib_defined.contains(n) || linker.contains(n) || funcs.contains(n) || n.contains(str(":")) || n.contains(str("@"));
    };
    auto syms = prj.symbols();
    py::set<str> words;
    for (const str& f : glob_files(prj.src, std::nullopt, py::list<str>({str(".c"), str(".h")}))) {
        for (const str& w : re::findall(str("\\w+"), read_latin1(f))) words.add(w);
    }
    py::set<str> stripped;                                  // ({w.lstrip("_") for w in words})
    for (const str& w : words) stripped.add(w.lstrip(str("_")));
    py::dict<str, int64_t> names;
    for (const auto& kv : syms.items()) {                   // (a name no source uses any more: not the program's)
        const str& n = std::get<0>(kv);
        const str& t = std::get<0>(std::get<1>(kv));
        int64_t a = std::get<1>(std::get<1>(kv));
        if (t == "D" && edata <= a && a < stack && !skip(n) && (words.contains(n.lstrip(str("_"))) || stripped.contains(n.lstrip(str("_"))))) names.set(n, a);
    }
    py::set<str> defined;
    py::set<str> used;
    for (const str& f : glob_files(prj.src, std::nullopt, py::list<str>({str(".c")}))) {
        str text = read_latin1(f);
        str b = py::os::path::basename(f);
        if (re::fullmatch(str("[de][0-9a-f]{5}\\.c"), b) || b.endswith(str("-data.c"))) {
            for (const str& g : re::findall(str("(?m)^(?:static )?(?:unsigned |struct \\w+ |union \\w+ )?\\w+ \\**(g_[0-9a-f]{4})\\b"), text)) defined.add(g);
        }
        for (const str& g : re::findall(str("\\bg_([0-9a-f]{4})\\b"), text)) used.add(g);
    }
    for (const str& x : py::sorted(py::list<str>::from(used))) {
        str n = str("_g_") + x;
        if (!defined.contains(str("g_") + x) && !names.contains(n) && !skip(n) && !syms.contains(n) && edata <= py::int_parse(x, 16) && py::int_parse(x, 16) < stack) {
            names.set(n, py::int_parse(x, 16));
        }
    }
    // the library's communal names, by the words of its code and data that name them
    for (const auto& kv : mp->allrefs.items()) {
        const str& n = std::get<0>(kv);
        const str& t = std::get<0>(std::get<1>(kv));
        int64_t a = std::get<1>(std::get<1>(kv));
        if (t == "D" && edata <= a && a < stack && !names.contains(n) && !skip(n)) names.set(n, a);
    }
    // the library modules' own _BSS: where their code points into it
    std::vector<std::tuple<int64_t, int64_t, str>> holes;
    for (const library::PlacedP& p : mp->coded) {
        int64_t ln = std::get<0>(p->m->seglen(str("_BSS")));
        if (!ln) continue;
        const str s0 = std::get<0>(p->m->code[0]);
        const bytes code = p->m->o.seg.at(s0);
        std::optional<int64_t> at;
        for (const auto& fx : p->m->o.fix) {
            const str& seg = std::get<0>(fx);
            int64_t fo = std::get<1>(fx);
            const str& kind = std::get<2>(fx);
            const auto& tgt = std::get<3>(fx);
            int64_t disp = std::get<4>(fx);
            bool seg_bss = std::get<0>(tgt) == "seg" && std::holds_alternative<str>(std::get<1>(tgt)) && std::get<str>(std::get<1>(tgt)) == "_BSS";
            if (seg == s0 && kind == "off" && seg_bss && fo + 2 <= code.size()) {
                at = (library::W(img, *p->code + fo) - ((library::W(code, fo) + disp) & 0xFFFF)) & 0xFFFF;
                break;
            }
        }
        if (at) holes.emplace_back(*at, ln, p->m->label());
    }
    std::sort(holes.begin(), holes.end());
    py::list<Hole> merged;
    for (const auto& hl : holes) {
        int64_t a = std::get<0>(hl), ln = std::get<1>(hl);
        if (!merged.empty() && a <= std::get<0>(merged[-1]) + std::get<1>(merged[-1]) + 1) {
            auto [pa, pl, pn] = merged[-1];
            py::list<str> nn = pn.copy();
            nn.append(std::get<2>(hl));
            merged[-1] = std::make_tuple(pa, std::max(pl, a + ln - pa), nn);
        } else {
            py::list<str> nn;
            nn.append(std::get<2>(hl));
            merged.append(std::make_tuple(a, ln, nn));
        }
    }
    Proposal r;
    const auto& comm = project::profile_parts(*prj.toolchain()).communal;     // (a compiler whose communal names go to a segment of their own after every module's _BSS: c_common)
    if (comm) {
        int64_t lo = edata;                                 // (behind the library's _BSS: an unpacked image can hold its variables as zeros, so not the image's end)
        if (!merged.empty()) {
            lo = std::get<0>(merged[0]) + std::get<1>(merged[0]);
            for (const auto& m : merged) lo = std::max(lo, std::get<0>(m) + std::get<1>(m));
        }
        if (comm->para) lo = py::floordiv(dg + lo + 15, 16) * 16 - dg;
        py::dict<str, int64_t> kept;
        for (const auto& kv : names.items()) {
            if (std::get<1>(kv) >= lo) kept.set(std::get<0>(kv), std::get<1>(kv));
        }
        r.edata = lo;
        r.stack = stack;
        r.names = kept;
        return r;
    }
    r.edata = edata;
    r.stack = stack;
    r.names = names;
    r.holes = merged;
    return r;
}

py::list<str> lines(project::Project& prj, int64_t edata, int64_t stack, const py::dict<str, int64_t>& names, const py::list<Hole>& holes) {
    py::list<str> out;
    out.append(str("# the variables that begin without a value (codebreaker exact bss): their range; a library module's"));
    out.append(str("# own _BSS between them (hole: its place and length); then a name and its DS offset each (hex)"));
    out.append(py::percent(str("range %04x %04x"), edata, stack));
    const auto& comm = project::profile_parts(*prj.toolchain()).communal;
    if (comm) {
        out.append(py::percent(str("segment %s%s     # (the compiler's segment of communal names: after the library)"), comm->segment,
                               comm->para ? str(" para") : str("")));
    }
    for (const auto& hl : holes) {
        std::u32string nms;
        const auto& l = std::get<2>(hl);
        for (int64_t i = 0; i < l.size(); ++i) {
            if (i) nms += U", ";
            nms += l[i].u32();
        }
        out.append(py::percent(str("hole %04x %x     # the _BSS of the library's %s"), std::get<0>(hl), std::get<1>(hl), str(nms)));
    }
    auto items = names.items();
    std::sort(items.begin(), items.end(), [](const auto& a, const auto& b) {
        return std::make_tuple(std::get<1>(a), std::get<0>(a)) < std::make_tuple(std::get<1>(b), std::get<0>(b));
    });
    for (const auto& kv : items) out.append(py::percent(str("%s %04x"), std::get<0>(kv), std::get<1>(kv)));
    return out;
}

int64_t main(project::Project& prj, const py::list<str>& argv, std::ostream& out) {
    Proposal p = propose(prj);
    py::list<str> ls = lines(prj, p.edata, p.stack, p.names, p.holes);
    std::u32string txt;
    for (const str& l : ls) {
        txt += l.u32();
        txt += U'\n';
    }
    out << str(txt).utf8() << "\n";
    if (argv.contains(str("--write"))) {
        py::file f = py::open(prj.path(str("BSS")), str("w"), std::nullopt, std::nullopt, str("\n"));
        f.write(str(txt));
        f.close();
    }
    return 0;
}

}  // namespace codebreaker::exact::bss
