// cb/exact/fixupp.hpp -- the fix-ups of a 16-bit OMF object as the FIXUPP records state them: every field of each
// one, the thread definitions resolved, and the same fields written again.
//
// formats/omf reads a module for what it means (a word of a segment that names a target); the exact lanes that cut or
// rebuild an object (recut, the stand-ins) must write the fix-ups back as the compiler's record listed them, so they
// need the stored fields: the location, the frame and target methods with their indices, the displacement.
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::exact::fixupp {

// One fix-up. `offset` is its word's place in the data record the FIXUPP record follows (10 bits as stored); `at`
// is the same place in the whole segment, which the caller fills in once it knows which data record that was.
struct Fixup {
    bool segment_relative = false;              // the M bit: the word is segment-relative (else self-relative)
    std::uint8_t location = 0;                  // 0 low byte, 1 offset, 2 base, 3 pointer ... (the 4-bit field as stored)
    std::uint16_t offset = 0;
    std::uint8_t frame_method = 0;              // 0-2 name their frame by an index; the others need none
    std::optional<std::uint16_t> frame_index;   // present for frame methods 0-2
    std::uint8_t target_method = 0;
    std::uint16_t target_index = 0;
    bool no_displacement = true;                // the P bit: the target displacement is zero and not stored
    std::uint16_t displacement = 0;
    std::uint32_t at = 0;
};

// The thread definitions met so far (THREAD subrecords of the FIXUPP records of one module); a fix-up may name one
// instead of stating its frame or target. They persist from one record to the next.
class Threads {
public:
    struct Def {
        std::uint8_t method = 0;
        std::optional<std::uint16_t> index;
    };
    void set(bool frame, std::uint8_t number, Def def) { defs_[frame ? 1 : 0][number & 3] = def; }
    // FormatError when the fix-up names a thread no record defined.
    const Def& get(bool frame, std::uint8_t number) const;

private:
    std::array<std::array<std::optional<Def>, 4>, 2> defs_;
};

// An OMF index: one byte below 0x80, else two (the high byte with bit 7 set, then the low byte).
struct Index {
    std::uint16_t value = 0;
    std::size_t next = 0;   // where the next field begins
};
// TruncatedError when the record ends inside it.
Index read_index(ByteView record, std::size_t pos);
// The index as stored. FormatError past 0x7FFF.
Bytes index_bytes(std::uint16_t value);

// The fix-ups of one FIXUPP (0x9C) record's payload; thread definitions in it are added to `threads`.
// FormatError for a record that names a thread nobody defined, TruncatedError for one that ends inside a field.
std::vector<Fixup> parse(ByteView payload, Threads& threads);

// A FIXUPP record (checksum included) holding `fixups`, each at its `at`, for a data record beginning at `base`.
// Every place must lie in [base, base + 1024).
Bytes encode(std::span<const Fixup> fixups, std::uint32_t base);

}  // namespace cb::exact::fixupp
