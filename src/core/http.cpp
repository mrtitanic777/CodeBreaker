// core/http.cpp -- HTTP(S) GET over WinHTTP (see core/http.hpp). Moves to src/core/ at integration.
// cb-unity: alone (file-local helpers of the port)
#include "core/http.hpp"

#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

#include "core/fs.hpp"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <winhttp.h>

namespace cb::http {

namespace {

std::wstring widen(std::string_view s) {
    if (s.empty()) return {};
    const int n = MultiByteToWideChar(CP_UTF8, 0, s.data(), static_cast<int>(s.size()), nullptr, 0);
    std::wstring w(static_cast<std::size_t>(n), L'\0');
    MultiByteToWideChar(CP_UTF8, 0, s.data(), static_cast<int>(s.size()), w.data(), n);
    return w;
}

// A WinHTTP handle, closed on scope exit.
class Handle {
public:
    Handle() = default;
    explicit Handle(HINTERNET h) : h_(h) {}
    Handle(const Handle&) = delete;
    Handle& operator=(const Handle&) = delete;
    ~Handle() {
        if (h_ != nullptr) WinHttpCloseHandle(h_);
    }
    HINTERNET get() const { return h_; }
    explicit operator bool() const { return h_ != nullptr; }

private:
    HINTERNET h_ = nullptr;
};

// One session for the process (WinHTTP sessions are thread-safe and hold the connection pool and the proxy settings).
HINTERNET session() {
    static const Handle s = [] {
        HINTERNET h = WinHttpOpen(L"CodeBreaker", WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY, WINHTTP_NO_PROXY_NAME,
                                  WINHTTP_NO_PROXY_BYPASS, 0);
        if (h == nullptr) h = WinHttpOpen(L"CodeBreaker", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME,
                                          WINHTTP_NO_PROXY_BYPASS, 0);
        return Handle(h);
    }();
    return s.get();
}

[[noreturn]] void fail(std::string_view url, int status, DWORD os_code, const std::string& what) {
    throw HttpError(url, status, os_code, what + (os_code != 0 ? " (WinHTTP error " + std::to_string(os_code) + ")" : ""));
}

}  // namespace

bool is_http_url(std::string_view text) {
    auto starts = [&](std::string_view p) {
        return text.size() >= p.size() &&
               std::equal(p.begin(), p.end(), text.begin(), [](char a, char b) { return a == static_cast<char>(std::tolower(static_cast<unsigned char>(b))); });
    };
    return starts("http://") || starts("https://");
}

Bytes get(std::string_view url, const Options& options) {
    if (!is_http_url(url)) throw HttpError(url, 0, 0, "not an http or https URL");
    const std::wstring wurl = widen(url);

    URL_COMPONENTS parts{};
    parts.dwStructSize = sizeof(parts);
    parts.dwSchemeLength = static_cast<DWORD>(-1);
    parts.dwHostNameLength = static_cast<DWORD>(-1);
    parts.dwUrlPathLength = static_cast<DWORD>(-1);
    parts.dwExtraInfoLength = static_cast<DWORD>(-1);
    if (!WinHttpCrackUrl(wurl.c_str(), static_cast<DWORD>(wurl.size()), 0, &parts)) fail(url, 0, GetLastError(), "malformed URL");

    const std::wstring host(parts.lpszHostName, parts.dwHostNameLength);
    std::wstring object(parts.lpszUrlPath, parts.dwUrlPathLength);
    if (parts.dwExtraInfoLength != 0) object.append(parts.lpszExtraInfo, parts.dwExtraInfoLength);
    if (object.empty()) object = L"/";
    const bool secure = parts.nScheme == INTERNET_SCHEME_HTTPS;

    if (session() == nullptr) fail(url, 0, GetLastError(), "WinHTTP could not start");
    Handle connection(WinHttpConnect(session(), host.c_str(), parts.nPort, 0));
    if (!connection) fail(url, 0, GetLastError(), "cannot connect");
    Handle request(WinHttpOpenRequest(connection.get(), L"GET", object.c_str(), nullptr, WINHTTP_NO_REFERER,
                                      WINHTTP_DEFAULT_ACCEPT_TYPES, secure ? WINHTTP_FLAG_SECURE : 0));
    if (!request) fail(url, 0, GetLastError(), "cannot open the request");

    WinHttpSetTimeouts(request.get(), options.connect_timeout_ms, options.connect_timeout_ms, options.connect_timeout_ms,
                       options.receive_timeout_ms);
    DWORD redirects = static_cast<DWORD>(options.max_redirects);
    WinHttpSetOption(request.get(), WINHTTP_OPTION_MAX_HTTP_AUTOMATIC_REDIRECTS, &redirects, sizeof(redirects));
    DWORD protocols = WINHTTP_FLAG_SECURE_PROTOCOL_TLS1_2 | 0x2000 /* WINHTTP_FLAG_SECURE_PROTOCOL_TLS1_3 */;
    WinHttpSetOption(request.get(), WINHTTP_OPTION_SECURE_PROTOCOLS, &protocols, sizeof(protocols));   // refused on an old system: its default stands

    const std::wstring agent = L"User-Agent: " + widen(options.user_agent);
    WinHttpAddRequestHeaders(request.get(), agent.c_str(), static_cast<DWORD>(-1L), WINHTTP_ADDREQ_FLAG_REPLACE | WINHTTP_ADDREQ_FLAG_ADD);

    if (!WinHttpSendRequest(request.get(), WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) fail(url, 0, GetLastError(), "request failed");
    if (!WinHttpReceiveResponse(request.get(), nullptr)) fail(url, 0, GetLastError(), "no response");

    DWORD status = 0, size = sizeof(status);
    if (!WinHttpQueryHeaders(request.get(), WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX, &status, &size,
                             WINHTTP_NO_HEADER_INDEX))
        fail(url, 0, GetLastError(), "no status");
    if (status >= 300) throw HttpError(url, static_cast<int>(status), 0, "HTTP status " + std::to_string(status));

    std::uint64_t expected = 0;
    bool known = false;
    {
        wchar_t len[32] = {};
        DWORD n = sizeof(len);
        if (WinHttpQueryHeaders(request.get(), WINHTTP_QUERY_CONTENT_LENGTH, WINHTTP_HEADER_NAME_BY_INDEX, len, &n, WINHTTP_NO_HEADER_INDEX)) {
            expected = _wcstoui64(len, nullptr, 10);
            known = true;
        }
    }

    Bytes body;
    if (known) body.reserve(static_cast<std::size_t>(std::min<std::uint64_t>(expected, 1ull << 30)));
    for (;;) {
        DWORD available = 0;
        if (!WinHttpQueryDataAvailable(request.get(), &available)) fail(url, static_cast<int>(status), GetLastError(), "the response broke off");
        if (available == 0) break;
        const std::size_t at = body.size();
        body.resize(at + available);
        DWORD read = 0;
        if (!WinHttpReadData(request.get(), body.data() + at, available, &read)) fail(url, static_cast<int>(status), GetLastError(), "the response broke off");
        body.resize(at + read);
        if (read == 0) break;
    }
    if (known && body.size() != expected) throw HttpError(url, static_cast<int>(status), 0, "the body is " + std::to_string(body.size()) + " bytes, not the " + std::to_string(expected) + " announced");
    return body;
}

void get_to_file(std::string_view url, const std::filesystem::path& file, const Options& options) {
    const Bytes body = get(url, options);
    cb::fs::write_file_atomic(file, body);
}

}  // namespace cb::http
