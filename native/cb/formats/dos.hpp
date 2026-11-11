// cb/formats/dos.hpp -- real-mode programs: what DOS, or the boot sector, would load (MZ programs, .COM files, device
// drivers, the IO.SYS kernel image), with packed programs unpacked.
//
// `read_program` classifies a file (`kind_of`) and lays it out as the machine would see it at its load segment: the
// image, its relocations, the entry points, the device-driver headers it carries, the registers it starts with.
// EXEPACK is undone statically; a program packed by anything else (LZEXE, PKLITE, DIET, UPX ...) is unpacked by
// running its own unpacker in the emulator (dos_unpack.cpp) until it jumps to the real entry point.
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/ordered_map.hpp"

namespace cb::formats::dos {

inline constexpr std::uint32_t kLoadSegment = 0x1000;    // the segment a program is loaded at
inline constexpr std::uint32_t kKernelSegment = 0x0070;  // where the boot sector and the loader put IO.SYS
inline constexpr std::uint32_t kLoaderSegment = 0x9000;  // where the loader moves itself
inline constexpr std::uint32_t kSysinitSegment = 0x4000; // a free segment for SYSINIT
inline constexpr std::uint32_t kStubProgram = 0x400;     // a DOS stub this big is a program of its own

enum class Kind {
    Com,
    Mz,
    Driver,          // a device driver (a header chain at the start), or one linked as an .EXE
    Kernel,          // IO.SYS: loader, BIOS+DOS body and SYSINIT in one MZ file
    KernelPart,      // the head of a PKLITE-packed kernel
    DriverWithVxd,   // an MZ file holding a DOS driver and a Windows VxD
    DosWithWindows,  // a DOS program stub in front of a Windows module
};
const char* kind_name(Kind kind);  // "com", "mz", "driver", "kernel", "kernel part", "driver+vxd", "dos+windows"

enum class Packer { ExePack, LzExe, PkLite, Diet, Upx, MovesItself };
const char* packer_name(Packer packer);  // "EXEPACK", "LZEXE", "PKLITE", "DIET", "UPX", "moves itself"
enum class Unpacked { Static, Emulated };

// The MZ header's fields. Sizes are in bytes.
struct MzHeader {
    std::uint16_t relocation_count = 0;
    std::uint32_t header_size = 0;  // header paragraphs * 16
    std::uint16_t min_alloc = 0, max_alloc = 0, ss = 0, sp = 0, ip = 0, cs = 0, relocation_offset = 0;
    std::uint64_t size = 0;  // the program's size in the file (header and image), cut to the file
};

struct Device {
    std::uint32_t offset = 0;  // of this header in the image
    std::optional<std::uint16_t> next;  // absent: the last in the chain
    std::uint16_t attributes = 0, strategy = 0, interrupt = 0;
    std::string name;  // a character device's 8-byte name; "block device (N units)"
    bool character() const { return (attributes & 0x8000) != 0; }
};

struct Entry {
    enum class Call { Far, Near };
    std::uint32_t address = 0;  // linear
    std::string name;
    Call call = Call::Far;
};

struct Span {
    std::uint32_t address = 0, length = 0;
    std::string label;
};

struct Program {
    Kind kind = Kind::Mz;
    std::optional<Packer> packer;
    std::optional<Unpacked> unpacked;
    std::uint32_t base = kLoadSegment * 16;  // linear address of image[0]
    Bytes image;
    std::vector<Span> spans;
    std::vector<Entry> entries;
    OrderedMap<std::uint32_t, std::uint32_t> segments;  // entry address -> its code segment
    std::vector<std::uint32_t> relocations;  // linear addresses of the words that hold segments
    std::vector<Device> devices;
    std::uint32_t device_segment = kLoadSegment;
    std::uint32_t cs = 0, ip = 0, ss = 0, sp = 0;
    std::optional<std::uint32_t> ds = kLoadSegment;
    Bytes overlay;  // bytes after the program in the file
    std::vector<std::string> notes;
    std::optional<std::uint64_t> windows_at;  // dos+windows: where the Windows module's header is

    bool inside(std::uint64_t linear) const;
    bool has_segment(std::uint32_t address) const { return segments.contains(address); }
};

// What `unpack_exepack` / the emulated unpacker give: the unpacked load module and where it starts.
struct Unpack {
    Bytes module;
    std::vector<std::uint32_t> relocations;  // offsets into `module` of segment words
    std::uint32_t cs = 0, ip = 0, ss = 0, sp = 0;  // relative to the load segment (cs, ss)
};

std::optional<MzHeader> mz_header(ByteView data, bool stub = false);
std::vector<Device> devices(ByteView image, std::uint64_t at = 0,
                            const std::vector<std::uint32_t>& segments = {0, 0xFFFF}, std::size_t limit = 64);
// Where IO.SYS's BIOS+DOS body starts in the file, and its first device header; nullopt if it is not a kernel.
std::optional<std::pair<std::uint64_t, std::uint64_t>> kernel_body(ByteView data, const MzHeader& h);
std::optional<Kind> kind_of(ByteView data, std::string_view name);
// (offset, size) of each program of a file made of MZ programs in a row; empty unless there are several.
std::vector<std::pair<std::uint64_t, std::uint64_t>> chain(ByteView data);
// The DOS driver part and the offset of the LE header of a driver+vxd file.
std::optional<std::pair<std::uint64_t, std::uint64_t>> driver_and_vxd(ByteView data);
std::optional<std::pair<MzHeader, std::uint64_t>> dos_and_windows(ByteView data);
std::optional<Packer> packer_of(ByteView data, const MzHeader& h);
// EXEPACK undone by reading the packed data. FormatError when it is not EXEPACK or the data is damaged.
Unpack unpack_exepack(ByteView module, std::uint32_t cs, std::uint32_t ip);
// A packed load module unpacked by running its unpacker to the real entry point (two runs, at two load segments,
// tell relocations apart). EmulationError when no entry point is reached or the runs disagree.
Unpack unpack_emulated(ByteView module, const std::vector<std::uint32_t>& relocations, std::uint32_t cs, std::uint32_t ip,
                       std::uint32_t ss, std::uint32_t sp, bool only_moved, std::uint64_t instruction_limit = 60000000);
// The segment the program sets DS to near its start, if it can be told from the code.
std::optional<std::uint32_t> data_segment(ByteView image, std::uint32_t base, std::uint32_t ip, Kind kind,
                                          const std::vector<std::uint32_t>& relocations, std::uint32_t entry);

// The program in `data`: FormatError when it is not a real-mode program. `name` (the file name) tells .COM files
// from other raw images. `run_movers` lets a program that only moves itself be run to see where it ends up.
Program read_program(ByteView data, std::string_view name, bool run_movers = true);
// The MZ file read as a plain program (whatever else it is: a kernel image's SYSINIT is read this way).
Program read_load_module(ByteView data, bool run_movers = true);
// The DOS driver part of a driver+vxd file.
Program read_driver_part(ByteView data);
// The unpacker head of a PKLITE-packed kernel: `start` bytes after the MZ header.
Program read_pklite_head(ByteView data, std::uint64_t start);
// A packed program written back as a plain MZ file (the relocations put back as a table).
Bytes unpacked_file(const Program& p, std::string_view name);

}  // namespace cb::formats::dos
