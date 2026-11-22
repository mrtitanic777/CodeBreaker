// `match <16-bit module>`: does Microsoft C turn the reconstruction of a 16-bit module back into its bytes?
//
// The production assembly of verify/authored/match16 (the driver: what to ask the compiler, in what order, and what to keep). Made here from the
// settings and the module's file:
//
//   - the toolchain: build/msc6's compile_many over Microsoft C 6.0a (on the DOS executive) or Visual C++ 1.52c's C 8.00c (on Windows itself), and the
//     comparison of compare/mask, the built code's padding trimmed;
//   - the original's functions: for each, its bytes with every word a relocation record of its segment names left out, and the distance of every
//     near call and jump to another function (the NE file's relocation chains, and the spans of verify/lanes/win16);
//   - the ways of saying each function (the passes of build/msc6) and the header the compiler is given;
//   - the compiler's own run-time library, found by its bytes in the libraries of Visual C++ 1.52c (LIB/MDLLCEW.LIB ...).
#include <algorithm>
#include <chrono>
#include <map>
#include <set>

#include "base/cache.hpp"
#include "build/msc6.hpp"
#include "exact/emu/native_dos_executive.hpp"
#include "compare/mask.hpp"
#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/log.hpp"
#include "core/text.hpp"
#include "formats/ne.hpp"
#include "platforms/decoder.hpp"
#include "source/pipeline_match16.hpp"
#include "symbols/libraries/omf.hpp"
#include "symbols/libraries/package.hpp"
#include "symbols/wine16.hpp"
#include "symbols/workbench.hpp"
#include "verify/lanes/win16.hpp"
#include "verify/verdicts.hpp"

namespace cb::source::pipeline {

namespace stdfs = std::filesystem;

namespace {

namespace am16 = verify::authored::match16;
namespace m6 = build::msc6;

// The switches tried for Microsoft C 8.00c (match.ARCH["x86-16"].flag_sets).
constexpr std::string_view kMsc8FlagSets[] = {"/AM /Gs /Od",  "/AM /G2s /Ox", "/AM /G2s /Oxs", "/AM /G2s /Os", "/AM /Gs /Ox",
                                              "/AM /Gs /Oxs", "/AM /Gsw /Od", "/AM /G2sw /Ox", "/AM /G3s /Ox", "/AM /G3s /Oxs"};

// ---- the tools -----------------------------------------------------------------------------------------------------------------------

m6::Tools m16p_tools(const config::Config& config) {
    m6::Tools t;
    t.msc6 = config.path("msc6").value_or(stdfs::path());
    t.msvc152 = config.path("msvc152").value_or(stdfs::path());
    const std::string native = text::lower(config.text("native16"));
    t.native16 = !(native == "off" || native == "0" || native == "false" || native == "no");
    t.native16_tools = config.path("native16_tools");
    t.native16_work = config.path("native16_work");
    return t;
}

std::optional<m6::Compiler> m16p_compiler_of(std::string_view name) {
    if (name == "msc6") return m6::Compiler::Msc6;
    if (name == "msvc152") return m6::Compiler::Msvc152;
    return std::nullopt;
}

// Without the padding a compiler puts after a function (int 3, nop, zero bytes).
am16::Masked m16p_trim(am16::Masked code) {
    std::size_t n = code.code.size();
    const auto padding = [](std::uint8_t b) { return b == 0xCC || b == 0x90 || b == 0x00; };
    while (n > 1 && n <= code.mask.size() && code.mask[n - 1] == 0 && padding(code.code[n - 1]) && (code.code[n - 2] == 0xC3 || padding(code.code[n - 2]))) --n;
    code.code.resize(n);
    code.mask.resize(std::min(n, code.mask.size()));
    return code;
}

// Microsoft C, on the DOS executive or on Windows itself, and the comparison.
class M16Chain final : public am16::Toolchain {
public:
    // (the compilers on CodeBreaker's own DOS executive)
    M16Chain(m6::Tools tools, const config::Config& config, base::cache::Cache* cache, stdfs::path work)
        : tools_(std::move(tools)), dos_(config), runner_(exact::emu::dos_runner(config, dos_)), context_{tools_, *runner_, cache},
          work_(std::move(work)) {}

    std::vector<std::string> flag_sets(const std::string& compiler) const override {
        std::vector<std::string> out;
        if (compiler == "msc6") {
            for (const std::string_view s : m6::flag_sets()) out.emplace_back(s);
        } else {
            for (const std::string_view s : kMsc8FlagSets) out.emplace_back(s);
        }
        return out;
    }

    std::map<std::string, std::optional<am16::Masked>> compile(const am16::Compilation& request) override {
        const std::optional<m6::Compiler> compiler = m16p_compiler_of(request.compiler);
        if (!compiler) throw UsageError("Microsoft C is not set up (settings.json: msc6, msvc152)");
        std::vector<m6::Source> sources;
        std::map<std::string, std::size_t> index;   // function -> its place in `sources`
        for (const auto& [fn, text] : request.sources) {
            const auto it = index.find(fn);
            if (it != index.end()) {
                sources[it->second].text = text;
                continue;
            }
            index[fn] = sources.size();
            sources.push_back({text, fn});
        }
        m6::CompileOptions options;
        options.work = work_;
        options.compiler = *compiler;
        if (request.files != nullptr) {
            for (const std::vector<std::string>& file : *request.files) {
                std::vector<std::size_t> group;
                for (const std::string& fn : file)
                    if (const auto it = index.find(fn); it != index.end()) group.push_back(it->second);
                if (!group.empty()) options.together.push_back(std::move(group));
            }
        }
        fs::make_dirs(work_);
        const std::vector<std::optional<m6::Code>> built = m6::compile_many(context_, sources, request.header, request.switches, options);
        std::map<std::string, std::optional<am16::Masked>> out;
        for (const auto& [fn, i] : index) {
            if (built[i]) out[fn] = am16::Masked{built[i]->code, built[i]->mask};
            else out[fn] = std::nullopt;
        }
        return out;
    }

    am16::Verdict distance(const am16::Masked& built, const am16::Masked& original) const override {
        const am16::Masked b = m16p_trim(built);
        const compare::mask::DistanceResult d = compare::mask::distance(b.code, original.code, b.mask, original.mask);
        am16::Verdict v;
        v.match = d.verdict == compare::mask::Match::Identical ? verify::ByteMatch::Identical
                  : d.verdict == compare::mask::Match::SameSize ? verify::ByteMatch::SameSize
                                                                : verify::ByteMatch::Different;
        v.differing = d.differing;
        v.total = d.compared;
        return v;
    }

private:
    m6::Tools tools_;
    exact::emu::NativeDosExecutive dos_;
    std::unique_ptr<build::DosRunner> runner_;
    m6::Context context_;
    stdfs::path work_;
};

// ---- the original's functions -------------------------------------------------------------------------------------------------------------

// The bytes a 16-bit relocation of each source type covers.
std::size_t m16p_relocation_size(unsigned source) {
    switch (source) {
        case 0: return 1;
        case 2: return 2;
        case 3: return 4;
        case 5: return 2;
        case 11: return 6;
        case 13: return 4;
        default: return 2;
    }
}

// {address: bytes and mask} for a 16-bit module's functions: every word a relocation record of its segment names, and the distance of every near
// call and jump to another function. `where` {address: (segment, offset)}, `spans` as verify/lanes/win16 makes them.
std::map<std::uint64_t, am16::Masked> m16p_originals(const formats::ne::Module& nef, const std::map<std::uint64_t, core16::SegOff>& where,
                                                     const std::map<std::uint64_t, std::pair<verify::lanes::CodeSpan, std::int64_t>>& spans) {
    struct SegmentCode {
        ByteView code;
        Bytes relocated;
    };
    std::map<std::uint32_t, SegmentCode> by_segment;
    const ByteView file = nef.data();
    for (const formats::ne::Segment& s : nef.segments()) {
        SegmentCode sc;
        if (s.file_offset < file.size()) sc.code = file.subspan(s.file_offset, std::min<std::uint64_t>(s.length, file.size() - s.file_offset));
        sc.relocated.assign(sc.code.size(), 0);
        for (const formats::ne::Relocation& r : nef.relocations(s)) {
            const std::size_t n = m16p_relocation_size(r.source);
            std::int64_t at = r.offset;
            int seen = 0;
            while (at >= 0 && static_cast<std::size_t>(at) < sc.code.size() && seen < 0x4000) {   // (the places are chained through the code)
                const std::size_t here = static_cast<std::size_t>(at);
                const std::uint32_t next = here + 2 <= sc.code.size() ? static_cast<std::uint32_t>(sc.code[here]) | static_cast<std::uint32_t>(sc.code[here + 1]) << 8 : 0xFFFF;
                for (std::size_t k = 0; k < n && here + k < sc.code.size(); ++k) sc.relocated[here + k] = 1;
                if (r.additive() || next == 0xFFFF) break;
                at = next;
                ++seen;
            }
        }
        by_segment[s.index] = std::move(sc);
    }
    platforms::DecoderSet decoders;
    const platforms::Decoder& decoder = decoders.get(platforms::Arch::X86, 16);
    std::map<std::uint64_t, am16::Masked> out;
    for (const auto& [va, at] : where) {
        const auto span = spans.find(va);
        const auto segment = by_segment.find(at.seg);
        if (span == spans.end() || segment == by_segment.end()) continue;
        const std::uint32_t start = span->second.first.start, end = span->second.first.end;
        const SegmentCode& sc = segment->second;
        const std::size_t from = std::min<std::size_t>(start, sc.code.size());
        const std::size_t to = std::max(from, std::min<std::size_t>(end, sc.code.size()));
        am16::Masked piece;
        piece.code.assign(sc.code.begin() + static_cast<std::ptrdiff_t>(from), sc.code.begin() + static_cast<std::ptrdiff_t>(to));
        piece.mask.assign(sc.relocated.begin() + static_cast<std::ptrdiff_t>(from), sc.relocated.begin() + static_cast<std::ptrdiff_t>(to));
        for (const platforms::Insn& insn : decoder.disasm(piece.code, start)) {
            const std::size_t place = static_cast<std::size_t>(insn.address - start);
            if (place < piece.code.size() && (piece.code[place] == 0xE8 || piece.code[place] == 0xE9) && insn.size == 3 && place + 3 <= piece.code.size()) {   // a near call or jump: its distance
                const std::int16_t distance = static_cast<std::int16_t>(piece.code[place + 1] | piece.code[place + 2] << 8);
                const std::uint32_t target = static_cast<std::uint32_t>(static_cast<std::int64_t>(insn.address) + 3 + distance) & 0xFFFF;
                if (!(start <= target && target < end)) piece.mask[place + 1] = piece.mask[place + 2] = 1;
            }
        }
        out[va] = m16p_trim(std::move(piece));
    }
    return out;
}

// ---- the compiler's run-time library ------------------------------------------------------------------------------------------------------------

// Which of the module's functions are the library's, linked and not written: each routine of each library of Visual C++ 1.52c is set beside the
// start of each function (the longest routine that fits names it). A far call in a routine is compared as a far call (only its address left out).
std::map<std::string, std::string> m16p_identify(const stdfs::path& msvc152, std::span<const am16::Function> functions) {
    static constexpr std::string_view kLibraries[] = {"MDLLCEW.LIB", "MDLLCAW.LIB", "MNOCRTW.LIB", "MNOCRTDW.LIB", "MLIBCEW.LIB", "MLIBCAW.LIB",
                                                      "SDLLCEW.LIB", "SNOCRTW.LIB", "LDLLCEW.LIB", "LNOCRTW.LIB", "LIBW.LIB"};
    std::vector<symbols::libraries::Function> routines;
    for (const std::string_view lib : kLibraries) {
        const stdfs::path path = msvc152 / "LIB" / std::string(lib);
        std::error_code ec;
        if (!stdfs::is_regular_file(path, ec)) continue;
        symbols::libraries::Library library;
        try {
            library = symbols::libraries::read_file(path);
        } catch (const Error&) {
            continue;
        }
        for (symbols::libraries::Function& f : library.functions) {
            f.name = std::string(lib) + ":" + f.name;
            f.mask = symbols::libraries::omf::fields_only(f.mask);   // (a far call stays one)
            const std::size_t free = static_cast<std::size_t>(std::count(f.mask.begin(), f.mask.end(), std::uint8_t{0}));
            if (f.code.size() >= 6 && free >= 4) routines.push_back(std::move(f));
        }
    }
    return identify_run_time_library(std::move(routines), functions);
}

}  // namespace

std::map<std::string, std::string> identify_run_time_library(std::vector<symbols::libraries::Function> routines, std::span<const am16::Function> functions) {
    std::stable_sort(routines.begin(), routines.end(), [](const auto& a, const auto& b) { return a.code.size() > b.code.size(); });   // longest first
    std::map<std::uint32_t, std::vector<const symbols::libraries::Function*>> by_start;
    for (const symbols::libraries::Function& f : routines)
        by_start[static_cast<std::uint32_t>(f.code[0]) | static_cast<std::uint32_t>(f.code[1]) << 8].push_back(&f);
    std::map<std::string, std::string> out;
    for (const am16::Function& fn : functions) {
        const Bytes& code = fn.original.code;
        if (code.size() < 2) continue;
        const auto candidates = by_start.find(static_cast<std::uint32_t>(code[0]) | static_cast<std::uint32_t>(code[1]) << 8);
        if (candidates == by_start.end()) continue;
        for (const symbols::libraries::Function* f : candidates->second) {
            if (f->code.size() + 16 < code.size()) continue;   // (a routine much shorter than the function: not it)
            if (symbols::libraries::fits(*f, code, fn.original.mask)) {
                out[fn.name] = f->name;
                break;
            }
        }
    }
    return out;
}

// ---- the run ----------------------------------------------------------------------------------------------------------------------------------

Match16Run run_match16(World& world, const stdfs::path& module, const Match16Options& options, const am16::Log& log) {
    const config::Config& config = world.config();
    const m6::Tools tools = m16p_tools(config);
    std::vector<std::string> compilers;
    if (options.compiler) {
        compilers = {*options.compiler};
    } else {
        std::vector<std::string_view> wanted = {"msvc152"};   // (Microsoft C 6.0a is tried only when asked for: settings match16_msc6)
        if (config.truthy("match16_msc6")) wanted.insert(wanted.begin(), "msc6");
        for (const std::string_view c : wanted)
            if (const auto id = m16p_compiler_of(c); id && m6::available(tools, *id)) compilers.emplace_back(c);
    }
    const bool set_up = !compilers.empty() && std::all_of(compilers.begin(), compilers.end(), [&](const std::string& c) {
        const auto id = m16p_compiler_of(c);
        return id && m6::available(tools, *id);
    });
    if (!set_up) throw UsageError("Microsoft C is not set up (settings.json: msc6, msvc152)");

    const std::shared_ptr<const verify::lanes::Kind> lane = world.kit().win16_lane();
    if (!lane) throw UsageError("the 16-bit lane is not set up here (the native decompiler)");
    const std::string file_name = text::lower(fs::to_text(module.filename()));
    const stdfs::path folder = config.state(stdfs::path("match16") / file_name);
    fs::make_dirs(folder / "source");
    fs::make_dirs(folder / "work");
    const std::unique_ptr<verify::lanes::Prepared> prepared = lane->prepare(module, folder / "source");
    const auto& m = static_cast<const verify::lanes::Win16Lane::PreparedWin16&>(*prepared);

    // where each function is, and the original's bytes
    const OrderedMap<std::uint64_t, std::string> by_va = m.module->made.by_va();
    std::map<std::uint64_t, core16::SegOff> where;
    std::map<std::string, std::uint64_t> address;
    for (const auto& [va, name] : by_va) {
        const auto at = m.module->where.find(static_cast<std::int64_t>(va));
        if (at == m.module->where.end()) continue;
        where[va] = at->second;
        address[name] = va;
    }
    const formats::ne::Module nef = formats::ne::Module::open_file(module);
    const auto spans = verify::lanes::Win16Lane::spans(nef, where);
    const std::map<std::uint64_t, am16::Masked> originals = m16p_originals(nef, where, spans);

    Match16Run run;
    for (const std::string& name : m.order) {
        if (options.first != 0 && run.functions.size() >= options.first) break;
        const auto va = address.find(name);
        if (va == address.end()) continue;
        const auto original = originals.find(va->second);
        if (original == originals.end()) continue;
        const core16::SegOff at = where.at(va->second);
        run.functions.push_back({name, at.seg, at.off, original->second});
    }

    // the header: a C function imported by number is not pascal; the dialect Microsoft C takes; the pascal order of the arguments; the data segment's
    // variables
    symbols::Workbench bench(symbols::Environment{&world.corpus(), &config});
    const m6::OrdinalName ordinal_name = [&bench](std::string_view module_name, std::uint32_t ordinal) { return bench.ordinals().name(module_name, ordinal); };
    const std::optional<std::string> sdk_text = m6::windows_h(tools);
    const std::string source_header = m6::cdecl_imports(m.header, sdk_text.value_or(""), ordinal_name);
    std::string header = m6::dialect(source_header);
    std::vector<std::string> all_texts = {source_header};
    for (const auto& [name, text] : m.texts) all_texts.push_back(text);
    const std::set<std::string> pascal = m6::pascal_names(all_texts);
    header = m6::pascal_order(header, pascal);
    {
        const m6::DsGlobals g = m6::ds_globals(header);
        header = g.text + g.declarations;   // (after the typedefs; the header's macros use them)
    }
    run.header = header;

    // each function said several ways, and judged by the one nearest the original
    std::vector<std::string> stages;
    if (options.fold) stages = {"fold", "far", "frame"};
    if (options.sdk) stages.emplace_back("sdk");
    if (options.wine) stages.emplace_back("wine");   // (imports typed by Wine's .spec: symbols/wine16)
    std::vector<am16::Variant> variants = am16::make_variants(stages);
    const m6::Prototypes protos = options.sdk ? m6::sdk_prototypes(sdk_text.value_or("")) : m6::Prototypes{};
    const symbols::wine16::Specs wine_specs(options.wine ? fs::from_text(config.text("wine16")) : stdfs::path());
    std::map<std::string, const symbols::wine16::Export*> own;   // the module's own exports, by their upper-case names
    if (options.wine) {
        const std::string stem = file_name.substr(0, file_name.find('.'));
        for (const auto& [ordinal, e] : wine_specs.exports(stem)) own[text::upper(e.name)] = &e;
    }
    const m6::ExportLookup lookup = m6::lookup_in(wine_specs);
    const auto said = [&](const std::string& fn, const std::set<std::string>& on) {
        std::string t = m.texts.at(fn);
        if (on.contains("wine")) {   // (the module's own export, typed by Wine's .spec)
            const auto e = own.find(text::upper(fn));
            if (e != own.end()) t = m6::wine_definition(t, e->second);
        }
        if (on.contains("fold")) t = m6::fold_temporaries(t);
        if (on.contains("far")) t = m6::join_far_parameters(t);
        if (on.contains("frame")) t = m6::split_frame(t);
        t = m6::pascal_order(t, pascal);
        if (on.contains("sdk")) t = m6::sdk_calls(t, protos, ordinal_name);
        const m6::DsGlobals g = m6::ds_globals(t);
        return g.declarations + g.text;
    };
    for (am16::Variant& v : variants) {
        OrderedMap<std::string, std::string> texts;
        for (const am16::Function& f : run.functions) texts.insert_or_assign(f.name, said(f.name, v.stages));
        std::string head = header;
        if (v.stages.contains("wine")) {
            m6::WineTyped typed = m6::wine_typing(header, texts, lookup);
            head = std::move(typed.header);
            texts = std::move(typed.texts);
        }
        v.header = v.stages.contains("sdk") ? m6::sdk_header(head, sdk_text) : head;
        for (const auto& [fn, t] : texts) v.texts[fn] = t;
    }

    run.toolchain = std::make_unique<M16Chain>(tools, config, &world.kit().cache(), folder / "work");
    am16::Inputs in;
    in.functions = run.functions;
    in.variants = std::move(variants);
    in.header = header;
    in.compilers = compilers;
    if (options.flags && !options.flags->empty()) in.switches = *options.flags;
    in.choose = options.choose;
    in.compiler_given = options.compiler.has_value();
    in.choices_file = folder / "choices.json";
    in.toolchain = run.toolchain.get();
    const stdfs::path msvc152 = tools.msvc152;
    in.identify = [msvc152](std::span<const am16::Function> fns) { return m16p_identify(msvc152, fns); };
    in.search_rounds = options.search;
    in.log = log;
    run.result = am16::run(in);
    return run;
}

int match16_command(World& world, const stdfs::path& module, const cli::Args& args) {
    Match16Options options;
    if (const auto flags = args.value("flags")) options.flags = *flags;
    if (const auto compiler = args.value("compiler")) options.compiler = *compiler;
    options.first = static_cast<std::size_t>(args.integer<std::int64_t>("first").value_or(0));
    options.choose = args.flag("choose");
    options.sdk = args.flag("sdk");
    options.fold = !args.flag("no-fold");
    options.wine = args.flag("wine");
    options.search = static_cast<int>(args.integer<std::int64_t>("search").value_or(0));
    const Match16Run run = run_match16(world, module, options, [](std::string_view line) { cb::out("{}\n", line); });
    for (const std::string& line : am16::report_lines(fs::to_text(module.filename()), run.result)) cb::out("{}\n", line);
    if (const auto json_file = args.value("json")) {
        Json j = Json::object();
        for (const auto& [name, v] : run.result.verdicts) {
            const auto used = run.result.setting.find(name);
            const auto segment = run.result.segment.find(name);
            j[name] = Json{{"verdict", std::string(verify::word(v.match))},
                           {"differing", v.differing},
                           {"total", v.total},
                           {"segment", segment == run.result.segment.end() ? 0U : segment->second},
                           {"flags", used == run.result.setting.end() ? std::string() : used->second.compiler + " " + used->second.switches}};
        }
        fs::write_text(fs::from_text(*json_file), json::dump(j, json::Style::Pretty));
    }
    return 0;
}

}  // namespace cb::source::pipeline
