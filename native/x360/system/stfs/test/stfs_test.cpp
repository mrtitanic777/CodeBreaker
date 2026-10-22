// Tests of the STFS library (runtime/stfs/test/build.bat).
//
//   stfs_test dump <package>              path, kind, size, SHA-1 of every entry (compare.py diffs it with
//                                         tools/scripts/stfs_extract.py)
//   stfs_test info <package>              metadata and the verification report
//   stfs_test selftest <work dir> [<package>...]
//                                         create / reopen / edit / convert / sign packages in <work dir>; the given
//                                         real packages must open and verify (read only, never copied)

#include "stfs/builder.h"
#include "stfs/package.h"
#include "stfs/sign.h"

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <bcrypt.h>

#include <cstdio>
#include <cstring>
#include <filesystem>
#include <format>
#include <fstream>
#include <random>
#include <string>
#include <vector>

namespace
{
    int g_failures = 0;
    int g_checks = 0;

    void Check(bool ok, const std::string& what)
    {
        ++g_checks;
        if (!ok)
        {
            ++g_failures;
            std::printf("FAIL: %s\n", what.c_str());
        }
    }

    std::string Hex(const uint8_t* p, size_t n)
    {
        std::string s;
        for (size_t i = 0; i < n; ++i)
            s += std::format("{:02x}", p[i]);
        return s;
    }

    std::string Narrow(const std::u16string& s)
    {
        std::string out;
        for (char16_t c : s)
            out.push_back(c < 0x80 ? char(c) : '?');
        return out;
    }

    std::vector<uint8_t> Pattern(size_t size, uint32_t seed)
    {
        std::vector<uint8_t> v(size);
        std::mt19937 rng(seed);
        for (auto& b : v)
            b = uint8_t(rng());
        return v;
    }

    int Dump(const char* path)
    {
        std::string error;
        auto package = stfs::Package::Open(path, &error);
        if (!package)
        {
            std::printf("error: %s\n", error.c_str());
            return 1;
        }
        std::vector<uint8_t> data;
        for (const stfs::Entry& e : package->Entries())
        {
            if (e.IsDirectory())
            {
                std::printf("%s\tdir\t0\t-\n", e.path.c_str());
                continue;
            }
            if (!package->ReadAll(e, data))
            {
                std::printf("%s\tfile\t%u\tREAD-ERROR\n", e.path.c_str(), e.Size());
                continue;
            }
            const stfs::Sha1Digest h = stfs::Sha1(data);
            std::printf("%s\tfile\t%u\t%s\n", e.path.c_str(), e.Size(), Hex(h.data(), 20).c_str());
        }
        return 0;
    }

    void PrintReport(const stfs::VerifyReport& r)
    {
        std::printf("  header hash %s, top hash %s, data blocks %u checked / %u bad, tables %u checked / %u bad, "
                    "signature %s\n",
                    r.headerHashOk ? "ok" : "BAD", r.topHashOk ? "ok" : "BAD", r.dataBlocksChecked, r.dataBlocksBad,
                    r.tablesChecked, r.tablesBad, stfs::SignatureStatusName(r.signature));
        for (const std::string& p : r.problems)
            std::printf("    %s\n", p.c_str());
    }

    int Info(const char* path)
    {
        std::string error;
        auto package = stfs::Package::Open(path, &error);
        if (!package)
        {
            std::printf("error: %s\n", error.c_str());
            return 1;
        }
        const stfs::Metadata& m = package->GetMetadata();
        std::printf("%s: %s, header %#x, content type %#x, metadata v%u, title %08X, content size %#llx\n", path,
                    stfs::MagicName(m.magic), m.headerSize, m.contentType, m.metadataVersion, m.titleId,
                    (unsigned long long)m.contentSize);
        std::printf("  console %s, profile %016llX, device %s, transfer flags %#x\n", Hex(m.consoleId.data(), 5).c_str(),
                    (unsigned long long)m.profileId, Hex(m.deviceId.data(), 20).c_str(), m.transferFlags);
        std::printf("  volume: flags %#x (%s), %u blocks (%u free), file table %u blocks at %u\n", m.volume.flags,
                    m.volume.ReadOnlyFormat() ? "read-only" : "read-write", m.volume.totalBlocks, m.volume.freeBlocks,
                    m.volume.fileTableBlocks, m.volume.fileTableBlock);
        std::printf("  name \"%s\", title \"%s\", publisher \"%s\", %zu entries, thumbnail %zu bytes\n",
                    Narrow(m.displayNames[0]).c_str(), Narrow(m.titleName).c_str(), Narrow(m.publisher).c_str(),
                    package->Entries().size(), m.thumbnail.size());
        PrintReport(package->Verify());
        return 0;
    }

    // ---- self test ----

    struct TestFile
    {
        std::string path;
        std::vector<uint8_t> data;
    };

    bool ContentsMatch(const stfs::Package& package, const std::vector<TestFile>& files, const std::string& label)
    {
        bool ok = true;
        std::vector<uint8_t> data;
        for (const TestFile& f : files)
        {
            const stfs::Entry* e = package.Find(f.path);
            if (!e || e->IsDirectory())
            {
                Check(false, std::format("{}: {} missing", label, f.path));
                ok = false;
                continue;
            }
            const bool same = package.ReadAll(*e, data) && data == f.data;
            Check(same, std::format("{}: {} contents ({} bytes)", label, f.path, f.data.size()));
            ok &= same;
        }
        return ok;
    }

    void CheckVerified(const stfs::Package& package, const std::string& label, stfs::SignatureStatus signature)
    {
        const stfs::VerifyReport r = package.Verify();
        Check(r.HashesOk(), label + ": hash chain verifies");
        Check(r.signature == signature, std::format("{}: signature {} (expected {})", label,
                                                    stfs::SignatureStatusName(r.signature),
                                                    stfs::SignatureStatusName(signature)));
        if (!r.HashesOk())
            PrintReport(r);
    }

    stfs::Metadata TestMetadata()
    {
        stfs::Metadata m;
        m.contentType = stfs::content_type::kSavedGame;
        m.titleId = 0x584111F7;
        m.profileId = 0xE000012345678901ull;
        m.consoleId = {1, 2, 3, 4, 5};
        m.displayNames[0] = u"Test save";
        m.descriptions[0] = u"Written by stfs_test";
        m.displayNames[10] = u"v2 name";
        m.titleName = u"STFS test";
        m.publisher = u"Shared";
        m.thumbnail = Pattern(300, 99);
        return m;
    }

    std::shared_ptr<stfs::Package> Reopen(const std::filesystem::path& path, const std::string& label)
    {
        std::string error;
        std::shared_ptr<stfs::Package> p = stfs::Package::Open(path, &error);
        Check(p != nullptr, label + ": reopens (" + error + ")");
        return p;
    }

    void TestCreate(const std::filesystem::path& work, bool readOnly)
    {
        const std::string label = readOnly ? "create (read-only format)" : "create (read-write format)";
        std::vector<TestFile> files = {
            {"empty.bin", {}},
            {"one.bin", Pattern(1, 1)},
            {"block.bin", Pattern(4096, 2)},
            {"block_plus.bin", Pattern(4097, 3)},
            {"Data/Nested/deep.dat", Pattern(10000, 4)},
            {"Data/big.dat", Pattern(173 * 4096 + 5, 5)}, // crosses a level-0 table: level 1 present
        };
        for (int i = 0; i < 70; ++i) // more than 64 entries: two file table blocks
            files.push_back({std::format("Many/f{:02}.txt", i), Pattern(i * 37, 100 + i)});

        stfs::Builder builder(TestMetadata());
        builder.AddDirectory("EmptyDir");
        for (const TestFile& f : files)
        {
            std::string error;
            Check(builder.AddFile(f.path, f.data, &error), label + ": add " + f.path + " " + error);
        }
        const std::filesystem::path out = work / (readOnly ? "created_ro.bin" : "created_rw.bin");
        stfs::Builder::SaveOptions options;
        options.readOnlyFormat = readOnly;
        std::string error;
        Check(builder.Save(out, options, &error), label + ": save " + error);

        auto package = Reopen(out, label);
        if (!package)
            return;
        const stfs::Metadata& m = package->GetMetadata();
        Check(m.magic == stfs::Magic::Con, label + ": magic CON");
        Check(m.volume.ReadOnlyFormat() == readOnly, label + ": format flag");
        Check(m.titleId == 0x584111F7 && m.profileId == 0xE000012345678901ull, label + ": title / profile");
        Check(m.displayNames[0] == u"Test save" && m.displayNames[10] == u"v2 name", label + ": display names");
        Check(m.descriptions[0] == u"Written by stfs_test" && m.titleName == u"STFS test", label + ": strings");
        Check(m.thumbnail == Pattern(300, 99), label + ": thumbnail");
        Check(m.contentSize + package->GetLayout().base == package->FileSize(), label + ": content size = file size");
        Check(package->Entries().size() == files.size() + 4, label + ": entry count"); // + EmptyDir, Data, Nested, Many
        Check(package->Find("emptydir") && package->Find("emptydir")->IsDirectory(), label + ": empty directory");
        Check(package->List("Many").size() == 70 && package->List("").size() == 7, label + ": List");
        ContentsMatch(*package, files, label);
        CheckVerified(*package, label, stfs::SignatureStatus::Unsigned);

        // Ranged reads across block and table boundaries.
        const stfs::Entry* big = package->Find("data\\BIG.dat");
        Check(big != nullptr, label + ": case-insensitive find");
        if (big)
        {
            const std::vector<uint8_t> expect = Pattern(173 * 4096 + 5, 5);
            const uint64_t starts[] = {0, 4095, 169 * 4096 + 100, 172 * 4096, expect.size() - 3};
            for (uint64_t start : starts)
            {
                std::vector<uint8_t> got(9000);
                const int64_t n = package->Read(*big, start, got.data(), got.size());
                const size_t want = size_t(std::min<uint64_t>(got.size(), expect.size() - start));
                Check(n == int64_t(want) && std::memcmp(got.data(), expect.data() + start, want) == 0,
                      std::format("{}: range read at {}", label, start));
            }
            std::vector<uint8_t> none(10);
            Check(package->Read(*big, expect.size(), none.data(), none.size()) == 0, label + ": read at end");
        }

        // Edit in place (the source package stays open while its file is replaced).
        auto edit = stfs::Builder::FromPackage(package);
        std::vector<TestFile> after;
        for (const TestFile& f : files)
            if (!f.path.starts_with("Many/") && f.path != "one.bin")
                after.push_back(f);
        after.push_back({"one.bin", Pattern(5000, 7)});    // replaced, now larger
        after.push_back({"Added/new.txt", Pattern(123, 8)}); // added
        Check(edit->AddFile("ONE.bin", Pattern(5000, 7)), label + ": replace");
        Check(edit->AddFile("Added/new.txt", Pattern(123, 8)), label + ": add");
        Check(edit->Remove("many"), label + ": remove directory");
        Check(!edit->Remove("does/not/exist"), label + ": remove missing");
        edit->GetMetadata().displayNames[0] = u"Edited";
        Check(edit->Save(out, &error), label + ": save in place " + error);
        auto edited = Reopen(out, label + " edited");
        if (!edited)
            return;
        Check(edited->GetMetadata().displayNames[0] == u"Edited", label + " edited: display name");
        Check(edited->Find("many") == nullptr && edited->Find("Many/f00.txt") == nullptr, label + " edited: removed");
        Check(edited->Entries().size() == after.size() + 4, label + " edited: entry count"); // EmptyDir Data Nested Added
        ContentsMatch(*edited, after, label + " edited");
        CheckVerified(*edited, label + " edited", stfs::SignatureStatus::Unsigned);
    }

    void TestLevel2(const std::filesystem::path& work)
    {
        // More than 28900 blocks: three hash levels.
        const std::string label = "level 2";
        const size_t size = size_t(28950) * 4096 + 77;
        std::vector<uint8_t> data(size);
        for (size_t i = 0; i < size; i += 4096)
            std::memcpy(&data[i], &i, std::min<size_t>(sizeof(i), size - i));
        stfs::Builder builder(TestMetadata());
        builder.AddFile("huge.bin", data);
        builder.AddFile("tail.bin", Pattern(9000, 9));
        const std::filesystem::path out = work / "level2.bin";
        for (bool readOnly : {true, false})
        {
            stfs::Builder::SaveOptions options;
            options.readOnlyFormat = readOnly;
            std::string error;
            const std::string l = label + (readOnly ? " read-only" : " read-write");
            Check(builder.Save(out, options, &error), l + ": save " + error);
            auto package = Reopen(out, l);
            if (!package)
                continue;
            Check(package->GetMetadata().volume.totalBlocks > stfs::kBlocksPerLevel[1], l + ": three levels");
            ContentsMatch(*package, {{"huge.bin", data}, {"tail.bin", Pattern(9000, 9)}}, l);
            CheckVerified(*package, l, stfs::SignatureStatus::Unsigned);
        }
        std::filesystem::remove(out);
    }

    void TestCorruption(const std::filesystem::path& work)
    {
        const std::string label = "corruption";
        stfs::Builder builder(TestMetadata());
        builder.AddFile("a.bin", Pattern(3 * 4096, 11));
        const std::filesystem::path out = work / "corrupt.bin";
        builder.Save(out);
        // Flip one byte of the second data block of a.bin (data block 2) and one metadata byte.
        auto package = Reopen(out, label);
        if (!package)
            return;
        const uint64_t at = package->GetLayout().Offset(package->GetLayout().DataBlockToBacking(2)) + 17;
        package.reset();
        {
            std::fstream f(out, std::ios::in | std::ios::out | std::ios::binary);
            f.seekp(std::streamoff(at));
            f.put(char(0x5A));
            f.seekp(stfs::offsets::kTitleName);
            f.put('X');
        }
        package = Reopen(out, label);
        if (!package)
            return;
        const stfs::VerifyReport r = package->Verify();
        Check(!r.headerHashOk, label + ": header hash mismatch reported");
        Check(r.dataBlocksBad == 1 && r.topHashOk && !r.tablesBad, label + ": exactly one bad data block");
        Check(!r.HashesOk(), label + ": not ok");
        std::vector<uint8_t> data;
        Check(package->ReadAll(*package->Find("a.bin"), data), label + ": still readable");
    }

    void TestConvertAndRefuse(const std::filesystem::path& work)
    {
        const std::string label = "convert";
        stfs::Builder builder(TestMetadata());
        builder.AddFile("x.bin", Pattern(5000, 21));
        const std::filesystem::path out = work / "fake_pirs.bin";
        builder.Save(out);
        {
            // Our own package dressed as PIRS, with a "Microsoft" signature area.
            std::fstream f(out, std::ios::in | std::ios::out | std::ios::binary);
            f.write("PIRS", 4);
            const std::vector<uint8_t> junk = Pattern(0x100, 22);
            f.write(reinterpret_cast<const char*>(junk.data()), junk.size());
        }
        auto pirs = Reopen(out, label);
        if (!pirs)
            return;
        Check(pirs->GetMetadata().magic == stfs::Magic::Pirs, label + ": reads as PIRS");
        CheckVerified(*pirs, label + " (PIRS)", stfs::SignatureStatus::NotChecked);
        auto edit = stfs::Builder::FromPackage(pirs);
        std::string error;
        Check(!edit->Save(work / "refused.bin", &error), label + ": saving a PIRS package is refused");
        Check(!std::filesystem::exists(work / "refused.bin"), label + ": nothing written");
        edit->GetMetadata().magic = stfs::Magic::Con;
        const std::filesystem::path con = work / "converted.bin";
        Check(edit->Save(con, &error), label + ": save as CON " + error);
        auto converted = Reopen(con, label);
        if (!converted)
            return;
        Check(converted->GetMetadata().magic == stfs::Magic::Con, label + ": magic CON");
        ContentsMatch(*converted, {{"x.bin", Pattern(5000, 21)}}, label);
        CheckVerified(*converted, label, stfs::SignatureStatus::Unsigned);

        // A byte copy keeps every byte.
        const std::filesystem::path copy = work / "copy.bin";
        Check(stfs::CopyPackage(out, copy), label + ": copy");
        std::ifstream a(out, std::ios::binary), b(copy, std::ios::binary);
        const std::vector<char> da((std::istreambuf_iterator<char>(a)), {}), db((std::istreambuf_iterator<char>(b)), {});
        Check(da == db, label + ": copy is byte-identical");
    }

    // A throwaway RSA-1024 key from bcrypt, in the shapes a keyvault holds (XECRYPT_RSAPRV_1024, certificate).
    bool TestKey(std::vector<uint8_t>& xecrypt, std::vector<uint8_t>& certificate)
    {
        BCRYPT_ALG_HANDLE alg = nullptr;
        BCRYPT_KEY_HANDLE key = nullptr;
        if (BCryptOpenAlgorithmProvider(&alg, BCRYPT_RSA_ALGORITHM, nullptr, 0) != 0)
            return false;
        bool ok = BCryptGenerateKeyPair(alg, &key, 1024, 0) == 0 && BCryptFinalizeKeyPair(key, 0) == 0;
        std::vector<uint8_t> blob;
        ULONG size = 0;
        ok = ok && BCryptExportKey(key, nullptr, BCRYPT_RSAPRIVATE_BLOB, nullptr, 0, &size, 0) == 0;
        blob.resize(size);
        ok = ok && BCryptExportKey(key, nullptr, BCRYPT_RSAPRIVATE_BLOB, blob.data(), size, &size, 0) == 0;
        if (key)
            BCryptDestroyKey(key);
        BCryptCloseAlgorithmProvider(alg, 0);
        if (!ok)
            return false;
        const auto* head = reinterpret_cast<const BCRYPT_RSAKEY_BLOB*>(blob.data());
        const uint8_t* p = blob.data() + sizeof(*head);
        uint32_t e = 0;
        for (ULONG i = 0; i < head->cbPublicExp; ++i)
            e = e << 8 | p[i];
        const uint8_t* n = p + head->cbPublicExp;
        const uint8_t* p1 = n + head->cbModulus;
        const uint8_t* p2 = p1 + head->cbPrime1;
        // Big-endian -> BnQw (qwords reversed, bytes within each kept).
        auto toBnQw = [](const uint8_t* be, size_t len, uint8_t* out)
        {
            for (size_t q = 0; q < len / 8; ++q)
                std::memcpy(out + q * 8, be + len - (q + 1) * 8, 8);
        };
        xecrypt.assign(0x1D0, 0);
        stfs::PutBe32(xecrypt.data(), 0x10);
        stfs::PutBe32(xecrypt.data() + 4, e);
        toBnQw(n, 0x80, xecrypt.data() + 0x10);
        toBnQw(p1, 0x40, xecrypt.data() + 0x90);
        toBnQw(p2, 0x40, xecrypt.data() + 0xD0);
        certificate.assign(stfs::kCertificateSize, 0);
        stfs::PutBe16(certificate.data(), 0x1A8);
        std::memcpy(certificate.data() + 0x07, "X000000-001", 11); // part number (header 0x0B)
        stfs::PutBe32(certificate.data() + 0x24, e);
        toBnQw(n, 0x80, certificate.data() + 0x28);
        return true;
    }

    void TestSigning(const std::filesystem::path& work)
    {
        const std::string label = "signing (generated test key)";
        std::vector<uint8_t> xecrypt, certificate;
        if (!TestKey(xecrypt, certificate))
        {
            Check(false, label + ": bcrypt key generation");
            return;
        }
        stfs::ConsoleKey key;
        std::string error;
        Check(stfs::ConsoleKey::FromXeCrypt(xecrypt, certificate, key, &error), label + ": key " + error);
        stfs::Builder builder(TestMetadata());
        builder.AddFile("save.dat", Pattern(20000, 31));
        stfs::Builder::SaveOptions options;
        options.key = &key;
        const std::filesystem::path out = work / "signed.bin";
        Check(builder.Save(out, options, &error), label + ": save signed " + error);
        auto package = Reopen(out, label);
        if (!package)
            return;
        CheckVerified(*package, label, stfs::SignatureStatus::Valid);
        Check(std::memcmp(package->HeaderBytes().data() + 4, certificate.data(), certificate.size()) == 0,
              label + ": certificate written");

        // Any change in the signed region breaks it; a change outside it (display name) breaks the header hash,
        // which is inside it.
        std::vector<uint8_t> header = package->HeaderBytes();
        header[stfs::offsets::kHeaderSize - 1] ^= 1;
        Check(stfs::VerifySignature(header) == stfs::SignatureStatus::Invalid, label + ": tampered region rejected");
        header = package->HeaderBytes();
        header[stfs::offsets::kConSignature + 5] ^= 0x10;
        Check(stfs::VerifySignature(header) == stfs::SignatureStatus::Invalid, label + ": tampered signature rejected");

        // A key that does not match the certificate is refused.
        stfs::ConsoleKey wrong = key;
        wrong.certificate[0x28 + 3] ^= 0xFF;
        Check(!builder.Save(work / "wrong.bin", {false, &wrong}, &error), label + ": mismatched certificate refused");
        Check(!std::filesystem::exists(work / "wrong.bin"), label + ": nothing written for a refused key");

        // Editing a signed package without a key keeps the old signature, which then no longer verifies.
        auto edit = stfs::Builder::FromPackage(package);
        edit->AddFile("more.dat", Pattern(10, 32));
        Check(edit->Save(work / "signed_edited.bin", &error), label + ": edit without key " + error);
        auto edited = Reopen(work / "signed_edited.bin", label);
        if (edited)
            CheckVerified(*edited, label + " edited without key", stfs::SignatureStatus::Invalid);
    }

    void TestReal(const std::filesystem::path& path)
    {
        const std::string label = "real " + path.filename().string();
        auto package = Reopen(path, label);
        if (!package)
            return;
        const stfs::VerifyReport r = package->Verify();
        Check(r.HashesOk(), label + ": hash chain verifies");
        Check(r.dataBlocksChecked == package->GetMetadata().volume.totalBlocks - package->GetMetadata().volume.freeBlocks,
              label + ": every allocated block checked");
        std::printf("  %s: %s, %zu entries, %u blocks, signature %s\n", label.c_str(),
                    stfs::MagicName(package->GetMetadata().magic), package->Entries().size(),
                    package->GetMetadata().volume.totalBlocks, stfs::SignatureStatusName(r.signature));
        if (!r.HashesOk())
            PrintReport(r);
        // Metadata written back over its own header reproduces it byte for byte.
        std::vector<uint8_t> header = package->HeaderBytes();
        stfs::WriteMetadata(package->GetMetadata(), header);
        Check(header == package->HeaderBytes(), label + ": WriteMetadata(ParseMetadata(header)) == header");
    }

    int SelfTest(int argc, char** argv)
    {
        const std::filesystem::path work = argv[2];
        std::filesystem::create_directories(work);
        TestCreate(work, false);
        TestCreate(work, true);
        TestLevel2(work);
        TestCorruption(work);
        TestConvertAndRefuse(work);
        TestSigning(work);
        for (int i = 3; i < argc; ++i)
            TestReal(argv[i]);
        std::printf("%d checks, %d failed\n", g_checks, g_failures);
        return g_failures ? 1 : 0;
    }
}

int main(int argc, char** argv)
{
    if (argc >= 3 && !std::strcmp(argv[1], "dump"))
        return Dump(argv[2]);
    if (argc >= 3 && !std::strcmp(argv[1], "info"))
        return Info(argv[2]);
    if (argc >= 3 && !std::strcmp(argv[1], "selftest"))
        return SelfTest(argc, argv);
    std::printf("usage: stfs_test dump|info <package>  |  stfs_test selftest <work dir> [<package>...]\n");
    return 2;
}
