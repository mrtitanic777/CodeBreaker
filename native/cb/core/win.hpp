// cb/core/win.hpp -- the one place a wide string exists: the Win32 W-API boundary.
//
// CodeBreaker text is UTF-8 everywhere. Calls into CreateProcessW, CreateFileW, the console and the like need
// UTF-16; this header is where the conversion happens, and no other interface mentions std::wstring. The
// header does not include <windows.h>.
#pragma once

#include <cstdint>
#include <string>
#include <string_view>

namespace cb::win {

// UTF-8 to UTF-16 for a W-API call. Invalid UTF-8 bytes become U+FFFD.
std::wstring to_wide(std::string_view utf8);
// UTF-16 from a W-API call to UTF-8. An unpaired surrogate becomes U+FFFD.
std::string from_wide(std::wstring_view wide);

// The text of a Win32 error code ("The system cannot find the file specified."), without the trailing newline.
std::string error_message(std::uint32_t code);
// GetLastError().
std::uint32_t last_error();

// Put the console in UTF-8 (code page 65001) for input and output. Called once at startup.
void use_utf8_console();

}  // namespace cb::win
