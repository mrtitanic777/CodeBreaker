// SysNotify.cpp -- PLATFORM REFERENCE. Xbox 360 system-notification queue. See SysNotify.h.
//
// The whole subsystem is three XDK calls: create a listener over the SYSTEM notification area, pump it
// with XNotifyGetNext, close it. What matters is not the API but WHERE it is pumped and what the title
// does while the Guide is up -- see the SysNotify.h header note.

#include "stdafx.h"                  // forced PCH: must be the first include
#include "SysNotify.h"

#if defined(_XBOX) || defined(_XENON)
  #include <xtl.h>                   // XNotifyCreateListener / XNotifyGetNext / XN_SYS_* (xam.h)
#endif

namespace SysNotify {

// ----------------------------------------------------------------------------- state
#if defined(_XBOX) || defined(_XENON)
static HANDLE g_listener = NULL;
#endif
static bool g_guideUp    = false;
static bool g_signIn     = false;    // one-shot latches, cleared by the consume* readers
static bool g_pads       = false;
static SilenceHook g_silence = 0;

void setSilenceHook(SilenceHook fn) { g_silence = fn; }
bool isGuideUp()                    { return g_guideUp; }

bool consumeSignInChanged()      { bool v = g_signIn; g_signIn = false; return v; }
bool consumeControllersChanged() { bool v = g_pads;   g_pads   = false; return v; }

// ----------------------------------------------------------------------------- listener lifetime
void globalInit()
{
#if defined(_XBOX) || defined(_XENON)
	if (g_listener) return;                                  // idempotent
	g_listener = XNotifyCreateListener(XNOTIFY_SYSTEM);       // SYSTEM area: Guide, sign-in, devices
#endif
}

void shutdown()
{
#if defined(_XBOX) || defined(_XENON)
	if (g_listener) { CloseHandle(g_listener); g_listener = NULL; }
#endif
	g_guideUp = false;
}

// ----------------------------------------------------------------------------- queue
#if defined(_XBOX) || defined(_XENON)
// Guide transition. Entering: silence the title (a stall cannot stop audio already submitted). Leaving:
// let it resume. The hook is optional -- a port with no audio layer registers none.
static void _setGuide(bool up)
{
	if (up == g_guideUp) return;
	g_guideUp = up;
	if (g_silence) g_silence(up);
}

// Drain everything queued right now. XNotifyGetNext with dwMsgFilter 0 returns any id in the listened
// area and is non-blocking -- FALSE means "nothing left", not an error.
static void _drain()
{
	if (!g_listener) return;
	DWORD id = 0; ULONG_PTR param = 0;
	while (XNotifyGetNext(g_listener, 0, &id, &param)) {
		switch (id) {
		case XN_SYS_UI:
			// param != 0 -> the system UI is now VISIBLE; 0 -> it closed.
			_setGuide(param != 0);
			break;
		case XN_SYS_SIGNINCHANGED:
			g_signIn = true;
			break;
		case XN_SYS_INPUTDEVICESCHANGED:
			g_pads = true;
			break;
		default:
			// Everything else in the SYSTEM area (storage, profile settings, mute list, play timer, the
			// NUI family) is deliberately ignored: draining it is what keeps the queue from filling.
			break;
		}
	}
}
#endif

void poll()
{
#if defined(_XBOX) || defined(_XENON)
	if (!g_listener) globalInit();
	_drain();
	// GUIDE STALL: do not return to the game loop while the overlay is up. Sleeping (rather than
	// spinning) hands the hardware thread back; 50 ms is far below the perceptible reopen latency and
	// costs nothing while paused. Keep draining inside the loop or the close notification is never seen
	// and the title would hang here forever.
	while (g_guideUp) {
		Sleep(50);
		_drain();
	}
#endif
}

}   // namespace SysNotify
