#include "exact/names.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/log.hpp"
#include "core/text.hpp"
#include "exact/carve.hpp"
#include "exact/x86.hpp"

namespace cb::exact::names {

namespace {

bool nm_hex(char c, bool upper_ok = true) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (upper_ok && c >= 'A' && c <= 'F'); }

// A hex number of `lo`..`hi` digits (the whole of `s`).
std::optional<std::int64_t> nm_hex_number(std::string_view s, std::size_t lo, std::size_t hi, bool upper_ok = true) {
    if (s.size() < lo || s.size() > hi) return std::nullopt;
    std::int64_t v = 0;
    for (char c : s) {
        if (!nm_hex(c, upper_ok)) return std::nullopt;
        v = v * 16 + (c <= '9' ? c - '0' : (c | 0x20) - 'a' + 10);
    }
    return v;
}

// A name that is a place: f or P and four or five hex digits (the address of a function), g_ or G and one to four (a variable's).
std::optional<std::int64_t> nm_code_name(std::string_view x) {
    if (x.size() < 5 || (x[0] != 'f' && x[0] != 'P')) return std::nullopt;
    return nm_hex_number(x.substr(1), 4, 5);
}
std::optional<std::int64_t> nm_data_name(std::string_view x) {
    if (x.starts_with("g_")) return nm_hex_number(x.substr(2), 1, 4);
    if (x.starts_with('G')) return nm_hex_number(x.substr(1), 1, 4);
    return std::nullopt;
}

// `_?f[0-9a-f]{5}`: the name the engine gives a function after its place.
bool nm_address_name(std::string_view n) {
    if (n.starts_with('_')) n.remove_prefix(1);
    return n.size() == 6 && n[0] == 'f' && std::all_of(n.begin() + 1, n.end(), [](char c) { return nm_hex(c, false); });
}
// `_?f[0-9a-f]{5}|P[0-9A-F]{5}`
bool nm_place_name(std::string_view n) {
    if (nm_address_name(n)) return true;
    return n.size() == 6 && n[0] == 'P' && std::all_of(n.begin() + 1, n.end(), [](char c) { return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F'); });
}

const x86::Decoder16& nm_decoder(project::Project& project) {
    const x86::Decoder16* d = project.services().decoder;
    return d ? *d : x86::capstone_decoder();
}

// The text of an operand with every address of three or more hex digits written N.
std::string nm_mask(std::string_view ops) {
    std::string out;
    for (std::size_t i = 0; i < ops.size();) {
        if (ops.substr(i, 2) == "0x") {
            std::size_t e = i + 2;
            while (e < ops.size() && nm_hex(ops[e], false)) ++e;
            if (e - (i + 2) >= 3) {
                out += 'N';
                i = e;
                continue;
            }
        }
        out += ops[i++];
    }
    return out;
}

std::string nm_signature(const std::vector<Insn>& code) {
    std::string out;
    for (const Insn& i : code) {
        out += i.masked;
        out += '\x1f';
    }
    return out;
}

std::vector<std::int64_t> nm_calls(const std::vector<Insn>& code) {
    std::vector<std::int64_t> out;
    for (const Insn& i : code)
        if (i.call) out.push_back(*i.call);
    return out;
}

// the name of function `b` of the other version, or the one its address makes
std::string nm_called(const Known& other, std::int64_t b) {
    const std::string* n = other.code.find(b);
    return n && !n->empty() ? *n : std::format("f{:05x}", b);
}

}  // namespace

Known known(project::Project& project, const std::optional<std::set<std::int64_t>>& starts) {
    Known out;
    std::set<std::int64_t> at;
    if (starts) {
        at = *starts;
    } else {
        for (const auto& [a, length] : functions(project)) at.insert(a);
    }
    for (const auto& [n, place] : project.symbols()) {
        if (n.find_first_of(":@") != std::string::npos) continue;
        if (place.kind == SymKind::Code || (place.kind == SymKind::Unknown && at.contains(place.address))) out.code.try_emplace(place.address, n);
        else if (place.kind == SymKind::Data || place.kind == SymKind::Unknown) out.data.try_emplace(place.address, n);
    }
    const project::Config& cfg = project.config();
    const std::pair<const char*, const std::optional<std::string>*> files[] = {{"place_by", &cfg.place_by}, {"names", &cfg.names}};
    for (const auto& [key, file] : files) {
        if (!*file) continue;
        const std::filesystem::path p = std::string_view(key) == "place_by" ? project.path(**file) : project.dir() / **file;
        std::error_code ec;
        if (!std::filesystem::exists(p, ec)) continue;
        const std::string contents = fs::read_text(p);
        for (std::string_view line : text::lines(contents)) {
            const std::vector<std::string_view> c = text::split_ws(line.substr(0, line.find('#')));
            if (c.size() < 2) continue;
            const std::pair<std::string_view, std::string_view> readings[] = {{c[1], c[0]}, {c[0], c[1]}};   // (the name, then the place; or the other way)
            for (const auto& [x, n] : readings) {
                if (const auto a = nm_code_name(x); a && !nm_code_name(n)) {
                    out.code.try_emplace(*a, std::string(n));
                    break;
                }
                if (const auto a = nm_data_name(x); a && !nm_data_name(n)) {
                    out.data.try_emplace(*a, std::string(n));
                    break;
                }
            }
        }
    }
    return out;
}

std::vector<std::pair<std::int64_t, std::int64_t>> functions(project::Project& project) {
    std::vector<std::pair<std::int64_t, std::int64_t>> fs;
    if (project.services().carve) {
        try {   // (a toolchain that cannot be carved is one without a carver, as far as naming goes: the places named are used)
            for (const project::CodeFunction& f : project.functions()) fs.emplace_back(f.address, f.length);
        } catch (const Error& e) {
            log::debug(std::format("names: no functions carved: {}", e.what()));
            fs.clear();
        }
    }
    if (fs.empty()) {   // (no carver for the toolchain: the places named)
        const Known k = known(project, std::set<std::int64_t>{});
        std::vector<std::int64_t> starts;
        for (const auto& [a, n] : k.code) starts.push_back(a);
        std::sort(starts.begin(), starts.end());
        for (std::size_t i = 0; i < starts.size(); ++i) {
            const std::int64_t end = i + 1 < starts.size() ? starts[i + 1] : std::min<std::int64_t>(static_cast<std::int64_t>(project.image().size()), starts[i] + 0x400);
            fs.emplace_back(starts[i], end - starts[i]);
        }
    }
    return fs;
}

std::vector<Insn> read(project::Project& project, std::int64_t a, std::int64_t length) {
    std::vector<Insn> out;
    for (const x86::Insn& i : x86::disasm(project.image(), static_cast<std::uint32_t>(a), static_cast<std::uint32_t>(a + length), false, nm_decoder(project))) {
        std::string ops = i.op_str;
        Insn row;
        if (i.mnemonic == "call" && ops.size() > 2 && ops.starts_with("0x") && std::all_of(ops.begin() + 2, ops.end(), [](char c) { return nm_hex(c, false); })) {
            row.call = std::stoll(ops, nullptr, 16);
            ops = "T";
        } else if (i.mnemonic.starts_with('j') || i.mnemonic.starts_with("loop")) {
            ops = "J";   // (a jump within the function: its shape is the code)
        }
        for (std::size_t open = ops.find('['); open != std::string::npos; open = ops.find('[', open + 1)) {   // `[0xNNNN]`: a data word
            const std::size_t close = ops.find(']', open);
            if (close == std::string::npos) break;
            const std::string_view inside = std::string_view(ops).substr(open + 1, close - open - 1);
            if (inside.size() > 2 && inside.starts_with("0x") && std::all_of(inside.begin() + 2, inside.end(), [](char c) { return nm_hex(c, false); })) {
                row.data = std::stoll(std::string(inside), nullptr, 16);
                break;
            }
        }
        row.masked = i.mnemonic + " " + nm_mask(ops);
        out.push_back(std::move(row));
    }
    return out;
}

Carried carry(project::Project& project, project::Project& other_project) {
    std::map<std::int64_t, std::vector<Insn>> mine, theirs;
    std::set<std::int64_t> mine_starts, their_starts;   // (carving is the costly part: once for each program)
    for (const auto& [a, n] : functions(project)) {
        mine[a] = read(project, a, n);
        mine_starts.insert(a);
    }
    for (const auto& [a, n] : functions(other_project)) {
        theirs[a] = read(other_project, a, n);
        their_starts.insert(a);
    }
    const Known other = known(other_project, their_starts);
    const Known mine_known = known(project, mine_starts);

    OrderedMap<std::string, std::vector<std::int64_t>> by_m, by_t;   // the masked code -> the functions that have it, in the order met
    for (const auto& [a, r] : mine) by_m.try_emplace(nm_signature(r)).first->second.push_back(a);
    for (const auto& [a, r] : theirs) by_t.try_emplace(nm_signature(r)).first->second.push_back(a);

    Carried out;
    OrderedMap<std::int64_t, std::int64_t>& pairs = out.pairs;
    std::set<std::int64_t> taken;   // the other's functions that are paired
    OrderedMap<std::int64_t, std::string> how;
    auto pair_up = [&](std::int64_t a, std::int64_t b, std::string why) {
        pairs.insert_or_assign(a, b);
        taken.insert(b);
        how.insert_or_assign(a, std::move(why));
    };

    for (const auto& [k, ms] : by_m) {   // 1. the same code, once on each side
        const std::vector<std::int64_t>* ts = by_t.find(k);
        const std::size_t instructions = static_cast<std::size_t>(std::count(k.begin(), k.end(), '\x1f'));
        if (ms.size() == 1 && ts != nullptr && ts->size() == 1 && instructions >= 4) pair_up(ms[0], (*ts)[0], "the same code");
    }
    for (const auto& [a, n] : mine_known.code) {   // (anchors: what both name alike -- the library's)
        std::optional<std::int64_t> b;
        for (const auto& [ob, on] : other.code)
            if (on == n) {
                b = ob;
                break;
            }
        if (mine.contains(a) && b && theirs.contains(*b) && !pairs.contains(a) && !taken.contains(*b) && !nm_address_name(n)) pair_up(a, *b, "named so on both sides");
    }

    OrderedMap<std::int64_t, std::int64_t>& dpairs = out.data_pairs;
    const auto spread = [&] {
        std::vector<std::pair<std::int64_t, std::int64_t>> work;   // 2. calls and data at the same places (the last found, first)
        for (const auto& [a, b] : pairs) work.emplace_back(a, b);
        while (!work.empty()) {
            const auto [a, b] = work.back();
            work.pop_back();
            const std::vector<Insn>& ra = mine.at(a);
            const std::vector<Insn>& rb = theirs.at(b);
            const bool same = nm_signature(ra) == nm_signature(rb);
            const std::vector<std::int64_t> ca_ = nm_calls(ra), cb_ = nm_calls(rb);
            if (same || ca_.size() == cb_.size()) {   // (the same code, or as many calls in it)
                for (std::size_t i = 0; i < std::min(ca_.size(), cb_.size()); ++i) {
                    const std::int64_t ca = ca_[i], cb = cb_[i];
                    const auto mc = mine.find(ca);
                    const auto tc = theirs.find(cb);
                    if (mc == mine.end() || tc == theirs.end()) continue;
                    // (the callees alike: a caller of the same code may call another function there in this version)
                    const bool alike = same || (nm_calls(mc->second).size() == nm_calls(tc->second).size() &&
                                                [&] {
                                                    const double ratio = static_cast<double>(mc->second.size()) / static_cast<double>(std::max<std::size_t>(tc->second.size(), 1));
                                                    return 0.8 <= ratio && ratio <= 1.25;
                                                }());
                    if (alike && !pairs.contains(ca) && !taken.contains(cb)) {
                        pair_up(ca, cb, "called where " + nm_called(other, b) + " calls it");
                        work.emplace_back(ca, cb);
                    }
                }
            }
            if (same)
                for (std::size_t i = 0; i < std::min(ra.size(), rb.size()); ++i)
                    if (ra[i].data && rb[i].data && !dpairs.contains(*ra[i].data)) dpairs.insert_or_assign(*ra[i].data, *rb[i].data);
        }
    };
    spread();
    while (true) {   // 3. who calls the paired ones, in the same order
        std::set<std::int64_t> inv = taken;
        OrderedMap<std::string, std::vector<std::int64_t>> sig_m, sig_t;   // the callees as they pair, by the function that has them
        for (const auto& [a, r] : mine) {
            if (pairs.contains(a)) continue;
            std::string s;
            int paired = 0;
            for (const std::int64_t c : nm_calls(r)) {
                const std::int64_t* b = pairs.find(c);
                s += b ? std::to_string(*b) : "?";
                s += ',';
                paired += b != nullptr;
            }
            if (paired >= 2) sig_m.try_emplace(s).first->second.push_back(a);
        }
        for (const auto& [b, r] : theirs) {
            if (inv.contains(b)) continue;
            std::string s;
            int paired = 0;
            for (const std::int64_t c : nm_calls(r)) {
                s += inv.contains(c) ? std::to_string(c) : "?";
                s += ',';
                paired += inv.contains(c);
            }
            if (paired >= 2) sig_t.try_emplace(s).first->second.push_back(b);
        }
        std::vector<std::pair<std::int64_t, std::int64_t>> added;
        for (const auto& [s, ms] : sig_m)
            if (const auto* ts = sig_t.find(s); ms.size() == 1 && ts != nullptr && ts->size() == 1) added.emplace_back(ms[0], (*ts)[0]);
        if (added.empty()) break;
        for (const auto& [a, b] : added) pair_up(a, b, "calls what " + nm_called(other, b) + " calls, in its order");
        spread();
    }

    // the names: sorted by place, a name once, none that an address makes, and not over a name this project has
    std::set<std::string> used;
    std::vector<std::pair<std::int64_t, std::int64_t>> sorted(pairs.begin(), pairs.end());
    std::sort(sorted.begin(), sorted.end());
    for (const auto& [a, b] : sorted) {
        const std::string* n = other.code.find(b);
        if (n == nullptr || n->empty() || nm_place_name(*n)) continue;   // (an address-name is no name to carry)
        const std::string* have = mine_known.code.find(a);
        const bool named_otherwise = have != nullptr && *have != *n && !nm_address_name(*have);
        if (!used.contains(*n) && !named_otherwise) {
            out.code[a] = {*n, how.at(a)};
            used.insert(*n);
        }
    }
    std::set<std::string> dused;
    std::vector<std::pair<std::int64_t, std::int64_t>> dsorted(dpairs.begin(), dpairs.end());
    std::sort(dsorted.begin(), dsorted.end());
    for (const auto& [a, b] : dsorted) {
        const std::string* n = other.data.find(b);
        if (n != nullptr && !n->empty() && !dused.contains(*n)) {
            out.data[a] = {*n, "named where the same code names it"};
            dused.insert(*n);
        }
    }
    return out;
}

int run(project::Project& project, const std::vector<std::string>& args) {
    const auto from = std::find(args.begin(), args.end(), "--from");
    if (from == args.end() || from + 1 == args.end()) {
        out("names: --from OTHER (a project of another version of the program, with names)\n");
        return 1;
    }
    const std::filesystem::path given = fs::from_text(*(from + 1));
    const std::shared_ptr<project::Project> other = project::Project::open(std::filesystem::absolute(given), project.services_ptr());
    const Carried c = carry(project, *other);
    const Known mine = known(project);
    std::int64_t agree = 0;
    std::vector<std::tuple<std::int64_t, std::string, std::string>> clash;
    for (const auto& [a, named] : c.code) {
        const std::string* have = mine.code.find(a);
        if (have != nullptr && *have == named.name) ++agree;
        if (have != nullptr && *have != named.name && !nm_address_name(*have)) clash.emplace_back(a, named.name, *have);
    }
    const bool verbose = std::find(args.begin(), args.end(), "-v") != args.end();
    if (verbose)
        for (const auto& [a, named] : c.code) {
            const std::string* have = mine.code.find(a);
            out("  f{:05x} {:<24} {}{}\n", a, named.name, named.how, have == nullptr ? "" : "  (has: " + *have + ")");
        }
    out("{} functions named ({} of them as the project names them already, {} it names otherwise), {} data\n", c.code.size(), agree, clash.size(), c.data.size());
    for (std::size_t i = 0; i < clash.size() && i < 10; ++i)
        out("   f{:05x}: {} here, {} by the other\n", std::get<0>(clash[i]), std::get<2>(clash[i]), std::get<1>(clash[i]));
    if (std::find(args.begin(), args.end(), "--write") != args.end()) {
        SymbolTable s = project.symbols();
        for (const auto& [a, named] : c.code) s.try_emplace(named.name, SymPlace{SymKind::Code, a});
        for (const auto& [a, named] : c.data) s.try_emplace(named.name, SymPlace{SymKind::Data, a});
        project.save_symbols(s);
        out("written to {}\n", fs::to_text(project.path("SYMBOLS")));
    }
    return 0;
}

}  // namespace cb::exact::names
