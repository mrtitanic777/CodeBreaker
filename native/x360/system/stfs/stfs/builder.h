#pragma once

// Writing STFS packages (Shared/docs/stfs.md, "Writing"): a new CON package from metadata, or an existing package's
// files and header taken over and changed (add / replace / delete files), saved as a complete new file with every
// hash level, the top hash and the header hash recomputed. Only CON packages are written: a LIVE / PIRS package is
// never rewritten (moving one between devices is a byte copy, stfs::CopyPackage); setting the metadata's magic to CON
// converts one (its Microsoft signature area is cleared, the license set to unrestricted).
//
// Signing: with SaveOptions::key (the user's own console key, never part of the library) the CON header is signed;
// without one the signature area keeps what it had (a new package: zero, i.e. unsigned).
//
// Save writes "<out>.tmp" and then renames it over <out>, so <out> may be the source package's own file.

#include "stfs/format.h"
#include "stfs/package.h"

#include <cstdint>
#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace stfs
{
    struct ConsoleKey;

    // FAT date (high 16 bits) and time (low 16 bits) of now, local time, as the file table stores it.
    uint32_t FatTimestampNow();

    class Builder
    {
    public:
        // A new, empty CON package (meta.magic is forced to CON; the volume fields are computed on Save).
        explicit Builder(const Metadata& meta);
        // Starts from an existing package: its header bytes, metadata and files (contents read from it on Save).
        static std::unique_ptr<Builder> FromPackage(std::shared_ptr<const Package> source);

        Metadata& GetMetadata() { return meta_; }
        // The header bytes the metadata is written over on Save (licenses, certificate and signature, reserved
        // areas are kept from here).
        std::vector<uint8_t>& HeaderTemplate() { return header_; }

        // Paths are '/'- or '\\'-separated, case-insensitive; missing parent directories are created. A name is at
        // most 40 characters. Adding an existing file replaces it.
        bool AddDirectory(std::string_view path, std::string* error = nullptr);
        bool AddFile(std::string_view path, std::vector<uint8_t> data, std::string* error = nullptr);
        bool AddFileFromDisk(std::string_view path, const std::filesystem::path& source, std::string* error = nullptr);
        // A file, or a directory with everything in it.
        bool Remove(std::string_view path);
        bool Exists(std::string_view path) const;
        size_t EntryCount() const { return nodes_.size(); }

        struct SaveOptions
        {
            // Read-only format: one copy of each hash table (what the system's packages use). Default: two copies
            // (read-write format, what LCEStudio's console-made saves have).
            bool readOnlyFormat = false;
            const ConsoleKey* key = nullptr; // sign the CON header with this key
        };
        bool Save(const std::filesystem::path& out, const SaveOptions& options, std::string* error = nullptr);
        bool Save(const std::filesystem::path& out, std::string* error = nullptr) { return Save(out, SaveOptions{}, error); }

    private:
        struct Node
        {
            std::string path; // as given (case kept)
            bool directory = false;
            enum class From { Memory, Disk, Package } from = From::Memory;
            std::vector<uint8_t> data;
            std::filesystem::path disk;
            const Entry* entry = nullptr; // in source_
            uint64_t size = 0;
            uint32_t updated = 0, accessed = 0;
        };

        bool Insert(std::string_view path, Node node, std::string* error);
        bool EnsureParents(const std::string& path, std::string* error);

        Metadata meta_;
        Magic originalMagic_ = Magic::Con;
        std::vector<uint8_t> header_;
        std::shared_ptr<const Package> source_;
        std::map<std::string, Node> nodes_; // key: lower-case path, '/' as '\x01' so parents sort before children
    };
}
