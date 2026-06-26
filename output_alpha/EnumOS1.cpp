#include "stdafx.h"
#include "EnumOS1.h"

EnumOS1* EnumOS1::s_linux = NULL;
EnumOS1* EnumOS1::s_solaris = NULL;
EnumOS1* EnumOS1::s_windows = NULL;
EnumOS1* EnumOS1::s_macos = NULL;
EnumOS1* EnumOS1::s_unknown = NULL;
EnumOS1* EnumOS1::values_[5] = {0};
const int EnumOS1::values_count = 5;

EnumOS1::EnumOS1() : ordinal(0) {}

void EnumOS1::initStatics()
{
	s_linux = new EnumOS1();
	s_linux->ordinal = 0; s_linux->nameStr = "linux"; values_[0] = s_linux;
	s_solaris = new EnumOS1();
	s_solaris->ordinal = 1; s_solaris->nameStr = "solaris"; values_[1] = s_solaris;
	s_windows = new EnumOS1();
	s_windows->ordinal = 2; s_windows->nameStr = "windows"; values_[2] = s_windows;
	s_macos = new EnumOS1();
	s_macos->ordinal = 3; s_macos->nameStr = "macos"; values_[3] = s_macos;
	s_unknown = new EnumOS1();
	s_unknown->ordinal = 4; s_unknown->nameStr = "unknown"; values_[4] = s_unknown;
}
