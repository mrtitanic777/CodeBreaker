// dotnet/decompiler/ilspycmd/single_file_bundle.hpp -- a .NET single-file bundle (an apphost with its assemblies, config and native files
// appended): finding it in a file, and reading its manifest (what each embedded file is called, where it lies, how large it is).
//
//     Bytes file = fs::read_file("App.exe");
//     std::int64_t header_offset;
//     if (SingleFileBundle::is_bundle(file, &header_offset)) {
//         SingleFileBundle::Header header = SingleFileBundle::read_manifest(file, header_offset);
//         for (const SingleFileBundle::Entry& e : header.entries) ...      // e.relative_path, e.offset, e.size, e.compressed_size
//     }
//
// Ported from ILSpy (ICSharpCode.Decompiler: SingleFileBundle, from SingleFileBundle.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT
// (see NOTICE in dotnet/decompiler). Written for ilspycmd's --dump-package, which is its only caller: if the decompiler grows a home for
// this class, it moves there. The file is read whole (upstream memory-maps it); a manifest that does not fit the file is
// InvalidDataError (InvalidDataException) and a read past the end EndOfStreamError.
#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "core/bytes.hpp"
#include "core/error.hpp"

namespace cb::dotnet::decompiler::ilspycmd {

// System.IO.InvalidDataException.
class InvalidDataError : public Error {
public:
    using Error::Error;
    CB_ERROR_BOILERPLATE(InvalidDataError)
};

class SingleFileBundle {
public:
    // Identifies the type of file embedded into the bundle.
    enum class FileType : std::uint8_t {
        Unknown,             // type not determined
        Assembly,            // IL and R2R assemblies
        NativeBinary,        // native binaries
        DepsJson,            // .deps.json configuration file
        RuntimeConfigJson,   // .runtimeconfig.json configuration file
        Symbols,             // PDB files
    };

    struct Entry {
        std::int64_t offset = 0;
        std::int64_t size = 0;
        std::int64_t compressed_size = 0;   // 0 if not compressed, otherwise the compressed size in the bundle
        FileType type = FileType::Unknown;
        std::string relative_path;          // path of an embedded file, relative to the bundle's source directory
    };

    struct Header {
        std::uint32_t major_version = 0;
        std::uint32_t minor_version = 0;
        int file_count = 0;
        std::string bundle_id;

        // introduced with v2
        std::int64_t deps_json_offset = 0;
        std::int64_t deps_json_size = 0;
        std::int64_t runtime_config_json_offset = 0;
        std::int64_t runtime_config_json_size = 0;
        std::uint64_t flags = 0;

        std::vector<Entry> entries;
    };

    // Whether the data holds a bundle's signature (with the offset of the bundle header before it).
    static bool is_bundle(ByteView data, std::int64_t* bundle_header_offset);

    // The manifest header at `bundle_header_offset`: InvalidDataError for an unsupported version or a file count the data cannot hold,
    // EndOfStreamError for data that ends inside the manifest.
    static Header read_manifest(ByteView data, std::int64_t bundle_header_offset);
};

}  // namespace cb::dotnet::decompiler::ilspycmd
