// cb/store/migrate_state.cpp -- see migrate_state.hpp.
#include "cb/store/migrate_state.hpp"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <format>
#include <optional>
#include <set>
#include <string_view>

#include <nlohmann/json.hpp>

#include "cb/base/sqlite.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"
#include "cb/engine/function_store.hpp"
#include "cb/knowledge/store.hpp"
#include "cb/store/functions.hpp"

namespace cb::store::migrate {

namespace sql = cb::base::sqlite;
namespace stdfs = std::filesystem;

namespace {

// ---- the settings a Python digest stood for ----------------------------------------------------------------------------------------
//
// The store keeps beside each profile digest the text its settings were made into: a JSON object for the 32-bit and driver lanes, the repr
// of a tuple for the others. They are read here once, as data of an older writer, down to what decides whether two profiles are the same
// verifier: the kind, the judge's and the comparer's revision, the inputs a run is given (where the text records them) and the tools (name
// and size; the time of a file means nothing outside the machine that wrote it).

struct PyLiteral {
    enum class Type { None, Bool, Int, Real, Text, List, Dict };
    Type type = Type::None;
    std::int64_t integer = 0;
    std::string text;
    std::vector<PyLiteral> items;                               // a list or a tuple
    std::vector<std::pair<std::string, PyLiteral>> entries;     // a dict (text keys)
};

// A subset of Python's literal syntax: what repr() gives for tuples, lists and dicts of numbers and strings.
class PyLiteralReader {
public:
    explicit PyLiteralReader(std::string_view text) : text_(text) {}

    // The one value the text is, or nothing when it is anything else.
    std::optional<PyLiteral> whole() {
        std::optional<PyLiteral> v = value(0);
        skip_blanks();
        if (!v || pos_ != text_.size()) return std::nullopt;
        return v;
    }

private:
    static constexpr int kMaxDepth = 64;

    void skip_blanks() {
        while (pos_ < text_.size() && (text_[pos_] == ' ' || text_[pos_] == '\n' || text_[pos_] == '\t' || text_[pos_] == '\r')) ++pos_;
    }
    bool take(char c) {
        skip_blanks();
        if (pos_ < text_.size() && text_[pos_] == c) {
            ++pos_;
            return true;
        }
        return false;
    }
    bool word(std::string_view w) {
        if (text_.substr(pos_).starts_with(w)) {
            pos_ += w.size();
            return true;
        }
        return false;
    }

    std::optional<PyLiteral> value(int depth) {
        if (depth > kMaxDepth) return std::nullopt;
        skip_blanks();
        if (pos_ >= text_.size()) return std::nullopt;
        const char c = text_[pos_];
        if (c == '(' || c == '[') return list(depth, c == '(' ? ')' : ']');
        if (c == '{') return dict(depth);
        if (c == '\'' || c == '"') {
            std::optional<std::string> s = quoted();
            if (!s) return std::nullopt;
            PyLiteral v;
            v.type = PyLiteral::Type::Text;
            v.text = std::move(*s);
            return v;
        }
        PyLiteral v;
        if (word("None")) return v;
        const bool yes = word("True");
        if (yes || word("False")) {
            v.type = PyLiteral::Type::Bool;
            v.integer = yes ? 1 : 0;
            return v;
        }
        return number();
    }

    std::optional<PyLiteral> list(int depth, char close) {
        ++pos_;   // the opening bracket
        PyLiteral v;
        v.type = PyLiteral::Type::List;
        while (true) {
            if (take(close)) return v;
            std::optional<PyLiteral> item = value(depth + 1);
            if (!item) return std::nullopt;
            v.items.push_back(std::move(*item));
            if (!take(',') && !(skip_blanks(), pos_ < text_.size() && text_[pos_] == close)) return std::nullopt;
        }
    }

    std::optional<PyLiteral> dict(int depth) {
        ++pos_;
        PyLiteral v;
        v.type = PyLiteral::Type::Dict;
        while (true) {
            if (take('}')) return v;
            skip_blanks();
            if (pos_ >= text_.size() || (text_[pos_] != '\'' && text_[pos_] != '"')) return std::nullopt;   // (keys are text)
            std::optional<std::string> key = quoted();
            if (!key || !take(':')) return std::nullopt;
            std::optional<PyLiteral> item = value(depth + 1);
            if (!item) return std::nullopt;
            v.entries.emplace_back(std::move(*key), std::move(*item));
            if (!take(',') && !(skip_blanks(), pos_ < text_.size() && text_[pos_] == '}')) return std::nullopt;
        }
    }

    std::optional<std::string> quoted() {
        const char quote = text_[pos_++];
        std::string out;
        while (pos_ < text_.size()) {
            const char c = text_[pos_++];
            if (c == quote) return out;
            if (c != '\\') {
                out += c;
                continue;
            }
            if (pos_ >= text_.size()) return std::nullopt;
            const char e = text_[pos_++];
            switch (e) {
            case 'n': out += '\n'; break;
            case 't': out += '\t'; break;
            case 'r': out += '\r'; break;
            case 'x': {
                if (pos_ + 2 > text_.size()) return std::nullopt;
                const std::optional<unsigned> byte = text::parse_int<unsigned>(text_.substr(pos_, 2), 16);
                if (!byte) return std::nullopt;
                out += static_cast<char>(*byte);
                pos_ += 2;
                break;
            }
            default: out += e; break;   // \\ \' \" and the rest as they stand
            }
        }
        return std::nullopt;
    }

    std::optional<PyLiteral> number() {
        const std::size_t start = pos_;
        if (pos_ < text_.size() && text_[pos_] == '-') ++pos_;
        const std::size_t digits = pos_;
        while (pos_ < text_.size() && text_[pos_] >= '0' && text_[pos_] <= '9') ++pos_;
        if (pos_ == digits) return std::nullopt;
        PyLiteral v;
        if (pos_ < text_.size() && (text_[pos_] == '.' || text_[pos_] == 'e' || text_[pos_] == 'E')) {   // a real: only its being a number matters
            ++pos_;
            while (pos_ < text_.size() && (std::isalnum(static_cast<unsigned char>(text_[pos_])) || text_[pos_] == '.' || text_[pos_] == '-' || text_[pos_] == '+')) ++pos_;
            v.type = PyLiteral::Type::Real;
            return v;
        }
        const std::optional<std::int64_t> n = text::parse_int<std::int64_t>(text_.substr(start, pos_ - start), 10);
        if (!n) return std::nullopt;
        v.type = PyLiteral::Type::Int;
        v.integer = *n;
        return v;
    }

    std::string_view text_;
    std::size_t pos_ = 0;
};

// What decides that two profiles are the same verifier.
struct VerifierFacts {
    std::string kind;
    std::optional<std::int64_t> judge, comparer;          // the lane's and the comparer's revision
    std::optional<std::vector<std::int64_t>> inputs;      // the numbers the runs are given, where the settings record them
    std::optional<std::vector<std::string>> tools;        // "name:size" of each tool the settings name
    std::optional<std::vector<std::int64_t>> paths;       // whether the branch-guided exploration is on, and its limits (true as 1)
};

// "wcc386.exe:174080:1789307814" -> "wcc386.exe:174080"
std::optional<std::string> tool_name_and_size(std::string_view stamp) {
    const std::vector<std::string_view> parts = text::split(stamp, ':');
    if (parts.size() < 2 || parts[0].empty() || parts[1].empty()) return std::nullopt;
    return text::lower(std::format("{}:{}", parts[0], parts[1]));
}

void collect_numbers(const PyLiteral& v, std::vector<std::int64_t>& out) {
    switch (v.type) {
    case PyLiteral::Type::Int:
    case PyLiteral::Type::Bool: out.push_back(v.integer); break;
    case PyLiteral::Type::List:
        for (const PyLiteral& item : v.items) collect_numbers(item, out);
        break;
    case PyLiteral::Type::Dict:
        for (const auto& entry : v.entries) collect_numbers(entry.second, out);
        break;
    default: break;
    }
}

// The numbers (a bool as 0 or 1) a list begins with after `skip` items, up to the first that is not one.
std::vector<std::int64_t> leading_numbers(const PyLiteral& list, std::size_t skip) {
    std::vector<std::int64_t> out;
    for (std::size_t i = skip; i < list.items.size(); ++i) {
        const PyLiteral& item = list.items[i];
        if (item.type != PyLiteral::Type::Int && item.type != PyLiteral::Type::Bool) break;
        out.push_back(item.integer);
    }
    return out;
}

std::optional<std::int64_t> integer_of(const PyLiteral* v) {
    if (v == nullptr || v->type != PyLiteral::Type::Int) return std::nullopt;
    return v->integer;
}

const PyLiteral* entry_of(const PyLiteral& dict, std::string_view key) {
    for (const auto& [name, value] : dict.entries)
        if (name == key) return &value;
    return nullptr;
}

std::optional<std::int64_t> json_integer(const Json& object, std::string_view key) {
    const auto it = object.find(std::string(key));
    if (it == object.end() || !it->is_number_integer()) return std::nullopt;
    return it->get<std::int64_t>();
}

// The settings of a JSON-made profile (the 32-bit lane: {"lane", "pipeline", "oracle", "compiler", ...}; the driver lane: {"kind", "judge",
// "comparer", "tools", ...}).
std::optional<VerifierFacts> facts_of_json(const Json& settings) {
    if (!settings.is_object()) return std::nullopt;
    VerifierFacts f;
    for (const char* key : {"lane", "kind"}) {
        const auto it = settings.find(key);
        if (it != settings.end() && it->is_string()) {
            f.kind = it->get<std::string>();
            break;
        }
    }
    if (f.kind.empty()) return std::nullopt;
    f.judge = json_integer(settings, "pipeline");
    if (!f.judge) f.judge = json_integer(settings, "judge");
    f.comparer = json_integer(settings, "oracle");
    if (!f.comparer) f.comparer = json_integer(settings, "comparer");
    if (const auto it = settings.find("paths"); it != settings.end() && it->is_array() && !it->empty()) {
        std::vector<std::int64_t> paths;
        for (const auto& item : *it) {
            if (item.is_boolean()) paths.push_back(item.get<bool>() ? 1 : 0);
            else if (item.is_number_integer()) paths.push_back(item.get<std::int64_t>());
            else break;
        }
        f.paths = std::move(paths);
    }
    if (const auto it = settings.find("compiler"); it != settings.end() && it->is_string()) {
        if (const auto tool = tool_name_and_size(it->get<std::string>())) f.tools = std::vector<std::string>{*tool};
    }
    if (const auto it = settings.find("tools"); it != settings.end() && it->is_array()) {
        std::vector<std::string> tools;
        for (const auto& stamp : *it)
            if (stamp.is_string())
                if (const auto tool = tool_name_and_size(stamp.get<std::string>())) tools.push_back(*tool);
        if (!tools.empty()) f.tools = std::move(tools);
    }
    return f;
}

// The settings of a repr-made profile: (kind, judge revision, ...). `ne` and `powerpc` have the comparer's revision second; `dos` has its
// tools; an image lane (`nes`, `gb`, ...) the states each function is run from; `java` only its revision matters here.
std::optional<VerifierFacts> facts_of_tuple(const PyLiteral& tuple) {
    if (tuple.type != PyLiteral::Type::List || tuple.items.size() < 2 || tuple.items[0].type != PyLiteral::Type::Text) return std::nullopt;
    VerifierFacts f;
    f.kind = tuple.items[0].text;
    f.judge = integer_of(&tuple.items[1]);
    if (!f.judge) return std::nullopt;
    if (tuple.items.size() > 2) {
        const PyLiteral& third = tuple.items[2];
        if (f.kind == "ne" || f.kind == "powerpc") {
            f.comparer = integer_of(&third);
        } else if (f.kind == "dos" && third.type == PyLiteral::Type::List) {
            std::vector<std::string> tools;
            for (const PyLiteral& stamp : third.items)
                if (stamp.type == PyLiteral::Type::Text)
                    if (const auto tool = tool_name_and_size(stamp.text)) tools.push_back(*tool);
            if (!tools.empty()) f.tools = std::move(tools);
        } else if (f.kind != "java" && third.type == PyLiteral::Type::List) {
            std::vector<std::int64_t> numbers;
            collect_numbers(third, numbers);
            f.inputs = std::move(numbers);
        }
    }
    for (std::size_t i = 3; i < tuple.items.size(); ++i) {   // ("paths", ready, ...) among the rest
        const PyLiteral& item = tuple.items[i];
        if (item.type == PyLiteral::Type::List && !item.items.empty() && item.items[0].type == PyLiteral::Type::Text && item.items[0].text == "paths")
            f.paths = leading_numbers(item, 1);
    }
    return f;
}

// What an older writer's settings text stood for; nothing when it is not one of the two forms (this program's own are neither).
std::optional<VerifierFacts> facts_of_older_settings(std::string_view settings) {
    const std::string_view trimmed = text::trim(settings);
    if (trimmed.starts_with("{")) {
        Json parsed;
        if (!json::try_parse(trimmed, parsed)) return std::nullopt;
        return facts_of_json(parsed);
    }
    if (trimmed.starts_with("(")) {
        const std::optional<PyLiteral> tuple = PyLiteralReader(trimmed).whole();
        return tuple ? facts_of_tuple(*tuple) : std::nullopt;
    }
    return std::nullopt;
}

// The numbers in a text, not those that are part of a name ("r0").
std::vector<std::int64_t> numbers_in(std::string_view text) {
    std::vector<std::int64_t> out;
    for (std::size_t i = 0; i < text.size();) {
        const bool digit = text[i] >= '0' && text[i] <= '9';
        const bool part_of_name = i > 0 && (std::isalpha(static_cast<unsigned char>(text[i - 1])) || text[i - 1] == '_');
        if (!digit || part_of_name) {
            ++i;
            continue;
        }
        const bool negative = i > 0 && text[i - 1] == '-';
        std::size_t end = i;
        while (end < text.size() && text[end] >= '0' && text[end] <= '9') ++end;
        if (const auto n = text::parse_int<std::int64_t>(text.substr(i, end - i), 10)) out.push_back(negative ? -*n : *n);
        i = end;
    }
    return out;
}

// What this program's settings text says of the same things ("win32 rev 48 compare 25 ... compiler wcc386.exe:174080:3", "kind=ne judge=24
// comparer=25 ...", "dos rev 93 tools wasm:269824:1 wlink:119808:2", "nes rev 2 states [{a=0, x=0}]").
VerifierFacts facts_of_current_settings(std::string_view kind, std::string_view settings) {
    VerifierFacts f;
    f.kind = std::string(kind);
    const std::vector<std::string_view> tokens = text::split_ws(settings);
    const auto number_after = [&](std::size_t i) -> std::optional<std::int64_t> {
        return i + 1 < tokens.size() ? text::parse_int<std::int64_t>(tokens[i + 1], 10) : std::nullopt;
    };
    std::vector<std::string> tools;
    for (std::size_t i = 0; i < tokens.size(); ++i) {
        const std::string_view t = tokens[i];
        if (t == "rev" && !f.judge) f.judge = number_after(i);
        else if (t == "compare" && !f.comparer) f.comparer = number_after(i);
        else if (t.starts_with("judge=") && !f.judge) f.judge = text::parse_int<std::int64_t>(t.substr(6), 10);
        else if (t.starts_with("comparer=") && !f.comparer) f.comparer = text::parse_int<std::int64_t>(t.substr(9), 10);
        else if (t == "compiler" && i + 1 < tokens.size()) {
            if (const auto tool = tool_name_and_size(tokens[i + 1])) tools.push_back(*tool);
        } else if (t == "tools") {
            for (std::size_t k = i + 1; k < tokens.size() && tokens[k].find(':') != std::string_view::npos; ++k)
                if (const auto tool = tool_name_and_size(tokens[k])) tools.push_back(*tool);
        } else if ((t == "paths" || t.starts_with("paths=")) && !f.paths) {
            // "paths true 3 4 5 <stamp>" (a token each) or "paths=true,3,4,<stamp>"
            std::vector<std::string_view> words;
            if (t == "paths") words.assign(tokens.begin() + static_cast<std::ptrdiff_t>(i) + 1, tokens.end());
            else words = text::split(t.substr(6), ',');
            std::vector<std::int64_t> paths;
            for (const std::string_view w : words) {
                if (w == "true" || w == "false") paths.push_back(w == "true" ? 1 : 0);
                else if (const auto n = text::parse_int<std::int64_t>(w, 10)) paths.push_back(*n);
                else break;
            }
            f.paths = std::move(paths);
        } else if (t == "states" && !f.inputs) {
            const std::size_t at = settings.find("states");   // (the first: the settings say it once)
            f.inputs = numbers_in(settings.substr(at + 6));
        }
    }
    if (!tools.empty()) f.tools = std::move(tools);
    return f;
}

// "": the same verifier; else how the older settings differ from the ones in force.
std::string how_they_differ(const VerifierFacts& older, const VerifierFacts& now) {
    if (!older.judge || !now.judge) return "the judge's revision is not in the settings";
    if (*older.judge != *now.judge) return std::format("judge revision {}, now {}", *older.judge, *now.judge);
    if (older.comparer != now.comparer) {
        const auto say = [](const std::optional<std::int64_t>& r) { return r ? std::to_string(*r) : std::string("none"); };
        return std::format("comparer revision {}, now {}", say(older.comparer), say(now.comparer));
    }
    if (older.inputs && now.inputs && *older.inputs != *now.inputs) return "the inputs a function is run from differ";
    if (older.paths && now.paths && !older.paths->empty() && !now.paths->empty()) {
        // (the settings' own lists: the shorter is a prefix of the longer where one lane keeps more of its limits)
        const std::size_t n = std::min(older.paths->size(), now.paths->size());
        if (!std::equal(older.paths->begin(), older.paths->begin() + static_cast<std::ptrdiff_t>(n), now.paths->begin()))
            return (*older.paths)[0] != (*now.paths)[0] ? std::string(now.paths->front() != 0 ? "the older judge did not explore paths, this one does"
                                                                                           : "the older judge explored paths, this one does not")
                                                         : std::string("the path exploration's limits differ");
    }
    if (older.tools && now.tools && *older.tools != *now.tools) return "a tool differs (name or size)";
    return "";
}

// ---- (a) judged results --------------------------------------------------------------------------------------------------------

double seconds_now() { return std::chrono::duration<double>(std::chrono::system_clock::now().time_since_epoch()).count(); }

std::string text_of(const sql::Value& v) {
    if (const auto* s = std::get_if<std::string>(&v)) return *s;
    if (const auto* i = std::get_if<std::int64_t>(&v)) return std::to_string(*i);
    return {};
}

std::int64_t count_of(const sql::Value& v) {
    if (const auto* i = std::get_if<std::int64_t>(&v)) return *i;
    return 0;
}

struct OlderProfile {
    std::string digest, settings;
    double made = 0;
};

// The profiles of the index in the order they were made, newest first.
std::vector<OlderProfile> index_profiles(const stdfs::path& index_file) {
    std::vector<OlderProfile> out;
    sql::Database db = sql::Database::open_readonly(index_file);
    for (const sql::Row& r : db.query("SELECT digest, settings, ts FROM profile ORDER BY ts DESC, digest")) {
        OlderProfile p{text_of(r[0]), text_of(r[1]), 0};
        if (const auto* d = std::get_if<double>(&r[2])) p.made = *d;
        out.push_back(std::move(p));
    }
    return out;
}

// The summary of a module in the index lists the profiles it holds results under, and the one it was last judged under.
void add_to_summary(sql::Database& index, const std::string& module, const std::set<std::string>& moved_from, const std::set<std::string>& moved_to) {
    const std::vector<sql::Row> rows = index.query("SELECT profiles, last_profile FROM summary WHERE module=?", {module});
    if (rows.empty()) return;   // (the summary of a module is made when the engine next writes to it)
    std::set<std::string> profiles;
    Json listed;
    if (json::try_parse(text_of(rows[0][0]), listed) && listed.is_array())
        for (const auto& p : listed)
            if (p.is_string()) profiles.insert(p.get<std::string>());
    profiles.insert(moved_to.begin(), moved_to.end());
    std::string last = text_of(rows[0][1]);
    if (moved_from.contains(last) && !moved_to.empty()) last = *moved_to.begin();
    Json out = Json::array();
    for (const std::string& p : profiles) out.push_back(p);
    index.exec("UPDATE summary SET profiles=?, last_profile=? WHERE module=?",
               {out.dump(), last.empty() ? sql::Value() : sql::Value(last), module});
}

VerdictReport migrate_verdicts(const Services& services, bool dry) {
    VerdictReport report;
    const std::unique_ptr<engine::FunctionStore> store = functions::open(services.state_dir, std::nullopt, /*readonly=*/true);
    if (!store) return report;
    report.store_found = true;
    const std::map<std::string, Verifier> now = services.verifier_profiles ? services.verifier_profiles() : std::map<std::string, Verifier>();
    std::set<std::string> current;
    for (const auto& [kind, verifier] : now) current.insert(verifier.digest);

    // which profile each older digest stands for
    struct Plan {
        std::size_t move;
        const Verifier* to;
    };
    std::map<std::string, Plan> plans;   // by the older digest
    const std::vector<OlderProfile> profiles = index_profiles(store->path());
    const std::map<std::string, engine::Summary> summaries = store->summaries();
    std::map<std::string, std::vector<std::string>> modules_of;   // older digest -> modules that hold results under it
    for (const auto& [module, summary] : summaries)
        for (const std::string& digest : summary.profiles) modules_of[digest].push_back(module);

    for (const OlderProfile& p : profiles) {
        if (current.contains(p.digest)) continue;
        const std::optional<VerifierFacts> older = facts_of_older_settings(p.settings);
        if (!older) continue;   // not an older writer's settings (this program's own, in a form that is not in force now)
        const auto held = modules_of.find(p.digest);
        if (held == modules_of.end()) continue;   // nothing is filed under it
        ProfileMove move;
        move.kind = older->kind;
        move.from = p.digest;
        move.modules = static_cast<std::int64_t>(held->second.size());
        const auto verifier = now.find(older->kind);
        if (verifier == now.end()) {
            move.why_not = std::format("no verifier for {} files is set up here", older->kind);
        } else if (const std::string why = how_they_differ(*older, facts_of_current_settings(older->kind, verifier->second.settings)); !why.empty()) {
            move.why_not = why;
        } else {
            move.to = verifier->second.digest;
        }
        report.profiles.push_back(std::move(move));
        if (!report.profiles.back().to.empty()) plans[p.digest] = {report.profiles.size() - 1, &now.at(older->kind)};
    }
    if (plans.empty()) return report;

    // carry, module by module
    std::optional<sql::Database> index;
    if (!dry) index = sql::Database::open(store->path(), std::chrono::seconds(60));
    std::set<std::string> registered;   // the digests whose settings the index has been told
    std::set<std::string> modules;
    for (const auto& [digest, plan] : plans)
        for (const std::string& module : modules_of.at(digest)) modules.insert(module);
    for (const std::string& module : modules) {
        const stdfs::path file = store->database_path(module);
        std::error_code ec;
        if (!stdfs::is_regular_file(file, ec)) continue;
        sql::Database db = dry ? sql::Database::open_readonly(file, std::chrono::seconds(60)) : sql::Database::open(file, std::chrono::seconds(60));
        std::set<std::string> moved_from, moved_to;
        // (the newest older profile first: a function judged under two that stand for the same settings keeps the newer verdict)
        for (const OlderProfile& p : profiles) {
            const auto plan = plans.find(p.digest);
            if (plan == plans.end()) continue;
            const std::string& to = plan->second.to->digest;
            ProfileMove& move = report.profiles[plan->second.move];
            const std::int64_t found = count_of(db.query("SELECT count(*) FROM verdict WHERE profile=?", {p.digest})[0][0]);
            if (found == 0 && count_of(db.query("SELECT count(*) FROM built WHERE profile=?", {p.digest})[0][0]) == 0) continue;
            const std::int64_t to_file = count_of(db.query("SELECT count(*) FROM verdict v WHERE v.profile=? AND NOT EXISTS "
                                                           "(SELECT 1 FROM verdict n WHERE n.candidate=v.candidate AND n.profile=?)",
                                                           {p.digest, to})[0][0]);
            const std::int64_t built_to_file = count_of(db.query("SELECT count(*) FROM built b WHERE b.profile=? AND NOT EXISTS "
                                                                 "(SELECT 1 FROM built n WHERE n.module=b.module AND n.addr=b.addr AND n.profile=?)",
                                                                 {p.digest, to})[0][0]);
            move.found += found;
            move.carried += to_file;
            move.present += found - to_file;
            move.built += built_to_file;
            if (to_file == 0 && built_to_file == 0) continue;
            if (!dry) {
                sql::Database::Transaction tx(db);
                db.exec("INSERT OR IGNORE INTO verdict(candidate, profile, sig, verdict, reason, limits, detail, ts, carried) "
                        "SELECT candidate, ?, sig, verdict, reason, limits, detail, ts, carried FROM verdict WHERE profile=?",
                        {to, p.digest});
                db.exec("INSERT OR IGNORE INTO built(module, addr, profile, sig) SELECT module, addr, ?, sig FROM built WHERE profile=?",
                        {to, p.digest});
                tx.commit();
            }
            moved_from.insert(p.digest);
            moved_to.insert(to);
        }
        if (moved_to.empty()) continue;
        ++report.modules_changed;
        if (dry) continue;
        for (const std::string& to : moved_to) {
            if (!registered.insert(to).second) continue;
            const auto verifier = std::find_if(now.begin(), now.end(), [&](const auto& kv) { return kv.second.digest == to; });
            index->exec("INSERT OR IGNORE INTO profile(digest, settings, ts) VALUES(?,?,?)", {to, verifier->second.settings, seconds_now()});
        }
        add_to_summary(*index, module, moved_from, moved_to);
    }
    return report;
}

// ---- (b) the knowledge base ----------------------------------------------------------------------------------------------------

// The learned kinds whose keys mean the same to both sides: the argument bytes an export of a 32-bit DLL removes, found from its code, keyed
// "<DLL>.<name>". (The 16-bit table is keyed by a digest of the Python's source that reads the exports: another table.)
bool learned_kind_carries(std::string_view platform, std::string_view kind) { return platform == "win32" && kind == "argument_bytes.code"; }

// A value the Python kept as JSON (tag J) or text (S), as this program's; nothing for bytes or a pickle.
std::optional<knowledge::Value> value_of_older_row(std::string_view tag, const sql::Value& stored) {
    std::string body;
    if (const auto* bytes = std::get_if<Bytes>(&stored)) body.assign(bytes->begin(), bytes->end());
    else if (const auto* s = std::get_if<std::string>(&stored)) body = *s;
    else if (const auto* i = std::get_if<std::int64_t>(&stored)) body = std::to_string(*i);
    else return std::nullopt;
    if (tag == "S") return knowledge::Value(std::move(body));
    if (tag != "J") return std::nullopt;
    try {
        return knowledge::Value::from_json_text(body, "knowledge value");
    } catch (const Error&) {   // a number past 64 bits, or text that is not JSON: not carried
        return std::nullopt;
    }
}

void note_left(KnowledgeReport& report, std::string what, std::int64_t rows, std::string why) {
    if (rows > 0) report.left.push_back({std::move(what), rows, std::move(why)});
}

KnowledgeReport migrate_knowledge(const Services& services, bool dry) {
    KnowledgeReport report;
    const stdfs::path older_file = services.state_dir / "db" / "knowledge.sqlite";
    const stdfs::path file = knowledge::Store::default_path(services.state_dir);
    std::error_code ec;
    if (!stdfs::is_regular_file(older_file, ec)) return report;
    report.found = true;
    if (fs::same_path(older_file, file)) {
        report.same_file = true;
        return report;
    }
    sql::Database older = sql::Database::open_readonly(older_file, std::chrono::seconds(60));

    // what this program already holds (never replaced), read before anything is written
    const bool have_target = stdfs::is_regular_file(file, ec);
    std::set<std::string> given_names;
    if (have_target) {
        sql::Database target = sql::Database::open_readonly(file, std::chrono::seconds(60));
        for (const sql::Row& r : target.query("SELECT hash FROM body_name")) given_names.insert(text_of(r[0]));
    }
    std::optional<knowledge::Store> kb;
    if (have_target || !dry) kb.emplace(file, knowledge::DataFolders::from_environment());

    // the bodies of each module, and what they are named
    for (const sql::Row& m : older.query("SELECT sha, name, platform FROM module ORDER BY sha")) {
        const std::string sha = text_of(m[0]);
        std::vector<knowledge::BodyRow> bodies;
        for (const sql::Row& b : older.query("SELECT addr, hash, name, source FROM body WHERE module=? ORDER BY addr", {sha})) {
            knowledge::BodyRow row{text_of(b[0]), text_of(b[1]), std::nullopt, std::nullopt};
            if (!std::holds_alternative<std::monostate>(b[2])) row.name = text_of(b[2]);
            if (!std::holds_alternative<std::monostate>(b[3])) row.source = text_of(b[3]);
            bodies.push_back(std::move(row));
        }
        if (bodies.empty()) continue;
        if (kb && !kb->bodies(sha).empty()) {
            ++report.modules_present;   // (indexed by this program itself: what it found stands)
            continue;
        }
        ++report.modules;
        report.bodies += static_cast<std::int64_t>(bodies.size());
        if (dry) continue;
        kb->add_module(sha, text_of(m[1]), text_of(m[2]));
        kb->set_bodies(sha, bodies);
    }
    for (const sql::Row& n : older.query("SELECT hash, name, source FROM body_name ORDER BY hash")) {
        const std::string digest = text_of(n[0]);
        if (given_names.contains(digest)) {
            ++report.names_present;
            continue;
        }
        ++report.names;
        if (!dry) kb->name_body(digest, text_of(n[1]), std::holds_alternative<std::monostate>(n[2]) ? std::string("named by hand") : text_of(n[2]));
    }

    // learned rows
    std::map<std::pair<std::string, std::string>, knowledge::Rows> to_put;   // by (platform, kind)
    std::map<std::string, std::int64_t> not_carried;                           // "platform kind" -> rows
    std::int64_t undecodable = 0;
    for (const sql::Row& r : older.query("SELECT platform, kind, key, tag, value FROM knowledge WHERE origin='learned' ORDER BY platform, kind, rowid")) {
        const std::string platform = text_of(r[0]), kind = text_of(r[1]), key = text_of(r[2]);
        if (!learned_kind_carries(platform, kind)) {
            ++not_carried[platform + " " + kind];
            continue;
        }
        if (kb && kb->has(platform, kind, key)) {
            ++report.learned_present;
            continue;
        }
        const std::optional<knowledge::Value> value = value_of_older_row(text_of(r[3]), r[4]);
        if (!value) {
            ++undecodable;
            continue;
        }
        ++report.learned;
        if (!dry) to_put[{platform, kind}].insert_or_assign(key, *value);
    }
    for (auto& [kind, rows] : to_put) kb->put_many(kind.first, kind.second, rows, "carried from the Python's knowledge base", knowledge::Origin::Learned);
    for (const auto& [what, rows] : not_carried) {
        note_left(report, "learned " + what, rows,
                  what == "win16 argument_bytes" ? "keyed by a digest of the Python's source that reads the exports: this program's reading is another table, made again"
                                                  : "not known to mean the same on both sides");
    }
    note_left(report, "learned rows that are not JSON or text", undecodable, "a pickle or bytes: only the Python can read them");

    // what is made again (counted for the report)
    for (const sql::Row& r : older.query("SELECT origin, count(*) FROM knowledge WHERE origin IN ('authored', 'derived') GROUP BY origin")) {
        const std::string origin = text_of(r[0]);
        note_left(report, origin + " rows", count_of(r[1]),
                  origin == "authored" ? "made again from their files when they change" : "made again from the headers when their stamp changes");
    }
    return report;
}

}  // namespace

Report migrate(const Services& services, const Options& options) {
    Report report;
    report.dry = options.dry;
    report.verdicts = migrate_verdicts(services, options.dry);
    report.knowledge = migrate_knowledge(services, options.dry);
    return report;
}

std::vector<std::string> render(const Report& report) {
    std::vector<std::string> out;
    out.push_back(report.dry ? "migrate-state (dry: nothing is written)" : "migrate-state");

    out.push_back("");
    out.push_back("judged results (function store profiles)");
    const VerdictReport& v = report.verdicts;
    if (!v.store_found) {
        out.push_back("  no function store in this state folder: nothing to carry");
    } else if (v.profiles.empty()) {
        out.push_back("  no older profile holds results (or no verifier settings are known here): nothing to carry");
    }
    std::int64_t carried = 0, present = 0;
    for (const ProfileMove& p : v.profiles) {
        if (p.to.empty()) {
            out.push_back(std::format("  {} {:<8} {} module(s): left -- {}", p.from, p.kind, p.modules, p.why_not));
            continue;
        }
        out.push_back(std::format("  {} {:<8} -> {}  {} module(s), {} verdict(s): {} {} filed, {} there already, {} build signature(s)", p.from, p.kind, p.to,
                                  p.modules, p.found, p.carried, report.dry ? "would be" : "now", p.present, p.built));
        carried += p.carried;
        present += p.present;
    }
    if (v.store_found && !v.profiles.empty())
        out.push_back(std::format("  {} verdict(s) {} filed under the settings in force ({} already were), {} module(s) changed", carried, report.dry ? "would be" : "now", present, v.modules_changed));

    out.push_back("");
    out.push_back("knowledge base (state/db/knowledge.sqlite -> knowledge.v2.sqlite)");
    const KnowledgeReport& k = report.knowledge;
    if (!k.found) {
        out.push_back("  no knowledge base from the Python in this state folder: nothing to carry");
    } else if (k.same_file) {
        out.push_back("  both name the same file (CB_KNOWLEDGE_DB): nothing to carry");
    } else {
        out.push_back(std::format("  bodies: {} module(s), {} bodies {}; {} module(s) this program had indexed already stay as they are", k.modules, k.bodies,
                                  report.dry ? "would be carried" : "carried", k.modules_present));
        out.push_back(std::format("  names given to bodies: {} {}, {} already given", k.names, report.dry ? "would be carried" : "carried", k.names_present));
        out.push_back(std::format("  learned rows: {} {}, {} already held", k.learned, report.dry ? "would be carried" : "carried", k.learned_present));
        for (const Left& l : k.left) out.push_back(std::format("  left: {} -- {} row(s): {}", l.what, l.rows, l.why));
    }
    return out;
}

cli::Command command(const Services& services) {
    cli::Command c;
    c.name = "migrate-state";
    c.summary = "carry what the Python-driven program kept (judged results, the knowledge base) to where this program reads it";
    c.description =
        "One-time and safe to run again: it files the verdicts judged under an older program's profile digests under the digest of the same verifier\n"
        "settings now, and copies the Python's knowledge base (the bodies and their names, the learned rows) into the one this program keeps.\n"
        "Every old file stays as it is and nothing this program already holds is replaced. See docs/STATE_MIGRATION.md.";
    c.params = {
        cli::Option{"--dry", "", "say what would be carried; write nothing", cli::Arg::Flag},
        cli::Option{"--state", "", "the state folder (the configured one by default)", cli::Arg::Value, "DIR"},
    };
    c.handler = [services](const cli::Args& a) {
        Services s = services;
        if (const std::optional<std::string> state = a.value("state")) s.state_dir = fs::from_text(*state);
        const Report report = migrate(s, Options{.dry = a.flag("dry")});
        for (const std::string& line : render(report)) out("{}\n", line);
        return kExitOk;
    };
    return c;
}

}  // namespace cb::store::migrate
