// cb/build/x86asm/format.cpp -- the format registry (NASM's outform.c) and the null debug format (nulldbg.c).
#include "cb/build/x86asm/format.hpp"

#include <array>

namespace cb::build::x86asm {

namespace drvns_format {

bool iequals(std::string_view a, std::string_view b) {
    if (a.size() != b.size()) return false;
    for (std::size_t i = 0; i < a.size(); ++i) {
        char x = a[i], y = b[i];
        if (x >= 'A' && x <= 'Z') x = static_cast<char>(x + 32);
        if (y >= 'A' && y <= 'Z') y = static_cast<char>(y + 32);
        if (x != y) return false;
    }
    return true;
}

// The formats in NASM's order (outform.h's drivers[]), each with its description, and its factory.
struct Entry {
    const char* shortname;
    const char* fullname;
    std::unique_ptr<OutputFormat> (*make)(FormatHost&, std::string_view);
};
constexpr std::array<Entry, 8> kFormats = {{
    {"bin", "Flat raw binary (MS-DOS, embedded, ...)", make_bin_format},
    {"ith", "Intel Hex encoded flat binary", make_bin_format},
    {"srec", "Motorola S-records encoded flat binary", make_bin_format},
    {"coff", "COFF (i386) (DJGPP, some Unix variants)", make_coff_format},
    {"obj", "Intel/Microsoft OMF (MS-DOS, OS/2, Win16)", make_obj_format},
    {"obj2", "Intel/Microsoft OMF (i386) (OS/2)", make_obj_format},
    {"win32", "Microsoft extended COFF for Win32 (i386)", make_coff_format},
    {"win64", "Microsoft extended COFF for Win64 (x86-64)", make_coff_format},
}};

struct Alias {
    const char* shortname;
    const char* target;
};
// outform.h's ofmt_aliases[], restricted to the formats present.
constexpr std::array<Alias, 5> kAliases = {{
    {"omf", "obj"},
    {"omf2", "obj2"},
    {"os2", "obj2"},
    {"win", "win32"},
    {"elf", "elf32"},
}};

class NullDebugFormat final : public DebugFormat {};

}  // namespace drvns_format
using namespace drvns_format;

const std::vector<PragmaFacility>& DebugFormat::pragmas() const {
    static const std::vector<PragmaFacility> none;
    return none;
}

const std::vector<PragmaFacility>& OutputFormat::pragmas() const {
    static const std::vector<PragmaFacility> none;
    return none;
}

const DebugFormat& null_debug_format() {
    static const NullDebugFormat null_format;
    return null_format;
}

std::unique_ptr<OutputFormat> make_format(std::string_view name, FormatHost& host, std::string* alias) {
    if (alias) alias->clear();
    for (const Entry& e : kFormats)
        if (iequals(name, e.shortname)) return e.make(host, e.shortname);
    for (const Alias& a : kAliases) {
        if (!iequals(name, a.shortname)) continue;
        for (const Entry& e : kFormats) {
            if (!iequals(a.target, e.shortname)) continue;
            if (alias) *alias = a.shortname;
            return e.make(host, e.shortname);
        }
    }
    return nullptr;
}

std::vector<FormatName> list_formats() {
    std::vector<FormatName> out;
    for (const Entry& e : kFormats) out.push_back({e.shortname, e.fullname, ""});
    for (const Alias& a : kAliases) out.push_back({a.shortname, "", a.target});
    return out;
}

const DebugFormat* find_debug_format(const OutputFormat& fmt, std::string_view name) {
    for (const DebugFormat* d : fmt.debug_formats())
        if (iequals(name, d->shortname())) return d;
    return nullptr;
}

}  // namespace cb::build::x86asm
