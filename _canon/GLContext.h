#ifndef GLCONTEXT_H
#define GLCONTEXT_H
// org.lwjgl.opengl.GLContext + ContextCapabilities stub. Capability flags are public bools
// (the real values come from the Xbox 360 platform/D3D9 layer) so `getCapabilities().GL_ARB_x`
// compiles. Default false = feature absent.
struct ContextCapabilities {
	bool GL_ARB_occlusion_query, GL_ARB_vertex_buffer_object, GL_ARB_multitexture;
	bool GL_ARB_texture_env_combine, GL_EXT_blend_func_separate, GL_ARB_shader_objects;
	bool GL_NV_fog_distance, GL_EXT_texture_filter_anisotropic, GL_ARB_texture_non_power_of_two;
	bool OpenGL11, OpenGL12, OpenGL13, OpenGL14, OpenGL15, OpenGL20, OpenGL21;
	ContextCapabilities() : GL_ARB_occlusion_query(false), GL_ARB_vertex_buffer_object(false),
		GL_ARB_multitexture(false), GL_ARB_texture_env_combine(false), GL_EXT_blend_func_separate(false),
		GL_ARB_shader_objects(false), GL_NV_fog_distance(false), GL_EXT_texture_filter_anisotropic(false),
		GL_ARB_texture_non_power_of_two(false), OpenGL11(false), OpenGL12(false), OpenGL13(false),
		OpenGL14(false), OpenGL15(false), OpenGL20(false), OpenGL21(false) {}
};
namespace GLContext {
	inline ContextCapabilities getCapabilities() { return ContextCapabilities(); }
}
#endif // GLCONTEXT_H
