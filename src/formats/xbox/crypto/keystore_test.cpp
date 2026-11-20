// Tests of cb/formats/xbox/crypto/keystore.hpp: the file names and checksums, loading a key
// whose checksum is accepted, and refusing a file that is not the genuine one. The genuine keys are not in the repository, so the
// tests make keys of their own and accept their checksums.
//
// Behaviour pinned here: the key folder is the store's own (the settings are read by the caller that makes the
// store), and a wrong checksum is a FormatError that names the file and both checksums.
#include "formats/xbox/crypto/keystore.hpp"

#include "core/crypto.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"

namespace keystore_test {

using namespace cb;
using namespace cb::formats::xbox::crypto;
using keystore::KeyFile;

static Bytes copy_of(ByteView v) { return Bytes(v.begin(), v.end()); }

CB_TEST("formats.xbox.crypto.keystore.the_files_and_their_checksums", [] {
    CB_CHECK(keystore::file_name(KeyFile::Bl1Pub) == "1BL_pub.bin");
    CB_CHECK(keystore::file_name(KeyFile::XmacsPub) == "XMACS_pub.bin");
    CB_CHECK(keystore::file_name(KeyFile::SbPrv) == "SB_prv.bin");
    CB_CHECK(keystore::file_name(KeyFile::HvxPrv) == "HVX_prv.bin");
    CB_CHECK(keystore::file_name(KeyFile::MasterPub) == "Master_pub.bin");
    CB_CHECK(keystore::checksum(KeyFile::Bl1Pub) == 0xD416B5E1u);
    CB_CHECK(keystore::checksum(KeyFile::XmacsPub) == 0xE4F01473u);
    CB_CHECK(keystore::checksum(KeyFile::MasterPub) == 0xE86E10FDu);
    CB_CHECK(keystore::checksum(KeyFile::SbPrv) == 0x490C9D35u);
    CB_CHECK(keystore::checksum(KeyFile::HvxPrv) == 0xDCC4B906u);
});

CB_TEST("formats.xbox.crypto.keystore.a_key_whose_checksum_is_accepted_loads", [] {
    fs::ScopedTempDir dir("cb-keystore");
    const xecrypt::RsaKey made = xecrypt::RsaKey::generate(1024);
    const Bytes bytes = copy_of(made.bytes());
    fs::write_file(dir.path() / "SB_prv.bin", bytes);
    keystore::KeyStore store(dir.path());
    CB_CHECK(store.folder() == dir.path());
    store.accept_checksum(KeyFile::SbPrv, cb::crypto::crc32(bytes));
    const xecrypt::RsaKey loaded = store.load_and_verify(KeyFile::SbPrv);
    CB_CHECK(loaded.is_private());
    CB_CHECK(copy_of(loaded.bytes()) == bytes);
    const xecrypt::RsaKey pub = made.public_key();
    const Bytes pub_bytes = copy_of(pub.bytes());
    fs::write_file(dir.path() / "1BL_pub.bin", pub_bytes);
    store.accept_checksum(KeyFile::Bl1Pub, cb::crypto::crc32(pub_bytes));
    CB_CHECK(!store.load_and_verify(KeyFile::Bl1Pub).is_private());
});

CB_TEST("formats.xbox.crypto.keystore.a_file_that_is_not_the_genuine_key_is_refused", [] {
    fs::ScopedTempDir dir("cb-keystore");
    const xecrypt::RsaKey made = xecrypt::RsaKey::generate(1024);
    fs::write_file(dir.path() / "SB_prv.bin", copy_of(made.bytes()));
    keystore::KeyStore store(dir.path());
    bool refused = false;
    try {
        (void)store.load_and_verify(KeyFile::SbPrv);   // its checksum is not the genuine SB key's
    } catch (const FormatError& e) {
        refused = true;
        const std::string what = e.what();
        CB_CHECK(what.find("invalid key checksum") != std::string::npos);
        CB_CHECK(what.find("490C9D35") != std::string::npos);
        CB_CHECK(what.find("SB_prv.bin") != std::string::npos);
    }
    CB_CHECK(refused);
    // a file whose checksum is accepted but which is no key
    const Bytes junk{1, 2, 3};
    fs::write_file(dir.path() / "HVX_prv.bin", junk);
    store.accept_checksum(KeyFile::HvxPrv, cb::crypto::crc32(junk));
    bool not_a_key = false;
    try {
        (void)store.load_and_verify(KeyFile::HvxPrv);
    } catch (const FormatError&) {
        not_a_key = true;
    }
    CB_CHECK(not_a_key);
});

CB_TEST("formats.xbox.crypto.keystore.a_missing_file_is_not_found", [] {
    fs::ScopedTempDir dir("cb-keystore");
    bool missing = false;
    try {
        (void)keystore::KeyStore(dir.path()).load_and_verify(KeyFile::MasterPub);
    } catch (const NotFoundError&) {
        missing = true;
    }
    CB_CHECK(missing);
});

}  // namespace keystore_test
