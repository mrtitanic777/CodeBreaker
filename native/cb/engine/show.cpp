// cb-unity: alone (the helpers are named for what they show)
// cb/engine/show.cpp -- see show.hpp.
#include "cb/engine/show.hpp"

#include <chrono>
#include <cmath>
#include <ctime>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/sys.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"

namespace cb::engine::show {

namespace {

std::string in_quotes(std::string_view s) {
    const bool single = s.find('\'') == std::string_view::npos || s.find('"') != std::string_view::npos;
    const char quote = single ? '\'' : '"';
    std::string out(1, quote);
    for (const char c : s) {
        if (c == quote || c == '\\') {
            out += '\\';
            out += c;
        } else if (c == '\n') {
            out += "\\n";
        } else if (c == '\r') {
            out += "\\r";
        } else if (c == '\t') {
            out += "\\t";
        } else {
            out += c;
        }
    }
    out += quote;
    return out;
}

std::string number_text(double d) {
    if (std::isnan(d)) return "nan";
    if (std::isinf(d)) return d < 0 ? "-inf" : "inf";
    std::string t = text::format_double(d);
    if (t.find_first_of(".en") == std::string::npos) t += ".0";
    return t;
}

// A scalar as one value of a document reads in a line.
std::string scalar_text(const knowledge::Value& v) {
    switch (v.kind()) {
        case knowledge::Value::Kind::Null: return "None";
        case knowledge::Value::Kind::Bool: return *v.boolean() ? "True" : "False";
        case knowledge::Value::Kind::Int: return std::to_string(*v.integer());
        case knowledge::Value::Kind::Double: return number_text(*v.number());
        case knowledge::Value::Kind::Text: return in_quotes(*v.text());
        default: return {};
    }
}

std::string cut(std::string_view s, std::size_t width) { return std::string(text::utf8_prefix(s, width)); }

std::string first_line(const std::string& s) { return s.substr(0, s.find('\n')); }

// A document's part as one line (what an entry of a list or map shows of itself).
std::string part_text(const knowledge::Value& v, std::size_t width);

std::string text_in_short(const std::string& s, std::size_t width) {
    std::vector<std::string_view> lines = text::lines(s);
    if (lines.empty()) lines.push_back({});
    std::string head;
    for (std::size_t i = 0; i < lines.size() && i < 12; ++i) head += (i ? "\n      " : "      ") + cut(lines[i], width);
    return std::format("{} lines\n{}{}", lines.size(), head, lines.size() > 12 ? "\n      ..." : "");
}

std::string document_short(const knowledge::Value& v, std::size_t width) {
    if (const knowledge::Value::Map* m = v.map()) {
        std::string out = "{";
        std::size_t n = 0;
        for (const auto& [key, value] : *m) {
            if (n++ == 12) break;
            out += (n > 1 ? ", " : "") + key + ": " + part_text(value, 40);
        }
        return out + (m->size() > 12 ? ", ..." : "") + "}";
    }
    if (const knowledge::Value::List* l = v.list()) {
        std::string out = std::format("[{}] ", l->size());
        for (std::size_t i = 0; i < l->size() && i < 6; ++i) out += (i ? ", " : "") + part_text((*l)[i], 40);
        return out + (l->size() > 6 ? ", ..." : "");
    }
    if (const std::string* t = v.text()) return text_in_short(*t, width);
    const std::string t = scalar_text(v);
    return cut(t, width) + (t.size() > width ? "..." : "");
}

std::string part_text(const knowledge::Value& v, std::size_t width) { return first_line(document_short(v, width)); }

// "{'kind': [kept, made], ...}" -- how many facts of each kind the last asking found current and made.
std::string counts_text(const std::vector<Run::Count>& counts) {
    std::string out = "{";
    for (std::size_t i = 0; i < counts.size(); ++i)
        out += std::format("{}'{}': [{}, {}]", i ? ", " : "", counts[i].kind, counts[i].kept, counts[i].made);
    return out + "}";
}

std::string key_text(const std::string& key) { return key.empty() ? "\"\"" : key; }

int list_databases(Engine& engine) {
    std::uint64_t total = 0;
    for (const facts::DatabaseFile& d : engine.databases().databases()) {
        total += d.bytes;
        out("  {:<70} {:>10}  opened {}\n", fs::to_text(d.path.filename()), size_text(d.bytes), when_text(d.opened));
    }
    out("  all {} (cap {}: setting db_max_gb)\n", size_text(total), size_text(engine.databases().config().max_bytes));
    return kExitOk;
}

}  // namespace

std::string size_text(std::uint64_t bytes) {
    if (bytes >= (1u << 20)) return std::format("{:.1f} MB", static_cast<double>(bytes) / 1048576.0);
    return std::format("{:.1f} KB", static_cast<double>(bytes) / 1024.0);
}

std::string when_text(double seconds_since_1970) {
    if (seconds_since_1970 == 0) return "-";
    const std::time_t t = static_cast<std::time_t>(seconds_since_1970);
    std::tm local{};
    local = cb::sys::local_tm(t);
    char buffer[40];
    std::strftime(buffer, sizeof buffer, "%Y-%m-%d %I:%M:%S %p", &local);
    return buffer;
}

std::string short_value(const Value& value, std::size_t width) {
    if (const Bytes* b = std::get_if<Bytes>(&value)) return std::format("{} bytes", b->size());
    if (const std::string* s = std::get_if<std::string>(&value)) return text_in_short(*s, width);
    return document_short(std::get<knowledge::Value>(value), width);
}

cli::Command command(const Environment& environment) {
    cli::Command c;
    c.name = "facts";
    c.summary = "what a binary's database holds, and why";
    c.params = {
        cli::Positional{"binary", "a path or a corpus name (not needed with --databases)", false},
        cli::Positional{"kind", "a kind of fact: each fact of it", false},
        cli::Positional{"key", "a key: one fact, its value and what it was made from", false},
        cli::Option{"--make", "", "make the fact current first (the engine runs)", cli::Arg::Flag},
        cli::Option{"--databases", "", "every database, its size, and the cap", cli::Arg::Flag},
    };
    c.handler = [environment](const cli::Args& a) {
        Engine& engine = environment.engine();
        if (a.flag("databases")) return list_databases(engine);
        const std::optional<std::string> binary = a.value("binary");
        if (!binary) throw UsageError("facts: name a binary (a path or a corpus name), or give --databases");
        const std::optional<std::filesystem::path> path = environment.retail(*binary);
        if (!path) {
            out("no binary '{}'\n", *binary);
            return kExitUsage;
        }
        const std::optional<std::string> kind = a.value("kind"), key = a.value("key");
        const std::shared_ptr<facts::Database> db = engine.open(*path);
        if (a.flag("make") && kind) {
            const auto begun = std::chrono::steady_clock::now();
            engine.get(db, *kind, key.value_or(""), [](std::string_view line) { out("{}\n", line); });
            out("made in {:.1f}s: {}\n", std::chrono::duration<double>(std::chrono::steady_clock::now() - begun).count(),
                counts_text(engine.last_run(*db)));
        }
        out("{}  ({})\n", fs::to_text(*path), fs::to_text(db->path()));
        if (!kind) {
            out("  {:<20} {:>8} {:>10}  {}\n", "kind", "facts", "size", "made by");
            for (const facts::KindSummary& k : db->kinds())
                out("  {:<20} {:>8} {:>10}  {}\n", k.kind, k.count, size_text(k.bytes), k.steps);
            return kExitOk;
        }
        std::optional<std::string> now;
        try {
            now = engine.steps().of(*kind)->version();
        } catch (const UnknownStepError&) {
        }
        if (!key) {
            const std::vector<facts::Row> rows = db->rows(*kind);
            for (const facts::Row& r : rows) {
                const char* state = r.given() ? "given" : (now && r.version == *now ? "version of now" : "OLD VERSION");
                out("  {:<14} {}  {:>10}  {}  {:>6.2f}s  {}\n", key_text(r.key), r.hash.substr(0, 12), size_text(r.size),
                    when_text(r.made), r.took, state);
            }
            out("  {} facts of {}\n", rows.size(), *kind);
            return kExitOk;
        }
        const std::optional<Why> why = engine.why(*db, *kind, *key);
        if (!why) {
            out("  no fact {} {} (--make to make it)\n", *kind, *key);
            return kExitFailed;
        }
        out("  {} {}  hash {}, {}\n", *kind, *key, why->hash, size_text(why->size));
        out("  made by    {}, version {}{}\n", why->step, why->version,
            why->version_now && *why->version_now == why->version
                ? ""
                : std::format(" (the version of now is {}: stale)", why->version_now.value_or("None")));
        out("  made       {}, in {:.2f}s\n", when_text(why->made), why->took);
        out("  from       {} facts\n", why->inputs.size());
        for (std::size_t i = 0; i < why->inputs.size() && i < 40; ++i) {
            const Why::InputNow& in = why->inputs[i];
            out("    {:<20} {:<14} {}  {}\n", in.input.kind, key_text(in.input.key), in.input.hash.substr(0, 12),
                in.same ? "the same now" : "CHANGED");
        }
        if (why->inputs.size() > 40) out("    ... {} more\n", why->inputs.size() - 40);
        const std::optional<facts::Row> row = db->row(*kind, *key);
        out("  value      {}\n", row ? short_value(row->value()) : std::string("None"));
        return kExitOk;
    };
    return c;
}

}  // namespace cb::engine::show
