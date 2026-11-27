// dotnet/decompiler/csharp/syntax/text_location.cpp -- see text_location.hpp.
#include "dotnet/decompiler/csharp/syntax/text_location.hpp"

#include <format>
#include <functional>

namespace cb::dotnet::decompiler::csharp::syntax {

std::string TextLocation::to_string() const { return std::format("(Line {}, Col {})", line_, column_); }

std::size_t TextLocation::hash_code() const {
    return static_cast<std::size_t>(static_cast<unsigned>(191 * column_) ^ static_cast<unsigned>(line_));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
