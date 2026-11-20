// java/finalize.hpp -- the text finalisers applied to EVERY emitted .h and .cpp.
//
// The statement translator and the source emitter each write C++ for the parts they own; these rewrites are the last word over
// the whole text, so they also catch what the emitters wrote outside method bodies (static-data arrays, inlined constructor
// bodies):
//   * an enum constant that collides with a macro (`EnumX::NULL` -> `EnumX::NULL_`);
//   * the invalid `'\u0000'` C++ character literal -> the standard `'\0'`;
//   * `new Object()` (a bare java.lang.Object, used only as a monitor or lock) -> a JObject VALUE: Object has no C++ class, the
//     universal box default-constructs, and the sync object is never used on the port;
//   * a discarded `new X(...);` -> `delete new X(...);`;
//   * on the Xbox 360, OpenGL call sites -> the Direct3D 9 layer, and every `throw` tagged with its site.
#pragma once

#include <array>
#include <set>
#include <span>
#include <string>
#include <string_view>

#include "java/projects/package.hpp"

namespace cb::java::finalize {

// The OpenGL API namespaces the Direct3D 9 layer replaces.
inline constexpr std::array<std::string_view, 2> kGlNamespaces = {"GL11", "GLU"};

struct Options {
    // The console runs NO OpenGL: every `GL11::glX`, `GLU::gluX` and `GL11::GL_CONST` of the converted game becomes the native
    // Direct3D 9 layer (namespace d3d9). The OpenGL->Direct3D9 registry (apimap/opengl.json, glu.json) is the authoritative
    // table; this applies it by rewriting the call sites so the emitted code contains Direct3D 9, not OpenGL.
    bool gl_to_d3d9 = false;
    // On the 360 emulator a C++ `throw` is a no-op that does not unwind (a landmine): tag every emitted throw with its site
    // (JavaRT.h's J2X_GUEST_THROW) so a fired one names its file:line in the debug log.
    bool trace_throws = false;
    // The project's last word on the text (Minecraft: GL picking -> a raycast), before the namespace rename; none: no project.
    const projects::Project* project = nullptr;
    // Classes whose construction hands `this` to someone (a registry, a list, a listener): a discarded `new` of one is kept by
    // someone else, and deleting it would leave them a dangling pointer (Item registers itself in Item.itemsList).
    const std::set<std::string, std::less<>>* this_escapes = nullptr;
};

// `GL11::` and `GLU::` (`namespaces`) -> `d3d9::`, wherever the namespace stands as a whole word. Only the `Ns::` qualified form is
// touched: `#include "GL11.h"` is left alone.
std::string convert_gl_to_d3d9(std::string_view text, std::span<const std::string_view> namespaces = kGlNamespaces);

// Prefix every LINE-LEADING `throw` statement with a separate `J2X_GUEST_THROW();` statement so a fired throw names its file:line.
// Robust by simplicity: it inserts a whole statement BEFORE the throw and never touches the throw's own text, so it cannot be fooled
// by a `;` inside a string literal (`throw new X("]);")`), a multi-line throw expression, or nested parentheses -- the three things
// that break any "find the statement end" approach. Only throws that begin their line are instrumented (the converter writes braced
// blocks, so essentially all do); a mid-line `if(c) throw ...;` is left alone rather than risk mangling it. `throws` in a method
// signature is excluded (the character after `throw` must be a space, a tab or `;`).
std::string trace_guest_throws(std::string_view text);

std::string finalize_emitted(std::string_view text, const Options& options = {});

}  // namespace cb::java::finalize
