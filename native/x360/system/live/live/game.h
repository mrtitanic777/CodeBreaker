#pragma once

// Game data (docs/x360/network_protocol.md "Game data"): a title's datagrams to other consoles, relayed by the
// server over its UDP port. A console is addressed by a handle 10.x.y.z made of its online XUID's low 24 bits.
// Started by a runtime whose titles play online (it hands received datagrams to its virtual sockets).

#include <cstddef>
#include <cstdint>
#include <functional>
#include <vector>

namespace live::game
{
    // A datagram for this PC's player `toXuid` (one of its signed-in users) from `fromXuid`'s console.
    using Receiver = std::function<void(uint64_t toXuid, uint64_t fromXuid, uint16_t fromPort, uint16_t toPort,
                                        const std::vector<uint8_t>& data)>;

    // Starts the UDP thread (keep-alives for every signed-in user every 2 s, received datagrams to `receiver`).
    void Start(Receiver receiver);
    void Stop();
    // Sends a datagram from `user` (signed in) to the console with address handle `toHandle`; false when the user is
    // not signed in or the client is not connected.
    bool Send(uint32_t user, uint32_t toHandle, uint16_t toPort, uint16_t fromPort, const uint8_t* data, size_t size);

    // The address handle of an online XUID (10.x.y.z).
    constexpr uint32_t AddressHandle(uint64_t xuid)
    {
        return 0x0A000000 | uint32_t(xuid & 0xFFFFFF);
    }
}
