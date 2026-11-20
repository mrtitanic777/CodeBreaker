// exact/emu/dos.hpp -- run a real-mode DOS program under Unicorn, in this process, with just enough of DOS behind
// INT 21h for the compilers, assemblers and linkers of the 1980s: files (handles and FCBs), memory blocks, console,
// EXEC, version, time.
//
// It is an ORACLE and a TRACER, not part of the reconstruction: the original binary assembles real source here, and
// every basic block it executes is recorded, so the reconstruction can be checked against behaviour (which function runs
// for which statement; what the output is).
//
// The executive itself is exact/emu/dosexec (linked in on Windows: the PSP, the MCB arena, program load, INT 20h/21h over the MemFS or
// the disk, EXEC, the console and the clock, and the DPMI host of cb/exact/emu/dpmi). What is here is what surrounds it:
//
//   * `Executive`  loads the DLL and runs a program - the arguments in a `RunSpec`, what it did in a `Result` - handing it
//                  the `MemFS` and taking back what the run changed in it;
//   * `MemFS`      the files a run sees, kept in memory: what the program writes stays here (its passes share them), what
//                  it reads and was not written comes from the disk - read once, kept for every run after (`DiskCache`) -
//                  except under the folders hidden (an include folder given whole each run). No file is written to the
//                  disk;
//   * the arena    DOS's memory control blocks, kept in guest RAM the way DOS keeps them: every block is preceded by a
//                  16-byte header (+0 'M' (another block follows) or 'Z' (the last), +1 owner PSP (0 free, 8 DOS), +3 size
//                  in paragraphs (not counting the header), +8 name; the next header is at mcb + 1 + size). Programs read
//                  this chain themselves (a DOS extender's loader walks it from its PSP - 1 to size the memory it may
//                  take), so it is the one source of truth: INT 21h 48h/49h/4Ah work on it directly, as DOS does. The
//                  functions here are the arena's operations on an `Engine`'s memory (the host side of the same arena
//                  serves the DPMI host's allocate/free/resize).
//
// `blocks` of a Result are the linear addresses of executed basic blocks; (addr - load_seg * 16) is an offset into the
// program's load image (for an EXEPACK'd program: into the UNPACKED image, since the unpacker runs there too and unpacks in
// place).
#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "core/bytes.hpp"
#include "core/error.hpp"
#include "exact/emu/dpmi.hpp"

namespace cb::exact::emu::dos {

inline constexpr std::uint16_t kPsp = 0x0100;      // PSP segment; the program loads at kPsp + 0x10 = 0x0110
inline constexpr std::uint16_t kTop = 0xA000;      // first segment past conventional memory
inline constexpr std::uint16_t kEnvironmentSegment = 0x00C0;   // the environment block (its header at one before is the first in the chain)
inline constexpr std::uint16_t kDosSegment = 0x0080;           // INT 21h 52h's list of lists (first MCB at -2)
inline constexpr std::uint16_t kListOffset = 0x0026;
inline constexpr std::uint16_t kListSize = 0x40;
inline constexpr std::uint16_t kOwnerFree = 0;
inline constexpr std::uint16_t kOwnerDos = 8;
inline constexpr std::uint64_t kDefaultMaxBlocks = 60'000'000;   // a run stops after this many basic blocks (a run that counts them)

// ---- the memory arena ----------------------------------------------------------------------------------------------

// A DOS memory error: the DOS error code (7 arena trashed, 9 bad block).
class ArenaError : public Error {
public:
    explicit ArenaError(std::uint16_t code) : Error("DOS memory error " + std::to_string(code)), code_(code) {}
    std::uint16_t code() const { return code_; }
    CB_ERROR_BOILERPLATE(ArenaError)

private:
    std::uint16_t code_;
};

// One header of the chain.
struct Mcb {
    std::uint16_t segment = 0;   // where the header is (the block is one paragraph on)
    char signature = 'M';        // 'M': another block follows; 'Z': the last
    std::uint16_t owner = 0;     // the owner's PSP; kOwnerFree, kOwnerDos
    std::uint16_t size = 0;      // paragraphs, the header not counted
    friend bool operator==(const Mcb&, const Mcb&) = default;
};

// The header at `segment`; ArenaError(7) if it is not one.
Mcb read_mcb(dpmi::Engine& memory, std::uint16_t segment);

// Write a header and clear the rest of its paragraph (name and all).
void write_mcb(dpmi::Engine& memory, std::uint16_t segment, char signature, std::uint16_t owner, std::uint16_t size);

// The whole arena from `first` to the 'Z' header. ArenaError(7) when the chain does not end.
std::vector<Mcb> mcb_chain(dpmi::Engine& memory, std::uint16_t first);

// Join each run of free blocks into one (DOS does this before it allocates or resizes).
void coalesce(dpmi::Engine& memory, std::uint16_t first);

// The size of the largest free block, in paragraphs (0 for a damaged arena).
std::uint16_t largest(dpmi::Engine& memory, std::uint16_t first);

// How a block is chosen (INT 21h 58h): first fit, best fit (the smallest that fits), last fit (the top of the last).
enum class Strategy : std::uint8_t { First = 0, Best = 1, Last = 2 };

// A new block of `paragraphs` for `owner`: its segment (the header's + 1), or none when no free block is large enough.
// A damaged arena has none either.
std::optional<std::uint16_t> allocate(dpmi::Engine& memory, std::uint16_t first, std::uint16_t paragraphs, std::uint16_t owner,
                                      Strategy strategy = Strategy::First);

// Free the block at segment `block` (INT 21h 49h). ArenaError(9) when it is not a block.
void free_block(dpmi::Engine& memory, std::uint16_t first, std::uint16_t block);

// Resize the block at segment `block` (INT 21h 4Ah): none when it could; else the most it could have (error 8).
// ArenaError(9) when it is not a block.
std::optional<std::uint16_t> resize(dpmi::Engine& memory, std::uint16_t first, std::uint16_t block, std::uint16_t paragraphs);

// ---- the files a run sees -----------------------------------------------------------------------------------------

// The tools' own files, read from the disk once and kept: path key -> (modification time, size, bytes). A file is read
// again when its time or size changed. Shared by every MemFS of a session; safe to use from several threads.
class DiskCache {
public:
    Bytes read(const std::filesystem::path& path);   // NotFoundError when the file is not there

private:
    struct Entry {
        std::filesystem::file_time_type modified;
        std::uint64_t size = 0;
        std::shared_ptr<const Bytes> data;
    };
    std::mutex mutex_;
    std::map<std::string, Entry> entries_;
};

// A file of a MemFS, opened: the operations the DOS calls use.
class MemFile {
public:
    explicit MemFile(std::shared_ptr<Bytes> data) : data_(std::move(data)) {}
    Bytes read(std::optional<std::size_t> count = std::nullopt);   // none: to the end
    std::size_t write(ByteView data);                              // past the end the gap is zeros
    std::uint64_t seek(std::int64_t offset, int whence = 0);       // 0 from the start, 1 from here, 2 from the end; never below 0
    std::uint64_t tell() const { return position_; }
    void truncate(std::optional<std::uint64_t> size = std::nullopt);   // at the position unless a size is given

private:
    std::shared_ptr<Bytes> data_;
    std::uint64_t position_ = 0;
};

class MemFS {
public:
    // `hide`: folders whose files the disk does not show (what the run was given whole is in the MemFS). `disk`: the
    // cache of the tools' files; a MemFS of its own when not given.
    explicit MemFS(std::vector<std::filesystem::path> hide = {}, std::shared_ptr<DiskCache> disk = nullptr);

    // The key a path is kept by: absolute, as the system spells it, lower case.
    static std::string key(const std::filesystem::path& path);

    bool is_file(const std::filesystem::path& path) const;
    std::uint64_t size(const std::filesystem::path& path) const;    // NotFoundError when there is none
    Bytes read(const std::filesystem::path& path) const;            // NotFoundError when there is none
    // `write` truncates (or makes) the file; otherwise it is brought in from the disk (NotFoundError if absent).
    MemFile open(const std::filesystem::path& path, bool write) const;
    void put(const std::filesystem::path& path, ByteView data) const;
    void remove(const std::filesystem::path& path) const;           // NotFoundError when there is none
    void replace(const std::filesystem::path& from, const std::filesystem::path& to) const;

    // What the files hold, by key: the executive hands it to a run and takes back what the run changed.
    const std::map<std::string, std::shared_ptr<Bytes>>& files() const { return files_; }
    const std::set<std::string>& gone() const { return gone_; }
    const std::vector<std::string>& hidden() const { return hide_; }

private:
    friend class Executive;
    bool is_hidden(const std::string& key) const;
    Bytes disk(const std::filesystem::path& path) const;

    mutable std::map<std::string, std::shared_ptr<Bytes>> files_;
    mutable std::set<std::string> gone_;
    std::vector<std::string> hide_;   // keys of folders, each with its closing separator
    std::shared_ptr<DiskCache> disk_;
};

// ---- the run -----------------------------------------------------------------------------------------------------

struct RunSpec {
    std::filesystem::path exe;           // the program
    std::filesystem::path workdir;       // the working folder (drive C:)
    std::string command_tail;            // the command line after the program's name, as typed (the executive's PSP gets
                                         // it as it stands: a leading blank is the caller's)
    Bytes stdin_data;                    // what the program reads from the console
    Bytes environment = {'P', 'A', 'T', 'H', '=', 0};   // the environment strings
    std::uint64_t max_blocks = kDefaultMaxBlocks;
    bool trace_calls = false;            // count the calls (Result::calls)
    std::optional<std::uint16_t> psp;    // the PSP segment (default kPsp)
    std::optional<std::uint16_t> top;    // the first segment past memory (default kTop)
    bool fast = false;                   // no hook on every basic block: the blocks are not counted or kept; a run that does not end is stopped by the time limit
    bool dpmi = false;                   // a DPMI host is built for the run
    MemFS* files = nullptr;              // the files the run sees: none is the disk itself
    std::uint64_t watch = 0;             // a function (image offset) whose every call is recorded (Result::watched); 0: none
    bool watch_near = false;             // it returns with RET (the default: RETF)
    int watch_max = 200;                 // the calls recorded
    int watch_ds_bytes = 0x10000;        // the bytes of DS hashed at each return
    int watch_dumps = 0;                 // the first calls whose DS bytes are kept (WatchedCall::ds_in_bytes / ds_bytes)
    std::map<char, std::filesystem::path> drives;   // a drive letter -> its folder (empty: every drive is workdir); the
                                                    // current drive is `drive`, else Limits::drive (2: C)
    std::optional<int> drive;            // the current drive for this run (0: A, 2: C, 3: D)
};

// One call of the watched function.
struct WatchedCall {
    std::uint64_t return_address = 0;          // linear
    std::array<std::uint16_t, 8> args{};       // the first argument words
    std::uint16_t ds_in = 0;
    std::uint64_t ds_in_hash = 0;
    bool returned = false;
    std::uint16_t ax = 0, dx = 0, ds = 0, flags = 0;
    std::uint64_t ds_hash = 0;
    std::array<std::uint32_t, 64> ds_parts{};  // the hash of each 1/64 of the hashed DS bytes: where they differ
    Bytes ds_in_bytes, ds_bytes;               // DS's bytes at entry and at return (RunSpec::watch_dumps)
};

struct Result {
    std::optional<std::uint32_t> exit;                         // the exit code; none: the run did not say
    Bytes console;                                             // what it wrote to the console (Latin-1)
    std::set<std::uint64_t> blocks;                            // the executed basic blocks' addresses (a run that is not fast)
    std::uint32_t load_seg = 0;                                // the segment the program was loaded at
    std::uint64_t steps = 0;                                   // basic blocks executed
    std::map<std::uint64_t, std::uint64_t> calls;              // address -> how many times it was called (trace_calls)
    std::vector<WatchedCall> watched;                          // the watched function's calls, in order (RunSpec::watch)
    Bytes standard_out;                                        // what went to standard output (console without handle 2)
};

// What the executive is run under.
struct Limits {
    double fast_limit = 20;    // seconds a fast run may take
    double dpmi_limit = 300;   // seconds a run with a DPMI host may take
    int drive = 2;             // INT 21h 19h's answer: the current drive (2: C)
    bool debug = false;
};

class Executive {
public:
    // Start the executive (exact/emu/dosexec, linked in) on the Unicorn at `unicorn_dll`. NotFoundError when that file is
    // missing; ToolError when it is not Unicorn or the executive cannot start; UnsupportedError where the executive is not
    // built (it is Windows-only).
    static std::unique_ptr<Executive> load(const std::filesystem::path& unicorn_dll, const Limits& limits = {});
    ~Executive();
    Executive(const Executive&) = delete;
    Executive& operator=(const Executive&) = delete;

    // Run the program. EmulationError when the executive failed in a way the caller cannot go on from (Unicorn refused,
    // an assertion of the executive); NotFoundError / AccessError / IoError for the file the run could not open; a
    // program that does not end has no exit code (its Result says so) - it is not an error.
    Result run(const RunSpec& spec) const;

    // The summary line printed after the console: "[exit N, S basic blocks executed, D distinct]".
    static std::string summary(const Result& result);

private:
    struct Impl;
    explicit Executive(std::unique_ptr<Impl> impl);
    std::unique_ptr<Impl> impl_;
};

}  // namespace cb::exact::emu::dos
