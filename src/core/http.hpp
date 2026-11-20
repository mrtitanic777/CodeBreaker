// core/http.hpp -- HTTP(S) GET over WinHTTP: a body to bytes, or a body to a file.
//
//     Bytes json = http::get("https://example.org/versions.json");
//     http::get_to_file("https://example.org/client.jar", "jars/minecraft.jar");   // written whole, or not at all
//
// Redirects are followed (up to Options::max_redirects; WinHTTP does not follow https to http). The User-Agent is
// "CodeBreaker" and nothing about the user or the machine is sent. A status of 400 or more, a connection that
// fails, or a body shorter than its Content-Length is an http::HttpError. The response is not decompressed
// (a jar served with a Content-Encoding comes back as sent). Safe to call from many threads at once.
// Windows only (the implementation is src/core/http.cpp, on winhttp.lib).
#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>

#include "core/bytes.hpp"
#include "core/error.hpp"

namespace cb::http {

struct Options {
    std::string user_agent = "CodeBreaker";
    int connect_timeout_ms = 30000;   // name resolution, connect and send, each
    int receive_timeout_ms = 60000;   // waiting for the next part of the response
    int max_redirects = 10;
};

// A request that did not give a body: status is the HTTP status (0 when no response came at all: the connection, the
// name or TLS failed; os_code is then WinHTTP's error).
class HttpError : public Error {
public:
    HttpError(std::string_view url, int status, unsigned long os_code, const std::string& what)
        : Error(std::string(url) + ": " + what), status_(status), os_code_(os_code) {}
    int status() const { return status_; }
    unsigned long os_code() const { return os_code_; }
    CB_ERROR_BOILERPLATE(HttpError)

private:
    int status_;
    unsigned long os_code_;
};

// Whether the text is an http:// or https:// URL (what java.net.URL accepts of these two; anything else is a path).
bool is_http_url(std::string_view text);

// The body of the response to GET url.
Bytes get(std::string_view url, const Options& options = {});

// GET url into the file (parent directories created; the file is replaced atomically, so a failed download leaves
// nothing behind and does not clobber an earlier file).
void get_to_file(std::string_view url, const std::filesystem::path& file, const Options& options = {});

}  // namespace cb::http
