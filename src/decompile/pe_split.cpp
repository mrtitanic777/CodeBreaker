// decompile/pe_split.cpp -- a 32-bit program's functions as source files (pe_split.hpp).
#include "decompile/pe_split.hpp"

#include <algorithm>
#include <format>
#include <limits>
#include <map>
#include <tuple>
#include <unordered_map>

#include "core/text.hpp"

namespace cb::decompile::pe_split {

namespace {

constexpr std::size_t kLongestRun = 512;      // no object of the period has more functions in a row than this
constexpr std::size_t kMostSharers = 6;       // data more functions than this refer to says nothing of a file
constexpr double kCallWeight = 2.0;
constexpr double kNeighbourData = 0.5;        // two functions' private data side by side
constexpr Va kNeighbourGap = 64;
constexpr double kNextDoor = 0.05;            // (so that a function nothing ties stays with its neighbours)

std::uint32_t ps_u32(ByteView d, std::size_t at) {
    return static_cast<std::uint32_t>(d[at]) | static_cast<std::uint32_t>(d[at + 1]) << 8 | static_cast<std::uint32_t>(d[at + 2]) << 16 |
           static_cast<std::uint32_t>(d[at + 3]) << 24;
}

std::uint16_t ps_u16(ByteView d, std::size_t at) {
    return static_cast<std::uint16_t>(d[at] | d[at + 1] << 8);
}

std::string ps_leaf(std::string_view path) {
    const std::size_t slash = path.find_last_of("/\\");
    return std::string(slash == std::string_view::npos ? path : path.substr(slash + 1));
}

// `foo.obj` as the C file it was compiled from.
std::string ps_c_name(std::string_view object) {
    std::string name = text::lower(ps_leaf(object));
    const std::size_t dot = name.rfind('.');
    if (dot != std::string::npos) name.resize(dot);
    return name + ".c";
}

}  // namespace

std::vector<Module> codeview_modules(const formats::pe::Image& image) {
    std::vector<Module> out;
    const auto d = image.data();
    const auto sections = image.sections();
    const std::uint64_t base = image.header().image_base;
    for (const formats::pe::DebugEntry& e : image.debug_entries()) {
        if (e.type != 2 || e.offset + 8ull > d.size()) continue;
        const std::size_t cv = e.offset;
        const std::string_view sig(reinterpret_cast<const char*>(d.data() + cv), 4);
        if (sig != "NB09" && sig != "NB11") continue;
        const std::size_t dir = cv + ps_u32(d, cv + 4);
        if (dir + 8 > d.size()) continue;
        const std::size_t header = ps_u16(d, dir), entry = ps_u16(d, dir + 2), count = ps_u32(d, dir + 4);
        for (std::size_t k = 0; k < count; ++k) {
            const std::size_t at = dir + header + entry * k;
            if (at + 12 > d.size()) break;
            if (ps_u16(d, at) != 0x120) continue;   // (sstModule)
            const std::size_t m = cv + ps_u32(d, at + 4);
            if (m + 8 > d.size()) continue;
            const std::size_t segments = ps_u16(d, m + 4);
            const std::size_t name_at = m + 8 + 12 * segments;
            if (name_at >= d.size() || name_at + 1 + d[name_at] > d.size()) continue;
            Module module;
            module.name.assign(reinterpret_cast<const char*>(d.data() + name_at + 1), d[name_at]);
            for (std::size_t s = 0; s < segments; ++s) {
                const std::size_t info = m + 8 + 12 * s;
                const std::size_t segment = ps_u16(d, info);
                const std::uint32_t offset = ps_u32(d, info + 4), size = ps_u32(d, info + 8);
                if (segment == 0 || segment > sections.size() || size == 0 || !sections[segment - 1].is_code()) continue;
                module.code.emplace_back(static_cast<Va>(base + sections[segment - 1].virtual_address + offset), size);
            }
            if (!module.code.empty()) out.push_back(std::move(module));
        }
    }
    return out;
}

bool runtime_module(std::string_view name) {
    const std::string n = text::lower(name);
    if (n.find("\\obj\\") != std::string::npos || n.find("/obj/") != std::string::npos) return true;   // (how the C library's are named)
    for (const std::string_view end : {".dll", ".exp", ".lib"})
        if (n.ends_with(end)) return true;
    const std::string leaf = ps_leaf(n);
    for (const std::string_view start : {"crtexe", "crtdll", "dllcrt", "dllmain", "dllargv", "atonexit", "fp8"})
        if (leaf.starts_with(start)) return true;
    return false;
}

std::vector<Function> functions_of(const formats::pe_functions::PeCarve& carve,
                                   std::span<const std::tuple<Va, std::string, std::string>> named) {
    std::vector<Function> out;
    out.reserve(named.size());
    for (const auto& [va, name, body] : named) out.push_back({va, name, body, {}, {}, false});
    std::sort(out.begin(), out.end(), [](const Function& a, const Function& b) { return a.start < b.start; });
    std::unordered_map<Va, std::size_t> index;
    for (std::size_t i = 0; i < out.size(); ++i) index[out[i].start] = i;
    const auto is_data = [&](Va v) {
        const std::uint64_t base = carve.image().header().image_base;
        for (const formats::pe::Section& s : carve.image().sections()) {
            if (s.is_code() || s.name == ".rsrc" || s.name == ".reloc" || s.name == ".idata" || s.name == ".edata") continue;
            const std::uint64_t at = base + s.virtual_address;
            if (v >= at && v < at + std::max(s.virtual_size, s.raw_size)) return true;
        }
        return false;
    };
    const auto& refs = carve.data_refs();
    const auto addresses = carve.instruction_addresses();
    for (std::size_t i = 0; i < out.size(); ++i) {
        Function& f = out[i];
        if (const auto found = refs.find(f.start); found != refs.end())
            for (const Va v : found->second)
                if (is_data(v)) f.data.push_back(v);
        const auto carved = carve.functions().find(f.start);
        if (carved == carve.functions().end()) continue;
        const Va end = carved->second.end;
        auto at = std::lower_bound(addresses.begin(), addresses.end(), f.start);
        bool first = true;
        for (; at != addresses.end() && *at < end; ++at, first = false) {
            const auto instruction = carve.instruction_at(*at);
            if (!instruction) continue;
            if (first && instruction->mnemonic == "jmp" && instruction->operands.size() == 1 &&
                instruction->operands[0].kind == formats::pe_functions::OperandKind::Memory && instruction->operands[0].base == 0 &&
                carve.imports().contains(static_cast<Va>(instruction->operands[0].disp)))
                f.stub = true;
            if (const auto target = instruction->call_target())
                if (const auto callee = index.find(*target); callee != index.end()) f.calls.push_back(callee->second);
        }
    }
    return out;
}

// The partition of the functions into runs that has the highest modularity (edges inside the runs, against what the
// functions' degrees would put there by chance). The best partition into contiguous runs is exact, by dynamic programming.
std::vector<std::size_t> boundaries(std::span<const Function> functions) {
    const std::size_t n = functions.size();
    if (n < 2) return {};
    std::map<std::pair<std::size_t, std::size_t>, double> weight;
    const auto tie = [&](std::size_t a, std::size_t b, double w) {
        if (a != b) weight[{std::min(a, b), std::max(a, b)}] += w;
    };
    std::map<Va, std::vector<std::size_t>> users;
    std::vector<std::size_t> callers(n, 0);
    for (std::size_t i = 0; i < n; ++i) {
        std::vector<Va> data = functions[i].data;
        std::sort(data.begin(), data.end());
        data.erase(std::unique(data.begin(), data.end()), data.end());
        for (const Va v : data) users[v].push_back(i);
        std::vector<std::size_t> calls = functions[i].calls;
        std::sort(calls.begin(), calls.end());
        calls.erase(std::unique(calls.begin(), calls.end()), calls.end());
        for (const std::size_t c : calls) ++callers[c];
    }
    for (const auto& [v, who] : users) {
        if (who.size() < 2 || who.size() > kMostSharers) continue;
        const double w = 1.0 / static_cast<double>(who.size() - 1);
        for (std::size_t x = 0; x < who.size(); ++x)
            for (std::size_t y = x + 1; y < who.size(); ++y) tie(who[x], who[y], w);
    }
    for (std::size_t i = 0; i < n; ++i) {
        std::vector<std::size_t> calls = functions[i].calls;
        std::sort(calls.begin(), calls.end());
        calls.erase(std::unique(calls.begin(), calls.end()), calls.end());
        for (const std::size_t c : calls) tie(i, c, kCallWeight / static_cast<double>(callers[c]));
    }
    const std::vector<std::size_t>* previous = nullptr;
    Va previous_at = 0;
    for (const auto& [v, who] : users) {
        if (who.size() > 2) continue;
        if (previous != nullptr && v - previous_at <= kNeighbourGap)
            for (const std::size_t x : who)
                for (const std::size_t y : *previous) tie(x, y, kNeighbourData);
        previous = &who;
        previous_at = v;
    }
    for (std::size_t i = 0; i + 1 < n; ++i) tie(i, i + 1, kNextDoor);

    double total = 0;
    std::vector<double> degree(n, 0.0);
    std::vector<std::vector<std::pair<std::size_t, double>>> earlier(n);   // the edges of each function to those before it
    for (const auto& [ends, w] : weight) {
        total += w;
        degree[ends.first] += w;
        degree[ends.second] += w;
        earlier[ends.second].emplace_back(ends.first, w);
    }
    std::vector<double> before(n + 1, 0.0);
    for (std::size_t i = 0; i < n; ++i) before[i + 1] = before[i] + degree[i];
    std::vector<double> best(n + 1, -std::numeric_limits<double>::infinity());
    std::vector<std::size_t> from(n + 1, 0);
    std::vector<double> into(n, 0.0);   // edges from k to anything in [k, j)
    best[0] = 0;
    for (std::size_t j = 1; j <= n; ++j) {
        for (const auto& [x, w] : earlier[j - 1]) into[x] += w;
        double inside = 0;
        const std::size_t lowest = j > kLongestRun ? j - kLongestRun : 0;
        for (std::size_t k = j; k-- > lowest;) {
            inside += into[k];
            const double share = (before[j] - before[k]) / (2 * total);
            const double score = best[k] + inside / total - share * share;
            if (score > best[j]) best[j] = score, from[j] = k;
        }
    }
    std::vector<std::size_t> cuts;
    for (std::size_t j = n; j > 0; j = from[j])
        if (from[j] > 0) cuts.push_back(from[j]);
    std::reverse(cuts.begin(), cuts.end());
    return cuts;
}

Split plan(std::span<const Function> functions, std::span<const Module> modules, const std::set<Va>& runtime, std::string_view stem) {
    Split out;
    const std::string base(stem);
    std::vector<std::size_t> library;
    const auto keep = [&](std::string file, Tier tier, std::vector<std::size_t> members) {
        if (!members.empty()) out.units.push_back({std::move(file), tier, std::move(members)});
    };
    std::vector<std::size_t> own;
    std::map<std::string, std::vector<std::size_t>> placed;
    std::vector<std::string> placed_order;
    for (std::size_t i = 0; i < functions.size(); ++i) {
        const Function& f = functions[i];
        const Module* module = nullptr;
        for (const Module& m : modules) {
            for (const auto& [at, size] : m.code)
                if (f.start >= at && f.start - at < size) module = &m;
            if (module != nullptr) break;
        }
        if (f.stub || runtime.contains(f.start) || (module != nullptr && runtime_module(module->name))) {
            library.push_back(i);
        } else if (module != nullptr) {
            const std::string file = ps_c_name(module->name);
            if (!placed.contains(file)) placed_order.push_back(file);
            placed[file].push_back(i);
        } else {
            own.push_back(i);
        }
    }
    for (const std::string& file : placed_order) keep(file, Tier::Proven, placed[file]);
    if (!own.empty()) {
        std::vector<Function> rest;
        rest.reserve(own.size());
        std::unordered_map<std::size_t, std::size_t> position;
        for (std::size_t k = 0; k < own.size(); ++k) position[own[k]] = k;
        for (const std::size_t i : own) {
            Function f = functions[i];
            std::vector<std::size_t> calls;
            for (const std::size_t c : f.calls)
                if (const auto found = position.find(c); found != position.end()) calls.push_back(found->second);
            f.calls = std::move(calls);
            rest.push_back(std::move(f));
        }
        std::vector<std::size_t> cuts = boundaries(rest);
        cuts.push_back(own.size());
        std::size_t from = 0;
        const bool alone = cuts.size() == 1 && out.units.empty();
        for (std::size_t c = 0; c < cuts.size(); ++c) {
            const std::vector<std::size_t> members(own.begin() + static_cast<std::ptrdiff_t>(from), own.begin() + static_cast<std::ptrdiff_t>(cuts[c]));
            keep(alone ? base + ".c" : std::format("{}_part{:02}.c", base, c + 1), Tier::BestGuess, members);
            from = cuts[c];
        }
    }
    keep(base + "_runtime.c", Tier::Runtime, std::move(library));
    return out;
}

std::vector<std::pair<std::string, std::string>> source_files(std::string_view stem, std::string_view common, const Split& split,
                                                              std::span<const Function> functions) {
    std::vector<std::pair<std::string, std::string>> out;
    const std::string base(stem);
    const std::string guard = "CODEBREAKER_" + text::upper(base) + "_H";
    const std::size_t title_end = common.find('\n');
    const std::string_view title = title_end == std::string_view::npos ? std::string_view() : common.substr(0, title_end + 1);
    const std::string_view rest = title_end == std::string_view::npos ? common : common.substr(title_end + 1);
    out.emplace_back(base + ".h", std::format("{}#ifndef {}\n#define {}\n{}\n#endif\n", title, guard, guard, rest));
    std::string manifest = std::format("{}: its source files, and how far each is known.\n\n"
                                       "proven      the program's own debug information names this object and its code\n"
                                       "best guess  neighbouring functions that call and share data most; NOT the original's file\n"
                                       "runtime     import stubs and the compiler's library\n\n",
                                       base);
    for (const Unit& unit : split.units) {
        const char* tier = unit.tier == Tier::Proven ? "proven" : unit.tier == Tier::Runtime ? "runtime" : "best guess";
        const char* said = unit.tier == Tier::Proven       ? "named by the program's debug information"
                           : unit.tier == Tier::Runtime    ? "import stubs and the compiler's library"
                                                           : "a best guess at a source file: its name and its edges are not the original's";
        std::string body = std::format("/* {} -- {} */\n\n#include \"{}.h\"\n", unit.file, said, base);
        manifest += std::format("{:<10}  {:<24} {:>5} functions  {:x}\n", tier, unit.file, unit.functions.size(),
                                unit.functions.empty() ? 0 : functions[unit.functions.front()].start);
        for (const std::size_t i : unit.functions) body += "\n" + std::string(text::trim(functions[i].text)) + "\n";
        out.emplace_back(unit.file, std::move(body));
    }
    out.emplace_back("SOURCE_FILES.txt", std::move(manifest));
    return out;
}

}  // namespace cb::decompile::pe_split
