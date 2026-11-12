// cb-unity: alone
#include "cb/exact/plm86.hpp"

#include <algorithm>
#include <format>

#include <nlohmann/json.hpp>

#include "cb/base/cache.hpp"
#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/exact/emu/isis.hpp"
#include "cb/exact/plugin.hpp"

namespace cb::exact::plm86 {

namespace stdfs = std::filesystem;

namespace {

// At most `max` bytes of UTF-8 text, never in the middle of a character.
std::string plm_prefix(std::string_view s, std::size_t max) {
    if (s.size() <= max) return std::string(s);
    std::size_t n = max;
    while (n > 0 && (static_cast<unsigned char>(s[n]) & 0xC0) == 0x80) --n;
    return std::string(s.substr(0, n));
}

}  // namespace

Plm86::Plm86(toolchain::Profile profile, std::optional<std::string> variant, const toolchain::Context& context)
    : toolchain::Toolchain(profile, variant, context), ctx_(context), raw_(std::make_unique<Json>(plugin::profile_json(*context.locations, profile.name))) {
    // the variant's version and controls: `variants` is {name: {"version", "controls"}}
    const Json& v = (*raw_)["variants"];
    const Json empty = Json::object();
    const Json& mine = v.is_object() && v.contains(this->variant()) ? v[this->variant()] : empty;
    version_ = plugin::text_or(mine, "version", "2.1");
    controls_ = plugin::text_or(mine, "controls", plugin::text_or(*raw_, "controls", ""));
    plugin_key_ = crypto::to_hex(crypto::sha1(to_bytes(name() + version_ + controls_ + std::string(kCacheVersion)))).substr(0, 12);
    const std::string ok = plugin::text_or(*raw_, "ok", "");
    if (!ok.empty()) ok_ = std::make_shared<Regex>(ok);
}

Plm86::~Plm86() = default;

namespace {

// The source as the tool reads it, the headers by upper-case name with CR LF line ends, and the controls to use.
struct PlmInput {
    Bytes source;
    std::vector<std::pair<std::string, Bytes>> headers;
    std::string controls;
};

PlmInput plm_input(ByteView source, const modules::CompileOptions& options, const std::string& default_controls) {
    PlmInput in;
    in.source = plugin::crlf(source);
    for (const auto& [name, bytes] : options.headers) in.headers.emplace_back(text::upper(name), plugin::crlf(bytes));
    std::sort(in.headers.begin(), in.headers.end());
    in.controls = options.switches && !options.switches->empty() ? *options.switches : default_controls;
    return in;
}

}  // namespace

std::string Plm86::cache_key(ByteView source, const modules::CompileOptions& options) const {
    const PlmInput in = plm_input(source, options, controls_);
    return plugin::compile_key(in.source, in.headers, in.controls, plugin_key_);
}

modules::CompileResult Plm86::build(ByteView source, const modules::CompileOptions& options) const {
    const PlmInput in = plm_input(source, options, controls_);
    return run_tool(in.source, in.headers, in.controls);
}

modules::CompileResult Plm86::compile(ByteView source, const modules::CompileOptions& options) const {
    const std::string key = cache_key(source, options);
    if (ctx_.cache) {
        if (auto got = ctx_.cache->value<plugin::Built>(toolchain::kCacheKind, key)) return {std::move(got->object), std::move(got->console)};
    }
    const modules::CompileResult built = build(source, options);
    if (ctx_.cache) ctx_.cache->keep(toolchain::kCacheKind, key, plugin::Built{built.object, built.console});
    return built;
}

modules::CompileResult Plm86::run_tool(const Bytes& source, const std::vector<std::pair<std::string, Bytes>>& headers,
                                               const std::string& controls) const {
    CB_ASSERT(ctx_.locations != nullptr, "a toolchain needs the locations of its kit");
    const auto kit_name = profile_data().kit;
    if (!kit_name) throw UsageError(name() + " names no kit to run its tool from");
    const stdfs::path kit = ctx_.locations->kit(*kit_name) / fs::from_text(plugin::text_or(*raw_, "tool", "plm86")) / fs::from_text(version_);
    const std::string source_name = plugin::text_or(*raw_, "source", "T.PLM");
    const fs::ScopedTempDir work("isis");   // gone when this returns, whatever happens
    std::error_code ec;
    for (stdfs::directory_iterator it(kit, ec), end; !ec && it != end; it.increment(ec))
        if (it->is_regular_file(ec)) stdfs::copy_file(it->path(), work.path() / text::upper(it->path().filename().string()), stdfs::copy_options::overwrite_existing);
    if (ec) throw IoError(kit, ec.value(), "the tool's folder cannot be read");
    fs::write_file(work.path() / source_name, source);
    for (const auto& [name, bytes] : headers) fs::write_file(work.path() / name, bytes);
    const std::string tail = toolchain::format_named(plugin::text_or(*raw_, "tail", " " + source_name + " {controls}"), {{"controls", controls}});
    emu::isis::Options options;
    options.limit = raw_->contains("limit") && (*raw_)["limit"].is_number_integer() ? (*raw_)["limit"].get<std::uint64_t>() : 400'000'000;
    const emu::isis::Result r = emu::isis::run(work.path() / plugin::text_or(*raw_, "program", "PLM86"), tail, {{0, work.path()}, {1, work.path()}}, options);
    const std::string stem = stdfs::path(source_name).stem().string();
    const stdfs::path listing = work.path() / (stem + ".LST");
    const std::string lst = stdfs::is_regular_file(listing) ? text::from_latin1(fs::read_file(listing)) : std::string();
    const stdfs::path object = work.path() / (stem + ".OBJ");
    bool ok = true;
    if (ok_) {
        const std::string both = r.console + lst;
        const auto m = ok_->search(both);
        ok = m && (m->group_count() <= 1 || m->group(1) == "0");
    }
    if (!stdfs::is_regular_file(object) || !ok) {
        std::string errors;
        const std::string listing_text = text::replace_all(lst, "\r", "");   // (split views it: keep it alive through the loop)
        for (const std::string_view line : text::split(listing_text, '\n')) {
            if (line.find("***") == std::string_view::npos && line.find("ERROR") == std::string_view::npos) continue;
            errors += (errors.empty() ? "" : "\n") + std::string(line);
        }
        return {std::nullopt, r.console + "\n" + plm_prefix(errors, 1500)};
    }
    return {fs::read_file(object), r.console};
}

void register_plugin(toolchain::Toolchains& toolchains) {
    toolchains.register_plugin(std::string(kPluginName), [](const toolchain::Profile& p, const std::optional<std::string>& variant, const toolchain::Context& c) {
        return std::make_shared<Plm86>(p, variant, c);
    });
}

}  // namespace cb::exact::plm86
