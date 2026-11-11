// cb/emulate/run.cpp -- see run.hpp.
#include "cb/emulate/run.hpp"

#include <format>


namespace cb::emulate {

std::string ended(const std::optional<std::string>& exit) {
    if (!exit) return "none";
    if (*exit == "ret") return "ret";
    if (std::string_view(*exit).starts_with("exception")) return "fault";
    return std::string(exit->substr(0, exit->find(':')));
}

bool Run::kept() const {
    if (!preserved) return false;
    for (const auto& [name, reg] : *preserved) {
        if (!reg.kept()) return false;
    }
    return true;
}

bool Run::kept(std::span<const std::string> names) const {
    if (!preserved) return false;
    for (const std::string& name : names) {
        const SavedRegister* reg = preserved->find(name);
        if (reg != nullptr && !reg->kept()) return false;
    }
    return true;
}

std::string Run::summary() const {
    return std::format("<Run {} steps={} calls={} writes={}{}>", exit ? *exit : std::string("none"),
                       steps ? std::to_string(*steps) : std::string("none"), calls.size(), writes.size(),
                       truncated ? " truncated" : "");
}

}  // namespace cb::emulate
