// codebreaker/formats/xbox/crypto/xell_unpack.cpp -- C++ from codebreaker.formats.xbox.crypto.xell_unpack.
//
// (The Python imports isfile from os.path and never uses it; it is left out.)
//
// The Python calls gzip.decompress: the runtime's py::gzip::decompress, with the same three failures --
// BadGzipFile, EOFError, zlib.error.

#include "codebreaker/formats/xbox/crypto/xell_unpack.hpp"

namespace codebreaker::formats::xbox::crypto::xell_unpack {

const py::str XELL_DIR("XeLL");

// Decompress a gzip compressed string in one shot.
py::bytes decompress(const py::bytes& data_in) { return py::gzip::decompress(data_in); }

// utilities
std::variant<py::bytes, py::str> read_file(const py::str& filename, bool text) {
    py::file f = py::open(filename, py::str(text ? "r" : "rb"));
    if (text) {
        py::str data = f.read_text();
        f.close();
        return data;
    }
    py::bytes data = f.read();
    f.close();
    return data;
}

void write_file(const py::str& filename, const std::variant<py::str, py::bytes, py::bytearray>& data) {
    bool is_str = std::holds_alternative<py::str>(data);
    py::file f = py::open(filename, py::str(is_str ? "w" : "wb"));
    if (is_str) f.write(std::get<py::str>(data));
    else if (std::holds_alternative<py::bytes>(data)) f.write(std::get<py::bytes>(data));
    else f.write(std::get<py::bytearray>(data));
    f.close();
}

void main() {
    // xell-1f.bin
    py::bytes xell_1f = std::get<py::bytes>(read_file(py::os::path::join(XELL_DIR, py::str("xell-1f.bin"))));
    py::bytes xell_1f_stage_1 = xell_1f.slice_to(XELL_STAGE1_SIZE);
    write_file(py::os::path::join(XELL_DIR, py::str("xell-1f-stage-1.bin")), xell_1f_stage_1);
    py::bytes xell_1f_stage_2 = xell_1f.slice(XELL_STAGE1_SIZE, XELL_STAGE1_SIZE + XELL_STAGE2_SIZE);
    write_file(py::os::path::join(XELL_DIR, py::str("xell-1f-stage-2-compressed.elf")), xell_1f_stage_2);
    xell_1f_stage_2 = decompress(xell_1f_stage_2);
    write_file(py::os::path::join(XELL_DIR, py::str("xell-1f-stage-2-decompressed.elf")), xell_1f_stage_2);

    // xell-2f.bin
    py::bytes xell_2f = std::get<py::bytes>(read_file(py::os::path::join(XELL_DIR, py::str("xell-2f.bin"))));
    py::bytes xell_2f_stage_1 = xell_2f.slice_to(XELL_STAGE1_SIZE);
    write_file(py::os::path::join(XELL_DIR, py::str("xell-2f-stage-1.bin")), xell_2f_stage_1);
    py::bytes xell_2f_stage_2 = xell_2f.slice(XELL_STAGE1_SIZE, XELL_STAGE1_SIZE + XELL_STAGE2_SIZE);
    write_file(py::os::path::join(XELL_DIR, py::str("xell-2f-stage-2-compressed.elf")), xell_2f_stage_2);
    xell_2f_stage_2 = decompress(xell_2f_stage_2);
    write_file(py::os::path::join(XELL_DIR, py::str("xell-2f-stage-2-decompressed.elf")), xell_2f_stage_2);

    // xell-gggggg.bin
    py::bytes xell_gggggg = std::get<py::bytes>(read_file(py::os::path::join(XELL_DIR, py::str("xell-gggggg.bin"))));
    py::bytes xell_gggggg_stage_1 = xell_gggggg.slice_to(XELL_STAGE1_SIZE);
    write_file(py::os::path::join(XELL_DIR, py::str("xell-gggggg-stage-1.bin")), xell_gggggg_stage_1);
    py::bytes xell_gggggg_stage_2 = xell_gggggg.slice(XELL_STAGE1_SIZE, XELL_STAGE1_SIZE + XELL_STAGE2_SIZE);
    write_file(py::os::path::join(XELL_DIR, py::str("xell-gggggg-stage-2-compressed.elf")), xell_gggggg_stage_2);
    xell_gggggg_stage_2 = decompress(xell_gggggg_stage_2);
    write_file(py::os::path::join(XELL_DIR, py::str("xell-gggggg-stage-2-decompressed.elf")), xell_gggggg_stage_2);
}

}  // namespace codebreaker::formats::xbox::crypto::xell_unpack
