#pragma once

// The console's system fonts (.xtt, e.g. xenonclatin.xtt = "Xbox TC", the Guide's Latin face) and a glyph rasteriser.
//
// .xtt ("xttf"): a 0x118-byte header (u32 at 0x10C: compressed size of the table directory, u32 at 0x110: its size),
// then the zlib-compressed sfnt table directory with its tables (cmap, head, hhea, hmtx, name, ... and xchk, xttf,
// xloc, xglf). The glyph outlines (TrueType glyf records) are not in the directory: the xglf entry's offset is a file
// offset, where 4 KiB pages of independent zlib streams follow; xloc holds one u32 per glyph (+1): page << 16 |
// offset inside the inflated page. Glyphs are rasterised from their outlines without hinting, with exact area coverage.

#include <cstdint>
#include <map>
#include <mutex>
#include <string>
#include <vector>

namespace xui
{
    struct GlyphBitmap
    {
        int left = 0, top = 0;      // offset of the bitmap from the pen position (pixels, y down)
        int width = 0, height = 0;
        std::vector<uint8_t> coverage; // 0-255
    };

    class Font
    {
    public:
        bool Load(std::vector<uint8_t> data, std::string& error);

        const std::string& Name() const { return name_; }
        int UnitsPerEm() const { return unitsPerEm_; }
        int Ascent() const { return ascent_; }   // font units, up
        int Descent() const { return descent_; } // font units, negative
        int LineGap() const { return lineGap_; }

        uint32_t GlyphIndex(char32_t c) const;
        int Advance(uint32_t glyph) const; // font units
        // `scale` = pixels per font unit; (subX, subY) (0..1) the fractional position of the pen / baseline, so the
        // bitmap is placed at whole pixels (offsets relative to floor(pen), floor(baseline)).
        const GlyphBitmap& Rasterize(uint32_t glyph, float scale, float subX, float subY);

    private:
        struct Point { float x, y; bool on; };
        bool GlyphData(uint32_t glyph, const uint8_t*& data, size_t& size);
        void Outline(uint32_t glyph, std::vector<std::vector<Point>>& contours, int depth);
        const uint8_t* Table(const char* tag, size_t& size) const;

        std::vector<uint8_t> file_, directory_;
        std::map<std::string, std::pair<uint32_t, uint32_t>> tables_;
        std::vector<uint32_t> locations_;
        std::map<uint32_t, std::vector<uint8_t>> pages_;
        size_t glyphBase_ = 0, glyphSize_ = 0;
        std::map<char32_t, uint32_t> cmap_;
        std::vector<uint16_t> advances_;
        std::string name_;
        int unitsPerEm_ = 2048, ascent_ = 0, descent_ = 0, lineGap_ = 0;
        std::map<uint64_t, GlyphBitmap> cache_;
        std::mutex cacheMutex_; // the overlay is drawn in bands on several threads (cache_ and pages_)
    };
}
