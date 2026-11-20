#include "live/titleserver.h"

#include "log.h"

#include <nlohmann/json.hpp>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <format>
#include <map>
#include <memory>
#include <mutex>

namespace live::detail
{
    // client.cpp
    std::optional<std::string> RawRequest(const std::string& request);
    void SetRawEventHandler(std::function<void(const std::string&)> handler);
}

namespace live::ts
{
    namespace
    {
        using json = nlohmann::json;

        std::string Base64(const uint8_t* data, size_t size)
        {
            static const char kAlphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
            std::string out;
            out.reserve((size + 2) / 3 * 4);
            for (size_t i = 0; i < size; i += 3)
            {
                uint32_t v = uint32_t(data[i]) << 16 | (i + 1 < size ? uint32_t(data[i + 1]) << 8 : 0) |
                             (i + 2 < size ? data[i + 2] : 0);
                out += kAlphabet[v >> 18 & 63];
                out += kAlphabet[v >> 12 & 63];
                out += i + 1 < size ? kAlphabet[v >> 6 & 63] : '=';
                out += i + 2 < size ? kAlphabet[v & 63] : '=';
            }
            return out;
        }

        std::vector<uint8_t> FromBase64(const std::string& text)
        {
            std::vector<uint8_t> out;
            uint32_t acc = 0;
            int bits = 0;
            for (char c : text)
            {
                int v;
                if (c >= 'A' && c <= 'Z') v = c - 'A';
                else if (c >= 'a' && c <= 'z') v = c - 'a' + 26;
                else if (c >= '0' && c <= '9') v = c - '0' + 52;
                else if (c == '+') v = 62;
                else if (c == '/') v = 63;
                else continue;
                acc = acc << 6 | uint32_t(v);
                bits += 6;
                if (bits >= 8)
                {
                    bits -= 8;
                    out.push_back(uint8_t(acc >> bits));
                    acc &= (1u << bits) - 1;
                }
            }
            return out;
        }

        struct Stream
        {
            OnData onData;
            OnClosed onClosed;
            bool connected = false, closed = false;
            std::string reason;
        };

        std::mutex g_mutex;
        std::condition_variable g_changed;
        std::map<std::pair<uint32_t, uint32_t>, std::shared_ptr<Stream>> g_streams; // (user, stream)
        OnDatagram g_datagram;
        std::atomic<uint32_t> g_nextStream = 1;
        std::once_flag g_once;

        std::string Hex8(uint32_t v) { return std::format("{:08X}", v); }

        std::optional<json> Call(json request, std::string* error)
        {
            const auto text = detail::RawRequest(request.dump());
            if (!text)
            {
                if (error)
                    *error = "not connected";
                return std::nullopt;
            }
            json reply = json::parse(*text, nullptr, false);
            if (!reply.is_object())
            {
                if (error)
                    *error = "bad reply";
                return std::nullopt;
            }
            if (!reply.value("ok", false))
            {
                if (error)
                    *error = reply.value("error", std::string("failed"));
                return std::nullopt;
            }
            return reply;
        }

        // The "ts_*" events of the server (the client's connection thread).
        void OnEvent(const std::string& line)
        {
            const json j = json::parse(line, nullptr, false);
            if (!j.is_object())
                return;
            const std::string op = j.value("op", std::string());
            const uint32_t user = j.value("user", 0u);
            if (op == "ts_udp")
            {
                OnDatagram handler;
                {
                    std::lock_guard lock(g_mutex);
                    handler = g_datagram;
                }
                if (handler && j.contains("data") && j["data"].is_string())
                    handler(user, j.value("service", 0u), uint16_t(j.value("port", 0u)), uint16_t(j.value("src_port", 0u)),
                            FromBase64(j["data"].get<std::string>()));
                return;
            }
            const uint32_t id = j.value("stream", 0u);
            std::shared_ptr<Stream> s;
            {
                std::lock_guard lock(g_mutex);
                const auto it = g_streams.find({ user, id });
                if (it == g_streams.end())
                    return;
                s = it->second;
            }
            if (op == "ts_connected")
            {
                std::lock_guard lock(g_mutex);
                s->connected = true;
                g_changed.notify_all();
            }
            else if (op == "ts_data")
            {
                if (j.contains("data") && j["data"].is_string() && s->onData)
                {
                    const auto data = FromBase64(j["data"].get<std::string>());
                    s->onData(data.data(), data.size());
                }
            }
            else if (op == "ts_closed")
            {
                OnClosed closed;
                {
                    std::lock_guard lock(g_mutex);
                    if (s->closed)
                        return;
                    s->closed = true;
                    s->reason = j.value("reason", std::string());
                    closed = s->onClosed;
                    g_streams.erase({ user, id });
                    g_changed.notify_all();
                }
                if (closed)
                    closed(s->reason);
            }
        }

        void Start()
        {
            std::call_once(g_once, [] { detail::SetRawEventHandler(OnEvent); });
        }
    }

    std::optional<ServerList> Enumerate(uint32_t user, uint32_t titleId, uint32_t start, uint32_t max, std::string* error)
    {
        const auto reply = Call({ { "op", "title_servers" }, { "user", user }, { "title_id", Hex8(titleId) },
                                  { "start", start }, { "max", std::clamp<uint32_t>(max, 1, 1000) } }, error);
        if (!reply)
            return std::nullopt;
        ServerList list;
        list.total = (*reply).value("total", 0u);
        if (const auto it = reply->find("servers"); it != reply->end() && it->is_array())
            for (const json& s : *it)
            {
                Server server;
                server.name = s.value("name", std::string());
                server.info = s.value("info", std::string());
                server.flags = s.value("flags", 0u);
                server.serviceId = s.value("service_id", 0u);
                server.tcp = s.value("tcp", true);
                server.udp = s.value("udp", true);
                list.servers.push_back(std::move(server));
            }
        return list;
    }

    uint32_t Open(uint32_t user, uint32_t titleId, uint32_t serviceId, uint16_t port, uint16_t sourcePort, OnData onData,
                  OnClosed onClosed, std::string* error)
    {
        Start();
        const uint32_t id = g_nextStream++ & 0x7FFFFFFF;
        auto stream = std::make_shared<Stream>();
        stream->onData = std::move(onData);
        stream->onClosed = std::move(onClosed);
        {
            std::lock_guard lock(g_mutex);
            g_streams[{ user, id }] = stream;
        }
        std::string failure;
        if (!Call({ { "op", "ts_open" }, { "user", user }, { "title_id", Hex8(titleId) }, { "stream", id },
                    { "service", serviceId }, { "port", port }, { "src_port", sourcePort } }, &failure))
        {
            std::lock_guard lock(g_mutex);
            g_streams.erase({ user, id });
            if (error)
                *error = failure;
            return 0;
        }
        std::unique_lock lock(g_mutex);
        const bool done = g_changed.wait_for(lock, std::chrono::seconds(6), [&] { return stream->connected || stream->closed; });
        if (!done || stream->closed || !stream->connected)
        {
            const std::string why = !done ? "timed out" : stream->reason.empty() ? "closed" : stream->reason;
            g_streams.erase({ user, id });
            lock.unlock();
            if (!done)
                Close(user, id);   // (the server may still be connecting: end it)
            if (error)
                *error = why;
            return 0;
        }
        return id;
    }

    bool Send(uint32_t user, uint32_t stream, const uint8_t* data, size_t size, std::string* error)
    {
        for (size_t at = 0; at < size || (size == 0 && at == 0); at += kSendChunk)
        {
            const size_t n = std::min<size_t>(kSendChunk, size - at);
            if (!Call({ { "op", "ts_send" }, { "user", user }, { "stream", stream }, { "data", Base64(data + at, n) } }, error))
                return false;
            if (size == 0)
                break;
        }
        return true;
    }

    void Close(uint32_t user, uint32_t stream)
    {
        {
            std::lock_guard lock(g_mutex);
            g_streams.erase({ user, stream });
        }
        Call({ { "op", "ts_close" }, { "user", user }, { "stream", stream } }, nullptr);
    }

    std::string SendDatagram(uint32_t user, uint32_t titleId, uint32_t serviceId, uint16_t port, uint16_t sourcePort,
                             const uint8_t* data, size_t size)
    {
        Start();
        std::string error;
        if (!Call({ { "op", "ts_udp" }, { "user", user }, { "title_id", Hex8(titleId) }, { "service", serviceId },
                    { "port", port }, { "src_port", sourcePort }, { "data", Base64(data, size) } }, &error))
            return error.empty() ? "failed" : error;
        return "";
    }

    void SetDatagramHandler(OnDatagram handler)
    {
        Start();
        std::lock_guard lock(g_mutex);
        g_datagram = std::move(handler);
    }
}
