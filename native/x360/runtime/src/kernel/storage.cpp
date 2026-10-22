// Storage devices: see storage.h.

#include "kernel/storage.h"

#include "kernel/kernel.h"
#include "kernel/profile_location.h"

#include "log.h"
#include "settings.h"

#include <windows.h>
#include <winioctl.h>

#include <cstring>
#include <format>
#include <fstream>
#include <mutex>
#include <thread>

namespace fs = std::filesystem;

namespace kernel::storage
{
    namespace
    {
        std::mutex g_mutex;
        std::vector<Device> g_devices;
        std::once_flag g_once;
        uint16_t g_nextId = 3; // XAM's counter (0x81A81328) after the hard drive took 1

        // XAM's default device names (0x815F37C0, by kind: 0x8167A488).
        std::u16string DefaultName(uint32_t kind)
        {
            switch (kind)
            {
            case kKindHardDrive: return u"Hard Drive";
            case kKindMemoryUnit: return u"Memory Unit";
            case 3: return u"System";
            case 4: return u"Optical Drive";
            case 8: return u"Transfer Cable";
            case 0xF: return u"Sapphire";
            case kKindUsb: return u"USB Storage";
            default: return u"Storage Device";
            }
        }

        // The volume (root path, "C:\") a path is on.
        fs::path VolumeOf(const fs::path& path)
        {
            wchar_t volume[MAX_PATH] = {};
            if (!GetVolumePathNameW(path.c_str(), volume, MAX_PATH))
                Fatal("Storage: no volume for {} (error {})", path.string(), GetLastError());
            return volume;
        }

        // The volume's serial number as the console's 0x14-byte field (hex, space padded): stable per PC volume.
        std::array<uint8_t, 0x14> Serial(const fs::path& volume)
        {
            std::array<uint8_t, 0x14> serial;
            serial.fill(' ');
            DWORD number = 0;
            if (GetVolumeInformationW(volume.c_str(), nullptr, 0, &number, nullptr, nullptr, nullptr, 0))
            {
                const std::string text = std::format("{:08X}", number);
                std::memcpy(serial.data(), text.data(), text.size());
            }
            return serial;
        }

        // USB when the volume's disk sits on a USB bus (IOCTL_STORAGE_QUERY_PROPERTY: BusType); GetDriveType alone
        // cannot tell, USB hard drives report DRIVE_FIXED .
        bool OnUsbBus(const fs::path& volume)
        {
            const std::wstring device = L"\\\\.\\" + volume.wstring().substr(0, 2);
            const HANDLE h = CreateFileW(device.c_str(), 0, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING,
                0, nullptr);
            if (h == INVALID_HANDLE_VALUE)
                return false;
            STORAGE_PROPERTY_QUERY query{};
            query.PropertyId = StorageDeviceProperty;
            query.QueryType = PropertyStandardQuery;
            alignas(8) uint8_t buffer[1024] = {};
            DWORD bytes = 0;
            const bool ok = DeviceIoControl(h, IOCTL_STORAGE_QUERY_PROPERTY, &query, sizeof(query), buffer,
                sizeof(buffer), &bytes, nullptr) && bytes >= sizeof(STORAGE_DEVICE_DESCRIPTOR);
            CloseHandle(h);
            return ok && reinterpret_cast<const STORAGE_DEVICE_DESCRIPTOR*>(buffer)->BusType == BusTypeUsb;
        }

        // name.txt (UTF-16BE with a BOM, no terminator: a console's stick); empty if none.
        std::u16string NameFile(const fs::path& root)
        {
            std::ifstream f(root / "name.txt", std::ios::binary);
            std::string bytes((std::istreambuf_iterator<char>(f)), {});
            if (bytes.size() < 2 || uint8_t(bytes[0]) != 0xFE || uint8_t(bytes[1]) != 0xFF)
                return {};
            std::u16string name;
            for (size_t i = 2; i + 1 < bytes.size(); i += 2)
                name += char16_t(uint8_t(bytes[i]) << 8 | uint8_t(bytes[i + 1]));
            return name;
        }

        // XAM's next device ID (0x8167B3C8): the counter's value, then it moves on to a number no device has, skipping
        // 0 and 2 and wrapping 499 -> 3.
        uint32_t NextId(const std::vector<Device>& taken)
        {
            const uint32_t id = g_nextId;
            auto used = [&](uint32_t n) {
                return std::any_of(taken.begin(), taken.end(), [&](const Device& d) { return d.id == n; });
            };
            do
            {
                g_nextId = uint16_t(g_nextId + 1);
                if (g_nextId > 499)
                    g_nextId = 3;
            } while (g_nextId == 0 || g_nextId == 2 || used(g_nextId));
            return id;
        }

        // The PC's volumes now (the hard drive first). `previous`: a volume found there with the same serial keeps its ID.
        std::vector<Device> Scan(const std::vector<Device>& previous, bool log)
        {
            std::vector<Device> g_devices;
            // The hard drive: content/ in the data directory.
            Device hdd;
            hdd.id = kHardDriveId;
            hdd.kind = kKindHardDrive;
            hdd.name = DefaultName(kKindHardDrive);
            std::error_code ec;
            fs::create_directories(DataDirectory() / "content", ec);
            hdd.contentRoot = DataDirectory();
            hdd.volume = VolumeOf(DataDirectory() / "content");
            hdd.volumePath = "\\Device\\Harddisk0\\Partition1\\";
            hdd.serial = Serial(hdd.volume);
            g_devices.push_back(hdd);

            uint32_t nextId = 3, usbIndex = 0, diskIndex = 1;
            wchar_t drives[512] = {};
            GetLogicalDriveStringsW(511, drives);
            for (const wchar_t* d = drives; *d; d += wcslen(d) + 1)
            {
                const fs::path volume = d;
                const UINT type = GetDriveTypeW(d);
                if (type != DRIVE_FIXED && type != DRIVE_REMOVABLE)
                {
                    if (log)
                        Log("Storage: {} skipped (drive type {}: optical, network and RAM drives are not storage "
                            "devices)", volume.string(), type);
                    continue;
                }
                if (_wcsicmp(volume.c_str(), hdd.volume.c_str()) == 0)
                    continue; // the hard drive's own volume
                wchar_t label[MAX_PATH] = {};
                if (!GetVolumeInformationW(d, label, MAX_PATH, nullptr, nullptr, nullptr, nullptr, 0))
                {
                    if (log)
                        Log("Storage: {} skipped (no medium: error {})", volume.string(), GetLastError());
                    continue;
                }
                Device dev;
                dev.serial = Serial(volume);
                const auto same = std::find_if(previous.begin(), previous.end(), [&](const Device& p) {
                    return p.id != kHardDriveId && _wcsicmp(p.volume.c_str(), volume.c_str()) == 0 &&
                        p.serial == dev.serial;
                });
                std::vector<Device> taken = previous;
                taken.insert(taken.end(), g_devices.begin(), g_devices.end());
                dev.id = same != previous.end() ? same->id : NextId(taken);
                dev.kind = type == DRIVE_REMOVABLE || OnUsbBus(volume) ? kKindUsb : kKindHardDrive;
                dev.volume = volume;
                // A stick a console formatted keeps its tree at the root; other drives get one hidden folder.
                const DWORD contentAttributes = GetFileAttributesW((volume / "Content").c_str());
                dev.consoleFormatted = fs::exists(volume / "name.txt") && contentAttributes != INVALID_FILE_ATTRIBUTES &&
                    (contentAttributes & FILE_ATTRIBUTE_DIRECTORY) && (contentAttributes & FILE_ATTRIBUTE_HIDDEN);
                dev.contentRoot = dev.consoleFormatted ? volume : volume / "xbox360recomped";
                dev.name = NameFile(dev.contentRoot);
                if (dev.name.empty() && label[0])
                    dev.name.assign(label, label + wcslen(label));
                dev.defaultName = dev.name.empty();
                if (dev.defaultName)
                    dev.name = DefaultName(dev.kind);
                dev.volumePath = dev.kind == kKindUsb ? std::format("\\Device\\Mass{}\\", usbIndex++)
                                                      : std::format("\\Device\\Harddisk{}\\Partition1\\", diskIndex++);
                g_devices.push_back(dev);
            }
            return g_devices;
        }

        void LogDevice(const char* what, const Device& dev)
        {
            Log("Storage: {}device {} {} \"{}\" on {} ({}{})", what, dev.id, dev.kind == kKindUsb ? "USB" : "hard drive",
                std::string(dev.name.begin(), dev.name.end()), dev.volume.string(), dev.contentRoot.string(),
                dev.consoleFormatted ? ", console formatted" : "");
        }

        // Every 2 s: the volumes again; on a change the list is replaced and XN_SYS_STORAGEDEVICESCHANGED goes out:
        // XNID(0, _XNAREA_SYSTEM, 0x000B) = 0x0000000B (XDK xam.h), what the dash's notification switch tests
        // (FUN_92186xxx: case 0xB refreshes its devices; case 0xA is XN_SYS_SIGNINCHANGED). XAM's 0x8000000B /
        // XamSendMessageToLoadedApps(0x8000000F) are its own internal messages, not sent here.
        void Watch()
        {
            DWORD lastMask = GetLogicalDrives();
            for (;;)
            {
                Sleep(2000);
                const DWORD mask = GetLogicalDrives();
                std::vector<Device> before, now;
                {
                    std::lock_guard lock(g_mutex);
                    before = g_devices;
                }
                now = Scan(before, false);
                auto key = [](const Device& d) { return std::pair(d.id, d.volume.wstring()); };
                bool changed = now.size() != before.size();
                for (size_t i = 0; !changed && i < now.size(); i++)
                    changed = key(now[i]) != key(before[i]);
                if (!changed && mask == lastMask)
                    continue;
                lastMask = mask;
                if (!changed)
                    continue;
                for (const Device& d : before)
                    if (std::none_of(now.begin(), now.end(), [&](const Device& n) { return n.id == d.id; }))
                        LogDevice("removed ", d);
                for (const Device& d : now)
                    if (std::none_of(before.begin(), before.end(), [&](const Device& b) { return b.id == d.id; }))
                        LogDevice("added ", d);
                {
                    std::lock_guard lock(g_mutex);
                    g_devices = now;
                }
                kernel::profile_location::ForgetAll();
                kernel::BroadcastNotification(0x0000000B, 0); // XN_SYS_STORAGEDEVICESCHANGED
            }
        }

        void Discover()
        {
            std::vector<Device> found = Scan({}, true);
            for (const Device& dev : found)
                LogDevice("", dev);
            {
                std::lock_guard lock(g_mutex);
                g_devices = std::move(found);
            }
            std::thread(Watch).detach();
        }
    }

    void Initialize()
    {
        std::call_once(g_once, Discover);
    }

    std::vector<Device> Devices()
    {
        Initialize();
        std::lock_guard lock(g_mutex);
        return g_devices;
    }

    std::optional<Device> Find(uint32_t id)
    {
        for (const Device& dev : Devices())
            if (dev.id == id)
                return dev;
        return std::nullopt;
    }

    fs::path ContentTree(const Device& device)
    {
        return device.id == kHardDriveId ? DataDirectory() / "content" : device.contentRoot / "Content";
    }

    std::vector<uint8_t> DeviceData(const Device& device)
    {
        auto put32 = [](std::vector<uint8_t>& v, size_t at, uint32_t x) {
            for (int i = 0; i < 4; i++)
                v[at + i] = uint8_t(x >> (24 - 8 * i));
        };
        auto put64 = [&](std::vector<uint8_t>& v, size_t at, uint64_t x) {
            put32(v, at, uint32_t(x >> 32));
            put32(v, at + 4, uint32_t(x));
        };
        ULARGE_INTEGER freeBytes{}, totalBytes{};
        if (!GetDiskFreeSpaceExW(device.volume.c_str(), &freeBytes, &totalBytes, nullptr))
            Fatal("XDEVICE_DATA: cannot query free space of {} (error {})", device.volume.string(), GetLastError());
        std::vector<uint8_t> data(0x50, 0);
        put32(data, 0x00, device.id);
        put32(data, 0x04, device.kind);
        put64(data, 0x08, totalBytes.QuadPart);
        put64(data, 0x10, freeBytes.QuadPart);
        for (size_t i = 0; i < device.name.size() && i < 26; i++) // WCHAR[27], terminated at +0x4C
        {
            data[0x18 + i * 2] = uint8_t(device.name[i] >> 8);
            data[0x18 + i * 2 + 1] = uint8_t(device.name[i]);
        }
        return data;
    }
}
