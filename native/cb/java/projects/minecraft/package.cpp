// cb-unity: alone (helpers named for what they do here)
#include "cb/java/projects/minecraft/package.hpp"

#include <format>

#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/java/projects/minecraft/achievements.hpp"
#include "cb/java/projects/minecraft/applet_ping.hpp"
#include "cb/java/projects/minecraft/content.hpp"
#include "cb/java/projects/minecraft/controller_nav.hpp"
#include "cb/java/projects/minecraft/edition.hpp"
#include "cb/java/projects/minecraft/keybind_mark.hpp"
#include "cb/java/projects/minecraft/platform_coverage.hpp"
#include "cb/java/projects/minecraft/profiles/minecraft_alpha.hpp"
#include "cb/java/projects/minecraft/profiles/minecraft_beta.hpp"
#include "cb/java/projects/minecraft/profiles/minecraft_indev.hpp"
#include "cb/java/projects/minecraft/profiles/minecraft_infdev.hpp"
#include "cb/java/projects/minecraft/profiles/minecraft_release.hpp"
#include "cb/java/projects/minecraft/profiles/minecraft_rubydung.hpp"
#include "cb/java/projects/minecraft/profiles/minecraft_xbox360.hpp"
#include "cb/java/projects/minecraft/raycast.hpp"
#include "cb/java/projects/minecraft/render_fix.hpp"
#include "cb/java/projects/minecraft/sound_fix.hpp"

namespace cb::java::projects::minecraft {

namespace {

constexpr std::string_view kConsoleTarget = "xbox360";

bool mc_on_console(std::optional<std::string_view> platform) { return platform && *platform == kConsoleTarget; }

}  // namespace

void register_profiles(java::profiles::ProfileSet& set) {
    namespace era = profiles;
    java::profiles::Profile all[] = {era::minecraft_xbox360::profile(), era::minecraft_rubydung::profile(),
                                     era::minecraft_indev::profile(),   era::minecraft_infdev::profile(),
                                     era::minecraft_alpha::profile(),   era::minecraft_beta::profile(),
                                     era::minecraft_release::profile()};
    for (java::profiles::Profile& p : all) {
        p.heavy_types_high = {"Tesselator", "Tessellator"};
        set.add(std::move(p));
    }
}

Minecraft::Minecraft(std::filesystem::path reference_dir) : reference_dir_(std::move(reference_dir)) {
    name = "minecraft";
    char_map = {{0x2302, 127}};   // U+2302 (house): the font's tile 127, its CP437 byte
    // The calls whose arguments map ids or names to classes: EntityList, Packet, TileEntity.
    registrars = {"addMapping", "addIdClassMapping", R"(register\w*)", "addClassMapping"};
}

std::optional<std::string> Minecraft::profile_for(std::string_view edition_name, std::string_view mc_version,
                                                  std::span<const ir::JClass> classes) const {
    const edition::CorpusIndex corpus = edition::CorpusIndex::of(classes);
    if (std::optional<std::string> r = edition::resolve(edition_name, mc_version, &corpus)) return r;
    return std::string(kDefaultProfile);
}

Patched Minecraft::ir_patches(Arena& arena, const std::vector<translate::ir::Node*>& units, std::optional<std::string_view> platform) {
    Patched patched;
    if (!mc_on_console(platform)) return patched;
    patched.sound = sound_fix::patch(arena, units);
    patched.applet = applet_ping::patch(units);
    patched.render = render_fix::patch(arena, units);
    return patched;
}

std::string Minecraft::finalize_text(std::string text, std::optional<std::string_view> platform) const {
    if (!mc_on_console(platform)) return text;
    return raycast::convert_gl_pick_to_raycast(text);
}

void Minecraft::after_emit(const std::filesystem::path& out_dir, std::optional<std::string_view> platform, const Patched& patched,
                           const Progress& progress) {
    if (!mc_on_console(platform)) return;

    // SFX BYPASS: SoundManager gates every sound effect on the OGG pool, EMPTY on the console (sound comes from XACT
    // banks) -> all SFX silent. The guard is dropped and the name routed to the bridge.
    const std::size_t sound_sites = patched.sound ? patched.sound->sites : 0;
    const std::size_t sound_files = patched.sound ? patched.sound->files.size() : 0;
    if (sound_sites != 0) {
        progress(std::format("[sound] SFX+music bypass: {} site(s) routed to the XACT bridge across {} file(s)", sound_sites, sound_files));
    }
    if (const std::vector<std::string> stranded = sound_fix::stranded(out_dir); !stranded.empty()) {
        progress(std::format("[sound] WARNING: a soundPool*->getRandomSound null-guard survived in {} -- sound may be silent there "
                             "(guard shape changed; update sound_fix)", text::join(stranded, ", ")));
    }

    // CONTROLLER MENU NAVIGATION: the D-pad adapter into GuiScreen + the per-frame call (the algorithm lives in
    // Input.reference). Before overlays (an overlay wins).
    const controller_nav::Result nav = controller_nav::run(out_dir);
    if (nav.guiscreen_cpp && nav.callsite) progress("[controller] menu nav injected into GuiScreen (+ per-frame call)");
    if (!nav.missing.empty()) {
        progress(std::format("[controller] WARNING: menu-nav anchor(s) not found -> menus may be un-navigable: {}", text::join(nav.missing, "; ")));
    }

    // APPLET ANTI-PIRACY PING: setLevel's getDocumentBase()->openStream() faults on console.
    const std::size_t ping_sites = patched.applet ? patched.applet->sites : 0;
    const std::size_t ping_files = patched.applet ? patched.applet->files.size() : 0;
    if (ping_sites != 0) {
        progress(std::format("[applet] anti-piracy URL ping neutralized: {} site(s) across {} file(s)", ping_sites, ping_files));
    }
    if (const std::vector<std::string> stranded = applet_ping::stranded(out_dir); !stranded.empty()) {
        progress(std::format("[applet] WARNING: a getDocumentBase()->openStream() ping survived in {} -- may crash on setLevel "
                             "(ping shape changed; update applet_ping)", text::join(stranded, ", ")));
    }

    // RENDER: the per-frame chunk-rebuild budget and the hurt-camera 0/0 NaN guard.
    const std::size_t update_sites = patched.render ? patched.render->update : 0;
    const std::size_t hurt_sites = patched.render ? patched.render->hurt : 0;
    if (update_sites != 0) progress(std::format("[render] chunk-rebuild budget applied to updateRenderers ({} site(s))", update_sites));
    if (hurt_sites != 0) progress(std::format("[render] hurt-camera 0/0 NaN guard applied ({} site(s))", hurt_sites));
    if (const std::vector<std::string> stranded = render_fix::stranded(out_dir); !stranded.empty()) {
        progress(std::format("[render] WARNING: a render-fix trigger survived in {} -- first in-game frame may hang or be invisible "
                             "(shape changed; update render_fix)", text::join(stranded, ", ")));
    }
}

void Minecraft::after_construction(const std::filesystem::path& out_dir, std::optional<std::string_view> platform,
                                   const Progress& progress) {
    if (!mc_on_console(platform)) return;
    // KEYBIND MARKING: F1/F3/ESC/hotbar... are keys the console does not have; tag each site.
    const keybind_mark::Result marked = keybind_mark::run(out_dir);
    progress(std::format("[keybind] tagged {} keyboard key-press site(s) -> KEYBIND_INDEX.md (route to controller/menu)", marked.sites.size()));
}

void Minecraft::reports(const std::filesystem::path& java_dir, const std::filesystem::path& out_dir,
                        std::span<const std::string> runtime_files, const Progress& progress) {
    // asset manifest: what textures/sounds/music/lang files must ship in the content folder
    const std::filesystem::path manifest = content::write_content_md(java_dir, out_dir);
    progress(std::format("[content] asset manifest written to {}", fs::to_text(manifest)));

    // achievements -> the console achievement system (title config + networked profile write)
    const std::vector<achievements::Achievement> found = achievements::scan_achievements(out_dir);
    if (!found.empty()) {
        (void)achievements::run(out_dir, out_dir);
        progress(std::format("[achievements] {} found -> ACHIEVEMENTS.md + achievements.xlast + Achievements.h/.cpp", found.size()));
    }

    // platform-layer coverage against the Alpha reference layer
    platform_coverage::WriteOptions options;
    if (!reference_dir_.empty() && std::filesystem::is_directory(reference_dir_)) options.reference_dir = reference_dir_;
    options.runtime_files.insert(runtime_files.begin(), runtime_files.end());
    const platform_coverage::Written coverage = platform_coverage::write_report(out_dir, options);
    progress(std::format("[platform] coverage report written to {}", fs::to_text(coverage.path)));
}

}  // namespace cb::java::projects::minecraft
