// ui/report.cpp -- see report.hpp.
#include "ui/report.hpp"

#include <algorithm>
#include <cstdio>
#include <format>
#include <vector>

#include <nlohmann/json.hpp>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "ui/mcp_server.hpp"

namespace cb::ui::report {

namespace {

namespace sfs = std::filesystem;

std::string rp_escape(std::string_view s) {
    std::string out;
    for (const char c : s) {
        if (c == '&') out += "&amp;";
        else if (c == '<') out += "&lt;";
        else if (c == '>') out += "&gt;";
        else if (c == '"') out += "&quot;";
        else out += c;
    }
    return out;
}

std::string rp_text(const Json& j, const char* key) {
    if (!j.is_object() || !j.contains(key) || j[key].is_null()) return {};
    return j[key].is_string() ? j[key].get<std::string>() : j[key].dump();
}

std::int64_t rp_count(const Json& counts, const char* key) {
    return counts.is_object() && counts.contains(key) && counts[key].is_number_integer() ? counts[key].get<std::int64_t>() : 0;
}

// The verdicts, in the order a reader takes them: what was proven, then what was not.
struct RpClass {
    const char* key;
    const char* words;
    const char* color;
};
constexpr RpClass kRpClasses[] = {
    {"verified", "verified: behaves like the original", "#2e9e4f"}, {"weak", "weak: behaves alike in what was tried", "#8bbf4a"},
    {"identified", "identified: library or tool code", "#7a6fd0"},     {"unverified", "unverified: differs from the original", "#d98a2b"},
    {"not-judged", "not judged yet", "#8a8a8a"},                     {"no-compile", "does not compile yet", "#d04a4a"},
    {"no-source", "no source yet", "#5a5a5a"},
};

}  // namespace

std::string html(const Json& t) {
    const std::string module = rp_text(t, "module");
    const Json is = t.value("is", Json::object());
    std::string out;
    out += "<!doctype html><html lang=\"en\"><head><meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
    out += "<title>" + rp_escape(module) + " - CodeBreaker report</title><style>"
           ":root{--bg:#fbfbfb;--card:#fff;--text:#1b1b1b;--muted:#5d5d5d;--line:#e3e3e3;color-scheme:light dark}"
           "@media (prefers-color-scheme:dark){:root{--bg:#202020;--card:#2b2b2b;--text:#f3f3f3;--muted:#b8b8b8;--line:#3a3a3a}}"
           "body{margin:0;background:var(--bg);color:var(--text);font:15px/1.5 'Segoe UI',system-ui,sans-serif}"
           "main{max-width:880px;margin:0 auto;padding:32px 16px}h1{font-size:26px;margin:0 0 4px}h2{font-size:17px;margin:28px 0 10px}"
           ".muted{color:var(--muted)}.card{background:var(--card);border:1px solid var(--line);border-radius:8px;padding:16px 18px}"
           ".bar{display:flex;height:14px;border-radius:7px;overflow:hidden;margin:10px 0}.bar span{display:block}"
           "table{border-collapse:collapse;width:100%}td,th{text-align:left;padding:6px 8px;border-bottom:1px solid var(--line);vertical-align:top}"
           "th{font-weight:600;color:var(--muted);font-size:13px}code,pre{font-family:Consolas,monospace;font-size:13px}"
           "pre{white-space:pre-wrap;margin:6px 0 0;max-height:220px;overflow:auto}.dot{display:inline-block;width:10px;height:10px;border-radius:5px;margin-right:6px}"
           "</style></head><body><main>";
    out += "<h1>" + rp_escape(module) + "</h1>";
    out += "<div class=\"muted\">" + rp_escape(rp_text(is, "platform")) + (rp_text(is, "lane").empty() ? "" : " &middot; " + rp_escape(rp_text(is, "lane"))) +
           (rp_text(is, "size").empty() ? "" : " &middot; " + rp_escape(rp_text(is, "size")) + " bytes") + "</div>";
    if (const Json summary = t.value("summary", Json()); summary.is_string() && !summary.get<std::string>().empty()) {
        out += "<p>" + rp_escape(summary.get<std::string>()) + "</p>";
    } else if (summary.is_object()) {   // the file's facts (format, machine, linker, when it was linked, ...): the plain ones, as a table
        out += "<div class=\"card\" style=\"margin-top:14px\"><table>";
        std::size_t shown = 0;
        for (const auto& [key, value] : summary.items()) {
            if (shown >= 12 || !(value.is_string() || value.is_number()) || (value.is_string() && value.get<std::string>().empty())) continue;
            std::string label = key;
            for (char& c : label)
                if (c == '_') c = ' ';
            out += "<tr><th>" + rp_escape(label) + "</th><td>" + rp_escape(value.is_string() ? value.get<std::string>() : value.dump()) + "</td></tr>";
            ++shown;
        }
        out += "</table></div>";
    }

    // how much came back, and how faithful
    const Json store = t.value("store", Json());
    const Json counts = store.is_object() ? store.value("counts", Json::object()) : Json::object();
    std::int64_t total = 0;
    for (const RpClass& c : kRpClasses) total += rp_count(counts, c.key);
    out += "<h2>Recovered source</h2><div class=\"card\">";
    if (total == 0) {
        const std::string say = store.is_object() ? rp_text(store, "say") : (store.is_string() ? store.get<std::string>() : std::string());
        out += "<p class=\"muted\">No function of it has been judged yet." + (say.empty() ? std::string() : " " + rp_escape(say) + (say.ends_with(".") ? "" : ".")) + " Convert it with CodeBreaker to rebuild and judge its functions.</p>";
    } else {
        const std::int64_t good = rp_count(counts, "verified") + rp_count(counts, "weak") + rp_count(counts, "identified");
        out += std::format("<div><b>{} of {}</b> functions behave like the original or are known library code ({}%).</div>", good, total, total ? good * 100 / total : 0);
        out += "<div class=\"bar\">";
        for (const RpClass& c : kRpClasses)
            if (const std::int64_t n = rp_count(counts, c.key); n > 0) out += std::format("<span style=\"width:{:.2f}%;background:{}\"></span>", 100.0 * static_cast<double>(n) / static_cast<double>(total), c.color);
        out += "</div><table>";
        for (const RpClass& c : kRpClasses)
            if (const std::int64_t n = rp_count(counts, c.key); n > 0)
                out += std::format("<tr><td><span class=\"dot\" style=\"background:{}\"></span>{}</td><td>{}</td></tr>", c.color, c.words, n);
        out += "</table>";
    }
    out += "</div>";

    // what made it
    const Json made = t.value("made_by", Json());
    if (made.is_object()) {
        out += "<h2>What made it</h2><div class=\"card\">";
        const Json who = made.value("compiler", Json());
        std::string compiler = who.is_string() ? who.get<std::string>() : who.is_object() ? rp_text(who, "compiler") : std::string();
        if (who.is_object() && !rp_text(who, "linker").empty()) compiler += (compiler.empty() ? "" : ", ") + std::string("linker ") + rp_text(who, "linker");
        out += "<div>Compiler: " + (compiler.empty() ? std::string("<span class=\"muted\">not recognized</span>") : rp_escape(compiler)) + "</div>";
        if (const Json libs = made.value("identified", Json::object()); libs.is_object() && !libs.empty()) {
            out += "<div class=\"muted\">Code recognized as libraries or tools: ";
            bool first = true;
            for (const auto& [lib, n] : libs.items()) {
                out += (first ? "" : ", ") + rp_escape(lib.empty() ? std::string("unnamed") : lib) + " (" + n.dump() + ")";
                first = false;
            }
            out += "</div>";
        }
        const Json imports = t.value("imports", Json::object());
        const Json exports = t.value("exports", Json::array());
        if (imports.is_object() && !imports.empty()) {   // the libraries it calls most, then how many more
            std::vector<std::pair<std::size_t, std::string>> uses;
            for (const auto& [dll, names] : imports.items()) uses.emplace_back(names.is_array() ? names.size() : 0, dll);
            std::stable_sort(uses.begin(), uses.end(), [](const auto& a, const auto& b) { return a.first > b.first; });
            out += std::format("<div>Calls {} librar{}: ", uses.size(), uses.size() == 1 ? "y" : "ies");
            for (std::size_t i = 0; i < uses.size() && i < 8; ++i) out += (i ? ", " : "") + rp_escape(uses[i].second) + std::format(" ({})", uses[i].first);
            if (uses.size() > 8) out += std::format(" and {} more", uses.size() - 8);
            out += "</div>";
        }
        if (exports.is_array() && !exports.empty()) out += std::format("<div>Offers {} function{} to other programs.</div>", exports.size(), exports.size() == 1 ? "" : "s");
        out += "</div>";
    }

    // what is left
    const Json look = t.value("to_look_at", Json::array());
    if (look.is_array() && !look.empty()) {
        out += "<h2>Still to look at</h2><div class=\"card\"><table><tr><th>Function</th><th>State</th><th>Why</th></tr>";
        for (const Json& f : look) {
            const std::string name = rp_text(f, "name").empty() ? rp_text(f, "addr") : rp_text(f, "name");
            out += "<tr><td><code>" + rp_escape(name) + "</code><div class=\"muted\"><code>" + rp_escape(rp_text(f, "addr")) + "</code></div></td><td>" +
                   rp_escape(rp_text(f, "class")) + "</td><td>" + rp_escape(rp_text(f, "why")) + "</td></tr>";
        }
        out += "</table>";
        if (const std::string more = rp_text(t, "more_to_look_at"); !more.empty()) out += "<p class=\"muted\">and " + rp_escape(more) + " more.</p>";
        out += "</div>";
    }

    // where its source is
    const Json source = t.value("source", Json());
    if (source.is_object() && !rp_text(source, "folder").empty()) {
        out += "<h2>Its source</h2><div class=\"card\"><div><code>" + rp_escape(rp_text(source, "folder")) + "</code></div>";
        if (const Json files = source.value("source", Json::array()); files.is_array() && !files.empty())
            out += std::format("<div class=\"muted\">{} source file{} there.</div>", files.size(), files.size() == 1 ? "" : "s");
        out += "</div>";
    }
    out += "<p class=\"muted\" style=\"margin-top:28px\">Made by CodeBreaker from its own reading of the program: each function's source is compiled and run "
           "beside the original, and judged by what it does.</p></main></body></html>\n";
    return out;
}

namespace {

int rp_run(const Environment& environment, const cli::Args& args) {
    const config::Config& config = environment.config;
    const std::string module = args.required("module");
    const std::shared_ptr<base::Corpus> corpus = environment.corpus ? environment.corpus : mcp::standard_corpus(config);
    service::Environment service_environment = mcp::standard_environment(config, *corpus);
    service::Service service(std::move(service_environment));
    service::ServiceLink link(service, {.mode = service::mode_of_environment(), .start = true});
    mcp::Server server(service, link, mcp::standard_services(config, *corpus), mcp::Options{.writes = false});
    const Json answer = server.handle(Json{{"jsonrpc", "2.0"}, {"id", 1}, {"method", "tools/call"}, {"params", Json{{"name", "triage"}, {"arguments", Json{{"module", module}}}}}});
    const Json result = answer.value("result", Json::object());
    const Json content = result.value("content", Json::array());
    if (!content.is_array() || content.empty() || !content[0].contains("text")) throw UsageError("could not read " + module);
    const std::string text = content[0]["text"].get<std::string>();
    if (result.value("isError", false)) throw UsageError(text);
    const Json triage = Json::parse(text, nullptr, false);
    if (triage.is_discarded()) throw UsageError(text);
    const sfs::path out = args.value("out") ? fs::from_text(*args.value("out")) : config.converted_root() / (sfs::path(module).filename().string() + "-report.html");
    fs::make_dirs(out.parent_path().empty() ? sfs::path(".") : out.parent_path());
    fs::write_text(out, html(triage));
    std::printf("%s\n", fs::to_text(out).c_str());
    return 0;
}

}  // namespace

cli::Command command(const Environment& environment) {
    cli::Command c;
    c.name = "report";
    c.summary = "one page about a program, to read or share: what it is, how much came back as source, how faithful, what is left";
    c.params = {cli::Positional{.name = "module", .help = "a file, or a module of the program collection"},
                cli::Option{.name = "out", .help = "the HTML file to write (default: <converted>/<module>-report.html)", .arg = cli::Arg::Value, .value_name = "FILE"}};
    c.description = "Writes one self-contained HTML page and prints where. Nothing is sent anywhere.";
    c.handler = [environment](const cli::Args& args) { return rp_run(environment, args); };
    return c;
}

}  // namespace cb::ui::report
