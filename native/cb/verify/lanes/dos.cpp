#include "cb/verify/lanes/dos.hpp"

#include <chrono>
#include <format>
#include <system_error>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"

namespace cb::verify::lanes {
const std::string& DosLane::name() const {
    static const std::string kName = "dos";
    return kName;
}

Profile DosLane::profile(const std::filesystem::path&) const {
    std::string tools;
    for (const auto& [tool, exe] : tools_()) {
        std::error_code ec;
        const std::uintmax_t size = std::filesystem::file_size(exe, ec);
        std::filesystem::file_time_type written;
        if (!ec)
            written = std::filesystem::last_write_time(exe, ec);
        if (ec)
            tools += std::format(" {}:missing", tool);
        else
            tools += std::format(" {}:{}:{}", tool, size,
                                 std::chrono::duration_cast<std::chrono::seconds>(written.time_since_epoch()).count());
    }
    std::string settings = std::format("dos rev {} tools{}", kRev, tools);
    return {crypto::to_hex(crypto::sha1(as_bytes(settings))).substr(0, 12), settings};
}

std::unique_ptr<Prepared> DosLane::prepare(const std::filesystem::path& module_path,
                                           const std::filesystem::path& work) const {
    auto m = std::make_unique<PreparedProgram>();
    m->written = writer_(module_path, work / "source");
    const Written& r = m->written;
    // (a program that did not get as far as source has none)
    const bool has_source = !r.source.empty() && std::filesystem::is_regular_file(r.source);
    if (has_source) {
        const Bytes text = fs::read_file(r.source);
        m->header.assign(as_text(text));
    }
    const std::string file_name = r.source.filename().string();
    for (const auto& [address, name] : r.functions) {
        m->order.push_back(name);
        std::string key = std::format("{:05x}", address);
        m->key[name] = key;
        m->all_keys.push_back(std::move(key));
        if (has_source)
            m->texts[name] = std::format("; {}, in {}", name, file_name);
    }
    // What a verdict was reached for: the lane's revision and the source as it is.
    m->sig = crypto::to_hex(crypto::sha1(as_bytes(std::format("{}", kRev) + std::string(1, '\0') + m->header)));
    return m;
}

Built DosLane::build(const Prepared& prepared, const std::filesystem::path&, const std::filesystem::path&) const {
    const Written& r = static_cast<const PreparedProgram&>(prepared).written;
    Built out;
    if (!r.linked) {
        const std::string why = why_not_built(r);
        for (const std::string& n : prepared.order)
            out.missing[n] = why;
        return out;
    }
    out.items = prepared.order;
    out.judge = [same = r.same, why = why_differs(r)](const std::string&) { return judge_function(same, why); };
    return out;
}

std::string DosLane::why_not_built(const Written& written) {
    return written.notes.empty() ? "did not build" : clip_note(written.notes.back());
}

std::string DosLane::why_differs(const Written& written) {
    if (written.same)
        return {};
    return written.notes.empty() ? "the built program differs from the original" : clip_note(written.notes.back());
}

Judgement DosLane::judge_function(bool same, const std::string& why) {
    Judgement j;
    if (same) {
        j.verdict = Verdict::Equivalent;
        j.detail = std::string(kSameProgram);
        return j;
    }
    j.verdict = Verdict::Divergent;
    j.reason = "image-differs";
    j.detail = why;
    return j;
}

}  // namespace cb::verify::lanes
