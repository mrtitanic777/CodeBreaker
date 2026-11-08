// codebreaker/java/projects/minecraft/raycast.cpp -- C++ port of codebreaker.java.projects.minecraft.raycast.

#include "codebreaker/java/projects/minecraft/raycast.hpp"

namespace codebreaker::java::projects::minecraft::raycast {

// --- Minecraft-family GL-selection picking -> screen-center raycast (xbox360) -----------------------
// GL selection-mode picking (glRenderMode(GL_SELECT) + gluPickMatrix + a glPushName name stack) has NO
// Direct3D equivalent. Stage 3 could only route the *calls* to the d3d9 layer, where they were software-
// emulated -- fragile, viewport/cursor-dependent, and effectively non-functional on console. The correct
// port replaces the whole idiom with a raycast (an ALGORITHM, not a call mapping), leaving the game with
// ZERO GL selection semantics. This finalizer detects the idiom in emitted game code and rewrites it:
//   * the DRIVER   -- a `Class::pick(` body containing GL_SELECT  -> screen-center voxel raycast
//   * the RENDERER -- a body containing glPushName (name stack)   -> emptied (dead after the raycast)
//   * the PICK CAM -- a body containing gluPickMatrix              -> emptied (dead after the raycast)
// The raycast references Minecraft-family members (this->player{x,y,z,xo,yo,zo,xRot,yRot},
// this->level->isSolidTile, this->hitResult, HitResult(x,y,z,o,face)); it is installed only when those
// members are present in the file (can_raycast), so a non-Minecraft port is left untouched.
const py::str RAYCAST_PICK_BODY_ = py::str(
    "\n"
    "\t// [J2X-BUGFIX raycast-pick] GL selection-mode picking has no Direct3D equivalent; replaced at the\n"
    "\t// converter level with a screen-center raycast that ray-marches the voxel grid from the interpolated\n"
    "\t// eye along the view vector. Emits the same HitResult(x,y,z,0,face); the face convention matches the\n"
    "\t// place/destroy offsets in the game's render(). Also fixes the original per-frame HitResult leak.\n"
    "\tdelete this->hitResult;\n"
    "\tthis->hitResult = NULL;\n"
    "\tif (this->player == NULL || this->level == NULL) { return; }\n"
    "\tconst float j2xD2R = 3.14159265358979f / 180.0f;\n"
    "\tfloat yawRad   = this->player->yRot * j2xD2R;\n"
    "\tfloat pitchRad = this->player->xRot * j2xD2R;\n"
    "\t// Camera-forward vector (world space) for the RubyDung view matrix Rx(xRot)*Ry(yRot):\n"
    "\t//   forward = ( sin(yaw)cos(pitch), -sin(pitch), -cos(yaw)cos(pitch) ).\n"
    "\t// xRot is POSITIVE looking DOWN (Entity::turn does xRot -= yo), hence dirY = -sin(pitch); the\n"
    "\t// horizontal signs match where the CAMERA looks (not RubyDung's movement-forward, which is opposite).\n"
    "\tfloat dirX =  sinf(yawRad) * cosf(pitchRad);\n"
    "\tfloat dirY = -sinf(pitchRad);\n"
    "\tfloat dirZ = -cosf(yawRad) * cosf(pitchRad);\n"
    "\tfloat px = this->player->xo + (this->player->x - this->player->xo) * a;\n"
    "\tfloat py = this->player->yo + (this->player->y - this->player->yo) * a;\n"
    "\tfloat pz = this->player->zo + (this->player->z - this->player->zo) * a;\n"
    "\tint lastX = (int)floorf(px), lastY = (int)floorf(py), lastZ = (int)floorf(pz);\n"
    "\tbool hadLast = false;\n"
    "\tfor (float dist = 0.0f; dist < 5.0f; dist += 0.05f) {\n"
    "\t\tint bx = (int)floorf(px + dirX * dist);\n"
    "\t\tint by = (int)floorf(py + dirY * dist);\n"
    "\t\tint bz = (int)floorf(pz + dirZ * dist);\n"
    "\t\tif (this->level->isSolidTile(bx, by, bz)) {\n"
    "\t\t\t// J2X-REVIEW[side-effect-in-branch]: confirmed intentional -- the raycast records exactly one\n"
    "\t\t\t// HitResult under the solid-tile guard, then returns; the side effect must stay guarded here.\n"
    "\t\t\tint face = 1;\n"
    "\t\t\tif (hadLast) {\n"
    "\t\t\t\tint dx = bx - lastX, dy = by - lastY, dz = bz - lastZ;\n"
    "\t\t\t\tif (dy < 0) { face = 1; }\n"
    "\t\t\t\telse if (dy > 0) { face = 0; }\n"
    "\t\t\t\telse if (dz < 0) { face = 3; }\n"
    "\t\t\t\telse if (dz > 0) { face = 2; }\n"
    "\t\t\t\telse if (dx < 0) { face = 5; }\n"
    "\t\t\t\telse if (dx > 0) { face = 4; }\n"
    "\t\t\t}\n"
    "\t\t\tthis->hitResult = new HitResult(bx, by, bz, 0, face);\n"
    "\t\t\treturn;\n"
    "\t\t}\n"
    "\t\tlastX = bx; lastY = by; lastZ = bz; hadLast = true;\n"
    "\t}\n"
);
const py::str EMPTY_PICK_HELPER_ = py::str(
    "\n"
    "\t// [J2X-BUGFIX raycast-pick] GL-selection helper removed -- pick() now raycasts the voxel grid\n"
    "\t// directly, so this name-stack / pick-matrix code is dead (no GL selection semantics remain).\n"
);

// Rewrite the Minecraft-family GL-selection picking idiom to a raycast (see block comment above).
// Operates on emitted C++ text. Matches only real method DEFINITIONS (`Class::method(...) {`), never
// control-flow blocks, and brace-matches each body while skipping string/char literals.
py::str convert_gl_pick_to_raycast(const py::str& text) {
    if (!text.contains("GL_SELECT") && !text.contains("glPushName") && !text.contains("gluPickMatrix")) {
        return text;
    }
    bool can_raycast = text.contains("this->player") && text.contains("this->level") && text.contains("this->hitResult");
    // (body_start, body_end_excl, replacement) -- in order, non-overlapping
    struct Span {
        int64_t start;
        int64_t end;
        const py::str* replacement;
    };
    py::list<Span> spans;
    static const py::re::Pattern def = py::re::compile(py::str("\\b[A-Za-z_]\\w*::[A-Za-z_]\\w*\\s*\\([^;{}]*\\)\\s*(?:const\\s*)?\\{"));
    const std::u32string& u = text.u32();
    for (const py::re::Match& m : def.finditer(text)) {
        int64_t open_brace = m.end() - 1;
        int64_t i = open_brace;
        int64_t n = static_cast<int64_t>(u.size());
        int64_t depth = 0;
        std::optional<int64_t> close;
        while (i < n) {
            char32_t c = u[static_cast<size_t>(i)];
            if (c == U'"' || c == U'\'') {
                char32_t q = c;
                i += 1;
                while (i < n) {
                    if (u[static_cast<size_t>(i)] == U'\\') {
                        i += 2;
                        continue;
                    }
                    if (u[static_cast<size_t>(i)] == q) {
                        break;
                    }
                    i += 1;
                }
            } else if (c == U'{') {
                depth += 1;
            } else if (c == U'}') {
                depth -= 1;
                if (depth == 0) {
                    close = i;
                    break;
                }
            }
            i += 1;
        }
        if (!close.has_value()) {
            continue;
        }
        py::str body = text.slice(open_brace + 1, *close);
        const py::str* repl = nullptr;
        if (body.contains("GL_SELECT") && can_raycast) {
            repl = &RAYCAST_PICK_BODY_;
        } else if (body.contains("glPushName") || body.contains("gluPickMatrix")) {
            repl = &EMPTY_PICK_HELPER_;  // dead GL-selection helper; safe to strip on D3D
        }
        if (repl != nullptr) {
            spans.append(Span{open_brace + 1, *close, repl});
        }
    }
    if (spans.empty()) {
        return text;
    }
    py::list<py::str> out;
    int64_t idx = 0;
    for (const Span& sp : spans) {
        out.append(text.slice(idx, sp.start));
        out.append(*sp.replacement);
        idx = sp.end;
    }
    out.append(text.slice_from(idx));
    return py::join(py::str(""), out);
}

}  // namespace codebreaker::java::projects::minecraft::raycast
