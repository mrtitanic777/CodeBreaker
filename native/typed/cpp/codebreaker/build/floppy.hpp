// codebreaker/build/floppy.hpp -- typed C++ port of codebreaker.build.floppy.
//
// floppy.py -- a FAT12 boot floppy made from rebuilt files, laid out as an original one is.
//
//     python -m codebreaker boot-floppy [--image ORIGINAL.img] [--out FILE.img] [--extract DIR]
//
//     Disk disk = floppy::read(img_bytes);          // .boot (512 bytes), .entries [Entry], .bpb
//     py::bytes img = floppy::make(disk, boot, contents);   // contents {NAME.EXT: bytes}
//
// The original floppy says what goes on the disk and how: its boot parameters (the BPB in the boot
// sector), its root directory -- the files, in their order, with their attributes and times, and
// the volume label. make() keeps all of that and puts in the boot sector given and each file's
// contents given (a file not given keeps the original's), each file in one run of clusters, in
// the order of the directory, both FATs written to match.
//
// The command builds the Windows 95 Setup boot floppy (setup/media/boot.img of the corpus) from
// what CodeBreaker rebuilt: the boot sector from Windows95/design/recon/bootdisk/msboot.asm
// (nasm), every real-mode program from its source (`dos-source`, state/bootfloppy/src/<NAME>),
// and the text files (CONFIG.SYS, AUTOEXEC.BAT, MSDOS.SYS ...) as they are -- they are source.
// A file with no rebuilt program is the original's, and the command says which.
//
// Notes on the port:
//   * The Python's `_bpb` dict (fixed keys) is the record `Bpb`; `Entry`/`Disk` are the classes.
//   * `source.dos_writer.reconstruct` has no typed port yet: main() calls `dos_writer_reconstruct()`, a hook the
//     program sets (it raises NotImplementedError until then). Programs that have a rebuilt file under
//     state/bootfloppy/src never reach it.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::build::floppy {

constexpr int64_t _ATTR_LABEL = 0x08, _ATTR_DIR = 0x10, _LFN = 0x0F;

struct Entry {
    py::bytearray raw;                  // the 32-byte directory entry
    std::optional<py::bytes> data;      // the file's bytes (None for the volume label)

    Entry(const py::bytes& raw, const std::optional<py::bytes>& data) : raw(raw), data(data) {}
    py::str name() const;
    bool is_file() const;
};

// The boot parameters, and where the root directory and the data begin.
struct Bpb {
    int64_t bps = 0, spc = 0, rsv = 0, nfat = 0, nroot = 0, total = 0, spf = 0, root = 0, data = 0;
    bool operator==(const Bpb& o) const {
        return bps == o.bps && spc == o.spc && rsv == o.rsv && nfat == o.nfat && nroot == o.nroot && total == o.total &&
               spf == o.spf && root == o.root && data == o.data;
    }
    bool operator!=(const Bpb& o) const { return !(*this == o); }
};

struct Disk {
    py::bytes boot;
    py::list<Entry> entries;
    Bpb bpb;
    Disk(const py::bytes& boot, const py::list<Entry>& entries, const Bpb& bpb) : boot(boot), entries(entries), bpb(bpb) {}
};

Bpb _bpb(const py::bytes& boot);

// A FAT12 floppy image. -> Disk (root directory only: a boot floppy has no folders).
Disk read(const py::bytes& img);

// The image: `boot` as the boot sector, the directory of `disk` with `contents` in it.
py::bytes make(const Disk& disk, const py::bytes& boot, const py::dict<py::str, py::bytes>& contents);

// The boot sector assembled from its source (nasm). -> (bytes, source path)
std::tuple<py::bytes, py::str> _boot_sector(const py::str& design);

// The file built from a program's source in state/bootfloppy/src/<NAME>, or None.
std::optional<py::str> _rebuilt(const py::str& src_dir, const py::str& name);

// What source.dos_writer.reconstruct answers (the keys main reads).
struct Reconstructed {
    bool same = false;
    bool equivalent = false;
    std::optional<py::str> file;
    py::str image;
};
std::function<Reconstructed(const py::str& path, const py::str& folder)>& dos_writer_reconstruct();

// python -m codebreaker boot-floppy ...; `argv` is sys.argv[1:]. -> the exit status.
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::build::floppy
