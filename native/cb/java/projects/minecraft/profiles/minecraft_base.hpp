// cb/java/projects/minecraft/profiles/minecraft_base.hpp -- shared, era-invariant building blocks for the
// per-edition Minecraft target profiles.
//
// This module is DATA, not a registered profile. The six edition profiles (rubydung ... release) compose their
// own Profile from these shared pieces plus their era-specific overrides (static-init seed, nullable vocabulary,
// package roles).
//
// The canonical, proven taxonomy lives in minecraft_xbox360 (a RubyDung + Alpha + Beta *mixture* hand-tuned over
// the alpha port). Rather than duplicate ~55 filter rules and the AWT/DOM dead-type regex, they are taken from
// there and re-exposed as the era-invariant subset. `minecraft_xbox360` stays the untouched default profile.
//
// What is genuinely era-invariant (shared here):
//   * the .vcxproj/.filters classification taxonomy -- extra regex rules for classes an era doesn't have are
//     simply inert, so a generous SUPERSET is correct and robust for every edition;
//   * the AWT/swing/applet/DOM dead-type regex (no console has a desktop window in any era);
//   * the heavy/owning types that must never be per-frame heap-allocated (Tesselator/NBT...).
//
// What is era-SPECIFIC (each edition profile overrides): static_init_order (the class set and ctor-mediated deps
// differ by era), the nullable field/getter vocabulary (API identifiers differ), and package_roles (package layout
// differs: com.mojang.rubydung vs net.minecraft.* vs ...).
#pragma once

#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/ordered_map.hpp"
#include "cb/java/profiles/package.hpp"

namespace cb::java::projects::minecraft::profiles::minecraft_base {

// Era-invariant re-exports (the single source of truth is minecraft_xbox360).
std::string_view shared_dead_types();
const std::vector<std::string>& shared_heavy_types();

// The full proven filter taxonomy already spans RubyDung/pre-classic AND Beta rules, so it classifies every era's
// class names. Editions reuse it through build_filters(); an edition may PREPEND a few era-specific rules
// (first-match-wins) without restating the whole table.
const std::vector<java::profiles::FilterRule>& full_filters();

// Package roles that are not tied to one era's package layout (the platform/runtime/util buckets). Editions supply
// their own game-package roles on top of these.
const OrderedMap<std::string, std::string>& shared_package_roles();

// The .vcxproj/.filters rules for an edition: the era-specific rules FIRST (they win under first-match-wins), then
// the shared full taxonomy. Pass no era rules to use the shared taxonomy as it is.
std::vector<java::profiles::FilterRule> build_filters(std::span<const java::profiles::FilterRule> era_rules = {});

}  // namespace cb::java::projects::minecraft::profiles::minecraft_base
