// knowledge.h -- the knowledge base's database (codebreaker/knowledge/store.py): every platform's
// knowledge keyed by platform, one SQLite file beside the program databases, and the digests
// knowledge/reads.py gives the engine for a piece a step read.  A value is a tag and bytes, as a
// fact's; what a value IS (its decoding, the tables decoded once per process) stays with the
// Python that asks.
#pragma once
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

#include "db.h"

namespace cbe {

class KnowledgeBase {
 public:
  explicit KnowledgeBase(const std::string& path);
  std::string path;
  void call(const std::string& method, Reader& args, Writer& out);
  std::string digest(const std::string& read);          // reads.digest, once the kind is brought up to date
  void close();

 private:
  std::unique_ptr<Conn> conn_;
  std::recursive_mutex mu_;
  std::map<std::pair<std::string, std::string>, std::map<std::string, std::string>> hashes_;
  int64_t version_ = -1;

  std::map<std::string, std::string>& hashes(const std::string& platform, const std::string& kind);
  bool kind_digest(const std::string& platform, const std::string& kind, std::string& out);
  bool name(const std::string& body, Row& out);          // (name, where it is from)
  std::vector<std::pair<std::string, std::pair<Field, Field>>> names_from_bodies(const std::string& module);
};

std::shared_ptr<KnowledgeBase> open_kb(const std::string& path);
void close_kb(const std::string& path);

}  // namespace cbe
