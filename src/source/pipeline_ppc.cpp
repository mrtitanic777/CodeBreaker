// `match` and `repair` of an Xbox 360 image, and `repair --truth` (pipeline_ppc.hpp).
//
// Every reconstructed function is compiled on its own by the Xbox 360 compiler, its words are read from the object, and each is compared with the
// original's at the bits the object says the linker fills in (the original's own calls out of the function are masked too). `repair` gives the same
// compiler and comparison to the search of verify/authored/repair.
#include "source/pipeline_ppc.hpp"

#include <nlohmann/json.hpp>

#include <algorithm>
#include <map>
#include <mutex>
#include <set>
#include <thread>

#include "compare/mask.hpp"
#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/log.hpp"
#include "core/parallel.hpp"
#include "core/process.hpp"
#include "core/text.hpp"
#include "formats/pe.hpp"
#include "formats/powerpc_image.hpp"
#include "formats/xex.hpp"
#include "symbols/libraries/coff.hpp"
#include "verify/lanes/powerpc.hpp"
#include "verify/verdicts.hpp"

namespace cb::source::pipeline {

namespace stdfs = std::filesystem;
namespace rp = verify::authored::repair;
namespace coff = symbols::libraries::coff;

namespace {

constexpr std::uint32_t kPpcLi = 0x03FFFFFC;     // the 24-bit distance of b / bl
constexpr std::uint32_t kPpcBd = 0x0000FFFC;     // the 14 bits of a conditional branch
constexpr std::uint32_t kPpcHalf = 0x0000FFFF;   // a 16-bit half of an address
constexpr std::uint32_t kPpcWord = 0xFFFFFFFF;

// The switches `--try` goes through (the first is the default).
const std::vector<std::string>& ppc_flag_sets() {
    static const std::vector<std::string> sets = {"/O2", "/Ox", "/O1", "/Os", "/O2 /Oy-", "/Od"};
    return sets;
}

// Which bits of a word the linker fills in for a relocation of this type (IMAGE_REL_PPC_*, the type's low byte: the high bits are hints to the linker).
std::uint32_t ppc_field(std::uint16_t type) {
    switch (type & 0xFF) {
        case 0x03: case 0x06: return kPpcLi;                                                  // ADDR24, REL24 (b / bl)
        case 0x05: case 0x07: return kPpcBd;                                                  // ADDR14, REL14 (bc)
        case 0x04: case 0x08: case 0x09: case 0x0F: case 0x10: case 0x11: case 0x13: case 0x14: return kPpcHalf;   // ADDR16, TOCREL, REFHI/LO ...
        case 0x02: case 0x0B: return kPpcWord;                                                // ADDR32, SECREL
        default: return 0;
    }
}

// Without the zero words a compiler pads a function with.
rp::Code ppc_trimmed(rp::Code code) {
    std::size_t n = code.bytes.size() / 4;
    const auto zero_word = [&](std::size_t i) { return code.bytes[4 * i] == 0 && code.bytes[4 * i + 1] == 0 && code.bytes[4 * i + 2] == 0 && code.bytes[4 * i + 3] == 0; };
    while (n > 1 && zero_word(n - 1)) --n;
    code.bytes.resize(4 * n);
    if (code.word_mask.size() > n) code.word_mask.resize(n);
    return code;
}

std::uint32_t ppc_word(ByteView bytes, std::size_t at) {
    return static_cast<std::uint32_t>(bytes[at]) << 24 | static_cast<std::uint32_t>(bytes[at + 1]) << 16 | static_cast<std::uint32_t>(bytes[at + 2]) << 8 | bytes[at + 3];
}

// The original's functions: {address: code and the bits that depend on where things are}. Each runs to the length its exception-table record gives, else
// to the next function, and ends before the first zero word (the padding between functions: no instruction is zero); a `b`/`bl` that leaves the function has
// its distance masked (where the callee sits is the linker's choice).
std::map<std::uint64_t, rp::Code> ppc_originals(const formats::powerpc_image::Image& image, const std::vector<std::uint64_t>& addresses) {
    using Address = formats::powerpc_image::Address;
    std::set<Address> starts;
    for (const Address a : image.functions()) starts.insert(a);
    for (const auto& entry : image.imports()) starts.insert(entry.first);
    for (const auto& entry : image.inlined()) starts.insert(entry.first);
    std::map<std::uint64_t, rp::Code> out;
    for (const std::uint64_t at : addresses) {
        const Address va = static_cast<Address>(at);
        std::uint64_t size = 0;
        if (const auto extent = image.extents().find(va); extent != image.extents().end()) size = extent->second.size;
        if (size == 0) {
            const auto next = starts.upper_bound(va);
            size = static_cast<std::uint64_t>(starts.contains(va) ? (next != starts.end() ? *next : image.code_end()) : va) - va;
        }
        const ByteView seen = image.bytes_at(va, static_cast<std::size_t>(size));
        std::size_t length = seen.size();
        for (std::size_t i = 0; i + 3 < seen.size(); i += 4) {
            if (ppc_word(seen, i) == 0) {
                length = i;
                break;
            }
        }
        if (length < 4) continue;
        rp::Code code;
        code.bytes.assign(seen.begin(), seen.begin() + static_cast<std::ptrdiff_t>(length));
        for (std::size_t i = 0; i < length / 4; ++i) {
            const std::uint32_t w = ppc_word(code.bytes, 4 * i);
            std::uint32_t ignore = 0;
            if (w >> 26 == 18 && !(w & 2)) {   // b / bl, by distance
                std::int64_t d = w & 0x03FFFFFC;
                if (d & 0x02000000) d -= 0x04000000;
                const std::int64_t target = static_cast<std::int64_t>(va) + 4 * static_cast<std::int64_t>(i) + d;
                if (!(static_cast<std::int64_t>(va) <= target && target < static_cast<std::int64_t>(va + length))) ignore = kPpcLi;
            }
            code.word_mask.push_back(ignore);
        }
        out[at] = ppc_trimmed(std::move(code));
    }
    return out;
}

// What the compiler made of a function: its words in the object, by the symbol that is its name; the bits the object's relocations fill. A function's
// symbol says so (type 0x20); labels inside one (class 6) do not.
std::optional<rp::Code> ppc_object_function(ByteView object, std::string_view name) {
    std::optional<coff::Object> parsed;
    try {
        parsed.emplace(coff::Object::parse(object));
    } catch (const Error&) {
        return std::nullopt;
    }
    const coff::Object& o = *parsed;
    if (o.machine() != coff::kPowerPc) return std::nullopt;
    std::map<std::size_t, std::vector<std::pair<std::uint32_t, std::string>>> by_section;
    for (const auto& s : o.symbols()) {
        if (!s || s->sec <= 0 || static_cast<std::size_t>(s->sec) > o.sections().size()) continue;
        if (!(o.sections()[static_cast<std::size_t>(s->sec) - 1].chars & coff::kSectionCode) || s->type != 0x20 || s->cls == 6) continue;
        by_section[static_cast<std::size_t>(s->sec)].emplace_back(s->value, s->name);
    }
    std::map<std::string, rp::Code> found;
    try {
        for (auto& [number, list] : by_section) {
            std::sort(list.begin(), list.end());
            const coff::Section& section = o.sections()[number - 1];
            std::vector<std::uint32_t> ignore((section.size + 3) / 4, 0);
            for (const coff::Relocation& r : o.relocations(section))
                if (r.offset / 4 < ignore.size()) ignore[r.offset / 4] |= ppc_field(r.type);
            for (std::size_t k = 0; k < list.size(); ++k) {
                const std::uint32_t value = list[k].first;
                const std::uint32_t end = k + 1 < list.size() ? list[k + 1].first : section.size;
                rp::Code code;
                const std::size_t from = std::min<std::size_t>(section.ptr + value, object.size());
                const std::size_t to = std::min<std::size_t>(section.ptr + end, object.size());
                if (to > from) code.bytes.assign(object.begin() + static_cast<std::ptrdiff_t>(from), object.begin() + static_cast<std::ptrdiff_t>(to));
                const std::size_t lo = value / 4, hi = (static_cast<std::size_t>(end) + 3) / 4;
                for (std::size_t i = lo; i < hi && i < ignore.size(); ++i) code.word_mask.push_back(ignore[i]);
                found[list[k].second] = std::move(code);
            }
        }
    } catch (const Error&) {
        return std::nullopt;
    }
    if (const auto it = found.find(std::string(name)); it != found.end()) return it->second;
    if (found.size() == 1) return found.begin()->second;
    return std::nullopt;
}

// How far a compiled function is from the original's, in words, outside the bits the linker fills in.
verify::authored::search16::ByteVerdict ppc_distance(const rp::Code& built, const rp::Code& original) {
    const rp::Code b = ppc_trimmed(built);
    const auto widen = [](const std::vector<std::uint32_t>& v) { return std::vector<std::uint64_t>(v.begin(), v.end()); };
    const std::vector<std::uint64_t> ma = widen(b.word_mask), mb = widen(original.word_mask);
    const compare::mask::DistanceResult d = compare::mask::distance_words(b.bytes, original.bytes, 4, compare::mask::ByteOrder::Big, ma, mb);
    verify::authored::search16::ByteVerdict v;
    v.match = d.verdict == compare::mask::Match::Identical ? verify::ByteMatch::Identical
              : d.verdict == compare::mask::Match::SameSize ? verify::ByteMatch::SameSize
                                                            : verify::ByteMatch::Different;
    v.differing = d.differing;
    v.total = d.compared;
    return v;
}

stdfs::path ppc_compiler(World& world) {
    const std::optional<stdfs::path> folder = world.config().path("powerpc_tools"), lib = world.config().path("powerpc_lib");
    std::error_code ec;
    if (!folder || !lib || !stdfs::is_regular_file(*folder / "cl.exe", ec) || !stdfs::is_regular_file(*folder / "link.exe", ec) || !stdfs::is_regular_file(*lib, ec))
        throw UsageError("no PowerPC compiler (settings.json: powerpc_tools, powerpc_lib)");
    return *folder / "cl.exe";
}

// The Xbox 360 compiler as the search is given it. A text compiled once is kept for the run (a search tries the same text again and again).
class PpcCl final : public rp::Compiler {
public:
    explicit PpcCl(stdfs::path cl) : cl_(std::move(cl)) {}

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
    verify::authored::search16::ByteVerdict distance(const rp::Code& built, const rp::Code& original) const override { return ppc_distance(built, original); }
    std::vector<std::string> flag_sets() const override { return ppc_flag_sets(); }

private:
    std::optional<rp::Code> compile_now(std::string_view header, std::string_view source, std::string_view name, std::string_view flags, const stdfs::path& work) const {
        fs::make_dirs(work);
        fs::write_text(work / "m.h", std::string(header));
        std::error_code ec;
        stdfs::remove(work / "one.obj", ec);
        stdfs::remove(work / "one.c", ec);
        fs::write_text(work / "one.c", std::string(source) + "\n");
        process::ProcessSpec spec;
        spec.argv = {fs::to_text(cl_), "/nologo", "/c", "/TC", "/GS-", "/Zl", "/Gy", "/W0", "/FIm.h"};
        for (const std::string_view flag : text::split_ws(flags)) spec.argv.emplace_back(flag);
        spec.argv.push_back("/Foone.obj");
        spec.argv.push_back("one.c");
        spec.cwd = work;
        spec.timeout = std::chrono::seconds(600);
        const process::ProcessResult r = process::run(spec);
        if (r.exit_code != 0 || r.timed_out || !stdfs::is_regular_file(work / "one.obj", ec)) return std::nullopt;
        return ppc_object_function(fs::read_file(work / "one.obj"), name);
    }

    stdfs::path cl_;
    mutable std::mutex lock_;
    mutable std::map<std::string, std::optional<rp::Code>> kept_;
};

// The PowerPC image of the module: an .xex is unpacked (once per distinct file).
stdfs::path ppc_image_path(World& world, const stdfs::path& path) {
    if (!text::iends_with(fs::to_text(path), ".xex")) return path;
    formats::xex::Tools tools;
    tools.unpacked_dir = world.config().state() / "unpacked" / "xex";
    return formats::xex::basefile(tools, path);
}

// What `prepare` gave of the image, as `match` and `repair` use it: the functions by name and address, and the original's code.
struct PpcModule {
    std::unique_ptr<verify::lanes::Prepared> prepared;
    std::map<std::string, std::uint64_t> address;   // name -> address
    std::map<std::uint64_t, rp::Code> originals;
    const verify::lanes::PowerPcLane::PreparedPowerPc& lane() const { return static_cast<const verify::lanes::PowerPcLane::PreparedPowerPc&>(*prepared); }
};

PpcModule ppc_prepare(World& world, const stdfs::path& image_path, const stdfs::path& work) {
    const std::shared_ptr<const verify::lanes::Kind> kind = world.kit().powerpc_lane();
    if (!kind) throw UsageError("no PowerPC compiler (settings.json: powerpc_tools, powerpc_lib)");
    PpcModule m;
    m.prepared = kind->prepare(image_path, work / "source");
    std::vector<std::uint64_t> addresses;
    for (const auto& [va, name] : m.lane().module.made.by_va()) {
        m.address[name] = static_cast<std::uint64_t>(va);
        addresses.push_back(static_cast<std::uint64_t>(va));
    }
    std::sort(addresses.begin(), addresses.end());
    m.originals = ppc_originals(formats::powerpc_image::Image::read(image_path), addresses);
    return m;
}

struct PpcResult {
    verify::ByteMatch match = verify::ByteMatch::NoCompile;
    std::size_t differing = 0, words = 0, original_words = 0;
};

// Every function of the image compiled under `flags` and compared: {function: result}, in the image's order.
std::vector<std::pair<std::string, PpcResult>> ppc_run(const PpcModule& m, const PpcCl& cl, const std::string& flags, const stdfs::path& work) {
    const std::vector<std::string>& names = m.prepared->order;
    const std::string& header = m.prepared->header;
    std::vector<std::optional<rp::Code>> built(names.size());
    parallel::parallel_for(names.size(), [&](std::size_t i) {
        const stdfs::path own = work / std::format("w{}", std::hash<std::thread::id>()(std::this_thread::get_id()) & 0x7FFFFFFF);
        const auto text = m.prepared->texts.find(names[i]);
        if (text == m.prepared->texts.end()) return;
        built[i] = cl.compile(header, text->second, names[i], flags, own);
    });
    std::vector<std::pair<std::string, PpcResult>> results;
    for (std::size_t i = 0; i < names.size(); ++i) {
        const auto va = m.address.find(names[i]);
        const auto original = va == m.address.end() ? m.originals.end() : m.originals.find(va->second);
        PpcResult r;
        r.original_words = original == m.originals.end() ? 0 : original->second.bytes.size() / 4;
        if (!built[i] || original == m.originals.end()) {
            r.match = !built[i] ? verify::ByteMatch::NoCompile : verify::ByteMatch::NoExtent;
        } else {
            const auto d = ppc_distance(*built[i], original->second);
            r.match = d.match;
            r.differing = d.differing;
            r.words = d.total;
        }
        results.emplace_back(names[i], r);
    }
    return results;
}

}  // namespace

bool is_powerpc_module(World& world, const stdfs::path& path, const std::string& given) {
    if (text::iends_with(given, ".xex")) return true;
    return world.corpus().lane(path) == base::Lane::Pe && world.reconstructor().kind_name(path) == "powerpc";
}

std::optional<rp::Code> ppc_compile_function(const stdfs::path& cl, std::string_view header, std::string_view source, std::string_view name, std::string_view flags,
                                             const stdfs::path& work) {
    return PpcCl(cl).compile(header, source, name, flags, work);
}

std::map<std::uint64_t, rp::Code> ppc_original_code(const formats::powerpc_image::Image& image, const std::vector<std::uint64_t>& addresses) {
    return ppc_originals(image, addresses);
}

verify::authored::search16::ByteVerdict ppc_compare(const rp::Code& built, const rp::Code& original) { return ppc_distance(built, original); }

int match_powerpc_command(World& world, const stdfs::path& path, const std::string& given, const cli::Args& args) {
    const PpcCl cl(ppc_compiler(world));
    const stdfs::path image = ppc_image_path(world, path);
    const stdfs::path work = world.config().state(stdfs::path("match") / text::lower(fs::to_text(image.filename())));
    const PpcModule module = ppc_prepare(world, image, work);
    const std::vector<std::string> sets = args.flag("try") ? ppc_flag_sets() : std::vector<std::string>{args.value_or("flags", ppc_flag_sets().front())};

    struct Best {
        std::pair<std::size_t, std::size_t> rank;   // the verdict's place, then the words that differ: smaller is nearer
        PpcResult result;
        std::string flags;
    };
    std::vector<std::string> order;
    std::map<std::string, Best> best;
    const auto rank_of = [](verify::ByteMatch m) { return static_cast<std::size_t>(std::find(verify::kByteMatches.begin(), verify::kByteMatches.end(), m) - verify::kByteMatches.begin()); };
    for (const std::string& set : sets) {
        std::string folder;
        for (const char c : set) folder += std::isalnum(static_cast<unsigned char>(c)) ? c : '_';
        const auto results = ppc_run(module, cl, set, work / (folder.empty() ? "default" : folder));
        std::size_t identical = 0;
        for (const auto& [name, r] : results) {
            if (r.match == verify::ByteMatch::Identical) ++identical;
            const std::pair<std::size_t, std::size_t> rank{rank_of(r.match), r.differing};
            const auto have = best.find(name);
            if (have == best.end()) order.push_back(name);
            if (have == best.end() || rank < have->second.rank) best[name] = Best{rank, r, set};
        }
        if (sets.size() > 1) cb::out("  {:<12} {} identical\n", set, identical);
    }
    std::map<verify::ByteMatch, std::size_t> counts;
    for (const auto& [name, b] : best) ++counts[b.result.match];
    cb::out("{}: {} functions\n", fs::to_text(fs::from_text(given).filename()), best.size());
    for (const verify::ByteMatch k : verify::kByteMatches)
        if (counts[k] != 0) cb::out("  {:<20} {}\n", verify::label(k), counts[k]);
    if (const auto json_file = args.value("json")) {
        Json j = Json::object();
        std::vector<std::string> sorted(order);
        std::sort(sorted.begin(), sorted.end());
        for (const std::string& name : sorted) {
            const Best& b = best.at(name);
            j[name] = Json{{"verdict", std::string(verify::word(b.result.match))},
                           {"words differing", b.result.differing},
                           {"words", b.result.words},
                           {"original words", b.result.original_words},
                           {"flags", b.flags}};
        }
        fs::write_text(fs::from_text(*json_file), json::dump(j, json::Style::Pretty));
    }
    return 0;
}

int repair_powerpc_command(World& world, const stdfs::path& path, const rp::Arguments& arguments) {
    const PpcCl cl(ppc_compiler(world));
    const stdfs::path image = ppc_image_path(world, path);
    const std::string file_name = text::lower(fs::to_text(image.filename()));
    const stdfs::path work = world.config().state(stdfs::path("repair") / file_name);
    const PpcModule m = ppc_prepare(world, image, work);
    const verify::reconstruct::Identity id = world.reconstructor().identity(image);
    engine::FunctionStore& store = world.store();
    store.add_module(id.sha, file_name, fs::to_text(image), "powerpc");
    std::vector<std::string> keys;
    for (const auto& [name, key] : m.lane().key) keys.push_back(key);
    store.add_functions(id.sha, keys);

    rp::Module module;
    module.isa = rp::Isa::PowerPC;
    module.header = m.prepared->header;
    module.compiler_name = "xbox 360 cl";
    for (const std::string& name : m.prepared->order) {
        const auto va = m.address.find(name);
        const auto original = va == m.address.end() ? m.originals.end() : m.originals.find(va->second);
        const auto text = m.prepared->texts.find(name);
        if (original == m.originals.end() || text == m.prepared->texts.end()) continue;
        module.targets.push_back({name, m.lane().key.at(name), text->second, original->second});
    }
    rp::RunOptions options;
    options.flags = arguments.flags;
    options.work = work;
    options.limit = arguments.first;
    options.log = [](std::string_view line) { cb::out("{}\n", line); };
    options.record = [&](const rp::Target& target, const rp::Repaired& r, const std::string& setting) {
        store.record_variant(id.sha, target.key, setting, r.source, static_cast<std::int64_t>(r.verdict.differing), static_cast<std::int64_t>(r.verdict.total),
                             verify::word(r.verdict.match), r.knobs, std::nullopt, std::string_view());
    };
    const rp::Counts counts = rp::run(module, cl, options);
    cb::out("{}\n", rp::summary_line(fs::to_text(path.filename()), counts));
    return 0;
}

// ---- repair --truth <compiler> [target] -------------------------------------------------------------------------------------------------------------
//
// The period compilers a machine has, and the units each gives to compare with (build/period): the 32-bit compiler of 1994 and Microsoft C of 1990-93
// compile one function at a time and have no units of their own; they are tried on the modules `repair` is given. Only the XENIX C compiler has units --
// the source files of the reconstruction of LINK and LIB, which the xcc script (the setting xenix_c's folder) finds the original executable's stretch for.

namespace {

std::vector<std::string> ppc_period_names(World& world) {
    std::vector<std::string> have;
    std::error_code ec;
    const std::string period = world.config().text("period_compiler");
    if (!period.empty() && stdfs::is_regular_file(fs::from_text(period) / "CL.EXE", ec)) have.push_back("msvc2");
    for (const std::string key : {"msc6", "msvc152"}) {
        const std::optional<stdfs::path> home = world.config().path(key);
        if (home && stdfs::is_regular_file(*home / "BIN" / "CL.EXE", ec)) have.push_back(key);
    }
    return have;
}

}  // namespace

int repair_truth_command(World& world, const std::vector<std::string>& words) {
    const std::vector<std::string> have = ppc_period_names(world);
    if (words.empty()) throw UsageError("repair --truth <compiler> [target]   (have: " + text::join(have, ", ") + ")");
    const std::string& name = words.front();
    if (std::find(have.begin(), have.end(), name) == have.end()) {
        if (name == "xenix-c" || name == "msc203")
            throw UsageError("repair --truth " + name + ": the units of this compiler come from the reconstruction's own scripts (xenix_c's xcc.py), which this program does not run");
        throw UsageError(std::format("no period compiler '{}' set up (have: {})", name, have.empty() ? std::string("none") : text::join(have, ", ")));
    }
    rp::Profile profile;
    profile.isa = rp::Isa::X86_32;
    profile.units = [] { return std::vector<rp::Unit>{}; };
    profile.compile = [](std::string_view, const stdfs::path&) { return std::optional<rp::Code>(); };
    profile.distance = [](const rp::Code&, const rp::Code&) { return verify::authored::search16::ByteVerdict{}; };
    const rp::TruthCounts t = rp::truth(profile, world.config().state("repair") / "truth", 0, [](std::string_view line) { cb::out("{}\n", line); });
    const std::vector<std::string> given(words.begin(), words.begin() + static_cast<std::ptrdiff_t>(std::min<std::size_t>(words.size(), 2)));
    cb::out("{}: {} sources, {} identical as they stand; {} knocked away from the original, {} brought back\n", text::join(given, " "), t.units, t.identical, t.knocked,
            t.brought_back);
    return 0;
}

}  // namespace cb::source::pipeline
