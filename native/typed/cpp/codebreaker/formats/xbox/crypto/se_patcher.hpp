// codebreaker/formats/xbox/crypto/se_patcher.hpp -- typed C++ port of codebreaker.formats.xbox.crypto.se_patcher.
//
// Patches, compresses, rehashes and signs the SE (5BL) and SD (4BL) bootloaders of an Xbox 360 image.
//
// Typing notes:
//   * sd_data: BinLike. The Python turns a `bytes` into a new bytearray and works on a bytearray in place
//     (the SD hash and size are written into it, padding is appended to it and it is signed in place, and it
//     is the very object returned), so there is one overload per kind: py::bytes works on a copy, a
//     py::bytearray argument is changed and returned.
//   * se_data: BinLike. It is only read (the patches and the compression make new buffers), so one py::bytes
//     parameter takes either. se_patch_data: BinLike | None is std::optional<py::bytes>.
//   * The result `((sd_len_nopad, sd_data), (se_len_nopad, se_data))` is a tuple of two (int, bytearray) tuples.
//   * SB_PRV_KEY is the module-level key, None until main() has loaded it (`global SB_PRV_KEY`):
//     std::optional. patch_rehash_sign_se with no key fails in sign_sd_4bl, after everything before it was
//     done, with the Python's AttributeError.
//   * main(): the Python takes no arguments and returns the exit code; it reads and writes relative paths
//     ("Output/...", "xeBuild_1.21/..."), and prints to the stream of set_streams (stdout when not set).
//   * not ported: the `__name__ == "__main__"` block (`exit(main())`).
#pragma once

#include <cstdint>
#include <iosfwd>
#include <optional>
#include <tuple>

#include "py.hpp"
#include "codebreaker/formats/xbox/crypto/XeCrypt.hpp"

namespace codebreaker::formats::xbox::crypto::se_patcher {

// SB_PRV_KEY: XeCryptRsaKey = None
extern std::optional<XeCrypt::XeCryptRsaKey> SB_PRV_KEY;

using SignedBootloader = std::tuple<int64_t, py::bytearray>;   // (length without padding, data)

// Apply the patches to the SE (if any), compress it, put its header on, hash it into the SD, and resign the
// SD. Returns ((sd_len_nopad, sd_data), (se_len_nopad, se_data)).
std::tuple<SignedBootloader, SignedBootloader> patch_rehash_sign_se(const py::bytes& sd_data, const py::bytes& se_data,
                                                                    const std::optional<py::bytes>& se_patch_data = std::nullopt);
// (a bytearray sd_data is changed in place and is the one returned)
std::tuple<SignedBootloader, SignedBootloader> patch_rehash_sign_se(const py::bytearray& sd_data, const py::bytes& se_data,
                                                                    const std::optional<py::bytes>& se_patch_data = std::nullopt);

// The output stream of print: stdout unless set.
void set_stream(std::ostream& out);

int64_t main();

}  // namespace codebreaker::formats::xbox::crypto::se_patcher
