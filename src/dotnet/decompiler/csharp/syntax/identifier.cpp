// dotnet/decompiler/csharp/syntax/identifier.cpp -- see identifier.hpp.
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

int utf16_length(const std::string& utf8) {
    int units = 0;
    for (unsigned char c : utf8) {
        if ((c & 0xC0) == 0x80) continue;  // a continuation byte belongs to the code point counted at its lead byte
        units += c >= 0xF0 ? 2 : 1;        // a four-byte sequence is a surrogate pair
    }
    return units;
}

TextLocation Identifier::end_location() const {
    TextLocation start = token_start_;
    return TextLocation(start.line(), start.column() + utf16_length(name_) + (is_verbatim_ ? 1 : 0));
}

Identifier* Identifier::create(const std::string& name_text, TextLocation location) {
    if (name_text.empty()) return make<Identifier>(std::string(), location);
    if (name_text[0] == '@') {
        auto* id = make<Identifier>(name_text.substr(1), TextLocation(location.line(), location.column() + 1));
        id->is_verbatim_ = true;
        return id;
    }
    return make<Identifier>(name_text, location);
}

Identifier* Identifier::create(const std::string& name_text, TextLocation location, bool is_verbatim) {
    if (name_text.empty()) return make<Identifier>(std::string(), location);
    auto* id = make<Identifier>(name_text, location);
    if (is_verbatim) id->is_verbatim_ = true;
    return id;
}

Identifier* Identifier::create_if_not_empty(const std::string& name_text) { return name_text.empty() ? nullptr : create(name_text); }

bool Identifier::do_match(AstNode* other, const pattern_matching::Match&) const {
    auto* identifier = as<Identifier>(other);
    return identifier != nullptr && match_string(name_, identifier->name_);
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
