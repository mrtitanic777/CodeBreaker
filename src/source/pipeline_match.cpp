// `match <32-bit module>`: does a period compiler turn the reconstruction back into the original's bytes?
//
// This is NOT the measure of a reconstruction: that is whether it behaves as the original (`measure`). It is a second, optional signal. Where a
// function's reconstructed C, compiled with a compiler of the original's time (settings: period_compiler, the folder holding CL.EXE), comes out as the
// very bytes the original has, the C is as close to what was written as a binary can show; where it does not, nothing follows -- the same source
// gives different bytes under another compiler version or another switch.
//
// Each reconstructed function is compiled on its own, its bytes are read from the object file with the places the linker fills in masked out, and
// compared with the original function's bytes, masked at the same places (compare/mask):
//
//     identical     every byte outside the masked places is the same
//     same size     as long as the original, some bytes differ (registers, instruction choice)
//     different     another length
//
// `--try` compiles under several sets of switches and reports, per function, the best; which set matches most functions says how the module was built.
// A 16-bit module goes to Microsoft C (pipeline_match16.cpp: verify/authored/match16, under its own options); an Xbox 360 image (its compiler's objects,
// read by bits) to the Xbox 360 compiler (pipeline_ppc.cpp).
#include <algorithm>
#include <map>
#include <mutex>
#include <set>
#include <unordered_map>

#include <nlohmann/json.hpp>
#include "compare/mask.hpp"
#include "verify/authored/repair.hpp"
#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/log.hpp"
#include "core/parallel.hpp"
#include "core/process.hpp"
#include "core/regex.hpp"
#include "core/text.hpp"
#include "formats/pe.hpp"
#include "formats/pe_functions.hpp"
#include "platforms/decoder.hpp"
#include "source/pipeline_match16.hpp"
#include "source/pipeline_period.hpp"
#include "source/pipeline_ppc.hpp"
#include "source/match_bytes.hpp"
#include "source/pipeline_world.hpp"
#include "symbols/libraries/coff.hpp"
#include "verify/lanes/win32.hpp"

namespace cb::source::pipeline {

namespace stdfs = std::filesystem;

namespace {

// The switch sets `--try` goes through (the first is the default).
const std::vector<std::string>& match_flag_sets() {
    static const std::vector<std::string> sets = {"/O2 /Gz", "/Oxs /Gz", "/O1 /Gz", "/Ox /Gz", "/O2 /Gz /Oy-", "/Od /Gz", "/O2 /Gd"};
    return sets;
}

// A function's bytes and which of them a loader or linker fills in (a mask byte is non-zero there).
struct MatchCode {
    Bytes bytes;
    Bytes mask;
};

// Without the padding a compiler puts after a function (int 3, nop, zero bytes).
MatchCode match_trimmed(MatchCode code) {
    std::size_t n = code.bytes.size();
    const auto padding = [](std::uint8_t b) { return b == 0xCC || b == 0x90 || b == 0x00; };
    while (n > 1 && code.mask[n - 1] == 0 && padding(code.bytes[n - 1]) && (code.bytes[n - 2] == 0xC3 || padding(code.bytes[n - 2]))) --n;
    code.bytes.resize(n);
    code.mask.resize(n);
    return code;
}

// The original's functions: {address: bytes and mask}; the mask marks relocated words and the distances of direct calls and jumps, which depend on
// where things are.
std::map<std::uint64_t, MatchCode> match_originals(const formats::pe::Image& pe, const std::vector<std::uint64_t>& addresses) {
    std::map<std::uint64_t, std::uint64_t> stops;   // start -> end
    for (const formats::pe_functions::Function& f : formats::pe_functions::function_spans(pe)) stops[static_cast<std::uint64_t>(f.start)] = static_cast<std::uint64_t>(f.end);
    std::set<std::uint64_t> relocated;
    for (const std::uint64_t rva : pe.base_relocs()) relocated.insert(pe.header().image_base + rva);
    platforms::DecoderSet decoders;
    const platforms::Decoder& decoder = decoders.get(platforms::Arch::X86, 32);
    std::map<std::uint64_t, MatchCode> out;
    for (const std::uint64_t start : addresses) {
        const auto stop = stops.find(start);
        if (stop == stops.end()) continue;
        const std::optional<std::uint64_t> offset = pe.file_offset(start - pe.header().image_base);
        if (!offset || *offset > pe.data().size()) continue;
        const std::size_t length = static_cast<std::size_t>(std::min<std::uint64_t>(stop->second - start, pe.data().size() - *offset));
        MatchCode code;
        code.bytes.assign(pe.data().begin() + static_cast<std::ptrdiff_t>(*offset), pe.data().begin() + static_cast<std::ptrdiff_t>(*offset + length));
        code.mask.assign(length, 0);
        for (std::uint64_t a = start; a < stop->second && a - start < length; ++a)
            if (relocated.contains(a))
                for (std::size_t k = 0; k < 4 && a - start + k < length; ++k) code.mask[a - start + k] = 1;
        for (const platforms::Insn& insn : decoder.disasm(code.bytes, start)) {
            const std::size_t at = static_cast<std::size_t>(insn.address - start);
            if (at < length && (code.bytes[at] == 0xE8 || code.bytes[at] == 0xE9) && insn.size == 5)
                for (std::size_t k = 1; k < 5 && at + k < length; ++k) code.mask[at + k] = 1;
        }
        out[start] = match_trimmed(std::move(code));
    }
    return out;
}

// What the compiler made of a function: its code in the object, by the symbol that is its name (`_name`, `@name@8`, `_name@12` all name `name`).
std::optional<MatchCode> match_object_function(ByteView object, std::string_view name) {
    std::vector<symbols::libraries::Function> found;
    try {
        found = symbols::libraries::coff::functions(object);
    } catch (const Error&) {
        return std::nullopt;
    }
    const auto plain = [](std::string_view symbol) {
        while (!symbol.empty() && (symbol.front() == '_' || symbol.front() == '@')) symbol.remove_prefix(1);
        return symbol.substr(0, symbol.find('@'));
    };
    for (const symbols::libraries::Function& f : found)
        if (plain(f.name) == name) return MatchCode{f.code, f.mask};
    if (found.size() == 1) return MatchCode{found.front().code, found.front().mask};
    return std::nullopt;
}

// The bytes in the matching blocks of two runs: the longest common run (the earliest in a, of those the earliest in b), then the longest of what is left of
// each side of it, and so on.
std::size_t match_blocks(ByteView a, ByteView b, std::size_t alo, std::size_t ahi, std::size_t blo, std::size_t bhi, const std::vector<std::vector<std::size_t>>& at) {
    // the longest run in a[alo:ahi] and b[blo:bhi]: the earliest in a, of those the earliest in b
    std::size_t best_i = alo, best_j = blo, best = 0;
    std::map<std::size_t, std::size_t> previous;
    for (std::size_t i = alo; i < ahi; ++i) {
        std::map<std::size_t, std::size_t> current;
        for (const std::size_t j : at[a[i]]) {
            if (j < blo) continue;
            if (j >= bhi) break;
            const auto before = previous.find(j - 1);
            const std::size_t k = (j > 0 && before != previous.end() ? before->second : 0) + 1;
            current[j] = k;
            if (k > best) {
                best_i = i + 1 - k;
                best_j = j + 1 - k;
                best = k;
            }
        }
        previous = std::move(current);
    }
    if (best == 0) return 0;
    std::size_t total = best;
    if (alo < best_i && blo < best_j) total += match_blocks(a, b, alo, best_i, blo, best_j, at);
    if (best_i + best < ahi && best_j + best < bhi) total += match_blocks(a, b, best_i + best, ahi, best_j + best, bhi, at);
    return total;
}

}  // namespace

// The ratio difflib's SequenceMatcher gives two runs of bytes (autojunk off): twice the bytes in the matching blocks over the bytes of both.
double sequence_ratio(ByteView a, ByteView b) {
    if (a.size() + b.size() == 0) return 1.0;
    std::vector<std::vector<std::size_t>> at(256);   // each byte's places in b, ascending
    for (std::size_t j = 0; j < b.size(); ++j) at[b[j]].push_back(j);
    return 2.0 * static_cast<double>(match_blocks(a, b, 0, a.size(), 0, b.size(), at)) / static_cast<double>(a.size() + b.size());
}

namespace {

struct MatchVerdict {
    verify::ByteMatch match = verify::ByteMatch::NoCompile;
    double share = 0.0;      // of the bytes outside the masked places, the share that are the original's
    std::size_t size = 0;    // the compiled function's, padding aside
    std::size_t original = 0;
    std::optional<MatchBytes> bytes;   // kept when asked for: a miss's two sides
};

// identical / same size / different, and the share of bytes equal outside the masked places.
MatchVerdict match_compare(MatchCode built, const MatchCode& original) {
    built = match_trimmed(std::move(built));
    MatchVerdict v;
    v.size = built.bytes.size();
    v.original = original.bytes.size();
    if (built.bytes.size() != original.bytes.size()) {
        v.match = verify::ByteMatch::Different;
        v.share = sequence_ratio(built.bytes, original.bytes);
        return v;
    }
    std::size_t free = 0;
    for (std::size_t i = 0; i < built.bytes.size(); ++i)
        if (!built.mask[i] && !original.mask[i]) ++free;
    const std::vector<ByteView> masks = {built.mask, original.mask};
    const std::size_t same = free - compare::mask::differing(built.bytes, original.bytes, masks).size();
    v.match = same == free ? verify::ByteMatch::Identical : verify::ByteMatch::SameSize;
    v.share = free ? static_cast<double>(same) / static_cast<double>(free) : 1.0;
    return v;
}

// Every function of the module compiled with CL.EXE under `flags` and compared: {function: verdict}, in the module's order.
std::vector<std::pair<std::string, MatchVerdict>> match_run(World& world, const stdfs::path& module, const stdfs::path& cl, const std::string& flags, const stdfs::path& work, bool keep_bytes) {
    const std::shared_ptr<const verify::lanes::Kind> lane = world.kit().win32_lane();
    if (!lane) throw UsageError("the 32-bit lane is not set up here (OpenWatcom and the native decompiler)");
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane->prepare(module, work / "source");
    const auto& m = static_cast<const verify::lanes::Win32Lane::PreparedModule&>(*prepared);
    std::string folder;
    for (const char c : flags) folder += (std::isalnum(static_cast<unsigned char>(c)) ? c : '_');
    while (!folder.empty() && folder.front() == '_') folder.erase(folder.begin());
    while (!folder.empty() && folder.back() == '_') folder.pop_back();
    const stdfs::path out_dir = work / (folder.empty() ? "default" : folder);
    fs::make_dirs(out_dir);
    // (the 32-bit C 8.00 of 1993 -- C13232 -- has no 64-bit integer: a function using one does not match anyway)
    std::error_code ec;
    const std::string wide = stdfs::is_regular_file(cl.parent_path() / "C13232.EXE", ec) ? "double" : "__int64";
    std::string header = text::replace_all(text::replace_all(m.header, "unsigned long long", "unsigned " + wide), "long long", wide);
    header = text::replace_all(header, "unsigned double", "double");
    // (the compiler's math intrinsics are __cdecl: under /Gz an undecorated declaration of one is a redefinition, and every function of the module fails)
    for (const char* const name : {"sqrt", "sin", "cos", "tan", "atan2", "pow", "ldexp", "log", "floor", "fmod"})
        header = text::replace_all(header, std::format("double {}(", name), std::format("double __cdecl {}(", name));
    fs::write_text(out_dir / "m.h", header);

    const std::string path_now = fs::get_env("PATH").value_or("");
    const std::map<std::string, std::string> env = {{"PATH", fs::to_text(cl.parent_path()) + ";" + path_now},
                                                    {"INCLUDE", fs::to_text(cl.parent_path().parent_path() / "INCLUDE")}};
    std::vector<std::string> switches;
    for (const std::string_view flag : text::split_ws(flags)) switches.emplace_back(flag);
    const std::vector<std::string>& names = m.order;
    std::vector<std::optional<Bytes>> objects(names.size());   // each function's object file; nullopt where it did not compile
    parallel::parallel_for(names.size(), [&](std::size_t i) {
        const std::string stem = std::format("f{:04}", i);
        fs::write_text(out_dir / (stem + ".c"), "#include \"m.h\"\n\n" + m.texts.at(names[i]) + "\n");
        process::ProcessSpec spec;
        spec.argv = {fs::to_text(cl), "/nologo", "/c", "/W0"};
        spec.argv.insert(spec.argv.end(), switches.begin(), switches.end());
        spec.argv.push_back(stem + ".c");
        spec.cwd = out_dir;
        spec.env = env;
        spec.timeout = std::chrono::seconds(300);
        const process::ProcessResult r = process::run(spec);
        if (r.exit_code != 0 || r.timed_out) return;
        objects[i] = fs::read_file(out_dir / (stem + ".obj"));
    });

    base::Corpus& corpus = world.corpus();
    (void)corpus;
    const formats::pe::Image pe = formats::pe::Image::open(module);
    std::vector<std::uint64_t> addresses;
    for (const auto& [name, va] : m.va) addresses.push_back(static_cast<std::uint64_t>(va));
    std::sort(addresses.begin(), addresses.end());
    const std::map<std::uint64_t, MatchCode> originals = match_originals(pe, addresses);
    std::vector<std::pair<std::string, MatchVerdict>> results;
    for (std::size_t i = 0; i < names.size(); ++i) {
        const auto va = m.va.find(names[i]);
        const auto original = va == m.va.end() ? originals.end() : originals.find(static_cast<std::uint64_t>(va->second));
        MatchVerdict v;
        if (!objects[i] || original == originals.end()) {
            v.match = !objects[i] ? verify::ByteMatch::NoCompile : verify::ByteMatch::NoExtent;
            v.original = original == originals.end() ? 0 : original->second.bytes.size();
        } else if (const std::optional<MatchCode> mine = match_object_function(*objects[i], names[i])) {
            v = match_compare(*mine, original->second);
            if (keep_bytes && (v.match == verify::ByteMatch::SameSize || v.match == verify::ByteMatch::Different)) {
                MatchCode trimmed = match_trimmed(*mine);
                v.bytes = MatchBytes{std::move(trimmed.bytes), std::move(trimmed.mask), original->second.bytes, original->second.mask};
            }
        } else {
            v.match = verify::ByteMatch::NoCompile;
            v.original = original->second.bytes.size();
        }
        results.emplace_back(names[i], v);
    }
    return results;
}

int match_command(World& world, const cli::Args& args) {
    const std::string given = args.required("module");
    const std::optional<stdfs::path> path = world.find(given);
    if (!path) throw UsageError("a 32-bit or 16-bit Windows module is needed: '" + given + "'");
    const std::optional<base::Lane> lane = world.corpus().lane(*path);
    if (is_powerpc_module(world, *path, given)) return match_powerpc_command(world, *path, given, args);   // the Xbox 360 compiler (pipeline_ppc.cpp)
    if (lane == base::Lane::Ne) return match16_command(world, *path, args);   // Microsoft C (pipeline_match16.cpp)
    if (lane != base::Lane::Pe) throw UsageError(given + ": a 32-bit or 16-bit Windows module is needed");

    const std::string period = world.config().text("period_compiler");
    const stdfs::path cl = fs::from_text(period) / "CL.EXE";
    std::error_code ec;
    if (period.empty() || !stdfs::is_regular_file(cl, ec)) throw UsageError("no period compiler (settings.json: period_compiler -- the folder holding CL.EXE)");

    const std::string flags = args.value_or("flags", match_flag_sets().front());
    const stdfs::path work = world.config().state(stdfs::path("match") / text::lower(fs::to_text(path->filename())));
    std::vector<std::string> sets;
    if (args.flag("try")) sets = match_flag_sets();
    else sets.push_back(flags);

    struct Best {
        verify::ByteMatch match = verify::ByteMatch::NoCompile;
        double share = 0.0;
        std::size_t size = 0, original = 0;
        std::string flags;
        std::size_t rank = 0;
        std::optional<MatchBytes> bytes;
    };
    std::vector<std::string> order;   // the functions, in the module's order
    std::map<std::string, Best> best;
    const auto rank_of = [](verify::ByteMatch m) { return static_cast<std::size_t>(std::find(verify::kByteMatches.begin(), verify::kByteMatches.end(), m) - verify::kByteMatches.begin()); };
    for (const std::string& set : sets) {
        auto results = match_run(world, *path, cl, set, work, args.value("bytes").has_value());
        std::size_t identical = 0;
        for (auto& [name, v] : results) {
            if (v.match == verify::ByteMatch::Identical) ++identical;
            const std::size_t rank = rank_of(v.match);
            const auto have = best.find(name);
            if (have == best.end()) order.push_back(name);
            if (have == best.end() || rank < have->second.rank || (rank == have->second.rank && v.share > have->second.share))
                best[name] = Best{v.match, v.share, v.size, v.original, set, rank, std::move(v.bytes)};
        }
        if (sets.size() > 1) cb::out("  {:<16} {} identical\n", set, identical);
    }
    std::map<verify::ByteMatch, std::size_t> counts;
    for (const auto& [name, b] : best) ++counts[b.match];
    std::vector<std::string> parts;
    for (const verify::ByteMatch k : verify::kByteMatches)
        if (counts[k] != 0) parts.push_back(std::format("{} {}", verify::label(k), counts[k]));
    cb::out("{}: {} functions compiled with the period compiler: {}\n", fs::to_text(path->filename()), best.size(), text::join(parts, ", "));
    std::vector<std::pair<double, std::string>> close;   // the nearest that are not identical, nearest first
    for (const auto& [name, b] : best)
        if (b.match != verify::ByteMatch::Identical) close.emplace_back(b.share, name);
    std::sort(close.begin(), close.end(), [](const auto& a, const auto& b) { return a > b; });
    for (std::size_t i = 0; i < close.size() && i < 8; ++i) {
        const Best& b = best.at(close[i].second);
        cb::out("  {:<28} {:<9} {:>3}% of bytes equal  ({} bytes, original {})\n", close[i].second, verify::label(b.match), static_cast<int>(100 * b.share), b.size, b.original);
    }
    if (const auto json_file = args.value("json")) {
        Json j = Json::object();
        std::vector<std::string> sorted(order);
        std::sort(sorted.begin(), sorted.end());
        for (const std::string& name : sorted) {
            const Best& b = best.at(name);
            j[name] = Json{{"verdict", std::string(verify::word(b.match))}, {"share", b.share}, {"size", b.size}, {"original", b.original}, {"flags", b.flags}};
        }
        fs::write_text(fs::from_text(*json_file), json::dump(j, json::Style::Pretty));
    }
    if (const auto bytes_file = args.value("bytes")) {
        std::map<std::string, MatchBytes> kept;
        for (auto& [name, b] : best)
            if (b.bytes) kept.emplace(name, std::move(*b.bytes));
        fs::write_text(fs::from_text(*bytes_file), match_bytes_json(kept));
    }
    return 0;
}

}  // namespace

// ---- repair <32-bit module> [--flags SWITCHES] [--first N] | --truth ... ----------------------------------------------------------------------------
//
// The search of verify/authored/repair is given the period compiler (CL.EXE) and the module's functions as a period compiler is given them: the
// reconstruction names the original's storage by its address and calls imports as plain functions, which is what lets it run beside the original;
// the original was not written that way, and a compiler shows the difference in every instruction that touches a global or calls Windows. So here
// the same source is given the declarations its author had (an import is `__declspec(dllimport)`, a global is a variable and an address taken as a
// number is the address of a variable, a function whose every `return` hands back "whatever was in EAX" returns nothing), which change no meaning.

namespace {

namespace rp = verify::authored::repair;

// The addresses the module's own relocations point at: every place in it that a word of code or data names by address.
std::set<std::uint64_t> repair_relocation_targets(const formats::pe::Image& pe) {
    std::set<std::uint64_t> out;
    for (const std::uint64_t rva : pe.base_relocs()) {
        const std::optional<std::uint64_t> at = pe.file_offset(rva);
        if (!at || *at + 4 > pe.data().size()) continue;
        const std::uint8_t* p = pe.data().data() + *at;
        out.insert(static_cast<std::uint64_t>(p[0]) | static_cast<std::uint64_t>(p[1]) << 8 | static_cast<std::uint64_t>(p[2]) << 16 | static_cast<std::uint64_t>(p[3]) << 24);
    }
    return out;
}

std::string repair_rstrip(std::string_view s) {
    while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back()))) s.remove_suffix(1);
    return std::string(s);
}

}  // namespace

RepairSource repair_period_source(const std::string& header, const std::vector<std::string>& order, const std::unordered_map<std::string, std::string>& texts,
                                  const std::set<std::string>& imports, const std::set<std::uint64_t>& targets) {
    static const Regex bound_line(R"(^#define (\w+) \(\*\(([^()]*?) ?\*\)0x([0-9a-fA-F]+)\)[ \t]*$)", RegexFlags::Multiline);
    static const Regex constant_re(R"((-\s*)?\b0x([0-9a-fA-F]{5,8})\b)");
    static const Regex nothing_line(R"(^[ \t]*return in_EAX;[ \t]*\n)", RegexFlags::Multiline);
    static const Regex in_eax_decl(R"(^[ \t]*\w+ in_EAX;[ \t]*\n)", RegexFlags::Multiline);
    static const Regex return_re(R"(\breturn\b[^;]*;)");
    static const Regex leading_type(R"(^\s*\w+\b)");
    static const Regex function_line(R"(^(\w+)((?: __\w+)?) (\w+)\((.*)\);[ \t]*$)");

    std::map<std::string, std::string> bound;
    bound_line.for_each_match(header, [&](const RegexMatch& m) {
        if (const auto a = text::parse_int<std::uint64_t>(m.group(3), 16); a && targets.contains(*a)) bound[std::string(m.group(1))] = std::string(m.group(2));
        return true;
    });
    std::map<std::uint64_t, std::string> names;
    const auto at = [&](std::uint64_t address) {
        return names[address] = std::format("codebreaker_at_{:08x}", address);
    };
    std::vector<std::string> body_parts;   // (what follows the first brace of each text, joined by line breaks)
    for (const std::string& name : order) {
        const std::string& t = texts.at(name);
        const std::size_t brace = t.find('{');
        body_parts.push_back(brace == std::string::npos ? std::string() : t.substr(brace + 1));
    }
    const std::string bodies = text::join(body_parts, "\n");
    const auto used = [&](const std::string& name) {
        const Regex call("\\b" + name + "\\s*\\(");   // (names are identifiers)
        bool found = false;
        call.for_each_match(bodies, [&](const RegexMatch& m) {
            const std::string before = repair_rstrip(std::string_view(bodies).substr(0, m.start()));
            const bool statement = before.empty() || std::string_view(";{})").find(before.back()) != std::string_view::npos || before.ends_with("else");
            if (!statement) {
                found = true;
                return false;
            }
            return true;
        });
        return found;
    };
    RepairSource out;
    std::set<std::string> nothing;
    for (const std::string& name : order) {
        std::string text = texts.at(name);
        const std::string original = text;
        text = constant_re.replace_all(original, [&](const RegexMatch& m) -> std::string {
            std::uint64_t value = text::parse_int<std::uint64_t>(m.group(2), 16).value_or(0);
            const bool minus = m.has(1);
            if (minus) value = (0 - value) & 0xFFFFFFFFull;
            if (!targets.contains(value)) return std::string(m.whole());
            const std::string before = repair_rstrip(std::string_view(original).substr(0, m.start()));
            const bool joined = minus && (before.empty() || std::isalnum(static_cast<unsigned char>(before.back())) || before.back() == '_' || before.back() == ')' || before.back() == ']');
            return std::string(joined ? "+ " : "") + "(int4)&" + at(value);
        });
        std::vector<std::string> returns = return_re.find_all(text);
        const bool all_nothing = !returns.empty() && std::all_of(returns.begin(), returns.end(), [](const std::string& r) {
            return text::replace_all(r, " ", "") == "returnin_EAX;";
        });
        if (all_nothing && !used(name)) {
            const std::size_t brace = text.find('{');
            std::string head = brace == std::string::npos ? text : text.substr(0, brace);
            std::string body = brace == std::string::npos ? std::string() : text.substr(brace + 1);
            body = nothing_line.replace_all(in_eax_decl.replace_all(body, ""), "");
            if (body.find("in_EAX") == std::string::npos) {   // (given a value somewhere: it is a result)
                head = leading_type.replace_first(head, "void");
                text = head + (brace == std::string::npos ? "" : "{") + body;
                nothing.insert(name);
            }
        }
        out.texts[name] = std::move(text);
    }
    std::vector<std::string> lines;
    for (const std::string_view line : text::split(header, '\n')) {
        std::string made(line);
        if (const auto m = bound_line.match(line); m && bound.contains(std::string(m->group(1)))) {
            made = std::format("extern {} {};", bound.at(std::string(m->group(1))), m->group(1));
        } else if (const auto f = function_line.match(line)) {
            if (imports.contains(std::string(f->group(3))) && f->group(4).empty()) made = "__declspec(dllimport) " + std::string(line);
            else if (nothing.contains(std::string(f->group(3)))) made = std::format("void{} {}({});", f->group(2), f->group(3), f->group(4));
        }
        lines.push_back(std::move(made));
    }
    for (const auto& [address, name] : names) lines.push_back("extern char " + name + ";");
    out.header = text::join(lines, "\n");
    return out;
}

// The header with only the declarations `text` uses (types and macros are all kept): what this compiler makes of a function depends on what was
// declared before it -- a variable the function never mentions, declared a thousand lines earlier, can turn `cmp eax, ecx` into `cmp ecx, eax`.
std::string repair_needed_header(const std::string& header, std::string_view text) {
    static const Regex word_re(R"([A-Za-z_]\w*)");
    static const Regex declares(R"(^(?:__declspec\(dllimport\) )?(?:extern )?[\w ]*?[ *](\w+)(?:\(.*\)|\[\d*\])?;[ \t]*$|^#define (\w+) \(\*)");
    std::set<std::string> words;
    for (const std::string& w : word_re.find_all(text)) words.insert(w);
    std::vector<std::string> keep;
    bool inside = false, wanted = false;
    for (const std::string_view line : text::split(header, '\n')) {
        if (inside || line.starts_with("typedef struct")) {
            if (line.starts_with("typedef struct")) {
                inside = true;
                const std::vector<std::string_view> parts = text::split_ws(line);
                wanted = parts.size() > 2 && words.contains(std::string(parts[2]));
            }
            if (wanted) keep.emplace_back(line);
            inside = !line.starts_with("}");
            continue;
        }
        if (const auto m = declares.match(line)) {
            const std::string name = std::string(m->has(1) ? m->group(1) : m->group(2));
            if (!words.contains(name) && !line.starts_with("typedef")) continue;
        }
        keep.emplace_back(line);
    }
    return text::join(keep, "\n");
}

namespace {

// The period compiler as the search is given it: CL.EXE, in the environment it was made for. A text compiled once is kept for the run (a search tries
// the same text again and again).
class RepairCl final : public rp::Compiler {
public:
    explicit RepairCl(stdfs::path cl) : cl_(std::move(cl)) {
        const std::string path_now = fs::get_env("PATH").value_or("");
        env_ = {{"PATH", fs::to_text(cl_.parent_path()) + ";" + path_now}, {"INCLUDE", fs::to_text(cl_.parent_path().parent_path() / "INCLUDE")}};
    }

    std::optional<rp::Code> compile(std::string_view header, std::string_view source, std::string_view name, std::string_view flags,
                                    const stdfs::path& work) const override {
        const std::string key = std::string(flags) + '\0' + std::string(header) + '\0' + std::string(source) + '\0' + std::string(name);
        {
            std::lock_guard hold(lock_);
            if (const auto it = kept_.find(key); it != kept_.end()) return it->second;
        }
        const std::optional<rp::Code> built = compile_now(header, source, name, flags, work);
        std::lock_guard hold(lock_);
        kept_.emplace(key, built);
        return built;
    }

    verify::authored::search16::ByteVerdict distance(const rp::Code& built, const rp::Code& original) const override {
        MatchCode b{built.bytes, built.mask};
        b = match_trimmed(std::move(b));
        const compare::mask::DistanceResult d = compare::mask::distance(b.bytes, original.bytes, b.mask, original.mask);
        verify::authored::search16::ByteVerdict v;
        v.match = d.verdict == compare::mask::Match::Identical ? verify::ByteMatch::Identical : d.verdict == compare::mask::Match::SameSize ? verify::ByteMatch::SameSize : verify::ByteMatch::Different;
        v.differing = d.differing;
        v.total = d.compared;
        return v;
    }

    std::vector<std::string> flag_sets() const override { return match_flag_sets(); }
    std::string needed_header(std::string_view header, std::string_view source) const override { return repair_needed_header(std::string(header), source); }

private:
    std::optional<rp::Code> compile_now(std::string_view header, std::string_view source, std::string_view name, std::string_view flags, const stdfs::path& work) const {
        fs::make_dirs(work);
        fs::write_text(work / "m.h", text::replace_all(text::replace_all(std::string(header), "unsigned long long", "unsigned __int64"), "long long", "__int64"));
        std::error_code ec;
        stdfs::remove(work / "one.obj", ec);
        stdfs::remove(work / "one.c", ec);
        fs::write_text(work / "one.c", "#include \"m.h\"\n\n" + std::string(source) + "\n");
        process::ProcessSpec spec;
        spec.argv = {fs::to_text(cl_), "/nologo", "/c", "/W0"};
        for (const std::string_view flag : text::split_ws(flags)) spec.argv.emplace_back(flag);
        spec.argv.push_back("one.c");
        spec.cwd = work;
        spec.env = env_;
        spec.timeout = std::chrono::seconds(300);
        const process::ProcessResult r = process::run(spec);
        if (r.exit_code != 0 || r.timed_out || !stdfs::is_regular_file(work / "one.obj", ec)) return std::nullopt;
        const std::optional<MatchCode> mine = match_object_function(fs::read_file(work / "one.obj"), name);
        if (!mine) return std::nullopt;
        return rp::Code{mine->bytes, mine->mask, {}};
    }

    stdfs::path cl_;
    std::map<std::string, std::string> env_;
    mutable std::mutex lock_;
    mutable std::map<std::string, std::optional<rp::Code>> kept_;
};

int repair_command(World& world, const cli::Args& args) {
    const std::vector<std::string> words = args.values("args");
    const std::optional<rp::Arguments> parsed = rp::parse_arguments(words);
    if (!parsed) {
        cb::out("{}\n", rp::kUsage);
        return kExitUsage;
    }
    if (parsed->truth) return repair_truth_command(world, parsed->truth_args);   // (pipeline_ppc.cpp)
    const std::optional<stdfs::path> path = world.find(parsed->module);
    if (!path) throw UsageError(parsed->module + ": a 32-bit Windows module or a PowerPC image is needed");
    const std::optional<base::Lane> lane = world.corpus().lane(*path);
    if (is_powerpc_module(world, *path, parsed->module)) return repair_powerpc_command(world, *path, *parsed);
    if (lane != base::Lane::Pe) throw UsageError(parsed->module + ": a 32-bit Windows module or a PowerPC image is needed");
    const std::string period = world.config().text("period_compiler");
    const stdfs::path cl = fs::from_text(period) / "CL.EXE";
    std::error_code ec;
    if (period.empty() || !stdfs::is_regular_file(cl, ec)) throw UsageError("no period compiler (settings.json: period_compiler)");

    const std::string file_name = text::lower(fs::to_text(path->filename()));
    const stdfs::path work = world.config().state(stdfs::path("repair") / file_name);
    const std::shared_ptr<const verify::lanes::Kind> kind = world.kit().win32_lane();
    if (!kind) throw UsageError("the 32-bit lane is not set up here (OpenWatcom and the native decompiler)");
    const std::unique_ptr<verify::lanes::Prepared> prepared = kind->prepare(*path, work / "source");
    const auto& m = static_cast<const verify::lanes::Win32Lane::PreparedModule&>(*prepared);
    const formats::pe::Image pe = formats::pe::Image::open(*path);
    std::vector<std::uint64_t> addresses;
    for (const auto& [name, va] : m.va) addresses.push_back(static_cast<std::uint64_t>(va));
    std::sort(addresses.begin(), addresses.end());
    const std::map<std::uint64_t, MatchCode> originals = match_originals(pe, addresses);
    const std::set<std::string> imports(m.module.imports.begin(), m.module.imports.end());
    const RepairSource period_source = repair_period_source(m.header, m.order, m.texts, imports, repair_relocation_targets(pe));

    const verify::reconstruct::Identity id = world.reconstructor().identity(*path);
    engine::FunctionStore& store = world.store();
    std::map<std::string, std::string> behaves;   // what the judge said of each function (by its key)
    for (const engine::Candidate& c : store.candidates(id.sha, id.profile))
        if (c.verdict) behaves[c.addr] = *c.verdict;
    store.add_module(id.sha, file_name, fs::to_text(*path), "pe");
    store.add_functions(id.sha, m.all_keys);

    rp::Module module;
    module.isa = rp::Isa::X86_32;
    module.header = period_source.header;
    module.compiler_name = text::lower(fs::to_text(cl.filename()));
    for (const std::string& name : m.order) {
        const auto va = m.va.find(name);
        const auto original = va == m.va.end() ? originals.end() : originals.find(static_cast<std::uint64_t>(va->second));
        if (original == originals.end()) continue;
        module.targets.push_back({name, m.key.at(name), period_source.texts.at(name), rp::Code{original->second.bytes, original->second.mask, {}}});
    }
    const RepairCl compiler(cl);
    rp::RunOptions options;
    options.flags = parsed->flags;
    options.work = work;
    options.limit = parsed->first;
    options.log = [](std::string_view line) { cb::out("{}\n", line); };
    options.record = [&](const rp::Target& target, const rp::Repaired& r, const std::string& setting) {
        const auto b = behaves.find(target.key);
        store.record_variant(id.sha, target.key, setting, r.source, static_cast<std::int64_t>(r.verdict.differing), static_cast<std::int64_t>(r.verdict.total),
                             verify::word(r.verdict.match), r.knobs, std::nullopt, b == behaves.end() ? std::string_view() : std::string_view(b->second));
    };
    const rp::Counts counts = rp::run(module, compiler, options);
    cb::out("{}\n", rp::summary_line(fs::to_text(path->filename()), counts));
    return 0;
}

}  // namespace

cli::Command repair(const Environment& environment) {
    cli::Command declared{"repair", "search for source that compiles to the original's bytes",
                          {cli::Positional{"args", "<module> [--flags SWITCHES] [--first N] | --truth <compiler> [target]", false, true}}, {}, {}, "Reconstruct",
                          std::string(rp::kUsage)};
    declared.passthrough = true;
    return in_world(std::move(declared), environment, [](World& world, const cli::Args& args) { return repair_command(world, args); });
}

cli::Command match(const Environment& environment) {
    cli::Command declared{"match",
                          "would a period compiler give a module's functions back as the original's bytes?",
                          {cli::Positional{"module", "a 32-bit module of the corpus, or a path"},
                           cli::Option{"--flags", "", "the compiler's switches (default: /O2 /Gz)", cli::Arg::Value, "SWITCHES"},
                           cli::Option{"--try", "", "go through several sets of switches and report the best for each function", cli::Arg::Flag},
                           cli::Option{"--json", "", "write each function's result there", cli::Arg::Value, "FILE"},
                           cli::Option{"--bytes", "", "write each near miss's compiled and original bytes there", cli::Arg::Value, "FILE"},
                           cli::Option{"--compiler", "", "a 16-bit module: the only compiler to try (msc6 or msvc152)", cli::Arg::Value, "NAME"},
                           cli::Option{"--first", "", "a 16-bit module: only its first N functions", cli::Arg::Value, "N"},
                           cli::Option{"--choose", "", "a 16-bit module: find the switches again though an earlier run remembered them", cli::Arg::Flag},
                           cli::Option{"--sdk", "", "a 16-bit module: also say the Windows calls through the SDK's prototypes", cli::Arg::Flag},
                           cli::Option{"--no-fold", "", "a 16-bit module: do not fold the decompiler's temporaries back", cli::Arg::Flag},
                           cli::Option{"--search", "", "a 16-bit module: ROUNDS of rewrites of the last bytes of the near misses", cli::Arg::Value, "ROUNDS"},
                           cli::Option{"--wine", "", "a 16-bit module: also type the imports as Wine's .spec files do", cli::Arg::Flag}},
                          {},
                          {},
                          "Reconstruct",
                          "A second, optional signal: where a function's C, compiled with a compiler of the original's time, comes out as the original's bytes, "
                          "the C is as near what was written as a binary can show. identical: every byte outside the places the linker fills in is the "
                          "same; same size: some bytes differ; different: another length. A 16-bit module is compiled by Microsoft C (in an emulator or on "
                          "Windows itself), segment by segment with the switches that fit it best (--flags names them)."};
    return in_world(std::move(declared), environment, [](World& world, const cli::Args& args) { return match_command(world, args); });
}

}  // namespace cb::source::pipeline
