// source/console_shapes.hpp -- inside the console writer (console_writer.hpp): the shapes a description names, each in its own file.
#pragma once

#include <any>
#include <filesystem>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "formats/code_scan.hpp"
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

// The head comment's lines, said already (`mark` ahead of each, an empty template an empty line).
std::vector<std::string> head_comment(const Description& d, const Values& values, std::string_view mark = "; ");

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

// The program a loader put in `contained.program` (a std::shared_ptr<const T>); std::bad_any_cast when it holds another.
template <class T>
const T& contained_as(const Loaded& program) {
    return *std::any_cast<const std::shared_ptr<const T>&>(program.contained.program);
}

// The `flat` shape (console_executable.cpp): a cartridge ROM written whole by its processor's engine (the description's `flat.cpu`).
image_writer::Written write_flat(const Description& d, const Loaded& program, const Values& values, const std::filesystem::path& folder);

// The flat shape's processors: the 8048 and the F8 (console_flat8.cpp), the TLCS-900/H (console_tlcs900.cpp), the 6809 (console_m6809.cpp),
// the CP1610 (console_cp1610.cpp), the V30MZ (console_v30.cpp).
namespace flat8_code {
image_writer::Written write(const Description& d, const Loaded& program, const Values& values, const std::filesystem::path& folder);
}
namespace tlcs900_code {
image_writer::Written write(const Description& d, const Loaded& program, const Values& values, const std::filesystem::path& folder);
}
namespace m6809_code {
image_writer::Written write(const Description& d, const Loaded& program, const Values& values, const std::filesystem::path& folder);
formats::Decoder decoder();   // (the Vectrex's code scan: formats::vectrex::Rom::functions)
}
namespace cp1610_code {
image_writer::Written write(const Description& d, const Loaded& program, const Values& values, const std::filesystem::path& folder);
}
namespace v30_code {
image_writer::Written write(const Description& d, const Loaded& program, const Values& values, const std::filesystem::path& folder);
formats::Decoder decoder(std::uint32_t code_base);   // (the WonderSwan's code scan: formats::wonderswan::Rom::functions)
}

// The executable shape's containers for the Sega CD (console_segacd.cpp), the 32X (console_sega32x.cpp), the 3DO (console_threedo.cpp) and
// the CD-i (console_cdi.cpp).
namespace orbis_code {   // (the PlayStation 4 and 5: console_orbis.cpp)
image_writer::Written write(const Description& d, const Loaded& program, const Values& values, const std::filesystem::path& folder);
}
namespace xboxone_code {   // (console_xboxone.cpp)
image_writer::Written write(const Description& d, const Loaded& program, const Values& values, const std::filesystem::path& folder);
}
namespace threedo_code {
image_writer::Written write(const Description& d, const Loaded& program, const Values& values, const std::filesystem::path& folder);
}
namespace cdi_code {
image_writer::Written write(const Description& d, const Loaded& program, const Values& values, const std::filesystem::path& folder);
}
namespace segacd_code {
image_writer::Written write(const Description& d, const Loaded& program, const Values& values, const std::filesystem::path& folder);
}
namespace sega32x_code {
image_writer::Written write(const Description& d, const Loaded& program, const Values& values, const std::filesystem::path& folder);
}

// The executable shape's containers that are engines of their own: `ps3` (console_ps3.cpp), `gekko` (console_gekko.cpp), `vita` (console_vita.cpp), `n3ds` (console_n3ds.cpp),
// `switch` (console_switch.cpp), `nds` (console_nds.cpp).
namespace ps3_code {
image_writer::Written write(const Description& d, const Loaded& program, const Values& values, const std::filesystem::path& folder);
}
namespace gekko_code {
image_writer::Written write(const Description& d, const Loaded& program, Values values, const std::filesystem::path& folder);
}
namespace vita_code {
image_writer::Written write(const Description& d, const Loaded& program, const Values& values, const std::filesystem::path& folder);
}
namespace n3ds_code {
image_writer::Written write(const Description& d, const Loaded& program, const Values& values, const std::filesystem::path& folder);
}
namespace switch_code {
image_writer::Written write(const Description& d, const Loaded& program, const Values& values, const std::filesystem::path& folder);
}
namespace nds_code {
image_writer::Written write(const Description& d, const Loaded& program, Values values, const std::filesystem::path& folder);
}

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
