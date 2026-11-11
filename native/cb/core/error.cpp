#include "cb/core/error.hpp"

#include <format>

#include "cb/core/win.hpp"

namespace cb {

std::string path_text(const std::filesystem::path& p) { return win::from_wide(p.generic_wstring()); }

std::string FormatError::hex(std::uint64_t v) { return std::format("0x{:x}", v); }

std::string ParseError::location(std::string_view file, std::uint32_t line, std::uint32_t column) {
    std::string out(file);
    if (line != 0) {
        out += std::format(":{}", line);
        if (column != 0) out += std::format(":{}", column);
    }
    return out;
}

int exit_code_of(const std::exception& e) {
    if (const auto* err = dynamic_cast<const Error*>(&e)) return err->exit_code();
    return kExitInternal;
}

void assertion_failed(const char* condition, std::string_view why, const char* file, int line) {
    throw InternalError(std::format("{}:{}: assertion failed: {} ({})", file, line, condition, why));
}

}  // namespace cb
