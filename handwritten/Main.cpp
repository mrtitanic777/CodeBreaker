// Main.cpp � Xbox 360 entry point for Minecraft Indev
// Creates D3D9 device, initializes subsystems, runs game loop

#include "stdafx.h"
#include "Common.h"
#include "Shaders.h"
#include "Tesselator.h"
#include "Textures.h"
#include "Minecraft.h"
#include "Block.h"
#include "Item.h"
#include "MathHelper.h"
#include "World.h"
#include <cstdlib>   // srand/rand

// Globals
IDirect3D9*       g_pD3D       = NULL;
IDirect3DDevice9* g_pd3dDevice = NULL;
IDirect3DTexture9* g_terrainTexture = NULL;
IDirect3DTexture9* g_guiTexture     = NULL;
IDirect3DTexture9* g_charTexture    = NULL;
IDirect3DTexture9* g_defaultTexture = NULL;

// The game instance (created in main, run() drives everything).
Minecraft* g_minecraft = NULL;

// Stored present params so the "Limit framerate" option can Reset the device with a new
// presentation interval at runtime (see SetFrameLimit).
static D3DPRESENT_PARAMETERS g_d3dpp;

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

    g_d3dpp = d3dpp;   // remember for SetFrameLimit (runtime present-interval swap)

    HRESULT hr = g_pD3D->CreateDevice(0, D3DDEVTYPE_HAL, NULL,
        D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice);
    return SUCCEEDED(hr);
}

// One-time device render-state + sampler defaults. Factored out so it can be re-applied after a
// device Reset (Reset returns all render/sampler state to D3D defaults).
static void ApplyDeviceDefaults() {
    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
    g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    g_pd3dDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
    g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
    g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    g_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
    g_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
    g_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
}

// PORT: "Limit framerate" option hot-swaps the presentation interval. ON -> D3DPRESENT_INTERVAL_ONE
// (wait for vblank: caps at the 60Hz refresh, no tearing); OFF -> D3DPRESENT_INTERVAL_IMMEDIATE
// (present as fast as possible, uncapped). The interval is a device-creation parameter, so changing it
// at runtime requires Reset(). On Xbox 360 Reset() preserves resources (no PC-style lost device /
// DEFAULT-pool loss); it does reset render/sampler state, so we re-apply our defaults afterward.
void SetFrameLimit(bool limit) {
    if (!g_pd3dDevice) return;
    DWORD want = limit ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;
    if (g_d3dpp.PresentationInterval == want) return;   // already in the requested mode
    D3DPRESENT_PARAMETERS pp = g_d3dpp;
    pp.PresentationInterval = want;
    if (SUCCEEDED(g_pd3dDevice->Reset(&pp))) {
        g_d3dpp = pp;
        ApplyDeviceDefaults();
    }
}

void __cdecl main() {
    OutputDebugStringA("[BOOT] main: enter\n");
    srand((unsigned int)GetTickCount());   // PORT: seed C rand() once at boot so the random main-menu splash (and other rand() users) varies each launch
    if (!InitD3D()) {
        OutputDebugStringA("[BOOT] D3D init failed\n");
        return;
    }
    OutputDebugStringA("[BOOT] D3D ok\n");

    // Render state + sampler defaults. (WRAP addressing matters for tiled UVs -- the menu dirt
    // background and terrain; the real 360 defaults to WRAP but Nexia360's D3D12 backend doesn't,
    // so UVs > 1 sampled garbage without this.) Factored into ApplyDeviceDefaults so a present-interval
    // Reset can restore them.
    ApplyDeviceDefaults();

    if (!Shaders::init()) {
        OutputDebugStringA("[BOOT] Shader compile failed\n");
        return;
    }
    OutputDebugStringA("[BOOT] shaders ok\n");

    // Load all textures from game:\Content\ folder
    Textures::init();
    Textures::loadAll();
    OutputDebugStringA("[BOOT] textures ok\n");

    // Convenience globals for the most-used textures
    g_terrainTexture = Textures::getById(Textures::load("game:\\Content\\terrain.png"));
    g_guiTexture     = Textures::getById(Textures::load("game:\\Content\\gui\\gui.png"));
    g_charTexture    = Textures::getById(Textures::load("game:\\Content\\char.png"));
    g_defaultTexture = Textures::getById(Textures::load("game:\\Content\\default.png"));

    OutputDebugStringA("[BOOT] textures-globals ok\n");

    // PORT: Run the Java `static {}` class initializers. In Java these run automatically
    // on first class access; the converter turned each into an initStatics() function but
    // NEVER emitted calls to them, so every Block/Item static (Block::s_stone, s_dirt,
    // s_plantYellow, ...) stayed NULL. Generation limped along because data reads through a
    // NULL block pointer hit the (readable) null page and returned garbage instead of
    // faulting — but the first VIRTUAL call through a NULL block (BlockFlower::canBlockStay
    // on s_plantYellow during the "Planting" phase) jumped through a null vtable to address
    // 0 -> IP=0x0 crash. Call them here in dependency order:
    //   Material::s_* are file-scope initializers (run at CRT startup) -> no call needed.
    //   MathHelper / World: no deps.  Block: needs Material.  Item: needs Block::s_crops.
    MathHelper::initStatics();
    World::initStatics();
    Block::initStatics();
    Item::initStatics();
    OutputDebugStringA("[BOOT] static initializers ok; creating Minecraft\n");

    // Create the game and run it. Minecraft::run() performs the remaining init (game
    // options, render engine, font, world/GUI) and drives the tick + render loop until
    // the player quits (BACK). The D3D9 device (g_pd3dDevice) created above is used by it.
    // (STFS save mount via Save360_Init() is triggered from the save/load menu, not at boot,
    //  so we don't block on the sign-in / device-selector guide UI here.)
    // Use the PARAMETERIZED ctor (canvas, applet, w, h, fullscreen) — its body creates
    // m_sndManager (= new SoundManager()) and m_textureWaterFX/LavaFX. The default ctor
    // has an EMPTY body and leaves those NULL, so run()'s first call
    // (m_sndManager->loadSoundSettings, a VIRTUAL method) dispatched through a null vtable
    // -> IP=0x0 access violation.
    g_minecraft = new Minecraft(NULL, NULL, SCREEN_WIDTH, SCREEN_HEIGHT, true);
    OutputDebugStringA("[BOOT] Minecraft ctor ok; calling run()\n");
    g_minecraft->run();
    OutputDebugStringA("[BOOT] run() returned\n");

    // Cleanup
    Shaders::shutdown();
    Textures::shutdown();
    if (g_pd3dDevice)     g_pd3dDevice->Release();
    if (g_pD3D)           g_pD3D->Release();
}
