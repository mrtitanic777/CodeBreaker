// java/init_driver.hpp -- the ordered static-initialisation entry point of a converted program.
//
// C++ gives no order for the static initialisation of different translation units, and Java runs a class's static initialisers
// lazily, on first use. The converter therefore defers every order-sensitive static initialiser into a per-class `initStatics()`
// (emit/source.hpp) and writes ONE driver that calls them all, in dependency order, at startup:
//
//     j2x_init.h / j2x_init.cpp   void j2x_initAll()      every class's initStatics(), a class after every class whose statics it reads
//     j2x_rtti.h / j2x_rtti.cpp   j2xRegisterRtti() ...   the class-ancestry registry `instanceof` (JObject::is<T>()) reads, and the
//                                                         value-equality hooks of classes that define equals() or hashCode()
#pragma once

#include <filesystem>
#include <set>
#include <span>
#include <string>
#include <vector>

#include "java/ir.hpp"

namespace cb::java::init_driver {

using NameSet = std::set<std::string, std::less<>>;

// The order in which the classes in `callable_init` run their initStatics(), so that a class runs AFTER every class whose statics it
// reads. Two dependency sources are combined:
//   * the SEED: proven, constructor-MEDIATED dependencies of the target profile (Block's constructor reads Material's statics --
//     invisible to a text scan, because Material is named in BlockStone's constructor, not in Block's static init). The seed is a
//     HARD chain: each seeded class runs after the previous one. (FIXED BUG static-init-order-seed: CraftingManager::initStatics() is
//     only `s_instance = new CraftingManager()`, so a text scan saw no dependency, it floated to the front, ran before Item and Block
//     and dereferenced null Item statics -- a black screen in j2x_initAll.)
//   * the DERIVED dependencies: DIRECT reads -- a class whose static field initialisers, static{} blocks or enum constant arguments
//     name another initable class's statics. They catch the registries the seed does not list (Achievement, StatList, typed enums
//     reading Material ...). A class whose initStatics() CONSTRUCTS objects (`new X`) runs X's constructor chain, and those constructors
//     read other classes' statics: the scan follows `new X` transitively into X's constructor, instance initialisers and ancestors'
//     constructors (FIXED BUG static-init-order-ctor: BiomeGenBase::initStatics() does `new BiomeGenRainforest()`, whose inherited
//     constructor reads Block::s_grass, so BiomeGenBase must init AFTER Block though its static-init text names Block zero times).
// Stable and deterministic: among classes that are ready, the seed's order and then the name decide; a cycle is broken the same way.
std::vector<std::string> compute_static_init_order(std::span<const ir::JClass> classes, const NameSet& callable_init, const std::vector<std::string>& seed);

// Writes j2x_init.h, j2x_init.cpp, j2x_rtti.h and j2x_rtti.cpp into `out_dir`. `init_classes` are the classes whose initStatics() does
// real work. The class-ancestry registry is a separate translation unit that includes every header, so a bad include cannot break the
// init driver. Every class is pre-registered at boot (FIXED BUG classreg-multithread-race): the registry stays READ-ONLY under the
// multiplayer network threads, so no reader thread races the main thread on a lazy insertion into it.
void write_init_driver(const std::filesystem::path& out_dir, const NameSet& init_classes, std::span<const ir::JClass> classes,
                       const std::vector<std::string>& seed);

}  // namespace cb::java::init_driver
