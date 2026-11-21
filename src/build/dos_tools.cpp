#include "build/dos_tools.hpp"

namespace cb::build::dos_tools {

std::optional<std::filesystem::path> masm_root(const config::Config& config) {
    const auto root = config.path("masm");
    if (!root) return std::nullopt;
    std::error_code ec;
    if (!std::filesystem::is_regular_file(*root / "BIN" / "MASM.EXE", ec)) return std::nullopt;
    return root;
}

}  // namespace cb::build::dos_tools
