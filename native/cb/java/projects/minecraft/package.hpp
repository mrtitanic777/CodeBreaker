// cb/java/projects/minecraft/package.hpp -- the Minecraft project: RubyDung, Indev, Infdev, Alpha, Beta and release.
//
// What only Minecraft needs, behind the project hooks (java/projects/package.hpp):
//   * its target profiles (profiles/), one per era, chosen by edition.hpp (--edition, --mc-version, or the source's own
//     marks);
//   * the console fixes to the game's own logic -- on the IR (sound_fix, applet_ping, render_fix) and on the written
//     program (controller_nav, keybind_mark, raycast);
//   * its reports: the asset manifest (content), achievements, platform-layer coverage;
//   * data: the font's house glyph, the class registrars its reflection uses.
//
//     ProfileSet profiles;
//     minecraft::register_profiles(profiles);                    // the seven era profiles
//     ProjectSet projects(java_dir / "projects");
//     projects.add("minecraft", std::make_unique<minecraft::Minecraft>(java_dir / "platform" / "reference"));
#pragma once

#include <filesystem>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/java/profiles/package.hpp"
#include "cb/java/projects/package.hpp"

namespace cb::java::projects::minecraft {

// The profile a conversion gets when the edition and the source's marks decide nothing.
inline constexpr std::string_view kDefaultProfile = "minecraft_xbox360";

// Register the era profiles (xbox360, rubydung, indev, infdev, alpha, beta, release) with `set`. Every one gets the
// Tessellator as a HIGH heavy type: it holds an MB-scale vertex buffer, so a by-value copy of it is a high finding.
void register_profiles(java::profiles::ProfileSet& set);

class Minecraft : public Project {
public:
    // `reference_dir`: the Alpha reference platform layer the coverage report compares against; empty or absent on disk
    // means the report is made without a reference.
    explicit Minecraft(std::filesystem::path reference_dir = {});

    // The era profile for the edition, the version id or the classes' marks; the default profile when none decides.
    // UsageError for an edition that names no era.
    std::optional<std::string> profile_for(std::string_view edition, std::string_view mc_version,
                                           std::span<const ir::JClass> classes) const override;
    // The sound, applet and render fixes on the Xbox 360 target; nothing for any other.
    Patched ir_patches(Arena& arena, const std::vector<translate::ir::Node*>& units, std::optional<std::string_view> platform) override;
    // Xbox 360: the GL pick becomes a ray cast.
    std::string finalize_text(std::string text, std::optional<std::string_view> platform) const override;
    void after_emit(const std::filesystem::path& out_dir, std::optional<std::string_view> platform, const Patched& patched,
                    const Progress& progress) override;
    void after_construction(const std::filesystem::path& out_dir, std::optional<std::string_view> platform,
                            const Progress& progress) override;
    void reports(const std::filesystem::path& java_dir, const std::filesystem::path& out_dir,
                 std::span<const std::string> runtime_files, const Progress& progress) override;

private:
    std::filesystem::path reference_dir_;
};

}  // namespace cb::java::projects::minecraft
