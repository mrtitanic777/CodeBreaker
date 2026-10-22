// XHttp (NetDll_XHttp*): XAM's WinHTTP-like HTTP client, as the dashboard's XRL downloader (CXrlConnection /
// CXrlTransactionBase, dash 0x9288BFA8 ...) and CMeasuredHttpRequest drive it. Semantics from XAM 17559's
// implementation:
// - Every call takes the caller type first (1 title, 2 system). BOOL calls set the thread's last error (0 on success)
//   and return error == 0; Open / OpenRequest(UsingMemory) return a handle or 0; SetStatusCallback returns the old one.
// - Handles: session (Open), connect (Connect), request (OpenRequest[UsingMemory]: with the caller's memory, at
//   least 0x5D0 bytes, the handle is that memory + 8).
// - Everything completes asynchronously: the status callback cb(handle, context, status, info, infoLength) runs from
//   XHttpDoWork(session) when (mask & status) != 0. Status values are WinHTTP's: 0x400000 send complete, 0x20000
//   headers available, 0x80000 read complete (info = the buffer, length = bytes; 0 / 0 at the end of the body),
//   0x200000 error (info = {DWORD api, DWORD error}: 5 send, 1 receive, 3 read).
// - QueryHeaders levels are XAM's own numbering: 0xFFFE status code (with 0x20000000, a DWORD), 9 Content-Length (a
//   DWORD into 4..7 bytes, a 64-bit value into 8 or more), 0x29 X-Err, 0x16 the raw headers (CRLF, NUL added, length
//   without it), 0xFFFF a header by name (length with the NUL). Missing: 12150; small buffer: 122 + the size needed.
//
// Network model: there is no Internet here (the ports talk only to our own Xbox LIVE server). Requests to our own
// hosts are answered in the runtime: "epix.localhost" serves the hub's online home (kernel/epix_home.cpp; LiveHive
// EpixXManUriRoot, kernel/xam_live.cpp). Any other host fails as an unreachable server does: an error callback with
// ERROR_WINHTTP_CANNOT_CONNECT (12029, the dash's 0x80072EFD).

#include "kernel/kernel.h"
#include "live/client.h"

#include <algorithm>
#include <cstring>
#include <deque>
#include <format>
#include <map>
#include <mutex>
#include <optional>
#include <string>
#include <vector>

namespace
{
    constexpr uint32_t kErrorInvalidHandle = 6;
    constexpr uint32_t kErrorInvalidParameter = 87;
    constexpr uint32_t kErrorInsufficientBuffer = 122;
    constexpr uint32_t kErrorInvalidOption = 12009;
    constexpr uint32_t kErrorIncorrectHandleType = 12018;
    constexpr uint32_t kErrorIncorrectHandleState = 12019;
    constexpr uint32_t kErrorCannotConnect = 12029;
    constexpr uint32_t kErrorHeaderNotFound = 12150;

    constexpr uint32_t kStatusSendComplete = 0x400000;
    constexpr uint32_t kStatusHeadersAvailable = 0x20000;
    constexpr uint32_t kStatusReadComplete = 0x80000;
    constexpr uint32_t kStatusError = 0x200000;
    constexpr uint32_t kApiReceiveResponse = 1, kApiReadData = 3, kApiSendRequest = 5;

    constexpr uint32_t kQueryNumber = 0x20000000;
    constexpr uint32_t kMinRequestMemory = 0x5D0;

    constexpr const char* kEpixHost = "epix.localhost";

    enum class Kind { Session, Connect, Request };

    struct Response
    {
        uint32_t status = 0;
        std::vector<std::pair<std::string, std::string>> headers;
        std::string body;
    };

    struct Handle
    {
        Kind kind;
        uint32_t caller = 0;
        uint32_t parent = 0;  // connect: its session; request: its connect
        uint32_t session = 0; // the session whose DoWork delivers this handle's callbacks
        uint32_t callback = 0, mask = 0, context = 0;
        std::string host;     // connect
        uint16_t port = 0;
        std::string verb, path; // request
        // Request state: 0 open, 1 sent, 2 send complete, 3 receiving, 4 headers, 5 failed.
        int state = 0;
        std::optional<Response> response; // nullopt: the host cannot be reached
        size_t readOffset = 0;
        std::map<uint32_t, uint32_t> options; // request DWORD options set (SetOption)
    };

    struct Event
    {
        uint32_t handle, status;
        uint32_t api = 0, error = 0;      // kStatusError
        uint32_t buffer = 0, length = 0;  // kStatusReadComplete
    };

    std::mutex g_mutex;
    std::map<uint32_t, Handle> g_handles;
    std::deque<Event> g_events;
    uint32_t g_nextHandle = 0x1000; // synthetic handles in unmapped low guest memory (never a guest address)

    void SetLastError(const PPCContext& ctx, uint32_t error)
    {
        const uint32_t thread = guest::g_memory.Read32(ctx.r13.u32 + kernel::pcr::kCurrentThread);
        guest::g_memory.Write32(thread + kernel::kthread::kLastError, error);
    }

    void ReturnBool(PPCContext& ctx, uint32_t error)
    {
        SetLastError(ctx, error);
        ctx.r3.u64 = error == 0;
    }

    std::string GuestString(uint32_t address, size_t limit = 4096)
    {
        if (address == 0)
            return {};
        const char* p = reinterpret_cast<const char*>(guest::g_memory.Translate(address));
        return std::string(p, strnlen(p, limit));
    }

    std::string Lower(std::string s)
    {
        std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return char(std::tolower(c)); });
        return s;
    }

    // Our own hosts' answers (see the top of the file).
    std::optional<Response> Answer(const Handle& connect, const Handle& request)
    {
        if (connect.host != kEpixHost)
            return std::nullopt;
        Response r;
        std::string error;
        const bool manifest = request.verb == "GET" && request.path.ends_with("/dashhome.xml");
        if (manifest)
            if (auto xml = kernel::EpixOnlineManifest(error))
            {
                r.status = 200;
                r.body = std::move(*xml);
                r.headers.push_back({ "Content-Type", "text/xml; charset=utf-8" });
            }
        if (request.verb == "GET" && request.path.starts_with("/ads/"))
            if (auto png = kernel::SpotlightPicture(request.path.substr(5)))
            {
                r.status = 200;
                r.body.assign(png->begin(), png->end());
                r.headers.push_back({ "Content-Type", Lower(request.path).ends_with(".png") ? "image/png" : "image/jpeg" });
            }
        // /live/...: a picture our Xbox LIVE makes (the hubs' online tiles, Shared server/epix_hubs.py).
        if (request.verb == "GET" && request.path.starts_with("/live/"))
            if (const auto reply = live::Web(-1, "GET", "http://live.local" + request.path.substr(5), {}, {});
                reply && reply->status == 200)
            {
                r.status = 200;
                r.body.assign(reply->body.begin(), reply->body.end());
                std::string type = "image/jpeg";
                for (const auto& [name, value] : reply->headers)
                    if (Lower(name) == "content-type")
                        type = value;
                r.headers.push_back({ "Content-Type", type });
            }
        if (r.status == 0)
        {
            Log("XHttp: {} http://{}{}: 404 {}", request.verb, connect.host, request.path, error);
            r.status = 404;
        }
        r.headers.insert(r.headers.begin(), { "Content-Length", std::to_string(r.body.size()) });
        return r;
    }

    Handle* Find(uint32_t handle, uint32_t caller, Kind kind, uint32_t& error)
    {
        const auto it = g_handles.find(handle);
        if (handle == 0 || handle == 0xFFFFFFFF || it == g_handles.end())
        {
            error = kErrorInvalidHandle;
            return nullptr;
        }
        if (it->second.kind != kind || it->second.caller != caller)
        {
            error = kErrorIncorrectHandleType;
            return nullptr;
        }
        return &it->second;
    }

    std::string RawHeaders(const Response& r)
    {
        std::string s = std::format("HTTP/1.1 {} {}\r\n", r.status, r.status == 200 ? "OK" : "Not Found");
        for (const auto& [name, value] : r.headers)
            s += name + ": " + value + "\r\n";
        return s + "\r\n";
    }

    const std::string* Header(const Response& r, const std::string& name)
    {
        for (const auto& [n, value] : r.headers)
            if (Lower(n) == Lower(name))
                return &value;
        return nullptr;
    }
}

// HINTERNET NetDll_XHttpOpen(caller, LPCSTR agent, DWORD accessType, LPCSTR proxy, LPCSTR bypass, DWORD flags)
GUEST_IMPORT(NetDll_XHttpOpen)
{
    const uint32_t caller = ctx.r3.u32, access = ctx.r5.u32;
    if ((caller != 1 && caller != 2) || (access != 0 && access != 1 && access != 3) || ctx.r7.u32 != 0)
    {
        SetLastError(ctx, kErrorInvalidParameter);
        ctx.r3.u64 = 0;
        return;
    }
    if (access == 1)
        Fatal("XHttpOpen with a named proxy ({})", GuestString(ctx.r6.u32));
    std::lock_guard lock(g_mutex);
    const uint32_t handle = g_nextHandle += 4;
    Handle h{ Kind::Session };
    h.caller = caller;
    h.session = handle;
    g_handles[handle] = h;
    Log("XHttp: Open \"{}\" flags {:08X} -> {:X}", GuestString(ctx.r4.u32), ctx.r8.u32, handle);
    SetLastError(ctx, 0);
    ctx.r3.u64 = handle;
}

// HINTERNET NetDll_XHttpConnect(caller, HINTERNET session, LPCSTR host, WORD port, DWORD flags): port 0 = 80, or 443
// with 0x800000 (secure).
GUEST_IMPORT(NetDll_XHttpConnect)
{
    const uint32_t caller = ctx.r3.u32, session = ctx.r4.u32, flags = ctx.r7.u32;
    const std::string host = Lower(GuestString(ctx.r5.u32, 300));
    std::lock_guard lock(g_mutex);
    uint32_t error = 0;
    if (!Find(session, caller, Kind::Session, error))
    {
        SetLastError(ctx, error);
        ctx.r3.u64 = 0;
        return;
    }
    if (host.empty() || host.size() > 255 || (flags & ~0x800003u) != 0)
    {
        SetLastError(ctx, kErrorInvalidParameter);
        ctx.r3.u64 = 0;
        return;
    }
    const uint32_t handle = g_nextHandle += 4;
    Handle h{ Kind::Connect };
    h.caller = caller;
    h.parent = session;
    h.session = session;
    h.host = host;
    h.port = uint16_t(ctx.r6.u32 ? ctx.r6.u32 : (flags & 0x800000) ? 443 : 80);
    g_handles[handle] = h;
    Log("XHttp: Connect {}:{} (flags {:X}) -> {:X}", host, h.port, flags, handle);
    SetLastError(ctx, 0);
    ctx.r3.u64 = handle;
}

namespace
{
    // XHttpOpenRequest(UsingMemory)(caller, connect, verb, path, version, referrer, acceptTypes, memory, memorySize,
    // flags) (XAM 0x8176A398).
    void OpenRequest(PPCContext& ctx, uint32_t memory, uint32_t memorySize)
    {
        const uint32_t caller = ctx.r3.u32, connect = ctx.r4.u32;
        if ((memory != 0 && memorySize < kMinRequestMemory) || (memory == 0 && memorySize != 0) || ctx.r6.u32 == 0)
        {
            SetLastError(ctx, kErrorInvalidParameter);
            ctx.r3.u64 = 0;
            return;
        }
        const std::string path = GuestString(ctx.r6.u32);
        if (path.empty() || path[0] != '/')
        {
            SetLastError(ctx, 12006);
            ctx.r3.u64 = 0;
            return;
        }
        std::lock_guard lock(g_mutex);
        uint32_t error = 0;
        Handle* c = Find(connect, caller, Kind::Connect, error);
        if (!c)
        {
            SetLastError(ctx, error);
            ctx.r3.u64 = 0;
            return;
        }
        for (const auto& [handle, h] : g_handles)
            if (h.kind == Kind::Request && h.parent == connect)
            {
                SetLastError(ctx, 183); // the connection already has a request
                ctx.r3.u64 = 0;
                return;
            }
        const uint32_t handle = memory ? memory + 8 : (g_nextHandle += 4);
        Handle h{ Kind::Request };
        h.caller = caller;
        h.parent = connect;
        h.session = c->session;
        h.verb = ctx.r5.u32 ? GuestString(ctx.r5.u32) : "GET";
        h.path = path;
        g_handles[handle] = h;
        Log("XHttp: OpenRequest {} http://{}{} -> {:X}", h.verb, c->host, path, handle);
        SetLastError(ctx, 0);
        ctx.r3.u64 = handle;
    }
}

GUEST_IMPORT(NetDll_XHttpOpenRequest)
{
    OpenRequest(ctx, 0, 0);
}

GUEST_IMPORT(NetDll_XHttpOpenRequestUsingMemory)
{
    OpenRequest(ctx, ctx.r10.u32, kernel::StackArg32(ctx, 8));
}

// XHTTP_STATUS_CALLBACK NetDll_XHttpSetStatusCallback(caller, HINTERNET, callback, DWORD notificationFlags, reserved)
GUEST_IMPORT(NetDll_XHttpSetStatusCallback)
{
    std::lock_guard lock(g_mutex);
    const auto it = g_handles.find(ctx.r4.u32);
    if (it == g_handles.end() || it->second.caller != ctx.r3.u32)
    {
        SetLastError(ctx, kErrorInvalidHandle);
        ctx.r3.u64 = 0xFFFFFFFF;
        return;
    }
    const uint32_t old = it->second.callback;
    it->second.callback = ctx.r5.u32;
    it->second.mask = ctx.r6.u32;
    SetLastError(ctx, 0);
    ctx.r3.u64 = old;
}

// BOOL NetDll_XHttpSetOption(caller, HINTERNET, DWORD option, LPVOID buffer, DWORD length): request options 0x3F
// (flags: 0x08, 0x10 = the caller writes Host:), 0x58 redirect policy, 0x59 maximum redirects, 0x5A, 6 timeout (ms)
// are kept without effect here (answers are local and immediate).
GUEST_IMPORT(NetDll_XHttpSetOption)
{
    const uint32_t option = ctx.r5.u32, buffer = ctx.r6.u32, length = ctx.r7.u32;
    std::lock_guard lock(g_mutex);
    uint32_t error = 0;
    if (!Find(ctx.r4.u32, ctx.r3.u32, Kind::Request, error))
        return ReturnBool(ctx, error);
    if (option != 0x3F && option != 0x58 && option != 0x59 && option != 0x5A && option != 6)
        Fatal("XHttpSetOption: option {:X} (length {})", option, length);
    if (length != 4 || buffer == 0)
        return ReturnBool(ctx, kErrorInvalidParameter);
    const uint32_t value = guest::g_memory.Read32(buffer);
    if (option == 0x3F && (value & ~0x18u) != 0)
        return ReturnBool(ctx, kErrorInvalidParameter);
    g_handles.at(ctx.r4.u32).options[option] = value;
    ReturnBool(ctx, 0);
}

// BOOL NetDll_XHttpQueryOption(caller, HINTERNET, DWORD option, LPVOID buffer, LPDWORD length) (XAM 0x8176A880):
// request DWORD options: 0x17 whether XAM answered from its cache (request flag 0x1000; never here), 0x16 a
// connection state bit (0 here), and the ones SetOption keeps (0x59, 6, 0x5A; 0x3F).
GUEST_IMPORT(NetDll_XHttpQueryOption)
{
    auto& mem = guest::g_memory;
    const uint32_t option = ctx.r5.u32, buffer = ctx.r6.u32, lengthAddress = ctx.r7.u32;
    if (lengthAddress == 0 || (mem.Read32(lengthAddress) != 0 && buffer == 0))
        return ReturnBool(ctx, kErrorInvalidParameter);
    std::lock_guard lock(g_mutex);
    uint32_t error = 0;
    Handle* r = Find(ctx.r4.u32, ctx.r3.u32, Kind::Request, error);
    if (!r)
        return ReturnBool(ctx, error);
    uint32_t value = 0;
    if (option == 0x59 || option == 6 || option == 0x5A || option == 0x3F)
        value = r->options.contains(option) ? r->options[option] : 0;
    else if (option != 0x17 && option != 0x16)
        Fatal("XHttpQueryOption: option {:X}", option);
    if (mem.Read32(lengthAddress) < 4)
    {
        mem.Write32(lengthAddress, 4);
        return ReturnBool(ctx, kErrorInsufficientBuffer);
    }
    mem.Write32(buffer, value);
    mem.Write32(lengthAddress, 4);
    ReturnBool(ctx, 0);
}

// BOOL NetDll_XHttpSendRequest(caller, request, LPCSTR headers, DWORD headersLength, LPVOID optional,
// DWORD optionalLength, DWORD totalLength, DWORD_PTR context)
GUEST_IMPORT(NetDll_XHttpSendRequest)
{
    const uint32_t request = ctx.r4.u32, optional = ctx.r7.u32, optionalLength = ctx.r8.u32,
                   total = ctx.r9.u32, context = ctx.r10.u32;
    std::lock_guard lock(g_mutex);
    uint32_t error = 0;
    Handle* r = Find(request, ctx.r3.u32, Kind::Request, error);
    if (!r)
        return ReturnBool(ctx, error);
    if (total == 0 && (optional != 0 || optionalLength != 0))
        return ReturnBool(ctx, kErrorInvalidParameter);
    if (r->state != 0)
        return ReturnBool(ctx, kErrorIncorrectHandleState);
    if (total != 0 && total != optionalLength)
        Fatal("XHttpSendRequest: a body sent through XHttpWriteData ({} of {} bytes given)", optionalLength, total);
    r->context = context;
    const Handle& c = g_handles.at(r->parent);
    r->response = Answer(c, *r);
    Log("XHttp: SendRequest {} http://{}{}: {}", r->verb, c.host, r->path,
        r->response ? std::format("{}", r->response->status) : std::string("cannot connect"));
    if (r->response)
    {
        r->state = 1;
        g_events.push_back({ request, kStatusSendComplete });
    }
    else
    {
        r->state = 5;
        g_events.push_back({ request, kStatusError, kApiSendRequest, kErrorCannotConnect });
    }
    ReturnBool(ctx, 0);
}

// BOOL NetDll_XHttpReceiveResponse(caller, request, reserved): headers available (from DoWork).
GUEST_IMPORT(NetDll_XHttpReceiveResponse)
{
    std::lock_guard lock(g_mutex);
    uint32_t error = 0;
    Handle* r = Find(ctx.r4.u32, ctx.r3.u32, Kind::Request, error);
    if (!r)
        return ReturnBool(ctx, error);
    if (r->state != 2)
        return ReturnBool(ctx, kErrorIncorrectHandleState);
    r->state = 3;
    g_events.push_back({ ctx.r4.u32, kStatusHeadersAvailable });
    ReturnBool(ctx, 0);
}

// BOOL NetDll_XHttpReadData(caller, request, LPVOID buffer, DWORD size, LPDWORD read (ignored)): read complete with
// the bytes copied, or 0 bytes at the end of the body (from DoWork).
GUEST_IMPORT(NetDll_XHttpReadData)
{
    const uint32_t buffer = ctx.r5.u32, size = ctx.r6.u32;
    std::lock_guard lock(g_mutex);
    uint32_t error = 0;
    Handle* r = Find(ctx.r4.u32, ctx.r3.u32, Kind::Request, error);
    if (!r)
        return ReturnBool(ctx, error);
    if (r->state != 4 || !r->response)
        return ReturnBool(ctx, kErrorIncorrectHandleState);
    const std::string& body = r->response->body;
    const uint32_t n = uint32_t(std::min<size_t>(size, body.size() - r->readOffset));
    if (n)
        std::memcpy(guest::g_memory.Translate(buffer), body.data() + r->readOffset, n);
    r->readOffset += n;
    g_events.push_back({ ctx.r4.u32, kStatusReadComplete, 0, 0, n ? buffer : 0, n });
    ReturnBool(ctx, 0);
}

// BOOL NetDll_XHttpQueryHeaders(caller, request, DWORD level, LPCSTR name, LPVOID buffer, LPDWORD length,
// LPDWORD index)
GUEST_IMPORT(NetDll_XHttpQueryHeaders)
{
    auto& mem = guest::g_memory;
    const uint32_t level = ctx.r5.u32, nameAddress = ctx.r6.u32, buffer = ctx.r7.u32, lengthAddress = ctx.r8.u32;
    std::lock_guard lock(g_mutex);
    uint32_t error = 0;
    Handle* r = Find(ctx.r4.u32, ctx.r3.u32, Kind::Request, error);
    if (!r)
        return ReturnBool(ctx, error);
    const uint32_t info = level & 0xFFFF, flags = level & 0xFFFF0000;
    if (lengthAddress == 0 || (info == 0xFFFF) != (nameAddress != 0))
        return ReturnBool(ctx, kErrorInvalidParameter);
    if (r->state != 4 || !r->response)
        return ReturnBool(ctx, kErrorIncorrectHandleState);
    const Response& resp = *r->response;
    const uint32_t available = mem.Read32(lengthAddress);
    auto number = [&](uint64_t value, bool wide) {
        const uint32_t need = wide && available >= 8 ? 8 : 4;
        if (available < 4 || buffer == 0)
        {
            mem.Write32(lengthAddress, 4);
            return ReturnBool(ctx, kErrorInsufficientBuffer);
        }
        if (need == 8)
            mem.Write64(buffer, value);
        else
            mem.Write32(buffer, uint32_t(value));
        mem.Write32(lengthAddress, need);
        ReturnBool(ctx, 0);
    };
    auto text = [&](const std::string& value, bool countNul) {
        if (buffer == 0 || available < value.size() + 1)
        {
            mem.Write32(lengthAddress, uint32_t(value.size() + 1));
            return ReturnBool(ctx, kErrorInsufficientBuffer);
        }
        std::memcpy(mem.Translate(buffer), value.data(), value.size());
        mem.Write8(buffer + uint32_t(value.size()), 0);
        mem.Write32(lengthAddress, uint32_t(value.size() + (countNul ? 1 : 0)));
        ReturnBool(ctx, 0);
    };
    if (info == 0xFFFE && flags == kQueryNumber)
        return number(resp.status, false);
    if (info == 0x16 && flags == 0)
        return text(RawHeaders(resp), false);
    const char* known = info == 9 ? "Content-Length" : info == 0x29 ? "X-Err" : nullptr;
    if (info == 0xFFFF && (flags == 0 || flags == kQueryNumber))
    {
        const std::string name = GuestString(nameAddress, 256);
        const std::string* value = Header(resp, name);
        if (!value)
            return ReturnBool(ctx, kErrorHeaderNotFound);
        return flags ? number(std::stoull(*value), false) : text(*value, true);
    }
    if (known && (flags == 0 || flags == kQueryNumber))
    {
        const std::string* value = Header(resp, known);
        if (!value)
            return ReturnBool(ctx, kErrorHeaderNotFound);
        return flags ? number(std::stoull(*value), info == 9) : text(*value, true);
    }
    Fatal("XHttpQueryHeaders: level {:08X}", level);
}

// BOOL NetDll_XHttpDoWork(caller, session, DWORD timeout): delivers the session's pending status callbacks.
GUEST_IMPORT(NetDll_XHttpDoWork)
{
    const uint32_t session = ctx.r4.u32;
    std::vector<std::pair<Event, Handle>> due;
    {
        std::lock_guard lock(g_mutex);
        uint32_t error = 0;
        if (!Find(session, ctx.r3.u32, Kind::Session, error))
            return ReturnBool(ctx, kErrorIncorrectHandleType);
        for (auto it = g_events.begin(); it != g_events.end();)
        {
            const auto h = g_handles.find(it->handle);
            if (h == g_handles.end())
            {
                it = g_events.erase(it);
                continue;
            }
            if (h->second.session != session)
            {
                ++it;
                continue;
            }
            // The request's state as the event takes effect.
            if (it->status == kStatusSendComplete)
                h->second.state = 2;
            else if (it->status == kStatusHeadersAvailable)
                h->second.state = 4;
            due.push_back({ *it, h->second });
            it = g_events.erase(it);
        }
    }
    // Callbacks run without the lock (they call XHttp again). The error record and handle go below the caller's
    // frame on its stack.
    for (const auto& [e, h] : due)
    {
        if (h.callback == 0 || (h.mask & e.status) == 0)
            continue;
        PPCContext call = ctx;
        const uint32_t frame = (ctx.r1.u32 - 0x200) & ~0xFu;
        uint32_t info = 0, length = 0;
        if (e.status == kStatusError)
        {
            info = frame + 0x100;
            guest::g_memory.Write32(info, e.api);
            guest::g_memory.Write32(info + 4, e.error);
            length = 8;
        }
        else if (e.status == kStatusReadComplete)
        {
            info = e.buffer;
            length = e.length;
        }
        call.r1.u64 = frame;
        call.r3.u64 = e.handle;
        call.r4.u64 = h.context;
        call.r5.u64 = e.status;
        call.r6.u64 = info;
        call.r7.u64 = length;
        kernel::CallGuest(call, h.callback);
    }
    ReturnBool(ctx, 0);
}

// BOOL NetDll_XHttpCloseHandle(caller, HINTERNET): its pending callbacks are dropped. (XAM may report 0x800 handle
// closing; the dash's callbacks ignore it.)
GUEST_IMPORT(NetDll_XHttpCloseHandle)
{
    std::lock_guard lock(g_mutex);
    const auto it = g_handles.find(ctx.r4.u32);
    if (ctx.r4.u32 == 0 || it == g_handles.end() || it->second.caller != ctx.r3.u32)
        return ReturnBool(ctx, kErrorInvalidHandle);
    g_handles.erase(it);
    std::erase_if(g_events, [&](const Event& e) { return e.handle == ctx.r4.u32; });
    ReturnBool(ctx, 0);
}

namespace
{
    // XHttpCrackUrl(W)(caller, url, DWORD length (0: NUL-terminated), DWORD flags, URL_COMPONENTS*): WinHTTP's
    // URL_COMPONENTS (0x3C bytes): size, scheme / length, nScheme (1 http, 2 https), host / length, WORD port,
    // user / length, password / length, path / length, extra / length. A part whose pointer is 0 and length non-zero
    // gets a pointer into the URL and its length; the query stays in the path when the extra part is not asked for.
    void CrackUrl(PPCContext& ctx, bool wide)
    {
        auto& mem = guest::g_memory;
        const uint32_t url = ctx.r4.u32, flags = ctx.r6.u32, c = ctx.r7.u32;
        uint32_t length = ctx.r5.u32;
        const uint32_t unit = wide ? 2 : 1;
        auto at = [&](uint32_t i) { return wide ? uint32_t(mem.Read16(url + i * 2)) : uint32_t(mem.Read8(url + i)); };
        if (url == 0 || c == 0 || mem.Read32(c) != 0x3C)
            return ReturnBool(ctx, kErrorInvalidParameter);
        if (flags != 0)
            Fatal("XHttpCrackUrl: flags {:08X}", flags);
        if (length == 0)
            while (at(length) != 0)
                length++;
        std::string s;
        for (uint32_t i = 0; i < length; i++)
            s += at(i) < 0x80 ? char(at(i)) : '?';
        const size_t schemeEnd = s.find("://");
        if (schemeEnd == std::string::npos)
            return ReturnBool(ctx, 12006); // ERROR_WINHTTP_UNRECOGNIZED_SCHEME
        const std::string scheme = Lower(s.substr(0, schemeEnd));
        if (scheme != "http" && scheme != "https")
            return ReturnBool(ctx, 12006);
        const size_t hostStart = schemeEnd + 3;
        size_t hostEnd = s.find_first_of(":/?", hostStart);
        if (hostEnd == std::string::npos)
            hostEnd = s.size();
        uint32_t port = scheme == "https" ? 443 : 80;
        size_t pathStart = hostEnd;
        if (hostEnd < s.size() && s[hostEnd] == ':')
        {
            pathStart = s.find_first_of("/?", hostEnd);
            if (pathStart == std::string::npos)
                pathStart = s.size();
            port = uint32_t(std::stoul(s.substr(hostEnd + 1, pathStart - hostEnd - 1)));
        }
        if (s.find('@', hostStart) < pathStart)
            Fatal("XHttpCrackUrl: a URL with a user name ({})", s);
        const size_t extraStart = std::min(s.find('?', pathStart), s.size());
        const bool wantExtra = mem.Read32(c + 0x34) != 0 || mem.Read32(c + 0x38) != 0;
        const size_t pathEnd = wantExtra ? extraStart : s.size();
        // A part: copy into the caller's buffer, or point into the URL.
        auto part = [&](uint32_t field, size_t start, size_t end) -> bool {
            const uint32_t pointer = mem.Read32(c + field), size = mem.Read32(c + field + 4);
            const uint32_t n = uint32_t(end - start);
            if (pointer == 0)
            {
                if (size != 0)
                {
                    mem.Write32(c + field, url + uint32_t(start) * unit);
                    mem.Write32(c + field + 4, n);
                }
                return true;
            }
            if (size <= n)
            {
                mem.Write32(c + field + 4, n + 1);
                return false;
            }
            for (uint32_t i = 0; i < n; i++)
                wide ? mem.Write16(pointer + i * 2, uint16_t(at(uint32_t(start) + i)))
                     : mem.Write8(pointer + i, uint8_t(at(uint32_t(start) + i)));
            wide ? mem.Write16(pointer + n * 2, 0) : mem.Write8(pointer + n, 0);
            mem.Write32(c + field + 4, n);
            return true;
        };
        bool ok = part(0x04, 0, schemeEnd);
        mem.Write32(c + 0x0C, scheme == "https" ? 2 : 1);
        ok &= part(0x10, hostStart, hostEnd);
        mem.Write16(c + 0x18, uint16_t(port));
        ok &= part(0x1C, hostStart, hostStart); // no user name
        ok &= part(0x24, hostStart, hostStart); // no password
        ok &= part(0x2C, pathStart, pathEnd);
        if (wantExtra)
            ok &= part(0x34, extraStart, s.size());
        ReturnBool(ctx, ok ? 0 : kErrorInsufficientBuffer);
    }
}

GUEST_IMPORT(NetDll_XHttpCrackUrl)
{
    CrackUrl(ctx, false);
}

GUEST_IMPORT(NetDll_XHttpCrackUrlW)
{
    CrackUrl(ctx, true);
}

// HRESULT XamVerifyXSignerSignature(PBYTE data, DWORD size, DWORD* contentOffset, DWORD flags, PBYTE hash,
// DWORD hashSize) (XAM 0x816E2A58 -> 0x816E2898): LIVE-delivered content (EPIX manifests ...) starts with an XSigner
// header of 0x130 bytes: "YTGR", version (LE), header size 0x130 (LE), content size at +0x14 (LE), the content's
// SHA-1 at +0x1C, an RSA signature (Microsoft's XSigner keys) of the first 0x30 bytes at +0x30; the content follows
// (*contentOffset = 0x130). Data without the header is 0x800B0100 (TRUST_E_NOSIGNATURE), except that XAM accepts it
// (*contentOffset = 0, S_OK) when the caller allows unsigned content with flag 2 on development hardware
// (XboxHardwareInfo 0x10). Our Xbox LIVE serves unsigned content (the hub's online home, kernel/epix_home.cpp), so
// this console takes that allowance as a development kit does; the dash passes flag 2 unless its EPIX config sets
// "signedcontentonly".
GUEST_IMPORT(XamVerifyXSignerSignature)
{
    auto& mem = guest::g_memory;
    const uint32_t data = ctx.r3.u32, size = ctx.r4.u32, offset = ctx.r5.u32, flags = ctx.r6.u32,
                   hash = ctx.r7.u32, hashSize = ctx.r8.u32;
    if (data == 0 || offset == 0 || (hash != 0 && hashSize != 0x14))
    {
        Log("XamVerifyXSignerSignature({:08X}, {}, {:08X}, flags {:X}, hash {:08X}/{}): invalid argument", data, size,
            offset, flags, hash, hashSize);
        if (offset)
            mem.Write32(offset, 0);
        ctx.r3.u64 = 0x80070057;
        return;
    }
    mem.Write32(offset, 0);
    if (size >= 0x130 && mem.Read32(data) == 0x59544752) // "YTGR"
        Fatal("XamVerifyXSignerSignature: XSigner-signed data ({} bytes, flags {:X}): not expected from our Xbox LIVE",
              size, flags);
    ctx.r3.u64 = (flags & 2) ? 0 : 0x800B0100;
    Log("XamVerifyXSignerSignature: {} unsigned bytes, flags {:X}: {}", size, flags,
        (flags & 2) ? "accepted (unsigned content allowed)" : "no signature");
}
