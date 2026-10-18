#ifndef OSK_H
#define OSK_H
// ============================== Xbox 360 on-screen keyboard ===================================
// Wraps XShowKeyboardUI (the system virtual keyboard) so converted Gui screens with text fields
// (e.g. the multiplayer server-address field) can take controller text input -- there is no PC
// keyboard on the console. Async: show() pops the overlay, poll() once per tick returns true when
// the user finishes. Pattern ported from Minecraft.Client GuiNameLevel.
// =============================================================================================
#include <string>

namespace OSK {
	// Pop the system keyboard with the given default text/title/description. No-op if one is already up.
	void show(const std::string& defaultText, const wchar_t* title, const wchar_t* description, int maxChars);
	// True while the keyboard overlay is active.
	bool active();
	// Call once per tick. Returns true the single tick the user finishes; fills outResult with the typed
	// text (or leaves it empty if the user cancelled).
	bool poll(std::string& outResult);
}
#endif // OSK_H
