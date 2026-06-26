#ifndef TEXTURES_H
#define TEXTURES_H

#include "stdafx.h"

// Xbox 360 texture manager — loads from game:\Content\ folder
// Replaces Java's RenderEngine texture system

class Textures {
public:
    static void init();
    static void shutdown();

    // Load a texture by Xbox 360 path (e.g. "game:\Content\terrain.png")
    // Returns a texture ID (index into internal array)
    static int load(const char* path);

    // Get the D3D9 texture pointer by ID
    static IDirect3DTexture9* getById(int id);

    // Bind a texture by ID to sampler stage 0
    static void bind(int id);

    // Bind a texture directly
    static void bindDirect(IDirect3DTexture9* tex);

    // Pre-load all known game textures
    static void loadAll();

    static const int MAX_TEXTURES = 128;

private:
    static IDirect3DTexture9* s_textures[MAX_TEXTURES];
    static char s_paths[MAX_TEXTURES][256];
    static int s_count;
};

#endif // TEXTURES_H
