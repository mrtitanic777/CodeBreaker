// codebreaker/formats/xbox/crypto/lzx.hpp -- C++ port of codebreaker.formats.xbox.crypto.lzx.
//
// Wrappers over the Xbox 360 LZX compression / decompression libraries (the xbox360_crypto_bin setting's
// LZX/Windows/LZXCompression.dll and LZXDecompression.dll, or LZX/Linux/liblzxc.so and liblzxd.so): the
// C structures those libraries share with their callers, and two classes that drive them.
//
// References:
//   https://msopenspecs.azureedge.net/files/MS-PATCH/%5bMS-PATCH%5d-210422.pdf
//
// Typing notes:
//   * The Python loads the libraries with ctypes. Here a `Library` loads one (LoadLibraryEx / dlopen, with
//     ctypes' own search flags and its error types) and each exported function is a typed function pointer.
//   * ctypes structures are C++ structures of the same layout (their sizes and field offsets are checked
//     against ctypes by the test harness). `NULL` is spelled `NULL_` (NULL is a macro).
//   * `BinLike = bytes | bytearray | memoryview` is py::bytes (a py::bytearray converts to it).
//   * `LZXBOX_BLOCK` is a big-endian ctypes structure; here it holds host-order values and converts with
//     `to_bytes` / `from_buffer_copy`.
//   * The output callback the library calls has no instance pointer (the Python passes fci_data NULL and
//     binds the instance in a closure); here the trampoline finds the instance that is inside a call into
//     the library (a thread-local, set around every such call).
//   * `compressed_stream` (an io.BytesIO) is a `ByteStream`.
//   * The module-level library paths are computed on first use (the setting is not known at
//     static-initialisation time): `COMPRESSION_LIBRARY_PATH()`, `DECOMPRESSION_LIBRARY_PATH()`.
//   * The 64-bit and Windows-or-Linux checks that end the Python program at import are compile-time here.
//   * `decompress` ends with ctypes.resize(), which refuses to shrink the buffer: a library that reports
//     fewer bytes than asked for makes it raise ValueError("minimum size is N") (kept); one that reports
//     more extends the buffer (the Python leaves the extra bytes uninitialised; zero here).
//   * `__enter__` / `__exit__` are the same-named methods; `main()` returns the exit code.
#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>

#include "py.hpp"

static_assert(sizeof(void*) == 8, "This only works on 64-bit operating systems!");
#if !defined(_WIN32) && !defined(__linux__)
#error "This only works on Windows or Linux!"
#endif

namespace codebreaker::formats::xbox::crypto::lzx {

using BinLike = py::bytes;   // bytes | bytearray | memoryview

// ---- the library paths (settings.xbox360_crypto_bin / LZX/<os>/...) ----
const py::str& COMPRESSION_LIBRARY_PATH();
const py::str& DECOMPRESSION_LIBRARY_PATH();

// ---- C constants ----
inline constexpr int64_t LCI_SIGNATURE = 0x4349434C;   // LCIC
inline constexpr int64_t LDI_SIGNATURE = 0x4349444C;   // LDIC
inline constexpr int64_t LZX_WINDOW_SIZE = 128 * 1024;   // 0x20000
inline constexpr int64_t LZX_CHUNK_SIZE = 32 * 1024;     // 0x8000
inline constexpr int64_t MAX_GROWTH = 0x1800;
inline constexpr int64_t NUM_REPEATED_OFFSETS = 3;
inline constexpr int64_t MAIN_TREE_TABLE_BITS = 10;
inline constexpr int64_t SECONDARY_LEN_TREE_TABLE_BITS = 8;
inline constexpr int64_t MAX_MAIN_TREE_ELEMENTS = 256 + (8 * 291);
inline constexpr int64_t MIN_MATCH = 2;
inline constexpr int64_t MAX_MATCH = MIN_MATCH + 255;
inline constexpr int64_t NUM_PRIMARY_LENGTHS = 7;
inline constexpr int64_t NUM_SECONDARY_LENGTHS = (MAX_MATCH - MIN_MATCH + 1) - NUM_PRIMARY_LENGTHS;
inline constexpr int64_t ALIGNED_TABLE_BITS = 7;
inline constexpr int64_t ALIGNED_NUM_ELEMENTS = 8;

// ---- C types ----
inline constexpr int64_t NULL_ = 0;   // (NULL)
inline void* const NULLPTR = nullptr;
using BOOL = uint32_t;
using BYTE = uint8_t;
using CHAR = int8_t;
using SHORT = int16_t;
using LONG = int32_t;
using USHORT = uint16_t;
using ULONG = uint32_t;
using SIGNATURE = uint32_t;
using MHANDLE = int64_t;
using LDI_CONTEXT_HANDLE = MHANDLE;
using LCI_CONTEXT_HANDLE = MHANDLE;

// ---- C pointers ----
using PVOID = void*;
using PBYTE = uint8_t*;
using PLONG = int32_t*;
using PUSHORT = uint16_t*;
using PULONG = uint32_t*;
using PLDI_CONTEXT_HANDLE = LDI_CONTEXT_HANDLE*;
using PLCI_CONTEXT_HANDLE = LCI_CONTEXT_HANDLE*;

// ---- C function prototypes ----
using FNALLOC = PVOID (*)(ULONG);
using FNFREE = void (*)(PVOID);
using FNCALLBACK = LONG (*)(PVOID, PBYTE, LONG, LONG);

// ---- C function pointers ----
using PFNALLOC = FNALLOC*;
using PFNFREE = FNFREE*;
using PFNCALLBACK = FNCALLBACK*;

enum class lzx_block_type : int64_t {
    BLOCKTYPE_INVALID = 0,
    BLOCKTYPE_VERBATIM = 1,
    BLOCKTYPE_ALIGNED = 2,
    BLOCKTYPE_UNCOMPRESSED = 3,
};

enum class decoder_state : int64_t {
    DEC_STATE_UNKNOWN = 0,
    DEC_STATE_START_NEW_BLOCK = 1,
    DEC_STATE_DECODING_DATA = 2,
};

#pragma pack(push, 2)
struct LZXCOMPRESS {
    LONG WindowSize;
    LONG SecondPartitionSize;
};

struct LZXDECOMPRESS {
    LONG WindowSize;
    LONG fCPUtype;
};

struct decision_node {
    ULONG link;
    ULONG path;
    ULONG repeated_offset[NUM_REPEATED_OFFSETS];
    ULONG numbits;
};
#pragma pack(pop)

struct t_encoder_context {
    PBYTE enc_MemWindow;
    ULONG enc_window_size;
    PULONG enc_tree_root;
    PULONG enc_Left;
    PULONG enc_Right;
    ULONG enc_bitbuf;
    CHAR enc_bitcount;
    BOOL enc_output_overflow;
    CHAR pad1[2];
    ULONG enc_literals;
    ULONG enc_distances;
    PULONG enc_DistData;
    PBYTE enc_LitData;
    PBYTE enc_ItemType;
    ULONG enc_repeated_offset_at_literal_zero[NUM_REPEATED_OFFSETS];
    ULONG enc_last_matchpos_offset[NUM_REPEATED_OFFSETS];
    ULONG enc_matchpos_table[MAX_MATCH + 1];
    ULONG enc_BufPos;
    USHORT enc_slot_table[1024];
    PBYTE enc_output_buffer_start;
    PBYTE enc_output_buffer_curpos;
    PBYTE enc_output_buffer_end;
    ULONG enc_input_running_total;
    ULONG enc_bufpos_last_output_block;
    ULONG enc_num_position_slots;
    ULONG enc_file_size_for_translation;
    BYTE enc_num_block_splits;
    BYTE enc_ones[256];
    BYTE enc_first_block;
    BOOL enc_need_to_recalc_stats;
    BOOL enc_first_time_this_group;
    ULONG enc_encoder_second_partition_size;
    ULONG enc_earliest_window_data_remaining;
    ULONG enc_bufpos_at_last_block;
    PBYTE enc_input_ptr;
    LONG enc_input_left;
    ULONG enc_instr_pos;
    PUSHORT enc_tree_freq;
    PUSHORT enc_tree_sortptr;
    PBYTE enc_len;
    SHORT enc_tree_heap[MAX_MAIN_TREE_ELEMENTS + 2];
    USHORT enc_tree_leftright[2 * (2 * MAX_MAIN_TREE_ELEMENTS - 1)];
    USHORT enc_tree_len_cnt[17];
    LONG enc_tree_n;
    SHORT enc_tree_heapsize;
    CHAR enc_depth;
    ULONG enc_next_tree_create;
    ULONG enc_last_literals;
    ULONG enc_last_distances;
    decision_node* enc_decision_node;
    BYTE enc_main_tree_len[MAX_MAIN_TREE_ELEMENTS + 1];
    BYTE enc_secondary_tree_len[NUM_SECONDARY_LENGTHS + 1];
    USHORT enc_main_tree_freq[MAX_MAIN_TREE_ELEMENTS * 2];
    USHORT enc_main_tree_code[MAX_MAIN_TREE_ELEMENTS];
    BYTE enc_main_tree_prev_len[MAX_MAIN_TREE_ELEMENTS + 1];
    USHORT enc_secondary_tree_freq[NUM_SECONDARY_LENGTHS * 2];
    USHORT enc_secondary_tree_code[NUM_SECONDARY_LENGTHS];
    BYTE enc_secondary_tree_prev_len[NUM_SECONDARY_LENGTHS + 1];
    USHORT enc_aligned_tree_freq[ALIGNED_NUM_ELEMENTS * 2];
    USHORT enc_aligned_tree_code[ALIGNED_NUM_ELEMENTS];
    BYTE enc_aligned_tree_len[ALIGNED_NUM_ELEMENTS];
    BYTE enc_aligned_tree_prev_len[ALIGNED_NUM_ELEMENTS];
    PBYTE enc_RealMemWindow;
    PULONG enc_RealLeft;
    PULONG enc_RealRight;
    ULONG enc_num_cfdata_frames;
    PVOID enc_fci_data;
    PFNALLOC enc_malloc;
    PFNFREE enc_free;
    ULONG enc_inserted_dict_size;
    ULONG enc_last_bsearch_bufpos;
    PFNCALLBACK enc_output_callback_function;
};

struct t_decoder_context {
    PBYTE dec_mem_window;
    ULONG dec_window_size;
    ULONG dec_window_mask;
    ULONG dec_last_matchpos_offset[NUM_REPEATED_OFFSETS];
    SHORT dec_main_tree_table[1 << MAIN_TREE_TABLE_BITS];
    SHORT dec_secondary_length_tree_table[1 << SECONDARY_LEN_TREE_TABLE_BITS];
    BYTE dec_main_tree_len[MAX_MAIN_TREE_ELEMENTS];
    BYTE dec_secondary_length_tree_len[NUM_SECONDARY_LENGTHS];
    BYTE pad1[3];
    CHAR dec_aligned_table[1 << ALIGNED_TABLE_BITS];
    BYTE dec_aligned_len[ALIGNED_NUM_ELEMENTS];
    SHORT dec_main_tree_left_right[MAX_MAIN_TREE_ELEMENTS * 4];
    SHORT dec_secondary_length_tree_left_right[NUM_SECONDARY_LENGTHS * 4];
    PBYTE dec_input_curpos;
    PBYTE dec_end_input_pos;
    PBYTE dec_output_buffer;
    LONG dec_position_at_start;
    BYTE dec_main_tree_prev_len[MAX_MAIN_TREE_ELEMENTS];
    BYTE dec_secondary_length_tree_prev_len[NUM_SECONDARY_LENGTHS];
    ULONG dec_bitbuf;
    CHAR dec_bitcount;
    ULONG dec_num_position_slots;
    BOOL dec_first_time_this_group;
    BOOL dec_error_condition;
    LONG dec_bufpos;
    ULONG dec_current_file_size;
    ULONG dec_instr_pos;
    ULONG dec_num_cfdata_frames;
    LONG dec_original_block_size;
    LONG dec_block_size;
    LONG _dec_block_type;       // lzx_block_type
    LONG _dec_decoder_state;    // decoder_state
    PFNALLOC dec_malloc;
    PFNFREE dec_free;

    // the dec_block_type / dec_decoder_state properties
    lzx_block_type dec_block_type() const;   // ValueError for a value that is not a member
    void dec_block_type(lzx_block_type value);
    void dec_block_type(int64_t value);
    decoder_state dec_decoder_state() const;   // ValueError for a value that is not a member
    void dec_decoder_state(decoder_state value);
    void dec_decoder_state(int64_t value);
};

struct LCI_CONTEXT {
    ULONG signature;
    PFNALLOC pfnAlloc;
    PFNFREE pfnFree;
    ULONG cbDataBlockMax;
    ULONG file_translation_size;
    t_encoder_context* encoder_context;
};

struct LDI_CONTEXT {
    ULONG signature;
    PFNALLOC pfnAlloc;
    PFNFREE pfnFree;
    ULONG cbDataBlockMax;
    ULONG fCPUtype;
    t_decoder_context* decoder_context;
};

// a big-endian structure in the Python; host-order values here
struct LZXBOX_BLOCK {
    USHORT CompressedSize = 0;
    USHORT UncompressedSize = 0;
    static constexpr int64_t SIZE = 4;   // ctypes.sizeof
    py::bytes to_bytes() const;                                   // bytes(hdr)
    static LZXBOX_BLOCK from_buffer_copy(const py::bytes& buf);   // ValueError when buf is under 4 bytes
};

// io.BytesIO, the part used here
struct ByteStream {
    std::string buf;
    int64_t pos = 0;
    bool closed = false;
    ByteStream() = default;
    explicit ByteStream(const py::bytes& initial) : buf(initial.raw()) {}
    int64_t tell() const;                // ValueError once closed
    py::bytes read(int64_t n);           // read(n)
    void write(const py::bytes& data);   // ValueError once closed
    py::bytes getvalue() const;          // ValueError once closed
    void close() { closed = true; }
};

// a library loaded the way ctypes.CDLL does
class Library {
public:
    Library() = default;
    explicit Library(const py::str& name);   // FileNotFoundError / OSError as CDLL raises them
    void* function(const char* name) const;  // AttributeError("function 'name' not found")
private:
    void* handle_ = nullptr;
};

class LZXCompression {
public:
    Library dll;
    int64_t chunk_size = 0;
    LCI_CONTEXT_HANDLE ctx = 0;
    FNCALLBACK callback = nullptr;
    ByteStream compressed_stream;

    explicit LZXCompression(int64_t chunk_size = LZX_CHUNK_SIZE);
    LZXCompression& __enter__() { return *this; }
    void __exit__();   // AssertionError when LCIDestroyCompression fails
    int64_t create();
    int64_t reset();
    int64_t destroy();
    LONG callback_func(PVOID pfol, PBYTE compressed_data, LONG compressed_size, LONG uncompressed_size);
    py::bytes compress(const BinLike& data);
    py::bytes flush();
    py::bytes compress_continuous(const BinLike& data);

private:
    using LCICreateCompression_t = LONG (*)(PULONG, PVOID, PULONG, PLCI_CONTEXT_HANDLE, FNCALLBACK, PVOID);
    using LCICompress_t = LONG (*)(LCI_CONTEXT_HANDLE, PVOID, LONG, PVOID, LONG, PULONG);
    using Handle_t = LONG (*)(LCI_CONTEXT_HANDLE);
    using LCISetTranslationSize_t = LONG (*)(LCI_CONTEXT_HANDLE, LONG);
    using LCIGetInputData_t = PBYTE (*)(LCI_CONTEXT_HANDLE, PULONG, PULONG);
    using LCISetWindowData_t = LONG (*)(LCI_CONTEXT_HANDLE, PBYTE, ULONG);
    using LZX_EncodeInit_t = BOOL (*)(t_encoder_context*, LONG, LONG, FNCALLBACK, PVOID);
    using LZX_EncodeNewGroup_t = void (*)(t_encoder_context*);
    using LZX_Encode_t = LONG (*)(t_encoder_context*, PBYTE, LONG, PLONG, LONG);
    using LZX_EncodeBool_t = BOOL (*)(t_encoder_context*);
    using LZX_GetInputData_t = PBYTE (*)(t_encoder_context*, PULONG, PULONG);
    using LZX_EncodeInsertDictionary_t = BOOL (*)(t_encoder_context*, PBYTE, ULONG);

    // the exports, resolved (and so checked) in the order the constructor names them
    LCICreateCompression_t LCICreateCompression_ = nullptr;
    LCICompress_t LCICompress_ = nullptr;
    Handle_t LCIFlushCompressorOutput_ = nullptr;
    Handle_t LCIResetCompression_ = nullptr;
    Handle_t LCIDestroyCompression_ = nullptr;
    LCISetTranslationSize_t LCISetTranslationSize_ = nullptr;
    LCIGetInputData_t LCIGetInputData_ = nullptr;
    LCISetWindowData_t LCISetWindowData_ = nullptr;
    LZX_EncodeInit_t LZX_EncodeInit_ = nullptr;
    LZX_EncodeNewGroup_t LZX_EncodeNewGroup_ = nullptr;
    LZX_Encode_t LZX_Encode_ = nullptr;
    LZX_EncodeBool_t LZX_EncodeFlush_ = nullptr;
    LZX_EncodeBool_t LZX_EncodeResetState_ = nullptr;
    LZX_GetInputData_t LZX_GetInputData_ = nullptr;
    LZX_EncodeInsertDictionary_t LZX_EncodeInsertDictionary_ = nullptr;
    LZX_EncodeNewGroup_t LZX_EncodeFree_ = nullptr;
};

class LZXDecompression {
public:
    Library dll;
    int64_t chunk_size = 0;
    LDI_CONTEXT_HANDLE ctx = 0;

    explicit LZXDecompression(int64_t chunk_size = LZX_CHUNK_SIZE);
    LZXDecompression& __enter__() { return *this; }
    void __exit__();   // AssertionError when LDIDestroyDecompression fails
    int64_t create();
    int64_t reset();
    int64_t destroy();
    std::optional<py::bytes> delta_decompress(const BinLike& base_data, const BinLike& patch_data);   // (not written: None)
    py::bytes decompress(const BinLike& data, int64_t size);
    py::bytes decompress_chunk(const BinLike& data);
    py::bytes decompress_continuous(const BinLike& data);

private:
    using LDICreateDecompression_t = LONG (*)(PULONG, PVOID, PULONG, PLDI_CONTEXT_HANDLE);
    using LDIDecompress_t = LONG (*)(LDI_CONTEXT_HANDLE, PVOID, LONG, PVOID, PULONG);
    using Handle_t = LONG (*)(LDI_CONTEXT_HANDLE);
    using LDIGetWindow_t = LONG (*)(LDI_CONTEXT_HANDLE, PBYTE*, PLONG, PLONG, PLONG);
    using LDISetWindowData_t = LONG (*)(LDI_CONTEXT_HANDLE, PBYTE, ULONG);
    using LZX_DecodeInit_t = BOOL (*)(t_decoder_context*, LONG);
    using LZX_DecodeNewGroup_t = void (*)(t_decoder_context*);
    using LZX_Decode_t = LONG (*)(t_decoder_context*, LONG, PBYTE, LONG, PBYTE, LONG, PLONG);
    using LZX_DecodeInsertDictionary_t = BOOL (*)(t_decoder_context*, PBYTE, ULONG);

    LDICreateDecompression_t LDICreateDecompression_ = nullptr;
    LDIDecompress_t LDIDecompress_ = nullptr;
    Handle_t LDIResetDecompression_ = nullptr;
    Handle_t LDIDestroyDecompression_ = nullptr;
    LDIGetWindow_t LDIGetWindow_ = nullptr;
    LDISetWindowData_t LDISetWindowData_ = nullptr;
    LZX_DecodeInit_t LZX_DecodeInit_ = nullptr;
    LZX_DecodeNewGroup_t LZX_DecodeNewGroup_ = nullptr;
    LZX_Decode_t LZX_Decode_ = nullptr;
    LZX_DecodeInsertDictionary_t LZX_DecodeInsertDictionary_ = nullptr;
    LZX_DecodeNewGroup_t LZX_DecodeFree_ = nullptr;
};

int64_t main();   // compress data/bootloader/uncompressed.bin and decompress it again; the exit code

// __all__
py::list<py::str> __all__();

}  // namespace codebreaker::formats::xbox::crypto::lzx
