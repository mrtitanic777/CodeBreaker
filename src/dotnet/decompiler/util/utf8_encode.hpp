// dotnet/decompiler/util/utf8_encode.hpp -- one code point as UTF-8 bytes appended to a string (what the readers of UTF-16 text in this folder need).
//
//     std::string s;  append_utf8_code_point(s, 0x20AC);     // "\xE2\x82\xAC"
//
// Written for the ILSpy port (no counterpart upstream: .NET converts text with Encoding.UTF8).
#pragma once

#include <cstdint>
#include <string>

namespace cb::dotnet::decompiler::util {

inline void append_utf8_code_point(std::string& out, std::uint32_t cp) {
    if (cp < 0x80) {
        out.push_back(static_cast<char>(cp));
    } else if (cp < 0x800) {
        out.push_back(static_cast<char>(0xC0 | (cp >> 6)));
        out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    } else if (cp < 0x10000) {
        out.push_back(static_cast<char>(0xE0 | (cp >> 12)));
        out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    } else {
        out.push_back(static_cast<char>(0xF0 | (cp >> 18)));
        out.push_back(static_cast<char>(0x80 | ((cp >> 12) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    }
}

}  // namespace cb::dotnet::decompiler::util
