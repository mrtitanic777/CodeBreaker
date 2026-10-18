#include "stdafx.h"
#include "Textures.h"
#include "Common.h"

IDirect3DTexture9* Textures::s_textures[MAX_TEXTURES];
char Textures::s_paths[MAX_TEXTURES][256];
int Textures::s_count = 0;

void Textures::init() {
    memset(s_textures, 0, sizeof(s_textures));
    memset(s_paths, 0, sizeof(s_paths));
    s_count = 0;
}

void Textures::shutdown() {
    for (int i = 0; i < s_count; ++i) {
        if (s_textures[i]) {
            s_textures[i]->Release();
            s_textures[i] = NULL;
        }
    }
    s_count = 0;
}

// MIPMAP (ported from Minecraft.Client loadTerrainTextureWithMipmaps): build a 3-level mip chain for
// the terrain atlas from PRE-BAKED images so distant terrain samples smaller tiles -> far less texture
// aliasing + texture-cache thrash = big perf win. The mip images are hand-downsampled PER TILE
// (terrain_MipmapLevel2/3.png) to avoid the cross-tile color bleed a naive atlas mip would cause.
//   level 0 = terrain.png            (native, e.g. 256, 16x16 tiles)
//   level 1 = terrain_MipmapLevel2.png (half,   8x8 tiles)
//   level 2 = terrain_MipmapLevel3.png (quarter, 4x4 tiles)
// All-or-nothing: if any level fails, release and let the caller fall back to the plain 1-level load.
static bool loadTerrainMipmapped(IDirect3DTexture9** out, const char* path) {
    *out = NULL;
    if (!g_pd3dDevice) return false;

    // Load mip0 (terrain.png) at native size; its LevelDesc gives the atlas dimensions.
    IDirect3DTexture9* src0 = NULL;
    if (FAILED(D3DXCreateTextureFromFileExA(g_pd3dDevice, path, D3DX_DEFAULT, D3DX_DEFAULT, 1, 0,
            D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0, NULL, NULL, &src0)) || !src0)
        return false;
    D3DSURFACE_DESC d0;
    src0->GetLevelDesc(0, &d0);
    UINT w = d0.Width, h = d0.Height;

    IDirect3DTexture9* tex = NULL;
    if (FAILED(g_pd3dDevice->CreateTexture(w, h, 3, 0, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &tex, NULL)) || !tex) {
        src0->Release();
        return false;
    }

    // Sibling mip paths live in the same directory as terrain.png.
    char dir[300];
    strncpy(dir, path, sizeof(dir) - 1); dir[sizeof(dir) - 1] = 0;
    char* cut = dir; for (char* s = dir; *s; ++s) { if (*s == '\\' || *s == '/') cut = s + 1; } *cut = 0;
    char mip1[320], mip2[320];
    strncpy(mip1, dir, sizeof(mip1) - 1); mip1[sizeof(mip1) - 1] = 0; strncat(mip1, "terrain_MipmapLevel2.png", sizeof(mip1) - strlen(mip1) - 1);
    strncpy(mip2, dir, sizeof(mip2) - 1); mip2[sizeof(mip2) - 1] = 0; strncat(mip2, "terrain_MipmapLevel3.png", sizeof(mip2) - strlen(mip2) - 1);

    bool ok = true;
    // level 0 from the already-loaded src0
    {
        IDirect3DSurface9* ss = NULL; IDirect3DSurface9* ds = NULL;
        src0->GetSurfaceLevel(0, &ss);
        tex->GetSurfaceLevel(0, &ds);
        ok = (ss && ds && SUCCEEDED(D3DXLoadSurfaceFromSurface(ds, NULL, NULL, ss, NULL, NULL, D3DX_FILTER_NONE, 0)));
        if (ss) ss->Release();
        if (ds) ds->Release();
    }
    src0->Release();

    // levels 1 and 2 from the pre-baked images
    const char* files[2] = { mip1, mip2 };
    for (int i = 0; ok && i < 2; ++i) {
        int lvl = i + 1; UINT lw = w >> lvl, lh = h >> lvl;
        IDirect3DTexture9* src = NULL;
        if (FAILED(D3DXCreateTextureFromFileExA(g_pd3dDevice, files[i], lw, lh, 1, 0,
                D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0, NULL, NULL, &src)) || !src) {
            ok = false; break;
        }
        IDirect3DSurface9* ss = NULL; IDirect3DSurface9* ds = NULL;
        src->GetSurfaceLevel(0, &ss);
        tex->GetSurfaceLevel((UINT)lvl, &ds);
        ok = (ss && ds && SUCCEEDED(D3DXLoadSurfaceFromSurface(ds, NULL, NULL, ss, NULL, NULL, D3DX_FILTER_NONE, 0)));
        if (ss) ss->Release();
        if (ds) ds->Release();
        src->Release();
    }

    if (!ok) { tex->Release(); return false; }
    *out = tex;
    return true;
}

int Textures::load(const char* path) {
    // Check if already loaded
    for (int i = 0; i < s_count; ++i) {
        if (_stricmp(s_paths[i], path) == 0) {
            return i;
        }
    }

    if (s_count >= MAX_TEXTURES) {
        return -1;
    }

    int id = s_count;
    strncpy(s_paths[id], path, 255);
    s_paths[id][255] = 0;

    // MIPMAP: the main terrain atlas (basename "terrain.png" only -- NOT terrain\sun.png / moon.png) gets
    // the pre-baked mip chain. Falls through to the normal 1-level load if the mip images aren't present.
    {
        const char* bn = path; for (const char* s = path; *s; ++s) { if (*s == '\\' || *s == '/') bn = s + 1; }
        if (_stricmp(bn, "terrain.png") == 0 && loadTerrainMipmapped(&s_textures[id], path)) {
            ++s_count;
            return id;
        }
    }

    HRESULT hr = D3DXCreateTextureFromFileExA(
        g_pd3dDevice, path,
        D3DX_DEFAULT, D3DX_DEFAULT,  // width, height from file
        1,                            // 1 mip level
        0,                            // usage
        D3DFMT_A8R8G8B8,             // force 32-bit ARGB
        D3DPOOL_DEFAULT,
        D3DX_FILTER_NONE,            // no filter on load
        D3DX_FILTER_NONE,            // no mip filter
        0,                           // PORT(★ HUD alpha): NO color key. 0xFF000000 forced every
                                     // OPAQUE-BLACK texel (heart/hotbar/button outlines, lava/terrain
                                     // detail) to alpha 0 -> corrupted HUD/world. Transparency comes
                                     // from each PNG's real alpha channel (matches the reference).
        NULL, NULL,
        &s_textures[id]);

    if (FAILED(hr)) {
        // PORT(leak-class fix): the full path didn't resolve (e.g. an unstripped "%blur%"
        // prefix, or a "misc/" subfolder the asset was flattened out of during conversion).
        // Retry with just the filename under Content root. A NULL texture here is dangerous:
        // the Xenos pixel shader always samples stage 0, so SetTexture(0, NULL) leaves the
        // PREVIOUS texture bound and the quad shows a leaked atlas (the vignette "Steve skin"
        // bug). Resolving the basename recovers vignette.png/pumpkinblur.png etc. wherever they
        // physically live under Content.
        const char* base = path;
        for (const char* s = path; *s; ++s) { if (*s == '\\' || *s == '/') base = s + 1; }
        if (base != path) {
            char alt[260];
            strncpy(alt, "game:\\Content\\", sizeof(alt) - 1);
            alt[sizeof(alt) - 1] = 0;
            strncat(alt, base, sizeof(alt) - strlen(alt) - 1);
            hr = D3DXCreateTextureFromFileExA(
                g_pd3dDevice, alt,
                D3DX_DEFAULT, D3DX_DEFAULT, 1, 0,
                D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT,
                D3DX_FILTER_NONE, D3DX_FILTER_NONE,
                0, NULL, NULL,   // PORT(HUD alpha): no color key (see main load above)
                &s_textures[id]);
            if (SUCCEEDED(hr)) {
            }
        }
    }

    if (FAILED(hr)) {
        s_textures[id] = NULL;
    } else if (s_textures[id] != NULL) {
    }

    ++s_count;
    return id;
}

IDirect3DTexture9* Textures::getById(int id) {
    if (id < 0 || id >= s_count) return NULL;
    return s_textures[id];
}

void Textures::bind(int id) {
    g_pd3dDevice->SetTexture(0, getById(id));
}

void Textures::bindDirect(IDirect3DTexture9* tex) {
    g_pd3dDevice->SetTexture(0, tex);
}

void Textures::loadAll() {
    // Pre-load all known game textures from Content folder
    static const char* texturePaths[] = {
        "game:\\Content\\terrain.png",
        "game:\\Content\\mob\\char.png",
        "game:\\Content\\default.png",
        "game:\\Content\\particles.png",
        "game:\\Content\\environment\\clouds.png",   // PORT: clouds.png lives under environment\, not Content root
        "game:\\Content\\rain.png",
        "game:\\Content\\water.png",
        "game:\\Content\\dirt.png",
        "game:\\Content\\grass.png",
        "game:\\Content\\rock.png",
        "game:\\Content\\shadow.png",
        "game:\\Content\\gui\\gui.png",
        "game:\\Content\\gui\\icons.png",
        "game:\\Content\\gui\\items.png",
        "game:\\Content\\gui\\logo.png",
        "game:\\Content\\gui\\inventory.png",
        "game:\\Content\\gui\\container.png",
        "game:\\Content\\gui\\crafting.png",
        "game:\\Content\\item\\arrows.png",
        "game:\\Content\\item\\sign.png",
        "game:\\Content\\misc\\gear.png",
        "game:\\Content\\misc\\gearmiddle.png",
        "game:\\Content\\mob\\creeper.png",
        "game:\\Content\\mob\\pig.png",
        "game:\\Content\\mob\\sheep.png",
        "game:\\Content\\mob\\sheep_fur.png",
        "game:\\Content\\mob\\skeleton.png",
        "game:\\Content\\mob\\spider.png",
        "game:\\Content\\mob\\spider_eyes.png",
        "game:\\Content\\mob\\zombie.png",
        "game:\\Content\\armor\\chain_1.png",
        "game:\\Content\\armor\\chain_2.png",
        "game:\\Content\\armor\\diamond_1.png",
        "game:\\Content\\armor\\diamond_2.png",
        "game:\\Content\\armor\\gold_1.png",
        "game:\\Content\\armor\\gold_2.png",
        "game:\\Content\\armor\\iron_1.png",
        "game:\\Content\\armor\\iron_2.png",
        "game:\\Content\\terrain\\sun.png",
        "game:\\Content\\terrain\\moon.png",
    };
    int count = sizeof(texturePaths) / sizeof(texturePaths[0]);
    for (int i = 0; i < count; ++i) {
        load(texturePaths[i]);
    }
    DbgPrintF("Texture loading complete: %d/%d loaded\n", s_count, count);
}
