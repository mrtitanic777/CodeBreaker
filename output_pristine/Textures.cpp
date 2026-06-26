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

int Textures::load(const char* path) {
    // Check if already loaded
    for (int i = 0; i < s_count; ++i) {
        if (_stricmp(s_paths[i], path) == 0) {
            return i;
        }
    }

    if (s_count >= MAX_TEXTURES) {
        DbgPrintF("ERROR: Texture limit reached (%d), cannot load: %s\n", MAX_TEXTURES, path);
        return -1;
    }

    int id = s_count;
    strncpy(s_paths[id], path, 255);
    s_paths[id][255] = 0;

    HRESULT hr = D3DXCreateTextureFromFileExA(
        g_pd3dDevice, path,
        D3DX_DEFAULT, D3DX_DEFAULT,  // width, height from file
        1,                            // 1 mip level
        0,                            // usage
        D3DFMT_A8R8G8B8,             // force 32-bit ARGB
        D3DPOOL_DEFAULT,
        D3DX_FILTER_NONE,            // no filter on load
        D3DX_FILTER_NONE,            // no mip filter
        0xFF000000,                  // color key (black = transparent)
        NULL, NULL,
        &s_textures[id]);

    if (FAILED(hr)) {
        DbgPrintF("WARNING: Failed to load texture: %s (hr=0x%08X)\n", path, hr);
        s_textures[id] = NULL;
    } else {
        DbgPrintF("Loaded texture [%d]: %s\n", id, path);
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
        "game:\\Content\\char.png",
        "game:\\Content\\default.png",
        "game:\\Content\\particles.png",
        "game:\\Content\\clouds.png",
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
    DbgPrintF("Loading %d textures from Content folder...\n", count);
    for (int i = 0; i < count; ++i) {
        load(texturePaths[i]);
    }
    DbgPrintF("Texture loading complete: %d/%d loaded\n", s_count, count);
}
