R"CBHELP(
Remarks:
  -o is valid with every option and required when using -p.

Examples:
    Decompile assembly to console out.
        ilspycmd sample.dll

    Decompile assembly to destination directory (single C# file).
        ilspycmd -o c:\decompiled sample.dll

    Decompile assembly to destination directory, create a project file, one source file per type.
        ilspycmd -p -o c:\decompiled sample.dll

    Decompile assembly to destination directory, create a project file, one source file per type, 
    into nicely nested directories.
        ilspycmd --nested-directories -p -o c:\decompiled sample.dll

    Generate a HTML diagrammer containing all type info into a folder next to the input assembly
        ilspycmd sample.dll --generate-diagrammer

    Generate a HTML diagrammer containing filtered type info into a custom output folder
    (including types in the LightJson namespace while excluding types in nested LightJson.Serialization namespace)
        ilspycmd sample.dll --generate-diagrammer -o c:\diagrammer --generate-diagrammer-include LightJson\\..+ --generate-diagrammer-exclude LightJson\\.Serialization\\..+

    List all embedded resources in a WPF assembly (including BAML entries inside .g.resources containers).
        ilspycmd sample.dll --list-resources

    Extract a single resource. If the name ends with .baml, the output is decompiled XAML; otherwise raw bytes.
        ilspycmd sample.dll --resource sample.g.resources/mainwindow.baml -o c:\decompiled

    Decompile assembly as a compilable project and convert all BAML resources to XAML Page items.
        ilspycmd sample.dll -p -o c:\decompiled --decompile-baml
)CBHELP"
