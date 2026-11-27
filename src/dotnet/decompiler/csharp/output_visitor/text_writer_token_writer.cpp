// dotnet/decompiler/csharp/output_visitor/text_writer_token_writer.cpp -- TextWriterTokenWriter; see text_writer_token_writer.hpp.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.OutputVisitor: TextWriterTokenWriter), Copyright AlphaSierraPapa for the ILSpy team, MIT
// (see NOTICE in dotnet/decompiler).
#include "dotnet/decompiler/csharp/output_visitor/text_writer_token_writer.hpp"

#include <cmath>
#include <cstdint>
#include <format>
#include <type_traits>
#include <variant>

#include "dotnet/decompiler/csharp/output_visitor/csharp_output_visitor.hpp"
#include "dotnet/decompiler/csharp/output_visitor/insert_missing_tokens_decorator.hpp"
#include "dotnet/decompiler/csharp/output_visitor/insert_required_spaces_decorator.hpp"
#include "dotnet/decompiler/util/char_info.hpp"
#include "dotnet/decompiler/util/number_format.hpp"

namespace cb::dotnet::decompiler::csharp::output_visitor {

using syntax::CommentType;
using syntax::LiteralFormat;
using syntax::PreProcessorDirectiveType;
using syntax::TextLocation;
using util::UnicodeCategory;

namespace {

// .NET's string.Length of UTF-8 text.
int length_of(std::string_view utf8) { return syntax::utf16_length(std::string(utf8)); }

// Upstream's UpdateEndLocation: moves line and column across the text of a multi-line comment.
void update_end_location(std::string_view content, int& line, int& column) {
    if (content.empty()) return;
    for (std::size_t i = 0; i < content.size(); i++) {
        char ch = content[i];
        switch (ch) {
            case '\r':
                if (i + 1 < content.size() && content[i + 1] == '\n') i++;
                [[fallthrough]];
            case '\n':
                line++;
                column = 0;
                break;
            default:
                break;
        }
        column++;
    }
}

bool is_unprintable_category(UnicodeCategory category) {
    switch (category) {
        case UnicodeCategory::NonSpacingMark:
        case UnicodeCategory::SpacingCombiningMark:
        case UnicodeCategory::EnclosingMark:
        case UnicodeCategory::LineSeparator:
        case UnicodeCategory::ParagraphSeparator:
        case UnicodeCategory::Control:
        case UnicodeCategory::Format:
        case UnicodeCategory::Surrogate:
        case UnicodeCategory::PrivateUse:
        case UnicodeCategory::ConnectorPunctuation:
        case UnicodeCategory::ModifierSymbol:
        case UnicodeCategory::OtherNotAssigned:
        case UnicodeCategory::SpaceSeparator:
            return true;
        default:
            return false;
    }
}

std::string hex_lower(unsigned value, int width) { return std::format("{:0{}x}", value, width); }

// The escape of a character, or none when it is written as it is. Does not convert ' or ".
std::optional<std::u16string> convert_char(char16_t ch) {
    switch (ch) {
        case u'\\': return std::u16string(u"\\\\");
        case u'\0': return std::u16string(u"\\0");
        case u'\a': return std::u16string(u"\\a");
        case u'\b': return std::u16string(u"\\b");
        case u'\f': return std::u16string(u"\\f");
        case u'\n': return std::u16string(u"\\n");
        case u'\r': return std::u16string(u"\\r");
        case u'\t': return std::u16string(u"\\t");
        case u'\v': return std::u16string(u"\\v");
        case u' ':
        case u'_':
        case u'`':
        case u'^':
            // ASCII characters we allow directly in the output even though we don't use other Unicode characters of the same category.
            return std::nullopt;
        case u'�': {
            std::string hex = "\\u" + hex_lower(static_cast<unsigned>(ch), 4);
            return util::utf16(hex);
        }
        default:
            if (is_unprintable_category(util::get_unicode_category(ch))) {
                std::string hex = "\\u" + hex_lower(static_cast<unsigned>(ch), 4);
                return util::utf16(hex);
            }
            return std::nullopt;
    }
}

// The category of the character starting at identifier[index] (a surrogate pair is one character).
bool is_printable_identifier_char(std::u16string_view identifier, std::size_t index) {
    switch (identifier[index]) {
        case u'\\':
            return false;
        case u' ':
        case u'_':
        case u'`':
        case u'^':
            return true;
        default:
            break;
    }
    char16_t c = identifier[index];
    if (util::is_high_surrogate(c) && index + 1 < identifier.size() && util::is_low_surrogate(identifier[index + 1])) {
        char32_t cp = 0x10000 + ((static_cast<char32_t>(c) - 0xD800) << 10) + (static_cast<char32_t>(identifier[index + 1]) - 0xDC00);
        // The tables classify code units only: private use planes, the emoji skin tone modifiers and the tag characters are
        // the supplementary characters that are not printable.
        if (cp >= 0xF0000) return false;
        if (cp >= 0x1F3FB && cp <= 0x1F3FF) return false;
        if (cp >= 0xE0000 && cp <= 0xE0FFF) return false;
        return true;
    }
    return !is_unprintable_category(util::get_unicode_category(c));
}

bool is_surrogate_pair_at(std::u16string_view s, std::size_t i) {
    return i + 1 < s.size() && util::is_high_surrogate(s[i]) && util::is_low_surrogate(s[i + 1]);
}

}  // namespace

// ---- TokenWriter factories (ITokenWriter.cs) -------------------------------------------------------------------------------------

std::unique_ptr<TokenWriter> TokenWriter::create(TextWriter& writer, const std::string& indentation) {
    auto target = std::make_unique<TextWriterTokenWriter>(writer);
    target->set_indentation_string(indentation);
    TokenWriter& inner = *target;
    auto spaces = std::make_unique<InsertRequiredSpacesDecorator>(inner);
    spaces->own(std::move(target));
    return spaces;
}

std::unique_ptr<TokenWriter> TokenWriter::create_writer_that_sets_locations_in_ast(TextWriter& writer, const std::string& indentation) {
    auto target = std::make_unique<TextWriterTokenWriter>(writer);
    target->set_indentation_string(indentation);
    TextWriterTokenWriter& target_ref = *target;
    auto missing = std::make_unique<InsertMissingTokensDecorator>(target_ref, target_ref);
    missing->own(std::move(target));
    TokenWriter& missing_ref = *missing;
    auto spaces = std::make_unique<InsertRequiredSpacesDecorator>(missing_ref);
    spaces->own(std::move(missing));
    return spaces;
}

std::unique_ptr<TokenWriter> TokenWriter::insert_required_spaces(TokenWriter& writer) {
    return std::make_unique<InsertRequiredSpacesDecorator>(writer);
}

std::unique_ptr<TokenWriter> TokenWriter::wrap_in_writer_that_sets_locations_in_ast(TokenWriter& writer) {
    auto* locatable = dynamic_cast<ILocatable*>(&writer);
    if (locatable == nullptr) throw std::logic_error("writer does not provide locations!");
    return std::make_unique<InsertMissingTokensDecorator>(writer, *locatable);
}

// ---- TextWriterTokenWriter ----------------------------------------------------------------------------------------------------

TextLocation TextWriterTokenWriter::location() const {
    return TextLocation(line_, column_ + (needs_indent_ ? indentation_ * length_of(indentation_string_) : 0));
}

void TextWriterTokenWriter::write_identifier(syntax::Identifier& identifier) {
    write_indentation();
    if (identifier.is_verbatim() || CSharpOutputVisitor::is_keyword(identifier.name(), &identifier)) {
        text_writer_.write('@');
        column_++;
        length_++;
    }
    std::string name = escape_identifier(identifier.name());
    text_writer_.write(name);
    int name_length = length_of(name);
    column_ += name_length;
    length_ += name_length;
    is_at_start_of_line_ = false;
}

void TextWriterTokenWriter::write_keyword(std::string_view keyword) {
    write_indentation();
    int keyword_length = length_of(keyword);
    column_ += keyword_length;
    length_ += keyword_length;
    text_writer_.write(keyword);
    is_at_start_of_line_ = false;
}

void TextWriterTokenWriter::write_token(std::string_view token) {
    write_indentation();
    int token_length = length_of(token);
    column_ += token_length;
    length_ += token_length;
    text_writer_.write(token);
    is_at_start_of_line_ = false;
}

void TextWriterTokenWriter::space() {
    write_indentation();
    column_++;
    length_++;
    text_writer_.write(' ');
}

void TextWriterTokenWriter::write_indentation() {
    if (needs_indent_) {
        needs_indent_ = false;
        for (int i = 0; i < indentation_; i++) text_writer_.write(indentation_string_);
        int added = indentation_ * length_of(indentation_string_);
        column_ += added;
        length_ += added;
    }
}

void TextWriterTokenWriter::new_line() {
    text_writer_.write_line();
    column_ = 1;
    line_++;
    length_ += length_of(text_writer_.new_line());
    needs_indent_ = true;
    is_at_start_of_line_ = true;
}

void TextWriterTokenWriter::write_comment(CommentType comment_type, std::string_view content) {
    write_indentation();
    int content_length = length_of(content);
    switch (comment_type) {
        case CommentType::SingleLine:
            text_writer_.write("//");
            text_writer_.write_line(content);
            length_ += 2 + content_length + length_of(text_writer_.new_line());
            column_ = 1;
            line_++;
            needs_indent_ = true;
            is_at_start_of_line_ = true;
            break;
        case CommentType::MultiLine:
            text_writer_.write("/*");
            text_writer_.write(content);
            text_writer_.write("*/");
            length_ += 4 + content_length;
            column_ += 2;
            update_end_location(content, line_, column_);
            column_ += 2;
            is_at_start_of_line_ = false;
            break;
        case CommentType::Documentation:
            text_writer_.write("///");
            text_writer_.write_line(content);
            length_ += 3 + content_length + length_of(text_writer_.new_line());
            column_ = 1;
            line_++;
            needs_indent_ = true;
            is_at_start_of_line_ = true;
            break;
        case CommentType::MultiLineDocumentation:
            text_writer_.write("/**");
            text_writer_.write(content);
            text_writer_.write("*/");
            length_ += 5 + content_length;
            column_ += 3;
            update_end_location(content, line_, column_);
            column_ += 2;
            is_at_start_of_line_ = false;
            break;
        default:
            text_writer_.write(content);
            column_ += content_length;
            length_ += content_length;
            break;
    }
}

std::string_view TextWriterTokenWriter::pre_processor_directive_name(PreProcessorDirectiveType type) {
    switch (type) {
        case PreProcessorDirectiveType::Invalid: return "invalid";
        case PreProcessorDirectiveType::Region: return "region";
        case PreProcessorDirectiveType::Endregion: return "endregion";
        case PreProcessorDirectiveType::If: return "if";
        case PreProcessorDirectiveType::Endif: return "endif";
        case PreProcessorDirectiveType::Elif: return "elif";
        case PreProcessorDirectiveType::Else: return "else";
        case PreProcessorDirectiveType::Define: return "define";
        case PreProcessorDirectiveType::Undef: return "undef";
        case PreProcessorDirectiveType::Error: return "error";
        case PreProcessorDirectiveType::Warning: return "warning";
        case PreProcessorDirectiveType::Pragma: return "pragma";
        case PreProcessorDirectiveType::Line: return "line";
    }
    return "";
}

void TextWriterTokenWriter::write_pre_processor_directive(PreProcessorDirectiveType type, std::string_view argument) {
    // pre-processor directive must start on its own line
    if (!is_at_start_of_line_) new_line();
    write_indentation();
    text_writer_.write('#');
    std::string_view directive = pre_processor_directive_name(type);
    text_writer_.write(directive);
    column_ += 1 + static_cast<int>(directive.size());
    length_ += 1 + static_cast<int>(directive.size());
    if (!argument.empty()) {
        text_writer_.write(' ');
        text_writer_.write(argument);
        int argument_length = length_of(argument);
        column_ += 1 + argument_length;
        length_ += 1 + argument_length;
    }
    new_line();
}

std::string TextWriterTokenWriter::print_primitive_value(const typesystem::ConstantValue& value) {
    StringTextWriter writer;
    TextWriterTokenWriter token_writer(writer);
    token_writer.write_primitive_value(value);
    return writer.take_text();
}

void TextWriterTokenWriter::write_primitive_value(const typesystem::ConstantValue& value, LiteralFormat format) {
    // A write of text that counts its length into the column and length.
    auto emit = [this](std::string_view text) {
        text_writer_.write(text);
        int n = length_of(text);
        column_ += n;
        length_ += n;
    };

    if (std::holds_alternative<std::monostate>(value)) {
        // usually NullReferenceExpression should be used for this, but we'll handle it anyways
        emit("null");
        return;
    }

    if (const bool* b = std::get_if<bool>(&value)) {
        emit(*b ? "true" : "false");
        return;
    }

    if (const std::u16string* s = std::get_if<std::u16string>(&value)) {
        std::string tmp = util::utf8(convert_string(*s));
        int n = length_of(tmp);
        column_ += n + 2;
        length_ += n + 2;
        text_writer_.write('"');
        text_writer_.write(tmp);
        text_writer_.write('"');
        if (format == LiteralFormat::Utf8Literal) {
            text_writer_.write("u8");
            column_ += 2;
            length_ += 2;
        }
    } else if (const char16_t* c = std::get_if<char16_t>(&value)) {
        std::string tmp = convert_char_literal(*c);
        int n = length_of(tmp);
        column_ += n + 2;
        length_ += n + 2;
        text_writer_.write('\'');
        text_writer_.write(tmp);
        text_writer_.write('\'');
    } else if (const util::Decimal* d = std::get_if<util::Decimal>(&value)) {
        emit(d->to_string() + "m");
    } else if (const float* fp = std::get_if<float>(&value)) {
        float f = *fp;
        if (std::isinf(f) || std::isnan(f)) {
            // Strictly speaking, these aren't PrimitiveExpressions; but we still support writing these to make life easier for code
            // generators.
            text_writer_.write("float");
            column_ += 5;
            length_ += 5;
            write_token(".");
            if (std::isinf(f) && f > 0) {
                emit("PositiveInfinity");
            } else if (std::isinf(f)) {
                emit("NegativeInfinity");
            } else {
                emit("NaN");
            }
            return;
        }
        std::string str = util::format_r(f) + "f";
        if (f == 0 && std::signbit(f) && str[0] != '-') {
            // negative zero is a special case
            str = "-" + str;
        }
        emit(str);
    } else if (const double* dp = std::get_if<double>(&value)) {
        double f = *dp;
        if (std::isinf(f) || std::isnan(f)) {
            text_writer_.write("double");
            column_ += 6;
            length_ += 6;
            write_token(".");
            if (std::isinf(f) && f > 0) {
                emit("PositiveInfinity");
            } else if (std::isinf(f)) {
                emit("NegativeInfinity");
            } else {
                emit("NaN");
            }
            return;
        }
        std::string number = util::format_r(f);
        if (f == 0 && std::signbit(f) && number[0] != '-') {
            number = "-" + number;
        }
        if (number.find('.') == std::string::npos && number.find('E') == std::string::npos) {
            number += ".0";
        }
        emit(number);
    } else {
        // IFormattable: the integers
        std::string b;
        bool is_unsigned_wide = std::holds_alternative<std::uint32_t>(value) || std::holds_alternative<std::uint64_t>(value);
        bool is_long = std::holds_alternative<std::int64_t>(value) || std::holds_alternative<std::uint64_t>(value);
        std::visit(
            [&](const auto& v) {
                using T = std::decay_t<decltype(v)>;
                if constexpr (std::is_integral_v<T> && !std::is_same_v<T, bool> && !std::is_same_v<T, char16_t>) {
                    if (format == LiteralFormat::HexadecimalNumber) {
                        // .NET formats a negative integer in hexadecimal as its two's complement of the type's width
                        b += "0x";
                        b += std::format("{:X}", static_cast<std::make_unsigned_t<T>>(v));
                    } else if constexpr (std::is_signed_v<T>) {
                        b += std::to_string(static_cast<std::int64_t>(v));
                    } else {
                        b += std::to_string(static_cast<std::uint64_t>(v));
                    }
                }
            },
            value);
        if (is_unsigned_wide) b += "u";
        if (is_long) b += "L";
        emit(b);
    }
}

void TextWriterTokenWriter::write_interpolated_text(std::string_view text) { text_writer_.write(convert_string(text)); }

std::string TextWriterTokenWriter::convert_char_literal(char16_t ch) {
    if (ch == u'\'') return "\\'";
    if (auto escaped = convert_char(ch)) return util::utf8(*escaped);
    return util::utf8(std::u16string(1, ch));
}

std::u16string TextWriterTokenWriter::convert_string(std::u16string_view str) {
    std::u16string sb;
    for (char16_t ch : str) {
        std::optional<std::u16string> s = ch == u'"' ? std::optional<std::u16string>(u"\\\"") : convert_char(ch);
        if (s)
            sb += *s;
        else
            sb += ch;
    }
    return sb;
}

std::string TextWriterTokenWriter::convert_string(std::string_view utf8_text) {
    return util::utf8(convert_string(util::utf16(utf8_text)));
}

std::string TextWriterTokenWriter::escape_identifier(std::string_view identifier_utf8) {
    if (identifier_utf8.empty()) return std::string(identifier_utf8);
    std::u16string identifier = util::utf16(identifier_utf8);
    std::u16string sb;
    for (std::size_t i = 0; i < identifier.size(); i++) {
        bool pair = is_surrogate_pair_at(identifier, i);
        if (is_printable_identifier_char(identifier, i)) {
            if (pair) {
                sb.append(identifier, i, 2);
                i++;
            } else {
                sb += identifier[i];
            }
        } else if (pair) {
            char32_t cp = 0x10000 + ((static_cast<char32_t>(identifier[i]) - 0xD800) << 10) + (static_cast<char32_t>(identifier[i + 1]) - 0xDC00);
            sb += util::utf16("\\U" + hex_lower(static_cast<unsigned>(cp), 8));
            i++;
        } else {
            sb += util::utf16("\\u" + hex_lower(static_cast<unsigned>(identifier[i]), 4));
        }
    }
    return util::utf8(sb);
}

bool TextWriterTokenWriter::contains_non_printable_identifier_char(std::string_view identifier_utf8) {
    if (identifier_utf8.empty()) return false;
    std::u16string identifier = util::utf16(identifier_utf8);
    for (std::size_t i = 0; i < identifier.size(); i++) {
        if (util::is_white_space(identifier[i])) return true;
        if (!is_printable_identifier_char(identifier, i)) return true;
    }
    return false;
}

void TextWriterTokenWriter::write_primitive_type(std::string_view type) {
    text_writer_.write(type);
    int n = length_of(type);
    column_ += n;
    length_ += n;
    if (type == "new") {
        text_writer_.write("()");
        column_ += 2;
        length_ += 2;
    }
}

void TextWriterTokenWriter::start_node(syntax::AstNode&) {
    // Write out the indentation, so that overrides of this method can rely use the current output length to identify the position of
    // the node in the output.
    write_indentation();
}

void TextWriterTokenWriter::end_node(syntax::AstNode&) {}

}  // namespace cb::dotnet::decompiler::csharp::output_visitor
