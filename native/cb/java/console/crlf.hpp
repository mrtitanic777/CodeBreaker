// cb/java/console/crlf.hpp -- text with Windows line ends.
//
// The files the console lane writes for Visual Studio, XACTBLD and cmd (.sln, .vcxproj, .filters, .xap, .bat) are
// written with CRLF line ends, as the tools themselves write them; the program builds its text with "\n".
#pragma once

#include <string>
#include <string_view>

namespace cb::java::console {

// The text with every line end made CRLF (an existing CRLF stays one pair).
inline std::string to_crlf(std::string_view s) {
    std::string out;
    out.reserve(s.size() + s.size() / 16);
    for (std::size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '\r' && i + 1 < s.size() && s[i + 1] == '\n') continue;   // the '\n' that follows adds the pair
        if (s[i] == '\n') out += "\r\n";
        else out += s[i];
    }
    return out;
}

}  // namespace cb::java::console
