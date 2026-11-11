// cb/base/capstone_dll.hpp -- where the Capstone and Unicorn DLLs are, for the decoders that load them by path.
//
// CodeBreaker ships capstone.dll and unicorn.dll beside its executable (docs/CPP_DESIGN.md, decision 1); nothing is
// read from Python's site-packages. Code that wants a function table loads them through cb::disasm::capstone::api()
// / cb::emu::unicorn::api() (cb/core); this is for the callers that need the file's path itself (a decoder that
// opens the DLL with its own binding, a diagnostic that says which copy it used).
//
// Found, in order: the environment variable named below, when it names a file; then the places DynLib looks
// (cb/core/dynlib.hpp: beside the executable, its third_party/bin, CB_THIRD_PARTY_BIN, the build tree's copy).
#pragma once
#include <filesystem>

namespace cb::base::capstone_dll {

// The path of capstone.dll: CB_CAPSTONE_DLL, else beside the executable (and the other places). NotFoundError when
// no copy exists.
std::filesystem::path capstone();

// The path of unicorn.dll: CB_UNICORN_DLL, else the same places. NotFoundError when no copy exists.
std::filesystem::path unicorn();

}  // namespace cb::base::capstone_dll
