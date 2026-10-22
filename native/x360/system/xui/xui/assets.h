#pragma once

// The system UI's assets as XAM 17559 finds them (tools/scripts/extract_system_ui.py writes them): XUI packages named
// <module>.<resource>.xzp, the HUD skin and the system fonts.
//
// Paths in scenes are XUI resource locators: "sharedres://x.png" = xam.xex's "shrdres" package, "xam://x.png" = its
// "xam" package, "controlpack://x" = its "controlp" package; a relative path is relative to the package of the scene
// (or skin) that names it. The HUD skin is huduiskin.xex's skin.xur; XAM loads it with xam.xex's "skin" package as its
// base, which holds the images the skin names (Blade_dark.png, ...).

#include "xui/font.h"
#include "xui/image.h"
#include "xui/package.h"
#include "xui/xur.h"

#include <filesystem>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <vector>

namespace xui
{
    class Assets
    {
    public:
        // Loads every package and font in `dir`. Returns false (with a reason) if the skin or the fonts are missing.
        bool Load(const std::filesystem::path& dir, std::string& error);

        // A package from elsewhere, as scenes' paths will name it (an older dashboard's "dash6719.dashmain": the
        // launcher's era themes). Replaces one of that name.
        bool AddPackage(std::string name, std::vector<uint8_t> data, std::string& error);
        // Every package in `dir` in place of the one of that name (an older dashboard's system update: its Guide, skin
        // and XAM scenes, system_ui/<update>/), and the HUD skin again if huduiskin.skin is one of them. What was
        // loaded from the packages it replaces stays alive (elements may point at it) but is no longer found.
        // The base folder's package it replaced stays as "base:<name>" (kBasePrefix): the runtime reads 17559's string
        // tables by index (XamStrings, hud.xex's Strings), which the older tables number differently.
        bool ReplacePackages(const std::filesystem::path& dir, std::string& error);
        static constexpr const char* kBasePrefix = "base:";
        const Package* FindPackage(std::string_view name) const; // "xam.shrdres"
        // A scene of a package (cached); null (logged once) if missing or unreadable.
        const Scene* LoadScene(std::string_view package, std::string_view entry);
        // A string table of a package (cached).
        const std::vector<std::string>* Strings(std::string_view package, std::string_view entry);

        // Skin visuals by name (the XuiVisual children of the skin's canvas, nested ones included).
        const Node* Visual(std::string_view name) const;
        // A visual for a control: the theme skins' first (SetThemeSkins), then the HUD skin's; with the scene it is
        // in and the package its paths are relative to. False if none has it.
        bool FindVisual(std::string_view name, const Node*& node, const Scene*& scene, std::string& base) const;
        // Skins whose visuals come before the HUD skin's (package, scene), first first: an older dashboard's (its
        // BladeSkin.xur and skin.xur) while the launcher shows its era; packages that are not there are skipped. An empty
        // list goes back to the HUD skin alone.
        bool SetThemeSkins(const std::vector<std::pair<std::string, std::string>>& skins, std::string& error);
        // Locator schemes resolved to other packages first ("common://" -> "dash6719.dashcomm", "sharedres://" -> an
        // older XAM's): an older dashboard's own while the launcher shows its era. Empty: XAM 17559's alone.
        void SetThemeSchemes(std::vector<std::pair<std::string, std::string>> schemes) { themeSchemes_ = std::move(schemes); }
        const Scene* SkinScene() const { return skin_.get(); }
        static constexpr const char* kSkinBase = "xam.skin";

        // An image by locator, relative to `basePackage`; null (logged once) if it cannot be found or decoded.
        const Image* LoadImage(std::string_view path, std::string_view basePackage);
        // Raw bytes of an entry by locator (sounds).
        const Package::Entry* FindEntry(std::string_view path, std::string_view basePackage) const;

        Font& DefaultFont() { return *fonts_.at("Xbox TC"); }

    private:
        bool LoadSkin(std::string& error);

        std::map<std::string, Package, std::less<>> packages_;
        std::map<std::string, std::unique_ptr<Scene>, std::less<>> scenes_;
        std::map<std::string, std::vector<std::string>, std::less<>> strings_;
        std::map<std::string, std::unique_ptr<Image>, std::less<>> images_;
        std::mutex imagesMutex_; // LoadImage runs on the overlay's drawing threads
        std::map<std::string, std::unique_ptr<Font>, std::less<>> fonts_;
        std::unique_ptr<Scene> skin_ = std::make_unique<Scene>();
        std::map<std::string, const Node*, std::less<>> visuals_;
        struct ThemeVisual
        {
            const Node* node;
            const Scene* scene;
            std::string base;
        };
        std::map<std::string, ThemeVisual, std::less<>> themeVisuals_;
        std::vector<std::pair<std::string, std::string>> themeSchemes_;
        // ReplacePackages: what came from replaced packages, kept for the elements that point at it.
        std::vector<Package> retiredPackages_;
        std::vector<std::unique_ptr<Scene>> retiredScenes_;
        std::vector<std::unique_ptr<Image>> retiredImages_;
    };
}
