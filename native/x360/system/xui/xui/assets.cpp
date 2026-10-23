#include "xui/assets.h"

#include "log.h"

#include <format>
#include <functional>

namespace xui
{
    bool Assets::Load(const std::filesystem::path& dir, std::string& error)
    {
        const std::vector<std::filesystem::path> files = ListFiles(dir);
        for (const std::filesystem::path& p : files)
        {
            if (p.extension() == ".xzp")
            {
                Package package;
                std::string why;
                if (!package.Load(p, why))
                {
                    error = std::format("{}: {}", p.filename().string(), why);
                    return false;
                }
                packages_.emplace(p.stem().string(), std::move(package));
            }
            else if (p.extension() == ".xtt")
            {
                std::vector<uint8_t> data;
                auto font = std::make_unique<Font>();
                std::string why;
                if (!ReadFile(p, data) || !font->Load(std::move(data), why))
                {
                    error = std::format("{}: {}", p.filename().string(), why);
                    return false;
                }
                const std::string name = font->Name();
                fonts_.emplace(name, std::move(font));
            }
        }
        if (files.empty())
        {
            error = std::format("cannot list {}", dir.string());
            return false;
        }
        if (!fonts_.contains("Xbox TC"))
        {
            error = "the system font xenonclatin.xtt (Xbox TC) is missing";
            return false;
        }
        return LoadSkin(error);
    }

    bool Assets::LoadSkin(std::string& error)
    {
        const Package* skin = FindPackage("huduiskin.skin");
        const Package::Entry* entry = skin ? skin->Find("skin.xur") : nullptr;
        auto scene = std::make_unique<Scene>();
        std::string why;
        if (!entry || !FindPackage(kSkinBase) || !scene->Load(entry->data, entry->size, why))
        {
            error = entry ? "skin.xur: " + why : "huduiskin.skin.xzp / xam.skin.xzp (the HUD skin) are missing";
            return false;
        }
        std::map<std::string, const Node*, std::less<>> visuals;
        std::function<void(const Node&)> index = [&](const Node& node) {
            if (node.className == "XuiVisual")
            {
                const std::string id = node.Id();
                if (!id.empty() && !visuals.contains(id))
                    visuals.emplace(id, &node);
            }
            for (const Node& child : node.children)
                index(child);
        };
        index(scene->Root());
        retiredScenes_.push_back(std::move(skin_));
        skin_ = std::move(scene);
        visuals_ = std::move(visuals);
        return true;
    }

    bool Assets::ReplacePackages(const std::filesystem::path& dir, std::string& error)
    {
        bool skin = false;
        const std::vector<std::filesystem::path> files = ListFiles(dir);
        for (const std::filesystem::path& p : files)
        {
            if (p.extension() != ".xzp")
                continue;
            Package package;
            std::string why;
            if (!package.Load(p, why))
            {
                error = std::format("{}: {}", p.string(), why);
                return false;
            }
            const std::string name = p.stem().string();
            skin |= name == "huduiskin.skin";
            if (auto it = packages_.find(name); it != packages_.end())
            {
                // The base folder's stays findable as "base:<name>": the runtime's string indices are its tables'.
                if (!packages_.contains(kBasePrefix + name))
                    packages_.emplace(kBasePrefix + name, std::move(it->second));
                else
                    retiredPackages_.push_back(std::move(it->second));
                packages_.erase(it);
            }
            packages_.emplace(name, std::move(package));
            // Its scenes and string tables are loaded again from the new one.
            const std::string prefix = name + "/";
            for (auto it = scenes_.begin(); it != scenes_.end();)
                if (it->first.starts_with(prefix))
                {
                    retiredScenes_.push_back(std::move(it->second));
                    it = scenes_.erase(it);
                }
                else
                    ++it;
            std::erase_if(strings_, [&](const auto& e) { return e.first.starts_with(prefix); });
        }
        if (files.empty())
        {
            error = std::format("cannot list {}", dir.string());
            return false;
        }
        // Images: a locator may name any package (sharedres://), so all are decoded again.
        {
            std::lock_guard lock(imagesMutex_);
            for (auto& [key, image] : images_)
                retiredImages_.push_back(std::move(image));
            images_.clear();
        }
        return !skin || LoadSkin(error);
    }

    bool Assets::AddPackage(std::string name, std::vector<uint8_t> data, std::string& error, bool version8Control)
    {
        Package package;
        if (!package.Load(std::move(data), error))
            return false;
        package.version8Control = version8Control;
        packages_.insert_or_assign(std::move(name), std::move(package));
        return true;
    }

    const Package* Assets::FindPackage(std::string_view name) const
    {
        const auto it = packages_.find(name);
        return it == packages_.end() ? nullptr : &it->second;
    }

    const Scene* Assets::LoadScene(std::string_view package, std::string_view entry)
    {
        const std::string key = std::string(package) + "/" + std::string(entry);
        if (const auto it = scenes_.find(key); it != scenes_.end())
            return it->second.get();
        std::unique_ptr<Scene> scene;
        const Package* p = FindPackage(package);
        const Package::Entry* e = p ? p->Find(entry) : nullptr;
        std::string error;
        if (!e)
            Log("System UI: scene {} is missing", key);
        else if (scene = std::make_unique<Scene>(); !scene->Load(e->data, e->size, error, p->version8Control))
        {
            Log("System UI: scene {}: {}", key, error);
            scene.reset();
        }
        return scenes_.emplace(key, std::move(scene)).first->second.get();
    }

    const std::vector<std::string>* Assets::Strings(std::string_view package, std::string_view entry)
    {
        const std::string key = std::string(package) + "/" + std::string(entry);
        if (const auto it = strings_.find(key); it != strings_.end())
            return &it->second;
        const Package* p = FindPackage(package);
        const Package::Entry* e = p ? p->Find(entry) : nullptr;
        std::vector<std::string> table;
        std::string error;
        if (!e || !ReadStringTable(e->data, e->size, table, error))
        {
            Log("System UI: string table {} {}", key, e ? error : std::string("is missing"));
            return nullptr;
        }
        return &strings_.emplace(key, std::move(table)).first->second;
    }

    bool Assets::FindVisual(std::string_view name, const Node*& node, const Scene*& scene, std::string& base) const
    {
        if (const auto it = themeVisuals_.find(name); it != themeVisuals_.end())
        {
            node = it->second.node;
            scene = it->second.scene;
            base = it->second.base;
            return true;
        }
        node = Visual(name);
        scene = skin_.get();
        base = kSkinBase;
        return node != nullptr;
    }

    bool Assets::SetThemeSkins(const std::vector<std::pair<std::string, std::string>>& skins, std::string& error)
    {
        themeVisuals_.clear();
        for (const auto& [package, entry] : skins)
        {
            if (!FindPackage(package))
                continue; // a skin package this version does not have
            const Scene* scene = LoadScene(package, entry);
            if (!scene)
            {
                error = std::format("theme skin {}/{} is missing or unreadable (log)", package, entry);
                themeVisuals_.clear();
                return false;
            }
            std::function<void(const Node&)> index = [&](const Node& node) {
                if (node.className == "XuiVisual" && !node.Id().empty())
                    themeVisuals_.try_emplace(node.Id(), ThemeVisual{ &node, scene, package });
                for (const Node& child : node.children)
                    index(child);
            };
            index(scene->Root());
        }
        return true;
    }

    const Node* Assets::Visual(std::string_view name) const
    {
        const auto it = visuals_.find(name);
        return it == visuals_.end() ? nullptr : it->second;
    }

    const Package::Entry* Assets::FindEntry(std::string_view path, std::string_view basePackage) const
    {
        static const std::pair<std::string_view, std::string_view> kSchemes[] = {
            { "sharedres://", "xam.shrdres" }, { "xam://", "xam.xam" }, { "controlpack://", "xam.controlp" } };
        std::string_view package = basePackage, name = path;
        bool themed = false;
        for (const auto& [scheme, target] : themeSchemes_)
            if (path.size() > scheme.size() && path.substr(0, scheme.size()) == scheme)
            {
                package = target;
                name = path.substr(scheme.size());
                themed = true;
            }
        for (const auto& [scheme, target] : kSchemes)
            if (!themed && path.size() > scheme.size() && path.substr(0, scheme.size()) == scheme)
            {
                package = target;
                name = path.substr(scheme.size());
            }
        // xzp://<package>/<entry>: the runtime's own locator (not XUI's) for any loaded package, such as an older
        // dashboard's added with AddPackage ("xzp://dash9199.controlp/defaultbackground.jpg").
        if (path.starts_with("xzp://"))
        {
            const std::string_view rest = path.substr(6);
            const size_t slash = rest.find('/');
            if (slash == std::string_view::npos)
                return nullptr;
            package = rest.substr(0, slash);
            name = rest.substr(slash + 1);
        }
        if (name.find("://") != std::string_view::npos)
            return nullptr; // file:// and other schemes: not packaged
        // The HUD skin's relative paths: the skin's own package first (huduiskin.xex 1888 ... 9199 carries its images
        // with skin.xur: kbrd.png ...), then XAM's skin package (17559's huduiskin has skin.xur alone).
        // xam.xex's shrdres / xam packages: huduiskin.xex's of the same name first, where 8495 ... 9199 keep them
        // (17559's huduiskin has neither).
        if (package == "xam.shrdres" || package == "xam.xam")
            if (const Package* moved = FindPackage("huduiskin." + std::string(package.substr(4))))
                if (const Package::Entry* e = moved->Find(name))
                    return e;
        if (package == kSkinBase)
            if (const Package* own = FindPackage("huduiskin.skin"))
                if (const Package::Entry* e = own->Find(name))
                    return e;
        const Package* p = FindPackage(package);
        return p ? p->Find(name) : nullptr;
    }

    const Image* Assets::LoadImage(std::string_view path, std::string_view basePackage)
    {
        const std::string key = std::string(basePackage) + "|" + std::string(path);
        std::lock_guard lock(imagesMutex_);
        if (const auto it = images_.find(key); it != images_.end())
            return it->second.get();
        std::unique_ptr<Image> image;
        const Package::Entry* e = FindEntry(path, basePackage);
        std::string error;
        if (!e)
            Log("System UI: image {} (base {}) not found", path, basePackage);
        else if (image = std::make_unique<Image>(); !DecodeImage(e->data, e->size, *image, error))
        {
            Log("System UI: image {}: {}", path, error);
            image.reset();
        }
        return images_.emplace(key, std::move(image)).first->second.get();
    }
}
