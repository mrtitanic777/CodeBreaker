// The kernel's STFS file system driver for the console's own XAM (CB_RECOMP_XAM): XAM opens a CON package file (a
// profile, a save) and mounts it as a device (StfsCreateDevice), reads and writes its files under the device's name,
// flushes it (StfsControlDevice), then writes, hashes and signs the package header itself (xam.xex 0x8167C250).
// The device's files are the package's mirror (kernel/stfs_mount.h: extracted into a host folder, empty for a package
// XAM has just created); a flush rebuilds the package's file system from the mirror in place, and the volume descriptor
// XAM reads back afterwards describes it.
//
// STFS_DEVICE_PARAMETERS (0x58 bytes, from xam.xex 0x81686DA0 / 0x816803xx): +0x00 device name (ANSI_STRING),
// +0x08 the header's size rounded to 4 KiB, +0x10 the most the package may grow to, +0x18 the package file's handle,
// +0x1C the header's volume descriptor (0x24 bytes), +0x40 a cache buffer and +0x50 its size in 4 KiB blocks,
// +0x44 the device extension size XAM needs; the driver returns the device object at +0x48 and its extension at
// +0x4C. +0x51 / +0x52 flags, +0x53 the package's signature kind (2 = this console's).

#include "kernel/kernel.h"
#include "kernel/objects.h"
#include "kernel/stfs_mount.h"
#include "vfs.h"

#include <cstring>
#include <fstream>
#include <map>
#include <mutex>
#include <string>

namespace
{
    constexpr uint32_t kStatusPending = 0x00000103;
    constexpr uint32_t kDeviceObjectSize = 0x40; // the extension follows the object

    struct StfsDevice
    {
        std::string name;
        std::filesystem::path package, mirror;
        uint64_t committed = 0; // stfs_mount::TreeSignature of the mirror when last written to the package
        bool mounted = false;
    };

    std::mutex g_mutex;
    std::map<uint32_t, StfsDevice> g_devices; // device object -> device

    std::string ReadAnsiString(uint32_t address)
    {
        auto& mem = guest::g_memory;
        const uint16_t length = mem.Read16(address);
        const uint32_t buffer = mem.Read32(address + 4);
        return std::string(reinterpret_cast<const char*>(mem.Translate(buffer)), length);
    }

    // Rebuilds the package from the mirror when its files changed since the last write. False with a message on a
    // failure.
    bool Flush(StfsDevice& d, std::string* error)
    {
        const uint64_t now = kernel::stfs_mount::TreeSignature(d.mirror);
        if (now == d.committed)
            return true;
        std::vector<uint8_t> header;
        if (!kernel::stfs_mount::CommitInPlace(d.package, d.mirror, header, error))
            return false;
        d.committed = kernel::stfs_mount::TreeSignature(d.mirror);
        return true;
    }

    void Dismount(StfsDevice& d)
    {
        if (!d.mounted)
            return;
        std::string error;
        if (!Flush(d, &error))
            Fatal("STFS device {}: cannot write {}: {}", d.name, d.package.string(), error);
        vfs::UnmountDevice(d.name);
        d.mounted = false;
        Log("STFS device {} dismounted", d.name);
    }
}

// NTSTATUS StfsCreateDevice(PSTFS_DEVICE_PARAMETERS Parameters, ULONG Size)
GUEST_IMPORT(StfsCreateDevice)
{
    auto& mem = guest::g_memory;
    const uint32_t p = ctx.r3.u32;
    if (ctx.r4.u32 != 0x58)
        Fatal("StfsCreateDevice: parameter block of {:#x} bytes (0x58 known)", ctx.r4.u32);
    StfsDevice d;
    d.name = ReadAnsiString(p);
    const uint32_t file = mem.Read32(p + 0x18);
    const auto package = kernel::HostFilePath(file);
    if (!package)
        Fatal("StfsCreateDevice({}): handle {:08X} is not an open package file", d.name, file);
    d.package = *package;
    std::string error;
    const auto mirror = kernel::stfs_mount::DeviceMirror(d.package, &error);
    if (!mirror)
    {
        Log("StfsCreateDevice({}): {}: {}", d.name, d.package.string(), error);
        ctx.r3.u64 = 0xC0000032; // STATUS_DISK_CORRUPT_ERROR
        return;
    }
    d.mirror = *mirror;
    d.committed = kernel::stfs_mount::TreeSignature(d.mirror);
    const uint32_t extensionSize = mem.Read32(p + 0x44);
    if (extensionSize > 0x100000)
        Fatal("StfsCreateDevice({}): device extension of {:#x} bytes", d.name, extensionSize);
    const uint32_t object = kernel::CreateOwnedObject(kDeviceObjectSize + extensionSize);
    std::memset(mem.Translate(object), 0, kDeviceObjectSize + extensionSize);
    kernel::ReferenceObject(object); // one for XAM, one for the device itself (IoDeleteDevice)
    mem.Write32(p + 0x48, object);
    mem.Write32(p + 0x4C, object + kDeviceObjectSize);
    vfs::MountDevice(d.name, d.mirror, false);
    d.mounted = true;
    Log("StfsCreateDevice: {} = {} (files in {}), device object {:08X}, flags {:02X} {:02X} {:02X}", d.name,
        d.package.string(), d.mirror.string(), object, mem.Read8(p + 0x51), mem.Read8(p + 0x52), mem.Read8(p + 0x53));
    kernel::SetObjectCleanup(object, [object] {
        std::lock_guard lock(g_mutex);
        if (auto it = g_devices.find(object); it != g_devices.end())
        {
            Dismount(it->second);
            g_devices.erase(it);
        }
    });
    std::lock_guard lock(g_mutex);
    g_devices[object] = std::move(d);
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// NTSTATUS StfsControlDevice(PDEVICE_OBJECT Device, ULONG Code, PVOID Data), codes from xam.xex's use: 0 / 1 take and
// give back the device for a header update (0x8167C250, 0x81680F14), 2 flushes, 3 copies the volume descriptor out
// (0x24 bytes), 4 hands the one XAM wrote into its header back, 5 asks whether the device has changes not yet
// flushed (STATUS_SUCCESS: none; XAM flushes the others, 0x8167C198).
GUEST_IMPORT(StfsControlDevice)
{
    auto& mem = guest::g_memory;
    std::lock_guard lock(g_mutex);
    auto it = g_devices.find(ctx.r3.u32);
    if (it == g_devices.end())
        Fatal("StfsControlDevice({:08X}, {}): not an STFS device", ctx.r3.u32, ctx.r4.u32);
    StfsDevice& d = it->second;
    std::string error;
    switch (ctx.r4.u32)
    {
    case 0:
    case 1:
    case 4:
        break;
    case 2:
        if (!Flush(d, &error))
            Fatal("STFS device {}: cannot write {}: {}", d.name, d.package.string(), error);
        break;
    case 3:
    {
        const std::vector<uint8_t> header = kernel::stfs_mount::ReadShared(d.package, 0, 0x39D);
        if (header.size() < 0x39D)
            Fatal("STFS device {}: cannot read {}'s header", d.name, d.package.string());
        std::memcpy(mem.Translate(ctx.r5.u32), header.data() + 0x379, 0x24);
        break;
    }
    case 5:
        ctx.r3.u64 = kernel::stfs_mount::TreeSignature(d.mirror) == d.committed ? kernel::kStatusSuccess : kStatusPending;
        return;
    default:
        Fatal("StfsControlDevice({}, code {}): not implemented", d.name, ctx.r4.u32);
    }
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// NTSTATUS IoDismountVolume(PDEVICE_OBJECT Device): the file system on it goes (written back first).
GUEST_IMPORT(IoDismountVolume)
{
    std::lock_guard lock(g_mutex);
    auto it = g_devices.find(ctx.r3.u32);
    if (it == g_devices.end())
        Fatal("IoDismountVolume({:08X}): not a device the runtime made", ctx.r3.u32);
    Dismount(it->second);
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// VOID IoDeleteDevice(PDEVICE_OBJECT Device): the device's own reference (StfsCreateDevice) goes.
GUEST_IMPORT(IoDeleteDevice)
{
    {
        std::lock_guard lock(g_mutex);
        if (!g_devices.count(ctx.r3.u32))
            Fatal("IoDeleteDevice({:08X}): not a device the runtime made", ctx.r3.u32);
    }
    kernel::DereferenceObject(ctx.r3.u32);
}
