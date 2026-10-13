// XEXMagic - Standalone XEX decompressor/decompiler
// Adapted from Xenia (BSD license) - xex_module.cc

#include "xex_loader.h"

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "byte_order.h"
#include "lzx.h"
#include "string_buffer.h"

#include "../third_party/crypto/TinySHA1.hpp"
#include "../third_party/crypto/rijndael-alg-fst.h"

static constexpr uint8_t xe_xex2_retail_key[16] = {
    0x20, 0xB1, 0x85, 0xA5, 0x9D, 0x28, 0xFD, 0xC3,
    0x40, 0x58, 0x3F, 0xBB, 0x08, 0x96, 0xBF, 0x91};
static constexpr uint8_t xe_xex2_devkit_key[16] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static constexpr uint32_t kXEX1Signature = 0x58455831;  // 'XEX1'
static constexpr uint32_t kXEX2Signature = 0x58455832;  // 'XEX2'

static void aes_decrypt_buffer(const uint8_t* session_key,
                               const uint8_t* input_buffer,
                               const size_t input_size,
                               uint8_t* output_buffer,
                               const size_t output_size) {
  uint32_t rk[4 * (MAXNR + 1)];
  uint8_t ivec[16] = {0};
  int32_t Nr = rijndaelKeySetupDec(rk, session_key, 128);
  const uint8_t* ct = input_buffer;
  uint8_t* pt = output_buffer;
  for (size_t n = 0; n < input_size; n += 16, ct += 16, pt += 16) {
    rijndaelDecrypt(rk, Nr, ct, pt);
    for (size_t i = 0; i < 16; i++) {
      pt[i] ^= ivec[i];
      ivec[i] = ct[i];
    }
  }
}

XexLoader::XexLoader() {
  std::memset(session_key_, 0, sizeof(session_key_));
  std::memset(&security_info_, 0, sizeof(security_info_));
}

XexLoader::~XexLoader() {}

void XexLoader::ReDeriveSessionKeyFromHeader() {
  ReadSecurityInfo();
  aes_decrypt_buffer(
      is_dev_kit_ ? xe_xex2_devkit_key : xe_xex2_retail_key,
      reinterpret_cast<const uint8_t*>(security_info_.aes_key), 16,
      session_key_, 16);
}

bool XexLoader::LoadFromFile(const char* path) {
  FILE* f = fopen(path, "rb");
  if (!f) {
    fprintf(stderr, "Failed to open file: %s\n", path);
    return false;
  }

  fseek(f, 0, SEEK_END);
  size_t file_size = ftell(f);
  fseek(f, 0, SEEK_SET);

  std::vector<uint8_t> file_data(file_size);
  if (fread(file_data.data(), 1, file_size, f) != file_size) {
    fclose(f);
    fprintf(stderr, "Failed to read file: %s\n", path);
    return false;
  }
  fclose(f);

  raw_xex_data_ = file_data;
  return LoadFromMemory(file_data.data(), file_data.size());
}

bool XexLoader::LoadFromMemory(const void* xex_addr, size_t xex_length) {
  auto src_header = reinterpret_cast<const xex2_header*>(xex_addr);

  uint32_t magic = src_header->magic;
  if (magic == kXEX1Signature) {
    xex_format_ = kFormatXex1;
  } else if (magic == kXEX2Signature) {
    xex_format_ = kFormatXex2;
  } else {
    fprintf(stderr, "Unknown XEX magic: 0x%08X\n", magic);
    return false;
  }

  // Read in XEX headers
  uint32_t header_size = src_header->header_size;
  if (header_size > xex_length) {
    fprintf(stderr, "XEX header_size (0x%X) exceeds file size (%zu)\n",
            header_size, xex_length);
    return false;
  }
  xex_header_mem_.resize(header_size);
  std::memcpy(xex_header_mem_.data(), src_header, header_size);

  // Read security info
  ReadSecurityInfo();

  // Determine base address
  if (!xex_security_info()) {
    fprintf(stderr, "Failed to read XEX security info\n");
    return false;
  }
  base_address_ = xex_security_info()->load_address;
  xe::be<uint32_t>* base_addr_opt = nullptr;
  if (GetOptHeader(XEX_HEADER_IMAGE_BASE_ADDRESS, &base_addr_opt)) {
    base_address_ = *base_addr_opt;
  }

  // Load the XEX image - try retail key first, then devkit
  int result_code = ReadImage(xex_addr, xex_length, false);
  if (result_code) {
    fprintf(stderr,
            "XEX load failed with code %d, trying devkit key...\n",
            result_code);
    result_code = ReadImage(xex_addr, xex_length, true);
    if (result_code) {
      fprintf(stderr,
              "XEX load failed with code %d, tried both keys\n",
              result_code);
      return false;
    }
  }

  return true;
}

bool XexLoader::GetOptHeader(const xex2_header* header, xex2_header_keys key,
                             void** out_ptr) {
  assert(header);
  assert(out_ptr);

  // Bound header_count to what actually fits in the header buffer
  uint32_t max_headers = header->header_count;
  if (header->header_size > sizeof(xex2_header)) {
    uint32_t available = (header->header_size - sizeof(xex2_header)) / sizeof(xex2_opt_header);
    if (max_headers > available) max_headers = available;
  } else {
    max_headers = 0;
  }
  for (uint32_t i = 0; i < max_headers; i++) {
    const xex2_opt_header& opt_header = header->headers[i];
    if (opt_header.key == key) {
      switch (key & 0xFF) {
        case 0x00: {
          // Return the value directly (assume out_ptr points to uint32_t)
          *reinterpret_cast<uint32_t*>(out_ptr) =
              static_cast<uint32_t>(opt_header.value);
        } break;
        case 0x01: {
          // Pointer to the value on the optional header
          *out_ptr = const_cast<void*>(
              reinterpret_cast<const void*>(&opt_header.value));
        } break;
        default: {
          // Pointer to the header data — bounds check the offset
          uint32_t off = opt_header.offset;
          if (off >= header->header_size) {
            *out_ptr = nullptr;
            return false;
          }
          *out_ptr =
              reinterpret_cast<void*>(uintptr_t(header) + off);
        } break;
      }
      return true;
    }
  }

  return false;
}

bool XexLoader::GetOptHeader(xex2_header_keys key, void** out_ptr) const {
  return XexLoader::GetOptHeader(xex_header(), key, out_ptr);
}

const void* XexLoader::GetSecurityInfo(const xex2_header* header) {
  uint32_t sec_off = header->security_offset;
  if (sec_off >= header->header_size) {
    fprintf(stderr, "Security info offset 0x%X beyond header size 0x%X\n",
            sec_off, (uint32_t)header->header_size);
    return nullptr;
  }
  return reinterpret_cast<const void*>(uintptr_t(header) + sec_off);
}

void XexLoader::ReadSecurityInfo() {
  if (xex_format_ == kFormatXex1) {
    const void* raw_sec = GetSecurityInfo(xex_header());
    if (!raw_sec) return;
    const xex1_security_info* xex1_sec_info =
        reinterpret_cast<const xex1_security_info*>(raw_sec);

    security_info_.rsa_signature = xex1_sec_info->rsa_signature;
    security_info_.aes_key = xex1_sec_info->aes_key;
    security_info_.image_size = xex1_sec_info->image_size;
    security_info_.image_flags = xex1_sec_info->image_flags;
    security_info_.export_table = xex1_sec_info->export_table;
    security_info_.load_address = xex1_sec_info->load_address;
    security_info_.page_descriptor_count = xex1_sec_info->page_descriptor_count;
    security_info_.page_descriptors = xex1_sec_info->page_descriptors;
  } else if (xex_format_ == kFormatXex2) {
    const void* raw_sec = GetSecurityInfo(xex_header());
    if (!raw_sec) return;
    const xex2_security_info* xex2_sec_info =
        reinterpret_cast<const xex2_security_info*>(raw_sec);

    security_info_.rsa_signature = xex2_sec_info->rsa_signature;
    security_info_.aes_key = xex2_sec_info->aes_key;
    security_info_.image_size = xex2_sec_info->image_size;
    security_info_.image_flags = xex2_sec_info->image_flags;
    security_info_.export_table = xex2_sec_info->export_table;
    security_info_.load_address = xex2_sec_info->load_address;
    security_info_.page_descriptor_count = xex2_sec_info->page_descriptor_count;
    security_info_.page_descriptors = xex2_sec_info->page_descriptors;
  }
}

uint32_t XexLoader::image_size() const {
  uint32_t total_size = 0;
  uint32_t ps = page_size();
  auto* sec = xex_security_info();
  if (!sec->page_descriptors || sec->page_descriptor_count > 0x10000)
    return 0;
  for (uint32_t i = 0; i < sec->page_descriptor_count; i++) {
    xex2_page_descriptor desc;
    desc.value = xe::byte_swap(sec->page_descriptors[i].value);
    total_size += desc.page_count * ps;
  }
  return total_size;
}

int XexLoader::ReadImage(const void* xex_addr, size_t xex_length,
                         bool use_dev_key) {
  if (!opt_file_format_info()) {
    return 1;
  }

  is_dev_kit_ = use_dev_key;

  aes_decrypt_buffer(
      use_dev_key ? xe_xex2_devkit_key : xe_xex2_retail_key,
      reinterpret_cast<const uint8_t*>(xex_security_info()->aes_key), 16,
      session_key_, 16);

  int result_code = 0;
  switch (opt_file_format_info()->compression_type) {
    case XEX_COMPRESSION_NONE:
      result_code = ReadImageUncompressed(xex_addr, xex_length);
      break;
    case XEX_COMPRESSION_BASIC:
      result_code = ReadImageBasicCompressed(xex_addr, xex_length);
      break;
    case XEX_COMPRESSION_NORMAL:
      result_code = ReadImageCompressed(xex_addr, xex_length);
      break;
    default:
      assert(false);
      return 2;
  }

  if (result_code) {
    return result_code;
  }

  // Verify PE signature (MZ = 0x5A4D in LE, stored as 0x905A4D with PE flag)
  if (decompressed_image_.size() >= 4) {
    uint32_t pe_sig = *reinterpret_cast<const uint32_t*>(
        decompressed_image_.data());
    if (pe_sig == 0x905A4D) {
      return 0;  // Valid PE
    }
  }

  // Image doesn't have proper PE header
  return 3;
}

int XexLoader::ReadImageUncompressed(const void* xex_addr, size_t xex_length) {
  const uint32_t exe_length =
      static_cast<uint32_t>(xex_length - xex_header()->header_size);
  const uint8_t* p =
      (const uint8_t*)xex_addr + xex_header()->header_size;

  uint32_t uncompressed_size = exe_length;
  decompressed_image_.resize(uncompressed_size, 0);

  switch (opt_file_format_info()->encryption_type) {
    case XEX_ENCRYPTION_NONE:
      if (exe_length > uncompressed_size) {
        return 1;
      }
      std::memcpy(decompressed_image_.data(), p, exe_length);
      return 0;
    case XEX_ENCRYPTION_NORMAL:
      aes_decrypt_buffer(session_key_, p, exe_length,
                         decompressed_image_.data(), uncompressed_size);
      return 0;
    default:
      assert(false);
      return 1;
  }

  return 0;
}

int XexLoader::ReadImageBasicCompressed(const void* xex_addr,
                                        size_t xex_length) {
  const uint32_t exe_length =
      static_cast<uint32_t>(xex_length - xex_header()->header_size);
  const uint8_t* source_buffer =
      (const uint8_t*)xex_addr + xex_header()->header_size;
  const uint8_t* p = source_buffer;

  // Calculate uncompressed length
  uint32_t uncompressed_size = 0;
  auto* file_info = opt_file_format_info();
  auto& comp_info = file_info->compression_info.basic;

  uint32_t block_count = (file_info->info_size - 8) / 8;
  for (uint32_t n = 0; n < block_count; n++) {
    const uint32_t data_size = comp_info.blocks[n].data_size;
    const uint32_t zero_size = comp_info.blocks[n].zero_size;
    uncompressed_size += data_size + zero_size;
  }

  // Calculate total size from page descriptors
  uint32_t total_size = image_size();

  decompressed_image_.resize(total_size, 0);
  uint8_t* d = decompressed_image_.data();

  uint32_t rk[4 * (MAXNR + 1)];
  uint8_t ivec[16] = {0};
  int32_t Nr = rijndaelKeySetupDec(rk, session_key_, 128);

  for (size_t n = 0; n < block_count; n++) {
    const uint32_t data_size = comp_info.blocks[n].data_size;
    const uint32_t zero_size = comp_info.blocks[n].zero_size;

    switch (opt_file_format_info()->encryption_type) {
      case XEX_ENCRYPTION_NONE:
        if (data_size > uncompressed_size -
                            static_cast<uint32_t>(d - decompressed_image_.data())) {
          return 1;
        }
        std::memcpy(d, p, data_size);
        break;
      case XEX_ENCRYPTION_NORMAL: {
        const uint8_t* ct = p;
        uint8_t* pt = d;
        for (size_t m = 0; m < data_size; m += 16, ct += 16, pt += 16) {
          rijndaelDecrypt(rk, Nr, ct, pt);
          for (size_t i = 0; i < 16; i++) {
            pt[i] ^= ivec[i];
            ivec[i] = ct[i];
          }
        }
      } break;
      default:
        assert(false);
        return 1;
    }

    p += data_size;
    d += data_size + zero_size;
  }

  return 0;
}

int XexLoader::ReadImageCompressed(const void* xex_addr, size_t xex_length) {
  const uint32_t exe_length =
      static_cast<uint32_t>(xex_length - xex_header()->header_size);
  const uint8_t* exe_buffer =
      (const uint8_t*)xex_addr + xex_header()->header_size;

  // Decrypt (if needed)
  bool free_input = false;
  const uint8_t* input_buffer = exe_buffer;
  size_t input_size = exe_length;

  switch (opt_file_format_info()->encryption_type) {
    case XEX_ENCRYPTION_NONE:
      break;
    case XEX_ENCRYPTION_NORMAL:
      free_input = true;
      input_buffer = (const uint8_t*)calloc(1, exe_length);
      if (!input_buffer) {
        fprintf(stderr, "Failed to allocate %u bytes for decryption\n",
                exe_length);
        return 1;
      }
      aes_decrypt_buffer(session_key_, exe_buffer, exe_length,
                         (uint8_t*)input_buffer, exe_length);
      break;
    default:
      assert(false);
      return 1;
  }

  const auto* compression_info = &opt_file_format_info()->compression_info;
  const xex2_compressed_block_info* cur_block =
      &compression_info->normal.first_block;

  uint8_t* compress_buffer = (uint8_t*)calloc(1, exe_length);
  if (!compress_buffer) {
    fprintf(stderr, "Failed to allocate %u bytes for decompression\n",
            exe_length);
    if (free_input) free((void*)input_buffer);
    return 1;
  }

  const uint8_t* p = input_buffer;
  uint8_t* d = compress_buffer;
  const uint8_t* input_end = input_buffer + input_size;
  uint8_t* compress_end = compress_buffer + exe_length;

  // De-block
  int result_code = 0;
  sha1::SHA1 s;

  uint8_t block_calced_digest[0x14];
  while (cur_block->block_size) {
    const uint8_t* pnext = p + cur_block->block_size;
    const auto* next_block = (const xex2_compressed_block_info*)p;

    // Bounds check
    if (pnext > input_end || p + 24 > input_end) {
      result_code = 3;
      break;
    }

    // Compare block hash
    s.reset();
    s.processBytes(p, cur_block->block_size);
    s.finalize(block_calced_digest);
    if (memcmp(block_calced_digest, cur_block->block_hash, 0x14) != 0) {
      result_code = 2;
      break;
    }

    // Skip block info (size + hash)
    p += 4;
    p += 20;

    while (true) {
      if (p + 2 > input_end) { result_code = 3; break; }
      const size_t chunk_size = (p[0] << 8) | p[1];
      p += 2;
      if (!chunk_size) {
        break;
      }
      if (p + chunk_size > input_end || d + chunk_size > compress_end) {
        result_code = 3;
        break;
      }
      std::memcpy(d, p, chunk_size);
      p += chunk_size;
      d += chunk_size;
    }
    if (result_code) break;

    p = pnext;
    cur_block = next_block;
  }

  if (!result_code) {
    uint32_t uncompressed_size = image_size();
    decompressed_image_.resize(uncompressed_size, 0);

    // LZX decompress
    result_code = lzx_decompress(
        compress_buffer, d - compress_buffer,
        decompressed_image_.data(), uncompressed_size,
        compression_info->normal.window_size, nullptr, 0);
  }

  if (compress_buffer) {
    free(compress_buffer);
  }
  if (free_input) {
    free((void*)input_buffer);
  }
  return result_code;
}

bool XexLoader::ParseImports() {
  xex2_opt_import_libraries* opt_import_libraries = nullptr;
  GetOptHeader(XEX_HEADER_IMPORT_LIBRARIES, &opt_import_libraries);

  if (!opt_import_libraries) {
    return true;  // No imports is OK
  }

  // Parse string table (max 32 entries)
  const char* string_table[32];
  std::memset(string_table, 0, sizeof(string_table));

  for (size_t i = 0, o = 0;
       i < opt_import_libraries->string_table.size &&
       o < opt_import_libraries->string_table.count;
       ++o) {
    assert(o < 32);
    const char* str = &opt_import_libraries->string_table.data[i];
    string_table[o] = str;
    i += std::strlen(str) + 1;
    // Padding to 4-byte boundary
    if ((i % 4) != 0) {
      i += 4 - (i % 4);
    }
  }

  auto library_data = reinterpret_cast<uint8_t*>(opt_import_libraries);
  uint32_t library_offset = opt_import_libraries->string_table.size + 12;

  while (library_offset < opt_import_libraries->size) {
    auto library =
        reinterpret_cast<xex2_import_library*>(library_data + library_offset);
    if (!library->size) {
      break;
    }

    size_t library_name_index = library->name_index & 0xFF;
    assert(library_name_index < opt_import_libraries->string_table.count);

    ImportLibrary library_info;
    library_info.name = string_table[library_name_index]
                            ? string_table[library_name_index]
                            : "unknown";
    library_info.id = library->id;
    library_info.version.value = library->version().value;
    library_info.min_version.value = library->version_min().value;

    // Parse individual imports
    for (uint32_t i = 0; i < library->count; i++) {
      uint32_t record_addr = library->import_table[i];
      if (!record_addr) continue;

      // In standalone mode, we can't read from Xbox memory.
      // We record the import addresses for reference.
      // The record_addr is a virtual address into the decompressed image.
      uint32_t image_offset = record_addr - base_address_;
      if (decompressed_image_.size() >= 4 && image_offset < decompressed_image_.size() - 4) {
        uint32_t record_value = *reinterpret_cast<xe::be<uint32_t>*>(
            decompressed_image_.data() + image_offset);

        uint16_t record_type = (record_value & 0xFF000000) >> 24;
        uint16_t ordinal = record_value & 0xFFFF;

        ImportLibraryFn import_info;
        import_info.ordinal = ordinal;
        import_info.value_address = record_addr;
        import_info.thunk_address = 0;

        if (record_type == 1 && library_info.imports.size() > 0) {
          // Thunk - update previous import's thunk address
          auto& prev = library_info.imports.back();
          if (prev.ordinal == ordinal) {
            prev.thunk_address = record_addr;
            continue;  // Don't add duplicate
          }
        }

        library_info.imports.push_back(import_info);
      }
    }

    import_libs_.push_back(library_info);
    library_offset += library->size;
  }

  return true;
}

bool XexLoader::ParseExports() {
  // XEX2 export table is accessed via the security info export_table field
  if (!xex_security_info()->export_table) {
    return true;  // No exports is OK
  }

  // The export table address is a virtual address into the decompressed image
  uint32_t export_table_va = xex_security_info()->export_table;
  uint32_t image_offset = export_table_va - base_address_;

  if (image_offset >= decompressed_image_.size()) {
    return false;
  }

  // Export table is available at this offset in the decompressed image
  // Callers can access it directly if needed
  return true;
}
