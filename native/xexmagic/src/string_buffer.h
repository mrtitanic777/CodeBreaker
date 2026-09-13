// XEXMagic - Standalone XEX decompressor/decompiler
// Adapted from Xenia (BSD license) - string_buffer.h
// Header-only minimal implementation

#ifndef XEXMAGIC_STRING_BUFFER_H_
#define XEXMAGIC_STRING_BUFFER_H_

#include <cassert>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <string_view>
#include <algorithm>

class StringBuffer {
 public:
  explicit StringBuffer(size_t initial_capacity = 0) {
    buffer_capacity_ = std::max(initial_capacity, static_cast<size_t>(16384));
    buffer_ = reinterpret_cast<char*>(std::malloc(buffer_capacity_));
    assert(buffer_);
    buffer_[0] = 0;
  }

  ~StringBuffer() {
    std::free(buffer_);
    buffer_ = nullptr;
  }

  // Non-copyable
  StringBuffer(const StringBuffer&) = delete;
  StringBuffer& operator=(const StringBuffer&) = delete;

  char* buffer() const { return buffer_; }
  size_t length() const { return buffer_offset_; }

  void Reset() {
    buffer_offset_ = 0;
    buffer_[0] = 0;
  }

  void Append(char c) {
    Grow(2);
    buffer_[buffer_offset_++] = c;
    buffer_[buffer_offset_] = 0;
  }

  void Append(const char* value) {
    if (!value) return;
    size_t len = std::strlen(value);
    Grow(len + 1);
    std::memcpy(buffer_ + buffer_offset_, value, len);
    buffer_offset_ += len;
    buffer_[buffer_offset_] = 0;
  }

  void Append(const std::string_view value) {
    Grow(value.size() + 1);
    std::memcpy(buffer_ + buffer_offset_, value.data(), value.size());
    buffer_offset_ += value.size();
    buffer_[buffer_offset_] = 0;
  }

  template <typename... Args>
  void AppendFormat(const char* format, Args... args) {
    int needed = std::snprintf(nullptr, 0, format, args...);
    if (needed <= 0) return;
    Grow(static_cast<size_t>(needed) + 1);
    std::snprintf(buffer_ + buffer_offset_,
                  buffer_capacity_ - buffer_offset_, format, args...);
    buffer_offset_ += static_cast<size_t>(needed);
    buffer_[buffer_offset_] = 0;
  }

  std::string ToString() const {
    return std::string(buffer_, buffer_offset_);
  }

  std::string to_string() const {
    return std::string(buffer_, buffer_offset_);
  }

  std::string_view to_string_view() const {
    return std::string_view(buffer_, buffer_offset_);
  }

 private:
  void Grow(size_t additional_length) {
    if (buffer_offset_ + additional_length <= buffer_capacity_) {
      return;
    }
    size_t new_capacity =
        std::max(buffer_offset_ + additional_length + 16384,
                 buffer_capacity_ * 2);
    auto new_buffer = std::realloc(buffer_, new_capacity);
    assert(new_buffer);
    buffer_ = reinterpret_cast<char*>(new_buffer);
    buffer_capacity_ = new_capacity;
  }

  char* buffer_ = nullptr;
  size_t buffer_offset_ = 0;
  size_t buffer_capacity_ = 0;
};

#endif  // XEXMAGIC_STRING_BUFFER_H_
