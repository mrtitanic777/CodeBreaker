// XLiveBase messages 0x5806A..0x5806F: XOnlineCallWebService and its Begin / Continue / Close calls (XAM 17559
// 0x818E1358 ...), performed by our own Xbox LIVE: the raw call (URL template, method, flags, the request and response
// schema strings, the caller's request bytes) goes to the server, which builds the HTTP request XAM would send,
// answers it and returns the caller's response buffer (Shared server/xws_engine.py, live::WsCall ...).
//
//   0x5806A CallWebService   {user, url, method, taskBuf, taskSize, flags, req, reqSize, reqSchema, DWORD* status,
//                             resp, respSize, respSchema, relyingParty}                                   (0x38 bytes)
//   0x5806B BeginRequest     the same up to +0x20, then +0x2C respSchema, +0x30 relyingParty, +0x34 HANDLE* out
//   0x5806C ContinueRequest  {h, pv, cb, fMore}: stream request bodies (uploads); none of the dash's calls yet
//   0x5806D BeginResponse    {h, DWORD* status, resp, respSize}: zero-fills resp, then the server's fill
//   0x5806E ContinueResponse {h, pv, cb}: the next min(cb, left) bytes of a stream body
//   0x5806F Close            {h}
// Every one completes through its overlapped as XMsgCompleteIORequest(ov, hr < 0 ? 0x65B : 0, hr, 0); the HTTP status
// is never an error (the caller checks *status). No server: 0x80072EFD (cannot connect), as before. Flag 8: the
// wrapper allocated the task buffer and XAM frees it when the call ends (we never use it: freed when the call starts,
// or at Close for an open call).
//
// Two phases: Prepare reads the message block and everything it points to on the caller's thread (the block is on
// its stack), Run does the network part and writes the results (into the caller's own buffers) on any thread.

#include "kernel/kernel.h"
#include "kernel/web_call.h"
#include "live/client.h"

#include <algorithm>
#include <cstring>
#include <format>
#include <map>
#include <mutex>
#include <string>

PPC_FUNC(__imp__XamFree); // xam_memory.cpp

namespace
{
    constexpr uint32_t kCannotConnect = 0x80072EFD;
    constexpr uint32_t kAccessDenied = 0x80070005;  // a call in the wrong state (XAM's answer)
    constexpr uint32_t kHandleBase = 0xFE5C0000;    // our web-service handles (only ever passed back to us)

    struct Call
    {
        uint32_t server = 0;     // the server's handle
        std::string respSchema;
        uint32_t flags = 0, taskBuffer = 0;
        uint32_t length = 0, offset = 0; // a stream body's length and how much ContinueResponse has delivered
        bool responded = false;
    };
    std::mutex g_mutex;
    std::map<uint32_t, Call> g_calls;
    uint32_t g_next = 1;

    std::string GuestString(uint32_t address)
    {
        if (address == 0)
            return {};
        const char* s = reinterpret_cast<const char*>(guest::g_memory.Translate(address));
        return std::string(s, strnlen(s, 0x10000));
    }

    std::vector<uint8_t> GuestBytes(uint32_t address, uint32_t size)
    {
        if (address == 0 || size == 0)
            return {};
        const uint8_t* p = guest::g_memory.Translate(address);
        return std::vector<uint8_t>(p, p + size);
    }

    void FreeTaskBuffer(PPCContext& ctx, uint32_t flags, uint32_t buffer)
    {
        if ((flags & 8) && buffer)
        {
            PPCContext call = ctx;
            call.r3.u64 = buffer;
            __imp__XamFree(call, guest::g_memory.base);
        }
    }

    // The status and the response buffer as the server returned them (the rest of the buffer zero).
    void WriteResponse(const live::WsResult& r, uint32_t status, uint32_t resp, uint32_t respSize)
    {
        auto& mem = guest::g_memory;
        if (status)
            mem.Write32(status, r.status);
        if (resp && respSize)
        {
            if (r.resp.size() > respSize)
                Fatal("XOnlineCallWebService: {} response bytes for a {}-byte buffer", r.resp.size(), respSize);
            uint8_t* p = mem.Translate(resp);
            std::memset(p, 0, respSize);
            std::memcpy(p, r.resp.data(), r.resp.size());
        }
    }

    live::WsCallRequest ReadCall(uint32_t b, bool begin)
    {
        auto& mem = guest::g_memory;
        live::WsCallRequest c;
        const uint32_t user = mem.Read32(b + 0x00);
        c.user = user < 4 ? int(user) : -1;
        c.url = GuestString(mem.Read32(b + 0x04));
        c.method = GuestString(mem.Read32(b + 0x08));
        c.flags = mem.Read32(b + 0x14);
        c.req = GuestBytes(mem.Read32(b + 0x18), mem.Read32(b + 0x1C));
        c.reqSchema = GuestString(mem.Read32(b + 0x20));
        c.respSchema = GuestString(mem.Read32(b + (begin ? 0x2C : 0x30)));
        c.respSize = begin ? 0 : mem.Read32(b + 0x2C);
        return c;
    }
}

namespace kernel
{
    WebCall WebCallPrepare(PPCContext& ctx, uint32_t message, uint32_t b)
    {
        auto& mem = guest::g_memory;
        WebCall w;
        w.message = message;
        switch (message)
        {
        case 0x5806A:
            w.call = ReadCall(b, false);
            w.status = mem.Read32(b + 0x24);
            w.resp = mem.Read32(b + 0x28);
            FreeTaskBuffer(ctx, w.call.flags, mem.Read32(b + 0x0C));
            break;
        case 0x5806B:
            w.call = ReadCall(b, true);
            w.out = mem.Read32(b + 0x34);
            w.taskBuffer = (w.call.flags & 8) ? mem.Read32(b + 0x0C) : 0;
            break;
        case 0x5806C:
            Fatal("XOnlineCallWebServiceContinueRequest (a stream request body) is not supported yet");
        case 0x5806D:
            w.handle = mem.Read32(b);
            w.status = mem.Read32(b + 4);
            w.resp = mem.Read32(b + 8);
            w.size = mem.Read32(b + 12);
            break;
        case 0x5806E:
            w.handle = mem.Read32(b);
            w.resp = mem.Read32(b + 4);
            w.size = mem.Read32(b + 8);
            break;
        case 0x5806F: {
            w.handle = mem.Read32(b);
            w.sync = true;
            uint32_t flags = 0, buffer = 0;
            {
                std::lock_guard lock(g_mutex);
                if (const auto it = g_calls.find(w.handle); it != g_calls.end())
                    flags = it->second.flags, buffer = it->second.taskBuffer;
            }
            FreeTaskBuffer(ctx, flags, buffer);
            break;
        }
        default:
            Fatal("XLiveBase web-service message {:05X}", message);
        }
        return w;
    }

    uint32_t WebCallRun(const WebCall& w)
    {
        auto& mem = guest::g_memory;
        switch (w.message)
        {
        case 0x5806A: {
            const auto r = live::WsCall(w.call);
            Log("XOnlineCallWebService({} {}): {}", w.call.method, w.call.url,
                r ? std::format("HTTP {}, {:08X}", r->status, r->hr) : std::string("no server: 80072EFD"));
            if (!r)
                return kCannotConnect;
            WriteResponse(*r, w.status, w.resp, w.call.respSize);
            return r->hr;
        }
        case 0x5806B: {
            const auto server = live::WsBegin(w.call);
            Log("XOnlineCallWebServiceBeginRequest({} {}): {}", w.call.method, w.call.url,
                server ? std::format("server handle {}", *server) : std::string("no server: 80072EFD"));
            if (!server)
            {
                if (w.out)
                    mem.Write32(w.out, 0);
                return kCannotConnect;
            }
            uint32_t handle;
            {
                std::lock_guard lock(g_mutex);
                handle = kHandleBase + g_next++;
                g_calls[handle] = { *server, w.call.respSchema, w.call.flags, w.taskBuffer };
            }
            if (w.out)
                mem.Write32(w.out, handle);
            return 0;
        }
        case 0x5806D: {
            Call call;
            {
                std::lock_guard lock(g_mutex);
                const auto it = g_calls.find(w.handle);
                if (it == g_calls.end() || it->second.responded)
                    return kAccessDenied;
                call = it->second;
            }
            const auto r = live::WsResponse(call.server, call.respSchema, w.size, call.flags);
            if (!r)
                return kCannotConnect;
            WriteResponse(*r, w.status, w.resp, w.size);
            {
                std::lock_guard lock(g_mutex);
                if (const auto it = g_calls.find(w.handle); it != g_calls.end())
                {
                    it->second.responded = true;
                    it->second.length = r->length.value_or(0);
                }
            }
            Log("XOnlineCallWebServiceBeginResponse: HTTP {}, {:08X}, {} body bytes", r->status, r->hr,
                r->length.value_or(0));
            return r->hr;
        }
        case 0x5806E: {
            uint32_t server, offset, n;
            {
                std::lock_guard lock(g_mutex);
                const auto it = g_calls.find(w.handle);
                if (it == g_calls.end() || !it->second.responded || it->second.offset >= it->second.length)
                {
                    Log("XOnlineCallWebServiceContinueResponse({:08X}): no body left: 80070005", w.handle);
                    return kAccessDenied;
                }
                server = it->second.server;
                offset = it->second.offset;
                n = std::min(w.size, it->second.length - offset);
            }
            const auto data = live::WsRead(server, offset, n);
            if (!data)
                return kCannotConnect;
            std::memcpy(mem.Translate(w.resp), data->data(), std::min<size_t>(data->size(), n));
            {
                std::lock_guard lock(g_mutex);
                if (const auto it = g_calls.find(w.handle); it != g_calls.end())
                    it->second.offset = offset + n;
            }
            Log("XOnlineCallWebServiceContinueResponse: {} bytes into {:08X} ({} of the body)", n, w.resp, offset + n);
            return 0;
        }
        case 0x5806F: {
            uint32_t server = 0;
            {
                std::lock_guard lock(g_mutex);
                const auto it = g_calls.find(w.handle);
                if (it == g_calls.end())
                    return 0;
                server = it->second.server;
                g_calls.erase(it);
            }
            live::WsClose(server);
            return 0;
        }
        }
        return kAccessDenied;
    }
}
