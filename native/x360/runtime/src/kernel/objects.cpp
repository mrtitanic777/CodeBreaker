#include "kernel/objects.h"

#include <windows.h>

#include <algorithm>
#include <condition_variable>
#include <deque>
#include <map>
#include <unordered_map>
#include <vector>

namespace kernel
{
    namespace
    {
        std::mutex g_dispatcherMutex;

        std::mutex g_handleMutex;
        std::unordered_map<uint32_t, uint32_t> g_handles;      // handle -> object
        std::unordered_map<uint32_t, uint32_t> g_references;   // owned object -> reference count
        std::map<std::string, uint32_t> g_namedObjects;        // name -> object
        std::unordered_map<uint32_t, std::function<void()>> g_cleanups;
        uint32_t g_nextHandle = 0xF8000000;
        std::unordered_map<uint32_t, std::deque<QueuePacket>> g_queuePackets; // KQUEUE -> packets (dispatcher lock)

        constexpr int64_t kFileTimeUnitsPerSecond = 10000000;

        int64_t NowFileTime()
        {
            FILETIME ft;
            GetSystemTimeAsFileTime(&ft);
            return int64_t(uint64_t(ft.dwHighDateTime) << 32 | ft.dwLowDateTime);
        }

        uint32_t CurrentThreadObject()
        {
            GuestThread* thread = CurrentThread();
            return thread ? thread->threadObject : 0;
        }

        int32_t SignalState(uint32_t object)
        {
            return int32_t(guest::g_memory.Read32(object + header::kSignalState));
        }

        void SetSignalState(uint32_t object, int32_t value)
        {
            guest::g_memory.Write32(object + header::kSignalState, uint32_t(value));
        }

        // Whether a wait on the object would be satisfied now (dispatcher lock held).
        bool IsSignaled(uint32_t object, uint32_t thread)
        {
            const uint8_t type = guest::g_memory.Read8(object + header::kType);
            switch (type)
            {
            case kObjectEventNotification:
            case kObjectEventSynchronization:
            case kObjectProcess:
            case kObjectThread:
            case kObjectTimerNotification:
            case kObjectTimerSynchronization:
                return SignalState(object) != 0;
            case kObjectSemaphore:
            case kObjectQueue:
                return SignalState(object) > 0;
            case kObjectMutant:
                return SignalState(object) > 0 || guest::g_memory.Read32(object + kKMutantOwner) == thread;
            default:
                Fatal("Wait on dispatcher object {:08X} of unsupported type {}", object, type);
            }
        }

        // Applies the side effects of a satisfied wait; returns true if the object was an abandoned mutant.
        bool Satisfy(uint32_t object, uint32_t thread)
        {
            auto& mem = guest::g_memory;
            switch (mem.Read8(object + header::kType))
            {
            case kObjectEventSynchronization:
            case kObjectTimerSynchronization:
                SetSignalState(object, 0);
                return false;
            case kObjectSemaphore:
                SetSignalState(object, SignalState(object) - 1);
                return false;
            case kObjectMutant:
            {
                const int32_t state = SignalState(object) - 1;
                SetSignalState(object, state);
                if (state == 0)
                    mem.Write32(object + kKMutantOwner, thread);
                const bool abandoned = mem.Read8(object + kKMutantAbandoned) != 0;
                mem.Write8(object + kKMutantAbandoned, 0);
                return abandoned;
            }
            default:
                return false; // notification objects stay signaled
            }
        }

        // A thread blocked in WaitForObjects or DelayExecution. Waiters are kept in the order their waits began
        // (all guarded by the dispatcher lock); a signal completes them in that order, as NT's KiWaitTest does.
        struct Waiter
        {
            const uint32_t* objects = nullptr;
            uint32_t count = 0;
            bool waitAll = false;
            bool alertable = false;
            uint32_t thread = 0;              // KTHREAD (mutant ownership)
            GuestThread* guestThread = nullptr;
            std::condition_variable wake;
            bool done = false;
            uint32_t result = 0;
            QueuePacket* removePacket = nullptr; // RemoveQueuePacket: completing takes a packet from objects[0]
        };

        // Takes the oldest packet of a queue that has one (dispatcher lock held).
        void PopPacket(uint32_t queue, QueuePacket& packet)
        {
            auto& packets = g_queuePackets[queue];
            packet = packets.front();
            packets.pop_front();
            SetSignalState(queue, int32_t(packets.size()));
        }
        std::vector<Waiter*> g_waiters;

        // Completes the wait if it can be satisfied now: consumes the objects (dispatcher lock held) and sets the
        // result (STATUS_WAIT_0 + index for wait-any, STATUS_SUCCESS for wait-all, abandoned variants for mutants).
        bool TryComplete(Waiter& w)
        {
            if (w.count == 0)
                return false;
            if (w.removePacket)
            {
                if (!IsSignaled(w.objects[0], w.thread))
                    return false;
                PopPacket(w.objects[0], *w.removePacket);
                w.result = kStatusSuccess;
                return true;
            }
            if (w.waitAll)
            {
                for (uint32_t i = 0; i < w.count; i++)
                    if (!IsSignaled(w.objects[i], w.thread))
                        return false;
                bool abandoned = false;
                for (uint32_t i = 0; i < w.count; i++)
                    abandoned |= Satisfy(w.objects[i], w.thread);
                w.result = abandoned ? kStatusAbandonedWait0 : kStatusSuccess;
                return true;
            }
            for (uint32_t i = 0; i < w.count; i++)
            {
                if (IsSignaled(w.objects[i], w.thread))
                {
                    w.result = (Satisfy(w.objects[i], w.thread) ? kStatusAbandonedWait0 : kStatusSuccess) + i;
                    return true;
                }
            }
            return false;
        }

        void FreeOwnedObject(uint32_t object)
        {
            if (auto cleanup = g_cleanups.find(object); cleanup != g_cleanups.end())
            {
                cleanup->second();
                g_cleanups.erase(cleanup);
            }
            for (auto it = g_namedObjects.begin(); it != g_namedObjects.end();)
                it = it->second == object ? g_namedObjects.erase(it) : std::next(it);
            guest::g_memory.SystemFree(object);
        }
    }

    std::unique_lock<std::mutex> LockDispatcher()
    {
        return std::unique_lock(g_dispatcherMutex);
    }

    void NotifyDispatcherChange(uint32_t object)
    {
        // KiWaitTest: in the order the waits began, satisfy each waiter on `object` whose wait can now complete, and
        // wake only those. A synchronization object is consumed by the first such waiter, so later ones keep waiting.
        for (size_t i = 0; i < g_waiters.size();)
        {
            Waiter* w = g_waiters[i];
            if (std::find(w->objects, w->objects + w->count, object) != w->objects + w->count && TryComplete(*w))
            {
                g_waiters.erase(g_waiters.begin() + i);
                w->done = true;
                w->wake.notify_one();
            }
            else
                i++;
        }
    }

    void InitializeDispatcherHeader(uint32_t object, uint8_t type, int32_t signalState)
    {
        auto& mem = guest::g_memory;
        mem.Write8(object + header::kType, type);
        mem.Write8(object + header::kAbsolute, 0);
        mem.Write8(object + 2, 0);
        mem.Write8(object + 3, 0);
        mem.Write32(object + header::kSignalState, uint32_t(signalState));
        mem.Write32(object + header::kWaitList, object + header::kWaitList); // empty list: flink = blink = self
        mem.Write32(object + header::kWaitList + 4, object + header::kWaitList);
    }

    std::optional<std::chrono::steady_clock::time_point> TimeoutToDeadline(uint32_t timeoutPtr)
    {
        if (timeoutPtr == 0)
            return std::nullopt;
        const int64_t timeout = int64_t(guest::g_memory.Read64(timeoutPtr));
        const auto now = std::chrono::steady_clock::now();
        int64_t relative100ns = timeout < 0 ? -timeout : std::max<int64_t>(0, timeout - NowFileTime());
        return now + std::chrono::nanoseconds(relative100ns) * 100;
    }

    namespace
    {
        // Blocks the calling thread (dispatcher lock held) until its waiter completes or the deadline passes; returns
        // the waiter's result or STATUS_TIMEOUT. A waiter that times out is taken off the wait list.
        // Ends a wait of a thread ExTerminateTitleProcess ends (the waiter's result; never a real status).
        constexpr uint32_t kResultTerminating = 0xDEAD7E57;

        uint32_t Block(std::unique_lock<std::mutex>& lock, Waiter& w,
            std::optional<std::chrono::steady_clock::time_point> deadline)
        {
            if (w.guestThread && w.guestThread->terminating)
                throw ThreadExit{ 0 };
            g_waiters.push_back(&w);
            while (!w.done)
            {
                if (!deadline)
                    w.wake.wait(lock);
                else if (w.wake.wait_until(lock, *deadline) == std::cv_status::timeout && !w.done)
                {
                    g_waiters.erase(std::find(g_waiters.begin(), g_waiters.end(), &w));
                    return kStatusTimeout;
                }
            }
            if (w.result == kResultTerminating)
                throw ThreadExit{ 0 };
            return w.result;
        }
    }

    uint32_t WaitForObjects(const uint32_t* objects, uint32_t count, bool waitAll, uint32_t timeoutPtr, bool alertable)
    {
        // An object of a type xam.xex defined (ObCreateObject) is waited on through its type's DefaultObject.
        std::vector<uint32_t> waitable(objects, objects + count);
        for (uint32_t& o : waitable)
            o = WaitableObject(o);
        objects = waitable.data();
        const auto deadline = TimeoutToDeadline(timeoutPtr);
        Waiter w;
        w.objects = objects;
        w.count = count;
        w.waitAll = waitAll;
        w.alertable = alertable;
        w.guestThread = CurrentThread();
        w.thread = w.guestThread ? w.guestThread->threadObject : 0;
        auto lock = LockDispatcher();
        if (alertable && w.guestThread && !w.guestThread->userApcs.empty())
            return kStatusUserApc;
        if (TryComplete(w))
            return w.result;
        if (deadline && std::chrono::steady_clock::now() >= *deadline)
            return kStatusTimeout;
        return Block(lock, w, deadline);
    }

    void InitializeQueue(uint32_t queue, uint32_t maximumCount)
    {
        auto& mem = guest::g_memory;
        auto lock = LockDispatcher();
        InitializeDispatcherHeader(queue, kObjectQueue, 0);
        mem.Write32(queue + 0x10, queue + 0x10); // EntryListHead (empty)
        mem.Write32(queue + 0x14, queue + 0x10);
        mem.Write32(queue + 0x18, 0);            // CurrentCount
        mem.Write32(queue + 0x1C, maximumCount); // MaximumCount
        mem.Write32(queue + 0x20, queue + 0x20); // ThreadListHead (empty)
        mem.Write32(queue + 0x24, queue + 0x20);
        g_queuePackets[queue].clear();
    }

    void InsertQueuePacket(uint32_t queue, const QueuePacket& packet)
    {
        auto lock = LockDispatcher();
        auto& packets = g_queuePackets[queue];
        packets.push_back(packet);
        SetSignalState(queue, int32_t(packets.size()));
        NotifyDispatcherChange(queue);
    }

    uint32_t RemoveQueuePacket(uint32_t queue, std::optional<std::chrono::steady_clock::time_point> deadline,
        QueuePacket& packet)
    {
        Waiter w;
        w.objects = &queue;
        w.count = 1;
        w.removePacket = &packet;
        w.guestThread = CurrentThread();
        w.thread = w.guestThread ? w.guestThread->threadObject : 0;
        auto lock = LockDispatcher();
        if (TryComplete(w))
            return w.result;
        if (deadline && std::chrono::steady_clock::now() >= *deadline)
            return kStatusTimeout;
        return Block(lock, w, deadline);
    }

    void WakeForTermination(GuestThread* thread)
    {
        auto lock = LockDispatcher();
        thread->terminating = true;
        for (size_t i = 0; i < g_waiters.size();)
        {
            Waiter* w = g_waiters[i];
            if (w->guestThread != thread)
            {
                i++;
                continue;
            }
            g_waiters.erase(g_waiters.begin() + i);
            w->result = kResultTerminating;
            w->done = true;
            w->wake.notify_one();
        }
    }

    void QueueUserApc(GuestThread* thread, const UserApc& apc)
    {
        auto lock = LockDispatcher();
        thread->userApcs.push_back(apc);
        // An alertable wait of the thread ends with STATUS_USER_APC.
        for (size_t i = 0; i < g_waiters.size(); i++)
        {
            Waiter* w = g_waiters[i];
            if (w->guestThread == thread && w->alertable)
            {
                g_waiters.erase(g_waiters.begin() + i);
                w->result = kStatusUserApc;
                w->done = true;
                w->wake.notify_one();
                break;
            }
        }
    }

    void DeliverUserApcs(PPCContext& ctx)
    {
        GuestThread* self = CurrentThread();
        for (;;)
        {
            UserApc apc;
            {
                auto lock = LockDispatcher();
                if (self->userApcs.empty())
                    return;
                apc = self->userApcs.front();
                self->userApcs.erase(self->userApcs.begin());
            }
            if (apc.apc)
                guest::g_memory.Write8(apc.apc + 2, 0); // KAPC.Inserted
            if (apc.kernelRoutine)
            {
                // KernelRoutine(Apc, &NormalRoutine, &NormalContext, &SystemArgument1, &SystemArgument2): the four
                // values in a scratch block above the stack pointer the routine gets (outside its frame).
                auto& mem = guest::g_memory;
                const uint32_t frame = (ctx.r1.u32 - 0x60) & ~0xFu, values = frame + 0x30;
                mem.Write32(values + 0, apc.routine);
                mem.Write32(values + 4, apc.context);
                mem.Write32(values + 8, apc.argument1);
                mem.Write32(values + 12, apc.argument2);
                PPCContext call = ctx;
                call.r1.u64 = frame;
                call.r3.u64 = apc.apc;
                call.r4.u64 = values;
                call.r5.u64 = values + 4;
                call.r6.u64 = values + 8;
                call.r7.u64 = values + 12;
                CallGuest(call, apc.kernelRoutine);
                apc.routine = mem.Read32(values + 0);
                apc.context = mem.Read32(values + 4);
                apc.argument1 = mem.Read32(values + 8);
                apc.argument2 = mem.Read32(values + 12);
            }
            if (apc.routine == 0)
                continue;
            PPCContext call = ctx;
            call.r3.u64 = apc.context;
            call.r4.u64 = apc.argument1;
            call.r5.u64 = apc.argument2;
            CallGuest(call, apc.routine);
        }
    }

    uint32_t DelayExecution(std::optional<std::chrono::steady_clock::time_point> deadline, bool alertable)
    {
        Waiter w; // waits on no object: only the deadline or (alertable) a user APC ends it
        w.alertable = alertable;
        w.guestThread = CurrentThread();
        auto lock = LockDispatcher();
        if (alertable && w.guestThread && !w.guestThread->userApcs.empty())
            return kStatusUserApc;
        if (deadline && std::chrono::steady_clock::now() >= *deadline)
            return kStatusSuccess;
        const uint32_t status = Block(lock, w, deadline);
        return status == kStatusTimeout ? kStatusSuccess : status;
    }

    uint32_t CreateOwnedObject(uint32_t size)
    {
        const uint32_t object = guest::g_memory.SystemAlloc(size);
        std::lock_guard lock(g_handleMutex);
        g_references[object] = 0;
        return object;
    }

    void SetObjectCleanup(uint32_t object, std::function<void()> cleanup)
    {
        std::lock_guard lock(g_handleMutex);
        g_cleanups[object] = std::move(cleanup);
    }

    void ReferenceObject(uint32_t object)
    {
        std::lock_guard lock(g_handleMutex);
        auto it = g_references.find(object);
        if (it != g_references.end())
            it->second++;
    }

    void DereferenceObject(uint32_t object)
    {
        bool last = false;
        {
            std::lock_guard lock(g_handleMutex);
            auto it = g_references.find(object);
            if (it != g_references.end() && --it->second == 0)
            {
                g_references.erase(it);
                last = true;
            }
        }
        if (!last)
            return;
        // The object's type procedures (an XAM-defined type's DeleteProcedure) run outside the handle lock: guest
        // code there may close or dereference other objects.
        ObjectDeleting(object);
        std::lock_guard lock(g_handleMutex);
        FreeOwnedObject(object);
    }

    uint32_t CreateHandle(uint32_t object)
    {
        ReferenceObject(object);
        std::lock_guard lock(g_handleMutex);
        const uint32_t handle = g_nextHandle;
        g_nextHandle += 4;
        g_handles[handle] = object;
        return handle;
    }

    uint32_t LookupHandle(uint32_t handle)
    {
        if (handle == kCurrentThreadHandle)
            return CurrentThreadObject();
        std::lock_guard lock(g_handleMutex);
        auto it = g_handles.find(handle);
        return it == g_handles.end() ? 0 : it->second;
    }

    bool CloseHandle(uint32_t handle)
    {
        uint32_t object;
        {
            std::lock_guard lock(g_handleMutex);
            auto it = g_handles.find(handle);
            if (it == g_handles.end())
                return false;
            object = it->second;
            g_handles.erase(it);
        }
        ObjectHandleClosing(object);
        DereferenceObject(object);
        return true;
    }

    uint32_t LookupNamedObject(const std::string& name)
    {
        std::lock_guard lock(g_handleMutex);
        auto it = g_namedObjects.find(name);
        return it == g_namedObjects.end() ? 0 : it->second;
    }

    void RegisterNamedObject(const std::string& name, uint32_t object)
    {
        std::lock_guard lock(g_handleMutex);
        g_namedObjects[name] = object;
    }

    std::string ReadObjectName(uint32_t objectAttributes)
    {
        auto& mem = guest::g_memory;
        if (objectAttributes == 0)
            return {};
        const uint32_t name = mem.Read32(objectAttributes + 4); // OBJECT_ATTRIBUTES.ObjectName -> ANSI_STRING
        if (name == 0)
            return {};
        const uint16_t length = mem.Read16(name);
        const uint32_t buffer = mem.Read32(name + 4);
        if (length == 0 || buffer == 0)
            return {};
        return std::string(reinterpret_cast<const char*>(mem.Translate(buffer)), length);
    }
}
