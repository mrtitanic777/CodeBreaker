#pragma once

// A read-only STFS package (CON / LIVE / PIRS) opened from a file: its header and metadata, its file system (list,
// stat, read a range) and a check of its hash chain and signature (docs/x360/stfs.md). Reading never needs a valid
// hash or signature; Verify only reports. Thread-safe for concurrent reads (positional file reads, cached hash
// tables under a lock).
//
// Runtime-neutral: it logs through the runtime's "log.h" (Log), like native/x360/system/live.

#include "stfs/format.h"

#include <cstdint>
#include <filesystem>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace stfs
{
    struct Entry
    {
        uint32_t index = 0;          // slot in the file table (what children's parent refers to)
        std::string path;            // '/'-separated from the package root, e.g. "Media/Bkg.xzp"
        DirEntry raw;
        const std::string& Name() const { return raw.name; }
        bool IsDirectory() const { return raw.directory; }
        uint32_t Size() const { return raw.size; }
    };

    enum class SignatureStatus
    {
        NotChecked, // LIVE / PIRS: signed with Microsoft's key, which the library does not carry
        Valid,      // CON: the console signature verifies with the certificate's public key
        Invalid,    // CON: it does not (modified, or never signed)
        Unsigned,   // CON: the signature area is all zero
    };
    const char* SignatureStatusName(SignatureStatus status);

    struct VerifyReport
    {
        bool headerHashOk = false;
        bool topHashOk = false;
        uint32_t dataBlocksChecked = 0, dataBlocksBad = 0;
        uint32_t tablesChecked = 0, tablesBad = 0;
        SignatureStatus signature = SignatureStatus::NotChecked;
        std::vector<std::string> problems; // the first mismatches, readable (capped)
        // The hash chain is intact (signature not included: it is only reported).
        bool HashesOk() const { return headerHashOk && topHashOk && !dataBlocksBad && !tablesBad; }
    };

    class Package
    {
    public:
        // Opens and parses the header and file table. Null (and *error) when the file is not a readable STFS package.
        static std::unique_ptr<Package> Open(const std::filesystem::path& path, std::string* error = nullptr);
        ~Package();
        Package(const Package&) = delete;
        Package& operator=(const Package&) = delete;

        const std::filesystem::path& Path() const { return path_; }
        const Metadata& GetMetadata() const { return meta_; }
        // The header's raw bytes: [0, headerSize), the signature / certificate area included.
        const std::vector<uint8_t>& HeaderBytes() const { return header_; }
        const Layout& GetLayout() const { return layout_; }
        uint64_t FileSize() const { return fileSize_; }

        // Every file and directory, in file-table order.
        const std::vector<Entry>& Entries() const { return entries_; }
        // Case-insensitive; '/' or '\\'; "" or "/" is the root (null: the root has no entry).
        const Entry* Find(std::string_view path) const;
        // Direct children of a directory ("" = root). Empty when it is not a directory.
        std::vector<const Entry*> List(std::string_view directory) const;

        // Reads up to size bytes at offset of a file; returns the count read (0 at / past the end), -1 on an error
        // (I/O failure or a broken block chain).
        int64_t Read(const Entry& entry, uint64_t offset, void* out, size_t size) const;
        bool ReadAll(const Entry& entry, std::vector<uint8_t>& out) const;

        // Hash chain (every allocated data block, every table, the top hash, the header hash) and the signature.
        VerifyReport Verify() const;

    private:
        Package() = default;
        bool Load(std::string* error);
        bool ReadAt(uint64_t offset, void* out, size_t size) const;
        // The active copy of the level-<level> table covering data block <block>, cached.
        const uint8_t* HashTable(uint32_t block, int level, uint64_t* backingOut = nullptr) const;
        bool HashEntryOf(uint32_t block, uint32_t& info, Sha1Digest* hash = nullptr) const;
        // Data block numbers of a file, following the chain (cached per entry).
        bool Chain(const Entry& entry, std::vector<uint32_t>& out) const;

        std::filesystem::path path_;
        void* file_ = nullptr; // HANDLE
        uint64_t fileSize_ = 0;
        std::vector<uint8_t> header_;
        Metadata meta_;
        Layout layout_;
        int topLevel_ = 0;
        std::vector<Entry> entries_;
        std::unordered_map<uint32_t, size_t> bySlot_; // file-table slot -> entries_ index

        mutable std::mutex mutex_; // the caches
        mutable std::unordered_map<uint64_t, std::vector<uint8_t>> tables_;      // backing block -> 4 KiB
        mutable std::unordered_map<uint32_t, std::vector<uint32_t>> chains_;    // slot -> blocks
    };

    // Byte copy of a package file (how packages move between devices: LIVE / PIRS bytes are never rewritten).
    bool CopyPackage(const std::filesystem::path& from, const std::filesystem::path& to, std::string* error = nullptr);
}
