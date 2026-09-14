// XEXMagic - Standalone XEX decompressor/decompiler
// Adapted from Xenia (BSD license) - xex_module.h/cc

#ifndef XEXMAGIC_XEX_LOADER_H_
#define XEXMAGIC_XEX_LOADER_H_

#include <cstdint>
#include <string>
#include <vector>

#include "byte_order.h"
#include "xex_info.h"

using namespace xe;

struct ImportLibraryFn {
  uint32_t ordinal;
  uint32_t value_address;
  uint32_t thunk_address;
};

struct ImportLibrary {
  std::string name;
  uint32_t id;
  xe_xex2_version_t version;
  xe_xex2_version_t min_version;
  std::vector<ImportLibraryFn> imports;
};

struct SecurityInfoContext {
  const char* rsa_signature;
  const char* aes_key;
  uint32_t image_size;
  uint32_t image_flags;
  uint32_t export_table;
  uint32_t load_address;
  uint32_t page_descriptor_count;
  const xex2_page_descriptor* page_descriptors;
};

class XexLoader {
 public:
  enum XexFormat {
    kFormatUnknown,
    kFormatXex1,
    kFormatXex2,
  };

  XexLoader();
  ~XexLoader();

  // Load XEX from file path
  bool LoadFromFile(const char* path);

  // Load XEX from memory buffer
  bool LoadFromMemory(const void* xex_addr, size_t xex_length);

  // Get optional header by key
  // Special case: if key & 0xFF == 0x00, returns the value (not pointer)
  static bool GetOptHeader(const xex2_header* header, xex2_header_keys key,
                           void** out_ptr);
  bool GetOptHeader(xex2_header_keys key, void** out_ptr) const;

  template <typename T>
  static bool GetOptHeader(const xex2_header* header, xex2_header_keys key,
                           T* out_ptr) {
    return GetOptHeader(header, key, reinterpret_cast<void**>(out_ptr));
  }

  template <typename T>
  bool GetOptHeader(xex2_header_keys key, T* out_ptr) const {
    return GetOptHeader(key, reinterpret_cast<void**>(out_ptr));
  }

  // Get security info pointer from header
  static const void* GetSecurityInfo(const xex2_header* header);

  // Accessors
  const xex2_header* xex_header() const {
    return reinterpret_cast<const xex2_header*>(xex_header_mem_.data());
  }

  const SecurityInfoContext* xex_security_info() const {
    return &security_info_;
  }

  const xex2_opt_file_format_info* opt_file_format_info() const {
    xex2_opt_file_format_info* retval = nullptr;
    GetOptHeader(XEX_HEADER_FILE_FORMAT_INFO, &retval);
    return retval;
  }

  const xex2_opt_execution_info* opt_execution_info() const {
    xex2_opt_execution_info* retval = nullptr;
    GetOptHeader(XEX_HEADER_EXECUTION_INFO, &retval);
    return retval;
  }

  const std::vector<uint8_t>& GetDecompressedImage() const {
    return decompressed_image_;
  }

  // -- accessors used by the TU delta-patch applier -------------------------
  std::vector<uint8_t>& MutableImage() { return decompressed_image_; }
  std::vector<uint8_t>& MutableHeaderMem() { return xex_header_mem_; }
  const uint8_t* session_key() const { return session_key_; }
  const SecurityInfoContext* security_info() const { return &security_info_; }
  void ReDeriveSessionKeyFromHeader();  // after header patch mutates aes_key

  const std::vector<uint8_t>& GetRawXexData() const {
    return raw_xex_data_;
  }

  uint32_t GetBaseAddress() const { return base_address_; }

  uint32_t GetEntryPoint() const {
    // XEX_HEADER_ENTRY_POINT key type is 0x00: value stored directly in pointer
    void* val = nullptr;
    if (const_cast<XexLoader*>(this)->GetOptHeader(XEX_HEADER_ENTRY_POINT, &val)) {
      return static_cast<uint32_t>(reinterpret_cast<uintptr_t>(val));
    }
    return 0;
  }

  uint32_t GetTitleId() const {
    auto* exec_info = opt_execution_info();
    if (exec_info) {
      return exec_info->title_id;
    }
    return 0;
  }

  XexFormat format() const { return xex_format_; }
  bool is_dev_kit() const { return is_dev_kit_; }

  const std::vector<ImportLibrary>& import_libraries() const {
    return import_libs_;
  }

  // Page size depends on image flags
  uint32_t page_size() const {
    if (security_info_.image_flags & XEX_IMAGE_PAGE_SIZE_4KB) {
      return 4096;
    }
    return 64 * 1024;
  }

  // Calculate total image size from page descriptors
  uint32_t image_size() const;

  // Parse imports from the optional header
  bool ParseImports();

  // Parse exports
  bool ParseExports();

 private:
  void ReadSecurityInfo();

  int ReadImage(const void* xex_addr, size_t xex_length, bool use_dev_key);
  int ReadImageUncompressed(const void* xex_addr, size_t xex_length);
  int ReadImageBasicCompressed(const void* xex_addr, size_t xex_length);
  int ReadImageCompressed(const void* xex_addr, size_t xex_length);

  std::vector<uint8_t> xex_header_mem_;
  std::vector<uint8_t> decompressed_image_;
  std::vector<uint8_t> raw_xex_data_;

  std::vector<ImportLibrary> import_libs_;

  uint8_t session_key_[0x10];
  bool is_dev_kit_ = false;
  uint32_t base_address_ = 0;

  XexFormat xex_format_ = kFormatUnknown;
  SecurityInfoContext security_info_ = {};
};

#endif  // XEXMAGIC_XEX_LOADER_H_
