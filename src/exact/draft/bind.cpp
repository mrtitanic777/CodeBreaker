#include "exact/draft/bind.hpp"

#include <algorithm>
#include <format>
#include <nlohmann/json.hpp>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/regex.hpp"
#include "core/text.hpp"
#include "exact/aim.hpp"
#include "exact/batch.hpp"
#include "exact/compare.hpp"
#include "exact/draft/candidates.hpp"
#include "exact/draft/early_c_model.hpp"
#include "exact/draft/forms.hpp"
#include "exact/draft/prove.hpp"
#include "exact/draft/shapes.hpp"
#include "exact/library.hpp"
#include "exact/omf_object.hpp"
#include "exact/plugin.hpp"
#include "exact/toolchain.hpp"
#include "formats/omf.hpp"

namespace cb::exact::draft {

namespace {

namespace bindcand = cb::exact::draft::candidates;
namespace bindprove = cb::exact::draft::prove;
namespace bindomf = cb::formats::omf;

// ---- what the toolchain's profile says to the draft stage ----------------------------------------------------------------

struct BindProfile {
    std::optional<forms::FrameRule> frame_order;
    std::vector<forms::TableMacroSpec> table_macros;
    std::vector<forms::DataMacroSpec> data_macros;
    std::vector<forms::LibraryMacroSpec> macros;
    std::vector<std::string> fp_conversions;
    std::map<std::string, std::string> long_helpers;
    std::vector<std::string> register_variables{"si", "di"};
    std::optional<std::string> kit, switches, file_switches, fp_switch, probe_switch;
};

std::optional<std::string> bind_string(const Json& d, const char* key) {
    if (!d.contains(key) || !d.at(key).is_string()) return std::nullopt;
    return d.at(key).get<std::string>();
}

BindProfile bind_read_profile(const toolchain::Locations& locations, std::string_view name) {
    const Json d = plugin::profile_json(locations, name);
    BindProfile p;
    p.kit = bind_string(d, "kit");
    p.switches = bind_string(d, "switches");
    p.file_switches = bind_string(d, "file_switches");
    p.fp_switch = bind_string(d, "fp_switch");
    p.probe_switch = bind_string(d, "probe_switch");
    if (d.contains("frame_order") && d.at("frame_order").is_object()) {
        forms::FrameRule rule;
        if (const Json& mod = d.at("frame_order").value("mod", Json()); mod.is_number_integer()) rule.mod = mod.get<std::uint32_t>();
        p.frame_order = rule;
    }
    if (d.contains("table_macros"))
        for (const Json& m : d.at("table_macros")) p.table_macros.push_back({m.value("header", std::string())});
    if (d.contains("data_macros")) {
        for (const Json& m : d.at("data_macros")) {
            forms::DataMacroSpec spec;
            spec.base = m.value("base", std::string());
            spec.size = m.value("size", 0u);
            if (m.contains("names")) spec.names = m.at("names").get<std::vector<std::string>>();
            if (m.contains("fields"))
                for (auto it = m.at("fields").begin(); it != m.at("fields").end(); ++it)
                    if (const auto at = cb::text::parse_int<std::uint32_t>(it.key(), 10)) spec.fields[*at] = it.value().get<std::string>();
            spec.include = bind_string(m, "include");
            p.data_macros.push_back(std::move(spec));
        }
    }
    if (d.contains("macros")) {
        for (const Json& m : d.at("macros")) {
            forms::LibraryMacroSpec spec;
            spec.call = m.value("call", std::string());
            spec.as = m.value("as", std::string());
            spec.type = m.value("type", std::string("FILE"));
            spec.include = bind_string(m, "include");
            if (m.contains("byte_args")) spec.byte_args = m.at("byte_args").get<std::vector<std::size_t>>();
            p.macros.push_back(std::move(spec));
        }
    }
    if (d.contains("fp_conversions")) p.fp_conversions = d.at("fp_conversions").get<std::vector<std::string>>();
    if (d.contains("long_helpers"))
        for (auto it = d.at("long_helpers").begin(); it != d.at("long_helpers").end(); ++it) p.long_helpers[it.key()] = it.value().get<std::string>();
    if (d.contains("carve") && d.at("carve").contains("register_variables"))
        p.register_variables = d.at("carve").at("register_variables").get<std::vector<std::string>>();
    return p;
}

// ---- the rewrites the beam tries, in its order ---------------------------------------------------------------------------

// Does a line of `source` begin `extern ` and name `name` as a word?
bool bind_declares(std::string_view source, std::string_view name) {
    const auto word = [](char c) { return std::isalnum(static_cast<unsigned char>(c)) != 0 || c == '_'; };
    for (const std::string_view line : cb::text::lines(source)) {
        if (!line.starts_with("extern ")) continue;
        for (std::size_t at = line.find(name); at != std::string_view::npos; at = line.find(name, at + 1))
            if ((at == 0 || !word(line[at - 1])) && (at + name.size() == line.size() || !word(line[at + name.size()]))) return true;
    }
    return false;
}

using BindRewrite = std::function<std::vector<std::string>(const std::string&)>;

std::vector<std::string> bind_some(std::optional<std::string> made) {
    std::vector<std::string> out;
    if (made) out.push_back(std::move(*made));
    return out;
}

// The refiners of the beam: each reading of a statement the model may have misspelled. `facts` is what the function's own
// passes learnt (kept in the candidates' State), `framed` that the profile has a frame order.
std::vector<bindprove::Refiner> bind_refiners(const bindcand::Settings& cs, text::FunctionFacts& facts, bool framed) {
    std::vector<bindprove::Refiner> out;
    const auto one = [&](std::string name, std::function<std::optional<std::string>(const std::string&)> f) {
        out.push_back({std::move(name), [f = std::move(f)](const std::string& s) { return bind_some(f(s)); }});
    };
    const auto many = [&](std::string name, BindRewrite f) { out.push_back({std::move(name), std::move(f)}); };
    const auto changed = [&](std::string name, std::function<std::string(const std::string&)> f) {
        out.push_back({std::move(name), [f = std::move(f)](const std::string& s) { return bind_some(text::changed(f, s)); }});
    };
    const forms::FlatReturns flat = [](std::string_view t) { return shapes::flat_returns(t); };
    using S = const std::string&;

    one("assign_in_args", [](S s) { return forms::assign_in_args(s); });
    one("return_sources", [](S s) { return forms::return_sources(s); });
    one("compound_assigns", [](S s) { return forms::compound_assigns(s); });
    one("chain_assigns", [](S s) { return forms::chain_assigns(s); });
    one("for_loops", [](S s) { return shapes::for_loops(s); });
    one("store_increments", [](S s) { return forms::store_increments(s); });
    one("pre_steps", [](S s) { return forms::pre_steps(s); });
    one("pre_increments", [&](S s) { return forms::pre_increments(s, facts); });
    one("assign_in_tests", [](S s) { return forms::assign_in_tests(s); });
    one("and_returns", [](S s) { return forms::and_returns(s); });
    one("register_temps", [](S s) { return forms::register_temps(s); });
    one("void_variant", [](S s) { return bindcand::void_variant(s); });
    many("pointer_widths", [](S s) { return forms::pointer_widths(s); });
    many("address_variants", [](S s) {   // (the numbers a pointer is given or compared with, as the places in the data they are)
        std::vector<std::string> made;
        for (const forms::AddressVariant& v : forms::address_variants(s, 0x10000, 3)) {
            made.push_back(bind_declares(v.text, v.name) ? v.text : "extern char " + v.name + "[];\n" + v.text);
        }
        return made;
    });
    many("string_variants", [&](S s) { return bindcand::string_variants(cs, s); });
    one("split_ternaries", [](S s) { return forms::split_ternaries(s); });
    one("default_last", [](S s) { return forms::default_last(s); });
    one("push_tails", [](S s) { return forms::push_tails(s); });
    one("unsigned_registers", [](S s) { return forms::unsigned_registers(s); });
    one("unmasked", [](S s) { return forms::unmasked(s); });
    one("no_default", [](S s) { return forms::no_default(s); });
    one("empty_cases_first", [](S s) { return forms::empty_cases_first(s); });
    one("register_slots", [framed](S s) { return bindcand::register_slots(s, framed); });
    one("register_copies", [&](S s) { return forms::register_copies(s, facts); });
    one("lead_filler", [](S s) { return forms::lead_filler(s); });
    one("scaled_places", [&](S s) { return forms::scaled_places(s, facts); });
    one("shared_tails", [](S s) { return forms::shared_tails(s); });
    one("guard_returns", [](S s) { return forms::guard_returns(s); });
    one("default_after", [](S s) { return forms::default_after(s); });
    one("frame_more", [](S s) { return forms::frame_more(s); });
    one("loop_tests", [](S s) { return shapes::loop_tests(s); });
    one("switch_breaks", [](S s) { return forms::switch_breaks(s); });
    one("register_reads", [](S s) { return forms::register_reads(s); });
    one("compare_results", [](S s) { return forms::compare_results(s); });
    one("or_loops", [](S s) { return forms::or_loops(s); });
    one("loop_guards", [](S s) { return shapes::loop_guards(s); });
    one("const_tests", [](S s) { return shapes::const_tests(s); });
    one("flat_returns", [](S s) { return shapes::flat_returns(s); });
    one("return_cases_first", [](S s) { return shapes::return_cases_first(s); });
    one("registers_last", [framed](S s) { return bindcand::registers_last(s, framed); });
    many("unsigned_locals", [](S s) { return bindcand::unsigned_locals(s); });
    one("guard_shared", [flat](S s) { return forms::guard_shared(s, flat); });
    one("guard_flat", [flat](S s) { return forms::guard_flat(s, flat); });
    changed("common_tails", [](S s) { return shapes::common_tails(s); });
    changed("byte_heads", [](S s) { return shapes::byte_heads(s); });
    changed("word_offsets", [](S s) { return shapes::word_offsets(s); });
    one("chain_tails", [](S s) { return shapes::chain_tails(s); });
    one("heads_variant", [](S s) { return shapes::heads_variant(s); });
    many("case_guards", [](S s) { return shapes::case_guards(s); });
    one("shared_exit", [](S s) { return shapes::shared_exit(s); });
    one("label_sinks", [](S s) { return shapes::label_sinks(s); });
    one("call_pairs", [](S s) { return shapes::call_pairs(s); });
    one("assign_loops", [](S s) { return shapes::assign_loops(s); });
    one("char_widens", [](S s) { return shapes::char_widens(s); });
    changed("tail_continues", [](S s) { return shapes::tail_continues(s); });
    many("adjacent_swaps", [](S s) { return shapes::adjacent_swaps(s); });
    many("unsigned_fields", [](S s) { return shapes::unsigned_fields(s); });
    if (cs.frame_order) {
        const forms::FrameRule rule = *cs.frame_order;
        many("frame_names", [rule](S s) {
            std::vector<std::string> made;
            for (const bool registers_last : {false, true}) {
                forms::FrameOptions options;
                options.registers_last = registers_last;
                options.register_local_has_slot = true;
                if (auto v = forms::frame_names(s, rule, options)) made.push_back(std::move(*v));
            }
            return made;
        });
    }
    // (a value and a zero word pushed: it widened; two zero words pushed: a long 0; the same first of more arguments)
    static const Regex widened(R"(\b([A-Za-z_]\w*), 0\))");
    static const Regex long_zero(R"(\b0, 0\))");
    static const Regex long_zero_first(R"((?<=\()0, 0(?=, ))");
    many("other", [](S s) { return std::vector<std::string>{widened.replace_first(s, "(long)$1)")}; });
    many("other", [](S s) { return std::vector<std::string>{long_zero.replace_all(s, "0L)")}; });
    many("other", [](S s) { return std::vector<std::string>{long_zero_first.replace_first(s, "0L")}; });
    return out;
}

// ---- the binding ----------------------------------------------------------------------------------------------------------

class Binding {
public:
    explicit Binding(std::shared_ptr<project::Project> project) : project_(std::move(project)) {
        const project::Project& p = *project_;
        toolchain_ = p.toolchain();
        profile_name_ = p.profile_parts()->name;
        const toolchain::Locations& locations = p.services().toolchains->locations();
        profile_ = bind_read_profile(locations, profile_name_);
        options_ = p.compile_options();
        batch_options_ = p.batch_options();
        program_ = p.program();
        symbols_ = p.symbols();
        decoder_ = p.services().decoder != nullptr ? p.services().decoder : &x86::capstone_decoder();
        priors_ = std::make_unique<bindprove::Priors>(locations.state / "exact" / "priors.json");

        bindcand::Settings& s = candidate_settings_;
        s.kit.project = fs::to_text(p.dir());
        s.kit.toolchain = profile_name_;
        if (profile_.kit) {
            try {
                s.kit.kit = locations.kit(*profile_.kit);
            } catch (const Error&) {   // (no kit set up: no headers to read)
            }
        }
        s.kit.library_data = [this] { return library_data(); };
        s.frame_order = profile_.frame_order;
        s.table_macros = profile_.table_macros;
        s.data_macros = profile_.data_macros;
        s.macros = profile_.macros;
        s.fp_conversions = profile_.fp_conversions;
        s.long_helpers = profile_.long_helpers;
        s.image = p.image();
        s.data_segment = static_cast<std::uint32_t>(std::max<std::int64_t>(p.dgroup(), 0));
        s.data_names = bindcand::data_names_of(symbols_);
        if (const auto given = fs::get_env("CB_BATCH"))   // (candidates compiled in one run: 0 for none)
            if (const auto n = cb::text::parse_int<std::size_t>(*given, 10)) batch_ = *n;
        aim_ = fs::get_env("CB_AIM").value_or("1") != "0";
    }

    draft::ProveOutcome prove(InverseModel& model, const ModelTexts& texts, std::uint32_t a, std::uint32_t ln, const std::map<std::uint32_t, std::string>& names) {
        const ByteView image = project_->image();
        const std::size_t from = std::min<std::size_t>(a, image.size());
        const ByteView code = image.subspan(from, std::min<std::size_t>(ln, image.size() - from));

        // what the code shows of widths (the model's checker mode and the code itself), once for the function
        bindcand::ByteFields widths;
        if (const auto* marking = dynamic_cast<const bindcand::MarkingModel*>(&model))
            if (auto found = bindcand::byte_fields(marking, code)) widths = std::move(*found);
        bindprove::BytePointers byte_pointers;
        for (const text::Site& site : widths.sites)
            if (site.offset == 0) byte_pointers.insert(site.pointer);

        // the passes' own record of the function: the model's longs and 8087 values, the widths
        bindcand::State state;
        state.facts.longs = texts.longs;
        state.fp = texts.fp;
        state.facts.byte_sites = widths.sites;
        state.facts.ranges = widths.ranges;
        state.frame_slots = widths.frame_slots;

        bindprove::Settings settings;
        settings.batch = batch_;
        settings.aim = aim_;
        settings.file_switches = profile_.file_switches;
        settings.switches = project_->switches().value_or(profile_.switches.value_or(""));
        settings.fp_switch = profile_.fp_switch;
        settings.probe_switch = profile_.probe_switch;
        settings.fp = texts.fp.fp;
        settings.probes = bindprove::probes(code);

        const std::vector<bindprove::Refiner> refiners = bind_refiners(candidate_settings_, state.facts, profile_.frame_order.has_value());
        bindprove::Collaborators c;
        c.compile = [this](std::string_view source) { return compile(source); };
        c.judge = [&](ByteView object, std::size_t k) { return judge(object, k, a, ln); };
        c.compile_many = [this](const std::vector<std::string>& sources) { return compile_many(sources); };
        c.diagnose = [&](std::string_view source) { return diagnose(source, code); };
        c.refiners = refiners;
        c.string_variants = [this](std::string_view source) { return bindcand::string_variants(candidate_settings_, source); };
        c.byte_pointers = [&] { return byte_pointers; };
        c.candidates = [&](const std::string& text, const bindcand::RegisterParams& regparam) {
            // (each variant of the text starts from what the function showed: the passes clear what belongs to one text)
            state.facts.longs = texts.longs;
            state.fp = texts.fp;
            state.facts.byte_sites = widths.sites;
            state.facts.ranges = widths.ranges;
            state.frame_slots = widths.frame_slots;
            return bindcand::candidates(candidate_settings_, text, a, names, profile_.register_variables, regparam, widths, texts.frame_size, state, caches_);
        };

        bindprove::Proof proof(std::move(settings), std::move(c), priors_.get(), ln);
        std::vector<std::pair<std::string, bindcand::RegisterParams>> given;
        for (const DraftText& t : texts.texts) given.emplace_back(t.text, t.regparam);
        const bindprove::Outcome outcome = proof.prove_texts(given);
        return {outcome.source, outcome.verdict.text()};
    }

private:
    batch::CompileOutcome compile(std::string_view source) const {
        const toolchain::Compiled made = toolchain_->compile_full(as_bytes(source), options_);
        return {made.object, made.console};
    }

    // The k-th function of the object, held to the program's function at `a`.
    bindprove::Verdict judge(ByteView object, std::size_t k, std::uint32_t a, std::uint32_t ln) const {
        const bindomf::Object parsed = bindomf::read(object);
        const std::vector<std::string> segments = bindomf::code_segments(parsed);
        std::vector<bindomf::FunctionSpan> functions;
        if (!segments.empty()) functions = bindomf::functions(parsed, segments.front());
        std::ranges::stable_sort(functions, {}, &bindomf::FunctionSpan::offset);
        if (functions.size() <= k) return bindprove::Verdict::no_function();
        const bindomf::FunctionSpan& f = functions[k];
        const ObjectFile file = object_of(parsed, object);
        const compare::Subject subject{file, f.name, static_cast<std::uint32_t>(f.offset), static_cast<std::uint32_t>(f.length), a, segments.front()};
        SymbolTable symbols = symbols_;   // (each judgement on its own: what comparing learns is not kept)
        SymbolTable learnt;
        const compare::Report report = compare::function(subject, project_->image(), program_, symbols, learnt, nullptr, *decoder_);
        const auto length = static_cast<std::uint32_t>(f.length);
        if (report.identical() && length == ln) return bindprove::Verdict::same();
        return bindprove::Verdict::differing(static_cast<std::uint32_t>(report.bad.size()),
                                             length != ln ? std::optional<std::pair<std::uint32_t, std::uint32_t>>(std::pair{length, ln}) : std::nullopt);
    }

    // The candidates compiled together: {index in sources: (the object, the function's index in it)}; empty when they cannot be.
    std::map<std::size_t, std::pair<Bytes, std::size_t>> compile_many(const std::vector<std::string>& sources) const {
        struct Compiler final : batch::Toolchain {
            const Binding& binding;
            explicit Compiler(const Binding& b) : binding(b) {}
            batch::CompileOutcome compile(ByteView source, const batch::CompileOptions&) override { return binding.compile(as_text(source)); }
        };
        std::map<std::size_t, std::pair<Bytes, std::size_t>> out;
        try {
            Compiler compiler(*this);
            const batch::SymbolRenamer rename = [](ByteView object, const std::function<std::string(std::string_view)>& fn) { return bindomf::rename_symbols(object, fn); };
            const auto made = batch::compile_many(compiler, batch_options_, rename, sources);
            if (!made) return out;
            for (const batch::Placement& placed : made->placed) out.emplace(placed.source, std::pair{made->object, placed.function});
        } catch (const Error&) {   // (a batch that cannot be made: each candidate is proved alone)
            out.clear();
        }
        return out;
    }

    // What differs between the program's function and a source's compile (one compile more, with the listing).
    std::optional<aim::Diagnosis> diagnose(std::string_view source, ByteView original) const {
        try {
            const toolchain::Compiled made = toolchain_->compile_full(as_bytes(source), options_, true);
            if (!made.object || !made.listing) return std::nullopt;
            const std::vector<aim::ListingEntry> listing = aim::listing_code(*made.listing);
            if (listing.empty()) return std::nullopt;
            return aim::diagnose(original, aim::code_of(listing), listing, *decoder_);
        } catch (const Error&) {
            return std::nullopt;
        }
    }

    // {name: address} of the library's data: what the placed modules' words name, and the modules' own data names.
    std::map<std::string, std::uint32_t> library_data() const {
        std::map<std::string, std::uint32_t> places;
        try {
            const std::shared_ptr<const library::Project> view = project_->as_library();
            const library::Map found = library::locate(*view, *decoder_);
            for (const auto& [name, ref] : found.all_refs)
                if (ref.kind == library::RefKind::Data) places[name] = static_cast<std::uint32_t>(ref.address);
            for (const auto* list : {&found.coded, &found.donly}) {
                for (const library::Placed& placed : *list) {
                    if (!placed.data) continue;
                    for (const auto& [name, place] : placed.module->pubs) {
                        if (!place.segment) continue;
                        const std::string segment = cb::text::upper(*place.segment);
                        if (segment == "_DATA" || segment == "DATA" || segment == "CONST") places.try_emplace(name, static_cast<std::uint32_t>(*placed.data + place.offset));
                    }
                }
            }
        } catch (const Error&) {   // (the library cannot be located: its data is not known)
            places.clear();
        }
        return places;
    }

    std::shared_ptr<project::Project> project_;
    std::shared_ptr<toolchain::Toolchain> toolchain_;
    std::string profile_name_;
    BindProfile profile_;
    modules::CompileOptions options_;
    batch::CompileOptions batch_options_;
    compare::Program program_;
    SymbolTable symbols_;
    const x86::Decoder16* decoder_ = nullptr;
    std::unique_ptr<bindprove::Priors> priors_;
    bindcand::Settings candidate_settings_;
    text::ProjectCaches caches_;
    std::size_t batch_ = 16;
    bool aim_ = true;
};

}  // namespace

project::ProveTexts bound_prover(const std::shared_ptr<project::Project>& project) {
    const auto binding = std::make_shared<Binding>(project);
    return [binding](InverseModel& model, const ModelTexts& texts, std::uint32_t address, std::uint32_t length, const std::map<std::uint32_t, std::string>& names) {
        return binding->prove(model, texts, address, length, names);
    };
}

}  // namespace cb::exact::draft
