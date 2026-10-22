#pragma once

// Xenos shader microcode: decoding and disassembly (spec: analysis/xenos/03a-03c).
// Microcode is an array of 32-bit words in host (numeric) order. Control-flow instructions are 48 bits, packed two per
// 96-bit slot at the start; ALU and fetch instructions are one slot each and are addressed by slot index.

#include <cstdint>
#include <string>
#include <vector>

namespace gpu::ucode
{
    enum class Stage : uint32_t
    {
        kVertex = 0,
        kPixel = 1,
    };

    // What a shader reads and writes, gathered while disassembling.
    struct Analysis
    {
        uint32_t cfCount = 0;                       // control-flow instructions (up to the first exec's address)
        std::vector<uint32_t> textureFetchConstants; // tf# used by texture instructions, sorted, unique
        std::vector<uint32_t> vertexFetchConstants;  // hardware vertex fetch constant (0-95), sorted, unique
        uint32_t interpolatorsWritten = 0;          // VS: bit n = o#n
        uint32_t colorsWritten = 0;                 // PS: bit n = oC#n
        bool writesPosition = false;
        bool writesPointSize = false;               // oPts
        bool writesDepth = false;
        bool memoryExport = false;                  // eA / eM# written
        bool kills = false;
        bool usesCalls = false;                     // call / ret
        bool usesLoops = false;
        bool usesPredication = false;
        std::vector<std::string> errors;            // encodings the spec does not define
    };

    // Text disassembly of a whole shader. Every undefined encoding is listed in `analysis.errors` (and marked in the
    // text); the caller decides whether that is fatal.
    std::string Disassemble(Stage stage, const std::vector<uint32_t>& words, Analysis& analysis);

    const char* StageName(Stage stage);
}
