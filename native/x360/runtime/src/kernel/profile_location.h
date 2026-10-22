#pragma once

#include <cstdint>
#include <filesystem>
#include <vector>

// Where a profile's package is (kernel/storage.h devices): the hard drive's folder (content/<XUID>/FFFE07D1/00010000/
// <XUID>/, the runtime's own format), or an STFS profile package (a CON file from a console, or one moved there) on
// any device, whose files are used through its mirror (kernel/stfs_mount.h) and written back after every change.
// A profile on a USB stick stays a profile, as on the console: it is listed, signed in and stored to there.
namespace kernel::profile_location
{
    struct Location
    {
        uint32_t device = 0;           // 0: nowhere yet (a new profile goes to the hard drive)
        std::filesystem::path package; // the folder or the STFS file
        bool stfs = false;
    };

    // The profile's package (found once, then cached until Forget).
    Location Find(uint64_t xuid);
    // The folder its files are read and written in: the package folder, an STFS package's mirror, or (nowhere) the
    // hard drive's folder path for a new profile.
    std::filesystem::path Folder(uint64_t xuid);
    // After a file in Folder(xuid) changed: an STFS package gets the folder written back into it (CON only).
    void Changed(uint64_t xuid);
    // Every profile XUID with a package on some device (offline XUIDs, E...), hard drive first.
    std::vector<uint64_t> All();
    // The package moved or was deleted: look it up again next time.
    void Forget(uint64_t xuid);
    // The devices changed (a drive plugged in or removed): every profile is looked up again.
    void ForgetAll();
}
