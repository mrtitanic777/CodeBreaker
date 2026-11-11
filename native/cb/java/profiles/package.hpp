// cb/java/profiles/package.hpp -- target profiles: the seam that keeps the transpiler core game-agnostic.
//
// Game- and platform-specific data (class names, package roles, nullable fields/getters, the static-init seed,
// project-filter rules) lives in a *profile* rather than being hardcoded in the core logic. The core reads it
// from the Profile a conversion was given; a different Profile retargets the tool. `ProfileSet::generic()` is the
// empty, game-agnostic baseline; a project registers its own profiles when it loads (java/projects).
//
// A ProfileSet is an ordinary object the application (or a test) creates -- there is no process-wide registry,
// so two conversions with different targets can run side by side. The set also remembers which profile is the
// "active" one, for the commands that work on the current target.
//
// (This is distinct from the repo-root `profiles/` folder, which holds per-conversion TYPE override configs such
// as `alpha.json`.)
#pragma once

#include <functional>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/ordered_map.hpp"
#include "cb/java/typemap.hpp"

namespace cb::java::profiles {

// One rule of the Visual Studio .vcxproj/.filters classification: a class whose basename matches `pattern` (an
// anchored regex) goes in the filter folder `folder`. First match wins.
struct FilterRule {
    std::string pattern;
    std::string folder;
};

// A bag of target-specific data. Every field is empty by default, so the generic core behaves
// game-agnostically until a profile supplies real data.
struct Profile {
    std::string name;
    // Proven, constructor-MEDIATED static-init dependencies that seed the topological order of static
    // initialisation (e.g. Block's ctor reads Material statics, so Material comes first).
    std::vector<std::string> static_init_order;
    // Package -> one-line role, for the signatures.md codebase map. Listed in this order.
    OrderedMap<std::string, std::string> package_roles;
    // Types heavy enough that a per-frame heap allocation of them is a hot-path finding; `heavy_types_high` are
    // the ones whose by-value copy is a HIGH finding (an MB-scale buffer) rather than a medium one.
    std::vector<std::string> heavy_types;
    std::vector<std::string> heavy_types_high;
    std::vector<FilterRule> vcxproj_filters;
    // Anchored regex matching class BASENAMES that are DEAD on this target -- external desktop/GUI types
    // (java.awt / javax.swing / java.applet, org.w3c.dom) that cannot function on the console. The pipeline drops
    // any dead type that is ALSO unreferenced (reachability is the safety net). Precise by design: explicit class
    // names, never `\w*Listener`-style wildcards that would sweep in real game types. nullopt: nothing is pruned.
    std::optional<std::string> dead_types;
    // Anchored regex matching class BASENAMES of OPTIONAL platform SUBSYSTEMS -- self-contained backend bundles
    // (audio, networking, ...) that only matter if the game uses them. Pruned by the same reachability pass as
    // dead_types. nullopt: nothing optional to prune.
    std::optional<std::string> optional_types;
    // What the target says about nullability, value types and byte signedness (typemap.hpp).
    typemap::TypeProfile type;
};

class ProfileSet {
public:
    // A set that holds the generic profile, which is also the active one.
    ProfileSet();

    // The hook that registers a project's profiles when a name is asked for that the set does not know yet:
    // `loader(set, name)` should add the profile(s) of the project that owns `name`. It runs at most once per
    // get() and may call add().
    using Loader = std::function<void(ProfileSet& set, std::string_view name)>;
    void set_loader(Loader loader) { loader_ = std::move(loader); }

    // Register a profile; one of the same name is replaced (its address stays valid). Returns the stored profile.
    Profile& add(Profile profile);

    // The profile registered under `name`, asking the loader first if the name is unknown; nullptr if it is
    // still unknown.
    const Profile* find(std::string_view name);
    // The same, for a name that must exist: UsageError (naming the known profiles) otherwise.
    const Profile& get(std::string_view name);

    // The names of every registered profile, sorted.
    std::vector<std::string> names() const;

    // The empty, game-agnostic baseline (always registered under "generic").
    const Profile& generic() const;

    // The profile the commands currently target (the generic one until use() is called). It is the profile
    // registered under the active name, so replacing that profile with add() changes what active() returns.
    const Profile& active() const;
    const std::string& active_name() const { return active_; }

    // Make `name` the active profile (UsageError if it is unknown, even after the loader ran); returns it.
    const Profile& use(std::string_view name);

private:
    std::map<std::string, Profile, std::less<>> profiles_;
    std::string active_;
    Loader loader_;
};

}  // namespace cb::java::profiles
