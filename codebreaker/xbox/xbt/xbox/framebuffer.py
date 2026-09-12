"""GPU framebuffer capture — read console display buffer and save as image."""

import struct
from pathlib import Path
from typing import Optional


def capture_framebuffer(probe, base_addr: int = 0xD0000000, width: int = 1280,
                        height: int = 720, pixel_format: str = "XRGB8888",
                        callback=None) -> Optional[bytes]:
    """Capture raw framebuffer data from console GPU memory.
    
    Note: The actual framebuffer location and format depend on the current
    display mode and what D3D has configured. The default address 0xD0000000
    is the GPU EDRAM base — actual framebuffer may be elsewhere.
    
    Args:
        probe: ConsoleLink instance
        base_addr: GPU framebuffer base address
        width: Display width in pixels
        height: Display height in pixels
        pixel_format: Pixel format (XRGB8888 = 4 bytes/pixel)
        callback: Progress callback(bytes_read, total_bytes)
    
    Returns: Raw pixel data or None
    """
    bpp = 4  # bytes per pixel for XRGB8888
    total = width * height * bpp

    return probe.dump_range(base_addr, total, callback=callback)


def raw_to_ppm(data: bytes, width: int, height: int, output: str) -> bool:
    """Convert raw XRGB8888 framebuffer to PPM image (portable, no PIL needed).
    
    PPM is readable by most image viewers and can be converted to PNG later.
    """
    bpp = 4
    expected = width * height * bpp
    if len(data) < expected:
        return False

    with open(output, "wb") as f:
        # PPM header
        f.write(f"P6\n{width} {height}\n255\n".encode())

        # Convert XRGB8888 to RGB
        for i in range(0, expected, bpp):
            # Xbox 360 is big-endian: bytes are X, R, G, B
            # But GPU framebuffer may be tiled/swizzled
            r = data[i + 1] if i + 1 < len(data) else 0
            g = data[i + 2] if i + 2 < len(data) else 0
            b = data[i + 3] if i + 3 < len(data) else 0
            f.write(bytes([r, g, b]))

    return True


def raw_to_bmp(data: bytes, width: int, height: int, output: str) -> bool:
    """Convert raw XRGB8888 framebuffer to BMP image.
    
    BMP is universally supported and doesn't need any libraries.
    """
    bpp = 4
    expected = width * height * bpp
    if len(data) < expected:
        return False

    row_size = width * 3
    # BMP rows must be padded to 4-byte boundary
    row_padding = (4 - (row_size % 4)) % 4
    padded_row_size = row_size + row_padding

    pixel_data_size = padded_row_size * height
    file_size = 54 + pixel_data_size  # 54-byte header

    with open(output, "wb") as f:
        # BMP file header (14 bytes)
        f.write(b"BM")
        f.write(struct.pack("<I", file_size))
        f.write(struct.pack("<HH", 0, 0))
        f.write(struct.pack("<I", 54))

        # DIB header (40 bytes)
        f.write(struct.pack("<I", 40))
        f.write(struct.pack("<i", width))
        f.write(struct.pack("<i", -height))  # Negative = top-down
        f.write(struct.pack("<HH", 1, 24))   # 1 plane, 24bpp
        f.write(struct.pack("<I", 0))         # No compression
        f.write(struct.pack("<I", pixel_data_size))
        f.write(struct.pack("<ii", 2835, 2835))  # 72 DPI
        f.write(struct.pack("<II", 0, 0))

        # Pixel data (BGR for BMP)
        for y in range(height):
            for x in range(width):
                idx = (y * width + x) * bpp
                r = data[idx + 1] if idx + 1 < len(data) else 0
                g = data[idx + 2] if idx + 2 < len(data) else 0
                b = data[idx + 3] if idx + 3 < len(data) else 0
                f.write(bytes([b, g, r]))  # BMP uses BGR
            f.write(b"\x00" * row_padding)

    return True
