#include "stdafx.h"
// Hand-written PORT override for GuiMultiplayer.java (Minecraft Alpha v1.2.6)
// ------------------------------------------------------------------------
// Started from the converter output (ConversionTool\output_alpha\GuiMultiplayer.cpp, ~21 errors)
// and fixed to compile against Alpha's GuiMultiplayer.h, using handwritten_alpha\GuiMainMenu.cpp
// as the porting guide for Alpha Gui idioms.
//
// The 360 port is SINGLE-PLAYER only. There is no NetClientHandler / java.net networking, so the
// actual "Connect" action (which in Java spun up a GuiConnecting + socket) is a // PORT: no-op.
// The screen still compiles, renders its title/help text + IP text-field + Connect/Cancel buttons,
// and the Cancel button returns to the parent screen.
//
// Notable converter fixes:
//   * Minecraft was an incomplete type -> #include "Minecraft.h" + "GameSettings.h".
//   * Java String methods that std::string lacks were reimplemented locally:
//       replaceAll(from,to)  -> replaceAll_str()
//       split(":")           -> splitFirst() (only the first ':' split is used: host[:port])
//       trim()               -> trim_str()
//       big charset.indexOf  -> isTypeableChar() printable-ASCII test (Java's set was the
//                               CP437 typeable glyphs; on the 360 keyboard only ASCII matters,
//                               and the ⌂.. literals tripped C4566 under code page 1252).
//   * atoi(var1.trim(.c_str())) garbled syntax -> atoi(trim_str(var1).c_str()).
//   * (GuiButton)x cast and Java pointer/dot mixups resolved.

#include "GuiMultiplayer.h"
#include "Common.h"
#include "GuiScreen.h"
#include "GuiButton.h"
#include "FontRenderer.h"
#include "Minecraft.h"
#include "GameSettings.h"
#include <string>
#include <cstdlib>

// ---------------------------------------------------------------------------
// PORT helpers: small reimplementations of the Java String methods used here.
// ---------------------------------------------------------------------------
static std::string replaceAll_str(const std::string& s, const std::string& from, const std::string& to) {
	if (from.empty()) {
		return s;
	}
	std::string out;
	out.reserve(s.length());
	std::string::size_type pos = 0;
	std::string::size_type prev = 0;
	while ((pos = s.find(from, prev)) != std::string::npos) {
		out.append(s, prev, pos - prev);
		out.append(to);
		prev = pos + from.length();
	}
	out.append(s, prev, s.length() - prev);
	return out;
}

static std::string trim_str(const std::string& s) {
	std::string::size_type b = 0;
	std::string::size_type e = s.length();
	while (b < e && (unsigned char)s[b] <= ' ') {
		++b;
	}
	while (e > b && (unsigned char)s[e - 1] <= ' ') {
		--e;
	}
	return s.substr(b, e - b);
}

// Java's keyTyped allowed only a fixed set of typeable glyphs. The CP437 tail of that set is
// irrelevant on the 360 (keyboard only emits ASCII), and the ⌂.. literals trip MSVC C4566
// under code page 1252. So accept the printable ASCII range, matching the relevant part of the set.
static bool isTypeableChar(char c) {
	return c >= 32 && c < 127;
}

// ---------------------------------------------------------------------------

GuiMultiplayer::GuiMultiplayer()
	: m_updateCounter(NULL)
	, m_parentScreen(0)
	, m_serverAddress("")
{
}

GuiMultiplayer::GuiMultiplayer(GuiScreen* var1)
	: m_updateCounter(NULL)
	, m_parentScreen(0)
	, m_serverAddress("")
{
	m_updateCounter = var1;
}

GuiMultiplayer::~GuiMultiplayer() {
	m_updateCounter = NULL;
}

void GuiMultiplayer::updateScreen() {
	++m_parentScreen;
}

void GuiMultiplayer::initGui() {
	controlList.clear();
	controlList.push_back(new GuiButton(0, width / 2 - 100, height / 4 + 96 + 12, "Connect"));
	controlList.push_back(new GuiButton(1, width / 2 - 100, height / 4 + 120 + 12, "Cancel"));
	m_serverAddress = replaceAll_str(mc->m_gameSettings->m_field_12259_z, "_", ":");
	controlList[0]->m_enabled = m_serverAddress.length() > 0;
}

void GuiMultiplayer::actionPerformed(GuiButton* var1) {
	if (var1->m_enabled) {
		if (var1->m_id == 1) {
			// Cancel -> back to the parent screen.
			mc->displayGuiScreen(m_updateCounter);
		} else if (var1->m_id == 0) {
			// Connect: persist the entered address (Java stored it underscore-encoded), then
			// PORT: no-op. Java built host[:port] and opened a GuiConnecting socket via
			// NetClientHandler / java.net -- there is no multiplayer on the 360.
			mc->m_gameSettings->m_field_12259_z = replaceAll_str(m_serverAddress, ":", "_");
			mc->m_gameSettings->saveOptions();
			// PORT: NetClientHandler / GuiConnecting networking dropped (single-player port).
		}
	}
}

int GuiMultiplayer::func_4067_a(const std::string& var1, int var2) {
	// Java parsed the port substring; on a parse failure it returned the default (var2).
	std::string trimmed = trim_str(var1);
	if (trimmed.empty()) {
		return var2;
	}
	for (std::string::size_type i = 0; i < trimmed.length(); ++i) {
		if (trimmed[i] < '0' || trimmed[i] > '9') {
			return var2; // not a valid integer -> default, matching the Java catch block.
		}
	}
	return atoi(trimmed.c_str());
}

void GuiMultiplayer::keyTyped(char var1, int var2) {
	if (var1 == 22) { // Ctrl+V paste
		std::string var3 = GuiScreen::getClipboardString();
		if (var3.empty()) {
			var3 = "";
		}

		int var4 = 32 - (int)m_serverAddress.length();
		if (var4 > (int)var3.length()) {
			var4 = (int)var3.length();
		}

		if (var4 > 0) {
			m_serverAddress = m_serverAddress + var3.substr(0, var4);
		}
	}

	if (var1 == 13) { // Enter -> trigger Connect
		actionPerformed(controlList[0]);
	}

	if (var2 == 14 && m_serverAddress.length() > 0) { // Backspace
		m_serverAddress = m_serverAddress.substr(0, m_serverAddress.length() - 1);
	}

	if (isTypeableChar(var1) && m_serverAddress.length() < 32) {
		m_serverAddress = m_serverAddress + var1;
	}

	controlList[0]->m_enabled = m_serverAddress.length() > 0;
}

void GuiMultiplayer::drawScreen(int var1, int var2, float var3) {
	drawDefaultBackground();
	drawCenteredString(fontRenderer, "Play Multiplayer", width / 2, height / 4 - 60 + 20, 16777215);
	drawString(fontRenderer, "Minecraft Multiplayer is currently not finished, but there", width / 2 - 140, height / 4 - 60 + 60 + 0, 10526880);
	drawString(fontRenderer, "is some buggy early testing going on.", width / 2 - 140, height / 4 - 60 + 60 + 9, 10526880);
	drawString(fontRenderer, "Enter the IP of a server to connect to it:", width / 2 - 140, height / 4 - 60 + 60 + 36, 10526880);
	int var4 = width / 2 - 100;
	int var5 = height / 4 - 10 + 50 + 18;
	int var6 = 200;
	int var7 = 20;
	drawRect(var4 - 1, var5 - 1, var4 + var6 + 1, var5 + var7 + 1, -6250336);
	drawRect(var4, var5, var4 + var6, var5 + var7, -16777216);
	drawString(fontRenderer, m_serverAddress + (m_parentScreen / 6 % 2 == 0 ? "_" : ""), var4 + 4, var5 + (var7 - 8) / 2, 14737632);
	GuiScreen::drawScreen(var1, var2, var3);
}
