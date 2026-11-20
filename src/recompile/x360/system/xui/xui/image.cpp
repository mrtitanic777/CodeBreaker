#include "xui/image.h"

#include "xui/inflate.h"

#include <windows.h>
#include <shlwapi.h>
#include <wincodec.h>
#include <wrl/client.h>

#include <cstring>
#include <format>

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "windowscodecs.lib")

namespace xui
{
    namespace
    {
        uint32_t Be32(const uint8_t* p) { return uint32_t(p[0]) << 24 | uint32_t(p[1]) << 16 | uint32_t(p[2]) << 8 | p[3]; }

        uint32_t Crc32(const uint8_t* data, size_t size)
        {
            static const auto table = [] {
                std::vector<uint32_t> t(256);
                for (uint32_t n = 0; n < 256; n++)
                {
                    uint32_t c = n;
                    for (int k = 0; k < 8; k++)
                        c = (c & 1) ? 0xEDB88320u ^ (c >> 1) : c >> 1;
                    t[n] = c;
                }
                return t;
            }();
            uint32_t c = 0xFFFFFFFFu;
            for (size_t i = 0; i < size; i++)
                c = table[(c ^ data[i]) & 0xFF] ^ (c >> 8);
            return c ^ 0xFFFFFFFFu;
        }

        uint8_t Paeth(int a, int b, int c)
        {
            const int p = a + b - c, pa = std::abs(p - a), pb = std::abs(p - b), pc = std::abs(p - c);
            return uint8_t(pa <= pb && pa <= pc ? a : pb <= pc ? b : c);
        }

        // Undoes the filters of one (sub)image in place; `rowBytes` excludes the filter byte.
        bool Unfilter(uint8_t* data, uint32_t rows, size_t rowBytes, size_t bpp, std::vector<uint8_t>& out)
        {
            out.assign(rows * rowBytes, 0);
            const uint8_t* prior = nullptr;
            for (uint32_t y = 0; y < rows; y++)
            {
                const uint8_t filter = data[y * (rowBytes + 1)];
                const uint8_t* in = data + y * (rowBytes + 1) + 1;
                uint8_t* row = out.data() + y * rowBytes;
                for (size_t x = 0; x < rowBytes; x++)
                {
                    const int a = x >= bpp ? row[x - bpp] : 0;
                    const int b = prior ? prior[x] : 0;
                    const int c = prior && x >= bpp ? prior[x - bpp] : 0;
                    int v = in[x];
                    switch (filter)
                    {
                    case 0: break;
                    case 1: v += a; break;
                    case 2: v += b; break;
                    case 3: v += (a + b) / 2; break;
                    case 4: v += Paeth(a, b, c); break;
                    default: return false;
                    }
                    row[x] = uint8_t(v);
                }
                prior = row;
            }
            return true;
        }
    }

    bool DecodePng(const uint8_t* data, size_t size, Image& out, std::string& error)
    {
        static const uint8_t kSignature[8] = { 0x89, 'P', 'N', 'G', '\r', '\n', 0x1A, '\n' };
        if (size < 8 || std::memcmp(data, kSignature, 8) != 0)
            return (error = "not a PNG", false);
        uint32_t width = 0, height = 0;
        uint8_t depth = 0, colorType = 0, interlace = 0;
        std::vector<uint8_t> palette, transparency, idat;
        bool haveHeader = false, ended = false;
        size_t at = 8;
        while (at + 12 <= size && !ended)
        {
            const uint32_t length = Be32(data + at);
            if (at + 12 + uint64_t(length) > size)
                return (error = "chunk runs past the end", false);
            const uint8_t* type = data + at + 4;
            const uint8_t* body = data + at + 8;
            if (Crc32(type, length + 4) != Be32(body + length))
                return (error = std::format("bad CRC on chunk {}", std::string(reinterpret_cast<const char*>(type), 4)),
                        false);
            const std::string tag(reinterpret_cast<const char*>(type), 4);
            if (tag == "IHDR")
            {
                if (length != 13)
                    return (error = "bad IHDR", false);
                width = Be32(body);
                height = Be32(body + 4);
                depth = body[8];
                colorType = body[9];
                interlace = body[12];
                if (body[10] != 0 || body[11] != 0 || interlace > 1 || width == 0 || height == 0 ||
                    width > 16384 || height > 16384)
                    return (error = "unsupported IHDR", false);
                haveHeader = true;
            }
            else if (tag == "PLTE")
                palette.assign(body, body + length);
            else if (tag == "tRNS")
                transparency.assign(body, body + length);
            else if (tag == "IDAT")
                idat.insert(idat.end(), body, body + length);
            else if (tag == "IEND")
                ended = true;
            else if (!(type[0] & 0x20))
                return (error = "unknown critical chunk " + tag, false);
            at += 12 + length;
        }
        if (!haveHeader || !ended)
            return (error = "missing IHDR or IEND", false);
        const int channels = colorType == 0 ? 1 : colorType == 2 ? 3 : colorType == 3 ? 1 : colorType == 4 ? 2 :
            colorType == 6 ? 4 : 0;
        const bool depthOk = colorType == 0 ? (depth == 1 || depth == 2 || depth == 4 || depth == 8 || depth == 16) :
            colorType == 3 ? (depth == 1 || depth == 2 || depth == 4 || depth == 8) : (depth == 8 || depth == 16);
        if (!channels || !depthOk)
            return (error = std::format("colour type {} depth {}", colorType, depth), false);
        if (colorType == 3 && (palette.empty() || palette.size() % 3))
            return (error = "missing palette", false);

        std::vector<uint8_t> raw;
        if (!Inflate(idat.data(), idat.size(), raw, true))
            return (error = "bad image data", false);

        const size_t bitsPerPixel = size_t(channels) * depth;
        const size_t bpp = std::max<size_t>(1, bitsPerPixel / 8);
        out.width = width;
        out.height = height;
        out.rgba.assign(size_t(width) * height * 4, 0);

        // Converts one sample row of a (sub)image to RGBA at the given output positions.
        auto sample = [&](const uint8_t* row, uint32_t i) -> uint32_t {
            auto get = [&](uint32_t index) -> uint32_t { // sample `index` of the row, scaled to its bit depth
                if (depth == 8)
                    return row[index];
                if (depth == 16)
                    return uint32_t(row[index * 2]) << 8 | row[index * 2 + 1];
                const size_t bit = size_t(index) * depth;
                return (row[bit / 8] >> (8 - depth - bit % 8)) & ((1u << depth) - 1);
            };
            auto to8 = [&](uint32_t v) -> uint8_t {
                if (depth == 16)
                    return uint8_t(v >> 8);
                return uint8_t(v * 255 / ((1u << depth) - 1));
            };
            uint8_t r, g, b, a = 255;
            switch (colorType)
            {
            case 0:
            {
                const uint32_t v = get(i);
                r = g = b = to8(v);
                if (transparency.size() >= 2 && v == (uint32_t(transparency[0]) << 8 | transparency[1]))
                    a = 0;
                break;
            }
            case 2:
            {
                const uint32_t vr = get(i * 3), vg = get(i * 3 + 1), vb = get(i * 3 + 2);
                r = to8(vr);
                g = to8(vg);
                b = to8(vb);
                if (transparency.size() >= 6 && vr == (uint32_t(transparency[0]) << 8 | transparency[1]) &&
                    vg == (uint32_t(transparency[2]) << 8 | transparency[3]) &&
                    vb == (uint32_t(transparency[4]) << 8 | transparency[5]))
                    a = 0;
                break;
            }
            case 3:
            {
                const uint32_t index = get(i);
                if (index * 3 + 2 >= palette.size())
                    r = g = b = 0;
                else
                {
                    r = palette[index * 3];
                    g = palette[index * 3 + 1];
                    b = palette[index * 3 + 2];
                }
                if (index < transparency.size())
                    a = transparency[index];
                break;
            }
            case 4:
                r = g = b = to8(get(i * 2));
                a = to8(get(i * 2 + 1));
                break;
            default:
                r = to8(get(i * 4));
                g = to8(get(i * 4 + 1));
                b = to8(get(i * 4 + 2));
                a = to8(get(i * 4 + 3));
                break;
            }
            return uint32_t(r) | uint32_t(g) << 8 | uint32_t(b) << 16 | uint32_t(a) << 24;
        };

        // Adam7 passes (or the whole image): start x/y and steps.
        static const uint32_t kPasses[7][4] = { { 0, 0, 8, 8 }, { 4, 0, 8, 8 }, { 0, 4, 4, 8 }, { 2, 0, 4, 4 },
                                                { 0, 2, 2, 4 }, { 1, 0, 2, 2 }, { 0, 1, 1, 2 } };
        static const uint32_t kWhole[1][4] = { { 0, 0, 1, 1 } };
        const auto* passes = interlace ? kPasses : kWhole;
        const int passCount = interlace ? 7 : 1;
        size_t offset = 0;
        std::vector<uint8_t> rows;
        for (int p = 0; p < passCount; p++)
        {
            const uint32_t x0 = passes[p][0], y0 = passes[p][1], dx = passes[p][2], dy = passes[p][3];
            const uint32_t w = width > x0 ? (width - x0 + dx - 1) / dx : 0;
            const uint32_t h = height > y0 ? (height - y0 + dy - 1) / dy : 0;
            if (!w || !h)
                continue;
            const size_t rowBytes = (size_t(w) * bitsPerPixel + 7) / 8;
            if (offset + h * (rowBytes + 1) > raw.size())
                return (error = "image data too short", false);
            if (!Unfilter(raw.data() + offset, h, rowBytes, bpp, rows))
                return (error = "bad filter type", false);
            offset += h * (rowBytes + 1);
            for (uint32_t y = 0; y < h; y++)
                for (uint32_t x = 0; x < w; x++)
                {
                    const uint32_t v = sample(rows.data() + y * rowBytes, x);
                    std::memcpy(&out.rgba[(size_t(y0 + y * dy) * width + x0 + x * dx) * 4], &v, 4);
                }
        }
        return true;
    }

    // Any format Windows' WIC codecs read (the container by its signature).
    bool DecodeWithWic(const uint8_t* data, size_t size, Image& out, std::string& error)
    {
        using Microsoft::WRL::ComPtr;
        static ComPtr<IWICImagingFactory> factory = [] {
            CoInitializeEx(nullptr, COINIT_MULTITHREADED);
            ComPtr<IWICImagingFactory> f;
            CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&f));
            return f;
        }();
        ComPtr<IStream> stream;
        stream.Attach(SHCreateMemStream(data, UINT(size)));
        ComPtr<IWICBitmapDecoder> decoder;
        ComPtr<IWICBitmapFrameDecode> frame;
        ComPtr<IWICFormatConverter> converter;
        if (!factory || !stream ||
            FAILED(factory->CreateDecoderFromStream(stream.Get(), nullptr, WICDecodeMetadataCacheOnDemand, &decoder)) ||
            FAILED(decoder->GetFrame(0, &frame)) || FAILED(factory->CreateFormatConverter(&converter)) ||
            FAILED(converter->Initialize(frame.Get(), GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, nullptr, 0,
                                         WICBitmapPaletteTypeCustom)))
        {
            error = "WIC cannot decode the picture";
            return false;
        }
        UINT w = 0, h = 0;
        converter->GetSize(&w, &h);
        out.width = w;
        out.height = h;
        out.rgba.resize(size_t(w) * h * 4);
        if (FAILED(converter->CopyPixels(nullptr, w * 4, UINT(out.rgba.size()), out.rgba.data())))
        {
            error = "WIC cannot read the picture's pixels";
            return false;
        }
        return true;
    }

    bool DecodeImage(const uint8_t* data, size_t size, Image& out, std::string& error)
    {
        if (size >= 4 && data[0] == 0x89 && std::memcmp(data + 1, "PNG", 3) == 0)
            return DecodePng(data, size, out, error);
        return DecodeWithWic(data, size, out, error);
    }
}
