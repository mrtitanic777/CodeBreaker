// codebreaker/formats/xbox/crypto/shadowboot.cpp -- typed C++ port of codebreaker.formats.xbox.crypto.shadowboot.

#include "codebreaker/formats/xbox/crypto/shadowboot.hpp"

#include <cstdio>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "json.hpp"   // py::json_parse (native/l2/src)
#include "codebreaker/formats/xbox/crypto/argparse_min.hpp"
#include "codebreaker/formats/xbox/crypto/keystore.hpp"

namespace codebreaker::formats::xbox::crypto::shadowboot {

using namespace XeCrypt;
using namespace build_lib;
using StreamIO::Endian;
using StreamIO::SEEK_CUR_;
using StreamIO::SEEK_END_;

const py::str __author__("Visual Studio");
const py::str __description__("A script to extract and build shadowboots");

// constants
const py::str& BIN_DIR() {
    static const py::str value = py::settings::get_str(py::str("xbox360_crypto_bin"));   // Free60 xenon-as / xenon-objcopy dir
    return value;
}
const py::str XELL_DIR("XeLL");
const py::str BUILD_DIR("Build");
const py::str PATCH_DIR("Patches");
const py::str MANIFEST_FILE("manifest.json");

// keys
std::optional<XeCryptRsaKey> ONE_BL_KEY;
std::optional<XeCryptRsaKey> SB_PRV_KEY;

// ------------------------------------------------------------------------------------------- exceptions
namespace {

// Python's NameError / UnboundLocalError and json.JSONDecodeError (a ValueError)
struct NameError : py::Exception {
    using py::Exception::Exception;
    const char* type() const override { return "NameError"; }
};
struct UnboundLocalError : NameError {
    using NameError::NameError;
    const char* type() const override { return "UnboundLocalError"; }
};
struct JSONDecodeError : py::ValueError {
    using py::ValueError::ValueError;
    const char* type() const override { return "JSONDecodeError"; }
};

[[noreturn]] void none_attribute(const char* attr) {
    throw py::AttributeError(std::string("'NoneType' object has no attribute '") + attr + "'");
}
[[noreturn]] void none_not_subscriptable() { throw py::TypeError("'NoneType' object is not subscriptable"); }

// the bytes of an attribute that is None (TypeError for what the Python subscripts or slices)
const py::bytes& sub_(const std::optional<py::bytes>& v) {
    if (!v) none_not_subscriptable();
    return *v;
}

// ------------------------------------------------------------------------------------------- Path
// pathlib.Path in its Windows spelling (see the header)
bool is_sep(char32_t c) { return c == U'\\' || c == U'/'; }

// the pieces of a Windows path: drive, root, and the names (no empty names, no ".")
struct PathParts {
    std::u32string drive;
    std::u32string root;
    std::vector<std::u32string> names;
};

PathParts split_path(const py::str& s) {
    std::u32string u = s.u32();
    auto dr = py::os::path::splitdrive_(u);
    PathParts p;
    p.drive = dr.first;
    for (char32_t& c : p.drive)
        if (c == U'/') c = U'\\';
    const std::u32string& rest = dr.second;
    size_t i = 0;
    if (!rest.empty() && is_sep(rest[0])) {
        p.root = U"\\";
        while (i < rest.size() && is_sep(rest[i])) i++;
    }
    while (i <= rest.size()) {
        size_t j = i;
        while (j < rest.size() && !is_sep(rest[j])) j++;
        std::u32string name = rest.substr(i, j - i);
        if (!name.empty() && name != U".") p.names.push_back(name);
        i = j + 1;
    }
    return p;
}

py::str join_parts(const PathParts& p) {
    std::u32string out = p.drive + p.root;
    for (size_t k = 0; k < p.names.size(); k++) {
        if (k) out += U'\\';
        out += p.names[k];
    }
    if (out.empty()) return py::str(".");
    return py::str(out);
}

std::u32string lower_(std::u32string s) {
    for (char32_t& c : s)
        if (c >= U'A' && c <= U'Z') c = c - U'A' + U'a';
    return s;
}

// Path(s)
py::str path_of(const py::str& s) { return join_parts(split_path(s)); }

// Path(a) / b  (ntpath.join's rules: a rooted b replaces the path, a b with another drive replaces all)
py::str path_join(const py::str& a, const py::str& b) {
    PathParts pa = split_path(a);
    PathParts pb = split_path(b);
    if (!pb.root.empty()) {
        PathParts r;
        r.drive = (!pb.drive.empty() || pa.drive.empty()) ? pb.drive : pa.drive;
        r.root = pb.root;
        r.names = pb.names;
        return join_parts(r);
    }
    if (!pb.drive.empty() && lower_(pb.drive) != lower_(pa.drive)) return join_parts(pb);
    PathParts r = pa;
    if (!pb.drive.empty()) r.drive = pb.drive;
    for (const std::u32string& n : pb.names) r.names.push_back(n);
    return join_parts(r);
}

// Path.absolute()
py::str path_absolute(const py::str& p) {
    PathParts parts = split_path(p);
    if (!parts.drive.empty() && !parts.root.empty()) return join_parts(parts);
    return path_join(py::os::getcwd(), join_parts(parts));
}

bool path_is_file(const py::str& p) { return py::os::path::isfile(p); }
bool path_is_dir(const py::str& p) { return py::os::path::isdir(p); }

// Path.mkdir(): FileExistsError if the name is taken, FileNotFoundError if the parent is missing
void path_mkdir(const py::str& path) {
    std::error_code ec;
    std::filesystem::path p = std::filesystem::u8path(path.utf8());
    if (std::filesystem::exists(p, ec)) {
        throw py::FileExistsError("[WinError 183] Cannot create a file when that file already exists: " + py::repr(path).utf8());
    }
    std::filesystem::create_directory(p, ec);
    if (ec) py::os::raise_os_(ec, path);
}

// Path.read_bytes()
py::bytes read_bytes(const py::str& path) {
    py::file f = py::open(path, py::str("rb"));
    py::bytes data = f.read();
    f.close();
    return data;
}

// Path.read_text()  (the locale's encoding, as open() without an encoding)
py::str read_text(const py::str& path) {
    py::file f = py::open(path, py::str("r"));
    py::str text = f.read_text();
    f.close();
    return text;
}

// Path.write_bytes(data)
void write_bytes(const py::str& path, const py::bytes& data) {
    py::file f = py::open(path, py::str("wb"));
    f.write(data);
    f.close();
}

const py::bytes& need_bytes(const std::optional<py::bytes>& v) {
    if (!v) throw py::TypeError("a bytes-like object is required, not 'NoneType'");
    return *v;
}

void say(const std::string& s) { py::print(py::str(s)); }

py::str none_or(const std::optional<int64_t>& v) { return py::str(v ? std::to_string(*v) : std::string("None")); }
py::str none_or(const std::optional<py::str>& v) { return v ? *v : py::str("None"); }

// XeCryptHmacSha(key, nonce)[:0x10]; the key is None when the key store has no 1BL_key.bin (cryptography: TypeError)
py::bytes derive_key(const std::optional<py::bytes>& key, const py::bytes& nonce) {
    if (!key) throw py::TypeError("key must be bytes-like");
    return XeCryptHmacSha(*key, py::list<py::bytes>{nonce}).slice(0, 0x10);
}

// what the Python's StreamIO does not have (see the header)
[[noreturn]] void no_stream_attribute(const char* name) {
    throw py::AttributeError(std::string("'StreamIO' object has no attribute '") + name + "'");
}
void set_label(StreamIO::StreamIO&, const py::str&) { no_stream_attribute("set_label"); }
void goto_label(StreamIO::StreamIO&, const py::str&, int64_t = 0) { no_stream_attribute("goto_label"); }
int64_t get_label(StreamIO::StreamIO&, const py::str&) { no_stream_attribute("get_label"); }

}  // namespace

// ------------------------------------------------------------------------------------------- functions
int64_t checksum_file(const py::str& filename) {
    int64_t cksm = 0;
    py::file f = py::open(filename, py::str("rb"));
    while (true) {
        py::bytes b = f.read(BUFFER_SIZE);
        if (b.empty()) break;
        cksm = py::crc32(b, cksm);
    }
    f.close();
    return cksm;
}

bool verify_checksum(const py::str& filename, int64_t cksm) {
    if (cksm == 0 || !py::os::path::isfile(filename)) return true;
    return checksum_file(filename) == cksm;
}

py::str pad_hex(int64_t i) {
    py::str h = py::pack(">I", i).hex();
    while (py::len(h) < 8) h = py::str("0") + h;
    return h;
}

py::str path_type(const py::str& value) { return path_of(value); }

py::bytes unecc(const py::str& path, int64_t block_size, int64_t spare_size) {
    std::string bio;
    py::file f = py::open(path, py::str("rb"));
    f.seek(0, 2);
    int64_t size = f.tell();
    f.seek(0);
    while (f.tell() < size) {
        py::bytes data = f.read(block_size);
        // (a seek to before the start of the file is an OSError)
        if (f.tell() + spare_size < 0) throw py::OSError("[Errno 22] Invalid argument");
        f.seek(spare_size, 1);
        bio += data.raw();
    }
    f.close();
    return py::bytes(bio);
}

// ------------------------------------------------------------------------------------------- ShadowbootImage
ShadowbootImage::ShadowbootImage() { reset(); }

ShadowbootImage ShadowbootImage::parse(const py::bytes& data, bool perform_checks, bool parse_patches) {
    ShadowbootImage img;

    // with BytesIO(data) as img._stream:
    img._stream = std::make_shared<StreamIO::StreamIO>(data);
    img._stream_closed = false;
    try {
        img.flash_header = img.read_header<FLASH_HEADER>();

        img.map_shadowboot();

        img.parse_smc();
        img.parse_smc_config();
        img.parse_keyvault();

        img.parse_sb_2bl();

        img.parse_sc_3bl();

        img.parse_sd_4bl();

        img.parse_se_5bl();
        img.decompress_se_5bl();

        img.parse_metadata();

        if (parse_patches) img.parse_patches();

        if (perform_checks) {
            if (!img.check_signature_sb_2bl()) throw py::Exception("Invalid SB signature");
            if (!img.check_signature_sc_3bl()) throw py::Exception("Invalid SC signature");
            if (!img.check_signature_sd_4bl()) throw py::Exception("Invalid SD signature");
            if (!img.check_hash_sd_4bl()) throw py::Exception("Invalid SD digest");
            if (!img.check_hash_se_5bl()) throw py::Exception("Invalid SE digest");
        }
    } catch (...) {
        img._stream_closed = true;
        throw;
    }
    img._stream_closed = true;
    return img;
}

py::bytes ShadowbootImage::create(const std::optional<py::bytes>& sb_data_in, const std::optional<py::bytes>& sc_data_in,
                                  const std::optional<py::bytes>& sd_data_in, const std::optional<py::bytes>& se_data_in,
                                  const std::optional<py::bytes>& smc_data_in,
                                  const std::optional<std::tuple<py::bytes, std::optional<py::bytes>>>& kv_data_in,
                                  const std::optional<py::bytes>& patches_in, bool test_kit, std::optional<int64_t> build_version) {
    // probably never going to actually implement this since I have building working already
    // img = ShadowbootImage()

    py::bytes new_sb_nonce = XeCryptRandom(0x10);
    py::bytes new_sb_key = derive_key(XECRYPT_1BL_KEY(), new_sb_nonce);

    py::bytes new_sc_nonce = XeCryptRandom(0x10);
    py::bytes new_sc_key = derive_key(py::bytes::zeros(0x10), new_sc_nonce);

    py::bytes new_sd_nonce = XeCryptRandom(0x10);
    py::bytes new_sd_key = derive_key(new_sc_key, new_sd_nonce);

    py::bytes new_se_nonce = XeCryptRandom(0x10);
    py::bytes new_se_key = derive_key(new_sd_key, new_se_nonce);

    BLHeader sb_hdr = BLHeader::parse(sub_(sb_data_in).slice(0, 0x20));
    if (!(sb_hdr.magic && *sb_hdr.magic == py::bytes("SB"))) throw py::AssertionError("Invalid SB bootloader!");
    py::bytes sb_data = sb_data_in->slice(0x20, sb_data_in->size());

    BLHeader sc_hdr = BLHeader::parse(sub_(sc_data_in).slice(0, 0x20));
    if (!(sc_hdr.magic && *sc_hdr.magic == py::bytes("SC"))) throw py::AssertionError("Invalid SC bootloader!");
    py::bytes sc_data = sc_data_in->slice(0x20, sc_data_in->size());

    BLHeader sd_hdr = BLHeader::parse(sub_(sd_data_in).slice(0, 0x20));
    if (!(sd_hdr.magic && *sd_hdr.magic == py::bytes("SD"))) throw py::AssertionError("Invalid SD bootloader!");
    py::bytes sd_data = sd_data_in->slice(0x20, sd_data_in->size());

    py::bytes se_data = sub_(se_data_in);
    BLHeader se_hdr = BLHeader::parse(se_data.slice(0, 0x20));
    if (se_hdr.magic && *se_hdr.magic == py::bytes("SE")) {   // compressed with header
        se_data = decompress_se(se_data);
    } else {
        // create header from scratch if one wasn't provided
        if (!build_version) throw py::StructError("required argument is not an integer");
        se_hdr = BLHeader::parse(py::pack(">2s 3H I 4x 16x", py::bytes("SE"), *build_version, static_cast<int64_t>(0x8000), static_cast<int64_t>(0),
                                          static_cast<int64_t>(0)));
    }

    // error if not decompressed properly
    if (!(se_data.slice(0, 2) == py::bytes("^N"))) throw py::AssertionError("Invalid SE bootloader!");

    // create NAND header
    FLASH_HEADER flash_header;
    flash_header.magic = 0xFF4F;
    // (ctypes takes an int into the 16 bits of the field; anything else is a TypeError)
    if (!build_version) throw py::TypeError("an integer is required");
    flash_header.build = static_cast<WORD>(*build_version);
    flash_header.qfe = 0x8000;
    flash_header.flags = 0;

    {
        // bytes(create_string_buffer(b"\xA9 2005-2023 Microsoft Corporation. All rights reserved", 0x40))
        const std::string text = "\xA9 2005-2023 Microsoft Corporation. All rights reserved";
        flash_header.copyright.fill(0);
        for (size_t i = 0; i < text.size() && i < flash_header.copyright.size(); ++i) flash_header.copyright[i] = static_cast<BYTE>(text[i]);
    }
    flash_header.patch_slots = 2;
    flash_header.kv_version = 0x712;
    flash_header.patch_slot_size = 0x10000;

    sb_hdr.nonce = new_sb_nonce;
    sc_hdr.nonce = new_sc_nonce;
    sd_hdr.nonce = new_sd_nonce;
    se_hdr.nonce = new_se_nonce;

    StreamIO::StreamIO img(Endian::BIG);   // with StreamIO(endian=Endian.BIG) as img:
    // write blank NAND header
    set_label(img, py::str("flash_hdr_offs"));
    img.write(py::bytes::zeros(FLASH_HEADER::SIZE));
    set_label(img, py::str("flash_hdr_end"));

    std::optional<py::bytes> smc_data = smc_data_in;
    if (smc_data) {
        smc_data = XeCryptSmcEncrypt(*smc_data);
        img.write(py::bytes::zeros(0x1000 - FLASH_HEADER::SIZE));
        set_label(img, py::str("smc_offs"));
        img.write(*smc_data);
        set_label(img, py::str("smc_end"));
        flash_header.smc_offset = 0x1000;
        flash_header.smc_length = static_cast<DWORD>(smc_data->size());
        flash_header.entry = static_cast<DWORD>(0x1000 + flash_header.smc_length);
    } else {
        flash_header.smc_offset = 0x1000;
        flash_header.smc_length = 0;
        flash_header.entry = static_cast<DWORD>(FLASH_HEADER::SIZE);
    }

    if (kv_data_in) {
        const py::bytes& cpu_key = std::get<0>(*kv_data_in);
        py::bytes kv_data = XeCryptKeyVaultEncrypt(cpu_key, py::bytes(need_bytes(std::get<1>(*kv_data_in))));
        flash_header.kv_offset = 0x4000;
        flash_header.kv_length = 0x4000;
        set_label(img, py::str("kv_offs"));
        img.write(kv_data);
        set_label(img, py::str("kv_end"));
        flash_header.entry += 0x4000;
    } else {
        flash_header.kv_offset = 0x4000;
        flash_header.kv_length = 0;
    }

    set_label(img, py::str("sb_hdr_offs"));
    img.write(sb_hdr.__bytes__());
    set_label(img, py::str("sb_data_offs"));
    img.write(sb_data);
    set_label(img, py::str("sb_data_end"));

    if (test_kit) {
        goto_label(img, py::str("sb_hdr_offs"), 0x1348);
        if (!(img.read(4) == py::bytes::fromhex(py::str("419A0014")))) throw py::AssertionError("Original bytes mismatch!");
        img.seek(-4, SEEK_CUR_);
        img.write(py::bytes::fromhex(py::str("48000194")));
        goto_label(img, py::str("sb_data_end"));
    }

    set_label(img, py::str("sc_hdr_offs"));
    img.write(sc_hdr.__bytes__());
    set_label(img, py::str("sc_data_offs"));
    img.write(sc_data);
    set_label(img, py::str("sc_data_end"));

    set_label(img, py::str("sd_hdr_offs"));
    img.write(sd_hdr.__bytes__());
    set_label(img, py::str("sd_data_offs"));
    img.write(sd_data);
    set_label(img, py::str("sd_data_end"));

    // apply patches to SE (HV/kernel)
    if (patches_in) {
        StreamIO::StreamIO bio(se_data);
        patch_in_place(bio, *patches_in);
        se_data = bio.getvalue();
    }

    se_data = compress_se(se_data, false);
    se_hdr.size = py::len(se_data) + 0x30;
    set_label(img, py::str("se_hdr_offs"));
    img.write(se_hdr.__bytes__());
    set_label(img, py::str("se_data_offs"));
    img.write(py::pack(">8x I 4x", static_cast<int64_t>(0x280000)));
    img.write(se_data);
    img.write(py::bytes::zeros(se_hdr.padded_size() - *se_hdr.size));
    set_label(img, py::str("se_data_end"));

    py::bytes se_hash = calc_se_hash_in_place(img, get_label(img, py::str("se_hdr_offs")));

    goto_label(img, py::str("sd_hdr_offs"), 0x24C);
    img.write(se_hash);
    img.seek(0, SEEK_END_);

    if (!SB_PRV_KEY) none_attribute("sig_create");
    sign_bldr_in_place(img, get_label(img, py::str("sd_hdr_offs")), *SB_PRV_KEY);

    encrypt_bldr_in_place(new_sb_key, img, get_label(img, py::str("sb_hdr_offs")));
    encrypt_bldr_in_place(new_sc_key, img, get_label(img, py::str("sc_hdr_offs")));
    encrypt_bldr_in_place(new_sd_key, img, get_label(img, py::str("sd_hdr_offs")));
    encrypt_bldr_in_place(new_se_key, img, get_label(img, py::str("se_hdr_offs")));

    set_label(img, py::str("size"));

    img.write(py::bytes::zeros(calc_pad_size(get_label(img, py::str("size")), 0x1000)));

    set_label(img, py::str("size"));

    // point to right after the shadowboot image (idk if this matters)
    flash_header.sys_upd_addr = static_cast<DWORD>(get_label(img, py::str("size")));

    img.seek(0);
    img.write(flash_header.to_bytes());

    py::bytes data = img.getvalue();

    if (!test_kit) {
        ShadowbootImage::parse(data);
        say("Final image verified!");
    } else {
        say("Final image created!");
    }

    return data;
}

ShadowbootImage& ShadowbootImage::__enter__() { return *this; }

void ShadowbootImage::__exit__() {}

void ShadowbootImage::reset() {
    _stream.reset();
    _stream_closed = false;
    img_map = py::dict<py::str, ImageMapEntry>();
    flash_header.reset();
    smc_data.reset();
    smc_config_data.reset();
    kv_data.reset();
    sb_data.reset();
    sc_data.reset();
    sd_data.reset();
    se_data.reset();
    kernel_data.reset();
    hypervisor_data.reset();
    patches = py::list<Patch>();
    sb_sig.reset();
    sb_pub_key.reset();
    sc_nonce.reset();
    sc_salt.reset();
    sd_salt.reset();
    sd_digest.reset();
    sc_sig.reset();
    sd_sig.reset();
    sd_pub_key.reset();
    sf_nonce.reset();
    sf_salt.reset();
    se_digest.reset();
    console_type.reset();
    smc_version.reset();
    sb_build.reset();
    sc_build.reset();
    sd_build.reset();
    se_build.reset();
    kernel_version.reset();
    hypervisor_version.reset();
    is_retail = false;
    is_testkit = false;
}

namespace {
// self._stream.<name>: AttributeError while it is None, ValueError once closed
StreamIO::StreamIO& open_stream(const ShadowbootImage& img, const char* name) {
    if (!img._stream) none_attribute(name);
    if (img._stream_closed) throw py::ValueError("I/O operation on closed file.");
    return *img._stream;
}
}  // namespace

py::bytes ShadowbootImage::read(int64_t size) { return open_stream(*this, "read").read(size); }

int64_t ShadowbootImage::write(const py::bytes& data) { return open_stream(*this, "write").write(data); }

int64_t ShadowbootImage::seek(int64_t offset, int64_t whence) { return open_stream(*this, "seek").seek(offset, whence); }

int64_t ShadowbootImage::tell() { return open_stream(*this, "tell").tell(); }

py::bytes ShadowbootImage::getvalue() { return open_stream(*this, "getvalue").getvalue(); }

void ShadowbootImage::map_shadowboot() {
    if (!flash_header) none_attribute("smc_offset");
    ImageMapEntry smc;
    smc.offset = flash_header->smc_offset;
    smc.size = flash_header->smc_length;
    img_map.set(py::str("SMC"), smc);
    ImageMapEntry kv;
    kv.offset = flash_header->kv_offset;
    kv.size = flash_header->kv_length;
    img_map.set(py::str("KV"), kv);
    seek(flash_header->entry);
    for (int64_t i = 0; i < 4; ++i) {
        // all of them are the same
        BLHeader hdr = BLHeader::parse(read(0x20));
        py::str bldr_name = hdr.magic->decode("utf-8");
        ImageMapEntry entry;
        entry.offset = tell() - hdr.header_size();
        entry.size = *hdr.size;
        entry.pad_size = hdr.padded_size();
        entry.header = hdr;
        img_map.set(bldr_name, entry);

        // derive keys
        if (bldr_name == py::str("SB")) {
            img_map[bldr_name].key = derive_key(XECRYPT_1BL_KEY(), *hdr.nonce);
            img_map[bldr_name].header->nonce = py::bytes::zeros(0x10);
        } else if (bldr_name == py::str("SC")) {
            img_map[bldr_name].key = derive_key(py::bytes::zeros(0x10), *hdr.nonce);
            img_map[bldr_name].header->nonce = py::bytes::zeros(0x10);
        } else if (bldr_name == py::str("SD")) {
            const ImageMapEntry& sc = img_map.at(py::str("SC"));
            if (!sc.key) throw py::KeyError("key");
            img_map[bldr_name].key = derive_key(*sc.key, *hdr.nonce);
            img_map[bldr_name].header->nonce = py::bytes::zeros(0x10);
        } else if (bldr_name == py::str("SE")) {
            const ImageMapEntry& sd = img_map.at(py::str("SD"));
            if (!sd.key) throw py::KeyError("key");
            img_map[bldr_name].key = derive_key(*sd.key, *hdr.nonce);
            img_map[bldr_name].header->nonce = py::bytes::zeros(0x10);
        }
        // don't seek for the last entry
        if (i != 3) seek(*hdr.size - 0x20, SEEK_CUR_);
    }
}

void ShadowbootImage::parse_smc() {
    if (img_map.at(py::str("SMC")).offset > 0) {
        seek(img_map.at(py::str("SMC")).offset);
        smc_data = XeCryptSmcDecrypt(read(img_map.at(py::str("SMC")).size));
    }
}

void ShadowbootImage::parse_smc_config() {
    if (!flash_header) none_attribute("smc_config_offset");
    if (flash_header->smc_config_offset > 0) {
        seek(flash_header->smc_config_offset);
        smc_config_data = read(SMC_CONFIG_SIZE);
    }
}

void ShadowbootImage::parse_keyvault() {
    if (img_map.at(py::str("KV")).offset > 0) {
        seek(img_map.at(py::str("KV")).offset);
        kv_data = read(img_map.at(py::str("KV")).size);
    }
}

namespace {
// parse_sb_2bl / parse_sc_3bl / parse_sd_4bl: the header and the RC4-decrypted rest; `size` is what is read after
// the header (the padded size; for the SE the size)
py::bytes decrypt_bootloader(ShadowbootImage& img, const char* name, bool padded) {
    const py::str key_name(name);
    // seek to the start  (self._stream.seek is looked up first: AttributeError while the stream is None)
    if (!img._stream) none_attribute("seek");
    img.seek(img.img_map.at(key_name).offset + 0x20);
    // read out the encrypted bytes after the header
    const ImageMapEntry& e = img.img_map.at(key_name);
    py::bytes enc = img.read((padded ? *e.pad_size : e.size) - 0x20);
    // decrypt
    const ImageMapEntry& e2 = img.img_map.at(key_name);
    if (!e2.key) throw py::KeyError("key");
    py::bytes dec = XeCryptRc4::new_(*e2.key).decrypt(enc);
    // prepend the header to the decrypted data
    return e2.header->__bytes__() + dec;
}
}  // namespace

void ShadowbootImage::parse_sb_2bl() { sb_data = decrypt_bootloader(*this, "SB", true); }
void ShadowbootImage::parse_sc_3bl() { sc_data = decrypt_bootloader(*this, "SC", true); }
void ShadowbootImage::parse_sd_4bl() { sd_data = decrypt_bootloader(*this, "SD", true); }
void ShadowbootImage::parse_se_5bl() { se_data = decrypt_bootloader(*this, "SE", false); }

void ShadowbootImage::decompress_se_5bl() {
    py::bytes data = decompress_se(need_bytes(se_data));
    hypervisor_data = data.slice(0, HYPERVISOR_SIZE);
    kernel_data = data.slice(HYPERVISOR_SIZE, data.size());
}

void ShadowbootImage::parse_hypervisor() {
    // The Python only seeks a BytesIO of the hypervisor data (0x105B8 dev PIRS public key, 0x106C8 expansion public
    // key, 0x10A18 XMACS public key, 0x11008 master public key, 0x11988 retail PIRS public key): nothing is read.
}

void ShadowbootImage::parse_patches() {
    static const py::bytes bldr_end = py::bytes::fromhex(py::str("4BFFFFB0000000000000000000000000"));
    if (!sd_data) none_attribute("find");
    int64_t end_loc = sd_data->find(bldr_end) + bldr_end.size();
    if (end_loc == -1 || end_loc == py::len(*sd_data)) return;   // no patches
    // bldr_end = bytes.fromhex("386000004E8000200000000000000000")  (an older marker, commented out in the Python)

    // print("Patches found!")
    StreamIO::StreamIO sio(*sd_data, Endian::BIG);
    sio.seek(end_loc);   // not static by any means
    py::bytes patch_loader = sio.read_ubytes(0x40);   // the loader code for patches
    Patch loader;
    loader.offset = end_loc;
    loader.size_bytes = py::len(patch_loader);
    loader.patch_loader = patch_loader;
    patches.append(loader);
    while (true) {
        int64_t offset = sio.tell();
        int64_t address = sio.read_uint32();
        if (address == 0xFFFFFFFF) break;   // end of patches
        int64_t size_int32 = sio.read_uint32();
        int64_t size_bytes = size_int32 * 4;
        py::bytes patch_code = sio.read_ubytes(size_bytes);
        py::bytes whole_patch = py::pack(">2I", address, size_int32) + patch_code;
        Patch p;
        p.offset = offset;
        p.address = address;
        p.size_int32 = size_int32;
        p.size_bytes = size_bytes * 4;
        p.patch_code = patch_code;
        p.whole_patch = whole_patch;
        patches.append(p);
    }
}

void ShadowbootImage::parse_metadata() {
    // SB
    const py::bytes& sb = sub_(sb_data);
    sb_sig = sb.slice(64, 64 + 256);
    sb_pub_key = XeCryptRsaKey(sb.slice(616, 616 + 272));   // verifies SC and SD
    sc_nonce = sb.slice(888, 888 + 0x10);
    sc_salt = sb.slice(904, 904 + 0xA);
    sd_salt = sb.slice(914, 914 + 0xA);
    sd_digest = sb.slice(924, 924 + 0x14);
    // SC
    sc_sig = sub_(sc_data).slice(32, 32 + 256);
    // SD
    const py::bytes& sd = sub_(sd_data);
    sd_sig = sd.slice(32, 32 + 256);
    sd_pub_key = XeCryptRsaKey(sd.slice(288, 288 + 272));   // verifies SE
    sf_nonce = sd.slice(560, 560 + 0x10);
    sf_salt = sd.slice(576, 576 + 0xA);
    se_digest = sd.slice(588, 588 + 0x14);
    // SMC and kernel
    if (smc_data && !(*smc_data == py::bytes(""))) {
        int64_t num = (*smc_data)[256];
        static const std::vector<py::str> console_types = {py::str("Error"),   py::str("Xenon"),  py::str("Zephyr"),
                                                           py::str("Falcon"),  py::str("Jasper"), py::str("Trinity"),
                                                           py::str("Corona"),  py::str("Winchester"), py::str("unknown"),
                                                           py::str("unknown")};
        const int64_t which = (num >> 4) & 15;
        if (which >= static_cast<int64_t>(console_types.size())) throw py::IndexError("list index out of range");
        console_type = console_types[static_cast<size_t>(which)];
        smc_version = py::str(std::to_string((num >> 4) & 15) + "." + std::to_string(num & 15) + " (" + std::to_string((*smc_data)[257]) +
                              "." + std::to_string((*smc_data)[258]) + ")");
    }
    // (self.kernel_version,) = unpack_from(">H", self.kernel_data, 0x40C)   (commented out in the Python)
    auto build_of = [&](const char* name) -> std::optional<int64_t> {
        const ImageMapEntry& e = img_map.at(py::str(name));
        if (!e.header) throw py::KeyError("header");
        return e.header->build;
    };
    sb_build = build_of("SB");
    sc_build = build_of("SC");
    sd_build = build_of("SD");
    se_build = build_of("SE");
    if (!hypervisor_data) throw py::TypeError("a bytes-like object is required, not 'NoneType'");
    // (self.hypervisor_version,) = unpack_from(">H", self.hypervisor_data, 0x2)
    hypervisor_version = py::unpack_from1<int64_t>(">H", *hypervisor_data, 0x2);
    kernel_version = se_build;
    is_retail = (*hypervisor_data)[0] == 0x4E;
    if (!kernel_data) throw py::TypeError("argument of type 'NoneType' is not iterable");
    is_testkit = kernel_data->contains(py::bytes::fromhex(py::str("5C746573746B69745C")));
}

bool ShadowbootImage::check_signature_sb_2bl() {
    const py::bytes& sb = sub_(sb_data);
    py::bytes sb_hash = XeCryptRotSumSha(sb.slice(0, 0x10) + sb.slice(0x140, sb.size()));   // skips the nonce and signature
    if (!ONE_BL_KEY) none_attribute("sig_verify");
    return ONE_BL_KEY->sig_verify(need_bytes(sb_sig), sb_hash, XECRYPT_1BL_SALT);
}

bool ShadowbootImage::check_signature_sc_3bl() {
    const py::bytes& sc = sub_(sc_data);
    py::bytes sc_hash = XeCryptRotSumSha(sc.slice(0, 0x10) + sc.slice(0x120, sc.size()));   // skips the nonce and signature
    if (!sb_pub_key) none_attribute("sig_verify");
    return sb_pub_key->sig_verify(need_bytes(sc_sig), sc_hash, need_bytes(sc_salt));
}

bool ShadowbootImage::check_signature_sd_4bl() {
    const py::bytes& sd = sub_(sd_data);
    py::bytes sd_hash = XeCryptRotSumSha(sd.slice(0, 0x10) + sd.slice(0x120, sd.size()));   // skips the nonce and signature
    if (!sb_pub_key) none_attribute("sig_verify");
    return sb_pub_key->sig_verify(need_bytes(sd_sig), sd_hash, need_bytes(sd_salt));
}

bool ShadowbootImage::check_hash_sd_4bl() {
    if (!sd_digest) throw py::TypeError("object of type 'NoneType' has no len()");
    if (!(*sd_digest == py::bytes::zeros(sd_digest->size()))) {
        const py::bytes& sd = sub_(sd_data);
        return XeCryptRotSumSha(sd.slice(0, 0x10) + sd.slice(0x20, sd.size())) == *sd_digest;
    }
    return true;
}

bool ShadowbootImage::check_hash_se_5bl() {
    if (!se_digest) throw py::TypeError("object of type 'NoneType' has no len()");
    if (!(*se_digest == py::bytes::zeros(se_digest->size()))) {
        const py::bytes& se = sub_(se_data);
        return XeCryptRotSumSha(se.slice(0, 0x10) + se.slice(0x20, se.size()) + py::bytes::zeros(calc_bldr_pad_size(py::len(se)))) == *se_digest;
    }
    return true;
}

void ShadowbootImage::print_info() {
    say("Console Type:   " + none_or(console_type).utf8());
    say("SMC Version:    " + none_or(smc_version).utf8());
    say("SB Version:     " + none_or(sb_build).utf8());
    say("SC Version:     " + none_or(sc_build).utf8());
    say("SD Version:     " + none_or(sd_build).utf8());
    say("SE Version:     " + none_or(se_build).utf8());
    say("HV Version:     " + none_or(hypervisor_version).utf8());
    say("Kernel Version: " + none_or(kernel_version).utf8());

    if (is_retail) {
        say("Main Menu:      Dashboard");
    } else {
        say("Main Menu:      XShell");
    }

    if (is_testkit) {
        say("Hardware:       Test Kit");
    } else {
        say("Hardware:       Development Kit");
    }
}

// ------------------------------------------------------------------------------------------- main
namespace {

using argparse_min::ArgumentError;
using argparse_min::ArgumentParser;
using argparse_min::Namespace;

// the parsed command line (argparse's Namespace of the whole tool)
struct Args {
    std::optional<py::str> command;
    std::optional<py::str> input, output, manifest, build_dir, revision, type;
    std::optional<int64_t> kernel_version;   // split's -k / --kernel
    bool nochecks = false, flash = false, all = false, smc = false, keyvault = false, sb = false, sc = false, sd = false, se = false;
    bool kernel = false, hypervisor = false, loader = false, patches = false;   // extract's flags
};

const char* const kCommands[] = {"build", "extract", "info", "split", "test"};

std::string top_usage(const py::str& prog) { return "usage: " + prog.utf8() + " [-h] {build,extract,info,split,test} ...\n"; }

std::string top_help(const py::str& prog) {
    return top_usage(prog) +
           "\n"
           "A script to extract and build shadowboots\n"
           "\n"
           "positional arguments:\n"
           "  {build,extract,info,split,test}\n"
           "\n"
           "options:\n"
           "  -h, --help            show this help message and exit\n";
}

[[noreturn]] void top_error(const py::str& prog, const std::string& message) {
    std::cerr << top_usage(prog) << prog.utf8() << ": error: " << message << "\n";
    std::cerr.flush();
    throw py::SystemExit(2);
}

// split's usage and help as the Python prints them (argparse_min shows a `choices` option as REVISION / TYPE)
std::string split_usage(const py::str& prog) {
    const std::string indent(static_cast<size_t>(7 + prog.utf8().size() + 7), ' ');
    return "usage: " + prog.utf8() + " split [-h] [-k KERNEL]\n" + indent + "[-r {xenon,zephyr,falcon,jasper,trinity,corona,winchester}]\n" + indent +
           "[-t {retail,testkit,devkit}] [--nochecks]\n" + indent + "input output\n";
}

std::string split_help(const py::str& prog) {
    return split_usage(prog) +
           "\n"
           "positional arguments:\n"
           "  input                 The input path\n"
           "  output                The output path\n"
           "\n"
           "options:\n"
           "  -h, --help            show this help message and exit\n"
           "  -k, --kernel KERNEL   The kernel version\n"
           "  -r, --revision {xenon,zephyr,falcon,jasper,trinity,corona,winchester}\n"
           "                        The console revision\n"
           "  -t, --type {retail,testkit,devkit}\n"
           "                        The console type\n"
           "  --nochecks            Perform shadowboot parsing without integrity checks\n";
}

void write_out(const std::string& text) {
    std::fwrite(text.data(), 1, text.size(), stdout);
    std::fflush(stdout);
}

// int(value) for `type=int`
ArgumentParser::Type int_type(const char* name) {
    return [name](const py::str& v) -> py::str {
        try {
            return py::str(std::to_string(py::int_parse(v, 10)));
        } catch (const py::ValueError&) {
            throw ArgumentError(py::str(name), py::str("invalid int value: ") + py::repr(v));
        }
    };
}

// `choices=[...]`
ArgumentParser::Type choice_type(const char* name, const std::vector<std::string>& choices) {
    return [name, choices](const py::str& v) -> py::str {
        for (const std::string& c : choices)
            if (v == py::str(c)) return v;
        std::string list;
        for (size_t i = 0; i < choices.size(); ++i) list += (i ? ", " : "") + choices[i];
        throw ArgumentError(py::str(name), py::str("invalid choice: ") + py::repr(v) + py::str(" (choose from " + list + ")"));
    };
}

// the sub-parser of a command and what it makes of the rest of the command line
Args parse_command(const py::str& prog, const py::str& cmd, const py::list<py::str>& rest, const std::string& top_extras) {
    std::ostringstream out, err;
    const py::str sub_prog = prog + py::str(" ") + cmd;
    ArgumentParser parser(sub_prog, py::str(""), out, err);
    ArgumentParser::Type path = [](const py::str& v) { return path_type(v); };
    const std::string c = cmd.utf8();
    if (c == "build") {
        // build_parser.add_argument("input", type=str, help="The input path")  (commented out in the Python)
        parser.add_positional("output", py::str("The output path"), path);
        parser.add_flag({py::str("--nochecks")}, py::str("Perform shadowboot parsing without integrity checks"));
        parser.add_option({py::str("-m"), py::str("--manifest")}, py::str("The build manifest file"), std::nullopt, path);
        parser.add_option({py::str("-b"), py::str("--build-dir")}, py::str("The build directory path"), std::nullopt, path);
    } else if (c == "extract") {
        parser.add_positional("input", py::str("The input path"), path);
        parser.add_positional("output", py::str("The output path"), path);
        parser.add_flag({py::str("--flash")}, py::str("Parse a flash image instead of a shadowboot"));
        parser.add_flag({py::str("--nochecks")}, py::str("Perform shadowboot parsing without integrity checks"));
        parser.add_flag({py::str("--all")}, py::str("Extract all sections"));
        parser.add_flag({py::str("--smc")}, py::str("Extract the SMC"));
        parser.add_flag({py::str("--keyvault"), py::str("--kv")}, py::str("Extract the keyvault"));
        parser.add_flag({py::str("--sb")}, py::str("Extract the SB"));
        parser.add_flag({py::str("--sc")}, py::str("Extract the SC"));
        parser.add_flag({py::str("--sd")}, py::str("Extract the SD"));
        parser.add_flag({py::str("--se")}, py::str("Extract the SE"));
        parser.add_flag({py::str("--kernel")}, py::str("Extract the kernel"));
        parser.add_flag({py::str("--hypervisor"), py::str("--hv")}, py::str("Extract the hypervisor"));
        parser.add_flag({py::str("--loader")}, py::str("Extract HV/kernel patch loader"));
        parser.add_flag({py::str("--patches")}, py::str("Extract HV/kernel patches"));
    } else if (c == "info") {
        parser.add_positional("input", py::str("The input path"), path);
        parser.add_flag({py::str("--flash")}, py::str("Parse a flash image instead of a shadowboot"));
        parser.add_flag({py::str("--nochecks")}, py::str("Perform shadowboot parsing without integrity checks"));
    } else if (c == "split") {
        parser.add_positional("input", py::str("The input path"), path);
        parser.add_positional("output", py::str("The output path"), path);
        parser.add_option({py::str("-k"), py::str("--kernel")}, py::str("The kernel version"), std::nullopt, int_type("-k/--kernel"));
        parser.add_option({py::str("-r"), py::str("--revision")}, py::str("The console revision"), std::nullopt,
                          choice_type("-r/--revision", {"xenon", "zephyr", "falcon", "jasper", "trinity", "corona", "winchester"}));
        parser.add_option({py::str("-t"), py::str("--type")}, py::str("The console type"), std::nullopt,
                          choice_type("-t/--type", {"retail", "testkit", "devkit"}));
        parser.add_flag({py::str("--nochecks")}, py::str("Perform shadowboot parsing without integrity checks"));
    } else {   // test
        parser.add_positional("input", py::str("The input path"), path);
        parser.add_positional("output", py::str("The output path"), path);
        parser.add_flag({py::str("--nochecks")}, py::str("Perform shadowboot parsing without integrity checks"));
    }

    Namespace ns;
    try {
        ns = parser.parse_args(rest);
    } catch (const py::SystemExit& e) {
        if (e.exit_status() == 0) {   // -h
            write_out(c == "split" ? split_help(prog) : out.str());
        } else {
            const std::string text = err.str();
            const std::string marker = sub_prog.utf8() + ": error: ";
            const size_t at = text.rfind(marker);
            std::string message = at == std::string::npos ? text : text.substr(at + marker.size());
            while (!message.empty() && (message.back() == '\n' || message.back() == '\r')) message.pop_back();
            static const std::string unrecognized = "unrecognized arguments: ";
            if (message.compare(0, unrecognized.size(), unrecognized) == 0) {
                // the sub-parser's leftovers are reported by the top-level parser, after its own
                std::string all = top_extras.empty() ? message.substr(unrecognized.size()) : top_extras + " " + message.substr(unrecognized.size());
                top_error(prog, unrecognized + all);
            }
            std::cerr << (c == "split" ? split_usage(prog) : text.substr(0, at)) << marker << message << "\n";
            std::cerr.flush();
        }
        throw;
    }
    // (a clean parse of a sub-parser can still leave extras for the top-level parser: none, argparse_min reports them)
    Args a;
    a.command = cmd;
    a.input = ns.get("input");
    a.output = ns.get("output");
    a.manifest = ns.get("manifest");
    a.build_dir = ns.get("build_dir");
    a.revision = ns.get("revision");
    a.type = ns.get("type");
    if (c == "split") {
        std::optional<py::str> k = ns.get("kernel");
        if (k) a.kernel_version = py::int_parse(*k, 10);
    }
    a.nochecks = ns.flag("nochecks");
    a.flash = ns.flag("flash");
    a.all = ns.flag("all");
    a.smc = ns.flag("smc");
    a.keyvault = ns.flag("keyvault");
    a.sb = ns.flag("sb");
    a.sc = ns.flag("sc");
    a.sd = ns.flag("sd");
    a.se = ns.flag("se");
    if (c == "extract") a.kernel = ns.flag("kernel");
    a.hypervisor = ns.flag("hypervisor");
    a.loader = ns.flag("loader");
    a.patches = ns.flag("patches");
    return a;
}

// parser.parse_args(): the top-level parser, which has -h and one sub-command (the first argument that is not an option)
Args parse_args(const py::list<py::str>& argv, const py::str& prog) {
    std::string extras;   // unrecognized options before the command
    auto add_extra = [&](const py::str& a) { extras += (extras.empty() ? "" : " ") + a.utf8(); };
    int64_t i = 0;
    const int64_t n = py::len(argv);
    while (i < n) {
        const py::str a = argv[i];
        const std::string s = a.utf8();
        if (s == "--") {
            ++i;
            if (i >= n) add_extra(a);   // a lone "--" is left over (there is no command for it to introduce)
            break;
        }
        if (s.size() > 1 && s[0] == '-') {
            // -h / --help, or an abbreviation of --help (--help=value: the flag takes no argument)
            const size_t eq = s.find('=');
            const std::string opt = s[1] == '-' ? s.substr(0, eq) : s;
            if (opt == "-h" || (opt.size() > 2 && opt[1] == '-' && std::string("--help").compare(0, opt.size(), opt) == 0 && opt.size() <= 6)) {
                if (eq != std::string::npos && s[1] == '-') {
                    top_error(prog, "argument -h/--help: ignored explicit argument " + py::repr(py::str(s.substr(eq + 1))).utf8());
                }
                write_out(top_help(prog));
                throw py::SystemExit(0);
            }
            // a negative number, or an argument with a space, is not an option
            const bool negative_number = py::re::match(py::str("-\\.?\\d"), a).has_value();
            if (!negative_number && s.find(' ') == std::string::npos) {
                add_extra(a);
                ++i;
                continue;
            }
        }
        break;
    }
    if (i >= n) {
        if (!extras.empty()) top_error(prog, "unrecognized arguments: " + extras);
        return Args();   // no command: args.command is None
    }
    const py::str cmd = argv[i];
    bool known = false;
    for (const char* c : kCommands) known = known || cmd == py::str(c);
    if (!known) top_error(prog, "argument command: invalid choice: " + py::repr(cmd).utf8() + " (choose from build, extract, info, split, test)");
    Args a = parse_command(prog, cmd, argv.slice(i + 1, n), extras);
    if (!extras.empty()) top_error(prog, "unrecognized arguments: " + extras);
    return a;
}

// d[key] of a JSON value: KeyError for a missing key, TypeError when the value is no dict
const py::V& getitem(const py::V& obj, const char* key) {
    if (obj.is_dict()) {
        if (!obj.has(key)) throw py::KeyError(std::string("'") + key + "'");
        return obj.at(key);
    }
    if (obj.is_str()) throw py::TypeError("string indices must be integers, not 'str'");
    if (obj.is_list()) throw py::TypeError("list indices must be integers or slices, not str");
    if (obj.is_none()) none_not_subscriptable();
    throw py::TypeError("object is not subscriptable");
}

// del d[key]
void delitem(const py::V& obj, const char* key) {
    if (obj.is_dict()) {
        if (!obj.has(key)) throw py::KeyError(std::string("'") + key + "'");
        obj.erase(py::V(key));
        return;
    }
    if (obj.is_none()) throw py::TypeError("'NoneType' object does not support item deletion");
    if (obj.is_str()) throw py::TypeError("'str' object doesn't support item deletion");
    if (obj.is_list()) throw py::TypeError("list indices must be integers or slices, not str");
    throw py::TypeError("object doesn't support item deletion");
}

// Path(value) of a JSON value (TypeError unless it is a string)
py::str as_path(const py::V& v) {
    if (!v.is_str()) throw py::TypeError("argument should be a str or an os.PathLike object where __fspath__ returns a str");
    return path_of(py::str(v.str()));
}
// bd / value
py::str join_json(const py::str& bd, const py::V& v) {
    if (!v.is_str()) throw py::TypeError("unsupported operand type(s) for /");
    return path_join(bd, py::str(v.str()));
}

// json.loads(text)
py::V loads(const py::str& text) {
    try {
        return py::json_parse(text.utf8());
    } catch (const py::PyError& e) {
        if (e.type == "ValueError") throw JSONDecodeError(e.what());
        throw;
    }
}

// try_read_sources(path, data): the first that gives something (data is None when absent)
std::optional<py::bytes> read_sources(const py::str& path, const std::optional<py::bytes>& data) {
    py::list<Source> sources;
    sources.append(Source(path));
    if (data) sources.append(Source(*data));
    auto r = try_read_sources(sources);
    if (!r) return std::nullopt;
    if (std::holds_alternative<py::bytes>(*r)) return std::get<py::bytes>(*r);
    return py::bytes(std::get<py::bytearray>(*r).view());
}

[[noreturn]] void unbound_manifest() {
    throw UnboundLocalError("cannot access local variable 'build_manifest' where it is not associated with a value");
}

}  // namespace

int64_t main(const py::list<py::str>& argv, const py::str& prog) {
    ONE_BL_KEY = keystore::load_and_verify_1bl_pub();
    SB_PRV_KEY = keystore::load_and_verify_sb_prv();

    Args args = parse_args(argv, prog);

    if (args.command && *args.command == py::str("build")) {
        if (!args.manifest) none_attribute("is_file");
        // build_manifest is only bound when the manifest file is read
        std::optional<py::V> build_manifest;
        py::str smc_bin_file, kv_file, sb_file, sc_file, sd_file, se_file, kernel_file, hypervisor_file, base_img_file, hvk_patches_file;
        if (path_is_file(*args.manifest)) {   // building with a manifest file
            // load the manifest file
            say("Loading build manifest...");
            build_manifest = loads(read_text(*args.manifest));
            const py::V& bm = *build_manifest;

            // remove comments
            delitem(bm, "_comment");
            delitem(getitem(bm, "build"), "_comment");
            delitem(getitem(bm, "options"), "_comment");
            delitem(getitem(bm, "files"), "_comment");

            // paths
            say("Setting up paths...");
            auto F = [&](const char* name) -> const py::V& { return getitem(getitem(bm, "files"), name); };
            py::str bd = as_path(F("base_directory"));
            base_img_file = as_path(F("base_image"));
            smc_bin_file = join_json(bd, F("SMC"));
            // smc_cfg_file = bd / build_manifest["files"]["SMC_config"]
            kv_file = join_json(bd, F("KV"));
            sb_file = join_json(bd, F("SB"));
            sc_file = join_json(bd, F("SC"));
            sd_file = join_json(bd, F("SD"));
            se_file = join_json(bd, F("SE"));
            kernel_file = join_json(bd, F("kernel"));
            hypervisor_file = join_json(bd, F("HV"));
            // sd_patches_file = bd / build_manifest["files"]["SD_patches"]
            // sd_code_file = bd / build_manifest["files"]["SD_code"]
            hvk_patches_file = join_json(bd, F("HVK_patches"));
        } else if (args.build_dir) {   // using a build directory vs a manifest
            const py::str& b = *args.build_dir;
            smc_bin_file = path_join(b, py::str("SMC_dec.bin"));
            // smc_cfg_file = args.build_dir / "SMC_config.bin"
            kv_file = path_join(b, py::str("KV_dec.bin"));
            sb_file = path_join(b, py::str("sb.bin"));
            sc_file = path_join(b, py::str("sc.bin"));
            sd_file = path_join(b, py::str("sd.bin"));
            se_file = path_join(b, py::str("se.bin"));
            kernel_file = path_join(b, py::str("kernel.bin"));
            hypervisor_file = path_join(b, py::str("hypervisor.bin"));
            base_img_file = path_join(b, py::str("xboxrom.bin"));

            // patches
            // sd_code_file = args.build_dir / "sdc.bin"  # SD code file
            // sd_patches_file = args.build_dir / "sdp.bin"  # SD patches file
            hvk_patches_file = path_join(b, py::str("hvk.bin"));
        } else {
            say("Building requires -m or -b arguments!");
            return 1;
        }
        auto manifest = [&]() -> const py::V& {
            if (!build_manifest) unbound_manifest();
            return *build_manifest;
        };

        // check for the base image and load it if it exists
        std::optional<ShadowbootImage> base_img;
        const bool base_img_available = path_is_file(base_img_file);
        if (base_img_available) {
            py::bytes base_data = read_bytes(base_img_file);
            base_img = ShadowbootImage::parse(base_data, !py::truthy(getitem(getitem(manifest(), "options"), "base_image_checks_disabled")));
        }

        py::bytes se_data;
        if (path_is_file(kernel_file) && path_is_file(hypervisor_file)) {
            say("Reading raw HV/kernel...");
            py::bytes kernel = read_bytes(kernel_file);
            py::bytes hypervisor = read_bytes(hypervisor_file);
            se_data = hypervisor + kernel;
        } else if (path_is_file(se_file)) {
            say("Decompressing SE...");
            se_data = decompress_se(read_bytes(se_file));
        } else if (base_img_available) {
            say("Reading HV/kernel from base image...");
            se_data = need_bytes(base_img->hypervisor_data) + need_bytes(base_img->kernel_data);
        } else {
            throw py::Exception("No HV/kernel pair, SE, or fallback image was provided!");
        }

        // the arguments of create, evaluated in order (base_img is None when there was no base image)
        auto base = [&](const char* attr) -> const ShadowbootImage& {
            if (!base_img) none_attribute(attr);
            return *base_img;
        };
        std::optional<py::bytes> sb_arg = read_sources(sb_file, base("sb_data").sb_data);
        std::optional<py::bytes> sc_arg = read_sources(sc_file, base("sc_data").sc_data);
        std::optional<py::bytes> sd_arg = read_sources(sd_file, base("sd_data").sd_data);
        std::optional<py::bytes> smc_arg;
        if (py::truthy(getitem(getitem(manifest(), "options"), "use_smc"))) smc_arg = read_sources(smc_bin_file, base("smc_data").smc_data);
        std::optional<std::tuple<py::bytes, std::optional<py::bytes>>> kv_arg;
        if (py::truthy(getitem(getitem(manifest(), "options"), "use_kv")))
            kv_arg = std::make_tuple(py::bytes::zeros(0x10), read_sources(kv_file, base("kv_data").kv_data));
        std::optional<py::bytes> patches_arg = read_sources(hvk_patches_file, std::nullopt);
        const bool test_kit = py::truthy(getitem(getitem(manifest(), "options"), "test_kit"));
        const py::V& version = getitem(getitem(manifest(), "build"), "version");
        // (an int, or a bool, is a number to ctypes and struct; any other JSON value is not)
        std::optional<int64_t> build_version;
        if (version.is_intlike()) build_version = version.as_int();

        py::bytes data = ShadowbootImage::create(sb_arg, sc_arg, sd_arg, std::optional<py::bytes>(se_data), smc_arg, kv_arg, patches_arg, test_kit,
                                                 build_version);
        write_bytes(*args.output, data);

        say("Final image location: \"" + path_absolute(*args.output).utf8() + "\"");
    } else if (args.command && *args.command == py::str("extract")) {
        ShadowbootImage img = args.flash ? ShadowbootImage::parse(unecc(*args.input), !args.nochecks)
                                         : ShadowbootImage::parse(read_bytes(*args.input), !args.nochecks);

        img.print_info();

        const py::str& out = *args.output;
        if (args.all || args.smc) {
            if (img.smc_data && py::len(*img.smc_data) > 0) write_bytes(path_join(out, py::str("SMC_dec.bin")), *img.smc_data);
        }
        // if args.all or args.smc_config:
        //     (args.output / "smc_config.bin").write_bytes(img.smc_config_data)
        if (args.all || args.keyvault) {
            if (img.kv_data && py::len(*img.kv_data) > 0) write_bytes(path_join(out, py::str("KV_enc.bin")), *img.kv_data);
        }
        if (args.all || args.sb) write_bytes(path_join(out, py::str("sb_" + none_or(img.sb_build).utf8() + ".bin")), need_bytes(img.sb_data));
        if (args.all || args.sc) write_bytes(path_join(out, py::str("sc_" + none_or(img.sc_build).utf8() + ".bin")), need_bytes(img.sc_data));
        if (args.all || args.sd) write_bytes(path_join(out, py::str("sd_" + none_or(img.sd_build).utf8() + ".bin")), need_bytes(img.sd_data));
        if (args.all || args.se) write_bytes(path_join(out, py::str("se_" + none_or(img.se_build).utf8() + ".bin")), need_bytes(img.se_data));
        if (args.all || args.kernel) write_bytes(path_join(out, py::str("kernel.exe")), need_bytes(img.kernel_data));
        if (args.all || args.hypervisor) write_bytes(path_join(out, py::str("hypervisor.bin")), need_bytes(img.hypervisor_data));
        if (args.all || args.loader) {
            if (py::len(img.patches) > 0) {
                write_bytes(path_join(out, py::str("patch_loader.bin")), need_bytes(img.patches[0].patch_loader));
            } else {
                say("No patch loader found!");
            }
        }
        if (args.all || args.patches) {
            if (py::len(img.patches) > 1) {
                if (!path_is_dir(path_join(out, py::str("Patches")))) path_mkdir(path_join(out, py::str("Patches")));

                for (int64_t k = 1; k < py::len(img.patches); ++k) {
                    const Patch& patch = img.patches[k];
                    int64_t addr = *patch.address;
                    if (addr > HYPERVISOR_SIZE) addr += 0x80000000LL;
                    write_bytes(path_join(path_join(out, py::str("Patches")), py::format(addr, "08X") + py::str(".bin")), need_bytes(patch.patch_code));
                }

                py::bytes combined;
                for (int64_t k = 1; k < py::len(img.patches); ++k) {
                    const Patch& patch = img.patches[k];
                    combined = combined + py::pack(">2I", *patch.address, *patch.size_int32);
                    combined = combined + need_bytes(patch.patch_code);
                }
                combined = combined + py::bytes(std::string(4, '\xFF'));

                write_bytes(path_join(out, py::str("patches_raw.bin")), combined);
            } else {
                say("No patches found!");
            }
        }
    } else if (args.command && *args.command == py::str("info")) {
        ShadowbootImage img = args.flash ? ShadowbootImage::parse(unecc(*args.input), !args.nochecks)
                                         : ShadowbootImage::parse(read_bytes(*args.input), !args.nochecks);
        img.print_info();
    } else if (args.command && *args.command == py::str("split")) {
        // cabextract -p --filter "KERNEL/*" XDK_0.cab > images.bin

        py::bytes image_data = read_bytes(*args.input);
        // IMAGE_EXP = re.compile(rb"Microsoft Corporation\. All rights reserved")
        static const py::bytes image_exp("Microsoft Corporation. All rights reserved");
        std::vector<int64_t> idxs;
        for (int64_t at = image_data.find(image_exp); at >= 0; at = image_data.find(image_exp, at + image_exp.size())) idxs.push_back(at - 28);
        for (size_t i = 0; i < idxs.size(); ++i) {
            py::bytes data;
            if (idxs[i] == idxs.back()) {   // last entry
                data = image_data.slice(idxs[i], image_data.size());
            } else {   // other entries
                data = image_data.slice(idxs[i], idxs[i + 1]);
            }
            ShadowbootImage img = ShadowbootImage::parse(data, !args.nochecks, false);

            // img.kernel_version == args.kernel == img.hypervisor_version
            auto versions_match = [&]() {
                return img.kernel_version == args.kernel_version && args.kernel_version == img.hypervisor_version;
            };
            // img.console_type.lower() == args.revision.lower()
            auto revision_matches = [&]() {
                if (!img.console_type) none_attribute("lower");
                if (!args.revision) none_attribute("lower");
                return img.console_type->lower() == args.revision->lower();
            };
            const bool type_retail = args.type && *args.type == py::str("retail");
            const bool type_testkit = args.type && *args.type == py::str("testkit");
            const bool type_devkit = args.type && *args.type == py::str("devkit");

            if (img.is_retail && type_retail) {
                if (versions_match()) {
                    if (revision_matches()) {
                        write_bytes(path_join(*args.output, py::str("xboxromw2d.bin")), data);
                        say("Found!");
                        break;
                    }
                }
            } else if (img.is_testkit && type_testkit) {
                if (versions_match()) {
                    if (revision_matches()) {
                        write_bytes(path_join(*args.output, py::str("xboxromtw2d.bin")), data);
                        say("Found!");
                        break;
                    }
                }
            } else if (!img.is_testkit && !img.is_retail && type_devkit) {
                if (versions_match()) {
                    if (revision_matches()) {
                        write_bytes(path_join(*args.output, py::str("xboxromw2d.bin")), data);
                        say("Found!");
                        break;
                    }
                }
            }
        }
    } else if (args.command && *args.command == py::str("test")) {
        // pass
    }

    return 0;
}

}  // namespace codebreaker::formats::xbox::crypto::shadowboot
