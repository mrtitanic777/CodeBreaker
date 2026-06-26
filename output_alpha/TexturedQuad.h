#ifndef TEXTUREDQUAD_H
#define TEXTUREDQUAD_H

// Auto-generated from TexturedQuad.java
// Target: Xbox 360 / C++03 / DirectX 9

class PositionTexureVertex;
class Tesselator;

struct TexturedQuad {
public:
	TexturedQuad();
	TexturedQuad(PositionTexureVertex* var1);
	TexturedQuad(PositionTexureVertex* var1, int var2, int var3, int var4, int var5);
	// Rule of three: m_field_1195_a is an owned new[] array (delete[]'d in dtor).
	// The converter doesn't emit copy semantics, so value-copies (e.g.
	// ModelRenderer's m_faces[i] = TexturedQuad(...)) would shallow-share the
	// pointer and double-free. Deep-copy instead.
	TexturedQuad(const TexturedQuad& other);
	TexturedQuad& operator=(const TexturedQuad& other);
	virtual ~TexturedQuad();

	PositionTexureVertex* m_field_1195_a;
	int m_field_1194_b;

	virtual void func_809_a();
	virtual void func_808_a(Tesselator* var1, float var2);

public: // was private
	bool m_field_1196_c;

public:  // array-length companions
	int m_field_1195_a_count;

};

#endif // TEXTUREDQUAD_H
