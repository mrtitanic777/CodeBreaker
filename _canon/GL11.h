#ifndef GL11_H
#define GL11_H
// LWJGL org.lwjgl.opengl.GL11 stub -- symbols only, so render code COMPILES; the real
// pipeline is the platform layer (Xbox 360 = Direct3D 9). Values are real GL 1.1 enums.

namespace GL11 {
	const int GL_POINTS = 0x0000;
	const int GL_FRONT = 0x0404;
	const int GL_BACK = 0x0405;
	const int GL_FRONT_AND_BACK = 0x0408;
	const int GL_LIGHTING = 0x0B50;
	const int GL_LIGHT0 = 0x4000;
	const int GL_LIGHT1 = 0x4001;
	const int GL_FOG = 0x0B60;
	const int GL_FOG_MODE = 0x0B65;
	const int GL_FOG_DENSITY = 0x0B62;
	const int GL_FOG_START = 0x0B63;
	const int GL_FOG_END = 0x0B64;
	const int GL_FOG_COLOR = 0x0B66;
	const int GL_DEPTH_TEST = 0x0B71;
	const int GL_CULL_FACE = 0x0B44;
	const int GL_ALPHA_TEST = 0x0BC0;
	const int GL_BLEND = 0x0BE2;
	const int GL_TEXTURE_2D = 0x0DE1;
	const int GL_NORMALIZE = 0x0BA1;
	const int GL_COLOR_MATERIAL = 0x0B57;
	const int GL_MODELVIEW = 0x1700;
	const int GL_PROJECTION = 0x1701;
	const int GL_SMOOTH = 0x1D01;
	const int GL_FLAT = 0x1D00;
	const int GL_EXP = 0x0800;
	const int GL_LINEAR = 0x2601;
	const int GL_NEAREST = 0x2600;
	const int GL_GREATER = 0x0204;
	const int GL_EQUAL = 0x0202;
	const int GL_LEQUAL = 0x0203;
	const int GL_SRC_ALPHA = 0x0302;
	const int GL_ONE_MINUS_SRC_ALPHA = 0x0303;
	const int GL_ONE = 0x0001;
	const int GL_DST_COLOR = 0x0306;
	const int GL_SRC_COLOR = 0x0300;
	const int GL_ONE_MINUS_SRC_COLOR = 0x0301;
	const int GL_ONE_MINUS_DST_COLOR = 0x0307;
	const int GL_DST_ALPHA = 0x0304;
	const int GL_RGB = 0x1907;
	const int GL_RGBA = 0x1908;
	const int GL_UNSIGNED_BYTE = 0x1401;
	const int GL_FLOAT = 0x1406;
	const int GL_TEXTURE_MAG_FILTER = 0x2800;
	const int GL_TEXTURE_MIN_FILTER = 0x2801;
	const int GL_TEXTURE_WRAP_S = 0x2802;
	const int GL_TEXTURE_WRAP_T = 0x2803;
	const int GL_REPEAT = 0x2901;
	const int GL_CLAMP = 0x2900;
	const int GL_COLOR_BUFFER_BIT = 0x4000;
	const int GL_DEPTH_BUFFER_BIT = 0x0100;
	const int GL_COMPILE = 0x1300;
	const int GL_VERTEX_ARRAY = 0x8074;
	const int GL_COLOR_ARRAY = 0x8076;
	const int GL_TEXTURE_COORD_ARRAY = 0x8078;
	const int GL_AMBIENT = 0x1200;
	const int GL_DIFFUSE = 0x1201;
	const int GL_SPECULAR = 0x1202;
	const int GL_POSITION = 0x1203;
	const int GL_AMBIENT_AND_DIFFUSE = 0x1602;
	const int GL_LIGHT_MODEL_AMBIENT = 0x0B53;
	const int GL_PACK_ALIGNMENT = 0x0D05;
	const int GL_VENDOR = 0x1F00;
	const int GL_RENDERER = 0x1F01;
	const int GL_VERSION = 0x1F02;
	const int GL_POLYGON_OFFSET_FILL = 0x8037;
	const int GL_PROJECTION_MATRIX = 0x0BA7;
	const int GL_MODELVIEW_MATRIX = 0x0BA6;

	inline void glAlphaFunc(...) {}
	inline void glBindTexture(...) {}
	inline void glBlendFunc(...) {}
	inline void glCallList(...) {}
	inline void glCallLists(...) {}
	inline void glClear(...) {}
	inline void glClearColor(...) {}
	inline void glClearDepth(...) {}
	inline void glColor3f(...) {}
	inline void glColor4f(...) {}
	inline void glColorMask(...) {}
	inline void glColorMaterial(...) {}
	inline void glColorPointer(...) {}
	inline void glCullFace(...) {}
	inline void glDeleteTextures(...) {}
	inline void glDepthFunc(...) {}
	inline void glDepthMask(...) {}
	inline void glDisable(...) {}
	inline void glDisableClientState(...) {}
	inline void glDrawArrays(...) {}
	inline void glEnable(...) {}
	inline void glEnableClientState(...) {}
	inline void glEndList(...) {}
	inline void glFog(...) {}
	inline void glFogf(...) {}
	inline void glFogi(...) {}
	inline void glGenTextures(...) {}
	inline void glLight(...) {}
	inline void glLightModel(...) {}
	inline void glLineWidth(...) {}
	inline void glLoadIdentity(...) {}
	inline void glMatrixMode(...) {}
	inline void glMultMatrix(...) {}
	inline void glNewList(...) {}
	inline void glNormal3f(...) {}
	inline void glOrtho(...) {}
	inline void glPixelStorei(...) {}
	inline void glPopMatrix(...) {}
	inline void glPushMatrix(...) {}
	inline void glReadPixels(...) {}
	inline void glRenderList(...) {}
	inline void glRotatef(...) {}
	inline void glScalef(...) {}
	inline void glShadeModel(...) {}
	inline void glSkyList(...) {}
	inline void glTexCoordPointer(...) {}
	inline void glTexImage2D(...) {}
	inline void glTexParameteri(...) {}
	inline void glTexSubImage2D(...) {}
	inline void glTranslatef(...) {}
	inline void glVertexPointer(...) {}
	inline void glViewport(...) {}
	inline int glGenLists(...) { return 1; }
	inline int glGenList(...) { return 1; }
	inline int glGetInteger(...) { return 0; }
	inline const char* glGetString(...) { return ""; }
	inline int glGetError() { return 0; }
	inline void glDeleteLists(...) {}
	inline void glPolygonOffset(...) {}
	inline void glGetFloat(...) {}
}
#endif // GL11_H
