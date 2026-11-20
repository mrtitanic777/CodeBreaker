#include "stdafx.h"        // xtl.h: XShowKeyboardUI / XOVERLAPPED / XGetOverlappedResult / VKBD_*
#include "OSK.h"
#include "Common.h"

#ifndef VKBD_LATIN_FULL
#define VKBD_LATIN_FULL 0x00000001
#endif

namespace {
	XOVERLAPPED s_ov;
	WCHAR       s_result[256];
	bool        s_active = false;
}

namespace OSK {

void show(const std::string& def, const wchar_t* title, const wchar_t* description, int maxChars) {
	if (s_active) return;
	memset(&s_ov, 0, sizeof(s_ov));
	memset(s_result, 0, sizeof(s_result));
	if (maxChars > 255) maxChars = 255;

	WCHAR wdef[256] = {0};
	for (int i = 0; i < (int)def.size() && i < 255; ++i) wdef[i] = (WCHAR)def[i];

	DWORD r = XShowKeyboardUI(
		0,                  // user 0
		VKBD_LATIN_FULL,    // full alphanumeric+symbols (IP, hostname, host:port)
		wdef,               // default text
		title,
		description,
		s_result,
		(DWORD)maxChars,
		&s_ov);
	s_active = (r == ERROR_IO_PENDING);
	if (!s_active) DbgPrintF("[osk] XShowKeyboardUI returned %lu (expected ERROR_IO_PENDING)\n", r);
}

bool active() { return s_active; }

bool poll(std::string& out) {
	if (!s_active) return false;
	if (!XHasOverlappedIoCompleted(&s_ov)) return false;

	DWORD res = 0;
	DWORD err = XGetOverlappedResult(&s_ov, &res, FALSE);
	s_active = false;

	if (err == ERROR_SUCCESS && res == ERROR_SUCCESS) {
		char narrow[256] = {0};
		WideCharToMultiByte(CP_ACP, 0, s_result, -1, narrow, 255, NULL, NULL);
		out = narrow;
		DbgPrintF("[osk] entered: %s\n", narrow);
	} else {
		out = "";   // cancelled / error
		DbgPrintF("[osk] cancelled (err=%lu res=%lu)\n", err, res);
	}
	return true;
}

} // namespace OSK
