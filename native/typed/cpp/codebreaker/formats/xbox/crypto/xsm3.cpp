// codebreaker/formats/xbox/crypto/xsm3.cpp -- typed C++ port of codebreaker.formats.xbox.crypto.xsm3.

#include "codebreaker/formats/xbox/crypto/xsm3.hpp"

#include <initializer_list>
#include <iostream>

#include "codebreaker/formats/xbox/crypto/XeCrypt.hpp"
#include "codebreaker/formats/xbox/crypto/build_lib.hpp"

namespace codebreaker::formats::xbox::crypto::xsm3 {

using namespace XeCrypt;

namespace {

std::ostream* out_stream_ = &std::cout;

py::bytes bytes_of(std::initializer_list<unsigned> values) {
    std::string s;
    for (unsigned v : values) s.push_back(static_cast<char>(v));
    return py::bytes(s);
}

// print("...")
void print_(const char* text) { *out_stream_ << text << "\n"; }

// bytes * 2
py::bytes twice(const py::bytes& key) { return key + key; }

// int.from_bytes(b, "big", signed=False) for at most 8 bytes (or the low 8 bytes of a longer value)
uint64_t be_u64(const py::bytes& b) {
    const int64_t n = py::len(b);
    uint64_t v = 0;
    for (int64_t i = (n > 8 ? n - 8 : 0); i < n; ++i) v = (v << 8) | static_cast<uint64_t>(b[i]);
    return v;
}

// (v & 0xFFFFFFFFFFFFFFFF).to_bytes(8, "big", signed=False)
py::bytes u64_be(uint64_t v) {
    std::string s(8, '\0');
    for (int i = 7; i >= 0; --i) {
        s[static_cast<size_t>(i)] = static_cast<char>(v & 0xFF);
        v >>= 8;
    }
    return py::bytes(s);
}

// the value of an attribute that is None in the Python: TypeError for what the Python does with it
template <class T>
const T& need(const std::optional<T>& v) {
    if (!v) throw py::TypeError("'NoneType' object is not subscriptable");
    return *v;
}

}  // namespace

py::bytes STATIC_KEY_1;
py::bytes STATIC_KEY_2;

py::bytes DYNAMIC_KEY_1;
py::bytes DYNAMIC_KEY_2;

const py::bytes SBOX = bytes_of({
    0xB0, 0x3D, 0x9B, 0x70, 0xF3, 0xC7, 0x80, 0x60,
    0x73, 0x9F, 0x6C, 0xC0, 0xF1, 0x3D, 0xBB, 0x40,
    0xB3, 0xC8, 0x37, 0x14, 0xDF, 0x49, 0xDA, 0xD4,
    0x48, 0x22, 0x78, 0x80, 0x6E, 0xCD, 0xE7, 0x00,
    0x81, 0x86, 0x68, 0xE1, 0x5D, 0x7C, 0x54, 0x2C,
    0x55, 0x7B, 0xEF, 0x48, 0x42, 0x7B, 0x3B, 0x68,
    0xE3, 0xDB, 0xAA, 0xC0, 0x0F, 0xA9, 0x96, 0x20,
    0x95, 0x05, 0x93, 0x94, 0x9A, 0xF6, 0xA3, 0x64,
    0x5D, 0xCC, 0x76, 0x00, 0xE5, 0x08, 0x19, 0xE8,
    0x8D, 0x29, 0xD7, 0x4C, 0x21, 0x91, 0x17, 0xF4,
    0xBC, 0x6A, 0xB3, 0x80, 0x83, 0xC6, 0xD4, 0x90,
    0x9B, 0xAE, 0x0E, 0xFE, 0x2E, 0x4A, 0xF2, 0x00,
    0x73, 0x88, 0xD9, 0x40, 0x66, 0xC5, 0xD4, 0x08,
    0x57, 0xB1, 0x89, 0x48, 0xDC, 0x54, 0xFC, 0x43,
    0x6A, 0x26, 0x87, 0xB8, 0x09, 0x5F, 0xCE, 0x80,
    0xE4, 0x0B, 0x05, 0x9C, 0x24, 0xF3, 0xDE, 0xE2,
    0x3E, 0xEC, 0x38, 0x8A, 0xA2, 0x55, 0xA4, 0x50,
    0x4E, 0x4B, 0xE9, 0x58, 0x7F, 0x9F, 0x7D, 0x80,
    0x23, 0x0C, 0x4D, 0x80, 0x05, 0x44, 0x26, 0xB8,
    0xE9, 0xD8, 0xBC, 0xE6, 0x76, 0x3A, 0x6E, 0xA4,
    0x19, 0xDE, 0xC2, 0xD0, 0xC4, 0xBC, 0xC3, 0x5C,
    0x59, 0xDF, 0x16, 0x46, 0x39, 0x70, 0xF4, 0xEE,
    0x2D, 0x58, 0x5A, 0xA8, 0x17, 0x86, 0x6B, 0x60,
    0x29, 0x58, 0x4D, 0xD2, 0x5F, 0x28, 0x7A, 0xD8,
    0x8E, 0x79, 0xEA, 0x82, 0x94, 0x33, 0x31, 0x81,
    0xD9, 0x22, 0xD5, 0x10, 0xDA, 0x92, 0xA0, 0x7D,
    0x3D, 0xDA, 0xAC, 0x1C, 0xA2, 0x53, 0x31, 0xB8,
    0x3C, 0x96, 0x52, 0x00, 0x82, 0x6B, 0x56, 0xA0,
    0xD3, 0xC2, 0x40, 0xC7, 0x1B, 0x7F, 0xDC, 0x01,
    0x72, 0x70, 0xB1, 0x8C, 0x01, 0x09, 0x09, 0x36,
    0xFC, 0x97, 0xEA, 0xDE, 0xE3, 0x0D, 0xAE, 0x7E,
    0xE3, 0x0D, 0xAE, 0x7E, 0x33, 0x69, 0x80, 0x40,
});

const py::bytes UsbdSecPlainTextData = bytes_of({
    0xD1, 0xD2, 0xF2, 0x80, 0x6E, 0xBA, 0x0C, 0xC0,
    0xB6, 0xC4, 0xC9, 0xD8, 0x61, 0x75, 0x1D, 0x1A,
    0x3F, 0x95, 0x58, 0xBE, 0xD8, 0x0D, 0xE2, 0xC0,
    0xD0, 0x21, 0x79, 0x20, 0x65, 0x2D, 0x99, 0x40,
    0x3C, 0x96, 0x52, 0x00, 0x1B, 0x7F, 0xDC, 0x01,
    0x82, 0x1C, 0x13, 0xD8, 0x33, 0x69, 0x80, 0x40,
    0xFC, 0x97, 0xEA, 0xDE, 0x08, 0xEA, 0x14, 0xDC,
    0xEB, 0x0F, 0x6A, 0x18, 0x6F, 0x78, 0x2C, 0xB0,
    0xD3, 0xC2, 0x40, 0xC7, 0x82, 0x6B, 0x56, 0xA0,
    0x19, 0x09, 0x36, 0xE0, 0x72, 0x70, 0xB1, 0x8C,
    0xE3, 0x0D, 0xAE, 0x7E, 0x50, 0xA5, 0x2B, 0xE2,
    0xC9, 0xAF, 0xC7, 0x70, 0x1C, 0x29, 0x80, 0x56,
    0x24, 0xF0, 0x66, 0xFA, 0x02, 0x2B, 0x58, 0x98,
    0x8F, 0xE4, 0xD1, 0x3C, 0x6E, 0x38, 0x2A, 0xFF,
    0xB8, 0xFA, 0x35, 0xB0, 0x52, 0x49, 0xC5, 0xB4,
    0x66, 0xFA, 0x47, 0x55, 0x6C, 0x8D, 0x40, 0x08,
});

const py::bytes UsbdSecXSM3GetIdentificationProtocolData = bytes_of({
    0x49, 0x4B, 0x00, 0x00, 0x17, 0x04, 0xE1, 0x11,
    0x54, 0x15, 0xED, 0x88, 0x55, 0x21, 0x01, 0x33,
    0x00, 0x00, 0x80, 0x02, 0x5E, 0x04, 0x8E, 0x02,
    0x03, 0x00, 0x01, 0x01, 0xC1,
});

const py::bytes UsbdSecXSM3SetChallengeProtocolData = bytes_of({
    0x09, 0x40, 0x00, 0x00, 0x1C, 0x0A, 0x0F, 0x6B,
    0x0B, 0xA1, 0x18, 0x26, 0x5F, 0x83, 0x3C, 0x45,
    0x13, 0x49, 0x53, 0xBD, 0x18, 0x61, 0x73, 0xCF,
    0x29, 0xDE, 0x2C, 0xD8, 0x66, 0xE4, 0xAE, 0x34,
    0xA9, 0x9C,
});

const py::bytes UsbdSecXSM3GetResponseChallengeProtocolData = bytes_of({
    0x49, 0x4C, 0x00, 0x00, 0x28, 0x81, 0xBD, 0x7C,
    0xB3, 0x70, 0xBD, 0x76, 0x1A, 0x2F, 0x28, 0x6E,
    0xD1, 0xF2, 0xC3, 0x8E, 0xF9, 0x0B, 0xB2, 0x83,
    0x49, 0xCB, 0x4B, 0x24, 0xA2, 0x90, 0x6C, 0x27,
    0xB1, 0x05, 0x0A, 0xB0, 0x47, 0x09, 0x75, 0x16,
    0x07, 0xE1, 0xD7, 0xE8, 0xAF, 0x57,
});

const py::bytes UsbdSecXSM3SetVerifyProtocolData1 = bytes_of({
    0x09, 0x41, 0x00, 0x00, 0x10, 0x5A, 0xDD, 0x1B,
    0xA0, 0x74, 0x87, 0xB7, 0x62, 0xB7, 0xA5, 0x8F,
    0x34, 0xFF, 0xE3, 0xD1, 0xD9, 0xA7,
});

const py::bytes UsbdSecXSM3GetResponseVerifyProtocolData1 = bytes_of({
    0x49, 0x4C, 0x00, 0x00, 0x10, 0x5A, 0x9C, 0xD6,
    0x72, 0xB3, 0x70, 0x8D, 0xA7, 0x57, 0x01, 0x06,
    0x50, 0x20, 0x60, 0xA9, 0xBC, 0xDE,
});

const py::bytes xsm3_id_data_ms_controller = bytes_of({
    0x49, 0x4B, 0x00, 0x00, 0x17, 0x41, 0x41, 0x41,
    0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,
    0x00, 0x00, 0x80, 0x02, 0x5E, 0x04, 0x8E, 0x02,
    0x03, 0x00, 0x01, 0x01, 0x16,
});

int64_t xsm3_calculate_checksum(const BinLike& packet) {
    const int64_t size = packet[4] + 5;
    int64_t csum = 0;
    for (int64_t i = 5; i < size; ++i) csum ^= packet[i];
    return csum & 0xFF;
}

bool xsm3_verify_checksum(const BinLike& packet) {
    const int64_t pkt_len = packet[4] + 5;
    return xsm3_calculate_checksum(packet) == packet[pkt_len];
}

py::bytes UsbdSecXSM3AuthenticationCrypt(const BinLike& key, const BinLike& data, CryptMode mode) {
    XeCryptDes3 c(twice(key).slice(0, 0x18), XeCryptDes3::MODE_CBC, py::bytes::zeros(8));
    if (mode == CryptMode::DECRYPT) {
        return c.decrypt(data);
    } else if (mode == CryptMode::ENCRYPT) {
        return c.encrypt(data);
    }
    return py::bytes("");
}

std::tuple<std::optional<py::bytes>, py::bytes> UsbdSecXSM3AuthenticationMac(const BinLike& key, const std::optional<py::bytes>& salt_in,
                                                                             const BinLike& data) {
    py::bytes temp("");
    std::optional<py::bytes> salt = salt_in;

    XeCryptDes c(key.slice(0, 8));
    if (salt && !salt->empty()) {
        uint64_t v0 = be_u64(*salt);
        v0 += 1;   // (v0 & 0xFFFFFFFFFFFFFFFF): the sum wraps in 64 bits
        salt = u64_be(v0);
        temp = c.encrypt(*salt);
    }

    for (int64_t i = 0; i < py::len(data); i += 8) {
        uint64_t v0 = be_u64(temp);
        const uint64_t v1 = be_u64(data.slice(i, i + 8));
        v0 ^= v1;
        temp = u64_be(v0);
        temp = c.encrypt(temp);
    }

    if (temp.empty()) throw py::IndexError("bytearray index out of range");
    std::string mac_block = temp.raw();   // temp = bytearray(temp); temp[0] ^= 0x80
    mac_block[0] = static_cast<char>(static_cast<unsigned char>(mac_block[0]) ^ 0x80);

    XeCryptDes3 c3(twice(key).slice(0, 0x18));
    return {salt, c3.encrypt(py::bytes(mac_block))};
}

py::bytes UsbdSecXSMAuthenticationAcr(const BinLike& key, const BinLike& cert, const BinLike& data) {
    const py::bytes block = data.slice(0, 4) + cert.slice(0, 4);

    const py::bytes iv = XeCryptParveEcb(key, SBOX, data.slice_from(0x10));

    const py::bytes cd = XeCryptParveEcb(key, SBOX, block);

    const py::bytes ab = XeCryptParveCbcMac(key, SBOX, iv, UsbdSecPlainTextData.slice(0, 0x80));
    py::bytes output = XeCryptChainAndSumMac(cd, ab, UsbdSecPlainTextData.slice(0, 0x80));

    const uint64_t v0 = be_u64(output);
    const uint64_t v1 = be_u64(ab);
    return u64_be(v0 ^ v1);
}

XSM3State::XSM3State(const BinLike& xsm3_ident_packet) {
    reset();

    xsm3_set_identification_data(xsm3_ident_packet);
}

void XSM3State::reset() {
    xsm3_identification_data = std::nullopt;
    xsm3_random_console_data = std::nullopt;
    xsm3_console_id = std::nullopt;
    xsm3_challenge_init_hash = std::nullopt;
    xsm3_random_controller_data = std::nullopt;

    xsm3_random_console_data_enc = std::nullopt;
    xsm3_random_console_data_swap_enc = std::nullopt;
}

void XSM3State::xsm3_set_identification_data(const BinLike& ident_packet) {
    if (py::len(ident_packet) != 0x1D) throw py::AssertionError("Invalid identification packet!");

    if (!xsm3_verify_checksum(ident_packet)) {
        print_("[ Checksum failed when setting identification data! ]");
    }

    auto [v0, v1, v2, v3, v4, v5] = py::unpack_from<py::bytes, int64_t, int64_t, int64_t, int64_t, int64_t>(">15s 2H B H B", ident_packet, 5);
    xsm3_identification_data = py::bytearray::zeros(0x20);
    py::pack_into(">15s x 2H 2B H", *xsm3_identification_data, 0, v0, v1, v2, v3, v5, v4);
}

std::optional<py::bytearray> XSM3State::xsm3_do_challenge_init(const BinLike& challenge_packet) {
    if (py::len(challenge_packet) != 0x22) throw py::AssertionError("Invalid challenge packet!");

    if (!xsm3_verify_checksum(challenge_packet)) {
        print_("[ Checksum failed when validating challenge init! ]");
        return std::nullopt;
    }

    // decrypt the packet content using the static key from the keyvault
    py::bytes xsm3_decryption_buffer = UsbdSecXSM3AuthenticationCrypt(STATIC_KEY_1, challenge_packet.slice(5, 5 + 0x18), CryptMode::DECRYPT);
    // first 0x10 bytes are random data
    xsm3_random_console_data = py::bytearray(xsm3_decryption_buffer.slice(0, 0x10));
    // next 0x8 bytes are from the console certificate
    xsm3_console_id = xsm3_decryption_buffer.slice(0x10, 0x10 + 8);
    // last 4 bytes of the packet are the last 4 bytes of the MAC
    auto [salt, incoming_packet_mac] = UsbdSecXSM3AuthenticationMac(STATIC_KEY_2, std::nullopt, challenge_packet.slice(5, 5 + 0x18));

    // validate the MAC
    if (!(incoming_packet_mac.slice_from(4) == challenge_packet.slice(5 + 0x18, 5 + 0x18 + 4))) {
        print_("[ MAC failed when validating challenge init! ]");
        return std::nullopt;
    }

    // the random value is swapped at an 8 byte boundary
    const py::bytes random_console_data(xsm3_random_console_data->view());
    const py::bytes xsm3_random_console_data_swap = random_console_data.slice(8, 8 + 8) + random_console_data.slice(0, 8);
    // and then encrypted - the regular value encrypted with key 1, the swapped value encrypted with key 2
    xsm3_random_console_data_enc = UsbdSecXSM3AuthenticationCrypt(STATIC_KEY_1, random_console_data, CryptMode::ENCRYPT);
    xsm3_random_console_data_swap_enc = UsbdSecXSM3AuthenticationCrypt(STATIC_KEY_2, xsm3_random_console_data_swap, CryptMode::ENCRYPT);

    // generate random data
    xsm3_random_controller_data = XeCryptRandom(0x10);   // random.randbytes(0x10)

    // set header and packet length of challenge response
    py::bytearray xsm3_challenge_response = py::bytearray::zeros(0x30);
    xsm3_challenge_response[0] = 0x49;   // packet magic
    xsm3_challenge_response[1] = 0x4C;
    xsm3_challenge_response[4] = 0x28;   // packet length

    // copy random controller, random console data to the encryption buffer
    py::bytearray decryption_buffer(xsm3_decryption_buffer);
    decryption_buffer.set_slice(0, 0x10, *xsm3_random_controller_data);
    decryption_buffer.set_slice(0x10, 0x10 + 0x10, random_console_data);

    // save the sha1 hash of the decrypted contents for later
    xsm3_challenge_init_hash = XeCryptSha(py::list<py::bytes>{decryption_buffer.view()});

    // encrypt challenge response packet using the encrypted random key
    xsm3_challenge_response.set_slice(5, 5 + 0x20, UsbdSecXSM3AuthenticationCrypt(*xsm3_random_console_data_enc, decryption_buffer.view(), CryptMode::ENCRYPT));
    // calculate MAC using the encrypted swapped random key and use it to calculate ACR
    auto [salt2, xsm3_response_packet_mac] =
        UsbdSecXSM3AuthenticationMac(*xsm3_random_console_data_swap_enc, std::nullopt, xsm3_challenge_response.slice(5, 5 + 0x20).view());
    // calculate ACR and append to the end of the xsm3_challenge_response
    xsm3_challenge_response.set_slice(5 + 0x20, 5 + 0x20 + 8,
                                      UsbdSecXSMAuthenticationAcr(xsm3_response_packet_mac, *xsm3_console_id, xsm3_identification_data->view()));
    // calculate the checksum for the response packet
    xsm3_challenge_response[5 + 0x28] = xsm3_calculate_checksum(xsm3_challenge_response.view());

    xsm3_random_console_data = py::bytearray(random_console_data);
    xsm3_random_console_data->set_slice(0, 4, xsm3_random_controller_data->slice(0xC, 0xC + 4));
    xsm3_random_console_data->set_slice(4, 4 + 4, xsm3_random_console_data->slice(0xC, 0xC + 4).view());

    return xsm3_challenge_response.slice(0, 5 + xsm3_challenge_response.view()[4] + 1);
}

std::optional<py::bytearray> XSM3State::xsm3_do_challenge_verify(const BinLike& challenge_packet) {
    if (py::len(challenge_packet) != 0x16) throw py::AssertionError("Invalid challenge packet!");

    if (!xsm3_verify_checksum(challenge_packet)) {
        print_("[ Checksum failed when validating challenge verify! ]");
        return std::nullopt;
    }

    py::bytes xsm3_decryption_buffer = UsbdSecXSM3AuthenticationCrypt(need(xsm3_random_controller_data), challenge_packet.slice(5, 5 + 8), CryptMode::DECRYPT);
    if (!xsm3_random_console_data) throw py::TypeError("'NoneType' object does not support item assignment");
    xsm3_random_console_data->set_slice(8, 8 + 8, xsm3_decryption_buffer);

    auto [salt, xsm3_incoming_packet_mac] = UsbdSecXSM3AuthenticationMac(need(xsm3_challenge_init_hash), py::bytes(xsm3_random_console_data->view()),
                                                                         challenge_packet.slice(5, 5 + 8));

    if (!(xsm3_incoming_packet_mac == challenge_packet.slice(5 + 8, 5 + 8 + 8))) {
        print_("[ MAC failed when validating challenge verify! ]");
        return std::nullopt;
    }

    // set header and packet length of challenge response
    py::bytearray xsm3_challenge_response = py::bytearray::zeros(0x30);
    xsm3_challenge_response[0] = 0x49;   // packet magic
    xsm3_challenge_response[1] = 0x4C;
    xsm3_challenge_response[4] = 0x10;   // packet length

    // calculate the ACR value and encrypt it into the outgoing packet using the encrypted random
    xsm3_decryption_buffer = UsbdSecXSMAuthenticationAcr(need(xsm3_identification_data).view(), need(xsm3_console_id),
                                                         xsm3_random_console_data->slice_from(8).view());
    xsm3_challenge_response.set_slice(5, 5 + 8, UsbdSecXSM3AuthenticationCrypt(need(xsm3_random_console_data_enc), xsm3_decryption_buffer.slice(0, 8), CryptMode::ENCRYPT));
    // calculate the MAC of the encrypted packet and append it to the end
    auto [salt2, mac2] = UsbdSecXSM3AuthenticationMac(need(xsm3_random_console_data_swap_enc), py::bytes(xsm3_random_console_data->view()),
                                                      xsm3_challenge_response.slice(5, 5 + 8).view());
    xsm3_challenge_response.set_slice(5 + 8, 5 + 8 + 8, mac2);
    // calculate the checksum for the response packet
    xsm3_challenge_response[5 + 0x10] = xsm3_calculate_checksum(xsm3_challenge_response.view());

    return xsm3_challenge_response.slice(0, 5 + xsm3_challenge_response.view()[4] + 1);
}

void set_stream(std::ostream& out) { out_stream_ = &out; }

int64_t main() {
    const py::bytes kv_data = std::get<py::bytes>(build_lib::read_file(py::str("KV/banned.bin")));
    if (py::len(kv_data) < XECRYPT_KEYVAULT::sizeof_) buffer_too_small_(py::len(kv_data), XECRYPT_KEYVAULT::sizeof_);
    auto keyvault_field = [&kv_data](const char* name) {
        for (const KeyvaultField& f : XECRYPT_KEYVAULT::fields())
            if (f.name == py::str(name)) return kv_data.slice(f.offset, f.offset + f.size);
        throw py::AttributeError(std::string("field not found: ") + name);
    };

    STATIC_KEY_1 = keyvault_field("global_dev_2des_key_1");
    STATIC_KEY_2 = keyvault_field("global_dev_2des_key_2");
    DYNAMIC_KEY_1 = py::bytes::fromhex(py::str("F19D6F2CB1EE6AC4635336A54C11007D"));
    DYNAMIC_KEY_2 = py::bytes::fromhex(py::str("C45582C89FC3DAD28C1FBBCF3D049B6F"));

    // cid = bytearray.fromhex("086D40C2C6")
    // cid += bytearray.fromhex("808182")
    // hcid = XeCryptSha(cid)[:0x10]

    XSM3State xsm3(xsm3_id_data_ms_controller);   // with XSM3State(...) as xsm3:
    {
        std::optional<py::bytearray> xsm3_challenge_response = xsm3.xsm3_do_challenge_init(UsbdSecXSM3SetChallengeProtocolData);
        if (!xsm3_challenge_response) throw py::TypeError("object of type 'NoneType' has no len()");
        char hex[32];
        std::snprintf(hex, sizeof hex, "0x%llX", static_cast<unsigned long long>(py::len(*xsm3_challenge_response)));
        *out_stream_ << hex << "\n";
        xsm3_challenge_response = xsm3.xsm3_do_challenge_verify(UsbdSecXSM3GetResponseVerifyProtocolData1);
        // print(f"0x{len(xsm3_challenge_response):X}")
    }

    return 0;
}

}  // namespace codebreaker::formats::xbox::crypto::xsm3
