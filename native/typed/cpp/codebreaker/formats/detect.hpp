// codebreaker/formats/detect.hpp -- typed C++ of codebreaker.formats.detect
//
// detect.py -- the ONE place that says what kind of executable a file is (formats.dll answers:
// native/formats/detect, docs/NATIVE.md step 4).
//
// Every MZ-family executable starts with a DOS header whose field at 0x3C (e_lfanew, 4 bytes)
// points at the "new" header; its two-byte signature decides the format.
//
//     signature(path)   -> b"PE" | b"NE" | b"LE" | b"LX" | b"W3" | b"W4" | b"" (MZ, no known new
//                          header) | None (not MZ)
//     format_of(path)   -> "PE" | "NE" | "LE" | "LX" | "W3" | "W4" | "MZ" | None
//     lane(path)        -> "pe" | "ne" | "le" | "dos" | None      (the decompiler lane)
//     kind(path)        -> lane(path), and "dos" for a DOS program with no header (.COM, a driver)
//     flat(kind)        -> whether a module of that kind is a flat 32-bit image (PE)
//     wide_resources(kind) -> whether its resources keep their strings wide (a PE resource tree)
//     holds_drivers(path, bundles=(b"W3", b"W4"))  an LE or LX image, or a bundle of them
//
// W3 and W4 are Windows 95's bundles of drivers (VMM32.VXD: VMM and the drivers loaded with it; W4
// is a W3 compressed). formats/le.py reads them. No lane reads a bundle whole: its lane is its
// DOS stub's, "dos".
//     identify(path)    -> [{format, detail, family, command}]: what ANY file is (executables,
//                          Xbox containers and assets, disc and disk images, objects, bytecode)
//
// Typing notes:
//   * native/formats/detect/detect.cc keeps its logic in an anonymous namespace (it only registers
//     its questions with the DLL's registry), so the logic is written out here, step for step, as typed
//     C++ over the same core readers (core/bytes, core/files, core/text).  dos_header()'s dict is the
//     struct DosHeader; identify()'s dicts are the struct Detected.
//   * kind()/flat()/wide_resources() take the kind as Python does: a str or None.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::formats::detect {

// New-header signature of the file at `path` (see top), or None when the file is unreadable or not
// an MZ executable.
std::optional<py::bytes> signature(const py::str& path);

// 'PE' | 'NE' | 'LE' | 'LX' | 'W3' | 'W4' | 'MZ' (plain DOS executable) | None (not an MZ executable).
std::optional<py::str> format_of(const py::str& path);

// The decompiler lane for a file: 'pe', 'ne', 'le' (VxD; LE and LX), 'dos', or None.
std::optional<py::str> lane(const py::str& path);

// lane(path), and for a file with no MZ header 'dos' when it is a DOS program all the same
// (a .COM file, a device driver: formats/dos.py tells), else None.
std::optional<py::str> kind(const py::str& path);

// Whether a module of this kind (as kind() names it) is a flat 32-bit image: one address
// space counted from its image base, run in user mode (PE). A driver (LE) is 32-bit too, but is
// placed object by object and runs privileged.
bool flat(const std::optional<py::str>& kind);

// Whether a module of this kind keeps its resources' strings wide (UTF-16), as a PE resource
// tree does; a 16-bit module's are single-byte.
bool wide_resources(const std::optional<py::str>& kind);

// Whether the file is an LE or LX image (a VxD), or one of the `bundles` of them.
bool holds_drivers(const py::str& path, const py::list<py::bytes>& bundles = py::list<py::bytes>{py::bytes("W3"), py::bytes("W4")});

// The DOS (MZ) header every one of these formats starts with, by field name, with the size of
// the program it loads and its relocation entries; None when `data` is not an MZ executable.
// For a plain DOS program this is the whole description of the file.
struct DosHeader {
    int64_t bytes_in_last_page = 0, pages = 0, relocations = 0, header_paragraphs = 0, header_size = 0, min_alloc = 0,
            max_alloc = 0, ss = 0, sp = 0, checksum = 0, ip = 0, cs = 0, relocation_table = 0, overlay = 0, new_header = 0,
            load_size = 0;
    py::list<std::tuple<int64_t, int64_t>> relocation_entries;   // (segment, offset)
};
std::optional<DosHeader> dos_header(const py::bytes& data);

// One thing a file can be.
struct Detected {
    py::str format, detail, family, command;
};

// What the file at `path` is, from its own bytes. -> [{"format", "detail", "family", "command"}]
// (usually one; empty when nothing is recognised). `command` is the codebreaker command that
// reads it ("" when none does yet). Reads the header and a few fixed offsets, never the file.
py::list<Detected> identify(const py::str& path);

}  // namespace codebreaker::formats::detect
