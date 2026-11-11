// codebreaker/formats/xbox/crypto/xsm3.hpp -- typed C++ port of codebreaker.formats.xbox.crypto.xsm3.
//
// Xbox Security Method 3 (XSM3), the challenge / response authentication of Xbox 360 controllers.
//
// References:
//   http://oct0xor.github.io/2017/05/03/xsm3/
//   https://github.com/oct0xor/xbox_security_method_3
//   https://github.com/InvoxiPlayGames/libxsm3/blob/master/xsm3.c
//
// Typing notes:
//   * BinLike (bytes | bytearray) is py::bytes (a py::bytearray converts to it). The functions that return the
//     result of a cipher return py::bytes; the response packets (built in a bytearray and sliced) are
//     py::bytearray.
//   * CryptMode is an enum class over int64_t (the IntEnum); a value that is no member (static_cast) gives
//     the Python's `b""` from UsbdSecXSM3AuthenticationCrypt.
//   * UsbdSecXSM3AuthenticationMac's salt is `BinLike | None`: std::optional<py::bytes>; it returns the salt
//     (None / the empty salt it was given, else the incremented 8-byte salt) with the MAC.
//   * XSM3State's fields start as None (std::optional). xsm3_random_console_data is a bytearray from the
//     moment it is first set (the Python holds it as bytes until the end of xsm3_do_challenge_init, which a
//     later call cannot see: xsm3_do_challenge_verify needs the controller data set at the same point).
//   * The module-level keys (STATIC_KEY_1 ...) are py::bytes, empty until main() reads them.
//   * What the Python prints goes to std::cout, or the stream of set_stream.
//   * random.randbytes(0x10) draws from the generator behind XeCrypt::XeCryptRandom (random_seed seeds it).
//   * main(): the Python takes no arguments and returns the exit code; it reads "KV/banned.bin".
//   * not ported: the `__name__ == "__main__"` block (`exit(main())`).
#pragma once

#include <cstdint>
#include <iosfwd>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::formats::xbox::crypto::xsm3 {

using BinLike = py::bytes;   // bytes | bytearray

extern py::bytes STATIC_KEY_1;
extern py::bytes STATIC_KEY_2;

extern py::bytes DYNAMIC_KEY_1;
extern py::bytes DYNAMIC_KEY_2;

extern const py::bytes SBOX;
extern const py::bytes UsbdSecPlainTextData;
extern const py::bytes UsbdSecXSM3GetIdentificationProtocolData;
extern const py::bytes UsbdSecXSM3SetChallengeProtocolData;
extern const py::bytes UsbdSecXSM3GetResponseChallengeProtocolData;
extern const py::bytes UsbdSecXSM3SetVerifyProtocolData1;
extern const py::bytes UsbdSecXSM3GetResponseVerifyProtocolData1;
extern const py::bytes xsm3_id_data_ms_controller;

enum class CryptMode : int64_t {
    DECRYPT = 0,
    ENCRYPT = 1,
};

// IndexError when the packet is shorter than its length byte says
int64_t xsm3_calculate_checksum(const BinLike& packet);
bool xsm3_verify_checksum(const BinLike& packet);

py::bytes UsbdSecXSM3AuthenticationCrypt(const BinLike& key, const BinLike& data, CryptMode mode);
// (salt, mac)
std::tuple<std::optional<py::bytes>, py::bytes> UsbdSecXSM3AuthenticationMac(const BinLike& key, const std::optional<py::bytes>& salt,
                                                                             const BinLike& data);
py::bytes UsbdSecXSMAuthenticationAcr(const BinLike& key, const BinLike& cert, const BinLike& data);

class XSM3State {
public:
    std::optional<py::bytearray> xsm3_identification_data;
    std::optional<py::bytearray> xsm3_random_console_data;
    std::optional<py::bytes> xsm3_console_id;
    std::optional<py::bytes> xsm3_challenge_init_hash;
    std::optional<py::bytes> xsm3_random_controller_data;

    std::optional<py::bytes> xsm3_random_console_data_enc;
    std::optional<py::bytes> xsm3_random_console_data_swap_enc;

    // AssertionError for an identification packet that is not 0x1D bytes
    explicit XSM3State(const BinLike& xsm3_ident_packet);

    XSM3State& __enter__() { return *this; }
    void __exit__() {}

    void reset();

    void xsm3_set_identification_data(const BinLike& ident_packet);
    // None when a checksum or MAC fails (and the failure is printed); AssertionError for a packet that is not 0x22 bytes
    std::optional<py::bytearray> xsm3_do_challenge_init(const BinLike& challenge_packet);
    // None when the checksum or MAC fails; AssertionError for a packet that is not 0x16 bytes
    std::optional<py::bytearray> xsm3_do_challenge_verify(const BinLike& challenge_packet);
};

// The output stream of print: std::cout unless set.
void set_stream(std::ostream& out);

int64_t main();

}  // namespace codebreaker::formats::xbox::crypto::xsm3
