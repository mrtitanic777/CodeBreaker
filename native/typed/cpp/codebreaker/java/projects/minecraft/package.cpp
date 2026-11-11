// codebreaker/java/projects/minecraft/package.cpp -- typed C++ port of codebreaker/java/projects/minecraft/__init__.py
// (see package.hpp).
#include "codebreaker/java/projects/minecraft/package.hpp"

#include <mutex>

#include "codebreaker/java/_paths.hpp"
#include "codebreaker/java/profiles/package.hpp"
#include "codebreaker/java/projects/minecraft/achievements.hpp"
#include "codebreaker/java/projects/minecraft/applet_ping.hpp"
#include "codebreaker/java/projects/minecraft/content.hpp"
#include "codebreaker/java/projects/minecraft/controller_nav.hpp"
#include "codebreaker/java/projects/minecraft/edition.hpp"
#include "codebreaker/java/projects/minecraft/keybind_mark.hpp"
#include "codebreaker/java/projects/minecraft/platform_coverage.hpp"
#include "codebreaker/java/projects/minecraft/profiles/minecraft_alpha.hpp"
#include "codebreaker/java/projects/minecraft/profiles/minecraft_beta.hpp"
#include "codebreaker/java/projects/minecraft/profiles/minecraft_indev.hpp"
#include "codebreaker/java/projects/minecraft/profiles/minecraft_infdev.hpp"
#include "codebreaker/java/projects/minecraft/profiles/minecraft_release.hpp"
#include "codebreaker/java/projects/minecraft/profiles/minecraft_rubydung.hpp"
#include "codebreaker/java/projects/minecraft/profiles/minecraft_xbox360.hpp"
#include "codebreaker/java/projects/minecraft/raycast.hpp"
#include "codebreaker/java/projects/minecraft/render_fix.hpp"
#include "codebreaker/java/projects/minecraft/sound_fix.hpp"
#include "py_os.hpp"

namespace jprofiles = codebreaker::java::profiles;
namespace mcprofiles = codebreaker::java::projects::minecraft::profiles;

namespace codebreaker::java::projects::minecraft {

using py::str;

void load_profiles() {
    static std::once_flag once;
    std::call_once(once, [] {
        // from ...profiles import (minecraft_xbox360, minecraft_rubydung, minecraft_indev, minecraft_infdev,
        //                          minecraft_alpha, minecraft_beta, minecraft_release): each registers itself
        jprofiles::register_(mcprofiles::minecraft_xbox360::PROFILE);
        jprofiles::register_(mcprofiles::minecraft_rubydung::PROFILE());
        jprofiles::register_(mcprofiles::minecraft_indev::PROFILE());
        jprofiles::register_(mcprofiles::minecraft_infdev::PROFILE());
        jprofiles::register_(mcprofiles::minecraft_alpha::PROFILE());
        jprofiles::register_(mcprofiles::minecraft_beta::PROFILE());
        jprofiles::register_(mcprofiles::minecraft_release::PROFILE());
        // the Tessellator holds an MB-scale vertex buffer: a by-value copy of it is HIGH (rules.py)
        for (const str& n : jprofiles::names())
            if (n.startswith(str("minecraft_"))) jprofiles::get(n)->heavy_types_high = py::list<str>{str("Tesselator"), str("Tessellator")};
    });
}

Minecraft::Minecraft() {
    name = str("minecraft");
    char_map.set(0x2302, 127);   // U+2302 (house): the font's tile 127, its CP437 byte
    registrars = py::list<str>{str("addMapping"), str("addIdClassMapping"), str("register\\w*"), str("addClassMapping")};
    // (EntityList, Packet, TileEntity)
}

std::optional<str> Minecraft::profile_for(const std::optional<str>& edition, const std::optional<str>& mc_version,
                                          const py::list<ClassRef>& classes) {
    std::optional<str> r = edition::resolve(edition, mc_version, std::optional<py::list<ClassRef>>(classes));
    if (r && !r->empty()) return r;
    return default_profile;
}

Patched Minecraft::ir_patches(const py::list<translate::ir::NodeP>& units, const std::optional<str>& platform) {
    Patched p;
    if (!(platform && *platform == str("xbox360"))) return p;
    p.sound = sound_fix::patch(units);
    p.applet = applet_ping::patch(units);
    p.render = render_fix::patch(units);
    return p;
}

str Minecraft::finalize_text(const str& text, const std::optional<str>& platform) {
    if (!(platform && *platform == str("xbox360"))) return text;
    return raycast::convert_gl_pick_to_raycast(text);
}

void Minecraft::after_emit(const str& out_dir, const std::optional<str>& platform, const Patched& patched, const Progress& prog) {
    if (!(platform && *platform == str("xbox360"))) return;
    // SFX BYPASS: SoundManager gates every sound effect on the OGG pool, EMPTY on the console (sound
    // comes from XACT banks) -> all SFX silent. The guard is dropped and the name routed to the bridge.
    int64_t sf_sites = patched.sound ? patched.sound->sites : 0;
    int64_t sf_files = patched.sound ? patched.sound->files.size() : 0;
    py::list<str> sf_stranded = sound_fix::stranded(out_dir);
    if (sf_sites) {
        prog(py::percent(str("[sound] SFX+music bypass: %d site(s) routed to the XACT bridge across %d file(s)"), sf_sites, sf_files));
    }
    if (py::truthy(sf_stranded)) {
        prog(py::percent(str("[sound] WARNING: a soundPool*->getRandomSound null-guard survived in %s -- sound may be "
                             "silent there (guard shape changed; update sound_fix.py)"),
                         py::join(str(", "), sf_stranded)));
    }
    // CONTROLLER MENU NAVIGATION: the D-pad adapter into GuiScreen + the per-frame call (the algorithm
    // lives in Input.reference). Before overlays (an overlay wins).
    controller_nav::Result cn = controller_nav::run(out_dir);
    if (cn.guiscreen_cpp && cn.callsite) {
        prog(str("[controller] menu nav injected into GuiScreen (+ per-frame call)"));
    }
    if (py::truthy(cn.missing)) {
        prog(py::percent(str("[controller] WARNING: menu-nav anchor(s) not found -> menus may be un-navigable: %s"),
                         py::join(str("; "), cn.missing)));
    }
    // APPLET ANTI-PIRACY PING: setLevel's getDocumentBase()->openStream() faults on console.
    int64_t ap_sites = patched.applet ? patched.applet->sites : 0;
    int64_t ap_files = patched.applet ? patched.applet->files.size() : 0;
    py::list<str> ap_stranded = applet_ping::stranded(out_dir);
    if (ap_sites) {
        prog(py::percent(str("[applet] anti-piracy URL ping neutralized: %d site(s) across %d file(s)"), ap_sites, ap_files));
    }
    if (py::truthy(ap_stranded)) {
        prog(py::percent(str("[applet] WARNING: a getDocumentBase()->openStream() ping survived in %s -- may crash on "
                             "setLevel (ping shape changed; update applet_ping.py)"),
                         py::join(str(", "), ap_stranded)));
    }
    // RENDER: the per-frame chunk-rebuild budget and the hurt-camera 0/0 NaN guard.
    int64_t rf_update = patched.render ? patched.render->update : 0;
    int64_t rf_hurt = patched.render ? patched.render->hurt : 0;
    py::list<str> rf_stranded = render_fix::stranded(out_dir);
    if (rf_update) {
        prog(py::percent(str("[render] chunk-rebuild budget applied to updateRenderers (%d site(s))"), rf_update));
    }
    if (rf_hurt) {
        prog(py::percent(str("[render] hurt-camera 0/0 NaN guard applied (%d site(s))"), rf_hurt));
    }
    if (py::truthy(rf_stranded)) {
        prog(py::percent(str("[render] WARNING: a render-fix trigger survived in %s -- first in-game frame may hang "
                             "or be invisible (shape changed; update render_fix.py)"),
                         py::join(str(", "), rf_stranded)));
    }
}

void Minecraft::after_construction(const str& out_dir, const std::optional<str>& platform, const Progress& prog) {
    if (!(platform && *platform == str("xbox360"))) return;
    // KEYBIND MARKING: F1/F3/ESC/hotbar... are keys the console does not have; tag each site.
    keybind_mark::Result kb = keybind_mark::run(out_dir);
    prog(py::percent(str("[keybind] tagged %d keyboard key-press site(s) -> KEYBIND_INDEX.md (route to controller/menu)"),
                     kb.sites.size()));
}

void Minecraft::reports(const str& java_dir, const str& out_dir, const py::list<str>& runtime_files, const Progress& prog) {
    // asset manifest: what textures/sounds/music/lang files must ship in the content folder
    str cm = content::write_content_md(java_dir, out_dir);
    prog(py::percent(str("[content] asset manifest written to %s"), cm));
    // achievements -> the console achievement system (title config + networked profile write)
    auto found = achievements::scan_achievements(out_dir);
    if (py::truthy(found)) {
        achievements::run(out_dir, out_dir);
        prog(py::percent(str("[achievements] %d found -> ACHIEVEMENTS.md + achievements.xlast + Achievements.h/.cpp"), found.size()));
    }
    // platform-layer coverage against the Alpha reference layer
    str ref_dir = _paths::data_path(py::list<str>{str("platform"), str("reference")});
    std::optional<str> ref = py::os::path::isdir(ref_dir) ? std::optional<str>(ref_dir) : std::nullopt;
    auto pc = platform_coverage::write_report(out_dir, ref, py::set<str>::from(runtime_files));
    prog(py::percent(str("[platform] coverage report written to %s"), std::get<0>(pc)));
}

std::shared_ptr<Project> PROJECT() {
    load_profiles();   // (importing the package registers the profiles)
    static const std::shared_ptr<Project> p = std::make_shared<Minecraft>();
    return p;
}

}  // namespace codebreaker::java::projects::minecraft
