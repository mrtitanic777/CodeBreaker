// codebreaker/java/projects/minecraft/package.hpp -- SHIM over cb/java/projects/minecraft/package.hpp
// (docs/CPP_DESIGN.md 6.3); deleted with its last py:: consumer.
//
// The old Minecraft class keeps the old Project's py:: hooks. What the new module decides (which era profile, the GL pick
// rewrite, the messages and the reports) is asked of cb::java::projects::minecraft::Minecraft; ir_patches stays on the OLD
// translate IR (the legacy sound_fix/applet_ping/render_fix::patch forms: COMPAT(py): remove when translate/ir is
// refactored's shims land) and its report is handed on as the counts the new hook prints.
// load_profiles() registers the old profile records with the old registry (the old registry is process-global state; the
// new ProfileSet is an object, see cb::java::projects::minecraft::register_profiles).
#pragma once

#include <memory>
#include <mutex>

#include "cb/compat/py_bridge.hpp"
#include "cb/java/projects/minecraft/package.hpp"
#include "codebreaker/java/_paths.hpp"
#include "codebreaker/java/console/_shim.hpp"
#include "codebreaker/java/profiles/package.hpp"
#include "codebreaker/java/projects/minecraft/applet_ping.hpp"
#include "codebreaker/java/projects/minecraft/edition.hpp"
#include "codebreaker/java/projects/minecraft/profiles/minecraft_alpha.hpp"
#include "codebreaker/java/projects/minecraft/profiles/minecraft_beta.hpp"
#include "codebreaker/java/projects/minecraft/profiles/minecraft_indev.hpp"
#include "codebreaker/java/projects/minecraft/profiles/minecraft_infdev.hpp"
#include "codebreaker/java/projects/minecraft/profiles/minecraft_release.hpp"
#include "codebreaker/java/projects/minecraft/profiles/minecraft_rubydung.hpp"
#include "codebreaker/java/projects/minecraft/profiles/minecraft_xbox360.hpp"
#include "codebreaker/java/projects/minecraft/render_fix.hpp"
#include "codebreaker/java/projects/minecraft/sound_fix.hpp"
#include "codebreaker/java/projects/package.hpp"

namespace codebreaker::java::projects::minecraft {

namespace shim_mc {

namespace cbm = cb::java::projects::minecraft;
namespace cbp = cb::java::projects;
namespace jprofiles = codebreaker::java::profiles;
namespace mcprofiles = codebreaker::java::projects::minecraft::profiles;

inline jprofiles::Profile to_profile(const mcprofiles::minecraft_xbox360::Profile& p) {
    jprofiles::Profile o;
    o.name = p.name;
    o.static_init_order = p.static_init_order;
    o.package_roles = p.package_roles;
    o.heavy_types = p.heavy_types;
    o.vcxproj_filters = p.vcxproj_filters;
    o.dead_types = p.dead_types;
    o.optional_types = p.optional_types;
    o.nullable = p.nullable;
    o.nullable_fields = p.nullable_fields;
    o.nullable_getters = p.nullable_getters;
    o.nullable_getter_return_type = p.nullable_getter_return_type;
    o.unsigned_byte_scalar_fields = p.unsigned_byte_scalar_fields;
    o.default_value_types = p.default_value_types;
    o.heavy_types_high = p.heavy_types_high;
    return o;
}

// The old reports as the new ones carry them to the hook (it reads the counts only).
inline cbp::Patched to_new(const Patched& p) {
    cbp::Patched n;
    if (p.sound) {
        n.sound.emplace();
        n.sound->sites = static_cast<std::size_t>(p.sound->sites);
        n.sound->files.resize(static_cast<std::size_t>(p.sound->files.size()));
    }
    if (p.applet) {
        n.applet.emplace();
        n.applet->sites = static_cast<std::size_t>(p.applet->sites);
        n.applet->files.resize(static_cast<std::size_t>(p.applet->files.size()));
    }
    if (p.render) {
        n.render.emplace();
        n.render->update = static_cast<std::size_t>(p.render->update);
        n.render->hurt = static_cast<std::size_t>(p.render->hurt);
    }
    return n;
}

inline std::optional<std::string_view> platform_of(const std::optional<py::str>& platform, std::string& hold) {
    if (!platform) return std::nullopt;
    hold = cb::compat::from_py(*platform);
    return hold;
}

}  // namespace shim_mc

// Register the era profiles with the old registry and give the minecraft_* ones the Tessellator's heavy_types_high. Idempotent.
inline void load_profiles() {
    static std::once_flag once;
    std::call_once(once, [] {
        namespace jprofiles = shim_mc::jprofiles;
        namespace mcprofiles = shim_mc::mcprofiles;
        jprofiles::register_(shim_mc::to_profile(mcprofiles::minecraft_xbox360::PROFILE));
        jprofiles::register_(shim_mc::to_profile(mcprofiles::minecraft_rubydung::PROFILE()));
        jprofiles::register_(shim_mc::to_profile(mcprofiles::minecraft_indev::PROFILE()));
        jprofiles::register_(shim_mc::to_profile(mcprofiles::minecraft_infdev::PROFILE()));
        jprofiles::register_(shim_mc::to_profile(mcprofiles::minecraft_alpha::PROFILE()));
        jprofiles::register_(shim_mc::to_profile(mcprofiles::minecraft_beta::PROFILE()));
        jprofiles::register_(shim_mc::to_profile(mcprofiles::minecraft_release::PROFILE()));
        for (const py::str& n : jprofiles::names())
            if (n.startswith(py::str("minecraft_")))
                jprofiles::get(n)->heavy_types_high = py::list<py::str>{py::str("Tesselator"), py::str("Tessellator")};
    });
}

class Minecraft : public Project {
  public:
    Minecraft() {
        name = py::str("minecraft");
        char_map.set(0x2302, 127);   // U+2302 (house): the font's tile 127, its CP437 byte
        registrars = py::list<py::str>{py::str("addMapping"), py::str("addIdClassMapping"), py::str("register\\w*"),
                                       py::str("addClassMapping")};
    }
    py::str default_profile = py::str("minecraft_xbox360");

    std::optional<py::str> profile_for(const std::optional<py::str>& edition, const std::optional<py::str>& mc_version,
                                       const py::list<ClassRef>& classes) override {
        std::optional<py::str> r = edition::resolve(edition, mc_version, std::optional<py::list<ClassRef>>(classes));
        if (r && !r->empty()) return r;
        return default_profile;
    }
    // The tree-shape passes run over the OLD IR (legacy forms).
    Patched ir_patches(const py::list<translate::ir::NodeP>& units, const std::optional<py::str>& platform) override {
        Patched p;
        if (!(platform && *platform == py::str("xbox360"))) return p;
        p.sound = sound_fix::patch(units);
        p.applet = applet_ping::patch(units);
        p.render = render_fix::patch(units);
        return p;
    }
    py::str finalize_text(const py::str& text, const std::optional<py::str>& platform) override {
        std::string hold;
        const auto plat = shim_mc::platform_of(platform, hold);
        return cb::compat::guard([&] { return cb::compat::to_py(project_.finalize_text(cb::compat::from_py(text), plat)); });
    }
    void after_emit(const py::str& out_dir, const std::optional<py::str>& platform, const Patched& patched,
                    const Progress& prog) override {
        std::string hold;
        const auto plat = shim_mc::platform_of(platform, hold);
        cb::compat::guard([&] {
            project_.after_emit(console::shim::path_of(out_dir), plat, shim_mc::to_new(patched), line_sink(prog));
            return 0;
        });
    }
    void after_construction(const py::str& out_dir, const std::optional<py::str>& platform, const Progress& prog) override {
        std::string hold;
        const auto plat = shim_mc::platform_of(platform, hold);
        cb::compat::guard([&] {
            project_.after_construction(console::shim::path_of(out_dir), plat, line_sink(prog));
            return 0;
        });
    }
    void reports(const py::str& java_dir, const py::str& out_dir, const py::list<py::str>& runtime_files,
                 const Progress& prog) override {
        cb::compat::guard([&] {
            const std::vector<std::string> files = cb::compat::from_py(runtime_files);
            // the same Alpha reference layer the old module looked for
            cbm::Minecraft project(console::shim::path_of(_paths::data_path(py::list<py::str>{py::str("platform"), py::str("reference")})));
            project.reports(console::shim::path_of(java_dir), console::shim::path_of(out_dir), files, line_sink(prog));
            return 0;
        });
    }

  private:
    static shim_mc::cbp::Progress line_sink(const Progress& prog) {
        return [&prog](std::string_view line) { prog(cb::compat::to_py(std::string(line))); };
    }
    cbm::Minecraft project_;
};

// PROJECT = Minecraft()
inline std::shared_ptr<Project> PROJECT() {
    load_profiles();   // (importing the package registers the profiles)
    static const std::shared_ptr<Project> p = std::make_shared<Minecraft>();
    return p;
}

}  // namespace codebreaker::java::projects::minecraft
