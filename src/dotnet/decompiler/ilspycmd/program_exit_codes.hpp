// dotnet/decompiler/ilspycmd/program_exit_codes.hpp -- the exit codes ilspycmd ends with (the sysexits of BSD).
//
//     return ProgramExitCodes::ex_dataerr;        // the input was wrong (a type that is not there, a file that is not a bundle)
//
// Ported from ILSpy (ICSharpCode.ILSpyCmd: ProgramExitCodes, from ProgramExitCodes.cs), Copyright (c) 2019 Christoph Wille, MIT (see
// NOTICE in dotnet/decompiler).
#pragma once

namespace cb::dotnet::decompiler::ilspycmd {

// https://www.freebsd.org/cgi/man.cgi?query=sysexits
struct ProgramExitCodes {
    static constexpr int ex_usage = 64;
    static constexpr int ex_dataerr = 65;
    static constexpr int ex_noinput = 66;
    static constexpr int ex_software = 70;
};

}  // namespace cb::dotnet::decompiler::ilspycmd
