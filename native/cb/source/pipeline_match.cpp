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
// A 16-bit module (Microsoft C in an emulator: verify/authored/match16) and an Xbox 360 image (its compiler's objects, read by bits) are not made here.
#include <algorithm>
#include <map>
#include <set>

#include <nlohmann/json.hpp>
#include "cb/compare/mask.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/log.hpp"
#include "cb/core/parallel.hpp"
#include "cb/core/process.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/pe.hpp"
#include "cb/formats/pe_functions.hpp"
#include "cb/platforms/decoder.hpp"
#include "cb/source/pipeline_world.hpp"
#include "cb/symbols/libraries/coff.hpp"
#include "cb/verify/lanes/win32.hpp"

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
std::vector<std::pair<std::string, MatchVerdict>> match_run(World& world, const stdfs::path& module, const stdfs::path& cl, const std::string& flags, const stdfs::path& work) {
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
    if (text::iends_with(given, ".xex") || (lane == base::Lane::Pe && world.reconstructor().kind_name(*path) == "powerpc"))
        throw UsageError("match: an Xbox 360 image is not made native yet (its compiler's objects are read by bits)");
    if (lane == base::Lane::Ne) throw UsageError("match: a 16-bit module goes through match16 (Microsoft C in an emulator), which is not made native yet");
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
    };
    std::vector<std::string> order;   // the functions, in the module's order
    std::map<std::string, Best> best;
    const auto rank_of = [](verify::ByteMatch m) { return static_cast<std::size_t>(std::find(verify::kByteMatches.begin(), verify::kByteMatches.end(), m) - verify::kByteMatches.begin()); };
    for (const std::string& set : sets) {
        const auto results = match_run(world, *path, cl, set, work);
        std::size_t identical = 0;
        for (const auto& [name, v] : results) {
            if (v.match == verify::ByteMatch::Identical) ++identical;
            const std::size_t rank = rank_of(v.match);
            const auto have = best.find(name);
            if (have == best.end()) order.push_back(name);
            if (have == best.end() || rank < have->second.rank || (rank == have->second.rank && v.share > have->second.share))
                best[name] = Best{v.match, v.share, v.size, v.original, set, rank};
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
    return 0;
}

}  // namespace

cli::Command match(const Environment& environment) {
    cli::Command declared{"match",
                          "would a period compiler give a module's functions back as the original's bytes?",
                          {cli::Positional{"module", "a 32-bit module of the corpus, or a path"},
                           cli::Option{"--flags", "", "the compiler's switches (default: /O2 /Gz)", cli::Arg::Value, "SWITCHES"},
                           cli::Option{"--try", "", "go through several sets of switches and report the best for each function", cli::Arg::Flag},
                           cli::Option{"--json", "", "write each function's result there", cli::Arg::Value, "FILE"}},
                          {},
                          {},
                          "Reconstruct",
                          "A second, optional signal: where a function's C, compiled with a compiler of the original's time, comes out as the original's bytes, "
                          "the C is as near what was written as a binary can show. identical: every byte outside the places the linker fills in is the "
                          "same; same size: some bytes differ; different: another length."};
    return in_world(std::move(declared), environment, [](World& world, const cli::Args& args) { return match_command(world, args); });
}

}  // namespace cb::source::pipeline
