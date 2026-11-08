// codebreaker/formats/xbox/chd.hpp -- C++ port of codebreaker.formats.xbox.chd.
//
// CHD (MAME Compressed Hunks of Data) -- Xbox disc images.
//
// `info` parses the CHD v5 header natively (no external tool). Conversions
// (chd<->iso, verify) are delegated to MAME's `chdman`, exactly as xVerter and
// every other CHD tool does -- CHD's multi-codec hunk format (zlib/lzma/flac/...) is
// maintained upstream and not worth reimplementing. chdman is found via `config`
// (key `chdman`) or PATH, the same driven-not-embedded policy as Ghidra/DOSBox.
//
// Commands: info <chd> | toiso <chd> <out.iso> | tochd <in.iso> <out.chd> | verify <chd>.
//
// Notes on the port:
//   * the setting `chdman` is read through py::settings::get_str (py_settings.hpp);
//   * _run starts the program with the Windows API (subprocess.run with stdout=PIPE, stderr=STDOUT,
//     text=True): its output is decoded as cp1252 (the locale codec of the Python it was ported
//     from) with universal newlines;
//   * a logical size of 2**63 bytes or more (Python's unbounded int) raises py::OverflowError;
//   * main() takes sys.argv as its argument (the program name first); sys.exit(2) is SystemExit.
#pragma once

#include "py.hpp"

namespace codebreaker::formats::xbox::chd {

extern const py::str DESCRIPTION;

extern const py::bytes CHD_MAGIC;

struct ChdError : py::Exception {
    using py::Exception::Exception;
    const char* type() const override { return "ChdError"; }
};

// sys.exit(code): not a py::Exception (it is not an Exception in Python either)
struct SystemExit {
    int64_t code;
};

// read_header's dict: ints and a list of str
struct Header {
    int64_t version = 0;
    int64_t header_len = 0;
    int64_t logical_bytes = 0;
    int64_t hunk_bytes = 0;
    int64_t unit_bytes = 0;
    py::list<py::str> compressors;
    int64_t meta_offset = 0;
    int64_t map_offset = 0;
};

// ── native CHD v5 header ───────────────────────────────────────────────

// Parse a CHD v5 header (big-endian). Raises for other versions (chdman can upgrade them
// with `chdman copy`).
Header read_header(const py::str& path);

// ── chdman driver ──────────────────────────────────────────────────────
py::str _need_chdman();

py::str _run(const py::list<py::str>& argv);

// chd -> ISO (chdman extractdvd).
int64_t extract(const py::str& chd_path, const py::str& out_iso);

// ISO -> chd (chdman createdvd).
int64_t build(const py::str& iso_path, const py::str& out_chd);

py::str verify(const py::str& chd_path);

// ── CLI ────────────────────────────────────────────────────────────────
// `sys_argv`: sys.argv, the program name first. Throws SystemExit where the Python calls sys.exit.
void main(const py::list<py::str>& sys_argv);

}  // namespace codebreaker::formats::xbox::chd
