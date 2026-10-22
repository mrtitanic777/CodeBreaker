// STFS package files as content packages: see stfs_mount.h.

#include "kernel/stfs_mount.h"

#include "kernel/console_keyvault.h"

#include "log.h"
#include "settings.h"
#include "stfs/builder.h"
#include "stfs/package.h"
#include "stfs/sign.h"

#include <algorithm>
#include <chrono>
#include <cstring>
#include <format>
#include <condition_variable>
#include <deque>
#include <fstream>
#include <mutex>
#include <set>
#include <thread>

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

namespace fs = std::filesystem;

namespace kernel::stfs_mount
{
    namespace
    {
        std::mutex g_mutex;

        // The user's console key from console_settings.ini's console_keyvault (read once): the console's
        // XECRYPT_RSAPRV_1024 at KV 0x298 (0x1D0 bytes) and its certificate at KV 0x9C8 (0x1A8 bytes)
        // (docs/x360/stfs.md "Signatures"). Null: unsigned. Its bytes are never logged.
        // The stand-in console's key (no keyvault): console_standin_key.bin = "SKEY", certificate (0x1A8), exponent,
        // modulus (0x80), primes (0x40 + 0x40), big-endian. Generated on the first run without a keyvault.
        stfs::ConsoleKey StandInKey()
        {
            const fs::path file = DataDirectory() / "console_standin_key.bin";
            stfs::ConsoleKey key;
            std::ifstream in(file, std::ios::binary);
            std::vector<uint8_t> bytes((std::istreambuf_iterator<char>(in)), {});
            if (bytes.size() == 4 + stfs::kCertificateSize + 4 + 0x80 + 0x80 && std::memcmp(bytes.data(), "SKEY", 4) == 0)
            {
                const uint8_t* p = bytes.data() + 4;
                std::memcpy(key.certificate.data(), p, stfs::kCertificateSize);
                p += stfs::kCertificateSize;
                key.exponent = stfs::Be32(p);
                p += 4;
                key.modulus.assign(p, p + 0x80);
                key.prime1.assign(p + 0x80, p + 0xC0);
                key.prime2.assign(p + 0xC0, p + 0x100);
                return key;
            }
            // Console ID: 5 bytes from the data directory's name (FNV-1a), as the stand-in serial number is.
            uint64_t h = 0xCBF29CE484222325ull;
            for (wchar_t c : DataDirectory().wstring())
                h = (h ^ uint64_t(towlower(c))) * 0x100000001B3ull;
            std::array<uint8_t, 5> id;
            for (int i = 0; i < 5; i++)
                id[i] = uint8_t(h >> (8 * (4 - i)));
            std::string error;
            if (!stfs::GenerateConsoleKey(id, key, &error))
                Fatal("STFS: no stand-in console key: {}", error);
            std::vector<uint8_t> out;
            out.insert(out.end(), { 'S', 'K', 'E', 'Y' });
            out.insert(out.end(), key.certificate.begin(), key.certificate.end());
            out.insert(out.end(), { uint8_t(key.exponent >> 24), uint8_t(key.exponent >> 16), uint8_t(key.exponent >> 8),
                                    uint8_t(key.exponent) });
            out.insert(out.end(), key.modulus.begin(), key.modulus.end());
            out.insert(out.end(), key.prime1.begin(), key.prime1.end());
            out.insert(out.end(), key.prime2.begin(), key.prime2.end());
            std::ofstream(file, std::ios::binary).write(reinterpret_cast<const char*>(out.data()), std::streamsize(out.size()));
            Log("STFS: a stand-in console key was made for this installation (console_standin_key.bin)");
            return key;
        }

        const stfs::ConsoleKey* SigningKey()
        {
            static const std::optional<stfs::ConsoleKey> key = []() -> std::optional<stfs::ConsoleKey> {
                const std::vector<uint8_t>& kv = kernel::ConsoleKeyvault();
                const std::string& path = kernel::ConsoleKeyvaultSource();
                if (kv.empty())
                {
                    stfs::ConsoleKey standIn = StandInKey();
                    std::vector<uint8_t> probe(0x344, 0);
                    probe[0] = 'C', probe[1] = 'O', probe[2] = 'N', probe[3] = ' ';
                    std::string error;
                    if (!stfs::SignCon(probe, standIn, &error))
                        Fatal("STFS: the stand-in console key does not sign: {} (delete console_standin_key.bin)", error);
                    Log("STFS: CON packages are signed with this installation's stand-in console key");
                    return standIn;
                }
                stfs::ConsoleKey out;
                std::string error;
                if (!stfs::ConsoleKey::FromXeCrypt(std::span(kv).subspan(0x298, 0x1D0),
                                                   std::span(kv).subspan(0x9C8, 0x1A8), out, &error))
                    Fatal("{}: {}", path, error);
                // A key that does not match its certificate is refused here, not at the first save.
                std::vector<uint8_t> probe(0x344, 0);
                probe[0] = 'C', probe[1] = 'O', probe[2] = 'N', probe[3] = ' ';
                if (!stfs::SignCon(probe, out, &error))
                    Fatal("{}: the key does not sign with its certificate: {}", path, error);
                Log("STFS: CON packages are signed with the console key of {}", path);
                return out;
            }();
            return key ? &*key : nullptr;
        }

        // The mirror's folder: one per package file (FNV-1a of its lower-case absolute path).
        fs::path MirrorRoot(const fs::path& package)
        {
            std::wstring key = fs::absolute(package).wstring();
            uint64_t h = 0xCBF29CE484222325ull;
            for (wchar_t c : key)
                h = (h ^ uint64_t(towlower(c))) * 0x100000001B3ull;
            return DataDirectory() / "stfs_mirror" / std::format("{:016X}", h);
        }

        // What the mirror was extracted from: the file's size and write time.
        std::string Stamp(const fs::path& package)
        {
            std::error_code ec;
            const auto size = fs::file_size(package, ec);
            const auto time = fs::last_write_time(package, ec).time_since_epoch().count();
            return std::format("{} {} {}\n", size, time, fs::absolute(package).string());
        }

        std::string ReadText(const fs::path& path)
        {
            std::ifstream in(path, std::ios::binary);
            return std::string((std::istreambuf_iterator<char>(in)), {});
        }

        void WriteText(const fs::path& path, const std::string& text)
        {
            std::ofstream out(path, std::ios::binary | std::ios::trunc);
            out << text;
        }

        // The package's files into folder (replacing what is there).
        bool Extract(const stfs::Package& package, const fs::path& folder, std::string* error)
        {
            std::error_code ec;
            fs::remove_all(folder, ec);
            fs::create_directories(folder, ec);
            std::vector<uint8_t> data;
            for (const stfs::Entry& e : package.Entries())
            {
                const fs::path out = folder / fs::path(e.path).make_preferred();
                if (e.IsDirectory())
                {
                    fs::create_directories(out, ec);
                    continue;
                }
                fs::create_directories(out.parent_path(), ec);
                if (!package.ReadAll(e, data))
                {
                    if (error)
                        *error = std::format("cannot read {} from {}", e.path, package.Path().string());
                    return false;
                }
                std::ofstream f(out, std::ios::binary | std::ios::trunc);
                f.write(reinterpret_cast<const char*>(data.data()), std::streamsize(data.size()));
                if (!f)
                {
                    if (error)
                        *error = std::format("cannot write {}", out.string());
                    return false;
                }
            }
            return true;
        }

        // Every file and folder under folder into the builder.
        bool AddTree(stfs::Builder& builder, const fs::path& folder, std::string* error)
        {
            std::error_code ec;
            for (auto it = fs::recursive_directory_iterator(folder, ec); it != fs::recursive_directory_iterator();
                 it.increment(ec))
            {
                const std::string path = fs::relative(it->path(), folder, ec).generic_string();
                if (it->is_directory(ec) ? !builder.AddDirectory(path, error)
                                         : !builder.AddFileFromDisk(path, it->path(), error))
                    return false;
            }
            return !ec;
        }
    }

    bool IsPackageFile(const fs::path& file)
    {
        std::ifstream in(file, std::ios::binary);
        char magic[4] = {};
        in.read(magic, 4);
        return in.gcount() == 4 &&
            (!std::memcmp(magic, "CON ", 4) || !std::memcmp(magic, "LIVE", 4) || !std::memcmp(magic, "PIRS", 4));
    }

    // Mirror / Commit with g_mutex held.
    std::optional<fs::path> MirrorLocked(const fs::path& package, std::string* error);
    bool CommitLocked(const fs::path& package, const fs::path& mirror, std::string* error);

    namespace
    {
        // Background write-backs (CommitAsync): packages queued or being written, keyed by their mirror folder.
        std::mutex g_queueMutex;
        std::condition_variable g_queueChanged;
        std::deque<std::pair<fs::path, fs::path>> g_queue; // package, mirror
        std::set<fs::path> g_pending;

        void WriteBackThread()
        {
            for (;;)
            {
                std::pair<fs::path, fs::path> job;
                {
                    std::unique_lock lock(g_queueMutex);
                    g_queueChanged.wait(lock, [] { return !g_queue.empty(); });
                    job = g_queue.front();
                    g_queue.pop_front();
                }
                std::string error;
                bool ok;
                {
                    std::lock_guard lock(g_mutex);
                    ok = CommitLocked(job.first, job.second, &error);
                }
                if (!ok)
                    Fatal("STFS: cannot write package {}: {}", job.first.string(), error);
                std::error_code ec;
                fs::remove(MirrorRoot(job.first) / "pending", ec);
                {
                    std::lock_guard lock(g_queueMutex);
                    g_pending.erase(MirrorRoot(job.first));
                }
                g_queueChanged.notify_all();
            }
        }
    }

    void CommitAsync(const fs::path& package, const fs::path& mirror)
    {
        WriteText(MirrorRoot(package) / "pending", fs::absolute(package).string() + "\n");
        static std::once_flag started;
        std::call_once(started, [] { std::thread(WriteBackThread).detach(); });
        {
            std::lock_guard lock(g_queueMutex);
            g_pending.insert(MirrorRoot(package));
            g_queue.emplace_back(package, mirror);
        }
        g_queueChanged.notify_all();
    }

    void WaitForWrites(const fs::path& package)
    {
        std::unique_lock lock(g_queueMutex);
        if (g_pending.empty())
            return;
        const fs::path key = MirrorRoot(package);
        g_queueChanged.wait(lock, [&] { return !g_pending.count(key); });
    }

    void WaitForAllWrites()
    {
        std::unique_lock lock(g_queueMutex);
        g_queueChanged.wait(lock, [] { return g_pending.empty(); });
    }

    std::optional<fs::path> Mirror(const fs::path& package, std::string* error)
    {
        WaitForWrites(package);
        std::lock_guard lock(g_mutex);
        return MirrorLocked(package, error);
    }

    std::optional<fs::path> MirrorLocked(const fs::path& package, std::string* error)
    {
        const fs::path root = MirrorRoot(package), files = root / "files", stampFile = root / "stamp";
        // A background write-back the process did not finish (it ended first): the mirror is newer than the package.
        if (std::error_code ec; fs::exists(root / "pending", ec) && fs::is_directory(files, ec))
        {
            Log("STFS: {}: finishing a write-back interrupted by the end of the previous run", package.string());
            if (!CommitLocked(package, files, error))
                return std::nullopt;
            fs::remove(root / "pending", ec);
        }
        const std::string stamp = Stamp(package);
        if (fs::is_directory(files) && ReadText(stampFile) == stamp)
            return files;
        const auto opened = stfs::Package::Open(package, error);
        if (!opened)
            return std::nullopt;
        if (!Extract(*opened, files, error))
            return std::nullopt;
        std::error_code ec;
        fs::create_directories(root, ec);
        WriteText(stampFile, stamp);
        Log("STFS: {} ({}, {} entries) extracted to {}", package.string(), stfs::MagicName(opened->GetMetadata().magic),
            opened->Entries().size(), files.string());
        return files;
    }

    uint64_t TreeSignature(const fs::path& mirror)
    {
        uint64_t h = 0xCBF29CE484222325ull;
        auto mix = [&h](const void* data, size_t size) {
            for (size_t i = 0; i < size; i++)
                h = (h ^ static_cast<const uint8_t*>(data)[i]) * 0x100000001B3ull;
        };
        std::error_code ec;
        std::vector<std::pair<std::wstring, std::pair<uint64_t, int64_t>>> entries;
        for (auto it = fs::recursive_directory_iterator(mirror, ec); !ec && it != fs::recursive_directory_iterator();
             it.increment(ec))
        {
            const uint64_t size = it->is_regular_file(ec) ? uint64_t(it->file_size(ec)) : ~0ull;
            const int64_t time = int64_t(it->last_write_time(ec).time_since_epoch().count());
            entries.push_back({ fs::relative(it->path(), mirror, ec).wstring(), { size, time } });
        }
        std::sort(entries.begin(), entries.end());
        for (const auto& [path, info] : entries)
        {
            mix(path.data(), path.size() * sizeof(wchar_t));
            mix(&info, sizeof(info));
        }
        return h;
    }

    bool Commit(const fs::path& package, const fs::path& mirror, std::string* error)
    {
        WaitForWrites(package);
        std::lock_guard lock(g_mutex);
        return CommitLocked(package, mirror, error);
    }

    bool CommitLocked(const fs::path& package, const fs::path& mirror, std::string* error)
    {
        const auto started = std::chrono::steady_clock::now();
        std::shared_ptr<const stfs::Package> source = stfs::Package::Open(package, error);
        if (!source)
            return false;
        if (source->GetMetadata().magic != stfs::Magic::Con)
        {
            if (error)
                *error = "only CON packages are written (LIVE / PIRS stay as Microsoft signed them)";
            return false;
        }
        stfs::Builder builder(source->GetMetadata());
        builder.HeaderTemplate() = source->HeaderBytes();
        const bool readOnlyFormat = source->GetMetadata().volume.ReadOnlyFormat();
        source.reset(); // the package file is replaced below
        if (!AddTree(builder, mirror, error))
            return false;
        stfs::Builder::SaveOptions options;
        options.readOnlyFormat = readOnlyFormat;
        options.key = SigningKey();
        if (!builder.Save(package, options, error))
            return false;
        WriteText(MirrorRoot(package) / "stamp", Stamp(package));
        Log("STFS: {} written back from {} ({} entries; {}; {:.1f} ms)", package.string(), mirror.string(),
            builder.EntryCount(), options.key ? "signed" : "unsigned",
            std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - started).count());
        return true;
    }

    bool RewriteHeader(const fs::path& package, const std::vector<uint8_t>& header, std::string* error)
    {
        // Built from the package's mirror, not from the open package: replacing a file that is still open fails on
        // FAT32 sticks (error 5).
        WaitForWrites(package);
        std::lock_guard lock(g_mutex);
        stfs::Metadata old;
        bool readOnlyFormat = false;
        uint32_t headerSize = 0;
        {
            const auto source = stfs::Package::Open(package, error);
            if (!source)
                return false;
            if (source->GetMetadata().magic != stfs::Magic::Con)
            {
                if (error)
                    *error = "only CON packages are written (LIVE / PIRS stay as Microsoft signed them)";
                return false;
            }
            readOnlyFormat = source->GetMetadata().volume.ReadOnlyFormat();
            headerSize = source->GetMetadata().headerSize;
        }
        const auto mirror = MirrorLocked(package, error);
        if (!mirror)
            return false;
        stfs::Metadata meta;
        if (!stfs::ParseMetadata(header, meta, error))
            return false;
        meta.headerSize = headerSize;
        stfs::Builder builder(meta);
        builder.HeaderTemplate().assign(header.begin(), header.begin() + std::min<size_t>(header.size(), headerSize));
        if (!AddTree(builder, *mirror, error))
            return false;
        stfs::Builder::SaveOptions options;
        options.readOnlyFormat = readOnlyFormat;
        options.key = SigningKey();
        if (!builder.Save(package, options, error))
            return false;
        WriteText(MirrorRoot(package) / "stamp", Stamp(package));
        return true;
    }

    bool CreateEmpty(const fs::path& package, const std::vector<uint8_t>& header, std::string* error)
    {
        WaitForWrites(package);
        stfs::Metadata meta;
        if (!stfs::ParseMetadata(header, meta, error))
            return false;
        // The package has no <package>.data folder (that is SVOD content): no data files.
        meta.dataFileCount = 0;
        meta.dataFileSize = 0;
        stfs::Builder builder(meta);
        builder.HeaderTemplate().assign(header.begin(), header.begin() + std::min<size_t>(header.size(),
            builder.HeaderTemplate().size()));
        std::error_code ec;
        fs::create_directories(package.parent_path(), ec);
        stfs::Builder::SaveOptions options;
        options.key = SigningKey();
        return builder.Save(package, options, error);
    }

    bool FromFolder(const fs::path& folder, const std::vector<uint8_t>& header, const fs::path& package,
                    std::string* error)
    {
        WaitForWrites(package);
        stfs::Metadata meta;
        if (!stfs::ParseMetadata(header, meta, error))
            return false;
        // The package has no <package>.data folder (that is SVOD content): no data files.
        meta.dataFileCount = 0;
        meta.dataFileSize = 0;
        stfs::Builder builder(meta);
        builder.HeaderTemplate().assign(header.begin(), header.begin() + std::min<size_t>(header.size(),
            builder.HeaderTemplate().size()));
        if (!AddTree(builder, folder, error))
            return false;
        std::error_code ec;
        fs::create_directories(package.parent_path(), ec);
        stfs::Builder::SaveOptions options;
        options.key = SigningKey();
        return builder.Save(package, options, error);
    }
}

namespace kernel::stfs_mount
{
    const stfs::ConsoleKey& ConsoleKey() { return *SigningKey(); }

    namespace
    {
        // The first bytes of a file replaced (its size unchanged), through a handle that shares with others.
        bool WriteHeaderShared(const fs::path& file, const std::vector<uint8_t>& bytes)
        {
            HANDLE h = CreateFileW(file.c_str(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                                   nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
            if (h == INVALID_HANDLE_VALUE)
                return false;
            DWORD written = 0;
            const bool ok = WriteFile(h, bytes.data(), DWORD(bytes.size()), &written, nullptr) && written == bytes.size();
            CloseHandle(h);
            return ok;
        }
    }

    uint32_t SignUnsignedPackages(const fs::path& contentRoot)
    {
        std::error_code ec;
        const fs::path root = contentRoot / "Content";
        if (!fs::is_directory(root, ec))
            return 0;
        uint32_t signedCount = 0;
        for (auto it = fs::recursive_directory_iterator(root, fs::directory_options::skip_permission_denied, ec);
             it != fs::recursive_directory_iterator(); it.increment(ec))
        {
            if (ec || !it->is_regular_file(ec) || it->file_size(ec) < 0xA000 || !IsPackageFile(it->path()))
                continue;
            std::vector<uint8_t> header(0x344);
            {
                std::ifstream in(it->path(), std::ios::binary);
                in.read(reinterpret_cast<char*>(header.data()), std::streamsize(header.size()));
                if (!in)
                    continue;
            }
            if (stfs::Be32(header.data()) != uint32_t(stfs::Magic::Con))
                continue;
            // The native XAM's headers left the license table and the console ID empty; a console's own packages
            // carry an unrestricted license (all ones) and its console ID. Filled in, then the header hash (SHA-1 of
            // [0x344, header size rounded up to 4 KiB), at 0x32C: xam.xex 0x816806xx) is redone and the package signed.
            const bool noLicense = std::all_of(header.begin() + 0x22C, header.begin() + 0x32C, [](uint8_t b) { return b == 0; });
            if (!noLicense && stfs::VerifySignature(header) != stfs::SignatureStatus::Unsigned)
                continue;
            if (noLicense)
            {
                const uint32_t headerSize = (stfs::Be32(header.data() + 0x340) + 0xFFF) & ~0xFFFu;
                std::vector<uint8_t> whole = ReadShared(it->path(), 0, headerSize);
                if (whole.size() != headerSize || headerSize < 0x1000)
                    continue;
                std::fill(whole.begin() + 0x22C, whole.begin() + 0x234, uint8_t(0xFF));
                if (std::all_of(whole.begin() + 0x36C, whole.begin() + 0x371, [](uint8_t b) { return b == 0; }))
                    std::memcpy(whole.data() + 0x36C, ConsoleKey().certificate.data() + 2, 5);
                const stfs::Sha1Digest hash = stfs::Sha1({ whole.data() + 0x344, headerSize - 0x344 });
                std::memcpy(whole.data() + 0x32C, hash.data(), hash.size());
                std::string error;
                if (!stfs::SignCon(whole, ConsoleKey(), &error) || !WriteHeaderShared(it->path(), whole))
                {
                    Log("STFS: {} not repaired: {}", it->path().string(), error);
                    continue;
                }
                signedCount++;
                Log("STFS: {}: the native XAM's header given an unrestricted license and this console's ID, signed",
                    it->path().string());
                continue;
            }
            std::string error;
            if (!stfs::SignCon(header, ConsoleKey(), &error))
            {
                Log("STFS: {} not signed: {}", it->path().string(), error);
                continue;
            }
            std::fstream f(it->path(), std::ios::binary | std::ios::in | std::ios::out);
            f.seekp(0);
            f.write(reinterpret_cast<const char*>(header.data()), std::streamsize(header.size()));
            if (f)
            {
                signedCount++;
                Log("STFS: {} was unsigned: signed with this console's key", it->path().string());
            }
        }
        return signedCount;
    }
}

namespace kernel::stfs_mount
{
    namespace
    {
        // The package's header as its file holds it now: [0, header size) (0x340 says how big), at least 0x344.
        std::vector<uint8_t> ReadHeader(const fs::path& package, std::string* error)
        {
            std::vector<uint8_t> header = ReadShared(package, 0, 0x344);
            if (header.size() < 0x344)
            {
                if (error)
                    *error = std::format("no package header (error {})", GetLastError());
                return {};
            }
            const uint32_t size = stfs::Be32(header.data() + 0x340);
            if (size > header.size() && size < 0x100000)
            {
                const std::vector<uint8_t> rest = ReadShared(package, 0x344, size - 0x344);
                header.insert(header.end(), rest.begin(), rest.end());
            }
            return header;
        }

        // The package's whole content replaced by `bytes` through a handle that shares with XAM's.
        bool WriteShared(const fs::path& file, const std::vector<uint8_t>& bytes)
        {
            HANDLE h = CreateFileW(file.c_str(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                                   nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
            if (h == INVALID_HANDLE_VALUE)
                return false;
            DWORD written = 0;
            const bool ok = WriteFile(h, bytes.data(), DWORD(bytes.size()), &written, nullptr) && written == bytes.size() &&
                SetEndOfFile(h);
            CloseHandle(h);
            return ok;
        }
    }

    std::vector<uint8_t> ReadShared(const fs::path& file, uint64_t offset, uint32_t size)
    {
        HANDLE h = CreateFileW(file.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                               nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (h == INVALID_HANDLE_VALUE)
            return {};
        std::vector<uint8_t> bytes(size);
        OVERLAPPED at{};
        at.Offset = DWORD(offset);
        at.OffsetHigh = DWORD(offset >> 32);
        DWORD read = 0;
        if (!ReadFile(h, bytes.data(), size, &read, &at))
            read = 0;
        CloseHandle(h);
        bytes.resize(read);
        return bytes;
    }

    std::optional<fs::path> DeviceMirror(const fs::path& package, std::string* error)
    {
        WaitForWrites(package);
        std::lock_guard lock(g_mutex);
        std::string openError;
        if (stfs::Package::Open(package, &openError))
            return MirrorLocked(package, error);
        // A header without a file system (a package XAM has just created): nothing to extract yet.
        const std::vector<uint8_t> header = ReadHeader(package, error);
        if (header.empty() || stfs::Be32(header.data()) != uint32_t(stfs::Magic::Con))
        {
            if (error)
                *error = "not a package: " + openError;
            return std::nullopt;
        }
        const fs::path root = MirrorRoot(package), files = root / "files";
        std::error_code ec;
        fs::remove_all(files, ec);
        fs::create_directories(files, ec);
        WriteText(root / "stamp", "");
        Log("STFS: {}: a new package (no file system yet), empty mirror {}", package.string(), files.string());
        return files;
    }

    bool CommitInPlace(const fs::path& package, const fs::path& mirror, std::vector<uint8_t>& header, std::string* error)
    {
        WaitForWrites(package);
        std::lock_guard lock(g_mutex);
        const auto started = std::chrono::steady_clock::now();
        header = ReadHeader(package, error);
        if (header.empty())
            return false;
        stfs::Metadata meta;
        if (!stfs::ParseMetadata(header, meta, error))
            return false;
        if (meta.magic != stfs::Magic::Con)
        {
            if (error)
                *error = "only CON packages are written (LIVE / PIRS stay as Microsoft signed them)";
            return false;
        }
        meta.headerSize = uint32_t(header.size());
        stfs::Builder builder(meta);
        builder.HeaderTemplate() = header;
        if (!AddTree(builder, mirror, error))
            return false;
        stfs::Builder::SaveOptions options;
        options.readOnlyFormat = meta.volume.ReadOnlyFormat();
        options.key = SigningKey();
        const fs::path built = package.string() + ".built";
        if (!builder.Save(built, options, error))
            return false;
        // Copied over the package's own bytes (not renamed over it): the file XAM has open stays the package.
        std::vector<uint8_t> bytes;
        {
            std::ifstream in(built, std::ios::binary);
            bytes.assign(std::istreambuf_iterator<char>(in), {});
        }
        std::error_code ec;
        fs::remove(built, ec);
        if (!WriteShared(package, bytes))
        {
            if (error)
                *error = std::format("cannot write the package file (error {})", GetLastError());
            return false;
        }
        header.assign(bytes.begin(), bytes.begin() + std::min<size_t>(bytes.size(), header.size()));
        WriteText(MirrorRoot(package) / "stamp", Stamp(package));
        Log("STFS: {} written in place from {} ({} entries, {} bytes; {:.1f} ms)", package.string(), mirror.string(),
            builder.EntryCount(), bytes.size(),
            std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - started).count());
        return true;
    }
}

namespace kernel::stfs_mount
{
    uint32_t ConvertFolderPackages(const fs::path& contentRoot)
    {
        std::error_code ec;
        const fs::path content = contentRoot / "Content";
        if (!fs::is_directory(content, ec))
            return 0;
        const fs::path backup = DataDirectory() / "folder_packages_backup";
        uint32_t converted = 0;
        // Content/<XUID>/<TitleID>/<Type>/<Name>
        for (const auto& xuid : fs::directory_iterator(content, ec))
            for (const auto& title : fs::directory_iterator(xuid.path(), ec))
                for (const auto& type : fs::directory_iterator(title.path(), ec))
                {
                    if (!type.is_directory(ec) || type.path().filename() == "Headers")
                        continue;
                    std::vector<fs::path> folders;
                    for (const auto& item : fs::directory_iterator(type.path(), ec))
                        if (item.is_directory(ec))
                            folders.push_back(item.path());
                    for (const fs::path& folder : folders)
                    {
                        const fs::path headerFile = title.path() / "Headers" / type.path().filename() /
                            (folder.filename().string() + ".header");
                        std::ifstream in(headerFile, std::ios::binary);
                        std::vector<uint8_t> header((std::istreambuf_iterator<char>(in)), {});
                        in.close();
                        if (header.size() < 0x344)
                            continue; // not a folder package (no header): left alone
                        const fs::path built = folder.string() + ".package";
                        std::string error;
                        if (!FromFolder(folder, header, built, &error))
                        {
                            Log("STFS: folder package {} not converted: {}", folder.string(), error);
                            fs::remove(built, ec);
                            continue;
                        }
                        const fs::path relative = fs::relative(folder, contentRoot, ec);
                        fs::create_directories((backup / relative).parent_path(), ec);
                        fs::rename(folder, backup / relative, ec);
                        if (ec)
                        {
                            Log("STFS: folder package {} not converted: cannot move it aside ({})", folder.string(),
                                ec.message());
                            fs::remove(built, ec);
                            continue;
                        }
                        fs::rename(built, folder, ec);
                        fs::rename(headerFile, (backup / relative).string() + ".header", ec);
                        converted++;
                        Log("STFS: folder package {} is now a package file (the folder: {})", folder.string(),
                            (backup / relative).string());
                    }
                }
        return converted;
    }
}
