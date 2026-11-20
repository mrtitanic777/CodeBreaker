// xex_image_test <module.xex> <image out>: the module read by the runtime's xex::ReadImage. Prints its base address,
// image size and resources (header 0x2FF) and writes the decoded image, for tests/test_x360.py to hold against the xex
// engine's basefile of the same file.

#include "log.h"
#include "xex.h"

#include <cstdio>
#include <fstream>

// The test's stand-in for the runtime's log (log.cpp writes the program's log file and talks to the LIVE server): lines
// to stderr.
namespace logging
{
    void Write(const std::string& line) { std::fprintf(stderr, "%s\n", line.c_str()); }
    void Flush() { std::fflush(stderr); }
    void PauseIfOwnConsole() {}
    void ShowFatal(const std::string&) {}
}

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::fprintf(stderr, "usage: xex_image_test <module.xex> <image out>\n");
        return 2;
    }
    const xex::HostImage image = xex::ReadImage(argv[1]);
    std::printf("base %08X size %zu\n", image.base, image.bytes.size());
    for (const auto& [name, where] : image.resources)
    {
        // At(): the bytes a reader of the resource gets (nullptr when it lies outside the image)
        const bool inside = image.At(where.first, where.second) != nullptr;
        std::printf("resource %s %08X %08X %s\n", name.c_str(), where.first, where.second, inside ? "inside" : "outside");
    }
    std::ofstream out(argv[2], std::ios::binary);
    out.write(reinterpret_cast<const char*>(image.bytes.data()), std::streamsize(image.bytes.size()));
    return out ? 0 : 1;
}
