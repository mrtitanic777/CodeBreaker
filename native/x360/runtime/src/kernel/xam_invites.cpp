// Game invites on our Xbox LIVE: see kernel/xam_invites.h.

#include "kernel/xam_invites.h"

#include "kernel/kernel.h"
#include "kernel/users.h"
#include "log.h"
#include "memory.h"

#include <cstring>
#include <map>
#include <mutex>
#include <vector>

namespace
{
    constexpr uint32_t kXnLiveInviteAccepted = 0x02000002; // XN_LIVE_INVITE_ACCEPTED
    constexpr uint32_t kErrorInvalidParameter = 0x80070057;
    constexpr uint32_t kMessagePropertyNotFound = 0x80155A03; // what XAM answers when no invite is waiting
    constexpr uint32_t kCreatePresence = 0x2, kCreateInvitesDisabled = 0x100;

    std::mutex g_mutex;
    std::map<uint32_t, live::InviteInfo> g_accepted; // user -> the invite the title has not read yet
    struct Hosted
    {
        uint32_t user = 0, flags = 0;
    };
    std::map<uint64_t, Hosted> g_hosted; // XNKID -> who hosts it here

    void Put32(std::vector<uint8_t>& out, size_t at, uint32_t v)
    {
        for (int i = 0; i < 4; i++)
            out[at + i] = uint8_t(v >> (24 - 8 * i));
    }
    void Put64(std::vector<uint8_t>& out, size_t at, uint64_t v)
    {
        Put32(out, at, uint32_t(v >> 32));
        Put32(out, at + 4, uint32_t(v));
    }

    // The XNADDR of a host as the title sees it: the handle address 10.x.y.z of its online XUID ("Game data").
    void PutHostAddress(std::vector<uint8_t>& out, size_t at, uint64_t xuid)
    {
        const uint8_t handle[4] = { 10, uint8_t(xuid >> 16), uint8_t(xuid >> 8), uint8_t(xuid) };
        std::memcpy(&out[at], handle, 4);     // ina
        std::memcpy(&out[at + 4], handle, 4); // inaOnline
        out[at + 8] = 3074 >> 8;              // wPortOnline
        out[at + 9] = 3074 & 0xFF;
        const uint8_t mac[6] = { 0x00, 0x1D, 0xD8, handle[1], handle[2], handle[3] };
        std::memcpy(&out[at + 10], mac, 6);   // abEnet
        Put64(out, at + 16, xuid);            // abOnline: the XUID first
    }
}

namespace kernel::invites
{
    void Accepted(uint32_t user, const live::InviteInfo& info)
    {
        if (user >= 4)
            return;
        {
            std::lock_guard lock(g_mutex);
            g_accepted[user] = info;
        }
        Log("Invite: user {} accepted {:016X}'s game session {:016X} (title {:08X}{})", user, info.host, info.sessionId,
            info.titleId, info.fromInvite ? ", from an invite" : ", from presence");
        kernel::BroadcastNotification(kXnLiveInviteAccepted, user);
    }

    uint32_t GetAcceptedInfo(uint32_t arguments)
    {
        auto& mem = guest::g_memory;
        const uint32_t count = arguments ? mem.Read32(arguments + 0x200) : 0;
        if (count < 2)
            return kErrorInvalidParameter;
        auto slot = [&](uint32_t i) { return mem.Read32(arguments + i * 16 + 12); }; // { u32 size, pad, u64 pointer }
        if (!slot(0) || !slot(1))
            return kErrorInvalidParameter;
        const uint32_t user = mem.Read32(slot(0)), info = slot(1);
        const auto profile = user < 4 ? kernel::users::SignedIn(user) : std::nullopt;
        if (!profile || profile->onlineXuid == 0)
            return kErrorInvalidParameter;
        live::InviteInfo accepted;
        {
            std::lock_guard lock(g_mutex);
            const auto it = g_accepted.find(user);
            if (it == g_accepted.end())
                return kMessagePropertyNotFound;
            accepted = it->second;
            g_accepted.erase(it);
        }
        std::vector<uint8_t> out(0x54, 0);
        Put64(out, 0, profile->onlineXuid);       // xuidInvitee
        Put64(out, 8, accepted.inviter);          // xuidInviter
        Put32(out, 0x10, accepted.titleId);       // dwTitleID
        Put64(out, 0x14, accepted.sessionId);     // hostInfo.sessionID (XNKID)
        PutHostAddress(out, 0x1C, accepted.host); // hostInfo.hostAddress: the host's handle address
        // hostInfo.keyExchangeKey (0x40, 16 bytes) stays zero: the relay and the direct paths need none
        Put32(out, 0x50, accepted.fromInvite ? 1 : 0); // fFromGameInvite
        std::memcpy(mem.Translate(info), out.data(), out.size());
        Log("Invite: the title read user {}'s accepted invite (session {:016X}, host {:016X})", user, accepted.sessionId,
            accepted.host);
        return 0;
    }

    void SessionHosted(uint32_t user, uint32_t flags, uint64_t sessionId)
    {
        std::lock_guard lock(g_mutex);
        g_hosted[sessionId] = { user, flags };
    }

    void SessionEnded(uint64_t sessionId)
    {
        std::lock_guard lock(g_mutex);
        g_hosted.erase(sessionId);
    }

    std::optional<uint64_t> HostedPresenceSession(uint32_t user)
    {
        std::lock_guard lock(g_mutex);
        for (const auto& [id, h] : g_hosted)
            if (h.user == user && (h.flags & kCreatePresence) && !(h.flags & kCreateInvitesDisabled))
                return id;
        return std::nullopt;
    }
}
