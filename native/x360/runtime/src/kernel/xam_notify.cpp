// XAM notification listeners (XamNotifyCreateListener / XNotifyGetNext).
//
// A listener is a handle to a manual-reset event (signaled while notifications are queued) plus a host-side queue.
// Notification IDs: area << 25 | version << 16 | index; a listener receives an ID if bit `area` of its mask is set and
// the ID's version does not exceed the listener's maximum version.
// Startup notifications: when a title has loaded, XAM's system app (message 0x80000002, 0x816BD930 -> 0x816A94E0)
// broadcasts the current state: XN_SYS_UI, XN_SYS_SIGNINCHANGED, the DVD drive's media type (0x8000000D) and the second
// drive's (0x80010014). Here the title's first listener of each area gets them.

#include "kernel/objects.h"
#include "kernel/users.h"
#include "xui/system_ui.h"

#include <cstdlib>
#include <deque>
#include <map>
#include <mutex>
#include <vector>

namespace
{
    constexpr uint32_t kNotifySystem = 0x01;
    constexpr uint32_t kNotifyLive = 0x02;
    constexpr uint32_t kNotifyXmp = 0x20;

    constexpr uint32_t kXnSysUi = 0x00000009;
    constexpr uint32_t kXnSysSignInChanged = 0x0000000A;
    constexpr uint32_t kXnSysDvdMediaChanged = 0x8000000D;    // param: XamLoaderGetMediaInfoEx's media type
    constexpr uint32_t kXnSysSecondDriveMediaChanged = 0x80010014; // param: its third output (loader +0x10 drive)
    constexpr uint32_t kXnLiveConnectionChanged = 0x02000001;
    constexpr uint32_t kXnLiveLinkStateChanged = 0x02000003;
    constexpr uint32_t kXnXmpPlaybackControllerChanged = 0x0A000003;

    constexpr uint32_t kOnlineLogonDisconnected = 0x001510F1; // X_ONLINE_S_LOGON_DISCONNECTED

    struct Listener
    {
        uint64_t mask = 0;
        uint32_t maxVersion = 0;
        bool system = false; // created by system code: XAM also delivers its system-only notifications to it
        std::deque<std::pair<uint32_t, uint32_t>> queue; // (id, param)
    };

    std::mutex g_mutex;
    std::map<uint32_t, Listener> g_listeners; // event object -> listener
    bool g_systemStartupSent = false;
    bool g_liveStartupSent = false;
    bool g_xmpStartupSent = false;
    // Listeners whose event was freed (the last handle closed): dropped before g_listeners is next used, so a
    // broadcast never writes into freed memory. A separate lock: object cleanups run under the handle table's lock.
    std::mutex g_closedMutex;
    std::vector<uint32_t> g_closed;

    // Caller holds g_mutex.
    void DropClosedListeners()
    {
        std::lock_guard lock(g_closedMutex);
        for (uint32_t event : g_closed)
            g_listeners.erase(event);
        g_closed.clear();
    }

    void SetSignal(uint32_t event, bool signaled)
    {
        auto lock = kernel::LockDispatcher();
        guest::g_memory.Write32(event + kernel::header::kSignalState, signaled ? 1 : 0);
        if (signaled)
            kernel::NotifyDispatcherChange(event);
    }

    // Caller holds g_mutex.
    void Enqueue(uint32_t event, Listener& listener, uint32_t id, uint32_t param)
    {
        const uint32_t area = (id >> 25) & 0x3F;
        const uint32_t version = (id >> 16) & 0x1FF;
        if (!(listener.mask & (1ull << area)) || version > listener.maxVersion)
            return;
        listener.queue.emplace_back(id, param);
        SetSignal(event, true);
    }
}

namespace
{
    // CB_TRACE_NOTIFY=1: every notification broadcast and every one a listener takes (XNotifyGetNext).
    bool TraceNotify()
    {
        static const bool on = [] { const char* v = std::getenv("CB_TRACE_NOTIFY"); return v && v[0] == '1'; }();
        return on;
    }
}

void kernel::BroadcastNotification(uint32_t id, uint32_t param)
{
    std::lock_guard lock(g_mutex);
    DropClosedListeners();
    if (TraceNotify())
        Log("Notify: broadcast {:08X} ({:08X}) to {} listeners", id, param, g_listeners.size());
    for (auto& [event, listener] : g_listeners)
        Enqueue(event, listener, id, param);
}

namespace
{
    // XAM's listener creation (0x816A8E50). The listener's event is a synchronization event in XAM; here it is a
    // notification event that XNotifyGetNext resets when the queue is empty (the same for one waiter).
    // XAM delivers the notifications its table 0x815F7CD8 flags as system-only to system listeners alone; not
    // filtered yet (no such notification is sent by the runtime).
    uint32_t CreateListener(uint64_t mask, uint32_t maxVersion, bool system)
    {
        maxVersion = std::min<uint32_t>(maxVersion, 10);

        const uint32_t event = kernel::CreateOwnedObject(kernel::kKEventSize);
        kernel::InitializeDispatcherHeader(event, kernel::kObjectEventNotification, 0);
        const uint32_t handle = kernel::CreateHandle(event);
        kernel::SetObjectCleanup(event, [event] {
            std::lock_guard lock(g_closedMutex);
            g_closed.push_back(event);
        });

        std::lock_guard lock(g_mutex);
        DropClosedListeners();
        Listener& listener = g_listeners[event];
        listener.mask = mask;
        listener.maxVersion = maxVersion;
        listener.system = system;
        if (!g_systemStartupSent && (mask & kNotifySystem))
        {
            g_systemStartupSent = true;
            Enqueue(event, listener, kXnSysUi, 0);              // system UI not showing
            Enqueue(event, listener, kXnSysSignInChanged, kernel::users::SignedInMask()); // the users signed in
            // Both drives empty (media type 0, as XamLoaderGetMediaInfo answers). The dash refreshes its content
            // manager on 0x8000000D (0x92182530 -> 0x92180388), which My Games' library waits for.
            Enqueue(event, listener, kXnSysDvdMediaChanged, 0);
            Enqueue(event, listener, kXnSysSecondDriveMediaChanged, 0);
        }
        if (!g_liveStartupSent && (mask & kNotifyLive))
        {
            g_liveStartupSent = true;
            Enqueue(event, listener, kXnLiveConnectionChanged, kOnlineLogonDisconnected);
            Enqueue(event, listener, kXnLiveLinkStateChanged, 0);
        }
        if (!g_xmpStartupSent && (mask & kNotifyXmp))
        {
            g_xmpStartupSent = true;
            Enqueue(event, listener, kXnXmpPlaybackControllerChanged, 1); // title controls playback (no custom soundtrack)
        }
        return handle;
    }
}

// HANDLE XamNotifyCreateListener(ULONGLONG Areas, DWORD MaxVersion): a listener for the caller's process type.
GUEST_IMPORT(XamNotifyCreateListener)
{
    auto& mem = guest::g_memory;
    const uint32_t thread = mem.Read32(ctx.r13.u32 + kernel::pcr::kCurrentThread);
    const bool system = mem.Read8(thread + kernel::kthread::kProcessType) == kernel::kProcessTypeSystem;
    ctx.r3.u64 = CreateListener(ctx.r3.u64, ctx.r4.u32, system);
}

// HANDLE XamNotifyCreateListenerInternal(ULONGLONG Areas, BOOL System, DWORD MaxVersion)
GUEST_IMPORT(XamNotifyCreateListenerInternal)
{
    ctx.r3.u64 = CreateListener(ctx.r3.u64, ctx.r5.u32, ctx.r4.u32 != 0);
}

// BOOL XNotifyGetNext(HANDLE Listener, DWORD MatchId, PDWORD Id, PULONG_PTR Param)
// MatchId 0 dequeues the oldest notification; otherwise the oldest notification with that ID.
GUEST_IMPORT(XNotifyGetNext)
{
    auto& mem = guest::g_memory;
    const uint32_t matchId = ctx.r4.u32;
    const uint32_t idPtr = ctx.r5.u32;
    const uint32_t paramPtr = ctx.r6.u32;
    if (paramPtr)
        mem.Write32(paramPtr, 0);
    if (idPtr == 0)
    {
        ctx.r3.u64 = 0;
        return;
    }
    mem.Write32(idPtr, 0);

    const uint32_t event = kernel::LookupHandle(ctx.r3.u32);
    std::lock_guard lock(g_mutex);
    DropClosedListeners();
    auto it = g_listeners.find(event);
    if (it == g_listeners.end())
    {
        ctx.r3.u64 = 0;
        return;
    }
    auto& queue = it->second.queue;
    auto found = queue.begin();
    if (matchId != 0)
        while (found != queue.end() && found->first != matchId)
            ++found;
    if (found == queue.end())
    {
        ctx.r3.u64 = 0;
        return;
    }
    mem.Write32(idPtr, found->first);
    if (paramPtr)
        mem.Write32(paramPtr, found->second);
    if (TraceNotify())
        Log("Notify: listener {:08X} took {:08X} ({:08X})", ctx.r3.u32, found->first, found->second);
    queue.erase(found);
    if (queue.empty())
        SetSignal(event, false);
    ctx.r3.u64 = 1;
}

// VOID XNotifyPositionUI(DWORD Position): where the system's notification toasts appear (XNOTIFYUI_POS_TOP 1,
// BOTTOM 2, LEFT 4, RIGHT 8; neither bit of a pair = centred). XAM stores it for its toast scene (xam 0x816ABA78).
GUEST_IMPORT(XNotifyPositionUI)
{
    Log("XNotifyPositionUI({:X})", ctx.r3.u32);
    xui::SetNotificationPosition(ctx.r3.u32);
}

// VOID XNotifyBroadcast(DWORD Id, ULONG_PTR Param) (XAM 0x816A93C8): queues the notification for every listener whose
// areas include it. (XAM also adds the id to a list at 0x81AA06E4 of broadcast ids; its use is not established.)
GUEST_IMPORT(XNotifyBroadcast)
{
    Log("XNotifyBroadcast({:08X}, {:08X})", ctx.r3.u32, ctx.r4.u32);
    kernel::BroadcastNotification(ctx.r3.u32, ctx.r4.u32);
}
