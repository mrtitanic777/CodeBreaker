// java/decompiler/const_exprent.cpp -- see exprents.hpp: ConstExprent and Java's text of numbers. Ported from Fernflower
// (de.fernflower.modules.decompiler.exps.ConstExprent), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0.
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include <charconv>
#include <cmath>
#include <cstring>
#include <limits>
#include <stdexcept>

#include "java/decompiler/code_constants.hpp"
#include "java/decompiler/collectors.hpp"
#include "java/decompiler/expr_processor.hpp"
#include "java/decompiler/exprents.hpp"
#include "java/decompiler/text_buffer.hpp"

namespace cb::java::decompiler {

namespace {

// Java's Double.toString / Float.toString layout from the shortest digits: [1e-3, 1e7) plain, else computerized scientific notation.
std::string java_layout(const char* sci_begin, const char* sci_end, bool negative) {
    // sci form: d[.ddd]e[+-]XX
    std::string digits;
    const char* p = sci_begin;
    for (; p != sci_end && *p != 'e'; ++p) {
        if (*p != '.') digits.push_back(*p);
    }
    const int exp10 = std::atoi(std::string(p + 1, sci_end).c_str());
    while (digits.size() > 1 && digits.back() == '0') digits.pop_back();
    std::string res = negative ? "-" : "";
    if (exp10 >= -3 && exp10 < 7) {
        if (exp10 >= 0) {
            const std::size_t int_len = static_cast<std::size_t>(exp10) + 1;
            std::string int_part = digits.substr(0, std::min(int_len, digits.size()));
            if (int_part.size() < int_len) int_part.append(int_len - int_part.size(), '0');
            std::string frac = digits.size() > int_len ? digits.substr(int_len) : std::string("0");
            res += int_part + "." + frac;
        } else {
            res += "0." + std::string(static_cast<std::size_t>(-exp10 - 1), '0') + digits;
        }
    } else {
        res += digits.substr(0, 1) + "." + (digits.size() > 1 ? digits.substr(1) : std::string("0")) + "E" + std::to_string(exp10);
    }
    return res;
}

// UTF-8 (CESU-8 surrogates included) to the UTF-16 code units Java's String holds.
std::vector<std::uint32_t> utf16_units(const std::string& s) {
    std::vector<std::uint32_t> out;
    std::size_t i = 0;
    while (i < s.size()) {
        const unsigned char c = static_cast<unsigned char>(s[i]);
        std::uint32_t cp;
        std::size_t n;
        if (c < 0x80) {
            cp = c;
            n = 1;
        } else if ((c >> 5) == 6 && i + 1 < s.size()) {
            cp = ((c & 0x1Fu) << 6) | (static_cast<unsigned char>(s[i + 1]) & 0x3Fu);
            n = 2;
        } else if ((c >> 4) == 14 && i + 2 < s.size()) {
            cp = ((c & 0x0Fu) << 12) | ((static_cast<unsigned char>(s[i + 1]) & 0x3Fu) << 6) | (static_cast<unsigned char>(s[i + 2]) & 0x3Fu);
            n = 3;
        } else if ((c >> 3) == 30 && i + 3 < s.size()) {
            cp = ((c & 0x07u) << 18) | ((static_cast<unsigned char>(s[i + 1]) & 0x3Fu) << 12) | ((static_cast<unsigned char>(s[i + 2]) & 0x3Fu) << 6) |
                 (static_cast<unsigned char>(s[i + 3]) & 0x3Fu);
            n = 4;
        } else {
            cp = c;
            n = 1;
        }
        i += n;
        if (cp >= 0x10000) {
            cp -= 0x10000;
            out.push_back(0xD800u + (cp >> 10));
            out.push_back(0xDC00u + (cp & 0x3FFu));
        } else {
            out.push_back(cp);
        }
    }
    return out;
}

void append_utf8(std::string& out, std::uint32_t c) {
    if (c < 0x80) {
        out.push_back(static_cast<char>(c));
    } else if (c < 0x800) {
        out.push_back(static_cast<char>(0xC0 | (c >> 6)));
        out.push_back(static_cast<char>(0x80 | (c & 0x3F)));
    } else {
        out.push_back(static_cast<char>(0xE0 | (c >> 12)));
        out.push_back(static_cast<char>(0x80 | ((c >> 6) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | (c & 0x3F)));
    }
}

// The text of a character as a Java literal's content: its escape, itself, or a unicode escape.
std::string escape_char(std::uint32_t c, bool ascii) {
    switch (c) {
        case 0x8: return "\\b";
        case 0x9: return "\\t";
        case 0xA: return "\\n";
        case 0xC: return "\\f";
        case 0xD: return "\\r";
        case 0x22: return "\\\"";
        case 0x27: return "\\\'";
        case 0x5C: return "\\\\";
        default: break;
    }
    if ((c >= 32 && c < 127) || (!ascii && is_printable_unicode(c))) {
        std::string s;
        append_utf8(s, c);
        return s;
    }
    return char_to_unicode_literal(static_cast<int>(c));
}

std::string convert_string_to_java(const std::string& value, bool ascii) {
    std::string buffer;
    buffer.reserve(value.size());
    for (const std::uint32_t c : utf16_units(value)) {
        if (c == 0x5C) buffer.append("\\\\");   // u005c: backslash
        else buffer.append(escape_char(c, ascii));
    }
    return buffer;
}

VarType guess_type(int val, bool bool_permitted) {
    if (bool_permitted) {
        VarType const_type = VarType::BOOLEAN;
        if (val != 0 && val != 1) const_type = const_type.copy(true);
        return const_type;
    }
    if (0 <= val && val <= 127) return VarType::BYTECHAR;
    if (-128 <= val && val <= 127) return VarType::BYTE;
    if (0 <= val && val <= 32767) return VarType::SHORTCHAR;
    if (-32768 <= val && val <= 32767) return VarType::SHORT;
    if (0 <= val && val <= 0xFFFF) return VarType::CHAR;
    return VarType::INT;
}

// Java's (int) of a double: saturating, NaN is 0.
int java_d2i(double d) {
    if (std::isnan(d)) return 0;
    if (d >= 2147483647.0) return std::numeric_limits<int>::max();
    if (d <= -2147483648.0) return std::numeric_limits<int>::min();
    return static_cast<int>(d);
}

std::uint64_t double_bits(double d) {
    if (std::isnan(d)) return 0x7ff8000000000000ULL;   // doubleToLongBits
    std::uint64_t b;
    std::memcpy(&b, &d, sizeof b);
    return b;
}

std::uint32_t float_bits(float f) {
    if (std::isnan(f)) return 0x7fc00000u;   // floatToIntBits
    std::uint32_t b;
    std::memcpy(&b, &f, sizeof b);
    return b;
}

// Java's equals of two boxed values.
bool value_equals(const ConstExprent::Value& a, const ConstExprent::Value& b) {
    if (a.index() != b.index()) return false;
    if (std::holds_alternative<float>(a)) return float_bits(std::get<float>(a)) == float_bits(std::get<float>(b));
    if (std::holds_alternative<double>(a)) return double_bits(std::get<double>(a)) == double_bits(std::get<double>(b));
    return a == b;
}

MatchValue to_match_value(const ConstExprent::Value& v) {
    return std::visit([](const auto& x) -> MatchValue { return x; }, v);
}

}  // namespace

std::string java_value_string(const ConstExprent::Value& v) {
    if (std::holds_alternative<std::monostate>(v)) return "null";
    if (std::holds_alternative<std::int32_t>(v)) return std::to_string(std::get<std::int32_t>(v));
    if (std::holds_alternative<std::int64_t>(v)) return std::to_string(std::get<std::int64_t>(v));
    if (std::holds_alternative<float>(v)) return java_float_string(std::get<float>(v));
    if (std::holds_alternative<double>(v)) return java_double_string(std::get<double>(v));
    return std::get<std::string>(v);
}

std::string java_double_string(double d) {
    if (std::isnan(d)) return "NaN";
    if (std::isinf(d)) return d < 0 ? "-Infinity" : "Infinity";
    const bool negative = std::signbit(d);
    if (d == 0.0) return negative ? "-0.0" : "0.0";
    char buf[64];
    const auto r = std::to_chars(buf, buf + sizeof buf, std::fabs(d), std::chars_format::scientific);
    return java_layout(buf, r.ptr, negative);
}

std::string java_float_string(float f) {
    if (std::isnan(f)) return "NaN";
    if (std::isinf(f)) return f < 0 ? "-Infinity" : "Infinity";
    const bool negative = std::signbit(f);
    if (f == 0.0f) return negative ? "-0.0" : "0.0";
    char buf[64];
    const auto r = std::to_chars(buf, buf + sizeof buf, std::fabs(f), std::chars_format::scientific);
    return java_layout(buf, r.ptr, negative);
}

// ---- ConstExprent

ConstExprent::ConstExprent(int val, bool bool_permitted, const Offsets& offsets)
    : ConstExprent(guess_type(val, bool_permitted), Value(static_cast<std::int32_t>(val)), bool_permitted, offsets) {}

ConstExprent::ConstExprent(VarType const_type, Value value, const Offsets& offsets) : ConstExprent(std::move(const_type), std::move(value), false, offsets) {}

ConstExprent::ConstExprent(VarType const_type, Value value, bool bool_permitted, const Offsets& offsets)
    : Exprent(EXPRENT_CONST), const_type_(std::move(const_type)), value_(std::move(value)), bool_permitted_(bool_permitted) {
    add_bytecode_offsets(offsets);
}

Exprent* ConstExprent::copy() const { return make<ConstExprent>(const_type_, value_, bytecode); }

TextBuffer ConstExprent::to_java(int, BytecodeMappingTracer& tracer) {
    const bool literal = DecompilerContext::option(pref::LITERALS_AS_IS);
    const bool ascii = DecompilerContext::option(pref::ASCII_STRING_CHARACTERS);

    tracer.add_mapping(bytecode);

    if (const_type_.type != cc::TYPE_NULL && std::holds_alternative<std::monostate>(value_)) {
        return TextBuffer(ExprProcessor::get_cast_type_name(const_type_));
    }
    switch (const_type_.type) {
        case cc::TYPE_BOOLEAN:
            return TextBuffer(std::get<std::int32_t>(value_) != 0 ? "true" : "false");

        case cc::TYPE_CHAR: {
            const std::int32_t val = std::get<std::int32_t>(value_);
            std::string ret;
            switch (val) {
                case 0x8: ret = "\\b"; break;
                case 0x9: ret = "\\t"; break;
                case 0xA: ret = "\\n"; break;
                case 0xC: ret = "\\f"; break;
                case 0xD: ret = "\\r"; break;
                case 0x22: ret = "\\\""; break;
                case 0x27: ret = "\\\'"; break;
                case 0x5C: ret = "\\\\"; break;
                default: {
                    const std::uint32_t c = static_cast<std::uint32_t>(val) & 0xFFFFu;
                    if ((c >= 32 && c < 127) || (!ascii && is_printable_unicode(c))) append_utf8(ret, c);
                    else ret = char_to_unicode_literal(static_cast<int>(c));
                }
            }
            return TextBuffer(ret).enclose("\'", "\'");
        }

        case cc::TYPE_BYTE:
        case cc::TYPE_BYTECHAR:
        case cc::TYPE_SHORT:
        case cc::TYPE_SHORTCHAR:
        case cc::TYPE_INT: {
            const std::int32_t ival = std::get<std::int32_t>(value_);
            std::string intfield;
            if (literal) return TextBuffer(std::to_string(ival));
            if (ival == std::numeric_limits<std::int32_t>::max()) intfield = "MAX_VALUE";
            else if (ival == std::numeric_limits<std::int32_t>::min()) intfield = "MIN_VALUE";
            else return TextBuffer(std::to_string(ival));
            return make<FieldExprent>(intfield, "java/lang/Integer", true, nullptr, FieldDescriptor::INTEGER_DESCRIPTOR, bytecode)->to_java(0, tracer);
        }

        case cc::TYPE_LONG: {
            const std::int64_t lval = std::get<std::int64_t>(value_);
            std::string longfield;
            if (literal) return TextBuffer(std::to_string(lval)).append("L");
            if (lval == std::numeric_limits<std::int64_t>::max()) longfield = "MAX_VALUE";
            else if (lval == std::numeric_limits<std::int64_t>::min()) longfield = "MIN_VALUE";
            else return TextBuffer(std::to_string(lval)).append("L");
            return make<FieldExprent>(longfield, "java/lang/Long", true, nullptr, FieldDescriptor::LONG_DESCRIPTOR, bytecode)->to_java(0, tracer);
        }

        case cc::TYPE_DOUBLE: {
            const double dval = std::get<double>(value_);
            std::string doublefield;
            if (literal) {
                if (std::isnan(dval)) return TextBuffer("0.0D / 0.0");
                if (dval == std::numeric_limits<double>::infinity()) return TextBuffer("1.0D / 0.0");
                if (dval == -std::numeric_limits<double>::infinity()) return TextBuffer("-1.0D / 0.0");
                return TextBuffer(java_double_string(dval)).append("D");
            }
            if (std::isnan(dval)) doublefield = "NaN";
            else if (dval == std::numeric_limits<double>::infinity()) doublefield = "POSITIVE_INFINITY";
            else if (dval == -std::numeric_limits<double>::infinity()) doublefield = "NEGATIVE_INFINITY";
            else if (dval == std::numeric_limits<double>::max()) doublefield = "MAX_VALUE";
            else if (dval == std::numeric_limits<double>::denorm_min()) doublefield = "MIN_VALUE";
            else return TextBuffer(java_double_string(dval)).append("D");
            return make<FieldExprent>(doublefield, "java/lang/Double", true, nullptr, FieldDescriptor::DOUBLE_DESCRIPTOR, bytecode)->to_java(0, tracer);
        }

        case cc::TYPE_FLOAT: {
            const float fval = std::get<float>(value_);
            std::string floatfield;
            if (literal) {
                if (std::isnan(fval)) return TextBuffer("0.0F / 0.0");
                if (fval == std::numeric_limits<float>::infinity()) return TextBuffer("1.0F / 0.0");
                if (fval == -std::numeric_limits<float>::infinity()) return TextBuffer("-1.0F / 0.0");
                return TextBuffer(java_float_string(fval)).append("F");
            }
            if (std::isnan(fval)) floatfield = "NaN";
            else if (fval == std::numeric_limits<float>::infinity()) floatfield = "POSITIVE_INFINITY";
            else if (fval == -std::numeric_limits<float>::infinity()) floatfield = "NEGATIVE_INFINITY";
            else if (fval == std::numeric_limits<float>::max()) floatfield = "MAX_VALUE";
            else if (fval == std::numeric_limits<float>::denorm_min()) floatfield = "MIN_VALUE";
            else return TextBuffer(java_float_string(fval)).append("F");
            return make<FieldExprent>(floatfield, "java/lang/Float", true, nullptr, FieldDescriptor::FLOAT_DESCRIPTOR, bytecode)->to_java(0, tracer);
        }

        case cc::TYPE_NULL:
            return TextBuffer("null");

        case cc::TYPE_OBJECT:
            if (const_type_ == VarType::STRING) {
                return TextBuffer(convert_string_to_java(java_value_string(value_), ascii)).enclose("\"", "\"");
            }
            if (const_type_ == VarType::CLASS) {
                const std::string strval = java_value_string(value_);
                // array of simple type, or a class
                const VarType classtype = strval.starts_with("[") ? VarType::parse(strval, false) : VarType::parse(strval, true);
                return TextBuffer(ExprProcessor::get_cast_type_name(classtype)).append(".class");
            }
            break;
        default:
            break;
    }

    throw std::runtime_error("invalid constant type");
}

bool ConstExprent::equals(const Exprent* o) const {
    if (o == this) return true;
    const ConstExprent* cn = dynamic_cast<const ConstExprent*>(o);
    if (cn == nullptr) return false;
    return const_type_ == cn->get_const_type() && value_equals(value_, cn->get_value());
}

bool ConstExprent::has_boolean_value() const {
    switch (const_type_.type) {
        case cc::TYPE_BOOLEAN:
        case cc::TYPE_CHAR:
        case cc::TYPE_BYTE:
        case cc::TYPE_BYTECHAR:
        case cc::TYPE_SHORT:
        case cc::TYPE_SHORTCHAR:
        case cc::TYPE_INT: {
            const std::int32_t ival = std::get<std::int32_t>(value_);
            return ival == 0 || (DecompilerContext::option(pref::BOOLEAN_TRUE_ONE) && ival == 1);
        }
        default:
            break;
    }
    return false;
}

bool ConstExprent::has_value_one() const {
    switch (const_type_.type) {
        case cc::TYPE_BOOLEAN:
        case cc::TYPE_CHAR:
        case cc::TYPE_BYTE:
        case cc::TYPE_BYTECHAR:
        case cc::TYPE_SHORT:
        case cc::TYPE_SHORTCHAR:
        case cc::TYPE_INT:
            return std::get<std::int32_t>(value_) == 1;
        case cc::TYPE_LONG:
            return static_cast<std::int32_t>(std::get<std::int64_t>(value_)) == 1;   // Long.intValue()
        case cc::TYPE_DOUBLE:
            return java_d2i(std::get<double>(value_)) == 1;
        case cc::TYPE_FLOAT:
            return java_d2i(static_cast<double>(std::get<float>(value_))) == 1;
        default:
            break;
    }
    return false;
}

ConstExprent* ConstExprent::get_zero_constant(int type_code) {
    switch (type_code) {
        case cc::TYPE_INT:
            return make<ConstExprent>(VarType::INT, Value(std::int32_t{0}), std::nullopt);
        case cc::TYPE_LONG:
            return make<ConstExprent>(VarType::LONG, Value(std::int64_t{0}), std::nullopt);
        case cc::TYPE_DOUBLE:
            return make<ConstExprent>(VarType::DOUBLE, Value(0.0), std::nullopt);
        case cc::TYPE_FLOAT:
            return make<ConstExprent>(VarType::FLOAT, Value(0.0f), std::nullopt);
        default:
            break;
    }
    throw std::runtime_error("Invalid argument!");
}

bool ConstExprent::match(const MatchNode* match_node, MatchEngine& engine) {
    if (!Exprent::match(match_node, engine)) return false;
    for (const auto& [key, rule_value] : match_node->get_rules()) {
        switch (key) {
            case MatchProperties::EXPRENT_CONSTTYPE:
                if (!equal_match_values(rule_value.value, MatchValue(const_type_))) return false;
                break;
            case MatchProperties::EXPRENT_CONSTVALUE:
                if (rule_value.is_variable()) {
                    if (!engine.check_and_set_variable_value(rule_value.to_string(), to_match_value(value_))) return false;
                }
                break;
            default:
                break;
        }
    }
    return true;
}

}  // namespace cb::java::decompiler
