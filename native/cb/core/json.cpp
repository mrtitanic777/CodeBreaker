#include "cb/core/json.hpp"

#include <nlohmann/json.hpp>

#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"

namespace cb::json {

namespace {

// Position (1-based line and column) of the 1-based byte index nlohmann reports.
std::pair<std::uint32_t, std::uint32_t> line_column(std::string_view text, std::size_t byte) {
    std::uint32_t line = 1, col = 1;
    const std::size_t stop = byte == 0 ? 0 : std::min(byte - 1, text.size());
    for (std::size_t i = 0; i < stop; ++i) {
        if (text[i] == '\n') {
            ++line;
            col = 1;
        } else {
            ++col;
        }
    }
    return {line, col};
}

[[noreturn]] void fail(std::string_view name, std::string_view text, const nlohmann::json::parse_error& e) {
    const auto [line, col] = line_column(text, e.byte);
    // e.what() carries nlohmann's own "[json.exception.parse_error.101] parse error at line 1, column 3: ..." text;
    // keep the part after the location, which says what was wrong.
    std::string what = e.what();
    if (const auto at = what.find("]: "); at == std::string::npos) what = "invalid JSON";
    else what = what.substr(at + 3);
    throw ParseError(name, line, col, what);
}

Json parse_named(std::string_view name, std::string_view text) {
    try {
        return Json::parse(text.begin(), text.end(), nullptr, true, false);
    } catch (const nlohmann::json::parse_error& e) {
        fail(name, text, e);
    }
}

// ---- the layout Python's json.dumps wrote -------------------------------------------------------------

void python_string(std::string& out, const std::string& s) {
    out.push_back('"');
    auto u_escape = [&out](char32_t u) { out += std::format("\\u{:04x}", static_cast<unsigned>(u)); };
    const std::string clean = text::is_valid_utf8(s) ? std::string() : text::sanitize_utf8(s);
    const std::string& t = clean.empty() ? s : clean;
    for (std::size_t i = 0; i < t.size();) {
        const auto b = static_cast<unsigned char>(t[i]);
        if (b < 0x80) {
            switch (b) {
                case '"': out += "\\\""; break;
                case '\\': out += "\\\\"; break;
                case '\n': out += "\\n"; break;
                case '\r': out += "\\r"; break;
                case '\t': out += "\\t"; break;
                case '\b': out += "\\b"; break;
                case '\f': out += "\\f"; break;
                default:
                    if (b < 0x20) u_escape(b);
                    else out.push_back(static_cast<char>(b));
            }
            ++i;
            continue;
        }
        const std::size_t len = b >= 0xF0 ? 4 : b >= 0xE0 ? 3 : 2;
        char32_t cp = b & (0xFFu >> (len + 1));
        for (std::size_t k = 1; k < len; ++k) cp = (cp << 6) | (static_cast<unsigned char>(t[i + k]) & 0x3Fu);
        i += len;
        if (cp >= 0x10000) {
            cp -= 0x10000;
            u_escape(0xD800 + (cp >> 10));
            u_escape(0xDC00 + (cp & 0x3FF));
        } else {
            u_escape(cp);
        }
    }
    out.push_back('"');
}

void python_value(std::string& out, const Json& j, int indent, int depth) {
    auto newline = [&](int d) {
        out.push_back('\n');
        out.append(static_cast<std::size_t>(indent * d), ' ');
    };
    switch (j.type()) {
        case nlohmann::json::value_t::null: out += "null"; break;
        case nlohmann::json::value_t::boolean: out += j.get<bool>() ? "true" : "false"; break;
        case nlohmann::json::value_t::number_integer: out += std::to_string(j.get<std::int64_t>()); break;
        case nlohmann::json::value_t::number_unsigned: out += std::to_string(j.get<std::uint64_t>()); break;
        case nlohmann::json::value_t::number_float: {
            std::string f = text::format_double(j.get<double>());
            if (f.find_first_of(".en") == std::string::npos) f += ".0";   // Python's repr(1.0) is "1.0"
            out += f;
            break;
        }
        case nlohmann::json::value_t::string: python_string(out, j.get<std::string>()); break;
        case nlohmann::json::value_t::array: {
            if (j.empty()) { out += "[]"; break; }
            out.push_back('[');
            bool first = true;
            for (const auto& item : j) {
                if (!first) out += indent > 0 ? "," : ", ";
                first = false;
                if (indent > 0) newline(depth + 1);
                python_value(out, item, indent, depth + 1);
            }
            if (indent > 0) newline(depth);
            out.push_back(']');
            break;
        }
        case nlohmann::json::value_t::object: {
            if (j.empty()) { out += "{}"; break; }
            out.push_back('{');
            bool first = true;
            for (const auto& [key, value] : j.items()) {
                if (!first) out += indent > 0 ? "," : ", ";
                first = false;
                if (indent > 0) newline(depth + 1);
                python_string(out, key);
                out += ": ";
                python_value(out, value, indent, depth + 1);
            }
            if (indent > 0) newline(depth);
            out.push_back('}');
            break;
        }
        default: out += "null"; break;   // binary and discarded values have no JSON form
    }
}

}  // namespace

Json parse(std::string_view text) { return parse_named("<json>", text); }

Json load(const std::filesystem::path& p) {
    const std::string content = fs::read_text(p);
    std::string_view body = content;
    if (body.starts_with("\xEF\xBB\xBF")) body.remove_prefix(3);   // a UTF-8 BOM is not part of the document
    return parse_named(fs::to_text(p), body);
}

bool try_parse(std::string_view text, Json& out) {
    Json value = Json::parse(text.begin(), text.end(), nullptr, false, false);
    if (value.is_discarded()) return false;
    out = std::move(value);
    return true;
}

std::string dump(const Json& j, Style style) {
    switch (style) {
        case Style::Compact: return j.dump(-1, ' ', false, nlohmann::json::error_handler_t::replace);
        case Style::Pretty: return j.dump(2, ' ', false, nlohmann::json::error_handler_t::replace);
        case Style::PythonCompatible:
        case Style::PythonCompatiblePretty: {
            std::string out;
            python_value(out, j, style == Style::PythonCompatiblePretty ? 2 : 0, 0);
            return out;
        }
    }
    return {};
}

void save(const std::filesystem::path& p, const Json& j, Style style) {
    const std::string text = dump(j, style) + "\n";
    fs::write_file_atomic(p, as_bytes(text));
}

}  // namespace cb::json
