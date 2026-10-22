// Where profiles are: see profile_location.h.

#include "kernel/profile_location.h"

#include "kernel/stfs_mount.h"
#include "kernel/storage.h"
#include "log.h"
#include "settings.h"

#include <algorithm>
#include <format>
#include <map>
#include <mutex>

namespace fs = std::filesystem;

namespace kernel::profile_location
{
    namespace
    {
        std::mutex g_mutex;
        std::map<uint64_t, Location> g_cache;

        fs::path PackageIn(const fs::path& tree, uint64_t xuid)
        {
            const std::string name = std::format("{:016X}", xuid);
            return tree / name / "FFFE07D1" / "00010000" / name;
        }

        Location Lookup(uint64_t xuid)
        {
            std::error_code ec;
            for (const storage::Device& dev : storage::Devices())
            {
                const fs::path package = PackageIn(storage::ContentTree(dev), xuid);
                if (fs::is_directory(package, ec))
                    return { dev.id, package, false };
                if (fs::is_regular_file(package, ec) && stfs_mount::IsPackageFile(package))
                    return { dev.id, package, true };
            }
            return {};
        }

        Location Cached(uint64_t xuid)
        {
            std::lock_guard lock(g_mutex);
            auto it = g_cache.find(xuid);
            if (it == g_cache.end())
                it = g_cache.emplace(xuid, Lookup(xuid)).first;
            return it->second;
        }
    }

    Location Find(uint64_t xuid)
    {
        return Cached(xuid);
    }

    fs::path Folder(uint64_t xuid)
    {
        const Location where = Cached(xuid);
        if (where.device == 0)
            return PackageIn(DataDirectory() / "content", xuid);
        if (!where.stfs)
            return where.package;
        std::string error;
        const auto mirror = stfs_mount::Mirror(where.package, &error);
        if (!mirror)
            Fatal("Profile {:016X}: cannot read its STFS package {}: {}", xuid, where.package.string(), error);
        return *mirror;
    }

    void Changed(uint64_t xuid)
    {
        const Location where = Cached(xuid);
        if (!where.stfs)
            return;
        std::string error;
        const auto mirror = stfs_mount::Mirror(where.package, &error);
        if (!mirror || !stfs_mount::Commit(where.package, *mirror, &error))
            Fatal("Profile {:016X}: cannot write its STFS package {}: {}", xuid, where.package.string(), error);
    }

    std::vector<uint64_t> All()
    {
        std::vector<uint64_t> out;
        std::error_code ec;
        for (const storage::Device& dev : storage::Devices())
            for (const auto& owner : fs::directory_iterator(storage::ContentTree(dev), ec))
            {
                const std::string name = owner.path().filename().string();
                if (name.size() != 16 || name.find_first_not_of("0123456789ABCDEFabcdef") != std::string::npos)
                    continue;
                const uint64_t xuid = std::stoull(name, nullptr, 16);
                if ((xuid >> 60) != 0xE || std::find(out.begin(), out.end(), xuid) != out.end())
                    continue;
                if (Cached(xuid).device == dev.id)
                    out.push_back(xuid);
            }
        return out;
    }

    void Forget(uint64_t xuid)
    {
        std::lock_guard lock(g_mutex);
        g_cache.erase(xuid);
    }

    void ForgetAll()
    {
        std::lock_guard lock(g_mutex);
        g_cache.clear();
    }
}
