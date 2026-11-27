// dotnet/decompiler/documentation/idocumentation_provider.hpp -- what gives the XML documentation of an entity: the text of its `<member>` in the
// documentation file the compiler wrote.
//
//     std::optional<std::string> doc = provider->get_documentation(entity);       // "<summary>...</summary>" ; nullopt for none
//
// Ported from ILSpy (ICSharpCode.Decompiler.Documentation: IDocumentationProvider, from XmlDocumentationProvider.cs), Copyright AlphaSierraPapa for
// the ILSpy team, MIT (see NOTICE in dotnet/decompiler). Upstream's null documentation is nullopt (an empty text is a documentation of nothing, which
// is different). XmlException, which the readers throw for an XML file that is not well formed, is util::XmlError.
#pragma once

#include <optional>
#include <string>

#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/decompiler/util/xml_text_reader.hpp"

namespace cb::dotnet::decompiler::documentation {

// System.Xml.XmlException
using XmlException = util::XmlError;

// Provides XML documentation for type and member definitions in source code.
class IDocumentationProvider {
public:
    virtual ~IDocumentationProvider() = default;

    // Returns the XML documentation for the given `entity`. May return nullopt, if no documentation is present for the entity.
    virtual std::optional<std::string> get_documentation(const typesystem::IEntity* entity) const = 0;
};

}  // namespace cb::dotnet::decompiler::documentation
