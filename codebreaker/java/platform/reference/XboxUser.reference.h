#ifndef XBOXUSER_H
#define XBOXUSER_H
#include "JavaString.h"
// ================================ Xbox 360 profile helpers ====================================
// Reads the signed-in profile on user/controller index 0 via the XDK XUser* APIs (xbox.h, pulled
// in through stdafx.h -> xtl.h). Used to drive the Minecraft username from the console gamertag so
// the player has a stable identity online (instead of a random "Player###"). See the verified
// signatures in docs/platform/XboxUser.reference.* :
//   XUSER_SIGNIN_STATE XUserGetSigninState(DWORD userIndex);
//   DWORD              XUserGetName(DWORD userIndex, LPSTR buf, DWORD cch);   // ERROR_SUCCESS == 0
//   XUSER_NAME_SIZE == 16 (15 chars + NUL)
namespace Xbox {
	// Sanitized gamertag for index 0, or "" if no profile is signed in there. Spaces -> '_' and any
	// char outside [A-Za-z0-9_] is dropped so the SMP server / chat / commands accept the name.
	JavaString getGamertag();
	bool isSignedIn();
}
#endif // XBOXUSER_H
