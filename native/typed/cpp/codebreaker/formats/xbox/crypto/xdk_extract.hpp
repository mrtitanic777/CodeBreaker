// codebreaker/formats/xbox/crypto/xdk_extract.hpp -- C++ from codebreaker.formats.xbox.crypto.xdk_extract.
//
// A script to extract XDKRecoveryXenonXXXXX.exe and XDKSetupXenonXXXXX.exe files
// (Python's __author__ = "Visual Studio", __description__ = that line, __platforms__ = Windows, Linux, macOS;
// double-underscore names are reserved in C++, so they are author, description and platforms here).
#pragma once

#include <cstdint>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::xbox::crypto::xdk_extract {

extern const py::str author;
extern const py::str description;
extern const py::list<py::str> platforms;

// Python's SystemExit (what argparse raises for --help and for a bad command line): `code` is the exit status.
using py::SystemExit;

inline constexpr int64_t DOSMagic = 0x5A4D;
extern const py::bytes PEMagic;
extern const py::bytes CABMagic;
inline constexpr int64_t MachineI386 = 0x14C;
inline constexpr int64_t NumberOfDataDirectories = 16;
inline constexpr int64_t NameSize = 8;

// The ctypes Structures (little-endian, natural alignment). sizeof is the SIZE of each; from_buffer_copy
// reads one from the front of a bytes (ValueError if it is shorter than the structure), as ctypes does.
struct CabHeader {
    static constexpr int64_t SIZE = 12;
    py::list<int64_t> Magic;   // c_ubyte * 4
    int64_t Reserved = 0;
    int64_t PackedSize = 0;
    static CabHeader from_buffer_copy(const py::bytes& buffer);
};

struct DOSHeader {
    static constexpr int64_t SIZE = 64;
    int64_t Magic = 0;
    int64_t UsedBytesInTheLastPage = 0;
    int64_t FileSizeInPages = 0;
    int64_t NumberOfRelocationItems = 0;
    int64_t HeaderSizeInParagraphs = 0;
    int64_t MinimumExtraParagraphs = 0;
    int64_t MaximumExtraParagraphs = 0;
    int64_t InitialRelativeSS = 0;
    int64_t InitialSP_first = 0;   // the Python lists "InitialSP" twice; its attribute InitialSP is the second one
    int64_t InitialSP = 0;
    int64_t InitialIP = 0;
    int64_t InitialRelativeCS = 0;
    int64_t AddressOfRelocationTable = 0;
    int64_t OverlayNumber = 0;
    py::list<int64_t> Reserved;    // c_uint16 * 4
    int64_t OEMid = 0;
    int64_t OEMinfo = 0;
    py::list<int64_t> Reserved2;   // c_uint16 * 10
    int64_t AddressOfNewExeHeader = 0;
    static DOSHeader from_buffer_copy(const py::bytes& buffer);
};

struct FileHeader {
    static constexpr int64_t SIZE = 20;
    int64_t Machine = 0;
    int64_t NumberOfSections = 0;
    int64_t TimeDateStamp = 0;
    int64_t PointerToSymbolTable = 0;
    int64_t NumberOfSymbols = 0;
    int64_t SizeOfOptionalHeader = 0;
    int64_t Characteristics = 0;
    static FileHeader from_buffer_copy(const py::bytes& buffer);
};

struct DataDirectory {
    static constexpr int64_t SIZE = 8;
    int64_t RelativeVirtualAddress = 0;
    int64_t Size = 0;
    static DataDirectory from_buffer_copy(const py::bytes& buffer);
};

struct PE32Header {
    static constexpr int64_t SIZE = 224;
    int64_t Magic = 0;
    int64_t MajorLinkerVersion = 0;
    int64_t MinorLinkerVersion = 0;
    int64_t SizeOfCode = 0;
    int64_t SizeOfInitializedData = 0;
    int64_t SizeOfUninitializedData = 0;
    int64_t AddressOfEntryPoint = 0;
    int64_t BaseOfCode = 0;
    int64_t BaseOfData = 0;
    int64_t ImageBase = 0;
    int64_t SectionAlignment = 0;
    int64_t FileAlignment = 0;
    int64_t MajorOperatingSystemVersion = 0;
    int64_t MinorOperatingSystemVersion = 0;
    int64_t MajorImageVersion = 0;
    int64_t MinorImageVersion = 0;
    int64_t MajorSubsystemVersion = 0;
    int64_t MinorSubsystemVersion = 0;
    int64_t Win32VersionValue = 0;
    int64_t SizeOfImage = 0;
    int64_t SizeOfHeaders = 0;
    int64_t CheckSum = 0;
    int64_t Subsystem = 0;
    int64_t DLLCharacteristics = 0;
    int64_t SizeOfStackReserve = 0;
    int64_t SizeOfStackCommit = 0;
    int64_t SizeOfHeapReserve = 0;
    int64_t SizeOfHeapCommit = 0;
    int64_t LoaderFlags = 0;
    int64_t NumberOfRvaAndSize = 0;
    py::list<DataDirectory> DataDirectories;   // DataDirectory * NumberOfDataDirectories
    static PE32Header from_buffer_copy(const py::bytes& buffer);
};

struct SectionHeader {
    static constexpr int64_t SIZE = 40;
    py::list<int64_t> Name;   // c_ubyte * NameSize
    int64_t VirtualSize = 0;
    int64_t VirtualAddress = 0;
    int64_t SizeOfRawData = 0;
    int64_t PointerToRawData = 0;
    int64_t PointerToRelocations = 0;
    int64_t PointerToLineNumbers = 0;
    int64_t NumberOfRelocations = 0;
    int64_t NumberOfLineNumbers = 0;
    int64_t Characteristics = 0;
    static SectionHeader from_buffer_copy(const py::bytes& buffer);
};

// Where the executable's own bytes end (and the appended cabinets begin): the end of the last section's raw data.
// `stream` is an open binary file. AssertionError for a bad DOS header, PE signature or machine; ValueError
// (ctypes) when the file is too short for a header it reads.
int64_t get_size_of_image(const py::file& stream);

// The command line: `argv` is sys.argv[1:], `prog` the basename of sys.argv[0] (argparse's prog). Raises
// SystemExit as argparse does (a usage error: code 2 with the usage and message on stderr; --help: code 0
// with the help on stdout). The help is laid out for an 80-column terminal, argparse's width when it cannot
// ask the terminal (COLUMNS unset); a prog long enough to make argparse wrap the usage line is not wrapped.
void main(const py::list<py::str>& argv, const py::str& prog = py::str("xdk_extract.py"));

}  // namespace codebreaker::formats::xbox::crypto::xdk_extract
