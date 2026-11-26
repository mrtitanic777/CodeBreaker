// dotnet/metadata/type_name.cpp -- the type name and assembly name parsers (System.Reflection.Metadata's, decision for decision).
#include "dotnet/metadata/type_name.hpp"

#include <charconv>

namespace cb::dotnet::metadata {
namespace {

// ---- text helpers: UTF-8 with .NET's white space ------------------------------------------------------------------------

// The code point at s[i] and its length in bytes (a malformed byte counts as one byte of its own value).
char32_t decode_at(std::string_view s, std::size_t i, std::size_t& length) {
    const auto b = static_cast<unsigned char>(s[i]);
    const auto cont = [&](std::size_t k) { return i + k < s.size() && (static_cast<unsigned char>(s[i + k]) & 0xC0) == 0x80; };
    if (b < 0x80) { length = 1; return b; }
    if ((b & 0xE0) == 0xC0 && cont(1)) { length = 2; return (char32_t(b & 0x1F) << 6) | (static_cast<unsigned char>(s[i + 1]) & 0x3F); }
    if ((b & 0xF0) == 0xE0 && cont(1) && cont(2)) {
        length = 3;
        return (char32_t(b & 0x0F) << 12) | (char32_t(static_cast<unsigned char>(s[i + 1]) & 0x3F) << 6) | (static_cast<unsigned char>(s[i + 2]) & 0x3F);
    }
    if ((b & 0xF8) == 0xF0 && cont(1) && cont(2) && cont(3)) {
        length = 4;
        return (char32_t(b & 0x07) << 18) | (char32_t(static_cast<unsigned char>(s[i + 1]) & 0x3F) << 12) |
               (char32_t(static_cast<unsigned char>(s[i + 2]) & 0x3F) << 6) | (static_cast<unsigned char>(s[i + 3]) & 0x3F);
    }
    length = 1;
    return b;
}

// char.IsWhiteSpace
bool is_unicode_white_space(char32_t c) {
    return (c >= 0x09 && c <= 0x0D) || c == 0x20 || c == 0x85 || c == 0xA0 || c == 0x1680 || (c >= 0x2000 && c <= 0x200A) || c == 0x2028 ||
           c == 0x2029 || c == 0x202F || c == 0x205F || c == 0x3000;
}

std::string_view trim_start(std::string_view s) {
    while (!s.empty()) {
        std::size_t length = 0;
        if (!is_unicode_white_space(decode_at(s, 0, length))) break;
        s.remove_prefix(length);
    }
    return s;
}

bool starts_with_white_space(std::string_view s) {
    std::size_t length = 0;
    return !s.empty() && is_unicode_white_space(decode_at(s, 0, length));
}

bool ends_with_white_space(std::string_view s) {
    if (s.empty()) return false;
    std::size_t start = s.size() - 1;
    while (start > 0 && (static_cast<unsigned char>(s[start]) & 0xC0) == 0x80 && s.size() - start < 4) --start;
    std::size_t length = 0;
    const char32_t c = decode_at(s, start, length);
    return start + length == s.size() && is_unicode_white_space(c);
}

bool equals_ignore_case(std::string_view a, std::string_view b) {
    if (a.size() != b.size()) return false;
    for (std::size_t i = 0; i < a.size(); ++i) {
        char x = a[i], y = b[i];
        if (x >= 'A' && x <= 'Z') x = static_cast<char>(x - 'A' + 'a');
        if (y >= 'A' && y <= 'Z') y = static_cast<char>(y - 'A' + 'a');
        if (x != y) return false;
    }
    return true;
}

// ---- TypeNameParserHelpers ----------------------------------------------------------------------------------------------

constexpr char kEscape = '\\';
constexpr int kMaxNodes = 20;   // TypeNameParseOptions.MaxNodes's default

bool needs_escaping(char c) { return c == '[' || c == ']' || c == '&' || c == '*' || c == ',' || c == '+' || c == kEscape; }

int get_full_type_name_length(std::string_view input, bool& is_nested_type) {
    is_nested_type = false;
    int offset = 0;
    for (; offset < static_cast<int>(input.size()); ++offset) {
        const char c = input[static_cast<std::size_t>(offset)];
        if (c == kEscape) {
            ++offset;   // skip the escaped char
            if (offset == static_cast<int>(input.size()) || !needs_escaping(input[static_cast<std::size_t>(offset)])) {
                offset = -1;   // ends with the escape character, or escapes a char that needs no escaping
                break;
            }
        } else if (needs_escaping(c)) {
            break;
        }
    }
    is_nested_type = offset > 0 && offset < static_cast<int>(input.size()) && input[static_cast<std::size_t>(offset)] == '+';
    return offset;
}

std::string_view get_name(std::string_view full_name) {
    long offset = static_cast<long>(full_name.find_last_of(".+"));
    if (full_name.find_last_of(".+") == std::string_view::npos) offset = -1;
    if (offset > 0 && full_name[static_cast<std::size_t>(offset - 1)] == kEscape) {
        for (; offset >= 0; --offset) {
            const char c = full_name[static_cast<std::size_t>(offset)];
            if (c == '.' || c == '+') {
                if (offset == 0 || full_name[static_cast<std::size_t>(offset - 1)] != kEscape) break;
                --offset;   // skip the escaping character
            }
        }
    }
    return offset < 0 ? full_name : full_name.substr(static_cast<std::size_t>(offset + 1));
}

std::string_view get_assembly_name_candidate(std::string_view input) {
    std::size_t offset = input.find(']');
    if (offset != std::string_view::npos && offset > 0 && input[offset - 1] == kEscape) {
        for (; offset < input.size(); ++offset)
            if (input[offset] == ']' && input[offset - 1] != kEscape) break;
    }
    return offset == std::string_view::npos ? input : input.substr(0, offset);
}

std::string rank_or_modifier_text(int rank_or_modifier) {
    if (rank_or_modifier == TypeName::kByRef) return "&";
    if (rank_or_modifier == TypeName::kPointer) return "*";
    if (rank_or_modifier == TypeName::kSZArray) return "[]";
    if (rank_or_modifier == 1) return "[*]";
    return "[" + std::string(static_cast<std::size_t>(rank_or_modifier - 1), ',') + "]";
}

bool try_strip_first_char_and_trailing_spaces(std::string_view& span, char value) {
    if (!span.empty() && span[0] == value) {
        span = trim_start(span.substr(1));
        return true;
    }
    return false;
}

bool is_beginning_of_generic_args(std::string_view& span, bool& double_brackets) {
    double_brackets = false;
    if (!span.empty() && span[0] == '[') {
        const std::string_view trimmed = trim_start(span.substr(1));
        if (!trimmed.empty()) {
            if (trimmed[0] == '[') {
                double_brackets = true;
                span = trim_start(trimmed.substr(1));
                return true;
            }
            if (!(trimmed[0] == ',' || trimmed[0] == '*' || trimmed[0] == ']')) {   // [] or [*] or [,] or [,,,, ...]
                span = trimmed;
                return true;
            }
        }
    }
    return false;
}

bool try_dive(int& depth) {
    ++depth;
    return depth <= kMaxNodes;
}

bool try_get_type_name_info(std::string_view& input, std::vector<int>& nested_name_lengths, int& recursive_depth, int& total_length) {
    bool is_nested_type = false;
    total_length = 0;
    do {
        int length = get_full_type_name_length(input.substr(static_cast<std::size_t>(total_length)), is_nested_type);
        if (length <= 0) return false;   // an invalid escape or an empty name
        // (as .NET: the leading '.' of a name such as ".NoNamespace" is dropped, looking at input[0] whatever the offset)
        if (length > 1 && input[0] == '.' && input.substr(0, static_cast<std::size_t>(length)).rfind('.') == 0) {
            input.remove_prefix(1);
            --length;
        }
        if (is_nested_type) {
            if (!try_dive(recursive_depth)) return false;
            nested_name_lengths.push_back(length);
            total_length += 1;   // skip the '+' sign in the next search
        }
        total_length += length;
    } while (is_nested_type);
    return true;
}

bool try_parse_next_decorator(std::string_view& input, int& rank_or_modifier) {
    const std::string_view original_input = input;
    if (try_strip_first_char_and_trailing_spaces(input, '*')) {
        rank_or_modifier = TypeName::kPointer;
        return true;
    }
    if (try_strip_first_char_and_trailing_spaces(input, '&')) {
        rank_or_modifier = TypeName::kByRef;
        return true;
    }
    if (try_strip_first_char_and_trailing_spaces(input, '[')) {
        int rank = 1;
        bool has_seen_asterisk = false;
        for (;;) {
            if (try_strip_first_char_and_trailing_spaces(input, ']')) {
                rank_or_modifier = rank == 1 && !has_seen_asterisk ? TypeName::kSZArray : rank;
                return true;
            }
            if (has_seen_asterisk) break;
            if (rank == 1 && try_strip_first_char_and_trailing_spaces(input, '*')) {
                has_seen_asterisk = true;
            } else if (try_strip_first_char_and_trailing_spaces(input, ',')) {
                ++rank;
            } else {
                break;
            }
        }
    }
    input = original_input;
    rank_or_modifier = 0;
    return false;
}

// ---- AssemblyNameParser / AssemblyNameFormatter -------------------------------------------------------------------------

class AssemblyNameParser {
public:
    explicit AssemblyNameParser(std::string_view input) : input_(input) {}

    struct Parts {
        std::string name;
        std::vector<std::uint16_t> version;
        std::optional<std::string> culture_name;
        std::uint32_t flags = 0;
        std::optional<std::vector<std::uint8_t>> public_key_or_token;
    };

    bool try_parse(Parts& result) {
        enum Seen : unsigned { Version = 1, Culture = 2, PublicKeyOrToken = 4, ProcessorArchitecture = 8, Retargetable = 16, ContentType = 32 };
        std::string name;
        Token token{};
        if (!try_get_next_token(name, token) || token != Token::String || name.empty()) return false;
        Parts parts;
        parts.name = name;
        unsigned already_seen = 0;
        const auto record = [&](unsigned kind) {
            if ((already_seen & kind) != 0) return false;
            already_seen |= kind;
            return true;
        };
        std::string ignored;
        if (!try_get_next_token(ignored, token)) return false;
        while (token != Token::End) {
            if (token != Token::Comma) return false;
            std::string attribute_name, attribute_value;
            if (!try_get_next_token(attribute_name, token) || token != Token::String) return false;
            if (!try_get_next_token(ignored, token) || token != Token::Equals) return false;
            if (!try_get_next_token(attribute_value, token) || token != Token::String) return false;
            if (attribute_name.empty()) return false;
            if (equals_ignore_case(attribute_name, "Version")) {
                if (!record(Version) || !try_parse_version(attribute_value, parts.version)) return false;
            } else if (equals_ignore_case(attribute_name, "Culture")) {
                if (!record(Culture)) return false;
                parts.culture_name = equals_ignore_case(attribute_value, "Neutral") ? std::string() : attribute_value;
            } else if (equals_ignore_case(attribute_name, "PublicKeyToken")) {
                if (!record(PublicKeyOrToken) || !try_parse_pkt(attribute_value, true, parts.public_key_or_token)) return false;
            } else if (equals_ignore_case(attribute_name, "PublicKey")) {
                if (!record(PublicKeyOrToken) || !try_parse_pkt(attribute_value, false, parts.public_key_or_token)) return false;
                parts.flags |= static_cast<std::uint32_t>(AssemblyNameFlags::PublicKey);
            } else if (equals_ignore_case(attribute_name, "ProcessorArchitecture")) {
                if (!record(ProcessorArchitecture)) return false;
                int arch = 0;
                if (equals_ignore_case(attribute_value, "msil")) arch = 1;
                else if (equals_ignore_case(attribute_value, "x86")) arch = 2;
                else if (equals_ignore_case(attribute_value, "ia64")) arch = 3;
                else if (equals_ignore_case(attribute_value, "amd64")) arch = 4;
                else if (equals_ignore_case(attribute_value, "arm")) arch = 5;
                if (arch == 0) return false;
                parts.flags |= static_cast<std::uint32_t>(arch) << 4;
            } else if (equals_ignore_case(attribute_name, "Retargetable")) {
                if (!record(Retargetable)) return false;
                if (equals_ignore_case(attribute_value, "Yes")) parts.flags |= static_cast<std::uint32_t>(AssemblyNameFlags::Retargetable);
                else if (!equals_ignore_case(attribute_value, "No")) return false;
            } else if (equals_ignore_case(attribute_name, "ContentType")) {
                if (!record(ContentType)) return false;
                if (!equals_ignore_case(attribute_value, "WindowsRuntime")) return false;
                parts.flags |= 1u << 9;   // AssemblyContentType.WindowsRuntime << 9
            }
            // (other attributes are ignored, as .NET does)
            if (!try_get_next_token(ignored, token)) return false;
        }
        result = std::move(parts);
        return true;
    }

private:
    enum class Token { Equals = 1, Comma = 2, String = 3, End = 4 };

    static bool is_white_space(char ch) { return ch == '\n' || ch == '\r' || ch == ' ' || ch == '\t'; }

    static bool try_parse_version(const std::string& value, std::vector<std::uint16_t>& version) {
        std::vector<std::string_view> parts;
        std::size_t start = 0;
        for (;;) {
            const std::size_t dot = value.find('.', start);
            parts.emplace_back(std::string_view(value).substr(start, dot == std::string::npos ? std::string::npos : dot - start));
            if (dot == std::string::npos) break;
            start = dot + 1;
            if (parts.size() > 5) break;
        }
        if (parts.size() < 2 || parts.size() > 4) return false;
        std::array<std::uint16_t, 4> numbers = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
        for (std::size_t i = 0; i < parts.size(); ++i) {
            // ushort.TryParse with NumberStyles.None: digits only
            const std::string_view p = parts[i];
            if (p.empty()) return false;
            unsigned v = 0;
            for (const char c : p) {
                if (c < '0' || c > '9') return false;
                v = v * 10 + static_cast<unsigned>(c - '0');
                if (v > 0xFFFF) return false;
            }
            numbers[i] = static_cast<std::uint16_t>(v);
        }
        if (numbers[0] == 0xFFFF || numbers[1] == 0xFFFF) return false;
        version = {numbers[0], numbers[1]};
        if (numbers[2] != 0xFFFF) {
            version.push_back(numbers[2]);
            if (numbers[3] != 0xFFFF) version.push_back(numbers[3]);
        }
        return true;
    }

    static bool try_parse_pkt(const std::string& value, bool is_token, std::optional<std::vector<std::uint8_t>>& result) {
        if (equals_ignore_case(value, "null") || value.empty()) {
            result = std::vector<std::uint8_t>();
            return true;
        }
        if (value.size() % 2 != 0 || (is_token && value.size() != 16)) return false;
        std::vector<std::uint8_t> bytes(value.size() / 2);
        const auto nibble = [](char c) -> int {
            if (c >= '0' && c <= '9') return c - '0';
            if (c >= 'a' && c <= 'f') return c - 'a' + 10;
            if (c >= 'A' && c <= 'F') return c - 'A' + 10;
            return -1;
        };
        for (std::size_t i = 0; i < bytes.size(); ++i) {
            const int hi = nibble(value[2 * i]), lo = nibble(value[2 * i + 1]);
            if (hi < 0 || lo < 0) return false;
            bytes[i] = static_cast<std::uint8_t>(hi << 4 | lo);
        }
        result = std::move(bytes);
        return true;
    }

    bool try_get_next_char(char& ch) {
        if (index_ < input_.size()) {
            ch = input_[index_++];
            if (ch == '\0') return false;
        } else {
            ch = '\0';
        }
        return true;
    }

    bool try_get_next_token(std::string& token_string, Token& token) {
        token_string.clear();
        char c = 0;
        for (;;) {
            if (!try_get_next_char(c)) return false;
            switch (c) {
            case ',': token = Token::Comma; return true;
            case '=': token = Token::Equals; return true;
            case '\0': token = Token::End; return true;
            default: break;
            }
            if (!is_white_space(c)) break;
        }
        std::string sb;
        char quote_char = '\0';
        if (c == '\'' || c == '"') {
            quote_char = c;
            if (!try_get_next_char(c)) return false;
        }
        for (;;) {
            if (c == 0) {
                if (quote_char != 0) return false;   // missing the closing quote
                break;
            }
            if (quote_char != 0 && c == quote_char) break;   // the closing quote
            if (quote_char == 0 && (c == ',' || c == '=')) {
                --index_;
                break;   // the start of a new ',' or '=' token
            }
            if (quote_char == 0 && (c == '\'' || c == '"')) return false;
            if (c == '\\') {
                if (!try_get_next_char(c)) return false;
                switch (c) {
                case '\\': case ',': case '=': case '\'': case '"': sb.push_back(c); break;
                case 't': sb.push_back('\t'); break;
                case 'r': sb.push_back('\r'); break;
                case 'n': sb.push_back('\n'); break;
                default: return false;
                }
            } else {
                sb.push_back(c);
            }
            if (!try_get_next_char(c)) return false;
        }
        std::size_t length = sb.size();
        if (quote_char == 0)
            while (length > 0 && is_white_space(sb[length - 1])) --length;
        token_string = sb.substr(0, length);
        token = Token::String;
        return true;
    }

    std::string_view input_;
    std::size_t index_ = 0;
};

void append_quoted(std::string& out, const std::string& s) {
    const bool needs_quoting = starts_with_white_space(s) || ends_with_white_space(s) || s.find_first_of("\"'") != std::string::npos;
    if (needs_quoting) out.push_back('"');
    for (const char c : s) {
        switch (c) {
        case '\\': case ',': case '=': case '\'': case '"': out.push_back('\\'); break;
        case '\t': out += "\\t"; continue;
        case '\r': out += "\\r"; continue;
        case '\n': out += "\\n"; continue;
        default: break;
        }
        out.push_back(c);
    }
    if (needs_quoting) out.push_back('"');
}

std::string compute_display_name(const AssemblyNameParser::Parts& p) {
    std::string out;
    append_quoted(out, p.name);
    for (std::size_t i = 0; i < p.version.size(); ++i) out += (i == 0 ? ", Version=" : ".") + std::to_string(p.version[i]);
    if (p.culture_name) {
        out += ", Culture=";
        append_quoted(out, p.culture_name->empty() ? std::string("neutral") : *p.culture_name);
    }
    if (p.public_key_or_token) {
        const bool is_public_key = (p.flags & static_cast<std::uint32_t>(AssemblyNameFlags::PublicKey)) != 0;
        if (!is_public_key && p.public_key_or_token->size() > 8) throw UsageError("AssemblyNameInfo: a public key token longer than 8 bytes");
        out += is_public_key ? ", PublicKey=" : ", PublicKeyToken=";
        if (p.public_key_or_token->empty()) {
            out += "null";
        } else {
            static constexpr char kHex[] = "0123456789abcdef";
            for (const std::uint8_t b : *p.public_key_or_token) {
                out.push_back(kHex[b >> 4]);
                out.push_back(kHex[b & 15]);
            }
        }
    }
    if ((p.flags & 0xFFFFF10Fu & static_cast<std::uint32_t>(AssemblyNameFlags::Retargetable)) != 0) out += ", Retargetable=Yes";
    if (((p.flags >> 9) & 0x7) == 1) out += ", ContentType=WindowsRuntime";
    return out;
}

// ---- TypeNameParser -----------------------------------------------------------------------------------------------------

class TypeNameParser {
public:
    explicit TypeNameParser(std::string_view name) : input_(name) {}

    std::shared_ptr<const TypeName> parse_next_type_name(bool allow_fully_qualified_name, int& recursive_depth) {
        if (!try_dive(recursive_depth)) return nullptr;
        std::vector<int> nested_name_lengths;
        int full_type_name_length = 0;
        if (!try_get_type_name_info(input_, nested_name_lengths, recursive_depth, full_type_name_length)) return nullptr;
        const std::string_view full_type_name = input_.substr(0, static_cast<std::size_t>(full_type_name_length));
        input_.remove_prefix(static_cast<std::size_t>(full_type_name_length));

        std::vector<std::shared_ptr<const TypeName>> generic_args;
        std::string_view captured_before_processing = input_;
        bool double_brackets = false;
        if (is_beginning_of_generic_args(input_, double_brackets)) {
            for (;;) {
                std::shared_ptr<const TypeName> generic_arg = parse_next_type_name(double_brackets, recursive_depth);
                if (generic_arg == nullptr) return nullptr;
                if (double_brackets && !try_strip_first_char_and_trailing_spaces(input_, ']')) return nullptr;
                generic_args.push_back(std::move(generic_arg));
                if (try_strip_first_char_and_trailing_spaces(input_, ',')) {
                    double_brackets = try_strip_first_char_and_trailing_spaces(input_, '[');
                    continue;
                }
                if (!try_strip_first_char_and_trailing_spaces(input_, ']')) return nullptr;
                break;
            }
        }
        if (generic_args.empty()) {
            input_ = captured_before_processing;   // [] or [*] or [,] are array decorators, not generic arguments
        } else if (!try_dive(recursive_depth)) {
            return nullptr;
        }

        int previous_decorator = 0;
        captured_before_processing = input_;
        int parsed_decorator = 0;
        while (try_parse_next_decorator(input_, parsed_decorator)) {
            if (!try_dive(recursive_depth)) return nullptr;
            previous_decorator = parsed_decorator;
        }

        std::optional<AssemblyNameInfo> assembly_name;
        if (allow_fully_qualified_name && !try_parse_assembly_name(assembly_name)) {
            if (trim_start(input_).rfind(',', 0) != 0) throw InvalidAssemblyNameError("The given assembly name was invalid.");
            return nullptr;
        }

        const std::string full_name(full_type_name);
        std::shared_ptr<const TypeName> declaring_type = get_declaring_type(full_name, nested_name_lengths, assembly_name);
        std::shared_ptr<const TypeName> result = std::make_shared<TypeName>(full_name, assembly_name, nullptr, declaring_type,
                                                                            std::vector<std::shared_ptr<const TypeName>>{}, 0, -1);
        if (!generic_args.empty())
            result = std::make_shared<TypeName>(std::nullopt, assembly_name, result, declaring_type, std::move(generic_args), 0, -1);
        if (previous_decorator != 0) {
            int parsed_modifier = 0;
            while (try_parse_next_decorator(captured_before_processing, parsed_modifier))
                result = std::make_shared<TypeName>(std::nullopt, assembly_name, result, nullptr, std::vector<std::shared_ptr<const TypeName>>{}, parsed_modifier, -1);
        }
        return result;
    }

    std::string_view input_;

private:
    bool try_parse_assembly_name(std::optional<AssemblyNameInfo>& assembly_name) {
        const std::string_view captured_before_processing = input_;
        if (try_strip_first_char_and_trailing_spaces(input_, ',')) {
            if (input_.empty()) {
                input_ = captured_before_processing;   // restore the state
                return false;
            }
            const std::string_view candidate = get_assembly_name_candidate(input_);
            assembly_name = AssemblyNameInfo::try_parse(candidate);
            if (!assembly_name) return false;
            input_.remove_prefix(candidate.size());
        }
        return true;
    }

    static std::shared_ptr<const TypeName> get_declaring_type(const std::string& full_type_name, const std::vector<int>& nested_name_lengths,
                                                              const std::optional<AssemblyNameInfo>& assembly_name) {
        std::shared_ptr<const TypeName> declaring_type;
        int name_offset = 0;
        for (const int nested_name_length : nested_name_lengths) {
            const int full_name_length = name_offset + nested_name_length;
            declaring_type = std::make_shared<TypeName>(full_type_name, assembly_name, nullptr, declaring_type,
                                                        std::vector<std::shared_ptr<const TypeName>>{}, 0, full_name_length);
            name_offset += nested_name_length + 1;   // the '+' that was skipped
        }
        return declaring_type;
    }
};

}  // namespace

// ---- AssemblyNameInfo ---------------------------------------------------------------------------------------------------

std::optional<AssemblyNameInfo> AssemblyNameInfo::try_parse(std::string_view assembly_name) {
    AssemblyNameParser::Parts parts;
    if (assembly_name.empty() || !AssemblyNameParser(assembly_name).try_parse(parts)) return std::nullopt;
    AssemblyNameInfo info;
    info.full_name_ = compute_display_name(parts);
    info.name_ = std::move(parts.name);
    info.version_ = std::move(parts.version);
    info.culture_name_ = std::move(parts.culture_name);
    info.flags_ = parts.flags;
    info.public_key_or_token_ = std::move(parts.public_key_or_token);
    return info;
}

// ---- TypeName -----------------------------------------------------------------------------------------------------------

TypeName::TypeName(std::optional<std::string> full_name, std::optional<AssemblyNameInfo> assembly_name, std::shared_ptr<const TypeName> element_or_generic_type,
                   std::shared_ptr<const TypeName> declaring_type, std::vector<std::shared_ptr<const TypeName>> generic_arguments, int rank_or_modifier,
                   int nested_name_length)
    : rank_or_modifier_(rank_or_modifier), element_or_generic_type_(std::move(element_or_generic_type)), declaring_type_(std::move(declaring_type)),
      generic_arguments_(std::move(generic_arguments)), assembly_name_(std::move(assembly_name)) {
    if (full_name) {
        full_name_ = nested_name_length > 0 && static_cast<int>(full_name->size()) > nested_name_length
                         ? full_name->substr(0, static_cast<std::size_t>(nested_name_length))
                         : *full_name;
    } else if (is_constructed_generic_type()) {
        full_name_ = element_or_generic_type_->full_name() + "[";
        for (const auto& a : generic_arguments_) full_name_ += "[" + a->assembly_qualified_name() + "],";
        full_name_.back() = ']';
    } else {
        full_name_ = element_or_generic_type_->full_name() + rank_or_modifier_text(rank_or_modifier_);
    }
    if (is_constructed_generic_type()) name_ = std::string(get_name(element_or_generic_type_->full_name()));
    else if (is_pointer() || is_by_ref() || is_array()) name_ = element_or_generic_type_->name() + rank_or_modifier_text(rank_or_modifier_);
    else name_ = std::string(get_name(full_name_));
    assembly_qualified_name_ = assembly_name_ ? full_name_ + ", " + assembly_name_->full_name() : full_name_;
}

std::shared_ptr<const TypeName> TypeName::try_parse(std::string_view type_name) {
    const std::string_view trimmed = trim_start(type_name);   // white space at the beginning is always OK
    if (trimmed.empty()) return nullptr;
    int recursive_depth = 0;
    TypeNameParser parser(trimmed);
    std::shared_ptr<const TypeName> parsed = parser.parse_next_type_name(true, recursive_depth);
    if (parsed == nullptr || !parser.input_.empty()) return nullptr;   // unconsumed input is an error
    return parsed;
}

const TypeName& TypeName::declaring_type() const {
    if (declaring_type_ == nullptr) throw UsageError("This operation is only valid on nested types.");
    return *declaring_type_;
}

const TypeName& TypeName::get_element_type() const {
    if (!(is_array() || is_pointer() || is_by_ref())) throw UsageError("This operation is only valid on arrays, pointers and references.");
    return *element_or_generic_type_;
}

const TypeName& TypeName::get_generic_type_definition() const {
    if (!is_constructed_generic_type()) throw UsageError("This operation is only valid on generic types.");
    return *element_or_generic_type_;
}

int TypeName::get_array_rank() const {
    if (!is_array()) throw UsageError("Must be an array type.");
    return rank_or_modifier_ == kSZArray ? 1 : rank_or_modifier_;
}

int TypeName::get_node_count() const {
    int result = 1;
    if (is_array() || is_pointer() || is_by_ref()) {
        result += get_element_type().get_node_count();
    } else if (is_constructed_generic_type()) {
        result += get_generic_type_definition().get_node_count();
        for (const auto& a : generic_arguments_) result += a->get_node_count();
    } else if (is_nested()) {
        result += declaring_type().get_node_count();
    }
    return result;
}

}  // namespace cb::dotnet::metadata
