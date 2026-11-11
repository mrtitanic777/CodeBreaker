// cb/platforms/vxd.hpp -- Windows 95 device drivers: LE images that run in ring 0 (VxDs), written in assembly.
//
// What the descriptor says: the 386 in flat ring 0; a driver's register convention (arguments and results in
// registers, the carry and zero flags an answer too); the driver emulator is the judge's runner; the kit's wasm (the
// one that knows the DDK's macros) assembles it and wlink links the LE. A driver file is an LE image, or a W3 bundle
// of them (vmm32.vxd). The LE reader, the writer of the source and the toolchain's paths are the formats, source and
// build layers'; they key their own tables on `Id::Vxd`.
#pragma once

#include <filesystem>
#include <string>

#include "cb/core/ordered_map.hpp"
#include "cb/formats/le.hpp"
#include "cb/platforms/package.hpp"

namespace cb::platforms {

Platform vxd();

// Everywhere the system or another driver can enter the first driver in the file ("control", "v86_api", "pm_api",
// "service_N"), in that order, each as its (object, offset). Empty for an image without a device descriptor.
// Throws IoError / FormatError for a file that holds no LE image.
OrderedMap<std::string, formats::le::Place> vxd_entry_points(const std::filesystem::path& path);

}  // namespace cb::platforms
