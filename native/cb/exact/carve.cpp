#include "cb/exact/carve.hpp"

#include <algorithm>
#include <format>
#include <map>
#include <set>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/regex.hpp"

namespace cb::exact::carve {

namespace {

constexpr std::uint8_t kCarveRet = 0xC3, kCarveRetf = 0xCB, kCarveRetN = 0xC2, kCarveRetfN = 0xCA, kCarveNop = 0x90;

bool carve_hex_digit(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }

// A signed word of the image.
std::int16_t carve_rel(ByteView img, std::size_t k) {
    if (k + 2 > img.size()) throw TruncatedError("exact carve", k, 2, img.size() > k ? img.size() - k : 0);
    return static_cast<std::int16_t>(img[k] | (img[k + 1] << 8));
}

// The library part's answer for the project, as the project's view asks it.
library::Map carve_locate(project::Project& project) {
    const x86::Decoder16* decoder = project.services().decoder;
    return library::locate(*project.as_library(), decoder ? *decoder : x86::capstone_decoder());
}

}  // namespace

bool Region::inside(std::int64_t a) const {
    if (a < lo || a >= hi) return false;
    return std::none_of(skip.begin(), skip.end(), [a](const auto& r) { return r.first <= a && a < r.second; });
}

Region region(project::Project& project, const library::Map* map) {
    std::optional<library::Map> own;
    if (map == nullptr) {
        own = carve_locate(project);
        map = &*own;
    }
    Region r;
    r.hi = map->start ? *map->start : project.dgroup();
    r.lo = project.config().code_start;
    for (const auto* group : {&map->coded, &map->lone})
        for (const library::Placed& p : *group)
            if (p.code && *p.code < r.hi) r.skip.emplace_back(*p.code, *p.end());   // (a placed module with code has an end)
    for (const library::Map::KitObject& o : map->objects) r.skip.emplace_back(o.start, o.end);
    std::sort(r.skip.begin(), r.skip.end());
    return r;
}

std::string hex_pattern(std::string_view pattern) {
    std::string s;   // (the blanks are only for the eye)
    for (char c : pattern)
        if (c != ' ') s += c;
    std::string out;
    for (std::size_t k = 0; k < s.size();) {
        if (s.compare(k, 2, "..") == 0) {
            out += '.';
            k += 2;
        } else if (k + 1 < s.size() && carve_hex_digit(s[k]) && carve_hex_digit(s[k + 1])) {
            out += std::format("\\x{}{}", s[k], s[k + 1]);
            k += 2;
        } else {
            out += s[k++];
        }
    }
    return out;
}

std::vector<Function> functions(project::Project& project, const library::Map* map) {
    std::optional<library::Map> own;
    if (map == nullptr) {
        own = carve_locate(project);
        map = &*own;
    }
    const ByteView img = project.image();
    const Region where = region(project, map);
    const std::int64_t lo = where.lo, hi = where.hi;
    const project::CarveSpec& spec = project.profile_parts()->carve;

    // the places the program's calls to its library's routines name: names the library's modules define, then SYMBOLS' code
    std::map<std::string, std::int64_t> names;
    for (const library::Placed& p : map->coded) {
        std::vector<const library::Member*> modules{p.module};
        modules.insert(modules.end(), p.also.begin(), p.also.end());
        for (const library::Member* m : modules)
            for (const auto& [name, place] : m->pubs)
                if (place.segment && !m->code.empty() && *place.segment == m->code.front().segment) names.emplace(name, *p.code + place.offset);
    }
    for (const auto& [name, place] : project.symbols())
        if (place.kind == SymKind::Code) names.emplace(name, place.address);

    std::set<std::int64_t> starts;
    std::map<std::int64_t, std::string> why;
    const std::int64_t end_of_image = static_cast<std::int64_t>(img.size());
    const std::int64_t from = std::min(lo, end_of_image), to = std::min(hi, end_of_image);

    std::optional<std::int64_t> target;
    if (spec.prologue_call)
        if (const auto n = names.find(*spec.prologue_call); n != names.end()) target = n->second;
    for (const std::string& rx : spec.prologues) {
        // (the patterns are the profile's data: compiled once each, here, not per match)
        const Regex re(hex_pattern(rx), RegexFlags::DotAll);
        const std::string_view region_bytes(reinterpret_cast<const char*>(img.data()) + from, static_cast<std::size_t>(std::max<std::int64_t>(0, to - from)));
        re.for_each_match(region_bytes, [&](const RegexMatch& m) {
            const std::int64_t a = from + static_cast<std::int64_t>(m.start());
            if (!where.inside(a)) return true;
            if (m.group_count() > 1 && m.has(1) && target) {   // group 1 is a near call's displacement: the call must go to the routine
                const std::int64_t k = from + static_cast<std::int64_t>(m.start(1));
                if (((k + 2 + carve_rel(img, static_cast<std::size_t>(k))) & 0xFFFF) != (*target & 0xFFFF)) return true;
            }
            starts.insert(a);
            why.emplace(a, "prologue");
            return true;
        });
    }
    if (spec.frames) {   // push bp / mov bp,sp after a return or a pad: a function compiled without the stack probe, or by hand
        constexpr std::uint8_t kFrame[3] = {0x55, 0x8B, 0xEC};
        for (std::int64_t a = 0; a + 3 <= to; ++a) {
            if (!std::equal(kFrame, kFrame + 3, img.begin() + a)) continue;
            if (a < lo || !where.inside(a)) continue;
            const auto before = [&](std::int64_t k) { return img[static_cast<std::size_t>(a - k)]; };
            if (a == lo || before(1) == kCarveRet || before(1) == kCarveRetf || before(1) == 0x00 || before(1) == kCarveNop || (a >= 3 && (before(3) == kCarveRetN || before(3) == kCarveRetfN))) {
                starts.insert(a);
                why.emplace(a, "frame after a return");
            }
        }
    }
    for (std::uint32_t a : project.config().starts_add) starts.insert(a);

    if (spec.calls) {   // near calls' targets, from the code disassembled from each known start (until a new one is not found)
        const x86::Decoder16* decoder = project.services().decoder;
        std::set<std::int64_t> done;
        while (true) {
            std::vector<std::int64_t> todo;
            for (std::int64_t a : starts)
                if (!done.contains(a)) todo.push_back(a);
            if (todo.empty()) break;
            const std::vector<std::int64_t> all(starts.begin(), starts.end());   // (the starts as they stood when this round began)
            for (std::int64_t a : todo) {
                done.insert(a);
                const auto next = std::upper_bound(all.begin(), all.end(), a);
                const std::int64_t nxt = std::min(next == all.end() ? hi : *next, hi);
                const std::uint32_t base = project.code_base(static_cast<std::uint32_t>(a));
                for (const x86::Insn& i : x86::body(img, static_cast<std::uint32_t>(a), static_cast<std::uint32_t>(nxt), spec.emulator_ints, base,
                                                    decoder ? *decoder : x86::capstone_decoder())) {
                    if (i.mnemonic != "call" || i.size() != 3 || i.bytes[0] != 0xE8) continue;
                    const std::int64_t ibase = project.code_base(i.address);
                    const std::int64_t t = ibase + ((static_cast<std::int64_t>(i.address) + 3 + carve_rel(img, i.address + 1) - ibase) & 0xFFFF);
                    if (where.inside(t) && !starts.contains(t)) {
                        starts.insert(t);
                        why.emplace(t, std::format("called from {:05x}", i.address));
                    }
                }
            }
        }
    }
    for (std::uint32_t a : project.config().starts_drop) starts.erase(a);

    // each function ends where the next begins, where the library's part or a kit object does, or where the program's own code ends
    std::set<std::int64_t> ends{hi};
    for (auto it = starts.begin(); it != starts.end(); ++it)
        if (it != starts.begin()) ends.insert(*it);
    for (const auto& [x, y] : where.skip)
        if (x > lo) ends.insert(x);
    std::vector<Function> out;
    for (std::int64_t a : starts) {
        const auto next = ends.upper_bound(a);
        if (next == ends.end()) throw FormatError("exact carve", static_cast<std::uint64_t>(a), "a function starts past where the program's own code ends");
        std::int64_t e = *next, z = e;
        while (z > a + 1 && img[static_cast<std::size_t>(z - 1)] == 0) --z;
        if (z < e && (img[static_cast<std::size_t>(z - 1)] == kCarveRet || img[static_cast<std::size_t>(z - 1)] == kCarveRetf ||
                      (z - a >= 3 && (img[static_cast<std::size_t>(z - 3)] == kCarveRetN || img[static_cast<std::size_t>(z - 3)] == kCarveRetfN))))
            e = z;   // (zeros after the return: a pad to the next module)
        const auto w = why.find(a);
        out.push_back({static_cast<std::uint32_t>(a), static_cast<std::uint32_t>(e - a), w == why.end() ? std::string() : w->second});
    }
    return out;
}

std::function<std::vector<project::CodeFunction>(project::Project&)> service() {
    return [](project::Project& project) {
        std::vector<project::CodeFunction> out;
        for (const Function& f : functions(project)) out.push_back({f.address, f.length});
        return out;
    };
}

int run(project::Project& project, const std::vector<std::string>& args) {
    const bool verbose = std::find(args.begin(), args.end(), "-v") != args.end();
    const std::vector<Function> fs = functions(project);
    std::uint64_t bytes = 0;
    for (const Function& f : fs) {
        bytes += f.length;
        if (verbose) out("{:05x} {:5}  {}\n", f.address, f.length, f.why);
    }
    out("{} functions of the program's own, {} bytes\n", fs.size(), bytes);
    if (std::find(args.begin(), args.end(), "--write") != args.end()) {
        std::string text = "# where the program's own functions begin, and their lengths (codebreaker exact carve)\n";
        for (const Function& f : fs) text += std::format("{:05x} {}  {}\n", f.address, f.length, f.why);
        fs::write_text(project.path("FUNCS"), text);
    }
    return 0;
}

}  // namespace cb::exact::carve
