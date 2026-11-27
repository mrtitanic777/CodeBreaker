// dotnet/decompiler/util/dotnet_path.hpp -- file names as text, the way System.IO.Path treats them (on Windows: both '\' and '/' separate, a
// drive letter and a UNC share are roots): combine, the file name, the directory, the extension. What the project writers do with the names they
// write into a project.
//
//     path::combine("out", "Properties") == "out\\Properties";   path::get_file_name("a/b.txt") == "b.txt";
//     path::change_extension("A.resources", ".resx") == "A.resx";  path::get_directory_name("a/b/c") == "a\\b";
//
// Ported from ILSpy's use of System.IO.Path (WholeProjectDecompiler, the project file writers, FileUtility), Copyright AlphaSierraPapa for the
// ILSpy team, MIT (see NOTICE in dotnet/decompiler). The behaviour is .NET's on Windows; on another system only the separator changes ('/' is
// the only one, and a colon is no volume separator). Names are UTF-8; null strings (upstream's `string?`) are std::optional.
#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <string_view>

namespace cb::dotnet::decompiler::util::path {

#ifdef _WIN32
inline constexpr char kDirectorySeparatorChar = '\\';
#else
inline constexpr char kDirectorySeparatorChar = '/';
#endif
inline constexpr char kAltDirectorySeparatorChar = '/';

bool is_directory_separator(char c);
// Path.IsPathRooted
bool is_path_rooted(std::string_view path);
// Path.Combine of two parts (a rooted second part wins; an empty part is skipped).
std::string combine(std::string_view path1, std::string_view path2);
std::string combine(std::string_view path1, std::string_view path2, std::string_view path3);
// Path.GetFileName: after the last separator.
std::string get_file_name(std::string_view path);
// Path.GetExtension: from the last '.' of the file name, "" for none (or a '.' that ends the name).
std::string get_extension(std::string_view path);
// Path.GetDirectoryName: separators normalized; nullopt for an empty path or a root.
std::optional<std::string> get_directory_name(std::string_view path);
// Path.ChangeExtension: nullopt removes the extension; an extension without its '.' gets one.
std::string change_extension(std::string_view path, std::optional<std::string_view> extension);
// Path.GetFileNameWithoutExtension
std::string get_file_name_without_extension(std::string_view path);

// A file system path as the text .NET holds a file name in: UTF-8, with the system's separators ('\' on Windows).
std::string native_text(const std::filesystem::path& path);
// The file system path of a file name as text (UTF-8).
std::filesystem::path from_text(std::string_view file_name);

}  // namespace cb::dotnet::decompiler::util::path
