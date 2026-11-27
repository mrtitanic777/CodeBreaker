// dotnet/decompiler/csharp/syntax/text_location.hpp -- a line and column in the written C# text (both 1-based; (0, 0) is "no location").
//
//     TextLocation start(3, 5);
//     bool inside = start <= where && where < end;       // how AstNode::contains compares
//     std::string text = start.to_string();              // "(Line 3, Col 5)"
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: TextLocation, from TextLocation.cs), Copyright AlphaSierraPapa for the ILSpy
// team, MIT (see NOTICE in dotnet/decompiler). Not ported: TextLocationConverter (a .NET TypeConverter for designers).
#pragma once

#include <compare>
#include <cstddef>
#include <string>

namespace cb::dotnet::decompiler::csharp::syntax {

class TextLocation {
public:
    static constexpr int min_line = 1;
    static constexpr int min_column = 1;

    // The location of nothing: (0, 0).
    static const TextLocation empty;

    constexpr TextLocation() = default;
    constexpr TextLocation(int line_number, int column_number) : line_(line_number), column_(column_number) {}

    constexpr int line() const { return line_; }
    constexpr int column() const { return column_; }

    // Upstream's test, kept exactly (it mixes up the two minimums, which are both 1): column < 1 && line < 1.
    constexpr bool is_empty() const { return column_ < min_line && line_ < min_column; }

    // "(Line 3, Col 5)".
    std::string to_string() const;

    // Equal when line and column are; ordered by line, then column.
    friend constexpr bool operator==(const TextLocation& a, const TextLocation& b) { return a.line_ == b.line_ && a.column_ == b.column_; }
    friend constexpr std::strong_ordering operator<=>(const TextLocation& a, const TextLocation& b) {
        if (a.line_ != b.line_) return a.line_ <=> b.line_;
        return a.column_ <=> b.column_;
    }

    // Upstream's GetHashCode: unchecked(191 * column ^ line).
    std::size_t hash_code() const;

private:
    int line_ = 0;
    int column_ = 0;
};

inline const TextLocation TextLocation::empty{0, 0};

}  // namespace cb::dotnet::decompiler::csharp::syntax
