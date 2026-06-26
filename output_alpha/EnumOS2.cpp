#include "stdafx.h"
#include "EnumOS2.h"

EnumOS2* EnumOS2::s_a = NULL;
EnumOS2* EnumOS2::s_b = NULL;
EnumOS2* EnumOS2::s_c = NULL;
EnumOS2* EnumOS2::s_d = NULL;
EnumOS2* EnumOS2::s_unknown = NULL;
EnumOS2* EnumOS2::values_[5] = {0};
const int EnumOS2::values_count = 5;

EnumOS2::EnumOS2() : ordinal(0) {}

void EnumOS2::initStatics()
{
	s_a = new EnumOS2();
	s_a->ordinal = 0; s_a->nameStr = "a"; values_[0] = s_a;
	s_b = new EnumOS2();
	s_b->ordinal = 1; s_b->nameStr = "b"; values_[1] = s_b;
	s_c = new EnumOS2();
	s_c->ordinal = 2; s_c->nameStr = "c"; values_[2] = s_c;
	s_d = new EnumOS2();
	s_d->ordinal = 3; s_d->nameStr = "d"; values_[3] = s_d;
	s_unknown = new EnumOS2();
	s_unknown->ordinal = 4; s_unknown->nameStr = "unknown"; values_[4] = s_unknown;
}
