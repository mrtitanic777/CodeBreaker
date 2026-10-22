#pragma once

#include "kernel/kernel.h"

#include <chrono>
#include <functional>
#include <mutex>
#include <optional>
#include <string>

// Dispatcher objects and handles.
//
// Xbox dispatcher objects (events, semaphores, mutants, threads, timers) live in guest memory; titles initialize and
// read their DISPATCHER_HEADER fields inline. The guest structure is therefore the only state: every change happens
// under one dispatcher lock and is written straight into guest memory. A blocked thread waits on its own host
// condition variable; a change to an object completes the waits on it in the order they began (NT's KiWaitTest:
// a synchronization object goes to the first waiter that can be satisfied) and wakes only those threads.
//
// DISPATCHER_HEADER: +0 Type (u8), +1 Absolute/Abandoned (u8), +2 Size (u8), +3 Inserted (u8),
//                    +4 SignalState (s32), +8 WaitListHead (LIST_ENTRY). Size 0x10.
namespace kernel
{
    namespace header
    {
        constexpr uint32_t kType = 0x0;
        constexpr uint32_t kAbsolute = 0x1;
        constexpr uint32_t kSignalState = 0x4;
        constexpr uint32_t kWaitList = 0x8;
        constexpr uint32_t kSize = 0x10;
    }

    constexpr uint8_t kObjectTimerNotification = 8;
    constexpr uint8_t kObjectTimerSynchronization = 9;

    constexpr uint32_t kKEventSize = 0x10;
    constexpr uint32_t kKSemaphoreSize = 0x14;  // header + Limit
    constexpr uint32_t kKSemaphoreLimit = 0x10;
    constexpr uint32_t kKMutantSize = 0x20;     // header + MutantListEntry + OwnerThread + Abandoned
    constexpr uint32_t kKMutantOwner = 0x18;
    constexpr uint32_t kKMutantAbandoned = 0x1C;

    constexpr uint32_t kStatusAbandonedWait0 = 0x00000080;
    constexpr uint32_t kStatusObjectNameExists = 0x40000000;
    constexpr uint32_t kStatusInvalidHandle = 0xC0000008;
    constexpr uint32_t kStatusObjectTypeMismatch = 0xC0000024;
    constexpr uint32_t kStatusSemaphoreLimitExceeded = 0xC0000047;

    constexpr uint32_t kCurrentProcessHandle = 0xFFFFFFFF;
    constexpr uint32_t kCurrentThreadHandle = 0xFFFFFFFE;

    // Lock guarding all dispatcher-object state. Anything that raises an object's SignalState (or releases a mutant)
    // must hold it and then call NotifyDispatcherChange(object), which satisfies the waits that can now complete.
    std::unique_lock<std::mutex> LockDispatcher();
    void NotifyDispatcherChange(uint32_t object);

    // Initializes a DISPATCHER_HEADER in guest memory.
    void InitializeDispatcherHeader(uint32_t object, uint8_t type, int32_t signalState);

    // Converts an NT timeout (LARGE_INTEGER pointer; null = infinite) to a host deadline.
    // Negative values are relative (100 ns units), positive values are absolute system time (FILETIME), 0 polls.
    std::optional<std::chrono::steady_clock::time_point> TimeoutToDeadline(uint32_t timeoutPtr);

    // Waits on guest dispatcher objects. Returns STATUS_WAIT_0 + index (WaitAny), STATUS_SUCCESS (WaitAll),
    // STATUS_ABANDONED_WAIT_0 + index for abandoned mutants, or STATUS_TIMEOUT.
    // An alertable user-mode wait (`alertable`) returns STATUS_USER_APC as soon as the thread has user APCs queued,
    // without satisfying any object; the caller then runs them with DeliverUserApcs.
    uint32_t WaitForObjects(const uint32_t* objects, uint32_t count, bool waitAll, uint32_t timeoutPtr,
        bool alertable = false);

    // Kernel queues (KQUEUE, type 4; the I/O completion ports of NtCreateIoCompletion). The header's SignalState is
    // the number of queued packets; the packets themselves are kept by the runtime. A plain wait on a queue does not
    // take a packet (as in NT); RemoveQueuePacket does, atomically with the wait.
    struct QueuePacket
    {
        uint32_t key = 0;
        uint32_t apcContext = 0;
        uint32_t status = 0;
        uint32_t information = 0;
    };
    constexpr uint32_t kKQueueSize = 0x28; // header, EntryListHead, CurrentCount, MaximumCount, ThreadListHead
    // Initializes a KQUEUE in guest memory (and drops packets left by an earlier object at that address).
    void InitializeQueue(uint32_t queue, uint32_t maximumCount);
    void InsertQueuePacket(uint32_t queue, const QueuePacket& packet);
    // Takes the oldest packet, waiting until the deadline (none = forever; not alertable). STATUS_SUCCESS or
    // STATUS_TIMEOUT.
    uint32_t RemoveQueuePacket(uint32_t queue, std::optional<std::chrono::steady_clock::time_point> deadline,
        QueuePacket& packet);

    // Queues a user APC to a thread and wakes it if it is in an alertable wait.
    void QueueUserApc(GuestThread* thread, const UserApc& apc);
    // Runs the current thread's queued user APCs, in order, on its own stack (ctx is the waiting caller's state).
    void DeliverUserApcs(PPCContext& ctx);
    // Waits until the deadline (or forever) or, if alertable, until a user APC is queued. Returns STATUS_USER_APC or
    // STATUS_SUCCESS (KeDelayExecutionThread).
    uint32_t DelayExecution(std::optional<std::chrono::steady_clock::time_point> deadline, bool alertable);

    // Objects the runtime allocates for Nt*Create calls are reference counted (handles + Ob references) and freed
    // when the count reaches zero. Objects in title memory (KEVENTs embedded in game structures) are never freed.
    uint32_t CreateOwnedObject(uint32_t size);
    // Runs when an owned object is freed (e.g. closing the host file behind a file object).
    void SetObjectCleanup(uint32_t object, std::function<void()> cleanup);
    void ReferenceObject(uint32_t object);
    // Hooks for objects of types XAM defines (kernel/xboxkrnl_drivers.cpp): a handle to it closing (the type's
    // CloseProcedure) and its last reference going (DeleteProcedure); called outside the handle lock.
    void ObjectHandleClosing(uint32_t object);
    void ObjectDeleting(uint32_t object);
    void DereferenceObject(uint32_t object);

    // Handle table. Handles reference guest dispatcher objects (by guest address); each handle holds a reference.
    uint32_t CreateHandle(uint32_t object);
    // Returns the object for a handle (pseudo-handles included), or 0.
    uint32_t LookupHandle(uint32_t handle);
    bool CloseHandle(uint32_t handle);
    // Named objects (OBJECT_ATTRIBUTES.ObjectName); returns 0 if no object has the name.
    uint32_t LookupNamedObject(const std::string& name);
    void RegisterNamedObject(const std::string& name, uint32_t object);

    // Reads OBJECT_ATTRIBUTES.ObjectName (ANSI_STRING) into a string; empty if there is none.
    std::string ReadObjectName(uint32_t objectAttributes);
}
