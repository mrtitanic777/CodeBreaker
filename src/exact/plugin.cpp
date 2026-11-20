// cb-unity: alone
#include "exact/plugin.hpp"

#include <nlohmann/json.hpp>

#include "base/cache.hpp"
#include "core/fs.hpp"
#include "core/json.hpp"

namespace cb::exact::plugin {

Json profile_json(const toolchain::Locations& locations, std::string_view name) {
    const std::filesystem::path file = locations.profiles / (std::string(name) + ".json");
    if (!std::filesystem::is_regular_file(file)) throw NotFoundError(file, 0, "the toolchain's profile is not there");
    return json::load(file);
}

Bytes crlf(ByteView b) {
    Bytes out;
    out.reserve(b.size() + b.size() / 16);
    for (std::size_t i = 0; i < b.size(); ++i) {
        if (b[i] == '\r' && i + 1 < b.size() && b[i + 1] == '\n') continue;   // (a CR LF is one line end: written below)
        if (b[i] == '\n') out.push_back('\r');
        out.push_back(b[i]);
    }
    return out;
}

std::string text_or(const Json& object, std::string_view key, std::string_view fallback) {
    if (object.is_object()) {
        const auto it = object.find(std::string(key));
        if (it != object.end() && it->is_string()) return it->get<std::string>();
    }
    return std::string(fallback);
}

std::string compile_key(ByteView source, const std::vector<std::pair<std::string, Bytes>>& headers, std::string_view switches,
                        std::string_view toolchain_key) {
    std::string joined;   // each header's name and bytes, as one part
    for (const auto& [name, bytes] : headers) {
        joined += name;
        joined += '\0';
        joined += as_text(bytes);
        joined += '\0';
    }
    return base::cache::key({std::string(as_text(source)), joined, std::string(switches), std::string(toolchain_key)});
}

}  // namespace cb::exact::plugin
