// dotnet/decompiler/ilspycmd/validation_attributes.hpp -- the checks ilspycmd makes of its command line once it is read: what must exist,
// and which options go together. Each returns the message of what is wrong (the command line is then refused, exit code 1), or nothing.
//
//     if (auto message = files_exist({"a.dll", "b.dll"}, working_directory)) ...         // "File '...\b.dll' does not exist!"
//
// Ported from ILSpy (ICSharpCode.ILSpyCmd: ProjectOptionRequiresOutputDirectoryValidationAttribute, FileExistsOrNullAttribute and
// FilesExistAttribute, from ValidationAttributes.cs), Copyright (c) 2019 Christoph Wille, MIT (see NOTICE in dotnet/decompiler). The
// attributes are functions here: there is no attribute machinery in C++, and the order they run in (the whole program, then the
// arguments, then the options in the order they are declared) is kept by the program.
//
// The checks McMaster brings (FileExists, DirectoryExists, Required) are here too, with the messages McMaster gives them.
#pragma once

#include <optional>
#include <string>
#include <vector>

namespace cb::dotnet::decompiler::ilspycmd {

// "--project cannot be used unless --outputdir is also specified" when a project is asked for and no output directory is.
std::optional<std::string> project_option_requires_output_directory(bool create_compilable_project_flag, const std::optional<std::string>& output_directory);

// [FileExistsOrNull]: nothing or an empty path pass; a relative path is looked for in the working directory.
std::optional<std::string> file_exists_or_null(const std::optional<std::string>& path, const std::string& working_directory);

// [FilesExist]: every path must be a file (a blank path is not one); a relative path is looked for in the working directory.
std::optional<std::string> files_exist(const std::vector<std::string>& paths, const std::string& working_directory);

// McMaster's [FileExists] and [DirectoryExists] ("The file 'x' does not exist.", "The directory 'x' does not exist."): the path as typed, relative to
// the current directory.
std::optional<std::string> file_exists(const std::string& path);
std::optional<std::string> directory_exists(const std::string& path);

// File.Exists: a file (not a directory) of that name; false for a path that cannot be read.
bool file_exists_on_disk(const std::string& path);

}  // namespace cb::dotnet::decompiler::ilspycmd
