// XEXMagic - Coarse progress reporting. See progress.h.

#include "progress.h"

#include <atomic>
#include <chrono>
#include <map>
#include <mutex>
#include <thread>

namespace xexmagic {
namespace progress {

namespace {

std::mutex g_mu;
std::string g_phase;
std::atomic<size_t> g_done{0};
std::atomic<size_t> g_total{0};
std::atomic<bool> g_ready{false};
// One entry per worker thread, so a parallel phase can show everything it is
// chewing on rather than whichever item happened to be last.
std::map<std::thread::id, std::string> g_current;

using Clock = std::chrono::steady_clock;
Clock::time_point g_phase_start = Clock::now();
Clock::time_point g_start = Clock::now();

int SecondsSince(Clock::time_point t) {
  return (int)std::chrono::duration_cast<std::chrono::seconds>(Clock::now() - t)
      .count();
}

}  // namespace

void BeginPhase(const std::string& phase, size_t total) {
  std::lock_guard<std::mutex> lk(g_mu);
  g_phase = phase;
  g_done = 0;
  g_total = total;
  g_current.clear();
  g_phase_start = Clock::now();
}

void SetCurrent(const std::string& item) {
  std::lock_guard<std::mutex> lk(g_mu);
  g_current[std::this_thread::get_id()] = item;
}

void Complete() {
  g_done.fetch_add(1);
  std::lock_guard<std::mutex> lk(g_mu);
  g_current.erase(std::this_thread::get_id());
}

void SetReady(bool ready) { g_ready = ready; }

Snapshot Get() {
  Snapshot s;
  s.done = g_done.load();
  s.total = g_total.load();
  s.ready = g_ready.load();
  std::lock_guard<std::mutex> lk(g_mu);
  s.phase = g_phase;
  s.phase_seconds = SecondsSince(g_phase_start);
  s.total_seconds = SecondsSince(g_start);
  for (const auto& kv : g_current) s.current.push_back(kv.second);
  return s;
}

}  // namespace progress
}  // namespace xexmagic
