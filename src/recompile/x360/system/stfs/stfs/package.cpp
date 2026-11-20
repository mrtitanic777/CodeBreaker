// Reading an STFS package (stfs/package.h): header, file table, block chains through the hash tables, verification.

#include "stfs/package.h"
#include "stfs/sign.h"

#include "log.h"

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

#include <algorithm>
#include <cstring>
#include <format>
#include <functional>

namespace stfs
{
    namespace
    {
        constexpr size_t kMaxProblems = 32;

        std::string Utf8(const std::filesystem::path& p)
        {
            const std::u8string s = p.u8string();
            return std::string(s.begin(), s.end());
        }

        std::string Hex(const uint8_t* p, size_t n)
        {
            std::string s;
            for (size_t i = 0; i < n; ++i)
                s += std::format("{:02x}", p[i]);
            return s;
        }

        // "a\\B/c/" -> "a/B/c"
        std::string NormalizePath(std::string_view path)
        {
            std::string s(path);
            std::replace(s.begin(), s.end(), '\\', '/');
            while (!s.empty() && s.front() == '/')
                s.erase(s.begin());
            while (!s.empty() && s.back() == '/')
                s.pop_back();
            return s;
        }

        bool EqualNoCase(std::string_view a, std::string_view b)
        {
            if (a.size() != b.size())
                return false;
            for (size_t i = 0; i < a.size(); ++i)
            {
                const unsigned char x = a[i], y = b[i];
                if ((x >= 'A' && x <= 'Z' ? x + 32 : x) != (y >= 'A' && y <= 'Z' ? y + 32 : y))
                    return false;
            }
            return true;
        }
    }

    const char* SignatureStatusName(SignatureStatus status)
    {
        switch (status)
        {
        case SignatureStatus::NotChecked: return "not checked (Microsoft key)";
        case SignatureStatus::Valid: return "valid";
        case SignatureStatus::Invalid: return "invalid";
        case SignatureStatus::Unsigned: return "unsigned";
        }
        return "?";
    }

    std::unique_ptr<Package> Package::Open(const std::filesystem::path& path, std::string* error)
    {
        std::unique_ptr<Package> package(new Package());
        package->path_ = path;
        std::string message;
        if (!package->Load(&message))
        {
            Log("STFS: {}: {}", Utf8(path), message);
            if (error)
                *error = message;
            return nullptr;
        }
        return package;
    }

    Package::~Package()
    {
        if (file_)
            CloseHandle(file_);
    }

    bool Package::ReadAt(uint64_t offset, void* out, size_t size) const
    {
        // Past the end of the file reads as zero (a package whose last block was cut short).
        auto* dst = static_cast<uint8_t*>(out);
        size_t available = offset >= fileSize_ ? 0 : size_t(std::min<uint64_t>(size, fileSize_ - offset));
        std::memset(dst + available, 0, size - available);
        while (available)
        {
            OVERLAPPED overlapped{};
            overlapped.Offset = DWORD(offset);
            overlapped.OffsetHigh = DWORD(offset >> 32);
            const DWORD chunk = DWORD(std::min<size_t>(available, 1u << 30));
            DWORD got = 0;
            if (!ReadFile(file_, dst, chunk, &got, &overlapped) || got == 0)
                return false;
            dst += got;
            offset += got;
            available -= got;
        }
        return true;
    }

    bool Package::Load(std::string* error)
    {
        auto fail = [&](std::string message)
        {
            *error = std::move(message);
            return false;
        };
        HANDLE h = CreateFileW(path_.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                               nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (h == INVALID_HANDLE_VALUE)
            return fail(std::format("cannot open (error {})", GetLastError()));
        file_ = h;
        LARGE_INTEGER size{};
        GetFileSizeEx(h, &size);
        fileSize_ = uint64_t(size.QuadPart);
        if (fileSize_ < offsets::kMetadataEnd)
            return fail("too small for an STFS header");

        std::vector<uint8_t> head(offsets::kMetadataEnd);
        if (!ReadAt(0, head.data(), head.size()))
            return fail("read error");
        std::string why;
        if (!ParseMetadata(head, meta_, &why))
            return fail(why);
        if (meta_.headerSize < offsets::kMetadataEnd || meta_.headerSize > 0x100000)
            return fail(std::format("implausible header size {:#x}", meta_.headerSize));
        if (meta_.volumeType != 0)
            return fail(std::format("volume type {} (SVOD) is not STFS", meta_.volumeType));
        if (meta_.volume.length != 0x24)
            return fail(std::format("not an STFS volume descriptor (length {:#x})", meta_.volume.length));
        if (meta_.volume.totalBlocks > kMaxBlocks)
            return fail(std::format("{} blocks is more than STFS holds", meta_.volume.totalBlocks));

        layout_ = Layout::For(meta_.headerSize, meta_.volume);
        topLevel_ = TopHashLevel(meta_.volume.totalBlocks);
        header_.resize(size_t(layout_.base));
        if (!ReadAt(0, header_.data(), header_.size()))
            return fail("read error");

        // File table: a chain of blocks of 64 entries. Empty slots are skipped (their slot numbers stay taken, since
        // children name their parent by slot).
        uint32_t block = meta_.volume.fileTableBlock;
        std::vector<uint8_t> buffer(kBlockSize);
        for (uint32_t n = 0; n < meta_.volume.fileTableBlocks; ++n)
        {
            if (block >= meta_.volume.totalBlocks)
                return fail(std::format("file table block {} out of range", block));
            if (!ReadAt(layout_.Offset(layout_.DataBlockToBacking(block)), buffer.data(), kBlockSize))
                return fail("read error");
            for (uint32_t i = 0; i < kDirEntriesPerBlock; ++i)
            {
                Entry e;
                if (!ParseDirEntry(buffer.data() + i * kDirEntrySize, e.raw))
                    continue;
                e.index = n * kDirEntriesPerBlock + i;
                bySlot_[e.index] = entries_.size();
                entries_.push_back(std::move(e));
            }
            if (n + 1 == meta_.volume.fileTableBlocks)
                break;
            uint32_t info = 0;
            if (!HashEntryOf(block, info))
                return fail("read error");
            block = info & 0xFFFFFF;
            if (block == kEndOfChain)
                break;
        }

        // Paths (parents may come after their children in the table).
        std::vector<int> state(entries_.size(), 0); // 0 todo, 1 in progress, 2 done
        std::function<bool(size_t)> resolve = [&](size_t i) -> bool
        {
            if (state[i] == 2)
                return true;
            if (state[i] == 1)
                return false; // cycle
            state[i] = 1;
            Entry& e = entries_[i];
            e.path = e.raw.name;
            if (e.raw.parent >= 0)
            {
                auto parent = bySlot_.find(uint32_t(e.raw.parent));
                if (parent == bySlot_.end() || !resolve(parent->second))
                    return false;
                e.path = entries_[parent->second].path + "/" + e.raw.name;
            }
            state[i] = 2;
            return true;
        };
        for (size_t i = 0; i < entries_.size(); ++i)
            if (!resolve(i))
                return fail(std::format("file table entry {} has a broken parent", entries_[i].raw.name));
        return true;
    }

    const uint8_t* Package::HashTable(uint32_t block, int level, uint64_t* backingOut) const
    {
        std::lock_guard lock(mutex_);
        auto load = [&](uint64_t backing) -> const uint8_t*
        {
            auto it = tables_.find(backing);
            if (it == tables_.end())
            {
                std::vector<uint8_t> table(kBlockSize);
                if (!ReadAt(layout_.Offset(backing), table.data(), kBlockSize))
                    return nullptr;
                it = tables_.emplace(backing, std::move(table)).first;
            }
            return it->second.data();
        };
        // Read-write packages: the volume descriptor picks the copy of the top table, each entry of a level-N table
        // (bit 30) the copy of the table below it.
        const bool twoCopies = layout_.tablesPerBlock == 2;
        uint32_t copy = twoCopies && meta_.volume.RootActiveIndex() ? 1 : 0;
        for (int l = topLevel_; l > level; --l)
        {
            const uint8_t* parent = load(layout_.HashTableBacking(block, l) + copy);
            if (!parent)
                return nullptr;
            const uint32_t info = Be32(parent + HashRecord(block, l) * kHashEntrySize + 20);
            copy = twoCopies ? (info >> 30) & 1 : 0;
        }
        const uint64_t backing = layout_.HashTableBacking(block, level) + copy;
        if (backingOut)
            *backingOut = backing;
        return load(backing);
    }

    bool Package::HashEntryOf(uint32_t block, uint32_t& info, Sha1Digest* hash) const
    {
        const uint8_t* table = HashTable(block, 0);
        if (!table)
            return false;
        const uint8_t* entry = table + HashRecord(block, 0) * kHashEntrySize;
        info = Be32(entry + 20);
        if (hash)
            std::memcpy(hash->data(), entry, 20);
        return true;
    }

    bool Package::Chain(const Entry& entry, std::vector<uint32_t>& out) const
    {
        {
            std::lock_guard lock(mutex_);
            auto it = chains_.find(entry.index);
            if (it != chains_.end())
            {
                out = it->second;
                return true;
            }
        }
        const uint32_t count = uint32_t((uint64_t(entry.raw.size) + kBlockSize - 1) / kBlockSize);
        std::vector<uint32_t> blocks;
        blocks.reserve(count);
        uint32_t block = entry.raw.startBlock;
        for (uint32_t i = 0; i < count; ++i)
        {
            if (block >= meta_.volume.totalBlocks)
            {
                Log("STFS: {}: {}: block chain leaves the package at block {} of {}", Utf8(path_), entry.path, i,
                    count);
                return false;
            }
            blocks.push_back(block);
            if (i + 1 == count)
                break;
            if (entry.raw.contiguous)
                ++block;
            else
            {
                uint32_t info = 0;
                if (!HashEntryOf(block, info))
                    return false;
                block = info & 0xFFFFFF;
            }
        }
        std::lock_guard lock(mutex_);
        out = chains_.emplace(entry.index, std::move(blocks)).first->second;
        return true;
    }

    const Entry* Package::Find(std::string_view path) const
    {
        const std::string wanted = NormalizePath(path);
        if (wanted.empty())
            return nullptr;
        for (const Entry& e : entries_)
            if (EqualNoCase(e.path, wanted))
                return &e;
        return nullptr;
    }

    std::vector<const Entry*> Package::List(std::string_view directory) const
    {
        std::vector<const Entry*> out;
        int32_t parent = -1;
        if (!NormalizePath(directory).empty())
        {
            const Entry* dir = Find(directory);
            if (!dir || !dir->IsDirectory())
                return out;
            parent = int32_t(dir->index);
        }
        for (const Entry& e : entries_)
            if (e.raw.parent == parent)
                out.push_back(&e);
        return out;
    }

    int64_t Package::Read(const Entry& entry, uint64_t offset, void* out, size_t size) const
    {
        if (entry.IsDirectory())
            return -1;
        if (offset >= entry.raw.size || size == 0)
            return 0;
        size = size_t(std::min<uint64_t>(size, entry.raw.size - offset));
        std::vector<uint32_t> blocks;
        if (!Chain(entry, blocks))
            return -1;
        auto* dst = static_cast<uint8_t*>(out);
        size_t done = 0;
        while (done < size)
        {
            const uint64_t at = offset + done;
            const uint32_t n = uint32_t(at / kBlockSize);
            const uint32_t within = uint32_t(at % kBlockSize);
            size_t take = std::min<size_t>(size - done, kBlockSize - within);
            // Runs of consecutive data blocks inside one 170-block group are consecutive on disk too.
            while (done + take < size)
            {
                const uint32_t next = uint32_t((at + take) / kBlockSize);
                if (blocks[next] != blocks[next - 1] + 1 || blocks[next] % kBlocksPerLevel[0] == 0)
                    break;
                take += std::min<size_t>(size - done - take, kBlockSize);
            }
            if (!ReadAt(layout_.Offset(layout_.DataBlockToBacking(blocks[n])) + within, dst + done, take))
                return -1;
            done += take;
        }
        return int64_t(done);
    }

    bool Package::ReadAll(const Entry& entry, std::vector<uint8_t>& out) const
    {
        out.resize(entry.raw.size);
        return Read(entry, 0, out.data(), out.size()) == int64_t(out.size());
    }

    VerifyReport Package::Verify() const
    {
        VerifyReport r;
        auto problem = [&](std::string text)
        {
            if (r.problems.size() < kMaxProblems)
                r.problems.push_back(std::move(text));
        };

        const Sha1Digest headerHash = Sha1({header_.data() + offsets::kSignedEnd, header_.size() - offsets::kSignedEnd});
        r.headerHashOk = std::memcmp(headerHash.data(), header_.data() + offsets::kHeaderHash, 20) == 0;
        if (!r.headerHashOk)
            problem(std::format("header hash {} (stored {})", Hex(headerHash.data(), 20),
                                Hex(header_.data() + offsets::kHeaderHash, 20)));

        const uint32_t total = meta_.volume.totalBlocks;
        std::vector<uint8_t> buffer(kBlockSize);
        for (uint32_t b = 0; b < total; ++b)
        {
            uint32_t info = 0;
            Sha1Digest stored{};
            if (!HashEntryOf(b, info, &stored))
            {
                problem(std::format("block {}: hash table unreadable", b));
                ++r.dataBlocksBad;
                continue;
            }
            if (!(info & 0x80000000))
                continue; // not allocated
            ++r.dataBlocksChecked;
            if (!ReadAt(layout_.Offset(layout_.DataBlockToBacking(b)), buffer.data(), kBlockSize) ||
                Sha1(buffer) != stored)
            {
                ++r.dataBlocksBad;
                problem(std::format("data block {} hash mismatch", b));
            }
        }

        // Each table against its entry in the table above; the top table against the volume descriptor.
        for (int level = 0; level <= topLevel_; ++level)
        {
            const uint32_t count = HashTableCount(total, level);
            for (uint32_t t = 0; t < count; ++t)
            {
                const uint32_t block = t * kBlocksPerLevel[level];
                uint64_t backing = 0;
                const uint8_t* table = HashTable(block, level, &backing);
                ++r.tablesChecked;
                if (!table)
                {
                    ++r.tablesBad;
                    problem(std::format("level {} table {} unreadable", level, t));
                    continue;
                }
                const Sha1Digest h = Sha1({table, kBlockSize});
                if (level == topLevel_)
                {
                    r.topHashOk = std::memcmp(h.data(), meta_.volume.topHash.data(), 20) == 0;
                    if (!r.topHashOk)
                        problem(std::format("top hash (level {} table at block {}) mismatch", level, backing));
                    continue;
                }
                const uint8_t* parent = HashTable(block, level + 1);
                if (!parent ||
                    std::memcmp(parent + HashRecord(block, level + 1) * kHashEntrySize, h.data(), 20) != 0)
                {
                    ++r.tablesBad;
                    problem(std::format("level {} table {} (block {}) hash mismatch", level, t, backing));
                }
            }
        }

        r.signature = VerifySignature(header_);
        return r;
    }

    bool CopyPackage(const std::filesystem::path& from, const std::filesystem::path& to, std::string* error)
    {
        if (CopyFileW(from.c_str(), to.c_str(), FALSE))
            return true;
        const DWORD code = GetLastError();
        Log("STFS: copying {} to {} failed (error {})", Utf8(from), Utf8(to), code);
        if (error)
            *error = std::format("copy failed (error {})", code);
        return false;
    }
}
