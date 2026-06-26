#include "stdafx.h"
// Hand-written PORT override for GuiChat.java (Minecraft Alpha v1.2.6)
// ------------------------------------------------------------------------
// Started from the converter output (ConversionTool\output_alpha\GuiChat.cpp, ~14 errors) and
// fixed to compile against Alpha's GuiChat.h. GuiChat is Alpha-new (no indev equivalent), so the
// porting base is GuiScreen.h + the already-ported handwritten_alpha\GuiMainMenu.cpp.
//
// Notable converter fixes:
//   * "literal".indexOf(ch) is invalid C++ -> Common.h's javaIndexOf(string, char). The trailing
//     extended/Unicode glyphs in the Java allowed-char string aren't representable in the source
//     code page (and aren't reachable from XInput text input anyway) -> the allowed set is the
//     printable ASCII range, matching what the controller keyboard can actually emit.
//   * Java String '+' concat in drawScreen -> std::string '+' (Common.h toStr where a non-string
//     operand sneaks in).
//   * mc->m_ingameGUI->m_field_933_a is a std::string member (the last-clicked URL/tooltip), NOT a
//     pointer -> the Java "!= null" guard becomes a "non-empty" check.
//   * 'mc', 'width', 'height', 'fontRenderer' are inherited from GuiScreen -> pull in the real
//     Minecraft / EntityPlayerSP / GuiIngame / FontRenderer types so the '->' chains resolve.

#include "GuiChat.h"
#include "Common.h"
#include "Minecraft.h"
#include "EntityPlayerSP.h"
#include "GuiIngame.h"
#include "FontRenderer.h"
#include <string>

GuiChat::GuiChat()
	: m_message("")
	, m_updateCounter(0)
{
}

GuiChat::~GuiChat() {
	// No dynamic resources
}

void GuiChat::initGui() {
	// PORT: Keyboard.enableRepeatEvents(true) -> XInput text entry replaces LWJGL Keyboard.
}

void GuiChat::onGuiClosed() {
	// PORT: Keyboard.enableRepeatEvents(false) -> XInput text entry replaces LWJGL Keyboard.
}

void GuiChat::updateScreen() {
	++m_updateCounter;
}

// The printable ASCII characters Alpha allows in chat input. Java's allowed-char string also
// listed the extended Latin/CP437 glyphs, but those are unreachable from the 360 controller
// keyboard and aren't representable in this source code page -> printable ASCII is the portable set.
static const char* CHAT_ALLOWED_CHARS =
	" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_'abcdefghijklmnopqrstuvwxyz{|}~";

void GuiChat::keyTyped(char var1, int var2) {
	if(var2 == 1) {
		mc->displayGuiScreen(NULL);
	} else if(var2 == 28) {
		std::string var3 = javaTrim(m_message);
		if(var3.length() > 0) {
			mc->thePlayer->sendChatMessage(javaTrim(m_message));
		}

		mc->displayGuiScreen(NULL);
	} else {
		if(var2 == 14 && m_message.length() > 0) {
			m_message = m_message.substr(0, m_message.length() - 1);
		}

		if(javaIndexOf(CHAT_ALLOWED_CHARS, var1) >= 0 && m_message.length() < 100) {
			m_message = m_message + var1;
		}

	}
}

void GuiChat::drawScreen(int var1, int var2, float var3) {
	drawRect(2, height - 14, width - 2, height - 2, (int)((-2147483647) - 1));
	std::string var4 = std::string("> ") + m_message + (m_updateCounter / 6 % 2 == 0 ? "_" : "");
	drawString(fontRenderer, var4, 4, height - 12, 14737632);
}

void GuiChat::mouseClicked(int var1, int var2, int var3) {
	if(var3 == 0 && mc->m_ingameGUI->m_field_933_a.length() > 0) {
		if(m_message.length() > 0 && !javaEndsWith(m_message, " ")) {
			m_message = m_message + " ";
		}

		m_message = m_message + mc->m_ingameGUI->m_field_933_a;
		unsigned int var4 = 100;
		if(m_message.length() > var4) {
			m_message = m_message.substr(0, var4);
		}
	}

}
