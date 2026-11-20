// recompile/project_host_impl.inc -- HOST_IMPL: the host implementation of each tractable named import, as the statements of its
// body in the recompiled ABI (args in ctx.r3, ctx.r4, ...; result in ctx.r3; guest memory is `base + guest_addr`). Included by
// project.cpp only.

const std::map<std::string, std::vector<std::string>, std::less<>>& host_impl() {
    static const std::map<std::string, std::vector<std::string>, std::less<>> table = {
        {"RtlInitializeCriticalSection", {R"CBX(ctx.r3.s64 = 0;)CBX"}},
        {"RtlInitializeCriticalSectionAndSpinCount", {R"CBX(ctx.r3.s64 = 0;)CBX"}},
        {"RtlEnterCriticalSection", {R"CBX(/* single-thread bring-up: no-op */)CBX"}},
        {"RtlLeaveCriticalSection", {R"CBX(/* single-thread bring-up: no-op */)CBX"}},
        {"RtlTryEnterCriticalSection", {R"CBX(ctx.r3.s64 = 1;)CBX"}},
        {"RtlDeleteCriticalSection", {R"CBX(/* no-op */)CBX"}},
        {"RtlZeroMemory", {R"CBX(std::memset(base + ctx.r3.u32, 0, ctx.r4.u32);)CBX"}},
        {"RtlFillMemory", {R"CBX(std::memset(base + ctx.r3.u32, (int)ctx.r5.u32, ctx.r4.u32);)CBX"}},
        {"RtlCopyMemory", {R"CBX(std::memcpy(base + ctx.r3.u32, base + ctx.r4.u32, ctx.r5.u32);)CBX"}},
        {"RtlCompareMemory", {R"CBX(uint32_t n = ctx.r5.u32, i = 0;)CBX", R"CBX(const uint8_t* a = base + ctx.r3.u32; const uint8_t* b = base + ctx.r4.u32;)CBX", R"CBX(while (i < n && a[i] == b[i]) ++i; ctx.r3.u64 = i;)CBX"}},
        {"ExAllocatePool", {R"CBX(ctx.r3.u64 = rex_heap_alloc(ctx.r3.u32);)CBX"}},
        {"ExAllocatePoolWithTag", {R"CBX(ctx.r3.u64 = rex_heap_alloc(ctx.r3.u32);)CBX"}},
        {"ExAllocatePoolTypeWithTag", {R"CBX(ctx.r3.u64 = rex_heap_alloc(ctx.r4.u32);)CBX"}},
        {"ExFreePool", {R"CBX(/* bump heap: no free */)CBX"}},
        {"RtlAllocateHeap", {R"CBX(ctx.r3.u64 = rex_heap_alloc(ctx.r5.u32);)CBX"}},
        {"RtlReAllocateHeap", {R"CBX(ctx.r3.u64 = rex_heap_alloc(ctx.r6.u32);)CBX"}},
        {"RtlFreeHeap", {R"CBX(ctx.r3.s64 = 1;)CBX"}},
        {"MmAllocatePhysicalMemory", {R"CBX(ctx.r3.u64 = rex_heap_alloc(ctx.r4.u32);)CBX"}},
        {"MmAllocatePhysicalMemoryEx", {R"CBX(ctx.r3.u64 = rex_heap_alloc(ctx.r4.u32);)CBX"}},
        {"MmFreePhysicalMemory", {R"CBX(/* bump heap: no free */)CBX"}},
        {"MmGetPhysicalAddress", {R"CBX(ctx.r3.u64 = ctx.r3.u32 & 0x1FFFFFFFu;)CBX"}},
        {"DbgPrint", {R"CBX(std::fprintf(stderr, "%s", (const char*)(base + ctx.r3.u32));)CBX"}},
        {"KeBugCheck", {R"CBX(std::fprintf(stderr, "[KeBugCheck] 0x%X\n", ctx.r3.u32); std::abort();)CBX"}},
        {"KeBugCheckEx", {R"CBX(std::fprintf(stderr, "[KeBugCheckEx] 0x%X\n", ctx.r3.u32); std::abort();)CBX"}},
        {"KeQueryPerformanceCounter", {R"CBX(static uint64_t t = 0; ctx.r3.u64 = (t += 1000);   // monotonic stub)CBX"}},
        {"KeQueryPerformanceFrequency", {R"CBX(ctx.r3.u64 = 50000000;   // Xenos 50 MHz timebase)CBX"}},
        {"KeQuerySystemTime", {R"CBX(rex_store64(base, ctx.r3.u32, rex_system_time_100ns());)CBX"}},
        {"KeDelayExecutionThread", {R"CBX(ctx.r3.u64 = 0;   // STATUS_SUCCESS, no real wait)CBX"}},
        {"KeGetCurrentProcessType", {R"CBX(ctx.r3.u64 = 1;   // title process)CBX"}},
        {"XexCheckExecutablePrivilege", {R"CBX(ctx.r3.u64 = 0;   // privilege not held)CBX"}},
        {"RtlImageXexHeaderField", {R"CBX(ctx.r3.u64 = 0;)CBX"}},
        {"KeEnableInterrupts", {R"CBX(ctx.r3.u64 = 1;)CBX"}},
        {"KeDisableInterrupts", {R"CBX(ctx.r3.u64 = 1;)CBX"}},
        {"KeLockL2", {R"CBX(ctx.r3.u64 = 0;)CBX"}},
        {"KeUnlockL2", {R"CBX(/* no-op */)CBX"}},
        {"DbgBreakPoint", {R"CBX(/* no-op (no debugger attached) */)CBX"}},
        {"HalReturnToFirmware", {R"CBX(std::fprintf(stderr, "[HalReturnToFirmware] guest requested reboot\n"); std::exit(0);)CBX"}},
        {"NtClose", {R"CBX(ctx.r3.u64 = 0;   // STATUS_SUCCESS)CBX"}},
        {"ObDereferenceObject", {R"CBX(/* no-op (no object manager yet) */)CBX"}},
    };
    return table;
}
