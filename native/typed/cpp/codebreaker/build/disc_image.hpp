// codebreaker/build/disc_image.hpp -- typed C++ port of codebreaker.build.disc_image.
//
// disc_image.py -- write an ISO-9660 CD image from a folder.
//
// The writer produces a plain level-1 image (8.3 upper-case names, no Joliet or Rock Ridge
// extensions), optionally bootable through an El Torito boot record with floppy emulation. File
// contents are copied through in pieces, never held in memory whole. Names are made to fit:
// upper-cased, characters other than A-Z 0-9 _ become _, cut to 8 characters plus a 3-character
// extension; if two names in one folder end up the same the write is refused and the clashes
// are listed (DiscError), instead of producing an image with two files of one name.
//
//     disc_image::write(folder, out_path, volume_id="CODEBREAKER", boot_image=None)  // -> bytes
//
// Reading an image back (and floppy and hard-disk images) is codebreaker/formats/media; the
// --list and --extract forms of the command below go there.
//
//     python -m codebreaker disc <folder> <out.iso> [--volume NAME] [--boot IMG]
//     python -m codebreaker disc --list <image.iso>
//     python -m codebreaker disc --extract <image.iso> <outdir>
//
// All signatures are plain-typed (str, int, bool, bytes, lists/tuples of those).
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::build::disc_image {

extern const int64_t SECTOR;

struct DiscError : py::ValueError {
    DiscError() : py::ValueError() {}
    explicit DiscError(const std::string& m) : py::ValueError(m) {}
    explicit DiscError(const char* m) : py::ValueError(m) {}
    explicit DiscError(const py::str& m) : py::ValueError(m.utf8()) {}
    const char* type() const override { return "DiscError"; }
};

// El Torito media type by boot image size: 1.2 MB, 1.44 MB and 2.88 MB floppies are emulated;
// any other size is loaded as-is ("no emulation", 0).
int64_t _floppy(int64_t image_size);

py::bytes _both16(int64_t v);
py::bytes _both32(int64_t v);
int64_t _sectors(int64_t size);
py::str _clean(const py::str& text, int64_t limit);

// The name a file or folder gets on the disc, as (base, extension).
std::tuple<py::str, py::str> level1_name(const py::str& name, bool is_dir);

// Write the contents of `folder` as an ISO-9660 image; returns the image size in bytes.
// `boot_image` (a floppy image, normally 1474560 bytes) makes the disc bootable.
int64_t write(const py::str& folder, const py::str& out_path, const py::str& volume_id = py::str("CODEBREAKER"),
              const std::optional<py::str>& boot_image = std::nullopt);

// python -m codebreaker disc ...; `argv` is sys.argv[1:]. -> the exit status.
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::build::disc_image
