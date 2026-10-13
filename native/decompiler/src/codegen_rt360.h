// XEXMagic - RT360 Code Generator
// Emits C/C++ source using Nexia360's PPCContext ABI for static recompilation.
// Includes guest memory initialization data baked from the XEX at decompile time.

#pragma once

#include <string>
#include <vector>

#include "il.h"

// Generate RT360 source for a set of functions, with guest init data.
std::string GenerateRT360Source(const std::vector<ILFunction>& funcs,
                                const CodeGenConfig& config,
                                const RT360GuestInitData* init_data,
                                const char* source_file = nullptr);

// Generate RT360 source for a single function.
std::string GenerateRT360FunctionSource(const ILFunction& func,
                                        const CodeGenConfig& config);

// Generate the .rt360map JSON source map file content.
std::string GenerateRT360SourceMap(const std::vector<ILFunction>& funcs,
                                   const CodeGenConfig& config,
                                   const char* source_file = nullptr);

// Build RT360GuestInitData from a loaded XEX.
// Call this in main.cc after loading the XEX, before codegen.
#include "pe_section.h"
class XexLoader;
RT360GuestInitData BuildRT360GuestInitData(
    const XexLoader& loader,
    const std::vector<PESectionInfo>& sections,
    const uint8_t* image, size_t image_size);
