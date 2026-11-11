#include "cb/verify/lanes/image_lane.hpp"

#include <algorithm>
#include <format>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"

namespace cb::verify::lanes {
namespace {

std::string states_text(const std::vector<State>& states) {
    std::string out = "[";
    for (std::size_t i = 0; i < states.size(); ++i) {
        out += i ? ", {" : "{";
        bool first = true;
        for (const auto& [name, value] : states[i]) {
            out += std::format("{}{}={}", first ? "" : ", ", name, value);
            first = false;
        }
        out += "}";
    }
    return out + "]";
}

std::string sha1_prefix(std::string_view text, std::size_t digits) {
    return crypto::to_hex(crypto::sha1(as_bytes(text))).substr(0, digits);
}

}  // namespace

ImageLane::ImageLane(ImageLaneSpec spec) : spec_(std::move(spec)) {
    CB_ASSERT(spec_.effects && spec_.start && spec_.writer, "an image lane needs effects, start and a writer");
}

Profile ImageLane::profile(const std::filesystem::path&) const {
    // (the states are part of what a verdict depends on)
    std::string settings = std::format("{} rev {} states {}", spec_.kind, spec_.rev, states_text(spec_.states));
    return {sha1_prefix(settings, 12), settings};
}

std::unique_ptr<Prepared> ImageLane::prepare(const std::filesystem::path& module_path,
                                             const std::filesystem::path& work) const {
    auto m = std::make_unique<PreparedImage>();
    m->written = spec_.writer(module_path, work / "source");
    const Written& r = m->written;
    const Bytes text = fs::read_file(r.source);   // (a writer that wrote no source is a fault: IoError)
    m->header.assign(as_text(text));
    const std::string file_name = r.source.filename().string();
    for (const auto& [address, name] : r.functions) {
        m->order.push_back(name);
        std::string key = std::format("{:0{}x}", address, spec_.key_digits);
        m->key[name] = key;
        m->all_keys.push_back(std::move(key));
        m->texts[name] = std::format("; {}, in {}", name, file_name);
        m->by_name[name] = address;
    }
    std::sort(m->all_keys.begin(), m->all_keys.end());
    // What a verdict was reached for: the lane's revision and the source as it is.
    m->sig = crypto::to_hex(crypto::sha1(as_bytes(std::format("{}", spec_.rev) + std::string(1, '\0') + m->header)));
    return m;
}

Built ImageLane::build(const Prepared& prepared, const std::filesystem::path& module_path,
                       const std::filesystem::path&) const {
    const auto& m = static_cast<const PreparedImage&>(prepared);
    const Written& r = m.written;
    Built out;
    if (!r.linked) {
        const std::string why = r.notes.empty() ? "did not build" : clip_note(r.notes.back());
        for (const std::string& n : m.order)
            out.missing[n] = why;
        return out;
    }
    auto subject = subjects_of(module_path, r);
    out.items = m.order;
    // The judge owns what it needs: the whole build's context and the entry of each function.
    out.judge = [this, same = r.same, original = std::move(subject.first), rebuilt = std::move(subject.second),
                 by_name = m.by_name](const std::string& name) {
        return judge_function(same, original, rebuilt, by_name.at(name));
    };
    return out;
}

std::pair<std::filesystem::path, std::filesystem::path> ImageLane::subjects_of(const std::filesystem::path& module_path,
                                                                               const Written& written) const {
    if (spec_.subjects)
        return spec_.subjects(module_path, written);
    return {module_path, written.built};
}

Judgement ImageLane::judge_function(bool same, const std::filesystem::path& original,
                                    const std::filesystem::path& rebuilt, std::uint32_t entry) const {
    Judgement j;
    if (same) {
        j.verdict = Verdict::Equivalent;
        j.detail = spec_.same;
        return j;
    }
    for (const State& state : spec_.states) {
        const Effects a = spec_.effects(original, entry, state);
        const Effects b = spec_.effects(rebuilt, entry, state);
        if (a == b)
            continue;
        // The first place that differs, by the names the lane gave them. Runs that differ only in how many places they
        // show (a lane's fault) have no such place.
        std::string place = "effects";
        const std::size_t shared = std::min({a.size(), b.size(), spec_.what.size()});
        for (std::size_t i = 0; i < shared; ++i)
            if (a[i] != b[i]) {
                place = spec_.what[i];
                break;
            }
        j.verdict = Verdict::Divergent;
        j.reason = place + "-differs";
        j.detail = spec_.start(state);
        return j;
    }
    j.verdict = Verdict::Equivalent;
    j.detail = std::format("the same effects from {} register states", spec_.states.size());
    return j;
}

}  // namespace cb::verify::lanes
