#include "cb/core/win.hpp"

#include <windows.h>

#include "cb/core/text.hpp"

namespace cb::win {

std::wstring to_wide(std::string_view utf8) {
    if (utf8.empty()) return {};
    // Sanitise first so that MultiByteToWideChar never fails on a bad sequence.
    const std::string clean = text::is_valid_utf8(utf8) ? std::string() : text::sanitize_utf8(utf8);
    std::string_view src = clean.empty() ? utf8 : std::string_view(clean);
    const int n = MultiByteToWideChar(CP_UTF8, 0, src.data(), static_cast<int>(src.size()), nullptr, 0);
    if (n <= 0) return {};
    std::wstring out(static_cast<std::size_t>(n), L'\0');
    MultiByteToWideChar(CP_UTF8, 0, src.data(), static_cast<int>(src.size()), out.data(), n);
    return out;
}

std::string from_wide(std::wstring_view wide) {
    if (wide.empty()) return {};
    const int n = WideCharToMultiByte(CP_UTF8, 0, wide.data(), static_cast<int>(wide.size()), nullptr, 0, nullptr, nullptr);
    if (n <= 0) return {};
    std::string out(static_cast<std::size_t>(n), '\0');
    WideCharToMultiByte(CP_UTF8, 0, wide.data(), static_cast<int>(wide.size()), out.data(), n, nullptr, nullptr);
    return out;
}

std::string error_message(std::uint32_t code) {
    wchar_t* buf = nullptr;
    const DWORD n = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                   nullptr, code, 0, reinterpret_cast<wchar_t*>(&buf), 0, nullptr);
    std::string out;
    if (n != 0 && buf != nullptr) out = from_wide(std::wstring_view(buf, n));
    if (buf != nullptr) LocalFree(buf);
    while (!out.empty() && (out.back() == '\n' || out.back() == '\r' || out.back() == ' ')) out.pop_back();
    if (out.empty()) out = "error " + std::to_string(code);
    return out;
}

std::uint32_t last_error() { return GetLastError(); }

void use_utf8_console() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

}  // namespace cb::win
