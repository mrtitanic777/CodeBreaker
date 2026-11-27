// dotnet/decompiler/ilspycmd/ilspycmd_program.hpp -- ilspycmd, ILSpy's command line tool, as a function: the same options, the same text on
// the standard output and error streams, the same files, the same exit codes.
//
//     std::vector<std::string> args = {"-l", "c", "Sample.dll"};
//     std::ostringstream out, err;
//     int exit_code = run_ilspycmd(args, out, err);       // out: "Class <Module>\r\nClass Sample.Circle\r\n..."
//
// Ported from ILSpy (ICSharpCode.ILSpyCmd: ILSpyCmdProgram, from IlspyCmdProgram.cs), Copyright (c) 2019 Christoph Wille, MIT (see NOTICE
// in dotnet/decompiler). The command line is read by command_line.hpp as McMaster.Extensions.CommandLineUtils reads it; the program is
// ilspycmd 11.0.0.9375 (what -v prints). Lines end with CR LF, as Console.WriteLine writes them on Windows, so a stream that translates
// line ends must be opened in binary mode.
//
// What is the tool's, as upstream has it, and what is not ported:
// - Not ported: --generate-diagrammer and its options (the HTML diagrammer of ILSpyX), -genpdb when the assembly has a debug directory
//   (the portable PDB writer; without a CodeView entry the tool's own refusal is written, as upstream), reading a PDB for --use-varnames-from-pdb
//   and --il-sequence-points (the debug information is not loaded: the output is that of an assembly without it, and a note says so),
//   BAML (--decompile-baml, and --resource of a .baml: the BAML decompiler, ICSharpCode.BamlDecompiler, is not part of the port; a project
//   keeps the .baml streams, and --resource says it cannot decompile one), the check for a newer version of the tool
//   (DotNetToolUpdateChecker: --disable-updatecheck is accepted and the check is never made), and the rewriting of the references
//   between the projects of a solution (solution_writer.hpp).
// - A mistake in the command line that .NET reports as an unhandled exception ends with that exception's first line, without the stack
//   trace, and the exit code .NET gives (-532462766, 0xE0434352).
// - An exception while decompiling is written as its type and message (upstream writes Exception.ToString(), with a stack trace).
#pragma once

#include <iosfwd>
#include <span>
#include <string>

namespace cb::dotnet::decompiler::ilspycmd {

// The version of ilspycmd and of ICSharpCode.Decompiler this program stands for.
inline constexpr const char* kIlspycmdVersion = "11.0.0.9375";

// Runs ilspycmd with the arguments (the program's name not among them): writes the standard output to `out` and the standard error to
// `err`, returns the exit code.
int run_ilspycmd(std::span<const std::string> args, std::ostream& out, std::ostream& err);

}  // namespace cb::dotnet::decompiler::ilspycmd
