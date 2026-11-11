// codebreaker/exact/link.cpp -- typed C++ of codebreaker.exact.link (see link.hpp).

#include "codebreaker/exact/link.hpp"

#include <algorithm>

#include "codebreaker/exact/carve.hpp"
#include "codebreaker/exact/emu/dos.hpp"
#include "codebreaker/exact/library.hpp"
#include "codebreaker/exact/linkwhole.hpp"
#include "codebreaker/exact/modules.hpp"
#include "codebreaker/exact/omf.hpp"
#include "codebreaker/exact/recut.hpp"
#include "codebreaker/exact/standin.hpp"

namespace codebreaker::exact::link {

const py::list<py::str> FIELDS = py::list<py::str>({py::str("magic"), py::str("lastpage"), py::str("pages"), py::str("relocs"), py::str("hdrparas"),
                                                    py::str("minalloc"), py::str("maxalloc"), py::str("ss"), py::str("sp"), py::str("csum"),
                                                    py::str("ip"), py::str("cs"), py::str("reloctab"), py::str("ovl")});

namespace {

using py::bytes;
using py::str;
namespace re = py::re;

int64_t hex(const str& s) { return py::int_parse(s, 16); }

py::list<str> split_ws(const str& s) {
    py::list<str> out;
    const std::u32string& u = s.u32();
    auto sp = [](char32_t c) { return str(std::u32string(1, c)).isspace(); };
    size_t i = 0;
    while (true) {
        while (i < u.size() && sp(u[i])) ++i;
        if (i >= u.size()) break;
        size_t j = i;
        while (j < u.size() && !sp(u[j])) ++j;
        out.append(str(u.substr(i, j - i)));
        i = j;
    }
    return out;
}

py::list<str> lines_of(const str& p) {
    py::file f = py::open(p);
    py::list<str> out;
    for (const str& l : f) out.append(l);
    return out;
}

bytes read_bytes(const str& p) {
    py::file f = py::open_rb(p);
    bytes b = f.read();
    f.close();
    return b;
}

str join(const str& sep, const py::list<str>& xs) {
    std::u32string out;
    for (int64_t i = 0; i < xs.size(); ++i) {
        if (i) out += sep.u32();
        out += xs[i].u32();
    }
    return str(out);
}

str hexstr(const bytes& b) {
    static const char* d = "0123456789abcdef";
    std::string s;
    for (int64_t i = 0; i < b.size(); ++i) {
        s += d[b[i] >> 4];
        s += d[b[i] & 15];
    }
    return str(s);
}

int64_t word(const bytes& b, int64_t k) { return py::unpack_from1<int64_t>("<H", b, k); }

// the program's size of an MZ header's file: h[2]*512 - ((512 - h[1]) if h[1] else 0)
int64_t file_size_of(const py::list<int64_t>& h) { return h[2] * 512 - (h[1] ? (512 - h[1]) : 0); }

}  // namespace

std::tuple<std::shared_ptr<toolchain::Toolchain>, project::LinkSpec> _lp(project::Project& prj) {
    auto tc = prj.toolchain();
    const auto& lk = project::profile_parts(*tc).link;
    if (!lk) throw py::SystemExit(py::percent(str("the toolchain %s has no linker (its profile's \"link\")"), tc->name));
    return std::make_tuple(tc, *lk);
}

str _kitfile(const std::shared_ptr<toolchain::Toolchain>& tc, const project::LinkSpec& lk, const str& path) {
    const auto& kit = project::profile_parts(*tc).kit;
    if (!kit) throw py::KeyError("'kit'");                  // (lk.get("kit", tc.p["kit"]): the default is read at once)
    return toolchain::kit_path(lk.kit ? lk.kit : kit, path);
}

namespace {
str lk_dir(const project::LinkSpec& lk) {
    if (!lk.dir) throw py::KeyError("'dir'");
    return *lk.dir;
}
}  // namespace

std::tuple<Objects, py::list<str>> objects(project::Project& prj, bool verbose, std::ostream& out) {
    auto [tc, lk] = _lp(prj);
    auto mv = project::as_modules(prj);
    py::list<modules::Row> rows = modules::rows(*mv);
    py::list<int64_t> starts;
    for (const auto& r : rows) {
        if (std::get<1>(r)) starts.append(*std::get<1>(r));
    }
    starts = py::sorted(starts);
    starts.append(_library_start(prj));
    if (prj.cfg.link_order && *prj.cfg.link_order == "name") {    // (the objects named to the linker in the order of their names - the data segment shows it)
        auto& v = rows.vec();
        std::stable_sort(v.begin(), v.end(), [](const modules::Row& a, const modules::Row& b) {
            return std::get<0>(py::os::path::splitext(std::get<0>(a))).lower() < std::get<0>(py::os::path::splitext(std::get<0>(b))).lower();
        });
    }
    Objects res;
    py::list<str> stand;
    py::list<std::shared_ptr<toolchain::Toolchain>> tcs;
    for (const auto& t : prj.toolchains()) tcs.append(std::get<1>(t));
    for (const auto& row : rows) {
        const str& mod = std::get<0>(row);
        std::optional<int64_t> a = std::get<1>(row), e = std::get<2>(row);
        str title = std::get<0>(py::os::path::splitext(mod)).upper();
        if (mod.upper().endswith(str(".OBJ"))) {
            res.append(std::make_tuple(title, read_bytes(_kitfile(tc, lk, py::os::path::join(lk_dir(lk), mod.upper())))));
            continue;
        }
        auto [raw, con, addrs] = modules::compile_module(*mv, row, tcs[0]);
        if (!raw) throw py::SystemExit(py::percent(str("%s does not compile:\n%s"), mod, con.slice(std::max<int64_t>(con.size() - 1500, 0), con.size())));
        bytes rawb = *raw;
        if (a && !addrs.empty()) {
            py::list<bytes> alts;
            for (int64_t k = 1; k < tcs.size(); ++k) {
                auto r2 = modules::compile_module(*mv, row, tcs[k]);
                if (std::get<0>(r2)) alts.append(*std::get<0>(r2));
            }
            int64_t end = 0;
            bool have_end = false;
            for (int64_t s : starts) {
                if (s > *a && (!have_end || s < end)) {
                    end = s;
                    have_end = true;
                }
            }
            if (e && *e && (!have_end || *e < end)) {
                end = *e;
                have_end = true;
            }
            if (!have_end) throw py::ValueError("min() arg is an empty sequence");
            auto [nb, st] = standin::rebuild(*project::as_standin(prj), rawb, addrs, end, std::optional<str>(title), alts, project::decoder16());
            rawb = nb;
            for (const str& s : st) stand.append(s);
            if (verbose && !st.empty()) out << py::percent(str("   %s: stand-ins %s"), mod, join(str(" "), st)).utf8() << "\n";
        }
        if (a && prj.cfg.recut) rawb = recut::recut(*project::as_recut(prj), rawb);        // (its code's records cut as the program's relocations show)
        res.append(std::make_tuple(title, rawb));
    }
    return std::make_tuple(res, stand);
}

int64_t _library_start(project::Project& prj) {
    str p = prj.path(str("CRTLIB"));
    py::list<int64_t> at;
    if (py::os::path::exists(p)) {
        for (const str& l : lines_of(p)) {
            for (const str& x : re::findall(str("\\bcode[= ]([0-9a-f]{5})\\b"), l)) at.append(hex(x));
        }
    }
    if (at.empty()) return prj.own_end();
    int64_t m = at[0];
    for (int64_t x : at) m = std::min(m, x);
    return m;
}

Objects first_objects(project::Project& prj) {
    auto [tc, lk] = _lp(prj);
    str p = prj.path(str("FIRSTOBJ"));
    Objects out;
    if (!py::os::path::exists(p)) return out;
    for (const str& l : lines_of(p)) {
        str f = py::split(l, str("#"))[0].strip();
        if (!f.empty()) out.append(std::make_tuple(std::get<0>(py::os::path::splitext(f)), read_bytes(_kitfile(tc, lk, py::os::path::join(lk_dir(lk), f)))));
    }
    return out;
}

BssObjects bss_objects(project::Project& prj) {
    BssObjects res;
    str p = prj.path(str("BSS"));
    if (!py::os::path::exists(p)) return res;
    py::dict<str, int64_t> names;
    std::optional<std::tuple<int64_t, int64_t>> rng;
    py::list<std::tuple<int64_t, int64_t>> holes;
    std::optional<std::tuple<str, bool>> seg;
    for (const str& l : lines_of(p)) {
        py::list<str> f = split_ws(py::split(l, str("#"))[0]);
        if (f.empty()) continue;
        if (f[0] == "segment") {                            // (communal names in a segment of their own: after all)
            bool para = false;
            for (int64_t k = 2; k < f.size(); ++k) para = para || f[k] == "para";
            seg = std::make_tuple(f[1], para);
        } else if (f[0] == "range") {
            rng = std::make_tuple(hex(f[1]), hex(f[2]));
        } else if (f[0] == "hole") {
            holes.append(std::make_tuple(hex(f[1]), hex(f[2])));
        } else {
            names.set(f[0], hex(f[1]));
        }
    }
    if (!rng) throw py::TypeError("'NoneType' object is not subscriptable");
    py::list<int64_t> cuts;
    cuts.append(std::get<0>(*rng));
    for (const auto& an : holes) {
        cuts.append(std::get<0>(an));
        cuts.append(std::get<0>(an) + std::get<1>(an));
    }
    cuts.append(std::get<1>(*rng));
    for (int64_t k = 0; k < cuts.size(); k += 2) {
        int64_t lo = cuts[k], hi = cuts[k + 1];
        str nm = py::percent(str("bssvar%d"), k / 2);
        py::dict<str, int64_t> in_range;
        for (const auto& kv : names.items()) {
            if (lo <= std::get<1>(kv) && std::get<1>(kv) < hi) in_range.set(std::get<0>(kv), std::get<1>(kv));
        }
        res.objects.append(std::make_tuple(nm, omf::bss_object(in_range, lo, hi, nm, seg ? std::get<0>(*seg) : str("_BSS"), seg ? std::get<1>(*seg) : false)));
    }
    res.after = seg.has_value();
    return res;
}

CrtResult crt(project::Project& prj, const Objects& objs) {
    CrtResult res;
    str p = prj.path(str("CRTLIB"));
    if (!py::os::path::exists(p)) return res;
    py::dict<str, bytes> mods;
    for (const auto& m : library::modules(*project::as_library(prj))) mods.set(m->label(), m->raw);
    const bytes& img = prj.image;
    const int64_t dg = prj.dgroup;
    py::list<str> order, always;
    py::dict<str, py::list<str>> group;                     // (modules of the same bytes at one place: one is taken)
    py::set<str> proven;
    auto mv = project::as_modules(prj);
    for (const str& l : lines_of(p)) {
        if (l.startswith(str("#"))) continue;
        auto m = re::search(str("\\s#"), l);                // (the note begins at " #": a name may hold a #, NAME#2)
        str body = m ? l.slice(0, m->start()) : l;
        str note = m ? l.slice(m->end(), l.size()) : str("");
        py::list<str> f = split_ws(body);
        if (f.empty()) continue;
        if (f[0].lower().endswith(str(".asm")) && !f[0].contains(str(":")) && !mods.contains(f[0])) {        // (a transcription of the program's bytes: src/crt)
            auto [part, tc] = modules::set_of(*mv, f[0]);
            const modules::Project& pr = part ? *part : *mv;
            auto [raw, con] = tc->compile(read_bytes(py::os::path::join(py::os::path::join(prj.src, str("crt")), f[0])), pr.headers(), pr.include(), std::nullopt);
            if (!raw) throw py::SystemExit(py::percent(str("crt/%s does not assemble:\n%s"), f[0], con.slice(std::max<int64_t>(con.size() - 600, 0), con.size())));
            bytes rb = *raw;
            if (prj.cfg.recut) rb = recut::recut(*project::as_recut(prj), rb);
            mods.set(f[0], rb);
        }
        if (f.size() > 1 && f[1].startswith(str("!"))) {    // TAG:NAME !SEG+OFF=HEX: a byte or two of its data as the program has them
            str key = f[0] + str(" ") + f[1];
            if (!mods.contains(key) && mods.contains(f[0])) mods.set(key, omf::patched(mods.at(f[0]), f[1].slice(1, f[1].size())));
            f = py::list<str>({key});
        }
        auto mc = re::search(str("\\bcode ([0-9a-f]{5})\\b"), note);
        if (!mc && re::match(str("\\s*([0-9a-f]{5}\\b|\\(data only)"), note) && f[0] != "RAW") always.append(f[0]);   // (a place alone, or "data only")
        auto md = re::match(str("\\s*data [0-9a-f]{4}\\b"), note);
        if (f[0] != "RAW" && (mc || md)) {                  // (found in the program by its bytes: it is there)
            py::list<str>& ms = group.setdefault(mc ? mc->group(1) : str("data:") + f[0], py::list<str>());
            if (note.contains(str("the same bytes as"))) ms.append(f[0]);       // (the one the library stage took goes first)
            else ms.insert(0, f[0]);
            if (note.contains(str("the one whose calls go where"))) proven.add(f[0]);   // (told apart by their calls: that one is the program's)
        }
        if (f[0] == "RAW") {                                // RAW NAME code=LO-HI [data=LO-HI] [pub=N@OFF ..] [dpub=..]
            py::dict<str, py::list<str>> kv;
            for (int64_t k = 2; k < f.size(); ++k) {
                auto [key, sep, v] = f[k].partition(str("="));
                kv.setdefault(key, py::list<str>()).append(v);
            }
            auto rng = [](const str& s) {
                py::list<str> parts = py::split(s, str("-"));
                if (parts.size() != 2) throw py::ValueError(parts.size() > 2 ? "too many values to unpack (expected 2)" : "not enough values to unpack (expected 2, got 1)");
                return std::make_tuple(hex(parts[0]), hex(parts[1]));
            };
            int64_t c0 = 0, c1 = 0, d0 = 0, d1 = 0;
            if (kv.contains(str("code"))) std::tie(c0, c1) = rng(kv.at(str("code"))[0]);      // (a stand-in of data only)
            if (kv.contains(str("data"))) std::tie(d0, d1) = rng(kv.at(str("data"))[0]);
            auto ps = [](const py::list<str>& vs) {
                py::dict<str, int64_t> d;
                for (const str& v : vs) {
                    for (const str& y : py::split(v, str(","))) {
                        if (y.empty()) continue;
                        py::list<str> an = py::split(y, str("@"));
                        d.set(an[0], hex(an[1]));
                    }
                }
                return d;
            };
            mods.set(f[1], omf::raw_object(f[1], img.slice(c0, c1), ps(kv.get(str("pub"), py::list<str>())), d1 ? img.slice(dg + d0, dg + d1) : bytes(std::string()),
                                           ps(kv.get(str("dpub"), py::list<str>())), str("_TEXT"), d1 && d0 % 2 == 0 ? 2 : 1));
            order.append(f[1]);
            if (!kv.contains(str("pub")) && !kv.contains(str("dpub"))) always.append(f[1]);        // (a piece no name calls for: always taken)
            continue;
        }
        order.append(f[0]);
    }
    py::list<str> unknown;
    for (const str& m : order) {
        if (!mods.contains(m)) unknown.append(m);
    }
    if (!unknown.empty()) throw py::SystemExit(str("CRTLIB names modules no library of the profile has: ") + join(str(" "), unknown));
    py::set<str> have, need;
    for (const auto& on : objs) {
        auto [pb, e] = omf::names_of(std::get<1>(on));
        have.update(pb);
        need.update(e);
    }
    py::list<str> take;
    py::set<str> taken;
    auto add = [&](const str& m) {
        take.append(m);
        taken.add(m);
        auto [pb, e] = omf::names_of(mods.at(m));
        have.update(pb);
        need.update(e);
    };
    py::set<str> others;
    for (const auto& kv : group.items()) {
        const auto& ms = std::get<1>(kv);
        bool any_proven = false;
        for (const str& x : ms) any_proven = any_proven || proven.contains(x);
        if (any_proven) {
            for (const str& m : ms) {
                if (!proven.contains(m)) others.add(m);
            }
        }
    }
    auto close = [&]() {
        while (true) {
            py::set<str> miss = need - have;
            std::optional<str> nxt;
            for (const str& m : order) {
                if (taken.contains(m) || others.contains(m)) continue;
                if (!(std::get<0>(omf::names_of(mods.at(m))) & miss).empty()) {
                    nxt = m;
                    break;
                }
            }
            if (!nxt) return;
            add(*nxt);
        }
    };
    for (const str& m : always) add(m);
    close();
    // what the library stage found by its bytes is in the program: a group of the same bytes none of whose members anything
    // names is taken all the same (its first: a module only a stand-in calls)
    for (const auto& kv : group.items()) {
        const auto& ms = std::get<1>(kv);
        bool any_taken = false;
        for (const str& m : ms) any_taken = any_taken || taken.contains(m);
        if (!any_taken) {
            add(ms[0]);
            close();
        }
    }
    for (const auto& kv : group.items()) {                  // (the program has one routine where ours names two)
        py::list<str> both;
        for (const str& m : std::get<1>(kv)) {
            if (taken.contains(m)) both.append(m);
        }
        if (both.size() > 1) {
            py::list<str> pubs;
            for (const str& m : both) {
                py::list<str> ps = py::sorted(py::list<str>::from(std::get<0>(omf::names_of(mods.at(m)))));
                pubs.append(ps[0]);                         // (the module's first public name: the least)
            }
            res.conflicts.append(py::percent(str("the program has one routine at %s for %s; ours names %s - the sources call it by another name than "
                                                 "the program's (a name the library's modules use is the program's)"),
                                             std::get<0>(kv), join(str(" and "), both), join(str(" and "), py::sorted(pubs))));
        }
    }
    for (const str& m : order) {
        if (taken.contains(m)) res.modules.append(std::make_tuple(m, mods.at(m)));
    }
    res.missing = py::sorted(py::list<str>::from(need - have));
    return res;
}

str dosname(const str& n) {
    str r = re::sub(str("[^A-Z0-9]"), str(""), n.upper());
    r = r.slice(0, 8);
    return r.empty() ? str("M") : r;
}

std::tuple<std::optional<bytes>, str, str> run_link(project::Project& prj, const Objects& objs, const std::optional<str>& keep, const str& opts) {
    auto [tc, lk] = _lp(prj);
    str w = keep ? *keep : py::tempfile::mkdtemp(std::nullopt, std::optional<str>(str("exlink_")));
    py::os::makedirs(w, 0777, true);
    auto cleanup = [&]() {
        if (!keep) py::shutil::rmtree(w, true);
    };
    try {
        py::shutil::copy(_kitfile(tc, lk, *lk.run), py::os::path::join(w, str("LINK.EXE")));
        py::list<str> libs = lk.libs.get(prj.cfg.model, py::list<str>());
        if (py::os::path::exists(prj.path(str("CRTLIB")))) libs = py::list<str>();         // (the library's part goes in as objects)
        py::set<str> cp = py::set<str>::from(libs);
        for (const str& c : lk.copy) cp.add(c);
        for (const str& l : cp) py::shutil::copy(_kitfile(tc, lk, py::os::path::join(lk_dir(lk), l)), w);
        py::list<str> names;
        for (const auto& on : objs) {
            str dn = dosname(std::get<0>(on));
            int64_t k = 1;
            while (names.contains(dn)) {                    // (DOS names: _exit and exit)
                dn = dosname(std::get<0>(on)).slice(0, 6) + py::percent(str("%02d"), k);
                ++k;
            }
            names.append(dn);
            py::file f = py::open(py::os::path::join(w, dn + str(".OBJ")), str("wb"));
            f.write(std::get<1>(on));
            f.close();
        }
        py::list<str> lines;
        for (int64_t k = 0; k < names.size(); k += 6) {
            py::list<str> chunk = names.slice(k, k + 6);
            lines.append(join(str("+"), chunk) + (k + 6 < names.size() ? str("+") : str("")));
        }
        const str exe("OUT.EXE");
        str rsp = join(str("\r\n"), lines) + py::percent(str("\r\n%s\r\nOUT.MAP\r\n%s%s;\r\n"), exe, join(str("+"), libs), opts);
        {
            py::file f = py::open(py::os::path::join(w, str("L.LNK")), str("wb"));
            f.write(rsp.encode("utf-8"));
            f.close();
        }
        const bytes env(std::string("PATH=C:\\\0TMP=C:\\\0", 16));
        int64_t psp = project::num(lk.psp);
        auto run = [&](const str& prog, const str& cmd) {
            return emu::dos::run(py::os::path::join(w, prog), w, cmd, bytes(std::string()), emu::dos::DEFAULT_MAX_BLOCKS, false, env, std::optional<int64_t>(psp));
        };
        str con = run(str("LINK.EXE"), str("@L.LNK")).console.decode("latin1").replace(str("\r"), str(""));
        str mf = prj.path(str("EXEMOD"));
        if (py::os::path::exists(py::os::path::join(w, exe)) && py::os::path::exists(mf) && lk.exemod) {
            py::shutil::copy(_kitfile(tc, lk, *lk.exemod), py::os::path::join(w, str("EXEMOD.EXE")));
            for (const str& l : lines_of(mf)) {             // (the header changed after the link, by the kit's EXEMOD: the checksum stays the link's)
                str a = py::split(l, str("#"))[0].strip();
                if (!a.empty()) con = con + run(str("EXEMOD.EXE"), exe + str(" ") + a).console.decode("latin1");
            }
        }
        std::optional<bytes> b;
        if (py::os::path::exists(py::os::path::join(w, exe))) b = read_bytes(py::os::path::join(w, exe));
        str mp;
        if (py::os::path::exists(py::os::path::join(w, str("OUT.MAP")))) {
            py::file f = py::open(py::os::path::join(w, str("OUT.MAP")), str("r"), str("latin1"));
            mp = f.read_text();
            f.close();
        }
        cleanup();
        return std::make_tuple(b, con, mp);
    } catch (...) {
        cleanup();
        throw;
    }
}

std::tuple<py::list<int64_t>, bytes, py::list<std::tuple<int64_t, int64_t>>> parts(const bytes& exe) {
    py::list<int64_t> h;
    if (exe.size() < 28) {
        throw py::StructError("unpack_from requires a buffer of at least 28 bytes for unpacking 28 bytes at offset 0 (actual buffer size is " +
                              std::to_string(exe.size()) + ")");
    }
    for (int64_t i = 0; i < 14; ++i) h.append(word(exe, 2 * i));
    int64_t size = file_size_of(h);
    py::list<std::tuple<int64_t, int64_t>> rel;
    for (int64_t k = 0; k < h[3]; ++k) {
        if (exe.size() < h[12] + 4 * k + 4) {
            throw py::StructError("unpack_from requires a buffer of at least " + std::to_string(h[12] + 4 * k + 4) + " bytes for unpacking 4 bytes at offset " +
                                  std::to_string(h[12] + 4 * k) + " (actual buffer size is " + std::to_string(exe.size()) + ")");
        }
        rel.append(std::make_tuple(word(exe, h[12] + 4 * k), word(exe, h[12] + 4 * k + 2)));
    }
    return std::make_tuple(h, exe.slice(h[4] * 16, size), rel);
}

py::list<str> diagnose(project::Project& prj, const std::optional<bytes>& ref, const bytes& exe) {
    py::list<str> out;
    auto [hb, ib0, rb0] = parts(exe);
    bytes ib = ib0;
    py::list<int64_t> ha;
    bytes ia;
    std::vector<std::tuple<int64_t, int64_t>> ra, rb = rb0.vec();
    if (!ref) {                                             // (the original packed: ours, linked unpacked, against the unpacked image - its segment words as loaded at loadseg - and the relocations EXEPACK kept, in their order)
        ia = prj.image.slice(0, prj.packed->dest * 16);
        for (int64_t a : prj.packed->relocs) ra.emplace_back(a, 0);
        std::vector<std::tuple<int64_t, int64_t>> rbi;
        for (const auto& os_ : rb) rbi.emplace_back(std::get<1>(os_) * 16 + std::get<0>(os_), 0);
        rb = rbi;
        std::string buf = ib.raw();
        for (const auto& a_ : rb) {
            int64_t a = std::get<0>(a_);
            if (a + 2 <= static_cast<int64_t>(buf.size())) {
                int64_t v = (static_cast<uint8_t>(buf[a]) | (static_cast<uint8_t>(buf[a + 1]) << 8)) + prj.loadseg;
                buf[a] = static_cast<char>(v & 0xFF);
                buf[a + 1] = static_cast<char>((v >> 8) & 0xFF);
            }
        }
        ib = bytes(buf) + bytes(std::string(static_cast<size_t>(std::max<int64_t>(0, ia.size() - static_cast<int64_t>(buf.size()))), '\0'));   // (its variables: zeros in the unpacked image)
        if (prj.packed->ss * 16 + prj.packed->sp != hb[7] * 16 + hb[8]) {
            out.append(py::percent(str("the stack: theirs %04x:%04x, ours %04x:%04x (LINKOPT /STACK, the variables' length)"), prj.packed->ss, prj.packed->sp, hb[7], hb[8]));
        }
    } else {
        auto [h2, i2, r2] = parts(*ref);
        ha = h2;
        ia = i2;
        ra = r2.vec();
    }
    for (int64_t k = 0; k < std::min(ha.size(), hb.size()); ++k) {
        int64_t x = ha[k], y = hb[k];
        const str& f = FIELDS[k];
        if (x != y && f != "csum") {
            out.append(py::percent(str("header %-8s theirs %04x ours %04x%s"), f, x, y,
                                   (f == "minalloc" || f == "maxalloc" || f == "sp") ? str("  (EXEMOD, LINKOPT: /STACK)") : str("")));
        }
    }
    std::vector<std::tuple<int64_t, int64_t>> sra = ra, srb = rb;
    std::sort(sra.begin(), sra.end());
    std::sort(srb.begin(), srb.end());
    if (sra != srb) {
        out.append(py::percent(str("the relocations differ (theirs %d, ours %d): the program's or the library's far references"), static_cast<int64_t>(ra.size()), static_cast<int64_t>(rb.size())));
    } else if (ra != rb) {
        out.append(str("the relocations are the same but in another order: the order of the objects and their records (fixups first, CRTLIB)"));
    }
    if (ia.size() != ib.size()) out.append(py::percent(str("the image is %d bytes, ours %d"), ia.size(), ib.size()));
    std::vector<int64_t> bad;
    for (int64_t x = 0; x < std::min(ia.size(), ib.size()); ++x) {
        if (ia[x] != ib[x]) bad.push_back(x);
    }
    if (!bad.empty()) {
        int64_t first = bad[0];
        const int64_t dg = prj.dgroup;
        int64_t own = prj.own_end();
        library::MapP lm = library::locate(*project::as_library(prj));
        int64_t lib0 = lm->start && *lm->start ? *lm->start : own;
        str where = first < std::min(own, lib0) ? str("the program's own code (a module's functions or their order: MODULES)")
                    : first < dg                ? str("the library's part (CRTLIB: a module, its order, a stand-in)")
                                                : str("the data segment (the modules' data: dADDR.c, the library's data, BSS)");
        std::vector<std::tuple<int64_t, int64_t>> runs;
        int64_t s = bad[0], p = bad[0];
        for (size_t k = 1; k < bad.size(); ++k) {
            int64_t x = bad[k];
            if (x > p + 8) {
                runs.emplace_back(s, p);
                s = x;
            }
            p = x;
        }
        runs.emplace_back(s, p);
        out.append(py::percent(str("%d bytes of the image differ, in %d places; the first at %05x: %s"), static_cast<int64_t>(bad.size()), static_cast<int64_t>(runs.size()), first, where));
        std::optional<int64_t> shift;
        for (int64_t d = -64; d <= 64; ++d) {               // (ours the same bytes, moved: something before is too long)
            if (!d) continue;
            bool all = true;
            for (int64_t k = 0; k < 32 && all; ++k) all = 0 <= first + d + k && first + d + k < ib.size() && first + k < ia.size() && ib[first + d + k] == ia[first + k];
            if (all) {
                shift = d;
                break;
            }
        }
        if (shift) {
            out.append(py::percent(str("   from there ours is the original moved by %+d bytes: what comes before it is %d bytes %s"), *shift, std::abs(*shift),
                                   *shift > 0 ? str("too long") : str("too short")));
        }
        for (size_t k = 0; k < std::min<size_t>(runs.size(), 12); ++k) {
            int64_t a = std::get<0>(runs[k]), b = std::get<1>(runs[k]);
            int64_t e = std::min(b + 1, a + 10);
            out.append(py::percent(str("   %05x..%05x  theirs %s  ours %s"), a, b, hexstr(ia.slice(a, e)), hexstr(ib.slice(a, e))));
        }
        // the data segment on its own (a shift there moves every word of the code that points into it)
        bytes da = ia.slice(dg, ia.size());
        bytes db = ib.size() > dg ? ib.slice(dg, ib.size()) : bytes(std::string());
        std::optional<int64_t> dbad;
        for (int64_t k = 0; k < std::min(da.size(), db.size()); ++k) {
            if (da[k] != db[k]) {
                dbad = k;
                break;
            }
        }
        if (dbad) {
            auto mv = project::as_modules(prj);
            std::optional<std::tuple<str, int64_t>> who;    // the module whose data starts last at or before dbad
            for (const auto& r : modules::rows(*mv)) {
                if (std::get<3>(r) && *std::get<3>(r) <= *dbad && (!who || *std::get<3>(r) > std::get<1>(*who))) who = std::make_tuple(std::get<0>(r), *std::get<3>(r));
            }
            str mod = who ? std::get<0>(*who) : str("?");
            std::optional<std::tuple<int64_t, str>> lib;    // the library item whose data starts last at or before dbad (the first of them)
            for (const auto& row : lm->order) {
                const library::Item& it = std::get<2>(row);
                std::optional<int64_t> d;
                str label;
                if (std::holds_alternative<library::PlacedP>(it)) {
                    d = std::get<library::PlacedP>(it)->data;
                    label = std::get<library::PlacedP>(it)->m->label();
                } else if (std::holds_alternative<library::DataRawP>(it)) {
                    const library::DataRawP& dr = std::get<library::DataRawP>(it);
                    d = dr->data;
                    label = dr->m ? dr->m->label() : dr->name;
                }
                if (d && *d <= *dbad && (!lib || *d > std::get<0>(*lib))) lib = std::make_tuple(*d, label);
            }
            std::optional<str> lname;
            if (lib) lname = std::get<1>(*lib);
            bool lib_later = lib && (!who || std::get<0>(*lib) > std::get<1>(*who));
            out.append(py::percent(str("the data segment parts at DS:%04x (after the module %s%s)"), *dbad, mod,
                                   (lname && lib_later) ? py::percent(str(", the library's %s"), *lname) : str("")));
            for (int64_t d = -64; d <= 64; ++d) {
                if (!d) continue;
                bool all = true;
                for (int64_t k = 0; k < 24 && all; ++k) all = 0 <= *dbad + d + k && *dbad + d + k < db.size() && *dbad + k < da.size() && db[*dbad + d + k] == da[*dbad + k];
                if (all) {
                    out.append(py::percent(str("   from there ours is the program's moved by %+d: what comes before is %d bytes %s"), d, std::abs(d),
                                           d > 0 ? str("too long") : str("too short")));
                    break;
                }
            }
            out.append(py::percent(str("   theirs %s\n   ours   %s"), hexstr(da.slice(*dbad, *dbad + 24)), hexstr(db.slice(*dbad, *dbad + 24))));
            for (const str& l : drift(da, db)) out.append(str("   ") + l);
        }
    }
    return out;
}

py::list<str> drift(const bytes& a, const bytes& b, int64_t step, int64_t span, int64_t reach) {
    py::list<str> out;
    str prev("?");
    for (int64_t k = 0; k < a.size() - span; k += step) {
        bytes seg = a.slice(k, k + span);
        py::set<int64_t> distinct;
        for (int64_t i = 0; i < seg.size(); ++i) distinct.add(seg[i]);
        if (distinct.size() <= 3) continue;
        std::optional<int64_t> d;
        std::vector<int64_t> dds{0};
        for (int64_t r = 1; r < reach / 2; ++r) {
            dds.push_back(2 * r);
            dds.push_back(-2 * r);
        }
        for (int64_t dd : dds) {
            if (k + dd >= 0 && b.slice(k + dd, k + dd + span) == seg) {
                d = dd;
                break;
            }
        }
        if (!d) continue;                                   // (a word that points elsewhere: not a move)
        str ds = py::percent(str("%+d"), *d);
        if (ds != prev) {
            out.append(py::percent(str("DS:%04x on, ours is %+d"), k, *d));
            prev = ds;
        }
    }
    return out.slice(0, 16);
}

str opts_of(project::Project& prj) {
    str lf = prj.path(str("LINKOPT"));
    if (!py::os::path::exists(lf)) return str("");
    py::list<str> parts_;
    for (const str& l : lines_of(lf)) {
        str x = py::split(l, str("#"))[0].strip();
        if (!x.empty()) parts_.append(x);
    }
    return join(str(" "), parts_);
}

Built build(project::Project& prj, const std::optional<str>& keep, bool verbose, bool quiet, const std::optional<str>& opts0, std::ostream& out) {
    auto [objs0, stand] = objects(prj, verbose, out);
    Objects objs = first_objects(prj) + objs0;
    BssObjects bss = bss_objects(prj);
    Objects after;
    auto mv = project::as_modules(prj);
    for (const str& f : prj.cfg.after_library) {            // (whole files linked after the library: a module of the communal variables at their places, as c_common is last)
        auto [part, tc] = modules::set_of(*mv, f);
        const modules::Project& pr = part ? *part : *mv;
        auto [raw, con] = tc->compile(read_bytes(py::os::path::join(prj.src, f)), pr.headers(), pr.include(), std::nullopt);
        if (!raw) throw py::SystemExit(py::percent(str("%s does not compile:\n%s"), f, con.slice(std::max<int64_t>(con.size() - 600, 0), con.size())));
        after.append(std::make_tuple(std::get<0>(py::os::path::splitext(f)), *raw));
    }
    CrtResult lib = crt(prj, objs + bss.objects + after);
    py::set<str> comm;                                      // (communal names are the linker's to place, and _edata, _end its own: not left open)
    for (const Objects* grp : {&objs, &bss.objects, &after, &lib.modules}) {
        for (const auto& on : *grp) {
            for (const str& n : omf::read(std::get<1>(on)).comm.keys()) comm.add(n);
        }
    }
    py::list<str> miss;
    const auto& lk = project::profile_parts(*prj.toolchain()).link;
    py::list<str> lnames = lk ? lk->names : py::list<str>({str("_edata"), str("_end")});
    for (const str& m : lib.missing) {
        if (!comm.contains(m) && !lnames.contains(m)) miss.append(m);
    }
    if (!miss.empty() && !quiet) out << (str("   not resolved: ") + join(str(" "), miss)).utf8() << "\n";
    if (!quiet) {
        for (const str& c : lib.conflicts) out << (str("   ") + c).utf8() << "\n";
    }
    Objects all = objs;
    if (bss.after) {
        all = all + lib.modules + bss.objects;
    } else {
        all = all + (bss.objects.empty() ? Objects() : bss.objects.slice(0, 1)) + lib.modules + (bss.objects.empty() ? Objects() : bss.objects.slice(1, bss.objects.size()));
    }
    all = all + after;
    str o = opts0 ? *opts0 : opts_of(prj);
    auto [exe, con, mp] = run_link(prj, all, keep, o);
    Built b;
    b.exe = exe;
    b.console = con;
    b.map = mp;
    b.stand = stand;
    return b;
}

py::list<str> fit(project::Project& prj, int64_t rounds) {
    py::list<str> out;
    auto write_crtlib = [&]() {
        py::file f = py::open(prj.path(str("CRTLIB")), str("w"), std::nullopt, std::nullopt, str("\n"));
        prj.forget_library();
        f.write(join(str("\n"), library::lines(*library::locate(*project::as_library(prj)))) + str("\n"));   // (the library stage's, with the sizes so far)
        f.close();
    };
    for (int64_t r = 0; r < rounds; ++r) {
        write_crtlib();
        Built bl = build(prj, std::nullopt, false, true);
        if (!bl.exe) {
            out.append(py::percent(str("round %d: no program was written"), r));
            break;
        }
        const int64_t dg = prj.dgroup;
        std::vector<std::tuple<int64_t, int64_t>> moves;
        for (const str& l : drift(prj.image.slice(dg, prj.image.size()), std::get<1>(parts(*bl.exe)).slice(dg, std::get<1>(parts(*bl.exe)).size()))) {
            auto m = re::match(str("DS:([0-9a-f]+) on, ours is ([+-]\\d+)"), l);
            if (m) moves.emplace_back(hex(m->group(1)), py::int_parse(m->group(2), 10));
        }
        std::optional<std::tuple<int64_t, int64_t>> first;
        for (const auto& pd : moves) {
            if (std::get<1>(pd)) {
                first = pd;
                break;
            }
        }
        if (!first) {
            out.append(py::percent(str("round %d: the data segment agrees"), r));
            break;
        }
        int64_t p = std::get<0>(*first), d = std::get<1>(*first);
        library::MapP lib = library::locate(*project::as_library(prj));
        const auto& sizes = prj.cfg.data_sizes;
        py::list<library::DataRawP> cands;
        for (const library::DataRawP& x : lib->dataraws) {
            bool named = false;
            for (const str& n : x->pubs.keys()) named = named || sizes.contains(n);
            if (x->end <= p && p <= x->end + 32 && (x->why.contains(str("a guess")) || named)) cands.append(x);
        }
        if (cands.empty()) {
            bytes ds = prj.image.slice(dg, prj.image.size());
            int64_t q = p;
            while (q > 0 && ds[q - 1] == 0) --q;
            if (d < 0 && p - q >= -d) {                     // (zeros no module accounts for: a pad, where they begin)
                int64_t at = q + (q & 1);
                prj.set_project_entry(str("data_pads"), py::percent(str("%04x"), at), -d);
                out.append(py::percent(str("round %d: at DS:%04x ours is %+d, after %d zeros: a pad of %d bytes at DS:%04x"), r, p, d, p - q, -d, at));
                continue;
            }
            out.append(py::percent(str("round %d: at DS:%04x ours is %+d, and no stand-in of a guessed length is before it"), r, p, d));
            break;
        }
        library::DataRawP x = cands[0];
        for (const auto& c : cands) {
            if (c->end > x->end) x = c;
        }
        str name = x->pubs.keys()[0];
        int64_t size = x->end - x->data - d;
        prj.set_project_entry(str("data_sizes"), name, size);
        out.append(py::percent(str("round %d: at DS:%04x ours is %+d: %s's stand-in made %d bytes"), r, p, d, name, size));
        write_crtlib();
    }
    return out;
}

int64_t main(project::Project& prj, const py::list<str>& argv, std::ostream& out) {
    auto say = [&](const str& s) { out << s.utf8() << "\n"; };
    auto lw = project::as_linkwhole(prj);
    if (linkwhole::wanted(*lw)) return linkwhole::main(*lw, argv, out);        // (a program of whole source files: exact/linkwhole.py)
    std::optional<str> keep;
    if (argv.contains(str("--keep"))) keep = argv[argv.index(str("--keep")) + 1];
    bool v = argv.contains(str("-v"));
    if (argv.contains(str("--fit"))) {
        for (const str& l : fit(prj)) say(str("   ") + l);
    }
    Built bl = build(prj, keep, v, false, std::nullopt, out);
    py::list<str> msgs;
    for (const str& l : py::split(bl.console, str("\n"))) {
        if (!l.strip().empty() && !l.contains(str("Microsoft")) && !l.contains(str("Copyright"))) msgs.append(l);
    }
    for (const str& l : msgs.slice(0, 20)) say(str("   LINK: ") + l);
    if (argv.contains(str("--map"))) say(bl.map);
    if (!bl.exe) {
        say(str("no program was written"));
        return 1;
    }
    const bytes& exe = *bl.exe;
    const bytes& ref = prj.packed ? prj.packed_file : prj.file;      // (an unpacked image: the file is held to the packed one)
    bool same = exe == ref;
    py::list<str> diag;
    if (!same && prj.packed) {                              // (where ours parts from it: linked again, not packed, and held to the unpacked image)
        str un = re::sub(str("\\s*/E(XEPACK|XE|X)?\\b"), str(""), opts_of(prj), 0, re::I);
        Built b2 = build(prj, keep, false, true, std::optional<str>(un), out);
        if (b2.exe) diag = diagnose(prj, std::nullopt, *b2.exe);
        bool hit = false;
        for (const str& l : diag) hit = hit || l.contains(str("differ")) || l.contains(str("stack")) || l.contains(str("image is"));
        if (!same && b2.exe && !hit) diag.append(str("unpacked, ours is the program; the packed files differ: EXEPACK's work (its table's order: the order of the fixups)"));
    } else if (!same) {
        diag = diagnose(prj, std::optional<bytes>(ref), exe);
    }
    for (const str& l : diag) say(str("   ") + l);
    bool bad = false;
    for (const str& l : diag) bad = bad || l.contains(str("image")) || l.contains(str("header")) || l.contains(str("relocations differ"));
    bool ok = same || !bad;
    say(py::percent(str("%s linked from the sources: %d bytes; %s"), py::os::path::basename(prj.exe), exe.size(),
                    same ? str("the FILE IDENTICAL") : (ok ? str("the image, header and relocations identical") : str("not identical"))));
    say(py::percent(str("   %d functions are stand-ins of the program's bytes%s"), bl.stand.size(),
                    !bl.stand.empty() ? str(": ") + join(str(" "), bl.stand) : str("")));
    return ok ? 0 : 1;
}

}  // namespace codebreaker::exact::link
