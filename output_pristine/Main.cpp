// Main.cpp — Xbox 360 entry point for Minecraft Indev
// Creates D3D9 device, initializes subsystems, runs game loop

#include "stdafx.h"
#include "Common.h"
#include "Shaders.h"
#include "Tesselator.h"
#include "Textures.h"

// Globals
IDirect3D9*       g_pD3D       = NULL;
IDirect3DDevice9* g_pd3dDevice = NULL;
IDirect3DTexture9* g_terrainTexture = NULL;
IDirect3DTexture9* g_guiTexture     = NULL;
IDirect3DTexture9* g_charTexture    = NULL;
IDirect3DTexture9* g_defaultTexture = NULL;

// Forward declarations
class Minecraft;
extern Minecraft* g_minecraft;

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

void __cdecl main() {
    if (!InitD3D()) {
        OutputDebugStringA("D3D init failed\n");
        return;
    }

    // Render state defaults
    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
    g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    g_pd3dDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
    g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
    g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    g_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

    if (!Shaders::init()) {
        OutputDebugStringA("Shader compile failed\n");
        return;
    }

    // Load all textures from game:\Content\ folder
    Textures::init();
    Textures::loadAll();

    // Convenience globals for the most-used textures
    g_terrainTexture = Textures::getById(Textures::load("game:\\Content\\terrain.png"));
    g_guiTexture     = Textures::getById(Textures::load("game:\\Content\\gui\\gui.png"));
    g_charTexture    = Textures::getById(Textures::load("game:\\Content\\char.png"));
    g_defaultTexture = Textures::getById(Textures::load("game:\\Content\\default.png"));

    OutputDebugStringA("Minecraft Indev — Xbox 360 port initialized\n");

    // PORT: Main game loop placeholder
    bool running = true;
    while (running) {
        // PORT: XInput polling
        XINPUT_STATE state;
        if (XInputGetState(0, &state) == ERROR_SUCCESS) {
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) {
                running = false;
            }
        }

        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
            D3DCOLOR_XRGB(128, 204, 255), 1.0f, 0);
        g_pd3dDevice->BeginScene();

        // PORT: game->render() here

        g_pd3dDevice->EndScene();
        g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
    }

    // Cleanup
    Shaders::shutdown();
    Textures::shutdown();
    if (g_pd3dDevice)     g_pd3dDevice->Release();
    if (g_pD3D)           g_pD3D->Release();
}
