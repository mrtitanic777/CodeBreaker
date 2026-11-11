// codebreaker/formats/xbox/ciso.hpp -- typed C++ from codebreaker.formats.xbox.ciso.
//
// CSO (Project Stellar / CISO v2) and CCI (Cerbios) -- block-compressed Xbox
// disc images. Both wrap a raw XDVDFS ISO as 2048-byte LZ4 sectors indexed by a
// u32 offset table (top bit = compressed). Reimplemented clean-room from the
// documented formats (facts cross-checked against xVerter, MIT).
//
//   CSO: magic 'CISO', 24-byte header, index at 0x18, block = [u32 len][lz4].
//   CCI: magic 'CCIM', 32-byte header, index at EOF, block = [u8 pad][lz4][pad].
//
// The decoded stream is a plain ISO, so `gdf` lists/extracts it directly.
//
// Commands: info / list / extract / toiso / tocso / tocci.
//
// Typing notes:
//   * The readers and the writers are formats.dll's (native/formats/xbox/discs.cc, linked in and asked through cbf_call).
//     A CsoReader / CciReader is a gdf::Device whose `_id` the reader gave (xdev.open_image); open_image() hands one out as the
//     std::shared_ptr<gdf::Device> the typed gdf takes.
//   * CisoError and Lz4Error are the typed gdf port's classes (the reader names them in an answer; gdf.hpp declared them
//     before this module was ported): the names here are using-declarations of those.
//   * `KIND` is a class attribute in Python; here KIND() is a virtual member.
//   * Replaces the hook `ciso_open_image` of formats::xbox::volumes (open_image(path)).
//   * main() takes sys.argv[1:].
#pragma once

#include <cstdint>
#include <memory>

#include "codebreaker/formats/xbox/gdf.hpp"
#include "py.hpp"

namespace codebreaker::formats::xbox::ciso {

using gdf::CisoError;
using gdf::Lz4Error;

extern const py::str DESCRIPTION;
extern const py::bytes CSO_MAGIC;
extern const py::bytes CCI_MAGIC;

// readers (expose read_at/size/close, so gdf can walk them; formats.dll's)
// size: the uncompressed bytes; _nblocks: its 2048-byte blocks; compressed: how many are compressed.
class _CompReader : public gdf::Device {
  public:
    int64_t size = 0;
    int64_t _nblocks = 0;
    int64_t compressed = 0;
    virtual const char* KIND() const = 0;
    virtual ~_CompReader() = default;

  protected:
    // (the checked constructor: a subclass names its KIND)
    _CompReader(const py::str& path, const char* kind);
};

class CsoReader : public _CompReader {
  public:
    explicit CsoReader(const py::str& path) : _CompReader(path, "CSO") {}
    const char* KIND() const override { return "CSO"; }
};

class CciReader : public _CompReader {
  public:
    explicit CciReader(const py::str& path) : _CompReader(path, "CCI") {}
    const char* KIND() const override { return "CCI"; }
};

std::shared_ptr<_CompReader> open_image(const py::str& path);

// ── writers (stream ISO sectors; patch the index afterwards) ───────────
// The ISO as a CSO image; -> the uncompressed size.
int64_t write_cso(const py::str& iso_path, const py::str& out_path);
// The ISO as a CCI image; -> the uncompressed size.
int64_t write_cci(const py::str& iso_path, const py::str& out_path);
// A CSO / CCI image as a plain ISO; -> the bytes written.
int64_t to_iso(const py::str& image_path, const py::str& out_path);

// ── CLI ────────────────────────────────────────────────────────────────
// the command line without the program name (sys.argv[1:]); prints to stdout
void main(const py::list<py::str>& argv);

}  // namespace codebreaker::formats::xbox::ciso
