// Controller mirroring between two copies on one PC: input_mirror.h.

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <winsock2.h>
#include <ws2tcpip.h>

#include "input_mirror.h"
#include "log.h"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include <string>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

namespace input::mirror
{
    namespace
    {
        // Packet: magic "CBIM", buttons, triggers, the four stick axes (little-endian), 16 bytes.
        constexpr uint32_t kMagic = 0x4D494243;
        struct Packet
        {
            uint32_t magic;
            uint16_t buttons;
            uint8_t leftTrigger, rightTrigger;
            int16_t thumbLX, thumbLY, thumbRX, thumbRY;
        };
        static_assert(sizeof(Packet) == 16);

        SOCKET g_sendSocket = INVALID_SOCKET;
        std::atomic<bool> g_sendOn = false;
        sockaddr_in g_target = {};
        std::mutex g_mutex;
        PadState g_received;
        std::chrono::steady_clock::time_point g_receivedAt{};
        bool g_haveReceived = false;

        void StartWinsock()
        {
            static std::once_flag once;
            std::call_once(once, [] {
                WSADATA data;
                WSAStartup(MAKEWORD(2, 2), &data);
            });
        }

        int16_t Further(int16_t a, int16_t b)
        {
            return std::abs(int(b)) > std::abs(int(a)) ? b : a;
        }
    }

    void Start(bool scripted)
    {
        g_sendOn = scripted;
        if (const char* to = std::getenv("CB_INPUT_MIRROR_TO"); to && *to)
        {
            StartWinsock();
            std::string text = to;
            std::string host = "127.0.0.1";
            if (const size_t colon = text.rfind(':'); colon != std::string::npos)
            {
                host = text.substr(0, colon);
                text = text.substr(colon + 1);
            }
            g_target.sin_family = AF_INET;
            g_target.sin_port = htons(uint16_t(std::atoi(text.c_str())));
            if (inet_pton(AF_INET, host.c_str(), &g_target.sin_addr) != 1 || g_target.sin_port == 0)
                Log("Input mirror: CB_INPUT_MIRROR_TO \"{}\" is not <port> or <IPv4>:<port>: not sending", to);
            else
            {
                g_sendSocket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
                Log("Input mirror: user 0's controller goes to {}:{} every frame{}", host, ntohs(g_target.sin_port),
                    scripted ? "" : " once F9 turns it on");
            }
        }
        if (const char* from = std::getenv("CB_INPUT_MIRROR_FROM"); from && *from)
        {
            StartWinsock();
            const uint16_t port = uint16_t(std::atoi(from));
            SOCKET s = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
            sockaddr_in local = {};
            local.sin_family = AF_INET;
            local.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
            local.sin_port = htons(port);
            if (s == INVALID_SOCKET || port == 0 || ::bind(s, reinterpret_cast<sockaddr*>(&local), sizeof(local)) != 0)
            {
                Log("Input mirror: cannot receive on 127.0.0.1:{} (CB_INPUT_MIRROR_FROM)", from);
                return;
            }
            Log("Input mirror: user 0 also takes the controller mirrored to 127.0.0.1:{}", port);
            std::thread([s] {
                for (;;)
                {
                    Packet p;
                    const int n = ::recv(s, reinterpret_cast<char*>(&p), sizeof(p), 0);
                    if (n == SOCKET_ERROR)
                    {
                        Sleep(10);
                        continue;
                    }
                    if (n != int(sizeof(p)) || p.magic != kMagic)
                        continue;
                    PadState pad;
                    pad.buttons = p.buttons;
                    pad.leftTrigger = p.leftTrigger;
                    pad.rightTrigger = p.rightTrigger;
                    pad.thumbLX = p.thumbLX;
                    pad.thumbLY = p.thumbLY;
                    pad.thumbRX = p.thumbRX;
                    pad.thumbRY = p.thumbRY;
                    std::lock_guard lock(g_mutex);
                    if (!g_haveReceived)
                        Log("Input mirror: the first mirrored controller state arrived");
                    g_haveReceived = true;
                    g_received = pad;
                    g_receivedAt = std::chrono::steady_clock::now();
                }
            }).detach();
        }
    }

    void Toggle()
    {
        g_sendOn = !g_sendOn;
        Log("Input mirror: {} (F9)", g_sendOn ? "on: this controller also drives the other copy" : "off");
    }

    bool Sending()
    {
        return g_sendSocket != INVALID_SOCKET;
    }

    void Send(const PadState& pad)
    {
        if (g_sendSocket == INVALID_SOCKET || !g_sendOn)
            return;
        const Packet p = { kMagic, pad.buttons, pad.leftTrigger, pad.rightTrigger,
                           pad.thumbLX, pad.thumbLY, pad.thumbRX, pad.thumbRY };
        ::sendto(g_sendSocket, reinterpret_cast<const char*>(&p), sizeof(p), 0,
                 reinterpret_cast<const sockaddr*>(&g_target), sizeof(g_target));
    }

    bool Received(PadState& pad)
    {
        std::lock_guard lock(g_mutex);
        if (!g_haveReceived || std::chrono::steady_clock::now() - g_receivedAt > std::chrono::seconds(1))
            return false;
        pad = g_received;
        return true;
    }

    PadState Merge(const PadState& own, const PadState& mirrored)
    {
        PadState pad = own;
        pad.buttons |= mirrored.buttons;
        pad.leftTrigger = std::max(own.leftTrigger, mirrored.leftTrigger);
        pad.rightTrigger = std::max(own.rightTrigger, mirrored.rightTrigger);
        pad.thumbLX = Further(own.thumbLX, mirrored.thumbLX);
        pad.thumbLY = Further(own.thumbLY, mirrored.thumbLY);
        pad.thumbRX = Further(own.thumbRX, mirrored.thumbRX);
        pad.thumbRY = Further(own.thumbRY, mirrored.thumbRY);
        return pad; // own.guide only
    }
}
