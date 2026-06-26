#ifndef LOGOEFFECTRANDOMIZER_H
#define LOGOEFFECTRANDOMIZER_H

// Auto-generated from LogoEffectRandomizer.java
// Target: Xbox 360 / C++03 / DirectX 9

class GuiMainMenu;

class LogoEffectRandomizer {
public:
	LogoEffectRandomizer();
	LogoEffectRandomizer(GuiMainMenu* var1, int var2, int var3);
	virtual ~LogoEffectRandomizer();

	double m_field_1312_a;
	double m_field_1311_b;
	double m_field_1314_c;

	virtual void func_875_a();

public: // was private
	GuiMainMenu* m_mainMenu;

};

#endif // LOGOEFFECTRANDOMIZER_H