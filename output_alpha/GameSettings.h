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
	GameSettings(Minecraft* var1, std::string /* PORT: was java.io.File */* var2);
	GameSettings();
	virtual ~GameSettings();

	float m_musicVolume;
	float m_soundVolume;
	float m_mouseSensitivity;
	bool m_invertMouse;
	int m_renderDistance;
	bool m_viewBobbing;
	bool m_anaglyph;
	bool m_limitFramerate;
	bool m_fancyGraphics;
	std::string m_skin;
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
	KeyBinding m_keyBindChat;
	bool m_hasKeyBindChat;
	KeyBinding m_keyBindToggleFog;
	bool m_hasKeyBindToggleFog;
	KeyBinding m_keyBindSneak;
	bool m_hasKeyBindSneak;
	KeyBinding* m_keyBindings;
	int m_numberOfOptions;
	int m_difficulty;
	bool m_thirdPersonView;
	std::string m_field_12259_z;

	virtual const std::string& getKeyBinding(int var1);
	virtual void setKeyBinding(int var1, int var2);
	virtual void setOptionFloatValue(int var1, float var2);
	virtual void setOptionValue(int var1, int var2);
	virtual int getOptionControlType(int var1);
	virtual float getOptionFloatValue(int var1);
	virtual const std::string& getOptionDisplayString(int var1);
	virtual void loadOptions();
	virtual void saveOptions();

public: // was protected
	Minecraft* mc;

public: // was private
	static std::string* s_RENDER_DISTANCES;
	static std::string* s_DIFFICULTY_LEVELS;
	File* m_optionsFile;

	float parseFloat(const std::string& var1);

public:  // array-length companions
	int m_keyBindings_count;

};

#endif // GAMESETTINGS_H