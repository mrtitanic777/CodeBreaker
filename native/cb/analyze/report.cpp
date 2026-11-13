#include "cb/analyze/report.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <format>
#include <optional>

#include <nlohmann/json.hpp>

#include "cb/analyze/call_graph.hpp"
#include "cb/analyze/classes.hpp"
#include "cb/analyze/conventions.hpp"
#include "cb/analyze/dispatch.hpp"
#include "cb/analyze/globals.hpp"
#include "cb/analyze/report_internal.hpp"
#include "cb/analyze/resources.hpp"
#include "cb/analyze/strings.hpp"
#include "cb/analyze/structures.hpp"
#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"

namespace cb::analyze::report {

namespace {

namespace stdfs = std::filesystem;
using Clock = std::chrono::steady_clock;

constexpr std::array<std::string_view, kAnalyses> kNames{"call_graph", "conventions", "globals", "strings", "structures", "classes", "dispatch", "resources", "observe"};

double since(Clock::time_point t) { return std::chrono::duration<double>(Clock::now() - t).count(); }
double tenth_or_hundredth(double x, int places) {
    const double f = std::pow(10.0, places);
    return std::round(x * f) / f;
}

std::string_view kind_label(ImageKind kind) {
    switch (kind) {
    case ImageKind::Pe: return "32-bit Windows module";
    case ImageKind::Ne: return "16-bit Windows module";
    case ImageKind::Le: return "virtual device driver";
    }
    return "";
}
std::string_view kind_word(ImageKind kind) { return kind == ImageKind::Pe ? "pe" : kind == ImageKind::Ne ? "ne" : "le"; }

// What each analysis is given of the ones before it, made when first asked for.
class Pipeline {
public:
    explicit Pipeline(Image& image) : image_(image) {}

    const call_graph::Graph& graph() {
        if (!graph_) graph_ = call_graph::build(image_);
        return *graph_;
    }
    const conventions::Report& conventions() {
        if (!conventions_) conventions_ = conventions::analyse(image_, graph());
        return *conventions_;
    }
    const ConventionFacts& summary() {
        if (!summary_) summary_ = conventions::summary(conventions().facts);
        return *summary_;
    }
    strings::Table& table() {
        if (!table_) table_ = strings::build_table(image_, graph());
        return *table_;
    }
    const std::vector<structures::Structure>& structures() {
        if (!structures_) structures_ = structures::recover(image_, graph(), summary());
        return *structures_;
    }
    Image& image() { return image_; }

private:
    Image& image_;
    std::optional<call_graph::Graph> graph_;
    std::optional<conventions::Report> conventions_;
    std::optional<ConventionFacts> summary_;
    std::optional<strings::Table> table_;
    std::optional<std::vector<structures::Structure>> structures_;
};

Counts counts_of(const OrderedMap<std::string, std::int64_t>& m) {
    Counts out;
    for (const auto& [k, v] : m) out.emplace_back(k, v);
    return out;
}

void run_analysis(Analysis which, Pipeline& p, ImageKind kind, const stdfs::path& path, Section& s) {
    switch (which) {
    case Analysis::CallGraph: {
        const call_graph::Report r = call_graph::analyse(p.image(), p.graph());
        s.counts = detail::call_graph_counts(r.counts);
        s.text = r.text;
        detail::call_graph_rows(s, r, kind);
        s.stores_facts = true;
        break;
    }
    case Analysis::Conventions: {
        const conventions::Report& r = p.conventions();
        s.counts = detail::conventions_counts(r.counts);
        s.text = r.text;
        for (facts::Row& row : conventions::fact_rows(r, kind)) {
            // the address of a row is the function's key; the module's own rows have none
            const knowledge::Value* addr = row.value.get("addr");
            s.row_at.push_back(addr != nullptr && addr->integer() ? std::optional<Va>(*addr->integer()) : std::nullopt);
            s.rows.push_back(std::move(row));
        }
        s.stores_facts = true;
        break;
    }
    case Analysis::Globals: {
        const globals::Report r = globals::analyse(p.image(), p.graph(), p.table());
        s.counts = detail::globals_counts(r.counts);
        s.text = r.text;
        detail::globals_rows(s, r, kind);
        s.stores_facts = true;
        break;
    }
    case Analysis::Strings: {
        const strings::Report r = strings::analyse(p.image(), p.graph());
        s.counts = detail::strings_counts(r.counts);
        s.text = r.text;
        detail::strings_rows(s, r, kind);
        s.stores_facts = true;
        break;
    }
    case Analysis::Structures: {
        const structures::Report r = structures::analyse(p.image(), p.graph(), p.summary());
        s.counts = detail::structures_counts(r.counts);
        s.text = r.text;
        detail::structures_rows(s, r, kind);
        s.stores_facts = true;
        break;
    }
    case Analysis::Classes: {
        const classes::Analysis r = classes::analyse(classes::Inputs{p.image(), p.graph(), p.summary()}, p.structures());
        s.counts = counts_of(r.counts);
        s.text = r.text;
        break;
    }
    case Analysis::Dispatch: {
        const dispatch::Report r = dispatch::analyse(p.image(), p.summary());
        s.counts = detail::dispatch_counts(r.counts);
        s.text = r.text;
        detail::dispatch_rows(s, r, kind);
        s.stores_facts = true;
        break;
    }
    case Analysis::Resources: {
        const resources::Uses uses = resources::find_uses(p.image(), p.graph(), p.table());
        const resources::Report r = resources::analyse(uses, detail::carried_resources(path, kind));
        s.counts = detail::resources_counts(r.counts);
        s.text = r.text;
        detail::resources_rows(s, r, kind);
        s.stores_facts = true;
        break;
    }
    case Analysis::Observe:
        throw UnsupportedError("analyze", 0, "observe runs the module's functions in the emulator: not run natively yet");
    }
}

// The function starts the analyses found beyond the decompiler's description, as lines of that description (decompile/describe).
std::vector<std::string> starts_of(Pipeline& p, ImageKind kind) {
    std::vector<std::string> lines;
    std::vector<Va> found = p.image().discovered();
    std::sort(found.begin(), found.end());
    const conventions::Facts& facts = p.conventions().facts;
    for (const Va va : found) {
        const auto fact = facts.find(va);
        const std::optional<std::int64_t> pop = fact != facts.end() ? fact->second.removes : std::nullopt;
        std::string line = std::format("F\t{:x}\t{}\t{}", va, p.image().function_name(va), pop ? *pop : -1);
        if (kind == ImageKind::Ne) {
            const bool far = fact != facts.end() && fact->second.convention && fact->second.convention->ends_with("far");
            line += std::format("\t__{}16{}", pop && *pop == 0 ? "cdecl" : "stdcall", far ? "far" : "near");
        }
        lines.push_back(std::move(line));
    }
    return lines;
}

// A fact's value as report text: addresses in hex, long lists cut.
std::string value_text(const knowledge::Value& v) {
    using K = knowledge::Value::Kind;
    switch (v.kind()) {
    case K::Null: return "None";
    case K::Bool: return *v.boolean() ? "True" : "False";
    case K::Int: {
        const std::int64_t n = *v.integer();
        return n > 0xFFFF ? std::format("{:#x}", n) : std::to_string(n);
    }
    case K::Double: return std::format("{}", *v.number());
    case K::Text: return *v.text();
    case K::List: {
        std::string out = "[";
        const auto& items = *v.list();
        for (std::size_t i = 0; i < items.size() && i < 40; ++i) out += (i ? ", " : "") + value_text(items[i]);
        return out + (items.size() > 40 ? ", ...]" : "]");
    }
    case K::Map: {
        std::string out = "{";
        std::size_t i = 0;
        for (const auto& [key, item] : *v.map()) {
            if (i++ >= 40) {
                out += ", ...";
                break;
            }
            out += (i > 1 ? ", " : "") + key + ": " + value_text(item);
        }
        return out + "}";
    }
    }
    return "";
}

std::string fact_line(const Section& s, std::size_t i, const Image& image) {
    const facts::Row& row = s.rows[i];
    const std::string where = s.row_at[i] ? image.label(*s.row_at[i]) : "(module)";
    std::string rest;
    if (const knowledge::Value::Map* m = row.value.map()) {
        for (const auto& [k, v] : *m) {
            if (k == "addr" || k == "kind" || k == "name") continue;
            rest += (rest.empty() ? "" : ", ") + k + "=" + value_text(v);
        }
    }
    return std::format("  {:<18} {:<22} {}", row.kind, where, rest);
}

std::string with_spaces(std::string s) {
    std::replace(s.begin(), s.end(), '_', ' ');
    return s;
}

std::int64_t count_of(const Counts& counts, std::string_view name) {
    for (const auto& [k, v] : counts)
        if (k == name) return v;
    return 0;
}

}  // namespace

std::string_view name_of(Analysis analysis) { return kNames[static_cast<std::size_t>(analysis)]; }

std::optional<Analysis> from_name(std::string_view name) {
    for (std::size_t i = 0; i < kNames.size(); ++i)
        if (kNames[i] == name) return static_cast<Analysis>(i);
    return std::nullopt;
}

std::vector<Analysis> defaults() {
    std::vector<Analysis> out;
    for (std::size_t i = 0; i < kAnalyses; ++i)
        if (static_cast<Analysis>(i) != Analysis::Observe) out.push_back(static_cast<Analysis>(i));
    return out;
}

std::string names() {
    std::string out;
    for (const std::string_view n : kNames) out += (out.empty() ? "" : ", ") + std::string(n);
    return out;
}

Result run(std::string_view module, const Options& options, const Environment& environment) {
    std::optional<stdfs::path> path;
    if (stdfs::is_regular_file(fs::from_text(module))) path = fs::from_text(module);
    else if (environment.retail) path = environment.retail(module);
    if (!path) throw UsageError(std::format("no module '{}' (a corpus name or a path)", module));

    const auto started = Clock::now();
    detail::Loaded loaded = detail::load_module(*path, environment);
    Result out;
    out.module = text::lower(fs::to_text(path->filename()));
    out.path = *path;
    out.kind = loaded.kind;
    out.image = std::move(loaded.image);
    out.bits = out.image->bits();
    out.functions = out.image->functions().size();
    out.discovered = out.image->discovered().size();
    out.load_seconds = since(started);

    // the analyses in their own order, whatever order they were asked in
    std::vector<Analysis> what = options.what.empty() ? defaults() : options.what;
    std::vector<Analysis> ordered;
    for (std::size_t i = 0; i < kAnalyses; ++i)
        if (std::find(what.begin(), what.end(), static_cast<Analysis>(i)) != what.end()) ordered.push_back(static_cast<Analysis>(i));

    Pipeline pipeline(*out.image);
    for (const Analysis which : ordered) {
        Section s;
        s.analysis = which;
        const auto t = Clock::now();
        try {
            run_analysis(which, pipeline, out.kind, *path, s);
        } catch (const std::exception& e) {   // one analysis failing on a module is a finding, not the end
            s = Section{};
            s.analysis = which;
            s.failed = e.what();
            s.counts = {};
        }
        s.seconds = since(t);
        out.sections.push_back(std::move(s));
    }
    out.starts = starts_of(pipeline, out.kind);
    out.total_seconds = since(started);

    if (options.store && !environment.facts_file.empty()) {
        std::optional<std::string> sha = environment.sha ? environment.sha(*path) : std::nullopt;
        if (!sha) sha = crypto::to_hex(crypto::sha256(fs::read_file(*path)));
        if (environment.add_module) environment.add_module(*sha, out.module, *path, kind_word(out.kind));
        facts::FactStore store = facts::FactStore::open(environment.facts_file);
        for (const Section& s : out.sections)
            if (s.stores_facts && !s.failed) store.put(*sha, name_of(s.analysis), s.rows);
        out.sha = *sha;
    }
    return out;
}

std::string text(const Result& result, bool all_facts) {
    std::vector<std::string> lines;
    lines.push_back(std::format("{}  ({}, {}-bit)  {}", result.module, kind_label(result.kind), result.bits, fs::to_text(result.path)));
    lines.push_back(std::format("{} functions, {} of them found from calls and held addresses beyond the decompiler's description", result.functions, result.discovered));
    lines.push_back(std::format("analysed in {:.1f} s (loading and decoding {:.1f} s)", result.total_seconds, result.load_seconds));
    lines.emplace_back();
    // what the decompiler's description could be told
    lines.emplace_back("what the decompiler's description could be told:");
    std::vector<std::pair<std::string, std::int64_t>> feedback{{"function starts the description does not have", static_cast<std::int64_t>(result.discovered)}};
    for (const Section& s : result.sections) {
        if (s.analysis != Analysis::Conventions || s.failed || s.counts.empty()) continue;
        feedback.emplace_back("caller-cleans functions whose argument count is now known",
                              count_of(s.counts, "argument_count_from_callers") + count_of(s.counts, "argument_count_from_frame"));
        feedback.emplace_back("functions taking arguments in registers", count_of(s.counts, "register_arguments"));
        feedback.emplace_back("functions returning nothing", count_of(s.counts, "result_none"));
        feedback.emplace_back("functions with a variable argument list", count_of(s.counts, "variadic"));
    }
    for (const auto& [what, n] : feedback) lines.push_back(std::format("  {:<58} {}", what, n));
    lines.emplace_back();
    for (const Section& s : result.sections) {
        lines.push_back(std::format("== {}  ({:.1f} s)", with_spaces(std::string(name_of(s.analysis))), s.seconds));
        if (s.failed) {
            lines.push_back("  failed  " + *s.failed);
        } else {
            std::size_t width = 0;
            for (const auto& [k, v] : s.counts) width = std::max(width, k.size());
            for (const auto& [k, v] : s.counts) lines.push_back(std::format("  {:<{}}  {}", with_spaces(k), width, v));
        }
        if (!s.text.empty()) {
            lines.emplace_back();
            for (const std::string& t : s.text) lines.push_back("  " + t);
        }
        if (all_facts && !s.rows.empty()) {
            lines.emplace_back();
            for (std::size_t i = 0; i < s.rows.size(); ++i) lines.push_back(fact_line(s, i, *result.image));
        }
        lines.emplace_back();
    }
    return text::join(lines, "\n");
}

std::string json(const Result& result) {
    nlohmann::ordered_json doc;
    doc["module"] = result.module;
    doc["kind"] = std::string(kind_word(result.kind));
    doc["functions"] = result.functions;
    nlohmann::ordered_json counts = nlohmann::ordered_json::object();
    nlohmann::ordered_json seconds = nlohmann::ordered_json::object();
    seconds["load"] = tenth_or_hundredth(result.load_seconds, 2);
    for (const Section& s : result.sections) {
        nlohmann::ordered_json c = nlohmann::ordered_json::object();
        if (s.failed) c["failed"] = *s.failed;
        for (const auto& [k, v] : s.counts) c[k] = v;
        counts[std::string(name_of(s.analysis))] = std::move(c);
        seconds[std::string(name_of(s.analysis))] = tenth_or_hundredth(s.seconds, 2);
    }
    seconds["total"] = tenth_or_hundredth(result.total_seconds, 2);
    doc["counts"] = std::move(counts);
    doc["seconds"] = std::move(seconds);
    return doc.dump(1);
}

std::string summary(const Result& result) {
    std::string out;
    for (const Section& s : result.sections) {
        std::string counts;
        std::size_t shown = 0;
        if (s.failed) counts = "failed " + *s.failed;
        for (const auto& [k, v] : s.counts) {
            if (shown++ >= 7) break;
            counts += (counts.empty() ? "" : ", ") + with_spaces(k) + " " + std::to_string(v);
        }
        out += std::format("{:<12} {:>5.1f}s  {}\n", name_of(s.analysis), s.seconds, counts);
    }
    return out;
}

cli::Command command(const Environment& environment) {
    cli::Command c;
    c.name = "analyze";
    c.summary = "run the analyses on a module and write what they found";
    c.area = "Analyze";
    c.description = "The analyses: " + names() + ". The default is all but observe.";
    c.params = {cli::Positional{"module", "a corpus name or a path: a 32-bit module (PE) or a 16-bit module (NE)"},
                cli::Option{"--what", "", "which analyses, comma-separated", cli::Arg::Value, "A,B,C"},
                cli::Option{"--out", "", "where the report goes (default: <state>/analyze/<module>.txt)", cli::Arg::Value, "FILE"},
                cli::Option{"--run", "", "also run N functions in the emulator (observe); \"all\" for every function", cli::Arg::Value, "N|all"},
                cli::Option{"--starts", "", "write the function starts the decompiler's description lacks", cli::Arg::Value, "FILE"},
                cli::Option{"--facts", "", "list every fact in the report", cli::Arg::Flag},
                cli::Option{"--json", "", "print the counts and timings as one JSON object", cli::Arg::Flag},
                cli::Option{"--no-store", "", "do not write the facts to the function store", cli::Arg::Flag},
                cli::Option{"--summary", "", "print one line per analysis instead of the report's path", cli::Arg::Flag}};
    c.handler = [environment](const cli::Args& args) {
        Options options;
        options.store = !args.flag("no-store");
        if (const auto what = args.value("what")) {
            for (const std::string_view piece : text::split(*what, ',')) {
                const std::string name(text::trim(piece));
                if (name.empty()) continue;
                const std::optional<Analysis> a = from_name(name);
                if (!a) {
                    err("analyze: no analysis called {} (there are: {})\n", name, names());
                    return 1;
                }
                options.what.push_back(*a);
            }
        }
        if (options.what.empty()) options.what = defaults();
        if (const auto run = args.value("run")) {
            if (std::find(options.what.begin(), options.what.end(), Analysis::Observe) == options.what.end()) options.what.push_back(Analysis::Observe);
            if (*run != "all") {
                const auto n = text::parse_int<std::size_t>(*run, 10);
                if (!n) throw UsageError("--run expects a number or 'all', got '" + *run + "'");
                options.observed = n;
            }
        }
        const Result result = report::run(args.required("module"), options, environment);
        stdfs::path out = args.value("out") ? fs::from_text(*args.value("out"))
                                            : environment.state / "analyze" / (text::replace_all(result.module, "#", "-") + ".txt");
        out = stdfs::absolute(out);
        fs::make_dirs(out.parent_path());
        fs::write_text(out, text(result, args.flag("facts")));
        if (const auto starts = args.value("starts")) fs::write_text(fs::from_text(*starts), text::join(result.starts, "\n") + "\n");
        if (args.flag("json")) {
            cb::out("{}\n", json(result));
        } else if (args.flag("summary")) {
            cb::out("{}", summary(result));
        } else {
            std::string ran;
            for (const Section& s : result.sections) ran += std::format("{}{} {:.1f}s", ran.empty() ? "" : ", ", name_of(s.analysis), s.seconds);
            cb::out("{}: {} functions, {}; report in {}\n", result.module, result.functions, ran, fs::to_text(out));
        }
        return 0;
    };
    return c;
}

}  // namespace cb::analyze::report
