// xboxkrnl file I/O exports (Nt*File). Paths are resolved by vfs; files are guest objects behind handles with host
// state (Win32 handle, current position). The host I/O is done during the call; an Event argument is signaled on
// completion. On a handle opened for asynchronous I/O (no FILE_SYNCHRONOUS_IO_* option, i.e. XAPI's
// FILE_FLAG_OVERLAPPED) a successful read or write still returns STATUS_PENDING, as the console's file system does
// for such handles: the request is already complete when the caller looks at its IO_STATUS_BLOCK. TU46 depends on
// this (its save-list loader only advances when ReadFile reports ERROR_IO_PENDING or ERROR_HANDLE_EOF).
// Information classes not seen yet stop with their number so they get implemented from real requests.

#include "kernel/objects.h"
#include "vfs.h"
#include "kernel/storage.h"

#include <windows.h>

#include <algorithm>
#include <array>
#include <cstring>
#include <map>
#include <mutex>
#include <vector>

using namespace kernel;

namespace
{
    constexpr uint32_t kStatusEndOfFile = 0xC0000011;
    constexpr uint32_t kStatusAccessDenied = 0xC0000022;
    constexpr uint32_t kStatusObjectNameNotFound = 0xC0000034;
    constexpr uint32_t kStatusObjectNameCollision = 0xC0000035;
    constexpr uint32_t kStatusObjectPathNotFound = 0xC000003A;
    constexpr uint32_t kStatusFileIsADirectory = 0xC00000BA;
    constexpr uint32_t kStatusNotADirectory = 0xC0000103;
    constexpr uint32_t kStatusNoSuchFile = 0xC000000F;
    constexpr uint32_t kStatusNoMoreFiles = 0x80000006;
    constexpr uint32_t kStatusInfoLengthMismatch = 0xC0000004;

    // CreateDisposition
    constexpr uint32_t kFileSupersede = 0, kFileOpen = 1, kFileCreate = 2, kFileOpenIf = 3, kFileOverwrite = 4, kFileOverwriteIf = 5;
    // CreateOptions
    constexpr uint32_t kFileDirectoryFile = 0x01;
    constexpr uint32_t kFileNonDirectoryFile = 0x40;
    constexpr uint32_t kFileSynchronousIo = 0x10 /* ALERT */ | 0x20 /* NONALERT */;
    constexpr uint32_t kStatusPending = 0x00000103;
    // IO_STATUS_BLOCK.Information for create
    constexpr uint32_t kFileSuperseded = 0, kFileOpened = 1, kFileCreated = 2, kFileOverwritten = 3;
    // Access bits that need a writable device
    constexpr uint32_t kWriteAccess = 0x40000000 /* GENERIC_WRITE */ | 0x2 /* FILE_WRITE_DATA */ |
        0x4 /* FILE_APPEND_DATA */ | 0x10000 /* DELETE */;

    constexpr uint32_t kAttributeDirectory = 0x10;
    constexpr uint32_t kAttributeNormal = 0x80;

    constexpr uint32_t kFileObjectSize = 0x40;

    struct FileState
    {
        HANDLE host = INVALID_HANDLE_VALUE;
        std::filesystem::path path;
        bool directory = false;
        bool readOnly = true;
        uint64_t position = 0;
        uint32_t access = 0;     // ACCESS_MASK the handle was opened with
        bool synchronous = true; // FILE_SYNCHRONOUS_IO_*: otherwise reads/writes report STATUS_PENDING
        // Directory enumeration (NtQueryDirectoryFile): the mask from the first query and the next entry.
        bool enumerating = false;
        std::string mask;
        std::vector<std::filesystem::path> entries;
        size_t nextEntry = 0;
    };

    std::mutex g_filesMutex;
    std::map<uint32_t, FileState> g_files; // file object -> state

    FileState* LookupFile(uint32_t handle)
    {
        const uint32_t object = LookupHandle(handle);
        std::lock_guard lock(g_filesMutex);
        auto it = g_files.find(object);
        return it == g_files.end() ? nullptr : &it->second;
    }

    void WriteIoStatus(uint32_t iosb, uint32_t status, uint32_t information)
    {
        if (iosb)
        {
            guest::g_memory.Write32(iosb + 0, status);
            guest::g_memory.Write32(iosb + 4, information);
        }
    }

    // A completed request's PIO_APC_ROUTINE: ApcRoutine(ApcContext, IoStatusBlock, 0), queued to the calling thread
    // as a user APC (it runs at that thread's next alertable wait). The routine's low bit is a flag, not part of its
    // address (XAPI's ReadFileEx passes its completion thunk with it set); a request that failed, or one without a
    // context, queues nothing.
    void QueueCompletionRoutine(uint32_t routine, uint32_t context, uint32_t iosb, uint32_t status)
    {
        routine &= ~1u;
        if (routine != 0 && context != 0 && status == kStatusSuccess)
            QueueUserApc(CurrentThread(), { routine, context, iosb, 0 });
    }

    void SignalEvent(uint32_t eventHandle)
    {
        if (eventHandle == 0)
            return;
        const uint32_t event = LookupHandle(eventHandle);
        if (event == 0)
            Fatal("File I/O names invalid event handle {:08X}", eventHandle);
        auto lock = LockDispatcher();
        guest::g_memory.Write32(event + header::kSignalState, 1);
        NotifyDispatcherChange(event);
    }

    uint64_t ToU64(FILETIME ft) { return uint64_t(ft.dwHighDateTime) << 32 | ft.dwLowDateTime; }

    struct HostAttributes
    {
        bool exists = false;
        bool directory = false;
        uint64_t size = 0;
        uint64_t creation = 0, access = 0, write = 0;
    };

    HostAttributes Stat(const std::filesystem::path& path)
    {
        HostAttributes a;
        WIN32_FILE_ATTRIBUTE_DATA data;
        if (!GetFileAttributesExW(path.c_str(), GetFileExInfoStandard, &data))
            return a;
        a.exists = true;
        a.directory = (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
        a.size = uint64_t(data.nFileSizeHigh) << 32 | data.nFileSizeLow;
        a.creation = ToU64(data.ftCreationTime);
        a.access = ToU64(data.ftLastAccessTime);
        a.write = ToU64(data.ftLastWriteTime);
        return a;
    }

    uint32_t GuestAttributes(const HostAttributes& a)
    {
        return a.directory ? kAttributeDirectory : kAttributeNormal;
    }

    // FILE_NETWORK_OPEN_INFORMATION: CreationTime, LastAccessTime, LastWriteTime, ChangeTime, AllocationSize,
    // EndOfFile (u64 each), FileAttributes (u32), pad. 0x38 bytes.
    void WriteNetworkOpenInfo(uint32_t info, const HostAttributes& a)
    {
        auto& mem = guest::g_memory;
        mem.Write64(info + 0x00, a.creation);
        mem.Write64(info + 0x08, a.access);
        mem.Write64(info + 0x10, a.write);
        mem.Write64(info + 0x18, a.write);
        mem.Write64(info + 0x20, a.directory ? 0 : a.size);
        mem.Write64(info + 0x28, a.directory ? 0 : a.size);
        mem.Write32(info + 0x30, GuestAttributes(a));
        mem.Write32(info + 0x34, 0);
    }

    // OBJECT_ATTRIBUTES (RootDirectory +0, ObjectName +4, Attributes +8): the name of a file object. RootDirectory 0
    // or the ObDosDevices pseudo-handle (0xFFFFFFFD) means the name is looked up from the DOS-devices namespace, where
    // "game:", "UPDATE:" etc. live; names relative to an open directory handle are not supported yet.
    constexpr uint32_t kObDosDevicesHandle = 0xFFFFFFFD;

    std::string FileObjectName(uint32_t objectAttributes)
    {
        const std::string name = ReadObjectName(objectAttributes);
        const uint32_t root = guest::g_memory.Read32(objectAttributes);
        if (root != 0 && root != kObDosDevicesHandle)
            Fatal("Nt*File \"{}\" relative to directory handle {:08X} is not implemented", name, root);
        return name;
    }

    // Shared by NtCreateFile and NtOpenFile.
    uint32_t CreateFile(uint32_t handlePtr, uint32_t access, uint32_t objectAttributes, uint32_t iosb,
        uint32_t disposition, uint32_t options)
    {
        auto& mem = guest::g_memory;
        const std::string guestPath = FileObjectName(objectAttributes);
        // Kernel devices that are not file systems: \Device\DeviceAuth (the console's device authentication, which
        // xam.xex opens at start-up and bug-checks without, 0x816E7FE8). A file object with no host file behind it;
        // its device controls are NtDeviceIoControlFile's.
        if (_stricmp(guestPath.c_str(), "\\Device\\DeviceAuth") == 0)
        {
            const uint32_t object = CreateOwnedObject(kFileObjectSize);
            InitializeDispatcherHeader(object, kObjectEventNotification, 1);
            {
                std::lock_guard lock(g_filesMutex);
                g_files[object] = { INVALID_HANDLE_VALUE, std::filesystem::path(guestPath), false, false, 0, access, true };
            }
            SetObjectCleanup(object, [object] {
                std::lock_guard lock(g_filesMutex);
                g_files.erase(object);
            });
            mem.Write32(handlePtr, CreateHandle(object));
            WriteIoStatus(iosb, kStatusSuccess, kFileOpened);
            return kStatusSuccess;
        }
        const auto resolved = vfs::Resolve(guestPath);
        if (!resolved)
        {
            Log("File open \"{}\": no such device", guestPath);
            WriteIoStatus(iosb, kStatusObjectPathNotFound, 0);
            return kStatusObjectPathNotFound;
        }

        const HostAttributes existing = Stat(resolved->host);
        const bool wantsWrite = (access & kWriteAccess) != 0 || disposition != kFileOpen;
        uint32_t status = kStatusSuccess, information = kFileOpened;
        if (existing.exists)
        {
            if (disposition == kFileCreate)
                status = kStatusObjectNameCollision;
            else if ((options & kFileDirectoryFile) && !existing.directory)
                status = kStatusNotADirectory;
            else if ((options & kFileNonDirectoryFile) && existing.directory)
                status = kStatusFileIsADirectory;
            else if (resolved->readOnly && wantsWrite && disposition != kFileOpenIf)
                status = kStatusAccessDenied;
            else if (disposition == kFileOverwrite || disposition == kFileOverwriteIf)
                information = kFileOverwritten;
            else if (disposition == kFileSupersede)
                information = kFileSuperseded;
        }
        else
        {
            if (disposition == kFileOpen || disposition == kFileOverwrite)
                status = kStatusObjectNameNotFound;
            else if (resolved->readOnly)
                status = kStatusAccessDenied;
            else
                information = kFileCreated;
        }
        if (status == kStatusSuccess && resolved->readOnly && (access & kWriteAccess))
            status = kStatusAccessDenied;
        Log("File open \"{}\" -> {} : {:08X}", guestPath, resolved->host.string(), status);
        if (status != kStatusSuccess)
        {
            WriteIoStatus(iosb, status, 0);
            return status;
        }

        const bool directory = existing.exists ? existing.directory : (options & kFileDirectoryFile) != 0;
        if (!existing.exists && directory)
            std::filesystem::create_directories(resolved->host);
        DWORD hostDisposition = OPEN_EXISTING;
        if (!directory)
        {
            switch (disposition)
            {
            case kFileSupersede: case kFileOverwriteIf: hostDisposition = CREATE_ALWAYS; break;
            case kFileCreate: hostDisposition = CREATE_NEW; break;
            case kFileOpenIf: hostDisposition = OPEN_ALWAYS; break;
            case kFileOverwrite: hostDisposition = TRUNCATE_EXISTING; break;
            default: hostDisposition = OPEN_EXISTING; break;
            }
        }
        // A directory handle reads (lists, queries); write / delete rights on it fail with a sharing violation when
        // the folder is in use, e.g. the data directory that is the process's current directory (xam.xex opens the
        // hard drive's root, \Device\Harddisk0\Partition1\).
        const DWORD hostAccess = resolved->readOnly || directory ? GENERIC_READ : (GENERIC_READ | GENERIC_WRITE | DELETE);
        HANDLE host = CreateFileW(resolved->host.c_str(), hostAccess, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            nullptr, hostDisposition, directory ? FILE_FLAG_BACKUP_SEMANTICS : FILE_ATTRIBUTE_NORMAL, nullptr);
        if (host == INVALID_HANDLE_VALUE)
            Fatal("Host open of {} failed (error {})", resolved->host.string(), GetLastError());

        // File objects are waitable like NT file objects: signaled when no I/O is outstanding.
        const uint32_t object = CreateOwnedObject(kFileObjectSize);
        InitializeDispatcherHeader(object, kObjectEventNotification, 1);
        {
            std::lock_guard lock(g_filesMutex);
            g_files[object] = { host, resolved->host, directory, resolved->readOnly, 0, access,
                (options & kFileSynchronousIo) != 0 };
        }
        SetObjectCleanup(object, [object]
            {
                std::lock_guard lock(g_filesMutex);
                auto it = g_files.find(object);
                if (it != g_files.end())
                {
                    CloseHandle(it->second.host);
                    g_files.erase(it);
                }
            });
        mem.Write32(handlePtr, CreateHandle(object));
        WriteIoStatus(iosb, kStatusSuccess, information);
        return kStatusSuccess;
    }
}

// NTSTATUS NtCreateFile(PHANDLE FileHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes,
//                       PIO_STATUS_BLOCK IoStatusBlock, PLARGE_INTEGER AllocationSize, ULONG FileAttributes,
//                       ULONG ShareAccess, ULONG CreateDisposition, ULONG CreateOptions)
GUEST_IMPORT(NtCreateFile)
{
    ctx.r3.u64 = CreateFile(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r10.u32, StackArg32(ctx, 8));
}

// NTSTATUS NtOpenFile(PHANDLE FileHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes,
//                     PIO_STATUS_BLOCK IoStatusBlock, ULONG ShareAccess, ULONG OpenOptions)
GUEST_IMPORT(NtOpenFile)
{
    ctx.r3.u64 = CreateFile(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, kFileOpen, ctx.r8.u32);
}

// NTSTATUS NtReadFile(HANDLE FileHandle, HANDLE Event, PIO_APC_ROUTINE ApcRoutine, PVOID ApcContext,
//                     PIO_STATUS_BLOCK IoStatusBlock, PVOID Buffer, ULONG Length, PLARGE_INTEGER ByteOffset)
// ByteOffset null or FILE_USE_FILE_POINTER_POSITION (-2) means the current position.
GUEST_IMPORT(NtReadFile)
{
    auto& mem = guest::g_memory;
    FileState* file = LookupFile(ctx.r3.u32);
    const uint32_t iosb = ctx.r7.u32;
    if (file == nullptr)
    {
        ctx.r3.u64 = kStatusInvalidHandle;
        return;
    }
    uint64_t offset = file->position;
    if (ctx.r10.u32)
    {
        const int64_t requested = int64_t(mem.Read64(ctx.r10.u32));
        if (requested != -2)
            offset = uint64_t(requested);
    }
    OVERLAPPED ov{};
    ov.Offset = DWORD(offset);
    ov.OffsetHigh = DWORD(offset >> 32);
    DWORD read = 0;
    if (!ReadFile(file->host, mem.Translate(ctx.r8.u32), ctx.r9.u32, &read, &ov) && GetLastError() != ERROR_HANDLE_EOF)
    {
        // Which pages of the buffer the host could not write (998: a page not committed or not writable).
        const DWORD error = GetLastError();
        std::string pages;
        for (uint32_t a = ctx.r8.u32 & ~0xFFFu; a < ctx.r8.u32 + ctx.r9.u32 && pages.size() < 400; )
        {
            MEMORY_BASIC_INFORMATION info{};
            VirtualQuery(mem.Translate(a), &info, sizeof(info));
            const uint32_t end = a + uint32_t(std::min<SIZE_T>(info.RegionSize, 0x7FFFFFFF));
            pages += std::format(" {:08X}-{:08X} state {:X} protect {:X};", a, end, info.State, info.Protect);
            a = end > a ? end : a + 0x1000;
        }
        Fatal("Host read of {} failed (error {}): {} bytes at {:08X} (offset {});{}", file->path.string(), error,
              ctx.r9.u32, ctx.r8.u32, offset, pages);
    }
    file->position = offset + read;
    // Reading at or beyond the end fails at once (also on an asynchronous handle; TU46 relies on ERROR_HANDLE_EOF for
    // an empty file read through an OVERLAPPED).
    const uint32_t status = (read == 0 && ctx.r9.u32 != 0) ? kStatusEndOfFile : kStatusSuccess;
    WriteIoStatus(iosb, status, read);
    SignalEvent(ctx.r4.u32);
    QueueCompletionRoutine(ctx.r5.u32, ctx.r6.u32, iosb, status);
    ctx.r3.u64 = status == kStatusSuccess && !file->synchronous ? kStatusPending : status;
}

// NTSTATUS NtWriteFile(HANDLE FileHandle, HANDLE Event, PIO_APC_ROUTINE ApcRoutine, PVOID ApcContext,
//                      PIO_STATUS_BLOCK IoStatusBlock, PVOID Buffer, ULONG Length, PLARGE_INTEGER ByteOffset)
// ByteOffset null or FILE_USE_FILE_POINTER_POSITION (-2) writes at the current position, FILE_WRITE_TO_END_OF_FILE
// (-1) appends. A handle opened without write access gets STATUS_ACCESS_DENIED.
GUEST_IMPORT(NtWriteFile)
{
    auto& mem = guest::g_memory;
    FileState* file = LookupFile(ctx.r3.u32);
    const uint32_t iosb = ctx.r7.u32;
    if (file == nullptr)
    {
        ctx.r3.u64 = kStatusInvalidHandle;
        return;
    }
    if (file->directory)
        Fatal("NtWriteFile on a directory ({})", file->path.string());
    if (file->readOnly || !(file->access & kWriteAccess))
    {
        WriteIoStatus(iosb, kStatusAccessDenied, 0);
        ctx.r3.u64 = kStatusAccessDenied;
        return;
    }
    uint64_t offset = file->position;
    if (ctx.r10.u32)
    {
        const int64_t requested = int64_t(mem.Read64(ctx.r10.u32));
        if (requested == -1)
            offset = Stat(file->path).size;
        else if (requested != -2)
            offset = uint64_t(requested);
    }
    OVERLAPPED ov{};
    ov.Offset = DWORD(offset);
    ov.OffsetHigh = DWORD(offset >> 32);
    DWORD written = 0;
    if (!WriteFile(file->host, mem.Translate(ctx.r8.u32), ctx.r9.u32, &written, &ov) || written != ctx.r9.u32)
        Fatal("Host write of {} bytes to {} failed (error {})", ctx.r9.u32, file->path.string(), GetLastError());
    file->position = offset + written;
    WriteIoStatus(iosb, kStatusSuccess, written);
    SignalEvent(ctx.r4.u32);
    QueueCompletionRoutine(ctx.r5.u32, ctx.r6.u32, iosb, kStatusSuccess);
    ctx.r3.u64 = file->synchronous ? kStatusSuccess : kStatusPending;
}

// NTSTATUS NtQueryInformationFile(HANDLE FileHandle, PIO_STATUS_BLOCK IoStatusBlock, PVOID FileInformation,
//                                 ULONG Length, FILE_INFORMATION_CLASS FileInformationClass)
GUEST_IMPORT(NtQueryInformationFile)
{
    auto& mem = guest::g_memory;
    FileState* file = LookupFile(ctx.r3.u32);
    const uint32_t iosb = ctx.r4.u32, info = ctx.r5.u32, cls = ctx.r7.u32;
    if (file == nullptr)
    {
        ctx.r3.u64 = kStatusInvalidHandle;
        return;
    }
    const HostAttributes a = Stat(file->path);
    uint32_t written = 0;
    switch (cls)
    {
    case 5: // FileStandardInformation: AllocationSize, EndOfFile (u64), NumberOfLinks (u32), DeletePending, Directory
        mem.Write64(info + 0x00, a.directory ? 0 : a.size);
        mem.Write64(info + 0x08, a.directory ? 0 : a.size);
        mem.Write32(info + 0x10, 1);
        mem.Write8(info + 0x14, 0);
        mem.Write8(info + 0x15, a.directory ? 1 : 0);
        written = 0x18;
        break;
    case 14: // FilePositionInformation: CurrentByteOffset
        mem.Write64(info, file->position);
        written = 8;
        break;
    case 34: // FileNetworkOpenInformation
        WriteNetworkOpenInfo(info, a);
        written = 0x38;
        break;
    default:
        Fatal("NtQueryInformationFile class {} is not implemented ({})", cls, file->path.string());
    }
    WriteIoStatus(iosb, kStatusSuccess, written);
    ctx.r3.u64 = kStatusSuccess;
}

// NTSTATUS NtSetInformationFile(HANDLE FileHandle, PIO_STATUS_BLOCK IoStatusBlock, PVOID FileInformation,
//                               ULONG Length, FILE_INFORMATION_CLASS FileInformationClass)
GUEST_IMPORT(NtSetInformationFile)
{
    auto& mem = guest::g_memory;
    FileState* file = LookupFile(ctx.r3.u32);
    const uint32_t iosb = ctx.r4.u32, info = ctx.r5.u32, cls = ctx.r7.u32;
    if (file == nullptr)
    {
        ctx.r3.u64 = kStatusInvalidHandle;
        return;
    }
    auto setHost = [&](FILE_INFO_BY_HANDLE_CLASS hostClass, void* data, DWORD size) {
        if (file->readOnly || file->directory)
            Fatal("NtSetInformationFile class {} on a read-only file or directory ({})", cls, file->path.string());
        if (!SetFileInformationByHandle(file->host, hostClass, data, size))
            Fatal("NtSetInformationFile class {}: host call failed for {} (error {})", cls, file->path.string(),
                GetLastError());
    };
    switch (cls)
    {
    case 4: // FileBasicInformation: CreationTime, LastAccessTime, LastWriteTime, ChangeTime (u64), FileAttributes
    {
        // A time of 0 leaves it unchanged (NT); attribute 0 leaves the attributes unchanged.
        FILE_BASIC_INFO basic{};
        basic.CreationTime.QuadPart = int64_t(mem.Read64(info + 0x00));
        basic.LastAccessTime.QuadPart = int64_t(mem.Read64(info + 0x08));
        basic.LastWriteTime.QuadPart = int64_t(mem.Read64(info + 0x10));
        basic.ChangeTime.QuadPart = int64_t(mem.Read64(info + 0x18));
        const uint32_t attributes = mem.Read32(info + 0x20);
        if (attributes & ~uint32_t(0x01 | 0x02 | 0x04 | 0x20 | 0x80))
            Fatal("NtSetInformationFile: file attributes {:08X} not implemented ({})", attributes, file->path.string());
        basic.FileAttributes = attributes;
        if (!file->directory)
            setHost(FileBasicInfo, &basic, sizeof(basic));
        break;
    }
    case 13: // FileDispositionInformation: BOOLEAN DeleteFile (the file goes when its last handle closes)
    {
        FILE_DISPOSITION_INFO disposition{ mem.Read8(info) != 0 };
        if (file->readOnly)
        {
            WriteIoStatus(iosb, kStatusAccessDenied, 0);
            ctx.r3.u64 = kStatusAccessDenied;
            return;
        }
        if (!SetFileInformationByHandle(file->host, FileDispositionInfo, &disposition, sizeof(disposition)))
            Fatal("NtSetInformationFile: delete disposition failed for {} (error {})", file->path.string(), GetLastError());
        break;
    }
    case 14: // FilePositionInformation
        file->position = mem.Read64(info);
        break;
    case 19: // FileAllocationInformation: AllocationSize (u64); a size below the end of file truncates
    {
        FILE_ALLOCATION_INFO allocation{};
        allocation.AllocationSize.QuadPart = int64_t(mem.Read64(info));
        setHost(FileAllocationInfo, &allocation, sizeof(allocation));
        break;
    }
    case 20: // FileEndOfFileInformation: EndOfFile (u64)
    {
        FILE_END_OF_FILE_INFO end{};
        end.EndOfFile.QuadPart = int64_t(mem.Read64(info));
        setHost(FileEndOfFileInfo, &end, sizeof(end));
        break;
    }
    default:
        Fatal("NtSetInformationFile class {} is not implemented ({})", cls, file->path.string());
    }
    WriteIoStatus(iosb, kStatusSuccess, 0);
    ctx.r3.u64 = kStatusSuccess;
}

// NTSTATUS NtQueryFullAttributesFile(POBJECT_ATTRIBUTES ObjectAttributes, PFILE_NETWORK_OPEN_INFORMATION Information)
GUEST_IMPORT(NtQueryFullAttributesFile)
{
    const std::string guestPath = FileObjectName(ctx.r3.u32);
    const auto resolved = vfs::Resolve(guestPath);
    const HostAttributes a = resolved ? Stat(resolved->host) : HostAttributes{};
    Log("File attributes \"{}\" -> {}", guestPath, a.exists ? "found" : "not found");
    if (!resolved)
    {
        ctx.r3.u64 = kStatusObjectPathNotFound;
        return;
    }
    if (!a.exists)
    {
        ctx.r3.u64 = kStatusObjectNameNotFound;
        return;
    }
    WriteNetworkOpenInfo(ctx.r4.u32, a);
    ctx.r3.u64 = kStatusSuccess;
}

// NTSTATUS NtFlushBuffersFile(HANDLE FileHandle, PIO_STATUS_BLOCK IoStatusBlock)
GUEST_IMPORT(NtFlushBuffersFile)
{
    FileState* file = LookupFile(ctx.r3.u32);
    if (file == nullptr)
    {
        ctx.r3.u64 = kStatusInvalidHandle;
        return;
    }
    if (!file->readOnly && !file->directory)
        FlushFileBuffers(file->host);
    WriteIoStatus(ctx.r4.u32, kStatusSuccess, 0);
    ctx.r3.u64 = kStatusSuccess;
}

namespace
{
    // Case-insensitive match of a name against a mask with '*' (any run) and '?' (one character).
    bool MatchesMask(const std::string& name, const std::string& mask)
    {
        auto lower = [](char c) { return char(c >= 'A' && c <= 'Z' ? c + 32 : c); };
        size_t n = 0, m = 0, starName = std::string::npos, starMask = std::string::npos;
        while (n < name.size())
        {
            if (m < mask.size() && (mask[m] == '?' || lower(mask[m]) == lower(name[n])))
            {
                n++;
                m++;
            }
            else if (m < mask.size() && mask[m] == '*')
            {
                starMask = m++;
                starName = n;
            }
            else if (starMask != std::string::npos)
            {
                m = starMask + 1;
                n = ++starName;
            }
            else
                return false;
        }
        while (m < mask.size() && mask[m] == '*')
            m++;
        return m == mask.size();
    }
}

// NTSTATUS NtQueryDirectoryFile(HANDLE FileHandle, HANDLE Event, PIO_APC_ROUTINE ApcRoutine, PVOID ApcContext,
//     PIO_STATUS_BLOCK IoStatusBlock, PFILE_DIRECTORY_INFORMATION FileInformation, ULONG Length,
//     POBJECT_STRING FileName, BOOLEAN RestartScan)
// One entry per call (as the title's RtlFindFirstFile/RtlFindNextFile expect). FILE_DIRECTORY_INFORMATION:
// NextEntryOffset +0, FileIndex +4, CreationTime +8, LastAccessTime +0x10, LastWriteTime +0x18, ChangeTime +0x20,
// EndOfFile +0x28, AllocationSize +0x30, FileAttributes +0x38, FileNameLength +0x3C, FileName (ANSI) +0x40.
// The mask is taken from the first query of a handle; "." and ".." are not returned (FATX has no such entries).
// Deviation: entries come in case-insensitive name order; the console returns them in on-disk order.
GUEST_IMPORT(NtQueryDirectoryFile)
{
    auto& mem = guest::g_memory;
    FileState* file = LookupFile(ctx.r3.u32);
    const uint32_t iosb = ctx.r7.u32, info = ctx.r8.u32, length = ctx.r9.u32, maskString = ctx.r10.u32;
    const bool restart = (StackArg32(ctx, 8) & 0xFF) != 0;
    if (ctx.r5.u32 != 0)
        Fatal("NtQueryDirectoryFile with an APC completion routine is not implemented");
    if (length < 0x48)
    {
        ctx.r3.u64 = kStatusInfoLengthMismatch;
        return;
    }
    if (file == nullptr)
    {
        ctx.r3.u64 = kStatusInvalidHandle;
        return;
    }
    if (!file->directory)
        Fatal("NtQueryDirectoryFile on a file ({})", file->path.string());

    const bool first = !file->enumerating;
    if (first || restart)
    {
        if (first)
        {
            file->mask = "*";
            if (maskString && mem.Read16(maskString) != 0)
                file->mask.assign(reinterpret_cast<const char*>(mem.Translate(mem.Read32(maskString + 4))),
                    mem.Read16(maskString));
            file->enumerating = true;
        }
        file->entries.clear();
        for (const auto& entry : std::filesystem::directory_iterator(file->path))
        {
            const std::wstring wide = entry.path().filename().wstring();
            if (std::any_of(wide.begin(), wide.end(), [](wchar_t c) { return c < 0x20 || c > 0x7E; }))
                Fatal("NtQueryDirectoryFile: non-ASCII host file name in {}", file->path.string());
            if (MatchesMask(entry.path().filename().string(), file->mask))
                file->entries.push_back(entry.path());
        }
        std::sort(file->entries.begin(), file->entries.end(), [](const auto& a, const auto& b)
            { return _wcsicmp(a.filename().c_str(), b.filename().c_str()) < 0; });
        file->nextEntry = 0;
    }

    uint32_t status = kStatusSuccess, written = 0;
    if (file->nextEntry >= file->entries.size())
        status = file->entries.empty() && first ? kStatusNoSuchFile : kStatusNoMoreFiles;
    else
    {
        const std::filesystem::path& path = file->entries[file->nextEntry];
        const std::string name = path.filename().string();
        if (0x40 + name.size() > length)
            Fatal("NtQueryDirectoryFile: {}-byte buffer is too small for \"{}\"", length, name);
        const HostAttributes a = Stat(path);
        mem.Write32(info + 0x00, 0);
        mem.Write32(info + 0x04, uint32_t(file->nextEntry));
        mem.Write64(info + 0x08, a.creation);
        mem.Write64(info + 0x10, a.access);
        mem.Write64(info + 0x18, a.write);
        mem.Write64(info + 0x20, a.write);
        mem.Write64(info + 0x28, a.directory ? 0 : a.size);
        mem.Write64(info + 0x30, a.directory ? 0 : a.size);
        mem.Write32(info + 0x38, GuestAttributes(a));
        mem.Write32(info + 0x3C, uint32_t(name.size()));
        std::memcpy(mem.Translate(info + 0x40), name.data(), name.size());
        file->nextEntry++;
        written = 0x40 + uint32_t(name.size());
    }
    WriteIoStatus(iosb, status, written);
    SignalEvent(ctx.r4.u32);
    ctx.r3.u64 = status;
}

// NTSTATUS NtDeleteFile(POBJECT_ATTRIBUTES ObjectAttributes): a file, or an empty directory.
GUEST_IMPORT(NtDeleteFile)
{
    constexpr uint32_t kStatusDirectoryNotEmpty = 0xC0000101;
    const std::string guestPath = FileObjectName(ctx.r3.u32);
    const auto resolved = vfs::Resolve(guestPath);
    if (!resolved)
    {
        ctx.r3.u64 = kStatusObjectPathNotFound;
        return;
    }
    const HostAttributes a = Stat(resolved->host);
    std::error_code ec;
    if (!a.exists)
        ctx.r3.u64 = kStatusObjectNameNotFound;
    else if (resolved->readOnly)
        ctx.r3.u64 = kStatusAccessDenied;
    else if (a.directory && !std::filesystem::is_empty(resolved->host, ec))
        ctx.r3.u64 = kStatusDirectoryNotEmpty;
    else
        ctx.r3.u64 = std::filesystem::remove(resolved->host, ec) ? kStatusSuccess : kStatusAccessDenied;
    Log("File delete \"{}\" -> {:08X}", guestPath, uint32_t(ctx.r3.u64));
}

// NTSTATUS NtQueryVolumeInformationFile(HANDLE FileHandle, PIO_STATUS_BLOCK IoStatusBlock, PVOID FsInformation,
//                                       ULONG Length, FS_INFORMATION_CLASS FsInformationClass)
// The volume of the file's host folder, as a console FATX volume (16 KiB clusters of 512-byte sectors). The console's
// structures use 8-bit names (the dash copies the label from +0x11 of class 1):
//   1 FileFsVolumeInformation: CreationTime (u64), SerialNumber, LabelLength, SupportsObjects (u8), Label[]
//   3 FileFsSizeInformation: TotalAllocationUnits, AvailableAllocationUnits (u64), SectorsPerAllocationUnit,
//     BytesPerSector
//   5 FileFsAttributeInformation: FileSystemAttributes, MaximumComponentNameLength, FileSystemNameLength, Name[]
// A buffer too small for the name returns STATUS_BUFFER_OVERFLOW with what fits.
GUEST_IMPORT(NtQueryVolumeInformationFile)
{
    constexpr uint32_t kStatusBufferOverflow = 0x80000005, kStatusInfoLengthMismatch = 0xC0000004;
    constexpr uint32_t kSectorSize = 512, kSectorsPerCluster = 32, kClusterSize = kSectorSize * kSectorsPerCluster;
    auto& mem = guest::g_memory;
    FileState* file = LookupFile(ctx.r3.u32);
    const uint32_t iosb = ctx.r4.u32, info = ctx.r5.u32, length = ctx.r6.u32, cls = ctx.r7.u32;
    if (file == nullptr)
    {
        ctx.r3.u64 = kStatusInvalidHandle;
        return;
    }
    const std::wstring root = file->path.root_path().wstring();
    uint32_t written = 0, status = kStatusSuccess;
    auto putName = [&](uint32_t at, const std::string& name) {
        const uint32_t room = length > at ? length - at : 0;
        const uint32_t n = std::min<uint32_t>(room, uint32_t(name.size()));
        std::memcpy(mem.Translate(info + at), name.data(), n);
        written = at + n;
        if (n < name.size())
            status = kStatusBufferOverflow;
    };
    switch (cls)
    {
    case 1:
    {
        if (length < 0x11)
        {
            status = kStatusInfoLengthMismatch;
            break;
        }
        wchar_t label[MAX_PATH + 1] = {};
        DWORD serial = 0;
        GetVolumeInformationW(root.c_str(), label, MAX_PATH + 1, &serial, nullptr, nullptr, nullptr, 0);
        std::string ansi;
        for (const wchar_t* c = label; *c; c++)
            ansi += *c < 0x80 ? char(*c) : '_';
        mem.Write64(info + 0x00, 0);
        mem.Write32(info + 0x08, serial);
        mem.Write32(info + 0x0C, uint32_t(ansi.size()));
        mem.Write8(info + 0x10, 0);
        putName(0x11, ansi);
        break;
    }
    case 3:
    {
        if (length < 0x18)
        {
            status = kStatusInfoLengthMismatch;
            break;
        }
        ULARGE_INTEGER freeBytes{}, totalBytes{};
        GetDiskFreeSpaceExW(file->path.parent_path().c_str(), &freeBytes, &totalBytes, nullptr);
        mem.Write64(info + 0x00, totalBytes.QuadPart / kClusterSize);
        mem.Write64(info + 0x08, freeBytes.QuadPart / kClusterSize);
        mem.Write32(info + 0x10, kSectorsPerCluster);
        mem.Write32(info + 0x14, kSectorSize);
        written = 0x18;
        break;
    }
    case 5:
    {
        if (length < 0x0C)
        {
            status = kStatusInfoLengthMismatch;
            break;
        }
        const std::string name = "FATX";
        mem.Write32(info + 0x00, 0);    // no special attributes
        mem.Write32(info + 0x04, 42);   // FATX file names are at most 42 characters
        mem.Write32(info + 0x08, uint32_t(name.size()));
        putName(0x0C, name);
        break;
    }
    default:
        Fatal("NtQueryVolumeInformationFile class {} is not implemented ({})", cls, file->path.string());
    }
    WriteIoStatus(iosb, status, written);
    ctx.r3.u64 = status;
}

// NTSTATUS NtDeviceIoControlFile(HANDLE File, HANDLE Event, PIO_APC_ROUTINE Apc, PVOID ApcContext,
//     PIO_STATUS_BLOCK IoStatus, ULONG IoControlCode, PVOID Input, ULONG InputLength, PVOID Output, ULONG OutputLength)
// Device controls on an opened volume. IOCTL_ATA_PASS_THROUGH (0x4D028) with IDENTIFY DEVICE (command register 0xEC
// at input +6, data length at +8, data buffer at +0xC): xam.xex reads the drive's serial number from the 512-byte
// answer (bytes 20-39, 0x8167DED0). The answer names the storage device's own serial (kernel/storage). Other control
// codes stop with their number.
GUEST_IMPORT(NtDeviceIoControlFile)
{
    auto& mem = guest::g_memory;
    const uint32_t code = ctx.r8.u32, input = ctx.r9.u32, inputLength = ctx.r10.u32, iosb = ctx.r7.u32;
    FileState* file = LookupFile(ctx.r3.u32);
    if (!file)
    {
        ctx.r3.u64 = 0xC0000008; // STATUS_INVALID_HANDLE
        return;
    }
    if (code == 0x4D028 && inputLength >= 0x10 && mem.Read8(input + 6) == 0xEC)
    {
        const uint32_t length = mem.Read32(input + 8), data = mem.Read32(input + 0xC);
        if (length < 512 || data == 0)
        {
            WriteIoStatus(iosb, kStatusInvalidParameter, 0);
            ctx.r3.u64 = kStatusInvalidParameter;
            return;
        }
        std::array<uint8_t, 0x14> serial{};
        for (const kernel::storage::Device& d : kernel::storage::Devices())
            if (file->path == d.contentRoot)
                serial = d.serial;
        uint8_t identify[512] = {};
        // ATA strings are byte-swapped in 16-bit words: serial (words 10-19), firmware (23-26), model (27-46).
        auto put = [&](uint32_t word, std::string_view text, uint32_t words) {
            std::string padded(text);
            padded.resize(words * 2, ' ');
            for (uint32_t i = 0; i < words * 2; i += 2)
            {
                identify[word * 2 + i] = uint8_t(padded[i + 1]);
                identify[word * 2 + i + 1] = uint8_t(padded[i]);
            }
        };
        put(10, std::string_view(reinterpret_cast<const char*>(serial.data()), serial.size()), 10);
        put(23, "PC", 4);
        put(27, "Xbox 360 hard drive (PC folder)", 20);
        memcpy(mem.Translate(data), identify, 512);
        WriteIoStatus(iosb, kStatusSuccess, 512);
        ctx.r3.u64 = kStatusSuccess;
        return;
    }
    // DeviceAuth's request wait (0x474000, asynchronous with an APC, xam.xex 0x816E7948): the kernel completes it
    // when a device asks to be authenticated; none ever does here, so it stays pending.
    if (code == 0x474000 && file->host == INVALID_HANDLE_VALUE)
    {
        WriteIoStatus(iosb, 0x103, 0); // STATUS_PENDING
        ctx.r3.u64 = 0x103;
        return;
    }
    Fatal("NtDeviceIoControlFile({}, code {:08X}, input {:08X} length {:X}): not implemented", file->path.string(), code,
        input, inputLength);
}

// NTSTATUS NtOpenSymbolicLinkObject(PHANDLE Handle, POBJECT_ATTRIBUTES Attributes): a handle to a symbolic link
// (xam.xex reads where \??\... links point, 0x816E0480). The object keeps the link's target.
namespace
{
    std::mutex g_linkMutex;
    std::map<uint32_t, std::string> g_linkObjects;
}

GUEST_IMPORT(NtOpenSymbolicLinkObject)
{
    const std::string name = ReadObjectName(ctx.r4.u32);
    const auto target = vfs::LinkTarget(name);
    if (!target)
    {
        ctx.r3.u64 = kStatusObjectNameNotFound;
        return;
    }
    const uint32_t object = CreateOwnedObject(0x10);
    InitializeDispatcherHeader(object, kObjectEventNotification, 1);
    {
        std::lock_guard lock(g_linkMutex);
        g_linkObjects[object] = *target;
    }
    SetObjectCleanup(object, [object] {
        std::lock_guard lock(g_linkMutex);
        g_linkObjects.erase(object);
    });
    guest::g_memory.Write32(ctx.r3.u32, CreateHandle(object));
    ctx.r3.u64 = kStatusSuccess;
}

// NTSTATUS NtQuerySymbolicLinkObject(HANDLE Link, PSTRING Target, PULONG ReturnedLength): the target into the STRING's
// buffer (MaximumLength bytes at most).
GUEST_IMPORT(NtQuerySymbolicLinkObject)
{
    auto& mem = guest::g_memory;
    std::string target;
    {
        std::lock_guard lock(g_linkMutex);
        auto it = g_linkObjects.find(LookupHandle(ctx.r3.u32));
        if (it == g_linkObjects.end())
        {
            ctx.r3.u64 = kStatusInvalidHandle;
            return;
        }
        target = it->second;
    }
    if (!target.empty() && target.back() == '\\')
        target.pop_back();
    const uint32_t string = ctx.r4.u32;
    if (ctx.r5.u32)
        mem.Write32(ctx.r5.u32, uint32_t(target.size()));
    const uint16_t maximum = mem.Read16(string + 2);
    if (target.size() > maximum)
    {
        ctx.r3.u64 = 0xC0000023; // STATUS_BUFFER_TOO_SMALL
        return;
    }
    std::memcpy(mem.Translate(mem.Read32(string + 4)), target.data(), target.size());
    mem.Write16(string, uint16_t(target.size()));
    ctx.r3.u64 = kStatusSuccess;
}

// NTSTATUS ObTranslateSymbolicLink(POBJECT_ATTRIBUTES Name, PSTRING Translated): the name (from the DOS devices for a
// RootDirectory of 0xFFFFFFFD, as XAM's content cache passes "GAME:\\...") with its links followed, into the STRING's
// buffer (xam.xex 0x816CF0C8 then terminates it at Length).
GUEST_IMPORT(ObTranslateSymbolicLink)
{
    auto& mem = guest::g_memory;
    std::string name = ReadObjectName(ctx.r3.u32);
    if (mem.Read32(ctx.r3.u32) == 0xFFFFFFFD && !name.empty() && name[0] != '\\')
        name = "\\??\\" + name;
    const auto translated = vfs::TranslatePath(name);
    if (!translated)
    {
        ctx.r3.u64 = kStatusObjectNameNotFound;
        return;
    }
    const uint32_t out = ctx.r4.u32;
    const uint16_t maximum = mem.Read16(out + 2);
    if (translated->size() > maximum)
    {
        ctx.r3.u64 = 0xC0000023; // STATUS_BUFFER_TOO_SMALL
        return;
    }
    std::memcpy(mem.Translate(mem.Read32(out + 4)), translated->data(), translated->size());
    mem.Write16(out, uint16_t(translated->size()));
    ctx.r3.u64 = kStatusSuccess;
}

namespace kernel
{
    std::optional<std::filesystem::path> HostFilePath(uint32_t handle)
    {
        FileState* file = LookupFile(handle);
        if (!file || file->directory)
            return std::nullopt;
        return file->path;
    }
}
