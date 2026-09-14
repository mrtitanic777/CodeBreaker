// Xbox 360 SDK location + a searchable index of the types it declares.
//
// The tool infers struct layouts from how code touches memory, which gives
// offsets and sizes but never names. The SDK headers have the names. Pointing
// XEXMagic at an SDK lets an inferred struct be matched against the real
// declarations instead of staying as field_0C forever.
#pragma once

#include <cstdint>
#include <string>
#include <map>
#include <vector>

// From coff_lib.h, at global scope. Forward declared so this header does not
// drag the COFF parser into every translation unit that wants the config.
struct CoffObject;

namespace xexmagic {

struct SdkField {
  std::string type;
  std::string name;
  uint32_t offset = 0;  // byte offset, as computed while parsing
  uint32_t size = 0;    // 0 when the type is not one we can size
  uint32_t array_count = 1;
};

struct SdkStruct {
  std::string name;
  std::string header;  // path the declaration came from, for attribution
  uint32_t size = 0;   // 0 when it contains something unsizable
  std::vector<SdkField> fields;
};

// Persisted settings. Lives beside the executable as xexmagic.json so it
// survives across images -- an SDK path is a property of the machine, not of
// the file being analysed.
class SdkConfig {
 public:
  // Loads from disk; missing or malformed config is not an error.
  static SdkConfig& Get();

  const std::string& sdk_path() const { return sdk_path_; }

  // Validates and stores the path, then writes the config. Returns false with
  // `error` set when the directory does not look like an Xbox 360 SDK.
  bool SetSdkPath(const std::string& path, std::string* error);

  // Where headers were found under sdk_path (…/include/xbox, …/include, …).
  const std::vector<std::string>& include_dirs() const { return include_dirs_; }

  bool indexed() const { return indexed_; }
  size_t struct_count() const { return structs_.size(); }
  size_t header_count() const { return header_count_; }
  size_t symbol_count() const { return symbols_.size(); }
  size_t lib_count() const { return lib_count_; }

  // Where the SDK's PPC .lib files live (lib/xbox and friends).
  const std::vector<std::string>& lib_dirs() const { return lib_dirs_; }

  // %APPDATA%\XEXMagic\Libraries.XDG - the indexed libraries. Its existence is
  // what tells the server whether first-run setup has been done.
  static std::string LibraryCachePath();

  // A function the SDK actually ships. If the image contains a call to
  // something we can tie back to one of these, it has a real name rather than
  // sub_82xxxxxx.
  struct SdkSymbol {
    std::string name;      // as stored in the archive (may be decorated)
    std::string demangled; // readable form when the name was C++-mangled
    std::string lib;       // xapilib.lib, xnet.lib, ...
    std::string obj;       // archive member it came from
    uint32_t code_size = 0;  // bytes of .text, 0 when it is not a function
  };

  // A function declaration from the SDK headers. Argument types and names are
  // not recoverable from code at all - a prototype is the only source for
  // them, and it is what turns `a5` into `D3DFORMAT DstFormat`.
  struct SdkPrototype {
    std::string name;
    std::string ret;
    std::string header;
    // (type, name); the name is empty for an unnamed parameter.
    std::vector<std::pair<std::string, std::string>> params;
  };

  const SdkPrototype* FindPrototype(const std::string& name) const;
  size_t prototype_count() const { return prototypes_.size(); }

  const SdkSymbol* FindSymbol(const std::string& name) const;
  std::vector<const SdkSymbol*> SearchSymbols(const std::string& needle,
                                              size_t limit) const;

  // A function body from a lib, prepared for comparison against image code.
  //
  // Linking rewrites every field the archive marked as relocatable -- branch
  // displacements, and the halves of lis/ori address pairs -- so those bytes
  // differ between the .lib and the linked image while everything else is
  // byte-identical. `mask` lists (offset,length) of the fields to ignore, taken
  // straight from the COFF relocation table, which is what makes the compare
  // exact rather than fuzzy.
  struct CodeSig {
    std::string name;
    std::string demangled;
    std::string lib;
    uint32_t size = 0;                                  // bytes of code
    std::vector<std::pair<uint32_t, uint32_t>> mask;    // offset, length
    uint64_t hash = 0;                                  // of the masked bytes
    std::vector<uint8_t> body;                          // for window indexing
  };

  // Signatures grouped by body size; matching only ever compares equal sizes.
  const std::vector<CodeSig>* SigsForSize(uint32_t size) const;

  // Hash `code` (size bytes) with the given mask applied, so it can be compared
  // against CodeSig::hash.
  static uint64_t HashMasked(const uint8_t* code, uint32_t size,
                             const std::vector<std::pair<uint32_t, uint32_t>>& mask);

  size_t sig_count() const { return sig_total_; }
  size_t window_count() const { return windows_.size(); }

  // Inlined library code: a run of instructions from a lib body found INSIDE a
  // larger function. Whole-function matching cannot see these, because the
  // compiler spliced the body into its caller rather than calling it.
  struct InlineHit {
    std::string name;
    std::string demangled;
    std::string lib;
    uint32_t offset = 0;   // byte offset within the image function
    uint32_t insns = 0;    // how many instructions matched
  };

  // Find library code inlined in `code`. Windows that occur in more than one
  // library function are dropped at index time, so a hit names exactly one
  // symbol or it is not reported at all.
  std::vector<InlineHit> FindInlined(const uint8_t* code, uint32_t size) const;

  // Parses every header under include_dirs. Safe to call repeatedly; it clears
  // and rebuilds. Returns the number of structs found.
  // Indexes the headers, and the SDK's own .lib files unless told not to.
  // The lib half is only ever consumed by the byte-exact code scan and the
  // inline search, so for an input that is itself a library - where both are
  // circular - it is pure cost and is skipped.
  size_t BuildIndex(bool include_libs = true);

  // Exact match by name (case-insensitive), or nullptr.
  const SdkStruct* FindStruct(const std::string& name) const;

  // Structs whose name contains `needle`, capped at `limit`.
  std::vector<const SdkStruct*> SearchStructs(const std::string& needle,
                                              size_t limit) const;

  // Candidate matches for an inferred layout: structs whose size matches (when
  // known) and which have a declared field at every offset given. Best first.
  struct Candidate {
    const SdkStruct* def;
    int score;
    std::string why;
  };
  std::vector<Candidate> MatchLayout(uint32_t size,
                                     const std::vector<uint32_t>& offsets,
                                     size_t limit) const;

  std::string ConfigJson() const;
  std::string StructJson(const SdkStruct& s) const;

 private:
  SdkConfig() { Load(); }

  void Load();
  void Save() const;
  static std::string ConfigPath();
  bool ScanIncludeDirs(const std::string& path,
                       std::vector<std::string>* out) const;
  void ScanLibDirs(const std::string& path, std::vector<std::string>* out) const;
  // What parsing one file produced. The index is built across every core, so
  // the parsers append here rather than to the shared containers, and the
  // results are merged in file order afterwards - a parallel index is then
  // identical to a serial one, which a race over a shared map would not be.
  struct IndexStage {
    std::vector<SdkStruct> structs;
    std::vector<std::pair<std::string, SdkPrototype>> prototypes;
    std::vector<SdkSymbol> symbols;
    std::vector<std::pair<uint32_t, CodeSig>> sigs;
    bool header = false;
    bool lib = false;
  };

  void ParseHeader(const std::string& path, IndexStage* out);
  // Second, whole-file pass over the same header: declarations span lines, so
  // the line-oriented struct parser cannot see them.
  void ParseHeaderPrototypes(const std::string& path, IndexStage* out);
  void ParseLib(const std::string& path, IndexStage* out);
  void BuildSigsForObject(const ::CoffObject& obj, const std::string& lib_name,
                          IndexStage* out);
  // Second pass: size fields whose type is another indexed struct.
  void ResolveNestedSizes();

  std::string sdk_path_;
  std::vector<std::string> include_dirs_;
  std::vector<std::string> lib_dirs_;
  std::vector<SdkStruct> structs_;
  std::map<std::string, SdkPrototype> prototypes_;
  std::vector<SdkSymbol> symbols_;
  std::map<uint32_t, std::vector<CodeSig>> sigs_;  // body size -> candidates
  size_t sig_total_ = 0;

  // Instruction-window hash -> which library function it came from. Built once
  // after all libs are parsed; ambiguous windows are removed.
  struct WindowOwner {
    std::string name;
    std::string demangled;
    std::string lib;
  };
  std::map<uint64_t, WindowOwner> windows_;

  void BuildWindowIndex();

  // ---- library cache ----
  //
  // Parsing the SDK's 132 .lib files - 444k symbols, 231k code signatures,
  // every C++ name pushed through DbgHelp one at a time because it cannot be
  // called concurrently - is the slowest part of a cold start and its input
  // never changes. The result is kept in %APPDATA%\XEXMagic\Libraries.XDG and
  // reloaded when the libraries on disk still match.

  // Identity of the .lib set: path, size and write time of every file.
  // Rebuilds the cache when any of them moves.
  uint64_t LibraryFingerprint() const;
  bool LoadLibraryCache(uint64_t fingerprint);
  void SaveLibraryCache(uint64_t fingerprint) const;
  size_t header_count_ = 0;
  size_t lib_count_ = 0;
  bool indexed_ = false;
};

}  // namespace xexmagic
