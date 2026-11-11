// cb/build/stubjoin.hpp -- a Windows module put behind a DOS program of its own, as one file.
//
//     Bytes file = stubjoin::join(stub, built, at);
//
// Every Windows module starts with a DOS program, its stub; usually the linker's own two-line "cannot be run in
// DOS mode". Some carry a real one -- REGEDIT's is a registry editor for DOS, XCOPY32's and UNINSTAL's do their
// work from DOS too -- and those are built as programs of their own (source/dos_writer). join() takes the module
// as OpenWatcom linked it, with its own stub, and puts it behind that program: its new header at `at` (the
// original's place, when the program is no longer than that), and every offset the module gives from the start
// of the FILE moved with it.
//
//   PE   the headers (signature, file header, optional header, section table) at `at`, the section data from
//        the next file-alignment boundary; moved: each section's raw data, relocations and line numbers, the
//        symbol table, the debug data, the certificates; SizeOfHeaders
//   NE   the header and its tables at `at`, the segments and resources from the next boundary of the module's
//        alignment unit; moved: each segment's and resource's sector, the non-resident name table
//
// A field that lies outside the module, or a moved offset that no longer fits its field, is a FormatError
// (TruncatedError for a read past the end); a module that is neither PE nor NE, or whose resources are aligned
// differently from its segments, is Unsupported.
#pragma once

#include <cstdint>
#include <optional>

#include "cb/core/bytes.hpp"

namespace cb::build::stubjoin {

// `stub`: the DOS program (an MZ file, its new-header field at 3Ch to be set); `built`: the Windows module as
// linked; `at`: where the module's header goes (used when it is not before the end of the stub, else the next
// 16-byte boundary after it). -> the file.
Bytes join(ByteView stub, ByteView built, std::optional<std::uint32_t> at = std::nullopt);

}  // namespace cb::build::stubjoin
