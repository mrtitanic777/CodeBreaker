// XEXMagic - xboxkrnl import trampolines. See kernel_exports.h.

#include "kernel_exports.h"

#include <map>

namespace xexmagic {
namespace {

struct ExportName {
  uint32_t ordinal;
  const char* name;
};

const ExportName kExports[] = {
#define KX(o, n) {o, n},
#include "kernel_export_table.inc"
#undef KX
};

// A guest heap for the allocating exports. The interpreter's memory is sparse
// and zero-filled, so any address that the image does not already occupy is
// usable; this sits well above the 0x82000000 image and below the default
// stack at 0x70000000 seen from the guest's unsigned view.
constexpr uint32_t kHeapBase = 0x90000000;
constexpr uint32_t kHeapEnd = 0x9F000000;
uint32_t g_heap_next = kHeapBase;

// TLS slots. One thread here, so a flat array is the entire model.
constexpr uint32_t kTlsSlots = 64;
uint32_t g_tls[kTlsSlots] = {};
uint32_t g_tls_next = 1;   // slot 0 reserved so a 0 return reads as failure

uint32_t HeapAlloc(uint32_t size, uint32_t align) {
  if (align < 16) align = 16;
  g_heap_next = (g_heap_next + align - 1) & ~(align - 1);
  uint32_t p = g_heap_next;
  uint32_t n = (size + 15) & ~15u;
  if (p + n >= kHeapEnd) return 0;   // out of scratch: caller sees a NULL
  g_heap_next = p + n;
  return p;
}

}  // namespace

bool IsImportThunk(uint32_t w0, uint32_t w1, uint32_t w2, uint32_t w3) {
  // 0x7D6903A6 = mtctr r11, 0x4E800420 = bctr. Both record words carry the
  // same ordinal; requiring types 1 and 2 in order keeps this from matching
  // ordinary code that happens to start with a 0x01 byte.
  return (w0 >> 24) == 0x01 && (w1 >> 24) == 0x02 &&
         (w0 & 0xFFFF) == (w1 & 0xFFFF) && w2 == 0x7D6903A6 &&
         w3 == 0x4E800420;
}

const char* KernelExportName(uint32_t ordinal) {
  static std::map<uint32_t, const char*>* m = [] {
    auto* t = new std::map<uint32_t, const char*>();
    for (const auto& e : kExports) (*t)[e.ordinal] = e.name;
    return t;
  }();
  auto it = m->find(ordinal);
  return it == m->end() ? "" : it->second;
}

KernelCallResult KernelInvoke(uint32_t ordinal, uint32_t* gpr, KernelMem& mem) {
  KernelCallResult r;
  r.implemented = true;
  const uint32_t r3 = gpr[3], r4 = gpr[4], r5 = gpr[5];

  switch (ordinal) {
    // ---- waits and events -------------------------------------------------
    // Nothing else runs in this harness, so a wait is always already
    // satisfied. Returning WAIT_OBJECT_0 rather than a timeout matters: the
    // fastfile driver spins re-waiting on a timeout.
    case 0x00FD:  // NtWaitForSingleObjectEx
    case 0x00FE:  // NtWaitForMultipleObjectsEx
    case 0x00FB:  // NtSignalAndWaitForSingleObjectEx
      r.r3 = 0;  // STATUS_SUCCESS / WAIT_OBJECT_0
      break;
    case 0x00F6:  // NtSetEvent
    case 0x00CE:  // NtClearEvent
    case 0x00E2:  // NtPulseEvent
      r.r3 = 0;
      break;

    // ---- critical sections ------------------------------------------------
    // Uncontended by construction: one thread. RtlEnter/Leave keep a recursion
    // count so guest code that inspects the section still sees something sane.
    case 0x012E: {  // RtlInitializeCriticalSection
      mem.Write32(r3 + 0x00, 0);
      mem.Write32(r3 + 0x04, 0xFFFFFFFF);
      mem.Write32(r3 + 0x08, 0);
      mem.Write32(r3 + 0x0C, 0);
      r.r3 = 0;
      break;
    }
    case 0x012F:  // RtlInitializeCriticalSectionAndSpinCount
      mem.Write32(r3 + 0x00, 0);
      mem.Write32(r3 + 0x04, 0xFFFFFFFF);
      mem.Write32(r3 + 0x08, 0);
      mem.Write32(r3 + 0x0C, r4);
      r.r3 = 0;
      break;
    case 0x0125:  // RtlEnterCriticalSection
      mem.Write32(r3 + 0x04, mem.Read32(r3 + 0x04) + 1);
      r.r3 = 0;
      break;
    case 0x0130:  // RtlLeaveCriticalSection
      mem.Write32(r3 + 0x04, mem.Read32(r3 + 0x04) - 1);
      r.r3 = 0;
      break;
    case 0x0141:  // RtlTryEnterCriticalSection
      mem.Write32(r3 + 0x04, mem.Read32(r3 + 0x04) + 1);
      r.r3 = 1;
      break;

    // ---- memory -----------------------------------------------------------
    case 0x00CC: {  // NtAllocateVirtualMemory(&base, &size, type, protect)
      uint32_t size = mem.Read32(r4);
      uint32_t p = HeapAlloc(size ? size : 0x1000, 0x1000);
      mem.Write32(r3, p);
      r.r3 = p ? 0 : 0xC0000017;  // STATUS_NO_MEMORY
      break;
    }
    case 0x00DB:  // NtFreeVirtualMemory
      r.r3 = 0;
      break;
    // Physical allocations. Returning 0 here is not a benign "no memory": the
    // DB's allocator retries forever on a NULL, so an unimplemented
    // MmAllocatePhysicalMemoryEx presents as an endless XPhysicalAlloc loop
    // rather than an error.
    case 0x00B9:  // MmAllocatePhysicalMemory(flags, size, protect)
      r.r3 = HeapAlloc(r4 ? r4 : gpr[3], 0x1000);
      break;
    case 0x00BA: {  // MmAllocatePhysicalMemoryEx(flags, size, prot, min, max, align)
      uint32_t align = gpr[8] ? gpr[8] : 0x1000;
      r.r3 = HeapAlloc(r4, align);
      break;
    }
    case 0x00BD:  // MmFreePhysicalMemory
      r.r3 = 0;
      break;
    case 0x00BE:  // MmGetPhysicalAddress - flat mapping here
      r.r3 = r3;
      break;
    case 0x00BF:  // MmIsAddressValid
      r.r3 = 1;
      break;
    case 0x00C5:  // MmQueryAllocationSize
      r.r3 = 0x1000;
      break;
    case 0x0009:  // ExAllocatePool(size)
    case 0x000A:  // ExAllocatePoolWithTag(size, tag)
    case 0x000B:  // ExAllocatePoolTypeWithTag(size, tag, type)
      r.r3 = HeapAlloc(r3, 16);
      break;

    // ---- thread-local storage --------------------------------------------
    // One thread, so a flat slot array is the whole implementation. The CRT
    // stores its per-thread state here and traps on a bad slot, so an
    // unimplemented KeTlsGetValue surfaces as a `twi` deep inside library code
    // rather than as anything resembling TLS.
    case 0x0152:  // KeTlsAlloc
      r.r3 = g_tls_next < kTlsSlots ? g_tls_next++ : 0xFFFFFFFF;
      break;
    case 0x0153:  // KeTlsFree
      r.r3 = 1;
      break;
    case 0x0154:  // KeTlsGetValue(slot)
      r.r3 = r3 < kTlsSlots ? g_tls[r3] : 0;
      break;
    case 0x0155:  // KeTlsSetValue(slot, value)
      if (r3 < kTlsSlots) g_tls[r3] = r4;
      r.r3 = 1;
      break;

    // ---- counted strings ---------------------------------------------------
    // STRING/UNICODE_STRING: { uint16 Length; uint16 MaximumLength; char* Buffer }
    case 0x012C: {  // RtlInitAnsiString(dest, src)
      uint32_t len = r4 ? (uint32_t)mem.ReadCStr(r4, 0xFFFF).size() : 0;
      mem.Write32(r3, (len << 16) | (r4 ? len + 1 : 0));
      mem.Write32(r3 + 4, r4);
      r.r3 = 0;
      break;
    }
    case 0x012D: {  // RtlInitUnicodeString(dest, src) - src is UTF-16
      uint32_t len = 0;
      if (r4) while (len < 0xFFFE && (mem.read8(r4 + len) || mem.read8(r4 + len + 1))) len += 2;
      mem.Write32(r3, (len << 16) | (r4 ? len + 2 : 0));
      mem.Write32(r3 + 4, r4);
      r.r3 = 0;
      break;
    }
    case 0x0142: {  // RtlUnicodeStringToAnsiString(dest, src, allocate)
      uint32_t slen = mem.Read32(r4) >> 16;      // Length, in bytes
      uint32_t sbuf = mem.Read32(r4 + 4);
      uint32_t n = slen / 2;
      uint32_t dbuf = r5 ? HeapAlloc(n + 1, 16) : mem.Read32(r3 + 4);
      for (uint32_t i = 0; i < n; i++)           // UTF-16BE -> ANSI, low byte
        mem.write8(dbuf + i, mem.read8(sbuf + i * 2 + 1));
      mem.write8(dbuf + n, 0);
      mem.Write32(r3, (n << 16) | (n + 1));
      mem.Write32(r3 + 4, dbuf);
      r.r3 = 0;
      break;
    }
    case 0x0127:  // RtlFreeAnsiString
      r.r3 = 0;
      break;

    // ---- misc ordinary answers -------------------------------------------
    case 0x00EE:  // NtQueryVirtualMemory
      r.r3 = 0;
      break;
    case 0x0135:  // RtlNtStatusToDosError
      r.r3 = 0;
      break;

    default:
      // Not implemented. Reported by name and answered with 0 so the run keeps
      // going -- one pass then lists everything still missing.
      r.implemented = false;
      r.r3 = 0;
      break;
  }
  return r;
}

}  // namespace xexmagic
