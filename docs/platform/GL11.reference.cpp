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
#include <map>

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

// ----------------------------------------------------------------------------- display-list recorder
// MC's renderer is display-list-DRIVEN: FontRenderer (256 glyph lists + color lists), WorldRenderer
// (terrain chunks), RenderGlobal (sky/stars) and ModelRenderer (entities) all glNewList(GL_COMPILE)
// once then glCallList every frame. GL_COMPILE means RECORD-don't-execute. So while a list is open we
// capture each draw/matrix/color op into the list instead of issuing it, and glCallList/glCallLists
// REPLAY them against the LIVE matrix+color+texture state (so a glyph list draws with the current
// text color and the camera matrix the chunk list is replayed under). This also fixes "draws during
// construction": chunk geometry built off the render path now records instead of touching the device.
struct GLListOp {
	int kind;   // 0 draw, 1 translate, 2 scale, 3 rotate, 4 loadIdentity, 5 push, 6 pop, 7 mult, 8 color, 9 bindTex
	std::vector<TesselatorVertex> tris;                  // kind 0: pre-triangulated, ready for DrawPrimitiveUP
	D3DPRIMITIVETYPE prim; int primCount; bool textured; bool stampColor;
	float a,b,c,d;                                        // kind 1/2 (xyz) / 3 (angle,x,y,z)
	D3DXMATRIX mat;                                       // kind 7
	DWORD color; int itex;                               // kind 8 / 9
	IDirect3DVertexBuffer9* vb;                          // PERSISTENT cached VB for baked-color geometry (chunk terrain); NULL until first replay
	GLListOp():kind(0),prim(D3DPT_TRIANGLELIST),primCount(0),textured(false),stampColor(false),
	           a(0),b(0),c(0),d(0),color(0xFFFFFFFF),itex(0),vb(NULL){}
};
typedef std::vector<GLListOp> GLDisplayList;
static std::map<int,GLDisplayList> s_lists;
static GLDisplayList* s_recList = NULL;     // non-NULL between glNewList..glEndList (recording)
static int            s_listNext = 1;       // next free list id for glGenLists

// Triangulate a gathered vertex run (GL_QUADS -> 2 tris w/ reversed winding for D3D CW cull).
static void triangulate(int mode, const std::vector<TesselatorVertex>& v,
                        std::vector<TesselatorVertex>& out, D3DPRIMITIVETYPE& prim, int& primCount) {
	if(mode==GL_QUADS){
		int quads=(int)v.size()/4; out.resize((size_t)quads*6); int o=0;
		for(int q=0;q<quads;++q){ int b=q*4;
			out[o++]=v[b+0]; out[o++]=v[b+2]; out[o++]=v[b+1];
			out[o++]=v[b+0]; out[o++]=v[b+3]; out[o++]=v[b+2]; }
		prim=D3DPT_TRIANGLELIST; primCount=o/3;
	} else if(mode==GL_TRIANGLES){
		out=v; prim=D3DPT_TRIANGLELIST; primCount=(int)v.size()/3;
	} else {                              // LINES (HUD selection box etc.)
		out=v; prim=D3DPT_LINELIST; primCount=(int)v.size()/2;
	}
}

// Issue one triangulated run with the LIVE matrices/shader (used by both immediate glDrawArrays and
// list replay). stampColor: the source had no color array -> paint with the current GL color now.
static void issueDraw(std::vector<TesselatorVertex>& tris, D3DPRIMITIVETYPE prim, int primCount,
                      bool textured, bool stampColor) {
	if(primCount<=0 || tris.empty()) return;
	if(textured) Shaders::useTexturedShader(); else Shaders::useUntexturedShader();
	Shaders::setWorldMatrix(s_modelview.back());
	Shaders::setProjectionMatrix(s_projection.back());
	{ D3DXMATRIX I; D3DXMatrixIdentity(&I); Shaders::setViewMatrix(I); }   // modelview folds model+view
	Shaders::updateShaderConstants();
	// DIAGNOSTIC (logo/world 3D): log the first few PERSPECTIVE draws (ortho proj has _34==0; perspective
	// has _34==-1). Shows whether 3D geometry lands inside the clip frustum (need w>0, |x|,|y|<=w, 0<=z<=w)
	// or is clipped/off-screen -- this is the shared submit point for the menu logo AND world terrain.
	if (s_projection.back()._34 != 0.0f) {
		static int s_diag3d = 0;
		if (s_diag3d < 5) { ++s_diag3d;
			D3DXMATRIX wvp; D3DXMatrixMultiply(&wvp, &s_modelview.back(), &s_projection.back());
			D3DXVECTOR3 v0(tris[0].x, tris[0].y, tris[0].z); D3DXVECTOR4 c;
			D3DXVec3Transform(&c, &v0, &wvp);
			DbgPrintF("[3d] draw#%d prim=%d cnt=%d tex=%d v0=(%.2f,%.2f,%.2f) clip=(%.2f,%.2f,%.2f w=%.2f)\n",
			          s_diag3d, (int)prim, primCount, textured?1:0, v0.x, v0.y, v0.z, c.x, c.y, c.z, c.w);
		}
	}
	if(stampColor) for(size_t i=0;i<tris.size();++i) tris[i].color=s_color;
	g_pd3dDevice->DrawPrimitiveUP(prim, primCount, &tris[0], sizeof(TesselatorVertex));
}

// ----------------------------------------------------------------------------- lifecycle
void glInit() {
	D3DXMATRIX I; D3DXMatrixIdentity(&I);
	s_modelview.clear();  s_modelview.push_back(I);
	s_projection.clear(); s_projection.push_back(I);
	// Sync the device to GL's INITIAL state. D3D9 defaults CULLMODE to CCW, but GL starts with cull
	// DISABLED (s_cullFace=false) and the 2D/GUI path never touches GL_CULL_FACE -> menu quads would be
	// back-face-culled by the stale CCW default and never appear. Push the GL defaults up front.
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE,         D3DCULL_NONE);   // GL: cull off until glEnable(GL_CULL_FACE)
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE,          TRUE);          // GL: depth test on (s_depthTest=true)
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);         // GL: blend off (s_blend=false)
	// NOTE: no D3DRS_LIGHTING — the Xbox 360 has NO fixed-function pipeline (shader-only), so the FFP
	// lighting/material/texture-stage render states don't exist. We light via Shaders:: (light baked
	// into vertex color), so there is nothing to disable here.
}

// ----------------------------------------------------------------------------- matrices
// Each GL op LEFT-multiplies its D3DX-native delta into the current matrix:  top = delta * top.
// (Derivation: GL CTM = T*R*... in column convention; the row-major D3DX equivalent is its
// transpose R^T*T^T, and D3DXMatrixTranslation/Rotation already produce those row-major forms, so
// composing newest-on-the-left reproduces GL semantics. Shaders::updateShaderConstants transposes
// the final WVP for the shader.)  HARDWARE-VERIFY this against the on-screen result.
static void preMul(const D3DXMATRIX& delta) { D3DXMatrixMultiply(&cur(), &delta, &cur()); }

void glMatrixMode(int mode) { s_matrixMode = mode; }
void glPushMatrix() {
	if(s_recList){ GLListOp op; op.kind=5; s_recList->push_back(op); return; }
	std::vector<D3DXMATRIX>& s = (s_matrixMode==GL_PROJECTION?s_projection:s_modelview); s.push_back(s.back());
}
void glPopMatrix()  {
	if(s_recList){ GLListOp op; op.kind=6; s_recList->push_back(op); return; }
	std::vector<D3DXMATRIX>& s = (s_matrixMode==GL_PROJECTION?s_projection:s_modelview); if (s.size()>1) s.pop_back();
}
void glLoadIdentity() {
	if(s_recList){ GLListOp op; op.kind=4; s_recList->push_back(op); return; }
	D3DXMatrixIdentity(&cur());
}
void glTranslatef(float x, float y, float z) {
	if(s_recList){ GLListOp op; op.kind=1; op.a=x;op.b=y;op.c=z; s_recList->push_back(op); return; }
	D3DXMATRIX m; D3DXMatrixTranslation(&m,x,y,z); preMul(m);
}
void glScalef(float x, float y, float z)     {
	if(s_recList){ GLListOp op; op.kind=2; op.a=x;op.b=y;op.c=z; s_recList->push_back(op); return; }
	D3DXMATRIX m; D3DXMatrixScaling(&m,x,y,z);     preMul(m);
}
void glRotatef(float angle, float x, float y, float z) {
	if(s_recList){ GLListOp op; op.kind=3; op.a=angle;op.b=x;op.c=y;op.d=z; s_recList->push_back(op); return; }
	D3DXMATRIX m; D3DXVECTOR3 axis(x,y,z);
	D3DXMatrixRotationAxis(&m, &axis, D3DXToRadian(angle)); preMul(m);
}
void glMultMatrix(FloatBuffer* mbuf) {
	// LWJGL passes 16 column-major floats; loaded row-major into D3DXMATRIX it is the transpose,
	// which is exactly the D3DX-native delta -> same preMul rule.
	D3DXMATRIX m; memcpy(&m, ((float*)((ByteBuffer*)mbuf)->base()) + 0, 16*sizeof(float));
	if(s_recList){ GLListOp op; op.kind=7; op.mat=m; s_recList->push_back(op); return; }
	preMul(m);
}
void glOrtho(double l, double r, double b, double t, double n, double f) {
	// RIGHT-handed: GL is RH (eye looks down -Z) and the GUI/loadScreen draw at eye-z = -2000 after a
	// glTranslatef(0,0,-2000) under ortho near/far = 1000/3000. An LH ortho maps z=-2000 -> NDC -1.5
	// (outside [0,1]) -> the whole menu is depth-CLIPPED -> black. RH maps -2000 -> 0.5 -> visible.
	D3DXMATRIX m; D3DXMatrixOrthoOffCenterRH(&m,(float)l,(float)r,(float)b,(float)t,(float)n,(float)f);
	s_projection.back() = m;     // GUI uses glOrtho directly; HARDWARE-VERIFY Y orientation (GL y-up)
}
void glViewport(int x, int y, int w, int h) {
	// GL viewport origin is BOTTOM-left, D3D is TOP-left -> flip Y. Without this a GL top-strip
	// viewport (y = H-h, used by GuiMainMenu::drawLogo for the 3D block logo) landed in the BOTTOM
	// strip, where the menu then overdrew it -> the logo was invisible. Full-screen (0,0,W,H) is
	// unaffected (Y stays 0).
	D3DVIEWPORT9 vp;
	vp.X = x; vp.Y = SCREEN_HEIGHT - y - h; vp.Width = w; vp.Height = h; vp.MinZ = 0.0f; vp.MaxZ = 1.0f;
	g_pd3dDevice->SetViewport(&vp);
}
void glLoadProjection(const D3DXMATRIX& m){ s_projection.back() = m; }   // GLU::gluPerspective uses this

// Read the current MODELVIEW or PROJECTION matrix into a FloatBuffer (16 floats), as GL's glGetFloat does.
// Used by ClippingHelperImplementation to build the view frustum for chunk culling. CONVENTION: GL11 stores
// D3DX row-major matrices = the GL column-vector CTM TRANSPOSED; that 16-float memory layout is EXACTLY GL's
// column-major glGetFloat array (Dmem[i*4+j] = CTM[j][i] = GLarr[i*4+j]), so copy the floats straight through.
// (Side+far frustum planes come out correct; the near plane is harmlessly lenient under D3D's [0,1] z.)
void glGetFloat(int pname, FloatBuffer* buf) {
	if(!buf) return;
	const D3DXMATRIX* m = (pname==GL_MODELVIEW_MATRIX)  ? &s_modelview.back()
	                    : (pname==GL_PROJECTION_MATRIX) ? &s_projection.back() : (const D3DXMATRIX*)0;
	if(!m) return;
	const float* src = (const float*)m;
	for(int i=0;i<16;++i) buf->put(src[i]);
}

// ----------------------------------------------------------------------------- color / normal
void glColor4f(float r,float g,float b,float a){
	int ri=(int)(r*255), gi=(int)(g*255), bi=(int)(b*255), ai=(int)(a*255);
	DWORD c = D3DCOLOR_ARGB(ai<0?0:ai>255?255:ai, ri<0?0:ri>255?255:ri, gi<0?0:gi>255?255:gi, bi<0?0:bi>255?255:bi);
	if(s_recList){ GLListOp op; op.kind=8; op.color=c; s_recList->push_back(op); return; }   // font color-code lists
	s_color = c;
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
void glGenTextures(IntBuffer* buf){ if(!buf) return; int n=buf->remaining(); for(int i=0;i<n;++i) buf->put(Textures::genId()); }
void glDeleteTextures(IntBuffer* buf){ if(!buf) return; int n=buf->remaining(); for(int i=0;i<n;++i) Textures::deleteId(buf->get()); }
void glBindTexture(int,int id){
	s_boundTex=id;
	g_pd3dDevice->SetTexture(0, id ? Textures::getById(id) : NULL);
}
void glTexParameteri(int,int,int){}                      // filtering set globally (point) in Main
void glTexImage2D(int,int,int,int w,int h,int,int,int,ByteBuffer* px){
	Textures::uploadRGBA(s_boundTex, w, h, px ? (const void*)px->base() : NULL);
}
void glTexSubImage2D(int,int,int x,int y,int w,int h,int,int,ByteBuffer* px){
	Textures::uploadSubRGBA(s_boundTex, x, y, w, h, px ? (const void*)px->base() : NULL);
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
	bool hadColor = s_col.on;                         // no color array -> color is resolved at draw/replay
	std::vector<TesselatorVertex> v((size_t)count);
	for(int i=0;i<count;++i){
		const char* vp = s_vtx.base + (long long)(first+i)*s_vtx.stride;
		const float* fp=(const float*)vp;
		v[i].x=fp[0]; v[i].y=fp[1]; v[i].z=(s_vtx.size>=3?fp[2]:0.0f);
		if(s_tex.on){ const float* tp=(const float*)(s_tex.base+(long long)(first+i)*s_tex.stride); v[i].u=tp[0]; v[i].v=tp[1]; }
		else { v[i].u=0; v[i].v=0; }
		if(hadColor){ DWORD rgba=*(const DWORD*)(s_col.base+(long long)(first+i)*s_col.stride); v[i].color=swizzleRGBAtoARGB(rgba); }
		else v[i].color=s_color;                       // (overwritten by live color at replay if recorded)
	}
	std::vector<TesselatorVertex> tris; D3DPRIMITIVETYPE prim; int primCount;
	triangulate(mode, v, tris, prim, primCount);

	if(s_recList){                                      // RECORDING (glNewList): stash, don't draw
		s_recList->push_back(GLListOp());               // push empty, then swap geometry in (no big copy)
		GLListOp& op = s_recList->back();
		op.kind=0; op.tris.swap(tris); op.prim=prim; op.primCount=primCount;
		op.textured = s_tex.on; op.stampColor = !hadColor;
		return;
	}
	issueDraw(tris, prim, primCount, s_tex.on && s_texture2D, false);   // immediate
}

// ----------------------------------------------------------------------------- display lists
// Free any cached persistent VBs in a list (call before clearing/erasing it).
static void _freeListVBs(GLDisplayList& dl){ for(size_t i=0;i<dl.size();++i){ if(dl[i].vb){ dl[i].vb->Release(); dl[i].vb=NULL; } } }

// Draw one recorded geometry op. BAKED-COLOR geometry (chunk terrain: stampColor==false) is uploaded
// ONCE into a persistent D3D vertex buffer and DrawPrimitive'd from VRAM on every later replay -- this
// removes the per-frame DrawPrimitiveUP RAM re-upload of every visible chunk, which was THE in-world fps
// bottleneck. stampColor geometry (sky/clouds/HUD, whose color is applied live each replay) stays
// immediate. The VB is freed + rebuilt when the list is re-recorded (glNewList) e.g. on a block edit.
static void issueDrawListOp(GLListOp& op){
	if(op.primCount<=0 || op.tris.empty()) return;
	if(op.stampColor){ issueDraw(op.tris, op.prim, op.primCount, op.textured, true); return; }
	if(op.textured) Shaders::useTexturedShader(); else Shaders::useUntexturedShader();
	Shaders::setWorldMatrix(s_modelview.back());
	Shaders::setProjectionMatrix(s_projection.back());
	{ D3DXMATRIX I; D3DXMatrixIdentity(&I); Shaders::setViewMatrix(I); }
	Shaders::updateShaderConstants();
	if(op.vb==NULL){
		UINT bytes=(UINT)(op.tris.size()*sizeof(TesselatorVertex));
		if(g_pd3dDevice->CreateVertexBuffer(bytes, D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &op.vb, NULL)==D3D_OK && op.vb){
			void* p=NULL;
			if(op.vb->Lock(0, bytes, &p, 0)==D3D_OK){ memcpy(p, &op.tris[0], bytes); op.vb->Unlock(); }
			else { op.vb->Release(); op.vb=NULL; }
		}
	}
	if(op.vb){
		g_pd3dDevice->SetStreamSource(0, op.vb, 0, sizeof(TesselatorVertex));
		g_pd3dDevice->DrawPrimitive(op.prim, 0, op.primCount);
	} else {
		issueDraw(op.tris, op.prim, op.primCount, op.textured, false);   // creation failed -> immediate fallback
	}
}

// Real record/replay (see the GLListOp recorder up top). glNewList opens a list for recording; every
// draw/matrix/color op until glEndList is captured; glCallList(s) replays against the LIVE state.
static void replayList(GLDisplayList& dl){
	for(size_t i=0;i<dl.size();++i){
		GLListOp& op=dl[i];
		switch(op.kind){
			case 0: issueDrawListOp(op); break;
			case 1: { D3DXMATRIX m; D3DXMatrixTranslation(&m,op.a,op.b,op.c); preMul(m); } break;
			case 2: { D3DXMATRIX m; D3DXMatrixScaling(&m,op.a,op.b,op.c); preMul(m); } break;
			case 3: { D3DXMATRIX m; D3DXVECTOR3 ax(op.b,op.c,op.d); D3DXMatrixRotationAxis(&m,&ax,D3DXToRadian(op.a)); preMul(m); } break;
			case 4: D3DXMatrixIdentity(&cur()); break;
			case 5: { std::vector<D3DXMATRIX>& s=(s_matrixMode==GL_PROJECTION?s_projection:s_modelview); s.push_back(s.back()); } break;
			case 6: { std::vector<D3DXMATRIX>& s=(s_matrixMode==GL_PROJECTION?s_projection:s_modelview); if(s.size()>1) s.pop_back(); } break;
			case 7: preMul(op.mat); break;
			case 8: s_color=op.color; break;
			case 9: glBindTexture(0, op.itex); break;
		}
	}
}
int  glGenLists(int n){ int base=s_listNext; if(n<1)n=1; s_listNext+=n; return base; }   // reserve n consecutive ids
void glNewList(int list,int){ GLDisplayList& dl=s_lists[list]; _freeListVBs(dl); dl.clear(); s_recList=&dl; }
void glEndList(){ s_recList=NULL; }
void glCallList(int list){
	std::map<int,GLDisplayList>::iterator it=s_lists.find(list);
	if(it!=s_lists.end()) replayList(it->second);
}
void glCallLists(IntBuffer* buf){                     // replay a run of absolute list ids (font / RenderList)
	if(!buf) return;
	int n=buf->remaining();
	const int* ids=(const int*)((ByteBuffer*)buf)->base();
	for(int i=0;i<n;++i) glCallList(ids[i]);
}
void glDeleteLists(int list,int n){ for(int i=0;i<n;++i){ std::map<int,GLDisplayList>::iterator it=s_lists.find(list+i); if(it!=s_lists.end()){ _freeListVBs(it->second); s_lists.erase(it); } } }

// ----------------------------------------------------------------------------- queries / misc
int  glGetError(){ return 0; }
JavaString glGetString(int){ return JavaString("Xbox360 D3D9"); }
void glPolygonOffset(float,float){}
void glLineWidth(float){}
void glColorMaterial(int,int){}
void glLightModel(int,FloatBuffer*){}

} // namespace GL11
