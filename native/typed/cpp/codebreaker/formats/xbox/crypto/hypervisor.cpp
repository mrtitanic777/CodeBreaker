// codebreaker/formats/xbox/crypto/hypervisor.cpp -- typed C++ port of codebreaker.formats.xbox.crypto.hypervisor.

#include "codebreaker/formats/xbox/crypto/hypervisor.hpp"

#include <type_traits>

#include "codebreaker/formats/xbox/crypto/MemCrypto.hpp"
#include "codebreaker/formats/xbox/crypto/XeCrypt.hpp"

namespace codebreaker::formats::xbox::crypto::hypervisor {

namespace {

// str(v) of a field of HvInfo: bytes are shown as hex (the Python's `v.hex().upper()` for bytes), the rest as
// str() shows it. (str() of a list of tuples is written out here: the runtime's repr does not take a list of
// tuples.)
py::str show(const py::bytes& v) { return v.hex().upper(); }
py::str show(const py::list<std::tuple<int64_t, int64_t>>& v) {
    py::str out("[");
    bool first = true;
    for (const auto& pair : v) {
        out = out + py::str(first ? "" : ", ") + py::str("(") + py::to_str(std::get<0>(pair)) + py::str(", ") +
              py::to_str(std::get<1>(pair)) + py::str(")");
        first = false;
    }
    return out + py::str("]");
}
py::str show(int64_t v) { return py::to_str(v); }
py::str show(bool v) { return py::to_str(v); }

}  // namespace

// Remove RGH patches from a dumped hypervisor using the base/offline version.
//
// Based on Hypervisor Manager's CleanHypervisor function.
py::bytes clean_hypervisor(const py::bytes& dumped, const py::bytes& base) {
    py::bytearray cleaned = py::bytearray(dumped.slice_to(0x40000));
    py::list<std::tuple<int64_t, int64_t, int64_t>> ranges = py::list<std::tuple<int64_t, int64_t, int64_t>>{std::tuple<int64_t, int64_t, int64_t>{0x08, 0x88, 0x0C}, std::tuple<int64_t, int64_t, int64_t>{0x34, 0x34, 0x0C}, std::tuple<int64_t, int64_t, int64_t>{0x70, 0x70, 0x04}, std::tuple<int64_t, int64_t, int64_t>{0x78, 0x78, 0xFF88}, std::tuple<int64_t, int64_t, int64_t>{0x10008, 0x10008, 0x18}, std::tuple<int64_t, int64_t, int64_t>{0x10130, 0x10130, 0x6190}, std::tuple<int64_t, int64_t, int64_t>{0x11018, 0x11018, 0x52BB}, std::tuple<int64_t, int64_t, int64_t>{0x16F00, 0x16F00, 0x29100}};
    for (const auto& [src_off, dst_off, size] : ranges) {
        if (src_off + size <= py::len(base) && dst_off + size <= py::len(cleaned)) {
            cleaned.set_slice(dst_off, dst_off + size, base.slice(src_off, src_off + size));
        }
    }
    return py::bytes(cleaned);
}

// Extract key information from a hypervisor image.
std::optional<HvInfo> extract_hv_info(const py::bytes& data) {
    if (py::len(data) < 0x40000) {
        return std::nullopt;   // the empty dict
    }

    // (From here on the image is at least 0x40000 bytes: the Python's `len(data) >= 0x10`, `>= 0x10130`,
    // `>= 0x10100` and `>= 0x108A8` guards all hold.)
    HvInfo info;
    info.size = py::len(data);

    // Version info at offset 0x02
    info.build = py::unpack_from1<int64_t>(">H", data, 0x02);
    info.qfe = py::unpack_from1<int64_t>(">H", data, 0x04);

    // Encryption keys (if present -- these are at 0x10100 in decrypted HV)
    info.white_key = data.slice(0x10100, 0x10110);
    info.aes_key = data.slice(0x10110, 0x10120);
    info.hash_key = data.slice(0x10120, 0x10130);
    info.keys_are_55 = true;
    for (const py::bytes* key : {&info.white_key, &info.aes_key, &info.hash_key}) {
        for (int64_t b : *key) {
            if (b != 0x55) info.keys_are_55 = false;
        }
    }

    // Security hash at 0x100F0
    info.security_hash_100f0 = data.slice(0x100F0, 0x10100);

    // Hash table address at 0x10878
    for (int64_t i = 0; i < 6; ++i) {
        int64_t addr = py::unpack_from1<int64_t>(">I", data, 0x10878 + i * 8);
        int64_t size = py::unpack_from1<int64_t>(">I", data, 0x10878 + i * 8 + 4);
        info.hash_table_entries.append(std::make_tuple(addr, size));
    }

    return info;
}

// Calculate the security hash at 0x100F0.
//
// Based on Hypervisor Manager's Calculate100F0 function.
py::bytes calc_100f0_hash(const py::bytes& hv_data, const py::bytes& ecc_data) {
    py::hashlib::Hash h = py::hashlib::sha1();

    for (int64_t i = 0; i < 6; ++i) {
        int64_t addr_raw = py::unpack_from1<int64_t>(">I", hv_data, 0x10878 + i * 8);
        int64_t size_raw = py::unpack_from1<int64_t>(">I", hv_data, 0x10878 + i * 8 + 4);

        // (addr_raw + 0x7F) & 0xFFFFFFFFFFFFFF80: the sum is non-negative, so the mask is -0x80 in int64_t
        int64_t addr = (addr_raw + 0x7F) & static_cast<int64_t>(-0x80);
        int64_t size = size_raw & 0xFFFFFF80;

        if (addr < size) {
            int64_t ecc_offset = py::shr(addr, 6);
            int64_t ecc_size = py::shr(size - addr, 6);
            if (ecc_offset + ecc_size <= py::len(ecc_data)) {
                h.update(ecc_data.slice(ecc_offset, ecc_offset + ecc_size));
            }
        }
    }

    return h.digest().slice(0, 0x10);
}

// Decrypt a code challenge payload.
//
// Based on Hypervisor Manager's DecryptChallengePayload function.
py::bytes decrypt_challenge_payload(const py::bytes& enc_data, const py::bytes& fbl_key) {
    py::bytes payload_hash = enc_data.slice(0x10, 0x20);
    py::bytes hmac_result = XeCrypt::XeCryptHmacSha(fbl_key, py::list<py::bytes>{payload_hash}).slice(0, 0x10);
    py::bytes data_section = enc_data.slice_from(0x20);

    py::bytearray decrypted = py::bytearray(enc_data);
    decrypted.set_slice(0x20, py::none, XeCrypt::XeCryptRc4::new_(hmac_result).decrypt(data_section));
    return py::bytes(decrypted);
}

// Extract hash address ranges from a decrypted code challenge.
//
// Based on Hypervisor Manager's OutputDigestRanges function.
ExtractChallengeRange extract_challenge_ranges(const py::bytes& dec_challenge) {
    ExtractChallengeRange result = ExtractChallengeRange{py::list<py::ref<ExtractChallengeRangeEntry>>{}, py::list<py::ref<ExtractChallengeRangeEntry>>{}};
    if (py::len(dec_challenge) <= 0x3B0) {
        return result;
    }
    int64_t raw = 0;
    int64_t i = 0;
    for (int64_t i_1 = 0; i_1 < 7; ++i_1) {
        i = i_1;
        raw = py::unpack_from1<int64_t>(">q", dec_challenge, 0x3B0 + i * 8);
        int64_t addr = py::shr(raw, 16) & 0xFFFFF;
        int64_t size = raw & 0xFFFF;
        result.hash1_ranges.append(std::make_shared<ExtractChallengeRangeEntry>(ExtractChallengeRangeEntry{addr, size, {}, {}}));
    }
    for (int64_t i_2 = 0; i_2 < 13; ++i_2) {
        i = i_2;
        raw = py::unpack_from1<int64_t>(">q", dec_challenge, 0x348 + i * 8);
        py::ref<ExtractChallengeRangeEntry> entry = std::make_shared<ExtractChallengeRangeEntry>(ExtractChallengeRangeEntry{{}, {}, raw, {}});
        if ((raw & 0x8000) != 0) {
            entry->type = "ECC";
            entry->address = py::shr(raw, 22) & 0xFFF;
            entry->size = static_cast<int64_t>(raw & 0x3FF) << 1;
        } else if ((raw & 0x7F) >= 16) {
            entry->type = "HV_ENC";
            entry->address = py::shr(raw, 16) & 0x3FFFF;
            entry->size = raw & 0x7F;
        } else {
            entry->type = "HV_DEC";
            entry->address = py::shr(raw, 16);
            entry->size = raw & 0x7F;
        }
        result.hash2_ranges.append(entry);
    }
    return result;
}

// Generate all Hash 1 results for all salts.
//
// Based on Hypervisor Manager's GenerateHash1Results function.
// Returns list of (salt, hash_result) tuples.
py::list<std::tuple<py::bytes, py::bytes>> generate_hash1_results(const py::bytes& clean_hv, const py::list<py::bytes>& salts) {
    py::list<std::tuple<py::bytes, py::bytes>> results;

    for (const py::bytes& salt : salts) {
        py::hashlib::Hash h = py::hashlib::sha1();
        h.update(salt.slice_to(0x10));
        h.update(clean_hv.slice(0x34, 0x34 + 0x40));
        h.update(clean_hv.slice(0x78, 0x78 + 0xFF88));
        h.update(clean_hv.slice(0x100C0, 0x100C0 + 0x40));
        h.update(clean_hv.slice(0x10350, 0x10350 + 0x5F70));
        h.update(clean_hv.slice(0x16EA0, 0x16EA0 + 0x9160));
        h.update(clean_hv.slice(0x20000, 0x20000 + 0xFFFF));
        h.update(clean_hv.slice(0x30000, 0x30000 + 0xFFFF));

        py::bytes digest = h.digest();
        py::bytes hash_result = digest.slice(0x0E, 0x0E + 0x06);
        results.append(std::make_tuple(salt, hash_result));
    }

    return results;
}

// Load the 256 salt array from a binary file.
py::list<py::bytes> load_salt_array(const py::str& salt_file) {
    py::bytes data = MemCrypto::read_file(salt_file);
    py::list<py::bytes> salts;
    for (int64_t i = 0; i < 256; ++i) {
        salts.append(data.slice(i * 0x10, (i + 1) * 0x10));
    }
    return salts;
}


const py::str DESCRIPTION("analyze an Xbox 360 hypervisor image (info; clean patches vs a base)");


void main(const py::list<py::str>& a) {
    if (a.empty() || a[0] == py::str("-h") || a[0] == py::str("--help")) {
        py::print(py::str("usage: hypervisor info <hv.bin>  |  hypervisor clean <dumped> <base> <out>"));
        return;
    }
    py::str op = a[0];
    if (op == py::str("info") && py::len(a) > 1) {
        py::bytes image = py::open_rb(a[1]).read();
        std::optional<HvInfo> info = extract_hv_info(image);
        if (info) {
            for_each_field(*info, [](const char* k, const auto& v) {
                py::print(py::percent(py::str("  %-20s %s"), py::str(k), show(v)));
            });
        }
    } else if (op == py::str("clean") && py::len(a) > 3) {
        py::bytes dumped = py::open_rb(a[1]).read();
        py::bytes base = py::open_rb(a[2]).read();
        py::bytes out = clean_hypervisor(dumped, base);
        py::file f = py::open(a[3], py::str("wb"));
        f.write(out);
        f.close();
        py::print(py::percent(py::str("cleaned HV (%d bytes) -> %s"), py::len(out), a[3]));
    } else {
        py::print(py::str("usage: hypervisor info <hv.bin>  |  hypervisor clean <dumped> <base> <out>"));
        throw py::SystemExit(2);
    }
}

}  // namespace codebreaker::formats::xbox::crypto::hypervisor
