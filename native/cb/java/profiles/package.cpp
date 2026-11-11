#include "cb/java/profiles/package.hpp"

#include <utility>

#include "cb/core/error.hpp"
#include "cb/core/text.hpp"

namespace cb::java::profiles {

namespace {
constexpr std::string_view kGenericName = "generic";
}

ProfileSet::ProfileSet() : active_(kGenericName) {
    Profile generic;
    generic.name = std::string(kGenericName);
    profiles_.emplace(generic.name, std::move(generic));
}

Profile& ProfileSet::add(Profile profile) {
    const std::string name = profile.name;
    return profiles_.insert_or_assign(name, std::move(profile)).first->second;
}

const Profile* ProfileSet::find(std::string_view name) {
    auto it = profiles_.find(name);
    if (it == profiles_.end() && loader_) {   // a project's profile: its project registers it on load
        loader_(*this, name);
        it = profiles_.find(name);
    }
    return it == profiles_.end() ? nullptr : &it->second;
}

const Profile& ProfileSet::get(std::string_view name) {
    if (const Profile* p = find(name)) return *p;
    throw UsageError("unknown target profile '" + std::string(name) + "' (known: " + text::join(names(), ", ") + ")");
}

std::vector<std::string> ProfileSet::names() const {
    std::vector<std::string> out;
    out.reserve(profiles_.size());
    for (const auto& entry : profiles_) out.push_back(entry.first);   // a std::map: already sorted
    return out;
}

const Profile& ProfileSet::generic() const { return profiles_.find(kGenericName)->second; }

const Profile& ProfileSet::active() const { return profiles_.find(active_)->second; }

const Profile& ProfileSet::use(std::string_view name) {
    const Profile& p = get(name);
    active_ = p.name;
    return p;
}

}  // namespace cb::java::profiles
