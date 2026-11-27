// dotnet/decompiler/ilspycmd/resource_extensions.cpp -- see resource_extensions.hpp.
#include "dotnet/decompiler/ilspycmd/resource_extensions.hpp"

#include <memory>
#include <optional>

#include "dotnet/decompiler/metadata/resource.hpp"
#include "dotnet/decompiler/util/platform.hpp"
#include "dotnet/metadata/blob_reader.hpp"

namespace cb::dotnet::decompiler::ilspycmd {
namespace {

bool ends_with_ignore_case(const std::string& text, std::string_view suffix) {
    return text.size() >= suffix.size() && util::compare_ordinal_ignore_case(std::string_view(text).substr(text.size() - suffix.size()), suffix) == 0;
}

bool starts_with_ignore_case(const std::string& text, std::string_view prefix) {
    return text.size() >= prefix.size() && util::compare_ordinal_ignore_case(std::string_view(text).substr(0, prefix.size()), prefix) == 0;
}

// The names of the entries of a .resources resource; false for one that has no data or is not a .resources file.
bool try_read_resources_entry_names(const metadata::Resource& r, std::vector<std::string>* names) {
    names->clear();
    const std::optional<ByteView> stream = r.try_open_stream();
    if (!stream) return false;
    try {
        const util::ResourcesFile resources_file(*stream);
        for (int i = 0; i < resources_file.resource_count(); ++i) names->push_back(resources_file.get_resource_name(i));
        return true;
    } catch (const cb::dotnet::metadata::BadImageFormatError&) {
        names->clear();
        return false;
    } catch (const util::EndOfStreamError&) {
        names->clear();
        return false;
    }
}

bool try_read_resources_entry(const metadata::Resource& r, const std::string& entry_name, util::ResourceValue* value) {
    const std::optional<ByteView> stream = r.try_open_stream();
    if (!stream) return false;
    try {
        const util::ResourcesFile resources_file(*stream);
        for (int i = 0; i < resources_file.resource_count(); ++i) {
            if (util::compare_ordinal_ignore_case(resources_file.get_resource_name(i), entry_name) != 0) continue;
            util::ResourceValue entry_value = resources_file.get_resource_value(i);
            if (auto* serialized = std::get_if<util::ResourceSerializedObject>(&entry_value))
                *value = Bytes(serialized->get_bytes());
            else if (auto* s = std::get_if<util::ResourceStream>(&entry_value))
                *value = Bytes(s->bytes);
            else
                *value = std::move(entry_value);
            return true;
        }
    } catch (const cb::dotnet::metadata::BadImageFormatError&) {
        return false;
    } catch (const util::EndOfStreamError&) {
        return false;
    }
    return false;
}

}  // namespace

std::vector<std::string> ResourceExtensions::enumerate_resource_paths(const metadata::MetadataFile& module) {
    std::vector<std::string> paths;
    for (const std::unique_ptr<metadata::Resource>& r : metadata::get_resources(module)) {
        if (r->resource_type() != metadata::ResourceType::Embedded) continue;
        const std::string name = r->name();
        std::vector<std::string> entries;
        if (ends_with_ignore_case(name, ".resources") && try_read_resources_entry_names(*r, &entries)) {
            for (const std::string& entry : entries) paths.push_back(name + "/" + entry);
        } else {
            paths.push_back(name);
        }
    }
    return paths;
}

bool ResourceExtensions::try_get_resource(const metadata::MetadataFile& module, const std::string& resource_path, util::ResourceValue* value) {
    for (const std::unique_ptr<metadata::Resource>& r : metadata::get_resources(module)) {
        if (r->resource_type() != metadata::ResourceType::Embedded) continue;
        const std::string name = r->name();
        if (util::compare_ordinal_ignore_case(resource_path, name) == 0) {
            const std::optional<ByteView> stream = r->try_open_stream();
            if (!stream) {
                *value = std::monostate();
                return false;
            }
            *value = Bytes(stream->begin(), stream->end());
            return true;
        }

        const std::string prefix = name + "/";
        if (ends_with_ignore_case(name, ".resources") && starts_with_ignore_case(resource_path, prefix)) {
            const std::string entry_name = resource_path.substr(prefix.size());
            if (try_read_resources_entry(*r, entry_name, value)) return true;
        }
    }
    *value = std::monostate();
    return false;
}

}  // namespace cb::dotnet::decompiler::ilspycmd
