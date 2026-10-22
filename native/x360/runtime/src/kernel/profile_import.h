#pragma once

// Console profiles brought onto this hard drive (content/) from the `profiles` folder beside TU46.xex: what the
// Guide's "Download Profile" does here (the console downloads the profile from Xbox LIVE; this PC copies it from
// that folder). A profile is either
// - a profile package copied from a console's hard drive or a USB stick (an STFS "CON " file of content type
//   0x00010000, Content/0000000000000000/FFFE07D1/00010000/<profile ID> on the console), any name, or
// - a folder in Xenia's layout (<ID>/FFFE07D1/00010000/<ID>/ with its Account file), as Xenia and Nexia360 keep them.
// Its files (Account, GPDs, tile_64.png gamer picture, ...) go to content/<ID>/FFFE07D1/00010000/<ID>/, the layout
// kernel/profile_store.h reads (tools/scripts/import_profile.py does the same offline).

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

namespace kernel::profile
{
    // The folder scanned: <TU46.xex's folder>/profiles.
    std::filesystem::path ImportFolder();
    // Copies every profile in ImportFolder() that is not on the hard drive yet; returns their profile IDs (offline
    // XUIDs). Profiles already there are left as they are.
    std::vector<uint64_t> ImportNewProfiles();
    // Gives every profile on the hard drive the package header a console keeps for it (content/<ID>/FFFE07D1/Headers/
    // 00010000/<ID>.header, the layout kernel/xam_content.cpp reads), so XAM lists profiles as content (type 0x10000:
    // System Settings > Storage > Profiles). The header comes from the profile's console package in ImportFolder()
    // (its first 0x971A bytes, as the console stores it); a profile without one (a Xenia folder) gets a header made
    // like a console's: CON, type 0x10000, title FFFE07D1, profile ID at 0x371, display name = the ID in hex,
    // thumbnail = its gamer picture. Profiles that have a header are left alone.
    void EnsureProfileHeaders();
    // Profiles deleted on this console (XamProfileDelete), kept in deleted_profiles.txt in the data folder (one
    // "<ID> <time> <gamertag>" line each): ImportNewProfiles does not copy them again unless their file in
    // ImportFolder() is newer than the deletion, and a gamertag of console_settings.ini that named a deleted profile
    // no longer makes a local profile (kernel/users.h). Deleting the line brings either back.
    void RecordDeleted(uint64_t id, const std::string& gamertag);
    bool IsDeleted(uint64_t id, std::filesystem::file_time_type source = std::filesystem::file_time_type::min());
    bool IsDeletedGamertag(const std::string& gamertag);
}
