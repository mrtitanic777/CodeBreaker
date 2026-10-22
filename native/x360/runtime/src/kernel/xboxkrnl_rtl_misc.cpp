// xboxkrnl Rtl* helpers (time, strings, memory patterns, status codes) and the kernel pool (ExAllocatePool*).

#include "kernel/kernel.h"
#include "pool.h"

#include <windows.h>

namespace
{
    constexpr uint32_t kStatusBufferOverflow = 0x80000005;

    // TIME_FIELDS: Year, Month, Day, Hour, Minute, Second, Milliseconds, Weekday (CSHORT each).
    namespace tf
    {
        constexpr uint32_t kYear = 0, kMonth = 2, kDay = 4, kHour = 6, kMinute = 8, kSecond = 10,
                           kMilliseconds = 12, kWeekday = 14;
    }

    // ANSI_STRING / UNICODE_STRING: Length (u16 bytes), MaximumLength (u16 bytes), Buffer (pointer).
    void WriteCountedString(uint32_t string, uint16_t length, uint16_t maximum, uint32_t buffer)
    {
        auto& mem = guest::g_memory;
        mem.Write16(string + 0, length);
        mem.Write16(string + 2, maximum);
        mem.Write32(string + 4, buffer);
    }

    // Guest code page: titles only pass ASCII/Latin-1 here; code points above 0xFF become '?'.
    uint8_t ToAnsi(uint16_t c)
    {
        return c <= 0xFF ? uint8_t(c) : uint8_t('?');
    }
}

// ---- Time --------------------------------------------------------------------------------------------------------

// VOID RtlTimeToTimeFields(PLARGE_INTEGER Time, PTIME_FIELDS TimeFields)
GUEST_IMPORT(RtlTimeToTimeFields)
{
    auto& mem = guest::g_memory;
    const uint64_t time = mem.Read64(ctx.r3.u32);
    const uint32_t fields = ctx.r4.u32;
    FILETIME ft{ DWORD(time), DWORD(time >> 32) };
    SYSTEMTIME st{};
    FileTimeToSystemTime(&ft, &st);
    mem.Write16(fields + tf::kYear, st.wYear);
    mem.Write16(fields + tf::kMonth, st.wMonth);
    mem.Write16(fields + tf::kDay, st.wDay);
    mem.Write16(fields + tf::kHour, st.wHour);
    mem.Write16(fields + tf::kMinute, st.wMinute);
    mem.Write16(fields + tf::kSecond, st.wSecond);
    mem.Write16(fields + tf::kMilliseconds, st.wMilliseconds);
    mem.Write16(fields + tf::kWeekday, st.wDayOfWeek);
}

// BOOLEAN RtlTimeFieldsToTime(PTIME_FIELDS TimeFields, PLARGE_INTEGER Time)
GUEST_IMPORT(RtlTimeFieldsToTime)
{
    auto& mem = guest::g_memory;
    const uint32_t fields = ctx.r3.u32;
    SYSTEMTIME st{};
    st.wYear = mem.Read16(fields + tf::kYear);
    st.wMonth = mem.Read16(fields + tf::kMonth);
    st.wDay = mem.Read16(fields + tf::kDay);
    st.wHour = mem.Read16(fields + tf::kHour);
    st.wMinute = mem.Read16(fields + tf::kMinute);
    st.wSecond = mem.Read16(fields + tf::kSecond);
    st.wMilliseconds = mem.Read16(fields + tf::kMilliseconds);
    FILETIME ft;
    if (!SystemTimeToFileTime(&st, &ft))
    {
        ctx.r3.u64 = 0;
        return;
    }
    mem.Write64(ctx.r4.u32, uint64_t(ft.dwHighDateTime) << 32 | ft.dwLowDateTime);
    ctx.r3.u64 = 1;
}

// ---- Strings -----------------------------------------------------------------------------------------------------

// VOID RtlInitAnsiString(PANSI_STRING DestinationString, PCSZ SourceString)
GUEST_IMPORT(RtlInitAnsiString)
{
    const uint32_t source = ctx.r4.u32;
    if (source == 0)
    {
        WriteCountedString(ctx.r3.u32, 0, 0, 0);
        return;
    }
    const size_t length = std::min<size_t>(strlen(reinterpret_cast<const char*>(guest::g_memory.Translate(source))), 0xFFFE);
    WriteCountedString(ctx.r3.u32, uint16_t(length), uint16_t(length + 1), source);
}

// VOID RtlInitUnicodeString(PUNICODE_STRING DestinationString, PCWSTR SourceString)
GUEST_IMPORT(RtlInitUnicodeString)
{
    auto& mem = guest::g_memory;
    const uint32_t source = ctx.r4.u32;
    if (source == 0)
    {
        WriteCountedString(ctx.r3.u32, 0, 0, 0);
        return;
    }
    uint32_t characters = 0;
    while (mem.Read16(source + characters * 2) != 0 && characters < 0x7FFE)
        characters++;
    WriteCountedString(ctx.r3.u32, uint16_t(characters * 2), uint16_t(characters * 2 + 2), source);
}

// VOID RtlFreeAnsiString(PANSI_STRING AnsiString): frees a buffer allocated by RtlUnicodeStringToAnsiString.
GUEST_IMPORT(RtlFreeAnsiString)
{
    auto& mem = guest::g_memory;
    const uint32_t string = ctx.r3.u32;
    const uint32_t buffer = mem.Read32(string + 4);
    if (buffer)
        KernelPool().Free(buffer);
    WriteCountedString(string, 0, 0, 0);
}

// NTSTATUS RtlUnicodeStringToAnsiString(PANSI_STRING Destination, PUNICODE_STRING Source, BOOLEAN Allocate)
GUEST_IMPORT(RtlUnicodeStringToAnsiString)
{
    auto& mem = guest::g_memory;
    const uint32_t destination = ctx.r3.u32;
    const uint32_t source = ctx.r4.u32;
    const bool allocate = ctx.r5.u32 != 0;
    const uint32_t characters = mem.Read16(source) / 2;
    const uint32_t sourceBuffer = mem.Read32(source + 4);

    uint32_t status = kernel::kStatusSuccess;
    uint32_t buffer;
    uint32_t copy = characters;
    if (allocate)
    {
        buffer = KernelPool().Allocate(characters + 1);
        if (buffer == 0)
        {
            ctx.r3.u64 = kernel::kStatusNoMemory;
            return;
        }
        WriteCountedString(destination, uint16_t(characters), uint16_t(characters + 1), buffer);
    }
    else
    {
        buffer = mem.Read32(destination + 4);
        const uint16_t maximum = mem.Read16(destination + 2);
        if (characters + 1 > maximum)
        {
            status = kStatusBufferOverflow;
            copy = maximum ? maximum - 1u : 0u;
        }
        mem.Write16(destination, uint16_t(copy));
    }
    for (uint32_t i = 0; i < copy; i++)
        mem.Write8(buffer + i, ToAnsi(mem.Read16(sourceBuffer + i * 2)));
    if (copy < characters + 1 && (allocate || copy < mem.Read16(destination + 2)))
        mem.Write8(buffer + copy, 0);
    ctx.r3.u64 = status;
}

// NTSTATUS RtlMultiByteToUnicodeN(PWCH UnicodeString, ULONG MaxBytesInUnicodeString, PULONG BytesInUnicodeString,
//                                 PCHAR MultiByteString, ULONG BytesInMultiByteString)
GUEST_IMPORT(RtlMultiByteToUnicodeN)
{
    auto& mem = guest::g_memory;
    const uint32_t count = std::min(ctx.r4.u32 / 2, ctx.r7.u32);
    for (uint32_t i = 0; i < count; i++)
        mem.Write16(ctx.r3.u32 + i * 2, mem.Read8(ctx.r6.u32 + i));
    if (ctx.r5.u32)
        mem.Write32(ctx.r5.u32, count * 2);
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// NTSTATUS RtlUnicodeToMultiByteN(PCHAR MultiByteString, ULONG MaxBytesInMultiByteString, PULONG BytesInMultiByteString,
//                                 PWCH UnicodeString, ULONG BytesInUnicodeString)
GUEST_IMPORT(RtlUnicodeToMultiByteN)
{
    auto& mem = guest::g_memory;
    const uint32_t count = std::min(ctx.r4.u32, ctx.r7.u32 / 2);
    for (uint32_t i = 0; i < count; i++)
        mem.Write8(ctx.r3.u32 + i, ToAnsi(mem.Read16(ctx.r6.u32 + i * 2)));
    if (ctx.r5.u32)
        mem.Write32(ctx.r5.u32, count);
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// ---- Memory patterns ---------------------------------------------------------------------------------------------

// SIZE_T RtlCompareMemoryUlong(PVOID Source, SIZE_T Length, ULONG Pattern): bytes matching before the first mismatch.
GUEST_IMPORT(RtlCompareMemoryUlong)
{
    auto& mem = guest::g_memory;
    const uint32_t source = ctx.r3.u32;
    const uint32_t length = ctx.r4.u32 & ~3u;
    uint32_t matched = 0;
    while (matched < length && mem.Read32(source + matched) == ctx.r5.u32)
        matched += 4;
    ctx.r3.u64 = matched;
}

// VOID RtlFillMemoryUlong(PVOID Destination, SIZE_T Length, ULONG Pattern)
GUEST_IMPORT(RtlFillMemoryUlong)
{
    auto& mem = guest::g_memory;
    const uint32_t length = ctx.r4.u32 & ~3u;
    for (uint32_t i = 0; i < length; i += 4)
        mem.Write32(ctx.r3.u32 + i, ctx.r5.u32);
}

// ---- Status codes ------------------------------------------------------------------------------------------------

// ULONG RtlNtStatusToDosError(NTSTATUS Status): the console kernel is NT-derived and shares NT's mapping table.
GUEST_IMPORT(RtlNtStatusToDosError)
{
    using Fn = ULONG(WINAPI*)(LONG);
    static const Fn convert = reinterpret_cast<Fn>(GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "RtlNtStatusToDosError"));
    ctx.r3.u64 = convert(LONG(ctx.r3.u32));
}

// ---- Kernel pool -------------------------------------------------------------------------------------------------

// PVOID ExAllocatePool(SIZE_T NumberOfBytes)
GUEST_IMPORT(ExAllocatePool)
{
    ctx.r3.u64 = KernelPool().Allocate(ctx.r3.u32);
}

// PVOID ExAllocatePoolTypeWithTag(SIZE_T NumberOfBytes, ULONG Tag, POOL_TYPE PoolType)
GUEST_IMPORT(ExAllocatePoolTypeWithTag)
{
    ctx.r3.u64 = KernelPool().Allocate(ctx.r3.u32);
}

// VOID ExFreePool(PVOID P)
GUEST_IMPORT(ExFreePool)
{
    if (!KernelPool().Free(ctx.r3.u32))
        Fatal("ExFreePool of {:08X}, which is not a pool allocation", ctx.r3.u32);
}

// PVOID ExAllocatePoolWithTag(SIZE_T NumberOfBytes, ULONG Tag)
GUEST_IMPORT(ExAllocatePoolWithTag)
{
    ctx.r3.u64 = KernelPool().Allocate(ctx.r3.u32);
}

namespace
{
    // RtlUpcaseUnicodeChar's mapping (Windows' own uppercase table, which covers what the console's does).
    uint16_t UpcaseWide(uint16_t c)
    {
        wchar_t w = wchar_t(c);
        CharUpperBuffW(&w, 1);
        return uint16_t(w);
    }

    uint8_t UpcaseAnsi(uint8_t c) { return c >= 'a' && c <= 'z' ? uint8_t(c - 32) : c; }

    // RtlCompareStringN / RtlCompareUnicodeStringN: the first differing character's difference over the shorter
    // length, else the length difference. A length of 0xFFFF means "up to the terminating zero" (Xenia).
    template <typename Char, typename Read, typename Upcase>
    int32_t CompareN(uint32_t a, uint32_t lengthA, uint32_t b, uint32_t lengthB, bool caseInsensitive, Read read,
                     Upcase upcase)
    {
        auto length = [&](uint32_t s, uint32_t n) {
            if (n != 0xFFFF)
                return n;
            uint32_t i = 0;
            while (read(s + i * sizeof(Char)) != 0)
                i++;
            return i;
        };
        lengthA = length(a, lengthA);
        lengthB = length(b, lengthB);
        const uint32_t n = std::min(lengthA, lengthB);
        for (uint32_t i = 0; i < n; i++)
        {
            Char x = Char(read(a + i * sizeof(Char))), y = Char(read(b + i * sizeof(Char)));
            if (caseInsensitive)
            {
                x = upcase(x);
                y = upcase(y);
            }
            if (x != y)
                return int32_t(x) - int32_t(y);
        }
        return int32_t(lengthA) - int32_t(lengthB);
    }
}

// LONG RtlCompareStringN(const CHAR* String1, ULONG Length1, const CHAR* String2, ULONG Length2,
//                        BOOLEAN CaseInSensitive): lengths in characters.
GUEST_IMPORT(RtlCompareStringN)
{
    auto& mem = guest::g_memory;
    ctx.r3.s64 = CompareN<uint8_t>(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, (ctx.r7.u32 & 0xFF) != 0,
        [&](uint32_t p) { return mem.Read8(p); }, UpcaseAnsi);
}

// LONG RtlCompareUnicodeStringN(const WCHAR* String1, ULONG Length1, const WCHAR* String2, ULONG Length2,
//                               BOOLEAN CaseInSensitive): lengths in characters.
GUEST_IMPORT(RtlCompareUnicodeStringN)
{
    auto& mem = guest::g_memory;
    ctx.r3.s64 = CompareN<uint16_t>(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, (ctx.r7.u32 & 0xFF) != 0,
        [&](uint32_t p) { return mem.Read16(p); }, UpcaseWide);
}

// WCHAR RtlUpcaseUnicodeChar(WCHAR SourceCharacter)
GUEST_IMPORT(RtlUpcaseUnicodeChar)
{
    ctx.r3.u64 = UpcaseWide(uint16_t(ctx.r3.u32));
}

// VOID RtlCopyString(PSTRING Destination, const STRING* Source): STRING = { u16 Length, u16 MaximumLength, PCHAR
// Buffer }. Copies at most the destination's MaximumLength bytes; a null source empties the destination.
GUEST_IMPORT(RtlCopyString)
{
    auto& mem = guest::g_memory;
    const uint32_t dst = ctx.r3.u32, src = ctx.r4.u32;
    if (!dst)
        return;
    if (!src)
    {
        mem.Write16(dst, 0);
        return;
    }
    const uint16_t length = std::min(mem.Read16(src), mem.Read16(dst + 2));
    if (length)
        std::memmove(mem.Translate(mem.Read32(dst + 4)), mem.Translate(mem.Read32(src + 4)), length);
    mem.Write16(dst, length);
}

// NTSTATUS RtlAppendStringToString(PSTRING Destination, const STRING* Source): appends when it fits in the
// destination's MaximumLength, else STATUS_BUFFER_TOO_SMALL and nothing changes.
GUEST_IMPORT(RtlAppendStringToString)
{
    auto& mem = guest::g_memory;
    const uint32_t dst = ctx.r3.u32, src = ctx.r4.u32;
    const uint16_t add = src ? mem.Read16(src) : 0;
    const uint16_t length = mem.Read16(dst), maximum = mem.Read16(dst + 2);
    if (uint32_t(length) + add > maximum)
    {
        ctx.r3.u64 = 0xC0000023; // STATUS_BUFFER_TOO_SMALL
        return;
    }
    if (add)
        std::memmove(mem.Translate(mem.Read32(dst + 4) + length), mem.Translate(mem.Read32(src + 4)), add);
    mem.Write16(dst, uint16_t(length + add));
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// SIZE_T RtlCompareMemory(const VOID* Source1, const VOID* Source2, SIZE_T Length): the number of leading bytes that
// match.
GUEST_IMPORT(RtlCompareMemory)
{
    auto& mem = guest::g_memory;
    const uint8_t* a = mem.Translate(ctx.r3.u32);
    const uint8_t* b = mem.Translate(ctx.r4.u32);
    uint32_t i = 0;
    while (i < ctx.r5.u32 && a[i] == b[i])
        i++;
    ctx.r3.u64 = i;
}
