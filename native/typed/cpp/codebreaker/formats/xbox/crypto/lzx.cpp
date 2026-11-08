// codebreaker/formats/xbox/crypto/lzx.cpp -- C++ port of codebreaker.formats.xbox.crypto.lzx.

#include "codebreaker/formats/xbox/crypto/lzx.hpp"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <vector>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::formats::xbox::crypto::lzx {

// =====================================================================================================
// the sizes the libraries and the Python agree on (the harness also checks them against ctypes)
// =====================================================================================================
static_assert(sizeof(LZXCOMPRESS) == 8 && sizeof(LZXDECOMPRESS) == 8, "ctypes layout");
static_assert(sizeof(decision_node) == 24, "ctypes layout");
static_assert(sizeof(LCI_CONTEXT) == 40 && sizeof(LDI_CONTEXT) == 40, "ctypes layout");

namespace {

// f"{ret:X}" of a signed 32-bit value: a negative one is "-" and the hex of its magnitude
std::string hex_upper(int64_t v) {
    char b[32];
    if (v < 0) std::snprintf(b, sizeof b, "-%llX", static_cast<unsigned long long>(0 - static_cast<uint64_t>(v)));
    else std::snprintf(b, sizeof b, "%llX", static_cast<unsigned long long>(v));
    return b;
}

std::string failed(const char* what, int64_t ret) {
    return std::string(what) + " failed with code 0x" + hex_upper(ret) + "!";
}

// str(Path(base) / rel): the parts joined with the platform's separator, empty and "." parts dropped
py::str path_str(const py::str& base, const std::string& rel) {
    std::string joined = base.utf8().empty() ? rel : base.utf8() + "/" + rel;   // (Path("") is ".")
#ifdef _WIN32
    const char sep = '\\';
    std::string drive;
    size_t i = 0;
    if (joined.size() >= 2 && joined[1] == ':' && std::isalpha(static_cast<unsigned char>(joined[0]))) {
        drive = joined.substr(0, 2);
        i = 2;
    }
    bool root = i < joined.size() && (joined[i] == '/' || joined[i] == '\\');
#else
    const char sep = '/';
    std::string drive;
    size_t i = 0;
    bool root = !joined.empty() && joined[0] == '/';
#endif
    std::string out = drive + (root ? std::string(1, sep) : std::string());
    bool first = true;
    std::string part;
    auto flush = [&]() {
        if (!part.empty() && part != ".") {
            if (!first) out += sep;
            out += part;
            first = false;
        }
        part.clear();
    };
    for (; i < joined.size(); ++i) {
        char c = joined[i];
        if (c == '/' || (sep == '\\' && c == '\\')) flush();
        else part += c;
    }
    flush();
    return py::str(out.empty() ? std::string(".") : out);
}

// a pointer field's `.contents` address (ctypes raises ValueError for a NULL pointer)
template <class P> int64_t contents_address(P* p) {
    if (!p) throw py::ValueError("NULL pointer access");
    return static_cast<int64_t>(reinterpret_cast<intptr_t>(p));
}

// ctypes: (c_ubyte * n)() for a negative n
void check_array_length(int64_t n) {
    if (n < 0) throw py::ValueError("Array length must be >= 0, not " + std::to_string(n));
}

}  // namespace

// =====================================================================================================
// the library paths
// =====================================================================================================
const py::str& COMPRESSION_LIBRARY_PATH() {
    static const py::str p = path_str(py::settings::get_str(py::str("xbox360_crypto_bin")),
#ifdef _WIN32
                                      "LZX/Windows/LZXCompression.dll");
#else
                                      "LZX/Linux/liblzxc.so");
#endif
    return p;
}

const py::str& DECOMPRESSION_LIBRARY_PATH() {
    static const py::str p = path_str(py::settings::get_str(py::str("xbox360_crypto_bin")),
#ifdef _WIN32
                                      "LZX/Windows/LZXDecompression.dll");
#else
                                      "LZX/Linux/liblzxd.so");
#endif
    return p;
}

// =====================================================================================================
// t_decoder_context properties
// =====================================================================================================
lzx_block_type t_decoder_context::dec_block_type() const {
    if (_dec_block_type < 0 || _dec_block_type > 3)
        throw py::ValueError(std::to_string(_dec_block_type) + " is not a valid lzx_block_type");
    return static_cast<lzx_block_type>(_dec_block_type);
}
void t_decoder_context::dec_block_type(lzx_block_type value) { _dec_block_type = static_cast<LONG>(static_cast<int64_t>(value)); }
void t_decoder_context::dec_block_type(int64_t value) { _dec_block_type = static_cast<LONG>(value); }

decoder_state t_decoder_context::dec_decoder_state() const {
    if (_dec_decoder_state < 0 || _dec_decoder_state > 2)
        throw py::ValueError(std::to_string(_dec_decoder_state) + " is not a valid decoder_state");
    return static_cast<decoder_state>(_dec_decoder_state);
}
void t_decoder_context::dec_decoder_state(decoder_state value) { _dec_decoder_state = static_cast<LONG>(static_cast<int64_t>(value)); }
void t_decoder_context::dec_decoder_state(int64_t value) { _dec_decoder_state = static_cast<LONG>(value); }

// =====================================================================================================
// LZXBOX_BLOCK (big-endian on the wire)
// =====================================================================================================
py::bytes LZXBOX_BLOCK::to_bytes() const {
    std::string s(4, '\0');
    s[0] = static_cast<char>(CompressedSize >> 8);
    s[1] = static_cast<char>(CompressedSize & 0xFF);
    s[2] = static_cast<char>(UncompressedSize >> 8);
    s[3] = static_cast<char>(UncompressedSize & 0xFF);
    return py::bytes(s);
}

LZXBOX_BLOCK LZXBOX_BLOCK::from_buffer_copy(const py::bytes& buf) {
    if (buf.size() < SIZE)
        throw py::ValueError("Buffer size too small (" + std::to_string(buf.size()) + " instead of at least " +
                             std::to_string(SIZE) + " bytes)");
    LZXBOX_BLOCK h;
    h.CompressedSize = static_cast<USHORT>((buf.byte_(0) << 8) | buf.byte_(1));
    h.UncompressedSize = static_cast<USHORT>((buf.byte_(2) << 8) | buf.byte_(3));
    return h;
}

// =====================================================================================================
// ByteStream (io.BytesIO)
// =====================================================================================================
int64_t ByteStream::tell() const {
    if (closed) throw py::ValueError("I/O operation on closed file.");
    return pos;
}

py::bytes ByteStream::read(int64_t n) {
    if (closed) throw py::ValueError("I/O operation on closed file.");
    int64_t size = static_cast<int64_t>(buf.size());
    if (pos >= size) return py::bytes();
    int64_t take = (n < 0 || n > size - pos) ? size - pos : n;
    py::bytes out(buf.substr(static_cast<size_t>(pos), static_cast<size_t>(take)));
    pos += take;
    return out;
}

void ByteStream::write(const py::bytes& data) {
    if (closed) throw py::ValueError("I/O operation on closed file.");
    int64_t size = static_cast<int64_t>(buf.size());
    if (pos > size) buf.append(static_cast<size_t>(pos - size), '\0');
    buf.replace(static_cast<size_t>(pos), std::min<size_t>(data.raw().size(), buf.size() - static_cast<size_t>(pos)), data.raw());
    pos += static_cast<int64_t>(data.raw().size());
}

py::bytes ByteStream::getvalue() const {
    if (closed) throw py::ValueError("I/O operation on closed file.");
    return py::bytes(buf);
}

// =====================================================================================================
// Library (ctypes.CDLL)
// =====================================================================================================
Library::Library(const py::str& name) {
    std::string n = name.utf8();
#ifdef _WIN32
    // ctypes: a name with a separator is made absolute and its own folder is searched for dependencies
    int wl = MultiByteToWideChar(CP_UTF8, 0, n.c_str(), -1, nullptr, 0);
    std::wstring w(static_cast<size_t>(wl > 0 ? wl - 1 : 0), L'\0');
    if (wl > 1) MultiByteToWideChar(CP_UTF8, 0, n.c_str(), -1, &w[0], wl);
    DWORD mode = LOAD_LIBRARY_SEARCH_DEFAULT_DIRS;
    std::wstring full = w;
    if (n.find('/') != std::string::npos || n.find('\\') != std::string::npos) {
        DWORD need = GetFullPathNameW(w.c_str(), 0, nullptr, nullptr);
        if (need) {
            std::wstring buf(need, L'\0');
            DWORD got = GetFullPathNameW(w.c_str(), need, &buf[0], nullptr);
            buf.resize(got);
            full = buf;
        }
        mode |= LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR;
    }
    HMODULE h = LoadLibraryExW(full.c_str(), nullptr, mode);
    if (!h) {
        DWORD err = GetLastError();
        int ul = WideCharToMultiByte(CP_UTF8, 0, full.c_str(), -1, nullptr, 0, nullptr, nullptr);
        std::string fu(static_cast<size_t>(ul > 0 ? ul - 1 : 0), '\0');
        if (ul > 1) WideCharToMultiByte(CP_UTF8, 0, full.c_str(), -1, &fu[0], ul, nullptr, nullptr);
        if (err == ERROR_MOD_NOT_FOUND)
            throw py::FileNotFoundError("Could not find module '" + fu +
                                        "' (or one of its dependencies). Try using the full path with constructor syntax.");
        char text[512] = {0};
        FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, err, 0, text, sizeof text, nullptr);
        std::string t = text;
        while (!t.empty() && (t.back() == '\r' || t.back() == '\n' || t.back() == ' ' || t.back() == '.')) t.pop_back();
        throw py::OSError("[WinError " + std::to_string(err) + "] " + t);
    }
    handle_ = h;
#else
    handle_ = dlopen(n.c_str(), RTLD_NOW);
    if (!handle_) throw py::OSError(dlerror());
#endif
}

void* Library::function(const char* name) const {
#ifdef _WIN32
    void* p = reinterpret_cast<void*>(GetProcAddress(static_cast<HMODULE>(handle_), name));
#else
    void* p = dlsym(handle_, name);
#endif
    if (!p) throw py::AttributeError(std::string("function '") + name + "' not found");
    return p;
}

// =====================================================================================================
// LZXCompression
// =====================================================================================================
namespace {

// The library calls back with no instance pointer; the instance that is inside a call into the library
// is the one to hand the data to.
thread_local LZXCompression* g_active = nullptr;

struct ActiveScope {
    LZXCompression* prev;
    explicit ActiveScope(LZXCompression* self) : prev(g_active) { g_active = self; }
    ~ActiveScope() { g_active = prev; }
};

// (ctypes prints and ignores an exception raised inside a callback and returns 0)
LONG output_trampoline(PVOID pfol, PBYTE compressed_data, LONG compressed_size, LONG uncompressed_size) {
    if (!g_active) return 0;
    try {
        return g_active->callback_func(pfol, compressed_data, compressed_size, uncompressed_size);
    } catch (...) {
        return 0;
    }
}

template <class F> F resolve(const Library& dll, const char* name) { return reinterpret_cast<F>(dll.function(name)); }

}  // namespace

LZXCompression::LZXCompression(int64_t chunk_size_) {
    compressed_stream = ByteStream();

    chunk_size = chunk_size_;

    dll = Library(COMPRESSION_LIBRARY_PATH());

    // LONG LCICreateCompression(PULONG pcbDataBlockMax, PVOID pvConfiguration, PULONG pcbDstBufferMin, LCI_CONTEXT_HANDLE* pmchHandle,
    // 	  LONG (*pfnlzx_output_callback)(
    //         PVOID pfol,
    //         PBYTE compressed_data,
    //         LONG compressed_size,
    //         LONG uncompressed_size
    //     ),
    //     PVOID fci_data);
    LCICreateCompression_ = resolve<LCICreateCompression_t>(dll, "LCICreateCompression");

    // LONG LCICompress(LCI_CONTEXT_HANDLE hmc, PVOID pbSrc, ULONG cbSrc, PVOID pbDst, ULONG cbDst, PULONG pcbResult);
    LCICompress_ = resolve<LCICompress_t>(dll, "LCICompress");

    // LONG LCIFlushCompressorOutput(LCI_CONTEXT_HANDLE hmc);
    LCIFlushCompressorOutput_ = resolve<Handle_t>(dll, "LCIFlushCompressorOutput");

    // LONG LCIResetCompression(LCI_CONTEXT_HANDLE hmc);
    LCIResetCompression_ = resolve<Handle_t>(dll, "LCIResetCompression");

    // LONG LCIDestroyCompression(LCI_CONTEXT_HANDLE hmc);
    LCIDestroyCompression_ = resolve<Handle_t>(dll, "LCIDestroyCompression");

    // LONG LCISetTranslationSize(LCI_CONTEXT_HANDLE hmc, ULONG size);
    LCISetTranslationSize_ = resolve<LCISetTranslationSize_t>(dll, "LCISetTranslationSize");

    // PBYTE LCIGetInputData(LCI_CONTEXT_HANDLE hmc, PULONG input_position, PULONG bytes_available);
    LCIGetInputData_ = resolve<LCIGetInputData_t>(dll, "LCIGetInputData");

    // LONG LCISetWindowData(LCI_CONTEXT_HANDLE hmd, PBYTE pbWindowData, ULONG pcbWindowData);
    LCISetWindowData_ = resolve<LCISetWindowData_t>(dll, "LCISetWindowData");

    // bool LZX_EncodeInit(
    // 	  t_encoder_context* enc_context,
    //     LONG compression_window_size,
    //     LONG second_partition_size,
    //     LONG (* pfnlzx_output_callback)(
    //         PVOID pfol,
    //         PBYTE compressed_data,
    //         LONG compressed_size,
    //         LONG uncompressed_size
    //     ),
    //     PVOID fci_data);
    LZX_EncodeInit_ = resolve<LZX_EncodeInit_t>(dll, "LZX_EncodeInit");

    // void LZX_EncodeNewGroup(t_encoder_context *context);
    LZX_EncodeNewGroup_ = resolve<LZX_EncodeNewGroup_t>(dll, "LZX_EncodeNewGroup");

    // LONG LZX_Encode(
    //     t_encoder_context* context,
    //     PBYTE input_data,
    //     LONG input_size,
    //     PLONG bytes_compressed,
    //     LONG file_size_for_translation);
    LZX_Encode_ = resolve<LZX_Encode_t>(dll, "LZX_Encode");

    // bool LZX_EncodeFlush(t_encoder_context *context);
    LZX_EncodeFlush_ = resolve<LZX_EncodeBool_t>(dll, "LZX_EncodeFlush");

    // bool LZX_EncodeResetState(t_encoder_context *context);
    LZX_EncodeResetState_ = resolve<LZX_EncodeBool_t>(dll, "LZX_EncodeResetState");

    // unsigned char* LZX_GetInputData(
    //     t_encoder_context *context,
    //     PULONG input_position,
    //     PULONG bytes_available);
    LZX_GetInputData_ = resolve<LZX_GetInputData_t>(dll, "LZX_GetInputData");

    // bool LZX_EncodeInsertDictionary(
    //     t_encoder_context* context,
    //     PBYTE input_data,
    //     ULONG input_size);
    LZX_EncodeInsertDictionary_ = resolve<LZX_EncodeInsertDictionary_t>(dll, "LZX_EncodeInsertDictionary");

    // void LZX_EncodeFree(t_encoder_context* context);
    LZX_EncodeFree_ = resolve<LZX_EncodeNewGroup_t>(dll, "LZX_EncodeFree");

    create();
}

void LZXCompression::__exit__() {
    int64_t ret = destroy();
    if (ret != 0) throw py::AssertionError(failed("LCIDestroyCompression", ret));
    compressed_stream.close();
}

int64_t LZXCompression::create() {
    ctx = 0;

    LZXCOMPRESS lzx;
    lzx.WindowSize = static_cast<LONG>(LZX_WINDOW_SIZE);
    lzx.SecondPartitionSize = 32 * 1024;

    ULONG pcbDataBlockMax = static_cast<ULONG>(chunk_size);
    ULONG pcbDstBufferMin = 0;

    // (the Python keeps the callback as an attribute so that it is not freed)
    callback = &output_trampoline;

    LONG ret;
    {
        ActiveScope scope(this);
        ret = LCICreateCompression_(&pcbDataBlockMax, &lzx, &pcbDstBufferMin, &ctx, callback, NULLPTR);
    }
    if (ret != 0) throw py::AssertionError(failed("LCICreateCompression", ret));

    LCI_CONTEXT* lci = reinterpret_cast<LCI_CONTEXT*>(static_cast<intptr_t>(ctx));

    int64_t a_addr_0 = contents_address(lci->pfnAlloc);
    int64_t f_addr_0 = contents_address(lci->pfnFree);

    t_encoder_context* enc = lci->encoder_context;
    contents_address(enc);   // (.contents of the context pointer)
    int64_t a_addr_1 = contents_address(enc->enc_malloc);
    int64_t f_addr_1 = contents_address(enc->enc_free);

    bool b = true;
    b &= static_cast<int64_t>(lci->signature) == LCI_SIGNATURE;
    b &= a_addr_0 == a_addr_1;
    b &= f_addr_0 == f_addr_1;

    if (!b) throw py::AssertionError("LCI t_encoder_context integrity failed!");

    return ret;
}

int64_t LZXCompression::reset() {
    compressed_stream.close();
    compressed_stream = ByteStream();
    Handle_t fn = resolve<Handle_t>(dll, "LDIResetCompression");   // (not named by the constructor: looked up now)
    ActiveScope scope(this);
    return fn(ctx);
}

int64_t LZXCompression::destroy() {
    ActiveScope scope(this);
    return LCIDestroyCompression_(ctx);
}

LONG LZXCompression::callback_func(PVOID pfol, PBYTE compressed_data, LONG compressed_size, LONG uncompressed_size) {
    (void)pfol;
    check_array_length(compressed_size);
    std::string pcd(static_cast<size_t>(compressed_size), '\0');
    if (compressed_size > 0) std::memmove(&pcd[0], compressed_data, static_cast<size_t>(compressed_size));

    // print("CALLBACK!")

    LZXBOX_BLOCK hdr;
    hdr.CompressedSize = static_cast<USHORT>(compressed_size);
    hdr.UncompressedSize = static_cast<USHORT>(uncompressed_size);

    compressed_stream.write(hdr.to_bytes());
    compressed_stream.write(py::bytes(pcd));

    return 0;
}

py::bytes LZXCompression::compress(const BinLike& data) {
    std::string in_data = data.raw();
    ULONG out_size = 0;
    LONG ret;
    {
        ActiveScope scope(this);
        ret = LCICompress_(ctx, &in_data[0], static_cast<LONG>(data.size()), NULLPTR,
                           static_cast<LONG>(data.size() + MAX_GROWTH), &out_size);
    }
    if (ret != 0) throw py::AssertionError(failed("LCICompress", ret));
    return compressed_stream.getvalue();
}

py::bytes LZXCompression::flush() {
    LONG ret;
    {
        ActiveScope scope(this);
        ret = LCIFlushCompressorOutput_(ctx);
    }
    if (ret != 0) throw py::AssertionError(failed("LCIFlushCompressorOutput", ret));
    return compressed_stream.getvalue();
}

py::bytes LZXCompression::compress_continuous(const BinLike& data) {
    int64_t size = data.size();
    ByteStream bio(data);
    int64_t pos;
    while ((pos = bio.tell()) < size) {
        int64_t chunk = chunk_size;
        if ((size - pos) < chunk) chunk = size - pos;
        compress(bio.read(chunk));
    }
    flush();
    return compressed_stream.getvalue();
}

// =====================================================================================================
// LZXDecompression
// =====================================================================================================
LZXDecompression::LZXDecompression(int64_t chunk_size_) {
    // self.reset()

    chunk_size = chunk_size_;

    dll = Library(DECOMPRESSION_LIBRARY_PATH());

    // int LDICreateDecompression(PULONG pcbDataBlockMax, PVOID pvConfiguration, PULONG pcbSrcBufferMin, LDI_CONTEXT_HANDLE* pmdhHandle);
    LDICreateDecompression_ = resolve<LDICreateDecompression_t>(dll, "LDICreateDecompression");

    // int LDIDecompress(LDI_CONTEXT_HANDLE hmd, PVOID pbSrc, UINT cbSrc, PVOID pbDst, PULONG pcbResult);
    LDIDecompress_ = resolve<LDIDecompress_t>(dll, "LDIDecompress");

    // int LDIResetDecompression(LDI_CONTEXT_HANDLE hmd);
    LDIResetDecompression_ = resolve<Handle_t>(dll, "LDIResetDecompression");

    // int LDIDestroyDecompression(LDI_CONTEXT_HANDLE hmd);
    LDIDestroyDecompression_ = resolve<Handle_t>(dll, "LDIDestroyDecompression");

    // int LDIGetWindow(LDI_CONTEXT_HANDLE hmd, PBYTE* ppWindow, PLONG pFileOffset, PLONG pWindowOffset, PLONG pcbBytesAvail);
    LDIGetWindow_ = resolve<LDIGetWindow_t>(dll, "LDIGetWindow");

    // int LDISetWindowData(LDI_CONTEXT_HANDLE hmd, PBYTE pbWindowData, ULONG pcbWindowData);
    LDISetWindowData_ = resolve<LDISetWindowData_t>(dll, "LDISetWindowData");

    // bool LZX_DecodeInit(t_decoder_context *context, LONG compression_window_size);
    LZX_DecodeInit_ = resolve<LZX_DecodeInit_t>(dll, "LZX_DecodeInit");

    // void LZX_DecodeNewGroup(t_decoder_context *context);
    LZX_DecodeNewGroup_ = resolve<LZX_DecodeNewGroup_t>(dll, "LZX_DecodeNewGroup");

    // int LZX_Decode(t_decoder_context *context, LONG bytes_to_decode, PBYTE compressed_input_buffer, LONG compressed_input_size, PBYTE uncompressed_output_buffer, LONG uncompressed_output_size, PLONG bytes_decoded);
    LZX_Decode_ = resolve<LZX_Decode_t>(dll, "LZX_Decode");

    // bool LZX_DecodeInsertDictionary(t_decoder_context *context, const PBYTE data, ULONG data_size);
    LZX_DecodeInsertDictionary_ = resolve<LZX_DecodeInsertDictionary_t>(dll, "LZX_DecodeInsertDictionary");

    // void LZX_DecodeFree(t_decoder_context* context);
    LZX_DecodeFree_ = resolve<LZX_DecodeNewGroup_t>(dll, "LZX_DecodeFree");

    create();
}

void LZXDecompression::__exit__() {
    int64_t ret = destroy();
    if (ret != 0) throw py::AssertionError(failed("LCIDestroyCompression", ret));
}

int64_t LZXDecompression::create() {
    ctx = 0;

    LZXDECOMPRESS lzx;
    lzx.WindowSize = static_cast<LONG>(LZX_WINDOW_SIZE);
    lzx.fCPUtype = 1;

    ULONG pcbDataBlockMax = static_cast<ULONG>(chunk_size);
    ULONG pcbSrcBufferMin = 0;

    LONG ret = LDICreateDecompression_(&pcbDataBlockMax, &lzx, &pcbSrcBufferMin, &ctx);
    if (ret != 0) throw py::AssertionError(failed("LDICreateDecompression", ret));

    LDI_CONTEXT* ldi = reinterpret_cast<LDI_CONTEXT*>(static_cast<intptr_t>(ctx));

    int64_t a_addr_0 = contents_address(ldi->pfnAlloc);
    int64_t f_addr_0 = contents_address(ldi->pfnFree);

    t_decoder_context* dec = ldi->decoder_context;
    contents_address(dec);   // (.contents of the context pointer)
    int64_t a_addr_1 = contents_address(dec->dec_malloc);
    int64_t f_addr_1 = contents_address(dec->dec_free);

    bool b = true;
    b &= static_cast<int64_t>(ldi->signature) == LDI_SIGNATURE;
    b &= a_addr_0 == a_addr_1;
    b &= f_addr_0 == f_addr_1;

    if (!b) throw py::AssertionError("LDI t_decoder_context integrity failed!");

    return ret;
}

int64_t LZXDecompression::reset() { return LDIResetDecompression_(ctx); }

int64_t LZXDecompression::destroy() { return LDIDestroyDecompression_(ctx); }

std::optional<py::bytes> LZXDecompression::delta_decompress(const BinLike& base_data, const BinLike& patch_data) {
    (void)base_data;
    (void)patch_data;
    return std::nullopt;
}

py::bytes LZXDecompression::decompress(const BinLike& data, int64_t size) {
    std::string in_data = data.raw();
    check_array_length(size);
    std::string out_data(static_cast<size_t>(size), '\0');
    ULONG out_size = static_cast<ULONG>(size);
    LONG ret = LDIDecompress_(ctx, &in_data[0], static_cast<LONG>(data.size()), &out_data[0], &out_size);
    if (ret != 0) throw py::AssertionError(failed("LDIDecompress", ret));
    // ctypes.resize(out_data, out_size.value): it will not shrink the buffer
    if (static_cast<int64_t>(out_size) < size) throw py::ValueError("minimum size is " + std::to_string(size));
    out_data.resize(out_size, '\0');
    return py::bytes(out_data);
}

py::bytes LZXDecompression::decompress_chunk(const BinLike& data_) {
    LZXBOX_BLOCK hdr = LZXBOX_BLOCK::from_buffer_copy(data_.slice(0, LZXBOX_BLOCK::SIZE));
    py::bytes data = data_.slice_from(LZXBOX_BLOCK::SIZE);
    if (data.size() != hdr.CompressedSize) throw py::AssertionError("data size is invalid!");
    return decompress(data, hdr.UncompressedSize);
}

py::bytes LZXDecompression::decompress_continuous(const BinLike& data) {
    int64_t size = data.size();
    ByteStream rbio(data);
    ByteStream wbio;
    int64_t pos;
    while ((pos = rbio.tell()) < size) {
        LZXBOX_BLOCK hdr = LZXBOX_BLOCK::from_buffer_copy(rbio.read(LZXBOX_BLOCK::SIZE));
        py::bytes block = rbio.read(hdr.CompressedSize);
        block = decompress(block, hdr.UncompressedSize);
        if (block.size() != hdr.UncompressedSize) throw py::AssertionError("Decompressed size mismatch!");
        wbio.write(block);
    }
    return wbio.getvalue();
}

// =====================================================================================================
// main
// =====================================================================================================
int64_t main() {
    py::str pr = path_str(py::str(""), "data/bootloader/uncompressed.bin");

    py::bytes uncomp_data;
    py::bytes comp_data;
    {
        // with LZXCompression() as lzxc, pr.open("rb") as fr:
        LZXCompression lzxc;
        lzxc.__enter__();
        try {
            py::file fr = py::open_rb(pr);
            try {
                uncomp_data = fr.read();
                comp_data = lzxc.compress_continuous(uncomp_data);
            } catch (...) {
                fr.close();
                throw;
            }
            fr.close();
        } catch (...) {
            lzxc.__exit__();   // (its own failure replaces the error)
            throw;
        }
        lzxc.__exit__();
    }

    py::bytes decomp_data;
    {
        // with LZXDecompression() as lzxd, BytesIO(comp_data) as fr:
        LZXDecompression lzxd;
        lzxd.__enter__();
        try {
            ByteStream fr(comp_data);
            decomp_data = lzxd.decompress_continuous(fr.read(-1));
        } catch (...) {
            lzxd.__exit__();
            throw;
        }
        lzxd.__exit__();
    }

    if (!(uncomp_data == decomp_data)) throw py::AssertionError("Files aren't the same!");

    py::print(py::str("Done!"));

    return 0;
}

py::list<py::str> __all__() { return py::list<py::str>{py::str("LZXCompression"), py::str("LZXDecompression")}; }

}  // namespace codebreaker::formats::xbox::crypto::lzx
