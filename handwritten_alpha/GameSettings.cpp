#include "stdafx.h"
// Hand-ported from GameSettings.java (Minecraft Alpha v1.2.6)
// Target: Xbox 360 / C++03 / DirectX 9
//
// Ported against Alpha's converted GameSettings.h (m_musicVolume/m_soundVolume floats,
// m_mouseSensitivity, m_viewBobbing, m_anaglyph, m_limitFramerate, m_fancyGraphics,
// m_difficulty, m_skin, 10 key bindings incl. Sneak). The auto-converted .cpp had
// ~34 errors: it mixed in indev field names (m_music/m_sound/m_showFPS/m_hasKeyBindings,
// bare keyBindForward), Java String '+', Float.parseFloat, NULL string arrays, a missing
// RenderGlobal include, and const-ref returns of temporaries. Logic adapted from indev's
// working GameSettings.cpp and the original Java; I/O uses the JavaIO File shim.

#include "GameSettings.h"
#include "Common.h"
#include "File.h"
#include "JavaIO.h"
#include "SoundManager.h"
#include "RenderGlobal.h"
#include "Minecraft.h"
#include "KeyBinding.h"
#include <string>
#include <sstream>
#include <cstdlib>

// --- Static label tables (Java String[]) --------------------------------------
// Header declares these as std::string* (a heap array the converter collapsed to NULL).
// Allocate + fill them so render-distance / difficulty labels index correctly.
std::string* GameSettings::s_RENDER_DISTANCES = NULL;
std::string* GameSettings::s_DIFFICULTY_LEVELS = NULL;

// One-time init of the label tables (idempotent).
static void ensureLabelTables() {
	if (GameSettings::s_RENDER_DISTANCES == NULL) {
		GameSettings::s_RENDER_DISTANCES = new std::string[4];
		GameSettings::s_RENDER_DISTANCES[0] = "FAR";
		GameSettings::s_RENDER_DISTANCES[1] = "NORMAL";
		GameSettings::s_RENDER_DISTANCES[2] = "SHORT";
		GameSettings::s_RENDER_DISTANCES[3] = "TINY";
	}
	if (GameSettings::s_DIFFICULTY_LEVELS == NULL) {
		GameSettings::s_DIFFICULTY_LEVELS = new std::string[4];
		GameSettings::s_DIFFICULTY_LEVELS[0] = "Peaceful";
		GameSettings::s_DIFFICULTY_LEVELS[1] = "Easy";
		GameSettings::s_DIFFICULTY_LEVELS[2] = "Normal";
		GameSettings::s_DIFFICULTY_LEVELS[3] = "Hard";
	}
}

// Scratch buffer backing the const std::string& returns (getKeyBinding /
// getOptionDisplayString). The header mandates by-reference returns; callers consume
// the result immediately (single-threaded UI), so a file-scope buffer is safe.
static std::string s_returnBuf;

// PORT: rebuild the Java `keyBindings = new KeyBinding[]{ ... }` aggregate the converter
// dropped (it left m_keyBindings = NULL). Order MUST match the Java field order so saved
// key_* lines reload onto the right action. Free function (header has no helper decl, and
// must compile unmodified).
static void buildKeyBindings(GameSettings* gs) {
	delete[] gs->m_keyBindings;
	gs->m_keyBindings_count = 10;
	gs->m_keyBindings = new KeyBinding[gs->m_keyBindings_count];
	gs->m_keyBindings[0] = gs->m_keyBindForward;
	gs->m_keyBindings[1] = gs->m_keyBindLeft;
	gs->m_keyBindings[2] = gs->m_keyBindBack;
	gs->m_keyBindings[3] = gs->m_keyBindRight;
	gs->m_keyBindings[4] = gs->m_keyBindJump;
	gs->m_keyBindings[5] = gs->m_keyBindSneak;
	gs->m_keyBindings[6] = gs->m_keyBindDrop;
	gs->m_keyBindings[7] = gs->m_keyBindInventory;
	gs->m_keyBindings[8] = gs->m_keyBindChat;
	gs->m_keyBindings[9] = gs->m_keyBindToggleFog;
}

GameSettings::GameSettings(Minecraft* var1, std::string /* PORT: was java.io.File */* var2)
	: m_musicVolume(1.0f)
	, m_soundVolume(1.0f)
	, m_mouseSensitivity(0.5f)
	, m_invertMouse(false)
	, m_renderDistance(0)
	, m_viewBobbing(true)
	, m_anaglyph(false)
	, m_limitFramerate(false)
	, m_fancyGraphics(true)
	, m_skin("Default")
	, m_keyBindForward(KeyBinding("Forward", 17))
	, m_keyBindLeft(KeyBinding("Left", 30))
	, m_keyBindBack(KeyBinding("Back", 31))
	, m_keyBindRight(KeyBinding("Right", 32))
	, m_keyBindJump(KeyBinding("Jump", 57))
	, m_keyBindInventory(KeyBinding("Inventory", 23))
	, m_keyBindDrop(KeyBinding("Drop", 16))
	, m_keyBindChat(KeyBinding("Chat", 20))
	, m_keyBindToggleFog(KeyBinding("Toggle fog", 33))
	, m_keyBindSneak(KeyBinding("Sneak", 42))
	, m_keyBindings(NULL)
	, m_numberOfOptions(10)
	, m_difficulty(2)
	, m_thirdPersonView(false)
	, m_field_12259_z("")
	, mc(NULL)
	, m_optionsFile(NULL)
	, m_keyBindings_count(0)
{
	ensureLabelTables();
	buildKeyBindings(this);
	mc = var1;
	m_optionsFile = new File(*var2, "options.txt");
	loadOptions();
}

GameSettings::GameSettings()
	: m_musicVolume(1.0f)
	, m_soundVolume(1.0f)
	, m_mouseSensitivity(0.5f)
	, m_invertMouse(false)
	, m_renderDistance(0)
	, m_viewBobbing(true)
	, m_anaglyph(false)
	, m_limitFramerate(false)
	, m_fancyGraphics(true)
	, m_skin("Default")
	, m_keyBindForward(KeyBinding("Forward", 17))
	, m_keyBindLeft(KeyBinding("Left", 30))
	, m_keyBindBack(KeyBinding("Back", 31))
	, m_keyBindRight(KeyBinding("Right", 32))
	, m_keyBindJump(KeyBinding("Jump", 57))
	, m_keyBindInventory(KeyBinding("Inventory", 23))
	, m_keyBindDrop(KeyBinding("Drop", 16))
	, m_keyBindChat(KeyBinding("Chat", 20))
	, m_keyBindToggleFog(KeyBinding("Toggle fog", 33))
	, m_keyBindSneak(KeyBinding("Sneak", 42))
	, m_keyBindings(NULL)
	, m_numberOfOptions(10)
	, m_difficulty(2)
	, m_thirdPersonView(false)
	, m_field_12259_z("")
	, mc(NULL)
	, m_optionsFile(NULL)
	, m_keyBindings_count(0)
{
	ensureLabelTables();
	buildKeyBindings(this);
}

GameSettings::~GameSettings() {
	delete[] m_keyBindings;
	m_keyBindings = NULL;
	mc = NULL;
	m_optionsFile = NULL;
}

const std::string& GameSettings::getKeyBinding(int var1) {
	if (m_keyBindings == NULL || var1 < 0 || var1 >= m_keyBindings_count) {
		s_returnBuf.clear();
		return s_returnBuf;
	}
	return m_keyBindings[var1].m_keyDescription;
}

void GameSettings::setKeyBinding(int var1, int var2) {
	if (m_keyBindings != NULL && var1 >= 0 && var1 < m_keyBindings_count) {
		m_keyBindings[var1].m_keyCode = var2;
		saveOptions();
	}
}

void GameSettings::setOptionFloatValue(int var1, float var2) {
	if (var1 == 0) {
		m_musicVolume = var2;
		if (mc != NULL && mc->m_sndManager != NULL) mc->m_sndManager->onSoundOptionsChanged();
	}

	if (var1 == 1) {
		m_soundVolume = var2;
		if (mc != NULL && mc->m_sndManager != NULL) mc->m_sndManager->onSoundOptionsChanged();
	}

	if (var1 == 3) {
		m_mouseSensitivity = var2;
	}

	saveOptions();
}

void GameSettings::setOptionValue(int var1, int var2) {
	if (var1 == 2) {
		m_invertMouse = !m_invertMouse;
	}

	if (var1 == 4) {
		m_renderDistance = (m_renderDistance + var2) & 3;
	}

	if (var1 == 5) {
		m_viewBobbing = !m_viewBobbing;
	}

	if (var1 == 6) {
		m_anaglyph = !m_anaglyph;
		// PORT: original reloads all textures here (anaglyph swaps the texture pack).
	}

	if (var1 == 7) {
		m_limitFramerate = !m_limitFramerate;
	}

	if (var1 == 8) {
		m_difficulty = (m_difficulty + var2) & 3;
	}

	if (var1 == 9) {
		m_fancyGraphics = !m_fancyGraphics;
		if (mc != NULL && mc->m_field_6323_f != NULL) mc->m_field_6323_f->func_958_a();
	}

	saveOptions();
}

int GameSettings::getOptionControlType(int var1) {
	return var1 == 0 ? 1 : (var1 == 1 ? 1 : (var1 == 3 ? 1 : 0));
}

float GameSettings::getOptionFloatValue(int var1) {
	return var1 == 0 ? m_musicVolume : (var1 == 1 ? m_soundVolume : (var1 == 3 ? m_mouseSensitivity : 0.0F));
}

const std::string& GameSettings::getOptionDisplayString(int var1) {
	std::ostringstream os;

	switch (var1) {
	case 0:
		os << "Music: ";
		if (m_musicVolume > 0.0F) os << (int)(m_musicVolume * 100.0F) << "%"; else os << "OFF";
		break;
	case 1:
		os << "Sound: ";
		if (m_soundVolume > 0.0F) os << (int)(m_soundVolume * 100.0F) << "%"; else os << "OFF";
		break;
	case 2:
		os << "Invert mouse: " << (m_invertMouse ? "ON" : "OFF");
		break;
	case 3:
		if (m_mouseSensitivity == 0.0F) os << "Sensitivity: *yawn*";
		else if (m_mouseSensitivity == 1.0F) os << "Sensitivity: HYPERSPEED!!!";
		else os << "Sensitivity: " << (int)(m_mouseSensitivity * 200.0F) << "%";
		break;
	case 4:
		os << "Render distance: " << s_RENDER_DISTANCES[m_renderDistance & 3];
		break;
	case 5:
		os << "View bobbing: " << (m_viewBobbing ? "ON" : "OFF");
		break;
	case 6:
		os << "3D Anaglyph: " << (m_anaglyph ? "ON" : "OFF");
		break;
	case 7:
		os << "Limit framerate: " << (m_limitFramerate ? "ON" : "OFF");
		break;
	case 8:
		os << "Difficulty: " << s_DIFFICULTY_LEVELS[m_difficulty & 3];
		break;
	case 9:
		os << "Graphics: " << (m_fancyGraphics ? "FANCY" : "FAST");
		break;
	default:
		break;
	}

	s_returnBuf = os.str();
	return s_returnBuf;
}

void GameSettings::loadOptions() {
	if (m_optionsFile == NULL || !m_optionsFile->exists()) return;

	FileInputStream _fis(m_optionsFile);
	std::string _all;
	{ int _c; while ((_c = _fis.read()) >= 0) _all += (char)_c; }

	size_t _p = 0;
	while (_p < _all.size()) {
		size_t _nl = _all.find('\n', _p);
		std::string var2 = _all.substr(_p, (_nl == std::string::npos ? _all.size() : _nl) - _p);
		_p = (_nl == std::string::npos) ? _all.size() : _nl + 1;
		if (!var2.empty() && var2[var2.size() - 1] == '\r') var2.erase(var2.size() - 1);

		size_t _colon = var2.find(':');
		if (_colon == std::string::npos) continue;
		std::string key = var2.substr(0, _colon);
		std::string val = var2.substr(_colon + 1);

		if (key == "music") m_musicVolume = parseFloat(val);
		if (key == "sound") m_soundVolume = parseFloat(val);
		if (key == "mouseSensitivity") m_mouseSensitivity = parseFloat(val);
		if (key == "invertYMouse") m_invertMouse = (val == "true");
		if (key == "viewDistance") m_renderDistance = atoi(val.c_str());
		if (key == "bobView") m_viewBobbing = (val == "true");
		if (key == "anaglyph3d") m_anaglyph = (val == "true");
		if (key == "limitFramerate") m_limitFramerate = (val == "true");
		if (key == "difficulty") m_difficulty = atoi(val.c_str());
		if (key == "fancyGraphics") m_fancyGraphics = (val == "true");
		if (key == "skin") m_skin = val;

		for (int var3 = 0; var3 < m_keyBindings_count; ++var3) {
			if (key == "key_" + m_keyBindings[var3].m_keyDescription) {
				m_keyBindings[var3].m_keyCode = atoi(val.c_str());
			}
		}
	}
}

// PORT: Java did `Float.parseFloat`; "true"/"false" map to 1.0/0.0 (old options.txt
// stored the volumes as booleans before Alpha switched them to floats).
float GameSettings::parseFloat(const std::string& var1) {
	if (var1 == "true") return 1.0F;
	if (var1 == "false") return 0.0F;
	return (float)atof(var1.c_str());
}

void GameSettings::saveOptions() {
	if (m_optionsFile == NULL) return;

	std::ostringstream os;
	os << "music:" << m_musicVolume << "\n";
	os << "sound:" << m_soundVolume << "\n";
	os << "mouseSensitivity:" << m_mouseSensitivity << "\n";
	os << "invertYMouse:" << (m_invertMouse ? "true" : "false") << "\n";
	os << "viewDistance:" << m_renderDistance << "\n";
	os << "bobView:" << (m_viewBobbing ? "true" : "false") << "\n";
	os << "anaglyph3d:" << (m_anaglyph ? "true" : "false") << "\n";
	os << "limitFramerate:" << (m_limitFramerate ? "true" : "false") << "\n";
	os << "difficulty:" << m_difficulty << "\n";
	os << "fancyGraphics:" << (m_fancyGraphics ? "true" : "false") << "\n";
	os << "skin:" << m_skin << "\n";
	for (int var2 = 0; var2 < m_keyBindings_count; ++var2) {
		os << "key_" << m_keyBindings[var2].m_keyDescription << ":" << m_keyBindings[var2].m_keyCode << "\n";
	}

	std::string _o = os.str();
	FileOutputStream _fos(m_optionsFile);
	if (!_o.empty()) _fos.write((const unsigned char*)_o.data(), (int)_o.size());
	_fos.close();
}
