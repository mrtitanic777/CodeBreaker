// XEXMagic - C/C++ Code Generator
// Emits compilable C or C++ source from validated, named IL functions.

#pragma once

#include <string>
#include <vector>

#include "il.h"

// Generate C or C++ source for a set of functions.
std::string GenerateSource(const std::vector<ILFunction>& funcs,
                           const CodeGenConfig& config,
                           const char* source_file = nullptr);

// Generate source for a single function (no header/footer).
std::string GenerateFunctionSource(const ILFunction& func,
                                   const CodeGenConfig& config);
