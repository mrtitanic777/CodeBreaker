// Tests of match_bytes.hpp: the side file's form.
#include "source/match_bytes.hpp"

#include <nlohmann/json.hpp>

#include "core/json.hpp"
#include "core/testing.hpp"

namespace cb::source {
namespace {

CB_TEST("source.match_bytes.each_side_and_its_mask_are_kept_as_hex", [] {
    std::map<std::string, MatchBytes> functions;
    functions["f"] = MatchBytes{Bytes{0x8B, 0xC3}, Bytes{0, 1}, Bytes{0x8B, 0xC1, 0x90}, Bytes{0, 0, 1}};
    const Json read = json::parse(match_bytes_json(functions));
    CB_CHECK_EQ(read.at("f").at("built").get<std::string>(), "8bc3");
    CB_CHECK_EQ(read.at("f").at("built_mask").get<std::string>(), "0001");
    CB_CHECK_EQ(read.at("f").at("original").get<std::string>(), "8bc190");
    CB_CHECK_EQ(read.at("f").at("original_mask").get<std::string>(), "000001");
    CB_CHECK_EQ(match_bytes_json({}), "{}");
});

}  // namespace
}  // namespace cb::source
