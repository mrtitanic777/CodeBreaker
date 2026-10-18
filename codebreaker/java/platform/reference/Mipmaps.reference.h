#pragma once
//==============================================================================
// Mipmaps.h
// 
// Mipmap generation and management for texture filtering
// Reduces texture aliasing and improves rendering quality at distance
//
// Direct3D 9 supports automatic mipmap generation, but for Xbox 360
// compatibility and control over quality, we may want manual generation.
//==============================================================================

#include "Common.h"

//==============================================================================
// MipmapGenerator
//
// Generates mipmaps for textures using various filtering algorithms
//==============================================================================
class MipmapGenerator
{
public:
    // Filter types for mipmap generation
    enum FilterType
    {
        FILTER_BOX,         // Simple 2x2 box filter (fastest)
        FILTER_BILINEAR,    // Bilinear interpolation
        FILTER_KAISER,      // Kaiser-windowed sinc (highest quality)
    };
    
    //--------------------------------------------------------------------------
    // Generate mipmaps for a Direct3D texture
    // 
    // Parameters:
    //   texture     - The texture to generate mipmaps for (must have mip levels)
    //   filter      - The filter algorithm to use
    //   
    // Returns:
    //   true if successful, false otherwise
    //
    // Notes:
    //   - Texture must be created with D3DUSAGE_AUTOGENMIPMAPS or have
    //     pre-allocated mip levels
    //   - For terrain.png, use FILTER_BOX to preserve pixel art appearance
    //   - For smoother textures (clouds, sky), FILTER_BILINEAR works well
    //--------------------------------------------------------------------------
    static bool generateMipmaps(IDirect3DTexture9* texture, FilterType filter = FILTER_BOX);
    
    //--------------------------------------------------------------------------
    // Generate mipmaps for a texture atlas with per-tile filtering
    // Prevents color bleeding across tile boundaries
    //
    // Parameters:
    //   texture     - The texture to generate mipmaps for (must have mip levels)
    //   tileSize    - Size of each tile in the atlas (e.g., 16 for 16x16 tiles)
    //   startLevel  - First mip level to generate (skip pre-baked levels)
    //   filter      - The filter algorithm to use
    //
    // Returns:
    //   true if successful, false otherwise
    //--------------------------------------------------------------------------
    static bool generateAtlasMipmaps(IDirect3DTexture9* texture, int tileSize, int startLevel = 1, FilterType filter = FILTER_BOX);
    
    //--------------------------------------------------------------------------
    // Generate mipmaps from raw pixel data
    //
    // Parameters:
    //   srcPixels   - Source pixel data (ARGB format, 32-bit)
    //   srcWidth    - Width of source image
    //   srcHeight   - Height of source image
    //   dstPixels   - Output array of mip level pixel data (caller must free)
    //   numLevels   - Output: number of mip levels generated
    //   filter      - The filter algorithm to use
    //
    // Returns:
    //   true if successful, false otherwise
    //--------------------------------------------------------------------------
    static bool generateMipmapsRaw(
        const unsigned int* srcPixels, 
        int srcWidth, 
        int srcHeight,
        unsigned int** dstPixels,
        int* numLevels,
        FilterType filter = FILTER_BOX
    );
    
    //--------------------------------------------------------------------------
    // Calculate the number of mip levels for a given texture size
    //
    // Parameters:
    //   width   - Texture width
    //   height  - Texture height
    //
    // Returns:
    //   Number of mip levels (including the base level)
    //--------------------------------------------------------------------------
    static int calculateMipLevels(int width, int height);

private:
    // Box filter - averages 2x2 pixels
    static void boxFilter(
        const unsigned int* src, int srcWidth, int srcHeight,
        unsigned int* dst, int dstWidth, int dstHeight
    );
    
    // Bilinear filter - smooth interpolation
    static void bilinearFilter(
        const unsigned int* src, int srcWidth, int srcHeight,
        unsigned int* dst, int dstWidth, int dstHeight
    );
    
    // Kaiser filter - high quality windowed sinc
    static void kaiserFilter(
        const unsigned int* src, int srcWidth, int srcHeight,
        unsigned int* dst, int dstWidth, int dstHeight
    );
};

//==============================================================================
// MipmapManager
//
// Manages mipmap settings for the game's textures
//==============================================================================
class MipmapManager
{
public:
    //--------------------------------------------------------------------------
    // Initialize the mipmap system
    //--------------------------------------------------------------------------
    static void init();
    
    //--------------------------------------------------------------------------
    // Shutdown the mipmap system
    //--------------------------------------------------------------------------
    static void shutdown();
    
    //--------------------------------------------------------------------------
    // Enable or disable mipmapping globally
    //
    // Parameters:
    //   enable  - true to enable mipmaps, false to disable
    //--------------------------------------------------------------------------
    static void setEnabled(bool enable);
    
    //--------------------------------------------------------------------------
    // Check if mipmapping is enabled
    //--------------------------------------------------------------------------
    static bool isEnabled();
    
    //--------------------------------------------------------------------------
    // Set the mipmap LOD bias
    //
    // Parameters:
    //   bias    - LOD bias value (-4.0 to 4.0, negative = sharper, positive = blurrier)
    //
    // Notes:
    //   - For pixel art, a slight negative bias (-0.5 to -1.0) helps keep textures sharp
    //   - For realistic textures, 0.0 is usually best
    //--------------------------------------------------------------------------
    static void setLODBias(float bias);
    
    //--------------------------------------------------------------------------
    // Get the current LOD bias
    //--------------------------------------------------------------------------
    static float getLODBias();
    
    //--------------------------------------------------------------------------
    // Apply mipmap settings to the device
    //
    // Call this before rendering to ensure correct mipmap filtering is set
    //--------------------------------------------------------------------------
    static void applySettings();
    
    //--------------------------------------------------------------------------
    // Set the maximum anisotropic filtering level
    //
    // Parameters:
    //   level   - Anisotropy level (1 = off, 2-16 = anisotropic filtering)
    //
    // Notes:
    //   - Higher values improve texture quality at oblique angles
    //   - Has performance cost, especially on Xbox 360
    //   - Recommend 2-4 for Xbox 360, 8-16 for PC
    //--------------------------------------------------------------------------
    static void setAnisotropicLevel(int level);
    
    //--------------------------------------------------------------------------
    // Get the current anisotropic filtering level
    //--------------------------------------------------------------------------
    static int getAnisotropicLevel();

private:
    static bool s_enabled;
    static float s_lodBias;
    static int s_anisotropicLevel;
};

//==============================================================================
// Usage Example:
//
// // During texture loading:
// IDirect3DTexture9* terrainTexture;
// D3DXCreateTextureFromFileEx(
//     device, "terrain.png",
//     D3DX_DEFAULT, D3DX_DEFAULT,
//     D3DX_DEFAULT,  // Generate all mip levels
//     0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED,
//     D3DX_FILTER_POINT,  // No filtering on load (preserve pixel art)
//     D3DX_FILTER_BOX,    // Box filter for mipmaps
//     0, NULL, NULL, &terrainTexture
// );
//
// // Or generate manually:
// MipmapGenerator::generateMipmaps(terrainTexture, MipmapGenerator::FILTER_BOX);
//
// // Before rendering:
// MipmapManager::setEnabled(true);
// MipmapManager::setLODBias(-0.5f);  // Keep pixel art sharp
// MipmapManager::setAnisotropicLevel(4);
// MipmapManager::applySettings();
//
// // Render with mipmapped textures...
//==============================================================================