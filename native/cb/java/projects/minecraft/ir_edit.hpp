// cb/java/projects/minecraft/ir_edit.hpp -- the tools the Minecraft fix passes (applet_ping, sound_fix, render_fix) change the
// translator's IR in place with. They are the translator's own pass tools, cb/translate/rewrite; this header keeps the name the
// passes use.
#pragma once

#include "cb/translate/rewrite.hpp"

namespace cb::java::projects::minecraft {

namespace ir_edit = cb::translate::rewrite;

}  // namespace cb::java::projects::minecraft
