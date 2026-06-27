#include "stdafx.h"
// GL11.cpp — OpenGL(LWJGL) -> Direct3D 9 emulator for the Xbox 360 j2x port.  [Step 2]
// REFERENCE IMPLEMENTATION (Xbox-side; not built by j2x). See GL11.reference.h for the design.
// Grounded in the old port's Shaders.cpp + Tesselator.cpp (the proven GL->D3D9 mechanism).
//
// PREREQUISITE: the NIO buffers (ByteBuffer/FloatBuffer/IntBuffer) must be REAL (byte-backed) here,
// not the converter's no-op stubs -- the Tessellator packs vertex data through them, and the
// vertex-array gather below reads that data. This file uses `char* ByteBuffer::base()`, which (per
// NIOBuffers.reference.h, Step 5) returns the backing start ALREADY OFFSET by the buffer's current
// byte position -- so a FloatBuffer parked at element 3 yields start+12, and the gather reads from
// base() directly (no extra position() arithmetic). All three buffer types upcast to ByteBuffer*.

#include "GL11.h"
#include "Common.h"          // g_pd3dDevice, SCREEN_*
#include "Shaders.h"         // TesselatorVertex, Shaders::*
#include "Textures.h"        // int-handle <-> IDirect3DTexture9*
#include <vector>

namespace GL11 {

// ----------------------------------------------------------------------------- state
static int       s_matrixMode = GL_MODELVIEW;
static std::vector<D3DXMATRIX> s_modelview, s_projection;   // stacks; back() is current
static DWORD     s_color      = 0xFFFFFFFF;                 // current color (D3DCOLOR ARGB)
static bool      s_texture2D  = false;                      // GL_TEXTURE_2D enable
static bool      s_blend      = false, s_depthTest = true, s_cullFace = false;
static int       s_boundTex   = 0;

// vertex-array pointers (non-VBO: a backing byte pointer + byte stride + offset within it)
struct ArrayPtr { bool on; const char* base; int size; int type; int stride; };
static ArrayPtr  s_vtx = {false,0,3,GL_FLOAT,0};
static ArrayPtr  s_col = {false,0,4,GL_UNSIGNED_BYTE,0};
static ArrayPtr  s_tex = {false,0,2,GL_FLOAT,0};

static D3DXMATRIX& cur() { return (s_matrixMode == GL_PROJECTION ? s_projection : s_modelview).back(); }

static DWORD swizzleRGBAtoARGB(DWORD rgba) {            // GL byte order R,G,B,A -> D3DCOLOR A,R,G,B
	DWORD r=(rgba)&0xFF, g=(rgba>>8)&0xFF, b=(rgba>>16)&0xFF, a=(rgba>>24)&0xFF;
	return (a<<24)|(r<<16)|(g<<8)|b;
}

// ----------------------------------------------------------------------------- lifecycle
void glInit() {
	D3DXMATRIX I; D3DXMatrixIdentity(&I);
	s_modelview.clear();  s_modelview.push_back(I);
	s_projection.clear(); s_projection.push_back(I);
}

// ----------------------------------------------------------------------------- matrices
// Each GL op LEFT-multiplies its D3DX-native delta into the current matrix:  top = delta * top.
// (Derivation: GL CTM = T*R*... in column convention; the row-major D3DX equivalent is its
// transpose R^T*T^T, and D3DXMatrixTranslation/Rotation already produce those row-major forms, so
// composing newest-on-the-left reproduces GL semantics. Shaders::updateShaderConstants transposes
// the final WVP for the shader.)  HARDWARE-VERIFY this against the on-screen result.
static void preMul(const D3DXMATRIX& delta) { D3DXMatrixMultiply(&cur(), &delta, &cur()); }

void glMatrixMode(int mode) { s_matrixMode = mode; }
void glPushMatrix() { std::vector<D3DXMATRIX>& s = (s_matrixMode==GL_PROJECTION?s_projection:s_modelview); s.push_back(s.back()); }
void glPopMatrix()  { std::vector<D3DXMATRIX>& s = (s_matrixMode==GL_PROJECTION?s_projection:s_modelview); if (s.size()>1) s.pop_back(); }
void glLoadIdentity() { D3DXMatrixIdentity(&cur()); }
void glTranslatef(float x, float y, float z) { D3DXMATRIX m; D3DXMatrixTranslation(&m,x,y,z); preMul(m); }
void glScalef(float x, float y, float z)     { D3DXMATRIX m; D3DXMatrixScaling(&m,x,y,z);     preMul(m); }
void glRotatef(float angle, float x, float y, float z) {
	D3DXMATRIX m; D3DXVECTOR3 axis(x,y,z);
	D3DXMatrixRotationAxis(&m, &axis, D3DXToRadian(angle)); preMul(m);
}
void glMultMatrix(FloatBuffer* mbuf) {
	// LWJGL passes 16 column-major floats; loaded row-major into D3DXMATRIX it is the transpose,
	// which is exactly the D3DX-native delta -> same preMul rule.
	D3DXMATRIX m; memcpy(&m, ((float*)((ByteBuffer*)mbuf)->base()) + 0, 16*sizeof(float)); preMul(m);
}
void glOrtho(double l, double r, double b, double t, double n, double f) {
	D3DXMATRIX m; D3DXMatrixOrthoOffCenterLH(&m,(float)l,(float)r,(float)b,(float)t,(float)n,(float)f);
	s_projection.back() = m;     // GUI uses glOrtho directly; HARDWARE-VERIFY Y orientation (GL y-up)
}
void glViewport(int x, int y, int w, int h) {
	D3DVIEWPORT9 vp; vp.X=x; vp.Y=y; vp.Width=w; vp.Height=h; vp.MinZ=0.0f; vp.MaxZ=1.0f;
	g_pd3dDevice->SetViewport(&vp);
}
void glLoadProjection(const D3DXMATRIX& m){ s_projection.back() = m; }   // GLU::gluPerspective uses this

// ----------------------------------------------------------------------------- color / normal
void glColor4f(float r,float g,float b,float a){
	int ri=(int)(r*255), gi=(int)(g*255), bi=(int)(b*255), ai=(int)(a*255);
	s_color = D3DCOLOR_ARGB(ai<0?0:ai>255?255:ai, ri<0?0:ri>255?255:ri, gi<0?0:gi>255?255:gi, bi<0?0:bi>255?255:bi);
}
void glColor3f(float r,float g,float b){ glColor4f(r,g,b,1.0f); }
void glNormal3f(float,float,float){}                    // light baked into vertex color

// ----------------------------------------------------------------------------- render state
static void applyDepth(){ g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, s_depthTest?TRUE:FALSE); }
static void applyBlend(){ g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, s_blend?TRUE:FALSE); }
static void applyCull(){ g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, s_cullFace?D3DCULL_CW:D3DCULL_NONE); }

void glEnable(int cap){
	if(cap==GL_DEPTH_TEST){s_depthTest=true;applyDepth();}
	else if(cap==GL_BLEND){s_blend=true;applyBlend();}
	else if(cap==GL_CULL_FACE){s_cullFace=true;applyCull();}
	else if(cap==GL_TEXTURE_2D){s_texture2D=true;}
	else if(cap==GL_ALPHA_TEST){Shaders::setAlphaRef(0.5f);}   // cutout on (leaves/glass/items)
	else if(cap==GL_FOG){Shaders::setFogEnabled(true);}
}
void glDisable(int cap){
	if(cap==GL_DEPTH_TEST){s_depthTest=false;applyDepth();}
	else if(cap==GL_BLEND){s_blend=false;applyBlend();}
	else if(cap==GL_CULL_FACE){s_cullFace=false;applyCull();}
	else if(cap==GL_TEXTURE_2D){s_texture2D=false;}
	else if(cap==GL_ALPHA_TEST){Shaders::setAlphaRef(0.0f);}   // keep all texels
	else if(cap==GL_FOG){Shaders::setFogEnabled(false);}
}
static D3DBLEND glBlendToD3D(int f){
	switch(f){case GL_ONE:return D3DBLEND_ONE;case GL_ZERO:return D3DBLEND_ZERO;
		case GL_SRC_ALPHA:return D3DBLEND_SRCALPHA;case GL_ONE_MINUS_SRC_ALPHA:return D3DBLEND_INVSRCALPHA;
		default:return D3DBLEND_ONE;}
}
void glBlendFunc(int s,int d){
	g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, glBlendToD3D(s));
	g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, glBlendToD3D(d));
}
void glDepthFunc(int func){
	D3DCMPFUNC c = func==GL_LEQUAL?D3DCMP_LESSEQUAL:func==GL_LESS?D3DCMP_LESS:
	               func==GL_GEQUAL?D3DCMP_GREATEREQUAL:func==GL_GREATER?D3DCMP_GREATER:D3DCMP_LESSEQUAL;
	g_pd3dDevice->SetRenderState(D3DRS_ZFUNC, c);
}
void glDepthMask(bool flag){ g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, flag?TRUE:FALSE); }
void glColorMask(bool r,bool g,bool b,bool a){
	DWORD m=(r?D3DCOLORWRITEENABLE_RED:0)|(g?D3DCOLORWRITEENABLE_GREEN:0)|(b?D3DCOLORWRITEENABLE_BLUE:0)|(a?D3DCOLORWRITEENABLE_ALPHA:0);
	g_pd3dDevice->SetRenderState(D3DRS_COLORWRITEENABLE, m);
}
void glCullFace(int){}                                   // direction handled by winding; see flush note
void glAlphaFunc(int,float ref){ Shaders::setAlphaRef(ref); }
void glShadeModel(int){}
void glClearColor(float,float,float,float){}             // clear handled in glClear (platform owns the value)
void glClearDepth(double){}
void glClear(int mask){
	DWORD flags=0;
	if(mask&GL_COLOR_BUFFER_BIT) flags|=D3DCLEAR_TARGET;
	if(mask&GL_DEPTH_BUFFER_BIT) flags|=D3DCLEAR_ZBUFFER;
	if(flags) g_pd3dDevice->Clear(0,NULL,flags, D3DCOLOR_ARGB(255,0,0,0), 1.0f, 0);
}

// ----------------------------------------------------------------------------- fog
void glFogf(int pname,float param){ if(pname==GL_FOG_DENSITY) Shaders::setFogDensity(param); }
void glFogi(int,int){}
void glFog(int pname, FloatBuffer* params){
	if(pname==GL_FOG_COLOR){ float* p=(float*)((ByteBuffer*)params)->base(); Shaders::setFogColor(p[0],p[1],p[2]); }
}

// ----------------------------------------------------------------------------- textures
int  glGenTextures(){ return Textures::genId(); }        // reserve an empty registry slot
void glDeleteTextures(int id){ Textures::deleteId(id); }
void glBindTexture(int,int id){
	s_boundTex=id;
	g_pd3dDevice->SetTexture(0, id ? Textures::getById(id) : NULL);
}
void glTexParameteri(int,int,int){}                      // filtering set globally (point) in Main
void glTexImage2D(int,int,int,int w,int h,int,int,int,IntBuffer* px){
	Textures::uploadRGBA(s_boundTex, w, h, px ? (const void*)((ByteBuffer*)px)->base() : NULL);
}
void glTexSubImage2D(int,int,int x,int y,int w,int h,int,int,IntBuffer* px){
	Textures::uploadSubRGBA(s_boundTex, x, y, w, h, px ? (const void*)((ByteBuffer*)px)->base() : NULL);
}
void glPixelStorei(int,int){}

// ----------------------------------------------------------------------------- vertex arrays (HOT PATH)
void glEnableClientState(int cap){
	if(cap==GL_VERTEX_ARRAY)s_vtx.on=true; else if(cap==GL_COLOR_ARRAY)s_col.on=true;
	else if(cap==GL_TEXTURE_COORD_ARRAY)s_tex.on=true;
}
void glDisableClientState(int cap){
	if(cap==GL_VERTEX_ARRAY)s_vtx.on=false; else if(cap==GL_COLOR_ARRAY)s_col.on=false;
	else if(cap==GL_TEXTURE_COORD_ARRAY)s_tex.on=false;
}
// NIO-buffer overloads: data pointer = buffer's base(), which ALREADY folds in the current byte
// position (see NIOBuffers.reference.h -- base() == backing start + byte position). Do NOT add
// position() again here: for a FloatBuffer parked at element 3, base() is already start+12 bytes.
static const char* bufBase(ByteBuffer* b){ return b->base(); }
void glVertexPointer(int size,int stride,FloatBuffer* b){ s_vtx.on=true; s_vtx.size=size; s_vtx.type=GL_FLOAT; s_vtx.stride=stride; s_vtx.base=bufBase((ByteBuffer*)b); }
void glTexCoordPointer(int size,int stride,FloatBuffer* b){ s_tex.on=true; s_tex.size=size; s_tex.type=GL_FLOAT; s_tex.stride=stride; s_tex.base=bufBase((ByteBuffer*)b); }
void glColorPointer(int size,bool,int stride,ByteBuffer* b){ s_col.on=true; s_col.size=size; s_col.type=GL_UNSIGNED_BYTE; s_col.stride=stride; s_col.base=bufBase(b); }
// VBO/offset overloads: we don't use VBOs -> not expected on console; left as no-ops (buffer path used).
void glVertexPointer(int,int,int,long long){}
void glColorPointer(int,int,int,long long){}
void glTexCoordPointer(int,int,int,long long){}

// gather `count` verts from the bound arrays into TesselatorVertex, then draw exactly like
// Tesselator::flush (quads -> triangles, reversed winding for D3D CW culling).
void glDrawArrays(int mode,int first,int count){
	if(!s_vtx.on || count<=0) return;
	std::vector<TesselatorVertex> v((size_t)count);
	for(int i=0;i<count;++i){
		const char* vp = s_vtx.base + (long long)(first+i)*s_vtx.stride;
		const float* fp=(const float*)vp;
		v[i].x=fp[0]; v[i].y=fp[1]; v[i].z=(s_vtx.size>=3?fp[2]:0.0f);
		if(s_tex.on){ const float* tp=(const float*)(s_tex.base+(long long)(first+i)*s_tex.stride); v[i].u=tp[0]; v[i].v=tp[1]; }
		else { v[i].u=0; v[i].v=0; }
		if(s_col.on){ DWORD rgba=*(const DWORD*)(s_col.base+(long long)(first+i)*s_col.stride); v[i].color=swizzleRGBAtoARGB(rgba); }
		else v[i].color=s_color;
	}
	if(s_tex.on && s_texture2D) Shaders::useTexturedShader(); else Shaders::useUntexturedShader();
	Shaders::setWorldMatrix(s_modelview.back());
	Shaders::setProjectionMatrix(s_projection.back());
	{ D3DXMATRIX I; D3DXMatrixIdentity(&I); Shaders::setViewMatrix(I); }   // modelview folds model+view
	Shaders::updateShaderConstants();

	if(mode==GL_QUADS){
		int quads=count/4; std::vector<TesselatorVertex> tris((size_t)quads*6); int o=0;
		for(int q=0;q<quads;++q){ int b=q*4;
			tris[o++]=v[b+0]; tris[o++]=v[b+2]; tris[o++]=v[b+1];     // reversed winding (CW cull)
			tris[o++]=v[b+0]; tris[o++]=v[b+3]; tris[o++]=v[b+2]; }
		if(o>0) g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,o/3,&tris[0],sizeof(TesselatorVertex));
	} else if(mode==GL_TRIANGLES){
		g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,count/3,&v[0],sizeof(TesselatorVertex));
	} else { // LINES / strips: approximate as a line list (rare; HUD selection box etc.)
		g_pd3dDevice->DrawPrimitiveUP(D3DPT_LINELIST,count/2,&v[0],sizeof(TesselatorVertex));
	}
}

// ----------------------------------------------------------------------------- display lists
// MC records chunk geometry once (glNewList) and replays each frame (glCallList). Until a real
// record/replay is in place, run lists immediately: glNewList/glEndList are no-ops and the geometry
// re-tessellates every frame (correct, just slower). HARDWARE-VERIFY perf; upgrade to recorded
// D3D vertex buffers if chunk draw is too slow. (See docs/platform/02 open question.)
int  glGenLists(int){ static int s_next=1; return s_next++; }
void glNewList(int,int){}
void glEndList(){}
void glCallList(int){}
void glDeleteLists(int,int){}

// ----------------------------------------------------------------------------- queries / misc
int  glGetError(){ return 0; }
JavaString glGetString(int){ return JavaString("Xbox360 D3D9"); }
void glPolygonOffset(float,float){}
void glLineWidth(float){}
void glColorMaterial(int,int){}
void glLightModel(int,FloatBuffer*){}

} // namespace GL11
