// cb/java/cmake.hpp -- the PC build: a CMakeLists.txt beside a pc_opengl conversion.
//
// Every .cpp in the folder is one executable (the converted program, the runtime, the SDL2/OpenGL platform layer
// and the generated entry). C++17; SDL2 and OpenGL from the system (MSYS2 / a Linux distribution / a CMake
// package). A `Content` folder beside the CMakeLists is copied next to the program: the game's resources, as the
// entry point looks for them.
#pragma once

#include <filesystem>
#include <string>
#include <string_view>

namespace cb::java::cmake {

// The CMakeLists.txt text for an executable called `name`.
std::string cmakelists_text(std::string_view name);

// Write out_dir/CMakeLists.txt for an executable called `name`; returns its path.
std::filesystem::path write_cmakelists(const std::filesystem::path& out_dir, std::string_view name);

}  // namespace cb::java::cmake
