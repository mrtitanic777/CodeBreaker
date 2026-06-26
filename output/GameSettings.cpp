#include "stdafx.h"
// Auto-generated from GameSettings.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GameSettings.h"
#include "Common.h"
#include "File.h"
#include "SoundManager.h"
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
		m_optionsFile = new File(*var2, "options.txt");
		loadOptions();
}

GameSettings::~GameSettings() {
	delete[] m_keyBindings;
	m_keyBindings = NULL;
	mc = NULL;
	m_optionsFile = NULL;
}

const std::string& GameSettings::setKeyBindingString(int var1) {
	return m_keyBindings[var1].m_keyDescription;
}

void GameSettings::setKeyBinding(int var1, int var2) {
		m_keyBindings[var1].m_keyCode = var2;
		saveOptions();
}

void GameSettings::setOptionValue(int var1, int var2) {
		if(var1 == 0) {
			m_music = !m_music;
			mc->m_sndManager->onSoundOptionsChanged();
		}

		if(var1 == 1) {
			m_sound = !m_sound;
			mc->m_sndManager->onSoundOptionsChanged();
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
		return var1 == 0 ? std::string("Music: ") + (m_music ? "ON" : "OFF") : (var1 == 1 ? std::string("Sound: ") + (m_sound ? "ON" : "OFF") : (var1 == 2 ? std::string("Invert mouse: ") + (m_invertMouse ? "ON" : "OFF") : (var1 == 3 ? std::string("Show FPS: ") + (m_showFPS ? "ON" : "OFF") : (var1 == 4 ? std::string("Render distance: ") + s_RENDER_DISTANCES[m_renderDistance] : (var1 == 5 ? std::string("View bobbing: ") + (m_fancyGraphics ? "ON" : "OFF") : (var1 == 6 ? std::string("3D Anaglyph: ") + (m_anaglyph ? "ON" : "OFF") : (var1 == 7 ? std::string("Limit framerate: ") + (m_limitFramerate ? "ON" : "OFF") : (var1 == 8 ? std::string("Difficulty: ") + s_DIFFICULTIES[m_difficulty] : ""))))))));
}

void GameSettings::loadOptions() {
	if (m_optionsFile == NULL || !m_optionsFile->exists()) return;
	FileInputStream _fis(m_optionsFile);
	std::string _all; { int _c; while ((_c = _fis.read()) >= 0) _all += (char)_c; }
	size_t _p = 0;
	while (_p < _all.size()) {
		size_t _nl = _all.find('\n', _p);
		std::string var2 = _all.substr(_p, (_nl == std::string::npos ? _all.size() : _nl) - _p);
		_p = (_nl == std::string::npos) ? _all.size() : _nl + 1;
		if (!var2.empty() && var2[var2.size() - 1] == '\r') var2.erase(var2.size() - 1);
		size_t _colon = var2.find(':');
		if (_colon == std::string::npos) continue;
		std::string var5[2]; var5[0] = var2.substr(0, _colon); var5[1] = var2.substr(_colon + 1);
		if (var5[0] == "m_music") m_music = var5[1] == "true";
		if (var5[0] == "m_sound") m_sound = var5[1] == "true";
		if (var5[0] == "invertYMouse") m_invertMouse = var5[1] == "true";
		if (var5[0] == "showFrameRate") m_showFPS = var5[1] == "true";
		if (var5[0] == "viewDistance") m_renderDistance = atoi(var5[1].c_str());
		if (var5[0] == "bobView") m_fancyGraphics = var5[1] == "true";
		if (var5[0] == "anaglyph3d") m_anaglyph = var5[1] == "true";
		if (var5[0] == "m_limitFramerate") m_limitFramerate = var5[1] == "true";
		if (var5[0] == "m_difficulty") m_difficulty = atoi(var5[1].c_str());
		for (int var3 = 0; var3 < m_keyBindings_count; ++var3)
			if (var5[0] == "key_" + m_keyBindings[var3].m_keyDescription)
				m_keyBindings[var3].m_keyCode = atoi(var5[1].c_str());
	}
}

void GameSettings::saveOptions() {
	if (m_optionsFile == NULL) return;
	FileOutputStream _fos(m_optionsFile);
	std::string _o;
	_o += "m_music:" + std::string(m_music ? "true" : "false") + "\n";
	_o += "m_sound:" + std::string(m_sound ? "true" : "false") + "\n";
	_o += "invertYMouse:" + std::string(m_invertMouse ? "true" : "false") + "\n";
	_o += "showFrameRate:" + std::string(m_showFPS ? "true" : "false") + "\n";
	_o += "viewDistance:" + toStr(m_renderDistance) + "\n";
	_o += "bobView:" + std::string(m_fancyGraphics ? "true" : "false") + "\n";
	_o += "anaglyph3d:" + std::string(m_anaglyph ? "true" : "false") + "\n";
	_o += "m_limitFramerate:" + std::string(m_limitFramerate ? "true" : "false") + "\n";
	_o += "m_difficulty:" + toStr(m_difficulty) + "\n";
	for (int var2 = 0; var2 < m_keyBindings_count; ++var2)
		_o += "key_" + m_keyBindings[var2].m_keyDescription + ":" + toStr(m_keyBindings[var2].m_keyCode) + "\n";
	if (!_o.empty()) _fos.write((const unsigned char*)_o.data(), (int)_o.size());
	_fos.close();
}
