// XEXMagic - TU delta-patch (.xexp) applier.
// Mirrors Xenia XexModule::ApplyPatch: applies an XEX2 delta patch onto a base
// XEX to produce the patched, decrypted, flat PE image (VA == file offset from
// base). Reuses XexLoader (base decrypt/decompress + session key) and lzx.cc
// (lzxdelta_apply_patch). AES-CBC via rijndael, SHA1 via TinySHA1.

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <vector>

#include "byte_order.h"
#include "xex_info.h"
#include "xex_loader.h"
#include "lzx.h"
#include "../third_party/crypto/TinySHA1.hpp"
#include "../third_party/crypto/rijndael-alg-fst.h"

using namespace xe;

static constexpr uint8_t kRetailKey[16] = {
    0x20, 0xB1, 0x85, 0xA5, 0x9D, 0x28, 0xFD, 0xC3,
    0x40, 0x58, 0x3F, 0xBB, 0x08, 0x96, 0xBF, 0x91};

static void aes_cbc_decrypt(const uint8_t* key, const uint8_t* in, size_t n,
                            uint8_t* out) {
  uint32_t rk[4 * (MAXNR + 1)];
  uint8_t ivec[16] = {0};
  int Nr = rijndaelKeySetupDec(rk, key, 128);
  const uint8_t* ct = in;
  uint8_t* pt = out;
  for (size_t k = 0; k < n; k += 16, ct += 16, pt += 16) {
    rijndaelDecrypt(rk, Nr, ct, pt);
    for (int i = 0; i < 16; i++) {
      pt[i] ^= ivec[i];
      ivec[i] = ct[i];
    }
  }
}

static inline uint32_t load_be(const uint8_t* p) {
  return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) |
         ((uint32_t)p[2] << 8) | (uint32_t)p[3];
}

static std::vector<uint8_t> read_file(const char* p) {
  std::vector<uint8_t> v;
  FILE* f = fopen(p, "rb");
  if (!f) return v;
  fseek(f, 0, SEEK_END);
  long sz = ftell(f);
  fseek(f, 0, SEEK_SET);
  v.resize(sz);
  fread(v.data(), 1, sz, f);
  fclose(f);
  return v;
}

// Locate an opt header value (offset or inline) in a raw xex header block.
static bool find_opt(const uint8_t* hdr, uint32_t key, uint32_t* out_val) {
  auto* h = reinterpret_cast<const xex2_header*>(hdr);
  uint32_t count = h->header_count;
  for (uint32_t i = 0; i < count; i++) {
    uint32_t k = load_be((const uint8_t*)hdr + 0x18 + i * 8);
    uint32_t v = load_be((const uint8_t*)hdr + 0x18 + i * 8 + 4);
    if (k == key) {
      *out_val = v;
      return true;
    }
  }
  return false;
}

int ApplyXexPatchCLI(const char* base_path, const char* patch_path,
                     const char* out_path) {
  XexLoader base;
  if (!base.LoadFromFile(base_path)) {
    fprintf(stderr, "apply-patch: failed to load base %s\n", base_path);
    return 1;
  }
  std::vector<uint8_t> patch = read_file(patch_path);
  if (patch.empty()) {
    fprintf(stderr, "apply-patch: failed to read patch %s\n", patch_path);
    return 1;
  }
  const uint8_t* ph = patch.data();
  auto* phdr = reinterpret_cast<const xex2_header*>(ph);
  uint32_t patch_header_size = phdr->header_size;
  uint32_t patch_sec_off = load_be((const uint8_t*)ph + 0x10);  // security_offset
  const uint8_t* patch_aes_key = ph + patch_sec_off + 0x150;  // xex2_security_info.aes_key @0x150

  uint32_t dpd_off = 0, ffi_off = 0;
  if (!find_opt(ph, XEX_HEADER_DELTA_PATCH_DESCRIPTOR, &dpd_off) ||
      !find_opt(ph, XEX_HEADER_FILE_FORMAT_INFO, &ffi_off)) {
    fprintf(stderr, "apply-patch: patch missing delta/fileformat opt headers\n");
    return 1;
  }
  auto* dpd = reinterpret_cast<const xex2_opt_delta_patch_descriptor*>(ph + dpd_off);
  auto* ffi = reinterpret_cast<const xex2_opt_file_format_info*>(ph + ffi_off);
  uint32_t window = ffi->compression_info.normal.window_size;

  fprintf(stderr,
          "apply-patch: dpd_off=%u ffi_off=%u window=%u enc=%d comp=%d\n"
          "  base header mem=%zu image=%zu sec_off=%u\n"
          "  first_block.block_size=%u body_len=%zu\n",
          dpd_off, ffi_off, window, (int)ffi->encryption_type,
          (int)ffi->compression_type, base.MutableHeaderMem().size(),
          base.MutableImage().size(), patch_sec_off,
          (uint32_t)ffi->compression_info.normal.first_block.block_size,
          patch.size() - patch_header_size);

  // ---- Header patch onto base xex header (updates base aes_key) ----
  std::vector<uint8_t>& bhdr = base.MutableHeaderMem();
  uint32_t target_hdr_size = dpd->size_of_target_headers;
  // Give lzxdelta headroom so an over-shooting write can't corrupt the heap.
  size_t hdr_cap = target_hdr_size ? target_hdr_size : bhdr.size();
  if (hdr_cap < bhdr.size()) hdr_cap = bhdr.size();
  bhdr.resize(hdr_cap + 0x10000, 0);
  if (dpd->delta_headers_source_offset) {
    memmove(bhdr.data() + dpd->delta_headers_target_offset,
            bhdr.data() + dpd->delta_headers_source_offset,
            dpd->delta_headers_source_size);
  }
  {
    int rc = lzxdelta_apply_patch(
        const_cast<xex2_delta_patch*>(&dpd->info), dpd->size, window,
        bhdr.data());
    fprintf(stderr, "apply-patch: header patch rc=%d\n", rc);
    if (rc) return 1;
  }
  bhdr.resize(hdr_cap);

  // Re-derive base session key from the PATCHED base header's aes_key.
  uint32_t base_sec_off = load_be(bhdr.data() + 0x10);
  fprintf(stderr, "apply-patch: patched base_sec_off=%u hdr=%zu\n", base_sec_off,
          bhdr.size());
  if (base_sec_off + 0x160 > bhdr.size()) {
    fprintf(stderr, "apply-patch: base_sec_off out of range\n");
    return 1;
  }
  const uint8_t* base_aes_key = bhdr.data() + base_sec_off + 0x150;
  uint8_t base_session_key[16];
  aes_cbc_decrypt(kRetailKey, base_aes_key, 16, base_session_key);

  // ---- Derive patch session key from the re-derived base key ----
  uint8_t patch_session_key[16];
  aes_cbc_decrypt(base_session_key, patch_aes_key, 16, patch_session_key);

  // ---- Decrypt patch body ----
  uint32_t body_len = (uint32_t)(patch.size() - patch_header_size);
  const uint8_t* body_enc = ph + patch_header_size;
  std::vector<uint8_t> body(body_len);
  if (ffi->encryption_type == XEX_ENCRYPTION_NORMAL) {
    aes_cbc_decrypt(patch_session_key, body_enc, body_len, body.data());
  } else {
    memcpy(body.data(), body_enc, body_len);
  }

  // ---- Prepare base image (grow to the PATCHED image_size) ----
  std::vector<uint8_t>& img = base.MutableImage();
  // Patched security info: image_size lives at security_offset + 4.
  uint32_t patched_image_size = load_be(bhdr.data() + base_sec_off + 4);
  uint32_t image_target_size = dpd->delta_image_target_offset +
                               dpd->delta_image_source_size;
  if (patched_image_size > image_target_size) image_target_size = patched_image_size;
  fprintf(stderr, "apply-patch: patched image_size=%u target=%u base=%zu\n",
          patched_image_size, image_target_size, img.size());
  if (image_target_size > img.size()) img.resize(image_target_size, 0);
  uint8_t* base_exe = img.data();
  if (dpd->delta_image_source_offset) {
    memmove(base_exe + dpd->delta_image_target_offset,
            base_exe + dpd->delta_image_source_offset,
            dpd->delta_image_source_size);
  }

  // ---- Walk compression blocks, lzxdelta each onto the image ----
  const xex2_compressed_block_info* cur = &ffi->compression_info.normal.first_block;
  const uint8_t* p = body.data();
  const uint8_t* end = body.data() + body.size();
  sha1::SHA1 s;
  uint8_t digest[20];
  int blocks = 0;
  while (cur->block_size && p < end) {
    const auto* next = reinterpret_cast<const xex2_compressed_block_info*>(p);
    uint32_t bs = cur->block_size;
    if (p + bs > end) bs = (uint32_t)(end - p);
    s.reset();
    s.processBytes(p, bs);
    s.finalize(digest);
    if (memcmp(digest, cur->block_hash, 20) != 0) {
      fprintf(stderr,
              "apply-patch: block %d SHA mismatch (wrong key?) bs=%u\n",
              blocks, (uint32_t)cur->block_size);
      return 1;
    }
    fprintf(stderr, "apply-patch: block %d SHA OK bs=%u\n", blocks,
            (uint32_t)cur->block_size);
    p += 20 + 4;
    uint32_t block_data_size = cur->block_size - 20 - 4;
    if (blocks == 0) {
      auto* e = (const xex2_delta_patch*)p;
      fprintf(stderr,
              "apply-patch: first delta entry old=%u new=%u unc=%u comp=%u\n",
              (uint32_t)e->old_addr, (uint32_t)e->new_addr,
              (uint32_t)e->uncompressed_len, (uint32_t)e->compressed_len);
    }
    int rc = lzxdelta_apply_patch((xex2_delta_patch*)p, block_data_size, window,
                                  base_exe);
    if (rc) {
      fprintf(stderr, "apply-patch: lzxdelta block %d failed rc=%d\n", blocks, rc);
      return 1;
    }
    p += block_data_size;
    cur = next;
    blocks++;
  }

  FILE* fo = fopen(out_path, "wb");
  if (!fo) {
    fprintf(stderr, "apply-patch: cannot open output %s\n", out_path);
    return 1;
  }
  fwrite(img.data(), 1, img.size(), fo);
  fclose(fo);
  fprintf(stderr,
          "apply-patch OK: %d blocks, image %zu bytes -> %s\n", blocks,
          img.size(), out_path);
  return 0;
}
