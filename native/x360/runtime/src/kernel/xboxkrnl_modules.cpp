// xboxkrnl module exports (Xex*).
//
// Module handles (HMODULE) are guest addresses of loader data table entries. Recompiled modules (bootanim.xex,
// dash.xex, a title) get theirs when modules::Load loads them; the native system modules xboxkrnl.exe and xam.xex get
// entries on first use. Exports of a native system module are reached through a module's own import thunks, which
// the recompiler already maps to the runtime's implementations; exports of a recompiled module are its own code.

#include "kernel/kernel.h"
#include "vfs.h"

#include <filesystem>
#include <fstream>
#include <cstring>
#include "kernel/objects.h"
#include "modules.h"
#include "xex.h"

#include <algorithm>
#include <cctype>
#include <map>
#include <mutex>
#include <tuple>

namespace
{
    constexpr uint32_t kXexHeaderSystemFlags = 0x00030000;
    constexpr uint32_t kXexHeaderSystemFlags32 = 0x00030100;
    constexpr uint32_t kXexHeaderResourceInfo = 0x000002FF;

    constexpr uint32_t kStatusNotFound = 0xC0000225;
    constexpr uint32_t kStatusDllNotFound = 0xC0000135;
    constexpr uint32_t kStatusProcedureNotFound = 0xC000007A;

    constexpr uint32_t kLdrEntrySize = 0x80;

    std::mutex g_modulesMutex;
    std::map<std::string, uint32_t> g_systemModules; // lower-case name -> HMODULE

    std::string Lower(std::string s)
    {
        std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return char(std::tolower(c)); });
        return s;
    }

    std::string ReadGuestString(uint32_t address)
    {
        return address ? std::string(reinterpret_cast<const char*>(guest::g_memory.Translate(address))) : std::string();
    }

    // HMODULE of a system module by file name, or 0 if the name is not a system module.
    uint32_t SystemModule(const std::string& name)
    {
        const std::string key = Lower(name);
        if (key != "xam.xex" && key != "xboxkrnl.exe")
            return 0;
        // CB_RECOMP_XAM: the console's own xam.xex is a loaded module with its real exports.
        if (key == "xam.xex")
            if (const modules::LoadedModule* xam = modules::FindByName("xam.xex"))
                return xam->handle;
        std::lock_guard lock(g_modulesMutex);
        auto it = g_systemModules.find(key);
        if (it != g_systemModules.end())
            return it->second;
        const uint32_t entry = guest::g_memory.SystemAlloc(kLdrEntrySize);
        g_systemModules[key] = entry;
        return entry;
    }

    std::string SystemModuleName(uint32_t handle)
    {
        std::lock_guard lock(g_modulesMutex);
        for (const auto& [name, entry] : g_systemModules)
            if (entry == handle)
                return name;
        return {};
    }
}

uint32_t kernel::SystemModuleHandle(const std::string& name)
{
    const uint32_t handle = SystemModule(name);
    if (handle == 0)
        Fatal("SystemModuleHandle(\"{}\"): not a native system module", name);
    return handle;
}

namespace
{
    // A resource section of xam.xex, copied once into guest memory (read-only data XAM's callers parse in place).
    bool XamSection(const std::string& name, uint32_t& address, uint32_t& size)
    {
        static std::mutex mutex;
        static std::map<std::string, std::pair<uint32_t, uint32_t>> copies;
        std::lock_guard lock(mutex);
        if (auto it = copies.find(name); it != copies.end())
        {
            std::tie(address, size) = it->second;
            return true;
        }
        // An older dashboard (CB_DASH) gets XAM's sections of its time (shrdres, gamercrd: kernel/xam_blades.cpp).
        if (const std::filesystem::path older = kernel::OlderDashSection(name); !older.empty())
        {
            std::ifstream in(older, std::ios::binary);
            const std::vector<uint8_t> bytes((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
            auto& mem = guest::g_memory;
            address = mem.virtual4k.Allocate(std::max<uint32_t>(uint32_t(bytes.size()), 1), 0x1000, true,
                guest::kPageReadWrite, true);
            if (address == 0 || bytes.empty())
                Fatal("{}: unable to load it ({} bytes)", older.string(), bytes.size());
            memcpy(mem.Translate(address), bytes.data(), bytes.size());
            size = uint32_t(bytes.size());
            copies[name] = { address, size };
            Log("xam.xex section \"{}\" of dashboard {}: {} ({:X} bytes at {:08X})", name, kernel::OlderDashVersion(),
                older.string(), size, address);
            return true;
        }
        const xex::HostImage& xam = kernel::XamImage();
        auto resource = xam.resources.find(name);
        if (resource == xam.resources.end())
            return false;
        const auto [source, bytes] = resource->second;
        const uint8_t* data = xam.At(source, bytes);
        if (data == nullptr)
            Fatal("xam.xex resource \"{}\" ({:08X}, {:X} bytes) is outside its image", name, source, bytes);
        auto& mem = guest::g_memory;
        address = mem.virtual4k.Allocate(std::max<uint32_t>(bytes, 1), 0x1000, true, guest::kPageReadWrite, true);
        if (address == 0)
            Fatal("xam.xex resource \"{}\": unable to allocate {:X} bytes", name, bytes);
        memcpy(mem.Translate(address), data, bytes);
        size = bytes;
        copies[name] = { address, size };
        Log("xam.xex section \"{}\": {:X} bytes at {:08X}", name, size, address);
        return true;
    }
}

// BOOL XexCheckExecutablePrivilege(DWORD Privilege)
// Privilege is a bit index into the executable's system flags (0-31), continued by the second flags field (32-63).
GUEST_IMPORT(XexCheckExecutablePrivilege)
{
    const uint32_t privilege = ctx.r3.u32;
    const uint32_t header = kernel::ExecutableXexHeader();
    uint32_t result = 0;
    if (privilege < 32)
        result = (kernel::XexHeaderField(header, kXexHeaderSystemFlags) >> privilege) & 1;
    else if (privilege < 64)
        result = (kernel::XexHeaderField(header, kXexHeaderSystemFlags32) >> (privilege - 32)) & 1;
    ctx.r3.u64 = result;
}

// NTSTATUS XexLoadImage(LPCSTR ModuleName, DWORD Flags, DWORD MinimumVersion, PHANDLE ModuleHandle)
GUEST_IMPORT(XexLoadImage)
{
    const std::string name = ReadGuestString(ctx.r3.u32);
    uint32_t handle = SystemModule(name);
    if (handle == 0)
        if (const modules::LoadedModule* module = modules::FindByName(name))
        {
            // Already loaded (recompiled modules are loaded by the runtime): another reference, like the loader's.
            auto& mem = guest::g_memory;
            handle = module->handle;
            mem.Write16(handle + 0x40, uint16_t(mem.Read16(handle + 0x40) + 1)); // LoadCount
        }
    // CB_RECOMP_XAM: a recompiled system module that is not loaded yet (xam.xex loads hud.xex).
    if (handle == 0 && kernel::RecompiledXam())
        if (const modules::LoadedModule* module = kernel::LoadRecompiledSystemModule(name, ctx))
            handle = module->handle;
    if (handle == 0)
    {
        // A module file that is not there (xam.xex's per-console security module, <serial>_secexp.xex, which LIVE
        // delivers): the loader's own answer for a missing file.
        const auto resolved = vfs::Resolve(name);
        std::error_code ec;
        if (!resolved || !std::filesystem::exists(resolved->host, ec))
        {
            Log("XexLoadImage(\"{}\"): no such file", name);
            ctx.r3.u64 = 0xC000000F; // STATUS_NO_SUCH_FILE
            return;
        }
        Fatal("XexLoadImage(\"{}\", flags {:08X}, version {:08X}): not a loaded recompiled module or a system module",
            name, ctx.r4.u32, ctx.r5.u32);
    }
    if (ctx.r6.u32)
        guest::g_memory.Write32(ctx.r6.u32, handle);
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// NTSTATUS XexUnloadImage(HANDLE ModuleHandle): the load count drops; system modules stay loaded; an app the
// recompiled XAM loaded leaves with its last reference (kernel/recompiled_modules.cpp).
GUEST_IMPORT(XexUnloadImage)
{
    const uint32_t handle = ctx.r3.u32;
    if (modules::FindByHandle(handle))
    {
        auto& mem = guest::g_memory;
        const uint16_t count = mem.Read16(handle + 0x40);
        if (count > 0)
            mem.Write16(handle + 0x40, uint16_t(count - 1));
        // CB_RECOMP_XAM: an app XAM loaded (signin.xex, Guide.*.xex ...) goes with its last reference.
        if (count == 1 && kernel::RecompiledXam())
            kernel::UnloadRecompiledSystemModule(handle, ctx);
        ctx.r3.u64 = kernel::kStatusSuccess;
        return;
    }
    ctx.r3.u64 = (handle == kernel::ExecutableModuleHandle() || !SystemModuleName(handle).empty())
        ? kernel::kStatusSuccess : kernel::kStatusInvalidHandle;
}

// NTSTATUS XexGetModuleHandle(LPCSTR ModuleName, PHANDLE ModuleHandle): a null or empty name means the executable.
GUEST_IMPORT(XexGetModuleHandle)
{
    // Name (PSZ)-1: the module that calls (Dash.Search.xex 0x9AD472E8 asks it for its own handle, to load its own
    // resources): the loaded module whose image holds the return address.
    if (ctx.r3.u32 == 0xFFFFFFFF)
    {
        const uint32_t caller = uint32_t(ctx.lr);
        uint32_t handle = 0;
        for (const modules::LoadedModule* m : modules::All())
            if (caller - m->xex.imageBase < m->xex.imageSize)
                handle = m->handle;
        if (handle == 0)
            Fatal("XexGetModuleHandle(-1) from {:08X}: no loaded module holds the caller", caller);
        if (ctx.r4.u32)
            guest::g_memory.Write32(ctx.r4.u32, handle);
        ctx.r3.u64 = kernel::kStatusSuccess;
        return;
    }
    const std::string name = ReadGuestString(ctx.r3.u32);
    uint32_t handle = name.empty() ? kernel::ExecutableModuleHandle() : SystemModule(name);
    if (handle == 0 && !name.empty())
        if (const modules::LoadedModule* module = modules::FindByName(name))
            handle = module->handle;
    if (handle == 0 && Lower(name) == "default.xex")
        handle = kernel::ExecutableModuleHandle();
    if (ctx.r4.u32)
        guest::g_memory.Write32(ctx.r4.u32, handle);
    ctx.r3.u64 = handle ? kernel::kStatusSuccess : kStatusDllNotFound;
}

// NTSTATUS XexGetProcedureAddress(HANDLE ModuleHandle, DWORD Ordinal, PVOID* Address)
// Ordinal values above 0xFFFF are pointers to export names.
GUEST_IMPORT(XexGetProcedureAddress)
{
    const uint32_t handle = ctx.r3.u32;
    const uint32_t ordinal = ctx.r4.u32;
    if (ordinal & 0xFFFF0000)
    {
        // By name: the module's XEX header key 0xE10402 (exports by name) points to {image offset, size} of a PE export
        // directory, little-endian; its table offsets are relative to the directory, its function values to the image
        // base (Dash.Search.lex: "PamInitializeState", the dash's channel-provider entry).
        const std::string wanted = ReadGuestString(ordinal);
        const modules::LoadedModule* module = modules::FindByHandle(handle);
        if (!module || !module->xexHeader)
            Fatal("XexGetProcedureAddress by name (\"{}\") on handle {:08X}: not a loaded recompiled module", wanted,
                handle);
        auto& mem = guest::g_memory;
        const auto le32 = [&](uint32_t a) -> uint32_t { return _byteswap_ulong(mem.Read32(a)); };
        const auto le16 = [&](uint32_t a) -> uint32_t { return _byteswap_ushort(mem.Read16(a)); };
        uint32_t directory = 0;
        const uint32_t count = mem.Read32(module->xexHeader + 0x14);
        for (uint32_t i = 0; i < count; i++)
            if (mem.Read32(module->xexHeader + 0x18 + i * 8) == 0x00E10402)
                directory = module->xex.imageBase +
                            mem.Read32(module->xexHeader + mem.Read32(module->xexHeader + 0x18 + i * 8 + 4));
        uint32_t address = 0;
        if (directory)
        {
            const uint32_t functionCount = le32(directory + 20), names = le32(directory + 24),
                           functions = le32(directory + 28), nameTable = le32(directory + 32),
                           ordinals = le32(directory + 36);
            for (uint32_t i = 0; i < names && !address; i++)
                if (ReadGuestString(directory + le32(directory + nameTable + i * 4)) == wanted)
                {
                    const uint32_t index = le16(directory + ordinals + i * 2);
                    if (index < functionCount)
                        address = module->xex.imageBase + le32(directory + functions + index * 4);
                }
        }
        Log("XexGetProcedureAddress({}, \"{}\") -> {:08X}", module->path, wanted, address);
        if (address != 0 && ctx.r5.u32)
            mem.Write32(ctx.r5.u32, address);
        ctx.r3.u64 = address ? kernel::kStatusSuccess : kStatusProcedureNotFound;
        return;
    }

    if (const modules::LoadedModule* module = modules::FindByHandle(handle))
    {
        const uint32_t address = modules::ExportAddress(*module, uint16_t(ordinal));
        if (address != 0 && ctx.r5.u32)
            guest::g_memory.Write32(ctx.r5.u32, address);
        ctx.r3.u64 = address ? kernel::kStatusSuccess : kStatusProcedureNotFound;
        return;
    }

    std::string library = SystemModuleName(handle);
    if (library.empty())
        Fatal("XexGetProcedureAddress on module handle {:08X} (ordinal {}): only system modules are supported", handle, ordinal);

    uint32_t address = kernel::LookupImportThunk(library, uint16_t(ordinal));
    if (address == 0)
        address = kernel::ResolveVariable(library, uint16_t(ordinal));
    if (address == 0)
        address = kernel::BindDynamicExport(library, uint16_t(ordinal));
    if (address == 0 && library == "xam.xex" && kernel::IsAbsentXamExport(uint16_t(ordinal)))
    {
        // An empty slot of the console's export table: STATUS_DRIVER_ENTRYPOINT_NOT_FOUND with a null address (Xenia's
        // XexGetProcedureAddress, the same for any export the module lacks).
        Log("XexGetProcedureAddress({:08X} {}, ordinal {}): not an export of this XAM", handle, library, ordinal);
        if (ctx.r5.u32)
            guest::g_memory.Write32(ctx.r5.u32, 0);
        ctx.r3.u64 = 0xC0000263;
        return;
    }
    if (address == 0)
        Fatal("XexGetProcedureAddress({}, ordinal {}): no implementation of this export (add a GUEST_EXPORT)",
            library, ordinal);
    if (ctx.r5.u32)
        guest::g_memory.Write32(ctx.r5.u32, address);
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// NTSTATUS XexGetModuleSection(HANDLE ModuleHandle, LPCSTR SectionName, PVOID* Data, PULONG Size)
// Sections are the XEX resource entries (name[8], address, size) in the resource-info header.
GUEST_IMPORT(XexGetModuleSection)
{
    auto& mem = guest::g_memory;
    const uint32_t handle = ctx.r3.u32;
    const std::string name = ReadGuestString(ctx.r4.u32);
    uint32_t header = 0;
    if (const modules::LoadedModule* module = modules::FindByHandle(handle))
        header = module->xexHeader;
    else if (handle == 0 || handle == kernel::ExecutableModuleHandle())
        header = kernel::ExecutableXexHeader();
    else if (SystemModuleName(handle) == "xam.xex")
    {
        // XAM's resources (shrdres, xam, gamercrd ...: XUI packages its callers open through section:// URLs).
        uint32_t address = 0, size = 0;
        const bool found = XamSection(name, address, size);
        if (found && ctx.r5.u32)
            mem.Write32(ctx.r5.u32, address);
        if (found && ctx.r6.u32)
            mem.Write32(ctx.r6.u32, size);
        ctx.r3.u64 = found ? kernel::kStatusSuccess : kStatusNotFound;
        return;
    }
    else
    {
        ctx.r3.u64 = SystemModuleName(handle).empty() ? kernel::kStatusInvalidHandle : kStatusNotFound;
        return;
    }
    const uint32_t info = header ? kernel::XexHeaderField(header, kXexHeaderResourceInfo) : 0;
    if (info == 0)
    {
        ctx.r3.u64 = kStatusNotFound;
        return;
    }
    const uint32_t count = (mem.Read32(info) - 4) / 16;
    for (uint32_t i = 0; i < count; i++)
    {
        const uint32_t entry = info + 4 + i * 16;
        const std::string entryName(reinterpret_cast<const char*>(mem.Translate(entry)), strnlen(reinterpret_cast<const char*>(mem.Translate(entry)), 8));
        if (entryName != name)
            continue;
        if (ctx.r5.u32)
            mem.Write32(ctx.r5.u32, mem.Read32(entry + 8));
        if (ctx.r6.u32)
            mem.Write32(ctx.r6.u32, mem.Read32(entry + 12));
        ctx.r3.u64 = kernel::kStatusSuccess;
        return;
    }
    ctx.r3.u64 = kStatusNotFound;
}

// NTSTATUS XexLoadExecutable(LPCSTR Name, PHANDLE Handle, DWORD TypeInfo, DWORD Version): loads the title's
// executable into the title process (CB_RECOMP_XAM: xam.xex's title loader, 0x8169E680, loads \SystemRoot\dash.xex at
// power-on). The image is a recompiled module (the dashboard is loaded at start-up, before XAM runs); it becomes the
// executable (its TLS, title ID, XexExecutableModuleHandle), as the native start-up does without the console's XAM.
GUEST_IMPORT(XexLoadExecutable)
{
    const std::string name = ReadGuestString(ctx.r3.u32);
    const modules::LoadedModule* module = modules::FindByName(name);
    if (!module)
        module = kernel::ReloadTerminatedExecutable(name);
    if (!module)
        module = kernel::LoadRecompiledSystemModule(name, ctx);
    if (!module)
    {
        Log("XexLoadExecutable(\"{}\"): no such recompiled module", name);
        ctx.r3.u64 = 0xC000000F; // STATUS_NO_SUCH_FILE
        return;
    }
    const xex::Module& x = module->xex;
    kernel::SetExecutable(module->handle, module->xexHeader, x.titleId, x.tls.slotCount, x.tls.dataSize,
        x.tls.rawDataAddress, x.tls.rawDataSize);
    Log("XexLoadExecutable(\"{}\"): title {:08X}, entry {:08X}", name, x.titleId, x.entryPoint);
    if (ctx.r4.u32)
        guest::g_memory.Write32(ctx.r4.u32, module->handle);
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// NTSTATUS XexStartExecutable(PVOID TitleStartRoutine): starts the executable XexLoadExecutable loaded. Its main
// thread (title process, the executable's default stack) runs XAM's title start routine first (xam.xex 0x816A0400:
// the D: / GAME: links, the wait for the boot animation, its notifications; it returns), then the entry point.
GUEST_IMPORT(XexStartExecutable)
{
    const uint32_t handle = kernel::ExecutableModuleHandle();
    const modules::LoadedModule* module = modules::FindByHandle(handle);
    if (!module)
        Fatal("XexStartExecutable: no executable loaded");
    kernel::ThreadParams params;
    params.stackSize = module->xex.defaultStackSize;
    params.startAddress = module->xex.entryPoint;
    params.preStart = ctx.r3.u32;
    params.mainThread = true;
    params.processType = kernel::kProcessTypeTitle;
    kernel::GuestThread* thread = kernel::CreateThread(params);
    kernel::StartThread(thread);
    Log("XexStartExecutable: {} main thread {} (start routine {:08X}, entry {:08X})", module->recompiled->name,
        thread->id, ctx.r3.u32, module->xex.entryPoint);
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// NTSTATUS XexLoadImageHeaders(PANSI_STRING Path, PVOID Buffer, DWORD Size): a module file's XEX header (to its PE
// data) into the buffer, its security info field as the address inside the buffer (as the loader keeps it; xam.xex
// reads a title's headers before loading it, 0x816D2440).
GUEST_IMPORT(XexLoadImageHeaders)
{
    auto& mem = guest::g_memory;
    const uint32_t ansi = ctx.r3.u32, buffer = ctx.r4.u32, size = ctx.r5.u32;
    const std::string path(reinterpret_cast<const char*>(mem.Translate(mem.Read32(ansi + 4))), mem.Read16(ansi));
    const auto resolved = vfs::Resolve(path);
    std::error_code ec;
    if (!resolved || !std::filesystem::is_regular_file(resolved->host, ec))
    {
        ctx.r3.u64 = 0xC000000F; // STATUS_NO_SUCH_FILE
        return;
    }
    std::ifstream in(resolved->host, std::ios::binary);
    uint8_t head[0x18] = {};
    in.read(reinterpret_cast<char*>(head), sizeof(head));
    const uint32_t headerSize = uint32_t(head[8]) << 24 | head[9] << 16 | head[10] << 8 | head[11];
    if (!in || std::memcmp(head, "XEX2", 4) != 0 || headerSize < 0x18)
    {
        ctx.r3.u64 = 0xC000007B; // STATUS_INVALID_IMAGE_FORMAT
        return;
    }
    if (headerSize > size)
    {
        ctx.r3.u64 = 0xC0000023; // STATUS_BUFFER_TOO_SMALL
        return;
    }
    in.seekg(0);
    in.read(reinterpret_cast<char*>(mem.Translate(buffer)), headerSize);
    mem.Write32(buffer + 0x10, buffer + mem.Read32(buffer + 0x10));
    ctx.r3.u64 = kernel::kStatusSuccess;
}
