#include "stdafx.h"
// Auto-generated from GameSettings.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GameSettings.h"
#include "Common.h"
#include "Minecraft.h"
#include <string>
#include "KeyBinding.h"

std::string GameSettings::s_RENDER_DISTANCES = NULL /* PORT: string array init {"FAR", "NORMAL", "SHORT", "TINY"} */;
std::string GameSettings::s_DIFFICULTIES = NULL /* PORT: string array init {"Peaceful", "Easy", "Normal", "Hard"} */;

GameSettings::GameSettings()
	: m_music(true)
	, m_sound(true)
	, m_invertMouse(false)
	, m_showFPS(false)
	, m_renderDistance(0)
	, m_fancyGraphics(true)
	, m_anaglyph(false)
	, m_limitFramerate(false)
	, m_keyBindForward(KeyBinding("Forward", 17))
	, m_keyBindLeft(KeyBinding("Left", 30))
	, m_keyBindBack(KeyBinding("Back", 31))
	, m_keyBindRight(KeyBinding("Right", 32))
	, m_keyBindJump(KeyBinding("Jump", 57))
	, m_keyBindInventory(KeyBinding("Inventory", 23))
	, m_keyBindDrop(KeyBinding("Drop", 16))
	, m_keyBindChat(KeyBinding("Chat", 20))
	, m_keyBindToggleFog(KeyBinding("Toggle fog", 33))
	, m_keyBindSave(KeyBinding("Save location", 28))
	, m_keyBindLoad(KeyBinding("Load location", 19))
	, m_keyBindings(NULL /* PORT: KeyBinding array init {keyBindForward, keyBindLeft, keyBindBack, keyBindRight, keyBindJump, keyBindDrop, keyBindInventory, keyBindChat, keyBindToggleFog, keyBindSave, keyBindLoad} */)
	, mc(NULL)
	, m_optionsFile(NULL)
	, m_numberOfOptions(9)
	, m_difficulty(2)
{
}

GameSettings::GameSettings(Minecraft* var1, std::string /* PORT: was java.io.File */* var2)
	: m_music(true)
	, m_sound(true)
	, m_invertMouse(false)
	, m_showFPS(false)
	, m_renderDistance(0)
	, m_fancyGraphics(true)
	, m_anaglyph(false)
	, m_limitFramerate(false)
	, m_keyBindForward(KeyBinding("Forward", 17))
	, m_keyBindLeft(KeyBinding("Left", 30))
	, m_keyBindBack(KeyBinding("Back", 31))
	, m_keyBindRight(KeyBinding("Right", 32))
	, m_keyBindJump(KeyBinding("Jump", 57))
	, m_keyBindInventory(KeyBinding("Inventory", 23))
	, m_keyBindDrop(KeyBinding("Drop", 16))
	, m_keyBindChat(KeyBinding("Chat", 20))
	, m_keyBindToggleFog(KeyBinding("Toggle fog", 33))
	, m_keyBindSave(KeyBinding("Save location", 28))
	, m_keyBindLoad(KeyBinding("Load location", 19))
	, m_keyBindings(NULL /* PORT: KeyBinding array init {keyBindForward, keyBindLeft, keyBindBack, keyBindRight, keyBindJump, keyBindDrop, keyBindInventory, keyBindChat, keyBindToggleFog, keyBindSave, keyBindLoad} */)
	, mc(NULL)
	, m_optionsFile(NULL)
	, m_numberOfOptions(9)
	, m_difficulty(2)
{
		mc = var1;
		m_optionsFile = new File(var2, "options.txt");
		loadOptions();
}

GameSettings::~GameSettings() {
	delete[] m_keyBindings;
	m_keyBindings = NULL;
	mc = NULL;
	m_optionsFile = NULL;
}

const std::string& GameSettings::setKeyBindingString(int var1) {
  // STUBBED: return m_keyBindings[var1].keyDescription + ": " + Keyboard.getKeyName(m_keyBindings[var1].keyCode);  // XInput replaces Keyboard
}

void GameSettings::setKeyBinding(int var1, int var2) {
		m_keyBindings[var1].m_keyCode = var2;
		saveOptions();
}

void GameSettings::setOptionValue(int var1, int var2) {
		if(var1 == 0) {
			m_music = !m_music;
			mc->m_sndManager.onSoundOptionsChanged();
		}

		if(var1 == 1) {
			m_sound = !m_sound;
			mc->m_sndManager.onSoundOptionsChanged();
		}

		if(var1 == 2) {
			m_invertMouse = !m_invertMouse;
		}

		if(var1 == 3) {
			m_showFPS = !m_showFPS;
		}

		if(var1 == 4) {
			m_renderDistance = m_renderDistance + var2 & 3;
		}

		if(var1 == 5) {
			m_fancyGraphics = !m_fancyGraphics;
		}

		if(var1 == 6) {
			m_anaglyph = !m_anaglyph;
			// PORT-TODO: texture reload);
		}

		if(var1 == 7) {
			m_limitFramerate = !m_limitFramerate;
		}

		if(var1 == 8) {
			m_difficulty = m_difficulty + var2 & 3;
		}

		saveOptions();
}

const std::string& GameSettings::setOptionString(int var1) {
		return var1 == 0 ? "Music: " + (m_music ? "ON" : "OFF") : (var1 == 1 ? "Sound: " + (m_sound ? "ON" : "OFF") : (var1 == 2 ? "Invert mouse: " + (m_invertMouse ? "ON" : "OFF") : (var1 == 3 ? "Show FPS: " + (m_showFPS ? "ON" : "OFF") : (var1 == 4 ? "Render distance: " + s_RENDER_DISTANCES[m_renderDistance] : (var1 == 5 ? "View bobbing: " + (m_fancyGraphics ? "ON" : "OFF") : (var1 == 6 ? "3D Anaglyph: " + (m_anaglyph ? "ON" : "OFF") : (var1 == 7 ? "Limit framerate: " + (m_limitFramerate ? "ON" : "OFF") : (var1 == 8 ? "Difficulty: " + s_DIFFICULTIES[m_difficulty] : ""))))))));
}

void GameSettings::loadOptions() {
  { // was try
			if(m_optionsFile->exists()) {
				// PORT-TODO: replace Java I/O — BufferedReader var1

				while(true) {
					// PORT-TODO: std::string var2 = var1->readLine();
					if(var2 == NULL) {
						// PORT-TODO: var1->close();
						return;
					}

					std::string[] var5 = var2.split(":");
					if(var5[0] == "m_music") {
						m_music = var5[1] == "true";
					}

					if(var5[0] == "m_sound") {
						m_sound = var5[1] == "true";
					}

					if(var5[0] == "invertYMouse") {
						m_invertMouse = var5[1] == "true";
					}

					if(var5[0] == "showFrameRate") {
						m_showFPS = var5[1] == "true";
					}

					if(var5[0] == "viewDistance") {
						m_renderDistance = atoi(var5[1].c_str());
					}

					if(var5[0] == "bobView") {
						m_fancyGraphics = var5[1] == "true";
					}

					if(var5[0] == "anaglyph3d") {
						m_anaglyph = var5[1] == "true";
					}

					if(var5[0] == "m_limitFramerate") {
						m_limitFramerate = var5[1] == "true";
					}

					if(var5[0] == "m_difficulty") {
						m_difficulty = atoi(var5[1].c_str());
					}

					for(int var3 = 0; var3 < m_keyBindings_count /* PORT: set this from allocation */; ++var3) {
						if(var5[0] == "key_" + m_keyBindings[var3].m_keyDescription) {
							m_keyBindings[var3].m_keyCode = atoi(var5[1].c_str());
						}
					}
				}
			}
		} { DbgPrintF("CATCH Exception\n"); // was catch(Exception var4)
			DbgPrintF("%s\n", "Failed to load options");
   // Removed Java stream/exception call: var4.printStackTrace();
		}
}

void GameSettings::saveOptions() {
  { // was try
			// PORT-TODO: replace Java I/O — PrintWriter var1
			// PORT-TODO: var1->println("m_music:" + m_music);
			// PORT-TODO: var1->println("m_sound:" + m_sound);
			// PORT-TODO: var1->println("invertYMouse:" + m_invertMouse);
			// PORT-TODO: var1->println("showFrameRate:" + m_showFPS);
			// PORT-TODO: var1->println("viewDistance:" + m_renderDistance);
			// PORT-TODO: var1->println("bobView:" + m_fancyGraphics);
			// PORT-TODO: var1->println("anaglyph3d:" + m_anaglyph);
			// PORT-TODO: var1->println("m_limitFramerate:" + m_limitFramerate);
			// PORT-TODO: var1->println("m_difficulty:" + m_difficulty);

			for(int var2 = 0; var2 < m_keyBindings_count /* PORT: set this from allocation */; ++var2) {
				// PORT-TODO: var1->println("key_" + m_keyBindings[var2].m_keyDescription + ":" + m_keyBindings[var2].m_keyCode);
			}

			// PORT-TODO: var1->close();
		} { DbgPrintF("CATCH Exception\n"); // was catch(Exception var3)
			DbgPrintF("%s\n", "Failed to save options");
   // Removed Java stream/exception call: var3.printStackTrace();
		}
}
