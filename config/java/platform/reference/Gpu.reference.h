// Gpu.h -- what the OpenGL 1.x layer (GL11.cpp) asks of a graphics API. GL11.cpp is the whole GL state machine and knows no
// graphics API; one backend file carries this out on one API: GpuD3D9.cpp (Direct3D 9: the Xbox 360 and the PC) or GpuD3D11.cpp
// (Direct3D 11, the PC). A port to another API is one more file of these functions; the GL side is not touched.
//
// The layer hands over finished work: vertices already in one format, render state already decided (blend, depth, cull, bias,
// scissor, viewport in the API's top-left pixels), the two shader stages' constants already computed, textures already in ARGB.
// A backend turns that into its own calls and caches what it can; it decides nothing GL decides.
//
// C++03: the Xbox 360's compiler builds this file.
#ifndef GPU_H
#define GPU_H

#include <vector>

namespace j2xgl {
namespace gpu {

// One vertex: everything GL's fixed-function vertex stage reads, plus the flat-shading copies (see GL11.cpp). 60 bytes.
struct Vertex {
	float x, y, z;
	float nx, ny, nz;
	unsigned int color;   // ARGB (0xAARRGGBB): Direct3D 9's D3DCOLOR, Direct3D 11's B8G8R8A8_UNORM
	float u, v;           // texture unit 0
	float u2, v2;         // texture unit 1
	float fnx, fny, fnz;  // the primitive's provoking normal and colour (flat shading)
	unsigned int fcolor;
};

enum Prim { kTriangles, kLines };   // lists: three vertices a triangle, two a line

// The fixed-function state a draw runs under, in the API's terms (the layer has made GL's choices already).
enum Blend { kZero, kOne, kSrcColor, kInvSrcColor, kSrcAlpha, kInvSrcAlpha, kDstAlpha, kInvDstAlpha, kDstColor, kInvDstColor, kSrcAlphaSat };
enum Compare { kNever, kLess, kEqual, kLessEqual, kGreater, kNotEqual, kGreaterEqual, kAlways };
enum Cull { kCullNone, kCullCW, kCullCCW };   // the winding, on screen, that is NOT drawn
struct RenderState {
	bool blend; int srcBlend, dstBlend;              // Blend
	bool depthTest; int depthFunc; bool depthWrite;  // Compare
	unsigned colorWrite;                             // bits: 1 red, 2 green, 4 blue, 8 alpha
	int cull;                                        // Cull
	float slopeBias, depthBias;                      // depthBias in depth-buffer units already (GL units / 2^24)
	bool scissor; int scX, scY, scW, scH;            // top-left origin, pixels
	int vpX, vpY, vpW, vpH;                          // top-left origin, pixels
};

// How a texture unit samples.
enum Filter { kPoint, kLinear };
struct Sampler {
	bool wrapU, wrapV;   // repeat, else clamp to the edge
	int mag, min;        // Filter
	int mip;             // -1 none, else Filter between levels
};

// The constants both shader stages read, laid out as the shaders declare them (GL11.cpp's shader comments name each register).
const int kVSConsts = 28, kPSConsts = 4;   // float4s
struct Constants {
	float vs[kVSConsts * 4];
	float ps[kPSConsts * 4];
};

typedef struct TextureObject* Texture;   // a backend's own texture
typedef struct BufferObject* Buffer;     // a backend's own vertex buffer

// What a draw binds.
struct Bindings {
	Texture tex[2];      // 0: unit off
	Sampler samp[2];
};

// ---------------------------------------------------------------- the device
bool ready();                 // a device to draw with, shaders built (GL calls before it are kept as state only)
const char* name();           // "Direct3D 9", "Direct3D 11"
int  targetWidth();           // the frame being drawn
int  targetHeight();
void forgetState();           // the device's state is unknown (new or reset device): the next draw sets everything
bool halfPixelOffset();       // pixel centres on integers (the PC's Direct3D 9): GL's geometry is moved half a pixel to match

// ---------------------------------------------------------------- drawing
void clear(bool color, bool depth, bool stencil, float r, float g, float b, float a, float z, const RenderState& rs);
void draw(Prim prim, const Vertex* v, int count, const RenderState& rs, const Constants& c, const Bindings& b);
Buffer createBuffer(const Vertex* v, int count);    // 0 when it cannot be made
void releaseBuffer(Buffer buf);
void drawBuffer(Prim prim, Buffer buf, int count, const RenderState& rs, const Constants& c, const Bindings& b);
// A run of buffer creations (a frame's newly built display lists): Direct3D 9 steps out of its scene once for all of them.
void beginUploads();
void endUploads();

// ---------------------------------------------------------------- textures (ARGB, rows top to bottom)
Texture createTexture(int w, int h, int levels);    // 0 when it cannot be made
void releaseTexture(Texture t);
bool writeTexture(Texture t, int level, int x, int y, int w, int h, const unsigned int* argb);   // w * h pixels, tightly packed
bool readTexture(Texture t, int level, std::vector<unsigned int>& argb, int& w, int& h);

// ---------------------------------------------------------------- the frame
bool readFrame(std::vector<unsigned int>& argb, int& w, int& h);   // what has been drawn so far, rows top to bottom

}  // namespace gpu
}  // namespace j2xgl

#endif // GPU_H
