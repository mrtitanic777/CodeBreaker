// build/x86asm/listing.hpp -- the listing file generator (NASM's listing.c / listing.h): the `-l` listing.
//
// One `Listing` per run. The preprocessor sends it the source lines and the level changes (includes, macros, TIMES,
// INCBIN), the encoder sends it the bytes it produces, and the error reporter sends it the messages (through
// Assembly::hooks.list_error); the listing is written into a byte buffer the driver gives it (the file named by -l).
#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "build/x86asm/assembly.hpp"
#include "build/x86asm/directives.hpp"
#include "build/x86asm/encode.hpp"
#include "core/bytes.hpp"

namespace cb::build::x86asm {

// List options implied by -L+ (listing.h).
inline constexpr std::string_view LIST_PLUS_OPTIONS = "bdefFmps";

// The bit of the option letters a-z, A-Z and 0-9 in an options word: bit 0 means the listing engine is active, bit 1 is
// reserved, so this returns bits 2 and higher; 0 for any other character (list_option_mask).
constexpr std::uint64_t list_option_mask(unsigned char x) {
    if (x >= 'a') {
        if (x > 'z') return 0;
        x = static_cast<unsigned char>(x - 'a' + 2);
    } else if (x >= 'A') {
        if (x > 'Z') return 0;
        x = static_cast<unsigned char>(x - 'A' + 2 + 26);
    } else if (x >= '0') {
        if (x > '9') return 0;
        x = static_cast<unsigned char>(x - '0' + 2 + 26 * 2);
    } else {
        return 0;
    }
    return std::uint64_t{1} << x;
}

inline constexpr std::uint64_t LIST_ALL_OPTIONS_MASK = ~std::uint64_t{3};

class Listing final : public ListingSink {
public:
    // `open` gives the buffer a listing file named by the string is written into (nullptr: it cannot be opened).
    Listing(Assembly& as, std::function<Bytes*(std::string_view)> open) : as_(as), open_(std::move(open)) {}

    // ---- options ----

    // The requested options, the ones active in this pass, and those the command line gave.
    std::uint64_t list_options = 0, active_list_options = 0, cmdline_list_options = 0;
    bool user_nolist = false;   // [LIST -]

    // True if the listing engine is active and an option is set (list_option).
    bool option(char x) const override { return (active_list_options & list_option_mask(static_cast<unsigned char>(x))) != 0; }
    // True if a listing should be started on this pass (list_on_this_pass).
    bool on_this_pass() const { return as_.pass_final() || (list_options & list_option_mask('p')) != 0; }
    // Is the listing engine active? (list_active)
    bool active() const { return (active_list_options & 1) != 0; }

    // -L and %pragma list options (list_update_options).
    void update_options(std::string_view str, bool from_cmdline);
    // %pragma list ... (list_pragma).
    DirectiveResult pragma(const Pragma& p);

    // ---- the generator (struct lfmt) ----

    void init(std::string_view fname);   // start the listing in the file `fname` (cleaning up a previous one first)
    void cleanup();                      // finish and close it
    void output(const OutData& data) override;    // bytes the encoder produced
    void line(ListType type, std::int32_t lineno, std::string_view text) override;   // a source line; lineno -1: the same as the last
    void uplevel(ListType type, std::int64_t size) override;
    void downlevel(ListType type) override;
    void error(ErrFlags severity, std::string_view text);   // a warning or error message
    void set_offset(std::int64_t offset) override { listoffset_ = static_cast<std::int32_t>(offset); }

    bool is_open() const { return fp_ != nullptr; }

private:
    void emit();
    void out(std::int64_t offset, std::string_view str);
    void address(std::int64_t offset, const char* brackets, std::int64_t addr, int size);
    void size_tag(std::int64_t offset, const char* tag, std::uint64_t size);

    Assembly& as_;
    std::function<Bytes*(std::string_view)> open_;
    Bytes* fp_ = nullptr;            // the listing file (listfp)

    std::string listline_;
    bool listlinep_ = false;
    std::vector<std::pair<ErrFlags, std::string>> list_errors_;
    std::string listdata_;
    std::int32_t listoffset_ = 0;
    std::int32_t listlineno_ = 0;
    int suppress_ = 0;               // for INCBIN & TIMES special cases
    int listlevel_ = 0, listlevel_e_ = 0;
};

}  // namespace cb::build::x86asm
