// cb/invert/quad_dump.hpp -- structural parser for Lattice C 2.12 quad files (<base>.q).
//
// Record framing (from LC2's reader at 0x28c5, confirmed by tiling every sample): each quad record is
// [opcode:1][length:1][payload: `length` bytes]. The reader reads the two bytes, then `length` more, then parses up
// to three operands from the payload. A function is a run of records starting at FUNC-BEGIN (0x23); the module ends
// at a 0xff terminator, after which come symbol trailer bytes and the NUL-terminated name pool.
//
// Opcode map (differentially verified by dynamic analysis; see re/notes/05-quad-format-and-harness.md and
// 07-quad-framing.md): unary 0x0a-0x0c, binary 0x10-0x1f, statements and structure from 0x20.
//
// `scan` walks the records of any file (nothing is required of the header); `parse` adds the header fields the dump
// shows and so needs the first 0x50 bytes. Records and names view into / copy out of the file the caller holds.
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::invert::quad_dump {

inline constexpr std::size_t kFirstRecord = 0x5c;   // the records start here, after the header
inline constexpr std::uint8_t kFuncBegin = 0x23;    // the record that opens a function
inline constexpr std::uint8_t kTerminator = 0xff;   // ends the records; not a quad

struct Record {
    std::size_t offset = 0;   // of the opcode byte
    std::uint8_t opcode = 0;
    ByteView payload;         // `length` bytes after the two-byte head
};

// What a walk of the records found.
struct Scan {
    std::size_t pool_offset = 0;       // where the name pool starts (see namepool_start)
    std::vector<Record> records;       // up to, not including, the terminator
    struct Terminated {
        std::size_t offset = 0;
        std::uint8_t length = 0;       // the length byte the terminator carries (meaningless, but shown)
    };
    std::optional<Terminated> terminator;   // absent: the records ran into the name pool without one
    std::size_t end_offset = 0;        // where the walk stopped: the terminator's offset, or the first byte not walked
    // A record whose head promises more payload than the file holds: its offset. The walk stops there and the
    // record is not in `records`.
    std::optional<std::size_t> overrun;
};

// The offset where the trailing run of printable ASCII and NUL bytes (the name pool) starts.
std::size_t namepool_start(ByteView file);

// Walk the records from kFirstRecord to the terminator or the name pool. Never throws: a file too short to hold any
// record has none.
Scan scan(ByteView file);

// A file's header fields and its scan.
struct File {
    std::uint16_t magic = 0;                          // offset 0x00
    std::array<std::uint16_t, 10> type_sizes{};       // 0x04: the type-size table
    std::uint16_t symbol_count = 0;                   // 0x4e
    std::vector<std::string> names;                   // the NUL-separated names of the pool (empty ones dropped)
    Scan scan;
    std::size_t size = 0;                             // the file's length
};
// TruncatedError if the file is shorter than the 0x50 bytes the header fields need.
File parse(ByteView file);

// The mnemonic of an opcode; empty for one the map does not know.
std::string_view opcode_name(std::uint8_t opcode);
// The mnemonic, or "opNN" for an unknown opcode.
std::string opcode_label(std::uint8_t opcode);
// A payload as hex bytes separated by spaces. (Operands are packed little-endian words.)
std::string format_payload(ByteView payload);

// The dump the tool prints: the header, then one line per record. `label` is the file's name in the first line.
std::string dump(const File& file, std::string_view label);

// What the tool prints when it is given no file: its description and the opcode map.
std::string usage();

}  // namespace cb::invert::quad_dump
