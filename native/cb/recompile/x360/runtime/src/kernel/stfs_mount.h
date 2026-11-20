#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

// STFS package files (CON / LIVE / PIRS) as content packages (native/x360/system/stfs, docs/stfs.md). The runtime's
// file I/O works on host folders, so a package is mounted through its mirror: its files extracted once into
// <data directory>/stfs_mirror/<id>/files (kept while the package file's size and time do not change). A writable
// mount (CON packages only) is written back into the package file on XamContentFlush / XamContentClose, rehashed
// and signed with the console key (ConsoleKey); LIVE / PIRS packages are only ever read or byte-copied.
namespace stfs
{
    struct ConsoleKey;
}

namespace kernel::stfs_mount
{
    // This console's key and certificate: the user's (console_settings.ini console_keyvault: KV 0x298 / 0x9C8), or
    // without a keyvault a stand-in console's of the runtime's own making, generated once and kept in the data
    // directory (console_standin_key.bin; its console ID from the data directory, like the stand-in serial number).
    // CON packages this console writes carry its signature; XAM checks them against it (kernel/xboxkrnl_console_key.cpp).
    const stfs::ConsoleKey& ConsoleKey();
    // Signs, once, every CON package under `root` whose signature area is empty (packages this runtime wrote before it
    // signed them): XAM ignores unsigned profiles and saves, as a console does. Returns how many were signed.
    uint32_t SignUnsignedPackages(const std::filesystem::path& root);
    // The hard drive's folder packages (the native XAM's: Content/<XUID>/<TitleID>/<Type>/<Name>/ with its header in
    // .../Headers/<Type>/<Name>.header) written as STFS package files, as a console keeps them, once; the folder and
    // its .header move to <data directory>/folder_packages_backup/. Returns how many were converted.
    uint32_t ConvertFolderPackages(const std::filesystem::path& contentRoot);

    // True when the file starts with an STFS package magic.
    bool IsPackageFile(const std::filesystem::path& file);
    // The package's mirror folder, extracted if needed. nullopt (and *error) when the file cannot be read.
    std::optional<std::filesystem::path> Mirror(const std::filesystem::path& package, std::string* error);
    // A fingerprint of the mirror's files (relative paths, sizes, modification times): a mount whose fingerprint did
    // not change since it was taken has nothing to write back.
    uint64_t TreeSignature(const std::filesystem::path& mirror);
    // CommitAsync: Commit on a background thread (XamContentClose: the rebuild took ~60 ms on the title's thread).
    // Until it is done, everything here and WaitForWrites wait for it; a "pending" marker in the mirror makes the next
    // Mirror() finish a write the process did not live to do. WaitForAllWrites: before the process ends.
    void CommitAsync(const std::filesystem::path& package, const std::filesystem::path& mirror);
    void WaitForWrites(const std::filesystem::path& package);
    void WaitForAllWrites();
    // The mirror of a package a console's STFS driver mounts (StfsCreateDevice): as Mirror, and for a package that has
    // a header but no file system yet (XAM writes a new package's header itself, then mounts it) an empty folder.
    // Bytes [offset, offset + size) of a file another handle (XAM's, with delete access) has open; shorter at its end.
    std::vector<uint8_t> ReadShared(const std::filesystem::path& file, uint64_t offset, uint32_t size);
    std::optional<std::filesystem::path> DeviceMirror(const std::filesystem::path& package, std::string* error);
    // Writes the mirror's files into the CON package with the header now at its start, in place (the package file
    // stays the same file: XAM keeps a handle to it), and returns the header written (its volume descriptor describes
    // the new file system; XAM writes, hashes and signs the header itself after).
    bool CommitInPlace(const std::filesystem::path& package, const std::filesystem::path& mirror,
                       std::vector<uint8_t>& header, std::string* error);
    // Writes the mirror's files into the CON package (replacing its file system), keeping its header and metadata.
    bool Commit(const std::filesystem::path& package, const std::filesystem::path& mirror, std::string* error);
    // Replaces a CON package's header and metadata (the console's header bytes) and rehashes it; its files stay.
    bool RewriteHeader(const std::filesystem::path& package, const std::vector<uint8_t>& header, std::string* error);
    // A new, empty CON package with this header (the console's header bytes: magic, metadata from 0x344).
    bool CreateEmpty(const std::filesystem::path& package, const std::vector<uint8_t>& header, std::string* error);
    // A folder package (extracted files + its header bytes) written as an STFS package file.
    bool FromFolder(const std::filesystem::path& folder, const std::vector<uint8_t>& header,
                    const std::filesystem::path& package, std::string* error);
}
