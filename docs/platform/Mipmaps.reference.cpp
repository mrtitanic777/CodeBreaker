#include "stdafx.h"
#include "Mipmaps.h"
#include <cmath>

//==============================================================================
// MipmapManager Static Members
//==============================================================================
bool MipmapManager::s_enabled = true;
float MipmapManager::s_lodBias = -3.0f;
int MipmapManager::s_anisotropicLevel = 1;

//==============================================================================
// MipmapGenerator Implementation
//==============================================================================

int MipmapGenerator::calculateMipLevels(int width, int height)
{
    int levels = 1;
    while (width > 1 || height > 1)
    {
        width = (width > 1) ? width / 2 : 1;
        height = (height > 1) ? height / 2 : 1;
        ++levels;
    }
    return levels;
}

bool MipmapGenerator::generateMipmaps(IDirect3DTexture9* texture, FilterType filter)
{
    // Use atlas mipmaps with tileSize=0 to indicate non-atlas (full-image filtering)
    if (texture == NULL || g_pd3dDevice == NULL)
        return false;
    
    D3DSURFACE_DESC desc;
    if (FAILED(texture->GetLevelDesc(0, &desc)))
        return false;
    
    int numLevels = texture->GetLevelCount();
    if (numLevels <= 1)
        return false;
    
    int width = desc.Width;
    int height = desc.Height;
    
    D3DLOCKED_RECT srcRect;
    if (FAILED(texture->LockRect(0, &srcRect, NULL, D3DLOCK_READONLY)))
        return false;
    
    unsigned int* srcPixels = (unsigned int*)srcRect.pBits;
    int srcPitch = srcRect.Pitch / sizeof(unsigned int);
    
    unsigned int* srcBuffer = NULL;
    if (srcPitch != width)
    {
        srcBuffer = new unsigned int[width * height];
        for (int y = 0; y < height; ++y)
            memcpy(&srcBuffer[y * width], &srcPixels[y * srcPitch], width * sizeof(unsigned int));
        srcPixels = srcBuffer;
    }
    
    int mipWidth = width;
    int mipHeight = height;
    
    for (int level = 1; level < numLevels; ++level)
    {
        int newWidth = (mipWidth > 1) ? mipWidth / 2 : 1;
        int newHeight = (mipHeight > 1) ? mipHeight / 2 : 1;
        
        D3DLOCKED_RECT dstRect;
        if (FAILED(texture->LockRect(level, &dstRect, NULL, 0)))
        {
            texture->UnlockRect(0);
            delete[] srcBuffer;
            return false;
        }
        
        unsigned int* dstPixels = (unsigned int*)dstRect.pBits;
        int dstPitch = dstRect.Pitch / sizeof(unsigned int);
        
        unsigned int* tempBuffer = new unsigned int[newWidth * newHeight];
        
        switch (filter)
        {
        case FILTER_BILINEAR:
            bilinearFilter(srcPixels, mipWidth, mipHeight, tempBuffer, newWidth, newHeight);
            break;
        case FILTER_KAISER:
            kaiserFilter(srcPixels, mipWidth, mipHeight, tempBuffer, newWidth, newHeight);
            break;
        case FILTER_BOX:
        default:
            boxFilter(srcPixels, mipWidth, mipHeight, tempBuffer, newWidth, newHeight);
            break;
        }
        
        for (int y = 0; y < newHeight; ++y)
            memcpy(&dstPixels[y * dstPitch], &tempBuffer[y * newWidth], newWidth * sizeof(unsigned int));
        
        texture->UnlockRect(level);
        
        delete[] srcBuffer;
        srcBuffer = tempBuffer;
        srcPixels = srcBuffer;
        mipWidth = newWidth;
        mipHeight = newHeight;
    }
    
    texture->UnlockRect(0);
    delete[] srcBuffer;
    
    return true;
}

//==============================================================================
// Atlas-aware mipmap generation
// Downsamples each tile independently to prevent cross-tile color bleeding
//==============================================================================
bool MipmapGenerator::generateAtlasMipmaps(IDirect3DTexture9* texture, int tileSize, int startLevel, FilterType filter)
{
    if (texture == NULL || g_pd3dDevice == NULL || tileSize <= 0)
        return false;
    
    D3DSURFACE_DESC desc;
    if (FAILED(texture->GetLevelDesc(0, &desc)))
        return false;
    
    int numLevels = texture->GetLevelCount();
    if (numLevels <= 1)
        return false;
    
    int atlasWidth = desc.Width;
    int atlasHeight = desc.Height;
    int tilesX = atlasWidth / tileSize;
    int tilesY = atlasHeight / tileSize;
    
    // We need to read from each previous level to generate the next
    // Start by reading the level before startLevel
    int prevLevel = startLevel - 1;
    
    int prevWidth = atlasWidth;
    int prevHeight = atlasHeight;
    for (int i = 0; i < prevLevel; ++i)
    {
        prevWidth = (prevWidth > 1) ? prevWidth / 2 : 1;
        prevHeight = (prevHeight > 1) ? prevHeight / 2 : 1;
    }
    
    // Read the source level into a buffer
    D3DLOCKED_RECT srcRect;
    if (FAILED(texture->LockRect(prevLevel, &srcRect, NULL, D3DLOCK_READONLY)))
        return false;
    
    unsigned int* srcData = (unsigned int*)srcRect.pBits;
    int srcPitch = srcRect.Pitch / sizeof(unsigned int);
    
    unsigned int* prevBuffer = new unsigned int[prevWidth * prevHeight];
    for (int y = 0; y < prevHeight; ++y)
        memcpy(&prevBuffer[y * prevWidth], &srcData[y * srcPitch], prevWidth * sizeof(unsigned int));
    
    texture->UnlockRect(prevLevel);
    
    int curWidth = prevWidth;
    int curHeight = prevHeight;
    
    for (int level = startLevel; level < numLevels; ++level)
    {
        int newWidth = (curWidth > 1) ? curWidth / 2 : 1;
        int newHeight = (curHeight > 1) ? curHeight / 2 : 1;
        
        // Calculate tile size at this mip level
        int curTileW = curWidth / tilesX;
        int curTileH = curHeight / tilesY;
        int newTileW = newWidth / tilesX;
        int newTileH = newHeight / tilesY;
        
        // If tiles have shrunk to 0, we can't subdivide further per-tile
        // Fall back to whole-image filtering
        if (curTileW < 2 || curTileH < 2 || newTileW < 1 || newTileH < 1)
        {
            // Just do a regular box filter for remaining levels
            D3DLOCKED_RECT dstRect;
            if (FAILED(texture->LockRect(level, &dstRect, NULL, 0)))
            {
                delete[] prevBuffer;
                return false;
            }
            
            unsigned int* dstData = (unsigned int*)dstRect.pBits;
            int dstPitch = dstRect.Pitch / sizeof(unsigned int);
            
            unsigned int* tempBuffer = new unsigned int[newWidth * newHeight];
            boxFilter(prevBuffer, curWidth, curHeight, tempBuffer, newWidth, newHeight);
            
            for (int y = 0; y < newHeight; ++y)
                memcpy(&dstData[y * dstPitch], &tempBuffer[y * newWidth], newWidth * sizeof(unsigned int));
            
            texture->UnlockRect(level);
            
            delete[] prevBuffer;
            prevBuffer = tempBuffer;
            curWidth = newWidth;
            curHeight = newHeight;
            continue;
        }
        
        // Allocate destination buffer
        unsigned int* newBuffer = new unsigned int[newWidth * newHeight];
        memset(newBuffer, 0, newWidth * newHeight * sizeof(unsigned int));
        
        // Process each tile independently
        for (int ty = 0; ty < tilesY; ++ty)
        {
            for (int tx = 0; tx < tilesX; ++tx)
            {
                // Source tile region in prevBuffer
                int srcTileX = tx * curTileW;
                int srcTileY = ty * curTileH;
                
                // Destination tile region in newBuffer
                int dstTileX = tx * newTileW;
                int dstTileY = ty * newTileH;
                
                // Box filter this tile: average 2x2 source pixels -> 1 dest pixel
                // Alpha-weighted to handle transparent pixels correctly
                for (int py = 0; py < newTileH; ++py)
                {
                    for (int px = 0; px < newTileW; ++px)
                    {
                        int sx = srcTileX + px * 2;
                        int sy = srcTileY + py * 2;
                        
                        // Gather 2x2 source pixels (clamped within tile)
                        unsigned int p00 = prevBuffer[sy * curWidth + sx];
                        unsigned int p10 = (px * 2 + 1 < curTileW) ? prevBuffer[sy * curWidth + sx + 1] : p00;
                        unsigned int p01 = (py * 2 + 1 < curTileH) ? prevBuffer[(sy + 1) * curWidth + sx] : p00;
                        unsigned int p11 = (px * 2 + 1 < curTileW && py * 2 + 1 < curTileH) ?
                                           prevBuffer[(sy + 1) * curWidth + sx + 1] : p00;
                        
                        // Extract alpha values
                        unsigned int a0 = (p00 >> 24) & 0xFF;
                        unsigned int a1 = (p10 >> 24) & 0xFF;
                        unsigned int a2 = (p01 >> 24) & 0xFF;
                        unsigned int a3 = (p11 >> 24) & 0xFF;
                        
                        unsigned int totalAlpha = a0 + a1 + a2 + a3;
                        
                        if (totalAlpha == 0)
                        {
                            // All transparent - output transparent
                            newBuffer[(dstTileY + py) * newWidth + (dstTileX + px)] = 0x00000000;
                        }
                        else
                        {
                            // Alpha-weighted average: weight RGB by each pixel's alpha
                            // This prevents transparent black pixels from darkening the result
                            unsigned int r = (((p00 >> 16) & 0xFF) * a0 + ((p10 >> 16) & 0xFF) * a1 +
                                             ((p01 >> 16) & 0xFF) * a2 + ((p11 >> 16) & 0xFF) * a3) / totalAlpha;
                            unsigned int g = (((p00 >> 8) & 0xFF) * a0 + ((p10 >> 8) & 0xFF) * a1 +
                                             ((p01 >> 8) & 0xFF) * a2 + ((p11 >> 8) & 0xFF) * a3) / totalAlpha;
                            unsigned int b = ((p00 & 0xFF) * a0 + (p10 & 0xFF) * a1 +
                                             (p01 & 0xFF) * a2 + (p11 & 0xFF) * a3) / totalAlpha;
                            unsigned int a = totalAlpha / 4;
                            
                            // Clamp
                            if (r > 255) r = 255;
                            if (g > 255) g = 255;
                            if (b > 255) b = 255;
                            
                            newBuffer[(dstTileY + py) * newWidth + (dstTileX + px)] =
                                (a << 24) | (r << 16) | (g << 8) | b;
                        }
                    }
                }
            }
        }
        
        // Write to texture
        D3DLOCKED_RECT dstRect;
        if (FAILED(texture->LockRect(level, &dstRect, NULL, 0)))
        {
            delete[] prevBuffer;
            delete[] newBuffer;
            return false;
        }
        
        unsigned int* dstData = (unsigned int*)dstRect.pBits;
        int dstPitch = dstRect.Pitch / sizeof(unsigned int);
        
        for (int y = 0; y < newHeight; ++y)
            memcpy(&dstData[y * dstPitch], &newBuffer[y * newWidth], newWidth * sizeof(unsigned int));
        
        texture->UnlockRect(level);
        
        char buf[128];
        sprintf(buf, "  Generated atlas mip level %d (%dx%d, tile %dx%d)\n", level, newWidth, newHeight, newTileW, newTileH);
        DbgPrint(buf);
        
        delete[] prevBuffer;
        prevBuffer = newBuffer;
        curWidth = newWidth;
        curHeight = newHeight;
    }
    
    delete[] prevBuffer;
    return true;
}

bool MipmapGenerator::generateMipmapsRaw(
    const unsigned int* srcPixels,
    int srcWidth,
    int srcHeight,
    unsigned int** dstPixels,
    int* numLevels,
    FilterType filter)
{
    if (srcPixels == NULL || dstPixels == NULL || numLevels == NULL)
        return false;
    
    *numLevels = calculateMipLevels(srcWidth, srcHeight);
    
    // Allocate array for all mip levels
    *dstPixels = NULL;  // TODO: Implement raw mipmap generation
    
    return true;
}

void MipmapGenerator::boxFilter(
    const unsigned int* src, int srcWidth, int srcHeight,
    unsigned int* dst, int dstWidth, int dstHeight)
{
    // Simple 2x2 box filter - average 4 pixels into 1
    for (int y = 0; y < dstHeight; ++y)
    {
        for (int x = 0; x < dstWidth; ++x)
        {
            int srcX = x * 2;
            int srcY = y * 2;
            
            // Get 4 source pixels
            unsigned int p00 = src[srcY * srcWidth + srcX];
            unsigned int p10 = (srcX + 1 < srcWidth) ? src[srcY * srcWidth + srcX + 1] : p00;
            unsigned int p01 = (srcY + 1 < srcHeight) ? src[(srcY + 1) * srcWidth + srcX] : p00;
            unsigned int p11 = (srcX + 1 < srcWidth && srcY + 1 < srcHeight) ? 
                               src[(srcY + 1) * srcWidth + srcX + 1] : p00;
            
            // Average each channel
            unsigned int a = ((p00 >> 24) + (p10 >> 24) + (p01 >> 24) + (p11 >> 24)) / 4;
            unsigned int r = (((p00 >> 16) & 0xFF) + ((p10 >> 16) & 0xFF) + 
                             ((p01 >> 16) & 0xFF) + ((p11 >> 16) & 0xFF)) / 4;
            unsigned int g = (((p00 >> 8) & 0xFF) + ((p10 >> 8) & 0xFF) + 
                             ((p01 >> 8) & 0xFF) + ((p11 >> 8) & 0xFF)) / 4;
            unsigned int b = ((p00 & 0xFF) + (p10 & 0xFF) + (p01 & 0xFF) + (p11 & 0xFF)) / 4;
            
            dst[y * dstWidth + x] = (a << 24) | (r << 16) | (g << 8) | b;
        }
    }
}

void MipmapGenerator::bilinearFilter(
    const unsigned int* src, int srcWidth, int srcHeight,
    unsigned int* dst, int dstWidth, int dstHeight)
{
    // TODO: Implement bilinear filtering
    // For now, fall back to box filter
    boxFilter(src, srcWidth, srcHeight, dst, dstWidth, dstHeight);
}

void MipmapGenerator::kaiserFilter(
    const unsigned int* src, int srcWidth, int srcHeight,
    unsigned int* dst, int dstWidth, int dstHeight)
{
    // TODO: Implement Kaiser-windowed sinc filtering
    // For now, fall back to box filter
    boxFilter(src, srcWidth, srcHeight, dst, dstWidth, dstHeight);
}

//==============================================================================
// MipmapManager Implementation
//==============================================================================

void MipmapManager::init()
{
    s_enabled = true;
    s_lodBias = -1.0f;       // Negative bias = prefer sharper mip levels
    s_anisotropicLevel = 1;  // Off by default, can enable if needed
}

void MipmapManager::shutdown()
{
    // Nothing to clean up
}

void MipmapManager::setEnabled(bool enable)
{
    s_enabled = enable;
}

bool MipmapManager::isEnabled()
{
    return s_enabled;
}

void MipmapManager::setLODBias(float bias)
{
    // Clamp to valid range
    if (bias < -4.0f) bias = -4.0f;
    if (bias > 4.0f) bias = 4.0f;
    s_lodBias = bias;
}

float MipmapManager::getLODBias()
{
    return s_lodBias;
}

void MipmapManager::setAnisotropicLevel(int level)
{
    // Clamp to valid range (1-16)
    if (level < 1) level = 1;
    if (level > 16) level = 16;
    s_anisotropicLevel = level;
}

int MipmapManager::getAnisotropicLevel()
{
    return s_anisotropicLevel;
}

void MipmapManager::applySettings()
{
    if (g_pd3dDevice == NULL)
        return;
    
    if (s_enabled)
    {
        // MAG filter: POINT - preserves pixel art crispness up close
        // MIN filter: POINT with mip selection - sharp within each mip level
        // MIP filter: LINEAR - smooth transitions between mip levels (trilinear-ish)
        g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
        g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
        g_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
        
        // Set LOD bias (negative = prefer sharper/lower mip levels)
        g_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPMAPLODBIAS, *(DWORD*)&s_lodBias);
        
        // Set anisotropic filtering if requested
        if (s_anisotropicLevel > 1)
        {
            g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
            g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, s_anisotropicLevel);
        }
    }
    else
    {
        // Disable mipmapping - use point filtering for pixel art
        g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
        g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
        g_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
    }
}