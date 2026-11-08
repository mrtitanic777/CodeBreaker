// codebreaker/formats/xbox/crypto/hypervisor.hpp -- typed C++ port of codebreaker.formats.xbox.crypto.hypervisor.
//
// Hypervisor analysis and manipulation tools.
//
// Typing notes (the Python's dicts become records):
//   * extract_hv_info returns a dict that is empty for an image shorter than 0x40000 bytes and has all of its
//     keys otherwise (the length guards inside the Python's body always hold once the image is that long):
//     std::optional<HvInfo>, nullopt for the empty dict.
//   * extract_challenge_ranges returns {"hash1_ranges": [...], "hash2_ranges": [...]}: the record
//     ExtractChallengeRange; an entry is a dict of address and size (hash 1) or of raw, type, address and size
//     (hash 2): ExtractChallengeRangeEntry with the keys it lacks left out.
//   * generate_hash1_results returns the (salt, hash_result) pairs as tuples of bytes.
//   * main() reads sys.argv[1:] in the Python; here the argument list is passed in. It prints with py::print and
//     leaves with py::SystemExit(2) for a bad command line, as sys.exit(2) does.
//   * calc_100f0_hash masks with 0xFFFFFFFFFFFFFF80 in the Python: the operand is an unsigned 32-bit value plus
//     0x7F, so the mask is applied to a non-negative int64_t.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::formats::xbox::crypto::hypervisor {

struct ExtractChallengeRange;
struct ExtractChallengeRangeEntry;

struct ExtractChallengeRange {
    py::list<py::ref<ExtractChallengeRangeEntry>> hash1_ranges;
    py::list<py::ref<ExtractChallengeRangeEntry>> hash2_ranges;
};
template <class F> void for_each_field(const ExtractChallengeRange& r, F&& f) {
    f("hash1_ranges", r.hash1_ranges);
    f("hash2_ranges", r.hash2_ranges);
}
template <class F> void for_each_field_mut(ExtractChallengeRange& r, F&& f) {
    f("hash1_ranges", r.hash1_ranges, true);
    f("hash2_ranges", r.hash2_ranges, true);
}

struct ExtractChallengeRangeEntry {
    std::optional<int64_t> address;
    std::optional<int64_t> size;
    std::optional<int64_t> raw;
    std::optional<py::str> type;
};
template <class F> void for_each_field(const ExtractChallengeRangeEntry& r, F&& f) {
    if (r.address) f("address", *r.address);
    if (r.size) f("size", *r.size);
    if (r.raw) f("raw", *r.raw);
    if (r.type) f("type", *r.type);
}
template <class F> void for_each_field_mut(ExtractChallengeRangeEntry& r, F&& f) {
    f("address", r.address, false);
    f("size", r.size, false);
    f("raw", r.raw, false);
    f("type", r.type, false);
}

// The result of extract_hv_info (the Python's dict with these keys, in this order).
struct HvInfo {
    int64_t size = 0;
    int64_t build = 0;
    int64_t qfe = 0;
    py::bytes white_key;
    py::bytes aes_key;
    py::bytes hash_key;
    bool keys_are_55 = false;
    py::bytes security_hash_100f0;
    py::list<std::tuple<int64_t, int64_t>> hash_table_entries;   // (address, size)
};
template <class F> void for_each_field(const HvInfo& r, F&& f) {
    f("size", r.size);
    f("build", r.build);
    f("qfe", r.qfe);
    f("white_key", r.white_key);
    f("aes_key", r.aes_key);
    f("hash_key", r.hash_key);
    f("keys_are_55", r.keys_are_55);
    f("security_hash_100f0", r.security_hash_100f0);
    f("hash_table_entries", r.hash_table_entries);
}
template <class F> void for_each_field_mut(HvInfo& r, F&& f) {
    f("size", r.size, true);
    f("build", r.build, true);
    f("qfe", r.qfe, true);
    f("white_key", r.white_key, true);
    f("aes_key", r.aes_key, true);
    f("hash_key", r.hash_key, true);
    f("keys_are_55", r.keys_are_55, true);
    f("security_hash_100f0", r.security_hash_100f0, true);
    f("hash_table_entries", r.hash_table_entries, true);
}

py::bytes clean_hypervisor(const py::bytes& dumped, const py::bytes& base);
std::optional<HvInfo> extract_hv_info(const py::bytes& data);
// struct.error when hv_data is shorter than the hash table (0x108A8 bytes)
py::bytes calc_100f0_hash(const py::bytes& hv_data, const py::bytes& ecc_data);
py::bytes decrypt_challenge_payload(const py::bytes& enc_data, const py::bytes& fbl_key);
ExtractChallengeRange extract_challenge_ranges(const py::bytes& dec_challenge);
py::list<std::tuple<py::bytes, py::bytes>> generate_hash1_results(const py::bytes& clean_hv, const py::list<py::bytes>& salts);
py::list<py::bytes> load_salt_array(const py::str& salt_file);

extern const py::str DESCRIPTION;

// `a` is sys.argv[1:].
void main(const py::list<py::str>& a);

}  // namespace codebreaker::formats::xbox::crypto::hypervisor
