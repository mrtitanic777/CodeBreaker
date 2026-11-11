// codebreaker/symbols/debug_build_estimate.cpp -- typed C++ port of codebreaker.symbols.debug_build_estimate (see .hpp).

#include "codebreaker/symbols/debug_build_estimate.hpp"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/formats/ne.hpp"
#include "py_json.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::symbols::debug_build_estimate {

namespace segment = codebreaker::source::ne_asm::segment;
namespace ne_mod = codebreaker::formats::ne;
namespace dn = codebreaker::symbols::debug_build;
using py::str;

const py::list<str> CONFIDENT_TIERS = {str("EXACT"), str("HIGH"), str("ALIGNED")};

// ---- difflib.SequenceMatcher -----------------------------------------------------------------------------------------
namespace {

class Matcher {
  public:
    Matcher(const std::vector<int64_t>& a, const std::vector<int64_t>& b, bool autojunk) : a_(a), b_(b) {
        // __chain_b: {element -> the indices of b it is at}, the popular elements dropped when autojunk and len(b) >= 200
        for (size_t j = 0; j < b_.size(); j++) b2j_[b_[j]].push_back(static_cast<int64_t>(j));
        const size_t n = b_.size();
        if (autojunk && n >= 200) {
            const size_t ntest = n / 100 + 1;
            for (auto it = b2j_.begin(); it != b2j_.end();) {
                if (it->second.size() > ntest) it = b2j_.erase(it);
                else ++it;
            }
        }
    }

    Block find_longest_match(int64_t alo, int64_t ahi, int64_t blo, int64_t bhi) const {
        int64_t besti = alo, bestj = blo, bestsize = 0;
        std::unordered_map<int64_t, int64_t> j2len;
        for (int64_t i = alo; i < ahi; i++) {
            std::unordered_map<int64_t, int64_t> newj2len;
            const auto found = b2j_.find(a_[i]);
            if (found != b2j_.end()) {
                for (const int64_t j : found->second) {
                    if (j < blo) continue;
                    if (j >= bhi) break;
                    const auto prev = j2len.find(j - 1);
                    const int64_t k = (prev == j2len.end() ? 0 : prev->second) + 1;
                    newj2len[j] = k;
                    if (k > bestsize) {
                        besti = i - k + 1;
                        bestj = j - k + 1;
                        bestsize = k;
                    }
                }
            }
            j2len = std::move(newj2len);
        }
        // extend the match with the elements that are equal on either side (nothing is junk here; popular elements
        // were left out of b2j, so this is what brings them back in)
        while (besti > alo && bestj > blo && a_[besti - 1] == b_[bestj - 1]) {
            besti--;
            bestj--;
            bestsize++;
        }
        while (besti + bestsize < ahi && bestj + bestsize < bhi && a_[besti + bestsize] == b_[bestj + bestsize]) bestsize++;
        return Block(besti, bestj, bestsize);
    }

    std::vector<Block> get_matching_blocks() const {
        const int64_t la = static_cast<int64_t>(a_.size()), lb = static_cast<int64_t>(b_.size());
        std::vector<std::tuple<int64_t, int64_t, int64_t, int64_t>> queue{{0, la, 0, lb}};
        std::vector<Block> matching;
        while (!queue.empty()) {
            const auto [alo, ahi, blo, bhi] = queue.back();
            queue.pop_back();
            const Block x = find_longest_match(alo, ahi, blo, bhi);
            const auto [i, j, k] = x;
            if (k) {
                matching.push_back(x);
                if (alo < i && blo < j) queue.emplace_back(alo, i, blo, j);
                if (i + k < ahi && j + k < bhi) queue.emplace_back(i + k, ahi, j + k, bhi);
            }
        }
        std::sort(matching.begin(), matching.end());
        int64_t i1 = 0, j1 = 0, k1 = 0;
        std::vector<Block> non_adjacent;
        for (const auto& [i2, j2, k2] : matching) {
            if (i1 + k1 == i2 && j1 + k1 == j2) {
                k1 += k2;
            } else {
                if (k1) non_adjacent.emplace_back(i1, j1, k1);
                i1 = i2;
                j1 = j2;
                k1 = k2;
            }
        }
        if (k1) non_adjacent.emplace_back(i1, j1, k1);
        non_adjacent.emplace_back(la, lb, 0);
        return non_adjacent;
    }

  private:
    const std::vector<int64_t>& a_;
    const std::vector<int64_t>& b_;
    std::unordered_map<int64_t, std::vector<int64_t>> b2j_;
};

// ids for the elements of sequences, equal elements the same id
class Ids {
  public:
    int64_t of(const std::string& key) {
        const auto it = ids_.find(key);
        if (it != ids_.end()) return it->second;
        const int64_t id = static_cast<int64_t>(ids_.size());
        ids_.emplace(key, id);
        return id;
    }

  private:
    std::unordered_map<std::string, int64_t> ids_;
};

// a skeleton (a tuple of instruction texts) as one key (each text length-prefixed, so equal skeletons are equal keys only)
std::string key_of(const py::list<str>& skel) {
    std::string k;
    for (const str& s : skel) {
        const std::string u = s.utf8();
        k += std::to_string(u.size()) + ":" + u + "|";
    }
    return k;
}

std::vector<int64_t> mnem_ids(Ids& ids, const py::list<str>& m) {
    std::vector<int64_t> out;
    for (const str& s : m) out.push_back(ids.of(s.utf8()));
    return out;
}

// round(x, 2) as Python rounds a float: to the nearest two-decimal value (the exact binary value decides ties)
double round2(double x) {
    char buf[64];
    std::snprintf(buf, sizeof buf, "%.2f", x);
    return std::strtod(buf, nullptr);
}

// "%d:%04x" % (seg, off)
str key_of_func(int64_t seg, int64_t off) { return py::percent(str("%d:%04x"), seg, off); }

// sorted(dict)
std::vector<int64_t> sorted_keys(const py::dict<int64_t, segment::Func>& d) {
    std::vector<int64_t> v;
    for (const int64_t k : d.keys()) v.push_back(k);
    std::sort(v.begin(), v.end());
    return v;
}

}  // namespace

std::vector<Block> get_matching_blocks(const std::vector<int64_t>& a, const std::vector<int64_t>& b, bool autojunk) {
    return Matcher(a, b, autojunk).get_matching_blocks();
}

double ratio(const std::vector<int64_t>& a, const std::vector<int64_t>& b, bool autojunk) {
    int64_t matches = 0;
    for (const Block& blk : get_matching_blocks(a, b, autojunk)) matches += std::get<2>(blk);
    const int64_t length = static_cast<int64_t>(a.size() + b.size());
    if (length) return 2.0 * static_cast<double>(matches) / static_cast<double>(length);
    return 1.0;
}

py::list<str> _skel(const segment::Seg& sg, int64_t start) {
    static const py::re::Pattern ws = py::re::compile(str("\\s+"));
    static const py::re::Pattern hex = py::re::compile(str("0x[0-9a-f]+"));
    py::list<str> out;
    const auto rows = std::get<1>(sg.func_rows(start));
    for (const auto& oa : rows) {
        out.append(hex.sub(str("X"), ws.sub(str(" "), std::get<1>(oa)).strip()));
    }
    return out;
}

py::list<str> _mnem(const segment::Seg& sg, int64_t start) {
    py::list<str> out;
    const auto rows = std::get<1>(sg.func_rows(start));
    for (const auto& oa : rows) {
        const py::list<str> parts = py::split(std::get<1>(oa));
        out.append(parts.size() ? parts[0] : str("?"));
    }
    return out;
}

py::dict<int64_t, py::dict<int64_t, str>> _map_names(const str& debug_map) {
    py::dict<int64_t, py::dict<int64_t, str>> names;
    const str txt = py::file::open(debug_map, str("r"), str("cp1252"), str("replace")).read_text();
    for (const py::re::Match& m : py::re::finditer(str("^ ([0-9A-Fa-f]{4}):([0-9A-Fa-f]{4})\\s+(\\w+)"), txt, py::re::M)) {
        names.setdefault(py::int_parse(m.group(1), 16), py::dict<int64_t, str>()).set(py::int_parse(m.group(2), 16), m.group(3));
    }
    return names;
}

py::dict<int64_t, Tier> _align_segment(int64_t i, const segment::Seg& rsg, const segment::Seg& dsg,
                                       const py::dict<int64_t, py::dict<int64_t, str>>& dbg_names, const py::dict<str, str>& exact_map,
                                       bool predict_exports) {
    const py::dict<int64_t, str>& seg_names = dbg_names.at(i);
    py::dict<str, int64_t> d_by_name;
    for (const auto& kv : seg_names.items()) {
        if (!d_by_name.contains(std::get<1>(kv))) d_by_name.set(std::get<1>(kv), std::get<0>(kv));
    }
    const std::vector<int64_t> r_offs = sorted_keys(rsg.funcs);
    std::vector<int64_t> d_offs;
    for (const int64_t off : seg_names.keys()) {
        if (dsg.valid.contains(off)) d_offs.push_back(off);
    }
    std::sort(d_offs.begin(), d_offs.end());
    std::map<int64_t, py::list<str>> r_skel, d_skel;
    std::map<int64_t, dn::Sig> r_strict, d_strict;
    std::map<int64_t, py::list<str>> r_mnem, d_mnem;
    for (const int64_t off : r_offs) {
        r_skel[off] = _skel(rsg, off);
        r_strict[off] = dn::norm_sig(rsg, off);
        r_mnem[off] = _mnem(rsg, off);
    }
    for (const int64_t off : d_offs) {
        d_skel[off] = _skel(dsg, off);
        d_strict[off] = dn::norm_sig(dsg, off);
        d_mnem[off] = _mnem(dsg, off);
    }

    auto is_target = [&](int64_t off) -> bool {
        const str nm = rsg.name_at(off);
        const bool exp = !nm.empty() && !nm.lower().startswith(str("sub_"));
        return predict_exports ? exp : !exp;
    };

    // order-consistent anchors from the validated-exact set
    std::vector<std::pair<int64_t, int64_t>> anch;
    for (const int64_t roff : r_offs) {
        const std::optional<str> nm = exact_map.get(key_of_func(i, roff));
        if (nm && !nm->empty() && d_by_name.contains(*nm)) anch.emplace_back(roff, d_by_name.at(*nm));
    }
    std::vector<std::pair<int64_t, int64_t>> keep;
    for (const auto& a : anch) {
        while (!keep.empty() && keep.back().second >= a.second) keep.pop_back();
        keep.push_back(a);
    }
    anch = keep;

    py::dict<int64_t, Tier> out;
    std::vector<std::pair<int64_t, int64_t>> bounds{{-1, -1}};
    bounds.insert(bounds.end(), anch.begin(), anch.end());
    bounds.emplace_back(int64_t(1) << 30, int64_t(1) << 30);
    Ids ids;
    for (size_t bi = 0; bi + 1 < bounds.size(); bi++) {
        const auto [ra, da] = bounds[bi];
        const auto [rb, db] = bounds[bi + 1];
        std::vector<int64_t> rgap, dgap;
        for (const int64_t x : r_offs) {
            if (ra < x && x < rb) rgap.push_back(x);
        }
        for (const int64_t x : d_offs) {
            if (da < x && x < db) dgap.push_back(x);
        }
        std::unordered_set<int64_t> used;
        std::vector<int64_t> sa, sb;
        for (const int64_t x : rgap) sa.push_back(ids.of(key_of(r_skel[x])));
        for (const int64_t x : dgap) sb.push_back(ids.of(key_of(d_skel[x])));
        for (const Block& blk : get_matching_blocks(sa, sb, false)) {
            const auto [ai, bj, size] = blk;
            for (int64_t k = 0; k < size; k++) {
                const int64_t roff = rgap[ai + k], doff = dgap[bj + k];
                if (!is_target(roff)) continue;
                const str tier = r_strict[roff] == d_strict[doff] ? str("EXACT") : str("HIGH");
                out.set(roff, Tier(seg_names.at(doff), tier, 1.0));
                used.insert(doff);
            }
        }
        std::vector<int64_t> rrem, drem;
        for (const int64_t x : rgap) {
            if (!out.contains(x)) rrem.push_back(x);
        }
        for (const int64_t x : dgap) {
            if (!used.count(x)) drem.push_back(x);
        }
        if (!rrem.empty() && rrem.size() == drem.size()) {
            for (size_t k = 0; k < rrem.size(); k++) {
                const int64_t roff = rrem[k], doff = drem[k];
                if (is_target(roff)) {
                    const double sc = ratio(mnem_ids(ids, r_mnem[roff]), mnem_ids(ids, d_mnem[doff]), true);
                    out.set(roff, Tier(seg_names.at(doff), str("ALIGNED"), round2(sc)));
                    used.insert(doff);
                }
            }
            continue;
        }
        for (const int64_t roff : rgap) {
            if (out.contains(roff) || !is_target(roff)) continue;
            std::optional<int64_t> best;
            double bs = 0.0;
            for (const int64_t doff : dgap) {
                if (used.count(doff)) continue;
                const double sc = ratio(mnem_ids(ids, r_mnem[roff]), mnem_ids(ids, d_mnem[doff]), true);
                if (sc > bs) {
                    best = doff;
                    bs = sc;
                }
            }
            if (best && bs >= FUZZY_MIN) {
                out.set(roff, Tier(seg_names.at(*best), bs >= PROBABLE_MIN ? str("PROBABLE") : str("GUESS"), round2(bs)));
                used.insert(*best);
            }
        }
    }
    return out;
}

std::tuple<py::dict<str, Tier>, py::dict<str, std::tuple<int64_t, int64_t>>> estimate_module(
    const str& retail_exe, const str& debug_exe, const str& debug_map, const py::dict<str, str>& ordinals) {
    const py::dict<int64_t, py::dict<int64_t, str>> dbg_names = _map_names(debug_map);
    const ne_mod::NEFile dbg_ne(debug_exe);
    const ne_mod::NEFile ret_ne(retail_exe);
    const py::dict<dn::Sig, str> idx = dn::build_debug_index(debug_exe, debug_map, ordinals);
    const py::dict<str, str> exact_map = std::get<0>(dn::match_module(retail_exe, idx, ordinals));

    py::dict<str, Tier> tiers;
    py::dict<str, std::tuple<int64_t, int64_t>> acc;
    for (const char* t : {"EXACT", "HIGH", "ALIGNED", "PROBABLE", "GUESS"}) acc.set(str(t), std::make_tuple(int64_t(0), int64_t(0)));
    int64_t i = 0;
    for (const ne_mod::Segment& s : ret_ne.segments()) {
        ++i;
        if (!(s.kind == str("CODE")) || !dbg_names.contains(i)) continue;
        const segment::Seg rsg(ret_ne, i, ordinals), dsg(dbg_ne, i, ordinals);
        for (const int64_t roff : rsg.funcs.keys()) {   // lock validated anchors
            const std::optional<str> nm = exact_map.get(key_of_func(i, roff));
            if (nm && !nm->empty()) tiers.set(key_of_func(i, roff), Tier(*nm, str("EXACT"), 1.0));
        }
        for (const auto& kv : _align_segment(i, rsg, dsg, dbg_names, exact_map).items()) {
            const str k = key_of_func(i, std::get<0>(kv));
            if (!tiers.contains(k)) tiers.set(k, std::get<1>(kv));
        }
        for (const auto& kv : _align_segment(i, rsg, dsg, dbg_names, exact_map, true).items()) {
            const Tier& v = std::get<1>(kv);
            const str real = rsg.name_at(std::get<0>(kv));
            auto& slot = acc.at(std::get<1>(v));
            const int64_t correct = std::get<0>(slot) + (std::get<0>(v).upper() == real.upper() ? 1 : 0);
            acc.set(std::get<1>(v), std::make_tuple(correct, std::get<1>(slot) + 1));
        }
    }
    return std::make_tuple(tiers, acc);
}

py::dict<str, str> confident_names(const py::dict<str, Tier>& tiers, const str& retail_exe, const py::dict<str, str>& ordinals) {
    const ne_mod::NEFile ne(retail_exe);
    py::dict<str, std::tuple<int64_t, int64_t>> exp_at;
    int64_t i = 0;
    for (const ne_mod::Segment& s : ne.segments()) {
        ++i;
        if (!(s.kind == str("CODE"))) continue;
        const segment::Seg sg(ne, i, ordinals);
        for (const auto& kv : sg.export_names.items()) exp_at.set(std::get<1>(kv).upper(), std::make_tuple(i, std::get<0>(kv)));
    }
    py::dict<str, str> conf;
    for (const auto& kv : tiers.items()) {
        const str& k = std::get<0>(kv);
        const str& name = std::get<0>(std::get<1>(kv));
        const str& tier = std::get<1>(std::get<1>(kv));
        bool confident = false;
        for (const str& t : CONFIDENT_TIERS) {
            if (t == tier) confident = true;
        }
        if (!confident) continue;
        const py::list<str> parts = py::split(k, str(":"));
        const int64_t si = py::int_parse(parts[0], 10), off = py::int_parse(parts[1], 16);
        const auto clash = exp_at.get(name.upper());
        if (clash && !(*clash == std::make_tuple(si, off))) continue;
        conf.set(k, name);
    }
    py::dict<str, int64_t> cnt;
    for (const auto& kv : conf.items()) cnt.set(std::get<1>(kv), cnt.get(std::get<1>(kv), 0) + 1);
    py::dict<str, str> out;
    for (const auto& kv : conf.items()) {
        if (cnt.at(std::get<1>(kv)) <= 1) out.set(std::get<0>(kv), std::get<1>(kv));
    }
    return out;
}

namespace {

// json.dump(obj, f, indent=0, sort_keys=True) of {str: str}
str dump_indent0(const py::dict<str, str>& d) {
    std::vector<std::string> keys;
    for (const str& k : d.keys()) keys.push_back(k.utf8());
    std::sort(keys.begin(), keys.end());   // (UTF-8 order is code point order)
    if (keys.empty()) return str("{}");
    std::string o = "{";
    bool first = true;
    for (const std::string& k : keys) {
        if (!first) o += ",";
        first = false;
        o += "\n";
        py::json::put(o, str(k));
        o += ": ";
        py::json::put(o, d.at(str(k)));
    }
    o += "\n}";
    return str(o);
}

void print_line(const str& s) {
    const std::string t = s.utf8() + "\n";
    std::fwrite(t.data(), 1, t.size(), stdout);
}

}  // namespace

void main() {
    const py::dict<str, str> ordinals = segment::load_ordinals(str("ORDINALS.md"));
    const str retail = *base::corpus::retail(str("krnl386.exe"));
    const str dbg = base::corpus::reference(py::list<str>{str("WIN32SDK"), str("SCT"), str("DEBUG16"), str("I386")});
    const auto [tiers, acc] = estimate_module(retail, py::os::path::join(dbg, str("KRNL386.EXE")), py::os::path::join(dbg, str("KRNL386.MAP")), ordinals);
    const py::dict<str, str> conf = confident_names(tiers, retail, ordinals);
    const str out = py::os::path::join(py::settings::get_str(str("state")), str("idiomatic"), str("krnl386"));
    py::os::makedirs(out, 0777, true);
    {
        py::file f = py::open(py::os::path::join(out, str("names.json")), str("w"));
        f.write(dump_indent0(conf));
    }
    for (const char* t : {"EXACT", "HIGH", "ALIGNED", "PROBABLE", "GUESS"}) {
        const auto [c, tot] = acc.at(str(t));
        int64_t n = 0;
        for (const auto& kv : tiers.items()) {
            if (std::get<1>(std::get<1>(kv)) == str(t)) n++;
        }
        const str a = tot ? py::percent(str("%d/%d=%.0f%%"), c, tot, 100.0 * static_cast<double>(c) / static_cast<double>(tot)) : str("-");
        print_line(py::percent(str("  %-9s %4d   export-accuracy %s"), str(t), n, a));
    }
    print_line(py::percent(str("confident names.json (EXACT+HIGH+ALIGNED, guarded): %d"), conf.size()));
}

}  // namespace codebreaker::symbols::debug_build_estimate
