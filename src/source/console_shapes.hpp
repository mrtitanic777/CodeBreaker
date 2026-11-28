// source/console_shapes.hpp -- inside the console writer (console_writer.hpp): the shapes a description names, each in its own file.
#pragma once

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

#include "source/console_writer.hpp"

namespace cb::source::console_writer {

// The `banked` shape (console_banked.cpp). `head` is the head comment's lines, said already.
image_writer::Written write_banked(const Description& d, const Loaded& program, const std::vector<std::string>& head, const Values& values,
                                   std::string_view file_name, const std::filesystem::path& folder);

// The `cart6502` shape (console_cart6502.cpp).
image_writer::Written write_cart6502(const Description& d, const Loaded& program, const std::vector<std::string>& head, std::string_view file_name,
                                     const std::filesystem::path& folder);

// The `segmented` shape (console_segmented.cpp).
image_writer::Written write_segmented(const Description& d, const Loaded& program, const std::vector<std::string>& head, const Values& values,
                                      std::string_view file_name, const std::filesystem::path& folder);

// The head comment's lines, said already ("; " ahead of each, an empty template an empty line).
std::vector<std::string> head_comment(const Description& d, const Values& values);

// The `vectored` shape for a MIPS program (console_mips.cpp): `values` has `file` and `stem`; the head may name `functions`, `base`, `text_size`.
namespace mips_code {
image_writer::Written write(const Description& d, const Loaded& program, Values values, const std::filesystem::path& folder);
}

// The `vectored` shape for an ARM7TDMI program (console_arm.cpp): the same, the head may name `functions` and `base`.
namespace arm_code {
image_writer::Written write(const Description& d, const Loaded& program, Values values, const std::filesystem::path& folder);
}

// The `executable` shape (console_executable.cpp): `values` has `file` and `stem`.
image_writer::Written write_executable(const Description& d, const Loaded& program, const Values& values, const std::filesystem::path& folder);

// The `vectored` shape for a V810 program (console_v810.cpp): `values` has `header_at` (the header's offset).
namespace v810_code {
image_writer::Written write(const Description& d, const Loaded& program, Values values, const std::filesystem::path& folder);
}

// The `vectored` shape (console_vectored.cpp).
image_writer::Written write_vectored(const Description& d, const Loaded& program, const std::vector<std::string>& head, const Values& values,
                                     std::string_view file_name, const std::filesystem::path& folder);

// The loaders whose code keeps a state flow follows (console_pce.cpp, console_snes.cpp).
namespace pce_code {
Loaded load_pce_file(const std::filesystem::path& file);
}
namespace snes_code {
Loaded load_snes_file(const std::filesystem::path& file);
}

}  // namespace cb::source::console_writer
