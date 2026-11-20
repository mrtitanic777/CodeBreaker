// build/x86link/directives/language.hpp -- wlink's directive language over the reader (reader.hpp): the keyword tables
// every directive and option is looked up in, a list of arguments, numbers and versions. A port of wlink's cmdutils
// (procOne, MatchOne, ProcArgListEx, getatol, GetGenVersion ...), typed.
//
// A keyword is written with the part that must be typed in capitals: "STack" is matched by `st`, `sta`, `stac` and
// `stack`, in any case, and by nothing longer.
#pragma once

#include <cstdint>
#include <span>
#include <string>
#include <string_view>

#include "build/x86link/directives/reader.hpp"
#include "linkstd.h"

namespace cb::x86link::directives {

// What a matched directive or option says about the directives after it (wlink's commandflag).
enum class Flag : std::uint32_t {
    none = 0,
    set_section = 0x00000001,        // a LIB or FIXEDLIB directive
    no_default_libs = 0x00000002,
    files_before_debug = 0x00000004,
    unnamed = 0x00000008,
    auto_segment_flag = 0x00000010,  // (OS/2)
    member_added = 0x00000020,
    separate_symbols = 0x00000040,
    auto_section = 0x00000080,
    section_there = 0x00000100,
    have_files = 0x00000200,
    have_realbreak = 0x00000400,
    language_english = 0x00000000,
    language_japanese = 0x00000800,
    language_chinese = 0x00001000,
    language_korean = 0x00001800,
    language_mask = 0x00001800,
    anonymous_export = 0x00002000,
    after_incremental = 0x00004000,  // an option that must come after `option incremental`
    doing_optlib = 0x00008000,
    no_extension = 0x00010000,       // the executable's name keeps no extension
    subset = 0x00020000,             // the directive may be in a system block run after the command line
};
constexpr Flag operator|(Flag a, Flag b) { return static_cast<Flag>(static_cast<std::uint32_t>(a) | static_cast<std::uint32_t>(b)); }
constexpr Flag operator&(Flag a, Flag b) { return static_cast<Flag>(static_cast<std::uint32_t>(a) & static_cast<std::uint32_t>(b)); }
constexpr Flag operator~(Flag a) { return static_cast<Flag>(~static_cast<std::uint32_t>(a)); }
constexpr Flag& operator|=(Flag& a, Flag b) { return a = a | b; }
constexpr Flag& operator&=(Flag& a, Flag b) { return a = a & b; }
constexpr bool any(Flag f) { return f != Flag::none; }

using Handler = bool (*)();

// One keyword of a table: what it runs, the formats it is valid for, and the flags it sets.
struct Entry {
    std::string_view keyword;   // (a literal: its text is followed by a '\0', for the messages that name it)
    Handler handler;
    exe_format formats;
    Flag flags = Flag::none;
};
using Table = std::span<const Entry>;

// The flags the directives read so far set (the link's; state.cpp).
Flag& cmd_flags();

// The next token, with `required` before it, looked up in the table: its handler's answer, or false with the token
// pushed back when it is not there (it may be the next table's). A keyword not valid for the format decided is warned
// of and taken. `process_one_subset` looks only at the entries flagged `subset`; `process_one_or_fail` stops the link
// (a syntax error) when the token is not there.
bool process_one(Table table, Sep required);
bool process_one_subset(Table table, Sep required);
bool process_one_or_fail(Table table, Sep required);
bool matches(Table table, std::string_view word);   // the word is a keyword of the table

// A list of arguments, `{a b c}` or `a, b, c`, the handler run for each (a wildcard in a file name: for each file it
// matches). With a reset point, the list ends at the end of that source's file (export files).
bool process_list(Handler handler, Want want, const Source* reset_point = nullptr);

// The current token as a number: decimal, 0x hexadecimal, and a k or m suffix (times 1024, 1024*1024).
enum class Number : std::uint8_t { ok, not_a_number, too_big };
Number number(std::uint32_t& value);
Number number16(std::uint16_t& value);   // ... no more than 0xffff
// `=` and a token after it (false, with the token read anyway, when there is no `=`); and `=` with a number.
bool have_equals(Want want);
bool get_number(std::uint32_t& value);

// A version `=major[.minor[.revision]]`: the limits in, the values out (0 for none given); `wanted` says which parts are
// read, the result which were. A Novell revision may be a letter (a is 1 ... z is 26).
enum class VersionPart : std::uint8_t { none = 0, major = 1, minor = 2, revision = 4 };
constexpr VersionPart operator|(VersionPart a, VersionPart b) { return static_cast<VersionPart>(static_cast<unsigned>(a) | static_cast<unsigned>(b)); }
constexpr bool has(VersionPart v, VersionPart bit) { return (static_cast<unsigned>(v) & static_cast<unsigned>(bit)) != 0; }
struct Version {
    std::uint32_t major = 0, minor = 0, revision = 0;
    const char* what;   // the option, for the message a bad value gives
};
VersionPart get_version(Version& v, VersionPart wanted, bool novell_revision);

// The current token as an object file's name, `lib(member)` a library's member: the file's name (wlink's FileName,
// with its default extension: an object's, or a library's for a member) and the member, or null. Both are wlink's
// memory (MemAlloc), as the object lists that keep them want.
char* file_name(char** member);

// Directives are being read from a system block (the current source or one under it).
bool in_system_block();

}  // namespace cb::x86link::directives
