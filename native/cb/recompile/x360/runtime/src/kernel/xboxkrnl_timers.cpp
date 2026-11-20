// xboxkrnl timer exports (NtCreateTimer / NtSetTimerEx / NtCancelTimer, KeInitializeTimerEx / KeSetTimerEx /
// KeCancelTimer).
//
// A timer is a KTIMER dispatcher object (type 8 notification, 9 synchronization) in guest memory: created by
// NtCreateTimer, or the caller's own KTIMER initialised by KeInitializeTimerEx. One host scheduler thread signals timers
// at their due time through the dispatcher (so waits see them like any other object), queues their DPC (KeSetTimerEx:
// DeferredRoutine(Dpc, DeferredContext, low / high 32 bits of the system time)) or user APC (NtSetTimerEx:
// ApcRoutine(ApcContext, low / high 32 bits of the system time), delivered at the setting thread's next alertable
// wait), and re-arms periodic timers. Re-setting or cancelling bumps a generation number so stale schedule entries
// are ignored.

#include "kernel/objects.h"

#include <windows.h>

#include <condition_variable>
#include <map>
#include <queue>
#include <thread>

using namespace kernel;

namespace
{
    constexpr uint32_t kKTimerSize = 0x28;

    using Clock = std::chrono::steady_clock;

    struct TimerState
    {
        uint64_t generation = 0;
        uint32_t periodMs = 0;
        bool armed = false;
        uint32_t dpc = 0;                      // KeSetTimerEx
        GuestThread* apcThread = nullptr;      // NtSetTimerEx with an APC routine
        uint32_t apcRoutine = 0, apcContext = 0;
    };

    uint64_t SystemTime()
    {
        FILETIME ft;
        GetSystemTimeAsFileTime(&ft);
        return uint64_t(ft.dwHighDateTime) << 32 | ft.dwLowDateTime;
    }

    struct Entry
    {
        Clock::time_point due;
        uint32_t timer;
        uint64_t generation;
        bool operator>(const Entry& other) const { return due > other.due; }
    };

    std::mutex g_mutex;
    std::condition_variable g_changed;
    std::map<uint32_t, TimerState> g_timers;
    std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> g_queue;
    bool g_threadStarted = false;

    void SchedulerThread()
    {
        std::unique_lock lock(g_mutex);
        for (;;)
        {
            if (g_queue.empty())
            {
                g_changed.wait(lock);
                continue;
            }
            const Entry next = g_queue.top();
            if (Clock::now() < next.due)
            {
                g_changed.wait_until(lock, next.due);
                continue;
            }
            g_queue.pop();
            auto it = g_timers.find(next.timer);
            if (it == g_timers.end() || it->second.generation != next.generation || !it->second.armed)
                continue;
            {
                auto dispatcher = LockDispatcher();
                guest::g_memory.Write32(next.timer + header::kSignalState, 1);
                NotifyDispatcherChange(next.timer);
            }
            const uint64_t now = SystemTime();
            if (it->second.dpc)
                QueueDpc(it->second.dpc, uint32_t(now), uint32_t(now >> 32));
            if (it->second.apcRoutine && it->second.apcThread)
                QueueUserApc(it->second.apcThread,
                             { it->second.apcRoutine, it->second.apcContext, uint32_t(now), uint32_t(now >> 32) });
            if (it->second.periodMs)
                g_queue.push({ next.due + std::chrono::milliseconds(it->second.periodMs), next.timer, next.generation });
            else
                it->second.armed = false;
        }
    }

    TimerState* Lookup(uint32_t timer)
    {
        auto it = g_timers.find(timer);
        return it == g_timers.end() ? nullptr : &it->second;
    }

    // Caller holds g_mutex.
    void StartScheduler()
    {
        if (!g_threadStarted)
        {
            g_threadStarted = true;
            std::thread(SchedulerThread).detach();
        }
    }

    // DueTime: negative = relative (100 ns), positive = absolute system time, 0 = now.
    Clock::time_point DueToDeadline(int64_t due)
    {
        const int64_t relative100ns = due < 0 ? -due : std::max<int64_t>(0, due - int64_t(SystemTime()));
        return Clock::now() + std::chrono::nanoseconds(relative100ns) * 100;
    }
}

// NTSTATUS NtCreateTimer(PHANDLE TimerHandle, POBJECT_ATTRIBUTES ObjectAttributes, TIMER_TYPE TimerType)
// TimerType: 0 = notification, 1 = synchronization.
GUEST_IMPORT(NtCreateTimer)
{
    auto& mem = guest::g_memory;
    const uint32_t handlePtr = ctx.r3.u32;
    const std::string name = ReadObjectName(ctx.r4.u32);
    const uint8_t type = ctx.r5.u32 == 0 ? kObjectTimerNotification : kObjectTimerSynchronization;
    if (!name.empty())
    {
        if (uint32_t existing = LookupNamedObject(name))
        {
            mem.Write32(handlePtr, CreateHandle(existing));
            ctx.r3.u64 = kStatusObjectNameExists;
            return;
        }
    }
    const uint32_t timer = CreateOwnedObject(kKTimerSize);
    InitializeDispatcherHeader(timer, type, 0);
    if (!name.empty())
        RegisterNamedObject(name, timer);
    {
        std::lock_guard lock(g_mutex);
        g_timers[timer] = {};
        StartScheduler();
    }
    SetObjectCleanup(timer, [timer]
        {
            std::lock_guard lock(g_mutex);
            g_timers.erase(timer);
        });
    mem.Write32(handlePtr, CreateHandle(timer));
    ctx.r3.u64 = kStatusSuccess;
}

// NTSTATUS NtSetTimerEx(HANDLE TimerHandle, PLARGE_INTEGER DueTime, PTIMER_APC_ROUTINE ApcRoutine,
//                       KPROCESSOR_MODE ApcMode, PVOID ApcContext, BOOLEAN ResumeTimer, LONG Period,
//                       PBOOLEAN PreviousState)
// DueTime: negative = relative (100 ns), positive = absolute system time. Period is in milliseconds.
GUEST_IMPORT(NtSetTimerEx)
{
    auto& mem = guest::g_memory;
    const uint32_t timer = LookupHandle(ctx.r3.u32);
    const uint32_t apcRoutine = ctx.r5.u32, apcContext = ctx.r7.u32;
    std::lock_guard lock(g_mutex);
    TimerState* state = timer ? Lookup(timer) : nullptr;
    if (state == nullptr)
    {
        ctx.r3.u64 = kStatusInvalidHandle;
        return;
    }
    const auto deadline = TimeoutToDeadline(ctx.r4.u32);
    int32_t previous;
    {
        // Setting a timer clears its signal state.
        auto dispatcher = LockDispatcher();
        previous = int32_t(mem.Read32(timer + header::kSignalState));
        mem.Write32(timer + header::kSignalState, 0);
    }
    state->generation++;
    state->periodMs = ctx.r9.u32;
    state->armed = true;
    state->apcRoutine = apcRoutine;
    state->apcContext = apcContext;
    state->apcThread = apcRoutine ? CurrentThread() : nullptr;
    g_queue.push({ deadline.value_or(Clock::now()), timer, state->generation });
    g_changed.notify_all();
    if (const uint32_t previousPtr = ctx.r10.u32)
        mem.Write8(previousPtr, previous != 0 ? 1 : 0);
    ctx.r3.u64 = kStatusSuccess;
}

// NTSTATUS NtCancelTimer(HANDLE TimerHandle, PBOOLEAN CurrentState)
GUEST_IMPORT(NtCancelTimer)
{
    auto& mem = guest::g_memory;
    const uint32_t timer = LookupHandle(ctx.r3.u32);
    std::lock_guard lock(g_mutex);
    TimerState* state = timer ? Lookup(timer) : nullptr;
    if (state == nullptr)
    {
        ctx.r3.u64 = kStatusInvalidHandle;
        return;
    }
    state->generation++;
    state->armed = false;
    if (const uint32_t currentPtr = ctx.r4.u32)
    {
        auto dispatcher = LockDispatcher();
        mem.Write8(currentPtr, mem.Read32(timer + header::kSignalState) != 0 ? 1 : 0);
    }
    ctx.r3.u64 = kStatusSuccess;
}

// VOID KeInitializeTimerEx(PKTIMER Timer, TIMER_TYPE Type, UCHAR ProcessType): the caller's KTIMER, not signaled.
// Type: 0 = notification, 1 = synchronization.
GUEST_IMPORT(KeInitializeTimerEx)
{
    const uint32_t timer = ctx.r3.u32;
    memset(guest::g_memory.Translate(timer), 0, kKTimerSize);
    InitializeDispatcherHeader(timer, ctx.r4.u32 == 0 ? kObjectTimerNotification : kObjectTimerSynchronization, 0);
    std::lock_guard lock(g_mutex);
    g_timers[timer] = {};
    StartScheduler();
}

// BOOLEAN KeSetTimerEx(PKTIMER Timer, LARGE_INTEGER DueTime, LONG Period, PKDPC Dpc): TRUE if the timer was already
// set (it is then re-set). Period in milliseconds; the DPC (if any) is queued at each expiry.
GUEST_IMPORT(KeSetTimerEx)
{
    auto& mem = guest::g_memory;
    const uint32_t timer = ctx.r3.u32;
    const int64_t due = int64_t(ctx.r4.u64);
    std::lock_guard lock(g_mutex);
    TimerState* state = Lookup(timer);
    if (state == nullptr)
        Fatal("KeSetTimerEx({:08X}): not a timer from KeInitializeTimerEx or NtCreateTimer", timer);
    const bool wasSet = state->armed;
    {
        auto dispatcher = LockDispatcher();
        mem.Write32(timer + header::kSignalState, 0);
    }
    state->generation++;
    state->periodMs = ctx.r5.u32;
    state->dpc = ctx.r6.u32;
    state->armed = true;
    g_queue.push({ DueToDeadline(due), timer, state->generation });
    g_changed.notify_all();
    ctx.r3.u64 = wasSet ? 1 : 0;
}

// BOOLEAN KeCancelTimer(PKTIMER Timer): TRUE if the timer was set.
GUEST_IMPORT(KeCancelTimer)
{
    std::lock_guard lock(g_mutex);
    TimerState* state = Lookup(ctx.r3.u32);
    const bool wasSet = state && state->armed;
    if (state)
    {
        state->generation++;
        state->armed = false;
    }
    ctx.r3.u64 = wasSet ? 1 : 0;
}
