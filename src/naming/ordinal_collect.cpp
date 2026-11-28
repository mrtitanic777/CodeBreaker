// naming/ordinal_collect.cpp -- see ordinal_collect.hpp.
#include "naming/ordinal_collect.hpp"

#include <algorithm>
#include <map>
#include <mutex>
#include <optional>
#include <set>
#include <variant>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"
#include "formats/ne.hpp"
#include "formats/pe.hpp"
#include "symbols/api_arguments.hpp"

namespace cb::naming {

namespace {

namespace oc_api = symbols::api_arguments;

// The 16-bit modules a corpus holds under another name than the one they give themselves (a driver exports under its interface's name, not
// its file's: Setup's display driver is VGA.DRV, module DISPLAY).
std::vector<std::string> oc_files_of_module(std::string_view upper_name) {
    if (upper_name == "KERNEL") return {"krnl386.exe", "kernel.exe"};
    if (upper_name == "USER") return {"user.exe"};
    if (upper_name == "GDI") return {"gdi.exe"};
    if (upper_name == "DISPLAY") return {"vga.drv", "display.drv"};
    if (upper_name == "MOUSE") return {"msmous31.drv", "lmouse31.drv", "mouse.drv"};
    return {};
}

class CorpusExportCheck final : public symbols::ordinals::ExportCheck {
public:
    CorpusExportCheck(const base::Corpus& corpus, decompile::environment::CorpusModules& modules) : corpus_(corpus), modules_(modules) {}

    // Every export number of the module: a 16-bit one found by its file, a 32-bit one by its file, then a 16-bit one by the name it gives itself.
    std::optional<std::set<std::uint32_t>> numbers(std::string_view stem_in) const override {
        const std::string stem(stem_in);
        if (const auto it = numbers_.find(stem); it != numbers_.end()) return it->second;
        std::optional<std::set<std::uint32_t>> found = ne_numbers_by_file(stem);
        if (!found) found = pe_numbers(stem);
        if (!found) {
            const auto named = ne_by_name().find(stem);
            if (named != ne_by_name().end()) found = ne_numbers(named->second);
        }
        numbers_[stem] = found;
        return found;
    }

    // Does the export with that number remove `argument_bytes` bytes of arguments on return? A 32-bit module's code settles it; nothing else does.
    std::optional<bool> arguments_agree(std::string_view stem, std::uint32_t ordinal, std::uint32_t argument_bytes) const override {
        const std::shared_ptr<const oc_api::ExportingModule> module = modules_.find(stem);
        if (!module) return std::nullopt;
        const std::optional<std::uint32_t> rva = module->export_by_ordinal(ordinal);
        if (!rva || *rva == 0) return std::nullopt;
        try {
            const std::optional<std::uint32_t> got = oc_api::StackScanner(modules_).scan(*module, *rva, 4);
            if (!got) return std::nullopt;
            return *got == argument_bytes;
        } catch (const Error&) {   // (code the scan cannot read does not settle it)
            return std::nullopt;
        }
    }

    // The modules a module of the corpus imports from by number.
    std::set<std::string> imported_by_number() const override {
        std::set<std::string> out;
        for (const std::filesystem::path& path : corpus_.modules(base::Lane::Ne)) {
            try {
                const formats::ne::Module module = formats::ne::Module::open_file(path);
                const std::vector<std::string> refs = module.module_refs();
                for (const formats::ne::Segment& segment : module.segments()) {
                    for (const formats::ne::Relocation& r : module.relocations(segment)) {
                        const auto* target = std::get_if<formats::ne::ImportOrdinalRef>(&r.target);
                        if (target != nullptr && target->module > 0 && target->module <= refs.size()) out.insert(text::upper(refs[target->module - 1]));
                    }
                }
            } catch (const Error&) {   // (a module that does not read imports nothing known)
            }
        }
        for (const std::filesystem::path& path : corpus_.modules(base::Lane::Pe)) {
            try {
                for (const formats::pe::Import& i : formats::pe::Image::open(path).imports())
                    if (i.name.size() > 1 && i.name[0] == '#' && std::all_of(i.name.begin() + 1, i.name.end(), [](char c) { return c >= '0' && c <= '9'; }))
                        out.insert(symbols::ordinals::module_stem(i.dll));
            } catch (const Error&) {
            }
        }
        return out;
    }

private:
    bool is_lane(const std::filesystem::path& path, base::Lane lane) const { return corpus_.lane(path) == std::optional<base::Lane>(lane); }

    static std::optional<std::set<std::uint32_t>> ne_numbers(const std::filesystem::path& path) {
        try {
            const std::vector<std::uint32_t> all = formats::ne::Module::open_file(path).export_ordinals();
            return std::set<std::uint32_t>(all.begin(), all.end());
        } catch (const Error&) {
            return std::nullopt;
        }
    }

    std::optional<std::set<std::uint32_t>> ne_numbers_by_file(const std::string& stem) const {
        std::vector<std::string> candidates = oc_files_of_module(stem);
        for (const char* ext : {".dll", ".exe", ".drv"}) candidates.push_back(text::lower(stem) + ext);
        for (const std::string& candidate : candidates) {
            const std::optional<std::filesystem::path> path = corpus_.retail(candidate);
            if (!path || path->empty() || !is_lane(*path, base::Lane::Ne)) continue;
            if (auto found = ne_numbers(*path)) return found;
        }
        return std::nullopt;
    }

    std::optional<std::set<std::uint32_t>> pe_numbers(const std::string& stem) const {
        for (const std::string_view ext : oc_api::module_extensions()) {
            const std::optional<std::filesystem::path> path = corpus_.retail(text::lower(stem) + std::string(ext));
            if (!path || path->empty() || !is_lane(*path, base::Lane::Pe)) continue;
            try {
                std::set<std::uint32_t> out;
                for (const formats::pe::Export& e : formats::pe::Image::open(*path).exports()) out.insert(static_cast<std::uint32_t>(e.ordinal));
                return out;
            } catch (const Error&) {
            }
        }
        return std::nullopt;
    }

    // The corpus's 16-bit modules by the name each gives itself, all opened, once.
    const std::map<std::string, std::filesystem::path>& ne_by_name() const {
        if (!by_name_) {
            std::map<std::string, std::filesystem::path> found;
            for (const std::filesystem::path& path : corpus_.modules(base::Lane::Ne)) {
                try {
                    found.try_emplace(text::upper(formats::ne::Module::open_file(path).module_name()), path);
                } catch (const Error&) {
                }
            }
            by_name_ = std::move(found);
        }
        return *by_name_;
    }

    const base::Corpus& corpus_;
    decompile::environment::CorpusModules& modules_;
    mutable std::map<std::string, std::optional<std::set<std::uint32_t>>> numbers_;   // by stem; lookup only
    mutable std::optional<std::map<std::string, std::filesystem::path>> by_name_;
};

}  // namespace

symbols::ordinals::ShippedTables remake_ordinal_table(const base::Corpus& corpus, decompile::environment::CorpusModules& modules,
                                             std::span<const std::filesystem::path> paths, const std::function<void(std::string_view)>& log) {
    const CorpusExportCheck check(corpus, modules);
    return symbols::ordinals::collect(paths, modules, check, log);
}

}  // namespace cb::naming
