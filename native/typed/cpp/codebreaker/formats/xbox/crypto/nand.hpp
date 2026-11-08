// codebreaker/formats/xbox/crypto/nand.hpp -- C++ port of codebreaker.formats.xbox.crypto.nand.
// NAND dump analysis and extraction tools.
#pragma once

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::xbox::crypto::nand {

// NAND page sizes
extern const int64_t SMALL_PAGE_SIZE;
extern const int64_t SMALL_SPARE_SIZE;
extern const int64_t BIG_PAGE_SIZE;  // 2KB for big block
extern const int64_t BIG_SPARE_SIZE;

// Magic values
extern const py::bytes MAGIC_NAND_HEADER;

// HV + kernel layout
extern const int64_t HV_SIZE;  // 256 KB
extern const int64_t KERNEL_OFFSET_IN_UPDATE;

// The dict extract_header_info returns: the keys it has, in the order it sets them
// (a field is absent when the dict does not hold the key).
struct HeaderInfo {
    std::optional<py::str> magic;
    std::optional<int64_t> build;
    std::optional<int64_t> qfe;
    std::optional<int64_t> flags;
    std::optional<int64_t> entry_point;
    std::optional<int64_t> size;
};

// The dict nand_info returns.
struct NandInfo {
    py::str path;
    int64_t size = 0;
    py::str type;
    HeaderInfo header;
};

// str(dict) of a HeaderInfo, as Python writes it ("{'magic': 'FF4F', 'build': 1, ...}")
py::str header_repr(const HeaderInfo& info);

py::str detect_nand_type(const py::bytes& data);
HeaderInfo extract_header_info(const py::bytes& data);
py::bytes extract_hv_from_nand(const py::bytes& data);
std::tuple<py::bytes, py::bytes> extract_hv_from_update(const py::bytes& update_data);
int64_t get_cb_offset(const py::bytes& data);
NandInfo nand_info(const py::str& path);

extern const py::str DESCRIPTION;

// main(): `a` is sys.argv[1:]; the return value is the exit status (0, or 2 where Python calls sys.exit(2)).
int64_t main(const py::list<py::str>& a);

}  // namespace codebreaker::formats::xbox::crypto::nand
