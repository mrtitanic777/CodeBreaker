// codebreaker/engine/win16.hpp -- COMPAT SHIM over cb/engine/win16.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/engine/win16.{hpp,cpp}: the 16-bit module build as steps of the engine (describe, decompile, compile, gate, link), handed the
// platform's stages (the lane kit makes them). The old port was reached only by the old module builder's first step, which builds through the lane kit's
// module builder now, so nothing in the old layer calls it and this header keeps nothing. No logic.
#pragma once

#include "cb/engine/win16.hpp"
