// dotnet/decompiler/csharp/syntax/primitive_type.cpp -- see primitive_type.hpp.
#include "dotnet/decompiler/csharp/syntax/primitive_type.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

TextLocation PrimitiveType::end_location() const {
    TextLocation start = start_location();
    return TextLocation(start.line(), start.column() + static_cast<int>(keyword_.size()));
}

std::string PrimitiveType::to_string(const output_visitor::CSharpFormattingOptions*) const { return keyword_; }

typesystem::KnownTypeCode PrimitiveType::get_type_code_for_primitive_type(std::string_view keyword_text) {
    using typesystem::KnownTypeCode;
    if (keyword_text == "string") return KnownTypeCode::String;
    if (keyword_text == "int") return KnownTypeCode::Int32;
    if (keyword_text == "uint") return KnownTypeCode::UInt32;
    if (keyword_text == "object") return KnownTypeCode::Object;
    if (keyword_text == "bool") return KnownTypeCode::Boolean;
    if (keyword_text == "sbyte") return KnownTypeCode::SByte;
    if (keyword_text == "byte") return KnownTypeCode::Byte;
    if (keyword_text == "short") return KnownTypeCode::Int16;
    if (keyword_text == "ushort") return KnownTypeCode::UInt16;
    if (keyword_text == "long") return KnownTypeCode::Int64;
    if (keyword_text == "ulong") return KnownTypeCode::UInt64;
    if (keyword_text == "float") return KnownTypeCode::Single;
    if (keyword_text == "double") return KnownTypeCode::Double;
    if (keyword_text == "decimal") return KnownTypeCode::Decimal;
    if (keyword_text == "char") return KnownTypeCode::Char;
    if (keyword_text == "void") return KnownTypeCode::Void;
    return KnownTypeCode::None;
}

bool PrimitiveType::do_match(AstNode* other, const pattern_matching::Match&) const {
    auto* primitive_type = as<PrimitiveType>(other);
    return primitive_type != nullptr && match_string(keyword_, primitive_type->keyword_);
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
