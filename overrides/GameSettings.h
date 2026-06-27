#ifndef GAMESETTINGS_H
#define GAMESETTINGS_H

// Auto-generated from GameSettings.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class File;
class KeyBinding;
class Minecraft;

class GameSettings {
public:
	GameSettings(Minecraft* var1, File* var2);
	GameSettings();
	virtual ~GameSettings();

	float musicVolume;
	float soundVolume;
	float mouseSensitivity;
	bool invertMouse;
	int renderDistance;
	bool viewBobbing;
	bool anaglyph;
	bool limitFramerate;
	bool fancyGraphics;
	JavaString skin;
	KeyBinding* keyBindForward;
	KeyBinding* keyBindLeft;
	KeyBinding* keyBindBack;
	KeyBinding* keyBindRight;
	KeyBinding* keyBindJump;
	KeyBinding* keyBindInventory;
	KeyBinding* keyBindDrop;
	KeyBinding* keyBindChat;
	KeyBinding* keyBindToggleFog;
	KeyBinding* keyBindSneak;
	JArray<KeyBinding* > keyBindings;
	Minecraft* mc;
	File* optionsFile;
	int numberOfOptions;
	int difficulty;
	bool thirdPersonView;
	JavaString lastServer;
	static JArray<JavaString > s_RENDER_DISTANCES;
	static JArray<JavaString > s_DIFFICULTY_LEVELS;

	virtual JavaString getKeyBinding(int var1);
	virtual void setKeyBinding(int var1, int var2);
	virtual void setOptionFloatValue(int var1, float var2);
	virtual void setOptionValue(int var1, int var2);
	virtual int getOptionControlType(int var1);
	virtual float getOptionFloatValue(int var1);
	virtual JavaString getOptionDisplayString(int var1);
	virtual void loadOptions();
	virtual float parseFloat(const JavaString& var1);
	virtual void saveOptions();

private:
	// owns heap; copying disabled so a copy cannot double-free (Rule of Three)
	GameSettings(const GameSettings&);
	GameSettings& operator=(const GameSettings&);
};

#endif // GAMESETTINGS_H