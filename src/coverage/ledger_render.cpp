#include <algorithm>
#include <format>

#include <nlohmann/json.hpp>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/log.hpp"
#include "core/text.hpp"
#include "coverage/ledger.hpp"

namespace cb::coverage::ledger {

namespace {

std::string render_text(std::int64_t n) { return std::to_string(n); }

// The cells every counted row shows: the three counts, each class, and the share verified.
std::vector<std::string> render_count_cells(const Counts& r) {
    std::vector<std::string> out = {render_text(r.functions), render_text(r.decompiled), render_text(r.compiled)};
    for (verify::Class c : verify::kClassOrder) out.push_back(render_text(r.classes.at(c)));
    out.push_back(percent(r.classes.at(verify::Class::Verified), r.functions));
    return out;
}

std::vector<std::string> render_joined(std::vector<std::string> a, const std::vector<std::string>& b) {
    a.insert(a.end(), b.begin(), b.end());
    return a;
}

std::vector<std::string> render_class_titles() {
    std::vector<std::string> out = {"functions", "decompiled", "compiled"};
    for (verify::Class c : verify::kClassOrder) out.emplace_back(verify::title(c));
    out.emplace_back("verified %");
    return out;
}

// The counts of a row under the names the CSV and JSON give them, in the order the ledger lists them.
std::vector<std::pair<std::string, std::int64_t>> render_named_counts(const Counts& c) {
    std::vector<std::pair<std::string, std::int64_t>> out = {{"functions", c.functions}, {"decompiled", c.decompiled}, {"compiled", c.compiled}};
    for (verify::Class cls : verify::kClassOrder) out.emplace_back(std::string(verify::word(cls)), c.classes.at(cls));
    out.emplace_back("code_bytes", c.code_bytes);
    out.emplace_back("bytes_known", c.bytes_known);
    out.emplace_back("bytes_verified", c.bytes_verified);
    out.emplace_back("bytes_weak", c.bytes_weak);
    return out;
}

std::string render_csv_field(const std::string& s) {
    if (s.find_first_of(",\"\r\n") == std::string::npos) return s;
    return "\"" + text::replace_all(s, "\"", "\"\"") + "\"";
}

Json render_summary_json(const Summary& s) {
    Json j = Json::object();
    j["kind"] = s.kind;
    j["name"] = s.name;
    j["modules"] = s.modules;
    j["in_store"] = s.in_store;
    j["measured"] = s.measured;
    for (const auto& [k, v] : render_named_counts(s)) j[k] = v;
    j["judged"] = s.judged;
    return j;
}

}  // namespace

std::string percent(std::int64_t a, std::int64_t b) {
    if (b == 0) return "     -";
    return std::format("{:5.1f}%", 100.0 * static_cast<double>(a) / static_cast<double>(b));
}

std::vector<std::string> table(const std::vector<std::string>& head, const std::vector<std::vector<std::string>>& rows) {
    std::vector<std::size_t> widths(head.size(), 0);
    for (std::size_t i = 0; i < head.size(); ++i) {
        widths[i] = head[i].size();
        for (const auto& r : rows) widths[i] = std::max(widths[i], r[i].size());
    }
    std::vector<std::string> out;
    const auto line = [&](const std::vector<std::string>& cells) {
        std::string s;
        for (std::size_t i = 0; i < cells.size(); ++i) {
            if (i) s += "  ";
            s += i == 0 ? std::format("{:<{}}", cells[i], widths[i]) : std::format("{:>{}}", cells[i], widths[i]);
        }
        while (!s.empty() && s.back() == ' ') s.pop_back();
        out.push_back(std::move(s));
    };
    line(head);
    for (const auto& r : rows) line(r);
    return out;
}

std::string_view state_of(const Row& r) {
    if (r.measured) return "measured";
    if (r.judged) return "part measured";
    if (r.in_store && r.functions == 0 && r.code_bytes == 0) return "no code";
    return r.in_store ? "not measured now" : "untouched";
}

std::vector<std::string> render(const Report& report, bool every) {
    std::vector<std::string> out;
    out.push_back("Coverage of " + report.corpus);
    std::string shown;
    for (const char* k : {"pe", "ne"}) {
        const auto it = report.profiles.find(k);
        const bool has = it != report.profiles.end() && it->second && !it->second->empty();
        shown += std::string(shown.empty() ? "" : ", ") + std::string(kind_title(k)) + " " + (has ? *it->second : "(none)");
    }
    out.push_back("verifier settings in force: " + shown);
    out.push_back("");
    out.push_back("Functions, by kind of module");
    const std::vector<std::string> names = render_class_titles();
    std::vector<const Summary*> kinds_and_total;
    for (const Summary& s : report.kinds) kinds_and_total.push_back(&s);
    kinds_and_total.push_back(&report.total);
    {
        std::vector<std::vector<std::string>> rows;
        for (const Summary* r : kinds_and_total)
            rows.push_back(render_joined({r->name, render_text(r->modules), render_text(r->in_store), render_text(r->measured)}, render_count_cells(*r)));
        for (std::string& l : table(render_joined({"kind", "modules", "in store", "measured"}, names), rows)) out.push_back(std::move(l));
    }
    out.push_back("");
    out.push_back("Bytes of code, by kind of module");
    {
        std::vector<std::vector<std::string>> rows;
        for (const Summary* r : kinds_and_total)
            rows.push_back({r->name, render_text(r->code_bytes), render_text(r->bytes_known), percent(r->bytes_known, r->code_bytes),
                            render_text(r->bytes_verified), percent(r->bytes_verified, r->code_bytes), render_text(r->bytes_weak),
                            percent(r->bytes_weak, r->code_bytes)});
        for (std::string& l : table({"kind", "code bytes", "in known functions", "", "in VERIFIED", "", "in VERIFIED (weak)", ""}, rows))
            out.push_back(std::move(l));
    }
    std::vector<const Row*> listed;
    for (const Row& r : report.modules)
        if (every || r.in_store) listed.push_back(&r);
    out.push_back("");
    out.push_back(std::string("Modules") + (every ? "" : " the store holds (--all lists the rest)"));
    if (!listed.empty()) {
        std::vector<std::vector<std::string>> rows;
        for (const Row* r : listed) {
            std::vector<std::string> cells = render_joined({r->name, std::string(kind_title(r->kind)), std::string(state_of(*r))}, render_count_cells(*r));
            cells.push_back(render_text(r->code_bytes));
            cells.push_back(percent(r->bytes_known, r->code_bytes));
            cells.push_back(percent(r->bytes_verified, r->code_bytes));
            rows.push_back(std::move(cells));
        }
        for (std::string& l : table(render_joined(render_joined({"module", "kind", "state"}, names), {"code bytes", "known", "verified"}), rows))
            out.push_back(std::move(l));
    } else {
        out.push_back("(none)");
    }
    return out;
}

std::size_t write_csv(const Report& report, const std::filesystem::path& path) {
    std::vector<std::string> head = {"name", "kind", "state", "path", "size"};
    for (const auto& [k, v] : render_named_counts(Counts{})) head.push_back(k);
    head.emplace_back("judged");
    std::string text;
    const auto write_row = [&](const std::vector<std::string>& cells) {
        for (std::size_t i = 0; i < cells.size(); ++i) text += (i ? "," : "") + render_csv_field(cells[i]);
        text += "\r\n";
    };
    write_row(head);
    for (const Row& r : report.modules) {
        std::vector<std::string> cells = {r.name, std::string(kind_title(r.kind)), std::string(state_of(r)), r.path, render_text(r.size)};
        for (const auto& [k, v] : render_named_counts(r)) cells.push_back(render_text(v));
        cells.push_back(render_text(r.judged));
        write_row(cells);
    }
    fs::write_text(path, text);
    return report.modules.size();
}

Json to_json(const Report& report) {
    Json j = Json::object();
    Json profiles = Json::object();
    for (const auto& [kind, digest] : report.profiles) profiles[kind] = digest ? Json(*digest) : Json(nullptr);
    j["profiles"] = std::move(profiles);
    Json kinds = Json::array();
    for (const Summary& s : report.kinds) kinds.push_back(render_summary_json(s));
    j["kinds"] = std::move(kinds);
    j["total"] = render_summary_json(report.total);
    Json modules = Json::array();
    for (const Row& r : report.modules) {
        Json m = Json::object();
        m["name"] = r.name;
        m["path"] = r.path;
        m["kind"] = r.kind;
        m["size"] = r.size;
        m["in_store"] = r.in_store;
        m["measured"] = r.measured;
        if (r.sha) m["sha"] = *r.sha;
        for (const auto& [k, v] : render_named_counts(r)) m[k] = v;
        m["judged"] = r.judged;
        modules.push_back(std::move(m));
    }
    j["modules"] = std::move(modules);
    j["generated"] = report.generated;
    j["corpus"] = report.corpus;
    return j;
}

std::vector<std::string> describe_row(const Row& r, const Report& report, const Ledger& ledger) {
    std::vector<std::string> out;
    out.push_back(std::format("{}  ({}, {})  {}", r.name, kind_title(r.kind), state_of(r), r.path));
    std::vector<std::vector<std::string>> rows;
    const auto add = [&](const std::string& label, std::int64_t n) { rows.push_back({label, render_text(n), percent(n, r.functions)}); };
    add("functions", r.functions);
    add("decompiled", r.decompiled);
    add("compiled", r.compiled);
    for (verify::Class c : verify::kClassOrder) add(std::string(verify::title(c)), r.classes.at(c));
    for (std::string& l : table({"", "functions", "share"}, rows)) out.push_back(std::move(l));
    const auto share = [&](std::int64_t n) { return std::string(text::trim(percent(n, r.code_bytes))); };
    out.push_back(std::format("code bytes {}; in known functions {} ({}); in VERIFIED {} ({}); in VERIFIED (weak) {} ({})", r.code_bytes,
                              r.bytes_known, share(r.bytes_known), r.bytes_verified, share(r.bytes_verified), r.bytes_weak,
                              share(r.bytes_weak)));
    const auto profile = report.profiles.find(r.kind);
    if (r.in_store && r.sha && profile != report.profiles.end() && profile->second && !profile->second->empty()) {
        for (const auto& [cls, found] : ledger.reasons(*r.sha, *profile->second)) {
            out.push_back(std::format("{}, by cause:", verify::title(cls)));
            for (const auto& [why, n] : found) out.push_back(std::format("  {:>5}  {}", n, why));
        }
    }
    return out;
}

cli::Command command(const Services& services) {
    cli::Command c;
    c.name = "coverage";
    c.summary = "how much of the corpus is accounted for, from the function store and the corpus";
    c.params = {
        cli::Option{"--module", "", "one module: its counts, its bytes, why functions fail", cli::Arg::Value, "NAME"},
        cli::Option{"--all", "", "list the modules nothing has been done to as well", cli::Arg::Flag},
        cli::Option{"--csv", "", "one row per corpus module, for a spreadsheet", cli::Arg::Value, "FILE"},
        cli::Option{"--json", "", "the whole ledger as JSON", cli::Arg::Flag},
        cli::Option{"--refresh", "", "read every corpus file's code size again instead of trusting the remembered ones", cli::Arg::Flag},
    };
    c.handler = [services](const cli::Args& a) {
        std::error_code ec;
        std::unique_ptr<engine::FunctionStore> store;
        if (std::filesystem::exists(services.store_index, ec)) {
            store = std::make_unique<engine::FunctionStore>(services.store_index, services.store_folder, /*readonly*/ true);
        } else {
            out("there is no function store yet ({}): nothing has been reconstructed; the corpus is listed alone\n",
                fs::to_text(services.store_index));
        }
        Ledger ledger(services, store.get());
        const std::optional<std::string> module = a.value("module");
        const std::optional<std::string> csv = a.value("csv");
        const Report report = ledger.measure(module, a.flag("refresh"));
        if (module && report.modules.empty()) {
            out("no module '{}' in the corpus or the store\n", *module);
            return 2;
        }
        if (csv) out("wrote {} modules to {}\n", write_csv(report, fs::from_text(*csv)), *csv);
        if (a.flag("json")) {
            out("{}\n", json::dump(to_json(report), json::Style::Pretty));
        } else if (module) {
            for (const Row& r : report.modules)
                for (const std::string& line : describe_row(r, report, ledger)) out("{}\n", line);
        } else if (!csv) {
            for (const std::string& line : render(report, a.flag("all"))) out("{}\n", line);
        }
        return 0;
    };
    return c;
}

}  // namespace cb::coverage::ledger
