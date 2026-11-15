#include "cb/engine/machine_lanes.hpp"

#include <algorithm>
#include <array>
#include <format>

#include "cb/core/error.hpp"

namespace cb::engine {

namespace {

constexpr std::array<std::string_view, 7> kBuiltIn = {"nes", "sms", "gb", "genesis", "ps1", "zx", "msx"};

bool well_formed(std::string_view name) {
    return !name.empty() && std::all_of(name.begin(), name.end(), [](char c) { return (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'); });
}

}  // namespace

bool built_in_machine(std::string_view name) { return std::find(kBuiltIn.begin(), kBuiltIn.end(), name) != kBuiltIn.end(); }

void MachineLaneRegistry::add(MachineLane lane) {
    if (!well_formed(lane.name)) throw UsageError(std::format("machine lane name '{}': lower-case letters and digits only", lane.name));
    if (!lane.probe || !lane.make_spec) throw UsageError(std::format("machine lane '{}' needs its probe and its spec", lane.name));
    if (built_in_machine(lane.name)) throw UsageError(std::format("machine lane '{}' is one the lane kit builds itself", lane.name));
    std::lock_guard hold(lock_);
    for (const auto& have : lanes_)
        if (have->name == lane.name) throw UsageError(std::format("machine lane '{}' is added twice", lane.name));
    lanes_.push_back(std::make_shared<const MachineLane>(std::move(lane)));
}

bool MachineLaneRegistry::remove(std::string_view name) {
    std::lock_guard hold(lock_);
    const auto it = std::find_if(lanes_.begin(), lanes_.end(), [&](const auto& l) { return l->name == name; });
    if (it == lanes_.end()) return false;
    lanes_.erase(it);
    return true;
}

std::shared_ptr<const MachineLane> MachineLaneRegistry::find(std::string_view name) const {
    std::lock_guard hold(lock_);
    for (const auto& l : lanes_)
        if (l->name == name) return l;
    return nullptr;
}

std::vector<std::string> MachineLaneRegistry::names() const {
    std::lock_guard hold(lock_);
    std::vector<std::string> out;
    out.reserve(lanes_.size());
    for (const auto& l : lanes_) out.push_back(l->name);
    return out;
}

std::string MachineLaneRegistry::name_of_file(const std::filesystem::path& path) const {
    std::vector<std::shared_ptr<const MachineLane>> copy;
    {
        std::lock_guard hold(lock_);
        copy = lanes_;   // (a probe reads the file: it runs outside the lock)
    }
    for (const auto& l : copy) {
        try {
            if (l->probe(path)) return l->name;
        } catch (const Error&) {   // isolation boundary: a damaged file is not this machine's
        }
    }
    return {};
}

MachineLaneRegistry& machine_lanes() {
    static MachineLaneRegistry registry;
    return registry;
}

}  // namespace cb::engine
