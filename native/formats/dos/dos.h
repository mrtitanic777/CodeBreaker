// dos.h -- real-mode programs: what DOS, or the boot sector, would load (MZ programs, .COM files,
// device drivers, the IO.SYS kernel image), and EXEPACK undone statically.
//
// The C++ form of codebreaker/formats/dos.py.  A program packed by anything but EXEPACK (or one
// EXEPACK could not undo statically) is unpacked by running its unpacker in the emulator, which
// stays with the Python reader: such a read is a Fallback.
#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

#include "../core/bytes.h"

namespace cbf::dos {

constexpr int64_t kLoad = 0x1000;            // the segment a program is loaded at
constexpr int64_t kKernelSegment = 0x0070;   // where the boot sector and the loader put IO.SYS
constexpr int64_t kLoaderSegment = 0x9000;   // where the loader moves itself
constexpr int64_t kSysinitSegment = 0x4000;  // a free segment for SYSINIT
constexpr int64_t kStubProgram = 0x400;      // a DOS stub this big is a program of its own

struct Mz {                                  // _mz(): the header's fields
    int64_t nreloc, header, min_alloc, max_alloc, ss, sp, ip, cs, reloc_off, size;
};

struct Device {
    int64_t offset;
    std::optional<int64_t> next;
    int64_t attributes, strategy, interrupt;
    std::string name;
    bool character() const { return attributes & 0x8000; }
};

struct Entry {
    int64_t address;
    std::string name, kind;                  // kind: "far" | "near"
};

struct Span {
    int64_t address, length;
    std::string label;
};

struct Program {                             // dos.Program's attributes (path and name: the caller's)
    std::string kind;
    std::optional<std::string> name;         // set only where the Python sets it apart from the path
    std::optional<std::string> packer, unpacked;
    int64_t base = kLoad * 16;
    std::string image;
    std::vector<Span> spans;
    std::vector<Entry> entries;
    std::vector<std::pair<int64_t, int64_t>> segments;   // a dict: entry address -> its code segment
    std::vector<int64_t> relocations;
    std::vector<Device> devices;
    int64_t device_segment = kLoad;
    int64_t cs = 0, ip = 0, ss = 0, sp = 0;
    std::optional<int64_t> ds = kLoad;
    std::string overlay;
    std::vector<std::string> notes;
    std::optional<int64_t> windows_at;

    bool inside(int64_t linear) const;
    bool has_segment(int64_t va) const;
};

struct Exepack {                             // unpack_exepack()
    std::string module;
    std::vector<int64_t> relocs;
    int64_t cs, ip, ss, sp;
};

std::optional<Mz> mz(Bytes data, bool stub = false);
std::vector<Device> devices(Bytes image, int64_t at = 0, const std::vector<int64_t> &segments = {0, 0xFFFF}, int64_t limit = 64);
std::optional<std::pair<int64_t, int64_t>> kernel_body(Bytes data, const Mz &h);
std::optional<std::string> kind_of(Bytes data, const std::string &name);
std::vector<std::pair<int64_t, int64_t>> chain(Bytes data);
std::optional<std::pair<int64_t, int64_t>> driver_and_vxd(Bytes data);
std::optional<std::pair<Mz, int64_t>> dos_and_windows(Bytes data);
std::optional<std::string> packer_of(Bytes data, const Mz &h);
Exepack unpack_exepack(Bytes module, int64_t cs, int64_t ip);
std::optional<int64_t> data_segment(Bytes image, int64_t base, int64_t ip, const std::string &kind,
                                    const std::vector<int64_t> &relocations, int64_t entry);

// read(path): the program; Fallback when its unpacker must run in the emulator
Program read(Bytes data, const std::string &basename, bool run_movers);
Program driver_part(Bytes data);
Program pklite_head(Bytes data, int64_t start);
// unpacked_file(p): a packed program written back as a plain MZ file
std::string unpacked_file(const Program &p, const std::string &name);

}  // namespace cbf::dos
