#ifndef LOGOEFFECTRANDOMIZER_H
#define LOGOEFFECTRANDOMIZER_H

// Auto-generated from LogoEffectRandomizer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class GuiMainMenu;

class LogoEffectRandomizer {
public:
	LogoEffectRandomizer(GuiMainMenu* var1, int var2, int var3);
	virtual ~LogoEffectRandomizer();

	double posY;
	double prevPosY;
	double motionY;
	GuiMainMenu* mainMenu;

	virtual void updatePosition();

	static void initStatics();
};

#endif // LOGOEFFECTRANDOMIZER_H