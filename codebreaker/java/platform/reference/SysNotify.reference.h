// SysNotify.h -- game-AGNOSTIC Xbox 360 system-notification handling for the j2ctool target.
//
// PLATFORM REFERENCE. The console tells a title about system events through a NOTIFICATION QUEUE, not
// through window messages: the Guide (dashboard overlay) opening or closing, the signed-in profile
// changing, controllers being plugged or unplugged, storage appearing or vanishing. A title that ignores
// the queue keeps simulating and keeps playing audio while the Guide is up, which is a CERTIFICATION
// FAILURE -- and never notices that the player signed out or yanked the pad.
//
// Nothing in the converted Java game knows this concept exists (a PC game has no Guide), so this is
// pure platform layer: the queue is drained from the one place a converted game reliably reaches every
// frame -- Display::update(), the LWJGL present point, right next to the input pump.
//
// GUIDE = HARD STALL. When the Guide opens, poll() does NOT return: it sleeps and keeps draining until
// the Guide closes. Because poll() is called from the frame pump, the caller's game loop stalls inside
// it -- no ticks, no physics, no new sounds, and the last rendered frame stays on screen behind the
// overlay. That is the cert-correct behavior and it needs ZERO cooperation from the game, which is why
// it is done here instead of asking every port to thread a paused flag through its main loop.
//
// SUSTAINED audio is the one thing a stall cannot fix -- music already submitted keeps playing. The
// audio layer therefore registers a silence hook (see setSilenceHook); the audio reference does this at
// init, so a port that links audio gets it automatically and a port that does not still compiles.
//
// API/ID provenance: XNotifyCreateListener / XNotifyGetNext and the XN_SYS_* ids are read from the
// installed XDK (include/xbox/xam.h), not from memory. Everything compiles to a no-op off-console so
// the same tree still builds for a PC/host target.

#ifndef SYSNOTIFY_H
#define SYSNOTIFY_H

namespace SysNotify {

	// Called with true when the title must go silent (Guide opening), false when it may resume.
	typedef void (*SilenceHook)(bool silence);

	// Create the system-notification listener. Idempotent; poll() self-initializes, so calling this is
	// optional -- it exists so a title can pay the cost at boot instead of on the first frame.
	void globalInit();

	// Release the listener. Safe to call without a preceding globalInit().
	void shutdown();

	// Drain the queue, then BLOCK while the Guide is up (see the header note). Call once per frame.
	void poll();

	// True while the Guide / system UI is overlaying the title. Only observable from another thread:
	// on the pump thread poll() does not return while it is true.
	bool isGuideUp();

	// One-shot reads -- each returns true ONCE per event, then resets. The signed-in profile changed
	// (re-read the gamertag; a save in flight may now belong to nobody) / a controller was added or
	// removed (re-acquire the pad, or put up a "reconnect controller" prompt).
	bool consumeSignInChanged();
	bool consumeControllersChanged();

	// Register the audio gate. Pass 0 to unregister. The audio platform layer calls this at init.
	void setSilenceHook(SilenceHook fn);

}   // namespace SysNotify

#endif // SYSNOTIFY_H
