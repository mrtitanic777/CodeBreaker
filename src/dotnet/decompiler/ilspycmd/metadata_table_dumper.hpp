// dotnet/decompiler/ilspycmd/metadata_table_dumper.hpp -- the --dump-table mode of ilspycmd: every row of one metadata table (its RID,
// token, resolved names, heap offsets and coded indexes) as an aligned text table or as JSON.
//
//     srm::TableIndex table;
//     if (MetadataTableDumper::try_parse_table_name("typedef", &table))                 // a name (any case) or a number (0x02, 2)
//         MetadataTableDumper::dump_table("Sample.dll", std::cout, table, false);
//
// Ported from ILSpy (ICSharpCode.ILSpyCmd: MetadataTableDumper, from MetadataTableDumper.cs), Copyright (c) 2026 AlphaSierraPapa for the
// SharpDevelop Team, MIT (see NOTICE in dotnet/decompiler). The columns of every table are spelled out in ECMA-335 declaration order, as
// upstream does, so the output is the same on every runtime.
//
// Differences: the columns are strings made at once (upstream boxes handles and enums and formats them later; the text is the same, and a
// column that is a number in the JSON is only the RID). Enums print as .NET's Enum.ToString does (dotnet_enum.hpp). A line ends with
// CR LF (Environment.NewLine on Windows), JSON as System.Text.Json writes it with its default encoder (what is not Basic Latin, and
// < > & ' + and the back quote, are \u escapes).
#pragma once

#include <iosfwd>
#include <string>
#include <string_view>

#include "dotnet/metadata/tables.hpp"

namespace cb::dotnet::decompiler::ilspycmd {

class MetadataTableDumper {
public:
    // "Module (0x00), TypeRef (0x01), ..." : the tables the dump knows, as the unknown-table message lists them.
    static std::string supported_table_names();

    // A table by its ECMA-335 name (any case) or number (decimal, or 0x-prefixed hex); false for one that is not a table the dump knows.
    static bool try_parse_table_name(std::string_view name, cb::dotnet::metadata::TableIndex* table);

    // TableIndex.ToString().
    static std::string table_to_string(cb::dotnet::metadata::TableIndex table);

    // Writes every row of the table of the assembly: 0 (the exit code). The assembly's file name is written as given into the JSON.
    static int dump_table(const std::string& assembly_file_name, std::ostream& output, cb::dotnet::metadata::TableIndex table, bool as_json);
};

}  // namespace cb::dotnet::decompiler::ilspycmd
