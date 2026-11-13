// codebreaker/verify/authored/repair.hpp -- COMPAT SHIM over cb/verify/authored/repair.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/verify/authored/repair.{hpp,cpp}: the search (knobs, scores by instruction, the beam), the driver over a module's
// functions and the test of the search on known sources. This header keeps what the old code calls over the old py:: types, header-only: the
// words of a verdict, the (bytes, mask) pairs, the `bits` of the Python (0, 16, 32 or BITS_PPC for "ppc"). What the module cannot know --
// match's compilers and comparison, the lanes' prepared modules, the function store -- is glue here that builds the module's `Module`,
// `Compiler` and `Record` from the old world and hands them to the new `run`.
//
// DIVERGES: the search no longer takes a Python generator's order of work from the old module -- it is the same algorithm, same knobs, same
// beam; the switches chosen by best_flags on a tie are the first of the sets (the Python's PowerPC run took the larger name); a function
// whose draft does not compile when `truth` runs is skipped (the Python raised); the `--json` option is accepted and ignored as before.
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>

#include "cb/base/ctext.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/verify/authored/repair.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/functions.hpp"
#include "codebreaker/decompile/c_text.hpp"
#include "codebreaker/engine/native_engine.hpp"
#include "codebreaker/formats/pe.hpp"
#include "codebreaker/formats/pe_functions.hpp"
#include "codebreaker/formats/xex.hpp"
#include "codebreaker/store/functions.hpp"
#include "codebreaker/translate/l2.hpp"
#include "codebreaker/verify/authored/match.hpp"
#include "codebreaker/formats/powerpc_image.hpp"
#include "cb/compat/old_lane_kit.hpp"
#include "cb/verify/lanes/powerpc.hpp"
#include "codebreaker/verify/lanes/win32.hpp"
#include "codebreaker/verify/reconstruct.hpp"
#include "codebreaker/verify/verdicts.hpp"
#include "py.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::verify::authored::repair {

namespace nw = cb::verify::authored::repair;

using match::Masked;
using Log = std::function<void(const py::str&)>;
inline void log_print(const py::str& line) { py::print(line); }
using FunctionStore = engine::native_engine::FunctionStore;

inline constexpr int64_t MAX_LOCALS = static_cast<int64_t>(nw::kMaxLocals);
inline constexpr int64_t MAX_SITES = static_cast<int64_t>(nw::kMaxSites);
inline constexpr int64_t MAX_WORK = static_cast<int64_t>(nw::kMaxWork);
inline constexpr int64_t BEAM = static_cast<int64_t>(nw::kBeam);
inline constexpr int64_t PATIENCE = static_cast<int64_t>(nw::kPatience);
inline constexpr int64_t BITS_PPC = -1;   // bits="ppc"

namespace detail_shim {

inline nw::Isa isa_of(int64_t bits) { return bits == BITS_PPC ? nw::Isa::PowerPC : bits == 32 ? nw::Isa::X86_32 : bits == 16 ? nw::Isa::X86_16 : nw::Isa::Unknown; }

inline nw::Code code_of(const Masked& m) {
    nw::Code c;
    c.bytes = cb::compat::from_py(std::get<0>(m));
    if (std::holds_alternative<py::bytes>(std::get<1>(m))) {
        c.mask = cb::compat::from_py(std::get<py::bytes>(std::get<1>(m)));
    } else {
        for (const int64_t w : std::get<py::list<int64_t>>(std::get<1>(m))) c.word_mask.push_back(static_cast<std::uint32_t>(w));
    }
    return c;
}
inline nw::Code code_of(const match::ByteMasked& m) { return code_of(Masked(std::get<0>(m), match::Mask(std::get<1>(m)))); }
inline nw::Code code_of(const match::WordMasked& m) { return code_of(Masked(std::get<0>(m), match::Mask(std::get<1>(m)))); }

inline Masked masked_of(const nw::Code& c, bool ppc) {
    if (ppc) {
        py::list<int64_t> words;
        for (const std::uint32_t w : c.word_mask) words.append(static_cast<int64_t>(w));
        return Masked(cb::compat::to_py(cb::ByteView(c.bytes)), match::Mask(words));
    }
    return Masked(cb::compat::to_py(cb::ByteView(c.bytes)), match::Mask(cb::compat::to_py(cb::ByteView(c.mask))));
}

inline nw::ByteVerdict verdict_of(const std::tuple<py::str, int64_t, int64_t, int64_t>& d) {
    const auto match_word = cb::verify::parse_byte_match(std::get<0>(d).utf8());
    return {match_word.value_or(cb::verify::ByteMatch::NoCompile), static_cast<std::size_t>(std::get<1>(d)), static_cast<std::size_t>(std::get<2>(d))};
}

// match.distance for the code of a processor
inline nw::Distance distance_for(int64_t bits) {
    const bool ppc = bits == BITS_PPC;
    return [ppc](const nw::Code& built, const nw::Code& original) {
        return verdict_of(ppc ? match::distance(masked_of(built, true), masked_of(original, true), py::str("powerpc"))
                              : match::distance(masked_of(built, false), masked_of(original, false)));
    };
}

inline std::string word_text(const std::string& bytes) { return bytes; }

inline std::vector<std::string> insns_text(const std::variant<py::list<py::bytes>, py::list<int64_t>>& v) {
    std::vector<std::string> out;
    if (std::holds_alternative<py::list<py::bytes>>(v)) {
        for (const py::bytes& b : std::get<py::list<py::bytes>>(v)) out.push_back(b.raw());
    } else {
        for (const int64_t w : std::get<py::list<int64_t>>(v))
            out.push_back(std::string{static_cast<char>(w >> 24), static_cast<char>(w >> 16), static_cast<char>(w >> 8), static_cast<char>(w)});
    }
    return out;
}

inline std::filesystem::path path_of(const py::str& s) { return std::filesystem::u8path(s.utf8()); }
inline py::str str_of(const std::filesystem::path& p) { return cb::compat::to_py(p.string()); }

// match's 32-bit period compiler and its comparison, as the module's Compiler
class X86Compiler final : public nw::Compiler {
  public:
    X86Compiler(py::str cl, py::dict<py::str, py::str> env) : cl_(std::move(cl)), env_(std::move(env)) {}
    std::optional<nw::Code> compile(std::string_view header, std::string_view source, std::string_view name, std::string_view flags,
                                    const std::filesystem::path& work) const override {
        auto built = match::compile_function(cl_, env_, cb::compat::to_py(header), cb::compat::to_py(source), cb::compat::to_py(name), str_of(work),
                                             cb::compat::to_py(flags));
        if (!built) return std::nullopt;
        return code_of(*built);
    }
    nw::ByteVerdict distance(const nw::Code& built, const nw::Code& original) const override { return distance_for(32)(built, original); }
    std::vector<std::string> flag_sets() const override {
        std::vector<std::string> out;
        for (const py::str& f : match::FLAG_SETS) out.push_back(f.utf8());
        return out;
    }
    std::string needed_header(std::string_view header, std::string_view source) const override {
        return match::needed_header(cb::compat::to_py(header), cb::compat::to_py(source)).utf8();
    }

  private:
    py::str cl_;
    py::dict<py::str, py::str> env_;
};

class PowerPcCompiler final : public nw::Compiler {
  public:
    PowerPcCompiler(py::str cl) : cl_(std::move(cl)) {}
    std::optional<nw::Code> compile(std::string_view header, std::string_view source, std::string_view name, std::string_view flags,
                                    const std::filesystem::path& work) const override {
        auto built = match::compile_powerpc(cl_, cb::compat::to_py(header), cb::compat::to_py(source), cb::compat::to_py(name), str_of(work),
                                            cb::compat::to_py(flags));
        if (!built) return std::nullopt;
        return code_of(*built);
    }
    nw::ByteVerdict distance(const nw::Code& built, const nw::Code& original) const override { return distance_for(BITS_PPC)(built, original); }
    std::vector<std::string> flag_sets() const override {
        std::vector<std::string> out;
        for (const py::str& f : match::POWERPC_FLAG_SETS) out.push_back(f.utf8());
        return out;
    }

  private:
    py::str cl_;
};

inline py::str verdict_word(const nw::Repaired& r) { return cb::compat::to_py(cb::verify::word(r.verdict.match)); }

inline void record_into(const std::shared_ptr<FunctionStore>& fs, const std::string& sha, const py::dict<py::str, py::str>& behaves, const nw::Target& t,
                        const nw::Repaired& r, const std::string& setting) {
    namespace eng = codebreaker::engine::native_engine;
    const py::str key = cb::compat::to_py(t.key);
    fs->record_variant(py::str(sha), key, cb::compat::to_py(setting), cb::compat::to_py(r.source), eng::Cell(static_cast<int64_t>(r.verdict.differing)),
                       eng::Cell(static_cast<int64_t>(r.verdict.total)), verdict_word(r), cb::compat::to_py(r.knobs), eng::Cell(), behaves.get(key, py::str("")));
}

inline nw::Log log_of(const Log& log) {
    if (!log) return {};
    return [log](std::string_view line) { log(cb::compat::to_py(line)); };
}

}  // namespace detail_shim

// (everything up to and including the opening brace, the declaration lines, the rest).
inline std::tuple<py::str, py::list<py::str>, py::str> _split(const py::str& source) {
    const nw::Split s = nw::split(source.utf8());
    py::list<py::str> decls;
    for (const std::string& d : s.decls) decls.append(cb::compat::to_py(d));
    return {cb::compat::to_py(s.head), decls, cb::compat::to_py(s.body)};
}

inline int64_t _frame(const py::bytes& code, int64_t bits = 32) {
    nw::Searcher s(detail_shim::isa_of(bits), detail_shim::distance_for(bits));
    return static_cast<int64_t>(s.frame(cb::compat::from_py(code)));
}

using Insns = std::variant<py::list<py::bytes>, py::list<int64_t>>;
inline Insns instructions(const Masked& built, int64_t bits) {
    nw::Searcher s(detail_shim::isa_of(bits), detail_shim::distance_for(bits));
    const std::vector<std::string> v = s.instructions(detail_shim::code_of(built));
    if (bits == BITS_PPC) {
        py::list<int64_t> out;
        for (const std::string& w : v)
            out.append((static_cast<int64_t>(static_cast<unsigned char>(w[0])) << 24) | (static_cast<int64_t>(static_cast<unsigned char>(w[1])) << 16) |
                       (static_cast<int64_t>(static_cast<unsigned char>(w[2])) << 8) | static_cast<unsigned char>(w[3]));
        return out;
    }
    py::list<py::bytes> out;
    for (const std::string& i : v) out.append(py::bytes(i));
    return out;
}

using Distance = std::tuple<py::str, int64_t, int64_t>;
using Score = std::tuple<std::tuple<int64_t, int64_t>, Distance>;
inline Score score(const Masked& built, const Masked& original, int64_t bits = 0, const std::optional<Insns>& target = std::nullopt) {
    nw::Searcher s(detail_shim::isa_of(bits), detail_shim::distance_for(bits));
    std::optional<std::vector<std::string>> want;
    if (target) want = detail_shim::insns_text(*target);
    const nw::Searcher::Score r = s.score(detail_shim::code_of(built), detail_shim::code_of(original), want ? &*want : nullptr);
    return {std::make_tuple(static_cast<int64_t>(r.rank.first), static_cast<int64_t>(r.rank.second)),
            Distance(cb::compat::to_py(cb::verify::word(r.verdict.match)), static_cast<int64_t>(r.verdict.differing), static_cast<int64_t>(r.verdict.total))};
}

inline py::str _blank(const py::str& text) { return cb::compat::to_py(cb::base::ctext::blank(text.utf8(), cb::base::ctext::Mode::WholeClosed)); }

inline int64_t _depth(const py::str& shadow, int64_t at) {
    int64_t d = 0;
    const std::string s = shadow.utf8();
    for (int64_t i = 0; i < at && i < static_cast<int64_t>(s.size()); ++i) d += (s[static_cast<size_t>(i)] == '(') - (s[static_cast<size_t>(i)] == ')');
    return d;
}

inline py::list<std::tuple<int64_t, int64_t, py::str>> _tests(const py::str& shadow, const py::str& body) {
    py::list<std::tuple<int64_t, int64_t, py::str>> out;
    for (const nw::TestSwap& t : nw::test_swaps(shadow.utf8(), body.utf8()))
        out.append(std::make_tuple(static_cast<int64_t>(t.begin), static_cast<int64_t>(t.end), cb::compat::to_py(t.text)));
    return out;
}

inline std::optional<py::str> _toggle_register(const py::str& line) {
    const auto r = nw::toggle_register(line.utf8());
    if (!r) return std::nullopt;
    return cb::compat::to_py(*r);
}

inline py::list<std::tuple<py::str, py::str>> _knobs(const py::str& source, int64_t target_frame, const std::optional<int64_t>& draft_frame) {
    std::optional<std::size_t> draft;
    if (draft_frame) draft = static_cast<std::size_t>(*draft_frame);
    py::list<std::tuple<py::str, py::str>> out;
    for (const nw::Knob& k : nw::knobs(source.utf8(), static_cast<std::size_t>(target_frame), draft))
        out.append(std::make_tuple(cb::compat::to_py(k.name), cb::compat::to_py(k.source)));
    return out;
}

// What the search found for one function.
struct Repaired {
    py::str source;
    py::str verdict;
    int64_t distance = 0;
    int64_t total = 0;
    py::str knobs;
    int64_t tried = 0;
};
namespace detail_shim {
inline Repaired old_repaired(const nw::Repaired& r) {
    return {cb::compat::to_py(r.source), verdict_word(r), static_cast<int64_t>(r.verdict.differing), static_cast<int64_t>(r.verdict.total),
            cb::compat::to_py(r.knobs), static_cast<int64_t>(r.tried)};
}
}  // namespace detail_shim

using Compile = std::function<std::optional<Masked>(const py::str& text)>;

inline std::optional<Repaired> repair(const Compile& compile, const py::str& source, const Masked& original, int64_t bits = 0) {
    nw::Searcher s(detail_shim::isa_of(bits), detail_shim::distance_for(bits));
    const bool ppc = bits == BITS_PPC;
    (void)ppc;
    const auto r = s.repair(
        [&](std::string_view text) -> std::optional<nw::Code> {
            auto m = compile(cb::compat::to_py(text));
            if (!m) return std::nullopt;
            return detail_shim::code_of(*m);
        },
        source.utf8(), detail_shim::code_of(original));
    if (!r) return std::nullopt;
    return detail_shim::old_repaired(*r);
}

inline std::optional<Repaired> repair_function(const py::str& cl, const py::dict<py::str, py::str>& env, const py::str& header, const py::str& source,
                                               const py::str& name, const Masked& original, const py::str& work, const py::str& flags) {
    const detail_shim::X86Compiler cc(cl, env);
    const nw::Target target{name.utf8(), "", source.utf8(), detail_shim::code_of(original)};
    const auto r = nw::repair_function(cc, header.utf8(), target, flags.utf8(), detail_shim::path_of(work));
    if (!r) return std::nullopt;
    return detail_shim::old_repaired(*r);
}

// A period compiler's unit: a source known to be the authored one, and the bytes it compiled to.
struct Unit {
    py::str name;
    py::str source;
    Masked original;
};
struct Profile {
    std::function<py::list<Unit>()> units;
    std::function<std::optional<Masked>(const py::str& text, const py::str& work)> compile;
    int64_t bits = 0;
};
struct Periods {
    std::function<py::list<py::str>()> names;
    std::function<Profile(const py::list<py::str>& args)> get;
};

// settings.state("repair") -- the folder repairs are worked in
inline py::str _scratch() {
    const py::str path = py::os::path::join(py::settings::get_str(py::str("state")), py::str("repair"));
    py::os::makedirs(py::os::path::dirname(path), 0777, true);
    return path;
}

inline std::tuple<int64_t, int64_t, int64_t, int64_t> truth(const Profile& profile, int64_t limit = 0, const Log& log = log_print) {
    nw::Profile p;
    p.isa = detail_shim::isa_of(profile.bits);
    p.distance = detail_shim::distance_for(profile.bits);
    p.units = [&] {
        std::vector<nw::Unit> out;
        for (const Unit& u : profile.units()) out.push_back({u.name.utf8(), u.source.utf8(), detail_shim::code_of(u.original)});
        return out;
    };
    p.compile = [&](std::string_view text, const std::filesystem::path& work) -> std::optional<nw::Code> {
        auto m = profile.compile(cb::compat::to_py(text), detail_shim::str_of(work));
        if (!m) return std::nullopt;
        return detail_shim::code_of(*m);
    };
    const nw::TruthCounts t =
        nw::truth(p, detail_shim::path_of(py::os::path::join(_scratch(), py::str("truth"))), static_cast<std::size_t>(limit), detail_shim::log_of(log));
    return {static_cast<int64_t>(t.units), static_cast<int64_t>(t.identical), static_cast<int64_t>(t.knocked), static_cast<int64_t>(t.brought_back)};
}

inline py::str best_flags(const py::str& cl, const py::dict<py::str, py::str>& env, const py::str& header, const py::dict<py::str, py::str>& texts,
                          const py::list<py::str>& order, const py::dict<int64_t, match::ByteMasked>& originals, const py::dict<py::str, int64_t>& address,
                          const py::str& work, int64_t sample = 60) {
    nw::Module m;
    m.isa = nw::Isa::X86_32;
    m.header = header.utf8();
    for (const py::str& n : order)
        if (originals.contains(address.at(n))) m.targets.push_back({n.utf8(), "", texts.at(n).utf8(), detail_shim::code_of(originals.at(address.at(n)))});
    return cb::compat::to_py(nw::best_flags(detail_shim::X86Compiler(cl, env), m, detail_shim::path_of(work), static_cast<std::size_t>(sample)));
}

// (functions, identical, same size, different)
using Counts = std::tuple<int64_t, int64_t, int64_t, int64_t>;
namespace detail_shim {
inline Counts old_counts(const nw::Counts& c) {
    return {static_cast<int64_t>(c.functions), static_cast<int64_t>(c.identical), static_cast<int64_t>(c.same_size), static_cast<int64_t>(c.different)};
}
}  // namespace detail_shim

// run() for a PowerPC image, with the Xbox 360 compiler (verify/authored/match.py).
inline Counts run_powerpc(const py::str& module_path, const std::optional<py::str>& flags, const py::str& work, const std::shared_ptr<FunctionStore>& fs_in = nullptr,
                          const Log& log = log_print, int64_t limit = 0) {
    auto tools = build::functions::powerpc_tools();
    if (!tools) throw py::SystemExit("no PowerPC compiler (settings.json: powerpc_tools, powerpc_lib)");
    const py::str cl = std::get<0>(*tools);
    const py::str sha = base::corpus::sha(module_path).value_or(py::str(""));
    auto fs = fs_in ? fs_in : store::functions::open();
    // The module decompiled and made compilable by the PowerPC lane (the lane kit's), and the original's words of each function.
    const std::shared_ptr<const cb::verify::lanes::Kind> lane = cb::compat::old_lane_kit().powerpc_lane();
    if (!lane) throw py::SystemExit("no PowerPC compiler (settings.json: powerpc_tools, powerpc_lib)");
    const std::unique_ptr<cb::verify::lanes::Prepared> made = cb::compat::guard([&] { return lane->prepare(detail_shim::path_of(module_path), detail_shim::path_of(py::os::path::join(work, py::str("source")))); });
    const auto& prepared = dynamic_cast<const cb::verify::lanes::PowerPcLane::PreparedPowerPc&>(*made);
    py::dict<py::str, int64_t> address;
    for (const auto& [va, name] : prepared.module.made.by_va()) address.set(cb::compat::to_py(name), static_cast<int64_t>(va));
    std::vector<int64_t> vas;
    for (const int64_t v : address.values()) vas.push_back(v);
    std::sort(vas.begin(), vas.end());
    const auto& arch = match::ARCH().at(py::str("powerpc"));
    const auto image = formats::powerpc_image::read(module_path);
    const py::dict<int64_t, match::WordMasked> originals = std::get<match::OriginalsPPC>(arch.originals)(*image, py::list<int64_t>(vas));
    fs->add_module(sha, py::os::path::basename(module_path).lower(), module_path, py::str("powerpc"));
    {
        py::list<py::str> keys;
        for (const py::str& n : address.keys()) keys.append(cb::compat::to_py(prepared.key.at(n.utf8())));
        fs->add_functions(sha, keys);
    }
    nw::Module module;
    module.isa = nw::Isa::PowerPC;
    module.header = prepared.header;
    module.compiler_name = "xbox 360 cl";
    for (const std::string& n : prepared.order)
        if (originals.contains(address.at(cb::compat::to_py(n))))
            module.targets.push_back({n, prepared.key.at(n), prepared.texts.at(n), detail_shim::code_of(originals.at(address.at(cb::compat::to_py(n))))});
    nw::RunOptions options;
    if (flags) options.flags = flags->utf8();
    options.work = detail_shim::path_of(work);
    options.limit = static_cast<std::size_t>(limit);
    options.log = detail_shim::log_of(log);
    options.record = [&](const nw::Target& t, const nw::Repaired& r, const std::string& setting) {
        detail_shim::record_into(fs, sha.utf8(), py::dict<py::str, py::str>(), t, r, setting);
    };
    return detail_shim::old_counts(nw::run(module, detail_shim::PowerPcCompiler(cl), options));
}

// Repair every function of a module and record the variants. `flags` None: the switches are found first (best_flags).
inline Counts run(const py::str& module_path, const std::optional<py::str>& flags, const py::str& work, const std::shared_ptr<FunctionStore>& fs_in = nullptr,
                  const Log& log = log_print, int64_t limit = 0) {
    const py::str cl = match::compiler();
    if (cl.empty()) throw py::SystemExit("no period compiler (settings.json: period_compiler)");
    const py::str sha = base::corpus::sha(module_path).value_or(py::str(""));
    auto fs = fs_in ? fs_in : store::functions::open();
    const py::dict<py::str, py::str> env = match::compiler_env(cl);
    reconstruct::Prepared prepared = lanes::win32::prepare(module_path, py::os::path::join(work, py::str("source")));
    const auto m = std::static_pointer_cast<decompile::c_text::Module32>(prepared.lane);
    py::dict<py::str, int64_t> address;
    for (const auto& kv : m->by_va().items()) address.set(std::get<1>(kv), std::get<0>(kv));
    std::vector<int64_t> vas;
    for (const int64_t v : address.values()) vas.push_back(v);
    std::sort(vas.begin(), vas.end());
    // (match::original_functions reads the functions' extents through a hook: pe_functions.function_spans, which has its port)
    match::set_function_spans([](const py::str& path) {
        py::dict<int64_t, int64_t> out;
        for (const auto& kv : formats::pe_functions::function_spans(path).items()) out.set(std::get<0>(kv), std::get<0>(std::get<1>(kv)));
        return out;
    });
    const py::dict<int64_t, match::ByteMasked> originals = match::original_functions(module_path, py::list<int64_t>(vas));
    py::dict<py::str, py::str> behaves;
    for (const auto& r : fs->candidates(sha, std::get<0>(lanes::win32::settings_profile(py::str("pe"))))) {
        if (std::holds_alternative<py::str>(r.addr) && std::holds_alternative<py::str>(r.verdict)) behaves.set(std::get<py::str>(r.addr), std::get<py::str>(r.verdict));
    }
    fs->add_module(sha, py::os::path::basename(module_path).lower(), module_path, py::str("pe"));
    {
        py::list<py::str> keys;
        for (const int64_t a : address.values()) keys.append(lanes::win32::LANES.at(py::str("pe")).key(a));
        fs->add_functions(sha, keys);
    }
    // the declarations a period compiler is given: imports as imports, globals as variables
    py::set<py::str> imports;
    for (const py::str& i : m->imports) imports.add(i);
    auto [header, texts] = match::period_source(prepared.header, prepared.texts, imports, match::relocation_targets(module_path));
    nw::Module module;
    module.isa = nw::Isa::X86_32;
    module.header = header.utf8();
    module.compiler_name = py::os::path::basename(cl).lower().utf8();
    for (const py::str& n : prepared.order)
        if (originals.contains(address.at(n)))
            module.targets.push_back({n.utf8(), lanes::win32::LANES.at(py::str("pe")).key(address.at(n)).utf8(), texts.at(n).utf8(),
                                      detail_shim::code_of(originals.at(address.at(n)))});
    nw::RunOptions options;
    if (flags) options.flags = flags->utf8();
    options.work = detail_shim::path_of(work);
    options.limit = static_cast<std::size_t>(limit);
    options.log = detail_shim::log_of(log);
    options.record = [&](const nw::Target& t, const nw::Repaired& r, const std::string& setting) {
        detail_shim::record_into(fs, sha.utf8(), behaves, t, r, setting);
    };
    return detail_shim::old_counts(nw::run(module, detail_shim::X86Compiler(cl, env), options));
}

inline const py::str DOC = py::str(std::string(nw::kUsage));

// python -m codebreaker repair <module> [--flags "/O2 /Gz"] [--first N]   (--truth <compiler> [target])
inline int64_t main(const py::list<py::str>& argv, const Periods& periods = Periods()) {
    std::vector<std::string> args;
    for (const py::str& a : argv) args.push_back(a.utf8());
    const std::optional<nw::Arguments> parsed = nw::parse_arguments(args);
    if (!parsed) {
        py::print(DOC);
        return 2;
    }
    if (parsed->truth) {
        if (!periods.names || !periods.get) throw py::NotImplementedError("build.period has no typed port");
        py::list<py::str> two;
        for (size_t i = 0; i < std::min<size_t>(2, parsed->truth_args.size()); ++i) two.append(cb::compat::to_py(parsed->truth_args[i]));
        auto [n, same, knocked, back] = truth(periods.get(two));
        py::print(py::percent(py::str("%s: %d sources, %d identical as they stand; %d knocked away from the original, %d brought back"),
                              py::join(py::str(" "), two), n, same, knocked, back));
        return 0;
    }
    const py::str given = cb::compat::to_py(parsed->module);
    std::optional<py::str> path = py::os::path::isfile(given) ? std::optional<py::str>(given) : base::corpus::retail(given);
    bool powerpc = given.lower().endswith(py::str(".xex"));
    if (powerpc) {
        path = formats::xex::basefile(given);
    } else if (path && base::corpus::lane(*path) == std::optional<py::str>(py::str("pe"))) {
        powerpc = formats::pe::open_pe(*path).machine == 0x01F2;
    }
    if (!path || base::corpus::lane(*path) != std::optional<py::str>(py::str("pe")))
        throw py::SystemExit(py::percent(py::str("%s: a 32-bit Windows module or a PowerPC image is needed"), given));
    const py::str work = py::os::path::join(py::settings::get_str(py::str("state")), py::str("repair"), py::os::path::basename(*path).lower());
    py::os::makedirs(py::os::path::dirname(work), 0777, true);
    std::optional<py::str> flags;
    if (parsed->flags) flags = cb::compat::to_py(*parsed->flags);
    const auto [n, ident, same, diff] = powerpc ? run_powerpc(*path, flags, work, nullptr, log_print, static_cast<int64_t>(parsed->first))
                                                : run(*path, flags, work, nullptr, log_print, static_cast<int64_t>(parsed->first));
    py::print(py::percent(py::str("%s: %d functions repaired toward the original's bytes: %d identical, %d same size, %d different"),
                          py::os::path::basename(*path), n, ident, same, diff));
    return 0;
}

}  // namespace codebreaker::verify::authored::repair
