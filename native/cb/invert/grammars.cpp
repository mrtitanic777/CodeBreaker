#include "cb/invert/grammars.hpp"

#include <array>
#include <format>

namespace cb::invert::grammars {

std::vector<std::string> thunk(const Shape& shape) {
    const std::size_t n = shape.nargs;
    std::vector<std::string> out;
    std::string params;
    for (std::size_t i = 0; i < n; ++i) params += std::format("{}int a{}", i ? ", " : "", i);
    for (const char* conv : {"pascal ", ""}) {
        for (const bool reversed : {false, true}) {
            std::string forwarded;
            for (std::size_t k = 0; k < n; ++k) {
                const std::size_t i = reversed ? n - 1 - k : k;
                forwarded += std::format("{}a{}", k ? ", " : "", i);
            }
            out.push_back(
                std::format("extern int far SIB();\nint far {}F({}){{ return SIB({}); }}", conv, params, forwarded));
        }
    }
    return out;
}

std::vector<std::string> bufread(const Shape&) {
    std::vector<std::string> out;
    // 2-byte WORD variant: the value is built from two bytes or read through an unsigned pointer.
    for (const int bufsz : {2, 4}) {
        for (const char* read : {"b[0]|(b[1]<<8)", "*(unsigned*)b"}) {
            for (const bool nested : {true, false}) {
                const std::string body =
                    nested ? std::format("unsigned char b[{}]; if (SIB(p,(char far*)b,2)){{ *out={}; return 0xffff; }} "
                                         "return 0;",
                                         bufsz, read)
                           : std::format("unsigned char b[{}]; if (!SIB(p,(char far*)b,2)) return 0; *out={}; return "
                                         "0xffff;",
                                         bufsz, read);
                out.push_back(std::format("extern int far SIB();\nint far F(int p, unsigned *out){{ {} }}", body));
            }
        }
    }
    // 4-byte long variant: declare `long v`, then build it AFTER the buffer is filled.
    constexpr std::string_view build = "v=b[3]; v=(v<<8)+b[2]; v=(v<<8)+b[1]; v=(v<<8)+b[0];";
    for (const int bufsz : {4, 6}) {
        for (const bool nested : {true, false}) {
            const std::string body =
                nested ? std::format("unsigned char b[{}]; long v; if (SIB(p,(char far*)b,4)){{ {} *out=v; return "
                                     "0xffff; }} return 0;",
                                     bufsz, build)
                       : std::format("unsigned char b[{}]; long v; if (!SIB(p,(char far*)b,4)) return 0; {} *out=v; "
                                     "return 0xffff;",
                                     bufsz, build);
            out.push_back(std::format("extern int far SIB();\nint far F(int p, long *out){{ {} }}", body));
        }
    }
    return out;
}

std::span<const Grammar> all() {
    static const std::array<Grammar, 2> kGrammars = {{{"thunk", thunk}, {"bufread", bufread}}};
    return kGrammars;
}

}  // namespace cb::invert::grammars
