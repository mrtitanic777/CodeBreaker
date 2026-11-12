// codebreaker/formats/xbox/crypto/nand_tool.hpp -- SHIM over cb/formats/xbox/crypto/nand_tool.hpp (docs/CPP_DESIGN.md 6.3); deleted
// with its last py:: consumer (the crypto tool runner). What no caller used is not carried over: the ctypes spare structures
// (cb::...::BlockSpare, parse_spare/encode_spare) and the stream-shaped NANDImage (cb::...::NandImage).
#pragma once

#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/formats/xbox/crypto/nand_tool.hpp"
#include "py.hpp"

namespace codebreaker::formats::xbox::crypto::nand_tool {

namespace cbn = cb::formats::xbox::crypto::nand_tool;

inline constexpr int64_t PAGE = 512;
inline constexpr int64_t SPARE = 16;
inline constexpr int64_t FULL = 528;

inline const py::str DESCRIPTION = py::str("Xbox 360 NAND image build: ecc / unecc / verify / fix ECC (528-byte page + spare, nandpro/xeBuild class)");

inline int64_t calc_page_ecc(const py::bytes& data, const py::bytes& spare) {
    return cb::compat::guard([&] { return int64_t(cbn::page_ecc(cb::compat::from_py(data), cb::compat::from_py(spare))); });
}

// (data, spare with the ECC filled in)
inline std::tuple<py::bytes, py::bytearray> fix_page_ecc(const py::bytes& data, const py::bytes& spare) {
    return cb::compat::guard([&] {
        cb::Bytes s = cb::compat::from_py(spare);
        cbn::fix_page_ecc(cb::compat::from_py(data), s);
        return std::make_tuple(data, py::bytearray(cb::compat::to_py(cb::ByteView(s))));
    });
}

inline int64_t _stored_ecc(const py::bytes& spare) {
    return cb::compat::guard([&] { return int64_t(cbn::stored_ecc(cb::compat::from_py(spare))); });
}

inline py::bytes strip_spare(const py::bytes& img) {
    return cb::compat::guard([&] { return cb::compat::to_py(cb::ByteView(cbn::strip_spare(cb::compat::from_py(img)))); });
}

inline py::bytes add_spare(const py::bytes& raw, const std::optional<py::bytes>& meta = std::nullopt) {
    return cb::compat::guard([&] {
        const cb::Bytes r = cb::compat::from_py(raw);
        std::optional<cb::Bytes> m;
        if (meta && !meta->empty()) m = cb::compat::from_py(*meta);   // (the Python took an empty template as none)
        return cb::compat::to_py(cb::ByteView(cbn::add_spare(r, m ? std::optional<cb::ByteView>(*m) : std::nullopt)));
    });
}

inline py::list<int64_t> verify_ecc(const py::bytes& img) {
    return cb::compat::guard([&] {
        py::list<int64_t> out;
        for (const std::size_t page : cbn::verify_ecc(cb::compat::from_py(img))) out.append(static_cast<int64_t>(page));
        return out;
    });
}

inline py::bytes fix_all_ecc(const py::bytes& img) {
    return cb::compat::guard([&] { return cb::compat::to_py(cb::ByteView(cbn::fix_all_ecc(cb::compat::from_py(img)))); });
}

// `nandtool ...`; `a` is sys.argv[1:]. A non-zero status is the SystemExit the script raised.
inline void main(const py::list<py::str>& a) {
    int code = 0;
    try {
        code = cbn::command(cb::compat::from_py(a));
    } catch (const cb::UsageError& e) {   // (the script printed its usage and exited 2)
        py::print(cb::compat::to_py(std::string(e.what())));
        throw py::SystemExit(2);
    } catch (const cb::Error& e) {
        cb::compat::rethrow_as_py(e);
    }
    if (code != 0) throw py::SystemExit(code);
}

}  // namespace codebreaker::formats::xbox::crypto::nand_tool
