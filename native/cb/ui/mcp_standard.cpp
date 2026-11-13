// cb/ui/mcp_standard.cpp -- the services of the MCP server that the program's own modules answer: the folders, the corpus, the inspector. The
// ones that need the whole application (the ledger, the analyses, the decompiler, the version comparison, the function store's names, the
// command table) are the application's to give (cb/app wires them); a tool whose service is not given says so.
#include <nlohmann/json.hpp>

#include "cb/base/config.hpp"
#include "cb/base/corpus.hpp"
#include "cb/core/error.hpp"
#include "cb/formats/inspect.hpp"
#include "cb/ui/mcp_server.hpp"

namespace cb::ui::mcp {

Services standard_services(const config::Config& config, const base::Corpus& corpus) {
    Services s;
    s.state = config.state();
    s.converted = config.path("converted").value_or(config.checkout_root() / "Converted");
    s.root = config.checkout_root();
    s.corpus_root = corpus.root();
    s.retail = [&corpus](std::string_view module) { return corpus.retail(module); };
    s.lane = [&corpus](const std::filesystem::path& file) {
        const std::optional<base::Lane> lane = corpus.lane(file);
        return lane ? std::string(base::lane_name(*lane)) : std::string();
    };
    s.inspect = [](const std::filesystem::path& file) { return formats::inspect::to_json(formats::inspect::describe(file)); };
    s.strings = [](const std::filesystem::path& file, std::int64_t min_len) {
        formats::inspect::StringOptions options;
        options.min_length = static_cast<std::size_t>(min_len);
        return formats::inspect::to_json(formats::inspect::strings(file, options));
    };
    s.hexdump = [](const std::filesystem::path& file, std::int64_t offset, std::int64_t n) {
        return formats::inspect::hexdump(file, static_cast<std::uint64_t>(offset), static_cast<std::uint64_t>(n));
    };
    s.resource_body = [](const std::filesystem::path& file, std::int64_t offset, std::int64_t size, std::int64_t type_id, bool wide) {
        const formats::inspect::ResourceBody body =
            formats::inspect::resource_body(file, static_cast<std::uint64_t>(offset), static_cast<std::uint64_t>(size), static_cast<std::int32_t>(type_id), wide);
        ResourceBody out;
        if (body.kind == formats::inspect::ResourceBody::Kind::Image) {
            out.image = true;
            out.bytes = body.bytes;
            out.format = body.content_type.substr(body.content_type.rfind('/') + 1);
            return out;
        }
        Json doc = Json::object();
        doc["kind"] = body.kind == formats::inspect::ResourceBody::Kind::Text ? "text" : "hex";
        doc["text"] = body.text;
        out.document = std::make_shared<Json>(std::move(doc));
        return out;
    };
    return s;
}

}  // namespace cb::ui::mcp
