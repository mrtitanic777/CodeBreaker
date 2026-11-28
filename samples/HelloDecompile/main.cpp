// HelloDecompile -- decompile one function to C with the CodeBreaker SDK, from C++ (cb.hpp: RAII handles, string views, exceptions).
//
//     HelloDecompile lz32.dll LZCopy          a function by the name the file gives it
//     HelloDecompile lz32.dll 0xbfea11aa      or by its address ("seg:off" for a 16-bit module)
//     HelloDecompile lz32.dll                 the first function the file names
//     HelloDecompile lz32.dll --module        every function, as one .c file (its size and counts are printed)
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <string_view>

#include "cb/cb.hpp"
#include "framework.h"

namespace {

// An address as cb_address_format writes it: "0xbfea11aa" (linear), "4:015c" (an NE segment or LE object, numbered in decimal, and an
// offset in hex), "1234:0010" (real mode, both in hex). Anything else is a name.
bool parse_address(std::string_view text, const cb::sdk::Binary& binary, cb_address& out) {
    const auto number = [](std::string_view s, unsigned base, std::uint64_t& v) {
        if (base == 16 && s.starts_with("0x")) s.remove_prefix(2);
        if (s.empty() || s.size() > 16) return false;
        v = 0;
        for (const char c : s) {
            const int d = c >= '0' && c <= '9' ? c - '0' : c >= 'a' && c <= 'f' ? c - 'a' + 10 : c >= 'A' && c <= 'F' ? c - 'A' + 10 : 99;
            if (d >= static_cast<int>(base)) return false;
            v = v * base + static_cast<std::uint64_t>(d);
        }
        return true;
    };
    std::uint64_t a = 0, b = 0;
    if (const std::size_t colon = text.find(':'); colon != std::string_view::npos) {
        const bool real_mode = binary.format() == "MZ";
        if (!number(text.substr(0, colon), real_mode ? 16 : 10, a) || !number(text.substr(colon + 1), 16, b)) return false;
        const std::uint32_t style = real_mode ? CB_ADDRESS_REAL_MODE : binary.format() == "NE" ? CB_ADDRESS_NE_SEGMENT : CB_ADDRESS_LE_OBJECT;
        out = cb_address{b, static_cast<std::uint32_t>(a), style};
        return true;
    }
    if (!text.starts_with("0x")) return false;   // (a bare word is a name, even one made of hex digits: "Add")
    if (!number(text, 16, a)) return false;
    out = cb::sdk::linear(a);
    return true;
}

}  // namespace

int main(int argc, char** argv) {
    static const char* const switches[] = {"--module", nullptr};
    fw_args args;
    fw_parse(argc, argv, "HelloDecompile <file> [function-or-address] [--module]", 1, switches, &args);
    fw_start(&args);
    try {
        cb::sdk::check_decompiler();
        cb::sdk::Binary binary = cb::sdk::Binary::open(args.values[0]);
        if (args.flags[0]) {
            const double t = fw_seconds();
            cb_module_info info{};
            const cb::sdk::String c = binary.decompile(&info);
            fw_timed("cb_module_decompile", t);
            std::cout << fw_file_name(args.values[0]) << ": " << info.decompiled << " of " << info.found << " functions decompiled ("
                      << info.library << " the compiler's library), " << c.size() << " bytes of C\n";
            fw_stop();
            return 0;
        }
        cb::sdk::Function f;
        if (args.count < 2) {
            if (binary.functions().size() == 0) throw cb::sdk::Error(CB_ERROR_NOT_FOUND, "the file names no function: give an address");
            f = binary.functions()[0];
        } else if (cb_address at{}; parse_address(args.values[1], binary, at)) {
            f = binary.function_at(at);
        } else {
            f = binary.function(args.values[1]);
        }
        const double t = fw_seconds();
        const cb::sdk::String c = f.decompile();
        fw_timed("cb_function_decompile", t);
        std::cout << "// " << f.name() << " at " << cb::sdk::address_text(f.address()) << " in " << fw_file_name(args.values[0]) << " ("
                  << binary.format() << ", " << binary.arch() << ")\n"
                  << c.view() << "\n";
    } catch (const cb::sdk::Error& e) {
        std::cerr << args.program << ": " << cb_status_name(e.status()) << ": " << e.what() << "\n";
        fw_stop();
        return 1;
    }
    fw_stop();
    return 0;
}
