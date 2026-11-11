// cb/formats/detect.hpp -- what kind of file a file is.
//
// The MZ family (DOS, NE, PE, LE/LX, the W3/W4 VxD bundles) is told apart by the "new header" signature the DOS stub
// points at; `lane` says which reader takes the file. `identify` recognises any other file CodeBreaker knows
// (Xbox and Xbox 360 formats, archives, images, disc images ...) by its magic bytes. Files are mapped, not read, so
// asking about a big image is cheap; an unreadable file is an ordinary "unknown" answer.
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::formats::detect {

// The signature at the DOS stub's new-header offset.
enum class NewHeader { None, Pe, Ne, Le, Lx, W3, W4 };
const char* signature_text(NewHeader sig);  // "PE" ... "W4"; "" for None

// The reader a file goes to.
enum class Lane { Pe, Ne, Le, Dos };
const char* lane_name(Lane lane);  // "pe", "ne", "le", "dos"

// The new-header signature of an MZ file: nullopt when the file is unreadable or not MZ; None when it is MZ with
// no known new header (a plain DOS program). `read_at` is how a signature beyond `head` is read.
std::optional<NewHeader> new_header(const std::filesystem::path& file);
std::optional<NewHeader> new_header(ByteView whole_file);
Lane lane_of(NewHeader sig);

std::optional<Lane> lane(const std::filesystem::path& file);
// "MZ" for a plain DOS program, else the signature ("PE", "NE", ...); nullopt for a non-MZ or unreadable file.
std::optional<std::string> format_of(const std::filesystem::path& file);
// Like `lane`, but also tells a real-mode program or driver that is not MZ (a .COM file, a bare driver) -> Dos.
std::optional<Lane> kind(const std::filesystem::path& file);
// Whether the lane's code is addressed flat (a PE), and whether its resource strings are UTF-16.
inline bool flat(std::optional<Lane> kind) { return kind == Lane::Pe; }
inline bool wide_resources(std::optional<Lane> kind) { return kind == Lane::Pe; }
// Whether the file holds VxD drivers: an LE/LX, or one of the `bundles` (W3 and W4 by default).
bool holds_drivers(const std::filesystem::path& file, bool w3_bundles = true, bool w4_bundles = true);

// The MZ header, every field, with the relocation table's entries as (segment, offset).
struct DosHeader {
    std::uint16_t bytes_in_last_page = 0, pages = 0, relocations = 0, header_paragraphs = 0;
    std::uint32_t header_size = 0;  // header_paragraphs * 16
    std::uint16_t min_alloc = 0, max_alloc = 0, ss = 0, sp = 0, checksum = 0, ip = 0, cs = 0, relocation_table = 0, overlay = 0;
    std::uint32_t new_header = 0;  // e_lfanew, 0 when the header is shorter than 0x40
    std::uint64_t load_size = 0;   // bytes the loader would read as the program
    struct Relocation {
        std::uint16_t segment = 0, offset = 0;
    };
    std::vector<Relocation> relocation_entries;  // those that lie inside the file
};
std::optional<DosHeader> dos_header(ByteView data);

// What a file is: its format, a detail, the family it belongs to and the CodeBreaker command that reads it.
struct Detected {
    std::string format, detail, family, command;
};
// Nothing for an unknown, unreadable or directory path.
std::vector<Detected> identify(const std::filesystem::path& file);
// The same on bytes that are already in memory; `name` is only used for files told by extension (.zar, .pck).
std::vector<Detected> identify(ByteView whole_file, std::string_view name);

}  // namespace cb::formats::detect
