// source/console_writer.cpp -- the one source writer for the consoles (console_writer.hpp).

#include "source/console_writer.hpp"

#include <format>
#include <map>
#include <memory>
#include <mutex>
#include <nlohmann/json.hpp>

#include "base/config.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "knowledge/package.hpp"
#include "source/z80_writer.hpp"

namespace cb::source::console_writer {

namespace {

std::string text_of(const nlohmann::ordered_json& j, const char* key) {
    const auto it = j.find(key);
    return it != j.end() && it->is_string() ? it->get<std::string>() : std::string();
}

Description read_description(std::string_view name) {
    const std::filesystem::path file = config::current().config_root() / "source" / "platforms" / (std::string(name) + ".json");
    if (!std::filesystem::exists(file)) throw UsageError(std::format("no console platform {} (config/source/platforms/{}.json)", name, name));
    const nlohmann::ordered_json j = nlohmann::ordered_json::parse(fs::read_text(file));
    Description d;
    d.name = std::string(name);
    d.platform = text_of(j, "platform");
    if (d.platform.empty()) d.platform = d.name;
    d.title = text_of(j, "title");
    d.shape = text_of(j, "shape");
    d.dialect = text_of(j, "dialect");
    d.ports = text_of(j, "ports");
    d.fixed = text_of(j, "fixed");
    if (const auto head = j.find("head"); head != j.end()) {
        for (const auto& line : *head) {
            if (line.is_string()) {
                d.head.push_back({line.get<std::string>(), {}, {}});
            } else {
                d.head.push_back({text_of(line, "line"), text_of(line, "when"), text_of(line, "each")});
            }
        }
    }
    return d;
}

const Value& value_of(const Values& values, std::string_view name) {
    const auto it = values.find(name);
    if (it == values.end()) throw UsageError(std::format("a head template names {}, which the loader does not give", name));
    return it->second;
}

bool is_set(const Value& v) {
    if (const auto* n = std::get_if<std::int64_t>(&v)) return *n != 0;
    if (const auto* s = std::get_if<std::string>(&v)) return !s->empty();
    return !std::get<std::vector<std::string>>(v).empty();
}

// The position of the first `c` in `text` outside braces, npos when there is none.
std::size_t top_level(std::string_view text, char c) {
    int depth = 0;
    for (std::size_t i = 0; i < text.size(); ++i) {
        if (text[i] == '{') ++depth;
        else if (text[i] == '}') --depth;
        else if (depth == 0 && text[i] == c) return i;
    }
    return std::string_view::npos;
}

std::string placeholder(std::string_view inner, const Values& values) {
    if (const std::size_t q = top_level(inner, '?'); q != std::string_view::npos) {
        const std::string_view branches = inner.substr(q + 1);
        const std::size_t bar = top_level(branches, '|');
        const std::string_view yes = branches.substr(0, bar);
        const std::string_view no = bar == std::string_view::npos ? std::string_view() : branches.substr(bar + 1);
        return fill(is_set(value_of(values, inner.substr(0, q))) ? yes : no, values);
    }
    const std::size_t colon = inner.find(':');
    const std::string_view name = inner.substr(0, colon);
    const std::string spec = colon == std::string_view::npos ? std::string("{}") : "{:" + std::string(inner.substr(colon + 1)) + "}";
    const Value& v = value_of(values, name);
    if (const auto* n = std::get_if<std::int64_t>(&v)) return std::vformat(spec, std::make_format_args(*n));
    if (const auto* s = std::get_if<std::string>(&v)) return std::vformat(spec, std::make_format_args(*s));
    throw UsageError(std::format("{} is a list: a head line says it with `each`", name));
}

// The head comment's lines the description's templates make.
std::vector<std::string> head_lines(const Description& d, const Values& values) {
    std::vector<std::string> out;
    for (const HeadLine& line : d.head) {
        if (!line.when.empty() && !is_set(value_of(values, line.when))) continue;
        if (line.each.empty()) {
            out.push_back(fill(line.text, values));
            continue;
        }
        const Value& list = value_of(values, line.each);
        if (!std::holds_alternative<std::vector<std::string>>(list)) throw UsageError(std::format("`each` names {}, which is not a list", line.each));
        Values with_it = values;
        for (const std::string& it : std::get<std::vector<std::string>>(list)) {
            with_it.insert_or_assign("it", it);
            out.push_back(fill(line.text, with_it));
        }
    }
    return out;
}

// The z80-block shape: one block at one address (source/z80_writer), with the platform's ports and fixed addresses.
image_writer::Written write_z80_block(const Description& d, const Loaded& program, const std::vector<std::string>& head, std::string_view stem,
                                      const std::filesystem::path& folder) {
    z80_writer::Machine machine{.ports = d.ports.empty() ? image_writer::Names{} : knowledge::standard().hardware_names(d.platform, d.ports),
                                .names = d.fixed.empty() ? image_writer::Names{} : knowledge::standard().table_names(d.platform, d.fixed),
                                .dialect = d.dialect.empty() ? std::string("sjasmplus") : d.dialect};
    const z80_writer::Block block{.image = program.image, .origin = program.origin, .entries = program.entries, .header = head};
    return z80_writer::write(block, machine, folder, stem);
}

}  // namespace

const Description& describe(std::string_view name) {
    static std::mutex lock;
    static std::map<std::string, std::unique_ptr<Description>, std::less<>> cache;
    std::lock_guard hold(lock);
    if (const auto it = cache.find(name); it != cache.end()) return *it->second;
    auto made = std::make_unique<Description>(read_description(name));
    const Description& ref = *made;
    cache.emplace(std::string(name), std::move(made));
    return ref;
}

std::string fill(std::string_view text, const Values& values) {
    std::string out;
    out.reserve(text.size());
    for (std::size_t i = 0; i < text.size(); ++i) {
        if (text[i] != '{') {
            out += text[i];
            continue;
        }
        int depth = 0;
        std::size_t close = i;
        for (; close < text.size(); ++close) {
            if (text[close] == '{') ++depth;
            else if (text[close] == '}' && --depth == 0) break;
        }
        if (close == text.size()) throw UsageError(std::format("a head template has an unclosed brace: {}", text));
        out += placeholder(text.substr(i + 1, close - i - 1), values);
        i = close;
    }
    return out;
}

image_writer::Written write(const Description& description, const Loaded& program, std::string_view file_name, const std::filesystem::path& folder) {
    Values values = program.values;
    values.insert_or_assign("file", std::string(file_name));
    values.insert_or_assign("size", static_cast<std::int64_t>(program.image.size()));
    values.insert_or_assign("kb", static_cast<std::int64_t>(program.image.size() / 1024));
    values.insert_or_assign("origin", static_cast<std::int64_t>(program.origin));
    const std::vector<std::string> head = head_lines(description, values);
    const std::string stem = std::filesystem::path(file_name).stem().string();
    if (description.shape == "z80-block") return write_z80_block(description, program, head, stem, folder);
    throw UsageError(std::format("console platform {} names the shape {}, which the writer does not have", description.name, description.shape));
}

image_writer::Written reconstruct(std::string_view platform, const std::filesystem::path& file, const std::filesystem::path& folder) {
    const Description& d = describe(platform);
    return write(d, load(platform, file), file.filename().string(), folder);
}

}  // namespace cb::source::console_writer
