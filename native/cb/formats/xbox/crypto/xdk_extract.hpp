// cb/formats/xbox/crypto/xdk_extract.hpp -- the cabinets hidden in an XDK installer: XDKRecoveryXenonXXXXX.exe and XDKSetupXenonXXXXX.exe are
// small 32-bit Windows programs with Microsoft cabinets (MSCF) appended after the last section's data.
//
//     std::vector<xdk_extract::Cabinet> cabs = xdk_extract::find_cabinets(exe_bytes);
//     xdk_extract::extract(exe_path, out_dir, "data");          // data_0.cab, data_1.cab ...
//
// The end of the program is the largest (raw data pointer + raw data size) of its sections; the cabinets follow one another from there, each
// naming its own size in its header (offset 8). The scan stops at the first place that is not a cabinet.
//
// Errors: FormatError for a file that is not an i386 PE (no MZ, no PE signature, another machine, or a section table that runs off the file).
#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/cli.hpp"

namespace cb::formats::xbox::crypto::xdk_extract {

struct Cabinet {
    std::uint64_t offset = 0;
    std::uint32_t size = 0;
};

// Where the PE image's raw data ends (and the first cabinet may start).
std::uint64_t end_of_image(ByteView exe);

std::vector<Cabinet> find_cabinets(ByteView exe);

// Write each cabinet of `exe` as <out_dir>/<prefix>_<n>.cab (the folder is made); the files written.
std::vector<std::filesystem::path> extract(const std::filesystem::path& exe, const std::filesystem::path& out_dir, const std::string& prefix = "data");

cli::Command command();

}  // namespace cb::formats::xbox::crypto::xdk_extract
