// dotnet/decompiler/disassembler/disassembler_helpers.cpp -- DisassemblerHelpers.
#include "dotnet/decompiler/disassembler/disassembler_helpers.hpp"

#include <bit>
#include <cmath>
#include <format>

#include "dotnet/decompiler/il/instruction_output_extensions.hpp"
#include "dotnet/decompiler/metadata/il_opcodes.hpp"
#include "dotnet/decompiler/util/char_info.hpp"
#include "dotnet/decompiler/util/number_format.hpp"

namespace cb::dotnet::decompiler::disassembler {
namespace {

bool is_valid_identifier_character(char16_t c) {
    return util::is_letter_or_digit(c) || c == u'_' || c == u'$' || c == u'@' || c == u'?' || c == u'`' || c == u'.';
}

bool is_valid_identifier(std::string_view identifier) {
    if (identifier.empty()) return false;
    const std::u16string id = util::utf16(identifier);
    if (util::is_digit(id[0])) return false;
    // as a special case, .ctor and .cctor are valid despite starting with a dot
    if (id[0] == u'.') return identifier == ".ctor" || identifier == ".cctor";
    if (identifier.find("..") != std::string_view::npos) return false;
    if (metadata::il_keywords().contains(std::string(identifier))) return false;
    for (const char16_t c : id)
        if (!is_valid_identifier_character(c)) return false;
    return true;
}

// The bytes of a float or double, in upper-case hex, as BitConverter.GetBytes gives them (little-endian).
template <class T>
void write_bytes(ITextOutput& writer, T value) {
    using U = std::conditional_t<sizeof(T) == 4, std::uint32_t, std::uint64_t>;
    const U bits = std::bit_cast<U>(value);
    writer.write('(');
    for (std::size_t i = 0; i < sizeof(T); ++i) {
        if (i > 0) writer.write(' ');
        writer.write(std::string_view(std::format("{:02X}", static_cast<unsigned>(bits >> (8 * i) & 0xFF))));
    }
    writer.write(')');
}

}  // namespace

std::string offset_to_string(long long offset) {
    // String.Format("IL_{0:x4}"): a negative number is its two's complement (eight digits for an int)
    if (offset < 0) return std::format("IL_{:x}", static_cast<std::uint32_t>(offset));
    return std::format("IL_{:04x}", offset);
}

void write_offset_reference(ITextOutput& writer, std::optional<int> offset) {
    if (!offset) writer.write("null");
    else writer.write_local_reference(offset_to_string(*offset), *offset);
}

void write_to(const srm::ExceptionRegion& h, const metadata::MetadataFile& module, const metadata::MetadataGenericContext& context, ITextOutput& writer) {
    writer.write(".try ");
    write_offset_reference(writer, h.try_offset);
    writer.write('-');
    write_offset_reference(writer, h.try_offset + h.try_length);
    writer.write(' ');
    switch (h.kind) {
    case srm::ExceptionRegionKind::Catch: writer.write("catch"); break;
    case srm::ExceptionRegionKind::Filter: writer.write("filter"); break;
    case srm::ExceptionRegionKind::Finally: writer.write("finally"); break;
    case srm::ExceptionRegionKind::Fault: writer.write("fault"); break;
    default: writer.write(std::to_string(static_cast<int>(h.kind))); break;
    }
    if (h.filter_offset != -1) {
        writer.write(' ');
        write_offset_reference(writer, h.filter_offset);
        writer.write(" handler ");
    }
    if (!h.catch_type.is_nil()) {
        writer.write(' ');
        il::write_to(h.catch_type, module, writer, context);
    }
    writer.write(' ');
    write_offset_reference(writer, h.handler_offset);
    writer.write('-');
    write_offset_reference(writer, h.handler_offset + h.handler_length);
}

std::string escape(std::string_view identifier) {
    if (is_valid_identifier(identifier)) return std::string(identifier);
    // The standard says a ' inside a quoted name takes an octal escape; as ILDasm, \' is written.
    const std::string s = escape_string(identifier);
    std::string out = "'";
    for (const char c : s) {
        if (c == '\'') out += "\\'";
        else out += c;
    }
    return out + "'";
}

std::string escape_string(std::string_view utf8) { return escape_string(util::utf16(utf8)); }

std::string escape_string(std::u16string_view str) {
    std::u16string b;
    std::string out;
    auto flush = [&] {
        out += util::utf8(b);
        b.clear();
    };
    for (const char16_t ch : str) {
        switch (ch) {
        case u'"': flush(); out += "\\\""; break;
        case u'\\': flush(); out += "\\\\"; break;
        case u'\0': flush(); out += "\\0"; break;
        case u'\a': flush(); out += "\\a"; break;
        case u'\b': flush(); out += "\\b"; break;
        case u'\f': flush(); out += "\\f"; break;
        case u'\n': flush(); out += "\\n"; break;
        case u'\r': flush(); out += "\\r"; break;
        case u'\t': flush(); out += "\\t"; break;
        case u'\v': flush(); out += "\\v"; break;
        default:
            // control characters and uncommon white space as numbers
            if (util::is_control(ch) || util::is_surrogate(ch) || (util::is_white_space(ch) && ch != u' ')) {
                flush();
                out += std::format("\\u{:04x}", static_cast<unsigned>(ch));
            } else {
                b.push_back(ch);
            }
            break;
        }
    }
    flush();
    return out;
}

void write_parameter_reference(ITextOutput& writer, const srm::MetadataReader& metadata, srm::MethodDefinitionHandle handle, int index) {
    auto parameter_name = [&](int number) -> std::optional<std::string> {
        const srm::MethodDefinition method = metadata.get_method_definition(handle);
        if ((method.attributes() & 0x0010) != 0) ++number;   // static
        for (const srm::ParameterHandle p : method.get_parameters()) {
            const srm::Parameter param = metadata.get_parameter(p);
            if (param.sequence_number() < number) continue;
            if (param.sequence_number() == number) {
                if (param.name().is_nil()) return std::nullopt;
                return escape(metadata.get_string(param.name()));
            }
            break;
        }
        return std::nullopt;
    };
    const std::optional<std::string> name = parameter_name(index);
    writer.write_local_reference(name ? *name : std::to_string(index), std::string("param_") + std::to_string(index));
}

void write_variable_reference(ITextOutput& writer, const srm::MetadataReader&, srm::MethodDefinitionHandle, int index) {
    writer.write_local_reference(std::to_string(index), std::string("loc_") + std::to_string(index));
}

void write_operand(ITextOutput& writer, const srm::ConstantValue& operand) {
    std::visit(
        [&](const auto& v) {
            using V = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<V, std::monostate>) throw UsageError("write_operand: a null operand");
            else if constexpr (std::is_same_v<V, std::u16string>) write_operand(writer, std::u16string_view(v));
            else if constexpr (std::is_same_v<V, char16_t>) writer.write(std::to_string(static_cast<int>(v)));
            else if constexpr (std::is_same_v<V, float> || std::is_same_v<V, double>) write_operand(writer, v);
            else if constexpr (std::is_same_v<V, bool>) writer.write(v ? "true" : "false");
            else writer.write(std::to_string(v));
        },
        operand);
}

void write_operand(ITextOutput& writer, long long value) { writer.write(std::to_string(value)); }

void write_operand(ITextOutput& writer, float value) {
    if (value == 0) {
        if (std::signbit(value)) writer.write('-');   // negative zero is a special case
        writer.write("0.0");
    } else if (std::isinf(value) || std::isnan(value)) {
        write_bytes(writer, value);
    } else {
        writer.write(util::format_r(value));
    }
}

void write_operand(ITextOutput& writer, double value) {
    if (value == 0) {
        if (std::signbit(value)) writer.write('-');
        writer.write("0.0");
    } else if (std::isinf(value) || std::isnan(value)) {
        write_bytes(writer, value);
    } else {
        writer.write(util::format_r(value));
    }
}

void write_operand(ITextOutput& writer, std::u16string_view operand) {
    writer.write('"');
    writer.write(escape_string(operand));
    writer.write('"');
}

std::optional<std::string> primitive_type_name(std::string_view full_name) {
    if (full_name == "System.SByte") return "int8";
    if (full_name == "System.Int16") return "int16";
    if (full_name == "System.Int32") return "int32";
    if (full_name == "System.Int64") return "int64";
    if (full_name == "System.Byte") return "uint8";
    if (full_name == "System.UInt16") return "uint16";
    if (full_name == "System.UInt32") return "uint32";
    if (full_name == "System.UInt64") return "uint64";
    if (full_name == "System.Single") return "float32";
    if (full_name == "System.Double") return "float64";
    if (full_name == "System.Void") return "void";
    if (full_name == "System.Boolean") return "bool";
    if (full_name == "System.String") return "string";
    if (full_name == "System.Char") return "char";
    if (full_name == "System.Object") return "object";
    if (full_name == "System.IntPtr") return "native int";
    return std::nullopt;
}

}  // namespace cb::dotnet::decompiler::disassembler
