// XEXMagic - Coarse progress reporting for long-running work.
//
// The web server starts listening before the image has been analysed, so it
// needs to be able to say what the rest of the process is doing. Any phase
// that takes noticeable time publishes its state here and the server renders
// it; the reporting side is a handful of atomics and never blocks the worker.

#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace xexmagic {
namespace progress {

// What the work is doing right now, as the server renders it.
struct Snapshot {
  std::string phase;                  // "Building functions", "Exporting", ...
  size_t done = 0;
  size_t total = 0;                   // 0 = indeterminate
  std::vector<std::string> current;   // items in flight, one per worker
  bool ready = false;                 // the model is up and serveable
  int phase_seconds = 0;              // how long this phase has been running
  int total_seconds = 0;              // how long the process has been working
};

// Starts a phase; resets the counters and the in-flight list.
void BeginPhase(const std::string& phase, size_t total = 0);

// Records an item as in flight for the calling thread.
void SetCurrent(const std::string& item);

// The calling thread finished its item: counts it and clears it. Counting on
// completion rather than on claim is what keeps a parallel phase from showing
// 100% while its last few items are still running.
void Complete();

// The model is built; the server can serve it.
void SetReady(bool ready);

Snapshot Get();

}  // namespace progress
}  // namespace xexmagic
