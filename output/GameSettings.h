#ifndef GAMESETTINGS_H
#define GAMESETTINGS_H

// Auto-generated from GameSettings.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "KeyBinding.h"
#include <string>

class Minecraft;

class File;
class GameSettings {
public:
	GameSettings();
	GameSettings(Minecraft* var1, std::string /* PORT: was java.io.File */* var2);
	virtual ~GameSettings();

	bool m_music;
	bool m_sound;
	bool m_invertMouse;
	bool m_showFPS;
	int m_renderDistance;
	bool m_fancyGraphics;
	bool m_anaglyph;
	bool m_limitFramerate;
	KeyBinding m_keyBindForward;
	bool m_hasKeyBindForward;
	KeyBinding m_keyBindLeft;
	bool m_hasKeyBindLeft;
	KeyBinding m_keyBindBack;
	bool m_hasKeyBindBack;
	KeyBinding m_keyBindRight;
	bool m_hasKeyBindRight;
	KeyBinding m_keyBindJump;
	bool m_hasKeyBindJump;
	KeyBinding m_keyBindInventory;
	bool m_hasKeyBindInventory;
	KeyBinding m_keyBindDrop;
	bool m_hasKeyBindDrop;
	KeyBinding m_keyBindToggleFog;
	bool m_hasKeyBindToggleFog;
	KeyBinding m_keyBindSave;
	bool m_hasKeyBindSave;
	KeyBinding m_keyBindLoad;
	bool m_hasKeyBindLoad;
	KeyBinding* m_keyBindings;
	int m_numberOfOptions;
	int m_difficulty;

	virtual const std::string& setKeyBindingString(int var1);
	virtual void setKeyBinding(int var1, int var2);
	virtual void setOptionValue(int var1, int var2);
	virtual const std::string& setOptionString(int var1);
	virtual void saveOptions();

public: // was private
	static std::string s_RENDER_DISTANCES;
	static std::string s_DIFFICULTIES;
	KeyBinding m_keyBindChat;
	bool m_hasKeyBindChat;
	Minecraft* mc;
	File* m_optionsFile;

	void loadOptions();

public:  // array-length companions
	int m_keyBindings_count;

};

#endif // GAMESETTINGS_H