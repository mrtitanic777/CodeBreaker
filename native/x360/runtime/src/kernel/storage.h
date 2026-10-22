#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

// The console's storage devices: the hard drive (content/ in the data directory) and every other PC volume, fixed or
// removable (docs/x360/storage_devices.md "Design"). XAM's rules (XAM 17559, SHOWS): device IDs from a counter
// starting at 1 that skips 0 and 2 (2 is "\Device\NetworkStorageCache"), so the hard drive is 1 and the others 3, 4,
// ... in the order found (0x8167B3C8); the kind (XDEVICE_DATA type) picks the default name (0x815F37C0); state 3 is a
// ready device. Devices are found at start-up and again whenever the PC's volumes change (a 2 s poll): a volume
// that stays keeps its ID; one that arrives gets the counter's next ID; XAM's change notifications are broadcast.
namespace kernel::storage
{
    constexpr uint32_t kHardDriveId = 1;
    // Device kinds (XDEVICE_DATA type, XAM's device table +0x7C).
    constexpr uint32_t kKindHardDrive = 1, kKindMemoryUnit = 2, kKindUsb = 0x10;

    struct Device
    {
        uint32_t id = 0;
        uint32_t kind = kKindHardDrive;
        std::u16string name;               // name.txt, else the volume label, else the kind's default name
        bool defaultName = true;           // the name is the kind's default English name (gets XAM's localized one)
        std::filesystem::path volume;      // the PC volume's root ("E:\")
        std::filesystem::path contentRoot; // where Content\ lives: content/'s parent for the hard drive, a console
                                           // stick's root, else <volume>\xbox360recomped
        std::string volumePath;            // the guest's volume path (XamContentGetDeviceVolumePath), ends with '\'
        bool consoleFormatted = false;     // a stick a console formatted (name.txt + hidden Content\ at its root)
        std::array<uint8_t, 0x14> serial{}; // XamContentGetDeviceSerialNumber's 0x14 bytes
    };

    // Finds the devices (the hard drive always) and starts watching for volumes that come and go.
    void Initialize();
    // A snapshot of the devices (the list changes when a drive is plugged in or removed).
    std::vector<Device> Devices();
    // nullopt: no such device (now).
    std::optional<Device> Find(uint32_t id);
    // The device's content tree (<XUID>\<title>\<type>\<package>): content/ for the hard drive, <content root>\Content
    // on the others.
    std::filesystem::path ContentTree(const Device& device);
    // XDEVICE_DATA (0x50 bytes): ID, type, total and free bytes of its PC volume, the name (WCHAR[27]).
    std::vector<uint8_t> DeviceData(const Device& device);
}
