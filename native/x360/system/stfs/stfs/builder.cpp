// Writing STFS packages (stfs/builder.h): lay out the file table and the files' blocks, hash every level, write the
// header, rename into place.

#include "stfs/builder.h"
#include "stfs/sign.h"

#include "log.h"

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

#include <algorithm>
#include <cstring>
#include <format>
#include <fstream>

namespace stfs
{
    namespace
    {
        std::string Utf8(const std::filesystem::path& p)
        {
            const std::u8string s = p.u8string();
            return std::string(s.begin(), s.end());
        }

        std::string Normalize(std::string_view path)
        {
            std::string s(path);
            std::replace(s.begin(), s.end(), '\\', '/');
            while (!s.empty() && s.front() == '/')
                s.erase(s.begin());
            while (!s.empty() && s.back() == '/')
                s.pop_back();
            return s;
        }

        std::string Key(const std::string& normalized)
        {
            std::string k = normalized;
            for (char& c : k)
                c = c == '/' ? '\x01' : (c >= 'A' && c <= 'Z' ? char(c + 32) : c);
            return k;
        }

        std::string BaseName(const std::string& path)
        {
            const size_t slash = path.rfind('/');
            return slash == std::string::npos ? path : path.substr(slash + 1);
        }

        std::string Parent(const std::string& path)
        {
            const size_t slash = path.rfind('/');
            return slash == std::string::npos ? std::string() : path.substr(0, slash);
        }

        bool Fail(std::string* error, std::string message)
        {
            if (error)
                *error = std::move(message);
            return false;
        }

        class OutFile
        {
        public:
            explicit OutFile(const std::filesystem::path& path)
            {
                handle_ = CreateFileW(path.c_str(), GENERIC_WRITE | DELETE, 0, nullptr, CREATE_ALWAYS,
                                      FILE_ATTRIBUTE_NORMAL, nullptr);
            }
            ~OutFile() { Close(); }
            bool Ok() const { return handle_ != INVALID_HANDLE_VALUE; }
            bool WriteAt(uint64_t offset, const void* data, size_t size)
            {
                OVERLAPPED overlapped{};
                overlapped.Offset = DWORD(offset);
                overlapped.OffsetHigh = DWORD(offset >> 32);
                DWORD written = 0;
                return WriteFile(handle_, data, DWORD(size), &written, &overlapped) && written == size;
            }
            bool SetSize(uint64_t size)
            {
                FILE_END_OF_FILE_INFO info{};
                info.EndOfFile.QuadPart = LONGLONG(size);
                return SetFileInformationByHandle(handle_, FileEndOfFileInfo, &info, sizeof(info));
            }
            void Close()
            {
                if (Ok())
                    CloseHandle(handle_);
                handle_ = INVALID_HANDLE_VALUE;
            }

        private:
            HANDLE handle_ = INVALID_HANDLE_VALUE;
        };

        // Rename over the target even while a reader (Package, opened with FILE_SHARE_DELETE) holds it open.
        bool ReplaceFile(const std::filesystem::path& from, const std::filesystem::path& to)
        {
            HANDLE h = CreateFileW(from.c_str(), DELETE | SYNCHRONIZE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                                   nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
            if (h != INVALID_HANDLE_VALUE)
            {
                const std::wstring target = std::filesystem::absolute(to).wstring();
                std::vector<uint8_t> buffer(sizeof(FILE_RENAME_INFO) + target.size() * sizeof(wchar_t));
                auto* info = reinterpret_cast<FILE_RENAME_INFO*>(buffer.data());
                info->Flags = 0x1 | 0x2; // FILE_RENAME_FLAG_REPLACE_IF_EXISTS | FILE_RENAME_FLAG_POSIX_SEMANTICS
                info->FileNameLength = DWORD(target.size() * sizeof(wchar_t));
                std::memcpy(info->FileName, target.c_str(), info->FileNameLength);
                const bool ok = SetFileInformationByHandle(h, FileRenameInfoEx, info, DWORD(buffer.size()));
                CloseHandle(h);
                if (ok)
                    return true;
            }
            return MoveFileExW(from.c_str(), to.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH);
        }
    }

    uint32_t FatTimestampNow()
    {
        FILETIME utc{}, local{};
        GetSystemTimeAsFileTime(&utc);
        FileTimeToLocalFileTime(&utc, &local);
        WORD date = 0, time = 0;
        FileTimeToDosDateTime(&local, &date, &time);
        return uint32_t(date) << 16 | time;
    }

    Builder::Builder(const Metadata& meta) : meta_(meta)
    {
        meta_.magic = Magic::Con;
        if (meta_.headerSize < offsets::kMetadataEnd)
            meta_.headerSize = kDefaultHeaderSize;
        header_.assign(meta_.headerSize, 0);
        // One unrestricted license (id all ones), as console-made content has.
        std::memset(header_.data() + offsets::kLicenses, 0xFF, 8);
    }

    std::unique_ptr<Builder> Builder::FromPackage(std::shared_ptr<const Package> source)
    {
        auto builder = std::make_unique<Builder>(source->GetMetadata());
        builder->meta_ = source->GetMetadata();
        builder->originalMagic_ = builder->meta_.magic;
        builder->header_ = source->HeaderBytes();
        builder->source_ = source;
        for (const Entry& e : source->Entries())
        {
            Node node;
            node.path = e.path;
            node.directory = e.IsDirectory();
            node.from = Node::From::Package;
            node.entry = &e;
            node.size = e.Size();
            node.updated = e.raw.updated;
            node.accessed = e.raw.accessed;
            builder->nodes_[Key(e.path)] = std::move(node);
        }
        return builder;
    }

    bool Builder::EnsureParents(const std::string& path, std::string* error)
    {
        const std::string parent = Parent(path);
        if (parent.empty())
            return true;
        auto it = nodes_.find(Key(parent));
        if (it != nodes_.end())
            return it->second.directory ? true : Fail(error, std::format("{} is a file", parent));
        Node dir;
        dir.directory = true;
        return Insert(parent, std::move(dir), error);
    }

    bool Builder::Insert(std::string_view rawPath, Node node, std::string* error)
    {
        const std::string path = Normalize(rawPath);
        if (path.empty())
            return Fail(error, "empty path");
        const std::string name = BaseName(path);
        if (name.empty() || name.size() > kMaxNameLength)
            return Fail(error, std::format("name '{}' must be 1..40 characters", name));
        if (!EnsureParents(path, error))
            return false;
        const std::string key = Key(path);
        auto it = nodes_.find(key);
        if (it != nodes_.end() && it->second.directory != node.directory)
            return Fail(error, std::format("{} exists as a {}", path, it->second.directory ? "directory" : "file"));
        if (it != nodes_.end() && node.directory)
            return true;
        node.path = path;
        node.updated = node.accessed = FatTimestampNow();
        nodes_[key] = std::move(node);
        return true;
    }

    bool Builder::AddDirectory(std::string_view path, std::string* error)
    {
        Node node;
        node.directory = true;
        return Insert(path, std::move(node), error);
    }

    bool Builder::AddFile(std::string_view path, std::vector<uint8_t> data, std::string* error)
    {
        if (data.size() > 0xFFFFFFFFull)
            return Fail(error, "files are at most 4 GiB");
        Node node;
        node.size = data.size();
        node.data = std::move(data);
        return Insert(path, std::move(node), error);
    }

    bool Builder::AddFileFromDisk(std::string_view path, const std::filesystem::path& source, std::string* error)
    {
        std::error_code ec;
        const uint64_t size = std::filesystem::file_size(source, ec);
        if (ec)
            return Fail(error, std::format("{}: {}", Utf8(source), ec.message()));
        if (size > 0xFFFFFFFFull)
            return Fail(error, "files are at most 4 GiB");
        Node node;
        node.from = Node::From::Disk;
        node.disk = source;
        node.size = size;
        return Insert(path, std::move(node), error);
    }

    bool Builder::Remove(std::string_view path)
    {
        const std::string key = Key(Normalize(path));
        if (key.empty() || !nodes_.erase(key))
            return false;
        const std::string prefix = key + '\x01';
        for (auto it = nodes_.lower_bound(prefix); it != nodes_.end() && it->first.starts_with(prefix);)
            it = nodes_.erase(it);
        return true;
    }

    bool Builder::Exists(std::string_view path) const
    {
        return nodes_.contains(Key(Normalize(path)));
    }

    bool Builder::Save(const std::filesystem::path& out, const SaveOptions& options, std::string* error)
    {
        if (meta_.magic != Magic::Con)
            return Fail(error, std::format("{} packages are never rewritten (set the magic to CON to convert one)",
                                           MagicName(meta_.magic)));
        if (nodes_.size() >= 0xFFFF)
            return Fail(error, "too many files");

        // File table order: parents before children (the map's key order). Blocks: the file table first, then each
        // file's blocks in a row (contiguous).
        struct Item
        {
            const Node* node;
            DirEntry entry;
        };
        std::vector<Item> items;
        std::map<std::string, uint32_t> slotOf;
        const uint32_t tableBlocks = std::max<uint32_t>(1, uint32_t((nodes_.size() + kDirEntriesPerBlock - 1) / kDirEntriesPerBlock));
        uint64_t cursor = tableBlocks;
        for (const auto& [key, node] : nodes_)
        {
            Item item{&node, {}};
            item.entry.name = BaseName(node.path);
            item.entry.directory = node.directory;
            const std::string parentKey = Key(Parent(node.path));
            item.entry.parent = parentKey.empty() ? -1 : int32_t(slotOf.at(parentKey));
            item.entry.updated = node.updated;
            item.entry.accessed = node.accessed;
            if (!node.directory)
            {
                const uint32_t blocks = uint32_t((node.size + kBlockSize - 1) / kBlockSize);
                item.entry.size = uint32_t(node.size);
                item.entry.validBlocks = item.entry.allocatedBlocks = blocks;
                item.entry.startBlock = blocks ? uint32_t(cursor) : 0;
                item.entry.contiguous = blocks != 0;
                cursor += blocks;
            }
            slotOf[key] = uint32_t(items.size());
            items.push_back(std::move(item));
        }
        if (cursor > kMaxBlocks)
            return Fail(error, "the files need more blocks than STFS holds");
        const uint32_t total = uint32_t(cursor);

        VolumeDescriptor& volume = meta_.volume;
        volume = VolumeDescriptor{};
        volume.flags = options.readOnlyFormat ? 1 : 0;
        volume.fileTableBlocks = uint16_t(tableBlocks);
        volume.fileTableBlock = 0;
        volume.totalBlocks = total;
        volume.freeBlocks = 0;
        meta_.volumeType = 0;
        const Layout layout = Layout::For(meta_.headerSize, volume);
        const int top = TopHashLevel(total);
        meta_.contentSize = BackingSize(layout, total);

        const std::filesystem::path temp = std::filesystem::path(out).concat(L".tmp");
        OutFile file(temp);
        if (!file.Ok())
            return Fail(error, std::format("cannot create {} (error {})", Utf8(temp), GetLastError()));
        auto abandon = [&](std::string message)
        {
            file.Close();
            DeleteFileW(temp.c_str());
            Log("STFS: writing {} failed: {}", Utf8(out), message);
            return Fail(error, std::move(message));
        };
        if (!file.SetSize(layout.base + meta_.contentSize))
            return abandon("cannot size the file");

        std::vector<std::vector<uint8_t>> tables[3];
        for (int level = 0; level <= top; ++level)
            tables[level].assign(HashTableCount(total, level), std::vector<uint8_t>(kBlockSize, 0));

        // Data blocks with their level-0 entries (allocated, next block of the chain).
        std::vector<uint8_t> block(kBlockSize);
        uint32_t index = 0;
        auto put = [&](uint32_t next) -> bool
        {
            if (!file.WriteAt(layout.Offset(layout.DataBlockToBacking(index)), block.data(), kBlockSize))
                return false;
            uint8_t* entry = tables[0][index / kHashesPerTable].data() + HashRecord(index, 0) * kHashEntrySize;
            const Sha1Digest h = Sha1(block);
            std::memcpy(entry, h.data(), 20);
            PutBe32(entry + 20, 0x80000000u | (next & 0xFFFFFF));
            ++index;
            return true;
        };
        std::vector<uint8_t> fileTable(size_t(tableBlocks) * kBlockSize, 0);
        for (size_t i = 0; i < items.size(); ++i)
            WriteDirEntry(items[i].entry, fileTable.data() + i * kDirEntrySize);
        for (uint32_t b = 0; b < tableBlocks; ++b)
        {
            std::memcpy(block.data(), fileTable.data() + size_t(b) * kBlockSize, kBlockSize);
            if (!put(b + 1 < tableBlocks ? index + 1 : kEndOfChain))
                return abandon("write error");
        }
        for (const Item& item : items)
        {
            const Node& node = *item.node;
            const uint32_t blocks = item.entry.validBlocks;
            if (node.directory || !blocks)
                continue;
            // Opened sharing read, write and delete: a file a runtime still has open (a title's save, XAM's profile
            // settings) can be read into the package.
            HANDLE disk = INVALID_HANDLE_VALUE;
            if (node.from == Node::From::Disk)
            {
                disk = CreateFileW(node.disk.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                                   nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
                if (disk == INVALID_HANDLE_VALUE)
                    return abandon(std::format("cannot read {} (error {})", Utf8(node.disk), GetLastError()));
            }
            struct CloseOnExit
            {
                HANDLE h;
                ~CloseOnExit() { if (h != INVALID_HANDLE_VALUE) CloseHandle(h); }
            } closeDisk{ disk };
            for (uint32_t b = 0; b < blocks; ++b)
            {
                const uint64_t at = uint64_t(b) * kBlockSize;
                const size_t take = size_t(std::min<uint64_t>(kBlockSize, node.size - at));
                std::fill(block.begin(), block.end(), uint8_t(0));
                bool ok = true;
                switch (node.from)
                {
                case Node::From::Memory: std::memcpy(block.data(), node.data.data() + at, take); break;
                case Node::From::Disk:
                {
                    DWORD read = 0;
                    ok = ReadFile(disk, block.data(), DWORD(take), &read, nullptr) && read == take;
                    break;
                }
                case Node::From::Package: ok = source_->Read(*node.entry, at, block.data(), take) == int64_t(take); break;
                }
                if (!ok)
                    return abandon(std::format("cannot read the contents of {}", node.path));
                if (!put(b + 1 < blocks ? index + 1 : kEndOfChain))
                    return abandon("write error");
            }
        }

        // Each level's tables hashed into the level above (entry info 0: copy 0 active, no free blocks); the top
        // table into the volume descriptor. Read-write packages get both copies the same.
        for (int level = 0; level <= top; ++level)
        {
            for (uint32_t t = 0; t < tables[level].size(); ++t)
            {
                const std::vector<uint8_t>& table = tables[level][t];
                const Sha1Digest h = Sha1(table);
                if (level == top)
                    volume.topHash = h;
                else
                {
                    uint8_t* entry = tables[level + 1][t / kHashesPerTable].data() + (t % kHashesPerTable) * kHashEntrySize;
                    std::memcpy(entry, h.data(), 20);
                }
                const uint64_t backing = layout.HashTableBacking(t * kBlocksPerLevel[level], level);
                for (uint32_t copy = 0; copy < layout.tablesPerBlock; ++copy)
                    if (!file.WriteAt(layout.Offset(backing + copy), table.data(), kBlockSize))
                        return abandon("write error");
            }
        }

        // Header: metadata, header hash, signature.
        std::vector<uint8_t> header = header_;
        header.resize(size_t(layout.base), 0);
        if (originalMagic_ != Magic::Con)
        {
            // Converted from LIVE / PIRS: Microsoft's signature cannot carry over.
            std::memset(header.data() + offsets::kCertificate, 0, offsets::kLicenses - offsets::kCertificate);
            std::memset(header.data() + offsets::kLicenses, 0, offsets::kHeaderHash - offsets::kLicenses);
            std::memset(header.data() + offsets::kLicenses, 0xFF, 8);
        }
        WriteMetadata(meta_, header);
        const Sha1Digest headerHash = Sha1({header.data() + offsets::kSignedEnd, header.size() - offsets::kSignedEnd});
        std::memcpy(header.data() + offsets::kHeaderHash, headerHash.data(), 20);
        meta_.headerHash = headerHash;
        if (options.key)
        {
            std::string why;
            if (!SignCon(header, *options.key, &why))
                return abandon("signing: " + why);
        }
        if (!file.WriteAt(0, header.data(), header.size()))
            return abandon("write error");
        file.Close();
        if (!ReplaceFile(temp, out))
        {
            const DWORD code = GetLastError();
            DeleteFileW(temp.c_str());
            return Fail(error, std::format("cannot replace {} (error {})", Utf8(out), code));
        }
        header_ = std::move(header);
        originalMagic_ = Magic::Con;
        return true;
    }
}
