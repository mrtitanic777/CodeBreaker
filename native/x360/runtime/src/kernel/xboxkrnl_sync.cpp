// xboxkrnl synchronization exports: critical sections, events, semaphores, waits, handles.

#include "kernel/objects.h"

#include <windows.h>

#include <functional>
#include <thread>
#include <vector>

using namespace kernel;

namespace
{
    // RTL_CRITICAL_SECTION (28 bytes): a synchronization-event DISPATCHER_HEADER (its Absolute byte holds the spin
    // count / 256), then LockCount (-1 = free), RecursionCount and OwningThread (KTHREAD).
    constexpr uint32_t kCsLockCount = 0x10;
    constexpr uint32_t kCsRecursionCount = 0x14;
    constexpr uint32_t kCsOwningThread = 0x18;

    uint32_t CurrentKThread(const PPCContext& ctx)
    {
        return guest::g_memory.Read32(ctx.r13.u32 + pcr::kCurrentThread);
    }

    void InitializeCriticalSection(uint32_t cs, uint32_t spinCount)
    {
        auto& mem = guest::g_memory;
        InitializeDispatcherHeader(cs, kObjectEventSynchronization, 0);
        mem.Write8(cs + header::kAbsolute, uint8_t(std::min<uint32_t>((spinCount + 255) >> 8, 0xFF)));
        mem.Write32(cs + kCsLockCount, 0xFFFFFFFF);
        mem.Write32(cs + kCsRecursionCount, 0);
        mem.Write32(cs + kCsOwningThread, 0);
    }

    // Signals a synchronization or notification event (dispatcher lock taken here). Returns the previous state.
    int32_t SetEvent(uint32_t event)
    {
        auto lock = LockDispatcher();
        const int32_t previous = int32_t(guest::g_memory.Read32(event + header::kSignalState));
        guest::g_memory.Write32(event + header::kSignalState, 1);
        NotifyDispatcherChange(event);
        return previous;
    }

    int32_t ResetEvent(uint32_t event)
    {
        auto lock = LockDispatcher();
        const int32_t previous = int32_t(guest::g_memory.Read32(event + header::kSignalState));
        guest::g_memory.Write32(event + header::kSignalState, 0);
        return previous;
    }

    bool HasType(uint32_t object, std::initializer_list<uint8_t> types)
    {
        const uint8_t type = guest::g_memory.Read8(object + header::kType);
        for (uint8_t t : types)
            if (t == type)
                return true;
        return false;
    }

    // Creates (or opens, if named and existing) an owned dispatcher object and returns a handle to it.
    uint32_t CreateNamedObject(PPCContext& ctx, uint32_t handlePtr, uint32_t objectAttributes, uint32_t size,
        const std::function<void(uint32_t)>& initialize)
    {
        auto& mem = guest::g_memory;
        const std::string name = ReadObjectName(objectAttributes);
        if (!name.empty())
        {
            if (uint32_t existing = LookupNamedObject(name))
            {
                mem.Write32(handlePtr, CreateHandle(existing));
                return kStatusObjectNameExists;
            }
        }
        const uint32_t object = CreateOwnedObject(size);
        initialize(object);
        if (!name.empty())
            RegisterNamedObject(name, object);
        mem.Write32(handlePtr, CreateHandle(object));
        return kStatusSuccess;
    }
}

// ---- Critical sections -----------------------------------------------------------------------------------------

// VOID RtlInitializeCriticalSection(PRTL_CRITICAL_SECTION CriticalSection)
GUEST_IMPORT(RtlInitializeCriticalSection)
{
    InitializeCriticalSection(ctx.r3.u32, 0);
}

// NTSTATUS RtlInitializeCriticalSectionAndSpinCount(PRTL_CRITICAL_SECTION CriticalSection, ULONG SpinCount)
GUEST_IMPORT(RtlInitializeCriticalSectionAndSpinCount)
{
    InitializeCriticalSection(ctx.r3.u32, ctx.r4.u32);
    ctx.r3.u64 = kStatusSuccess;
}

// VOID RtlEnterCriticalSection(PRTL_CRITICAL_SECTION CriticalSection)
GUEST_IMPORT(RtlEnterCriticalSection)
{
    auto& mem = guest::g_memory;
    const uint32_t cs = ctx.r3.u32;
    const uint32_t thread = CurrentKThread(ctx);

    // Spin (SpinCount from initialization) trying to take a free lock before falling back to the queued path.
    for (uint32_t spin = uint32_t(mem.Read8(cs + header::kAbsolute)) * 256; spin > 0; spin--)
    {
        if (mem.CompareExchange32(cs + kCsLockCount, 0xFFFFFFFF, 0))
        {
            mem.Write32(cs + kCsOwningThread, thread);
            mem.Write32(cs + kCsRecursionCount, 1);
            return;
        }
        YieldProcessor();
    }

    if (mem.AtomicAdd32(cs + kCsLockCount, 1) != 0)
    {
        if (mem.Read32(cs + kCsOwningThread) == thread)
        {
            mem.Write32(cs + kCsRecursionCount, mem.Read32(cs + kCsRecursionCount) + 1);
            return;
        }
        // Contended: wait for the releasing thread to signal the critical section's event.
        const uint32_t object = cs;
        WaitForObjects(&object, 1, false, 0);
    }
    mem.Write32(cs + kCsOwningThread, thread);
    mem.Write32(cs + kCsRecursionCount, 1);
}

// VOID RtlLeaveCriticalSection(PRTL_CRITICAL_SECTION CriticalSection)
GUEST_IMPORT(RtlLeaveCriticalSection)
{
    auto& mem = guest::g_memory;
    const uint32_t cs = ctx.r3.u32;
    const uint32_t recursion = mem.Read32(cs + kCsRecursionCount) - 1;
    mem.Write32(cs + kCsRecursionCount, recursion);
    if (recursion != 0)
    {
        mem.AtomicAdd32(cs + kCsLockCount, -1);
        return;
    }
    mem.Write32(cs + kCsOwningThread, 0);
    if (int32_t(mem.AtomicAdd32(cs + kCsLockCount, -1)) >= 0)
        SetEvent(cs); // another thread is waiting: hand the lock over
}

// BOOL RtlTryEnterCriticalSection(PRTL_CRITICAL_SECTION CriticalSection)
GUEST_IMPORT(RtlTryEnterCriticalSection)
{
    auto& mem = guest::g_memory;
    const uint32_t cs = ctx.r3.u32;
    const uint32_t thread = CurrentKThread(ctx);
    if (mem.CompareExchange32(cs + kCsLockCount, 0xFFFFFFFF, 0))
    {
        mem.Write32(cs + kCsOwningThread, thread);
        mem.Write32(cs + kCsRecursionCount, 1);
        ctx.r3.u64 = 1;
    }
    else if (mem.Read32(cs + kCsOwningThread) == thread)
    {
        mem.AtomicAdd32(cs + kCsLockCount, 1);
        mem.Write32(cs + kCsRecursionCount, mem.Read32(cs + kCsRecursionCount) + 1);
        ctx.r3.u64 = 1;
    }
    else
    {
        ctx.r3.u64 = 0;
    }
}

// ---- Events ------------------------------------------------------------------------------------------------------

// VOID KeInitializeEvent(PRKEVENT Event, EVENT_TYPE Type, BOOLEAN State): the caller's KEVENT.
// Type: 0 = notification (manual reset), 1 = synchronization.
GUEST_IMPORT(KeInitializeEvent)
{
    InitializeDispatcherHeader(ctx.r3.u32, ctx.r4.u32 == 0 ? kObjectEventNotification : kObjectEventSynchronization,
        (ctx.r5.u32 & 0xFF) ? 1 : 0);
}

// LONG KeSetEvent(PKEVENT Event, KPRIORITY Increment, BOOLEAN Wait)
GUEST_IMPORT(KeSetEvent)
{
    ctx.r3.u64 = uint32_t(SetEvent(ctx.r3.u32));
}

// LONG KeResetEvent(PKEVENT Event)
GUEST_IMPORT(KeResetEvent)
{
    ctx.r3.u64 = uint32_t(ResetEvent(ctx.r3.u32));
}

// NTSTATUS NtCreateEvent(PHANDLE EventHandle, POBJECT_ATTRIBUTES ObjectAttributes, EVENT_TYPE EventType,
//                        BOOLEAN InitialState)   EventType: 0 = notification (manual reset), 1 = synchronization
GUEST_IMPORT(NtCreateEvent)
{
    const uint8_t type = ctx.r5.u32 == 0 ? kObjectEventNotification : kObjectEventSynchronization;
    const int32_t initial = ctx.r6.u32 ? 1 : 0;
    ctx.r3.u64 = CreateNamedObject(ctx, ctx.r3.u32, ctx.r4.u32, kKEventSize,
        [&](uint32_t object) { InitializeDispatcherHeader(object, type, initial); });
}

// NTSTATUS NtSetEvent(HANDLE EventHandle, PLONG PreviousState)
GUEST_IMPORT(NtSetEvent)
{
    const uint32_t event = LookupHandle(ctx.r3.u32);
    if (event == 0 || !HasType(event, { kObjectEventNotification, kObjectEventSynchronization }))
    {
        ctx.r3.u64 = event ? kStatusObjectTypeMismatch : kStatusInvalidHandle;
        return;
    }
    const int32_t previous = SetEvent(event);
    if (ctx.r4.u32)
        guest::g_memory.Write32(ctx.r4.u32, uint32_t(previous));
    ctx.r3.u64 = kStatusSuccess;
}

// NTSTATUS NtClearEvent(HANDLE EventHandle)
GUEST_IMPORT(NtClearEvent)
{
    const uint32_t event = LookupHandle(ctx.r3.u32);
    if (event == 0 || !HasType(event, { kObjectEventNotification, kObjectEventSynchronization }))
    {
        ctx.r3.u64 = event ? kStatusObjectTypeMismatch : kStatusInvalidHandle;
        return;
    }
    ResetEvent(event);
    ctx.r3.u64 = kStatusSuccess;
}

// ---- Semaphores --------------------------------------------------------------------------------------------------

// ---- Mutants --------------------------------------------------------------------------------------------------
// KMUTANT: SignalState 1 = free; each acquisition by the owner takes one off (objects.cpp Satisfy), each release
// gives one back; at 1 again it has no owner.

// NTSTATUS NtCreateMutant(PHANDLE MutantHandle, POBJECT_ATTRIBUTES ObjectAttributes, BOOLEAN InitialOwner)
GUEST_IMPORT(NtCreateMutant)
{
    const bool owned = (ctx.r5.u32 & 0xFF) != 0;
    const GuestThread* self = CurrentThread();
    const uint32_t owner = owned && self ? self->threadObject : 0;
    ctx.r3.u64 = CreateNamedObject(ctx, ctx.r3.u32, ctx.r4.u32, kKMutantSize,
        [&](uint32_t object)
        {
            InitializeDispatcherHeader(object, kObjectMutant, owned ? 0 : 1);
            guest::g_memory.Write32(object + kKMutantOwner, owner);
            guest::g_memory.Write8(object + kKMutantAbandoned, 0);
        });
}

// NTSTATUS NtReleaseMutant(HANDLE MutantHandle, PLONG PreviousCount): STATUS_MUTANT_NOT_OWNED unless the caller owns it.
GUEST_IMPORT(NtReleaseMutant)
{
    constexpr uint32_t kStatusMutantNotOwned = 0xC0000046;
    auto& mem = guest::g_memory;
    const uint32_t mutant = LookupHandle(ctx.r3.u32);
    if (mutant == 0 || !HasType(mutant, { kObjectMutant }))
    {
        ctx.r3.u64 = mutant ? kStatusObjectTypeMismatch : kStatusInvalidHandle;
        return;
    }
    const GuestThread* self = CurrentThread();
    auto lock = LockDispatcher();
    if (!self || mem.Read32(mutant + kKMutantOwner) != self->threadObject)
    {
        ctx.r3.u64 = kStatusMutantNotOwned;
        return;
    }
    const int32_t previous = int32_t(mem.Read32(mutant + header::kSignalState));
    mem.Write32(mutant + header::kSignalState, uint32_t(previous + 1));
    if (previous + 1 > 0)
    {
        mem.Write32(mutant + kKMutantOwner, 0);
        NotifyDispatcherChange(mutant);
    }
    if (const uint32_t previousPtr = ctx.r4.u32)
        mem.Write32(previousPtr, uint32_t(previous));
    ctx.r3.u64 = kStatusSuccess;
}

// NTSTATUS NtCreateSemaphore(PHANDLE SemaphoreHandle, POBJECT_ATTRIBUTES ObjectAttributes, LONG InitialCount,
//                            LONG MaximumCount)
GUEST_IMPORT(NtCreateSemaphore)
{
    const int32_t initial = int32_t(ctx.r5.u32);
    const int32_t limit = int32_t(ctx.r6.u32);
    if (limit <= 0 || initial < 0 || initial > limit)
    {
        ctx.r3.u64 = kStatusInvalidParameter;
        return;
    }
    ctx.r3.u64 = CreateNamedObject(ctx, ctx.r3.u32, ctx.r4.u32, kKSemaphoreSize,
        [&](uint32_t object)
        {
            InitializeDispatcherHeader(object, kObjectSemaphore, initial);
            guest::g_memory.Write32(object + kKSemaphoreLimit, uint32_t(limit));
        });
}

// NTSTATUS NtReleaseSemaphore(HANDLE SemaphoreHandle, LONG ReleaseCount, PLONG PreviousCount)
GUEST_IMPORT(NtReleaseSemaphore)
{
    auto& mem = guest::g_memory;
    const uint32_t semaphore = LookupHandle(ctx.r3.u32);
    if (semaphore == 0 || !HasType(semaphore, { kObjectSemaphore }))
    {
        ctx.r3.u64 = semaphore ? kStatusObjectTypeMismatch : kStatusInvalidHandle;
        return;
    }
    const int32_t release = int32_t(ctx.r4.u32);
    auto lock = LockDispatcher();
    const int32_t previous = int32_t(mem.Read32(semaphore + header::kSignalState));
    const int32_t limit = int32_t(mem.Read32(semaphore + kKSemaphoreLimit));
    if (release <= 0 || int64_t(previous) + release > limit)
    {
        ctx.r3.u64 = kStatusSemaphoreLimitExceeded;
        return;
    }
    mem.Write32(semaphore + header::kSignalState, uint32_t(previous + release));
    NotifyDispatcherChange(semaphore);
    lock.unlock();
    if (ctx.r5.u32)
        mem.Write32(ctx.r5.u32, uint32_t(previous));
    ctx.r3.u64 = kStatusSuccess;
}

// ---- Waits -------------------------------------------------------------------------------------------------------
// Alertable user-mode waits (WaitMode UserMode = 1) return STATUS_USER_APC when the thread has user APCs queued,
// after running them, as NT does. The only user-APC source is XAM's XOVERLAPPED completion routines.

namespace
{
    constexpr uint32_t kUserMode = 1;

    // Runs queued user APCs if the wait was interrupted for them.
    uint32_t AfterWait(PPCContext& ctx, uint32_t status)
    {
        if (status == kStatusUserApc)
            DeliverUserApcs(ctx);
        return status;
    }
}

// NTSTATUS KeWaitForSingleObject(PVOID Object, KWAIT_REASON WaitReason, KPROCESSOR_MODE WaitMode, BOOLEAN Alertable,
//                                PLARGE_INTEGER Timeout)
GUEST_IMPORT(KeWaitForSingleObject)
{
    const uint32_t object = ctx.r3.u32;
    const bool alertable = ctx.r5.u32 == kUserMode && (ctx.r6.u32 & 0xFF) != 0;
    ctx.r3.u64 = AfterWait(ctx, WaitForObjects(&object, 1, false, ctx.r7.u32, alertable));
}

// NTSTATUS KeWaitForMultipleObjects(ULONG Count, PVOID Object[], WAIT_TYPE WaitType, KWAIT_REASON WaitReason,
//                                   KPROCESSOR_MODE WaitMode, BOOLEAN Alertable, PLARGE_INTEGER Timeout,
//                                   PKWAIT_BLOCK WaitBlockArray)   WaitType: 0 = WaitAll, 1 = WaitAny
GUEST_IMPORT(KeWaitForMultipleObjects)
{
    auto& mem = guest::g_memory;
    const uint32_t count = ctx.r3.u32;
    std::vector<uint32_t> objects(count);
    for (uint32_t i = 0; i < count; i++)
        objects[i] = mem.Read32(ctx.r4.u32 + i * 4);
    const bool alertable = ctx.r7.u32 == kUserMode && (ctx.r8.u32 & 0xFF) != 0;
    ctx.r3.u64 = AfterWait(ctx, WaitForObjects(objects.data(), count, ctx.r5.u32 == 0, ctx.r9.u32, alertable));
}

// NTSTATUS NtWaitForSingleObjectEx(HANDLE Handle, KPROCESSOR_MODE WaitMode, BOOLEAN Alertable, PLARGE_INTEGER Timeout)
GUEST_IMPORT(NtWaitForSingleObjectEx)
{
    const uint32_t object = LookupHandle(ctx.r3.u32);
    if (object == 0)
    {
        ctx.r3.u64 = kStatusInvalidHandle;
        return;
    }
    const bool alertable = ctx.r4.u32 == kUserMode && (ctx.r5.u32 & 0xFF) != 0;
    ctx.r3.u64 = AfterWait(ctx, WaitForObjects(&object, 1, false, ctx.r6.u32, alertable));
}

// NTSTATUS NtWaitForMultipleObjectsEx(ULONG Count, PHANDLE Handles, WAIT_TYPE WaitType, KPROCESSOR_MODE WaitMode,
//                                     BOOLEAN Alertable, PLARGE_INTEGER Timeout)
GUEST_IMPORT(NtWaitForMultipleObjectsEx)
{
    auto& mem = guest::g_memory;
    const uint32_t count = ctx.r3.u32;
    std::vector<uint32_t> objects(count);
    for (uint32_t i = 0; i < count; i++)
    {
        objects[i] = LookupHandle(mem.Read32(ctx.r4.u32 + i * 4));
        if (objects[i] == 0)
        {
            ctx.r3.u64 = kStatusInvalidHandle;
            return;
        }
    }
    const bool alertable = ctx.r6.u32 == kUserMode && (ctx.r7.u32 & 0xFF) != 0;
    ctx.r3.u64 = AfterWait(ctx, WaitForObjects(objects.data(), count, ctx.r5.u32 == 0, ctx.r8.u32, alertable));
}

// NTSTATUS KeDelayExecutionThread(KPROCESSOR_MODE WaitMode, BOOLEAN Alertable, PLARGE_INTEGER Interval)
GUEST_IMPORT(KeDelayExecutionThread)
{
    const auto deadline = TimeoutToDeadline(ctx.r5.u32);
    const bool alertable = ctx.r3.u32 == kUserMode && (ctx.r4.u32 & 0xFF) != 0;
    uint32_t status;
    if (!deadline || *deadline <= std::chrono::steady_clock::now())
    {
        status = DelayExecution(std::chrono::steady_clock::now(), alertable);
        if (status != kStatusUserApc)
            SwitchToThread(); // zero interval: give up the rest of the time slice
    }
    else
        status = DelayExecution(deadline, alertable);
    ctx.r3.u64 = AfterWait(ctx, status);
}

// ---- Handles and object references -------------------------------------------------------------------------------

// NTSTATUS NtClose(HANDLE Handle)
GUEST_IMPORT(NtClose)
{
    ctx.r3.u64 = CloseHandle(ctx.r3.u32) ? kStatusSuccess : kStatusInvalidHandle;
}

// NTSTATUS NtDuplicateObject(HANDLE SourceHandle, PHANDLE TargetHandle, ULONG Options)  Options: 1 = close source
GUEST_IMPORT(NtDuplicateObject)
{
    const uint32_t source = ctx.r3.u32;
    const uint32_t object = LookupHandle(source);
    if (object == 0)
    {
        ctx.r3.u64 = kStatusInvalidHandle;
        return;
    }
    if (ctx.r4.u32)
        guest::g_memory.Write32(ctx.r4.u32, CreateHandle(object));
    if (ctx.r5.u32 & 1)
        CloseHandle(source);
    ctx.r3.u64 = kStatusSuccess;
}

// NTSTATUS ObReferenceObjectByHandle(HANDLE Handle, POBJECT_TYPE ObjectType, PVOID* Object)
// ObjectType is the address of an imported type descriptor (ExThreadObjectType, ExEventObjectType) or null for any.
GUEST_IMPORT(ObReferenceObjectByHandle)
{
    const uint32_t object = LookupHandle(ctx.r3.u32);
    const uint32_t objectType = ctx.r4.u32;
    if (object == 0)
    {
        ctx.r3.u64 = kStatusInvalidHandle;
        return;
    }
    if (objectType != 0)
    {
        bool matches = true;
        if (objectType == ResolveVariable("xboxkrnl.exe", 27)) // ExThreadObjectType
            matches = HasType(object, { kObjectThread });
        else if (objectType == ResolveVariable("xboxkrnl.exe", 14)) // ExEventObjectType
            matches = HasType(object, { kObjectEventNotification, kObjectEventSynchronization });
        else if (IsCreatedObjectType(objectType)) // a type xam.xex defined (ObCreateObject)
            matches = CreatedObjectType(object) == objectType;
        else
            Fatal("ObReferenceObjectByHandle with unknown object type descriptor {:08X}", objectType);
        if (!matches)
        {
            ctx.r3.u64 = kStatusObjectTypeMismatch;
            return;
        }
    }
    ReferenceObject(object);
    if (ctx.r5.u32)
        guest::g_memory.Write32(ctx.r5.u32, object);
    ctx.r3.u64 = kStatusSuccess;
}

// VOID ObDereferenceObject(PVOID Object)
GUEST_IMPORT(ObDereferenceObject)
{
    DereferenceObject(ctx.r3.u32);
}

// ---- Kernel-mode semaphores (KSEMAPHORE in caller memory) --------------------------------------------------------

// VOID KeInitializeSemaphore(PRKSEMAPHORE Semaphore, LONG Count, LONG Limit)
GUEST_IMPORT(KeInitializeSemaphore)
{
    InitializeDispatcherHeader(ctx.r3.u32, kObjectSemaphore, int32_t(ctx.r4.u32));
    guest::g_memory.Write32(ctx.r3.u32 + kKSemaphoreLimit, ctx.r5.u32);
}

// LONG KeReleaseSemaphore(PRKSEMAPHORE Semaphore, KPRIORITY Increment, LONG Adjustment, BOOLEAN Wait)
// Returns the previous count. Exceeding the limit raises STATUS_SEMAPHORE_LIMIT_EXCEEDED in NT (ExRaiseStatus);
// that is reported here instead. Wait = TRUE (the caller waits right after, without a context switch in between) only
// changes scheduling, not the result.
GUEST_IMPORT(KeReleaseSemaphore)
{
    auto& mem = guest::g_memory;
    const uint32_t semaphore = ctx.r3.u32;
    const int32_t adjustment = int32_t(ctx.r5.u32);
    auto lock = LockDispatcher();
    const int32_t previous = int32_t(mem.Read32(semaphore + header::kSignalState));
    const int32_t limit = int32_t(mem.Read32(semaphore + kKSemaphoreLimit));
    if (adjustment <= 0 || int64_t(previous) + adjustment > limit)
        Fatal("KeReleaseSemaphore({:08X}, adjustment {}) exceeds the limit {} (count {}): the kernel raises "
            "STATUS_SEMAPHORE_LIMIT_EXCEEDED here", semaphore, adjustment, limit, previous);
    mem.Write32(semaphore + header::kSignalState, uint32_t(previous + adjustment));
    NotifyDispatcherChange(semaphore);
    ctx.r3.u64 = uint32_t(previous);
}

// ---- Executive read/write locks (ERWLOCK in caller memory) -----------------------------------------------------
// ERWLOCK (0x38): LockCount (s32, -1 = free; holders + waiters - 1), WritersWaitingCount, ReadersWaitingCount,
// ReadersEntryCount (readers holding it), WriterEvent (synchronization KEVENT, +0x10), ReaderSemaphore (KSEMAPHORE,
// +0x20), SpinLock (+0x34). Like NT's ERESOURCE-lite scheme: a releasing writer hands the lock to all waiting readers
// at once, else to one waiting writer; the last reader out wakes one writer. The counters change under one host
// mutex (the kernel's spin lock); the wake-ups go through the dispatcher like KeSetEvent / KeReleaseSemaphore, and a
// waiter that has not started waiting yet still finds the event / semaphore signalled. Xenia Canary's release
// (reference) decrements ReadersEntryCount below zero when a writer leaves with only writers waiting, so it is not
// followed there.

namespace
{
    constexpr uint32_t kRwLockCount = 0x00;
    constexpr uint32_t kRwWritersWaiting = 0x04;
    constexpr uint32_t kRwReadersWaiting = 0x08;
    constexpr uint32_t kRwReadersEntry = 0x0C;
    constexpr uint32_t kRwWriterEvent = 0x10;
    constexpr uint32_t kRwReaderSemaphore = 0x20;
    constexpr uint32_t kRwSpinLock = 0x34;

    std::mutex g_rwLockMutex;

    void WaitForever(uint32_t object)
    {
        WaitForObjects(&object, 1, false, 0);
    }

    void ReleaseReaders(uint32_t semaphore, uint32_t count)
    {
        auto& mem = guest::g_memory;
        auto lock = LockDispatcher();
        mem.Write32(semaphore + header::kSignalState, mem.Read32(semaphore + header::kSignalState) + count);
        NotifyDispatcherChange(semaphore);
    }
}

// VOID ExInitializeReadWriteLock(PERWLOCK ReadWriteLock)
GUEST_IMPORT(ExInitializeReadWriteLock)
{
    auto& mem = guest::g_memory;
    const uint32_t rw = ctx.r3.u32;
    mem.Write32(rw + kRwLockCount, 0xFFFFFFFF);
    mem.Write32(rw + kRwWritersWaiting, 0);
    mem.Write32(rw + kRwReadersWaiting, 0);
    mem.Write32(rw + kRwReadersEntry, 0);
    InitializeDispatcherHeader(rw + kRwWriterEvent, kObjectEventSynchronization, 0);
    InitializeDispatcherHeader(rw + kRwReaderSemaphore, kObjectSemaphore, 0);
    mem.Write32(rw + kRwReaderSemaphore + kKSemaphoreLimit, 0x7FFFFFFF);
    mem.Write32(rw + kRwSpinLock, 0);
}

// VOID ExAcquireReadWriteLockExclusive(PERWLOCK ReadWriteLock)
GUEST_IMPORT(ExAcquireReadWriteLockExclusive)
{
    auto& mem = guest::g_memory;
    const uint32_t rw = ctx.r3.u32;
    {
        std::lock_guard lock(g_rwLockMutex);
        const int32_t count = int32_t(mem.Read32(rw + kRwLockCount)) + 1;
        mem.Write32(rw + kRwLockCount, uint32_t(count));
        if (count == 0)
            return;
        mem.Write32(rw + kRwWritersWaiting, mem.Read32(rw + kRwWritersWaiting) + 1);
    }
    WaitForever(rw + kRwWriterEvent);
}

// BOOLEAN ExTryToAcquireReadWriteLockExclusive(PERWLOCK ReadWriteLock)
GUEST_IMPORT(ExTryToAcquireReadWriteLockExclusive)
{
    auto& mem = guest::g_memory;
    const uint32_t rw = ctx.r3.u32;
    std::lock_guard lock(g_rwLockMutex);
    const bool free = int32_t(mem.Read32(rw + kRwLockCount)) < 0;
    if (free)
        mem.Write32(rw + kRwLockCount, 0);
    ctx.r3.u64 = free ? 1 : 0;
}

// VOID ExAcquireReadWriteLockShared(PERWLOCK ReadWriteLock): joins other readers unless a writer waits.
GUEST_IMPORT(ExAcquireReadWriteLockShared)
{
    auto& mem = guest::g_memory;
    const uint32_t rw = ctx.r3.u32;
    {
        std::lock_guard lock(g_rwLockMutex);
        const int32_t count = int32_t(mem.Read32(rw + kRwLockCount)) + 1;
        mem.Write32(rw + kRwLockCount, uint32_t(count));
        const uint32_t readers = mem.Read32(rw + kRwReadersEntry);
        if (count == 0 || (readers != 0 && mem.Read32(rw + kRwWritersWaiting) == 0))
        {
            mem.Write32(rw + kRwReadersEntry, readers + 1);
            return;
        }
        mem.Write32(rw + kRwReadersWaiting, mem.Read32(rw + kRwReadersWaiting) + 1);
    }
    // The writer that releases it counts this reader into ReadersEntryCount before signalling.
    WaitForever(rw + kRwReaderSemaphore);
}

// BOOLEAN ExTryToAcquireReadWriteLockShared(PERWLOCK ReadWriteLock)
GUEST_IMPORT(ExTryToAcquireReadWriteLockShared)
{
    auto& mem = guest::g_memory;
    const uint32_t rw = ctx.r3.u32;
    std::lock_guard lock(g_rwLockMutex);
    const int32_t count = int32_t(mem.Read32(rw + kRwLockCount));
    const uint32_t readers = mem.Read32(rw + kRwReadersEntry);
    const bool ok = count < 0 || (readers != 0 && mem.Read32(rw + kRwWritersWaiting) == 0);
    if (ok)
    {
        mem.Write32(rw + kRwLockCount, uint32_t(count + 1));
        mem.Write32(rw + kRwReadersEntry, readers + 1);
    }
    ctx.r3.u64 = ok ? 1 : 0;
}

// VOID ExReleaseReadWriteLock(PERWLOCK ReadWriteLock): releases a shared or exclusive hold (ReadersEntryCount says
// which: 0 = the writer).
GUEST_IMPORT(ExReleaseReadWriteLock)
{
    auto& mem = guest::g_memory;
    const uint32_t rw = ctx.r3.u32;
    std::unique_lock lock(g_rwLockMutex);
    const int32_t count = int32_t(mem.Read32(rw + kRwLockCount)) - 1;
    mem.Write32(rw + kRwLockCount, uint32_t(count));
    if (count < 0)
    {
        mem.Write32(rw + kRwReadersEntry, 0); // nobody holds or waits
        return;
    }
    const uint32_t readersEntry = mem.Read32(rw + kRwReadersEntry);
    if (readersEntry == 0)
    {
        // A writer leaves: all waiting readers enter together, else one writer.
        if (const uint32_t readersWaiting = mem.Read32(rw + kRwReadersWaiting))
        {
            mem.Write32(rw + kRwReadersWaiting, 0);
            mem.Write32(rw + kRwReadersEntry, readersWaiting);
            lock.unlock();
            ReleaseReaders(rw + kRwReaderSemaphore, readersWaiting);
            return;
        }
    }
    else
    {
        // A reader leaves; the last one out lets a waiting writer in (someone waits: count >= 0).
        mem.Write32(rw + kRwReadersEntry, readersEntry - 1);
        if (readersEntry - 1 != 0)
            return;
    }
    mem.Write32(rw + kRwWritersWaiting, mem.Read32(rw + kRwWritersWaiting) - 1);
    lock.unlock();
    SetEvent(rw + kRwWriterEvent);
}

// ---- I/O completion ports (KQUEUE) -------------------------------------------------------------------------------
// dash.xex uses one through CreateIoCompletionPort / PostQueuedCompletionStatus / GetQueuedCompletionStatus for a
// Media Foundation work queue: packets are only posted by hand (no file is associated with the port), one thread
// takes them with a timeout. The concurrency limit (Count) only affects thread scheduling; it is recorded, not
// enforced.

// NTSTATUS NtCreateIoCompletion(PHANDLE Handle, ACCESS_MASK Access, POBJECT_ATTRIBUTES Attributes, ULONG Count)
GUEST_IMPORT(NtCreateIoCompletion)
{
    const uint32_t count = ctx.r6.u32 ? ctx.r6.u32 : 1;
    ctx.r3.u64 = CreateNamedObject(ctx, ctx.r3.u32, ctx.r5.u32, kKQueueSize,
        [&](uint32_t object) { InitializeQueue(object, count); });
}

// NTSTATUS NtSetIoCompletion(HANDLE Port, ULONG_PTR Key, PVOID ApcContext, NTSTATUS Status, ULONG_PTR Information)
GUEST_IMPORT(NtSetIoCompletion)
{
    const uint32_t queue = LookupHandle(ctx.r3.u32);
    if (queue == 0 || !HasType(queue, { kObjectQueue }))
    {
        ctx.r3.u64 = queue ? kStatusObjectTypeMismatch : kStatusInvalidHandle;
        return;
    }
    InsertQueuePacket(queue, { ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32 });
    ctx.r3.u64 = kStatusSuccess;
}

// NTSTATUS NtRemoveIoCompletion(HANDLE Port, PULONG_PTR Key, PVOID* ApcContext, PIO_STATUS_BLOCK IoStatus,
//                               PLARGE_INTEGER Timeout): the oldest packet, or STATUS_TIMEOUT with nothing written.
// The waiting thread holds a reference, so closing the handle meanwhile does not free the port under it.
GUEST_IMPORT(NtRemoveIoCompletion)
{
    auto& mem = guest::g_memory;
    const uint32_t queue = LookupHandle(ctx.r3.u32);
    if (queue == 0 || !HasType(queue, { kObjectQueue }))
    {
        ctx.r3.u64 = queue ? kStatusObjectTypeMismatch : kStatusInvalidHandle;
        return;
    }
    const uint32_t keyPtr = ctx.r4.u32, apcPtr = ctx.r5.u32, iosb = ctx.r6.u32;
    const auto deadline = TimeoutToDeadline(ctx.r7.u32);
    ReferenceObject(queue);
    QueuePacket packet;
    const uint32_t status = RemoveQueuePacket(queue, deadline, packet);
    DereferenceObject(queue);
    if (status == kStatusSuccess)
    {
        if (keyPtr)
            mem.Write32(keyPtr, packet.key);
        if (apcPtr)
            mem.Write32(apcPtr, packet.apcContext);
        if (iosb)
        {
            mem.Write32(iosb, packet.status);
            mem.Write32(iosb + 4, packet.information);
        }
    }
    ctx.r3.u64 = status;
}

// ---- Object lookups ----------------------------------------------------------------------------------------------

// VOID ObReferenceObject(PVOID Object)
GUEST_IMPORT(ObReferenceObject)
{
    ReferenceObject(ctx.r3.u32);
}

// NTSTATUS ObOpenObjectByPointer(PVOID Object, POBJECT_TYPE ObjectType, PHANDLE Handle): a new handle to the object.
GUEST_IMPORT(ObOpenObjectByPointer)
{
    guest::g_memory.Write32(ctx.r5.u32, CreateHandle(ctx.r3.u32));
    ctx.r3.u64 = kStatusSuccess;
}

// NTSTATUS ObLookupThreadByThreadId(ULONG ThreadId, PKTHREAD* Thread): the thread, referenced (the caller
// dereferences it); STATUS_INVALID_PARAMETER for an unknown or finished thread.
GUEST_IMPORT(ObLookupThreadByThreadId)
{
    uint32_t threadObject = 0;
    ForEachThread([&](GuestThread* t) {
        if (t->id == ctx.r3.u32 && !t->finished)
            threadObject = t->threadObject;
    });
    if (threadObject == 0)
    {
        ctx.r3.u64 = kStatusInvalidParameter;
        return;
    }
    ReferenceObject(threadObject);
    guest::g_memory.Write32(ctx.r4.u32, threadObject);
    ctx.r3.u64 = kStatusSuccess;
}

// NTSTATUS NtYieldExecution(): gives up the rest of the time slice.
GUEST_IMPORT(NtYieldExecution)
{
    SwitchToThread();
    ctx.r3.u64 = kStatusSuccess;
}
