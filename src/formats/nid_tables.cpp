// formats/nid_tables.cpp -- the console function-ID lookups (vita.hpp, ps3.hpp, psp.hpp, orbis.hpp): each is a binary search of a table
// read once, on first use, from config/nids/<console>.nids (nid_data.hpp). An unknown ID gives an empty name.
#include <array>
#include <string>

#include "core/crypto.hpp"
#include "formats/nid_data.hpp"
#include "formats/orbis.hpp"
#include "formats/ps3.hpp"
#include "formats/psp.hpp"
#include "formats/vita.hpp"

namespace cb::formats {

namespace vita {

std::string function_name(std::uint32_t nid) {
    static const nids::NumericTable& table = nids::numeric("vita.nids", "functions");
    const nids::Row* r = table.find(nid);
    return r ? std::string(r->name) : std::string();
}

std::string library_name(std::uint32_t nid) {
    static const nids::NumericTable& table = nids::numeric("vita.nids", "libraries");
    const nids::Row* r = table.find(nid);
    return r ? std::string(r->name) : std::string();
}

}  // namespace vita

namespace ps3 {

std::string function_name(std::uint32_t nid) {
    static const nids::NumericTable& table = nids::numeric("ps3.nids", "functions");
    const nids::Row* r = table.find(nid);
    return r ? std::string(r->name) : std::string();
}

std::string function_library(std::uint32_t nid) {
    static const nids::NumericTable& table = nids::numeric("ps3.nids", "functions");
    const nids::Row* r = table.find(nid);
    return r ? std::string(r->extra) : std::string();
}

std::string syscall_name(std::uint32_t number) {
    static const nids::NumericTable& table = nids::numeric("ps3.nids", "syscalls");
    const nids::Row* r = table.find(number);
    return r ? std::string(r->name) : std::string();
}

}  // namespace ps3

namespace psp {

std::string_view nid_name(std::uint32_t nid) {
    static const nids::NumericTable& table = nids::numeric("psp.nids", "functions");
    const nids::Row* r = table.find(nid);
    return r ? r->name : std::string_view();
}

}  // namespace psp

namespace orbis {

std::string nid_of(const std::string& name) {
    // the system's salt
    static constexpr std::uint8_t kSalt[16] = {0x51, 0x8D, 0x64, 0xA6, 0x35, 0xDE, 0xD8, 0xC1, 0xE6, 0xB0, 0x39, 0xB1, 0xC3, 0xE5, 0x52, 0x30};
    Bytes input(name.begin(), name.end());
    input.insert(input.end(), std::begin(kSalt), std::end(kSalt));
    const crypto::Sha1Digest digest = crypto::sha1(input);
    static constexpr char kAlphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";
    // the first eight bytes, reversed, as base 64: eleven digits (the last holds four bits)
    std::array<std::uint8_t, 8> reversed{};
    for (std::size_t k = 0; k < 8; ++k) reversed[k] = digest[7 - k];
    std::string out;
    for (std::size_t k = 0; k < 8; k += 3) {
        const std::uint32_t b0 = reversed[k], b1 = k + 1 < 8 ? reversed[k + 1] : 0, b2 = k + 2 < 8 ? reversed[k + 2] : 0;
        const std::uint32_t triple = b0 << 16 | b1 << 8 | b2;
        const std::size_t digits = k + 2 < 8 ? 4 : 3;   // the last group has two bytes: three digits
        for (std::size_t d = 0; d < digits; ++d) out += kAlphabet[(triple >> (18 - 6 * d)) & 63];
    }
    return out;
}

std::string function_name(const std::string& nid) {
    static const nids::TextTable& table = nids::text("orbis.nids", "functions");
    const nids::TextRow* r = table.find(nid);
    return r ? std::string(r->name) : std::string();
}

std::string syscall_name(std::uint32_t number) {
    static const nids::NumericTable& table = nids::numeric("orbis.nids", "syscalls");
    const nids::Row* r = table.find(number);
    return r ? std::string(r->name) : std::string();
}

}  // namespace orbis

}  // namespace cb::formats
