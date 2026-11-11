// cb/verify/gate16/contract16.hpp -- the calls of each function rebuilt from C beside the original's: per callee, the
// bytes the caller takes off the stack after the call.
//
// The original's callers say how each callee is to be called -- one that removes its own arguments is called without
// the caller taking them off -- and a rebuilt function that takes off what the callee already removed (or leaves what
// it does not) leaves the stack wrong for everything after it.
//
// The isolated check (isolated16) answers a call as the description says the callee does, so it agrees with any belief
// the description has; this reads both modules' instructions as they are and so sees a wrong belief.
//
//     std::map<std::string, std::string> why = mismatches(original, built, link_map, names);
//
// `original` and `built` are the two modules as the describer sees them (`Described`: the relocated image of each
// segment, the functions' names, the description's text). Describing a module is the describer's work and is kept by
// whoever runs it (a module's description is the same for every build of it); this module only reads what it says.
//
// The 16-bit instruction decoder is the platforms' (cb/platforms/decoder.hpp); an engine is not shared between
// threads, so `calls` takes the caller's.
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/platforms/decoder.hpp"

namespace cb::verify::gate16::contract16 {

// ---- what a call's target is -------------------------------------------------------------------------------------

// A near call: the segment it is in and the offset it goes to.
struct NearCall {
    unsigned segment = 0;
    std::uint16_t offset = 0;
    friend auto operator<=>(const NearCall&, const NearCall&) = default;
};
// A far call: the address (selector << 16 | offset) it goes to, as the describer addresses functions.
struct FarCall {
    std::uint32_t address = 0;
    friend auto operator<=>(const FarCall&, const FarCall&) = default;
};
using CalleeKey = std::variant<NearCall, FarCall>;

// The bytes a caller takes off the stack after a call: a count, or an operand the count is read from (`add sp, bx`),
// or "whatever the frame's LEAVE restores" (the comparison of two builds treats that as saying nothing).
struct TakenOff {
    enum class Kind : std::uint8_t { Bytes, Operand, Leave };
    Kind kind = Kind::Bytes;
    std::int64_t bytes = 0;     // Kind::Bytes
    std::string operand;        // Kind::Operand: the text of the operand, as the decoder prints it
    std::string text() const;   // "4", "?bx", "leave"
    friend auto operator<=>(const TakenOff&, const TakenOff&) = default;
};

// One direct call found in a function's code.
struct Call {
    std::string callee;
    TakenOff taken_off;                    // Bytes or Operand; whatever the caller removes right after the call
    // The argument bytes pushed for this call: a count; a negative count means "at least that many"; none when it is
    // not known.
    std::optional<std::int64_t> pushed;
    bool at_leave = false;                 // the frame's LEAVE follows, so the stack is set back from BP whatever was taken off
    friend bool operator==(const Call&, const Call&) = default;
};

// The name of the function a call goes to (none: not known), and the bytes that function removes itself (its `retf n`;
// none: not known). The callee's name is what the two modules are matched by.
using NameOf = std::function<std::optional<std::string>(const CalleeKey&)>;
using Removes = std::function<std::optional<std::int64_t>(const CalleeKey&)>;

// The direct calls of the code in [lo, hi) of segment `segment` of `image`, in order. `hi` past the end of the image
// is the end of the image (a function runs to the next one or the segment's end). Calls through a pointer have no
// callee here and are not listed; a call whose callee has no name is not either.
std::vector<Call> calls(const platforms::Decoder& decoder, ByteView image, unsigned segment, std::size_t lo,
                        std::size_t hi, const NameOf& name_of, const Removes& removes = {});

// ---- the two modules -----------------------------------------------------------------------------------------------

// The selector of a segment in a described module (describe.py FIRST_SELECTOR: moves to describe.hpp when that module
// is refactored) and the address of an offset in it.
inline constexpr std::uint32_t kFirstSelector = 0x1000;
constexpr std::uint32_t address_of(unsigned segment, std::uint32_t offset) {
    return ((kFirstSelector + 8 * (segment - 1)) << 16) + offset;
}
// Where an address is in the module's segments. An address outside them (an import's stand-in, a selector below the
// first) has a segment no module has: <= 0 or past its last.
struct Place {
    std::int64_t segment = 0;
    std::uint32_t offset = 0;
    friend auto operator<=>(const Place&, const Place&) = default;
};
Place place_of(std::uint32_t address);

// How the function at `offset` of `code` returns: the bytes of arguments it removes, or none when no return is met or
// it is not a count a function removes. `far`: the return is a `retf`. This is describe's `returns`.
using ReturnProbe = std::function<std::optional<std::int64_t>(ByteView code, std::uint32_t offset, bool far)>;

// A module as the describer describes it.
struct Described {
    std::map<unsigned, Bytes> images;                // by segment number: the relocated image
    std::map<std::uint32_t, std::string> names;      // function address -> its name
    std::string description;                         // the description's text (its records are tab separated)
    ReturnProbe returns;                             // the nearest return of a function (see above)
};

// What the description says of the functions it names: the X (import) and F (function) records.
struct DescribedFacts {
    std::map<std::uint32_t, std::string> names;                       // address -> name
    // address -> bytes the function removes; none: the description says it is not known (a negative count)
    std::map<std::uint32_t, std::optional<std::int64_t>> pops;
};
DescribedFacts parse_description(std::string_view text);

// A describer: the module at a path, described (the description is kept by whoever provides this).
using Describer = std::function<Described(const std::filesystem::path& module_path)>;

// The functions of `names` whose calls take off different bytes, or push a different count of argument bytes, than the
// original's calls to the same callee: {function name: why}. `link_map` is the text of the build's map file (the
// symbols of each segment: "ssss:oooo name" lines, with the linker's leading "_" and trailing "@n" decoration removed);
// only functions named there and in the original are compared. A call through a function that takes its arguments in
// registers is a mismatch of its own (C sets none of them).
std::map<std::string, std::string> mismatches(const Described& original, const Described& built, std::string_view link_map,
                                              std::span<const std::string> names);

// The same for the modules at two paths, with the build's folder holding `m.map`: no map (the build did not link) is no
// mismatch.
std::map<std::string, std::string> mismatches(const Describer& describe, const std::filesystem::path& original,
                                              const std::filesystem::path& folder, const std::filesystem::path& image,
                                              std::span<const std::string> names);

}  // namespace cb::verify::gate16::contract16
