// dotnet/decompiler/util/file_utility.hpp -- file names as text: normalized (slashes unified, "." and ".." evaluated), compared, tested for a base
// directory, made relative to a directory, shortened for display.
//
//     normalize_path("a/./b/../c")  == "a/c";      get_relative_path("C:\\proj", "C:\\proj\\lib\\x.dll") == "lib\\x.dll";
//     is_equal_file_name("A\\b", "a/B");           // true: normalized and compared ignoring case
//
// Ported from ILSpy (ICSharpCode.Decompiler.Util: FileUtility, from FileUtility.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see
// NOTICE in dotnet/decompiler). Names are UTF-8 text; upstream's `string?` is std::optional<std::string> where a null is told from "" (the
// functions that take or return one).
#pragma once

#include <optional>
#include <string>
#include <string_view>

namespace cb::dotnet::decompiler::util::file_utility {

// The normalized version of a file name: slashes are replaced with backslashes (or kept as slashes for a Unix or web path), "." and ".."
// are evaluated. The empty name stays empty (a null stays null); a name that normalizes to nothing is ".".
std::optional<std::string> normalize_path(const std::optional<std::string>& file_name);
std::string normalize_path(std::string_view file_name);

bool is_equal_file_name(const std::optional<std::string>& file_name1, const std::optional<std::string>& file_name2);

// Whether test_directory is in base_directory (or is it).
bool is_base_directory(const std::optional<std::string>& base_directory, const std::optional<std::string>& test_directory);

// The path that leads from base_directory_path to abs_path. With no base directory, abs_path itself.
std::string get_relative_path(const std::optional<std::string>& base_directory_path, std::string_view abs_path);

// A path of at most max_chars characters, with an ellipsis where the middle was cut.
std::optional<std::string> trim_path(const std::optional<std::string>& path, int max_chars);

}  // namespace cb::dotnet::decompiler::util::file_utility
