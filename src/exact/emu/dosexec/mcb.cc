// mcb.cc -- the DOS memory arena, kept in guest RAM the way DOS keeps it (dos.py _mcb_*).
//
// Every block is preceded by a 16-byte arena header one paragraph before it: +0 'M' (another block
// follows) or 'Z' (the last), +1 owner PSP (0 free, 8 DOS), +3 size in paragraphs, +8 name.  The
// next header is at mcb + 1 + size.  Programs walk this chain themselves (a DOS extender's loader
// sizes the memory it may take from it), so it is the one source of truth.
#include "dosexec.h"

#include <algorithm>

namespace cbd {

void mcb_write(Run &r, int64_t seg, uint8_t sig, int64_t owner, int64_t size) {
    uint8_t h[16] = {0};
    h[0] = sig;
    h[1] = uint8_t(owner & 0xFF);
    h[2] = uint8_t((owner >> 8) & 0xFF);
    h[3] = uint8_t(size & 0xFF);
    h[4] = uint8_t((size >> 8) & 0xFF);
    r.wr(uint64_t(seg) * 16, h, 16);
}

static void mcb_set(Run &r, int64_t seg, std::optional<uint8_t> sig, std::optional<int64_t> owner,
                    std::optional<int64_t> size) {
    if (sig) r.wr(uint64_t(seg) * 16, &*sig, 1);
    if (owner) {
        uint8_t b[2] = {uint8_t(*owner & 0xFF), uint8_t((*owner >> 8) & 0xFF)};
        r.wr(uint64_t(seg) * 16 + 1, b, 2);
    }
    if (size) {
        uint8_t b[2] = {uint8_t(*size & 0xFF), uint8_t((*size >> 8) & 0xFF)};
        r.wr(uint64_t(seg) * 16 + 3, b, 2);
    }
}

static Mcb mcb_read(Run &r, int64_t seg) {
    Bytes h = r.rd(uint64_t(seg) * 16, 5);
    if (h[0] != 0x4D && h[0] != 0x5A)
        throw ArenaError{7};
    return Mcb{seg, h[0], int64_t(h[1] | h[2] << 8), int64_t(h[3] | h[4] << 8)};
}

std::vector<Mcb> mcb_chain(Run &r, int64_t first) {
    std::vector<Mcb> out;
    int64_t seg = first;
    for (int k = 0; k < 0x10000; k++) {
        Mcb m = mcb_read(r, seg);
        out.push_back(m);
        if (m.sig == 0x5A)
            return out;
        seg += 1 + m.size;
        if (seg > 0xFFFF)
            break;
    }
    throw ArenaError{7};
}

static void mcb_coalesce(Run &r, int64_t first) {
    auto chain = mcb_chain(r, first);
    size_t i = 0;
    while (i < chain.size()) {
        Mcb c = chain[i];
        int64_t size = c.size;
        uint8_t sig = c.sig;
        size_t j = i;
        while (c.owner == MCB_FREE && j + 1 < chain.size() && chain[j + 1].owner == MCB_FREE) {
            j++;
            size += 1 + chain[j].size;
            sig = chain[j].sig;
        }
        if (j > i)
            mcb_set(r, c.seg, sig, std::nullopt, size);
        i = j + 1;
    }
}

int64_t mcb_largest(Run &r, int64_t first) {
    try {
        mcb_coalesce(r, first);
        int64_t m = 0;
        for (const auto &c : mcb_chain(r, first))
            if (c.owner == MCB_FREE) m = std::max(m, c.size);
        return m;
    } catch (ArenaError &) {
        return 0;
    }
}

static void split(Run &r, int64_t seg, uint8_t sig, int64_t size, int64_t keep) {
    if (size > keep) {
        mcb_write(r, seg + 1 + keep, sig, MCB_FREE, size - keep - 1);
        mcb_set(r, seg, uint8_t('M'), std::nullopt, keep);
    }
}

std::optional<int64_t> mcb_alloc(Run &r, int64_t first, int64_t paras, int64_t owner, int64_t strategy) {
    std::vector<Mcb> chain;
    try {
        mcb_coalesce(r, first);
        chain = mcb_chain(r, first);
    } catch (ArenaError &) {
        return std::nullopt;
    }
    std::vector<Mcb> fits;
    for (const auto &c : chain)
        if (c.owner == MCB_FREE && c.size >= paras) fits.push_back(c);
    if (fits.empty())
        return std::nullopt;
    strategy &= 3;
    Mcb c = fits[0];
    if (strategy == 2) {
        c = fits.back();
    } else if (strategy != 0) {                               // best fit: the first of the smallest
        for (const auto &f : fits)
            if (f.size < c.size) c = f;
    }
    int64_t seg = c.seg, size = c.size;
    if (strategy == 2 && size > paras) {                      // the block at the free block's top end
        mcb_set(r, seg, uint8_t('M'), std::nullopt, size - paras - 1);
        seg += size - paras;
        mcb_write(r, seg, c.sig, owner, paras);
        return seg + 1;
    }
    split(r, seg, c.sig, size, paras);
    mcb_set(r, seg, std::nullopt, owner, std::nullopt);
    return seg + 1;
}

void mcb_free(Run &r, int64_t first, int64_t block) {
    try {
        if (block < 1) throw ArenaError{9};
        mcb_read(r, block - 1);
    } catch (ArenaError &) {
        throw ArenaError{9};                                   // no header there: not a block
    }
    bool found = false;
    for (const auto &c : mcb_chain(r, first))
        if (c.seg == block - 1) found = true;
    if (!found)
        throw ArenaError{9};
    mcb_set(r, block - 1, std::nullopt, int64_t(MCB_FREE), std::nullopt);
    mcb_coalesce(r, first);
}

std::optional<int64_t> mcb_resize(Run &r, int64_t first, int64_t block, int64_t paras) {
    mcb_coalesce(r, first);
    auto chain = mcb_chain(r, first);
    size_t k = chain.size();
    for (size_t i = 0; i < chain.size(); i++)
        if (chain[i].seg == block - 1) { k = i; break; }
    if (k == chain.size())
        throw ArenaError{9};
    Mcb c = chain[k];
    uint8_t sig = c.sig;
    int64_t size = c.size;
    const Mcb *nxt = k + 1 < chain.size() ? &chain[k + 1] : nullptr;
    int64_t avail = size + ((nxt && nxt->owner == MCB_FREE) ? 1 + nxt->size : 0);
    if (paras > avail)
        return avail;
    if (paras > size) {                                        // grow into the free block behind
        sig = nxt->sig;
        size = avail;
        mcb_set(r, c.seg, sig, std::nullopt, size);
    }
    split(r, c.seg, sig, size, paras);
    mcb_coalesce(r, first);
    return std::nullopt;
}

}  // namespace cbd
