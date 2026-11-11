#include "cb/java/projects/minecraft/profiles/minecraft_base.hpp"

#include "cb/java/projects/minecraft/profiles/minecraft_xbox360.hpp"

namespace cb::java::projects::minecraft::profiles::minecraft_base {

namespace xbox = minecraft_xbox360;

std::string_view shared_dead_types() { return xbox::dead_types(); }

const std::vector<std::string>& shared_heavy_types() { return xbox::heavy_types(); }

const std::vector<java::profiles::FilterRule>& full_filters() { return xbox::vcxproj_filters(); }

const OrderedMap<std::string, std::string>& shared_package_roles() {
    static const OrderedMap<std::string, std::string> roles = [] {
        OrderedMap<std::string, std::string> m;
        m.insert_or_assign("util", xbox::package_roles().at("util"));
        return m;
    }();
    return roles;
}

std::vector<java::profiles::FilterRule> build_filters(std::span<const java::profiles::FilterRule> era_rules) {
    std::vector<java::profiles::FilterRule> rules(era_rules.begin(), era_rules.end());
    rules.insert(rules.end(), full_filters().begin(), full_filters().end());
    return rules;
}

}  // namespace cb::java::projects::minecraft::profiles::minecraft_base
