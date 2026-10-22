// CB_KERNEL_TEST=1: checks of kernel imports the dashboard reaches rarely (run instead of the dashboard, after the
// kernel starts). The imports are called exactly as recompiled code calls them (their __imp__ functions, from a guest
// thread); the routines they call back (DPC, APC kernel / normal routines) are host functions given guest addresses
// through the dynamic-export range. Each check logs one line; the process exits with the number of failures.

#include "kernel/objects.h"

#include <atomic>
#include <chrono>
#include <cstring>
#include <format>
#include <string>
#include <thread>

PPC_EXTERN_FUNC(__imp__KeInitializeEvent);
PPC_EXTERN_FUNC(__imp__KeSetEvent);
PPC_EXTERN_FUNC(__imp__KeWaitForSingleObject);
PPC_EXTERN_FUNC(__imp__KeDelayExecutionThread);
PPC_EXTERN_FUNC(__imp__KeInitializeTimerEx);
PPC_EXTERN_FUNC(__imp__KeSetTimerEx);
PPC_EXTERN_FUNC(__imp__KeCancelTimer);
PPC_EXTERN_FUNC(__imp__KeInitializeDpc);
PPC_EXTERN_FUNC(__imp__NtCreateTimer);
PPC_EXTERN_FUNC(__imp__NtSetTimerEx);
PPC_EXTERN_FUNC(__imp__NtClose);
PPC_EXTERN_FUNC(__imp__KeInitializeApc);
PPC_EXTERN_FUNC(__imp__KeInsertQueueApc);
PPC_EXTERN_FUNC(__imp__NtCreateMutant);
PPC_EXTERN_FUNC(__imp__NtReleaseMutant);
PPC_EXTERN_FUNC(__imp__NtWaitForSingleObjectEx);
PPC_EXTERN_FUNC(__imp__XeCryptMd5);
PPC_EXTERN_FUNC(__imp__XeCryptMd5Init);
PPC_EXTERN_FUNC(__imp__XeCryptMd5Update);
PPC_EXTERN_FUNC(__imp__XeCryptMd5Final);
PPC_EXTERN_FUNC(__imp__XeCryptHmacMd5Init);
PPC_EXTERN_FUNC(__imp__XeCryptHmacMd5Update);
PPC_EXTERN_FUNC(__imp__XeCryptHmacMd5Final);
PPC_EXTERN_FUNC(__imp__XeCryptDesParity);
PPC_EXTERN_FUNC(__imp__XeCryptDesKey);
PPC_EXTERN_FUNC(__imp__XeCryptDesEcb);
PPC_EXTERN_FUNC(__imp__XeCryptDes3Key);
PPC_EXTERN_FUNC(__imp__XeCryptDes3Cbc);
PPC_EXTERN_FUNC(__imp__XeCryptAesKey);
PPC_EXTERN_FUNC(__imp__XeCryptAesCbc);
PPC_EXTERN_FUNC(__imp__XeCryptBnQw_SwapDwQwLeBe);
PPC_EXTERN_FUNC(__imp__XeCryptBnQwNeRsaPubCrypt);
PPC_EXTERN_FUNC(__imp__RtlCompareStringN);
PPC_EXTERN_FUNC(__imp__RtlCompareUnicodeStringN);
PPC_EXTERN_FUNC(__imp__RtlUpcaseUnicodeChar);
PPC_EXTERN_FUNC(__imp__ObLookupThreadByThreadId);
PPC_EXTERN_FUNC(__imp__ObOpenObjectByPointer);
PPC_EXTERN_FUNC(__imp__ObDereferenceObject);
PPC_EXTERN_FUNC(__imp__NtAllocateVirtualMemory);
PPC_EXTERN_FUNC(__imp__NtProtectVirtualMemory);
PPC_EXTERN_FUNC(__imp__NtFreeVirtualMemory);
PPC_EXTERN_FUNC(__imp__RtlInitAnsiString);
PPC_EXTERN_FUNC(__imp__NtOpenFile);
PPC_EXTERN_FUNC(__imp__NtQueryVolumeInformationFile);
PPC_EXTERN_FUNC(__imp__NtDeleteFile);
PPC_EXTERN_FUNC(__imp__XamGetLanguageLocaleString);
PPC_EXTERN_FUNC(__imp__XamGetOnlineCountryFromLocale);
PPC_EXTERN_FUNC(__imp__XamGetOnlineLanguageAndCountryString);
PPC_EXTERN_FUNC(__imp__XamGetLocaleString);

namespace
{
    using Clock = std::chrono::steady_clock;

    // Host routines with guest addresses (library "kernel_test").
    std::atomic<int> g_dpcCalls = 0;
    std::atomic<uint32_t> g_dpcContext = 0, g_dpcEvent = 0;
    std::atomic<int> g_apcCalls = 0;
    std::atomic<uint32_t> g_apcContext = 0, g_apcArgument1 = 0;
    std::atomic<int> g_kernelRoutineCalls = 0, g_normalCalls = 0;
    std::atomic<uint32_t> g_normalContext = 0;

    PPC_FUNC(TestDpcRoutine) // (Dpc, DeferredContext, SystemArgument1, SystemArgument2)
    {
        g_dpcCalls++;
        g_dpcContext = ctx.r4.u32;
        if (const uint32_t event = g_dpcEvent.load())
        {
            PPCContext call = ctx;
            call.r3.u64 = event;
            call.r4.u64 = 0;
            call.r5.u64 = 0;
            __imp__KeSetEvent(call, base);
        }
    }

    PPC_FUNC(TestTimerApc) // (ApcContext, TimerLow, TimerHigh)
    {
        g_apcCalls++;
        g_apcContext = ctx.r3.u32;
        g_apcArgument1 = ctx.r4.u32;
    }

    PPC_FUNC(TestKernelRoutine) // (Apc, &NormalRoutine, &NormalContext, &Argument1, &Argument2)
    {
        g_kernelRoutineCalls++;
        auto& mem = guest::g_memory;
        mem.Write32(ctx.r5.u32, mem.Read32(ctx.r5.u32) + 100); // NormalContext += 100
    }

    PPC_FUNC(TestNormalRoutine) // (NormalContext, Argument1, Argument2)
    {
        g_normalCalls++;
        g_normalContext = ctx.r3.u32;
    }

    kernel::DynamicExportRegistrar g_r1("kernel_test", 1, TestDpcRoutine);
    kernel::DynamicExportRegistrar g_r2("kernel_test", 2, TestTimerApc);
    kernel::DynamicExportRegistrar g_r3("kernel_test", 3, TestKernelRoutine);
    kernel::DynamicExportRegistrar g_r4("kernel_test", 4, TestNormalRoutine);

    struct Runner
    {
        PPCContext& ctx;
        uint8_t* base;
        int failures = 0;

        uint32_t Call(PPCFunc* f, std::initializer_list<uint64_t> args)
        {
            PPCContext call = ctx;
            PPCRegister* regs[] = { &call.r3, &call.r4, &call.r5, &call.r6, &call.r7, &call.r8, &call.r9, &call.r10 };
            size_t i = 0;
            for (uint64_t a : args)
                regs[i++]->u64 = a;
            f(call, base);
            return call.r3.u32;
        }

        void Check(bool ok, const std::string& what)
        {
            Log("Kernel test: {} {}", ok ? "ok  " : "FAIL", what);
            failures += !ok;
        }
    };

    uint32_t Alloc(uint32_t size)
    {
        const uint32_t p = kernel::CreateOwnedObject(size);
        memset(guest::g_memory.Translate(p), 0, size);
        return p;
    }

    // A relative timeout (ms) as the LARGE_INTEGER a pointer argument points to.
    uint32_t Timeout(int64_t ms)
    {
        const uint32_t p = Alloc(8);
        guest::g_memory.Write64(p, uint64_t(-ms * 10000));
        return p;
    }

    int Run(PPCContext& ctx, uint8_t* base)
    {
        Runner t{ ctx, base };
        auto& mem = guest::g_memory;
        const uint32_t dpcRoutine = kernel::BindDynamicExport("kernel_test", 1);
        const uint32_t timerApc = kernel::BindDynamicExport("kernel_test", 2);
        const uint32_t kernelRoutine = kernel::BindDynamicExport("kernel_test", 3);
        const uint32_t normalRoutine = kernel::BindDynamicExport("kernel_test", 4);
        constexpr uint32_t kStatusUserApc = 0xC0, kStatusTimeout = 0x102;

        // KeInitializeEvent + KeInitializeTimerEx + KeSetTimerEx with a DPC: the timer signals, the DPC runs.
        const uint32_t event = Alloc(0x10), timer = Alloc(0x28), dpc = Alloc(0x1C);
        t.Call(__imp__KeInitializeEvent, { event, 1, 0 });
        t.Call(__imp__KeInitializeTimerEx, { timer, 0, 0 });
        t.Call(__imp__KeInitializeDpc, { dpc, dpcRoutine, 0x1234 });
        g_dpcEvent = event;
        auto started = Clock::now();
        t.Check(t.Call(__imp__KeSetTimerEx, { timer, uint64_t(-20 * 10000), 0, dpc }) == 0, "KeSetTimerEx on an idle timer returns FALSE");
        const uint32_t waitTimer = t.Call(__imp__KeWaitForSingleObject, { timer, 0, 0, 0, Timeout(1000) });
        const double ms = std::chrono::duration<double, std::milli>(Clock::now() - started).count();
        t.Check(waitTimer == 0 && ms >= 15 && ms < 500, std::format("the timer signals after its due time ({:.1f} ms)", ms));
        const uint32_t waitEvent = t.Call(__imp__KeWaitForSingleObject, { event, 0, 0, 0, Timeout(1000) });
        t.Check(waitEvent == 0 && g_dpcCalls == 1 && g_dpcContext == 0x1234, "its DPC ran once with its context and set the event");

        // KeCancelTimer: TRUE while set, FALSE after.
        t.Call(__imp__KeSetTimerEx, { timer, uint64_t(-5000 * 10000), 0, 0 });
        t.Check(t.Call(__imp__KeCancelTimer, { timer }) == 1 && t.Call(__imp__KeCancelTimer, { timer }) == 0,
                "KeCancelTimer: TRUE when set, then FALSE");
        t.Check(t.Call(__imp__KeWaitForSingleObject, { timer, 0, 0, 0, Timeout(50) }) == kStatusTimeout,
                "a cancelled timer does not signal");

        // Periodic: 10 ms period over ~200 ms.
        g_dpcEvent = 0;
        g_dpcCalls = 0;
        t.Call(__imp__KeSetTimerEx, { timer, uint64_t(-10 * 10000), 10, dpc });
        std::this_thread::sleep_for(std::chrono::milliseconds(205));
        t.Call(__imp__KeCancelTimer, { timer });
        const int periodic = g_dpcCalls;
        t.Check(periodic >= 15 && periodic <= 21, std::format("a 10 ms periodic timer's DPC ran {} times in ~205 ms", periodic));

        // NtSetTimerEx with an APC routine: delivered at the next alertable user-mode wait.
        const uint32_t handlePtr = Alloc(4);
        t.Check(t.Call(__imp__NtCreateTimer, { handlePtr, 0, 0 }) == 0, "NtCreateTimer");
        const uint32_t handle = mem.Read32(handlePtr);
        t.Call(__imp__NtSetTimerEx, { handle, Timeout(10), timerApc, 1, 0x55, 0, 0, 0 });
        const uint32_t delay = t.Call(__imp__KeDelayExecutionThread, { 1, 1, Timeout(2000) });
        t.Check(delay == kStatusUserApc && g_apcCalls == 1 && g_apcContext == 0x55 && g_apcArgument1 != 0,
                std::format("the timer's APC ends an alertable wait (status {:X}, calls {}, context {:X})", delay,
                            g_apcCalls.load(), g_apcContext.load()));
        t.Call(__imp__NtClose, { handle });

        // KeInitializeApc / KeInsertQueueApc (user mode): kernel routine, then normal routine with its result.
        const uint32_t apc = Alloc(0x28);
        const uint32_t self = kernel::CurrentThread()->threadObject;
        t.Call(__imp__KeInitializeApc, { apc, self, kernelRoutine, 0, normalRoutine, 1, 7 });
        t.Check(mem.Read32(apc + 4) == self, "KAPC.Thread at +4");
        t.Check(t.Call(__imp__KeInsertQueueApc, { apc, 1, 2, 0 }) == 1, "KeInsertQueueApc: TRUE");
        t.Check(t.Call(__imp__KeInsertQueueApc, { apc, 1, 2, 0 }) == 0, "KeInsertQueueApc again while queued: FALSE");
        const uint32_t delay2 = t.Call(__imp__KeDelayExecutionThread, { 1, 1, Timeout(1000) });
        t.Check(delay2 == kStatusUserApc && g_kernelRoutineCalls == 1 && g_normalCalls == 1 && g_normalContext == 107,
                std::format("alertable wait runs the kernel routine, then the normal routine with its context (status {:X}, "
                            "context {})", delay2, g_normalContext.load()));
        t.Check(t.Call(__imp__KeInsertQueueApc, { apc, 1, 2, 0 }) == 1, "after delivery the KAPC can be queued again");
        t.Call(__imp__KeDelayExecutionThread, { 1, 1, Timeout(1000) });

        // Mutants: owned at creation, recursive acquisition, release by a non-owner.
        t.Check(t.Call(__imp__NtCreateMutant, { handlePtr, 0, 1 }) == 0, "NtCreateMutant (initially owned)");
        const uint32_t mutant = mem.Read32(handlePtr);
        t.Check(t.Call(__imp__NtWaitForSingleObjectEx, { mutant, 1, 0, Timeout(100) }) == 0, "the owner acquires it again");
        const uint32_t previousPtr = Alloc(4);
        t.Check(t.Call(__imp__NtReleaseMutant, { mutant, previousPtr }) == 0 && int32_t(mem.Read32(previousPtr)) == -1,
                "first release: previous count -1");
        t.Check(t.Call(__imp__NtReleaseMutant, { mutant, previousPtr }) == 0 && int32_t(mem.Read32(previousPtr)) == 0,
                "second release frees it (previous 0)");
        t.Check(t.Call(__imp__NtReleaseMutant, { mutant, 0 }) == 0xC0000046, "a release by a non-owner: STATUS_MUTANT_NOT_OWNED");
        t.Check(t.Call(__imp__NtWaitForSingleObjectEx, { mutant, 1, 0, Timeout(100) }) == 0, "a free mutant is acquired");
        t.Call(__imp__NtReleaseMutant, { mutant, 0 });
        t.Call(__imp__NtClose, { mutant });

        // ---- Crypto (published test vectors) ----
        auto bytes = [&](const char* hex) {
            const uint32_t n = uint32_t(strlen(hex) / 2), p = Alloc(n + 16);
            for (uint32_t i = 0; i < n; i++)
                mem.Write8(p + i, uint8_t(std::stoul(std::string(hex + i * 2, 2), nullptr, 16)));
            return p;
        };
        auto hexOf = [&](uint32_t p, uint32_t n) {
            std::string out;
            for (uint32_t i = 0; i < n; i++)
                out += std::format("{:02x}", mem.Read8(p + i));
            return out;
        };
        const uint32_t out = Alloc(64), state = Alloc(0x200);
        const uint32_t abc = bytes("616263");
        t.Call(__imp__XeCryptMd5, { abc, 1, abc + 1, 2, 0, 0, out, 16 });
        t.Check(hexOf(out, 16) == "900150983cd24fb0d6963f7d28e17f72", "XeCryptMd5(\"abc\") (RFC 1321)");
        t.Call(__imp__XeCryptMd5Init, { state });
        t.Call(__imp__XeCryptMd5Update, { state, abc, 3 });
        t.Call(__imp__XeCryptMd5Final, { state, out, 16 });
        t.Check(hexOf(out, 16) == "900150983cd24fb0d6963f7d28e17f72", "XeCryptMd5Init / Update / Final");
        const uint32_t hmacKey = bytes("0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b"), hiThere = bytes("4869205468657265");
        t.Call(__imp__XeCryptHmacMd5Init, { state, hmacKey, 16 });
        t.Call(__imp__XeCryptHmacMd5Update, { state, hiThere, 8 });
        t.Call(__imp__XeCryptHmacMd5Final, { state, out, 16 });
        t.Check(hexOf(out, 16) == "9294727a3638bb1c13f48ef8158bfc9d", "HMAC-MD5 (RFC 2104 test 1)");

        const uint32_t parityIn = bytes("000103fe"), parityOut = Alloc(4);
        t.Call(__imp__XeCryptDesParity, { parityIn, 4, parityOut });
        t.Check(hexOf(parityOut, 4) == "010102fe", std::format("XeCryptDesParity ({})", hexOf(parityOut, 4)));
        const uint32_t desKey = bytes("133457799bbcdff1"), desPlain = bytes("0123456789abcdef");
        t.Call(__imp__XeCryptDesKey, { state, desKey });
        t.Call(__imp__XeCryptDesEcb, { state, desPlain, out, 1 });
        t.Check(hexOf(out, 8) == "85e813540f0ab405", "XeCryptDesEcb encrypt (the classic DES example)");
        t.Call(__imp__XeCryptDesEcb, { state, out, out + 8, 0 });
        t.Check(hexOf(out + 8, 8) == "0123456789abcdef", "XeCryptDesEcb decrypt");
        const uint32_t des3Key = bytes("133457799bbcdff1133457799bbcdff1133457799bbcdff1"), feed = Alloc(16);
        t.Call(__imp__XeCryptDes3Key, { state, des3Key });
        t.Call(__imp__XeCryptDes3Cbc, { state, desPlain, 8, out, feed, 1 });
        t.Check(hexOf(out, 8) == "85e813540f0ab405" && hexOf(feed, 8) == "85e813540f0ab405",
                "XeCryptDes3Cbc with three equal keys = DES; the feed holds the last block");

        const uint32_t aesKey = bytes("000102030405060708090a0b0c0d0e0f"), aesPlain = bytes("00112233445566778899aabbccddeeff");
        t.Call(__imp__XeCryptAesKey, { state, aesKey });
        for (int i = 0; i < 16; i++)
            mem.Write8(feed + i, 0);
        t.Call(__imp__XeCryptAesCbc, { state, aesPlain, 16, out, feed, 1 });
        t.Check(hexOf(out, 16) == "69c4e0d86a7b0430d8cdb78070b4c55a", "XeCryptAesCbc encrypt (FIPS-197 C.1)");
        for (int i = 0; i < 16; i++)
            mem.Write8(feed + i, 0);
        t.Call(__imp__XeCryptAesCbc, { state, out, 16, out + 16, feed, 0 });
        t.Check(hexOf(out + 16, 16) == "00112233445566778899aabbccddeeff", "XeCryptAesCbc decrypt");
        t.Call(__imp__XeCryptAesKey, { state, bytes("2b7e151628aed2a6abf7158809cf4f3c") });
        t.Check(hexOf(state + 0xA0, 16) == "d014f9a8c9ee2589e13f0cc8b6630ca6" && hexOf(state + 0xB0, 16) == hexOf(state + 0xA0, 16),
                "XeCryptAesKey: round key 10 (FIPS-197 A.1) and the inverse schedule starting with it");

        const uint32_t swapIn = bytes("0102030405060708"), swapOut = Alloc(8);
        t.Call(__imp__XeCryptBnQw_SwapDwQwLeBe, { swapIn, swapOut, 1 });
        t.Check(hexOf(swapOut, 8) == "0807060504030201", "XeCryptBnQw_SwapDwQwLeBe");
        // A 512-bit modulus 2^511 + 1 (8 qwords, least significant first, each big-endian), 2^3 = 8.
        const uint32_t rsa = Alloc(0x10 + 64), rsaIn = Alloc(64), rsaOut = Alloc(64);
        mem.Write32(rsa, 8);
        mem.Write32(rsa + 4, 3);
        mem.Write64(rsa + 0x10, 1);
        mem.Write64(rsa + 0x10 + 7 * 8, 0x8000000000000000ull);
        mem.Write64(rsaIn, 2);
        const uint32_t rsaOk = t.Call(__imp__XeCryptBnQwNeRsaPubCrypt, { rsaIn, rsaOut, rsa });
        bool high = true;
        for (int i = 1; i < 8; i++)
            high &= mem.Read64(rsaOut + i * 8) == 0;
        t.Check(rsaOk == 1 && mem.Read64(rsaOut) == 8 && high, "XeCryptBnQwNeRsaPubCrypt: 2^3 mod (2^511 + 1) = 8");

        // ---- Strings, objects, memory, volumes ----
        auto cstr = [&](const std::string& text) {
            const uint32_t p = Alloc(uint32_t(text.size()) + 1);
            std::memcpy(mem.Translate(p), text.data(), text.size());
            return p;
        };
        auto wstr = [&](const std::u16string& text) {
            const uint32_t p = Alloc(uint32_t(text.size() * 2) + 2);
            for (size_t i = 0; i < text.size(); i++)
                mem.Write16(p + uint32_t(i * 2), text[i]);
            return p;
        };
        const uint32_t hello = cstr("Hello"), help = cstr("HELP");
        t.Check(int32_t(t.Call(__imp__RtlCompareStringN, { hello, 3, help, 3, 1 })) == 0, "RtlCompareStringN: \"Hel\" = \"HEL\" ignoring case");
        t.Check(int32_t(t.Call(__imp__RtlCompareStringN, { hello, 5, help, 4, 1 })) == int32_t('L') - int32_t('P'),
                "RtlCompareStringN: the first difference (L - P)");
        t.Check(int32_t(t.Call(__imp__RtlCompareStringN, { hello, 3, help, 3, 0 })) > 0, "RtlCompareStringN: case-sensitive 'e' > 'E'");
        t.Check(int32_t(t.Call(__imp__RtlCompareStringN, { hello, 0xFFFF, hello, 4, 0 })) == 1, "RtlCompareStringN: 0xFFFF = up to the zero");
        const uint32_t w1 = wstr(u"café"), w2 = wstr(u"CAFÉ");
        t.Check(int32_t(t.Call(__imp__RtlCompareUnicodeStringN, { w1, 4, w2, 4, 1 })) == 0, "RtlCompareUnicodeStringN ignoring case (with é)");
        t.Check(t.Call(__imp__RtlUpcaseUnicodeChar, { 0xE9 }) == 0xC9 && t.Call(__imp__RtlUpcaseUnicodeChar, { 'q' }) == 'Q',
                "RtlUpcaseUnicodeChar");

        const uint32_t threadPtr = Alloc(4);
        t.Check(t.Call(__imp__ObLookupThreadByThreadId, { kernel::CurrentThread()->id, threadPtr }) == 0 &&
                mem.Read32(threadPtr) == kernel::CurrentThread()->threadObject, "ObLookupThreadByThreadId (this thread)");
        t.Check(t.Call(__imp__ObOpenObjectByPointer, { mem.Read32(threadPtr), 0, handlePtr }) == 0 && mem.Read32(handlePtr) != 0,
                "ObOpenObjectByPointer gives a handle");
        t.Call(__imp__NtClose, { mem.Read32(handlePtr) });
        t.Call(__imp__ObDereferenceObject, { mem.Read32(threadPtr) });
        t.Check(t.Call(__imp__ObLookupThreadByThreadId, { 0x7FFFFFF, threadPtr }) == 0xC000000D, "ObLookupThreadByThreadId: unknown id");

        const uint32_t basePtr = Alloc(4), sizePtr = Alloc(4), oldPtr = Alloc(4);
        mem.Write32(basePtr, 0);
        mem.Write32(sizePtr, 0x3000);
        t.Check(t.Call(__imp__NtAllocateVirtualMemory, { basePtr, sizePtr, 0x3000, 4, 0 }) == 0, "NtAllocateVirtualMemory (3 pages)");
        const uint32_t pages = mem.Read32(basePtr);
        mem.Write32(basePtr, pages + 0x1010);
        mem.Write32(sizePtr, 0x10);
        const uint32_t protect = t.Call(__imp__NtProtectVirtualMemory, { basePtr, sizePtr, 2, oldPtr, 0 });
        const uint32_t pBase = mem.Read32(basePtr), pSize = mem.Read32(sizePtr);
        t.Check(protect == 0 && pBase <= pages + 0x1010 && pBase % 0x1000 == 0 && pSize % 0x1000 == 0 &&
                pBase + pSize >= pages + 0x1020 && mem.Read32(oldPtr) == 4,
                std::format("NtProtectVirtualMemory: page-rounded, old protection PAGE_READWRITE ({:X} {:X} {:X})",
                            mem.Read32(basePtr) - pages, mem.Read32(sizePtr), mem.Read32(oldPtr)));
        mem.Write32(basePtr, pages);
        mem.Write32(sizePtr, 0);
        t.Call(__imp__NtFreeVirtualMemory, { basePtr, sizePtr, 0x8000, 0 });

        // A flash file: its volume (read-only), and a delete there is refused.
        auto attributes = [&](const char* path) {
            const uint32_t ansi = Alloc(8), oa = Alloc(12);
            t.Call(__imp__RtlInitAnsiString, { ansi, cstr(path) });
            mem.Write32(oa + 0, 0);
            mem.Write32(oa + 4, ansi);
            mem.Write32(oa + 8, 0x40);
            return oa;
        };
        const uint32_t iosb = Alloc(8), info = Alloc(0x200);
        const uint32_t openStatus = t.Call(__imp__NtOpenFile, { handlePtr, 0x80100000, attributes("\\SystemRoot\\xenonjklatin.xtt"), iosb, 1, 0x40 });
        t.Check(openStatus == 0, std::format("NtOpenFile of a flash file ({:08X})", openStatus));
        const uint32_t file = mem.Read32(handlePtr);
        const uint32_t sizeStatus = t.Call(__imp__NtQueryVolumeInformationFile, { file, iosb, info, 0x18, 3 });
        t.Check(sizeStatus == 0 &&
                mem.Read64(info) > mem.Read64(info + 8) && mem.Read64(info + 8) > 0 && mem.Read32(info + 0x10) == 32 &&
                mem.Read32(info + 0x14) == 512, std::format("volume size: {} of {} clusters free, 16 KiB clusters",
                mem.Read64(info + 8), mem.Read64(info)));
        const uint32_t volumeStatus = t.Call(__imp__NtQueryVolumeInformationFile, { file, iosb, info, 0x100, 1 });
        t.Check(volumeStatus == 0 &&
                mem.Read32(iosb + 4) == 0x11 + mem.Read32(info + 0x0C), std::format("volume information: label of {} bytes",
                mem.Read32(info + 0x0C)));
        t.Check(t.Call(__imp__NtQueryVolumeInformationFile, { file, iosb, info, 0x10F, 5 }) == 0 && mem.Read32(info + 8) == 4 &&
                std::string(reinterpret_cast<const char*>(mem.Translate(info + 0x0C)), 4) == "FATX", "file system: FATX");
        t.Check(t.Call(__imp__NtQueryVolumeInformationFile, { file, iosb, info, 0x0E, 5 }) == 0x80000005,
                "file system name in a short buffer: STATUS_BUFFER_OVERFLOW");
        t.Call(__imp__NtClose, { file });
        t.Check(t.Call(__imp__NtDeleteFile, { attributes("\\SystemRoot\\xenonjklatin.xtt") }) == 0xC0000022,
                "NtDeleteFile on the read-only flash: access denied");
        t.Check(t.Call(__imp__NtDeleteFile, { attributes("\\SystemRoot\\no_such_file.bin") }) == 0xC0000034,
                "NtDeleteFile of a missing file: not found");

        // ---- XAM strings from xam.xex's tables ----
        auto wideAt = [&](uint32_t p) {
            std::string out;
            for (uint32_t i = 0; i < 64; i++)
            {
                const uint16_t c = mem.Read16(p + i * 2);
                if (c == 0)
                    break;
                out += char(c);
            }
            return out;
        };
        const uint32_t text = Alloc(128);
        t.Check(t.Call(__imp__XamGetLanguageLocaleString, { 1, 36, 32, text }) == 0 && wideAt(text) == "en-US",
                "XamGetLanguageLocaleString(English, United States) = en-US");
        t.Check(t.Call(__imp__XamGetLanguageLocaleString, { 2, 20, 32, text }) == 0 && wideAt(text) == "ja-JP",
                "XamGetLanguageLocaleString(Japanese, Japan) = ja-JP");
        t.Check(t.Call(__imp__XamGetLanguageLocaleString, { 1, 36, 4, text }) == 0x8007007A && wideAt(text) == "en-",
                "a 4-character buffer: en- and STRSAFE_E_INSUFFICIENT_BUFFER");
        t.Check(t.Call(__imp__XamGetLocaleString, { 35, 8, text }) == 0 && wideAt(text) == "GB", "XamGetLocaleString(35) = GB");
        t.Check(t.Call(__imp__XamGetOnlineCountryFromLocale, { 36 }) == 103 && t.Call(__imp__XamGetOnlineCountryFromLocale, { 35 }) == 35,
                "XamGetOnlineCountryFromLocale: US 103, GB 35 (the table xam_info.cpp already had)");
        t.Check(t.Call(__imp__XamGetOnlineLanguageAndCountryString, { 1, 103, 32, text }) == 0 && wideAt(text) == "en-US",
                "XamGetOnlineLanguageAndCountryString(1, 103) = en-US");

        Log("Kernel tests: {}", t.failures ? std::format("{} FAILED", t.failures) : "all passed");
        return t.failures;
    }
}

namespace kernel
{
    int RunKernelTests()
    {
        std::atomic<int> result = -1;
        StartHostThread([&result] {
            ThreadParams params;
            params.stackSize = 0x20000;
            GuestThread* thread = CreateThread(params);
            PPCContext* ctx = AttachHostThread(thread);
            result = Run(*ctx, guest::g_memory.base);
        });
        while (result < 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        return result;
    }
}
