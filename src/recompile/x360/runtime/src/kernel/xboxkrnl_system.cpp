// System-level exports: debug output, bug checks, title exit, symbolic links, crypto helpers, execution ID.

#include "kernel/kernel.h"
#include "vfs.h"

#include <windows.h>
#include <bcrypt.h>

#include <cstdlib>

namespace
{
    std::string ReadAnsiString(uint32_t ansiString)
    {
        auto& mem = guest::g_memory;
        if (ansiString == 0)
            return {};
        const uint16_t length = mem.Read16(ansiString);
        const uint32_t buffer = mem.Read32(ansiString + 4);
        return buffer ? std::string(reinterpret_cast<const char*>(mem.Translate(buffer)), length) : std::string();
    }

    [[noreturn]] void ExitTitle(const char* reason)
    {
        Log("Title exit: {}", reason);
        std::fflush(nullptr);
        logging::PauseIfOwnConsole();
        std::_Exit(0);
    }
}

// ULONG DbgPrint(PCSTR Format, ...): debug output. Logged as the format string plus the first integer arguments.
GUEST_IMPORT(DbgPrint)
{
    const std::string format = ctx.r3.u32 ? std::string(reinterpret_cast<const char*>(guest::g_memory.Translate(ctx.r3.u32))) : "";
    Log("[DbgPrint] \"{}\" args {:08X} {:08X} {:08X} {:08X}", format, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32);
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// VOID XamDbgPrint(DWORD Level, PCSTR Format, ...) (xam): XAM's leveled debug output, which the 3519 debug build
// prints through ("*** CNetworkApp::RegisterXuiClasses ***", level 3). Logged as DbgPrint is.
GUEST_IMPORT(XamDbgPrint)
{
    const std::string format = ctx.r4.u32 ? std::string(reinterpret_cast<const char*>(guest::g_memory.Translate(ctx.r4.u32))) : "";
    Log("[XamDbgPrint {}] \"{}\" args {:08X} {:08X} {:08X} {:08X}", ctx.r3.u32, format, ctx.r5.u32, ctx.r6.u32,
        ctx.r7.u32, ctx.r8.u32);
}

// VOID DbgBreakPoint(): with no debugger attached a retail console continues.
GUEST_IMPORT(DbgBreakPoint)
{
    Log("DbgBreakPoint called from {:08X} (ignored: no debugger)", uint32_t(ctx.lr));
}

// VOID KeBugCheck(ULONG BugCheckCode)
GUEST_IMPORT(KeBugCheck)
{
    Fatal("KeBugCheck({:08X}) called from {:08X}", ctx.r3.u32, uint32_t(ctx.lr));
}

// VOID KeBugCheckEx(ULONG BugCheckCode, ULONG_PTR P1, ULONG_PTR P2, ULONG_PTR P3, ULONG_PTR P4)
GUEST_IMPORT(KeBugCheckEx)
{
    Fatal("KeBugCheckEx({:08X}, {:08X}, {:08X}, {:08X}, {:08X}) called from {:08X}",
        ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32, uint32_t(ctx.lr));
}

// VOID HalReturnToFirmware(FIRMWARE_REENTRY Routine): reboot / power off / return to the dashboard.
GUEST_IMPORT(HalReturnToFirmware)
{
    Log("HalReturnToFirmware({})", ctx.r3.u32);
    ExitTitle("HalReturnToFirmware");
}

// VOID XamLoaderTerminateTitle(): the title asked to exit to the dashboard.
GUEST_IMPORT(XamLoaderTerminateTitle)
{
    ExitTitle("XamLoaderTerminateTitle");
}

// NTSTATUS ObCreateSymbolicLink(PANSI_STRING SymbolicLinkName, PANSI_STRING DeviceName)
GUEST_IMPORT(ObCreateSymbolicLink)
{
    vfs::CreateLink(ReadAnsiString(ctx.r3.u32), ReadAnsiString(ctx.r4.u32));
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// NTSTATUS ObDeleteSymbolicLink(PANSI_STRING SymbolicLinkName)
GUEST_IMPORT(ObDeleteSymbolicLink)
{
    const std::string link = ReadAnsiString(ctx.r3.u32);
    ctx.r3.u64 = vfs::DeleteLink(link) ? kernel::kStatusSuccess : 0xC0000034; // STATUS_OBJECT_NAME_NOT_FOUND
}

// VOID XeCryptSha(PBYTE Input1, DWORD Size1, PBYTE Input2, DWORD Size2, PBYTE Input3, DWORD Size3,
//                 PBYTE Digest, DWORD DigestSize): SHA-1 over the concatenated inputs; DigestSize bytes written.
GUEST_IMPORT(XeCryptSha)
{
    auto& mem = guest::g_memory;
    BCRYPT_HASH_HANDLE hash = nullptr;
    if (!BCRYPT_SUCCESS(BCryptCreateHash(BCRYPT_SHA1_ALG_HANDLE, &hash, nullptr, 0, nullptr, 0, 0)))
        Fatal("XeCryptSha: unable to create a SHA-1 hash");
    const uint32_t inputs[3][2] = { { ctx.r3.u32, ctx.r4.u32 }, { ctx.r5.u32, ctx.r6.u32 }, { ctx.r7.u32, ctx.r8.u32 } };
    for (const auto& [address, size] : inputs)
        if (address && size)
            BCryptHashData(hash, mem.Translate(address), size, 0);
    uint8_t digest[20];
    BCryptFinishHash(hash, digest, sizeof(digest), 0);
    BCryptDestroyHash(hash);
    if (ctx.r9.u32)
        memcpy(mem.Translate(ctx.r9.u32), digest, std::min<uint32_t>(ctx.r10.u32, sizeof(digest)));
}

// VOID XeCryptRandom(PBYTE Buffer, DWORD Size)
GUEST_IMPORT(XeCryptRandom)
{
    if (ctx.r3.u32 && ctx.r4.u32)
        BCryptGenRandom(nullptr, guest::g_memory.Translate(ctx.r3.u32), ctx.r4.u32, BCRYPT_USE_SYSTEM_PREFERRED_RNG);
}

// DWORD XamGetExecutionId(PXEX_EXECUTION_ID* ExecutionId): points at the execution info in the title's XEX header.
GUEST_IMPORT(XamGetExecutionId)
{
    constexpr uint32_t kXexHeaderExecutionInfo = 0x00040006;
    const uint32_t info = kernel::XexHeaderField(kernel::ExecutableXexHeader(), kXexHeaderExecutionInfo);
    if (ctx.r3.u32)
        guest::g_memory.Write32(ctx.r3.u32, info);
    ctx.r3.u64 = info ? 0 : 0x80070490; // ERROR_NOT_FOUND as HRESULT
}
