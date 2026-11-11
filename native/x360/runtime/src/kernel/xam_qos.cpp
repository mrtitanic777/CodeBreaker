// XNet's QoS (NetDll_XNetQos*) on our Xbox LIVE.
//
// On a console a host calls XNetQosListen for its session's XNKID (ENABLE, SET_DATA: a block of at most the
// XNetStartupParams limit, usually a few hundred bytes) and its XNet answers other consoles' QoS probes on that XNKID
// with the block. A client calls XNetQosLookup with the hosts a search found (XNADDR, XNKID, XNKEY); XNet allocates
// an XNQOS, probes each host and fills one XNQOSINFO per host as answers come, decrementing cxnqosPending and
// setting the event after each; XNetQosRelease frees it. Minecraft lists a friend's game only when its entry has
// TARGET_CONTACTED and the host's data (its GameSessionData: host name, net version, joinable).
//
// Here the host's data goes to our server with its session (protocol `session_qos`) and a lookup reads it back
// (`session`), so it works through NAT with no probe between the consoles. INFER: the round-trip time is this
// client's round trip to the server for that request, and the bandwidth a fixed estimate (the console measures both
// from its probes to the host). A system link session (XNKID top nibble 0) never reaches the server: its entries
// complete without contact (QoS between PCs on a LAN is not written). Structures (winsockx.h):
//   XNQOS     { UINT cxnqos; UINT cxnqosPending; XNQOSINFO axnqosinfo[] }                        8 + 0x18 each
//   XNQOSINFO { BYTE bFlags, bReserved; WORD cProbesXmit, cProbesRecv, cbData; BYTE* pbData;
//               WORD wRttMinInMsecs, wRttMedInMsecs; DWORD dwUpBitsPerSec, dwDnBitsPerSec }       0x18

#include "kernel/kernel.h"
#include "kernel/objects.h"
#include "kernel/xam_matchmaking.h"
#include "live/client.h"
#include "log.h"
#include "memory.h"

#include <algorithm>
#include <chrono>
#include <cstring>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <thread>
#include <vector>

namespace
{
    constexpr uint32_t kWsaEInval = 10022;
    constexpr uint32_t kQosDataMax = 1024; // the server's limit (protocol `session_qos`)
    constexpr uint32_t kInfoSize = 0x18;

    constexpr uint32_t kListenEnable = 0x1, kListenDisable = 0x2, kListenSetData = 0x4, kListenSetBitsPerSec = 0x8,
                       kListenRelease = 0x10;
    constexpr uint8_t kInfoComplete = 0x01, kInfoContacted = 0x02, kInfoDisabled = 0x04, kInfoDataReceived = 0x08;

    // INFER: the bandwidth a lookup reports both ways (the console estimates it from its probe pairs).
    constexpr uint32_t kEstimatedBitsPerSec = 4'000'000;

    bool OnlineId(uint64_t id)
    {
        return (id >> 60) == 0x8; // XNKID of an online session (kernel/xam_msg.cpp XSessionCreate)
    }

    // ---- the host side ----
    struct Listener
    {
        bool enabled = false;
        std::vector<uint8_t> data;
        uint32_t bitsPerSec = 0;
    };
    std::mutex g_listenMutex;
    std::map<uint64_t, Listener> g_listeners; // by XNKID

    // What a client's lookup should read for session `id`: the block while listening, nothing otherwise.
    void Publish(uint64_t id, std::vector<uint8_t> data)
    {
        if (!OnlineId(id))
            return;
        const auto user = kernel::matchmaking::HostUser(id);
        if (!user)
        {
            Log("QoS: listen data for session {:016X}, which this console does not host on Xbox LIVE: not sent", id);
            return;
        }
        // After the session's create (the same worker runs the session requests in order).
        kernel::matchmaking::PostRequest([id, u = *user, data = std::move(data)] {
            if (const auto error = live::SetGameSessionQos(u, id, data))
                Log("QoS: sending session {:016X}'s listen data failed: {}", id, *error);
        });
    }

    // ---- the client side ----
    struct Lookup
    {
        std::vector<uint32_t> blocks; // guest allocations: the XNQOS first, then each host's data
        bool done = false, released = false;
    };
    std::mutex g_lookupMutex;
    std::map<uint32_t, std::shared_ptr<Lookup>> g_lookups; // by the XNQOS's guest address

    void FreeBlocks(const Lookup& l)
    {
        for (const uint32_t b : l.blocks)
            guest::g_memory.SystemFree(b);
    }

    void SignalEvent(uint32_t handle)
    {
        if (handle == 0)
            return;
        const uint32_t event = kernel::LookupHandle(handle);
        if (event == 0)
            return; // the title closed it already
        auto lock = kernel::LockDispatcher();
        guest::g_memory.Write32(event + kernel::header::kSignalState, 1);
        kernel::NotifyDispatcherChange(event);
    }

    std::optional<uint32_t> LiveUser()
    {
        for (uint32_t u = 0; u < 4; u++)
            if (live::SignedIn(u))
                return u;
        return std::nullopt;
    }

    struct Target
    {
        uint64_t id = 0;   // a console's XNKID; 0 for a security gateway
        bool gateway = false;
    };

    // One entry's result, written into the XNQOS; then the pending count and the event.
    void Finish(uint32_t xnqos, uint32_t index, uint8_t flags, uint16_t probesSent, uint16_t probesReceived,
                uint32_t data, uint16_t dataSize, uint16_t rtt, uint32_t hEvent)
    {
        auto& mem = guest::g_memory;
        const uint32_t e = xnqos + 8 + index * kInfoSize;
        mem.Write8(e + 1, 0);
        mem.Write16(e + 2, probesSent);
        mem.Write16(e + 4, probesReceived);
        mem.Write16(e + 6, dataSize);
        mem.Write32(e + 8, data);
        mem.Write16(e + 0xC, rtt);
        mem.Write16(e + 0xE, rtt);
        const bool contacted = flags & kInfoContacted;
        mem.Write32(e + 0x10, contacted ? kEstimatedBitsPerSec : 0);
        mem.Write32(e + 0x14, contacted ? kEstimatedBitsPerSec : 0);
        mem.Write8(e, flags); // the flags last: COMPLETE says the rest is there
        mem.AtomicAdd32(xnqos + 4, -1);
        SignalEvent(hEvent);
    }

    void RunLookup(uint32_t xnqos, std::shared_ptr<Lookup> lookup, std::vector<Target> targets, uint16_t probes,
                   uint32_t hEvent)
    {
        const auto user = LiveUser();
        for (uint32_t i = 0; i < targets.size(); i++)
        {
            {
                std::lock_guard lock(g_lookupMutex);
                if (lookup->released)
                    break; // the title released it before it finished: nobody reads the rest
            }
            const Target& t = targets[i];
            if (!user || (!t.gateway && !OnlineId(t.id)))
            {
                Finish(xnqos, i, kInfoComplete, probes, 0, 0, 0, 0, hEvent);
                continue;
            }
            const auto start = std::chrono::steady_clock::now();
            std::optional<live::GameSessionInfo> session;
            if (!t.gateway)
                session = live::GameSession(*user, t.id);
            const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
            const uint16_t rtt = uint16_t(std::clamp<long long>(ms, 1, 0xFFFF));
            if (t.gateway)
            {
                Finish(xnqos, i, kInfoComplete | kInfoContacted, probes, probes, 0, 0, rtt, hEvent);
                continue;
            }
            if (!session)
            {
                Log("QoS: session {:016X} not found on the server", t.id);
                Finish(xnqos, i, kInfoComplete, probes, 0, 0, 0, 0, hEvent);
                continue;
            }
            uint8_t flags = kInfoComplete | kInfoContacted;
            uint32_t data = 0;
            const uint16_t size = uint16_t(std::min<size_t>(session->qos.size(), kQosDataMax));
            if (size == 0)
                flags |= kInfoDisabled; // INFER: the host listens with no data, or not at all (the server keeps no flag)
            else
            {
                data = guest::g_memory.SystemAlloc(size);
                std::memcpy(guest::g_memory.Translate(data), session->qos.data(), size);
                flags |= kInfoDataReceived;
                std::lock_guard lock(g_lookupMutex);
                lookup->blocks.push_back(data);
            }
            Log("QoS: session {:016X}: contacted, {} bytes of data, {} ms", t.id, size, rtt);
            Finish(xnqos, i, flags, probes, probes, data, size, rtt, hEvent);
        }
        std::lock_guard lock(g_lookupMutex);
        lookup->done = true;
        if (lookup->released)
            FreeBlocks(*lookup);
    }

    uint32_t StartLookup(std::vector<Target> targets, uint32_t probes, uint32_t hEvent, uint32_t ppxnqos)
    {
        auto& mem = guest::g_memory;
        const uint32_t n = uint32_t(targets.size());
        const uint32_t xnqos = mem.SystemAlloc(8 + std::max<uint32_t>(n, 1) * kInfoSize);
        mem.Write32(xnqos, n);
        mem.Write32(xnqos + 4, n);
        auto lookup = std::make_shared<Lookup>();
        lookup->blocks.push_back(xnqos);
        {
            std::lock_guard lock(g_lookupMutex);
            g_lookups[xnqos] = lookup;
        }
        mem.Write32(ppxnqos, xnqos);
        const uint16_t p = uint16_t(std::clamp<uint32_t>(probes ? probes : 8, 1, 0xFFFF));
        std::thread(RunLookup, xnqos, lookup, std::move(targets), p, hEvent).detach();
        return xnqos;
    }
}

// INT NetDll_XNetQosListen(XNCALLER_TYPE Caller, const XNKID* pxnkid, const BYTE* pb, UINT cb, DWORD dwBitsPerSec,
// DWORD dwFlags)
GUEST_IMPORT(NetDll_XNetQosListen)
{
    auto& mem = guest::g_memory;
    const uint32_t keyId = ctx.r4.u32, data = ctx.r5.u32, size = ctx.r6.u32, bitsPerSec = ctx.r7.u32, flags = ctx.r8.u32;
    if (keyId == 0 || flags == 0 || (flags & ~0x1Fu) || ((flags & kListenSetData) && size && data == 0) ||
        ((flags & kListenEnable) && (flags & kListenDisable)))
    {
        ctx.r3.u64 = kWsaEInval;
        return;
    }
    const uint64_t id = mem.Read64(keyId);
    std::optional<std::vector<uint8_t>> publish;
    {
        std::lock_guard lock(g_listenMutex);
        if (flags & kListenRelease)
        {
            const bool had = g_listeners.erase(id) != 0;
            if (had)
                publish = std::vector<uint8_t>{};
        }
        else
        {
            Listener& l = g_listeners[id];
            const bool wasPublic = l.enabled && !l.data.empty();
            const std::vector<uint8_t> before = l.data;
            if (flags & kListenSetData)
            {
                const uint32_t n = std::min(size, kQosDataMax);
                if (n < size)
                    Log("QoS: session {:016X}'s listen data is {} bytes: the first {} are kept", id, size, n);
                l.data.assign(mem.Translate(data), mem.Translate(data) + n);
            }
            if (flags & kListenSetBitsPerSec)
                l.bitsPerSec = bitsPerSec;
            if (flags & kListenEnable)
                l.enabled = true;
            if (flags & kListenDisable)
                l.enabled = false;
            const bool isPublic = l.enabled && !l.data.empty();
            if (isPublic != wasPublic || (isPublic && l.data != before))
                publish = isPublic ? l.data : std::vector<uint8_t>{};
        }
    }
    Log("QoS: listen on session {:016X}, flags {:X}, {} bytes", id, flags, (flags & kListenSetData) ? size : 0);
    if (publish)
        Publish(id, std::move(*publish));
    ctx.r3.u64 = 0;
}

// INT NetDll_XNetQosLookup(XNCALLER_TYPE Caller, UINT cxna, const XNADDR* apxna[], const XNKID* apxnkid[],
// const XNKEY* apxnkey[], UINT cina, const IN_ADDR aina[], const DWORD adwServiceId[], UINT cProbes,
// DWORD dwBitsPerSec, DWORD dwFlags, WSAEVENT hEvent, XNQOS** ppxnqos): the arguments after the eighth on the stack
// (+0x54, +0x5C, +0x64, +0x6C, +0x74).
GUEST_IMPORT(NetDll_XNetQosLookup)
{
    auto& mem = guest::g_memory;
    const uint32_t consoles = ctx.r4.u32, keyIds = ctx.r6.u32, gateways = ctx.r8.u32;
    const uint32_t sp = ctx.r1.u32;
    const uint32_t probes = mem.Read32(sp + 0x54), hEvent = mem.Read32(sp + 0x6C), ppxnqos = mem.Read32(sp + 0x74);
    if (ppxnqos == 0 || consoles + gateways == 0 || (consoles && keyIds == 0) || consoles + gateways > 0x1000)
    {
        ctx.r3.u64 = kWsaEInval;
        return;
    }
    std::vector<Target> targets;
    for (uint32_t i = 0; i < consoles; i++)
    {
        const uint32_t p = mem.Read32(keyIds + i * 4);
        targets.push_back({ p ? mem.Read64(p) : 0, false });
    }
    for (uint32_t i = 0; i < gateways; i++)
        targets.push_back({ 0, true });
    const uint32_t xnqos = StartLookup(std::move(targets), probes, hEvent, ppxnqos);
    Log("QoS: lookup of {} consoles and {} gateways ({} probes) -> {:08X}", consoles, gateways, probes, xnqos);
    ctx.r3.u64 = 0;
}

// INT NetDll_XNetQosServiceLookup(XNCALLER_TYPE Caller, DWORD dwFlags, WSAEVENT hEvent, XNQOS** ppxnqos): QoS to the
// Xbox LIVE service itself: one entry (our server).
GUEST_IMPORT(NetDll_XNetQosServiceLookup)
{
    const uint32_t hEvent = ctx.r5.u32, ppxnqos = ctx.r6.u32;
    if (ppxnqos == 0)
    {
        ctx.r3.u64 = kWsaEInval;
        return;
    }
    const uint32_t xnqos = StartLookup({ Target{ 0, true } }, 8, hEvent, ppxnqos);
    Log("QoS: service lookup (flags {:X}) -> {:08X}", ctx.r4.u32, xnqos);
    ctx.r3.u64 = 0;
}

// INT NetDll_XNetQosRelease(XNCALLER_TYPE Caller, XNQOS* pxnqos)
GUEST_IMPORT(NetDll_XNetQosRelease)
{
    const uint32_t xnqos = ctx.r4.u32;
    std::lock_guard lock(g_lookupMutex);
    const auto it = g_lookups.find(xnqos);
    if (it == g_lookups.end())
    {
        ctx.r3.u64 = kWsaEInval;
        return;
    }
    it->second->released = true;
    if (it->second->done)
        FreeBlocks(*it->second);
    g_lookups.erase(it);
    ctx.r3.u64 = 0;
}

// INT NetDll_XNetQosGetListenStats(XNCALLER_TYPE Caller, const XNKID* pxnkid, XNQOSLISTENSTATS* pQosListenStats):
// dwSizeOfStruct (0x1C) set by the caller, then six counters. Probes never reach this console (the server answers
// lookups), so the counters stay 0.
GUEST_IMPORT(NetDll_XNetQosGetListenStats)
{
    auto& mem = guest::g_memory;
    const uint32_t stats = ctx.r5.u32;
    if (ctx.r4.u32 == 0 || stats == 0 || mem.Read32(stats) != 0x1C)
    {
        ctx.r3.u64 = kWsaEInval;
        return;
    }
    std::memset(mem.Translate(stats + 4), 0, 0x18);
    ctx.r3.u64 = 0;
}
