// codebreaker/base/capstone_dll.hpp -- where the capstone DLL is: the one lookup the C++ decoders share.
//
// Python's `import capstone` finds the package on sys.path and its lib/capstone.dll beside it. C++ has no import: every decoder
// (decompile/describe, analyze/image + native + dos_functions, symbols/x86) loads the DLL by path. They all ask this lookup when
// nobody told them a path (their set_capstone_dll() stays: it overrides), so forgetting a setter cannot change a result.
//
// Found, in order: the CB_CAPSTONE_DLL environment variable; the setting "capstone_dll"; <entry>/capstone/lib/capstone.dll for each
// entry of PYTHONPATH; the site-packages of the `python` on PATH and of its user site (%APPDATA%\Python\PythonXY\site-packages).
// ImportError ("No module named 'capstone'") when none holds one.
#pragma once

#include "py.hpp"

namespace codebreaker::base::capstone_dll {

// The path of capstone.dll (looked for once).
py::str find();

// The path of unicorn.dll, the one the Python unicorn package carries (unicorn/lib/unicorn.dll), looked for the same way:
// CB_UNICORN_DLL, the setting "unicorn_dll", then the site-packages. ImportError ("No module named 'unicorn'") when none holds one.
py::str find_unicorn();

}  // namespace codebreaker::base::capstone_dll
