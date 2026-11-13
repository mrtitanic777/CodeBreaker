#include "cb/translate/javajudge_lane.hpp"

#include <utility>

#include "cb/core/error.hpp"
#include "cb/verify/verdicts.hpp"

namespace cb::translate::javajudge {

Dialect dialect_of(Backend backend) {
    return backend == Backend::Cpp17 ? Dialect{"C++17", "c++17"} : Dialect{"C++03", "c++03"};
}

std::vector<std::string> JavaTreeLane::prepare(const std::filesystem::path& src, const std::filesystem::path& work, Backend backend) {
    const Dialect dialect = dialect_of(backend);
    lane_ = std::make_unique<verify::lanes::JavaLane>(tools_, converter_for_(dialect.label),
                                                      verify::lanes::JavaLane::Options{.standard = std::string(dialect.standard)});
    prepared_ = lane_->prepare(src, work);
    built_ = lane_->build(*prepared_, src, work);
    return built_.items;
}

LaneResult JavaTreeLane::judge(const std::string& cls) {
    if (!built_.judge) throw UsageError("javajudge: judge() before prepare()");
    const verify::Judgement judged = built_.judge(cls);
    LaneResult out;
    out.reason = judged.reason;
    out.detail = judged.detail;
    if (judged.verdict == verify::Verdict::NoCompile) out.verdict = LaneVerdict::NoCompile;
    else if (judged.verdict == verify::Verdict::Divergent) out.verdict = LaneVerdict::Divergent;
    else if (verify::is_good(judged.verdict)) out.verdict = LaneVerdict::Good;
    return out;
}

std::size_t JavaTreeLane::gaps(const std::string& cls) {
    if (!prepared_) return 0;
    const auto text = prepared_->texts.find(cls);
    if (text == prepared_->texts.end()) return 0;
    std::size_t count = 0;
    constexpr std::string_view kGap = "J2X-GAP:";
    for (std::size_t at = text->second.find(kGap); at != std::string::npos; at = text->second.find(kGap, at + kGap.size())) ++count;
    return count;
}

}  // namespace cb::translate::javajudge
