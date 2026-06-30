#include "stdafx.h"        // xtl.h -> xbox.h: XUserGetName / XUserGetSigninState / XUSER_SIGNIN_STATE / XUSER_NAME_SIZE
#include "XboxUser.h"
#include <string>

namespace Xbox {

JavaString getGamertag()
{
	// User/controller index 0 = the primary signed-in profile (the player who launched the game).
	XUSER_SIGNIN_STATE st = XUserGetSigninState(0);
	if (st == eXUserSigninState_NotSignedIn) {
		return JavaString("");                       // no profile -> caller picks a default name
	}

	char name[XUSER_NAME_SIZE];                       // 16 = 15 chars + NUL
	name[0] = '\0';
	if (XUserGetName(0, name, XUSER_NAME_SIZE) != ERROR_SUCCESS || name[0] == '\0') {
		return JavaString("");
	}

	// Minecraft usernames don't allow spaces; gamertags can ("Major Nelson"). Map space -> '_' and
	// drop anything outside [A-Za-z0-9_] so the offline-mode SMP server (and chat/commands) accept it.
	std::string out;
	for (int i = 0; i < XUSER_NAME_SIZE && name[i] != '\0'; ++i) {
		char c = name[i];
		if (c == ' ') c = '_';
		if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
		    (c >= '0' && c <= '9') || c == '_') {
			out.push_back(c);
		}
	}
	return JavaString(out);
}

bool isSignedIn()
{
	return XUserGetSigninState(0) != eXUserSigninState_NotSignedIn;
}

} // namespace Xbox
