// cb/verify/gate16/fix95_cache.hpp -- FIX95CPU's patched files, cached, so Setup installs them and the install needs no FIX95CPU disk.
//
// The first restart of a Windows 95 install fails at "While initializing device IOS" on a host this fast; FIX95CPU's floppy copies patched drivers
// (IOS.VXD, ESDI_506.PDR, NDIS.VXD ...) over the installed ones. The files are FIX95CPU's own: the run that goes through the floppy reads them back
// off the installed disk (each must be the very file the floppy holds) and keeps them under the cache folder, keyed on the FIX95CPU.IMA's hash and
// the Setup CD's. A later run puts them among the files Setup installs (setup16's Replace: the CD's cabinets are written again with them), and the
// floppy stage is skipped.
//
//     <cache>/<key>/manifest.txt        written last: a folder without it is not a cache
//     <cache>/<key>/files/<NAME>        one file each, by name (the cabinets know files by name)
//
// What the floppy does besides copying files (the registry's Setup\Updates bookkeeping, WININIT.EXE) is not part of this.
#pragma once

#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/verify/gate16/setup16.hpp"

namespace cb::verify::gate16::fix95 {

// SHA-256 (lower-case hex) of a file's bytes. IoError when it cannot be read.
std::string hash_file(const std::filesystem::path& file);

// The cache's key: FIX95CPU.IMA's hash and the Setup CD's (the CD's files are the originals the patched ones replace: they are pinned by it).
std::string key(std::string_view fix_hash, std::string_view iso_hash);

// A file FIX95CPU copies: where it lands under the Windows folder ("SYSTEM/VMM32/IOS.VXD"; the folders of the floppy's W95BOTH / W95RTM / W95OSR2
// are the editions, and POST only an order of copying), and the hash of the floppy's bytes.
struct Wanted {
    std::string name;        // upper case: "IOS.VXD"
    std::string under;       // under the Windows folder, '/' between names
    std::string hash;
};
// Every file of the FIX95CPU disk image that is copied into Windows (all three editions' folders).
std::vector<Wanted> files_of(const std::filesystem::path& ima);

struct Captured {
    std::string name;
    std::string disk_path;   // as found on the disk, from the root ("WINDOWS/SYSTEM/VMM32/IOS.VXD")
    Bytes data;
};
struct Capture {
    std::vector<Captured> files;
    std::vector<std::string> conflicts;   // names two different places on the disk hold, or two different files: not replaceable by name
    std::vector<std::string> notes;       // each file of the floppy that is not on the disk as the floppy has it, and why
};
// The files of `ima` that the installed disk `disk_image` (C:, its first FAT volume with an MSDOS.SYS) holds byte for byte. The edition the
// installation is of decides which of the floppy's copies are there; the others are listed in `notes`.
Capture capture(const std::filesystem::path& disk_image, const std::filesystem::path& ima);

struct File {
    std::string name, disk_path, patched_hash, original_hash;
};
struct Entry {
    std::filesystem::path folder;
    std::vector<File> files;
    std::vector<std::string> additions;       // files FIX95CPU adds that no cabinet of the CD has (OSR2's NTKERN.VXD): Setup installs nothing of that name, so they stay out
    std::vector<std::string> unreplaceable;   // captured names that conflict (two places or two contents on the disk): not replaceable by name
    // The entry is a whole replacement of FIX95CPU's work that Setup can install: something to install, and nothing ambiguous.
    bool usable() const { return !files.empty() && unreplaceable.empty(); }
    std::filesystem::path path_of(const std::string& name) const { return folder / "files" / name; }
};

// Write the entry: the files, then the manifest. `originals` is setup16::cabinet_originals of the captured names (a name missing from it is not in
// any cabinet: unreplaceable). The folder is replaced if it was there.
Entry store(const std::filesystem::path& folder, const Capture& capture, const std::map<std::string, std::string>& originals);

// The entry in `folder`, every file checked against the manifest's hash; nullopt for no entry, a damaged one, or a manifest of another version.
std::optional<Entry> load(const std::filesystem::path& folder);

// What the run does with the cache.
struct Plan {
    std::optional<setup16::Replace> replace;   // `replace` plus the entry's files: the floppy stage is skipped. None: it is not.
    std::string note;                          // one line: used, or why not
};
// The files Setup installs with the cache: the run's own plus the entry's. No pre-apply when there is no usable entry, or when a file of the entry
// is one the run replaces itself (the run's wins, and then FIX95CPU's version of it is not installed).
Plan plan(const setup16::Replace& replace, const std::optional<Entry>& entry);

}  // namespace cb::verify::gate16::fix95
