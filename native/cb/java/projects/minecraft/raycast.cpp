#include "cb/java/projects/minecraft/raycast.hpp"

#include <optional>
#include <vector>

#include "cb/core/regex.hpp"

namespace cb::java::projects::minecraft::raycast {

namespace {

// The picking driver's replacement. It ray-marches the voxel grid from the interpolated eye along the view vector
// and emits the same HitResult(x,y,z,0,face); the face convention matches the place/destroy offsets in the game's
// render(). It also fixes the original per-frame HitResult leak.
constexpr std::string_view kRaycastPickBody =
R"cpp(
	// [J2X-BUGFIX raycast-pick] GL selection-mode picking has no Direct3D equivalent; replaced at the
	// converter level with a screen-center raycast that ray-marches the voxel grid from the interpolated
	// eye along the view vector. Emits the same HitResult(x,y,z,0,face); the face convention matches the
	// place/destroy offsets in the game's render(). Also fixes the original per-frame HitResult leak.
	delete this->hitResult;
	this->hitResult = NULL;
	if (this->player == NULL || this->level == NULL) { return; }
	const float j2xD2R = 3.14159265358979f / 180.0f;
	float yawRad   = this->player->yRot * j2xD2R;
	float pitchRad = this->player->xRot * j2xD2R;
	// Camera-forward vector (world space) for the RubyDung view matrix Rx(xRot)*Ry(yRot):
	//   forward = ( sin(yaw)cos(pitch), -sin(pitch), -cos(yaw)cos(pitch) ).
	// xRot is POSITIVE looking DOWN (Entity::turn does xRot -= yo), hence dirY = -sin(pitch); the
	// horizontal signs match where the CAMERA looks (not RubyDung's movement-forward, which is opposite).
	float dirX =  sinf(yawRad) * cosf(pitchRad);
	float dirY = -sinf(pitchRad);
	float dirZ = -cosf(yawRad) * cosf(pitchRad);
	float px = this->player->xo + (this->player->x - this->player->xo) * a;
	float py = this->player->yo + (this->player->y - this->player->yo) * a;
	float pz = this->player->zo + (this->player->z - this->player->zo) * a;
	int lastX = (int)floorf(px), lastY = (int)floorf(py), lastZ = (int)floorf(pz);
	bool hadLast = false;
	for (float dist = 0.0f; dist < 5.0f; dist += 0.05f) {
		int bx = (int)floorf(px + dirX * dist);
		int by = (int)floorf(py + dirY * dist);
		int bz = (int)floorf(pz + dirZ * dist);
		if (this->level->isSolidTile(bx, by, bz)) {
			// J2X-REVIEW[side-effect-in-branch]: confirmed intentional -- the raycast records exactly one
			// HitResult under the solid-tile guard, then returns; the side effect must stay guarded here.
			int face = 1;
			if (hadLast) {
				int dx = bx - lastX, dy = by - lastY, dz = bz - lastZ;
				if (dy < 0) { face = 1; }
				else if (dy > 0) { face = 0; }
				else if (dz < 0) { face = 3; }
				else if (dz > 0) { face = 2; }
				else if (dx < 0) { face = 5; }
				else if (dx > 0) { face = 4; }
			}
			this->hitResult = new HitResult(bx, by, bz, 0, face);
			return;
		}
		lastX = bx; lastY = by; lastZ = bz; hadLast = true;
	}
)cpp";

constexpr std::string_view kEmptyPickHelper =
R"cpp(
	// [J2X-BUGFIX raycast-pick] GL-selection helper removed -- pick() now raycasts the voxel grid
	// directly, so this name-stack / pick-matrix code is dead (no GL selection semantics remain).
)cpp";

// A method DEFINITION header: `Class::method(...) [const] {`. A name may hold letters beyond ASCII (any byte of a
// multi-byte character counts as part of the name).
const Regex& definition_header() {
    static const Regex re(
        R"(\b[A-Za-z_](?:\w|[^\x00-\x7f])*::[A-Za-z_](?:\w|[^\x00-\x7f])*\s*\([^;{}]*\)\s*(?:const\s*)?\{)");
    return re;
}

// The index of the brace that closes the one at `open`, skipping string/char literals and comments (an apostrophe
// in a comment must not start a character literal); nullopt if the text ends first.
std::optional<std::size_t> matching_brace(std::string_view s, std::size_t open) {
    std::size_t depth = 0;
    for (std::size_t i = open; i < s.size(); ++i) {
        const char c = s[i];
        if (c == '"' || c == '\'') {
            for (++i; i < s.size() && s[i] != c; ++i) {
                if (s[i] == '\\') ++i;
            }
        } else if (c == '/' && i + 1 < s.size() && s[i + 1] == '/') {
            const std::size_t nl = s.find('\n', i);
            if (nl == std::string_view::npos) return std::nullopt;
            i = nl;
        } else if (c == '/' && i + 1 < s.size() && s[i + 1] == '*') {
            const std::size_t end = s.find("*/", i + 2);
            if (end == std::string_view::npos) return std::nullopt;
            i = end + 1;
        } else if (c == '{') {
            ++depth;
        } else if (c == '}') {
            if (--depth == 0) return i;
        }
    }
    return std::nullopt;
}

bool has(std::string_view s, std::string_view part) { return s.find(part) != std::string_view::npos; }

}  // namespace

std::string_view raycast_pick_body() { return kRaycastPickBody; }

std::string_view empty_pick_helper() { return kEmptyPickHelper; }

std::string convert_gl_pick_to_raycast(std::string_view text) {
    if (!has(text, "GL_SELECT") && !has(text, "glPushName") && !has(text, "gluPickMatrix")) return std::string(text);
    const bool can_raycast = has(text, "this->player") && has(text, "this->level") && has(text, "this->hitResult");

    struct Edit {
        std::size_t begin;   // the first byte of the body (after its `{`)
        std::size_t end;     // the closing `}`
        std::string_view replacement;
    };
    std::vector<Edit> edits;   // in order, non-overlapping
    std::size_t resume = 0;    // where the last replaced body ended: a header inside it is part of it
    definition_header().for_each_match(text, [&](const RegexMatch& m) {
        const std::size_t open = m.end() - 1;
        if (open < resume) return true;
        const std::optional<std::size_t> close = matching_brace(text, open);
        if (!close) return true;
        const std::string_view body = text.substr(open + 1, *close - open - 1);
        std::string_view replacement;
        if (has(body, "GL_SELECT") && can_raycast) {
            replacement = kRaycastPickBody;
        } else if (has(body, "glPushName") || has(body, "gluPickMatrix")) {
            replacement = kEmptyPickHelper;   // a dead GL-selection helper: safe to strip on D3D
        }
        if (!replacement.empty()) {
            edits.push_back({open + 1, *close, replacement});
            resume = *close;
        }
        return true;
    });
    if (edits.empty()) return std::string(text);

    std::string out;
    std::size_t at = 0;
    for (const Edit& e : edits) {
        out.append(text.substr(at, e.begin - at));
        out.append(e.replacement);
        at = e.end;
    }
    out.append(text.substr(at));
    return out;
}

}  // namespace cb::java::projects::minecraft::raycast
