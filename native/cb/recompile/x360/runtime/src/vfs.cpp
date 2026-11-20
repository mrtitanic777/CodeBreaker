#include "vfs.h"

#include "log.h"

#include <algorithm>
#include <cctype>
#include <map>
#include <mutex>
#include <vector>

namespace vfs
{
    namespace
    {
        struct Device
        {
            std::filesystem::path root;
            bool readOnly = true;
        };

        std::mutex g_mutex;
        std::map<std::string, Device> g_devices;        // name key -> device
        std::map<std::string, std::string> g_links;     // name key ("savedrive", "\systemroot") -> guest target path

        // NT object names that are devices or links ("\device\flash", "\systemroot"), lower case. Guarded by
        // g_mutex like the maps.
        std::vector<std::string> g_ntNames;

        std::string Lower(std::string s)
        {
            std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return char(std::tolower(c)); });
            return s;
        }

        // Strips "\??\" and returns (device, remainder) for "device:\rest" or for a path below a registered NT
        // object name ("\SystemRoot\rest" -> ("\systemroot", "\rest")), or nullopt. Caller holds g_mutex.
        std::optional<std::pair<std::string, std::string>> SplitDevice(std::string path)
        {
            std::replace(path.begin(), path.end(), '/', '\\');
            // \??\ (the title's DOS devices) and \System??\ (the system process's, where XAM makes its drive
            // letters): one namespace here.
            if (path.rfind("\\??\\", 0) == 0)
            {
                path = path.substr(4);
            }
            else if (Lower(path).rfind("\\system??\\", 0) == 0)
            {
                path = path.substr(10);
            }
            else if (path.rfind("\\", 0) == 0)
            {
                const std::string lower = Lower(path);
                const std::string* best = nullptr;
                for (const auto& name : g_ntNames)
                    if (lower.rfind(name, 0) == 0 && (lower.size() == name.size() || lower[name.size()] == '\\') &&
                        (!best || name.size() > best->size()))
                        best = &name;
                if (!best)
                    return std::nullopt;
                return std::make_pair(*best, path.substr(best->size()));
            }
            const auto colon = path.find(':');
            if (colon == std::string::npos || colon == 0 || path.find('\\') < colon)
                return std::nullopt;
            return std::make_pair(Lower(path.substr(0, colon)), path.substr(colon + 1));
        }

        // Key of a device or link name: "game" for "game:" / "\??\GAME:", "\device\flash" for an NT object name.
        // Caller holds g_mutex.
        std::string NameKey(const std::string& name)
        {
            if (name.rfind("\\", 0) == 0 && name.rfind("\\??\\", 0) != 0 && Lower(name).rfind("\\system??\\", 0) != 0)
                return Lower(name);
            auto split = SplitDevice(name.back() == ':' ? name + "\\" : name);
            return split ? split->first : Lower(name);
        }

        void AddNtName(const std::string& key)
        {
            if (key.rfind("\\", 0) == 0 && std::find(g_ntNames.begin(), g_ntNames.end(), key) == g_ntNames.end())
                g_ntNames.push_back(key);
        }

        // Normalizes "\a\.\b\..\c" to components; nullopt if ".." would leave the root.
        std::optional<std::vector<std::string>> Components(const std::string& rest)
        {
            std::vector<std::string> parts;
            size_t start = 0;
            while (start <= rest.size())
            {
                size_t end = rest.find('\\', start);
                if (end == std::string::npos)
                    end = rest.size();
                const std::string part = rest.substr(start, end - start);
                if (part == "..")
                {
                    if (parts.empty())
                        return std::nullopt;
                    parts.pop_back();
                }
                else if (!part.empty() && part != ".")
                {
                    parts.push_back(part);
                }
                start = end + 1;
            }
            return parts;
        }
    }

    void MountDevice(const std::string& device, const std::filesystem::path& root, bool readOnly)
    {
        std::lock_guard lock(g_mutex);
        const std::string key = NameKey(device);
        AddNtName(key);
        g_devices[key] = { root, readOnly };
        Log("Mounted {} -> {}{}", device, root.string(), readOnly ? " (read-only)" : "");
    }

    bool UnmountDevice(const std::string& device)
    {
        std::lock_guard lock(g_mutex);
        const bool removed = g_devices.erase(NameKey(device)) != 0;
        if (removed)
            Log("Unmounted {}", device);
        return removed;
    }

    std::optional<ResolvedPath> Resolve(const std::string& guestPath)
    {
        std::string path = guestPath;
        std::lock_guard lock(g_mutex);
        for (int depth = 0; depth < 8; depth++)
        {
            auto split = SplitDevice(path);
            if (!split)
                return std::nullopt;
            auto& [device, rest] = *split;
            if (auto link = g_links.find(device); link != g_links.end())
            {
                path = link->second + rest; // follow the symbolic link and resolve again
                continue;
            }
            auto it = g_devices.find(device);
            if (it == g_devices.end())
                return std::nullopt;
            auto parts = Components(rest);
            if (!parts)
                return std::nullopt;
            ResolvedPath resolved{ it->second.root, it->second.readOnly, device };
            for (const auto& part : *parts)
                resolved.host /= part;
            return resolved;
        }
        return std::nullopt;
    }

    std::optional<std::string> TranslatePath(const std::string& guestPath)
    {
        std::string path = guestPath;
        std::lock_guard lock(g_mutex);
        for (int depth = 0; depth < 8; depth++)
        {
            auto split = SplitDevice(path);
            if (!split)
                return std::nullopt;
            auto& [device, rest] = *split;
            if (auto link = g_links.find(device); link != g_links.end())
            {
                path = link->second + rest;
                continue;
            }
            if (g_devices.find(device) == g_devices.end())
                return std::nullopt;
            return path;
        }
        return std::nullopt;
    }

    std::optional<std::string> LinkTarget(const std::string& link)
    {
        std::lock_guard lock(g_mutex);
        auto it = g_links.find(NameKey(link));
        if (it == g_links.end())
            return std::nullopt;
        return it->second;
    }

    void CreateLink(const std::string& link, const std::string& target)
    {
        std::lock_guard lock(g_mutex);
        const std::string key = NameKey(link);
        AddNtName(key);
        g_links[key] = target.back() == ':' ? target + "\\" : target;
        Log("Symbolic link {} -> {}", link, target);
    }

    bool DeleteLink(const std::string& link)
    {
        std::lock_guard lock(g_mutex);
        return g_links.erase(NameKey(link)) != 0;
    }
}
