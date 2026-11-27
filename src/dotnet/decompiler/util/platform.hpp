// dotnet/decompiler/util/platform.hpp -- what depends on the operating system: how file names compare. On Windows two file names are equal when
// they differ only in case (StringComparer.OrdinalIgnoreCase); on other systems they are compared as they are (StringComparer.Ordinal).
//
//     std::unordered_set<std::string, FileNameHash, FileNameEqual> directories;     // a HashSet<string>(Platform.FileNameComparer)
//     std::sort(names.begin(), names.end(), FileNameLess{});
//
// Ported from ILSpy (ICSharpCode.Decompiler.Util: Platform, from Platform.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in
// dotnet/decompiler). Names are UTF-8. Case folding is the invariant upper case of each UTF-16 character, as OrdinalIgnoreCase does; here done
// for the letters of the Basic Latin, Latin-1 Supplement, Greek and Cyrillic blocks (what a file name of a .NET assembly holds in practice), the
// rest compare as they are.
#pragma once

#include <cstddef>
#include <functional>
#include <string>
#include <string_view>

namespace cb::dotnet::decompiler::util {

// true when file names compare ignoring case (Windows).
#ifdef _WIN32
inline constexpr bool kFileNamesIgnoreCase = true;
#else
inline constexpr bool kFileNamesIgnoreCase = false;
#endif

// OrdinalIgnoreCase: -1, 0 or 1; compares the invariant upper-case forms of the characters, as code points.
int compare_ordinal_ignore_case(std::string_view a, std::string_view b);
// A hash that agrees with compare_ordinal_ignore_case.
std::size_t hash_ordinal_ignore_case(std::string_view s);

// Platform.FileNameComparer as the three things a C++ container asks for.
struct FileNameEqual {
    bool operator()(std::string_view a, std::string_view b) const {
        return kFileNamesIgnoreCase ? compare_ordinal_ignore_case(a, b) == 0 : a == b;
    }
};
struct FileNameLess {
    bool operator()(std::string_view a, std::string_view b) const {
        return kFileNamesIgnoreCase ? compare_ordinal_ignore_case(a, b) < 0 : a < b;
    }
};
struct FileNameHash {
    std::size_t operator()(std::string_view s) const { return kFileNamesIgnoreCase ? hash_ordinal_ignore_case(s) : std::hash<std::string_view>()(s); }
};
// The same for a comparison that is always OrdinalIgnoreCase (StringComparer.OrdinalIgnoreCase, as GroupBy and OrderBy ask for it in
// WholeProjectDecompiler and the project file writers).
struct OrdinalIgnoreCaseEqual {
    bool operator()(std::string_view a, std::string_view b) const { return compare_ordinal_ignore_case(a, b) == 0; }
};
struct OrdinalIgnoreCaseLess {
    bool operator()(std::string_view a, std::string_view b) const { return compare_ordinal_ignore_case(a, b) < 0; }
};
struct OrdinalIgnoreCaseHash {
    std::size_t operator()(std::string_view s) const { return hash_ordinal_ignore_case(s); }
};

}  // namespace cb::dotnet::decompiler::util
