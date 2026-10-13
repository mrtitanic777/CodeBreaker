// XEXMagic - Interactive web UI backend
//
// Serves a whole-program decompilation model over localhost HTTP and a
// single-page browser frontend. Functions and locals can be renamed
// interactively; renames propagate to every reference on re-decompile and
// persist to a <input>.xdb sidecar JSON.

#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <vector>

#include "il.h"

// Everything the UI needs about the loaded image. The caller builds the IL
// (whole-program) exactly as the -c/decompile path does and hands it here.
struct WebProgram {
  std::string module_name;                 // "xboxkrnl" etc. ("" if unknown)
  std::string input_path;                  // for locating the .xdb sidecar
  uint32_t base = 0;
  std::vector<ILFunction> funcs;           // moved in
  std::map<uint32_t, std::string> symbols; // addr -> name (funcs + data exports)
  std::map<uint32_t, uint32_t> ordinals;   // export addr -> ordinal (for headers)
  uint32_t entry_point = 0;
  std::map<uint32_t, std::string> strings; // address -> literal, for search+refs
  // What the image imports. A XEX imports purely by ordinal, so the name is
  // resolved from the baked kernel tables; searching these is how you find
  // where a title calls into xam or the kernel.
  struct Import {
    std::string module;    // "xam", "xboxkrnl"
    std::string name;      // resolved name, or xam_ordinal_1234
    uint32_t ordinal = 0;
    uint32_t thunk_address = 0;
    uint32_t value_address = 0;
  };
  std::vector<Import> imports;

  // The input is a COFF archive laid out as one image, not a linked binary.
  // Its symbols are already real names, and matching it against the SDK's own
  // code signatures would be circular.
  bool is_archive = false;

  // One section of the loaded image, for the memory view and table scanner.
  struct Section {
    std::string name;
    uint32_t address = 0;   // virtual address
    uint32_t size = 0;      // usable bytes at `address`
    uint32_t file_offset = 0;
    bool code = false;
  };
  std::vector<Section> sections;

  // Borrowed view of the loaded image, indexed by (va - base). Must outlive
  // the server; the serve path never returns so the caller's buffer is fine.
  const std::vector<uint8_t>* image = nullptr;

  // Builds a function at an arbitrary code address that whole-program detection
  // missed (call targets inside a detected function's range, cold stubs, ...).
  // Returns false when the address isn't in a code section. Lets the UI
  // navigate to ANY call target, not just pre-detected entries.
  std::function<bool(uint32_t, ILFunction*)> build_at;
};

// Binds and starts listening on 127.0.0.1:<port> immediately, on its own
// thread, and returns. Until SetWebProgram hands over a model, every request
// is answered with a self-refreshing progress page built from progress.h - so
// --serve can put the port up before the image has been analysed.
// Returns non-zero on a fatal socket error.
int StartWebServer(int port);

// Publishes the finished model. Indexes the SDK first, as the prototypes have
// to be in place before the whole-program pass runs.
void SetWebProgram(WebProgram program);

// True until the SDK libraries have been indexed at least once. The index is
// what puts real names on library code, and it has to be in place before an
// image is analysed rather than after.
bool SetupRequired();

// Serves the setup page and blocks until the user has indexed an SDK or
// chosen to skip. Nothing else is served meanwhile.
void RunSetup();

// --serve with no file: the UI shows a drop target instead of a model, and
// opening one relaunches the process against it.
void SetAwaitingFile(bool awaiting);

// --no-browser-launch: do not `start` the UI URL when the port comes up.
// Set before StartWebServer.
void SetNoBrowserLaunch(bool v);

// Blocks forever; the server runs until the process is killed.
void WaitWebServer();

// StartWebServer + SetWebProgram + WaitWebServer, for callers that have the
// model in hand already. Returns non-zero on a fatal socket error.
int RunWebServer(WebProgram program, int port);
