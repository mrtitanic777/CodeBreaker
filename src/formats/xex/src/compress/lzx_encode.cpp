// lzx_encode.cpp - the MS-LZX compressor (optimal parse, deterministic).
//
// MODEL: the encoder keeps a 0x4350-byte context block plus several heap
// buffers, storing 32-bit offsets inside the block and doing biased offset
// arithmetic (a window base minus the window size, a tree base minus four times
// it, ...). To stay exact and portable to 64-bit, the context and every buffer
// live in ONE arena and each stored "pointer" is a 32-bit signed byte offset
// into it: `*(T*)(slot + idx)` becomes `*(T*)(A + (int32_t)slot + idx)`, which
// keeps flat 32-bit address arithmetic exact (differences and biases included).
// The context's fields are named below by their byte offset.
//
// Verified byte-exact by compressing a 36 MB Xbox 360 basefile and diffing
// against canonical `compress` output.
#include "lzx_encode.h"
#include <cstring>
#include <stdexcept>

namespace xex::compress {
namespace {

// ---- static tables ---------------------------------------------------------
// extra_bits[slot]
static const uint8_t kExtraBits[52] = {
    0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,
    13,13,14,14,15,15,16,16,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,0};
// footer_mask[slot] = (1<<extra_bits)-1
static const uint32_t kFooterMask[51] = {
    0,0,0,0,1,1,3,3,7,7,0xf,0xf,0x1f,0x1f,0x3f,0x3f,0x7f,0x7f,0xff,0xff,
    0x1ff,0x1ff,0x3ff,0x3ff,0x7ff,0x7ff,0xfff,0xfff,0x1fff,0x1fff,0x3fff,0x3fff,
    0x7fff,0x7fff,0xffff,0xffff,0x1ffff,0x1ffff,0x3ffff,0x3ffff,0x7ffff,0x7ffff,
    0xfffff,0xfffff,0x1fffff,0x1fffff,0x3fffff,0x3fffff,0x7fffff,0x7fffff,0x1ffffff};
// pretree length-delta symbol, centred: kDelta[16 + prevLen - curLen]
// (negative deltas wrap to 17+delta). Indexable for delta in [-16, +16].
static const uint8_t kDelta[33] = {
    1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
    0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
// block-split divergence tables: kSplitMag[x]=bit-length of x,
// kSplitWeight[m]=m^2. metric = Σ|kSplitWeight[mag(h1)]-kSplitWeight[mag(h2)]|
static const uint8_t kSplitMag[256] = {
    0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
    6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8};
static const uint32_t kSplitWeight[18] = {
    0,1,4,9,16,25,36,49,64,81,100,121,144,169,196,225,256,0};
static inline uint32_t split_mag(uint32_t f){ return f < 0x100 ? kSplitMag[f] : (uint32_t)kSplitMag[f >> 8] + 8; }

// ---- the context block: byte offsets of its fields --------------------------
// Match finder
constexpr size_t WIN_BASE      = 0x000;  // window base, biased so WIN_BASE + pos is window byte pos
constexpr size_t WINDOW        = 0x004;  // window size
constexpr size_t BT_HEAD       = 0x008;  // last position inserted into the binary tree
constexpr size_t BT_LEFT       = 0x00c;  // left-child array base (biased)
constexpr size_t BT_RIGHT      = 0x010;  // right-child array base (biased)
// Bit writer
constexpr size_t BITBUF        = 0x014;  // 32-bit accumulator, MSB first
constexpr size_t BITBUF_B2     = 0x016;  //   its byte 2 and
constexpr size_t BITBUF_B3     = 0x017;  //   byte 3: the 16 bits written out next
constexpr size_t BITFREE       = 0x018;  // free bits in the accumulator (starts at 0x20)
constexpr size_t OUT_OVERFLOW  = 0x01c;  // the output buffer wrapped
// Pending tokens (the parse's output, not yet in a block)
constexpr size_t TOKENS        = 0x024;  // literals + matches pending
constexpr size_t MATCHES       = 0x028;  // matches pending
constexpr size_t OFFBUF        = 0x02c;  // -> offset code per match (u32)
constexpr size_t LENBUF        = 0x030;  // -> literal byte, or match length-2, per token
constexpr size_t FLAGBUF       = 0x034;  // -> one bit per token: set = match
constexpr size_t EMIT_REPS     = 0x038;  // R0, R1, R2 as the block writer sees them
constexpr size_t PARSE_REPS    = 0x044;  // R0, R1, R2 as the parse sees them
constexpr size_t MATCH_OFF     = 0x050;  // [len]: offset code of the best match of each length
constexpr size_t CUR_POS       = 0x458;  // parse position in the window
constexpr size_t SLOT_TABLE    = 0x45c;  // [0x400]: position slot of an offset (or offset>>9)
// Output
constexpr size_t OUT_BASE      = 0x85c;  // -> output buffer
constexpr size_t OUT_PTR       = 0x860;  // -> next output byte
constexpr size_t OUT_END       = 0x864;  // -> output buffer end
constexpr size_t CHUNK_BYTES   = 0x868;  // decoded bytes in the current 32 KB chunk
constexpr size_t BLOCK_START   = 0x86c;  // window position of the pending block
constexpr size_t NUM_SLOTS     = 0x870;  // position slots for this window size
constexpr size_t E8_SIZE       = 0x874;  // Intel E8 translation size (0 = off); u16 halves at
constexpr size_t E8_SIZE_HI    = 0x876;  //   0x874 / 0x876
constexpr size_t CHUNK_SPLITS  = 0x87c;  // adaptive splits taken in the current chunk (u8)
constexpr size_t POPCOUNT      = 0x87d;  // [0x100]: set bits per byte
constexpr size_t BLOCK_PENDING = 0x97d;  // an adaptive block boundary may still be taken (u8)
constexpr size_t STATS_STALE   = 0x980;  // token statistics must be recounted from zero
constexpr size_t FIRST_PARSE   = 0x984;  // nothing parsed yet: the stream header is due
constexpr size_t SLIDE         = 0x988;  // bytes the window slides by
constexpr size_t PARSED        = 0x98c;  // parse position, window-relative
constexpr size_t EMITTED       = 0x990;  // block writer's position in the window
constexpr size_t INPUT_POS     = 0x994;  // next input byte
constexpr size_t INPUT_LEFT    = 0x998;  // input bytes not yet fed
constexpr size_t E8_POS        = 0x99c;  // E8 translation's running position
// Huffman builder scratch
constexpr size_t HF_FREQ       = 0x9a0;  // -> symbol frequencies (u16)
constexpr size_t HF_ORDER      = 0x9a4;  // -> symbols in tree order (u16), cursor
constexpr size_t HF_LENS       = 0x9a8;  // -> code lengths out (u8)
constexpr size_t HF_HEAP       = 0x9ac;  // heap of symbols, 1-indexed (u16)
constexpr size_t HF_CHILD      = 0xf28;  // [node]: left child at +0, right at +2
constexpr size_t HF_BLCOUNT    = 0x2504; // [0..16]: codes per length (u16)
constexpr size_t HF_NUMSYM     = 0x2528; // symbols in the tree (internal nodes number above)
constexpr size_t HF_HEAPSIZE   = 0x252c; // (u16)
constexpr size_t HF_DEPTH      = 0x252e; // depth while counting (u8)
// Statistics and trees
constexpr size_t NEXT_REFRESH  = 0x2530; // token count at which the cost model is refreshed
constexpr size_t STATS_TOKENS  = 0x2534; // tokens already counted into the statistics
constexpr size_t STATS_MATCHES = 0x2538; // matches already counted
constexpr size_t NODES         = 0x253c; // -> optimal-parse nodes
constexpr size_t MAIN_LEN      = 0x2540; // main tree code lengths: 0x100 literals, then 8 per slot
constexpr size_t MAIN_LEN_M    = MAIN_LEN + 0x100;   // ... the match part
constexpr size_t LEN_LEN       = 0x27fd; // length tree code lengths (0xf9)
constexpr size_t MAIN_FREQ     = 0x28f8; // main tree frequencies (u16)
constexpr size_t MAIN_FREQ_M   = MAIN_FREQ + 0x200;
constexpr size_t MAIN_CODE     = 0x33e8; // main tree codes (u16)
constexpr size_t MAIN_CODE_M   = MAIN_CODE + 0x200;
constexpr size_t MAIN_PREV     = 0x3960; // main tree lengths last sent (pretree deltas)
constexpr size_t LEN_FREQ      = 0x3c1e; // length tree frequencies (u16)
constexpr size_t LEN_CODE      = 0x4002; // length tree codes (u16)
constexpr size_t LEN_PREV      = 0x41f4; // length tree lengths last sent
constexpr size_t ALN_FREQ      = 0x42ee; // aligned-offset tree frequencies (u16 x8)
constexpr size_t ALN_CODE      = 0x430e; // aligned-offset tree codes (u16 x8)
constexpr size_t ALN_LEN       = 0x431e; // aligned-offset tree lengths (u8 x8)
// Buffers (where each lives in the arena)
constexpr size_t WINDOW_BUF    = 0x4330; // -> window
constexpr size_t BT_LEFT_BUF   = 0x4334; // -> left-child array
constexpr size_t BT_RIGHT_BUF  = 0x4338; // -> right-child array
constexpr size_t CHUNKS_FED    = 0x433c; // 32 KB chunks fed so far
constexpr size_t CONTEXT_SIZE  = 0x4350;

// Optimal-parse node, one per window position (0x18 bytes)
constexpr int64_t NODE_SIZE = 0x18;
constexpr int64_t N_OFFCODE = 0x00;      // offset code of the step that reaches this position
constexpr int64_t N_FROM    = 0x04;      // position that step starts at
constexpr int64_t N_R0      = 0x08;      // R0..R2 after that step
constexpr int64_t N_R1      = 0x0c;
constexpr int64_t N_R2      = 0x10;
constexpr int64_t N_COST    = 0x14;      // cheapest cost (bits) to reach this position

// ---- arena field accessors -------------------------------------------------
static inline int32_t&  i32(uint8_t* A, size_t o){ return *reinterpret_cast<int32_t*>(A+o); }
static inline uint32_t& u32(uint8_t* A, size_t o){ return *reinterpret_cast<uint32_t*>(A+o); }
static inline int16_t&  i16(uint8_t* A, size_t o){ return *reinterpret_cast<int16_t*>(A+o); }
static inline uint16_t& u16(uint8_t* A, size_t o){ return *reinterpret_cast<uint16_t*>(A+o); }
static inline uint8_t&  u8 (uint8_t* A, size_t o){ return *(A+o); }
static inline int8_t&   i8 (uint8_t* A, size_t o){ return *reinterpret_cast<int8_t*>(A+o); }
// the offset stored at byte `o`, plus `idx`: where it points
static inline size_t P(uint8_t* A, size_t o, int64_t idx=0){
    return static_cast<size_t>(static_cast<int64_t>(i32(A,o)) + idx);
}
// R0..R2 as the parse / the block writer sees them
static inline int32_t& prep(uint8_t* A, uint32_t k){ return i32(A, PARSE_REPS + k*4); }
static inline int32_t& erep(uint8_t* A, uint32_t k){ return i32(A, EMIT_REPS + k*4); }
// offset code of the best match of length `len`
static inline int32_t& match_off(uint8_t* A, int64_t len){ return i32(A, (size_t)(MATCH_OFF + len*4)); }

// Arena byte offsets of the heap sub-buffers (filled by setup()).
struct Layout {
    uint32_t window, btL, btR, lenbuf, offbuf, flagbuf, nodes, outbuf;
    uint32_t ptFreq, ptLen, ptCode;   // pretree Huffman scratch
    uint32_t total;
};

} // namespace

// ---------------------------------------------------------------------------
// Canonical Huffman builder, on the context's HF_* scratch.
namespace {
void huff_sift(uint8_t* a, int16_t node);
void huff_countdepth(uint8_t* a, int16_t node);
void huff_lengths(uint8_t* a, int16_t rootNode);

// sift a node down the min-heap (keyed on frequency, unsigned 16-bit).
void huff_sift(uint8_t* a, int16_t node) {
    uint32_t freq = (uint32_t)i32(a, HF_FREQ);
    int16_t value = i16(a, HF_HEAP + node*2);
    int child = node*2;
    int size = i16(a, HF_HEAPSIZE);
    if (size < child) { i16(a, HF_HEAP + node*2) = value; return; }
    do {
        int pick = child;
        if (child < size &&
            u16(a, freq + (uint16_t)i16(a, HF_HEAP + 2 + child*2)*2) <
            u16(a, freq + (uint16_t)i16(a, HF_HEAP + child*2)*2))
            pick = child + 1;
        int picked = i16(a, HF_HEAP + pick*2);
        if (u16(a, freq + (uint16_t)value*2) <= u16(a, freq + (uint16_t)(int16_t)picked*2)) break;
        i16(a, HF_HEAP + node*2) = (int16_t)picked;
        size = i16(a, HF_HEAPSIZE);
        child = pick*2;
        node = (int16_t)pick;
    } while (child <= size);
    i16(a, HF_HEAP + node*2) = value;
}

// recursively tally leaf depths into the per-length counts.
void huff_countdepth(uint8_t* a, int16_t node) {
    int n = node;
    if (n < i32(a, HF_NUMSYM)) {
        int d = (int8_t)u8(a, HF_DEPTH);
        if ((int8_t)u8(a, HF_DEPTH) > 0xf) d = 0x10;
        i16(a, HF_BLCOUNT + d*2) += 1;
        return;
    }
    u8(a, HF_DEPTH) += 1;
    huff_countdepth(a, i16(a, HF_CHILD + n*4));
    huff_countdepth(a, i16(a, HF_CHILD + 2 + n*4));
    u8(a, HF_DEPTH) -= 1;
}

// derive code lengths from the tree, limited to 16 bits, written to the
// lengths-out buffer in tree order.
void huff_lengths(uint8_t* a, int16_t rootNode) {
    for (uint32_t o = HF_BLCOUNT; o <= HF_BLCOUNT + 0x1c; o += 4) i32(a,o) = 0;
    i16(a, HF_BLCOUNT + 0x20) = 0;
    int root = rootNode;
    if (root < i32(a, HF_NUMSYM)) {
        int d = (int8_t)u8(a, HF_DEPTH);
        if ((int8_t)u8(a, HF_DEPTH) > 0xf) d = 0x10;
        i16(a, HF_BLCOUNT + d*2) += 1;
    } else {
        u8(a, HF_DEPTH) += 1;
        huff_countdepth(a, i16(a, HF_CHILD + root*4));
        huff_countdepth(a, i16(a, HF_CHILD + 2 + root*4));
        u8(a, HF_DEPTH) -= 1;
    }
    // Kraft sum over lengths 16..1, weighted 2^(16-len): non-zero = over-subscribed
    uint8_t shift = 0; int lenSlot = HF_BLCOUNT + 0x20; int8_t bits = 0x10; int16_t excess = 0;
    int p = lenSlot;
    do {
        uint8_t s = shift & 0x1f;
        bits--; shift++;
        excess = (int16_t)(excess + ((int)i16(a, p) << s));
        p -= 2;
    } while (bits != 0);
    while (true) {
        if (excess == 0) {
            bits = 0x10;
            do {
                excess = i16(a, lenSlot);
                while ((excess = (int16_t)(excess - 1)) >= 0) {
                    uint32_t lenBase = (uint32_t)i32(a, HF_LENS);
                    uint16_t sym = u16(a, (uint32_t)i32(a, HF_ORDER));
                    u8(a, lenBase + sym) = (uint8_t)bits;
                    i32(a, HF_ORDER) += 2;
                }
                bits--; lenSlot -= 2;
            } while (bits != 0);
            return;
        }
        // move one code from length 16 down: take a code from the longest
        // non-empty length below 16 and split it in two
        i16(a, lenSlot) -= 1;
        uint32_t len = 0xf;
        uint8_t below;
        do {
            if (i16(a, HF_BLCOUNT + len*2) != 0) {
                i16(a, HF_BLCOUNT + len*2) -= 1;
                i16(a, HF_BLCOUNT + 2 + len*2) += 2;
                break;
            }
            below = (uint8_t)((char)len - 1);
            len = below;
        } while (below != 0);
        excess = (int16_t)(excess - 1);
    }
}

// build the Huffman tree by repeatedly joining the two least frequent.
void huff_treebuild(uint8_t* a, int16_t count, uint32_t freqOff, uint32_t codeOff) {
    for (int16_t s = (int16_t)(i16(a, HF_HEAPSIZE) >> 1); s > 0; s--) huff_sift(a, s);
    i32(a, HF_ORDER) = (int32_t)codeOff;
    int16_t nextNode = count, node = count;
    do {
        node = nextNode;
        uint16_t lo = u16(a, HF_HEAP + 2);                      // least frequent
        if ((int16_t)lo < i32(a, HF_NUMSYM)) {
            u16(a, (uint32_t)i32(a, HF_ORDER)) = lo; i32(a, HF_ORDER) += 2;
        }
        uint16_t last = (uint16_t)i16(a, HF_HEAP + i16(a, HF_HEAPSIZE)*2);
        i16(a, HF_HEAPSIZE) -= 1;
        u16(a, HF_HEAP + 2) = last;
        huff_sift(a, 1);
        int16_t hi = i16(a, HF_HEAP + 2);                       // next least frequent
        if (hi < i32(a, HF_NUMSYM)) {
            u16(a, (uint32_t)i32(a, HF_ORDER)) = (uint16_t)hi; i32(a, HF_ORDER) += 2;
        }
        int n = node;
        u16(a, freqOff + (uint16_t)node*2) =
            (uint16_t)(u16(a, freqOff + (uint16_t)hi*2) + u16(a, freqOff + lo*2));
        i16(a, HF_HEAP + 2) = node;
        huff_sift(a, 1);
        u16(a, HF_CHILD + n*4) = lo;
        i16(a, HF_CHILD + 2 + n*4) = hi;
        nextNode = (int16_t)(node + 1);
    } while (1 < i16(a, HF_HEAPSIZE));
    i32(a, HF_ORDER) = (int32_t)codeOff;
    huff_lengths(a, node);
}

// assign canonical codes from the per-length counts.
void huff_codes(uint8_t* a, int count, uint32_t codeOff, uint32_t lenOff) {
    int16_t nextCode[18]; nextCode[0] = 0; nextCode[1] = 0;
    for (int len = 1; len < 0x11; len++)
        nextCode[len+1] = (int16_t)((nextCode[len] + i16(a, HF_BLCOUNT + len*2)) * 2);
    for (int i = 0; i < count; i++) {
        uint8_t len = u8(a, lenOff + i);
        i16(a, codeOff + i*2) = nextCode[len];
        nextCode[len] = (int16_t)(nextCode[len] + 1);
    }
}

// a whole tree: collect the symbols that occur, build the tree, then the codes.
void huff_build(uint8_t* a, int count, uint32_t freqOff, uint32_t lenOff,
                uint32_t codeOff, int doCodes) {
    while (true) {
        i32(a, HF_LENS) = (int32_t)lenOff;
        int16_t sym = 0; int i = 0;
        i32(a, HF_NUMSYM) = count;
        i32(a, HF_FREQ) = (int32_t)freqOff;
        u8(a, HF_DEPTH) = 0;
        i16(a, HF_HEAPSIZE) = 0;
        i16(a, HF_HEAP + 2) = 0;
        if (count < 1) break;
        do {
            u8(a, lenOff + i) = 0;
            if (u16(a, freqOff + i*2) != 0) {
                i16(a, HF_HEAPSIZE) += 1;
                i16(a, HF_HEAP + i16(a, HF_HEAPSIZE)*2) = sym;
            }
            sym++; i = sym;
        } while (i < count);
        if (1 < i16(a, HF_HEAPSIZE)) {
            huff_treebuild(a, (int16_t)count, freqOff, codeOff);
            if (doCodes != 0) huff_codes(a, count, codeOff, lenOff);
            return;
        }
        if (i16(a, HF_HEAPSIZE) == 0) break;
        // one symbol only: give a second one a count so a tree can be built
        if (i16(a, HF_HEAP + 2) == 0) u16(a, freqOff + 2) = 1;
        else u16(a, freqOff + 0) = 1;
    }
    u16(a, codeOff + (uint16_t)i16(a, HF_HEAP + 2)*2) = 0;
}
} // namespace

// ---------------------------------------------------------------------------
// Binary-tree match finder (insert / search / slide-delete). The window byte at
// position k is arena[WIN_BASE + k]; a node's children are at BT_LEFT / BT_RIGHT
// + k*4. Each tree insert also cuts off nodes at or below a threshold.
namespace {
static inline size_t btL_off(uint8_t* a, int64_t k){ return (size_t)((int64_t)i32(a, BT_LEFT) + k*4); }
static inline size_t btR_off(uint8_t* a, int64_t k){ return (size_t)((int64_t)i32(a, BT_RIGHT) + k*4); }
static inline int32_t& BTL(uint8_t* a, int64_t k){ return i32(a, btL_off(a,k)); }
static inline int32_t& BTR(uint8_t* a, int64_t k){ return i32(a, btR_off(a,k)); }
static inline uint8_t  WIN(uint8_t* a, int64_t k){ return u8(a, (size_t)((int64_t)i32(a, WIN_BASE) + k)); }

// insert position `pos` into the tree, dropping nodes at or below `limit`.
void bt_insert(uint8_t* a, int pos, int limit) {
    int cand = i32(a, BT_HEAD);
    i32(a, BT_HEAD) = pos;
    if (cand <= limit) { BTR(a,pos) = 0; BTL(a,pos) = 0; return; }
    size_t linkL = btL_off(a, pos);
    int skip = 0;                                   // bytes known equal on both sides
    size_t linkR = btR_off(a, pos);
    int greaterLen = 0, lessLen = 0;
    while (true) {
        int cp = skip + cand;
        int diff = (int)WIN(a, cp) - (int)WIN(a, (int64_t)skip + pos);
        int len = skip;
        if (diff == 0) {
            int pp = (cp - cand) + pos;
            do {
                len++; pp++;
                if (0x31 < len) break;
                diff = (int)WIN(a, (int64_t)(cp - skip) + len) - (int)WIN(a, pp);
            } while (diff == 0);
        }
        if (diff < 0) {
            if (lessLen < len) {
                if (0x31 < len) { i32(a,linkL)=BTL(a,cand); i32(a,linkR)=BTR(a,cand); return; }
                skip = greaterLen; lessLen = len;
                if (len <= greaterLen) skip = len;
            }
            i32(a, linkR) = cand;
            linkR = btL_off(a, cand);
            cand = i32(a, linkR);
        } else {
            if (greaterLen < len) {
                if (0x31 < len) { i32(a,linkL)=BTL(a,cand); i32(a,linkR)=BTR(a,cand); return; }
                skip = len; greaterLen = len;
                if (lessLen <= len) skip = lessLen;
            }
            i32(a, linkL) = cand;
            linkL = btR_off(a, cand);
            cand = i32(a, linkL);
        }
        if (cand <= limit) { i32(a,linkL)=0; i32(a,linkR)=0; return; }
    }
}

// insert `pos` and find its matches: MATCH_OFF[len] receives the offset code
// of the nearest match of each length found; returns the longest length,
// capped so a match does not cross the 32 KB frame (0 = no usable match).
int bt_search(uint8_t* a, uint32_t pos) {
    uint32_t cand = (uint32_t)i32(a, BT_HEAD);
    uint32_t limit = (pos - (uint32_t)i32(a, WINDOW)) + 4;
    i32(a, BT_HEAD) = (int32_t)pos;
    if (cand <= limit) { BTR(a,pos)=0; BTL(a,pos)=0; return 0; }
    size_t linkL = btL_off(a, pos);
    int best = 0, skip = 0;
    size_t linkR = btR_off(a, pos);
    int greaterLen = 0, lessLen = 0;
    bool done = false;
    while (true) {
        int cp = (int)cand + skip;
        int diff = (int)WIN(a, cp) - (int)WIN(a, (int64_t)pos + skip);
        int len = skip;
        bool longer = false;                        // a new longest match: record it
        if (diff == 0) {
            int pp = (cp - (int)cand) + (int)pos;
            do {
                len++; pp++;
                if (0x100 < len) { longer = true; break; }
                diff = (int)WIN(a, (int64_t)(cp - skip) + len) - (int)WIN(a, pp);
            } while (diff == 0);
        }
        if (!longer && diff < 0) {
            if (lessLen < len) {
                if (best < len) longer = true;
                else { skip = greaterLen; lessLen = len; if (len <= greaterLen) skip = len; }
            }
            if (!longer) {
                i32(a, linkR) = (int32_t)cand;
                linkR = btL_off(a, cand);
                cand = (uint32_t)i32(a, linkR);
            }
        } else if (!longer) {  // diff >= 0
            if (greaterLen < len) {
                if (best < len) {
                    size_t slot = MATCH_OFF + (size_t)best*4;
                    do { best++; slot += 4; i32(a,slot) = (int32_t)((pos - cand) + 2); }
                    while (best < len);
                    if (0x31 < len) {
                        i32(a, linkL) = BTL(a, cand);
                        i32(a, linkR) = BTR(a, cand);
                        done = true;
                    }
                }
                if (!done) { skip = lessLen; greaterLen = len; if (len < lessLen) skip = len; }
            }
            if (!done) {
                i32(a, linkL) = (int32_t)cand;
                linkL = btR_off(a, cand);
                cand = (uint32_t)BTR(a, cand);
            }
        }
        if (longer) {
            size_t slot = MATCH_OFF + (size_t)best*4;
            do { best++; slot += 4; i32(a,slot) = (int32_t)((pos - cand) + 2); }
            while (best < len);
            if (0x31 < len) {
                i32(a, linkL) = BTL(a, cand);
                i32(a, linkR) = BTR(a, cand);
                done = true;
            } else {
                skip = greaterLen; lessLen = len; if (len <= greaterLen) skip = len;
                i32(a, linkR) = (int32_t)cand;
                linkR = btL_off(a, cand);
                cand = (uint32_t)i32(a, linkR);
            }
        }
        if (done) break;
        if (!(limit < cand)) { i32(a,linkL)=0; i32(a,linkR)=0; break; }
    }
    // a repeated offset (R0, R1, R2) is cheaper: let it take the lengths it covers
    if (best < 2) return 0;
    int r0Len = 0;
    if (0 < best) {
        int64_t from = (int64_t)pos - prep(a,0);
        while (WIN(a, (int64_t)pos + r0Len) == WIN(a, from + r0Len)) { r0Len++; if (!(r0Len < best)) break; }
    }
    if (1 < r0Len) {
        size_t slot = MATCH_OFF + (size_t)r0Len*4;
        int k = r0Len;
        do { i32(a,slot)=0; k--; slot-=4; } while (1 < k);
        if (0x32 < r0Len) goto capped;
    }
    {
        int repLen = 0;
        if (0 < best) {
            int64_t from = (int64_t)pos - prep(a,1);
            while (WIN(a,(int64_t)pos+repLen) == WIN(a,from+repLen)) { repLen++; if (!(repLen<best)) break; }
        }
        if (r0Len < repLen) {
            size_t slot = MATCH_OFF + (size_t)r0Len*4;
            do { r0Len++; slot+=4; i32(a,slot)=1; } while (r0Len < repLen);
        }
        repLen = 0;
        if (0 < best) {
            int64_t from = (int64_t)pos - prep(a,2);
            while (WIN(a,(int64_t)pos+repLen) == WIN(a,from+repLen)) { repLen++; if (!(repLen<best)) break; }
        }
        if (r0Len < repLen) {
            size_t slot = MATCH_OFF + (size_t)r0Len*4;
            do { r0Len++; slot+=4; i32(a,slot)=2; } while (r0Len < repLen);
        }
    }
capped:
    int room = 0x7fff - (int)(pos & 0x7fff);
    if (best <= room) return best;
    if (room < 2) return 0;
    return room;
}

// unlink `pos` (the newest node) when the nodes below `thresh` slide out.
void bt_slide_delete(uint8_t* a, uint32_t pos, uint32_t thresh) {
    size_t link = BT_HEAD;
    if ((uint32_t)i32(a, BT_HEAD) != pos) return;
    if ((uint32_t)i32(a, BT_HEAD) <= thresh) { i32(a, BT_HEAD)=0; BTR(a,pos)=0; BTL(a,pos)=0; return; }
    uint32_t l = (uint32_t)BTL(a,pos); if (l <= thresh) { BTL(a,pos)=0; l=0; }
    uint32_t r = (uint32_t)BTR(a,pos); if (r <= thresh) { BTR(a,pos)=0; r=0; }
    while (true) {
        for (; l <= r; r = (uint32_t)BTL(a, r)) {
            if (r <= thresh) r = 0;
            i32(a, link) = (int32_t)r;
            if (r == 0) return;
            link = btL_off(a, r);
        }
        if (l <= thresh) l = 0;
        i32(a, link) = (int32_t)l;
        if (l == 0) break;
        link = btR_off(a, l);
        l = (uint32_t)i32(a, link);
    }
}
} // namespace

static inline uint32_t off_slot(uint8_t* a, uint32_t off);  // defined below

// The encoder: owns the arena and the assembled output stream.
struct LzxEnc {
    std::vector<uint8_t> arena;
    Layout L{};
    std::vector<uint8_t> output;          // assembled compressed chunks
    std::vector<uint8_t> input;           // the data being compressed
    uint8_t* A() { return arena.data(); }

    // ---- write `nbits` bits of `val` ---------------------------------------
    void write_bits(uint32_t val, int8_t nbits) {
        uint8_t* a = A();
        int8_t free = (int8_t)(i8(a, BITFREE) - nbits);
        u32(a, BITBUF) |= val << ((i8(a, BITFREE) - nbits) & 0x1f);
        i8(a, BITFREE) = free;
        while (free < 0x11) {
            if (u32(a, OUT_END) <= u32(a, OUT_PTR)) {  // wrap
                i32(a, OUT_OVERFLOW) = 1;
                i32(a, OUT_PTR) = i32(a, OUT_BASE);
            }
            u8(a, P(a, OUT_PTR)) = u8(a, BITBUF_B2);
            i32(a, OUT_PTR) += 1;
            u8(a, P(a, OUT_PTR)) = u8(a, BITBUF_B3);
            i32(a, OUT_PTR) += 1;
            i32(a, BITBUF) <<= 0x10;
            i8(a, BITFREE) += 0x10;
            free = i8(a, BITFREE);
        }
    }

    // ---- offset -> position-slot table ---------------------------------------
    void build_slot_table() {
        uint8_t* a = A();
        u8(a,SLOT_TABLE)=0; u8(a,SLOT_TABLE+1)=1; u8(a,SLOT_TABLE+2)=2; u8(a,SLOT_TABLE+3)=3;
        int size = 2, pos = 4; uint8_t val = 4;
        do {
            std::memset(a+SLOT_TABLE+pos, val, size); pos += size;
            uint8_t cur = val; val = (uint8_t)(val + 1);
            std::memset(a+SLOT_TABLE+pos, val, size); pos += size;
            val = (uint8_t)(cur + 2);
            size *= 2;
        } while (pos < 0x400);
    }

    // ---- per-run state + default tree lengths --------------------------------
    void init_run() {
        uint8_t* a = A();
        int32_t win = i32(a, WINDOW);
        i32(a, WIN_BASE)    = i32(a, WINDOW_BUF) - win;
        i32(a, BT_LEFT)     = i32(a, BT_LEFT_BUF) - win*4;
        i32(a, CUR_POS)     = win;
        i32(a, BLOCK_START) = win;
        i32(a, BT_RIGHT)    = i32(a, BT_RIGHT_BUF) - win*4;
        i32(a, BT_HEAD)     = 0;
        prep(a,0)=1; prep(a,1)=1; prep(a,2)=1;
        erep(a,0)=1; erep(a,1)=1; erep(a,2)=1;
        u8(a, BLOCK_PENDING) = 1;
        i32(a, STATS_STALE) = 1;
        u8(a, BITFREE) = 0x20;
        i32(a, BITBUF)=0; i32(a, OUT_OVERFLOW)=0;
        std::memset(a+MAIN_PREV, 0, (size_t)i32(a,NUM_SLOTS)*8 + 0x100);
        std::memset(a+LEN_PREV, 0, 0xf9);
        std::memset(a+MAIN_LEN, 8, 0x100);       // literals: 8 bits
        std::memset(a+MAIN_LEN_M, 9, (size_t)i32(a,NUM_SLOTS)*8);   // matches: 9
        std::memset(a+LEN_LEN, 6, 0xf9);         // lengths: 6
        i32(a,ALN_LEN) = 0x03030303;             // aligned offsets: 3
        i32(a,ALN_LEN+4) = 0x03030303;
        uint32_t slot = u8(a, SLOT_TABLE+0x100) + 0x12;   // slots beyond the window: never
        if (slot < (uint32_t)i32(a,NUM_SLOTS)) {
            uint32_t off = MAIN_LEN_M + slot*8;
            do { u8(a,off)=100; slot++; off+=8; } while (slot < (uint32_t)i32(a,NUM_SLOTS));
        }
        i32(a,EMITTED)=i32(a,CUR_POS); i32(a,PARSED)=i32(a,CUR_POS);
        i32(a,CHUNK_BYTES)=0; i32(a,FIRST_PARSE)=1;
        std::memset(a + P(a,FLAGBUF), 0, 0x2000);
        i32(a,TOKENS)=0; i32(a,MATCHES)=0; u8(a,CHUNK_SPLITS)=0;
        erep(a,0)=1; erep(a,1)=1; erep(a,2)=1;
        i32(a,E8_POS)=0; i32(a,CHUNKS_FED)=0;
        std::memset(a+MAIN_FREQ, 0, 0xaf0);
        std::memset(a+LEN_FREQ, 0, 0x3e4);
        for (uint32_t o = ALN_FREQ; o <= ALN_FREQ + 0x1c; o += 4) i32(a,o)=0;
        std::memset(a + P(a,WINDOW_BUF), 0, (size_t)i32(a,SLIDE) + 0x1101 + i32(a,WINDOW));
    }

    // ---- allocate + init -----------------------------------------------------
    void setup(uint32_t window, uint32_t e8size) {
        if (window & 0x7fff) window &= 0xffff8000u;
        if ((int)window < 0x8000) window = 0x8000;
        uint32_t W = window, C = window;
        // number of position slots
        int32_t slots = 4; { uint32_t acc = 4;
            do { uint8_t eb = kExtraBits[slots]; slots++; acc += (1u << eb); } while (acc < W); }
        // arena layout
        uint32_t off = CONTEXT_SIZE;
        auto place = [&](uint32_t sz){ off=(off+15)&~15u; uint32_t p=off; off+=sz; return p; };
        L.window  = place(C + 0x1101 + W);
        L.btL     = place((C+W)*4 + 0x4404);
        L.btR     = place((C+W)*4 + 0x4404);
        L.lenbuf  = place(0x10000);
        L.offbuf  = place(0x20000);
        L.flagbuf = place(0x2000);
        L.nodes   = place(0x18150);
        L.outbuf  = place(0x9800);
        L.ptFreq  = place(0x60);   // 20 syms + internal nodes (shorts)
        L.ptLen   = place(0x20);   // 20 pretree code lengths
        L.ptCode  = place(0x40);   // 20 pretree codes (shorts)
        L.total   = (off+15)&~15u;
        arena.assign(L.total, 0);
        uint8_t* a = A();
        i32(a,WINDOW)    = W;
        i32(a,SLIDE)     = C;
        i32(a,NUM_SLOTS) = slots;
        i32(a,E8_SIZE)   = (int32_t)e8size;
        i32(a,WINDOW_BUF) = L.window;  i32(a,BT_LEFT_BUF) = L.btL;  i32(a,BT_RIGHT_BUF) = L.btR;
        i32(a,OFFBUF) = L.offbuf;  i32(a,LENBUF) = L.lenbuf; i32(a,FLAGBUF) = L.flagbuf;
        i32(a,NODES)  = L.nodes;
        i32(a,OUT_BASE) = L.outbuf;  i32(a,OUT_PTR) = L.outbuf; i32(a,OUT_END) = L.outbuf + 0x97c0;
        build_slot_table();
        for (uint32_t v = 0; v < 0x100; ++v) {
            uint8_t c = 0; for (uint32_t x = v; x; x >>= 1) c += (x & 1);
            u8(a, POPCOUNT + v) = c;
        }
        init_run();
    }

    // ---- defined below --------------------------------------------------------
    void parse(int len);
    void refresh_costs();
    void flush_pending(uint32_t pos);
    int  block_boundary(uint32_t* pos);
    void build_emit_block();
    void emit_one_block(uint32_t tokens, uint32_t matches);
    int  count_tokens(uint32_t offIdx, uint32_t from, uint32_t to);
    void build_codes(int doMain);
    void fill_default_lengths();
    int  choose_block_type(uint32_t matches);
    void count_aligned(uint32_t matches);
    uint32_t compressed_size();
    void update_repeats(uint32_t matches);
    void store_uncompressed(uint32_t from, uint32_t len);
    void emit_tokens_verbatim(uint32_t tokens);
    void emit_tokens_aligned(uint32_t tokens);
    void pretree_encode(uint32_t srcLen, uint32_t count, uint32_t prevLen);
    void emit_aligned_tree();
    void emit_pretrees();
    void find_split(uint32_t* outTok, uint32_t tokens, uint32_t matches, uint32_t* outOff);
    uint32_t eval_split(uint32_t tok1, uint32_t tok2, uint32_t off1, uint32_t off2);
    void e8_translate(int len, uint32_t dst);
    int  feed_window(uint32_t dst);
    void feed_chunk();
    void chunk_flush();
    void out_chunk(uint32_t srcOff, int complen);
};

// ===== statistics ==========================================================
// count tokens [from,to) into the tree frequencies; offIdx is the
// offset-buffer index at `from`. Returns the bytes those tokens decode to.
int LzxEnc::count_tokens(uint32_t offIdx, uint32_t from, uint32_t to) {
    uint8_t* a = A();
    int bytes = 0;
    uint32_t bit = from & 7;
    uint8_t mask = (uint8_t)((1u << bit) | (1u >> (8 - bit)));
    if (from < to) {
        int offPos = (int)offIdx * 4;
        do {
            if ((u8(a, (from >> 3) + (uint32_t)i32(a,FLAGBUF)) & mask) == 0) {
                uint32_t lit = u8(a, (uint32_t)i32(a,LENBUF) + from);
                i16(a, MAIN_FREQ + lit*2) += 1;
                bytes += 1;
            } else {
                uint32_t offset = (uint32_t)i32(a, (uint32_t)i32(a,OFFBUF) + offPos);
                uint32_t lenHdr = u8(a, from + (uint32_t)i32(a,LENBUF));
                uint32_t slot = off_slot(a, offset);
                if (lenHdr < 7) {
                    i16(a, MAIN_FREQ_M + (lenHdr + slot*8)*2) += 1;
                } else {
                    i16(a, slot*0x10 + MAIN_FREQ_M + 0xe) += 1;
                    i16(a, LEN_FREQ - 0xe + lenHdr*2) += 1;
                }
                bytes = (int)lenHdr + 2 + bytes;
                offPos += 4;
            }
            from += 1;
            mask = (uint8_t)((mask << 1) | ((mask & 0x80) ? 1 : 0));
        } while (from < to);
    }
    return bytes;
}

// build the three Huffman trees from the frequencies.
void LzxEnc::build_codes(int doMain) {
    uint8_t* a = A();
    int mainSyms = i32(a,NUM_SLOTS) * 8 + 0x100;
    huff_build(a, mainSyms, MAIN_FREQ, MAIN_LEN, MAIN_CODE, doMain);
    huff_build(a, 0xf9,     LEN_FREQ,  LEN_LEN,  LEN_CODE,  doMain);
    huff_build(a, 8,        ALN_FREQ,  ALN_LEN,  ALN_CODE,  1);
}

// give symbols that did not occur a default length, so the cost model can price them.
void LzxEnc::fill_default_lengths() {
    uint8_t* a = A();
    uint32_t i = 0;
    do { if (i8(a, MAIN_LEN + i) == 0) u8(a, MAIN_LEN + i) = 0xb; i++; } while (i < 0x100);
    uint32_t mainSyms = (uint32_t)(i32(a,NUM_SLOTS) * 8 + 0x100);
    if (i < mainSyms) {
        do { if (i8(a, MAIN_LEN + i) == 0) u8(a, MAIN_LEN + i) = 0xc; i++; } while (i < mainSyms);
    }
    uint32_t off = LEN_LEN; int left = 0xf9;
    do { if (i8(a, off) == 0) u8(a, off) = 8; off++; left--; } while (left != 0);
    i = u8(a, SLOT_TABLE+0x100) + 0x12;
    if (i < (uint32_t)i32(a,NUM_SLOTS)) {
        uint32_t p = MAIN_LEN_M + i*8;
        do { u8(a, p) = 100; i++; p += 8; } while (i < (uint32_t)i32(a,NUM_SLOTS));
    }
}

// refresh the adaptive cost model from the tokens parsed so far.
void LzxEnc::refresh_costs() {
    uint8_t* a = A();
    uint32_t tokens = (uint32_t)i32(a,TOKENS);
    if (tokens != 0) {
        if (i32(a,STATS_STALE) == 0) {
            count_tokens((uint32_t)i32(a,STATS_MATCHES), (uint32_t)i32(a,STATS_TOKENS), tokens);
        } else {
            std::memset(a + MAIN_FREQ, 0, (size_t)(i32(a,NUM_SLOTS) + 0x20) * 0x10);
            std::memset(a + LEN_FREQ, 0, 0x1f2);
            count_tokens(0, 0, tokens);
            i32(a,STATS_STALE) = 0;
        }
        build_codes(0);
        fill_default_lengths();
        i32(a,STATS_TOKENS) = i32(a,TOKENS);
        i32(a,STATS_MATCHES) = i32(a,MATCHES);
    }
}

// ===== blocks ==============================================================
// write a block out of the pending tokens.
void LzxEnc::flush_pending(uint32_t pos) {
    uint8_t* a = A();
    u8(a,BLOCK_PENDING) = 0;
    i32(a,STATS_STALE) = 1;
    build_emit_block();
    i32(a,BLOCK_START) = (int32_t)pos;
    if ((uint32_t)i32(a,TOKENS) < 0x1000) i32(a,NEXT_REFRESH) = 0x1000;
    else i32(a,NEXT_REFRESH) = i32(a,TOKENS) + 0x1000;
}

// take an adaptive block boundary if the statistics have shifted: recount up
// to the split, reset the pending tokens and rewind the parse to the block start.
int LzxEnc::block_boundary(uint32_t* pos) {
    uint8_t* a = A();
    uint32_t back = (uint32_t)i32(a,WINDOW);
    int start = i32(a,BLOCK_START);
    int cur = (int)*pos;
    u8(a,BLOCK_PENDING) = 0;
    if (start - back < back) back = (uint32_t)(start - (int)back);
    if ((int)(i32(a,WIN_BASE) - i32(a,WINDOW_BUF) + cur) < (int)((cur - start) + (int)back)) return 0;
    uint32_t split;
    find_split(&split, (uint32_t)i32(a,TOKENS), (uint32_t)i32(a,MATCHES), nullptr);
    std::memset(a + MAIN_FREQ, 0, (size_t)(i32(a,NUM_SLOTS) + 0x20) * 0x10);
    std::memset(a + LEN_FREQ, 0, 0x1f2);
    count_tokens(0, 0, split);
    build_codes(0);
    fill_default_lengths();
    i32(a,BT_HEAD) = 0;
    std::memset(a + P(a,FLAGBUF), 0, 0x2000);
    *pos = (uint32_t)start;
    i32(a,CHUNK_BYTES) = 0; i32(a,TOKENS) = 0; i32(a,MATCHES) = 0;
    prep(a,0) = 1; prep(a,1) = 1; prep(a,2) = 1;
    erep(a,0) = 1; erep(a,1) = 1; erep(a,2) = 1;
    i32(a,STATS_STALE) = 1;
    i32(a,NEXT_REFRESH) = (int32_t)split;
    return 1;
}

// split the pending tokens, write the first part as a block, keep the rest.
void LzxEnc::build_emit_block() {
    uint8_t* a = A();
    u8(a,BLOCK_PENDING) = 0;
    uint32_t tokens, matches;
    find_split(&tokens, (uint32_t)i32(a,TOKENS), (uint32_t)i32(a,MATCHES), &matches);
    emit_one_block(tokens, matches);
    if (tokens == (uint32_t)i32(a,TOKENS)) {
        std::memset(a + P(a,FLAGBUF), 0, 0x2000);
        i32(a,TOKENS) = 0; i32(a,MATCHES) = 0;
        fill_default_lengths();
        return;
    }
    std::memmove(a + P(a,FLAGBUF), a + (P(a,FLAGBUF) + (tokens >> 3)),
                 (((uint32_t)i32(a,TOKENS) >> 3) - (tokens >> 3)) + 1);
    uint32_t kept = ((uint32_t)i32(a,TOKENS) - tokens) >> 3;
    std::memset(a + (kept + 1 + P(a,FLAGBUF)), 0, 0x1fff - kept);
    std::memmove(a + P(a,LENBUF), a + (P(a,LENBUF) + tokens), (uint32_t)i32(a,TOKENS) - tokens);
    std::memmove(a + P(a,OFFBUF), a + (P(a,OFFBUF) + (size_t)matches * 4),
                 (size_t)((uint32_t)i32(a,MATCHES) - matches) * 4);
    i32(a,TOKENS) = (int32_t)((uint32_t)i32(a,TOKENS) - tokens);
    i32(a,MATCHES) = (int32_t)((uint32_t)i32(a,MATCHES) - matches);
    fill_default_lengths();
}

// write one LZX block: verbatim (1), aligned-offset (2) or uncompressed (3).
void LzxEnc::emit_one_block(uint32_t tokens, uint32_t matches) {
    uint8_t* a = A();
    std::memset(a + MAIN_FREQ, 0, (size_t)(i32(a,NUM_SLOTS) + 0x20) * 0x10);
    std::memset(a + LEN_FREQ, 0, 0x1f2);
    uint32_t bytes = (uint32_t)count_tokens(0, 0, tokens);
    uint32_t type = (uint32_t)choose_block_type(matches);
    build_codes(1);
    uint32_t cost = compressed_size();
    if (bytes <= cost && (uint32_t)i32(a,PARSED) <= (uint32_t)i32(a,EMITTED)) type = 3;
    write_bits(type & 0xff, 3);
    write_bits((bytes >> 0x10) & 0xff, 8);
    write_bits((bytes >> 8) & 0xff, 8);
    write_bits(bytes & 0xff, 8);
    if (type == 1) {
        emit_pretrees();
        emit_tokens_verbatim(tokens);
        update_repeats(matches);
        i32(a,EMITTED) += (int32_t)bytes;
        return;
    }
    if (type == 2) {
        emit_aligned_tree();
        emit_pretrees();
        emit_tokens_aligned(tokens);
        update_repeats(matches);
        i32(a,EMITTED) += (int32_t)bytes;
        return;
    }
    if (type == 3) {
        update_repeats(matches);
        store_uncompressed((uint32_t)i32(a,EMITTED), bytes);
    }
    i32(a,EMITTED) += (int32_t)bytes;
}

// position slot of an offset code.
static inline uint32_t off_slot(uint8_t* a, uint32_t off) {
    if (off < 0x400) return u8(a, SLOT_TABLE + off);
    if (off < 0x80000) return (uint8_t)(u8(a, SLOT_TABLE + (off >> 9)) + 0x12);
    return (uint8_t)((off >> 0x11) + 0x22);
}

// pretree symbol for the length change at index i between the last-sent and current tree.
static inline uint8_t delta_sym(uint8_t* a, uint32_t prevOff, uint32_t curOff, uint32_t i) {
    return kDelta[16 + (int)u8(a, prevOff + i) - (int)u8(a, curOff + i)];
}

// tally the aligned-offset tree's symbols (an offset's low 3 bits).
void LzxEnc::count_aligned(uint32_t matches) {
    uint8_t* a = A();
    uint32_t off = P(a,OFFBUF);
    uint32_t n = matches;
    if (n != 0) {
        do {
            uint32_t offset = (uint32_t)i32(a, off);
            off += 4;
            if (0xf < offset) i16(a, ALN_FREQ + (offset & 7)*2) += 1;
            n--;
        } while (n != 0);
    }
}

// verbatim (1), or aligned-offset (2) when the offsets' low bits are skewed enough.
int LzxEnc::choose_block_type(uint32_t matches) {
    uint8_t* a = A();
    i16(a,ALN_FREQ)=0; i16(a,ALN_FREQ+2)=0;
    i32(a,ALN_FREQ+4)=0; i32(a,ALN_FREQ+8)=0; i32(a,ALN_FREQ+0xc)=0;
    uint32_t total=0, peak=0;
    count_aligned(matches);
    for (int i = 0; i < 8; i++) {
        uint32_t f = u16(a, ALN_FREQ + i*2);
        if (peak < f) peak = f;
        total += f;
    }
    return (peak <= total/5 || matches < 100) ? 1 : 2;
}

// send one tree's code lengths through a pretree (run lengths + deltas against
// the lengths last sent), then remember them as last sent.
void LzxEnc::pretree_encode(uint32_t srcLen, uint32_t count, uint32_t prevLen) {
    uint8_t* a = A();
    const uint32_t fF = L.ptFreq, fL = L.ptLen, fC = L.ptCode;
    std::memset(a + fF, 0, 0x60);
    uint8_t saved = u8(a, srcLen + count);
    u8(a, srcLen + count) = 0x7b;                       // sentinel ends every run
    int i = 0;
    uint8_t sym;
    if (0 < (int)count) {                               // pass 1: pretree frequencies
        do {
            char v = i8(a, srcLen + i);
            int run = 0;
            uint32_t pc = srcLen + i + 1;
            if (i8(a, pc) == v) {
                do { pc++; run++; } while (i8(a, pc) == v);
                if (run < 4) goto count_single;
                if (v == 0) {
                    if (run < 0x34) {
                        if (run < 0x14) { i16(a, fF + 17*2) += 1; i = i - 1 + run; goto count_next; }
                    } else { run = 0x33; }
                    i16(a, fF + 18*2) += 1; i = i - 1 + run;
                } else {
                    if (5 < run) run = 5;
                    sym = delta_sym(a, prevLen, srcLen, i);
                    i16(a, fF + sym*2) += 1;
                    i16(a, fF + 19*2) += 1;
                    i = i - 1 + run;
                }
            } else {
count_single:
                sym = delta_sym(a, prevLen, srcLen, i);
                i16(a, fF + sym*2) += 1;
            }
count_next:
            i++;
        } while (i < (int)count);
    }
    huff_build(a, 0x14, fF, fL, fC, 1);
    for (int k = 0; k < 0x14; k++) write_bits(u8(a, fL + k), 4);
    i = 0;
    if (0 < (int)count) {                               // pass 2: write
        do {
            uint8_t b = u8(a, srcLen + i);
            uint32_t pb = srcLen + i + 1;
            int run = 0;
            char code;
            if (u8(a, pb) == b) {
                do { pb++; run++; } while (u8(a, pb) == b);
                if (run < 4) goto write_single;
                if (b == 0) {
                    if (run < 0x34) {
                        if (run < 0x14) { code = 0x11; goto write_code; }
                    } else { run = 0x33; }
                    code = 0x12;
                } else {
                    if (5 < run) run = 5;
                    code = 0x13;
                }
            } else {
write_single:
                code = (char)delta_sym(a, prevLen, srcLen, i);
            }
write_code:
            write_bits(u16(a, fC + (uint8_t)code*2), u8(a, fL + (uint8_t)code));
            if (code == 0x11) {                         // 4..19 zeros
                write_bits(run - 4, 4); i = i - 1 + run;
            } else if (code == 0x12) {                  // 20..51 zeros
                write_bits(run - 0x14, 5); i = i - 1 + run;
            } else if (code == 0x13) {                  // 4..5 of the same length
                write_bits(run - 4, 1);
                uint8_t dm = delta_sym(a, prevLen, srcLen, i);
                write_bits(u16(a, fC + dm*2), u8(a, fL + dm));
                i = i - 1 + run;
            }
            i++;
        } while (i < (int)count);
    }
    u8(a, srcLen + count) = saved;
    std::memcpy(a + prevLen, a + srcLen, count);
}

// the main tree (literals, then matches) and the length tree, through pretrees.
void LzxEnc::emit_pretrees() {
    uint8_t* a = A();
    pretree_encode(MAIN_LEN, 0x100, MAIN_PREV);
    pretree_encode(MAIN_LEN_M, (uint32_t)i32(a,NUM_SLOTS) * 8, MAIN_PREV + 0x100);
    pretree_encode(LEN_LEN, 0xf9, LEN_PREV);
}

// the aligned-offset tree: 8 symbols, 3 bits each.
void LzxEnc::emit_aligned_tree() {
    uint8_t* a = A();
    huff_build(a, 8, ALN_FREQ, ALN_LEN, ALN_CODE, 1);
    for (int i = 0; i < 8; i++) write_bits(u8(a, ALN_LEN + i), 3);
}

// Output sink: each 32 KB decoded chunk is stored as a 2-byte big-endian
// compressed length followed by that many bytes.
void LzxEnc::out_chunk(uint32_t srcOff, int complen) {
    uint8_t* a = A();
    output.push_back((uint8_t)((uint32_t)complen >> 8));
    output.push_back((uint8_t)complen);
    output.insert(output.end(), a + srcOff, a + srcOff + complen);
}

// ===== input ===============================================================
// feed the next chunk into the window, E8-translate it if asked, parse it.
void LzxEnc::feed_chunk() {
    uint8_t* a = A();
    int buf = i32(a,WINDOW_BUF);
    int pos = i32(a,CUR_POS);
    int base = i32(a,WIN_BASE);
    uint32_t dest = (uint32_t)(buf + (pos - buf) + base);   // = window base + pos
    int n = feed_window(dest);
    if (-1 < n) {
        if (i32(a,E8_SIZE) != 0 && (uint32_t)i32(a,CHUNKS_FED) < 0x8000)
            e8_translate(n, dest);
        i32(a,CHUNKS_FED) += 1;
        if (0 < n) parse(n);
    }
}

// copy the next ≤0x8000 input bytes into the window at `dst`.
int LzxEnc::feed_window(uint32_t dst) {
    uint8_t* a = A();
    uint32_t sz = (uint32_t)i32(a,INPUT_LEFT);
    if (0x7fff < (int)sz) {
        std::memcpy(a + dst, input.data() + (uint32_t)i32(a,INPUT_POS), 0x8000);
        i32(a,INPUT_LEFT) -= 0x8000;
        i32(a,INPUT_POS) += 0x8000;
        return 0x8000;
    }
    if ((int)sz < 1) return 0;
    std::memcpy(a + dst, input.data() + (uint32_t)i32(a,INPUT_POS), sz);
    i32(a,INPUT_POS) += i32(a,INPUT_LEFT);
    i32(a,INPUT_LEFT) = 0;
    return (int)sz;
}

// Intel E8 call translation at compress time (call operand -> absolute).
void LzxEnc::e8_translate(int len, uint32_t dst) {
    uint8_t* a = A();
    if (6 < len) {
        uint32_t tail4 = (uint32_t)i32(a, dst + len - 6);
        uint16_t tail2 = u16(a, dst + len - 2);
        for (int k = 0; k < 6; k++) u8(a, dst + len - 6 + k) = 0xe8;  // sentinel
        uint32_t stop = (uint32_t)i32(a,E8_POS) - 10 + len;
        uint32_t p = dst;
        while (true) {
            char c = i8(a, p);
            while (c != (char)0xe8) { i32(a,E8_POS) += 1; p++; c = i8(a, p); }
            uint32_t at = (uint32_t)i32(a,E8_POS);
            if (stop <= at) break;
            int rel = i32(a, p + 1);
            uint32_t abs = at + (uint32_t)rel;
            uint32_t size;
            if ((-1 < (int)abs) && (size = (uint32_t)i32(a,E8_SIZE), abs < at + size)) {
                if (size <= abs) abs = (uint32_t)rel - size;
                u8(a, p+2) = (uint8_t)(abs >> 8);
                u8(a, p+1) = (uint8_t)abs;
                u8(a, p+3) = (uint8_t)(abs >> 0x10);
                u8(a, p+4) = (uint8_t)(abs >> 0x18);
            }
            p += 5; i32(a,E8_POS) += 5;
        }
        i32(a, dst + len - 6) = (int32_t)tail4;
        u16(a, dst + len - 2) = tail2;
        i32(a,E8_POS) = (int32_t)(stop + 10);
    } else {
        i32(a,E8_POS) += len;
    }
}

// ===== block splitting =====================================================
// how different the symbol statistics of two 0x400-token windows are.
uint32_t LzxEnc::eval_split(uint32_t tok1, uint32_t tok2, uint32_t off1, uint32_t off2) {
    uint8_t* a = A();
    int groups = i32(a,NUM_SLOTS) + 0x20;
    uint32_t syms = (uint32_t)groups * 8;
    if (799 < syms) return 0;
    int16_t h1[800] = {0}, h2[800] = {0};
    uint32_t c1 = tok1, c2 = tok2;
    int o1 = (int)off1 * 4, o2 = (int)off2 * 4;
    uint8_t m1 = (uint8_t)(1u << (tok1 & 7));
    uint8_t m2 = (uint8_t)(1u << (tok2 & 7));
    int n = 0x400;
    do {
        uint32_t s1;
        if ((u8(a, (c1 >> 3) + (uint32_t)i32(a,FLAGBUF)) & m1) == 0) s1 = u8(a, (uint32_t)i32(a,LENBUF) + c1);
        else {
            uint8_t b = u8(a, (uint32_t)i32(a,LENBUF) + c1);
            uint32_t off = (uint32_t)i32(a, o1 + (uint32_t)i32(a,OFFBUF));
            uint32_t slot = off_slot(a, off);
            s1 = (b < 7) ? (b + 0x100 + slot*8) : (slot*8 + 0x107);
            o1 += 4;
        }
        h1[s1]++;
        c1++; m1 = (uint8_t)((m1 << 1) | ((m1 & 0x80) ? 1 : 0));
        uint32_t s2;
        if ((u8(a, (c2 >> 3) + (uint32_t)i32(a,FLAGBUF)) & m2) == 0) s2 = u8(a, (uint32_t)i32(a,LENBUF) + c2);
        else {
            uint8_t b = u8(a, (uint32_t)i32(a,LENBUF) + c2);
            uint32_t off = (uint32_t)i32(a, (uint32_t)i32(a,OFFBUF) + o2);
            uint32_t slot = off_slot(a, off);
            s2 = (b < 7) ? (b + 0x100 + slot*8) : (slot*8 + 0x107);
            o2 += 4;
        }
        h2[s2]++;
        c2++; m2 = (uint8_t)((m2 << 1) | ((m2 & 0x80) ? 1 : 0));
        n--;
    } while (n != 0);
    uint32_t diff = 0;
    for (uint32_t s = 0; s < syms; s++) {
        int v = (int)kSplitWeight[split_mag((uint16_t)h1[s])] - (int)kSplitWeight[split_mag((uint16_t)h2[s])];
        diff += (uint32_t)(v < 0 ? -v : v);
    }
    return diff;
}

// where to end the next block: the first point (stepping 0x400 tokens) at which
// the statistics before and after differ enough, refined to 0x40 tokens.
void LzxEnc::find_split(uint32_t* outTok, uint32_t tokens, uint32_t matches, uint32_t* outOff) {
    uint8_t* a = A();
    *outTok = tokens;
    if (outOff != nullptr) *outOff = matches;
    uint8_t splits;
    if (tokens < 0x1800 || (splits = u8(a,CHUNK_SPLITS), 3 < splits)) return;
    uint16_t matchesBefore[1032];                       // per 8 tokens (one flag byte)
    uint16_t m = 0; uint32_t i = 0;
    if (tokens >> 3 != 0) {
        uint32_t flag = (uint32_t)i32(a,FLAGBUF);
        int j = 0;
        do {
            if ((i & 7) == 0) matchesBefore[j++] = m;
            uint8_t fb = u8(a, flag + i);
            i++;
            m = (uint16_t)(m + u8(a, POPCOUNT + fb));
        } while (i < (tokens >> 3));
    }
    uint32_t last = tokens - 0x1000;
    if (0x800 < last) {
        uint32_t at = 0xc00;
        uint32_t best = 0;
        do {
            uint32_t d = eval_split(at-0x400, at, matchesBefore[(at-0x400)>>6], matchesBefore[at>>6]);
            if (0x578 < d &&
                (d = eval_split(at-0x800, at+0x400, matchesBefore[(at-0x800)>>6], matchesBefore[(at+0x400)>>6]), 0x578 < d) &&
                (d = eval_split(at-0xc00, at+0x800, matchesBefore[(at-0xc00)>>6], matchesBefore[(at+0x800)>>6]), 0x578 < d)) {
                uint32_t hi = at - 0x200;
                uint32_t bestD = 0;
                if (hi < at + 0x600) {
                    uint32_t lo = at - 0x600;
                    do {
                        uint32_t d2 = eval_split(lo, hi, matchesBefore[lo>>6], matchesBefore[hi>>6]);
                        if (bestD < d2) { bestD = d2; best = hi; }
                        hi += 0x40; lo += 0x40;
                    } while (hi < at + 0x600);
                    if (0x6a3 < bestD && 0xfff < best) {
                        u8(a,CHUNK_SPLITS) = (uint8_t)(splits + 1);
                        *outTok = best;
                        if (outOff != nullptr) *outOff = matchesBefore[best>>6];
                        break;
                    }
                }
            }
            bool more = at < last;
            at += 0x400;
            if (!more) break;
        } while (true);
    }
}

// pad + ship the current 32 KB chunk through the output sink.
void LzxEnc::chunk_flush() {
    uint8_t* a = A();
    if (i32(a,CHUNK_BYTES) != 0) {
        if ((int8_t)i8(a,BITFREE) < 0x20)
            write_bits(0, (int8_t)((char)i8(a,BITFREE) - 0x10));
        int n = i32(a,OUT_PTR) - i32(a,OUT_BASE);
        if (0 < n) out_chunk(P(a,OUT_BASE), n);
    }
    i32(a,CHUNK_BYTES) = 0;
    i32(a,OUT_PTR) = i32(a,OUT_BASE);
    u8(a,BITFREE) = 0x20;
    i32(a,BITBUF) = 0;
}

// carry R0/R1/R2 through the block's matches (only from the last point where
// three new offsets in a row reset them).
void LzxEnc::update_repeats(uint32_t matches) {
    uint8_t* a = A();
    int n = (int)matches;
    int i = n - 1;
    uint8_t run = 0;
    if (-1 < i) {
        uint32_t p = (uint32_t)i32(a,OFFBUF) + (uint32_t)i*4;
        do {
            if ((uint32_t)i32(a, p) < 3) run = 0;
            else { run++; if (2 < run) goto replay; }
            i--; p -= 4;
        } while (-1 < i);
        if (2 < run) goto replay;
    }
    i = 0;
replay:
    for (; i < n; i++) {
        uint32_t code = (uint32_t)i32(a, (uint32_t)i32(a,OFFBUF) + (uint32_t)i*4);
        if (code != 0) {
            if (code < 3) {
                int t = erep(a, code);
                erep(a, code) = erep(a, 0);
                erep(a, 0) = t;
            } else {
                erep(a, 2) = erep(a, 1);
                erep(a, 1) = erep(a, 0);
                erep(a, 0) = (int32_t)(code - 2);
            }
        }
    }
}

// an uncompressed block: aligned, R0/R1/R2, then the bytes themselves.
void LzxEnc::store_uncompressed(uint32_t from, uint32_t len) {
    uint8_t* a = A();
    write_bits(0, (int8_t)((char)i8(a,BITFREE) - 0x10));
    uint32_t rep = EMIT_REPS;
    int k = 3;
    do {
        int v = i32(a, rep);
        u8(a, P(a,OUT_PTR)) = (uint8_t)v;           i32(a,OUT_PTR) += 1;
        u8(a, P(a,OUT_PTR)) = (uint8_t)(v >> 8);    i32(a,OUT_PTR) += 1;
        u8(a, P(a,OUT_PTR)) = (uint8_t)(v >> 0x10); i32(a,OUT_PTR) += 1;
        u8(a, P(a,OUT_PTR)) = (uint8_t)(v >> 0x18); i32(a,OUT_PTR) += 1;
        rep += 4; k--;
    } while (k != 0);
    uint32_t odd = len & 1;
    if (len != 0) {
        do {
            u8(a, P(a,OUT_PTR)) = WIN(a, from); i32(a,OUT_PTR) += 1;
            i32(a,CHUNK_BYTES) += 1;
            from++; len--;
            if (i32(a,CHUNK_BYTES) == 0x8000) {
                if ((int8_t)i8(a,BITFREE) < 0x20) write_bits(0, (int8_t)((char)i8(a,BITFREE) - 0x10));
                int n = i32(a,OUT_PTR) - i32(a,OUT_BASE);
                if (0 < n) out_chunk(P(a,OUT_BASE), n);
                i32(a,CHUNK_BYTES) = 0;
                i32(a,OUT_PTR) = i32(a,OUT_BASE);
                u8(a,BITFREE) = 0x20;
                i32(a,BITBUF) = 0;
                u8(a,CHUNK_SPLITS) = 0;
            }
        } while (len != 0);
    }
    if (odd != 0) { u8(a, P(a,OUT_PTR)) = 0; i32(a,OUT_PTR) += 1; }
    i32(a,BITBUF) = 0;
    u8(a,BITFREE) = 0x20;
}

// the block's size in bytes if compressed with the current trees.
uint32_t LzxEnc::compressed_size() {
    uint8_t* a = A();
    int s1 = 0, s0 = 0x4b0, s3 = 0, s2 = 0;            // bits; 0x4b0 for the trees
    uint32_t pl = MAIN_LEN + 1, pf = MAIN_FREQ + 2;
    int n = 0x40;
    do {                                                // literals, four at a time
        s0 += (int)u16(a, pf-2) * u8(a, pl-1);
        s1 += (int)u16(a, pf)   * u8(a, pl);
        s2 += (int)u16(a, pf+2) * u8(a, pl+1);
        s3 += (int)u16(a, pf+4) * u8(a, pl+2);
        pf += 8; pl += 4; n--;
    } while (n != 0);
    s0 = s0 + s3 + s2 + s1;
    uint32_t slot = 0; uint8_t next = 0;
    if (i32(a,NUM_SLOTS) != 0) {
        do {                                            // matches: each slot's 8 symbols
            int base8 = (int)slot * 8;
            int sym = (int)slot * 8 + 0x100;
            uint32_t eb = kExtraBits[slot];
            next++; slot = next;
            s0 = (u8(a, MAIN_LEN + 2 + sym) + eb) * (int)u16(a, MAIN_FREQ + 4 + sym*2) + s0
               + (u8(a, MAIN_LEN_M + 1 + base8) + eb) * (int)u16(a, MAIN_FREQ + 2 + sym*2)
               + (u8(a, MAIN_LEN + sym) + eb) * (int)u16(a, MAIN_FREQ + sym*2)
               + (u8(a, MAIN_LEN + 4 + sym) + eb) * (int)u16(a, MAIN_FREQ + 8 + sym*2)
               + (u8(a, MAIN_LEN + 5 + sym) + eb) * (int)u16(a, MAIN_FREQ + 0xa + sym*2)
               + (u8(a, MAIN_LEN + 6 + sym) + eb) * (int)u16(a, MAIN_FREQ + 0xc + sym*2)
               + (u8(a, MAIN_LEN + 7 + sym) + eb) * (int)u16(a, MAIN_FREQ + 0xe + sym*2)
               + (u8(a, MAIN_LEN + 3 + sym) + eb) * (int)u16(a, MAIN_FREQ + 6 + sym*2);
        } while (slot < (uint32_t)i32(a,NUM_SLOTS));
    }
    int t2 = 0; s1 = 0;
    uint32_t pl2 = LEN_LEN + 1, pf2 = LEN_FREQ + 2;
    n = 0x53;
    do {                                                // lengths, three at a time
        s0 += (int)u16(a, pf2-2) * u8(a, pl2-1);
        s1 += (int)u8(a, pl2)    * u16(a, pf2);
        t2 += (int)u16(a, pf2+2) * u8(a, pl2+1);
        pf2 += 6; pl2 += 3; n--;
    } while (n != 0);
    return (uint32_t)(s0 + t2 + s1 + 7) >> 3;
}

// the tokens of a verbatim block.
void LzxEnc::emit_tokens_verbatim(uint32_t tokens) {
    uint8_t* a = A();
    uint32_t i = 0;
    if (tokens != 0) {
        int offPos = 0;
        do {
            if ((u8(a, (i >> 3) + (uint32_t)i32(a,FLAGBUF)) & (uint8_t)(1 << (i & 7))) == 0) {
                uint32_t lit = u8(a, i + (uint32_t)i32(a,LENBUF));
                write_bits(u16(a, MAIN_CODE + lit*2), u8(a, MAIN_LEN + lit));
                i32(a,CHUNK_BYTES) += 1;
            } else {
                uint8_t lenHdr = u8(a, i + (uint32_t)i32(a,LENBUF));
                uint32_t offset = (uint32_t)i32(a, offPos + (uint32_t)i32(a,OFFBUF));
                offPos += 4;
                uint32_t slot = off_slot(a, offset);
                if (lenHdr < 7) {
                    write_bits(u16(a, MAIN_CODE_M + ((uint32_t)lenHdr + slot*8)*2),
                               u8(a, (uint32_t)lenHdr + MAIN_LEN_M + slot*8));
                } else {
                    write_bits(u16(a, slot*0x10 + MAIN_CODE_M + 0xe), u8(a, MAIN_LEN_M + 7 + slot*8));
                    write_bits(u16(a, LEN_CODE - 0xe + (uint32_t)lenHdr*2), u8(a, (uint32_t)lenHdr + LEN_LEN - 7));
                }
                if (kExtraBits[slot] != 0)
                    write_bits(kFooterMask[slot] & offset, kExtraBits[slot]);
                i32(a,CHUNK_BYTES) += (int)lenHdr + 2;
            }
            i += 1;
            if (i32(a,CHUNK_BYTES) == 0x8000) { chunk_flush(); u8(a,CHUNK_SPLITS) = 0; }
        } while (i < tokens);
    }
}

// the tokens of an aligned-offset block: an offset's low 3 bits go through the aligned tree.
void LzxEnc::emit_tokens_aligned(uint32_t tokens) {
    uint8_t* a = A();
    uint32_t i = 0;
    if (tokens != 0) {
        int offPos = 0;
        do {
            if ((u8(a, (i >> 3) + (uint32_t)i32(a,FLAGBUF)) & (uint8_t)(1 << (i & 7))) == 0) {
                uint32_t lit = u8(a, (uint32_t)i32(a,LENBUF) + i);
                write_bits(u16(a, MAIN_CODE + lit*2), u8(a, MAIN_LEN + lit));
                i32(a,CHUNK_BYTES) += 1;
            } else {
                uint8_t lenHdr = u8(a, (uint32_t)i32(a,LENBUF) + i);
                uint32_t offset = (uint32_t)i32(a, offPos + (uint32_t)i32(a,OFFBUF));
                offPos += 4;
                uint32_t slot = off_slot(a, offset);
                if (lenHdr < 7) {
                    write_bits(u16(a, MAIN_CODE_M + ((uint32_t)lenHdr + slot*8)*2),
                               u8(a, (uint32_t)lenHdr + MAIN_LEN_M + slot*8));
                } else {
                    write_bits(u16(a, slot*0x10 + MAIN_CODE_M + 0xe), u8(a, MAIN_LEN_M + 7 + slot*8));
                    write_bits(u16(a, LEN_CODE - 0xe + (uint32_t)lenHdr*2), u8(a, (uint32_t)lenHdr + LEN_LEN - 7));
                }
                uint32_t lenBytes = lenHdr;
                uint8_t eb = kExtraBits[slot];
                if (eb < 3) {
                    if (eb != 0) write_bits(kFooterMask[slot] & offset, eb);
                } else {
                    if (3 < eb) write_bits(((1u << (eb - 3)) - 1) & (offset >> 3), eb - 3);
                    uint32_t al = offset & 7;
                    write_bits(u16(a, ALN_CODE + al*2), u8(a, ALN_LEN + al));
                }
                i32(a,CHUNK_BYTES) += (int)lenBytes + 2;
            }
            i += 1;
            if (i32(a,CHUNK_BYTES) == 0x8000) { chunk_flush(); u8(a,CHUNK_SPLITS) = 0; }
        } while (i < tokens);
    }
}

// ===== cost-optimal forward parse ============================================
// Parse `len` new bytes into tokens. A long match (0x32+) is taken at once; a
// shorter one opens an optimal-parse span: every position ahead gets the
// cheapest cost of reaching it (a node per position), the span ends where no
// cheaper path can cross, and the cheapest path is walked back and emitted.
void LzxEnc::parse(int len) {
    uint8_t* a = A();
    const int64_t nodes = i32(a, NODES);
    uint32_t pos = (uint32_t)i32(a, CUR_POS);
    uint32_t end = (uint32_t)(len + pos);
    uint32_t cursor = pos;
    // (declared up front: the gotos below cross what would otherwise be their scopes)
    uint32_t mlen, frameEnd, limit, offset = 0, reach, horizon, run, at, stop, mcode, cost;
    uint32_t k, left, start, nextNode, jumpCode, litCost, steps;
    int64_t base, tab, cur, next, costp, target, p, flagByte, from;
    int prevPos;
    uint8_t slot;

    if (i32(a, FIRST_PARSE) == 0) {
        uint32_t warm = pos - 0x32;                    // re-insert the last 0x32 positions
        for (int i = 0x32; i != 0; i--) {
            bt_insert(a, warm, (pos - (uint32_t)i32(a,WINDOW)) + 4);
            warm++;
        }
    } else {                                           // stream header: E8 translation on/off
        i32(a, FIRST_PARSE) = 0;
        i32(a, NEXT_REFRESH) = 10000;
        if (i32(a, E8_SIZE) == 0) write_bits(0, 1);
        else { write_bits(1, 1); write_bits(u16(a,E8_SIZE_HI), 0x10); write_bits(u16(a,E8_SIZE), 0x10); }
    }

next_frame:
    while (true) {
        frameEnd = pos + 0x8000;
next_token:
        if (end <= pos) break;                         // -> tail
        limit = frameEnd & 0xffff8000;
        if (end < (frameEnd & 0xffff8000)) limit = end;
        mlen = (uint32_t)bt_search(a, pos);
        if (((int)mlen < 2) ||
            (limit < mlen + pos && (mlen = limit - pos, (int)mlen < 2))) {
            // a literal
            u8(a, (uint32_t)i32(a,LENBUF) + i32(a,TOKENS)) = WIN(a, pos);
            i32(a,TOKENS) += 1;
            frameEnd++; pos++; cursor = pos;
            if (0xfff7 < (uint32_t)i32(a,TOKENS)) flush_pending(pos);
            goto next_token;
        }
        if (0x31 < (int)mlen) {
            // a long match: take it
            mcode = (uint32_t)match_off(a, mlen);
            if (mcode == 3 && 0x10 < (int)mlen) {
                bt_insert(a, pos + 1, (pos - (uint32_t)i32(a,WINDOW)) + 5);
            } else {
                for (uint32_t k = 1; k < mlen; k++)
                    bt_insert(a, k + pos, (k - (uint32_t)i32(a,WINDOW)) + 4 + pos);
            }
            flagByte = ((int64_t)((uint32_t)i32(a,TOKENS) >> 3)) + i32(a,FLAGBUF);
            pos += mlen;
            u8(a, (size_t)flagByte) |= (uint8_t)(1 << ((uint8_t)i32(a,TOKENS) & 7));
            i8(a, (uint32_t)i32(a,LENBUF) + i32(a,TOKENS)) = (int8_t)((char)mlen - 2);
            i32(a,TOKENS) += 1;
            i32(a, (uint32_t)i32(a,OFFBUF) + (uint32_t)i32(a,MATCHES)*4) = (int32_t)mcode;
            i32(a,MATCHES) += 1;
            if (mcode < 3) {
                if (mcode != 0) {
                    int t = prep(a,0); prep(a,0) = prep(a, mcode); prep(a, mcode) = t;
                }
            } else {
                prep(a,2) = prep(a,1); prep(a,1) = prep(a,0); prep(a,0) = (int32_t)(mcode - 2);
            }
            cursor = pos;
            if (0xfff7 < (uint32_t)i32(a,TOKENS) || 0x7ff7 < (uint32_t)i32(a,MATCHES)) flush_pending(pos);
            goto next_frame;
        }
        // ---- optimal parse from a short match (2..0x31) ----------------------
        reach = mlen + pos;                            // furthest position priced
        horizon = pos + 0xefd;                         // the span may not grow past this
        i32(a, nodes + NODE_SIZE + N_COST) = (int32_t)u8(a, MAIN_LEN + (uint32_t)WIN(a, pos));
        i32(a, nodes + NODE_SIZE + N_FROM) = (int32_t)pos;
        k = 2;
        if (1 < mlen) {                                // price every match length from here
            int64_t nodeOff = 2 * NODE_SIZE;
            tab = MATCH_OFF + 2 * 4;
            do {
                offset = (uint32_t)i32(a, (size_t)tab);
                slot = (uint8_t)off_slot(a, offset);
                if (k < 9)
                    i32(a, nodes + nodeOff + N_COST) =
                        (int32_t)(u8(a, k + (uint32_t)slot*8 + MAIN_LEN_M - 2) + kExtraBits[slot]);
                else
                    i32(a, nodes + nodeOff + N_COST) =
                        (int32_t)(u8(a, (uint32_t)slot*8 + MAIN_LEN_M + 7) + u8(a, k + LEN_LEN - 9) + kExtraBits[slot]);
                i32(a, nodes + nodeOff + N_FROM) = (int32_t)pos;
                i32(a, nodes + nodeOff + N_OFFCODE) = i32(a, (size_t)tab);
                k++; nodeOff += NODE_SIZE; tab += 4;
            } while (k <= mlen);
        }
        i32(a, nodes + N_COST) = 0;
        i32(a, nodes + N_R0) = prep(a,0);
        i32(a, nodes + N_R1) = prep(a,1);
        i32(a, nodes + N_R2) = prep(a,2);
        base = nodes - (int64_t)pos * NODE_SIZE;       // node of window position q: base + q*NODE_SIZE
        prevPos = pos - 1;
        // counts down from the last offset code read above (not the frame limit)
        left = offset - pos;
        cur = (int64_t)pos * NODE_SIZE + N_FROM + base;    // &node[pos].from
        start = pos;
        while (true) {
            // step to the next position: its R0..R2 follow from how it was reached
            from = i32(a, cur + NODE_SIZE);
            left = left - 1;
            next = cur + NODE_SIZE;
            prevPos = prevPos + 1;
            at = pos + 1;
            if (from != prevPos) {
                uint32_t code = (uint32_t)i32(a, cur + NODE_SIZE - N_FROM + N_OFFCODE);
                int64_t src = base + from * NODE_SIZE;
                if (code < 3) {
                    if (code == 0) {
                        prep(a,0) = i32(a, src + N_R0);
                        prep(a,1) = i32(a, src + N_R1);
                        prep(a,2) = i32(a, src + N_R2);
                    } else if (code == 1) {
                        prep(a,0) = i32(a, src + N_R1);
                        prep(a,1) = i32(a, src + N_R0);
                        prep(a,2) = i32(a, src + N_R2);
                    } else {
                        prep(a,0) = i32(a, src + N_R2);
                        prep(a,1) = i32(a, src + N_R1);
                        prep(a,2) = i32(a, src + N_R0);
                    }
                } else {
                    prep(a,0) = (int32_t)(code - 2);
                    prep(a,1) = i32(a, src + N_R0);
                    prep(a,2) = i32(a, src + N_R1);
                }
            }
            i32(a, next - N_FROM + N_R0) = prep(a,0);
            i32(a, next - N_FROM + N_R1) = prep(a,1);
            i32(a, next - N_FROM + N_R2) = prep(a,2);
            nextNode = (uint32_t)next;
            if (reach == at) goto walk_back;
            run = (uint32_t)bt_search(a, at);
            if ((run + at <= limit) || (run = left, 1 < (int)left)) break;
            run = 0;
consider:
            if (horizon <= run + at) goto take_long;
            if (((2 < (int)run) || (run == 2 && (uint32_t)match_off(a,2) < 0x800)) &&
                reach < run + at) {
                // reaching further: open the new nodes at "unreached"
                uint32_t far = (run - start) + at;
                if (0xefb < far) far = 0xefc;
                int64_t q = (int64_t)(reach - start) + 1;
                if (q <= (int64_t)far) {
                    int64_t off = q * NODE_SIZE;
                    int64_t cnt = ((int64_t)far - q) + 1;
                    do { i32(a, nodes + N_COST + off) = (int32_t)0xffffffff; off += NODE_SIZE; cnt--; } while (cnt != 0);
                }
                reach = run + at;
            }
            // a literal from here
            cost = (uint32_t)i32(a, cur + NODE_SIZE - N_FROM + N_COST);
            litCost = (uint32_t)u8(a, MAIN_LEN + (uint32_t)WIN(a, at)) + cost;
            if (litCost < (uint32_t)i32(a, cur + 2*NODE_SIZE - N_FROM + N_COST)) {
                i32(a, cur + 2*NODE_SIZE - N_FROM + N_COST) = (int32_t)litCost;
                i32(a, cur + 2*NODE_SIZE) = (int32_t)at;
            }
            k = 2; cur = next; pos = at;
            if (1 < run) {                             // a match of every length from here
                tab = MATCH_OFF + 2 * 4;
                costp = nextNode + 2*NODE_SIZE + (N_COST - N_FROM);
                do {
                    offset = (uint32_t)i32(a, (size_t)tab);
                    slot = (uint8_t)off_slot(a, offset);
                    uint32_t c;
                    if (k < 9) c = u8(a, k + (uint32_t)slot*8 + MAIN_LEN_M - 2);
                    else c = u8(a, (uint32_t)slot*8 + MAIN_LEN_M + 7) + u8(a, k + LEN_LEN - 9);
                    c = c + kExtraBits[slot] + cost;
                    if (c < (uint32_t)i32(a, costp)) {
                        i32(a, costp) = (int32_t)c;
                        i32(a, costp - N_COST + N_FROM) = (int32_t)at;
                        i32(a, costp - N_COST + N_OFFCODE) = i32(a, (size_t)tab);
                    }
                    k++; tab += 4; costp += NODE_SIZE;
                } while (k <= run);
            }
        }
        if ((int)run < 0x33) goto consider;
take_long:
        // a long match ends the span: take it, then walk back
        jumpCode = (uint32_t)match_off(a, run);
        stop = run + at;
        target = base + (int64_t)stop * NODE_SIZE;
        i32(a, target + N_OFFCODE) = (int32_t)jumpCode;
        i32(a, target + N_FROM) = (int32_t)at;
        if (jumpCode == 3 && 0x10 < (int)run) {
            bt_insert(a, pos + 2, (at - (uint32_t)i32(a,WINDOW)) + 5);
        } else {
            for (pos = 1; pos < run; pos++)
                bt_insert(a, pos + at, (pos - (uint32_t)i32(a,WINDOW)) + 4 + at);
        }
        at = stop;
        if (jumpCode < 3) {
            if (jumpCode != 0) { int t=prep(a,0); prep(a,0)=prep(a,jumpCode); prep(a,jumpCode)=t; }
        } else { prep(a,2)=prep(a,1); prep(a,1)=prep(a,0); prep(a,0)=(int32_t)(jumpCode-2); }
walk_back:
        {
            // reverse the cheapest path's links, then emit it start to end
            p = base;
            steps = 0;
            pos = (uint32_t)i32(a, base + N_FROM + (int64_t)at * NODE_SIZE);
            do {
                cursor = pos;
                pos = (uint32_t)i32(a, (int64_t)cursor * NODE_SIZE + N_FROM + base);
                steps = steps + 1;
                i32(a, (int64_t)cursor * NODE_SIZE + N_FROM + base) = (int32_t)at;
                at = cursor;
            } while (cursor != start);
            while (0xfff7 < (uint32_t)i32(a,TOKENS) + steps || 0x7ff7 < (uint32_t)i32(a,MATCHES) + steps)
                flush_pending(cursor);
            do {
                p = p + N_FROM + (int64_t)cursor * NODE_SIZE;
                if (cursor + 1 < (uint32_t)i32(a, p)) {        // a match
                    flagByte = ((int64_t)((uint32_t)i32(a,TOKENS) >> 3)) + i32(a,FLAGBUF);
                    u8(a,(size_t)flagByte) |= (uint8_t)(1 << ((uint8_t)i32(a,TOKENS) & 7));
                    i8(a, (uint32_t)i32(a,TOKENS) + (uint32_t)i32(a,LENBUF)) =
                        (int8_t)(((char)i32(a,p) - (char)cursor) - 2);
                    i32(a,TOKENS) += 1;
                    i32(a, (uint32_t)i32(a,OFFBUF) + (uint32_t)i32(a,MATCHES)*4) =
                        i32(a, base + (int64_t)i32(a,p) * NODE_SIZE + N_OFFCODE);
                    i32(a,MATCHES) += 1;
                    cursor = (uint32_t)i32(a, p);
                } else {                                       // a literal
                    u8(a, (uint32_t)i32(a,TOKENS) + (uint32_t)i32(a,LENBUF)) = WIN(a, cursor);
                    i32(a,TOKENS) += 1;
                    cursor = cursor + 1;
                }
                steps = steps - 1;
                p = base;
            } while (steps != 0);
        }
        if ((uint32_t)i32(a,NEXT_REFRESH) <= (uint32_t)i32(a,TOKENS)) { refresh_costs(); i32(a,NEXT_REFRESH) += 0x1000; }
        pos = cursor;
        if (u8(a,BLOCK_PENDING) != 0 &&
            (0xfdff < (uint32_t)i32(a,TOKENS) || 0x7dff < (uint32_t)i32(a,MATCHES))) {
            int r = block_boundary(&cursor); pos = cursor;
            if (r == 0) { flush_pending(cursor); pos = cursor; }
        }
    }
    // ---- tail: slide the window when it is full --------------------------------
    i32(a,PARSED) = (int32_t)(pos - (uint32_t)i32(a,WINDOW));
    if (0x7fff < len) {
        uint32_t q = pos;
        uint32_t thresh = (pos - (uint32_t)i32(a,WINDOW)) + 0x36;
        q--;
        for (int n = 0x32; n != 0; n--) { bt_slide_delete(a, q, thresh); q--; }
        pos = cursor;
        if ((uint32_t)(i32(a,SLIDE) + i32(a,WINDOW)) <=
            (uint32_t)(i32(a,WIN_BASE) - i32(a,WINDOW_BUF) + (int32_t)cursor)) {
            if (u8(a,BLOCK_PENDING) != 0) {
                int r = block_boundary(&cursor); pos = cursor;
                if (r != 0) goto next_frame;
            }
            pos = cursor;
            std::memmove(a + P(a,WINDOW_BUF), a + (uint32_t)(i32(a,SLIDE) + i32(a,WINDOW_BUF)), i32(a,WINDOW));
            std::memmove(a + P(a,BT_LEFT_BUF), a + (uint32_t)(i32(a,BT_LEFT_BUF) + i32(a,SLIDE)*4), (size_t)i32(a,WINDOW)*4);
            std::memmove(a + P(a,BT_RIGHT_BUF), a + (uint32_t)(i32(a,BT_RIGHT_BUF) + i32(a,SLIDE)*4), (size_t)i32(a,WINDOW)*4);
            i32(a,PARSED) = (int32_t)(pos - (uint32_t)i32(a,WINDOW));
            int32_t slide = i32(a,SLIDE);
            i32(a,WIN_BASE) -= slide;
            i32(a,BT_LEFT) += slide * -4;
            i32(a,BT_RIGHT) += slide * -4;
        }
        i32(a,CUR_POS) = (int32_t)pos;
        return;
    }
    if (u8(a,BLOCK_PENDING) == 0) { i32(a,CUR_POS) = (int32_t)pos; return; }
    {
        int r = block_boundary(&cursor); pos = cursor;
        if (r == 0) { i32(a,CUR_POS) = (int32_t)cursor; return; }
    }
    goto next_frame;
}

// Compress `in` into a continuous LZX bitstream. The SHA-1 block wrapping is
// applied by the caller.
std::vector<uint8_t> lzx_compress(const uint8_t* in, size_t in_size,
                                  uint32_t window_size, uint32_t e8_filesize) {
    LzxEnc enc;
    enc.setup(window_size, e8_filesize);
    enc.init_run();
    uint8_t* a = enc.A();
    enc.input.assign(in, in + in_size);
    i32(a, INPUT_POS) = 0;
    i32(a, INPUT_LEFT) = (int32_t)in_size;
    while (i32(a, INPUT_LEFT) > 0) {
        enc.feed_chunk();
        if (i32(a, OUT_OVERFLOW) == 0) { enc.build_codes(0); enc.fill_default_lengths(); }
    }
    while (i32(a, TOKENS) != 0) enc.build_emit_block();
    enc.chunk_flush();
    return std::move(enc.output);
}

} // namespace xex::compress
