// source/match_bytes.cpp -- see match_bytes.hpp.
#include "source/match_bytes.hpp"

#include <nlohmann/json.hpp>

#include "core/crypto.hpp"
#include "core/json.hpp"

namespace cb::source {

std::string match_bytes_json(const std::map<std::string, MatchBytes>& functions) {
    Json all = Json::object();
    for (const auto& [name, b] : functions)
        all[name] = Json{{"built", crypto::to_hex(b.built)}, {"built_mask", crypto::to_hex(b.built_mask)}, {"original", crypto::to_hex(b.original)}, {"original_mask", crypto::to_hex(b.original_mask)}};
    return json::dump(all, json::Style::Compact);
}

}  // namespace cb::source
