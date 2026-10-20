#include "stdafx.h"
#include "Sys.h"
#include <SDL2/SDL.h>

namespace Sys {
long long getTime() { return (long long)SDL_GetTicks64(); }
}
