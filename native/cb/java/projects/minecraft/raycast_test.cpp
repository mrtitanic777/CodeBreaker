// DIVERGES (checked against the Python on its 261 recorded cases; the old algorithm re-implemented in the one-off replay
// reproduces all of them, so the differences below are the whole of it):
//   * a definition-shaped text INSIDE a body that is being replaced (`void P::q() { void R::s(int) { gluPickMatrix(); } }`) is
//     part of that body. The Python and the old port spliced the two overlapping edits and duplicated text (182 of the 261
//     recorded cases contain such a nest, the fuzzed ones too).
//   * a quote or apostrophe inside a comment of the body no longer starts a character literal (`// don't`): the old brace
//     matching lost count and left the method alone, or took the wrong body.

#include <cstdint>
#include <string>

#include "cb/core/testing.hpp"
#include "cb/java/projects/minecraft/raycast.hpp"

namespace {

using namespace cb::java::projects::minecraft::raycast;

std::uint64_t fnv1a(std::string_view s) {
    std::uint64_t h = 0xcbf29ce484222325ULL;
    for (const char c : s) h = (h ^ static_cast<unsigned char>(c)) * 0x100000001b3ULL;
    return h;
}

const std::string kPlayer =
    "void Minecraft::init() { this->player = NULL; this->level = NULL; this->hitResult = NULL; }\n";

std::string pick_with(std::string_view body) { return "void Minecraft::pick(float a) {" + std::string(body) + "}\n"; }

CB_TEST("java.minecraft.raycast.templates_are_the_proven_ones", [] {
    // Byte-for-byte the text of the Python module's templates (size and FNV-1a taken from it).
    CB_CHECK_EQ(raycast_pick_body().size(), std::size_t{2326});
    CB_CHECK_EQ(fnv1a(raycast_pick_body()), 0x25fcbd581db271f6ULL);
    CB_CHECK_EQ(empty_pick_helper().size(), std::size_t{193});
    CB_CHECK_EQ(fnv1a(empty_pick_helper()), 0xf5df1e1908fbc5deULL);
    // what the game relies on: the same HitResult constructor and early outs
    CB_CHECK(raycast_pick_body().find("this->hitResult = new HitResult(bx, by, bz, 0, face);") != std::string_view::npos);
    CB_CHECK(raycast_pick_body().find("if (this->player == NULL || this->level == NULL) { return; }") != std::string_view::npos);
});

CB_TEST("java.minecraft.raycast.driver_is_replaced", [] {
    const std::string pick = "void Minecraft::pick(float a) {\n    glRenderMode(GL_SELECT);\n    if (x) { y(); }\n    glInitNames();\n}\n";
    CB_CHECK_EQ(convert_gl_pick_to_raycast(kPlayer + pick), kPlayer + pick_with(raycast_pick_body()));
    // three drivers are three replacements
    CB_CHECK_EQ(convert_gl_pick_to_raycast(kPlayer + pick + pick + pick),
                kPlayer + pick_with(raycast_pick_body()) + pick_with(raycast_pick_body()) + pick_with(raycast_pick_body()));
});

CB_TEST("java.minecraft.raycast.driver_needs_the_game_members", [] {
    const std::string pick = "void Minecraft::pick(float a) {\n    glRenderMode(GL_SELECT);\n}\n";
    // without this->player / this->level / this->hitResult in the file this is not the Minecraft idiom: left alone
    CB_CHECK_EQ(convert_gl_pick_to_raycast(pick), pick);
    CB_CHECK_EQ(convert_gl_pick_to_raycast("void A::f() { GL_SELECT; }\nint x = this->player + this->level;\n"),
                "void A::f() { GL_SELECT; }\nint x = this->player + this->level;\n");
});

CB_TEST("java.minecraft.raycast.helpers_are_emptied", [] {
    const std::string push = "void LevelRenderer::renderHit(int i) {\n    glPushName(i);\n    draw();\n}\n";
    const std::string cam = "void GameRenderer::pickCamera(float a)\n{\n    gluPickMatrix(1, 2, 3, 4, v);\n}\n";
    // a name-stack renderer and a pick-matrix camera are dead after the raycast, with or without the game members
    CB_CHECK_EQ(convert_gl_pick_to_raycast(push),
                "void LevelRenderer::renderHit(int i) {" + std::string(empty_pick_helper()) + "}\n");
    CB_CHECK_EQ(convert_gl_pick_to_raycast(kPlayer + cam),
                kPlayer + "void GameRenderer::pickCamera(float a)\n{" + std::string(empty_pick_helper()) + "}\n");
    // all three together
    const std::string pick = "void Minecraft::pick(float a) {\n    glRenderMode(GL_SELECT);\n}\n";
    CB_CHECK_EQ(convert_gl_pick_to_raycast(kPlayer + pick + push + cam),
                kPlayer + pick_with(raycast_pick_body()) + "void LevelRenderer::renderHit(int i) {" +
                    std::string(empty_pick_helper()) + "}\nvoid GameRenderer::pickCamera(float a)\n{" +
                    std::string(empty_pick_helper()) + "}\n");
});

CB_TEST("java.minecraft.raycast.const_methods_and_spacing", [] {
    CB_CHECK_EQ(convert_gl_pick_to_raycast("int Minecraft::pickIt(float a, int b) const {\n  int s = GL_SELECT; return s;\n}\n" + kPlayer),
                "int Minecraft::pickIt(float a, int b) const {" + std::string(raycast_pick_body()) + "}\n" + kPlayer);
    CB_CHECK_EQ(convert_gl_pick_to_raycast("void A::f(int a) const{glPushName(0);}"),
                "void A::f(int a) const{" + std::string(empty_pick_helper()) + "}");
    CB_CHECK_EQ(convert_gl_pick_to_raycast("void _a1::_b2 ( ) { gluPickMatrix }"),
                "void _a1::_b2 ( ) {" + std::string(empty_pick_helper()) + "}");
});

CB_TEST("java.minecraft.raycast.not_definitions_are_left_alone", [] {
    // (a free function, a control-flow block, a header with `;` or `{` in its parameter list: none is `Class::method(...) {`)
    for (const char* text : {"", "plain text", "void Q::x() { return; }\n", "GL_SELECT", "glPushName", "gluPickMatrix",
                             "void run() { if (a) { glRenderMode(GL_SELECT); } }\nvoid B::g() { if (z) { y(); } }\n",
                             "A::f() { }", "void A::f() {", "void A::f(x;y) { glPushName }", "void A::f({) { glPushName }",
                             "void run() { glPushName(1); }"}) {
        CB_CHECK_EQ(convert_gl_pick_to_raycast(text), text);
        CB_CHECK_EQ(convert_gl_pick_to_raycast(std::string(text) + kPlayer), std::string(text) + kPlayer);
    }
});

CB_TEST("java.minecraft.raycast.braces_in_literals_do_not_count", [] {
    // braces and quotes inside string and character literals must not end the body early
    const std::string body = "\n  puts(\"}{ GL_SELECT \\\" glPushName\"); char c = '{'; char d = '}';\n  glPushName(1);\n";
    const std::string text = "void A::f() {" + body + "}\n";
    CB_CHECK_EQ(convert_gl_pick_to_raycast(text), "void A::f() {" + std::string(empty_pick_helper()) + "}\n");
    // with the game members present, the GL_SELECT inside the literal makes it a driver (the body text is what is searched)
    CB_CHECK_EQ(convert_gl_pick_to_raycast(kPlayer + text), kPlayer + "void A::f() {" + std::string(raycast_pick_body()) + "}\n");
});

CB_TEST("java.minecraft.raycast.unclosed_bodies_are_left_alone", [] {
    const std::string open_brace = "void K::broken() {\n   glPushName(1);\n   if (x) {\n";
    CB_CHECK_EQ(convert_gl_pick_to_raycast(open_brace), open_brace);
    const std::string open_string = "void K::s() { glPushName(1); puts(\"abc }\n";
    CB_CHECK_EQ(convert_gl_pick_to_raycast(open_string), open_string);
    const std::string backslash_end = "void K::s() { glPushName(1); puts(\"abc\\";
    CB_CHECK_EQ(convert_gl_pick_to_raycast(backslash_end), backslash_end);
});

CB_TEST("java.minecraft.raycast.nested_and_neighbouring_definitions", [] {
    // an inner "definition" inside a body that is being replaced is part of that body (the old code spliced the
    // overlapping edits into garbage); a neighbour after it is its own edit
    const std::string text = "void P::q() { void R::s(int x) { gluPickMatrix(x); } } void T::u() { glPushName(2); }\n";
    const std::string h(empty_pick_helper());
    CB_CHECK_EQ(convert_gl_pick_to_raycast(text), "void P::q() {" + h + "} void T::u() {" + h + "}\n");
});

CB_TEST("java.minecraft.raycast.utf8_text_passes_through", [] {
    // text with multi-byte characters around the idiom: the bytes outside the bodies are copied as they are, and a
    // class or method name with a non-ASCII letter (Z, e-acute) is one name, as it was for the Python's \w
    const std::string text = "// caf\xC3\xA9 \xE4\xB8\xAD\nvoid \xC3\x89::f() {}\nvoid Z\xC3\xA9::g() { glPushName(1); } // \xF0\x9F\x98\x80\n"
                             "void C::pick(float a) { GL_SELECT; }\n";
    const std::string out = convert_gl_pick_to_raycast(text + kPlayer);
    CB_CHECK(out.starts_with("// caf\xC3\xA9 \xE4\xB8\xAD\nvoid \xC3\x89::f() {}\nvoid Z\xC3\xA9::g() {" + std::string(empty_pick_helper()) +
                             "} // \xF0\x9F\x98\x80\n"));
    CB_CHECK(out.ends_with(kPlayer));
    CB_CHECK(out.find("void C::pick(float a) {" + std::string(raycast_pick_body()) + "}\n") != std::string::npos);
    // (a name that STARTS with a non-ASCII letter is not an identifier here either: `\xC3\x89::f` is left alone)
});

// DIVERGES: an apostrophe or quote in a comment inside the body. The Python and the old port read it as the start
// of a character literal, ran to the next quote, lost count of the braces and left the method (or took a wrong
// body). Comments are now skipped, so the body is found and replaced.
CB_TEST("java.minecraft.raycast.comments_with_quotes", [] {
    const std::string text = "void A::f() {\n // don't count this\n glPushName(1);\n}\nvoid B::g() { glPushName(2); }\n";
    const std::string h(empty_pick_helper());
    CB_CHECK_EQ(convert_gl_pick_to_raycast(text), "void A::f() {" + h + "}\nvoid B::g() {" + h + "}\n");
    const std::string block = "void A::f() {\n /* a } brace \" and a quote */ glPushName(1);\n}\n";
    CB_CHECK_EQ(convert_gl_pick_to_raycast(block), "void A::f() {" + h + "}\n");
});

}  // namespace
