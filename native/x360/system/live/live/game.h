#pragma once

// Game data (docs/x360/network_protocol.md "Game data"): a title's datagrams to other consoles, relayed by the
// server over its UDP port, or sent straight to the console when a direct path was punched ("Direct connections
// (P2P)"). A console is addressed by a handle 10.x.y.z made of its online XUID's low 24 bits.
// Started by a runtime whose titles play online (it hands received datagrams to its virtual sockets).

#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace live::game
{
    // A datagram for this PC's player `toXuid` (one of its signed-in users) from `fromXuid`'s console.
    using Receiver = std::function<void(uint64_t toXuid, uint64_t fromXuid, uint16_t fromPort, uint16_t toPort,
                                        const std::vector<uint8_t>& data)>;

    // Starts the UDP thread (keep-alives for every signed-in user every 2 s, received datagrams to `receiver`).
    void Start(Receiver receiver);
    void Stop();
    // Sends a datagram from `user` (signed in) to the console with address handle `toHandle`: straight to it when a
    // direct path to that console is up, else through the server. False when the user is not signed in or the client is
    // not connected.
    bool Send(uint32_t user, uint32_t toHandle, uint16_t toPort, uint16_t fromPort, const uint8_t* data, size_t size);

    // The address handle of an online XUID (10.x.y.z).
    constexpr uint32_t AddressHandle(uint64_t xuid)
    {
        return 0x0A000000 | uint32_t(xuid & 0xFFFFFF);
    }

    // ---- direct connections (P2P) ----
    // The server's `peer` event for `user`: the console `xuid`'s public endpoint, LAN endpoints ("a.b.c.d:port") and
    // the pair's secret (32 hex digits). Called by the client's connection thread; starts punching a hole to the peer.
    void OnPeerEvent(uint32_t user, uint64_t xuid, const std::string& publicEndpoint,
                     const std::vector<std::string>& lanEndpoints, const std::string& tokenHex);

    // For diagnostics and tests.
    struct PeerStatus
    {
        uint32_t user;
        uint64_t xuid;
        bool direct;   // traffic to it goes straight to it now
    };
    std::vector<PeerStatus> Peers();
    struct Stats
    {
        uint64_t relaySent = 0, relayReceived = 0, directSent = 0, directReceived = 0;
    };
    Stats GetStats();
    // The UDP port the game socket is bound to (0: not open); a test aims forged packets at it.
    uint16_t LocalPort();
    // Test hook: drops every direct packet (probes and game data, both ways) as a path that stopped working would.
    void SetDirectBlocked(bool blocked);
    // Direct connections off (true): no LAN endpoints reported, peer events ignored; everything goes through the server.
    void SetDirectDisabled(bool disabled);
}
