// formats/detect.hpp -- what kind of file a file is.
//
// The MZ family (DOS, NE, PE, LE/LX, the W3/W4 VxD bundles) is told apart by the "new header" signature the DOS stub
// points at; `lane` says which reader takes the file. `identify` recognises any other file CodeBreaker knows
// (Xbox and Xbox 360 formats, archives, images, disc images ...) by its magic bytes. Files are mapped, not read, so
// asking about a big image is cheap; an unreadable file is an ordinary "unknown" answer.
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <initializer_list>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "core/bytes.hpp"

namespace cb::formats::detect {

// The signature at the DOS stub's new-header offset.
enum class NewHeader { None, Pe, Ne, Le, Lx, W3, W4 };
const char* signature_text(NewHeader sig);  // "PE" ... "W4"; "" for none

// The reader a file goes to.
enum class Lane { Pe, Ne, Le, Dos };
const char* lane_name(Lane lane);  // "pe", "ne", "le", "dos"

// The new-header signature of an MZ file: nullopt when the file is unreadable or not MZ; none when it is MZ with
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

// ---- the descriptions' rules, and what a platform lane adds: its own detector ------------------------------------------
// A description says how its files are told by `detect` rules (config/formats/*.fmt; formats/format_engine.hpp): a priority, a
// condition on the file's bytes and name, the text of the answer. identify runs them with the lanes' code detectors below, by
// priority; a lane keeps code only for what a condition cannot say (a disc's file system, an archive's members, a reader's
// verdict).
// A platform lane (engine/lane_registry.hpp) says what its files are by a detector: the text `info` prints for one of its files. identify
// tries the registered detectors, in the order of their priority (then of registration), after the Xbox and Windows headers and before the
// generic magic table; the first that answers decides. A detector answers nullopt for a file that is not its platform's.
struct DetectInput {
    ByteView file;            // the whole file (mapped)
    ByteView head;            // its first 0x400 bytes (fewer for a short file)
    std::uint64_t size = 0;   // the file's size
    std::string_view name;    // the file's name, for what is told by its extension
};
using Detector = std::function<std::optional<std::vector<Detected>>(const DetectInput&)>;

// Add a detector of `lane`. Lower priorities are tried first; equal ones in the order added.
void add_detector(std::string lane, int priority, Detector detector);
// Take the detectors of `lane` out again (a test's); false when it had none.
bool remove_detectors(std::string_view lane);

// What a detector's answer is made of.
std::vector<Detected> one(std::string format, std::string detail, std::string_view family, std::string_view command);
// Does the file name end in one of the (lower-case) extensions, whatever its case?
bool name_ends_with(std::string_view name, std::initializer_list<std::string_view> extensions);
// The big-endian 32-bit word at `off`, 0 when the file ends before it.
std::uint32_t be32_or_zero(ByteView d, std::uint64_t off);

}  // namespace cb::formats::detect
