// Main.cpp — Xbox 360 entry point for Minecraft ALPHA (handwritten_alpha override).
// The converter's default Main.cpp is a PLACEHOLDER that only clears the screen blue and
// never creates the game -> that was the "blue screen on boot". This version mirrors the
// proven indev boot: init D3D9 + shaders + textures, run the Java static{} initializers
// (the converter emits initStatics() but never calls them), then create Minecraft and
// drive run() (which performs the rest of init and owns the tick+render+Present loop).

#include "stdafx.h"
#include "Common.h"
#include "Shaders.h"
#include "Tesselator.h"
#include "Textures.h"
#include "Minecraft.h"
// Classes whose Java `static {}` blocks became initStatics() (must be called by hand):
#include "MathHelper.h"
#include "Block.h"
#include "EntityList.h"
#include "TileEntity.h"
#include "MobSpawnerBase.h"
#include "OsMap.h"
#include "EnumSkyBlock.h"
#include "EnumCreatureType.h"
#include "EnumMobType.h"
#include "EnumArt.h"
#include "EnumOS1.h"
#include "EnumOS2.h"

// Globals
IDirect3D9*        g_pD3D           = NULL;
IDirect3DDevice9*  g_pd3dDevice     = NULL;
IDirect3DTexture9* g_terrainTexture = NULL;
IDirect3DTexture9* g_guiTexture     = NULL;
IDirect3DTexture9* g_charTexture    = NULL;
IDirect3DTexture9* g_defaultTexture = NULL;

// The game instance (created in main, run() drives everything).
Minecraft* g_minecraft = NULL;

static bool InitD3D() {
    g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (!g_pD3D) return false;

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.BackBufferWidth        = SCREEN_WIDTH;
    d3dpp.BackBufferHeight       = SCREEN_HEIGHT;
    d3dpp.BackBufferFormat       = D3DFMT_X8R8G8B8;
    d3dpp.BackBufferCount        = 1;
    d3dpp.MultiSampleType        = D3DMULTISAMPLE_NONE;
    d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow          = NULL;
    d3dpp.Windowed               = FALSE;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
    d3dpp.Flags                  = 0;
    d3dpp.FullScreen_RefreshRateInHz = 60;
    d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;

    HRESULT hr = g_pD3D->CreateDevice(0, D3DDEVTYPE_HAL, NULL,
        D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice);
    return SUCCEEDED(hr);
}

static void ApplyDefaultRenderState() {
    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
    g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    g_pd3dDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
    g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
    g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    g_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void __cdecl main() {
    if (!InitD3D()) { DbgPrintF("[BOOT] D3D init failed\n"); return; }
    ApplyDefaultRenderState();

    if (!Shaders::init()) { DbgPrintF("[BOOT] shader compile failed\n"); return; }

    Textures::init();
    Textures::loadAll();
    g_terrainTexture = Textures::getById(Textures::load("game:\\Content\\terrain.png"));
    g_guiTexture     = Textures::getById(Textures::load("game:\\Content\\gui\\gui.png"));
    g_charTexture    = Textures::getById(Textures::load("game:\\Content\\mob\\char.png"));
    g_defaultTexture = Textures::getById(Textures::load("game:\\Content\\default.png"));
    DbgPrintF("[BOOT] D3D + shaders + textures ok\n");

    // ---- Java static{} initializers (converter emits these but never calls them; without
    // them every enum singleton / Block / registry is NULL -> first use jumps a null vtable).
    // Enums first (no deps), then MathHelper, Block (file-scope statics + extra setup),
    // entity/tile registries, then MobSpawnerBase biome lookup (needs EnumCreatureType).
    EnumSkyBlock::initStatics();
    EnumCreatureType::initStatics();
    EnumMobType::initStatics();
    EnumArt::initStatics();
    EnumOS1::initStatics();
    EnumOS2::initStatics();
    MathHelper::initStatics();
    Block::initStatics();
    EntityList::initStatics();
    TileEntity::initStatics();
    MobSpawnerBase::initStatics();
    OsMap::initStatics();
    DbgPrintF("[BOOT] static initializers ok; creating Minecraft\n");

    // Create + run. The default Minecraft() ctor is fully populated (its init list creates
    // m_sndManager, the renderers, the timer, etc.), so it is safe to use; we just supply
    // the display size the parameterized AWT ctor would have set. run() does the remaining
    // init (game settings, render engine, font, main menu) and owns the loop until BACK.
    g_minecraft = new Minecraft();
    g_minecraft->m_displayWidth  = SCREEN_WIDTH;
    g_minecraft->m_displayHeight = SCREEN_HEIGHT;
    DbgPrintF("[BOOT] Minecraft ctor ok; calling run()\n");
    g_minecraft->run();
    DbgPrintF("[BOOT] run() returned\n");

    // Cleanup
    Shaders::shutdown();
    Textures::shutdown();
    if (g_pd3dDevice) g_pd3dDevice->Release();
    if (g_pD3D)       g_pD3D->Release();
}
