// cb/exact/plugin.hpp -- what the toolchains whose running is code (plm86, mspascal, xenix_c) have in common: the profile's own
// JSON, the sources' line ends, the compile as the cache keeps it, and its key.
//
// A plugin is a `toolchain::Toolchain` registered under the name its profile gives in "plugin" (`register_plugin`); the profile
// entries the generic toolchain does not keep (a tool's folder, a program's name, a variant's version ...) are read from the
// profile's JSON file, once, by `profile_json`.
#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "cb/base/cache_codec.hpp"
#include "cb/core/bytes.hpp"
#include "cb/core/json_fwd.hpp"
#include "cb/exact/toolchain.hpp"

namespace cb::exact::plugin {

// What a plugin's compile came to, as the cache keeps it: the object module (none when the tool failed) and what it said.
struct Built {
    std::optional<Bytes> object;
    std::string console;
};

// The JSON of the profile's file (toolchains/NAME.json). NotFoundError when there is none.
Json profile_json(const toolchain::Locations& locations, std::string_view name);

// `b` with every line end a CR LF, as the DOS and ISIS tools read their sources.
Bytes crlf(ByteView b);

// A string entry of a JSON object, else `fallback`.
std::string text_or(const Json& object, std::string_view key, std::string_view fallback);

// The cache key of a compile: the source, each header (name, bytes), the switches and the toolchain's own key.
std::string compile_key(ByteView source, const std::vector<std::pair<std::string, Bytes>>& headers, std::string_view switches,
                        std::string_view toolchain_key);

}  // namespace cb::exact::plugin

namespace cb::base::cache {
template <>
struct Codec<cb::exact::plugin::Built> {
    static constexpr std::uint32_t kSchema = 0x7102;
    static void write(io::ByteWriter& w, const cb::exact::plugin::Built& c) {
        Codec<std::optional<Bytes>>::write(w, c.object);
        Codec<std::string>::write(w, c.console);
    }
    static cb::exact::plugin::Built read(io::ByteReader& r) {
        cb::exact::plugin::Built c;
        c.object = Codec<std::optional<Bytes>>::read(r);
        c.console = Codec<std::string>::read(r);
        return c;
    }
};
}  // namespace cb::base::cache
