// dotnet/decompiler/csharp/project_decompiler/project_item_info.hpp -- one file of a project as its project file lists it: the kind of item
// ("Compile", "EmbeddedResource", "ApplicationIcon" ...), its file name, the parts of partial types it holds, and extra properties the item carries
// (the LogicalName of a resource).
//
//     ProjectItemInfo item("EmbeddedResource", "Strings.resx");
//     item.with("LogicalName", "App.Strings.resources");
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.ProjectDecompiler: ProjectItemInfo, from WholeProjectDecompiler.cs), Copyright AlphaSierraPapa for
// the ILSpy team, MIT (see NOTICE in dotnet/decompiler). Upstream's record struct is a value: copying it copies the references to its two lists, which
// the copy then shares; here a copy has lists of its own. The properties are kept in the order they were added (a Dictionary's order when nothing
// is removed from it), a name added twice is a UsageError (Dictionary.Add's ArgumentException).
#pragma once

#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "core/error.hpp"
#include "dotnet/decompiler/partial_type_info.hpp"

namespace cb::dotnet::decompiler::csharp::project_decompiler {

struct ProjectItemInfo {
    std::string item_type;
    std::string file_name;
    std::optional<std::vector<PartialTypeInfo>> partial_types;
    std::optional<std::vector<std::pair<std::string, std::string>>> additional_properties;

    ProjectItemInfo() = default;
    ProjectItemInfo(std::string item_type_, std::string file_name_) : item_type(std::move(item_type_)), file_name(std::move(file_name_)) {}

    // Adds a property to the item; returns the item. UsageError when the item has one of that name.
    ProjectItemInfo& with(const std::string& name, const std::string& value) {
        if (!additional_properties) additional_properties.emplace();
        for (const auto& [existing, ignored] : *additional_properties)
            if (existing == name) throw UsageError("An item with the same key has already been added: " + name);
        additional_properties->emplace_back(name, value);
        return *this;
    }
    ProjectItemInfo& with(const std::vector<std::pair<std::string, std::string>>& pairs) {
        for (const auto& [name, value] : pairs) with(name, value);
        if (!additional_properties) additional_properties.emplace();
        return *this;
    }
};

}  // namespace cb::dotnet::decompiler::csharp::project_decompiler
