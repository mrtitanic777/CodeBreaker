// XEXMagic - compatibility shim for the ported PPC decoder.
//
// The decoder tables come from Nexia unmodified, so they are re-syncable. They
// expect xe::StringBuffer with fmt-style AppendFormat; this provides exactly
// the three methods the generated disassembler uses, over std::string.
#ifndef XEXMAGIC_PPC_COMPAT_STRING_BUFFER_H_
#define XEXMAGIC_PPC_COMPAT_STRING_BUFFER_H_

#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <string>
#include <string_view>

namespace xe {

class StringBuffer {
 public:
  StringBuffer() = default;

  void Append(char c) { s_ += c; }
  void Append(const char* v) { s_ += v; }
  void Append(std::string_view v) { s_.append(v.data(), v.size()); }
  void AppendBytes(const uint8_t* b, size_t n) {
    s_.append(reinterpret_cast<const char*>(b), n);
  }

  size_t length() const { return s_.size(); }
  const std::string& to_string() const { return s_; }
  std::string ToString() const { return s_; }
  void Reset() { s_.clear(); }

  // The generated disassembler only ever uses "{}" and "0x{:X}", so the
  // formatter handles those two and passes anything else through literally
  // rather than pretending to be a full fmt implementation.
  template <typename... Args>
  void AppendFormat(const char* format, Args... args) {
    FormatInto(format, args...);
  }

 private:
  void FormatInto(const char* f) { s_ += f; }

  template <typename T, typename... Rest>
  void FormatInto(const char* f, T value, Rest... rest) {
    const char* open = strchr(f, '{');
    if (!open) {
      s_ += f;
      return;
    }
    s_.append(f, open - f);
    const char* close = strchr(open, '}');
    if (!close) {
      s_ += open;
      return;
    }
    std::string spec(open + 1, close - open - 1);
    AppendValue(value, spec);
    FormatInto(close + 1, rest...);
  }

  void AppendValue(const char* v, const std::string&) { s_ += v; }
  void AppendValue(const std::string& v, const std::string&) { s_ += v; }
  void AppendValue(char v, const std::string&) { s_ += v; }

  template <typename T>
  void AppendValue(T v, const std::string& spec) {
    char buf[32];
    if (spec == ":X")
      snprintf(buf, sizeof(buf), "%llX", (unsigned long long)(uint64_t)v);
    else if (spec == ":x")
      snprintf(buf, sizeof(buf), "%llx", (unsigned long long)(uint64_t)v);
    else if (std::is_signed<T>::value)
      snprintf(buf, sizeof(buf), "%lld", (long long)v);
    else
      snprintf(buf, sizeof(buf), "%llu", (unsigned long long)(uint64_t)v);
    s_ += buf;
  }

  std::string s_;
};

}  // namespace xe

#endif  // XEXMAGIC_PPC_COMPAT_STRING_BUFFER_H_
