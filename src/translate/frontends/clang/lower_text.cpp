// translate/frontends/clang/lower_text.cpp -- the entry points of the lowering and the small text and width helpers it
// shares: string and character literals, a type's spelling, a constant reduced to its type's width.
#include <algorithm>
#include <format>
#include <limits>

#include "core/json.hpp"
#include "core/text.hpp"
#include "translate/frontends/clang/lower_impl.hpp"

namespace cb::translate::frontends::clang {

namespace {

bool is_hex_digit(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }
bool is_octal_digit(char c) { return c >= '0' && c <= '7'; }

// The code point as UTF-8 (a lone surrogate as the three bytes generalised UTF-8 gives it).
void append_code_point(std::string& out, std::uint32_t cp) {
    if (cp < 0x80) {
        out += static_cast<char>(cp);
    } else if (cp < 0x800) {
        out += static_cast<char>(0xC0 | (cp >> 6));
        out += static_cast<char>(0x80 | (cp & 0x3F));
    } else if (cp < 0x10000) {
        out += static_cast<char>(0xE0 | (cp >> 12));
        out += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
        out += static_cast<char>(0x80 | (cp & 0x3F));
    } else {
        out += static_cast<char>(0xF0 | (cp >> 18));
        out += static_cast<char>(0x80 | ((cp >> 12) & 0x3F));
        out += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
        out += static_cast<char>(0x80 | (cp & 0x3F));
    }
}

[[noreturn]] void bad_literal(const std::string& what) { throw ParseError("string literal", 0, 0, what); }

std::uint32_t digits_value(std::string_view digits, int base) {
    if (digits.empty()) bad_literal("an escape with no digits");
    std::uint64_t v = 0;
    for (const char c : digits) {
        const int d = c <= '9' ? c - '0' : (c | 0x20) - 'a' + 10;
        v = v * static_cast<std::uint64_t>(base) + static_cast<std::uint64_t>(d);
        if (v > 0xFFFFFFFFull) v &= 0xFFFFFFFFull;
    }
    return static_cast<std::uint32_t>(v);
}

std::optional<char> simple_escape(char e) {
    switch (e) {
        case 'n': return '\n';
        case 't': return '\t';
        case 'r': return '\r';
        case '0': return '\0';
        case 'a': return '\a';
        case 'b': return '\b';
        case 'f': return '\f';
        case 'v': return '\v';
        case '\\': return '\\';
        case '\'': return '\'';
        case '"': return '"';
        case '?': return '?';
        case 'e': return '\x1b';
        default: return std::nullopt;
    }
}

// A wide literal's body: its UTF-16 code units, one character each (written as UTF-8).
std::string wide_value(std::string_view body) {
    std::string out;
    std::size_t i = 0;
    while (i < body.size()) {
        const char c = body[i];
        if (c != '\\') {
            out += c;
            ++i;
            continue;
        }
        if (i + 1 >= body.size()) bad_literal("a literal ends in a backslash");
        const char e = body[i + 1];
        if (e == 'x' || e == 'u' || e == 'U') {
            std::size_t j = i + 2;
            const std::size_t limit = e == 'x' ? std::string_view::npos : i + 2 + (e == 'u' ? 4 : 8);
            while (j < body.size() && is_hex_digit(body[j]) && j < limit) ++j;
            append_code_point(out, digits_value(body.substr(i + 2, j - (i + 2)), 16) & 0xFFFF);
            i = j;
        } else if (is_octal_digit(e)) {
            std::size_t j = i + 1;
            while (j < body.size() && j < i + 4 && is_octal_digit(body[j])) ++j;
            append_code_point(out, digits_value(body.substr(i + 1, j - (i + 1)), 8));
            i = j;
        } else {
            const std::optional<char> simple = (e == 'e') ? std::nullopt : simple_escape(e);   // (no \e in a wide literal)
            if (simple) {
                out += *simple;
            } else {
                out += e;
            }
            i += 2;
        }
    }
    return out;
}

}  // namespace

std::string string_literal_value(std::string_view s) {
    const std::size_t quote = s.find('"');
    const std::size_t last = s.rfind('"');
    if (quote == std::string_view::npos) bad_literal("not a string literal");
    const std::string_view prefix = s.substr(0, quote);
    if (prefix == "U") bad_literal("UTF-32 string literal");
    const std::string_view body = last > quote ? s.substr(quote + 1, last - quote - 1) : std::string_view();
    if (prefix == "L" || prefix == "u") return wide_value(body);
    std::string out;   // each byte of the result is one latin-1 character, written as UTF-8
    const auto put_byte = [&out](std::uint8_t b) { append_code_point(out, b); };
    std::size_t i = 0;
    while (i < body.size()) {
        const auto c = static_cast<std::uint8_t>(body[i]);
        if (c != '\\') {
            put_byte(c);
            ++i;
            continue;
        }
        ++i;
        if (i >= body.size()) bad_literal("a literal ends in a backslash");
        const char e = body[i];
        if (e == 'x') {
            std::size_t j = i + 1;
            while (j < body.size() && is_hex_digit(body[j])) ++j;
            put_byte(static_cast<std::uint8_t>(digits_value(body.substr(i + 1, j - (i + 1)), 16) & 0xFF));
            i = j;
        } else if (is_octal_digit(e)) {
            std::size_t j = i;
            while (j < body.size() && j < i + 3 && is_octal_digit(body[j])) ++j;
            put_byte(static_cast<std::uint8_t>(digits_value(body.substr(i, j - i), 8) & 0xFF));
            i = j;
        } else if (const std::optional<char> simple = simple_escape(e)) {
            put_byte(static_cast<std::uint8_t>(*simple));
            ++i;
        } else {
            put_byte(static_cast<std::uint8_t>(body[i]));
            ++i;
        }
    }
    return out;
}

std::optional<std::string> char_literal_text(std::int64_t v) {
    if (v >= 32 && v < 127 && v != '\'' && v != '\\') return std::format("'{}'", static_cast<char>(v));
    switch (v) {
        case 10: return "'\\n'";
        case 9: return "'\\t'";
        case 13: return "'\\r'";
        case 0: return "'\\0'";
        case 39: return "'\\''";
        case 92: return "'\\\\'";
        default: return std::nullopt;
    }
}

std::string spell(const ir::TypeP& t) {
    if (!t) return "?";
    return std::visit(
        [&](const auto& body) -> std::string {
            using B = std::decay_t<decltype(body)>;
            if constexpr (std::is_same_v<B, ir::ClassType>) {
                if (body.args.empty()) return body.name;
                std::vector<std::string> args;
                for (const ir::TypeP& a : body.args) args.push_back(spell(a));
                return body.name + "<" + text::join(args, ", ") + ">";
            } else if constexpr (std::is_same_v<B, ir::PtrType>) {
                return spell(body.to) + (body.ref ? "&" : "*");
            } else if constexpr (std::is_same_v<B, ir::ArrayType>) {
                return spell(body.elem) + "[" + (body.count ? std::to_string(*body.count) : std::string()) + "]";
            } else if constexpr (std::is_same_v<B, ir::FnPtrType>) {
                return spell(body.fn) + "*";
            } else if constexpr (std::is_same_v<B, ir::FnType>) {
                std::vector<std::string> params;
                for (const ir::FnParam& p : body.params) params.push_back(spell(p.type));
                return spell(body.result.type) + "(" + text::join(params, ",") + ")";
            } else {
                return std::string(t->kind());
            }
        },
        t->body);
}

std::int64_t wrap_to_type(std::int64_t v, const ir::TypeP& t) {
    const auto* prim = t ? std::get_if<ir::PrimType>(&t->body) : nullptr;
    if (prim == nullptr) return v;
    unsigned bits = 0;
    bool is_signed = false;
    switch (prim->prim) {
        case ir::Prim::Bool: return v != 0 ? 1 : 0;
        case ir::Prim::I8: bits = 8; is_signed = true; break;
        case ir::Prim::U8: bits = 8; break;
        case ir::Prim::Char: bits = 8; is_signed = prim->is_signed.value_or(true); break;
        case ir::Prim::I16: bits = 16; is_signed = true; break;
        case ir::Prim::U16: bits = 16; break;
        case ir::Prim::I32: bits = 32; is_signed = true; break;
        case ir::Prim::U32: bits = 32; break;
        case ir::Prim::I64: return v;
        case ir::Prim::U64: return v;   // (a value past int64 cannot be held: the constant is kept as it is)
        default: return v;
    }
    const std::uint64_t mask = (std::uint64_t{1} << bits) - 1;
    std::uint64_t u = static_cast<std::uint64_t>(v) & mask;
    if (is_signed && u >= (std::uint64_t{1} << (bits - 1))) return static_cast<std::int64_t>(u) - (std::int64_t{1} << bits);
    return static_cast<std::int64_t>(u);
}

Lowered lower(Arena& arena, const Json& tree, const Options& options) {
    Lowering lowering(arena, tree, options);
    Lowered out;
    out.unit = lowering.unit();
    for (ir::Node* n : ir::unsupported(out.unit)) {
        const auto* u = ir::get<ir::Unsupported>(n);
        out.refused.push_back(Refusal{n->line, u->kind, u->raw});
    }
    return out;
}

Lowered lower_text(Arena& arena, std::string_view dump_text, const Options& options) {
    const Json tree = json::parse(dump_text);
    return lower(arena, tree, options);
}

}  // namespace cb::translate::frontends::clang
