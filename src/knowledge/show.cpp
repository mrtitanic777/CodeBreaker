// cb-unity: alone (helpers named for what they do here)
#include "knowledge/show.hpp"
#include "core/sys.hpp"

#include <algorithm>
#include <cmath>
#include <ctime>
#include <format>

#include <nlohmann/json.hpp>

#include "core/json.hpp"
#include "core/log.hpp"
#include "core/text.hpp"

namespace cb::knowledge::show {

namespace {

// The longest prefix of `s` that is at most `limit` bytes and does not cut a UTF-8 character.
std::string_view prefix_at_character(std::string_view s, std::size_t limit) {
    if (s.size() <= limit) return s;
    std::size_t cut = limit;
    while (cut > 0 && (static_cast<unsigned char>(s[cut]) & 0xC0) == 0x80) --cut;   // s[cut] is a continuation byte
    return s.substr(0, cut);
}

Json to_json(const Value& v) {
    switch (v.kind()) {
        case Value::Kind::Null: return nullptr;
        case Value::Kind::Bool: return *v.boolean();
        case Value::Kind::Int: return *v.integer();
        case Value::Kind::Double: return *v.number();
        case Value::Kind::Text: return *v.text();
        case Value::Kind::List: {
            Json out = Json::array();
            for (const Value& item : *v.list()) out.push_back(to_json(item));
            return out;
        }
        case Value::Kind::Map: {
            Json out = Json::object();
            for (const auto& [key, item] : *v.map()) out[key] = to_json(item);
            return out;
        }
    }
    return nullptr;
}

// Bytes have no JSON form: they show as their size and the first bytes in hex.
std::string bytes_preview(ByteView b) {
    constexpr std::size_t kShown = 32;
    std::string hex;
    for (std::size_t i = 0; i < std::min(b.size(), kShown); ++i) hex += std::format("{:02x}", b[i]);
    return std::format("<{} bytes: {}{}>", b.size(), hex, b.size() > kShown ? "..." : "");
}

std::string clip_source(std::string_view source) {
    const std::string_view head = text::utf8_prefix(source, kSourceColumns);
    return head.size() < source.size() ? std::string(head) + "..." : std::string(head);
}

std::string kind_line(const KindRow& row) {
    std::vector<std::string> origins;
    for (const auto& [name, count] : row.origins) origins.push_back(std::format("{} {}", name, count));
    return std::format("  {:<8} {:<24} {:>7}  {:>8.1f} KB  {:<28} {}", row.platform, row.kind, row.keys,
                       static_cast<double>(row.bytes) / 1024.0, text::join(origins, ", "), clip_source(row.source));
}

}  // namespace

std::string dump(const Stored& value) {
    Json j;
    switch (value.kind) {
        case Stored::Kind::Bytes: j = bytes_preview(value.bytes); break;
        case Stored::Kind::Text: j = value.text; break;
        case Stored::Kind::Structured: j = to_json(value.structured); break;
    }
    return j.dump(1, ' ', false, nlohmann::json::error_handler_t::replace);
}

std::string local_time_text(double epoch_seconds) {
    const std::time_t t = static_cast<std::time_t>(std::floor(epoch_seconds));
    std::tm local{};
    local = cb::sys::local_tm(t);
    char buf[64];
    std::strftime(buf, sizeof buf, "%Y-%m-%d %I:%M %p", &local);
    return buf;
}

Outcome run(Base& base, std::span<const std::string> args_in) {
    std::vector<std::string> args;
    bool load = false;
    for (const std::string& a : args_in) {
        if (a == "--load") {
            load = true;
        } else {
            args.push_back(a);
        }
    }
    if (load) base.load_authored();

    Outcome out;
    auto say = [&](const std::string& line) { out.text += line + "\n"; };

    if (args.size() <= 1) {   // every kind, of one platform or of all
        std::optional<std::string> want;
        if (!args.empty()) want = base.platform(args[0]);
        say(base.path());
        for (const KindRow& row : base.kinds(want ? std::optional<std::string_view>(*want) : std::nullopt)) say(kind_line(row));
        const SharedBodies bodies = base.shared_bodies();
        say(std::format("  (bodies) {} recorded, {} distinct, {} in two or more modules", bodies.recorded, bodies.distinct,
                        bodies.in_several_modules));
        return out;
    }

    const std::string platform = base.platform(args[0]);
    const std::string& kind = args[1];
    if (args.size() == 2) {   // the keys of a kind
        std::size_t total = 0;
        for (const std::string& key : base.first_keys(platform, kind, kKeyListLimit, total)) say(key);
        say(std::format("({})", total));
        return out;
    }

    const std::string& key = args[2];   // one piece
    const std::optional<Stored> value = base.value(platform, kind, key);
    const std::optional<Provenance> about = base.about(platform, kind, key);
    if (!about) {
        say(std::format("nothing known: {} {} {}", platform, kind, key));
        out.exit_code = 1;
        return out;
    }
    const std::string shown = value ? dump(*value) : "null";
    say(std::string(prefix_at_character(shown, kMaxValueBytes)));
    const double made = about->made_at ? *about->made_at : static_cast<double>(std::time(nullptr));
    say(std::format("source: {}\norigin: {}\nmade:   {}\nhash:   {}", about->source, about->origin, local_time_text(made), about->hash));
    return out;
}

cli::Command command(const Environment& environment) {
    cli::Command c;
    c.name = "knowledge";
    c.summary = "what the knowledge base holds, and where each piece came from";
    c.params = {
        cli::Positional{"platform", "a platform (or a lane's name for it): its kinds", false},
        cli::Positional{"kind", "a kind: its keys (the first 200)", false},
        cli::Positional{"key", "a key: its value, source, origin", false},
        cli::Option{"--load", "", "load every authored file first", cli::Arg::Flag},
    };
    c.handler = [environment](const cli::Args& a) {
        std::vector<std::string> words;
        for (const char* name : {"platform", "kind", "key"}) {
            const std::optional<std::string> word = a.value(name);
            if (!word) break;
            words.push_back(*word);
        }
        if (a.flag("load")) words.emplace_back("--load");
        const Outcome outcome = run(environment.base(), words);
        out_text(outcome.text);
        return outcome.exit_code;
    };
    return c;
}

}  // namespace cb::knowledge::show
