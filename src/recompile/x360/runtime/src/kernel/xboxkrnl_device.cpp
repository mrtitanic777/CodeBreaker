// Kernel device objects opened by name (ObReferenceObjectByName with IoDeviceObjectType) and their I/O controls.
//
// The 17559 dashboard polls the HD DVD add-on drive every 500 ms from its "Monitor HDDVD Tray" thread (dash
// 0x92262520 -> 0x92267A90): it references \Device\HdDvdRom by name and sends IOCTL_SCSI_PASS_THROUGH_DIRECT
// (0x4D014) with MODE SENSE(10) page 0x3D. The only status it reads as "no add-on attached" is STATUS_DEVICE_REMOVED
// (it then keeps IsHdDvdConnected = 0); any other failure would make it show an HD DVD drive. The console's kernel keeps
// the device object and fails its I/O that way when nothing is plugged in, which is what this console (no add-on) does.
//
// The DVD drive (\Device\CdRom0) is present and empty, tray closed. With no disc (XamLoaderGetMediaInfo type 0) and
// tray state 0, the dash (0x9225CE20 -> 0x92267B20) sends it I/O control 0x240E4 for the drive's 18-byte SCSI sense
// data and shows "Unrecognized Disc" only for sense key 1/3/6 with certain ASCs (0x92267B48); otherwise "Open Tray".
// An empty drive answers NOT READY, MEDIUM NOT PRESENT - TRAY CLOSED (key 2, ASC 0x3A, ASCQ 0x01; ASCQ 0x02 TRAY OPEN while
// the tray is open or moving).

#include "kernel/kernel.h"

#include <cstring>
#include <map>
#include <set>
#include <mutex>

namespace
{
    constexpr uint32_t kStatusDeviceRemoved = 0xC00002B6;
    constexpr uint32_t kStatusObjectTypeMismatch = 0xC0000024;
    constexpr uint32_t kDeviceObjectSize = 0x40;
    constexpr uint16_t kIoDeviceObjectType = 58; // xboxkrnl.exe ordinal of the variable

    std::string ReadAnsiString(uint32_t ansiString)
    {
        auto& mem = guest::g_memory;
        const uint16_t length = mem.Read16(ansiString);
        const uint32_t buffer = mem.Read32(ansiString + 4);
        return std::string(reinterpret_cast<const char*>(mem.Translate(buffer)), length);
    }

    std::string Lower(std::string s)
    {
        for (char& c : s)
            c = char(std::tolower(static_cast<unsigned char>(c)));
        return s;
    }

    // Devices of hardware this console does not have attached: their objects exist, their I/O fails as removed.
    bool IsRemovedDevice(const std::string& lowerName)
    {
        return lowerName == "\\device\\hddvdrom" || lowerName == "\\device\\hddvdstorage" ||
            lowerName == "\\device\\hddvdplayer";
    }

    // Devices this console model does not have at all: no object by that name (STATUS_OBJECT_NAME_NOT_FOUND), as on a
    // console without them. xam.xex probes the built-in memory units (the 4 GB models' internal flash) at startup.
    bool IsAbsentDevice(const std::string& lowerName)
    {
        return lowerName.starts_with("\\device\\builtinmu");
    }

    bool IsDvdDrive(const std::string& lowerName)
    {
        return lowerName == "\\device\\cdrom0";
    }

    constexpr uint32_t kIoctlDvdSenseData = 0x000240E4;

    std::mutex g_mutex;
    std::map<std::string, uint32_t> g_devices; // lower-case name -> device object (never freed)
    std::map<uint32_t, std::string> g_deviceNames;
}

// NTSTATUS ObReferenceObjectByName(PANSI_STRING ObjectName, ULONG Attributes, POBJECT_TYPE ObjectType,
//                                  PVOID ParseContext, PVOID* Object)
GUEST_IMPORT(ObReferenceObjectByName)
{
    const std::string name = ReadAnsiString(ctx.r3.u32);
    const uint32_t type = ctx.r5.u32;
    const std::string key = Lower(name);
    if (IsAbsentDevice(key))
    {
        Log("ObReferenceObjectByName(\"{}\"): no such device on this console", name);
        ctx.r3.u64 = 0xC0000034; // STATUS_OBJECT_NAME_NOT_FOUND
        return;
    }
    if (!IsRemovedDevice(key) && !IsDvdDrive(key))
        Fatal("ObReferenceObjectByName(\"{}\", type {:08X}): no such object in the runtime's namespace", name, type);
    if (type != 0 && type != kernel::ResolveVariable("xboxkrnl.exe", kIoDeviceObjectType))
    {
        ctx.r3.u64 = kStatusObjectTypeMismatch;
        return;
    }
    uint32_t device;
    {
        std::lock_guard lock(g_mutex);
        auto it = g_devices.find(key);
        if (it == g_devices.end())
        {
            device = guest::g_memory.SystemAlloc(kDeviceObjectSize);
            g_devices[key] = device;
            g_deviceNames[device] = name;
        }
        else
        {
            device = it->second;
        }
    }
    if (ctx.r7.u32)
        guest::g_memory.Write32(ctx.r7.u32, device);
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// NTSTATUS IoSynchronousDeviceIoControlRequest(ULONG IoControlCode, PDEVICE_OBJECT DeviceObject, PVOID InputBuffer,
//     ULONG InputBufferLength, PVOID OutputBuffer, ULONG OutputBufferLength, PULONG ReturnedOutputBufferLength,
//     BOOLEAN InternalDeviceIoControl)
GUEST_IMPORT(IoSynchronousDeviceIoControlRequest)
{
    const uint32_t code = ctx.r3.u32, device = ctx.r4.u32;
    std::string name;
    {
        std::lock_guard lock(g_mutex);
        if (auto it = g_deviceNames.find(device); it != g_deviceNames.end())
            name = it->second;
    }
    if (name.empty())
        Fatal("IoSynchronousDeviceIoControlRequest({:08X}) on unknown device object {:08X}", code, device);
    auto& mem = guest::g_memory;
    if (IsDvdDrive(Lower(name)))
    {
        const uint32_t out = ctx.r7.u32, outSize = ctx.r8.u32;
        // The media check (no buffers; xam.xex 0x816AD780 / 0x816AE600): an empty drive answers
        // STATUS_NO_MEDIA_IN_DEVICE, which XAM takes as "no disc".
        if (code == 0x00024800 && ctx.r6.u32 == 0 && out == 0)
        {
            ctx.r3.u64 = 0xC0000013;
            return;
        }
        // A drive setting with no answer (xam.xex: 0x24084 / 0x240C4, 4 bytes, at 0x816AE140 / 0x816AD908; 0x240D0,
        // 0x28 bytes, at 0x816ADCF0): taken by the empty drive.
        if (ctx.r6.u32 >= 4 && out == 0)
        {
            static std::set<uint32_t> logged;
            if (logged.insert(code).second)
                Log("IoSynchronousDeviceIoControlRequest({:08X}) on {}: setting {:08X} taken", code, name,
                    mem.Read32(ctx.r5.u32));
            ctx.r3.u64 = kernel::kStatusSuccess;
            return;
        }
        if (code != kIoctlDvdSenseData || out == 0 || outSize < 0x12)
            Fatal("IoSynchronousDeviceIoControlRequest({:08X}) on {} (output {:08X}, {} bytes) is not implemented", code,
                name, out, outSize);
        uint8_t sense[0x12] = {};
        sense[0] = 0x70;  // current error, fixed format
        sense[2] = 0x02;  // NOT READY
        sense[7] = 0x0A;  // additional sense length
        sense[12] = 0x3A; // MEDIUM NOT PRESENT
        sense[13] = kernel::DvdTrayState() == 0 ? 0x01 : 0x02; // - TRAY CLOSED / - TRAY OPEN
        std::memcpy(mem.Translate(out), sense, sizeof(sense));
        if (ctx.r9.u32)
            mem.Write32(ctx.r9.u32, sizeof(sense));
        ctx.r3.u64 = kernel::kStatusSuccess;
        return;
    }
    if (!IsRemovedDevice(Lower(name)))
        Fatal("IoSynchronousDeviceIoControlRequest({:08X}) on {} is not implemented", code, name);
    if (ctx.r9.u32)
        mem.Write32(ctx.r9.u32, 0);
    ctx.r3.u64 = kStatusDeviceRemoved;
}
